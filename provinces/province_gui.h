#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "gui\\gui.hpp"
#include "simple_mpl\\simple_mpl.hpp"

namespace provinces {
	/*
	using budget_window_t = ui::gui_window <
	CT_STRING("tab_takenloans"), ui::button_group_member,
	CT_STRING("tab_givenloans"), ui::button_group_member,
	CT_STRING("loans_tab_group"), ui::button_group<CT_STRING("tab_takenloans"), CT_STRING("tab_givenloans"), simple_button_group>,
	CT_STRING("debt_sort_country"), ui::simple_button<mb_button<3>>,
	CT_STRING("debt_sort_amount"), ui::simple_button<mb_button<4>>,
	CT_STRING("tax_0_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tax_1_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tax_2_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("land_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("naval_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("projects_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_0_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_1_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_2_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_3_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tariff_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("close_button"), ui::simple_button<mb_button<5>>,
	CT_STRING("take_loan"), ui::simple_button<mb_button<6>>,
	CT_STRING("repay_loan"), ui::simple_button<mb_button<7>>,
	CT_STRING("chart_0"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_1"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_2"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_debt"), ui::piechart<empty_gui_obj>,
	CT_STRING("debt_listbox"), ui::display_listbox<debt_lb, debt_listitem_t, 56>,
	CT_STRING("gunboat_alert"), ui::dynamic_icon<hidden_icon>,
	CT_STRING("tax_0_pops"), ui::overlap_box<pop_type_a, ui::window_tag, pop_item_t, 32>,
	ui::draggable_region >
	*/

	class close_province_window {
	public:
		void button_function(ui::gui_object_tag t, world_state& ws);
	};
	class terrain_icon {
		void update(ui::dynamic_icon<terrain_icon>& ico, world_state& ws);
	};

	class province_name {
	public:
		template<typename ...PARAMS>
		province_name(PARAMS&&...) {}

		template<typename window_type>
		void windowed_update(window_type& w, ui::tagged_gui_object obj, text_data::alignment align, ui::text_format& fmt, world_state& ws) {
			auto selected = ws.w.province_window.selected_province;
			if(is_valid_index(selected))
				ui::detail::create_linear_text(ws.s.gui_m, ws.w.gui_m, obj, ws.w.province_s.province_state_container[selected].name, align, fmt, nullptr, 0ui32);
		}
	};

	class province_state_name {
	public:
		template<typename ...PARAMS>
		province_state_name(PARAMS&&...) {}

		template<typename window_type>
		void windowed_update(window_type& w, ui::tagged_gui_object obj, text_data::alignment align, ui::text_format& fmt, world_state& ws) {
			auto selected = ws.w.province_window.selected_province;
			if(is_valid_index(selected) && is_valid_index(ws.s.province_m.province_container[selected].state_id))
				ui::detail::create_linear_text(ws.s.gui_m, ws.w.gui_m, obj, ws.s.province_m.state_names[ws.s.province_m.province_container[selected].state_id], align, fmt, nullptr, 0ui32);
		}
	};

	class province_window_header_base : public ui::gui_behavior {
	public:
		province_tag selected_province;
		date_tag last_update;

		template<typename ...P>
		explicit province_window_header_base(P&& ... params) {}
		void on_create(world_state&);
	};

	using province_window_header = ui::gui_window<
		CT_STRING("prov_terrain"), ui::dynamic_icon<terrain_icon>,
		CT_STRING("state_name"), ui::display_text<province_state_name>,
		CT_STRING("province_name"), ui::display_text<province_name>,
		province_window_header_base>;

	class province_window_base : public ui::fixed_region {
	public:
		province_tag selected_province;
		date_tag last_update;

		template<typename ...P>
		explicit province_window_base(P&& ... params) : ui::fixed_region(std::forward<P>(params)...){}
		void on_create(world_state&);
	};

	using province_window_t = ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<close_province_window>,
		CT_STRING("province_view_header"), province_window_header,
		province_window_base>;
}
