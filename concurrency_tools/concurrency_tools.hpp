#pragma once
#include "concurrency_tools.h"
#include "common\\common.h"
#include <cstdlib>
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
class serialization::serializer<stable_vector<object_type, index_type, block_size, index_size>> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static void serialize_object(std::byte* &output, stable_vector<object_type, index_type, block_size, index_size> const& obj, CONTEXT&& ... c) {
		serialize(output, obj.indices_in_use);

		for(uint32_t i = 0ui32; i < obj.indices_in_use; ++i) {
			object_type* block = obj.index_array[i];
			for(uint32_t j = 0; j < static_cast<int32_t>(block_size); ++j) {
				if(is_valid_index(obj.get_id(block[j].id))) {
					bool b = true;
					serialize(output, b);
					serialize(output, block[j], std::forward<CONTEXT>(c) ...);
				} else {
					bool b = false;
					serialize(output, b);
				}
			}
		}
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, stable_vector<object_type, index_type, block_size, index_size>& obj, CONTEXT&& ... c) {
		// clear existing blocks
		for(uint32_t i = 0ui32; i < obj.indices_in_use; ++i) {
			object_type* block = obj.index_array[i];
			for(int32_t j = 0; j < static_cast<int32_t>(block_size); ++j) {
				block[j].~object_type();
				new (block + j) object_type();
			}
			
		}
		
		const auto old_in_use = obj.indices_in_use;
		uint32_t new_in_use = 0;
		deserialize(input, new_in_use);

		// ensure indices in use has correct value
		obj.indices_in_use = std::max(new_in_use, old_in_use);

		// prepare memory of new blocks
		for(uint32_t i = old_in_use; i < new_in_use; ++i) {
			object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size, 64);
			obj.index_array[i] = new_block;

			for(uint32_t j = 0; j < static_cast<int32_t>(block_size); ++j)
				new (new_block + j) object_type();
		}

		obj.first_free = index_type(static_cast<value_base_of<index_type>>(to_index(index_type()) | high_bit_mask<index_type>));

		// add unused old blocks to free list
		for(uint32_t i = new_in_use; i < old_in_use; ++i) {
			object_type* block = obj.index_array[i];
			for(uint32_t j = 0; j < static_cast<int32_t>(block_size); ++j) {
				(block + j)->id = obj.first_free;
				obj.first_free = index_type(static_cast<value_base_of<index_type>>(((i << ct_log2(block_size)) + j) | high_bit_mask<index_type>));
			}
		}

		// deserialize routine

		for(uint32_t i = 0ui32; i < new_in_use; ++i) {
			object_type* block = obj.index_array[i];
			for(uint32_t j = 0; j < static_cast<int32_t>(block_size); ++j) {
				bool item_exists = false;
				deserialize(input, item_exists);
				if(item_exists) {
					(block + j)->id = index_type(static_cast<value_base_of<index_type>>(((i << ct_log2(block_size)) + j)));
					deserialize(input, block[j], std::forward<CONTEXT>(c) ...);
				} else {
					(block + j)->id = obj.first_free;
					obj.first_free = index_type(static_cast<value_base_of<index_type>>(((i << ct_log2(block_size)) + j) | high_bit_mask<index_type>));
				}
			}
		}
	}
	template<typename ... CONTEXT>
	static size_t size(stable_vector<object_type, index_type, block_size, index_size> const& obj, CONTEXT&& ... c) {
		size_t size = sizeof(uint32_t) + sizeof(bool) * obj.indices_in_use * block_size;
		for(uint32_t i = 0ui32; i < obj.indices_in_use; ++i) {
			object_type* block = obj.index_array[i];
			for(uint32_t j = 0; j < static_cast<int32_t>(block_size); ++j) {
				if(is_valid_index(block[j].id))
					size += serialize_size(block[j], std::forward<CONTEXT>(c)...);
			}
		}
		return size;
	}
};

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
class serialization::serializer<stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static void serialize_object(std::byte* &output, stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size> const& obj, CONTEXT&& ... c) {
		serialize(output, obj.indices_in_use);
		serialize(output, obj.inner_size);

		for(uint32_t i = 0ui32; i < obj.indices_in_use; ++i) {
			if constexpr(serializer<object_type>::has_simple_serialize)
				serialize_array(output, obj.index_array[i], block_size * obj.inner_size);
			else
				serialize_range(output, obj.index_array[i], obj.index_array[i] + block_size * obj.inner_size, std::forward<CONTEXT>(c)...);
		}
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>& obj, CONTEXT&& ... c) {
		obj.~stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>();

		deserialize(input, obj.indices_in_use);
		deserialize(input, obj.inner_size);

		for(uint32_t i = 0ui32; i < obj.indices_in_use; ++i) {
			object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size * obj.inner_size, 64);
			obj.index_array[i] = new_block;
			if constexpr(serializer<object_type>::has_simple_serialize)
				deserialize_array(input, new_block, block_size * obj.inner_size);
			else
				deserialize_into_range(input, new_block, new_block + block_size * obj.inner_size, std::forward<CONTEXT>(c)...);
		}
	}
	static size_t size(stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size> const& obj) {
		return sizeof(obj.indices_in_use) + sizeof(obj.inner_size) + obj.indices_in_use * sizeof(object_type) * block_size * obj.inner_size;
	}
};

