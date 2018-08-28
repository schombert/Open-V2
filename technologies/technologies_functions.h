#pragma once
#include "common\\common.h"
#include "technologies.h"

class world_state;

namespace nations {
	struct nation;
}

namespace technologies {
	void apply_technology(world_state& ws, nations::nation& this_nation, tech_tag tech); // does not update stats of existing units or national modifier values
	void apply_single_technology(world_state& ws, nations::nation& this_nation, tech_tag tech);
	void restore_technologies(world_state& ws, nations::nation& this_nation); // reset technologies and base unit stats first
	void reset_technologies(world_state& ws, nations::nation& this_nation);

	float daily_research_points(world_state const& ws, nations::nation const& this_nation);
}
