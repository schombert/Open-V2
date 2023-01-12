#include "common\\common.h"
#include "population_functions.hpp"
#include "world_state\\world_state.h"
#include "modifiers\\modifier_functions.h"
#include "technologies\\technologies.h"
#include <ppl.h>
#include "concurrency_tools\\ve.h"
#include "issues\\issues_functions.h"
#include "nations\\nations_functions.hpp"
#include "cultures\\cultures_functions.h"
#include "provinces\\province_functions.hpp"
#include "concurrency_tools/ve_avx2.h"
#include <random>
#include "economy/economy_functions.h"

#undef min
#undef max

namespace population {

	pop_tag allocate_new_pop(world_state& ws) {
		auto const new_tag = ws.w.population_s.pops.get_new();
		ws.w.population_s.pop_demographics.ensure_capacity(to_index(new_tag) + 1);
		return new_tag;
	}


	namespace {
		template<bool protect>
		struct maybe_has_a_lock;

		template<>
		struct maybe_has_a_lock<true> {
			static std::mutex lock;
		};

		template<>
		struct maybe_has_a_lock<false> {};

		std::mutex maybe_has_a_lock<true>::lock;

		template<bool protect>
		pop_tag make_new_pop_internal(
			world_state& ws,
			float initial_size,
			float militancy,
			float consciousness,
			float literacy,
			provinces::province_tag location,
			pop_type_tag t,
			cultures::culture_tag c,
			cultures::religion_tag r) {

			assert(std::isfinite(initial_size) && initial_size >= 1.0f);
			assert(is_valid_index(t));

			auto const new_id = [&ws, location]() {
				if constexpr(protect) {
					std::lock_guard<std::mutex> guard(maybe_has_a_lock<true>().lock);
					auto const n = allocate_new_pop(ws);
					add_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(location), n);
					return n;
				} else {
					auto const n = allocate_new_pop(ws);
					add_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(location), n);
					return n;
				}
			}();
			auto const prov_owner = ws.w.province_s.province_state_container.get<province_state::owner>(location);

			ws.w.population_s.pops.set<pop::location>(new_id, location);
			ws.w.population_s.pops.set<pop::culture>(new_id, c);
			ws.w.population_s.pops.set<pop::religion>(new_id, r);

			ws.w.population_s.pops.set<pop::militancy>(new_id, militancy);
			ws.w.population_s.pops.set<pop::consciousness>(new_id, consciousness);
			ws.w.population_s.pops.set<pop::literacy>(new_id, literacy);

			change_pop_type(ws, new_id, t);
			init_pop_demographics(ws, new_id, initial_size);

			
			ws.w.population_s.pops.set<pop::is_accepted>(new_id, is_pop_accepted(ws, new_id, prov_owner));

			default_initialize_issues_and_ideology(ws, new_id);

			assert(ws.w.population_s.pops.get<pop::size>(new_id) == ws.w.population_s.pop_demographics.get(new_id, total_population_tag));

