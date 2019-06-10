#pragma once
#include "common\common.h"
#include "military\military.h"

namespace military {
	template<auto new_leader_fn, typename world_state_t>
	RELEASE_INLINE leader_tag internal_make_empty_leader(world_state_t& ws, cultures::culture_tag culture, bool is_general) {
		leader_tag new_leader = new_leader_fn(ws);
		auto cgroup = ws.get<culture::group>(culture);

		auto& local_generator = get_local_generator();
		auto const lp = ws.get<culture_group::leader_pictures>(cgroup);

		if(is_general) {
			std::uniform_int_distribution<int32_t> r(0, int32_t(lp.general_size) - 1);
			ws.set<military_leader::portrait>(
				new_leader,
				ws.get<cultures::leader_pictures>(lp.general_offset + r(local_generator)));
		} else {
			std::uniform_int_distribution<int32_t> r(0, int32_t(lp.admiral_size) - 1);
			ws.set<military_leader::portrait>(
				new_leader,
				ws.get<cultures::leader_pictures>(size_t(lp.admiral_offset + r(local_generator))));
		}

		auto first_name_range = ws.get_row<culture::first_names>(culture);
		auto last_name_range = ws.get_row<culture::last_names>(culture);

		std::uniform_int_distribution<int32_t> fn(0, int32_t(first_name_range.second - first_name_range.first) - 1);
		std::uniform_int_distribution<int32_t> ln(0, int32_t(last_name_range.second - last_name_range.first) - 1);

		ws.set<military_leader::first_name>(new_leader, *(first_name_range.first + fn(local_generator)));
		ws.set<military_leader::last_name>(new_leader, *(last_name_range.first + ln(local_generator)));

		return new_leader;
	}

	template<auto make_empty_leader_fn, auto calculate_leader_traits_fn, typename world_state_t>
	RELEASE_INLINE leader_tag internal_make_auto_leader(world_state_t& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date) {
		leader_tag new_leader = make_empty_leader_fn(ws, culture, is_general);
		ws.set<military_leader::creation_date>(new_leader, creation_date);

		std::uniform_int_distribution<int32_t> br(0, ws.size<military::background_traits>() - 1);
		std::uniform_int_distribution<int32_t> pr(0, ws.size<military::personality_traits>() - 1);

		auto& local_generator = get_local_generator();

		ws.set<military_leader::background>(new_leader, ws.get<military::background_traits>(br(local_generator)));
		ws.set<military_leader::personality>(new_leader, ws.get<military::personality_traits>(pr(local_generator)));

		calculate_leader_traits_fn(ws, new_leader);

		return new_leader;
	}

	template<typename world_state_t>
	RELEASE_INLINE void internal_calculate_leader_traits(world_state_t& ws, leader_tag l) {
		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>> source_a(
			ws.get_row<military::leader_trait_values>(ws.get<military_leader::background>(l)).data());
		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>> source_b(
			ws.get_row<military::leader_trait_values>(ws.get<military_leader::person>(l)).data());

		Eigen::Matrix<traits::value_type, traits::trait_count, 1> dest_vec = source_a + source_b;

		ws.set<military_leader::organisation>(l, dest_vec[military::traits::organisation]);
		ws.set<military_leader::morale>(l, dest_vec[military::traits::morale]);
		ws.set<military_leader::attack>(l, dest_vec[military::traits::attack]);
		ws.set<military_leader::defence>(l, dest_vec[military::traits::defence]);
		ws.set<military_leader::reconnaissance>(l, dest_vec[military::traits::reconnaissance]);
		ws.set<military_leader::speed>(l, dest_vec[military::traits::speed]);
		ws.set<military_leader::experience>(l, dest_vec[military::traits::experience]);
		ws.set<military_leader::reliability>(l, dest_vec[military::traits::reliability]);
	}

	template<auto new_army_function, typename world_state_t>
	RELEASE_INLINE army_tag internal_make_army(world_state_t& ws, nations::country_tag n, provinces::province_tag location) {
		army_tag new_army = new_army_function(ws);
		ws.set<army::location>(new_army, location);

		ws.set<army::owner>(new_army, n);
		ws.add_item(ws.get<nation::armies>(n), new_army);

		auto target_hq = ws.get<province_state::strat_hq>(location);
		if(target_hq) {
			ws.set<army::hq>(new_army, target_hq);
			ws.add_item(ws.get<strategic_hq::army_set>(target_hq), new_army);
		}
		
		return new_army;
	}

	template<auto new_fleet_function, typename world_state_t>
	RELEASE_INLINE fleet_tag internal_make_fleet(world_state_t& ws, nations::country_tag n, provinces::province_tag location) {
		fleet_tag new_fleet = new_fleet_function(ws); // ws.w.military_s.fleets.get_new();
		ws.set<fleet::location>(new_fleet, location);
		ws.add_item(ws.get<nation::fleets>(n), new_fleet);
		return new_fleet;
	}

	template<typename world_state_t>
	RELEASE_INLINE bool internal_in_war_with(world_state_t const& ws, nations::country_tag this_nation, nations::country_tag nation_with) {
		return ws.contains_item(ws.get<nation::allies_in_war>(this_nation), nation_with);
	}

