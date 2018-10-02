#pragma once
#include "common\\common.h"
#include "world_state\\world_state.h"
#include "gui\\gui.hpp"
#include "economy_functions.h"
#include "economy_gui.h"

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
		production_window_base
	>{};

	template<typename W>
	void production_window_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
		w.template get<CT_STRING("production_tab_button_group")>().set_selected(ws.w.gui_m, 0);
	}
}
