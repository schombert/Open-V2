#pragma once
#include "concurrency_tools.h"
#include <cstdlib>

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

constexpr uint32_t ct_log2(uint32_t n) {
	return ((n < 2) ? 0 : 1 + ct_log2(n / 2));
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_list<T, block, index_sz>::fixed_sz_list() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(created + block);

	keys[block - 1].store((uint32_t)-1, std::memory_order_release);
	for (int32_t i = block - 2; i >= 0; --i) {
		keys[i].store(i + 1, std::memory_order_release);
	}
	index_array[0].store(created, std::memory_order_release);
	first_free.store(0, std::memory_order_release);
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_list<T, block, index_sz>::~fixed_sz_list() {
	auto p = first_in_list.load(std::memory_order_relaxed);
	while (p != (uint32_t)-1) {
		const auto block_num = p >> ct_log2(block);
		const auto block_index = p & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_relaxed);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		local_index[block_index].~T();

		p = keys[block_index].load(std::memory_order_relaxed);
	}
	for (int32_t i = first_free_index.load(std::memory_order_relaxed) - 1; i >= 0; --i) {
		_aligned_free(index_array[i].load(std::memory_order_relaxed));
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename ...P>
void fixed_sz_list<T, block, index_sz>::emplace(P&& ... params) {
	auto free_spot = first_free.load(std::memory_order_acquire);

	while (free_spot != (uint32_t)-1) {
		const auto block_num = free_spot >> ct_log2(block);
		const auto block_index = free_spot & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		auto& this_spot = local_index[block_index];
		const auto next_free = keys[block_index].load(std::memory_order_acquire);

		if (first_free.compare_exchange_strong(free_spot, next_free, std::memory_order_release, std::memory_order_acquire)) {
			new (&this_spot) T(std::forward<P>(params) ...);

			auto expected_first_in_list = first_in_list.load(std::memory_order_acquire);
			keys[block_index].store(expected_first_in_list, std::memory_order_release);

			while (!first_in_list.compare_exchange_strong(expected_first_in_list, free_spot, std::memory_order_release, std::memory_order_acquire)) {
				keys[block_index].store(expected_first_in_list, std::memory_order_release);
			}
			return;
		}
	}

	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const ckeys = (std::atomic<uint32_t>*)(created + block);

	const auto new_index = first_free_index.fetch_add(1, std::memory_order_acq_rel);

	if (new_index >= index_sz) {
		_aligned_free(created);
		std::abort();
	}

	const auto block_num = new_index << ct_log2(block);

	for (int32_t i = block - 2; i >= 0; --i) {
		ckeys[i].store(block_num + i + 1, std::memory_order_release);
	}

	auto expected_first = first_free.load(std::memory_order_acquire);
	ckeys[block - 1].store(expected_first, std::memory_order_release);

	index_array[new_index].store(created, std::memory_order_release);

	while (!first_free.compare_exchange_strong(expected_first, block_num + 1, std::memory_order_release, std::memory_order_acquire)) {
		ckeys[block - 1].store(expected_first, std::memory_order_release);
	}

	new (&created[0]) T(std::forward<P>(params) ...);

	auto expected_first_in_list = first_in_list.load(std::memory_order_acquire);
	ckeys[0].store(expected_first_in_list, std::memory_order_release);

	while (!first_in_list.compare_exchange_strong(expected_first_in_list, block_num, std::memory_order_release, std::memory_order_acquire)) {
		ckeys[0].store(expected_first_in_list, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename F>
void fixed_sz_list<T, block, index_sz>::try_pop(const F& f) {
	auto head = first_in_list.load(std::memory_order_acquire);

	while (head != (uint32_t)-1) {

		const auto block_num = head >> ct_log2(block);
		const auto block_index = head & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		auto& this_spot = local_index[block_index];
		auto expected_next = keys[block_index].load(std::memory_order_acquire);

		if (first_in_list.compare_exchange_strong(head, expected_next, std::memory_order_release, std::memory_order_acquire)) {
			f(this_spot);
			this_spot.~T();

			auto expected_first_free = first_free.load(std::memory_order_acquire);
			keys[block_index].store(expected_first_free, std::memory_order_release);

			while (!first_free.compare_exchange_strong(expected_first_free, head, std::memory_order_release, std::memory_order_acquire)) {
				keys[block_index].store(expected_first_free, std::memory_order_release);
			}

			return;
		}
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename F>
void fixed_sz_list<T, block, index_sz>::flush(const F& f) {
	auto head = first_in_list.load(std::memory_order_acquire);

	while (head != (uint32_t)-1) {

		const auto block_num = head >> ct_log2(block);
		const auto block_index = head & (block - 1);

		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		auto& this_spot = local_index[block_index];
		auto expected_next = keys[block_index].load(std::memory_order_acquire);

		if (first_in_list.compare_exchange_strong(head, expected_next, std::memory_order_release, std::memory_order_acquire)) {
			f(this_spot);
			this_spot.~T();

			auto expected_first_free = first_free.load(std::memory_order_acquire);
			keys[block_index].store(expected_first_free, std::memory_order_release);

			while (!first_free.compare_exchange_strong(expected_first_free, head, std::memory_order_release, std::memory_order_acquire)) {
				keys[block_index].store(expected_first_free, std::memory_order_release);
			}

			head = first_in_list.load(std::memory_order_acquire);
		}
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque<T, block, index_sz>::fixed_sz_deque() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(created + block);

	keys[block - 1].store((uint32_t)-1, std::memory_order_release);
	for (int32_t i = block - 2; i >= 0; --i) {
		keys[i].store(i+2, std::memory_order_release);
	}
	index_array[0].store(created, std::memory_order_release);
	first_free.store(1, std::memory_order_release);
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque<T, block, index_sz>::~fixed_sz_deque() {
	for (int32_t i = first_free_index.load(std::memory_order_relaxed) - 1; i >= 0; --i) {
		const auto p = index_array[i].load(std::memory_order_relaxed);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(p + block);

		for (int32_t j = block - 1; j >= 0; --j) {
			if (keys[j].load(std::memory_order_relaxed) == 0)
				p[j].~T();
		}
		_aligned_free(p);
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
T& fixed_sz_deque<T, block, index_sz>::at(uint32_t index) const {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	return local_index[block_index];
}

template<typename T, uint32_t block, uint32_t index_sz>
template<typename F>
void fixed_sz_deque<T, block, index_sz>::visit(uint32_t index, const F& f) const {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);

	if (block_num < index_sz) {
		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		if(local_index != nullptr && keys[block_index].load(std::memory_order::memory_order_acquire) == 0)
			f(local_index[block_index]);
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
T* fixed_sz_deque<T, block, index_sz>::safe_at(uint32_t index) const {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);

	if (block_num < index_sz) {
		const auto local_index = index_array[block_num].load(std::memory_order_acquire);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		if (local_index != nullptr && keys[block_index].load(std::memory_order::memory_order_acquire) == 0)
			return &local_index[block_index];
	}
	return nullptr;
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
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	local_index[block_index].~T();

	++index;

	auto free_spot = first_free.load(std::memory_order_acquire);
	keys[block_index].store(free_spot, std::memory_order_release);
	while (!first_free.compare_exchange_strong(free_spot, index, std::memory_order_release, std::memory_order_acquire)) {
		keys[block_index].store(free_spot, std::memory_order_release);
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
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

		auto& this_spot = local_index[block_index];
		const auto next_free = keys[block_index].load(std::memory_order_acquire);

		if (first_free.compare_exchange_strong(free_spot, next_free, std::memory_order_release, std::memory_order_acquire)) {
			new (&this_spot) T(std::forward<P>(params) ...);
			keys[block_index].store(0, std::memory_order_release);
			return free_spot - 1;
		}
	}

	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	const auto new_index = first_free_index.fetch_add(1, std::memory_order_acq_rel);

	if (new_index >= index_sz) {
		_aligned_free(created);
		std::abort();
	}
	
	std::atomic<uint32_t>* const ckeys = (std::atomic<uint32_t>*)(created + block);
	const auto block_num = new_index << ct_log2(block);

	for (int32_t i = block - 2; i >= 0; --i) {
		ckeys[i].store(block_num + i + 2, std::memory_order_release);
	}

	auto expected_first = first_free.load(std::memory_order_acquire);
	ckeys[block - 1].store(expected_first, std::memory_order_release);

	index_array[new_index].store(created, std::memory_order_release);

	while (!first_free.compare_exchange_strong(expected_first, block_num + 2, std::memory_order_release, std::memory_order_acquire)) {
		ckeys[block - 1].store(expected_first, std::memory_order_release);
	}

	new (&created[0]) T(std::forward<P>(params) ...);
	ckeys[0].store(0, std::memory_order_release);

	return block_num;
}