#pragma once
#include "common\\common.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>
#include "ve.h"

#define VARIABLE_LAYOUT_STRUCT_OF_ARRAYS

namespace variable_layout_detail {
	template<typename tag_type, int32_t size, typename ... T>
	class variable_layout_tagged_vector_impl;

	template<typename tag_type, int32_t size>
	class variable_layout_tagged_vector_impl <tag_type, size> {
	public:
		template<typename T>
		using value_type = void;

		struct data {
		};

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		__forceinline static void reset(data& d) {}
		template<typename T>
		__forceinline static void get(tag_type i, const data&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename T>
		__forceinline static void get_row(data&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename T>
		__forceinline static void get_row(const data&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename T>
		__forceinline static void get(tag_type i, data&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data&, T const&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data&, T) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		__forceinline static void clear(tag_type i, data&) {}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) { return 0; }
#else
		__forceinline static void reset(data& d) {}
		template<typename T>
		__forceinline static void get(const data&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename T>
		__forceinline static void get(data&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(data&, T const&) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(data&, T) {
#ifdef _DEBUG
			assert(false);
#else
			__assume(0);
#endif
		}
		__forceinline static void clear(data&) {}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, data const& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, data& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static size_t size_impl(data const& obj, CONTEXT&& ... c) { return 0; }
#endif
	};

	template<typename tag_type, int32_t size, typename index_type, typename member_type, typename ... REST>
	class variable_layout_tagged_vector_impl<tag_type, size, index_type, member_type, REST ...> {
	public:
		template<typename T>
		using value_type = std::conditional_t<std::is_same_v<T, index_type>, member_type, typename variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template value_type<T>>;

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		struct data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::data {
			uint8_t pre_padding[64 * ((sizeof(member_type) + 63) / 64)];
			member_type values[size + ((size * sizeof(member_type)) % 64) / sizeof(member_type)];

			data() {
				std::uninitialized_value_construct_n(values - 1, size + 1 + ((size * sizeof(member_type)) % 64) / sizeof(member_type) );
			}
		};

		static_assert(sizeof(data) % 64ui32 == 0);
#else
		struct data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::data {
			member_type value;
			data() : value() {}
		};
#endif

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		__forceinline static void reset(data& d) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::reset(d);

			std::destroy_n(d.values - 1, size + 1 + ((size * sizeof(member_type)) % 64) / sizeof(member_type));
			std::uninitialized_value_construct_n(d.values - 1, size + 1 + ((size * sizeof(member_type)) % 64) / sizeof(member_type));
		}

		template<typename T>
		__forceinline static auto get(tag_type i, data const& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type const&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values[to_index(i)];
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		__forceinline static auto get(tag_type i, data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values[to_index(i)];
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		__forceinline static auto get_row(data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename T>
		__forceinline static auto get_row(const data& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type const*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T const& val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.values[to_index(i)] = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.d_union.values[to_index(i)] = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		__forceinline static void clear(tag_type i, data& dat) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::clear(i, dat);
			dat.values[to_index(i)] = member_type();
		}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::serialize_object_impl(output, max, obj, std::forward<CONTEXT>(c)...);

			if constexpr(serialization::tagged_serializer<index_type, member_type>::has_static_size) {
				if constexpr(serialization::tagged_serializer<index_type, member_type>::size() == 0) {
					// do nothing
				} else if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
					serialization::serialize_array(output, obj.values, max);
				} else {
					for(int32_t i = 0; i < max; ++i)
						serialization::tagged_serializer<index_type, member_type>::serialize_object(output, obj.d_union.values[i], std::forward<CONTEXT>(c)...);
				}

			} else {
				if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
					serialization::serialize_array(output, obj.values, max);
				} else {
					for(int32_t i = 0; i < max; ++i)
						serialization::tagged_serializer<index_type, member_type>::serialize_object(output, obj.d_union.values[i], std::forward<CONTEXT>(c)...);
				}
			}
			
		}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::deserialize_object_impl(input, max, obj, std::forward<CONTEXT>(c)...);

			if constexpr(serialization::tagged_serializer<index_type, member_type>::has_static_size) {
				if constexpr(serialization::tagged_serializer<index_type, member_type>::size() == 0) {
					// do nothing
				} else if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
					serialization::deserialize_array(input, obj.values, max);
				} else {
					for(int32_t i = 0; i < max; ++i)
						serialization::tagged_serializer<index_type, member_type>::deserialize_object(input, obj.d_union.values[i], std::forward<CONTEXT>(c)...);
				}

			} else {
				if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
					serialization::deserialize_array(input, obj.values + 1, max);
				} else {
					for(int32_t i = 0; i < max; ++i)
						serialization::tagged_serializer<index_type, member_type>::deserialize_object(input, obj.d_union.values[i], std::forward<CONTEXT>(c)...);
				}

			}
		}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) {
			if(max <= 1)
				return 0;

			if constexpr(serialization::tagged_serializer<index_type, member_type>::has_static_size) {
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + 
					size_t(max  * serialization::tagged_serializer<index_type, member_type>::size());
			} else if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) +
					size_t(serialization::tagged_serializer<index_type, member_type>::size(obj.d_union.values[0], std::forward<CONTEXT>(c)...) * max);
			} else {
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + 
					std::transform_reduce(obj.d_union.values, obj.d_union.values + max, 0ui64, std::plus<>(), [&](member_type const& m) {
						return serialization::tagged_serializer<index_type, member_type>::size(m, std::forward<CONTEXT>(c)...);
					});
			}
		}