			return new_id;
		}
	}

	pop_tag make_new_pop_deferred(world_state& ws,
		float initial_size,
		float militancy,
		float consciousness,
		float literacy,
		provinces::province_tag location,
		pop_type_tag t,
		cultures::culture_tag c,
		cultures::religion_tag r,
		concurrency::task_group& tg) {
		
		auto const new_id = allocate_new_pop(ws);
		add_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(location), new_id);

		ws.w.population_s.pop_demographics.get(new_id, total_population_tag) = initial_size;
		ws.w.population_s.pops.set<pop::size>(new_id, initial_size);

		ws.w.population_s.pops.set<pop::location>(new_id, location);
		ws.w.population_s.pops.set<pop::culture>(new_id, c);
		ws.w.population_s.pops.set<pop::religion>(new_id, r);

		change_pop_type(ws, new_id, t);

		tg.run([&ws, new_id, initial_size, militancy, consciousness, literacy, location, t, c, r]() {
			if((ws.s.population_m.pop_types[t].flags & population::pop_type::is_employable) == 0)
				ws.w.population_s.pop_demographics.get(new_id, total_employment_tag) = initial_size;

			auto const prov_owner = ws.w.province_s.province_state_container.get<province_state::owner>(location);

			ws.w.population_s.pops.set<pop::militancy>(new_id, militancy);
			ws.w.population_s.pops.set<pop::consciousness>(new_id, consciousness);
			ws.w.population_s.pops.set<pop::literacy>(new_id, literacy);

			ws.w.population_s.pops.set<pop::is_accepted>(new_id, is_pop_accepted(ws, new_id, prov_owner));
			default_initialize_issues_and_ideology(ws, new_id);
		});

		return new_id;
	}

	pop_tag make_new_pop(
		world_state& ws,
		float initial_size,
		float militancy,
		float consciousness,
		float literacy,
		provinces::province_tag location,
		pop_type_tag t,
		cultures::culture_tag c,
		cultures::religion_tag r) {

		return make_new_pop_internal<false>(ws, initial_size, militancy, consciousness, literacy, location, t, c, r);
	}

	pop_tag concurrent_make_new_pop(
		world_state& ws,
		float initial_size,
		float militancy,
		float consciousness,
		float literacy,
		provinces::province_tag location,
		pop_type_tag t,
		cultures::culture_tag c,
		cultures::religion_tag r) {

		return make_new_pop_internal<true>(ws, initial_size, militancy, consciousness, literacy, location, t, c, r);
	}

	void init_population_state(world_state& ws) {
		ws.w.population_s.pop_demographics.reset(aligned_32_issues_ideology_demo_size(ws));

		ws.w.population_s.independence_rebel_support.resize(ws.s.culture_m.national_tags.size());
		ws.w.population_s.independence_movement_support.resize(ws.s.culture_m.national_tags.size());
	}

	void reset_state(population_state& s) {
		s.pop_arrays.reset();
	}

	void init_pop_demographics(world_state& ws, pop_tag p, float size) {
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

	void set_militancy_direct(world_state& ws, pop_tag p, float v) {
		ws.w.population_s.pops.set<pop::militancy>(p, std::clamp(v, 0.0f, 10.0f) / 10.0f);
	}
	void set_literacy_direct(world_state& ws, pop_tag p, float v) {
		ws.w.population_s.pops.set<pop::literacy>(p, std::clamp(v, 0.0f, 1.0f));
	}
	void set_consciousness_direct(world_state& ws, pop_tag p, float v) {
		ws.w.population_s.pops.set<pop::consciousness>(p, std::clamp(v, 0.0f, 10.0f) / 10.0f);
	}

	namespace {
		constexpr float drop_small_value(float v) {
			return v < 1.0f ? 0.0f : v;
		}
	}

	float total_size_change(world_state const& ws, pop_tag p) {
		auto const p_type = ws.w.population_s.pops.get<pop::type>(p);
		if(p_type != ws.s.population_m.slave) {
			return -std::ceil(std::max(ws.w.population_s.pops.get<pop::size_change_from_assimilation_away>(p), 0.0f))
				- drop_small_value(ws.w.population_s.pops.get<pop::size_change_from_combat>(p))
				- drop_small_value(ws.w.population_s.pops.get<pop::size_change_from_emigration>(p))
				+ ws.w.population_s.pops.get<pop::size_change_from_growth>(p)
				- drop_small_value(ws.w.population_s.pops.get<pop::size_change_from_local_migration>(p))
				- drop_small_value(std::abs(ws.w.population_s.pops.get<pop::size_change_from_type_change_away>(p)))
				;
		} else {
			return ws.w.population_s.pops.get<pop::size_change_from_growth>(p);
		}
	}

	bool is_dominant_issue(world_state const& ws, pop_tag id, issues::option_tag opt) {
		auto const issue_offset =
			&(ws.w.population_s.pop_demographics.get_row(id)[population::to_demo_tag(ws, issues::option_tag(0))]);
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> options_vector(issue_offset, ws.s.issues_m.tracked_options_count);
		return options_vector.maxCoeff() == issue_offset[to_index(opt)];
	}

	bool is_dominant_ideology(world_state const& ws, pop_tag id, ideologies::ideology_tag opt) {
		auto const ideology_offset =
			&(ws.w.population_s.pop_demographics.get_row(id)[population::to_demo_tag(ws, ideologies::ideology_tag(0))]);
		Eigen::Map<const Eigen::Matrix<float, -1, 1>> ideology_vector(ideology_offset, ws.s.ideologies_m.ideologies_count);
		return ideology_vector.maxCoeff() == ideology_offset[to_index(opt)];
	}


	void change_pop_type(world_state& ws, pop_tag this_pop, pop_type_tag new_type) {
		// todo: fix employment
		assert(is_valid_index(new_type));
		ws.w.population_s.pops.set<pop::type>(this_pop, new_type);

		auto const strata = ws.s.population_m.pop_types[new_type].flags & population::pop_type::strata_mask;
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
		assert(new_size != 0);
		ws.w.population_s.pop_demographics.get(this_pop, total_population_tag) = float(new_size);
		ws.w.population_s.pops.set<pop::size>(this_pop, float(new_size));
	}

	void grow_pop_immediate(world_state& ws, pop_tag this_pop, float size_change) {
		// todo: fix employment
		auto& sz = ws.w.population_s.pops.get<pop::size>(this_pop);
		sz += size_change;
		ws.w.population_s.pop_demographics.get(this_pop, total_population_tag) = sz;
		
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
		const auto location = ws.w.population_s.pops.get<pop::location>(this_pop);
		const auto local_rgo = ws.w.province_s.province_state_container.get<province_state::rgo_production>(location);
		const bool is_mine = (ws.s.economy_m.goods[local_rgo].flags & economy::good_definition::mined) != 0;
		change_pop_type(ws, this_pop, is_mine ? ws.s.economy_m.rgo_mine.workers[0].type : ws.s.economy_m.rgo_farm.workers[0].type);
	}

	void determine_farmer_and_laborer(scenario::scenario_manager& s) {
		for(uint32_t i = 0; i < std::extent_v<decltype(s.economy_m.rgo_farm.workers)>; ++i) {
			if(s.economy_m.rgo_farm.workers[i].type != s.economy_m.rgo_mine.workers[i].type) {
				if(is_valid_index(s.economy_m.rgo_mine.workers[i].type) && s.economy_m.rgo_mine.workers[i].type != s.population_m.slave)
					s.population_m.laborer = s.economy_m.rgo_mine.workers[i].type;
				if(is_valid_index(s.economy_m.rgo_farm.workers[i].type) && s.economy_m.rgo_farm.workers[i].type != s.population_m.slave)
					s.population_m.farmer = s.economy_m.rgo_farm.workers[i].type;
			}
		}
	}

	pop_type_tag fit_type_to_province(world_state const& ws, provinces::province_tag prov, pop_type_tag type) {
		auto const production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(prov);
		bool const is_mined = (ws.s.economy_m.goods[production].flags & economy::good_definition::mined) != 0;

		if(is_mined) {
			if(type == ws.s.population_m.farmer)
				return ws.s.population_m.laborer;
			else
				return type;
		} else {
			if(type == ws.s.population_m.laborer)
				return ws.s.population_m.farmer;
			else
				return type;
		}
	}

	void trigger_rising(world_state&, rebel_type_tag, nations::country_tag) {
		// todo
	}

	void default_initialize_issues_and_ideology(world_state& ws, pop_tag this_pop) {
		const auto total_pop_size = ws.w.population_s.pops.get<pop::size>(this_pop);
		if(total_pop_size > 0) {
			auto const ideology_demo = &(ws.w.population_s.pop_demographics.get_row(this_pop)[to_demo_tag(ws, ideologies::ideology_tag(0))]);
			auto const issues_demo = &(ws.w.population_s.pop_demographics.get_row(this_pop)[to_demo_tag(ws, issues::option_tag(0))]);
			auto const pop_type = ws.w.population_s.pops.get<pop::type>(this_pop);

			for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
				ideologies::ideology_tag const this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
				if(ws.w.ideology_s.ideology_enabled[this_tag] != 0ui8) {
					auto const pop_incl = ws.s.population_m.ideological_inclination.get(pop_type, this_tag);
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
				issues::option_tag const this_tag(static_cast<issues::option_tag::value_base_t>(i));
				auto const pop_incl = ws.s.population_m.issue_inclination.get(pop_type, this_tag);
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
	}

	void update_ideology_preference(world_state& ws, pop_tag this_pop) {	
		const auto total_pop_size = ws.w.population_s.pops.get<pop::size>(this_pop);
		if(total_pop_size > 0) {
			const auto ideology_demo = &(ws.w.population_s.pop_demographics.get_row(this_pop)[to_demo_tag(ws, ideologies::ideology_tag(0))]);
			const auto pop_type = ws.w.population_s.pops.get<pop::type>(this_pop);

			const int32_t icount = int32_t(ws.s.ideologies_m.ideologies_count);
			for(int32_t i = 0; i < icount; ++i) {
				ideologies::ideology_tag const this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
				if(ws.w.ideology_s.ideology_enabled[this_tag] != 0ui8) {
					auto const pop_incl = ws.s.population_m.ideological_inclination.get(pop_type, this_tag);
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
	}

	void update_issues_preference(world_state& ws, pop_tag this_pop) {
		const auto total_pop_size = ws.w.population_s.pops.get<pop::size>(this_pop);
		if(total_pop_size > 0) {
			auto const demo = ws.w.population_s.pop_demographics.get_row(this_pop);
			auto const issues_demo = &(demo[to_demo_tag(ws, issues::option_tag(0))]);
			auto const pop_type = ws.w.population_s.pops.get<pop::type>(this_pop);

			auto const location = ws.w.population_s.pops.get<pop::location>(this_pop);
			auto const owner = ws.w.province_s.province_state_container.get<province_state::owner>(location);

			auto const owner_pr_modifier = 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::political_reform_desire>(owner);
			auto const owner_sr_modifier = 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::social_reform_desire>(owner);
			auto const owner_issue_change = 1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::issue_change_speed>(owner);

			const int32_t icount = int32_t(ws.s.issues_m.tracked_options_count);
			for(int32_t i = 0; i < icount; ++i) {
				issues::option_tag const this_tag(static_cast<issues::option_tag::value_base_t>(i));
				auto const pop_incl = ws.s.population_m.issue_inclination.get(pop_type, this_tag);
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
	}

	void default_initialize_world_issues_and_ideology(world_state& ws) {
		ws.w.population_s.pops.parallel_for_each([&ws](pop_tag pt) {
			default_initialize_issues_and_ideology(ws, pt);
		});
	}

	constexpr uint32_t pop_update_frequency = 32ui32;
	constexpr uint32_t pop_update_group_size = uint32_t(std::max(16, ve::vector_size));

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
		RELEASE_INLINE void operator()(T pop_v) {
			auto location = ve::load(pop_v, pop_locations);
			auto state_indices = ve::load(location, province_state_owners);
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
		RELEASE_INLINE void operator()(T state_v) {
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
				ve::min(1.0f, (((clergy_amount / total_pop) - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)
					* (1.0f / (ws.s.modifiers_m.global_defines.max_clergy_for_literacy - ws.s.modifiers_m.global_defines.base_clergy_for_literacy))))
				* ((gath_education_efficiency_modifier + 1.0f)
					* (gath_tech_education_efficiency_modifier + 1.0f))
				* (gath_education_spending
					* (ws.s.modifiers_m.global_defines.literacy_change_speed * 0.01f))
			);
		}
	};

	float project_literacy_change(world_state const& ws, nations::state_tag state_v) {
		auto owners = ve::load(state_v, ws.w.nation_s.states.get_row<state::owner>());

		auto gath_education_efficiency_modifier = ve::load(owners, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::education_efficiency_modifier>(0));
		auto gath_tech_education_efficiency_modifier = ve::load(owners, ws.w.nation_s.tech_attributes.get_row<technologies::tech_offset::education_efficiency>(0));
		auto gath_education_spending = ve::load(owners, ws.w.nation_s.nations.get_row<nation::f_education_spending>());

		auto total_pop = ve::load(state_v, ws.w.nation_s.states.get_row<state::total_population>());

		auto clergy_amount = ws.w.nation_s.state_demographics.get(state_v, population::to_demo_tag(ws, ws.s.population_m.clergy));

		return
			std::min(1.0f, ((clergy_amount / total_pop) - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)
			* (1.0f / (ws.s.modifiers_m.global_defines.max_clergy_for_literacy - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)))
			* (gath_education_efficiency_modifier + 1.0f)
			* (gath_tech_education_efficiency_modifier + 1.0f)
			* gath_education_spending
			* (ws.s.modifiers_m.global_defines.literacy_change_speed * 0.01f);
		;
	}

	void update_literacy(world_state& ws) {
		concurrent_cache_aligned_buffer<float, nations::state_tag, true> change_by_state(ws.w.nation_s.states.size());

		ve::execute_parallel<nations::state_tag>(ws.w.nation_s.states.vector_size(),
			gather_literacy_change_operation(ws, change_by_state.view()));

		change_by_state.padding() = 0.0f;

		const uint32_t pop_size = ws.w.population_s.pops.vector_size();
		const uint32_t chunk_size = pop_size / (pop_update_frequency * pop_update_group_size);

		const uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));
		if(chunk_index != pop_update_frequency - 1) {
			ve::execute_parallel<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, chunk_size * pop_update_group_size * (chunk_index + 1ui32),
				literacy_update_operation(ws, change_by_state.view()));
		} else {
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, pop_size,
				literacy_update_operation(ws, change_by_state.view()));
		}
	}

	void update_pop_ideology_and_issues(world_state& ws) {
		const uint32_t pop_count = uint32_t(ws.w.population_s.pops.size());
		const auto chunk_size = (pop_count + pop_update_frequency - 1) / pop_update_frequency;
		const uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));

		concurrency::parallel_for(chunk_size * chunk_index, std::min(chunk_size * (chunk_index + 1), pop_count), [&ws](uint32_t i) {
			pop_tag const pt = pop_tag(pop_tag::value_base_t(i));

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
		tagged_array_view<float const, nations::country_tag> tech_separatism;
		tagged_array_view<float const, nations::country_tag> nat_core_pop_mil;

		world_state const& ws;

		gather_militancy_by_province_operation(world_state& w,
			tagged_array_view<float, provinces::province_tag> a,
			tagged_array_view<float, provinces::province_tag> b) :
			base_modifier_out(a), non_accepted_modifier_out(b),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			province_states(w.w.province_s.province_state_container.get_row<province_state::state_instance>()),
			prov_militancy_mod(w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_militancy_modifier>(0)),
			nat_militancy_mod(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_militancy_modifier>(0)),
			nat_war_exhaustion(w.w.nation_s.nations.get_row<nation::war_exhaustion>()),
			nat_non_accepted_mil(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_militancy_modifier>(0)),
			tech_separatism(w.w.nation_s.tech_attributes.get_row<technologies::tech_offset::seperatism>(0)),
			nat_core_pop_mil(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_militancy_modifier>(0)),
			ws(w) {}

		template<typename T>
		void operator()(T province_v) {
			auto owner_indices = ve::load(province_v, province_owners);
			auto state_indices = ve::load(province_v, province_states);

			auto prov_mil_mod = ve::load(province_v, prov_militancy_mod);

			auto owner_mil_mod = ve::load(owner_indices, nat_militancy_mod);
			auto owner_war_ex = ve::load(owner_indices, nat_war_exhaustion);
			auto owner_non_accepted = ve::load(owner_indices, nat_non_accepted_mil);
			auto owner_separatism = ve::load(owner_indices, tech_separatism);
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
				(owner_separatism + 1.0f),
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
			pop_militancy(w.w.population_s.pops.get_row<pop::militancy>()),
			base_prov_modifier(a),
			prov_non_accepted_modifier(b), province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			pop_locations(w.w.population_s.pops.get_row<pop::location>()),
			pop_acceptance(w.w.population_s.pops.get_row<pop::is_accepted>()),
			pop_size(w.w.population_s.pops.get_row<pop::size>()),
			pop_satisfaction(w.w.population_s.pops.get_row<pop::needs_satisfaction>()),
			pop_political_reform_support(w.w.population_s.pops.get_row<pop::political_interest>()),
			pop_social_reform_support(w.w.population_s.pops.get_row<pop::social_interest>()),
			ws(w) {}

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
					auto const pop_c_support = _this->ws.w.population_s.pop_demographics.get(pid, to_demo_tag(_this->ws, _this->ws.s.ideologies_m.conservative_ideology));
					auto const pop_rp_support = _this->ws.w.population_s.pop_demographics.get(pid, to_demo_tag(_this->ws, ri));

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

	float project_militancy_change(world_state const& ws, pop_tag pop_v) {
		auto const province_v = ws.w.population_s.pops.get<pop::location>(pop_v);

		auto owner_indices = ve::load(province_v, ws.w.province_s.province_state_container.get_row<province_state::owner>());
		auto state_indices = ve::load(province_v, ws.w.province_s.province_state_container.get_row<province_state::state_instance>());

		auto prov_mil_mod = ve::load(province_v, ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_militancy_modifier>(0));

		auto owner_mil_mod = ve::load(owner_indices, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_militancy_modifier>(0));
		auto owner_war_ex = ve::load(owner_indices, ws.w.nation_s.nations.get_row<nation::war_exhaustion>());
		auto owner_non_accepted = ve::load(owner_indices, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_militancy_modifier>(0));
		auto owner_separatism = ve::load(owner_indices, ws.w.nation_s.tech_attributes.get_row<technologies::tech_offset::seperatism>(0));
		auto owner_core_pop_mil = ve::load(owner_indices, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_militancy_modifier>(0));

		bool core_mask = nations::is_colonial_or_protectorate(ws, state_indices);
			
		auto prov_non_accepted_modifier =
			(owner_separatism + 1.0f) * ws.s.modifiers_m.global_defines.mil_non_accepted
			+ owner_non_accepted;

		auto satisfaction = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::needs_satisfaction>());

		auto satisfaction_factor =
			ve::select(satisfaction < 0.5f,
				ve::negate_multiply_and_add(
					ws.s.modifiers_m.global_defines.mil_no_life_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_no_life_need * 0.5f
				),
				0.0f)
			+ ve::select(satisfaction < 1.5f,
				ve::negate_multiply_and_add(
					ws.s.modifiers_m.global_defines.mil_lack_everyday_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_lack_everyday_need * 1.5f
				),
				0.0f)
			+ ve::select(satisfaction > 1.5f,
				ve::multiply_and_subtract(
					ws.s.modifiers_m.global_defines.mil_has_everyday_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_has_everyday_need * 1.5f
				),
				0.0f)
			+ ve::select(satisfaction > 2.5f,
				ve::multiply_and_subtract(
					ws.s.modifiers_m.global_defines.mil_has_luxury_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_has_luxury_need * 2.5f
				),
				0.0f);

		auto non_accepted_factor = ve::select(ve::load(pop_v, ws.w.population_s.pops.get_row<pop::is_accepted>()),
			0.0f,
			prov_non_accepted_modifier);

		auto ruling_ideology = ve::load(owner_indices, ws.w.nation_s.nations.get_row<nation::ruling_ideology>());

		auto const psize = ws.w.population_s.pops.get<pop::size>(pop_v) != 0 ? ws.w.population_s.pops.get<pop::size>(pop_v) : 1.0f;
		auto const pop_c_support = ws.w.population_s.pop_demographics.get(pop_v, to_demo_tag(ws, ws.s.ideologies_m.conservative_ideology)) / psize;
		auto const pop_rp_support = ws.w.population_s.pop_demographics.get(pop_v, to_demo_tag(ws, ruling_ideology)) / psize;

		return 
			(satisfaction_factor
			+ non_accepted_factor
			+ pop_c_support * ws.s.modifiers_m.global_defines.mil_ideology
			+ pop_rp_support * ws.s.modifiers_m.global_defines.mil_ruling_party
			+ ve::load(pop_v, ws.w.population_s.pops.get_row<pop::political_interest>()) * ws.s.modifiers_m.global_defines.mil_require_reform
			+ ve::load(pop_v, ws.w.population_s.pops.get_row<pop::social_interest>()) * ws.s.modifiers_m.global_defines.mil_require_reform
			+ owner_war_ex * ws.s.modifiers_m.global_defines.mil_war_exhaustion
			+ owner_mil_mod
			+ prov_mil_mod
			+ ve::select(core_mask, owner_core_pop_mil, 0.0f))
			* 0.1f;
	}

	void update_militancy(world_state& ws) {
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> base_modifier(ws.w.province_s.province_state_container.size());
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> non_accepted_modifier(ws.w.province_s.province_state_container.size());


		ve::execute_parallel<provinces::province_tag>(ws.w.province_s.province_state_container.vector_size(),
			gather_militancy_by_province_operation(ws, base_modifier.view(), non_accepted_modifier.view()));


		uint32_t const pop_size = ws.w.population_s.pops.vector_size();
		uint32_t const chunk_size = pop_size / (pop_update_frequency * pop_update_group_size);

		update_militancy_operation op(ws, base_modifier.view(), non_accepted_modifier.view());

		uint32_t const chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));
		if(chunk_index != pop_update_frequency - 1)
			ve::execute_parallel<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, chunk_size * pop_update_group_size * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, pop_size, op);
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
			fixed_factor_out(a), clergy_factor_out(b), literacy_factor_out(c),
			owner_indices(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			state_indices(w.w.province_s.province_state_container.get_row<province_state::state_instance>()),
			nat_mod_literacy_impact(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::literacy_con_impact>(0)),
			nat_plurality(w.w.nation_s.nations.get_row<nation::plurality>()),
			prov_population(w.w.province_s.province_state_container.get_row<province_state::total_population>()),
			nat_con_mod(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_consciousness_modifier>(0)),
			prov_con_mod(w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_consciousness_modifier>(0)),
			nat_core_con_mod(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_consciousness_modifier>(0)),
			ws(w) {}

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
			tagged_array_view<float const, provinces::province_tag> j) :
			pop_consciousness(w.w.population_s.pops.get_row<pop::consciousness>()),
			pop_locations(w.w.population_s.pops.get_row<pop::location>()),
			owner_indices(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			pop_literacy(w.w.population_s.pops.get_row<pop::literacy>()),
			pop_satisfaction(w.w.population_s.pops.get_row<pop::needs_satisfaction>()),
			nat_non_accepted_con(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_consciousness_modifier>(0)),
			pop_is_poor(w.w.population_s.pops.get_row<pop::is_poor>()),
			pop_is_accepted(w.w.population_s.pops.get_row<pop::is_accepted>()),
			prov_fixed_factor(h),
			prov_clergy_factor(i), prov_literacy_factor(j), ws(w) {}

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

	float project_consciousness_change(world_state const& ws, pop_tag pop_v) {
		auto owners = get_pop_owner(ws, pop_v);
		auto province_v = ws.w.population_s.pops.get<pop::location>(pop_v);
		auto states = ve::load(province_v, ws.w.province_s.province_state_container.get_row<province_state::state_instance>());
		auto core_mask = nations::is_colonial_or_protectorate(ws, states);

		auto colonial_multiplier = ve::select(core_mask, 1.0f, ws.s.modifiers_m.global_defines.con_colonial_factor);

		auto const literacy_factor = 
			colonial_multiplier
			* (1.0f + ve::load(owners, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::literacy_con_impact>(0)))
			* ve::load(owners, ws.w.nation_s.nations.get_row<nation::plurality>())
			* ws.s.modifiers_m.global_defines.con_literacy;

		auto clergy_populations = ws.w.province_s.province_demographics.get(province_v, to_demo_tag(ws, ws.s.population_m.clergy));
		auto const prov_pop = ve::load(province_v, ws.w.province_s.province_state_container.get_row<province_state::total_population>());
		auto const clergy_factor = colonial_multiplier * clergy_populations / (prov_pop > 0.0f ? prov_pop : 1.0f);

		auto lux_satisfaction = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::needs_satisfaction>()) - 2.0f;
		auto base_sum =
			clergy_factor *
				ve::select(ve::load(pop_v, ws.w.population_s.pops.get_row<pop::is_poor>()), ws.s.modifiers_m.global_defines.con_poor_clergy, ws.s.modifiers_m.global_defines.con_midrich_clergy)
			+ ve::load(province_v, ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_consciousness_modifier>(0))
			+ ve::load(owners, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_consciousness_modifier>(0))
			+ ve::select(core_mask,
				ve::load(owners, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_consciousness_modifier>(0)),
				0.0f)
			+ literacy_factor * ve::load(pop_v, ws.w.population_s.pops.get_row<pop::literacy>())
			+ ve::select(lux_satisfaction > 0.0f,
					lux_satisfaction * ws.s.modifiers_m.global_defines.con_luxury_goods,
					0.0f)
			+ ve::select(ve::load(pop_v, ws.w.population_s.pops.get_row<pop::is_accepted>()),
				0.0f,
				ve::load(owners, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_consciousness_modifier>(0)))
			;

		return base_sum * 0.1f;
	}

	void update_consciousness(world_state& ws) {
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> fixed_factor(ws.w.province_s.province_state_container.size());
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> clergy_factor(ws.w.province_s.province_state_container.size());
		concurrent_cache_aligned_buffer<float, provinces::province_tag, true> literacy_factor(ws.w.province_s.province_state_container.size());


		ve::execute_parallel<provinces::province_tag>(ws.w.province_s.province_state_container.vector_size(),
			gather_consciousness_factors_operation(ws, fixed_factor.view(), clergy_factor.view(), literacy_factor.view()));

		clergy_factor.padding() = 0.0f;

		uint32_t const pop_size = ws.w.population_s.pops.vector_size();
		uint32_t const chunk_size = pop_size / (pop_update_frequency * pop_update_group_size);

		update_consciousness_operation op(ws, fixed_factor.view(), clergy_factor.view(), literacy_factor.view());

		uint32_t const chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));
		if(chunk_index != pop_update_frequency - 1)
			ve::execute_parallel<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, chunk_size * pop_update_group_size * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, pop_size, op);
	}

	struct calculate_promotion_operation {
		world_state const& ws;

		tagged_array_view<float, pop_tag> change_amount;

		tagged_array_view<pop_type_tag const, pop_tag> pop_types;
		tagged_array_view<float const, pop_tag> pop_sizes;
		tagged_array_view<provinces::province_tag const, pop_tag> pop_location;
		tagged_array_view<nations::country_tag const, provinces::province_tag> province_owners;
		tagged_array_view<float const, nations::country_tag> national_admin_eff;

		tagged_array_view<nations::state_tag const, provinces::province_tag> province_state_instances;
		tagged_array_view<modifiers::national_focus_tag const, nations::state_tag> states_focuses;
		tagged_array_view<pop_type_tag const, modifiers::national_focus_tag> national_focus_type;

		calculate_promotion_operation(world_state& w) : ws(w),
			change_amount(w.w.population_s.pops.get_row<pop::size_change_from_type_change_away>()),
			pop_types(w.w.population_s.pops.get_row<pop::type>()),
			pop_sizes(w.w.population_s.pops.get_row<pop::size>()),
			pop_location(w.w.population_s.pops.get_row<pop::location>()),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			national_admin_eff(w.w.nation_s.nations.get_row<nation::national_administrative_efficiency>()),
			province_state_instances(w.w.province_s.province_state_container.get_row<province_state::state_instance>()),
			states_focuses(w.w.nation_s.states.get_row<state::owner_national_focus>()),
			national_focus_type(w.s.modifiers_m.focus_to_pop_types.view()) {}

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
				ve::max((ve::load(prov_nations, national_admin_eff) * sz)
					* (p_trigger_amount * ws.s.modifiers_m.global_defines.promotion_scale), 0.0f);

			auto d_result =
				ve::max(sz * d_trigger_amount * ws.s.modifiers_m.global_defines.promotion_scale, 0.0f);

			ve::store(pop_v, change_amount, ve::select(focus_target, 0.0f, p_result - d_result));
		}
	};

	float project_promotion_amount(world_state const& ws, pop_tag pop_v) {
		auto loc = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::location>());
		auto sz = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::size>());

		auto prov_states = ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::state_instance>());
		auto prov_nations = ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::owner>());

		auto state_focus = ve::load(prov_states, ws.w.nation_s.states.get_row<state::owner_national_focus>());
		auto focus_target = (ve::load(state_focus, ws.s.modifiers_m.focus_to_pop_types.view()) == ve::load(pop_v, ws.w.population_s.pops.get_row<pop::type>()));

		auto p_trigger_amount = modifiers::test_additive_factor(ws.s.population_m.promotion_chance, ws, pop_v, pop_v);
		auto d_trigger_amount = modifiers::test_additive_factor(ws.s.population_m.demotion_chance, ws, pop_v, pop_v);

		auto p_result =
			ve::max((ve::load(prov_nations, ws.w.nation_s.nations.get_row<nation::national_administrative_efficiency>()) * sz)
				* (p_trigger_amount * ws.s.modifiers_m.global_defines.promotion_scale), 0.0f);

		auto d_result =
			ve::max(sz * d_trigger_amount * ws.s.modifiers_m.global_defines.promotion_scale, 0.0f);

		return focus_target ? 0.0f : p_result - d_result;

	}

	void calculate_promotion_and_demotion_qnty(world_state& ws) {
		const uint32_t pop_size = ws.w.population_s.pops.vector_size();
		const uint32_t chunk_size = pop_size / (pop_update_frequency * pop_update_group_size);

		calculate_promotion_operation op(ws);

		const uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));
		if(chunk_index != pop_update_frequency - 1)
			ve::execute_parallel<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, chunk_size * pop_update_group_size * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, pop_size, op);
	}

	struct calculate_migration_operation {
		world_state const& ws;

		tagged_array_view<float, pop_tag> local_migration_amount;
		// tagged_array_view<float, pop_tag> colonial_migration_amount;
		tagged_array_view<float, pop_tag> emigration_amount;

		tagged_array_view<float const, pop_tag> pop_sizes;
		tagged_array_view<provinces::province_tag const, pop_tag> pop_location;
		tagged_array_view<bitfield_type const, provinces::province_tag> provinces_are_colonial;
		// tagged_array_view<bitfield_type const, pop_tag> poor_pops;
		// tagged_array_view<bitfield_type const, pop_tag> middle_pops;
		tagged_array_view<nations::country_tag const, provinces::province_tag> province_owners;
		tagged_array_view<float const, provinces::province_tag> immigrant_push_modifier;
		// tagged_array_view<float const, nations::country_tag> tech_colonial_migration;
		tagged_array_view<bitfield_type const, nations::country_tag> owner_is_civilized;
		float const immigration_scale;


		calculate_migration_operation(world_state& w) : ws(w),
			local_migration_amount(w.w.population_s.pops.get_row<pop::size_change_from_local_migration>()),
			// colonial_migration_amount(w.w.population_s.pops.get_row<pop::size_change_from_colonial_migration>()),
			emigration_amount(w.w.population_s.pops.get_row<pop::size_change_from_emigration>()),

			pop_sizes(w.w.population_s.pops.get_row<pop::size>()),
			pop_location(w.w.population_s.pops.get_row<pop::location>()),
			provinces_are_colonial(w.w.province_s.province_state_container.get_row<province_state::is_non_state>()),
			// poor_pops(w.w.population_s.pops.get_row<pop::is_poor>()),
			// middle_pops(w.w.population_s.pops.get_row<pop::is_middle>()),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			immigrant_push_modifier(w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::immigrant_push>(0)),
			// tech_colonial_migration(w.w.nation_s.tech_attributes.get_row<technologies::tech_offset::colonial_migration>(0)),
			owner_is_civilized(ws.w.nation_s.nations.get_row<nation::is_civilized>()),
			immigration_scale(w.s.modifiers_m.global_defines.immigration_scale) {}

		template<typename T>
		void operator()(T pop_v) {
			auto loc = ve::load(pop_v, pop_location);
			auto sz = ve::load(pop_v, pop_sizes);
			//auto poor = ve::load(pop_v, poor_pops);
			//auto mid = ve::load(pop_v, middle_pops);

			auto mod = ve::load(loc, immigrant_push_modifier);
			auto owner = ve::load(loc, province_owners);

			auto trigger_amount_local = modifiers::test_contiguous_additive_factor(
				ws.s.population_m.migration_chance, ws, pop_v, pop_v);
			//auto trigger_amount_colonial = modifiers::test_contiguous_additive_factor(
			//	ws.s.population_m.colonial_migration_chance, ws, pop_v, pop_v);
			auto trigger_amount_emigration = modifiers::test_contiguous_additive_factor(
				ws.s.population_m.emigration_chance, ws, pop_v, pop_v);

			auto owner_civilization = ve::load(owner, owner_is_civilized);
			auto province_not_colonial = ~(ve::load(loc, provinces_are_colonial));
			// auto owner_tech_mod = ve::load(owner, tech_colonial_migration);
			auto mod_sq = ve::multiply_and_add(mod, mod + 2.0f, 1.0f);
			// auto adj_size = ve::select(mid | poor, sz, 0.0f);

			ve::store(pop_v, local_migration_amount, ve::select(province_not_colonial & ve::is_valid_index(owner), ve::multiply_and_add(mod, sz, sz) * (trigger_amount_local * immigration_scale), 0.0f));
			//ve::store(pop_v, colonial_migration_amount,
			//	ve::multiply_and_add(mod, adj_size, adj_size) *
			//	ve::multiply_and_add(owner_tech_mod, immigration_scale, immigration_scale) *
			//	(trigger_amount_colonial));
			ve::store(pop_v, emigration_amount, ve::select(owner_civilization & province_not_colonial, (sz * mod_sq) * (trigger_amount_emigration * immigration_scale), 0.0f));
		}
	};

	float project_emigration_amount(world_state const& ws, pop_tag pop_v) {
		auto loc = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::location>());
		auto sz = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::size>());

		auto mod = ve::load(loc, ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::immigrant_push>(0));
		auto owner = ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::owner>());

		auto trigger_amount_emigration = modifiers::test_additive_factor(
			ws.s.population_m.emigration_chance, ws, pop_v, pop_v);

		auto owner_civilization = ve::load(owner, ws.w.nation_s.nations.get_row<nation::is_civilized>());
		auto province_colonial = ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::is_non_state>());
		auto mod_sq = ve::multiply_and_add(mod, mod + 2.0f, 1.0f);

		return ve::select(owner_civilization && !province_colonial, (sz * mod_sq) * (trigger_amount_emigration * ws.s.modifiers_m.global_defines.immigration_scale), 0.0f);
	}
	float project_migration_amount(world_state const& ws, pop_tag pop_v) {
		auto loc = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::location>());
		auto sz = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::size>());

		auto mod = ve::load(loc, ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::immigrant_push>(0));
		auto owner = ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::owner>());

		auto trigger_amount_local = modifiers::test_additive_factor(
			ws.s.population_m.migration_chance, ws, pop_v, pop_v);

		auto province_colonial = ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::is_non_state>());
		return ve::select(province_colonial || !is_valid_index(owner), 0.0f, ve::multiply_and_add(mod, sz, sz) * (trigger_amount_local * ws.s.modifiers_m.global_defines.immigration_scale));
	}

	void calculate_migration_qnty(world_state& ws) {
		const uint32_t pop_size = ws.w.population_s.pops.vector_size();
		const uint32_t chunk_size = pop_size / (pop_update_frequency * pop_update_group_size);

		calculate_migration_operation op(ws);

		const uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));
		if(chunk_index != pop_update_frequency - 1)
			ve::execute_parallel<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, chunk_size * pop_update_group_size * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, pop_size, op);
	}

	struct calculate_assimilation_operation {
		world_state const& ws;

		tagged_array_view<float, pop_tag> assimilation_amount;

		tagged_array_view<float const, pop_tag> pop_sizes;
		tagged_array_view<provinces::province_tag const, pop_tag> pop_location;
		tagged_array_view<cultures::religion_tag const, pop_tag> pop_religions;
		tagged_array_view<bitfield_type const, pop_tag> is_accepted;
		tagged_array_view<nations::country_tag const, provinces::province_tag> province_owners;
		tagged_array_view<float const, provinces::province_tag> province_assimilation_mod;
		tagged_array_view<float const, nations::country_tag> national_assimilation_mod;
		tagged_array_view<cultures::religion_tag const, nations::country_tag> national_religions;
		float const conversion_scale;
		float const assimilation_scale;


		calculate_assimilation_operation(world_state& w) : ws(w),
			assimilation_amount(w.w.population_s.pops.get_row<pop::size_change_from_assimilation_away>()),
			pop_sizes(w.w.population_s.pops.get_row<pop::size>()),
			pop_location(w.w.population_s.pops.get_row<pop::location>()),
			pop_religions(w.w.population_s.pops.get_row<pop::religion>()),
			is_accepted(w.w.population_s.pops.get_row<pop::is_accepted>()),
			province_owners(w.w.province_s.province_state_container.get_row<province_state::owner>()),
			province_assimilation_mod(w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::assimilation_rate>(0)),
			national_assimilation_mod(w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_assimilation_rate>(0)),
			national_religions(ws.w.nation_s.nations.get_row<nation::national_religion>()),
			conversion_scale(w.s.modifiers_m.global_defines.conversion_scale),
			assimilation_scale(w.s.modifiers_m.global_defines.assimilation_scale) {}

		template<typename T>
		void operator()(T pop_v) {
			auto loc = ve::load(pop_v, pop_location);
			auto sz = ve::load(pop_v, pop_sizes);
			auto accepted_culture = ve::load(pop_v, is_accepted);
			
			auto prov_mod = ve::load(loc, province_assimilation_mod);
			auto owner = ve::load(loc, province_owners);
			auto national_mod = ve::load(owner, national_assimilation_mod);
			auto is_nat_religion = ve::load(pop_v, pop_religions) == ve::load(owner, national_religions);

			auto trigger_amount_religious = modifiers::test_contiguous_additive_factor(
				ws.s.population_m.conversion_chance, ws, pop_v, pop_v);
			auto trigger_amount_cultural = modifiers::test_contiguous_additive_factor(
				ws.s.population_m.assimilation_chance, ws, pop_v, pop_v);

			auto result = ve::max(sz * ve::select(accepted_culture,
				ve::select(is_nat_religion, 0.0f, conversion_scale * trigger_amount_religious),
				ve::multiply_and_add(prov_mod, assimilation_scale, assimilation_scale) *
				ve::multiply_and_add(national_mod, trigger_amount_cultural, trigger_amount_cultural)), 0.0f);

			ve::store(pop_v, assimilation_amount, result);
		}
	};

	float project_assimilation_amount(world_state const& ws, pop_tag pop_v) {
		auto loc = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::location>());
		auto sz = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::size>());
		auto accepted_culture = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::is_accepted>());

		auto prov_mod = ve::load(loc, ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::assimilation_rate>(0));
		auto owner = ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::owner>());
		auto national_mod = ve::load(owner, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_assimilation_rate>(0));
		auto is_nat_religion = ve::load(pop_v, ws.w.population_s.pops.get_row<pop::religion>()) == ve::load(owner, ws.w.nation_s.nations.get_row<nation::national_religion>());

		auto trigger_amount_religious = modifiers::test_additive_factor(
			ws.s.population_m.conversion_chance, ws, pop_v, pop_v);
		auto trigger_amount_cultural = modifiers::test_additive_factor(
			ws.s.population_m.assimilation_chance, ws, pop_v, pop_v);

		return ve::max(sz * ve::select(accepted_culture,
			ve::select(is_nat_religion, 0.0f, ws.s.modifiers_m.global_defines.conversion_scale * trigger_amount_religious),
			ve::multiply_and_add(prov_mod, ws.s.modifiers_m.global_defines.assimilation_scale, ws.s.modifiers_m.global_defines.assimilation_scale) *
			ve::multiply_and_add(national_mod, trigger_amount_cultural, trigger_amount_cultural)), 0.0f);
	}

	void calculate_assimilation_qnty(world_state& ws) {
		const uint32_t pop_size = ws.w.population_s.pops.vector_size();
		const uint32_t chunk_size = pop_size / (pop_update_frequency * pop_update_group_size);

		calculate_assimilation_operation op(ws);

		const uint32_t chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));
		if(chunk_index != pop_update_frequency - 1)
			ve::execute_parallel<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, chunk_size * pop_update_group_size * (chunk_index + 1ui32), op);
		else
			ve::execute_parallel_exact<population::pop_tag>(chunk_size * pop_update_group_size * chunk_index, pop_size, op);
	}

	constexpr float max_movement_support = 0.5f;
	constexpr float max_rebel_support = 0.5f;

	inline auto make_movement_acc(world_state const& ws, cultures::culture_tag holder_pc) {
		return ve::make_value_accumulator<population::pop_tag>([&ws, holder_pc](auto v) {
			const static float con_limit = ws.s.modifiers_m.global_defines.issue_movement_leave_limit / 10.0f;
			const static float inv_con_limit = 1.0f / (1.0f - ws.s.modifiers_m.global_defines.issue_movement_leave_limit / 10.0f);

			auto pop_con = ve::load(v, ws.w.population_s.pops.get_row<pop::consciousness>());
			return ve::select(
				ve::load(v, ws.w.population_s.pops.get_row<pop::is_accepted>())
				| (pop_con < con_limit)
				| (ve::load(v, ws.w.population_s.pops.get_row<pop::culture>()) != holder_pc),
				0.0f,
				((pop_con - con_limit) * inv_con_limit) * ve::load(v, ws.w.population_s.pops.get_row<pop::literacy>()) * (max_movement_support * ve::load(v, ws.w.population_s.pops.get_row<pop::size>())));
		});
	}

	inline auto make_rebel_acc(world_state const& ws, cultures::culture_tag holder_pc) {
		return ve::make_value_accumulator<population::pop_tag>([&ws, holder_pc](auto v) {
			const static float mil_limit = ws.s.modifiers_m.global_defines.mil_to_join_rebel / 10.0f;
			const static float inv_mil_limit = 1.0f / (1.0f - ws.s.modifiers_m.global_defines.mil_to_join_rebel / 10.0f);

			auto pop_mil = ve::load(v, ws.w.population_s.pops.get_row<pop::militancy>());
			return ve::select(
				ve::load(v, ws.w.population_s.pops.get_row<pop::is_accepted>())
				| (pop_mil < mil_limit)
				| (ve::load(v, ws.w.population_s.pops.get_row<pop::culture>()) != holder_pc),
				0.0f,
				((pop_mil - mil_limit) * inv_mil_limit) * (max_rebel_support * ve::load(v, ws.w.population_s.pops.get_row<pop::size>())));
		});
	}

	void update_non_union_independence_movement_and_rebels(world_state& ws, cultures::national_tag t) {
		float const old_rebel_support = ws.w.population_s.independence_rebel_support[t];
		float const old_movement_support = ws.w.population_s.independence_movement_support[t];

		auto holder = ws.w.culture_s.tags_to_holders[t];
		auto cores = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[t].core_provinces);
		auto holder_pc = ws.w.nation_s.nations.get<nation::primary_culture>(holder);

		auto movement_acc = make_movement_acc(ws, holder_pc);
		auto rebel_acc = make_rebel_acc(ws, holder_pc);

		for(auto p : cores) {
			if(ws.w.province_s.province_state_container.get<province_state::owner>(p) != holder
				&& t == cultures::dominant_tag_for_culture(ws, p, holder_pc)) {

				provinces::for_each_pop(ws, p, [&ws, &movement_acc, &rebel_acc](population::pop_tag po) {
					movement_acc.add_value(po);
					rebel_acc.add_value(po);
				});
			}
		}

		float const total_movement_support = movement_acc.flush();
		float const total_rebel_support = rebel_acc.flush();

		ws.w.population_s.independence_rebel_support[t] = old_rebel_support < total_rebel_support ? (old_rebel_support + total_rebel_support) / 2.0f : total_rebel_support;
		ws.w.population_s.independence_movement_support[t] = old_movement_support < total_movement_support ? (old_movement_support + total_movement_support) / 2.0f : total_movement_support;
	}


	void update_union_independence_movement_and_rebels(world_state& ws, cultures::national_tag t) {
		float const old_rebel_support = ws.w.population_s.independence_rebel_support[t];
		float const old_movement_support = ws.w.population_s.independence_movement_support[t];

		auto const holder = ws.w.culture_s.tags_to_holders[t];
		auto const cores = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[t].core_provinces);
		auto const tag_group = ws.s.culture_m.tags_to_groups[t];

		auto const group_cultures = ws.s.culture_m.culture_by_culture_group.get_range(tag_group);
		int32_t const cultures_count = int32_t(group_cultures.second - group_cultures.first);

		using acc_pair = std::pair<decltype(make_movement_acc(ws, cultures::culture_tag())), decltype(make_rebel_acc(ws, cultures::culture_tag()))>;
		boost::container::small_vector<acc_pair, 16, concurrent_allocator<acc_pair>> accumulators;

		for(int32_t i = 0; i < cultures_count; ++i)
			accumulators.emplace_back(make_movement_acc(ws, group_cultures.first[i]), make_rebel_acc(ws, group_cultures.first[i]));

		for(auto p : cores) {
			if(ws.w.province_s.province_state_container.get<province_state::owner>(p) != holder) {
				for(int32_t i = 0; i < cultures_count; ++i) {
					if(t == cultures::dominant_tag_for_culture(ws, p, group_cultures.first[i])) {
						provinces::for_each_pop(ws, p, [&ws, &accumulators, i](population::pop_tag po) {
							accumulators[i].first.add_value(po);
							accumulators[i].second.add_value(po);
						});
					}
				}
			}
		}

		float const total_movement_support = std::transform_reduce(std::begin(accumulators), std::end(accumulators), 0.0f, std::plus<>(), [](auto& v) { return v.first.flush(); });
		float const total_rebel_support = std::transform_reduce(std::begin(accumulators), std::end(accumulators), 0.0f, std::plus<>(), [](auto& v) { return v.second.flush(); });

		ws.w.population_s.independence_rebel_support[t] = old_rebel_support < total_rebel_support ? (old_rebel_support + total_rebel_support) / 2.0f : total_rebel_support;
		ws.w.population_s.independence_movement_support[t] = old_movement_support < total_movement_support ? (old_movement_support + total_movement_support) / 2.0f : total_movement_support;
	}

	void update_local_movements_and_rebels(world_state& ws, nations::country_tag n) {
		float* local_movements = static_cast<float*>(ve_aligned_alloca(sizeof(float) * ws.s.issues_m.tracked_options_count));
		std::fill_n(local_movements, ws.s.issues_m.tracked_options_count, 0.0f);

		float* local_rebels = static_cast<float*>(ve_aligned_alloca(sizeof(float) * ws.s.population_m.rebel_types.size()));
		std::fill_n(local_rebels, ws.s.population_m.rebel_types.size(), 0.0f);

		nations::for_each_pop(ws, n, [&ws, local_movements, local_rebels](pop_tag p) {
			const static float con_limit = ws.s.modifiers_m.global_defines.issue_movement_leave_limit / 10.0f;
			const static float inv_con_limit = 1.0f / (1.0f - ws.s.modifiers_m.global_defines.issue_movement_leave_limit / 10.0f);
			const static float mil_limit = ws.s.modifiers_m.global_defines.mil_to_join_rebel / 10.0f;
			const static float inv_mil_limit = 1.0f / (1.0f - ws.s.modifiers_m.global_defines.mil_to_join_rebel / 10.0f);

			if(ws.w.population_s.pops.get<pop::is_accepted>(p)) {
				auto const pop_con = ws.w.population_s.pops.get<pop::consciousness>(p);
				auto const pop_mil = ws.w.population_s.pops.get<pop::militancy>(p);
				auto const pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_con >= con_limit) {
					ve::accumulate_scaled(ws.s.issues_m.tracked_options_count,
						tagged_array_view<float, int32_t>(local_movements, ws.s.issues_m.tracked_options_count),
						tagged_array_view<float, int32_t>(ws.w.population_s.pop_demographics.get_row(p).data() + to_index(to_demo_tag(ws, issues::option_tag(0))), ws.s.issues_m.tracked_options_count),
						((pop_con - con_limit) * inv_con_limit) * ws.w.population_s.pops.get<pop::literacy>(p) * (max_movement_support * pop_size),
						ve::serial_unaligned());
				}
				if(pop_mil >= mil_limit) {
					rebel_type_tag best_fit;
					float best_support = 0.0f;
					for(auto const& rt : ws.s.population_m.rebel_types) {
						if(rt.id != ws.s.population_m.nationalist_rebels) {
							auto const chance = modifiers::test_multiplicative_factor(rt.spawn_chance, ws, p, p);
							if(chance > best_support) {
								best_support = chance;
								best_fit = rt.id;
							}
						}
					}
					if(best_fit) {
						auto const reb_support = ((pop_mil - mil_limit) * inv_mil_limit) * (max_rebel_support * pop_size);
						local_rebels[to_index(best_fit)] += reb_support;
					}
				}
			}
		});

		auto const local_m_support = ws.w.nation_s.local_movement_support.get_row(n);
		for(int32_t i = ws.s.issues_m.party_issues_options_count; i < int32_t(ws.s.issues_m.tracked_options_count); ++i) {
			if(local_m_support[issues::option_tag(issues::option_tag::value_base_t(i))] < local_movements[i]) {
				local_m_support[issues::option_tag(issues::option_tag::value_base_t(i))] =
					(local_m_support[issues::option_tag(issues::option_tag::value_base_t(i))] + local_movements[i]) / 2.0f;
			} else {
				local_m_support[issues::option_tag(issues::option_tag::value_base_t(i))] = local_movements[i];
			}
		}

		auto const local_r_support = ws.w.nation_s.local_rebel_support.get_row(n);
		for(int32_t i = 0; i < ws.s.population_m.rebel_types.size(); ++i) {
			if(local_r_support[rebel_type_tag(rebel_type_tag::value_base_t(i))] < local_rebels[i]) {
				local_r_support[rebel_type_tag(rebel_type_tag::value_base_t(i))] =
					(local_r_support[rebel_type_tag(rebel_type_tag::value_base_t(i))] + local_rebels[i]) / 2.0f;
			} else {
				local_r_support[rebel_type_tag(rebel_type_tag::value_base_t(i))] = local_rebels[i];
			}
		}
	}

	constexpr uint32_t independence_movement_update_frequency = 16;

	void update_independence_movements(world_state& ws) {
		const uint32_t tag_count = uint32_t(ws.s.culture_m.national_tags.size());
		auto const chunk_size = (tag_count + independence_movement_update_frequency - 1ui32) / independence_movement_update_frequency;
		uint32_t const chunk_index = uint32_t(to_index(ws.w.current_date) & (independence_movement_update_frequency - 1));

		concurrency::parallel_for(chunk_size * chunk_index, std::min(chunk_size * (chunk_index + 1), tag_count), [&ws](uint32_t i) {
			cultures::national_tag const t = cultures::national_tag(cultures::national_tag::value_base_t(i));
			if(ws.s.culture_m.tags_to_groups[t]) {
				update_union_independence_movement_and_rebels(ws, t);
			} else {
				update_non_union_independence_movement_and_rebels(ws, t);
			}
		}, concurrency::static_partitioner());
	}

	constexpr uint32_t rebel_update_frequency = 16;

	void update_local_rebels_and_movements(world_state& ws) {
		const uint32_t nation_count = uint32_t(ws.w.nation_s.nations.size());
		auto const chunk_size = (nation_count + rebel_update_frequency - 1ui32) / rebel_update_frequency;
		uint32_t const chunk_index = uint32_t(to_index(ws.w.current_date) & (rebel_update_frequency - 1));

		concurrency::parallel_for(chunk_size * chunk_index, std::min(chunk_size * (chunk_index + 1), nation_count), [&ws](uint32_t i) {
			nations::country_tag t = nations::country_tag(nations::country_tag::value_base_t(i));
			if(ws.w.nation_s.nations.get<nation::current_capital>(t)) {
				update_local_movements_and_rebels(ws, t);
			}
		}, concurrency::static_partitioner());
	}

	pop_tag find_in_province(world_state const& ws, provinces::province_tag prov, pop_type_tag type, cultures::culture_tag c, cultures::religion_tag r) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(prov));
		for(auto const p : pop_range) {
			if(ws.w.population_s.pops.get<pop::type>(p) == type && ws.w.population_s.pops.get<pop::culture>(p) == c &&
				ws.w.population_s.pops.get<pop::religion>(p) == r)
				return p;
		}
		return pop_tag();
	}

	population_range population_amount_range(world_state const& ws) {
		float min_v = std::numeric_limits<float>::max();
		float max_v = 0;

		ws.s.province_m.for_each_land_province([&min_v, &max_v, &ws](provinces::province_tag p) {
			auto amount = ws.w.province_s.province_state_container.get<province_state::total_population>(p);
			if(amount > 0) {
				min_v = std::min(min_v, amount);
				max_v = std::max(max_v, amount);
			}
		});

		return population_range{min_v, max_v};
	}
	population_range population_density_range(world_state const& ws) {
		float min_v = std::numeric_limits<float>::max();
		float max_v = 0;

		ws.s.province_m.for_each_land_province([&min_v, &max_v, &ws](provinces::province_tag p) {
			auto amount = ws.w.province_s.province_state_container.get<province_state::total_population>(p);
			auto area = ws.s.province_m.province_container.get<province::area>(p);
			if(amount > 0 && area > 0) {
				min_v = std::min(min_v, amount/area);
				max_v = std::max(max_v, amount/area);
			}
		});

		return population_range{ min_v, max_v };
	}

	population_range internal_migration_range(world_state const& ws) {
		float min_v = 0;
		float max_v = 0;

		ws.s.province_m.for_each_land_province([&min_v, &max_v, &ws](provinces::province_tag p) {
			auto amount = ws.w.province_s.province_state_container.get<province_state::old_migration_growth>(p);
			
			min_v = std::min(min_v, amount);
			max_v = std::max(max_v, amount);
			
		});

		return population_range{ min_v, max_v };
	}
	population_range external_migration_range(world_state const& ws) {
		float min_v = 0;
		float max_v = 0;

		ws.s.province_m.for_each_land_province([&min_v, &max_v, &ws](provinces::province_tag p) {
			auto amount = ws.w.province_s.province_state_container.get<province_state::old_immigration_growth>(p);

			min_v = std::min(min_v, amount);
			max_v = std::max(max_v, amount);

		});

		return population_range{ min_v, max_v };
	}

	struct pop_migration_key {
		cultures::culture_tag c;
		//cultures::religion_tag r;
		pop_type_tag t;

		constexpr bool operator==(pop_migration_key const& o) const {
			return c == o.c /*&& r == o.r*/ && t == o.t;
		}
	};

	struct emigrating_pop_data {
		float amount;
		float pop_militancy;
		float pop_consciousness;
		float pop_literacy;
		cultures::religion_tag pop_religion;
	};
	struct migrating_pop_data {
		float amount;
		float pop_militancy;
		float pop_consciousness;
		float pop_literacy;
		nations::country_tag within_nation;
		cultures::religion_tag pop_religion;
	};

	struct pop_migration_data {
		moveable_concurrent_cache_aligned_buffer<float, nations::country_tag, true> nation_weights;
		moveable_concurrent_cache_aligned_buffer<float, provinces::province_tag, true> province_weights;
		moveable_concurrent_cache_aligned_buffer<float, nations::country_tag, true> sum_province_weights_by_nation;
		std::vector<emigrating_pop_data, concurrent_allocator<emigrating_pop_data>> emigrating_pops;
		std::vector<migrating_pop_data, concurrent_allocator<migrating_pop_data>> migrating_pops;

		float nation_weights_sum = 0.0f;
		pop_tag from_pop;

		pop_migration_data(world_state const& ws, pop_type_tag p_type, pop_tag origin_pop) : nation_weights(ws.w.nation_s.nations.vector_size()),
			province_weights(ws.w.province_s.province_state_container.vector_size()),
			sum_province_weights_by_nation(ws.w.nation_s.nations.vector_size()), from_pop(origin_pop) {
		}

		void populate_weights(world_state const& ws, pop_type_tag p_type, cultures::culture_tag p_culture) {
			if(emigrating_pops.size() != 0) {
				auto const nation_count = ws.w.nation_s.nations.size();

				auto const mt_trigger = ws.s.population_m.pop_types[p_type].country_migration_target;
				for(int32_t i = 0; i < nation_count; i += ve::vector_size) {
					ve::contiguous_tags<nations::country_tag> off(i);
					auto civ_status = ve::load(off, ws.w.nation_s.nations.get_row<nation::is_civilized>());
					auto const nat_mod = ve::load(off, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_immigrant_attract>(nation_count));
					auto const modifier_result = modifiers::test_semi_contiguous_multiplicative_factor(mt_trigger, ws, off, from_pop, from_pop);
					ve::store(
						off,
						nation_weights.view(int32_t(nation_count)),
						ve::max(ve::select(civ_status, ve::multiply_and_add(nat_mod, modifier_result, modifier_result), 0.0f), 0.0f));
				}
				float const avg = std::reduce(
					                  nation_weights.begin(),
					                  nation_weights.begin() + nation_count,
									  -0.000001f,
					                  std::plus<>()) / float(nation_count) ;
				
				float temp_sum = 0.0f;

				for(int32_t i = 0; i < nation_count; ++i) {
					temp_sum += std::max(0.0f, nation_weights[nations::country_tag(nations::country_tag::value_base_t(i))] - avg);
					nation_weights[nations::country_tag(nations::country_tag::value_base_t(i))] = temp_sum;
				}
				nation_weights_sum = temp_sum;
			}

			auto const mt_trigger = ws.s.population_m.pop_types[p_type].migration_target;
			auto const vs = ws.s.province_m.first_sea_province;
			for(int32_t i = 0; i < int32_t(vs); i += ve::vector_size) {
				ve::contiguous_tags<provinces::province_tag> off(i);
				auto const prov_mod = ve::load(off, ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::immigrant_attract>(vs));
				auto const modifier_result = modifiers::test_semi_contiguous_multiplicative_factor(mt_trigger, ws, off, from_pop, from_pop);
				ve::store(
					off,
					province_weights.view(int32_t(vs)),
					ve::max(ve::multiply_and_add(prov_mod, modifier_result, modifier_result), 0.0f));
			}
		}

		nations::country_tag choose_nation(world_state const& ws) {
			int32_t const nation_count = ws.w.nation_s.nations.size();
			std::uniform_real_distribution<float> dist(0.0f, nation_weights_sum);
			auto const chosen = dist(get_local_generator());

			auto const chosen_position = std::lower_bound(nation_weights.begin(), nation_weights.begin() + nation_count, chosen);
			return (chosen_position != nation_weights.begin() + nation_count)
				       ? nations::country_tag(nations::country_tag::value_base_t(chosen_position - nation_weights.begin()))
				       : nations::country_tag();
		}

		provinces::province_tag choose_province_from_nation(world_state const& ws, nations::country_tag n) {
			auto p_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));

			if(sum_province_weights_by_nation[n] == 0.0f) {
				float const avg = std::transform_reduce(
					p_range.first,
					p_range.second,
					-0.000001f,
					std::plus<>(),
					[_this = this](provinces::province_tag p) { return _this->province_weights[p]; }) / float(p_range.second - p_range.first);
				
				float temp_sum = 0.0f;
				for(auto p : p_range) {
					temp_sum += std::max(0.0f, province_weights[p] - avg);
					province_weights[p] = temp_sum;
				}
				sum_province_weights_by_nation[n] = temp_sum;
			}

			auto const total_weights = sum_province_weights_by_nation[n];
			std::uniform_real_distribution<float> dist(0.0f, total_weights);
			auto chosen = dist(get_local_generator());

			auto const chosen_position = std::lower_bound(
				p_range.first,
				p_range.second,
				chosen,
				[_this = this](provinces::province_tag a, float val) { return _this->province_weights[a] < val; });

			return (chosen_position != p_range.second)
				? *chosen_position
				: provinces::province_tag();
		}
	};
}

