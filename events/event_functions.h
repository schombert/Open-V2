#pragma once
#include "common\\common.h"
#include "events.h"

class world_state;

namespace events {
	void init_events_state(world_state& ws);
	void reset_state(event_state& s);

	int32_t wait_for_player_choice(world_state& ws);
	void make_player_choice(world_state& ws, int32_t option);
}