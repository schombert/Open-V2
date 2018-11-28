#include "common\\common.h"
#include "nations_functions.hpp"
#include "world_state\\world_state.h"
#include "scenario\\scenario.h"
#include "military\\military_functions.h"
#include "technologies\\technologies_functions.h"
#include "governments\\governments_functions.h"
#include "issues\\issues_functions.hpp"
#include "population\\population_function.h"
#include "provinces\\province_functions.h"
#include <ppl.h>
#include "economy\\economy_functions.h"

namespace nations {
	nations::country_tag state_owner(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.states.get<state::owner>(s);
	}
	void reset_nation(world_state& ws, nations::country_tag new_nation) {
		if(auto t = ws.w.nation_s.nations.get<nation::tag>(new_nation); is_valid_index(t))
			governments::get_best_parties_at_date(ws.w.nation_s.active_parties.get_row(new_nation), t, ws.w.current_date, ws.s);
	
		governments::reset_upper_house(ws, new_nation);
		issues::reset_active_issues(ws, new_nation);
		governments::update_current_rules(ws, new_nation);
		military::reset_unit_stats(ws, new_nation);

		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32>(
			ws.w.nation_s.national_stockpiles.get_row(new_nation),
			ws.s.economy_m.aligned_32_goods_count) =
			Eigen::Matrix<economy::goods_qnty_type, -1, 1>::Zero(ws.s.economy_m.aligned_32_goods_count);

		Eigen::Map<Eigen::Matrix<float, -1, 1>>(
			ws.w.nation_s.national_variables.get_row(new_nation),
			ws.s.variables_m.count_national_variables) =
			Eigen::Matrix<float, -1, 1>::Zero(ws.s.variables_m.count_national_variables);

		ws.w.nation_s.nations.set<nation::rich_tax>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::middle_tax>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::poor_tax>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::social_spending>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::administrative_spending>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::education_spending>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::military_spending>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::tarrifs>(new_nation, 0i8);
		ws.w.nation_s.nations.set<nation::army_stockpile_spending>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::navy_stockpile_spending>(new_nation, 10i8);
		ws.w.nation_s.nations.set<nation::projects_stockpile_spending>(new_nation, 10i8);

		technologies::reset_technologies(ws, new_nation);
	}

	void reset_state(nations_state& s) {
		s.static_modifier_arrays.reset();
		s.timed_modifier_arrays.reset();
		s.state_arrays.reset();
		s.influence_arrays.reset();
		s.nations_arrays.reset();
		s.state_tag_arrays.reset();
		s.relations_arrays.reset();
		s.truce_arrays.reset();
	}

