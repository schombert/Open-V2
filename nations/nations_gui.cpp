#include "common\\common.h"
#include "nations_gui.hpp"

namespace nations {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.hide_diplomacy_window();
	}
	void diplomacy_tab_button_group::on_select(world_state & ws, uint32_t i) {
		if(i == 0) {
			ws.w.show_diplomacy_window_gp();
		} else if(i == 1) {
			ws.w.show_diplomacy_window_wars();
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
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, target->adjective),
				[](ui::tagged_gui_object) {});
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_title], fmt, ws.s.gui_m, ws.w.gui_m, box, lm, &repl, 1);
			lm.finish_current_line();
		} else {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_crisis], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	void crisis_subtitle::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto target_name = [target = ws.w.current_crisis.target]() { return bool(target) ? target->name : text_data::text_tag(); }();
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
			if(ws.w.current_crisis.primary_attacker == nullptr || ws.w.current_crisis.primary_defender == nullptr)
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
			self.set_displayed_flag(ws, p->tag);
		else
			self.set_displayed_flag(ws, cultures::national_tag());
	}
	void crisis_defender_gp_flag::update(ui::masked_flag<crisis_defender_gp_flag>& self, world_state & ws) {
		if(auto p = ws.w.current_crisis.primary_defender; p)
			self.set_displayed_flag(ws, p->tag);
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
		if(auto t = ws.w.current_crisis.primary_attacker)
			ui::add_linear_text(ui::xy_pair{ 0,0 }, t->name,
				ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void crisis_defender_country_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto t = ws.w.current_crisis.primary_defender)
			ui::add_linear_text(ui::xy_pair{ 0,0 }, t->name,
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
		if(ws.w.selected_diplomacy.filter_africa && ws.w.selected_diplomacy.filter_asia && ws.w.selected_diplomacy.filter_europe &&
			ws.w.selected_diplomacy.filter_oceania && ws.w.selected_diplomacy.filter_north_america && ws.w.selected_diplomacy.filter_oceania
			&& ws.w.selected_diplomacy.filter_south_america && !ws.w.selected_diplomacy.filter_allies && !ws.w.selected_diplomacy.filter_enemies
			&& !ws.w.selected_diplomacy.filter_neighbors && !ws.w.selected_diplomacy.filter_sphere) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_all_button::button_function(ui::simple_button<filter_all_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_north_america = true;
		ws.w.selected_diplomacy.filter_south_america = true;
		ws.w.selected_diplomacy.filter_europe = true;
		ws.w.selected_diplomacy.filter_africa = true;
		ws.w.selected_diplomacy.filter_asia = true;
		ws.w.selected_diplomacy.filter_oceania = true;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_north_america_button::update(ui::simple_button<filter_north_america_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_north_america) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_north_america_button::button_function(ui::simple_button<filter_north_america_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_north_america = !ws.w.selected_diplomacy.filter_north_america;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_south_america_button::update(ui::simple_button<filter_south_america_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_south_america) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_south_america_button::button_function(ui::simple_button<filter_south_america_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_south_america = !ws.w.selected_diplomacy.filter_south_america;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_europe_button::update(ui::simple_button<filter_europe_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_europe) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_europe_button::button_function(ui::simple_button<filter_europe_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_europe = !ws.w.selected_diplomacy.filter_europe;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_africa_button::update(ui::simple_button<filter_africa_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_africa) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_africa_button::button_function(ui::simple_button<filter_africa_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_africa = !ws.w.selected_diplomacy.filter_africa;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_asia_button::update(ui::simple_button<filter_asia_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_asia) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_asia_button::button_function(ui::simple_button<filter_asia_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_asia = !ws.w.selected_diplomacy.filter_asia;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_oceania_button::update(ui::simple_button<filter_oceania_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_oceania) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_oceania_button::button_function(ui::simple_button<filter_oceania_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_oceania = !ws.w.selected_diplomacy.filter_oceania;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_neighbours_button::update(ui::simple_button<filter_neighbours_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_neighbors) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_neighbours_button::button_function(ui::simple_button<filter_neighbours_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_north_america = true;
		ws.w.selected_diplomacy.filter_south_america = true;
		ws.w.selected_diplomacy.filter_europe = true;
		ws.w.selected_diplomacy.filter_africa = true;
		ws.w.selected_diplomacy.filter_asia = true;
		ws.w.selected_diplomacy.filter_oceania = true;
		ws.w.selected_diplomacy.filter_neighbors = !ws.w.selected_diplomacy.filter_neighbors;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_sphere_button::update(ui::simple_button<filter_sphere_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_sphere) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_sphere_button::button_function(ui::simple_button<filter_sphere_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_north_america = true;
		ws.w.selected_diplomacy.filter_south_america = true;
		ws.w.selected_diplomacy.filter_europe = true;
		ws.w.selected_diplomacy.filter_africa = true;
		ws.w.selected_diplomacy.filter_asia = true;
		ws.w.selected_diplomacy.filter_oceania = true;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = !ws.w.selected_diplomacy.filter_sphere;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_enemies_button::update(ui::simple_button<filter_enemies_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_enemies) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_enemies_button::button_function(ui::simple_button<filter_enemies_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_north_america = true;
		ws.w.selected_diplomacy.filter_south_america = true;
		ws.w.selected_diplomacy.filter_europe = true;
		ws.w.selected_diplomacy.filter_africa = true;
		ws.w.selected_diplomacy.filter_asia = true;
		ws.w.selected_diplomacy.filter_oceania = true;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = !ws.w.selected_diplomacy.filter_enemies;
		ws.w.selected_diplomacy.filter_allies = false;
		ws.w.update_diplomacy_window();
	}
	void filter_allies_button::update(ui::simple_button<filter_allies_button>& self, world_state & ws) {
		if(ws.w.selected_diplomacy.filter_allies) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_allies_button::button_function(ui::simple_button<filter_allies_button>&, world_state & ws) {
		ws.w.selected_diplomacy.filter_north_america = true;
		ws.w.selected_diplomacy.filter_south_america = true;
		ws.w.selected_diplomacy.filter_europe = true;
		ws.w.selected_diplomacy.filter_africa = true;
		ws.w.selected_diplomacy.filter_asia = true;
		ws.w.selected_diplomacy.filter_oceania = true;
		ws.w.selected_diplomacy.filter_neighbors = false;
		ws.w.selected_diplomacy.filter_sphere = false;
		ws.w.selected_diplomacy.filter_enemies = false;
		ws.w.selected_diplomacy.filter_allies = !ws.w.selected_diplomacy.filter_allies;
		ws.w.update_diplomacy_window();
	}
	void sort_by_country_button::button_function(ui::simple_button<sort_by_country_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::name;
		ws.w.update_diplomacy_window();
	}
	void sort_by_prio_button::button_function(ui::simple_button<sort_by_prio_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::priority;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag0_button::update(ui::masked_flag<sort_by_gpflag0_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 0 < r.second) {
			if(auto id = *(r.first + 0); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag0_button::button_function(ui::masked_flag<sort_by_gpflag0_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_one;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag1_button::update(ui::masked_flag<sort_by_gpflag1_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 1 < r.second) {
			if(auto id = *(r.first + 1); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag1_button::button_function(ui::masked_flag<sort_by_gpflag1_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_two;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag2_button::update(ui::masked_flag<sort_by_gpflag2_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 2 < r.second) {
			if(auto id = *(r.first + 2); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag2_button::button_function(ui::masked_flag<sort_by_gpflag2_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_three;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag3_button::update(ui::masked_flag<sort_by_gpflag3_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 3 < r.second) {
			if(auto id = *(r.first + 3); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag3_button::button_function(ui::masked_flag<sort_by_gpflag3_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_four;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag4_button::update(ui::masked_flag<sort_by_gpflag4_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 4 < r.second) {
			if(auto id = *(r.first + 4); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag4_button::button_function(ui::masked_flag<sort_by_gpflag4_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_five;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag5_button::update(ui::masked_flag<sort_by_gpflag5_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 5 < r.second) {
			if(auto id = *(r.first + 5); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag5_button::button_function(ui::masked_flag<sort_by_gpflag5_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_six;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag6_button::update(ui::masked_flag<sort_by_gpflag6_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 6 < r.second) {
			if(auto id = *(r.first + 6); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag6_button::button_function(ui::masked_flag<sort_by_gpflag6_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_seven;
		ws.w.update_diplomacy_window();
	}
	void sort_by_gpflag7_button::update(ui::masked_flag<sort_by_gpflag7_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 7 < r.second) {
			if(auto id = *(r.first + 7); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag7_button::button_function(ui::masked_flag<sort_by_gpflag7_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::gp_eight;
		ws.w.update_diplomacy_window();
	}
	void sort_by_boss_button::button_function(ui::simple_button<sort_by_boss_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::sphere_leader;
		ws.w.update_diplomacy_window();
	}
	void sort_by_prestige_button::button_function(ui::simple_button<sort_by_prestige_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::prestige_rank;
		ws.w.update_diplomacy_window();
	}
	void sort_by_economic_button::button_function(ui::simple_button<sort_by_economic_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::economic_rank;
		ws.w.update_diplomacy_window();
	}
	void sort_by_military_button::button_function(ui::simple_button<sort_by_military_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::military_rank;
		ws.w.update_diplomacy_window();
	}
	void sort_by_total_button::button_function(ui::simple_button<sort_by_total_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::overall_rank;
		ws.w.update_diplomacy_window();
	}
	void sort_by_opinion_button::button_function(ui::simple_button<sort_by_opinion_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::opinion;
		ws.w.update_diplomacy_window();
	}
	void sort_by_relation_button::button_function(ui::simple_button<sort_by_relation_button>&, world_state & ws) {
		ws.w.selected_diplomacy.sort_type = current_state::country_sort::relation;
		ws.w.update_diplomacy_window();
	}
	void nation_details_background_button::button_function(ui::simple_button<nation_details_background_button>& self, world_state & ws) {
		ws.w.show_diplomacy_window(tag);
	}
	void nation_details_priority_button::button_function(ui::simple_button<nation_details_priority_button>& self, world_state & ws) {}
}