namespace std {
	template<>
	struct hash<population::pop_migration_key> {
		size_t operator()(population::pop_migration_key k) const noexcept {
			// from: https://github.com/martinus/robin-hood-hashing/blob/master/src/include/robin_hood.h
			//uint32_t const v = (uint32_t(k.c.value) << 8) ^ (uint32_t(k.t.value) << 3) ^ uint32_t(k.r.value);
			uint32_t const v = (uint32_t(k.c.value) << 5) ^ uint32_t(k.t.value);

			__int64 const high_out = __mulh(0xfa1371431ef43ae1ui64, v);
			return static_cast<uint64_t>(high_out) * 0xfe9b65e7da1b3187ui64;
		}
	};
}

namespace population {
	void add_pop_to_array(float* array, pop_type_tag type, float size, float literacy) {
		array[to_index(type) * 2] += size;
		array[to_index(type) * 2 +  1] += size * literacy;
	}

	std::unique_ptr<float[]> make_new_pop_array(world_state const& ws) {
		std::unique_ptr<float[]> r(new float[ws.s.population_m.count_poptypes * 2]);
		std::fill_n(r.get(), ws.s.population_m.count_poptypes * 2, 0.0f);
		return r;
	}

	namespace {
		void handle_pop_promotion_and_demotion(
			world_state& ws,
			provinces::province_tag t,
			pop_type_tag focused_type,
			pop_tag pop_j,
			cultures::culture_tag pop_j_culture,
			cultures::religion_tag pop_j_religion,
			pop_type_tag const pop_j_type,
			float pop_j_militancy,
			float pop_j_consciousness,
			float pop_j_literacy,
			float promotion_val) {

			float* const chances = static_cast<float*>(_alloca(sizeof(float) * (ws.s.population_m.count_poptypes + 1)));
			std::fill_n(chances, ws.s.population_m.count_poptypes + 1, 0.0f);
			float total_chances = 0.0f;

			if(promotion_val > 0.0f) {
				// promote
				for(int32_t k = 0; k < int32_t(ws.s.population_m.count_poptypes); ++k) {
					pop_type_tag pt = pop_type_tag(pop_type_tag::value_base_t(k));
					auto const trigger = ws.s.population_m.promote_to.get(pop_j_type, pt);
					float const chance = trigger ? std::max(
						0.0f,
						modifiers::test_additive_factor(
							trigger,
							ws,
							pop_j,
							pop_j)) : 0.0f;
					total_chances += chance;
					chances[k + 1] = total_chances;
				}
			} else {
				// demote
				for(int32_t k = 0; k < int32_t(ws.s.population_m.count_poptypes); ++k) {
					pop_type_tag pt = pop_type_tag(pop_type_tag::value_base_t(k));
					auto const trigger = ws.s.population_m.demote_to.get(pop_j_type, pt);
					float const chance = trigger ? std::max(
						0.0f,
						modifiers::test_additive_factor(
							trigger,
							ws,
							pop_j,
							pop_j)) : 0.0f;
					total_chances += chance;
					chances[k + 1] = total_chances;
				}
			}

			pop_type_tag const result_target_base = [&ws, t, focused_type, chances, total_chances]() {
				if(total_chances == 0) {
					return pop_type_tag();
				} else if(focused_type && (chances[to_index(focused_type) + 1] - chances[to_index(focused_type)]) > 0) {
					return focused_type;
				} else {
					std::uniform_real_distribution<float> dist(0.0f, total_chances);
					auto const chosen = dist(get_local_generator());

					auto const lb = std::lower_bound(chances + 1, chances + 1 + ws.s.population_m.count_poptypes, chosen);
					if(lb != chances + 1 + ws.s.population_m.count_poptypes)
						return pop_type_tag(pop_type_tag::value_base_t(lb - (chances + 1)));
					else
						return pop_type_tag(pop_type_tag::value_base_t(ws.s.population_m.count_poptypes - 1));
				}
			}();

			pop_type_tag const result_target = (result_target_base == ws.s.population_m.farmer) ? economy::correct_worker_type(ws, t) : result_target_base;

			if(result_target) {
				auto const target_pop = find_in_province(ws, t, result_target, pop_j_culture, pop_j_religion);
				if(target_pop) {
					ws.w.population_s.pops.get<pop::size_change_from_growth>(target_pop) += std::abs(promotion_val);
				} else {
					concurrent_make_new_pop(
						ws,
						std::abs(promotion_val),
						pop_j_militancy,
						pop_j_consciousness,
						pop_j_literacy,
						t,
						result_target,
						pop_j_culture,
						pop_j_religion);
				}
			}
		}

