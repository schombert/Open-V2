#pragma once
#include "common\\common.h"
#include "nations_gui.h"
#include "gui\\gui.hpp"
#include "world_state\\world_state.h"
#include "military\\military_functions.h"

namespace nations {
	template<typename W>
	void diplomacy_window_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
	}

	template<typename window_type>
	void war_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];

			text_data::replacement repl[3] = {
				text_data::replacement{text_data::value_type::order, vector_backed_string<char16_t>(""), [](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::first, text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.first_adj), [](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::second, text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second_adj), [](ui::tagged_gui_object) {}}
			};
			ui::add_linear_text(ui::xy_pair{ 0,0 }, this_war.war_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 3);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void attacker_strength::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		war = win.war;
		if(is_valid_index(war)) {
			auto& this_war = ws.w.military_s.wars[war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);

			int32_t total = 0;
			for(auto a : r) {
				if(is_valid_index(a))
					total += military::total_active_divisions(ws, ws.w.nation_s.nations[a]);
			}

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, total);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void defender_strength::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		war = win.war;
		if(is_valid_index(war)) {
			auto& this_war = ws.w.military_s.wars[war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);

			int32_t total = 0;
			for(auto a : r) {
				if(is_valid_index(a))
					total += military::total_active_divisions(ws, ws.w.nation_s.nations[a]);
			}

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, total);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
}
