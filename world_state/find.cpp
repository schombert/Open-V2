#include "common\\common.h"
#include "world_state.h"
#include "find.hpp"
#include "graphics\\world_map.h"

namespace find_dialog {
	void find_close_button::button_function(ui::simple_button<find_close_button>&, world_state& ws) {
		ws.w.find_w.hide_find_window(ws.w.gui_m);
	}
	ui::window_tag find_item_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["openv2_find_entry"]);
	}
	void find_item_button::button_function(ui::simple_button<find_item_button>& self, world_state & ws) {
		if(std::holds_alternative<provinces::province_tag>(value)) {
			graphics::map_goto(ws, std::get<provinces::province_tag>(value));
			ws.w.province_w.show_province_window(ws.w.gui_m, std::get<provinces::province_tag>(value));
		} else if(std::holds_alternative<nations::country_tag>(value)) {
			graphics::map_goto(ws, std::get<nations::country_tag>(value));
		} else if(std::holds_alternative<nations::state_tag>(value)) {
			graphics::map_goto(ws, std::get<nations::state_tag>(value));
		}
		ws.w.find_w.hide_find_window(ws.w.gui_m);
	}
	find_window::find_window() : win(std::make_unique<find_window_t>()) {}
	find_window::~find_window() {}
	void find_window::hide_find_window(ui::gui_manager & gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void find_window::update_find_window(ui::gui_manager & gui_m) {
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void find_window::show_find_window(world_state & ws) {
		ui::move_to_front(ws.w.gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		win->get<CT_STRING("find_edit")>().clear();
		ui::set_focus(ws, ui::find_in_parent(ws.w.gui_m, *(win->get<CT_STRING("find_edit")>().associated_object)));
		ui::make_visible_and_update(ws.w.gui_m, *(win->associated_object));
	}
	void find_window::init_find_window(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["openv2_find_window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void find_box::on_edit(ui::edit_box<find_box>& self, world_state & ws) {
		ws.w.find_w.update_find_window(ws.w.gui_m);
	}
}