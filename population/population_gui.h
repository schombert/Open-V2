#pragma once
#include "common\\common.h"
#include "population.h"
#include "gui\\gui.h"
#include "simple_mpl\\simple_mpl.hpp"

class world_state;

namespace population {

	enum class population_sort : uint8_t {
		none,
		size,
		type,
		culture,
		religion,
		location,
		militancy,
		consciousness,
		ideoology,
		issues,
		unemployment,
		cash,
		life_needs,
		everyday_needs,
		luxury_needs,
		revolt_risk,
		size_change,
		literacy
	};

	enum class population_display : uint8_t {
		none,
		province,
		state,
		nation,
	};

	class population_window_t;

	class population_window {
	public:
		std::unique_ptr<population_window_t> win;
		population_display display_type = population_display::none;
		population_sort sort_type = population_sort::none;

		provinces::province_tag population_for_province;
		nations::state_tag population_for_state;
		nations::country_tag population_for_nation;

		tagged_vector<uint8_t, population::pop_type_tag> filtered_pop_types;

		population_window();
		~population_window();
		void init_population_window(world_state& ws);
		void hide_population_window(ui::gui_manager& gui_m);
		void show_population_window(ui::gui_manager& gui_m, nations::country_tag top, nations::state_tag s, provinces::province_tag p);
		void show_population_window(ui::gui_manager& gui_m, nations::country_tag top, nations::state_tag t);
		void show_population_window(ui::gui_manager& gui_m, nations::country_tag t);
		void update_population_window(ui::gui_manager& gui_m);
		bool population_window_has_state_expanded(nations::state_tag t);
		void population_window_set_state_expanded(ui::gui_manager& gui_m, nations::state_tag t, bool expand);
	};
}
