#include "common\\common.h"
#include "world_state.h"
#include "provinces\\province_functions.h"
#include "nations\\nations_functions.h"
#include "cultures\\cultures_functions.h"
#include "population\\population_function.h"
#include "variables\\variables_functions.h"
#include "military\\military_functions.h"
#include "economy\\economy_functions.h"
#include "provinces\\province_gui.hpp"
#include "ideologies\\ideologies_functions.h"
#include "population\\population_gui.hpp"

void ready_world_state(world_state& ws) {
	variables::init_variables_state(ws);
	provinces::init_ready_provinces(ws);
	cultures::init_cultures_state(ws);
	population::init_population_state(ws);
	nations::init_nations_state(ws);
	military::init_military_state(ws);
	economy::init_economic_scenario(ws);
	ideologies::init_ideology_state(ws);
}

namespace current_state {
	class gui_state {
	public:
		provinces::province_window_t province_window;
		population::population_window_t population_window;
	};

	state::state() : gui_objects(std::make_unique<gui_state>()) {};
	state::~state() {}

	void state::hide_province_window() {
		auto gobj = gui_objects->province_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void state::show_province_window(provinces::province_tag p) {
		selected_province = p;
		ui::make_visible_and_update(gui_m, *(gui_objects->province_window.associated_object));
	}
	void state::init_gui_objects(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["province_view"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->province_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_pop"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->population_window);
	}
	void state::hide_population_window() {
		auto gobj = gui_objects->population_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void state::show_population_window(provinces::province_tag p) {
		selected_population.population_for_province = p;
		selected_population.display_type = population_display::province;
		ui::make_visible_and_update(gui_m, *(gui_objects->population_window.associated_object));
	}
}
