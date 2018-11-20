#include "common\\common.h"
#include "province_functions.hpp"
#include "modifiers\\modifiers.h"
#include "population\\population_function.h"
#include "nations\\nations_functions.h"
#include "modifiers\\modifier_functions.h"
#include "military\\military_functions.h"
#include <ppl.h>

namespace provinces {
	nations::nation const* province_owner(world_state const& ws, province_tag p) {
		if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(p); ws.w.nation_s.nations.is_valid_index(owner)) {
			return &ws.w.nation_s.nations[owner];
		}
		return nullptr;
	}
	nations::nation const* province_controller(world_state const& ws, province_tag p) {
		if(auto controller = ws.w.province_s.province_state_container.get<province_state::controller>(p); ws.w.nation_s.nations.is_valid_index(controller)) {
			return &ws.w.nation_s.nations[controller];
		}
		return nullptr;
	}
	nations::state_instance const* province_state(world_state const& ws, province_tag p) {
		if(auto si = ws.w.province_s.province_state_container.get<province_state::state_instance>(p); ws.w.nation_s.states.is_valid_index(si)) {
			return &ws.w.nation_s.states[si];
		}
		return nullptr;
	}
	nations::nation* province_owner(world_state& ws, province_tag p) {
		if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(p); ws.w.nation_s.nations.is_valid_index(owner)) {
			return &ws.w.nation_s.nations[owner];
		}
		return nullptr;
	}
	nations::nation* province_controller(world_state& ws, province_tag p) {
		if(auto controller = ws.w.province_s.province_state_container.get<province_state::controller>(p); ws.w.nation_s.nations.is_valid_index(controller)) {
			return &ws.w.nation_s.nations[controller];
		}
		return nullptr;
	}
	nations::state_instance* province_state(world_state& ws, province_tag p) {
		if(auto si = ws.w.province_s.province_state_container.get<province_state::state_instance>(p); ws.w.nation_s.states.is_valid_index(si)) {
			return &ws.w.nation_s.states[si];
		}
		return nullptr;
	}
	void reset_state(provinces_state& s) {
		s.province_state_container.for_each([&s](provinces::province_tag p){
			s.province_state_container.set<province_state::cores>(p, set_tag<cultures::national_tag>());
			s.province_state_container.set<province_state::fleets>(p, set_tag<military::fleet_presence>());
			s.province_state_container.set<province_state::orders>(p, military::army_orders_tag());
			s.province_state_container.set<province_state::controller>(p, nations::country_tag());
			s.province_state_container.set<province_state::static_modifiers>(p, set_tag<modifiers::provincial_modifier_tag>());
			s.province_state_container.set<province_state::timed_modifiers>(p, multiset_tag<provinces::timed_provincial_modifier>());
			s.province_state_container.set<province_state::pops>(p, array_tag<population::pop_tag>());
		});
		s.core_arrays.reset();
		s.static_modifier_arrays.reset();
		s.timed_modifier_arrays.reset();
		s.province_arrays.reset();
	}

	void add_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		add_item(ws.province_s.core_arrays, ws.province_s.province_state_container.get<province_state::cores>(prov), tag);
		add_item(ws.province_s.province_arrays, ws.culture_s.national_tags_state[tag].core_provinces, prov);
	}
	void remove_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		remove_item(ws.province_s.core_arrays, ws.province_s.province_state_container.get<province_state::cores>(prov), tag);
		remove_item(ws.province_s.province_arrays, ws.culture_s.national_tags_state[tag].core_provinces, prov);
	}
	bool province_has_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		return contains_item(ws.province_s.core_arrays, ws.province_s.province_state_container.get<province_state::cores>(prov), tag);
	}

	void add_province_modifier(world_state& ws, provinces::province_tag p, modifiers::provincial_modifier_tag t) {
		add_item(ws.w.province_s.static_modifier_arrays,
			ws.w.province_s.province_state_container.get<province_state::static_modifiers>(p),
			t);
	}

	void add_timed_province_modifier(world_state& ws, provinces::province_tag p, modifiers::provincial_modifier_tag t, date_tag d) {
		add_item(ws.w.province_s.timed_modifier_arrays,
			ws.w.province_s.province_state_container.get<province_state::timed_modifiers>(p),
			timed_provincial_modifier{ d, t });
	}

	void init_province_state(world_state& ws) {
		const auto prov_count = ws.s.province_m.province_container.size();
		
		ws.w.province_s.province_distance_to.resize(prov_count * prov_count);
		ws.w.province_s.province_path_to.resize(prov_count * prov_count);

		if(ws.w.province_s.party_loyalty.inner_size() != uint32_t(ws.s.ideologies_m.ideology_container.size()))
			ws.w.province_s.party_loyalty.reset(uint32_t(ws.s.ideologies_m.ideology_container.size()));
		if(ws.w.province_s.party_loyalty.outer_size() != prov_count)
			ws.w.province_s.party_loyalty.resize(prov_count);
		if(ws.w.province_s.province_state_container.size() != prov_count)
			ws.w.province_s.province_state_container.resize(prov_count);
		ws.w.province_s.is_canal_enabled.resize(ws.s.province_m.canals.size());

		for(int32_t i = 0; i < prov_count; ++i) {
			auto this_tag = province_tag(static_cast<uint16_t>(i));

			std::string name_temp("PROV");
			name_temp += std::to_string(i);

			ws.w.province_s.province_state_container.set<province_state::name>(
				this_tag,
				text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, name_temp.data(), name_temp.data() + name_temp.length()));

			if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::controller>(this_tag)))
				ws.w.province_s.province_state_container.set<province_state::controller>(
					this_tag,
					ws.w.province_s.province_state_container.get<province_state::owner>(this_tag));
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

		ws.w.province_s.province_state_container.parallel_for_each([
			&ws, vector_size, full_vector_size,
			ppdt, mpdt, rpdt, cdt, mdt, ldt, pmpdt, mmpdt, rmpdt, plndt, mlndt, rlndt, pendt, mendt, rendt, pxndt, mxndt, rxndt
		](provinces::province_tag prov_id){
			auto& container = ws.w.province_s.province_state_container;

			auto pop_range = get_range(ws.w.population_s.pop_arrays, container.get<province_state::pops>(prov_id));

			Eigen::Map<Eigen::Matrix<int32_t, -1, 1>, Eigen::AlignmentType::Aligned32> province_partial_demo(ws.w.province_s.province_demographics.get_row(prov_id), vector_size);
			Eigen::Map<Eigen::Matrix<int32_t, -1, 1>, Eigen::AlignmentType::Aligned32> province_full_demo(ws.w.province_s.province_demographics.get_row(prov_id), full_vector_size);
			
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

			if(province_full_demo[to_index(population::total_population_tag)] != 0) {
				const auto culture_offset = population::to_demo_tag(ws, cultures::culture_tag(0));
				auto max_culture_off = maximum_index(province_full_demo.data() + to_index(culture_offset), int32_t(ws.s.culture_m.count_cultures));
				container.set<province_state::dominant_culture>(prov_id, cultures::culture_tag(static_cast<value_base_of<cultures::culture_tag>>(max_culture_off)));

				const auto religion_offset = population::to_demo_tag(ws, cultures::religion_tag(0));
				auto max_religion_off = maximum_index(province_full_demo.data() + to_index(religion_offset), int32_t(ws.s.culture_m.count_religions));
				container.set<province_state::dominant_religion>(prov_id, cultures::religion_tag(static_cast<value_base_of<cultures::religion_tag>>(max_religion_off)));

				const auto ideology_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));
				auto max_ideology_off = maximum_index(province_full_demo.data() + to_index(ideology_offset), int32_t(ws.s.ideologies_m.ideologies_count));
				container.set<province_state::dominant_ideology>(prov_id, ideologies::ideology_tag(static_cast<value_base_of<ideologies::ideology_tag>>(max_ideology_off)));
				
				const auto options_offset = population::to_demo_tag(ws, issues::option_tag(0));
				auto max_opinion_off = maximum_index(province_full_demo.data() + to_index(options_offset), int32_t(ws.s.issues_m.tracked_options_count));
				container.set<province_state::dominant_issue>(prov_id, issues::option_tag(static_cast<value_base_of<issues::option_tag>>(max_opinion_off)));
			}
		});
	}

	nations::country_tag get_province_seiger(world_state&, province_tag) {
		return nations::country_tag();
	}

	float get_life_rating(world_state const& ws, province_tag p) {
		auto& container = ws.w.province_s.province_state_container;
		return float(container.get<province_state::base_life_rating>(p)) * (1.0f + container.get<province_state::modifier_values>(p)[modifiers::provincial_offsets::life_rating]);
	}


	void silent_remove_province_owner(world_state& ws, province_tag p) {
		auto& container = ws.w.province_s.province_state_container;


		if(nations::country_tag& owner = container.get<province_state::owner>(p); is_valid_index(owner)) {
			nations::remove_owned_province(ws, owner, p);
			modifiers::detach_province_modifiers(ws, p, owner);
			owner = nations::country_tag();
		}

		container.set<province_state::has_owner_core>(p, false);

		if(nations::state_tag si = container.get<province_state::state_instance>(p); is_valid_index(si)) {
			nations::remove_province_from_state(ws, p);

			if(nations::is_state_empty(ws, si)) {
				nations::destroy_state_instance(ws, si);
				ws.w.nation_s.states.remove(si);
			}
		}
	}

	void silent_remove_province_controller(world_state& ws, province_tag prov) {
		auto& container = ws.w.province_s.province_state_container;

		if(nations::country_tag& controller = container.get<province_state::controller>(prov); is_valid_index(controller)) {
			nations::remove_controlled_province(ws, controller, prov);
			controller = nations::country_tag();
			container.set<province_state::last_controller_change>(prov, ws.w.current_date);
		}
	}

	void silent_set_province_controller(world_state& ws, nations::nation& new_controller, province_tag prov) {
		auto& container = ws.w.province_s.province_state_container;

		auto& controller = container.get<province_state::controller>(prov);
		if(controller != new_controller.id) {
			nations::remove_controlled_province(ws, controller, prov);
			nations::add_controlled_province(ws, new_controller.id, prov);
			controller = new_controller.id;

			container.set<province_state::last_controller_change>(prov, ws.w.current_date);
		}
	}

	void silent_set_province_owner(world_state& ws, nations::nation& new_owner, province_tag prov) {
		auto& container = ws.w.province_s.province_state_container;

		silent_remove_province_owner(ws, prov);
		nations::add_owned_province(ws, new_owner.id, prov);

		modifiers::attach_province_modifiers(ws, prov, new_owner.id);

		container.set<province_state::owner>(prov, new_owner.id);
		const auto region_id = ws.s.province_m.province_container.get<province::state_id>(prov);

		nations::region_state_pair* found = find(ws.w.nation_s.state_arrays, new_owner.member_states, nations::region_state_pair{ region_id, nullptr });
		if(found) {
			container.set<province_state::state_instance>(prov, found->state->id);
			found->state->state_capital = nations::find_state_capital(ws, *(found->state));
		} else {
			auto& new_state = nations::make_state(region_id, ws);
			container.set<province_state::state_instance>(prov, new_state.id);

			new_state.owner = &new_owner;
			new_state.state_capital = nations::find_state_capital(ws, new_state);
			add_item(ws.w.nation_s.state_arrays, new_owner.member_states, nations::region_state_pair{ region_id, &new_state });

			auto state_index = to_index(new_state.id);
			auto aligned_state_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(state_index + 1) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));
			ws.w.nation_s.nations.parallel_for_each([&ws, aligned_state_max, state_index, &new_owner](nations::nation& n){
				if(get_size(ws.w.economy_s.purchasing_arrays, n.statewise_tarrif_mask) < aligned_state_max)
					resize(ws.w.economy_s.purchasing_arrays, n.statewise_tarrif_mask, aligned_state_max);
				get(ws.w.economy_s.purchasing_arrays, n.statewise_tarrif_mask, state_index) = nations::tarrif_multiplier(ws, n, new_owner);
			});
		}

		auto cores_range = get_range(ws.w.province_s.core_arrays, container.get<province_state::cores>(prov));
		container.set<province_state::has_owner_core>(prov, std::find(cores_range.first, cores_range.second, new_owner.tag) != cores_range.second);
	}

	void silent_on_conquer_province(world_state& ws, province_tag prov) {
		auto& owner = ws.w.nation_s.nations[ws.w.province_s.province_state_container.get<province_state::owner>(prov)];
		auto owner_tag = owner.tag;
		if(is_valid_index(owner_tag)) {
			if(prov == ws.w.culture_s.national_tags_state[owner_tag].capital)
				owner.current_capital = prov;
		}
	}

	void enable_canal(world_state& ws, uint32_t canal_id) {
		ws.w.province_s.is_canal_enabled[canal_id] = 1ui8;
	}
	double distance(world_state const& ws, province_tag a, province_tag b) {
		auto& container = ws.s.province_m.province_container;
		return acos(container.get<province::centroid>(a).dot(container.get<province::centroid>(b))) * 40'075.0 / 6.2831853071;
	}

	constexpr double sea_cost_multiplier = 0.10; // cost havled for sea travel
	constexpr double border_transition = 50.0; // additional cost for a path through a border
	constexpr float maximum_distance = float(40'075.0 * 16.0);

	float with_adjustments_distance(
		world_state const& ws,
		province_tag a,
		province_tag b) {

		auto& container = ws.s.province_m.province_container;
		auto& state_container = ws.w.province_s.province_state_container;

		auto avg_movement_cost =
			(
			((container.get<province::is_sea>(a) ? sea_cost_multiplier : 1.0) * state_container.get<province_state::modifier_values>(a)[modifiers::provincial_offsets::movement_cost])
			+ ((container.get<province::is_sea>(b) ? sea_cost_multiplier : 1.0) * state_container.get<province_state::modifier_values>(b)[modifiers::provincial_offsets::movement_cost])
			) / 2.0;

		auto a_owner_i = state_container.get<province_state::owner>(a);
		auto b_owner_i = state_container.get<province_state::owner>(b);
		nations::nation* a_owner = is_valid_index(a_owner_i) ? &ws.w.nation_s.nations[a_owner_i] : nullptr;
		nations::nation* b_owner = is_valid_index(b_owner_i) ? &ws.w.nation_s.nations[b_owner_i] : nullptr;


		bool no_ff_nation_transition =
			a_owner == b_owner
			|| b_owner == nullptr
			|| a_owner == nullptr
			|| a_owner->sphere_leader == b_owner
			|| a_owner->overlord == b_owner
			|| b_owner->sphere_leader == a_owner
			|| b_owner->overlord == a_owner
			|| a_owner->sphere_leader == b_owner->sphere_leader
			|| a_owner->overlord == b_owner->overlord
			;
		if(no_ff_nation_transition)
			return float(avg_movement_cost * acos(container.get<province::centroid>(a).dot(container.get<province::centroid>(b))) * 40'075.0 / 6.2831853071);
		else
			return float(border_transition + avg_movement_cost * acos(container.get<province::centroid>(a).dot(container.get<province::centroid>(b))) * 40'075.0 / 6.2831853071);
	}

	struct province_distance {
		float distance;
		provinces::province_tag id;
		bool used_sea;
		province_tag origin;
	};

	void fill_distance_arrays(world_state& ws) {
		auto prov_count = ws.w.province_s.province_state_container.size();
		auto prov_distance_data = ws.w.province_s.province_distance_to.data();
		auto province_path_data = ws.w.province_s.province_path_to.data();

		ws.w.province_s.province_state_container.parallel_for_each(
			[&ws, prov_count, prov_distance_data, province_path_data](province_tag ps) {
			path_wise_distance_cost(ws, ps, prov_distance_data + to_index(ps) * prov_count, province_path_data + to_index(ps) * prov_count);
		});
	}

	void path_wise_distance_cost(world_state const& ws, province_tag a, float* results, province_tag* p_results) { // in ~km
		auto& container = ws.s.province_m.province_container;
		
		std::fill_n(results, ws.s.province_m.province_container.size(), maximum_distance);
		std::fill_n(p_results, ws.s.province_m.province_container.size(), province_tag());

		boost::container::small_vector<province_distance, 256, concurrent_allocator<province_distance>> unfinished;
		unfinished.push_back(province_distance{0.0f, a, false, province_tag()});

		while(unfinished.size() != 0) {
			province_distance current = unfinished.back();
			unfinished.pop_back();

			results[to_index(current.id)] = current.distance;
			p_results[to_index(current.id)] = current.origin;

			

			bool is_sea = container.get<province::is_sea>(current.id);
			auto same_adjacent = ws.s.province_m.same_type_adjacency.get_row(current.id);
			auto coastal_adjacent = ws.s.province_m.coastal_adjacency.get_row(current.id);

			for(auto p : same_adjacent) {

				if(results[to_index(p)] == maximum_distance) {
					auto distance = with_adjustments_distance(ws, current.id, p);

					[&unfinished, p, total_distance = distance + current.distance, current]() {
						const auto lend = unfinished.end().get_ptr();
						for(auto i = unfinished.begin().get_ptr(); i != lend; ++i) {
							if(i->id == p) {
								if(i->distance > total_distance) {
									i->distance = total_distance;
									i->used_sea = current.used_sea;
									i->origin = current.origin;
									auto new_upper_limit = std::upper_bound(i + 1, lend, *i, [](province_distance a, province_distance b) { return a.distance > b.distance; });
									if(new_upper_limit != i + 1)
										std::rotate(i, i + 1, new_upper_limit);
								}
								return;
							}
						}
						province_distance t{ total_distance, p , current.used_sea, is_valid_index(current.origin) ? current.origin : p};
						auto end_position = std::upper_bound(unfinished.begin(), unfinished.end(), t, [](province_distance a, province_distance b) { return a.distance > b.distance; });
						unfinished.insert(end_position, t);
					}();
				}
			}
			if(current.used_sea == false || is_sea) {
				for(auto p : coastal_adjacent) {

					if(results[to_index(p)] == maximum_distance) {
						auto distance = with_adjustments_distance(ws, current.id, p);

						[&unfinished, p, total_distance = distance + current.distance, current]() {
							const auto lend = unfinished.end().get_ptr();
							for(auto i = unfinished.begin().get_ptr(); i != lend; ++i) {
								if(i->id == p) {
									if(i->distance > total_distance) {
										i->distance = total_distance;
										i->used_sea = true;
										i->origin = current.origin;
										auto new_upper_limit = std::upper_bound(i + 1, lend, *i, [](province_distance a, province_distance b) { return a.distance > b.distance; });
										if(new_upper_limit != i + 1)
											std::rotate(i, i + 1, new_upper_limit);
									}
									return;
								}
							}
							province_distance t{ total_distance, p, true, is_valid_index(current.origin) ? current.origin : p };
							auto end_position = std::upper_bound(unfinished.begin(), unfinished.end(), t, [](province_distance a, province_distance b) { return a.distance > b.distance; });
							unfinished.insert(end_position, t);
						}();
					}
				}
			}
			
			//std::sort(unfinished.begin(), unfinished.end(), [](province_distance a, province_distance b) { return a.distance > b.distance; });
		}
	}

	void state_distances_manager::update(world_state const & ws) {
		auto max_states = ws.w.nation_s.states.minimum_continuous_size();
		auto aligned_state_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(max_states) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));

		if(last_aligned_state_max < int32_t(aligned_state_max)) {
			if(distance_data)
				_aligned_free(distance_data);
			distance_data = (float*)_aligned_malloc(sizeof(float) * aligned_state_max * aligned_state_max, 32);
			std::fill_n(distance_data, aligned_state_max * aligned_state_max, 0.0f);
			last_aligned_state_max = int32_t(aligned_state_max);
		}

		concurrency::parallel_for(0, int32_t(max_states), [&ws, max_states, aligned_state_max, d = this->distance_data](int32_t i) {
			nations::state_tag this_state = nations::state_tag(nations::state_tag::value_base_t(i));
			provinces::province_tag capital = ws.w.nation_s.states[this_state].state_capital;
			const auto province_count = ws.s.province_m.province_container.size();
			if(is_valid_index(capital)) {
				for(int32_t j = 0; j < int32_t(max_states); ++j) {
					nations::state_tag other_state = nations::state_tag(nations::state_tag::value_base_t(j));
					provinces::province_tag other_capital = ws.w.nation_s.states[other_state].state_capital;

					if(is_valid_index(other_capital)) {
						d[i * aligned_state_max + j] = ws.w.province_s.province_distance_to[
							to_index(capital) * province_count + to_index(other_capital)];
					}
				}
			}
		});
	}

	float state_distances_manager::distance(nations::state_tag a, nations::state_tag b) const {
		return distance_data[to_index(a) * last_aligned_state_max + to_index(b)];
	}
}
