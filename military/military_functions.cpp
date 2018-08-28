#include "common\\common.h"
#include "military_functions.h"
#include "world_state\\world_state.h"
#include <random>

namespace military {
	void init_military_state(world_state& ws) {
		if(ws.w.military_s.army_supplies.inner_size != ws.s.economy_m.aligned_32_goods_count)
			ws.w.military_s.army_supplies.reset(ws.s.economy_m.aligned_32_goods_count);
		if(ws.w.military_s.fleet_supplies.inner_size != ws.s.economy_m.aligned_32_goods_count)
			ws.w.military_s.fleet_supplies.reset(ws.s.economy_m.aligned_32_goods_count);
		if(ws.w.military_s.unit_type_composition.inner_size != uint32_t(ws.s.military_m.unit_types.size()))
			ws.w.military_s.unit_type_composition.reset(uint32_t(ws.s.military_m.unit_types.size()));
	}

	void reset_state(military_state& s) {
		s.leader_arrays.reset();
		s.ship_arrays.reset();
		s.army_arrays.reset();
		s.orders_arrays.reset();
		s.fleet_arrays.reset();
		s.war_arrays.reset();
		s.war_goal_arrays.reset();
		s.fleet_presence_arrays.reset();
		s.naval_control_arrays.reset();
	}

