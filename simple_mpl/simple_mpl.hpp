#pragma once
#include <cctype>
#include <type_traits>
#include <utility>
#include <common\\common.h>

template<char ...>
struct ct_string {};

template<typename A, typename B>
struct ct_string_append;

template<char ... a, char ... b>
struct ct_string_append<ct_string<a...>, ct_string<b...>> {
	using type = ct_string<a..., b...>;
};

template <typename CharT, CharT ...s>
constexpr auto operator"" _s() {
	return ct_string<s...>();
}

#define CT_S(x) decltype(x ## _s)

template<typename T>
struct type_object {
	using type = T;
};

template<size_t n, typename T>
struct ct_str_get {
	constexpr static char value = '\0';
};

template<size_t n>
struct ct_str_get<n, ct_string<>> {
	constexpr static char value = '\0';
};

template<size_t n, char f, char ... r>
struct ct_str_get<n, ct_string<f, r ...>> {
	constexpr static char value = ct_str_get<n - 1, ct_string<r ...>>::value;
};

template<char f, char ... r>
struct ct_str_get<0ui64, ct_string<f, r ...>> {
	constexpr static char value = f;
};

template<typename A, typename B>
struct ct_str_eq {
	constexpr static bool value = false;
};

template<>
struct ct_str_eq<ct_string <>, ct_string <>> {
	constexpr static bool value = true;
};

template<char a, char ... b, char c, char ... d>
struct ct_str_eq<ct_string <a, b...>, ct_string <c, d...>> {
	constexpr static bool value = (a == c && ct_str_eq<ct_string <b...>, ct_string <d...>>::value);
};

template<typename A, typename B>
struct ct_str_cmp {
	constexpr static int value = 0;
};

template<>
struct ct_str_cmp<ct_string <>, ct_string <>> {
	constexpr static int value = 0;
};

template<char a, char ... b, char c, char ... d>
struct ct_str_cmp<ct_string <a, b...>, ct_string <c, d...>> {
	constexpr static int value = (a == c) ? (ct_str_cmp<ct_string <b...>, ct_string<d...>>::value) : ((int)a - (int)c);
};

template<typename T>
struct compile_time_lex_compare_ci {
	static constexpr int value(const char* const, const char* const) {
		return 0;
	}
};

template<>
struct compile_time_lex_compare_ci<ct_string<>> {
	static constexpr int value(const char* const, const char* const) {
		return 0;
	}
};

