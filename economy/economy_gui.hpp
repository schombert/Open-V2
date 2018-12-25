#pragma once
#include "common\\common.h"
#include "world_state\\world_state.h"
#include "gui\\gui.hpp"
#include "economy_functions.h"
#include "economy_gui.h"
#include "nations\\nations_functions.h"

namespace economy {
	class production_window_base : public ui::draggable_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class close_button {
	public:
		void button_function(ui::simple_button<close_button>&, world_state&);
	};

	class production_tab_button_group {
	public:
		void on_select(world_state& ws, uint32_t i);
	};

	class empty_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_pane_base : public ui::window_pane {
	public:
	};

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

	class clear_text_button {
	public:
		void on_create(ui::simple_button<clear_text_button>&, world_state&);
	};

	class sort_by_country_button {
	public:
		void button_function(ui::simple_button<sort_by_country_button>&, world_state& ws);
	};
	class sort_by_my_flag_button {
	public:
		void update(ui::masked_flag<sort_by_my_flag_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_my_flag_button>&, world_state& ws);
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
	class sort_by_invest_factories_button {
	public:
		void button_function(ui::simple_button<sort_by_invest_factories_button>&, world_state& ws);
	};

	class sort_by_project_state_button {
	public:
		void button_function(ui::simple_button<sort_by_project_state_button>&, world_state& ws);
	};
	class sort_by_project_type_button {
	public:
		void button_function(ui::simple_button<sort_by_project_type_button>&, world_state& ws);
	};
	class sort_by_project_completion_button {
	public:
		void button_function(ui::simple_button<sort_by_project_completion_button>&, world_state& ws);
	};
	class sort_by_project_investors_button {
	public:
		void button_function(ui::simple_button<sort_by_project_investors_button>&, world_state& ws);
	};

	class investment_country_item_base : public ui::visible_region {
	public:
		nations::country_tag tag;
		void set_value(nations::country_tag t) {
			tag = t;
		}

		template<typename W>
		void on_create(W& w, world_state&);
	};

	class investment_country_item_background_button {
	public:
		nations::country_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<investment_country_item_background_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<investment_country_item_background_button>& self, world_state& ws);
	};

	class investment_country_item_flag {
	public:
	   template<typename W>
	   void windowed_update(ui::masked_flag<investment_country_item_flag>& self, W& w, world_state& ws);
	};

