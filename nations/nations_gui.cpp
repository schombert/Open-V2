#include "common\\common.h"
#include "nations_gui.hpp"

namespace nations {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.hide_diplomacy_window();
	}
	void diplomacy_tab_button_group::on_select(world_state & ws, uint32_t i) {
		if(i == 0) {
			ws.w.show_diplomacy_window_self();
		} else if(i == 1) {
			ws.w.show_diplomacy_window(military::war_tag());
		} else if(i == 2) {
			ws.w.show_diplomacy_window_cbs();
		} else if(i == 3) {
			ws.w.show_diplomacy_window_crisis();
		}
	}
	void attacker_strength::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void defender_strength::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void war_score_overlay::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}

	void wg_sub_item::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(this_wg.from_country) && is_valid_index(this_wg.cb_type)) {
			ui::line_manager lm;
			auto cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations[this_wg.from_country].name, ui::tooltip_text_format,
				ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::demands_label], ui::tooltip_text_format,
				ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();

			text_data::replacement repl[3] = {
				text_data::replacement{text_data::value_type::recipient,
					is_valid_index(this_wg.target_country) ? 
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations[this_wg.target_country].name) :
						vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::state,
					is_valid_index(this_wg.target_state) ?
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.states[this_wg.target_state].name) :
						vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::third, 
					is_valid_index(this_wg.liberation_target) ?
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations[this_wg.liberation_target].name) :
						vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}}
			};

			cursor = ui::add_linear_text(cursor, ws.s.military_m.cb_types[this_wg.cb_type].explanation, ui::tooltip_text_format,
				ws.s.gui_m, ws.w.gui_m, tw, lm, repl, 3);
			lm.finish_current_line();
		}
	}

	void join_attacker_button::button_function(ui::simple_button<join_attacker_button>&, world_state &) {}

	void join_attacker_button::on_create(ui::simple_button<join_attacker_button>& self, world_state & ws) {
		ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *self.associated_object, ui::gui_object_tag(0) });
	}

	void join_attacker_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void join_defender_button::button_function(ui::simple_button<join_defender_button>&, world_state &) {}
	void join_defender_button::on_create(ui::simple_button<join_defender_button>& self, world_state & ws) {
		ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *self.associated_object, ui::gui_object_tag(0) });
	}
	void join_defender_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}

	void flag_sub_item::button_function(ui::masked_flag<flag_sub_item>& self, world_state & ws) {
		auto flag = self.get_displayed_flag();
		if(is_valid_index(flag)) {
			if(auto holder = ws.w.culture_s.national_tags_state[flag].holder; holder) {
				if(auto id = holder->id; ws.w.nation_s.nations.is_valid_index(id))
					ws.w.show_diplomacy_window(id);
			}
		}
	}
	ui::window_tag defenders_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	ui::window_tag attackers_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	ui::window_tag defenders_wg_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["wargoal"]);
	}
	ui::window_tag attackers_wg_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["wargoal"]);
	}
	ui::window_tag war_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_war_info"]);
	}
}
