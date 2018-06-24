#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"

namespace modifiers {
	class modifiers_manager;
}

namespace provinces {

	struct province_state {
		void* owner = nullptr; // 8
		void* controller = nullptr; // 16
		void* rebel_controller = nullptr; // 24
		void* state_instance = nullptr; // 32

		atomic_tag<date_tag> last_update; // 36
		float nationalism = 0.0f; // 40
		date_tag last_controller_change; // 44

		modifiers::provincial_modifier_tag crime; // 46
		modifiers::provincial_modifier_tag terrain; // 48

		
		int16_t life_rating = 0i16; // 50
		province_tag id; // 52

		economy::goods_tag rgo_production; // 53

		uint8_t fort_level = 0ui8; // 54
		uint8_t railroad_level = 0ui8; // 55
		uint8_t naval_base_level = 0ui8; // 56
		uint8_t rgo_size = 1ui8; // 57
	};

	static_assert(sizeof(province_state) == 64); //IMPORTANT: 1 cache line

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

	class provinces_state {
	public:
		tagged_vector<province_state, province_tag> province_state_container;
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
