#include "common\\common.h"
#include "population_function.h"
#include "world_state\\world_state.h"

namespace population {
	void init_rebel_faction_from_rebel_type(rebel_faction& faction, rebel_type& type) {
		faction.type = type.id;
		faction.icon = type.icon;
		faction.flags = type.flags;
	}

	pop& make_new_pop(world_state& ws) {
		return ws.w.population_s.pops.get_new();
	}

	void init_population_state(world_state& ws) {
		ws.w.population_s.pop_demographics.reset(aligned_32_issues_ideology_demo_size(ws));
	}

	void init_pop_demographics(world_state& ws, pop& p) {
		ws.w.population_s.pop_demographics.ensure_capacity(to_index(p.id) + 1);
		ws.w.population_s.pop_demographics.get(p.id, total_population_tag) = p.size;
	}

	bool is_pop_accepted(world_state& ws, pop& p, nations::nation& n) {
		return p.culture == n.primary_culture || contains_item(ws.w.culture_s.culture_arrays, n.accepted_cultures, p.culture);
	}
}
