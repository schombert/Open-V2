#include "scenario_io.h"

namespace scenario {
	void read_scenario(scenario_manager& s, const directory& root) {
		//stage 1

		graphics::name_maps gobj_nmaps;
		ui::load_gui_from_directory(root, s.gui_m, gobj_nmaps);

		//stage 2
		auto const country_files = cultures::read_national_tags(s.culture_m, root);

		cultures::read_religions(s.culture_m, root, s.gui_m.text_data_sequences);
		cultures::read_cultures(s.culture_m, s.gui_m.textures, root, s.gui_m.text_data_sequences);

		economy::read_goods(s.economy_m, root, s.gui_m.text_data_sequences);
		auto building_production_map = economy::read_buildings(s.economy_m, root, s.gui_m.text_data_sequences);

		auto const ideology_state = ideologies::pre_parse_ideologies(s.ideologies_m, root, s.gui_m.text_data_sequences);

		auto const issues_state = issues::pre_parse_issues(s.issues_m, root, s.gui_m.text_data_sequences);

		auto const government_names = governments::read_governments(s.governments_m, root, s.gui_m.text_data_sequences, s.ideologies_m);

		military::parsing_state military_state(s.gui_m.text_data_sequences, s.military_m);
		military::pre_parse_unit_types(military_state, root);
		military::pre_parse_cb_types(military_state, root);
		military::read_leader_traits(military_state, root);

		modifiers::parsing_state modifiers_state(s.gui_m.text_data_sequences, s.modifiers_m);
		modifiers::read_defines(s.modifiers_m, root);
		modifiers::pre_parse_crimes(modifiers_state, root);
		modifiers::pre_parse_triggered_modifiers(modifiers_state, root);
		modifiers::read_national_values(modifiers_state, root);
		modifiers::read_static_modifiers(modifiers_state, root);
		modifiers::read_event_modifiers(modifiers_state, root);

		population::parsing_state pop_state(s.gui_m.text_data_sequences, s.population_m);
		population::pre_parse_pop_types(s.population_m, root, s.gui_m.text_data_sequences);
		population::pre_parse_rebel_types(pop_state, root);

		provinces::parsing_state province_state(s.gui_m.text_data_sequences, s.province_m, s.modifiers_m);
		provinces::read_default_map_file(province_state, root);

		provinces::read_terrain_modifiers(s.gui_m.text_data_sequences, s.province_m, s.modifiers_m, gobj_nmaps, root);
		provinces::read_states(province_state, root);
		provinces::read_continents(province_state, root);
		provinces::read_climates(province_state, root);

		sound::populate_music(s.sound_m, root);
		sound::read_effects(s.sound_m, s.gui_m.text_data_sequences, root);

		technologies::parsing_state tech_state(s.gui_m.text_data_sequences, root.get_directory(u"\\technologies"), s.technology_m, s.modifiers_m);
		technologies::pre_parse_technologies(tech_state, root);
		technologies::pre_parse_inventions(tech_state, root);

		// stage 3

		events::event_creation_manager ecm;

		governments::ready_party_issues(s.governments_m, s.issues_m);

		cultures::read_country_files(country_files, s, root);
		cultures::read_flag_graphics(s, root);
		cultures::populate_country_names(s, government_names);

		economy::read_production_types(s, building_production_map, root);

		ideologies::read_ideologies(s, ideology_state);

		issues::read_issue_options(issues_state, s, ecm);

		military::read_unit_types(military_state, s.military_m, s.economy_m, s.sound_m, s.gui_m.text_data_sequences);
		military::read_cb_types(military_state, s, ecm);

		modifiers::read_crimes(modifiers_state, s);
		modifiers::read_triggered_modifiers(modifiers_state, s);
		modifiers::read_national_focuses(s, root);

		population::read_main_poptype_file(s, root);
		population::read_poptypes(s, root);
		population::read_rebel_types(pop_state, s, ecm);
		population::populate_demote_to(s.population_m);

		events::read_on_actions_file(s, ecm, root);
		events::read_event_files(s, ecm, root);
		events::read_decision_files(s, ecm, root);

		technologies::prepare_technologies_read(s);
		technologies::read_inventions(tech_state, s);
		technologies::read_technologies(tech_state, s);

		governments::setup_party_rules(s);

		// stage 4

		commit_pending_triggered_events(s, ecm, root);

		prepare_fixed_ui_text(s);
	}

