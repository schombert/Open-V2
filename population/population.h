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

	struct pop {
		pop_type_tag type;
		pop_tag id;
	};

	class population_manager {
	public:
		boost::container::flat_map<text_data::text_tag, pop_type_tag> named_pop_type_index;
		
		tagged_vector<pop_type, pop_type_tag> pop_types;
		
		uint32_t count_poptypes = 0;
	};

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
}
