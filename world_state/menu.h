#pragma once
#include "common\\common.h"
#include "gui\\gui.h"

namespace scenario {
	class scenario_manager;
}

class world_state;

namespace menu {
	class menu_window_t;
	class graphics_panel_t;
	class sound_panel_t;
	class controls_panel_t;

	class menu_window {
	public:
		std::unique_ptr<menu_window_t> win;

		std::unique_ptr<graphics_panel_t> graphics;
		std::unique_ptr<sound_panel_t> sound;
		std::unique_ptr<controls_panel_t> controls;

		menu_window();
		~menu_window();

		void hide_menu_window(world_state & ws);
		void update_menu_window(ui::gui_manager& gui_m);
		void show_menu_window(world_state & ws);
		void init_menu_window(world_state& ws);
	};

}
