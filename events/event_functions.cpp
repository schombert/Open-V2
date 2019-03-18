#include "common\\common.h"
#include "event_functions.h"
#include "world_state\\world_state.h"

namespace events {
	void init_events_state(world_state& ws) {
		ws.w.event_s.country_event_has_fired.resize(ws.s.event_m.country_events.size());
		ws.w.event_s.province_event_has_fired.resize(ws.s.event_m.province_events.size());
	}
	void reset_state(event_state& s) {
		std::fill(s.country_event_has_fired.begin(), s.country_event_has_fired.end(), bitfield_type{});
		std::fill(s.province_event_has_fired.begin(), s.province_event_has_fired.end(), bitfield_type{});
	}

	int32_t wait_for_player_choice(world_state& ws) {
		std::unique_lock<std::mutex> lk(ws.w.local_player_data.player_choice_guard);

		ws.w.local_player_data.player_choice_condition.wait(lk, [&ws] {
			return ws.w.local_player_data.player_chosen_option.load(std::memory_order_acquire) != -1;
		});

		auto const result = ws.w.local_player_data.player_chosen_option.load(std::memory_order_acquire);
		ws.w.local_player_data.player_chosen_option.store(-1, std::memory_order_release);

		return result;
	}
	void make_player_choice(world_state& ws, int32_t option) {
		{
			std::lock_guard<std::mutex> lk(ws.w.local_player_data.player_choice_guard);
			ws.w.local_player_data.player_chosen_option.store(option, std::memory_order_release);
		}

		ws.w.local_player_data.player_choice_condition.notify_one();
	}
}