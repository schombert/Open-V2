#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"

namespace modifiers {
	class modifiers_manager;
}

namespace provinces {

	struct province_state {
		void* owner;
		void* controller;
		void* rebel_controller;

		float nationalism = 0.0f;
		
		modifiers::provincial_modifier_tag crime;
		modifiers::provincial_modifier_tag terrain;

		date_tag last_controller_change;
		int16_t life_rating = 0i16;

		economy::goods_tag rgo_production;

		uint8_t fort_level = 0ui8;
		uint8_t railroad_level = 0ui8;
		uint8_t naval_base_level = 0ui8;
		uint8_t rgo_size = 1ui8;
	};

	struct province {
		constexpr static uint16_t sea = 0x0001;
		constexpr static uint16_t coastal = 0x0002;
		constexpr static uint16_t lake = 0x0004;

		//modifiers::provincial_modifier_tag terrain;
		modifiers::provincial_modifier_tag continent;
		modifiers::provincial_modifier_tag climate;

		
		text_data::text_tag name;

		state_tag state_id;
		province_tag id;

		uint16_t flags = 0;
	};

	class province_manager {
	public:
		tagged_vector<province, province_tag> province_container;
		tagged_vector<text_data::text_tag, state_tag> state_names;

		boost::container::flat_multimap<state_tag, province_tag> states_to_province_index;
		boost::container::flat_map<text_data::text_tag, state_tag> named_states_index;

		v_vector<province_tag::value_base_t, province_tag::value_base_t> same_type_adjacency;
		v_vector<province_tag::value_base_t, province_tag::value_base_t> coastal_adjacency;
		std::vector<std::pair<province_tag, province_tag>> canals;

		std::vector<uint16_t> province_map_data;
		int32_t province_map_width = 0;
		int32_t province_map_height = 0; 
	};

	struct color_to_terrain_map {
		modifiers::provincial_modifier_tag data[256] = { modifiers::provincial_modifier_tag() };
	};

	constexpr uint32_t rgb_to_prov_index(uint8_t r, uint8_t g, uint8_t b) {
		return static_cast<uint32_t>(r) | (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(b) << 16);
	}
}
