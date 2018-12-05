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

template<>
class serialization::serializer<population::pop_movement> {
public:
	static constexpr bool has_static_size = true;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, population::pop_movement const& obj);
	static void deserialize_object(std::byte const* &input, population::pop_movement& obj);
	static size_t size(population::pop_movement const& obj);
	static size_t size();
};

template<>
class serialization::serializer<population::rebel_faction> {
public:
	static constexpr bool has_static_size = true;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, population::rebel_faction const& obj);
	static void deserialize_object(std::byte const* &input, population::rebel_faction& obj, world_state& ws);
	static size_t size(population::rebel_faction const& obj);
	static size_t size();
};

template<>
class serialization::serializer<population::population_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, population::population_state const& obj, world_state const&);
	static void deserialize_object(std::byte const* &input, population::population_state& obj, world_state& ws);
	static size_t size(population::population_state const& obj, world_state const&);
};

template<>
class serialization::serializer<population::population_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(population::population_manager& obj) {
		obj.factory_workers.clear();

		for(auto const& i_pop : obj.pop_types) {
			obj.named_pop_type_index.emplace(i_pop.name, i_pop.id);
			if((i_pop.flags & population::pop_type::factory_worker) != 0)
				obj.factory_workers.push_back(i_pop.id);
		}
		for(auto const& i_rebel : obj.rebel_types)
			obj.named_rebel_type_index.emplace(i_rebel.name, i_rebel.id);
		obj.count_poptypes = static_cast<uint32_t>(obj.pop_types.size());
	}

	static void serialize_object(std::byte* &output, population::population_manager const& obj) {
		serialize(output, obj.pop_types);
		serialize(output, obj.rebel_types);
		serialize(output, obj.life_needs);
		serialize(output, obj.everyday_needs);
		serialize(output, obj.luxury_needs);
		serialize(output, obj.rebel_units);
		serialize(output, obj.issue_inclination);
		serialize(output, obj.ideological_inclination);
		serialize(output, obj.promote_to);
		serialize(output, obj.demote_to);
		serialize(output, obj.rebel_change_government_to);
		serialize(output, obj.promotion_chance);
		serialize(output, obj.demotion_chance);
		serialize(output, obj.migration_chance);
		serialize(output, obj.colonialmigration_chance);
		serialize(output, obj.emigration_chance);
		serialize(output, obj.assimilation_chance);
		serialize(output, obj.conversion_chance);
		serialize(output, obj.clergy_pay);
		serialize(output, obj.bureaucrat_pay);
		serialize(output, obj.soldier_pay);
		serialize(output, obj.officer_pay);
		serialize(output, obj.officer_leadership);
		serialize(output, obj.artisan);
		serialize(output, obj.capitalist);
		serialize(output, obj.clergy);
		serialize(output, obj.bureaucrat);
		serialize(output, obj.slave);
		serialize(output, obj.soldier);
		serialize(output, obj.officer);
	}
	static void deserialize_object(std::byte const* &input, population::population_manager& obj) {
		deserialize(input, obj.pop_types);
		deserialize(input, obj.rebel_types);
		deserialize(input, obj.life_needs);
		deserialize(input, obj.everyday_needs);
		deserialize(input, obj.luxury_needs);
		deserialize(input, obj.rebel_units);
		deserialize(input, obj.issue_inclination);
		deserialize(input, obj.ideological_inclination);
		deserialize(input, obj.promote_to);
		deserialize(input, obj.demote_to);
		deserialize(input, obj.rebel_change_government_to);
		deserialize(input, obj.promotion_chance);
		deserialize(input, obj.demotion_chance);
		deserialize(input, obj.migration_chance);
		deserialize(input, obj.colonialmigration_chance);
		deserialize(input, obj.emigration_chance);
		deserialize(input, obj.assimilation_chance);
		deserialize(input, obj.conversion_chance);
		deserialize(input, obj.clergy_pay);
		deserialize(input, obj.bureaucrat_pay);
		deserialize(input, obj.soldier_pay);
		deserialize(input, obj.officer_pay);
		deserialize(input, obj.officer_leadership);
		deserialize(input, obj.artisan);
		deserialize(input, obj.capitalist);
		deserialize(input, obj.clergy);
		deserialize(input, obj.bureaucrat);
		deserialize(input, obj.slave);
		deserialize(input, obj.soldier);
		deserialize(input, obj.officer);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, population::population_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.pop_types);
		deserialize(input, obj.rebel_types);
		deserialize(input, obj.life_needs);
		deserialize(input, obj.everyday_needs);
		deserialize(input, obj.luxury_needs);
		deserialize(input, obj.rebel_units);
		deserialize(input, obj.issue_inclination);
		deserialize(input, obj.ideological_inclination);
		deserialize(input, obj.promote_to);
		deserialize(input, obj.demote_to);
		deserialize(input, obj.rebel_change_government_to);
		deserialize(input, obj.promotion_chance);
		deserialize(input, obj.demotion_chance);
		deserialize(input, obj.migration_chance);
		deserialize(input, obj.colonialmigration_chance);
		deserialize(input, obj.emigration_chance);
		deserialize(input, obj.assimilation_chance);
		deserialize(input, obj.conversion_chance);
		deserialize(input, obj.clergy_pay);
		deserialize(input, obj.bureaucrat_pay);
		deserialize(input, obj.soldier_pay);
		deserialize(input, obj.officer_pay);
		deserialize(input, obj.officer_leadership);
		deserialize(input, obj.artisan);
		deserialize(input, obj.capitalist);
		deserialize(input, obj.clergy);
		deserialize(input, obj.bureaucrat);
		deserialize(input, obj.slave);
		deserialize(input, obj.soldier);
		deserialize(input, obj.officer);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(population::population_manager const& obj) {
		return 
			serialize_size(obj.pop_types) +
			serialize_size(obj.rebel_types) +
			serialize_size(obj.life_needs) +
			serialize_size(obj.everyday_needs) +
			serialize_size(obj.luxury_needs) +
			serialize_size(obj.rebel_units) +
			serialize_size(obj.issue_inclination) +
			serialize_size(obj.ideological_inclination) +
			serialize_size(obj.promote_to) +
			serialize_size(obj.demote_to) +
			serialize_size(obj.rebel_change_government_to) +
			serialize_size(obj.promotion_chance) +
			serialize_size(obj.demotion_chance) +
			serialize_size(obj.migration_chance) +
			serialize_size(obj.colonialmigration_chance) +
			serialize_size(obj.emigration_chance) +
			serialize_size(obj.assimilation_chance) +
			serialize_size(obj.conversion_chance) +
			serialize_size(obj.clergy_pay) +
			serialize_size(obj.bureaucrat_pay) +
			serialize_size(obj.soldier_pay) +
			serialize_size(obj.officer_pay) +
			serialize_size(obj.officer_leadership) +
			serialize_size(obj.artisan) +
			serialize_size(obj.capitalist) +
			serialize_size(obj.clergy) +
			serialize_size(obj.bureaucrat) +
			serialize_size(obj.slave) +
			serialize_size(obj.soldier) +
			serialize_size(obj.officer);
	}
};

namespace population {
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
