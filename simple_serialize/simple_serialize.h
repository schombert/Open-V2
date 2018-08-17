#pragma once
#include "common\\common.h"

namespace serialization {
	template<typename T, typename ... CONTEXT>
	void serialize(std::byte* &output, T const& obj, CONTEXT&& ... c);
	template<typename T, typename ... CONTEXT>
	void deserialize(std::byte const* &input, T& obj, CONTEXT&& ... c);
	template<typename T, typename ... CONTEXT>
	size_t serialize_size(T const& obj, CONTEXT&& ... c);

	template<typename T>
	class serializer;


	struct serialize_file_header {
		uint8_t identifier[64]; // blake2b hash of mods and mod versions
		uint64_t decompressed_size; // 0 if no compression
		uint64_t version; // file format version
		char16_t display_name[256]; // name to display in UI
	};

	template<typename T, typename ... CONTEXT>
	void serialize_to_file(std::u16string const& file_name, bool compress, serialize_file_header& header, T const& obj, CONTEXT&& ... c);
	template<typename T, typename ... CONTEXT>
	void deserialize_from_file(std::u16string const& file_name, T& obj, CONTEXT&& ... c);
}
