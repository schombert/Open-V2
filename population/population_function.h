#pragma once
#include "common\\common.h"
#include "population.h"

class world_state;

namespace population {
	void init_rebel_faction_from_rebel_type(rebel_faction&, rebel_type&);
	pop& make_new_pop(world_state& ws);
}
