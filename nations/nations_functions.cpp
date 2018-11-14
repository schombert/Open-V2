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
	void reset_nation(world_state& ws, nations::nation& new_nation) {
		if(is_valid_index(new_nation.tag))
			governments::get_best_parties_at_date(ws.w.nation_s.active_parties.get_row(new_nation.id), new_nation.tag, ws.w.current_date, ws.s);
	
		governments::reset_upper_house(ws, new_nation.id);
		issues::reset_active_issues(ws, new_nation.id);
		governments::update_current_rules(ws, new_nation);
		military::reset_unit_stats(ws, new_nation.id);

		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32>(
			ws.w.nation_s.national_stockpiles.get_row(new_nation.id),
			ws.s.economy_m.aligned_32_goods_count) =
			Eigen::Matrix<economy::goods_qnty_type, -1, 1>::Zero(ws.s.economy_m.aligned_32_goods_count);

		Eigen::Map<Eigen::Matrix<float, -1, 1>>(
			ws.w.nation_s.national_variables.get_row(new_nation.id),
			ws.s.variables_m.count_national_variables) =
			Eigen::Matrix<float, -1, 1>::Zero(ws.s.variables_m.count_national_variables);

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

	void destroy_nation(world_state& ws, nations::nation& new_nation) {
		clear(ws.w.culture_s.culture_arrays, new_nation.accepted_cultures);

		auto owned_range = get_range(ws.w.province_s.province_arrays, new_nation.owned_provinces);
		for(auto p : owned_range)
			ws.w.province_s.province_state_container[p].owner = nullptr;
		clear(ws.w.province_s.province_arrays, new_nation.owned_provinces);

		auto controlled_range = get_range(ws.w.province_s.province_arrays, new_nation.controlled_provinces);
		for(auto p : controlled_range)
			ws.w.province_s.province_state_container[p].controller = nullptr;
		clear(ws.w.province_s.province_arrays, new_nation.controlled_provinces);

		auto prange = get_range(ws.w.province_s.province_arrays, new_nation.naval_patrols);
		for(auto p : prange) {
			remove_item_if(ws.w.military_s.fleet_presence_arrays, ws.w.province_s.province_state_container[p].fleets,
				[id = new_nation.id](military::fleet_presence const& fp) { return fp.owner == id; });
		}
		clear(ws.w.province_s.province_arrays, new_nation.naval_patrols);

		auto vrange = get_range(ws.w.nation_s.nations_arrays, new_nation.vassals);
		for(auto v : vrange)
			ws.w.nation_s.nations[v].overlord = nullptr;
		clear(ws.w.nation_s.nations_arrays, new_nation.vassals);

		auto arange = get_range(ws.w.nation_s.nations_arrays, new_nation.allies);
		for(auto a : arange)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[a].allies, new_nation.id);
		clear(ws.w.nation_s.nations_arrays, new_nation.allies);

		auto srange = get_range(ws.w.nation_s.nations_arrays, new_nation.sphere_members);
		for(auto s : srange)
			ws.w.nation_s.nations[s].sphere_leader = nullptr;
		clear(ws.w.nation_s.nations_arrays, new_nation.sphere_members);

		auto nrange = get_range(ws.w.nation_s.nations_arrays, new_nation.neighboring_nations);
		for(auto n : nrange)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[n].neighboring_nations, new_nation.id);
		clear(ws.w.nation_s.nations_arrays, new_nation.neighboring_nations);

		if(new_nation.sphere_leader)
			remove_item(ws.w.nation_s.nations_arrays, new_nation.sphere_leader->sphere_members, new_nation.id);
		new_nation.sphere_leader = nullptr;

		if(new_nation.overlord)
			remove_item(ws.w.nation_s.nations_arrays, new_nation.overlord->vassals, new_nation.id);
		new_nation.overlord = nullptr;

		auto inf_range = get_range(ws.w.nation_s.influence_arrays, new_nation.gp_influence);
		for(auto i = inf_range.first; i != inf_range.second; ++i)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[i->target].influencers, new_nation.id);
		clear(ws.w.nation_s.influence_arrays, new_nation.gp_influence);

		auto rev_inf_range = get_range(ws.w.nation_s.nations_arrays, new_nation.influencers);
		for(auto i : rev_inf_range)
			remove_item(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations[i].gp_influence, influence{0.0f, new_nation.id, 0ui8, 0i8});
		clear(ws.w.nation_s.nations_arrays, new_nation.influencers);
		
		auto rel_range = get_range(ws.w.nation_s.relations_arrays, new_nation.relations);
		for(auto r = rel_range.first; r != rel_range.second; ++r)
			remove_item(ws.w.nation_s.relations_arrays, ws.w.nation_s.nations[r->tag].relations, relationship{ new_nation.id, 0i16 });
		clear(ws.w.nation_s.relations_arrays, new_nation.relations);

		auto truce_range = get_range(ws.w.nation_s.truce_arrays, new_nation.truces);
		for(auto t = truce_range.first; t != truce_range.second; ++t)
			remove_item(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations[t->tag].truces, truce{date_tag(), new_nation.id});
		clear(ws.w.nation_s.truce_arrays, new_nation.truces);
		
		clear(ws.w.economy_s.purchasing_arrays, new_nation.statewise_tarrif_mask);
		clear(ws.w.variable_s.national_flags_arrays, new_nation.national_flags);
		clear(ws.w.nation_s.static_modifier_arrays, new_nation.static_modifiers);
		clear(ws.w.nation_s.timed_modifier_arrays, new_nation.timed_modifiers);

		auto movements = get_range(ws.w.population_s.pop_movement_arrays, new_nation.active_movements);
		for(auto m : movements) {
			population::destroy_pop_movement(ws, ws.w.population_s.pop_movements[m]);
			ws.w.population_s.pop_movements.remove(m);
		}
		clear(ws.w.population_s.pop_movement_arrays, new_nation.active_movements);

		auto factions = get_range(ws.w.population_s.rebel_faction_arrays, new_nation.active_rebel_factions);
		for(auto rf : factions) {
			population::destroy_rebel_faction(ws, ws.w.population_s.rebel_factions[rf]);
			ws.w.population_s.rebel_factions.remove(rf);
		}
		clear(ws.w.population_s.rebel_faction_arrays, new_nation.active_rebel_factions);

		auto wally_range = get_range(ws.w.nation_s.nations_arrays, new_nation.allies_in_war);
		for(auto n : wally_range)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[n].allies_in_war, new_nation.id);
		clear(ws.w.nation_s.nations_arrays, new_nation.allies_in_war);

		auto wopp_range = get_range(ws.w.nation_s.nations_arrays, new_nation.opponents_in_war);
		for(auto n : wopp_range)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[n].opponents_in_war, new_nation.id);
		clear(ws.w.nation_s.nations_arrays, new_nation.opponents_in_war);

		auto war_range = get_range(ws.w.military_s.war_arrays, new_nation.wars_involved_in);
		for(auto w = war_range.first; w != war_range.second; ++w)
			military::silent_remove_from_war(ws, ws.w.military_s.wars[w->war_id], new_nation.id);
		clear(ws.w.military_s.war_arrays, new_nation.wars_involved_in);

		auto order_range = get_range(ws.w.military_s.orders_arrays, new_nation.active_orders);
		for(auto o : order_range) {
			military::partial_destroy_orders(ws, ws.w.military_s.army_orders_container[o]);
			ws.w.military_s.army_orders_container.remove(o);
		}
		clear(ws.w.military_s.orders_arrays, new_nation.active_orders);

		auto army_range = get_range(ws.w.military_s.army_arrays, new_nation.armies);
		for(auto a : army_range) {
			military::partial_destroy_army(ws, ws.w.military_s.armies[a]);
			ws.w.military_s.armies.remove(a);
		}
		clear(ws.w.military_s.army_arrays, new_nation.armies);

		auto fleet_range = get_range(ws.w.military_s.fleet_arrays, new_nation.fleets);
		for(auto f : fleet_range) {
			military::partial_destroy_fleet(ws, ws.w.military_s.fleets[f]);
			ws.w.military_s.fleets.remove(f);
		}
		clear(ws.w.military_s.fleet_arrays, new_nation.fleets);

		auto gen_range = get_range(ws.w.military_s.leader_arrays, new_nation.generals);
		for(auto g : gen_range)
			ws.w.military_s.leaders.remove(g);
		clear(ws.w.military_s.leader_arrays, new_nation.generals);

		auto adm_range = get_range(ws.w.military_s.leader_arrays, new_nation.admirals);
		for(auto a : adm_range)
			ws.w.military_s.leaders.remove(a);
		clear(ws.w.military_s.leader_arrays, new_nation.admirals);

		auto focus_range = get_range(ws.w.nation_s.state_tag_arrays, new_nation.national_focus_locations);
		for(auto s : focus_range)
			remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.states[s].flashpoint_tension_focuses, new_nation.id);
		clear(ws.w.nation_s.state_tag_arrays, new_nation.national_focus_locations);

		auto ms_range = get_range(ws.w.nation_s.state_arrays, new_nation.member_states);
		for(auto s = ms_range.first; s != ms_range.second; ++s) {
			partial_destroy_state_instance(ws, *(s->state));
			ws.w.nation_s.states.remove(s->state->id);
		}
		clear(ws.w.nation_s.state_arrays, new_nation.member_states);
	}


	void annex_nation(world_state& ws, nations::nation& this_nation, nations::nation& to_annex) {
		boost::container::small_vector<provinces::province_tag, 64> owned_copy;
		auto target_owned = get_range(ws.w.province_s.province_arrays, to_annex.owned_provinces);
		for(auto p : target_owned)
			owned_copy.push_back(p);

		for(auto p : owned_copy) {
			auto& state = ws.w.province_s.province_state_container[p];
			provinces::silent_set_province_owner(ws, this_nation, state);
			provinces::silent_on_conquer_province(ws, state);
		}

		destroy_nation(ws, to_annex);
	}

	nations::nation& liberate_uncored_cores(world_state& ws, nations::nation& from, cultures::national_tag t) {
		if(from.tag == t)
			return from;

		auto liberation_target = make_nation_for_tag(ws, t);
		
		boost::container::small_vector<provinces::province_tag, 64> owned_copy;
		auto target_owned = get_range(ws.w.province_s.province_arrays, from.owned_provinces);
		for(auto p : target_owned)
			owned_copy.push_back(p);

		for(auto p : owned_copy) {
			auto& state = ws.w.province_s.province_state_container[p];
			if(contains_item(ws.w.province_s.core_arrays, state.cores, t) && !contains_item(ws.w.province_s.core_arrays, state.cores, from.tag)) {
				provinces::silent_set_province_owner(ws, *liberation_target, state);
				provinces::silent_on_conquer_province(ws, state);
			}
		}

		if(get_size(ws.w.province_s.province_arrays, from.owned_provinces) == 0)
			destroy_nation(ws, from);

		return *liberation_target;
	}

	nations::nation& liberate_all_cores(world_state& ws, nations::nation& from, cultures::national_tag t) {
		if(from.tag == t)
			return from;

		auto liberation_target = make_nation_for_tag(ws, t);

		boost::container::small_vector<provinces::province_tag, 64> owned_copy;
		auto target_owned = get_range(ws.w.province_s.province_arrays, from.owned_provinces);
		for(auto p : target_owned)
			owned_copy.push_back(p);

		for(auto p : owned_copy) {
			auto& state = ws.w.province_s.province_state_container[p];
			if(contains_item(ws.w.province_s.core_arrays, state.cores, t)) {
				provinces::silent_set_province_owner(ws, *liberation_target, state);
				provinces::silent_on_conquer_province(ws, state);
			}
		}

		if(get_size(ws.w.province_s.province_arrays, from.owned_provinces) == 0)
			destroy_nation(ws, from);

		return *liberation_target;
	}

	void make_vassal(world_state& ws, nations::nation& overlord, nations::nation& vassal) {
		if(&overlord == &vassal || vassal.overlord == &overlord)
			return;

		if(overlord.overlord) {
			make_vassal(ws, *overlord.overlord, vassal);
		} else {
			if(vassal.overlord)
				free_vassal(ws, vassal);

			vassal.overlord = &overlord;
			add_item(ws.w.nation_s.nations_arrays, overlord.vassals, vassal.id);

			auto allies_range = get_range(ws.w.nation_s.nations_arrays, vassal.allies);
			for(auto a : allies_range)
				remove_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[a].allies, vassal.id);
			clear(ws.w.nation_s.nations_arrays, vassal.allies);

			auto war_range = get_range(ws.w.military_s.war_arrays, vassal.wars_involved_in);
			boost::container::small_vector<military::war_identifier, 8> wars_copy(war_range.first, war_range.second);
			for(auto w : wars_copy) {
				auto& this_war = ws.w.military_s.wars[w.war_id];
				if(!military::is_target_of_war_goal(ws, this_war, vassal.id))
					military::remove_from_war(ws, this_war, vassal.id);
			}

			auto overlord_war_range = get_range(ws.w.military_s.war_arrays, overlord.wars_involved_in);
			for(auto w = overlord_war_range.first; w != overlord_war_range.second; ++w)
				military::add_to_war(ws, ws.w.military_s.wars[w->war_id], w->is_attacker, vassal);

			governments::silent_set_government(ws, vassal, overlord.current_government);
		}
	}
	void free_vassal(world_state& ws, nations::nation& vassal) {
		if(!vassal.overlord)
			return;

		remove_item(ws.w.nation_s.nations_arrays, vassal.overlord->vassals, vassal.id);
		vassal.overlord = nullptr;
		vassal.flags &= ~nations::nation::is_substate;

		auto war_range = get_range(ws.w.military_s.war_arrays, vassal.wars_involved_in);
		boost::container::small_vector<military::war_identifier, 8> wars_copy(war_range.first, war_range.second);
		for(auto w : wars_copy) {
			auto& this_war = ws.w.military_s.wars[w.war_id];
			if(!military::is_target_of_war_goal(ws, this_war, vassal.id))
				military::remove_from_war(ws, this_war, vassal.id);
		}
	}

	nation* make_nation_for_tag(world_state& ws, cultures::national_tag nt) {
		cultures::national_tag_state& tag_state = ws.w.culture_s.national_tags_state[nt];
		if(tag_state.holder)
			return tag_state.holder;
		nation& new_nation = ws.w.nation_s.nations.get_new();

		auto& fixed_tag = ws.s.culture_m.national_tags[nt];

		tag_state.holder = &new_nation;
		new_nation.tag = nt;
		new_nation.name = fixed_tag.default_name.name;
		new_nation.adjective = fixed_tag.default_name.adjective;
		new_nation.flag = fixed_tag.base_flag;

		new_nation.current_color = fixed_tag.color;

		ws.w.nation_s.active_parties.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.nation_demographics.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.nation_colonial_demographics.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.upper_house.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_technologies.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_goods.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.collected_tariffs.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_issue_options.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.national_stockpiles.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.national_variables.ensure_capacity(to_index(new_nation.id) + 1);

		ws.w.nation_s.unit_stats.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.rebel_org_gain.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.production_adjustments.ensure_capacity(to_index(new_nation.id) + 1);

		reset_nation(ws, new_nation);

		return &new_nation;
	}

	bool is_state_empty(world_state const& ws, state_instance const& s) {
		for(auto prange = ws.s.province_m.states_to_province_index.get_row(s.region_id); prange.first != prange.second; ++prange.first) {
			if(ws.w.province_s.province_state_container[*prange.first].state_instance == &s)
				return false;
		}
		return true;
	}

	bool is_colonial_or_protectorate(state_instance const& s) {
		return (s.flags & (state_instance::is_protectorate | state_instance::is_colonial)) != 0;
	}

	void partial_destroy_state_instance(world_state& ws, state_instance& si) {
		auto ft_range = get_range(ws.w.nation_s.nations_arrays, si.flashpoint_tension_focuses);
		for(auto n : ft_range)
			remove_item(ws.w.nation_s.state_tag_arrays, ws.w.nation_s.nations[n].national_focus_locations, si.id);
		clear(ws.w.nation_s.nations_arrays, si.flashpoint_tension_focuses);

		for(auto prange = ws.s.province_m.states_to_province_index.get_row(si.region_id); prange.first != prange.second; ++prange.first) {
			if(ws.w.province_s.province_state_container[*prange.first].state_instance == &si)
				ws.w.province_s.province_state_container[*prange.first].state_instance = nullptr;
		}
	}

	void destroy_state_instance(world_state& ws, state_instance& si) {
		partial_destroy_state_instance(ws, si);
		if(si.owner) {
			remove_item(ws.w.nation_s.state_arrays, si.owner->member_states, region_state_pair{ si.region_id, nullptr });
			si.owner = nullptr;
		}
	}

	void remove_province_from_state(world_state& ws, provinces::province_state& p) {
		auto old_state = p.state_instance;
		p.state_instance = nullptr;
		if(old_state)
			old_state->state_capital = nations::find_state_capital(ws, *old_state);
	}

	state_instance& make_state(provinces::state_tag region, world_state& ws) {
		state_instance& new_state = ws.w.nation_s.states.get_new();
		new_state.region_id = region;
		new_state.name = ws.s.province_m.state_names[region];

		ws.w.nation_s.state_demographics.ensure_capacity(to_index(new_state.id) + 1);
		ws.w.nation_s.state_prices.ensure_capacity(to_index(new_state.id) + 1);
		ws.w.nation_s.state_production.ensure_capacity(to_index(new_state.id) + 1);
		ws.w.nation_s.state_demand.ensure_capacity(to_index(new_state.id) + 1);
		ws.w.nation_s.state_global_demand.ensure_capacity(to_index(new_state.id) + 1);
		ws.w.nation_s.state_purchases.ensure_capacity(to_index(new_state.id) + 1);

		auto prices = ws.w.nation_s.state_prices.get_row(new_state.id);
		for(economy::goods_tag::value_base_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			prices[i] = ws.s.economy_m.goods[economy::goods_tag(i)].base_price;
		}
		std::fill_n(prices + ws.s.economy_m.aligned_32_goods_count, ws.s.economy_m.aligned_32_goods_count, economy::money_qnty_type(0));

		return new_state;
	}

	void init_empty_states(world_state& ws) {
		for(int32_t i = int32_t(ws.w.province_s.province_state_container.size()) - 1; i > 0; --i) {
			const provinces::province_tag this_prov_id(static_cast<provinces::province_tag::value_base_t>(i));
			auto& this_province = ws.w.province_s.province_state_container[this_prov_id];
			auto state_id = ws.s.province_m.province_container[this_prov_id].state_id;

			if((this_province.owner == nullptr) &
				((ws.s.province_m.province_container[this_prov_id].flags & provinces::province::sea) == 0) &
				(this_province.state_instance == nullptr) &
				is_valid_index(state_id)) {

				this_province.state_instance = &make_state(state_id, ws);

				auto same_region_range = ws.s.province_m.states_to_province_index.get_row(this_province.state_instance->region_id);
				for(auto same_region_prov : same_region_range) {
					auto& other_province = ws.w.province_s.province_state_container[same_region_prov];
					if((other_province.owner == nullptr) &
						((ws.s.province_m.province_container[same_region_prov].flags & provinces::province::sea) == 0) &
						(other_province.state_instance == nullptr)) {
						other_province.state_instance = this_province.state_instance;
					}
				}
			}
		}
		auto state_max = ws.w.nation_s.states.minimum_continuous_size();
		auto aligned_state_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(state_max) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));
		ws.w.nation_s.nations.parallel_for_each([&ws, aligned_state_max](nations::nation& n) {
			resize(ws.w.economy_s.purchasing_arrays, n.statewise_tarrif_mask, aligned_state_max);
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
		ws.w.nation_s.state_production.reset(uint32_t(ws.s.economy_m.aligned_32_goods_count * 2));
		ws.w.nation_s.state_demand.reset(uint32_t(ws.s.economy_m.aligned_32_goods_count * 2));
		ws.w.nation_s.state_global_demand.reset(uint32_t(ws.s.economy_m.aligned_32_goods_count * 2));
		ws.w.nation_s.national_variables.reset(ws.s.variables_m.count_national_variables);
		ws.w.nation_s.state_purchases.reset(uint32_t(ws.s.economy_m.goods_count));

		ws.w.nation_s.unit_stats.reset(static_cast<uint32_t>(ws.s.military_m.unit_types.size()));
		ws.w.nation_s.rebel_org_gain.reset(static_cast<uint32_t>(ws.s.population_m.rebel_types.size()));
		ws.w.nation_s.production_adjustments.reset(ws.s.economy_m.goods_count * uint32_t(technologies::production_adjustment::production_adjustment_count));
	}

	void update_state_nation_demographics(world_state& ws) {
		const auto full_vector_size = population::aligned_32_demo_size(ws);

		ws.w.nation_s.nations.parallel_for_each([&ws, full_vector_size](nation& n) {
			Eigen::Map<Eigen::Matrix<float, -1, 1>, Eigen::AlignmentType::Aligned32> nation_demo(ws.w.nation_s.nation_demographics.get_row(n.id), full_vector_size);
			Eigen::Map<Eigen::Matrix<float, -1, 1>, Eigen::AlignmentType::Aligned32> nation_c_demo(ws.w.nation_s.nation_colonial_demographics.get_row(n.id), full_vector_size);
			nation_demo.setZero();
			nation_c_demo.setZero();

			const auto state_range = get_range(ws.w.nation_s.state_arrays, n.member_states);

			for(auto s = state_range.first; s != state_range.second; ++s) {
				Eigen::Map<Eigen::Matrix<int32_t, -1, 1>, Eigen::AlignmentType::Aligned32> state_demo(ws.w.nation_s.state_demographics.get_row(s->state->id), full_vector_size);

				state_demo.setZero();

				const auto p_in_region_range = ws.s.province_m.states_to_province_index.get_row(s->region_id);
				for(auto p = p_in_region_range.first; p != p_in_region_range.second; ++p) {
					if(ws.w.province_s.province_state_container[*p].owner == &n) {
						Eigen::Map<Eigen::Matrix<int32_t, -1, 1>, Eigen::AlignmentType::Aligned32> province_demo(ws.w.province_s.province_demographics.get_row(*p), full_vector_size);
						state_demo += province_demo;
					}
				}

				if(state_demo[to_index(population::total_population_tag)] != 0) {
					const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));
					auto max_culture_off = maximum_index(state_demo.data() + to_index(culture_offset), int32_t(ws.s.culture_m.count_cultures));
					s->state->dominant_culture = cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(max_culture_off));

					const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));
					auto max_religion_off = maximum_index(state_demo.data() + to_index(religion_offset), int32_t(ws.s.culture_m.count_religions));
					s->state->dominant_religion = cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(max_religion_off));

					const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));
					auto max_ideology_off = maximum_index(state_demo.data() + to_index(ideology_offset), int32_t(ws.s.ideologies_m.ideologies_count));
					s->state->dominant_ideology = ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(max_ideology_off));

					const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));
					auto max_opinion_off = maximum_index(state_demo.data() + to_index(options_offset), int32_t(ws.s.issues_m.tracked_options_count));
					s->state->dominant_issue = issues::option_tag(static_cast<value_base_of<issues::option_tag>>(max_opinion_off));
				}

				if(!nations::is_colonial_or_protectorate(*s->state))
					nation_demo += state_demo.cast<float>();
				else
					nation_c_demo += state_demo.cast<float>();
			}

			if(nation_demo[to_index(population::total_population_tag)] != 0) {
				const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));
				auto max_culture_off = maximum_index(nation_demo.data() + to_index(culture_offset), int32_t(ws.s.culture_m.count_cultures));
				n.dominant_culture = cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(max_culture_off));

				const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));
				auto max_religion_off = maximum_index(nation_demo.data() + to_index(religion_offset), int32_t(ws.s.culture_m.count_religions));
				n.dominant_religion = cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(max_religion_off));

				const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));
				auto max_ideology_off = maximum_index(nation_demo.data() + to_index(ideology_offset), int32_t(ws.s.ideologies_m.ideologies_count));
				n.dominant_ideology = ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(max_ideology_off));

				const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));
				auto max_opinion_off = maximum_index(nation_demo.data() + to_index(options_offset), int32_t(ws.s.issues_m.tracked_options_count));
				n.dominant_issue = issues::option_tag(static_cast<value_base_of<issues::option_tag>>(max_opinion_off));
			}

			n.political_interest_fraction = issues::calculate_political_interest(ws, nation_demo.data());
			n.social_interest_fraction = issues::calculate_social_interest(ws, nation_demo.data());
		});
	}

	void fix_capitals(world_state& ws) {
		ws.w.nation_s.nations.for_each([&ws](nations::nation& n) {
			if(!is_valid_index(n.current_capital) || ws.w.province_s.province_state_container[n.current_capital].owner != &n)
				n.current_capital = find_best_capital(ws, n);
		});
	}
	provinces::province_tag find_best_capital(world_state const& ws, nation const& owner) {
		if(is_valid_index(owner.tag)) {
			const auto tag_capital = ws.w.culture_s.national_tags_state[owner.tag].capital;
			if(is_valid_index(tag_capital) && ws.w.province_s.province_state_container[tag_capital].owner == &owner)
				return tag_capital;

			int32_t population = 0;
			provinces::province_tag best_province;

			const auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[owner.tag].core_provinces);
			for(auto c = core_range.first; c != core_range.second; ++c) {
				if(ws.w.province_s.province_state_container[*c].owner == &owner) {
					int32_t ppop = ws.w.province_s.province_demographics.get(*c, population::total_population_tag);
					if(ppop > population) {
						best_province = *c;
						population = ppop;
					}
				}
			}
			if(is_valid_index(best_province))
				return best_province;
		}
		
		int32_t population = 0;
		provinces::province_tag best_province;

		const auto owned_range = get_range(ws.w.province_s.province_arrays, owner.owned_provinces);
		for(auto c = owned_range.first; c != owned_range.second; ++c) {
			int32_t ppop = ws.w.province_s.province_demographics.get(*c, population::total_population_tag);
			if(ppop > population) {
				best_province = *c;
				population = ppop;
			}
		}

		return best_province;
	}

	void set_relationship(world_state& ws, nation& a, nation& b, int32_t value) {
		if(auto f = find(ws.w.nation_s.relations_arrays, a.relations, relationship{ b.id, 0i16 }); f)
			f->value = int16_t(value);
		else
			add_item(ws.w.nation_s.relations_arrays, a.relations, relationship{ b.id, int16_t(value) });

		if(auto f = find(ws.w.nation_s.relations_arrays, b.relations, relationship{ a.id, 0i16 }); f)
			f->value = int16_t(value);
		else
			add_item(ws.w.nation_s.relations_arrays, b.relations, relationship{ a.id, int16_t(value) });
	}
	void adjust_relationship(world_state& ws, nation& a, nation& b, int32_t value) {
		if(auto f = find(ws.w.nation_s.relations_arrays, a.relations, relationship{ b.id, 0i16 }); f)
			f->value = int16_t(std::clamp(value + f->value, -200, 200));
		else
			add_item(ws.w.nation_s.relations_arrays, a.relations, relationship{ b.id, int16_t(std::clamp(value, -200, 200)) });

		if(auto f = find(ws.w.nation_s.relations_arrays, b.relations, relationship{ a.id, 0i16 }); f)
			f->value = int16_t(std::clamp(value + f->value, -200, 200));
		else
			add_item(ws.w.nation_s.relations_arrays, b.relations, relationship{ a.id, int16_t(std::clamp(value, -200, 200)) });
	}
	int32_t get_relationship(world_state const& ws, nation const& a, country_tag b) {
		if(auto f = find(ws.w.nation_s.relations_arrays, a.relations, relationship{ b, 0i16 }); f)
			return f->value;
		return 0;
	}
	int32_t get_influence_value(world_state const& ws, nation const& a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->amount;
		return 0;
	}
	int32_t get_influence_level(world_state const& ws, nation const& a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->level;
		return 2;
	}
	nations::influence get_influence(world_state const& ws, nation const& nation_by, country_tag nation_over) {
		if(auto f = find(ws.w.nation_s.influence_arrays, nation_by.gp_influence, influence{ 0.0f, nation_over, 0ui8, 0i8 }); f)
			return *f;
		return nations::influence{0.0f, nation_over, 0ui8, 2i8 };
	}
	void set_influence(world_state& ws, nation& a, country_tag b, int32_t value, int32_t level) {
		auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 });
		if(f == nullptr) {
			add_item(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, uint8_t(0), int8_t(2) });
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[b].influencers, a.id);

			f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 });
		}

		f->amount = uint8_t(value);

		if(f->level == 5i8 && level != 5) {
			ws.w.nation_s.nations.get(b).sphere_leader = nullptr;
			remove_item(ws.w.nation_s.nations_arrays, a.sphere_members, b);
			f->level = int8_t(level);
		} else if(f->level != 5i8 && level == 5) {
			if(ws.w.nation_s.nations.get(b).sphere_leader == nullptr) {
				ws.w.nation_s.nations.get(b).sphere_leader = &a;
				add_item(ws.w.nation_s.nations_arrays, a.sphere_members, b);
				f->level = int8_t(5);
			} else {
				f->level = int8_t(4);
			}
		} else {
			f->level = int8_t(level);
		}
	}
	void set_influence_value(world_state& ws, nation& a, country_tag b, int32_t value) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f) {
			f->amount = uint8_t(value);
		} else {
			add_item(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, uint8_t(value), int8_t(2) });
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[b].influencers, a.id);
		}
	}
	float get_foreign_investment(world_state const& ws, nation const& a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->investment_amount;
		return 0.0f;
	}
	void set_foreign_investment(world_state& ws, nation& a, country_tag b, float value) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f) {
			f->investment_amount = value;
		} else {
			add_item(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ value, b, 0ui8, 0i8 });
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[b].influencers, a.id);
		}
	}
	void remove_investment_and_influence(world_state& ws, nation& nation_by, nation& nation_target) {
		remove_item(ws.w.nation_s.influence_arrays, nation_by.gp_influence, influence{ 0.0f, nation_target.id, 0ui8, 0i8 });
		remove_item(ws.w.nation_s.nations_arrays, nation_target.influencers, nation_by.id);
	}

	int32_t colonial_points_to_make_protectorate(world_state const&, state_instance const&) {
		return 0;
	}
	int32_t colonial_points_to_make_colony(world_state const&, state_instance const&) {
		return 0;
	}
	int32_t colonial_points_to_make_state(world_state const&, state_instance const&) {
		return 0;
	}
	int32_t free_colonial_points(world_state const&, nation const&) {
		return 0;
	}
	int32_t points_for_next_colonial_stage(world_state const&, nation const&, state_instance const&) {
		return 0;
	}

	text_data::text_tag get_nation_status_text(world_state const& ws, nation const& this_nation) {
		if((this_nation.flags & nation::is_civilized) == 0) {
			if(this_nation.modifier_values[modifiers::national_offsets::civilization_progress_modifier] > modifiers::value_type(0))
				return ws.s.fixed_ui_text[scenario::fixed_ui::partialy_civilized_nation];
			else
				return ws.s.fixed_ui_text[scenario::fixed_ui::uncivilized_nation];
		} else if(is_great_power(ws, this_nation))
			return ws.s.fixed_ui_text[scenario::fixed_ui::great_power];
		else if(this_nation.overall_rank <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
			return ws.s.fixed_ui_text[scenario::fixed_ui::secondary_power];
		else
			return ws.s.fixed_ui_text[scenario::fixed_ui::civilized_nation];
		
	}
	
	bool is_great_power(world_state const&, nation const& n) {
		return n.overall_rank <= 8i16;
	}

	void silent_make_alliance(world_state& ws, nation& a, nation& b) {
		add_item(ws.w.nation_s.nations_arrays, a.allies, b.id);
		add_item(ws.w.nation_s.nations_arrays, b.allies, a.id);
	}

	void make_alliance(world_state& ws, nation& a, nation& b) {
		silent_make_alliance(ws, a, b);
	}
	void end_alliance(world_state& ws, nation& a, nation& b) {
		remove_item(ws.w.nation_s.nations_arrays, a.allies, b.id);
		remove_item(ws.w.nation_s.nations_arrays, b.allies, a.id);
	}

	void silent_make_vassal(world_state& ws, nation& overlord, nation& vassal) {
		if(vassal.overlord == nullptr) {
			vassal.overlord = &overlord;
			add_item(ws.w.nation_s.nations_arrays, overlord.vassals, vassal.id);
		}
	}

	void silent_make_substate(world_state& ws, nation& overlord, nation& vassal) {
		silent_make_vassal(ws, overlord, vassal);
		vassal.flags |= nation::is_substate;
	}

	nation* union_holder_for(world_state const& ws, cultures::culture_tag pculture) {
		auto cgroup = ws.s.culture_m.culture_container[pculture].group;
		auto union_tag = ws.s.culture_m.culture_groups[cgroup].union_tag;
		if(is_valid_index(union_tag))
			return ws.w.culture_s.national_tags_state[union_tag].holder;
		else
			return nullptr;
	}

	cultures::national_tag union_tag_of(world_state const& ws, nation const& this_nation) {
		auto pculture = this_nation.primary_culture;
		if(is_valid_index(pculture)) {
			auto cgroup = ws.s.culture_m.culture_container[pculture].group;
			return ws.s.culture_m.culture_groups[cgroup].union_tag;
		}
		else
			return cultures::national_tag();
	}

	nation* union_holder_of(world_state const& ws, nation const& this_nation) {
		auto pculture = this_nation.primary_culture;
		if(is_valid_index(pculture))
			return union_holder_for(ws, pculture);
		else
			return nullptr;
	}

	economy::goods_qnty_type national_treasury(world_state const& ws, country_tag id) {
		return ws.w.nation_s.national_stockpiles.get(id, economy::money_good);
	}

	float fraction_of_cores_owned(world_state const& ws, nation const& this_nation) {
		auto tag = this_nation.tag;
		if(is_valid_index(tag)) {
			auto cores_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);
			float cores_owned_count = 0.0f;
			for(auto p : cores_range) {
				if(ws.w.province_s.province_state_container[p].owner == &this_nation)
					cores_owned_count += 1.0f;
			}
			return (cores_range.second != cores_range.first) ? (cores_owned_count / float(cores_range.second - cores_range.first)) : 1.0f;
		} else {
			return 1.0f;
		}
	}

	bool can_release_as_vassal(world_state const& ws, nation const& this_nation, cultures::national_tag vassal) {
		auto& vassal_tag_info = ws.w.culture_s.national_tags_state[vassal];

		if(vassal_tag_info.is_not_releasable)
			return false;

		auto current_holder = vassal_tag_info.holder;
		if(current_holder && 0 != get_size(ws.w.province_s.province_arrays, current_holder->owned_provinces))
			return false; // national already exists

		auto core_range = get_range(ws.w.province_s.province_arrays, vassal_tag_info.core_provinces);
		for(auto p : core_range) {
			if(contains_item(ws.w.province_s.province_arrays, this_nation.owned_provinces, p))
				return true;
		}
		return false;
	}

	float get_prestige(nations::nation const& n) {
		return n.base_prestige + n.tech_attributes[technologies::tech_offset::permanent_prestige];
	}
	int32_t get_colonial_points(nations::nation const& n) {
		return int32_t(n.base_colonial_points) + int32_t(n.tech_attributes[technologies::tech_offset::colonial_points]);
	}

	float calculate_state_administrative_efficiency(world_state const& ws, nations::state_instance const& this_state, float admin_requirement) {
		if(this_state.owner == nullptr)
			return 0.0f;

		int32_t count_non_core = 0;

		auto region_provs = ws.s.province_m.states_to_province_index.get_row(this_state.region_id);
		for(auto p : region_provs) {
			auto& ps = ws.w.province_s.province_state_container[p];
			if((ps.state_instance == &this_state) & ((ps.flags & provinces::province_state::has_owner_core) == 0))
				++count_non_core;
		}
		//stub
		return std::clamp(
			float(count_non_core) * ws.s.modifiers_m.global_defines.noncore_tax_penalty +
			ws.s.modifiers_m.global_defines.base_country_admin_efficiency + 
			this_state.owner->modifier_values[modifiers::national_offsets::administrative_efficiency_modifier] +
			float(ws.w.nation_s.state_demographics.get(this_state.id, population::to_demo_tag(ws, ws.s.population_m.bureaucrat))) /
			(float(ws.w.nation_s.state_demographics.get(this_state.id, population::total_population_tag)) * admin_requirement),
			0.05f, 1.0f);
	}

	float calculate_national_administrative_efficiency(world_state const& ws, nations::nation const& n) {
		auto b_amount = ws.w.nation_s.nation_demographics.get(n.id, population::to_demo_tag(ws, ws.s.population_m.bureaucrat));
		auto total_pop = ws.w.nation_s.nation_demographics.get(n.id, population::total_population_tag);

		if(total_pop == 0)
			return 0.0f;

		auto ratio_num = b_amount * (1.0f + n.modifier_values[modifiers::national_offsets::administrative_efficiency_modifier]) / total_pop;

		auto issues_range = ws.w.nation_s.active_issue_options.get_row(n.id);
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

	void update_neighbors(world_state& ws, nations::nation& this_nation) {
		resize(ws.w.nation_s.nations_arrays, this_nation.neighboring_nations, 0ui32);

		auto owned_range = get_range(ws.w.province_s.province_arrays, this_nation.owned_provinces);
		for(auto p : owned_range) {
			auto adj_range = ws.s.province_m.same_type_adjacency.get_row(p);
			for(auto a : adj_range) {
				auto& adj = ws.w.province_s.province_state_container[a];
				if(adj.owner != nullptr && adj.owner != &this_nation) {
					add_item(ws.w.nation_s.nations_arrays, this_nation.neighboring_nations, adj.owner->id);
				}
			}
		}
	}

	uint32_t calculate_blockaded_count(world_state const& ws, nations::nation const& this_nation) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, this_nation.owned_provinces);
		uint32_t total = 0ui32;

		for(auto p : owned_range) {
			auto& ps = ws.w.province_s.province_state_container[p];
			if((ps.flags & (provinces::province_state::is_overseas | provinces::province_state::is_blockaded)) == provinces::province_state::is_blockaded)
				++total;
		}
		return total;
	}

	uint32_t calculate_crime_count(world_state const& ws, nations::nation const& this_nation) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, this_nation.owned_provinces);
		uint32_t total = 0ui32;

		for(auto p : owned_range) {
			auto& ps = ws.w.province_s.province_state_container[p];
			if((ps.flags & provinces::province_state::is_overseas) == 0 && is_valid_index(ps.crime))
				++total;
		}
		return total;
	}

	uint32_t calculate_rebel_controlled_count(world_state const& ws, nations::nation const& this_nation) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, this_nation.owned_provinces);
		uint32_t total = 0ui32;

		for(auto p : owned_range) {
			auto& ps = ws.w.province_s.province_state_container[p];
			if((ps.flags & provinces::province_state::is_overseas) == 0 && bool(ps.rebel_controller))
				++total;
		}
		return total;
	}

	void update_province_counts(world_state& ws, nations::nation& this_nation) {
		this_nation.central_province_count = 0ui16;
		this_nation.blockaded_count = 0ui16;
		this_nation.crime_count = 0ui16;
		this_nation.rebel_controlled_provinces = 0ui16;
		this_nation.num_ports = 0ui16;
		this_nation.num_connected_ports = 0ui16;

		if(!is_valid_index(this_nation.current_capital))
			return;

		auto owned_range = get_range(ws.w.province_s.province_arrays, this_nation.owned_provinces);
		for(auto p : owned_range) {
			auto& ps = ws.w.province_s.province_state_container[p];
			ps.flags &= ~provinces::province_state::is_overseas;
			if(ps.naval_base_level != 0)
				++this_nation.num_ports;
		}

		boost::container::flat_set<provinces::province_tag> already_added;
		boost::container::flat_set<provinces::province_tag> pending_neighbor_check;

		pending_neighbor_check.insert(this_nation.current_capital);

		while(pending_neighbor_check.size() != 0) {
			auto tp = *(pending_neighbor_check.end() - 1);
			pending_neighbor_check.erase(pending_neighbor_check.end() - 1);
			already_added.insert(tp);

			auto& ps = ws.w.province_s.province_state_container[tp];
			++this_nation.central_province_count;
			if(ps.naval_base_level != 0)
				++this_nation.num_connected_ports;
			if(ps.rebel_controller)
				++this_nation.rebel_controlled_provinces;
			if(is_valid_index(ps.crime))
				++this_nation.crime_count;
			if((ps.flags & provinces::province_state::is_blockaded) != 0)
				++this_nation.blockaded_count;

			auto adj_range = ws.s.province_m.same_type_adjacency.get_row(tp);
			for(auto a : adj_range) {
				if(ws.w.province_s.province_state_container[a].owner == &this_nation && already_added.count(a) == 0 && pending_neighbor_check.count(a) == 0)
					pending_neighbor_check.insert(a);
			}
		}
	}

	void update_movement_support(world_state& ws, nations::nation& this_nation) {
		float total_pop = float(ws.w.nation_s.nation_demographics.get(this_nation.id, population::total_population_tag));
		if(total_pop == 0.0f) {
			this_nation.social_movement_support = 0.0f;
			this_nation.political_movement_support = 0.0f;
			return;
		}

		auto movements = get_range(ws.w.population_s.pop_movement_arrays, this_nation.active_movements);
		int64_t total_social_support = 0;
		int64_t total_political_support = 0;
		for(auto m : movements) {
			if(ws.w.population_s.pop_movements[m].type == population::movement_type::political) {
				total_political_support += ws.w.population_s.pop_movements[m].total_population_support;
			} else if(ws.w.population_s.pop_movements[m].type == population::movement_type::social) {
				total_social_support += ws.w.population_s.pop_movements[m].total_population_support;
			}
		}

		
		this_nation.social_movement_support = std::min(1.0f, (float(total_social_support) / total_pop) * ws.s.modifiers_m.global_defines.movement_support_uh_factor);
		this_nation.political_movement_support = std::min(1.0f, (float(total_political_support) / total_pop) * ws.s.modifiers_m.global_defines.movement_support_uh_factor);
	}

	int32_t calculate_industrial_score(world_state const& ws, nations::nation const& this_nation) {
		float total = 0;
		auto states = get_range(ws.w.nation_s.state_arrays, this_nation.member_states);

		boost::container::small_vector<population::demo_tag, 8> factory_demo_tags;
		boost::container::small_vector<population::demo_tag, 8> factory_employment_tags;
		for(auto ct : ws.s.population_m.factory_workers) {
			factory_demo_tags.push_back(population::to_demo_tag(ws, ct));
			factory_employment_tags.push_back(population::to_employment_demo_tag(ws, ct));
		}

		for(auto s = states.first; s != states.second; ++s) {
			auto state_worker_pop = std::accumulate(factory_demo_tags.begin(), factory_demo_tags.end(), 0.0f,
				[&ws, sid = s->state->id](float sum, population::demo_tag t) { return sum + float(ws.w.nation_s.state_demographics.get(sid, t)); });
			if(state_worker_pop != 0.0f) {
				auto state_worker_employed = std::accumulate(factory_employment_tags.begin(), factory_employment_tags.end(), 0.0f,
					[&ws, sid = s->state->id](float sum, population::demo_tag t) { return sum + float(ws.w.nation_s.state_demographics.get(sid, t)); });

				int32_t total_flevels_x_4 = 0;
				for(uint32_t i = 0; i < std::extent_v<decltype(s->state->factories)>; ++i) {
					total_flevels_x_4 += 4 * s->state->factories[i].level;
				}

				total += float(total_flevels_x_4 * state_worker_employed) / state_worker_pop;
			}
		}

		auto investment = get_range(ws.w.nation_s.influence_arrays, this_nation.gp_influence);
		for(auto v = investment.first; v != investment.second; ++v) {
			total += v->investment_amount * ws.s.modifiers_m.global_defines.investment_score_factor * 0.01f;
		}
		return int32_t(total);
	}

	int32_t calculate_military_score(world_state const& ws, nations::nation const& this_nation) {
		float* unit_type_scores = (float*)_alloca(sizeof(float) * ws.s.military_m.unit_types_count);
		std::fill_n(unit_type_scores, ws.s.military_m.unit_types_count, 0.0f);

		auto unit_attributes_row = ws.w.nation_s.unit_stats.get_row(this_nation.id);
		float land_unit_type_count = 0.0f;
		float land_uint_score = 0.0f;

		for(uint32_t i = 0; i < ws.s.military_m.unit_types_count; ++i) {
			military::unit_type_tag this_tag(static_cast<military::unit_type_tag::value_base_t>(i));
			auto masked_type = ws.s.military_m.unit_types[this_tag].flags & military::unit_type::class_mask;
			if(masked_type == military::unit_type::class_big_ship) {
				// capital ship
				unit_type_scores[i] =
					(unit_attributes_row[i][military::unit_attribute::hull] + this_nation.modifier_values[modifiers::national_offsets::naval_defense_modifier]) *
					(unit_attributes_row[i][military::unit_attribute::gun_power] + this_nation.modifier_values[modifiers::national_offsets::naval_attack_modifier]) /
					250.0f;
			} else if(masked_type == military::unit_type::class_light_ship || masked_type == military::unit_type::class_transport) {
				// other naval
			} else {
				// land
				land_uint_score +=
					(unit_attributes_row[i][military::unit_attribute::attack] +
					this_nation.modifier_values[modifiers::national_offsets::land_attack_modifier] +
					unit_attributes_row[i][military::unit_attribute::defense] +
					this_nation.modifier_values[modifiers::national_offsets::land_defense_modifier]) * 
					unit_attributes_row[i][military::unit_attribute::discipline];
				++land_unit_type_count;
			}
		}
		bool is_disarmed = is_valid_index(this_nation.disarmed_until) && ws.w.current_date < this_nation.disarmed_until;

		float total_active_regiments = 0.0f;
		auto armies = get_range(ws.w.military_s.army_arrays, this_nation.armies);
		for(auto a : armies)
			total_active_regiments += float(ws.w.military_s.armies[a].total_soldiers);
		total_active_regiments /= ws.s.modifiers_m.global_defines.pop_size_per_regiment;

		float total_possible_regiments =
			(ws.w.nation_s.nation_demographics.get(this_nation.id, population::to_demo_tag(ws, ws.s.population_m.soldier)) + 
				ws.w.nation_s.nation_colonial_demographics.get(this_nation.id, population::to_demo_tag(ws, ws.s.population_m.soldier))) /
			ws.s.modifiers_m.global_defines.pop_size_per_regiment;

		float total_sum = ((land_uint_score / land_unit_type_count) *
			(is_disarmed ? ws.s.modifiers_m.global_defines.disarmament_army_hit : 1.0f) *
			std::max(0.0f, (this_nation.modifier_values[modifiers::national_offsets::supply_consumption] + 1.0f)) *
			std::min(total_active_regiments * 4.0f, total_possible_regiments)) / 7.0f; // = land contribution

		auto fleets = get_range(ws.w.military_s.fleet_arrays, this_nation.fleets);
		for(auto f : fleets) {
			auto ships = get_range(ws.w.military_s.ship_arrays, ws.w.military_s.fleets[f].ships);
			for(auto s = ships.first; s != ships.second; ++s) {
				total_sum += unit_type_scores[to_index(s->type)];
			}
		}

		total_sum += std::min(float(get_size(ws.w.military_s.leader_arrays, this_nation.admirals) + get_size(ws.w.military_s.leader_arrays, this_nation.generals)), total_active_regiments);

		return int32_t(total_sum);
	}

	void update_nation_ranks(world_state& ws) {
		uint32_t nations_count = 0;
		ws.w.nation_s.nations.for_each([&nations_count](nations::nation const& n) {
			++nations_count;
		});

		nations::nation** temp_buf = (nations::nation**)_alloca(sizeof(nations::nation*) * nations_count);

		ws.w.nation_s.nations.for_each([temp_buf, temp_count = 0](nations::nation& n) mutable {
			temp_buf[temp_count] = &n;
			++temp_count;
		});

		std::sort(temp_buf, temp_buf + nations_count, [](nations::nation const* a, nations::nation const* b) {
			return a->industrial_score > b->industrial_score;
		});
		for(uint32_t i = 0; i < nations_count; ++i)
			temp_buf[i]->industrial_rank = int16_t(i + 1);
		std::sort(temp_buf, temp_buf + nations_count, [](nations::nation const* a, nations::nation const* b) {
			return a->military_score > b->military_score;
		});
		for(uint32_t i = 0; i < nations_count; ++i)
			temp_buf[i]->military_rank = int16_t(i + 1);
		std::sort(temp_buf, temp_buf + nations_count, [](nations::nation const* a, nations::nation const* b) {
			return nations::get_prestige(*a) > nations::get_prestige(*b);
		});
		for(uint32_t i = 0; i < nations_count; ++i)
			temp_buf[i]->prestige_rank = int16_t(i + 1);

		std::sort(temp_buf, temp_buf + nations_count, [](nations::nation const* a, nations::nation const* b) {
			return (nations::get_prestige(*a) + a->military_score + a->industrial_score) > (nations::get_prestige(*b) + b->military_score + b->industrial_score);
		});
		std::stable_partition(temp_buf, temp_buf + nations_count, [](nations::nation const* a) { return a->overlord == nullptr; });
		std::stable_partition(temp_buf, temp_buf + nations_count, [](nations::nation const* a) { return (a->flags & nations::nation::is_civilized) != 0; });
		std::stable_partition(temp_buf, temp_buf + nations_count, [](nations::nation const* a) { return is_valid_index(a->current_capital); });

		resize(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank, nations_count);
		for(uint32_t i = 0; i < nations_count; ++i) {
			temp_buf[i]->overall_rank = int16_t(i + 1);
			get(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank, i) = temp_buf[i]->id;
		}
	}

	void change_tag(world_state& ws, nations::nation& this_nation, cultures::national_tag new_tag) {
		auto& new_tag_state = ws.w.culture_s.national_tags_state[new_tag];

		if(is_valid_index(this_nation.tag))
			ws.w.culture_s.national_tags_state[this_nation.tag].holder = new_tag_state.holder;

		if(new_tag_state.holder)
			new_tag_state.holder->tag = this_nation.tag;

		new_tag_state.holder = &this_nation;
		this_nation.tag = new_tag;
	}

	void civilize_nation(world_state& ws, nations::nation& this_nation) {
		this_nation.flags |= nations::nation::is_civilized;

		auto issue_opts = ws.w.nation_s.active_issue_options.get_row(this_nation.id);
		for(int32_t i = int32_t(ws.s.issues_m.issues_container.size()); i--; ) {
			issues::issue_tag this_issue_tag(static_cast<issues::issue_tag::value_base_t>(i));
			auto& this_issue = ws.s.issues_m.issues_container[this_issue_tag];
			if(this_issue.type == issues::issue_group::military || this_issue.type == issues::issue_group::economic) {
				issue_opts[i] = issues::option_tag();
			}
		}

		events::fire_event_from_list(ws, ws.s.event_m.on_civilize, this_nation.id, std::monostate());
	}

	void perform_nationalization(world_state& ws, nations::nation& this_nation) {
		auto influencers_range = get_range(ws.w.nation_s.nations_arrays, this_nation.influencers);
		for(auto n : influencers_range) {
			auto fr = find(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations[n].gp_influence, nations::influence{0.0f, this_nation.id, 0ui8, 0i8});
			if(fr->investment_amount != 0.0f) {
				fr->investment_amount = 0.0f;
				events::fire_event_from_list(ws, ws.s.event_m.on_my_factories_nationalized, n, this_nation.id);
			}
		}
	}

	void uncivilize_nation(world_state& ws, nations::nation& this_nation) {
		this_nation.flags &= ~nations::nation::is_civilized;

		auto issue_opts = ws.w.nation_s.active_issue_options.get_row(this_nation.id);
		for(int32_t i = int32_t(ws.s.issues_m.issues_container.size()); i--; ) {
			issues::issue_tag this_issue_tag(static_cast<issues::issue_tag::value_base_t>(i));
			auto& this_issue = ws.s.issues_m.issues_container[this_issue_tag];
			if(this_issue.type == issues::issue_group::military || this_issue.type == issues::issue_group::economic) {
				issue_opts[i] = this_issue.options[0];
			}
		}
	}

	void make_slave_state(world_state&, nations::state_instance& this_state) {
		this_state.flags |= nations::state_instance::is_slave_state;
	}
	void unmake_slave_state(world_state& ws, nations::state_instance& this_state) {
		this_state.flags &= ~nations::state_instance::is_slave_state;
		for_each_pop(ws, this_state, [&ws](population::pop& this_pop) {
			if(this_pop.type == ws.s.population_m.slave)
				population::free_slave(ws, this_pop);
		});
	}

	void adjust_influence(world_state& ws, nation& nation_by, nations::country_tag nation_target, int32_t amount) {
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

	provinces::province_tag find_state_capital(world_state const& ws, nations::state_instance const& s) {
		if(auto rid = s.region_id; is_valid_index(rid)) {
			auto prange = ws.s.province_m.states_to_province_index.get_row(rid);
			for(auto p : prange) {
				auto& ps = ws.w.province_s.province_state_container[p];
				if(ps.state_instance == &s)
					return ps.id;
			}
		}
		return provinces::province_tag();
	}
	provinces::province_state const* get_state_capital(world_state const& ws, nations::state_instance const& s) {
		if(auto cap = s.state_capital; is_valid_index(cap))
			return &ws.w.province_s.province_state_container[cap];
		else
			return nullptr;
	}

	provinces::province_state* get_state_capital(world_state& ws, nations::state_instance& s) {
		if(auto cap = s.state_capital; is_valid_index(cap))
			return &ws.w.province_s.province_state_container[cap];
		else
			return nullptr;
	}

	bool are_states_physically_neighbors(world_state const& ws, nations::state_instance const& a, nations::state_instance const& b) {
		auto prange = ws.s.province_m.states_to_province_index.get_row(a.region_id);
		for(auto p : prange) {
			auto& ps = ws.w.province_s.province_state_container[p];
			if(ps.state_instance == &a) {
				auto prov_adj = ws.s.province_m.same_type_adjacency.get_row(p);
				for(auto ip : prov_adj) {
					if(ws.w.province_s.province_state_container[ip].state_instance == &b)
						return true;
				}
			}
		}
		return false;
	}
	
	float tarrif_multiplier(world_state const&, nations::nation const& source, nations::nation const& target) {
		if(&target == &source || &target == source.overlord || &target == source.sphere_leader)
			return 0.0f;
		else
			return 1.0f;
	}
}
