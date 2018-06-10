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

namespace population {

	enum class income_type : uint8_t {
		administration, military, education, reforms, none
	};

	struct government_employment {
		float life_needs_income_weight = 0.0f;
		float everyday_needs_income_weight = 0.0f;
		float luxury_needs_income_weight = 0.0f;

		income_type life_needs_income_type = income_type::none;
		income_type everyday_needs_income_type = income_type::none;
		income_type luxury_needs_income_type = income_type::none;
	};

	struct pop_type {
		static constexpr uint8_t cannot_vote = 0x04;
		static constexpr uint8_t is_employable = 0x08;
		static constexpr uint8_t state_capital_only = 0x10;
		static constexpr uint8_t demote_on_migration = 0x20;

		static constexpr uint8_t strata_mask = 0x03;
		static constexpr uint8_t strata_poor = 0x00;
		static constexpr uint8_t strata_middle = 0x01;
		static constexpr uint8_t strata_rich = 0x02;

		graphics::color_rgb color;

		float research_optimum = 0.0f;

		text_data::text_tag name;
		
		modifiers::factor_tag migration_target;
		modifiers::factor_tag country_migration_target;

		uint8_t sprite = 0ui8;
		uint8_t research_points = 0ui8;

		uint8_t flags = 0ui8;
		
		pop_type_tag id;
	};


	struct rebel_type {
		static constexpr uint16_t restrict_by_culture = 0x0001;
		static constexpr uint16_t restrict_by_religion = 0x0002;
		static constexpr uint16_t restrict_by_ideology = 0x0004;
		static constexpr uint16_t restrict_by_culture_group = 0x0008;
		static constexpr uint16_t break_alliance_on_win = 0x0080;

		static constexpr uint16_t area_mask = 0x0070;

		static constexpr uint16_t area_all = 0x0000;
		static constexpr uint16_t area_nation = 0x0010;
		static constexpr uint16_t area_culture = 0x0020;
		static constexpr uint16_t area_nation_culture = 0x0030;
		static constexpr uint16_t area_nation_religion = 0x0040;
		static constexpr uint16_t area_religion = 0x0050;
		static constexpr uint16_t area_culture_group = 0x0060;

		static constexpr uint16_t defection_mask = 0x0700;

		static constexpr uint16_t defection_none = 0x0000;
		static constexpr uint16_t defection_culture = 0x0100;
		static constexpr uint16_t defection_culture_group = 0x0200;
		static constexpr uint16_t defection_religion = 0x0300;
		static constexpr uint16_t defection_ideology = 0x0400;
		static constexpr uint16_t defection_any = 0x0500;
		static constexpr uint16_t defection_pan_nationalist = 0x0600;

		static constexpr uint16_t independence_mask = 0x7000;

		static constexpr uint16_t independence_none = 0x0000;
		static constexpr uint16_t independence_culture = 0x1000;
		static constexpr uint16_t independence_culture_group = 0x2000;
		static constexpr uint16_t independence_religion = 0x3000;
		static constexpr uint16_t independence_colonial = 0x4000;
		static constexpr uint16_t independence_any = 0x5000;
		static constexpr uint16_t independence_pan_nationalist = 0x6000;

		float occupation_mult = 1.0f;

		text_data::text_tag name;

		modifiers::factor_tag spawn_chance;
		modifiers::factor_tag will_rise;
		triggers::trigger_tag siege_won_trigger;
		triggers::effect_tag siege_won_effect;
		triggers::trigger_tag demands_enforced_trigger;
		triggers::effect_tag demands_enforced_effect;

		uint16_t flags = 0ui16;

		ideologies::ideology_tag ideology;

		uint8_t icon = 0ui8;
		rebel_type_tag id;
	};

	struct pop {
		pop_type_tag type;
		pop_tag id;
	};

	class population_manager {
	public:
		boost::container::flat_map<text_data::text_tag, pop_type_tag> named_pop_type_index;
		boost::container::flat_map<text_data::text_tag, rebel_type_tag> named_rebel_type_index;
		
		tagged_vector<pop_type, pop_type_tag> pop_types;
		tagged_vector<rebel_type, rebel_type_tag> rebel_types;
		
		tagged_fixed_blocked_2dvector<
			economy::goods_qnty_type,
			pop_type_tag,
			economy::goods_tag,
			aligned_allocator_32<economy::goods_qnty_type>> life_needs;
		tagged_fixed_blocked_2dvector<
			economy::goods_qnty_type,
			pop_type_tag,
			economy::goods_tag,
			aligned_allocator_32<economy::goods_qnty_type>> everyday_needs;
		tagged_fixed_blocked_2dvector<
			economy::goods_qnty_type,
			pop_type_tag,
			economy::goods_tag,
			aligned_allocator_32<economy::goods_qnty_type>> luxury_needs;
		tagged_fixed_blocked_2dvector<
			float,
			pop_type_tag,
			military::unit_type_tag,
			aligned_allocator_32<float>> rebel_units;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, issues::option_tag> issue_inclination;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, ideologies::ideology_tag> ideological_inclination;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, pop_type_tag> promote_to;
		tagged_fixed_2dvector<modifiers::factor_tag, pop_type_tag, pop_type_tag> demote_to;

		tagged_fixed_2dvector<governments::government_tag, rebel_type_tag, governments::government_tag> rebel_change_government_to;

		modifiers::factor_tag promotion_chance;
		modifiers::factor_tag demotion_chance;
		modifiers::factor_tag migration_chance;
		modifiers::factor_tag colonialmigration_chance;
		modifiers::factor_tag emigration_chance;
		modifiers::factor_tag assimilation_chance;
		modifiers::factor_tag conversion_chance;

		government_employment clergy_pay;
		government_employment bureaucrat_pay;
		government_employment soldier_pay;
		government_employment officer_pay;

		int32_t officer_leadership = 2;

		pop_type_tag artisan;
		pop_type_tag capitalist;
		pop_type_tag clergy;
		pop_type_tag bureaucrat;
		pop_type_tag slave;
		pop_type_tag soldier;
		pop_type_tag officer;

		uint32_t count_poptypes = 0;
	};

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, population_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function);

	void pre_parse_rebel_types(
		parsing_state& state,
		const directory& source_directory);

	void read_main_poptype_file(scenario::scenario_manager& s, const directory& root);
	void read_poptypes(scenario::scenario_manager& s, const directory& root);
	void populate_demote_to(population_manager& m);
}
