#include "common\\common.h"
#include "events_gui.hpp"

namespace events {

	province_event_window::province_event_window() : win(std::make_unique<province_event_window_t>()) {}
	province_event_window::~province_event_window() {}

	nation_event_window::nation_event_window() : win(std::make_unique<nation_event_window_t>()) {}
	nation_event_window::~nation_event_window() {}

	major_event_window::major_event_window() : win(std::make_unique<major_event_window_t>()) {}
	major_event_window::~major_event_window() {}

	void province_event_window::hide_province_event_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void province_event_window::show_province_event_window(ui::gui_manager& gui_m, pending_province_event e) {
		displayed_event = e;
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void province_event_window::init_province_event_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["Event_Province_Window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void nation_event_window::hide_nation_event_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void nation_event_window::show_nation_event_window(ui::gui_manager& gui_m, pending_nation_event e) {
		displayed_event = e;
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void nation_event_window::init_nation_event_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["Event_Country_Window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void major_event_window::hide_major_event_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void major_event_window::show_major_event_window(ui::gui_manager& gui_m, pending_nation_event e) {
		displayed_event = e;
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void major_event_window::init_major_event_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["Event_Major_Window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void hidden_image::update(ui::dynamic_icon<hidden_image>& self, world_state& ws) {
		ui::hide(*self.associated_object);
	}

	void province_event_title::update(
		ui::tagged_gui_object box,
		ui::text_box_line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {
		
		if(auto e = ws.w.province_event_w.displayed_event.e; e) {
			ui::add_linear_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].title,
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm);
		}

	}
}
