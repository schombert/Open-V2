#include "pch.h"
#include <Windows.h>
#include <string>
#include <vector>

std::string extract_string(char const* & input, char const* end) {
	while(input < end && (*input == ' ' || *input == ',' || *input == '\t' || *input == '\r' || *input == '\n'))
		++input;

	if(input >= end)
		return std::string();

	std::string result;
	bool in_quote = false;

	while(input < end && (in_quote || (*input != ' ' && *input != '\t' && *input != ',' && *input != '\r' && *input != '\n'))) {
		if(*input == '\"')
			in_quote = !in_quote;
		else
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

	for(int32_t i = 1; i < argc; ++i) {

		if(lstrlen(argv[i]) <= 0)
			continue;

		std::string namespace_name;
		bool is_sparse = false;
		std::string index_type;
		std::string container_size;
		std::vector<key_and_type> keys_and_types;

		const auto handle = CreateFile(argv[i], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(handle != INVALID_HANDLE_VALUE) {

			LARGE_INTEGER file_size;
			GetFileSizeEx(handle, &file_size);
			auto const sz = (size_t)(file_size.QuadPart);

			char* buffer = new char[sz];
			DWORD read;
			ReadFile(handle, buffer, (DWORD)sz, &read, nullptr);

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
		} else {
			MessageBox(NULL, argv[i], L"File not found", MB_OK);
		}


		std::string output;
		output += "#include \"common\\\\common.h\"\r\n";
		output += "#include \"common\\\\shared_tags.h\"\r\n";
		output += "#include \"concurrency_tools\\\\ve.h\"\r\n";
		output += "#include \"simple_serialize\\\\simple_serialize.hpp\"\r\n";
		output += "\r\n";

		output += "#pragma warning( push )\r\n";
		output += "#pragma warning( disable : 4324 )\r\n";
		output += "\r\n";

		output += "namespace ";
		output += namespace_name;
		output += " {\r\n";
		output += "\tclass container;\r\n";
		output += "}\r\n";
		output += "\r\n";


		output += "template<>\r\n";
		output += "class serialization::serializer<" + namespace_name + "::container>;\r\n";
		output += "\r\n";

		output += "namespace ";
		output += namespace_name;
		output += " {\r\n";

		//key defs
		for(auto& kp : keys_and_types) {
			output += "\tstruct ";
			output += kp.key;
			kp.key = namespace_name + "::" + kp.key;
			output += ";\r\n";
		}
		output += "\r\n";

		output += "\tclass alignas(64) container {\r\n"; // BEGIN: container class

		// commom members
		output += "\t\t int32_t size_used = 0;\r\n";
		if(is_sparse) {
			output += std::string("\t\t ") + index_type + " first_free;";
		}
		output += "\r\n";

		// member variables
		if(is_sparse) {
			std::string member_count = std::string("(sizeof(") + index_type + ") <= 64 ? ("
				"uint32_t(" + container_size + ") + (64ui32 / uint32_t(sizeof(" + index_type + "))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(" + index_type + ")) - 1ui32"
				") : uint32_t(" + container_size + "))";
			output += "\t\t struct alignas(64) dtype_index { \r\n"
				"\t\t\t uint8_t padding[(sizeof(" + index_type + ") + 63ui32) & ~63ui32]; \r\n"
				"\t\t\t " + index_type + " values[" + member_count + "]; \r\n"
				"\t\t\t dtype_index() { std::uninitialized_value_construct_n(values - 1, " + member_count + " + 1); } "
				"\t\t } m_index;\r\n";
		}
		output += "\r\n";

		for(int32_t i = 0; i < int32_t(keys_and_types.size()); ++i) {
			if(keys_and_types[i].type == "bitfield" || keys_and_types[i].type == "bitfield_type") {
				std::string bytes_size = std::string("((uint32_t(") + container_size + " + 7)) / 8ui32 + 63ui32) & ~63ui32";
				output += "\t\t struct alignas(64) dtype_" + std::to_string(i) + " { \r\n"
					"\t\t\t bitfield_type padding[64]; \r\n"
					"\t\t\t bitfield_type values[" + bytes_size + "]; \r\n"
					"\t\t\t dtype_" + std::to_string(i) + "() { std::fill_n(values - 1, 1 + " + bytes_size + ", bitfield_type{ 0ui8 }); }\r\n"
					"\t\t } m_" + std::to_string(i) + ";\r\n";
			} else {
				std::string member_count = std::string("(sizeof(") + keys_and_types[i].type + ") <= 64 ? ("
					"uint32_t(" + container_size + ") + (64ui32 / uint32_t(sizeof(" + keys_and_types[i].type + "))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(" + keys_and_types[i].type + ")) - 1ui32"
					") : uint32_t(" + container_size + "))";
				output += "\t\t struct alignas(64) dtype_" + std::to_string(i) + " { \r\n"
					"\t\t\t uint8_t padding[(sizeof(" + keys_and_types[i].type + ") + 63ui32) & ~63ui32]; \r\n"
					"\t\t\t " + keys_and_types[i].type + " values[" + member_count + "]; \r\n"
					"\t\t\t dtype_" + std::to_string(i) + "() { std::uninitialized_value_construct_n(values - 1, " + member_count + " + 1); }\r\n"
					"\t\t } m_" + std::to_string(i) + ";\r\n";
			}
		}
		output += "\r\n";
		output += "\t\t public:\r\n";
		output += "\t\t friend class serialization::serializer<container>;\r\n";
		// constructor
		if(is_sparse) {
			output += "\t\t container() {\r\n";
			output += "\t\t\t for(int32_t i = " + container_size + " - 1; i >= 0; --i) {\r\n";
			output += "\t\t\t\t m_index.values[i] = first_free;\r\n";
			output += std::string("\t\t\t\t first_free = ") + index_type + "(" + index_type + "::value_base_t(i));\r\n";
			output += "\t\t\t }\r\n";
			output += "\t\t }\r\n";
			output += "\r\n";
		} else {

		}

		// tagged member functions
		for(int32_t i = 0; i < int32_t(keys_and_types.size()); ++i) {
			if(keys_and_types[i].type == "bitfield" || keys_and_types[i].type == "bitfield_type") {
				// get
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">, bool> get(" + index_type + " i) const {\r\n"
					"\t\t\t return bit_vector_test(m_" + std::to_string(i) + ".values, to_index(i));\r\n"
					"\t\t }\r\n";
				// set
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t auto set(" + index_type + " i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">> {\r\n"
					"\t\t\t bit_vector_set(m_" + std::to_string(i) + ".values, to_index(i), v);\r\n"
					"\t\t }\r\n";
				// get row
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">, tagged_array_view<bitfield_type, " + index_type + ">> get_row() {\r\n"
					"\t\t\t return tagged_array_view<bitfield_type, " + index_type + ">(m_" + std::to_string(i) + ".values, int32_t(uint32_t(size_used + 7) / 8ui32));\r\n"
					"\t\t }\r\n";
				// const get row
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">, tagged_array_view<bitfield_type const, " + index_type + ">> get_row() const {\r\n"
					"\t\t\t return tagged_array_view<bitfield_type const, " + index_type + ">(m_" + std::to_string(i) + ".values, int32_t(uint32_t(size_used + 7) / 8ui32));\r\n"
					"\t\t }\r\n";
			} else {
				// get
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">, " + keys_and_types[i].type + "&> get(" + index_type + " i) {\r\n"
					"\t\t\t return m_" + std::to_string(i) + ".values[to_index(i)];\r\n"
					"\t\t }\r\n";
				// const get
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">, " + keys_and_types[i].type + " const&> get(" + index_type + " i) const {\r\n"
					"\t\t\t return m_" + std::to_string(i) + ".values[to_index(i)];\r\n"
					"\t\t }\r\n";
				// set
				output += std::string("\t\t template<typename INDEX, typename value_type>\r\n") +
					"\t\t auto set(" + index_type + " i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, " + keys_and_types[i].key + ">> {\r\n"
					"\t\t\t m_" + std::to_string(i) + ".values[to_index(i)] = v;\r\n"
					"\t\t }\r\n";
				output += std::string("\t\t template<typename INDEX, typename value_type>\r\n") +
					"\t\t auto set(" + index_type + " i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, " + keys_and_types[i].key + ">> {\r\n"
					"\t\t\t m_" + std::to_string(i) + ".values[to_index(i)] = v;\r\n"
					"\t\t }\r\n";
				// get row
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">, tagged_array_view<" + keys_and_types[i].type + ", " + index_type + ">> get_row() {\r\n"
					"\t\t\t return tagged_array_view<" + keys_and_types[i].type + ", " + index_type + ">(m_" + std::to_string(i) + ".values, ve::to_vector_size(uint32_t(size_used)));\r\n"
					"\t\t }\r\n";
				// const get row
				output += std::string("\t\t template<typename INDEX>\r\n") +
					"\t\t std::enable_if_t<std::is_same_v<INDEX, " + keys_and_types[i].key + ">, tagged_array_view<" + keys_and_types[i].type + " const, " + index_type + ">> get_row() const {\r\n"
					"\t\t\t return tagged_array_view<" + keys_and_types[i].type + " const, " + index_type + ">(m_" + std::to_string(i) + ".values, ve::to_vector_size(uint32_t(size_used)));\r\n"
					"\t\t }\r\n";
			}
		}

		output += "\r\n";


		if(is_sparse) {
			// get_new

			output += std::string("\t\t ") + index_type + " get_new() {\r\n";
			output += "#ifdef _DEBUG\r\n";
			output += "\t\t\t if(!::is_valid_index(first_free))\r\n";
			output += "\t\t\t\t std::abort();\r\n";
			output += "#endif\r\n";
			output += "\t\t\t auto allocated = first_free;\r\n";
			output += "\t\t\t first_free = m_index.values[to_index(first_free)];\r\n";
			output += "\t\t\t m_index.values[to_index(allocated)] = allocated;\r\n";
			output += "\t\t\t if(int32_t(to_index(allocated) + 1) > size_used)\r\n";
			output += "\t\t\t\t size_used = int32_t(to_index(allocated) + 1);\r\n";
			output += "\t\t\t return allocated;\r\n";
			output += "\t\t }\r\n";

			// resize
			output += "\t\t void resize(int32_t) {}\r\n";

			// release
			output += std::string("\t\t void release(") + index_type + " i) {\r\n";
			for(int32_t i = 0; i < int32_t(keys_and_types.size()); ++i) {
				if(keys_and_types[i].type == "bitfield" || keys_and_types[i].type == "bitfield_type") {
					output += "\t\t\t set<" + keys_and_types[i].key + ">(i, false);\r\n";
				} else {
					output += "\t\t\t set<" + keys_and_types[i].key + ">(i, " + keys_and_types[i].type + "());\r\n";
				}
			}
			output += "\t\t\t m_index.values[to_index(i)] = first_free;\r\n"; // FN
			output += "\t\t\t first_free = i;\r\n";
			output += "\t\t\t if(size_used - 1 == to_index(i)) {\r\n"; // OIF
			output += "\t\t\t\t for(int32_t j = size_used; j >= 0; --j) {\r\n"; // JLOOP
			output += "\t\t\t\t\t if(m_index.values[j] == " + index_type + "(" + index_type + "::value_base_t(j))) {\r\n"; // IIF
			output += "\t\t\t\t\t\t size_used = j + 1;\r\n";
			output += "\t\t\t\t\t\t return;\r\n";
			output += "\t\t\t\t\t }\r\n"; // IIF
			output += "\t\t\t\t }\r\n"; // JLOOP
			output += "\t\t\t\t size_used = 0;";
			output += "\t\t\t }\r\n"; // OIF
			output += "\t\t }\r\n"; // FN/
		} else {
			// resize
			output += "\t\t void resize(int32_t s) { size_used = s; }\r\n";
		}

		// reset
		output += "\t\t void reset() { this->~container(); new (this)container(); }\r\n";
		// size
		output += "\t\t int32_t size() const { return size_used; }\r\n";
		// vector_size
		output += "\t\t uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }\r\n";
		// is_valid_index
		if(is_sparse) {
			output += std::string("\t\t bool is_valid_index(") + index_type + " i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }\r\n";
		} else {
			output += std::string("\t\t bool is_valid_index(") + index_type + " i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used); }\r\n";
		}
		// for_each
		if(is_sparse) {
			output += "\t\t template<typename FN>\r\n";
			output += "\t\t void for_each(FN const& f) const {\r\n";
			output += "\t\t\t for(int32_t i = 0; i < size_used; ++i) {\r\n";
			output += std::string("\t\t\t\t if(m_index.values[i] == ") + index_type + "(" + index_type + "::value_base_t(i))) f(" + index_type + "(" + index_type + "::value_base_t(i)));\r\n";
			output += "\t\t\t }\r\n";
			output += "\t\t }\r\n";
			output += "\t\t template<typename FN, typename P = concurrency::auto_partitioner>\r\n";
			output += "\t\t void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {\r\n";
			output += "\t\t\t concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {\r\n";
			output += std::string("\t\t\t\t if(_this->m_index.values[i] == ") + index_type + "(" + index_type + "::value_base_t(i))) f(" + index_type + "(" + index_type + "::value_base_t(i)));\r\n";
			output += "\t\t\t }, p);\r\n";
			output += "\t\t }\r\n";
			output += "\t\t template<typename FN>\r\n";
			output += "\t\t bool any_of(FN const& f) const {\r\n";
			output += "\t\t\t for(int32_t i = 0; i < size_used; ++i) {\r\n";
			output += std::string("\t\t\t\t if(m_index.values[i] == ") + index_type + "(" + index_type + "::value_base_t(i))) { if(f(" + index_type + "(" + index_type + "::value_base_t(i)))) return true; }\r\n";
			output += "\t\t\t }\r\n";
			output += "\t\t\t return false;\r\n";
			output += "\t\t }\r\n";
		} else {
			output += "\t\t template<typename FN>\r\n";
			output += "\t\t void for_each(FN const& f) const {\r\n";
			output += "\t\t\t for(int32_t i = 0; i < size_used; ++i) {\r\n";
			output += std::string("\t\t\t\t f(") + index_type + "(" + index_type + "::value_base_t(i)));\r\n";
			output += "\t\t\t }\r\n";
			output += "\t\t }\r\n";
			output += "\t\t template<typename FN, typename P = concurrency::auto_partitioner>\r\n";
			output += "\t\t void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {\r\n";
			output += "\t\t\t concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {\r\n";
			output += std::string("\t\t\t\t f(") + index_type + "(" + index_type + "::value_base_t(i)));\r\n";
			output += "\t\t\t }, p);\r\n";
			output += "\t\t }\r\n";
			output += "\t\t template<typename FN>\r\n";
			output += "\t\t bool any_of(FN const& f) const {\r\n";
			output += "\t\t\t for(int32_t i = 0; i < size_used; ++i) {\r\n";
			output += std::string("\t\t\t\t if(f(") + index_type + "(" + index_type + "::value_base_t(i)))) return true;\r\n";
			output += "\t\t\t }\r\n";
			output += "\t\t\t return false;\r\n";
			output += "\t\t }\r\n";
		}

		output += "\t};\r\n"; // END: container class

		
		output += "\r\n";

		output += "}\r\n"; // END namespace


		//index support
		for(auto& kp : keys_and_types) {
			output += "template<>\r\n";
			output += "struct supports_index<" + namespace_name + "::container, " + kp.key + "> {\r\n";
			output += "\tstatic constexpr bool value = true;\r\n";
			if(kp.type == "bitfield" || kp.type == "bitfield_type") {
				output += "\t using type = bool;\r\n";
				output += "\t using const_type = bool;\r\n";
				output += "\t using row = tagged_array_view<bitfield_type, " + index_type + ">;\r\n";
				output += "\t using const_row = tagged_array_view<bitfield_type const, " + index_type + ">;\r\n";
			} else {
				output += "\t using type = " + kp.type + "&;\r\n";
				output += "\t using const_type = " + kp.type + " const&;\r\n";
				output += "\t using row = tagged_array_view<" + kp.type + ", " + index_type + ">;\r\n";
				output += "\t using const_row = tagged_array_view<" + kp.type + " const, " + index_type + ">;\r\n";
			}
			output += "};\r\n";
		}

		output += "\r\n";

		// serialization

		output += "template<>\r\n";
		output += std::string("class serialization::serializer<") + namespace_name + "::container> {\r\n";
		output += "\t public:\r\n";
		output += "\t static constexpr bool has_static_size = false; \r\n";
		output += "\t static constexpr bool has_simple_serialize = false; \r\n";

		output += "\t template<typename ... CONTEXT>\r\n";
		output += std::string("\t static void serialize_object(std::byte* &output, ") + namespace_name + "::container const& obj, CONTEXT&& ... c) {\r\n";

		// serialize
		output += "\t\t serialization::serialize(output, obj.size_used);\r\n";

		if(is_sparse) {
			output += "\t\t for(int32_t i = 0; i < obj.size_used; ++i) {\r\n";
			output += std::string("\t\t\t if(obj.m_index.values[i] == ") + index_type + "(" + index_type + "::value_base_t(i))) {\r\n";
			output += "\t\t\t\t serialization::serialize(output, obj.m_index.values[i]);\r\n";
			output += "\t\t\t } else {\r\n";
			output += "\t\t\t\t auto tag = " + index_type + "();\r\n";
			output += "\t\t\t\t serialization::serialize(output, tag);\r\n";
			output += "\t\t\t }\r\n";
			output += "\t\t }\r\n";
		}

		for(int32_t i = 0; i < int32_t(keys_and_types.size()); ++i) {
			if(keys_and_types[i].type == "bitfield" || keys_and_types[i].type == "bitfield_type") {
				output += std::string("\t\t serialization::serialize_array(output, obj.m_") + std::to_string(i) + ".values, uint32_t(obj.size_used + 7) / 8ui32);\r\n";
			} else {
				output += "\t\tif constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_static_size) {\r\n";
				output += "\t\t\t if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::size() == 0) {\r\n";
				output += "\t\t\t } else if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_simple_serialize) {\r\n";
				output += "\t\t\t\t serialization::serialize_array(output, obj.m_" + std::to_string(i) + ".values, obj.size_used);\r\n";
				output += "\t\t\t } else {\r\n";
				output += "\t\t\t\t for(int32_t i = 0; i < obj.size_used; ++i)\r\n";
				output += "\t\t\t\t\t serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::serialize_object(output, obj.m_" + std::to_string(i) + ".values[i], std::forward<CONTEXT>(c)...);\r\n";
				output += "\t\t\t }\r\n";
				output += "\t\t} else {\r\n";
				output += "\t\t\t if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_simple_serialize) {\r\n";
				output += "\t\t\t\t serialization::serialize_array(output, obj.m_" + std::to_string(i) + ".values, obj.size_used);\r\n";
				output += "\t\t\t } else {\r\n";
				output += "\t\t\t\t for(int32_t i = 0; i < obj.size_used; ++i)\r\n";
				output += "\t\t\t\t\t serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::serialize_object(output, obj.m_" + std::to_string(i) + ".values[i], std::forward<CONTEXT>(c)...);\r\n";
				output += "\t\t\t }\r\n";
				output += "\t\t}\r\n";
			}
		}

		output += "\t }\r\n";
		output += "\t template<typename ... CONTEXT>\r\n";
		output += std::string("\t static void deserialize_object(std::byte const* &input, ") + namespace_name + "::container& obj, CONTEXT&& ... c) {\r\n";

		// deserialize
		output += "\t\t obj.reset();\r\n";
		output += "\t\t serialization::deserialize(input, obj.size_used);\r\n";
		if(is_sparse) {
			output += "\t\t serialization::deserialize_array(input, obj.m_index.values, obj.size_used);\r\n";
		}

		if(is_sparse) {
			output += "\t\tobj.first_free = " + index_type + "();\r\n";
			output += std::string("\t\tfor(int32_t i = ") + container_size + " - 1; i >= obj.size_used; --i) {\r\n";
			output += "\t\t\t obj.m_index.values[i] = obj.first_free;\r\n";
			output += std::string("\t\t\t obj.first_free = ") + index_type + "(" + index_type + "::value_base_t(i));\r\n";
			output += "\t\t}\r\n";
			output += "\t\tfor(int32_t i = obj.size_used - 1; i >= 0; --i) {\r\n";
			output += "\t\t\t if(!::is_valid_index(obj.m_index.values[i])) {\r\n";
			output += "\t\t\t\t obj.m_index.values[i] = obj.first_free;\r\n";
			output += std::string("\t\t\t\t obj.first_free = ") + index_type + "(" + index_type + "::value_base_t(i));\r\n";
			output += "\t\t\t}\r\n";
			output += "\t\t}\r\n";
		}

		for(int32_t i = 0; i < int32_t(keys_and_types.size()); ++i) {
			if(keys_and_types[i].type == "bitfield" || keys_and_types[i].type == "bitfield_type") {
				output += std::string("\t\t serialization::deserialize_array(input, obj.m_") + std::to_string(i) + ".values, uint32_t(obj.size_used + 7) / 8ui32);\r\n";
			} else {
				output += "\t\tif constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_static_size) {\r\n";
				output += "\t\t\t if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::size() == 0) {\r\n";
				output += "\t\t\t } else if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_simple_serialize) {\r\n";
				output += "\t\t\t\t serialization::deserialize_array(input, obj.m_" + std::to_string(i) + ".values, obj.size_used);\r\n";
				output += "\t\t\t } else {\r\n";
				output += "\t\t\t\t for(int32_t i = 0; i < obj.size_used; ++i)\r\n";
				output += "\t\t\t\t\t serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::deserialize_object(input, obj.m_" + std::to_string(i) + ".values[i], std::forward<CONTEXT>(c)...);\r\n";
				output += "\t\t\t }\r\n";
				output += "\t\t} else {\r\n";
				output += "\t\t\t if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_simple_serialize) {\r\n";
				output += "\t\t\t\t serialization::deserialize_array(input, obj.m_" + std::to_string(i) + ".values, obj.size_used);\r\n";
				output += "\t\t\t } else {\r\n";
				output += "\t\t\t\t for(int32_t i = 0; i < obj.size_used; ++i)\r\n";
				output += "\t\t\t\t\t serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::deserialize_object(input, obj.m_" + std::to_string(i) + ".values[i], std::forward<CONTEXT>(c)...);\r\n";
				output += "\t\t\t }\r\n";
				output += "\t\t}\r\n";

			}
		}

		output += "\t }\r\n";
		output += "\t template<typename ... CONTEXT>\r\n";
		output += std::string("\t static size_t size(") + namespace_name + "::container const& obj, CONTEXT&& ... c) {\r\n";

		// size
		output += "\t\t return size_t(sizeof(obj.size_used)";
		if(is_sparse) {
			output += std::string("+ sizeof(") + index_type + ") * obj.size_used";
		}

		for(int32_t i = 0; i < int32_t(keys_and_types.size()); ++i) {
			if(keys_and_types[i].type == "bitfield" || keys_and_types[i].type == "bitfield_type") {
				output += "+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)";
			} else {
				output += "+ [&, max = obj.size_used](){\r\n";
				output += "\t\t\t if(max == 0)\r\n";
				output += "\t\t\t\t return size_t(0);\r\n";
				output += "\t\t\t if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_static_size) {\r\n";
				output += "\t\t\t\t return size_t(max  * serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::size());\r\n";
				output += "\t\t\t } else if constexpr(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::has_simple_serialize) {\r\n";
				output += "\t\t\t\t return size_t(serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::size(obj.m_" + std::to_string(i) + ".values[0], std::forward<CONTEXT>(c)...) * max); \r\n";
				output += "\t\t\t } else {\r\n";
				output += "\t\t\t\t return size_t(std::transform_reduce(obj.m_" + std::to_string(i) + ".values, obj.m_" + std::to_string(i) + ".values + max, 0ui64, std::plus<>(), [&](" + keys_and_types[i].type + " const& m) {\r\n";
				output += "\t\t\t\t\t return serialization::tagged_serializer<" + keys_and_types[i].key + ", " + keys_and_types[i].type + ">::size(m, std::forward<CONTEXT>(c)...); \r\n";
				output += "\t\t\t\t })); \r\n";
				output += "\t\t\t }\r\n";
				output += "\t\t }()\r\n";
			}
		}

		output += ");\r\n";

		output += "\t }\r\n";
		output += "};\r\n"; // END SERIALIZER CLASS

		output += "#pragma warning( pop )\r\n";
		output += "\r\n";

		std::wstring output_name;
		for(auto c : namespace_name) {
			output_name += wchar_t(c);
		}
		output_name += wchar_t('.');
		output_name += wchar_t('h');

		const auto handleb = CreateFile(output_name.c_str(), GENERIC_WRITE | GENERIC_READ, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if(handleb != INVALID_HANDLE_VALUE) {
			DWORD written;
			WriteFile(handleb, output.c_str(), DWORD(output.length()), &written, nullptr);
			CloseHandle(handleb);
		}
	}
	return 0;
}
