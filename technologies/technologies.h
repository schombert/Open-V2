#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "concurrency_tools\\concurrency_tools.h"
#include "text_data\\text_data.h"

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

	class technologies_manager {
	public:
		boost::container::flat_map<text_data::text_tag, tech_category_tag> named_category_index;
		boost::container::flat_map<text_data::text_tag, tech_subcategory_tag> named_subcategory_index;
		boost::container::flat_map<text_data::text_tag, tech_tag> named_technology_index;


		tagged_vector<technology_category, tech_category_tag> technology_categories;
		tagged_vector<technology_subcategory, tech_subcategory_tag> technology_subcategories;
		tagged_vector<technology, tech_tag> technologies_container;
	};

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;
	using tech_file_handler = std::function<void(const token_and_type&, tech_category_tag, parsed_data&, const text_handle_lookup&, technologies_manager&)>;

	tech_file_handler make_subfile_perparse_handler(directory tech_directory);
	void parse_single_tech_file(tech_category_tag cat, const text_handle_lookup& tl, technologies_manager& m, std::vector<token_group>& results);

	void pre_parse_main_technology_file(
		technologies_manager& tech_manager,
		std::vector<token_group>& parse_results,
		const text_handle_lookup& text_function,
		const tech_file_handler& file_function);

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(const text_handle_lookup& tl, const tech_file_handler& fh, technologies_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	parsing_state pre_parse_technologies(
		technologies_manager& tech_manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
};
