#pragma once
#include "provinces_io.h"
#include "world_state\\world_state.h"

template<typename T>
void serialization::tagged_serializer<province_state::cores, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.province_s.core_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<province_state::cores, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.province_s.core_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<province_state::cores, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.province_s.core_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<province_state::timed_modifiers, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.province_s.timed_modifier_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<province_state::timed_modifiers, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.province_s.timed_modifier_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<province_state::timed_modifiers, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.province_s.timed_modifier_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<province_state::static_modifiers, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.province_s.static_modifier_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<province_state::static_modifiers, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.province_s.static_modifier_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<province_state::static_modifiers, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.province_s.static_modifier_arrays, obj);
}
