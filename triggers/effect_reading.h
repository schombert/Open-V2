#pragma once
#include "triggers.h"
#include "Parsers\\parsers.h"

namespace scenario {
	class scenario_manager;
}
namespace events {
	struct event_creation_manager;
}

namespace triggers {
	struct effect_parsing_environment {
		scenario::scenario_manager& s;
		events::event_creation_manager& ecm;

		std::vector<uint16_t> data;
		trigger_scope_state current_scope;

		effect_parsing_environment(scenario::scenario_manager& sm, events::event_creation_manager& e, trigger_scope_state scope) : s(sm), ecm(e), current_scope(scope) {}
	};


	void add_simple_effect(
		effect_parsing_environment& env,
		const token_and_type& effect_name,
		association_type a,
		const token_and_type& effect_value);

	bool effect_scope_is_empty(const uint16_t* source);
	bool effect_scope_has_single_member(const uint16_t* source); //precondition: scope known to not be empty
	int32_t simplify_effect(uint16_t* source); //yields new source size
	std::vector<uint16_t> parse_effect(scenario::scenario_manager& s, trigger_scope_state outer_scope, const token_group* start, const token_group* end);
	effect_tag commit_effect(trigger_manager& trigger_manager, const std::vector<uint16_t>& new_effect);
}