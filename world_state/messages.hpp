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
		m_prev_button.associated_object->position = ui::xy_pair{ int16_t(500 - 24 - 20 * 3),22i16};

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, lr_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			m_next_button));
		m_next_button.set_frame(ws.w.gui_m, 1);
		m_next_button.associated_object->position = ui::xy_pair{ int16_t(500 - 24 - 20 * 2),22i16 };

		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, close_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			m_close_button));
		m_close_button.associated_object->position = ui::xy_pair{ int16_t(500 - 24 - 20) ,22i16 };

		w.get<CT_STRING("LeftShield")>().associated_object->position += ui::xy_pair{8, -6 };
		w.get<CT_STRING("RightShield")>().associated_object->position += ui::xy_pair{ 8, -6 };

		ui::hide(*associated_object);
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

	class hidden_button {
	public:
		void update(ui::simple_button<hidden_button>& self, world_state& ws);
	};

	class message_window_t : public ui::gui_window <
		CT_STRING("Background"), ui::simple_button<hidden_button>,
		CT_STRING("AgreeButton"), ui::button<goto_button>,
		CT_STRING("LeftShield"), ui::masked_flag<messaage_flag>,
		CT_STRING("RightShield"), ui::masked_flag<messaage_flag>,
		CT_STRING("Title"), ui::display_text<message_header>,
		CT_STRING("Description"), ui::multiline_text<message_body, 0, -70>,
		message_window_base
	> {};

	class message_settings_window_base : public ui::draggable_region {
	public:
	};

	class message_settings_close_button {
	public:
		void button_function(ui::simple_button<message_settings_close_button>&, world_state&);
	};

	class mesasge_settings_button_group {
	public:
		void on_select(world_state& ws, uint32_t i);
	};

	template<uint32_t i>
	class stars_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<stars_icon<i>>& self, window_type& win, world_state& ws);
	};

	class importance_label {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	enum class group_setting {
		self,
		neighbors,
		sphere_members,
		vassals,
		allies,
		great_powers,
		sphere_leader,
		overlord
	};
	struct message_id {
		int32_t value = 0;
	};

	using message_setting = std::variant<std::monostate, group_setting, nations::country_tag, message_id>;

	class message_settings_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class message_setting_item_base : public ui::visible_region {
	public:
		message_setting value;
		void set_value(message_setting t) {
			value = t;
		}
	};

	class message_setting_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<message_setting_flag>& self, W& w, world_state& ws);
	};

	class message_setting_label {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<int32_t i>
	class message_type_button {
	public:
		message_setting value;

		template<typename window_type>
		void windowed_update(ui::simple_button<message_type_button<i>>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<message_type_button<i>>& self, world_state& ws);
	};

	class importance_button {
	public:
		message_setting value;

		template<typename window_type>
		void windowed_update(ui::simple_button<importance_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<importance_button>& self, world_state& ws);
	};

	using message_setting_item = ui::gui_window <
		CT_STRING("country_flag"), ui::masked_flag<message_setting_flag>,
		CT_STRING("entry_text"), ui::display_text<message_setting_label>,
		CT_STRING("stars_zero_button"), ui::simple_button<message_type_button<0>>,
		CT_STRING("stars_one_button"), ui::simple_button<message_type_button<1>>,
		CT_STRING("stars_two_button"), ui::simple_button<message_type_button<2>>,
		CT_STRING("stars_three_button"), ui::simple_button<message_type_button<3>>,
		CT_STRING("importance_button"), ui::simple_button<importance_button>,
		message_setting_item_base
	> ;

	class message_settings_window_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<message_settings_close_button>,
		CT_STRING("mesasge_settings_button_group"), ui::button_group<
		CT_STRING("category_messages_button"),
		CT_STRING("category_sources_button"), mesasge_settings_button_group>,
		CT_STRING("stars_zero"), ui::dynamic_icon<stars_icon<0>>,
		CT_STRING("stars_one"), ui::dynamic_icon<stars_icon<1>>,
		CT_STRING("stars_two"), ui::dynamic_icon<stars_icon<2>>,
		CT_STRING("stars_three"), ui::dynamic_icon<stars_icon<3>>,
		CT_STRING("label_importance"), ui::display_text<importance_label>,
		CT_STRING("message_settings_items"), ui::discrete_listbox<message_settings_lb, message_setting_item, message_setting>,
		message_settings_window_base
	> {};
}
