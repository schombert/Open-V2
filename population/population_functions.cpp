#include "common\\common.h"
#include "population_functions.hpp"
#include "world_state\\world_state.h"
#include "modifiers\\modifier_functions.h"
#include "technologies\\technologies.h"
#include <ppl.h>
#include "concurrency_tools\\ve.h"
#include "issues\\issues_functions.h"
#include "nations\\nations_functions.h"

#undef min
#undef max

namespace population {

	pop_tag make_new_pop(world_state& ws) {
		return ws.w.population_s.pops.get_new();
	}

	void init_population_state(world_state& ws) {
		ws.w.population_s.pop_demographics.reset(aligned_32_issues_ideology_demo_size(ws));

		ws.w.population_s.independance_rebel_support.resize(ws.s.culture_m.national_tags.size());
		ws.w.population_s.independance_movement_support.resize(ws.s.culture_m.national_tags.size());
	}

	void reset_state(population_state& s) {
		s.pop_arrays.reset();
	}

	void init_pop_demographics(world_state& ws, pop_tag p, float size) {
		ws.w.population_s.pop_demographics.ensure_capacity(to_index(p) + 1);
		ws.w.population_s.pop_demographics.get(p, total_population_tag) = size;
		ws.w.population_s.pops.set<pop::size>(p, size);
		if((ws.s.population_m.pop_types[ws.w.population_s.pops.get<pop::type>(p)].flags & population::pop_type::is_employable) == 0)
			ws.w.population_s.pop_demographics.get(p, total_employment_tag) = size;
	}

