#pragma once
#include "common\\common.h"
#include "governments.h"

class world_state;

namespace ui {
	class gui_manager;
}

namespace governments {
	class government_window_t;

	class government_window {
	public:
		std::unique_ptr< government_window_t> win;

		government_window();
		~government_window();
		void init_government_window(world_state& ws);
		void hide_government_window(ui::gui_manager& gui_m);
		void show_government_reforms_tab(ui::gui_manager& gui_m);
		void show_government_movements_tab(ui::gui_manager& gui_m);
		void show_government_decisions_tab(ui::gui_manager& gui_m);
		void show_government_release_nations_tab(ui::gui_manager& gui_m);
	};


}
