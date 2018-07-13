#pragma once
#include "province_gui.h"
#include "nations\\nations_functions.h"

namespace provinces {
	template<typename window_type>
	void slave_state_icon::windowed_update(ui::dynamic_icon<slave_state_icon>& ico, window_type const&, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;
			if(state) {
				if(state->flags & nations::state_instance::is_slave_state)
					ui::make_visible_immediate(*ico.associated_object);
				else
					ico.set_visibility(ws.w.gui_m, false);
				return;
			}
		}
		ico.set_visibility(ws.w.gui_m, false);
	}

	template<typename window_type>
	void colony_button::windowed_update(ui::simple_button<colony_button>& ico, window_type const&, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;

			if(state) {
				if(ws.w.province_s.province_state_container[selected_prov].owner != ws.w.local_player_nation || ws.w.local_player_nation == nullptr) {
					ico.set_enabled(false);
					if(state->flags & nations::state_instance::is_protectorate) {
						ico.set_frame(ws.w.gui_m, 0ui32);
						ui::make_visible_immediate(*ico.associated_object);
					} else if(state->flags & nations::state_instance::is_colonial) {
						ico.set_frame(ws.w.gui_m, 1ui32);
						ui::make_visible_immediate(*ico.associated_object);
					} else {
						ico.set_visibility(ws.w.gui_m, false);
					}
				} else {
					int32_t free_points = nations::free_colonial_points(ws, *ws.w.local_player_nation);
					if(state->flags & nations::state_instance::is_protectorate) {
						ico.set_frame(ws.w.gui_m, 0ui32);
						ico.set_enabled(nations::colonial_points_to_make_colony(ws, *state) <= free_points);
						ui::make_visible_immediate(*ico.associated_object);
					} else if(state->flags & nations::state_instance::is_colonial) {
						ico.set_frame(ws.w.gui_m, 1ui32);
						ico.set_enabled(nations::colonial_points_to_make_state(ws, *state) <= free_points);
						ui::make_visible_immediate(*ico.associated_object);
					} else {
						ico.set_visibility(ws.w.gui_m, false);
					}
				}
				return;
			}
		}
		ico.set_visibility(ws.w.gui_m, false);
	}
}
