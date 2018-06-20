#pragma once
#include "text_data.h"
#include "simple_fs\\simple_fs.h"
#include "simple_serialize\\simple_serialize.hpp"

template<>
class serialization::serializer<text_data::text_component> : public serialization::memcpy_serializer<text_data::text_component> {};
template<>
class serialization::serializer<text_data::text_sequence> : public serialization::memcpy_serializer<text_data::text_sequence> {};

template<>
class serialization::serializer<text_data::text_sequences> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, text_data::text_sequences const& obj) {
		serialize(output, obj.text_data);
		serialize(output, obj.key_data);
		serialize(output, obj.all_components);
		serialize(output, obj.all_sequences);
		serialize(output, obj.key_to_sequence_map);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, text_data::text_sequences& obj, CONTEXT&& ... c) {
		deserialize(input, obj.text_data);
		deserialize(input, obj.key_data);
		deserialize(input, obj.all_components);
		deserialize(input, obj.all_sequences);
		deserialize(input, obj.key_to_sequence_map);
	}
	static size_t size(text_data::text_sequences const& obj) {
		return serialize_size(obj.text_data) +
			serialize_size(obj.key_data) +
			serialize_size(obj.all_components) +
			serialize_size(obj.all_sequences) +
			serialize_size(obj.key_to_sequence_map);
	}
};

namespace text_data {
	void load_text_sequences_from_directory(const directory& source_directory, text_data::text_sequences& container);
}
