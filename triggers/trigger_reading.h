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

		trigger_parsing_environment(scenario::scenario_manager& sm, trigger_scope_state scope) : s(sm), current_scope(scope) {}
	};

	void add_simple_trigger(
		trigger_parsing_environment& env,
		const token_and_type& trigger_name,
		association_type a,
		const token_and_type& trigger_value);

	void invert_trigger(uint16_t* source);
	bool scope_is_empty(const uint16_t* source);
	bool scope_has_single_member(const uint16_t* source); //precondition: scope known to not be empty
	int32_t simplify_trigger(uint16_t* source); //yields new source size
	std::vector<uint16_t> parse_trigger(scenario::scenario_manager& s, trigger_scope_state outer_scope, const token_group* start, const token_group* end);
}