#else
		__forceinline static void reset(data& d) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::reset(d);
			d.value = member_type();
		}

		template<typename T>
		__forceinline static auto get(data const& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type const&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.value;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(dat);
		}
		template<typename T>
		__forceinline static auto get(data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.value;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(dat);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(data& dat, T const& val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.value = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(dat, val);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.value = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(dat, val);
		}
		__forceinline static void clear(data& dat) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::clear(dat);
			dat.value = member_type();
		}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, data const& obj, CONTEXT&& ... c) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::serialize_object_impl(output, obj, std::forward<CONTEXT>(c)...);
			serialization::tagged_serializer<index_type, member_type>::serialize_object(output, obj.value, std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, data& obj, CONTEXT&& ... c) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::deserialize_object_impl(input, obj, std::forward<CONTEXT>(c)...);
			serialization::tagged_serializer<index_type, member_type>::deserialize_object(input, obj.value, std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static size_t size_impl(data const& obj, CONTEXT&& ... c) {
			return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(obj, std::forward<CONTEXT>(c)...) +
				serialization::tagged_serializer<index_type, member_type>::size(obj.value, std::forward<CONTEXT>(c)...);
		}
#endif
	};


#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
	template<typename tag_type, int32_t size, typename index_type, typename ... REST>
	class variable_layout_tagged_vector_impl<tag_type, size, index_type, bitfield_type, REST ...> {
	public:
		template<typename T>
		using value_type = std::conditional_t<std::is_same_v<T, index_type>, bitfield_type, typename variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template value_type<T>>;

		struct data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::data {
			union d_union_type {
				bitfield_type values[uint32_t(size + 7) / 8ui32];
				uint8_t padding[(uint32_t(size + 7) / 8ui32 + 63ui32) & ~63ui32];

				static_assert(sizeof(values) <= sizeof(padding));
				static_assert(((uint32_t(size + 7) / 8ui32 + 63ui32) & ~63ui32) % 64ui32 == 0);

				d_union_type() {
					std::fill_n(values, uint32_t(size + 7) / 8ui32, bitfield_type{ 0ui8 });
				}
			} d_union;
			data() : d_union() {}
		};

		static_assert(sizeof(data) % 64ui32 == 0);

		__forceinline static void reset(data& d) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::reset(d);
			std::fill_n(d.d_union.values, uint32_t(size + 7) / 8ui32, bitfield_type{ 0ui8 });
		}

		template<typename T>
		__forceinline static auto get(tag_type i, data const& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, bool, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return bit_vector_test(dat.d_union.values, uint32_t(to_index(i) + 1));
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		__forceinline static auto get(tag_type i, data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, bool, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return bit_vector_test(dat.d_union.values, uint32_t(to_index(i) + 1));
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		__forceinline static auto get_row(data& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, bitfield_type*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.d_union.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename T>
		__forceinline static auto get_row(const data& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, bitfield_type const*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.d_union.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T const& val) {
			if constexpr(std::is_same_v<U, index_type>)
				bit_vector_set(dat.d_union.values, uint32_t(to_index(i) + 1), val);
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				bit_vector_set(dat.d_union.values, uint32_t(to_index(i) + 1), val);
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		__forceinline static void clear(tag_type i, data& dat) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::clear(i, dat);
			bit_vector_set(dat.d_union.values, uint32_t(to_index(i) + 1), false);
		}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {
			auto count = uint32_t(max + 7) / 8ui32;
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::serialize_object_impl(output, max, obj, std::forward<CONTEXT>(c)...);
			serialization::serialize_array(output, obj.d_union.values, count);
		}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {
			auto count = uint32_t(max + 7) / 8ui32;
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::deserialize_object_impl(input, max, obj, std::forward<CONTEXT>(c)...);
			serialization::deserialize_array(input, obj.d_union.values, count);
		}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) {
			auto count = uint32_t(max + 7) / 8ui32;
			return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + sizeof(bitfield_type) * count;
		}
	};
#endif
}

