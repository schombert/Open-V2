#pragma once
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "graphics_objects.h"
#include "simple_serialize\\simple_serialize.hpp"

template<>
class serialization::serializer<graphics::object> : public serialization::memcpy_serializer<graphics::object> {};

template<>
class serialization::serializer<graphics::object_definitions> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, graphics::object_definitions const& obj) {
		serialize(output, obj.definitions);
		serialize(output, obj.standard_text_background);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, graphics::object_definitions& obj, CONTEXT&& ... c) {
		deserialize(input, obj.definitions);
		deserialize(input, obj.standard_text_background);
	}
	static size_t size(graphics::object_definitions const& obj) {
		return serialize_size(obj.definitions) + serialize_size(obj.standard_text_background);
	}
};

namespace graphics {
	void load_graphics_object_definitions_from_directory(
		const directory& source_directory,
		graphics::name_maps& nmaps,
		graphics::object_definitions& defs,
		std::vector<std::pair<std::string, graphics::errors>>& errors_generated,
		const texture_lookup& th_f);
}
