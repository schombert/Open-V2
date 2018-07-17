#pragma once
#include "province_gui.h"
#include "nations\\nations_functions.h"

namespace provinces {

	template<typename lb_type>
	void modifier_lb::populate_list(lb_type& lb, world_state& ws) {
		auto selected = ws.w.province_window.selected_province;
		if(is_valid_index(selected)) {
			auto& pstate = ws.w.province_s.province_state_container[selected];
			auto static_mod_range = get_range(ws.w.province_s.static_modifier_arrays, pstate.static_modifiers);
			for(auto s = static_mod_range.first; s != static_mod_range.second; ++s) {
				lb.add_item(ws, ws.s.modifiers_m.provincial_modifiers[*s].icon, *s, date_tag());
			}
			auto timed_mod_range = get_range(ws.w.province_s.timed_modifier_arrays, pstate.timed_modifiers);
			for(auto s = timed_mod_range.first; s != timed_mod_range.second; ++s) {
				lb.add_item(ws, ws.s.modifiers_m.provincial_modifiers[s->mod].icon, s->mod, s->expiration);
			}
		}
	}

	template<typename window_type>
	void province_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected = ws.w.province_window.selected_province;
		if(is_valid_index(selected))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container[selected].name, fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void province_state_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected = ws.w.province_window.selected_province;
		if(is_valid_index(selected) && is_valid_index(ws.s.province_m.province_container[selected].state_id))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.province_m.state_names[ws.s.province_m.province_container[selected].state_id], fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void crime_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto crime = ws.w.province_s.province_state_container[selected_prov].crime;
			if(is_valid_index(crime)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.modifiers_m.provincial_modifiers[crime].name, fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
				lm.finish_current_line();
				ui::make_visible_immediate(obj.object);
				return;
			}
		}
		ui::hide(obj.object);
	}

	template<typename W>
	void province_controller_flag::windowed_update(ui::masked_flag<province_controller_flag>& self, W& w, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto controller = ws.w.province_s.province_state_container[selected_prov].controller;
			if(controller) {
				self.set_displayed_flag(ws, controller->tag);
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		self.set_visibility(ws.w.gui_m, false);
	}

	template<typename W>
	void occupation_flag::windowed_update(ui::masked_flag<occupation_flag>& self, W& w, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			auto sieging_nation = get_province_seiger(ws, ws.w.province_s.province_state_container[selected_prov]);
			if(is_valid_index(sieging_nation)) {
				self.set_displayed_flag(ws, ws.w.nation_s.nations.get(sieging_nation).tag);
				ui::make_visible_immediate(*self.associated_object);
			}
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}

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
	void crime_icon::windowed_update(ui::dynamic_icon<crime_icon>& ico, window_type const&, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto crime = ws.w.province_s.province_state_container[selected_prov].crime;
			if(is_valid_index(crime)) {
				ico.set_frame(ws.w.gui_m, ws.s.modifiers_m.provincial_modifiers[crime].icon);
				ui::make_visible_immediate(*ico.associated_object);
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

	template<typename W>
	void occupation_progress_bar::windowed_update(ui::progress_bar<occupation_progress_bar>& self, W&, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			self.set_fraction(ws.w.province_s.province_state_container[selected_prov].siege_progress);
			ui::make_visible_immediate(*self.associated_object);
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}


	template<typename window_type>
	void occupation_icon::windowed_update(ui::dynamic_icon<occupation_icon>& self, window_type&, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			ui::make_visible_immediate(*self.associated_object);
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}
}