	void destroy_nation(world_state& ws, nations::country_tag new_nation) {
		clear(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(new_nation));

		auto& owned_provinces = ws.w.nation_s.nations.get<nation::owned_provinces>(new_nation);
		auto owned_range = get_range(ws.w.province_s.province_arrays, owned_provinces);
		for(auto p : owned_range)
			ws.w.province_s.province_state_container.set<province_state::owner>(p, country_tag());
		clear(ws.w.province_s.province_arrays, owned_provinces);

		auto& controlled_provinces = ws.w.nation_s.nations.get<nation::controlled_provinces>(new_nation);
		auto controlled_range = get_range(ws.w.province_s.province_arrays, controlled_provinces);
		for(auto p : controlled_range)
			ws.w.province_s.province_state_container.set<province_state::controller>(p, country_tag());
		clear(ws.w.province_s.province_arrays, controlled_provinces);

		auto& naval_patrols = ws.w.nation_s.nations.get<nation::naval_patrols>(new_nation);
		auto prange = get_range(ws.w.province_s.province_arrays, naval_patrols);
		for(auto p : prange) {
			remove_item_if(ws.w.military_s.fleet_presence_arrays, ws.w.province_s.province_state_container.get<province_state::fleets>(p),
				[id = new_nation](military::fleet_presence const& fp) { return fp.owner == id; });
		}
		clear(ws.w.province_s.province_arrays, naval_patrols);

		auto& vassals = ws.w.nation_s.nations.get<nation::vassals>(new_nation);
		auto vrange = get_range(ws.w.nation_s.nations_arrays, vassals);
		for(auto v : vrange)
			ws.w.nation_s.nations.set<nation::overlord>(v, nations::country_tag());
		clear(ws.w.nation_s.nations_arrays, vassals);

		auto& allies = ws.w.nation_s.nations.get<nation::allies>(new_nation);
		auto arange = get_range(ws.w.nation_s.nations_arrays, allies);
		for(auto a : arange)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), new_nation);
		clear(ws.w.nation_s.nations_arrays, allies);

		auto& sphere_members = ws.w.nation_s.nations.get<nation::sphere_members>(new_nation);
		auto srange = get_range(ws.w.nation_s.nations_arrays, sphere_members);
		for(auto s : srange)
			ws.w.nation_s.nations.set<nation::sphere_leader>(s, nations::country_tag());
		clear(ws.w.nation_s.nations_arrays, sphere_members);

		auto& neighboring_nations = ws.w.nation_s.nations.get<nation::neighboring_nations>(new_nation);
		auto nrange = get_range(ws.w.nation_s.nations_arrays, neighboring_nations);
		for(auto n : nrange)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(n), new_nation);
		clear(ws.w.nation_s.nations_arrays, neighboring_nations);

		auto& sphere_leader = ws.w.nation_s.nations.get<nation::sphere_leader>(new_nation);
		if(is_valid_index(sphere_leader))
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(sphere_leader), new_nation);
		sphere_leader = nations::country_tag();

		auto& overlord = ws.w.nation_s.nations.get<nation::overlord>(new_nation);
		if(is_valid_index(overlord))
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(overlord), new_nation);
		overlord = nations::country_tag();

		auto& gp_influence = ws.w.nation_s.nations.get<nation::gp_influence>(new_nation);
		auto inf_range = get_range(ws.w.nation_s.influence_arrays, gp_influence);
		for(auto i = inf_range.first; i != inf_range.second; ++i)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(i->target), new_nation);
		clear(ws.w.nation_s.influence_arrays, gp_influence);

		auto& influencers = ws.w.nation_s.nations.get<nation::influencers>(new_nation);
		auto rev_inf_range = get_range(ws.w.nation_s.nations_arrays, influencers);
		for(auto i : rev_inf_range)
			remove_item(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(i), influence{0.0f, new_nation, 0ui8, 0i8});
		clear(ws.w.nation_s.nations_arrays, influencers);
		
		auto& relations = ws.w.nation_s.nations.get<nation::relations>(new_nation);
		auto rel_range = get_range(ws.w.nation_s.relations_arrays, relations);
		for(auto r = rel_range.first; r != rel_range.second; ++r)
			remove_item(ws.w.nation_s.relations_arrays, ws.w.nation_s.nations.get<nation::relations>(r->tag), relationship{ new_nation, 0i16 });
		clear(ws.w.nation_s.relations_arrays, relations);

		auto& truces = ws.w.nation_s.nations.get<nation::truces>(new_nation);
		auto truce_range = get_range(ws.w.nation_s.truce_arrays, truces);
		for(auto t = truce_range.first; t != truce_range.second; ++t)
			remove_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations.get<nation::truces>(t->tag), truce{date_tag(), new_nation});
		clear(ws.w.nation_s.truce_arrays, truces);
		
		clear(ws.w.economy_s.purchasing_arrays, ws.w.nation_s.nations.get<nation::statewise_tarrif_mask>(new_nation));
		clear(ws.w.variable_s.national_flags_arrays, ws.w.nation_s.nations.get<nation::national_flags>(new_nation));
		clear(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(new_nation));
		clear(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(new_nation));

		auto& active_movements = ws.w.nation_s.nations.get<nation::active_movements>(new_nation);
		auto movements = get_range(ws.w.population_s.pop_movement_arrays, active_movements);
		for(auto m : movements) {
			population::destroy_pop_movement(ws, ws.w.population_s.pop_movements[m]);
			ws.w.population_s.pop_movements.remove(m);
		}
		clear(ws.w.population_s.pop_movement_arrays, active_movements);

		auto& active_rebel_factions = ws.w.nation_s.nations.get<nation::active_rebel_factions>(new_nation);
		auto factions = get_range(ws.w.population_s.rebel_faction_arrays, active_rebel_factions);
		for(auto rf : factions) {
			population::destroy_rebel_faction(ws, ws.w.population_s.rebel_factions[rf]);
			ws.w.population_s.rebel_factions.remove(rf);
		}
		clear(ws.w.population_s.rebel_faction_arrays, active_rebel_factions);

		auto& allies_in_war = ws.w.nation_s.nations.get<nation::allies_in_war>(new_nation);
		auto wally_range = get_range(ws.w.nation_s.nations_arrays, allies_in_war);
		for(auto n : wally_range)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies_in_war>(n), new_nation);
		clear(ws.w.nation_s.nations_arrays, allies_in_war);

		auto& opponents_in_war = ws.w.nation_s.nations.get<nation::opponents_in_war>(new_nation);
		auto wopp_range = get_range(ws.w.nation_s.nations_arrays, opponents_in_war);
		for(auto n : wopp_range)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::opponents_in_war>(n), new_nation);
		clear(ws.w.nation_s.nations_arrays, opponents_in_war);

		auto& wars_involved_in = ws.w.nation_s.nations.get<nation::wars_involved_in>(new_nation);
		auto war_range = get_range(ws.w.military_s.war_arrays, wars_involved_in);
		for(auto w = war_range.first; w != war_range.second; ++w)
			military::silent_remove_from_war(ws, ws.w.military_s.wars[w->war_id], new_nation);
		clear(ws.w.military_s.war_arrays, wars_involved_in);

		auto& active_orders = ws.w.nation_s.nations.get<nation::active_orders>(new_nation);
		auto order_range = get_range(ws.w.military_s.orders_arrays, active_orders);
		for(auto o : order_range) {
			military::partial_destroy_orders(ws, ws.w.military_s.army_orders_container[o]);
			ws.w.military_s.army_orders_container.remove(o);
		}
		clear(ws.w.military_s.orders_arrays, active_orders);

		auto& armies = ws.w.nation_s.nations.get<nation::armies>(new_nation);
		auto army_range = get_range(ws.w.military_s.army_arrays, armies);
		for(auto a : army_range) {
			military::partial_destroy_army(ws, ws.w.military_s.armies[a]);
			ws.w.military_s.armies.remove(a);
		}
		clear(ws.w.military_s.army_arrays, armies);

		auto& fleets = ws.w.nation_s.nations.get<nation::fleets>(new_nation);
		auto fleet_range = get_range(ws.w.military_s.fleet_arrays, fleets);
		for(auto f : fleet_range) {
			military::partial_destroy_fleet(ws, ws.w.military_s.fleets[f]);
			ws.w.military_s.fleets.remove(f);
		}
		clear(ws.w.military_s.fleet_arrays, fleets);

		auto& generals = ws.w.nation_s.nations.get<nation::generals>(new_nation);
		auto gen_range = get_range(ws.w.military_s.leader_arrays, generals);
		for(auto g : gen_range)
			ws.w.military_s.leaders.remove(g);
		clear(ws.w.military_s.leader_arrays, generals);

		auto& admirals = ws.w.nation_s.nations.get<nation::admirals>(new_nation);
		auto adm_range = get_range(ws.w.military_s.leader_arrays, admirals);
		for(auto a : adm_range)
			ws.w.military_s.leaders.remove(a);
		clear(ws.w.military_s.leader_arrays, admirals);

		auto& national_focus_locations = ws.w.nation_s.nations.get<nation::national_focus_locations>(new_nation);
		auto focus_range = get_range(ws.w.nation_s.state_tag_arrays, national_focus_locations);
		for(auto s : focus_range)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.states.get<state::flashpoint_tension_focuses>(s), new_nation);
		clear(ws.w.nation_s.state_tag_arrays, national_focus_locations);

		auto& member_states = ws.w.nation_s.nations.get<nation::member_states>(new_nation);
		auto ms_range = get_range(ws.w.nation_s.state_arrays, member_states);
		for(auto s = ms_range.first; s != ms_range.second; ++s) {
			partial_destroy_state_instance(ws, s->state);
			ws.w.nation_s.states.release(s->state);
		}
		clear(ws.w.nation_s.state_arrays, member_states);
	}

	void remove_owned_province(world_state& ws, nations::country_tag n, provinces::province_tag p) { // removes province from list of owned
		remove_item(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n), p);
	}
	void remove_controlled_province(world_state& ws, nations::country_tag n, provinces::province_tag p) { // removes province from list of controlled
		remove_item(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::controlled_provinces>(n), p);
	}
	void add_owned_province(world_state& ws, nations::country_tag n, provinces::province_tag p) { // add province to list of owned
		add_item(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n), p);
	}
	void add_controlled_province(world_state& ws, nations::country_tag n, provinces::province_tag p) { // add province to list of controlled
		add_item(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::controlled_provinces>(n), p);
	}

	void annex_nation(world_state& ws, nations::country_tag this_nation, nations::country_tag to_annex) {
		boost::container::small_vector<provinces::province_tag, 64> owned_copy;
		auto target_owned = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(to_annex));
		for(auto p : target_owned)
			owned_copy.push_back(p);

		for(auto p : owned_copy) {
			provinces::silent_set_province_owner(ws, this_nation, p);
			provinces::silent_on_conquer_province(ws, p);
		}

		destroy_nation(ws, to_annex);
	}

	nations::country_tag liberate_uncored_cores(world_state& ws, nations::country_tag from, cultures::national_tag t) {
		auto from_tag = ws.w.nation_s.nations.get<nation::tag>(from);
		if(from_tag == t)
			return from;

		auto liberation_target = make_nation_for_tag(ws, t);
		
		auto target_owned = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(from));
		boost::container::small_vector<provinces::province_tag, 128> owned_copy(target_owned.first, target_owned.second);
		
		for(auto p : owned_copy) {
			auto core_array = ws.w.province_s.province_state_container.get<province_state::cores>(p);
			if(contains_item(ws.w.province_s.core_arrays, core_array, t) && !contains_item(ws.w.province_s.core_arrays, core_array, from_tag)) {
				provinces::silent_set_province_owner(ws, liberation_target, p);
				provinces::silent_on_conquer_province(ws, p);
			}
		}

		if(get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(from)) == 0)
			destroy_nation(ws, from);

		return liberation_target;
	}

	nations::country_tag liberate_all_cores(world_state& ws, nations::country_tag from, cultures::national_tag t) {
		auto from_tag = ws.w.nation_s.nations.get<nation::tag>(from);
		if(from_tag == t)
			return from;

		auto liberation_target = make_nation_for_tag(ws, t);

		auto target_owned = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(from));
		boost::container::small_vector<provinces::province_tag, 128> owned_copy(target_owned.first, target_owned.second);

		for(auto p : owned_copy) {
			if(contains_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(p), t)) {
				provinces::silent_set_province_owner(ws, liberation_target, p);
				provinces::silent_on_conquer_province(ws, p);
			}
		}

		if(get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(from)) == 0)
			destroy_nation(ws, from);

		return liberation_target;
	}

	void make_vassal(world_state& ws, nations::country_tag overlord, nations::country_tag vassal) {
		auto& vassal_overlord = ws.w.nation_s.nations.get<nation::overlord>(vassal);

		if(overlord == vassal || vassal_overlord == overlord)
			return;

		auto overlord_overlord = ws.w.nation_s.nations.get<nation::overlord>(overlord);
		if(is_valid_index(overlord_overlord)) {
			make_vassal(ws, overlord_overlord, vassal);
		} else {
			if(is_valid_index(vassal_overlord))
				free_vassal(ws, vassal);

			vassal_overlord = overlord;
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(overlord), vassal);

			auto& v_allies = ws.w.nation_s.nations.get<nation::allies>(vassal);
			auto allies_range = get_range(ws.w.nation_s.nations_arrays, v_allies);
			for(auto a : allies_range)
				remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), vassal);
			clear(ws.w.nation_s.nations_arrays, v_allies);

			auto war_range = get_range(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(vassal));
			boost::container::small_vector<military::war_identifier, 8> wars_copy(war_range.first, war_range.second);
			for(auto w : wars_copy) {
				auto& this_war = ws.w.military_s.wars[w.war_id];
				if(!military::is_target_of_war_goal(ws, this_war, vassal))
					military::remove_from_war(ws, this_war, vassal);
			}

			auto overlord_war_range = get_range(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(overlord));
			for(auto w = overlord_war_range.first; w != overlord_war_range.second; ++w)
				military::add_to_war(ws, ws.w.military_s.wars[w->war_id], w->is_attacker, vassal);

			governments::silent_set_government(ws, vassal, ws.w.nation_s.nations.get<nation::current_government>(overlord));
		}
	}
	void free_vassal(world_state& ws, nations::country_tag vassal) {
		auto& vassal_overlord = ws.w.nation_s.nations.get<nation::overlord>(vassal);
		if(!is_valid_index(vassal_overlord))
			return;

		remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(vassal_overlord), vassal);
		vassal_overlord = country_tag();
		ws.w.nation_s.nations.set<nation::is_substate>(vassal, false);

		auto war_range = get_range(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(vassal));
		boost::container::small_vector<military::war_identifier, 8> wars_copy(war_range.first, war_range.second);
		for(auto w : wars_copy) {
			auto& this_war = ws.w.military_s.wars[w.war_id];
			if(!military::is_target_of_war_goal(ws, this_war, vassal))
				military::remove_from_war(ws, this_war, vassal);
		}
	}

	country_tag make_nation_for_tag(world_state& ws, cultures::national_tag nt) {
		cultures::national_tag_state& tag_state = ws.w.culture_s.national_tags_state[nt];
		if(tag_state.holder)
			return tag_state.holder;
		country_tag new_nation = ws.w.nation_s.nations.get_new();

		auto& fixed_tag = ws.s.culture_m.national_tags[nt];

		tag_state.holder = new_nation;
		ws.w.nation_s.nations.set<nation::tag>(new_nation, nt);
		ws.w.nation_s.nations.set<nation::name>(new_nation, fixed_tag.default_name.name);
		ws.w.nation_s.nations.set<nation::adjective>(new_nation, fixed_tag.default_name.adjective);
		ws.w.nation_s.nations.set<nation::flag>(new_nation, fixed_tag.base_flag);

		ws.w.nation_s.nations.set<nation::current_color>(new_nation, fixed_tag.color);

		ws.w.nation_s.active_parties.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.nation_demographics.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.nation_colonial_demographics.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.upper_house.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.active_technologies.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.active_goods.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.collected_tariffs.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.active_issue_options.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.national_stockpiles.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.national_variables.ensure_capacity(to_index(new_nation) + 1);

		ws.w.nation_s.unit_stats.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.rebel_org_gain.ensure_capacity(to_index(new_nation) + 1);
		ws.w.nation_s.production_adjustments.ensure_capacity(to_index(new_nation) + 1);

		reset_nation(ws, new_nation);

		return new_nation;
	}

	bool is_state_empty(world_state const& ws, state_tag sid) {
		
		for(auto prange = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(sid)); prange.first != prange.second; ++prange.first) {
			if(ws.w.province_s.province_state_container.get<province_state::state_instance>(*prange.first) == sid)
				return false;
		}
		return true;
	}

	bool is_colonial_or_protectorate(world_state const& ws, state_tag s) {
		return ws.w.nation_s.states.get<state::is_protectorate>(s) || ws.w.nation_s.states.get<state::is_colonial>(s);
	}

	void partial_destroy_state_instance(world_state& ws, state_tag sid) {

		auto& fp_focuses = ws.w.nation_s.states.get<state::flashpoint_tension_focuses>(sid);
		auto ft_range = get_range(ws.w.nation_s.nations_arrays, fp_focuses);
		for(auto n : ft_range)
			remove_item(ws.w.nation_s.state_tag_arrays, ws.w.nation_s.nations.get<nation::national_focus_locations>(n), sid);
		clear(ws.w.nation_s.nations_arrays, fp_focuses);

		for(auto prange = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(sid)); prange.first != prange.second; ++prange.first) {
			auto& state_tag_ref = ws.w.province_s.province_state_container.get<province_state::state_instance>(*prange.first);
			if(state_tag_ref == sid)
				state_tag_ref = state_tag();
		}
	}

	void destroy_state_instance(world_state& ws, state_tag sid) {
		partial_destroy_state_instance(ws, sid);
		if(auto& owner = ws.w.nation_s.states.get<state::owner>(sid); bool(owner)) {
			remove_item(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(owner), region_state_pair{ ws.w.nation_s.states.get<state::region_id>(sid), sid });
			owner = country_tag();
		}
	}

	void remove_province_from_state(world_state& ws, provinces::province_tag p) {
		auto& state_tag_ref = ws.w.province_s.province_state_container.get<province_state::state_instance>(p);
		auto old_state = state_tag_ref;

		state_tag_ref = state_tag();
		if(is_valid_index(old_state))
			ws.w.nation_s.states.set<state::state_capital>(old_state, nations::find_state_capital(ws, old_state));
	}

	state_tag make_state(provinces::state_tag region, world_state& ws) {
		state_tag new_state = ws.w.nation_s.states.get_new();
		ws.w.nation_s.states.set<state::region_id>(new_state, region);
		ws.w.nation_s.states.set<state::name>(new_state, ws.s.province_m.state_names[region]);

		ws.w.nation_s.state_demographics.ensure_capacity(to_index(new_state) + 1);
		ws.w.nation_s.state_prices.ensure_capacity(to_index(new_state) + 1);
		ws.w.nation_s.state_demand.ensure_capacity(to_index(new_state) + 1);
		ws.w.nation_s.state_purchases.ensure_capacity(to_index(new_state) + 1);

		auto prices = ws.w.nation_s.state_prices.get_row(new_state);
		for(economy::goods_tag::value_base_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			prices[i] = ws.s.economy_m.goods[economy::goods_tag(i)].base_price;
		}
		std::fill_n(prices + ws.s.economy_m.aligned_32_goods_count, ws.s.economy_m.aligned_32_goods_count, economy::money_qnty_type(0));

		return new_state;
	}

	void init_empty_states(world_state& ws) {
		for(int32_t i = int32_t(ws.w.province_s.province_state_container.size()) - 1; i > 0; --i) {
			const provinces::province_tag this_prov_id(static_cast<provinces::province_tag::value_base_t>(i));

			auto state_id = ws.s.province_m.province_container.get<province::state_id>(this_prov_id);

			if((!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(this_prov_id))) &
				(ws.s.province_m.province_container.get<province::is_sea>(this_prov_id) == false) &
				(!(is_valid_index(ws.w.province_s.province_state_container.get<province_state::state_instance>(this_prov_id)))) &
				is_valid_index(state_id)) {

				auto new_state = make_state(state_id, ws);
				ws.w.province_s.province_state_container.set<province_state::state_instance>(this_prov_id, new_state);

				auto same_region_range = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(new_state));
				for(auto same_region_prov : same_region_range) {
					if((!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(same_region_prov))) &
						(ws.s.province_m.province_container.get<province::is_sea>(same_region_prov) == false) &
						(!(is_valid_index(ws.w.province_s.province_state_container.get<province_state::state_instance>(same_region_prov))))) {

						ws.w.province_s.province_state_container.set<province_state::state_instance>(same_region_prov, new_state);
					}
				}
			}
		}
		auto state_max = ws.w.nation_s.states.size();
		auto aligned_state_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(state_max) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));
		ws.w.nation_s.nations.parallel_for_each([&ws, aligned_state_max](nations::country_tag n) {
			resize(ws.w.economy_s.purchasing_arrays, ws.w.nation_s.nations.get<nation::statewise_tarrif_mask>(n), aligned_state_max);
		});
	}

	void init_nations_state(world_state& ws) {
		ws.w.nation_s.nation_demographics.reset(population::aligned_32_demo_size(ws));
		ws.w.nation_s.nation_colonial_demographics.reset(population::aligned_32_demo_size(ws));
		ws.w.nation_s.state_demographics.reset(population::aligned_32_demo_size(ws));
		ws.w.nation_s.active_parties.reset(ws.s.ideologies_m.ideologies_count);
		ws.w.nation_s.upper_house.reset(ws.s.ideologies_m.ideologies_count);
		ws.w.nation_s.active_technologies.reset((uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32);
		ws.w.nation_s.active_goods.reset((ws.s.economy_m.goods_count + 63ui32) / 64ui32);
		ws.w.nation_s.collected_tariffs.reset(ws.s.economy_m.goods_count);
		ws.w.nation_s.active_issue_options.reset(uint32_t(ws.s.issues_m.issues_container.size()));
		ws.w.nation_s.national_stockpiles.reset(uint32_t(ws.s.economy_m.aligned_32_goods_count));
		ws.w.nation_s.state_prices.reset(uint32_t(ws.s.economy_m.aligned_32_goods_count * 2));
		ws.w.nation_s.state_demand.reset(uint32_t(ws.s.economy_m.aligned_32_goods_count * 2));

		ws.w.nation_s.state_production.resize(int32_t(ws.s.economy_m.goods_count));
		ws.w.nation_s.state_global_demand.resize(int32_t(ws.s.economy_m.goods_count));

		ws.w.nation_s.national_variables.reset(ws.s.variables_m.count_national_variables);
		ws.w.nation_s.state_purchases.reset(uint32_t(ws.s.economy_m.goods_count));

		ws.w.nation_s.unit_stats.reset(static_cast<uint32_t>(ws.s.military_m.unit_types.size()));
		ws.w.nation_s.rebel_org_gain.reset(static_cast<uint32_t>(ws.s.population_m.rebel_types.size()));
		ws.w.nation_s.production_adjustments.reset(ws.s.economy_m.goods_count * uint32_t(technologies::production_adjustment::production_adjustment_count));
	}

	void update_state_nation_demographics(world_state& ws) {
		const auto full_vector_size = population::aligned_32_demo_size(ws);

		ws.w.nation_s.nations.parallel_for_each([&ws, full_vector_size](country_tag n) {
			Eigen::Map<Eigen::Matrix<float, -1, 1>, Eigen::AlignmentType::Aligned32> nation_demo(ws.w.nation_s.nation_demographics.get_row(n), full_vector_size);
			Eigen::Map<Eigen::Matrix<float, -1, 1>, Eigen::AlignmentType::Aligned32> nation_c_demo(ws.w.nation_s.nation_colonial_demographics.get_row(n), full_vector_size);
			nation_demo.setZero();
			nation_c_demo.setZero();

			const auto state_range = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(n));

			for(auto s = state_range.first; s != state_range.second; ++s) {
				Eigen::Map<Eigen::Matrix<float, -1, 1>, Eigen::AlignmentType::Aligned32> state_demo(ws.w.nation_s.state_demographics.get_row(s->state), full_vector_size);

				state_demo.setZero();

				const auto p_in_region_range = ws.s.province_m.states_to_province_index.get_row(s->region_id);
				for(auto p = p_in_region_range.first; p != p_in_region_range.second; ++p) {
					if(ws.w.province_s.province_state_container.get<province_state::owner>(*p) == n) {
						Eigen::Map<Eigen::Matrix<float, -1, 1>, Eigen::AlignmentType::Aligned32> province_demo(ws.w.province_s.province_demographics.get_row(*p), full_vector_size);
						state_demo += province_demo;
					}
				}

				if(state_demo[to_index(population::total_population_tag)] != 0) {
					const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));
					auto max_culture_off = maximum_index(state_demo.data() + to_index(culture_offset), int32_t(ws.s.culture_m.count_cultures));
					ws.w.nation_s.states.set<state::dominant_culture>(s->state, cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(max_culture_off)));

					const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));
					auto max_religion_off = maximum_index(state_demo.data() + to_index(religion_offset), int32_t(ws.s.culture_m.count_religions));
					ws.w.nation_s.states.set<state::dominant_religion>(s->state, cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(max_religion_off)));

					const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));
					auto max_ideology_off = maximum_index(state_demo.data() + to_index(ideology_offset), int32_t(ws.s.ideologies_m.ideologies_count));
					ws.w.nation_s.states.set<state::dominant_ideology>(s->state, ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(max_ideology_off)));

					const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));
					auto max_opinion_off = maximum_index(state_demo.data() + to_index(options_offset), int32_t(ws.s.issues_m.tracked_options_count));
					ws.w.nation_s.states.set<state::dominant_issue>(s->state, issues::option_tag(static_cast<value_base_of<issues::option_tag>>(max_opinion_off)));
				}

				if(!nations::is_colonial_or_protectorate(ws, s->state))
					nation_demo += state_demo;
				else
					nation_c_demo += state_demo;
			}

			if(nation_demo[to_index(population::total_population_tag)] != 0) {
				const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));
				auto max_culture_off = maximum_index(nation_demo.data() + to_index(culture_offset), int32_t(ws.s.culture_m.count_cultures));
				ws.w.nation_s.nations.set<nation::dominant_culture>(n, cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(max_culture_off)));

				const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));
				auto max_religion_off = maximum_index(nation_demo.data() + to_index(religion_offset), int32_t(ws.s.culture_m.count_religions));
				ws.w.nation_s.nations.set<nation::dominant_religion>(n, cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(max_religion_off)));

				const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));
				auto max_ideology_off = maximum_index(nation_demo.data() + to_index(ideology_offset), int32_t(ws.s.ideologies_m.ideologies_count));
				ws.w.nation_s.nations.set<nation::dominant_ideology>(n, ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(max_ideology_off)));

				const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));
				auto max_opinion_off = maximum_index(nation_demo.data() + to_index(options_offset), int32_t(ws.s.issues_m.tracked_options_count));
				ws.w.nation_s.nations.set<nation::dominant_issue>(n, issues::option_tag(static_cast<value_base_of<issues::option_tag>>(max_opinion_off)));
			}

			ws.w.nation_s.nations.set<nation::political_interest_fraction>(n, issues::calculate_political_interest(ws, nation_demo.data()));
			ws.w.nation_s.nations.set<nation::social_interest_fraction>(n, issues::calculate_social_interest(ws, nation_demo.data()));
		});
	}

	void fix_capitals(world_state& ws) {
		ws.w.nation_s.nations.for_each([&ws](nations::country_tag n) {
			auto& cc = ws.w.nation_s.nations.get<nation::current_capital>(n);
			if(!is_valid_index(cc) || ws.w.province_s.province_state_container.get<province_state::owner>(cc) != n)
				cc = find_best_capital(ws, n);
		});
	}
	provinces::province_tag find_best_capital(world_state const& ws, country_tag owner) {
		auto owner_tag = ws.w.nation_s.nations.get<nation::tag>(owner);
		if(is_valid_index(owner_tag)) {
			const auto tag_capital = ws.w.culture_s.national_tags_state[owner_tag].capital;
			if(is_valid_index(tag_capital) && ws.w.province_s.province_state_container.get<province_state::owner>(tag_capital) == owner)
				return tag_capital;

			float population = 0;
			provinces::province_tag best_province;

			const auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[owner_tag].core_provinces);
			for(auto c = core_range.first; c != core_range.second; ++c) {
				if(ws.w.province_s.province_state_container.get<province_state::owner>(*c) == owner) {
					float ppop = ws.w.province_s.province_demographics.get(*c, population::total_population_tag);
					if(ppop > population) {
						best_province = *c;
						population = ppop;
					}
				}
			}
			if(is_valid_index(best_province))
				return best_province;
		}
		
		float population = 0;
		provinces::province_tag best_province;

		const auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(owner));
		for(auto c = owned_range.first; c != owned_range.second; ++c) {
			float ppop = ws.w.province_s.province_demographics.get(*c, population::total_population_tag);
			if(ppop > population) {
				best_province = *c;
				population = ppop;
			}
		}

		return best_province;
	}

	void set_relationship(world_state& ws, country_tag a, country_tag b, int32_t value) {
		auto& a_relations = ws.w.nation_s.nations.get<nation::relations>(a);
		if(auto f = find(ws.w.nation_s.relations_arrays, a_relations, relationship{ b, 0i16 }); f)
			f->value = int16_t(value);
		else
			add_item(ws.w.nation_s.relations_arrays, a_relations, relationship{ b, int16_t(value) });

		auto& b_relations = ws.w.nation_s.nations.get<nation::relations>(b);
		if(auto f = find(ws.w.nation_s.relations_arrays, b_relations, relationship{ a, 0i16 }); f)
			f->value = int16_t(value);
		else
			add_item(ws.w.nation_s.relations_arrays, b_relations, relationship{ a, int16_t(value) });
	}
	void adjust_relationship(world_state& ws, country_tag a, country_tag b, int32_t value) {
		auto& a_relations = ws.w.nation_s.nations.get<nation::relations>(a);
		if(auto f = find(ws.w.nation_s.relations_arrays, a_relations, relationship{ b, 0i16 }); f)
			f->value = int16_t(std::clamp(value + f->value, -200, 200));
		else
			add_item(ws.w.nation_s.relations_arrays, a_relations, relationship{ b, int16_t(std::clamp(value, -200, 200)) });

		auto& b_relations = ws.w.nation_s.nations.get<nation::relations>(b);
		if(auto f = find(ws.w.nation_s.relations_arrays, b_relations, relationship{ a, 0i16 }); f)
			f->value = int16_t(std::clamp(value + f->value, -200, 200));
		else
			add_item(ws.w.nation_s.relations_arrays, b_relations, relationship{ a, int16_t(std::clamp(value, -200, 200)) });
	}
	int32_t get_relationship(world_state const& ws, country_tag a, country_tag b) {
		if(auto f = find(ws.w.nation_s.relations_arrays, ws.w.nation_s.nations.get<nation::relations>(a), relationship{ b, 0i16 }); f)
			return f->value;
		return 0;
	}
	int32_t get_influence_value(world_state const& ws, country_tag a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(a), influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->amount;
		return 0;
	}
	int32_t get_influence_level(world_state const& ws, country_tag a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(a), influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->level;
		return 2;
	}
	nations::influence get_influence(world_state const& ws, country_tag nation_by, country_tag nation_over) {
		if(auto f = find(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(nation_by), influence{ 0.0f, nation_over, 0ui8, 0i8 }); f)
			return *f;
		return nations::influence{0.0f, nation_over, 0ui8, 2i8 };
	}
	void set_influence(world_state& ws, country_tag a, country_tag b, int32_t value, int32_t level) {
		auto& a_inf = ws.w.nation_s.nations.get<nation::gp_influence>(a);
		auto f = find(ws.w.nation_s.influence_arrays, a_inf, influence{ 0.0f, b, 0ui8, 0i8 });
		if(f == nullptr) {
			add_item(ws.w.nation_s.influence_arrays, a_inf, influence{ 0.0f, b, uint8_t(0), int8_t(2) });
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(b), a);

			f = find(ws.w.nation_s.influence_arrays, a_inf, influence{ 0.0f, b, 0ui8, 0i8 });
		}

		f->amount = uint8_t(value);

		if(f->level == 5i8 && level != 5) {
			ws.w.nation_s.nations.set<nation::sphere_leader>(b, country_tag());
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(a), b);
			f->level = int8_t(level);
		} else if(f->level != 5i8 && level == 5) {
			auto& b_leader = ws.w.nation_s.nations.get<nation::sphere_leader>(b);
			if(b_leader == country_tag()) {
				b_leader = a;
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(a), b);
				f->level = int8_t(5);
			} else {
				f->level = int8_t(4);
			}
		} else {
			f->level = int8_t(level);
		}
	}
	void set_influence_value(world_state& ws, country_tag a, country_tag b, int32_t value) {
		auto& a_inf = ws.w.nation_s.nations.get<nation::gp_influence>(a);
		if(auto f = find(ws.w.nation_s.influence_arrays, a_inf, influence{ 0.0f, b, 0ui8, 0i8 }); f) {
			f->amount = uint8_t(value);
		} else {
			add_item(ws.w.nation_s.influence_arrays, a_inf, influence{ 0.0f, b, uint8_t(value), int8_t(2) });
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(b), a);
		}
	}
	float get_foreign_investment(world_state const& ws, country_tag a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(a), influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->investment_amount;
		return 0.0f;
	}
	void set_foreign_investment(world_state& ws, country_tag a, country_tag b, float value) {
		auto& a_inf = ws.w.nation_s.nations.get<nation::gp_influence>(a);
		if(auto f = find(ws.w.nation_s.influence_arrays, a_inf, influence{ 0.0f, b, 0ui8, 0i8 }); f) {
			f->investment_amount = value;
		} else {
			add_item(ws.w.nation_s.influence_arrays, a_inf, influence{ value, b, 0ui8, 0i8 });
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(b), a);
		}
	}
	void remove_investment_and_influence(world_state& ws, country_tag nation_by, country_tag nation_target) {
		remove_item(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(nation_by), influence{ 0.0f, nation_target, 0ui8, 0i8 });
		remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(nation_target), nation_by);
	}

	int32_t colonial_points_to_make_protectorate(world_state const&, state_tag) {
		return 0;
	}
	int32_t colonial_points_to_make_colony(world_state const&, state_tag) {
		return 0;
	}
	int32_t colonial_points_to_make_state(world_state const&, state_tag) {
		return 0;
	}
	int32_t free_colonial_points(world_state const&, country_tag) {
		return 0;
	}
	int32_t points_for_next_colonial_stage(world_state const&, country_tag, state_tag) {
		return 0;
	}

	text_data::text_tag get_nation_status_text(world_state const& ws, country_tag this_nation) {
		if(ws.w.nation_s.nations.get<nation::is_civilized>(this_nation) == false) {
			if(ws.w.nation_s.modifier_values.get<modifiers::national_offsets::civilization_progress_modifier>(this_nation) > modifiers::value_type(0))
				return ws.s.fixed_ui_text[scenario::fixed_ui::partialy_civilized_nation];
			else
				return ws.s.fixed_ui_text[scenario::fixed_ui::uncivilized_nation];
		} else if(is_great_power(ws, this_nation))
			return ws.s.fixed_ui_text[scenario::fixed_ui::great_power];
		else if(ws.w.nation_s.nations.get<nation::overall_rank>(this_nation) <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
			return ws.s.fixed_ui_text[scenario::fixed_ui::secondary_power];
		else
			return ws.s.fixed_ui_text[scenario::fixed_ui::civilized_nation];
		
	}
	
	bool is_great_power(world_state const& ws, country_tag n) {
		return ws.w.nation_s.nations.get<nation::overall_rank>(n) <= 8i16;
	}

	void silent_make_alliance(world_state& ws, country_tag a, country_tag b) {
		add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), b);
		add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(b), a);
	}

	void make_alliance(world_state& ws, country_tag a, country_tag b) {
		silent_make_alliance(ws, a, b);
	}
	void end_alliance(world_state& ws, country_tag a, country_tag b) {
		remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(a), b);
		remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(b), a);
	}

	void simple_make_vassal(world_state& ws, country_tag overlord, country_tag vassal) {
		auto& voverlord = ws.w.nation_s.nations.get<nation::overlord>(vassal);
		if(voverlord == country_tag()) {
			voverlord = overlord;
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(overlord), vassal);
		}
	}

	void simple_make_substate(world_state& ws, country_tag overlord, country_tag vassal) {
		simple_make_vassal(ws, overlord, vassal);
		ws.w.nation_s.nations.set<nation::is_substate>(vassal, true);
	}

	country_tag union_holder_for(world_state const& ws, cultures::culture_tag pculture) {
		auto cgroup = ws.s.culture_m.culture_container[pculture].group;
		auto union_tag = ws.s.culture_m.culture_groups[cgroup].union_tag;
		if(is_valid_index(union_tag))
			return ws.w.culture_s.national_tags_state[union_tag].holder;
		else
			return country_tag();
	}

	cultures::national_tag union_tag_of(world_state const& ws, country_tag this_nation) {
		auto pculture = ws.w.nation_s.nations.get<nation::primary_culture>(this_nation);
		if(is_valid_index(pculture)) {
			auto cgroup = ws.s.culture_m.culture_container[pculture].group;
			return ws.s.culture_m.culture_groups[cgroup].union_tag;
		}
		else
			return cultures::national_tag();
	}

	country_tag union_holder_of(world_state const& ws, country_tag this_nation) {
		auto pculture = ws.w.nation_s.nations.get<nation::primary_culture>(this_nation);
		if(is_valid_index(pculture))
			return union_holder_for(ws, pculture);
		else
			return country_tag();
	}

	economy::goods_qnty_type national_treasury(world_state const& ws, country_tag id) {
		return ws.w.nation_s.national_stockpiles.get(id, economy::money_good);
	}

	float fraction_of_cores_owned(world_state const& ws, country_tag this_nation) {
		auto tag = ws.w.nation_s.nations.get<nation::tag>(this_nation);
		if(is_valid_index(tag)) {
			auto cores_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);
			float cores_owned_count = 0.0f;
			for(auto p : cores_range) {
				if(ws.w.province_s.province_state_container.get<province_state::owner>(p) == this_nation)
					cores_owned_count += 1.0f;
			}
			return (cores_range.second != cores_range.first) ? (cores_owned_count / float(cores_range.second - cores_range.first)) : 1.0f;
		} else {
			return 1.0f;
		}
	}

	bool can_release_as_vassal(world_state const& ws, country_tag this_nation, cultures::national_tag vassal) {
		auto& vassal_tag_info = ws.w.culture_s.national_tags_state[vassal];

		if(vassal_tag_info.is_not_releasable)
			return false;

		auto current_holder = vassal_tag_info.holder;
		if(current_holder && 0 != get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(current_holder)))
			return false; // national already exists

		auto core_range = get_range(ws.w.province_s.province_arrays, vassal_tag_info.core_provinces);
		auto this_owned = ws.w.nation_s.nations.get<nation::owned_provinces>(this_nation);
		for(auto p : core_range) {
			if(contains_item(ws.w.province_s.province_arrays, this_owned, p))
				return true;
		}
		return false;
	}

	float get_prestige(world_state const& ws, nations::country_tag n) {
		return ws.w.nation_s.nations.get<nation::base_prestige>(n) + ws.w.nation_s.nations.get<nation::tech_attributes>(n)[technologies::tech_offset::permanent_prestige];
	}
	int32_t get_colonial_points(world_state const& ws, nations::country_tag n) {
		return int32_t(ws.w.nation_s.nations.get<nation::base_colonial_points>(n)) + int32_t(ws.w.nation_s.nations.get<nation::tech_attributes>(n)[technologies::tech_offset::colonial_points]);
	}

	float calculate_state_administrative_efficiency(world_state const& ws, state_tag this_state, float admin_requirement) {
		auto owner = ws.w.nation_s.states.get<state::owner>(this_state);
		if(bool(owner))
			return 0.0f;

		int32_t count_non_core = 0;

		auto region_provs = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(this_state));
		for(auto p : region_provs) {
			if((ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == this_state)
				& ((ws.w.province_s.province_state_container.get<province_state::has_owner_core>(p)) == false)) {
				++count_non_core;
			}
		}
		//stub
		return std::clamp(
			float(count_non_core) * ws.s.modifiers_m.global_defines.noncore_tax_penalty +
			ws.s.modifiers_m.global_defines.base_country_admin_efficiency + 
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::administrative_efficiency_modifier>(owner) +
			float(ws.w.nation_s.state_demographics.get(this_state, population::to_demo_tag(ws, ws.s.population_m.bureaucrat))) /
			(float(ws.w.nation_s.state_demographics.get(this_state, population::total_population_tag)) * admin_requirement),
			0.05f, 1.0f);
	}

	float calculate_national_administrative_efficiency(world_state const& ws, nations::country_tag n) {
		auto b_amount = ws.w.nation_s.nation_demographics.get(n, population::to_demo_tag(ws, ws.s.population_m.bureaucrat));
		auto total_pop = ws.w.nation_s.nation_demographics.get(n, population::total_population_tag);

		if(total_pop == 0)
			return 0.0f;

		auto ratio_num = b_amount * (1.0f + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::administrative_efficiency_modifier>(n)) / total_pop;

		auto issues_range = ws.w.nation_s.active_issue_options.get_row(n);
		auto ratio_denom = ws.s.modifiers_m.global_defines.max_bureaucracy_percentage
			+ (ws.s.modifiers_m.global_defines.bureaucracy_percentage_increment
			* std::transform_reduce(issues_range, issues_range + ws.s.issues_m.tracked_options_count, 0.0f, std::plus<>(),
			[&ws](issues::option_tag opt) {
			if(is_valid_index(opt))
				return float(ws.s.issues_m.options[opt].administrative_multiplier);
			else 
				return 0.0f;
		}));

		return std::clamp(ratio_num / ratio_denom, 0.05f, 1.0f);
	}

	float calculate_revanchism(world_state const & ws, country_tag n) {
		float sum = 0;
		if(auto n_tag = ws.w.nation_s.nations.get<nation::tag>(n);  is_valid_index(n_tag)) {
			auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[n_tag].core_provinces);
			auto pculture = ws.w.nation_s.nations.get<nation::primary_culture>(n);
			for(auto c : core_range) {
				if(ws.w.province_s.province_state_container.get<province_state::owner>(c) != n) {
					if(ws.w.province_s.province_state_container.get<province_state::dominant_culture>(c) == pculture)
						sum += 1.0f;
					else
						sum += 0.25f;
				}
			}
			if(core_range.first != core_range.second)
				return sum / float(core_range.second - core_range.first);
		}
		return 0.0f;
	}

	void update_neighbors(world_state& ws, nations::country_tag this_nation) {
		auto& nn = ws.w.nation_s.nations.get<nation::neighboring_nations>(this_nation);
		resize(ws.w.nation_s.nations_arrays, nn, 0ui32);

		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(this_nation));
		for(auto p : owned_range) {
			auto adj_range = ws.s.province_m.same_type_adjacency.get_row(p);
			for(auto a : adj_range) {
				auto adj_owner = ws.w.province_s.province_state_container.get<province_state::owner>(a);
				if(is_valid_index(adj_owner) && adj_owner != this_nation) {
					add_item(ws.w.nation_s.nations_arrays, nn, adj_owner);
				}
			}
		}
	}

	uint32_t calculate_blockaded_count(world_state const& ws, nations::country_tag this_nation) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(this_nation));
		uint32_t total = 0ui32;

		for(auto p : owned_range) {
			if(ws.w.province_s.province_state_container.get<province_state::is_overseas>(p) == false &&
				ws.w.province_s.province_state_container.get<province_state::is_blockaded>(p) == true) {
				++total;
			}
		}
		return total;
	}

	uint32_t calculate_crime_count(world_state const& ws, nations::country_tag this_nation) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(this_nation));
		uint32_t total = 0ui32;

		for(auto p : owned_range) {
			if(ws.w.province_s.province_state_container.get<province_state::is_overseas>(p) == false
				&& is_valid_index(ws.w.province_s.province_state_container.get<province_state::crime>(p))) {
				++total;
			}
		}
		return total;
	}

	uint32_t calculate_rebel_controlled_count(world_state const& ws, nations::country_tag this_nation) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(this_nation));
		uint32_t total = 0ui32;

		for(auto p : owned_range) {
			if(ws.w.province_s.province_state_container.get<province_state::is_overseas>(p) == false
				&& is_valid_index(ws.w.province_s.province_state_container.get<province_state::rebel_controller>(p))) {
				++total;
			}
		}
		return total;
	}

	void update_province_counts(world_state& ws, nations::country_tag this_nation) {
		auto& central_province_count = ws.w.nation_s.nations.get<nation::central_province_count>(this_nation);
		central_province_count = 0ui16;
		
		auto& blockaded_count = ws.w.nation_s.nations.get<nation::blockaded_count>(this_nation);
		blockaded_count = 0ui16;

		auto& crime_count = ws.w.nation_s.nations.get<nation::crime_count>(this_nation);
		crime_count = 0ui16;

		auto& rebel_controlled_provinces = ws.w.nation_s.nations.get<nation::rebel_controlled_provinces>(this_nation);
		rebel_controlled_provinces = 0ui16;

		auto& num_ports = ws.w.nation_s.nations.get<nation::num_ports>(this_nation);
		num_ports = 0ui16;

		auto& num_connected_ports = ws.w.nation_s.nations.get<nation::num_connected_ports>(this_nation);
		num_connected_ports = 0ui16;

		auto current_capital = ws.w.nation_s.nations.get<nation::current_capital>(this_nation);
		if(!is_valid_index(current_capital))
			return;

		auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(this_nation));
		for(auto p : owned_range) {
			ws.w.province_s.province_state_container.set<province_state::is_overseas>(p, false);
			if(ws.w.province_s.province_state_container.get<province_state::naval_base_level>(p) != 0)
				++num_ports;
		}

		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>> already_added;
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>> pending_neighbor_check;

		pending_neighbor_check.insert(current_capital);

		while(pending_neighbor_check.size() != 0) {
			auto tp = *(pending_neighbor_check.end() - 1);
			pending_neighbor_check.erase(pending_neighbor_check.end() - 1);
			already_added.insert(tp);

			++central_province_count;
			if(ws.w.province_s.province_state_container.get<province_state::naval_base_level>(tp) != 0)
				++num_connected_ports;
			if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::rebel_controller>(tp)))
				++rebel_controlled_provinces;
			if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::crime>(tp)))
				++crime_count;
			if(ws.w.province_s.province_state_container.get<province_state::is_blockaded>(tp))
				++blockaded_count;

			auto adj_range = ws.s.province_m.same_type_adjacency.get_row(tp);
			for(auto a : adj_range) {
				if(ws.w.province_s.province_state_container.get<province_state::owner>(a) == this_nation && already_added.count(a) == 0 && pending_neighbor_check.count(a) == 0)
					pending_neighbor_check.insert(a);
			}
		}
	}

	void update_movement_support(world_state& ws, nations::country_tag this_nation) {
		float total_pop = float(ws.w.nation_s.nation_demographics.get(this_nation, population::total_population_tag));
		auto& social_movement_support = ws.w.nation_s.nations.get<nation::social_movement_support>(this_nation);
		auto& political_movement_support = ws.w.nation_s.nations.get<nation::political_movement_support>(this_nation);

		if(total_pop == 0.0f) {
			social_movement_support = 0.0f;
			political_movement_support = 0.0f;
			return;
		}

		auto movements = get_range(ws.w.population_s.pop_movement_arrays, ws.w.nation_s.nations.get<nation::active_movements>(this_nation));
		int64_t total_social_support = 0;
		int64_t total_political_support = 0;
		for(auto m : movements) {
			if(ws.w.population_s.pop_movements[m].type == population::movement_type::political) {
				total_political_support += ws.w.population_s.pop_movements[m].total_population_support;
			} else if(ws.w.population_s.pop_movements[m].type == population::movement_type::social) {
				total_social_support += ws.w.population_s.pop_movements[m].total_population_support;
			}
		}

		
		social_movement_support = std::min(1.0f, (float(total_social_support) / total_pop) * ws.s.modifiers_m.global_defines.movement_support_uh_factor);
		political_movement_support = std::min(1.0f, (float(total_political_support) / total_pop) * ws.s.modifiers_m.global_defines.movement_support_uh_factor);
	}

	int32_t calculate_industrial_score(world_state const& ws, nations::country_tag this_nation) {
		float total = 0;
		auto states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(this_nation));

		boost::container::small_vector<population::demo_tag, 8> factory_demo_tags;
		boost::container::small_vector<population::demo_tag, 8> factory_employment_tags;
		for(auto ct : ws.s.population_m.factory_workers) {
			factory_demo_tags.push_back(population::to_demo_tag(ws, ct));
			factory_employment_tags.push_back(population::to_employment_demo_tag(ws, ct));
		}

		for(auto s = states.first; s != states.second; ++s) {
			auto state_worker_pop = std::accumulate(factory_demo_tags.begin(), factory_demo_tags.end(), 0.0f,
				[&ws, sid = s->state](float sum, population::demo_tag t) { return sum + float(ws.w.nation_s.state_demographics.get(sid, t)); });
			if(state_worker_pop != 0.0f) {
				auto state_worker_employed = std::accumulate(factory_employment_tags.begin(), factory_employment_tags.end(), 0.0f,
					[&ws, sid = s->state](float sum, population::demo_tag t) { return sum + float(ws.w.nation_s.state_demographics.get(sid, t)); });

				int32_t total_flevels_x_4 = 0;
				auto& factories = ws.w.nation_s.states.get<state::factories>(s->state);
				for(uint32_t i = 0; i < factories.size(); ++i) {
					total_flevels_x_4 += 4 * factories[i].level;
				}

				total += float(total_flevels_x_4 * state_worker_employed) / state_worker_pop;
			}
		}

		auto investment = get_range(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(this_nation));
		for(auto v = investment.first; v != investment.second; ++v) {
			total += v->investment_amount * ws.s.modifiers_m.global_defines.investment_score_factor * 0.01f;
		}
		return int32_t(total);
	}

	int32_t calculate_military_score(world_state const& ws, nations::country_tag this_nation) {
		float* unit_type_scores = (float*)_alloca(sizeof(float) * ws.s.military_m.unit_types_count);
		std::fill_n(unit_type_scores, ws.s.military_m.unit_types_count, 0.0f);

		auto unit_attributes_row = ws.w.nation_s.unit_stats.get_row(this_nation);
		float land_unit_type_count = 0.0f;
		float land_uint_score = 0.0f;

		for(uint32_t i = 0; i < ws.s.military_m.unit_types_count; ++i) {
			military::unit_type_tag this_tag(static_cast<military::unit_type_tag::value_base_t>(i));
			auto masked_type = ws.s.military_m.unit_types[this_tag].flags & military::unit_type::class_mask;
			if(masked_type == military::unit_type::class_big_ship) {
				// capital ship
				unit_type_scores[i] =
					(unit_attributes_row[i][military::unit_attribute::hull] + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::naval_defense_modifier>(this_nation)) *
					(unit_attributes_row[i][military::unit_attribute::gun_power] + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::naval_attack_modifier>(this_nation)) /
					250.0f;
			} else if(masked_type == military::unit_type::class_light_ship || masked_type == military::unit_type::class_transport) {
				// other naval
			} else {
				// land
				land_uint_score +=
					(unit_attributes_row[i][military::unit_attribute::attack] +
						ws.w.nation_s.modifier_values.get<modifiers::national_offsets::land_attack_modifier>(this_nation) +
						unit_attributes_row[i][military::unit_attribute::defense] +
						ws.w.nation_s.modifier_values.get<modifiers::national_offsets::land_defense_modifier>(this_nation)) *
						unit_attributes_row[i][military::unit_attribute::discipline];
				++land_unit_type_count;
			}
		}
		auto disarm_date = ws.w.nation_s.nations.get<nation::disarmed_until>(this_nation);
		bool is_disarmed = is_valid_index(disarm_date) && ws.w.current_date < disarm_date;

		float total_active_regiments = 0.0f;
		auto armies = get_range(ws.w.military_s.army_arrays, ws.w.nation_s.nations.get<nation::armies>(this_nation));
		for(auto a : armies)
			total_active_regiments += float(ws.w.military_s.armies[a].total_soldiers);
		total_active_regiments /= ws.s.modifiers_m.global_defines.pop_size_per_regiment;

		float total_possible_regiments =
			(ws.w.nation_s.nation_demographics.get(this_nation, population::to_demo_tag(ws, ws.s.population_m.soldier)) + 
				ws.w.nation_s.nation_colonial_demographics.get(this_nation, population::to_demo_tag(ws, ws.s.population_m.soldier))) /
			ws.s.modifiers_m.global_defines.pop_size_per_regiment;

		float total_sum = ((land_uint_score / land_unit_type_count) *
			(is_disarmed ? ws.s.modifiers_m.global_defines.disarmament_army_hit : 1.0f) *
			std::max(0.0f, (ws.w.nation_s.modifier_values.get<modifiers::national_offsets::supply_consumption>(this_nation) + 1.0f)) *
			std::min(total_active_regiments * 4.0f, total_possible_regiments)) / 7.0f; // = land contribution

		auto fleets = get_range(ws.w.military_s.fleet_arrays, ws.w.nation_s.nations.get<nation::fleets>(this_nation));
		for(auto f : fleets) {
			auto ships = get_range(ws.w.military_s.ship_arrays, ws.w.military_s.fleets[f].ships);
			for(auto s = ships.first; s != ships.second; ++s) {
				total_sum += unit_type_scores[to_index(s->type)];
			}
		}

		total_sum += std::min(float(get_size(ws.w.military_s.leader_arrays, ws.w.nation_s.nations.get<nation::admirals>(this_nation))
			+ get_size(ws.w.military_s.leader_arrays, ws.w.nation_s.nations.get<nation::generals>(this_nation))), total_active_regiments);

		return int32_t(total_sum);
	}

	void update_nation_ranks(world_state& ws) {
		int32_t nations_count = 0;
		ws.w.nation_s.nations.for_each([&nations_count](nations::country_tag n) {
			++nations_count;
		});

		country_tag* temp_buf = (country_tag*)_alloca(sizeof(country_tag) * ws.w.nation_s.nations.size());
		std::uninitialized_value_construct_n(temp_buf, ws.w.nation_s.nations.size());
		ws.w.nation_s.nations.for_each([temp_buf, temp_count = 0](nations::country_tag n) {
			temp_buf[to_index(n)] = n;
		});
		std::stable_partition(temp_buf, temp_buf + ws.w.nation_s.nations.size(), [](nations::country_tag a) { return is_valid_index(a); });

		std::sort(temp_buf, temp_buf + nations_count, [&ws](nations::country_tag a, nations::country_tag b) {
			return ws.w.nation_s.nations.get<nation::industrial_score>(a) > ws.w.nation_s.nations.get<nation::industrial_score>(b);
		});
		for(int32_t i = 0; i < nations_count; ++i)
			ws.w.nation_s.nations.set<nation::industrial_rank>(temp_buf[i], int16_t(i + 1));
		std::sort(temp_buf, temp_buf + nations_count, [&ws](nations::country_tag a, nations::country_tag b) {
			return ws.w.nation_s.nations.get<nation::military_score>(a) > ws.w.nation_s.nations.get<nation::military_score>(b);
		});
		for(int32_t i = 0; i < nations_count; ++i)
			ws.w.nation_s.nations.set<nation::military_rank>(temp_buf[i], int16_t(i + 1));
		std::sort(temp_buf, temp_buf + nations_count, [&ws](nations::country_tag a, nations::country_tag b) {
			return nations::get_prestige(ws, a) > nations::get_prestige(ws, b);
		});
		for(int32_t i = 0; i < nations_count; ++i)
			ws.w.nation_s.nations.set<nation::prestige_rank>(temp_buf[i], int16_t(i + 1));

		std::sort(temp_buf, temp_buf + nations_count, [&ws](nations::country_tag a, nations::country_tag b) {
			return (nations::get_prestige(ws, a) + ws.w.nation_s.nations.get<nation::military_score>(a) + ws.w.nation_s.nations.get<nation::industrial_score>(a))
		> (nations::get_prestige(ws, b) + ws.w.nation_s.nations.get<nation::military_score>(b) + ws.w.nation_s.nations.get<nation::industrial_score>(b));
		});
		std::stable_partition(temp_buf, temp_buf + nations_count, [&ws](nations::country_tag a) {
			return !is_valid_index(ws.w.nation_s.nations.get<nation::overlord>(a));
		});
		std::stable_partition(temp_buf, temp_buf + nations_count, [&ws](nations::country_tag a) {
			return ws.w.nation_s.nations.get<nation::is_civilized>(a);
		});
		std::stable_partition(temp_buf, temp_buf + nations_count, [&ws](nations::country_tag a) {
			return is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(a));
		});

		resize(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank, nations_count);
		for(int32_t i = 0; i < nations_count; ++i) {
			ws.w.nation_s.nations.set<nation::overall_rank>(temp_buf[i], int16_t(i + 1));
			get(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank, i) = temp_buf[i];
		}
	}

	void change_tag(world_state& ws, nations::country_tag this_nation, cultures::national_tag new_tag) {
		auto& new_tag_state = ws.w.culture_s.national_tags_state[new_tag];

		auto& ntag = ws.w.nation_s.nations.get<nation::tag>(this_nation);
		if(is_valid_index(ntag))
			ws.w.culture_s.national_tags_state[ntag].holder = new_tag_state.holder;

		if(new_tag_state.holder)
			ws.w.nation_s.nations.set<nation::tag>(new_tag_state.holder, ntag);

		new_tag_state.holder = this_nation;
		ntag = new_tag;
	}

	void civilize_nation(world_state& ws, nations::country_tag this_nation) {
		ws.w.nation_s.nations.set<nation::is_civilized>(this_nation, true);

		auto issue_opts = ws.w.nation_s.active_issue_options.get_row(this_nation);
		for(int32_t i = int32_t(ws.s.issues_m.issues_container.size()); i--; ) {
			issues::issue_tag this_issue_tag(static_cast<issues::issue_tag::value_base_t>(i));
			auto& this_issue = ws.s.issues_m.issues_container[this_issue_tag];
			if(this_issue.type == issues::issue_group::military || this_issue.type == issues::issue_group::economic) {
				issue_opts[i] = issues::option_tag();
			}
		}

		events::fire_event_from_list(ws, ws.s.event_m.on_civilize, this_nation, std::monostate());
	}

	void perform_nationalization(world_state& ws, nations::country_tag this_nation) {
		auto influencers_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(this_nation));
		for(auto n : influencers_range) {
			auto fr = find(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(n), nations::influence{0.0f, this_nation, 0ui8, 0i8});
			if(fr->investment_amount != 0.0f) {
				fr->investment_amount = 0.0f;
				events::fire_event_from_list(ws, ws.s.event_m.on_my_factories_nationalized, n, this_nation);
			}
		}
	}

	void uncivilize_nation(world_state& ws, nations::country_tag this_nation) {
		ws.w.nation_s.nations.set<nation::is_civilized>(this_nation, false);

		auto issue_opts = ws.w.nation_s.active_issue_options.get_row(this_nation);
		for(int32_t i = int32_t(ws.s.issues_m.issues_container.size()); i--; ) {
			issues::issue_tag this_issue_tag(static_cast<issues::issue_tag::value_base_t>(i));
			auto& this_issue = ws.s.issues_m.issues_container[this_issue_tag];
			if(this_issue.type == issues::issue_group::military || this_issue.type == issues::issue_group::economic) {
				issue_opts[i] = this_issue.options[0];
			}
		}
	}

	void make_slave_state(world_state& ws, nations::state_tag this_state) {
		ws.w.nation_s.states.set<state::is_slave_state>(this_state, true);
	}
	void unmake_slave_state(world_state& ws, nations::state_tag this_state) {
		ws.w.nation_s.states.set<state::is_slave_state>(this_state, false);
		for_each_pop(ws, this_state, [&ws](population::pop_tag this_pop) {
			if(ws.w.population_s.pops.get<pop::type>(this_pop) == ws.s.population_m.slave)
				population::free_slave(ws, this_pop);
		});
	}

	void adjust_influence(world_state& ws, country_tag nation_by, nations::country_tag nation_target, int32_t amount) {
		auto current_influence = get_influence(ws, nation_by, nation_target);
		auto influence_result = int32_t(current_influence.amount) + amount;

		if(influence_result < 0) {
			auto decrease_cost = int32_t(ws.s.modifiers_m.global_defines.decreaseopinion_influence_cost);

			while(influence_result < 0 && current_influence.level != 0) {
				influence_result += decrease_cost;
				--current_influence.level;
			}

			influence_result = std::max(0, influence_result);

			set_influence(ws, nation_by, nation_target, influence_result, current_influence.level);
		} else if(influence_result > 100) {
			auto increase_cost = int32_t(ws.s.modifiers_m.global_defines.increaseopinion_influence_cost);
			auto sphere_cost = int32_t(ws.s.modifiers_m.global_defines.addtosphere_influence_cost);

			while(influence_result > 100 && current_influence.level < 4) {
				influence_result -= increase_cost;
				++current_influence.level;
			}
			if(current_influence.level == 4 && influence_result > 100 && influence_result >= sphere_cost) {
				current_influence.level = uint8_t(5);
				influence_result -= sphere_cost;
			}
			influence_result = std::min(100, influence_result);

			set_influence(ws, nation_by, nation_target, influence_result, current_influence.level);
		} else {
			set_influence_value(ws, nation_by, nation_target, influence_result);
		}
	}

	text_data::text_tag influence_level_to_text(world_state const& ws, int32_t i) {
		switch(i) {
			case 0:
				return ws.s.fixed_ui_text[scenario::fixed_ui::rel_hostile];
			case 1:
				return ws.s.fixed_ui_text[scenario::fixed_ui::rel_opposed];
			default:
			case 2:
				return ws.s.fixed_ui_text[scenario::fixed_ui::rel_neutral];
			case 3:
				return ws.s.fixed_ui_text[scenario::fixed_ui::rel_cordial];
			case 4:
				return ws.s.fixed_ui_text[scenario::fixed_ui::rel_friendly];
			case 5:
				return ws.s.fixed_ui_text[scenario::fixed_ui::rel_sphere];
		}
	}

	provinces::province_tag find_state_capital(world_state const& ws, nations::state_tag s) {
		if(auto rid = ws.w.nation_s.states.get<state::region_id>(s); is_valid_index(rid)) {
			auto prange = ws.s.province_m.states_to_province_index.get_row(rid);
			for(auto p : prange) {
				if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == s)
					return p;
			}
		}
		return provinces::province_tag();
	}
	provinces::province_tag state_port_province(world_state const & ws, nations::state_tag s) {
		provinces::province_tag port;
		nations::for_each_province(ws, s, [&port, &ws](provinces::province_tag p) {
			if(ws.w.province_s.province_state_container.get<province_state::naval_base_level>(p) > 0
				|| ws.w.province_s.province_state_container.get<province_state::naval_base_upgrade_progress>(p) != 0)
				port = p;
		});
		return port;
	}
	provinces::province_tag get_state_capital(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.states.get<state::state_capital>(s);
	}

	bool are_states_physically_neighbors(world_state const& ws, nations::state_tag a, nations::state_tag b) {
		auto prange = ws.s.province_m.states_to_province_index.get_row(ws.w.nation_s.states.get<state::region_id>(a));
		for(auto p : prange) {
			if(ws.w.province_s.province_state_container.get<province_state::state_instance>(p) == a) {
				auto prov_adj = ws.s.province_m.same_type_adjacency.get_row(p);
				for(auto ip : prov_adj) {
					if(ws.w.province_s.province_state_container.get<province_state::state_instance>(ip) == b)
						return true;
				}
			}
		}
		return false;
	}
	
	float tarrif_multiplier(world_state const& ws, nations::country_tag source, nations::country_tag target) {
		if(target == source || target == ws.w.nation_s.nations.get<nation::overlord>(source) || target == ws.w.nation_s.nations.get<nation::sphere_leader>(source))
			return 0.0f;
		else
			return 1.0f;
	}
}
