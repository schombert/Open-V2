#include "common\\common.h"
#include "bottombar.hpp"

namespace current_state {
	void close_log_button::button_function(ui::simple_button<close_log_button>&, world_state & ws) {
		ui::hide(*ws.w.bottombar_w.win->get<CT_STRING("messagelog_window")>().associated_object);
		ws.w.bottombar_w.log_is_open = false;
		ws.w.bottombar_w.update_bottombar(ws.w.gui_m);
	}
	void open_log_button::button_function(ui::simple_button<open_log_button>& self, world_state & ws) {
		ui::make_visible_and_update(ws.w.gui_m, *ws.w.bottombar_w.win->get<CT_STRING("messagelog_window")>().associated_object);
		ui::hide(*self.associated_object);
		ws.w.bottombar_w.log_is_open = true;
	}
	void menu_button::button_function(ui::simple_button<menu_button>& self, world_state & ws) {
		ws.w.menu_w.show_menu_window(ws);
	}
	void ledger_button::button_function(ui::simple_button<ledger_button>& self, world_state & ws) {}
	void goto_button::button_function(ui::simple_button<goto_button>& self, world_state & ws) {
		ws.w.find_w.show_find_window(ws);
	}
	void goto_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::find_label], ui::tooltip_text_format, ws, tw);
	}
	void zoom_in_button::button_function(ui::simple_button<zoom_in_button>& self, world_state & ws) {
		ws.w.map.rescale_by(2.0f);
	}
	void zoom_out_button::button_function(ui::simple_button<zoom_out_button>& self, world_state & ws) {
		ws.w.map.rescale_by(0.5f);
	}
	bool mini_map::on_lclick(ui::gui_object_tag o, world_state & ws, const ui::lbutton_down& l) {
		Eigen::Vector3f target = graphics::globe_point_from_position(float(l.x) / 266.0f, float(l.y + 9) / 133.0f, 1.57f, -1.57f);
		Eigen::Vector3f destination = ws.w.map.get_unrotated_vector_for(
			std::pair<float, float>(0.0f, 0.0f));
		ws.w.map.move_vector_to(target, destination);
		ws.w.bottombar_w.update_location(ws);
		return true;
	}
	void bottombar::update_location(world_state& ws) {
		auto const map_center = ws.w.map.map_coordinates_from_screen(std::pair<float, float>(0.0f,0.0f));
		auto new_location = ui::xy_pair{ int16_t(map_center.first / float(ws.s.province_m.province_map_width) * 266.0f) - 15,
				int16_t((map_center.second / float(ws.s.province_m.province_map_height)) * 133.0f - 9.0f - 15) };
		if(ws.w.bottombar_w.win->mmap.location.associated_object)
			ws.w.bottombar_w.win->mmap.location.associated_object->position = new_location;
	}
	ui::window_tag log_items_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_logtext"]);
	}
	bottombar::bottombar() : win(std::make_unique<bottombar_t>()) {}
	bottombar::~bottombar() {}
	void bottombar::init_bottombar(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["menubar"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void bottombar::update_bottombar(ui::gui_manager & gui_m) {
		if(win && win->associated_object)
			ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void message_settings_button::button_function(ui::simple_button<message_settings_button>& self, world_state & ws) {
		ws.w.message_settings_w.show_message_settings_window(ws.w.gui_m);
	}
	void message_settings_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::message_settings], ui::tooltip_text_format, ws, tw);
	}
}
