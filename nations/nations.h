#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "economy\\economy.h"
#include "issues\\issues.h"
#include "technologies\\technologies.h"
#include "modifiers\\modifiers.h"
#include "concurrency_tools\\variable_layout.h"

namespace modifiers {
	struct national_focus;
}

namespace nations {
	struct pop_project {
		economy::money_qnty_type funds = economy::money_qnty_type(0);
		provinces::province_tag location; // for railroad
		economy::factory_type_tag factory_type;
		economy::pop_project_type type;
	};
	struct region_state_pair {
		provinces::state_tag region_id;
		state_tag state;

		bool operator<(region_state_pair other)  const noexcept { return region_id < other.region_id; }
		bool operator==(region_state_pair other) const noexcept { return region_id == other.region_id; }
	};
	struct influence {
		float investment_amount = 0.0f;
		float amount = 0.0f; // up to 100

		country_tag target;
		uint8_t packed_data = 2ui8;
		
		int32_t level() const { return int32_t(packed_data & 0x07); } // 0 to 5 (5 = in sphere, 2 = neutral)
		int32_t priority() const { return int32_t((packed_data & 0x30) >> 4); } // 0 to 3
		bool is_banned() const { return (packed_data & 0x08) != 0; }

		void level(int32_t v) { packed_data = uint8_t((packed_data & ~(0x07)) | (v & 0x07)); }
		void priority(int32_t v) { packed_data = uint8_t((packed_data & ~(0x30)) | ((v << 4) & 0x30)); }
		void is_banned(bool v) { packed_data = uint8_t((packed_data & ~(0x08)) | (int32_t(v) << 3)); }

		influence() noexcept : investment_amount(0.0f), target(), amount(0), packed_data(2i8) {}
		influence(nations::country_tag n) noexcept : investment_amount(0.0f), target(n), amount(0), packed_data(2i8) {}
		influence(float a, nations::country_tag n, float b, int8_t c, int8_t d) noexcept : investment_amount(a), target(n), amount(b), packed_data(uint8_t(c | (d << 4))) {}

		bool operator<(influence const& other)  const noexcept { return target < other.target; }
		bool operator==(influence const& other) const noexcept { return target == other.target; }
	};
	struct relationship {
		country_tag tag;
		int16_t value = 0;

		bool operator<(relationship const& other)  const noexcept { return tag < other.tag; }
		bool operator==(relationship const& other) const noexcept { return tag == other.tag; }
	};
	struct truce {
		date_tag until;
		country_tag tag;

		bool operator<(truce const& other)  const noexcept { return tag < other.tag; }
		bool operator==(truce const& other) const noexcept { return tag == other.tag; }
	};
	struct timed_national_modifier {
		date_tag expiration;
		modifiers::national_modifier_tag mod;

		bool operator<(timed_national_modifier const& other)  const noexcept { return mod < other.mod; }
		bool operator==(timed_national_modifier const& other) const noexcept { return mod == other.mod && expiration == other.expiration; }
	};
}

namespace nation {
	struct f_rich_tax;
	struct f_middle_tax;
	struct f_poor_tax;
	struct f_social_spending;
	struct f_administrative_spending;
	struct f_education_spending;
	struct f_military_spending;
	struct f_tariffs;
	//struct debt_setting;
	struct f_army_stockpile_spending;
	struct f_navy_stockpile_spending;
	struct f_projects_stockpile_spending;

	struct ruling_party;
	struct current_capital;
	struct tag;
	struct primary_culture;
	struct national_religion;
	struct current_government;
	struct current_rules;
	struct ruling_ideology;
	struct national_value;
	struct tech_school;
	struct enabled_crimes;

	struct current_research;

	struct dominant_culture;
	struct dominant_issue;
	struct dominant_ideology;
	struct dominant_religion;

	struct cb_construction_progress;
	struct cb_construction_target;
	struct cb_construction_type;

	struct flag;
	struct current_color;
	struct name;
	struct adjective;

	struct military_score;
	struct industrial_score;
	struct overall_rank;
	struct prestige_rank;
	struct military_rank;
	struct industrial_rank;

	struct province_count;
	struct central_province_count; // connected to the capital
	struct rebel_controlled_provinces; // connected to the capital only
	struct blockaded_count; // connected to the capital only
	struct crime_count; // connected to the capital only

