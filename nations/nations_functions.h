#pragma once
#include "common\\common.h"
#include "nations.h"
#include "provinces\\provinces.h"

namespace current_state {
	class state;
}
class world_state;


namespace nations {
	void silent_remove_province_owner(world_state& ws, nation* owner, provinces::province_tag prov);
	void silent_remove_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov);
	void silent_set_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov);
	void destroy_state_instance(world_state& ws, state_instance& si);
	void remove_province_from_state(world_state& ws, provinces::province_state& p);
	void silent_set_province_owner(world_state& ws, nation* owner, provinces::province_tag prov);
	nation* make_nation_for_tag(world_state& ws, cultures::national_tag nt);
	bool is_state_empty(world_state& ws, nation* owner, provinces::state_tag region);
	void init_nations_state(world_state& ws);
	void update_state_nation_demographics(world_state& ws);
	provinces::province_tag find_best_capital(world_state& ws, nation& owner);
	void set_relationship(world_state& ws, nation& a, nation& b, int32_t value);
	int32_t get_relationship(world_state& ws, nation& a, country_tag b);
	int32_t get_influence_value(world_state& ws, nation& a, country_tag b);
	int32_t get_influence_level(world_state& ws, nation& a, country_tag b);
	void set_influence(world_state& ws, nation& a, country_tag b, int32_t value, int32_t level);
	float get_foreign_investment(world_state& ws, nation& a, country_tag b);
	void set_foreign_investment(world_state& ws, nation& a, country_tag b, float value);
	void init_empty_states(world_state& ws);

	int32_t colonial_points_to_make_protectorate(world_state& ws, state_instance& si);
	int32_t colonial_points_to_make_colony(world_state& ws, state_instance& si);
	int32_t colonial_points_to_make_state(world_state& ws, state_instance& si);
	int32_t free_colonial_points(world_state& ws, nation& n);
	int32_t points_for_next_colonial_stage(world_state& ws, nation& n, state_instance& si);

	text_data::text_tag get_nation_status_text(world_state& ws, nation const& this_nation);
	bool is_great_power(world_state& ws, nation const& this_nation);

	void silent_make_alliance(world_state& ws, nation& a, nation& b);
	void silent_make_vassal(world_state& ws, nation& overlord, nation& vassal);
	void silent_make_substate(world_state& ws, nation& overlord, nation& vassal);
	nation* union_holder_of(world_state& ws, nation const& this_nation);
	nation* union_holder_for(world_state& ws, cultures::culture_tag);
	cultures::national_tag union_tag_of(world_state& ws, nation const& this_nation);

	economy::goods_qnty_type national_treasury(world_state const& ws, country_tag id);
	float fraction_of_cores_owned(world_state const& ws, nation const& this_nation);
}
