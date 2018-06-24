#pragma once
#include "concurrency_tools.h"
#include "common\\common.h"
#include <cstdlib>

__declspec(restrict) void* concurrent_alloc_wrapper(size_t sz);
void concurrent_free_wrapper(void* p);

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
stable_vector<object_type, index_type, block_size, index_size>::~stable_vector() {
	for(auto i : index_array) {
		if(i) {
			for(int32_t j = static_cast<int32_t>(block_size) - 1; j >= 0; --j)
				i[j]->~object_type();
			_aligned_free(i);
		}
	}
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
stable_vector<object_type, index_type, block_size, index_size>::stable_vector() { }

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_vector<object_type, index_type, block_size, index_size>::get(index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);
	return (index_array[block_num])[block_index];
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
bool stable_vector<object_type, index_type, block_size, index_size>::is_valid_index(index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	return is_valid_index(i) & (index_array[block_num] != nullptr);
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_vector<object_type, index_type, block_size, index_size>::safe_get(index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);

	if(index_array[block_num] == nullptr) {
		object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size, 64);
		for(int32_t i = static_cast<int32_t>(block_size) - 1; i >= 0; --i)
			new (new_block + i) object_type();
		index_array[block_num] = new_block;
	}

	return (index_array[block_num])[block_index];
}

#ifdef _DEBUG
struct stable_vector_full {};
#endif

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_vector<object_type, index_type, block_size, index_size>::get_new() {
	for(uint32_t i = 0; i < index_size; ++i) {
		if(index_array[i]) {
			for(int32_t j = static_cast<int32_t>(block_size) - 1; j >= 0; --j) {
				if(!is_valid_index((index_array[i])[j].id)) {
					(index_array[i])[j].id = index_type(static_cast<value_base_of<index_type>>(j + (i << ct_log2(block))));
					return (index_array[i])[j];
				}
			}
		} else {
			object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size, 64);
			for(int32_t i = static_cast<int32_t>(block_size) - 1; i >= 0; --i)
				new (new_block + i) object_type();
			index_array[i] = new_block;
			new_block[0].id = index_type(static_cast<value_base_of<index_type>>(i << ct_log2(block)));
			return new_block[0];
		}
	}
#ifdef _DEBUG
	throw stable_vector_full();
#else
	std::abort();
#endif
}

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
void stable_vector<object_type, index_type, block_size, index_size>::remove(index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);

	(index_array[block_num])[block_index].~object_type();
	new ((index_array[block_num]) + block_index) object_type();
}

