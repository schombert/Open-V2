#pragma once
#include "common\\common.h"
#include "economy.h"

class world_state;

namespace nations {
	struct nation;
	struct state_instance;
}
namespace provinces {
	struct province_state;
}

namespace economy {
	void init_economic_scenario(world_state& ws);
	void reset_state(economic_state& s);

	production_modifiers rgo_production_modifiers(
		world_state const& ws,
		worked_instance const& instance,
		workers_information const& workers_info,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		int32_t rgo_base_size,
		float production_scale,
		economy::goods_tag production,
		float mobilization_effect);

	production_modifiers factory_production_modifiers(
		world_state const& ws,
		worked_instance const& instance,
		bonus* bonuses,
		workers_information const& workers_info,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		int32_t base_size,
		float production_scale,
		economy::goods_tag production,
		float mobilization_effect);

	production_modifiers artisan_production_modifiers(
		world_state const& ws,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		economy::goods_tag production,
		float mobilization_effect);

	void match_rgo_worker_type(world_state& ws, provinces::province_state& ps);

	void update_rgo_employment(world_state& ws, provinces::province_state& ps);
	void update_factories_employment(world_state& ws, nations::state_instance& si);
	int32_t rgo_max_employment(world_state const& ws, provinces::province_state& ps);

	void init_factory_employment(world_state& ws);
	float get_per_worker_profit(world_state const& ws, nations::state_instance const& si, factory_instance const& fi);
	float factory_employment_fraction(world_state const& ws, factory_instance const& fi);
	money_qnty_type get_factory_profit(world_state const& ws, provinces::province_state const& in_province, factory_instance const& f, money_qnty_type const* prices);
	bool possible_to_invest_in(world_state const& ws, nations::nation const& investor, nations::nation const& target);
	int32_t count_factories_in_state(nations::state_instance const& si);
	int32_t count_factories_in_nation(world_state const& ws, nations::nation const& n);
	float average_railroad_level(world_state const& ws, nations::state_instance const& si);
	bool factory_is_open(factory_instance const& fi);
	bool factory_is_closed(factory_instance const& fi); // NOTE: factories under construction will report as neither open nor closed
	bool factory_is_under_construction(factory_instance const& fi);
	bool factory_is_upgrading(factory_instance const& fi);

	factory_project_type get_factory_project_type(nations::state_instance const& location, factory_type_tag ftype);
	money_qnty_type get_factory_project_cost(world_state const& ws, factory_type_tag ftype, factory_project_type ptype, money_qnty_type const* prices);
	money_qnty_type get_railroad_cost(world_state const& ws, money_qnty_type const* prices);
	float project_completion(world_state const& ws, nations::state_instance const& si, money_qnty_type const* prices);
	uint32_t storage_space_for_n_neighbors(world_state const& ws, uint32_t neighbor_count);
	goods_qnty_type* imports_for_nth_neighbor(world_state const& ws, goods_qnty_type* data, uint32_t neighbor_count);
	void allocate_new_state_production(world_state& ws, nations::state_instance& si);
	money_qnty_type* state_current_prices(world_state const& ws, nations::state_instance const& si);

	void update_state_production_and_consumption(world_state& ws, nations::state_instance& si);
	void world_economy_update_tick(world_state& ws);
	void init_artisan_producation(world_state& ws);
}
