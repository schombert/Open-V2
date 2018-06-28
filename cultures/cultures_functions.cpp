#include "common\\common.h"
#include "cultures_functions.h"
#include "world_state\\world_state.h"

namespace cultures {
	void init_cultures_state(world_state& ws) {
		ws.w.culture_s.national_tags_state.resize(ws.s.culutre_m.national_tags.size());
	}
}
