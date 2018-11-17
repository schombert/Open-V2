#pragma once
#include "common\\common.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>

#define VARIABLE_LAYOUT_STRUCT_OF_ARRAYS

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
struct bitfield_type {};
#else
using bitfield_type = bool;
#endif

namespace {
	template<typename tag_type, int32_t size, typename ... T>
	class variable_layout_tagged_vector_impl;

	template<typename tag_type, int32_t size>
	class variable_layout_tagged_vector_impl <tag_type, size> {
	public:
		struct data {
		};

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		__forceinline static void reset(data& d) {}
		template<typename T>
		__forceinline static void get(tag_type i, const data&) {}
		template<typename T>
		__forceinline static void get(tag_type i, data&) {}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data&, T const&) {}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data&, T) {}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static size_t size_impl(tag_type i, data const& obj, CONTEXT&& ... c) { return 0; }
#else
		__forceinline static void reset(data& d) {}
		template<typename T>
		__forceinline static void get(const data&) {}
		template<typename T>
		__forceinline static void get(data&) {}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(data&, T const&) {}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(data&, T) {}

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
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		struct data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::data {
			union d_union_type {
				member_type values[size];
				uint8_t padding[(uint32_t(size) * uint32_t(sizeof(member_type)) + 63ui32) & ~63ui32];

				static_assert(sizeof(values) <= sizeof(padding));
				static_assert(((uint32_t(size) * uint32_t(sizeof(member_type)) + 63ui32) & ~63ui32) % 64ui32 == 0);

				d_union_type() {
					std::uninitialized_value_construct_n(values, size);
				}
			} d_union;
			data() : d_union() {}
		};

		static_assert(sizeof(data) % 64ui32 == 0);
#else
		struct data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::typename data {
			member_type value;
			data() : value() {}
		};
#endif

#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		__forceinline static void reset(data& d) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::reset(d);

			std::destroy_n(d.d_union.values, values, size);
			std::uninitialized_value_construct_n(d.d_union.values, size);
		}

		template<typename T>
		__forceinline static auto get(tag_type i, data const& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type const&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.d_union.values[to_index(i)];
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		__forceinline static auto get(tag_type i, data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.d_union.values[to_index(i)];
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T const& val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.d_union.values[to_index(i)] = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<T>(i, dat, val);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.d_union.values[to_index(i)] = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<T>(i, dat, val);
		}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::serialize_object_impl(output, max, obj, std::forward<CONTEXT>(c)...);

			if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
				serialization::serialize_array(output, obj.dat.d_union.values, size_t(max));
			} else {
				for(int32_t i = 0; i < max; ++i)
					serialization::tagged_serializer<index_type, member_type>::serialize_object(output, dat.d_union.values[i], std::forward<CONTEXT>(c)...);
			}
			
		}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::deserialize_object_impl(input, max, obj, std::forward<CONTEXT>(c)...);

			if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
				serialization::deserialize_array(input, obj.dat.d_union.values, size_t(max));
			} else {
				for(int32_t i = 0; i < max; ++i)
					serialization::tagged_serializer<index_type, member_type>::deserialize_object(input, dat.d_union.values[i], std::forward<CONTEXT>(c)...);
			}
		}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) {
			if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) +
					size_t(serialization::tagged_serializer<index_type, member_type>::size(obj.dat.d_union.values[0], std::forward<CONTEXT>(c)...) * max);
			} else {
				return return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + 
					std::transform_reduce(obj.dat.d_union.values, obj.dat.d_union.values + max, 0ui64, std::plus<>(), [&](member_type const* m) {
						return serialization::tagged_serializer<index_type, member_type>::size(*m, std::forward<CONTEXT>(c)...);
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
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<T>(dat, val);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.value = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<T>(dat, val);
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
		struct data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::data {
			union d_union_type {
				uint64_t values[uint32_t(size + 63) / 64ui32];
				uint8_t padding[(uint32_t(size + 7) / 8ui32 + 63ui32) & ~63ui32];

				static_assert(sizeof(values) <= sizeof(padding));
				static_assert(((uint32_t(size + 7) / 8ui32 + 63ui32) & ~63ui32) % 64ui32 == 0);

				d_union_type() {
					std::uninitialized_value_construct_n(values, uint32_t(size + 63) / 64ui32);
				}
			} d_union;
			data() : d_union() {}
		};

		static_assert(sizeof(data) % 64ui32 == 0);

		__forceinline static void reset(data& d) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::reset(d);
			std::fill_n(d.d_union.values, uint32_t(size + 63) / 64, 0ui64);
		}

		template<typename T>
		__forceinline static auto get(tag_type i, data const& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, bool, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return bit_vector_test(dat.d_union.values, uint32_t(to_index(i)));
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		__forceinline static auto get(tag_type i, data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, bool, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return bit_vector_test(dat.d_union.values, uint32_t(to_index(i)));
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T const& val) {
			if constexpr(std::is_same_v<U, index_type>)
				bit_vector_set(dat.d_union.values, uint32_t(to_index(i)), val);
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<T>(i, dat, val);
		}
		template<typename U, typename T>
		__forceinline static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				bit_vector_set(dat.d_union.values, uint32_t(to_index(i)), val);
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<T>(i, dat, val);
		}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {
			auto count = uint32_t(max + 63) / 64ui32;
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::serialize_object_impl(output, max, obj, std::forward<CONTEXT>(c)...);
			serialization::serialize_array(output, obj.dat.d_union.values, count);
		}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {
			auto count = uint32_t(max + 63) / 64ui32;
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::deserialize_object_impl(input, max, obj, std::forward<CONTEXT>(c)...);
			serialization::deserialize_array(input, obj.dat.d_union.values, count);
		}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) {
			auto count = uint32_t(max + 63) / 64ui32;
			return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + sizeof(int64_t) * count;
		}
	};
