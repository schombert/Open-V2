#include "common\\common.h"
#include "issues.h"
#include "issues_functions.h"
#include "world_state\\world_state.h"
#include "triggers\\effects.h"
#include "governments\\governments_functions.h"
#include "modifiers\modifier_functions.h"
#include "nations\nations_functions.hpp"
#include "provinces\province_functions.hpp"

namespace issues {
	float calculate_political_interest(world_state const& ws, tagged_array_view<const float, population::demo_tag> demographics_array) {
		auto political_offset =
			&(demographics_array[population::to_demo_tag(ws, issues::option_tag(0))]) +
			ws.s.issues_m.party_issues_options_count +
			ws.s.issues_m.social_issues_options_count;
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> political_vector(political_offset, ws.s.issues_m.political_issues_options_count);
		return (demographics_array[population::total_population_tag] != 0) ?
			(float(political_vector.sum()) / float(demographics_array[population::total_population_tag])) : 0.0f;
	}
	float calculate_social_interest(world_state const& ws, tagged_array_view<const float, population::demo_tag> demographics_array) {
		auto social_offset =
			&(demographics_array[population::to_demo_tag(ws, issues::option_tag(0))]) +
			ws.s.issues_m.party_issues_options_count;
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> social_vector(social_offset, ws.s.issues_m.social_issues_options_count);
		return (demographics_array[population::total_population_tag] != 0) ?
			(float(social_vector.sum()) / float(demographics_array[population::total_population_tag])) : 0.0f;
	}

	void reset_active_issues(world_state& ws, nations::country_tag nation_for) {
		uint32_t max_issues = uint32_t(ws.s.issues_m.issues_container.size());

		for(uint32_t i = 0; i < max_issues; ++i) {
			auto itag = issue_tag(static_cast<issue_tag::value_base_t>(i));
			auto& this_issue = ws.s.issues_m.issues_container[itag];
			if(this_issue.type == issue_group::political || this_issue.type == issue_group::social || this_issue.type == issue_group::party) {
				ws.w.nation_s.active_issue_options.set(nation_for, itag, this_issue.options[0]);
			} else {
				ws.w.nation_s.active_issue_options.set(nation_for, itag, option_tag());
			}
		}
	}

	float administrative_requirement(world_state const& ws, nations::country_tag nation_for) {
		int32_t sum_multiplier = 0;
		
		uint32_t max_issues = uint32_t(ws.s.issues_m.issues_container.size());

		for(uint32_t i = 0; i < max_issues; ++i) {
			auto itag = issue_tag(static_cast<issue_tag::value_base_t>(i));
			auto opt = ws.w.nation_s.active_issue_options.get(nation_for, itag);
			if(is_valid_index(opt))
				sum_multiplier += ws.s.issues_m.options[opt].administrative_multiplier;
		}

		return float(sum_multiplier) * ws.s.modifiers_m.global_defines.bureaucracy_percentage_increment + ws.s.modifiers_m.global_defines.max_bureaucracy_percentage;
	}

	void change_issue_option(world_state& ws, issues::option_tag opt, nations::country_tag nation_for, bool as_player_choice) {
		auto& this_option = ws.s.issues_m.options[opt];
		auto current_issue = this_option.parent_issue;

		if(ws.w.nation_s.active_issue_options.get(nation_for, current_issue) != opt) {

			if(!is_valid_index(this_option.on_execute_trigger) || 
				triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(this_option.on_execute_trigger), ws, nation_for, nation_for, triggers::const_parameter())) {

				if(is_valid_index(this_option.on_execute_effect)) {
					triggers::execute_effect(ws.s.trigger_m.effect_data.data() + to_index(this_option.on_execute_effect), ws, nation_for, nation_for, triggers::const_parameter(), get_local_generator());
				}
			}

			ws.w.nation_s.active_issue_options.get(nation_for, current_issue) = opt;
			governments::update_current_rules(ws, nation_for);
		}

