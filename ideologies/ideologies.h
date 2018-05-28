#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "concurrency_tools\\concurrency_tools.h"
#include "text_data\\text_data.h"

namespace scenario {
	class scenario_manager;
}

namespace ideologies {
	struct ideology_group {
		text_data::text_tag name;
		ideology_group_tag id;
	};

	struct ideology {
		graphics::color_rgb color;
		text_data::text_tag name;
		date_tag enable_date;
		modifiers::factor_tag add_political_reform;
		modifiers::factor_tag add_social_reform;
		modifiers::factor_tag remove_political_reform;
		modifiers::factor_tag remove_social_reform;
		modifiers::factor_tag add_military_reform;
		modifiers::factor_tag add_economic_reform;
		ideology_group_tag group;
		ideology_tag id;
		bool uncivilized = true;
	};

	class ideologies_manager {
	public:
		boost::container::flat_map<text_data::text_tag, ideology_group_tag> named_group_index;
		boost::container::flat_map<text_data::text_tag, ideology_tag> named_ideology_index;

		tagged_vector<ideology_group, ideology_group_tag> ideology_groups;
		tagged_vector<ideology, ideology_tag> ideology_container;

		ideology_tag conservative_ideology;
	};

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(const text_handle_lookup& tl, ideologies_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	parsing_state pre_parse_ideologies(
		ideologies_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
	void parse_single_ideology(scenario::scenario_manager& s, ideology_tag tag, const token_group* start, const token_group* end);
	void parse_ideologies(scenario::scenario_manager& s, const parsing_state& state);
}
