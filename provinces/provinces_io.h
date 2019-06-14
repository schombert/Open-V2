#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "simple_fs\\simple_fs.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "graphics_objects\\graphics_objects.h"
#include <ppl.h>
#include "economy\economy.h"

class world_state;


template<>
class serialization::serializer<economy::worked_instance> : public serialization::memcpy_serializer<economy::worked_instance> {};

template<>
class serialization::serializer<provinces::borders_manager::province_border_info> : public serialization::memcpy_serializer<provinces::borders_manager::province_border_info> {};

template<>
class serialization::serializer<provinces::borders_manager::border_block> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, provinces::borders_manager::border_block const& obj) {
		serialize(output, obj.coastal_borders_size);
		serialize(output, obj.state_borders_size);
		serialize(output, obj.province_borders_size);
		serialize(output, obj.province_borders);
		serialize(output, obj.indices_data);
		serialize(output, obj.vertices_data);
	}
	static void deserialize_object(std::byte const* &input, provinces::borders_manager::border_block& obj) {
		deserialize(input, obj.coastal_borders_size);
		deserialize(input, obj.state_borders_size);
		deserialize(input, obj.province_borders_size);
		deserialize(input, obj.province_borders);
		deserialize(input, obj.indices_data);
		deserialize(input, obj.vertices_data);
	}
	static size_t size(provinces::borders_manager::border_block const& obj) {
		return serialize_size(obj.coastal_borders_size) +
			serialize_size(obj.state_borders_size) +
			serialize_size(obj.province_borders_size) +
			serialize_size(obj.province_borders) +
			serialize_size(obj.indices_data) +
			serialize_size(obj.vertices_data);
	}
};

template<>
class serialization::serializer<provinces::borders_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, provinces::borders_manager const& obj) {
		serialize(output, obj.borders);
	}
	static void deserialize_object(std::byte const* &input, provinces::borders_manager& obj) {
		deserialize(input, obj.borders);
	}
	static size_t size(provinces::borders_manager const& obj) {
		return serialize_size(obj.borders);
	}
};

template<typename T>
class serialization::tagged_serializer<province_state::cores, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};

template<typename T>
class serialization::tagged_serializer<province_state::timed_modifiers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};

template<typename T>
class serialization::tagged_serializer<province_state::static_modifiers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};

template<typename T>
class serialization::tagged_serializer<province_state::pops, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<province_state::total_population, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<province_state::fleets, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<province_state::dominant_culture, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<province_state::dominant_religion, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<province_state::dominant_issue, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<province_state::dominant_ideology, T> : public serialization::discard_serializer<T> {};


namespace provinces {
	class province_manager;
	class provinces_state;

	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, province_manager& m, modifiers::modifiers_manager& mm);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void read_default_map_file(
		parsing_state& state,
		const directory& source_directory);

	void read_terrain_modifiers(
		text_data::text_sequences& text,
		province_manager& pm,
		modifiers::modifiers_manager& mm,
		graphics::name_maps& gname_maps,
		const directory& source_directory); // adds provincial modifiers
	color_to_terrain_map read_terrain_colors(
		text_data::text_sequences& text,
		province_manager& pm,
		modifiers::modifiers_manager& mm,
		const directory& source_directory); // returns color to terrain tag map
	
	tagged_vector<uint8_t, province_tag> generate_province_terrain(size_t province_count, uint16_t const* province_map_data, uint8_t const* terrain_color_map_data, int32_t height, int32_t width);
	tagged_vector<uint8_t, province_tag> generate_province_terrain_inverse(size_t province_count, uint16_t const* province_map_data, uint8_t const* terrain_color_map_data, int32_t height, int32_t width);
	void load_province_map_data(province_manager& m, directory const& root); // returns province to terrain color array
	tagged_vector<uint8_t, province_tag> load_province_terrain_data(province_manager& m, directory const& root);
	void assign_terrain_color(provinces_state& m, tagged_vector<uint8_t, province_tag> const & terrain_colors, color_to_terrain_map const & terrain_map);
	
	std::map<province_tag, boost::container::flat_set<province_tag>> generate_map_adjacencies(uint16_t const* province_map_data, int32_t height, int32_t width);
	void read_adjacnencies_file(std::map<province_tag, boost::container::flat_set<province_tag>>& adj_map, directory const& root, scenario::scenario_manager& s);
	void make_lakes(std::map<province_tag, boost::container::flat_set<province_tag>>& adj_map, province_manager& m);
	void make_adjacency(std::map<province_tag, boost::container::flat_set<province_tag>>& adj_map, province_manager& m);

	void read_states(
		parsing_state& state,
		const directory& source_directory);
	void read_continents(
		parsing_state& state,
		const directory& source_directory); // adds provincial modifiers
	void read_climates(
		parsing_state& state,
		const directory& source_directory); // adds provincial modifiers
	boost::container::flat_map<uint32_t, int32_t> read_province_definition_file(directory const& source_directory);

	void read_province_history(world_state& ws, province_tag p, date_tag target_date, token_group const* start, token_group const* end);
	void read_province_histories(world_state& ws, const directory& root, date_tag target_date);
	void calculate_province_areas(province_manager& m, float top_latitude, float bottom_latitude);
	void set_base_rgo_size(world_state& ws);
}
