#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"

namespace population {
	struct pop_type {
		text_data::text_tag name;
		pop_type_tag id;
	};

	struct rebel_type {
		text_data::text_tag name;
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
		
		uint32_t count_poptypes = 0;
	};

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(const text_handle_lookup& tl, population_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);

	void pre_parse_rebel_types(
		parsing_state& state,
		const directory& source_directory);
}