	struct leadership_points;
	struct base_colonial_points; // add to tech for actual
	struct num_connected_ports; // number of ports connected to capital by land
	struct num_ports;

	struct total_foreign_investment;
	struct treasury;
	struct plurality;
	struct revanchism;
	struct base_prestige;
	struct infamy;
	struct war_exhaustion;
	struct suppression_points;
	struct diplomacy_points;
	struct research_points;
	struct national_debt;
	struct tax_base;
	struct political_interest_fraction;
	struct social_interest_fraction;
	struct national_administrative_efficiency;
	struct social_movement_support; // sum of social movement supporters / total pop * defines factor
	struct political_movement_support; // sum of social movement supporters / total pop * defines factor
	// mobilization_impact // == 1.0 - std::max(0.0f, this_nation.modifier_values[national_offsets::mobilisation_size]) * this_nation.modifier_values[national_offsets::mobilisation_economy_impact];

	struct last_election;
	struct last_manual_ruling_party_change;
	struct last_reform_date;
	struct last_lost_war;
	struct disarmed_until;

	struct sphere_leader;
	struct overlord;

	struct owned_provinces;
	struct controlled_provinces;
	struct naval_patrols; //sea provinces fleets will try to control
	struct sphere_members;
	struct vassals;
	struct allies;
	struct neighboring_nations;
	struct accepted_cultures;
	struct member_states;
	struct gp_influence;
	struct influencers;  // nations nationally investing in or influencing this country
	struct relations;
	struct truces;
	struct national_focus_locations;
	struct national_flags;
	struct static_modifiers;
	struct timed_modifiers;
	struct statewise_tariff_mask;
	struct generals;
	struct admirals;
	struct armies;
	struct fleets;
	struct active_orders;
	struct strategic_hqs;
	struct active_cbs;
	struct wars_involved_in;
	struct opponents_in_war;
	struct allies_in_war;

	struct is_civilized;
	struct is_substate;
	struct is_mobilized;
	struct is_not_ai_controlled;
	struct is_holding_election;
	struct is_colonial_nation;
	struct is_at_war;
	struct cb_construction_discovered;
	struct has_gas_attack;
	struct has_gas_defence;

	struct total_core_population;
	struct player_importance;

	struct demographics;
	struct colonial_demographics;

	constexpr int32_t container_size = 400;

	using container = variable_layout_tagged_vector < nations::country_tag, container_size,

		sphere_leader, nations::country_tag,
		overlord, nations::country_tag,

		enabled_crimes, uint64_t,
		current_rules, int32_t,
		total_core_population, float,
		last_election, date_tag,
		last_reform_date, date_tag,
		last_manual_ruling_party_change, date_tag,
		last_lost_war, date_tag,
		disarmed_until, date_tag,

		total_foreign_investment, float,
		treasury, float,
		plurality, float,
		revanchism, float,
		base_prestige, float,
		infamy, float,
		war_exhaustion, float,
		suppression_points, float,
		diplomacy_points, float,
		research_points, float,
		national_debt, float,
		tax_base, float,
		political_interest_fraction, float,
		social_interest_fraction, float,
		national_administrative_efficiency, float,
		social_movement_support, float,
		political_movement_support, float,

		owned_provinces, set_tag<provinces::province_tag>,
		controlled_provinces, set_tag<provinces::province_tag>,
		naval_patrols, set_tag<provinces::province_tag>,
		sphere_members, set_tag<nations::country_tag>,
		vassals, set_tag<nations::country_tag>,
		allies, set_tag<nations::country_tag>,
		neighboring_nations, set_tag<nations::country_tag>,
		accepted_cultures, set_tag<cultures::culture_tag>,
		member_states, set_tag<nations::region_state_pair>,
		gp_influence, set_tag<nations::influence>,
		influencers, set_tag<nations::country_tag>,
		relations, set_tag<nations::relationship>,
		truces, set_tag<nations::truce>,
		national_focus_locations, set_tag<nations::state_tag>,
		national_flags, set_tag<variables::national_flag_tag>,
		static_modifiers, multiset_tag<modifiers::national_modifier_tag>,
		timed_modifiers, multiset_tag<nations::timed_national_modifier>,
		statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>,
		generals, array_tag<military::leader_tag, int32_t, false>,
		admirals, array_tag<military::leader_tag, int32_t, false>,
		armies, array_tag<military::army_tag, int32_t, false>,
		fleets, array_tag<military::fleet_tag, int32_t, false>,
		active_orders, array_tag<military::army_orders_tag, int32_t, false>,
		strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>,
		active_cbs, array_tag<military::pending_cb, int32_t, false>,
		wars_involved_in, set_tag<military::war_identifier>,
		opponents_in_war, set_tag<nations::country_tag>,
		allies_in_war, set_tag<nations::country_tag>,

