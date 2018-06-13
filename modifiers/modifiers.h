#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "concurrency_tools\\concurrency_tools.hpp"

namespace scenario {
	class scenario_manager;
}

namespace modifiers {
	struct provincial_modifier {
		uint32_t icon = 0;

		text_data::text_tag name;
		national_modifier_tag complement;
		provincial_modifier_tag id;
	};

	struct national_modifier {
		uint32_t icon = 0;

		text_data::text_tag name;
		national_modifier_tag id;
	};
	
	struct factor_modifier {
		float factor = 0.0f;
		float base = 0.0f;
		uint16_t data_offset = 0ui16;
		uint16_t data_length = 0ui16;

		bool operator==(const factor_modifier& other) const {
			return (factor == other.factor) & (base == other.base) & (data_offset == other.data_offset) & (data_length == other.data_length);
		}
	};

	struct national_focus {
		float railroads = 0.0f;
		float loyalty = 0.0f;
		float pop_type_value = 0.0f;
		float flashpoint_tension = 0.0f;

		triggers::trigger_tag limit;
		text_data::text_tag name;
		provincial_modifier_tag modifier;
		text_data::text_tag group;

		population::pop_type_tag pop_type;
		ideologies::ideology_tag ideology;
		bool outliner_show_as_percent = false;
		bool has_goods = false;

		uint8_t icon;
		national_focus_tag id;
	};

	struct crime {
		text_data::text_tag name;
		triggers::trigger_tag crime_trigger;
		provincial_modifier_tag modifier;
		bool default_active = false;
	};

	struct factor_segment {
		float factor = 0.0f;
		triggers::trigger_tag condition;

		bool operator==(factor_segment other) const {
			return (factor == other.factor) & (condition == other.condition);
		}
	};

	namespace provincial_offsets {
		constexpr static uint32_t supply_limit = 0;
		constexpr static uint32_t attrition = 1;
		constexpr static uint32_t max_attrition = 2;
		constexpr static uint32_t local_ruling_party_support = 3;
		constexpr static uint32_t poor_life_needs = 4;
		constexpr static uint32_t rich_life_needs = 5;
		constexpr static uint32_t middle_life_needs = 6;
		constexpr static uint32_t poor_everyday_needs = 7;
		constexpr static uint32_t rich_everyday_needs = 8;
		constexpr static uint32_t middle_everyday_needs = 9;
		constexpr static uint32_t poor_luxury_needs = 10;
		constexpr static uint32_t middle_luxury_needs = 11;
		constexpr static uint32_t rich_luxury_needs = 12;
		constexpr static uint32_t population_growth = 13;
		constexpr static uint32_t local_factory_input = 14;
		constexpr static uint32_t local_factory_output = 15;
		constexpr static uint32_t local_factory_throughput = 16;
		constexpr static uint32_t local_rgo_input = 17;
		constexpr static uint32_t local_rgo_output = 18;
		constexpr static uint32_t local_rgo_throughput = 19;
		constexpr static uint32_t local_artisan_input = 20;
		constexpr static uint32_t local_artisan_output = 21;
		constexpr static uint32_t local_artisan_throughput = 22;
		constexpr static uint32_t number_of_voters = 23;
		constexpr static uint32_t goods_demand = 24;
		constexpr static uint32_t assimilation_rate = 25;
		constexpr static uint32_t life_rating = 26;
		constexpr static uint32_t farm_rgo_eff = 27;
		constexpr static uint32_t mine_rgo_eff = 28;
		constexpr static uint32_t farm_rgo_size = 29;
		constexpr static uint32_t mine_rgo_size = 30;
		constexpr static uint32_t pop_militancy_modifier = 31;
		constexpr static uint32_t pop_consciousness_modifier = 32;
		constexpr static uint32_t rich_income_modifier = 33;
		constexpr static uint32_t middle_income_modifier = 34;
		constexpr static uint32_t poor_income_modifier = 35;
		constexpr static uint32_t boost_strongest_party = 36;
		constexpr static uint32_t immigrant_attract = 37;
		constexpr static uint32_t immigrant_push = 38;
		constexpr static uint32_t local_repair = 39;
		constexpr static uint32_t local_ship_build = 40;
		constexpr static uint32_t movement_cost = 41;
		constexpr static uint32_t defense = 42;
		constexpr static uint32_t attack = 43;
		constexpr static uint32_t combat_width = 44;
		constexpr static uint32_t min_build_naval_base = 45;
		constexpr static uint32_t min_build_railroad = 46;
		constexpr static uint32_t min_build_fort = 47;

		constexpr static uint32_t count = 48;
		constexpr static uint32_t aligned_32_size = ((static_cast<uint32_t>(sizeof(value_type)) * count + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(value_type));
	}

