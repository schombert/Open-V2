#pragma once
#include "common\\common.h"
#include "economy.h"

class world_state;

namespace ui {
	class gui_manager;
}

namespace economy {
	class production_window_t;

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

	class production_window {
	public:
		std::unique_ptr<production_window_t> win;

		nations::country_tag foreign_investment_nation;

		bool filter_north_america = true;
		bool filter_south_america = true;
		bool filter_europe = true;
		bool filter_africa = true;
		bool filter_asia = true;
		bool filter_oceania = true;
		country_sub_filter sub_filter = country_sub_filter::continent;

		country_sort sort_type = country_sort::none;

		production_window();
		~production_window();

		void hide(ui::gui_manager& gui_m);
		void init(world_state& ws);
		void update(ui::gui_manager& gui_m);
		void show(ui::gui_manager& gui_m);

		void show_factories(ui::gui_manager& gui_m);
		void show_projects(ui::gui_manager& gui_m);
		void show_production(ui::gui_manager& gui_m);
		void show_foreign_investment(ui::gui_manager& gui_m);
		void show_particular_foreign_investment(ui::gui_manager& gui_m, nations::country_tag target);

	};
}
