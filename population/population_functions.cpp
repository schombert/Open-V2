#include "common\\common.h"
#include "population_function.h"
#include "world_state\\world_state.h"
#undef min
#undef max

namespace population {
	void init_rebel_faction_from_rebel_type(rebel_faction& faction, rebel_type& type) {
		faction.type = type.id;
		faction.icon = type.icon;
		faction.flags = type.flags;
	}

	pop& make_new_pop(world_state& ws) {
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

	void init_pop_demographics(world_state& ws, pop& p, int32_t size) {
		ws.w.population_s.pop_demographics.ensure_capacity(to_index(p.id) + 1);
		ws.w.population_s.pop_demographics.get(p.id, total_population_tag) = size;
	}

	bool is_pop_accepted(world_state& ws, pop& p, nations::nation& n) {
		return p.culture == n.primary_culture || contains_item(ws.w.culture_s.culture_arrays, n.accepted_cultures, p.culture);
	}

	pop* get_unassigned_soldier_in_province(world_state& ws, provinces::province_tag prov) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[prov].pops);
		for(auto i = pop_range.first; i != pop_range.second; ++i) {
			pop& p = ws.w.population_s.pops.get(*i);
			if(!is_valid_index(p.associated_army) & (p.type == ws.s.population_m.soldier))
				return &p;
		}
		return nullptr;
	}
	void set_militancy_direct(pop &p, float v) {
		p.militancy = static_cast<uint16_t>(std::clamp(v, 0.0f, 10.0f) * float(std::numeric_limits<uint16_t>::max()) / 10.0f);
	}
	void set_literacy_direct(pop &p, float v) {
		p.literacy = static_cast<uint16_t>(std::clamp(v, 0.0f, 1.0f) * float(std::numeric_limits<uint16_t>::max()));
	}
	void set_consciousness_direct(pop & p, float v) {
		p.consciousness = static_cast<uint16_t>(std::clamp(v, 0.0f, 10.0f) * float(std::numeric_limits<uint16_t>::max()) / 10.0f);
	}

	float get_militancy_direct(pop const& p) {
		return float(p.militancy) * 10.0f / float(std::numeric_limits<uint16_t>::max());
	}
	float get_literacy_direct(pop const& p) {
		return float(p.literacy) / float(std::numeric_limits<uint16_t>::max());
	}
	float get_consciousness_direct(pop const& p) {
		return float(p.consciousness) * 10.0f / float(std::numeric_limits<uint16_t>::max());
	}

	economy::money_qnty_type desired_needs_spending(world_state&, pop const&) {
		return economy::money_qnty_type(0);
	}

	nations::nation* get_pop_owner(world_state const& ws, pop const& p) {
		auto loc = p.location;
		if(is_valid_index(loc))
			return ws.w.province_s.province_state_container[loc].owner;
		else
			return nullptr;
	}

	bool is_dominant_issue(world_state const& ws, pop_tag id, issues::option_tag opt) {
		auto issue_offset =
			ws.w.population_s.pop_demographics.get_row(id) +
			to_index(population::to_demo_tag(ws, issues::option_tag(0)));
		Eigen::Map<Eigen::Matrix<int32_t, -1, 1>> options_vector(issue_offset, ws.s.issues_m.tracked_options_count);
		return options_vector.maxCoeff() == issue_offset[to_index(opt)];
	}

	bool is_dominant_ideology(world_state const& ws, pop_tag id, ideologies::ideology_tag opt) {
		auto ideology_offset =
			ws.w.population_s.pop_demographics.get_row(id) +
			to_index(population::to_demo_tag(ws, ideologies::ideology_tag(0)));
		Eigen::Map<Eigen::Matrix<int32_t, -1, 1>> ideology_vector(ideology_offset, ws.s.ideologies_m.ideologies_count);
		return ideology_vector.maxCoeff() == ideology_offset[to_index(opt)];
	}

	void destroy_pop_movement(world_state& ws, pop_movement& m) {
		auto prange = get_range(ws.w.population_s.pop_arrays, m.member_pops);
		for(auto p : prange)
			ws.w.population_s.pops[p].movement = movement_tag();
		clear(ws.w.population_s.pop_arrays, m.member_pops);
	}

	void destroy_rebel_faction(world_state& ws, rebel_faction& r) {
		auto prange = get_range(ws.w.population_s.pop_arrays, r.member_pops);
		for(auto p : prange)
			ws.w.population_s.pops[p].rebel_faction = rebel_faction_tag();
		clear(ws.w.population_s.pop_arrays, r.member_pops);

		auto prov_range = get_range(ws.w.province_s.province_arrays, r.controlled_provinces);
		for(auto p : prov_range)
			ws.w.province_s.province_state_container[p].rebel_controller = nullptr;
		clear(ws.w.province_s.province_arrays, r.controlled_provinces);
	}

	void change_pop_type(world_state& ws, pop& this_pop, pop_type_tag new_type) {
		// todo: fix employment
		this_pop.type = new_type;
	}

	void change_pop_size(world_state& ws, pop& this_pop, int32_t new_size) {
		// todo: fix employment
		ws.w.population_s.pop_demographics.get(this_pop.id, total_population_tag) = new_size;
	}

	void remove_pop_from_province(world_state& ws, pop& this_pop) {
		// todo: fix employment
		if(is_valid_index(this_pop.location)) {
			remove_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[this_pop.location].pops, this_pop.id);
			this_pop.location = provinces::province_tag();
		}
	}

	void change_pop_location(world_state& ws, pop& this_pop, provinces::province_tag new_location) {
		// todo: fix employment
		remove_pop_from_province(ws, this_pop);
		this_pop.location = new_location;
		add_item(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[new_location].pops, this_pop.id);
	}

	void free_slave(world_state& ws, pop& this_pop) {
		auto location = this_pop.location;
		auto local_rgo = ws.w.province_s.province_state_container[location].rgo_production;
		bool is_mine = (ws.s.economy_m.goods[local_rgo].flags & economy::good_definition::mined) != 0;
		change_pop_type(ws, this_pop, is_mine ? ws.s.economy_m.rgo_mine.workers[0].type : ws.s.economy_m.rgo_farm.workers[0].type);
	}

	void trigger_rising(world_state& ws, rebel_faction& faction, nations::nation& in_nation) {
		// todo
	}
}
