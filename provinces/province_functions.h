#pragma once
#include "common\\common.h"
#include "provinces.h"

namespace current_state {
	class state;
}
class world_state;

namespace provinces {
	void add_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	void remove_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	bool province_has_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	void init_province_state(world_state& ws);
	void update_province_demographics(world_state& ws);
	void add_province_modifier(world_state& ws, province_tag p, modifiers::provincial_modifier_tag t);
	void add_timed_province_modifier(world_state& ws, province_tag p, modifiers::provincial_modifier_tag t, date_tag d);
	nations::country_tag get_province_seiger(world_state& ws, province_tag p);
	float get_life_rating(world_state const& ws, province_tag p);
	void reset_state(provinces_state& s);

	template<typename F>
	void for_each_pop(world_state const& ws, province_tag p, F&& f);

	void silent_remove_province_owner(world_state& ws, province_tag p);
	void silent_remove_province_controller(world_state& ws, province_tag p);
	void silent_set_province_owner(world_state& ws, nations::country_tag new_owner, province_tag p);
	void silent_set_province_controller(world_state& ws, nations::country_tag new_controller, province_tag p);
	void silent_on_conquer_province(world_state& ws, province_tag p);
	void enable_canal(world_state& ws, uint32_t canal_id);
	double distance(world_state const& ws, province_tag a, province_tag b); // in km
	void path_wise_distance_cost(world_state const& ws, province_tag a, float* results, province_tag* p_results); // in ~km
	void fill_distance_arrays(world_state& ws);
	nations::country_tag province_owner(world_state const& ws, province_tag p);
	nations::country_tag province_controller(world_state const& ws, province_tag p);
	nations::state_tag province_state(world_state const& ws, province_tag p);
}
