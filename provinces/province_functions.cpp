#include "common\\common.h"
#include "province_functions.hpp"
#include "modifiers\\modifiers.h"
#include "population\\population_function.h"
#include "nations\\nations_functions.h"
#include "modifiers\\modifier_functions.h"

namespace provinces {
	void reset_state(provinces_state& s) {
		for(auto& ps : s.province_state_container) {
			ps.cores = set_tag<cultures::national_tag>();
			ps.fleets = set_tag<military::fleet_presence>();
			ps.orders = nullptr;
			ps.owner = nullptr;
			ps.controller = nullptr;
			ps.orders = nullptr;
			ps.static_modifiers = set_tag<modifiers::provincial_modifier_tag>();
			ps.timed_modifiers = multiset_tag<provinces::timed_provincial_modifier>();
			ps.pops = array_tag<population::pop_tag>();
		}
		s.core_arrays.reset();
		s.static_modifier_arrays.reset();
		s.timed_modifier_arrays.reset();
		s.province_arrays.reset();
	}

	void add_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		add_item(ws.province_s.core_arrays, ws.province_s.province_state_container[prov].cores, tag);
		add_item(ws.province_s.province_arrays, ws.culture_s.national_tags_state[tag].core_provinces, prov);
	}
	void remove_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		remove_item(ws.province_s.core_arrays, ws.province_s.province_state_container[prov].cores, tag);
		remove_item(ws.province_s.province_arrays, ws.culture_s.national_tags_state[tag].core_provinces, prov);
	}
	bool province_has_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		return contains_item(ws.province_s.core_arrays, ws.province_s.province_state_container[prov].cores, tag);
	}

	void add_province_modifier(world_state& ws, province_state& p, modifiers::provincial_modifier_tag t) {
		add_item(ws.w.province_s.static_modifier_arrays, p.static_modifiers, t);
	}

	void add_timed_province_modifier(world_state& ws, province_state& p, modifiers::provincial_modifier_tag t, date_tag d) {
		add_item(ws.w.province_s.timed_modifier_arrays, p.timed_modifiers, timed_provincial_modifier{ d, t });
	}

	void init_ready_provinces(world_state& ws) {
		const auto prov_count = ws.s.province_m.province_container.size();
		
		if(ws.w.province_s.party_loyalty.inner_size() != uint32_t(ws.s.ideologies_m.ideology_container.size()))
			ws.w.province_s.party_loyalty.reset(uint32_t(ws.s.ideologies_m.ideology_container.size()));
		if(ws.w.province_s.party_loyalty.outer_size() != prov_count)
			ws.w.province_s.party_loyalty.resize(prov_count);
		if(ws.w.province_s.province_state_container.size() != prov_count)
			ws.w.province_s.province_state_container.resize(prov_count);
		ws.w.province_s.is_canal_enabled.resize(ws.s.province_m.canals.size());

		for(uint32_t i = 0; i < prov_count; ++i) {
			auto& p = ws.w.province_s.province_state_container[province_tag(static_cast<uint16_t>(i))];
			p.id = province_tag(static_cast<uint16_t>(i));

			std::string name_temp("PROV");
			name_temp += std::to_string(i);
			p.name = text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, name_temp.data(), name_temp.data() + name_temp.length());

			if(!p.controller)
				p.controller = p.owner;
		}

		if(ws.w.province_s.province_demographics.inner_size() != population::aligned_32_demo_size(ws))
			ws.w.province_s.province_demographics.reset(population::aligned_32_demo_size(ws));
		if(ws.w.province_s.province_demographics.outer_size() != prov_count)
			ws.w.province_s.province_demographics.resize(prov_count);
	}

	void update_province_demographics(world_state& ws) {
		const auto vector_size = population::aligned_32_issues_ideology_demo_size(ws);
		const auto full_vector_size = population::aligned_32_demo_size(ws);

		const auto ppdt = population::poor_population_demo_tag(ws);
		const auto mpdt = population::middle_population_demo_tag(ws);
		const auto rpdt = population::rich_population_demo_tag(ws);

		const auto cdt = population::consciousness_demo_tag(ws);
		const auto mdt = population::militancy_demo_tag(ws);
		const auto ldt = population::literacy_demo_tag(ws);

		const auto pmpdt = population::poor_militancy_demo_tag(ws);
		const auto mmpdt = population::middle_militancy_demo_tag(ws);
		const auto rmpdt = population::rich_militancy_demo_tag(ws);

		const auto plndt = population::poor_life_needs_demo_tag(ws);
		const auto mlndt = population::middle_life_needs_demo_tag(ws);
		const auto rlndt = population::rich_life_needs_demo_tag(ws);

		const auto pendt = population::poor_everyday_needs_demo_tag(ws);
		const auto mendt = population::middle_everyday_needs_demo_tag(ws);
		const auto rendt = population::rich_everyday_needs_demo_tag(ws);

		const auto pxndt = population::poor_luxury_needs_demo_tag(ws);
		const auto mxndt = population::middle_luxury_needs_demo_tag(ws);
		const auto rxndt = population::rich_luxury_needs_demo_tag(ws);

		int32_t count_provs = int32_t(ws.w.province_s.province_state_container.size());
		for(int32_t prov_index = count_provs - 1; prov_index > 0; --prov_index) {
			auto& prov = ws.w.province_s.province_state_container[province_tag(uint16_t(prov_index))];

			auto pop_range = get_range(ws.w.population_s.pop_arrays, prov.pops);

			Eigen::Map<Eigen::Matrix<int32_t, -1, 1>, Eigen::AlignmentType::Aligned32> province_partial_demo(ws.w.province_s.province_demographics.get_row(prov.id), vector_size);
			Eigen::Map<Eigen::Matrix<int32_t, -1, 1>, Eigen::AlignmentType::Aligned32> province_full_demo(ws.w.province_s.province_demographics.get_row(prov.id), full_vector_size);
			
			province_full_demo.setZero();

			for(auto pop = pop_range.first; pop != pop_range.second; ++pop) {
				Eigen::Map<Eigen::Matrix<int32_t, -1, 1>, Eigen::AlignmentType::Aligned32> pop_demo_source(ws.w.population_s.pop_demographics.get_row(*pop), vector_size);
				province_partial_demo += pop_demo_source;

				population::pop& this_pop = ws.w.population_s.pops.get(*pop);

				auto pop_size = pop_demo_source[to_index(population::total_population_tag)];

				province_full_demo[to_index(population::to_demo_tag(ws, this_pop.culture))] += pop_size;
				province_full_demo[to_index(population::to_demo_tag(ws, this_pop.religion))] += pop_size;
				province_full_demo[to_index(population::to_demo_tag(ws, this_pop.type))] += pop_size;
				province_full_demo[to_index(population::to_employment_demo_tag(ws, this_pop.type))] += pop_demo_source[to_index(population::total_employment_tag)];


				province_full_demo[to_index(cdt)] += int32_t(((uint64_t(this_pop.consciousness) + 1ui64) * uint64_t(pop_size)) >> 16ui64);
				province_full_demo[to_index(ldt)] += int32_t(((uint64_t(this_pop.literacy) + 1ui64) * uint64_t(pop_size)) >> 16ui64);

				const int32_t weighted_militancy = int32_t(((uint64_t(this_pop.literacy) + 1ui64) * uint64_t(pop_size)) >> 16ui64);
				province_full_demo[to_index(mdt)] += weighted_militancy;

				const auto strata = ws.s.population_m.pop_types[this_pop.type].flags & population::pop_type::strata_mask;
				if(strata == population::pop_type::strata_poor) {
					province_full_demo[to_index(pmpdt)] += weighted_militancy;
					province_full_demo[to_index(ppdt)] += pop_size;
					if(this_pop.needs_satisfaction < 1.0f) {
						province_full_demo[to_index(plndt)] += int32_t(float(pop_size) * this_pop.needs_satisfaction);
					} else {
						province_full_demo[to_index(plndt)] += pop_size;
						if(this_pop.needs_satisfaction < 2.0f) {
							province_full_demo[to_index(pendt)] += int32_t(float(pop_size) * (this_pop.needs_satisfaction - 1.0f));
						} else {
							province_full_demo[to_index(pendt)] += pop_size;
							if(this_pop.needs_satisfaction < 3.0f) {
								province_full_demo[to_index(pxndt)] += int32_t(float(pop_size) * (this_pop.needs_satisfaction - 2.0f));
							} else {
								province_full_demo[to_index(pxndt)] += pop_size;
							}
						}
					}
				} else if(strata == population::pop_type::strata_middle) {
					province_full_demo[to_index(mmpdt)] += weighted_militancy;
					province_full_demo[to_index(mpdt)] += pop_size;
					if(this_pop.needs_satisfaction < 1.0f) {
						province_full_demo[to_index(mlndt)] += int32_t(float(pop_size) * this_pop.needs_satisfaction);
					} else {
						province_full_demo[to_index(mlndt)] += pop_size;
						if(this_pop.needs_satisfaction < 2.0f) {
							province_full_demo[to_index(mendt)] += int32_t(float(pop_size) * (this_pop.needs_satisfaction - 1.0f));
						} else {
							province_full_demo[to_index(mendt)] += pop_size;
							if(this_pop.needs_satisfaction < 3.0f) {
								province_full_demo[to_index(mxndt)] += int32_t(float(pop_size) * (this_pop.needs_satisfaction - 2.0f));
							} else {
								province_full_demo[to_index(mxndt)] += pop_size;
							}
						}
					}
				} else if(strata == population::pop_type::strata_rich) {
					province_full_demo[to_index(rmpdt)] += weighted_militancy;
					province_full_demo[to_index(rpdt)] += pop_size;
					if(this_pop.needs_satisfaction < 1.0f) {
						province_full_demo[to_index(rlndt)] += int32_t(float(pop_size) * this_pop.needs_satisfaction);
					} else {
						province_full_demo[to_index(rlndt)] += pop_size;
						if(this_pop.needs_satisfaction < 2.0f) {
							province_full_demo[to_index(rendt)] += int32_t(float(pop_size) * (this_pop.needs_satisfaction - 1.0f));
						} else {
							province_full_demo[to_index(rendt)] += pop_size;
							if(this_pop.needs_satisfaction < 3.0f) {
								province_full_demo[to_index(rxndt)] += int32_t(float(pop_size) * (this_pop.needs_satisfaction - 2.0f));
							} else {
								province_full_demo[to_index(rxndt)] += pop_size;
							}
						}
					}
				}
			}

			{
				const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));

				prov.dominant_culture = cultures::culture_tag(0);
				int32_t max_pop = province_full_demo[to_index(culture_offset)];

				for(uint32_t i = 1ui32; i < ws.s.culture_m.count_cultures; ++i) {
					if(province_full_demo[to_index(culture_offset) + i] > max_pop) {
						max_pop = province_full_demo[to_index(culture_offset) + i];
						prov.dominant_culture = cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(i));
					}
				}
			}

			{
				const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));

				prov.dominant_religion = cultures::religion_tag(0);
				int32_t max_pop = province_full_demo[to_index(religion_offset)];

				for(uint32_t i = 1ui32; i < ws.s.culture_m.count_religions; ++i) {
					if(province_full_demo[to_index(religion_offset) + i] > max_pop) {
						max_pop = province_full_demo[to_index(religion_offset) + i];
						prov.dominant_religion = cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(i));
					}
				}
			}

			{
				const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));

				prov.dominant_ideology = ideologies::ideology_tag(0);
				int32_t max_pop = province_full_demo[to_index(ideology_offset)];

				for(uint32_t i = 1ui32; i < ws.s.ideologies_m.ideologies_count; ++i) {
					if(province_full_demo[to_index(ideology_offset) + i] > max_pop) {
						max_pop = province_full_demo[to_index(ideology_offset) + i];
						prov.dominant_ideology = ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(i));
					}
				}
			}

			{
				const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));

				prov.dominant_issue = issues::option_tag(0);
				int32_t max_pop = province_full_demo[to_index(options_offset)];

				for(uint32_t i = 1ui32; i < ws.s.issues_m.tracked_options_count; ++i) {
					if(province_full_demo[to_index(options_offset) + i] > max_pop) {
						max_pop = province_full_demo[to_index(options_offset) + i];
						prov.dominant_issue = issues::option_tag(static_cast<value_base_of<issues::option_tag>>(i));
					}
				}
			}
		}
	}

	nations::country_tag get_province_seiger(world_state&, province_state&) {
		return nations::country_tag();
	}

	int32_t get_province_rgo_employment_max(world_state&, province_state&) {
		return 1;
	}

	float get_life_rating(province_state const& p) {
		return float(p.base_life_rating) * (1.0f + p.modifier_values[modifiers::provincial_offsets::life_rating]);
	}


	void silent_remove_province_owner(world_state& ws, provinces::province_state& prov_state) {
		if(prov_state.owner) {
			remove_item(ws.w.province_s.province_arrays, prov_state.owner->owned_provinces, prov_state.id);
			modifiers::detach_province_modifiers(ws, prov_state, *prov_state.owner);
		}

		prov_state.owner = nullptr;
		nations::state_instance* old_state = prov_state.state_instance;

		if(prov_state.state_instance) {
			auto old_state = prov_state.state_instance;
			nations::remove_province_from_state(ws, prov_state);

			if(nations::is_state_empty(ws, *old_state)) {
				nations::destroy_state_instance(ws, *old_state);
				ws.w.nation_s.states.remove(old_state->id);
			}
		}
	}

	void silent_remove_province_controller(world_state& ws, provinces::province_state& prov) {
		if(prov.controller) {
			remove_item(ws.w.province_s.province_arrays, prov.controller->controlled_provinces, prov.id);
			prov.controller = nullptr;
		}
	}

	void silent_set_province_controller(world_state& ws, nations::nation& new_controller, provinces::province_state& prov) {
		add_item(ws.w.province_s.province_arrays, new_controller.controlled_provinces, prov.id);
		if(prov.controller != nullptr)
			silent_remove_province_controller(ws, prov);
		prov.controller = &new_controller;
	}

	void silent_set_province_owner(world_state& ws, nations::nation& new_owner, provinces::province_state& prov) {
		add_item(ws.w.province_s.province_arrays, new_owner.owned_provinces, prov.id);
		silent_remove_province_owner(ws, prov);

		modifiers::attach_province_modifiers(ws, prov, new_owner);

		prov.owner = &new_owner;
		const auto region_id = ws.s.province_m.province_container[prov.id].state_id;

		nations::region_state_pair* found = find(ws.w.nation_s.state_arrays, new_owner.member_states, nations::region_state_pair{ region_id, nullptr });
		if(found) {
			prov.state_instance = found->state;
		} else {
			nations::state_instance* si = &(ws.w.nation_s.states.get_new());
			si->owner = &new_owner;
			si->region_id = ws.s.province_m.province_container[prov.id].state_id;
			si->name = ws.s.province_m.state_names[si->region_id];

			ws.w.nation_s.state_demographics.ensure_capacity(to_index(si->id) + 1);

			prov.state_instance = si;
			add_item(ws.w.nation_s.state_arrays, new_owner.member_states, nations::region_state_pair{ region_id, si });
		}
	}

	void silent_on_conquer_province(world_state& ws, provinces::province_state& prov) {
		auto owner = prov.owner;
		auto owner_tag = owner->tag;
		if(is_valid_index(owner_tag)) {
			if(prov.id == ws.w.culture_s.national_tags_state[owner_tag].capital)
				owner->current_capital = prov.id;
		}
	}
}