	namespace national_offsets {
		constexpr static uint32_t war_exhaustion = 0;
		constexpr static uint32_t max_war_exhaustion = 1;
		constexpr static uint32_t leadership = 2;
		constexpr static uint32_t leadership_modifier = 3;
		constexpr static uint32_t supply_consumption = 4;
		constexpr static uint32_t org_regain = 5;
		constexpr static uint32_t reinforce_speed = 6;
		constexpr static uint32_t land_organisation = 7;
		constexpr static uint32_t naval_organisation = 8;
		constexpr static uint32_t research_points = 9;
		constexpr static uint32_t research_points_modifier = 10;
		constexpr static uint32_t research_points_on_conquer = 11;
		constexpr static uint32_t import_cost = 12;
		constexpr static uint32_t loan_interest = 13;
		constexpr static uint32_t tax_efficiency = 14;
		constexpr static uint32_t min_tax = 15;
		constexpr static uint32_t max_tax = 16;
		constexpr static uint32_t min_military_spending = 17;
		constexpr static uint32_t max_military_spending = 18;
		constexpr static uint32_t min_social_spending = 19;
		constexpr static uint32_t max_social_spending = 20;
		constexpr static uint32_t factory_owner_cost = 21;
		constexpr static uint32_t min_tariff = 22;
		constexpr static uint32_t max_tariff = 23;
		constexpr static uint32_t ruling_party_support = 24;
		constexpr static uint32_t rich_vote = 25;
		constexpr static uint32_t middle_vote = 26;
		constexpr static uint32_t poor_vote = 27;
		constexpr static uint32_t minimum_wage = 28;
		constexpr static uint32_t factory_maintenance = 29;
		constexpr static uint32_t poor_life_needs = 30;
		constexpr static uint32_t rich_life_needs = 31;
		constexpr static uint32_t middle_life_needs = 32;
		constexpr static uint32_t poor_everyday_needs = 33;
		constexpr static uint32_t rich_everyday_needs = 34;
		constexpr static uint32_t middle_everyday_needs = 35;
		constexpr static uint32_t poor_luxury_needs = 36;
		constexpr static uint32_t middle_luxury_needs = 37;
		constexpr static uint32_t rich_luxury_needs = 38;
		constexpr static uint32_t unemployment_benefit = 39;
		constexpr static uint32_t pension_level = 40;
		constexpr static uint32_t global_population_growth = 41;
		constexpr static uint32_t factory_input = 42;
		constexpr static uint32_t factory_output = 43;
		constexpr static uint32_t factory_throughput = 44;
		constexpr static uint32_t rgo_input = 45;
		constexpr static uint32_t rgo_output = 46;
		constexpr static uint32_t rgo_throughput = 47;
		constexpr static uint32_t artisan_input = 48;
		constexpr static uint32_t artisan_output = 49;
		constexpr static uint32_t artisan_throughput = 50;
		constexpr static uint32_t goods_demand = 51;
		constexpr static uint32_t badboy = 52;
		constexpr static uint32_t global_assimilation_rate = 53;
		constexpr static uint32_t prestige = 54;
		constexpr static uint32_t factory_cost = 55;
		constexpr static uint32_t farm_rgo_eff = 56;
		constexpr static uint32_t mine_rgo_eff = 57;
		constexpr static uint32_t farm_rgo_size = 58;
		constexpr static uint32_t mine_rgo_size = 59;
		constexpr static uint32_t issue_change_speed = 60;
		constexpr static uint32_t social_reform_desire = 61;
		constexpr static uint32_t political_reform_desire = 62;
		constexpr static uint32_t literacy_con_impact = 63;
		constexpr static uint32_t rich_income_modifier = 64;
		constexpr static uint32_t middle_income_modifier = 65;
		constexpr static uint32_t poor_income_modifier = 66;
		constexpr static uint32_t global_immigrant_attract = 67;
		constexpr static uint32_t poor_savings_modifier = 68;
		constexpr static uint32_t influence_modifier = 69;
		constexpr static uint32_t diplomatic_points_modifier = 70;
		constexpr static uint32_t mobilisation_size = 71;
		constexpr static uint32_t mobilisation_economy_impact = 72;
		constexpr static uint32_t global_pop_militancy_modifier = 73;
		constexpr static uint32_t global_pop_consciousness_modifier = 74;
		constexpr static uint32_t core_pop_militancy_modifier = 75;
		constexpr static uint32_t core_pop_consciousness_modifier = 76;
		constexpr static uint32_t non_accepted_pop_militancy_modifier = 77;
		constexpr static uint32_t non_accepted_pop_consciousness_modifier = 78;
		constexpr static uint32_t cb_generation_speed_modifier = 79;
		constexpr static uint32_t mobilization_impact = 80;
		constexpr static uint32_t suppression_points_modifier= 81;
		constexpr static uint32_t education_efficiency_modifier = 82;
		constexpr static uint32_t civilization_progress_modifier = 83;
		constexpr static uint32_t administrative_efficiency_modifier = 84;
		constexpr static uint32_t land_unit_start_experience = 85;
		constexpr static uint32_t naval_unit_start_experience = 86;
		constexpr static uint32_t naval_attack_modifier = 87;
		constexpr static uint32_t naval_defense_modifier = 88;
		constexpr static uint32_t land_attack_modifier = 89;
		constexpr static uint32_t land_defense_modifier = 90;
		constexpr static uint32_t tariff_efficiency_modifier = 91;
		constexpr static uint32_t max_loan_modifier = 92;
		constexpr static uint32_t unciv_economic_modifier = 93;
		constexpr static uint32_t unciv_military_modifier = 94;
		constexpr static uint32_t self_unciv_economic_modifier = 95;
		constexpr static uint32_t self_unciv_military_modifier = 96;
		constexpr static uint32_t commerce_tech_research_bonus = 97;
		constexpr static uint32_t army_tech_research_bonus = 98;
		constexpr static uint32_t industry_tech_research_bonus = 99;
		constexpr static uint32_t navy_tech_research_bonus = 100;
		constexpr static uint32_t culture_tech_research_bonus = 101;

