#include "common\\common.h"
#include "ideologies.h"
#include "ideologies_functions.h"
#include "world_state\\world_state.h"

namespace ideologies {
	void init_ideology_state(world_state& ws) {
		ws.w.ideology_s.ideology_enabled.resize(ws.s.ideologies_m.ideologies_count);
	}
	bool ideology_enabled(world_state const& ws, ideology_tag id) {
		return ws.w.ideology_s.ideology_enabled[id];
	}
}
