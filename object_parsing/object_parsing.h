#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "Parsers\\parsers.h"
#include "text_classifier\\text_classifiers.h"

template<typename result_type>
class object_parser {
private:
	using boolean_classifying_function = std::function<bool(const char*, const char*)>;
	using handling_function = std::function<void(result_type&, const token_group&, const token_group*, const token_group*)>;

	binary_search_classifier tag_classifier;
	handling_function default_handler = [](result_type&, const token_group&, const token_group*, const token_group*) {};
	std::vector<std::pair<const char*, handling_function>> tag_handlers;
public:
	result_type operator()(const token_group* start, const token_group* end) const;
	void add_case_handler(const char* tag, const handling_function &handle);
	void reset_default_handler(const handling_function& f);
	void add_special_case_handler(const boolean_classifying_function &test, const handling_function &handle);
};

std::string associated_string(const token_group& left, const token_group* child_start, const token_group* child_end);

template<typename T>
T value_from_rh(association_type, const token_and_type& t) {
	return token_to<T>(t);
};

inline int discard_from_rh(association_type, const token_and_type&) { return 0; }
inline int discard_from_full(const token_and_type&, association_type, const token_and_type&) { return 0; }

inline std::pair<association_type, token_and_type> yield_rh(association_type a, const token_and_type& t) {
	return std::pair<association_type, token_and_type>(a, t);
}
inline std::tuple<token_and_type, association_type, token_and_type> yield_full(const token_and_type& l, association_type a, const token_and_type& r) {
	return std::tuple<token_and_type, association_type, token_and_type>(l, a, r);
}

template<typename T>
T value_from_full_rh(const token_and_type&, association_type, const token_and_type& t) {
	return token_to<T>(t);
};

template<typename T>
std::pair<association_type, T> association_and_value_from_rh(association_type a, const token_and_type& t) {
	return std::make_pair(a, token_to<T>(t));
};

template<typename T>
std::pair<association_type, T> association_and_value_from_full_rh(const token_and_type&, association_type a, const token_and_type& t) {
	return std::make_pair(a, token_to<T>(t));
};

template<typename T>
T value_from_lh(const token_and_type& t, association_type, const token_and_type&) {
	return token_to<T>(t);
};

template<typename T, typename U>
std::pair<T,U> pair_from_lh_rh(const token_and_type& t, association_type, const token_and_type& u) {
	return std::make_pair(token_to<T>(t), token_to<U>(u));
};

inline token_and_type token_from_rh(association_type, const token_and_type& t) {
	return t;
}

inline token_and_type token_from_lh(const token_and_type& t, association_type, const token_and_type&) {
	return t;
}

inline std::pair<token_and_type, token_and_type> token_pair_from_full(const token_and_type& l, association_type, const token_and_type& r) {
	return std::pair<token_and_type, token_and_type>(l,r);
}

constexpr bool accept_all(const char*, const char*) {
	return true;
}

template<typename T, typename R>
void move_to_member(T&& member, R&& result) {
	member = std::move(result);
}
template<typename V, typename A, typename T, typename = std::enable_if_t<std::is_constructible_v<V, T>>>
void move_to_member(std::vector<V, A>& member, T&& result) {
	member.emplace_back(std::forward<T>(result));
}

