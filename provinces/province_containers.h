#pragma once
#include "common\common.h"
#include "provinces.h"
#include "provinces_io.h"
#include "province.h"
#include "province_state.h"

namespace provinces {
	class provinces_state {
	public:
		province_state::container province_state_container;
		fixed_vectorizable_2d_array<provinces::province_tag, modifiers::value_type, province_state::container_size, modifiers::provincial_offsets::count> modifier_values;

		tagged_fixed_2dvector<float, province_tag, ideologies::ideology_tag> party_loyalty;
		tagged_fixed_blocked_2dvector<float, province_tag, population::demo_tag, aligned_allocator_32<int32_t>> province_demographics;

		std::vector<uint8_t> is_canal_enabled;

		std::vector<float> province_distance_to;
		std::vector<province_tag> province_path_to;
		state_distances_manager state_distances;

		stable_variable_vector_storage_mk_2<cultures::national_tag, 4, 8192> core_arrays;
		stable_variable_vector_storage_mk_2<modifiers::provincial_modifier_tag, 4, 8192> static_modifier_arrays;
		stable_variable_vector_storage_mk_2<timed_provincial_modifier, 4, 8192> timed_modifier_arrays;

		stable_variable_vector_storage_mk_2<province_tag, 4, 8192> province_arrays;

		modifiers::provincial_modifier_tag europe_modifier;
		modifiers::provincial_modifier_tag asia_modifier;
		modifiers::provincial_modifier_tag north_america_modifier;
		modifiers::provincial_modifier_tag south_america_modifier;
		modifiers::provincial_modifier_tag africa_modifier;
		modifiers::provincial_modifier_tag oceania_modifier;
	};

	class province_manager {
	public:
		province::container province_container;
		tagged_vector<text_data::text_tag, state_tag> state_names;
		std::vector<provinces::province_tag> integer_to_province;
		int32_t first_sea_province = 0;

		v_vector<province_tag, state_tag> states_to_province_index;
		boost::container::flat_map<text_data::text_tag, state_tag> named_states_index;

		v_vector<province_tag, province_tag> same_type_adjacency;
		v_vector<province_tag, province_tag> coastal_adjacency;
		std::vector<std::tuple<province_tag, province_tag, text_data::text_tag, province_tag>> canals; // connection A, connection B, name, connection through

		boost::container::flat_map<modifiers::provincial_modifier_tag, graphics::obj_definition_tag> terrain_graphics;

		std::vector<uint16_t> province_map_data;
		int32_t province_map_width = 0;
		int32_t province_map_height = 0;
		borders_manager borders;

		template<typename F>
		void for_each_province(F const& f) const {
			int32_t const cmax = int32_t(province_container.size());
			for(int32_t i = 1; i < cmax; ++i) {
				f(province_tag(province_tag::value_base_t(i)));
			}
		}
		template<typename F>
		void for_each_land_province(F const& f) const {
			int32_t const cmax = first_sea_province;
			for(int32_t i = 1; i < cmax; ++i) {
				f(province_tag(province_tag::value_base_t(i)));
			}
		}
	};
}

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
		serialize_array(output, obj.integer_to_province.data(), obj.province_container.size());
		serialize(output, obj.state_names);
		serialize(output, obj.states_to_province_index);
		serialize(output, obj.same_type_adjacency);
		serialize(output, obj.coastal_adjacency);
		serialize(output, obj.canals);
		serialize(output, obj.terrain_graphics);
		serialize(output, obj.province_map_data);
		serialize(output, obj.province_map_width);
		serialize(output, obj.province_map_height);
		serialize(output, obj.first_sea_province);
		serialize(output, obj.borders);
	}
	static void deserialize_object(std::byte const* &input, provinces::province_manager& obj) {
		deserialize(input, obj.province_container);
		obj.integer_to_province.resize(obj.province_container.size());
		deserialize_array(input, obj.integer_to_province.data(), obj.province_container.size());
		deserialize(input, obj.state_names);
		deserialize(input, obj.states_to_province_index);
		deserialize(input, obj.same_type_adjacency);
		deserialize(input, obj.coastal_adjacency);
		deserialize(input, obj.canals);
		deserialize(input, obj.terrain_graphics);
		deserialize(input, obj.province_map_data);
		deserialize(input, obj.province_map_width);
		deserialize(input, obj.province_map_height);
		deserialize(input, obj.first_sea_province);
		deserialize(input, obj.borders);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, provinces::province_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.province_container);
		obj.integer_to_province.resize(obj.province_container.size());
		deserialize_array(input, obj.integer_to_province.data(), obj.province_container.size());
		deserialize(input, obj.state_names);
		deserialize(input, obj.states_to_province_index);
		deserialize(input, obj.same_type_adjacency);
		deserialize(input, obj.coastal_adjacency);
		deserialize(input, obj.canals);
		deserialize(input, obj.terrain_graphics);
		deserialize(input, obj.province_map_data);
		deserialize(input, obj.province_map_width);
		deserialize(input, obj.province_map_height);
		deserialize(input, obj.first_sea_province);
		deserialize(input, obj.borders);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(provinces::province_manager const& obj) {
		return serialize_size(obj.province_container) +
			sizeof(provinces::province_tag) * obj.province_container.size() + /*integer_to_province*/
			serialize_size(obj.state_names) +
			serialize_size(obj.states_to_province_index) +
			serialize_size(obj.same_type_adjacency) +
			serialize_size(obj.coastal_adjacency) +
			serialize_size(obj.canals) +
			serialize_size(obj.terrain_graphics) +
			serialize_size(obj.province_map_data) +
			serialize_size(obj.province_map_width) +
			serialize_size(obj.province_map_height) +
			serialize_size(obj.first_sea_province) +
			serialize_size(obj.borders);;
	}
	template<typename T>
	static size_t size(provinces::province_manager const& obj, T const&) {
		return size(obj);
	}
};

