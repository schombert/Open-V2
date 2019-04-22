#pragma once
#include "common\\common.h"
#include "find.h"
#include "world_state.h"
#include "gui\\gui.hpp"
#include "nations\\nations_functions.hpp"

namespace find_dialog {
	class find_window_base : public ui::draggable_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class find_close_button {
	public:
		void button_function(ui::simple_button<find_close_button>&, world_state&);
	};
	class find_box {
	public:
		void on_edit(ui::edit_box<find_box>& self, world_state& ws);
	};

	using find_item = std::variant<std::monostate, provinces::province_tag, nations::country_tag, nations::state_tag>;

	class find_item_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class find_item_button {
	public:
		find_item value;

		template<typename window_type>
		void windowed_update(ui::simple_button<find_item_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<find_item_button>& self, world_state& ws);
	};

	class find_item_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<find_item_flag>& self, W& w, world_state& ws);
	};

	class find_item_label {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class find_lb_item_base : public ui::visible_region {
	public:
		find_item value;
		void set_value(find_item t) {
			value = t;
		}
		template<typename W>
		void on_create(W& w, world_state&);
	};

	using find_lb_item = ui::gui_window <
		CT_STRING("entry_bg"), ui::simple_button<find_item_button>,
		CT_STRING("country_flag"), ui::masked_flag<find_item_flag>,
		CT_STRING("entry_text"), ui::display_text<find_item_label>,
		find_lb_item_base
	>;

	class find_window_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<find_close_button>,
		CT_STRING("find_edit"), ui::edit_box<find_box>,
		CT_STRING("find_entries"), ui::discrete_listbox<find_item_lb, find_lb_item, find_item>,
		find_window_base
	> {};

	template<typename W>
	void find_window_base::on_create(W & w, world_state& ws) {
		ui::hide(*associated_object);
	}
	template<typename W>
	void find_item_flag::windowed_update(ui::masked_flag<find_item_flag>& self, W & w, world_state & ws) {
		if(std::holds_alternative<nations::country_tag>(w.value)) {
			ui::make_visible_immediate(*self.associated_object);
			self.set_displayed_flag(ws, std::get<nations::country_tag>(w.value));
		} else {
			ui::hide(*self.associated_object);
		}
	}

	inline text_data::text_tag find_item_to_text(world_state const& ws, find_item i) {
		if(std::holds_alternative<nations::country_tag>(i))
			return ws.w.nation_s.nations.get<nation::name>(std::get<nations::country_tag>(i));
		if(std::holds_alternative<nations::state_tag>(i))
			return ws.w.nation_s.states.get<state::name>(std::get<nations::state_tag>(i));
		if(std::holds_alternative<provinces::province_tag>(i))
			return ws.w.province_s.province_state_container.get<province_state::name>(std::get<provinces::province_tag>(i));

		return text_data::text_tag();
	}
	template<typename window_type>
	void find_item_label::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const t = find_item_to_text(ws, win.value); t)
			ui::add_linear_text(ui::xy_pair{ 0,0 }, t, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
	}
	template<typename window_type>
	void find_item_button::windowed_update(ui::simple_button<find_item_button>& self, window_type & win, world_state & ws) {
		value = win.value;
	}
	template<typename lb_type>
	void find_item_lb::populate_list(lb_type & lb, world_state & ws) {
		std::vector<find_item, concurrent_allocator<find_item>> list_items;
		list_items.reserve(ws.w.nation_s.states.size() + ws.w.nation_s.nations.size() + ws.s.province_m.first_sea_province);

		auto& search_box = ws.w.find_w.win->get<CT_STRING("find_edit")>();
		bool const search_filter = search_box.size != 0;

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
		ws.w.nation_s.states.for_each([&list_items, &ws, search_filter, &search_box](nations::state_tag n) {
			if(ws.w.nation_s.states.get<state::state_capital>(n)) {
				if(search_filter) {
					if(text_data::contains_case_insensitive(ws.s.gui_m.text_data_sequences, ws.w.nation_s.states.get<state::name>(n), search_box.contents, search_box.size))
						list_items.emplace_back(n);
				} else {
					list_items.emplace_back(n);
				}
			}
		});
		for(int32_t i = 0; i < ws.s.province_m.first_sea_province; ++i) {
			provinces::province_tag n{uint16_t(i)};
			if(search_filter) {
				if(text_data::contains_case_insensitive(ws.s.gui_m.text_data_sequences, ws.w.province_s.province_state_container.get<province_state::name>(n), search_box.contents, search_box.size))
					list_items.emplace_back(n);
			} else {
				list_items.emplace_back(n);
			}
		}

		vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
		std::sort(list_items.begin(), list_items.end(), [&ws, &lss](find_item a, find_item b) {
			if(!std::holds_alternative<std::monostate>(a) && !std::holds_alternative<std::monostate>(b)) {
				auto const a_name = find_item_to_text(ws, a);
				auto const b_name = find_item_to_text(ws, b);
				return lss(
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
			} else {
				return false;
			}
		});


		lb.new_list(list_items.begin(), list_items.end());
	}
	template<typename W>
	void find_lb_item_base::on_create(W & w, world_state &) {
		w.get<CT_STRING("entry_bg")>().associated_object->size.x = associated_object->size.x;
	}
}
