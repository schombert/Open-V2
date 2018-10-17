#pragma once
#include "common\\common.h"
#include "economy.h"
#include "gui\\gui.h"

class world_state;

namespace ui {
	class gui_manager;
}

namespace economy {
	class trade_window_t;

	class trade_window {
	public:
		std::unique_ptr<trade_window_t> win;

		goods_tag selected_good;
		nations::state_tag selected_state;

		trade_window();
		~trade_window();

		void hide(ui::gui_manager& gui_m);
		void init(world_state& ws);
		void update(ui::gui_manager& gui_m);
		void show(ui::gui_manager& gui_m);
		void show(ui::gui_manager& gui_m, goods_tag t);
	};
}
