#pragma once
#include "common\\common.h"
#include "ideologies.h"

class world_state;

namespace ideologies {
	bool ideology_enabled(world_state const& ws, ideology_tag id);
	void init_ideology_state(world_state& ws);
	void reset_state(ideologies_state& s);
}
