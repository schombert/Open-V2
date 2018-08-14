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
}
