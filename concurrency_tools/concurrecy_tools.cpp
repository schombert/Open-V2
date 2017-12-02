#include "concurrency_tools.h"
#include <ppl.h>

using namespace concurrency;

concurrent_string::concurrent_string() {
	_data.local_data[0] = 0;
	_data.local_data[internal_concurrent_string_size - 1] = internal_concurrent_string_size - 1;
}
concurrent_string::concurrent_string(const char* source) : concurrent_string(source, strlen(source)) {};

concurrent_string::concurrent_string(const char* start, const char* end) : concurrent_string(start, end - start) {};

concurrent_string::concurrent_string(const char* start, uint32_t size) {
	if (size <= (internal_concurrent_string_size - 1)) {
		memcpy(_data.local_data, start, size * sizeof(char));
		_data.local_data[size] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = (internal_concurrent_string_size - 1) - size;
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
	if (total_len <= (internal_concurrent_string_size - 1)) {
		memcpy(_data.local_data + this_len, o._data.local_data, other_len);
		_data.local_data[total_len] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = (internal_concurrent_string_size - 1) - (total_len);
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
	if (total_len <= (internal_concurrent_string_size - 1)) {
		memcpy(_data.local_data + this_len, o, other_len);
		_data.local_data[total_len] = 0;
		_data.local_data[internal_concurrent_string_size - 1] = (internal_concurrent_string_size - 1) - (total_len);
	} else {
		auto new_data = (char*)concurrency::Alloc(total_len + 1);
		memcpy(new_data, c_str(), this_len);
		memcpy(new_data + this_len, o, other_len);
		new_data[total_len] = 0;

		this->~concurrent_string();

		_data.local_data[internal_concurrent_string_size - 1] = 127;
		_data.remote_data.data = new_data;
		_data.remote_data.length = total_len;
	}
	return *this;
}

uint32_t concurrent_string::length() const {
	if (_data.local_data[internal_concurrent_string_size - 1] != 127) {
		return internal_concurrent_string_size - (_data.local_data[internal_concurrent_string_size - 1] + 1);
	} else {
		return _data.remote_data.length;
	}
}

const char* concurrent_string::c_str() const {
	if (_data.local_data[internal_concurrent_string_size - 1] != 127) {
		return _data.local_data;
	} else {
		return _data.remote_data.data;
	}
}