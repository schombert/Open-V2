#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "concurrency_tools\\ve.h"

namespace current_state {
	class state;
}
class world_state;

namespace provinces {
	class provinces_state;

	void add_core(world_state& ws, province_tag prov, cultures::national_tag tag);
	void remove_core(world_state& ws, province_tag prov, cultures::national_tag tag);
	bool province_has_core(world_state& ws, province_tag prov, cultures::national_tag tag);
	void init_province_state(world_state& ws);
	void update_province_demographics(world_state& ws); // move total population to old population, then recalculate
	void recalculate_province_demographics(world_state& ws);
	void add_province_modifier(world_state& ws, province_tag p, modifiers::provincial_modifier_tag t);
	void add_timed_province_modifier(world_state& ws, province_tag p, modifiers::provincial_modifier_tag t, date_tag d);
	nations::country_tag get_province_seiger(world_state& ws, province_tag p);
	void reset_state(provinces_state& s);

	template<typename T>
	auto get_life_rating(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f));
	template<typename T>
	auto crime_fighting_value(world_state const& ws, T p) -> decltype(ve::widen_to<T>(0.0f));
	template<typename F>
	void for_each_pop(world_state const& ws, province_tag p, F&& f);
	template<typename T>
	auto province_owner(world_state const& ws, T p) -> decltype(ve::widen_to<T>(nations::country_tag()));
	template<typename T>
	auto province_controller(world_state const& ws, T p) -> decltype(ve::widen_to<T>(nations::country_tag()));
	template<typename T>
	auto province_state(world_state const& ws, T p) -> decltype(ve::widen_to<T>(nations::state_tag()));

	auto get_connected_owned_provinces(world_state const& ws, provinces::province_tag start) -> boost::container::flat_set<provinces::province_tag, std::less<provinces::province_tag>, concurrent_allocator<provinces::province_tag>>;

	void silent_remove_province_owner(world_state& ws, province_tag p);
	void silent_remove_province_controller(world_state& ws, province_tag p);
	void silent_set_province_owner(world_state& ws, nations::country_tag new_owner, province_tag p);
	void silent_set_province_controller(world_state& ws, nations::country_tag new_controller, province_tag p);
	void silent_on_conquer_province(world_state& ws, province_tag p);
	void enable_canal(world_state& ws, uint32_t canal_id);
	double distance(world_state const& ws, province_tag a, province_tag b); // in km
	void path_wise_distance_cost(world_state const& ws, province_tag a, float* results, province_tag* p_results); // in ~km
	void fill_distance_arrays(world_state& ws);

	void ready_initial_province_statistics(world_state& ws);

	void update_crime(world_state& ws);
}