		constexpr static uint32_t count = 102;
		constexpr static uint32_t aligned_32_size = ((static_cast<uint32_t>(sizeof(value_type)) * count + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(value_type));
	}

	constexpr uint16_t bad_offset = 1024;

	inline constexpr uint32_t pack_offset_pair(uint16_t prov, uint16_t national) {
		return ((uint32_t)prov << 16) | (uint32_t)national;
	}
	inline constexpr uint32_t get_national_offset_from_packed(uint32_t v) {
		return v & 0xFFFF;
	}
	inline constexpr uint32_t get_provincial_offset_from_packed(uint32_t v) {
		return (v >> 16) & 0xFFFF;
	}

	std::pair<uint32_t, uint32_t> get_provincial_and_national_offsets_from_token(const char* s, const char* e);

	struct defines {
		date_tag start_date = date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1));
		date_tag end_date = date_to_tag(boost::gregorian::date(1935, boost::gregorian::Dec, 31));
		float years_of_nationalism = 20.0f;
		float base_country_tax_efficiency = 0.2f;
		float base_country_admin_efficiency = 0.2f;
		float gold_to_cash_rate = 0.5f;
		float gold_to_worker_pay_rate = 3.5f;
		float great_nations_count = 8.0f;
		float greatness_days = 365.0f;
		float badboy_limit = 25.0f;
		float max_bureaucracy_percentage = 0.01f;
		float bureaucracy_percentage_increment = 0.001f;
		float min_crimefight_percent = 0.2f;
		float max_crimefight_percent = 0.99f;
		float admin_efficiency_crimefight_percent = 0.5f;
		float conservative_increase_after_reform = 0.25f;
		float campaign_event_base_time = 80.0f;
		float campaign_event_min_time = 21.0f;
		float campaign_event_state_scale = -3.0f;
		float campaign_duration = 6.0f;
		float colonial_rank = 16.0f;
		float colony_to_state_prestige_gain = 10.0f;
		float colonial_liferating = 35.0f;
		float base_greatpower_daily_influence = 0.25f;
		float ai_support_reform = 0.05f;
		float base_monthly_diplopoints = 0.3f;
		float diplomat_travel_time = 14.0f;
		float province_overseas_penalty = 0.005f;
		float noncore_tax_penalty = -0.05f;
		float base_tariff_efficiency = 0.2f;
		float colony_formed_prestige = 1.0f;
		float created_cb_valid_time = 12.0f;
		float loyalty_boost_on_party_win = 0.1f;
		float movement_radicalism_base = 25.0f;
		float movement_radicalism_passed_reform_effect = -2.0f;
		float movement_radicalism_nationalism_factor = 1.0f;
		float suppression_points_gain_base = 170.0f;
		float suppress_bureaucrat_factor = 0.5f;
		float wrong_reform_militancy_impact = 1.0f;
		float suppression_radicalisation_hit = 10.0f;
		float investment_score_factor = 0.005f;
		float unciv_tech_spread_max = 0.60f;
		float unciv_tech_spread_min = 0.15f;
		float min_delay_between_reforms = 6.0f;
		float economic_reform_uh_factor = -0.40f;
		float military_reform_uh_factor = -0.40f;
		float wrong_reform_radical_impact = 15.0f;
		float tech_year_span = 140.0f;
		float tech_factor_vassal = 0.5f;
		float max_suppression = 100.0f;
		float prestige_hit_on_break_country = -0.20f;
		float min_mobilize_limit = 3.0f;
		float pop_growth_country_cache_days = 30.0f;
		float newspaper_printing_frequency = 350.0f;
		float newspaper_timeout_period = 890.0f;
		float newspaper_max_tension = 750.0f;
		float naval_base_supply_score_base = 10.0f;
		float naval_base_supply_score_empty = 2.0f;
		float naval_base_non_core_supply_score = 0.3f;
		float colonial_points_from_supply_factor = 1.0f;
		float colonial_points_for_non_core_base = 1.0f;
		float mobilization_speed_base = 0.12f;
		float mobilization_speed_rails_mult = 3.0f;
		float colonization_interest_lead = 3.0f;
		float colonization_influence_lead = 3.0f;
		float colonization_months_to_colonize = 6.0f;
		float colonization_days_between_investment = 90.0f;
		float colonization_days_for_initial_investment = 270.0f;
		float colonization_protectorate_province_maintainance = 4.0f;
		float colonization_colony_province_maintainance = 5.0f;
		float colonization_colony_industry_maintainance = 1.0f;
		float colonization_colony_railway_maintainance = 0.1f;
		float colonization_interest_cost_initial = 100.0f;
		float colonization_interest_cost_neighbor_modifier = -20.0f;
		float colonization_interest_cost = 20.0f;
		float colonization_influence_cost = 20.0f;
		float colonization_extra_guard_cost = 10.0f;
		float colonization_release_dominion_cost = 30.0f;
		float colonization_create_state_cost = 300.0f;
		float colonization_create_protectorate_cost = 5.0f;
		float colonization_create_colony_cost = 4.0f;
		float colonization_colony_state_distance = 400.0f;
		float colonization_influence_temperature_per_day = 0.08f;
		float colonization_influence_temperature_per_level = 0.005f;
		float party_loyalty_hit_on_war_loss = 0.95f;
		float research_points_on_conquer_mult = 360.0f;
		float max_research_points = 25000.0f;
		float max_daily_research = 100.0f;
		float loan_base_interest = 0.02f;
		float bankruptcy_external_loan_years = 10.0f;
		float bankruptcy_factor = 0.2f;
		float shadowy_financiers_max_loan_amount = 1500.0f;
		float max_loan_cap_from_banks = 3.0f;
		float gunboat_low_tax_cap = 0.0f;
		float gunboat_high_tax_cap = 1.0f;
		float gunboat_fleet_size_factor = 100.0f;
		float province_size_divider = 50.0f;
		float capitalist_build_factory_state_employment_percent = 0.7f;
		float goods_focus_swap_chance = 0.0f;
		float num_closed_factories_per_state_lassiez_faire = 1.0f;
		float min_num_factories_per_state_before_deleting_lassiez_faire = 2.0f;
		float bankrupcy_duration = 2.0f;
		float second_rank_base_share_factor = 0.5f;
		float civ_base_share_factor = 0.75f;
		float unciv_base_share_factor = 1.0f;
		float factory_paychecks_leftover_factor = 0.25f;
		float max_factory_money_save = 1000.0f;
		float small_debt_limit = 10000.0f;
		float factory_upgrade_employee_factor = 0.8f;
		float rgo_supply_demand_factor_hire_hi = 0.2f;
		float rgo_supply_demand_factor_hire_lo = 0.02f;
		float rgo_supply_demand_factor_fire = 0.4f;
		float employment_hire_lowest = 0.001f;
		float employment_fire_lowest = 0.001f;
		float trade_cap_low_limit_land = 0.0f;
		float trade_cap_low_limit_naval = 0.3f;
		float trade_cap_low_limit_constructions = 0.0f;
		float factory_purchase_min_factor = 0.75f;
		float factory_purchase_drawdown_factor = 0.025f;
		float dig_in_increase_each_days = 5.0f;
		float reinforce_speed = 0.2f;
		float combat_difficulty_impact = 0.2f;
		float base_combat_width = 30.0f;
		float pop_min_size_for_regiment = 1000.0f;
		float pop_size_per_regiment = 3000.0f;
		float soldier_to_pop_damage = 0.2f;
		float land_speed_modifier = 1.0f;
		float naval_speed_modifier = 2.0f;
		float exp_gain_div = 0.05f;
		float leader_recruit_cost = 20.0f;
		float supply_range = 250.0f;
		float pop_min_size_for_regiment_protectorate_multiplier = 8.0f;
		float pop_min_size_for_regiment_colony_multiplier = 5.0f;
		float pop_min_size_for_regiment_noncore_multiplier = 3.0f;
		float gas_attack_modifier = 3.0f;
		float combatloss_war_exhaustion = 3.0f;
		float leader_max_random_prestige = 0.05f;
		float leader_age_death_factor = 4.0f;
		float leader_prestige_to_morale_factor = 0.8f;
		float leader_prestige_to_max_org_factor = 0.7f;
		float leader_transfer_penalty_on_country_prestige = 0.05f;
		float leader_prestige_land_gain = 0.3f;
		float leader_prestige_naval_gain = 0.3f;
		float naval_combat_seeking_chance = 0.5f;
		float naval_combat_seeking_chance_min = 0.1f;
		float naval_combat_self_defence_chance = 2.0f;
		float naval_combat_shift_back_on_next_target = 0.2f;
		float naval_combat_shift_back_duration_scale = 7.0f;
		float naval_combat_speed_to_distance_factor = 0.05f;
		float naval_combat_change_target_chance = 0.03f;
		float naval_combat_damage_org_mult = 0.4f;
		float naval_combat_damage_str_mult = 0.2f;
		float naval_combat_damage_mult_no_org = 2.0f;
		float naval_combat_retreat_chance = 0.07f;
		float naval_combat_retreat_str_org_level = 0.15f;
		float naval_combat_retreat_speed_mod = 0.4f;
		float naval_combat_retreat_min_distance = 0.25f;
		float naval_combat_damaged_target_selection = 2.5f;
		float naval_combat_stacking_target_change = 0.03f;
		float naval_combat_stacking_target_select = 0.2f;
		float naval_combat_max_targets = 6.0f;
		float ai_bigship_proportion = 0.4f;
		float ai_lightship_proportion = 0.4f;
		float ai_transport_proportion = 0.2f;
		float ai_cavalry_proportion = 0.15f;
		float ai_support_proportion = 0.3f;
		float ai_special_proportion = 0.15f;
		float ai_escort_ratio = 2.0f;
		float ai_army_taxbase_fraction = 0.3f;
		float ai_navy_taxbase_fraction = 0.3f;
		float ai_blockade_range = 200.0f;
		float recon_unit_ratio = 0.1f;
		float engineer_unit_ratio = 0.1f;
		float siege_brigades_min = 3.0f;
		float siege_brigades_max = 13.0f;
		float siege_brigades_bonus = 0.5f;
		float recon_siege_effect = 0.5f;
		float siege_attrition = 2.0f;
		float base_military_tactics = 1.5f;
		float naval_low_supply_damage_supply_status = 0.25f;
		float naval_low_supply_damage_days_delay = 30.0f;
		float naval_low_supply_damage_min_str = 5.0f;
		float naval_low_supply_damage_per_day = 0.25f;
		float war_prestige_cost_base = 100.0f;
		float war_prestige_cost_high_prestige = 1.0f;
		float war_prestige_cost_neg_prestige = 1.0f;
		float war_prestige_cost_truce = 100.0f;
		float war_prestige_cost_honor_alliance = -100.0f;
		float war_prestige_cost_honor_guarnatee = -50.0f;
		float war_prestige_cost_uncivilized = -50.0f;
		float war_prestige_cost_core = -50.0f;
		float war_failed_goal_militancy = 2.0f;
		float war_failed_goal_prestige_base = -10.0f;
		float war_failed_goal_prestige = -0.1f;
		float discredit_days = 180.0f;
		float discredit_influence_cost_factor = 2.0f;
		float discredit_influence_gain_factor = -0.75f;
		float banembassy_days = 365.0f;
		float declarewar_relation_on_accept = -50.0f;
		float declarewar_diplomatic_cost = 1.0f;
		float addwargoal_relation_on_accept = 0.0f;
		float addwargoal_diplomatic_cost = 1.0f;
		float add_unjustified_goal_badboy = 1.0f;
		float peace_relation_on_accept = 5.0f;
		float peace_relation_on_decline = -10.0f;
		float peace_diplomatic_cost = 1.0f;
		float alliance_relation_on_accept = 100.0f;
		float alliance_relation_on_decline = -50.0f;
		float alliance_diplomatic_cost = 1.0f;
		float cancelalliance_relation_on_accept = -20.0f;
		float cancelalliance_diplomatic_cost = 1.0f;
		float callally_relation_on_accept = 20.0f;
		float callally_relation_on_decline = -20.0f;
		float callally_diplomatic_cost = 0.0f;
		float askmilaccess_relation_on_accept = 30.0f;
		float askmilaccess_relation_on_decline = -10.0f;
		float askmilaccess_diplomatic_cost = 1.0f;
		float cancelaskmilaccess_relation_on_accept = 0.0f;
		float cancelaskmilaccess_diplomatic_cost = 1.0f;
		float givemilaccess_relation_on_accept = 10.0f;
		float givemilaccess_relation_on_decline = 0.0f;
		float givemilaccess_diplomatic_cost = 1.0f;
		float cancelgivemilaccess_relation_on_accept = -10.0f;
		float cancelgivemilaccess_diplomatic_cost = 1.0f;
		float warsubsidy_relation_on_accept = 20.0f;
		float warsubsidy_diplomatic_cost = 1.0f;
		float cancelwarsubsidy_relation_on_accept = 0.0f;
		float cancelwarsubsidy_diplomatic_cost = 0.0f;
		float discredit_relation_on_accept = -5.0f;
		float discredit_influence_cost = 25.0f;
		float expeladvisors_relation_on_accept = -5.0f;
		float expeladvisors_influence_cost = 50.0f;
		float ceasecolonization_relation_on_accept = 20.0f;
		float ceasecolonization_relation_on_decline = -20.0f;
		float ceasecolonization_diplomatic_cost = 1.0f;
		float banembassy_relation_on_accept = -10.0f;
		float banembassy_influence_cost = 65.0f;
		float increaserelation_relation_on_accept = 15.0f;
		float increaserelation_relation_on_decline = 0.0f;
		float increaserelation_diplomatic_cost = 2.0f;
		float decreaserelation_relation_on_accept = -25.0f;
		float decreaserelation_diplomatic_cost = 1.0f;
		float addtosphere_relation_on_accept = 0.0f;
		float addtosphere_influence_cost = 100.0f;
		float removefromsphere_relation_on_accept = -10.0f;
		float removefromsphere_influence_cost = 100.0f;
		float removefromsphere_prestige_cost = 10.0f;
		float removefromsphere_infamy_cost = 1.0f;
		float increaseopinion_relation_on_accept = 20.0f;
		float increaseopinion_influence_cost = 50.0f;
		float decreaseopinion_relation_on_accept = -10.0f;
		float decreaseopinion_influence_cost = 50.0f;
		float make_cb_diplomatic_cost = 1.0f;
		float make_cb_relation_on_accept = 0.0f;
		float disarmament_army_hit = 0.5f;
		float reparations_tax_hit = 0.25f;
		float prestige_reduction_base = 25.0f;
		float prestige_reduction = 0.1f;
		float reparations_years = 5.0f;
		float min_warscore_to_intervene = -1.0f;
		float min_months_to_intervene = 0.0f;
		float max_warscore_from_battles = 50.0f;
		float gunboat_diplomatic_cost = 1.0f;
		float gunboat_relation_on_accept = 1.0f;
		float wargoal_jingoism_requirement = 0.07f;
		float liberate_state_relation_increase = 50.0f;
		float dishonored_callally_prestige_penalty = -3.0f;
		float base_truce_months = 60.0f;
		float max_influence = 100.0f;
		float warsubsidies_percent = 0.20f;
		float neighbour_bonus_influence_percent = 0.50f;
		float sphere_neighbour_bonus_influence_percent = 0.2f;
		float other_continent_bonus_influence_percent = -0.50f;
		float puppet_bonus_influence_percent = 1.0f;
		float release_nation_prestige = 0.0f;
		float release_nation_infamy = -5.0f;
		float good_peace_refusal_militancy = 1.0f;
		float good_peace_refusal_warexh = 5.0f;
		float cb_generation_base_speed = 0.5f;
		float cb_generation_speed_bonus_on_colony_competition = 1.0f;
		float cb_generation_speed_bonus_on_colony_competition_troops_presence = 1.0f;
		float make_cb_relation_limit = 100.0f;
		float cb_detection_chance_base = 15.0f;
		float investment_influence_defense = 0.5f;
		float relation_influence_modifier = 1000.0f;
		float on_cb_detected_relation_change = -50.0f;
		float gw_intervene_min_relations = 100.0f;
		float gw_intervene_max_exhaustion = 1.0f;
		float gw_justify_cb_badboy_impact = 0.33f;
		float gw_cb_construction_speed = 0.25f;
		float gw_wargoal_jingoism_requirement_mod = 0.25f;
		float gw_warscore_cost_mod = 0.65f;
		float gw_warscore_cost_mod_2 = 0.3f;
		float gw_warscore_2_threshold = 50.0f;
		float tension_decay = -0.08f;
		float tension_from_cb = 0.04f;
		float tension_from_movement = 0.02f;
		float tension_from_movement_max = 1.2f;
		float at_war_tension_decay = -0.12f;
		float tension_on_cb_discovered = 20.0f;
		float tension_on_revolt = 50.0f;
		float tension_while_crisis = -1.0f;
		float crisis_cooldown_months = 60.0f;
		float crisis_base_chance = 20.0f;
		float crisis_temperature_increase = 0.05f;
		float crisis_offer_diplomatic_cost = 0.0f;
		float crisis_offer_relation_on_accept = 0.0f;
		float crisis_offer_relation_on_decline = 0.0f;
		float crisis_did_not_take_side_prestige_factor_base = -0.1f;
		float crisis_did_not_take_side_prestige_factor_year = -0.002f;
		float crisis_winner_prestige_factor_base = 10.0f;
		float crisis_winner_prestige_factor_year = 0.4f;
		float crisis_winner_relations_impact = 25.0f;
		float back_crisis_diplomatic_cost = 0.0f;
		float back_crisis_relation_on_accept = 0.0f;
		float back_crisis_relation_on_decline = 0.0f;
		float crisis_temperature_on_offer_decline = 0.0f;
		float crisis_temperature_participant_factor = 10.0f;
		float crisis_temperature_on_mobilize = 10.0f;
		float crisis_wargoal_infamy_mult = 1.0f;
		float crisis_wargoal_prestige_mult = 1.0f;
		float crisis_wargoal_militancy_mult = 0.0f;
		float crisis_interest_war_exhaustion_limit = 20.0f;
		float rank_1_tension_decay = -0.5f;
		float rank_2_tension_decay = -0.4f;
		float rank_3_tension_decay = -0.3f;
		float rank_4_tension_decay = -0.2f;
		float rank_5_tension_decay = -0.08f;
		float rank_6_tension_decay = -0.06f;
		float rank_7_tension_decay = -0.04f;
		float rank_8_tension_decay = -0.02f;
		float tws_fulfilled_speed = 0.1f;
		float tws_not_fulfilled_speed = 0.1f;
		float tws_grace_period_days = 730.0f;
		float tws_cb_limit_default = 100.0f;
		float tws_fulfilled_idle_space = 0.75f;
		float tws_battle_min_count = 5.0f;
		float tws_battle_max_aspect = 8.0f;
		float large_population_influence_penalty = -0.4f;
		float lone_backer_prestige_factor = 0.05f;
		float base_clergy_for_literacy = 0.005f;
		float max_clergy_for_literacy = 0.04f;
		float literacy_change_speed = 0.1f;
		float assimilation_scale = 0.004f;
		float conversion_scale = 0.01f;
		float immigration_scale = 0.004f;
		float promotion_scale = 0.002f;
		float promotion_assimilation_chance = 1.0f;
		float luxury_threshold = 500.0f;
		float base_goods_demand = 0.8f;
		float base_popgrowth = 0.0001f;
		float min_life_rating_for_growth = 30.0f;
		float life_rating_growth_bonus = 0.0001f;
		float life_need_starvation_limit = 0.5f;
		float mil_lack_everyday_need = 0.1f;
		float mil_has_everyday_need = -0.1f;
		float mil_has_luxury_need = -0.2f;
		float mil_no_life_need = 0.2f;
		float mil_require_reform = 0.2f;
		float mil_ideology = -0.1f;
		float mil_ruling_party = -0.1f;
		float mil_reform_impact = 2.0f;
		float mil_war_exhaustion = 0.005f;
		float mil_non_accepted = 0.05f;
		float con_literacy = 0.1f;
		float con_luxury_goods = 0.1f;
		float con_poor_clergy = -2.5f;
		float con_midrich_clergy = -1.25f;
		float con_reform_impact = -50.0f;
		float con_colonial_factor = 0.5f;
		float ruling_party_happy_change = -1.0f;
		float ruling_party_angry_change = 2.0f;
		float pdef_base_con = 20.0f;
		float national_focus_divider = 400000.0f;
		float pop_savings = 0.018f;
		float state_creation_admin_limit = 0.01f;
		float mil_to_join_rebel = 7.0f;
		float mil_to_join_rising = 8.0f;
		float mil_to_autorise = 9.0f;
		float reduction_after_riseing = 0.0f;
		float reduction_after_defeat = 3.0f;
		float pop_to_leadership = 0.0001f;
		float artisan_min_productivity = 1.0f;
		float slave_growth_divisor = 10.0f;
		float mil_hit_from_conquest = 4.0f;
		float luxury_con_change = 0.001f;
		float invention_impact_on_demand = 0.005f;
		float artisan_suppressed_colonial_goods_category = 0.0f;
		float issue_movement_join_limit = 8.0f;
		float issue_movement_leave_limit = 7.0f;
		float movement_con_factor = 0.05f;
		float movement_lit_factor = 0.3f;
		float mil_on_reb_move = 8.0f;
		float population_suppression_factor = 0.0f;
		float population_movement_radical_factor = 300.0f;
		float nationalist_movement_mil_cap = 3.0f;
		float movement_support_uh_factor = 3.0f;
		float rebel_occupation_strength_bonus = 0.01f;
		float large_population_limit = 9000000.0f;
		float large_population_influence_penalty_chunk = 250000.0f;
		float colony_weight = 5.0f;
		float administrator_weight = 10.0f;
		float industryworker_weight = 10.0f;
		float educator_weight = 10.0f;
		float soldier_weight = 30.0f;
		float soldier_fraction = 0.03f;
		float capitalist_fraction = 0.001f;
		float production_weight = 0.05f;
		float spam_penalty = 10.0f;
		float one_side_max_warscore = 150.0f;
		float pop_project_investment_max_budget_factor = 0.25f;
		float relation_limit_no_alliance_offer = 0.0f;
		float naval_supply_penalty_limit = 0.2f;
		float chance_build_railroad = 0.2f;
		float chance_build_naval_base = 0.75f;
		float chance_build_fort = 0.5f;
		float chance_invest_pop_proj = 0.16f;
		float chance_foreign_invest = 0.16f;
		float tws_awareness_score_low_cap = 0.2f;
		float tws_awareness_score_aspect = 0.5f;
		float peace_base_reluctance = 20.0f;
		float peace_time_months = 30.0f;
		float peace_time_factor = 0.6f;
		float peace_time_factor_no_goals = 2.0f;
		float peace_war_exhaustion_factor = 0.5f;
		float peace_war_direction_factor = 1.0f;
		float peace_war_direction_winning_mult = 5.0f;
		float peace_force_balance_factor = 0.3f;
		float peace_ally_base_reluctance_mult = 2.0f;
		float peace_ally_time_mult = 0.0f;
		float peace_ally_war_exhaustion_mult = 1.0f;
		float peace_ally_war_direction_mult = 0.0f;
		float peace_ally_force_balance_mult = 0.0f;
		float aggression_base = 5.0f;
		float aggression_unciv_bonus = 10.0f;
		float fleet_size = 30.0f;
		float min_fleets = 1.0f;
		float max_fleets = 10.0f;
		float months_before_disband = 6.0f;
	};

