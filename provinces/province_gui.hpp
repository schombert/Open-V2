#pragma once
#include "province_gui.h"
#include "nations\\nations_functions.h"

namespace provinces {

	template<typename lb_type>
	void modifier_lb::populate_list(lb_type& lb, world_state& ws) {
		auto selected = ws.w.selected_province;
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
		auto selected = ws.w.selected_province;
		if(is_valid_index(selected))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container[selected].name, fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void province_state_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected = ws.w.selected_province;
		if(is_valid_index(selected) && is_valid_index(ws.s.province_m.province_container[selected].state_id))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.province_m.state_names[ws.s.province_m.province_container[selected].state_id], fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void crime_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.selected_province;
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
		auto selected_prov = ws.w.selected_province;
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
		auto selected_prov = ws.w.selected_province;
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
		auto selected_prov = ws.w.selected_province;
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
		auto selected_prov = ws.w.selected_province;
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
		auto selected_prov = ws.w.selected_province;
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
		auto selected_prov = ws.w.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			self.set_fraction(ws.w.province_s.province_state_container[selected_prov].siege_progress);
			ui::make_visible_immediate(*self.associated_object);
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}


	template<typename window_type>
	void occupation_icon::windowed_update(ui::dynamic_icon<occupation_icon>& self, window_type&, world_state& ws) {
		auto selected_prov = ws.w.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			ui::make_visible_immediate(*self.associated_object);
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}

	template<typename window_type>
	void party_loyalty_icon::windowed_update(ui::dynamic_icon<party_loyalty_icon>& ico, window_type&, world_state& ws) {
		ico.set_visibility(ws.w.gui_m, false);
	}

