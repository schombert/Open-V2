#pragma once
#include "common\\common.h"
#include "events.h"

class world_state;

namespace events {
	void init_events_state(world_state& ws);
	void reset_state(event_state& s);

	int32_t wait_for_player_choice(world_state& ws);
	void make_player_choice(world_state& ws, int32_t option);

	void fire_event(world_state& ws, event_tag e, event_slot_content primary_contents, event_slot_content from);
	void fire_delayed_event(world_state& ws, event_tag e, int32_t delay_days, event_slot_content primary_contents, event_slot_content from);
	void fire_event_from_list(world_state& ws, std::vector<std::pair<event_tag, uint16_t>> const& event_list, event_slot_content primary_contents, event_slot_content from);

	void execute_decision_set(std::vector<std::pair<nations::country_tag, events::decision_tag>>const& decision_set, world_state& ws);

	void daily_update(world_state& ws);
}