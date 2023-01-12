#pragma once

#include <stdint.h>
#include <vector>
#include <atomic>
#include <string>
#include <tuple>
#include <numeric>
#include <variant>
#include <optional>
#include <tuple>
#include <array>

#ifdef __llvm__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-template"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wdeprecated-dynamic-exception-spec"
#pragma clang diagnostic ignored "-Wdeprecated"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING 1
#define BOOST_ALL_NO_LIB

#include "boost\\container\\flat_map.hpp"
#include "boost\\container\\small_vector.hpp"
#include "boost\\container\\flat_set.hpp"
#include "boost\date_time\\gregorian\\gregorian_types.hpp" 

#define EIGEN_INITIALIZE_MATRICES_BY_ZERO
#define EIGEN_NO_MALLOC


#pragma warning( push )
#pragma warning(disable:5054)

#include "Eigen\\Dense"

#pragma warning( pop )

#ifdef __llvm__
#pragma clang diagnostic pop
#endif

#ifndef _DEBUG
#define CALL __vectorcall
#define RELEASE_INLINE __forceinline
#else
#define CALL
#define RELEASE_INLINE inline
#endif

#ifndef _DEBUG
namespace boost {
	inline void __cdecl throw_exception(std::exception const &) { std::abort(); }
}
#endif

#undef max
#undef min
namespace std {
	template<typename A, typename = typename std::iterator_traits<A>::iterator_category>
	constexpr auto begin(std::pair<A, A> const& p) {
		return p.first;
	}
	template<typename A, typename = typename std::iterator_traits<A>::iterator_category>
	constexpr auto end(std::pair<A, A> const& p) {
		return p.second;
	}

	template<typename A>
	constexpr auto begin(std::pair<A*, A*> const& p) {
		return p.first;
	}
	template<typename A>
	constexpr auto end(std::pair<A*, A*> const& p) {
		return p.second;
	}
}

template<typename T>
constexpr T null_value_of = std::numeric_limits<T>::max();

template<typename T>
constexpr T to_index(T in) { return in; }

template<typename T>
constexpr bool is_valid_index(T in) { return in != std::numeric_limits<T>::max(); }

template<typename T>
struct value_base_of_s { using type = T; };

template<typename T>
struct zero_is_null_of_s { using type = std::false_type; };

template<typename T>
struct individuator_of_s { using type = std::false_type; };

template<typename T>
void normalize_integer_vector(T* vec, uint32_t count, T sum, uint32_t remainder_index = 0) {
	int32_t current_sum = std::accumulate(vec, vec + count, 0);
	float factor = current_sum != 0 ? float(sum) / float(current_sum) : 0.0f;

	float fractional_part = 0.0f;

	for(uint32_t i = 0; i < count; ++i) {
		float integral_point = 0.0f;
		fractional_part = std::modf(factor * float(vec[i]) + fractional_part, &integral_point);
		vec[i] = T(integral_point);
	}
	vec[remainder_index] += T(int32_t(sum) - std::accumulate(vec, vec + count, 0));
}

template<typename value_base, typename zero_is_null, typename individuator>
struct tag_type {
	static constexpr value_base null_value = std::is_same_v<std::true_type, zero_is_null> ? 0 : value_base(-1);
	using value_base_t = value_base;
	using zero_is_null_t = zero_is_null;
	using individuator_t = individuator;
	using index_result_t = std::conditional_t<sizeof(value_base) <= 4, int32_t, std::make_signed_t<value_base>>;

	value_base value;

	explicit constexpr tag_type(value_base v) noexcept : value(v + (std::is_same_v<std::true_type, zero_is_null> ? 1 : 0)) {}
	constexpr tag_type(const tag_type& v) noexcept = default;
	constexpr tag_type(tag_type&& v) noexcept = default;
	constexpr tag_type(value_base v, std::true_type) noexcept : value(v) {}

	constexpr tag_type() noexcept : value(null_value) {}

	constexpr index_result_t index() const noexcept {
		if constexpr(std::is_same_v<std::true_type, zero_is_null>) {
			return index_result_t(value) - 1;
		} else {
			if constexpr(std::is_signed_v<value_base> || std::is_same_v<index_result_t, std::make_signed_t<value_base>>) {
				return value;
			} else {
				value_base i = value + value_base(1);
				return index_result_t(i) - 1;
			}
		}
	}
	constexpr bool is_valid() const noexcept { return value != null_value; }
	tag_type& operator=(tag_type&& v) noexcept = default;
	tag_type& operator=(tag_type const& v) noexcept = default;

	constexpr bool operator==(tag_type v) const noexcept { return value == v.value; }
	constexpr bool operator!=(tag_type v) const noexcept { return value != v.value; }
	constexpr bool operator<(tag_type v) const noexcept { return value < v.value; }
	constexpr bool operator<=(tag_type v) const noexcept { return value <= v.value; }

	explicit constexpr operator bool() const noexcept { return value != null_value; }
};

template<typename value_base, typename zero_is_null, typename individuator>
constexpr tag_type<value_base, zero_is_null, individuator> null_value_of<tag_type<value_base, zero_is_null, individuator>> = tag_type<value_base, zero_is_null, individuator>();

template<typename value_base, typename zero_is_null, typename individuator>
constexpr auto to_index(tag_type<value_base, zero_is_null, individuator> in) { return in.index(); }

template<typename value_base, typename zero_is_null, typename individuator>
constexpr bool is_valid_index(tag_type<value_base, zero_is_null, individuator> in) { return in.is_valid(); }

template<typename value_base, typename zero_is_null, typename individuator>
struct value_base_of_s<tag_type<value_base, zero_is_null, individuator>> { using type = value_base; };

template<typename value_base, typename zero_is_null, typename individuator>
struct zero_is_null_of_s<tag_type<value_base, zero_is_null, individuator>> { using type = zero_is_null; };

template<typename value_base, typename zero_is_null, typename individuator>
struct individuator_of_s<tag_type<value_base, zero_is_null, individuator>> { using type = individuator; };

template<typename T>
struct decay_tag_s {
	using type = T;
};
template<typename T>
using decay_tag = typename decay_tag_s<T>::type;

struct union_tag {
	int32_t value = 0;
	using value_base_t = int32_t;
	using zero_is_null_t = std::true_type;

	constexpr union_tag() : value(0) {}
	constexpr explicit union_tag(int32_t v) : value(v + 1) {}
	constexpr union_tag(const union_tag& v) noexcept = default;
	constexpr union_tag(union_tag&& v) noexcept = default;
	constexpr union_tag(int32_t v, std::true_type) noexcept : value(v) {}

	template<typename value_base, typename zero_is_null, typename individuator>
	constexpr union_tag(tag_type<value_base, zero_is_null, individuator> b) noexcept :
		value(zero_is_null::value ? int32_t(b.value) : int32_t(b.value + value_base(1))) {}

	template<typename value_base, typename zero_is_null, typename individuator>
	constexpr operator tag_type<value_base, zero_is_null, individuator>() const noexcept {
		if constexpr(zero_is_null::value) {
			return tag_type<value_base, zero_is_null, individuator>(typename tag_type<value_base, zero_is_null, individuator>::value_base_t(value), std::true_type());
		} else {
			if(value != 0)
				return tag_type<value_base, zero_is_null, individuator>(typename tag_type<value_base, zero_is_null, individuator>::value_base_t(value));
			else
				tag_type<value_base, zero_is_null, individuator>();
		}
	}

	constexpr bool is_valid() const noexcept { return value != 0; }

	union_tag& operator=(union_tag&& v) noexcept = default;
	union_tag& operator=(union_tag const& v) noexcept = default;

	constexpr bool operator==(union_tag v) const noexcept { return value == v.value; }
	constexpr bool operator!=(union_tag v) const noexcept { return value != v.value; }
	constexpr bool operator<(union_tag v) const noexcept { return value < v.value; }
	constexpr bool operator<=(union_tag v) const noexcept { return value <= v.value; }

	explicit constexpr operator bool() const noexcept { return value != 0; }
};

constexpr int32_t to_index(union_tag in) { return in.value - 1; }

template<>
constexpr bool is_valid_index<union_tag>(union_tag in) { return in.value != 0; }

template<>
struct value_base_of_s<union_tag> { using type = int32_t; };

template<>
struct zero_is_null_of_s<union_tag> { using type = std::true_type; };

template<>
struct individuator_of_s<union_tag> { using type = typename union_tag; };


template<typename T>
using value_base_of = typename value_base_of_s<T>::type;

template<typename T>
using zero_is_null_of = typename zero_is_null_of_s<T>::type;

template<typename T>
using individuator_of = typename individuator_of_s<T>::type;


template<typename individuator, typename index_type, bool padding>
struct array_tag : public tag_type<uint32_t, std::false_type, individuator> {};

template<typename individuator, typename index_type, bool padding>
constexpr array_tag<individuator, index_type, padding> null_value_of<array_tag<individuator, index_type, padding>> = array_tag<individuator, index_type, padding>();

template<typename individuator, typename index_type, bool padding>
constexpr uint32_t to_index(array_tag<individuator, index_type, padding> in) { return in.value; }

template<typename individuator, typename index_type, bool padding>
constexpr bool is_valid_index(array_tag<individuator, index_type, padding> in) { return in.value != std::numeric_limits<uint32_t>::max(); }

template<typename individuator, typename index_type, bool padding>
struct individuator_of_s<array_tag<individuator, index_type, padding>> { using type = individuator; };


template<typename individuator>
struct set_tag : public tag_type<uint32_t, std::false_type, individuator> {};

template<typename individuator>
constexpr set_tag<individuator> null_value_of<set_tag<individuator>> = set_tag<individuator>();

template<typename individuator>
constexpr uint32_t to_index(set_tag<individuator> in) { return in.value; }

template<typename individuator>
constexpr bool is_valid_index(set_tag<individuator> in) { return in.value != std::numeric_limits<uint32_t>::max(); }

template<typename individuator>
struct individuator_of_s<set_tag<individuator>> { using type = individuator; };


