#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"

namespace military {
	struct cb_type {
		text_data::text_tag name;
		cb_type_tag id;
	};

	struct unit_type {
		text_data::text_tag name;
		unit_type_tag id;
	};

	class military_manager {
	public:
		tagged_vector<cb_type, cb_type_tag> cb_types;
		tagged_vector<unit_type, unit_type_tag> unit_types;

		boost::container::flat_map<text_data::text_tag, unit_type_tag> named_unit_type_index;
		boost::container::flat_map<text_data::text_tag, cb_type_tag> named_cb_type_index;
	};

	struct parsing_environment;

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(const text_handle_lookup& tl, military_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void pre_parse_unit_types(
		parsing_state& state,
		const directory& source_directory);
	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory);
}
