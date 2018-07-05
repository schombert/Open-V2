#pragma once
#include "common\\common.h"
#include "population.h"

class world_state;
namespace nations {
	struct nation;
}

namespace population {
	void init_rebel_faction_from_rebel_type(rebel_faction&, rebel_type&);
	pop& make_new_pop(world_state& ws);
	void init_population_state(world_state& ws);
	void init_pop_demographics(world_state& ws, pop& p);
	bool is_pop_accepted(world_state& ws, pop& p, nations::nation& n);
	pop* get_unassigned_soldier_in_province(world_state& ws, provinces::province_tag prov);
}
