#pragma once
#include "issues_functions.h"
#include "world_state\\world_state.h"

namespace issues {
	template<typename value_type>
	float calculate_political_interest(world_state const& ws, value_type* demographics_array) {
		auto political_offset =
			demographics_array +
			to_index(population::to_demo_tag(ws, issues::option_tag(0))) +
			ws.s.issues_m.party_issues_options_count +
			ws.s.issues_m.social_issues_options_count;
		Eigen::Map<Eigen::Matrix<value_type, -1, 1>> political_vector(political_offset, ws.s.issues_m.political_issues_options_count);
		return (demographics_array[to_index(population::total_population_tag)] != value_type(0)) ?
			(float(political_vector.sum()) / float(demographics_array[to_index(population::total_population_tag)])) : 0.0f;
	}
	template<typename value_type>
	float calculate_social_interest(world_state const& ws, value_type* demographics_array) {
		auto social_offset =
			demographics_array +
			to_index(population::to_demo_tag(ws, issues::option_tag(0))) +
			ws.s.issues_m.party_issues_options_count;
		Eigen::Map<Eigen::Matrix<value_type, -1, 1>> social_vector(social_offset, ws.s.issues_m.social_issues_options_count);
		return (demographics_array[to_index(population::total_population_tag)] != value_type(0)) ?
			(float(social_vector.sum()) / float(demographics_array[to_index(population::total_population_tag)])) : 0.0f;
	}
}
