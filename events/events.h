#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "Parsers\\parsers.h"
#include "simple_fs\\simple_fs.h"
#include "graphics\\texture.h"

namespace scenario {
	class scenario_manager;
}

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

	struct event_option {
		text_data::text_tag name;
		modifiers::factor_tag ai_chance;
		triggers::effect_tag effect;
	};

	struct event {
		constexpr static uint8_t fire_only_once = 0x0001;
		constexpr static uint8_t is_major = 0x0002;

		event_option options[6];

		text_data::text_tag title;
		text_data::text_tag body;
		graphics::texture_tag picture;

		modifiers::factor_tag mean_time_to_happen;
		triggers::trigger_tag trigger;
		triggers::effect_tag immediate_effect;

		event_tag id;

		uint8_t flags = 0;
	};

	struct decision {
		text_data::text_tag title;
		text_data::text_tag body;
		graphics::texture_tag picture;

		triggers::effect_tag effect;
		triggers::trigger_tag potential;
		triggers::trigger_tag allow;
		modifiers::factor_tag ai_will_do;

		decision_tag id;
	};

	struct event_manager {
		tagged_vector<event, event_tag> event_container;
		tagged_vector<decision, decision_tag> decision_container;

		std::vector<event_tag> country_events;
		std::vector<event_tag> province_events;

		std::vector<std::pair<event_tag, uint16_t>> on_yearly_pulse;
		std::vector<std::pair<event_tag, uint16_t>> on_quarterly_pulse;
		std::vector<std::pair<event_tag, uint16_t>> on_new_great_nation;
		std::vector<std::pair<event_tag, uint16_t>> on_lost_great_nation;
		std::vector<std::tuple<event_tag, uint16_t, text_data::text_tag>> on_election_tick; // event + chance + issue group
		std::vector<std::pair<event_tag, uint16_t>> on_colony_to_state; // passes state
		std::vector<std::pair<event_tag, uint16_t>> on_state_conquest; // passes state
		std::vector<std::pair<event_tag, uint16_t>> on_colony_to_state_free_slaves; // passes state
		std::vector<std::pair<event_tag, uint16_t>> on_battle_won; // passes province
		std::vector<std::pair<event_tag, uint16_t>> on_battle_lost; // passes province
		std::vector<std::pair<event_tag, uint16_t>> on_debtor_default; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_debtor_default_small; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_debtor_default_second; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_civilize;
		std::vector<std::pair<event_tag, uint16_t>> on_my_factories_nationalized; // has from
		std::vector<std::pair<event_tag, uint16_t>> on_crisis_declare_interest;


		boost::container::flat_map<int32_t, event_tag> events_by_id;
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
	event_tag parse_single_event(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const triggers::trigger_scope_state& scope,
		const token_group* start, const token_group* end);
	event_tag parse_or_defer_event(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const triggers::trigger_scope_state& scope,
		const token_group* start, const token_group* end);
	void parse_event_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end);
	void parse_event_files(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& source_directory);
	void commit_pending_triggered_events(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root);
	void parse_on_actions_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& source_directory);
	decision_tag parse_decision(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end);
	void parse_decision_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end);
	void parse_decision_files(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& root);
}
