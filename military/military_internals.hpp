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

	template<auto test_trigger_fn, auto test_cb_conditions_fn, typename world_state_t>
	RELEASE_INLINE bool internal_is_cb_construction_valid_against(world_state_t const& ws, cb_type_tag cb, nations::country_tag nation_by, nations::country_tag nation_target) {
		auto const cb_flags = ws.get<::cb_type::flags>(cb);

		if((cb_flags & (cb_type::is_not_triggered_only | cb_type::is_not_constructing_cb)) != 0)
			return false;
		if(is_valid_index(ws.get<::cb_type::allowed_substate_regions>(cb))) {
			return false;
		}
		if(auto const can_use = ws.get<::cb_type::can_use>(cb);  is_valid_index(can_use)) {
			if(!test_trigger_fn(ws, can_use, nation_target, nation_by, triggers::const_parameter()))
				return false;
		}

		return test_cb_conditions_fn(ws, nation_by, nation_target, cb);
	}

	template<auto nation_exists_fn, auto get_player_fn, auto test_trigger_fn, typename world_state_t>
	RELEASE_INLINE void internal_init_player_cb_state(world_state_t& ws) {
		ws.for_each<cb_type_tag>([&ws](cb_type_tag c){
			auto const cb_flags = ws.get<::cb_type::flags>(c);
			if((cb_flags & cb_type::is_not_triggered_only) != 0) {
				auto& bit_vector = ws.get<current_state::player_cb_state>(c);
				bit_vector.resize(ws.size<nation::name>());

				ws.for_each<nations::country_tag>([&ws, &bit_vector, can_use = ws.get<::cb_type::can_use>(c)](nations::country_tag n) {
					if(nation_exists_fn(ws, n) && get_player_fn(ws) != n) {
						bit_vector.set(n,
							!is_valid_index(can_use) || test_trigger_fn(ws, can_use, n, get_player_fn(ws), triggers::const_parameter()));
					}
				});
			}
		});
	}

	template<auto nation_exists_fn, auto get_player_fn, auto test_trigger_fn, auto post_msg_fn, typename world_state_t>
	RELEASE_INLINE void internal_update_player_cb_state(world_state_t& ws) {
		if(get_player_fn(ws))
			return;

		ws.par_for_each<military::cb_type_tag>([&ws](military::cb_type_tag c) {
			auto const cb_flags = ws.get<::cb_type::flags>(c);
			if((cb_flags & cb_type::is_not_triggered_only) != 0) {
				auto& bit_vector = ws.get<current_state::player_cb_state>(c);
				bit_vector.resize(ws.size<nation::name>());

				ws.for_each<nations::country_tag>([&ws, c, &bit_vector, can_use = ws.get<::cb_type::can_use>(c)](nations::country_tag n) {
					const auto old_state = bit_vector[n];
					if(nation_exists_fn(ws, n) && get_player_fn(ws) != n) {
						auto const new_state = !is_valid_index(can_use) || test_trigger_fn(ws, can_use, n, get_player_fn(ws), triggers::const_parameter());
						bit_vector.set(n, new_state);

						if(new_state != old_state) {
							post_msg_fn(new_state, ws, get_player_fn(ws), n, c);
						}
					}
				});
			}
		});
	}

	template<typename world_state_t>
	RELEASE_INLINE bool internal_has_units_in_province(world_state_t const& ws, nations::country_tag this_nation, provinces::province_tag ps) {
		auto orders = ws.get<province_state::orders>(ps);
		if(orders) {
			auto involved_range = ws.get_range(ws.get<army_order::army_set>(orders));
			for(auto a : involved_range) {
				if(ws.get<army::owner>(a) == this_nation)
					return true;
			}
		}
		return false;
	}

	template<auto regiment_sz_fn, typename world_state_t>
	RELEASE_INLINE uint32_t internal_total_units_in_province(world_state_t const& ws, provinces::province_tag ps) {
		auto orders = ws.get<province_state::orders>(ps);
		if(orders) {
			auto& o_obj = ws.w.military_s.army_orders_container[orders];

			float total = 0;
			const float regiment_size = regiment_sz_fn(ws);

			auto province_size = get_size(ws.get<army_order::province_set>(orders));
			auto involved_range = get_range(ws.get<army_order::army_set>(orders));

			if(province_size == 0)
				return 0ui32;

			for(auto a : involved_range) {
				total += ws.get<army::current_soldiers>(a);
			}

			return uint32_t(total / (regiment_size * province_size) + 0.5f);
		} else {
			return 0ui32;
		}
	}

	template<auto regiment_sz_fn, typename world_state_t>
	RELEASE_INLINE uint32_t internal_total_active_divisions(world_state_t const& ws, nations::country_tag this_nation) {
		float total = 0ui32;
		const float regiment_size = regiment_sz_fn(ws);

		auto army_range = get_range(ws.get<nation::armies>(this_nation));
		for(auto a : army_range) {
			total += ws.get<army::current_soldiers>(a);
		}
		return uint32_t(total / regiment_size + 0.5f);
	}

	template<typename world_state_t>
	RELEASE_INLINE uint32_t internal_total_active_ships(world_state_t const& ws, nations::country_tag this_nation) {
		float total = 0;
		auto fleet_range = get_range(ws.get<nation::fleets>(this_nation));
		for(auto f : fleet_range) {
			total += ws.get<fleet::size>(f);
		}
		return uint32_t(total + 0.5f);
	}

	template<auto soldier_demo_index, typename world_state_t>
	RELEASE_INLINE float internal_recruited_pop_fraction(world_state_t const& ws, nations::country_tag this_nation) {
		float total_soldier_pops =
			ws.get<nation::demographics>(this_nation, soldier_demo_index(ws))
			+ ws.get<nation::colonial_demographics>(this_nation, soldier_demo_index(ws));
		float total_soldier_pops_assigned = 0;

		if(total_soldier_pops <= 0)
			return 0.0f;

		auto army_range = get_range(ws.get<nation::armies>(this_nation));
		for(auto a : army_range) {
			total_soldier_pops_assigned += ws.get<army::current_soldiers>(a);
		}

		return total_soldier_pops_assigned / total_soldier_pops;
	}

	template<auto msg_function, typename world_state_t>
	RELEASE_INLINE void internal_remove_from_war(world_state_t& ws, war_tag this_war, nations::country_tag to_remove) {
		msg_function(ws, this_war, to_remove);

		ws.remove_item_if(ws.get<war::war_goals>(this_war),
			[to_remove](war_goal const& wg) { return wg.from_country == to_remove || wg.target_country == to_remove; });
		ws.remove_item(ws.get<war::attackers>(this_war), to_remove);
		ws.remove_item(ws.get<war::defenders>(this_war), to_remove);

		if(ws.get<war::primary_attacker>(this_war) == to_remove) {
			nations::country_tag best = nations::country_tag();
			int16_t best_rank = std::numeric_limits<int16_t>::max();

			auto attackers = ws.get_range(ws.get<war::attackers>(this_war));
			for(auto n : attackers) {
				auto rank = ws.get<nation::overall_rank>(n);
				if(rank < best_rank) {
					best_rank = rank;
					best = n;
				}
			}

			ws.set<war::primary_attacker>(this_war, best);
		} else if(ws.get<war::primary_defender>(this_war) == to_remove) {
			nations::country_tag best = nations::country_tag();
			int16_t best_rank = std::numeric_limits<int16_t>::max();

			auto defenders = ws.get_range(ws.get<war::defenders>(this_war));
			for(auto n : defenders) {
				auto rank = ws.get<nation::overall_rank>(n);
				if(rank < best_rank) {
					best_rank = rank;
					best = n;
				}
			}

			ws.set<war::primary_defender>(this_war, best);
		}

		ws.remove_item(ws.get<nation::wars_involved_in>(to_remove), war_identifier{ this_war, false });
	}

	template<auto msg_function, typename world_state_t>
	RELEASE_INLINE void internal_add_to_war(world_state_t& ws, war_tag this_war, bool attacker, nations::country_tag to_add) {
		msg_function(ws, this_war, attacker, to_add);

		auto& allies_in_war = ws.get<nation::allies_in_war>(to_add);
		auto& opponents_in_war = ws.get<nation::opponents_in_war>(to_add);

		if(attacker) {
			auto attacker_range = ws.get_range(ws.get<war::attackers>(this_war));
			for(auto a : attacker_range) {
				ws.add_item(allies_in_war, a);
				ws.add_item(ws.get<nation::allies_in_war>(a), to_add);
			}
			auto defender_range = ws.get_range(ws.get<war::defenders>(this_war));
			for(auto a : defender_range) {
				ws.add_item(opponents_in_war, a);
				ws.add_item(ws.get<nation::opponents_in_war>(a), to_add);
			}
			ws.add_item(ws.get<war::attackers>(this_war), to_add);
			ws.add_item(ws.get<nation::wars_involved_in>(to_add), war_identifier{ this_war, true });
		} else {
			auto attacker_range = get_range(ws.get<war::attackers>(this_war));
			for(auto a : attacker_range) {
				ws.add_item(opponents_in_war, a);
				ws.add_item(ws.get<nation::opponents_in_war>(a), to_add);
			}
			auto defender_range = get_range(ws.get<war::defenders>(this_war));
			for(auto a : defender_range) {
				ws.add_item(allies_in_war, a);
				ws.add_item(ws.get<nation::allies_in_war>(a), to_add);
			}
			ws.add_item(ws.get<war::defenders>(this_war), to_add);
			ws.add_item(ws.get<nation::wars_involved_in>(to_add), war_identifier{ this_war, false });
		}
	}

	template<typename world_state_t>
	RELEASE_INLINE bool internal_is_target_of_war_goal(world_state_t const& ws, war_tag this_war, nations::country_tag target) {
		auto wg_range = ws.get_range(ws.get<war::war_goals>(this_war));
		for(auto& wg : wg_range) {
			if(wg.target_country == target)
				return true;
		}
		return false;
	}

	template<auto release_army_fn, bool remove_from_nation_arrays, typename world_state_t>
	RELEASE_INLINE void internal_destroy_army(world_state_t& ws, army_tag a) {
		if constexpr(remove_from_nation_arrays) {
			auto const owner = ws.get<army::owner>(a);
			ws.remove_item(ws.get<nation::armies>(owner), a);
		}

		if(auto o = ws.get<army::order>(a); o) {
			ws.remove_item(ws.get<army_order::army_set>(o), a);
		}
		if constexpr(remove_from_nation_arrays) {
			if(auto h = ws.get<army::hq>(a); h) {
				ws.get<strategic_hq::allocated_soldiers>(h) -= ws.get<army::current_soldiers>(a);
				ws.remove_item(ws.get<strategic_hq::army_set>(h), a);
			}
		}
		if(auto l = ws.get<army::leader>(a); l) {
			ws.set<military_leader::is_attached>(l, false);
		}
		release_army_fn(ws, a);
	}

	template<auto release_fleet_fn, bool remove_from_nation_arrays, typename world_state_t>
	RELEASE_INLINE void internal_destroy_fleet(world_state_t& ws, fleet_tag f) {
		if constexpr(remove_from_nation_arrays) {
			auto const owner = ws.get<army::owner>(f);
			ws.remove_item(ws.get<nation::fleets>(owner), f);
		}

		if(auto l = ws.get<fleet::leader>(f); l) {
			ws.set<military_leader::is_attached>(l, false);
		}

		release_fleet_fn(ws, f);
	}
	

	template<auto release_orders_fn, bool remove_from_nation_arrays, typename world_state_t>
	RELEASE_INLINE void internal_destroy_orders(world_state_t& ws, army_orders_tag o, nations::country_tag owner) {
		
		auto army_range = get_range(ws.get<army_order::army_set>(o));
		for(auto a : army_range)
			ws.set<army::order>(a, army_orders_tag());
		ws.clear(ws.get<army_order::army_set>(o));

		if constexpr(remove_from_nation_arrays) {
			ws.remove_item(ws.get<nation::active_orders>(owner), o);
		}

		auto prange = ws.get_range(ws.get<army_order::province_set>(o));
		for(auto p : prange)
			ws.set<province_state::orders>(p, military::army_orders_tag());
		ws.clear(ws.get<army_order::province_set>(o));

		if(auto l = ws.get<army_order::leader>(o); l) {
			ws.set<military_leader::is_attached>(l, false);
		}

		release_orders_fn(ws, o);
	}

	template<auto release_hq_fn, auto destroy_army_fn, bool remove_from_nation_arrays, typename world_state_t>
	RELEASE_INLINE void internal_destroy_hq(world_state_t& ws, strategic_hq_tag o, nations::country_tag owner) {
		auto army_range = get_range(ws.get<strategic_hq::army_set>(o));
		if constexpr(remove_from_nation_arrays) {
			for(auto a : army_range) {
				ws.set<army::hq>(a, strategic_hq_tag());
				destroy_army_fn(ws, a);
			}
		}
		ws.clear(ws.get<strategic_hq::army_set>(o));

		auto prange = ws.get_range(ws.get<strategic_hq::province_set>(o));
		for(auto p : prange)
			ws.set<province_state::strat_hq>(p, strategic_hq_tag());
		ws.clear(ws.get<strategic_hq::province_set>(o));

		if constexpr(remove_from_nation_arrays) {
			ws.remove_item(ws.get<nation::strategic_hqs>(owner), o);
		}

		release_hq_fn(ws, o);
	}

	template<auto release_leader_fn, typename world_state_t>
	RELEASE_INLINE void internal_destroy_admiral(world_state_t& ws, leader_tag l, nations::country_tag owner) {
		if(ws.get<military_leader::is_attached>(l)) {
			auto fr = ws.get_range(ws.get<nation::fleets>(owner));
			for(auto fl : fr) {
				if(ws.get<fleet::leader>(fl) == l) {
					ws.set<fleet::leader>(fl, leader_tag());
					break;
				}
			}
		}

		release_leader_fn(l);
	}

	template<auto release_leader_fn, typename world_state_t>
	RELEASE_INLINE void internal_destroy_general(world_state_t& ws, leader_tag l, nations::country_tag owner) {
		if(ws.get<military_leader::is_attached>(l)) {
			[&ws, l, owner]() {
				auto ar = ws.get_range(ws.get<nation::armies>(owner));
				for(auto a : ar) {
					if(ws.get<army::leader>(a) == l) {
						ws.set<army::leader>(a, leader_tag());
						return;
					}
				}

				auto orr = ws.get_range(ws.get<nation::active_orders>(owner));
				for(auto o : orr) {
					if(ws.get<army_order::leader>(o) == l) {
						ws.set<army_order::leader>(o, leader_tag());
						return;
					}
				}

				auto hr = ws.get_range(ws.get<nation::strategic_hqs>(owner));
				for(auto h : hr ) {
					if(ws.get<strategic_hq::leader>(h) == l) {
						ws.set<strategic_hq::leader>(h, leader_tag());
						return;
					}
				}
			}();
		
		}
		release_leader_fn(l);
	}

	template<auto release_war_fn, typename world_state_t>
	RELEASE_INLINE void internal_destroy_war(world_state_t& ws, war_tag this_war) {
		auto attacker_range = ws.get_range(ws.get<war::attackers>(this_war));
		for(auto a : attacker_range)
			ws.remove_item(ws.get<nation::wars_involved_in>(a), military::war_identifier{ this_war, false });

		auto defender_range = ws.get_range(ws.get<war::defenders>(this_war));
		for(auto a : defender_range)
			ws.remove_item(ws.get<nation::wars_involved_in>(a), military::war_identifier{ this_war, false });

		ws.clear(ws.get<war::attackers>(this_war));
		ws.clear(ws.get<war::defenders>(this_war));
		ws.clear(ws.get<war::naval_control_set>(this_war));
		ws.clear(ws.get<war::war_goals>(this_war));

		release_war_fn(this_war);
	}

	template<typename world_state_t>
	RELEASE_INLINE war_tag internal_get_war_between(world_state_t& ws, nations::country_tag a, nations::country_tag b) {
		auto wars = ws.get_range(ws.get<nation::wars_involved_in>(a));
		for(auto w = wars.first; w != wars.second; ++w) {
			if(w->is_attacker) {
				if(ws.contains_item(ws.get<war::defenders>(w->war_id), b))
					return w->war_id;
			} else {
				if(ws.contains_item(ws.get<war::attackers>(w->war_id), b))
					return w->war_id;
			}
		}
		return war_tag();
	}
	
	template<auto allocate_war_fn, auto date_fn, typename world_state_t>
	RELEASE_INLINE war_tag internal_create_war(world_state_t& ws, nations::country_tag attacker, nations::country_tag defender, bool /*call_willing_attacker_allies*/) {
		war_tag new_war = allocate_war_fn(ws);
		ws.set<war::primary_attacker>(new_war, attacker);
		ws.set<war::primary_defender>(new_war, defender);
		ws.add_item(ws.get<war::attackers>(new_war), attacker);
		ws.add_item(ws.get<war::defenders>(new_war), defender);

		ws.set<war::start_date>(new_war, date_fn(ws));

		// todo: call defender allies
		// todo: call attacker allies
		// todo: populate naval control
		// todo: name war

		return new_war;
	}

	template<auto count_factories_fn, auto state_is_colonial_fn, typename world_state_t>
	float internal_single_state_war_score(world_state_t const& ws, nations::state_tag si, float owner_total_pop) {
		int32_t factory_count = count_factories_fn(ws, si);
		auto owner_id = ws.get<state::owner>(si);

		float running_total = 0.0f;

		if(bool(owner_id) && !state_is_colonial_fn(ws, si)) {
			auto owner_tag = ws.get<nation::tag>(owner_id);
			auto owner_cap = ws.get<nation::current_capital>(owner_id);

			nations::for_each_province(ws, si, [&running_total, owner_tag, owner_cap, owner_total_pop, &ws, &factory_count](provinces::province_tag prov) {
				auto province_pop = ws.get<province::demographics>(prov, population::total_population_tag);

				int32_t base_cost_temp = 
					ws.get<province_state::fort_level>(prov)
					+ ws.get<province_state::naval_base_level>(prov)
					+ factory_count + 1;

				factory_count = 0; // only add factories once

				if(ws.contains_item(ws.get<province_state::cores>(prov), owner_tag))
					base_cost_temp *= 2;
				if(owner_cap == prov)
					base_cost_temp *= 3;

				running_total += 2.8f * std::clamp((owner_total_pop != 0) ? (float(base_cost_temp) * 200.0f * float(province_pop) / owner_total_pop) : float(base_cost_temp), 1.0f, float(base_cost_temp));
			});
		} else {
			nations::for_each_province(ws, si, [&running_total](provinces::province_tag) {
				running_total += 2.8f;
			});
		}
		return running_total;
	}
}
