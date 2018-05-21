#include "events.h"
#include "object_parsing\\object_parsing.hpp"
#include "scenario\\scenario.h"

namespace events {
	event_tag event_creation_manager::register_triggered_event(event_manager& m, int32_t event_id, triggers::trigger_scope_state scope) {
		const auto created_find_result = created_triggered_events.find(id_scope_pair{ event_id, scope });
		if (created_find_result != created_triggered_events.end())
			return created_find_result->second;

		const auto pending_find_result = pending_triggered_events.find(id_scope_pair{ event_id, scope });
		if (pending_find_result != pending_triggered_events.end())
			return pending_find_result->second;

		const auto new_event_tag = m.event_container.emplace_back();
		pending_triggered_events.emplace(id_scope_pair{ event_id, scope }, new_event_tag);
		return new_event_tag;
	}

	event_tag parse_or_defer_event(scenario::scenario_manager& s, event_creation_manager& ecm, const triggers::trigger_scope_state& scope, const token_group* start, const token_group* end) {
		const auto[event_id, is_triggered] = pre_parse_event(start, end);
		ecm.event_sources.emplace(event_id, token_group_range{start, end});

		if (!is_triggered)
			return parse_single_event(s, ecm, scope, start, end);
		else
			return event_tag();
	}

	struct event_preparse {
		int32_t id = -1;
		bool is_triggered_only = false;
		void discard(int) {}
	};

#ifdef _DEBUG
	struct too_many_event_options {};
#endif

	struct event_reader {
		scenario::scenario_manager& env;
		event_reader(scenario::scenario_manager& e) : env(e) {}

		event under_construction;
		int32_t options_count = 0;

		void set_fire_only_once(bool v) {
			if(v)
				under_construction.flags |= event::file_only_once;
		}
		void set_major(bool v) {
			if(v)
				under_construction.flags |= event::is_major;
		}
		void set_tile(const token_and_type& t) {
			under_construction.title = text_data::get_thread_safe_text_handle(env.text_m, t.start, t.end);
		}
		void set_desc(const token_and_type& t) {
			under_construction.body = text_data::get_thread_safe_text_handle(env.text_m, t.start, t.end);
		}
		void set_mtth(const modifiers::factor_tag mtth) {
			under_construction.mean_time_to_happen = mtth;
		}
		void set_immediate(triggers::effect_tag e) {
			under_construction.immediate_effect = e;
		}
		void set_option(const event_option& opt) {
			if(options_count <= 5) {
				under_construction.options[options_count] = opt;
				++options_count;
			} else {
#ifdef _DEBUG
				throw too_many_event_options();
#endif
			}
		}

		void discard(int) {}
	};

	struct empty_type {
		void discard(int) {}
	};

	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }
}

MEMBER_FDEF(events::empty_type, discard, "unknown_key");
MEMBER_DEF(events::event_preparse, id, "id");
MEMBER_DEF(events::event_preparse, is_triggered_only, "is_triggered_only");
MEMBER_FDEF(events::event_preparse, discard, "discard");

namespace events {
	BEGIN_DOMAIN(single_event_preparsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(event_preparse)
		MEMBER_ASSOCIATION("id", "id", value_from_rh<int32_t>)
		MEMBER_ASSOCIATION("is_triggered_only", "is_triggered_only", value_from_rh<bool>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("discard", accept_all, empty_type, discard_empty_type)
		MEMBER_VARIABLE_ASSOCIATION("discard", accept_all, discard_from_full)
		END_TYPE
	END_DOMAIN;

	std::pair<int32_t, bool> pre_parse_event(const token_group* start, const token_group* end) { // returns id &  bool is triggered 
		const auto result = parse_object<event_preparse, single_event_preparsing_domain>(start, end);
		return std::pair<int32_t, bool>(result.id, result.is_triggered_only);
	}

	event_tag parse_single_event(scenario::scenario_manager&, event_creation_manager&, const triggers::trigger_scope_state&, const token_group*, const token_group*) {
		//TODO
		return event_tag();
	}
}
