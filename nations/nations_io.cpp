#include "nations_io.h"
#include "world_state\\world_state.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "population\\population_function.h"
#include "nations_functions.h"
#include "governments\\governments_functions.h"
#include "military\\military_io.h"
#include "nations\\nations_functions.h"
#include "technologies\\technologies_functions.h"
#include "military\\military_functions.h"
#include "issues\\issues_functions.h"
#include "economy\\economy_io.h"

#undef max
#undef min


void serialization::serializer<nations::state_instance>::serialize_object(std::byte *& output, nations::state_instance const & obj, world_state const& ws) {
	auto owner_id = obj.owner ? obj.owner->id : nations::country_tag();
	serialize(output, owner_id);

	auto nat_focus_id = obj.owner_national_focus ? obj.owner_national_focus->id : modifiers::national_focus_tag();
	serialize(output, nat_focus_id);

	serialize(output, obj.project);

	for(uint32_t i = 0; i < std::extent_v<decltype(obj.factories)>; ++i) {
		serialize(output, obj.factories[i], ws);
	}

	serialize_array(output, obj.colonizers, std::extent_v<decltype(obj.colonizers)>);

	serialize(output, obj.last_population);
	serialize(output, obj.current_tension);
	serialize(output, obj.name);
	serialize(output, obj.crisis_tag);
	serialize(output, obj.region_id);
	serialize(output, obj.flags);

	auto prices = ws.w.nation_s.state_prices.get_row(obj.id);
	serialize_array(output, prices, ws.s.economy_m.aligned_32_goods_count * 2);
	auto production = ws.w.nation_s.state_production.get_row(obj.id);
	serialize_array(output, production, ws.s.economy_m.aligned_32_goods_count * 2);
	auto demand = ws.w.nation_s.state_demand.get_row(obj.id);
	serialize_array(output, demand, ws.s.economy_m.aligned_32_goods_count * 2);

	serialize_stable_array(output, ws.w.nation_s.nations_arrays, obj.flashpoint_tension_focuses);
	serialize_stable_array(output, ws.w.nation_s.state_neighbor_arrays, obj.neighbors);
	serialize_stable_array(output, ws.w.nation_s.state_goods_arrays, obj.production_imports_arrays);
}

void serialization::serializer<nations::state_instance>::deserialize_object(std::byte const *& input, nations::state_instance & obj, world_state & ws) {
	ws.w.nation_s.state_demographics.ensure_capacity(to_index(obj.id) + 1);
	ws.w.nation_s.state_prices.ensure_capacity(to_index(obj.id) + 1);
	ws.w.nation_s.state_production.ensure_capacity(to_index(obj.id) + 1);
	ws.w.nation_s.state_demand.ensure_capacity(to_index(obj.id) + 1);

	nations::country_tag owner;
	deserialize(input, owner);
	obj.owner = ws.w.nation_s.nations.get_location(owner);

	modifiers::national_focus_tag nat_focus;
	deserialize(input, nat_focus);
	if(is_valid_index(nat_focus))
		obj.owner_national_focus = &(ws.s.modifiers_m.national_focuses[nat_focus]);

	deserialize(input, obj.project);

	for(uint32_t i = 0; i < std::extent_v<decltype(obj.factories)>; ++i) {
		deserialize(input, obj.factories[i], ws);
	}

	deserialize_array(input, obj.colonizers, std::extent_v<decltype(obj.colonizers)>);

	deserialize(input, obj.last_population);
	deserialize(input, obj.current_tension);
	deserialize(input, obj.name);
	deserialize(input, obj.crisis_tag);
	deserialize(input, obj.region_id);
	deserialize(input, obj.flags);

	auto prices = ws.w.nation_s.state_prices.get_row(obj.id);
	deserialize_array(input, prices, ws.s.economy_m.aligned_32_goods_count * 2);
	auto production = ws.w.nation_s.state_production.get_row(obj.id);
	deserialize_array(input, production, ws.s.economy_m.aligned_32_goods_count * 2);
	auto demand = ws.w.nation_s.state_demand.get_row(obj.id);
	deserialize_array(input, demand, ws.s.economy_m.aligned_32_goods_count * 2);

	deserialize_stable_array(input, ws.w.nation_s.nations_arrays, obj.flashpoint_tension_focuses);
	deserialize_stable_array(input, ws.w.nation_s.state_neighbor_arrays, obj.neighbors);
	deserialize_stable_array(input, ws.w.nation_s.state_goods_arrays, obj.production_imports_arrays);
}