		name, text_data::text_tag,
		adjective, text_data::text_tag,
		national_value, modifiers::national_modifier_tag,
		tech_school, modifiers::national_modifier_tag,
		flag, graphics::texture_tag,
		current_color, graphics::color_rgb,
		current_research, technologies::tech_tag,

		military_score, int16_t,
		industrial_score, int16_t,
		overall_rank, int16_t,
		prestige_rank, int16_t,
		military_rank, int16_t,
		industrial_rank, int16_t,

		province_count, uint16_t,
		central_province_count, uint16_t,
		rebel_controlled_provinces, uint16_t,
		blockaded_count, uint16_t,
		crime_count, uint16_t,
		leadership_points, int16_t,
		base_colonial_points, int16_t,
		num_connected_ports, uint16_t,
		num_ports, uint16_t,

		player_importance, int8_t,

		cb_construction_progress, float,
		cb_construction_target, nations::country_tag,
		cb_construction_type, military::cb_type_tag,

		ruling_party, governments::party_tag,
		current_capital, provinces::province_tag,
		tag, cultures::national_tag,
		primary_culture, cultures::culture_tag,
		dominant_culture, cultures::culture_tag,
		dominant_issue, issues::option_tag,
		dominant_ideology, ideologies::ideology_tag,
		dominant_religion, cultures::religion_tag,
		national_religion, cultures::religion_tag,
		current_government, governments::government_tag,
		ruling_ideology, ideologies::ideology_tag,

		f_rich_tax, float,
		f_middle_tax, float,
		f_poor_tax, float,
		f_social_spending, float,
		f_administrative_spending, float,
		f_education_spending, float,
		f_military_spending, float,
		f_tariffs, float,
		f_army_stockpile_spending, float,
		f_navy_stockpile_spending, float,
		f_projects_stockpile_spending, float,

		is_civilized, bitfield_type,
		is_substate, bitfield_type,
		is_mobilized, bitfield_type,
		is_at_war, bitfield_type,
		is_not_ai_controlled, bitfield_type,
		is_holding_election, bitfield_type,
		is_colonial_nation, bitfield_type,
		cb_construction_discovered, bitfield_type,
		has_gas_attack, bitfield_type,
		has_gas_defence, bitfield_type
	>;

