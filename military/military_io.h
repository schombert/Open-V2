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
class serialization::serializer<military::war_goal> : public serialization::memcpy_serializer<military::war_goal> {};
template<>
class serialization::serializer<military::naval_control> : public serialization::memcpy_serializer<military::war_goal> {};
template<>
class serialization::serializer<military::cb_type> : public serialization::memcpy_serializer<military::cb_type> {};
template<>
class serialization::serializer<military::army_orders_type> : public serialization::memcpy_serializer<military::army_orders_type> {};



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
	static size_t size();
};
template<typename T>
class serialization::tagged_serializer<war::defenders, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};
template<typename T>
class serialization::tagged_serializer<war::naval_control_set, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};
template<typename T>
class serialization::tagged_serializer<war::war_goals, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};

namespace scenario {
	class scenario_manager;
}

namespace military {
	template<typename S>
	struct parsing_environment;

	class military_manager;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment<scenario::scenario_manager>> impl;

		parsing_state(scenario::scenario_manager& m, events::event_creation_manager& e);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	//void pre_parse_unit_types(
	//	parsing_state& state,
	//	const directory& source_directory);
	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory);
	void read_leader_traits(parsing_state& state,
		const directory& source_directory);
	//void read_unit_types(
	//	parsing_state& state,
	//	military_manager& military_m,
	//	economy::economic_scenario& economy_m,
	//	sound::sound_manager& sound_m,
	//	text_data::text_sequences& text_m);
	void read_cb_types(parsing_state const& state);
	void read_oob_file(world_state& ws, nations::country_tag for_nation, token_generator& gen);
	void read_wars(world_state& ws, date_tag target_date, const directory& root);
}