size_t serialization::serializer<nations::state_instance>::size(nations::state_instance const & obj, world_state const& ws) {
	return
		sizeof(nations::country_tag) + // owner tag
		sizeof(modifiers::national_focus_tag) + // national focus
		serialize_size(obj.project) +
		std::accumulate(std::begin(obj.factories), std::end(obj.factories), 0ui64, [&ws](size_t v, economy::factory_instance const& f) {
			return v + serialize_size(f, ws);
		}) + // factories
		sizeof(obj.colonizers[0]) * std::extent_v<decltype(obj.colonizers)> + // colonizers
		sizeof(obj.last_population) +
		sizeof(obj.current_tension) +
		sizeof(obj.name) +
		sizeof(obj.crisis_tag) +
		sizeof(obj.region_id) +
		sizeof(obj.flags) +
		sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count * 2 + // state prices
		sizeof(economy::goods_qnty_type) * ws.s.economy_m.aligned_32_goods_count * 2 + // state production
		sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count * 2 + // state demand
		serialize_stable_array_size(ws.w.nation_s.nations_arrays, obj.flashpoint_tension_focuses) +
		serialize_stable_array_size(ws.w.nation_s.state_neighbor_arrays, obj.neighbors) +
		serialize_stable_array_size(ws.w.nation_s.state_goods_arrays, obj.production_imports_arrays)
		;
}

void serialization::serializer<nations::nation>::serialize_object(std::byte *& output, nations::nation const & obj, world_state const& ws) {
	auto overlord_id = obj.overlord ? obj.overlord->id : nations::country_tag();
	serialize(output, overlord_id);

	auto sphere_leader_id = obj.sphere_leader ? obj.sphere_leader->id : nations::country_tag();
	serialize(output, sphere_leader_id);

	serialize(output, obj.last_income);
	serialize(output, obj.last_population);
	serialize(output, obj.last_election);
	serialize(output, obj.last_lost_war);
	serialize(output, obj.disarmed_until);

	serialize(output, obj.plurality);
	serialize(output, obj.revanchism);
	serialize(output, obj.base_prestige);
	serialize(output, obj.infamy);
	serialize(output, obj.war_exhaustion);
	serialize(output, obj.suppression_points);
	serialize(output, obj.diplomacy_points);

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

	serialize(output, obj.research_points);
	serialize(output, obj.leadership_points);
	serialize(output, obj.base_colonial_points);
	serialize(output, obj.current_color);
	serialize(output, obj.current_research);

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
	serialize_stable_array(output, ws.w.military_s.fleet_arrays, obj.fleets);
	serialize_stable_array(output, ws.w.military_s.orders_arrays, obj.active_orders);
	serialize_stable_array(output, ws.w.military_s.cb_arrays, obj.active_cbs);
	serialize_stable_array(output, ws.w.military_s.war_arrays, obj.wars_involved_in);
	serialize_stable_array(output, ws.w.population_s.rebel_faction_arrays, obj.active_rebel_factions);
	serialize_stable_array(output, ws.w.population_s.pop_movement_arrays, obj.active_movements);
}

void serialization::serializer<nations::nation>::deserialize_object(std::byte const *& input, nations::nation & obj, world_state & ws) {
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

	technologies::reset_technologies(ws, obj);
	military::reset_unit_stats(ws, obj.id);

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
		add_item(ws.w.nation_s.nations_arrays, obj.sphere_leader->sphere_members, obj.id);
	}

	deserialize(input, obj.last_income);
	deserialize(input, obj.last_population);
	deserialize(input, obj.last_election);
	deserialize(input, obj.last_lost_war);
	deserialize(input, obj.disarmed_until);

	deserialize(input, obj.plurality);
	deserialize(input, obj.revanchism);
	deserialize(input, obj.base_prestige);
	deserialize(input, obj.infamy);
	deserialize(input, obj.war_exhaustion);
	deserialize(input, obj.suppression_points);
	deserialize(input, obj.diplomacy_points);

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

	deserialize(input, obj.research_points);
	deserialize(input, obj.leadership_points);
	deserialize(input, obj.base_colonial_points);
	deserialize(input, obj.current_color);
	deserialize(input, obj.current_research);

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
	deserialize_stable_array(input, ws.w.military_s.fleet_arrays, obj.fleets);
	deserialize_stable_array(input, ws.w.military_s.orders_arrays, obj.active_orders);
	deserialize_stable_array(input, ws.w.military_s.cb_arrays, obj.active_cbs);
	deserialize_stable_array(input, ws.w.military_s.war_arrays, obj.wars_involved_in);
	deserialize_stable_array(input, ws.w.population_s.rebel_faction_arrays, obj.active_rebel_factions);
	deserialize_stable_array(input, ws.w.population_s.pop_movement_arrays, obj.active_movements);

	governments::update_current_rules(ws, obj);
}

