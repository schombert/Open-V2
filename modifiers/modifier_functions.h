#pragma once
#include "common\\common.h"
#include "modifiers.h"

class world_state;

namespace nations {
	struct nation;
}
namespace provinces {
	struct province_state;
}
namespace population {
	struct rebel_faction;
}

namespace modifiers {
	void add_unique_static_modifier_to_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod);
	void add_static_modifier_to_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod);
	void add_static_modifier_to_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod);
	void add_timed_modifier_to_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod, date_tag expiration);
	void add_timed_modifier_to_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod, date_tag expiration);
	void remove_static_modifier_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod);
	void remove_static_modifier_from_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod);
	void remove_timed_modifier_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod, date_tag expiration);
	void remove_timed_modifier_from_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod, date_tag expiration);
	void remove_all_static_modifiers_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod);
	void remove_all_timed_modifiers_from_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod);
	void remove_all_timed_modifiers_from_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod);

	bool has_provincial_modifier(world_state const& ws, provinces::province_state const& this_province, provincial_modifier_tag mod);
	bool has_national_modifier(world_state const& ws, nations::nation const& this_nation, national_modifier_tag mod);

	void detach_province_modifiers(world_state& ws, provinces::province_state& this_province, nations::nation& nation_from);
	void attach_province_modifiers(world_state& ws, provinces::province_state& this_province, nations::nation& nation_to);

	void set_default_crimes(world_state const& ws, nations::nation& this_nation);
	void reset_national_modifier(world_state& ws, nations::nation& this_nation);
	void reset_provincial_modifier(world_state& ws, provinces::province_state& this_province);
	
	float test_multiplicative_factor(factor_tag t, world_state& ws, void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot);
	float test_multiplicative_factor(factor_modifier const& f, world_state& ws, void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot);
	float test_additive_factor(factor_tag t, world_state& ws, void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot);
	float test_additive_factor(factor_modifier const& f, world_state& ws, void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot);
	int32_t maximum_national_focuses(world_state const& ws, nations::nation const& this_nation);
}
