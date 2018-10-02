#include "common\\common.h"
#include "economy_gui.hpp"

namespace economy {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.production_w.hide(ws.w.gui_m);
	}
	production_window::production_window() : win(std::make_unique<production_window_t>()) {}
	production_window::~production_window() {}
	void production_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void production_window::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_production"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void production_window::update(ui::gui_manager & gui_m) {
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void production_window::show(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void production_window::show_factories(ui::gui_manager & gui_m) {
		show(gui_m);
	}
	void production_window::show_projects(ui::gui_manager & gui_m) {
		show(gui_m);
	}
	void production_window::show_production(ui::gui_manager & gui_m) {
		show(gui_m);
	}
	void production_window::show_foreign_investment(ui::gui_manager & gui_m) {
		foreign_investment_nation = nations::country_tag();
		show(gui_m);
	}
	void production_window::show_particular_foreign_investment(ui::gui_manager & gui_m, nations::country_tag target) {
		foreign_investment_nation = target;
		show(gui_m);
	}
	void production_tab_button_group::on_select(world_state & ws, uint32_t i) {
		if(i == 0)
			ws.w.production_w.show_factories(ws.w.gui_m);
		else if(i == 1)
			ws.w.production_w.show_foreign_investment(ws.w.gui_m);
		else if(i == 2)
			ws.w.production_w.show_production(ws.w.gui_m);
		else if(i == 3)
			ws.w.production_w.show_projects(ws.w.gui_m);
	}
	void empty_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
}