	START_INT_MAPPING(nation_modifiers_map)
	MAP_INT(nation_modifiers_map, war_exhaustion, modifiers::national_offsets::war_exhaustion)
	MAP_INT(nation_modifiers_map, max_war_exhaustion, modifiers::national_offsets::max_war_exhaustion)
	MAP_INT(nation_modifiers_map, leadership, modifiers::national_offsets::leadership)
	MAP_INT(nation_modifiers_map, leadership_modifier, modifiers::national_offsets::leadership_modifier)
	MAP_INT(nation_modifiers_map, supply_consumption, modifiers::national_offsets::supply_consumption)
	MAP_INT(nation_modifiers_map, org_regain, modifiers::national_offsets::org_regain)
	MAP_INT(nation_modifiers_map, reinforce_speed, modifiers::national_offsets::reinforce_speed)
	MAP_INT(nation_modifiers_map, land_organisation, modifiers::national_offsets::land_organisation)
	MAP_INT(nation_modifiers_map, naval_organisation, modifiers::national_offsets::naval_organisation)
	MAP_INT(nation_modifiers_map, research_points, modifiers::national_offsets::research_points)
	MAP_INT(nation_modifiers_map, research_points_modifier, modifiers::national_offsets::research_points_modifier)
	MAP_INT(nation_modifiers_map, research_points_on_conquer, modifiers::national_offsets::research_points_on_conquer)
	MAP_INT(nation_modifiers_map, import_cost, modifiers::national_offsets::import_cost)
	MAP_INT(nation_modifiers_map, loan_interest, modifiers::national_offsets::loan_interest)
	MAP_INT(nation_modifiers_map, tax_efficiency, modifiers::national_offsets::tax_efficiency)
	MAP_INT(nation_modifiers_map, min_tax, modifiers::national_offsets::min_tax)
	MAP_INT(nation_modifiers_map, max_tax, modifiers::national_offsets::max_tax)
	MAP_INT(nation_modifiers_map, min_military_spending, modifiers::national_offsets::min_military_spending)
	MAP_INT(nation_modifiers_map, max_military_spending, modifiers::national_offsets::max_military_spending)
	MAP_INT(nation_modifiers_map, min_social_spending, modifiers::national_offsets::min_social_spending)
	MAP_INT(nation_modifiers_map, max_social_spending, modifiers::national_offsets::max_social_spending)
	MAP_INT(nation_modifiers_map, factory_owner_cost, modifiers::national_offsets::factory_owner_cost)
	MAP_INT(nation_modifiers_map, min_tariff, modifiers::national_offsets::min_tariff)
	MAP_INT(nation_modifiers_map, max_tariff, modifiers::national_offsets::max_tariff)
	MAP_INT(nation_modifiers_map, ruling_party_support, modifiers::national_offsets::ruling_party_support)
	MAP_INT(nation_modifiers_map, rich_vote, modifiers::national_offsets::rich_vote)
	MAP_INT(nation_modifiers_map, middle_vote, modifiers::national_offsets::middle_vote)
	MAP_INT(nation_modifiers_map, poor_vote, modifiers::national_offsets::poor_vote)
	MAP_INT(nation_modifiers_map, minimum_wage, modifiers::national_offsets::minimum_wage)
	MAP_INT(nation_modifiers_map, factory_maintenance, modifiers::national_offsets::factory_maintenance)
	MAP_INT(nation_modifiers_map, poor_life_needs, modifiers::national_offsets::poor_life_needs)
	MAP_INT(nation_modifiers_map, rich_life_needs, modifiers::national_offsets::rich_life_needs)
	MAP_INT(nation_modifiers_map, middle_life_needs, modifiers::national_offsets::middle_life_needs)
	MAP_INT(nation_modifiers_map, poor_everyday_needs, modifiers::national_offsets::poor_everyday_needs)
	MAP_INT(nation_modifiers_map, rich_everyday_needs, modifiers::national_offsets::rich_everyday_needs)
	MAP_INT(nation_modifiers_map, middle_everyday_needs, modifiers::national_offsets::middle_everyday_needs)
	MAP_INT(nation_modifiers_map, poor_luxury_needs, modifiers::national_offsets::poor_luxury_needs)
	MAP_INT(nation_modifiers_map, middle_luxury_needs, modifiers::national_offsets::middle_luxury_needs)
	MAP_INT(nation_modifiers_map, rich_luxury_needs, modifiers::national_offsets::rich_luxury_needs)
	MAP_INT(nation_modifiers_map, unemployment_benefit, modifiers::national_offsets::unemployment_benefit)
	MAP_INT(nation_modifiers_map, pension_level, modifiers::national_offsets::pension_level)
	MAP_INT(nation_modifiers_map, global_population_growth, modifiers::national_offsets::global_population_growth)
	MAP_INT(nation_modifiers_map, factory_input, modifiers::national_offsets::factory_input)
	MAP_INT(nation_modifiers_map, factory_output, modifiers::national_offsets::factory_output)
	MAP_INT(nation_modifiers_map, factory_throughput, modifiers::national_offsets::factory_throughput)
	MAP_INT(nation_modifiers_map, rgo_input, modifiers::national_offsets::rgo_input)
	MAP_INT(nation_modifiers_map, rgo_output, modifiers::national_offsets::rgo_output)
	MAP_INT(nation_modifiers_map, rgo_throughput, modifiers::national_offsets::rgo_throughput)
	MAP_INT(nation_modifiers_map, artisan_input, modifiers::national_offsets::artisan_input)
	MAP_INT(nation_modifiers_map, artisan_output, modifiers::national_offsets::artisan_output)
	MAP_INT(nation_modifiers_map, artisan_throughput, modifiers::national_offsets::artisan_throughput)
	MAP_INT(nation_modifiers_map, goods_demand, modifiers::national_offsets::goods_demand)
	MAP_INT(nation_modifiers_map, badboy, modifiers::national_offsets::badboy)
	MAP_INT(nation_modifiers_map, global_assimilation_rate, modifiers::national_offsets::global_assimilation_rate)
	MAP_INT(nation_modifiers_map, prestige, modifiers::national_offsets::prestige)
	MAP_INT(nation_modifiers_map, factory_cost, modifiers::national_offsets::factory_cost)
	MAP_INT(nation_modifiers_map, farm_rgo_eff, modifiers::national_offsets::farm_rgo_eff)
	MAP_INT(nation_modifiers_map, mine_rgo_eff, modifiers::national_offsets::mine_rgo_eff)
	MAP_INT(nation_modifiers_map, farm_rgo_size, modifiers::national_offsets::farm_rgo_size)
	MAP_INT(nation_modifiers_map, mine_rgo_size, modifiers::national_offsets::mine_rgo_size)
	MAP_INT(nation_modifiers_map, issue_change_speed, modifiers::national_offsets::issue_change_speed)
	MAP_INT(nation_modifiers_map, social_reform_desire, modifiers::national_offsets::social_reform_desire)
	MAP_INT(nation_modifiers_map, political_reform_desire, modifiers::national_offsets::political_reform_desire)
	MAP_INT(nation_modifiers_map, literacy_con_impact, modifiers::national_offsets::literacy_con_impact)
	MAP_INT(nation_modifiers_map, rich_income_modifier, modifiers::national_offsets::rich_income_modifier)
	MAP_INT(nation_modifiers_map, middle_income_modifier, modifiers::national_offsets::middle_income_modifier)
	MAP_INT(nation_modifiers_map, poor_income_modifier, modifiers::national_offsets::poor_income_modifier)
	MAP_INT(nation_modifiers_map, global_immigrant_attract, modifiers::national_offsets::global_immigrant_attract)
	MAP_INT(nation_modifiers_map, poor_savings_modifier, modifiers::national_offsets::poor_savings_modifier)
	MAP_INT(nation_modifiers_map, influence_modifier, modifiers::national_offsets::influence_modifier)
	MAP_INT(nation_modifiers_map, diplomatic_points_modifier, modifiers::national_offsets::diplomatic_points_modifier)
	MAP_INT(nation_modifiers_map, mobilisation_size, modifiers::national_offsets::mobilisation_size)
	MAP_INT(nation_modifiers_map, mobilisation_economy_impact, modifiers::national_offsets::mobilisation_economy_impact)
	MAP_INT(nation_modifiers_map, global_pop_militancy_modifier, modifiers::national_offsets::global_pop_militancy_modifier)
	MAP_INT(nation_modifiers_map, global_pop_consciousness_modifier, modifiers::national_offsets::global_pop_consciousness_modifier)
	MAP_INT(nation_modifiers_map, core_pop_militancy_modifier, modifiers::national_offsets::core_pop_militancy_modifier)
	MAP_INT(nation_modifiers_map, core_pop_consciousness_modifier, modifiers::national_offsets::core_pop_consciousness_modifier)
	MAP_INT(nation_modifiers_map, non_accepted_pop_militancy_modifier, modifiers::national_offsets::non_accepted_pop_militancy_modifier)
	MAP_INT(nation_modifiers_map, non_accepted_pop_consciousness_modifier, modifiers::national_offsets::non_accepted_pop_consciousness_modifier)
	MAP_INT(nation_modifiers_map, cb_generation_speed_modifier, modifiers::national_offsets::cb_generation_speed_modifier)
	MAP_INT(nation_modifiers_map, mobilization_impact, modifiers::national_offsets::mobilization_impact)
	MAP_INT(nation_modifiers_map, suppression_points_modifier, modifiers::national_offsets::suppression_points_modifier)
	MAP_INT(nation_modifiers_map, education_efficiency_modifier, modifiers::national_offsets::education_efficiency_modifier)
	MAP_INT(nation_modifiers_map, civilization_progress_modifier, modifiers::national_offsets::civilization_progress_modifier)
	MAP_INT(nation_modifiers_map, administrative_efficiency_modifier, modifiers::national_offsets::administrative_efficiency_modifier)
	MAP_INT(nation_modifiers_map, land_unit_start_experience, modifiers::national_offsets::land_unit_start_experience)
	MAP_INT(nation_modifiers_map, naval_unit_start_experience, modifiers::national_offsets::naval_unit_start_experience)
	MAP_INT(nation_modifiers_map, naval_attack_modifier, modifiers::national_offsets::naval_attack_modifier)
	MAP_INT(nation_modifiers_map, naval_defense_modifier, modifiers::national_offsets::naval_defense_modifier)
	MAP_INT(nation_modifiers_map, land_attack_modifier, modifiers::national_offsets::land_attack_modifier)
	MAP_INT(nation_modifiers_map, land_defense_modifier, modifiers::national_offsets::land_defense_modifier)
	MAP_INT(nation_modifiers_map, tariff_efficiency_modifier, modifiers::national_offsets::tariff_efficiency_modifier)
	MAP_INT(nation_modifiers_map, max_loan_modifier, modifiers::national_offsets::max_loan_modifier)
	MAP_INT(nation_modifiers_map, unciv_economic_modifier, modifiers::national_offsets::unciv_economic_modifier)
	MAP_INT(nation_modifiers_map, unciv_military_modifier, modifiers::national_offsets::unciv_military_modifier)
	MAP_INT(nation_modifiers_map, self_unciv_economic_modifier, modifiers::national_offsets::self_unciv_economic_modifier)
	MAP_INT(nation_modifiers_map, self_unciv_military_modifier, modifiers::national_offsets::self_unciv_military_modifier)
	MAP_INT(nation_modifiers_map, commerce_tech_research_bonus, modifiers::national_offsets::commerce_tech_research_bonus)
	MAP_INT(nation_modifiers_map, army_tech_research_bonus, modifiers::national_offsets::army_tech_research_bonus)
	MAP_INT(nation_modifiers_map, industry_tech_research_bonus, modifiers::national_offsets::industry_tech_research_bonus)
	MAP_INT(nation_modifiers_map, navy_tech_research_bonus, modifiers::national_offsets::navy_tech_research_bonus)
	MAP_INT(nation_modifiers_map, culture_tech_research_bonus, modifiers::national_offsets::culture_tech_research_bonus)
	END_INT_MAPPING
}

