#pragma once
#include "common\\common.h"
#include "modifiers.h"
#include "triggers\\triggers.h"

class world_state;

namespace modifiers {
	void add_unique_static_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod);
	void add_static_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod);
	void add_static_modifier_to_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod);
	void add_timed_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod, date_tag expiration);
	void add_unique_timed_modifier_to_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod, date_tag expiration);
	void add_timed_modifier_to_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod, date_tag expiration);
	void remove_static_modifier_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod);
	void remove_static_modifier_from_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod);
	void remove_timed_modifier_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod, date_tag expiration);
	void remove_timed_modifier_from_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod, date_tag expiration);
	void remove_all_static_modifiers_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod);
	void remove_all_timed_modifiers_from_nation(world_state& ws, nations::country_tag this_nation, national_modifier_tag mod);
	void remove_all_timed_modifiers_from_province(world_state& ws, provinces::province_tag this_province, provincial_modifier_tag mod);

	bool has_provincial_modifier(world_state const& ws, provinces::province_tag this_province, provincial_modifier_tag mod);
	bool has_national_modifier(world_state const& ws, nations::country_tag this_nation, national_modifier_tag mod);

	void detach_province_modifiers(world_state& ws, provinces::province_tag this_province, nations::country_tag nation_from);
	void attach_province_modifiers(world_state& ws, provinces::province_tag this_province, nations::country_tag nation_to);

	void set_default_crimes(world_state& ws, nations::country_tag this_nation);

	//void reset_national_modifier(world_state& ws, nations::country_tag);
	//void reset_provincial_modifier(world_state& ws, provinces::province_tag);
	void reset_national_modifiers(world_state& ws);
	void reset_provincial_modifiers(world_state& ws);
	
	float test_multiplicative_factor(factor_tag t, world_state const& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot);
	float test_multiplicative_factor(factor_modifier const& f, world_state const& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot);
	float test_additive_factor(factor_tag t, world_state const& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot);
	float test_additive_factor(factor_modifier const& f, world_state const& ws, triggers::const_parameter primary_slot, triggers::const_parameter from_slot);

	ve::fp_vector test_contiguous_multiplicative_factor(factor_tag t, world_state const& ws, ve::contiguous_tags_base<union_tag> primary_offset, ve::contiguous_tags_base<union_tag> from_offset);
	ve::fp_vector test_contiguous_additive_factor(factor_tag t, world_state const& ws, ve::contiguous_tags_base<union_tag> primary_offset, ve::contiguous_tags_base<union_tag> from_offset);

	int32_t maximum_national_focuses(world_state const& ws, nations::country_tag this_nation);
	float extract_value_from_definition(int32_t offset, modifier_definition const& def);
}