		void handle_pop_assimilation(
			world_state& ws,
			provinces::province_tag t,
			cultures::culture_tag owner_culture,
			cultures::religion_tag owner_religion,
			pop_tag pop_j,
			cultures::culture_tag pop_j_culture,
			cultures::religion_tag pop_j_religion,
			pop_type_tag pop_j_type,
			float const pop_j_militancy,
			float const pop_j_consciousness,
			float const pop_j_literacy,
			float const assimilation_val) {

			if(ws.w.population_s.pops.get<pop::is_accepted>(pop_j)) {
				if(pop_j_religion != owner_religion) {
					auto const target_pop = find_in_province(ws, t, pop_j_type, pop_j_culture, owner_religion);
					if(target_pop) {
						ws.w.population_s.pops.get<pop::size_change_from_growth>(target_pop) += assimilation_val;
					} else {
						concurrent_make_new_pop(
							ws,
							assimilation_val,
							pop_j_militancy,
							pop_j_consciousness,
							pop_j_literacy,
							t,
							pop_j_type,
							pop_j_culture,
							owner_religion);
					}
				}
			} else {
				auto const target_pop = find_in_province(ws, t, pop_j_type, owner_culture, pop_j_religion);
				if(target_pop) {
					ws.w.population_s.pops.get<pop::size_change_from_growth>(target_pop) += assimilation_val;
				} else {
					concurrent_make_new_pop(
						ws,
						assimilation_val,
						pop_j_militancy,
						pop_j_consciousness,
						pop_j_literacy,
						t,
						pop_j_type,
						owner_culture,
						pop_j_religion);
				}
			}
		}
	}