	template<typename world_state_t>
	RELEASE_INLINE bool internal_in_war_against(world_state_t const& ws, nations::country_tag this_nation, nations::country_tag nation_against) {
		return ws.contains_item(ws.get<nation::opponents_in_war>(this_nation), nation_against);
	}

	template<auto check_war_tag_fn, typename world_state_t>
	RELEASE_INLINE void internal_update_at_war_with_and_against(world_state_t& ws, nations::country_tag this_nation) {
		auto& allies_in_war = ws.get<nation::allies_in_war>(this_nation);
		auto& opponents_in_war = ws.get<nation::opponents_in_war>(this_nation);

		ws.resize(allies_in_war, 0ui32);
		ws.resize(opponents_in_war, 0ui32);

		auto owner_wars = ws.get_range(ws.get<nation::wars_involved_in>(this_nation));
		ws.set<nation::is_at_war>(this_nation, owner_wars.first != owner_wars.second);

		for(auto iwar = owner_wars.first; iwar != owner_wars.second; ++iwar) {
			if(auto warid = iwar->war_id; check_war_tag_fn(ws, warid)) {
				if(iwar->is_attacker) {
					auto attacker_range = ws.get_range(ws.get<war::attackers>(warid));
					for(auto d : attacker_range)
						ws.add_item(allies_in_war, d);
					auto defender_range = ws.get_range(ws.get<war::defenders>(warid));
					for(auto d : defender_range)
						ws.add_item(opponents_in_war, d);
				} else {
					auto attacker_range = ws.get_range(ws.get<war::attackers>(warid));
					for(auto d : attacker_range)
						ws.add_item(opponents_in_war, d);
					auto defender_range = ws.get_range(ws.get<war::defenders>(warid));
					for(auto d : defender_range)
						ws.add_item(allies_in_war, d);
				}
			}
		}
	}

	template<auto test_trigger_fn, typename world_state_t>
	RELEASE_INLINE bool internal_can_use_cb_against(world_state_t const& ws, nations::country_tag nation_by, nations::country_tag nation_target) {
		auto pending_range = ws.get_range(ws.get<nation::active_cbs>(nation_by));
		for(auto& c : pending_range) {
			if(c.target == nation_target)
				return true;
		}
		if(ws.if_any<cb_type_tag>([&ws, nation_target](cb_type_tag c) {
			return (ws.get<::cb_type::flags>(c) & cb_type::is_not_triggered_only) == 0 &&
				test_trigger_fn(ws, ws.get<::cb_type::can_use>(c), nation_target, nation_by);
		}))
			return true;
		auto vassal_range = ws.get_range(ws.get<nation::vassals>(nation_target));
		for(auto v : vassal_range) {
			if(internal_can_use_cb_against<test_trigger_fn>(ws, nation_by, nation_target))
				return true;
		}
		return false;
	}

	template<auto test_trigger_fn, auto owns_releasable_core_fn, typename world_state_t>
	RELEASE_INLINE bool internal_test_cb_conditions(world_state_t const& ws, nations::country_tag nation_by, nations::country_tag nation_target, cb_type_tag c) {
		if(auto const allowed_countries = ws.get<::cb_type::allowed_countries>(c); allowed_countries) {
			auto const cb_flags = ws.get<::cb_type::flags>(c);
			if((cb_flags & cb_type::po_release_puppet) != 0) {
				auto vassal_range_b = ws.get_range(ws.get<nation::vassals>(nation_target));
				const auto any_vassal = [&ws, t = allowed_countries, vassal_range_b, nation_target, nation_by]() {
					for(auto v : vassal_range_b) {
						if(test_trigger_fn(ws, t, nation_target, nation_by, v))
							return true;
					}
					return false;
				}();
				if(!any_vassal)
					return false;
			} else if((cb_flags & cb_type::po_take_from_sphere) != 0) {
				auto sm_range = ws.get_range(ws.get<nation::sphere_members>(nation_target));
				const auto any_sphere_member = [&ws, t = allowed_countries, sm_range, nation_target, nation_by]() {
					for(auto v : sm_range) {
						if(test_trigger_fn(ws, t, nation_target, nation_by, v))
							return true;
					}
					return false;
				}();
				if(!any_sphere_member)
					return false;
			} else if((cb_flags & cb_type::po_liberate) != 0) {
				return owns_releasable_core_fn(ws, nation_target);
			} else {
				if(!test_trigger_fn(ws, allowed_countries, nation_target, nation_by, nation_target))
					return false;
			}
		}
		if(auto const allowed_states = ws.get<::cb_type::allowed_states>(c); allowed_states) {
			const auto any_state = [&ws, t = allowed_states, nation_target, nation_by]() {
				auto target_state_range = ws.get_range(ws.get<nation::member_states>(nation_target));
				for(auto p : target_state_range) {
					if(test_trigger_fn(ws, t, p.state, nation_by, nation_target))
						return true;
				}
				return false;
			}();
			if(!any_state)
				return false;
		}
		return true;
	}
}
