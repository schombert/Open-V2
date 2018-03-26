#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"

namespace modifiers {
	struct provincial_modifier {
		text_data::text_tag name;
		provincial_modifier_tag id;
	};

	struct national_modifier {
		text_data::text_tag name;
		national_modifier_tag id;
	};

	class modifiers_manager {
	public:
		tagged_vector<national_modifier, national_modifier_tag> national_modifiers;
		tagged_vector<provincial_modifier, provincial_modifier_tag> provincial_modifiers;

		boost::container::flat_map<text_data::text_tag, national_modifier_tag> named_national_modifiers_index;
		boost::container::flat_map<text_data::text_tag, provincial_modifier_tag> named_provincial_modifiers_index;
	};

	struct parsing_environment;

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(const text_handle_lookup& tl, modifiers_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void pre_parse_crimes(
		parsing_state& state,
		const directory& source_directory);
	void pre_parse_national_values(
		parsing_state& state,
		const directory& source_directory);
}
