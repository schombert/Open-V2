#pragma once
#include "population\\population.h"
#include "cultures\\cultures.h"
#include "economy\\economy.h"
#include "governments\\governments.h"
#include "ideologies\\ideologies.h"
#include "issues\\issues.h"
#include "modifiers\\modifiers.h"
#include "provinces\\provinces.h"
#include "technologies\\technologies.h"
#include "text_data\\text_data.h"
#include "variables\\variables.h"
#include "military\\military.h"
#include "events\\events.h"
#include "triggers\\triggers.h"
#include "gui\\gui.h"
#include "sound\\sound.h"

namespace scenario {
	class scenario_manager {
	public:
		population::population_manager population_m;
		cultures::culture_manager culutre_m;
		economy::economic_scenario economy_m;
		governments::governments_manager governments_m;
		ideologies::ideologies_manager ideologies_m;
		issues::issues_manager issues_m;
		modifiers::modifiers_manager modifiers_m;
		provinces::province_manager province_m;
		technologies::technologies_manager technology_m;
		variables::variables_manager variables_m;
		military::military_manager military_m;
		events::event_manager event_m;
		triggers::trigger_manager trigger_m;

		ui::gui_static gui_m;
		sound::sound_manager sound_m;

		scenario_manager() {}
	};
}
