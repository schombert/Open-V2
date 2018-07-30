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
		p.militancy = static_cast<uint16_t>(v * float(std::numeric_limits<uint16_t>::max()) / 10.0f);
	}
	void set_literacy_direct(pop &p, float v) {
		p.literacy = static_cast<uint16_t>(v * float(std::numeric_limits<uint16_t>::max()));
	}
	void set_consciousness_direct(pop & p, float v) {
		p.consciousness = static_cast<uint16_t>(v * float(std::numeric_limits<uint16_t>::max()) / 10.0f);
	}
	economy::money_qnty_type desired_needs_spending(world_state&, pop const&) {
		return economy::money_qnty_type(0);
	}

	nations::nation* get_pop_owner(world_state& ws, pop const& p) {
		auto loc = p.location;
		if(is_valid_index(loc))
			return ws.w.province_s.province_state_container[loc].owner;
		else
			return nullptr;
	}
}
