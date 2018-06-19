#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"

namespace modifiers {
	class modifiers_manager;
}

namespace provinces {
	struct province {
		constexpr static uint16_t sea = 0x0001;
		constexpr static uint16_t coastal = 0x0002;

		modifiers::provincial_modifier_tag terrain;
		modifiers::provincial_modifier_tag continent;
		modifiers::provincial_modifier_tag climate;

		modifiers::provincial_modifier_tag crime;

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
	};

	constexpr uint32_t rgb_to_prov_index(uint8_t r, uint8_t g, uint8_t b) {
		return static_cast<uint32_t>(r) | (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(b) << 16);
	}
}
