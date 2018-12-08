#include "common\\common.h"
#include "issues.h"
#include "issues_functions.h"
#include "world_state\\world_state.h"
#include "triggers\\effects.h"
#include "governments\\governments_functions.h"

namespace issues {
	float calculate_political_interest(world_state const& ws, tagged_array_view<const float, population::demo_tag, false> demographics_array) {
		auto political_offset =
			&(demographics_array[population::to_demo_tag(ws, issues::option_tag(0))]) +
			ws.s.issues_m.party_issues_options_count +
			ws.s.issues_m.social_issues_options_count;
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> political_vector(political_offset, ws.s.issues_m.political_issues_options_count);
		return (demographics_array[population::total_population_tag] != 0) ?
			(float(political_vector.sum()) / float(demographics_array[population::total_population_tag])) : 0.0f;
	}
	float calculate_social_interest(world_state const& ws, tagged_array_view<const float, population::demo_tag, false> demographics_array) {
		auto social_offset =
			&(demographics_array[population::to_demo_tag(ws, issues::option_tag(0))]) +
			ws.s.issues_m.party_issues_options_count;
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> social_vector(social_offset, ws.s.issues_m.social_issues_options_count);
		return (demographics_array[population::total_population_tag] != 0) ?
			(float(social_vector.sum()) / float(demographics_array[population::total_population_tag])) : 0.0f;
	}

	void reset_active_issues(world_state& ws, nations::country_tag nation_for) {
		uint32_t max_issues = uint32_t(ws.s.issues_m.issues_container.size());
		auto issues_row = ws.w.nation_s.active_issue_options.get_row(nation_for);

		for(uint32_t i = 0; i < max_issues; ++i) {
			auto itag = issue_tag(static_cast<issue_tag::value_base_t>(i));
			auto& this_issue = ws.s.issues_m.issues_container[itag];
			if(this_issue.type == issue_group::political || this_issue.type == issue_group::social || this_issue.type == issue_group::party) {
				issues_row[itag] = this_issue.options[0];
			} else {
				issues_row[itag] = option_tag();
			}
		}
	}

	float administrative_requirement(world_state const& ws, nations::country_tag nation_for) {
		int32_t sum_multiplier = 0;
		
		uint32_t max_issues = uint32_t(ws.s.issues_m.issues_container.size());

		auto active_options = ws.w.nation_s.active_issue_options.get_row(nation_for);
		for(uint32_t i = 0; i < max_issues; ++i) {
			auto itag = issue_tag(static_cast<issue_tag::value_base_t>(i));
			if(is_valid_index(active_options[itag]))
				sum_multiplier += ws.s.issues_m.options[active_options[itag]].administrative_multiplier;
		}

		return float(sum_multiplier) * ws.s.modifiers_m.global_defines.bureaucracy_percentage_increment + ws.s.modifiers_m.global_defines.max_bureaucracy_percentage;
	}

	void change_issue_option(world_state& ws, issues::option_tag opt, nations::country_tag nation_for) {
		auto& this_option = ws.s.issues_m.options[opt];
		auto current_issue = this_option.parent_issue;

		if(ws.w.nation_s.active_issue_options.get(nation_for, current_issue) != opt) {

			if(!is_valid_index(this_option.on_execute_trigger) || 
				triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(this_option.on_execute_trigger), ws, nation_for, nation_for, nullptr)) {

				if(is_valid_index(this_option.on_execute_effect)) {
					triggers::execute_effect(ws.s.trigger_m.effect_data.data() + to_index(this_option.on_execute_effect), ws, nation_for, nation_for, nullptr, get_local_generator());
				}
			}

			ws.w.nation_s.active_issue_options.get(nation_for, current_issue) = opt;
			governments::update_current_rules(ws, nation_for);
		}
	}
}
