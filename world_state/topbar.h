#pragma once
#include "common\\common.h"
#include "gui\\gui.h"

class world_state;

namespace current_state {
	class topbar_t;

	class topbar {
	public:
		std::unique_ptr<topbar_t> win;

		topbar();
		~topbar();
		void init_topbar(world_state& ws);
		void update_topbar(ui::gui_manager& gui_m);
		void resize_topbar(ui::gui_manager& gui_m);
	};

}
