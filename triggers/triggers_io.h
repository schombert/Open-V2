#pragma once
#include "triggers.h"
#include "simple_serialize\\simple_serialize.hpp"

template<>
class serialization::serializer<triggers::trigger_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, triggers::trigger_manager const& obj) {
		serialize(output, obj.trigger_data);
		serialize(output, obj.effect_data);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, triggers::trigger_manager& obj, CONTEXT&& ... c) {
		deserialize(input, obj.trigger_data);
		deserialize(input, obj.effect_data);
	}
	static size_t size(triggers::trigger_manager const& obj) {
		return serialize_size(obj.trigger_data) +
			serialize_size(obj.effect_data);
	}
};
