#pragma once
#include "common\\common.h"
#include "nations_functions.h"
#include "world_state\\world_state.h"

namespace nations {
	template<typename F>
	void for_each_state(world_state const& ws, nations::country_tag n, F&& f) {
		auto mrange = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(n));
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
	void for_each_province(world_state const& ws, nations::country_tag n, F&& f) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));
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
					f(po);
			}
		});
	}
	template<typename F>
	void for_each_pop(world_state const& ws, nations::country_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(po);
			}
		});
	}


	template<typename F>
	void for_each_state(world_state& ws, nations::country_tag n, F&& f) {
		auto mrange = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(n));
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
	void for_each_province(world_state& ws, nations::country_tag n, F&& f) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));
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
					f(po);
			}
		});
	}
	template<typename F>
	void for_each_pop(world_state& ws, nations::country_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(po);
			}
		});
	}


	template<typename C, typename T>
	auto is_culture_accepted(world_state const& ws, C c, T n) -> decltype(ve::widen_to<T>(true)) {
		return ve::apply(n, c, [&ws](nations::country_tag in, cultures::culture_tag ic) {
			contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(in), ic);
		}) | (ve::load(n, ws.w.nation_s.nations.get_row<nation::primary_culture>()) == c);
	}


	template<typename T>
	auto national_culture_group(world_state const& ws, T n)-> decltype(ve::widen_to<T>(cultures::culture_group_tag())) {
		auto pcultures = ve::load(n, ws.w.nation_s.nations.get_row<nation::primary_culture>());
		return ve::load(pcultures, ws.s.culture_m.cultures_to_groups.view());
	}


	template<typename T>
	auto union_holder_of(world_state const& ws, T this_nation) -> decltype(ve::widen_to<T>(nations::country_tag())) {
		auto pculture = ve::load(this_nation, ws.w.nation_s.nations.get_row<nation::primary_culture>());
		return union_holder_for(ws, pculture);
	}

	template<typename T>
	auto union_holder_for(world_state const& ws, T c) -> decltype(ve::widen_to<T>(nations::country_tag())) {
		auto union_tag = ve::load(c, ws.s.culture_m.cultures_to_tags.view());
		return ve::load(union_tag, ws.w.culture_s.tags_to_holders.view());
	}
}
