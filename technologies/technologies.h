#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.h"
#include "military\\military.h"

namespace scenario {
	class scenario_manager;
}

namespace modifiers {
	class modifiers_manager;
}

namespace technologies {
	enum class tech_category_type : uint8_t {
		army, navy, commerce, industry, culture, undefined
	};

	constexpr uint32_t number_of_subcategories = 5ui32;
	constexpr uint32_t number_of_member_techs = 6ui32;

	struct technology_category {
		text_data::text_tag name;
		tech_subcategory_tag subcategories[number_of_subcategories];
		tech_category_type type = tech_category_type::undefined;
		tech_category_tag id;
	};

	struct technology_subcategory {
		tech_tag member_techs[number_of_member_techs];
		text_data::text_tag name;

		tech_category_tag parent;
		tech_subcategory_tag id;
	};

	using tech_attribute_type = float;

	namespace tech_offset {
		constexpr int32_t max_national_focus = 0;
		constexpr int32_t war_exhaustion = 1;
		constexpr int32_t supply_limit = 2;
		constexpr int32_t prestige = 3;
		constexpr int32_t combat_width = 4;
		constexpr int32_t dig_in_cap = 5;
		constexpr int32_t influence = 6;
		constexpr int32_t repair_rate = 7;
		constexpr int32_t reinforce_rate = 8;
		constexpr int32_t soldier_to_pop_loss = 9;
		constexpr int32_t regular_experience_level = 10;
		constexpr int32_t colonial_life_rating = 11;
		constexpr int32_t education_efficiency = 12;
		constexpr int32_t military_tactics = 13;
		constexpr int32_t seperatism = 14;
		constexpr int32_t land_attrition = 15;
		constexpr int32_t naval_attrition = 16;
		constexpr int32_t supply_range = 17;
		constexpr int32_t plurality = 18;
		constexpr int32_t factory_cost = 19;
		constexpr int32_t permanent_prestige = 20;
		constexpr int32_t colonial_prestige = 21;
		constexpr int32_t max_fort = 22;
		constexpr int32_t max_naval_base = 23;
		constexpr int32_t max_railroad = 24;
		constexpr int32_t morale = 25;
		constexpr int32_t colonial_migration = 26;
		constexpr int32_t colonial_points = 27;
		constexpr int32_t cb_creation_speed = 28;

		constexpr int32_t count = 29;
		constexpr static size_t aligned_32_size = ((sizeof(tech_attribute_type) * count + 31ui64) & ~31ui64) / sizeof(tech_attribute_type);
	}

	constexpr uint32_t tech_definition_size = 8;
	__declspec(align(64)) struct tech_definition {
		int32_t offsets[tech_definition_size] = { 0,0,0,0,0,0,0,0 };
		float values[tech_definition_size] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	};

	struct alignas(64) technology {
		static constexpr uint8_t activate_railroad = 0x01;
		static constexpr uint8_t activate_fort = 0x02;
		static constexpr uint8_t activate_naval_base = 0x04;
		static constexpr uint8_t unciv_military = 0x08;

		static constexpr uint8_t gas_attack = 0x10;
		static constexpr uint8_t gas_defence = 0x20;

		tech_definition attributes;

		float shared_prestige = 0.0f;
		uint16_t year = 0ui16;
		uint16_t cost = 0ui16;

		text_data::text_tag name;
		graphics::texture_tag picture;

		rebel_adjustment_tag rebel_adjustment;
		production_adjustment_tag production_adjustment;
		unit_adjustment_tag unit_adjustment;

		triggers::trigger_tag allow;
		modifiers::national_modifier_tag modifier;
		modifiers::factor_tag ai_chance; // or: invention chance
		economy::factory_type_tag activate_factory;

		modifiers::provincial_modifier_tag enable_crime;

		tech_tag preceeding;
		tech_tag id;

		tech_category_type category = tech_category_type::undefined;
		uint8_t flags = 0ui8;
	};


	using adjusted_goods_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 23020578>>;

	namespace production_adjustment {
		constexpr int32_t rgo_size = 0;
		constexpr int32_t rgo_goods_output = 1;
		constexpr int32_t rgo_goods_throughput = 2;
		constexpr int32_t factory_goods_input = 3;
		constexpr int32_t factory_goods_output = 4;
		constexpr int32_t factory_goods_throughput = 5;
		constexpr int32_t artisan_goods_input = 6;
		constexpr int32_t artisan_goods_output = 7;
		constexpr int32_t artisan_goods_throughput = 8;

		constexpr int32_t production_adjustment_count = 9;
	}

	template<int32_t production_type>
	inline adjusted_goods_tag economy_tag_to_production_adjustment(economy::goods_tag t) {
		return adjusted_goods_tag(adjusted_goods_tag::value_base_t(to_index(t) * production_adjustment::production_adjustment_count + production_type));
	}

	

	class technologies_state {
	public:
		tagged_vector<int32_t, tech_tag> discovery_count;
	};

	class technologies_manager {
	public:
		boost::container::flat_map<text_data::text_tag, tech_category_tag> named_category_index;
		boost::container::flat_map<text_data::text_tag, tech_subcategory_tag> named_subcategory_index;
		boost::container::flat_map<text_data::text_tag, tech_tag> named_technology_index;

		tagged_vector<technology_category, tech_category_tag> technology_categories;
		tagged_vector<technology_subcategory, tech_subcategory_tag> technology_subcategories;
		tagged_vector<technology, tech_tag, aligned_allocator_64<technology>> technologies_container;
		std::vector<tech_tag> inventions;

		tagged_fixed_blocked_2dvector<float, production_adjustment_tag, adjusted_goods_tag, aligned_allocator_32<float>> production_adjustments;
		tagged_fixed_blocked_2dvector<military::unit_attribute_vector, unit_adjustment_tag, military::unit_type_tag, aligned_allocator_32<military::unit_attribute_vector>>
			unit_type_adjustments;
		tagged_fixed_blocked_2dvector<float, rebel_adjustment_tag, population::rebel_type_tag, aligned_allocator_32<float>> rebel_org_gain;

		std::vector<text_data::text_tag> tech_modifier_names;
	};
}
