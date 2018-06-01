#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "concurrency_tools\\concurrency_tools.hpp"

namespace economy {
	class economic_scenario;
}
namespace sound {
	class sound_manager;
}
namespace military {
	struct cb_type {
		text_data::text_tag name;
		cb_type_tag id;
	};

	using unit_attribute_type = float;

	namespace unit_type_attribute {
		constexpr int32_t defense = 0;
		constexpr int32_t hull = 0;
		constexpr int32_t attack = 1;
		constexpr int32_t gun_power = 1;
		constexpr int32_t reconnaissance = 2;
		constexpr int32_t fire_range = 2;
		constexpr int32_t support = 3;
		constexpr int32_t torpedo_attack = 3;
		constexpr int32_t maneuver = 4;
		constexpr int32_t evasion = 4;
		constexpr int32_t speed = 5;
		constexpr int32_t organization = 6;
		constexpr int32_t build_time = 7;
		constexpr int32_t supply_consumption = 8;
		constexpr int32_t strength = 9;
		constexpr int32_t siege = 10;
		constexpr int32_t discipline = 11;

		constexpr int32_t count = 12;
		constexpr static size_t aligned_32_size = ((sizeof(unit_attribute_type) * count + 31ui64) & ~31ui64) / sizeof(unit_attribute_type);
	}

	struct alignas(32) unit_type {
		Eigen::Matrix<
			unit_attribute_type,
			unit_type_attribute::aligned_32_size,
			1> base_attributes = Eigen::Matrix<unit_attribute_type, unit_type_attribute::aligned_32_size, 1>::Zero();

		static constexpr uint8_t primary_culture = 0x10;
		static constexpr uint8_t default_inactive = 0x20;
		static constexpr uint8_t cant_build_overseas = 0x40;
		static constexpr uint8_t is_sail = 0x80;

		static constexpr uint8_t class_mask = 0x0F;

		static constexpr uint8_t class_infantry = 0x00;
		static constexpr uint8_t class_cavalry = 0x01;
		static constexpr uint8_t class_special = 0x02;
		static constexpr uint8_t class_support = 0x03;
		static constexpr uint8_t class_big_ship = 0x04;
		static constexpr uint8_t class_light_ship = 0x05;
		static constexpr uint8_t class_transport = 0x06;

		text_data::text_tag name; // 2 bytes
		
		sound::effect_tag select_sound; // 3 bytes
		sound::effect_tag move_sound; // 4 bytes

		int8_t limit_per_port = -1i8; // 5 bytes
		uint8_t supply_consumption_score = 0ui8; // 6 bytes
		uint8_t icon = 0ui8; // 7 bytes
		uint8_t naval_icon = 0ui8; // 8 bytes
		uint8_t colonial_points = 0ui8; // 9 bytes
		uint8_t min_port_level = 0ui8; // 10 bytes

		uint8_t flags = 0ui8; // 11 bytes

		unit_type_tag id; // 12 bytes
	};

	const size_t type_size = sizeof(unit_type);
	

	namespace traits {
		constexpr int32_t organisation = 0;
		constexpr int32_t morale = 1;
		constexpr int32_t attack = 2;
		constexpr int32_t defence = 3;
		constexpr int32_t reconnaissance = 4;
		constexpr int32_t speed = 5;
		constexpr int32_t experience = 6;
		constexpr int32_t reliability = 7;

		constexpr int32_t trait_count = 8;

		using value_type = float;
	}

	

	class military_manager {
	public:
		tagged_vector<cb_type, cb_type_tag> cb_types;
		tagged_vector<unit_type, unit_type_tag> unit_types;

		tagged_vector<text_data::text_tag, leader_trait_tag> leader_traits;
		tagged_fixed_blocked_2dvector<traits::value_type, leader_trait_tag, uint32_t, aligned_allocator_32<traits::value_type>> leader_trait_definitions;

		std::vector<leader_trait_tag> personality_traits;
		std::vector<leader_trait_tag> background_traits;
		leader_trait_tag no_personality_trait;
		leader_trait_tag no_background_trait;

		boost::container::flat_map<text_data::text_tag, unit_type_tag> named_unit_type_index;
		boost::container::flat_map<text_data::text_tag, cb_type_tag> named_cb_type_index;
		boost::container::flat_map<text_data::text_tag, leader_trait_tag> named_leader_trait_index;

		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, unit_type_tag, economy::goods_tag, aligned_allocator_32<economy::goods_qnty_type>> unit_build_costs;
		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, unit_type_tag, economy::goods_tag, aligned_allocator_32<economy::goods_qnty_type>> unit_base_supply_costs;
	};

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, military_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void pre_parse_unit_types(
		parsing_state& state,
		const directory& source_directory);
	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory);
	void parse_leader_traits(parsing_state& state,
		const directory& source_directory);
	void read_unit_types(
		parsing_state& state,
		military_manager& military_m,
		economy::economic_scenario& economy_m,
		sound::sound_manager& sound_m,
		text_data::text_sequences& text_m);
}
