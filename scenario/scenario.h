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

		text_data::text_sequences& text_m;

		scenario_manager(text_data::text_sequences& tm) : text_m(tm) {}
	};
}