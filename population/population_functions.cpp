#include "common\\common.h"
#include "population_function.h"
#include "world_state\\world_state.h"
#include "modifiers\\modifier_functions.h"
#include <ppl.h>

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
		if((ws.s.population_m.pop_types[ws.w.population_s.pops.get<pop::type>(p)].flags & population::pop_type::is_employable) == 0)
			ws.w.population_s.pop_demographics.get(p, total_employment_tag) = size;
	}

	bool is_pop_accepted(world_state const& ws, pop_tag p, nations::nation const& n) {
		auto p_culture = ws.w.population_s.pops.get<pop::culture>(p);
		return p_culture == n.primary_culture || contains_item(ws.w.culture_s.culture_arrays, n.accepted_cultures, p_culture);
	}

	pop_tag get_unassigned_soldier_in_province(world_state const& ws, provinces::province_tag prov) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(prov));
		for(auto i = pop_range.first; i != pop_range.second; ++i) {
			if(!is_valid_index(ws.w.population_s.pops.get<pop::associated_army>(*i)) & (ws.w.population_s.pops.get<pop::type>(*i) == ws.s.population_m.soldier))
				return *i;
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

	nations::nation* get_pop_owner(world_state const& ws, pop_tag p) {
		auto loc = ws.w.population_s.pops.get<pop::location>(p);
		if(is_valid_index(loc)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(loc);
			if(is_valid_index(owner))
				return &ws.w.nation_s.nations[owner];
		}
		return nullptr;
	}

	bool is_dominant_issue(world_state const& ws, pop_tag id, issues::option_tag opt) {
		auto issue_offset =
			ws.w.population_s.pop_demographics.get_row(id) +
			to_index(population::to_demo_tag(ws, issues::option_tag(0)));
		Eigen::Map<Eigen::Matrix<float, -1, 1>> options_vector(issue_offset, ws.s.issues_m.tracked_options_count);
		return options_vector.maxCoeff() == issue_offset[to_index(opt)];
	}

	bool is_dominant_ideology(world_state const& ws, pop_tag id, ideologies::ideology_tag opt) {
		auto ideology_offset =
			ws.w.population_s.pop_demographics.get_row(id) +
			to_index(population::to_demo_tag(ws, ideologies::ideology_tag(0)));
		Eigen::Map<Eigen::Matrix<float, -1, 1>> ideology_vector(ideology_offset, ws.s.ideologies_m.ideologies_count);
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
	}

	void change_pop_size(world_state& ws, pop_tag this_pop, int32_t new_size) {
		// todo: fix employment
		ws.w.population_s.pop_demographics.get(this_pop, total_population_tag) = float(new_size);
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

	void trigger_rising(world_state&, rebel_faction&, nations::nation&) {
		// todo
	}

	void default_initialize_issues_and_ideology(world_state& ws, pop_tag this_pop) {
		auto ideology_demo = ws.w.population_s.pop_demographics.get_row(this_pop) + to_index(to_demo_tag(ws, ideologies::ideology_tag(0)));
		auto issues_demo = ws.w.population_s.pop_demographics.get_row(this_pop) + to_index(to_demo_tag(ws, issues::option_tag(0)));
		auto total_pop_size = ws.w.population_s.pop_demographics.get(this_pop, total_population_tag);
		auto pop_type = ws.w.population_s.pops.get<pop::type>(this_pop);

		for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
			ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
			if(ws.w.ideology_s.ideology_enabled[this_tag] != 0ui8) {
				auto pop_incl = ws.s.population_m.ideological_inclination.get(pop_type, this_tag);
				if(is_valid_index(pop_incl)) {
					if(ws.s.ideologies_m.ideology_container[this_tag].uncivilized) {
						ideology_demo[i] = 1000.0f * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
					} else if(auto owner = get_pop_owner(ws, this_pop); bool(owner) && ((owner->flags & nations::nation::is_civilized) != 0)) {
						ideology_demo[i] = 1000.0f * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
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
				issues_demo[i] = 1000.0f * modifiers::test_multiplicative_factor(pop_incl, ws, this_pop, nullptr, nullptr);
			else
				issues_demo[i] = 0;
		}
		Eigen::Map<Eigen::Matrix<float, 1, -1>> ovec(issues_demo, ws.s.issues_m.tracked_options_count);
		ovec *= total_pop_size / ovec.sum();
	}

	void default_initialize_world_issues_and_ideology(world_state& ws) {
		ws.w.population_s.pops.parallel_for_each([&ws](pop_tag pt) {
			default_initialize_issues_and_ideology(ws, pt);
		});
	}
}
