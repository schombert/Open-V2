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

namespace military {
	void init_military_state(world_state& ws);
	military_leader& make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general);
	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date);
	void calculate_leader_traits(world_state& ws, military_leader& l);
	army& make_army(world_state& ws, nations::nation& n, provinces::province_tag location);
	fleet& make_fleet(world_state& ws, nations::nation& n, provinces::province_tag location);
	void immediate_add_pop_to_army(world_state& ws, army& target_army, population::pop& p);
	void list_opposing_countries(world_state& ws, nations::nation& this_nation, boost::container::small_vector<nations::country_tag, 32, concurrent_allocator<nations::country_tag>>& result);
	bool can_make_or_use_cb_against(world_state const& ws, nations::nation const& nation_by, nations::nation const& nation_target);
	bool has_military_access_with(world_state const& ws, nations::nation const& nation_by, nations::nation const& nation_target);
	bool in_war_with(world_state const& ws, nations::nation const& this_nation, nations::country_tag nation_with); // with = on the same side as
	bool in_war_against(world_state const& ws, nations::nation const& this_nation, nations::country_tag nation_against); // against = on opposing side of
	bool has_units_in_province(world_state const& ws, nations::nation const& this_nation, provinces::province_state const& this_province);
	bool province_is_contested(world_state const& ws, provinces::province_state const& this_province);
	bool has_named_leader(world_state const& ws, nations::nation const& n, text_data::text_tag t);
	uint32_t total_active_divisions(world_state const& ws, nations::nation const& this_nation);
	uint32_t total_active_ships(world_state const& ws, nations::nation const& this_nation);
	uint32_t total_units_in_province(world_state const& ws, provinces::province_state const& this_province);
	float recruited_pop_fraction(world_state const& ws, nations::nation const& n);
}
