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

	template<typename W>
	void crisis_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 638i16, 249i16 };
		associated_object->position += ui::xy_pair{ -16i16, -6i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 16i16, 6i16 };
		});
	}

	template<typename window_type>
	void war_display_base::on_create(window_type & w, world_state & ws) {
		w.associated_object->position.x = 0;
	}

	template<typename window_type>
	void war_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];

			text_data::replacement repl[7] = {
				text_data::replacement{text_data::value_type::order, vector_backed_string<char16_t>(u""), [](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::first,
					is_valid_index(this_war.first_adj) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.first_adj) : vector_backed_string<char16_t>(u"") ,
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::second,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::state,
					is_valid_index(this_war.state_name) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.state_name) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::second_country,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::country,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::country_adj,
					is_valid_index(this_war.second) ? text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, this_war.second) : vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}}
			};
			ui::add_linear_text(ui::xy_pair{ 0,0 }, this_war.war_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 7);
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

	template<typename window_type>
	void war_score_progress_bar::windowed_update(ui::progress_bar<war_score_progress_bar>& bar, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			bar.set_fraction((this_war.current_war_score + 1.0f) / 2.0f);
		}
	}

	template<typename window_type>
	void war_score_overlay::windowed_update(ui::dynamic_icon<war_score_overlay>& self, window_type & win, world_state & ws) {
		war = win.war;
	}

	template<typename window_type>
	void attacker_wg_marker::windowed_update(ui::dynamic_icon<attacker_wg_marker>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			float total_ws = std::min(military::total_attacker_demands_war_score(ws, this_war), 1.0f);

			int32_t x_off = 192 + (216 / 2) + int32_t((216 / 2) * total_ws) - self.associated_object->size.x / 2;
			self.associated_object->position.x = int16_t(x_off);
		}
	}

	template<typename window_type>
	void defender_wg_marker::windowed_update(ui::dynamic_icon<defender_wg_marker>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			float total_ws = std::min(military::total_defender_demands_war_score(ws, this_war), 1.0f);

			int32_t x_off = 192 + (216 / 2) - int32_t((216 / 2) * total_ws) - self.associated_object->size.x / 2;
			self.associated_object->position.x = int16_t(x_off);
		}
	}

	template<typename window_type>
	void war_score_text_box::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::percent, this_war.current_war_score);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void join_attacker_button::windowed_update(ui::simple_button<join_attacker_button>& self, window_type & win, world_state &) {
		war = win.war;
		self.set_enabled(false);
	}

	template<typename window_type>
	void join_defender_button::windowed_update(ui::simple_button<join_defender_button>& self, window_type & win, world_state &) {
		war = win.war;
		self.set_enabled(false);
	}

	template<typename W>
	void flag_sub_item::windowed_update(ui::masked_flag<flag_sub_item>& self, W & w, world_state & ws) {
		if(ws.w.nation_s.nations.is_valid_index(w.c))
			self.set_displayed_flag(ws, ws.w.nation_s.nations[w.c].tag);
	}

	template<typename window_type>
	void wg_sub_item::windowed_update(ui::dynamic_icon<wg_sub_item>& self, window_type & win, world_state & ws) {
		this_wg = win.this_wg;
		if(is_valid_index(this_wg.cb_type)) {
			auto icon = ws.s.military_m.cb_types[this_wg.cb_type].sprite_index;
			self.set_frame(ws.w.gui_m, icon != 0 ? uint32_t(icon - 1) : 0ui32);
		}
	}

	template<typename lb_type, typename window_type>
	void defenders_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.defenders);
			for(auto n : r) {
				lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type, typename window_type>
	void attackers_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.nation_s.nations_arrays, this_war.attackers);
			for(auto n : r) {
				lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type, typename window_type>
	void defenders_wg_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.military_s.war_goal_arrays, this_war.war_goals);
			for(auto i = r.first; i != r.second; ++i) {
				if(contains_item(ws.w.nation_s.nations_arrays, this_war.defenders, i->from_country))
					lb.add_item(ws, *i);
			}
		}
	}

	template<typename lb_type, typename window_type>
	void attackers_wg_lb::windowed_update(lb_type & lb, window_type & win, world_state & ws) {
		if(is_valid_index(win.war)) {
			military::war& this_war = ws.w.military_s.wars[win.war];
			auto r = get_range(ws.w.military_s.war_goal_arrays, this_war.war_goals);
			for(auto i = r.first; i != r.second; ++i) {
				if(contains_item(ws.w.nation_s.nations_arrays, this_war.attackers, i->from_country))
					lb.add_item(ws, *i);
			}
		}
	}

	template<typename lb_type>
	void war_lb::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<military::war_tag, 16, concurrent_allocator<military::war_tag>> data;
		if(auto tw = ws.w.selected_diplomacy.selected_war; is_valid_index(tw)) {
			data.push_back(tw);
		}
		ws.w.military_s.wars.for_each([tw = ws.w.selected_diplomacy.selected_war, &data, &ws](military::war const& w) {
			if(auto id = w.id; ws.w.military_s.wars.is_valid_index(id) && id != tw) {
				data.push_back(id);
			}
		});
		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}
}
