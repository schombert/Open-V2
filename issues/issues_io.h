#pragma once
#include "common\\common.h"
#include "issues.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include <ppl.h>

template<>
class serialization::serializer<issues::issue_option> : public serialization::memcpy_serializer<issues::issue_option> {};
template<>
class serialization::serializer<issues::issue> : public serialization::memcpy_serializer<issues::issue> {};

template<>
class serialization::serializer<issues::issues_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(issues::issues_manager& obj) {
		for(auto const& i_issue : obj.issues_container)
			obj.named_issue_index.emplace(i_issue.name, i_issue.id);
		for(auto const& i_option : obj.options)
			obj.named_option_index.emplace(i_option.name, i_option.id);
	}

	static void serialize_object(std::byte* &output, issues::issues_manager const& obj) {
		serialize(output, obj.issues_container);
		serialize(output, obj.options);
		serialize(output, obj.party_issues);
		serialize(output, obj.political_issues);
		serialize(output, obj.social_issues);
		serialize(output, obj.economic_issues);
		serialize(output, obj.military_issues);
		serialize(output, obj.jingoism);
	}
	static void deserialize_object(std::byte const* &input, issues::issues_manager& obj) {
		deserialize(input, obj.issues_container);
		deserialize(input, obj.options);
		deserialize(input, obj.party_issues);
		deserialize(input, obj.political_issues);
		deserialize(input, obj.social_issues);
		deserialize(input, obj.economic_issues);
		deserialize(input, obj.military_issues);
		deserialize(input, obj.jingoism);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, issues::issues_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.issues_container);
		deserialize(input, obj.options);
		deserialize(input, obj.party_issues);
		deserialize(input, obj.political_issues);
		deserialize(input, obj.social_issues);
		deserialize(input, obj.economic_issues);
		deserialize(input, obj.military_issues);
		deserialize(input, obj.jingoism);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(issues::issues_manager const& obj) {
		return serialize_size(obj.issues_container) +
			serialize_size(obj.options) +
			serialize_size(obj.party_issues) +
			serialize_size(obj.political_issues) +
			serialize_size(obj.social_issues) +
			serialize_size(obj.economic_issues) +
			serialize_size(obj.military_issues) +
			serialize_size(obj.jingoism);
	}
};

namespace issues {
	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, issues_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	parsing_state pre_parse_issues(
		issues_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function);

	rules_set read_rules(const token_group* start, const token_group* end);
	void read_issue_options(
		const parsing_state& ps,
		scenario::scenario_manager& s,
		events::event_creation_manager& ecm);
}
