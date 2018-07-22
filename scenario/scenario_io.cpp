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
	}
}
