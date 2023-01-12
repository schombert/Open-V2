#include "common\\common.h"
#include "province_functions.hpp"
#include "modifiers\\modifiers.h"
#include "population\\population_functions.hpp"
#include "nations\\nations_functions.h"
#include "modifiers\\modifier_functions.h"
#include "military\\military_functions.h"
#include <ppl.h>
#include <random>
#include "concurrency_tools\\ve.h"

namespace provinces {

	void reset_state(provinces_state& s) {
		s.province_state_container.for_each([&s](provinces::province_tag p) {
			s.province_state_container.set<province_state::cores>(p, set_tag<cultures::national_tag>());
			s.province_state_container.set<province_state::fleets>(p, set_tag<military::fleet_presence>());
			s.province_state_container.set<province_state::orders>(p, military::army_orders_tag());
			s.province_state_container.set<province_state::controller>(p, nations::country_tag());
			s.province_state_container.set<province_state::static_modifiers>(p, set_tag<modifiers::provincial_modifier_tag>());
			s.province_state_container.set<province_state::timed_modifiers>(p, multiset_tag<provinces::timed_provincial_modifier>());
			s.province_state_container.set<province_state::pops>(p, array_tag<population::pop_tag, int32_t, false>());
		});
		s.core_arrays.reset();
		s.static_modifier_arrays.reset();
		s.timed_modifier_arrays.reset();
		s.province_arrays.reset();
	}

	void add_core(world_state& ws, province_tag prov, cultures::national_tag tag) {
		ws.add_item(ws.get<province_state::cores>(prov), tag);
		ws.add_item(ws.get<nation_tag::core_provinces>(tag), prov);
	}
	void remove_core(world_state& ws, province_tag prov, cultures::national_tag tag) {
		ws.remove_item(ws.get<province_state::cores>(prov), tag);
		ws.remove_item(ws.get<nation_tag::core_provinces>(tag), prov);
	}
	bool province_has_core(world_state& ws, province_tag prov, cultures::national_tag tag) {
		return ws.contains_item(ws.get<province_state::cores>(prov), tag);
	}

	void add_province_modifier(world_state& ws, provinces::province_tag p, modifiers::provincial_modifier_tag t) {
		ws.add_item(ws.get<province_state::static_modifiers>(p), t);
	}

	void add_timed_province_modifier(world_state& ws, provinces::province_tag p, modifiers::provincial_modifier_tag t, date_tag d) {
		ws.add_item(ws.get<province_state::timed_modifiers>(p), timed_provincial_modifier{ d, t });
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
				ws.s.province_m.integer_to_province[i],
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
		//ve::copy(ve::to_vector_size(uint32_t(ws.s.province_m.province_container.size() + 1)),
		//	ws.w.province_s.province_state_container.get_row<province_state::last_population>(),
		//	ws.w.province_s.province_state_container.get_row<province_state::total_population>(),
		//	ve::par());
		recalculate_province_demographics(ws);
	}

