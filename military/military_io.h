#pragma once
#include "common\\common.h"
#include "military.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "simple_fs\\simple_fs.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>
#include "military_containers.h"


class world_state;

template<>
class serialization::serializer<military::war_goal> : public serialization::memcpy_serializer<military::war_goal> {};
template<>
class serialization::serializer<military::naval_control> : public serialization::memcpy_serializer<military::war_goal> {};
template<>
class serialization::serializer<military::cb_type> : public serialization::memcpy_serializer<military::cb_type> {};



template<typename T>
class serialization::tagged_serializer<army_order::province_set, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<army_order::army_set, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<strategic_hq::army_set, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<strategic_hq::province_set, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<war::attackers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
};
template<typename T>
class serialization::tagged_serializer<war::defenders, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
};
template<typename T>
class serialization::tagged_serializer<war::naval_control_set, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
};


template<>
class serialization::serializer<military::military_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, military::military_state const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, military::military_state& obj, world_state& ws);
	static size_t size(military::military_state const& obj, world_state const& ws);
};

template<>
class serialization::serializer<military::military_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(military::military_manager& obj) {

		obj.cb_type_to_speed.resize(obj.cb_types.size());
		for(auto const& i_cb : obj.cb_types) {
			obj.named_cb_type_index.emplace(i_cb.name, i_cb.id);
			obj.cb_type_to_speed[i_cb.id] = i_cb.construction_speed;
		}
		for(int32_t i = static_cast<int32_t>(obj.leader_traits.size()) - 1; i >= 0; --i)
			obj.named_leader_trait_index.emplace(obj.leader_traits[military::leader_trait_tag(static_cast<military::leader_trait_tag::value_base_t>(i))], military::leader_trait_tag(static_cast<military::leader_trait_tag::value_base_t>(i)));
		
		
	}

	static void serialize_object(std::byte* &output, military::military_manager const& obj) {
		serialize(output, obj.cb_types);
		serialize(output, obj.leader_traits);
		serialize(output, obj.leader_trait_definitions);
		serialize(output, obj.personality_traits);
		serialize(output, obj.background_traits);
	}
	static void deserialize_object(std::byte const* &input, military::military_manager& obj) {
		deserialize(input, obj.cb_types);
		deserialize(input, obj.leader_traits);
		deserialize(input, obj.leader_trait_definitions);
		deserialize(input, obj.personality_traits);
		deserialize(input, obj.background_traits);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, military::military_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.cb_types);
		deserialize(input, obj.leader_traits);
		deserialize(input, obj.leader_trait_definitions);
		deserialize(input, obj.personality_traits);
		deserialize(input, obj.background_traits);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(military::military_manager const& obj) {
		return serialize_size(obj.cb_types) +
			serialize_size(obj.leader_traits) +
			serialize_size(obj.leader_trait_definitions) +
			serialize_size(obj.personality_traits) +
			serialize_size(obj.background_traits);
	}
};

namespace military {
	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, military_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void pre_parse_unit_types(
		parsing_state& state,
		const directory& source_directory);
	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory);
	void read_leader_traits(parsing_state& state,
		const directory& source_directory);
	void read_unit_types(
		parsing_state& state,
		military_manager& military_m,
		economy::economic_scenario& economy_m,
		sound::sound_manager& sound_m,
		text_data::text_sequences& text_m);
	void read_cb_types(parsing_state const& state, scenario::scenario_manager& s, events::event_creation_manager& ecm);
	void read_oob_file(world_state& ws, nations::country_tag for_nation, token_group const* start, token_group const* end);
	void read_wars(world_state& ws, date_tag target_date, const directory& root);
}
