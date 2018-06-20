#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "simple_fs\\simple_fs.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include <ppl.h>

template<>
class serialization::serializer<provinces::province> : public serialization::memcpy_serializer<provinces::province> {};

template<>
class serialization::serializer<provinces::province_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(provinces::province_manager& obj) {
		for(int32_t i = static_cast<int32_t>(obj.state_names.size()) - 1; i >= 0; --i)
			obj.named_states_index.emplace(obj.state_names[provinces::state_tag(static_cast<provinces::state_tag::value_base_t>(i))], provinces::state_tag(static_cast<provinces::state_tag::value_base_t>(i)));
		for(auto const& i_prov : obj.province_container)
			obj.states_to_province_index.emplace(i_prov.state_id, i_prov.id);
	}

	static void serialize_object(std::byte* &output, provinces::province_manager const& obj) {
		serialize(output, obj.province_container);
		serialize(output, obj.state_names);
	}
	static void deserialize_object(std::byte const* &input, provinces::province_manager& obj) {
		deserialize(input, obj.province_container);
		deserialize(input, obj.state_names);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, provinces::province_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.province_container);
		deserialize(input, obj.state_names);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(provinces::province_manager const& obj) {
		return serialize_size(obj.province_container) +
			serialize_size(obj.state_names);
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

	boost::container::flat_map<uint32_t, modifiers::provincial_modifier_tag>
		pre_parse_terrain(
			parsing_state& state,
			const directory& source_directory); // returns color to terrain tag map; adds provincial modifiers
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
}
