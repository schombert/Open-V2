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

		auto first_name_range = ws.s.culture_m.first_names_by_culture.get_row(to_index(culture));
		auto last_name_range = ws.s.culture_m.first_names_by_culture.get_row(to_index(culture));

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

		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>, Eigen::Aligned32> source_a(ws.s.military_m.leader_trait_definitions.get_row(l.background));
		Eigen::Map<Eigen::Matrix<traits::value_type, traits::trait_count, 1>, Eigen::Aligned32> source_b(ws.s.military_m.leader_trait_definitions.get_row(l.personality));

		dest_vec = source_a + source_b;
	}

	army& make_army(world_state& ws, nations::nation& n, provinces::province_tag location) {
		army& new_army = ws.w.military_s.armies.get_new();
		new_army.base = location;
		add_item(ws.w.military_s.army_arrays, n.armies, new_army.id);
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
		return new_fleet;
	}

	void list_opposing_countries(world_state& ws, nations::nation& this_nation, boost::container::small_vector<nations::country_tag, 32, concurrent_allocator<nations::country_tag>>& result) {
		auto owner_wars = get_range(ws.w.military_s.war_arrays, this_nation.wars_involved_in);
		for(auto iwar = owner_wars.first; iwar != owner_wars.second; ++iwar) {
			if(auto warid = iwar->war_id; ws.w.military_s.wars.is_valid_index(warid)) {
				if(iwar->is_attacker) {
					auto defender_range = get_range(ws.w.nation_s.nations_arrays, ws.w.military_s.wars[warid].defenders);
					for(auto d : defender_range) {
						if(is_valid_index(d) && std::find(result.begin(), result.end(), d) == result.end())
							result.push_back(d);
					}
				} else {
					auto attacker_range = get_range(ws.w.nation_s.nations_arrays, ws.w.military_s.wars[warid].attackers);
					for(auto d : attacker_range) {
						if(is_valid_index(d) && std::find(result.begin(), result.end(), d) == result.end())
							result.push_back(d);
					}
				}
			}
		}
	}
}
