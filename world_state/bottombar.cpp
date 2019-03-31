#include "common\\common.h"
#include "bottombar.hpp"

namespace current_state {
	void hidden_button::update(ui::simple_button<hidden_button>& self, world_state & ws) {
		ui::hide(*self.associated_object);
	}
	void close_log_button::button_function(ui::simple_button<close_log_button>&, world_state & ws) {
		ui::hide(*ws.w.bottombar_w.win->get<CT_STRING("messagelog_window")>().associated_object);
		ws.w.bottombar_w.log_is_open = false;
	}
	void open_log_button::button_function(ui::simple_button<open_log_button>& self, world_state & ws) {
		ui::make_visible_and_update(ws.w.gui_m, *ws.w.bottombar_w.win->get<CT_STRING("messagelog_window")>().associated_object);
		ui::hide(*self.associated_object);
		ws.w.bottombar_w.log_is_open = true;
	}
	void menu_button::button_function(ui::simple_button<menu_button>& self, world_state & ws) {}
	void ledger_button::button_function(ui::simple_button<ledger_button>& self, world_state & ws) {}
	void goto_button::button_function(ui::simple_button<goto_button>& self, world_state & ws) {}
	void zoom_in_button::button_function(ui::simple_button<zoom_in_button>& self, world_state & ws) {
		ws.w.map.state.rescale_by(2.0f);
	}
	void zoom_out_button::button_function(ui::simple_button<zoom_out_button>& self, world_state & ws) {
		ws.w.map.state.rescale_by(0.5f);
	}
	bool mini_map::on_lclick(ui::gui_object_tag o, world_state & ws, const ui::lbutton_down& l) {
		Eigen::Vector3f target = graphics::globe_point_from_position(float(l.x) / 266.0f, float(l.y + 9) / 133.0f, 1.57f, -1.57f);
		Eigen::Vector3f destination = ws.w.map.state.get_unrotated_vector_for(
			std::pair<float, float>(0.0f, 0.0f));
		ws.w.map.state.move_vector_to(target, destination);
		return true;
	}
	ui::window_tag log_items_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["logtext"]);
	}
	bottombar::bottombar() : win(std::make_unique<bottombar_t>()) {}
	bottombar::~bottombar() {}
	void bottombar::init_bottombar(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["menubar"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void bottombar::update_bottombar(ui::gui_manager & gui_m) {
		if(win->associated_object)
			ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
}
