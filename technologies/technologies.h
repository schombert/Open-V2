#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "concurrency_tools\\concurrency_tools.h"
#include "text_data\\text_data.h"

namespace modifiers {
	class modifiers_manager;
}

namespace technologies {
	struct technology_category {
		text_data::text_tag name;

		tech_category_tag id;
	};

	struct technology_subcategory {
		text_data::text_tag name;

		tech_category_tag parent;
		tech_subcategory_tag id;
	};

	struct technology {
		text_data::text_tag name;

		tech_tag id;
		tech_subcategory_tag parent;
	};

	struct invention {
		text_data::text_tag name;
		invention_tag id;
	};

	class technologies_manager {
	public:
		boost::container::flat_map<text_data::text_tag, tech_category_tag> named_category_index;
		boost::container::flat_map<text_data::text_tag, tech_subcategory_tag> named_subcategory_index;
		boost::container::flat_map<text_data::text_tag, tech_tag> named_technology_index;
		boost::container::flat_map<text_data::text_tag, modifiers::national_modifier_tag> named_tech_school_index;
		boost::container::flat_map<text_data::text_tag, invention_tag> named_invention_index;


		tagged_vector<technology_category, tech_category_tag> technology_categories;
		tagged_vector<technology_subcategory, tech_subcategory_tag> technology_subcategories;
		tagged_vector<technology, tech_tag> technologies_container;
		tagged_vector<invention, invention_tag> inventions;
	};

	
	void pre_parse_single_tech_file(
		tech_category_tag cat,
		text_data::text_sequences& tl,
		technologies_manager& m,
		const token_group* start,
		const token_group* end);

	void parse_main_technology_file(
		technologies_manager& tech_manager,
		std::vector<token_group>& parse_results,
		text_data::text_sequences& text_function,
		const directory& tech_root,
		modifiers::modifiers_manager& mm);

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, const directory& tech_directory, technologies_manager& m, modifiers::modifiers_manager& mm);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void parse_technologies(
		parsing_state& state,
		const directory& source_directory);
	void pre_parse_inventions(
		parsing_state& state,
		const directory& source_directory);
};
