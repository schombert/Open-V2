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

	template<typename T>
	auto get_life_rating(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f)) {
		auto base_ratings = ve::load(p, ws.w.province_s.province_state_container.get_row<province_state::base_life_rating>());
		auto modifiers = ve::load(p, ws.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::life_rating>(0));
		return ve::multiply_and_add(modifiers, base_ratings, base_ratings);
	}

	template<typename T>
	auto province_owner(world_state const& ws, T p) -> decltype(ve::widen_to<T>(nations::country_tag())) {
		return ve::load(p, ws.w.province_s.province_state_container.get_row<province_state::owner>());
	}
	template<typename T>
	auto province_state(world_state const& ws, T p) -> decltype(ve::widen_to<T>(nations::state_tag())) {
		return ve::load(p, ws.w.province_s.province_state_container.get_row<province_state::state_instance>());
	}
	template<typename T>
	auto province_controller(world_state const& ws, T p) -> decltype(ve::widen_to<T>(nations::country_tag())) {
		return ve::load(p, ws.w.province_s.province_state_container.get_row<province_state::controller>());
	}
}
