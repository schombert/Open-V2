#include "common\\common.h"
#include "population_function.h"
#include "world_state\\world_state.h"

namespace population {
	void init_rebel_faction_from_rebel_type(rebel_faction& faction, rebel_type& type) {
		faction.type = type.id;
		faction.icon = type.icon;
		faction.flags = type.flags;
	}
}
