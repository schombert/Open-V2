#pragma once
#include "common\\common.h"
#include "provinces\\provinces.h"
#include "cultures\\cultures.h"
#include "nations\\nations.h"
#include "scenario\\scenario.h"
#include "economy\\economy.h"
#include "population\\population.h"
#include "variables\\variables.h"
#include "military\\military.h"
#include "gui\\gui.h"
#include "provinces\\province_gui.h"

namespace current_state {
	class state {
	public:
		provinces::provinces_state province_s;
		cultures::cultures_state culture_s;
		military::military_state military_s;
		nations::nations_state nation_s;
		economy::economic_state economy_s;
		population::population_state population_s;
		variables::variables_state variable_s;
		ui::gui_manager gui_m;

		//gui state
		provinces::province_window_t province_window;
	};
}

class world_state {
public:
	current_state::state w;
	scenario::scenario_manager s;
};

//called after loading a scenario & before loading a specific world state
void ready_world_state(world_state& ws);
