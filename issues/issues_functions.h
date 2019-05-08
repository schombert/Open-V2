#pragma once
#include "common\\common.h"
#include "concurrency_tools\\concurrency_tools.h"
#include "issues.h"

class world_state;

namespace issues {
	void reset_active_issues(world_state& ws, nations::country_tag nation_for);
	float administrative_requirement(world_state const& ws, nations::country_tag nation_for);
	void change_issue_option(world_state& ws, issues::option_tag opt, nations::country_tag nation_for, bool as_player_choice = false);

	float calculate_political_interest(world_state const& ws, tagged_array_view<const float, population::demo_tag> demographics_array);
	float calculate_social_interest(world_state const& ws, tagged_array_view<const float, population::demo_tag> demographics_array);

	float get_uh_reform_support(world_state const& ws, nations::country_tag nation_for, issues::option_tag opt);
	bool is_reform_timer_ready(world_state const& ws, nations::country_tag nation_for);
	bool is_reform_next_step(world_state const& ws, nations::country_tag nation_for, issues::option_tag opt);

	bool is_reform_possible(world_state const& ws, nations::country_tag nation_for, issues::option_tag opt);
}