	class modifiers_manager {
	public:
		tagged_vector<national_modifier, national_modifier_tag> national_modifiers;
		tagged_vector<provincial_modifier, provincial_modifier_tag> provincial_modifiers;
		tagged_vector<factor_modifier, factor_tag> factor_modifiers;
		tagged_vector<national_focus, national_focus_tag> national_focuses;

		std::vector<factor_segment> factor_data;

		std::vector<std::pair<national_modifier_tag, triggers::trigger_tag>> triggered_modifiers;
		boost::container::flat_map<provincial_modifier_tag, crime> crimes;

		boost::container::flat_map<text_data::text_tag, national_modifier_tag> named_national_modifiers_index;
		boost::container::flat_map<text_data::text_tag, provincial_modifier_tag> named_provincial_modifiers_index;

		tagged_fixed_blocked_2dvector<value_type, national_modifier_tag, uint32_t, aligned_allocator_32<value_type>> national_modifier_definitions;
		tagged_fixed_blocked_2dvector<value_type, provincial_modifier_tag, uint32_t, aligned_allocator_32<value_type>> provincial_modifier_definitions;

		tagged_fixed_2dvector<float, national_focus_tag, economy::goods_tag> national_focus_goods_weights;
		int32_t national_focus_group_count = 0;

		struct {
			provincial_modifier_tag overseas;
			provincial_modifier_tag coastal;
			provincial_modifier_tag non_coastal;
			provincial_modifier_tag coastal_sea;
			provincial_modifier_tag sea_zone;
			provincial_modifier_tag land_province;
			provincial_modifier_tag blockaded;
			provincial_modifier_tag no_adjacent_controlled;
			provincial_modifier_tag core;
			provincial_modifier_tag has_siege;
			provincial_modifier_tag occupied;
			provincial_modifier_tag nationalism;
			provincial_modifier_tag infrastructure;