constexpr char to_lower_helper_char(const char c) {
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

template<char f, char ...r>
struct compile_time_lex_compare_ci<ct_string<f, r...>> {
	static constexpr int value(const char* const as, const char* const ae) {
		if (ascii_to_lower(*as) != to_lower_helper_char(f))
			return ascii_to_lower(*as) - (int)(to_lower_helper_char(f));
		else if (as + 1 != ae)
			return compile_time_lex_compare_ci<ct_string<r...>>::value(as + 1, ae);
		else
			return 0;
	}
};


template<typename ct_str>
int compile_time_str_compare_ci(const char* const as, const char* const ae) {
	if (auto ord = (int)(ae - as) - (int)(ct_str::length); ord != 0)
		return ord;
	return compile_time_lex_compare_ci<ct_str>::value(as, ae);
}

template<>
struct ct_string<> {
	constexpr static size_t length = 0ui64;
	using tail = ct_string<>;
	template<char c>
	using cons = ct_string<c>;


	template<typename T>
	constexpr static bool equals = ct_str_eq<ct_string<>, T>::value;

	constexpr ct_string() {}
};

template<typename A, typename B>
constexpr int ct_string_compare = A::length != B::length ? ((int)A::length - (int)B::length) : ct_str_cmp<A, B>::value;

template<char f, char ... c>
struct ct_string<f, c...> {
	constexpr static size_t length = 1ui64 + sizeof...(c);
	using tail = ct_string<c...>;
	template<char n>
	using cons = ct_string<n, f, c...>;

	template<typename O>
	static constexpr int compare = ct_string_compare<ct_string<f, c...>, O>;

	template<typename T>
	constexpr static bool equals = ct_str_eq<ct_string<f, c...>, T>::value;

	constexpr ct_string() {}
};

template<char ... c>
struct ct_trimmed;

template<char f, char ... c>
struct ct_trimmed<f, c...> {
	using parent_string = typename ct_trimmed<c...>::string;
	using string = typename parent_string::template cons<f>;
};

template<char ... c>
struct ct_trimmed<'\0', c...> {
	using string = ct_string<>;
};

#define MACRO_GET_1(str, i) \
    (sizeof(str) > (i) ? str[(i)] : 0)

#define MACRO_GET_4(str, i) \
    MACRO_GET_1(str, i+0),  \
    MACRO_GET_1(str, i+1),  \
    MACRO_GET_1(str, i+2),  \
    MACRO_GET_1(str, i+3)

#define MACRO_GET_16(str, i) \
    MACRO_GET_4(str, i+0),   \
    MACRO_GET_4(str, i+4),   \
    MACRO_GET_4(str, i+8),   \
    MACRO_GET_4(str, i+12)

#define MACRO_GET_64(str, i) \
    MACRO_GET_16(str, i+0),  \
    MACRO_GET_16(str, i+16), \
    MACRO_GET_16(str, i+32), \
    MACRO_GET_16(str, i+48)

#define MACRO_GET_STR(str) MACRO_GET_64(str, 0) //guard for longer strings
#define CT_STRING(str) typename ct_trimmed<MACRO_GET_STR(str)>::string

template<typename ...R>
struct first_of_s;

template<>
struct first_of_s<> {};

template<typename T, typename ...R>
struct first_of_s<T, R...> {
	using type = T;
};

template<typename ...R>
using first_of = typename first_of_s<R...>::type;

template<template <typename ...> typename TPL, typename ...R>
struct rest_of_s;

template<template <typename ...> typename TPL>
struct rest_of_s<TPL> {
	using type = TPL<>;
};

template<template <typename ...> typename TPL, typename T, typename ...R>
struct rest_of_s<TPL, T, R...> {
	using type = TPL<R...>;
};

template<template <typename ...> typename TPL, typename ...R>
using rest_of = typename rest_of_s<TPL, R ...>::type;

template<typename ... T>
struct type_list;

template<size_t n, typename T>
struct type_list_get;

template<size_t n>
struct type_list_get<n, type_list<>> {
	using type = void;
};

template<size_t n, typename A, typename ...B>
struct type_list_get<n, type_list<A, B...>> {
	using type = typename type_list_get<n - 1, type_list<B ...>>::type;
};

template<typename A, typename ...B>
struct type_list_get<0ui64, type_list<A, B...>> {
	using type = A;
};

template<size_t n, typename T>
using type_list_get_t = typename type_list_get<n, T>::type;


template<typename ...T>
struct merge_lists_s;

template<>
struct merge_lists_s<> {
	using type = type_list<>;
};

template<typename T>
struct merge_lists_s<T> {
	using type = T;
};

template<typename ...T, typename ...R>
struct merge_lists_s<type_list<T...>, type_list<R...>> {
	using type = type_list<T..., R...>;
};

template<typename T, typename ...R>
struct merge_lists_s<T, R...> {
	using type = merge_lists_s<T, typename merge_lists_s<R...>::type>;
};

template<typename ...T>
using merge_lists = typename merge_lists_s<T...>::type;

template<typename T>
struct flatten_lists_s;

template<typename ...T>
struct flatten_lists_s<type_list<T...>> {
	using type = typename merge_lists_s<T...>::type;
};

template<typename T>
using flatten_list = typename flatten_lists_s<T>::type;

template<template <typename> typename F, typename T>
struct apply_to_all_s;

template<template <typename> typename F, typename ...T>
struct apply_to_all_s<F, type_list<T ...>> {
	using type = type_list<typename F<T>::type ...>;
};

template<template <typename> typename F, typename T>
using apply_to_all = typename apply_to_all_s<F, T>::type;

template<size_t n, typename T>
struct head;

template<size_t n>
struct head<n, type_list<>>  {
	using type = type_list<>;
};

template<typename F, typename ...G>
struct head<1ui64, type_list<F, G...>> {
	using type = type_list<F>;
};

template<typename F, typename ...G>
struct head<0ui64, type_list<F, G...>> {
	using type = type_list<>;
};

template<size_t n, typename F, typename ...G>
struct head<n, type_list<F, G...>> {
	using rest_head = typename head<n - 1, type_list<G...>>::type;
	using type = typename rest_head::template cons<F>;
};

template<size_t n, typename T>
using head_t = typename head<n, T>::type;

template<size_t n, typename T>
struct rest;

template<size_t n>
struct rest<n, type_list<>> {
	using type = type_list<>;
};

template< typename ...G>
struct rest<0ui64, type_list<G...>> {
	using type = type_list<G...>;
};

template<typename F, typename ...G>
struct rest<1ui64, type_list<F, G...>> {
	using type = type_list<G...>;
};

template<size_t n, typename F, typename ...G>
struct rest<n, type_list<F, G...>> {
	using type = typename rest<n - 1, type_list<G...>>::type;
};

template<size_t n, typename T>
using rest_t = typename rest<n, T>::type;

template <typename T>
struct least;

template <typename T>
struct least<type_list<T>> {
	using type = T;
};

template <typename F, typename ...T>
struct least<type_list<F, T...>> {
	using type = std::conditional_t <
		ct_string_compare<typename F::first, typename least<type_list<T ...>>::type::first> < 0,
		F,
		typename least<type_list<T ...>>::type >;
};

template <typename T>
using least_t = typename least<T>::type;

template <typename T, typename KEY>
struct type_map_get {
	using type = void;
};

template <typename T, typename KEY>
struct type_map_get<type_list<T>, KEY> {
	using type = std::conditional_t<std::is_same_v<KEY, typename T::first>, typename T::second, void>;
};

template <typename F, typename ...T, typename KEY >
struct type_map_get<type_list<F, T...>, KEY> {
	using type = std::conditional_t <
		std::is_same_v<KEY, typename F::first>,
		typename F::second,
		typename type_map_get<type_list<T ...>, KEY>::type >;
};

template <typename T, typename KEY>
using type_map_get_t = typename type_map_get<T, KEY>::type;

template <typename T>
struct sorted;

template <>
struct sorted<type_list<>> {
	using type = type_list<>;
};

template <typename T>
struct sorted<type_list<T>> {
	using type = type_list<T>;
};

template<typename T, typename U>
struct splice_s;

template<typename ...T>
struct splice_s<type_list<>, type_list<T...>> {
	using type = type_list<T...>;
};
template<typename ...T>
struct splice_s<type_list<T...>, type_list<>> {
	using type = type_list<T...>;
};

template<typename A, typename B, typename ... T, typename ... U>
struct splice_s<type_list<A, T...>, type_list<B, U...>> {
	static constexpr auto type_hider() {
		if constexpr(ct_string_compare<typename A::first, typename B::first> < 0)
			return typename splice_s<type_list<T...>, type_list<B, U...>>::type::template cons<A>();
		else
			return typename splice_s<type_list<A, T...>, type_list<U...>>::type::template cons<B>();
	}
	using type = decltype(type_hider());
};

template<typename A, typename B>
using splice = typename splice_s<A, B>::type;

template <typename ...T>
struct sorted<type_list<T...>> {
	constexpr static auto partition_value = type_list<T...>::length / 2ui64;
	using type = splice<typename sorted<head_t<partition_value, type_list<T...>>>::type, typename sorted<rest_t<partition_value, type_list<T...>>>::type>;
};

template <typename T>
using sorted_t = typename sorted<T>::type;

template<typename A, typename B, typename C>
struct splice_type_list;

template< typename B, typename ... A, typename ... C>
struct splice_type_list<type_list<A...>, B, type_list<C...>> {
	using type = type_list<A..., B, C...>;
};

template<typename A, typename B, typename C>
using splice_type_list_t = typename splice_type_list<A, B, C>::type;

template <typename T>
struct map_functions;

template <typename ...T>
struct map_functions<type_list<T...>> {
	template<typename value_type>
	static value_type scan_ci(const char* const as, const char* const ae, value_type def) {
		if constexpr(sizeof...(T) == 0) {
			return def;
		} else if constexpr(sizeof...(T) == 1) {
			if (compile_time_str_compare_ci<typename first_of<T...>::first>(as, ae) == 0)
				return first_of<T...>::second::value;
			return def;
		} else {
			if (compile_time_str_compare_ci<typename first_of<T...>::first>(as, ae) == 0)
				return first_of<T...>::second::value;
			else
				return map_functions<rest_of<type_list, T...>>::scan_ci(as, ae, def);
		}
	}
	template<typename value_type>
	static value_type bt_scan_ci(const char* const as, const char* const ae, value_type def) {
		if constexpr(sizeof...(T) == 0) {
			return def;
		} else if constexpr(sizeof...(T) == 1) {
			if (compile_time_str_compare_ci<typename first_of<T...>::first>(as, ae) == 0) {
				return first_of<T...>::second::value;
			} else {
				return def;
			}
		} else {
			constexpr auto partition_value = type_list<T...>::length / 2ui64;
			using partition_pair = type_list_get_t<partition_value, type_list<T ...>>;

			const int compare_result = compile_time_str_compare_ci<typename partition_pair::first>(as, ae);

			if (compare_result == 0) {
				return partition_pair::second::value;
			} else if (compare_result < 0) {
				if constexpr(partition_value >= 1)
					return map_functions<head_t<partition_value, type_list<T ...>>>::bt_scan_ci(as, ae, def);
				else
					return def;
			} else {
				if constexpr(partition_value + 1 < type_list<T...>::length)
					return map_functions<rest_t<partition_value + 1, type_list<T ...>>>::bt_scan_ci(as, ae, def);
				else
					return def;
			}
		}
	}
};

template <typename T>
struct map_call_functions;

template <typename ...T>
struct map_call_functions<type_list<T...>> {
	template<typename default_type, typename function_type, typename... ARGS>
	static auto scan_by_predicate(const function_type& f, ARGS&& ...args) {
		if constexpr(sizeof...(T) == 0)
			return default_type();
		else {
			if (typename first_of<T...>::first()(std::forward<ARGS>(args)...)) {
				return f(type_object<typename first_of<T...>::second>());
			} else {
				if constexpr(sizeof...(T) > 1)
					return map_call_functions<rest_of<type_list, T...>>::template scan_by_predicate<default_type>(f, std::forward<ARGS>(args)...);
				else
					return default_type();
			}
		}
	}
	template<typename default_type, typename function_type>
	static auto bt_scan_ci(const char* const as, const char* const ae, const function_type& f) {
		if constexpr(sizeof...(T) == 0)
			return default_type();
		else if constexpr(sizeof...(T) == 1) {
			if (compile_time_str_compare_ci<typename first_of<T...>::first>(as, ae) == 0) {
				return f(type_object<typename first_of<T...>::second>());
			}
			return default_type();
		} else {
			constexpr auto partition_value = type_list<T...>::length / 2ui64;
			using partition_pair = type_list_get_t<partition_value, type_list<T ...>>;

			const int compare_result = compile_time_str_compare_ci<typename partition_pair::first>(as, ae);

			if (compare_result == 0) {
				return f(type_object<typename partition_pair::second>());
			} else if (compare_result < 0) {
				if constexpr(partition_value >= 1)
					return map_call_functions<head_t<partition_value, type_list<T ...>>>::template bt_scan_ci<default_type>(as, ae, f);
				else
					return default_type();
			} else {
				if constexpr(partition_value + 1 < type_list<T...>::length)
					return map_call_functions<rest_t<partition_value + 1, type_list<T ...>>>::template bt_scan_ci<default_type>(as, ae, f);
				else
					return default_type();
			}
		}
	}
};

template<typename ...T>
struct without_s;

template<typename T>
struct without_s<T, type_list<>> {
	using type = type_list<>;
};

template<typename T, typename ...L>
struct without_s<T, type_list<L...>> {
	using type = std::conditional_t<std::is_same_v<T, first_of<L...>>, typename without_s<T, rest_of<type_list, L...>>::type, typename without_s<T, rest_of<type_list, L...>>::type::template cons<first_of<L...>> >;
};

template<typename ... T>
struct type_list {
	constexpr static size_t length = sizeof...(T);

	template<typename C>
	using cons = type_list<C, T...>;

	template<typename C>
	using without = typename without_s<C, type_list<T...>>::type;

	template<size_t index, typename to_append>
	using append_to_member = splice_type_list_t<
		head_t<index, type_list<T ...>>,
		typename type_list_get_t<index, type_list<T ...>>::template cons<to_append>,
		rest_t<index + 1, type_list<T ...>>>;
};

template<typename A, typename B>
struct typepair {
	using first = A;
	using second = B;
};

template<typename A, A a>
struct generic_constant {
	using type = A;
	static constexpr A value = a;
};

template<typename A, A a>
struct generic_function {
	using type = A;
	template<typename ...ARGS>
	auto operator()(ARGS&& ... args) {
		return a(std::forward<ARGS>(args)...);
	}
};

template<typename A, A a, typename tag_t>
struct generic_tagged_constant {
	using type = A;
	static constexpr A value = a;
	using tag = tag_t;
};

template<typename A, typename tag_t>
struct generic_tagged_type {
	using type = A;
	using tag = tag_t;
};

#define CT_STRING_INT(str, i) typepair< CT_STRING( str ), std::integral_constant<int, i > >
