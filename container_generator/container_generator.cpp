#include "pch.h"
#include <Windows.h>
#include <string>
#include <vector>

std::string extract_string(char const* & input, char const* end) {
	while(input < end && (*input == ' ' || *input == ',' || *input == '\t' || *input == '\r' || *input == '\n'))
		++input;

	if(input == end)
		return std::string();

	std::string result;
	
	while(input < end && (*input != ' ' && *input != '\t' && *input != ',' && *input != '\r' && *input == '\n')) {
		result += *input;
		++input;
	}

	return result;
}

struct key_and_type {
	std::string key;
	std::string type;
};

int wmain(int argc, wchar_t *argv[]) {

	std::string namespace_name;
	bool is_sparse = false;
	std::string index_type;
	std::string container_size;
	std::vector<key_and_type> keys_and_types;

	const auto handle = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(handle != INVALID_HANDLE_VALUE) {
		LARGE_INTEGER file_size;
		GetFileSizeEx(handle, &file_size);
		auto const sz = (size_t)(file_size.QuadPart);

		char* buffer = new char[sz];
		ReadFile(handle, buffer, (DWORD)sz, nullptr, nullptr);

		char const* input = buffer;

		namespace_name = extract_string(input, buffer + sz);
		is_sparse = (extract_string(input, buffer + sz) == "sparse");
		index_type = extract_string(input, buffer + sz);
		container_size = extract_string(input, buffer + sz);
		
		std::string next_key = extract_string(input, buffer + sz);
		while(next_key.length() > 0) {
			keys_and_types.push_back(key_and_type{ next_key, extract_string(input, buffer + sz) });
			next_key = extract_string(input, buffer + sz);
		}

		delete[] buffer;

		CloseHandle(handle);
	}


	std::string output;
	output += "#include \"common\\common.h\"\r\n";
	output += "#include \"common\\shared_tags.h\"\r\n";
	output += "\r\n";
	output += "namespace ";
	output += namespace_name;
	output += " {\r\n";

	for(auto& kp : keys_and_types) {
		output += "\tstruct ";
		output += kp.key;
		output += ";\r\n";
	}

	output += "\r\n";
	output += "\tclass container {\r\n"; // BEGIN: container class

	//member variables
	// todo: index row
	for(int32_t i = 0; i < int32_t(keys_and_types.size()); ++i) {
		if(keys_and_types[i].type == "bitfield" || keys_and_types[i].type == "bitfield_type") {
			std::string bytes_size = std::string("((uint32_t(") + container_size + " + 7)) / 8ui32 + 63ui32) & ~63ui32";
			output += "\t\t struct alignas(64) dtype_" + std::to_string(i) + " { \r\n"
				"\t\t\t bitfield_type padding[64]; \r\n"
				"\t\t\t bitfield_type values[" + bytes_size + "]; \r\n"
				"\t\t\t dtype_" + std::to_string(i) + "() { std::fill_n(values - 1, 1 + " + bytes_size + ", bitfield_type{ 0ui8 }); } "
				"\t\t } m_" + std::to_string(i) + ";\r\n";
		} else {
			std::string member_count = std::string("(sizeof(") + keys_and_types[i].type + ") <= 64 ? ("
				"uint32_t(" + container_size + ") + (64ui32 / uint32_t(sizeof(" + keys_and_types[i].type + "))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(" + keys_and_types[i].type + ")) - 1ui32"
				") : uint32_t(" + container_size + "))";
			output += "\t\t struct alignas(64) dtype_" + std::to_string(i) + " { \r\n"
				"\t\t\t uint8_t padding[(sizeof(" + keys_and_types[i].type + ") + 63ui32) & ~63ui32]; \r\n"
				"\t\t\t " + keys_and_types[i].type + " values[" + member_count + "]; \r\n"
				"\t\t\t dtype_" + std::to_string(i) + "() { std::uninitialized_value_construct_n(values - 1, " + member_count + " + 1); } "
				"\t\t } m_" + std::to_string(i) + ";\r\n";
		}
	}

	// get
	// const get
	// set
	// get_row
	// const get_row

	// get_new
	// thread_safe_get_new
	// reset
	// size
	// vector_size
	// is_valid_index
	// for_each
	// parallel_for_each

	// serialize

	output += "\t};\r\n"; // END: container class

	output += "}\r\n";
	output += "\r\n";
	return 0;
}