			national_modifier_tag war;
			national_modifier_tag peace;
			national_modifier_tag disarming;
			national_modifier_tag war_exhaustion;
			national_modifier_tag badboy;
			national_modifier_tag debt_default_to;
			national_modifier_tag bad_debter;
			national_modifier_tag great_power;
			national_modifier_tag second_power;
			national_modifier_tag civ_nation;
			national_modifier_tag unciv_nation;
			national_modifier_tag average_literacy;
			national_modifier_tag plurality;
			national_modifier_tag generalised_debt_default;
			national_modifier_tag total_occupation;
			national_modifier_tag total_blockaded;
			national_modifier_tag in_bankrupcy;
		} static_modifiers;

		defines global_defines;

		national_modifier_tag fetch_unique_national_modifier(text_data::text_tag n); // adds index only if valid text tag
		provincial_modifier_tag fetch_unique_provincial_modifier(text_data::text_tag n); // adds index only if valid text tag

		modifiers_manager() {
			national_modifier_definitions.reset(national_offsets::count);
			provincial_modifier_definitions.reset(provincial_offsets::count);
		}
	};

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, modifiers_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	class modifier_reading_base {
	public:
		std::vector<value_type, aligned_allocator_32<float>> modifier_data;
		uint32_t icon = 0;
		uint32_t count_unique_provincial = 0;
		uint32_t count_unique_national = 0;
		uint32_t total_attributes = 0;

		modifier_reading_base() : modifier_data(provincial_offsets::aligned_32_size + national_offsets::aligned_32_size) {}
		void add_attribute(const std::pair<token_and_type, float>& p);
		void add_attribute(uint32_t provincial_offset, uint32_t national_offset, float v);
		void remove_shared_national_attributes();
	};

