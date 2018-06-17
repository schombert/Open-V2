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
namespace scenario {
	class scenario_manager;
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

	struct name_pair {
		text_data::text_tag name;
		text_data::text_tag adjective;
	};

	struct national_tag_object {
		graphics::color_rgb color;
		uint32_t tag_code = 0ui32;
		national_tag id;

		governments::party_tag first_party;
		governments::party_tag last_party;

		name_pair default_name;

		graphics::texture_tag base_flag;
		graphics::texture_tag communist_flag;
		graphics::texture_tag republic_flag;
		graphics::texture_tag fascist_flag;
		graphics::texture_tag monarchy_flag;
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

		tagged_fixed_2dvector<name_pair, national_tag, governments::government_tag> country_names_by_government;

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

	void read_religions(
		culture_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text);
	void read_cultures(
		culture_manager& manager,
		graphics::texture_manager &tm,
		const directory& source_directory,
		text_data::text_sequences& text);
	tagged_vector<std::string, national_tag> read_national_tags(
		culture_manager& manager,
		const directory& source_directory); // invoke before parsing cultures, returns tag files array

	void read_country_files(tagged_vector<std::string, national_tag> const& v, scenario::scenario_manager& s, const directory& source_directory);
	void read_flag_graphics(scenario::scenario_manager& s, const directory& source_directory);
	void populate_country_names(scenario::scenario_manager& s, tagged_vector<std::string, governments::government_tag> const& gbase_names);
}
