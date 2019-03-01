#pragma once
#include "common\\common.h"
#include "messages.h"
#include "world_state.h"
#include "gui\\gui.hpp"

namespace messages {

	class close_button {
	public:
		void button_function(ui::simple_button<close_button>&, world_state&);
	};

	class next_button {
	public:
		void button_function(ui::simple_button<next_button>&, world_state&);
	};

	class prev_button {
	public:
		void button_function(ui::simple_button<prev_button>&, world_state&);
	};

	class goto_button {
	public:
		void button_function(ui::button<goto_button>&, world_state&);
		void update(ui::button<goto_button>&, world_state&);
	};

	class message_window_base : public ui::draggable_region {
	public:
		ui::simple_button<close_button> m_close_button;
		ui::simple_button<next_button> m_next_button;
		ui::simple_button<prev_button> m_prev_button;

		template<typename W>
		void on_create(W& w, world_state&);

		void update(world_state& ws);
	};

	template <typename W>
	void message_window_base::on_create(W& w, world_state& ws) {
	
		auto lr_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_left_right_button"]);
		auto close_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_msg_close_button"]);

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, lr_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			m_prev_button));
		m_prev_button.set_frame(ws.w.gui_m, 0);
		m_prev_button.associated_object->position = ui::xy_pair{ int16_t(500 - 18 - 20 * 3),56i16};

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, lr_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			m_next_button));
		m_next_button.set_frame(ws.w.gui_m, 1);
		m_next_button.associated_object->position = ui::xy_pair{ int16_t(500 - 18 - 20 * 2),56i16 };

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, close_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			m_close_button));
		m_close_button.associated_object->position = ui::xy_pair{ int16_t(500 - 18 - 20) ,56i16 };

	}

	class messaage_flag {
	public:
		void update(ui::masked_flag<messaage_flag>& self, world_state& ws);
	};

	class message_header {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class message_body {
	public:
		void update(ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class message_window_t : public ui::gui_window <
		CT_STRING("AgreeButton"), ui::button<goto_button>,
		CT_STRING("LeftShield"), ui::masked_flag<messaage_flag>,
		CT_STRING("RightShield"), ui::masked_flag<messaage_flag>,
		CT_STRING("Title"), ui::display_text<message_header>,
		CT_STRING("Description"), ui::multiline_text<message_body>,
		message_window_base
	> {};
}
