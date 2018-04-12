#pragma once
#include <vector>
#include "triggers.h"

namespace scenario {
	class scenario_manager;
}

namespace triggers {
	struct trigger_parsing_environment {
		scenario::scenario_manager& s;
		std::vector<uint16_t> data;
		trigger_scope_state current_scope;
	};

	void add_simple_trigger(
		trigger_parsing_environment& env,
		const token_and_type& trigger_name,
		association_type a,
		const token_and_type& trigger_value);
}
