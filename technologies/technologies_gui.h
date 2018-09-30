#pragma once
#include "common\\common.h"
#include "technologies.h"
#include "gui\\gui.h"

class world_state;

namespace technologies {
	class tech_window_t;

	enum class invention_sort : uint8_t {
		none, type, name, chance
	};

	class technology_window {
	public:
		std::unique_ptr< tech_window_t> win;
		invention_sort techui_invention_sort = invention_sort::none;
		technologies::tech_tag selected_technology;
		technologies::tech_category_tag selected_tech_category = technologies::tech_category_tag(0);

		technology_window();
		~technology_window();

		void show_technology_window(ui::gui_manager& gui_m);
		void hide_technology_window(ui::gui_manager& gui_m);
		void init_technology_winodw(world_state& ws);
	};

	ui::xy_pair eplain_technology(tech_tag t, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm, ui::text_format const& fmt);

}
