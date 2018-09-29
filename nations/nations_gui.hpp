#pragma once
#include "common\\common.h"
#include "nations_gui.h"
#include "gui\\gui.hpp"
#include "world_state\\world_state.h"
#include "military\\military_functions.h"
#include "nations_functions.h"

namespace nations {
	template<typename W>
	void diplomacy_window_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});

		w.template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(ws.w.gui_m, 0);
		ui::hide(*(w.template get <CT_STRING("crisis_info_win")>().associated_object));
		ui::hide(*(w.template get<CT_STRING("war_listbox")>().associated_object));
	}

	template<typename W>
	void crisis_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 638i16, 249i16 };
		associated_object->position += ui::xy_pair{ -16i16, -6i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 16i16, 6i16 };
		});
	}

	template<typename W>
	void crisis_attacker_defender_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 215i16, 175i16 };
		associated_object->position += ui::xy_pair{ -20i16, -26i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 20i16, 26i16 };
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
		ws.w.military_s.wars.for_each([&data, &ws](military::war const& w) {
			if(auto id = w.id; ws.w.military_s.wars.is_valid_index(id))
				data.push_back(id);
		});
		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename window_type>
	void crisis_attacker_primary_flag::windowed_update(ui::masked_flag<crisis_attacker_primary_flag>& self, window_type & win, world_state & ws) {
		auto t = ws.w.current_crisis.on_behalf_of;
		if(bool(t) && t != ws.w.current_crisis.primary_attacker) {
			self.set_displayed_flag(ws, t->tag);
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}

	template<typename window_type>
	void crisis_defender_primary_flag::windowed_update(ui::masked_flag<crisis_defender_primary_flag>& self, window_type & win, world_state & ws) {
		auto t = ws.w.current_crisis.target;
		if(bool(t) && t != ws.w.current_crisis.primary_defender) {
			self.set_displayed_flag(ws, t->tag);
			ui::make_visible_immediate(*self.associated_object);
		} else {
			ui::hide(*self.associated_object);
		}
	}

	template<typename lb_type>
	void crisis_attacker_backer_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers);
		for(auto n : r) {
			if(is_valid_index(n)) {
				auto nloc = ws.w.nation_s.nations.get_location(n);
				if(nloc != ws.w.current_crisis.on_behalf_of && nloc != ws.w.current_crisis.primary_attacker)
					lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type>
	void crisis_defender_backer_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders);
		for(auto n : r) {
			if(is_valid_index(n)) {
				auto nloc = ws.w.nation_s.nations.get_location(n);
				if(nloc != ws.w.current_crisis.target && nloc != ws.w.current_crisis.primary_defender)
					lb.add_item(ws, n);
			}
		}
	}

	template<typename lb_type>
	void crisis_attacker_wg_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.military_s.war_goal_arrays, ws.w.current_crisis.goals);
		for(auto i = r.first; i != r.second; ++i) {
			if(contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers, i->from_country))
				lb.add_item(ws, *i);
		}
	}

	template<typename lb_type>
	void crisis_defender_wg_lb::populate_list(lb_type & lb, world_state & ws) {
		auto r = get_range(ws.w.military_s.war_goal_arrays, ws.w.current_crisis.goals);
		for(auto i = r.first; i != r.second; ++i) {
			if(contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders, i->from_country))
				lb.add_item(ws, *i);
		}
	}

	template<typename window_type>
	void make_offer_button::windowed_update(ui::simple_button<make_offer_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<typename window_type>
	void crisis_defender_back_down_button::windowed_update(ui::simple_button<crisis_defender_back_down_button>& self, window_type & win, world_state &) {
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void crisis_attacker_back_down_button::windowed_update(ui::simple_button<crisis_attacker_back_down_button>& self, window_type & win, world_state &) {
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	inline void crisis_support_attacker_button::windowed_update(ui::simple_button<crisis_support_attacker_button>&, window_type & win, world_state &) {}

	template<typename window_type>
	inline void crisis_support_defender_button::windowed_update(ui::simple_button<crisis_support_defender_button>&, window_type & win, world_state &) {}

	template<typename window_type>
	void undecided_gp_flag::windowed_update(ui::masked_flag<undecided_gp_flag>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag))
			self.set_displayed_flag(ws, ws.w.nation_s.nations[win.tag].tag);
	}

	template<typename W>
	void undecided_pane_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{ 210i16, 150i16 };
		associated_object->position += ui::xy_pair{ -6i16, 0i16 };

		auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["fence_sitter_entry"]);
		for(uint32_t i = 0; i < std::extent_v<decltype(undecided_nations)>; ++i) {
			auto wwin = ui::create_static_element(
				ws, common_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				undecided_nations[i]);
			undecided_nations[i].associated_object->position = ui::xy_pair{ int16_t(52 * (i % 4)), int16_t(i < 4 ? 0 : 75) };
		}
	}

	template<typename W>
	void undecided_pane_base::windowed_update(W & w, world_state & ws) {
		if(ws.w.current_crisis.type == current_state::crisis_type::none) {
			ui::hide(*associated_object);
		} else {
			auto interested_range = get_range(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested);
			for(uint32_t i = 0; i < std::extent_v<decltype(undecided_nations)>; ++i) {
				if(interested_range.first + i < interested_range.second) {
					undecided_nations[i].tag = *(interested_range.first + i);
					ui::make_visible_immediate(*undecided_nations[i].associated_object);
				} else {
					ui::hide(*undecided_nations[i].associated_object);
				}
			}
			ui::make_visible_immediate(*associated_object);
		}
	}

	template<typename W>
	void undecided_gp_entry_base::on_create(W & w, world_state &) {
		associated_object->size = ui::xy_pair{ 52i16, 75i16 };
	}

	template<typename W>
	void nation_details_item_base::on_create(W & w, world_state & ws) {
		w.associated_object->position.x = 0;
	}

	template<typename window_type>
	void nation_details_background_button::windowed_update(ui::simple_button<nation_details_background_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		if(tag == ws.w.selected_diplomacy.selected_nation)
			self.set_frame(ws.w.gui_m, 1ui32);
		else
			self.set_frame(ws.w.gui_m, 0ui32);
	}

	template<typename W>
	void nation_details_flag::windowed_update(ui::masked_flag<nation_details_flag>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			self.set_displayed_flag(ws, ws.w.nation_s.nations[w.tag].tag);
		}
	}

	template<typename window_type>
	void nation_details_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations[win.tag].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_priority_button::windowed_update(ui::simple_button<nation_details_priority_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<int32_t index>
	template<typename window_type>
	void gp_influence_text<index>::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + index < r.second) {
			if(auto id = *(r.first + index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::integer, nations::get_influence_value(ws, ws.w.nation_s.nations[id], win.tag));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}


	template<typename W>
	void nation_details_sphere_leader_flag::windowed_update(ui::masked_flag<nation_details_flag>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			auto leader = ws.w.nation_s.nations[w.tag].sphere_leader;
			if(leader) {
				self.set_displayed_flag(ws, leader->tag);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename window_type>
	void nation_details_military_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].military_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_economic_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].industrial_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_prestige_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].prestige_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_overall_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].overall_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_relations_value::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, nations::get_relationship(ws, *player, win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void nation_details_opinion_type::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, nations::influence_level_to_text(ws, nations::get_influence_level(ws, *player, win.tag), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
}
