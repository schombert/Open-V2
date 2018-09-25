#pragma once
#include "common\\common.h"
#include "triggers.h"

class world_state;

namespace triggers {
	bool refers_to_technology(trigger_tag t, technologies::tech_tag tech, world_state const& ws);
}
