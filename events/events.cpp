#include "events.h"

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
}
