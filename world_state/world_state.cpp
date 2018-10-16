#include "common\\common.h"
#include "world_state.h"
#include "provinces\\province_functions.h"
#include "nations\\nations_functions.h"
#include "cultures\\cultures_functions.h"
#include "population\\population_function.h"
#include "variables\\variables_functions.h"
#include "military\\military_functions.h"
#include "economy\\economy_functions.h"
#include "ideologies\\ideologies_functions.h"
#include "technologies\\technologies_functions.h"

void ready_world_state(world_state& ws) {
	variables::init_variables_state(ws);
	provinces::init_province_state(ws);
	cultures::init_cultures_state(ws);
	population::init_population_state(ws);
	nations::init_nations_state(ws);
	military::init_military_state(ws);
	economy::init_economic_scenario(ws);
	ideologies::init_ideology_state(ws);
	technologies::init_technology_state(ws);
}

namespace current_state {

	state::state() {};
	state::~state() {}

	void state::init_gui_objects(world_state& ws) {
		topbar_w.init_topbar(ws);
		government_w.init_government_window(ws);
		province_w.init_province_window(ws);
		diplomacy_w.init_diplomacy_window(ws);
		technologies_w.init_technology_winodw(ws);
		population_w.init_population_window(ws);
		production_w.init(ws);
		trade_w.init(ws);
	}

	void state::toggle_pause() {
		paused = !paused;
		topbar_w.update_topbar(gui_m);
	}
	void state::increase_speed() {
		speed = std::min(speed + 1, current_state::max_speed);
		topbar_w.update_topbar(gui_m);
	}
	void state::decrease_speed() {
		speed = std::max(speed - 1, 1);
		topbar_w.update_topbar(gui_m);
	}
}
