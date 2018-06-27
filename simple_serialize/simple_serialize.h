#pragma once
#include "common\\common.h"

namespace serialization {
	template<typename T, typename ... CONTEXT>
	void serialize(std::byte* &output, T const& obj, CONTEXT&& ... c);
	template<typename T, typename ... CONTEXT>
	void deserialize(std::byte const* &input, T& obj, CONTEXT&& ... c);
	template<typename T>
	size_t serialize_size(T const& obj);

	template<typename T>
	class serializer;

	template<typename T>
	size_t serialize_size(T const& obj);

	template<typename T, typename ... CONTEXT>
	void serialize_to_file(std::u16string const& file_name, T const& obj, CONTEXT&& ... c);
	template<typename T, typename ... CONTEXT>
	void deserialize_from_file(std::u16string const& file_name, T& obj, CONTEXT&& ... c);
}
