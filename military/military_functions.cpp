#include "common\\common.h"
#include "military_functions.h"
#include "world_state\\world_state.h"
#include <random>
#include "nations\\nations_functions.hpp"
#include "provinces\\province_functions.h"
#include "economy\\economy_functions.h"
#include "world_state\\messages.h"
#include "military_internals.hpp"

namespace military {
	void init_military_state(world_state& ws) {
		
	}

	void reset_state(military_state& s) {
		s.leader_arrays.reset();
		s.army_arrays.reset();
		s.orders_arrays.reset();
		s.fleet_arrays.reset();
		s.war_arrays.reset();
		s.war_goal_arrays.reset();
		s.fleet_presence_arrays.reset();
		s.naval_control_arrays.reset();
		s.cb_arrays.reset();
	}

	inline leader_tag new_leader_fn(world_state& ws) {
		return ws.w.military_s.leaders.get_new();
	}

	leader_tag make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general) {
		return internal_make_empty_leader<new_leader_fn>(ws, culture, is_general);
	}

	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date) {
		return internal_make_auto_leader<make_empty_leader, calculate_leader_traits>(ws, culture, is_general, creation_date);
	}

	void calculate_leader_traits(world_state& ws, leader_tag l) {
		internal_calculate_leader_traits(ws, l);
	}

	inline army_tag new_army_fn(world_state& ws) {
		return ws.w.military_s.armies.get_new();
	}

	army_tag make_army(world_state& ws, nations::country_tag n, provinces::province_tag location) {
		return internal_make_army<new_army_fn>(ws, n, location);
	}

	inline fleet_tag new_fleet_fn(world_state& ws) {
		return ws.w.military_s.fleets.get_new();
	}


	fleet_tag make_fleet(world_state& ws, nations::country_tag n, provinces::province_tag location) {
		return internal_make_fleet<new_fleet_fn>(ws, n, location);
	}

	bool in_war_with(world_state const& ws, nations::country_tag this_nation, nations::country_tag nation_with) {
		return internal_in_war_with(ws, this_nation, nation_with);
	}

	bool in_war_against(world_state const& ws, nations::country_tag this_nation, nations::country_tag nation_against) {
		return internal_in_war_against(ws, this_nation, nation_against);
	}

	inline bool check_war_tag(world_state const& ws, war_tag wid) {
		return ws.w.military_s.wars.is_valid_index(wid);
	}

	void update_at_war_with_and_against(world_state& ws, nations::country_tag this_nation) {
		internal_update_at_war_with_and_against<check_war_tag>(ws, this_nation);
	}

	inline bool test_cb_trigger(world_state const& ws, triggers::trigger_tag can_use, nations::country_tag nation_target, nations::country_tag nation_by) {
		return is_valid_index(can_use)
			&& triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(can_use), ws, nation_target, nation_by, triggers::const_parameter());
	}

	bool can_use_cb_against(world_state const& ws, nations::country_tag nation_by, nations::country_tag nation_target) {
		return internal_can_use_cb_against<test_cb_trigger>(ws, nation_by, nation_target);
	}

	inline bool generic_test_trigger(world_state const& ws, triggers::trigger_tag t, triggers::const_parameter a, triggers::const_parameter b, triggers::const_parameter c) {
		return triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(t), ws, a, b, c);
	}

	bool is_cb_construction_valid_against(world_state const& ws, cb_type_tag cb, nations::country_tag nation_by, nations::country_tag nation_target) {
		auto& c = ws.s.military_m.cb_types[cb];
		if((c.flags & (cb_type::is_not_triggered_only | cb_type::is_not_constructing_cb)) != 0)
			return false;
		if(is_valid_index(c.allowed_substate_regions)) {
			return false;
		}
		if(is_valid_index(c.can_use)) {
			if(!triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(c.can_use), ws, nation_target, nation_by, triggers::const_parameter()))
				return false;
		}
		
		return internal_test_cb_conditions<generic_test_trigger, nations::owns_releasable_core>(ws, nation_by, nation_target, cb);
	}

	void init_player_cb_state(world_state& ws) {
		for(auto const& c : ws.s.military_m.cb_types) {
			if((c.flags & cb_type::is_not_triggered_only) != 0) {
				ws.w.local_player_data.triggered_cb_state[c.id].resize(ws.w.nation_s.nations.size());

				ws.w.nation_s.nations.for_each([&ws, cb_id = c.id, can_use = c.can_use](nations::country_tag n) {
					if(nations::nation_exists(ws, n) && n != ws.w.local_player_nation) {
						ws.w.local_player_data.triggered_cb_state[cb_id].set(n, !is_valid_index(can_use) || triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(can_use), ws, n, ws.w.local_player_nation, triggers::const_parameter()));
					}
				});
			}
		}
	}
	void update_player_cb_state(world_state& ws) {
		if(!is_valid_index(ws.w.local_player_nation))
			return;

		concurrency::parallel_for_each(ws.s.military_m.cb_types.begin(), ws.s.military_m.cb_types.end(), [&ws](military::cb_type const& c){
			if((c.flags & cb_type::is_not_triggered_only) != 0) {
				ws.w.local_player_data.triggered_cb_state[c.id].resize(ws.w.nation_s.nations.size());

				ws.w.nation_s.nations.for_each([&ws, cb_id = c.id, can_use = c.can_use](nations::country_tag n) {
					const auto old_state = ws.w.local_player_data.triggered_cb_state[cb_id][n];
					if(nations::nation_exists(ws, n) && n != ws.w.local_player_nation) {
						auto const new_state = !is_valid_index(can_use) || triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(can_use), ws, n, ws.w.local_player_nation, triggers::const_parameter());
						ws.w.local_player_data.triggered_cb_state[cb_id].set(n, new_state);

						if(new_state != old_state) {
							if(new_state) 
								messages::acquired_cb(ws, ws.w.local_player_nation, n, cb_id);
							else 
								messages::lost_cb(ws, ws.w.local_player_nation, n, cb_id);
						}
					}
				});
			}
		});
	}

	bool has_units_in_province(world_state const& ws, nations::country_tag this_nation, provinces::province_tag ps) {
		auto orders = ws.w.province_s.province_state_container.get<province_state::orders>(ps);
		if(is_valid_index(orders)) {
			auto involved_range = get_range(ws.w.military_s.army_arrays, ws.w.military_s.army_orders_container[orders].involved_armies);
			for(auto a : involved_range) {
				if(ws.w.military_s.armies[a].owner == this_nation)
					return true;
			}
		}
		return false;
	}

	uint32_t total_units_in_province(world_state const& ws, provinces::province_tag ps) {
		auto orders = ws.w.province_s.province_state_container.get<province_state::orders>(ps);
		if(is_valid_index(orders)) {
			auto& o_obj = ws.w.military_s.army_orders_container[orders];

			uint32_t total = 0ui32;
			const uint32_t regiment_size = uint32_t(ws.s.modifiers_m.global_defines.pop_size_per_regiment);

			auto province_size = get_size(ws.w.province_s.province_arrays, o_obj.involved_provinces);
			auto involved_range = get_range(ws.w.military_s.army_arrays, o_obj.involved_armies);

			if(province_size == 0)
				return 0ui32;

			for(auto a : involved_range) {
				total += ws.w.military_s.armies[a].total_soldiers;
			}

			return (total + regiment_size * province_size - 1ui32) / (regiment_size * province_size);
		} else {
			return 0ui32;
		}
	}

	uint32_t total_active_divisions(world_state const& ws, nations::country_tag this_nation) {
		uint32_t total = 0ui32;
		const uint32_t regiment_size = uint32_t(ws.s.modifiers_m.global_defines.pop_size_per_regiment);

		auto army_range = get_range(ws.w.military_s.army_arrays, ws.w.nation_s.nations.get<nation::armies>(this_nation));
		for(auto a : army_range) {
			if(is_valid_index(a)) {
				total += ws.w.military_s.armies[a].total_soldiers ;
			}
		}
		return (total + regiment_size - 1ui32) / regiment_size;
	}
	uint32_t total_active_ships(world_state const& ws, nations::country_tag this_nation) {
		uint32_t total = 0ui32;
		auto fleet_range = get_range(ws.w.military_s.fleet_arrays, ws.w.nation_s.nations.get<nation::fleets>(this_nation));
		for(auto f : fleet_range) {
			if(is_valid_index(f))
				total += get_size(ws.w.military_s.ship_arrays, ws.w.military_s.fleets[f].ships);
		}
		return total;
	}
	float recruited_pop_fraction(world_state const& ws, nations::country_tag this_nation) {
		float total_soldier_pops = 0;
		float total_soldier_pops_assigned = 0;

		nations::for_each_pop(ws, this_nation, [&ws, &total_soldier_pops, &total_soldier_pops_assigned](population::pop_tag p) {
			if(ws.w.population_s.pops.get<pop::type>(p) == ws.s.population_m.soldier) {
				++total_soldier_pops;
				total_soldier_pops_assigned += is_valid_index(ws.w.population_s.pops.get<pop::associated_army>(p)) ? 1.0f : 0.0f;
			}
		});
		
		if(total_soldier_pops == 0)
			return 0.0f;
		else
			return float(total_soldier_pops_assigned) / float(total_soldier_pops);
	}

	bool has_named_leader(world_state const&, nations::country_tag, text_data::text_tag) {
		return true;
	}

	float get_warscore(world_state const&, war const& wr) {
		return wr.current_war_score;
	}


	void silent_remove_from_war(world_state& ws, war& this_war, nations::country_tag to_remove) {
		remove_item_if(ws.w.military_s.war_goal_arrays, this_war.war_goals,
			[to_remove](war_goal const& wg) { return wg.from_country == to_remove || wg.target_country == to_remove; });
		remove_item(ws.w.nation_s.nations_arrays, this_war.attackers, to_remove);
		remove_item(ws.w.nation_s.nations_arrays, this_war.defenders, to_remove);

		if(this_war.primary_attacker == to_remove) {
			nations::country_tag best = nations::country_tag();
			int16_t best_rank = std::numeric_limits<int16_t>::max();

			auto attackers = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
			for(auto n : attackers) {
				auto rank = ws.w.nation_s.nations.get<nation::overall_rank>(n);
				if(rank < best_rank) {
					best_rank = rank;
					best = n;
				}
			}

			this_war.primary_attacker = best;
		} else if(this_war.primary_defender == to_remove) {
			nations::country_tag best = nations::country_tag();
			int16_t best_rank = std::numeric_limits<int16_t>::max();

			auto defenders = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto n : defenders) {
				auto rank = ws.w.nation_s.nations.get<nation::overall_rank>(n);
				if(rank < best_rank) {
					best_rank = rank;
					best = n;
				}
			}

			this_war.primary_defender = best;
		}
	}

	void remove_from_war(world_state& ws, war& this_war, nations::country_tag to_remove) {
		silent_remove_from_war(ws, this_war, to_remove);
		remove_item(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(to_remove), war_identifier{ this_war.id, false });
	}
	void add_to_war(world_state& ws, war& this_war, bool attacker, nations::country_tag to_add) {
		auto& allies_in_war = ws.w.nation_s.nations.get<nation::allies_in_war>(to_add);
		auto& opponents_in_war = ws.w.nation_s.nations.get<nation::opponents_in_war>(to_add);

		if(attacker) {
			auto attacker_range = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
			for(auto a : attacker_range) {
				add_item(ws.w.nation_s.nations_arrays, allies_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies_in_war>(a), to_add);
			}
			auto defender_range = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto a : defender_range) {
				add_item(ws.w.nation_s.nations_arrays, opponents_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::opponents_in_war>(a), to_add);
			}
			add_item(ws.w.nation_s.nations_arrays, this_war.attackers, to_add);
			add_item(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(to_add), war_identifier{ this_war.id, true });
		} else {
			auto attacker_range = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
			for(auto a : attacker_range) {
				add_item(ws.w.nation_s.nations_arrays, opponents_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::opponents_in_war>(a), to_add);
			}
			auto defender_range = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto a : defender_range) {
				add_item(ws.w.nation_s.nations_arrays, allies_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies_in_war>(a), to_add);
			}
			add_item(ws.w.nation_s.nations_arrays, this_war.defenders, to_add);
			add_item(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(to_add), war_identifier{ this_war.id, false });
		}
	}

	bool is_target_of_war_goal(world_state const& ws, war const& this_war, nations::country_tag target) {
		auto wg_range = get_range(ws.w.military_s.war_goal_arrays, this_war.war_goals);
		for(auto& wg : wg_range) {
			if(wg.target_country == target)
				return true;
		}
		return false;
	}

	void destroy_army(world_state& ws, army& a, nations::country_tag owner) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, a.backing_pops);
		for(auto p : pop_range)
			ws.w.population_s.pops.set<pop::associated_army>(p, army_tag());
		clear(ws.w.population_s.pop_arrays, a.backing_pops);

		remove_item(ws.w.military_s.army_arrays, ws.w.nation_s.nations.get<nation::armies>(owner), a.id);
		if(a.current_orders) {
			remove_item(ws.w.military_s.army_arrays, a.current_orders->involved_armies, a.id);
			a.current_orders = nullptr;
		}
		if(a.leader) {
			a.leader->attached = false;
			a.leader = nullptr;
		}
	}

	void destroy_fleet(world_state& ws, fleet& f, nations::country_tag owner) {
		clear(ws.w.military_s.ship_arrays, f.ships);
		remove_item(ws.w.military_s.fleet_arrays, ws.w.nation_s.nations.get<nation::fleets>(owner), f.id);

		if(f.leader) {
			f.leader->attached = false;
			f.leader = nullptr;
		}
	}

	void partial_destroy_fleet(world_state& ws, fleet& f) {
		clear(ws.w.military_s.ship_arrays, f.ships);

		if(f.leader) {
			f.leader->attached = false;
			f.leader = nullptr;
		}
	}

	void partial_destroy_army(world_state& ws, army& a) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, a.backing_pops);
		for(auto p : pop_range)
			ws.w.population_s.pops.set<pop::associated_army>(p, army_tag());
		clear(ws.w.population_s.pop_arrays, a.backing_pops);

		if(a.leader) {
			a.leader->attached = false;
			a.leader = nullptr;
		}
	}

	void destroy_orders(world_state& ws, army_orders& o, nations::country_tag owner) {
		if(o.leader) {
			o.leader->attached = false;
			o.leader = nullptr;
		}
		auto army_range = get_range(ws.w.military_s.army_arrays, o.involved_armies);
		for(auto a : army_range) 
			ws.w.military_s.armies[a].current_orders = nullptr;
		clear(ws.w.military_s.army_arrays, o.involved_armies);

		remove_item(ws.w.military_s.orders_arrays, ws.w.nation_s.nations.get<nation::active_orders>(owner), o.id);

		auto prange = get_range(ws.w.province_s.province_arrays, o.involved_provinces);
		for(auto p : prange)
			ws.w.province_s.province_state_container.set<province_state::orders>(p, military::army_orders_tag());
		clear(ws.w.province_s.province_arrays, o.involved_provinces);
	}

	void partial_destroy_orders(world_state& ws, army_orders& o) {
		if(o.leader) {
			o.leader->attached = false;
			o.leader = nullptr;
		}

		clear(ws.w.military_s.army_arrays, o.involved_armies);

		auto prange = get_range(ws.w.province_s.province_arrays, o.involved_provinces);
		for(auto p : prange)
			ws.w.province_s.province_state_container.set<province_state::orders>(p, military::army_orders_tag());
		clear(ws.w.province_s.province_arrays, o.involved_provinces);
	}

	void destroy_admiral(world_state& ws, military_leader& l, nations::country_tag owner) {
		if(l.attached) {
			auto fr = get_range(ws.w.military_s.fleet_arrays, ws.w.nation_s.nations.get<nation::fleets>(owner));
			for(auto fl : fr) {
				if(ws.w.military_s.fleets[fl].leader == &l)
					ws.w.military_s.fleets[fl].leader = nullptr;
			}
		}
	}

	void destroy_general(world_state& ws, military_leader& l, nations::country_tag owner) {
		if(l.attached) {
			auto ar = get_range(ws.w.military_s.army_arrays, ws.w.nation_s.nations.get<nation::armies>(owner));
			for(auto a : ar) {
				if(ws.w.military_s.armies[a].leader == &l)
					ws.w.military_s.armies[a].leader = nullptr;
			}

			auto order_range = get_range(ws.w.military_s.orders_arrays, ws.w.nation_s.nations.get<nation::active_orders>(owner));
			for(auto o : order_range) {
				if(ws.w.military_s.army_orders_container[o].leader == &l)
					ws.w.military_s.army_orders_container[o].leader = nullptr;
			}
		}
	}

	uint32_t calculate_minimum_soldiers(world_state const& ws, army_tag a) {
		Eigen::Map<const Eigen::Matrix<uint16_t, -1, 1>> composition(ws.w.military_s.unit_type_composition.get_row(a).data(), ws.s.military_m.unit_types_count);
		auto count_brigades = composition.sum();
		return uint32_t(ws.s.modifiers_m.global_defines.pop_size_per_regiment) * count_brigades;
	}

	void rebuild_fleet_presence(world_state&, nations::country_tag) {
		//stub
	}


	void destroy_war(world_state& ws, war& this_war) {
		auto attacker_range = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
		for(auto a : attacker_range)
			remove_item(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(a), military::war_identifier{ this_war.id, false });
		
		auto defender_range = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
		for(auto a : defender_range)
			remove_item(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(a), military::war_identifier{ this_war.id, false });

		clear(ws.w.nation_s.nations_arrays, this_war.attackers);
		clear(ws.w.nation_s.nations_arrays, this_war.defenders);
		clear(ws.w.military_s.naval_control_arrays, this_war.naval_control_set);
		clear(ws.w.military_s.war_goal_arrays, this_war.war_goals);
	}

	war* get_war_between(world_state& ws, nations::country_tag a, nations::country_tag b) {
		auto wars = get_range(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(a));
		for(auto w = wars.first; w != wars.second; ++w) {
			war& this_war = ws.w.military_s.wars[w->war_id];
			if(w->is_attacker) {
				if(contains_item(ws.w.nation_s.nations_arrays, this_war.defenders, b))
					return &this_war;
			} else {
				if(contains_item(ws.w.nation_s.nations_arrays, this_war.attackers, b))
					return &this_war;
			}
		}
		return nullptr;
	}

	war& create_war(world_state& ws, nations::country_tag attacker, nations::country_tag defender, bool /*call_willing_attacker_allies*/) {
		auto& new_war = ws.w.military_s.wars.get_new();
		new_war.primary_attacker = attacker;
		new_war.primary_defender = defender;
		add_item(ws.w.nation_s.nations_arrays, new_war.attackers, attacker);
		add_item(ws.w.nation_s.nations_arrays, new_war.defenders, defender);

		new_war.start_date = ws.w.current_date;

		// todo: call defender allies
		// todo: call attacker allies
		// todo: populate naval control
		// todo: name war

		return new_war;
	}

	bool is_naval_unit(military::unit_type_tag t, world_state const& ws) {
		auto unit_class = ws.s.military_m.unit_types[t].flags & military::unit_type::class_mask;
		if((unit_class == military::unit_type::class_big_ship) |
			(unit_class == military::unit_type::class_light_ship) |
			(unit_class == military::unit_type::class_transport))
			return true;
		else
			return false;
	}

	float single_state_war_score(world_state const& ws, nations::state_tag si, float owner_total_pop) {
		int32_t factory_count = economy::count_factories_in_state(ws, si);
		auto owner_id = ws.w.nation_s.states.get<state::owner>(si);
		
		float running_total = 0.0f;

		if(bool(owner_id) && !nations::is_colonial_or_protectorate(ws, si)) {
			auto owner_tag = ws.w.nation_s.nations.get<nation::tag>(owner_id);
			auto owner_cap = ws.w.nation_s.nations.get<nation::current_capital>(owner_id);

			nations::for_each_province(ws, si, [&running_total, owner_tag, owner_cap, owner_total_pop, &ws, &factory_count](provinces::province_tag prov) {
				auto province_pop = ws.w.province_s.province_demographics.get(prov, population::total_population_tag);
				auto& container = ws.w.province_s.province_state_container;

				int32_t base_cost_temp = container.get<province_state::fort_level>(prov) + container.get<province_state::naval_base_level>(prov) + factory_count + 1;
				factory_count = 0; // only add factories once

				if(contains_item(ws.w.province_s.core_arrays, container.get<province_state::cores>(prov), owner_tag))
					base_cost_temp *= 2;
				if(owner_cap == prov)
					base_cost_temp *= 3;

				running_total += 2.8f * std::clamp((owner_total_pop != 0) ? (float(base_cost_temp) * 200.0f * float(province_pop) / owner_total_pop) : float(base_cost_temp), 1.0f, float(base_cost_temp));
			});
		} else {
			nations::for_each_province(ws, si, [&running_total](provinces::province_tag ) {
				running_total += 2.8f;
			});
		}
		return running_total;
	}

	float calculate_base_war_score_cost(world_state const& ws, war_goal const& wg) {
		auto& this_cb_type = ws.s.military_m.cb_types[wg.cb_type];

		if((this_cb_type.flags & cb_type::great_war_obligatory) != 0)
			return 0.0f;

		float total_cost = 0.0f;

		if((this_cb_type.flags & cb_type::po_gunboat) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_gunboat;
		if((this_cb_type.flags & cb_type::po_add_to_sphere) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_add_to_sphere;
		if((this_cb_type.flags & cb_type::po_release_puppet) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_release_puppet;
		if((this_cb_type.flags & cb_type::po_colony) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_colony;
		if((this_cb_type.flags & cb_type::po_disarmament) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_disarmament;
		if((this_cb_type.flags & cb_type::po_destroy_forts) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_destroy_forts;
		if((this_cb_type.flags & cb_type::po_destroy_naval_bases) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_destroy_naval_bases;
		if((this_cb_type.flags & cb_type::po_reparations) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_reparations;
		if((this_cb_type.flags & cb_type::po_remove_prestige) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_prestige;
		if((this_cb_type.flags & cb_type::po_status_quo) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_status_quo;
		if((this_cb_type.flags & cb_type::po_annex) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_annex;
		if((this_cb_type.flags & cb_type::po_install_communist_gov_type) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_install_communist_gov_type;
		if((this_cb_type.flags & cb_type::po_uninstall_communist_gov_type) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_uninstall_communist_gov_type;
		if((this_cb_type.flags & cb_type::po_clear_union_sphere) != 0)
			total_cost += ws.s.modifiers_m.global_defines.peace_cost_clear_union_sphere * 4;

		// sum province values -- if all allowed states -- find all matching states to sum

		if((this_cb_type.flags & (cb_type::po_transfer_provinces | cb_type::po_demand_state)) != 0) {
			if(auto target = wg.target_country; ws.w.nation_s.nations.is_valid_index(target)) {
				auto target_total_pop = ws.w.nation_s.nation_demographics.get(target, population::total_population_tag);

				if((this_cb_type.flags & cb_type::all_allowed_states) == 0) {
					if(auto state = wg.target_state; is_valid_index(state))
						total_cost += single_state_war_score(ws, state, float(target_total_pop));
				} else {
					auto wg_from = wg.from_country;
					auto wg_liberation = wg.liberation_target;
					if(is_valid_index(wg_from) && is_valid_index(this_cb_type.allowed_states) && is_valid_index(wg_liberation)) {
						auto trigger_to_test = ws.s.trigger_m.trigger_data.data() + to_index(this_cb_type.allowed_states);
						nations::for_each_state(ws, target, [&ws, wg_from, wg_liberation, trigger_to_test, tp = float(target_total_pop), &total_cost](nations::state_tag si) {
							if(triggers::test_trigger(trigger_to_test, ws, si, wg_from, wg_liberation))
								total_cost += single_state_war_score(ws, si, tp);
						});
					}
				}
			}
		}

		return std::clamp(total_cost * this_cb_type.peace_cost_factor / 100.0f, 0.0f, 1.0f);
	}

	float total_attacker_demands_war_score(world_state const& ws, war const& w) {
		float sum = 0.0f;
		auto wg_range = get_range(ws.w.military_s.war_goal_arrays, w.war_goals);
		for(auto& i : wg_range) {
			if(contains_item(ws.w.nation_s.nations_arrays, w.attackers, i.from_country))
				sum += calculate_base_war_score_cost(ws, i);
		}
		return sum;
	}
	float total_defender_demands_war_score(world_state const& ws, war const& w) {
		float sum = 0.0f;
		auto wg_range = get_range(ws.w.military_s.war_goal_arrays, w.war_goals);
		for(auto& i : wg_range) {
			if(contains_item(ws.w.nation_s.nations_arrays, w.defenders, i.from_country))
				sum += calculate_base_war_score_cost(ws, i);
		}
		return sum;
	}

	float base_cb_infamy(world_state const& ws, cb_type_tag cb) {
		auto const& cb_definition = ws.s.military_m.cb_types[cb];
	
		float total = 0.0f;
		if((cb_definition.flags & cb_type::po_annex) != 0) {
			total += 10.0f;
		}
		if((cb_definition.flags & cb_type::po_demand_state) != 0) {
			total += 5.0f;
		}
		if((cb_definition.flags & cb_type::po_add_to_sphere) != 0) {
			total += 2.0f;
		}
		if((cb_definition.flags & cb_type::po_disarmament) != 0) {
			total += 5.0f;
		}
		if((cb_definition.flags & cb_type::po_reparations) != 0) {
			total += 5.0f;
		}
		if((cb_definition.flags & cb_type::po_transfer_provinces) != 0) {
			total += 5.0f;
		}
		if((cb_definition.flags & cb_type::po_remove_prestige) != 0) {
			total += 2.0f;
		}
		if((cb_definition.flags & cb_type::po_make_puppet) != 0) {
			total += 5.0f;
		}
		if((cb_definition.flags & cb_type::po_release_puppet) != 0) {
			total += 0.5f;
		}
		if((cb_definition.flags & cb_type::po_install_communist_gov_type) != 0) {
			total += 5.0f;
		}
		if((cb_definition.flags & cb_type::po_uninstall_communist_gov_type) != 0) {
			total += 5.0f;
		}
		if((cb_definition.flags & cb_type::po_destroy_forts) != 0) {
			total += 2.0f;
		}
		if((cb_definition.flags & cb_type::po_destroy_naval_bases) != 0) {
			total += 2.0f;
		}

		return total * cb_definition.badboy_factor;
	}

	float daily_cb_progress(world_state const& ws, nations::country_tag n, cb_type_tag type) {
		const auto nat_mod = ve::load(n, ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::cb_generation_speed_modifier>(ws.w.nation_s.nations.vector_size()));

		const auto adjusted_mod = nat_mod + 1.0f;
		const auto cb_speed = ve::load(type, ws.s.military_m.cb_type_to_speed.view());
		const auto base_speed = cb_speed * ws.s.modifiers_m.global_defines.cb_generation_base_speed * 0.01f;
		const auto old_value = ve::load(n, ws.w.nation_s.nations.get_row<nation::cb_construction_progress>());

		return adjusted_mod * base_speed;
	}

	void update_cb_construction(world_state& ws) {
		ve::execute_serial<nations::country_tag>(ws.w.nation_s.nations.vector_size(), [
			&ws,
			increment = ws.s.modifiers_m.global_defines.cb_generation_base_speed * 0.01f,
			mod_row = ws.w.nation_s.modifier_values.get_row<modifiers::national_offsets::cb_generation_speed_modifier>(ws.w.nation_s.nations.vector_size()),
			cb_type_row = ws.w.nation_s.nations.get_row<nation::cb_construction_type>(),
			progress_row = ws.w.nation_s.nations.get_row<nation::cb_construction_progress>()
		](auto off) {
			const auto nat_mod = ve::load(off, mod_row);
			const auto cb_type = ve::load(off, cb_type_row);

			const auto adjusted_mod = nat_mod + 1.0f;
			const auto cb_speed = ve::load(cb_type, ws.s.military_m.cb_type_to_speed.view());
			const auto base_speed = cb_speed * increment;
			const auto old_value = ve::load(off, progress_row);

			ve::store(off, progress_row, ve::multiply_and_add(adjusted_mod, base_speed, old_value));
		});

		ws.w.nation_s.nations.parallel_for_each([&ws](nations::country_tag n) {
			auto const cb_type_id = ws.w.nation_s.nations.get<nation::cb_construction_type>(n);
			auto const cb_target = ws.w.nation_s.nations.get<nation::cb_construction_target>(n);
			if(cb_target && cb_type_id) {
				if(ws.w.nation_s.nations.get<nation::cb_construction_progress>(n) >= 1.0f) {
					if(is_cb_construction_valid_against(ws, cb_type_id, n, cb_target)) {
						// is valid: add
						auto& cb_array = ws.w.nation_s.nations.get<nation::active_cbs>(n);
						add_item(ws.w.military_s.cb_arrays, cb_array, pending_cb{cb_target, cb_type_id, date_tag()});

						messages::acquired_cb(ws, n, cb_target, cb_type_id);
					} else {
						// not valid: post invalid message
						messages::cb_construction_invalid(ws, n, cb_target, cb_type_id);
					}
					// either case: erase
					ws.w.nation_s.nations.set<nation::cb_construction_type>(n, cb_type_tag());
					ws.w.nation_s.nations.set<nation::cb_construction_target>(n, nations::country_tag());
					ws.w.nation_s.nations.set<nation::cb_construction_discovered>(n, false);
					ws.w.nation_s.nations.set<nation::cb_construction_progress>(n, 0.0f);
				} else {
					// check for discovery & then validity
					if(ws.w.nation_s.nations.get<nation::cb_construction_discovered>(n) == false) {
						std::uniform_int_distribution<int32_t> dist(0, 1'000);
						auto const chance = dist(get_local_generator());
						auto const defines_probability = int32_t(ws.s.modifiers_m.global_defines.cb_detection_chance_base);
							
						if(chance <= defines_probability) {
							if(is_cb_construction_valid_against(ws, cb_type_id, n, cb_target)) {
								ws.w.nation_s.nations.set<nation::cb_construction_discovered>(n, true);
								auto const infamy_gain = base_cb_infamy(ws, cb_type_id) * (
									1.0f - ws.w.nation_s.nations.get<nation::cb_construction_progress>(n));
								ws.w.nation_s.nations.get<nation::infamy>(n) += infamy_gain;

								messages::cb_detected(ws, n, cb_target, cb_type_id, infamy_gain);
							} else {
								// not valid, erase and post invalid message
								ws.w.nation_s.nations.set<nation::cb_construction_type>(n, cb_type_tag());
								ws.w.nation_s.nations.set<nation::cb_construction_target>(n, nations::country_tag());
								ws.w.nation_s.nations.set<nation::cb_construction_discovered>(n, false);
								ws.w.nation_s.nations.set<nation::cb_construction_progress>(n, 0.0f);

								messages::cb_construction_invalid(ws, n, cb_target, cb_type_id);
							}
						}
					}
				}
			}
		});
	}
}
