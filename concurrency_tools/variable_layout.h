#pragma once
#include "common\\common.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>
#include "ve.h"

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

		RELEASE_INLINE static void reset(data& d) {}
		
		template<typename T>
		RELEASE_INLINE static void get(tag_type i, const data&);
		template<typename T>
		RELEASE_INLINE static void get_row(data&);
		template<typename T>
		RELEASE_INLINE static void get_row(const data&);
		template<typename T>
		RELEASE_INLINE static void get(tag_type i, data&);
		template<typename U, typename T>
		RELEASE_INLINE static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data&, T const&);
		template<typename U, typename T>
		RELEASE_INLINE static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data&, T);
		RELEASE_INLINE static void clear(tag_type i, data&) {}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) { return 0; }

	};

	template<typename tag_type, int32_t size, typename index_type, typename member_type, typename ... REST>
	class variable_layout_tagged_vector_impl<tag_type, size, index_type, member_type, REST ...> {
	public:
		template<typename T>
		using value_type = std::conditional_t<std::is_same_v<T, index_type>, member_type, typename variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template value_type<T>>;

		constexpr static uint32_t members_count = 
			sizeof(member_type) <= 64 ? 
				(uint32_t(size) + (64ui32 / uint32_t(sizeof(member_type))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(member_type)) - 1ui32) :
				uint32_t(size) ;

		struct alignas(64) data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::data {
			uint8_t pre_padding[(sizeof(member_type) + 63ui32) & ~63ui32];
			member_type values[members_count];

			data() {
				std::uninitialized_value_construct_n(values - 1, members_count + 1);
			}
		};

		static_assert(members_count >= size);
		//static_assert(sizeof(member_type) > 64 || (sizeof(member_type) * members_count) % 64ui32 == 0);
		static_assert(sizeof(data) % 64ui32 == 0);

		RELEASE_INLINE static void reset(data& d) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::reset(d);

			std::destroy_n(d.values - 1, members_count + 1);
			std::uninitialized_value_construct_n(d.values - 1, members_count + 1);
		}

		template<typename T>
		RELEASE_INLINE static auto get(tag_type i, data const& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type const&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values[to_index(i)];
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		RELEASE_INLINE static auto get(tag_type i, data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type&, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values[to_index(i)];
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		RELEASE_INLINE static auto get_row(data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename T>
		RELEASE_INLINE static auto get_row(const data& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, member_type const*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename U, typename T>
		RELEASE_INLINE static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T const& val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.values[to_index(i)] = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		template<typename U, typename T>
		RELEASE_INLINE static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				dat.values[to_index(i)] = val;
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		RELEASE_INLINE static void clear(tag_type i, data& dat) {
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
						serialization::tagged_serializer<index_type, member_type>::serialize_object(output, obj.values[i], std::forward<CONTEXT>(c)...);
				}

			} else {
				if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
					serialization::serialize_array(output, obj.values, max);
				} else {
					for(int32_t i = 0; i < max; ++i)
						serialization::tagged_serializer<index_type, member_type>::serialize_object(output, obj.values[i], std::forward<CONTEXT>(c)...);
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
						serialization::tagged_serializer<index_type, member_type>::deserialize_object(input, obj.values[i], std::forward<CONTEXT>(c)...);
				}

			} else {
				if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
					serialization::deserialize_array(input, obj.values, max);
				} else {
					for(int32_t i = 0; i < max; ++i)
						serialization::tagged_serializer<index_type, member_type>::deserialize_object(input, obj.values[i], std::forward<CONTEXT>(c)...);
				}

			}
		}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) {
			if(max == 0)
				return 0;

			if constexpr(serialization::tagged_serializer<index_type, member_type>::has_static_size) {
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + 
					size_t(max  * serialization::tagged_serializer<index_type, member_type>::size());
			} else if constexpr(serialization::tagged_serializer<index_type, member_type>::has_simple_serialize) {
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) +
					size_t(serialization::tagged_serializer<index_type, member_type>::size(obj.values[0], std::forward<CONTEXT>(c)...) * max);
			} else {
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + 
					std::transform_reduce(obj.values, obj.values + max, 0ui64, std::plus<>(), [&](member_type const& m) {
						return serialization::tagged_serializer<index_type, member_type>::size(m, std::forward<CONTEXT>(c)...);
					});
			}
		}

	};


	template<typename tag_type, int32_t size, typename index_type, typename ... REST>
	class variable_layout_tagged_vector_impl<tag_type, size, index_type, bitfield_type, REST ...> {
	public:
		template<typename T>
		using value_type = std::conditional_t<std::is_same_v<T, index_type>, bitfield_type, typename variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template value_type<T>>;

		constexpr static uint32_t bytes_count = ((uint32_t(size + 7)) / 8ui32 + 63ui32) & ~63ui32;

		struct data : public variable_layout_tagged_vector_impl<tag_type, size, REST ...>::data {
			bitfield_type padding[64];
			bitfield_type values[bytes_count];

			data()  {
				std::fill_n(values - 1, 1 + bytes_count, bitfield_type{ 0ui8 });
			}
		};

		static_assert(sizeof(data) % 64ui32 == 0);

		RELEASE_INLINE static void reset(data& d) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::reset(d);
			std::fill_n(d.values - 1 , 1 + bytes_count, bitfield_type{ 0ui8 });
		}

		template<typename T>
		RELEASE_INLINE static auto get(tag_type i, data const& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, bool, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return bit_vector_test(dat.values, to_index(i));
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		RELEASE_INLINE static auto get(tag_type i, data& dat) 
			-> std::conditional_t<std::is_same_v<T, index_type>, bool, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return bit_vector_test(dat.values, to_index(i));
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get<T>(i, dat);
		}
		template<typename T>
		RELEASE_INLINE static auto get_row(data& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, bitfield_type*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename T>
		RELEASE_INLINE static auto get_row(const data& dat)
			-> std::conditional_t<std::is_same_v<T, index_type>, bitfield_type const*, decltype(variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat))> {
			if constexpr(std::is_same_v<T, index_type>)
				return dat.values;
			else
				return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template get_row<T>(dat);
		}
		template<typename U, typename T>
		RELEASE_INLINE static std::enable_if_t<!std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T const& val) {
			if constexpr(std::is_same_v<U, index_type>)
				bit_vector_set(dat.values, to_index(i), val);
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		template<typename U, typename T>
		RELEASE_INLINE static std::enable_if_t<std::is_trivially_copyable_v<T>> set(tag_type i, data& dat, T val) {
			if constexpr(std::is_same_v<U, index_type>)
				bit_vector_set(dat.values, to_index(i), val);
			else
				variable_layout_tagged_vector_impl<tag_type, size, REST ...>::template set<U>(i, dat, val);
		}
		RELEASE_INLINE static void clear(tag_type i, data& dat) {
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::clear(i, dat);
			bit_vector_set(dat.values, to_index(i), false);
		}

		template<typename ... CONTEXT>
		static void serialize_object_impl(std::byte* &output, int32_t max, data const& obj, CONTEXT&& ... c) {
			auto const count = uint32_t(max + 7) / 8ui32;
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::serialize_object_impl(output, max, obj, std::forward<CONTEXT>(c)...);
			serialization::serialize_array(output, obj.values, count);
		}
		template<typename ... CONTEXT>
		static void deserialize_object_impl(std::byte const* &input, int32_t max, data& obj, CONTEXT&& ... c) {
			auto const count = uint32_t(max + 7) / 8ui32;
			variable_layout_tagged_vector_impl<tag_type, size, REST ...>::deserialize_object_impl(input, max, obj, std::forward<CONTEXT>(c)...);
			serialization::deserialize_array(input, obj.values, count);
		}
		template<typename ... CONTEXT>
		static size_t size_impl(int32_t max, data const& obj, CONTEXT&& ... c) {
			auto const count = uint32_t(max + 7) / 8ui32;
			return variable_layout_tagged_vector_impl<tag_type, size, REST ...>::size_impl(max, obj, std::forward<CONTEXT>(c)...) + sizeof(bitfield_type) * count;
		}
	};
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
	using container_type = variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, index_type_marker, tag_type, T ...>;
	ptr_type* const ptr;
	tag_type first_free;
	int32_t size_used = 0;
	std::mutex allocation_lock;
