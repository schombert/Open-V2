#include "common\\common.h"
#include "simple_serialize.hpp"
#include <Windows.h>
#include "zlib.h"

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

		if(final_size != 0) {
			_LARGE_INTEGER pvalue;
			pvalue.QuadPart = int64_t(final_size);
			SetFilePointerEx(file_handle, pvalue, nullptr, FILE_BEGIN);
			SetEndOfFile(file_handle);
		}

		if(file_handle)
			CloseHandle(file_handle);
	}
	std::byte* serialize_file_wrapper::get_bytes() const {
		return (std::byte*)mapped_bytes;
	}
	bool serialize_file_wrapper::file_valid() const {
		return (file_handle != nullptr) & (mapping_handle != nullptr);
	}

	uint64_t serialize_file_wrapper::get_size() const {
		_LARGE_INTEGER pvalue;
		GetFileSizeEx(file_handle, &pvalue);
		return uint64_t(pvalue.QuadPart);
	}

	uint64_t impl_get_compressed_upper_bound(uint64_t source_size) {
		return compressBound(uLong(source_size));
	}
	uint64_t impl_compress(uint64_t source_size, std::byte* source, std::byte* dest) {
		uLongf size_in_out = compressBound(uLong(source_size));
		compress((unsigned char*)dest, &size_in_out, (unsigned char*)source, uLong(source_size));
		return size_in_out;
	}
	void impl_decompress(uint64_t source_size, std::byte const* source, uint64_t dest_size, std::byte* dest) {
		uLongf size_in_out = uLong(dest_size);
		uncompress((unsigned char*)dest, &size_in_out, (unsigned char const*)source, uLong(source_size));
	}
}
