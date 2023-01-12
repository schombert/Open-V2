#pragma once
#include "common\common.h"
#include "military\military.h"
#include <random>

namespace military {

	template<typename world_state_t>
	graphics::texture_tag internal_get_leader_picture(world_state_t const& ws, cultures::culture_tag culture, bool is_general) {
		auto& local_generator = get_local_generator();
		auto cgroup = ws.get<culture::group>(culture);
		auto const lp = ws.get<culture_group::leader_pictures>(cgroup);

		if(is_general) {
			auto const mx = int32_t(lp.general_size) - 1;
			if(mx >= 0) {
				std::uniform_int_distribution<int32_t> r(0, mx);
				return ws.get<cultures::leader_pictures>(lp.general_offset + r(local_generator));
			}
		} else {
			auto const mx = int32_t(lp.admiral_size) - 1;
			if(mx >= 0) {
				std::uniform_int_distribution<int32_t> r(0, mx);
				return ws.get<cultures::leader_pictures>(lp.admiral_offset + r(local_generator));
			}
		}
		return graphics::texture_tag();
	}

	struct names_result {
		vector_backed_string<char16_t> first;
		vector_backed_string<char16_t> last;
	};

	template<typename world_state_t>
	names_result internal_get_leader_name(world_state_t const& ws, cultures::culture_tag culture) {
		auto& local_generator = get_local_generator();

		auto first_name_range = ws.get_row<culture::first_names>(culture);
		auto first_name_sz = ws.size<culture::first_names>(culture);
		auto last_name_range = ws.get_row<culture::last_names>(culture);
		auto last_name_sz = ws.size<culture::first_names>(culture);

		names_result result;

		if(auto const mx_fn = first_name_sz - 1; mx_fn >= 0) {
			std::uniform_int_distribution<int32_t> fn(0, mx_fn);
			result.first = first_name_range[fn(local_generator)];
		}

		if(auto const mx_ln = last_name_sz - 1; mx_ln >= 0) {
			std::uniform_int_distribution<int32_t> ln(0, mx_ln);
			result.last = last_name_range[ln(local_generator)];
		}

		return result;
	}

	template<typename world_state_t,
		leader_tag (*new_leader_fn)(world_state_t&),
		graphics::texture_tag (*picture_function)(world_state_t const&, cultures::culture_tag, bool),
		names_result (*names_function)(world_state_t const&, cultures::culture_tag culture)>
	RELEASE_INLINE leader_tag internal_make_empty_leader(world_state_t& ws, cultures::culture_tag culture, bool is_general) {
		
		leader_tag new_leader = new_leader_fn(ws);

		ws.set<military_leader::portrait>(new_leader, picture_function(ws, culture, is_general));

		auto const nr = names_function(ws, culture);

		ws.set<military_leader::first_name>(new_leader, nr.first);
		ws.set<military_leader::last_name>(new_leader, nr.last);
		ws.set<military_leader::is_general>(new_leader, is_general);

		return new_leader;
	}