	military_leader& make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general) {
		military_leader& new_leader = ws.w.military_s.leaders.get_new();
		auto& culture_obj = ws.s.culture_m.culture_container[culture];
		auto& cgroup = ws.s.culture_m.culture_groups[culture_obj.group];

		auto& local_generator = get_local_generator();

		if(is_general) {
			std::uniform_int_distribution<int32_t> r(0, int32_t(cgroup.leader_pictures.general_size) - 1);
			new_leader.portrait = ws.s.culture_m.leader_pictures[size_t(cgroup.leader_pictures.general_offset + r(local_generator))];
		} else {
			std::uniform_int_distribution<int32_t> r(0, int32_t(cgroup.leader_pictures.admiral_size) - 1);
			new_leader.portrait = ws.s.culture_m.leader_pictures[size_t(cgroup.leader_pictures.admiral_offset + r(local_generator))];
		}

		auto first_name_range = ws.s.culture_m.first_names_by_culture.get_row(culture);
		auto last_name_range = ws.s.culture_m.first_names_by_culture.get_row(culture);

		std::uniform_int_distribution<int32_t> fn(0, int32_t(first_name_range.second - first_name_range.first) - 1);
		std::uniform_int_distribution<int32_t> ln(0, int32_t(last_name_range.second - last_name_range.first) - 1);

		new_leader.first_name = *(first_name_range.first + fn(local_generator));
		new_leader.last_name = *(last_name_range.first + ln(local_generator));

		return new_leader;
	}

	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date) {
		military_leader& new_leader = make_empty_leader(ws, culture, is_general);
		new_leader.creation_date = creation_date;
		
		std::uniform_int_distribution<int32_t> br(0, int32_t(ws.s.military_m.background_traits.size()) - 1);
		std::uniform_int_distribution<int32_t> pr(0, int32_t(ws.s.military_m.personality_traits.size()) - 1);

		auto& local_generator = get_local_generator();

		
		new_leader.background = ws.s.military_m.background_traits[size_t(br(local_generator))];
		new_leader.personality = ws.s.military_m.personality_traits[size_t(pr(local_generator))];

		calculate_leader_traits(ws, new_leader);

		return new_leader.id;
	}

	void calculate_leader_traits(world_state& ws, military_leader& l) {
		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>> dest_vec(l.leader_traits);

		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>> source_a(ws.s.military_m.leader_trait_definitions.get_row(l.background));
		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>> source_b(ws.s.military_m.leader_trait_definitions.get_row(l.personality));

		dest_vec = source_a + source_b;
	}

	void reset_unit_stats(world_state& ws, nations::country_tag nation_for) {
		auto unit_type_count = ws.s.military_m.unit_types_count;
		for(uint32_t i = 0; i < unit_type_count; ++i) {
			ws.w.nation_s.unit_stats.get(nation_for, unit_type_tag(static_cast<unit_type_tag::value_base_t>(i))) = ws.s.military_m.unit_types[unit_type_tag(static_cast<unit_type_tag::value_base_t>(i))].base_attributes;
		}
	}

	void update_army_attributes(world_state& ws, nations::nation const& owning_nation, army& this_army) {
		uint16_t* count = ws.w.military_s.unit_type_composition.get_row(this_army.id);
		auto nation_id = owning_nation.id;

		this_army.total_attributes = unit_attribute_vector::Zero();
		auto unit_type_count = ws.s.military_m.unit_types_count;
		for(uint32_t i = to_index(naval_unit_base) + 1ui32; i < unit_type_count; ++i) {
			this_army.total_attributes += ws.w.nation_s.unit_stats.get(nation_id, unit_type_tag(static_cast<unit_type_tag::value_base_t>(i))) * unit_attribute_type(count[i]);
		}
	}

	void update_fleet_attributes(world_state& ws, nations::nation const& owning_nation, fleet& this_fleet) {
		auto nation_id = owning_nation.id;

		this_fleet.total_attributes = unit_attribute_vector::Zero();

		auto ship_range = get_range(ws.w.military_s.ship_arrays, this_fleet.ships);
		for(auto s = ship_range.first; s != ship_range.second; ++s) {
			this_fleet.total_attributes += ws.w.nation_s.unit_stats.get(nation_id, s->type);
		}
	}

	void update_all_unit_attributes(world_state& ws, nations::nation const& owning_nation) {
		auto armies_range = get_range(ws.w.military_s.army_arrays, owning_nation.armies);
		for(auto a : armies_range) {
			update_army_attributes(ws, owning_nation, ws.w.military_s.armies[a]);
		}
		auto fleets_range = get_range(ws.w.military_s.fleet_arrays, owning_nation.fleets);
		for(auto f : fleets_range) {
			update_fleet_attributes(ws, owning_nation, ws.w.military_s.fleets[f]);
		}
	}

	army& make_army(world_state& ws, nations::nation& n, provinces::province_tag location) {
		army& new_army = ws.w.military_s.armies.get_new();
		new_army.base = location;
		new_army.owner = n.id;
		add_item(ws.w.military_s.army_arrays, n.armies, new_army.id);
		ws.w.military_s.army_supplies.ensure_capacity(to_index(new_army.id));
		ws.w.military_s.unit_type_composition.ensure_capacity(to_index(new_army.id));
		return new_army;
	}

	void immediate_add_pop_to_army(world_state& ws, army& target_army, population::pop& p) {
		target_army.total_soldiers += uint32_t(ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag));
		p.associated_army = target_army.id;
		add_item(ws.w.population_s.pop_arrays, target_army.backing_pops, p.id);
	}

	fleet& make_fleet(world_state& ws, nations::nation& n, provinces::province_tag location) {
		fleet& new_fleet = ws.w.military_s.fleets.get_new();
		new_fleet.base = location;
		add_item(ws.w.military_s.fleet_arrays, n.fleets , new_fleet.id);
		ws.w.military_s.fleet_supplies.ensure_capacity(to_index(new_fleet.id));
		return new_fleet;
	}

	bool in_war_with(world_state const& ws, nations::nation const& this_nation, nations::country_tag nation_with) {
		return contains_item(ws.w.nation_s.nations_arrays, this_nation.allies_in_war, nation_with);
	}

	bool in_war_against(world_state const& ws, nations::nation const& this_nation, nations::country_tag nation_against) {
		return contains_item(ws.w.nation_s.nations_arrays, this_nation.opponents_in_war, nation_against);
	}

	void update_at_war_with_and_against(world_state& ws, nations::nation& this_nation) {
		resize(ws.w.nation_s.nations_arrays, this_nation.allies_in_war, 0ui32);
		resize(ws.w.nation_s.nations_arrays, this_nation.opponents_in_war, 0ui32);

		auto owner_wars = get_range(ws.w.military_s.war_arrays, this_nation.wars_involved_in);
		for(auto iwar = owner_wars.first; iwar != owner_wars.second; ++iwar) {
			if(auto warid = iwar->war_id; ws.w.military_s.wars.is_valid_index(warid)) {
				if(iwar->is_attacker) {
					auto attacker_range = get_range(ws.w.nation_s.nations_arrays, ws.w.military_s.wars[warid].attackers);
					for(auto d : attacker_range)
						add_item(ws.w.nation_s.nations_arrays, this_nation.allies_in_war, d);
					auto defender_range = get_range(ws.w.nation_s.nations_arrays, ws.w.military_s.wars[warid].defenders);
					for(auto d : defender_range)
						add_item(ws.w.nation_s.nations_arrays, this_nation.opponents_in_war, d);
				} else {
					auto attacker_range = get_range(ws.w.nation_s.nations_arrays, ws.w.military_s.wars[warid].attackers);
					for(auto d : attacker_range)
						add_item(ws.w.nation_s.nations_arrays, this_nation.opponents_in_war, d);
					auto defender_range = get_range(ws.w.nation_s.nations_arrays, ws.w.military_s.wars[warid].defenders);
					for(auto d : defender_range)
						add_item(ws.w.nation_s.nations_arrays, this_nation.allies_in_war, d);
				}
			}
		}
	}

	bool can_use_cb_against(world_state& ws, nations::nation& nation_by, nations::nation& nation_target) {
		auto pending_range = get_range(ws.w.military_s.cb_arrays, nation_by.active_cbs);
		for(auto c = pending_range.first; c != pending_range.second; ++c) {
			if(c->target == nation_target.id)
				return true;
		}
		for(auto& c : ws.s.military_m.cb_types) {
			if((c.flags & cb_type::is_not_triggered_only) == 0 &&
				is_valid_index(c.can_use) &&
				triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(c.can_use), ws, &nation_target, &nation_by, nullptr, nullptr))
				return true;
		}
		return false;
	}

	bool has_units_in_province(world_state const& ws, nations::nation const& this_nation, provinces::province_state const& ps) {
		auto orders = ps.orders;
		if(orders) {
			auto involved_range = get_range(ws.w.military_s.army_arrays, orders->involved_armies);
			for(auto a : involved_range) {
				if(ws.w.military_s.armies[a].owner == this_nation.id)
					return true;
			}
		}
		return false;
	}

	uint32_t total_units_in_province(world_state const& ws, provinces::province_state const& ps) {
		auto orders = ps.orders;
		if(orders) {
			uint32_t total = 0ui32;
			const uint32_t regiment_size = uint32_t(ws.s.modifiers_m.global_defines.pop_size_per_regiment);

			auto province_size = get_size(ws.w.province_s.province_arrays, orders->involved_provinces);
			auto involved_range = get_range(ws.w.military_s.army_arrays, orders->involved_armies);

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

	bool province_is_contested(world_state const&, provinces::province_state const& ps) {
		return ps.siege_progress != 0.0f;
	}

	uint32_t total_active_divisions(world_state const& ws, nations::nation const& this_nation) {
		uint32_t total = 0ui32;
		const uint32_t regiment_size = uint32_t(ws.s.modifiers_m.global_defines.pop_size_per_regiment);

		auto army_range = get_range(ws.w.military_s.army_arrays, this_nation.armies);
		for(auto a : army_range) {
			if(is_valid_index(a)) {
				total += ws.w.military_s.armies[a].total_soldiers ;
			}
		}
		return (total + regiment_size - 1ui32) / regiment_size;
	}
	uint32_t total_active_ships(world_state const& ws, nations::nation const& this_nation) {
		uint32_t total = 0ui32;
		auto fleet_range = get_range(ws.w.military_s.fleet_arrays, this_nation.fleets);
		for(auto f : fleet_range) {
			if(is_valid_index(f))
				total += get_size(ws.w.military_s.ship_arrays, ws.w.military_s.fleets[f].ships);
		}
		return total;
	}
	float recruited_pop_fraction(world_state const& ws, nations::nation const& this_nation) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, this_nation.owned_provinces);

		uint32_t total_soldier_pops = 0ui32;
		uint32_t total_soldier_pops_assigned = 0ui32;

		for(auto p : owned_range) {
			if(is_valid_index(p)) {
				auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[p].pops);
				for(auto po : pop_range) {
					if(is_valid_index(po)) {
						auto& po_obj = ws.w.population_s.pops[po];
						if(po_obj.type == ws.s.population_m.soldier) {
							++total_soldier_pops;
							total_soldier_pops_assigned += is_valid_index(po_obj.associated_army) ? 1ui32 : 0ui32;
						}
					}
				}
			}
		}
		if(total_soldier_pops == 0)
			return 0.0f;
		else
			return float(total_soldier_pops_assigned) / float(total_soldier_pops);
	}

	bool has_named_leader(world_state const&, nations::nation const&, text_data::text_tag) {
		return true;
	}

	float get_warscore(world_state const&, war const& wr) {
		return wr.current_war_score;
	}

	bool has_military_access_with(world_state const& ws, nations::nation const& nation_by, nations::nation const& nation_target) {
		if((nation_by.overlord == &nation_target) | (nation_target.overlord == &nation_by))
			return true;
		if(in_war_with(ws, nation_by, nation_target.id))
			return true;
		return false;
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
				if(ws.w.nation_s.nations[n].overall_rank < best_rank) {
					best_rank = ws.w.nation_s.nations[n].overall_rank;
					best = n;
				}
			}

			this_war.primary_attacker = best;
		} else if(this_war.primary_defender == to_remove) {
			nations::country_tag best = nations::country_tag();
			int16_t best_rank = std::numeric_limits<int16_t>::max();

			auto defenders = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto n : defenders) {
				if(ws.w.nation_s.nations[n].overall_rank < best_rank) {
					best_rank = ws.w.nation_s.nations[n].overall_rank;
					best = n;
				}
			}

			this_war.primary_defender = best;
		}
	}

	void remove_from_war(world_state& ws, war& this_war, nations::country_tag to_remove) {
		silent_remove_from_war(ws, this_war, to_remove);
		remove_item_if(ws.w.military_s.war_arrays, ws.w.nation_s.nations[to_remove].wars_involved_in, war_identifier{ this_war.id, false });
	}
	void add_to_war(world_state& ws, war& this_war, bool attacker, nations::nation& to_add) {
		if(attacker) {
			auto attacker_range = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
			for(auto a : attacker_range) {
				add_item(ws.w.nation_s.nations_arrays, to_add.allies_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[a].allies_in_war, to_add.id);
			}
			auto defender_range = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto a : defender_range) {
				add_item(ws.w.nation_s.nations_arrays, to_add.opponents_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[a].opponents_in_war, to_add.id);
			}
			add_item(ws.w.nation_s.nations_arrays, this_war.attackers, to_add.id);
			add_item(ws.w.military_s.war_arrays, to_add.wars_involved_in, war_identifier{ this_war.id, true });
		} else {
			auto attacker_range = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
			for(auto a : attacker_range) {
				add_item(ws.w.nation_s.nations_arrays, to_add.opponents_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[a].opponents_in_war, to_add.id);
			}
			auto defender_range = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto a : defender_range) {
				add_item(ws.w.nation_s.nations_arrays, to_add.allies_in_war, a);
				add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[a].allies_in_war, to_add.id);
			}
			add_item(ws.w.nation_s.nations_arrays, this_war.defenders, to_add.id);
			add_item(ws.w.military_s.war_arrays, to_add.wars_involved_in, war_identifier{ this_war.id, false });
		}
	}

	bool is_target_of_war_goal(world_state const& ws, war const& this_war, nations::country_tag target) {
		auto wg_range = get_range(ws.w.military_s.war_goal_arrays, this_war.war_goals);
		for(auto wg = wg_range.first; wg != wg_range.second; ++wg) {
			if(wg->target_country == target)
				return true;
		}
		return false;
	}

	void destroy_army(world_state& ws, army& a, nations::nation& owner) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, a.backing_pops);
		for(auto p : pop_range)
			ws.w.population_s.pops[p].associated_army = army_tag();
		clear(ws.w.population_s.pop_arrays, a.backing_pops);

		remove_item(ws.w.military_s.army_arrays, owner.armies, a.id);
		if(a.current_orders) {
			remove_item(ws.w.military_s.army_arrays, a.current_orders->involved_armies, a.id);
			a.current_orders = nullptr;
		}
		if(a.leader) {
			a.leader->attached = false;
			a.leader = nullptr;
		}
	}

	void destroy_fleet(world_state& ws, fleet& f, nations::nation& owner) {
		clear(ws.w.military_s.ship_arrays, f.ships);
		remove_item(ws.w.military_s.fleet_arrays, owner.fleets, f.id);

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
			ws.w.population_s.pops[p].associated_army = army_tag();
		clear(ws.w.population_s.pop_arrays, a.backing_pops);

		if(a.leader) {
			a.leader->attached = false;
			a.leader = nullptr;
		}
	}

	void destroy_orders(world_state& ws, army_orders& o, nations::nation& owner) {
		if(o.leader) {
			o.leader->attached = false;
			o.leader = nullptr;
		}
		auto army_range = get_range(ws.w.military_s.army_arrays, o.involved_armies);
		for(auto a : army_range) 
			ws.w.military_s.armies[a].current_orders = nullptr;
		clear(ws.w.military_s.army_arrays, o.involved_armies);

		remove_item(ws.w.military_s.orders_arrays, owner.active_orders, o.id);

		auto prange = get_range(ws.w.province_s.province_arrays, o.involved_provinces);
		for(auto p : prange)
			ws.w.province_s.province_state_container[p].orders = nullptr;
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
			ws.w.province_s.province_state_container[p].orders = nullptr;
		clear(ws.w.province_s.province_arrays, o.involved_provinces);
	}

	void destroy_admiral(world_state& ws, military_leader& l, nations::nation& owner) {
		if(l.attached) {
			auto fr = get_range(ws.w.military_s.fleet_arrays, owner.fleets);
			for(auto fl : fr) {
				if(ws.w.military_s.fleets[fl].leader == &l)
					ws.w.military_s.fleets[fl].leader = nullptr;
			}
		}
	}

	void destroy_general(world_state& ws, military_leader& l, nations::nation& owner) {
		if(l.attached) {
			auto ar = get_range(ws.w.military_s.army_arrays, owner.armies);
			for(auto a : ar) {
				if(ws.w.military_s.armies[a].leader == &l)
					ws.w.military_s.armies[a].leader = nullptr;
			}

			auto order_range = get_range(ws.w.military_s.orders_arrays, owner.active_orders);
			for(auto o : order_range) {
				if(ws.w.military_s.army_orders_container[o].leader == &l)
					ws.w.military_s.army_orders_container[o].leader = nullptr;
			}
		}
	}

	uint32_t calculate_minimum_soldiers(world_state const& ws, army_tag a) {
		Eigen::Map<Eigen::Matrix<uint16_t, -1, 1>> composition(ws.w.military_s.unit_type_composition.get_row(a), ws.s.military_m.unit_types_count);
		auto count_brigades = composition.sum();
		return uint32_t(ws.s.modifiers_m.global_defines.pop_size_per_regiment) * count_brigades;
	}

	void rebuild_fleet_presence(world_state&, nations::nation&) {
		//stub
	}


	void destroy_war(world_state& ws, war& this_war) {
		auto attacker_range = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
		for(auto a : attacker_range)
			remove_item(ws.w.military_s.war_arrays, ws.w.nation_s.nations[a].wars_involved_in, military::war_identifier{ this_war.id, false });
		
		auto defender_range = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
		for(auto a : defender_range)
			remove_item(ws.w.military_s.war_arrays, ws.w.nation_s.nations[a].wars_involved_in, military::war_identifier{ this_war.id, false });

		clear(ws.w.nation_s.nations_arrays, this_war.attackers);
		clear(ws.w.nation_s.nations_arrays, this_war.defenders);
		clear(ws.w.military_s.naval_control_arrays, this_war.naval_control_set);
		clear(ws.w.military_s.war_goal_arrays, this_war.war_goals);
	}

	war* get_war_between(world_state& ws, nations::nation& a, nations::country_tag b) {
		auto wars = get_range(ws.w.military_s.war_arrays, a.wars_involved_in);
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

	war& create_war(world_state& ws, nations::nation& attacker, nations::nation& defender, bool call_willing_attacker_allies) {
		auto& new_war = ws.w.military_s.wars.get_new();
		new_war.primary_attacker = attacker.id;
		new_war.primary_defender = defender.id;
		add_item(ws.w.nation_s.nations_arrays, new_war.attackers, attacker.id);
		add_item(ws.w.nation_s.nations_arrays, new_war.defenders, defender.id);

		new_war.start_date = ws.w.current_date;
		new_war.last_update = ws.w.current_date;

		// todo: call defender allies
		// todo: call attacker allies
		// todo: populate naval control

		return new_war;
	}
}
