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
		void button_function(ui::gui_object_tag, ui::gui_manager&) {
			//MessageBoxA(nullptr, std::to_string(n).c_str(), "MB_A", MB_OK | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION);
		}
	};

	using province_window_header = ui::gui_window<
		ui::fixed_region>;

	using province_window_t = ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<close_province_window>,
		ui::fixed_region>;
}