namespace state {
	struct factories;
	struct colonizers;
	struct owner;
	struct owner_national_focus;
	struct project;
	struct total_population;

	struct flashpoint_tension_focuses;

	struct administrative_efficiency;
	struct current_tension;

	struct name;
	struct state_capital;
	struct dominant_culture;

	struct crisis_tag;
	struct region_id;

	struct dominant_issue;
	struct dominant_ideology;
	struct dominant_religion;

	struct is_slave_state;
	struct is_colonial;
	struct is_protectorate;

	struct demographics;

	constexpr int32_t factories_count = 8;
	constexpr int32_t colonizers_count = 4;

	constexpr int32_t container_size = 900;

	using container = variable_layout_tagged_vector < nations::state_tag, container_size,
		is_slave_state, bitfield_type,
		is_colonial, bitfield_type,
		is_protectorate, bitfield_type,
		is_slave_state, bitfield_type,

		dominant_religion, cultures::religion_tag,
		dominant_ideology, ideologies::ideology_tag,
		dominant_issue, issues::option_tag,
		region_id, provinces::state_tag,
		crisis_tag, cultures::national_tag,
		dominant_culture, cultures::culture_tag,
		state_capital, provinces::province_tag,
		name, text_data::text_tag,
		current_tension, float,
		administrative_efficiency, float,
		total_population, float,