	void set_provincial_modifier(provincial_modifier_tag tag, modifier_reading_base& mod, modifiers_manager& manager);
	void set_national_modifier(national_modifier_tag tag, const modifier_reading_base& mod, modifiers_manager& manager);

	provincial_modifier_tag add_provincial_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager);
	national_modifier_tag add_national_modifier(text_data::text_tag name, const modifier_reading_base& mod, modifiers_manager& manager);
	std::pair<provincial_modifier_tag, national_modifier_tag> add_indeterminate_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager);
	std::pair<uint16_t, bool> commit_factor(modifiers_manager& m, const std::vector<factor_segment>& factor);  // returns [offset into data, was newly added]
	
	provincial_modifier_tag parse_provincial_modifier(text_data::text_tag name, modifiers_manager& manager, const token_group* s, const token_group* e);
	national_modifier_tag parse_national_modifier(text_data::text_tag name, modifiers_manager& manager, const token_group* s, const token_group* e);
	std::pair<provincial_modifier_tag, national_modifier_tag> parse_indeterminate_modifier(
		text_data::text_tag name,
		modifiers_manager& manager,
		const token_group* s,
		const token_group* e);

	void read_defines(modifiers_manager& m, const directory& source_directory);
	void pre_parse_crimes(
		parsing_state& state,
		const directory& source_directory);
	void pre_parse_triggered_modifiers(parsing_state& state, const directory& source_directory);
	void parse_national_values(
		parsing_state& state,
		const directory& source_directory);
	void read_static_modifiers(
		parsing_state& state,
		const directory& source_directory);
	void read_event_modifiers(
		parsing_state& state,
		const directory& source_directory);

	void read_crimes(parsing_state& state, scenario::scenario_manager& s);
	void read_triggered_modifiers(parsing_state& state, scenario::scenario_manager& s);

	void read_national_focuses(scenario::scenario_manager& s, const directory& source_directory);

	factor_tag parse_modifier_factors(
		scenario::scenario_manager& s,
		triggers::trigger_scope_state modifier_scope,
		float default_factor,
		float default_base,
		const token_group* start,
		const token_group* end);
}