	void recalculate_province_demographics(world_state& ws) {
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
		](provinces::province_tag prov_id) {
			if(to_index(prov_id) >= ws.s.province_m.first_sea_province)
				return;

			auto& container = ws.w.province_s.province_state_container;

			auto pop_range = get_range(ws.w.population_s.pop_arrays, container.get<province_state::pops>(prov_id));

			auto province_full_demo = ws.w.province_s.province_demographics.get_row(prov_id);

			ve::set_zero(ve::to_vector_size(ws.w.province_s.province_demographics.inner_size()), province_full_demo);
			const auto pop_demo_size = ws.w.population_s.pop_demographics.inner_size;

			for(auto p : pop_range) {
				auto pop_demo_source = ws.w.population_s.pop_demographics.get_row(p);
				ve::accumulate(pop_demo_size, province_full_demo, pop_demo_source, ve::serial_exact());

				auto ptype = ws.w.population_s.pops.get<pop::type>(p);
				auto needs_satisfaction = ws.w.population_s.pops.get<pop::needs_satisfaction>(p);

				auto pop_size = pop_demo_source[population::total_population_tag];
				auto other_pop_size = ws.w.population_s.pops.get<pop::size>(p);
				assert(std::isfinite(pop_size) && pop_size > 0.0f && pop_size == other_pop_size);

				province_full_demo[population::to_demo_tag(ws, ws.w.population_s.pops.get<pop::culture>(p))] += pop_size;
				province_full_demo[population::to_demo_tag(ws, ws.w.population_s.pops.get<pop::religion>(p))] += pop_size;
				province_full_demo[population::to_demo_tag(ws, ptype)] += pop_size;
				province_full_demo[population::to_employment_demo_tag(ws, ptype)] += pop_demo_source[population::total_employment_tag];


				province_full_demo[cdt] += ws.w.population_s.pops.get<pop::consciousness>(p) * pop_size;
				province_full_demo[ldt] += ws.w.population_s.pops.get<pop::literacy>(p) * pop_size;

				const float weighted_militancy = ws.w.population_s.pops.get<pop::militancy>(p) * pop_size;
				province_full_demo[mdt] += weighted_militancy;

				const auto strata = ws.s.population_m.pop_types[ptype].flags & population::pop_type::strata_mask;
				if(strata == population::pop_type::strata_poor) {
					province_full_demo[pmpdt] += weighted_militancy;
					province_full_demo[ppdt] += pop_size;
					if(needs_satisfaction < 1.0f) {
						province_full_demo[plndt] += float(pop_size) * needs_satisfaction;
					} else {
						province_full_demo[plndt] += pop_size;
						if(needs_satisfaction < 2.0f) {
							province_full_demo[pendt] += float(pop_size) * (needs_satisfaction - 1.0f);
						} else {
							province_full_demo[pendt] += pop_size;
							if(needs_satisfaction < 3.0f) {
								province_full_demo[pxndt] += float(pop_size) * (needs_satisfaction - 2.0f);
							} else {
								province_full_demo[pxndt] += pop_size;
							}
						}
					}
				} else if(strata == population::pop_type::strata_middle) {
					province_full_demo[mmpdt] += weighted_militancy;
					province_full_demo[mpdt] += pop_size;
					if(needs_satisfaction < 1.0f) {
						province_full_demo[mlndt] += float(pop_size) * needs_satisfaction;
					} else {
						province_full_demo[mlndt] += pop_size;
						if(needs_satisfaction < 2.0f) {
							province_full_demo[mendt] += float(pop_size) * (needs_satisfaction - 1.0f);
						} else {
							province_full_demo[mendt] += pop_size;
							if(needs_satisfaction < 3.0f) {
								province_full_demo[mxndt] += float(pop_size) * (needs_satisfaction - 2.0f);
							} else {
								province_full_demo[mxndt] += pop_size;
							}
						}
					}
				} else /*if(strata == population::pop_type::strata_rich)*/ {
					province_full_demo[rmpdt] += weighted_militancy;
					province_full_demo[rpdt] += pop_size;
					if(needs_satisfaction < 1.0f) {
						province_full_demo[rlndt] += float(pop_size) * needs_satisfaction;
					} else {
						province_full_demo[rlndt] += pop_size;
						if(needs_satisfaction < 2.0f) {
							province_full_demo[rendt] += float(pop_size) * (needs_satisfaction - 1.0f);
						} else {
							province_full_demo[rendt] += pop_size;
							if(needs_satisfaction < 3.0f) {
								province_full_demo[rxndt] += float(pop_size) * (needs_satisfaction - 2.0f);
							} else {
								province_full_demo[rxndt] += pop_size;
							}
						}
					}
				}
			}

			if(province_full_demo[population::total_population_tag] != 0) {
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

			ws.w.province_s.province_state_container.set<province_state::total_population>(prov_id, province_full_demo[population::total_population_tag]);
		});
	}

	nations::country_tag get_province_seiger(world_state&, province_tag) {
		return nations::country_tag();
	}


	void silent_remove_province_owner(world_state& ws, province_tag p) {
		auto& container = ws.w.province_s.province_state_container;


		if(auto& owner = container.get<province_state::owner>(p); is_valid_index(owner)) {
			nations::remove_owned_province(ws, owner, p);
			modifiers::detach_province_modifiers(ws, p, owner);
			owner = nations::country_tag();
		}

		container.set<province_state::has_owner_core>(p, false);

		if(nations::state_tag si = container.get<province_state::state_instance>(p); is_valid_index(si)) {
			nations::remove_province_from_state(ws, p);

			if(nations::is_state_empty(ws, si)) {
				nations::destroy_state_instance(ws, si);
				ws.w.nation_s.states.release(si);
			}
		}
	}

	void silent_remove_province_controller(world_state& ws, province_tag prov) {
		auto& container = ws.w.province_s.province_state_container;

		if(auto& controller = container.get<province_state::controller>(prov); is_valid_index(controller)) {
			nations::remove_controlled_province(ws, controller, prov);
			controller = nations::country_tag();
			container.set<province_state::last_controller_change>(prov, ws.w.current_date);
		}
	}

	void silent_set_province_controller(world_state& ws, nations::country_tag new_controller, province_tag prov) {
		auto& container = ws.w.province_s.province_state_container;

		auto& controller = container.get<province_state::controller>(prov);
		if(controller != new_controller) {
			if(is_valid_index(controller))
				nations::remove_controlled_province(ws, controller, prov);
			nations::add_controlled_province(ws, new_controller, prov);
			controller = new_controller;

			container.set<province_state::last_controller_change>(prov, ws.w.current_date);
		}
	}

	void silent_set_province_owner(world_state& ws, nations::country_tag new_owner, province_tag prov) {
		auto& container = ws.w.province_s.province_state_container;

		silent_remove_province_owner(ws, prov);
		nations::add_owned_province(ws, new_owner, prov);

		modifiers::attach_province_modifiers(ws, prov, new_owner);

		container.set<province_state::owner>(prov, new_owner);
		const auto region_id = ws.s.province_m.province_container.get<province::state_id>(prov);

		nations::region_state_pair* found = find(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(new_owner), nations::region_state_pair{ region_id, nations::state_tag() });
		if(found) {
			container.set<province_state::state_instance>(prov, found->state);
			container.set<province_state::is_non_state>(prov, ws.w.nation_s.states.get<state::is_colonial>(found->state) || ws.w.nation_s.states.get<state::is_protectorate>(found->state));
			ws.w.nation_s.states.set<state::state_capital>(found->state, nations::find_state_capital(ws, found->state));

		} else {
			auto new_state = nations::make_state(region_id, ws);
			container.set<province_state::state_instance>(prov, new_state);
			container.set<province_state::is_non_state>(prov, false);

			ws.w.nation_s.states.set<state::owner>(new_state, new_owner);
			ws.w.nation_s.states.set<state::state_capital>(new_state, nations::find_state_capital(ws, new_state));
			add_item(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(new_owner), nations::region_state_pair{ region_id, new_state });

			auto aligned_state_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(ws.w.nation_s.states.size()) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));
			ws.w.nation_s.nations.parallel_for_each([&ws, aligned_state_max, new_state, new_owner](nations::country_tag n) {
				auto& tfmask = ws.w.nation_s.nations.get<nation::statewise_tariff_mask>(n);
				if(get_size(ws.w.economy_s.purchasing_arrays, tfmask) < aligned_state_max)
					resize(ws.w.economy_s.purchasing_arrays, tfmask, aligned_state_max);
				get(ws.w.economy_s.purchasing_arrays, tfmask, new_state) = nations::tarrif_multiplier(ws, n, new_owner);
			});
		}

		auto cores_range = get_range(ws.w.province_s.core_arrays, container.get<province_state::cores>(prov));
		container.set<province_state::has_owner_core>(prov, std::find(cores_range.first, cores_range.second, ws.w.nation_s.nations.get<nation::tag>(new_owner)) != cores_range.second);

		for_each_pop(ws, prov, [&ws, new_owner](population::pop_tag p) {
			auto pc = ws.w.population_s.pops.get<pop::culture>(p);
			ws.w.population_s.pops.set<pop::is_accepted>(p, nations::is_culture_accepted(ws, pc, new_owner));
		});
	}

	void silent_on_conquer_province(world_state& ws, province_tag prov) {
		auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(prov);
		auto owner_tag = ws.w.nation_s.nations.get<nation::tag>(owner);
		if(is_valid_index(owner_tag)) {
			if(prov == ws.w.culture_s.national_tags_state[owner_tag].capital)
				ws.w.nation_s.nations.set<nation::current_capital>(owner, prov);
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
			((container.get<province::is_sea>(a) ? sea_cost_multiplier : 1.0) * ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::movement_cost>(a))
				+ ((container.get<province::is_sea>(b) ? sea_cost_multiplier : 1.0) * ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::movement_cost>(b))
				) / 2.0;

		auto a_owner = state_container.get<province_state::owner>(a);
		auto b_owner = state_container.get<province_state::owner>(b);

		auto a_sphere_leader = a_owner ? nations::country_tag(ws.w.nation_s.nations.get<nation::sphere_leader>(a_owner)) : nations::country_tag();
		auto a_overlord = a_owner ? nations::country_tag(ws.w.nation_s.nations.get<nation::overlord>(a_owner)) : nations::country_tag();
		auto b_sphere_leader = b_owner ? nations::country_tag(ws.w.nation_s.nations.get<nation::sphere_leader>(b_owner)) : nations::country_tag();
		auto b_overlord = b_owner ? nations::country_tag(ws.w.nation_s.nations.get<nation::overlord>(b_owner)) : nations::country_tag();

		bool no_ff_nation_transition =
			a_owner == b_owner
			|| b_owner == nations::country_tag()
			|| a_owner == nations::country_tag()
			|| a_sphere_leader == b_owner
			|| a_overlord == b_owner
			|| b_sphere_leader == a_owner
			|| b_overlord == a_owner
			|| a_sphere_leader == b_sphere_leader
			|| a_overlord == b_overlord
			;
		if(no_ff_nation_transition)
			return float(avg_movement_cost * acos(container.get<province::centroid>(a).dot(container.get<province::centroid>(b))) * 40'075.0 / 6.2831853071);
		else
			return float(border_transition + avg_movement_cost * acos(container.get<province::centroid>(a).dot(container.get<province::centroid>(b))) * 40'075.0 / 6.2831853071);
	}
	
	constexpr float non_allied_factor = 10'000.0f;

	float military_with_adjustments_distance(
		world_state const& ws,
		province_tag a,
		province_tag b,
		set_tag<nations::country_tag> source_allies) {

		auto& container = ws.s.province_m.province_container;

		auto avg_movement_cost =
			(
			((container.get<province::is_sea>(a) ? sea_cost_multiplier : 1.0) * ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::movement_cost>(a))
				+ ((container.get<province::is_sea>(b) ? sea_cost_multiplier : 1.0) * ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::movement_cost>(b))
				) / 2.0;

		if(ws.contains_item(source_allies, ws.get<province_state::controller>(b)))
			return float(avg_movement_cost * acos(container.get<province::centroid>(a).dot(container.get<province::centroid>(b))) * 40'075.0 / 6.2831853071);
		else
			return non_allied_factor * float(avg_movement_cost * acos(container.get<province::centroid>(a).dot(container.get<province::centroid>(b))) * 40'075.0 / 6.2831853071);
	}

	struct province_distance {
		float distance;
		provinces::province_tag id;
		bool used_sea;
		province_tag origin;
	};

	void fill_distance_arrays(world_state& ws) {
		auto prov_count = ws.s.province_m.province_container.size();
		auto prov_distance_data = ws.w.province_s.province_distance_to.data();
		auto province_path_data = ws.w.province_s.province_path_to.data();

		ws.w.province_s.province_state_container.parallel_for_each(
			[&ws, prov_count, prov_distance_data, province_path_data](province_tag ps) {
			path_wise_distance_cost(ws, ps, prov_distance_data + to_index(ps) * prov_count, province_path_data + to_index(ps) * prov_count);
		});
	}


	template <typename ITTYPE, typename COMP>
	void sift_up(ITTYPE first, ITTYPE pos, COMP && cmp) {
		using std::iter_swap;

		while(pos != first) {
			auto parent = first + (pos - first - 1) / 2;
			if(!cmp(*parent, *pos))
				return;
			std::iter_swap(parent, pos);
			pos = parent;
		}
	}

	void old_path_wise_distance_cost(world_state const& ws, province_tag a, float* results, province_tag* p_results) { // in ~km
		auto& container = ws.s.province_m.province_container;

		std::fill_n(results, ws.s.province_m.province_container.size(), maximum_distance);
		std::fill_n(p_results, ws.s.province_m.province_container.size(), province_tag());

		boost::container::small_vector<province_distance, 256, concurrent_allocator<province_distance>> unfinished;
		unfinished.push_back(province_distance{ 0.0f, a, false, province_tag() });

		while(unfinished.size() != 0) {
			std::pop_heap(unfinished.begin(), unfinished.end(), [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
			province_distance current = unfinished.back();
			unfinished.pop_back();

			results[to_index(current.id)] = current.distance;
			p_results[to_index(current.id)] = current.origin;



			bool is_sea = container.get<province::is_sea>(current.id);
			auto same_adjacent = ws.s.province_m.same_type_adjacency.get_range(current.id);
			auto coastal_adjacent = ws.s.province_m.coastal_adjacency.get_range(current.id);

			for(auto p : same_adjacent) {

				if(results[to_index(p)] == maximum_distance) {
					auto distance = with_adjustments_distance(ws, current.id, p);
					const auto total_distance = distance + current.distance;
					
					if(auto const f = std::find_if(unfinished.begin(), unfinished.end(), [p](province_distance const& a) { return a.id == p; });
						f != unfinished.end()) {

						if(f->distance > total_distance) {
							f->distance = total_distance;
							f->used_sea = current.used_sea;
							f->origin = current.origin;

							sift_up(unfinished.begin(), f, [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
						}
					} else {
						province_distance t{ total_distance, p , current.used_sea, is_valid_index(current.origin) ? current.origin : p };
						unfinished.push_back(t);
						std::push_heap(unfinished.begin(), unfinished.end(), [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
					}
				}
			}
			if(current.used_sea == false || is_sea) {
				for(auto p : coastal_adjacent) {

					if(results[to_index(p)] == maximum_distance) {
						auto distance = with_adjustments_distance(ws, current.id, p);
						const auto total_distance = distance + current.distance;

						if(auto const f = std::find_if(unfinished.begin(), unfinished.end(), [p](province_distance const& a) { return a.id == p; });
							f != unfinished.end()) {

							if(f->distance > total_distance) {
								f->distance = total_distance;
								f->used_sea = true;
								f->origin = current.origin;

								sift_up(unfinished.begin(), f, [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
							}
						} else {
							province_distance t{ total_distance, p , true, is_valid_index(current.origin) ? current.origin : p };
							unfinished.push_back(t);
							std::push_heap(unfinished.begin(), unfinished.end(), [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
						}
					}
				}
			}

			//std::sort(unfinished.begin(), unfinished.end(), [](province_distance a, province_distance b) { return a.distance > b.distance; });
		}
	}

	// variant checks for duplication upon taking out of unfinished list rather than upon addition
	// trades speed of insertion to list against larger list size

	void path_wise_distance_cost(world_state const& ws, province_tag a, float* results, province_tag* p_results) { // in ~km
		auto& container = ws.s.province_m.province_container;

		std::fill_n(results, ws.s.province_m.province_container.size(), maximum_distance);
		std::fill_n(p_results, ws.s.province_m.province_container.size(), province_tag());

		boost::container::small_vector<province_distance, 256, concurrent_allocator<province_distance>> unfinished;
		unfinished.push_back(province_distance{ 0.0f, a, false, province_tag() });

		while(unfinished.size() != 0) {
			std::pop_heap(unfinished.begin(), unfinished.end(), [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
			province_distance current = unfinished.back();
			unfinished.pop_back();

			if(results[to_index(current.id)] > current.distance) {
				results[to_index(current.id)] = current.distance;
				p_results[to_index(current.id)] = current.origin;



				bool is_sea = container.get<province::is_sea>(current.id);
				auto same_adjacent = ws.s.province_m.same_type_adjacency.get_range(current.id);
				auto coastal_adjacent = ws.s.province_m.coastal_adjacency.get_range(current.id);

				for(auto p : same_adjacent) {
					if(results[to_index(p)] == maximum_distance) {
						auto distance = with_adjustments_distance(ws, current.id, p);

						province_distance t{ distance + current.distance, p , current.used_sea, is_valid_index(current.origin) ? current.origin : p };
						//auto end_position = std::upper_bound(unfinished.begin(), unfinished.end(), t, [](province_distance a, province_distance b) { return a.distance > b.distance; });
						unfinished.push_back(t);
						std::push_heap(unfinished.begin(), unfinished.end(), [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
					}
				}
				if(current.used_sea == false || is_sea) {
					for(auto p : coastal_adjacent) {
						if(results[to_index(p)] == maximum_distance) {
							auto distance = with_adjustments_distance(ws, current.id, p);

							province_distance t{ distance + current.distance, p, true, is_valid_index(current.origin) ? current.origin : p };
							//auto end_position = std::upper_bound(unfinished.begin(), unfinished.end(), t, [](province_distance a, province_distance b) { return a.distance > b.distance; });
							unfinished.push_back(t);
							std::push_heap(unfinished.begin(), unfinished.end(), [](province_distance const& a, province_distance const& b) { return a.distance > b.distance; });
						}
					}
				}
			}
		}
	}

	struct military_province_distance {
		float distance;
		provinces::province_tag id;
	};

	void military_path_wise_distance_cost(world_state const& ws, province_tag a, nations::country_tag border_from, nations::country_tag border_to,
		tagged_array_view<float, provinces::province_tag> max_distances) { // in ~km

		std::fill_n(max_distances.data(), ws.s.province_m.province_container.size(), maximum_distance * non_allied_factor);
		auto const source_owner = ws.get<province_state::owner>(a);
		auto const source_allies = ws.get<nation::allies_in_war>(source_owner);

		boost::container::small_vector<military_province_distance, 256, concurrent_allocator<military_province_distance>> unfinished;
		unfinished.push_back(military_province_distance{ 0.0f, a, });

		while(unfinished.size() != 0) {
			std::pop_heap(unfinished.begin(), unfinished.end(), [](military_province_distance const& a, military_province_distance const& b) { return a.distance > b.distance; });
			military_province_distance current = unfinished.back();
			unfinished.pop_back();

			if(max_distances[current.id] > current.distance) {
				max_distances[current.id] = current.distance;

				auto same_adjacent = ws.s.province_m.same_type_adjacency.get_range(current.id);
				auto coastal_adjacent = ws.s.province_m.coastal_adjacency.get_range(current.id);

				for(auto p : same_adjacent) {
					if(max_distances[p] == maximum_distance) {
						auto distance = military_with_adjustments_distance(ws, current.id, p, source_allies);

						military_province_distance t{ distance + current.distance, p };
						unfinished.push_back(t);
						std::push_heap(unfinished.begin(), unfinished.end(), [](military_province_distance const& a, military_province_distance const& b) { return a.distance > b.distance; });
					}
				}
				
				for(auto p : coastal_adjacent) {
					if(max_distances[p] == maximum_distance) {
						auto distance = military_with_adjustments_distance(ws, current.id, p, source_allies);

						military_province_distance t{ distance + current.distance, p };
						unfinished.push_back(t);
						std::push_heap(unfinished.begin(), unfinished.end(), [](military_province_distance const& a, military_province_distance const& b) { return a.distance > b.distance; });
					}
				}
			}
		}

	}

	void state_distances_manager::update(world_state const & ws) {
		auto max_states = ws.w.nation_s.states.size() + 1;
		auto aligned_state_max = ((static_cast<uint32_t>(sizeof(float)) * uint32_t(max_states) + 63ui32) & ~63ui32) / static_cast<uint32_t>(sizeof(float));

		if(last_aligned_state_max < int32_t(aligned_state_max)) {
			if(distance_data)
				_aligned_free(distance_data - 15);
			distance_data = (float*)_aligned_malloc(sizeof(float) * aligned_state_max * aligned_state_max + 64, 64) + 15;
			std::fill_n(distance_data, aligned_state_max * aligned_state_max, 0.0f);
			last_aligned_state_max = int32_t(aligned_state_max);
		}

		concurrency::parallel_for(0, int32_t(max_states), [&ws, max_states, aligned_state_max, d = this->distance_data](int32_t i) {
			nations::state_tag this_state = nations::state_tag(nations::state_tag::value_base_t(i));
			provinces::province_tag capital = ws.w.nation_s.states.get<state::state_capital>(this_state);
			const auto province_count = ws.s.province_m.province_container.size();
			if(is_valid_index(capital)) {
				for(int32_t j = 0; j < int32_t(max_states); ++j) {
					nations::state_tag other_state = nations::state_tag(nations::state_tag::value_base_t(j));
					provinces::province_tag other_capital = ws.w.nation_s.states.get<state::state_capital>(other_state);

					if(is_valid_index(other_capital)) {
						d[(i + 1) * aligned_state_max + (j + 1)] = ws.w.province_s.province_distance_to[
							to_index(capital) * province_count + to_index(other_capital)];
					}
				}
			}
		});
	}

	float state_distances_manager::distance(nations::state_tag a, nations::state_tag b) const {
		return distance_data[(to_index(a) + 1) * last_aligned_state_max + to_index(b) + 1];
	}

	void ready_initial_province_statistics(world_state& ws) {
		for(int32_t i = 0; i < ws.s.province_m.province_container.size(); ++i) {
			provinces::province_tag t = provinces::province_tag(provinces::province_tag::value_base_t(i));
			auto const p_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(t));
			float const total = std::transform_reduce(
				p_range.first,
				p_range.second,
				0.0f,
				std::plus<>(),
				[&ws](population::pop_tag p) { return ws.w.population_s.pops.get<pop::size>(p); });
			ws.w.province_s.province_state_container.set<province_state::monthly_population>(t, total);
			ws.w.province_s.province_state_container.set<province_state::old_monthly_population>(t, total);
		}
	}

	constexpr uint32_t crime_update_frequency = 32ui32;
	constexpr uint32_t crime_update_size = uint32_t(std::max(16, ve::vector_size));

	void update_crime(world_state& ws) {
		const uint32_t index = uint32_t(to_index(ws.w.current_date) & (crime_update_frequency - 1));
		auto const part = ve::generate_partition_range<crime_update_frequency, crime_update_size>(
			index, ws.s.province_m.first_sea_province);

		concurrency::parallel_for(part.low, part.high, [&ws](uint32_t i) {
			auto const val = provinces::province_tag(provinces::province_tag::value_base_t(i));

			auto cf_value = crime_fighting_value(ws, val);
			auto& current_crime = ws.w.province_s.province_state_container.get<province_state::crime>(val);


			std::uniform_real_distribution<float> dist(0.0f, 1.0f);
			auto const result = dist(get_local_generator());

			if(result < cf_value) {
				current_crime = modifiers::provincial_modifier_tag();
			} else if(!current_crime) {
				std::uniform_int_distribution<int32_t> idist(0, int32_t(ws.s.modifiers_m.crimes.size()));
				auto const index = idist(get_local_generator());
				auto prov_owner = province_owner(ws, val);

				if(bit_vector_test((bitfield_type*)(&(ws.w.nation_s.nations.get<nation::enabled_crimes>(prov_owner))), uint32_t(index)))
					current_crime = ws.s.modifiers_m.crimes[index].modifier;
			}
		});
	}

	auto get_connected_owned_provinces(world_state const& ws, provinces::province_tag start)->boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>> {
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>> already_added;
		boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>> pending_neighbor_check;

		pending_neighbor_check.insert(start);
		auto const first_owner = ws.get<province_state::owner>(start);

		while(pending_neighbor_check.size() != 0) {
			auto tp = *(pending_neighbor_check.end() - 1);
			pending_neighbor_check.erase(pending_neighbor_check.end() - 1);
			already_added.insert(tp);

			auto adj_range = ws.s.province_m.same_type_adjacency.get_range(tp);
			for(auto a : adj_range) {
				if(ws.w.province_s.province_state_container.get<province_state::owner>(a) == first_owner && already_added.count(a) == 0 && pending_neighbor_check.count(a) == 0)
					pending_neighbor_check.insert(a);
			}
		}

		return already_added;
	}


	auto provinces_are_same_type_adjacent(world_state const& ws, provinces::province_tag a, provinces::province_tag b) -> bool {
		auto adj_range = ws.s.province_m.same_type_adjacency.get_range(a);
		for(auto j : adj_range) {
			if(j == b)
				return true;
		}
		return false;
	}
}
