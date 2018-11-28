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

template<typename T>
void serialization::tagged_serializer<nation::allies, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::allies, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::allies, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::gp_influence, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.influence_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::gp_influence, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.influence_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::gp_influence, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.influence_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::naval_patrols, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.province_s.province_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::naval_patrols, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.province_s.province_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::naval_patrols, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.province_s.province_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::accepted_cultures, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.culture_s.culture_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::accepted_cultures, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.culture_s.culture_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::accepted_cultures, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.culture_s.culture_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::relations, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.relations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::relations, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.relations_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::relations, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.relations_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::truces, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.truce_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::truces, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.truce_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::truces, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.truce_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::national_focus_locations, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.state_tag_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::national_focus_locations, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.state_tag_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::national_focus_locations, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.state_tag_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::national_flags, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.variable_s.national_flags_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::national_flags, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.variable_s.national_flags_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::national_flags, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.variable_s.national_flags_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::static_modifiers, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.static_modifier_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::static_modifiers, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.static_modifier_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::static_modifiers, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.static_modifier_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::timed_modifiers, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.nation_s.timed_modifier_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::timed_modifiers, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.nation_s.timed_modifier_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::timed_modifiers, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.nation_s.timed_modifier_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::generals, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.leader_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::generals, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.leader_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::generals, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.leader_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::admirals, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.leader_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::admirals, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.leader_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::admirals, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.leader_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::fleets, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.fleet_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::fleets, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.fleet_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::fleets, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.fleet_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_orders, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.orders_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_orders, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.orders_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::active_orders, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.orders_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_cbs, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.cb_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_cbs, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.cb_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::active_cbs, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.cb_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::wars_involved_in, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.military_s.war_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::wars_involved_in, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.military_s.war_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::wars_involved_in, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.military_s.war_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_rebel_factions, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.population_s.rebel_faction_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_rebel_factions, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.population_s.rebel_faction_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::active_rebel_factions, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.population_s.rebel_faction_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_movements, T>::serialize_object(std::byte *& output, T const & obj, world_state const & ws) {
	serialize_stable_array(output, ws.w.population_s.pop_movement_arrays, obj);
}

template<typename T>
void serialization::tagged_serializer<nation::active_movements, T>::deserialize_object(std::byte const *& input, T & obj, world_state & ws) {
	deserialize_stable_array(input, ws.w.population_s.pop_movement_arrays, obj);
}

template<typename T>
size_t serialization::tagged_serializer<nation::active_movements, T>::size(T const & obj, world_state const & ws) {
	return serialize_stable_array_size(ws.w.population_s.pop_movement_arrays, obj);
}
