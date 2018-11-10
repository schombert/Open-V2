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
		world_state const& ws,
		worked_instance const& instance,
		bonus* bonuses,
		workers_information const& workers_info,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		int32_t base_size,
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
	money_qnty_type* state_current_prices(world_state const& ws, nations::state_tag s);
	money_qnty_type* state_old_prices(world_state const& ws, nations::state_tag s);
	goods_qnty_type* state_current_production(world_state const& ws, nations::state_tag s);
	money_qnty_type* state_current_demand(world_state const& ws, nations::state_tag s);

	struct range_information {
		float minimum;
		float maximum;
		float average;
	};

	range_information global_price_range(world_state const& ws, economy::goods_tag t);
	money_qnty_type get_life_needs_cost(world_state const& ws, nations::state_instance const& si, population::pop_type_tag ptype);
	artisan_type_tag get_profitable_artisan(world_state const& ws, provinces::province_state const& ps);

	void init_artisan_producation(world_state& ws);
	void set_initial_money(world_state& ws);
	void collect_taxes(world_state& ws);
	void pay_unemployement_pensions_salaries(world_state& ws, nations::nation& n);
	void economy_update_tick(world_state& ws);
	void economy_demand_adjustment_tick(world_state& ws);

	economy::money_qnty_type project_player_tarrif_income(world_state const& ws, float tarrif_amount);

	money_qnty_type military_spending_amount(world_state const& ws, nations::nation const& n);
	money_qnty_type social_spending_amount(world_state const& ws, nations::nation const& n);
	money_qnty_type education_spending_amount(world_state const& ws, nations::nation const& n);
	money_qnty_type administrative_spending_amount(world_state const& ws, nations::nation const& n);
}
