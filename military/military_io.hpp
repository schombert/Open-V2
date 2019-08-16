#pragma once
#include "common\common.h"
#include "military_io.h"
#include "world_state\world_state.h"

template<typename T>
void serialization::tagged_serializer<war::attackers, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<war::attackers, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<war::attackers, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<war::defenders, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<war::defenders, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<war::defenders, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<war::naval_control_set, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.naval_control_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<war::naval_control_set, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.naval_control_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<war::naval_control_set, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.naval_control_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<war::war_goals, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.war_goal_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<war::war_goals, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.war_goal_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<war::war_goals, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.war_goal_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<border_information::hqs, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.hq_commitment_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<border_information::hqs, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.hq_commitment_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<border_information::hqs, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.hq_commitment_arrays, obj);
}