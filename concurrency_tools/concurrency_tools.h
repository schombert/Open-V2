#pragma once
#include <stdint.h>
#include <atomic>
#include <string>
#include <type_traits>
#include "common\\common.h"

#undef min
#undef max

//source: Bob Jenkins
class jsf_prng {
private:
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
public:
	using result_type = uint32_t;

	jsf_prng(uint32_t seed);
	jsf_prng(jsf_prng const& o) noexcept : a(o.a), b(o.b), c(o.c), d(o.d) {}
	jsf_prng(jsf_prng&& o) noexcept : a(o.a), b(o.b), c(o.c), d(o.d) {}
	constexpr static uint32_t max() { return std::numeric_limits<uint32_t>::max(); }
	constexpr static uint32_t min() { return std::numeric_limits<uint32_t>::min(); }
	uint32_t operator()();
	template<uint32_t n>
	void advance_n() {
		for(uint32_t i = n; i--; )
			this->operator()();
	}
};

jsf_prng& get_local_generator();

constexpr uint32_t ct_log2(uint32_t n) {
	return ((n < 2) ? 0 : 1 + ct_log2(n / 2));
}

inline uint32_t rt_log2(uint32_t n) {
#ifdef __llvm__
	return 31ui32 - uint32_t(__builtin_clz(n | 1ui32));
#else
	return 31ui32 - uint32_t(__lzcnt(n | 1ui32));
#endif
}

constexpr uint32_t ct_log2_round_up(uint32_t n) {
	return ((1ui32 << ct_log2(n)) >= n) ? ct_log2(n) : ct_log2(n) + 1ui32;
}

inline uint32_t rt_log2_round_up(uint32_t n) {
#ifdef __llvm__
	return n > 1ui32 ? 32ui32 - uint32_t(__builtin_clz(n - 1ui32)) : 0ui32;
#else
	return n > 1ui32 ? 32ui32 - uint32_t(__lzcnt(n - 1ui32)) : 0ui32;
#endif
}

template<typename E1, typename E2>
class string_sum_expression;

struct string_expression_common_base {};
template<typename T>
struct string_expression;

template<typename T>
class string_expression_support {
public:
	template<typename O>
	auto operator+(const O& o) const {
		if constexpr(std::is_base_of_v<string_expression_common_base, T> && std::is_base_of_v<string_expression_common_base, O>)
			return string_sum_expression<T, O>(*static_cast<const T*>(this), o);
		else if constexpr(!std::is_base_of_v<string_expression_common_base, T> && std::is_base_of_v<string_expression_common_base, O>)
			return string_sum_expression<string_expression<T>, O>(string_expression<T>(*static_cast<const T*>(this)), o);
		else if constexpr(std::is_base_of_v<string_expression_common_base, T> && !std::is_base_of_v<string_expression_common_base, O>)
			return string_sum_expression<T, string_expression<const O>>(*static_cast<const T*>(this), string_expression<const O>(o));
		else if constexpr(!std::is_base_of_v<string_expression_common_base, T> && !std::is_base_of_v<string_expression_common_base, O>)
			return string_sum_expression<string_expression<T>, string_expression<const O>>(string_expression<T>(*static_cast<const T*>(this)), string_expression<const O>(o));
	}
	operator std::string() const;
};

template<typename T>
struct string_expression : public string_expression_support<string_expression<T>>, public string_expression_common_base {
private:
	const T& base;
public:
	string_expression(const T& b) : base(b) {}
	uint32_t length() const { return static_cast<uint32_t>(base.length()); }
	char operator[](uint32_t i) const { return base[i]; }
};

template<>
struct string_expression<const char*> : public string_expression_support<string_expression<const char*>>, public string_expression_common_base {
private:
	const char* base;
	const uint32_t len = 0;
public:
	string_expression(const char* b) : base(b), len(static_cast<uint32_t>(strlen(b))) {}
	uint32_t length() const { return len; }
	char operator[](uint32_t i) const { return base[i]; }
};

template<>
struct string_expression<char*> : public string_expression<const char*>  {};

