#pragma once
#include "nations_io.h"
#include "world_state\\world_state.h"

template<typename T>
void serialization::tagged_serializer<state::flashpoint_tension_focuses, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<state::flashpoint_tension_focuses, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<state::flashpoint_tension_focuses, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj);
}
