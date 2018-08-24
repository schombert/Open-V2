#pragma once
#include "common\\common.h"
#include "nations.h"
#include "provinces\\provinces.h"

namespace current_state {
	class state;
}

class world_state;


namespace nations {
	void init_nations_state(world_state& ws);
	void reset_state(nations_state& s);
	void remove_province_from_state(world_state& ws, provinces::province_state& p);
	nation* make_nation_for_tag(world_state& ws, cultures::national_tag nt);
	bool is_state_empty(world_state const& ws, state_instance const& s);
	void update_state_nation_demographics(world_state& ws);
	provinces::province_tag find_best_capital(world_state const& ws, nation const& owner);
	void set_relationship(world_state& ws, nation& a, nation& b, int32_t value);
	int32_t get_relationship(world_state const& ws, nation const& a, country_tag b);
	int32_t get_influence_value(world_state const& ws, nation const& nation_by, country_tag nation_over);
	int32_t get_influence_level(world_state const& ws, nation const& nation_by, country_tag nation_over);
	void set_influence(world_state& ws, nation& nation_by, country_tag nation_over, int32_t value, int32_t level);
	float get_foreign_investment(world_state const& ws, nation const& nation_by, country_tag nation_in);
	void set_foreign_investment(world_state& ws, nation& nation_by, country_tag nation_in, float value);
	void remove_investment_and_influence(world_state& ws, nation& nation_by, nation& nation_target);
	void init_empty_states(world_state& ws);
	void reset_nation(world_state& ws, nations::nation& new_nation);
	void change_tag(world_state& ws, nations::nation& this_nation, cultures::national_tag new_tag);
	void annex_nation(world_state& ws, nations::nation& this_nation, nations::nation& to_annex);
	void liberate_uncored_cores(world_state& ws, nations::nation& from, cultures::national_tag);
	void liberate_all_cores(world_state& ws, nations::nation& from, cultures::national_tag);

	void destroy_state_instance(world_state& ws, state_instance& si);
	void partial_destroy_state_instance(world_state& ws, state_instance& si);
	void destroy_nation(world_state& ws, nations::nation& new_nation);

	int32_t colonial_points_to_make_protectorate(world_state& ws, state_instance& si);
	int32_t colonial_points_to_make_colony(world_state& ws, state_instance& si);
	int32_t colonial_points_to_make_state(world_state& ws, state_instance& si);
	int32_t free_colonial_points(world_state& ws, nation& n);
	int32_t points_for_next_colonial_stage(world_state& ws, nation& n, state_instance& si);

	text_data::text_tag get_nation_status_text(world_state& ws, nation const& this_nation);
	bool is_great_power(world_state const& ws, nation const& this_nation);

	void silent_make_alliance(world_state& ws, nation& a, nation& b);
	void silent_make_vassal(world_state& ws, nation& overlord, nation& vassal);
	void silent_make_substate(world_state& ws, nation& overlord, nation& vassal);
	nation* union_holder_of(world_state& ws, nation const& this_nation);
	nation* union_holder_for(world_state& ws, cultures::culture_tag);
	cultures::national_tag union_tag_of(world_state& ws, nation const& this_nation);

	economy::goods_qnty_type national_treasury(world_state const& ws, country_tag id);
	float fraction_of_cores_owned(world_state const& ws, nation const& this_nation);
	bool can_release_as_vassal(world_state const& ws, nation const& this_nation, cultures::national_tag vassal);  // checks for target already existing, allowed by scenario, & core owned by nation

	float get_prestige(nations::nation const& n);
	int32_t get_colonial_points(nations::nation const& n);
	float calculate_state_administrative_efficiency(world_state const& ws, nations::state_instance& si, float admin_requirement);
	void update_neighbors(world_state& ws, nations::nation& this_nation);

	uint32_t calculate_blockaded_count(world_state const& ws, nations::nation const& this_nation);
	uint32_t calculate_crime_count(world_state const& ws, nations::nation const& this_nation);
	uint32_t calculate_rebel_controlled_count(world_state const& ws, nations::nation const& this_nation);

	void update_province_counts(world_state& ws, nations::nation& this_nation); // marks provinces, updates count of central provinces, blockaded, with crime, rebel controlled, ports
	void update_movement_support(world_state& ws, nations::nation& this_nation);

	int32_t calculate_industrial_score(world_state const& ws, nations::nation const& this_nation);
	int32_t calculate_military_score(world_state const& ws, nations::nation const& this_nation);

	void update_nation_ranks(world_state& ws);

	template<typename F>
	void for_each_province(world_state& ws, nations::state_instance const& s, F&& f);
	template<typename F>
	void for_each_province(world_state& ws, nations::nation const& s, F&& f);
	template<typename F>
	void for_each_state(world_state& ws, nations::nation const& s, F&& f);
	template<typename F>
	void for_each_pop(world_state& ws, nations::state_instance const& s, F&& f);
	template<typename F>
	void for_each_pop(world_state& ws, nations::nation const& s, F&& f);
}
