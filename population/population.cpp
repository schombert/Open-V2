#include "population.h"
#include "world_state\\world_state.h"

namespace population {
	demo_tag to_demo_tag(world_state const&, ideologies::ideology_tag t) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(to_index(t) + 1));
	}
	demo_tag to_demo_tag(world_state const& ws, issues::option_tag t) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(to_index(t) + 1 + ws.s.ideologies_m.ideologies_count));
	}
	demo_tag to_demo_tag(world_state const& ws, cultures::culture_tag t) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(to_index(t) + 1 + ws.s.ideologies_m.ideologies_count + ws.s.issues_m.options_count));
	}
	demo_tag to_demo_tag(world_state const& ws, cultures::religion_tag t) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(to_index(t) + 1 +
			ws.s.ideologies_m.ideologies_count + 
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures));
	}
	demo_tag to_demo_tag(world_state const& ws, pop_type_tag t) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(to_index(t) + 1 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions));
	}
	demo_tag to_employment_demo_tag(world_state const& ws, pop_type_tag t) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(to_index(t) + 1 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes));
	}
	demo_tag poor_population_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(1 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag middle_population_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(2 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag rich_population_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(3 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}

	demo_tag militancy_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(4 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag consciousness_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(5 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag literacy_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(6 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}

	demo_tag poor_militancy_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(7 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag middle_militancy_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(9 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag rich_militancy_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(10 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}

	demo_tag poor_life_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(11 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag middle_life_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(12 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag rich_life_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(13 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}

	demo_tag poor_everyday_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(14 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag middle_everyday_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(15 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag rich_everyday_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(16 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}

	demo_tag poor_luxury_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(17 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag middle_luxury_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(18 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}
	demo_tag rich_luxury_needs_demo_tag(world_state const& ws) {
		return demo_tag(static_cast<value_base_of<demo_tag>>(19 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes) * 2ui32);
	}

	uint32_t aligned_32_issues_ideology_demo_size(world_state const& ws) {
		return ((static_cast<uint32_t>(sizeof(uint32_t)) * uint32_t(1ui32 + ws.s.ideologies_m.ideologies_count + ws.s.issues_m.party_issues_options_count) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(uint32_t));
	}
	uint32_t aligned_32_demo_size(world_state const& ws) {
		return (((static_cast<uint32_t>(sizeof(uint32_t)) * uint32_t(19ui32 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes * 2ui32)) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(uint32_t));
	}
	uint32_t aligned_64_demo_size(world_state const& ws) {
		return (((static_cast<uint32_t>(sizeof(uint64_t)) * uint32_t(19ui32 +
			ws.s.ideologies_m.ideologies_count +
			ws.s.issues_m.party_issues_options_count +
			ws.s.culture_m.count_cultures +
			ws.s.culture_m.count_religions +
			ws.s.population_m.count_poptypes * 2ui32)) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(uint64_t));
	}
}
