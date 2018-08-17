#include "common\\common.h"
#include "nations_functions.h"
#include "world_state\\world_state.h"
#include "scenario\\scenario.h"
#include "military\\military_functions.h"
#include "technologies\\technologies_functions.h"
#include "governments\\governments_functions.h"
#include "issues\\issues_functions.h"
#include "population\\population_function.h"

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
		
		clear(ws.w.nation_s.loan_arrays, new_nation.loans);
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
		ws.w.nation_s.upper_house.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_technologies.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_goods.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_issue_options.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.national_stockpiles.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.national_variables.ensure_capacity(to_index(new_nation.id) + 1);

		ws.w.nation_s.unit_stats.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.rebel_org_gain.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.production_adjustments.ensure_capacity(to_index(new_nation.id) + 1);

		reset_nation(ws, new_nation);

		return &new_nation;
	}

	bool is_state_empty(world_state const& ws, nation const& owner, provinces::state_tag region) {
		for(auto prange = ws.s.province_m.states_to_province_index.get_row(region); prange.first != prange.second; ++prange.first) {
			if(ws.w.province_s.province_state_container[*prange.first].owner == &owner)
				return false;
		}
		return true;
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

	void destroy_state_instance(world_state& ws, state_instance& si, nation& owner) {
		partial_destroy_state_instance(ws, si);
		remove_item(ws.w.nation_s.state_arrays, owner.member_states, region_state_pair{ si.region_id, nullptr });
	}

	void remove_province_from_state(world_state&, provinces::province_state& p) {
		p.state_instance = nullptr;
	}

	void silent_remove_province_owner(world_state& ws, nation* owner, provinces::province_tag prov) {
		remove_item(ws.w.province_s.province_arrays, owner->owned_provinces, prov);

		auto& prov_state = ws.w.province_s.province_state_container[prov];

		prov_state.owner = nullptr;
		state_instance* old_state = prov_state.state_instance;

		remove_province_from_state(ws, prov_state);

		const auto region_tag = ws.s.province_m.province_container[prov].state_id;
		if(is_state_empty(ws, *owner, region_tag)) {
			remove_item(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ old_state->region_id, nullptr });
			if(old_state) {
				destroy_state_instance(ws, *old_state, *owner);
				ws.w.nation_s.states.remove(old_state->id);
			}
		}
	}

	void silent_remove_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov) {
		remove_item(ws.province_s.province_arrays, controller->controlled_provinces, prov);
		ws.province_s.province_state_container[prov].controller = nullptr;
	}

	void silent_set_province_owner(world_state& ws, nation* owner, provinces::province_tag prov) {
		auto& prov_state = ws.w.province_s.province_state_container[prov];

		add_item(ws.w.province_s.province_arrays, owner->owned_provinces, prov);
		if(prov_state.owner != nullptr)
			silent_remove_province_owner(ws, prov_state.owner, prov);

		
		prov_state.owner = owner;
		const auto region_id = ws.s.province_m.province_container[prov].state_id;

		region_state_pair* found = find(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ region_id, nullptr });
		if(found) {
			prov_state.state_instance = found->state;
		} else {
			state_instance* si = &(ws.w.nation_s.states.get_new());
			si->owner = owner;
			si->region_id = ws.s.province_m.province_container[prov].state_id;

			ws.w.nation_s.state_demographics.ensure_capacity(to_index(si->id) + 1);

			prov_state.state_instance = si;
			add_item(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ region_id, si });
		}
	}

	void silent_set_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov) {
		add_item(ws.province_s.province_arrays, controller->controlled_provinces, prov);
		if(ws.province_s.province_state_container[prov].controller != nullptr)
			silent_remove_province_controller(ws, ws.province_s.province_state_container[prov].controller, prov);
		ws.province_s.province_state_container[prov].controller = controller;
	}

	void init_empty_states(world_state& ws) {
		for(int32_t i = int32_t(ws.w.province_s.province_state_container.size()) - 1; i > 0; --i) {
			const provinces::province_tag this_prov_id(static_cast<provinces::province_tag::value_base_t>(i));
			auto& this_province = ws.w.province_s.province_state_container[this_prov_id];
			if((this_province.owner == nullptr) &
				((ws.s.province_m.province_container[this_prov_id].flags & provinces::province::sea) == 0) &
				(this_province.state_instance == nullptr) &
				is_valid_index(ws.s.province_m.province_container[this_prov_id].state_id)) {

				state_instance* si = &(ws.w.nation_s.states.get_new());
				si->region_id = ws.s.province_m.province_container[this_prov_id].state_id;

				ws.w.nation_s.state_demographics.ensure_capacity(to_index(si->id) + 1);

				this_province.state_instance = si;

				auto same_region_range = ws.s.province_m.states_to_province_index.get_row(si->region_id);
				for(auto same_region_prov : same_region_range) {
					auto& other_province = ws.w.province_s.province_state_container[same_region_prov];
					if((other_province.owner == nullptr) &
						((ws.s.province_m.province_container[same_region_prov].flags & provinces::province::sea) == 0) &
						(other_province.state_instance == nullptr)) {
						other_province.state_instance = si;
					}
				}
			}
		}
	}

	void init_nations_state(world_state& ws) {
		ws.w.nation_s.nation_demographics.reset(population::aligned_64_demo_size(ws));
		ws.w.nation_s.state_demographics.reset(population::aligned_32_demo_size(ws));
		ws.w.nation_s.active_parties.reset(ws.s.ideologies_m.ideologies_count);
		ws.w.nation_s.upper_house.reset(ws.s.ideologies_m.ideologies_count);
		ws.w.nation_s.active_technologies.reset((uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32);
		ws.w.nation_s.active_goods.reset((ws.s.economy_m.goods_count + 63ui32) / 64ui32);
		ws.w.nation_s.active_issue_options.reset(uint32_t(ws.s.issues_m.issues_container.size()));
		ws.w.nation_s.national_stockpiles.reset(uint32_t(ws.s.economy_m.aligned_32_goods_count));
		ws.w.nation_s.national_variables.reset(ws.s.variables_m.count_national_variables);

		ws.w.nation_s.unit_stats.reset(static_cast<uint32_t>(ws.s.military_m.unit_types.size()));
		ws.w.nation_s.rebel_org_gain.reset(static_cast<uint32_t>(ws.s.population_m.rebel_types.size()));
		ws.w.nation_s.production_adjustments.reset(ws.s.economy_m.goods_count * uint32_t(technologies::production_adjustment::production_adjustment_count));
	}

	void update_state_nation_demographics(world_state& ws) {
		const auto full_vector_size = population::aligned_32_demo_size(ws);

		ws.w.nation_s.nations.for_each([&ws, full_vector_size](nation& n) {
			Eigen::Map<Eigen::Matrix<int64_t, -1, 1>, Eigen::AlignmentType::Aligned32> nation_demo(ws.w.nation_s.nation_demographics.get_row(n.id), full_vector_size);
			nation_demo.setZero();

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

				{
					const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));

					s->state->dominant_culture = cultures::culture_tag(0);
					int32_t max_pop = state_demo[to_index(culture_offset)];

					for(uint32_t i = 1ui32; i < ws.s.culture_m.count_cultures; ++i) {
						if(state_demo[to_index(culture_offset) + i] > max_pop) {
							max_pop = state_demo[to_index(culture_offset) + i];
							s->state->dominant_culture = cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(i));
						}
					}
				}

				{
					const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));

					s->state->dominant_religion = cultures::religion_tag(0);
					int32_t max_pop = state_demo[to_index(religion_offset)];

					for(uint32_t i = 1ui32; i < ws.s.culture_m.count_religions; ++i) {
						if(state_demo[to_index(religion_offset) + i] > max_pop) {
							max_pop = state_demo[to_index(religion_offset) + i];
							s->state->dominant_religion = cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(i));
						}
					}
				}

				{
					const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));

					s->state->dominant_ideology = ideologies::ideology_tag(0);
					int32_t max_pop = state_demo[to_index(ideology_offset)];

					for(uint32_t i = 1ui32; i < ws.s.ideologies_m.ideologies_count; ++i) {
						if(state_demo[to_index(ideology_offset) + i] > max_pop) {
							max_pop = state_demo[to_index(ideology_offset) + i];
							s->state->dominant_ideology = ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(i));
						}
					}
				}

				{
					const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));

					s->state->dominant_issue = issues::option_tag(0);
					int32_t max_pop = state_demo[to_index(options_offset)];

					for(uint32_t i = 1ui32; i < ws.s.issues_m.tracked_options_count; ++i) {
						if(state_demo[to_index(options_offset) + i] > max_pop) {
							max_pop = state_demo[to_index(options_offset) + i];
							s->state->dominant_issue = issues::option_tag(static_cast<value_base_of<issues::option_tag>>(i));
						}
					}
				}
				nation_demo += state_demo.cast<int64_t>();
			}

			{
				const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));

				n.dominant_culture = cultures::culture_tag(0);
				int64_t max_pop = nation_demo[to_index(culture_offset)];

				for(uint32_t i = 1ui32; i < ws.s.culture_m.count_cultures; ++i) {
					if(nation_demo[to_index(culture_offset) + i] > max_pop) {
						max_pop = nation_demo[to_index(culture_offset) + i];
						n.dominant_culture = cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(i));
					}
				}
			}

			{
				const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));

				n.dominant_religion = cultures::religion_tag(0);
				int64_t max_pop = nation_demo[to_index(religion_offset)];

				for(uint32_t i = 1ui32; i < ws.s.culture_m.count_religions; ++i) {
					if(nation_demo[to_index(religion_offset) + i] > max_pop) {
						max_pop = nation_demo[to_index(religion_offset) + i];
						n.dominant_religion = cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(i));
					}
				}
			}

			{
				const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));

				n.dominant_ideology = ideologies::ideology_tag(0);
				int64_t max_pop = nation_demo[to_index(ideology_offset)];

				for(uint32_t i = 1ui32; i < ws.s.ideologies_m.ideologies_count; ++i) {
					if(nation_demo[to_index(ideology_offset) + i] > max_pop) {
						max_pop = nation_demo[to_index(ideology_offset) + i];
						n.dominant_ideology = ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(i));
					}
				}
			}

			{
				const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));

				n.dominant_issue = issues::option_tag(0);
				int64_t max_pop = nation_demo[to_index(options_offset)];

				for(uint32_t i = 1ui32; i < ws.s.issues_m.tracked_options_count; ++i) {
					if(nation_demo[to_index(options_offset) + i] > max_pop) {
						max_pop = nation_demo[to_index(options_offset) + i];
						n.dominant_issue = issues::option_tag(static_cast<value_base_of<issues::option_tag>>(i));
					}
				}
			}
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
	void set_influence(world_state& ws, nation& a, country_tag b, int32_t value, int32_t level) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f) {
			if(f->level == 5i8 && level != 5) {
				ws.w.nation_s.nations.get(b).sphere_leader = nullptr;
				remove_item(ws.w.nation_s.nations_arrays, a.sphere_members, b);
			}
			f->amount = uint8_t(value);
			f->level = int8_t(level);
		} else {
			add_item(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, uint8_t(value), int8_t(level) });
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[b].influencers, a.id);
		}
		if(level == 5) {
			ws.w.nation_s.nations.get(b).sphere_leader = &a;
			add_item(ws.w.nation_s.nations_arrays, a.sphere_members, b);
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

	int32_t colonial_points_to_make_protectorate(world_state&, state_instance&) {
		return 0;
	}
	int32_t colonial_points_to_make_colony(world_state&, state_instance&) {
		return 0;
	}
	int32_t colonial_points_to_make_state(world_state&, state_instance&) {
		return 0;
	}
	int32_t free_colonial_points(world_state&, nation&) {
		return 0;
	}
	int32_t points_for_next_colonial_stage(world_state&, nation&, state_instance&) {
		return 0;
	}

	text_data::text_tag get_nation_status_text(world_state& ws, nation const& this_nation) {
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
	
	bool is_great_power(world_state const&, nation const&) {
		return false;
	}

	void silent_make_alliance(world_state& ws, nation& a, nation& b) {
		add_item(ws.w.nation_s.nations_arrays, a.allies, b.id);
		add_item(ws.w.nation_s.nations_arrays, b.allies, a.id);
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

	nation* union_holder_for(world_state& ws, cultures::culture_tag pculture) {
		auto cgroup = ws.s.culture_m.culture_container[pculture].group;
		auto union_tag = ws.s.culture_m.culture_groups[cgroup].union_tag;
		if(is_valid_index(union_tag))
			return ws.w.culture_s.national_tags_state[union_tag].holder;
		else
			return nullptr;
	}

	cultures::national_tag union_tag_of(world_state& ws, nation const& this_nation) {
		auto pculture = this_nation.primary_culture;
		if(is_valid_index(pculture)) {
			auto cgroup = ws.s.culture_m.culture_container[pculture].group;
			return ws.s.culture_m.culture_groups[cgroup].union_tag;
		}
		else
			return cultures::national_tag();
	}

	nation* union_holder_of(world_state& ws, nation const& this_nation) {
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
}
