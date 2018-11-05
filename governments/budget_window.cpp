#include "common\\common.h"
#include "budget_window.hpp"

namespace governments {
	budget_window::budget_window() : win(std::make_unique<budget_window_t>()) {}
	budget_window::~budget_window() {}
	void budget_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void budget_window::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_budget"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void budget_window::update(ui::gui_manager & gui_m) {
		if((win->associated_object->flags.load(std::memory_order_acquire) & (ui::gui_object::visible | ui::gui_object::visible_after_update)) != 0)
			ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void budget_window::show(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m);
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void bw_close_button::button_function(ui::simple_button<bw_close_button>&, world_state & ws) {
		ws.w.budget_w.hide(ws.w.gui_m);
	}
	void hidden_button::on_create(ui::simple_button<hidden_button>& b, world_state & ws) {
		b.set_visibility(ws.w.gui_m, false);
	}
	void hidden_text::on_create(ui::fixed_text<hidden_text>& b, world_state &) {
		ui::hide(*b.associated_object);
	}
}
