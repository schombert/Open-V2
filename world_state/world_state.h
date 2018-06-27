#pragma once
#include "common\\common.h"
#include "provinces\\provinces.h"
#include "cultures\\cultures.h"
#include "nations\\nations.h"
#include "scenario\\scenario.h"

namespace current_state {
	class state {
	public:
		provinces::provinces_state province_s;
		cultures::cultures_state culture_s;
		nations::nations_state nation_s;
	};
}

class world_state {
public:
	current_state::state w;
	scenario::scenario_manager s;
};