__declspec(restrict) void* concurrent_alloc_wrapper(size_t sz);
void concurrent_free_wrapper(void* p);

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
stable_vector<object_type, index_type, block_size, index_size>::~stable_vector() {
	for(uint32_t i = 0; i < indices_in_use; ++i) {
		for(int32_t j = static_cast<int32_t>(block_size) - 1; j >= 0; --j)
			(index_array[i])[j].~object_type();
		_aligned_free(index_array[i]);
		index_array[i] = nullptr;
	}
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
stable_vector<object_type, index_type, block_size, index_size>::stable_vector() { }

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_vector<object_type, index_type, block_size, index_size>::get(index_type i) const noexcept {
	const auto block_num = to_index(i) >> ct_log2(block_size);
	const auto block_index = to_index(i) & (block_size - 1);
	return (index_array[block_num])[block_index];
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_vector<object_type, index_type, block_size, index_size>::untyped_get(uint32_t i) const noexcept {
	const auto block_num = i >> ct_log2(block_size);
	const auto block_index = i & (block_size - 1);
	return (index_array[block_num])[block_index];
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_vector<object_type, index_type, block_size, index_size>::operator[](index_type i) const noexcept {
	return get(i);
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
template<typename T>
void stable_vector<object_type, index_type, block_size, index_size>::for_each(T&& f) const {
	for(uint32_t i = 0; i < indices_in_use; ++i) {
		for(uint32_t j = 0; j < block_size; ++j) {
			if(((to_index((index_array[i])[j].id) & high_bit_mask<index_type>) == 0) & ::is_valid_index((index_array[i])[j].id))
				f((index_array[i])[j]);
		}
	}
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
template<typename T>
void stable_vector<object_type, index_type, block_size, index_size>::parallel_for_each(T const& f) {
	concurrency::parallel_for(0ui32, indices_in_use * block_size, [&f, _this = this](uint32_t i) {
		auto& th = _this->untyped_get(i);
		if(((to_index(th.id) & high_bit_mask<index_type>) == 0) & ::is_valid_index(th.id))
			f(th);
	});
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
bool stable_vector<object_type, index_type, block_size, index_size>::is_valid_index(index_type i) const {
	const auto block_num = uint32_t(to_index(i)) >> ct_log2(block_size);
	return ::is_valid_index(i) & (block_num < indices_in_use);
}

#ifdef _DEBUG
struct stable_vector_full {};
#endif

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type* stable_vector<object_type, index_type, block_size, index_size>::get_location(index_type i) {
	if(!::is_valid_index(i))
		return nullptr;

	const auto block_num = to_index(i) >> ct_log2(block_size);
	const auto block_index = to_index(i) & (block_size - 1);

	while(indices_in_use <= uint32_t(block_num)) {
		object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size, 64);
		for(int32_t j = static_cast<int32_t>(block_size); j--; ) {
			new (new_block + j) object_type();
			(new_block + j)->id = first_free;
			first_free = index_type(static_cast<value_base_of<index_type>>(((indices_in_use << ct_log2(block_size)) + uint32_t(j)) | high_bit_mask<index_type>));
		}

		index_array[indices_in_use] = new_block;
		++indices_in_use;
	}
	return (index_array[block_num]) + block_index;
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_vector<object_type, index_type, block_size, index_size>::get_new() {
	if(first_free != index_type(static_cast<value_base_of<index_type>>(to_index(index_type()) | high_bit_mask<index_type>))) {
		const auto real_ff = index_type(static_cast<value_base_of<index_type>>(to_index(first_free) & ~high_bit_mask<index_type>));
		const auto block_num = to_index(real_ff) >> ct_log2(block_size);
		const auto block_index = to_index(real_ff) & (block_size - 1);

		first_free = (index_array[block_num])[block_index].id;
		(index_array[block_num])[block_index].id = real_ff;

		return (index_array[block_num])[block_index];
	}
	
	if(indices_in_use < index_size) {
		object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size, 64);
		for(int32_t j = static_cast<int32_t>(block_size) - 1; j > 0; --j) {
			new (new_block + j) object_type();
			(new_block + j)->id = first_free;
			first_free = index_type(static_cast<value_base_of<index_type>>(((indices_in_use << ct_log2(block_size)) + uint32_t(j)) | high_bit_mask<index_type>));
		}

		index_array[indices_in_use] = new_block;
		
		new (new_block + 0) object_type();
		new_block[0].id = index_type(static_cast<value_base_of<index_type>>(indices_in_use << ct_log2(block_size)));

		++indices_in_use;

		return new_block[0];
	}

#ifdef _DEBUG
	throw stable_vector_full();
#else
	std::abort();
#endif
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
void stable_vector<object_type, index_type, block_size, index_size>::remove(index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block_size);
	const auto block_index = to_index(i) & (block_size - 1);

	if((to_index((index_array[block_num])[block_index].id) & high_bit_mask<index_type>) == 0) {
		const auto old_index = (index_array[block_num])[block_index].id;

		(index_array[block_num])[block_index].~object_type();
		new ((index_array[block_num]) + block_index) object_type();

		(index_array[block_num])[block_index].id = first_free;
		first_free = index_type(static_cast<value_base_of<index_type>>(to_index(old_index) | high_bit_mask<index_type>));
	}
}

#ifdef _DEBUG
struct stable_2d_vector_full {};
#endif

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::stable_2d_vector() {}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::~stable_2d_vector() {
	for(uint32_t i = 0; i < indices_in_use; ++i) {
		for(int32_t j = static_cast<int32_t>(block_size * inner_size) - 1; j >= 0; --j)
			(index_array[i])[j].~object_type();
		_aligned_free(index_array[i]);
		index_array[i] = nullptr;
	}
	indices_in_use = 0ui32;
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
void stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::reset(uint32_t new_inner_size) {
	for(uint32_t i = 0; i < indices_in_use; ++i) {
		for(int32_t j = static_cast<int32_t>(block_size * inner_size) - 1; j >= 0; --j)
			(index_array[i])[j].~object_type();
		_aligned_free(index_array[i]);
		index_array[i] = nullptr;
	}
	indices_in_use = 0ui32;
	inner_size = new_inner_size;
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
void stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::ensure_capacity(uint32_t new_outer_size) {
	const auto block_num = new_outer_size != 0 ? (new_outer_size - 1) >> ct_log2(block_size) : 0;

	if(block_num >= index_size) {
#ifdef _DEBUG
		throw stable_2d_vector_full();
#else
		std::abort();
#endif
	}

	for(uint32_t i = indices_in_use; i <= block_num; ++i) {
		object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size * inner_size, 64);
		for(int32_t j = static_cast<int32_t>(block_size * inner_size) - 1; j >= 0; --j)
			new (new_block + j) object_type();
		index_array[i] = new_block;
	}
	indices_in_use = block_num + 1;
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
void stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::clear_row(outer_index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block_size);
	const auto block_index = to_index(i) & (block_size - 1);

	object_type* const block = index_array[block_num];
	for(int32_t i = int32_t(inner_size) - 1; i >= 0; --i) {
		block[block_index * inner_size + i].~object_type();
		new (block + block_index * inner_size + i) object_type();
	}
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type* stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::get_row(outer_index_type i) const {
	const auto block_num = to_index(i) >> ct_log2(block_size);
	const auto block_index = to_index(i) & (block_size - 1);

	return index_array[block_num] + block_index * inner_size;
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type* stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::safe_get_row(outer_index_type i) {
	ensure_capacity(i);
	return get_row(i);
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::get(outer_index_type i, inner_index_type j) const {
	const auto block_num = to_index(i) >> ct_log2(block_size);
	const auto block_index = to_index(i) & (block_size - 1);

	return *(index_array[block_num] + block_index * inner_size + to_index(j));
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::safe_get(outer_index_type i, inner_index_type j) {
	ensure_capacity(i);
	return get(i, j);
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
bool stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::is_valid_index(outer_index_type i) const {
	const auto block_num = to_index(i) >> ct_log2(block_size);
	return block_num < indices_in_use;
}


template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i) {
	return get_range(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i) {
	return get_range(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type> i) {
	return get_range(storage, i.value);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_subrange(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj) {
	auto range = get_range(storage, i.value);
	return std::pair<object_type*, object_type*>(std::lower_bound(range.first, range.second, obj), std::upper_bound(range.first, range.second, obj));
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i, uint32_t inner_index) {
	return get(storage, i.value, inner_index);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i) {
	return get_capacity(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i) {
	return get_size(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, uint32_t inner_index) {
	return get(storage, i.value, inner_index);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i) {
	return get_capacity(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i) {
	return get_size(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type> i, uint32_t inner_index) {
	return get(storage, i.value, inner_index);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type> i) {
	return get_capacity(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type> i) {
	return get_size(storage, i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type>& i, object_type obj) {
	push_back(storage, i.value, obj);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, object_type obj) {
	add_unique_item(storage, i.value, obj);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj) {
	add_item(storage, i.value, obj);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type>& i, object_type obj) {
	remove_unsorted_item(storage, i.value, obj);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, object_type obj) {
	remove_sorted_item(storage, i.value, obj);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj) {
	remove_multisorted_item(storage, i.value, obj);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_single_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj) {
	const auto range = get_range(storage, i);
	const auto lb = std::lower_bound(range.first, range.second, obj);
	const auto ub = std::upper_bound(range.first, range.second, obj);
	
	auto fr = std::find(lb, ub, obj);
	if(fr == ub)
		return;

	std::copy(fr + 1, range.second, fr);
	pop_back(storage, i.value);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_subrange(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj) {
	remove_subitem_range(storage, i.value, obj);
}

namespace detail {
	struct alignas(8) mk_2_header {
		stable_mk_2_tag next_free;
		uint16_t size;
		uint16_t capacity;
	};
	static_assert(sizeof(mk_2_header) == 8);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename FUNC>
void remove_item_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type>& i, FUNC const& f) {
	const auto range = get_range(storage, i);
	auto size = range.second - range.first;

	for(int32_t j = int32_t(size); j--; ) {
		if(f(range.first[j])) {
			range.first[j] = std::move(range.first[size - 1]);
			--size;
		}
	}

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
	header->size = uint16_t(size);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename FUNC>
void remove_item_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, FUNC const& f) {
	const auto range = get_range(storage, i);
	const auto new_end = std::remove_if(range.first, range.second, f);
	
	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
	header->size = uint16_t(new_end - range.first);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename FUNC>
void remove_item_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, FUNC const& f) {
	const auto range = get_range(storage, i);
	const auto new_end = std::remove_if(range.first, range.second, f);

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
	header->size = uint16_t(new_end - range.first);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename FUNC>
void remove_subitem_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type key, FUNC const& f) {
	const auto range = get_range(storage, i);
	auto lb = std::lower_bound(range.first, range.second, key);
	auto ub = std::upper_bound(range.first, range.second, key);

	const auto new_end = std::remove_if(lb, ub, f);
	std::copy(ub, range.second, new_end);

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
	header->size = uint16_t(header->size - (ub - new_end));
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type> i, object_type obj) {
	const auto rng = get_range(storage, i.value);
	return std::find(rng.first, rng.second, obj) != rng.second;
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i, object_type obj) {
	return contains_item(storage, i.value, obj);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj) {
	return contains_item(storage, i.value, obj);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_subitem(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj) {
	const auto range = get_subrange(storage, i, obj);
	return std::find(range.first, range.second, obj) != range.second;
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void resize(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type>& i, uint32_t new_size) {
	auto old_size = get_size(storage, i);
	if(new_size < old_size) {
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
		header->size = uint16_t(new_size);
	} else if(new_size > old_size) {
		storage.increase_capacity(i.value, new_size);
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
		std::fill((object_type*)(header + 1) + header->size, (object_type*)(header + 1) + new_size, object_type());
		header->size = uint16_t(new_size);
	}

}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void resize(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, uint32_t new_size) {
	auto old_size = get_size(storage, i);
	if(new_size < old_size) {
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
		header->size = uint16_t(new_size);
	} else if(new_size > old_size) {
		storage.increase_capacity(i.value, new_size);
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
		object_type* start = (object_type*)(header + 1);
		if(old_size != 0) {
			const auto insertion_pos = std::lower_bound(start, start + old_size, object_type());
			std::copy_backward(insertion_pos, start + old_size, start + new_size);
			std::fill(insertion_pos, insertion_pos + (new_size - old_size), object_type());
		} else {
			std::fill(start, start + new_size, object_type());
		}
		header->size = uint16_t(new_size);
	}
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void resize(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, uint32_t new_size) {
	auto old_size = get_size(storage, i);
	if(new_size < old_size) {
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
		header->size = uint16_t(new_size);
	} else if(new_size > old_size) {
		storage.increase_capacity(i.value, new_size);
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i.value);
		object_type* start = (object_type*)(header + 1);
		if(old_size != 0) {
			const auto insertion_pos = std::lower_bound(start, start + old_size, object_type());
			std::copy_backward(insertion_pos, start + old_size, start + new_size);
			std::fill(insertion_pos, insertion_pos + (new_size - old_size), object_type());
		} else {
			std::fill(start, start + new_size, object_type());
		}
		header->size = uint16_t(new_size);
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type>& i, object_type const* first, object_type const* last) {
	add_unordered_range(storage, i.value, first, last);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, object_type const* first, object_type const* last) {
	add_unique_ordered_range(storage, i.value, first, last);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type const* first, object_type const* last) {
	add_ordered_range(storage, i.value, first, last);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void clear(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type>& i) {
	storage.release(i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void clear(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i) {
	storage.release(i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void clear(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i) {
	storage.release(i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void shrink(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type>& i) {
	storage.shrink_capacity(i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void shrink(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i) {
	storage.shrink_capacity(i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void shrink(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i) {
	storage.shrink_capacity(i.value);
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type* find(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type> i, object_type obj) {
	const auto rng = get_range(storage, i.value);
	const auto f = std::find(rng.first, rng.second, obj);
	if(f == rng.second)
		return nullptr;
	return f;
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type* find(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i, object_type obj) {
	const auto rng = get_range(storage, i.value);
	const auto lb = std::lower_bound(rng.first, rng.second, obj);
	if(lb != rng.second && *lb == obj)
		return lb;
	return nullptr;
}
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type* find(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj) {
	const auto rng = get_range(storage, i.value);
	const auto lb = std::lower_bound(rng.first, rng.second, obj);
	const auto ub = std::upper_bound(rng.first, rng.second, obj);
	if(lb != ub)
		return lb;
	return nullptr;
}

namespace serialization {
	template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename type_tag>
	void serialize_stable_array(std::byte* &output, stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, type_tag i) {
		uint16_t element_count = uint16_t(get_size(storage, i));
		serialize(output, element_count);
		serialize_array(output, get_range(storage, i).first, element_count);
	}
	template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename type_tag>
	void deserialize_stable_array(std::byte const* &input, stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, type_tag& i) {
		uint16_t element_count = 0;
		deserialize(input, element_count);
		resize(storage, i, element_count);
		deserialize_array(input, get_range(storage, i).first, element_count);
	}

	template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename type_tag>
	size_t serialize_stable_array_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, type_tag i) {
		return sizeof(uint16_t) + sizeof(object_type) * get_size(storage, i);
	}
}
	
namespace detail {
	template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
	stable_mk_2_tag return_new_memory(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, uint32_t requested_capacity) {
		const uint32_t real_capacity = 1ui32 << rt_log2_round_up(requested_capacity > minimum_size ? requested_capacity : minimum_size);

		mk_2_header* new_header;
		stable_mk_2_tag new_mem;

		if constexpr(!is_aligned) {
			const uint32_t qword_size = 1ui32 + (real_capacity * sizeof(object_type) + 7ui32) / 8ui32;
			new_header = (mk_2_header*)(storage.backing_storage + storage.first_free);
			new_mem = storage.first_free;
			storage.first_free += qword_size;
		} else {
			auto next_aligned_free = (storage.first_free + 4ui32) & ~3ui32;
			const uint32_t qword_size = (real_capacity * sizeof(object_type) + 7ui32) / 8ui32;
			new_header = (mk_2_header*)(storage.backing_storage + (next_aligned_free - 1ui32));
			new_mem = next_aligned_free - 1ui32;
			storage.first_free = next_aligned_free + qword_size;
		}

		new_header->capacity = uint16_t(real_capacity);
		new_header->size = 0ui16;
		new_header->next_free = null_value_of<stable_mk_2_tag>;

#ifdef _DEBUG
		if(storage.first_free >= memory_size) {
			std::abort();
		}
#endif
		
		object_type* objects = (object_type*)(new_header + 1);
		for(int32_t i = int32_t(real_capacity) - 1; i >= 0; --i)
			new (objects + i) object_type();

		return new_mem;
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>::stable_variable_vector_storage_mk_2() {
	if constexpr(!is_aligned)
		backing_storage = new uint64_t[memory_size];
	else
		backing_storage = (uint64_t*)_aligned_malloc(sizeof(uint64_t) * memory_size, 32);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>::~stable_variable_vector_storage_mk_2() {
	if constexpr(!is_aligned)
		delete[] backing_storage;
	else
		_aligned_free(backing_storage);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>::reset() {
	first_free = 0ui32;
	for(uint32_t i = 0; i < std::extent_v<decltype(free_lists)>; ++i)
		free_lists[i] = null_value_of<stable_mk_2_tag>;
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
stable_mk_2_tag stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>::make_new(uint32_t capacity) {
	const uint32_t free_list_pos = rt_log2_round_up(capacity > minimum_size ? capacity : minimum_size);

	if(free_lists[free_list_pos] != null_value_of<stable_mk_2_tag>) {
		stable_mk_2_tag item = free_lists[free_list_pos];
		detail::mk_2_header* new_header = (detail::mk_2_header*)(backing_storage + item);
		new_header->size = 0ui16;
		free_lists[free_list_pos] = new_header->next_free;
		return item;
	} else if(free_lists[free_list_pos + 1] != null_value_of<stable_mk_2_tag>) {
		stable_mk_2_tag item = free_lists[free_list_pos + 1];
		detail::mk_2_header* new_header = (detail::mk_2_header*)(backing_storage + item);
		new_header->size = 0ui16;
		free_lists[free_list_pos + 1] = new_header->next_free;
		return item;
	} else {
		return detail::return_new_memory(*this, capacity);
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>::release(stable_mk_2_tag& i) {
	if(!is_valid_index(i))
		return;

	detail::mk_2_header* header = (detail::mk_2_header*)(backing_storage + i);

	const uint32_t free_list_pos = rt_log2(header->capacity);

	header->next_free = free_lists[free_list_pos];
	header->size = 0ui16;
	free_lists[free_list_pos] = i;

	i = null_value_of<stable_mk_2_tag>;
}


template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>::shrink_capacity(stable_mk_2_tag& i) {
	detail::mk_2_header* header = (detail::mk_2_header*)(backing_storage + i);

	if(header->size == 0ui16) {
		release(i);
		return;
	}

	const uint32_t target_free_list_pos = rt_log2_round_up(header->size > minimum_size ? header->size : minimum_size);
	const uint32_t free_list_pos = rt_log2(header->capacity);

	if(target_free_list_pos + 1 < free_list_pos) {
		const auto new_item = make_new(header->size);

		detail::mk_2_header* new_header = (detail::mk_2_header*)(backing_storage + new_item);
		new_header->size = header->size;
		std::copy((object_type*)(header + 1), ((object_type*)(header + 1)) + header->size, (object_type*)(new_header + 1));

		release(i);
		i = new_item;
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>::increase_capacity(stable_mk_2_tag& i, uint32_t new_capacity) {
	if(!is_valid_index(i)) {
		i = make_new(new_capacity);
	} else {
		detail::mk_2_header* header = (detail::mk_2_header*)(backing_storage + i);
		if(new_capacity > header->capacity) {
			const auto new_item = make_new(new_capacity);

			detail::mk_2_header* new_header = (detail::mk_2_header*)(backing_storage + new_item);
			new_header->size = header->size;
			std::copy((object_type*)(header + 1), ((object_type*)(header + 1)) + header->size, (object_type*)(new_header + 1));

			release(i);
			i = new_item;
		}
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i) {
	if(i != null_value_of<stable_mk_2_tag>) {
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
		return std::pair<object_type*, object_type*>((object_type*)(header + 1), (object_type*)(header + 1) + header->size);
	} else {
		return std::pair<object_type*, object_type*>(nullptr, nullptr);
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i, uint32_t inner_index) {
	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	return *((object_type*)(header + 1) + inner_index);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i) {
	if(i != null_value_of<stable_mk_2_tag>) {
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
		return header->capacity;
	} else {
		return 0ui32;
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i) {
	if(i != null_value_of<stable_mk_2_tag>) {
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
		return header->size;
	} else {
		return 0ui32;
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void push_back(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type obj) {
	if(!is_valid_index(i)) {
		storage.increase_capacity(i, 1);
		detail::mk_2_header*header = (detail::mk_2_header*)(storage.backing_storage + i);

		*((object_type*)(header + 1)) = obj;
		header->size = 1ui16;
	} else {
		detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
		if(header->size >= header->capacity) {
			storage.increase_capacity(i, header->size + 1);
			header = (detail::mk_2_header*)(storage.backing_storage + i);
		}
		*((object_type*)(header + 1) + header->size) = obj;
		++header->size;
	}
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void pop_back(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i) {
	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	if(header->size != 0)
		--header->size;
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_unordered_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type const* first, object_type const* last) {
	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	const uint32_t distance = uint32_t(last - first);

	if(header->size + distance >= header->capacity) {
		storage.increase_capacity(i, header->size + uint32_t(last - first));
		header = (detail::mk_2_header*)(storage.backing_storage + i);
	}

	std::copy(first, last, (object_type*)(header + 1) + header->size);
	header->size += uint16_t(distance);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_unsorted_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj) {
	const auto range = get_range(storage, i);
	const auto f = std::find(range.first, range.second, obj);

	if(f == range.second)
		return;

	*f = *(range.second - 1);
	pop_back(storage, i);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type obj) {
	const auto size = get_size(storage, i);
	const auto capacity = get_capacity(storage, i);

	if(size >= capacity)
		storage.increase_capacity(i, size + 1);

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	object_type* start = (object_type*)(header + 1);

	if(size != 0) {
		const auto insertion_pos = std::lower_bound(start, start + size, obj);
		std::copy_backward(insertion_pos, start + size, start + size + 1);
		*insertion_pos = obj;
	} else {
		*start = obj;
	}

	++header->size;
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_unique_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type obj) {
	if(!contains_item(storage, i, obj))
		add_item(storage, i, obj);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_ordered_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type const* first, object_type const* last) {
	const uint32_t count = static_cast<uint32_t>(last - first);

	const auto size = get_size(storage, i);
	const auto capacity = get_capacity(storage, i);

	if(size + count > capacity)
		storage.increase_capacity(i, size + count);

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	object_type* start = (object_type*)(header + 1);

	object_type* const temp_buffer = (object_type*)_alloca(sizeof(object_type) * size);

	std::copy(start, start + size, temp_buffer);
	std::merge(temp_buffer, temp_buffer + size, first, last, start);

	header->size = uint16_t(size + count);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_unique_ordered_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type const* first, object_type const* last) {
	const uint32_t count = static_cast<uint32_t>(last - first);

	const auto size = get_size(storage, i);
	const auto capacity = get_capacity(storage, i);

	if(size + count > capacity) 
		storage.increase_capacity(i, size + count);

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	object_type* first_item = (object_type*)(header + 1);

	object_type* const temp_buffer = (object_type*)_alloca(sizeof(object_type) * size);

	std::copy(first_item, first_item + size, temp_buffer);
	const auto new_last = std::set_union(temp_buffer, temp_buffer + size, first, last, first_item);

	header->size = uint16_t(new_last - first_item);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_sorted_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj) {
	const auto range = get_range(storage, i);
	const auto lb = std::lower_bound(range.first, range.second, obj);
	if(lb == range.second || !(*lb == obj))
		return;

	std::copy(lb + 1, range.second, lb);
	pop_back(storage, i);
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_multisorted_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj) {
	const auto range = get_range(storage, i);
	const auto lb = std::lower_bound(range.first, range.second, obj);
	const auto ub = std::upper_bound(range.first, range.second, obj);

	const auto new_end = std::remove(lb, ub, obj);
	std::copy(ub, range.second, new_end);

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	header->size = uint16_t(header->size - (ub - new_end));
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_subitem_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj) {
	const auto range = get_range(storage, i);
	const auto lb = std::lower_bound(range.first, range.second, obj);
	const auto ub = std::upper_bound(range.first, range.second, obj);

	if(lb == ub)
		return;

	std::copy(ub, range.second, lb);

	detail::mk_2_header* header = (detail::mk_2_header*)(storage.backing_storage + i);
	header->size = uint16_t(std::max(0, int32_t(header->size) - int32_t(ub - lb)));
}

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i, object_type obj) {
	const auto range = get_range(storage, i);
	return std::binary_search(range.first, range.second, obj);
}

template<typename T>
concurrent_string::concurrent_string(const string_expression<T>& t) {
	const auto tsz = t.length();
	if (tsz <= (internal_concurrent_string_size - 1)) {
		for (int32_t i = tsz-1; i >= 0; --i)
			_data.local_data[i] = t[i];
		_data.local_data[tsz] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = (internal_concurrent_string_size - 1) - tsz;
	} else {
		_data.local_data[internal_concurrent_string_size - 1] = 127;
		_data.remote_data.data = (char*)concurrent_alloc_wrapper(tsz + 1);
		for (int32_t i = tsz - 1; i >= 0; --i)
			_data.local_data[i] = t[i];
		_data.remote_data.data[tsz] = 0;
		_data.remote_data.length = tsz;
	}
}

template<typename T>
bool concurrent_string::operator==(const string_expression<T>& o) const {
	const auto this_length = length();
	if (this_length != o.length())
		return false;
	else {
		const auto this_data = c_str();
		for (int32_t i = (int32_t)this_length - 1; i >= 0; --i) {
			if (this_data[i] != o[i])
				return false;
		}
		return true;
	}
}

template<typename T>
concurrent_string& concurrent_string::operator=(const string_expression<T>& o) {
	this->~concurrent_string();
	new (this) concurrent_string(o);
	return *this;
}

template<typename T>
concurrent_string& concurrent_string::operator+=(const string_expression<T>& o) {
	*this = *this + o;
	return *this;
}

template <typename T>
string_expression_support<T>::operator std::string() const {
	std::string l(static_cast<const T*>(this)->length(), 0);
	for (int32_t i = (int32_t)static_cast<const T*>(this)->length() - 1; i >= 0; --i)
		l[(size_t)i] = static_cast<const T*>(this)->operator[]((uint32_t)i);
	return l;
}

template<typename T>
T* concurrent_allocator<T>::allocate(size_t n) {
	return (T*)concurrent_alloc_wrapper(n * sizeof(T));
}

template<typename T>
void concurrent_allocator<T>::deallocate(T* p, size_t) {
	concurrent_free_wrapper(p);
}

template<typename T>
T* aligned_allocator_32<T>::allocate(size_t n) {
	return (T*)_aligned_malloc(n * sizeof(T), 32);
}

template<typename T>
void aligned_allocator_32<T>::deallocate(T* p, size_t) {
	_aligned_free(p);
}

union concurrent_key_pair_helper {
	struct {
		uint32_t index;
		uint32_t counter;
	} parts;
	uint64_t value;

	constexpr concurrent_key_pair_helper(uint32_t i, uint32_t c) : parts{ i, c } {}
	constexpr concurrent_key_pair_helper(uint64_t v) : value{ v } {}
};

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_list<T, block, index_sz>::fixed_sz_list() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(created + block);

	keys[block - 1].store((uint32_t)-1, std::memory_order_release);
	for (int32_t i = static_cast<int32_t>(block) - 2; i >= 0; --i) {
		keys[i].store(static_cast<uint32_t>(i + 1), std::memory_order_release);
	}
	index_array[0].store(created, std::memory_order_release);

	first_free.store(concurrent_key_pair_helper(0,0).value, std::memory_order_release);
	first_in_list.store(concurrent_key_pair_helper((uint32_t)-1, 0).value, std::memory_order_release);
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_list<T, block, index_sz>::~fixed_sz_list() {
	auto p = concurrent_key_pair_helper(first_in_list.load(std::memory_order_relaxed)).parts.index;
	while (p != (uint32_t)-1) {
		const auto block_num = p >> ct_log2(block);
		const auto block_index = p & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_relaxed);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		local_index[block_index].~T();

		p = keys[block_index].load(std::memory_order_relaxed);
	}
	for (int32_t i = static_cast<int32_t>(first_free_index.load(std::memory_order_relaxed)) - 1; i >= 0; --i) {
		_aligned_free(index_array[i].load(std::memory_order_relaxed));
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
void fixed_sz_list<T, block, index_sz>::create_new_block() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const ckeys = (std::atomic<uint32_t>*)(created + block);

	const auto new_index = first_free_index.fetch_add(1, std::memory_order_acq_rel);

	if (new_index >= index_sz) {
		_aligned_free(created);
		std::abort();
	}

	const auto block_num = new_index << ct_log2(block);

	for (int32_t i = block - 2; i >= 0; --i) {
		ckeys[i].store(block_num + static_cast<uint32_t>(i) + 1, std::memory_order_release);
	}

	concurrent_key_pair_helper expected_first(first_free.load(std::memory_order_acquire));
	ckeys[block - 1].store(expected_first.parts.index, std::memory_order_release);

	index_array[new_index].store(created, std::memory_order_release);

	while (!first_free.compare_exchange_strong(expected_first.value, concurrent_key_pair_helper(block_num, expected_first.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
		ckeys[block - 1].store(expected_first.parts.index, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename ...P>
void fixed_sz_list<T, block, index_sz>::emplace(P&& ... params) {
	concurrent_key_pair_helper free_spot(first_free.load(std::memory_order_acquire));
	while (true) {
		if (free_spot.parts.index == (uint32_t)-1) {
			create_new_block();
			free_spot.value = first_free.load(std::memory_order_acquire);
		}
		while (free_spot.parts.index != (uint32_t)-1) {
			const auto block_num = free_spot.parts.index >> ct_log2(block);
			const auto block_index = free_spot.parts.index & (block - 1);

			const auto local_index = index_array[block_num].load(std::memory_order_acquire);
			std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

			auto& this_spot = local_index[block_index];
			const auto next_free = keys[block_index].load(std::memory_order_acquire);

			if (first_free.compare_exchange_strong(free_spot.value, concurrent_key_pair_helper(next_free, free_spot.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
				new (&this_spot) T(std::forward<P>(params) ...);

				concurrent_key_pair_helper expected_first_in_list(first_in_list.load(std::memory_order_acquire));
				keys[block_index].store(expected_first_in_list.parts.index, std::memory_order_release);

				while (!first_in_list.compare_exchange_strong(expected_first_in_list.value, concurrent_key_pair_helper(free_spot.parts.index, expected_first_in_list.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
					keys[block_index].store(expected_first_in_list.parts.index, std::memory_order_release);
				}
				return;
			}
		}
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename F>
void fixed_sz_list<T, block, index_sz>::try_pop(const F& f) {
	concurrent_key_pair_helper head(first_in_list.load(std::memory_order_acquire));

	while (head.parts.index != (uint32_t)-1) {

		const auto block_num = head.parts.index >> ct_log2(block);
		const auto block_index = head.parts.index & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		auto& this_spot = local_index[block_index];
		auto expected_next = keys[block_index].load(std::memory_order_acquire);

		if (first_in_list.compare_exchange_strong(head.value, concurrent_key_pair_helper(expected_next, head.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
			f(this_spot);
			this_spot.~T();

			concurrent_key_pair_helper expected_first_free(first_free.load(std::memory_order_acquire));
			keys[block_index].store(expected_first_free.parts.index, std::memory_order_release);

			while (!first_free.compare_exchange_strong(expected_first_free.value, concurrent_key_pair_helper(head.parts.index, expected_first_free.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
				keys[block_index].store(expected_first_free.parts.index, std::memory_order_release);
			}

			return;
		}
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename F>
void fixed_sz_list<T, block, index_sz>::flush(const F& f) {
	concurrent_key_pair_helper head(first_in_list.load(std::memory_order_acquire));

	while (head.parts.index != (uint32_t)-1) {

		const auto block_num = head.parts.index >> ct_log2(block);
		const auto block_index = head.parts.index & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		auto& this_spot = local_index[block_index];
		auto expected_next = keys[block_index].load(std::memory_order_acquire);

		if (first_in_list.compare_exchange_strong(head.value, concurrent_key_pair_helper(expected_next, head.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
			f(this_spot);
			this_spot.~T();

			concurrent_key_pair_helper expected_first_free(first_free.load(std::memory_order_acquire));
			keys[block_index].store(expected_first_free.parts.index, std::memory_order_release);

			while (!first_free.compare_exchange_strong(expected_first_free.value, concurrent_key_pair_helper(head.parts.index, expected_first_free.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
				keys[block_index].store(expected_first_free.parts.index, std::memory_order_release);
			}

			head.value = first_in_list.load(std::memory_order_acquire);
		}
	}
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
fixed_sz_deque<T, block, index_sz, tag_type>::fixed_sz_deque() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(created + block);

	keys[block - 1].store((uint32_t)-1, std::memory_order_release);
	for (int32_t i = block - 2; i >= 0; --i) {
		keys[i].store(static_cast<uint32_t>(i+1), std::memory_order_release);
	}
	index_array[0].store(created, std::memory_order_release);
	first_free.store(concurrent_key_pair_helper(0, 0).value, std::memory_order_release);
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
fixed_sz_deque<T, block, index_sz, tag_type>::~fixed_sz_deque() {
	for (int32_t i = static_cast<int32_t>(first_free_index.load(std::memory_order_relaxed)) - 1; i >= 0; --i) {
		const auto p = index_array[i].load(std::memory_order_relaxed);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(p + block);

		for (int32_t j = block - 1; j >= 0; --j) {
			if (keys[j].load(std::memory_order_relaxed) == (uint32_t)-2)
				p[j].~T();
		}
		_aligned_free(p);
	}
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
T& fixed_sz_deque<T, block, index_sz, tag_type>::at(tag_type index) const {
	const uint32_t block_num = static_cast<uint32_t>(to_index(index)) >> ct_log2(block);
	const uint32_t block_index = static_cast<uint32_t>(to_index(index)) & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	return local_index[block_index];
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
template<typename F>
void fixed_sz_deque<T, block, index_sz, tag_type>::visit(tag_type index, const F& f) const {
	const uint32_t block_num = static_cast<uint32_t>(to_index(index)) >> ct_log2(block);
	const uint32_t block_index = static_cast<uint32_t>(to_index(index)) & (block - 1);

	if (block_num < index_sz) {
		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		if(local_index != nullptr && keys[block_index].load(std::memory_order::memory_order_acquire) == (uint32_t)-2)
			f(local_index[block_index]);
	}
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
T* fixed_sz_deque<T, block, index_sz, tag_type>::safe_at(tag_type index) const {
	if (!is_valid_index(index))
		return nullptr;

	const uint32_t block_num = static_cast<uint32_t>(to_index(index)) >> ct_log2(block);
	const uint32_t block_index = static_cast<uint32_t>(to_index(index)) & (block - 1);

	if (block_num < index_sz) {
		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		if (local_index != nullptr && keys[block_index].load(std::memory_order::memory_order_acquire) == (uint32_t)-2)
			return &local_index[block_index];
	}
	return nullptr;
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
fixed_sz_deque_iterator<T, block, index_sz, tag_type> fixed_sz_deque<T, block, index_sz, tag_type>::begin() const {
	return fixed_sz_deque_iterator<T, block, index_sz, tag_type>(*this, 0);
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
fixed_sz_deque_iterator<T, block, index_sz, tag_type> fixed_sz_deque<T, block, index_sz, tag_type>::end() const {
	return fixed_sz_deque_iterator<T, block, index_sz, tag_type>(*this, static_cast<int32_t>(past_end()));
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
uint32_t fixed_sz_deque<T, block, index_sz, tag_type>::past_end() const {
	for (uint32_t i = 1; i < index_sz; ++i) {
		if (index_array[i].load(std::memory_order_acquire) == nullptr) {
			return i << ct_log2(block);
		}
	}
	return index_sz << ct_log2(block);
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
template<typename U>
void fixed_sz_deque<T, block, index_sz, tag_type>::free(tag_type index, U& u) {
	const uint32_t block_num = static_cast<uint32_t>(to_index(index)) >> ct_log2(block);
	const uint32_t block_index = static_cast<uint32_t>(to_index(index)) & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	u.destroy(local_index[block_index]);

	concurrent_key_pair_helper free_spot(first_free.load(std::memory_order_acquire));

	keys[block_index].store(free_spot.parts.index, std::memory_order_release);
	while (!first_free.compare_exchange_strong(free_spot.value, concurrent_key_pair_helper(to_index(index), free_spot.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
		keys[block_index].store(free_spot.parts.index, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
void fixed_sz_deque<T, block, index_sz, tag_type>::free(tag_type index) {
	const uint32_t block_num = static_cast<uint32_t>(to_index(index)) >> ct_log2(block);
	const uint32_t block_index = static_cast<uint32_t>(to_index(index)) & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	local_index[block_index].~T();

	concurrent_key_pair_helper free_spot (first_free.load(std::memory_order_acquire));

	keys[block_index].store(free_spot.parts.index, std::memory_order_release);
	while (!first_free.compare_exchange_strong(free_spot.value, concurrent_key_pair_helper(to_index(index), free_spot.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
		keys[block_index].store(free_spot.parts.index, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
void fixed_sz_deque<T, block, index_sz, tag_type>::create_new_block() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	const auto new_index = first_free_index.fetch_add(1, std::memory_order_acq_rel);

	if (new_index >= index_sz) {
		_aligned_free(created);
		std::abort();
	}

	std::atomic<uint32_t>* const ckeys = (std::atomic<uint32_t>*)(created + block);
	const auto block_num = new_index << ct_log2(block);

	for (int32_t i = block - 2; i >= 0; --i) {
		ckeys[i].store(block_num + static_cast<uint32_t>(i) + 1, std::memory_order_release);
	}

	concurrent_key_pair_helper expected_first(first_free.load(std::memory_order_acquire));

	ckeys[block - 1].store(expected_first.parts.index, std::memory_order_release);

	index_array[new_index].store(created, std::memory_order_release);

	while (!first_free.compare_exchange_strong(expected_first.value, concurrent_key_pair_helper(block_num, expected_first.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
		ckeys[block - 1].store(expected_first.parts.index, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
template<typename ...P>
T& fixed_sz_deque<T, block, index_sz, tag_type>::emplace_at(tag_type location, P&& ... params) {
	const uint32_t block_num = static_cast<uint32_t>(to_index(location)) >> ct_log2(block);
	const uint32_t block_index = static_cast<uint32_t>(to_index(location)) & (block - 1);

	while (first_free_index.load(std::memory_order_relaxed) < block_num) {
		create_new_block();
	}
	
	const auto local_index = index_array[block_num].load(std::memory_order_relaxed);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	if (keys[block_index].load(std::memory_order_relaxed) != (uint32_t)-2) {
		concurrent_key_pair_helper ff(first_free.load(std::memory_order_relaxed));
		std::atomic<uint32_t>* current_key = (std::atomic<uint32_t>*)(index_array[ff.parts.index >> ct_log2(block)].load(std::memory_order_relaxed) + block)
			+ (ff.parts.index & (block - 1));

		if (ff.parts.index == static_cast<uint32_t>(to_index(location))) {
			first_free.store(concurrent_key_pair_helper(current_key->load(std::memory_order_relaxed), ff.parts.counter + 1).value, std::memory_order_release);
		} else {
			while (current_key->load(std::memory_order_relaxed) != (uint32_t)-1) {
				const auto current_key_position = current_key->load(std::memory_order_relaxed);
				std::atomic<uint32_t>* next_key =
					(std::atomic<uint32_t>*)(index_array[current_key_position >> ct_log2(block)].load(std::memory_order_relaxed) + block)
					+ (current_key_position & (block - 1));
				if (current_key_position == static_cast<uint32_t>(to_index(location))) {
					current_key->store(next_key->load(std::memory_order_relaxed), std::memory_order_release);
					break;
				}
				current_key = next_key;
			}
		}
	} else {
		local_index[block_index].~T();
	}

	new (&(local_index[block_index])) T(std::forward<P>(params) ...);
	keys[block_index].store((uint32_t)-2, std::memory_order_release);

	return local_index[block_index];
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
T& fixed_sz_deque<T, block, index_sz, tag_type>::ensure_reserved(tag_type location) {
	const uint32_t block_num = static_cast<uint32_t>(to_index(location)) >> ct_log2(block);
	const uint32_t block_index = static_cast<uint32_t>(to_index(location)) & (block - 1);

	while (first_free_index.load(std::memory_order_relaxed) < block_num) {
		create_new_block();
	}

	const auto local_index = index_array[block_num].load(std::memory_order_aquire);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	while (keys[block_index].load(std::memory_order_aquire) == (uint32_t)-2) {
		emplace();
	}

	return local_index[block_index];
}


template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
template<typename ...P>
tagged_object<T, tag_type> fixed_sz_deque<T, block, index_sz, tag_type>::emplace(P&& ... params) {
	concurrent_key_pair_helper free_spot(first_free.load(std::memory_order_acquire));

	while (true) {
		if (free_spot.parts.index == (uint32_t)-1) {
			create_new_block();
			free_spot = concurrent_key_pair_helper(first_free.load(std::memory_order_acquire));
		}
		while (free_spot.parts.index != (uint32_t)-1) {
			const auto block_num = (free_spot.parts.index) >> ct_log2(block);
			const auto block_index = (free_spot.parts.index) & (block - 1);

			const auto local_index = index_array[block_num].load(std::memory_order_acquire);
			std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

			auto& this_spot = local_index[block_index];
			const auto next_free = keys[block_index].load(std::memory_order_acquire);

			if (first_free.compare_exchange_strong(free_spot.value, concurrent_key_pair_helper(next_free, free_spot.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
				new (&this_spot) T(std::forward<P>(params) ...);
				keys[block_index].store((uint32_t)-2, std::memory_order_release);
				return tagged_object<T, tag_type>{this_spot, tag_type(static_cast<value_base_of<tag_type>>(free_spot.parts.index))};
			}
		}
	}
}