	template<typename lb_type>
	void cores_lb::populate_list(lb_type& lb, world_state& ws) {
		auto selected = ws.w.selected_province;
		if(is_valid_index(selected)) {
			auto core_range = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[selected].cores);
			for(auto i = core_range.first; i != core_range.second; ++i)
				lb.add_item(ws, *i);
		}
	}

	template<typename window_type>
	void province_colony_base::windowed_update(window_type&, world_state& ws) {
		auto selected = ws.w.selected_province;
		if(is_valid_index(selected)) {
			if(ws.w.province_s.province_state_container[selected].owner == nullptr) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void province_statistics_base::windowed_update(window_type&, world_state& ws) {
		if(auto selected = ws.w.selected_province; is_valid_index(selected)) {
			if(ws.w.province_s.province_state_container[selected].owner == ws.w.local_player_nation) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void province_buildings_base::windowed_update(window_type&, world_state& ws) {
		if(auto selected = ws.w.selected_province; is_valid_index(selected)) {
			if(ws.w.province_s.province_state_container[selected].owner == ws.w.local_player_nation) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void province_other_base::windowed_update(window_type&, world_state& ws) {
		auto selected = ws.w.selected_province;
		if(is_valid_index(selected)) {
			auto owner = ws.w.province_s.province_state_container[selected].owner;
			if(owner != nullptr && owner != ws.w.local_player_nation) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void infrastructure_progress_base::windowed_update(window_type&, world_state& ws) {
		ui::hide(*associated_object);
	}
	
	template<typename lb_type>
	void colonist_lb::populate_list(lb_type& lb, world_state& ws) {
		auto selected = ws.w.selected_province;
		if(is_valid_index(selected)) {
			auto state = ws.w.province_s.province_state_container[selected].state_instance;
			if(state) {
				for(uint32_t i = 0; i < std::extent_v<decltype(state->colonizers)>; ++i) {
					if(state->colonizers[i].second != 0)
						lb.add_item(ws, state->colonizers[i].first, state->colonizers[i].second);
				}
			}
		}
	}

	template<typename lb_type>
	void stage_lb::populate_list(lb_type& lb, world_state& ws) {
		if(ws.w.local_player_nation == nullptr || ws.w.local_player_nation->id != colonizer) {
			for(int32_t i = 0; i < stage && i < 5; ++i) {
				lb.add_item(ws, i, -1);
			}
		} else {
			int32_t i = 0;
			for(; i < (stage - 1) && i < 4; ++i) {
				lb.add_item(ws, i, -1);
			}
			auto selected = ws.w.selected_province;
			if(is_valid_index(selected)) {
				auto state = ws.w.province_s.province_state_container[selected].state_instance;
				if(state)
					lb.add_item(ws, i, nations::points_for_next_colonial_stage(ws, *ws.w.local_player_nation, *state));
			}
		}
	}

	template<typename window_type>
	void stage_progress_icon::windowed_update(ui::dynamic_icon<stage_progress_icon>& ico, window_type& w, world_state& ws) {
		if(w.required_points == -1) {
			ico.set_frame(ws.w.gui_m, uint32_t(w.stage));
		} else {
			ico.set_visibility(ws.w.gui_m, false);
		}
	}

	template<typename window_type>
	void stage_progress_button::windowed_update(ui::simple_button<stage_progress_button>& ico, window_type& w, world_state& ws) {
		if(req_pts == -1) {
			ico.set_visibility(ws.w.gui_m, false);
		} else {
			ico.set_frame(ws.w.gui_m, uint32_t(w.stage));
			if(req_pts < nations::free_colonial_points(ws, *ws.w.local_player_nation)) {
				ico.set_enabled(false);
			} else {
				ico.set_enabled(true);
			}
		}
	}

	template<typename window_type>
	void progress_counter_text_box::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		
		if(win.stage >= 5) {
			char16_t formatted_value[64];
			put_value_in_buffer(formatted_value, display_type::integer, win.stage);

			ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(formatted_value),
				box,
				ui::xy_pair{ 0,0 },
				fmt,
				lm);

			lm.finish_current_line();
		}
	}
	
	template<typename lb_type>
	void colonist_lb::on_create(lb_type& lb, world_state&) {
		lb.associated_object->size = ui::xy_pair{ 375i16, 250i16 };
	}


	template<typename lb_type>
	void sphere_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto selected_prov = ws.w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				auto leader = owner->sphere_leader;
				if(leader == nullptr) {
					auto sphere_range = get_range(ws.w.nation_s.nations_arrays, owner->sphere_members);
					for(auto i : sphere_range) {
						if(is_valid_index(i))
							lb.add_item(ws, ws.w.nation_s.nations[i].tag);
					}
				} else
					lb.add_item(ws, leader->tag);
			}
		}
	}

	template<typename lb_type>
	void puppets_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto selected_prov = ws.w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				auto leader = owner->overlord;
				if(leader == nullptr) {
					auto vassal_range = get_range(ws.w.nation_s.nations_arrays, owner->vassals);
					for(auto i : vassal_range) {
						if(is_valid_index(i))
							lb.add_item(ws, ws.w.nation_s.nations[i].tag);
					}
				} else
					lb.add_item(ws, leader->tag);
			}
		}
	}

	template<typename lb_type>
	void allies_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto selected_prov = ws.w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				auto allies_range = get_range(ws.w.nation_s.nations_arrays, owner->allies);
				for(auto i : allies_range) {
					if(is_valid_index(i))
						lb.add_item(ws, ws.w.nation_s.nations[i].tag);
				}
			}
		}
	}

	template<typename lb_type>
	void war_lb::populate_list(lb_type& lb, world_state& ws) {
		//if(auto selected_prov = ws.w.selected_province; is_valid_index(selected_prov)) {
		//	if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				
		//	}
		//}
	}
	
	template<typename window_type>
	void fort_progress_bar::windowed_update(window_type& win, ui::progress_bar<fort_progress_bar>& self, world_state& ws) {
		ui::hide(*self.associated_object);
		ui::hide(*win.fort_expand_text.associated_object);
	}
	
	template<typename window_type>
	void naval_base_progress_bar::windowed_update(window_type&, ui::progress_bar<naval_base_progress_bar>& self, world_state& ws) {
		ui::hide(*self.associated_object);
		ui::hide(*win.naval_base_expand_text.associated_object);
	}
	
	template<typename window_type>
	void railroad_progress_bar::windowed_update(window_type&, ui::progress_bar<railroad_progress_bar>& self, world_state& ws) {
		ui::hide(*self.associated_object);
		ui::hide(*win.railroad_expand_text.associated_object);
	}


	template<typename window_type>
	void fort_expand_button::windowed_update(ui::simple_button<fort_expand_button>& self, window_type& w, world_state& ws) {}
	template<typename window_type>
	void naval_base_expand_button::windowed_update(ui::simple_button<naval_base_expand_button>& self, window_type& w, world_state& ws) {}
	template<typename window_type>
	void railroad_expand_button::windowed_update(ui::simple_button<railroad_expand_button>& self, window_type& w, world_state& ws) {}

	template<typename window_type>
	void province_buildings_base::on_create(window_type& win, world_state& ws) {
		associated_object->position += ui::xy_pair{ 0i16, -10i16 };
		associated_object->size += ui::xy_pair{ 0i16, 10i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, 10i16 };
		});

		const auto& window_definition = ws.s.gui_m.ui_definitions.windows[std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building"])];
		for(auto i = window_definition.sub_object_definitions.cbegin(); i != window_definition.sub_object_definitions.cend(); ++i) {
			if(std::holds_alternative<ui::button_tag>(*i)) {
				ui::create_static_element(ws, std::get<ui::button_tag>(*i), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_button);
				ui::create_static_element(ws, std::get<ui::button_tag>(*i), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_button);
				ui::create_static_element(ws, std::get<ui::button_tag>(*i), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_button);
				break;
			}
		}

		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["build_icon0"]), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_icon);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["build_icon1"]), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_icon);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["build_icon2"]), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_icon);

		ui::create_static_element(ws, std::get<ui::text_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["expand_text"]), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_expand_text);
		ui::create_static_element(ws, std::get<ui::text_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["expand_text"]), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_expand_text);
		ui::create_static_element(ws, std::get<ui::text_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["expand_text"]), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_expand_text);

		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building_progress"]), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_bar);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building_progress"]), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_bar);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building_progress"]), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_bar);

		ui::add_linear_text(ui::xy_pair{ 69ui16, 15ui16 }, ws.s.fixed_ui_text[scenario::fixed_ui::fort],
			ui::text_format{ ui::text_color::black, graphics::font_tag(2), 20 }, ws.s.gui_m, ws.w.gui_m,
			ui::tagged_gui_object{ *associated_object, win.window_object });

		ui::add_linear_text(ui::xy_pair{ 69ui16, 50ui16 }, ws.s.fixed_ui_text[scenario::fixed_ui::naval_base],
			ui::text_format{ ui::text_color::black, graphics::font_tag(2), 20 }, ws.s.gui_m, ws.w.gui_m,
			ui::tagged_gui_object{ *associated_object, win.window_object });

		ui::add_linear_text(ui::xy_pair{ 69ui16, 85ui16 }, ws.s.fixed_ui_text[scenario::fixed_ui::railroad],
			ui::text_format{ ui::text_color::black, graphics::font_tag(2), 20 }, ws.s.gui_m, ws.w.gui_m,
			ui::tagged_gui_object{ *associated_object, win.window_object });

		fort_icon.associated_object->position.y = 7i16;
		naval_base_icon.associated_object->position.y = 42i16;
		railroad_icon.associated_object->position.y = 77i16;

		fort_bar.associated_object->position.y = 15i16;
		naval_base_bar.associated_object->position.y = 50i16;
		railroad_bar.associated_object->position.y = 85i16;

		fort_expand_text.associated_object->position.y = 15i16;
		naval_base_expand_text.associated_object->position.y = 50i16;
		railroad_expand_text.associated_object->position.y = 85i16;

		fort_button.associated_object->position.y = 13i16;
		naval_base_button.associated_object->position.y = 48i16;
		railroad_button.associated_object->position.y = 83i16;
	}
}
