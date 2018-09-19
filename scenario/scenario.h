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
		constexpr uint32_t can_social_reform = 94ui32;
		constexpr uint32_t cannot_social_reform = 95ui32;
		constexpr uint32_t can_social_revoke = 96ui32;
		constexpr uint32_t can_political_reform = 97ui32;
		constexpr uint32_t cannot_political_reform = 98ui32;
		constexpr uint32_t can_political_revoke = 99ui32;
		constexpr uint32_t every = 100ui32;
		constexpr uint32_t any = 101ui32;
		constexpr uint32_t random = 102ui32;
		constexpr uint32_t neighboring_province = 103ui32;
		constexpr uint32_t where = 104ui32;
		constexpr uint32_t neighboring_nation = 105ui32;
		constexpr uint32_t nation = 106ui32;
		constexpr uint32_t empty_neighboring_province = 107ui32;
		constexpr uint32_t poor_strata_pop = 109ui32;
		constexpr uint32_t middle_strata_pop = 110ui32;
		constexpr uint32_t rich_strata_pop = 111ui32;
		constexpr uint32_t pop = 112ui32;
		constexpr uint32_t owned_province = 113ui32;
		constexpr uint32_t singular_nation = 114ui32;
		constexpr uint32_t singular_state = 115ui32;
		constexpr uint32_t singular_province = 116ui32;
		constexpr uint32_t singular_pop = 117ui32;
		constexpr uint32_t core_of = 118ui32;
		constexpr uint32_t state_of = 119ui32;
		constexpr uint32_t one_of_the_following = 120ui32;
		constexpr uint32_t chance_of = 121ui32;
		constexpr uint32_t owner_of = 122ui32;
		constexpr uint32_t controller_of = 123ui32;
		constexpr uint32_t location_of_pop = 124ui32;
		constexpr uint32_t nation_of_pop = 125ui32;
		constexpr uint32_t capital_of = 126ui32;
		constexpr uint32_t this_nation = 127ui32;
		constexpr uint32_t this_state = 128ui32;
		constexpr uint32_t this_province = 129ui32;
		constexpr uint32_t this_pop = 130ui32;
		constexpr uint32_t from_nation = 131ui32;
		constexpr uint32_t from_state = 132ui32;
		constexpr uint32_t from_province = 133ui32;
		constexpr uint32_t from_pop = 134ui32;
		constexpr uint32_t adjacent_sea = 135ui32;
		constexpr uint32_t cultural_union_of = 136ui32;
		constexpr uint32_t overlord_of = 137ui32;
		constexpr uint32_t sphere_leader_of = 138ui32;
		constexpr uint32_t crisis_state = 139ui32;
		constexpr uint32_t containing_state = 140ui32;
		constexpr uint32_t province_in = 141ui32;
		constexpr uint32_t reb_independence_nation = 142ui32;
		constexpr uint32_t flashpoint_nation = 143ui32;
		constexpr uint32_t no_effect = 144ui32;
		constexpr uint32_t move_capital_to = 145ui32;
		constexpr uint32_t add_x_core = 146ui32;
		constexpr uint32_t add_core_to = 147ui32;
		constexpr uint32_t rebel = 148ui32;
		constexpr uint32_t remove_x_core = 149ui32;
		constexpr uint32_t remove_core_from = 150ui32;
		constexpr uint32_t change_name_to = 151ui32;
		constexpr uint32_t change_rgo_production_to = 152ui32;
		constexpr uint32_t make_accepted_culture = 153ui32;
		constexpr uint32_t primary_culture_changes_to = 154ui32;
		constexpr uint32_t union_culture_accepted = 155ui32;
		constexpr uint32_t this_nation_culture = 156ui32;
		constexpr uint32_t from_nation_culture = 157ui32;
		constexpr uint32_t remove_accepted_culture = 158ui32;
		constexpr uint32_t life_rating = 159ui32;
		constexpr uint32_t make_national_religion = 160ui32;
		constexpr uint32_t make_slave_state = 161ui32;
		constexpr uint32_t make_slave_pop = 162ui32;
		constexpr uint32_t research_points = 163ui32;
		constexpr uint32_t change_tech_school = 164ui32;
		constexpr uint32_t change_government_to = 165ui32;
		constexpr uint32_t add_to_treasury = 166ui32;
		constexpr uint32_t war_exhaustion = 167ui32;
		constexpr uint32_t become_blank = 168ui32;
		constexpr uint32_t cultural_union_nation = 169ui32;
		constexpr uint32_t player_control_change = 170ui32;
		constexpr uint32_t set_national_flag = 171ui32;
		constexpr uint32_t remove_national_flag = 172ui32;
		constexpr uint32_t infamy = 173ui32;
		constexpr uint32_t change_province_owner = 174ui32;
		constexpr uint32_t annex_effect = 175ui32;
		constexpr uint32_t annexed_by_effect = 176ui32;
		constexpr uint32_t core_return = 177ui32;
		constexpr uint32_t core_return_short = 178ui32;
		constexpr uint32_t change_province_controller = 179ui32;
		constexpr uint32_t railroad_level = 180ui32;
		constexpr uint32_t pop_savings = 181ui32;
		constexpr uint32_t leadership_points = 182ui32;
		constexpr uint32_t release_as_vassal = 183ui32;
		constexpr uint32_t end_alliance = 184ui32;
		constexpr uint32_t end_war_with = 185ui32;
		constexpr uint32_t enable_blank = 186ui32;
		constexpr uint32_t change_ruling_party = 187ui32;
		constexpr uint32_t remove_prov_mod = 188ui32;
		constexpr uint32_t remove_nat_mod = 189ui32;
		constexpr uint32_t make_alliance = 190ui32;
		constexpr uint32_t become_independent = 191ui32;
		constexpr uint32_t release_as_independent = 192ui32;
		constexpr uint32_t enable_canal = 108ui32;
		constexpr uint32_t set_global_flag = 193ui32;
		constexpr uint32_t remove_global_flag = 194ui32;
		constexpr uint32_t change_national_value = 195ui32;
		constexpr uint32_t become_civ = 196ui32;
		constexpr uint32_t become_unciv = 197ui32;
		constexpr uint32_t free_slave_state = 198ui32;
		constexpr uint32_t free_slave_pop = 199ui32;
		constexpr uint32_t hold_election = 200ui32;
		constexpr uint32_t issue_change = 201ui32;
		constexpr uint32_t add_relative_income = 202ui32;
		constexpr uint32_t make_neutral = 203ui32;
		constexpr uint32_t pop_size = 204ui32;
		constexpr uint32_t move_pop_to = 205ui32;
		constexpr uint32_t change_pop_type = 206ui32;
		constexpr uint32_t years_of_research = 207ui32;
		constexpr uint32_t remove_mil_reforms = 208ui32;
		constexpr uint32_t remove_econ_reforms = 209ui32;
		constexpr uint32_t add_crime = 210ui32;
		constexpr uint32_t remove_crime = 211ui32;
		constexpr uint32_t perform_nationalization = 212ui32;
		constexpr uint32_t build_factory_in_capital = 213ui32;
		constexpr uint32_t enable_great_wars = 214ui32;
		constexpr uint32_t enable_world_wars = 215ui32;
		constexpr uint32_t disable_great_wars = 216ui32;
		constexpr uint32_t disable_world_wars = 217ui32;
		constexpr uint32_t assimilate_pop = 218ui32;
		constexpr uint32_t assimilate_province = 219ui32;
		constexpr uint32_t owner_primary_culture = 220ui32;
		constexpr uint32_t literacy = 221ui32;
		constexpr uint32_t add_crisis_interest = 222ui32;
		constexpr uint32_t crisis_temperature = 223ui32;
		constexpr uint32_t militancy = 224ui32;
		constexpr uint32_t consciousness = 225ui32;
		constexpr uint32_t fort_level = 226ui32;
		constexpr uint32_t naval_base_level = 227ui32;
		constexpr uint32_t rgo_size = 228ui32;
		constexpr uint32_t trigger_every_revolt = 229ui32;
		constexpr uint32_t where_ideology_is = 230ui32;
		constexpr uint32_t where_culture_is = 231ui32;
		constexpr uint32_t where_religion_is = 232ui32;
		constexpr uint32_t of_type = 233ui32;
		constexpr uint32_t diplomatic_influence_with = 234ui32;
		constexpr uint32_t relations_with = 235ui32;
		constexpr uint32_t add_modifier_until = 236ui32;
		constexpr uint32_t add_modifier = 237ui32;
		constexpr uint32_t add_cb_months = 238ui32;
		constexpr uint32_t add_cb_no_months = 239ui32;
		constexpr uint32_t add_cb_reversed_months = 240ui32;
		constexpr uint32_t add_cb_reversed_no_months = 241ui32;
		constexpr uint32_t remove_cb = 242ui32;
		constexpr uint32_t remove_cb_reversed = 243ui32;
		constexpr uint32_t no_nation = 244ui32;
		constexpr uint32_t declare_war_on = 245ui32;
		constexpr uint32_t attacker_wg_label = 246ui32;
		constexpr uint32_t defender_wg_label = 247ui32;
		constexpr uint32_t attacker_no_allies = 248ui32;
		constexpr uint32_t attacker_yes_allies = 249ui32;
		constexpr uint32_t for_text = 250ui32;
		constexpr uint32_t event_fires = 251ui32;
		constexpr uint32_t event_fires_in_days = 252ui32;
		constexpr uint32_t support_for_blank = 253ui32;

		constexpr uint32_t up_to = 254ui32;
		constexpr uint32_t scaled_support = 255ui32;
		constexpr uint32_t scaled_unemployment = 256ui32;
		constexpr uint32_t stockpile_of = 257ui32;
		constexpr uint32_t create_general = 258ui32;
		constexpr uint32_t create_admiral = 259ui32;


		constexpr uint32_t count = 166ui32;
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
