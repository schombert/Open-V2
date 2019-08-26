#include "concurrency_tools.hpp"
#include <ppl.h>
#include <random>
#include <Windows.h>

#undef min
#undef max
#undef small

using namespace concurrency;

jsf_prng& get_local_generator() {
	static thread_local jsf_prng local_generator(std::random_device{}());
	return local_generator;
}

jsf_prng::jsf_prng(uint32_t seed) {
	a = 0xf1ea5eed;
	b = c = d = seed;

	for(size_t i = 0; i < 20; ++i)
		(*this)();
}


#define rot32(x,k) (((x)<<(k))|((x)>>(32-(k))))

uint32_t jsf_prng::operator()() {
	uint32_t e = a - rot32(b, 27);
	a = b ^ rot32(c, 17);
	b = c + d;
	c = d + e;
	d = e + a;
	return d;
}

#undef rot32

concurrent_string::concurrent_string() {
	_data.local_data[0] = 0;
	_data.local_data[internal_concurrent_string_size - 1] = internal_concurrent_string_size - 1;
}

concurrent_string::concurrent_string(const char* source) : concurrent_string(source, static_cast<uint32_t>(strlen(source))) {};

concurrent_string::concurrent_string(const char* start, const char* end) : concurrent_string(start, static_cast<uint32_t>(end - start)) {};

concurrent_string::concurrent_string(const char* start, uint32_t size) {
	if (size <= (internal_concurrent_string_size - 1)) {
		memcpy(_data.local_data, start, size * sizeof(char));
		_data.local_data[size] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = static_cast<char>((internal_concurrent_string_size - 1) - size);
	} else {
		_data.local_data[internal_concurrent_string_size - 1] = 127;
		_data.remote_data.data = (char*)concurrency::Alloc(size + 1);
		memcpy(_data.remote_data.data, start, size * sizeof(char));
		_data.remote_data.data[size] = 0;
		_data.remote_data.length = size;
	}
}

concurrent_string::~concurrent_string() {
	if (_data.local_data[internal_concurrent_string_size - 1] == 127) {
		concurrency::Free(_data.remote_data.data);
		_data.local_data[0] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = internal_concurrent_string_size - 1;
	}
}

void concurrent_string::clear() {
	if (_data.local_data[internal_concurrent_string_size - 1] == 127) {
		concurrency::Free(_data.remote_data.data);
	}
	_data.local_data[0] = 0;
	_data.local_data[internal_concurrent_string_size - 1] = internal_concurrent_string_size - 1;
}

char concurrent_string::operator[](uint32_t i) const {
	return c_str()[i];
}

concurrent_string::concurrent_string(const concurrent_string& o) : concurrent_string(o.c_str(), o.length()) {}

concurrent_string::concurrent_string(concurrent_string&& o) noexcept {
	if (o._data.local_data[internal_concurrent_string_size - 1] != 127) {
		memcpy(_data.local_data, o._data.local_data, internal_concurrent_string_size * sizeof(char));
	} else {
		_data.remote_data = o._data.remote_data;
		_data.local_data[internal_concurrent_string_size - 1] = 127;
		o._data.local_data[0] = 0;
		o._data.local_data[internal_concurrent_string_size - 1] = internal_concurrent_string_size - 1;
	}
}

bool concurrent_string::operator==(const char* o) const {
	return strncmp(o, c_str(), length() + 1) == 0;
}

bool concurrent_string::operator==(const concurrent_string& o) const {
	const auto this_length = length();
	if (this_length != o.length())
		return false;
	else
	return memcmp(c_str(), o.c_str(), this_length);
}

concurrent_string& concurrent_string::operator=(const concurrent_string& o) {
	this->~concurrent_string();
	new (this) concurrent_string(o);
	return *this;
}

concurrent_string& concurrent_string::operator=(concurrent_string&& o) noexcept {
	this->~concurrent_string();
	new (this) concurrent_string(std::move(o));
	return *this;
}

concurrent_string& concurrent_string::operator+=(const concurrent_string& o) {
	const auto this_len = length();
	const auto other_len = o.length();
	const auto total_len = this_len + other_len;
	if(total_len <= (internal_concurrent_string_size - 1)) {
		memcpy(_data.local_data + this_len, o._data.local_data, other_len);
		_data.local_data[total_len] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = static_cast<char>((internal_concurrent_string_size - 1) - (total_len));
	} else {
		auto new_data = (char*)concurrency::Alloc(total_len + 1);
		memcpy(new_data, c_str(), this_len);
		memcpy(new_data + this_len, o.c_str(), other_len);
		new_data[total_len] = 0;

		this->~concurrent_string();

		_data.local_data[internal_concurrent_string_size - 1] = 127;
		_data.remote_data.data = new_data;
		_data.remote_data.length = total_len;
	}
	return *this;
}

concurrent_string& concurrent_string::operator+=(const char* o) {
	const auto this_len = length();
	const auto other_len = strlen(o);
	const auto total_len = this_len + other_len;
	if(total_len <= (internal_concurrent_string_size - 1)) {
		memcpy(_data.local_data + this_len, o, other_len);
		_data.local_data[total_len] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = static_cast<char>((internal_concurrent_string_size - 1) - (total_len));
	} else {
		auto new_data = (char*)concurrency::Alloc(total_len + 1);
		memcpy(new_data, c_str(), this_len);
		memcpy(new_data + this_len, o, other_len);
		new_data[total_len] = 0;

		this->~concurrent_string();

		_data.local_data[internal_concurrent_string_size - 1] = 127;
		_data.remote_data.data = new_data;
		_data.remote_data.length = static_cast<uint32_t>(total_len);
	}
	return *this;
}

uint32_t concurrent_string::length() const {
	if(_data.local_data[internal_concurrent_string_size - 1] != 127) {
		return static_cast<uint32_t>(static_cast<int32_t>(internal_concurrent_string_size) - (_data.local_data[internal_concurrent_string_size - 1] + 1));
	} else {
		return _data.remote_data.length;
	}
}

const char* concurrent_string::c_str() const {
	if(_data.local_data[internal_concurrent_string_size - 1] != 127) {
		return _data.local_data;
	} else {
		return _data.remote_data.data;
	}
}

__declspec(restrict) void* concurrent_alloc_wrapper(size_t sz) {
	return concurrency::Alloc(sz);
}

void concurrent_free_wrapper(void* p) {
	concurrency::Free(p);
}

namespace ct {
	int32_t optimal_thread_count() {
		static const int32_t total = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
		return total;
	}
	int32_t count_threads_seen() {
		static std::atomic<int32_t> total_count = 0;
		return total_count.fetch_add(1, std::memory_order_acq_rel);
	}
	int32_t thread_pool_id() {
		static const thread_local int32_t id = count_threads_seen();
		return id;
	}
}
