#include "common\\common.h"
#include "simple_serialize.hpp"
#include <Windows.h>


namespace serialization {
	serialize_file_wrapper::serialize_file_wrapper(std::u16string const& file_name) {
		file_handle = CreateFileW((wchar_t const*)(file_name.c_str()), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(file_handle == INVALID_HANDLE_VALUE) {
			file_handle = nullptr;
		} else {
			mapping_handle = CreateFileMapping(file_handle, nullptr, PAGE_READONLY, 0, 0, nullptr);
			if(mapping_handle) {
				mapped_bytes = MapViewOfFile(mapping_handle, FILE_MAP_READ, 0, 0, 0);
			}
		}
	}
	serialize_file_wrapper::serialize_file_wrapper(std::u16string const& file_name, size_t size) {
		file_handle = CreateFileW((wchar_t const*)(file_name.c_str()), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(file_handle == INVALID_HANDLE_VALUE) {
			file_handle = nullptr;
		} else {
			mapping_handle = CreateFileMapping(file_handle, nullptr, PAGE_READWRITE, DWORD(size >> 32ui64), DWORD(size), nullptr);
			if(mapping_handle) {
				mapped_bytes = MapViewOfFile(mapping_handle, FILE_MAP_WRITE, 0, 0, size);
			}
		}
	}
	serialize_file_wrapper::~serialize_file_wrapper() {
		if(mapped_bytes)
			UnmapViewOfFile(mapped_bytes);
		if(mapping_handle)
			CloseHandle(mapping_handle);
		if(file_handle)
			CloseHandle(file_handle);
	}
	std::byte* serialize_file_wrapper::get_bytes() const {
		return (std::byte*)mapped_bytes;
	}
	bool serialize_file_wrapper::file_valid() const {
		return (file_handle != nullptr) & (mapping_handle != nullptr);
	}
}
