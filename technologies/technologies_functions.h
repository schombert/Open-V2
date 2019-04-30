#pragma once
#include "common\\common.h"
#include "technologies.h"
#include "concurrency_tools\\ve.h"

class world_state;

namespace technologies {
	void init_technology_state(world_state& ws);
	void apply_technology(world_state& ws, nations::country_tag this_nation, tech_tag tech); // does not update stats of existing units or national modifier values
	void apply_single_technology(world_state& ws, nations::country_tag this_nation, tech_tag tech);
	void restore_technologies(world_state& ws, nations::country_tag this_nation); // reset technologies and base unit stats first
	void reset_technologies(world_state& ws, nations::country_tag this_nation);

	template<typename T, typename U = float>
	auto daily_research_points(world_state const& ws, T this_nation, U base = 0.0f) -> decltype(ve::widen_to<T>(0.0f));

	float effective_tech_cost(tech_tag t, world_state const& ws, nations::country_tag this_nation);
	float get_invention_chance(tech_tag t, world_state& ws, nations::country_tag this_nation);
	ve::fp_vector get_invention_chance(tech_tag t, world_state& ws, ve::contiguous_tags_base<nations::country_tag> this_nation);
	bool can_research(tech_tag t, world_state const& ws, nations::country_tag this_nation);

	void daily_update(world_state& ws);
}