struct index_type_marker {};

template<typename tag_type, int32_t size, typename ... T>
class variable_layout_tagged_vector;

template<typename tag_type, int32_t size, typename ... T>
class serialization::serializer<variable_layout_tagged_vector<tag_type, size, T ...>>;

template<typename tag_type, int32_t container_size, typename ... T>
class variable_layout_tagged_vector {
private:
	using ptr_type = typename variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>::data;
	using container_type = typename variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>;
	ptr_type* const ptr;
	tag_type first_free;
	int32_t size_used = 0;
public:
	using index_type = tag_type;

	friend class serialization::serializer<variable_layout_tagged_vector<tag_type, container_size, T ...>>;

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
	variable_layout_tagged_vector() : ptr((ptr_type*)_aligned_malloc(sizeof(ptr_type), 64)) {
		new (ptr) ptr_type();

		for(int32_t i = container_size - 1; i >= 0; --i) {
			container_type::template set<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), *ptr, first_free);
			first_free = tag_type(typename tag_type::value_base_t(i));
		}
	}
#else
	variable_layout_tagged_vector() : ptr((ptr_type*)_aligned_malloc(sizeof(ptr_type) * container_size, 64)) {
		std::uninitialized_value_construct_n(ptr, container_size);

		for(int32_t i = container_size - 1; i >= 0; --i) {
			container_type::template set<index_type_marker>(ptr[i], first_free);
			first_free = tag_type(typename tag_type::value_base_t(i));
		}
	}
#endif
	

	variable_layout_tagged_vector(variable_layout_tagged_vector const&) = delete;
	variable_layout_tagged_vector(variable_layout_tagged_vector &&) = delete;

	~variable_layout_tagged_vector() {
		_aligned_free(ptr);
	}

	tag_type get_new() {
#ifdef _DEBUG
		if(!::is_valid_index(first_free))
			std::abort();
#endif
		auto allocated = first_free;
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		first_free = container_type::template get<index_type_marker>(first_free, *ptr);
		container_type::template set<index_type_marker>(allocated, *ptr, allocated);
#else
		first_free = container_type::template get<index_type_marker>(ptr[to_index(first_free)]);
		container_type::template set<index_type_marker>(ptr[to_index(allocated)], allocated);
#endif
		if(int32_t(to_index(allocated) + 1) > size_used)
			size_used = int32_t(to_index(allocated) + 1);

		return allocated;
	}

	void release(tag_type i) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		container_type::clear(i, *ptr);
		container_type::template set<index_type_marker>(i, *ptr, first_free);