public:
	using index_type = tag_type;

	friend class serialization::serializer<variable_layout_tagged_vector<tag_type, container_size, T ...>>;

	variable_layout_tagged_vector() : ptr(static_cast<ptr_type*>(_aligned_malloc(sizeof(ptr_type), 64))) {
		new (ptr) ptr_type();

		for(int32_t i = container_size - 1; i >= 0; --i) {
			container_type::template set<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), *ptr, first_free);
			first_free = tag_type(typename tag_type::value_base_t(i));
		}
	}
	

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

		first_free = container_type::template get<index_type_marker>(first_free, *ptr);
		container_type::template set<index_type_marker>(allocated, *ptr, allocated);
		if(int32_t(to_index(allocated) + 1) > size_used)
			size_used = int32_t(to_index(allocated) + 1);

		return allocated;
	}

	tag_type thread_safe_get_new() {
		std::lock_guard lg(allocation_lock);
		return get_new();
	}

	void release(tag_type i) {
		container_type::clear(i, *ptr);
		container_type::template set<index_type_marker>(i, *ptr, first_free);

		first_free = i;

		if(size_used - 1 == to_index(i)) {
			for(int32_t j = size_used; j >= 0; --j) {
				if(container_type::template get<index_type_marker>(tag_type(typename tag_type::value_base_t(j)), *ptr) == tag_type(typename tag_type::value_base_t(j))) {
					size_used = j + 1;
					return;
				}
			}
			size_used = 0;
		}
	}

	void reset() {
		size_used = 0;
		first_free = tag_type();

		container_type::reset(*ptr);

		for(int32_t i = container_size - 1; i >= 0; --i) {
			container_type::template set<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), *ptr, first_free);
			first_free = tag_type(typename tag_type::value_base_t(i));
		}
	}

	int32_t size() const {
		return size_used;
	}
	template<typename UNUSED>
	int32_t size() const {
		return size_used;
	}
	template<typename UNUSED>
	void resize(int32_t size);

	uint32_t vector_size() const {
		return ve::to_vector_size(uint32_t(size_used));
	}
	bool is_valid_index(tag_type t) const noexcept {
		return ::is_valid_index(t) & (int32_t(to_index(t)) < size_used);
	}

	template<typename U>
	RELEASE_INLINE auto get(tag_type i) const -> decltype(container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr))) {
		return container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr));
	}
	template<typename U>
	RELEASE_INLINE auto get(tag_type i) -> decltype(container_type::template get<U>(i, *ptr)) {
		return container_type::template get<U>(i, *ptr);
	}

	template<typename U>
	RELEASE_INLINE auto get_row() const {
		if constexpr(std::is_same_v<typename container_type::template value_type<U>, void> || std::is_same_v<typename container_type::template value_type<U>, const void>)
			return;
		else if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U> const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), int32_t(uint32_t(size_used + 7) / 8ui32));
	}
	template<typename U>
	RELEASE_INLINE auto get_row() {
		if constexpr(std::is_same_v<typename container_type::template value_type<U>, void> || std::is_same_v<typename container_type::template value_type<U>, const void>)
			return;
		else if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U>, tag_type>(container_type::template get_row<U>(*ptr), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type*>(ptr)), int32_t(uint32_t(size_used + 7) / 8ui32));
	}

	template<typename U, typename V>
	RELEASE_INLINE std::enable_if_t<!std::is_trivially_copyable_v<V>> set(tag_type i, V const& val) {
		container_type::template set<U>(i, *ptr, val);
	}
	template<typename U, typename V>
	RELEASE_INLINE std::enable_if_t<std::is_trivially_copyable_v<V>> set(tag_type i, V val) {
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
};

