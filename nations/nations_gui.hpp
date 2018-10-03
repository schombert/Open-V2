#pragma once
#include "common\\common.h"
#include "nations_gui.h"
#include "gui\\gui.hpp"
#include "world_state\\world_state.h"
#include "military\\military_functions.h"
#include "nations_functions.h"

namespace nations {
	class flag_item_base : public ui::visible_region {
	public:
		nations::country_tag c;
		flag_item_base(nations::country_tag t) : c(t) {}
	};

	class flag_sub_item {
	public:
		flag_sub_item(nations::country_tag t) {}
		template<typename W>
		void windowed_update(ui::masked_flag<flag_sub_item>& self, W& w, world_state& ws);
	};

	using flag_item = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<flag_sub_item>,
		flag_item_base
	>;

	class gp_display_base : public ui::window_pane {
	public:
		int32_t gp_index = 0;

		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class gp_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gp_flag {
	public:
		template<typename window_type>
		void windowed_update(ui::masked_flag<gp_flag>& self, window_type& win, world_state& ws);
	};

	class gp_prestige {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gp_military {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gp_industry {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gp_overall {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gp_sphere {
	public:
		template<typename lb_type, typename window_type>
		void windowed_update(lb_type& lb, window_type& win, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using gp_display = ui::gui_window<
		CT_STRING("country_name"), ui::display_text<gp_name>,
		CT_STRING("country_flag"), ui::masked_flag<gp_flag>,
		CT_STRING("country_puppets"), ui::overlap_box<gp_sphere, ui::window_tag, flag_item>,
		CT_STRING("gp_prestige"), ui::display_text<gp_prestige, -8>,
		CT_STRING("gp_economic"), ui::display_text<gp_industry, -8>,
		CT_STRING("gp_military"), ui::display_text<gp_military, -8>,
		CT_STRING("gp_total"), ui::display_text<gp_overall, -8>,
		gp_display_base
	>;

	class diplomacy_window_base : public ui::draggable_region {
	public:
		gp_display gp_items[8];
		
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class close_button {
	public:
		void button_function(ui::simple_button<close_button>&, world_state&);
	};

	class diplomacy_tab_button_group {
	public:
		void on_select(world_state& ws, uint32_t i);
	};

	class war_display_base : public ui::visible_region {
	public:
		military::war_tag war;
		void set_value(military::war_tag t) {
			war = t;
		}
		template<typename window_type>
		void on_create(window_type& w, world_state& ws);
	};

	class war_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class attacker_strength {
	public:
		military::war_tag war;
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(war); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class defender_strength {
	public:
		military::war_tag war;
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(war); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class war_score_progress_bar {
	public:
		template<typename window_type>
		void windowed_update(ui::progress_bar<war_score_progress_bar>& bar, window_type& win, world_state& ws);
	};

	class war_score_overlay {
	public:
		military::war_tag war;
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<war_score_overlay>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(war); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class attacker_wg_marker {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<attacker_wg_marker>& self, window_type& win, world_state& ws);
	};

	class defender_wg_marker {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<defender_wg_marker>& self, window_type& win, world_state& ws);
	};

	class war_score_text_box {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class join_attacker_button {
	public:
		military::war_tag war;
		template<typename window_type>
		void windowed_update(ui::simple_button<join_attacker_button>&, window_type& win, world_state&);
		void button_function(ui::simple_button<join_attacker_button>&, world_state&);
		void on_create(ui::simple_button<join_attacker_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(war); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class join_defender_button {
	public:
		military::war_tag war;
		template<typename window_type>
		void windowed_update(ui::simple_button<join_defender_button>&, window_type& win, world_state&);
		void button_function(ui::simple_button<join_defender_button>&, world_state&);
		void on_create(ui::simple_button<join_defender_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(war); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	

	class wg_item_base : public ui::visible_region {
	public:
		military::war_goal this_wg;
		wg_item_base(military::war_goal t) : this_wg(t) {}
	};

	class wg_sub_item {
	public:
		military::war_goal this_wg;
		wg_sub_item(military::war_goal t) {}
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<wg_sub_item>&, window_type& win, world_state&);
		bool has_tooltip(world_state&) { return is_valid_index(this_wg.cb_type); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	

	using wg_item = ui::gui_window<
		CT_STRING("wargoal_icon"), ui::dynamic_icon<wg_sub_item>,
		wg_item_base
	>;

	class defenders_lb {
	public:
		template<typename lb_type, typename window_type>
		void windowed_update(lb_type& lb, window_type& win, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class attackers_lb {
	public:
		template<typename lb_type, typename window_type>
		void windowed_update(lb_type& lb, window_type& win, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class defenders_wg_lb {
	public:
		template<typename lb_type, typename window_type>
		void windowed_update(lb_type& lb, window_type& win, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class attackers_wg_lb {
	public:
		template<typename lb_type, typename window_type>
		void windowed_update(lb_type& lb, window_type& win, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using war_display = ui::gui_window<
		CT_STRING("war_name"), ui::display_text<war_name>,
		CT_STRING("attackers_mil_strength"), ui::display_text<attacker_strength, -6>,
		CT_STRING("defenders_mil_strength"), ui::display_text<defender_strength, -6>,
		CT_STRING("warscore"), ui::progress_bar<war_score_progress_bar>,
		CT_STRING("diplo_war_progress_overlay"), ui::dynamic_icon<war_score_overlay>,
		CT_STRING("diplo_warscore_marker1"), ui::dynamic_icon<attacker_wg_marker>,
		CT_STRING("diplo_warscore_marker2"), ui::dynamic_icon<defender_wg_marker>,
		CT_STRING("warscore_text"), ui::display_text<war_score_text_box, -2>,
		CT_STRING("defenders"), ui::overlap_box<defenders_lb, ui::window_tag, flag_item>,
		CT_STRING("attackers"), ui::overlap_box<attackers_lb, ui::window_tag, flag_item>,
		CT_STRING("defenders_wargoals"), ui::overlap_box<defenders_wg_lb, ui::window_tag, wg_item>,
		CT_STRING("attackers_wargoals"), ui::overlap_box<attackers_wg_lb, ui::window_tag, wg_item>,
		CT_STRING("join_attackers"), ui::simple_button<join_attacker_button>,
		CT_STRING("join_defenders"), ui::simple_button<join_defender_button>,
		war_display_base
	>;

	class war_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class crisis_pane_base : public ui::window_pane {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class crisis_name {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class crisis_subtitle {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class crisis_status {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class crisis_temperature_bar {
	public:
		void update(ui::progress_bar<crisis_temperature_bar>& bar, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(ui::progress_bar<crisis_temperature_bar>& bar, world_state& ws, ui::tagged_gui_object tw);
	};

	class crisis_attacker_defender_pane_base : public ui::window_pane {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class crisis_attacker_gp_flag {
	public:
		void update(ui::masked_flag<crisis_attacker_gp_flag>& self, world_state& ws);
	};

	class crisis_attacker_primary_flag {
	public:
		template<typename window_type>
		void windowed_update(ui::masked_flag<crisis_attacker_primary_flag>& self, window_type& win, world_state& ws);
	};

	class crisis_attacker_country_name {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class crisis_attacker_wg_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class crisis_attacker_backer_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class crisis_support_attacker_button {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<crisis_support_attacker_button>&, window_type& win, world_state&);
		void button_function(ui::simple_button<crisis_support_attacker_button>&, world_state&);
	};

	class crisis_attacker_back_down_button {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<crisis_attacker_back_down_button>&, window_type& win, world_state&);
		void button_function(ui::simple_button<crisis_attacker_back_down_button>&, world_state&);
	};

	using crisis_attacker_pane = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<crisis_attacker_gp_flag>,
		CT_STRING("sponsored_flag"), ui::masked_flag<crisis_attacker_primary_flag>,
		CT_STRING("country_name"), ui::display_text<crisis_attacker_country_name>,
		CT_STRING("wargoals"), ui::overlap_box<crisis_attacker_wg_lb, ui::window_tag, wg_item>,
		CT_STRING("backers"), ui::overlap_box<crisis_attacker_backer_lb, ui::window_tag, flag_item>,
		CT_STRING("support_side"), ui::simple_button<crisis_support_attacker_button>,
		CT_STRING("back_down"), ui::simple_button<crisis_attacker_back_down_button>,
		crisis_attacker_defender_pane_base
	>;

	class crisis_defender_gp_flag {
	public:
		void update(ui::masked_flag<crisis_defender_gp_flag>& self, world_state& ws);
	};

	class crisis_defender_primary_flag {
	public:
		template<typename window_type>
		void windowed_update(ui::masked_flag<crisis_defender_primary_flag>& self, window_type& win, world_state& ws);
	};

	class crisis_defender_country_name {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class crisis_defender_wg_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class crisis_defender_backer_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class crisis_support_defender_button {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<crisis_support_defender_button>&, window_type& win, world_state&);
		void button_function(ui::simple_button<crisis_support_defender_button>&, world_state&);
	};

	class crisis_defender_back_down_button {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<crisis_defender_back_down_button>&, window_type& win, world_state&);
		void button_function(ui::simple_button<crisis_defender_back_down_button>&, world_state&);
	};

	using crisis_defender_pane = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<crisis_defender_gp_flag>,
		CT_STRING("sponsored_flag"), ui::masked_flag<crisis_defender_primary_flag>,
		CT_STRING("country_name"), ui::display_text<crisis_defender_country_name>,
		CT_STRING("wargoals"), ui::overlap_box<crisis_defender_wg_lb, ui::window_tag, wg_item>,
		CT_STRING("backers"), ui::overlap_box<crisis_defender_backer_lb, ui::window_tag, flag_item>,
		CT_STRING("support_side"), ui::simple_button<crisis_support_defender_button>,
		CT_STRING("back_down"), ui::simple_button<crisis_defender_back_down_button>,
		crisis_attacker_defender_pane_base
	>;

	class undecided_gp_entry_base : public ui::gui_behavior {
	public:
		nations::country_tag tag;
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class undecided_gp_flag {
	public:
		template<typename window_type>
		void windowed_update(ui::masked_flag<undecided_gp_flag>& self, window_type& win, world_state& ws);
	};

	class make_offer_button {
	public:
		nations::country_tag tag;
		template<typename window_type>
		void windowed_update(ui::simple_button<make_offer_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<make_offer_button>&, world_state&);
		bool has_tooltip(world_state&) { return is_valid_index(tag); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using undecided_gp_entry = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<undecided_gp_flag>,
		CT_STRING("make_offer"), ui::simple_button<make_offer_button>,
		undecided_gp_entry_base
	>;

	class undecided_pane_base : public ui::window_pane {
	public:
		undecided_gp_entry undecided_nations[8];

		template<typename W>
		void on_create(W& w, world_state&);
		template<typename W>
		void windowed_update(W& w, world_state&);
	};

	using undecided_pane = ui::gui_window<undecided_pane_base>;

	using crisis_pane = ui::gui_window<
		CT_STRING("crisis_title"), ui::display_text<crisis_name>,
		CT_STRING("crisis_sub_title"), ui::display_text<crisis_subtitle>,
		CT_STRING("crisis_temperature"), ui::progress_bar<crisis_temperature_bar>,
		CT_STRING("crisis_status_label"), ui::display_text<crisis_status>,
		CT_STRING("attacker_win"), crisis_attacker_pane,
		CT_STRING("defender_win"), crisis_defender_pane,
		CT_STRING("fence_sitters_win"), undecided_pane,
		crisis_pane_base
	>;

	class filter_all_button {
	public:
		void update(ui::simple_button<filter_all_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_all_button>&, world_state&);
	};

	class filter_north_america_button {
	public:
		void update(ui::simple_button<filter_north_america_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_north_america_button>&, world_state&);
	};

	class filter_south_america_button {
	public:
		void update(ui::simple_button<filter_south_america_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_south_america_button>&, world_state&);
	};

	class filter_europe_button {
	public:
		void update(ui::simple_button<filter_europe_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_europe_button>&, world_state& ws);
	};

	class filter_africa_button {
	public:
		void update(ui::simple_button<filter_africa_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_africa_button>&, world_state& ws);
	};

	class filter_asia_button {
	public:
		void update(ui::simple_button<filter_asia_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_asia_button>&, world_state& ws);
	};

	class filter_oceania_button {
	public:
		void update(ui::simple_button<filter_oceania_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_oceania_button>&, world_state& ws);
	};

	class filter_neighbours_button {
	public:
		void update(ui::simple_button<filter_neighbours_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_neighbours_button>&, world_state& ws);
	};

	class filter_sphere_button {
	public:
		void update(ui::simple_button<filter_sphere_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_sphere_button>&, world_state& ws);
	};
	class filter_enemies_button {
	public:
		void update(ui::simple_button<filter_enemies_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_enemies_button>&, world_state& ws);
	};
	class filter_allies_button {
	public:
		void update(ui::simple_button<filter_allies_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_allies_button>&, world_state& ws);
	};

	class sort_by_country_button {
	public:
		void button_function(ui::simple_button<sort_by_country_button>&, world_state& ws);
	};
	class sort_by_prio_button {
	public:
		void button_function(ui::simple_button<sort_by_prio_button>&, world_state& ws);
	};

	class clear_text_button {
	public:
		void on_create(ui::simple_button<clear_text_button>&, world_state&);
	};

	class sort_by_gpflag0_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag0_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag0_button>&, world_state& ws);
	};
	class sort_by_gpflag1_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag1_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag1_button>&, world_state& ws);
	};
	class sort_by_gpflag2_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag2_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag2_button>&, world_state& ws);
	};
	class sort_by_gpflag3_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag3_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag3_button>&, world_state& ws);
	};
	class sort_by_gpflag4_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag4_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag4_button>&, world_state& ws);
	};
	class sort_by_gpflag5_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag5_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag5_button>&, world_state& ws);
	};
	class sort_by_gpflag6_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag6_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag6_button>&, world_state& ws);
	};
	class sort_by_gpflag7_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag7_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag7_button>&, world_state& ws);
	};
	class sort_by_boss_button {
	public:
		void button_function(ui::simple_button<sort_by_boss_button>&, world_state& ws);
	};
	class sort_by_prestige_button {
	public:
		void button_function(ui::simple_button<sort_by_prestige_button>&, world_state& ws);
	};
	class sort_by_economic_button {
	public:
		void button_function(ui::simple_button<sort_by_economic_button>&, world_state& ws);
	};
	class sort_by_military_button {
	public:
		void button_function(ui::simple_button<sort_by_military_button>&, world_state& ws);
	};
	class sort_by_total_button {
	public:
		void button_function(ui::simple_button<sort_by_total_button>&, world_state& ws);
	};
	class sort_by_opinion_button {
	public:
		void button_function(ui::simple_button<sort_by_opinion_button>&, world_state& ws);
	};
	class sort_by_relation_button {
	public:
		void button_function(ui::simple_button<sort_by_relation_button>&, world_state& ws);
	};


	class nation_details_item_base : public ui::visible_region {
	public:
		nations::country_tag tag;
		void set_value(nations::country_tag t) {
			tag = t;
		}
		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class nation_details_background_button {
	public:
		nations::country_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<nation_details_background_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<nation_details_background_button>& self, world_state& ws);
	};

	class nation_details_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<nation_details_flag>& self, W& w, world_state& ws);
	};

	class nation_details_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_details_priority_button {
	public:
		nations::country_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<nation_details_priority_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<nation_details_priority_button>& self, world_state& ws);
	};

	template<int32_t index>
	class gp_influence_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_details_sphere_leader_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<nation_details_sphere_leader_flag>& self, W& w, world_state& ws);
	};

	class nation_details_military_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_details_economic_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_details_prestige_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_details_overall_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_details_opinion_type {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_details_relations_value {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using nation_details_item = ui::gui_window <
		CT_STRING("country_select"), ui::simple_button<nation_details_background_button>,
		CT_STRING("country_flag"), ui::masked_flag<nation_details_flag>,
		CT_STRING("country_name"), ui::display_text<nation_details_name>,
		CT_STRING("country_prio"), ui::simple_button<nation_details_priority_button>,
		CT_STRING("country_gp0"), ui::display_text<gp_influence_text<0>>,
		CT_STRING("country_gp1"), ui::display_text<gp_influence_text<1>>,
		CT_STRING("country_gp2"), ui::display_text<gp_influence_text<2>>,
		CT_STRING("country_gp3"), ui::display_text<gp_influence_text<3>>,
		CT_STRING("country_gp4"), ui::display_text<gp_influence_text<4>>,
		CT_STRING("country_gp5"), ui::display_text<gp_influence_text<5>>,
		CT_STRING("country_gp6"), ui::display_text<gp_influence_text<6>>,
		CT_STRING("country_gp7"), ui::display_text<gp_influence_text<7>>,
		CT_STRING("country_boss_flag"), ui::masked_flag<nation_details_sphere_leader_flag>,
		CT_STRING("country_prestige"), ui::display_text<nation_details_prestige_rank>,
		CT_STRING("country_economic"), ui::display_text<nation_details_economic_rank>,
		CT_STRING("country_military"), ui::display_text<nation_details_military_rank>,
		CT_STRING("country_total"), ui::display_text<nation_details_overall_rank>,
		CT_STRING("country_opinion"), ui::display_text<nation_details_opinion_type>,
		CT_STRING("country_relation"), ui::display_text<nation_details_relations_value>,
		nation_details_item_base
	> ;

	class nations_details_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class cb_item_base : public ui::visible_region {
	public:
		nations::country_tag tag;
		flag_item cb_fabrication_by;
		flag_item cb_fabrication_target;
		
		cb_item_base() : cb_fabrication_by(nations::country_tag()), cb_fabrication_target(nations::country_tag()) {}

		void set_value(nations::country_tag t) {
			tag = t;
		}
		template<typename W>
		void on_create(W& w, world_state& ws);

		void update(world_state& ws);
	};

	class cb_item_icon {
	public:
		military::cb_type_tag tag;
		template<typename W>
		void windowed_update(ui::dynamic_icon<cb_item_icon>& self, W& w, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(tag); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class cb_item_progress {
	public:
		template<typename W>
		void windowed_update(ui::progress_bar<cb_item_progress>& self, W& w, world_state& ws);
	};

	class cb_item_progress_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class cb_item_cancel_button {
	public:
		nations::country_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<cb_item_cancel_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<cb_item_cancel_button>& self, world_state& ws);
	};

	using cb_item = ui::gui_window <
		CT_STRING("cb_type_icon"), ui::dynamic_icon<cb_item_icon>,
		CT_STRING("cb_progress"), ui::progress_bar<cb_item_progress>,
		CT_STRING("cb_progress_text"), ui::display_text<cb_item_progress_text>,
		CT_STRING("cancel"), ui::simple_button<cb_item_cancel_button>,
		cb_item_base
	>;

	class cb_fabrication_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using cb_pane = ui::gui_window<
		CT_STRING("cb_listbox"), ui::discrete_listbox<cb_fabrication_lb, cb_item, nations::country_tag>,
		ui::window_pane
	>;

	class generic_influence_action_button {
	public:
		int32_t gp_index = 0;

		void button_function(ui::simple_button<generic_influence_action_button>&, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class influence_details_base : public ui::visible_region {
	public:
		int32_t gp_index = 0;

		ui::simple_button<generic_influence_action_button> influence_button;

		template<typename W>
		void on_create(W& w, world_state&);
		void update(world_state& ws);
	};

	class influence_details_gp_flag {
	public:
		template<typename window_type>
		void windowed_update(ui::masked_flag<influence_details_gp_flag>& self, window_type& win, world_state& ws);
	};

	class influence_details_opinion {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class influence_details_influence {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class influence_details_investment {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class influence_details_discredited_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<influence_details_discredited_icon>& self, window_type& win, world_state& ws);
	};

	class influence_details_banned_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<influence_details_banned_icon>& self, window_type& win, world_state& ws);
	};

	using influence_details = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<influence_details_gp_flag>,
		CT_STRING("nongp_country_opinion"), ui::display_text<influence_details_opinion>,
		CT_STRING("nongp_country_influence"), ui::display_text<influence_details_influence>,
		CT_STRING("nongp_country_invest"), ui::display_text<influence_details_investment>,
		CT_STRING("country_discredited"), ui::dynamic_icon<influence_details_discredited_icon>,
		CT_STRING("country_banned_embassy"), ui::dynamic_icon<influence_details_banned_icon>,
		influence_details_base
	>;

	class influence_details_window_container : public ui::visible_region {
	public:
		influence_details influence[8];

		template<typename W>
		void on_create(W& w, world_state&);
		template<typename W>
		void windowed_update(W& w, world_state& ws);
	};

	using influence_details_window_container_t = ui::gui_window<
		influence_details_window_container
	>;

	class increase_relations_button {
	public:
		void button_function(ui::simple_button<increase_relations_button>&, world_state&);
		void update(ui::simple_button<increase_relations_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class decrease_relations_button {
	public:
		void button_function(ui::simple_button<decrease_relations_button>&, world_state&);
		void update(ui::simple_button<decrease_relations_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class make_break_alliance_button {
	public:
		void button_function(ui::button<make_break_alliance_button>&, world_state&);
		void update(ui::button<make_break_alliance_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class call_ally_button {
	public:
		void button_function(ui::button<call_ally_button>&, world_state&);
		void update(ui::button<call_ally_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class expel_advisors_button{
	public:
		void button_function(ui::button<expel_advisors_button>&, world_state&);
		void update(ui::button<expel_advisors_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class ban_embassy_button {
	public:
		void button_function(ui::button<ban_embassy_button>&, world_state&);
		void update(ui::button<ban_embassy_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class justify_war_button {
	public:
		void button_function(ui::button<justify_war_button>&, world_state&);
		void update(ui::button<justify_war_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class declare_war_offer_peace_button {
	public:
		void button_function(ui::button<declare_war_offer_peace_button>&, world_state&);
		void update(ui::button<declare_war_offer_peace_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_base : public ui::window_pane {
	public:
		ui::simple_button<increase_relations_button> increase_relations;
		ui::simple_button<decrease_relations_button> decrease_relations;

		ui::button<make_break_alliance_button> make_break_alliance;
		ui::button<call_ally_button> call_ally;
		ui::button<expel_advisors_button> expel_advisors;
		ui::button<ban_embassy_button> ban_embassy;
		ui::button<justify_war_button> justify_war;
		ui::button<declare_war_offer_peace_button> declare_war_offer_peace;

		template<typename W>
		void on_create(W& w, world_state&);
		template<typename W>
		void windowed_update(W& w, world_state&);
	};

	class details_flag {
	public:
		void update(ui::masked_flag<details_flag>& self, world_state& ws);
	};

	class details_flag_frame {
	public:
		void update(ui::dynamic_transparent_icon<details_flag_frame>& ico, world_state& ws);
	};

	class details_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_name {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_relation_value {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_rank_status {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_government_type {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_party {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_party_icon {
	public:
		void update(ui::tinted_icon<details_party_icon>& self, world_state& ws);
	};

	class details_tech_school {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_prestige {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_industrial_score {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_military_score {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_total_score {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_prestige_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_industrial_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_military_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_overall_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_population_value {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_primary_culture {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_accepted_cultures {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class details_gp_sphere {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class details_gp_friendly {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class details_gp_coridal {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class details_non_gp_background {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<details_non_gp_background>& self, W& win, world_state& ws);
		bool has_tooltip(world_state&) { return false; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw) {}
	};

	class details_placeholder {
	};

	class details_infamy_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_infamy_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_war_exhaustion_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_war_exhaustion_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_brigade_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_brigade_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_ships_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_ships_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_add_wg_button {
	public:
		void update(ui::simple_button<details_add_wg_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class details_wars {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class details_allies {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class details_protected {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class flag_truce_item_base : public ui::visible_region {
	public:
		nations::country_tag c;
		date_tag until;
		flag_truce_item_base(nations::truce t) : c(t.tag), until(t.until) {}
	};

	class flag_truce_sub_item {
	public:
		nations::country_tag c;
		date_tag until;

		flag_truce_sub_item(nations::truce t) {}
		template<typename W>
		void windowed_update(ui::masked_flag<flag_truce_sub_item>& self, W& w, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(c); }
		void create_tooltip(ui::masked_flag<flag_truce_sub_item>& self, world_state& ws, ui::tagged_gui_object tw);
	};

	using flag_truce_item = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<flag_truce_sub_item>,
		flag_truce_item_base
	>;

	class details_truce {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class details_cb_item_base : public ui::visible_region {
	public:
		military::pending_cb this_cb;

		details_cb_item_base(military::pending_cb t) : this_cb(t) {}
	};

	class details_cb_sub_item {
	public:
		military::pending_cb this_cb;

		details_cb_sub_item(military::pending_cb t) {}
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<details_cb_sub_item>&, window_type& win, world_state&);
		bool has_tooltip(world_state&) { return is_valid_index(this_cb.type); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using details_cb_item = ui::gui_window<
		CT_STRING("wargoal_icon"), ui::dynamic_icon<details_cb_sub_item>,
		details_cb_item_base
	>;

	class details_cbs {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class details_wargoals {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using details_pane = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<details_flag>,
		CT_STRING("country_flag_overlay"), ui::dynamic_transparent_icon<details_flag_frame>,
		CT_STRING("selected_nation_totalrank"), ui::display_text<details_rank, -16>,
		CT_STRING("country_name"), ui::display_text<details_name>,
		CT_STRING("our_relation"), ui::display_text<details_relation_value>,
		CT_STRING("country_status"), ui::display_text<details_rank_status, -4>,
		CT_STRING("country_gov"), ui::display_text<details_government_type, -4>,
		CT_STRING("ideology_icon"), ui::tinted_icon<details_party_icon>,
		CT_STRING("country_party"), ui::display_text<details_party, -4>,
		CT_STRING("country_tech"), ui::display_text<details_tech_school, -4>,
		CT_STRING("country_prestige"), ui::display_text<details_prestige, -4>,
		CT_STRING("selected_prestige_rank"), ui::display_text<details_prestige_rank, -14>,
		CT_STRING("country_economic"), ui::display_text<details_industrial_score, -4>,
		CT_STRING("selected_industry_rank"), ui::display_text<details_industrial_rank, -14>,
		CT_STRING("country_military"), ui::display_text<details_military_score, -4>,
		CT_STRING("selected_military_rank"), ui::display_text<details_military_rank, -14>,
		CT_STRING("country_total"), ui::display_text<details_total_score, -4>,
		CT_STRING("selected_total_rank"), ui::display_text<details_overall_rank, -14>,
		CT_STRING("country_population"), ui::display_text<details_population_value>,
		CT_STRING("country_primary_cultures"), ui::display_text<details_primary_culture>,
		CT_STRING("country_accepted_cultures"), ui::display_text<details_accepted_cultures>,
		CT_STRING("country_GP_friendly"), ui::overlap_box<details_gp_friendly, ui::window_tag, flag_item>,
		CT_STRING("country_GP_cordial"), ui::overlap_box<details_gp_coridal, ui::window_tag, flag_item>,
		CT_STRING("country_GP_sphere"), ui::overlap_box<details_gp_sphere, ui::window_tag, flag_item>,
		CT_STRING("nongp_extra_info_bg"), influence_details_window_container_t,

		//moves start here
		CT_STRING("war_extra_info_bg"), ui::dynamic_icon<details_placeholder>,
		CT_STRING("country_wars"), ui::overlap_box<details_wars, ui::window_tag, flag_item>,
		CT_STRING("country_wars_text"), ui::fixed_text<details_placeholder>,
		CT_STRING("country_allies"), ui::overlap_box<details_allies, ui::window_tag, flag_item>,
		CT_STRING("country_allies_text"), ui::fixed_text<details_placeholder>,
		CT_STRING("country_protected"), ui::overlap_box<details_protected, ui::window_tag, flag_item>,
		CT_STRING("country_protected_text"), ui::fixed_text<details_placeholder>,
		CT_STRING("country_truce"), ui::overlap_box<details_truce, ui::window_tag, flag_truce_item>,
		CT_STRING("country_truce_text"), ui::fixed_text<details_placeholder>,
		CT_STRING("country_cb"), ui::overlap_box<details_cbs, ui::window_tag, details_cb_item>,
		CT_STRING("country_cb_text"), ui::fixed_text<details_placeholder>,
		CT_STRING("infamy_icon"), ui::dynamic_icon<details_infamy_icon>,
		CT_STRING("warexhastion_icon"), ui::dynamic_icon<details_war_exhaustion_icon>,
		CT_STRING("brigade_icon"), ui::dynamic_icon<details_brigade_icon>,
		CT_STRING("ships_icon"), ui::dynamic_icon<details_ships_icon>,
		CT_STRING("infamy_text"), ui::display_text<details_infamy_text>,
		CT_STRING("warexhastion_text"), ui::display_text<details_war_exhaustion_text>,
		CT_STRING("brigade_text"), ui::display_text<details_brigade_text>,
		CT_STRING("ships_text"), ui::display_text<details_ships_text>,
		CT_STRING("current_wargoals"), ui::overlap_box<details_wargoals, ui::window_tag, wg_item>,
		CT_STRING("add_wargoal"), ui::simple_button<details_add_wg_button>,
		details_base
	>;

	class diplomacy_window_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<close_button>,
		CT_STRING("gp_info"), ui::button_group_member,
		CT_STRING("war_info"), ui::button_group_member,
		CT_STRING("cb_info"), ui::button_group_member,
		CT_STRING("crisis_info"), ui::button_group_member,
		CT_STRING("diplomacy_tab_button_group"), ui::button_group<
		CT_STRING("gp_info"),
		CT_STRING("war_info"),
		CT_STRING("cb_info"),
		CT_STRING("crisis_info"), diplomacy_tab_button_group>,
		CT_STRING("war_listbox"), ui::discrete_listbox<war_lb, war_display, military::war_tag>,
		CT_STRING("cb_info_win"), cb_pane,
		CT_STRING("crisis_info_win"), crisis_pane,
		CT_STRING("filter_all"), ui::simple_button<filter_all_button>,
		CT_STRING("filter_north_america"), ui::simple_button<filter_north_america_button>,
		CT_STRING("filter_south_america"), ui::simple_button<filter_south_america_button>,
		CT_STRING("filter_europe"), ui::simple_button<filter_europe_button>,
		CT_STRING("filter_africa"), ui::simple_button<filter_africa_button>,
		CT_STRING("filter_asia"), ui::simple_button<filter_asia_button>,
		CT_STRING("filter_oceania"), ui::simple_button<filter_oceania_button>,
		CT_STRING("filter_neighbours"), ui::simple_button<filter_neighbours_button>,
		CT_STRING("filter_sphere"), ui::simple_button<filter_sphere_button>,
		CT_STRING("filter_enemies"), ui::simple_button<filter_enemies_button>,
		CT_STRING("filter_allies"), ui::simple_button<filter_allies_button>,
		CT_STRING("sort_by_country"), ui::simple_button<sort_by_country_button>,
		CT_STRING("sort_by_prio"), ui::simple_button<sort_by_prio_button>,
		CT_STRING("sort_by_gp0"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag0"), ui::masked_flag<sort_by_gpflag0_button>,
		CT_STRING("sort_by_gp1"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag1"), ui::masked_flag<sort_by_gpflag1_button>,
		CT_STRING("sort_by_gp2"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag2"), ui::masked_flag<sort_by_gpflag2_button>,
		CT_STRING("sort_by_gp3"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag3"), ui::masked_flag<sort_by_gpflag3_button>,
		CT_STRING("sort_by_gp4"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag4"), ui::masked_flag<sort_by_gpflag4_button>,
		CT_STRING("sort_by_gp5"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag5"), ui::masked_flag<sort_by_gpflag5_button>,
		CT_STRING("sort_by_gp6"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag6"), ui::masked_flag<sort_by_gpflag6_button>,
		CT_STRING("sort_by_gp7"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag7"), ui::masked_flag<sort_by_gpflag7_button>,
		CT_STRING("sort_by_boss"), ui::simple_button<sort_by_boss_button>,
		CT_STRING("sort_by_prestige"), ui::simple_button<sort_by_prestige_button>,
		CT_STRING("sort_by_economic"), ui::simple_button<sort_by_economic_button>,
		CT_STRING("sort_by_military"), ui::simple_button<sort_by_military_button>,
		CT_STRING("sort_by_total"), ui::simple_button<sort_by_total_button>,
		CT_STRING("sort_by_opinion"), ui::simple_button<sort_by_opinion_button>,
		CT_STRING("sort_by_relation"), ui::simple_button<sort_by_relation_button>,
		CT_STRING("country_listbox"), ui::discrete_listbox<nations_details_lb, nation_details_item, country_tag>,
		CT_STRING("diplomacy_country_facts"), details_pane,
		diplomacy_window_base
	> {};

	template<typename W>
	void influence_details_base::on_create(W & w, world_state & ws) {
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, 6i16 };
		});

		auto common_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["topbar_outlinerbutton"]);
		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			influence_button));
		influence_button.associated_object->align = ui::alignment::top_left;
		influence_button.associated_object->position = ui::xy_pair{ 80i16, 6i16 };
	}

	template<typename W>
	void influence_details_window_container::on_create(W & w, world_state & ws) {
		auto common_tag_b = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["diplomacy_non_gp_extra_info"]);

		for(uint32_t i = 0; i < std::extent_v<decltype(influence)>; ++i) {
			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag_b,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				influence[i]));
			influence[i].gp_index = int32_t(i);
			influence[i].associated_object->position = ui::xy_pair{ int16_t(i < 4 ? 0 + 6 : 157 + 6), int16_t(35 * (i % 4) + 4) };
		}
	}

	template<typename W>
	void diplomacy_window_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});

		auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["diplomacy_greatpower_info"]);

		for(uint32_t i = 0; i < std::extent_v<decltype(gp_items)>; ++i) {
			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				gp_items[i]));
			gp_items[i].gp_index = int32_t(i);
			gp_items[i].associated_object->position = ui::xy_pair{ int16_t(i < 4 ? 26 : 342), int16_t(60 * (i % 4) + 55) };
		}

		w.template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(ws.w.gui_m, 0);
		ui::hide(*(w.template get <CT_STRING("crisis_info_win")>().associated_object));
		ui::hide(*(w.template get<CT_STRING("war_listbox")>().associated_object));
		ui::hide(*(w.template get<CT_STRING("cb_info_win")>().associated_object));

		ui::hide(*associated_object);
	}

	template<typename W>
	inline void details_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{ 340i16, 636i16 };
		associated_object->position += ui::xy_pair{ 37i16, 0i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -37i16, 0i16 };
		});

		{
			auto common_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["topbar_outlinerbutton"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				increase_relations));
			increase_relations.associated_object->align = ui::alignment::top_left;
			increase_relations.associated_object->position = ui::xy_pair{ 286i16, 23i16 };

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				decrease_relations));
			decrease_relations.associated_object->align = ui::alignment::top_left;
			decrease_relations.associated_object->position = ui::xy_pair{ 265i16, 23i16 };
			decrease_relations.set_frame(ws.w.gui_m, 1ui32);
		}

		{
			auto common_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["action_option"]);

			constexpr int16_t x_off = 0i16;
			constexpr int16_t y_off = 520i16;
			constexpr int16_t x_size = 160i16;
			constexpr int16_t y_size = 24i16;

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				make_break_alliance));
			make_break_alliance.associated_object->position = ui::xy_pair{ x_off, y_off };

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				call_ally));
			call_ally.associated_object->position = ui::xy_pair{ x_off + x_size, y_off };
			call_ally.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::call_ally_button]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				expel_advisors));
			expel_advisors.associated_object->position = ui::xy_pair{ x_off, int16_t(y_off + y_size * 1) };
			expel_advisors.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::expel_advisors_button]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				ban_embassy));
			ban_embassy.associated_object->position = ui::xy_pair{ x_off + x_size, int16_t(y_off + y_size * 1) };
			ban_embassy.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::ban_embassy_button]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				justify_war));
			justify_war.associated_object->position = ui::xy_pair{ x_off, int16_t(y_off + y_size * 2) };
			justify_war.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::justify_war_button]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				declare_war_offer_peace));
			declare_war_offer_peace.associated_object->position = ui::xy_pair{ x_off + x_size, int16_t(y_off + y_size * 2) };
		}

		constexpr int16_t yadjust = 144i16;
		w.template get<CT_STRING("war_extra_info_bg")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_wars")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_wars_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_allies")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_allies_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_protected")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_protected_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_truce")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_truce_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_cb")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("country_cb_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("infamy_icon")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("warexhastion_icon")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("brigade_icon")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("ships_icon")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("infamy_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("warexhastion_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("brigade_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("ships_text")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("current_wargoals")>().associated_object->position.y += yadjust;
		w.template get<CT_STRING("add_wargoal")>().associated_object->position.y += yadjust;
	}

	template<typename W>
	void details_base::windowed_update(W & w, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(ws.w.diplomacy_w.selected_nation == player->id) {
				ui::hide(*increase_relations.associated_object);
				ui::hide(*decrease_relations.associated_object);
			} else {
				ui::make_visible_and_update(ws.w.gui_m, *increase_relations.associated_object);
				ui::make_visible_and_update(ws.w.gui_m, *decrease_relations.associated_object);
			}
		}
	}

	template<typename W>
	void crisis_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 638i16, 249i16 };
		associated_object->position += ui::xy_pair{ -16i16, -6i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 16i16, 6i16 };
		});
	}

	template<typename W>
	void crisis_attacker_defender_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 215i16, 175i16 };
		associated_object->position += ui::xy_pair{ -20i16, -26i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 20i16, 26i16 };
		});
	}

	template<typename window_type>
	void war_display_base::on_create(window_type & w, world_state & ws) {
		w.associated_object->position.x = 0;
	}

	template<typename window_type>
	void war_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];

			text_data::replacement repl[7] = {
				text_data::replacement{text_data::value_type::order, vector_backed_string<char16_t>(u""), [](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::first,
					is_valid_index(this_war.first_adj) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.first_adj) : vector_backed_string<char16_t>(u"") ,
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::second,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::state,
					is_valid_index(this_war.state_name) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.state_name) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::second_country,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::country,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::country_adj,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}}
			};
			ui::add_linear_text(ui::xy_pair{ 0,0 }, this_war.war_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 7);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void attacker_strength::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		war = win.war;
		if(is_valid_index(war)) {
			auto& this_war = ws.w.military_s.wars[war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);

			int32_t total = 0;
			for(auto a : r) {
				if(is_valid_index(a))
					total += military::total_active_divisions(ws, ws.w.nation_s.nations[a]);
			}

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, total);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void defender_strength::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		war = win.war;
		if(is_valid_index(war)) {
			auto& this_war = ws.w.military_s.wars[war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);

			int32_t total = 0;
			for(auto a : r) {
				if(is_valid_index(a))
					total += military::total_active_divisions(ws, ws.w.nation_s.nations[a]);
			}

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, total);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void war_score_progress_bar::windowed_update(ui::progress_bar<war_score_progress_bar>& bar, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			bar.set_fraction((this_war.current_war_score + 1.0f) / 2.0f);
		}
	}

	template<typename window_type>
	void war_score_overlay::windowed_update(ui::dynamic_icon<war_score_overlay>& self, window_type & win, world_state & ws) {
		war = win.war;
	}

	template<typename window_type>
	void attacker_wg_marker::windowed_update(ui::dynamic_icon<attacker_wg_marker>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			float total_ws = std::min(military::total_attacker_demands_war_score(ws, this_war), 1.0f);

			int32_t x_off = 192 + (216 / 2) + int32_t((216 / 2) * total_ws) - self.associated_object->size.x / 2;
			self.associated_object->position.x = int16_t(x_off);
		}
	}

	template<typename window_type>
	void defender_wg_marker::windowed_update(ui::dynamic_icon<defender_wg_marker>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			float total_ws = std::min(military::total_defender_demands_war_score(ws, this_war), 1.0f);

			int32_t x_off = 192 + (216 / 2) - int32_t((216 / 2) * total_ws) - self.associated_object->size.x / 2;
			self.associated_object->position.x = int16_t(x_off);
		}
	}

	template<typename window_type>
	void war_score_text_box::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::percent, this_war.current_war_score);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void join_attacker_button::windowed_update(ui::simple_button<join_attacker_button>& self, window_type & win, world_state &) {
		war = win.war;
		self.set_enabled(false);
	}

	template<typename window_type>
	void join_defender_button::windowed_update(ui::simple_button<join_defender_button>& self, window_type & win, world_state &) {
		war = win.war;
		self.set_enabled(false);
	}

	template<typename W>
	void flag_sub_item::windowed_update(ui::masked_flag<flag_sub_item>& self, W & w, world_state & ws) {
		if(ws.w.nation_s.nations.is_valid_index(w.c))
			self.set_displayed_flag(ws, ws.w.nation_s.nations[w.c].tag);
	}

	template<typename window_type>
	void wg_sub_item::windowed_update(ui::dynamic_icon<wg_sub_item>& self, window_type & win, world_state & ws) {
		this_wg = win.this_wg;
		if(is_valid_index(this_wg.cb_type)) {
			auto icon = ws.s.military_m.cb_types[this_wg.cb_type].sprite_index;
			self.set_frame(ws.w.gui_m, icon != 0 ? uint32_t(icon - 1) : 0ui32);
		}
	}

	template<typename lb_type, typename window_type>
	void defenders_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto n : r) {
				lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type, typename window_type>
	void attackers_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
			for(auto n : r) {
				lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type, typename window_type>
	void defenders_wg_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.military_s.war_goal_arrays, this_war.war_goals);
			for(auto i = r.first; i != r.second; ++i) {
				if(contains_item(ws.w.nation_s.nations_arrays, this_war.defenders, i->from_country))
					lb.add_item(ws, *i);
			}
		}
	}

