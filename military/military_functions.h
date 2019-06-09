#pragma once
#include "common\\common.h"
#include "military.h"
#include "concurrency_tools\\ve.h"

class world_state;

namespace military {
	void init_military_state(world_state& ws);
	void reset_state(military_state& s);
	leader_tag make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general);
	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date);
	void calculate_leader_traits(world_state& ws, leader_tag l);
	army_tag make_army(world_state& ws, nations::country_tag n, provinces::province_tag location);
	fleet_tag make_fleet(world_state& ws, nations::country_tag n, provinces::province_tag location);
	void immediate_add_pop_to_army(world_state& ws, army_tag target_army, population::pop_tag p);
	bool can_use_cb_against(world_state const& ws, nations::country_tag nation_by, nations::country_tag nation_target);
	bool is_cb_construction_valid_against(world_state const& ws, cb_type_tag cb, nations::country_tag nation_by, nations::country_tag nation_target);
	float base_cb_infamy(world_state const& ws, cb_type_tag cb);

	template<typename T, typename U>
	auto has_military_access_with(world_state const& ws, T nation_by, U nation_target) -> decltype(ve::widen_to<T, U>(true));
	template<typename T>
	auto province_is_contested(world_state const& ws, T this_province) -> decltype(ve::widen_to<T>(true));

	bool in_war_with(world_state const& ws, nations::country_tag this_nation, nations::country_tag nation_with); // with = on the same side as
	bool in_war_against(world_state const& ws, nations::country_tag this_nation, nations::country_tag nation_against); // against = on opposing side of
	void update_at_war_with_and_against(world_state& ws, nations::country_tag this_nation);

	void update_army_attributes(world_state& ws, nations::country_tag owning_nation, army_tag this_army);
	void update_fleet_attributes(world_state& ws, nations::country_tag owning_nation, fleet_tag this_fleet);
	void update_all_unit_attributes(world_state& ws, nations::country_tag owning_nation);
	void reset_unit_stats(world_state& ws, nations::country_tag nation_for);

	bool has_units_in_province(world_state const& ws, nations::country_tag this_nation, provinces::province_tag this_province);
	
	bool has_named_leader(world_state const& ws, nations::country_tag n, text_data::text_tag t);
	uint32_t total_active_divisions(world_state const& ws, nations::country_tag this_nation);
	uint32_t total_active_ships(world_state const& ws, nations::country_tag this_nation);
	uint32_t total_units_in_province(world_state const& ws, provinces::province_tag this_province);
	float recruited_pop_fraction(world_state const& ws, nations::country_tag n);
	float get_warscore(world_state const& ws, war_tag this_war); // if attacker is winning, warscore is positive, negative if defender is winning

	bool is_target_of_war_goal(world_state const& ws, war_tag this_war, nations::country_tag target);
	void silent_remove_from_war(world_state& ws, war_tag this_war, nations::country_tag to_remove); // NOTE: does not affect refence to war in nation object
	void remove_from_war(world_state& ws, war_tag this_war, nations::country_tag to_remove);
	void add_to_war(world_state& ws, war_tag this_war, bool attacker, nations::country_tag to_add);
	war_tag get_war_between(world_state& ws, nations::country_tag a, nations::country_tag b);

	void destroy_army(world_state& ws, army_tag a, nations::country_tag owner);
	void destroy_fleet(world_state& ws, fleet_tag f, nations::country_tag owner);
	void partial_destroy_fleet(world_state& ws, fleet_tag f); // does not remove from nation
	void partial_destroy_army(world_state& ws, army_tag a); // does not remove from orders or nation
	void destroy_orders(world_state& ws, army_orders_tag o, nations::country_tag owner);
	void partial_destroy_orders(world_state& ws, army_orders_tag o); // does not remove armies or from nation
	void destroy_admiral(world_state& ws, leader_tag l, nations::country_tag owner);
	void destroy_general(world_state& ws, leader_tag l, nations::country_tag owner);

	void destroy_war(world_state& ws, war_tag this_war);
	uint32_t calculate_minimum_soldiers(world_state const& ws, army_tag a);
	void rebuild_fleet_presence(world_state& ws, nations::country_tag owner);

	bool is_naval_unit(military::unit_type_tag t, world_state const& ws);
	float calculate_base_war_score_cost(world_state const& ws, war_goal const& wg); // great war modifiers not applied
	float single_state_war_score(world_state const& ws, nations::state_tag si, float owner_total_pop);
	float total_attacker_demands_war_score(world_state const& ws, war_tag w);
	float total_defender_demands_war_score(world_state const& ws, war_tag w);

	war_tag create_war(world_state& ws, nations::country_tag attacker, nations::country_tag defender, bool call_willing_attacker_allies);

	float daily_cb_progress(world_state const& ws, nations::country_tag n, cb_type_tag type);
	void update_cb_construction(world_state& ws);

	void init_player_cb_state(world_state& ws);
	void update_player_cb_state(world_state& ws);
}
