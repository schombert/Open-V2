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
	template<typename T>
	auto crime_fighting_value(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f)) {
		auto si = province_state(ws, p);
		auto owner = province_owner(ws, p);
		auto state_admin_eff = ve::load(si, ws.w.nation_s.states.get_row<state::administrative_efficiency>());
		auto owner_admin_spending = ve::load(owner, ws.w.nation_s.nations.get_row<nation::f_administrative_spending>());
		auto const gamma = ws.s.modifiers_m.global_defines.admin_efficiency_crimefight_percent;

		//return ws.s.modifiers_m.global_defines.min_crimefight_percent
		//	+ (ws.s.modifiers_m.global_defines.max_crimefight_percent - ws.s.modifiers_m.global_defines.min_crimefight_percent)
		//	* (ve::multiply_and_add(owner_admin_spending, (1.0f - gamma), state_admin_eff * gamma));

		auto const range = ws.s.modifiers_m.global_defines.max_crimefight_percent - ws.s.modifiers_m.global_defines.min_crimefight_percent;
		return ws.s.modifiers_m.global_defines.min_crimefight_percent
			+ (ve::multiply_and_add(state_admin_eff - owner_admin_spending, gamma * range, owner_admin_spending * range));
	}

	template<typename F>
	auto visit_connected_owned_provinces(world_state const& ws, provinces::province_tag start, F const& fun) -> void {
		auto const set = get_connected_owned_provinces(ws, start);
		for(auto p : set)
			fun(p);
	}


	template<typename F>
	auto provinces_are_connected(world_state const& ws, provinces::province_tag start, provinces::province_tag end, float range, F const& test_fn) -> bool {
		
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>>
			already_added;
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>>
			pending_neighbor_check;

		pending_neighbor_check.insert(end);

		while(pending_neighbor_check.size() != 0) {
			auto tp = *(pending_neighbor_check.end() - 1);
			pending_neighbor_check.erase(pending_neighbor_check.end() - 1);

			already_added.insert(tp);

			if(test_fn(tp)) {
				auto adj_range = ws.s.province_m.same_type_adjacency.get_range(tp);
				for(auto a : adj_range) {
					if(ws.get<province::centroid>(start).dot(ws.get<province::centroid>(a)) >= range
						&& already_added.count(a) == 0
						&& pending_neighbor_check.count(a) == 0) {

						if(a == start)
							return true;

						pending_neighbor_check.insert(a);
					}
				}
			}
		}

		return false;
	}

	template<typename F>
	auto get_limited_connected_provinces(world_state const& ws, provinces::province_tag start, float range, F const& test_fn)->boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>> {
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>>
			already_added;
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>>
			pending_neighbor_check;
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>>
			failed_set;

		already_added.insert(start);

		// force addition of adjacent provinces redardless of range
		auto adj_range = ws.s.province_m.same_type_adjacency.get_range(start);
		for(auto a : adj_range) {
			pending_neighbor_check.insert(a);
		}

		while(pending_neighbor_check.size() != 0) {
			auto tp = *(pending_neighbor_check.end() - 1);
			pending_neighbor_check.erase(pending_neighbor_check.end() - 1);

			if(!test_fn(tp)) {
				failed_set.insert(tp);
			} else {
				already_added.insert(tp);
			
				auto adj_range = ws.s.province_m.same_type_adjacency.get_range(tp);
				for(auto a : adj_range) {
					if(ws.get<province::centroid>(start).dot(ws.get<province::centroid>(a)) >= range
						&& already_added.count(a) == 0
						&& pending_neighbor_check.count(a) == 0
						&& failed_set.count(a) == 0) {

						pending_neighbor_check.insert(a);
					}
				}
			}
		}

		return already_added;
	}
}
