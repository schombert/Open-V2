#pragma once
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "text_data\\text_data.h"
#include <variant>

namespace cultures {
	struct culture {
		graphics::color_rgb color;
		text_data::text_tag name;
		culture_tag id;
		culture_group_tag group;
	};
	struct culture_group {
		text_data::text_tag name;
		national_tag union_tag;
		culture_group_tag id;
	};
	struct religion {
		graphics::color_rgb color;
		text_data::text_tag name;
		religion_tag id;
	};
	struct national_tag_object {
		national_tag id;
		//eventually: flags
	};

	class culture_manager {
	public:
		tagged_vector<culture_group, culture_group_tag> culture_groups;
		tagged_vector<religion, religion_tag> religions;
		tagged_vector<culture, culture_tag> culture_container;
		tagged_vector<national_tag_object, national_tag> national_tags;

		std::vector<char16_t> name_data;
		v_vector<vector_backed_string<char16_t>, value_base_of<culture_tag>> first_names_by_culture;
		v_vector<vector_backed_string<char16_t>, value_base_of<culture_tag>> last_names_by_culture;

		boost::container::flat_map<text_data::text_tag, culture_tag> named_culture_index;
		boost::container::flat_map<text_data::text_tag, religion_tag> named_religion_index;
		boost::container::flat_map<text_data::text_tag, culture_group_tag> named_culture_group_index;
		boost::container::flat_map<uint32_t, national_tag> national_tags_index;
	};

	uint32_t tag_to_encoding(const char* start, const char* end);

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	void parse_religions(
		culture_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
	void parse_cultures(
		culture_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
	void parse_national_tags(
		culture_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function); // invoke before parsing cultures
}