template<size_t N>
struct string_expression<const char[N]> : public string_expression_support<string_expression<const char[N]>>, public string_expression_common_base {
private:
	const char* base;
public:
	string_expression(const char(&b)[N]) : base(b) {}
	uint32_t length() const { return N - 1; }
	char operator[](uint32_t i) const { return base[i]; }
};

template<size_t N>
struct string_expression<char[N]>  : public string_expression<const char[N]> {};

struct empty_string_expression : public string_expression_support<empty_string_expression>, public string_expression_common_base {
public:
	empty_string_expression() {}
	empty_string_expression(const string_expression<empty_string_expression>&) {}
	uint32_t length() const { return 0; }
	char operator[](uint32_t) const { return 0; }
};

template<typename E1, typename E2>
class string_sum_expression : public string_expression_support<string_sum_expression<E1, E2>>, public string_expression_common_base {
public:
	const E1 a;
	const E2 b;
	string_sum_expression(const E1& aa, const E2& bb) : a(aa), b(bb) {}
	uint32_t length() const {
		return a.length() + b.length();
	}
	char operator[](uint32_t i) const {
		const auto a_len = a.length();
		if (i < a_len)
			return a[i];
		else
			return b[i - a_len];
	}
};

template<typename E2>
class string_sum_expression<empty_string_expression, E2> :
	public string_expression_support<string_sum_expression<empty_string_expression, E2>>, public string_expression_common_base {
public:
	const E2 b;
	string_sum_expression(const empty_string_expression&, const E2& bb) : b(bb) {}
	uint32_t length() const {
		return b.length();
	}
	char operator[](uint32_t i) const {
		return b[i];
	}
};

class concurrent_string : public string_expression_support<concurrent_string> {
private:
	static constexpr uint32_t internal_concurrent_string_size = 16;

	union {
		char local_data[internal_concurrent_string_size];
		struct {
			char* data;
			uint32_t length;
		} remote_data;
	} _data;
public:
	concurrent_string();
	concurrent_string(const char* source);
	concurrent_string(const char* start, const char* end);
	concurrent_string(const char* start, uint32_t size);
	concurrent_string(const concurrent_string&);
	concurrent_string(concurrent_string&&) noexcept;
	template<typename T>
	concurrent_string(const string_expression<T>&);
	~concurrent_string();

	uint32_t length() const;
	const char* c_str() const;
	char operator[](uint32_t i) const;
	concurrent_string& operator=(const concurrent_string&);
	template<typename T>
	concurrent_string& operator=(const string_expression<T>&);
	concurrent_string& operator=(concurrent_string&&) noexcept;
	concurrent_string& operator+=(const concurrent_string&);
	concurrent_string& operator+=(const char*);
	template<typename T>
	concurrent_string& operator+=(const string_expression<T>&);
	bool operator==(const char*) const;
	bool operator==(const concurrent_string&) const;
	template<typename T>
	bool operator==(const string_expression<T>&) const;
	void clear();
};

template <typename T>
struct concurrent_allocator {
	using value_type = T;
	constexpr concurrent_allocator() noexcept {}
	template <typename U>
	constexpr concurrent_allocator(const concurrent_allocator<U>&) noexcept {}
	T* allocate(size_t n);
	void deallocate(T* p, size_t n);
};

template <typename T>
struct concurrent_aligned_allocator{
	using value_type = T;
	constexpr concurrent_aligned_allocator() noexcept {}
	template <typename U>
	constexpr concurrent_aligned_allocator(const concurrent_aligned_allocator<U>&) noexcept {}
	T* allocate(size_t n);
	void deallocate(T* p, size_t n);
};

template<typename T, typename index_type, bool padded>
struct concurrent_cache_aligned_buffer {
private:
	T* const allocated_address;
	T* const buffer;
	const uint32_t _size;
public:
	concurrent_cache_aligned_buffer(uint32_t size);
	concurrent_cache_aligned_buffer(uint32_t size, T initial_value);
	~concurrent_cache_aligned_buffer();

	concurrent_cache_aligned_buffer(concurrent_cache_aligned_buffer const&) = delete;
	concurrent_cache_aligned_buffer(concurrent_cache_aligned_buffer&&) = delete;
	concurrent_cache_aligned_buffer& operator=(concurrent_cache_aligned_buffer const&) = delete;
	concurrent_cache_aligned_buffer& operator=(concurrent_cache_aligned_buffer&&) = delete;

