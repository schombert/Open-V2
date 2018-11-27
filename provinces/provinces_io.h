#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "simple_fs\\simple_fs.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "graphics_objects\\graphics_objects.h"
#include <ppl.h>
#include <map>

class world_state;

template<typename T>
class serialization::tagged_serializer<province_state::cores, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.province_s.core_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.province_s.core_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.province_s.core_arrays, obj);
	}
};

template<typename T>
class serialization::tagged_serializer<province_state::timed_modifiers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.province_s.timed_modifier_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.province_s.timed_modifier_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.province_s.timed_modifier_arrays, obj);
	}
};

template<typename T>
class serialization::tagged_serializer<province_state::static_modifiers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws) {
		serialize_stable_array(output, ws.w.province_s.static_modifier_arrays, obj);
	}
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws) {
		deserialize_stable_array(input, ws.w.province_s.static_modifier_arrays, obj);
	}
	static size_t size(T const& obj, world_state const& ws) {
		return serialize_stable_array_size(ws.w.province_s.static_modifier_arrays, obj);
	}
};

template<typename T>
class serialization::tagged_serializer<province_state::pops, T> : public serialization::discard_serializer<T> {};

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

template<>
class serialization::serializer<provinces::provinces_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, provinces::provinces_state const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, provinces::provinces_state& obj, world_state& ws);
	static size_t size(provinces::provinces_state const& obj, world_state const& ws);
};

template<>
class serialization::serializer<provinces::province_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(provinces::province_manager& obj) {
		for(int32_t i = static_cast<int32_t>(obj.state_names.size()) - 1; i >= 0; --i)
			obj.named_states_index.emplace(obj.state_names[provinces::state_tag(static_cast<provinces::state_tag::value_base_t>(i))], provinces::state_tag(static_cast<provinces::state_tag::value_base_t>(i)));
	}

	static void serialize_object(std::byte* &output, provinces::province_manager const& obj) {
		serialize(output, obj.province_container);
		serialize(output, obj.state_names);
		serialize(output, obj.states_to_province_index);
		serialize(output, obj.same_type_adjacency);
		serialize(output, obj.coastal_adjacency);
		serialize(output, obj.canals);
		serialize(output, obj.terrain_graphics);
		serialize(output, obj.province_map_data);
		serialize(output, obj.province_map_width);
		serialize(output, obj.province_map_height);
	}
	static void deserialize_object(std::byte const* &input, provinces::province_manager& obj) {
		deserialize(input, obj.province_container);
		deserialize(input, obj.state_names);
		deserialize(input, obj.states_to_province_index);
		deserialize(input, obj.same_type_adjacency);
		deserialize(input, obj.coastal_adjacency);
		deserialize(input, obj.canals);
		deserialize(input, obj.terrain_graphics);
		deserialize(input, obj.province_map_data);
		deserialize(input, obj.province_map_width);
		deserialize(input, obj.province_map_height);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, provinces::province_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.province_container);
		deserialize(input, obj.state_names);
		deserialize(input, obj.states_to_province_index);
		deserialize(input, obj.same_type_adjacency);
		deserialize(input, obj.coastal_adjacency);
		deserialize(input, obj.canals);
		deserialize(input, obj.terrain_graphics);
		deserialize(input, obj.province_map_data);
		deserialize(input, obj.province_map_width);
		deserialize(input, obj.province_map_height);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(provinces::province_manager const& obj) {
		return serialize_size(obj.province_container) +
			serialize_size(obj.state_names) +
			serialize_size(obj.states_to_province_index) +
			serialize_size(obj.same_type_adjacency) +
			serialize_size(obj.coastal_adjacency) +
			serialize_size(obj.canals) +
			serialize_size(obj.terrain_graphics) +
			serialize_size(obj.province_map_data) +
			serialize_size(obj.province_map_width) +
			serialize_size(obj.province_map_height);
	}
};

namespace provinces {
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
	void read_adjacnencies_file(std::map<province_tag, boost::container::flat_set<province_tag>>& adj_map, std::vector<std::tuple<province_tag, province_tag, text_data::text_tag, province_tag>>& canals, directory const& root, text_data::text_sequences& text);
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
	boost::container::flat_map<uint32_t, province_tag> read_province_definition_file(directory const& source_directory);

	void read_province_history(world_state& ws, province_tag p, date_tag target_date, token_group const* start, token_group const* end);
	void read_province_histories(world_state& ws, const directory& root, date_tag target_date);
	void calculate_province_areas(province_manager& m, float top_latitude, float bottom_latitude);
	void set_base_rgo_size(world_state& ws);
}
