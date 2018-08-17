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
class serialization::serializer<nations::loan> : public serialization::memcpy_serializer<nations::loan> {};
template<>
class serialization::serializer<nations::timed_national_modifier> : public serialization::memcpy_serializer<nations::timed_national_modifier> {};

template<>
class serialization::serializer<nations::state_instance> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, nations::state_instance const& obj, world_state& ws) {
		auto owner_id = obj.owner ? obj.owner->id : nations::country_tag();
		serialize(output, owner_id);

		auto nat_focus_id = obj.owner_national_focus ? obj.owner_national_focus->id : modifiers::national_focus_tag();
		serialize(output, nat_focus_id);

		for(uint32_t i = 0; i < std::extent_v<decltype(obj.factories)>; ++i) {
			auto factory_type = obj.factories[i].type ? obj.factories[i].type->id : economy::factory_type_tag();
			serialize(output, factory_type);
			serialize(output, obj.factories[i].level);
		}

		serialize_array(output, obj.colonizers, std::extent_v<decltype(obj.colonizers)>);

		serialize(output, obj.current_tension);
		serialize(output, obj.crisis_tag);
		serialize(output, obj.region_id);
		serialize(output, obj.flags);

		serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.flashpoint_tension_focuses);
	}
	static void deserialize_object(std::byte const* &input, nations::state_instance& obj, world_state& ws) {
		ws.w.nation_s.state_demographics.ensure_capacity(to_index(obj.id) + 1);

		nations::country_tag owner;
		deserialize(input, owner);
		if(is_valid_index(owner))
			obj.owner = ws.w.nation_s.nations.get_location(owner);

		modifiers::national_focus_tag nat_focus;
		deserialize(input, nat_focus);
		if(is_valid_index(nat_focus))
			obj.owner_national_focus = &(ws.s.modifiers_m.national_focuses[nat_focus]);

		for(uint32_t i = 0; i < std::extent_v<decltype(obj.factories)>; ++i) {
			economy::factory_type_tag factory_type;
			deserialize(input, factory_type);
			if(is_valid_index(factory_type))
				obj.factories[i].type = &ws.s.economy_m.factory_types[factory_type];
			else
				obj.factories[i].type = nullptr;
			deserialize(input, obj.factories[i].level);
		}

		deserialize_array(input, obj.colonizers, std::extent_v<decltype(obj.colonizers)>);

		deserialize(input, obj.current_tension);
		deserialize(input, obj.crisis_tag);
		deserialize(input, obj.region_id);
		deserialize(input, obj.flags);

		deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.flashpoint_tension_focuses);

		obj.last_update = ws.w.current_date;

		//final patching TODO:
		//
		// add states to owner's member states list
		// calculate administrative efficiency
		// rebuild demographics
	}
	static size_t size(nations::state_instance const& obj, world_state& ws) {
		return
			sizeof(nations::country_tag) + // owner tag
			sizeof(modifiers::national_focus_tag) + // national focus
			(sizeof(economy::factory_type_tag) + sizeof(obj.factories[0].level)) * std::extent_v<decltype(obj.factories)> + // factories
			sizeof(obj.colonizers[0]) * std::extent_v<decltype(obj.colonizers)> + // colonizers
			sizeof(obj.current_tension) +
			sizeof(obj.crisis_tag) +
			sizeof(obj.region_id) +
			sizeof(obj.flags) +
			serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.flashpoint_tension_focuses)
		;
	}
};

template<>
class serialization::serializer<nations::nation> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, nations::nation const& obj, world_state& ws) {
		auto overlord_id = obj.overlord ? obj.overlord->id : nations::country_tag();
		serialize(output, overlord_id);

		auto sphere_leader_id = obj.sphere_leader ? obj.sphere_leader->id : nations::country_tag();
		serialize(output, sphere_leader_id);

		serialize(output, obj.last_election);
		serialize(output, obj.last_lost_war);
		serialize(output, obj.disarmed_until);

