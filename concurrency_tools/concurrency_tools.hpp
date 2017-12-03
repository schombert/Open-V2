#pragma once
#include "concurrency_tools.h"

__declspec(restrict) void* concurrent_alloc_wrapper(size_t sz);
void concurrent_free_wrapper(void* p);


template<typename T>
T* concurrent_allocator<T>::allocate(size_t n) {
	return (T*)concurrent_alloc_wrapper(n * sizeof(T));
}

template<typename T>
void concurrent_allocator<T>::deallocate(T* p, size_t n) {
	concurrent_free_wrapper(p);
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque<T, block, index_sz>::fixed_sz_deque() {
	const auto created = (entry_node*)new char[block * sizeof(entry_node)];
	
	created[block - 1].next_free.store((uint32_t)-1, std::memory_order_release);
	for (int32_t i = block - 2; i >= 0; --i) {
		created[i].next_free.store(i+2, std::memory_order_release);
	}
	index_array[0].store(created, std::memory_order_release);
	first_free.store(1, std::memory_order_release);
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque<T, block, index_sz>::~fixed_sz_deque() {
	for (int32_t i = first_free_index.load(std::memory_order_relaxed) - 1; i >= 0; --i) {
		const auto p = index_array[i].load(std::memory_order_relaxed);
		for (int32_t j = block - 1; j >= 0; --j) {
			if (p[j].next_free.load(std::memory_order_relaxed) == 0)
				p[j].e.~T();
		}
		delete[] p;
	}
}

constexpr uint32_t ct_log2(uint32_t n) {
	return ((n < 2) ? 0 : 1 + ct_log2(n / 2));
}

template<typename T, uint32_t block, uint32_t index_sz>
T& fixed_sz_deque<T, block, index_sz>::at(uint32_t index) const {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	return local_index[block_index].e;
}

template<typename T, uint32_t block, uint32_t index_sz>
typename fixed_sz_deque<T, block, index_sz>::entry_node& fixed_sz_deque<T, block, index_sz>::node_at(uint32_t index) const {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	return local_index[block_index];
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque_iterator<T, block, index_sz> fixed_sz_deque<T, block, index_sz>::begin() const {
	return fixed_sz_deque_iterator<T, block, index_sz>(*this, 0);
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque_iterator<T, block, index_sz> fixed_sz_deque<T, block, index_sz>::end() const {
	return fixed_sz_deque_iterator<T, block, index_sz>(*this, past_end());
}

template<typename T, uint32_t block, uint32_t index_sz>
uint32_t fixed_sz_deque<T, block, index_sz>::past_end() const {
	for (int32_t i = 1; i < index_sz; ++i) {
		if (index_array[i].load(std::memory_order_acquire) == nullptr) {
			return i << ct_log2(block);
		}
	}
	return index_sz << ct_log2(block);
}

template<typename T, uint32_t block, uint32_t index_sz>
void fixed_sz_deque<T, block, index_sz>::free(uint32_t index) {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);
	const auto local_index = index_array[block_num].load(std::memory_order_acquire);

	local_index[block_index].e.~T();

	++index;

	auto free_spot = first_free.load(std::memory_order_acquire);
	local_index[block_index].next_free.store(free_spot, std::memory_order_release);
	while (!first_free.compare_exchange_strong(free_spot, index, std::memory_order_release, std::memory_order_acquire)) {
		local_index[block_index].next_free.store(free_spot, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename ...P>
uint32_t fixed_sz_deque<T, block, index_sz>::emplace_back(P&& ... params) {
	auto free_spot = first_free.load(std::memory_order_acquire);

	while (free_spot != (uint32_t)-1) {
		const auto block_num = (free_spot - 1) >> ct_log2(block);
		const auto block_index = (free_spot - 1) & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		auto& this_spot = local_index[block_index];
		const auto next_free = this_spot.next_free.load(std::memory_order_acquire);

		if (first_free.compare_exchange_strong(free_spot, next_free, std::memory_order_release, std::memory_order_acquire)) {
			new (&this_spot.e) T(std::forward<P>(params) ...);
			this_spot.next_free.store(0, std::memory_order_release);
			return free_spot - 1;
		}
	}

	const auto created = (entry_node*)new char[block * sizeof(entry_node)];
	const auto new_index = first_free_index.fetch_add(1, std::memory_order_acq_rel);

	if (new_index >= index_sz) {
		delete[] created;
		std::abort();
	}
		
	const auto block_num = new_index << ct_log2(block);

	for (int32_t i = block - 2; i >= 0; --i) {
		created[i].next_free.store(block_num + i + 2, std::memory_order_release);
	}

	auto expected_first = first_free.load(std::memory_order_acquire);
	created[block - 1].next_free.store(expected_first, std::memory_order_release);

	index_array[new_index].store(created, std::memory_order_release);

	while (!first_free.compare_exchange_strong(expected_first, block_num + 2, std::memory_order_release, std::memory_order_acquire)) {
		created[block - 1].next_free.store(expected_first, std::memory_order_release);
	}

	new (&created[0].e) T(std::forward<P>(params) ...);
	created[0].next_free.store(0, std::memory_order_release);

	return block_num;
}