template<typename tag_type, int32_t size, typename ... T>
class variable_layout_contiguous_tagged_vector;

template<typename tag_type, int32_t size, typename ... T>
class serialization::serializer<variable_layout_contiguous_tagged_vector<tag_type, size, T ...>>;

template<typename tag_type, int32_t container_size, typename ... T>
class variable_layout_contiguous_tagged_vector {
private:
	using ptr_type = typename variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::data;
	using container_type = variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>;
	ptr_type* const ptr;
	int32_t size_used = 0;

	friend class serialization::serializer<variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>>;
public:
	using index_type = tag_type;

	variable_layout_contiguous_tagged_vector() : ptr(static_cast<ptr_type*>(_aligned_malloc(sizeof(ptr_type), 64))) {
		new (ptr) ptr_type();
	}

	variable_layout_contiguous_tagged_vector(variable_layout_contiguous_tagged_vector const&) = delete;
	variable_layout_contiguous_tagged_vector(variable_layout_contiguous_tagged_vector &&) = delete;

	~variable_layout_contiguous_tagged_vector() {
		_aligned_free(ptr);
	}

	void resize(int32_t size) {
		size_used = size;
	}
	template<typename UNUSED>
	void resize(int32_t size) {
		size_used = size;
	}

	int32_t size() const {
		return size_used;
	}
	template<typename UNUSED>
	int32_t size() const {
		return size_used;
	}
	uint32_t vector_size() const {
		return ve::to_vector_size(uint32_t(size_used));
	}

