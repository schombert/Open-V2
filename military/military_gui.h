#pragma once
#include "common\\common.h"
#include "military.h"
#include "gui\\gui.h"

class world_state;

namespace military {
	class fabricate_cb_window_t;

	class fabricate_cb_window {
	public:
		std::unique_ptr<fabricate_cb_window_t> win;
		nations::country_tag target;
		military::cb_type_tag selected_type;

		fabricate_cb_window();
		~fabricate_cb_window();

		void hide_fabricate_window(ui::gui_manager& gui_m);
		void update_fabricate_window(ui::gui_manager& gui_m);
		void show_fabricate_window(ui::gui_manager& gui_m, nations::country_tag t);
		void init_fabricate_window(world_state& ws);
	};
}