		if(as_player_choice) {
			ws.w.nation_s.nations.set<nation::last_reform_date>(nation_for, ws.w.current_date);

			auto const p_issue = ws.s.issues_m.options[opt].parent_issue;
			auto const current_reform = ws.w.nation_s.active_issue_options.get(nation_for, p_issue);
			bool is_reform = to_index(opt) > to_index(current_reform);

			float* support_factors = (float*)_alloca(ws.s.ideologies_m.ideologies_count * sizeof(float));
			std::fill_n(support_factors, ws.s.ideologies_m.ideologies_count, 0.0f);

			if(!is_reform) {
				if(ws.s.issues_m.issues_container[p_issue].type == issue_group::social) {
					ws.s.ideologies_m.for_each_ideology([nation_for, support_factors, &ws](ideologies::ideology_tag i) {
						auto const ftag = ws.s.ideologies_m.ideology_container[i].add_social_reform;
						if(ftag)
							support_factors[to_index(i)] = float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
					});
				} else if(ws.s.issues_m.issues_container[p_issue].type == issue_group::political) {
					ws.s.ideologies_m.for_each_ideology([nation_for, support_factors, &ws](ideologies::ideology_tag i) {
						auto const ftag = ws.s.ideologies_m.ideology_container[i].add_political_reform;
						if(ftag)
							support_factors[to_index(i)] = float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
					});
				}
			} else {
				if(ws.s.issues_m.issues_container[p_issue].type == issue_group::social) {
					ws.s.ideologies_m.for_each_ideology([nation_for, support_factors, &ws](ideologies::ideology_tag i) {
						auto const ftag = ws.s.ideologies_m.ideology_container[i].remove_social_reform;
						if(ftag)
							support_factors[to_index(i)] = float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
					});
				} else if(ws.s.issues_m.issues_container[p_issue].type == issue_group::political) {
					ws.s.ideologies_m.for_each_ideology([nation_for, support_factors, &ws](ideologies::ideology_tag i) {
						auto const ftag = ws.s.ideologies_m.ideology_container[i].remove_political_reform;
						if(ftag)
							support_factors[to_index(i)] = float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
					});
				}
			}

			nations::parallel_for_each_province(ws, nation_for, [support_factors, opt, &ws](provinces::province_tag p) {
				provinces::for_each_pop(ws, p, [support_factors, opt, &ws](population::pop_tag o) {
					float const pop_size = ws.w.population_s.pops.get<pop::size>(o);
					auto const ideology_support = ws.w.population_s.pop_demographics.get_row(o) 
						+ to_index(population::to_demo_tag(ws, ideologies::ideology_tag(0)));
					if(pop_size > 0.0f) {
						auto const militancy_change = ve::dot_product(ws.s.ideologies_m.ideologies_count, 
							ideology_support, 
							tagged_array_view<float, population::demo_tag>(support_factors, ws.s.ideologies_m.ideologies_count)) 
							* ws.s.modifiers_m.global_defines.mil_reform_impact * 0.1f / pop_size;
						auto const consciousness_change = ws.w.population_s.pop_demographics.get(o, population::to_demo_tag(ws, opt)) 
							* ws.s.modifiers_m.global_defines.con_reform_impact * 0.1f / pop_size;

						// todo: include movement support factor

						auto& con = ws.w.population_s.pops.get<pop::consciousness>(o);
						auto& mil = ws.w.population_s.pops.get<pop::militancy>(o);
						con = std::clamp(con + consciousness_change, 0.0f, 1.0f);
						mil = std::clamp(mil + militancy_change, 0.0f, 1.0f);
					}
				});

			});
		}
	}

	bool is_reform_timer_ready(world_state const& ws, nations::country_tag nation_for) {
		auto const last_date = ws.w.nation_s.nations.get<nation::last_reform_date>(nation_for);
		return 
			!is_valid_index(last_date)
			|| ((to_index(last_date) + int32_t(ws.s.modifiers_m.global_defines.min_delay_between_reforms) * 32) > to_index(ws.w.current_date));
	}

	bool is_reform_next_step(world_state const& ws, nations::country_tag nation_for, issues::option_tag opt) {
		auto const p_issue = ws.s.issues_m.options[opt].parent_issue;
		auto const current_reform = ws.w.nation_s.active_issue_options.get(nation_for, p_issue);

		return !(current_reform == opt)
			&& ws.s.issues_m.issues_container[p_issue].next_step_only
			&& (to_index(opt) + 1 == to_index(current_reform) || to_index(opt) == to_index(current_reform) + 1);
	}

	float get_uh_reform_support(world_state const& ws, nations::country_tag nation_for, issues::option_tag opt) {
		auto const p_issue = ws.s.issues_m.options[opt].parent_issue;
		auto const current_reform = ws.w.nation_s.active_issue_options.get(nation_for, p_issue);
		bool is_reform = to_index(opt) > to_index(current_reform);

		float total_support = 0.0f;
		if(is_reform) {
			if(ws.s.issues_m.issues_container[p_issue].type == issue_group::social) {
				ws.s.ideologies_m.for_each_ideology([nation_for, &total_support, &ws](ideologies::ideology_tag i) {
					auto const ftag = ws.s.ideologies_m.ideology_container[i].add_social_reform;
					if(ftag)
						total_support += float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
				});
			} else if(ws.s.issues_m.issues_container[p_issue].type == issue_group::political) {
				ws.s.ideologies_m.for_each_ideology([nation_for, &total_support, &ws](ideologies::ideology_tag i) {
					auto const ftag = ws.s.ideologies_m.ideology_container[i].add_political_reform;
					if(ftag)
						total_support += float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
				});
			}
		} else {
			if(ws.s.issues_m.issues_container[p_issue].type == issue_group::social) {
				ws.s.ideologies_m.for_each_ideology([nation_for, &total_support, &ws](ideologies::ideology_tag i) {
					auto const ftag = ws.s.ideologies_m.ideology_container[i].remove_social_reform;
					if(ftag)
						total_support += float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
				});
			} else if(ws.s.issues_m.issues_container[p_issue].type == issue_group::political) {
				ws.s.ideologies_m.for_each_ideology([nation_for, &total_support, &ws](ideologies::ideology_tag i) {
					auto const ftag = ws.s.ideologies_m.ideology_container[i].remove_political_reform;
					if(ftag)
						total_support += float(ws.w.nation_s.upper_house.get(nation_for, i)) * modifiers::test_additive_factor(ftag, ws, nation_for, nation_for);
				});
			}
		}
		return total_support;
	}

	bool is_reform_possible(world_state const& ws, nations::country_tag nation_for, issues::option_tag opt) {
		auto const opt_trigger = ws.s.issues_m.options[opt].allow;
		return is_reform_timer_ready(ws, nation_for)
			&& is_reform_next_step(ws, nation_for, opt)
			&& (!is_valid_index(opt_trigger) || triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(opt_trigger), ws, nation_for, nation_for, nation_for))
			&& get_uh_reform_support(ws, nation_for, opt) > 50.0f;
	}
}
