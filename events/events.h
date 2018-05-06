#pragma once
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "triggers\\triggers.h"

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
		text_data::text_tag title;
	};

	struct event {
		text_data::text_tag title;
		text_data::text_tag body;

		event_tag id;
	};

	struct event_manager {
		tagged_vector<event, event_tag> event_container;

		std::vector<std::pair<event_tag, triggers::trigger_tag>> country_events;
		std::vector<std::pair<event_tag, triggers::trigger_tag>> province_events;
	};

	struct token_group_range {
		const token_group* start;
		const token_group* end;
	};

	struct event_creation_manager {
		boost::container::flat_map<int32_t, token_group_range> event_sources;
		boost::container::flat_map<id_scope_pair, event_tag> pending_triggered_events;
		boost::container::flat_map<id_scope_pair, event_tag> created_triggered_events;

		event_tag register_triggered_event(event_manager& m, int32_t event_id, triggers::trigger_scope_state scope);
	};
}