#define BEGIN_DERIVED_DOMAIN(name, base)    using name = typename base
#define BEGIN_DOMAIN(name)    using name = typename type_list<>
#define EMPTY_TYPE(type_name)     ::template cons< typepair< type_name, type_list<type_list<>, type_list<>, type_list<>, type_list<>, type_list<>> >>
#define BEGIN_TYPE(type_name)     ::template cons< typepair< type_name, typename type_list<type_list<>, type_list<>, type_list<>, type_list<>, type_list<>>
#define MEMBER_ASSOCIATION(member_name, tag, generating_function) ::template append_to_member<0ui64, typepair<CT_STRING( tag ), function_and_tuple< CT_STRING( member_name ), decltype(generating_function), generating_function >>>
#define MEMBER_ASSOCIATION_1(member_name, tag, generating_function, parameter) ::template append_to_member<0ui64, typepair<CT_STRING( tag ), function_and_tuple< CT_STRING( member_name ), decltype(generating_function), generating_function, generic_constant< decltype(parameter), parameter> > > >
#define MEMBER_ASSOCIATION_2(member_name, tag, generating_function, parameter, parameterb) ::template append_to_member<0ui64, typepair<CT_STRING( tag ), function_and_tuple< CT_STRING( member_name ), decltype(generating_function), generating_function, generic_constant< decltype(parameter), parameter>, generic_constant< decltype(parameterb), parameterb> > > >
#define MEMBER_TYPE_ASSOCIATION(member_name, tag, to_type)   ::template append_to_member<1ui64, typepair<CT_STRING( tag ), function_and_object_tuple<to_type, CT_STRING(member_name)>>>
#define MEMBER_TYPE_EXTERN(member_name, tag, to_type, function)   ::template append_to_member<1ui64, typepair<CT_STRING( tag ), function_and_object_tuple_w_parse<to_type, CT_STRING(member_name), decltype(function), function>>>
#define MEMBER_TYPE_ASSOCIATION_EX0(member_name, tag, to_type, function)   ::template append_to_member<1ui64, typepair<CT_STRING( tag ), function_and_object_tuple_with_extra<to_type, CT_STRING(member_name), decltype(function), function>>>
#define MEMBER_TYPE_ASSOCIATION_EX1(member_name, tag, to_type, function, parameter)   ::template append_to_member<1ui64, typepair<CT_STRING( tag ), function_and_object_tuple_with_extra<to_type, CT_STRING(member_name), decltype(function), function, generic_constant< decltype(parameter), parameter>>>>
#define MEMBER_VARIABLE_ASSOCIATION(member_name, classifying_function, generating_function)   ::template append_to_member<2ui64, typepair<generic_function<decltype(classifying_function), classifying_function>, function_and_tuple_ext<CT_STRING(member_name), decltype(generating_function), generating_function>>>
#define MEMBER_VARIABLE_ASSOCIATION_1(member_name, classifying_function, generating_function, parameter)   ::template append_to_member<2ui64, typepair<generic_function<decltype(classifying_function), classifying_function>, function_and_tuple_ext<CT_STRING(member_name), decltype(generating_function), generating_function, generic_constant< decltype(parameter), parameter>>>>
#define MEMBER_VARIABLE_TYPE_ASSOCIATION(member_name, classifying_function, to_type, generating_function) ::template append_to_member<3ui64, typepair<generic_function<decltype(classifying_function), classifying_function>, function_and_object_tuple_ext<to_type, CT_STRING(member_name), decltype(generating_function), generating_function> >>
#define MEMBER_VARIABLE_TYPE_EXTERN(member_name, classifying_function, to_type, function)   ::template append_to_member<3ui64, typepair<generic_function<decltype(classifying_function), classifying_function>, function_and_object_tuple_w_parse<to_type, CT_STRING(member_name), decltype(function), function>>>
#define INHERIT_FROM(type)  ::template append_to_member<4ui64, type >
#define END_TYPE                   > >
#define END_DOMAIN            ;

/*template<typename dest_type, typename member_ident, typename function_type, function_type parse_func>
struct function_and_object_tuple_w_parse*/


template<typename A, typename B>
struct _has_parse_func : public std::false_type {};
template<typename A>
struct _has_parse_func<A, decltype(void(std::declval<typename A::parse_function_object>()))> : public std::true_type {};
template<typename A>
constexpr bool has_parse_func = _has_parse_func<A, void>::value;

template<typename IDENT, typename CLASS>
struct _set_member;

template<typename CLASS>
struct _set_member<CT_STRING("this"), CLASS> {
	template<typename T>
	static void set(CLASS& in, T&& t) { move_to_member(in, std::forward<T>(t)); }
};

#define MEMBER_DEF(class_name, member, identifier) \
template<> \
struct _set_member<CT_STRING(identifier), class_name>{ \
    template<typename V>\
    static void set(class_name& class_passed, V&& v) { \
        move_to_member( class_passed. member , std::forward<V>(v));\
    } \
};

#define MEMBER_FDEF(class_name, member, identifier) \
template<> \
struct _set_member<CT_STRING(identifier), class_name>{ \
    template<typename V>\
    static void set(class_name& class_passed, V&& v) { \
        class_passed. member (std::forward<V>(v));\
    } \
};

template<size_t nth, typename context>
struct _get_nth_list {
	template<typename T>
	struct get_from_context {
		using type = type_list_get_t< nth, type_map_get_t<context, T>>;
	};
};

