#include "common\\common.h"
#include "world_state_io.h"
#include "cultures\\cultures_io.h"
#include "economy\\economy_io.h"
#include "ideologies\\ideologies_io.h"
#include "nations\\nations_io.hpp"
#include "population\\population_io.h"
#include "provinces\\provinces_io.hpp"
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
#include "technologies\\technologies_io.h"
#include "governments\\governments_functions.h"
#include <ppl.h>

/*
player_net_income_history income_history;
			economy::money_qnty_type collected_poor_tax = 0;
			economy::money_qnty_type collected_middle_tax = 0;
			economy::money_qnty_type collected_rich_tax = 0;
			tagged_vector<array_tag<economy::money_qnty_type>, economy::goods_tag> imports_by_country;
*/

void serialization::serializer<decltype(current_state::state::local_player_data)>::serialize_object(std::byte *& output, decltype(current_state::state::local_player_data) const & obj, world_state const & ws) {
	serialize(output, obj.income_history);
	serialize(output, obj.collected_poor_tax);
	serialize(output, obj.collected_middle_tax);
	serialize(output, obj.collected_rich_tax);
	for(uint32_t i = 1; i < ws.s.economy_m.goods_count; ++i) {
		serialize_stable_array(output, ws.w.economy_s.purchasing_arrays, obj.imports_by_country[economy::goods_tag(economy::goods_tag::value_base_t(i))]);
	}
}

void serialization::serializer<decltype(current_state::state::local_player_data)>::deserialize_object(std::byte const *& input, decltype(current_state::state::local_player_data)& obj, world_state & ws) {
	deserialize(input, obj.income_history);
	deserialize(input, obj.collected_poor_tax);
	deserialize(input, obj.collected_middle_tax);
	deserialize(input, obj.collected_rich_tax);
	for(uint32_t i = 1; i < ws.s.economy_m.goods_count; ++i) {
		deserialize_stable_array(input, ws.w.economy_s.purchasing_arrays, obj.imports_by_country[economy::goods_tag(economy::goods_tag::value_base_t(i))]);
	}
}

size_t serialization::serializer<decltype(current_state::state::local_player_data)>::size(decltype(current_state::state::local_player_data) const & obj, world_state const & ws) {
	return serialize_size(obj.income_history) +
		serialize_size(obj.collected_poor_tax) +
		serialize_size(obj.collected_middle_tax) +
		serialize_size(obj.collected_rich_tax) +
		std::transform_reduce(integer_iterator(1), integer_iterator(ws.s.economy_m.goods_count), 0ui64, std::plus<>(), [&ws, &obj](int32_t i) {
			return serialize_stable_array_size(ws.w.economy_s.purchasing_arrays, obj.imports_by_country[economy::goods_tag(economy::goods_tag::value_base_t(i))]);
		});
}

void serialization::serializer<current_state::crisis_state>::serialize_object(std::byte* &output, current_state::crisis_state const& obj, world_state const& ws) {
	serialize(output, obj.temperature);
	uint8_t ctype = uint8_t(obj.type);
	serialize(output, ctype);

	serialize(output, obj.primary_attacker);
	serialize(output, obj.primary_defender);
	serialize(output, obj.target);
	serialize(output, obj.on_behalf_of);

	serialize(output, obj.state);

	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.attackers);
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.defenders);
	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.interested);
	serialize_stable_array(output, ws.w.military_s.war_goal_arrays, obj.goals);
}
void serialization::serializer<current_state::crisis_state>::deserialize_object(std::byte const* &input, current_state::crisis_state& obj, world_state& ws) {
	deserialize(input, obj.temperature);

	uint8_t ctype = 0ui8;
	deserialize(input, ctype);
	obj.type = current_state::crisis_type(ctype);

	deserialize(input, obj.primary_attacker);
	deserialize(input, obj.primary_defender);
	deserialize(input, obj.target);
	deserialize(input, obj.on_behalf_of);

	deserialize(input, obj.state);

	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.attackers);
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.defenders);
	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.interested);
	deserialize_stable_array(input, ws.w.military_s.war_goal_arrays, obj.goals);
}
size_t serialization::serializer<current_state::crisis_state>::size(current_state::crisis_state const& obj, world_state const& ws) {
	return serialize_size(obj.temperature) +
		sizeof(uint8_t) + // type
		sizeof(nations::country_tag) + // primary attacker
		sizeof(nations::country_tag) + // primary defender
		sizeof(nations::country_tag) + // crisis target
		sizeof(nations::country_tag) + // crisis on behalf of
		serialize_size(obj.state) +
		serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.attackers) +
		serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.defenders) +
		serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.interested) +
		serialize_stable_array_size(ws.w.military_s.war_goal_arrays, obj.goals);
}

