#include "common\\common.h"
#include "ideologies.h"
#include "ideologies_functions.h"
#include "world_state\\world_state.h"

namespace ideologies {
	void init_ideology_state(world_state& ws) {
		ws.w.ideology_s.ideology_enabled.resize(ws.s.ideologies_m.ideologies_count);
	}
	void reset_state(ideologies_state&) {

	}
	bool ideology_enabled(world_state const& ws, ideology_tag id) {
		return ws.w.ideology_s.ideology_enabled[id];
	}
	void set_default_enabled_ideologies(world_state& ws) {
		for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
			ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
			if(ws.s.ideologies_m.ideology_container[this_tag].uncivilized || ws.s.ideologies_m.ideology_container[this_tag].enable_date < ws.w.current_date)
				ws.w.ideology_s.ideology_enabled[this_tag] = 1ui8;
		}
	}
}
