#pragma once
#include <stdint.h>
#include <atomic>
#include <string>
#include <type_traits>
#include "common\\common.h"

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
	concurrent_allocator() noexcept {}
	template <typename U>
	concurrent_allocator(const concurrent_allocator<U>&) noexcept {}
	T* allocate(size_t n);
	void deallocate(T* p, size_t n);
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
struct aligned_allocator_32 {
	using value_type = T;
	aligned_allocator_32() noexcept {}
	template <typename U>
	aligned_allocator_32(const aligned_allocator_32<U>&) noexcept {}
	T* allocate(size_t n);
	void deallocate(T* p, size_t n);
};

template <typename T, typename U, size_t alignment>
constexpr bool operator== (const aligned_allocator_32<T>&, const aligned_allocator_32<U>&) noexcept {
	return true;
}

template <class T, class U, size_t alignment>
constexpr bool operator!= (const aligned_allocator_32<T>&, const aligned_allocator_32<U>&) noexcept {
	return false;
}

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type>
class fixed_sz_deque_iterator;

template<typename T, uint32_t block, uint32_t index_sz, typename tag_type = uint32_t>
class fixed_sz_deque {
private:
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
	void flush(const F& f);
};

template<typename object_type, typename index_type, uint32_t block_size, uint32_t index_size>
class stable_vector {
private:
	object_type* index_array[index_size] = { nullptr };
public:
	stable_vector();
	~stable_vector();

	object_type& get(index_type i); // safe from any thread
	bool is_valid_index(index_type i); //safe (but potentially inaccurate) from any thread, but if true, can use get without possible memory error

	object_type& safe_get(index_type i); // single thread only
	object_type& get_new(); // single thread only
	void remove(index_type i); // single thread only
};

template<typename object_type, typename outer_index_type, typename inner_index_type, uint32_t block_size, uint32_t index_size>
class stable_2d_vector {
private:
	object_type* index_array[index_size] = { nullptr };
	uint32_t inner_size = 0;
public:
	stable_2d_vector();
	~stable_2d_vector();

	void reset(uint32_t inner_size); // single thread only
	void ensure_capacity(uint32_t outer_size); // single thread only
	void clear_row(outer_index_type i); // single thread only

	object_type* get_row(outer_index_type i); // safe from any thread
	object_type* safe_get_row(outer_index_type i); // single thread only
	object_type& get(outer_index_type i, inner_index_type j); // safe from any thread
	object_type& safe_get(outer_index_type i, inner_index_type j); // single thread only
	bool is_valid_index(outer_index_type i); //safe from any thread; if true, can use get without possible memory error
};

struct stable_variable_vector_tag {
	std::atomic<uint16_t> block_offset = 0ui16;
	std::atomic<uint16_t> size = 0ui16;
	std::atomic<uint16_t> capacity = 0ui16; //as item count
};

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
class stable_variable_vector_storage {
private:
	object_type* index_array[index_size] = { nullptr };
	bool block_reservations[index_size * contiguous_block_count] = { false };
public:
	stable_variable_vector_storage();
	~stable_variable_vector_storage();

	void increase_capacity(stable_variable_vector_tag& i, uint32_t new_capacity_in_blocks);
	void grow(stable_variable_vector_tag& i);
	void shrink_capacity(stable_variable_vector_tag& i);
	void release(stable_variable_vector_tag& i);
};

//general interface, safe from any thread
template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
std::pair<object_type*, object_type*> get_range(stable_variable_vector_tag& i, stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage);

//unsorted interface
template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void push_back(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj);

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_unordered_range(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type* first, object_type* last);

//sorted interface
template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj);

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_unique_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj);

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_ordered_range(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type* first, object_type* last);

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void add_unique_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj);

template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
void remove_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj);

//sorted interface safe from any thread
template<typename object_type, uint32_t block_size, uint32_t contiguous_block_count, uint32_t index_size>
bool contains_item(stable_variable_vector_storage<object_type, block_size, contiguous_block_count, index_size>& storage, stable_variable_vector_tag& i, object_type obj);


/*
concurrent_vector
concurrent_2d_vector
concurrent_variable_vector_tag
concurrent_variable_vector_storage
*/

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
