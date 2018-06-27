#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "world_state\\world_state.h"

namespace provinces {
	void add_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	void remove_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	bool province_has_core(current_state::state& ws, province_tag prov, cultures::national_tag tag);
	void init_ready_provinces(current_state::state& ws);
}
