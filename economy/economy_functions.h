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
}
