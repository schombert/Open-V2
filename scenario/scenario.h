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
#include "military\\military_containers.h"
#include "events\\events.h"
#include "triggers\\triggers.h"
#include "gui\\gui.h"
#include "sound\\sound.h"
#include "simple_fs\\simple_fs.h"
#include "world_state\\messages.h"
#include "provinces\province_containers.h"
#include "population\population_containers.h"


template<typename T>
using name_map_t = boost::container::flat_map<text_data::text_tag, T>;

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
		constexpr uint32_t no_crisis_alert = 67ui32;
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
		constexpr uint32_t crisis_temperature_plain = 223ui32;
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
		constexpr uint32_t add_war_goal = 260ui32;
		constexpr uint32_t blank_loyalty = 261ui32;
		constexpr uint32_t in_blank = 262ui32;
		constexpr uint32_t railroad_in_capital = 263ui32;
		constexpr uint32_t railroad_in_capital_state = 264ui32;
		constexpr uint32_t fort_in_capital = 265ui32;
		constexpr uint32_t fort_in_capital_state = 266ui32;
		constexpr uint32_t set_national_variable_to = 267ui32;
		constexpr uint32_t increase_national_variable_by = 268ui32;
		constexpr uint32_t decrease_national_variable_by = 269ui32;

		constexpr uint32_t cmp_ge = 270ui32;
		constexpr uint32_t cmp_le = 271ui32;
		constexpr uint32_t cmp_gt = 272ui32;
		constexpr uint32_t cmp_lt = 273ui32;
		constexpr uint32_t cmp_eq = 274ui32;
		constexpr uint32_t cmp_ne = 275ui32;
		constexpr uint32_t all_of_the_following = 276ui32;
		constexpr uint32_t any_of_the_following = 277ui32;
		constexpr uint32_t nation_at_war_against = 278ui32;
		constexpr uint32_t core_in = 279ui32;
		constexpr uint32_t substate_of = 280ui32;
		constexpr uint32_t nation_in_sphere = 281ui32;
		constexpr uint32_t cmp_has_eq = 282ui32;
		constexpr uint32_t cmp_has_ne = 283ui32;
		constexpr uint32_t year = 284ui32;
		constexpr uint32_t month = 285ui32;
		constexpr uint32_t a_port = 286ui32;
		constexpr uint32_t national_rank = 287ui32;
		constexpr uint32_t technology = 288ui32;
		constexpr uint32_t minimum_state_life_raing = 289ui32;
		constexpr uint32_t an_empy_adj_prov = 290ui32;
		constexpr uint32_t belongs_to_same_state = 291ui32;
		constexpr uint32_t does_not_belong_to_same_state = 292ui32;
		constexpr uint32_t contains_blank = 293ui32;
		constexpr uint32_t does_not_contain_blank = 294ui32;
		constexpr uint32_t times_target_needs_spending = 295ui32;
		constexpr uint32_t slavery = 296ui32;
		constexpr uint32_t allowed = 297ui32;
		constexpr uint32_t a_slave_state = 298ui32;
		constexpr uint32_t a_slave = 299ui32;
		constexpr uint32_t an_independent_nation = 300ui32;
		constexpr uint32_t non_prim_culture_pops = 301ui32;
		constexpr uint32_t government = 302ui32;
		constexpr uint32_t capital = 303ui32;
		constexpr uint32_t technology_school = 304ui32;
		constexpr uint32_t primary_culture = 305ui32;
		constexpr uint32_t an_accepted_culture = 306ui32;
		constexpr uint32_t culture = 307ui32;
		constexpr uint32_t dominant_culture = 308ui32;
		constexpr uint32_t a_primary_or_accepted = 309ui32;
		constexpr uint32_t rebel_culture = 310ui32;
		constexpr uint32_t this_nation_prim_culture = 311ui32;
		constexpr uint32_t from_nation_prim_culture = 312ui32;
		constexpr uint32_t culture_group = 313ui32;
		constexpr uint32_t rebel_culture_group = 314ui32;
		constexpr uint32_t this_nation_culture_group = 315ui32;
		constexpr uint32_t from_nation_culture_group = 316ui32;
		constexpr uint32_t rebel_religion = 317ui32;
		constexpr uint32_t this_nation_religion = 318ui32;
		constexpr uint32_t from_nation_religion = 319ui32;
		constexpr uint32_t province_terrain = 320ui32;
		constexpr uint32_t rgo_production = 321ui32;
		constexpr uint32_t a_secondary_power = 322ui32;
		constexpr uint32_t owner = 323ui32;
		constexpr uint32_t an_active_rebel = 324ui32;
		constexpr uint32_t a_member_of = 325ui32;
		constexpr uint32_t unclaimed_cores = 326ui32;
		constexpr uint32_t a_cultural_union = 327ui32;
		constexpr uint32_t union_for_pop = 328ui32;
		constexpr uint32_t union_for_nation = 329ui32;
		constexpr uint32_t the_rebel_ind_nation = 330ui32;
		constexpr uint32_t capitalists_can_build = 331ui32;
		constexpr uint32_t capitalists_cannot_build = 332ui32;
		constexpr uint32_t at_war = 333ui32;
		constexpr uint32_t total_blockade = 334ui32;
		constexpr uint32_t owns = 335ui32;
		constexpr uint32_t does_not_own = 336ui32;
		constexpr uint32_t controls = 337ui32;
		constexpr uint32_t does_not_control = 338ui32;
		constexpr uint32_t a_core_in = 339ui32;
		constexpr uint32_t a_core_of = 340ui32;
		constexpr uint32_t percentage_reb_control = 341ui32;
		constexpr uint32_t num_of_reb_control = 342ui32;
		constexpr uint32_t num_owned_provinces = 343ui32;
		constexpr uint32_t num_provinces_owned_by = 344ui32;
		constexpr uint32_t num_of_ports = 345ui32;
		constexpr uint32_t num_of_allies = 346ui32;
		constexpr uint32_t num_of_vassals = 347ui32;
		constexpr uint32_t owned_by = 348ui32;
		constexpr uint32_t exists = 349ui32;
		constexpr uint32_t does_not_exist = 350ui32;
		constexpr uint32_t set = 351ui32;
		constexpr uint32_t fx_in = 352ui32;
		constexpr uint32_t a_casus_belli_against = 353ui32;
		constexpr uint32_t military_access_with = 354ui32;
		constexpr uint32_t prestige_of = 355ui32;
		constexpr uint32_t a_railroad = 356ui32;
		constexpr uint32_t a_fort = 357ui32;
		constexpr uint32_t a_naval_base = 358ui32;
		constexpr uint32_t a_factory = 359ui32;
		constexpr uint32_t empty = 360ui32;
		constexpr uint32_t blockaded = 361ui32;
		constexpr uint32_t national_modifier = 362ui32;
		constexpr uint32_t provincial_modifier = 363ui32;
		constexpr uint32_t part_of = 364ui32;
		constexpr uint32_t a_border_with = 365ui32;
		constexpr uint32_t units_in_province = 366ui32;
		constexpr uint32_t at_war_against = 367ui32;
		constexpr uint32_t num_of_divisions = 368ui32;
		constexpr uint32_t an_ongoing_battle = 369ui32;
		constexpr uint32_t cmp_has_lt = 370ui32;
		constexpr uint32_t cmp_has_gt = 371ui32;
		constexpr uint32_t cmp_has_ge = 372ui32;
		constexpr uint32_t cmp_has_le = 373ui32;
		constexpr uint32_t treasury = 374ui32;
		constexpr uint32_t percentage_unowned_cores = 375ui32;
		constexpr uint32_t a_vassal = 376ui32;
		constexpr uint32_t ruling_party_ideology = 377ui32;
		constexpr uint32_t ruling_party = 378ui32;
		constexpr uint32_t enabled = 379ui32;
		constexpr uint32_t political_reform_desire = 380ui32;
		constexpr uint32_t social_reform_desire = 381ui32;
		constexpr uint32_t num_of_ships = 382ui32;
		constexpr uint32_t crime_percent = 383ui32;
		constexpr uint32_t national_religion = 384ui32;
		constexpr uint32_t member_of_nat_religion = 385ui32;
		constexpr uint32_t dominant_religion = 386ui32;
		constexpr uint32_t this_pop_culture = 387ui32;
		constexpr uint32_t this_nation_primary_culture = 388ui32;
		constexpr uint32_t coastal = 389ui32;
		constexpr uint32_t in_sphere_of = 390ui32;
		constexpr uint32_t a_producer_of = 391ui32;
		constexpr uint32_t next_reform = 392ui32;
		constexpr uint32_t always = 393ui32;
		constexpr uint32_t never = 394ui32;
		constexpr uint32_t percentage_recruited = 395ui32;
		constexpr uint32_t in_cultural_core_prov = 396ui32;
		constexpr uint32_t nationalism = 397ui32;
		constexpr uint32_t overseas = 398ui32;
		constexpr uint32_t controlled_by_rebel = 399ui32;
		constexpr uint32_t controlled_by = 400ui32;
		constexpr uint32_t state_capital = 401ui32;
		constexpr uint32_t a_truce_with = 402ui32;
		constexpr uint32_t population = 403ui32;
		constexpr uint32_t size = 404ui32;
		constexpr uint32_t pops_of_type = 405ui32;
		constexpr uint32_t of_type_plain = 406ui32;
		constexpr uint32_t ai_controlled = 407ui32;
		constexpr uint32_t a_leader_named = 408ui32;
		constexpr uint32_t a_possible_vassal = 409ui32;
		constexpr uint32_t a_vassal_of = 410ui32;
		constexpr uint32_t a_substate_of = 411ui32;
		constexpr uint32_t allied_to = 412ui32;
		constexpr uint32_t has_recently_lost_war = 413ui32;
		constexpr uint32_t has_not_recently_lost_war = 414ui32;
		constexpr uint32_t mobilized = 415ui32;
		constexpr uint32_t mobilization_size = 416ui32;
		constexpr uint32_t admin_spending = 417ui32;
		constexpr uint32_t greater_than_edu_spending = 418ui32;
		constexpr uint32_t support_for_rp = 419ui32;
		constexpr uint32_t colonial = 420ui32;
		constexpr uint32_t in_default_to = 421ui32;
		constexpr uint32_t num_of_ports_connected = 422ui32;
		constexpr uint32_t holding_an_election = 423ui32;
		constexpr uint32_t set_globally = 424ui32;
		constexpr uint32_t national_value = 425ui32;
		constexpr uint32_t industrial_score = 426ui32;
		constexpr uint32_t industrial_score_of = 427ui32;
		constexpr uint32_t military_score = 428ui32;
		constexpr uint32_t military_score_of = 429ui32;
		constexpr uint32_t civilized = 430ui32;
		constexpr uint32_t percent_occupied = 431ui32;
		constexpr uint32_t rich_tax = 432ui32;
		constexpr uint32_t middle_tax = 433ui32;
		constexpr uint32_t poor_tax = 434ui32;
		constexpr uint32_t social_spending = 435ui32;
		constexpr uint32_t edu_spending = 436ui32;
		constexpr uint32_t mil_spending = 437ui32;
		constexpr uint32_t colonies = 438ui32;
		constexpr uint32_t dominant_ideology = 439ui32;
		constexpr uint32_t dominant_issue = 440ui32;
		constexpr uint32_t poor_militancy = 441ui32;
		constexpr uint32_t middle_militancy = 442ui32;
		constexpr uint32_t rich_militancy = 443ui32;
		constexpr uint32_t gt_poor_tax = 444ui32;
		constexpr uint32_t cultural_union = 445ui32;
		constexpr uint32_t same_cultural_union = 446ui32;
		constexpr uint32_t non_accepted_pops = 447ui32;
		constexpr uint32_t crime = 448ui32;
		constexpr uint32_t num_substates = 449ui32;
		constexpr uint32_t num_vassals_no_substates = 450ui32;
		constexpr uint32_t num_regiments = 451ui32;
		constexpr uint32_t num_regiments_of = 452ui32;
		constexpr uint32_t fabricating_on = 453ui32;
		constexpr uint32_t cb_discovered = 454ui32;
		constexpr uint32_t cb_not_discovered = 455ui32;
		constexpr uint32_t cb_progress = 456ui32;
		constexpr uint32_t civ_progress = 457ui32;
		constexpr uint32_t constructing_a = 458ui32;
		constexpr uint32_t casus_belli = 459ui32;
		constexpr uint32_t nation_in_scope = 460ui32;
		constexpr uint32_t a_substate = 461ui32;
		constexpr uint32_t gw_enabled = 462ui32;
		constexpr uint32_t gw_not_enabled = 463ui32;
		constexpr uint32_t ww_enabled = 464ui32;
		constexpr uint32_t ww_not_enabled = 465ui32;
		constexpr uint32_t can_perform_nationalization = 466ui32;
		constexpr uint32_t cannot_perform_nationalization = 467ui32;
		constexpr uint32_t in_sphere = 468ui32;
		constexpr uint32_t num_states = 469ui32;
		constexpr uint32_t a_releasable_vassal = 470ui32;
		constexpr uint32_t recent_immigration = 471ui32;
		constexpr uint32_t num_control_days = 472ui32;
		constexpr uint32_t disarmed = 473ui32;
		constexpr uint32_t social_mov_support = 474ui32;
		constexpr uint32_t political_mov_support = 475ui32;
		constexpr uint32_t a_member_of_social_mov = 476ui32;
		constexpr uint32_t a_member_of_political_mov = 477ui32;
		constexpr uint32_t cultural_sphere_members = 478ui32;
		constexpr uint32_t pops = 479ui32;
		constexpr uint32_t this_pop_religion = 480ui32;
		constexpr uint32_t pops_with = 481ui32;
		constexpr uint32_t life_needs_satisfaction = 482ui32;
		constexpr uint32_t everyday_needs_satisfaction = 483ui32;
		constexpr uint32_t luxury_needs_satisfaction = 484ui32;
		constexpr uint32_t present_in_state = 485ui32;
		constexpr uint32_t flashpoint = 486ui32;
		constexpr uint32_t there = 487ui32;
		constexpr uint32_t an_ongoing_crisis = 488ui32;
		constexpr uint32_t current_crisis = 489ui32;
		constexpr uint32_t liberation_crisis = 490ui32;
		constexpr uint32_t claim_crisis = 491ui32;
		constexpr uint32_t involved_in_crisis = 492ui32;
		constexpr uint32_t poor_life_needs_satisfaction = 493ui32;
		constexpr uint32_t poor_everyday_needs_satisfaction = 494ui32;
		constexpr uint32_t poor_luxury_needs_satisfaction = 495ui32;
		constexpr uint32_t middle_life_needs_satisfaction = 496ui32;
		constexpr uint32_t middle_everyday_needs_satisfaction = 497ui32;
		constexpr uint32_t middle_luxury_needs_satisfaction = 498ui32;
		constexpr uint32_t rich_life_needs_satisfaction = 499ui32;
		constexpr uint32_t rich_everyday_needs_satisfaction = 501ui32;
		constexpr uint32_t rich_luxury_needs_satisfaction = 502ui32;
		constexpr uint32_t dip_influence_over = 503ui32;
		constexpr uint32_t relationship_with = 504ui32;
		constexpr uint32_t this_pop_type = 505ui32;
		constexpr uint32_t nat_variable = 506ui32;
		constexpr uint32_t support_in_uh = 507ui32;
		constexpr uint32_t support_in = 508ui32;
		constexpr uint32_t scope_province = 509ui32;
		constexpr uint32_t able_build_rr = 510ui32;
		constexpr uint32_t able_build_fort = 511ui32;
		constexpr uint32_t able_build_naval_base = 512ui32;
		constexpr uint32_t work = 513ui32;
		constexpr uint32_t available_for = 514ui32;
		constexpr uint32_t support = 515ui32;
		constexpr uint32_t percentage = 516ui32;
		constexpr uint32_t stockpile = 517ui32;
		constexpr uint32_t this_nat_religion = 518ui32;
		constexpr uint32_t from_nat_religion = 519ui32;
		constexpr uint32_t strata = 520ui32;
		constexpr uint32_t rich = 521ui32;
		constexpr uint32_t poor = 522ui32;
		constexpr uint32_t middle = 523ui32;
		constexpr uint32_t traditional_academic = 524ui32;
		constexpr uint32_t base_plus = 525ui32;
		constexpr uint32_t base_times = 526ui32;
		constexpr uint32_t chance = 527ui32;
		constexpr uint32_t reform = 528ui32;
		constexpr uint32_t enable_crime_tech = 529ui32;
		constexpr uint32_t activate_goods = 530ui32;
		constexpr uint32_t gas_attack_cap = 531ui32;
		constexpr uint32_t gas_defence_cap = 532ui32;
		constexpr uint32_t tech_activation_year = 533ui32;
		constexpr uint32_t tech_research_points = 534ui32;
		constexpr uint32_t tech_rebel_org_gain = 535ui32;
		constexpr uint32_t rgo_output_tech = 536ui32;
		constexpr uint32_t rgo_throughput_tech = 537ui32;
		constexpr uint32_t factory_input_tech = 538ui32;
		constexpr uint32_t factory_output_tech = 539ui32;
		constexpr uint32_t factory_throughput_tech = 540ui32;
		constexpr uint32_t artisan_input_tech = 541ui32;
		constexpr uint32_t artisan_output_tech = 542ui32;
		constexpr uint32_t artisan_throughput_tech = 543ui32;
		constexpr uint32_t unit_defence = 544ui32;
		constexpr uint32_t unit_hull = 545ui32;
		constexpr uint32_t unit_attack = 546ui32;
		constexpr uint32_t unit_gun_power = 547ui32;
		constexpr uint32_t unit_reconnaissance = 548ui32;
		constexpr uint32_t unit_fire_range = 549ui32;
		constexpr uint32_t unit_support = 550ui32;
		constexpr uint32_t unit_torpedo_attack = 551ui32;
		constexpr uint32_t unit_maneuver = 552ui32;
		constexpr uint32_t unit_evasion = 553ui32;
		constexpr uint32_t unit_speed = 554ui32;
		constexpr uint32_t unit_organization = 555ui32;
		constexpr uint32_t unit_build_time = 556ui32;
		constexpr uint32_t unit_supply_consumption = 557ui32;
		constexpr uint32_t unit_strength = 558ui32;
		constexpr uint32_t unit_siege = 559ui32;
		constexpr uint32_t unit_discipline = 560ui32;
		constexpr uint32_t disabled = 561ui32;
		constexpr uint32_t tx_day = 562ui32;
		constexpr uint32_t shared_prestige_tech = 563ui32;
		constexpr uint32_t demands_label = 564ui32;
		constexpr uint32_t crisis_title = 565ui32;
		constexpr uint32_t crisis_colonize = 566ui32;
		constexpr uint32_t crisis_influence = 567ui32;
		constexpr uint32_t crisis_liberation = 568ui32;
		constexpr uint32_t crisis_reclaim = 569ui32;
		constexpr uint32_t no_crisis = 570ui32;
		constexpr uint32_t crisis_temperature = 571ui32;
		constexpr uint32_t crisis_waiting = 572ui32;
		constexpr uint32_t crisis_exists = 573ui32;
		constexpr uint32_t rel_hostile = 574ui32;
		constexpr uint32_t rel_opposed = 575ui32;
		constexpr uint32_t rel_neutral = 576ui32;
		constexpr uint32_t rel_cordial = 577ui32;
		constexpr uint32_t rel_friendly = 578ui32;
		constexpr uint32_t rel_sphere = 579ui32;
		constexpr uint32_t brigades = 580ui32;
		constexpr uint32_t warships = 581ui32;
		constexpr uint32_t truce_lasts_until = 582ui32;
		constexpr uint32_t against = 583ui32;
		constexpr uint32_t make_alliance_button = 584ui32;
		constexpr uint32_t cancel_alliance_button = 585ui32;
		constexpr uint32_t call_ally_button = 586ui32;
		constexpr uint32_t expel_advisors_button = 587ui32;
		constexpr uint32_t ban_embassy_button = 588ui32;
		constexpr uint32_t justify_war_button = 589ui32;
		constexpr uint32_t declare_war_button = 590ui32;
		constexpr uint32_t offer_peace_button = 591ui32;
		constexpr uint32_t show_empty_states = 592ui32;
		constexpr uint32_t hide_empty_states = 593ui32;
		constexpr uint32_t closed = 594ui32;
		constexpr uint32_t great_powers = 595ui32;
		constexpr uint32_t build = 596ui32;
		constexpr uint32_t reopen = 597ui32;
		constexpr uint32_t expand = 598ui32;
		constexpr uint32_t exports = 599ui32;
		constexpr uint32_t imports = 600ui32;
		constexpr uint32_t demand_label = 601ui32;
		constexpr uint32_t supply_label = 602ui32;
		constexpr uint32_t fixed_income = 603ui32;
		constexpr uint32_t variable_expenses = 604ui32;
		constexpr uint32_t total_debt = 605ui32;
		constexpr uint32_t borrowing_and_saving = 606ui32;
		constexpr uint32_t pie_no_needs = 607ui32;
		constexpr uint32_t pie_ln = 608ui32;
		constexpr uint32_t pie_en = 609ui32;
		constexpr uint32_t pie_lx = 610ui32;
		constexpr uint32_t no_pops_of_type = 611ui32;
		constexpr uint32_t b_poor_tax = 612ui32;
		constexpr uint32_t b_middle_tax = 613ui32;
		constexpr uint32_t b_rich_tax = 614ui32;
		constexpr uint32_t tariffs = 615ui32;
		constexpr uint32_t interest = 616ui32;
		constexpr uint32_t national_stockpile = 617ui32;
		constexpr uint32_t allowed_build_railroad = 616ui32;
		constexpr uint32_t level_less_than = 619ui32;
		constexpr uint32_t already_upgrading = 620ui32;
		constexpr uint32_t rail_tech_prerequisite = 621ui32;
		constexpr uint32_t no_other_naval_base = 622ui32;
		constexpr uint32_t is_coastal = 623ui32;
		constexpr uint32_t bankruptcy_warning = 624ui32;
		constexpr uint32_t nationalist_movement = 625ui32;
		constexpr uint32_t pop_size_growth = 626ui32;
		constexpr uint32_t pop_size_combat = 627ui32;
		constexpr uint32_t pop_size_type_change = 628ui32;
		constexpr uint32_t pop_size_assimilation = 629ui32;
		constexpr uint32_t pop_size_migration = 630ui32;
		constexpr uint32_t pop_size_emigration = 631ui32;
		constexpr uint32_t goto_label = 632ui32;
		constexpr uint32_t fabricate_title = 633ui32;
		constexpr uint32_t fabricate_description = 634ui32;
		constexpr uint32_t fabrication_condition = 635ui32;
		constexpr uint32_t tech_researched_header = 636ui32;
		constexpr uint32_t tech_condition_previous = 637ui32;
		constexpr uint32_t tech_condition_year = 638ui32;
		constexpr uint32_t tech_condition_not_done = 639ui32;
		constexpr uint32_t automate_option = 640ui32;
		constexpr uint32_t monthly_chance = 641ui32;
		constexpr uint32_t importance_label = 642ui32;
		constexpr uint32_t message_type_discard = 643ui32;
		constexpr uint32_t message_type_log = 644ui32;
		constexpr uint32_t message_type_popup = 645ui32;
		constexpr uint32_t message_type_pause = 646ui32;
		constexpr uint32_t message_group_self = 647ui32;
		constexpr uint32_t message_group_neighbors = 648ui32;
		constexpr uint32_t message_group_sphere_members = 649ui32;
		constexpr uint32_t message_group_vassals = 650ui32;
		constexpr uint32_t message_group_allies = 651ui32;
		constexpr uint32_t message_group_great_powers = 652ui32;
		constexpr uint32_t message_group_sphere_leader = 653ui32;
		constexpr uint32_t message_group_overlord = 654ui32;
		constexpr uint32_t message_settings = 655ui32;
		constexpr uint32_t find_label = 656ui32;
		constexpr uint32_t window_mode_one = 657ui32;
		constexpr uint32_t window_mode_two = 658ui32;
		constexpr uint32_t projection_one = 659ui32;
		constexpr uint32_t projection_two = 660ui32;
		constexpr uint32_t zoom_one = 661ui32;
		constexpr uint32_t zoom_two = 662ui32;
		constexpr uint32_t cannot_appoint_ruling_party = 663ui32;
		constexpr uint32_t party_not_permitted = 664ui32;
		constexpr uint32_t party_already_ruling = 665ui32;
		constexpr uint32_t build_factory = 666ui32;
		constexpr uint32_t expand_factory = 667ui32;
		constexpr uint32_t open_factory = 668ui32;
		constexpr uint32_t destroy_factory = 669ui32;
		constexpr uint32_t factory_priority = 670ui32;
		constexpr uint32_t can_subsidise = 671ui32;
		constexpr uint32_t pop_build_factory = 672ui32;
		constexpr uint32_t pop_expand_factory = 673ui32;
		constexpr uint32_t pop_open_factory = 674ui32;
		constexpr uint32_t delete_factory_if_no_input = 675ui32;
		constexpr uint32_t build_factory_invest = 676ui32;
		constexpr uint32_t expand_factory_invest = 677ui32;
		constexpr uint32_t open_factory_invest = 678ui32;
		constexpr uint32_t build_railway_invest = 679ui32;
		constexpr uint32_t can_invest_in_pop_projects = 680ui32;
		constexpr uint32_t pop_build_factory_invest = 681ui32;
		constexpr uint32_t pop_expand_factory_invest = 682ui32;
		constexpr uint32_t pop_open_factory_invest = 683ui32;
		constexpr uint32_t allow_foreign_investment = 684ui32;
		constexpr uint32_t slavery_allowed = 685ui32;
		constexpr uint32_t build_railway = 686ui32;
		constexpr uint32_t no_change_party = 687ui32;
		constexpr uint32_t upper_house_support = 688ui32;
		constexpr uint32_t no_more_reform = 689ui32;
		constexpr uint32_t reform_next_step = 690ui32;
		constexpr uint32_t no_assimilation = 691ui32;
		constexpr uint32_t assimilation_factors = 692ui32;
		constexpr uint32_t projected_assimilation = 693ui32;
		constexpr uint32_t assimilation_scale = 694ui32;
		constexpr uint32_t factor = 695ui32;
		constexpr uint32_t no_conversion = 696ui32;
		constexpr uint32_t conversion_factors = 697ui32;
		constexpr uint32_t projected_conversion = 698ui32;
		constexpr uint32_t conversion_scale = 699ui32;
		constexpr uint32_t no_promotion = 700ui32;
		constexpr uint32_t no_demotion = 701ui32;
		constexpr uint32_t no_migration = 702ui32;
		constexpr uint32_t no_emigration = 703ui32;
		constexpr uint32_t no_colonial_migration = 704ui32;
		constexpr uint32_t projected_promotion = 705ui32;
		constexpr uint32_t projected_demotion = 706ui32;
		constexpr uint32_t projected_migration = 707ui32;
		constexpr uint32_t projected_emigration = 708ui32;
		constexpr uint32_t promotion_factors = 709ui32;
		constexpr uint32_t demotion_factors = 710ui32;
		constexpr uint32_t migration_factors = 711ui32;
		constexpr uint32_t emigration_factors = 712ui32;
		constexpr uint32_t national_focus = 713ui32;
		constexpr uint32_t promotion_scale = 714ui32;
		constexpr uint32_t admin_efficiency = 715ui32;
		constexpr uint32_t emigration_scale = 716ui32;
		constexpr uint32_t parenthetical_squared = 717ui32;
		constexpr uint32_t tech_edu_efficiency = 718ui32;
		constexpr uint32_t literacy_change_speed = 719ui32;
		constexpr uint32_t clergy_amount = 720ui32;
		constexpr uint32_t projected_literacy = 721ui32;
		constexpr uint32_t lit_factors = 722ui32;
		constexpr uint32_t projected_militancy = 723ui32;
		constexpr uint32_t militancy_factors = 724ui32;
		constexpr uint32_t needs_satisfaction = 725ui32;
		constexpr uint32_t non_accepted_factors = 726ui32;
		constexpr uint32_t owner_seperatism = 727ui32;
		constexpr uint32_t base_non_accepted_mil = 728ui32;
		constexpr uint32_t conservative_support = 729ui32;
		constexpr uint32_t ruling_party_support = 730ui32;
		constexpr uint32_t projected_consciouness = 731ui32;
		constexpr uint32_t consciousness_factors = 732ui32;
		constexpr uint32_t literacy_factor = 733ui32;
		constexpr uint32_t colonial_factor = 734ui32;
		constexpr uint32_t poor_clergy = 735ui32;
		constexpr uint32_t mid_rich_clergy = 736ui32;
		constexpr uint32_t lux_satisfaction_factor = 737ui32;
		constexpr uint32_t weight = 738ui32;
		constexpr uint32_t national_political_reform_desire = 739ui32;
		constexpr uint32_t national_social_reform_desire = 740ui32;
		constexpr uint32_t parenthetical_national_modifier = 741ui32;
		constexpr uint32_t parenthetical_provincial_modifier = 742ui32;
		constexpr uint32_t profit = 743ui32;
		constexpr uint32_t total_cost = 744ui32;
		constexpr uint32_t factory_unlocked = 745ui32;
		constexpr uint32_t foreign_investment_allowed = 746ui32;
		constexpr uint32_t factory_building_allowed = 747ui32;
		constexpr uint32_t factory_limit = 748ui32;
		constexpr uint32_t state_is_coastal = 749ui32;
		constexpr uint32_t factory_already_present = 750ui32;
		constexpr uint32_t not_colonial = 751ui32;
		constexpr uint32_t no_focus = 752ui32;
		constexpr uint32_t focus_limit = 753ui32;
		constexpr uint32_t state_population = 754ui32;
		constexpr uint32_t replace_national_focus = 755ui32;
		constexpr uint32_t map_legend_culture = 756ui32;
		constexpr uint32_t map_legend_political = 757ui32;
		constexpr uint32_t owner_label = 758ui32;
		constexpr uint32_t map_legend_region = 759ui32;
		constexpr uint32_t map_legend_sphere = 760ui32;
		constexpr uint32_t sphere_label = 761ui32;
		constexpr uint32_t region_label = 762ui32;
		constexpr uint32_t none = 763ui32;
		constexpr uint32_t map_legend_population = 764ui32;
		constexpr uint32_t population_label = 765ui32;
		constexpr uint32_t map_legend_relations = 766ui32;
		constexpr uint32_t map_relation_explanation = 767ui32;
		constexpr uint32_t map_legend_migration = 768ui32;
		constexpr uint32_t migration_external_label = 769ui32;
		constexpr uint32_t migration_internal_label = 770ui32;
		constexpr uint32_t map_legend_infrastructure = 771ui32;
		constexpr uint32_t map_legend_rgo = 772ui32;
		constexpr uint32_t all = 773ui32;
		constexpr uint32_t map_legend_prices = 774ui32;
		constexpr uint32_t map_legend_production = 775ui32;
		constexpr uint32_t price_label = 776ui32;
		constexpr uint32_t production_label = 777ui32;
		constexpr uint32_t map_legend_voting = 778ui32;
		constexpr uint32_t map_legend_admin = 779ui32;
		constexpr uint32_t admin_eff_label = 780ui32;
		constexpr uint32_t map_legend_military = 781ui32;
		constexpr uint32_t controller_label = 782ui32;

		constexpr uint32_t count = 783ui32;
	}


	constexpr uint32_t zoom_labels[] = {
		scenario::fixed_ui::zoom_one,
		scenario::fixed_ui::zoom_two,
	};
	constexpr auto zoom_type_count = std::extent_v<decltype(zoom_labels)>;

	constexpr uint32_t projection_labels[] = {
		scenario::fixed_ui::projection_one,
		scenario::fixed_ui::projection_two,
	};
	constexpr auto projection_type_count = std::extent_v<decltype(projection_labels)>;


	constexpr uint32_t window_mode_labels[] = {
		scenario::fixed_ui::window_mode_one,
		scenario::fixed_ui::window_mode_two,
	};
	constexpr auto window_mode_count = std::extent_v<decltype(window_mode_labels)>;

	constexpr float ui_scales[] = {
		0.5f,
		0.75f,
		0.9f,
		1.0f,
		1.25f,
		1.5f,
		1.75f,
		2.0f,
		2.5f,
		3.0f,
		4.0f,
		5.0f
	};
	constexpr auto ui_scale_count = std::extent_v<decltype(ui_scales)>;

	enum class zoom_type : int8_t {
		to_center = 0,
		to_cursor = 1
	};

	struct game_settings {
		int32_t window_mode = 0;
		int32_t projection = 0;
		int32_t ui_scale = 3;

		float master_volume = 1.0f;
		float effects_volume = 1.0f;
		float interface_volume = 1.0f;
		float music_volume = 1.0f;

		zoom_type zoom_setting = zoom_type::to_cursor;
	};

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
		messages::messages_manager message_m;
		ui::gui_static gui_m;
		sound::sound_manager sound_m;

		game_settings settings;

		std::vector<text_data::text_tag> fixed_ui_text;

		scenario_manager() {}

		GET_SET_STV(culture_m.culture_groups)
		GET_SET_STV(culture_m.religions)
		GET_SET_STV(culture_m.culture_container)
		GET_SET_STV(culture_m.national_tags)
		GET_SET_TV(::culture::union_tag, culture_m.cultures_to_tags)
		GET_SET_TV(::culture::group_direct, culture_m.cultures_to_groups)
		GET_SET_TV(::culture_group::union_tag_direct, culture_m.groups_to_tags)
		GET_SET_TV(::nation_tag::culture_group, culture_m.tags_to_groups)
		GET_SET_TFV(::nation_tag::government_names, culture_m.country_names_by_government)
		GET_SET_TFV_VV(::culture::first_names, culture_m.first_names_by_culture)
		GET_SET_TFV_VV(::culture::last_names, culture_m.last_names_by_culture)
		GET_SET_TFV_VV(::culture_group::cultures, culture_m.culture_by_culture_group)
		GET_SET_TV(::cultures::leader_pictures, culture_m.leader_pictures)
		GET_SET_GEN(province_m.province_container)
		GET_SET_TV(state_region::name, province_m.state_names)
		GET_SET_TFV_VV(state_region::provinces, province_m.states_to_province_index)
		GET_SET_TFV_VV(province::same_type_adjacency, province_m.same_type_adjacency)
		GET_SET_TFV_VV(province::coastal_adjacency, province_m.coastal_adjacency)
		GET_SET_STV(military_m.cb_types)
		GET_SET_TV(military::leader_trait_name, military_m.leader_traits)
		GET_SET_TV(::military::personality_traits, military_m.personality_traits)
		GET_SET_TV(::military::background_traits, military_m.background_traits)
		GET_SET_TFV(military::leader_trait_values, military_m.leader_trait_definitions)
		GET_SET_TV(::cb_type::construction_speed_direct, military_m.cb_type_to_speed)


		text_data::text_tag get_text_handle(const char* key_start, const char* key_end) {
			return text_data::get_text_handle(gui_m.text_data_sequences, key_start, key_end);
		}
		text_data::text_tag get_thread_safe_text_handle(const char* key_start, const char* key_end) {
			return text_data::get_thread_safe_text_handle(gui_m.text_data_sequences, key_start, key_end);
		}
		text_data::text_tag get_existing_text_handle(const char* key_start, const char* key_end) const {
			return text_data::get_existing_text_handle(gui_m.text_data_sequences, key_start, key_end);
		}
		text_data::text_tag get_thread_safe_existing_text_handle(const char* key_start, const char* key_end) const {
			return text_data::get_thread_safe_existing_text_handle(gui_m.text_data_sequences, key_start, key_end);
		}

		template<size_t N>
		text_data::text_tag get_thread_safe_existing_text_handle(const char(&t)[N]) const {
			return get_thread_safe_existing_text_handle(t, t + N - 1);
		}
		template<size_t N>
		text_data::text_tag get_existing_text_handle(const char(&t)[N]) const {
			return get_existing_text_handle(t, t + N - 1);
		}
		template<size_t N>
		text_data::text_tag get_thread_safe_text_handle(const char(&t)[N]) {
			return get_thread_safe_text_handle(t, t + N - 1);
		}
		template<size_t N>
		text_data::text_tag get_text_handle(const char(&t)[N]) {
			return get_text_handle(t, t + N - 1);
		}

		template<typename index_t>
		std::enable_if_t<std::is_same_v<index_t, military::cb_type_tag>, name_map_t<military::cb_type_tag>&> name_map() {
			return military_m.named_cb_type_index;
		}
		template<typename index_t>
		std::enable_if_t<std::is_same_v<index_t, military::cb_type_tag>, name_map_t<military::cb_type_tag> const&> name_map() const {
			return military_m.named_cb_type_index;
		}

		template<typename index_t>
		std::enable_if_t<std::is_same_v<index_t, military::leader_trait_tag>, name_map_t<military::leader_trait_tag>&> name_map() {
			return military_m.named_leader_trait_index;
		}
		template<typename index_t>
		std::enable_if_t<std::is_same_v<index_t, military::leader_trait_tag>, name_map_t<military::leader_trait_tag> const&> name_map() const {
			return military_m.named_leader_trait_index;
		}
		
		template<typename index_t>
		std::enable_if_t<std::is_same_v<index_t, cultures::national_tag>, boost::container::flat_map<uint32_t, cultures::national_tag>&> name_map() {
			return culture_m.national_tags_index;
		}
		template<typename index_t>
		std::enable_if_t<std::is_same_v<index_t, cultures::national_tag>, boost::container::flat_map<uint32_t, cultures::national_tag> const&> name_map() const {
			return culture_m.national_tags_index;
		}

		template<typename tag_type, typename F>
		std::enable_if_t<std::is_same_v<tag_type, military::cb_type_tag>> for_each(F const& f) const {
			int32_t const cmax = int32_t(military_m.cb_types.size());
			for(int32_t i = 0; i < cmax; ++i) {
				f(military::cb_type_tag(military::cb_type_tag::value_base_t(i)));
			}
		}
		template<typename tag_type, typename F, typename partitioner_t = concurrency::auto_partitioner>
		std::enable_if_t<std::is_same_v<tag_type, military::cb_type_tag>> par_for_each(F const& f, partitioner_t&& p = concurrency::auto_partitioner()) const {
			int32_t const cmax = int32_t(military_m.cb_types.size());
			concurrency::parallel_for(0, cmax, [&f](int32_t i) {
				f(military::cb_type_tag(military::cb_type_tag::value_base_t(i)));
			}, p);
		}
		template<typename tag_type, typename F>
		std::enable_if_t<std::is_same_v<tag_type, military::cb_type_tag>, bool> any_of(F const& f) const {
			int32_t const cmax = int32_t(military_m.cb_types.size());
			for(int32_t i = 0; i < cmax; ++i) {
				if(f(military::cb_type_tag(military::cb_type_tag::value_base_t(i))))
					return true;
			}
			return false;
		}
	};

	void ready_scenario(scenario_manager& s, const directory& root);
}