#else
		container_type::clear(ptr[to_index(i)]);
		container_type::template set<index_type_marker>(ptr[to_index(i)], first_free);
#endif
		first_free = i;

		if(size_used - 1 == to_index(i)) {
			for(int32_t j = size_used; j >= 0; --j) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
				if(container_type::template get<index_type_marker>(tag_type(typename tag_type::value_base_t(j)), *ptr) == tag_type(typename tag_type::value_base_t(j))) {
					size_used = j + 1;
					return;
				}
#else
				if(container_type::template get<index_type_marker>(ptr[j]) == tag_type(typename tag_type::value_base_t(j))) {
					size_used = j + 1;
					return;
				}
#endif
			}
			size_used = 0;
		}
	}

	void reset() {
		size_used = 0;
		first_free = tag_type();

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		container_type::reset(*ptr);

		for(int32_t i = container_size - 1; i >= 0; --i) {
			container_type::template set<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), *ptr, first_free);
			first_free = tag_type(typename tag_type::value_base_t(i));
		}
#else
		for(int32_t i = container_size - 1; i >= 0; --i) {
			container_type::reset(ptr[i]);
			container_type::template set<index_type_marker>(ptr[i], first_free);
			first_free = tag_type(typename tag_type::value_base_t(i));
		}
#endif
	}

	int32_t size() const {
		return size_used;
	}
	uint32_t vector_size() const {
		return ve::to_vector_size(uint32_t(size_used));
	}
	bool is_valid_index(tag_type t) const noexcept {
		return ::is_valid_index(t) & (int32_t(to_index(t)) < size_used);
	}

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
	template<typename U>
	__forceinline auto get(tag_type i) const -> decltype(container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr))) {
		return container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr));
	}
	template<typename U>
	__forceinline auto get(tag_type i) -> decltype(container_type::template get<U>(i, *ptr)) {
		return container_type::template get<U>(i, *ptr);
	}

	template<typename U>
	__forceinline auto get_row() const {
		if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U> const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), int32_t(uint32_t(size_used + 8) / 8ui32));
	}
	template<typename U>
	__forceinline auto get_row() {
		if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U>, tag_type>(container_type::template get_row<U>(*ptr), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type*>(ptr)), int32_t(uint32_t(size_used + 8) / 8ui32));
	}

	template<typename U, typename V>
	__forceinline std::enable_if_t<!std::is_trivially_copyable_v<V>> set(tag_type i, V const& val) {
		container_type::template set<U>(i, *ptr, val);
	}
	template<typename U, typename V>
	__forceinline std::enable_if_t<std::is_trivially_copyable_v<V>> set(tag_type i, V val) {
		container_type::template set<U>(i, *ptr, val);
	}

	template<typename F>
	void for_each(F const& func) const {
		for(int32_t i = 0; i < size_used; ++i) {
			if(container_type::template get<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), *ptr) == tag_type(typename tag_type::value_base_t(i)))
				func(tag_type(typename tag_type::value_base_t(i)));
		}
	}
	template<typename F, typename P = concurrency::auto_partitioner>
	void parallel_for_each(F const& func, P&& p = concurrency::auto_partitioner()) const {
		concurrency::parallel_for(0, size_used, [&func, p = this->ptr](int32_t i) {
			if(container_type::template get<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), *p) == tag_type(typename tag_type::value_base_t(i)))
				func(tag_type(typename tag_type::value_base_t(i)));
		}, p);
	}