		serialize(output, obj.plurality);
		serialize(output, obj.revanchism);
		serialize(output, obj.base_prestige);
		serialize(output, obj.infamy);
		serialize(output, obj.war_exhaustion);

		serialize(output, obj.rich_tax);
		serialize(output, obj.middle_tax);
		serialize(output, obj.poor_tax);
		serialize(output, obj.social_spending);
		serialize(output, obj.administrative_spending);
		serialize(output, obj.education_spending);
		serialize(output, obj.military_spending);

		serialize(output, obj.current_government);
		serialize(output, obj.primary_culture);
		serialize(output, obj.national_religion);
		serialize(output, obj.current_capital);
		serialize(output, obj.ruling_party);

		serialize(output, obj.flags);

		serialize(output, obj.cb_construction_progress);
		serialize(output, obj.cb_construction_target);
		serialize(output, obj.cb_construction_type);

		serialize(output, obj.base_colonial_points);
		serialize(output, obj.current_color);

		serialize(output, obj.national_value);
		serialize(output, obj.tech_school);

		auto active_parties = ws.w.nation_s.active_parties.get_row(obj.id);
		serialize_array(output, active_parties, ws.s.ideologies_m.ideologies_count);

		auto upper_house = ws.w.nation_s.upper_house.get_row(obj.id);
		serialize_array(output, upper_house, ws.s.ideologies_m.ideologies_count);

