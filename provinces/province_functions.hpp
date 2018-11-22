#pragma once
#include "common\\common.h"
#include "provinces\\province_functions.h"
#include "world_state\\world_state.h"

namespace provinces {
	template<typename F>
	void for_each_pop(world_state const& ws, provinces::province_tag p, F&& f) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(p));
		for(auto po : pop_range) {
			if(is_valid_index(po))
				f(po);
		}
	}
}