#else
	template<typename U>
	__forceinline auto get(tag_type i) const -> decltype(container_type::template get<U>(static_cast<ptr_type const*>(ptr)[to_index(i)])) {
		return container_type::template get<U>(static_cast<ptr_type const*>(ptr)[to_index(i)]);
	}
	template<typename U>
	__forceinline auto get(tag_type i) -> decltype(container_type::template get<U>(ptr[to_index(i)])) {
		return container_type::template get<U>(ptr[to_index(i)]);
	}
	template<typename U, typename V>
	__forceinline std::enable_if_t<!std::is_trivially_copyable_v<V>> set(tag_type i, V const& val) {
		container_type::template set<U>(ptr[to_index(i)], val);
	}
	template<typename U, typename V>
	__forceinline std::enable_if_t<std::is_trivially_copyable_v<V>> set(tag_type i, V val) {
		container_type::template set<U>(ptr[to_index(i)], val);
	}

	template<typename F>
	void for_each(F const& func) const {
		for(int32_t i = 0; i < size_used ; ++i) {
			if(container_type::template get<index_type_marker>(ptr[i]) == tag_type(typename tag_type::value_base_t(i)))
				func(tag_type(typename tag_type::value_base_t(i)));
		}
	}
	template<typename F, typename P = concurrency::auto_partitioner>
	void parallel_for_each(F const& func, P&& p = concurrency::auto_partitioner()) const {
		concurrency::parallel_for(0, size_used, [&func, p = this->ptr](int32_t i) {
			if(container_type::template get<index_type_marker>(p[i]) == tag_type(typename tag_type::value_base_t(i)))
				func(tag_type(typename tag_type::value_base_t(i)));
		}, p);
	}
#endif
};

template<typename tag_type, int32_t size, typename ... T>
class variable_layout_contiguous_tagged_vector;

template<typename tag_type, int32_t size, typename ... T>
class serialization::serializer<variable_layout_contiguous_tagged_vector<tag_type, size, T ...>>;

template<typename tag_type, int32_t container_size, typename ... T>
class variable_layout_contiguous_tagged_vector {
private:
	using ptr_type = typename variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::data;
	using container_type = typename variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>;
	ptr_type* const ptr;
	int32_t size_used = 0;

	friend class serialization::serializer<variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>>;
public:
	using index_type = tag_type;

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
	variable_layout_contiguous_tagged_vector() : ptr((ptr_type*)_aligned_malloc(sizeof(ptr_type), 64)) {
		new (ptr) ptr_type();
	}
#else
	variable_layout_contiguous_tagged_vector() : ptr((ptr_type*)_aligned_malloc(sizeof(ptr_type) * container_size, 64)) {
		std::uninitialized_value_construct_n(ptr, container_size);
	}
#endif

	variable_layout_contiguous_tagged_vector(variable_layout_contiguous_tagged_vector const&) = delete;
	variable_layout_contiguous_tagged_vector(variable_layout_contiguous_tagged_vector &&) = delete;

	~variable_layout_contiguous_tagged_vector() {
		_aligned_free(ptr);
	}

	void resize(int32_t size) {
		size_used = size;
	}

	int32_t size() const {
		return size_used;
	}
	uint32_t vector_size() const {
		return ve::to_vector_size(uint32_t(size_used));
	}

	void reset() {
		size_used = 1;
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		container_type::reset(*ptr);
#else
		for(int32_t i = container_size - 1; i >= 0; --i)
			container_type::reset(ptr[i]);
#endif
	}

	void reset_nth(tag_type i) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		container_type::clear(i, *ptr);
#else
		container_type::clear(ptr[to_index(i)]);
#endif
	}

	bool is_valid_index(tag_type t) const noexcept {
		return ::is_valid_index(t) & (int32_t(to_index(t)) < size_used - 1);
	}

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
	template<typename U>
	__forceinline auto get(tag_type i) const -> decltype(container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr))) {
		return container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr));
	}
	template<typename U>
	__forceinline auto get(tag_type i) -> decltype(container_type::template get<U>(i, *ptr)) {
		return container_type::template get<U>(i, *ptr);
	}
	template<typename U>
	__forceinline auto get_row() const {
		if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U> const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), int32_t(uint32_t(size_used + 8) / 8ui32));
	}
	template<typename U>
	__forceinline auto get_row() {
		if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U>, tag_type>(container_type::template get_row<U>(*ptr), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), int32_t(uint32_t(size_used + 8) / 8ui32));
	}
	template<typename U, typename V>
	__forceinline std::enable_if_t<!std::is_trivially_copyable_v<V>> set(tag_type i, V const& val) {
		container_type::template set<U>(i, *ptr, val);
	}
	template<typename U, typename V>
	__forceinline std::enable_if_t<std::is_trivially_copyable_v<V>> set(tag_type i, V val) {
		container_type::template set<U>(i, *ptr, val);
	}