	void reset() {
		size_used = 0;
		container_type::reset(*ptr);
	}

	void reset_nth(tag_type i) {
		container_type::clear(i, *ptr);
	}

	bool is_valid_index(tag_type t) const noexcept {
		return ::is_valid_index(t) & (int32_t(to_index(t)) < size_used - 1);
	}

	template<typename U>
	RELEASE_INLINE auto get(tag_type i) const -> decltype(container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr))) {
		return container_type::template get<U>(i, *static_cast<ptr_type const*>(ptr));
	}
	template<typename U>
	RELEASE_INLINE auto get(tag_type i) -> decltype(container_type::template get<U>(i, *ptr)) {
		return container_type::template get<U>(i, *ptr);
	}
	template<typename U>
	RELEASE_INLINE auto get_row() const {
		if constexpr(std::is_same_v<typename container_type::template value_type<U>, void> || std::is_same_v<typename container_type::template value_type<U>, const void>)
			return;
		else if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U> const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type const, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type const*>(ptr)), int32_t(uint32_t(size_used + 7) / 8ui32));
	}
	template<typename U>
	RELEASE_INLINE auto get_row() {
		if constexpr(std::is_same_v<typename container_type::template value_type<U>, void> || std::is_same_v<typename container_type::template value_type<U>, const void>)
			return;
		else if constexpr(!std::is_same_v<typename container_type::template value_type<U>, bitfield_type>)
			return tagged_array_view<typename container_type::template value_type<U>, tag_type>(container_type::template get_row<U>(*ptr), ve::to_vector_size(uint32_t(size_used)));
		else
			return tagged_array_view<bitfield_type, tag_type>(container_type::template get_row<U>(*static_cast<ptr_type*>(ptr)), int32_t(uint32_t(size_used + 7) / 8ui32));
	}
	template<typename U, typename V>
	RELEASE_INLINE std::enable_if_t<!std::is_trivially_copyable_v<V>> set(tag_type i, V const& val) {
		container_type::template set<U>(i, *ptr, val);
	}
	template<typename U, typename V>
	RELEASE_INLINE std::enable_if_t<std::is_trivially_copyable_v<V>> set(tag_type i, V val) {
		container_type::template set<U>(i, *ptr, val);
	}

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

		serialization::serialize(output, obj.size_used);

		for(int32_t i = 0; i < obj.size_used; ++i) {
			if(obj.template get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))) == tag_type(typename tag_type::value_base_t(i))) {
				auto tag = tag_type(typename tag_type::value_base_t(i));
				serialization::serialize(output, tag);
			} else {
				auto tag = tag_type();
				serialization::serialize(output, tag);
			}
		}

		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
			output, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, variable_layout_tagged_vector<tag_type, container_size, T ...>& obj, CONTEXT&& ... c) {
		obj.reset();
		serialization::deserialize(input, obj.size_used);

		serialization::deserialize_array(input, obj.ptr->values, obj.size_used);
		
		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
			input, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);

		obj.first_free = tag_type();
		for(int32_t i = container_size - 1; i >= obj.size_used; --i) {
			obj.template set<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), obj.first_free);
			obj.first_free = tag_type(typename tag_type::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			if(!::is_valid_index(obj.template get<index_type_marker>(tag_type(typename tag_type::value_base_t(i))))) {
				obj.template set<index_type_marker>(tag_type(typename tag_type::value_base_t(i)), obj.first_free);
				obj.first_free = tag_type(typename tag_type::value_base_t(i));
			}
		}
	}
	template<typename ... CONTEXT>
	static size_t size(variable_layout_tagged_vector<tag_type, container_size, T ...> const& obj, CONTEXT&& ... c) {
		return sizeof(obj.size_used) + serialization::serialize_size(tag_type()) * obj.size_used +
			variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(
				obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
	}
};

