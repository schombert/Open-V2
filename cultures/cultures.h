#pragma once
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "text_data\\text_data.h"
#include <variant>

namespace graphics {
	class texture_manager;
}

namespace governments {
	struct government_type;
}

namespace cultures {
	struct leader_picture_info {
		uint16_t admiral_offset = 0ui16;
		uint16_t admiral_size = 0ui16;
		uint16_t general_offset = 0ui16;
		uint16_t general_size = 0ui16;
	};
	struct culture {
		float radicalism = 0.0f;
		graphics::color_rgb color;
		text_data::text_tag name;
		culture_tag id;
		culture_group_tag group;
	};
	struct culture_group {
		leader_picture_info leader_pictures;
		text_data::text_tag name;
		national_tag union_tag;
		culture_group_tag id;
		bool is_overseas = true;
	};
	struct religion {
		graphics::color_rgb color;
		text_data::text_tag name;
		religion_tag id;
		uint8_t icon = 0ui8;
		bool pagan = false;
	};
	struct national_tag_object {
		graphics::color_rgb color;
		uint32_t tag_code = 0ui32;
		national_tag id;

		//eventually: flags
	};

	class culture_manager {
	public:
		tagged_vector<culture_group, culture_group_tag> culture_groups;
		tagged_vector<religion, religion_tag> religions;
		tagged_vector<culture, culture_tag> culture_container;
		tagged_vector<national_tag_object, national_tag> national_tags;

		std::vector<graphics::texture_tag> leader_pictures;
		std::vector<char16_t> name_data;
		v_vector<vector_backed_string<char16_t>, value_base_of<culture_tag>> first_names_by_culture;
		v_vector<vector_backed_string<char16_t>, value_base_of<culture_tag>> last_names_by_culture;

		boost::container::flat_map<text_data::text_tag, culture_tag> named_culture_index;
		boost::container::flat_map<text_data::text_tag, religion_tag> named_religion_index;
		boost::container::flat_map<text_data::text_tag, culture_group_tag> named_culture_group_index;
		boost::container::flat_map<uint32_t, national_tag> national_tags_index;

		graphics::texture_tag no_leader;
	};

	struct tag_as_text {
		char tag[4];
	};

	uint32_t tag_to_encoding(const char* start, const char* end);
	tag_as_text encoded_tag_to_text_tag(uint32_t tag_value);

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	void parse_religions(
		culture_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
	void parse_cultures(
		culture_manager& manager,
		graphics::texture_manager &tm,
		const directory& source_directory,
		const text_handle_lookup& text_function);
	tagged_vector<std::string, national_tag> parse_national_tags(
		culture_manager& manager,
		const directory& source_directory); // invoke before parsing cultures, returns tag files array

	std::pair<text_data::text_tag, text_data::text_tag> get_name_and_adjective(
		const national_tag_object&,
		const governments::government_type&,
		const text_data::text_sequences& );
}
