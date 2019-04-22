#pragma once
#include "common\\common.h"
#include "gui\\gui.h"

namespace scenario {
	class scenario_manager;
}

class world_state;

namespace find_dialog {
	class find_window_t;

	class find_window {
	public:
		std::unique_ptr<find_window_t> win;

		find_window();
		~find_window();

		void hide_find_window(ui::gui_manager& gui_m);
		void update_find_window(ui::gui_manager& gui_m);
		void show_find_window(ui::gui_manager& gui_m);
		void init_find_window(world_state& ws);
	};

}
