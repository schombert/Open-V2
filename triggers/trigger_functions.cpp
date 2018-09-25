#include "common\\common.h"
#include "trigger_functions.h"
#include "codes.h"
#include "world_state\\world_state.h"

namespace triggers {
	bool refers_to_technology(trigger_tag t, technologies::tech_tag tech, world_state const& ws) {
		bool result = false;
		recurse_over_triggers(ws.s.trigger_m.trigger_data.data() + to_index(t), [&result, tech](uint16_t const* dat) {
			if((*dat & trigger_codes::is_scope) == 0 &&
				(*dat & trigger_codes::code_mask) == trigger_codes::technology &&
				trigger_payload(dat[2]).tech == tech) {

				result = true;
			}
		});
		return result;
	}
}