	void prepare_fixed_ui_text(scenario_manager& s) {
		s.fixed_ui_text.resize(fixed_ui::count);

		s.fixed_ui_text[fixed_ui::expires_on] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "EXPIRES_ON");
		s.fixed_ui_text[fixed_ui::slave_state] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PW_SLAVE_STATE");
		s.fixed_ui_text[fixed_ui::colonial_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PW_COLONY");
		s.fixed_ui_text[fixed_ui::protectorate_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PW_PROTECTORATE");
		s.fixed_ui_text[fixed_ui::colonial_province_upgrade] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PW_COLONY_STATE");
		s.fixed_ui_text[fixed_ui::protectorate_province_upgrade] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PW_UPGRADE_TO_COLONY");
		s.fixed_ui_text[fixed_ui::province_view_admin] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PROVINCEVIEW_ADMIN");
		s.fixed_ui_text[fixed_ui::province_view_admin_base] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PROVINCEVIEW_ADMIN_BASE");
		s.fixed_ui_text[fixed_ui::province_view_admin_tech] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PROVINCEVIEW_ADMIN_TECH");
		s.fixed_ui_text[fixed_ui::province_view_admin_pops] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PROVINCEVIEW_ADMIN_POPS");
		s.fixed_ui_text[fixed_ui::controller] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PV_CONTROLLER");
		s.fixed_ui_text[fixed_ui::flashpoint_tension] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "FLASHPOINT_TENSION");
		s.fixed_ui_text[fixed_ui::has_no_flashpoint] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "HAS_NO_FLASHPOINT");
		s.fixed_ui_text[fixed_ui::province_liferating] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PROVINCEVIEW_LIFERATING");
		s.fixed_ui_text[fixed_ui::siege_progress] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PV_SIEGE_PROGRESS");
		s.fixed_ui_text[fixed_ui::owner_presence] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PROVINCEVIEW_RGOOWNER");
		s.fixed_ui_text[fixed_ui::other] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POP_OTHER_CULT");
		s.fixed_ui_text[fixed_ui::open_pop_screen] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PW_OPEN_POPSCREEN");
		s.fixed_ui_text[fixed_ui::colonial_investment_cost] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COLONY_INVEST_COST");
		s.fixed_ui_text[fixed_ui::province_view_state_non_cores] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "STATE_NONCORES");
		s.fixed_ui_text[fixed_ui::great_power] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "great_power");
		s.fixed_ui_text[fixed_ui::secondary_power] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "second_power");
		s.fixed_ui_text[fixed_ui::civilized_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "civ_nation");
		s.fixed_ui_text[fixed_ui::uncivilized_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "unciv_nation");
		s.fixed_ui_text[fixed_ui::partialy_civilized_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "DIPLOMACY_ALMOST_WESTERN_NATION_STATUS");
		s.fixed_ui_text[fixed_ui::prestige] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "RANK_PRESTIGE");
		s.fixed_ui_text[fixed_ui::military_power] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "RANK_MILITARY");
		s.fixed_ui_text[fixed_ui::industrial_power] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "RANK_INDUSTRY");
		s.fixed_ui_text[fixed_ui::is_mobilized] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "IS_MOBILIZED_IRO");
		s.fixed_ui_text[fixed_ui::sphere] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "DIPLOMACY_SPHERE_LABEL");
		s.fixed_ui_text[fixed_ui::sphere_leader] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "SPHERE_LEADER");
		s.fixed_ui_text[fixed_ui::puppets] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRY_PUPPETS_LIST");
		s.fixed_ui_text[fixed_ui::liege] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "TRIGGER_OVERLORD");
		s.fixed_ui_text[fixed_ui::fort] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PV_FORT");
		s.fixed_ui_text[fixed_ui::naval_base] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PV_NAVALBASE");
		s.fixed_ui_text[fixed_ui::railroad] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PV_RAILROAD");
		s.fixed_ui_text[fixed_ui::workforce] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "WORKFORCE_DISTTITLE");
		s.fixed_ui_text[fixed_ui::religion] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "RELIGION_DISTTITLE");
		s.fixed_ui_text[fixed_ui::ideology] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "IDEOLOGY_DISTTITLE");
		s.fixed_ui_text[fixed_ui::nationality] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "NATIONALITY_DISTTITLE");
		s.fixed_ui_text[fixed_ui::dominant_issues] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "DOMINANT_ISSUES_DISTTITLE");
		s.fixed_ui_text[fixed_ui::electorate] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "ELECTORATE_DISTTITLE");
		s.fixed_ui_text[fixed_ui::unemployment] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "UNEMPLOYMENT");
		s.fixed_ui_text[fixed_ui::life_needs] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "LIFE_NEEDS");
		s.fixed_ui_text[fixed_ui::everyday_needs] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "EVERYDAY_NEEDS");
		s.fixed_ui_text[fixed_ui::luxury_needs] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "LUXURY_NEEDS");
		s.fixed_ui_text[fixed_ui::no_closed_factories] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_HasClosedFactories");
		s.fixed_ui_text[fixed_ui::closed_factories] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_HasClosedFactories");
		s.fixed_ui_text[fixed_ui::unemployed_workers] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_HasUnemployedWorkers");
		s.fixed_ui_text[fixed_ui::no_unemployed_workers] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_HasUnemployedWorkers");
		s.fixed_ui_text[fixed_ui::building_factories] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_IsBuildingFactories");
		s.fixed_ui_text[fixed_ui::not_building_factories] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_IsBuildingFactories");
		s.fixed_ui_text[fixed_ui::no_research] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "TECHNOLOGYVIEW_NO_RESEARCH");
		s.fixed_ui_text[fixed_ui::optimal_is] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "PV_CLERGY_LIT_DESC2");
		s.fixed_ui_text[fixed_ui::can_reform] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_CanDoReforms");
		s.fixed_ui_text[fixed_ui::cannot_reform] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_CanDoReforms");
		s.fixed_ui_text[fixed_ui::has_decisions] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_CanDoDecisions");
		s.fixed_ui_text[fixed_ui::no_decisions] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "REMOVE_COUNTRYALERT_NO_CanDoDecisions");
		s.fixed_ui_text[fixed_ui::holding_election] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_IsInElection");
		s.fixed_ui_text[fixed_ui::no_election] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_IsInElection");
		s.fixed_ui_text[fixed_ui::has_rebels] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_HaveRebels");
		s.fixed_ui_text[fixed_ui::no_rebels] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_HaveRebels");
		s.fixed_ui_text[fixed_ui::is_not_gp] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_GPStatus");
		s.fixed_ui_text[fixed_ui::is_not_losing_gp] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_LoosingGPStatus");
		s.fixed_ui_text[fixed_ui::is_losing_gp] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_LoosingGPStatus");
		s.fixed_ui_text[fixed_ui::cannot_influence] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_CanIncreaseOpinion");
		s.fixed_ui_text[fixed_ui::can_influence] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_CanIncreaseOpinion");
		s.fixed_ui_text[fixed_ui::no_crisis] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_Crisis");
		s.fixed_ui_text[fixed_ui::has_crisis] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_Crisis");
		s.fixed_ui_text[fixed_ui::protectorate_pending] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_ColonialGood_protectorate");
		s.fixed_ui_text[fixed_ui::colony_pending] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_ColonialGood_colony");
		s.fixed_ui_text[fixed_ui::state_pending] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_ColonialGood_state");
		s.fixed_ui_text[fixed_ui::colonial_investment_pending] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_ColonialGood_invest");
		s.fixed_ui_text[fixed_ui::losing_colonial_race] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_ColonialBad_influence");
		s.fixed_ui_text[fixed_ui::no_colonial_activity] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "COUNTRYALERT_NO_Colonial");
		s.fixed_ui_text[fixed_ui::at_peace] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "ATPEACE");
		s.fixed_ui_text[fixed_ui::month_1] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "January");
		s.fixed_ui_text[fixed_ui::month_2] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "February");
		s.fixed_ui_text[fixed_ui::month_3] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "March");
		s.fixed_ui_text[fixed_ui::month_4] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "April");
		s.fixed_ui_text[fixed_ui::month_5] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "May");
		s.fixed_ui_text[fixed_ui::month_6] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "June");
		s.fixed_ui_text[fixed_ui::month_7] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "July");
		s.fixed_ui_text[fixed_ui::month_8] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "August");
		s.fixed_ui_text[fixed_ui::month_9] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "September");
		s.fixed_ui_text[fixed_ui::month_10] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "October");
		s.fixed_ui_text[fixed_ui::month_11] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "November");
		s.fixed_ui_text[fixed_ui::month_12] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "December");
		s.fixed_ui_text[fixed_ui::can_appoint_ruling_party] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "CAN_APPOINT_RULING_PARTY");
		s.fixed_ui_text[fixed_ui::next_election] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "NEXT_ELECTION");
		s.fixed_ui_text[fixed_ui::plurality] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "plurality");
		s.fixed_ui_text[fixed_ui::revanchism] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "revanchism");
		s.fixed_ui_text[fixed_ui::cannot_hold_election] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_CANNOT_HOLD_ELECTION_TOOLTIP");
		s.fixed_ui_text[fixed_ui::already_holding_election] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_ALREADY_HOLDING_ELECTION_TOOLTIP");
		s.fixed_ui_text[fixed_ui::can_social_reform] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_CAN_DO_SOCIAL_REFROMS");
		s.fixed_ui_text[fixed_ui::cannot_social_reform] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_CAN_NOT_DO_SOCIAL_REFROMS");
		s.fixed_ui_text[fixed_ui::can_social_revoke] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_CAN_REVOKE_SOCIAL_REFROMS");
		s.fixed_ui_text[fixed_ui::can_political_reform] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_CAN_DO_POLITICAL_REFROMS");
		s.fixed_ui_text[fixed_ui::cannot_political_reform] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_CAN_NOT_DO_POLITICAL_REFROMS");
		s.fixed_ui_text[fixed_ui::can_political_revoke] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "POLITICS_CAN_REVOKE_POLITICAL_REFROMS");
		s.fixed_ui_text[fixed_ui::every] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "EVERY");
		s.fixed_ui_text[fixed_ui::any] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "ANY");
		s.fixed_ui_text[fixed_ui::random] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "RANDOM");
		s.fixed_ui_text[fixed_ui::neighboring_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "neighboring_province");
		s.fixed_ui_text[fixed_ui::where] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "WHERE");
		s.fixed_ui_text[fixed_ui::neighboring_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "neighboring_nation");
		s.fixed_ui_text[fixed_ui::nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "NATION");
		s.fixed_ui_text[fixed_ui::empty_neighboring_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "empty_neighboring_province");
		s.fixed_ui_text[fixed_ui::great_power] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "great_power");
		s.fixed_ui_text[fixed_ui::poor_strata_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "poor_strata_pop");
		s.fixed_ui_text[fixed_ui::middle_strata_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "middle_strata_pop");
		s.fixed_ui_text[fixed_ui::rich_strata_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "rich_strata_pop");
		s.fixed_ui_text[fixed_ui::pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "pop");
		s.fixed_ui_text[fixed_ui::owned_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "owned_province");
		s.fixed_ui_text[fixed_ui::singular_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "singular_nation");
		s.fixed_ui_text[fixed_ui::singular_state] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "singular_state");
		s.fixed_ui_text[fixed_ui::singular_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "singular_province");
		s.fixed_ui_text[fixed_ui::singular_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "singular_pop");
		s.fixed_ui_text[fixed_ui::core_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "core_of");
		s.fixed_ui_text[fixed_ui::state_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "state_of");
		s.fixed_ui_text[fixed_ui::one_of_the_following] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "one_of_the_following");
		s.fixed_ui_text[fixed_ui::chance_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "chance_of");
		s.fixed_ui_text[fixed_ui::owner_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "owner_of");
		s.fixed_ui_text[fixed_ui::controller_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "controller_of");
		s.fixed_ui_text[fixed_ui::location_of_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "location_of_pop");
		s.fixed_ui_text[fixed_ui::nation_of_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "nation_of_pop");
		s.fixed_ui_text[fixed_ui::capital_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "capital_of");
		s.fixed_ui_text[fixed_ui::this_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "this_nation");
		s.fixed_ui_text[fixed_ui::this_state] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "this_state");
		s.fixed_ui_text[fixed_ui::this_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "this_province");
		s.fixed_ui_text[fixed_ui::this_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "this_pop");
		s.fixed_ui_text[fixed_ui::from_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "from_nation");
		s.fixed_ui_text[fixed_ui::from_state] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "from_state");
		s.fixed_ui_text[fixed_ui::from_province] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "from_province");
		s.fixed_ui_text[fixed_ui::from_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "from_pop");
		s.fixed_ui_text[fixed_ui::adjacent_sea] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "adjacent_sea");
		s.fixed_ui_text[fixed_ui::cultural_union_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "cultural_union_of");

		s.fixed_ui_text[fixed_ui::overlord_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "overlord_of");
		s.fixed_ui_text[fixed_ui::sphere_leader_of] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "sphere_leader_of");
		s.fixed_ui_text[fixed_ui::crisis_state] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "crisis_state");
		s.fixed_ui_text[fixed_ui::containing_state] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "containing_state");
		s.fixed_ui_text[fixed_ui::province_in] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "province_in");
		s.fixed_ui_text[fixed_ui::reb_independence_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "reb_independence_nation");
		s.fixed_ui_text[fixed_ui::flashpoint_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "flashpoint_nation");
		s.fixed_ui_text[fixed_ui::no_effect] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "NO_EFFECT");
		s.fixed_ui_text[fixed_ui::move_capital_to] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "move_capital_to");
		s.fixed_ui_text[fixed_ui::add_x_core] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "add_x_core");
		s.fixed_ui_text[fixed_ui::add_core_to] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "add_core_to");
		s.fixed_ui_text[fixed_ui::rebel] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "rebel");
		s.fixed_ui_text[fixed_ui::remove_x_core] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "remove_x_core");
		s.fixed_ui_text[fixed_ui::remove_core_from] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "remove_core_from");
		s.fixed_ui_text[fixed_ui::change_name_to] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "change_name_to");
		s.fixed_ui_text[fixed_ui::change_rgo_production_to] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "change_rgo_production_to");
		s.fixed_ui_text[fixed_ui::make_accepted_culture] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "make_accepted_culture");
		s.fixed_ui_text[fixed_ui::primary_culture_changes_to] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "primary_culture_changes_to");
		s.fixed_ui_text[fixed_ui::union_culture_accepted] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "union_culture_accepted");
		s.fixed_ui_text[fixed_ui::this_nation_culture] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "this_nation_culture");
		s.fixed_ui_text[fixed_ui::this_nation_culture] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "from_nation_culture");
		s.fixed_ui_text[fixed_ui::remove_accepted_culture] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "remove_accepted_culture");
		s.fixed_ui_text[fixed_ui::life_rating] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "life_rating");
		s.fixed_ui_text[fixed_ui::make_national_religion] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "make_national_religion");
		s.fixed_ui_text[fixed_ui::make_slave_state] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "make_slave_state");
		s.fixed_ui_text[fixed_ui::make_slave_pop] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "make_slave_pop");
		s.fixed_ui_text[fixed_ui::research_points] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "research_points");
		s.fixed_ui_text[fixed_ui::change_tech_school] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "change_tech_school");
		s.fixed_ui_text[fixed_ui::change_government_to] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "change_government_to");
		s.fixed_ui_text[fixed_ui::add_to_treasury] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "add_to_treasury");
		s.fixed_ui_text[fixed_ui::war_exhaustion] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "war_exhaustion");
		s.fixed_ui_text[fixed_ui::become_blank] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "become_blank");
		s.fixed_ui_text[fixed_ui::cultural_union_nation] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "cultural_union_nation");
		s.fixed_ui_text[fixed_ui::player_control_change] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "player_control_change");
		s.fixed_ui_text[fixed_ui::set_national_flag] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "set_national_flag");
		s.fixed_ui_text[fixed_ui::remove_national_flag] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "remove_national_flag");
		s.fixed_ui_text[fixed_ui::infamy] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "infamy");
		s.fixed_ui_text[fixed_ui::change_province_owner] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "change_province_owner");
		s.fixed_ui_text[fixed_ui::annex_effect] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "annex_effect");
		s.fixed_ui_text[fixed_ui::annexed_by_effect] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "annexed_by_effect");
	}
}