		flashpoint_tension_focuses, set_tag<nations::country_tag>,

		project, nations::pop_project,
		owner_national_focus, modifiers::national_focus_tag,
		owner, nations::country_tag,

		colonizers, std::array<std::pair<nations::country_tag, int32_t>, colonizers_count>,
		factories, std::array<economy::factory_instance, factories_count>
		>;
}

namespace nations {

	class nations_state {
	public:
		array_tag<country_tag, int32_t, false> nations_by_rank;

		nation::container nations;
		state::container states;

		varying_vectorizable_2d_array<state_tag, economy::goods_tag, float, state::container_size> state_production;
		varying_vectorizable_2d_array<state_tag, economy::goods_tag, float, state::container_size> state_global_demand;
		fixed_vectorizable_2d_array<nations::country_tag, float, nation::container_size, modifiers::national_offsets::count> modifier_values;
		fixed_vectorizable_2d_array<nations::country_tag, float, nation::container_size, technologies::tech_offset::count> tech_attributes;

		stable_2d_vector<float, nations::country_tag, population::rebel_type_tag, 512, 16> local_rebel_support;
		stable_2d_vector<float, nations::country_tag, issues::option_tag, 512, 16> local_movement_support;
		stable_2d_vector<float, nations::country_tag, issues::option_tag, 512, 16> local_movement_radicalism;
		stable_2d_vector<float, nations::country_tag, issues::option_tag, 512, 16> local_movement_radicalism_cache;