void serialization::serializer<current_state::state>::serialize_object(std::byte *& output, current_state::state const & obj, world_state const & ws) {
	serialize(output, obj.province_s, ws);
	serialize(output, obj.culture_s, ws);
	serialize(output, obj.military_s, ws);
	serialize(output, obj.nation_s, ws);
	serialize(output, obj.economy_s, ws);
	serialize(output, obj.population_s, ws);
	serialize(output, obj.variable_s, ws);
	serialize(output, obj.ideology_s, ws);
	serialize(output, obj.technology_s, ws);

	serialize(output, obj.current_crisis, ws);

	serialize(output, obj.current_date);
	serialize(output, obj.great_wars_enabled);
	serialize(output, obj.world_wars_enabled);

	serialize(output, obj.local_player_nation);
	serialize(output, obj.local_player_data, ws);
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
	deserialize(input, obj.technology_s, ws);

	deserialize(input, obj.current_crisis, ws);

	deserialize(input, obj.current_date);
	deserialize(input, obj.great_wars_enabled);
	deserialize(input, obj.world_wars_enabled);

	deserialize(input, obj.local_player_nation);
	deserialize(input, obj.local_player_data, ws);

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
		serialize_size(obj.technology_s, ws) +
		serialize_size(obj.current_crisis, ws) +
		serialize_size(obj.current_date) +
		serialize_size(obj.great_wars_enabled) +
		serialize_size(obj.world_wars_enabled) +
		serialize_size(obj.local_player_data, ws) +
		sizeof(nations::country_tag); // player id
}

