#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"

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

	struct parsing_environment;

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(const text_handle_lookup& tl, province_manager& m, modifiers::modifiers_manager& mm);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void read_default_map_file(
		parsing_state& state,
		const directory& source_directory);

	boost::container::flat_map<uint32_t, modifiers::provincial_modifier_tag>
		pre_parse_terrain(
		parsing_state& state,
		const directory& source_directory); // returns color to terrain tag map; adds provincial modifiers

	void read_states(
		parsing_state& state,
		const directory& source_directory);

	void pre_parse_continents(
		parsing_state& state,
		const directory& source_directory); // adds provincial modifiers

	void pre_parse_climates(
		parsing_state& state,
		const directory& source_directory); // adds provincial modifiers
}
