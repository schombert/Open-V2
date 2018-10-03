#pragma once
#include "common\\common.h"
#include "nations.h"

class world_state;

namespace ui {
	class gui_manager;
}

namespace nations {
	class diplomacy_window_t;

	enum class country_sub_filter : uint8_t {
		all,
		continent_asia,
		continent_africa,
		continent_oceania,
		continent_north_america,
		continent_south_america,
		continent_europe,
		neighbor,
		ally,
		enemy,
		sphere
	};

	enum class country_sort : uint8_t {
		none,
		name,
		priority,
		gp_one,
		gp_two,
		gp_three,
		gp_four,
		gp_five,
		gp_six,
		gp_seven,
		gp_eight,
		sphere_leader,
		prestige_rank,
		economic_rank,
		military_rank,
		overall_rank,
		opinion,
		relation
	};

	class diplomacy_window {
	public:
		std::unique_ptr<diplomacy_window_t> win;

		nations::country_tag selected_nation;
		bool goto_selected_pending = false;

		country_sub_filter filter = country_sub_filter::all;

		country_sort sort_type = country_sort::none;

		diplomacy_window();
		~diplomacy_window();

		void hide_diplomacy_window(ui::gui_manager& gui_m);
		void update_diplomacy_window(ui::gui_manager& gui_m);
		void show_diplomacy_window(ui::gui_manager& gui_m, country_tag t);
		void show_diplomacy_window_gp(ui::gui_manager& gui_m);
		void show_diplomacy_window_wars(ui::gui_manager& gui_m);
		void show_diplomacy_window_crisis(ui::gui_manager& gui_m);
		void show_diplomacy_window_cbs(ui::gui_manager& gui_m);
		void init_diplomacy_window(world_state& ws);

	};
}
