#pragma once
#include "nations.h"
#include "simple_fs\\simple_fs.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "technologies\\technologies_functions.h"

class world_state;

template<>
class serialization::serializer<nations::influence> : public serialization::memcpy_serializer<nations::influence> {};
template<>
class serialization::serializer<nations::relationship> : public serialization::memcpy_serializer<nations::relationship> {};
template<>
class serialization::serializer<nations::truce> : public serialization::memcpy_serializer<nations::truce> {};
template<>
class serialization::serializer<nations::timed_national_modifier> : public serialization::memcpy_serializer<nations::timed_national_modifier> {};
template<>
class serialization::serializer<nations::pop_project> : public serialization::memcpy_serializer<nations::pop_project> {};

template<typename T>
class serialization::tagged_serializer<state::flashpoint_tension_focuses, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
};

template<typename T>
class serialization::tagged_serializer<state::dominant_ideology, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::dominant_issue, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::dominant_religion, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::dominant_culture, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::administrative_efficiency, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::state_capital, T> : public serialization::discard_serializer<T> {};


template<typename T>
class serialization::tagged_serializer<nation::current_rules, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::ruling_ideology, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::enabled_crimes, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::dominant_culture, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::dominant_issue, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::dominant_ideology, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::dominant_religion, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::military_score, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::industrial_score, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::overall_rank, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::prestige_rank, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::military_rank, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::industrial_rank, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::central_province_count, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::rebel_controlled_provinces, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::blockaded_count, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::crime_count, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::num_connected_ports, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::num_ports, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::revanchism, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::political_interest_fraction, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::social_interest_fraction, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::national_administrative_efficiency, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::social_movement_support, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::political_movement_support, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::tech_attributes, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::is_colonial_nation, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::has_gas_attack, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::has_gas_defence, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::name, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::adjective, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::flag, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<nation::owned_provinces, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::controlled_provinces, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::sphere_members, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::vassals, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::neighboring_nations, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::member_states, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::influencers, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::statewise_tarrif_mask, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::armies, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::opponents_in_war, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<nation::allies_in_war, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<nation::allies, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::gp_influence, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.nation_s.influence_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.nation_s.influence_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.nation_s.influence_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::naval_patrols, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.province_s.province_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.province_s.province_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.province_s.province_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::accepted_cultures, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.culture_s.culture_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.culture_s.culture_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.culture_s.culture_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::relations, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.nation_s.relations_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.nation_s.relations_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.nation_s.relations_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::truces, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.nation_s.truce_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.nation_s.truce_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.nation_s.truce_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::national_focus_locations, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.nation_s.state_tag_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.nation_s.state_tag_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.nation_s.state_tag_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::national_flags, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.variable_s.national_flags_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.variable_s.national_flags_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.variable_s.national_flags_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::static_modifiers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.nation_s.static_modifier_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.nation_s.static_modifier_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.nation_s.static_modifier_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::timed_modifiers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.nation_s.timed_modifier_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.nation_s.timed_modifier_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.nation_s.timed_modifier_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::generals, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.military_s.leader_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.military_s.leader_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.military_s.leader_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::admirals, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.military_s.leader_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.military_s.leader_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.military_s.leader_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::fleets, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.military_s.fleet_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.military_s.fleet_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.military_s.fleet_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::active_orders, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.military_s.orders_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.military_s.orders_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.military_s.orders_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::active_cbs, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.military_s.cb_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.military_s.cb_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.military_s.cb_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::wars_involved_in, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.military_s.war_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.military_s.war_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.military_s.war_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::active_rebel_factions, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.population_s.rebel_faction_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.population_s.rebel_faction_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.population_s.rebel_faction_arrays, obj);
	}
};
template<typename T>
class serialization::tagged_serializer<nation::active_movements, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.population_s.pop_movement_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.population_s.pop_movement_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.population_s.pop_movement_arrays, obj);
	}
};

template<>
class serialization::serializer<nations::nations_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, nations::nations_state const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, nations::nations_state& obj, world_state& ws);
	static size_t size(nations::nations_state const& obj, world_state const& ws);
};

namespace nations {
	void read_nation_file(world_state& ws, cultures::national_tag nat_tag_id, date_tag target_date, file const& fi, directory const& units_dir, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_nations_files(world_state& ws, date_tag target_date, directory const& root, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_diplomacy_files(world_state& ws, date_tag target_date, directory const& root);
	void set_default_tech_school(world_state& ws);
}
