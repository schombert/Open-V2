#include "common\\common.h"
#include "topbar.hpp"
#include "nations\\nations_functions.hpp"
#include "technologies\\technologies_functions.h"
#include "modifiers\\modifiers_gui.h"
#include "modifiers\\modifier_functions.h"
#include "governments\\governments_functions.h"

namespace current_state {
	void topbar_bg::on_create(ui::simple_button<topbar_bg>& bttn, world_state &) {
		bttn.associated_object->flags.fetch_or(ui::gui_object::force_transparency_check, std::memory_order_acq_rel);
		bttn.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::interactable), std::memory_order_acq_rel);
	}

	void player_flag::update(ui::masked_flag<player_flag>& self, world_state& ws) {
		auto player = ws.w.local_player_nation;
		if(player)
			self.set_displayed_flag(ws, player);
	}

	void player_flag_frame::update(ui::dynamic_transparent_icon<player_flag_frame>& ico, world_state& ws) {
		auto player = ws.w.local_player_nation;
		if(player) {
			if(ws.w.nation_s.nations.get<nation::is_civilized>(player) == false)
				ico.set_frame(ws.w.gui_m, 3ui32);
			else if(nations::is_great_power(ws, player))
				ico.set_frame(ws.w.gui_m, 0ui32);
			else if(ws.w.nation_s.nations.get<nation::overall_rank>(player) <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
				ico.set_frame(ws.w.gui_m, 1ui32);
			else
				ico.set_frame(ws.w.gui_m, 2ui32);
		}
	}
	void production_button::button_function(ui::simple_button<production_button>&, world_state & ws) {
		ws.w.production_w.show_factories(ws.w.gui_m);
	}
	void production_button::update(ui::simple_button<production_button>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void budget_button::button_function(ui::simple_button<budget_button>&, world_state& ws) {
		ws.w.budget_w.show(ws.w.gui_m);
	}
	void budget_button::update(ui::simple_button<budget_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void tech_button::button_function(ui::simple_button<tech_button>&, world_state& ws) {
		ws.w.technologies_w.show_technology_window(ws.w.gui_m);
	};
	void tech_button::update(ui::simple_button<tech_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void politics_button::button_function(ui::simple_button<politics_button>&, world_state& ws) {
		ws.w.government_w.show_government_reforms_tab(ws.w.gui_m);
	}
	void politics_button::update(ui::simple_button<politics_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void pops_button::button_function(ui::simple_button<pops_button>&, world_state& ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id))
				ws.w.population_w.show_population_window(ws.w.gui_m, id);
		}
	}
	void pops_button::update(ui::simple_button<pops_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void trade_button::button_function(ui::simple_button<trade_button>&, world_state& ws) {
		ws.w.trade_w.show(ws.w.gui_m);
	}
	void trade_button::update(ui::simple_button<trade_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void diplomacy_button::button_function(ui::simple_button<diplomacy_button>&, world_state& ws) {
		ws.w.diplomacy_w.show_diplomacy_window(ws.w.gui_m, ws.w.local_player_nation ? ws.w.local_player_nation : nations::country_tag());
	}
	void diplomacy_button::update(ui::simple_button<diplomacy_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void military_button::button_function(ui::simple_button<military_button>&, world_state&) {}
	void military_button::update(ui::simple_button<military_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}

	void building_factories::update(ui::dynamic_icon<building_factories>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void building_factories::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::not_building_factories], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void closed_factories::update(ui::dynamic_icon<closed_factories>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void closed_factories::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_closed_factories], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void unemployed_workers::update(ui::dynamic_icon<unemployed_workers>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto id = player;
			if(ws.w.nation_s.nations.is_valid_index(id)) {
				for(auto w : ws.s.population_m.factory_workers) {
					if(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, w)) < ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, w)))
						self.set_frame(ws.w.gui_m, 0ui32);
				}
			}
		}
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void unemployed_workers::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::xy_pair cursor{ 0,0 };
		bool has_unemployed = false;

		if(auto player = ws.w.local_player_nation; player) {
			auto id = player;
			if(ws.w.nation_s.nations.is_valid_index(id)) {
				for(auto w : ws.s.population_m.factory_workers) {
					if(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, w)) < ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, w)))
						has_unemployed = true;
				}
			}

			ui::unlimited_line_manager lm;

			if(has_unemployed) {
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::unemployed_workers], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();
				lm.increase_indent(1);
				nations::for_each_state(ws, player, [&ws, &cursor, tw, &lm](nations::state_tag si_id) {
					if(ws.w.nation_s.states.is_valid_index(si_id)) {
						for(auto w : ws.s.population_m.factory_workers) {
							if(ws.w.nation_s.state_demographics.get(si_id, population::to_employment_demo_tag(ws, w)) <
								ws.w.nation_s.state_demographics.get(si_id, population::to_demo_tag(ws, w))) {
								cursor = ui::add_linear_text(cursor, ws.w.nation_s.states.get<::state::name>(si_id), ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
								cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
								lm.finish_current_line();
								return;
							}

						}
					}
				});
			} else {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_unemployed_workers], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
			}
		} else {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_unemployed_workers], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
		}
	}
	void budget_linechart::update(ui::linechart<budget_linechart, 32>& self, world_state & ws) {
		float adjusted_values[32];

		float most_extreme = 1.0f;
		for(uint32_t i = 0; i < 32; ++i) {
			most_extreme = std::max(most_extreme, std::abs(ws.w.local_player_data.income_history.values[i]));
		}
		for(int32_t i = 0; i < 32; ++i) {
			adjusted_values[i] = (ws.w.local_player_data.income_history.values[(int32_t(to_index(ws.w.current_date)) - (31 - i)) % 32] * 0.5f)  / most_extreme + 0.5f;
		}

		self.set_values(ws.w.gui_m, adjusted_values);
	}
	void budget_linechart::create_tooltip(world_state & ws, ui::tagged_gui_object tw, int32_t x) {
		ui::xy_pair cursor{ 0,0 };
		char16_t local_buf[64];

		auto income_value = ws.w.local_player_data.income_history.values[(int32_t(to_index(ws.w.current_date)) - (31 - x)) % 32];

		if(income_value < 0.0f) {
			local_buf[0] = u'-';
			auto end_pos = put_pos_value_in_buffer(local_buf + 1, display_type::fp_two_places, -income_value);
			*end_pos = u'\u00A3';
			*(end_pos + 1) = char16_t(0);
			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				ui::text_format{ ui::text_color::red, ui::tooltip_text_format.font_handle, ui::tooltip_text_format.font_size }
				);
		} else {
			local_buf[0] = u'+';
			auto end_pos = put_pos_value_in_buffer(local_buf + 1, display_type::fp_two_places, income_value);
			*end_pos = u'\u00A3';
			*(end_pos + 1) = char16_t(0);
			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				ui::text_format{ ui::text_color::green, ui::tooltip_text_format.font_handle, ui::tooltip_text_format.font_size }
				);
		}
	}
	void treasury_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto pid = player;
			if(ws.w.nation_s.nations.is_valid_index(pid)) {
				char16_t local_buf[64];

				auto tval = ws.w.nation_s.nations.get<nation::treasury>(pid);
				auto end_pos = put_value_in_buffer(local_buf, display_type::integer, tval);
				*end_pos = u'\u00A3';
				*(end_pos + 1) = char16_t(0);
				ui::text_chunk_to_instances(
					ws.s.gui_m,
					ws.w.gui_m,
					vector_backed_string<char16_t>(local_buf),
					box,
					ui::xy_pair{ 0,0 },
					fmt,
					lm
				);

				lm.finish_current_line();
			}
		}
	}
	void current_research_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto cr = ws.w.nation_s.nations.get<nation::current_research>(player);
			if(is_valid_index(cr)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.technology_m.technologies_container[cr].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			} else {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_research], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			}
			lm.finish_current_line();
		}
	}
	void research_progress_bar::update(ui::progress_bar<research_progress_bar>& bar, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto tech = ws.w.nation_s.nations.get<nation::current_research>(player); is_valid_index(tech)) {
				if(auto total_points = technologies::effective_tech_cost(tech, ws, player); total_points != 0.0f) {
					bar.set_fraction(std::min(1.0f, float(ws.w.nation_s.nations.get<nation::research_points>(player)) / total_points));
					return;
				}
			}
		}
		bar.set_fraction(0.0f);
	}
	void literacy_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto pid = player;
			if(ws.w.nation_s.nations.is_valid_index(pid)) {
				auto total_pop = ws.w.nation_s.nation_demographics.get(pid, population::total_population_tag);
				auto literacy = ws.w.nation_s.nation_demographics.get(pid, population::literacy_demo_tag(ws));
				if(total_pop != 0) {
					char16_t local_buf[16];

					put_value_in_buffer(local_buf, display_type::percent, float(literacy) / float(total_pop));

					ui::text_chunk_to_instances(
						ws.s.gui_m,
						ws.w.gui_m,
						vector_backed_string<char16_t>(local_buf),
						box,
						ui::xy_pair{ 0,0 },
						fmt,
						lm
					);

					lm.finish_current_line();
				}
			}
		}
	}
	void research_points::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto points = technologies::daily_research_points(ws, player);

			char16_t local_buf[16];

			put_value_in_buffer(local_buf, display_type::fp_two_places, points);

			ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				box,
				ui::xy_pair{ 0,0 },
				fmt,
				lm
			);

			lm.finish_current_line();
		}
	}

	inline void make_research_points_tt(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; player) {
			auto id = player;
			if(!ws.w.nation_s.nations.is_valid_index(id))
				return;

			ui::unlimited_line_manager lm;
			ui::xy_pair cursor{ 0,0 };

			cursor = ui::add_linear_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::research_points], ui::tooltip_text_format,
				ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
			lm.increase_indent(1);

			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			auto pop_by_type = &(ws.w.nation_s.nation_demographics.get_row(id)[population::to_demo_tag(ws, population::pop_type_tag(0))]);

			if(total_pop != 0) {
				for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
					population::pop_type_tag this_tag(static_cast<population::pop_type_tag::value_base_t>(i));
					auto& pt = ws.s.population_m.pop_types[this_tag];
					if(pt.research_points != 0 && pt.research_optimum != 0.0f) {
						auto points_from_type = float(pt.research_points) * std::min(1.0f, (float(pop_by_type[i]) / float(total_pop)) / ws.s.population_m.pop_types[this_tag].research_optimum);

						char16_t local_buf[32];

						if(points_from_type < 0.0f) {
							local_buf[0] = u'-';
							put_pos_value_in_buffer(local_buf + 1, display_type::fp_two_places, -points_from_type);
							cursor = ui::text_chunk_to_instances(
								ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf), tw, cursor,
								ui::text_format{ ui::text_color::red, ui::tooltip_text_format.font_handle, ui::tooltip_text_format.font_size }, lm);
						} else {
							local_buf[0] = u'+';
							put_pos_value_in_buffer(local_buf + 1, display_type::fp_two_places, points_from_type);
							cursor = ui::text_chunk_to_instances(
								ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf), tw, cursor,
								ui::text_format{ ui::text_color::green, ui::tooltip_text_format.font_handle, ui::tooltip_text_format.font_size }, lm);
						}

						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
						cursor = ui::add_linear_text(cursor, ws.s.population_m.pop_types[this_tag].name, ui::tooltip_text_format, ws.s.gui_m,
							ws.w.gui_m, tw, lm);

						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
						put_value_in_buffer(local_buf, display_type::percent_fp_one_place, float(pop_by_type[i]) / float(total_pop));
						cursor = ui::text_chunk_to_instances(
							ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf), tw, cursor,
							ui::tooltip_text_format, lm);

						cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
						cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m,
							vector_backed_string<char16_t>(u"("),
							tw, cursor, ui::tooltip_text_format, lm);

						put_value_in_buffer(local_buf, display_type::percent_fp_one_place, ws.s.population_m.pop_types[this_tag].research_optimum);
						text_data::replacement repl(text_data::value_type::opt, vector_backed_string<char16_t>(local_buf), [](ui::tagged_gui_object) {});

						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::optimal_is], ui::tooltip_text_format, ws.s.gui_m,
							ws.w.gui_m, tw, lm, &repl, 1);

						cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m,
							vector_backed_string<char16_t>(u")"),
							tw, cursor, ui::tooltip_text_format, lm);

						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
						lm.finish_current_line();
					}
				}
			}

			cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, player, modifiers::national_offsets::research_points);

			lm.decrease_indent(1);

			cursor = ui::add_linear_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::research_points_modifier], ui::tooltip_text_format,
				ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
			lm.increase_indent(1);

			cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, player, modifiers::national_offsets::research_points_modifier);
		}
	}

	void research_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		make_research_points_tt(ws, tw);
	}

	void research_points::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		make_research_points_tt(ws, tw);
	}
	void party_icon::update(ui::tinted_icon<party_icon>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto ideo = ws.w.nation_s.nations.get<nation::ruling_ideology>(player); is_valid_index(ideo)) {
				auto ideology_color = ws.s.ideologies_m.ideology_container[ideo].color;
				self.set_color(ws.w.gui_m, float(ideology_color.r) / 255.0f, float(ideology_color.g) / 255.0f, float(ideology_color.b) / 255.0f);
			}
		}
	}
	void party_name_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto rp = ws.w.nation_s.nations.get<nation::ruling_party>(player); is_valid_index(rp)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.governments_m.parties[rp].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}
	void suppression_points_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {

			char16_t local_buf[16];

			put_value_in_buffer(local_buf, display_type::exact_integer, ws.w.nation_s.nations.get<nation::suppression_points>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}
	void infamy_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {

			char16_t local_buf[16];

			put_value_in_buffer(local_buf, display_type::fp_two_places, ws.w.nation_s.nations.get<nation::infamy>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}
	void reform_alert::button_function(ui::simple_button<reform_alert>&, world_state & ws) {
		ws.w.government_w.show_government_reforms_tab(ws.w.gui_m);
	}
	void reform_alert::update(ui::simple_button<reform_alert>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void reform_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::cannot_reform], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void decision_alert::button_function(ui::simple_button<decision_alert>&, world_state & ws) {
		ws.w.government_w.show_government_decisions_tab(ws.w.gui_m);
	}
	void decision_alert::update(ui::simple_button<decision_alert>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void decision_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_decisions], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void rebels_alert::button_function(ui::simple_button<rebels_alert>&, world_state & ws) {
		ws.w.government_w.show_government_movements_tab(ws.w.gui_m);
	}
	void rebels_alert::update(ui::simple_button<rebels_alert>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void rebels_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_rebels], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void election_alert::update(ui::dynamic_icon<election_alert>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(ws.w.nation_s.nations.get<nation::is_holding_election>(player))
				self.set_frame(ws.w.gui_m, 0ui32);
			else
				self.set_frame(ws.w.gui_m, 1ui32);
		}
	}
	void election_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; player) {
			if(ws.w.nation_s.nations.get<nation::is_holding_election>(player)) {
				ui::unlimited_line_manager lm;

				char16_t formatted_date[64];
				u16_format_date(formatted_date, governments::election_end_date(ws, player));
				text_data::replacement repl(text_data::value_type::date, vector_backed_string<char16_t>(formatted_date), [](ui::tagged_gui_object){});
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::holding_election],
					ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &repl, 1);
			} else {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_election], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
			}
		}
	}
	void topbar_total_population_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buf[16];

				put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nation_demographics.get(id, population::total_population_tag));

				ui::text_chunk_to_instances(
					ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
					box, ui::xy_pair{ 0,0 }, fmt, lm
				);
				lm.finish_current_line();
			}
		}
	}
	void topbar_national_focus_count::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto max_focuses = modifiers::maximum_national_focuses(ws, player);
			auto used_focuses = int32_t(get_size(ws.w.nation_s.state_tag_arrays, ws.w.nation_s.nations.get<nation::national_focus_locations>(player)));

			char16_t local_buf[32];
			auto posa = put_value_in_buffer(local_buf, display_type::integer, max_focuses - used_focuses);
			*posa = u'/';
			put_value_in_buffer(posa + 1, display_type::integer, max_focuses);

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, ui::text_format{ used_focuses < max_focuses ? ui::text_color::red : fmt.color , fmt.font_handle , fmt.font_size }, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_militancy::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
				auto militancy = ws.w.nation_s.nation_demographics.get(id, population::militancy_demo_tag(ws));

				char16_t local_buf[16];
				put_value_in_buffer(local_buf, display_type::fp_two_places, total_pop != 0 ? 10.0f * float(militancy) / float(total_pop) : 0.0f);

				ui::text_chunk_to_instances(
					ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
					box, ui::xy_pair{ 0,0 }, ui::text_format{ ui::text_color::red , fmt.font_handle , fmt.font_size }, lm
				);
				lm.finish_current_line();
			}
		}
	}
	void topbar_consciousness::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
				auto militancy = ws.w.nation_s.nation_demographics.get(id, population::consciousness_demo_tag(ws));

				char16_t local_buf[16];
				put_value_in_buffer(local_buf, display_type::fp_two_places, total_pop != 0 ? 10.0f * float(militancy) / float(total_pop) : 0.0f);

				ui::text_chunk_to_instances(
					ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
					box, ui::xy_pair{ 0,0 }, ui::text_format{ ui::text_color::green , fmt.font_handle , fmt.font_size }, lm
				);
				lm.finish_current_line();
			}
		}
	}

	void war_against_flag_button::update(ui::masked_flag<war_against_flag_button>& self, world_state& ws) {
		self.set_displayed_flag(ws, tag);
	}
	ui::window_tag war_against_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["province_core"]);
	}
	void diplomatic_points_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::fp_one_place, ws.w.nation_s.nations.get<nation::diplomacy_points>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void crisis_alert::button_function(ui::simple_button<crisis_alert>&, world_state & ws) {
		ws.w.diplomacy_w.show_diplomacy_window_crisis(ws.w.gui_m);
	}
	void crisis_alert::update(ui::simple_button<crisis_alert>& self, world_state & ws) {
		if(ws.w.current_crisis.type != current_state::crisis_type::none) {
			if(ws.w.current_crisis.temperature >= 90.0f)
				self.set_frame(ws.w.gui_m, 1ui32);
			else
				self.set_frame(ws.w.gui_m, 0ui32);
		} else {
			self.set_frame(ws.w.gui_m, 2ui32);
		}
	}
	void crisis_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(ws.w.current_crisis.type != current_state::crisis_type::none) {
			ui::unlimited_line_manager lm;

			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.current_crisis.temperature);

			text_data::replacement repl(text_data::value_type::temperature, vector_backed_string<char16_t>(local_buf), [](ui::tagged_gui_object){});
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::has_crisis], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &repl, 1);
		} else {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_crisis_alert], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
		}
		
	}
	void losing_gp_alert::update(ui::dynamic_icon<losing_gp_alert>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void losing_gp_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; player) {
			if(nations::is_great_power(ws, player))
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::is_not_losing_gp], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
			else
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::is_not_gp], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
		}
	}
	void influence_alert::button_function(ui::simple_button<influence_alert>&, world_state &) {}
	void influence_alert::update(ui::simple_button<influence_alert>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void influence_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::cannot_influence], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void colonial_alert::button_function(ui::simple_button<colonial_alert>&, world_state &) {}
	void colonial_alert::update(ui::simple_button<colonial_alert>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 2ui32);
	}
	void colonial_alert::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_colonial_activity], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void at_peace_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(get_size(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(player)) == 0) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::at_peace], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}
	void army_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto army_count = get_size(ws.w.military_s.army_arrays, ws.w.nation_s.nations.get<nation::armies>(player));
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, army_count);

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void navy_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto fleet_count = get_size(ws.w.military_s.fleet_arrays, ws.w.nation_s.nations.get<nation::fleets>(player));
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, fleet_count);

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void manpower_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto solider_count = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, ws.s.population_m.soldier));

				char16_t local_buf[16];
				put_value_in_buffer(local_buf, display_type::integer, solider_count);

				ui::text_chunk_to_instances(
					ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
					box, ui::xy_pair{ 0,0 }, fmt, lm
				);
				lm.finish_current_line();
			}
		}
	}
	void leadership_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::leadership_points>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void outliner_button::button_function(ui::simple_button<outliner_button>&, world_state &) {}
	void outliner_button::update(ui::simple_button<outliner_button>&, world_state &) {}

	void topbar_overall_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::overall_rank>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_prestige_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::prestige_rank>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_military_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::military_rank>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_industrial_rank::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::industrial_rank>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_prestige::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, nations::get_prestige(ws, player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_military_score::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::military_score>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_industrial_score::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::industrial_score>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_colonial_points::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, nations::get_colonial_points(ws, player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);
			lm.finish_current_line();
		}
	}
	void topbar_country_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(player), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		}
	}
	void pause_button::button_function(ui::simple_button<pause_button>&, world_state & ws) {
		ws.w.toggle_pause();
	}
	void increase_speed_button::button_function(ui::simple_button<increase_speed_button>&, world_state & ws) {
		ws.w.increase_speed();
	}
	void decrease_speed_button::button_function(ui::simple_button<decrease_speed_button>&, world_state & ws) {
		ws.w.decrease_speed();
	}
	void speed_indicator_button::update(ui::simple_button<speed_indicator_button>& self, world_state & ws) {
		self.set_enabled(!ws.w.force_paused);

		if(ws.w.paused || ws.w.force_paused)
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, uint32_t(ws.w.speed));
	}
	void topbar_date::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto ymd = tag_to_date(ws.w.current_date).year_month_day();

		auto cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::month_1 + ymd.month - 1], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);

		char16_t local_buf[16];
		put_value_in_buffer(local_buf, display_type::integer, int32_t(ymd.day));
		cursor = ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
			box, cursor, fmt, lm
		);
		cursor = ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u", "),
			box, cursor, fmt, lm
		);

		put_value_in_buffer(local_buf, display_type::integer, int32_t(ymd.year));
		cursor = ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
			box, cursor, fmt, lm
		);

		lm.finish_current_line();
	}
	topbar::topbar() : win(std::make_unique<topbar_t>()) {}
	topbar::~topbar() {}
	void topbar::init_topbar(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["topbar"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void topbar::update_topbar(ui::gui_manager & gui_m) {
		if(win->associated_object)
			ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void topbar::resize_topbar(ui::gui_manager & gui_m) {
		if(win->associated_object)
			win->associated_object->size.x = int16_t(gui_m.width());
	}
}
