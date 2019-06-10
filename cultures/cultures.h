#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"


namespace graphics {
	class texture_manager;
}
namespace scenario {
	class scenario_manager;
}
namespace governments {
	struct government_type;
}

namespace culture {
	struct radicalism;
	struct color;
	struct name;
	struct group;
	struct union_tag;
	struct group_direct;

	struct first_names;
	struct last_names;
}

namespace culture_group {
	struct leader_pictures;
	struct name;
	struct union_tag;
	struct is_overseas;
	struct union_tag_direct;

	struct cultures;
}

namespace religion {
	struct color;
	struct name;
	struct icon;
	struct is_pagan;
}


namespace nation_tag {
	struct culture_group;
	struct color;
	struct tag_code;
	struct first_party;
	struct last_party;
	struct default_name;
	struct base_flag;
	struct communist_flag;
	struct republic_flag;
	struct fascist_flag;
	struct monarchy_flag;

	struct holder;
	struct core_provinces;
	struct capital;
	struct is_not_releasable;
	struct government_names;
	struct flags;
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

	struct national_tag_state {
		set_tag<provinces::province_tag> core_provinces;
		provinces::province_tag capital;
		bool is_not_releasable = false;
	};
}

START_STRUCT(cultures::culture)
STRUCT_MEMBER(culture::radicalism, radicalism)
STRUCT_MEMBER(culture::color, color)
STRUCT_MEMBER(culture::name, name)
STRUCT_MEMBER(culture::group, group)
END_STRUCT

START_STRUCT(cultures::culture_group)
STRUCT_MEMBER(culture_group::leader_pictures, leader_pictures)
STRUCT_MEMBER(culture_group::union_tag, union_tag)
STRUCT_MEMBER(culture_group::name, name)
STRUCT_MEMBER(culture_group::is_overseas, is_overseas)
END_STRUCT

START_STRUCT(cultures::religion)
STRUCT_MEMBER(religion::color, color)
STRUCT_MEMBER(religion::icon, icon)
STRUCT_MEMBER(religion::name, name)
STRUCT_MEMBER(religion::is_pagan, pagan)
END_STRUCT

START_STRUCT(cultures::national_tag_object)
STRUCT_MEMBER(nation_tag::color, color)
STRUCT_MEMBER(nation_tag::tag_code, tag_code)
STRUCT_MEMBER(nation_tag::first_party, first_party)
STRUCT_MEMBER(nation_tag::last_party, last_party)
STRUCT_MEMBER(nation_tag::default_name, default_name)
STRUCT_MEMBER(nation_tag::base_flag, base_flag)
STRUCT_MEMBER(nation_tag::communist_flag, communist_flag)
STRUCT_MEMBER(nation_tag::republic_flag, republic_flag)
STRUCT_MEMBER(nation_tag::fascist_flag, fascist_flag)
STRUCT_MEMBER(nation_tag::monarchy_flag, monarchy_flag)
END_STRUCT

START_STRUCT(cultures::national_tag_state)
STRUCT_MEMBER(nation_tag::core_provinces, core_provinces)
STRUCT_MEMBER(nation_tag::capital, capital)
STRUCT_MEMBER(nation_tag::is_not_releasable, is_not_releasable)
END_STRUCT

namespace cultures {

	class cultures_state {
	public:
		tagged_vector<national_tag_state, national_tag> national_tags_state;
		tagged_vector<nations::country_tag, national_tag, padded_aligned_allocator_64<nations::country_tag>, true> tags_to_holders;
		tagged_fixed_2dvector<graphics::texture_tag, national_tag, governments::government_tag> country_flags_by_government;

		stable_variable_vector_storage_mk_2<culture_tag, 4, 8192> culture_arrays;

		GET_SET(national_tags_state)
		GET_SET_TV(::nation_tag::holder, tags_to_holders)
		GET_SET_TFV(::nation_tag::flags, country_flags_by_government)
		ARRAY_BACKING_BASE(culture_arrays)
	};

