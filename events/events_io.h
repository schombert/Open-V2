#pragma once
#include "common\\common.h"
#include "events.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>

template<>
class serialization::serializer<events::event> : public serialization::memcpy_serializer<events::event> {};
template<>
class serialization::serializer<events::decision> : public serialization::memcpy_serializer<events::decision> {};

template<>
class serialization::serializer<events::event_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(events::event_manager& obj) {
		for(auto const& i_desc : obj.decision_container)
			obj.decisions_by_title_index.emplace(i_desc.title, i_desc.id);
	}

	static void serialize_object(std::byte* &output, events::event_manager const& obj) {
		serialize(output, obj.event_container);
		serialize(output, obj.decision_container);
		serialize(output, obj.country_events);
		serialize(output, obj.province_events);
		serialize(output, obj.on_yearly_pulse);
		serialize(output, obj.on_quarterly_pulse);
		serialize(output, obj.on_new_great_nation);
		serialize(output, obj.on_lost_great_nation);
		serialize(output, obj.on_election_tick);
		serialize(output, obj.on_colony_to_state);
		serialize(output, obj.on_battle_won);
		serialize(output, obj.on_battle_lost);
		serialize(output, obj.on_debtor_default);
		serialize(output, obj.on_debtor_default_small);
		serialize(output, obj.on_debtor_default_second);
		serialize(output, obj.on_civilize);
		serialize(output, obj.on_my_factories_nationalized);
		serialize(output, obj.on_crisis_declare_interest);
		serialize(output, obj.events_by_id);
	}
	static void deserialize_object(std::byte const* &input, events::event_manager& obj) {
		deserialize(input, obj.event_container);
		deserialize(input, obj.decision_container);
		deserialize(input, obj.country_events);
		deserialize(input, obj.province_events);
		deserialize(input, obj.on_yearly_pulse);
		deserialize(input, obj.on_quarterly_pulse);
		deserialize(input, obj.on_new_great_nation);
		deserialize(input, obj.on_lost_great_nation);
		deserialize(input, obj.on_election_tick);
		deserialize(input, obj.on_colony_to_state);
		deserialize(input, obj.on_battle_won);
		deserialize(input, obj.on_battle_lost);
		deserialize(input, obj.on_debtor_default);
		deserialize(input, obj.on_debtor_default_small);
		deserialize(input, obj.on_debtor_default_second);
		deserialize(input, obj.on_civilize);
		deserialize(input, obj.on_my_factories_nationalized);
		deserialize(input, obj.on_crisis_declare_interest);
		deserialize(input, obj.events_by_id);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, events::event_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.event_container);
		deserialize(input, obj.decision_container);
		deserialize(input, obj.country_events);
		deserialize(input, obj.province_events);
		deserialize(input, obj.on_yearly_pulse);
		deserialize(input, obj.on_quarterly_pulse);
		deserialize(input, obj.on_new_great_nation);
		deserialize(input, obj.on_lost_great_nation);
		deserialize(input, obj.on_election_tick);
		deserialize(input, obj.on_colony_to_state);
		deserialize(input, obj.on_battle_won);
		deserialize(input, obj.on_battle_lost);
		deserialize(input, obj.on_debtor_default);
		deserialize(input, obj.on_debtor_default_small);
		deserialize(input, obj.on_debtor_default_second);
		deserialize(input, obj.on_civilize);
		deserialize(input, obj.on_my_factories_nationalized);
		deserialize(input, obj.on_crisis_declare_interest);
		deserialize(input, obj.events_by_id);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(events::event_manager const& obj) {
		return serialize_size(obj.event_container) +
			serialize_size(obj.decision_container) +
			serialize_size(obj.country_events) +
			serialize_size(obj.province_events) +
			serialize_size(obj.on_yearly_pulse) +
			serialize_size(obj.on_quarterly_pulse) +
			serialize_size(obj.on_new_great_nation) +
			serialize_size(obj.on_lost_great_nation) +
			serialize_size(obj.on_election_tick) +
			serialize_size(obj.on_colony_to_state) +
			serialize_size(obj.on_battle_won) +
			serialize_size(obj.on_battle_lost) +
			serialize_size(obj.on_debtor_default) +
			serialize_size(obj.on_debtor_default_small) +
			serialize_size(obj.on_debtor_default_second) +
			serialize_size(obj.on_civilize) +
			serialize_size(obj.on_my_factories_nationalized) +
			serialize_size(	obj.on_crisis_declare_interest) +
			serialize_size(obj.events_by_id);
	}
};


template<>
class serialization::serializer<events::event_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, events::event_state const& obj, world_state const& ws) {
		serialize_array(output, obj.country_event_has_fired.array(), obj.country_event_has_fired.end() - obj.country_event_has_fired.begin());
		serialize_array(output, obj.province_event_has_fired.array(), obj.province_event_has_fired.end() - obj.province_event_has_fired.begin());
	}
	static void deserialize_object(std::byte const* &input, events::event_state& obj, world_state& ws) {
		deserialize_array(input, obj.country_event_has_fired.array(), obj.country_event_has_fired.end() - obj.country_event_has_fired.begin());
		deserialize_array(input, obj.province_event_has_fired.array(), obj.province_event_has_fired.end() - obj.province_event_has_fired.begin());
	}

	static size_t size(events::event_state const& obj, world_state const& ws) {
		return (obj.country_event_has_fired.end() - obj.country_event_has_fired.begin()) +
			(obj.province_event_has_fired.end() - obj.province_event_has_fired.begin());
	}
};

namespace events {
	struct id_scope_pair {
		int32_t id;
		triggers::trigger_scope_state scope;

		bool operator<(const id_scope_pair& other) const {
			const int64_t this_value = (int64_t(id) << 32) | int64_t(scope.to_integer());
			const int64_t other_value = (int64_t(other.id) << 32) | int64_t(other.scope.to_integer());

			return this_value < other_value;
		}
	};

	struct token_group_range {
		const token_group* start;
		const token_group* end;
	};

	struct event_creation_manager {
		boost::container::flat_map<int32_t, token_group_range> event_sources;
		boost::container::flat_map<id_scope_pair, event_tag> pending_triggered_events;
		boost::container::flat_map<id_scope_pair, event_tag> created_triggered_events;
		std::vector<parsed_data> event_files_data;

		event_tag register_triggered_event(event_manager& m, int32_t event_id, triggers::trigger_scope_state scope);
	};

	std::pair<int32_t, bool> pre_parse_event(const token_group* start, const token_group* end); // returns id &  bool is triggered
	token_and_type get_issue_group_for_event(const token_group* start, const token_group* end);
	event_tag read_single_event(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const triggers::trigger_scope_state& scope,
		const token_group* start, const token_group* end);
	event_tag read_or_defer_event(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const triggers::trigger_scope_state& scope,
		const token_group* start, const token_group* end);
	void read_event_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end);
	void read_event_files(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& source_directory);
	void commit_pending_triggered_events(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root);
	void read_on_actions_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& source_directory);
	decision_tag read_decision(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end);
	void read_decision_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end);
	void read_decision_files(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& root);
}
