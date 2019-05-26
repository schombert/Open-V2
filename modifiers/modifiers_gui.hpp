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
		bool choose_new_focus = true;

		template<typename W>
		void on_create(W& win, world_state& ws);

		void update(world_state& ws);
	};

	class nf_replacement_item_base : public ui::visible_region {
	public:
		nations::state_tag tag;

		void set_value(nations::state_tag v) {
			tag = v;
		}
	};

	class replacement_choice_button {
	public:
		nations::state_tag tag;

		template<typename W>
		void windowed_update(ui::simple_button<replacement_choice_button>& self, W& win, world_state& ws);
		void button_function(ui::simple_button<replacement_choice_button>& self, world_state& ws);

	};
	class replacment_nf_icon {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<replacment_nf_icon>&, W& win, world_state& ws);
	};

	class replacement_nf_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class replacement_nf_location {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using nf_replacement_item = ui::gui_window<
		CT_STRING("background"), ui::simple_button<replacement_choice_button>,
		CT_STRING("nf_icon"), ui::dynamic_icon<replacment_nf_icon>,
		CT_STRING("nf_name"), ui::display_text<replacement_nf_name>,
		CT_STRING("nf_location"), ui::display_text<replacement_nf_location>,
		nf_replacement_item_base
	>;

	class nf_replacement_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class national_focus_window_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<close_nf_window>,
		CT_STRING("header"), ui::display_text<nf_window_header>,
		CT_STRING("nf_listbox"), ui::discrete_listbox<nf_replacement_lb, nf_replacement_item, nations::state_tag>,
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
	template<typename W>
	void replacement_choice_button::windowed_update(ui::simple_button<replacement_choice_button>& self, W & win, world_state & ws) {
		tag = win.tag;
	}
	template<typename W>
	void replacment_nf_icon::windowed_update(ui::dynamic_icon<replacment_nf_icon>& self, W & win, world_state & ws) {
		self.set_frame(ws.w.gui_m, nf_tag_to_frame(ws, ws.w.nation_s.states.get<state::owner_national_focus>(win.tag)));
	}
	template<typename window_type>
	void replacement_nf_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const nf = ws.w.nation_s.states.get<state::owner_national_focus>(win.tag); nf) 
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.modifiers_m.national_focuses[nf].name, fmt, ws, box, lm);
		else 
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::no_focus, fmt, ws, box, lm);
	}
	template<typename window_type>
	void replacement_nf_location::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.states.get<state::name>(win.tag), fmt, ws, box, lm);
	}
	template<typename lb_type>
	void nf_replacement_lb::populate_list(lb_type & lb, world_state & ws) {
		auto rng = get_range(ws.w.nation_s.state_tag_arrays, ws.w.nation_s.nations.get<nation::national_focus_locations>(ws.w.local_player_nation));
		lb.new_list(rng.first, rng.second);
	}
}