template<typename tag_type, int32_t container_size, typename ... T>
class serialization::serializer<variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static void serialize_object(std::byte* &output, variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...> const& obj, CONTEXT&& ... c) {
		serialization::serialize(output, obj.size_used);
		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::serialize_object_impl(
			output, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...>& obj, CONTEXT&& ... c) {
		obj.reset();
		serialization::deserialize(input, obj.size_used);

		variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::deserialize_object_impl(
			input, obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);

	}
	template<typename ... CONTEXT>
	static size_t size(variable_layout_contiguous_tagged_vector<tag_type, container_size, T ...> const& obj, CONTEXT&& ... c) {
		return sizeof(obj.size_used) +
			variable_layout_detail::variable_layout_tagged_vector_impl<tag_type, container_size, T ...>::size_impl(
				obj.size_used, *(obj.ptr), std::forward<CONTEXT>(c)...);
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

	fixed_vectorizable_2d_array() : ptr(static_cast<data_line*>(_aligned_malloc(sizeof(data_line) * inner_size, 64))) {
		std::uninitialized_value_construct_n(ptr, inner_size);
	}

	fixed_vectorizable_2d_array(fixed_vectorizable_2d_array const&) = delete;
	fixed_vectorizable_2d_array(fixed_vectorizable_2d_array &&) = delete;

	~fixed_vectorizable_2d_array() {
		_aligned_free(ptr);
	}

	RELEASE_INLINE value_type& get(tag_type t, int32_t index) {
		return ptr[index].values[to_index(t)];
	}
	RELEASE_INLINE value_type const& get(tag_type t, int32_t index) const {
		return ptr[index].values[to_index(t)];
	}
	template<int32_t index>
	RELEASE_INLINE value_type& get(tag_type t) {
		static_assert(index >= 0 && index < inner_size);
		return ptr[index].values[to_index(t)];
	}
	template<int32_t index>
	RELEASE_INLINE value_type const& get(tag_type t) const {
		static_assert(index >= 0 && index < inner_size);
		return ptr[index].values[to_index(t)];
	}
	template<int32_t index>
	RELEASE_INLINE void set(tag_type t, value_type v) {
		static_assert(index >= 0 && index < inner_size);
		return ptr[index].values[to_index(t)] = v;
	}
	RELEASE_INLINE tagged_array_view<value_type, tag_type> get_row(int32_t index, int32_t size) {
		return tagged_array_view<value_type, tag_type>(ptr[index].values, size);
	}
	RELEASE_INLINE tagged_array_view<const value_type, tag_type> get_row(int32_t index, int32_t size) const {
		return tagged_array_view<const value_type, tag_type>(ptr[index].values, size);
	}
	RELEASE_INLINE auto get_row(int32_t index) {
		return tagged_array_view<value_type, tag_type>(ptr[index].values, int32_t(extended_size));
	}
	RELEASE_INLINE auto get_row(int32_t index) const {
		return tagged_array_view<const value_type, tag_type>(ptr[index].values, int32_t(extended_size));
	}
	template<int32_t index>
	RELEASE_INLINE auto get_row(int32_t size) {
		if constexpr(index >= 0 && index < inner_size)
			return tagged_array_view<value_type, tag_type>(ptr[index].values, size);
		else
			return;
	}
	template<int32_t index>
	RELEASE_INLINE auto get_row(int32_t size) const {
		if constexpr(index >= 0 && index < inner_size)
			return tagged_array_view<const value_type, tag_type>(ptr[index].values, size);
		else
			return;
	}
	template<int32_t index>
	RELEASE_INLINE auto get_row() {
		if constexpr(index >= 0 && index < inner_size)
			return tagged_array_view<value_type, tag_type>(ptr[index].values, int32_t(extended_size));
		else
			return;
	}
	template<int32_t index>
	RELEASE_INLINE auto get_row() const {
		if constexpr(index >= 0 && index < inner_size)
			return tagged_array_view<const value_type, tag_type>(ptr[index].values, int32_t(extended_size));
		else
			return;
	}
	void reset() {
		std::uninitialized_value_construct_n(ptr, inner_size);
	}
	int32_t size() const {
		return int32_t(extended_size);
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

			ptr = static_cast<data_line*>(_aligned_malloc(sizeof(data_line) * new_size, 64));
			std::uninitialized_value_construct_n(ptr, new_size);

			current_size = new_size;
		}
	}

