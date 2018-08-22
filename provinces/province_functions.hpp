#pragma once
#include "common\\common.h"
#include "provinces\\province_functions.h"
#include "world_state\\world_state.h"

namespace provinces {
	template<typename F>
	void for_each_pop(world_state const& ws, provinces::province_state const& p, F&& f) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, p.pops);
		for(auto po : pop_range) {
			if(is_valid_index(po))
				f(ws.w.population_s.pops[po]);
		}
	}
}