	template<typename lb_type, typename window_type>
	void attackers_wg_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.military_s.war_goal_arrays, this_war.war_goals);
			for(auto i = r.first; i != r.second; ++i) {
				if(contains_item(ws.w.nation_s.nations_arrays, this_war.attackers, i->from_country))
					lb.add_item(ws, *i);
			}
		}
	}

	template<typename lb_type>
	void war_lb::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<military::war_tag, 16, concurrent_allocator<military::war_tag>> data;
		ws.w.military_s.wars.for_each([&data, &ws](military::war const& w) {
			if(auto id = w.id; ws.w.military_s.wars.is_valid_index(id))
				data.push_back(id);
		});
		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename window_type>
	void crisis_attacker_primary_flag::windowed_update(ui::masked_flag<crisis_attacker_primary_flag>& self, window_type & win, world_state & ws) {
		auto t = ws.w.current_crisis.on_behalf_of;
		if(bool(t) && t != ws.w.current_crisis.primary_attacker) {
			self.set_displayed_flag(ws, t->tag);
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}

	template<typename window_type>
	void crisis_defender_primary_flag::windowed_update(ui::masked_flag<crisis_defender_primary_flag>& self, window_type & win, world_state & ws) {
		auto t = ws.w.current_crisis.target;
		if(bool(t) && t != ws.w.current_crisis.primary_defender) {
			self.set_displayed_flag(ws, t->tag);
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}

	template<typename lb_type>
	void crisis_attacker_backer_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers);
		for(auto n : r) {
			if(is_valid_index(n)) {
				auto nloc = ws.w.nation_s.nations.get_location(n);
				if(nloc != ws.w.current_crisis.on_behalf_of && nloc != ws.w.current_crisis.primary_attacker)
					lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type>
	void crisis_defender_backer_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders);
		for(auto n : r) {
			if(is_valid_index(n)) {
				auto nloc = ws.w.nation_s.nations.get_location(n);
				if(nloc != ws.w.current_crisis.target && nloc != ws.w.current_crisis.primary_defender)
					lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type>
	void crisis_attacker_wg_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.military_s.war_goal_arrays, ws.w.current_crisis.goals);
		for(auto i = r.first; i != r.second; ++i) {
			if(contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers, i->from_country))
				lb.add_item(ws, *i);
		}
	}

	template<typename lb_type>
	void crisis_defender_wg_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.military_s.war_goal_arrays, ws.w.current_crisis.goals);
		for(auto i = r.first; i != r.second; ++i) {
			if(contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders, i->from_country))
				lb.add_item(ws, *i);
		}
	}

