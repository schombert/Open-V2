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