template<typename individuator>
struct multiset_tag : public tag_type<uint32_t, std::false_type, individuator> {};

template<typename individuator>
constexpr multiset_tag<individuator> null_value_of<multiset_tag<individuator>> = multiset_tag<individuator>();

template<typename individuator>
constexpr uint32_t to_index(multiset_tag<individuator> in) { return in.value; }

template<typename individuator>
constexpr bool is_valid_index(multiset_tag<individuator> in) { return in.value != std::numeric_limits<uint32_t>::max(); }

template<typename individuator>
struct individuator_of_s<multiset_tag<individuator>> { using type = individuator; };



template<typename tag_base>
struct atomic_tag {
	using value_base_t = value_base_of<tag_base>;
	using zero_is_null_t = zero_is_null_of<tag_base>;
	using individuator_t = individuator_of<tag_base>;

	static constexpr value_base_t null_value = std::is_same_v<std::true_type, zero_is_null_t> ? value_base_t(0) : std::numeric_limits<value_base_t>::max();

	std::atomic<value_base_t> value;

	explicit atomic_tag(value_base_t v) noexcept : value(v + (std::is_same_v<std::true_type, zero_is_null_t> ? 1 : 0)) {}
	explicit atomic_tag(tag_base v) noexcept : value(to_index(v) + (std::is_same_v<std::true_type, zero_is_null_t> ? 1 : 0)) {}
	atomic_tag(const atomic_tag &v) noexcept : value(v.value.load(std::memory_order_acquire)) {}
	atomic_tag(atomic_tag &&v) noexcept : value(v.value.load(std::memory_order_acquire)) {}

	atomic_tag() noexcept : value(null_value) {}

	value_base_t index() const noexcept {
		if constexpr(std::is_same_v<std::true_type, zero_is_null_t>)
			return value.load(std::memory_order_acquire) - 1;
		else
			return value.load(std::memory_order_acquire);
	}
	bool is_valid() const noexcept {
		if constexpr(std::is_same_v<std::true_type, zero_is_null_t>)
			return value.load(std::memory_order_acquire) != 0;
		else
			return value.load(std::memory_order_acquire) != std::numeric_limits<value_base_t>::max();
	}

	void operator=(const atomic_tag &v) noexcept { value.store(v.value.load(std::memory_order_acquire), std::memory_order_release); }
	void operator=(tag_base v) noexcept { value.store(v.value, std::memory_order_release); }

	bool operator==(const atomic_tag &v) const noexcept { return value.load(std::memory_order_acquire) == v.value.load(std::memory_order_acquire); }
	bool operator==(tag_base v) const noexcept { return value.load(std::memory_order_acquire) == v.value; }
	bool operator!=(const atomic_tag &v) const noexcept { return value.load(std::memory_order_acquire) != v.value.load(std::memory_order_acquire); }
	bool operator!=(tag_base v) const noexcept { return value.load(std::memory_order_acquire) != v.value; }
	
	operator tag_base() const noexcept { return tag_base(value.load(std::memory_order_acquire), std::true_type()); }
};

template<typename tag_base>
constexpr tag_base null_value_of<atomic_tag<tag_base>> = tag_base();

template<typename tag_base>
constexpr typename atomic_tag<tag_base>::value_base_t to_index(const atomic_tag<tag_base>& in) { return in.index(); }

template<typename tag_base>
constexpr bool is_valid_index(const atomic_tag<tag_base>& in) { return in.is_valid(); }

template<typename tag_base>
struct value_base_of_s<atomic_tag<tag_base>> { using type = typename atomic_tag<tag_base>::value_base_t; };

template<typename tag_base>
struct zero_is_null_of_s<atomic_tag<tag_base>> { using type = typename atomic_tag<tag_base>::zero_is_null_t; };

template<typename tag_base>
struct individuator_of_s<atomic_tag<tag_base>> { using type = typename atomic_tag<tag_base>::individuator_t; };

template<typename T, typename tag_type>
struct tagged_object {
	T& object;
	const tag_type id;

	tagged_object(T& first, tag_type second) noexcept : object(first), id(second) {}

	operator T&() const noexcept {
		return object;
	}
	operator tag_type() const noexcept {
		return id;
	}
};


struct bitfield_type {
	uint8_t v = 0;

	bool operator[](int32_t i) const { return ((v >> i) & 1) != 0; }
};

static_assert(sizeof(bitfield_type) == 1);

inline void bit_vector_set(bitfield_type* v, int32_t index, bool value) {
	const int32_t real_index = index >> 3;
	const uint32_t sub_index = uint32_t(index) & 7ui32;
	if(value)
		v[real_index].v |= uint8_t(1ui32 << sub_index);
	else
		v[real_index].v &= uint8_t(~(1ui32 << sub_index));
}

inline bool bit_vector_test(bitfield_type const* v, int32_t index) {
	const int32_t real_index = index >> 3;
	const uint32_t sub_index = uint32_t(index) & 7ui32;
	return (v[real_index].v & (1ui32 << sub_index)) != 0;
}

template<typename T, typename index_type>
struct tagged_array_view {
private:
	T* const ptr = nullptr;
public:
#ifdef _DEBUG
	int32_t size = 0;
#endif

	constexpr tagged_array_view(T* p, int32_t s) noexcept : ptr(p)
#ifdef _DEBUG
		, size(s) 
#endif
	{}

	T& operator[](index_type i) const noexcept {
		return ptr[to_index(i)];
	}
	T* begin() const noexcept {
		return ptr;
	}
#ifdef _DEBUG
	T* debug_end() const noexcept {
		return ptr + size;
	}
#endif
	T* data() const noexcept {
		return ptr;
	}
	explicit operator bool() const noexcept {
		return ptr != nullptr;
	}
	constexpr operator tagged_array_view<const T, index_type>() const noexcept {
		return tagged_array_view<const T, index_type>(ptr, 
#ifdef _DEBUG
			size
#else
			0
#endif
			);
	}
	constexpr tagged_array_view<T, index_type> operator+(int32_t i) const noexcept {
		return tagged_array_view<T, index_type>(ptr + i,
#ifdef _DEBUG
			size - i
#else
			0
#endif
			);
	}
};

template<typename index_type>
struct tagged_array_view<void, index_type> {
private:
public:
#ifdef _DEBUG
	int32_t size = 0;
#endif

	constexpr tagged_array_view(void* p, int32_t s) noexcept
#ifdef _DEBUG
		: size(s)
#endif
	{}

	void operator[](index_type i) const noexcept {
	}
	void begin() const noexcept {
	}
#ifdef _DEBUG
	void debug_end() const noexcept {
	}
#endif
	void data() const noexcept {
	}
	explicit operator bool() const noexcept {
		return false;
	}
	constexpr operator tagged_array_view<const void, index_type>() const noexcept {
		return tagged_array_view<const void, index_type>(nullptr,
#ifdef _DEBUG
			size
#else
			0
#endif
			);
	}
};

template<typename index_type>
struct tagged_array_view<const void, index_type> {
private:
public:
#ifdef _DEBUG
	int32_t size = 0;
#endif

	constexpr tagged_array_view(void* p, int32_t s) noexcept
#ifdef _DEBUG
		: size(s)
#endif
	{}

	void operator[](index_type i) const noexcept {}
	void begin() const noexcept {}
#ifdef _DEBUG
	void debug_end() const noexcept {}
#endif
	void data() const noexcept {}
	explicit operator bool() const noexcept {
		return false;
	}
};

template<typename index_type>
void bit_vector_set(tagged_array_view<bitfield_type, index_type> v, index_type index, bool value) {
	bit_vector_set(v.data(), to_index(index), value);
}

template<typename T, typename index_type>
auto bit_vector_test(tagged_array_view<T, index_type> v, index_type index) -> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, bitfield_type>, bool> {
	return bit_vector_test(v.data(), to_index(index));
}

#define STRUCT_MEMBER( i_type, name ) \
	template<typename INDEX> \
	static auto get_member(_struct_type& s) noexcept -> std::enable_if_t<std::is_same_v<INDEX, i_type>, std::add_lvalue_reference_t<decltype(s. name )>> { \
		return s. name ; \
	} \
	template<typename INDEX> \
	static auto get_member(_struct_type const& s) noexcept -> std::enable_if_t<std::is_same_v<INDEX, i_type>, std::add_lvalue_reference_t<std::add_const_t<decltype(s. name )>>> { \
		return s. name ; \
	}

template<typename T>
struct struct_wrapper {
	using _struct_type = T;
};