#endif

	struct index_type_marker {};
}

template<typename tag_type, int32_t size, typename ... T>
class variable_layout_tagged_vector;

template<typename tag_type, int32_t size, typename ... T>
class serialization::serializer<variable_layout_tagged_vector<tag_type, size, T ...>>;

template<typename tag_type, int32_t container_size, typename ... T>
class variable_layout_tagged_vector {
private:
	using ptr_type = typename variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>::data;
	using container_type = typename variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>;
	ptr_type* const ptr;
	tag_type first_free;
	int32_t size_used = 0;
public:
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
			ptr_type::template set<index_type_marker>(ptr[i], first_free);
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
		if(!is_valid_index(first_free))
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
		if(int32_t(to_index(allocated)) + 1 > size_used)
			size_used = int32_t(to_index(allocated)) + 1;

		return allocated;
	}

	void release(tag_type i) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		container_type::template set<index_type_marker>(i, *ptr, first_free);
#else
		ptr_type::template set<index_type_marker>(ptr[to_index(i)], first_free);
#endif
		first_free = i;

		if(size_used - 1 == to_index(i)) {
			for(int32_t i = size_used - 1; i >= 0; --i) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
				if(container_type::template get<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), *ptr) == tag_type(typename tag_type::value_base_t(i))) {
					size_used = i + 1;
					return;
				}
#else
				if(container_type::template get<index_type_marker>(ptr[i]) == tag_type(typename tag_type::value_base_t(i))) {
					size_used = i + 1;
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
			container_type::template set<index_type_marker>(tag_type(tag_type::typename value_base_t(i)), *ptr, first_free);
			first_free = tag_type(tag_type::typename value_base_t(i));
		}
#else
		for(int32_t i = container_size - 1; i >= 0; --i) {
			container_type::reset(ptr[i]);
			container_type::template set<index_type_marker>(ptr[i], first_free);
			first_free = tag_type(tag_type::typename value_base_t(i));
		}