	void execute_size_changes(world_state& ws) {

		uint32_t const provinces_count = uint32_t(ws.s.province_m.first_sea_province);

		uint32_t const chunk_size = (provinces_count / (pop_update_frequency * pop_update_group_size));
		uint32_t const chunk_index = uint32_t(to_index(ws.w.current_date) & (pop_update_frequency - 1));

		int32_t const lower_limit = int32_t(chunk_size * chunk_index * pop_update_group_size);
		int32_t const upper_limit = int32_t(chunk_index != pop_update_frequency - 1 ? chunk_size * pop_update_group_size * (chunk_index + 1) : provinces_count);

		concurrency::parallel_for(lower_limit, upper_limit, [&ws](uint32_t i) {
			provinces::province_tag t = provinces::province_tag(provinces::province_tag::value_base_t(i));
			provinces::for_each_pop(ws, t, [&ws](population::pop_tag p) {
				ws.w.population_s.pops.set<pop::size_change_from_growth>(p, 0.0f);
			});
		}, concurrency::static_partitioner());

		std::unordered_map<pop_migration_key, pop_migration_data> migration_map;

		for(int32_t i = lower_limit; i < upper_limit; i += ve::vector_size) {
			ve::contiguous_tags<provinces::province_tag> off(i);
			auto migration = ws.w.province_s.province_state_container.get_row<province_state::net_migration_growth>();
			ve::store(off, ws.w.province_s.province_state_container.get_row<province_state::old_migration_growth>(), ve::load(off, migration));
			ve::store(off, migration, 0.0f);

			auto immigration = ws.w.province_s.province_state_container.get_row<province_state::net_immigration_growth>();
			ve::store(off, ws.w.province_s.province_state_container.get_row<province_state::old_immigration_growth>(), ve::load(off, immigration));
			ve::store(off, immigration, 0.0f);

			auto monthly_pop = ws.w.province_s.province_state_container.get_row<province_state::monthly_population>();
			ve::store(off, ws.w.province_s.province_state_container.get_row<province_state::old_monthly_population>(), ve::load(off, monthly_pop));
			ve::store(off, monthly_pop, 0.0f);
		}

		concurrency::parallel_for(lower_limit, upper_limit, [&ws](uint32_t i) {
			provinces::province_tag t = provinces::province_tag(provinces::province_tag::value_base_t(i));
			auto const province_owner = ws.w.province_s.province_state_container.get<province_state::owner>(t);
			auto const owner_culture = ws.w.nation_s.nations.get<nation::primary_culture>(province_owner);
			auto const owner_religion = ws.w.nation_s.nations.get<nation::national_religion>(province_owner);

			auto const state = ws.w.province_s.province_state_container.get<province_state::state_instance>(t);
			auto const state_focus = ws.w.nation_s.states.get<state::owner_national_focus>(state);
			auto const focused_type = ws.s.modifiers_m.focus_to_pop_types[state_focus];

			auto const pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(t));

			int32_t const fixed_pop_count = int32_t(pop_range.second - pop_range.first);
			population::pop_tag* const fixed_range = static_cast<population::pop_tag*>(_alloca(sizeof(population::pop_tag) * fixed_pop_count));
			std::copy_n(pop_range.first, fixed_pop_count, fixed_range);

			float const province_growth = std::max(0.0f,
				(provinces::get_life_rating(ws, t) - ws.s.modifiers_m.global_defines.min_life_rating_for_growth)
				* ws.s.modifiers_m.global_defines.life_rating_growth_bonus)
				+ ws.s.modifiers_m.global_defines.base_popgrowth;

			float const owner_growth_mod = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::global_population_growth>(province_owner);

			// first pass : zero growth
			for(int32_t j = 0; j < fixed_pop_count; ++j) {
				ws.w.population_s.pops.set<pop::size_change_from_growth>(fixed_range[j], 0.0f);
			}

			// second pass : calculate growth
			for(int32_t j = 0; j < fixed_pop_count; ++j) {
				auto const pop_j = fixed_range[j];
				auto const pop_j_type = ws.w.population_s.pops.get<pop::type>(pop_j);
				auto const pop_j_size = ws.w.population_s.pops.get<pop::size>(pop_j);
				auto const pop_j_satisfaction = ws.w.population_s.pops.get<pop::needs_satisfaction>(pop_j);

				if(pop_j_type != ws.s.population_m.slave) {
					auto const pop_j_culture = ws.w.population_s.pops.get<pop::culture>(pop_j);
					auto const pop_j_religion = ws.w.population_s.pops.get<pop::religion>(pop_j);
					auto const pop_j_militancy = ws.w.population_s.pops.get<pop::militancy>(pop_j);
					auto const pop_j_consciousness = ws.w.population_s.pops.get<pop::consciousness>(pop_j);
					auto const pop_j_literacy = ws.w.population_s.pops.get<pop::literacy>(pop_j);

					auto const promotion_val = ws.w.population_s.pops.get<pop::size_change_from_type_change_away>(pop_j);
					if(abs(promotion_val) >= 1.0f) {
						handle_pop_promotion_and_demotion(
							ws,
							t,
							focused_type,
							pop_j,
							pop_j_culture,
							pop_j_religion,
							pop_j_type,
							pop_j_militancy,
							pop_j_consciousness,
							pop_j_literacy,
							promotion_val);
					}

					auto const assimilation_val = std::ceil(ws.w.population_s.pops.get<pop::size_change_from_assimilation_away>(pop_j));
					if(assimilation_val >= 1.0f) {
						// change culture or religion
						handle_pop_assimilation(
							ws,
							t,
							owner_culture,
							owner_religion,
							pop_j,
							pop_j_culture,
							pop_j_religion,
							pop_j_type,
							pop_j_militancy,
							pop_j_consciousness,
							pop_j_literacy,
							assimilation_val);
					}

					//generic growth
					auto const modified_prov_growth = (pop_j_satisfaction < ws.s.modifiers_m.global_defines.life_need_starvation_limit)
						? (province_growth * (1.0f + 4.0f * (pop_j_satisfaction - ws.s.modifiers_m.global_defines.life_need_starvation_limit)))
						: province_growth;
					auto const growth_amount = (owner_growth_mod + modified_prov_growth) * pop_j_size;
					ws.w.population_s.pops.get<pop::size_change_from_growth>(pop_j) += growth_amount;

				} else {
					// slave case
					auto const modified_prov_growth = (pop_j_satisfaction < ws.s.modifiers_m.global_defines.life_need_starvation_limit)
						? (province_growth * (1.0f + 4.0f * (pop_j_satisfaction - ws.s.modifiers_m.global_defines.life_need_starvation_limit)))
						: province_growth;
					auto const growth_amount = (owner_growth_mod + modified_prov_growth) * pop_j_size / ws.s.modifiers_m.global_defines.slave_growth_divisor;
					ws.w.population_s.pops.get<pop::size_change_from_growth>(pop_j) += growth_amount;
				}
			}
		});

