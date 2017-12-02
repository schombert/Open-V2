#pragma once
#include <stdint.h>

class concurrent_string {
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
	~concurrent_string();

	concurrent_string& operator=(const concurrent_string&);
	concurrent_string& operator=(concurrent_string&&) noexcept;
	concurrent_string& operator+=(const concurrent_string&);
	concurrent_string& operator+=(const char*);
	bool operator==(const char*) const;
	bool operator==(const concurrent_string&) const;
	uint32_t length() const;
	const char* c_str() const;
	void clear();
};

class concurrent_u16string {
private:
	static constexpr uint32_t internal_concurrent_string_size = 16;

	union {
		char16_t local_data[internal_concurrent_string_size];
		struct {
			char16_t* data;
			uint32_t length;
		} remote_data;
	} _data;
public:
	concurrent_u16string();
	concurrent_u16string(const char16_t* source);
	concurrent_u16string(const char16_t* start, const char16_t* end);
	concurrent_u16string(const char16_t* start, uint32_t size);
	concurrent_u16string(const concurrent_u16string&);
	concurrent_u16string(concurrent_u16string&&) noexcept;
	~concurrent_u16string();

	concurrent_u16string& operator=(const concurrent_u16string&);
	concurrent_u16string& operator=(concurrent_u16string&&) noexcept;
	concurrent_u16string& operator+=(const concurrent_u16string&);
	concurrent_u16string& operator+=(const char16_t*);
	bool operator==(const char*) const;
	bool operator==(const concurrent_u16string&) const;
	uint32_t length() const;
	const char16_t* c_str() const;
	void clear();
};