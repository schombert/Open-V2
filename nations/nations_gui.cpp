#include "common\\common.h"
#include "nations_gui.hpp"

namespace nations {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.hide_diplomacy_window();
	}
	void diplomacy_tab_button_group::on_select(world_state & ws, uint32_t i) {
		if(i == 0) {
			ws.w.show_diplomacy_window_self();
		} else if(i == 1) {
			ws.w.show_diplomacy_window(military::war_tag());
		} else if(i == 2) {
			ws.w.show_diplomacy_window_cbs();
		} else if(i == 3) {
			ws.w.show_diplomacy_window_crisis();
		}
	}
	void attacker_strength::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void defender_strength::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void war_score_overlay::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
}
