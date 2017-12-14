#pragma once
#include "concurrency_tools.h"
#include <cstdlib>

__declspec(restrict) void* concurrent_alloc_wrapper(size_t sz);
void concurrent_free_wrapper(void* p);

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
		l[i] = static_cast<const T*>(this)->operator[](i);
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

constexpr uint32_t ct_log2(uint32_t n) {
	return ((n < 2) ? 0 : 1 + ct_log2(n / 2));
}

union concurrent_key_pair_helper {
	struct {
		uint32_t index;
		uint32_t counter;
	} parts;
	uint64_t value;

	constexpr concurrent_key_pair_helper(uint32_t i, uint32_t c) : parts{ i, c } {};
	constexpr concurrent_key_pair_helper(uint64_t v) : value{ v } {};
};

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_list<T, block, index_sz>::fixed_sz_list() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(created + block);

	keys[block - 1].store((uint32_t)-1, std::memory_order_release);
	for (int32_t i = block - 2; i >= 0; --i) {
		keys[i].store(i + 1, std::memory_order_release);
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
	for (int32_t i = first_free_index.load(std::memory_order_relaxed) - 1; i >= 0; --i) {
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
		ckeys[i].store(block_num + i + 1, std::memory_order_release);
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

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque<T, block, index_sz>::fixed_sz_deque() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(created + block);

	keys[block - 1].store((uint32_t)-1, std::memory_order_release);
	for (int32_t i = block - 2; i >= 0; --i) {
		keys[i].store(i+1, std::memory_order_release);
	}
	index_array[0].store(created, std::memory_order_release);
	first_free.store(concurrent_key_pair_helper(0, 0).value, std::memory_order_release);
}

template<typename T, uint32_t block, uint32_t index_sz>
fixed_sz_deque<T, block, index_sz>::~fixed_sz_deque() {
	for (int32_t i = first_free_index.load(std::memory_order_relaxed) - 1; i >= 0; --i) {
		const auto p = index_array[i].load(std::memory_order_relaxed);
		std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(p + block);

		for (int32_t j = block - 1; j >= 0; --j) {
			if (keys[j].load(std::memory_order_relaxed) == (uint32_t)-2)
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

		if(local_index != nullptr && keys[block_index].load(std::memory_order::memory_order_acquire) == (uint32_t)-2)
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

		if (local_index != nullptr && keys[block_index].load(std::memory_order::memory_order_acquire) == (uint32_t)-2)
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
template<typename U>
void fixed_sz_deque<T, block, index_sz>::free(uint32_t index, U& u) {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	u.destroy(local_index[block_index]);

	concurrent_key_pair_helper free_spot(first_free.load(std::memory_order_acquire));

	keys[block_index].store(free_spot.value, std::memory_order_release);
	while (!first_free.compare_exchange_strong(free_spot.value, concurrent_key_pair_helper(index, free_spot.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
		keys[block_index].store(free_spot.parts.index, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
void fixed_sz_deque<T, block, index_sz>::free(uint32_t index) {
	const auto block_num = index >> ct_log2(block);
	const auto block_index = index & (block - 1);

	const auto local_index = index_array[block_num].load(std::memory_order_acquire);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	local_index[block_index].~T();

	concurrent_key_pair_helper free_spot (first_free.load(std::memory_order_acquire));

	keys[block_index].store(free_spot.value, std::memory_order_release);
	while (!first_free.compare_exchange_strong(free_spot.value, concurrent_key_pair_helper(index, free_spot.parts.counter + 1).value, std::memory_order_release, std::memory_order_acquire)) {
		keys[block_index].store(free_spot.parts.index, std::memory_order_release);
	}
}

template<typename T, uint32_t block, uint32_t index_sz>
void fixed_sz_deque<T, block, index_sz>::create_new_block() {
	const auto created = (T*)_aligned_malloc(block * sizeof(T) + block * sizeof(std::atomic<uint32_t>), 64);
	const auto new_index = first_free_index.fetch_add(1, std::memory_order_acq_rel);

	if (new_index >= index_sz) {
		_aligned_free(created);
		std::abort();
	}

	std::atomic<uint32_t>* const ckeys = (std::atomic<uint32_t>*)(created + block);
	const auto block_num = new_index << ct_log2(block);

	for (int32_t i = block - 2; i >= 0; --i) {
		ckeys[i].store(block_num + i + 1, std::memory_order_release);
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
T& fixed_sz_deque<T, block, index_sz>::emplace_at(uint32_t location, P&& ... params) {
	const auto block_num = (location) >> ct_log2(block);
	const auto block_index = (location) & (block - 1);

	while (first_free_index.load(std::memory_order_relaxed) < block_num) {
		create_new_block();
	}
	
	const auto local_index = index_array[block_num].load(std::memory_order_relaxed);
	std::atomic<uint32_t>* const keys = (std::atomic<uint32_t>*)(local_index + block);

	if (keys[block_index].load(std::memory_order_relaxed) != (uint32_t)-2) {
		concurrent_key_pair_helper ff(first_free.load(std::memory_order_relaxed));
		std::atomic<uint32_t>* current_key = (std::atomic<uint32_t>*)(index_array[ff.parts.index >> ct_log2(block)].load(std::memory_order_relaxed) + block)
			+ (ff.parts.index & (block - 1));

		if (ff.parts.index == location) {
			first_free.store(concurrent_key_pair_helper(current_key->load(std::memory_order_relaxed), ff.parts.counter + 1).value, std::memory_order_release);
		} else {
			while (current_key->load(std::memory_order_relaxed) != (uint32_t)-1) {
				const auto current_key_position = current_key->load(std::memory_order_relaxed);
				std::atomic<uint32_t>* next_key =
					(std::atomic<uint32_t>*)(index_array[current_key_position >> ct_log2(block)].load(std::memory_order_relaxed) + block)
					+ (current_key_position & (block - 1));
				if (current_key_position == location) {
					current_key->store(next_key->load(std::memory_order_relaxed), std::memory_order_release);
					break;
				}
				current_key = next_key;
			}
		}
	}

	new (&(local_index[block_index])) T(std::forward<P>(params) ...);
	keys[block_index].store((uint32_t)-2, std::memory_order_release);

	return local_index[block_index];
}

template<typename T, uint32_t block, uint32_t index_sz>
T& fixed_sz_deque<T, block, index_sz>::ensure_reserved(uint32_t location) {
	const auto block_num = (location) >> ct_log2(block);
	const auto block_index = (location) & (block - 1);

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


template<typename T, uint32_t block, uint32_t index_sz>
template<typename ...P>
tagged_object<T> fixed_sz_deque<T, block, index_sz>::emplace(P&& ... params) {
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
				return tagged_object<T>{this_spot, free_spot.parts.index};
			}
		}
	}
}