	struct leader_pictures;

	class culture_manager {
	public:
		tagged_vector<culture_group, culture_group_tag> culture_groups;
		tagged_vector<religion, religion_tag> religions;
		tagged_vector<culture, culture_tag> culture_container;
		tagged_vector<culture_group_tag, culture_tag, padded_aligned_allocator_64<culture_group_tag>, true> cultures_to_groups;
		tagged_vector<national_tag, culture_group_tag, padded_aligned_allocator_64<national_tag>, true> groups_to_tags;
		tagged_vector<national_tag, culture_tag, padded_aligned_allocator_64<national_tag>, true> cultures_to_tags;
		tagged_vector<culture_group_tag, national_tag, padded_aligned_allocator_64<culture_group_tag>, true> tags_to_groups;

		tagged_vector<national_tag_object, national_tag> national_tags;

		tagged_vector<graphics::texture_tag, int32_t> leader_pictures;
		std::vector<char16_t> name_data;
		v_vector<vector_backed_string<char16_t>, culture_tag> first_names_by_culture;
		v_vector<vector_backed_string<char16_t>, culture_tag> last_names_by_culture;

		v_vector<culture_tag, culture_group_tag> culture_by_culture_group;

		tagged_fixed_2dvector<name_pair, national_tag, governments::government_tag> country_names_by_government;

		boost::container::flat_map<text_data::text_tag, culture_tag> named_culture_index;
		boost::container::flat_map<text_data::text_tag, religion_tag> named_religion_index;
		boost::container::flat_map<text_data::text_tag, culture_group_tag> named_culture_group_index;
		boost::container::flat_map<uint32_t, national_tag> national_tags_index;
		
		uint32_t count_cultures = 0ui32;
		uint32_t count_religions = 0ui32;

		graphics::texture_tag no_leader;
		constexpr static culture_tag immigrant_culture = culture_tag(0);

		template<typename F>
		void for_each_culture(F const& f) const {
			int32_t const cmax = int32_t(count_cultures);
			for(int32_t i = 0; i < cmax; ++i) {
				f(cultures::culture_tag(cultures::culture_tag::value_base_t(i)));
			}
		}
		template<typename F>
		void for_each_religion(F const& f) const {
			int32_t const cmax = int32_t(count_religions);
			for(int32_t i = 0; i < cmax; ++i) {
				f(cultures::religion_tag(cultures::religion_tag::value_base_t(i)));
			}
		}
		template<typename F>
		void for_each_culture_group(F const& f) const {
			int32_t const cmax = int32_t(culture_groups.size());
			for(int32_t i = 0; i < cmax; ++i) {
				f(cultures::culture_group_tag(cultures::culture_group_tag::value_base_t(i)));
			}
		}

		GET_SET(culture_groups)
		GET_SET(religions)
		GET_SET(culture_container)
		GET_SET(national_tags)
		GET_SET_TV(::culture::union_tag, cultures_to_tags)
		GET_SET_TV(::culture::group_direct, cultures_to_groups)
		GET_SET_TV(::culture_group::union_tag_direct, groups_to_tags)
		GET_SET_TV(::nation_tag::culture_group, tags_to_groups)
		GET_SET_TFV(::nation_tag::government_names, country_names_by_government)
		GET_SET_TFV(::culture::first_names, first_names_by_culture)
		GET_SET_TFV(::culture::last_names, last_names_by_culture)
		GET_SET_TFV(::culture_group::cultures, culture_by_culture_group)
		GET_SET_TV(::cultures::leader_pictures, leader_pictures)
	};

	struct tag_as_text {
		char tag[4];
	};

	uint32_t tag_to_encoding(const char* start, const char* end);

	template<size_t N>
	uint32_t tag_to_encoding(const char(&t)[N]) {
		return tag_to_encoding(t, t + N - 1);
	}

	tag_as_text encoded_tag_to_text_tag(uint32_t tag_value);
}