	RELEASE_INLINE ret_type get(tag_type t, inner_tag_type index) {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return ptr[to_index(index)].values[to_index(t)];
		else
			return bit_vector_test(ptr[to_index(index)].values, to_index(t));
	}
	RELEASE_INLINE const_ret_type get(tag_type t, inner_tag_type index) const {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return ptr[to_index(index)].values[to_index(t)];
		else
			return bit_vector_test(ptr[to_index(index)].values, to_index(t));
	}
	RELEASE_INLINE void set(tag_type t, inner_tag_type index, param_type p) {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			ptr[to_index(index)].values[to_index(t)] = p;
		else
			bit_vector_set(ptr[to_index(index)].values, to_index(t), p);
	}
	
	RELEASE_INLINE auto get_row(inner_tag_type index, int32_t size) {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return tagged_array_view<vector_type, tag_type>(ptr[to_index(index)].values, size);
		else
			return tagged_array_view<bitfield_type, tag_type>(
				ptr[to_index(index)].values,
				int32_t(uint32_t(size + 7) / 8ui32));
	}
	RELEASE_INLINE auto get_row(inner_tag_type index, int32_t size) const {
		if constexpr(!std::is_same_v<value_type, bitfield_type>)
			return tagged_array_view<const vector_type, tag_type>(ptr[to_index(index)].values, size);
		else
			return tagged_array_view<const bitfield_type, tag_type>(
				ptr[to_index(index)].values,
				int32_t(uint32_t(size + 7) / 8ui32));
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