	template<
		typename world_state_t,
		leader_tag(*make_empty_leader_fn)(world_state_t&, cultures::culture_tag, bool),
		void(*calculate_leader_traits_fn)(world_state_t&, leader_tag)>
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
			ws.get_row<military::leader_trait_values>(ws.get<military_leader::personality>(l)).data());

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

	template<typename world_state_t, army_tag(*new_army_function)(world_state_t&)>
	RELEASE_INLINE army_tag internal_make_army(world_state_t& ws, nations::country_tag n, provinces::province_tag location) {
		army_tag new_army = new_army_function(ws);
		if(location) {
			ws.set<army::location>(new_army, location);
			ws.add_item(ws.get<province_state::armies>(location), new_army);
		}
		ws.set<army::owner>(new_army, n);
		ws.add_item(ws.get<nation::armies>(n), new_army);

		ws.set<army::readiness>(new_army, 1.0f);
		ws.set<army::supply>(new_army, 1.0f);

		auto target_hq = ws.get<province_state::strat_hq>(location);
		if(target_hq) {
			ws.set<army::hq>(new_army, target_hq);
			ws.add_item(ws.get<strategic_hq::army_set>(target_hq), new_army);
		}
		
		return new_army;
	}

	template<typename world_state_t, fleet_tag(*new_fleet_function)(world_state_t&)>
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

	template<typename world_state_t, bool(*check_war_tag_fn)(world_state_t const&, war_tag)>
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

	template<typename world_state_t,
		bool(*test_trigger_fn)(world_state_t const&, triggers::trigger_tag, triggers::const_parameter, triggers::const_parameter, triggers::const_parameter)>
	RELEASE_INLINE bool internal_can_use_cb_against(world_state_t const& ws, nations::country_tag nation_by, nations::country_tag nation_target) {
		auto pending_range = ws.get_range(ws.get<nation::active_cbs>(nation_by));
		for(auto& c : pending_range) {
			if(c.target == nation_target)
				return true;
		}
		if(ws.any_of<cb_type_tag>([&ws, nation_target, nation_by](cb_type_tag c) {
			return (ws.get<::cb_type::flags>(c) & cb_type::is_not_triggered_only) == 0
				&& is_valid_index(ws.get<::cb_type::can_use>(c))
				&&  test_trigger_fn(ws, ws.get<::cb_type::can_use>(c), nation_target, nation_by, triggers::const_parameter());
		}))
			return true;
		auto vassal_range = ws.get_range(ws.get<nation::vassals>(nation_target));
		for(auto v : vassal_range) {
			if(internal_can_use_cb_against<world_state_t, test_trigger_fn>(ws, nation_by, nation_target))
				return true;
		}
		return false;
	}

	template<typename world_state_t,
		bool(*owns_releasable_core_fn)(world_state_t const&, nations::country_tag),
		bool(*test_trigger_fn)(world_state_t const&, triggers::trigger_tag, triggers::const_parameter, triggers::const_parameter, triggers::const_parameter)>
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

	template<typename world_state_t,
		bool(*test_trigger_fn)(world_state_t const&, triggers::trigger_tag, triggers::const_parameter, triggers::const_parameter, triggers::const_parameter),
		bool (*test_cb_conditions_fn)(world_state_t const&, nations::country_tag, nations::country_tag, cb_type_tag)>
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

	template<typename world_state_t,
		bool(*nation_exists_fn)(world_state_t const&, nations::country_tag),
		nations::country_tag(*get_player_fn)(world_state_t const&),
		bool(*test_trigger_fn)(world_state_t const&, triggers::trigger_tag, triggers::const_parameter, triggers::const_parameter, triggers::const_parameter)>
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

	template<typename world_state_t,
		bool(*nation_exists_fn)(world_state_t const&, nations::country_tag),
		nations::country_tag(*get_player_fn)(world_state_t const&),
		bool(*test_trigger_fn)(world_state_t const&, triggers::trigger_tag, triggers::const_parameter, triggers::const_parameter, triggers::const_parameter),
		void (*post_msg_fn)(bool, world_state_t& ws, nations::country_tag, nations::country_tag, cb_type_tag)>
	RELEASE_INLINE void internal_update_player_cb_state(world_state_t& ws) {
		if(!get_player_fn(ws))
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

	template<typename world_state_t,
		float (*regiment_sz_fn)(world_state_t const& ws)>
	RELEASE_INLINE uint32_t internal_total_units_in_province(world_state_t const& ws, provinces::province_tag ps) {
		auto orders = ws.get<province_state::orders>(ps);
		if(orders) {
			float total = 0;
			const float regiment_size = regiment_sz_fn(ws);

			auto province_size = ws.get_size(ws.get<army_order::province_set>(orders));
			auto involved_range = ws.get_range(ws.get<army_order::army_set>(orders));

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

	template<typename world_state_t,
		float(*regiment_sz_fn)(world_state_t const& ws)>
	RELEASE_INLINE uint32_t internal_total_active_divisions(world_state_t const& ws, nations::country_tag this_nation) {
		float total = 0ui32;
		const float regiment_size = regiment_sz_fn(ws);

		auto army_range = ws.get_range(ws.get<nation::armies>(this_nation));
		for(auto a : army_range) {
			total += ws.get<army::current_soldiers>(a);
		}
		return uint32_t(total / regiment_size + 0.5f);
	}

	template<typename world_state_t>
	RELEASE_INLINE uint32_t internal_total_active_ships(world_state_t const& ws, nations::country_tag this_nation) {
		float total = 0;
		auto fleet_range = ws.get_range(ws.get<nation::fleets>(this_nation));
		for(auto f : fleet_range) {
			total += ws.get<fleet::size>(f);
		}
		return uint32_t(total + 0.5f);
	}

	template<typename world_state_t,
		population::demo_tag (*soldier_demo_index)(world_state_t const&)>
	RELEASE_INLINE float internal_recruited_pop_fraction(world_state_t const& ws, nations::country_tag this_nation) {
		if(this_nation) {
			float total_soldier_pops =
				ws.get<nation::demographics>(this_nation, soldier_demo_index(ws))
				+ ws.get<nation::colonial_demographics>(this_nation, soldier_demo_index(ws));
			float total_soldier_pops_assigned = 0;

			if(total_soldier_pops <= 0)
				return 0.0f;

			auto army_range = ws.get_range(ws.get<nation::armies>(this_nation));
			for(auto a : army_range) {
				total_soldier_pops_assigned += ws.get<army::current_soldiers>(a);
			}

			return total_soldier_pops_assigned / total_soldier_pops;
		} else {
			return 0.0f;
		}
	}

	template<typename world_state_t,
		void (*msg_function)(world_state_t&, war_tag, nations::country_tag)>
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

	template<typename world_state_t,
		void (*msg_function)(world_state_t&, war_tag, bool, nations::country_tag)>
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
			auto attacker_range = ws.get_range(ws.get<war::attackers>(this_war));
			for(auto a : attacker_range) {
				ws.add_item(opponents_in_war, a);
				ws.add_item(ws.get<nation::opponents_in_war>(a), to_add);
			}
			auto defender_range = ws.get_range(ws.get<war::defenders>(this_war));
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

	template<typename world_state_t,
		void (*release_army_fn)(world_state_t&, army_tag),
		bool remove_from_nation_arrays>
		RELEASE_INLINE void internal_destroy_army(world_state_t& ws, army_tag a) {
		if constexpr(remove_from_nation_arrays) {
			auto const owner = ws.get<army::owner>(a);
			ws.remove_item(ws.get<nation::armies>(owner), a);
		}

		if(auto o = ws.get<army::order>(a); o) {
			ws.remove_item(ws.get<army_order::army_set>(o), a);
		}
		if(auto l = ws.get<army::location>(a); l) {
			ws.remove_item(ws.get<province_state::armies>(l), a);
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

	template<typename world_state_t,
		void(*release_fleet_fn)(world_state_t&, fleet_tag),
		bool remove_from_nation_arrays>
	RELEASE_INLINE void internal_destroy_fleet(world_state_t& ws, fleet_tag f, nations::country_tag owner) {
		if constexpr(remove_from_nation_arrays) {
			ws.remove_item(ws.get<nation::fleets>(owner), f);
		}

		if(auto l = ws.get<fleet::leader>(f); l) {
			ws.set<military_leader::is_attached>(l, false);
		}

		release_fleet_fn(ws, f);
	}
	

	template<typename world_state_t,
		void(*release_orders_fn)(world_state_t&, army_orders_tag),
		bool remove_from_nation_arrays>
	RELEASE_INLINE void internal_destroy_orders(world_state_t& ws, army_orders_tag o, nations::country_tag owner) {
		
		auto army_range = ws.get_range(ws.get<army_order::army_set>(o));
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

	template<typename world_state_t,
		void (*release_hq_fn)(world_state_t&, strategic_hq_tag),
		void (*destroy_army_fn)(world_state_t&, army_tag),
		bool remove_from_nation_arrays>
	RELEASE_INLINE void internal_destroy_hq(world_state_t& ws, strategic_hq_tag o, nations::country_tag owner) {
		auto army_range = ws.get_range(ws.get<strategic_hq::army_set>(o));
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

	template<typename world_state_t,
		void(*release_leader_fn)(world_state_t&, leader_tag)>
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

		release_leader_fn(ws, l);
	}

	template<typename world_state_t,
		void(*release_leader_fn)(world_state_t&, leader_tag)>
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
		release_leader_fn(ws, l);
	}

	template<typename world_state_t,
		void(*release_war_fn)(world_state_t&, war_tag)>
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

		release_war_fn(ws, this_war);
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
	
	template<typename world_state_t,
		war_tag (*allocate_war_fn)(world_state_t&),
		date_tag(*date_fn)(world_state_t const&)>
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

	template<typename world_state_t,
		int32_t(*count_factories_fn)(world_state_t const&, nations::state_tag),
		bool(*state_is_colonial_fn)(world_state_t const&, nations::state_tag)>
	RELEASE_INLINE float internal_single_state_war_score(world_state_t const& ws, nations::state_tag si, float owner_total_pop) {
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

	template<typename world_state_t,
		modifiers::defines const& (*get_setting_struct)(world_state_t const&),
		bool(*nation_exists_fn)(world_state_t const&, nations::country_tag),
		float (*single_state_value_fn)(world_state_t const&, nations::state_tag, float),
		bool(*test_trigger_fn)(world_state_t const&, triggers::trigger_tag, triggers::const_parameter, triggers::const_parameter, triggers::const_parameter)>
	RELEASE_INLINE float internal_calculate_base_war_score_cost(world_state_t const& ws, war_goal const& wg) {
		auto const this_cb_type = wg.cb_type;
		auto const cb_flags = ws.get<::cb_type::flags>(this_cb_type);

		if((cb_flags & cb_type::great_war_obligatory) != 0)
			return 0.0f;

		float total_cost = 0.0f;

		auto const& ss = get_setting_struct(ws);

		if((cb_flags & cb_type::po_gunboat) != 0)
			total_cost += ss.peace_cost_gunboat;
		if((cb_flags & cb_type::po_add_to_sphere) != 0)
			total_cost += ss.peace_cost_add_to_sphere;
		if((cb_flags & cb_type::po_release_puppet) != 0)
			total_cost += ss.peace_cost_release_puppet;
		if((cb_flags & cb_type::po_colony) != 0)
			total_cost += ss.peace_cost_colony;
		if((cb_flags & cb_type::po_disarmament) != 0)
			total_cost += ss.peace_cost_disarmament;
		if((cb_flags & cb_type::po_destroy_forts) != 0)
			total_cost += ss.peace_cost_destroy_forts;
		if((cb_flags & cb_type::po_destroy_naval_bases) != 0)
			total_cost += ss.peace_cost_destroy_naval_bases;
		if((cb_flags & cb_type::po_reparations) != 0)
			total_cost += ss.peace_cost_reparations;
		if((cb_flags & cb_type::po_remove_prestige) != 0)
			total_cost += ss.peace_cost_prestige;
		if((cb_flags & cb_type::po_status_quo) != 0)
			total_cost += ss.peace_cost_status_quo;
		if((cb_flags & cb_type::po_annex) != 0)
			total_cost += ss.peace_cost_annex;
		if((cb_flags & cb_type::po_install_communist_gov_type) != 0)
			total_cost += ss.peace_cost_install_communist_gov_type;
		if((cb_flags & cb_type::po_uninstall_communist_gov_type) != 0)
			total_cost += ss.peace_cost_uninstall_communist_gov_type;
		if((cb_flags & cb_type::po_clear_union_sphere) != 0)
			total_cost += ss.peace_cost_clear_union_sphere * 4;

		// sum province values -- if all allowed states -- find all matching states to sum

		if((cb_flags & (cb_type::po_transfer_provinces | cb_type::po_demand_state)) != 0) {
			if(auto target = wg.target_country; nation_exists_fn(ws, target)) {
				auto target_total_pop = ws.get<nation::demographics>(target, population::total_population_tag);

				if((cb_flags & cb_type::all_allowed_states) == 0) {
					if(auto state = wg.target_state; state)
						total_cost += single_state_value_fn(ws, state, target_total_pop);
				} else {
					auto wg_from = wg.from_country;
					auto wg_liberation = wg.liberation_target;
					auto allowed_states = ws.get<::cb_type::allowed_states>(this_cb_type);
					if(is_valid_index(wg_from) && is_valid_index(allowed_states) && is_valid_index(wg_liberation)) {
						nations::for_each_state(ws, target, [&ws, wg_from, wg_liberation, allowed_states, tp = target_total_pop, &total_cost](nations::state_tag si) {
							if(test_trigger_fn(ws, allowed_states, si, wg_from, wg_liberation))
								total_cost += single_state_value_fn(ws, si, tp);
						});
					}
				}
			}
		}

		return std::clamp(total_cost * ws.get<::cb_type::peace_cost_factor>(this_cb_type) / 100.0f, 0.0f, 1.0f);
	}

	template<typename world_state_t,
		float (*ws_cost_fn)(world_state_t const&, war_goal const&)>
	RELEASE_INLINE float internal_total_attacker_demands_war_score(world_state_t const& ws, war_tag w) {
		float sum = 0.0f;
		auto wg_range = ws.get_range(ws.get<war::war_goals>(w));
		for(auto& i : wg_range) {
			if(ws.contains_item(ws.get<war::attackers>(w), i.from_country))
				sum += ws_cost_fn(ws, i);
		}
		return sum;
	}

	template<typename world_state_t,
		float(*ws_cost_fn)(world_state_t const&, war_goal const&)>
	RELEASE_INLINE float internal_total_defender_demands_war_score(world_state_t const& ws, war_tag w) {
		float sum = 0.0f;
		auto wg_range = ws.get_range(ws.get<war::war_goals>(w));
		for(auto& i : wg_range) {
			if(ws.contains_item(ws.get<war::defenders>(w), i.from_country))
				sum += ws_cost_fn(ws, i);
		}
		return sum;
	}

	template<typename world_state_t>
	RELEASE_INLINE float internal_base_cb_infamy(world_state_t const& ws, cb_type_tag cb) {
		auto const flags = ws.get<::cb_type::flags>(cb);

		float total = 0.0f;
		if((flags & cb_type::po_annex) != 0) {
			total += 10.0f;
		}
		if((flags & cb_type::po_demand_state) != 0) {
			total += 5.0f;
		}
		if((flags & cb_type::po_add_to_sphere) != 0) {
			total += 2.0f;
		}
		if((flags & cb_type::po_disarmament) != 0) {
			total += 5.0f;
		}
		if((flags & cb_type::po_reparations) != 0) {
			total += 5.0f;
		}
		if((flags & cb_type::po_transfer_provinces) != 0) {
			total += 5.0f;
		}
		if((flags & cb_type::po_remove_prestige) != 0) {
			total += 2.0f;
		}
		if((flags & cb_type::po_make_puppet) != 0) {
			total += 5.0f;
		}
		if((flags & cb_type::po_release_puppet) != 0) {
			total += 0.5f;
		}
		if((flags & cb_type::po_install_communist_gov_type) != 0) {
			total += 5.0f;
		}
		if((flags & cb_type::po_uninstall_communist_gov_type) != 0) {
			total += 5.0f;
		}
		if((flags & cb_type::po_destroy_forts) != 0) {
			total += 2.0f;
		}
		if((flags & cb_type::po_destroy_naval_bases) != 0) {
			total += 2.0f;
		}

		return total * ws.get<::cb_type::badboy_factor>(cb);
	}


	template<typename world_state_t,
		modifiers::defines const& (*get_setting_struct)(world_state_t const&)>
	RELEASE_INLINE float internal_daily_cb_progress(world_state_t const& ws, nations::country_tag n, cb_type_tag type) {
		auto const& ss = get_setting_struct(ws);
		const auto nat_mod =
			ve::load(n, ws.get_row<nation::cb_generation_speed_modifier>());

		const auto adjusted_mod = nat_mod + 1.0f;
		const auto cb_speed = ve::load(type, ws.get_row<::cb_type::construction_speed_direct>());
		const auto base_speed = cb_speed * ss.cb_generation_base_speed * 0.01f;
		//const auto old_value = ve::load(n, ws.get_row<nation::cb_construction_progress>());

		return adjusted_mod * base_speed;
	}

	template<typename world_state_t,
		modifiers::defines const& (*get_setting_struct)(world_state_t const&),
		bool (*cb_validity_fn)(world_state_t const&, cb_type_tag, nations::country_tag, nations::country_tag),
		void(*cb_acquired_msg)(world_state_t&, nations::country_tag, nations::country_tag, cb_type_tag),
		void(*cb_detected_msg)(world_state_t&, nations::country_tag, nations::country_tag, cb_type_tag, float),
		void(*cb_invalid_msg)(world_state_t&, nations::country_tag, nations::country_tag, cb_type_tag)>
	RELEASE_INLINE void internal_update_cb_construction(world_state_t& ws) {
		ve::execute_serial<nations::country_tag>(ve::to_vector_size(ws.size<nation::name>()), [
			&ws,
			increment = get_setting_struct(ws).cb_generation_base_speed * 0.01f,
			mod_row = ws.get_row<nation::cb_generation_speed_modifier>(),
			cb_type_row = ws.get_row<nation::cb_construction_type>(),
			progress_row = ws.get_row<nation::cb_construction_progress>()
		](auto off) {
				const auto nat_mod = ve::load(off, mod_row);
				const auto cb_type = ve::load(off, cb_type_row);

				const auto adjusted_mod = nat_mod + 1.0f;
				const auto cb_speed = ve::load(cb_type, ws.get_row<::cb_type::construction_speed_direct>());
				const auto base_speed = cb_speed * increment;
				const auto old_value = ve::load(off, progress_row);

				ve::store(off, progress_row, ve::multiply_and_add(adjusted_mod, base_speed, old_value));
		});

		auto const base_detection_chance = int32_t(get_setting_struct(ws).cb_detection_chance_base);
		ws.par_for_each<nations::country_tag>([&ws, base_detection_chance](nations::country_tag n) {
			auto const cb_type_id = ws.get<nation::cb_construction_type>(n);
			auto const cb_target = ws.get<nation::cb_construction_target>(n);
			if(cb_target && cb_type_id) {
				if(ws.get<nation::cb_construction_progress>(n) >= 1.0f) {
					if(cb_validity_fn(ws, cb_type_id, n, cb_target)) {
						// is valid: add
						auto& cb_array = ws.get<nation::active_cbs>(n);
						ws.add_item(cb_array, pending_cb{ cb_target, cb_type_id, date_tag() });

						cb_acquired_msg(ws, n, cb_target, cb_type_id);
					} else {
						// not valid: post invalid message
						cb_invalid_msg(ws, n, cb_target, cb_type_id);
					}
					// either case: erase
					ws.set<nation::cb_construction_type>(n, cb_type_tag());
					ws.set<nation::cb_construction_target>(n, nations::country_tag());
					ws.set<nation::cb_construction_discovered>(n, false);
					ws.set<nation::cb_construction_progress>(n, 0.0f);
				} else {
					// check for discovery & then validity
					if(ws.get<nation::cb_construction_discovered>(n) == false) {
						std::uniform_int_distribution<int32_t> dist{ 0, 1'000 };
						auto const chance = dist(get_local_generator());
						auto const defines_probability = base_detection_chance;

						if(chance <= defines_probability) {
							if(cb_validity_fn(ws, cb_type_id, n, cb_target)) {
								ws.set<nation::cb_construction_discovered>(n, true);
								auto const infamy_gain = base_cb_infamy(ws, cb_type_id) * (
									1.0f - ws.get<nation::cb_construction_progress>(n));
								ws.get<nation::infamy>(n) += infamy_gain;

								cb_detected_msg(ws, n, cb_target, cb_type_id, infamy_gain);
							} else {
								// not valid, erase and post invalid message
								ws.set<nation::cb_construction_type>(n, cb_type_tag());
								ws.set<nation::cb_construction_target>(n, nations::country_tag());
								ws.set<nation::cb_construction_discovered>(n, false);
								ws.set<nation::cb_construction_progress>(n, 0.0f);

								cb_invalid_msg(ws, n, cb_target, cb_type_id);
							}
						}
					}
				}
			}
		});
	}


	template<typename worldstate_t>
	auto internal_change_army_location(worldstate_t& ws, army_tag t, provinces::province_tag new_location) -> void {
		auto old_location = ws.get<army::location>(t);
		if(old_location == new_location)
			return;

		if(old_location)
			ws.remove_item(ws.get<province_state::armies>(old_location), t);
		ws.set<army::location>(t, new_location);
		ws.add_item(ws.get<province_state::armies>(new_location), t);
	}
}