#define START_STRUCT( struct_name ) \
template<> \
struct struct_wrapper<struct_name> { \
	using _struct_type = struct_name ;

#define END_STRUCT };

#define START_INT_MAPPING(mapping_name) \
template<typename T> \
struct mapping_name { \
	constexpr static int32_t value = -1;\
};

#define MAP_INT(mapping_name, tag_name, int_value) \
struct tag_name; \
template<> \
struct mapping_name<tag_name> { \
	constexpr static int32_t value = int_value; \
};

#define END_INT_MAPPING

template<typename A, typename B, typename ... C>
struct _supports_get : std::false_type {};
template<typename A, typename ... C>
struct _supports_get<A, decltype(void(std::declval<A>().get(std::declval<C>() ...))), C...> : std::true_type {};
template<typename A, typename ... C>
constexpr bool supports_get = _supports_get<A, void, C ...>::value;

template<typename A, typename B, typename index, typename ... C>
struct _supports_get_t1 : std::false_type {};
template<typename A, typename index, typename ... C>
struct _supports_get_t1<A, decltype(void(std::declval<A>().template get<index>(std::declval<C>() ...))), index, C...> : std::true_type {};
template<typename A, typename index, typename ... C>
constexpr bool supports_get_t1 = _supports_get_t1<A, void, index, C ...>::value;

template<typename container, typename index>
struct supports_index {
	static constexpr bool value = false;
	using type = void;
	using const_type = void;
	using row = tagged_array_view<int32_t, int32_t>;
	using const_row = tagged_array_view<const int32_t, int32_t>;
};

#define GET_SET(container_name) \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) noexcept -> std::enable_if_t<decltype(container_name)::supports_index<INDEX>, decltype(container_name)::get_type<INDEX>> { \
	return container_name.get<INDEX>(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) const noexcept-> std::enable_if_t<decltype(container_name)::supports_index<INDEX>, decltype(container_name)::const_get_type<INDEX>> { \
	return container_name.get<INDEX>(t); \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type v) noexcept -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && decltype(container_name)::supports_index<INDEX>, void> { \
	return container_name.set<INDEX>(t, v); \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type const& v) noexcept -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && decltype(container_name)::supports_index<INDEX>, void> { \
	return container_name.set<INDEX>(t, v); \
} \
template<typename INDEX> \
RELEASE_INLINE auto get_row() noexcept -> std::enable_if_t<decltype(container_name)::supports_index<INDEX>, tagged_array_view<typename decltype(container_name)::template value_type<INDEX>, decltype(container_name)::index_type>> { \
	return container_name.get_row<INDEX>(); \
} \
template<typename INDEX> \
RELEASE_INLINE auto get_row() const noexcept -> std::enable_if_t<decltype(container_name)::supports_index<INDEX>, tagged_array_view<typename decltype(container_name)::template value_type<INDEX> const, decltype(container_name)::index_type>> { \
	return container_name.get_row<INDEX>(); \
} \
template<typename INDEX> \
RELEASE_INLINE auto size() const noexcept -> std::enable_if_t<decltype(container_name)::supports_index<INDEX>, int32_t> { \
	return int32_t(container_name.size<INDEX>()); \
} \
template<typename INDEX> \
RELEASE_INLINE auto resize(int32_t sz) const noexcept -> std::enable_if_t<decltype(container_name)::supports_index<INDEX>, void> { \
	int32_t(container_name.resize<INDEX>(sz)); \
}

#define GET_SET_GEN(container_name) \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) noexcept -> std::enable_if_t<supports_index<decltype(container_name), INDEX>::value, typename supports_index<decltype(container_name), INDEX>::type> { \
	return container_name.get<INDEX>(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) const noexcept-> std::enable_if_t<supports_index<decltype(container_name), INDEX>::value, typename supports_index<decltype(container_name), INDEX>::const_type> { \
	return container_name.get<INDEX>(t); \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type v) noexcept -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && supports_index<decltype(container_name), INDEX>::value, void> { \
	return container_name.set<INDEX>(t, v); \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type const& v) noexcept -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && supports_index<decltype(container_name), INDEX>::value, void> { \
	return container_name.set<INDEX>(t, v); \
} \
template<typename INDEX> \
RELEASE_INLINE auto get_row() noexcept -> std::enable_if_t<supports_index<decltype(container_name), INDEX>::value, typename supports_index<decltype(container_name), INDEX>::row> { \
	return container_name.get_row<INDEX>(); \
} \
template<typename INDEX> \
RELEASE_INLINE auto get_row() const noexcept -> std::enable_if_t<supports_index<decltype(container_name), INDEX>::value, typename supports_index<decltype(container_name), INDEX>::const_row> { \
	return container_name.get_row<INDEX>(); \
} \
template<typename INDEX> \
RELEASE_INLINE auto size() const noexcept -> std::enable_if_t<supports_index<decltype(container_name), INDEX>::value, int32_t> { \
	return int32_t(container_name.size()); \
} \
template<typename INDEX> \
RELEASE_INLINE auto resize(int32_t sz) const noexcept -> std::enable_if_t<supports_index<decltype(container_name), INDEX>::value, void> { \
	int32_t(container_name.resize(sz)); \
}

#define GET_SET_STV(container_name) \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) noexcept -> std::enable_if_t<!std::is_same_v<decltype(container_name.get<INDEX>(t)), void>, decltype(container_name.get<INDEX>(t))> { \
	return container_name.get<INDEX>(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) const noexcept -> std::enable_if_t<!std::is_same_v<decltype(container_name.get<INDEX>(t)), void>, decltype(container_name.get<INDEX>(t))> { \
	return container_name.get<INDEX>(t); \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type v) noexcept -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name.get<INDEX>(t)), void>, void> { \
	return container_name.set<INDEX>(t, v); \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type const& v) noexcept -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name.get<INDEX>(t)), void>, void> { \
	return container_name.set<INDEX>(t, v); \
} \
template<typename INDEX> \
RELEASE_INLINE auto size() const noexcept -> std::enable_if_t<!std::is_same_v<decltype(container_name.get<INDEX>(decltype(container_name)::public_tag_type())), void>, int32_t> { \
	return int32_t(container_name.size<INDEX>()); \
} \
template<typename INDEX> \
RELEASE_INLINE auto resize(int32_t sz) const noexcept -> std::enable_if_t<!std::is_same_v<decltype(container_name.get<INDEX>(decltype(container_name)::public_tag_type())), void>, void> { \
	int32_t(container_name.resize<INDEX>(sz)); \
} \
template<typename INDEX> \
std::enable_if_t<!std::is_same_v<decltype(container_name.get<INDEX>(decltype(container_name)::public_tag_type())), void>, \
std::vector<std::remove_reference_t<decltype(container_name.get<INDEX>(decltype(container_name)::public_tag_type()))>>& > \
get_row() noexcept;


#define GET_SET_TV(index_name, container_name) \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name[t]), void>, decltype(container_name[t])> { \
	return container_name[t]; \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name[t]), void>, decltype(container_name[t])> { \
	return container_name[t]; \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type v) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name[t]), void>, void> { \
	return container_name.set(t, v); \
} \
template<typename INDEX, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type const& v) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name[t]), void>, void> { \
	return container_name.set(t, v); \
} \
template<typename INDEX> \
RELEASE_INLINE auto get_row() noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.view()), void>, decltype(container_name.view())> { \
	return container_name.view(); \
} \
template<typename INDEX> \
RELEASE_INLINE auto get_row() const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.view()), void>, decltype(container_name.view())> { \
	return container_name.view(); \
} \
template<typename INDEX> \
RELEASE_INLINE auto size() const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.view()), void>, decltype(int32_t(container_name.size<INDEX>()))> { \
	return int32_t(container_name.size<INDEX>()); \
} \
template<typename INDEX> \
RELEASE_INLINE auto resize(int32_t sz) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.view()), void>, decltype(container_name.resize<INDEX>(sz))> { \
	return container_name.resize<INDEX>(sz); \
} \
template<typename INDEX> \
RELEASE_INLINE auto as_vector() noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name>, std::add_lvalue_reference_t<decltype(container_name)>> { \
	return container_name; \
} \
template<typename INDEX> \
RELEASE_INLINE auto as_vector() const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name>, std::add_lvalue_reference_t<const decltype(container_name)>> { \
	return container_name; \
}

#define GET_SET_MAPPED(map_name, container_name) \
template<typename index_name, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) noexcept -> std::enable_if_t<map_name<index_name>::value != -1, decltype(container_name.get<map_name<index_name>::value>(t))> { \
	return container_name.get<map_name<index_name>::value>(t); \
} \
template<typename index_name, typename tag_type> \
RELEASE_INLINE auto get(tag_type t) const noexcept -> std::enable_if_t<map_name<index_name>::value != -1, decltype(container_name.get<map_name<index_name>::value>(t))> { \
	return container_name.get<map_name<index_name>::value>(t); \
} \
template<typename index_name, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type v) noexcept -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && map_name<index_name>::value != -1, decltype(container_name.set<map_name<index_name>::value>(t, v))> { \
	return container_name.set<map_name<index_name>::value>(t, v); \
} \
template<typename index_name, typename tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, value_type const& v) noexcept -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && map_name<index_name>::value != -1, decltype(container_name.set<map_name<index_name>::value>(t, v))> { \
	return container_name.set<map_name<index_name>::value>(t, v); \
} \
template<typename index_name> \
RELEASE_INLINE auto get_row() noexcept -> std::enable_if_t<map_name<index_name>::value != -1, decltype(container_name.get_row<map_name<index_name>::value>())> { \
	return container_name.get_row<map_name<index_name>::value>(); \
} \
template<typename index_name> \
RELEASE_INLINE auto get_row() const noexcept -> std::enable_if_t<map_name<index_name>::value != -1, decltype(container_name.get_row<map_name<index_name>::value>())> { \
	return container_name.get_row<map_name<index_name>::value>(); \
} \
template<typename index_name> \
RELEASE_INLINE auto size() const noexcept -> std::enable_if_t<map_name<index_name>::value != -1, int32_t> { \
	return container_name.size(); \
} \
template<typename index_name> \
RELEASE_INLINE auto resize(int32_t sz) noexcept -> std::enable_if_t<map_name<index_name>::value != -1, void> { \
}



