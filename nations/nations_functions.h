#pragma once
#include "common\\common.h"
#include "nations.h"
#include "provinces\\provinces.h"

class world_state;

namespace nations {
	void init_nations_state(world_state& ws);
	void reset_state(nations_state& s);
	state_tag make_state(provinces::state_tag region, world_state& ws);
	void remove_province_from_state(world_state& ws, provinces::province_tag p);
	nations::country_tag make_nation_for_tag(world_state& ws, cultures::national_tag nt);
	bool is_state_empty(world_state const& ws, nations::state_tag s);
	bool is_colonial_or_protectorate(world_state const& ws, state_tag s);
	void recalculate_state_nation_demographics(world_state& ws);
	void update_state_nation_demographics(world_state& ws); // update last population with current, then recalculates
	provinces::province_tag find_best_capital(world_state const& ws, nations::country_tag owner);
	void fix_capitals(world_state& ws);
	provinces::province_tag get_state_capital(world_state const& ws, nations::state_tag s);
	provinces::province_tag find_state_capital(world_state const& ws, nations::state_tag s);
	provinces::province_tag state_port_province(world_state const& ws, nations::state_tag s);
	void set_relationship(world_state& ws, nations::country_tag a, nations::country_tag b, int32_t value);
	int32_t get_relationship(world_state const& ws, nations::country_tag a, country_tag b);
	void adjust_relationship(world_state& ws, nations::country_tag a, nations::country_tag b, int32_t value);
	nations::influence get_influence(world_state const& ws, nations::country_tag nation_by, country_tag nation_over);
	int32_t get_influence_value(world_state const& ws, nations::country_tag nation_by, country_tag nation_over);
	int32_t get_influence_level(world_state const& ws, nations::country_tag nation_by, country_tag nation_over);
	void set_influence(world_state& ws, nations::country_tag nation_by, country_tag nation_over, int32_t value, int32_t level); // will not remove from some else's sphere
	void set_influence_value(world_state& ws, nations::country_tag nation_by, country_tag nation_over, int32_t value);
	float get_foreign_investment(world_state const& ws, nations::country_tag nation_by, country_tag nation_in);
	void set_foreign_investment(world_state& ws, nations::country_tag nation_by, country_tag nation_in, float value);
	void remove_investment_and_influence(world_state& ws, nations::country_tag nation_by, nations::country_tag nation_target);
	void init_empty_states(world_state& ws);
	void reset_nation(world_state& ws, nations::country_tag new_nation);
	void change_tag(world_state& ws, nations::country_tag this_nation, cultures::national_tag new_tag);
	void annex_nation(world_state& ws, nations::country_tag this_nation, nations::country_tag to_annex);
	nations::country_tag liberate_uncored_cores(world_state& ws, nations::country_tag from, cultures::national_tag); // returns new nation
	nations::country_tag liberate_all_cores(world_state& ws, nations::country_tag from, cultures::national_tag); // returns new nation
	void make_vassal(world_state& ws, nations::country_tag overlord, nations::country_tag vassal);
	void free_vassal(world_state& ws, nations::country_tag vassal);
	text_data::text_tag influence_level_to_text(world_state const& ws, int32_t i);
	void adjust_influence(world_state& ws, nations::country_tag nation_by, nations::country_tag nation_target, int32_t amount); // will also adjust influence level on overflow / underflow
	void remove_owned_province(world_state& ws, nations::country_tag n, provinces::province_tag p); // removes province from list of owned
	void remove_controlled_province(world_state& ws, nations::country_tag n, provinces::province_tag p); // removes province from list of controlled
	void add_owned_province(world_state& ws, nations::country_tag n, provinces::province_tag p); // add province to list of owned
	void add_controlled_province(world_state& ws, nations::country_tag n, provinces::province_tag p); // add province to list of controlled
	void destroy_state_instance(world_state& ws, nations::state_tag si);
	void partial_destroy_state_instance(world_state& ws, nations::state_tag si);
	void destroy_nation(world_state& ws, nations::country_tag new_nation);
	float tarrif_multiplier(world_state const& ws, nations::country_tag source, nations::country_tag target);