		varying_vectorizable_2d_array<nations::country_tag, technologies::tech_tag, bitfield_type, nation::container_size> active_technologies;
		varying_vectorizable_2d_array<nations::country_tag, issues::issue_tag, issues::option_tag, nation::container_size> active_issue_options;


		stable_2d_vector<governments::party_tag, country_tag, ideologies::ideology_tag, 512, 16> active_parties;
		stable_2d_vector<uint8_t, country_tag, ideologies::ideology_tag, 512, 16> upper_house;
		//stable_2d_vector<uint64_t, country_tag, technologies::tech_tag, 512, 16> active_technologies;
		stable_2d_vector<bitfield_type, country_tag, economy::goods_tag, 512, 16> active_goods;
		stable_2d_vector<economy::money_qnty_type, country_tag, economy::goods_tag, 512, 16> collected_tariffs;
		//stable_2d_vector<issues::option_tag, country_tag, issues::issue_tag, 512, 16> active_issue_options;
		stable_2d_vector<economy::goods_qnty_type, country_tag, economy::goods_tag, 512, 16> national_stockpiles;
		stable_2d_vector<float, country_tag, variables::national_variable_tag, 512, 16> national_variables;
		stable_2d_vector<float, country_tag, technologies::adjusted_goods_tag, 512, 16> production_adjustments;
		//stable_2d_vector<military::unit_attribute_vector, country_tag, military::unit_type_tag, 512, 16> unit_stats;
		stable_2d_vector<float, country_tag, population::rebel_type_tag, 512, 16> rebel_org_gain;

		stable_2d_vector<economy::money_qnty_type, state_tag, economy::goods_tag, 512, 16> state_prices;
		stable_2d_vector<economy::money_qnty_type, state_tag, economy::goods_tag, 512, 16> state_price_delta;
		stable_2d_vector<economy::money_qnty_type, state_tag, economy::goods_tag, 512, 16> state_demand;
		stable_2d_vector<array_tag<economy::money_qnty_type, nations::state_tag, true>, state_tag, economy::goods_tag, 512, 16> state_purchases;
		

		stable_variable_vector_storage_mk_2<modifiers::national_modifier_tag, 4, 8192> static_modifier_arrays;
		stable_variable_vector_storage_mk_2<timed_national_modifier, 4, 8192> timed_modifier_arrays;
		stable_variable_vector_storage_mk_2<region_state_pair, 2, 8192> state_arrays;
		stable_variable_vector_storage_mk_2<influence, 2, 8192> influence_arrays;
		stable_variable_vector_storage_mk_2<country_tag, 4, 8192> nations_arrays;
		stable_variable_vector_storage_mk_2<state_tag, 4, 8192> state_tag_arrays;
		stable_variable_vector_storage_mk_2<relationship, 4, 8192> relations_arrays;
		stable_variable_vector_storage_mk_2<truce, 4, 8192> truce_arrays;

		stable_2d_vector<float, state_tag, population::demo_tag, 512, 16> state_demographics;
		stable_2d_vector<float, country_tag, population::demo_tag, 512, 16> nation_demographics;
		stable_2d_vector<float, country_tag, population::demo_tag, 512, 16> nation_colonial_demographics;
	};
}