size_t serialization::serializer<nations::nation>::size(nations::nation const & obj, world_state const& ws) {
	return
		sizeof(nations::country_tag) + // overlord id
		sizeof(nations::country_tag) + // sphere leader tag
		sizeof(obj.last_income) +
		sizeof(obj.last_population) +
		sizeof(date_tag) * 3 + // last election, last lost war, disarmed until
		sizeof(float) * 5 + // plurality ... war exhaustion
		serialize_size(obj.suppression_points) +
		serialize_size(obj.diplomacy_points) +
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
		sizeof(obj.research_points) +
		sizeof(obj.leadership_points) +
		sizeof(obj.base_colonial_points) +
		sizeof(obj.current_color) +
		serialize_size(obj.current_research) +
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
		serialize_stable_array_size(ws.w.culture_s.culture_arrays, obj.accepted_cultures) +
		serialize_stable_array_size(ws.w.nation_s.relations_arrays, obj.relations) +
		serialize_stable_array_size(ws.w.nation_s.truce_arrays, obj.truces) +
		serialize_stable_array_size(ws.w.nation_s.loan_arrays, obj.loans) +
		serialize_stable_array_size(ws.w.nation_s.state_tag_arrays, obj.national_focus_locations) +
		serialize_stable_array_size(ws.w.variable_s.national_flags_arrays, obj.national_flags) +
		serialize_stable_array_size(ws.w.nation_s.static_modifier_arrays, obj.static_modifiers) +
		serialize_stable_array_size(ws.w.nation_s.timed_modifier_arrays, obj.timed_modifiers) +
		serialize_stable_array_size(ws.w.military_s.leader_arrays, obj.generals) +
		serialize_stable_array_size(ws.w.military_s.leader_arrays, obj.admirals) +
		serialize_stable_array_size(ws.w.military_s.fleet_arrays, obj.fleets) +
		serialize_stable_array_size(ws.w.military_s.orders_arrays, obj.active_orders) +
		serialize_stable_array_size(ws.w.military_s.cb_arrays, obj.active_cbs) +
		serialize_stable_array_size(ws.w.military_s.war_arrays, obj.wars_involved_in) +
		serialize_stable_array_size(ws.w.population_s.rebel_faction_arrays, obj.active_rebel_factions) +
		serialize_stable_array_size(ws.w.population_s.pop_movement_arrays, obj.active_movements)
		;
}

namespace nations {
	struct relation_parse_obj {
		world_state& ws;
		date_tag start_date;
		date_tag end_date;
		cultures::national_tag first;
		cultures::national_tag second;

		relation_parse_obj(world_state& w, date_tag) : ws(w) {}