#else
	template<typename U>
	__forceinline auto get(tag_type i) const -> decltype(container_type::template get<U>(static_cast<ptr_type const*>(ptr)[to_index(i) + 1])) {
		return container_type::template get<U>(static_cast<ptr_type const*>(ptr)[to_index(i)]);
	}
	template<typename U>
	__forceinline auto get(tag_type i) -> decltype(container_type::template get<U>(ptr[to_index(i)])) {
		return container_type::template get<U>(ptr[to_index(i)]);
	}
	template<typename U, typename V>
	__forceinline std::enable_if_t<!std::is_trivially_copyable_v<V>> set(tag_type i, V const& val) {
		container_type::template set<U>(ptr[to_index(i)], val);
	}
	template<typename U, typename V>
	__forceinline std::enable_if_t<std::is_trivially_copyable_v<V>> set(tag_type i, V val) {
		container_type::template set<U>(ptr[to_index(i)], val);
	}
#endif

	template<typename F>
	void for_each(F const& func) const {
		for(int32_t i = 0; i < size_used; ++i) {
			func(tag_type(typename tag_type::value_base_t(i)));
		}
	}
	template<typename F, typename P = concurrency::auto_partitioner>
	void parallel_for_each(F const& func, P&& p = concurrency::auto_partitioner()) const {
		concurrency::parallel_for(0, size_used, [&func](int32_t i) {
			func(tag_type(typename tag_type::value_base_t(i)));
		}, p);
	}
};


template<typename tag_type, int32_t container_size, typename ... T>
class serialization::serializer<variable_layout_tagged_vector<tag_type, container_size, T ...>> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static void serialize_object(std::byte* &output, variable_layout_tagged_vector<tag_type, container_size, T ...> const& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		
		serialization::serialize(output, obj.size_used);

		for(int32_t i = 0; i < obj.size_used; ++i) {
			if(obj.get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))) == tag_type(typename tag_type::value_base_t(i))) {
				auto tag = tag_type(typename tag_type::value_base_t(i));
				serialization::serialize(output, tag);
			} else {
				auto tag = tag_type();
				serialization::serialize(output, tag);
			}
		}

		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
			output, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		serialization::serialize(output, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			if(obj.get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))) == tag_type(typename tag_type::value_base_t(i))) {
				serialization::serialize(output, obj.get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))));
				variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
					output, obj.ptr[i], std::forward<CONTEXT>(c)...);
			} else {
				serialization::serialize(output, tag_type());
			}
		}
#endif
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, variable_layout_tagged_vector<tag_type, container_size, T ...>& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		obj.reset();
		serialization::deserialize(input, obj.size_used);

		serialization::deserialize_array(input, obj.ptr->values, obj.size_used);
		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
			input, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);

#else
		obj.reset();
		serialization::deserialize(input, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			serialization::deserialize(input, obj.get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))));
			if(::is_valid_index(obj.get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))))) {
				variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
					input, obj.ptr[i], std::forward<CONTEXT>(c)...);
			}
		}
#endif

		obj.first_free = tag_type();
		for(int32_t i = container_size - 1; i >= 0; --i) {
			if(!::is_valid_index(obj.get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))))) {
				obj.set<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), obj.first_free);
				obj.first_free = tag_type(typename tag_type::value_base_t(i));
			}
		}
	}
	template<typename ... CONTEXT>
	static size_t size(variable_layout_tagged_vector<tag_type, container_size, T ...> const& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		return sizeof(obj.size_used) + serialization::serialize_size(tag_type()) * obj.size_used +
			variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(
				obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		return sizeof(obj.size_used) + serialization::serialize_size(tag_type()) * obj.size_used +
			std::transform_reduce(obj.ptr, obj.ptr + obj.size_used, 0ui64, std::plus<>(),
				[&](typename variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>::data const& o_ptr) {
					return variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(o_ptr, std::forward<CONTEXT>(c)...);
			});
#endif
	}
};

