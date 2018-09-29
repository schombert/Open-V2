#pragma once
#include "common\\common.h"
#include "nations.h"
#include "gui\\gui.hpp"

class world_state;

namespace nations {
	class diplomacy_window_base : public ui::draggable_region {
	public:
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

	class flag_item_base : public ui::visible_region {
	public:
		nations::country_tag c;
		flag_item_base(nations::country_tag t) : c(t) {}
	};

	class wg_item_base : public ui::visible_region {
	public:
		military::war_goal this_wg;
		wg_item_base(military::war_goal t) : this_wg(t) {}
	};

	class flag_sub_item {
	public:
		flag_sub_item(nations::country_tag t) {}
		template<typename W>
		void windowed_update(ui::masked_flag<flag_sub_item>& self, W& w, world_state& ws);
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

	using flag_item = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<flag_sub_item>,
		flag_item_base
	>;

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
		void windowed_update(ui::masked_flag<nation_details_flag>& self, W& w, world_state& ws);
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

	using nation_details_item = ui::gui_window<
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
	>;

	class nations_details_lb {
	public:
	};

	using diplomacy_window_t = ui::gui_window <
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
		diplomacy_window_base
	>;

}
