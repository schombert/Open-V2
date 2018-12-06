#include "common\\common.h"
#include "population_function.h"
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
	void init_rebel_faction_from_rebel_type(rebel_faction& faction, rebel_type& type) {
		faction.type = type.id;
		faction.icon = type.icon;
		faction.flags = type.flags;
	}

	pop_tag make_new_pop(world_state& ws) {
		return ws.w.population_s.pops.get_new();
	}

	void init_population_state(world_state& ws) {
		ws.w.population_s.pop_demographics.reset(aligned_32_issues_ideology_demo_size(ws));
	}

	void reset_state(population_state& s) {
		s.pop_arrays.reset();
		s.rebel_faction_arrays.reset();
		s.pop_movement_arrays.reset();
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

	float get_militancy_direct(world_state const& ws, pop_tag p) {
		return ws.w.population_s.pops.get<pop::militancy>(p) * 10.0f;
	}
	float get_literacy_direct(world_state const& ws, pop_tag p) {
		return ws.w.population_s.pops.get<pop::literacy>(p);
	}
	float get_consciousness_direct(world_state const& ws, pop_tag p) {
		return ws.w.population_s.pops.get<pop::consciousness>(p) * 10.0f;
	}

	float total_size_change(world_state const& ws, pop_tag p) {
		return ws.w.population_s.pops.get<pop::size_change_from_assimilation>(p)
			+ ws.w.population_s.pops.get<pop::size_change_from_combat>(p)
			+ ws.w.population_s.pops.get<pop::size_change_from_emmigration>(p)
			+ ws.w.population_s.pops.get<pop::size_change_from_growth>(p)
			+ ws.w.population_s.pops.get<pop::size_change_from_local_migration>(p)
			+ ws.w.population_s.pops.get<pop::size_change_from_type_change>(p);
	}

	economy::money_qnty_type desired_needs_spending(world_state const&, pop_tag) {
		return economy::money_qnty_type(0);
	}

	nations::country_tag get_pop_owner(world_state const& ws, pop_tag p) {
		auto loc = ws.w.population_s.pops.get<pop::location>(p);
		if(is_valid_index(loc)) {
			return ws.w.province_s.province_state_container.get<province_state::owner>(loc);
		}
		return nations::country_tag();
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

	void destroy_pop_movement(world_state& ws, pop_movement& m) {
		auto prange = get_range(ws.w.population_s.pop_arrays, m.member_pops);
		for(auto p : prange)
			ws.w.population_s.pops.set<pop::movement>(p, movement_tag());
		clear(ws.w.population_s.pop_arrays, m.member_pops);
	}

	void destroy_rebel_faction(world_state& ws, rebel_faction& r) {
		auto prange = get_range(ws.w.population_s.pop_arrays, r.member_pops);
		for(auto p : prange)
			ws.w.population_s.pops.set<pop::rebel_faction>(p, rebel_faction_tag());
		clear(ws.w.population_s.pop_arrays, r.member_pops);

		auto prov_range = get_range(ws.w.province_s.province_arrays, r.controlled_provinces);
		for(auto p : prov_range)
			ws.w.province_s.province_state_container.set<province_state::rebel_controller>(p, rebel_faction_tag());
		clear(ws.w.province_s.province_arrays, r.controlled_provinces);
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

	void trigger_rising(world_state&, rebel_faction&, nations::country_tag) {
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
						ideology_demo[i] = modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
					} else if(auto owner = get_pop_owner(ws, this_pop); bool(owner) && ws.w.nation_s.nations.get<nation::is_civilized>(owner)) {
						ideology_demo[i] = modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
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
				issues_demo[i] = modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
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
						ideology_demo[i] += total_pop_size * 0.25f * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
					} else if(auto owner = get_pop_owner(ws, this_pop); ws.w.nation_s.nations.get<nation::is_civilized>(owner)) {
						ideology_demo[i] += total_pop_size * 0.25f * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
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
					issues_demo[i] += 0.20f * owner_issue_change * owner_sr_modifier * total_pop_size * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
				else if(ws.s.issues_m.options[this_tag].type == issues::issue_group::political)
					issues_demo[i] += 0.20f * owner_issue_change * owner_pr_modifier * total_pop_size * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
				else
					issues_demo[i] += 0.20f * owner_issue_change * total_pop_size * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
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
		float const* const change_by_state;
		int32_t const* const pop_locations;
		int32_t const* const province_state_owners;
		float* const pop_literacy;

		literacy_update_operation(float const* a, int32_t const* b, int32_t const* c, float* d) : 
			change_by_state(a), pop_locations(b), province_state_owners(c), pop_literacy(d) {}

		template<typename T>
		__forceinline void operator()(T executor) {
			auto plocation = executor.load(pop_locations);
			auto state_indices = executor.gather_load(province_state_owners, plocation);
			auto change = executor.gather_load(change_by_state, state_indices);
			auto new_literacy = executor.load(pop_literacy) + change;
			executor.store(pop_literacy, ve::max(executor.zero(), ve::min(new_literacy, executor.constant(1.0f))));
		}
	};

	struct gather_literacy_change_operation {
		world_state const& ws;
		float const* const state_populations;
		int32_t const* const state_owners;
		float const* const education_efficiency_modifier;
		float const* const tech_education_efficiency_modifier;
		float const* const education_spending;
		float* const change_out;

		gather_literacy_change_operation(world_state const& w, float const* a, int32_t const* b, float const* c, float const* d, float const* e, float* f) : 
			ws(w), state_populations(a), state_owners(b), education_efficiency_modifier(c), tech_education_efficiency_modifier(d), education_spending(e),
			change_out(f) {}

		template<typename T>
		__forceinline void operator()(T executor) {
			auto owners = executor.load(state_owners);

			auto gath_education_efficiency_modifier = executor.gather_load(education_efficiency_modifier, owners);
			auto gath_tech_education_efficiency_modifier = executor.gather_load(tech_education_efficiency_modifier, owners);
			auto gath_education_spending = executor.gather_load(education_spending, owners);

			auto total_pop = executor.load(state_populations);

			auto clergy_amount = executor.generate([_this = this](uint32_t offset) {
				if(offset != 0) {
					nations::state_tag tag = nations::state_tag(nations::state_tag::value_base_t(offset), std::true_type());
					return _this->ws.w.nation_s.state_demographics.get(tag, population::to_demo_tag(_this->ws, _this->ws.s.population_m.clergy));
				} else {
					return 0.0f;
				}
			});

			executor.store(change_out,
				(((clergy_amount / total_pop) - executor.constant(ws.s.modifiers_m.global_defines.base_clergy_for_literacy))
				* executor.constant(1.0f / (ws.s.modifiers_m.global_defines.max_clergy_for_literacy - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)))
				* ((gath_education_efficiency_modifier + 1.0f)
				* (gath_tech_education_efficiency_modifier + 1.0f))
				* (gath_education_spending
				* executor.constant(ws.s.modifiers_m.global_defines.literacy_change_speed * 0.01f))
				);
		}
	};

	void update_literacy(world_state& ws) {
		concurrent_cache_aligned_buffer<float, nations::state_tag, true> change_by_state(ws.w.nation_s.states.size());

		auto state_populations = ws.w.nation_s.states.get_row<state::total_population>();
		auto state_owners = ws.w.nation_s.states.get_row<state::owner>();
		auto nat_edu_efficiency = ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::education_efficiency_modifier>(ws.w.nation_s.nations.size());
		auto tech_edu_efficiency = ws.w.nation_s.tech_attributes.get_row<technologies::tech_offset::education_efficiency>(ws.w.nation_s.nations.size());
		
		auto edu_spending = ws.w.nation_s.nations.get_row<nation::f_education_spending>();

		ve::execute_parallel(uint32_t(ws.w.nation_s.states.size() + 1),
			gather_literacy_change_operation(ws, state_populations.data(), (int32_t*)(state_owners.data()),
				nat_edu_efficiency.data(), tech_edu_efficiency.data(), edu_spending.data(), change_by_state.data()));
	
		change_by_state.padding() = 0.0f;

		auto pop_locations = ws.w.population_s.pops.get_row<pop::location>(); 
		auto province_states = ws.w.province_s.province_state_container.get_row<province_state::state_instance>();
		auto pop_literacy = ws.w.population_s.pops.get_row<pop::literacy>();

		uint32_t pop_size = uint32_t(ws.w.population_s.pops.size() + 1);
		uint32_t chunk_size = pop_size / (32ui32 * 16ui32);

		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);
		if(chunk_index != 31) {
			ve::execute_parallel(chunk_size * 16ui32 * chunk_index, chunk_size * 16ui32 * (chunk_index + 1ui32),
				literacy_update_operation(change_by_state.data(), (int32_t*)(pop_locations.data()), (int32_t*)(province_states.data()), pop_literacy.data()));
		} else {
			ve::execute_parallel_exact(chunk_size * 16ui32 * chunk_index, pop_size,
				literacy_update_operation(change_by_state.data(), (int32_t*)(pop_locations.data()), (int32_t*)(province_states.data()), pop_literacy.data()));
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
		float* const base_modifier_out;
		float* const non_accepted_modifier_out;

		int32_t const* const province_owners;
		int32_t const* const province_states;

		float const* const prov_militancy_mod;
		float const* const nat_militancy_mod;
		float const* const nat_war_exhaustion;
		float const* const nat_non_accepted_mil;
		float const* const tech_seperatism;
		float const* const nat_core_pop_mil;

		world_state const& ws;

		gather_militancy_by_province_operation(world_state const& w, float* a, float* b, int32_t const* c,
			int32_t const* d, float const* e, float const* f, float const* g, float const* h, float const* i, float const* j) :
			ws(w), base_modifier_out(a), non_accepted_modifier_out(b), province_owners(c), province_states(d), prov_militancy_mod(e),
			nat_militancy_mod(f), nat_war_exhaustion(g), nat_non_accepted_mil(h), tech_seperatism(i), nat_core_pop_mil(j) {}

		template<typename T>
		void operator()(T executor) {
			auto owner_indices = executor.load(province_owners);
			auto state_indices = executor.load(province_states);

			auto prov_mil_mod = executor.load(prov_militancy_mod);

			auto owner_mil_mod = executor.gather_load(nat_militancy_mod, owner_indices);
			auto owner_war_ex = executor.gather_load(nat_war_exhaustion, owner_indices);
			auto owner_non_accepted = executor.gather_load(nat_non_accepted_mil, owner_indices);
			auto owner_seperatism = executor.gather_load(tech_seperatism, owner_indices);
			auto owner_core_pop_mil = executor.gather_load(nat_core_pop_mil, owner_indices);

			//is_colonial_or_protectorate
			auto core_mask = executor.apply_for_mask([_this = this](int32_t state_index, uint32_t offset) {
				if(nations::is_colonial_or_protectorate(_this->ws, nations::state_tag(nations::state_tag::value_base_t(state_index), std::true_type())))
					return 0;
				else
					return -1;
			}, state_indices);

			executor.store(base_modifier_out,
				ve::multiply_and_add(
					owner_war_ex, 
					executor.constant(ws.s.modifiers_m.global_defines.mil_war_exhaustion),
					owner_mil_mod) + (prov_mil_mod + ve::select(core_mask, owner_core_pop_mil, executor.zero()))
				);
			executor.store(non_accepted_modifier_out,
				ve::multiply_and_add(
					(owner_seperatism + executor.constant(1.0f)),
					executor.constant(ws.s.modifiers_m.global_defines.mil_non_accepted),
					owner_non_accepted)
			);
		}
	};

	struct update_militancy_operation {
		float* const pop_militancy;

		float const* const base_prov_modifier;
		float const* const prov_non_accepted_modifier;

		int32_t const* const province_owners;
		int32_t const* const pop_locations;

		int8_t const* const pop_acceptance;

		float const* const pop_size;
		float const* const pop_satisfaction;
		float const* const pop_political_reform_support;
		float const* const pop_social_reform_support;

		world_state const& ws;

		update_militancy_operation(world_state const& w, float* mil_out, float const* a, float const* b, int32_t const* c, int32_t const* d,
			int8_t const* e, float const* f, float const* g, float const* h, float const* i) :
			ws(w), pop_militancy(mil_out), base_prov_modifier(a), prov_non_accepted_modifier(b), province_owners(c), pop_locations(d), 
			pop_acceptance(e), pop_size(f), pop_satisfaction(g), pop_political_reform_support(h), pop_social_reform_support(i) {}

		template<typename T>
		void operator()(T executor) {
			auto province_indices = executor.load(pop_locations);

			auto satisfaction = executor.load(pop_satisfaction);

			auto satisfaction_factor =
				ve::select(satisfaction < executor.constant(0.5f),
					ve::negate_multiply_and_add(
						executor.constant(ws.s.modifiers_m.global_defines.mil_no_life_need),
						satisfaction,
						executor.constant(ws.s.modifiers_m.global_defines.mil_no_life_need * 0.5f)
					),
					executor.zero())
				+ ve::select(satisfaction < executor.constant(1.5f),
					ve::negate_multiply_and_add(
						executor.constant(ws.s.modifiers_m.global_defines.mil_lack_everyday_need),
						satisfaction,
						executor.constant(ws.s.modifiers_m.global_defines.mil_lack_everyday_need * 1.5f)
					),
					executor.zero())
				+ ve::select(satisfaction > executor.constant(1.5f),
					ve::multiply_and_subtract(
						executor.constant(ws.s.modifiers_m.global_defines.mil_has_everyday_need),
						satisfaction,
						executor.constant(ws.s.modifiers_m.global_defines.mil_has_everyday_need * 1.5f)
					),
					executor.zero())
				+ ve::select(satisfaction > executor.constant(2.5f),
					ve::multiply_and_subtract(
						executor.constant(ws.s.modifiers_m.global_defines.mil_has_luxury_need),
						satisfaction,
						executor.constant(ws.s.modifiers_m.global_defines.mil_has_luxury_need * 2.5f)
					),
					executor.zero());

			ve::int_vector nation_indices = executor.gather_load(province_owners, province_indices);

			auto non_accepted_factor = ve::select(executor.load(pop_acceptance),
				executor.zero(),
				executor.gather_load(prov_non_accepted_modifier, province_indices));

			auto scaled_support_factors = executor.apply(
				[sf = ws.s.modifiers_m.global_defines.mil_ideology, rp = ws.s.modifiers_m.global_defines.mil_ruling_party, _this = this]
			(int32_t nation_index, uint32_t offset) {
				if(offset != 0) {
					auto nid = nations::country_tag(nations::country_tag::value_base_t(nation_index), std::true_type());
					auto pid = pop_tag(pop_tag::value_base_t(offset), std::true_type());

					auto pop_c_support = _this->ws.w.population_s.pop_demographics.get(pid, to_demo_tag(_this->ws, _this->ws.s.ideologies_m.conservative_ideology));
					auto ruling_ideology = _this->ws.w.nation_s.nations.get<nation::ruling_ideology>(nid);
					auto pop_rp_support = _this->ws.w.population_s.pop_demographics.get(pid, to_demo_tag(_this->ws, ruling_ideology));

					return pop_c_support * sf + pop_rp_support * rp;
				} else {
					return 0.0f;
				}
			}, nation_indices);
			
			auto support_factor = scaled_support_factors / executor.load(pop_size);

			auto rr_factor = ws.s.modifiers_m.global_defines.mil_require_reform;
			auto reforms_factor = ve::multiply_and_add(executor.constant(rr_factor), executor.load(pop_political_reform_support), executor.load(pop_social_reform_support) * executor.constant(rr_factor));
			
			auto change =
				((satisfaction_factor + non_accepted_factor) + (support_factor + reforms_factor) + executor.gather_load(base_prov_modifier, province_indices))
				* executor.constant(0.1f);
			auto new_militancy = executor.load(pop_militancy) + change;
			executor.store(pop_militancy, ve::max(executor.zero(), ve::min(new_militancy, executor.constant(1.0f))));
		}
	};

	void update_militancy(world_state& ws) {
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> base_modifier(ws.w.province_s.province_state_container.size());
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> non_accepted_modifier(ws.w.province_s.province_state_container.size());

		auto province_owners = ws.w.province_s.province_state_container.get_row<province_state::owner>();
		auto province_states = ws.w.province_s.province_state_container.get_row<province_state::state_instance>();
		auto province_mil_mod = ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_militancy_modifier>(ws.w.province_s.province_state_container.size());
		auto nat_mil_mod = ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_militancy_modifier>(ws.w.nation_s.nations.size());
		auto nat_war_exh = ws.w.nation_s.nations.get_row<nation::war_exhaustion>();
		auto nat_non_accepted_mil_mod = ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_militancy_modifier>(ws.w.nation_s.nations.size());
		auto nat_core_mil_mod = ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_militancy_modifier>(ws.w.nation_s.nations.size());
		auto tech_seperatism = ws.w.nation_s.tech_attributes.get_row<technologies::tech_offset::seperatism>(ws.w.nation_s.nations.size());

		/*
		float* const base_modifier_out;
		float* const non_accepted_modifier_out;

		int32_t const* const province_owners;
		int32_t const* const province_states;

		float const* const prov_militancy_mod;
		float const* const nat_militancy_mod;
		float const* const nat_war_exhaustion;
		float const* const nat_non_accepted_mil;
		float const* const tech_seperatism;
		float const* const nat_core_pop_mil;
		*/

		ve::execute_parallel(uint32_t(ws.w.province_s.province_state_container.size() + 1),
			gather_militancy_by_province_operation(ws, base_modifier.data(), non_accepted_modifier.data(),
			(int32_t*)(province_owners.data()), (int32_t*)(province_states.data()), province_mil_mod.data(), nat_mil_mod.data(),
			nat_war_exh.data(), nat_non_accepted_mil_mod.data(), tech_seperatism.data(), nat_core_mil_mod.data()));

		/*
		float* const pop_militancy;

		float const* const base_prov_modifier;
		float const* const prov_non_accepted_modifier;

		int32_t const* const province_owners;
		int32_t const* const pop_locations;

		int8_t const* const pop_acceptance;

		float const* const pop_size;
		float const* const pop_satisfaction;
		float const* const pop_political_reform_support;
		float const* const pop_social_reform_support;
		*/

		auto pop_locations = ws.w.population_s.pops.get_row<pop::location>();
		auto pop_militancy = ws.w.population_s.pops.get_row<pop::militancy>();
		auto pop_is_accepted = ws.w.population_s.pops.get_row<pop::is_accepted>();
		auto pop_sizes = ws.w.population_s.pops.get_row<pop::size>();
		auto pop_satisfaction = ws.w.population_s.pops.get_row<pop::needs_satisfaction>();

		auto prs = ws.w.population_s.pops.get_row<pop::political_interest>();
		auto srs = ws.w.population_s.pops.get_row<pop::social_interest>();

		uint32_t pop_size = uint32_t(ws.w.population_s.pops.size() + 1);
		uint32_t chunk_size = pop_size / (32ui32 * 16ui32);

		update_militancy_operation op(ws, pop_militancy.data(), base_modifier.data(), non_accepted_modifier.data(), (int32_t*)(province_owners.data()), 
			(int32_t*)(pop_locations.data()), (int8_t*)(pop_is_accepted.data()), pop_sizes.data(), pop_satisfaction.data(), prs.data(), srs.data());

		uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & 31);
		if(chunk_index != 31)
			ve::execute_parallel(chunk_size * 16ui32 * chunk_index, chunk_size * 16ui32 * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact(chunk_size * 16ui32 * chunk_index, pop_size, op);
	}
}
