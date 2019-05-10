#pragma once
#include "common\\common.h"
#include "topbar.h"
#include "world_state\\world_state.h"
#include "gui\\gui.hpp"

namespace current_state {
	class player_flag {
	public:
		void update(ui::masked_flag<player_flag>& self, world_state& ws);
	};

	class player_flag_frame {
	public:
		void update(ui::dynamic_transparent_icon<player_flag_frame>& ico, world_state& ws);
	};

	class production_button {
	public:
		void button_function(ui::simple_button<production_button>&, world_state&);
		void update(ui::simple_button<production_button>& self, world_state& ws);
	};

	class topbar_base : public ui::gui_behavior {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	template<int32_t nth>
	class top_produced_icon {
	public:
		economy::goods_tag tag;
		void update(ui::dynamic_icon<top_produced_icon<nth>>& self, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(tag); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class building_factories {
	public:
		void update(ui::dynamic_icon<building_factories>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class closed_factories {
	public:
		void update(ui::dynamic_icon<closed_factories>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class unemployed_workers {
	public:
		void update(ui::dynamic_icon<unemployed_workers>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class budget_button {
	public:
		void button_function(ui::simple_button<budget_button>&, world_state&);
		void update(ui::simple_button<budget_button>& self, world_state& ws);
	};

	class budget_linechart {
	public:
		void update(ui::linechart<budget_linechart, 32>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw, int32_t x);
	};

	class treasury_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class tech_button {
	public:
		void button_function(ui::simple_button<tech_button>&, world_state&);
		void update(ui::simple_button<tech_button>& self, world_state& ws);
	};

	class current_research_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class literacy_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class research_progress_bar {
	public:
		void update(ui::progress_bar<research_progress_bar>& bar, world_state& ws);
	};

	class research_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class research_points {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class politics_button {
	public:
		void button_function(ui::simple_button<politics_button>&, world_state&);
		void update(ui::simple_button<politics_button>& self, world_state& ws);
	};

	class party_icon {
	public:
		void update(ui::tinted_icon<party_icon>& self, world_state& ws);
	};

	class party_name_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class suppression_points_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class infamy_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class reform_alert {
	public:
		void button_function(ui::simple_button<reform_alert>&, world_state&);
		void update(ui::simple_button<reform_alert>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class decision_alert {
	public:
		void button_function(ui::simple_button<decision_alert>&, world_state&);
		void update(ui::simple_button<decision_alert>& self, world_state &ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class election_alert {
	public:
		void update(ui::dynamic_icon<election_alert>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class rebels_alert {
	public:
		void button_function(ui::simple_button<rebels_alert>&, world_state& ws);
		void update(ui::simple_button<rebels_alert>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class pops_button {
	public:
		void button_function(ui::simple_button<pops_button>&, world_state&);
		void update(ui::simple_button<pops_button>& self, world_state& ws);
	};

	class topbar_total_population_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_national_focus_count {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_militancy {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_consciousness {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class trade_button {
	public:
		void button_function(ui::simple_button<trade_button>&, world_state&);
		void update(ui::simple_button<trade_button>& self, world_state& ws);
	};

	template<int32_t nth>
	class top_imported_icon {
	public:
		economy::goods_tag tag;
		void update(ui::dynamic_icon<top_imported_icon<nth>>& self, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(tag); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	template<int32_t nth>
	class top_exported_icon {
	public:
		economy::goods_tag tag;
		void update(ui::dynamic_icon<top_exported_icon<nth>>& self, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(tag); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class diplomacy_button {
	public:
		void button_function(ui::simple_button<diplomacy_button>&, world_state&);
		void update(ui::simple_button<diplomacy_button>& self, world_state& ws);
	};

	class war_against_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class war_against_flag_button {
	public:
		cultures::national_tag tag;

		war_against_flag_button(cultures::national_tag t) : tag(t) {}

		void update(ui::masked_flag<war_against_flag_button>& self, world_state& ws);
	};

	using war_against_flag = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<war_against_flag_button>,
		ui::gui_behavior
	>;

	class diplomatic_points_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class at_peace_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class colonial_alert {
	public:
		void button_function(ui::simple_button<colonial_alert>&, world_state&);
		void update(ui::simple_button<colonial_alert>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class crisis_alert {
	public:
		void button_function(ui::simple_button<crisis_alert>&, world_state&);
		void update(ui::simple_button<crisis_alert>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class influence_alert {
	public:
		void button_function(ui::simple_button<influence_alert>&, world_state&);
		void update(ui::simple_button<influence_alert>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};
	class losing_gp_alert {
	public:
		void update(ui::dynamic_icon<losing_gp_alert>& self, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class military_button {
	public:
		void button_function(ui::simple_button<military_button>&, world_state&);
		void update(ui::simple_button<military_button>& self, world_state& ws);
	};

	class army_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class navy_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class manpower_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class leadership_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class outliner_button {
	public:
		void button_function(ui::simple_button<outliner_button>&, world_state&);
		void update(ui::simple_button<outliner_button>&, world_state&);
	};

	class topbar_overall_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_prestige_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_military_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_industrial_rank {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_prestige {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_military_score {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_industrial_score {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_colonial_points {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class topbar_country_name {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class pause_button {
	public:
		void button_function(ui::simple_button<pause_button>&, world_state& ws);
	};

	class increase_speed_button {
	public:
		void button_function(ui::simple_button<increase_speed_button>&, world_state& ws);
	};

	class decrease_speed_button {
	public:
		void button_function(ui::simple_button<decrease_speed_button>&, world_state& ws);
	};

	class speed_indicator_button {
	public:
		void update(ui::simple_button<speed_indicator_button>& self, world_state& ws);
	};

	class topbar_date {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};


	class topbar_t : public ui::gui_window <
		CT_STRING("topbar_bg"), ui::background_icon,
		CT_STRING("topbar_paper"), ui::background_icon,
		CT_STRING("player_flag"), ui::masked_flag<player_flag>,
		CT_STRING("topbar_flag_overlay"), ui::dynamic_transparent_icon<player_flag_frame>,
		CT_STRING("topbarbutton_production"), ui::simple_button<production_button>,
		CT_STRING("topbar_produced0"), ui::dynamic_icon<top_produced_icon<0>>,
		CT_STRING("topbar_produced1"), ui::dynamic_icon<top_produced_icon<1>>,
		CT_STRING("topbar_produced2"), ui::dynamic_icon<top_produced_icon<2>>,
		CT_STRING("topbar_produced3"), ui::dynamic_icon<top_produced_icon<3>>,
		CT_STRING("topbar_produced4"), ui::dynamic_icon<top_produced_icon<4>>,
		CT_STRING("alert_building_factories"), ui::dynamic_icon<building_factories>,
		CT_STRING("alert_closed_factories"), ui::dynamic_icon<closed_factories>,
		CT_STRING("alert_unemployed_workers"), ui::dynamic_icon<unemployed_workers>,
		CT_STRING("topbarbutton_budget"), ui::simple_button<budget_button>,
		CT_STRING("budget_linechart"), ui::linechart<budget_linechart, 32>,
		CT_STRING("budget_funds"), ui::display_text<treasury_text_box>,
		CT_STRING("topbarbutton_tech"), ui::simple_button<tech_button>,
		CT_STRING("topbar_tech_progress"), ui::progress_bar<research_progress_bar>,
		CT_STRING("tech_current_research"), ui::display_text<current_research_text_box>,
		CT_STRING("tech_literacy_value"), ui::display_text<literacy_text_box>,
		CT_STRING("topbar_researchpoints"), ui::dynamic_icon<research_icon>,
		CT_STRING("topbar_researchpoints_value"), ui::display_text<research_points>,
		CT_STRING("topbarbutton_politics"), ui::simple_button<politics_button>,
		CT_STRING("politics_party_icon"), ui::tinted_icon<party_icon>,
		CT_STRING("politics_ruling_party"), ui::display_text<party_name_text_box>,
		CT_STRING("politics_supressionpoints_value"), ui::display_text<suppression_points_text_box>,
		CT_STRING("politics_infamy_value"), ui::display_text<infamy_text_box>,
		CT_STRING("alert_can_do_reforms"), ui::simple_button<reform_alert>,
		CT_STRING("alert_can_do_decisions"), ui::simple_button<decision_alert>,
		CT_STRING("alert_is_in_election"), ui::dynamic_icon<election_alert>,
		CT_STRING("alert_have_rebels"), ui::simple_button<rebels_alert>,
		CT_STRING("topbarbutton_pops"), ui::simple_button<pops_button>,
		CT_STRING("population_total_value"), ui::display_text<topbar_total_population_text_box>,
		CT_STRING("topbar_focus_value"), ui::display_text<topbar_national_focus_count>,
		CT_STRING("population_avg_mil_value"), ui::display_text<topbar_militancy>,
		CT_STRING("population_avg_con_value"), ui::display_text<topbar_consciousness>,
		CT_STRING("topbarbutton_trade"), ui::simple_button<trade_button>,
		CT_STRING("topbar_import0"), ui::dynamic_icon<top_imported_icon<0>>,
		CT_STRING("topbar_import1"), ui::dynamic_icon<top_imported_icon<1>>,
		CT_STRING("topbar_import2"), ui::dynamic_icon<top_imported_icon<2>>,
		CT_STRING("topbar_export0"), ui::dynamic_icon<top_exported_icon<0>>,
		CT_STRING("topbar_export1"), ui::dynamic_icon<top_exported_icon<1>>,
		CT_STRING("topbar_export2"), ui::dynamic_icon<top_exported_icon<2>>,
		CT_STRING("topbarbutton_diplomacy"), ui::simple_button<diplomacy_button>,
		CT_STRING("diplomacy_status"), ui::display_text<at_peace_text_box>,
		CT_STRING("diplomacy_at_war"), ui::overlap_box<war_against_lb, ui::window_tag, war_against_flag>,
		CT_STRING("diplomacy_diplopoints_value"), ui::display_text<diplomatic_points_text_box>,
		CT_STRING("alert_colony"), ui::simple_button<colonial_alert>,
		CT_STRING("alert_crisis"), ui::simple_button<crisis_alert>,
		CT_STRING("alert_can_increase_opinion"), ui::simple_button<influence_alert>,
		CT_STRING("alert_loosing_gp"), ui::dynamic_icon<losing_gp_alert>,
		CT_STRING("topbarbutton_military"), ui::simple_button<military_button>,
		CT_STRING("military_army_value"), ui::display_text<army_text_box>,
		CT_STRING("military_navy_value"), ui::display_text<navy_text_box>,
		CT_STRING("military_manpower_value"), ui::display_text<manpower_text_box>,
		CT_STRING("military_leadership_value"), ui::display_text<leadership_text_box>,
		CT_STRING("topbar_outlinerbutton"), ui::simple_button<outliner_button>,
		CT_STRING("nation_totalrank"), ui::display_text<topbar_overall_rank, -18>,
		CT_STRING("country_prestige"), ui::display_text<topbar_prestige, -8>,
		CT_STRING("selected_prestige_rank"), ui::display_text<topbar_prestige_rank, -15>,
		CT_STRING("country_economic"), ui::display_text<topbar_industrial_score, -8>,
		CT_STRING("selected_industry_rank"), ui::display_text<topbar_industrial_rank, -15>,
		CT_STRING("country_military"), ui::display_text<topbar_military_score, -8>,
		CT_STRING("selected_military_rank"), ui::display_text<topbar_military_rank, -15>,
		CT_STRING("country_colonial_power"), ui::display_text<topbar_colonial_points, -15>,
		CT_STRING("button_speedup"), ui::simple_button<increase_speed_button>,
		CT_STRING("button_speeddown"), ui::simple_button<decrease_speed_button>,
		CT_STRING("pause_bg"), ui::simple_button<pause_button>,
		CT_STRING("speed_indicator"), ui::simple_button<speed_indicator_button>,
		CT_STRING("DateText"), ui::display_text<topbar_date, -8>,
		CT_STRING("CountryName"), ui::display_text<topbar_country_name, -14>,
		topbar_base
	> {};

	template<typename W>
	void topbar_base::on_create(W& w, world_state& ws) {
		w.associated_object->size.y = 160i16;
		w.associated_object->size.x = int16_t(ws.w.gui_m.width());

		w.get<CT_STRING("topbar_bg")>().associated_object->flags.fetch_or(ui::gui_object::force_transparency_check, std::memory_order_acq_rel);
	}

	template<int32_t nth>
	inline void top_produced_icon<nth>::update(ui::dynamic_icon<top_produced_icon<nth>>& self, world_state & ws) {
		if(!is_valid_index(tag))
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[tag].icon);
	}

	template<int32_t nth>
	inline void top_produced_icon<nth>::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tag))
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[tag].name, ui::tooltip_text_format, ws, tw);
	}

	template<int32_t nth>
	inline void top_imported_icon<nth>::update(ui::dynamic_icon<top_imported_icon<nth>>& self, world_state & ws) {
		if(!is_valid_index(tag))
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[tag].icon);
	}

	template<int32_t nth>
	inline void top_imported_icon<nth>::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tag))
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[tag].name, ui::tooltip_text_format, ws, tw);
	}

	template<int32_t nth>
	inline void top_exported_icon<nth>::update(ui::dynamic_icon<top_exported_icon<nth>>& self, world_state & ws) {
		if(!is_valid_index(tag))
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[tag].icon);
	}

	template<int32_t nth>
	inline void top_exported_icon<nth>::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tag))
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[tag].name, ui::tooltip_text_format, ws, tw);
	}

	template<typename lb_type>
	void war_against_lb::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto against_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::opponents_in_war>(player));
			for(auto against : against_range) {
				if(is_valid_index(against) && is_valid_index(ws.w.nation_s.nations.get<nation::tag>(against))) {
					lb.add_item(ws, ws.w.nation_s.nations.get<nation::tag>(against));
				}
			}
		}
	}
}