template<typename tag_type, int32_t container_size, typename ... T>
class serialization::serializer<variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static void serialize_object(std::byte* &output, variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...> const& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		serialization::serialize(output, obj.size_used);
		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
			output, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		serialization::serialize(output, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
				output, obj.ptr[i], std::forward<CONTEXT>(c)...);
		}
#endif
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		obj.reset();
		serialization::deserialize(input, obj.size_used);

		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
			input, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);

#else
		obj.reset();
		serialization::deserialize(input, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
				input, obj.ptr[i], std::forward<CONTEXT>(c)...);
		}
#endif
	}
	template<typename ... CONTEXT>
	static size_t size(variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...> const& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		return sizeof(obj.size_used) +
			variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(
				obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		return sizeof(obj.size_used) +
			std::transform_reduce(obj.ptr + 1, obj.ptr + obj.size_used, 0ui64, std::plus<>(),
				[&](typename variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::data const& o_ptr) {
			return variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(o_ptr, std::forward<CONTEXT>(c)...);
		});
#endif
	}
};

template<typename tag_type, typename value_type, int32_t outer_size, int32_t inner_size>
class fixed_vectorizable_2d_array {
private:
	static constexpr uint32_t extended_size = uint32_t(outer_size + (64ui32 / sizeof(value_type) - 1)) & ~(uint32_t(64ui32 / sizeof(value_type) - 1));
	struct data_line {
		uint8_t padding[64];
		value_type values[extended_size];
		data_line() {
			std::uninitialized_value_construct_n(values - 1, extended_size + 1);
		}
	};

	static_assert((extended_size * sizeof(value_type)) % 64 == 0);
	static_assert(extended_size >= outer_size);
	static_assert(sizeof(data_line) % 64 == 0);

	data_line* const ptr;
public:
	using index_type = tag_type;

	fixed_vectorizable_2d_array() : ptr((data_line*)_aligned_malloc(sizeof(data_line) * inner_size, 64)) {
		std::uninitialized_value_construct_n(ptr, inner_size);
	}

	fixed_vectorizable_2d_array(fixed_vectorizable_2d_array const&) = delete;
	fixed_vectorizable_2d_array(fixed_vectorizable_2d_array &&) = delete;

	~fixed_vectorizable_2d_array() {
		_aligned_free(ptr);
	}

	__forceinline value_type& get(tag_type t, int32_t index) {
		return ptr[index].values[to_index(t)];
	}
	__forceinline value_type const& get(tag_type t, int32_t index) const {
		return ptr[index].values[to_index(t)];
	}
	template<int32_t index>
	__forceinline value_type& get(tag_type t) {
		static_assert(index >= 0 && index < inner_size);
		return ptr[index].values[to_index(t)];
	}
	template<int32_t index>
	__forceinline value_type const& get(tag_type t) const {
		static_assert(index >= 0 && index < inner_size);
		return ptr[index].values[to_index(t)];
	}
	__forceinline tagged_array_view<value_type, tag_type> get_row(int32_t index, int32_t size) {
		return tagged_array_view<value_type, tag_type>(ptr[index].values, size);
	}
	__forceinline tagged_array_view<const value_type, tag_type> get_row(int32_t index, int32_t size) const {
		return tagged_array_view<const value_type, tag_type>(ptr[index].values, size);
	}
	template<int32_t index>
	__forceinline tagged_array_view<value_type, tag_type> get_row(int32_t size) {
		static_assert(index >= 0 && index < inner_size);
		return tagged_array_view<value_type, tag_type>(ptr[index].values, size);
	}
	template<int32_t index>
	__forceinline tagged_array_view<const value_type, tag_type> get_row(int32_t size) const {
		static_assert(index >= 0 && index < inner_size);
		return tagged_array_view<const value_type, tag_type>(ptr[index].values, size);
	}
	void reset() {
		std::uninitialized_value_construct_n(ptr, inner_size);
	}
};

