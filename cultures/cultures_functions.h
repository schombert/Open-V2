#pragma once
#include "common\\common.h"
#include "cultures.h"

class world_state;

namespace cultures {
	void init_cultures_state(world_state& ws);
	void reset_state(cultures_state& s);
}
