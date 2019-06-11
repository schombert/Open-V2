#pragma once
#include "common\\common.h"
#include "nations_functions.h"
#include "world_state\\world_state.h"
#include "concurrency_tools\\ve.h"

namespace nations {
	template<typename world_state_t, typename F>
	void for_each_state(world_state_t const& ws, nations::country_tag n, F&& f) {
		auto mrange = ws.get_range(ws.get<nation::member_states>(n));
		for(auto s = mrange.first; s != mrange.second; ++s) {
			auto state = s->state;
			if(bool(state))
				f(state);
		}
	}

	template<typename world_state_t, typename F>
	void for_each_province(world_state_t const& ws, state_tag s, F&& f) {
		auto region = ws.get<state::region_id>(s);
		auto prange = is_valid_index(region)
			? ws.s.province_m.states_to_province_index.get_row(region)
			: std::pair<provinces::province_tag const*, provinces::province_tag const*>(nullptr, nullptr);
		for(auto p : prange) {
			if(ws.get<province_state::state_instance>(p) == s)
				f(p);
		}
	}

	template<typename world_state_t, typename F>
	void for_each_province(world_state_t const& ws, nations::country_tag n, F&& f) {
		auto owned_range = ws.get_range(ws.get<nation::owned_provinces>(n));
		for(auto p : owned_range) {
			if(is_valid_index(p))
				f(p);
		}
	}
	template<typename world_state_t, typename F>
	void for_each_pop(world_state_t const& ws, state_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = ws.get_range(ws.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(po);
			}
		});
	}
	template<typename world_state_t, typename F>
	void for_each_pop(world_state_t const& ws, nations::country_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = ws.get_range(ws.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(po);
			}
		});
	}


	template<typename world_state_t, typename F>
	void for_each_state(world_state_t& ws, nations::country_tag n, F&& f) {
		auto mrange = ws.get_range(ws.get<nation::member_states>(n));
		for(auto s = mrange.first; s != mrange.second; ++s) {
			auto state = s->state;
			if(bool(state))
				f(state);
		}
	}

	template<typename world_state_t, typename F>
	void for_each_province(world_state_t& ws, state_tag s, F&& f) {
		auto region = ws.get<state::region_id>(s);
		auto prange = is_valid_index(region)
			? ws.s.province_m.states_to_province_index.get_row(region)
			: std::pair<provinces::province_tag*, provinces::province_tag*>(nullptr, nullptr);

		for(auto p : prange) {
			if(ws.get<province_state::state_instance>(p) == s)
				f(p);
		}
	}

	template<typename world_state_t, typename F>
	void for_each_province(world_state_t& ws, nations::country_tag n, F&& f) {
		auto owned_range = ws.get_range(ws.get<nation::owned_provinces>(n));
		for(auto p : owned_range) {
			if(is_valid_index(p))
				f(p);
		}
	}
	template<typename world_state_t, typename F>
	void for_each_pop(world_state_t& ws, state_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = ws.get_range(ws.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(po);
			}
		});
	}
	template<typename world_state_t, typename F>
	void for_each_pop(world_state_t& ws, nations::country_tag s, F&& f) {
		for_each_province(ws, s, [&ws, &f](provinces::province_tag p) {
			auto pop_range = ws.get_range(ws.get<province_state::pops>(p));
			for(auto po : pop_range) {
				if(is_valid_index(po))
					f(po);
			}
		});
	}


	template<typename C, typename T>
	auto is_culture_accepted(world_state const& ws, C c, T n) -> decltype(ve::widen_to<C, T>(true)) {
		return ve::apply(n, c, [&ws](nations::country_tag in, cultures::culture_tag ic) {
			return contains_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(in), ic);
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

	template<typename T>
	auto get_prestige(world_state const& ws, T this_nation) -> decltype(ve::widen_to<T>(0.0f)) {
		return 
			ve::load(this_nation, ws.w.nation_s.nations.get_row<nation::base_prestige>())
			+ ve::load(this_nation, ws.w.nation_s.tech_attributes.get_row<technologies::tech_offset::permanent_prestige>(0));
	}

	template<typename T>
	auto has_factory(world_state const& ws, T this_state) -> decltype(ve::widen_to<T>(true)) {
		return ve::apply(this_state, [&ws](nations::state_tag si) {
			auto& factories = ws.w.nation_s.states.get<state::factories>(si);
			
			for(uint32_t i = 0; i < state::factories_count; ++i) {
				if(factories[i].type)
					return true;
			}
			return false;
		});
	}

	template<typename T>
	auto has_factory(world_state const& ws, T this_state, economy::factory_type_tag f_type) -> decltype(ve::widen_to<T>(true)) {
		return ve::apply(this_state, [&ws, f_type](nations::state_tag si) {
			auto& factories = ws.w.nation_s.states.get<state::factories>(si);

			for(uint32_t i = 0; i < state::factories_count; ++i) {
				if(factories[i].type && factories[i].type->id == f_type)
					return true;
			}
			return false;
		});
	}

	template<typename T>
	auto national_treasury(world_state const& ws, T this_nation)-> decltype(ve::widen_to<T>(0.0f)) {
		return ve::load(this_nation, ws.w.nation_s.nations.get_row<nation::treasury>());
	}

	template<typename T>
	auto state_owner(world_state const& ws, T this_state)-> decltype(ve::widen_to<T>(nations::country_tag())) {
		return ve::load(this_state, ws.w.nation_s.states.get_row<state::owner>());
	}

	template<typename T>
	auto is_colonial_or_protectorate(world_state const& ws, T s) -> decltype(ve::widen_to<T>(true)) {
		return ve::widen_mask(
			ve::load(s, ws.w.nation_s.states.get_row<state::is_protectorate>())
			| ve::load(s, ws.w.nation_s.states.get_row<state::is_colonial>()));
	}

	template<typename T>
	auto is_great_power(world_state const& ws, T this_nation)-> decltype(ve::widen_to<T>(true)) {
		return ve::load(this_nation, ws.w.nation_s.nations.get_row<nation::overall_rank>()) <= 8i16;
	}

	template<typename T>
	auto state_port_province(world_state const& ws, T this_state) -> decltype(ve::widen_to<T>(provinces::province_tag())) {
		return ve::apply(this_state, [&ws](nations::state_tag s) {
			provinces::province_tag port;
			nations::for_each_province(ws, s, [&port, &ws](provinces::province_tag p) {
				if(ws.w.province_s.province_state_container.get<province_state::naval_base_level>(p) > 0
					|| ws.w.province_s.province_state_container.get<province_state::naval_base_upgrade_progress>(p) != 0)
					port = p;
			});
			return port;
		});
	}

	template<typename T>
	auto union_tag_of(world_state const& ws, T this_nation) -> decltype(ve::widen_to<T>(cultures::national_tag())) {
		auto pculture = ve::load(this_nation, ws.w.nation_s.nations.get_row<nation::primary_culture>());
		return ve::load(pculture, ws.s.culture_m.cultures_to_tags.view());
	}

	template<typename T>
	auto nation_exists(world_state const& ws, T n) -> decltype(ve::widen_to<T>(true)) {
		return is_valid_index(ve::load(n, ws.w.nation_s.nations.get_row<nation::current_capital>()));
	}

	template<typename F>
	void parallel_for_each_province(world_state const& ws, nations::country_tag n, F&& f) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));
		concurrency::parallel_for_each(owned_range.first, owned_range.second, [&f](provinces::province_tag p) {
			if(is_valid_index(p))
				f(p);
		});
	}
	template<typename F>
	void parallel_for_each_province(world_state& ws, nations::country_tag n, F&& f) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n));
		concurrency::parallel_for_each(owned_range.first, owned_range.second, [&f](provinces::province_tag p) {
			if(is_valid_index(p))
				f(p);
		});
	}
}