template<typename tag_to_function, typename function_to_function, typename T, typename U>
bool try_and_fallback(const token_and_type& lhtoken, const T& f1, const U& f2) {
	if (!(map_call_functions<tag_to_function>::template bt_scan_ci<bool>(lhtoken.start, lhtoken.end, f1))) {
		return map_call_functions<function_to_function>::template scan_by_predicate<bool>(f2, lhtoken.start, lhtoken.end);
	}
	return true;
}

template<typename result_type, typename ... obj_params>
result_type obj_construction_wrapper(obj_params&& ... params) {
	if constexpr(std::is_constructible_v<result_type, obj_params...>) {
		return result_type(std::forward<obj_params>(params)...);
	} else {
		return result_type();
	}
}

template<typename result_type, typename context, typename ... obj_params>
result_type parse_object(const token_group* start, const token_group* end, const token_and_type& name, obj_params&& ... params);
template<typename result_type, typename context, typename ... obj_params>
result_type parse_object(const token_group* start, const token_group* end, obj_params&& ... params);

template<typename context, typename result_type, typename ... obj_params>
void parse_constructed_object(const token_group* start, const token_group* end, result_type& result, obj_params&& ... params) {
	using this_in_context = type_map_get_t<context, result_type>;
	using inheritance_list = type_list_get_t<4ui64, this_in_context>;



	using tag_to_function =
		sorted_t<
		merge_lists<
		type_list_get_t<0ui64, this_in_context>,
		flatten_list<
		apply_to_all<_get_nth_list<0ui64, context>::template get_from_context, inheritance_list>
		>
		>
		>;
	using tag_to_generator =
		sorted_t<
		merge_lists<
		type_list_get_t<1ui64, this_in_context>,
		flatten_list<
		apply_to_all<_get_nth_list<1ui64, context>::template get_from_context, inheritance_list>
		>
		>
		>;
	using function_to_function =
		sorted_t<
		merge_lists<
		type_list_get_t<2ui64, this_in_context>,
		flatten_list<
		apply_to_all<_get_nth_list<2ui64, context>::template get_from_context, inheritance_list>
		>
		>
		>;
	using function_to_generator =
		sorted_t<
		merge_lists<
		type_list_get_t<3ui64, this_in_context>,
		flatten_list<
		apply_to_all<_get_nth_list<3ui64, context>::template get_from_context, inheritance_list>
		>
		>
		>;

	forall_tokens(start, end, [&result, &params ...](const token_group &n, const token_group *child_start, const token_group *child_end) {
		if (n.association != association_type::list) {
			if (n.group_size == 0) {
				if (!try_and_fallback<tag_to_function, function_to_function>(n.token, [&result](auto t) {
					using type_passed = typename decltype(t)::type;
					typename type_passed::function_object()(
						result,
						association_type::none,
						token_and_type{ nullptr, nullptr, token_type::unknown }
					);
					return true;
				}, [&result, &n](auto t) {
					using type_passed = typename decltype(t)::type;

					typename type_passed::function_object()(
						result,
						n.token,
						association_type::none,
						token_and_type{ nullptr, nullptr, token_type::unknown }
					);
					return true;
				})) {
#ifdef _DEBUG
					OutputDebugStringA("Error: failed to find association for free token ");
					OutputDebugStringA(std::string(n.token.start, n.token.end).c_str());
#endif
				}
			} else {
				if (!try_and_fallback<tag_to_function, function_to_function>(n.token, [&result, asc = n.association, right = child_start](auto t) {
					using type_passed = typename decltype(t)::type;
					typename type_passed::function_object()(
						result,
						asc,
						right->token
						);
					return true;
				}, [&result, &n, right = child_start](auto t) {
					using type_passed = typename decltype(t)::type;
					typename type_passed::function_object()(
						result,
						n.token,
						n.association,
						right->token
						);
					return true;
				})) {
#ifdef _DEBUG
					OutputDebugStringA("Error: failed to find token-identifier pair association for token ");
					OutputDebugStringA(std::string(n.token.start, n.token.end).c_str());
					OutputDebugStringA(" = ");
					OutputDebugStringA(std::string(child_start->token.start, child_start->token.end).c_str());
					OutputDebugStringA("\n");
#endif
				}
			}
		} else {
			if (!try_and_fallback<tag_to_generator, function_to_generator>(n.token, [&result, &n, child_start, child_end, &params ...](const auto t) {
				using type_passed = typename decltype(t)::type;
				if constexpr(has_parse_func<type_passed>) {
					typename type_passed::function_object()(
						result,
						typename type_passed::parse_function_object()(child_start, child_end, params ...)
						);
				} else {
					typename type_passed::function_object()(
						result, n.token, n.association,
						parse_object<typename type_passed::type, context>(child_start, child_end, params ...)
						);
				}
				return true;
			}, [&result, &n, child_start, child_end, &params ...](const auto t) {
				using type_passed = typename decltype(t)::type;
				if constexpr(has_parse_func<type_passed>) {
					typename type_passed::function_object()(
						result,
						typename type_passed::parse_function_object()(child_start, child_end, n.token, params ...)
						);
				} else if constexpr(std::is_constructible_v<typename type_passed::type, token_and_type, obj_params...>) {
					typename type_passed::function_object()(
						result, n.token, n.association,
						parse_object<typename type_passed::type, context>(child_start, child_end, n.token, params ...)
						);
				} else {
					typename type_passed::function_object()(
						result, n.token, n.association,
						parse_object<typename type_passed::type, context>(child_start, child_end, params ...)
						);
				}
				return true;
			})) {
#ifdef _DEBUG
				OutputDebugStringA("Error: failed to find token list association for token ");
				OutputDebugStringA(std::string(n.token.start, n.token.end).c_str());
				OutputDebugStringA("\n");
#endif
			}
		}
	});
}