	T* data() const {
		return buffer;
	}
	T& operator[](index_type i) const {
		return buffer[to_index(i) + int32_t(padded)];
	}
	T& padding() const {
		return buffer[0];
	}
	operator bool() const noexcept {
		return allocated_address != nullptr;
	}

	operator tagged_array_view<T, index_type, padded>() const noexcept {
		return tagged_array_view<T, index_type, padded>(buffer, int32_t(_size));
	}
	tagged_array_view<T, index_type, padded> view() const noexcept {
		return tagged_array_view<T, index_type, padded>(buffer, int32_t(_size));
	}
	tagged_array_view<T, index_type, padded> view(int32_t size) const noexcept {
		assert(size + int32_t(padded) <= int32_t(_size));
		return tagged_array_view<T, index_type, padded>(buffer, size + int32_t(padded));
	}
};

template<typename T, typename index_type, bool padded>
struct moveable_concurrent_cache_aligned_buffer {
private:
	T* allocated_address = nullptr;
	T* buffer = nullptr;
	uint32_t _size;
public:
	moveable_concurrent_cache_aligned_buffer(uint32_t size);
	moveable_concurrent_cache_aligned_buffer(uint32_t size, T initial_value);
	~moveable_concurrent_cache_aligned_buffer();

	moveable_concurrent_cache_aligned_buffer(moveable_concurrent_cache_aligned_buffer const&) = delete;
	moveable_concurrent_cache_aligned_buffer(moveable_concurrent_cache_aligned_buffer&&);
	moveable_concurrent_cache_aligned_buffer& operator=(moveable_concurrent_cache_aligned_buffer const&) = delete;
	moveable_concurrent_cache_aligned_buffer& operator=(moveable_concurrent_cache_aligned_buffer&&);

	T* data() const {
		return buffer;
	}
	T& operator[](index_type i) const {
		return buffer[to_index(i) + int32_t(padded)];
	}
	T& padding() const {
		return buffer[0];
	}
	operator bool() const noexcept {
		return allocated_address != nullptr;
	}

	operator tagged_array_view<T, index_type, padded>() const noexcept {
		return tagged_array_view<T, index_type, padded>(buffer, int32_t(_size));
	}
	tagged_array_view<T, index_type, padded> view() const noexcept {
		return tagged_array_view<T, index_type, padded>(buffer, int32_t(_size));
	}
	tagged_array_view<T, index_type, padded> view(int32_t size) const noexcept {
		assert(size + int32_t(padded) <= int32_t(_size));
		return tagged_array_view<T, index_type, padded>(buffer, size + int32_t(padded));
	}
};

template <typename T, typename U>
constexpr bool operator== (const concurrent_allocator<T>&, const concurrent_allocator<U>&) noexcept {
	return true;
}

template <class T, class U>
constexpr bool operator!= (const concurrent_allocator<T>&, const concurrent_allocator<U>&) noexcept {
	return false;
}

template <typename T>
struct aligned_allocator_64 {
	using value_type = T;
	aligned_allocator_64() noexcept {}
	template <typename U>
	aligned_allocator_64(const aligned_allocator_64<U>&) noexcept {}
	T* allocate(size_t n);
	void deallocate(T* p, size_t n);
	template<typename U>
	constexpr bool operator==(aligned_allocator_64<U> const&) const { return true; }
	template<typename U>
	constexpr bool operator!=(aligned_allocator_64<U> const&) const { return false; }
};

template<typename T>
using aligned_allocator = aligned_allocator_64<T>;
template<typename T>
using aligned_allocator_32 = aligned_allocator_64<T>;

template <typename T, typename U, size_t alignment>
constexpr bool operator== (const aligned_allocator_64<T>&, const aligned_allocator_64<U>&) noexcept {
	return true;
}

