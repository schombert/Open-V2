#pragma once
#include "nations.h"
#include "simple_fs\\simple_fs.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "technologies\\technologies_functions.h"

class world_state;

template<>
class serialization::serializer<nations::influence> : public serialization::memcpy_serializer<nations::influence> {};
template<>
class serialization::serializer<nations::relationship> : public serialization::memcpy_serializer<nations::relationship> {};
template<>
class serialization::serializer<nations::truce> : public serialization::memcpy_serializer<nations::truce> {};
template<>
class serialization::serializer<nations::timed_national_modifier> : public serialization::memcpy_serializer<nations::timed_national_modifier> {};
template<>
class serialization::serializer<nations::pop_project> : public serialization::memcpy_serializer<nations::pop_project> {};

template<>
class serialization::serializer<nations::nation> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, nations::nation const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, nations::nation& obj, world_state& ws);
	static size_t size(nations::nation const& obj, world_state const& ws);
};

template<typename T>
class serialization::tagged_serializer<state::flashpoint_tension_focuses, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
};

template<typename T>
class serialization::tagged_serializer<state::dominant_ideology, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::dominant_issue, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::dominant_religion, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::dominant_culture, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::administrative_efficiency, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<state::state_capital, T> : public serialization::discard_serializer<T> {};

template<>
class serialization::serializer<nations::nations_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, nations::nations_state const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, nations::nations_state& obj, world_state& ws);
	static size_t size(nations::nations_state const& obj, world_state const& ws);
};

namespace nations {
	void read_nation_file(world_state& ws, cultures::national_tag nat_tag_id, date_tag target_date, file const& fi, directory const& units_dir, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_nations_files(world_state& ws, date_tag target_date, directory const& root, std::vector<std::pair<country_tag, events::decision_tag>>& decisions);
	void read_diplomacy_files(world_state& ws, date_tag target_date, directory const& root);
	void set_default_tech_school(world_state& ws);
}