		auto active_tech = ws.w.nation_s.active_technologies.get_row(obj.id);
		serialize_array(output, active_tech, (uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32);

		auto active_issues = ws.w.nation_s.active_issue_options.get_row(obj.id);
		serialize_array(output, active_issues, uint32_t(ws.s.issues_m.issues_container.size()));

		auto stockpile = ws.w.nation_s.national_stockpiles.get_row(obj.id);
		serialize_array(output, stockpile, ws.s.economy_m.aligned_32_goods_count);

		auto variables = ws.w.nation_s.national_variables.get_row(obj.id);
		serialize_array(output, variables, ws.s.variables_m.count_national_variables);

		serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.allies);
		serialize_stable_array(output, ws.w.nation_s.influence_arrays, obj.gp_influence);
		serialize_stable_array(output, ws.w.province_s.province_arrays, obj.naval_patrols);
		serialize_stable_array(output, ws.w.culture_s.culture_arrays, obj.accepted_cultures);
		serialize_stable_array(output, ws.w.nation_s.relations_arrays, obj.relations);
		serialize_stable_array(output, ws.w.nation_s.truce_arrays, obj.truces);
		serialize_stable_array(output, ws.w.nation_s.loan_arrays, obj.loans);
		serialize_stable_array(output, ws.w.nation_s.state_tag_arrays, obj.national_focus_locations);
		serialize_stable_array(output, ws.w.variable_s.national_flags_arrays, obj.national_flags);
		serialize_stable_array(output, ws.w.nation_s.static_modifier_arrays, obj.static_modifiers);
		serialize_stable_array(output, ws.w.nation_s.timed_modifier_arrays, obj.timed_modifiers);
		serialize_stable_array(output, ws.w.military_s.leader_arrays, obj.generals);
		serialize_stable_array(output, ws.w.military_s.leader_arrays, obj.admirals);
		serialize_stable_array(output, ws.w.military_s.army_arrays, obj.armies);
		serialize_stable_array(output, ws.w.military_s.fleet_arrays, obj.fleets);
		serialize_stable_array(output, ws.w.military_s.orders_arrays, obj.active_orders);
		serialize_stable_array(output, ws.w.military_s.war_arrays, obj.wars_involved_in);
		serialize_stable_array(output, ws.w.population_s.rebel_faction_arrays, obj.active_rebel_factions);
		serialize_stable_array(output, ws.w.population_s.pop_movement_arrays, obj.active_movements);
	}

	static void deserialize_object(std::byte const* &input, nations::nation& obj, world_state& ws) {
		ws.w.nation_s.active_parties.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.nation_demographics.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.upper_house.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.active_technologies.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.active_goods.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.active_issue_options.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.national_stockpiles.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.national_variables.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.unit_stats.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.rebel_org_gain.ensure_capacity(to_index(obj.id) + 1);
		ws.w.nation_s.production_adjustments.ensure_capacity(to_index(obj.id) + 1);

		nations::country_tag overlord_id;
		deserialize(input, overlord_id);
		if(is_valid_index(overlord_id)) {
			obj.overlord = &ws.w.nation_s.nations.get(overlord_id);
			add_item(ws.w.nation_s.nations_arrays, obj.overlord->vassals, obj.id);
		}


		nations::country_tag sphere_leader_id;
		deserialize(input, sphere_leader_id);
		if(is_valid_index(sphere_leader_id)) {
			obj.sphere_leader = &ws.w.nation_s.nations.get(sphere_leader_id);
			add_item(ws.w.nation_s.nations_arrays, obj.overlord->sphere_members, obj.id);
		}

		obj.last_update = ws.w.current_date;

		deserialize(input, obj.last_election);
		deserialize(input, obj.last_lost_war);
		deserialize(input, obj.disarmed_until);

		deserialize(input, obj.plurality);
		deserialize(input, obj.revanchism);
		deserialize(input, obj.base_prestige);
		deserialize(input, obj.infamy);
		deserialize(input, obj.war_exhaustion);

		deserialize(input, obj.rich_tax);
		deserialize(input, obj.middle_tax);
		deserialize(input, obj.poor_tax);
		deserialize(input, obj.social_spending);
		deserialize(input, obj.administrative_spending);
		deserialize(input, obj.education_spending);
		deserialize(input, obj.military_spending);

		deserialize(input, obj.current_government);
		deserialize(input, obj.primary_culture);
		deserialize(input, obj.national_religion);
		deserialize(input, obj.current_capital);
		deserialize(input, obj.ruling_party);

		deserialize(input, obj.flags);

		deserialize(input, obj.cb_construction_progress);
		deserialize(input, obj.cb_construction_target);
		deserialize(input, obj.cb_construction_type);

		deserialize(input, obj.base_colonial_points);
		deserialize(input, obj.current_color);

		deserialize(input, obj.national_value);
		deserialize(input, obj.tech_school);

		auto active_parties = ws.w.nation_s.active_parties.get_row(obj.id);
		deserialize_array(input, active_parties, ws.s.ideologies_m.ideologies_count);

		auto upper_house = ws.w.nation_s.upper_house.get_row(obj.id);
		deserialize_array(input, upper_house, ws.s.ideologies_m.ideologies_count);

		auto active_tech = ws.w.nation_s.active_technologies.get_row(obj.id);
		deserialize_array(input, active_tech, (uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32);

		auto active_issues = ws.w.nation_s.active_issue_options.get_row(obj.id);
		deserialize_array(input, active_issues, uint32_t(ws.s.issues_m.issues_container.size()));

		auto stockpile = ws.w.nation_s.national_stockpiles.get_row(obj.id);
		deserialize_array(input, stockpile, ws.s.economy_m.aligned_32_goods_count);

		auto variables = ws.w.nation_s.national_variables.get_row(obj.id);
		deserialize_array(input, variables, ws.s.variables_m.count_national_variables);

		deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.allies);

		deserialize_stable_array(input, ws.w.nation_s.influence_arrays, obj.gp_influence);
		auto inf_range = get_range(ws.w.nation_s.influence_arrays, obj.gp_influence);
		for(auto c = inf_range.first; c != inf_range.second; ++c)
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[c->target].influencers, obj.id);

		deserialize_stable_array(input, ws.w.province_s.province_arrays, obj.naval_patrols);
		deserialize_stable_array(input, ws.w.culture_s.culture_arrays, obj.accepted_cultures);
		deserialize_stable_array(input, ws.w.nation_s.relations_arrays, obj.relations);
		deserialize_stable_array(input, ws.w.nation_s.truce_arrays, obj.truces);
		deserialize_stable_array(input, ws.w.nation_s.loan_arrays, obj.loans);
		deserialize_stable_array(input, ws.w.nation_s.state_tag_arrays, obj.national_focus_locations);
		deserialize_stable_array(input, ws.w.variable_s.national_flags_arrays, obj.national_flags);
		deserialize_stable_array(input, ws.w.nation_s.static_modifier_arrays, obj.static_modifiers);
		deserialize_stable_array(input, ws.w.nation_s.timed_modifier_arrays, obj.timed_modifiers);
		deserialize_stable_array(input, ws.w.military_s.leader_arrays, obj.generals);
		deserialize_stable_array(input, ws.w.military_s.leader_arrays, obj.admirals);
		deserialize_stable_array(input, ws.w.military_s.army_arrays, obj.armies);
		deserialize_stable_array(input, ws.w.military_s.fleet_arrays, obj.fleets);
		deserialize_stable_array(input, ws.w.military_s.orders_arrays, obj.active_orders);
		deserialize_stable_array(input, ws.w.military_s.war_arrays, obj.wars_involved_in);
		deserialize_stable_array(input, ws.w.population_s.rebel_faction_arrays, obj.active_rebel_factions);
		deserialize_stable_array(input, ws.w.population_s.pop_movement_arrays, obj.active_movements);

		//final patching TODO:
		//
		// rebuild
		// set_tag<country_tag> opponents_in_war;
		// set_tag<country_tag> allies_in_war; -- update_at_war_with_and_against -- after military reloaded
		//
		//calculate derived values:
		//float blockade_fraction = 0.0f;
		//float rebel_control_fraction = 0.0f; // of provinces connected to capital
		//float political_interest_fraction = 0.0f;
		//float social_interest_fraction = 0.0f;
		//float crime_fraction = 0.0f; //fraction of provinces with active crime
		//float social_movement_support = 0.0f; // sum of social movement supporters / total pop * defines factor
		//float political_movement_support = 0.0f; // sum of social movement supporters / total pop * defines factor
		//uint8_t num_of_active_revolts = 0ui8;
		//ideologies::ideology_tag ruling_ideology;
		//issues::option_tag dominant_issue;
		//ideologies::ideology_tag dominant_ideology;
		//cultures::religion_tag dominant_religion;
		//cultures::culture_tag dominant_culture;
		//uint16_t num_connected_ports = 0ui16; // number of ports connected to capital by land
		//uint16_t num_ports = 0ui16;
		//int16_t military_score = 0i16;
		//int16_t industrial_score = 0i16;
		//int16_t overall_rank = 0i16;
		//int16_t prestige_rank = 0i16;
		//int16_t military_rank = 0i16;
		//int16_t industrial_rank = 0i16;
		//
		//fill owned and controlled arrays from province state
		//
		//set_tag<country_tag> neighboring_nations;
		//
		//determine tag from national tags -> holders value
		//graphics::texture_tag flag; -- from tag
		//text_data::text_tag name; -- from tag
		//text_data::text_tag adjective; -- from tag
		//
		// technologies::reset_technologies(ws, obj); ... then
		//reapply all technologies (only after units have been loaded)
		//
		//update demographics
	}

	static size_t size(nations::nation const& obj, world_state& ws) {
		return
			sizeof(nations::country_tag) + // overlord id
			sizeof(nations::country_tag) + // sphere leader tag
			sizeof(date_tag) * 3  + // last election, last lost war, disarmed until
			sizeof(float) * 5  + // plurality ... war exhaustion
			sizeof(int8_t) * 7 + // budget items
			sizeof(obj.current_government) +
			sizeof(obj.primary_culture) +
			sizeof(obj.national_religion) +
			sizeof(obj.current_capital) +
			sizeof(obj.ruling_party) +
			sizeof(obj.flags) +
			sizeof(obj.cb_construction_progress) +
			sizeof(obj.cb_construction_target) +
			sizeof(obj.cb_construction_type) +
			sizeof(obj.base_colonial_points) +
			sizeof(obj.current_color) + 
			sizeof(obj.national_value) +
			sizeof(obj.tech_school) +
			sizeof(governments::party_tag) * ws.s.ideologies_m.ideologies_count + // active parties
			sizeof(uint8_t) * ws.s.ideologies_m.ideologies_count + // upper house
			sizeof(uint64_t) * ((uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32) + // active technologies
			sizeof(issues::option_tag) * ws.s.issues_m.issues_container.size() + // active issue options
			sizeof(economy::goods_qnty_type) * ws.s.economy_m.aligned_32_goods_count + // national stockpiles
			sizeof(float) * ws.s.variables_m.count_national_variables + // national variables
			serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.allies) + 
			serialize_stable_array_size(ws.w.nation_s.influence_arrays, obj.gp_influence) +
			serialize_stable_array_size(ws.w.province_s.province_arrays, obj.naval_patrols) +
			serialize_stable_array_size( ws.w.culture_s.culture_arrays, obj.accepted_cultures) +
			serialize_stable_array_size( ws.w.nation_s.relations_arrays, obj.relations) +
			serialize_stable_array_size( ws.w.nation_s.truce_arrays, obj.truces) +
			serialize_stable_array_size( ws.w.nation_s.loan_arrays, obj.loans) +
			serialize_stable_array_size( ws.w.nation_s.state_tag_arrays, obj.national_focus_locations) +
			serialize_stable_array_size( ws.w.variable_s.national_flags_arrays, obj.national_flags) +
			serialize_stable_array_size( ws.w.nation_s.static_modifier_arrays, obj.static_modifiers) +
			serialize_stable_array_size( ws.w.nation_s.timed_modifier_arrays, obj.timed_modifiers) +
			serialize_stable_array_size( ws.w.military_s.leader_arrays, obj.generals) +
			serialize_stable_array_size( ws.w.military_s.leader_arrays, obj.admirals) +
			serialize_stable_array_size( ws.w.military_s.army_arrays, obj.armies) +
			serialize_stable_array_size( ws.w.military_s.fleet_arrays, obj.fleets) +
			serialize_stable_array_size( ws.w.military_s.orders_arrays, obj.active_orders) +
			serialize_stable_array_size( ws.w.military_s.war_arrays, obj.wars_involved_in) +
			serialize_stable_array_size( ws.w.population_s.rebel_faction_arrays, obj.active_rebel_factions) +
			serialize_stable_array_size( ws.w.population_s.pop_movement_arrays, obj.active_movements)
			;
	}
};


template<>
class serialization::serializer<nations::nations_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, nations::nations_state const& obj, world_state& ws) {
		serialize(output, obj.nations, ws);
		serialize(output, obj.states, ws);
	}
	static void deserialize_object(std::byte const* &input, nations::nations_state& obj, world_state& ws) {
		deserialize(input, obj.nations, ws);
		deserialize(input, obj.states, ws);

		//final patching TODO:
		//
		// rebuild  nations by rank array
	}
	static size_t size(nations::nations_state const& obj, world_state& ws) {
		return serialize_size(obj.nations, ws) + serialize_size(obj.states, ws);
	}
};

namespace nations {
	void read_nation_file(world_state& ws, cultures::national_tag nat_tag_id, date_tag target_date, file const& fi, directory const& units_dir, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_nations_files(world_state& ws, date_tag target_date, directory const& root, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_diplomacy_files(world_state& ws, date_tag target_date, directory const& root);
}
