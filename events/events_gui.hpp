#pragma once
#include "common\\common.h"
#include "events_gui.h"
#include "gui\\gui.hpp"
#include "world_state\\world_state.h"

namespace events {
	class hidden_image {
	public:
		void update(ui::dynamic_icon<hidden_image>& self, world_state& ws);
	};

	class hidden_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {}
	};

	class province_event_title {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class province_event_location {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class province_event_body {
	public:
		void update(ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<int32_t index>
	class province_event_option {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<province_event_option<index>>& ico, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class province_event_window_base : public ui::draggable_region {
	public:
		ui::simple_button<province_event_option<1>> option_two;
		ui::simple_button<province_event_option<2>> option_three;
		ui::simple_button<province_event_option<3>> option_four;

		template<typename W>
		void on_create(W& w, world_state&);
	};

	class province_event_window_t : public ui::gui_window <
		CT_STRING("event_images"), ui::dynamic_icon<hidden_image>,
		CT_STRING("Title"), ui::display_text<province_event_title>,
		CT_STRING("Date"), ui::display_text<hidden_text>,
		CT_STRING("Province"), ui::display_text<province_event_location>,
		CT_STRING("Description"), ui::multiline_text<province_event_body>,
		CT_STRING("Event_Province_OptionButton"), ui::simple_button<province_event_option<0>>,
		province_event_window_base
	> {};

	class nation_event_window_t : public ui::gui_window <
		CT_STRING("event_images"), ui::dynamic_icon<nation_event_image>,
		CT_STRING("Title"), ui::display_text<nation_event_title>,
		CT_STRING("Date"), ui::display_text<hidden_text>,
		CT_STRING("Description"), ui::multiline_text<nation_event_body>,
		CT_STRING("Event_Country_OptionButton"), ui::simple_button<nation_event_option<0>>,
		nation_event_window_base
	> {};


	class major_event_window_t : public ui::gui_window <
		CT_STRING("country_flag1"), ui::masked_flag<major_event_flag>,
		CT_STRING("country_flag2"), ui::masked_flag<major_event_flag>,
		CT_STRING("Title"), ui::display_text<major_event_title>,
		CT_STRING("Date"), ui::display_text<hidden_text>,
		CT_STRING("Description"), ui::multiline_text<major_event_body>,
		CT_STRING("Event_Major_OptionButton"), ui::simple_button<major_event_option<0>>,
		major_event_window_base
	> {};

}