	bool is_pop_accepted(world_state const& ws, pop_tag p, nations::country_tag n) {
		auto p_culture = ws.w.population_s.pops.get<pop::culture>(p);
		return p_culture == ws.w.nation_s.nations.get<nation::primary_culture>(n)
			|| contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(n), p_culture);
	}

	pop_tag get_unassigned_soldier_in_province(world_state const& ws, provinces::province_tag prov) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(prov));
		for(auto i : pop_range) {
			if(!is_valid_index(ws.w.population_s.pops.get<pop::associated_army>(i)) & (ws.w.population_s.pops.get<pop::type>(i) == ws.s.population_m.soldier))
				return i;
		}
		return pop_tag();
	}
	void set_militancy_direct(world_state& ws, pop_tag p, float v) {
		ws.w.population_s.pops.set<pop::militancy>(p, std::clamp(v, 0.0f, 10.0f) / 10.0f);
	}
	void set_literacy_direct(world_state& ws, pop_tag p, float v) {
		ws.w.population_s.pops.set<pop::literacy>(p, std::clamp(v, 0.0f, 1.0f));
	}
	void set_consciousness_direct(world_state& ws, pop_tag p, float v) {
		ws.w.population_s.pops.set<pop::consciousness>(p, std::clamp(v, 0.0f, 10.0f) / 10.0f);
	}

	

	float total_size_change(world_state const& ws, pop_tag p) {
		return ws.w.population_s.pops.get<pop::size_change_from_assimilation>(p)
			+ ws.w.population_s.pops.get<pop::size_change_from_combat>(p)
			- ws.w.population_s.pops.get<pop::size_change_from_emmigration>(p)
			+ ws.w.population_s.pops.get<pop::size_change_from_growth>(p)
			- ws.w.population_s.pops.get<pop::size_change_from_local_migration>(p)
			- ws.w.population_s.pops.get<pop::size_change_from_colonial_migration>(p)
			- ws.w.population_s.pops.get<pop::size_change_from_promotion>(p)
			- ws.w.population_s.pops.get<pop::size_change_from_demotion>(p);
	}

	bool is_dominant_issue(world_state const& ws, pop_tag id, issues::option_tag opt) {
		auto issue_offset =
			&(ws.w.population_s.pop_demographics.get_row(id)[population::to_demo_tag(ws, issues::option_tag(0))]);
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> options_vector(issue_offset, ws.s.issues_m.tracked_options_count);
		return options_vector.maxCoeff() == issue_offset[to_index(opt)];
	}

	bool is_dominant_ideology(world_state const& ws, pop_tag id, ideologies::ideology_tag opt) {
		auto ideology_offset =
			&(ws.w.population_s.pop_demographics.get_row(id)[population::to_demo_tag(ws, ideologies::ideology_tag(0))]);
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> ideology_vector(ideology_offset, ws.s.ideologies_m.ideologies_count);
		return ideology_vector.maxCoeff() == ideology_offset[to_index(opt)];
	}


	void change_pop_type(world_state& ws, pop_tag this_pop, pop_type_tag new_type) {
		// todo: fix employment
		ws.w.population_s.pops.set<pop::type>(this_pop, new_type);

		auto strata = ws.s.population_m.pop_types[new_type].flags & population::pop_type::strata_mask;
		if(strata == population::pop_type::strata_poor) {
			ws.w.population_s.pops.set<pop::is_poor>(this_pop, true);
			ws.w.population_s.pops.set<pop::is_middle>(this_pop, false);
		} else if(strata == population::pop_type::strata_poor) {
			ws.w.population_s.pops.set<pop::is_poor>(this_pop, false);
			ws.w.population_s.pops.set<pop::is_middle>(this_pop, true);
		} else {
			ws.w.population_s.pops.set<pop::is_poor>(this_pop, false);
			ws.w.population_s.pops.set<pop::is_middle>(this_pop, false);
		}
	}

	void change_pop_size(world_state& ws, pop_tag this_pop, int32_t new_size) {
		// todo: fix employment
		ws.w.population_s.pop_demographics.get(this_pop, total_population_tag) = float(new_size);
		ws.w.population_s.pops.set<pop::size>(this_pop, float(new_size));
	}

	void remove_pop_from_province(world_state& ws, pop_tag this_pop) {
		// todo: fix employment
		auto& location = ws.w.population_s.pops.get<pop::location>(this_pop);
		if(is_valid_index(location)) {
			remove_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(location), this_pop);
			location = provinces::province_tag();
		}
	}

	void change_pop_location(world_state& ws, pop_tag this_pop, provinces::province_tag new_location) {
		// todo: fix employment
		auto& location = ws.w.population_s.pops.get<pop::location>(this_pop);
		if(is_valid_index(location)) {
			remove_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(location), this_pop);
		}
		location = new_location;
		add_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(new_location), this_pop);
	}

	void free_slave(world_state& ws, pop_tag this_pop) {
		auto location = ws.w.population_s.pops.get<pop::location>(this_pop);
		auto local_rgo = ws.w.province_s.province_state_container.get<province_state::rgo_production>(location);
		bool is_mine = (ws.s.economy_m.goods[local_rgo].flags & economy::good_definition::mined) != 0;
		change_pop_type(ws, this_pop, is_mine ? ws.s.economy_m.rgo_mine.workers[0].type : ws.s.economy_m.rgo_farm.workers[0].type);
	}

	void trigger_rising(world_state&, rebel_faction_tag, nations::country_tag) {
		// todo
	}

	void default_initialize_issues_and_ideology(world_state& ws, pop_tag this_pop) {
		auto ideology_demo = &(ws.w.population_s.pop_demographics.get_row(this_pop)[to_demo_tag(ws, ideologies::ideology_tag(0))]);
		auto issues_demo = &(ws.w.population_s.pop_demographics.get_row(this_pop)[to_demo_tag(ws, issues::option_tag(0))]);
		auto total_pop_size = ws.w.population_s.pop_demographics.get(this_pop, total_population_tag);
		auto pop_type = ws.w.population_s.pops.get<pop::type>(this_pop);

		for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
			ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
			if(ws.w.ideology_s.ideology_enabled[this_tag] != 0ui8) {
				auto pop_incl = ws.s.population_m.ideological_inclination.get(pop_type, this_tag);
				if(is_valid_index(pop_incl)) {
					if(ws.s.ideologies_m.ideology_container[this_tag].uncivilized) {
						ideology_demo[i] = modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
					} else if(auto owner = get_pop_owner(ws, this_pop); bool(owner) && ws.w.nation_s.nations.get<nation::is_civilized>(owner)) {
						ideology_demo[i] = modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
					} else {
						ideology_demo[i] = 0.0f;
					}
				} else {
					ideology_demo[i] = 0.0f;
				}
			}
		}
		Eigen::Map<Eigen::Matrix<float, 1, -1>> ivec(ideology_demo, ws.s.ideologies_m.ideologies_count);
		ivec *= total_pop_size / ivec.sum();

		for(uint32_t i = 0; i < ws.s.issues_m.tracked_options_count; ++i) {
			issues::option_tag this_tag(static_cast<issues::option_tag::value_base_t>(i));
			auto pop_incl = ws.s.population_m.issue_inclination.get(pop_type, this_tag);
			if(is_valid_index(pop_incl))
				issues_demo[i] = modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
			else
				issues_demo[i] = 0;
		}
		Eigen::Map<Eigen::Matrix<float, 1, -1>> ovec(issues_demo, ws.s.issues_m.tracked_options_count);
		ovec *= total_pop_size / ovec.sum();

		ws.w.population_s.pops.set<pop::social_interest>(this_pop, issues::calculate_social_interest(ws, ws.w.population_s.pop_demographics.get_row(this_pop)));
		ws.w.population_s.pops.set<pop::political_interest>(this_pop, issues::calculate_political_interest(ws, ws.w.population_s.pop_demographics.get_row(this_pop)));
	}

	void update_ideology_preference(world_state& ws, pop_tag this_pop) {
		auto ideology_demo = &(ws.w.population_s.pop_demographics.get_row(this_pop)[to_demo_tag(ws, ideologies::ideology_tag(0))]);
		auto total_pop_size = ws.w.population_s.pop_demographics.get(this_pop, total_population_tag);
		auto pop_type = ws.w.population_s.pops.get<pop::type>(this_pop);

		const int32_t icount = int32_t(ws.s.ideologies_m.ideologies_count);
		for(int32_t i = 0; i < icount; ++i) {
			ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
			if(ws.w.ideology_s.ideology_enabled[this_tag] != 0ui8) {
				auto pop_incl = ws.s.population_m.ideological_inclination.get(pop_type, this_tag);
				if(is_valid_index(pop_incl)) {
					if(ws.s.ideologies_m.ideology_container[this_tag].uncivilized) {
						ideology_demo[i] += total_pop_size * 0.25f * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
					} else if(auto owner = get_pop_owner(ws, this_pop); ws.w.nation_s.nations.get<nation::is_civilized>(owner)) {
						ideology_demo[i] += total_pop_size * 0.25f * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
					}
				}
			}
		}
		Eigen::Map<Eigen::Matrix<float, 1, -1>> ivec(ideology_demo, ws.s.ideologies_m.ideologies_count);
		ivec *= (total_pop_size / ivec.sum());
	}

	void update_issues_preference(world_state& ws, pop_tag this_pop) {
		auto demo = ws.w.population_s.pop_demographics.get_row(this_pop);
		auto issues_demo = &(demo[to_demo_tag(ws, issues::option_tag(0))]);
		auto total_pop_size = ws.w.population_s.pop_demographics.get(this_pop, total_population_tag);
		auto pop_type = ws.w.population_s.pops.get<pop::type>(this_pop);

		auto location = ws.w.population_s.pops.get<pop::location>(this_pop);
		auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(location);

		auto owner_pr_modifier = 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::political_reform_desire>(owner);
		auto owner_sr_modifier = 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::social_reform_desire>(owner);
		auto owner_issue_change = 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::issue_change_speed>(owner);

		const int32_t icount = int32_t(ws.s.issues_m.tracked_options_count);
		for(int32_t i = 0; i < icount; ++i) {
			issues::option_tag this_tag(static_cast<issues::option_tag::value_base_t>(i));
			auto pop_incl = ws.s.population_m.issue_inclination.get(pop_type, this_tag);
			if(is_valid_index(pop_incl)) {
				if(ws.s.issues_m.options[this_tag].type == issues::issue_group::social)
					issues_demo[i] += 0.20f * owner_issue_change * owner_sr_modifier * total_pop_size * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
				else if(ws.s.issues_m.options[this_tag].type == issues::issue_group::political)
					issues_demo[i] += 0.20f * owner_issue_change * owner_pr_modifier * total_pop_size * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
				else
					issues_demo[i] += 0.20f * owner_issue_change * total_pop_size * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, triggers::const_parameter());
			}
		}
		Eigen::Map<Eigen::Matrix<float, 1, -1>> ovec(issues_demo, ws.s.issues_m.tracked_options_count);
		ovec *= (total_pop_size / ovec.sum());

		ws.w.population_s.pops.set<pop::social_interest>(this_pop, issues::calculate_social_interest(ws, demo));
		ws.w.population_s.pops.set<pop::political_interest>(this_pop, issues::calculate_political_interest(ws, demo));
	}

	void default_initialize_world_issues_and_ideology(world_state& ws) {
		ws.w.population_s.pops.parallel_for_each([&ws](pop_tag pt) {
			default_initialize_issues_and_ideology(ws, pt);
		});
	}

	struct literacy_update_operation {
		tagged_array_view<float const, nations::state_tag> change_by_state;
		tagged_array_view<provinces::province_tag const, population::pop_tag> pop_locations;
		tagged_array_view<nations::state_tag const, provinces::province_tag> province_state_owners;
		tagged_array_view<float, population::pop_tag> pop_literacy;

		literacy_update_operation(world_state& ws, tagged_array_view<float const, nations::state_tag> a) :
			change_by_state(a),
			pop_locations(ws.w.population_s.pops.get_row<pop::location>()),
			province_state_owners(ws.w.province_s.province_state_container.get_row<province_state::state_instance>()),
			pop_literacy(ws.w.population_s.pops.get_row<pop::literacy>()) {}

		template<typename T>
		__forceinline void operator()(T pop_v) {
			auto plocation = ve::load(pop_v, pop_locations);
			auto state_indices = ve::load(plocation, province_state_owners);
			auto change = ve::load(state_indices, change_by_state);
			auto new_literacy = ve::load(pop_v, pop_literacy) + change;
			ve::store(pop_v, pop_literacy, ve::max(0.0f, ve::min(new_literacy, 1.0f)));
		}
	};

	struct gather_literacy_change_operation {
		world_state const& ws;

		tagged_array_view<float const, nations::state_tag> state_populations;
		tagged_array_view<nations::country_tag const, nations::state_tag> state_owners;
		tagged_array_view<float const, nations::country_tag> education_efficiency_modifier;
		tagged_array_view<float const, nations::country_tag> tech_education_efficiency_modifier;
		tagged_array_view<float const, nations::country_tag> education_spending;

		tagged_array_view<float, nations::state_tag> change_out;

		gather_literacy_change_operation(world_state& w, tagged_array_view<float, nations::state_tag> f) :
			ws(w),
			state_populations(w.w.nation_s.states.get_row<state::total_population>()),
			state_owners(w.w.nation_s.states.get_row<state::owner>()),
			education_efficiency_modifier(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::education_efficiency_modifier>(0)),
			tech_education_efficiency_modifier(w.w.nation_s.tech_attributes.get_row<technologies::tech_offset::education_efficiency>(0)), 
			education_spending(w.w.nation_s.nations.get_row<nation::f_education_spending>()),
			change_out(f) {}

		template<typename T>
		__forceinline void operator()(T state_v) {
			auto owners = ve::load(state_v, state_owners);

			auto gath_education_efficiency_modifier = ve::load(owners, education_efficiency_modifier);
			auto gath_tech_education_efficiency_modifier = ve::load(owners, tech_education_efficiency_modifier);
			auto gath_education_spending = ve::load(owners, education_spending);

			auto total_pop = ve::load(state_v, state_populations);

			auto clergy_amount = ve::apply(state_v, [_this = this](nations::state_tag sid) {
				if(_this->ws.w.nation_s.states.is_valid_index(sid))
					return _this->ws.w.nation_s.state_demographics.get(sid, population::to_demo_tag(_this->ws, _this->ws.s.population_m.clergy));
				else
					return 0.0f;
			});

			ve::store(state_v, change_out,
				(((clergy_amount / total_pop) - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)
				* (1.0f / (ws.s.modifiers_m.global_defines.max_clergy_for_literacy - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)))
				* ((gath_education_efficiency_modifier + 1.0f)
				* (gath_tech_education_efficiency_modifier + 1.0f))
				* (gath_education_spending
				* (ws.s.modifiers_m.global_defines.literacy_change_speed * 0.01f))
				);
		}
	};

	void update_literacy(world_state& ws) {
		concurrent_cache_aligned_buffer<float, nations::state_tag, true> change_by_state(ws.w.nation_s.states.size());

		ve::execute_parallel<nations::state_tag>(ws.w.nation_s.states.vector_size(),
			gather_literacy_change_operation(ws, change_by_state.view()));
	
		change_by_state.padding() = 0.0f;

		uint32_t pop_size = ws.w.population_s.pops.vector_size();
		uint32_t chunk_size = pop_size / (32ui32 * 16ui32);

		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);
		if(chunk_index != 31) {
			ve::execute_parallel<population::pop_tag>(chunk_size * 16ui32 * chunk_index, chunk_size * 16ui32 * (chunk_index + 1ui32),
				literacy_update_operation(ws, change_by_state.view()));
		} else {
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * 16ui32 * chunk_index, pop_size,
				literacy_update_operation(ws, change_by_state.view()));
		}
	}

	void update_pop_ideology_and_issues(world_state& ws) {
		const uint32_t pop_count = uint32_t(ws.w.population_s.pops.size());
		auto chunk_size = (pop_count + 31ui32) / 32ui32;
		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);

		concurrency::parallel_for(chunk_size * chunk_index, std::min(chunk_size * (chunk_index + 1), pop_count), [&ws](uint32_t i) {
			pop_tag pt = pop_tag(pop_tag::value_base_t(i));

			update_ideology_preference(ws, pt);
			update_issues_preference(ws, pt);
		}, concurrency::static_partitioner());
	}

	struct gather_militancy_by_province_operation {
		tagged_array_view<float, provinces::province_tag> base_modifier_out;
		tagged_array_view<float, provinces::province_tag> non_accepted_modifier_out;

		tagged_array_view<nations::country_tag const, provinces::province_tag> province_owners;
		tagged_array_view<nations::state_tag const, provinces::province_tag> province_states;

		tagged_array_view<float const, provinces::province_tag> prov_militancy_mod;
		tagged_array_view<float const, nations::country_tag> nat_militancy_mod;
		tagged_array_view<float const, nations::country_tag> nat_war_exhaustion;
		tagged_array_view<float const, nations::country_tag> nat_non_accepted_mil;
		tagged_array_view<float const, nations::country_tag> tech_seperatism;
		tagged_array_view<float const, nations::country_tag> nat_core_pop_mil;

		world_state const& ws;

		gather_militancy_by_province_operation(world_state& w,
			tagged_array_view<float, provinces::province_tag> a,
			tagged_array_view<float, provinces::province_tag> b) :
			ws(w), base_modifier_out(a), non_accepted_modifier_out(b),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			province_states(w.w.province_s.province_state_container.get_row<province_state::state_instance>()),
			prov_militancy_mod(w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_militancy_modifier>(0)),
			nat_militancy_mod(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_militancy_modifier>(0)),
			nat_war_exhaustion(w.w.nation_s.nations.get_row<nation::war_exhaustion>()),
			nat_non_accepted_mil(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_militancy_modifier>(0)),
			tech_seperatism(w.w.nation_s.tech_attributes.get_row<technologies::tech_offset::seperatism>(0)),
			nat_core_pop_mil(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_militancy_modifier>(0)) {}

		template<typename T>
		void operator()(T province_v) {
			auto owner_indices = ve::load(province_v, province_owners);
			auto state_indices = ve::load(province_v, province_states);

			auto prov_mil_mod = ve::load(province_v, prov_militancy_mod);

			auto owner_mil_mod = ve::load(owner_indices, nat_militancy_mod);
			auto owner_war_ex = ve::load(owner_indices, nat_war_exhaustion);
			auto owner_non_accepted = ve::load(owner_indices, nat_non_accepted_mil);
			auto owner_seperatism = ve::load(owner_indices, tech_seperatism);
			auto owner_core_pop_mil = ve::load(owner_indices, nat_core_pop_mil);

			//is_colonial_or_protectorate
			auto core_mask = nations::is_colonial_or_protectorate(ws, state_indices);

			ve::store(province_v, base_modifier_out,
				ve::multiply_and_add(
					owner_war_ex, 
					ws.s.modifiers_m.global_defines.mil_war_exhaustion,
					owner_mil_mod) + (prov_mil_mod + ve::select(core_mask, owner_core_pop_mil, ve::fp_vector()))
				);
			ve::store(province_v, non_accepted_modifier_out,
				ve::multiply_and_add(
					(owner_seperatism + 1.0f),
					ws.s.modifiers_m.global_defines.mil_non_accepted,
					owner_non_accepted)
			);
		}
	};

	struct update_militancy_operation {
		tagged_array_view<float, pop_tag> pop_militancy;

		tagged_array_view<float const, provinces::province_tag> base_prov_modifier;
		tagged_array_view<float const, provinces::province_tag> prov_non_accepted_modifier;

		tagged_array_view<nations::country_tag const, provinces::province_tag> province_owners;
		tagged_array_view<provinces::province_tag const, pop_tag> pop_locations;

		tagged_array_view<bitfield_type const, pop_tag> pop_acceptance;

		tagged_array_view<float const, pop_tag> pop_size;
		tagged_array_view<float const, pop_tag> pop_satisfaction;
		tagged_array_view<float const, pop_tag> pop_political_reform_support;
		tagged_array_view<float const, pop_tag> pop_social_reform_support;

		world_state const& ws;

		update_militancy_operation(world_state& w,
			tagged_array_view<float const, provinces::province_tag> a,
			tagged_array_view<float const, provinces::province_tag> b) :
			ws(w),
			pop_militancy(w.w.population_s.pops.get_row<pop::militancy>()),
			base_prov_modifier(a), prov_non_accepted_modifier(b),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			pop_locations(w.w.population_s.pops.get_row<pop::location>()),
			pop_acceptance(w.w.population_s.pops.get_row<pop::is_accepted>()),
			pop_size(w.w.population_s.pops.get_row<pop::size>()),
			pop_satisfaction(w.w.population_s.pops.get_row<pop::needs_satisfaction>()),
			pop_political_reform_support(w.w.population_s.pops.get_row<pop::political_interest>()),
			pop_social_reform_support(w.w.population_s.pops.get_row<pop::social_interest>()) {}

		template<typename T>
		void operator()(T pop_v) {
			auto province_indices = ve::load(pop_v, pop_locations);

			auto satisfaction = ve::load(pop_v, pop_satisfaction);

			auto satisfaction_factor =
				ve::select(satisfaction < 0.5f,
					ve::negate_multiply_and_add(
						ws.s.modifiers_m.global_defines.mil_no_life_need,
						satisfaction,
						ws.s.modifiers_m.global_defines.mil_no_life_need * 0.5f
					),
					ve::fp_vector())
				+ ve::select(satisfaction < 1.5f,
					ve::negate_multiply_and_add(
						ws.s.modifiers_m.global_defines.mil_lack_everyday_need,
						satisfaction,
						ws.s.modifiers_m.global_defines.mil_lack_everyday_need * 1.5f
					),
					ve::fp_vector())
				+ ve::select(satisfaction > 1.5f,
					ve::multiply_and_subtract(
						ws.s.modifiers_m.global_defines.mil_has_everyday_need,
						satisfaction,
						ws.s.modifiers_m.global_defines.mil_has_everyday_need * 1.5f
					),
					ve::fp_vector())
				+ ve::select(satisfaction > 2.5f,
					ve::multiply_and_subtract(
						ws.s.modifiers_m.global_defines.mil_has_luxury_need,
						satisfaction,
						ws.s.modifiers_m.global_defines.mil_has_luxury_need * 2.5f
					),
					ve::fp_vector());

			auto nation_indices = ve::load(province_indices, province_owners);

			auto non_accepted_factor = ve::select(ve::load(pop_v, pop_acceptance),
				ve::fp_vector(),
				ve::load(province_indices, prov_non_accepted_modifier));

			auto ruling_ideology = ve::load(nation_indices, ws.w.nation_s.nations.get_row<nation::ruling_ideology>());

			auto scaled_support_factors = ve::apply(pop_v, nation_indices, ruling_ideology,
				[sf = ws.s.modifiers_m.global_defines.mil_ideology, rp = ws.s.modifiers_m.global_defines.mil_ruling_party, _this = this]
			(population::pop_tag pid, nations::country_tag nid, ideologies::ideology_tag ri) {
				if(is_valid_index(pid)) {
					auto pop_c_support = _this->ws.w.population_s.pop_demographics.get(pid, to_demo_tag(_this->ws, _this->ws.s.ideologies_m.conservative_ideology));
					auto pop_rp_support = _this->ws.w.population_s.pop_demographics.get(pid, to_demo_tag(_this->ws, ri));

					return pop_c_support * sf + pop_rp_support * rp;
				} else {
					return 0.0f;
				}
			});
			
			auto support_factor = scaled_support_factors / ve::load(pop_v, pop_size);

			auto rr_factor = ws.s.modifiers_m.global_defines.mil_require_reform;
			auto reforms_factor = ve::multiply_and_add(
				rr_factor,
				ve::load(pop_v, pop_political_reform_support),
				ve::load(pop_v, pop_social_reform_support) * rr_factor);
			
			auto change =
				((satisfaction_factor + non_accepted_factor) + (support_factor + reforms_factor) + ve::load(province_indices, base_prov_modifier))
				* 0.1f;
			auto new_militancy = ve::load(pop_v, pop_militancy) + change;
			ve::store(pop_v, pop_militancy, ve::max(ve::fp_vector(), ve::min(new_militancy, 1.0f)));
		}
	};

	void update_militancy(world_state& ws) {
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> base_modifier(ws.w.province_s.province_state_container.size());
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> non_accepted_modifier(ws.w.province_s.province_state_container.size());


		ve::execute_parallel<provinces::province_tag>(ws.w.province_s.province_state_container.vector_size(),
			gather_militancy_by_province_operation(ws, base_modifier.view(), non_accepted_modifier.view()));


		uint32_t pop_size = ws.w.population_s.pops.vector_size();
		uint32_t chunk_size = pop_size / (32ui32 * 16ui32);

		update_militancy_operation op(ws, base_modifier.view(), non_accepted_modifier.view());

		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);
		if(chunk_index != 31)
			ve::execute_parallel<population::pop_tag>(chunk_size * 16ui32 * chunk_index, chunk_size * 16ui32 * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * 16ui32 * chunk_index, pop_size, op);
	}

	struct gather_consciousness_factors_operation {
		tagged_array_view<float, provinces::province_tag> fixed_factor_out;
		tagged_array_view<float, provinces::province_tag> clergy_factor_out;
		tagged_array_view<float, provinces::province_tag> literacy_factor_out;

		tagged_array_view<nations::country_tag const, provinces::province_tag> owner_indices;
		tagged_array_view<nations::state_tag const, provinces::province_tag> state_indices;

		tagged_array_view<float const, nations::country_tag> nat_mod_literacy_impact;
		tagged_array_view<float const, nations::country_tag> nat_plurality;
		tagged_array_view<float const, provinces::province_tag> prov_population;
		tagged_array_view<float const, nations::country_tag> nat_con_mod;
		tagged_array_view<float const, provinces::province_tag> prov_con_mod;
		tagged_array_view<float const, nations::country_tag> nat_core_con_mod;

		world_state const& ws;

		gather_consciousness_factors_operation(world_state const& w,
			tagged_array_view<float, provinces::province_tag> a,
			tagged_array_view<float, provinces::province_tag> b,
			tagged_array_view<float, provinces::province_tag> c) :
			ws(w), fixed_factor_out(a), clergy_factor_out(b), literacy_factor_out(c),
			owner_indices(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			state_indices(w.w.province_s.province_state_container.get_row<province_state::state_instance>()),
			nat_mod_literacy_impact(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::literacy_con_impact>(0)),
			nat_plurality(w.w.nation_s.nations.get_row<nation::plurality>()),
			prov_population(w.w.province_s.province_state_container.get_row<province_state::total_population>()),
			nat_con_mod(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_consciousness_modifier>(0)),
			prov_con_mod(w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_consciousness_modifier>(0)),
			nat_core_con_mod(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_consciousness_modifier>(0)) {}

		template<typename T>
		void operator()(T province_v) {
			auto owners = ve::load(province_v, owner_indices);
			auto states = ve::load(province_v, state_indices);
			auto core_mask = nations::is_colonial_or_protectorate(ws, states);

			auto colonial_multiplier = ve::select(core_mask, 1.0f, ws.s.modifiers_m.global_defines.con_colonial_factor);

			ve::store(province_v, literacy_factor_out,
				ve::multiply_and_add(colonial_multiplier, ve::load(owners, nat_mod_literacy_impact), colonial_multiplier)
				* (ve::load(owners, nat_plurality) * ws.s.modifiers_m.global_defines.con_literacy));

			auto clergy_populations = ve::apply(province_v, [_this = this](provinces::province_tag p) {
				if(_this->ws.w.province_s.province_state_container.is_valid_index(p))
					return _this->ws.w.province_s.province_demographics.get(p, to_demo_tag(_this->ws, _this->ws.s.population_m.clergy));
				else
					return 0.0f;
			});
			ve::store(province_v, clergy_factor_out, colonial_multiplier * clergy_populations / ve::load(province_v, prov_population));
			ve::store(province_v, fixed_factor_out, (ve::load(province_v, prov_con_mod) + ve::load(owners, nat_con_mod))
				+ ve::select(core_mask, ve::load(owners, nat_core_con_mod), ve::fp_vector()));
		}
	};

	struct update_consciousness_operation {
		tagged_array_view<float, pop_tag> pop_consciousness;

		tagged_array_view<provinces::province_tag const, pop_tag> pop_locations;
		tagged_array_view<nations::country_tag const, provinces::province_tag> owner_indices;

		tagged_array_view<float const, pop_tag> pop_literacy;
		tagged_array_view<float const, pop_tag> pop_satisfaction;
		tagged_array_view<float const, nations::country_tag> nat_non_accepted_con;

		tagged_array_view<bitfield_type const, pop_tag> pop_is_poor;
		tagged_array_view<bitfield_type const, pop_tag> pop_is_accepted;
		
		tagged_array_view<float const, provinces::province_tag> prov_fixed_factor;
		tagged_array_view<float const, provinces::province_tag> prov_clergy_factor;
		tagged_array_view<float const, provinces::province_tag> prov_literacy_factor;

		world_state const& ws;

		update_consciousness_operation(world_state& w,
			tagged_array_view<float const, provinces::province_tag> h,
			tagged_array_view<float const, provinces::province_tag> i,
			tagged_array_view<float const, provinces::province_tag> j) : ws(w),
			pop_consciousness(w.w.population_s.pops.get_row<pop::consciousness>()),
			pop_locations(w.w.population_s.pops.get_row<pop::location>()),
			owner_indices(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			pop_literacy(w.w.population_s.pops.get_row<pop::literacy>()),
			pop_satisfaction(w.w.population_s.pops.get_row<pop::needs_satisfaction>()),
			nat_non_accepted_con(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_consciousness_modifier>(0)),
			pop_is_poor(w.w.population_s.pops.get_row<pop::is_poor>()),
			pop_is_accepted(w.w.population_s.pops.get_row<pop::is_accepted>()),
			prov_fixed_factor(h), prov_clergy_factor(i), prov_literacy_factor(j)
		{}

		template<typename T>
		void operator()(T pop_v) {
			auto locations = ve::load(pop_v, pop_locations);

			auto fixed_factor = ve::load(locations, prov_fixed_factor);
			auto clergy_factor = ve::load(locations, prov_clergy_factor);
			auto literacy_factor = ve::load(locations, prov_literacy_factor);
			auto owners = ve::load(locations, owner_indices);

			auto lux_satisfaction = ve::load(pop_v, pop_satisfaction) - 2.0f;
			auto base_sum =
				ve::multiply_and_add(
					clergy_factor,
					ve::select(ve::load(pop_v, pop_is_poor),
						ws.s.modifiers_m.global_defines.con_poor_clergy,
						ws.s.modifiers_m.global_defines.con_midrich_clergy),
					fixed_factor)
				+ ve::multiply_and_add(
					literacy_factor,
					ve::load(pop_v, pop_literacy),
					ve::select(lux_satisfaction > ve::fp_vector(),
						lux_satisfaction * ws.s.modifiers_m.global_defines.con_luxury_goods,
						ve::fp_vector()))
				+ ve::select(ve::load(pop_v, pop_is_accepted), ve::fp_vector(), ve::load(owners, nat_non_accepted_con))
				;

			auto new_con = ve::multiply_and_add(base_sum, 0.1f, ve::load(pop_v, pop_consciousness));
			ve::store(pop_v, pop_consciousness, ve::max(ve::fp_vector(), ve::min(new_con, 1.0f)));
		}
	};

	void update_consciousness(world_state& ws) {
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> fixed_factor(ws.w.province_s.province_state_container.size());
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> clergy_factor(ws.w.province_s.province_state_container.size());
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> literacy_factor(ws.w.province_s.province_state_container.size());


		ve::execute_parallel<provinces::province_tag>(ws.w.province_s.province_state_container.vector_size(),
			gather_consciousness_factors_operation(ws, fixed_factor.view(), clergy_factor.view(), literacy_factor.view()));

		clergy_factor.padding() = 0.0f;
	
		uint32_t pop_size = ws.w.population_s.pops.vector_size();
		uint32_t chunk_size = pop_size / (32ui32 * 16ui32);

		update_consciousness_operation op(ws, fixed_factor.view(), clergy_factor.view(), literacy_factor.view());

		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);
		if(chunk_index != 31)
			ve::execute_parallel<population::pop_tag>(chunk_size * 16ui32 * chunk_index, chunk_size * 16ui32 * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * 16ui32 * chunk_index, pop_size, op);
	}

	struct calculate_promotion_operation {
		world_state const& ws;

		tagged_array_view<float, pop_tag> promotion_amount;
		tagged_array_view<float, pop_tag> demotion_amount;

		tagged_array_view<pop_type_tag const, pop_tag> pop_types;
		tagged_array_view<float const, pop_tag> pop_sizes;
		tagged_array_view<provinces::province_tag const, pop_tag> pop_location;
		tagged_array_view<nations::country_tag const, provinces::province_tag> province_owners;
		tagged_array_view<float const, nations::country_tag> national_admin_eff;

		tagged_array_view<nations::state_tag const, provinces::province_tag> province_state_instances;
		tagged_array_view<modifiers::national_focus_tag const, nations::state_tag> states_focuses;
		tagged_array_view<pop_type_tag const, modifiers::national_focus_tag> national_focus_type;

		calculate_promotion_operation(world_state& w) : ws(w),
			promotion_amount(w.w.population_s.pops.get_row<pop::size_change_from_promotion>()),
			demotion_amount(w.w.population_s.pops.get_row<pop::size_change_from_demotion>()),
			pop_types(w.w.population_s.pops.get_row<pop::type>()),
			pop_sizes(w.w.population_s.pops.get_row<pop::size>()),
			pop_location(w.w.population_s.pops.get_row<pop::location>()),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			national_admin_eff(w.w.nation_s.nations.get_row<nation::national_administrative_efficiency>()),
			province_state_instances(w.w.province_s.province_state_container.get_row<province_state::state_instance>()),
			states_focuses(w.w.nation_s.states.get_row<state::owner_national_focus>()),
			national_focus_type(w.s.modifiers_m.focus_to_pop_types.view())
		{}

		template<typename T>
		void operator()(T pop_v) {
			auto loc = ve::load(pop_v, pop_location);
			auto sz = ve::load(pop_v, pop_sizes);

			auto prov_states = ve::load(loc, province_state_instances);
			auto prov_nations = ve::load(loc, province_owners);

			auto state_focus = ve::load(prov_states, states_focuses);
			auto focus_target = (ve::load(state_focus, national_focus_type) == ve::load(pop_v, pop_types));

			/*auto p_trigger_amount = ve::apply(pop_v, [this](pop_tag p){
				return modifiers::test_additive_factor(ws.s.population_m.promotion_chance, this->ws, p, p);
			});
			auto d_trigger_amount = ve::apply(pop_v, [this](pop_tag p) {
				return modifiers::test_additive_factor(ws.s.population_m.demotion_chance, this->ws, p, p);
			});*/

			auto p_trigger_amount = modifiers::test_contiguous_additive_factor(ws.s.population_m.promotion_chance, ws, pop_v, pop_v);
			auto d_trigger_amount = modifiers::test_contiguous_additive_factor(ws.s.population_m.demotion_chance, ws, pop_v, pop_v);

			auto p_result =
				(ve::load(prov_nations, national_admin_eff) * sz)
				* (p_trigger_amount * ws.s.modifiers_m.global_defines.promotion_scale);

			auto d_result =
				sz * d_trigger_amount * ws.s.modifiers_m.global_defines.promotion_scale;

			ve::store(pop_v, promotion_amount, ve::select(focus_target, 0.0f, p_result));
			ve::store(pop_v, demotion_amount, ve::select(focus_target, 0.0f, d_result));
		}
	};

	void calculate_promotion_and_demotion_qnty(world_state& ws) {
		uint32_t pop_size = ws.w.population_s.pops.vector_size();
		uint32_t chunk_size = pop_size / (32ui32 * 16ui32);

		calculate_promotion_operation op(ws);

		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);
		if(chunk_index != 31)
			ve::execute_parallel<population::pop_tag>(chunk_size * 16ui32 * chunk_index, chunk_size * 16ui32 * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * 16ui32 * chunk_index, pop_size, op);
	}

	struct calculate_migration_operation {
		world_state const& ws;

		tagged_array_view<float, pop_tag> local_migration_amount;
		tagged_array_view<float, pop_tag> colonial_migration_amount;
		tagged_array_view<float, pop_tag> emmigration_amount;

		tagged_array_view<float const, pop_tag> pop_sizes;
		tagged_array_view<provinces::province_tag const, pop_tag> pop_location;
		tagged_array_view<bitfield_type const, pop_tag> poor_pops;
		tagged_array_view<bitfield_type const, pop_tag> middle_pops;
		tagged_array_view<nations::country_tag const, provinces::province_tag> province_owners;
		tagged_array_view<float const, provinces::province_tag> immigrant_push_modifier;
		tagged_array_view<float const, nations::country_tag> tech_colonial_migration;
		float const immigration_scale;


		calculate_migration_operation(world_state& w) : ws(w),
			local_migration_amount(w.w.population_s.pops.get_row<pop::size_change_from_local_migration>()),
			colonial_migration_amount(w.w.population_s.pops.get_row<pop::size_change_from_colonial_migration>()),
			emmigration_amount(w.w.population_s.pops.get_row<pop::size_change_from_emmigration>()),

			pop_sizes(w.w.population_s.pops.get_row<pop::size>()),
			pop_location(w.w.population_s.pops.get_row<pop::location>()),
			poor_pops(w.w.population_s.pops.get_row<pop::is_poor>()),
			middle_pops(w.w.population_s.pops.get_row<pop::is_middle>()),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			immigrant_push_modifier(w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::immigrant_push>(0)),
			tech_colonial_migration(w.w.nation_s.tech_attributes.get_row<technologies::tech_offset::colonial_migration>(0)),
			immigration_scale(w.s.modifiers_m.global_defines.immigration_scale)
			{}

		template<typename T>
		void operator()(T pop_v) {
			auto loc = ve::load(pop_v, pop_location);
			auto sz = ve::load(pop_v, pop_sizes);
			auto poor = ve::load(pop_v, poor_pops);
			auto mid = ve::load(pop_v, middle_pops);

			auto mod = ve::load(loc, immigrant_push_modifier);
			auto owner = ve::load(loc, province_owners);

			auto trigger_amount_local = modifiers::test_contiguous_additive_factor(ws.s.population_m.migration_chance, ws, pop_v, pop_v);
			auto trigger_amount_colonial = modifiers::test_contiguous_additive_factor(ws.s.population_m.colonialmigration_chance, ws, pop_v, pop_v);
			auto trigger_amount_emmigration = modifiers::test_contiguous_additive_factor(ws.s.population_m.emigration_chance, ws, pop_v, pop_v);

			auto owner_tech_mod = ve::load(owner, tech_colonial_migration);
			auto mod_sq = ve::multiply_and_add(mod, mod + 2.0f, 1.0f);
			auto adj_size = ve::select(mid | poor, sz, 0.0f);

			ve::store(pop_v, local_migration_amount, ve::multiply_and_add(mod, sz, sz) * (trigger_amount_local * immigration_scale));
			ve::store(pop_v, colonial_migration_amount,
				ve::multiply_and_add(mod, adj_size, adj_size) *
				ve::multiply_and_add(owner_tech_mod, immigration_scale, immigration_scale) *
				(trigger_amount_colonial));
			ve::store(pop_v, emmigration_amount, (sz * mod_sq) * (trigger_amount_emmigration * immigration_scale));
		}
	};

	void calculate_migration_qnty(world_state& ws) {
		uint32_t pop_size = ws.w.population_s.pops.vector_size();
		uint32_t chunk_size = pop_size / (32ui32 * 16ui32);

		calculate_migration_operation op(ws);

		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);
		if(chunk_index != 31)
			ve::execute_parallel<population::pop_tag>(chunk_size * 16ui32 * chunk_index, chunk_size * 16ui32 * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * 16ui32 * chunk_index, pop_size, op);
	}
}
