#pragma once
#include "common\\common.h"
#include "nations.h"
#include "gui\\gui.h"

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
		bool has_tooltip(world_state&) { return is_valid_index(war); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class join_defender_button {
	public:
		military::war_tag war;
		template<typename window_type>
		void windowed_update(ui::simple_button<join_defender_button>&, window_type& win, world_state&);
		void button_function(ui::simple_button<join_defender_button>&, world_state&);
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
		void button_function(ui::masked_flag<flag_sub_item>&, world_state&);
		template<typename W>
		void windowed_update(ui::masked_flag<flag_sub_item>& self, W& w, world_state& ws);
	};

	class wg_sub_item {
	public:
		military::war_goal this_wg;
		template<typename window_type>
		void windowed_update(ui::simple_button<wg_sub_item>&, window_type& win, world_state&);
		void button_function(ui::simple_button<wg_sub_item>&, world_state&);
		bool has_tooltip(world_state&) { return is_valid_index(this_wg.cb_type); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using flag_item = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<flag_sub_item>,
		flag_item_base
	>;

	using wg_item = ui::gui_window<
		CT_STRING("wargoal_icon"), ui::simple_button<wg_sub_item>,
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
		CT_STRING("attackers_mil_strength"), ui::display_text<attacker_strength>,
		CT_STRING("defenders_mil_strength"), ui::display_text<defender_strength>,
		CT_STRING("warscore"), ui::progress_bar<war_score_progress_bar>,
		CT_STRING("diplo_war_progress_overlay"), ui::dynamic_icon<war_score_overlay>,
		CT_STRING("diplo_warscore_marker1"), ui::dynamic_icon<attacker_wg_marker>,
		CT_STRING("diplo_warscore_marker2"), ui::dynamic_icon<defender_wg_marker>,
		CT_STRING("warscore_text"), ui::display_text<war_score_text_box>,
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
		diplomacy_window_base
	>;
	
	

	template<typename window_type>
	void war_score_progress_bar::windowed_update(ui::progress_bar<war_score_progress_bar>& bar, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			bar.set_fraction((this_war.current_war_score + 1.0f) / 2.0f)
		}
	}

	template<typename window_type>
	void war_score_overlay::windowed_update(ui::dynamic_icon<war_score_overlay>& self, window_type & win, world_state & ws) {
		war = win.war;
	}

}
