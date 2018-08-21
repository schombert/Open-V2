#include "common\\common.h"
#include "world_state_io.h"
#include "cultures\\cultures_io.h"
#include "economy\\economy_io.h"
#include "ideologies\\ideologies_io.h"
#include "nations\\nations_io.h"
#include "population\\population_io.h"
#include "provinces\\provinces_io.h"
#include "military\\military_io.h"
#include "variables\\variables_io.h"
#include "provinces\\province_functions.h"
#include "cultures\\cultures_functions.h"
#include "military\\military_functions.h"
#include "nations\\nations_functions.h"
#include "economy\\economy_functions.h"
#include "population\\population_function.h"
#include "variables\\variables_functions.h"
#include "ideologies\\ideologies_functions.h"
#include "modifiers\\modifier_functions.h"
#include "issues\\issues_functions.h"

void serialization::serializer<current_state::state>::serialize_object(std::byte *& output, current_state::state const & obj, world_state const & ws) {
	serialize(output, obj.province_s, ws);
	serialize(output, obj.culture_s, ws);
	serialize(output, obj.military_s, ws);
	serialize(output, obj.nation_s, ws);
	serialize(output, obj.economy_s, ws);
	serialize(output, obj.population_s, ws);
	serialize(output, obj.variable_s, ws);
	serialize(output, obj.ideology_s, ws);

	serialize(output, obj.crisis_temperature);
	uint8_t ctype = uint8_t(obj.current_crisis_type);
	serialize(output, ctype);

	auto pattacker_tag = obj.crisis_primary_attacker ? obj.crisis_primary_attacker->id : nations::country_tag();
	serialize(output, pattacker_tag);
	auto pdefender_tag = obj.crisis_primary_defender ? obj.crisis_primary_defender->id : nations::country_tag();
	serialize(output, pdefender_tag);
	auto target_tag = obj.crisis_target ? obj.crisis_target->id : nations::country_tag();
	serialize(output, target_tag);
	serialize(output, obj.crisis_state);

	serialize(output, obj.current_date);
	serialize(output, obj.great_wars_enabled);
	serialize(output, obj.world_wars_enabled);

	auto player_tag = obj.local_player_nation ? obj.local_player_nation->id : nations::country_tag();
	serialize(output, player_tag);

	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.crisis_attackers);
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.crisis_defenders);
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.crisis_interested);
	serialize_stable_array(output, ws.w.military_s.war_goal_arrays, obj.crisis_goals);
}

void serialization::serializer<current_state::state>::deserialize_object(std::byte const *& input, current_state::state & obj, uint64_t version, world_state & ws) {
	if(version != 1ui64)
		std::abort();

	provinces::reset_state(obj.province_s);
	cultures::reset_state(obj.culture_s);
	military::reset_state(obj.military_s);
	nations::reset_state(obj.nation_s);
	economy::reset_state(obj.economy_s);
	population::reset_state(obj.population_s);
	variables::reset_state(obj.variable_s);
	ideologies::reset_state(obj.ideology_s);

	deserialize(input, obj.province_s, ws);
	deserialize(input, obj.culture_s, ws);
	deserialize(input, obj.military_s, ws);
	deserialize(input, obj.nation_s, ws);
	deserialize(input, obj.economy_s, ws);
	deserialize(input, obj.population_s, ws);
	deserialize(input, obj.variable_s, ws);
	deserialize(input, obj.ideology_s, ws);

	deserialize(input, obj.crisis_temperature);

	uint8_t ctype = 0ui8;
	deserialize(input, ctype);
	obj.current_crisis_type = current_state::crisis_type(ctype);

	nations::country_tag pattacker_tag;
	deserialize(input, pattacker_tag);
	obj.crisis_primary_attacker = ws.w.nation_s.nations.get_location(pattacker_tag);

	nations::country_tag pdefender_tag;
	deserialize(input, pdefender_tag);
	obj.crisis_primary_defender = ws.w.nation_s.nations.get_location(pattacker_tag);

	nations::country_tag target_tag;
	deserialize(input, target_tag);
	obj.crisis_target = ws.w.nation_s.nations.get_location(pattacker_tag);

	deserialize(input, obj.crisis_state);

	deserialize(input, obj.current_date);
	deserialize(input, obj.great_wars_enabled);
	deserialize(input, obj.world_wars_enabled);

	nations::country_tag player_tag;
	deserialize(input, player_tag);
	obj.local_player_nation = ws.w.nation_s.nations.get_location(player_tag);

	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.crisis_attackers);
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.crisis_defenders);
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.crisis_interested);
	deserialize_stable_array(input, ws.w.military_s.war_goal_arrays, obj.crisis_goals);

	restore_world_state(ws);
}

