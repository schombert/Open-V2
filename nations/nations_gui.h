#pragma once
#include "common\\common.h"
#include "nations.h"
#include "gui\\gui.h"

class world_state;

namespace nations {
	class diplomacy_window_t;

	enum class country_sub_filter : uint8_t {
		continent,
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

		bool filter_north_america = true;
		bool filter_south_america = true;
		bool filter_europe = true;
		bool filter_africa = true;
		bool filter_asia = true;
		bool filter_oceania = true;
		country_sub_filter sub_filter = country_sub_filter::continent;

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
