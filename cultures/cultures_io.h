#pragma once
#include "common\\common.h"
#include "cultures.h"
#include "simple_fs\\simple_fs.h"
#include "text_data\\text_data.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>


class world_state;

template<>
class serialization::serializer<cultures::culture> : public serialization::memcpy_serializer<cultures::culture> {};
template<>
class serialization::serializer<cultures::culture_group> : public serialization::memcpy_serializer<cultures::culture_group> {};
template<>
class serialization::serializer<cultures::religion> : public serialization::memcpy_serializer<cultures::religion> {};
template<>
class serialization::serializer<cultures::national_tag_object> : public serialization::memcpy_serializer<cultures::national_tag_object> {};
template<>
class serialization::serializer<cultures::name_pair> : public serialization::memcpy_serializer<cultures::name_pair> {};

template<>
class serialization::serializer<cultures::national_tag_state> {
public:
	static constexpr bool has_static_size = true;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, cultures::national_tag_state const& obj, world_state const&);
	static void deserialize_object(std::byte const* &input, cultures::national_tag_state& obj, world_state&);
	static size_t size();
};

template<>
class serialization::serializer<cultures::cultures_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, cultures::cultures_state const& obj, world_state const& ws) {
		serialize(output, obj.national_tags_state, ws);
		serialization::serialize_array(output, obj.tags_to_holders.data() + 1, obj.national_tags_state.size());
		serialize(output, obj.country_flags_by_government);
	}
	static void deserialize_object(std::byte const* &input, cultures::cultures_state& obj, world_state& ws) {
		deserialize(input, obj.national_tags_state, ws);
		obj.tags_to_holders.resize(obj.national_tags_state.size());
		serialization::deserialize_array(input, obj.tags_to_holders.data() + 1, obj.national_tags_state.size());
		deserialize(input, obj.country_flags_by_government);
	}
	static size_t size(cultures::cultures_state const& obj, world_state const& ws) {
		return serialize_size(obj.national_tags_state, ws) + serialize_size(obj.country_flags_by_government);
	}
};

template<>
class serialization::serializer<cultures::culture_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(cultures::culture_manager& obj) {
		obj.cultures_to_groups.resize(obj.culture_container.size());
		for(auto const& i_culture : obj.culture_container) {
			obj.named_culture_index.emplace(i_culture.name, i_culture.id);
			obj.culture_by_culture_group.add_to_row(i_culture.group, i_culture.id);
			obj.cultures_to_groups[i_culture.id] = i_culture.group;
			obj.cultures_to_tags[i_culture.id] = obj.culture_groups[i_culture.group].union_tag;
		}
		obj.groups_to_tags.resize(obj.culture_groups.size());
		for(auto const& i_culture_group : obj.culture_groups) {
			obj.named_culture_group_index.emplace(i_culture_group.name, i_culture_group.id);
			obj.groups_to_tags[i_culture_group.id] = i_culture_group.union_tag;
		}
		
		for(auto const& i_religion : obj.religions)
			obj.named_religion_index.emplace(i_religion.name, i_religion.id);
		for(auto const& i_tag : obj.national_tags)
			obj.national_tags_index.emplace(i_tag.tag_code, i_tag.id);

		obj.count_cultures = uint32_t(obj.culture_container.size());
		obj.count_religions = uint32_t(obj.religions.size());
	}

	static void serialize_object(std::byte* &output, cultures::culture_manager const& obj) {
		serialize(output, obj.culture_groups);
		serialize(output, obj.religions);
		serialize(output, obj.culture_container);
		serialize(output, obj.national_tags);
		serialize(output, obj.leader_pictures);
		serialize(output, obj.name_data);
		serialize(output, obj.first_names_by_culture);
		serialize(output, obj.last_names_by_culture);
		serialize(output, obj.country_names_by_government);
	}
	static void deserialize_object(std::byte const* &input, cultures::culture_manager& obj) {
		deserialize(input, obj.culture_groups);
		deserialize(input, obj.religions);
		deserialize(input, obj.culture_container);
		deserialize(input, obj.national_tags);
		deserialize(input, obj.leader_pictures);
		deserialize(input, obj.name_data);
		deserialize(input, obj.first_names_by_culture);
		deserialize(input, obj.last_names_by_culture);
		deserialize(input, obj.country_names_by_government);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, cultures::culture_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.culture_groups);
		deserialize(input, obj.religions);
		deserialize(input, obj.culture_container);
		deserialize(input, obj.national_tags);
		deserialize(input, obj.leader_pictures);
		deserialize(input, obj.name_data);
		deserialize(input, obj.first_names_by_culture);
		deserialize(input, obj.last_names_by_culture);
		deserialize(input, obj.country_names_by_government);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(cultures::culture_manager const& obj) {
		return serialize_size(obj.culture_groups) +
			serialize_size(obj.religions) +
			serialize_size(obj.culture_container) +
			serialize_size(obj.national_tags) +
			serialize_size(obj.leader_pictures) +
			serialize_size(obj.name_data) +
			serialize_size(obj.first_names_by_culture) +
			serialize_size(obj.last_names_by_culture) +
			serialize_size(obj.country_names_by_government);
	}
};

namespace cultures {
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
