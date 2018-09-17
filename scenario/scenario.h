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
#include "simple_fs\\simple_fs.h"

namespace scenario {
	namespace fixed_ui {
		constexpr uint32_t expires_on = 0ui32;
		constexpr uint32_t slave_state = 1ui32;
		constexpr uint32_t colonial_province = 2ui32;
		constexpr uint32_t protectorate_province = 3ui32;
		constexpr uint32_t colonial_province_upgrade = 4ui32;
		constexpr uint32_t protectorate_province_upgrade = 5ui32;
		constexpr uint32_t province_view_admin = 6ui32;
		constexpr uint32_t province_view_admin_base = 7ui32;
		constexpr uint32_t province_view_admin_tech = 8ui32;
		constexpr uint32_t province_view_admin_pops = 9ui32;
		constexpr uint32_t controller = 10ui32;
		constexpr uint32_t flashpoint_tension = 11ui32;
		constexpr uint32_t has_no_flashpoint = 12ui32;
		constexpr uint32_t province_liferating = 13ui32;
		constexpr uint32_t siege_progress = 14ui32;
		constexpr uint32_t owner_presence = 15ui32;
		constexpr uint32_t other = 16ui32;
		constexpr uint32_t open_pop_screen = 17ui32;
		constexpr uint32_t colonial_investment_cost = 18ui32;
		constexpr uint32_t province_view_state_non_cores = 19ui32;
		constexpr uint32_t great_power = 20ui32;
		constexpr uint32_t secondary_power = 21ui32;
		constexpr uint32_t civilized_nation = 22ui32;
		constexpr uint32_t uncivilized_nation = 23ui32;
		constexpr uint32_t partialy_civilized_nation = 24ui32;
		constexpr uint32_t prestige = 25ui32;
		constexpr uint32_t military_power = 26ui32;
		constexpr uint32_t industrial_power = 27ui32;
		constexpr uint32_t is_mobilized = 28ui32;
		constexpr uint32_t sphere = 29ui32;
		constexpr uint32_t sphere_leader = 30ui32;
		constexpr uint32_t puppets = 31ui32;
		constexpr uint32_t liege = 32ui32;
		constexpr uint32_t fort = 33ui32;
		constexpr uint32_t railroad = 34ui32;
		constexpr uint32_t naval_base = 35ui32;
		constexpr uint32_t workforce = 36ui32;
		constexpr uint32_t religion = 37ui32;
		constexpr uint32_t ideology = 38ui32;
		constexpr uint32_t nationality = 39ui32;
		constexpr uint32_t dominant_issues = 40ui32;
		constexpr uint32_t electorate = 41ui32;
		constexpr uint32_t unemployment = 42ui32;
		constexpr uint32_t life_needs = 43ui32;
		constexpr uint32_t everyday_needs = 44ui32;
		constexpr uint32_t luxury_needs = 45ui32;
		constexpr uint32_t closed_factories = 46ui32;
		constexpr uint32_t no_closed_factories = 47ui32;
		constexpr uint32_t unemployed_workers = 48ui32;
		constexpr uint32_t no_unemployed_workers = 49ui32;
		constexpr uint32_t building_factories = 50ui32;
		constexpr uint32_t not_building_factories = 51ui32;
		constexpr uint32_t no_research = 52ui32;
		constexpr uint32_t optimal_is = 53ui32;
		constexpr uint32_t can_reform = 54ui32;
		constexpr uint32_t cannot_reform = 55ui32;
		constexpr uint32_t has_decisions = 56ui32;
		constexpr uint32_t no_decisions = 57ui32;
		constexpr uint32_t holding_election = 58ui32;
		constexpr uint32_t no_election = 59ui32;
		constexpr uint32_t has_rebels = 60ui32;
		constexpr uint32_t no_rebels = 61ui32;
		constexpr uint32_t is_not_gp = 62ui32;
		constexpr uint32_t is_not_losing_gp = 63ui32;
		constexpr uint32_t is_losing_gp = 64ui32;
		constexpr uint32_t cannot_influence = 65ui32;
		constexpr uint32_t can_influence = 66ui32;
		constexpr uint32_t no_crisis = 67ui32;
		constexpr uint32_t has_crisis = 68ui32;
		constexpr uint32_t protectorate_pending = 69ui32;
		constexpr uint32_t colony_pending = 70ui32;
		constexpr uint32_t state_pending = 71ui32;
		constexpr uint32_t colonial_investment_pending = 72ui32;
		constexpr uint32_t losing_colonial_race = 73ui32;
		constexpr uint32_t no_colonial_activity = 74ui32;
		constexpr uint32_t at_peace = 75ui32;
		constexpr uint32_t month_1 = 76ui32;
		constexpr uint32_t month_2 = 77ui32;
		constexpr uint32_t month_3 = 78ui32;
		constexpr uint32_t month_4 = 79ui32;
		constexpr uint32_t month_5 = 80ui32;
		constexpr uint32_t month_6 = 81ui32;
		constexpr uint32_t month_7 = 82ui32;
		constexpr uint32_t month_8 = 83ui32;
		constexpr uint32_t month_9 = 84ui32;
		constexpr uint32_t month_10 = 85ui32;
		constexpr uint32_t month_11 = 86ui32;
		constexpr uint32_t month_12 = 87ui32;
		constexpr uint32_t can_appoint_ruling_party = 88ui32;
		constexpr uint32_t next_election = 89ui32;
		constexpr uint32_t plurality = 90ui32;
		constexpr uint32_t revanchism = 91ui32;
		constexpr uint32_t cannot_hold_election = 92ui32;
		constexpr uint32_t already_holding_election = 93ui32;

		constexpr uint32_t count = 94ui32;
	}

	class scenario_manager {
	public:
		population::population_manager population_m;
		cultures::culture_manager culture_m;
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

		std::vector<text_data::text_tag> fixed_ui_text;

		scenario_manager() {}
	};

	void ready_scenario(scenario_manager& s, const directory& root);
}
