#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "world_state\\world_state.h"

namespace provinces {
	void add_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	void remove_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	bool province_has_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	void init_ready_provinces(world_state& ws);
	void update_province_demographics(world_state& ws);
	void add_province_modifier(world_state& ws, province_state& p, modifiers::provincial_modifier_tag t);
	void add_timed_province_modifier(world_state& ws, province_state& p, modifiers::provincial_modifier_tag t, date_tag d);
	nations::country_tag get_province_seiger(world_state& ws, province_state& p);
	int32_t get_province_rgo_employment_max(world_state& ws, province_state& p);
	float get_life_rating(province_state const& p);
}
