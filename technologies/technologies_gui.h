#pragma once
#include "common\\common.h"
#include "technologies.h"
#include "gui\\gui.h"

namespace technologies {
	class tech_window_base : public ui::draggable_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class close_button {
	public:
		void button_function(ui::simple_button<close_button>&, world_state&);
	};

	class tech_school_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class school_item_base : public ui::gui_behavior {
	public:
		float modifier = 0.0f;
		uint32_t index = 0;
		text_data::text_tag modifier_name;

		school_item_base(school_item_base&&) = default;
		school_item_base(std::tuple<float, uint32_t, text_data::text_tag> const& t) :
			modifier(std::get<0>(t)), index(std::get<1>(t)), modifier_name(std::get<2>(t)) {}

		template<typename W>
		void on_create(W& w, world_state&);
		virtual ui::tooltip_behavior has_tooltip(ui::gui_object_tag, world_state&, const ui::mouse_move&) final override { return ui::tooltip_behavior::tooltip; }
		virtual void create_tooltip(ui::gui_object_tag, world_state&, const ui::mouse_move&, ui::tagged_gui_object /*tooltip_window*/) final override;
	};

	class school_item_icon {
	public:
		school_item_icon(school_item_icon&&) = default;
		school_item_icon(std::tuple<float, uint32_t, text_data::text_tag> const&) {}
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<school_item_icon>&, window_type&, world_state&);
	};

	class plusminus_icon {
	public:
		plusminus_icon(plusminus_icon&&) = default;
		plusminus_icon(std::tuple<float, uint32_t, text_data::text_tag> const&) {}
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<plusminus_icon>&, window_type&, world_state&);
	};

	class school_modifiers_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};


	using school_modifier_item = ui::gui_window <
		CT_STRING("main_icon"), ui::dynamic_icon<school_item_icon>,
		CT_STRING("plusminus_icon"), ui::dynamic_icon<plusminus_icon>,
		school_item_base>;

	class research_progress_bar {
	public:
		void update(ui::progress_bar<research_progress_bar>& bar, world_state& ws);
	};

	class research_name_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class research_category_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using tech_window_t = ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<close_button>,
		CT_STRING("administration_type"), ui::display_text<tech_school_text_box>,
		CT_STRING("school_bonus_icons"), ui::overlap_box<school_modifiers_lb, ui::window_tag, school_modifier_item, 17>,
		CT_STRING("research_progress"), ui::progress_bar<research_progress_bar>,
		CT_STRING("research_progress_name"), ui::display_text<research_name_text_box>,
		CT_STRING("research_progress_category"), ui::display_text<research_category_text_box>,
		tech_window_base>;
}
