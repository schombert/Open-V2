#pragma once
#include "common\\common.h"
#include "messages.h"
#include "world_state.h"
#include "gui\\gui.hpp"
#include "nations\nations_functions.h"

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
		CT_STRING("Description"), ui::multiline_text<message_body, 0, -40>,
		message_window_base
	> {};

	class message_settings_window_base : public ui::draggable_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class message_settings_close_button {
	public:
		void button_function(ui::simple_button<message_settings_close_button>&, world_state&);
	};

	class message_settings_button_group {
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

	class search_box {
	public:
		void on_edit(ui::edit_box<search_box>& self, world_state& ws);
	};

	
	struct message_id {
		int32_t value = 0;
	};

	using setting_item = std::variant<std::monostate, group_setting, nations::country_tag, message_id>;

	class message_settings_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class message_setting_item_base : public ui::visible_region {
	public:
		setting_item value;
		void set_value(setting_item t) {
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
		setting_item value;

		template<typename window_type>
		void windowed_update(ui::button<message_type_button<i>>& self, window_type& win, world_state& ws);
		void button_function(ui::button<message_type_button<i>>& self, world_state& ws);
		void button_function(ui::button<message_type_button<i>>& self, ui::rbutton_down m, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class importance_button {
	public:
		setting_item value;

		template<typename window_type>
		void windowed_update(ui::button<importance_button>& self, window_type& win, world_state& ws);
		void button_function(ui::button<importance_button>& self, world_state& ws);
		void button_function(ui::button<importance_button>& self, ui::rbutton_down m, world_state& ws);
	};

	using message_setting_item = ui::gui_window <
		CT_STRING("country_flag"), ui::masked_flag<message_setting_flag>,
		CT_STRING("entry_text"), ui::display_text<message_setting_label>,
		CT_STRING("stars_zero_button"), ui::button<message_type_button<0>>,
		CT_STRING("stars_one_button"), ui::button<message_type_button<1>>,
		CT_STRING("stars_two_button"), ui::button<message_type_button<2>>,
		CT_STRING("stars_three_button"), ui::button<message_type_button<3>>,
		CT_STRING("importance_button"), ui::button<importance_button>,
		message_setting_item_base
	> ;

	class message_settings_window_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<message_settings_close_button>,
		CT_STRING("category_messages_button"), ui::button_group_member,
		CT_STRING("category_sources_button"), ui::button_group_member,
		CT_STRING("mesasge_settings_button_group"), ui::button_group<
		CT_STRING("category_messages_button"),
		CT_STRING("category_sources_button"), message_settings_button_group>,
		CT_STRING("search_edit"), ui::edit_box<search_box>,
		CT_STRING("stars_zero"), ui::dynamic_icon<stars_icon<0>>,
		CT_STRING("stars_one"), ui::dynamic_icon<stars_icon<1>>,
		CT_STRING("stars_two"), ui::dynamic_icon<stars_icon<2>>,
		CT_STRING("stars_three"), ui::dynamic_icon<stars_icon<3>>,
		CT_STRING("label_importance"), ui::display_text<importance_label>,
		CT_STRING("message_settings_items"), ui::discrete_listbox<message_settings_lb, message_setting_item, setting_item>,
		message_settings_window_base
	> {};
	
	template<uint32_t i>
	template<typename window_type>
	void stars_icon<i>::windowed_update(ui::dynamic_icon<stars_icon<i>>& self, window_type & win, world_state & ws) {
		if(ws.w.message_settings_w.showing_messages) {
			ui::make_visible_immediate(*self.associated_object);
			self.set_frame(ws.w.gui_m, i);
		} else {
			ui::hide(*self.associated_object);
		}
	}
	template<typename window_type>
	void importance_label::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(!ws.w.message_settings_w.showing_messages) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::importance_label], fmt, ws, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void message_setting_flag::windowed_update(ui::masked_flag<message_setting_flag>& self, W & w, world_state & ws) {
		if(std::holds_alternative<nations::country_tag>(w.value)) {
			self.set_displayed_flag(ws, std::get<nations::country_tag>(w.value));
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}
	template<int32_t i>
	template<typename window_type>
	void message_type_button<i>::windowed_update(ui::button<message_type_button<i>>& self, window_type & win, world_state & ws) {
		value = win.value;
		if(std::holds_alternative<message_id>(win.value)) {
			self.set_frame(ws.w.gui_m, uint32_t(ws.s.message_m.settings[std::get<message_id>(win.value).value * 4 + i]));
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
		
	}
	template<int32_t i>
	void message_type_button<i>::button_function(ui::button<message_type_button<i>>& self, world_state & ws) {
		if(std::holds_alternative<message_id>(value)) {
			auto const old_setting = uint32_t(ws.s.message_m.settings[std::get<message_id>(value).value * 4 + i]);
			auto const new_setting = (old_setting + 1) % 4;
			ws.s.message_m.settings[std::get<message_id>(value).value * 4 + i]  = message_setting(new_setting);
			
			self.set_frame(ws.w.gui_m, new_setting);
			ws.w.message_settings_w.setting_changed = true;
		}
	}
	template<int32_t i>
	void message_type_button<i>::button_function(ui::button<message_type_button<i>>& self, ui::rbutton_down m, world_state & ws) {
		if(std::holds_alternative<message_id>(value)) {
			auto const old_setting = uint32_t(ws.s.message_m.settings[std::get<message_id>(value).value * 4 + i]);
			auto const new_setting = (old_setting + 3) % 4;
			ws.s.message_m.settings[std::get<message_id>(value).value * 4 + i] = message_setting(new_setting);

			self.set_frame(ws.w.gui_m, new_setting);
			ws.w.message_settings_w.setting_changed = true;
		}
	}
	template<int32_t i>
	void message_type_button<i>::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(std::holds_alternative<message_id>(value)) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::message_type_discard + uint32_t(ws.s.message_m.settings[std::get<message_id>(value).value * 4 + i])], ui::tooltip_text_format, ws, tw);
		}
	}
	template<typename window_type>
	void importance_button::windowed_update(ui::button<importance_button>& self, window_type & w, world_state & ws) {
		value = w.value;
		if(std::holds_alternative<group_setting>(value)) {
			self.set_frame(ws.w.gui_m, ws.s.message_m.group_importance[int32_t(std::get<group_setting>(value))]);
			ui::make_visible_immediate(*self.associated_object);
		} else if(std::holds_alternative<nations::country_tag>(value)) {
			self.set_frame(ws.w.gui_m, ws.w.nation_s.nations.get<nation::player_importance>(std::get<nations::country_tag>(value)));
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}
	template<typename window_type>
	void message_setting_label::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto const value = win.value;
		if(std::holds_alternative<group_setting>(value)) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::message_group_self + uint32_t(std::get<group_setting>(value))], fmt, ws, box, lm);
		} else if(std::holds_alternative<nations::country_tag>(value)) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(std::get<nations::country_tag>(value)), fmt, ws, box, lm);
		} else if(std::holds_alternative<message_id>(value)) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.message_m.description_text[std::get<message_id>(value).value], fmt, ws, box, lm);
		}
	}

	template<typename lb_type>
	void message_settings_lb::populate_list(lb_type & lb, world_state & ws) {
		std::vector<setting_item, concurrent_allocator<setting_item>> list_items;
		list_items.reserve(std::max(message_count, group_setting_count + ws.w.nation_s.nations.size()));

		auto& search_box = ws.w.message_settings_w.win->get<CT_STRING("search_edit")>();
		bool const search_filter = search_box.size != 0;

		if(ws.w.message_settings_w.showing_messages) {
			for(int32_t i = 0; i < message_count; ++i) {
				if(search_filter) {
					if(text_data::contains_case_insensitive(ws.s.gui_m.text_data_sequences, ws.s.message_m.description_text[i], search_box.contents, search_box.size))
						list_items.emplace_back(message_id{ i });
				} else {
					list_items.emplace_back(message_id{ i });
				}
			}
		} else {
			if(!search_filter) {
				for(int32_t i = 0; i < group_setting_count; ++i) {
					list_items.emplace_back(group_setting(i));
				}
			}
			ws.w.nation_s.nations.for_each([&list_items, &ws, search_filter, &search_box](nations::country_tag n) {
				if(nations::nation_exists(ws, n)) {
					if(search_filter) {
						if(text_data::contains_case_insensitive(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(n), search_box.contents, search_box.size))
							list_items.emplace_back(n);
					} else {
						list_items.emplace_back(n);
					}
				}
			});

			vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
			std::sort(list_items.begin() + (search_filter ? 0 : group_setting_count), list_items.end(), [&ws, &lss](setting_item a, setting_item b) {
				if(std::holds_alternative<nations::country_tag>(a) && std::holds_alternative<nations::country_tag>(b)) {
					auto a_name = ws.w.nation_s.nations.get<nation::name>(std::get<nations::country_tag>(a));
					auto b_name = ws.w.nation_s.nations.get<nation::name>(std::get<nations::country_tag>(b));
					return lss(
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
				} else if(!std::holds_alternative<nations::country_tag>(a) && std::holds_alternative<nations::country_tag>(b)) {
					return true;
				} else {
					return false;
				}
			});
		}

		lb.new_list(list_items.begin(), list_items.end());
	}
	template<typename W>
	void message_settings_window_base::on_create(W & w, world_state &) {
		ui::hide(*associated_object);
	}
}
