#pragma once
#include "common\\common.h"
#include "population.h"

class world_state;
namespace nations {
	struct nation;
}

namespace population {
	void init_population_state(world_state& ws);
	void reset_state(population_state& s);
	void init_rebel_faction_from_rebel_type(rebel_faction&, rebel_type&);
	pop& make_new_pop(world_state& ws);
	void init_pop_demographics(world_state& ws, pop& p, int32_t size);
	bool is_pop_accepted(world_state& ws, pop& p, nations::nation& n);
	pop* get_unassigned_soldier_in_province(world_state& ws, provinces::province_tag prov);
	void set_militancy_direct(pop &p, float v);
	void set_literacy_direct(pop &p, float v);
	void set_consciousness_direct(pop &p, float v);
	float get_militancy_direct(pop const& p);
	float get_literacy_direct(pop const& p);
	float get_consciousness_direct(pop const& p);
	economy::money_qnty_type desired_needs_spending(world_state& ws, pop const& p);
	nations::nation* get_pop_owner(world_state const& ws, pop const& p);
	bool is_dominant_issue(world_state const& ws, pop_tag id, issues::option_tag opt);
	bool is_dominant_ideology(world_state const& ws, pop_tag id, ideologies::ideology_tag opt);
	void destroy_pop_movement(world_state& ws, pop_movement& m);
	void destroy_rebel_faction(world_state& ws, rebel_faction& r);
	void change_pop_type(world_state& ws, pop& this_pop, pop_type_tag new_type);
	void change_pop_size(world_state& ws, pop& this_pop, int32_t new_size);
	void change_pop_location(world_state& ws, pop& this_pop, provinces::province_tag new_location);
	void remove_pop_from_province(world_state& ws, pop& this_pop);
	void free_slave(world_state& ws, pop& this_pop);
	void trigger_rising(world_state& ws, rebel_faction& faction, nations::nation& in_nation);
	void default_initialize_issues_and_ideology(world_state& ws, pop& this_pop);
	void default_initialize_world_issues_and_ideology(world_state& ws);
	int32_t total_size_change(pop const& this_pop);
}