#define GET_SET_TFV(index_name, container_name) \
template<typename INDEX, typename tag_type, typename inner_tag_type> \
RELEASE_INLINE auto get(tag_type t, inner_tag_type u) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get(t,u)), void>, decltype(container_name.get(t,u))> { \
	return container_name.get(t,u); \
} \
template<typename INDEX, typename tag_type, typename inner_tag_type> \
RELEASE_INLINE auto get(tag_type t, inner_tag_type u) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get(t,u)), void>, decltype(container_name.get(t,u))> { \
	return container_name.get(t,u); \
} \
template<typename INDEX, typename tag_type, typename inner_tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, inner_tag_type u, value_type v) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name.get(t,u)), void>, void> { \
	return container_name.set(t, u, v); \
} \
template<typename INDEX, typename tag_type, typename inner_tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, inner_tag_type u, value_type const& v) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name.get(t,u)), void>, void> { \
	return container_name.set(t, u, v); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get_row(tag_type t) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, decltype(container_name.get_row(t))> { \
	return container_name.get_row(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get_row(tag_type t) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, decltype(container_name.get_row(t))> { \
	return container_name.get_row(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto size(tag_type t) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, int32_t> { \
	return int32_t(container_name.size(t)); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto resize(tag_type t, int32_t sz) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, void> { \
	container_name.resize(sz); \
}

#define GET_SET_TDV(index_name, container_name) \
template<typename INDEX, typename tag_type, typename inner_tag_type> \
RELEASE_INLINE auto get(tag_type t, inner_tag_type u) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get(t,u)), void>, decltype(container_name.get(t,u))> { \
	return container_name.get(t,u); \
} \
template<typename INDEX, typename tag_type, typename inner_tag_type> \
RELEASE_INLINE auto get(tag_type t, inner_tag_type u) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get(t,u)), void>, decltype(container_name.get(t,u))> { \
	return container_name.get(t,u); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get_row(tag_type t) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, decltype(container_name.get_row(t))> { \
	return container_name.get_row(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get_row(tag_type t) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, decltype(container_name.get_row(t))> { \
	return container_name.get_row(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto size(tag_type t) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, int32_t> { \
	return int32_t(container_name.size(t)); \
}

#define GET_SET_TFV_VV(index_name, container_name) \
template<typename INDEX, typename tag_type, typename inner_tag_type> \
RELEASE_INLINE auto get(tag_type t, inner_tag_type u) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get(t,u)), void>, decltype(container_name.get(t,u))> { \
	return container_name.get(t,u); \
} \
template<typename INDEX, typename tag_type, typename inner_tag_type> \
RELEASE_INLINE auto get(tag_type t, inner_tag_type u) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get(t,u)), void>, decltype(container_name.get(t,u))> { \
	return container_name.get(t,u); \
} \
template<typename INDEX, typename tag_type, typename inner_tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, inner_tag_type u, value_type v) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name.get(t,u)), void>, void> { \
	return container_name.set(t, u, v); \
} \
template<typename INDEX, typename tag_type, typename inner_tag_type, typename value_type> \
RELEASE_INLINE auto set(tag_type t, inner_tag_type u, value_type const& v) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_trivially_copyable_v<value_type> && !std::is_same_v<decltype(container_name.get(t,u)), void>, void> { \
	return container_name.set(t, u, v); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get_row(tag_type t) noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, decltype(container_name.get_row(t))> { \
	return container_name.get_row(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto get_row(tag_type t) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, decltype(container_name.get_row(t))> { \
	return container_name.get_row(t); \
} \
template<typename INDEX, typename tag_type> \
RELEASE_INLINE auto size(tag_type t) const noexcept -> std::enable_if_t<std::is_same_v<INDEX, index_name> && !std::is_same_v<decltype(container_name.get_row(t)), void>, int32_t> { \
	return int32_t(container_name.size(t)); \
}

#define ARRAY_BACKING(container_name) \
template<typename obj_type> \
RELEASE_INLINE auto array_backing() noexcept -> std::enable_if_t<!std::is_same_v<decltype(container_name.array_backing<obj_type>()), void>, decltype(container_name.array_backing<obj_type>())> { \
	return container_name.array_backing<obj_type>(); \
} \
template<typename obj_type> \
RELEASE_INLINE auto array_backing() const noexcept -> std::enable_if_t<!std::is_same_v<decltype(container_name.array_backing<obj_type>()), void>, decltype(container_name.array_backing<obj_type>())> { \
	return container_name.array_backing<obj_type>(); \
}

#define ARRAY_BACKING_BASE(container_name) \
template<typename obj_type> \
RELEASE_INLINE auto array_backing() noexcept -> std::enable_if_t<std::is_same_v<typename decltype(container_name)::contents_type, obj_type>, std::add_lvalue_reference_t<decltype(container_name)>> { \
	return container_name; \
} \
template<typename obj_type> \
RELEASE_INLINE auto array_backing() const noexcept -> std::enable_if_t<std::is_same_v<typename decltype(container_name)::contents_type, obj_type>, std::add_lvalue_reference_t<std::add_const_t<decltype(container_name)>>> { \
	return container_name; \
}

template<typename value_type, typename tag_type, typename allocator = std::allocator<value_type>, bool padded = false>
class tagged_vector {
private:
	std::vector<value_type, allocator> storage;
public:
	using public_value_type = value_type;
	using public_tag_type = tag_type;

	tagged_vector() {
		if constexpr(padded)
			storage.resize(1);
	}
	const value_type& operator[](tag_type t) const {
		return *(storage.data() + to_index(t) + int32_t(padded));
	}
	value_type& operator[](tag_type t) {
		return *(storage.data() + to_index(t) + int32_t(padded));
	}
	template<typename ...T>
	tag_type emplace_back(T&& ... ts) {
		storage.emplace_back(std::forward<T>(ts)...);
		return tag_type(static_cast<value_base_of<tag_type>>(storage.size() - 1));
	}
	value_type& safe_get(tag_type t) {
		if (to_index(t) + int32_t(padded) >= storage.size())
			storage.resize(to_index(t) + int32_t(padded) + 1);
		return storage[to_index(t) + int32_t(padded)];
	}
	void set(tag_type t, value_type v) {
		*(storage.data() + to_index(t) + int32_t(padded)) = v;
	}
	auto data() const { return storage.data() + int32_t(padded); }
	auto data() { return storage.data() + int32_t(padded); }
	auto array() const { return storage.data() + int32_t(padded); }
	auto array() { return storage.data() + int32_t(padded); }
	auto begin() const { return storage.begin() + int32_t(padded); }
	auto end() const { return storage.end(); }
	auto begin() { return storage.begin() + int32_t(padded); }
	auto end() { return storage.end(); }
	template<typename = void>
	int32_t size() const { return int32_t(storage.size()) - int32_t(padded); }
	void resize(size_t size) { storage.resize(size + size_t(padded)); }
	template<typename = void>
	void resize(int32_t size) { storage.resize(size_t(size + int32_t(padded))); }
	void reserve(size_t size) { storage.reserve(size + size_t(padded)); }
	void pop_back() { storage.pop_back(); }
	void push_back(value_type v) { storage.push_back(v); }
	tagged_array_view<value_type, tag_type> view() {
		return tagged_array_view<value_type, tag_type>(storage.data() + int32_t(padded),
#ifdef _DEBUG
			int32_t(storage.size()) - int32_t(padded)
#else
			0
#endif
			);
	};
	tagged_array_view<value_type const, tag_type> view() const {
		return tagged_array_view<value_type const, tag_type>(storage.data() + int32_t(padded),
#ifdef _DEBUG
			int32_t(storage.size()) - int32_t(padded)
#else
			0
#endif
		);
	};

	template<typename i_type>
	auto get(tag_type t) noexcept -> decltype(struct_wrapper<value_type>::template get_member<i_type>(*(storage.data() + to_index(t) + int32_t(padded)))) {
		return struct_wrapper<value_type>::template get_member<i_type>(*(storage.data() + to_index(t) + int32_t(padded)));
	}
	template<typename i_type>
	auto get(tag_type t) const noexcept -> decltype(struct_wrapper<value_type>::template get_member<i_type>(*(storage.data() + to_index(t) + int32_t(padded)))) {
		return struct_wrapper<value_type>::template get_member<i_type>(*(storage.data() + to_index(t) + int32_t(padded)));
	}
	template<typename i_type>
	void set(tag_type t, decltype(struct_wrapper<value_type>::template get_member<i_type>(*(storage.data() + to_index(t) + int32_t(padded)))) j) noexcept {
		struct_wrapper<value_type>::template get_member<i_type>(*(storage.data() + to_index(t) + int32_t(padded))) = j;
	}

};

template<typename tag_type, typename allocator, bool padded>
class tagged_vector<bitfield_type, tag_type, allocator, padded> {
public:
	using value_type = bitfield_type;
private:
	std::vector<value_type, allocator> storage;
public:
	bool operator[](tag_type t) const {
		return bit_vector_test(storage.data() + int32_t(padded), to_index(t));
	}
	bool safe_get(tag_type t) {
		if(to_index(t) / (sizeof(bitfield_type) * 8) + int32_t(padded) >= storage.size())
			storage.resize((to_index(t)) / 8 + int32_t(padded) + 1);
		return bit_vector_test(storage.data() + int32_t(padded), to_index(t));
	}
	void set(tag_type t, bool value) {
		bit_vector_set(storage.data() + int32_t(padded), to_index(t), value);
	}
	void safe_set(tag_type t, bool value) {
		if(to_index(t) / (sizeof(bitfield_type) * 8) + int32_t(padded) >= storage.size())
			storage.resize((to_index(t)) / 8 + int32_t(padded) + 1);
		bit_vector_set(storage.data() + int32_t(padded), to_index(t), value);
	}
	auto data() const { return storage.data(); }
	auto data() { return storage.data(); }
	auto array() const { return storage.data() + int32_t(padded); }
	auto array() { return storage.data() + int32_t(padded); }
	auto begin() const { return storage.begin() + int32_t(padded); }
	auto end() const { return storage.end(); }
	auto begin() { return storage.begin() + int32_t(padded); }
	auto end() { return storage.end(); }
	template<typename = void>
	size_t size() const { return (storage.size() - int32_t(padded)) * sizeof(bitfield_type) * 8; }
	template<typename = void>
	void resize(size_t size) { storage.resize((size + 7) / 8 + size_t(padded)); }

	tagged_array_view<value_type, tag_type> view() {
		return tagged_array_view<value_type, tag_type>(storage.data() + int32_t(padded),
#ifdef _DEBUG
			int32_t(storage.size()) - int32_t(padded)
#else
			0
#endif
			);
	};
	tagged_array_view<value_type const, tag_type> view() const {
		return tagged_array_view<value_type const, tag_type>(storage.data() + int32_t(padded),
#ifdef _DEBUG
			int32_t(storage.size()) - int32_t(padded)
#else
			0
#endif
			);
	};
};

template<typename value_type, typename variable_tag_type, typename fixed_tag_type, typename allocator = std::allocator<value_type>>
class tagged_fixed_2dvector {
private:
	std::vector<value_type, allocator> storage;
	uint32_t _inner_size = 1;
public:
	value_type & get(variable_tag_type outer, fixed_tag_type inner) {
		return storage[(uint32_t)to_index(inner) + (uint32_t)to_index(outer) * _inner_size];
	}
	const value_type & get(variable_tag_type outer, fixed_tag_type inner) const {
		return storage[(uint32_t)to_index(inner) + (uint32_t)to_index(outer) * _inner_size];
	}
	void set(variable_tag_type outer, fixed_tag_type inner, value_type v) {
		storage[(uint32_t)to_index(inner) + (uint32_t)to_index(outer) * _inner_size] = v;
	}
	tagged_array_view<value_type, fixed_tag_type> get_row(variable_tag_type outer) {
		return tagged_array_view<value_type, fixed_tag_type>((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size), int32_t(_inner_size));
	}
	tagged_array_view<const value_type, fixed_tag_type> get_row(variable_tag_type outer) const {
		return tagged_array_view<const value_type, fixed_tag_type>((const value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size), int32_t(_inner_size));
	}
	value_type& safe_get(variable_tag_type outer, fixed_tag_type inner) {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return storage[(uint32_t)to_index(inner) + (uint32_t)to_index(outer) * _inner_size];
	}
	void reset(uint32_t new_inner_size) {
		storage.clear();
		_inner_size = new_inner_size != 0 ? new_inner_size : 1;
	}
	size_t size() const { return storage.size(); }
	int32_t size(variable_tag_type) const { return int32_t(_inner_size); }
	size_t outer_size() const { return storage.size() / _inner_size; }
	uint32_t inner_size() const { return _inner_size; }
	auto data() const { return storage.data(); }
	auto data() { return storage.data(); }
	auto begin() const { return storage.begin(); }
	auto end() const { return storage.end(); }
	auto begin() { return storage.begin(); }
	auto end() { return storage.end(); }
	void resize(size_t outer_size) { storage.resize(outer_size * _inner_size); }
	void reserve(size_t outer_size) { storage.reserve(outer_size * _inner_size); }
};


template<typename value_type, typename variable_tag_type, typename fixed_tag_type, typename allocator>
class tagged_fixed_blocked_2dvector {
public:
	constexpr static size_t block_size = 64;

	struct alignas(block_size) block_s {
		unsigned char data[block_size] = { 0 };
	};
private:
	std::vector<block_s, typename std::allocator_traits<allocator>::template rebind_alloc<block_s>> storage;
	uint32_t _inner_size = 1;
	uint32_t observable_inner_size = 1;
public:
	value_type & get(variable_tag_type outer, fixed_tag_type inner) {
		return *((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size) + (uint32_t)to_index(inner));
	}
	const value_type & get(variable_tag_type outer, fixed_tag_type inner) const {
		return *((const value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size) + (uint32_t)to_index(inner));
	}
	void set(variable_tag_type outer, fixed_tag_type inner, value_type v) {
		*((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size) + (uint32_t)to_index(inner)) = v;
	}
	tagged_array_view<value_type, fixed_tag_type> get_row(variable_tag_type outer) {
		return tagged_array_view<value_type, fixed_tag_type>((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size), _inner_size * block_size / sizeof(value_type));
	}
	tagged_array_view<const value_type, fixed_tag_type> get_row(variable_tag_type outer) const {
		return tagged_array_view<const value_type, fixed_tag_type>((const value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size), _inner_size * block_size / sizeof(value_type));
	}
	tagged_array_view<value_type, fixed_tag_type> safe_get_row(variable_tag_type outer) {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return tagged_array_view<value_type, fixed_tag_type>((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size), _inner_size * block_size / sizeof(value_type));
	}
	tagged_array_view<const value_type, fixed_tag_type> safe_get_row(variable_tag_type outer) const {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return tagged_array_view<const value_type, fixed_tag_type>((const value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size), _inner_size * block_size / sizeof(value_type));
	}
	value_type& safe_get(variable_tag_type outer, fixed_tag_type inner) {
		const auto n2 = ((uint32_t)to_index(outer) + 1ui32) * _inner_size;
		if (n2 >= storage.size())
			storage.resize(n2);
		return *((value_type*)(storage.data() + (uint32_t)to_index(outer) * _inner_size) + (uint32_t)to_index(inner));
	}
	void reset(uint32_t new_inner_size) {
		storage.clear();
		observable_inner_size = new_inner_size;
		_inner_size = (new_inner_size * sizeof(value_type) + (block_size - 1)) / block_size;
	}
	size_t size() const { return storage.size(); }
	int32_t size(variable_tag_type) const { return int32_t(observable_inner_size); }
	size_t outer_size() const { return storage.size() / _inner_size; }
	uint32_t inner_size() const { return observable_inner_size; }
	auto data() const { return storage.data(); }
	auto data() { return storage.data(); }
	void resize(size_t outer_size) { storage.resize(outer_size * _inner_size); }
	void reserve(size_t outer_size) { storage.reserve(outer_size * _inner_size); }
};

enum key_modifiers {
	modifiers_none = 0x0,
	modifiers_alt = 0x4,
	modifiers_ctrl = 0x2,
	modifiers_shift = 0x1,
	modifiers_ctrl_shift = 0x3,
	modifiers_ctrl_alt = 0x6,
	modifiers_alt_shift = 0x5,
	modifiers_all = 0x7
};

enum class virtual_key : uint8_t {
	NONE = 0x00,
	LBUTTON = 0x01,
	RBUTTON = 0x02,
	CANCEL = 0x03,
	MBUTTON = 0x04,
	XBUTTON_1 = 0x05,
	XBUTTON_2 = 0x06,
	BACK = 0x08,
	TAB = 0x09,
	CLEAR = 0x0C,
	RETURN = 0x0D,
	SHIFT = 0x10,
	CONTROL = 0x11,
	MENU = 0x12,
	PAUSE = 0x13,
	CAPITAL = 0x14,
	KANA = 0x15,
	JUNJA = 0x17,
	FINAL = 0x18,
	KANJI = 0x19,
	ESCAPE = 0x1B,
	CONVERT = 0x1C,
	NONCONVERT = 0x1D,
	ACCEPT = 0x1E,
	MODECHANGE = 0x1F,
	SPACE = 0x20,
	PRIOR = 0x21,
	NEXT = 0x22,
	END = 0x23,
	HOME = 0x24,
	LEFT = 0x25,
	UP = 0x26,
	RIGHT = 0x27,
	DOWN = 0x28,
	SELECT = 0x29,
	PRINT = 0x2A,
	EXECUTE = 0x2B,
	SNAPSHOT = 0x2C,
	INSERT = 0x2D,
	DELETE_KEY = 0x2E,
	HELP = 0x2F,
	NUM_0 = 0x30,
	NUM_1 = 0x31,
	NUM_2 = 0x32,
	NUM_3 = 0x33,
	NUM_4 = 0x34,
	NUM_5 = 0x35,
	NUM_6 = 0x36,
	NUM_7 = 0x37,
	NUM_8 = 0x38,
	NUM_9 = 0x39,
	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,
	LWIN = 0x5B,
	RWIN = 0x5C,
	APPS = 0x5D,
	SLEEP = 0x5F,
	NUMPAD0 = 0x60,
	NUMPAD1 = 0x61,
	NUMPAD2 = 0x62,
	NUMPAD3 = 0x63,
	NUMPAD4 = 0x64,
	NUMPAD5 = 0x65,
	NUMPAD6 = 0x66,
	NUMPAD7 = 0x67,
	NUMPAD8 = 0x68,
	NUMPAD9 = 0x69,
	MULTIPLY = 0x6A,
	ADD = 0x6B,
	SEPARATOR = 0x6C,
	SUBTRACT = 0x6D,
	DECIMAL = 0x6E,
	DIVIDE = 0x6F,
	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,
	F13 = 0x7C,
	F14 = 0x7D,
	F15 = 0x7E,
	F16 = 0x7F,
	F17 = 0x80,
	F18 = 0x81,
	F19 = 0x82,
	F20 = 0x83,
	F21 = 0x84,
	F22 = 0x85,
	F23 = 0x86,
	F24 = 0x87,
	NAVIGATION_VIEW = 0x88,
	NAVIGATION_MENU = 0x89,
	NAVIGATION_UP = 0x8A,
	NAVIGATION_DOWN = 0x8B,
	NAVIGATION_LEFT = 0x8C,
	NAVIGATION_RIGHT = 0x8D,
	NAVIGATION_ACCEPT = 0x8E,
	NAVIGATION_CANCEL = 0x8F,
	NUMLOCK = 0x90,
	SCROLL = 0x91,
	OEM_NEC_EQUAL = 0x92,
	LSHIFT = 0xA0,
	RSHIFT = 0xA1,
	LCONTROL = 0xA2,
	RCONTROL = 0xA3,
	LMENU = 0xA4,
	RMENU = 0xA5,
	SEMICOLON = 0xBA,
	PLUS = 0xBB,
	COMMA = 0xBC,
	MINUS = 0xBD,
	PERIOD = 0xBE,
	FORWARD_SLASH = 0xBF,
	TILDA = 0xC0,
	OPEN_BRACKET = 0xDB,
	BACK_SLASH = 0xDC,
	CLOSED_BRACKET = 0xDD,
	QUOTE = 0xDE
};


inline char ascii_to_lower(char in) {
	constexpr static char converted[256] = {
		//      0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F
		/*0*/'\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07','\x08','\x09','\x0A','\x0B','\x0C','\x0D','\x0E','\x0F',
		/*1*/'\x10','\x11','\x12','\x13','\x14','\x15','\x16','\x17','\x18','\x19','\x1A','\x1B','\x1C','\x1D','\x1E','\x1F',
		/*2*/  ' ' ,  '!' ,  '\"',  '#' ,  '$' ,  '%' ,  '&' ,  '\'',  '(' ,  ')' ,  '*' ,  '+' ,  ',' ,  '-' ,  '.' ,  '/' ,
		/*3*/  '0' ,  '1' ,  '2' ,  '3' ,  '4' ,  '5' ,  '6' ,  '7' ,  '8' ,  '9' ,  ':' ,  ';' ,  '<' ,  '=' ,  '>' ,  '?' ,
		/*4*/  '@' ,  'a' ,  'b' ,  'c' ,  'd' ,  'e' ,  'f' ,  'g' ,  'h' ,  'i' ,  'j' ,  'k' ,  'l' ,  'm' ,  'n' ,  'o' ,
		/*5*/  'p' ,  'q' ,  'r' ,  's' ,  't' ,  'u' ,  'v' ,  'w' ,  'x' ,  'y' ,  'z' ,  '[' ,  '\\',  ']' ,  '^' ,  '_' ,
		/*6*/  '`' ,  'a' ,  'b' ,  'c' ,  'd' ,  'e' ,  'f' ,  'g' ,  'h' ,  'i' ,  'j' ,  'k' ,  'l',   'm' ,  'n' ,  'o' ,
		/*7*/  'p' ,  'q' ,  'r' ,  's' ,  't' ,  'u' ,  'v' ,  'w' ,  'x' ,  'y' ,  'z' ,  '{' ,  '|',   '}' ,  '~' ,'\x7F',
		/*8*/'\x80','\x81','\x82','\x83','\x84','\x85','\x86','\x87','\x88','\x89','\x8A','\x8B','\x8C','\x8D','\x8E','\x8F',
		/*9*/'\x90','\x91','\x92','\x93','\x94','\x95','\x96','\x97','\x98','\x99','\x9A','\x9B','\x9C','\x9D','\x9E','\x9F',
		/*A*/'\xA0','\xA1','\xA2','\xA3','\xA4','\xA5','\xA6','\xA7','\xA8','\xA9','\xAA','\xAB','\xAC','\xAD','\xAE','\xAF',
		/*B*/'\xB0','\xB1','\xB2','\xB3','\xB4','\xB5','\xB6','\xB7','\xB8','\xB9','\xBA','\xBB','\xBC','\xBD','\xBE','\xBF',
		/*C*/'\xC0','\xC1','\xC2','\xC3','\xC4','\xC5','\xC6','\xC7','\xC8','\xC9','\xCA','\xCB','\xCC','\xCD','\xCE','\xCF',
		/*D*/'\xD0','\xD1','\xD2','\xD3','\xD4','\xD5','\xD6','\xD7','\xD8','\xD9','\xDA','\xDB','\xDC','\xDD','\xDE','\xDF',
		/*E*/'\xE0','\xE1','\xE2','\xE3','\xE4','\xE5','\xE6','\xE7','\xE8','\xE9','\xEA','\xEB','\xEC','\xED','\xEE','\xEF',
		/*F*/'\xF0','\xF1','\xF2','\xF3','\xF4','\xF5','\xF6','\xF7','\xF8','\xF9','\xFA','\xFB','\xFC','\xFD','\xFE','\xFF'
	};

	return converted[(uint8_t)in];
}

inline char16_t lower_ascii(char16_t in) {
	if(in <= 255)
		return char16_t(ascii_to_lower(char(in)));
	else
		return in;
}

inline char16_t* _u16itoa(uint32_t i, char16_t* buffer) {
	if (i >= 10) {
		const auto num = i / 10;
		const auto rem = i % 10;

		const auto tail = _u16itoa(num, buffer);
		tail[0] = static_cast<char16_t>(u'0' + rem);
		return tail + 1;
	} else {
		buffer[0] = static_cast<char16_t>(u'0' + i);
		return buffer + 1;
	}
}

inline char16_t* u16itoa(int32_t i, char16_t* buffer) {
	if (i < 0) {
		buffer[0] = u'-';
		const auto res = _u16itoa(uint32_t(-i), buffer + 1);
		*res = char16_t(0);
		return res;
	} else {
		const auto res = _u16itoa(uint32_t(i), buffer);
		*res = char16_t(0);
		return res;
	}
}

inline int64_t u16atoui(char16_t const* start, char16_t const* end) {
	int64_t value = 0;
	for(; start != end; ++start) {
		if((*start >= u'0') & (*start <= u'9')) {
			value = value * 10i64 + int64_t(*start - u'0');
		} else
			return value;
	}
	return value;
}

inline int64_t u16atoi(char16_t const* start, char16_t const* end) {
	if(start == end)
		return 0i64;
	if(*start == u'-')
		return -1i64 * u16atoui(start + 1, end);
	else
		return u16atoui(start, end);
}

enum class display_type : uint8_t {
	integer,
	fp_one_place,
	fp_two_places,
	fp_three_places,
	percent,
	neutral_integer,
	neutral_percent,
	exact_integer,
	percent_fp_one_place,
	currency
};

template<typename value_type>
value_type wrapped_modf(value_type value_in, value_type* int_out) {
	if constexpr(std::is_same_v<value_type, float> || std::is_same_v<value_type, double>)
		return modf(value_in, int_out);
	else {
		*int_out = value_in;
		return value_type(0);
	}
}

template<typename value_type>
inline char16_t* put_pos_value_in_buffer(char16_t* dest, display_type display_as, value_type value) {
#ifdef _DEBUG
	if constexpr(std::is_same_v<value_type, float> || std::is_same_v<value_type, double>) {
		if(std::isnan(value))
			std::abort();
	}
#endif
	switch(display_as) {
		case display_type::neutral_percent:
		case display_type::percent:
		{
			uint32_t int_value = uint32_t(value_type(100.0) * value + value_type(0.5));
			if(int_value == 0 && value != value_type(0)) {
				dest[0] = u'<';
				dest[1] = u'1';
				dest[2] = u'%';
				dest[3] = char16_t(0);
				return dest + 3;
			} else {
				auto value_end = _u16itoa(int_value, dest);
				*value_end = u'%';
				*(value_end + 1) = char16_t(0);
				return value_end + 1;
			}
		}
		case display_type::exact_integer:
		{
			uint32_t int_value = uint32_t(value + value_type(0.5));
			auto value_end = _u16itoa(int_value, dest);
			*value_end = char16_t(0);
			return value_end;
		}
		default:
		case display_type::integer:
		case display_type::neutral_integer:
		{
			uint64_t int_value = uint64_t(value + value_type(0.5));
			if(int_value < 10'000) {
				auto value_end = _u16itoa(uint32_t(int_value), dest);
				*value_end = char16_t(0);
				return value_end;
			} else if(int_value < 1'000'000) {
				int_value /= 1'000;
				auto value_end = _u16itoa(uint32_t(int_value), dest);
				*value_end = u'k';
				*(value_end+1) = char16_t(0);
				return value_end + 1;
			} else {
				double integer_part = 0.0;
				double fractional_part = modf(double(value) / double(1'000'000) + 0.05, &integer_part);

				auto value_end = _u16itoa(uint32_t(integer_part), dest);
				*value_end = u'.';

				uint32_t f_value = uint32_t(fractional_part * 10.0);

				if(f_value == 0ui32) {
					*(value_end + 1) = u'0';
					*(value_end + 2) = u'M';
					*(value_end + 3) = char16_t(0);
					return value_end + 3;
				} else {
					auto new_value_end = _u16itoa(f_value, value_end + 1);
					*new_value_end = u'M';
					*(new_value_end + 1) = char16_t(0);
					return new_value_end + 1;
				}
			}
		}
		case display_type::currency:
		{
			uint64_t int_value = uint64_t(value + value_type(0.5));

			if(value < value_type(10)) {
				value_type integer_part = value_type(0);
				value_type fractional_part = wrapped_modf(value_type(value + value_type(0.005)), &integer_part);

				auto value_end = _u16itoa(uint32_t(integer_part), dest);
				*value_end = u'.';

				uint32_t f_value = uint32_t(fractional_part * value_type(100));
				if(f_value == 0ui32) {
					*(value_end + 1) = u'0';
					*(value_end + 2) = u'0';
					*(value_end + 3) = u'\u00A3';
					*(value_end + 4) = char16_t(0);
					return value_end + 4;
				} else if(f_value < 10ui32) {
					*(value_end + 1) = u'0';
					auto new_value_end = _u16itoa(f_value, value_end + 2);
					*new_value_end = u'\u00A3';
					*(new_value_end + 1) = char16_t(0);
					return new_value_end + 1;
				} else {
					auto new_value_end = _u16itoa(f_value, value_end + 1);
					*new_value_end = u'\u00A3';
					*(new_value_end + 1) = char16_t(0);
					return new_value_end + 1;
				}
			} else if(value < value_type(100)) {
				value_type integer_part = value_type(0);
				value_type fractional_part = wrapped_modf(value_type(value + value_type(0.05)), &integer_part);

				auto value_end = _u16itoa(uint32_t(integer_part), dest);
				*value_end = u'.';

				uint32_t f_value = uint32_t(fractional_part * value_type(10));
				if(f_value == 0ui32) {
					*(value_end + 1) = u'0';
					*(value_end + 2) = u'\u00A3';
					*(value_end + 3) = char16_t(0);
					return value_end + 3;
				} else {
					auto new_value_end = _u16itoa(f_value, value_end + 1);
					*new_value_end = u'\u00A3';
					*(new_value_end + 1) = char16_t(0);
					return new_value_end + 1;
				}
			} else if(int_value < 10'000) {
				auto value_end = _u16itoa(uint32_t(int_value), dest);
				*value_end = u'\u00A3';
				*(value_end + 1) = char16_t(0);
				return value_end + 1;
			} else if(int_value < 1'000'000) {
				int_value /= 1'000;
				auto value_end = _u16itoa(uint32_t(int_value), dest);
				*value_end = u'k';
				*(value_end + 1) = u'\u00A3';
				*(value_end + 2) = char16_t(0);
				return value_end + 2;
			} else {
				double integer_part = 0.0;
				double fractional_part = modf(double(value) / double(1'000'000) + 0.05, &integer_part);

				auto value_end = _u16itoa(uint32_t(integer_part), dest);
				*value_end = u'.';

				uint32_t f_value = uint32_t(fractional_part * 10.0);

				if(f_value == 0ui32) {
					*(value_end + 1) = u'0';
					*(value_end + 2) = u'M';
					*(value_end + 3) = u'\u00A3';
					*(value_end + 4) = char16_t(0);
					return value_end + 5;
				} else {
					auto new_value_end = _u16itoa(f_value, value_end + 1);
					*new_value_end = u'M';
					*(new_value_end + 1) = u'\u00A3';
					*(new_value_end + 2) = char16_t(0);
					return new_value_end + 2;
				}
			}
		}
		case display_type::percent_fp_one_place:
		{
			value_type integer_part = value_type(0);
			value_type fractional_part = wrapped_modf(value_type(value * value_type(100.0) + value_type(0.05)), &integer_part);

			uint32_t int_value = uint32_t(integer_part);
			auto value_end = _u16itoa(int_value, dest);
			*value_end = u'.';

			uint32_t f_value = uint32_t(fractional_part * value_type(10));
			if(f_value == 0ui32) {
				*(value_end + 1) = u'0';
				*(value_end + 2) = u'%';
				*(value_end + 3) = char16_t(0);
				return value_end + 3;
			} else {
				auto new_value_end = _u16itoa(f_value, value_end + 1);
				*(new_value_end) = u'%';
				*(new_value_end + 1) = char16_t(0);
				return new_value_end + 1;
			}
		}
		case display_type::fp_one_place:
		{
			value_type integer_part = value_type(0);
			value_type fractional_part = wrapped_modf(value_type(value + value_type(0.05)), &integer_part);

			uint32_t int_value = uint32_t(integer_part);
			auto value_end = _u16itoa(int_value, dest);
			*value_end = u'.';

			uint32_t f_value = uint32_t(fractional_part * value_type(10));
			if(f_value == 0ui32) {
				*(value_end + 1) = u'0';
				*(value_end + 2) = char16_t(0);
				return value_end + 2;
			} else {
				auto new_value_end = _u16itoa(f_value, value_end + 1);
				*new_value_end = char16_t(0);
				return new_value_end;
			}
		}
		case display_type::fp_two_places:
		{
			value_type integer_part = value_type(0);
			value_type fractional_part = wrapped_modf(value_type(value + value_type(0.005)), &integer_part);

			uint32_t int_value = uint32_t(integer_part);
			auto value_end = _u16itoa(int_value, dest);
			*value_end = u'.';

			uint32_t f_value = uint32_t(fractional_part * value_type(100));
			if(f_value == 0ui32) {
				*(value_end + 1) = u'0';
				*(value_end + 2) = u'0';
				*(value_end + 3) = char16_t(0);
				return value_end + 3;
			} else if(f_value < 10ui32) {
				*(value_end + 1) = u'0';
				auto new_value_end = _u16itoa(f_value, value_end + 2);
				*new_value_end = char16_t(0);
				return new_value_end;
			} else {
				auto new_value_end = _u16itoa(f_value, value_end + 1);
				*new_value_end = char16_t(0);
				return new_value_end;
			}
		}
		case display_type::fp_three_places:
		{
			value_type integer_part = value_type(0);
			value_type fractional_part = wrapped_modf(value_type(value + value_type(0.0005)), &integer_part);

			uint32_t int_value = uint32_t(integer_part);
			auto value_end = _u16itoa(int_value, dest);
			*value_end = u'.';

			uint32_t f_value = uint32_t(fractional_part * value_type(1000));
			if(f_value == 0ui32) {
				*(value_end + 1) = u'0';
				*(value_end + 2) = u'0';
				*(value_end + 3) = u'0';
				*(value_end + 4) = char16_t(0);
				return value_end + 4;
			} else if(f_value < 10ui32) {
				*(value_end + 1) = u'0';
				*(value_end + 2) = u'0';
				auto new_value_end = _u16itoa(f_value, value_end + 3);
				*new_value_end = char16_t(0);
				return new_value_end;
			} else if(f_value < 100ui32) {
				*(value_end + 1) = u'0';
				auto new_value_end = _u16itoa(f_value, value_end + 2);
				*new_value_end = char16_t(0);
				return new_value_end;
			} else {
				auto new_value_end = _u16itoa(f_value, value_end + 1);
				*new_value_end = char16_t(0);
				return new_value_end;
			}
		}
	}
}

template<typename value_type>
inline char16_t* put_value_in_buffer(char16_t* dest, display_type display_as, value_type value) {
	if constexpr(!std::is_unsigned_v<value_type>) {
		if(value < value_type(0)) {
			*dest = u'-';
			return put_pos_value_in_buffer(dest + 1, display_as, -value);
		} else {
			return put_pos_value_in_buffer(dest, display_as, value);
		}
	} else {
		return put_pos_value_in_buffer(dest, display_as, value);
	}
}

inline std::pair<char*, bool> bom_test(char* buffer, size_t buffer_size) {
	const bool is_utf8 = buffer_size >= 3 && (((uint8_t)buffer[0] == 0xEF) & ((uint8_t)buffer[1] == 0xBB) & ((uint8_t)buffer[2] == 0xBF));
	if (is_utf8)
		return std::make_pair(buffer + 3, true);
	else
		return std::make_pair(buffer, false);
}

struct vector_backed_string_data {
	uint32_t offset;
	uint16_t length;
	uint16_t high_mask;
};

template<typename char_type>
struct vector_backed_string {
	union {
		vector_backed_string_data vbs;
		const char_type* ptr;
	} data;
	vector_backed_string() { data.vbs = vector_backed_string_data{0, 0, (uint16_t)(-1)}; }
	vector_backed_string(const char_type* c) { data.ptr = c; }
	vector_backed_string(const char_type* start, const char_type* end, std::vector<char_type>& vec) {
		data.vbs.offset = static_cast<uint32_t>(vec.size());
		data.vbs.length = (uint16_t)(end - start);
		data.vbs.high_mask = (uint16_t)(-1);
		vec.insert(vec.end(), start, end);
	}
	vector_backed_string(uint32_t offset, uint16_t length) {
		data.vbs.offset = offset;
		data.vbs.length = length;
		data.vbs.high_mask = (uint16_t)(-1);
	}
	vector_backed_string(const std::basic_string<char_type>& str, std::vector<char_type>& vec) {
		data.vbs.offset = (uint32_t)vec.size();
		data.vbs.length = (uint16_t)(str.length());
		data.vbs.high_mask = (uint16_t)(-1);
		vec.insert(vec.end(), str.begin(), str.end());
	}
	static vector_backed_string<char_type> create_unique(const std::basic_string<char_type>& str, std::vector<char_type>& vec) {
		const auto search_result = std::search(vec.begin(), vec.end(), str.begin(), str.end());
		if (search_result != vec.end()) {
			return vector_backed_string<char_type>(static_cast<uint32_t>(search_result - vec.begin()), static_cast<uint16_t>(str.length()));
		} else {
			return vector_backed_string<char_type>(str, vec);
		}
	}
	static vector_backed_string<char_type> create_unique(const char_type* start, const char_type* end, std::vector<char_type>& vec) {
		const auto search_result = std::search(vec.begin(), vec.end(), start, end);
		if (search_result != vec.end()) {
			return vector_backed_string<char_type>(static_cast<uint32_t>(search_result - vec.begin()), static_cast<uint16_t>(end - start));
		} else {
			return vector_backed_string<char_type>(start, end, vec);
		}
	}
	int32_t length() const {
		return data.vbs.high_mask == (uint16_t)-1 ? data.vbs.length : (data.ptr ? static_cast<int32_t>(std::char_traits<char_type>::length(data.ptr)) : 0);
	}
	const char_type* get_str(const std::vector<char_type>& vec) const {
		return data.vbs.high_mask == (uint16_t)-1 ? vec.data() + data.vbs.offset : data.ptr;
	}
	std::basic_string<char_type> get_string(const std::vector<char_type>& vec) const {
		return data.vbs.high_mask == (uint16_t)-1 ? std::basic_string<char_type>(vec.data() + data.vbs.offset, size_t(data.vbs.length)) : std::basic_string<char_type>(data.ptr);
	}
	void operator=(vector_backed_string other) {
		static_assert(sizeof(data.ptr) >= sizeof(data.vbs));
		data.ptr = other.data.ptr;
	}
	bool operator==(vector_backed_string other) const {
		return other.data.ptr == data.ptr;
	}
};

template<typename char_type>
struct vector_backed_string_less {
	const std::vector<char_type>& backing;
	vector_backed_string_less(const std::vector<char_type>& b) : backing(b) {}
	bool operator()(vector_backed_string<char_type> a, vector_backed_string<char_type> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len < b_len)
			return true;
		else if (a_len > b_len)
			return false;
		else {
			const char_type* a_str = a.get_str(backing);
			const char_type* b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (a_str[i] < b_str[i])
					return true;
				if (a_str[i] > b_str[i])
					return false;
			}
		}
		return false;
	}
};

template<typename char_type>
struct vector_backed_string_lex_less {
	const std::vector<char_type>& backing;
	vector_backed_string_lex_less(const std::vector<char_type>& b) : backing(b) {}
	bool operator()(vector_backed_string<char_type> a, vector_backed_string<char_type> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		const auto min_length = std::min(a_len, b_len);

		const char_type* a_str = a.get_str(backing);
		const char_type* b_str = b.get_str(backing);
		for(int32_t i = 0; i < min_length; ++i) {
			if(a_str[i] < b_str[i])
				return true;
			if(a_str[i] > b_str[i])
				return false;
		}
		
		if(a_len < b_len)
			return true;
		return false;
	}
};

struct vector_backed_string_less_ci {
	const std::vector<char>& backing;
	vector_backed_string_less_ci(const std::vector<char>& b) : backing(b) {}
	bool operator()(vector_backed_string<char> a, vector_backed_string<char> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len < b_len)
			return true;
		else if (a_len > b_len)
			return false;
		else {
			const char* a_str = a.get_str(backing);
			const char* b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (ascii_to_lower(a_str[i]) < ascii_to_lower(b_str[i]))
					return true;
				if (ascii_to_lower(a_str[i]) > ascii_to_lower(b_str[i]))
					return false;
			}
		}
		return false;
	}
};

template<typename char_type>
struct vector_backed_string_equality {
	const std::vector<char_type>& backing;
	vector_backed_string_equality(const std::vector<char_type>& b) : backing(b) {}
	bool operator()(vector_backed_string<char_type> a, vector_backed_string<char_type> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len != b_len)
			return false;
		else {
			const auto a_str = a.get_str(backing);
			const auto b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (a_str[i] != b_str[i])
					return false;
			}
		}
		return true;
	}
};

struct vector_backed_string_equality_ci {
	const std::vector<char>& backing;
	vector_backed_string_equality_ci(const std::vector<char>& b) : backing(b) {}
	bool operator()(vector_backed_string<char> a, vector_backed_string<char> b) const {
		const auto a_len = a.length();
		const auto b_len = b.length();
		if (a_len != b_len)
			return false;
		else {
			const auto a_str = a.get_str(backing);
			const auto b_str = b.get_str(backing);
			for (int32_t i = a_len - 1; i >= 0; --i) {
				if (ascii_to_lower(a_str[i]) != ascii_to_lower(b_str[i]))
					return false;
			}
		}
		return true;
	}
};


template<typename T, typename I = uint32_t, typename allocator = std::allocator<T>>
class v_vector {
public:
	std::vector<T, typename std::allocator_traits<allocator>::template rebind_alloc<T>> elements;
	std::vector<uint32_t, typename std::allocator_traits<allocator>::template rebind_alloc<uint32_t>> index;

	v_vector() {
		index.push_back(0ui32);
	}

	void clear() {
		index.clear();
		elements.clear();
		index.push_back(I());
	}

	void insert(typename std::vector<T>::iterator it, T elem) {
		size_t pos = std::distance(elements.begin(), it);
		elements.insert(it, elem);
		size_t i = 0;
		const auto isize = index.size();
		for (; index[i] < pos && i != isize; ++i)
			;
		if (i != isize && index[i] == pos)
			++i;
		for (; i != isize; ++i)
			++index[i];
	}

	void new_row() {
		index.push_back(static_cast<uint32_t>(elements.size()));
	}

	void expand_rows(uint32_t sz) {
		if (index.size() < sz + 1)
			index.resize(sz + 1, static_cast<uint32_t>(elements.size()));
	}

	uint32_t row_size() const {
		return static_cast<uint32_t>(index.size());
	}

	std::pair<T*, T*> get_range(I i) {
		std::pair<T*, T*> p;
		p.first = elements.data() + *(index.data() + to_index(i));
		if (to_index(i) + 1 < index.size()) {
			p.second = elements.data() + *(index.data() + to_index(i) + 1);
		} else {
			p.second = elements.data() + elements.size();
		}
		return p;
	}

	std::pair<T const*, T const*> get_range(I i) const {
		std::pair<T const*, T const*> p;
		p.first = elements.data() + *(index.data() + to_index(i));
		if (to_index(i) + 1 < index.size()) {
			p.second = elements.data() + *(index.data() + to_index(i) + 1);
		} else {
			p.second = elements.data() + elements.size();
		}
		return p;
	}

	tagged_array_view<T, int32_t> get_row(I i) {
		auto const rng = get_range(i);
		return tagged_array_view<T, int32_t>(rng.first, int32_t(rng.second - rng.first));
	}
	tagged_array_view<T const, int32_t> get_row(I i) const {
		auto const rng = get_range(i);
		return tagged_array_view<T const, int32_t>(rng.first, int32_t(rng.second - rng.first));
	}

	int32_t size(I i) const {
		auto const r = get_range(i);
		return int32_t(r.second - r.first);
	}

	T& get(I x, uint32_t y) {
		auto it = elements.begin() + index[to_index(x)] + y;
		return *it;
	}

	const T& get(I x, uint32_t y) const {
		const auto it = elements.cbegin() + index[to_index(x)] + y;
		return *it;
	}

	void set(I x, uint32_t y, T value) {
		auto it = elements.begin() + index[to_index(x)] + y;
		*it = value;
	}

	void push_back(const T& elem) {
		elements.push_back(elem);
	}

	void add_to_row(I i, const T& elem) {
		if (to_index(i) >= index.size()) {
			index.resize(to_index(i) + 1, uint32_t(elements.size()));
		}
		elements.insert(elements.begin() + index[to_index(i)], elem);
		size_t sz = index.size();
		for (uint32_t j = uint32_t(to_index(i) + 1); j < sz; ++j) {
			++index[j];
		}
	}

	template<typename IT>
	void add_range_to_row(I i, const IT& start, const IT& end) {
		if (to_index(i) >= index.size()) {
			index.resize(to_index(i) + 1, uint32_t(elements.size()));
		}
		elements.insert(elements.begin() + index[to_index(i)], start, end);
		const uint32_t num_added(static_cast<uint32_t>(end - start));
		size_t sz = index.size();
		for (uint32_t j = uint32_t(to_index(i) + 1); j < sz; ++j) {
			index[j] += num_added;
		}
	}

	void append_to_row(I i, const T& elem) {
		size_t sz = index.size();
		if (to_index(i) + 1 < sz)
			elements.insert(elements.begin() + index[to_index(i) + 1], elem);
		else
			elements.push_back(elem);
		for (uint32_t j = uint32_t(to_index(i) + 1); j < sz; ++j) {
			++index[j];
		}
	}
};

inline int32_t find_best_relative_prime(int32_t prime_to) {
	int32_t base = (prime_to* 4 + 1) / 5;

	if(std::gcd(prime_to, base) == 1)
		return base;

	int32_t n = 1;
	while(true) {
		if(std::gcd(prime_to, base + n) == 1)
			return base + n;
		if(std::gcd(prime_to, base - n) == 1)
			return base - n;

		++n;
	}
}


class integer_iterator {
protected:
	int32_t index = 0;
public:
	constexpr integer_iterator() noexcept {};
	constexpr explicit integer_iterator(int32_t i) noexcept : index(i) {};
	constexpr explicit integer_iterator(uint32_t i) noexcept : index(int32_t(i)) {};

	constexpr int32_t operator*() const noexcept {
		return index;
	}
	integer_iterator& operator+=(int32_t i) noexcept {
		index += i;
		return *this;
	}
	constexpr integer_iterator operator+(int32_t i) const noexcept {
		return integer_iterator(index + i);
	}
	constexpr integer_iterator& operator-=(int32_t i) noexcept {
		index -= i;
		return *this;
	}
	constexpr integer_iterator operator-(int32_t i) const noexcept {
		return integer_iterator(index - i);
	}
	constexpr int32_t operator-(integer_iterator const& o) const noexcept {
		return index - o.index;
	}
	constexpr int32_t operator[](int32_t i) const noexcept {
		return index + i;
	}
	constexpr bool operator==(integer_iterator const& o) const noexcept {
		return index == o.index;
	}
	constexpr bool operator!=(integer_iterator const& o) const noexcept {
		return index != o.index;
	}
	constexpr bool operator<(integer_iterator const& o) const noexcept {
		return index < o.index;
	}
	constexpr bool operator>(integer_iterator const& o) const noexcept {
		return index > o.index;
	}
	constexpr bool operator<=(integer_iterator const& o) const noexcept {
		return index <= o.index;
	}
	constexpr bool operator>=(integer_iterator const& o) const noexcept {
		return index <= o.index;
	}
	integer_iterator& operator++() noexcept {
		++index;
		return *this;
	}
	integer_iterator& operator--() noexcept {
		--index;
		return *this;
	}
	integer_iterator operator++(int) noexcept {
		return integer_iterator(index++);
	}
	integer_iterator operator--(int) noexcept {
		return integer_iterator(index--);
	}

	using iterator_category = std::random_access_iterator_tag;
	using value_type = int32_t;
	using difference_type = int32_t;
	using pointer = int32_t*;
	using reference = int32_t&;
};

constexpr std::pair<integer_iterator, integer_iterator> integer_range(int32_t b, int32_t e) noexcept {
	return std::pair<integer_iterator, integer_iterator>(integer_iterator(b), integer_iterator(e));
}

namespace graphics {
	struct color_rgba {
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 255;
	};

	inline uint32_t rgba_to_int(color_rgba c) {
		union local_u {
			color_rgba color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.color = c;
		return u.value;
	}

	inline color_rgba int_to_rgba(uint32_t v) {
		union local_u {
			color_rgba color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.value = v;
		return u.color;
	}

	struct color_rgb {
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
	};

	inline uint32_t rgb_to_int(color_rgb c) {
		union local_u {
			color_rgb color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.color = c;
		return u.value;
	}

	inline color_rgb int_to_rgb(uint32_t v) {
		union local_u {
			color_rgb color;
			uint32_t value;
			local_u() { value = 0; }
		} u;
		u.value = v;
		return u.color;
	}

	namespace detail {
		inline float hue_helper(float v) {
			if(v < 0.0f) v += 1.0;
			if(v > 1.0f) v -= 1.0;
			if(v < 1.0f / 6.0f) return 6.0f * v;
			if(v < 0.5f) return 1.0f;
			if(v < 2.0f / 3.0f) return (2.0f / 3.0f - v) * 6.0f;
			return 0.0f;
		}
	}

	inline color_rgb hue_to_rgb(float h) {
		float r = detail::hue_helper(h + 1.0f / 3.0f);
		float g = detail::hue_helper(h);
		float b = detail::hue_helper(h - 1.0f / 3.0f);

		return color_rgb{ uint8_t(r * 255.0f + 0.5f),  uint8_t(g * 255.0f + 0.5f),  uint8_t(b * 255.0f + 0.5f) };
	}

}