template<typename tag_type, typename inner_tag_type, typename value_type, int32_t outer_size>
class varying_vectorizable_2d_array {
private:
	static constexpr uint32_t extended_size = 
		!std::is_same_v<value_type, bitfield_type> ?
			uint32_t(outer_size + (64ui32 / sizeof(value_type) - 1)) & ~(uint32_t(64ui32 / sizeof(value_type) - 1)) :
			uint32_t((outer_size + 7) / 8 + 63) & ~63ui32;
	using ret_type = std::conditional_t<std::is_same_v<value_type, bitfield_type>, bool, value_type&>;
	using const_ret_type = std::conditional_t<std::is_same_v<value_type, bitfield_type>, bool, value_type const&>;
	using param_type = std::conditional_t<std::is_same_v<value_type, bitfield_type>, bool, value_type>;
	using vector_type = value_type;

	struct data_line {
		uint8_t padding[64];
		value_type values[extended_size];
		data_line() {
			std::uninitialized_value_construct_n(values - 1, extended_size + 1);
		}
	};

	static_assert((extended_size * sizeof(value_type)) % 64 == 0);
	static_assert(std::is_same_v<value_type, bitfield_type> || extended_size >= outer_size);
	static_assert(sizeof(data_line) % 64 == 0);

	data_line* ptr = nullptr;
	int32_t current_size = 0;
public:
	using index_type = tag_type;

	varying_vectorizable_2d_array() {}
	varying_vectorizable_2d_array(varying_vectorizable_2d_array const&) = delete;
	varying_vectorizable_2d_array(varying_vectorizable_2d_array &&) = delete;

	~varying_vectorizable_2d_array() {
		if(ptr)
			_aligned_free(ptr);
		current_size = 0;
		ptr = nullptr;
	}

	void resize(int32_t new_size) {
		if(new_size > current_size) {
			if(ptr)
				_aligned_free(ptr);

			ptr = (data_line*)_aligned_malloc(sizeof(data_line) * new_size, 64);
			std::uninitialized_value_construct_n(ptr, new_size);

			current_size = new_size;
		}
	}

	__forceinline ret_type get(tag_type t, inner_tag_type index) {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return ptr[to_index(index)].values[to_index(t)];
		else
			return bit_vector_test(ptr[to_index(index)].values, to_index(t) + 1);
	}
	__forceinline const_ret_type get(tag_type t, inner_tag_type index) const {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return ptr[to_index(index)].values[to_index(t)];
		else
			return bit_vector_test(ptr[to_index(index)].values, to_index(t) + 1);
	}
	__forceinline void set(tag_type t, inner_tag_type index, param_type p) {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			ptr[to_index(index)].values[to_index(t)] = p;
		else
			bit_vector_set(ptr[to_index(index)].values, to_index(t) + 1, p);
	}
	
	__forceinline auto get_row(inner_tag_type index, int32_t size) {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return tagged_array_view<vector_type, tag_type>(ptr[to_index(index)].values, size);
		else
			return tagged_array_view<bitfield_type, tag_type>(
				ptr[to_index(index)].values,
				int32_t(uint32_t(size + 8) / 8ui32));
	}
	__forceinline auto get_row(inner_tag_type index, int32_t size) const {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return tagged_array_view<const vector_type, tag_type>(ptr[to_index(index)].values, size);
		else
			return tagged_array_view<const bitfield_type, tag_type>(
				ptr[to_index(index)].values,
				int32_t(uint32_t(size + 8) / 8ui32));
	}
	void reset() {
		if(ptr)
			std::uninitialized_value_construct_n(ptr, current_size);
	}

	void reset_row(inner_tag_type index) {
		if(ptr)
			std::uninitialized_value_construct_n(ptr + to_index(index), 1);
	}
};
