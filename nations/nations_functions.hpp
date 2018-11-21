#pragma once
#include "common\\common.h"
#include "nations_functions.h"
#include "world_state\\world_state.h"

namespace nations {
	template<typename F>
	void for_each_state(world_state const& ws, nations::nation const& n, F&& f) {
		auto mrange = get_range(ws.w.nation_s.state_arrays, n.member_states);
		for(auto s = mrange.first; s != mrange.second; ++s) {
			auto state = s->state;
			if(bool(state))
				f(state);
		}
	}

	template<typename F>
	void for_each_province(world_state const& ws, state_tag s, F&& f) {
		auto prange = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(s));
		for(auto p : prange) {
			if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == s)
				f(p);
		}
	}

	template<typename F>
	void for_each_province(world_state const& ws, nations::nation const& n, F&& f) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, n.owned_provinces);
		for(auto p : owned_range) {
			if(is_valid_index(p))
				f(p);
		}
	}
	template<typename F>
	void for_each_pop(world_state const& ws, state_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(ws.w.population_s.pops[po]);
			}
		});
	}
	template<typename F>
	void for_each_pop(world_state const& ws, nations::nation const& s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(ws.w.population_s.pops[po]);
			}
		});
	}


	template<typename F>
	void for_each_state(world_state& ws, nations::nation& n, F&& f) {
		auto mrange = get_range(ws.w.nation_s.state_arrays, n.member_states);
		for(auto s = mrange.first; s != mrange.second; ++s) {
			auto state = s->state;
			if(bool(state))
				f(state);
		}
	}

	template<typename F>
	void for_each_province(world_state& ws, state_tag s, F&& f) {
		auto prange = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(s));
		for(auto p : prange) {
			if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == s)
				f(p);
		}
	}

	template<typename F>
	void for_each_province(world_state& ws, nations::nation& n, F&& f) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, n.owned_provinces);
		for(auto p : owned_range) {
			if(is_valid_index(p))
				f(p);
		}
	}
	template<typename F>
	void for_each_pop(world_state& ws, state_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(ws.w.population_s.pops[po]);
			}
		});
	}
	template<typename F>
	void for_each_pop(world_state& ws, nations::nation& s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(ws.w.population_s.pops[po]);
			}
		});
	}
}