template <class T, class U, size_t alignment>
constexpr bool operator!= (const aligned_allocator_64<T>&, const aligned_allocator_64<U>&) noexcept {
	return false;
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
class fixed_sz_deque_iterator;

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type = uint32_t>
class fixed_sz_deque {
private:
	static_assert(1ui64 << ct_log2(block) == block);
	constexpr static uint32_t bit_shift = ct_log2(block);

	std::atomic<T*> index_array[index_sz] = { nullptr };
	std::atomic<uint64_t> first_free = 0;
	std::atomic<uint32_t> first_free_index = 1;

	void create_new_block();
public:
	fixed_sz_deque();
	~fixed_sz_deque();

	T& at(tag_type index) const;
	T* safe_at(tag_type index) const;
	void free(tag_type index);
	template<typename U>
	void free(tag_type index, U&);
	uint32_t past_end() const;

	template<typename ...P>
	tagged_object<T, tag_type> emplace(P&& ... params);
	template<typename ...P>
	T& emplace_at(tag_type location, P&& ... params); // not thread safe
	T& ensure_reserved(tag_type location);
	template<typename F>
	void visit(tag_type location, const F& f) const;

	fixed_sz_deque_iterator<T, block, index_sz, tag_type> begin() const;
	fixed_sz_deque_iterator<T, block, index_sz, tag_type> end() const;
};

template<typename T, uint32_t block, uint32_t index_sz>
class fixed_sz_list {
private:
	static_assert(1ui64 << ct_log2(block) == block);
	constexpr static uint32_t bit_shift = ct_log2(block);

	std::atomic<T*> index_array[index_sz] = { nullptr };
	std::atomic<uint64_t> first_free = 0;
	std::atomic<uint64_t> first_in_list = 0;
	std::atomic<uint32_t> first_free_index = 1;

	void create_new_block();
public:
	fixed_sz_list();
	~fixed_sz_list();

	template<typename ...P>
	void emplace(P&& ... params);
	template<typename F>
	void try_pop(const F& f);
	template<typename F>
	bool flush(const F& f);
};

template<typename T>
constexpr value_base_of<T> high_bit_mask = value_base_of<T>(1ui64 << (sizeof(value_base_of<T>) * 8ui64 - 1ui64));

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
class stable_vector {
public:
	constexpr static uint32_t bit_shift = ct_log2(block_size);

	static_assert(1ui64 << ct_log2(block_size) == block_size);

	constexpr static uint32_t block_size_v = block_size;

	object_type* index_array[index_size] = { nullptr };
	uint32_t indices_in_use = 0ui32;
	index_type first_free = index_type(static_cast<value_base_of<index_type>>(to_index(index_type()) | high_bit_mask<index_type>));

	stable_vector();
	~stable_vector();

	static index_type get_id(object_type const& o) {
		if((to_index(o.id) & high_bit_mask<index_type>) != 0)
			return null_value_of<index_type>;
		return o.id;
	}
	static index_type get_id(index_type i) {
		if((to_index(i) & high_bit_mask<index_type>) != 0)
			return null_value_of<index_type>;
		return i;
	}

	uint32_t size_upper_bound() const {
		return block_size * indices_in_use;
	}

	object_type& get(index_type i) const noexcept; // safe from any thread
	object_type& untyped_get(uint32_t i) const noexcept; // safe from any thread
	object_type& operator[](index_type i) const noexcept; // safe from any thread
	bool is_valid_index(index_type i) const; //safe (but potentially inaccurate) from any thread, but if true, can use get without possible memory error

	object_type& get_new(); // single thread only
	void remove(index_type i); // single thread only
	object_type* get_location(index_type i); // single thread only, forces storage to expand
	uint32_t minimum_continuous_size() const;

	template<typename T>
	void for_each(T&& f) const;
	template<typename T, typename P = concurrency::auto_partitioner>
	void parallel_for_each(T const& f, P&& p = concurrency::auto_partitioner());
};


template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
class stable_2d_vector {
public:
	constexpr static uint32_t bit_shift = ct_log2(block_size);

	static_assert(1ui64 << ct_log2(block_size) == block_size);

	object_type* index_array[index_size] = { nullptr };
	uint32_t inner_size = 0ui32;
	uint32_t indices_in_use = 0ui32;

	stable_2d_vector();
	~stable_2d_vector();

	void reset(uint32_t inner_size); // single thread only
	void ensure_capacity(uint32_t outer_size); // single thread only
	void clear_row(outer_index_type i); // single thread only
	void clear_all(); // single thread only


	tagged_array_view<const object_type, inner_index_type, false> get_row(outer_index_type i) const; // safe from any thread
	tagged_array_view<object_type, inner_index_type, false> get_row(outer_index_type i); // safe from any thread
	tagged_array_view<object_type, inner_index_type, false> safe_get_row(outer_index_type i); // single thread only
	object_type& get(outer_index_type i, inner_index_type j) const; // safe from any thread
	object_type& safe_get(outer_index_type i, inner_index_type j); // single thread only
	bool is_valid_index(outer_index_type i) const; //safe from any thread; if true, can use get without possible memory error
};

using stable_mk_2_tag = uint32_t;

union concurrent_key_pair_helper {
	struct {
		uint32_t index;
		uint32_t counter;
	} parts;
	uint64_t value;

	constexpr explicit concurrent_key_pair_helper(uint32_t i, uint32_t c) : parts{ i, c } {}
	constexpr explicit concurrent_key_pair_helper(uint64_t v) : value{ v } {}
};

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned = false>
class stable_variable_vector_storage_mk_2 {
public:
	uint64_t* backing_storage = nullptr;
	std::atomic<uint32_t> first_free = 0ui32;

	std::atomic<uint64_t> free_lists[17] = {
		concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value,
		concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value,
		concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value,
		concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value, concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value,
		concurrent_key_pair_helper(null_value_of<stable_mk_2_tag>, 0).value };

	stable_variable_vector_storage_mk_2();
	~stable_variable_vector_storage_mk_2();

	static_assert(!is_aligned || minimum_size * sizeof(object_type) >= 64);

	void reset();
	stable_mk_2_tag make_new(uint32_t capacity);
	void increase_capacity(stable_mk_2_tag& i, uint32_t new_capacity);
	void shrink_capacity(stable_mk_2_tag& i);
	void release(stable_mk_2_tag& i);
};

//general interface, safe from any thread
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i, uint32_t inner_index); // safe to read from any thread

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i);

