#pragma once
#include "common\\common.h"
#include "governments.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "simple_fs\\simple_fs.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include <ppl.h>

template<>
class serialization::serializer<governments::government_type> : public serialization::memcpy_serializer<governments::government_type> {};
template<>
class serialization::serializer<governments::party> : public serialization::memcpy_serializer<governments::party> {};

template<>
class serialization::serializer<governments::governments_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(governments::governments_manager& obj) {
		for(auto const& i_government : obj.governments_container)
			obj.named_government_index.emplace(i_government.name, i_government.id);
	}

	static void serialize_object(std::byte* &output, governments::governments_manager const& obj) {
		serialize(output, obj.governments_container);
		serialize(output, obj.permitted_ideologies);
		serialize(output, obj.parties);
		serialize(output, obj.party_issues);
	}
	static void deserialize_object(std::byte const* &input, governments::governments_manager& obj) {
		deserialize(input, obj.governments_container);
		deserialize(input, obj.permitted_ideologies);
		deserialize(input, obj.parties);
		deserialize(input, obj.party_issues);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, governments::governments_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.governments_container);
		deserialize(input, obj.permitted_ideologies);
		deserialize(input, obj.parties);
		deserialize(input, obj.party_issues);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(governments::governments_manager const& obj) {
		return serialize_size(obj.governments_container) +
			serialize_size(obj.permitted_ideologies) +
			serialize_size(obj.parties) +
			serialize_size(obj.party_issues);
	}
};

namespace governments {
	tagged_vector<std::string, governments::government_tag> read_governments(
		governments_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function,
		const ideologies::ideologies_manager& ideologies_source);
	void ready_party_issues(governments_manager& manager, issues::issues_manager& im);
	party_tag read_party(token_group const* start, token_group const* end, scenario::scenario_manager& s);
	void setup_party_rules(scenario::scenario_manager& s);
}
