#include "common\\common.h"
#include "military_gui.hpp"

namespace military {
	ui::window_tag fabricate_cb_types_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["cb_type_item"]);
	}

	void cb_type_button::button_function(ui::button<cb_type_button>& self, world_state& ws) {
		ws.w.fabricate_cb_w.selected_type = type;
		ws.w.fabricate_cb_w.update_fabricate_window(ws.w.gui_m);
	}

	void hidden_button::update(ui::simple_button<hidden_button>& self, world_state & ws) {
		ui::hide(*self.associated_object);
	}

	void self_flag::update(ui::masked_flag<self_flag>& self, world_state& ws) {
		self.set_displayed_flag(ws, ws.w.local_player_nation);
	}

	fabricate_cb_window::fabricate_cb_window() : win(std::make_unique<fabricate_cb_window_t>()) {}
	fabricate_cb_window::~fabricate_cb_window() {}

	void fabricate_cb_window::hide_fabricate_window(ui::gui_manager& gui_m) {
		target = nations::country_tag();
		selected_type = military::cb_type_tag();
		win->get<CT_STRING("cb_list")>().new_list(nullptr, nullptr);

		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}

	void fabricate_cb_window::update_fabricate_window(ui::gui_manager& gui_m) {
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void fabricate_cb_window::show_fabricate_window(ui::gui_manager& gui_m, nations::country_tag t) {
		target = t;
		selected_type = military::cb_type_tag();

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void fabricate_cb_window::init_fabricate_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["MakeCBDialog"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void target_flag::update(ui::masked_flag<target_flag>& self, world_state & ws) {
		self.set_displayed_flag(ws, ws.w.fabricate_cb_w.target);
	}
	void ok_button::update(ui::simple_button<ok_button>& self, world_state & ws) {
		self.set_enabled(is_valid_index(ws.w.fabricate_cb_w.selected_type));
	}
	void ok_button::button_function(ui::simple_button<ok_button>& self, world_state & ws) {
		ws.w.fabricate_cb_w.hide_fabricate_window(ws.w.gui_m);

		if(ws.w.fabricate_cb_w.selected_type)
			ws.w.pending_commands.add<commands::fabricate_cb>(ws.w.local_player_nation, ws.w.fabricate_cb_w.target, ws.w.fabricate_cb_w.selected_type);
	}
	void cancel_button::button_function(ui::simple_button<cancel_button>& self, world_state & ws) {
		ws.w.fabricate_cb_w.hide_fabricate_window(ws.w.gui_m);
	}

	void dialog_title::update(
		ui::tagged_gui_object box,
		ui::text_box_line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {
		
		ui::add_linear_text(
			ui::xy_pair{0, 0},
			ws.s.fixed_ui_text[scenario::fixed_ui::fabricate_title],
			fmt,
			ws.s.gui_m,
			ws.w.gui_m,
			box,
			lm);
		lm.finish_current_line();
	}

	void description::update(ui::tagged_gui_object box, ui::line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const type = ws.w.fabricate_cb_w.selected_type; type) {
			auto const daily_amount = daily_cb_progress(ws, ws.w.local_player_nation, type);
			auto const total_days = daily_amount > 0 ? std::ceil(1.0f / daily_amount) : 9999.0f;

			char16_t days_buffer[16];
			put_value_in_buffer(days_buffer, display_type::exact_integer, total_days);

			char16_t infamy_buffer[16];
			put_value_in_buffer(infamy_buffer, display_type::fp_one_place, base_cb_infamy(ws, type));

			text_data::replacement repl[3] = {
				text_data::replacement{text_data::value_type::type,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::days,
					vector_backed_string<char16_t>(days_buffer),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::badboy,
					vector_backed_string<char16_t>(infamy_buffer),
					[](ui::tagged_gui_object) {}}
			};

			ui::add_multiline_text(
				ui::xy_pair{ 0, 0 },
				ws.s.fixed_ui_text[scenario::fixed_ui::fabricate_description],
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm,
				repl,
				3);
			lm.finish_current_line();
		}
	}
}
