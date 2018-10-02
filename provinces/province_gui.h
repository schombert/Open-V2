#pragma once
#include "common\\common.h"
#include "provinces.h"

class world_state;

namespace ui {
	class gui_manager;
}

namespace provinces {
	class province_window_t;

	class province_window {
	public:
		std::unique_ptr<province_window_t> win;
		provinces::province_tag selected_province;

		province_window();
		~province_window();
		void hide_province_window(ui::gui_manager& gui_m);
		void show_province_window(ui::gui_manager& gui_m, provinces::province_tag p);
		void init_province_window(world_state& ws);
	};

}
