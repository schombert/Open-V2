#include "common\\common.h"
#include "trade_window.hpp"

namespace economy {
	trade_window::trade_window() : win(std::make_unique<trade_window_t>()) {}
	trade_window::~trade_window() {}
	void trade_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void trade_window::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_trade"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void trade_window::update(ui::gui_manager & gui_m) {
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void trade_window::show(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m);
	}
	void trade_window::show(ui::gui_manager & gui_m, goods_tag t) {
		selected_good = t;

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m);
	}
	void tw_close_button::button_function(ui::simple_button<tw_close_button>&, world_state & ws) {
		ws.w.trade_w.hide(ws.w.gui_m);
	}
	void tw_good_item_background::button_function(ui::simple_button<tw_good_item_background>& self, world_state & ws) {
		if(is_valid_index(tag)) {
			ws.w.trade_w.show(ws.w.gui_m, tag);
			ws.w.map_view.selected_good = tag;
			ws.w.map_view.mode = current_state::map_mode::prices;
			ws.w.map_view.changed = true;
		}
	}
	void tw_good_item_background::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(tag)) {
			if(auto cap = player->current_capital; is_valid_index(cap)) {
				if(auto cap_state = ws.w.province_s.province_state_container[cap].state_instance; cap_state) {
					ui::xy_pair cursor{ 0,0 };
					{
						auto v = economy::state_current_production(ws, *cap_state);
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::supply_label], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);

						char16_t local_buffer[16];
						put_value_in_buffer(local_buffer, display_type::fp_two_places, v[to_index(tag)]);
						cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);
						
						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
					}
					{
						auto v = economy::state_current_consumption(ws, *cap_state);
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::demand_label], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);

						char16_t local_buffer[16];
						put_value_in_buffer(local_buffer, display_type::fp_two_places, v[to_index(tag)]);
						cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);

						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
					}
					auto n_range = get_range(ws.w.nation_s.state_neighbor_arrays, cap_state->neighbors);
					if(n_range.first != n_range.second) {
						
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::imports], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
						cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u":"), tw, cursor, ui::tooltip_text_format);
						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);

						for(auto i = n_range.first; i != n_range.second; ++i) {
							if(auto nt = i->neighbor_tag; ws.w.nation_s.states.is_valid_index(nt)) {
								auto v = economy::state_current_imports(ws, *cap_state, nt);
								char16_t local_buffer[16];
								put_value_in_buffer(local_buffer, display_type::fp_two_places, v[to_index(tag)]);
								cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);
								cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
								cursor = ui::add_linear_text(cursor, ws.w.nation_s.states[nt].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
								cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
							}
						}

						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::exports], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
						cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u":"), tw, cursor, ui::tooltip_text_format);
						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);

						for(auto i = n_range.first; i != n_range.second; ++i) {
							if(auto nt = i->neighbor_tag; ws.w.nation_s.states.is_valid_index(nt)) {
								auto v = economy::state_current_exports(ws, *cap_state, nt);
								char16_t local_buffer[16];
								put_value_in_buffer(local_buffer, display_type::fp_two_places, v[to_index(tag)]);
								cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);
								cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
								cursor = ui::add_linear_text(cursor, ws.w.nation_s.states[nt].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
								cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
							}
						}
					}
					
				}
			}
		}
	}
}