		void set_first(token_and_type const& t) {
			first = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end));
		}
		void set_second(token_and_type const& t) {
			second = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end));
		}
	};
	struct relation_file_reader {
		world_state& ws;
		date_tag filter_date;

		relation_file_reader(world_state& w, date_tag d) : ws(w), filter_date(d) {}

		void set_vassal(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				silent_make_vassal(ws, *make_nation_for_tag(ws, r.first), *make_nation_for_tag(ws, r.second));
		}
		void set_substate(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				silent_make_substate(ws, *make_nation_for_tag(ws, r.first), *make_nation_for_tag(ws, r.second));
		}
		void set_alliance(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				silent_make_alliance(ws, *make_nation_for_tag(ws, r.first), *make_nation_for_tag(ws, r.second));
		}
	};

	struct upper_house_parse_obj {
		world_state& ws;
		std::vector<uint32_t> upper_house;

		upper_house_parse_obj(world_state& w, date_tag) : ws(w) {
			upper_house.resize(ws.s.ideologies_m.ideologies_count);
		}
		void set_upper_house_pair(std::pair<token_and_type, uint32_t> const& p) {
			const auto ihandle = tag_from_text(ws.s.ideologies_m.named_ideology_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			upper_house[to_index(ihandle)] = p.second;
		}
	};
	struct investment_parse_obj {
		world_state& ws;
		std::vector<std::pair<cultures::national_tag, uint32_t>> investment;

		investment_parse_obj(world_state& w, date_tag) : ws(w) {}
		void set_investment_pair(std::pair<token_and_type, uint32_t> const& p) {
			const auto ihandle = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(p.first.start, p.first.end));
			investment.emplace_back(ihandle, p.second);
		}
	};

	inline std::pair<token_and_type, uint32_t> make_token_unit32_pair(token_and_type const& t, association_type, token_and_type const& r) {
		return std::pair<token_and_type, uint32_t>(t, token_to<uint32_t>(r));
	}
	inline std::pair<token_and_type, token_and_type> make_token_token_pair(token_and_type const& t, association_type, token_and_type const& r) {
		return std::pair<token_and_type, token_and_type>(t, r);
	}

	struct govt_flag {
		world_state& ws;
		governments::government_tag source;
		governments::government_tag replacement;

		govt_flag(world_state& w, date_tag) : ws(w) {}
		void set_government(token_and_type const& t) {
			source = tag_from_text(ws.s.governments_m.named_government_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_flag(token_and_type const& t) {
			replacement = tag_from_text(ws.s.governments_m.named_government_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
	};

	struct nation_parse_object;
	void combine_nation_parse_object(nation_parse_object& base, nation_parse_object& other);

	struct nation_parse_object {
		world_state& ws;
		provinces::province_tag capital;
		governments::government_tag gov;
		governments::party_tag ruling_party;
		std::vector<events::decision_tag> decisions;
		std::string oob_filename;
		cultures::culture_tag primary_culture;
		std::vector<cultures::culture_tag> accepted_cultures;
		std::vector<cultures::culture_tag> remove_accepted_cultures;
		cultures::religion_tag religion;
		std::vector<variables::national_flag_tag> set_flags;
		std::vector<variables::global_variable_tag> global_flags;
		std::vector<variables::national_flag_tag> clear_flags;
		std::vector<uint32_t> upper_house;
		date_tag last_election;
		std::optional<float> literacy;
		std::optional<float> non_state_culture_literacy;
		std::optional<float> plurality;
		std::optional<float> consciousness;
		std::optional<float> nonstate_consciousness;
		std::optional<float> prestige;
		std::optional<bool> civilized;
		std::optional<bool> is_releasbale_vassal;
		modifiers::national_modifier_tag nationalvalue;
		modifiers::national_modifier_tag techschool;
		std::vector<std::pair<cultures::national_tag, uint32_t>> investment;
		std::vector<std::pair<governments::government_tag, governments::government_tag>> govt_flags; //first = to be replaced, second = with flag of this type
		std::vector<uint64_t> tech_bit_vector;
		std::vector<issues::option_tag> set_options;

		date_tag target_date;

		nation_parse_object(world_state& w, date_tag t) : ws(w), target_date(t) {
			tech_bit_vector.resize(ws.s.technology_m.technologies_container.size() >> 6ui64);
			set_options.resize(ws.s.issues_m.issues_container.size());
		}

		void set_capital(uint16_t v) {
			capital = provinces::province_tag(v);
		}
		void set_oob(token_and_type const& t) {
			oob_filename = std::string(t.start, t.end);
		}
		void set_ruling_party(token_and_type const& t) {
			ruling_party = tag_from_text(ws.s.governments_m.named_party_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_decision(token_and_type const& t) {
			std::string title = std::string(t.start, t.end) + "_title";
			const auto tt_title = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, title.c_str(), title.c_str() + title.length());
			decisions.push_back(tag_from_text(ws.s.event_m.descisions_by_title_index, tt_title));
		}
		void set_primary_culture(token_and_type const& t) {
			primary_culture = tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_culture(token_and_type const& t) {
			accepted_cultures.push_back(tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void remove_culture(token_and_type const& t) {
			const auto chandle = tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
			if(auto f = std::find(accepted_cultures.begin(), accepted_cultures.end(), chandle); f != accepted_cultures.end()) {
				*f = accepted_cultures.back();
				accepted_cultures.pop_back();
			} else {
				remove_accepted_cultures.push_back(chandle);
			}
		}
		void set_religion(token_and_type const& t) {
			religion = tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_flag(token_and_type const& t) {
			set_flags.push_back(tag_from_text(ws.s.variables_m.named_national_flags, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void remove_flag(token_and_type const& t) {
			const auto fhandle = tag_from_text(ws.s.variables_m.named_national_flags, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
			if(auto f = std::find(set_flags.begin(), set_flags.end(), fhandle); f != set_flags.end()) {
				*f = set_flags.back();
				set_flags.pop_back();
			} else {
				clear_flags.push_back(fhandle);
			}
		}
		void set_upper_house(upper_house_parse_obj&& o) {
			upper_house = std::move(o.upper_house);
		}
		void set_last_election(token_and_type const& t) {
			last_election = parse_date(t.start, t.end);
		}
		void set_nationalvalue(token_and_type const& t) {
			nationalvalue = tag_from_text(ws.s.modifiers_m.named_national_modifiers_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_techschool(token_and_type const& t) {
			techschool = tag_from_text(ws.s.modifiers_m.named_national_modifiers_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_government(token_and_type const& t) {
			gov = tag_from_text(ws.s.governments_m.named_government_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_investment(investment_parse_obj&& o) {
			investment = std::move(o.investment);
		}
		void set_govt_flag(govt_flag const& o) {
			govt_flags.emplace_back(o.source, o.replacement);
		}
		void add_global_flag(token_and_type const& t) {
			global_flags.push_back(tag_from_text(ws.s.variables_m.named_global_variables, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void set_other(std::pair<token_and_type, token_and_type> const& p) {
			const auto left_text = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			const auto tech = tag_from_text(ws.s.technology_m.named_technology_index, left_text);
			if(is_valid_index(tech)) {
				bit_vector_set(tech_bit_vector.data(), to_index(tech), is_fixed_token_ci(p.second, "yes") || is_fixed_token_ci(p.second, "1"));
			} else {
				const auto iss = tag_from_text(ws.s.issues_m.named_issue_index, left_text);
				if(is_valid_index(iss)) {
					const auto opt = tag_from_text(ws.s.issues_m.named_option_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, p.second.start, p.second.end));
					if(ws.s.issues_m.options[opt].parent_issue == iss) {
						set_options[to_index(iss)] = opt;
					}
				}
			}
			
		}
		void add_dated_block(std::pair<token_and_type, nation_parse_object>&& p) {
			auto date = parse_date(p.first.start, p.first.end);
			if(date <= target_date)
				combine_nation_parse_object(*this, p.second);
		}
	};

	inline std::pair<token_and_type, nation_parse_object> name_dated_block(token_and_type const& t, association_type, nation_parse_object&& p) {
		return std::pair<token_and_type, nation_parse_object>(t, std::move(p));
	}

	inline void add_data_to_nation(world_state& ws, nation& target_nation, cultures::national_tag_state& nat_tag, cultures::national_tag nat_tag_id, directory const& units_dir, nation_parse_object& npo) {
		nat_tag.capital = npo.capital;
		target_nation.current_government = npo.gov;
		
		target_nation.primary_culture = npo.primary_culture;
		for(auto c : npo.accepted_cultures)
			add_item(ws.w.culture_s.culture_arrays, target_nation.accepted_cultures, c);
		target_nation.national_religion = npo.religion;
		for(auto f : npo.set_flags)
			add_item(ws.w.variable_s.national_flags_arrays, target_nation.national_flags, f);
		for(auto f : npo.global_flags)
			ws.w.variable_s.global_variables[f] = 1.0f;
		if(npo.upper_house.size() != 0) {
			auto uh_row = ws.w.nation_s.upper_house.get_row(target_nation.id);
			for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i)
				uh_row[i] = uint8_t(npo.upper_house[i]);
		}
		target_nation.last_election = npo.last_election;

		auto owned_provs_range = get_range(ws.w.province_s.province_arrays, target_nation.owned_provinces);
		for(auto i = owned_provs_range.first; i != owned_provs_range.second; ++i) {
			auto pops_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*i].pops);
			for(auto j = pops_range.first; j < pops_range.second; ++j) {
				auto& this_pop = ws.w.population_s.pops.get(*j);
				if(population::is_pop_accepted(ws, this_pop, target_nation)) {
					if(npo.literacy)
						population::set_literacy_direct(this_pop, *npo.literacy);
					if(npo.consciousness)
						population::set_consciousness_direct(this_pop, *npo.consciousness);
				} else {
					if(npo.non_state_culture_literacy)
						population::set_literacy_direct(this_pop, *npo.non_state_culture_literacy);
					if(npo.nonstate_consciousness)
						population::set_consciousness_direct(this_pop, *npo.nonstate_consciousness);
				}
			}
		}
		if(npo.plurality)
			target_nation.plurality = *npo.plurality / 100.0f;
		if(npo.prestige)
			target_nation.base_prestige = *npo.prestige;
		if(npo.civilized && *npo.civilized)
			target_nation.flags |= nation::is_civilized;
		if(npo.is_releasbale_vassal)
			nat_tag.is_not_releasable = !(*npo.is_releasbale_vassal);
		target_nation.national_value = npo.nationalvalue;
		target_nation.tech_school = npo.techschool;
		target_nation.current_government = npo.gov;

		target_nation.current_capital = npo.capital;
		for(auto& ip : npo.investment) {
			auto holding_nation = make_nation_for_tag(ws, ip.first);
			set_foreign_investment(ws, target_nation, holding_nation->id, static_cast<float>(ip.second));
		}

		for(auto& ip : npo.govt_flags)
			ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.first) = ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.second);

		auto tech_row = ws.w.nation_s.active_technologies.get_row(target_nation.id);
		for(uint32_t i = 0; i < npo.tech_bit_vector.size(); ++i)
			tech_row[i] = npo.tech_bit_vector[i];

		auto issues_row = ws.w.nation_s.active_issue_options.get_row(target_nation.id);
		for(uint32_t i = 0; i < ws.s.issues_m.issues_container.size(); ++i)
			issues_row[i] = npo.set_options[i];

		if(is_valid_index(npo.ruling_party))
			governments::silent_set_ruling_party(ws, target_nation, npo.ruling_party);
			
		for(uint32_t i = 0; i < npo.oob_filename.length(); ++i) {
			if(npo.oob_filename[i] == '/')
				npo.oob_filename[i] = '\\';
		}
		auto fi = units_dir.open_file(npo.oob_filename.c_str(), npo.oob_filename.c_str() + npo.oob_filename.length());
		if(fi) {
			const auto sz = fi->size();
			std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
			std::vector<token_group> parse_results;

			fi->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(parse_results.size() > 0)
				military::read_oob_file(ws, target_nation, parse_results.data(), parse_results.data() + parse_results.size());
		}
		//not used: decisions
	}

	void combine_nation_parse_object(nation_parse_object& base, nation_parse_object& other) {
		if(is_valid_index(other.capital))
			base.capital = other.capital;
		if(is_valid_index(other.gov))
			base.gov = other.gov;
		if(is_valid_index(other.ruling_party))
			base.ruling_party = other.ruling_party;
		for(auto d : other.decisions) {
			if(std::find(base.decisions.begin(), base.decisions.end(), d) == base.decisions.end())
				base.decisions.push_back(d);
		}
		for(auto f : other.global_flags)
			base.global_flags.push_back(f);
		if(other.oob_filename.length() != 0)
			base.oob_filename = other.oob_filename;
		if(is_valid_index(other.primary_culture))
			base.primary_culture = other.primary_culture;
		for(auto d : other.remove_accepted_cultures) {
			if(auto f = std::find(base.accepted_cultures.begin(), base.accepted_cultures.end(), d); f != base.accepted_cultures.end()) {
				*f = base.accepted_cultures.back();
				base.accepted_cultures.pop_back();
			}
		}
		for(auto d : other.accepted_cultures) {
			if(std::find(base.accepted_cultures.begin(), base.accepted_cultures.end(), d) == base.accepted_cultures.end())
				base.accepted_cultures.push_back(d);
		}
		if(is_valid_index(other.religion))
			base.religion = other.religion;
		for(auto d : other.clear_flags) {
			if(auto f = std::find(base.set_flags.begin(), base.set_flags.end(), d); f != base.set_flags.end()) {
				*f = base.set_flags.back();
				base.set_flags.pop_back();
			}
		}
		for(auto d : other.set_flags) {
			if(std::find(base.set_flags.begin(), base.set_flags.end(), d) == base.set_flags.end())
				base.set_flags.push_back(d);
		}
		if(other.upper_house.size() != 0) {
			for(uint32_t i = 0; i < other.upper_house.size(); ++i)
				base.upper_house[i] = other.upper_house[i];
		}
		if(is_valid_index(other.last_election))
			base.last_election = other.last_election;
		if(other.literacy)
			base.literacy = other.literacy;
		if(other.non_state_culture_literacy)
			base.non_state_culture_literacy = other.non_state_culture_literacy;
		if(other.plurality)
			base.plurality = other.plurality;
		if(other.consciousness)
			base.consciousness = other.consciousness;
		if(other.nonstate_consciousness)
			base.nonstate_consciousness = other.nonstate_consciousness;
		if(other.prestige)
			base.prestige = other.prestige;
		if(other.civilized)
			base.civilized = other.civilized;
		if(other.is_releasbale_vassal)
			base.is_releasbale_vassal = other.is_releasbale_vassal;
		if(is_valid_index(other.nationalvalue))
			base.nationalvalue = other.nationalvalue;
		if(is_valid_index(other.techschool))
			base.techschool = other.techschool;
		if(other.investment.size() != 0) {
			base.investment = std::move(other.investment);
		}
		for(auto o : other.govt_flags) {
			base.govt_flags.push_back(o);
		}
		for(uint32_t i = 0; i < other.tech_bit_vector.size(); ++i) {
			base.tech_bit_vector[i] |= other.tech_bit_vector[i];
		}
		for(uint32_t i = 0; i < other.set_options.size(); ++i) {
			if(is_valid_index(other.set_options[i]))
				base.set_options[i] = other.set_options[i];
		}
	}
}

MEMBER_FDEF(nations::relation_file_reader, set_substate, "substate");
MEMBER_FDEF(nations::relation_file_reader, set_vassal, "vassal");
MEMBER_FDEF(nations::relation_file_reader, set_alliance, "alliance");
MEMBER_DEF(nations::relation_parse_obj, start_date, "start_date");
MEMBER_DEF(nations::relation_parse_obj, end_date, "end_date");
MEMBER_FDEF(nations::relation_parse_obj, set_first, "first");
MEMBER_FDEF(nations::relation_parse_obj, set_second, "second");

MEMBER_FDEF(nations::upper_house_parse_obj, set_upper_house_pair, "upper_house_pair");
MEMBER_FDEF(nations::investment_parse_obj, set_investment_pair, "investment_pair");
MEMBER_FDEF(nations::govt_flag, set_government, "government");
MEMBER_FDEF(nations::govt_flag, set_flag, "flag");
MEMBER_FDEF(nations::nation_parse_object, set_capital, "capital");
MEMBER_FDEF(nations::nation_parse_object, add_decision, "decision");
MEMBER_FDEF(nations::nation_parse_object, set_primary_culture, "primary_culture");
MEMBER_FDEF(nations::nation_parse_object, add_culture, "culture");
MEMBER_FDEF(nations::nation_parse_object, remove_culture, "remove_culture");
MEMBER_FDEF(nations::nation_parse_object, set_religion, "religion");
MEMBER_FDEF(nations::nation_parse_object, add_flag, "add_flag");
MEMBER_FDEF(nations::nation_parse_object, remove_flag, "remove_flag");
MEMBER_FDEF(nations::nation_parse_object, set_upper_house, "upper_house");
MEMBER_FDEF(nations::nation_parse_object, set_last_election, "last_election");
MEMBER_FDEF(nations::nation_parse_object, set_nationalvalue, "nationalvalue");
MEMBER_FDEF(nations::nation_parse_object, set_techschool, "school");
MEMBER_FDEF(nations::nation_parse_object, set_investment, "investment");
MEMBER_FDEF(nations::nation_parse_object, set_govt_flag, "govt_flag");
MEMBER_FDEF(nations::nation_parse_object, add_global_flag, "global_flag");
MEMBER_FDEF(nations::nation_parse_object, set_other, "other");
MEMBER_FDEF(nations::nation_parse_object, set_government, "government");
MEMBER_FDEF(nations::nation_parse_object, add_dated_block, "dated_block");
MEMBER_FDEF(nations::nation_parse_object, set_oob, "oob");
MEMBER_DEF(nations::nation_parse_object, literacy, "literacy");
MEMBER_DEF(nations::nation_parse_object, non_state_culture_literacy, "non_state_culture_literacy");
MEMBER_DEF(nations::nation_parse_object, plurality, "plurality");
MEMBER_DEF(nations::nation_parse_object, consciousness, "consciousness");
MEMBER_DEF(nations::nation_parse_object, nonstate_consciousness, "nonstate_consciousness");
MEMBER_DEF(nations::nation_parse_object, prestige, "prestige");
MEMBER_DEF(nations::nation_parse_object, civilized, "civilized");
MEMBER_FDEF(nations::nation_parse_object, set_ruling_party, "ruling_party");
MEMBER_DEF(nations::nation_parse_object, is_releasbale_vassal, "is_releasbale_vassal");

namespace nations {
	BEGIN_DOMAIN(relation_file)
		BEGIN_TYPE(relation_parse_obj)
			MEMBER_ASSOCIATION("start_date", "start_date", value_from_rh<date_tag>)
			MEMBER_ASSOCIATION("end_date", "end_date", value_from_rh<date_tag>)
			MEMBER_ASSOCIATION("first", "first", token_from_rh)
			MEMBER_ASSOCIATION("second", "second", token_from_rh)
		END_TYPE
		BEGIN_TYPE(relation_file_reader)
			MEMBER_TYPE_ASSOCIATION("alliance", "alliance", relation_parse_obj)
			MEMBER_TYPE_ASSOCIATION("substate", "substate", relation_parse_obj)
			MEMBER_TYPE_ASSOCIATION("vassal", "vassal", relation_parse_obj)
			MEMBER_TYPE_ASSOCIATION("vassal", "union", relation_parse_obj)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(nation_file)
		BEGIN_TYPE(upper_house_parse_obj)
		MEMBER_VARIABLE_ASSOCIATION("upper_house_pair", accept_all, make_token_unit32_pair)
		END_TYPE
		BEGIN_TYPE(investment_parse_obj)
		MEMBER_VARIABLE_ASSOCIATION("investment_pair", accept_all, make_token_unit32_pair)
		END_TYPE
		BEGIN_TYPE(govt_flag)
		MEMBER_ASSOCIATION("government", "government", token_from_rh)
		MEMBER_ASSOCIATION("flag", "flag", token_from_rh)
		END_TYPE
		BEGIN_TYPE(nation_parse_object)
		MEMBER_ASSOCIATION("capital", "capital", value_from_rh<uint16_t>)
		MEMBER_ASSOCIATION("decision", "decision", token_from_rh)
		MEMBER_ASSOCIATION("primary_culture", "primary_culture", token_from_rh)
		MEMBER_ASSOCIATION("government", "government", token_from_rh)
		MEMBER_ASSOCIATION("ruling_party", "ruling_party", token_from_rh)
		MEMBER_ASSOCIATION("culture", "culture", token_from_rh)
		MEMBER_ASSOCIATION("remove_culture", "remove_culture", token_from_rh)
		MEMBER_ASSOCIATION("religion", "religion", token_from_rh)
		MEMBER_ASSOCIATION("add_flag", "set_country_flag", token_from_rh)
		MEMBER_ASSOCIATION("remove_flag", "clr_country_flag", token_from_rh)
		MEMBER_ASSOCIATION("last_election", "last_election", token_from_rh)
		MEMBER_ASSOCIATION("nationalvalue", "nationalvalue", token_from_rh)
		MEMBER_ASSOCIATION("school", "schools", token_from_rh)
		MEMBER_ASSOCIATION("global_flag", "set_global_flag", token_from_rh)
		MEMBER_ASSOCIATION("oob", "oob", token_from_rh)
		MEMBER_TYPE_ASSOCIATION("upper_house", "upper_house", upper_house_parse_obj)
		MEMBER_TYPE_ASSOCIATION("investment", "foreign_investment", investment_parse_obj)
		MEMBER_TYPE_ASSOCIATION("govt_flag", "govt_flag", govt_flag)
		MEMBER_ASSOCIATION("literacy", "literacy", value_from_rh<float>)
		MEMBER_ASSOCIATION("non_state_culture_literacy", "non_state_culture_literacy", value_from_rh<float>)
		MEMBER_ASSOCIATION("plurality", "plurality", value_from_rh<float>)
		MEMBER_ASSOCIATION("consciousness", "consciousness", value_from_rh<float>)
		MEMBER_ASSOCIATION("nonstate_consciousness", "nonstate_consciousness", value_from_rh<float>)
		MEMBER_ASSOCIATION("prestige", "prestige", value_from_rh<float>)
		MEMBER_ASSOCIATION("civilized", "civilized", value_from_rh<bool>)
		MEMBER_ASSOCIATION("is_releasbale_vassal", "is_releasbale_vassal", value_from_rh<bool>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("dated_block", accept_all, nation_parse_object, name_dated_block)
		MEMBER_VARIABLE_ASSOCIATION("other", accept_all, make_token_token_pair)
		END_TYPE
	END_DOMAIN;

	void read_nation_file(world_state& ws, cultures::national_tag nat_tag_id, date_tag target_date, file const& fi, directory const& units_dir, std::vector<std::pair<country_tag, events::decision_tag>>& decisions) {
		const auto sz = fi.size();
		std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
		std::vector<token_group> parse_results;

		fi.read_to_buffer(parse_data.get(), sz);
		parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

		if(parse_results.size() != 0) {
			nation* this_nation = make_nation_for_tag(ws, nat_tag_id);
			auto npo = parse_object<nation_parse_object, nation_file>(parse_results.data(), parse_results.data() + parse_results.size(), ws, target_date);
			add_data_to_nation(ws, *this_nation, ws.w.culture_s.national_tags_state[nat_tag_id], nat_tag_id, units_dir, npo);
			for(auto d : npo.decisions)
				decisions.emplace_back(this_nation->id, d);
		}
	}

	void read_nations_files(world_state& ws, date_tag target_date, directory const& root, std::vector<std::pair<country_tag, events::decision_tag>>& decisions) {
		auto history_dir = root.get_directory(u"\\history");
		auto countries_dir = history_dir.get_directory(u"\\countries");
		auto units_dir = history_dir.get_directory(u"\\units");

		auto countries_files = countries_dir.list_files(u".txt");
		for(auto f : countries_files) {
			auto fname = f.file_name();
			if(fname.length() >= 3) {
				char tag_buffer[3] = { '\0' };

				tag_buffer[0] = char(fname[0]);
				tag_buffer[1] = char(fname[1]);
				tag_buffer[2] = char(fname[2]);

				if(tag_buffer[0] != 'R' || tag_buffer[1] != 'E' || tag_buffer[2] != 'B') {
					auto nation_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(tag_buffer, tag_buffer + 3));
					auto open_file = f.open_file();
					if(is_valid_index(nation_tag) && open_file) {
						read_nation_file(ws, nation_tag, target_date, *open_file, units_dir, decisions);
					}
				}

			}
		}
		set_default_tech_school(ws);
	}

	void read_diplomacy_files(world_state& ws, date_tag target_date, directory const& root) {
		auto history_dir = root.get_directory(u"\\history");
		auto diplomacy_dir = history_dir.get_directory(u"\\diplomacy");

		auto dip_files = diplomacy_dir.list_files(u".txt");
		for(auto f : dip_files) {
			if(auto open_file = f.open_file(); open_file) {
				const auto sz = open_file->size();
				std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
				std::vector<token_group> parse_results;

				open_file->read_to_buffer(parse_data.get(), sz);
				parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

				parse_object<relation_file_reader, relation_file>(parse_results.data(), parse_results.data() + parse_results.size(), ws, target_date);
			}
		}
	}
	void set_default_tech_school(world_state & ws) {
		auto mod_thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "traditional_academic");
		auto nmod = ws.s.modifiers_m.named_national_modifiers_index[mod_thandle];
		ws.w.nation_s.nations.for_each([nmod](nations::nation& n) {
			if(!is_valid_index(n.tech_school))
				n.tech_school = nmod;
		});
	}
}
