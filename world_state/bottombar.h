#pragma once
#include "common\\common.h"
#include "gui\\gui.h"

class world_state;

namespace current_state {
	class bottombar_t;

	class bottombar {
	public:
		std::unique_ptr<bottombar_t> win;
		bool log_is_open = false;

		bottombar();
		~bottombar();
		void init_bottombar(world_state& ws);
		void update_bottombar(ui::gui_manager& gui_m);
	};
}