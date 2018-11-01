#pragma once
#include "common\\common.h"
#include "governments.h"
#include "gui\\gui.h"

class world_state;

namespace ui {
	class gui_manager;
}

namespace governments {
	class budget_window_t;

	class budget_window {
	public:
		std::unique_ptr<budget_window_t> win;

		budget_window();
		~budget_window();

		void hide(ui::gui_manager& gui_m);
		void init(world_state& ws);
		void update(ui::gui_manager& gui_m);
		void show(ui::gui_manager& gui_m);
	};
}