#endif
	}

	int32_t size() const {
		return size_used;
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
			if(container_type::template get<index_type_marker>(tag_type(tag_type::typename value_base_t(i)), *ptr) == tag_type(tag_type::typename value_base_t(i)))
				func(tag_type(tag_type::typename value_base_t(i)));
		}
	}
	template<typename F>
	void parallel_for_each(F const& func) const {
		concurrency::parallel_for(0, size_used, [&func, p = this->ptr](int32_t i) {
			if(container_type::template get<index_type_marker>(tag_type(tag_type::typename value_base_t(i)), *p) == tag_type(tag_type::typename value_base_t(i)))
				func(tag_type(tag_type::typename value_base_t(i)));
		});
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
		for(int32_t i = 0; i < size_used; ++i) {
			if(container_type::template get<index_type_marker>(ptr[i]) == tag_type(tag_type::typename value_base_t(i)))
				func(tag_type(tag_type::typename value_base_t(i)));
		}
	}
	template<typename F>
	void parallel_for_each(F const& func) const {
		concurrency::parallel_for(0, size_used, [&func, p = this->ptr](int32_t i) {
			if(container_type::template get<index_type_marker>(p[i]) == tag_type(tag_type::typename value_base_t(i)))
				func(tag_type(tag_type::typename value_base_t(i)));
		});
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
	using ptr_type = typename variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::data;
	using container_type = typename variable_layout_tagged_vector_impl<tag_type, container_size, T ...>;
	ptr_type* const ptr;
	int32_t size_used = 0;

	friend class serialization::serializer<variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>>;
public:

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

	void reset() {
		size_used = 0;
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		container_type::reset(*ptr);
#else
		for(int32_t i = container_size - 1; i >= 0; --i)
			container_type::reset(ptr[i]);
#endif
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
	__forceinline auto get(tag_type i) const -> decltype(container_type::template get<U>(static_cast<ptr_type const*>(ptr)[to_index(i)])) {
		return container_type::template get<U>(static_cast<ptr_type const*>(ptr)[to_index(i)]);
	}
	template<typename U>
	__forceinline auto get(tag_type i) -> decltype(container_type::template get<U>(i, *ptr)) {
		return container_type::template get<U>(i, *ptr);
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
			func(tag_type(tag_type::typename value_base_t(i)));
		}
	}
	template<typename F>
	void parallel_for_each(F const& func) const {
		concurrency::parallel_for(0, size_used, [&func](int32_t i) {
			func(tag_type(tag_type::typename value_base_t(i)));
		});
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
		tag_type temp[container_size];
		for(int32_t i = 0; i < obj.size_used; ++i) {
			if(obj.get<index_type_marker>(tag_type(tag_type::typename value_base_t(i))) == tag_type(tag_type::typename value_base_t(i)))
				temp[i] = tag_type(tag_type::typename value_base_t(i));
			else
				temp[i] = tag_type();
		}

		serialization::serialize(output, obj.size_used);
		serialization::serialize_array(output, temp, obj.size_used);
		variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
			output, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		serialization::serialize(output, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			if(obj.get<index_type_marker>(tag_type(tag_type::typename value_base_t(i))) == tag_type(tag_type::typename value_base_t(i))) {
				serialization::serialize(output, obj.get<index_type_marker>(tag_type(tag_type::typename value_base_t(i))));
				variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
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

		serialization::deserialize_array(input, obj.ptr->d_union.values, obj.size_used);
		variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
			input, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);

#else
		obj.reset();
		serialization::deserialize(input, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			serialization::deserialize(input, obj.get<index_type_marker>(tag_type(tag_type::typename value_base_t(i))));
			if(is_valid_index(obj.get<index_type_marker>(tag_type(tag_type::typename value_base_t(i))))) {
				variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
					input, obj.ptr[i], std::forward<CONTEXT>(c)...);
			}
		}
#endif

		obj.first_free = tag_type();
		for(int32_t i = container_size - 1; i >= 0; --i) {
			if(!is_valid_index(obj.get<index_type_marker>(tag_type(tag_type::typename value_base_t(i))))) {
				obj.set<index_type_marker>(tag_type(tag_type::typename value_base_t(i)), obj.first_free);
				obj.first_free = tag_type(tag_type::typename value_base_t(i));
			}
		}
	}
	template<typename ... CONTEXT>
	static size_t size(variable_layout_tagged_vector<tag_type, container_size, T ...>& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		return sizeof(obj.size_used) + serialization::serialize_size(tag_type()) * obj.size_used +
			variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(
				obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		return sizeof(obj.size_used) + serialization::serialize_size(tag_type()) * obj.size_used +
			std::transform_reduce(obj.ptr, obj.ptr + obj.size_used, 0ui64, std::plus<>(),
				[&](variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>* o_ptr) {
					return variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(*o_ptr, std::forward<CONTEXT>(c)...);
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
		variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
			output, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		serialization::serialize(output, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
				output, obj.ptr[i], std::forward<CONTEXT>(c)...);
		}
#endif
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		obj.reset();
		serialization::deserialize(input, obj.size_used);

		variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
			input, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);

#else
		obj.reset();
		serialization::deserialize(input, obj.size_used);
		for(int32_t i = 0; i < obj.size_used; ++i) {
			variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
				input, obj.ptr[i], std::forward<CONTEXT>(c)...);
		}
#endif
	}
	template<typename ... CONTEXT>
	static size_t size(variable_layout_tagged_vector<tag_type, container_size, T ...>& obj, CONTEXT&& ... c) {
#ifdef VARIABLE_LAYOUT_STRUCT_OF_ARRAYS
		return sizeof(obj.size_used) +
			variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(
				obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
#else
		return sizeof(obj.size_used) +
			std::transform_reduce(obj.ptr, obj.ptr + obj.size_used, 0ui64, std::plus<>(),
				[&](variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>* o_ptr) {
			return variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(*o_ptr, std::forward<CONTEXT>(c)...);
		});
#endif
	}
};