		// collect moving pops
		for(int32_t i = lower_limit; i < upper_limit; ++i) {
			provinces::province_tag t = provinces::province_tag(provinces::province_tag::value_base_t(i));
			auto const province_owner = ws.w.province_s.province_state_container.get<province_state::owner>(t);

			auto const pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(t));

			int32_t const fixed_pop_count = int32_t(pop_range.second - pop_range.first);
			population::pop_tag* const fixed_range = static_cast<population::pop_tag*>(_alloca(sizeof(population::pop_tag) * fixed_pop_count));
			std::copy_n(pop_range.first, fixed_pop_count, fixed_range);
			
			for(int32_t j = 0; j < fixed_pop_count; ++j) {
				auto const pop_j = fixed_range[j];
				auto const pop_j_type = ws.w.population_s.pops.get<pop::type>(pop_j);

				if(pop_j_type != ws.s.population_m.slave) {
					auto const pop_j_culture = ws.w.population_s.pops.get<pop::culture>(pop_j);
					auto const pop_j_religion = ws.w.population_s.pops.get<pop::religion>(pop_j);


					auto const pop_j_militancy = ws.w.population_s.pops.get<pop::militancy>(pop_j);
					auto const pop_j_consciousness = ws.w.population_s.pops.get<pop::consciousness>(pop_j);
					auto const pop_j_literacy = ws.w.population_s.pops.get<pop::literacy>(pop_j);

					auto const migration_val = ws.w.population_s.pops.get<pop::size_change_from_local_migration>(pop_j);
					if(migration_val >= 1.0f) {
						// move within nation

						//auto const emplace_result = migration_map.try_emplace(pop_migration_key{ pop_j_culture, pop_j_religion, pop_j_type }, ws, pop_j_type, pop_j);
						auto const emplace_result = migration_map.try_emplace(pop_migration_key{ pop_j_culture, pop_j_type }, ws, pop_j_type, pop_j);
						emplace_result.first->second.migrating_pops.push_back(
							migrating_pop_data{
								migration_val,
								pop_j_militancy,
								pop_j_consciousness,
								pop_j_literacy,
								province_owner,
								pop_j_religion
							});
						ws.w.province_s.province_state_container.get<province_state::net_migration_growth>(t) -= migration_val;
					}

					auto const emigration_val = ws.w.population_s.pops.get<pop::size_change_from_emigration>(pop_j);
					if(emigration_val >= 1.0f) {
						// move outside nation

						//auto const emplace_result = migration_map.try_emplace(pop_migration_key{ pop_j_culture, pop_j_religion, pop_j_type }, ws, pop_j_type, pop_j);
						auto const emplace_result = migration_map.try_emplace(pop_migration_key{ pop_j_culture, pop_j_type }, ws, pop_j_type, pop_j);
						emplace_result.first->second.emigrating_pops.push_back(
							emigrating_pop_data{
								emigration_val,
								pop_j_militancy,
								pop_j_consciousness,
								pop_j_literacy,
								pop_j_religion
							});
						ws.w.province_s.province_state_container.get<province_state::net_immigration_growth>(t) -= emigration_val;
					}
				}
			}
		}

		// resolve migrating pops
		concurrency::parallel_for_each(migration_map.begin(), migration_map.end(), [&ws](auto& map_pair) {
			map_pair.second.populate_weights(ws, map_pair.first.t, map_pair.first.c);
		});

		static concurrency::task_group pop_creation_tg;

		for(auto& map_pair : migration_map) {
			for(auto& em_pop : map_pair.second.emigrating_pops) {
				auto const dest_nation = map_pair.second.choose_nation(ws);
				if(dest_nation) {
					auto const dest_province = map_pair.second.choose_province_from_nation(ws, dest_nation);
					auto const dest_nation_religion = ws.w.nation_s.nations.get<nation::national_religion>(dest_nation);

					if(dest_province) {
						ws.w.province_s.province_state_container.get<province_state::net_immigration_growth>(dest_province) += em_pop.amount;

						auto const fitted_type = fit_type_to_province(ws, dest_province, map_pair.first.t);
						auto const target_pop = find_in_province(ws, dest_province, fitted_type, map_pair.first.c, em_pop.pop_religion);
						if(target_pop) {
							grow_pop_immediate(ws, target_pop, em_pop.amount);
						} else if(auto const immigrant_target_pop = find_in_province(ws, dest_province, fitted_type, ws.s.culture_m.immigrant_culture, dest_nation_religion); immigrant_target_pop) {
							grow_pop_immediate(ws, immigrant_target_pop, em_pop.amount);
						} else {
							make_new_pop_deferred(
								ws,
								em_pop.amount,
								em_pop.pop_militancy,
								em_pop.pop_consciousness,
								em_pop.pop_literacy,
								dest_province,
								fitted_type,
								ws.s.culture_m.immigrant_culture,
								dest_nation_religion,
								pop_creation_tg);
						}
					}
				}
			}
			for(auto& mi_pop : map_pair.second.migrating_pops) {
				auto const dest_province = map_pair.second.choose_province_from_nation(ws, mi_pop.within_nation);
				auto const dest_nation_religion = ws.w.nation_s.nations.get<nation::national_religion>(mi_pop.within_nation);

				if(dest_province) {
					
					ws.w.province_s.province_state_container.get<province_state::net_migration_growth>(dest_province) += mi_pop.amount;

					auto const fitted_type = fit_type_to_province(ws, dest_province, map_pair.first.t);
					auto const target_pop = find_in_province(ws, dest_province, fitted_type, map_pair.first.c, mi_pop.pop_religion);
					if(target_pop) {
						grow_pop_immediate(ws, target_pop, mi_pop.amount);
					} else if(auto const immigrant_target_pop = find_in_province(ws, dest_province, fitted_type, ws.s.culture_m.immigrant_culture, dest_nation_religion); immigrant_target_pop) {
						grow_pop_immediate(ws, immigrant_target_pop, mi_pop.amount);
					} else {
						make_new_pop_deferred(
							ws,
							mi_pop.amount,
							mi_pop.pop_militancy,
							mi_pop.pop_consciousness,
							mi_pop.pop_literacy,
							dest_province,
							fitted_type,
							ws.s.culture_m.immigrant_culture,
							dest_nation_religion,
							pop_creation_tg);
					}
				}
			}
		}

		pop_creation_tg.wait();

		concurrency::parallel_for(lower_limit, upper_limit, [&ws](uint32_t i) {
			static std::mutex release_lock;

			provinces::province_tag t = provinces::province_tag(provinces::province_tag::value_base_t(i));
			
			// third pass: apply growth, remove dead pops -- new pops have size >= 1.0f, thus do not need to be checked and cannot have grown
			auto& pops_array_ref = ws.w.province_s.province_state_container.get<province_state::pops>(t);
			
			auto const pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(t));
			int32_t const fixed_pop_count = int32_t(pop_range.second - pop_range.first);
			population::pop_tag* const fixed_range = static_cast<population::pop_tag*>(_alloca(sizeof(population::pop_tag) * fixed_pop_count));
			std::copy_n(pop_range.first, fixed_pop_count, fixed_range);

			for(int32_t j = 0; j < fixed_pop_count; ++j) {
				auto const pop_j = fixed_range[j];
				auto const change = total_size_change(ws, pop_j);

				grow_pop_immediate(ws, pop_j, change);

				if(ws.w.population_s.pops.get<pop::size>(pop_j) < 1.0f) {
					remove_item(ws.w.population_s.pop_arrays, pops_array_ref, pop_j);
					{
						std::lock_guard guard(release_lock);
						ws.w.population_s.pops.release(pop_j);
					}
				}
			}

			//update monthly pop totals
			auto const updated_range = get_range(ws.w.population_s.pop_arrays, pops_array_ref);
			float const total = std::transform_reduce(
				updated_range.first,
				updated_range.second,
				0.0f,
				std::plus<>(),
				[&ws](pop_tag p) {
					auto const sz = ws.w.population_s.pops.get<pop::size>(p);
#ifdef _DEBUG
					auto const total_sz = ws.w.population_s.pop_demographics.get(p, total_population_tag);
					assert(sz >= 1.0f);
					assert(sz == total_sz);
#endif
					return sz;
			});
			ws.w.province_s.province_state_container.set<province_state::monthly_population>(t, total);
		});
	}
}
