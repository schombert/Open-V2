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
	void tw_good_item_background::button_function(ui::simple_button<tw_good_item_background>& self, world_state & ws) {}
	void tw_good_item_background::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tag)) {
			ws.w.trade_w.show(ws.w.gui_m, tag);
		}
	}
}
