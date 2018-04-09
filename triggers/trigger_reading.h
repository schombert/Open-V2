#pragma once
#include <vector>
#include "triggers.h"

namespace triggers {
	void add_simple_trigger(
		std::vector<uint16_t>& data,
		scenario::scenario_manager& s,
		const trigger_scope_state& scope,
		const token_and_type& trigger_name,
		association_type a,
		const token_and_type& trigger_value);
}
