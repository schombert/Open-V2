#pragma once
#include "common\\common.h"
#include "population_functions.h"
#include "world_state\\world_state.h"

namespace population {
	template<typename T>
	auto desired_needs_spending(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f)) {
		return ve::widen_to<T>(0.0f);
	}

	template<typename T>
	auto get_pop_owner(world_state const& ws, T p) -> decltype(ve::widen_to<T>(nations::country_tag())) {
		auto loc = ve::load(p, ws.w.population_s.pops.get_row<pop::location>());
		return ve::load(loc, ws.w.province_s.province_state_container.get_row<province_state::owner>());
	}

	template<typename T>
	auto get_militancy_direct(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f)) {
		return ve::load(p, ws.w.population_s.pops.get_row<pop::militancy>()) * 10.0f;
	}
	template<typename T>
	auto get_literacy_direct(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f)) {
		return ve::load(p, ws.w.population_s.pops.get_row<pop::literacy>());
	}
	template<typename T>
	auto get_consciousness_direct(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f)) {
		return ve::load(p, ws.w.population_s.pops.get_row<pop::consciousness>()) * 10.0f;
	}
}
