#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include <variant>

namespace ideologies {
	class ideologies_manager;
}

namespace governments {
	enum class flag_type : uint8_t {
		communist, republic, fascist, monarchy, not_specified
	};

	flag_type text_to_flag_type(const char* start, const char* end);
	const char* flag_type_to_text(flag_type t);

	struct government_type {
		std::string name_base;
		text_data::text_tag name;
		uint32_t duration = 48;
		government_tag id;
		flag_type flag = flag_type::not_specified;
		bool appoint_ruling_party = true;
		bool election = false;
	};

	class governments_manager {
	public:
		boost::container::flat_map<text_data::text_tag, government_tag> named_government_index;

		tagged_vector<government_type, government_tag> governments_container;
		tagged_fixed_2dvector<uint8_t, government_tag, ideologies::ideology_tag> permitted_ideologies;
	};

	void read_governments(
		governments_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function,
		const ideologies::ideologies_manager& ideologies_source);
}
