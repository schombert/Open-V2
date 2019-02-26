#pragma once
#include "common\\common.h"
#include "messages.h"
#include "gui\\gui.hpp"
#include "world_state.h"

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
		void button_function(ui::simple_button<goto_button>&, world_state&);
		void update(ui::simple_button<goto_button>&, world_state&);
	};

	class message_window_base : public ui::draggable_region {
	public:
		ui::simple_button<close_button> m_close_button;
		ui::simple_button<next_button> m_next_button;
		ui::simple_button<prev_button> m_prev_button;

		template<typename W>
		void on_create(W& w, world_state&);
	};

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
		CT_STRING("AgreeButton"), ui::simple_button<goto_button>,
		CT_STRING("LeftShield"), ui::masked_flag<messaage_flag>,
		CT_STRING("RightShield"), ui::masked_flag<messaage_flag>,
		CT_STRING("Title"), ui::display_text<message_header>,
		CT_STRING("Description"), ui::multiline_text<message_body>,
		message_window_base
	> {};
}