template<typename result_type, typename context, typename ... obj_params>
result_type parse_object(const token_group* start, const token_group* end, const token_and_type& name, obj_params&& ... params) {
	result_type result = obj_construction_wrapper<result_type>(name, params...);
	parse_constructed_object<context>(start, end, result, std::forward<obj_params>(params)...);
	return result;
}

template<typename result_type, typename context, typename ... obj_params>
result_type parse_object(const token_group* start, const token_group* end, obj_params&& ... params) {
	result_type result = obj_construction_wrapper<result_type>(params...);
	parse_constructed_object<context>(start, end, result, std::forward<obj_params>(params)...);
	return result;
}


template<typename member_ident, typename function_type, function_type finstance, typename ... ARGS>
struct function_and_tuple {
	struct function_object {
		template<typename in_class>
		void operator()(in_class& cls, association_type b, const token_and_type& c) {
			_set_member<member_ident, in_class>::set(cls, finstance(b, c, ARGS::value ...));
		}
	};
};

template<typename dest_type, typename member_ident, typename function_type, function_type finstance, typename ... ARGS>
struct function_and_object_tuple_ext {
	using type = dest_type;
	using tag = member_ident;
	struct function_object {
		template<typename in_class, typename from>
		void operator()(in_class& cls, const token_and_type& a, association_type b, from&& c) {
			_set_member<member_ident, in_class>::set(cls, finstance(a, b, c, ARGS::value ...));
		}
	};
};

template<typename dest_type, typename member_ident>
struct function_and_object_tuple {
	using type = dest_type;
	using tag = member_ident;
	struct function_object {
		template<typename in_class, typename from>
		void operator()(in_class& cls, const token_and_type&, association_type, from&& c) {
			_set_member<member_ident, in_class>::set(cls, c);
		}
	};
};

template<typename dest_type, typename member_ident, typename function_type, function_type parse_func>
struct function_and_object_tuple_w_parse {
	using type = dest_type;
	using tag = member_ident;

	struct function_object {
		template<typename in_class, typename from>
		void operator()(in_class& cls, from&& c) {
			_set_member<member_ident, in_class>::set(cls, c);
		}
	};
	struct parse_function_object {
		template<typename ... PARAMS>
		auto operator()(const token_group* start, const token_group* end, PARAMS&& ... params) {
			return parse_func(start, end, std::forward<PARAMS>(params)...);
		}
	};
};

template<typename dest_type, typename member_ident, typename function_type, function_type finstance, typename ... ARGS>
struct function_and_object_tuple_with_extra {
	using type = dest_type;
	using tag = member_ident;
	struct function_object {
		template<typename in_class, typename from>
		void operator()(in_class& cls, const token_and_type&, association_type t, from&& c) {
			finstance(c, t, ARGS::value ...);
			_set_member<member_ident, in_class>::set(cls, c);
		}
	};
};

template<typename member_ident, typename function_type, function_type finstance, typename ... ARGS>
struct function_and_tuple_ext {
	struct function_object {
		template<typename in_class>
		void operator()(in_class& cls, const token_and_type& a, association_type b, const token_and_type& c) {
			_set_member<member_ident, in_class>::set(cls, finstance(a, b, c, ARGS::value ...));
		}
	};
};
