#pragma once
#include "common\\common.h"
#include "military.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "simple_fs\\simple_fs.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>


class world_state;

template<>
class serialization::serializer<military::ship> : public serialization::memcpy_serializer<military::ship> {};
template<>
class serialization::serializer<military::military_leader> : public serialization::memcpy_serializer<military::military_leader> {};
template<>
class serialization::serializer<military::war_goal> : public serialization::memcpy_serializer<military::war_goal> {};
template<>
class serialization::serializer<military::naval_control> : public serialization::memcpy_serializer<military::war_goal> {};

template<>
class serialization::serializer<military::cb_type> : public serialization::memcpy_serializer<military::cb_type> {};
template<>
class serialization::serializer<military::unit_type> : public serialization::memcpy_serializer<military::unit_type> {};

template<>
class serialization::serializer<military::army_orders> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, military::army_orders const& obj, world_state const&);
	static void deserialize_object(std::byte const* &input, military::army_orders& obj, world_state& ws);
	static size_t size(military::army_orders const&, world_state const&);
	static size_t size();
};

template<>
class serialization::serializer<military::army> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, military::army const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, military::army& obj, world_state& ws);
	static size_t size(military::army const& obj, world_state const& ws);
};

template<>
class serialization::serializer<military::fleet> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, military::fleet const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, military::fleet& obj, world_state& ws);
	static size_t size(military::fleet const& obj, world_state const& ws);
};

template<>
class serialization::serializer<military::war> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, military::war const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, military::war& obj, world_state& ws);
	static size_t size(military::war const& obj, world_state const& ws);
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
		for(auto const& i_unit : obj.unit_types)
			obj.named_unit_type_index.emplace(i_unit.name, i_unit.id);
		for(auto const& i_cb : obj.cb_types)
			obj.named_cb_type_index.emplace(i_cb.name, i_cb.id);
		for(int32_t i = static_cast<int32_t>(obj.leader_traits.size()) - 1; i >= 0; --i)
			obj.named_leader_trait_index.emplace(obj.leader_traits[military::leader_trait_tag(static_cast<military::leader_trait_tag::value_base_t>(i))], military::leader_trait_tag(static_cast<military::leader_trait_tag::value_base_t>(i)));
		obj.unit_types_count = uint32_t(obj.unit_types.size());
	}

	static void serialize_object(std::byte* &output, military::military_manager const& obj) {
		serialize(output, obj.cb_types);
		serialize(output, obj.unit_types);
		serialize(output, obj.leader_traits);
		serialize(output, obj.leader_trait_definitions);
		serialize(output, obj.personality_traits);
		serialize(output, obj.background_traits);
		serialize(output, obj.unit_build_costs);
		serialize(output, obj.unit_base_supply_costs);
	}
	static void deserialize_object(std::byte const* &input, military::military_manager& obj) {
		deserialize(input, obj.cb_types);
		deserialize(input, obj.unit_types);
		deserialize(input, obj.leader_traits);
		deserialize(input, obj.leader_trait_definitions);
		deserialize(input, obj.personality_traits);
		deserialize(input, obj.background_traits);
		deserialize(input, obj.unit_build_costs);
		deserialize(input, obj.unit_base_supply_costs);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, military::military_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.cb_types);
		deserialize(input, obj.unit_types);
		deserialize(input, obj.leader_traits);
		deserialize(input, obj.leader_trait_definitions);
		deserialize(input, obj.personality_traits);
		deserialize(input, obj.background_traits);
		deserialize(input, obj.unit_build_costs);
		deserialize(input, obj.unit_base_supply_costs);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(military::military_manager const& obj) {
		return serialize_size(obj.cb_types) +
			serialize_size(obj.unit_types) +
			serialize_size(obj.leader_traits) +
			serialize_size(obj.leader_trait_definitions) +
			serialize_size(obj.personality_traits) +
			serialize_size(obj.background_traits) +
			serialize_size(obj.unit_build_costs) +
			serialize_size(obj.unit_base_supply_costs);
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
