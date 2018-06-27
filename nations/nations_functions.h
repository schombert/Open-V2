#pragma once
#include "common\\common.h"
#include "nations.h"
#include "provinces\\provinces.h"

namespace current_state {
	class state;
}
class world_state;


namespace nations {
	void silent_remove_province_owner(world_state& ws, nation* owner, provinces::province_tag prov);
	void silent_remove_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov);
	void silent_set_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov);
	void destroy_state_instance(world_state& ws, state_tag st);
	void remove_province_from_state(world_state& ws, provinces::province_state& p);
	void silent_set_province_owner(world_state& ws, nation* owner, provinces::province_tag prov);
	nation* make_nation_for_tag(world_state& ws, cultures::national_tag nt);
	bool is_state_empty(world_state& ws, nation* owner, provinces::state_tag region);
}