	template<typename window_type>
	void make_offer_button::windowed_update(ui::simple_button<make_offer_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<typename window_type>
	void crisis_defender_back_down_button::windowed_update(ui::simple_button<crisis_defender_back_down_button>& self, window_type & win, world_state &) {
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void crisis_attacker_back_down_button::windowed_update(ui::simple_button<crisis_attacker_back_down_button>& self, window_type & win, world_state &) {
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	inline void crisis_support_attacker_button::windowed_update(ui::simple_button<crisis_support_attacker_button>&, window_type & win, world_state &) {}

	template<typename window_type>
	inline void crisis_support_defender_button::windowed_update(ui::simple_button<crisis_support_defender_button>&, window_type & win, world_state &) {}

	template<typename window_type>
	void undecided_gp_flag::windowed_update(ui::masked_flag<undecided_gp_flag>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag))
			self.set_displayed_flag(ws, ws.w.nation_s.nations[win.tag].tag);
	}

	template<typename W>
	void undecided_pane_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{ 210i16, 150i16 };
		associated_object->position += ui::xy_pair{ -6i16, 0i16 };

		auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["fence_sitter_entry"]);
		for(uint32_t i = 0; i < std::extent_v<decltype(undecided_nations)>; ++i) {
			auto wwin = ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				undecided_nations[i]);
			undecided_nations[i].associated_object->position = ui::xy_pair{ int16_t(52 * (i % 4)), int16_t(i < 4 ? 0 : 75) };
		}
	}

	template<typename W>
	void undecided_pane_base::windowed_update(W & w, world_state & ws) {
		if(ws.w.current_crisis.type == current_state::crisis_type::none) {
			ui::hide(*associated_object);
		} else {
			auto interested_range = get_range(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested);
			for(uint32_t i = 0; i < std::extent_v<decltype(undecided_nations)>; ++i) {
				if(interested_range.first + i < interested_range.second) {
					undecided_nations[i].tag = *(interested_range.first + i);
					ui::make_visible_immediate(*undecided_nations[i].associated_object);
				} else {
					ui::hide(*undecided_nations[i].associated_object);
				}
			}
			ui::make_visible_immediate(*associated_object);
		}
	}

	template<typename W>
	void undecided_gp_entry_base::on_create(W & w, world_state &) {
		associated_object->size = ui::xy_pair{ 52i16, 75i16 };
	}

	template<typename W>
	void nation_details_item_base::on_create(W & w, world_state & ws) {
		w.associated_object->position.x = 0;
	}

	template<typename window_type>
	void nation_details_background_button::windowed_update(ui::simple_button<nation_details_background_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		if(tag == ws.w.diplomacy_w.selected_nation)
			self.set_frame(ws.w.gui_m, 1ui32);
		else
			self.set_frame(ws.w.gui_m, 0ui32);
	}

	template<typename W>
	void nation_details_flag::windowed_update(ui::masked_flag<nation_details_flag>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			self.set_displayed_flag(ws, ws.w.nation_s.nations[w.tag].tag);
		}
	}

	template<typename window_type>
	void nation_details_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations[win.tag].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_priority_button::windowed_update(ui::simple_button<nation_details_priority_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<int32_t index>
	template<typename window_type>
	void gp_influence_text<index>::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + index < r.second) {
			if(auto id = *(r.first + index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::integer, nations::get_influence_value(ws, ws.w.nation_s.nations[id], win.tag));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}


	template<typename W>
	void nation_details_sphere_leader_flag::windowed_update(ui::masked_flag<nation_details_sphere_leader_flag>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			auto leader = ws.w.nation_s.nations[w.tag].sphere_leader;
			if(leader) {
				self.set_displayed_flag(ws, leader->tag);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename window_type>
	void nation_details_military_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].military_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_economic_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].industrial_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_prestige_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].prestige_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_overall_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].overall_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_relations_value::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, nations::get_relationship(ws, *player, win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_opinion_type::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, nations::influence_level_to_text(ws, nations::get_influence_level(ws, *player, win.tag)), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}


	template<typename lb_type>
	void nations_details_lb::populate_list(lb_type & lb, world_state & ws) {
		std::vector<country_tag, concurrent_allocator<country_tag>> data;

		switch(ws.w.diplomacy_w.filter) {
			default:
			case country_sub_filter::all:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0)
						data.push_back(id);
				});
				break;
			case country_sub_filter::continent_africa:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.africa_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_asia:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.asia_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_europe:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.europe_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_north_america:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.north_america_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_oceania:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.oceania_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_south_america:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.south_america_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::enemy:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->opponents_in_war);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::ally:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->allies);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::neighbor:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->neighboring_nations);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::sphere:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->sphere_members);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
		}

		switch(ws.w.diplomacy_w.sort_type) {
			default:
			case country_sort::none:
				break;
			case country_sort::name:
			{
				vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
				std::sort(data.begin(), data.end(), [&ws, &lss](country_tag a, country_tag b) {
					auto a_name = ws.w.nation_s.nations[a].name;
					auto b_name = ws.w.nation_s.nations[b].name;
					return lss(
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
				});
			}
				break;
			case country_sort::priority:
				break;
			case country_sort::gp_one:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 0 < r.second) {
					if(auto id = *(r.first + 0); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_two:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 1 < r.second) {
					if(auto id = *(r.first + 1); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_three:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 2 < r.second) {
					if(auto id = *(r.first + 2); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_four:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 3 < r.second) {
					if(auto id = *(r.first + 3); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_five:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 4 < r.second) {
					if(auto id = *(r.first + 4); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_six:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 5 < r.second) {
					if(auto id = *(r.first + 5); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_seven:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 6 < r.second) {
					if(auto id = *(r.first + 6); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_eight:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 7 < r.second) {
					if(auto id = *(r.first + 7); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](country_tag a, country_tag b) {
							return nations::get_influence_value(ws, *n, a) > nations::get_influence_value(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::sphere_leader:
				std::sort(data.begin(), data.end(), [&ws](country_tag a, country_tag b) {
					return ws.w.nation_s.nations[a].sphere_leader > ws.w.nation_s.nations[b].sphere_leader;
				});
				break;
			case country_sort::prestige_rank:
				std::sort(data.begin(), data.end(), [&ws](country_tag a, country_tag b) {
					return ws.w.nation_s.nations[a].prestige_rank < ws.w.nation_s.nations[b].prestige_rank;
				});
				break;
			case country_sort::economic_rank:
				std::sort(data.begin(), data.end(), [&ws](country_tag a, country_tag b) {
					return ws.w.nation_s.nations[a].industrial_rank < ws.w.nation_s.nations[b].industrial_rank;
				});
				break;
			case country_sort::military_rank:
				std::sort(data.begin(), data.end(), [&ws](country_tag a, country_tag b) {
					return ws.w.nation_s.nations[a].military_rank < ws.w.nation_s.nations[b].military_rank;
				});
				break;
			case country_sort::overall_rank:
				std::sort(data.begin(), data.end(), [&ws](country_tag a, country_tag b) {
					return ws.w.nation_s.nations[a].overall_rank < ws.w.nation_s.nations[b].overall_rank;
				});
				break;
			case country_sort::opinion:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](country_tag a, country_tag b) {
						return nations::get_influence_level(ws, *player, a) > nations::get_influence_level(ws, *player, b);
					});
				}
				break;
			case country_sort::relation:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](country_tag a, country_tag b) {
						return nations::get_relationship(ws, *player, a) > nations::get_relationship(ws, *player, b);
					});
				}
				break;
		}

		lb.update_list(data.begin(), data.end());
		if(ws.w.diplomacy_w.goto_selected_pending) {
			lb.goto_element(ws.w.diplomacy_w.selected_nation, ws.w.gui_m);
			ws.w.diplomacy_w.goto_selected_pending = false;
		}
	}

	template<typename W>
	void cb_item_base::on_create(W & w, world_state & ws) {
		w.associated_object->position.x = 0i16;

		auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["diplomacy_puppet"]);

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			cb_fabrication_by));
		cb_fabrication_by.associated_object->position = ui::xy_pair{ 42i16, 10i16 };
		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			cb_fabrication_target));
		cb_fabrication_target.associated_object->position = ui::xy_pair{ 535i16, 10i16 };
	}

	template<typename W>
	void cb_item_icon::windowed_update(ui::dynamic_icon<cb_item_icon>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			nations::nation& n = ws.w.nation_s.nations[w.tag];
			tag = n.cb_construction_type;
			if(is_valid_index(tag)) {
				self.set_frame(ws.w.gui_m, ws.s.military_m.cb_types[tag].sprite_index != 0 ? uint32_t(ws.s.military_m.cb_types[tag].sprite_index - 1) : 0ui32);
			}
		} else {
			tag = military::cb_type_tag();
		}
	}
	
	template<typename W>
	void cb_item_progress::windowed_update(ui::progress_bar<cb_item_progress>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			nations::nation& n = ws.w.nation_s.nations[w.tag];
			self.set_fraction(n.cb_construction_progress);
		} else {
			self.set_fraction(0.0f);
		}
	}

	template<typename window_type>
	void cb_item_progress_text::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			nations::nation& n = ws.w.nation_s.nations[win.tag];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::percent, n.cb_construction_progress);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void cb_item_cancel_button::windowed_update(ui::simple_button<cb_item_cancel_button>& self, window_type & win, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(win.tag == player->id)
				ui::make_visible_immediate(*self.associated_object);
			else
				ui::hide(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}

	template<typename lb_type>
	void cb_fabrication_lb::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<nations::country_tag, 32, concurrent_allocator<nations::country_tag>> data;
		ws.w.nation_s.nations.for_each([&data, player = ws.w.local_player_nation, &ws](nations::nation const& n) {
			if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && is_valid_index(n.cb_construction_type)) {
				if((n.flags & nations::nation::cb_construction_discovered) != 0 || &n == player) {
					data.push_back(id);
				}
			}
		});
		lb.new_list(data.begin(), data.end());
	}

	template<typename W>
	void gp_display_base::on_create(W & w, world_state & ws) {
		w.associated_object->size = ui::xy_pair{ 316i16, 60i16};
	}

	template<typename window_type>
	void gp_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(auto id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations[id].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void gp_prestige::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(auto id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::exact_integer, nations::get_prestige(ws.w.nation_s.nations[id]));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void gp_military::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations[id].military_score);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void gp_industry::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations[id].industrial_score);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void gp_overall::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::exact_integer, nations::get_prestige(ws.w.nation_s.nations[id]) + ws.w.nation_s.nations[id].industrial_score + ws.w.nation_s.nations[id].military_score);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void gp_flag::windowed_update(ui::masked_flag<gp_flag>& self, window_type & win, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
				return;
			}
		}
		self.set_displayed_flag(ws, cultures::national_tag());
	}

	template<typename lb_type, typename window_type>
	void gp_sphere::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				auto sphere_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[id].sphere_members);
				for(auto c : sphere_range) {
					if(is_valid_index(c))
						lb.add_item(ws, c);
				}
			}
		}
	}

	template<typename lb_type>
	void details_gp_sphere::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(!nations::is_great_power(ws, ws.w.nation_s.nations[selected]))
				return;
			auto sphere_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[selected].sphere_members);
			for(auto s : sphere_range) {
				lb.add_item(ws, s);
			}
		}
	}

	template<typename lb_type>
	void details_gp_friendly::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(!nations::is_great_power(ws, ws.w.nation_s.nations[selected]))
				return;
			auto inf_range = get_range(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations[selected].gp_influence);
			for(auto i = inf_range.first; i != inf_range.second; ++i) {
				if(i->level == 4i8)
					lb.add_item(ws, i->target);
			}
		}
	}

	template<typename lb_type>
	void details_gp_coridal::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(!nations::is_great_power(ws, ws.w.nation_s.nations[selected]))
				return;
			auto inf_range = get_range(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations[selected].gp_influence);
			for(auto i = inf_range.first; i != inf_range.second; ++i) {
				if(i->level == 3i8)
					lb.add_item(ws, i->target);
			}
		}
	}

	template<typename W>
	void details_non_gp_background::windowed_update(ui::dynamic_icon<details_non_gp_background>& self, W & win, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(nations::is_great_power(ws, ws.w.nation_s.nations[selected]))
				ui::hide(*self.associated_object);
			else
				ui::make_visible_immediate(*self.associated_object);
		}
	}

	template<typename lb_type>
	void details_wars::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			auto opp_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[selected].opponents_in_war);
			for(auto n : opp_range) {
				if(is_valid_index(n))
					lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type>
	void details_allies::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			auto allies_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations[selected].allies);
			for(auto n : allies_range) {
				if(is_valid_index(n))
					lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type>
	void details_truce::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			auto truce_range = get_range(ws.w.nation_s.truce_arrays, ws.w.nation_s.nations[selected].truces);
			for(auto i = truce_range.first; i != truce_range.second; ++i)
				lb.add_item(ws, *i);
		}
	}

	template<typename lb_type>
	void details_cbs::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			auto cb_range = get_range(ws.w.military_s.cb_arrays, ws.w.nation_s.nations[selected].active_cbs);
			for(auto i = cb_range.first; i != cb_range.second; ++i)
				lb.add_item(ws, *i);
		}
	}

	template<typename lb_type>
	void details_protected::populate_list(lb_type & lb, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(auto s = ws.w.nation_s.nations[selected].sphere_leader; s) {
				if(auto id = s->id; ws.w.nation_s.nations.is_valid_index(id))
					lb.add_item(ws, id);
			}
			if(auto s = ws.w.nation_s.nations[selected].overlord; bool(s) && s != ws.w.nation_s.nations[selected].sphere_leader) {
				if(auto id = s->id; ws.w.nation_s.nations.is_valid_index(id))
					lb.add_item(ws, id);
			}
		}
	}

	template<typename lb_type>
	void details_wargoals::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto wrange = get_range(ws.w.military_s.war_arrays, player->wars_involved_in);
			for(auto i = wrange.first; i != wrange.second; ++i) {
				if(auto wid = i->war_id; ws.w.military_s.wars.is_valid_index(wid)) {
					auto wg_range = get_range(ws.w.military_s.war_goal_arrays, ws.w.military_s.wars[wid].war_goals);
					for(auto j = wg_range.first; j != wg_range.second; ++j) {
						if(j->from_country == player->id && j->target_country == ws.w.diplomacy_w.selected_nation)
							lb.add_item(ws, *j);
					}
				}
			}
		}
	}

	template<typename W>
	void flag_truce_sub_item::windowed_update(ui::masked_flag<flag_truce_sub_item>& self, W & w, world_state & ws) {
		c = w.c;
		until = w.until;
		self.set_displayed_flag(ws, c);
	}

	template<typename window_type>
	void details_cb_sub_item::windowed_update(ui::dynamic_icon<details_cb_sub_item>& self, window_type & win, world_state & ws) {
		this_cb = win.this_cb;
		if(is_valid_index(this_cb.type)) {
			self.set_frame(ws.w.gui_m, ws.s.military_m.cb_types[this_cb.type].sprite_index != 0 ? uint32_t(ws.s.military_m.cb_types[this_cb.type].sprite_index - 1) : 0ui32);
		}
	}

	template<typename window_type>
	void influence_details_gp_flag::windowed_update(ui::masked_flag<influence_details_gp_flag>& self, window_type & win, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
				return;
			}
		}
		self.set_displayed_flag(ws, cultures::national_tag());
	}

	template<typename window_type>
	void influence_details_opinion::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 },
					influence_level_to_text(ws, get_influence_level(ws, ws.w.nation_s.nations[id], ws.w.diplomacy_w.selected_nation)),
					fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void influence_details_influence::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::integer, get_influence_value(ws, ws.w.nation_s.nations[id], ws.w.diplomacy_w.selected_nation));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void influence_details_investment::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + win.gp_index < r.second) {
			if(nations::country_tag id = *(r.first + win.gp_index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::currency, get_influence(ws, ws.w.nation_s.nations[id], ws.w.diplomacy_w.selected_nation).investment_amount);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void influence_details_discredited_icon::windowed_update(ui::dynamic_icon<influence_details_discredited_icon>& self, window_type & win, world_state & ws) {
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void influence_details_banned_icon::windowed_update(ui::dynamic_icon<influence_details_banned_icon>& self, window_type & win, world_state & ws) {
		ui::hide(*self.associated_object);
	}


	template<typename W>
	void influence_details_window_container::windowed_update(W & w, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(nations::is_great_power(ws, ws.w.nation_s.nations[selected]))
				ui::hide(*associated_object);
			else
				ui::make_visible_immediate(*associated_object);
		}
	}
}
