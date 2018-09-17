#include "events.h"
#include "triggers\\effects.h"
#include "world_state\\world_state.h"

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

	void execute_decision_set(std::vector<std::pair<nations::country_tag, events::decision_tag>>const& decision_set, world_state& ws) {
		for(auto& p : decision_set) {
			auto& gen = get_local_generator();
			triggers::execute_effect(ws.s.trigger_m.effect_data.data() + to_index(ws.s.event_m.decision_container[p.second].effect), ws, &(ws.w.nation_s.nations[p.first]), &(ws.w.nation_s.nations[p.first]), nullptr, nullptr, gen);
			gen.advance_n<8>();
		}
	}
}