void restore_world_state(world_state& ws) {
	for(int32_t i = int32_t(ws.w.culture_s.national_tags_state.size()); i--; ) {
		cultures::national_tag this_tag(static_cast<cultures::national_tag::value_base_t>(i));
		auto& nt = ws.w.culture_s.national_tags_state[this_tag];

		if(nt.holder) {
			ws.w.nation_s.nations.set<nation::tag>(nt.holder, this_tag);
			auto names = ws.s.culture_m.country_names_by_government.get(this_tag, ws.w.nation_s.nations.get<nation::current_government>(nt.holder));

			if(is_valid_index(names.name))
				ws.w.nation_s.nations.set<nation::name>(nt.holder, names.name);
			else
				ws.w.nation_s.nations.set<nation::name>(nt.holder, ws.s.culture_m.national_tags[this_tag].default_name.name);

			if(is_valid_index(names.adjective))
				ws.w.nation_s.nations.set<nation::adjective>(nt.holder, names.adjective);
			else
				ws.w.nation_s.nations.set<nation::adjective>(nt.holder, ws.s.culture_m.national_tags[this_tag].default_name.adjective);

			ws.w.nation_s.nations.set<nation::flag>(nt.holder, ws.w.culture_s.country_flags_by_government.get(this_tag, ws.w.nation_s.nations.get<nation::current_government>(nt.holder)));
		}
	}

	ws.w.province_s.province_state_container.for_each([&ws](provinces::province_tag p) {
		auto& container = ws.w.province_s.province_state_container;
		
		auto cores_range = get_range(ws.w.province_s.core_arrays, container.get<province_state::cores>(p));
		container.set<province_state::has_owner_core>(p, false);

		auto owner = container.get<province_state::owner>(p);
		auto owner_tag = is_valid_index(owner) ? ws.w.nation_s.nations.get<nation::tag>(owner) : cultures::national_tag();

		for(auto c : cores_range) {
			add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[c].core_provinces, p);
			if(owner_tag == c) {
				container.set<province_state::has_owner_core>(p, true);
			}
		}
		if(auto orders = container.get<province_state::orders>(p); is_valid_index(orders))
			add_item(ws.w.province_s.province_arrays, ws.w.military_s.army_orders_container[orders].involved_provinces, p);
		if(is_valid_index(owner))
			add_item(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(owner), p);
		if(auto controller = container.get<province_state::controller>(p); is_valid_index(controller))
			add_item(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::controlled_provinces>(controller), p);
		if(auto rebel_controller = container.get<province_state::rebel_controller>(p); is_valid_index(rebel_controller))
			add_item(ws.w.province_s.province_arrays, ws.w.population_s.rebel_factions[rebel_controller].controlled_provinces, p);
	});

	ws.w.military_s.armies.for_each([&ws](military::army& a) {
		if(a.current_orders)
			add_item(ws.w.military_s.army_arrays, a.current_orders->involved_armies, a.id);
		add_item(ws.w.military_s.army_arrays, ws.w.nation_s.nations.get<nation::armies>(a.owner), a.id);
	});

	ws.w.nation_s.states.for_each([&ws](nations::state_tag s) {
		auto owner_id = ws.w.nation_s.states.get<state::owner>(s);
		if(is_valid_index(owner_id))
			add_item(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(owner_id), nations::region_state_pair{ ws.w.nation_s.states.get<state::region_id>(s), s });
	});
	ws.w.nation_s.states.parallel_for_each([&ws](nations::state_tag s) {
		ws.w.nation_s.states.set<state::state_capital>(s, nations::find_state_capital(ws, s));
	});

	ws.w.population_s.pops.for_each([&ws](population::pop_tag p) {
		if(auto parmy = ws.w.population_s.pops.get<pop::associated_army>(p); is_valid_index(parmy)) {
			auto& a = ws.w.military_s.armies[parmy];
			add_item(ws.w.population_s.pop_arrays, a.backing_pops, p);
			a.total_soldiers += uint32_t(ws.w.population_s.pop_demographics.get(p, population::total_population_tag));
		}
		if(auto rfaction = ws.w.population_s.pops.get<pop::rebel_faction>(p); is_valid_index(rfaction)) {
			auto& rf = ws.w.population_s.rebel_factions[rfaction];
			add_item(ws.w.population_s.pop_arrays, rf.member_pops, p);
		}
		if(auto movement = ws.w.population_s.pops.get<pop::movement>(p); is_valid_index(movement)) {
			auto& m = ws.w.population_s.pop_movements[movement];
			add_item(ws.w.population_s.pop_arrays, m.member_pops, p);
			m.total_population_support += decltype(m.total_population_support)(ws.w.population_s.pop_demographics.get(p, population::total_population_tag));
		}
		if(auto loc = ws.w.population_s.pops.get<pop::location>(p); is_valid_index(loc))
			add_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(loc), p);
	});

	provinces::update_province_demographics(ws);
	nations::update_state_nation_demographics(ws);

	std::fill_n(ws.w.technology_s.discovery_count.data(), ws.s.technology_m.technologies_container.size(), 0);

	ws.w.nation_s.modifier_values.reset();
	ws.w.nation_s.nations.parallel_for_each([&ws](nations::country_tag n) {
		technologies::restore_technologies(ws, n);
		modifiers::reset_national_modifier(ws, n);
	});

	ws.w.nation_s.nations.for_each([&ws](nations::country_tag n) {
		military::update_at_war_with_and_against(ws, n);

		ws.w.nation_s.nations.set<nation::ruling_ideology>(n, ws.s.governments_m.parties[ws.w.nation_s.nations.get<nation::ruling_party>(n)].ideology);
		governments::update_current_rules(ws, n);

		nations::update_neighbors(ws, n);
		nations::update_province_counts(ws, n);
		nations::update_movement_support(ws, n);

		military::rebuild_fleet_presence(ws, n);

		ws.w.nation_s.nations.set<nation::military_score>(n, int16_t(nations::calculate_military_score(ws, n)));
		ws.w.nation_s.nations.set<nation::industrial_score>(n, int16_t(nations::calculate_industrial_score(ws, n)));

		ws.w.nation_s.nations.set<nation::national_administrative_efficiency>(n, nations::calculate_national_administrative_efficiency(ws, n));
		ws.w.nation_s.nations.set<nation::revanchism>(n, nations::calculate_revanchism(ws, n));

		auto admin_req = issues::administrative_requirement(ws, n);
		auto member_states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(n));
		for(auto s = member_states.first; s != member_states.second; ++s)
			ws.w.nation_s.states.set<state::administrative_efficiency>(s->state, nations::calculate_state_administrative_efficiency(ws, s->state, admin_req));
	});

	ws.w.province_s.province_state_container.parallel_for_each([&ws](provinces::province_tag ps) {
		modifiers::reset_provincial_modifier(ws, ps);
	});

	provinces::fill_distance_arrays(ws);
	ws.w.province_s.state_distances.update(ws);

	//restore tarrif masks
	auto state_max = ws.w.nation_s.states.size();
	
	ws.w.nation_s.nations.parallel_for_each([&ws, state_max](nations::country_tag n) {
		auto& tm = ws.w.nation_s.nations.get<nation::statewise_tarrif_mask>(n);
		resize(ws.w.economy_s.purchasing_arrays, tm, state_max);
		auto ptr = get_range(ws.w.economy_s.purchasing_arrays, tm);
		for(int32_t i = 0; i < int32_t(state_max); ++i) {
			nations::state_tag this_state = nations::state_tag(nations::state_tag::value_base_t(i));
			if(auto owner = ws.w.nation_s.states.get<state::owner>(this_state); is_valid_index(owner))
				ptr[this_state] = nations::tarrif_multiplier(ws, n, owner);
		}
	});

	nations::update_nation_ranks(ws);
}
