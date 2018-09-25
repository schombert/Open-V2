#pragma once
#include "common\\common.h"
#include "military.h"

class world_state;

namespace nations {
	struct nation;
}
namespace population {
	struct pop;
}

namespace provinces {
	struct province_state;
}

namespace military {
	void init_military_state(world_state& ws);
	void reset_state(military_state& s);
	military_leader& make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general);
	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date);
	void calculate_leader_traits(world_state& ws, military_leader& l);
	army& make_army(world_state& ws, nations::nation& n, provinces::province_tag location);
	fleet& make_fleet(world_state& ws, nations::nation& n, provinces::province_tag location);
	void immediate_add_pop_to_army(world_state& ws, army& target_army, population::pop& p);
	bool can_use_cb_against(world_state& ws, nations::nation& nation_by, nations::nation& nation_target);
	bool has_military_access_with(world_state const& ws, nations::nation const& nation_by, nations::nation const& nation_target);

	bool in_war_with(world_state const& ws, nations::nation const& this_nation, nations::country_tag nation_with); // with = on the same side as
	bool in_war_against(world_state const& ws, nations::nation const& this_nation, nations::country_tag nation_against); // against = on opposing side of
	void update_at_war_with_and_against(world_state& ws, nations::nation& this_nation);

	void update_army_attributes(world_state& ws, nations::nation const& owning_nation, army& this_army);
	void update_fleet_attributes(world_state& ws, nations::nation const& owning_nation, fleet& this_fleet);
	void update_all_unit_attributes(world_state& ws, nations::nation const& owning_nation);
	void reset_unit_stats(world_state& ws, nations::country_tag nation_for);

	bool has_units_in_province(world_state const& ws, nations::nation const& this_nation, provinces::province_state const& this_province);
	bool province_is_contested(world_state const& ws, provinces::province_state const& this_province);
	bool has_named_leader(world_state const& ws, nations::nation const& n, text_data::text_tag t);
	uint32_t total_active_divisions(world_state const& ws, nations::nation const& this_nation);
	uint32_t total_active_ships(world_state const& ws, nations::nation const& this_nation);
	uint32_t total_units_in_province(world_state const& ws, provinces::province_state const& this_province);
	float recruited_pop_fraction(world_state const& ws, nations::nation const& n);
	float get_warscore(world_state const& ws, war const& this_war); // if attacker is winning, warscore is positive, negative if defender is winning

	bool is_target_of_war_goal(world_state const& ws, war const& this_war, nations::country_tag target);
	void silent_remove_from_war(world_state& ws, war& this_war, nations::country_tag to_remove); // NOTE: does not affect refence to war in nation object
	void remove_from_war(world_state& ws, war& this_war, nations::country_tag to_remove);
	void add_to_war(world_state& ws, war& this_war, bool attacker, nations::nation& to_add);
	war* get_war_between(world_state& ws, nations::nation& a, nations::country_tag b);

	void destroy_army(world_state& ws, army& a, nations::nation& owner);
	void destroy_fleet(world_state& ws, fleet& f, nations::nation& owner);
	void partial_destroy_fleet(world_state& ws, fleet& f); // does not remove from nation
	void partial_destroy_army(world_state& ws, army& a); // does not remove from orders or nation
	void destroy_orders(world_state& ws, army_orders& o, nations::nation& owner);
	void partial_destroy_orders(world_state& ws, army_orders& o); // does not remove armies or from nation
	void destroy_admiral(world_state& ws, military_leader& l, nations::nation& owner);
	void destroy_general(world_state& ws, military_leader& l, nations::nation& owner);

	void destroy_war(world_state& ws, war& this_war);
	uint32_t calculate_minimum_soldiers(world_state const& ws, army_tag a);
	void rebuild_fleet_presence(world_state& ws, nations::nation& owner);

	bool is_naval_unit(military::unit_type_tag t, world_state const& ws);

	war& create_war(world_state& ws, nations::nation& attacker, nations::nation& defender, bool call_willing_attacker_allies);
}
