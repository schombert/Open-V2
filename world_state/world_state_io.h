#pragma once
#include "common\\common.h"
#include "world_state.h"
#include "simple_serialize\\simple_serialize.hpp"


template<>
class serialization::serializer<current_state::state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, current_state::state const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, current_state::state& obj, world_state& ws);
	static size_t size(current_state::state const& obj, world_state const& ws);
};

void restore_world_state(world_state& ws);
