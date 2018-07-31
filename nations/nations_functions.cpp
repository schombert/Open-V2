#include "common\\common.h"
#include "nations_functions.h"
#include "world_state\\world_state.h"
#include "scenario\\scenario.h"

namespace nations {
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
		ws.w.nation_s.national_modifiers.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.upper_house.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_technologies.ensure_capacity(to_index(new_nation.id) + 1);
		ws.w.nation_s.active_issue_options.ensure_capacity(to_index(new_nation.id) + 1);

		return &new_nation;
	}

	bool is_state_empty(world_state& ws, nation* owner, provinces::state_tag region) {
		for(auto prange = ws.s.province_m.states_to_province_index.get_row(region); prange.first != prange.second; ++prange.first) {
			if(ws.w.province_s.province_state_container[*prange.first].owner == owner)
				return false;
		}
		return true;
	}

	void destroy_state_instance(world_state& ws, state_instance& si) {
		ws.w.nation_s.states.remove(si.id);
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
		if(is_state_empty(ws, owner, region_tag)) {
			remove_item(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ old_state->region_id, nullptr });
			if(old_state) {
				destroy_state_instance(ws, *old_state);
				remove_item(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ old_state->region_id, nullptr });
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
		ws.w.nation_s.national_modifiers.reset(modifiers::national_offsets::aligned_32_size);
		ws.w.nation_s.active_parties.reset(ws.s.ideologies_m.ideologies_count);
		ws.w.nation_s.upper_house.reset(ws.s.ideologies_m.ideologies_count);
		ws.w.nation_s.active_technologies.reset(uint32_t(ws.s.technology_m.technologies_container.size() >> 6ui32));
		ws.w.nation_s.active_issue_options.reset(uint32_t(ws.s.issues_m.issues_container.size()));
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
		});
	}

	provinces::province_tag find_best_capital(world_state& ws, nation& owner) {
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
	int32_t get_relationship(world_state& ws, nation& a, country_tag b) {
		if(auto f = find(ws.w.nation_s.relations_arrays, a.relations, relationship{ b, 0i16 }); f)
			return f->value;
		return 0;
	}
	int32_t get_influence_value(world_state& ws, nation& a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->amount;
		return 0;
	}
	int32_t get_influence_level(world_state& ws, nation& a, country_tag b) {
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
		}
		if(level == 5) {
			ws.w.nation_s.nations.get(b).sphere_leader = &a;
			add_item(ws.w.nation_s.nations_arrays, a.sphere_members, b);
		}
	}
	float get_foreign_investment(world_state& ws, nation& a, country_tag b) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f)
			return f->investment_amount;
		return 0.0f;
	}
	void set_foreign_investment(world_state& ws, nation& a, country_tag b, float value) {
		if(auto f = find(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ 0.0f, b, 0ui8, 0i8 }); f) {
			f->investment_amount = value;
		} else {
			add_item(ws.w.nation_s.influence_arrays, a.gp_influence, influence{ value, b, 0ui8, 0i8 });
		}
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
		auto this_id = this_nation.id;
		if(ws.w.nation_s.nations.is_valid_index(this_id)) {
			if((this_nation.flags & nation::is_civilized) == 0) {
				if(ws.w.nation_s.national_modifiers.get(this_id, modifiers::national_offsets::civilization_progress_modifier) > modifiers::value_type(0))
					return ws.s.fixed_ui_text[scenario::fixed_ui::partialy_civilized_nation];
				else
					return ws.s.fixed_ui_text[scenario::fixed_ui::uncivilized_nation];
			} else if(is_great_power(ws, this_nation))
				return ws.s.fixed_ui_text[scenario::fixed_ui::great_power];
			else if(this_nation.overall_rank <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
				return ws.s.fixed_ui_text[scenario::fixed_ui::secondary_power];
			else
				return ws.s.fixed_ui_text[scenario::fixed_ui::civilized_nation];
		} else {
			return ws.s.fixed_ui_text[scenario::fixed_ui::civilized_nation];
		}
	}
	
	bool is_great_power(world_state&, nation const&) {
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

	nations::nation* union_holder_for(world_state& ws, cultures::culture_tag pculture) {
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
			cultures::national_tag();
	}

	nations::nation* union_holder_of(world_state& ws, nation const& this_nation) {
		auto pculture = this_nation.primary_culture;
		if(is_valid_index(pculture))
			return union_holder_for(ws, pculture);
		else
			return nullptr;
	}
}
