#pragma once
#include "common\\common.h"
#include "economy.h"

class world_state;

namespace economy {
	void init_economic_scenario(world_state& ws);
	void reset_state(economic_state& s);

	production_modifiers rgo_production_modifiers(
		world_state const& ws,
		worked_instance const& instance,
		workers_information const& workers_info,
		nations::country_tag in_nation,
		provinces::province_tag in_province,
		int32_t rgo_base_size,
		economy::goods_tag production,
		float mobilization_effect);

	production_modifiers factory_production_modifiers(
		world_state const& ws,
		worked_instance const& instance,
		bonus* bonuses,
		workers_information const& workers_info,
		nations::country_tag in_nation,
		provinces::province_tag in_province,
		int32_t base_size,
		economy::goods_tag production,
		float mobilization_effect);

	production_modifiers artisan_production_modifiers(
		world_state const& ws,
		nations::country_tag in_nation,
		provinces::province_tag in_province,
		economy::goods_tag production,
		float mobilization_effect);

	void match_rgo_worker_type(world_state& ws, provinces::province_tag ps);

	void update_rgo_employment(world_state& ws, provinces::province_tag ps);
	void update_factories_employment(world_state& ws, nations::state_tag si);
	int32_t rgo_max_employment(world_state const& ws, provinces::province_tag ps);

	void init_factory_employment(world_state& ws);
	float get_per_worker_profit(world_state const& ws, nations::state_tag si, factory_instance const& fi);
	float factory_employment_fraction(world_state const& ws, factory_instance const& fi);
	money_qnty_type get_factory_profit(world_state const& ws, provinces::province_tag in_province, factory_instance const& f, tagged_array_view<const float, goods_tag> prices);
	bool possible_to_invest_in(world_state const& ws, nations::country_tag investor, nations::country_tag target);
	int32_t count_factories_in_state(world_state const& ws, nations::state_tag s);
	int32_t count_factories_in_nation(world_state const& ws, nations::country_tag n);
	float average_railroad_level(world_state const& ws, nations::state_tag si);
	bool factory_is_open(factory_instance const& fi);
	bool factory_is_closed(factory_instance const& fi); // NOTE: factories under construction will report as neither open nor closed
	bool factory_is_under_construction(factory_instance const& fi);
	bool factory_is_upgrading(factory_instance const& fi);

	goods_qnty_type province_rgo_production_qnty(world_state const& ws, provinces::province_tag p);

	factory_project_type get_factory_project_type(world_state const& ws, nations::state_tag location, factory_type_tag ftype);
	money_qnty_type get_factory_project_cost(world_state const& ws, factory_type_tag ftype, factory_project_type ptype, tagged_array_view<const float, goods_tag> prices);
	money_qnty_type get_railroad_cost(world_state const& ws, tagged_array_view<const float, goods_tag> prices);
	float project_completion(world_state const& ws, nations::state_tag si, tagged_array_view<const float, goods_tag> prices);
	tagged_array_view<const float, goods_tag> state_current_prices(world_state const& ws, nations::state_tag s);
	tagged_array_view<const float, goods_tag> state_price_delta(world_state const& ws, nations::state_tag s);
	float state_current_production(world_state const& ws, nations::state_tag s, goods_tag g);
	tagged_array_view<const float, goods_tag> state_current_demand(world_state const& ws, nations::state_tag s);
	tagged_array_view<float, goods_tag> state_current_prices(world_state& ws, nations::state_tag s);
	tagged_array_view<float, goods_tag> state_price_delta(world_state& ws, nations::state_tag s);
	tagged_array_view<float, goods_tag> state_current_demand(world_state& ws, nations::state_tag s);
	money_qnty_type calculate_daily_debt_payment(world_state const& ws, nations::country_tag n);
	money_qnty_type daily_state_owner_building_cost(world_state const& ws, nations::state_tag si);
	money_qnty_type daily_national_building_cost(world_state const& ws, nations::country_tag n);

	struct range_information {
		float minimum;
		float maximum;
		float average;
	};

	range_information global_price_range(world_state const& ws, economy::goods_tag t);
	money_qnty_type get_life_needs_cost(world_state const& ws, nations::state_tag si, population::pop_type_tag ptype);
	money_qnty_type get_all_needs_cost(world_state const& ws, nations::state_tag si, population::pop_type_tag ptype);
	artisan_type_tag get_profitable_artisan(world_state const& ws, provinces::province_tag ps);

	void init_artisan_producation(world_state& ws);
	void set_initial_money(world_state& ws);
	void update_bankrupcy(world_state& ws);
	bool is_bankrupt(world_state const& ws, nations::country_tag n);
	void collect_taxes(world_state& ws);
	void pay_unemployment_pensions_salaries(world_state& ws, nations::country_tag n);
	void economy_update_tick(world_state& ws);
	void economy_demand_adjustment_tick(world_state& ws);
	void update_construction_and_projects(world_state& ws);

	economy::money_qnty_type project_player_tariff_income(world_state const& ws, float tariff_amount);

	money_qnty_type military_spending_amount(world_state const& ws, nations::country_tag n);
	money_qnty_type social_spending_amount(world_state const& ws, nations::country_tag n);
	money_qnty_type education_spending_amount(world_state const& ws, nations::country_tag n);
	money_qnty_type administrative_spending_amount(world_state const& ws, nations::country_tag n);
}
