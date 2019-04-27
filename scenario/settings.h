#pragma once
#include "common\\common.h"
#include "scenario.h"

namespace settings {
	void save_settings(scenario::scenario_manager const& s);
	void load_settings(scenario::scenario_manager& s);
}
