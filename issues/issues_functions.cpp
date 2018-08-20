#include "common\\common.h"
#include "issues.h"
#include "issues_functions.h"
#include "world_state\\world_state.h"

namespace issues {
	void reset_active_issues(world_state& ws, nations::country_tag nation_for) {
		uint32_t max_issues = uint32_t(ws.s.issues_m.issues_container.size());
		auto issues_row = ws.w.nation_s.active_issue_options.get_row(nation_for);

		for(uint32_t i = 0; i < max_issues; ++i) {
			auto& this_issue = ws.s.issues_m.issues_container[issue_tag(static_cast<issue_tag::value_base_t>(i))];
			if(this_issue.type == issue_group::political || this_issue.type == issue_group::social || this_issue.type == issue_group::party) {
				issues_row[i] = this_issue.options[0];
			} else {
				issues_row[i] = option_tag();
			}
		}
	}

	float administrative_requirement(world_state const& ws, nations::country_tag nation_for) {
		int32_t sum_multiplier = 0;
		
		uint32_t max_issues = uint32_t(ws.s.issues_m.issues_container.size());

		auto active_options = ws.w.nation_s.active_issue_options.get_row(nation_for);
		for(uint32_t i = 0; i < max_issues; ++i) {
			if(is_valid_index(active_options[i]))
				sum_multiplier += ws.s.issues_m.options[active_options[i]].administrative_multiplier;
		}

		return float(sum_multiplier) * ws.s.modifiers_m.global_defines.bureaucracy_percentage_increment + ws.s.modifiers_m.global_defines.max_bureaucracy_percentage;
	}
}