	int32_t colonial_points_to_make_protectorate(world_state const& ws, state_tag si);
	int32_t colonial_points_to_make_colony(world_state const& ws, state_tag si);
	int32_t colonial_points_to_make_state(world_state const& ws, state_tag si);
	int32_t free_colonial_points(world_state const& ws, nations::country_tag n);
	int32_t points_for_next_colonial_stage(world_state const& ws, nations::country_tag n, state_tag si);

	text_data::text_tag get_nation_status_text(world_state const& ws, nations::country_tag this_nation);
	bool is_great_power(world_state const& ws, nations::country_tag this_nation);

	void silent_make_alliance(world_state& ws, nations::country_tag a, nations::country_tag b);
	void make_alliance(world_state& ws, nations::country_tag a, nations::country_tag b);
	void end_alliance(world_state& ws, nations::country_tag a, nations::country_tag b);
	void simple_make_vassal(world_state& ws, nations::country_tag overlord, nations::country_tag vassal);
	void simple_make_substate(world_state& ws, nations::country_tag overlord, nations::country_tag vassal);
	nations::country_tag union_holder_of(world_state const& ws, nations::country_tag this_nation);
	nations::country_tag union_holder_for(world_state const& ws, cultures::culture_tag);
	cultures::national_tag union_tag_of(world_state const& ws, nations::country_tag this_nation);

	economy::goods_qnty_type national_treasury(world_state const& ws, country_tag id);
	float fraction_of_cores_owned(world_state const& ws, nations::country_tag this_nation);
	bool can_release_as_vassal(world_state const& ws, nations::country_tag this_nation, cultures::national_tag vassal);  // checks for target already existing, allowed by scenario, & core owned by nation

	float get_prestige(world_state const& ws, nations::country_tag n);
	int32_t get_colonial_points(world_state const& ws, nations::country_tag n);
	float calculate_state_administrative_efficiency(world_state const& ws, state_tag si, float admin_requirement);
	float calculate_national_administrative_efficiency(world_state const& ws, nations::country_tag n);
	float calculate_revanchism(world_state const& ws, nations::country_tag n);
	void update_neighbors(world_state& ws, nations::country_tag this_nation);

	uint32_t calculate_blockaded_count(world_state const& ws, nations::country_tag this_nation);
	uint32_t calculate_crime_count(world_state const& ws, nations::country_tag this_nation);
	uint32_t calculate_rebel_controlled_count(world_state const& ws, nations::country_tag this_nation);

	void update_province_counts(world_state& ws, nations::country_tag this_nation); // marks provinces, updates count of central provinces, blockaded, with crime, rebel controlled, ports
	void update_movement_support(world_state& ws, nations::country_tag this_nation);

	int32_t calculate_industrial_score(world_state const& ws, nations::country_tag this_nation);
	int32_t calculate_military_score(world_state const& ws, nations::country_tag this_nation);

	void update_nation_ranks(world_state& ws);
	void civilize_nation(world_state& ws, nations::country_tag this_nation);
	void uncivilize_nation(world_state& ws, nations::country_tag this_nation);
	void perform_nationalization(world_state& ws, nations::country_tag this_nation);

	void make_slave_state(world_state& ws, state_tag this_state);
	void unmake_slave_state(world_state& ws, state_tag this_state);

	bool are_states_physically_neighbors(world_state const& ws, state_tag a, state_tag b);
	nations::country_tag state_owner(world_state const& ws, nations::state_tag s);

	template<typename F>
	void for_each_province(world_state const& ws, state_tag s, F&& f);
	template<typename F>
	void for_each_province(world_state const& ws, nations::country_tag s, F&& f);
	template<typename F>
	void for_each_state(world_state const& ws, nations::country_tag s, F&& f);
	template<typename F>
	void for_each_pop(world_state const& ws, state_tag s, F&& f);
	template<typename F>
	void for_each_pop(world_state const& ws, nations::country_tag s, F&& f);

	template<typename F>
	void for_each_province(world_state& ws, state_tag s, F&& f);
	template<typename F>
	void for_each_province(world_state& ws, nations::country_tag s, F&& f);
	template<typename F>
	void for_each_state(world_state& ws, nations::country_tag s, F&& f);
	template<typename F>
	void for_each_pop(world_state& ws, state_tag s, F&& f);
	template<typename F>
	void for_each_pop(world_state& ws, nations::country_tag s, F&& f);
}
