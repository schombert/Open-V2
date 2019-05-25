#pragma once
#include "common/common.h"
#include "modifiers_gui.h"
#include "gui\\gui.hpp"

namespace modifiers {

	class close_nf_window {
	public:
		void button_function(ui::simple_button<close_nf_window>& self, world_state& ws);
	};

	class nf_window_header {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class focus_choice_button {
	public:
		modifiers::national_focus_tag tag;

		void update(ui::simple_button<focus_choice_button>& self, world_state& ws);
		void button_function(ui::simple_button<focus_choice_button>& self, world_state& ws);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class national_focus_window_base : public ui::window_pane {
	public:
		std::vector<ui::simple_button<focus_choice_button>> nf_buttons;

		template<typename W>
		void on_create(W& win, world_state& ws);
	};

	class national_focus_window_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<close_nf_window>,
		CT_STRING("header"), ui::display_text<nf_window_header>,
		national_focus_window_base
	> {};
	
	template<typename W>
	void national_focus_window_base::on_create(W & win, world_state & ws) {
		int32_t x_off = 0;
		int32_t y_off = 0;

		auto ui_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["focus_icon"]);

		nf_buttons.emplace_back();
		auto const res = ui::create_static_element(
			ws, ui_tag,
			ui::tagged_gui_object{ *associated_object, win.window_object },
			nf_buttons.back());
		ui::move_to_front(ws.w.gui_m, res);
		res.object.position.x = int16_t((x_off++) * 39 + 38);
		res.object.position.y = int16_t((y_off) * 36 + 62);
		if(x_off == 7) {
			x_off = 0;
			++y_off;
		}

		ws.s.modifiers_m.for_each_national_focus([&ws, _this = this, &x_off, &y_off, ui_tag, &win](national_focus_tag nf) {
			if(ws.s.modifiers_m.national_focuses[nf].modifier) {
				_this->nf_buttons.emplace_back();
				auto const res = ui::create_static_element(
					ws, ui_tag,
					ui::tagged_gui_object{ *(_this->associated_object), win.window_object },
					_this->nf_buttons.back());
				_this->nf_buttons.back().tag = nf;
				ui::move_to_front(ws.w.gui_m, res);
				res.object.position.x = int16_t((x_off++) * 39 + 38);
				res.object.position.y = int16_t((y_off) * 36 + 62);
				if(x_off == 7) {
					x_off = 0;
					++y_off;
				}
			}
		});

		x_off = 0;
		++y_off;

		ws.s.modifiers_m.for_each_national_focus([&ws, _this = this, &x_off, &y_off, ui_tag, &win](national_focus_tag nf) {
			if(ws.s.modifiers_m.national_focuses[nf].pop_type) {
				_this->nf_buttons.emplace_back();
				auto const res = ui::create_static_element(
					ws, ui_tag,
					ui::tagged_gui_object{ *(_this->associated_object), win.window_object },
					_this->nf_buttons.back());
				_this->nf_buttons.back().tag = nf;
				ui::move_to_front(ws.w.gui_m, res);
				res.object.position.x = int16_t((x_off++) * 39 + 38);
				res.object.position.y = int16_t((y_off) * 36 + 62);
				if(x_off == 7) {
					x_off = 0;
					++y_off;
				}
			}
		});

		ui::hide(*associated_object);
	}
}
