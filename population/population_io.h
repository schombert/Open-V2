#pragma once
#include "common\\common.h"
#include "population.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "simple_fs\\simple_fs.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include <ppl.h>

class world_state;

template<>
class serialization::serializer<population::pop_type> : public serialization::memcpy_serializer<population::pop_type> {};
template<>
class serialization::serializer<population::rebel_type> : public serialization::memcpy_serializer<population::rebel_type> {};
template<>
class serialization::serializer<population::government_employment> : public serialization::memcpy_serializer<population::government_employment> {};

template<typename T>
class serialization::tagged_serializer<pop::social_interest, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<pop::political_interest, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<pop::is_accepted, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<pop::is_poor, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<pop::is_middle, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<pop::size, T> : public serialization::discard_serializer<T> {};


/*template<>
class serialization::serializer<population::pop_movement> {
public:
	static constexpr bool has_static_size = true;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, population::pop_movement const& obj);
	static void deserialize_object(std::byte const* &input, population::pop_movement& obj);
	static size_t size(population::pop_movement const& obj);
	static size_t size();
};*/

/*template<>
class serialization::serializer<population::rebel_faction> {
public:
	static constexpr bool has_static_size = true;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, population::rebel_faction const& obj);
	static void deserialize_object(std::byte const* &input, population::rebel_faction& obj, world_state& ws);
	static size_t size(population::rebel_faction const& obj);
	static size_t size();
};*/

namespace population {
	class population_manager;
	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, population_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function);

	void pre_parse_rebel_types(
		parsing_state& state,
		const directory& source_directory);

	void read_main_poptype_file(scenario::scenario_manager& s, const directory& root);
	void read_poptypes(scenario::scenario_manager& s, const directory& root);
	void read_rebel_types(parsing_state const& state, scenario::scenario_manager& s, events::event_creation_manager& ecm);
	void populate_demote_to(population_manager& m);

	int read_pops_in_province(token_group const* s, token_group const* e, token_and_type const& pid, world_state& ws);
	void read_pop_file(token_group const* s, token_group const* e, world_state& ws);
	void read_all_pops(directory const& root, world_state& ws, date_tag target_date);
}