//unsorted interface
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void push_back(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type obj);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void pop_back(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_unordered_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type const* first, object_type const* last);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_unsorted_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj);

//sorted interface
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type obj);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_unique_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type obj);

//range passed must already be sorted
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_ordered_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type const* first, object_type const* last);

//range passed must already be sorted and contain no duplicates
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_unique_ordered_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag& i, object_type const* first, object_type const* last);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_sorted_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_multisorted_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj);

template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_subitem_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, stable_mk_2_tag i, object_type obj);

//sorted interface safe from any thread
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, stable_mk_2_tag i, object_type obj);


template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
std::pair<object_type*, object_type*> get_subrange(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type, index_type, padding> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
tagged_array_view<const object_type, index_type, padding> get_view(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type, index_type, padding> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
tagged_array_view<object_type, index_type, padding> get_view(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i, uint32_t inner_index);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, uint32_t inner_index);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
object_type& get(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type, index_type, padding> i, index_type inner_index);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
uint32_t get_capacity(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type, index_type, padding> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
uint32_t get_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type, index_type, padding> i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
void remove_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_single_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void remove_subrange(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding, typename FUNC>
void remove_item_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding>& i, FUNC const& f);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename FUNC>
void remove_item_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, FUNC const& f);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename FUNC>
void remove_item_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, FUNC const& f);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename FUNC>
void remove_subitem_if(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type key, FUNC const& f);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type, index_type, padding> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_item(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
bool contains_subitem(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
void add_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding>& i, object_type const* first, object_type const* last);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, object_type const* first, object_type const* last);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void add_range(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, object_type const* first, object_type const* last);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
void clear(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding>& i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void clear(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void clear(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
void resize(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding>& i, uint32_t new_size);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void resize(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i, uint32_t new_size);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void resize(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i, uint32_t new_size);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
void shrink(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding>& i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void shrink(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, set_tag<object_type>& i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
void shrink(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, multiset_tag<object_type>& i);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
object_type const* find(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, array_tag<object_type, index_type, padding> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename index_type, bool padding>
object_type* find(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, array_tag<object_type, index_type, padding> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type* find(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, set_tag<object_type> i, object_type obj);
template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned>
object_type* find(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, multiset_tag<object_type> i, object_type obj);

namespace serialization {
	template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename type_tag>
	void serialize_stable_array(std::byte* &output, stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, type_tag i);
	template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename type_tag>
	void deserialize_stable_array(std::byte const* &input, stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned>& storage, type_tag& i);
	template<typename object_type, uint32_t minimum_size, size_t memory_size, bool is_aligned, typename type_tag>
	size_t serialize_stable_array_size(stable_variable_vector_storage_mk_2<object_type, minimum_size, memory_size, is_aligned> const& storage, type_tag i);
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
class fixed_sz_deque_iterator {
private:
	const fixed_sz_deque<T, block, index_sz, tag_type>* parent;
	int32_t position;
public:
	fixed_sz_deque_iterator() : parent(nullptr), position(0) {}
	fixed_sz_deque_iterator(const fixed_sz_deque<T, block, index_sz, tag_type>& p) : parent(&p), position(0) {}
	fixed_sz_deque_iterator(const fixed_sz_deque<T, block, index_sz, tag_type>& p, int32_t o) : parent(&p), position(o) {}

	bool operator==(const fixed_sz_deque_iterator& o) const {
		return position == o.position;
	}
	bool operator!=(const fixed_sz_deque_iterator& o) const {
		return position != o.position;
	}
	T* operator*() const {
		return parent->safe_at(tag_type( static_cast<value_base_of<tag_type>>(position)));
	}
	T* operator[](int32_t offset) const {
		return parent->safe_at(tag_type(static_cast<value_base_of<tag_type>>(position + offset)));
	}
	T* operator->() const {
		return parent->safe_at(tag_type(static_cast<value_base_of<tag_type>>(position)));
	}
	fixed_sz_deque_iterator& operator++() {
		++position;
		return *this;
	}
	fixed_sz_deque_iterator& operator--() {
		++position;
		return *this;
	}
	fixed_sz_deque_iterator operator++(int) {
		++position;
		return fixed_sz_deque_iterator(parent, position - 1);
	}
	fixed_sz_deque_iterator operator--(int) {
		++position;
		return fixed_sz_deque_iterator(parent, position + 1);
	}
	fixed_sz_deque_iterator& operator+=(int32_t v) {
		position += v;
		return *this;
	}
	fixed_sz_deque_iterator& operator-=(int32_t v) {
		position -= v;
		return *this;
	}
	fixed_sz_deque_iterator operator+(int32_t v) {
		return fixed_sz_deque_iterator(parent, position + v);
	}
	fixed_sz_deque_iterator operator-(int32_t v) {
		return fixed_sz_deque_iterator(parent, position - v);
	}
	int32_t operator-(const fixed_sz_deque_iterator& o) {
		return position - o.position;
	}
	bool operator<(const fixed_sz_deque_iterator& o) const {
		return position < o.position;
	}
	bool operator<=(const fixed_sz_deque_iterator& o) const {
		return position <= o.position;
	}
	bool operator>(const fixed_sz_deque_iterator& o) const {
		return position > o.position;
	}
	bool operator>=(const fixed_sz_deque_iterator& o) const {
		return position >= o.position;
	}
};

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
class std::iterator_traits<fixed_sz_deque_iterator<T, block, index_sz, tag_type>> {
public:
	using difference_type = int32_t;
	using value_type = T;
	using pointer = T*;
	using reference = T*&;
	using iterator_category = std::random_access_iterator_tag;
};

//based on: https://github.com/WojciechMula/toys/tree/master/simd-min-index

int32_t minimum_index(int32_t const* data, int32_t size);
int32_t maximum_index(int32_t const* data, int32_t size);
int32_t minimum_index(int64_t const* data, int32_t size);
int32_t maximum_index(int64_t const* data, int32_t size);
int32_t minimum_index(float const* data, int32_t size);
int32_t maximum_index(float const* data, int32_t size);
int32_t minimum_index(double const* data, int32_t size);
int32_t maximum_index(double const* data, int32_t size);