	class investment_country_item_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_self_investment_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<int32_t index>
	class gp_investment_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_sphere_leader_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<investment_country_item_sphere_leader_flag>& self, W& w, world_state& ws);
	};

	class investment_country_item_military_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_industrial_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_prestige_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_overall_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_opinion_type {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_relations_value {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_factories_count {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using investment_country_item = ui::gui_window<
		CT_STRING("country_select"), ui::simple_button<investment_country_item_background_button>,
		CT_STRING("country_flag"), ui::masked_flag<investment_country_item_flag>,
		CT_STRING("country_name"), ui::display_text<investment_country_item_name>,
		CT_STRING("invest"), ui::display_text<investment_country_item_self_investment_text>,
		CT_STRING("country_gp0"), ui::display_text<gp_investment_text<0>>,
		CT_STRING("country_gp1"), ui::display_text<gp_investment_text<1>>,
		CT_STRING("country_gp2"), ui::display_text<gp_investment_text<2>>,
		CT_STRING("country_gp3"), ui::display_text<gp_investment_text<3>>,
		CT_STRING("country_gp4"), ui::display_text<gp_investment_text<4>>,
		CT_STRING("country_gp5"), ui::display_text<gp_investment_text<5>>,
		CT_STRING("country_gp6"), ui::display_text<gp_investment_text<6>>,
		CT_STRING("country_gp7"), ui::display_text<gp_investment_text<7>>,
		CT_STRING("country_boss_flag"), ui::masked_flag<investment_country_item_sphere_leader_flag>,
		CT_STRING("country_prestige"), ui::display_text<investment_country_item_prestige_rank>,
		CT_STRING("country_economic"), ui::display_text<investment_country_item_industrial_rank>,
		CT_STRING("country_military"), ui::display_text<investment_country_item_military_rank>,
		CT_STRING("country_total"), ui::display_text<investment_country_item_overall_rank>,
		CT_STRING("country_opinion"), ui::display_text<investment_country_item_opinion_type>,
		CT_STRING("country_relation"), ui::display_text<investment_country_item_relations_value>,
		CT_STRING("factories"), ui::display_text<investment_country_item_factories_count>,
		investment_country_item_base
	>;

	class investment_country_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class subsidize_all_button {
	public:
		void update(ui::simple_button<subsidize_all_button>& self, world_state& ws);
		void button_function(ui::simple_button<subsidize_all_button>& self, world_state& ws);
	};

	class unsubsidize_all_button {
	public:
		void update(ui::simple_button<unsubsidize_all_button>& self, world_state& ws);
		void button_function(ui::simple_button<unsubsidize_all_button>& self, world_state& ws);
	};

	class open_all_factories_button {
	public:
		void update(ui::simple_button<open_all_factories_button>& self, world_state& ws);
		void button_function(ui::simple_button<open_all_factories_button>& self, world_state& ws);
	};

	class close_all_factories_button {
	public:
		void update(ui::simple_button<close_all_factories_button>& self, world_state& ws);
		void button_function(ui::simple_button<close_all_factories_button>& self, world_state& ws);
	};

	class select_all_factories_filters_button {
	public:
		void button_function(ui::simple_button<select_all_factories_filters_button>& self, world_state& ws);
	};

	class deselect_all_factories_filters_button {
	public:
		void button_function(ui::simple_button<deselect_all_factories_filters_button>& self, world_state& ws);
	};

	class show_hide_empty_states_button {
	public:
		void update(ui::button<show_hide_empty_states_button>& self, world_state& ws);
		void button_function(ui::button<show_hide_empty_states_button>& self, world_state& ws);
	};

	class sort_factories_by_state_name {
	public:
		void button_function(ui::simple_button<sort_factories_by_state_name>& self, world_state& ws);
	};

	class sort_factories_by_worker_a {
	public:
		void button_function(ui::button<sort_factories_by_worker_a>& self, world_state& ws);
	};

	class sort_factories_by_worker_b {
	public:
		void button_function(ui::button<sort_factories_by_worker_b>& self, world_state& ws);
	};

	class sort_factories_by_owner {
	public:
		void button_function(ui::button<sort_factories_by_owner>& self, world_state& ws);
	};

	class sort_factories_by_count {
	public:
		void button_function(ui::simple_button<sort_factories_by_count>& self, world_state& ws);
	};

	class sort_factories_by_infrastructure {
	public:
		void button_function(ui::simple_button<sort_factories_by_infrastructure>& self, world_state& ws);
	};

	class factory_filter_buttons_base : public ui::window_pane {
	public:
		ui::button<sort_factories_by_worker_a> workers_a;
		ui::button<sort_factories_by_worker_b> workers_b;
		ui::button<sort_factories_by_owner> owner;

		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	class good_filter_item_base : public ui::visible_region {
	public:
		economy::goods_tag tag;
	};

	class good_filter_item_button {
	public:
		economy::goods_tag tag;
		template<typename window_type>
		void windowed_update(ui::simple_button<good_filter_item_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<good_filter_item_button>& self, world_state& ws);
	};

	class good_filter_item_enabled_bg {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<good_filter_item_enabled_bg>& self, window_type& win, world_state& ws);
	};
	class good_filter_item_type {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<good_filter_item_type>& self, window_type& win, world_state& ws);
	};
	
	using good_filter_item = ui::gui_window<
		CT_STRING("filter_button"), ui::simple_button<good_filter_item_button>,
		CT_STRING("filter_enabled"), ui::dynamic_icon<good_filter_item_enabled_bg>,
		CT_STRING("goods_type"), ui::dynamic_icon<good_filter_item_type>,
		good_filter_item_base
	>;

	class goods_filters_base : public ui::window_pane {
	public:
		std::vector<good_filter_item> filter_buttons;
		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	using factory_filter_buttons = ui::gui_window<
		CT_STRING("prod_subsidize_all"), ui::simple_button<subsidize_all_button>,
		CT_STRING("prod_unsubsidize_all"), ui::simple_button<unsubsidize_all_button>,
		CT_STRING("prod_open_all_factories"), ui::simple_button<open_all_factories_button>,
		CT_STRING("prod_close_all_factories"), ui::simple_button<close_all_factories_button>,
		CT_STRING("select_all"), ui::simple_button<select_all_factories_filters_button>,
		CT_STRING("deselect_all"), ui::simple_button<deselect_all_factories_filters_button>,
		CT_STRING("show_empty_states"), ui::button<show_hide_empty_states_button>,
		CT_STRING("sort_by_name"), ui::simple_button<sort_factories_by_state_name>,
		CT_STRING("sort_by_factories"), ui::simple_button<sort_factories_by_count>,
		CT_STRING("sort_by_infra"), ui::simple_button<sort_factories_by_infrastructure>,
		CT_STRING("filter_bounds"), ui::gui_window<goods_filters_base>,
		factory_filter_buttons_base
	>;

	class gp_investment_subwindow_base : public ui::visible_region {
	public:
		int32_t nth_nation = 0;
	};

	class gp_subwindow_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<gp_subwindow_flag>& self, W& w, world_state& ws);
	};

	class gp_subwindow_investment_value {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using gp_investment_subwindow = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<gp_subwindow_flag>,
		CT_STRING("nongp_country_investment"), ui::display_text<gp_subwindow_investment_value>,
		gp_investment_subwindow_base>;

	class investment_filter_buttons_base : public ui::window_pane {
	public:
		ui::button<sort_factories_by_worker_a> workers_a;
		ui::button<sort_factories_by_worker_b> workers_b;
		ui::button<sort_factories_by_owner> owner;

		gp_investment_subwindow gp_windows[8];

		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	class investment_target_flag {
	public:
		void update(ui::masked_flag< investment_target_flag>& self, world_state& ws);
	};

	class investment_target_flag_overlay {
	public:
		void update(ui::dynamic_icon<investment_target_flag_overlay>& self, world_state& ws);
	};

	class investment_target_name {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_target_amount {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_target_back_button {
	public:
		void button_function(ui::simple_button<investment_target_back_button>& self, world_state& ws);
	};

	using investment_filter_buttons = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<investment_target_flag>,
		CT_STRING("country_flag_overlay"), ui::dynamic_icon<investment_target_flag_overlay>,
		CT_STRING("country_name"), ui::display_text<investment_target_name>,
		CT_STRING("country_investment"), ui::display_text<investment_target_amount>,
		CT_STRING("invest_country_browse"), ui::simple_button<investment_target_back_button>,
		CT_STRING("select_all"), ui::simple_button<select_all_factories_filters_button>,
		CT_STRING("deselect_all"), ui::simple_button<deselect_all_factories_filters_button>,
		CT_STRING("show_empty_states"), ui::button<show_hide_empty_states_button>,
		CT_STRING("sort_by_name"), ui::simple_button<sort_factories_by_state_name>,
		CT_STRING("sort_by_factories"), ui::simple_button<sort_factories_by_count>,
		CT_STRING("sort_by_infra"), ui::simple_button<sort_factories_by_infrastructure>,
		CT_STRING("filter_bounds"), ui::gui_window<goods_filters_base>,
		investment_filter_buttons_base
	>;

	using investment_pane = ui::gui_window<
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
		CT_STRING("sort_by_my_invest"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_my_flag"), ui::masked_flag<sort_by_my_flag_button>,
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
		CT_STRING("sort_by_invest_factories"), ui::simple_button<sort_by_invest_factories_button>,
		CT_STRING("country_listbox"), ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag>,
		investment_pane_base
	>;

	class state_pop_display_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<state_pop_display_icon>& self, window_type& win, world_state& ws);
	};

	class state_pop_display_amount {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class state_pop_display_window_base : public ui::visible_region {
	public:
		population::pop_type_tag type;
		nations::state_tag in_state;

		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	using state_pop_display_window = ui::gui_window<
		CT_STRING("pop_icon"), ui::dynamic_icon<state_pop_display_icon>,
		CT_STRING("pop_amount"), ui::display_text<state_pop_display_amount>,
		CT_STRING("pop_amount_2"), ui::display_text<empty_text_box>,
		state_pop_display_window_base
	>;

	class factory_display_base : public ui::visible_region {
	public:
		nations::state_tag location;
		int32_t index = -1;

		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	class factory_open_background {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<factory_open_background>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class factory_closed_background {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<factory_closed_background>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class factory_construction_progress_bar {
	public:
		template<typename window_type>
		void windowed_update(ui::progress_bar<factory_construction_progress_bar>& self, window_type& win, world_state& ws);
	};

	class factory_cancel_construction_button {
	public:
		nations::state_tag location;
		int32_t index = -1;

		template<typename window_type>
		void windowed_update(ui::simple_button<factory_cancel_construction_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<factory_cancel_construction_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class factory_upgrade_progress_bar {
	public:
		template<typename window_type>
		void windowed_update(ui::progress_bar<factory_upgrade_progress_bar>& self, window_type& win, world_state& ws);
	};

	class factory_upgrade_progress_overlay {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<factory_upgrade_progress_overlay>& self, window_type& win, world_state& ws);
	};

	class factory_level_text {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class factory_upgrade_button {
	public:
		nations::state_tag location;
		int32_t index = -1;

		template<typename window_type>
		void windowed_update(ui::button<factory_upgrade_button>& self, window_type& win, world_state& ws);
		void button_function(ui::button<factory_upgrade_button>& self, key_modifiers mod, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class hidden_icon {
	public:
		void update(ui::dynamic_icon<hidden_icon>& self, world_state& ws);
	};

	template<int32_t nth_input>
	class factory_input {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<factory_input>& self, window_type& win, world_state& ws);
	};

	class factory_income_arrow {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<factory_income_arrow>& self, window_type& win, world_state& ws);
	};

	class factory_income_text {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class factory_subsidize_button {
	public:
		nations::state_tag location;
		int32_t index = -1;

		template<typename window_type>
		void windowed_update(ui::simple_button<factory_subsidize_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<factory_subsidize_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class hidden_button {
	public:
		void update(ui::simple_button<hidden_button>& self, world_state& ws);
	};

	class factory_workers_display {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<factory_workers_display>& self, window_type& win, world_state& ws);
	};

	class factory_closed_overlay {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<factory_closed_overlay>& self, window_type& win, world_state& ws);
	};

	class factory_output {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<factory_output>& self, window_type& win, world_state& ws);
	};

	class factory_delete_button {
	public:
		nations::state_tag location;
		int32_t index = -1;

		template<typename window_type>
		void windowed_update(ui::simple_button<factory_delete_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<factory_delete_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class factory_open_or_close_button {
	public:
		nations::state_tag location;
		int32_t index = -1;

		template<typename window_type>
		void windowed_update(ui::simple_button<factory_open_or_close_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<factory_open_or_close_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class factory_closed_text {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using factory_display = ui::gui_window<
		CT_STRING("prod_factory_bg"), ui::dynamic_icon<factory_open_background>,
		CT_STRING("prod_factory_inprogress_bg"), ui::dynamic_icon<factory_closed_background>,
		CT_STRING("build_factory_progress"), ui::progress_bar<factory_construction_progress_bar>,
		CT_STRING("prod_cancel_progress"), ui::simple_button<factory_cancel_construction_button>,
		CT_STRING("upgrade_factory_progress"), ui::progress_bar<factory_upgrade_progress_bar>,
		CT_STRING("progress_overlay_16_64"), ui::dynamic_icon<factory_upgrade_progress_overlay>,
		CT_STRING("level"), ui::display_text<factory_level_text>,
		CT_STRING("upgrade"), ui::button<factory_upgrade_button>,
		CT_STRING("input_0_lack2"), ui::dynamic_icon<hidden_icon>,
		CT_STRING("input_1_lack2"), ui::dynamic_icon<hidden_icon>,
		CT_STRING("input_2_lack2"), ui::dynamic_icon<hidden_icon>,
		CT_STRING("input_3_lack2"), ui::dynamic_icon<hidden_icon>,
		CT_STRING("input_0"), ui::dynamic_icon<factory_input<0>>,
		CT_STRING("input_1"), ui::dynamic_icon<factory_input<1>>,
		CT_STRING("input_2"), ui::dynamic_icon<factory_input<2>>,
		CT_STRING("input_3"), ui::dynamic_icon<factory_input<3>>,
		CT_STRING("income_icon"), ui::dynamic_icon<factory_income_arrow>,
		CT_STRING("income"), ui::display_text<factory_income_text, -14>,
		CT_STRING("subsidise"), ui::simple_button<factory_subsidize_button>,
		CT_STRING("priority"), ui::simple_button<hidden_button>,
		CT_STRING("employment_ratio"), ui::dynamic_icon<factory_workers_display>,
		CT_STRING("closed_overlay"), ui::dynamic_icon<factory_closed_overlay>,
		CT_STRING("output"), ui::simple_button<factory_output>,
		CT_STRING("delete_factory"), ui::simple_button<factory_delete_button>,
		CT_STRING("open_close"), ui::simple_button<factory_open_or_close_button>,
		CT_STRING("factory_closed_text"), ui::display_text<factory_closed_text>,
		factory_display_base
	>;

	class state_window_base : public ui::window_pane {
	public:
		nations::state_tag tag;

		state_pop_display_window workers_a;
		state_pop_display_window workers_b;
		state_pop_display_window owner;

		factory_display factories[state::factories_count];

		void set_value(nations::state_tag t) {
			tag = t;
			workers_a.in_state = t;
			workers_b.in_state = t;
			owner.in_state = t;
		}

		template<typename W>
		void on_create(W& win, world_state& ws);
		void update(world_state& ws);
	};

	class state_focus_button {
	public:
		nations::state_tag tag;
		template<typename window_type>
		void windowed_update(ui::simple_button<state_focus_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<state_focus_button>& self, world_state& ws);
	};

	class state_name {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class state_factory_count {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class state_build_factory_button {
	public:
		nations::state_tag tag;
		template<typename window_type>
		void windowed_update(ui::simple_button<state_build_factory_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<state_build_factory_button>& self, world_state& ws);
	};

	class state_average_infrastructure {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using state_window = ui::gui_window<
		CT_STRING("state_focus"), ui::simple_button<state_focus_button>,
		CT_STRING("state_name"), ui::display_text<state_name>,
		CT_STRING("factory_count"), ui::display_text<state_factory_count>,
		CT_STRING("build_new_factory"), ui::simple_button<state_build_factory_button>,
		CT_STRING("avg_infra_text"), ui::display_text<state_average_infrastructure>,
		state_window_base
	>;

	class state_details_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class investment_state_details_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class goods_category_label_base : public ui::visible_region {
	public:
		economy::goods_type_tag category;
	};

	class goods_category_name {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using goods_category_label = ui::gui_window<
		CT_STRING("cat_name"), ui::display_text<goods_category_name>,
		goods_category_label_base
	>;

	class production_info_base : public ui::window_pane {
	public:
		economy::goods_tag tag;

		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	class production_backkground {
	public:
		economy::goods_tag tag;

		template<typename window_type>
		void windowed_update(ui::dynamic_icon<production_backkground>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class production_pop_icon {
	public:
		population::pop_type_tag type;

		template<typename window_type>
		void windowed_update(ui::dynamic_icon<production_pop_icon>& self, window_type& win, world_state& ws);
	};

	class production_pop_output {
	public:
		population::pop_type_tag type;

		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class production_input_icon {
	public:
		economy::goods_tag tag;

		template<typename window_type>
		void windowed_update(ui::dynamic_icon<production_input_icon>& self, window_type& win, world_state& ws);
	};

	class production_total_output {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class production_output_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<production_output_icon>& self, window_type& win, world_state& ws);
	};

	class production_no_production_overlay {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<production_no_production_overlay>& self, window_type& win, world_state& ws);
	};

	using production_info = ui::gui_window<
		CT_STRING("prod_producing_entry"), ui::dynamic_icon<production_backkground>,
		CT_STRING("pop_factory"), ui::dynamic_icon<production_pop_icon>,
		CT_STRING("pop_factory2"), ui::dynamic_icon<production_pop_icon>,
		CT_STRING("output"), ui::display_text<production_pop_output>,
		CT_STRING("output2"), ui::display_text<production_pop_output>,
		CT_STRING("input_factory_0"), ui::dynamic_icon<production_input_icon>,
		CT_STRING("input_factory_1"), ui::dynamic_icon<production_input_icon>,
		CT_STRING("input_factory_2"), ui::dynamic_icon<production_input_icon>,
		CT_STRING("input_factory_3"), ui::dynamic_icon<production_input_icon>,
		CT_STRING("output_total"), ui::display_text<production_total_output>,
		CT_STRING("prod_producing_not_total"), ui::dynamic_icon<production_no_production_overlay>,
		CT_STRING("output_factory"), ui::simple_button<production_output_icon>,
		production_info_base
	>;

	class production_info_pane_base : public ui::window_pane {
	public:
		std::vector<goods_category_label> categories;
		std::vector<production_info> production_details;

		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	using production_info_pane = ui::gui_window<production_info_pane_base>;

	class project_item_base : public ui::visible_region {
	public:
		nations::state_tag location;
		void set_value(nations::state_tag t) {
			location = t;
		}
		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	class project_state_name {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class project_resource_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<project_resource_icon>& self, window_type& win, world_state& ws);
	};

	class project_infrastructure_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<project_infrastructure_icon>& self, window_type& win, world_state& ws);
	};

	class project_name {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class project_cost {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class project_investor_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<project_investor_icon>& self, window_type& win, world_state& ws);
	};

	class project_investor_amount {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class project_invest_button {
	public:
		nations::state_tag location;

		template<typename window_type>
		void windowed_update(ui::button<project_invest_button>& self, window_type& win, world_state& ws);
		void button_function(ui::button<project_invest_button>& self, key_modifiers mod, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using project_item = ui::gui_window<
		CT_STRING("state_name"), ui::display_text<project_state_name>,
		CT_STRING("project_icon"), ui::dynamic_icon<project_resource_icon>,
		CT_STRING("infra"), ui::dynamic_icon<project_infrastructure_icon>,
		CT_STRING("project_name"), ui::display_text<project_name>,
		CT_STRING("project_cost"), ui::display_text<project_cost>,
		CT_STRING("pop_icon"), ui::dynamic_icon<project_investor_icon>,
		CT_STRING("pop_amount"), ui::display_text<project_investor_amount>,
		CT_STRING("invest_project"), ui::button<project_invest_button>,
		project_item_base
	>;

	class projects_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class production_window_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<close_button>,
		CT_STRING("tab_factories"), ui::button_group_member,
		CT_STRING("tab_invest"), ui::button_group_member,
		CT_STRING("tab_goodsproduction"), ui::button_group_member,
		CT_STRING("tab_popprojects"), ui::button_group_member,
		CT_STRING("production_tab_button_group"), ui::button_group<
			CT_STRING("tab_factories"),
			CT_STRING("tab_invest"),
			CT_STRING("tab_goodsproduction"),
			CT_STRING("tab_popprojects"), production_tab_button_group>,
		CT_STRING("tab_factories_text"), ui::display_text<empty_text_box>,
		CT_STRING("tab_invest_text"), ui::display_text<empty_text_box>,
		CT_STRING("tab_goodsproduction_text"), ui::display_text<empty_text_box>,
		CT_STRING("tab_popprojects_text"), ui::display_text<empty_text_box>,
		CT_STRING("investment_browser"), investment_pane,
		CT_STRING("sort_by_state"), ui::simple_button<sort_by_project_state_button>,
		CT_STRING("sort_by_projects"), ui::simple_button<sort_by_project_type_button>,
		CT_STRING("sort_by_completion"), ui::simple_button<sort_by_project_completion_button>,
		CT_STRING("sort_by_projecteers"), ui::simple_button<sort_by_project_investors_button>,
		CT_STRING("factory_buttons"), factory_filter_buttons,
		CT_STRING("invest_buttons"), investment_filter_buttons,
		CT_STRING("state_listbox"), ui::discrete_listbox<state_details_lb, state_window, nations::state_tag>,
		CT_STRING("state_listbox_invest"), ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag>,
		CT_STRING("good_production"), production_info_pane,
		CT_STRING("project_listbox"), ui::discrete_listbox<projects_lb, project_item, nations::state_tag>,
		production_window_base
	>{};

	template<typename W>
	void project_item_base::on_create(W & win, world_state & ws) {
		associated_object->align = ui::alignment::top_left;
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -1i16 };
		});
	}

	template<typename W>
	void production_info_pane_base::on_create(W & win, world_state & ws) {
		associated_object->size = ui::xy_pair{ 968i16, 700i16 };
		associated_object->position = ui::xy_pair{ 28i16, 50i16 };

		auto header_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["production_goods_name"]);
		auto& header_definition = ws.s.gui_m.ui_definitions.windows[header_tag];
		auto details_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["production_info"]);
		auto& details_definition = ws.s.gui_m.ui_definitions.windows[details_tag];

		int16_t cursor_x = 0;
		int16_t cursor_y = 0;

		categories.resize(ws.s.economy_m.good_type_names.size());
		production_details.resize(ws.s.economy_m.goods_count);

		for(uint32_t i = 0; i < ws.s.economy_m.good_type_names.size(); ++i) {
			goods_type_tag this_category(static_cast<goods_type_tag::value_base_t>(i));

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, header_tag,
				ui::tagged_gui_object{ *associated_object, win.window_object },
				categories[i]));
			categories[i].category = this_category;
			if(cursor_x != 0) {
				cursor_x = 0;
				cursor_y += details_definition.size.y;
			}
			categories[i].associated_object->size.x = 968i16;
			categories[i].associated_object->position = ui::xy_pair{ cursor_x, cursor_y };
			categories[i].associated_object->align = ui::alignment::top_left;
			cursor_y += header_definition.size.y;

			for(uint32_t j = 0; j < ws.s.economy_m.goods_count; ++j) {
				goods_tag this_good(static_cast<goods_tag::value_base_t>(j));
				if(ws.s.economy_m.goods[this_good].type == this_category) {
					ui::move_to_front(ws.w.gui_m, ui::create_static_element(
						ws, details_tag,
						ui::tagged_gui_object{ *associated_object, win.window_object },
						production_details[j]));
					production_details[j].associated_object->position = ui::xy_pair{ cursor_x, cursor_y };
					cursor_x += details_definition.size.x;
					if(cursor_x > associated_object->size.x - details_definition.size.x) {
						cursor_x = 0;
						cursor_y += details_definition.size.y;
					}
					
					production_details[j].tag = this_good;
					bool has_rgo = (ws.s.economy_m.goods[this_good].flags & economy::good_definition::has_rgo) != 0;
					int32_t count = 0;
					if(has_rgo) {
						production_details[j].template get<CT_STRING("pop_factory")>().type =
							((ws.s.economy_m.goods[this_good].flags & economy::good_definition::mined) != 0) ?
							ws.s.economy_m.rgo_mine.workers[0].type : ws.s.economy_m.rgo_farm.workers[0].type;
						production_details[j].template get<CT_STRING("output")>().type =
							((ws.s.economy_m.goods[this_good].flags & economy::good_definition::mined) != 0) ?
							ws.s.economy_m.rgo_mine.workers[0].type : ws.s.economy_m.rgo_farm.workers[0].type;
						++count;
					}
					if(is_valid_index(ws.s.economy_m.goods[this_good].factory_id)) {
						if(count == 1) {
							production_details[j].template get<CT_STRING("pop_factory2")>().type = ws.s.population_m.factory_workers[1];
							production_details[j].template get<CT_STRING("output2")>().type = ws.s.population_m.factory_workers[1];
						} else if(count == 0) {
							production_details[j].template get<CT_STRING("pop_factory")>().type = ws.s.population_m.factory_workers[1];
							production_details[j].template get<CT_STRING("output")>().type = ws.s.population_m.factory_workers[1];
						}
						++count;
					}
					if(is_valid_index(ws.s.economy_m.goods[this_good].artisan_id)) {
						if(count == 1) {
							production_details[j].template get<CT_STRING("pop_factory2")>().type = ws.s.population_m.artisan;
							production_details[j].template get<CT_STRING("output2")>().type = ws.s.population_m.artisan;
						} else if(count == 0) {
							production_details[j].template get<CT_STRING("pop_factory")>().type = ws.s.population_m.artisan;
							production_details[j].template get<CT_STRING("output")>().type = ws.s.population_m.artisan;
						}
					}

					if(is_valid_index(ws.s.economy_m.goods[this_good].factory_id)) {
						uint32_t input_count = 0;
						for(uint32_t k = 0; k < ws.s.economy_m.goods_count; ++k) {
							goods_tag this_input(static_cast<goods_tag::value_base_t>(k));
							if(ws.s.economy_m.factory_input_goods.get(ws.s.economy_m.goods[this_good].factory_id, this_input) != 0) {
								if(input_count == 0) {
									production_details[j].template get<CT_STRING("input_factory_0")>().tag = this_input;
								} else if(input_count == 1) {
									production_details[j].template get<CT_STRING("input_factory_1")>().tag = this_input;
								} else if(input_count == 2) {
									production_details[j].template get<CT_STRING("input_factory_2")>().tag = this_input;
								} else if(input_count == 3) {
									production_details[j].template get<CT_STRING("input_factory_3")>().tag = this_input;
								}
								++input_count;
							}
						}
					}
				}
			}
		}
	}

	template<typename window_type>
	void production_no_production_overlay::windowed_update(ui::dynamic_icon<production_no_production_overlay>& self, window_type & win, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			if(auto pid = player; ws.w.nation_s.nations.is_valid_index(pid)) {
				auto active_goods = ws.w.nation_s.active_goods.get_row(pid);
				if(bit_vector_test<economy::goods_tag, false>(active_goods, win.tag)) 
					ui::hide(*self.associated_object);
				else 
					ui::make_visible_immediate(*self.associated_object);
			}
		}
	}

	template<typename W>
	void production_total_output::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	}

	template<typename W>
	void production_pop_output::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	}

	template<typename window_type>
	void production_backkground::windowed_update(ui::dynamic_icon<production_backkground>& self, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<typename window_type>
	void production_output_icon::windowed_update(ui::simple_button<production_output_icon>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag)) {
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[win.tag].icon);
		}
	}

	template<typename window_type>
	void production_input_icon::windowed_update(ui::dynamic_icon<production_input_icon>& self, window_type & win, world_state & ws) {
		if(is_valid_index(tag)) {
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[tag].icon);
			ui::make_visible_immediate(*self.associated_object);
		} else
			ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void production_pop_icon::windowed_update(ui::dynamic_icon<production_pop_icon>& self, window_type & win, world_state & ws) {
		if(is_valid_index(type)) {
			self.set_frame(ws.w.gui_m, ws.s.population_m.pop_types[type].sprite - 1ui32);
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}

	template<typename W>
	inline void production_info_base::on_create(W & win, world_state & ws) {
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -2i16 };
		});
	}

	template<typename W>
	void production_window_base::on_create(W & w, world_state & ws) {
		ui::hide(*associated_object);
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
		w.template get<CT_STRING("production_tab_button_group")>().set_selected(ws.w.gui_m, 0);
	}

	template<typename W>
	void factory_display_base::on_create(W & win, world_state & ws) {
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -2i16, -11i16 };
		});
		associated_object->size.y = 92i16;
		associated_object->align = ui::alignment::top_left;
	}

	template<typename W>
	void state_pop_display_window_base::on_create(W & win, world_state & ws) {
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 5i16, 4i16 };
		});
	}

	template<typename W>
	void state_window_base::on_create(W & win, world_state & ws) {
		associated_object->align = ui::alignment::top_left;

		auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["factory_pop"]);

		constexpr int16_t size_x = 131i16;
		constexpr int16_t base_x = 470i16;

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, win.window_object },
			workers_a));
		workers_a.type = ws.s.population_m.factory_workers[0];
		workers_a.associated_object->position.y = 0i16;
		workers_a.associated_object->position.x = base_x;

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, win.window_object },
			workers_b));
		workers_b.type = ws.s.population_m.factory_workers[1];
		workers_b.associated_object->position.y = 0i16;
		workers_b.associated_object->position.x = base_x + size_x;

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, win.window_object },
			owner));
		owner.type = ws.s.population_m.capitalist;
		owner.associated_object->position.y = 0i16;
		owner.associated_object->position.x = base_x + size_x + size_x;

		auto factory_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["factory_info"]);

		for(uint32_t i = 0; i < 8; ++i) {
			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, factory_tag,
				ui::tagged_gui_object{ *associated_object, win.window_object },
				factories[i]));
			factories[i].associated_object->position = ui::xy_pair{int16_t(i * 112 + 2), 26i16 };
		}
	}

	template<typename W>
	void factory_filter_buttons_base::on_create(W & win, world_state & ws) {
		associated_object->position.y += 50i16;
		associated_object->size.y = 90i16;
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -50i16 };
		});

		auto common_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["sort_by_pop_template"]);
		constexpr int16_t size_x = 131i16;

		auto left_position = win.template get<CT_STRING("sort_by_factories")>().associated_object->position;
		auto left_size_x = win.template get<CT_STRING("sort_by_factories")>().associated_object->size.x;

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, win.window_object },
			workers_a));
		workers_a.set_text(ws, ws.s.population_m.pop_types[ws.s.population_m.factory_workers[0]].name);
		workers_a.associated_object->position = left_position;
		workers_a.associated_object->position.x += left_size_x - 1i16;

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, win.window_object },
			workers_b));
		workers_b.set_text(ws, ws.s.population_m.pop_types[ws.s.population_m.factory_workers[1]].name);
		workers_b.associated_object->position = left_position;
		workers_b.associated_object->position.x += left_size_x + size_x - 1i16;

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, common_tag,
			ui::tagged_gui_object{ *associated_object, win.window_object },
			owner));
		owner.set_text(ws, ws.s.population_m.pop_types[ws.s.population_m.capitalist].name);
		owner.associated_object->position = left_position;
		owner.associated_object->position.x += left_size_x + size_x + size_x - 1i16;
	}

	template<typename W>
	void investment_filter_buttons_base::on_create(W & win, world_state & ws) {
		associated_object->position.y += 50i16;
		associated_object->size.y = 207i16;
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -50i16 };
		});

		{
			auto common_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["sort_by_pop_template"]);
			constexpr int16_t size_x = 131i16;

			auto left_position = win.template get<CT_STRING("sort_by_factories")>().associated_object->position;
			auto left_size_x = win.template get<CT_STRING("sort_by_factories")>().associated_object->size.x;

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, win.window_object },
				workers_a));
			workers_a.set_text(ws, ws.s.population_m.pop_types[ws.s.population_m.factory_workers[0]].name);
			workers_a.associated_object->position = left_position;
			workers_a.associated_object->position.x += left_size_x - 1i16;

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, win.window_object },
				workers_b));
			workers_b.set_text(ws, ws.s.population_m.pop_types[ws.s.population_m.factory_workers[1]].name);
			workers_b.associated_object->position = left_position;
			workers_b.associated_object->position.x += left_size_x + size_x - 1i16;

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, win.window_object },
				owner));
			owner.set_text(ws, ws.s.population_m.pop_types[ws.s.population_m.capitalist].name);
			owner.associated_object->position = left_position;
			owner.associated_object->position.x += left_size_x + size_x + size_x - 1i16;
		}
		{
			auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["invest_non_gp_extra_info"]);
			for(int32_t i = 0; i < 8; ++i) {
				gp_windows[i].nth_nation = i;
				ui::move_to_front(ws.w.gui_m, ui::create_static_element(
					ws, common_tag,
					ui::tagged_gui_object{ *associated_object, win.window_object },
					gp_windows[i]));
				gp_windows[i].associated_object->position = ui::xy_pair{ int16_t(460 + (i % 4) * 134), int16_t(i < 4 ? 20 : 70) };
			}
		}
	}

	template<typename W>
	void investment_country_item_base::on_create(W & w, world_state &) {
		associated_object->position.x = 0i16;
	}
	template<typename window_type>
	void investment_country_item_background_button::windowed_update(ui::simple_button<investment_country_item_background_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		if(auto player = ws.w.local_player_nation; player && is_valid_index(tag)) {
			self.set_enabled(possible_to_invest_in(ws, player, tag));
		}
	}
	template<typename W>
	void investment_country_item_flag::windowed_update(ui::masked_flag<investment_country_item_flag>& self, W & w, world_state & ws) {
		self.set_displayed_flag(ws, w.tag);
	}
	template<typename window_type>
	void investment_country_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(win.tag), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_self_investment_text::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, nations::get_foreign_investment(ws, player, win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<int32_t index>
	template<typename window_type>
	void gp_investment_text<index>::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(std::begin(r) + index < std::end(r)) {
			if(auto id = r[index]; ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::currency, nations::get_foreign_investment(ws, id, win.tag));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	template<typename W>
	void investment_country_item_sphere_leader_flag::windowed_update(ui::masked_flag<investment_country_item_sphere_leader_flag>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			auto leader = ws.w.nation_s.nations.get<nation::sphere_leader>(w.tag);
			if(leader) {
				self.set_displayed_flag(ws, leader);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}
	template<typename window_type>
	void investment_country_item_military_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations.get<nation::military_rank>(win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_industrial_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations.get<nation::industrial_rank>(win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_prestige_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations.get<nation::prestige_rank>(win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_overall_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations.get<nation::overall_rank>(win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_opinion_type::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, nations::influence_level_to_text(ws, nations::get_influence_level(ws, player, win.tag)), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_relations_value::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, nations::get_relationship(ws, player, win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_factories_count::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			nations::country_tag t = win.tag;
			auto states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(t));
			int32_t total = 0;
			for(auto s = states.first; s != states.second; ++s) {
				if(auto si = s->state; bool(si)) {
					total += count_factories_in_state(ws, si);
				}
			}
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, total);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename lb_type>
	void investment_country_lb::populate_list(lb_type & lb, world_state & ws) {
		std::vector<nations::country_tag, concurrent_allocator<nations::country_tag>> data;

		switch(ws.w.production_w.filter) {
			default:
			case country_sub_filter::all:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::country_tag n) {
					if(auto id = n; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n)) != 0)
						data.push_back(id);
				});
				break;
			case country_sub_filter::continent_africa:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::country_tag n) {
					if(auto id = n; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n)) != 0) {
						if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) &&
							ws.s.province_m.province_container.get<province::continent>(ws.w.nation_s.nations.get<nation::current_capital>(n)) == ws.w.province_s.africa_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_asia:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::country_tag n) {
					if(auto id = n; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n)) != 0) {
						if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) &&
							ws.s.province_m.province_container.get<province::continent>(ws.w.nation_s.nations.get<nation::current_capital>(n)) == ws.w.province_s.asia_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_europe:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::country_tag n) {
					if(auto id = n; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n)) != 0) {
						if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) &&
							ws.s.province_m.province_container.get<province::continent>(ws.w.nation_s.nations.get<nation::current_capital>(n)) == ws.w.province_s.europe_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_north_america:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::country_tag n) {
					if(auto id = n; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n)) != 0) {
						if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) &&
							ws.s.province_m.province_container.get<province::continent>(ws.w.nation_s.nations.get<nation::current_capital>(n)) == ws.w.province_s.north_america_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_oceania:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::country_tag n) {
					if(auto id = n; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n)) != 0) {
						if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) &&
							ws.s.province_m.province_container.get<province::continent>(ws.w.nation_s.nations.get<nation::current_capital>(n)) == ws.w.province_s.oceania_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_south_america:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::country_tag n) {
					if(auto id = n; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(n)) != 0) {
						if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) &&
							ws.s.province_m.province_container.get<province::continent>(ws.w.nation_s.nations.get<nation::current_capital>(n)) == ws.w.province_s.south_america_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::enemy:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::opponents_in_war>(player));
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::ally:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(player));
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::neighbor:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(player));
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::sphere:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(player));
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
		}

		switch(ws.w.production_w.sort_type) {
			default:
			case country_sort::none:
				break;
			case country_sort::name:
			{
				vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
				std::sort(data.begin(), data.end(), [&ws, &lss](nations::country_tag a, nations::country_tag b) {
					auto a_name = ws.w.nation_s.nations.get<nation::name>(a);
					auto b_name = ws.w.nation_s.nations.get<nation::name>(b);
					return lss(
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
				});
			}
			break;
			case country_sort::gp_self:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return nations::get_foreign_investment(ws, player, a) > nations::get_foreign_investment(ws, player, b);
					});					
				}
				break;
			case country_sort::gp_one:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 0 < std::end(r)) {
					if(auto id = r[0]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::gp_two:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 1 < std::end(r)) {
					if(auto id = r[1]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::gp_three:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 2 < std::end(r)) {
					if(auto id = r[2]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::gp_four:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 3 < std::end(r)) {
					if(auto id = r[3]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::gp_five:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 4 < std::end(r)) {
					if(auto id = r[4]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::gp_six:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 5 < std::end(r)) {
					if(auto id = r[5]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::gp_seven:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 6 < std::end(r)) {
					if(auto id = r[6]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::gp_eight:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); std::begin(r) + 7 < std::end(r)) {
					if(auto id = r[7]; ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = id](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, n, a) > nations::get_foreign_investment(ws, n, b);
						});
					}
				}
				break;
			case country_sort::sphere_leader:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return !(ws.w.nation_s.nations.get<nation::sphere_leader>(a) < ws.w.nation_s.nations.get<nation::sphere_leader>(b));
				});
				break;
			case country_sort::prestige_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations.get<nation::prestige_rank>(a) < ws.w.nation_s.nations.get<nation::prestige_rank>(b);
				});
				break;
			case country_sort::economic_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations.get<nation::industrial_rank>(a) < ws.w.nation_s.nations.get<nation::industrial_rank>(b);
				});
				break;
			case country_sort::military_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations.get<nation::military_rank>(a) < ws.w.nation_s.nations.get<nation::military_rank>(b);
				});
				break;
			case country_sort::overall_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations.get<nation::overall_rank>(a) < ws.w.nation_s.nations.get<nation::overall_rank>(b);
				});
				break;
			case country_sort::opinion:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return nations::get_influence_level(ws, player, a) > nations::get_influence_level(ws, player, b);
					});
				}
				break;
			case country_sort::relation:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return nations::get_relationship(ws, player, a) > nations::get_relationship(ws, player, b);
					});
				}
				break;
			case country_sort::number_of_factories:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return count_factories_in_nation(ws, a) > count_factories_in_nation(ws, b);
					});
				}
				break;
		}

		lb.update_list(data.begin(), data.end());
	}

	template<typename window_type>
	void good_filter_item_button::windowed_update(ui::simple_button<good_filter_item_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<typename window_type>
	void good_filter_item_enabled_bg::windowed_update(ui::dynamic_icon<good_filter_item_enabled_bg>& self, window_type & win, world_state & ws) {
		if(ws.w.production_w.factory_goods_filters[to_index(win.tag)] != 0)
			self.set_frame(ws.w.gui_m, 1ui32);
		else
			self.set_frame(ws.w.gui_m, 0ui32);
	}

	template<typename window_type>
	void good_filter_item_type::windowed_update(ui::dynamic_icon<good_filter_item_type>& self, window_type & win, world_state & ws) {
		self.set_frame(ws.w.gui_m, uint32_t(ws.s.economy_m.goods[win.tag].icon));
	}

	template<typename W>
	void goods_filters_base::on_create(W & win, world_state & ws) {
		filter_buttons.resize(ws.s.economy_m.goods_count);

		auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["goods_filter_template"]);
		auto common_size  = ws.s.gui_m.ui_definitions.windows[common_tag].size;

		associated_object->size.y = 62;
		auto bounding_size = associated_object->size;

		int16_t cx = 0;
		int16_t cy = 0;

		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, win.window_object },
				filter_buttons[i]));
			filter_buttons[i].tag = goods_tag(static_cast<goods_tag::value_base_t>(i));
			filter_buttons[i].associated_object->position = ui::xy_pair{ cx, cy };
			cx += common_size.x;
			if(cx > bounding_size.x - common_size.x) {
				cx = 0;
				cy += common_size.y;
			}
		}

	}

	template<typename W>
	void gp_subwindow_flag::windowed_update(ui::masked_flag<gp_subwindow_flag>& self, W & w, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(std::begin(r) + w.nth_nation < std::end(r)) {
			if(auto id = r[w.nth_nation]; ws.w.nation_s.nations.is_valid_index(id)) {
				self.set_displayed_flag(ws, id);
			}
		}
	}

	template<typename W>
	void gp_subwindow_investment_value::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(std::begin(r) + w.nth_nation < std::end(r)) {
			if(auto id = r[w.nth_nation]; ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::currency, nations::get_foreign_investment(ws, id, ws.w.production_w.foreign_investment_nation));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void state_pop_display_icon::windowed_update(ui::dynamic_icon<state_pop_display_icon>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.type))
			self.set_frame(ws.w.gui_m, uint32_t(ws.s.population_m.pop_types[win.type].sprite) - 1ui32);
	}

	template<typename W>
	void state_pop_display_amount::windowed_update(W & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.type) && is_valid_index(win.in_state)) {
			if(ws.s.population_m.capitalist == win.type) {
				auto total_pop = ws.w.nation_s.state_demographics.get(win.in_state, population::total_population_tag);
				auto cap_pop = ws.w.nation_s.state_demographics.get(win.in_state, population::to_demo_tag(ws, ws.s.population_m.capitalist));

				float percent = total_pop != 0 ? float(cap_pop) / float(total_pop) : 0.0f;
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::percent, percent);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			} else {
				auto total_pop = ws.w.nation_s.state_demographics.get(win.in_state, population::to_demo_tag(ws, win.type));
				auto unemployed_pop = total_pop - ws.w.nation_s.state_demographics.get(win.in_state, population::to_employment_demo_tag(ws, win.type));

				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::exact_integer, unemployed_pop);
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void state_focus_button::windowed_update(ui::simple_button<state_focus_button>& self, window_type & win, world_state & ws) {}

	template<typename W>
	void state_name::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.states.get<state::name>(w.tag), fmt,
				ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename W>
	void state_factory_count::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, economy::count_factories_in_state(ws, w.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void state_build_factory_button::windowed_update(ui::simple_button<state_build_factory_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		//NOTE: may be either in a state owned by player OR a remote state
	}

	template<typename W>
	void state_average_infrastructure::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::fp_one_place, economy::average_railroad_level(ws, w.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename lb_type>
	void state_details_lb::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			boost::container::small_vector<nations::state_tag, 32, concurrent_allocator<nations::state_tag>> data;
			auto state_range = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(player));
			for(auto s = state_range.first; s != state_range.second; ++s) {
				
				if(auto state_id = s->state; ws.w.nation_s.states.is_valid_index(state_id)) {
					if(ws.w.production_w.show_empty_states) {
						data.push_back(state_id);
					} else {
						bool factories = false;
						auto& s_factories = ws.w.nation_s.states.get<state::factories>(state_id);
						for(uint32_t i = 0; i < state::factories_count; ++i) {
							if(auto ftype = s_factories[i].type; bool(ftype) && ws.w.production_w.factory_goods_filters[to_index(ftype->output_good)] != 0) {
								factories = true;
								break;
							}
						}
						if(factories)
							data.push_back(state_id);
					}
					
				}
				
			}

			//sort
			switch(ws.w.production_w.factory_sort_type) {
				default:
				case factories_sort::none:
					break;
				case factories_sort::state_name:
				{
					vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
					std::sort(data.begin(), data.end(), [&ws, &lss](nations::state_tag a, nations::state_tag b) {
						auto a_name = ws.w.nation_s.states.get<state::name>(a);
						auto b_name = ws.w.nation_s.states.get<state::name>(b);
						return lss(
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
					});
				}
				break;
				case factories_sort::count:
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return economy::count_factories_in_state(ws, a) > economy::count_factories_in_state(ws, b);
					});
				break;
				case factories_sort::infrastructure:
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return economy::average_railroad_level(ws, a) > economy::average_railroad_level(ws, b);
					});
				break;
				case factories_sort::owner_pop:
				{
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						auto a_total_pop = float(ws.w.nation_s.state_demographics.get(a, population::total_population_tag));
						auto b_total_pop = float(ws.w.nation_s.state_demographics.get(b, population::total_population_tag));
						return (float(ws.w.nation_s.state_demographics.get(a, population::to_demo_tag(ws, ws.s.population_m.capitalist))) * b_total_pop)
							> (ws.w.nation_s.state_demographics.get(b, population::to_demo_tag(ws, ws.s.population_m.capitalist)) * a_total_pop);
					});
				}
				break;
				case factories_sort::worker_pop_a:
				{
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return (ws.w.nation_s.state_demographics.get(a, population::to_demo_tag(ws, ws.s.population_m.factory_workers[0])) - 
							ws.w.nation_s.state_demographics.get(a, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[0])))
							> (ws.w.nation_s.state_demographics.get(b, population::to_demo_tag(ws, ws.s.population_m.factory_workers[0])) -
								ws.w.nation_s.state_demographics.get(b, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[0])));
					});
				}
				break;
				case factories_sort::worker_pop_b:
				{
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return (ws.w.nation_s.state_demographics.get(a, population::to_demo_tag(ws, ws.s.population_m.factory_workers[1])) -
							ws.w.nation_s.state_demographics.get(a, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[1])))
							> (ws.w.nation_s.state_demographics.get(b, population::to_demo_tag(ws, ws.s.population_m.factory_workers[1])) -
								ws.w.nation_s.state_demographics.get(b, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[1])));
					});
				}
				break;
			}

			lb.update_list(data.begin(), data.end());
		}
	}

	template<typename lb_type>
	void projects_lb::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto state_range = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(player));
			boost::container::small_vector<nations::state_tag, 32, concurrent_allocator<nations::state_tag>> data;

			for(auto s = state_range.first; s != state_range.second; ++s) {
				if(auto state_id = s->state; ws.w.nation_s.states.is_valid_index(state_id)) {
					if(ws.w.nation_s.states.get<state::project>(state_id).type != economy::pop_project_type::none) {
						data.push_back(state_id);
					}
				}
			}

			switch(ws.w.production_w.project_sort_type) {
				default:
				case project_sort::state:
				{
					vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
					std::sort(data.begin(), data.end(), [&ws, &lss](nations::state_tag a, nations::state_tag b) {
						auto a_name = ws.w.nation_s.states.get<state::name>(a);
						auto b_name = ws.w.nation_s.states.get<state::name>(b);
						return lss(
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
					});
				}
					break;
				case project_sort::project_type:
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						auto a_type = ws.w.nation_s.states.get<state::project>(a).type;
						auto b_type = ws.w.nation_s.states.get<state::project>(b).type;
						if(a_type != b_type)
							return a_type > b_type;
						if(a_type == economy::pop_project_type::factory) {
							auto fta = ws.w.nation_s.states.get<state::project>(a).factory_type;
							auto ftb = ws.w.nation_s.states.get<state::project>(b).factory_type;
							if(is_valid_index(fta) && is_valid_index(ftb)) {
								return economy::get_factory_project_type(ws, a, fta) < economy::get_factory_project_type(ws, b, ftb);
							}
						}
						return false;
					});
					break;
				case project_sort::completion:
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return project_completion(ws, a, state_current_prices(ws, a)) > project_completion(ws, b, state_current_prices(ws, b));
					});
					//project_completion
					break;
				case project_sort::investors:
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return ws.w.nation_s.states.get<state::project>(a).funds > ws.w.nation_s.states.get<state::project>(b).funds;
					});
					break;
			}

			lb.update_list(data.begin(), data.end());
		}
	}

	template<typename lb_type>
	void investment_state_details_lb::populate_list(lb_type & lb, world_state & ws) {
		if(is_valid_index(ws.w.production_w.foreign_investment_nation)) {
			boost::container::small_vector<nations::state_tag, 32, concurrent_allocator<nations::state_tag>> data;
			auto state_range = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(ws.w.production_w.foreign_investment_nation));
			for(auto s = state_range.first; s != state_range.second; ++s) {

				if(auto state_id = s->state; ws.w.nation_s.states.is_valid_index(state_id)) {
					if(ws.w.production_w.show_empty_states) {
						data.push_back(state_id);
					} else {
						bool factories = false;
						auto& s_factories = ws.w.nation_s.states.get<state::factories>(state_id);
						for(uint32_t i = 0; i < state::factories_count; ++i) {
							if(auto ftype = s_factories[i].type; bool(ftype) && ws.w.production_w.factory_goods_filters[to_index(ftype->output_good)] != 0) {
								factories = true;
								break;
							}
						}
						if(factories)
							data.push_back(state_id);
					}
				}
			}

			//sort
			switch(ws.w.production_w.factory_sort_type) {
				default:
				case factories_sort::none:
					break;
				case factories_sort::state_name:
				{
					vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
					std::sort(data.begin(), data.end(), [&ws, &lss](nations::state_tag a, nations::state_tag b) {
						auto a_name = ws.w.nation_s.states.get<state::name>(a);
						auto b_name = ws.w.nation_s.states.get<state::name>(b);
						return lss(
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
					});
				}
				break;
				case factories_sort::count:
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return economy::count_factories_in_state(ws, a) > economy::count_factories_in_state(ws, b);
					});
					break;
				case factories_sort::infrastructure:
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return economy::average_railroad_level(ws, a) > economy::average_railroad_level(ws, b);
					});
					break;
				case factories_sort::owner_pop:
				{
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						auto a_total_pop = float(ws.w.nation_s.state_demographics.get(a, population::total_population_tag));
						auto b_total_pop = float(ws.w.nation_s.state_demographics.get(b, population::total_population_tag));
						return (float(ws.w.nation_s.state_demographics.get(a, population::to_demo_tag(ws, ws.s.population_m.capitalist))) * b_total_pop)
							> (ws.w.nation_s.state_demographics.get(b, population::to_demo_tag(ws, ws.s.population_m.capitalist)) * a_total_pop);
					});
				}
				break;
				case factories_sort::worker_pop_a:
				{
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return (ws.w.nation_s.state_demographics.get(a, population::to_demo_tag(ws, ws.s.population_m.factory_workers[0])) -
							ws.w.nation_s.state_demographics.get(a, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[0])))
							> (ws.w.nation_s.state_demographics.get(b, population::to_demo_tag(ws, ws.s.population_m.factory_workers[0])) -
								ws.w.nation_s.state_demographics.get(b, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[0])));
					});
				}
				break;
				case factories_sort::worker_pop_b:
				{
					std::sort(data.begin(), data.end(), [&ws](nations::state_tag a, nations::state_tag b) {
						return (ws.w.nation_s.state_demographics.get(a, population::to_demo_tag(ws, ws.s.population_m.factory_workers[1])) -
							ws.w.nation_s.state_demographics.get(a, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[1])))
							> (ws.w.nation_s.state_demographics.get(b, population::to_demo_tag(ws, ws.s.population_m.factory_workers[1])) -
								ws.w.nation_s.state_demographics.get(b, population::to_employment_demo_tag(ws, ws.s.population_m.factory_workers[1])));
					});
				}
				break;
			}

			lb.update_list(data.begin(), data.end());
		}
	}

	template<typename window_type>
	void factory_open_background::windowed_update(ui::dynamic_icon<factory_open_background>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(!factory_is_closed(f))
				ui::make_visible_immediate(*self.associated_object);
			else
				ui::hide(*self.associated_object);
		}
	}

	template<typename window_type>
	void factory_closed_background::windowed_update(ui::dynamic_icon<factory_closed_background>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(!factory_is_closed(f))
				ui::hide(*self.associated_object);
			else
				ui::make_visible_immediate(*self.associated_object);
		}
	}

	template<typename window_type>
	void factory_construction_progress_bar::windowed_update(ui::progress_bar<factory_construction_progress_bar>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(factory_is_under_construction(f)) {
				self.set_fraction(f.factory_progress);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename window_type>
	void factory_upgrade_progress_bar::windowed_update(ui::progress_bar<factory_upgrade_progress_bar>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(factory_is_upgrading(f)) {
				self.set_fraction(f.factory_progress);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename window_type>
	void factory_upgrade_progress_overlay::windowed_update(ui::dynamic_icon<factory_upgrade_progress_overlay>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(factory_is_upgrading(f)) {
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename W>
	void factory_level_text::windowed_update(W & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, f.level);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void factory_cancel_construction_button::windowed_update(ui::simple_button<factory_cancel_construction_button>& self, window_type & win, world_state & ws) {
		location = win.location;
		index = win.index;
		if(auto player = ws.w.local_player_nation; player) {
			if(win.index >= 0 && is_valid_index(win.location) && ws.w.nation_s.states.get<state::owner>(location) == player) {
				economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
				if(f.factory_progress > 0.0f && f.level == 0) {
					ui::make_visible_immediate(*self.associated_object);
					self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::destroy_factory) != 0);
					return;
				}
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void factory_delete_button::windowed_update(ui::simple_button<factory_delete_button>& self, window_type & win, world_state & ws) {
		location = win.location;
		index = win.index;
		if(auto player = ws.w.local_player_nation; player) {
			if(win.index >= 0 && is_valid_index(win.location) && ws.w.nation_s.states.get<state::owner>(location) == player) {
				economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
				if(factory_is_closed(f)) {
					ui::make_visible_immediate(*self.associated_object);
					self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::destroy_factory) != 0);
					return;
				}
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void factory_open_or_close_button::windowed_update(ui::simple_button<factory_open_or_close_button>& self, window_type & win, world_state & ws) {
		location = win.location;
		index = win.index;
		if(auto player = ws.w.local_player_nation; player) {
			if(win.index >= 0 && is_valid_index(win.location)) {
				economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];

				if(factory_is_closed(f)) {
					// closed case -- button opens
					self.set_frame(ws.w.gui_m, 0ui32);
					ui::make_visible_immediate(*self.associated_object);
					if(ws.w.nation_s.states.get<state::owner>(location) == player) {
						self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::open_factory) != 0);
						return;
					} else {
						self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::open_factory_invest) != 0);
						return;
					}
				} else {
					// open case -- button closes
					self.set_frame(ws.w.gui_m, 1ui32);
					if(ws.w.nation_s.states.get<state::owner>(location) == player) {
						ui::make_visible_immediate(*self.associated_object);
						self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::destroy_factory) != 0);
						return;
					}
				}
			}
		}
		ui::hide(*self.associated_object);

	}

	template<typename window_type>
	void factory_upgrade_button::windowed_update(ui::button<factory_upgrade_button>& self, window_type & win, world_state & ws) {
		location = win.location;
		index = win.index;
		if(auto player = ws.w.local_player_nation; player) {
			if(win.index >= 0 && is_valid_index(win.location)) {
				economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
				if(ws.w.nation_s.states.get<state::owner>(location) == player && f.level != 0) {
					ui::make_visible_immediate(*self.associated_object);
					self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::expand_factory) != 0 && f.factory_progress == 0.0f);
					return;
				}
				if(f.level != 0) {
					ui::make_visible_immediate(*self.associated_object);
					self.set_enabled(f.factory_progress == 0.0f && (ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::expand_factory_invest) != 0);
					return;
				}
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void factory_subsidize_button::windowed_update(ui::simple_button<factory_subsidize_button>& self, window_type & win, world_state & ws) {
		location = win.location;
		index = win.index;
		if(auto player = ws.w.local_player_nation; player) {
			if(win.index >= 0 && is_valid_index(win.location) && ws.w.nation_s.states.get<state::owner>(location) == player) {
				economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];

				if((f.flags & economy::factory_instance::is_subsidized) != 0)
					self.set_frame(ws.w.gui_m, 1ui32);
				else
					self.set_frame(ws.w.gui_m, 0ui32);

				self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::can_subsidise) != 0);
				return;
			}
		}
		ui::hide(*self.associated_object);
	}

	template<int32_t nth_input>
	template<typename window_type>
	void factory_input<nth_input>::windowed_update(ui::dynamic_icon<factory_input>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(auto f_type = f.type; f_type) {
				auto type_id = f_type->id;
				int32_t input_found_count = 0;
				for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
					goods_tag this_tag(static_cast<goods_tag::value_base_t>(i));
					if(ws.s.economy_m.factory_input_goods.get(type_id, this_tag) != 0) {
						if(input_found_count == nth_input) {
							self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[this_tag].icon);
							ui::make_visible_immediate(*self.associated_object);
							return;
						}
						++input_found_count;
					}
				}
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void factory_output::windowed_update(ui::simple_button<factory_output>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(auto f_type = f.type; f_type) {
				if(is_valid_index(f_type->output_good)) {
					self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[f_type->output_good].icon);
					ui::make_visible_immediate(*self.associated_object);
					return;
				}
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void factory_income_arrow::windowed_update(ui::dynamic_icon<factory_income_arrow>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];

			if(auto state_cap = nations::get_state_capital(ws, win.location); is_valid_index(state_cap)) {
				auto profit = economy::get_factory_profit(ws, state_cap, f, state_current_prices(ws, win.location));

				if(profit < money_qnty_type(0)) {
					self.set_frame(ws.w.gui_m, 1ui32);
					return;
				}
			}
		}
		self.set_frame(ws.w.gui_m, 0ui32);
	}
	template<typename W>
	void factory_income_text::windowed_update(W & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			
			if(auto state_cap = nations::get_state_capital(ws, win.location); is_valid_index(state_cap)) {
				ui::text_format blk{ui::text_color::black, fmt.font_handle, fmt.font_size};

				auto profit = economy::get_factory_profit(ws, state_cap, f, state_current_prices(ws, win.location));

				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::fp_two_places, profit);
				auto cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, blk, lm);
				cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u00A3"), box, cursor, blk, lm);
				lm.finish_current_line();
			}
		}
	}
	template<typename window_type>
	void factory_workers_display::windowed_update(ui::dynamic_icon<factory_workers_display>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			auto employment_fraction = factory_employment_fraction(ws, f);
			if(employment_fraction != 0.0f)	
				self.set_frame(ws.w.gui_m, 1ui32 + std::min(9ui32, static_cast<uint32_t>(10.0f * employment_fraction)));
			else
				self.set_frame(ws.w.gui_m, 0ui32);
		}
	}

	template<typename window_type>
	void factory_closed_overlay::windowed_update(ui::dynamic_icon<factory_closed_overlay>& self, window_type & win, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(!factory_is_closed(f))
				ui::hide(*self.associated_object);
			else
				ui::make_visible_immediate(*self.associated_object);
		}
	}

	template<typename W>
	void factory_closed_text::windowed_update(W & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(win.index >= 0 && is_valid_index(win.location)) {
			economy::factory_instance& f = ws.w.nation_s.states.get<state::factories>(win.location)[win.index];
			if(factory_is_closed(f)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::closed], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename W>
	void goods_category_name::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.category)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.good_type_names[w.category], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename W>
	void project_state_name::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.location)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.states.get<state::name>(w.location), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void project_resource_icon::windowed_update(ui::dynamic_icon<project_resource_icon>& self, window_type & w, world_state & ws) {
		if(is_valid_index(w.location)) {
			if(auto ft = ws.w.nation_s.states.get<state::project>(w.location).factory_type; is_valid_index(ft) && ws.w.nation_s.states.get<state::project>(w.location).type == economy::pop_project_type::factory) {
				self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[ws.s.economy_m.factory_types[ft].output_good].icon);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename window_type>
	void project_infrastructure_icon::windowed_update(ui::dynamic_icon<project_infrastructure_icon>& self, window_type & w, world_state & ws) {
		if(is_valid_index(w.location)) {
			if(ws.w.nation_s.states.get<state::project>(w.location).type == economy::pop_project_type::railroad)
				ui::make_visible_immediate(*self.associated_object);
			else
				ui::hide(*self.associated_object);
		}
	}

	template<typename W>
	void project_investor_amount::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.location)) {

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, ws.w.nation_s.states.get<state::project>(w.location).funds);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename W>
	void project_cost::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.location)) {

			economy::money_qnty_type cost = [&ws, sid = w.location]() {
				if(auto ft = ws.w.nation_s.states.get<state::project>(sid).factory_type; is_valid_index(ft) && ws.w.nation_s.states.get<state::project>(sid).type == economy::pop_project_type::factory) {
					return economy::get_factory_project_cost(ws, ft, economy::get_factory_project_type(ws, sid, ft), state_current_prices(ws, sid));
				} else if(ws.w.nation_s.states.get<state::project>(sid).type == economy::pop_project_type::railroad) {
					return economy::get_railroad_cost(ws, state_current_prices(ws, sid));
				}
				return economy::money_qnty_type(0);
			}();
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, cost);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename W>
	void project_name::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(w.location)) {
			
			if(ws.w.nation_s.states.get<state::project>(w.location).type == economy::pop_project_type::railroad) {
				if(auto p = ws.w.nation_s.states.get<state::project>(w.location).location; is_valid_index(p)) {
					ui::xy_pair cursor{ 0,0 };
					if(ws.w.province_s.province_state_container.get<province_state::railroad_level>(p) == 0) 
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::build], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					else 
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::expand], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
					ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::railroad], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					lm.finish_current_line();
				}
			} else if(ws.w.nation_s.states.get<state::project>(w.location).type == economy::pop_project_type::factory) {
				if(auto ft = ws.w.nation_s.states.get<state::project>(w.location).factory_type; is_valid_index(ft)) {
					auto ftype = economy::get_factory_project_type(ws, w.location, ft);
					ui::xy_pair cursor{ 0,0 };
					if(ftype == economy::factory_project_type::expand)
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::expand], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					else if(ftype == economy::factory_project_type::reopen)
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::reopen], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					else if(ftype == economy::factory_project_type::open)
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::build], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
					ui::add_linear_text(cursor, ws.s.economy_m.factory_types[ft].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					lm.finish_current_line();
				}
			}
		}
	}

	template<typename window_type>
	void project_investor_icon::windowed_update(ui::dynamic_icon<project_investor_icon>& self, window_type & win, world_state & ws) {
		self.set_frame(ws.w.gui_m, ws.s.population_m.pop_types[ws.s.population_m.capitalist].sprite - 1ui32);
	}

	template<typename window_type>
	void project_invest_button::windowed_update(ui::button<project_invest_button>& self, window_type & win, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::can_invest_in_pop_projects) != 0);
		}
	}
}