size_t serialization::serializer<current_state::state>::size(current_state::state const & obj, world_state const & ws) {
	return serialize_size(obj.province_s, ws) +
		serialize_size(obj.culture_s, ws) +
		serialize_size(obj.military_s, ws) +
		serialize_size(obj.nation_s, ws) +
		serialize_size(obj.economy_s, ws) +
		serialize_size(obj.population_s, ws) +
		serialize_size(obj.variable_s, ws) +
		serialize_size(obj.ideology_s, ws) +
		serialize_size(obj.crisis_temperature) +
		sizeof(uint8_t) +
		sizeof(nations::country_tag) +
		sizeof(nations::country_tag) +
		sizeof(nations::country_tag) +
		serialize_size(obj.crisis_state) +
		serialize_size(obj.current_date) +
		serialize_size(obj.great_wars_enabled) +
		serialize_size(obj.world_wars_enabled) +
		sizeof(nations::country_tag) +
		serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.crisis_attackers) +
		serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.crisis_defenders) +
		serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.crisis_interested) +
		serialize_stable_array_size(ws.w.military_s.war_goal_arrays, obj.crisis_goals);
}

void restore_world_state(world_state& ws) {
	for(int32_t i = int32_t(ws.w.culture_s.national_tags_state.size()); i--; ) {
		cultures::national_tag this_tag(static_cast<cultures::national_tag::value_base_t>(i));
		auto& nt = ws.w.culture_s.national_tags_state[this_tag];

		if(nt.holder) {
			nt.holder->tag = this_tag;
			auto names = ws.s.culture_m.country_names_by_government.get(this_tag, nt.holder->current_government);

			if(is_valid_index(names.name))
				nt.holder->name = names.name;
			else
				nt.holder->name = ws.s.culture_m.national_tags[this_tag].default_name.name;

			if(is_valid_index(names.adjective))
				nt.holder->adjective = names.adjective;
			else
				nt.holder->adjective = ws.s.culture_m.national_tags[this_tag].default_name.adjective;

			nt.holder->flag = ws.w.culture_s.country_flags_by_government.get(this_tag, nt.holder->current_government);
		}
	}

	for(auto &ps : ws.w.province_s.province_state_container) {
		auto cores_range = get_range(ws.w.province_s.core_arrays, ps.cores);
		ps.flags &= ~provinces::province_state::has_owner_core;
		for(auto c : cores_range) {
			add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[c].core_provinces, ps.id);
			if(ps.owner && ps.owner->tag == c)
				ps.flags |= provinces::province_state::has_owner_core;
		}
		if(ps.orders)
			add_item(ws.w.province_s.province_arrays, ps.orders->involved_provinces, ps.id);
		if(ps.owner)
			add_item(ws.w.province_s.province_arrays, ps.owner->owned_provinces, ps.id);
		if(ps.controller)
			add_item(ws.w.province_s.province_arrays, ps.controller->controlled_provinces, ps.id);
		if(ps.rebel_controller)
			add_item(ws.w.province_s.province_arrays, ps.rebel_controller->controlled_provinces, ps.id);
	}

	ws.w.military_s.armies.for_each([&ws](military::army& a) {
		if(a.current_orders)
			add_item(ws.w.military_s.army_arrays, a.current_orders->involved_armies, a.id);
		add_item(ws.w.military_s.army_arrays, ws.w.nation_s.nations[a.owner].armies, a.id);
	});

	ws.w.nation_s.states.for_each([&ws](nations::state_instance& s) {
		if(s.owner)
			add_item(ws.w.nation_s.state_arrays, s.owner->member_states, nations::region_state_pair{ s.region_id, &s });
	});

	ws.w.population_s.pops.for_each([&ws](population::pop& p) {
		if(is_valid_index(p.associated_army)) {
			auto& a = ws.w.military_s.armies[p.associated_army];
			add_item(ws.w.population_s.pop_arrays, a.backing_pops, p.id);
			a.total_soldiers += uint32_t(ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag));
		}
		if(is_valid_index(p.rebel_faction)) {
			auto& rf = ws.w.population_s.rebel_factions[p.rebel_faction];
			add_item(ws.w.population_s.pop_arrays, rf.member_pops, p.id);
		}
		if(is_valid_index(p.movement)) {
			auto& m = ws.w.population_s.pop_movements[p.movement];
			add_item(ws.w.population_s.pop_arrays, m.member_pops, p.id);
			m.total_population_support += ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);
		}
		if(is_valid_index(p.location))
			add_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[p.location].pops, p.id);
	});

	provinces::update_province_demographics(ws);
	nations::update_state_nation_demographics(ws);

	ws.w.nation_s.nations.for_each([&ws](nations::nation& n) {
		technologies::restore_technologies(ws, n);
		military::update_at_war_with_and_against(ws, n);

		n.ruling_ideology = ws.s.governments_m.parties[n.ruling_party].ideology;

		nations::update_neighbors(ws, n);
		nations::update_province_counts(ws, n);
		nations::update_movement_support(ws, n);

		military::rebuild_fleet_presence(ws, n);

		modifiers::reset_national_modifier(ws, n);

		n.military_score = int16_t(nations::calculate_military_score(ws, n));
		n.industrial_score = int16_t(nations::calculate_industrial_score(ws, n));

		auto admin_req = issues::administrative_requirement(ws, n.id);
		auto member_states = get_range(ws.w.nation_s.state_arrays, n.member_states);
		for(auto s = member_states.first; s != member_states.second; ++s)
			s->state->administrative_efficiency = nations::calculate_state_administrative_efficiency(ws, *(s->state), admin_req);
	});

	for(auto &ps : ws.w.province_s.province_state_container)
		modifiers::reset_provincial_modifier(ws, ps);

	nations::update_nation_ranks(ws);
}
