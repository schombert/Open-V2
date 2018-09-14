#include "events.h"

namespace events {

#ifdef __llvm__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

	void fire_event(world_state& ws, event_tag e, event_slot_content primary_contents, event_slot_content from) {

	}
	void fire_delayed_event(world_state& ws, event_tag e, int32_t delay_days, event_slot_content primary_contents, event_slot_content from) {

	}
	void fire_event_from_list(world_state& ws, std::vector<std::pair<event_tag, uint16_t>> const& event_list, event_slot_content primary_contents, event_slot_content from) {

	}

#ifdef __llvm__
#pragma clang diagnostic pop
#endif
}
