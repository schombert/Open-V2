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
		return internal_is_cb_construction_valid_against<
			generic_test_trigger,
			internal_test_cb_conditions<generic_test_trigger, nations::owns_releasable_core, world_state>>(
				ws, cb, nation_by, nation_target);
	}

	inline auto get_player(world_state& ws, nations::country_tag n) {
		return ws.w.local_player_nation;
	}

	inline auto nation_exists_stub(world_state& ws, nations::country_tag n) {
		return nations::nation_exists(ws, n);
	}

	void init_player_cb_state(world_state& ws) {
		internal_init_player_cb_state<nation_exists_stub, get_player, generic_test_trigger>(ws);
	}

	void post_cb_change_message(bool new_state, world_state& ws, nations::country_tag player, nations::country_tag n, cb_type_tag cb_id) {
		if(new_state)
			messages::acquired_cb(ws, player, n, cb_id);
		else
			messages::lost_cb(ws, player, n, cb_id);
	}

	void update_player_cb_state(world_state& ws) {
		internal_update_player_cb_state<nation_exists_stub, get_player, generic_test_trigger, post_cb_change_message>(ws);
	}

	bool has_units_in_province(world_state const& ws, nations::country_tag this_nation, provinces::province_tag ps) {
		return internal_has_units_in_province(ws, this_nation, ps);
	}

	inline float regiment_size(world_state& ws) {
		return ws.s.modifiers_m.global_defines.pop_size_per_regiment;
	}

	uint32_t total_units_in_province(world_state const& ws, provinces::province_tag ps) {
		return internal_total_units_in_province<regiment_size>(ws, ps);
	}

	uint32_t total_active_divisions(world_state const& ws, nations::country_tag this_nation) {
		return internal_total_active_divisions< regiment_size>(ws, this_nation);
	}
	uint32_t total_active_ships(world_state const& ws, nations::country_tag this_nation) {
		return internal_total_active_ships(ws, this_nation);
	}

	population::demo_tag soldier_demo_tag(world_state const& ws) {
		return population::to_demo_tag(ws, ws.s.population_m.soldier);
	}

	float recruited_pop_fraction(world_state const& ws, nations::country_tag this_nation) {
		return internal_recruited_pop_fraction<soldier_demo_tag>(ws, this_nation);
	}

	bool has_named_leader(world_state const&, nations::country_tag, text_data::text_tag) {
		return true;
	}

	float get_warscore(world_state const& ws, war_tag wr) {
		return ws.get<war::current_war_score>(wr);
	}

	void empty_left_war_message(world_state& ws, war_tag this_war, nations::country_tag to_remove) {

	}

	void left_war_message(world_state& ws, war_tag this_war, nations::country_tag to_remove) {

	}

	void silent_remove_from_war(world_state& ws, war_tag this_war, nations::country_tag to_remove) {
		internal_remove_from_war<empty_left_war_message>(ws, this_war, to_remove);
	}

	void remove_from_war(world_state& ws, war_tag this_war, nations::country_tag to_remove) {
		internal_remove_from_war<left_war_message>(ws, this_war, to_remove);
	}

	void add_to_war_message(world_state& ws, war_tag this_war, bool as_attacker, nations::country_tag to_add) {

	}

	void add_to_war(world_state& ws, war_tag this_war, bool attacker, nations::country_tag to_add) {
		internal_add_to_war<add_to_war_message>(ws, this_war, attacker, to_add);
	}

	bool is_target_of_war_goal(world_state const& ws, war_tag this_war, nations::country_tag target) {
		return internal_is_target_of_war_goal(ws, this_war, target);
	}

	void release_army(world_state& ws, army_tag a) {
		ws.w.military_s.armies.release(a);
	}

	void destroy_army(world_state& ws, army_tag a) {
		internal_destroy_army<release_army, true>(ws, a);
	}

	void release_fleet(world_state& ws, fleet_tag f) {
		ws.w.military_s.fleets.release(f);
	}

	void destroy_fleet(world_state& ws, fleet_tag f) {
		internal_destroy_fleet<release_fleet, true>(ws, f);
	}

	void partial_destroy_fleet(world_state& ws, fleet_tag f) {
		internal_destroy_fleet<release_fleet, false>(ws, f);
	}

	void partial_destroy_army(world_state& ws, army_tag a) {
		internal_destroy_army<release_army, false>(ws, a);
	}

	void release_orders(world_state& ws, army_orders_tag o) {
		ws.w.military_s.army_orders.release(o);
	}

	void destroy_orders(world_state& ws, army_orders_tag o, nations::country_tag owner) {
		internal_destroy_orders<release_orders, true>(ws, o, owner);
	}

	void partial_destroy_orders(world_state& ws, army_orders_tag o) {
		internal_destroy_orders<release_orders, false>(ws, o, nations::country_tag());
	}

	void release_hq(world_state& ws, strategic_hq_tag o) {
		ws.w.military_s.strategic_hqs.release(o);
	}

	void destroy_hq(world_state& ws, strategic_hq_tag o, nations::country_tag owner) {
		internal_destroy_hq<release_hq, destroy_army, true>(ws, o, owner);
	}
	void partial_destroy_hq(world_state& ws, strategic_hq_tag o) {
		internal_destroy_hq<release_hq, destroy_army, false>(ws, o, nations::country_tag());
	}

	void release_leader(world_state& ws, leader_tag o) {
		ws.w.military_s.leaders.release(o);
	}

	void destroy_admiral(world_state& ws, leader_tag l, nations::country_tag owner) {
		internal_destroy_admiral<release_leader>(ws, l, owner);
	}

	void destroy_general(world_state& ws, leader_tag l, nations::country_tag owner) {
		internal_destroy_general<release_leader>(ws, l, owner);
	}

	void rebuild_fleet_presence(world_state&, nations::country_tag) {
		//stub
	}

	void release_war(world_state& ws, war_tag o) {
		ws.w.military_s.wars.release(o);
	}

	void destroy_war(world_state& ws, war_tag this_war) {
		internal_destroy_war<release_war>(ws, this_war);
	}

	war_tag get_war_between(world_state& ws, nations::country_tag a, nations::country_tag b) {
		return internal_get_war_between(ws, a, b);
	}

	date_tag get_date(world_state const& ws) {
		return ws.w.current_date;
	}
	war_tag allocate_war(world_state& ws) {
		return ws.w.military_s.wars.get_new();
	}

	war_tag create_war(world_state& ws, nations::country_tag attacker, nations::country_tag defender, bool call_willing_attacker_allies) {
		return internal_create_war<allocate_war, get_date>(ws, attacker, defender, call_willing_attacker_allies);
	}


	float single_state_war_score(world_state const& ws, nations::state_tag si, float owner_total_pop) {
		return internal_single_state_war_score<
			economy::count_factories_in_state,
			nations::is_colonial_or_protectorate<nations::country_tag>>(
				ws, si, owner_total_pop);
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

	float total_attacker_demands_war_score(world_state const& ws, war_tag w) {
		float sum = 0.0f;
		auto wg_range = get_range(ws.w.military_s.war_goal_arrays, w.war_goals);
		for(auto& i : wg_range) {
			if(contains_item(ws.w.nation_s.nations_arrays, w.attackers, i.from_country))
				sum += calculate_base_war_score_cost(ws, i);
		}
		return sum;
	}
	float total_defender_demands_war_score(world_state const& ws, war_tag w) {
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
