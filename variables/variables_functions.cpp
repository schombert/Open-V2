#include "common\\common.h"
#include "variables_functions.h"
#include "world_state\\world_state.h"

namespace variables {
	void init_variables_state(world_state& ws) {
		if(ws.w.variable_s.global_variables.size() != int32_t(ws.s.variables_m.count_global_variables))
			ws.w.variable_s.global_variables.resize(ws.s.variables_m.count_global_variables);
	}
	void reset_state(variables_state& s) {
		s.national_flags_arrays.reset();
	}
}
