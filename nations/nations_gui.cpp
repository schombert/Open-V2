#include "common\\common.h"
#include "nations_gui.hpp"

namespace nations {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.diplomacy_w.hide_diplomacy_window(ws.w.gui_m);
	}
	void diplomacy_tab_button_group::on_select(world_state & ws, uint32_t i) {
		if(i == 0) {
			ws.w.diplomacy_w.show_diplomacy_window_gp(ws.w.gui_m);
		} else if(i == 1) {
			ws.w.diplomacy_w.show_diplomacy_window_wars(ws.w.gui_m);
		} else if(i == 2) {
			ws.w.diplomacy_w.show_diplomacy_window_cbs(ws.w.gui_m);
		} else if(i == 3) {
			ws.w.diplomacy_w.show_diplomacy_window_crisis(ws.w.gui_m);
		}
	}
	void attacker_strength::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void defender_strength::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void war_score_overlay::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}

	void wg_sub_item::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(this_wg.from_country) && is_valid_index(this_wg.cb_type)) {
			ui::line_manager lm;
			auto cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(this_wg.from_country), ui::tooltip_text_format,
				ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::demands_label], ui::tooltip_text_format,
				ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();

			text_data::replacement repl[3] = {
				text_data::replacement{text_data::value_type::recipient,
					is_valid_index(this_wg.target_country) ? 
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(this_wg.target_country)) :
						vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::state,
					is_valid_index(this_wg.target_state) ?
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.states.get<state::name>(this_wg.target_state)) :
						vector_backed_string<char16_t>(u""),
					[](ui::tagged_gui_object) {}},
				text_data::replacement{text_data::value_type::third, 
					is_valid_index(this_wg.liberation_target) ?
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(this_wg.liberation_target)) :
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
	void crisis_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto target = ws.w.current_crisis.target; ws.w.current_crisis.type != current_state::crisis_type::none && bool(target)) {
			text_data::replacement repl(text_data::value_type::country_adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::adjective>(target)),
				[](ui::tagged_gui_object) {});
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_title], fmt, ws.s.gui_m, ws.w.gui_m, box, lm, &repl, 1);
			lm.finish_current_line();
		} else {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_crisis], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	void crisis_subtitle::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto target_name = [&ws, target = ws.w.current_crisis.target]() { return bool(target) ? ws.w.nation_s.nations.get<nation::name>(target) : text_data::text_tag(); }();
		text_data::replacement repl(text_data::value_type::country,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, target_name),
			[](ui::tagged_gui_object) {});
		switch(ws.w.current_crisis.type) {
			default:
			case current_state::crisis_type::none:
				break;
			case current_state::crisis_type::colonial:
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_colonize], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
				break;
			case current_state::crisis_type::influence:
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_influence], fmt, ws.s.gui_m, ws.w.gui_m, box, lm, &repl, 1);
				lm.finish_current_line();
				break;
			case current_state::crisis_type::liberation:
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_liberation], fmt, ws.s.gui_m, ws.w.gui_m, box, lm, &repl, 1);
				lm.finish_current_line();
				break;
			case current_state::crisis_type::claim:
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_reclaim], fmt, ws.s.gui_m, ws.w.gui_m, box, lm, &repl, 1);
				lm.finish_current_line();
				break;
		}
	}
	void crisis_status::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(ws.w.current_crisis.type != current_state::crisis_type::none) {
			if(ws.w.current_crisis.primary_attacker == nations::country_tag() || ws.w.current_crisis.primary_defender == nations::country_tag())
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_waiting], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			else {
				//ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_exists], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			}
			lm.finish_current_line();
		}
	}

	void crisis_temperature_bar::update(ui::progress_bar<crisis_temperature_bar>& bar, world_state& ws) {
		bar.set_fraction(ws.w.current_crisis.temperature / 100.0f);
	}
	void crisis_temperature_bar::create_tooltip(ui::progress_bar<crisis_temperature_bar>& bar, world_state & ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		char16_t local_buffer[16];
		put_value_in_buffer(local_buffer, display_type::integer, ws.w.current_crisis.temperature);
		
		text_data::replacement repl(text_data::value_type::value,
			vector_backed_string<char16_t>(local_buffer),
			[](ui::tagged_gui_object) {});

		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_temperature],
			ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &repl, 1);
	}
	void crisis_attacker_gp_flag::update(ui::masked_flag<crisis_attacker_gp_flag>& self, world_state & ws) {
		if(auto p = ws.w.current_crisis.primary_attacker; p)
			self.set_displayed_flag(ws, ws.w.nation_s.nations.get<nation::tag>(p));
		else
			self.set_displayed_flag(ws, cultures::national_tag());
	}
	void crisis_defender_gp_flag::update(ui::masked_flag<crisis_defender_gp_flag>& self, world_state & ws) {
		if(auto p = ws.w.current_crisis.primary_defender; p)
			self.set_displayed_flag(ws, p);
		else
			self.set_displayed_flag(ws, cultures::national_tag());
	}
	ui::window_tag crisis_attacker_wg_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["wargoal"]);
	}
	ui::window_tag crisis_defender_wg_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["wargoal"]);
	}
	ui::window_tag crisis_attacker_backer_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	ui::window_tag crisis_defender_backer_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	void crisis_attacker_country_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto t = ws.w.current_crisis.primary_attacker; t)
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(t),
				ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void crisis_defender_country_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto t = ws.w.current_crisis.primary_defender; t)
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(t),
				ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
	void crisis_support_attacker_button::button_function(ui::simple_button<crisis_support_attacker_button>&, world_state &) {}
	void crisis_attacker_back_down_button::button_function(ui::simple_button<crisis_attacker_back_down_button>&, world_state &) {}
	void crisis_support_defender_button::button_function(ui::simple_button<crisis_support_defender_button>&, world_state &) {}
	void crisis_defender_back_down_button::button_function(ui::simple_button<crisis_defender_back_down_button>&, world_state &) {}
	void make_offer_button::button_function(ui::simple_button<make_offer_button>&, world_state &) {}
	void make_offer_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void clear_text_button::on_create(ui::simple_button<clear_text_button>& self, world_state& ws) {
		ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *self.associated_object, ui::gui_object_tag(0) });
	}
	void filter_all_button::update(ui::simple_button<filter_all_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::all) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_all_button::button_function(ui::simple_button<filter_all_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = country_sub_filter::all;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_north_america_button::update(ui::simple_button<filter_north_america_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::continent_north_america) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_north_america_button::button_function(ui::simple_button<filter_north_america_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::continent_north_america) ?
			country_sub_filter::all :
			country_sub_filter::continent_north_america;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_south_america_button::update(ui::simple_button<filter_south_america_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::continent_south_america) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_south_america_button::button_function(ui::simple_button<filter_south_america_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::continent_south_america) ?
			country_sub_filter::all :
			country_sub_filter::continent_south_america;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_europe_button::update(ui::simple_button<filter_europe_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::continent_europe) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_europe_button::button_function(ui::simple_button<filter_europe_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::continent_europe) ?
			country_sub_filter::all :
			country_sub_filter::continent_europe;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_africa_button::update(ui::simple_button<filter_africa_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::continent_africa) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_africa_button::button_function(ui::simple_button<filter_africa_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::continent_africa) ?
			country_sub_filter::all :
			country_sub_filter::continent_africa;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_asia_button::update(ui::simple_button<filter_asia_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::continent_asia) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_asia_button::button_function(ui::simple_button<filter_asia_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::continent_asia) ?
			country_sub_filter::all :
			country_sub_filter::continent_asia;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_oceania_button::update(ui::simple_button<filter_oceania_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::continent_oceania) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_oceania_button::button_function(ui::simple_button<filter_oceania_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::continent_oceania) ?
			country_sub_filter::all :
			country_sub_filter::continent_oceania;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_neighbours_button::update(ui::simple_button<filter_neighbours_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::neighbor) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_neighbours_button::button_function(ui::simple_button<filter_neighbours_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::neighbor) ?
			country_sub_filter::all :
			country_sub_filter::neighbor;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_sphere_button::update(ui::simple_button<filter_sphere_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::sphere) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_sphere_button::button_function(ui::simple_button<filter_sphere_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::sphere) ?
			country_sub_filter::all :
			country_sub_filter::sphere;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_enemies_button::update(ui::simple_button<filter_enemies_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::enemy) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_enemies_button::button_function(ui::simple_button<filter_enemies_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::enemy) ?
			country_sub_filter::all :
			country_sub_filter::enemy;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void filter_allies_button::update(ui::simple_button<filter_allies_button>& self, world_state & ws) {
		if(ws.w.diplomacy_w.filter == country_sub_filter::ally) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_allies_button::button_function(ui::simple_button<filter_allies_button>&, world_state & ws) {
		ws.w.diplomacy_w.filter = (ws.w.diplomacy_w.filter == country_sub_filter::ally) ?
			country_sub_filter::all :
			country_sub_filter::ally;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_country_button::button_function(ui::simple_button<sort_by_country_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::name;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_prio_button::button_function(ui::simple_button<sort_by_prio_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::priority;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag0_button::update(ui::masked_flag<sort_by_gpflag0_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) != end(r)) {
			if(auto id = r.first[0]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag0_button::button_function(ui::masked_flag<sort_by_gpflag0_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_one;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag1_button::update(ui::masked_flag<sort_by_gpflag1_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 1 < end(r)) {
			if(auto id = r.first[1]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag1_button::button_function(ui::masked_flag<sort_by_gpflag1_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_two;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag2_button::update(ui::masked_flag<sort_by_gpflag2_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 2 < end(r)) {
			if(auto id = r.first[2]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag2_button::button_function(ui::masked_flag<sort_by_gpflag2_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_three;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag3_button::update(ui::masked_flag<sort_by_gpflag3_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 3 < end(r)) {
			if(auto id = r.first[3]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag3_button::button_function(ui::masked_flag<sort_by_gpflag3_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_four;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag4_button::update(ui::masked_flag<sort_by_gpflag4_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 4 < end(r)) {
			if(auto id = r.first[4]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag4_button::button_function(ui::masked_flag<sort_by_gpflag4_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_five;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag5_button::update(ui::masked_flag<sort_by_gpflag5_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 5 < end(r)) {
			if(auto id = r.first[5]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag5_button::button_function(ui::masked_flag<sort_by_gpflag5_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_six;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag6_button::update(ui::masked_flag<sort_by_gpflag6_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 6 < end(r)) {
			if(auto id = r.first[6]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag6_button::button_function(ui::masked_flag<sort_by_gpflag6_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_seven;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_gpflag7_button::update(ui::masked_flag<sort_by_gpflag7_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 7 < end(r)) {
			if(auto id = r.first[7]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag7_button::button_function(ui::masked_flag<sort_by_gpflag7_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::gp_eight;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_boss_button::button_function(ui::simple_button<sort_by_boss_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::sphere_leader;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_prestige_button::button_function(ui::simple_button<sort_by_prestige_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::prestige_rank;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_economic_button::button_function(ui::simple_button<sort_by_economic_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::economic_rank;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_military_button::button_function(ui::simple_button<sort_by_military_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::military_rank;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_total_button::button_function(ui::simple_button<sort_by_total_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::overall_rank;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_opinion_button::button_function(ui::simple_button<sort_by_opinion_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::opinion;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void sort_by_relation_button::button_function(ui::simple_button<sort_by_relation_button>&, world_state & ws) {
		ws.w.diplomacy_w.sort_type = country_sort::relation;
		ws.w.diplomacy_w.update_diplomacy_window(ws.w.gui_m);
	}
	void nation_details_background_button::button_function(ui::simple_button<nation_details_background_button>& self, world_state & ws) {
		ws.w.diplomacy_w.show_diplomacy_window(ws.w.gui_m, tag);
	}
	void nation_details_priority_button::button_function(ui::simple_button<nation_details_priority_button>& self, world_state & ws) {}
	ui::window_tag nations_details_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_country_info"]);
	}


	diplomacy_window::diplomacy_window() : win(std::make_unique<diplomacy_window_t>()) {}

	diplomacy_window::~diplomacy_window() {}

	void diplomacy_window::hide_diplomacy_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
		win->template get<CT_STRING("country_listbox")>().new_list(nullptr, nullptr);
	}
	void diplomacy_window::update_diplomacy_window(ui::gui_manager& gui_m) {
		win->template get<CT_STRING("country_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void diplomacy_window::show_diplomacy_window(ui::gui_manager& gui_m, nations::country_tag t) {
		selected_nation = t;
		goto_selected_pending = true;

		win->template get<CT_STRING("country_listbox")>().new_list(nullptr, nullptr);
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void diplomacy_window::show_diplomacy_window_gp(ui::gui_manager& gui_m) {
		win->template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 0);

		for(uint32_t i = 0; i < std::extent_v<decltype(win->gp_items)>; ++i)
			ui::make_visible_and_update(gui_m, *(win->gp_items[i].associated_object));
		ui::hide(*(win->template get <CT_STRING("crisis_info_win")>().associated_object));
		ui::hide(*(win->template get <CT_STRING("cb_info_win")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("war_listbox")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void diplomacy_window::show_diplomacy_window_wars(ui::gui_manager& gui_m) {

		win->template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 1);
		for(uint32_t i = 0; i < std::extent_v<decltype(win->gp_items)>; ++i)
			ui::hide(*(win->gp_items[i].associated_object));
		ui::hide(*(win->template get <CT_STRING("crisis_info_win")>().associated_object));
		ui::hide(*(win->template get <CT_STRING("cb_info_win")>().associated_object));
		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("war_listbox")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void diplomacy_window::show_diplomacy_window_crisis(ui::gui_manager& gui_m) {

		win->template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 3);
		for(uint32_t i = 0; i < std::extent_v<decltype(win->gp_items)>; ++i)
			ui::hide(*(win->gp_items[i].associated_object));
		ui::make_visible_and_update(gui_m, *(win->template get <CT_STRING("crisis_info_win")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("war_listbox")>().associated_object));
		ui::hide(*(win->template get <CT_STRING("cb_info_win")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void diplomacy_window::show_diplomacy_window_cbs(ui::gui_manager& gui_m) {

		win->template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 2);
		for(uint32_t i = 0; i < std::extent_v<decltype(win->gp_items)>; ++i)
			ui::hide(*(win->gp_items[i].associated_object));
		ui::make_visible_and_update(gui_m, *(win->template get <CT_STRING("cb_info_win")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("war_listbox")>().associated_object));
		ui::hide(*(win->template get <CT_STRING("crisis_info_win")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void diplomacy_window::init_diplomacy_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_diplomacy"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void cb_item_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.military_m.cb_types[tag].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void cb_item_cancel_button::button_function(ui::simple_button<cb_item_cancel_button>& self, world_state & ws) {}

	ui::window_tag cb_fabrication_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_cb_info_player"]);
	}
	void cb_item_base::update(world_state & ws) {
		if(is_valid_index(tag)) {
			cb_fabrication_by.template get<CT_STRING("country_flag")>().set_displayed_flag(ws, tag);

			if(auto id = ws.w.nation_s.nations.get<nation::cb_construction_target>(tag); ws.w.nation_s.nations.is_valid_index(id))
				cb_fabrication_target.template get<CT_STRING("country_flag")>().set_displayed_flag(ws, id);
			else
				cb_fabrication_target.template get<CT_STRING("country_flag")>().set_displayed_flag(ws, cultures::national_tag());
		}
	}
	ui::window_tag gp_sphere::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	void details_flag_frame::update(ui::dynamic_transparent_icon<details_flag_frame>& ico, world_state & ws) {
		
		if(auto selected_nation = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected_nation)) {
			if(ws.w.nation_s.nations.get<nation::is_civilized>(selected_nation) == false)
				ico.set_frame(ws.w.gui_m, 3ui32);
			else if(nations::is_great_power(ws, selected_nation))
				ico.set_frame(ws.w.gui_m, 0ui32);
			else if(ws.w.nation_s.nations.get<nation::overall_rank>(selected_nation) <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
				ico.set_frame(ws.w.gui_m, 1ui32);
			else
				ico.set_frame(ws.w.gui_m, 2ui32);
		}
	}
	void details_flag::update(ui::masked_flag<details_flag>& self, world_state & ws) {
		self.set_displayed_flag(ws, ws.w.diplomacy_w.selected_nation);
	}
	void details_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(selected), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	void details_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::overall_rank>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_relation_value::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(auto player = ws.w.local_player_nation; player && selected != player) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::exact_integer, nations::get_relationship(ws, player, selected));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			} else {
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"--"), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
			
		}
	}
	void details_rank_status::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, nations::get_nation_status_text(ws, selected), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	void details_government_type::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(auto gov = ws.w.nation_s.nations.get<nation::current_government>(selected); is_valid_index(gov)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.governments_m.governments_container[gov].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}
	void details_party::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(auto party = ws.w.nation_s.nations.get<nation::ruling_party>(selected); is_valid_index(party)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.governments_m.parties[party].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}
	void details_party_icon::update(ui::tinted_icon<details_party_icon>& self, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(auto ideology = ws.w.nation_s.nations.get<nation::ruling_ideology>(selected); is_valid_index(ideology)) {
				auto ideology_color = ws.s.ideologies_m.ideology_container[ideology].color;
				self.set_color(ws.w.gui_m, float(ideology_color.r) / 255.0f, float(ideology_color.g) / 255.0f, float(ideology_color.b) / 255.0f);
			}
		}
	}
	void details_tech_school::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(auto school = ws.w.nation_s.nations.get<nation::tech_school>(selected); is_valid_index(school)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.modifiers_m.national_modifiers[school].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}
	void details_prestige::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, nations::get_prestige(ws, selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_industrial_score::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::industrial_score>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_military_score::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::military_score>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_total_score::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::military_score>(selected)
				+ ws.w.nation_s.nations.get<nation::industrial_score>(selected) + nations::get_prestige(ws, selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_prestige_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::prestige_rank>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_industrial_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::industrial_rank>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_military_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::military_rank>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_overall_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.nation_s.nations.get<nation::overall_rank>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_population_value::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nation_demographics.get(selected, population::total_population_tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_primary_culture::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			if(auto c = ws.w.nation_s.nations.get<nation::primary_culture>(selected); is_valid_index(c)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.culture_m.culture_container[c].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}
	void details_accepted_cultures::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			auto r = get_range(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(selected));
			ui::xy_pair cursor{ 0,0 };
			if(r.first != r.second) 
				cursor = ui::add_linear_text(cursor, ws.s.culture_m.culture_container[*r.first].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			for(auto i = r.first + 1; i < r.second; ++i) {
				cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u", "), box, cursor, fmt, lm);
				cursor = ui::add_linear_text(cursor, ws.s.culture_m.culture_container[*i].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			}
			lm.finish_current_line();
		}
	}
	ui::window_tag details_gp_sphere::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	ui::window_tag details_gp_friendly::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	ui::window_tag details_gp_coridal::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	void details_infamy_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::infamy], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_infamy_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::fp_one_place, ws.w.nation_s.nations.get<nation::infamy>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_infamy_text::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::infamy], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_war_exhaustion_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::war_exhaustion], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_war_exhaustion_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::percent, ws.w.nation_s.nations.get<nation::war_exhaustion>(selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_war_exhaustion_text::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::war_exhaustion], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_brigade_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::brigades], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_brigade_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, military::total_active_divisions(ws, selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_brigade_text::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::brigades], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_ships_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::warships], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_ships_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto selected = ws.w.diplomacy_w.selected_nation; ws.w.nation_s.nations.is_valid_index(selected)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, military::total_active_ships(ws, selected));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void details_ships_text::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::warships], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void details_add_wg_button::update(ui::simple_button<details_add_wg_button>& self, world_state & ws) {}
	void details_add_wg_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	ui::window_tag details_wars::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	ui::window_tag details_allies::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	ui::window_tag details_protected::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	void flag_truce_sub_item::create_tooltip(ui::masked_flag<flag_truce_sub_item>& self, world_state & ws, ui::tagged_gui_object tw) {
		if(ws.w.nation_s.nations.is_valid_index(c)) {
			ui::line_manager lm;
			auto cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(c), ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);

			char16_t local_buffer[32];
			u16_format_date(local_buffer, until);
			text_data::replacement repl{ text_data::value_type::date, vector_backed_string<char16_t>(local_buffer), [](ui::tagged_gui_object) {} };

			cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::truce_lasts_until],
				ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &repl, 1);
		}
	}
	ui::window_tag details_truce::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["diplomacy_puppet"]);
	}
	void details_cb_sub_item::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;
		if(is_valid_index(this_cb.type)) {
			cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.military_m.cb_types[this_cb.type].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
		}
		if(ws.w.nation_s.nations.is_valid_index(this_cb.target)) {
			cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::against], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(this_cb.target), ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
		}
		if(is_valid_index(this_cb.expiration)) {
			char16_t local_buffer[32];
			u16_format_date(local_buffer, this_cb.expiration);
			text_data::replacement repl{ text_data::value_type::date, vector_backed_string<char16_t>(local_buffer), [](ui::tagged_gui_object) {} };

			cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::expires_on],
				ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &repl, 1);
		}
	}
	ui::window_tag details_cbs::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["wargoal"]);
	}
	ui::window_tag details_wargoals::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["wargoal"]);
	}
	void generic_influence_action_button::button_function(ui::simple_button<generic_influence_action_button>&, world_state &) {}
	void generic_influence_action_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}

	void influence_details_base::update(world_state & ws) {
		influence_button.gp_index = gp_index;
		if(auto player = ws.w.local_player_nation; player) {
			if(is_great_power(ws, player))
				ui::make_visible_immediate(*influence_button.associated_object);
			else
				ui::hide(*influence_button.associated_object);

			influence_button.set_frame(ws.w.gui_m, 1ui32);
			if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); begin(r) + gp_index < end(r)) {
				if(player == r.first[gp_index])
					influence_button.set_frame(ws.w.gui_m, 0ui32);
			}
		} else {
			ui::hide(*influence_button.associated_object);
		}
	}
	void decrease_relations_button::button_function(ui::simple_button<decrease_relations_button>&, world_state &) {}
	void decrease_relations_button::update(ui::simple_button<decrease_relations_button>& self, world_state & ws) {}
	void decrease_relations_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void increase_relations_button::button_function(ui::simple_button<increase_relations_button>&, world_state &) {}
	void increase_relations_button::update(ui::simple_button<increase_relations_button>& self, world_state & ws) {}
	void increase_relations_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void declare_war_offer_peace_button::button_function(ui::button<declare_war_offer_peace_button>&, world_state &) {}
	void declare_war_offer_peace_button::update(ui::button<declare_war_offer_peace_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(player == ws.w.diplomacy_w.selected_nation) {
				self.set_enabled(false);
				return;
			}
			self.set_enabled(true);
			if(contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::opponents_in_war>(player), ws.w.diplomacy_w.selected_nation)) {
				self.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::offer_peace_button]);
			} else {
				self.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::declare_war_button]);
			}
		}
	}
	void declare_war_offer_peace_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void justify_war_button::button_function(ui::button<justify_war_button>&, world_state &) {}
	void justify_war_button::update(ui::button<justify_war_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(player == ws.w.diplomacy_w.selected_nation) {
				self.set_enabled(false);
				return;
			}
			if(is_valid_index(ws.w.nation_s.nations.get<nation::cb_construction_target>(player)))
				self.set_enabled(false);
			else
				self.set_enabled(true);
		}
	}
	void justify_war_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void ban_embassy_button::button_function(ui::button<ban_embassy_button>&, world_state &) {}
	void ban_embassy_button::update(ui::button<ban_embassy_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(player == ws.w.diplomacy_w.selected_nation) {
				self.set_enabled(false);
				return;
			}
			if(!is_great_power(ws, player)) {
				self.set_enabled(false);
				return;
			}
			self.set_enabled(true);
		}
	}
	void ban_embassy_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void expel_advisors_button::button_function(ui::button<expel_advisors_button>&, world_state &) {}
	void expel_advisors_button::update(ui::button<expel_advisors_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(player == ws.w.diplomacy_w.selected_nation) {
				self.set_enabled(false);
				return;
			}
			if(!is_great_power(ws, player)) {
				self.set_enabled(false);
				return;
			}
			self.set_enabled(true);
		}
	}
	void expel_advisors_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void call_ally_button::button_function(ui::button<call_ally_button>&, world_state &) {}
	void call_ally_button::update(ui::button<call_ally_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(player == ws.w.diplomacy_w.selected_nation) {
				self.set_enabled(false);
				return;
			}
			if(contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(player), ws.w.diplomacy_w.selected_nation)) {
				self.set_enabled(true);
			} else {
				self.set_enabled(false);
			}
		}
	}
	void call_ally_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void make_break_alliance_button::button_function(ui::button<make_break_alliance_button>&, world_state &) {}
	void make_break_alliance_button::update(ui::button<make_break_alliance_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(player == ws.w.diplomacy_w.selected_nation) {
				self.set_enabled(false);
				return;
			}
			self.set_enabled(true);
			if(contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(player), ws.w.diplomacy_w.selected_nation)) {
				self.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::cancel_alliance_button]);
			} else {
				self.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::make_alliance_button]);
			}
		}
	}
	void make_break_alliance_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void gp_heading::on_create(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::great_powers], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
}
