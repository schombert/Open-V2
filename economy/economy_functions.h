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
		economy::goods_tag production,
		float mobilization_effect);

	production_modifiers factory_production_modifiers(
		world_state& ws,
		worked_instance const& instance,
		bonus* bonuses,
		workers_information const& workers_info,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		int32_t base_size,
		economy::goods_tag production,
		float mobilization_effect);

	void match_rgo_worker_type(world_state& ws, provinces::province_state& ps);

	void update_rgo_employment(world_state& ws, provinces::province_state& ps);
	void update_factories_employment(world_state& ws, nations::state_instance& si);
	int32_t rgo_max_employment(world_state const& ws, provinces::province_state& ps);

	void init_factory_employment(world_state& ws);
	float get_per_worker_profit(world_state const& ws, nations::state_instance const& si, factory_instance const& fi);
}