#ifdef _DEBUG
struct stable_2d_vector_full {};
#endif

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::stable_2d_vector() {}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::~stable_2d_vector() {
	for(auto i : index_array) {
		if(i) {
			for(int32_t j = static_cast<int32_t>(block_size) * inner_size - 1; j >= 0; --j)
				i[j]->~object_type();
			_aligned_free(i);
		}
	}
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
void stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::reset(uint32_t new_inner_size) {
	for(auto& i : index_array) {
		if(i) {
			for(int32_t j = static_cast<int32_t>(block_size) * inner_size - 1; j >= 0; --j)
				i[j]->~object_type();
			_aligned_free(i);
			i = nullptr;
		}
	}
	inner_size = new_inner_size;
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
void stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::ensure_capacity(uint32_t new_outer_size) {
	const auto block_num = new_outer_size >> ct_log2(block);

	if(block_num >= index_size) {
#ifdef _DEBUG
		throw stable_2d_vector_full();
#else
		std::abort();
#endif
	}

	for(uint32_t i = 0; i <= block_num; ++i) {
		if(index_array[i] == nullptr) {
			object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size * inner_size, 64);
			for(int32_t i = static_cast<int32_t>(block_size) * inner_size - 1; i >= 0; --i)
				new (new_block + i) object_type();
			index_array[i] = new_block;
		}
	}
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
void stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::clear_row(outer_index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);

	object_type* const block = index_array[block_num];
	for(int32_t i = inner_size - 1; i >= 0; --i) {
		block[block_index * inner_size + i].~object_type();
		new (new_block + block_index * inner_size + i) object_type();
	}
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type* stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::get_row(outer_index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);

	return index_array[block_num] + block_index * inner_size;
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type* stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::safe_get_row(outer_index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);

	if(block_num >= index_size) {
#ifdef _DEBUG
		throw stable_2d_vector_full();
#else
		std::abort();
#endif
	}

	if(index_array[block_num] == nullptr) {
		object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size * inner_size, 64);
		for(int32_t i = static_cast<int32_t>(block_size) * inner_size - 1; i >= 0; --i)
			new (new_block + i) object_type();
		index_array[block_num] = new_block;
		return new_block + block_index * inner_size;
	} else {
		return index_array[block_num] + block_index * inner_size;
	}
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::get(outer_index_type i, inner_index_type j) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);

	return *(index_array[block_num] + block_index * inner_size + to_index(j));
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
object_type& stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::safe_get(outer_index_type i, inner_index_type j) {
	const auto block_num = to_index(i) >> ct_log2(block);
	const auto block_index = to_index(i) & (block - 1);

	if(block_num >= index_size) {
#ifdef _DEBUG
		throw stable_2d_vector_full();
#else
		std::abort();
#endif
	}

	if(index_array[block_num] == nullptr) {
		object_type* new_block = (object_type*)_aligned_malloc(sizeof(object_type) * block_size * inner_size, 64);
		for(int32_t i = static_cast<int32_t>(block_size) * inner_size - 1; i >= 0; --i)
			new (new_block + i) object_type();
		index_array[block_num] = new_block;
		return *(new_block + block_index * inner_size + to_index(j));
	} else {
		return *(index_array[block_num] + block_index * inner_size + to_index(j));
	}
}

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
bool stable_2d_vector<object_type, outer_index_type, inner_index_type, block_size, index_size>::is_valid_index(outer_index_type i) {
	const auto block_num = to_index(i) >> ct_log2(block);
	return index_array[block_num] != nullptr;
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>::stable_variable_vector_storage() {}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>::~stable_variable_vector_storage() {
	for(auto i : index_array) {
		if(i) {
			for(int32_t j = static_cast<int32_t>(block_size * contiguous_block_count) - 1; j >= 0; --j)
				i[j]->~object_type();
			_aligned_free(i);
		}
	}
}

#ifdef _DEBUG
struct stable_variable_vector_storage_full {};
#endif

#ifdef _DEBUG
struct vector_too_big_for_stable_variable_vector_storage {};
#endif

namespace detail {
	struct array_chunk {
		uint16_t previous_in_free_list; // 0 = empty, subtract 1 for index
		uint16_t next_in_free_list; // 0 = empty, subtract 1 for index

		uint8_t local_left_chunk;
		uint8_t local_right_chunk;
		uint8_t size; // + 1 = real size (no size 0)
		uint8_t is_free;
	};

	template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
	void pop_chunk_from_free_list(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, uint32_t size_in_blocks) {
		const auto first_in_list = storage.index_array[size_in_blocks - 1];

		const auto this_block_num = (first_in_list - 1) >> ct_log2(contiguous_block_count);
		const auto this_block_index = (first_in_list - 1) & (contiguous_block_count - 1);

		array_chunk* cstart = (array_chunk*)(storage.index_array[this_block_num] + block_size * contiguous_block_count);

		storage.index_array[size_in_blocks - 1] = (cstart + this_block_index)->next_in_free_list;
		(cstart + this_block_index)->next_in_free_list = 0ui16;
		(cstart + this_block_index)->is_free = 0ui8;
	}

	template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
	void push_chunk_to_free_list(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, uint32_t chunk_index, array_chunk* chunk) {
		const auto first_in_list = storage.index_array[chunk->size + 1 - 1];
		chunk->next_in_free_list = first_in_list;

		const auto this_block_num = (first_in_list - 1) >> ct_log2(contiguous_block_count);
		const auto this_block_index = (first_in_list - 1) & (contiguous_block_count - 1);

		array_chunk* cstart = (array_chunk*)(storage.index_array[this_block_num] + block_size * contiguous_block_count);

		(cstart + this_block_index)->previous_in_free_list = chunk_index;
	}

	template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
	void remove_chunk_from_free_list(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, uint32_t chunk_index, array_chunk* chunk) {
		//fix next
		if(chunk->next_in_free_list != 0) {
			const auto this_block_num = (chunk->next_in_free_list - 1) >> ct_log2(contiguous_block_count);
			const auto this_block_index = (chunk->next_in_free_list - 1) & (contiguous_block_count - 1);

			array_chunk* cstart = (array_chunk*)(storage.index_array[this_block_num] + block_size * contiguous_block_count);
			(cstart + this_block_index)->previous_in_free_list = chunk->previous_in_free_list;
		}
		//fix previous
		if(chunk->previous_in_free_list != 0) {
			const auto this_block_num = (chunk->previous_in_free_list - 1) >> ct_log2(contiguous_block_count);
			const auto this_block_index = (chunk->previous_in_free_list - 1) & (contiguous_block_count - 1);

			array_chunk* cstart = (array_chunk*)(storage.index_array[this_block_num] + block_size * contiguous_block_count);
			(cstart + this_block_index)->next_in_free_list = chunk->next_in_free_list;
		} else {
			//is head: pop
			storage.index_array[chunk->size + 1 - 1] = chunk->next_in_free_list;
		}
		chunk->previous_in_free_list = 0ui16;
		chunk->next_in_free_list = 0ui16;
	}

	template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
	void allocate_from_array_chunk(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, uint32_t chunk_index, uint32_t allocated_blocks) {
		const auto this_block_num = (chunk_index - 1) >> ct_log2(contiguous_block_count);
		const auto this_block_index = (chunk_index - 1) & (contiguous_block_count - 1);

		array_chunk* cstart = (array_chunk*)(storage.index_array[this_block_num] + block_size * contiguous_block_count);
		
		array_chunk* allocated_portion = cstart + this_block_index;
		array_chunk* new_free = cstart + this_block_index + allocated_blocks;

		if(allocated_portion->local_right_chunk != 0ui8) {
			array_chunk* old_right = cstart + allocated_portion->local_right_chunk;
			old_right->local_left_chunk = uint8_t(this_block_index + allocated_blocks);
		}

		*new_free = array_chunk{0ui16, 0ui16, uint8_t(this_block_index), allocated_portion->local_right_chunk, uint8_t(allocated_portion->size - allocated_blocks), 1ui8 };
		*allocated_portion = array_chunk{ 0ui16, 0ui16, allocated_portion->local_left_chunk, uint8_t(this_block_index + allocated_blocks), uint8_t(allocated_blocks - 1), 0ui8};

		push_chunk_to_free_list(storage, chunk_index + allocated_blocks, new_free);
	}

	template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
	void free_chunk(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, uint32_t chunk_index) {
		auto this_block_num = (chunk_index - 1) >> ct_log2(contiguous_block_count);
		auto this_block_index = (chunk_index - 1) & (contiguous_block_count - 1);

		array_chunk* cstart = (array_chunk*)(storage.index_array[this_block_num] + block_size * contiguous_block_count);
		array_chunk* this_chunk = cstart + this_block_index;

		this_chunk->is_free = 1ui8;

		if(this_block_index != 0) {
			//try to merge left
			array_chunk* left_chunk = cstart + this_chunk->local_left_chunk;
			if(left_chunk->is_free != 0) {
				remove_chunk_from_free_list(storage, chunk_index - this_block_index + this_chunk->local_left_chunk, left_chunk);
				left_chunk->local_right_chunk = this_chunk->local_right_chunk;
				left_chunk->size += (this_chunk->size + 1);
				
				chunk_index += (this_chunk->local_left_chunk - this_block_index);
				this_block_index = this_chunk->local_left_chunk;
				this_chunk = left_chunk;
			}
		}
		if(this_chunk->local_right_chunk != 0ui8) {
			array_chunk* right_chunk = cstart + this_chunk->local_right_chunk;
			if(right_chunk->is_free != 0ui8) {
				if(right_chunk->local_right_chunk != 0ui8)
					(cstart + right_chunk->local_right_chunk)->local_left_chunk = uint8_t(this_block_index);

				remove_chunk_from_free_list(storage, chunk_index - this_block_index + this_chunk->local_right_chunk, right_chunk);
				this_chunk->local_right_chunk = right_chunk->local_right_chunk;
				this_chunk->size += (right_chunk->size + 1);
			}
		}
		push_chunk_to_free_list(storage, chunk_index, this_chunk);
	}

	template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
	void make_new_backing_buffer(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, uint32_t index, uint32_t allocated_blocks) {
		const auto new_mem = (object_type*)_aligned_malloc(sizeof(object_type) * block_size * contiguous_block_count + sizeof(array_chunk) * contiguous_block_count, 64);
		storage.index_array[index] = new_mem;

		for(int32_t i = static_cast<int32_t>(block_size * contiguous_block_count) - 1; i >= 0; --i)
			new (new_mem + i) object_type();
		
		array_chunk* fblock = (array_chunk*)(new_mem + block_size * contiguous_block_count);

		const auto this_block_index = (index << ct_log2(contiguous_block_count)) + 1;
		if(allocated_blocks != contiguous_block_count) {
			*fblock = array_chunk{ 0ui16, 0ui16, 0ui8, uint8_t(allocated_blocks), uint8_t(allocated_blocks - 1), 0ui8 };
			*(fblock + allocated_blocks) = array_chunk{ storage.free_lists[contiguous_block_count - allocated_blocks - 1], 0ui16, 0ui8, uint8_t(allocated_blocks), uint8_t(contiguous_block_count - allocated_blocks - 1), 1ui8};
			storage.free_lists[contiguous_block_count - allocated_blocks - 1] = uint16_t(this_block_index + allocated_blocks);
		} else {
			*fblock = array_chunk{ 0ui16, 0ui16, 0ui8, 0ui8, uint8_t(contiguous_block_count - 1), 0ui8 };
		}
	}

	template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
	uint32_t allocate_blocks(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, uint32_t allocated_blocks) {
		if(storage.free_lists[allocated_blocks - 1] != 0ui16) {
			const auto allocated = storage.free_lists[allocated_blocks - 1];
			pop_chunk_from_free_list(storage, allocated_blocks);
			return allocated - 1;
		}
		for(uint32_t i = allocated_blocks; i < contiguous_block_count; ++i) {
			if(storage.free_lists[i] != 0ui16) {
				const auto allocated = storage.free_lists[i];
				pop_chunk_from_free_list(storage, i + 1);
				allocate_from_array_chunk(storage, allocated, allocated_blocks);
				return allocated - 1;
			}
		}
		for(uint32_t i = 0; i < index_size; ++i) {
			if(storage.index_array[i] == nullptr) {
				make_new_backing_buffer(storage, i, allocated_blocks);
				return i << ct_log2(contiguous_block_count);
			}
		}
#ifdef _DEBUG
		throw stable_variable_vector_storage_full();
#else
		std::abort();
#endif
	}
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
uint32_t get_capacity(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag const& i) {
	const auto offset = i.block_offset.load(std::memory_order_acquire);

	auto this_block_num = (offset) >> ct_log2(contiguous_block_count);
	auto this_block_index = (offset) & (contiguous_block_count - 1);

	detail::array_chunk* cstart = (detail::array_chunk*)(storage.index_array[this_block_num] + block_size * contiguous_block_count);
	detail::array_chunk* this_chunk = cstart + this_block_index;

	return uint32_t(this_chunk->size + 1) * block_size;
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>::increase_capacity(stable_variable_vector_tag& index_obj, uint32_t new_capacity_in_blocks) {
	if(new_capacity_in_blocks > contiguous_block_count) {
#ifdef _DEBUG
		throw vector_too_big_for_stable_variable_vector_storage();
#else
		std::abort();
#endif
	}

	const auto old_block_capacity = get_capacity(*this, index_obj) / block_size;

	if(old_block_capacity >= new_capacity_in_blocks)
		return;

	const auto old_range = get_range(*this, index_obj);
	const auto new_offset = detail::allocate_blocks(*this, new_capacity_in_blocks);

	const auto block_num = new_offset >> ct_log2(contiguous_block_count);
	const auto block_index = new_offset & (contiguous_block_count - 1);
	object_type* first = storage.index_array[block_num] + block_index * block_size;

	std::copy(old_range.first, old_range.second, first);

	index_obj.block_offset.store(new_offset, std::memory_order_release);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>::grow(stable_variable_vector_tag& index_obj) {
	const auto old_block_capacity = get_capacity(*this, index_obj) / block_size;

	if(old_block_capacity + 1 <= contiguous_block_count) {
		const auto old_range = get_range(*this, index_obj);
		const auto new_offset = detail::allocate_blocks(*this, new_capacity_in_blocks);

		const auto block_num = new_offset >> ct_log2(contiguous_block_count);
		const auto block_index = new_offset & (contiguous_block_count - 1);
		object_type* first = storage.index_array[block_num] + block_index * block_size;

		std::copy(old_range.first, old_range.second, first);

		index_obj.block_offset.store(new_offset, std::memory_order_release);
	} else {
#ifdef _DEBUG
			throw vector_too_big_for_stable_variable_vector_storage();
#else
			std::abort();
#endif
	}
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>::shrink_capacity(stable_variable_vector_tag& index_obj) {

	const auto old_block_capacity = get_capacity(*this, index_obj) / block_size;
	const auto new_block_capacity = (index_obj.size.load(std::memory_order_relaxed) + (block_size - 1)) / block_size;

	if(new_block_capacity < old_block_capacity) {
		const auto new_block_offset = detail::allocate_blocks(*this, new_capacity_in_blocks);
		const auto old_range = get_range(*this, index_obj);

		const auto block_num = new_block_offset >> ct_log2(contiguous_block_count);
		const auto block_index = new_block_offset & (contiguous_block_count - 1);

		object_type* first = storage.index_array[block_num] + block_index * block_size;

		std::copy(old_range.first, old_range.second, first);

		detail::free_chunk(*this, index_obj.block_offset.load(std::memory_order_relaxed) + 1ui16);
		index_obj.block_offset.store(new_block_offset, std::memory_order_release);
	}
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>::release(stable_variable_vector_tag& index_obj) {
	detail::free_chunk(*this, index_obj.block_offset.load(std::memory_order_relaxed) + 1ui16);
	index_obj.size.store(uint16_t(0), std::memory_order_release);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag const& i) {
	const auto size = i.size.load(std::memory_order_acquire);
	const auto offset = i.block_offset.load(std::memory_order_acquire));

	const auto block_num = offset >> ct_log2(contiguous_block_count);
	const auto block_index = offset & (contiguous_block_count - 1);
	
	object_type* first = storage.index_array[block_num] + block_index * block_size;
	return std::pair<object_type*, object_type*>(first, first + size);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void push_back(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj) {
	const auto size = i.size.load(std::memory_order_relaxed);
	const auto capacity = get_capacity(storage, i);

	if(size >= capacity)
		storage.grow(storage);

	const auto offset = i.block_offset.load(std::memory_order_relaxed));
	const auto block_num = offset >> ct_log2(contiguous_block_count);
	const auto block_index = offset & (contiguous_block_count - 1);

	*(storage.index_array[block_num] + block_index * block_size + size) = obj;
	i.size.store(size + 1, std::memory_order_release);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_unordered_range(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type const* first, object_type const* last) {
	const uint32_t count = static_cast<uint32_t>(last - first);

	const auto size = i.size.load(std::memory_order_relaxed);
	const auto capacity = get_capacity(storage, i);

	if(size + count > capacity) {
		const uint32_t new_block_capacity = (size + count + block_size - 1) / block_size;
		storage.increase_capacity(i, new_block_capacity);
	}

	const auto offset = i.block_offset.load(std::memory_order_relaxed));
	const auto block_num = offset >> ct_log2(contiguous_block_count);
	const auto block_index = offset & (contiguous_block_count - 1);

	std::copy(first, last, storage.index_array[block_num] + block_index * block_size + size);

	i.size.store(uint16_t(size + count), std::memory_order_release);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj) {
	const auto size = i.size.load(std::memory_order_relaxed);
	const auto capacity = get_capacity(storage, i);

	if(size >= capacity)
		storage.grow(storage);

	const auto offset = i.block_offset.load(std::memory_order_relaxed));
	const auto block_num = offset >> ct_log2(contiguous_block_count);
	const auto block_index = offset & (contiguous_block_count - 1);

	const auto start = storage.index_array[block_num] + block_index * block_size;
	*(start + size) = obj;

	i.size.store(size + 1, std::memory_order_release);

	std::sort(start, start + size + 1);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
bool contains_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj) {
	const auto range = get_range(storage, i);
	return std::binary_search(range.first, range.second, obj);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_unique_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj) {
	if(!contains_item(storage, i, obj))
		add_item(storage, i, obj);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_ordered_range(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type const* first, object_type const* last) {
	const uint32_t count = static_cast<uint32_t>(last - first);

	const auto size = i.size.load(std::memory_order_relaxed);
	const auto capacity = get_capacity(storage, i);

	if(size + count > capacity) {
		const uint32_t new_block_capacity = (size + count + block_size - 1) / block_size;
		storage.increase_capacity(i, new_block_capacity);
	}

	const auto offset = i.block_offset.load(std::memory_order_relaxed));
	const auto block_num = offset >> ct_log2(contiguous_block_count);
	const auto block_index = offset & (contiguous_block_count - 1);

	const auto first_item = storage.index_array[block_num] + block_index * block_size;
	const auto temp_buffer = _alloca(sizeof(object_type) * size);

	std::copy(first_item, first_item + size, temp_buffer);
	std::merge(temp_buffer, temp_buffer + size, first, last, first_item);

	i.size.store(uint16_t(size + count), std::memory_order_release);

	_freea(temp_buffer);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_unique_ordered_range(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type const* first, object_type const* last) {
	const uint32_t count = static_cast<uint32_t>(last - first);

	const auto size = i.size.load(std::memory_order_relaxed);
	const auto capacity = get_capacity(storage, i);

	if(size + count > capacity) {
		const uint32_t new_block_capacity = (size + count + block_size - 1) / block_size;
		storage.increase_capacity(i, new_block_capacity);
	}

	const auto offset = i.block_offset.load(std::memory_order_relaxed));
	const auto block_num = offset >> ct_log2(contiguous_block_count);
	const auto block_index = offset & (contiguous_block_count - 1);

	const auto first_item = storage.index_array[block_num] + block_index * block_size;
	auto temp_buffer = _alloca(sizeof(object_type) * size);

	std::copy(first_item, first_item + size, temp_buffer);
	const auto new_last = std::set_union(temp_buffer, temp_buffer + size, first, last, first_item);

	i.size.store(uint16_t(new_last - first_item), std::memory_order_release);

	_freea(temp_buffer);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void remove_sorted_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj) {
	const auto range = get_range(storage, i);
	const auto lb = std::lower_bound(range.first, range.second, obj);
	if(lb == range.second || *lb != obj)
		return;
	
	std::copy(lb + 1, range.second, lb);
	i.size.store(i.size.load(std::memory_order_relaxed) - 1, std::memory_order_release);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void remove_unsorted_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj) {
	const auto range = get_range(storage, i);
	const auto f = std::find(range.first, range.second, obj);
	if(f == range.second)
		return;
	*f = *(range.second - 1);
	i.size.store(i.size.load(std::memory_order_relaxed) - 1, std::memory_order_release);
}

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void pop_back(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj) {
	const auto old_size = i.size.load(std::memory_order_relaxed);
	if(old_size != 0)
		i.size.store(old_size - 1, std::memory_order_release);
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
			if (this_data[i] != t[i])
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
void concurrent_allocator<T>::deallocate(T* p, size_t n) {
	concurrent_free_wrapper(p);
}

template<typename T>
T* aligned_allocator_32<T>::allocate(size_t n) {
	return (T*)_aligned_malloc(n * sizeof(T), 32);
}

template<typename T>
void aligned_allocator_32<T>::deallocate(T* p, size_t n) {
	_aligned_free(p);
}

constexpr uint32_t ct_log2(uint32_t n) {
	return ((n < 2) ? 0 : 1 + ct_log2(n / 2));
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
