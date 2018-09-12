#pragma once
#include "common\\common.h"
#include "gui\\gui.hpp"

class world_state;

namespace current_state {
	class topbar_bg {
	public:
		void button_function(ui::simple_button<topbar_bg>&, world_state&) {} // just to trap clicks
		void on_create(ui::simple_button<topbar_bg>&, world_state&);
	};

	class player_flag {
	public:
		void button_function(ui::masked_flag<player_flag>&, world_state&);
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

	using topbar_t = ui::gui_window <
		CT_STRING("topbar_bg"), ui::simple_button<topbar_bg>,
		CT_STRING("topbar_paper"), ui::simple_button<topbar_bg>,
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
		CT_STRING("tech_current_research"), ui::display_text<current_research_text_box>,
		CT_STRING("tech_literacy_value"), ui::display_text<literacy_text_box>,
		topbar_base
		>;
}
