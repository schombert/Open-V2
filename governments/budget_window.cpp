#include "common\\common.h"
#include "budget_window.hpp"
#include "nations\\nations_functions.hpp"

namespace governments {
	budget_window::budget_window() : win(std::make_unique<budget_window_t>()) {}
	budget_window::~budget_window() {}
	void budget_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void budget_window::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_budget"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void budget_window::update(ui::gui_manager & gui_m) {
		if((win->associated_object->flags.load(std::memory_order_acquire) & (ui::gui_object::visible | ui::gui_object::visible_after_update)) != 0)
			ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void budget_window::show(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m);
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void bw_close_button::button_function(ui::simple_button<bw_close_button>&, world_state & ws) {
		ws.w.budget_w.hide(ws.w.gui_m);
	}
	void hidden_button::on_create(ui::simple_button<hidden_button>& b, world_state & ws) {
		b.set_visibility(ws.w.gui_m, false);
	}
	void hidden_text::on_create(ui::fixed_text<hidden_text>& b, world_state &) {
		ui::hide(*b.associated_object);
	}
	void poor_tax_scrollbar::on_position(world_state& ws, ui::scrollbar<poor_tax_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::poor_tax, int8_t(pos));
		}
	}
	void poor_tax_scrollbar::update(ui::scrollbar<poor_tax_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tax]), player->modifier_values[modifiers::national_offsets::max_tax] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tax]) : 100);
			sb.update_position(player->poor_tax);
		}
	}
	void middle_tax_scrollbar::on_position(world_state& ws, ui::scrollbar<middle_tax_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::middle_tax, int8_t(pos));
		}
	}
	void middle_tax_scrollbar::update(ui::scrollbar<middle_tax_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tax]), player->modifier_values[modifiers::national_offsets::max_tax] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tax]) : 100);
			sb.update_position(player->middle_tax);
		}
	}
	void rich_tax_scrollbar::on_position(world_state& ws, ui::scrollbar<rich_tax_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::rich_tax, int8_t(pos));
		}
	}
	void rich_tax_scrollbar::update(ui::scrollbar<rich_tax_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tax]), player->modifier_values[modifiers::national_offsets::max_tax] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tax]) : 100);
			sb.update_position(player->rich_tax);
		}
	}
	void land_spending_scrollbar::on_position(world_state& ws, ui::scrollbar<land_spending_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::army_stockpile_spending, int8_t(pos));
		}
	}
	void land_spending_scrollbar::update(ui::scrollbar<land_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->army_stockpile_spending);
		}
	}
	void naval_spending_scrollbar::on_position(world_state& ws, ui::scrollbar<naval_spending_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::navy_stockpile_spending, int8_t(pos));
		}
	}
	void naval_spending_scrollbar::update(ui::scrollbar<naval_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->navy_stockpile_spending);
		}
	}
	void projects_spending_scrollbar::on_position(world_state& ws, ui::scrollbar<projects_spending_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::projects_stockpile_spending, int8_t(pos));
		}
	}
	void projects_spending_scrollbar::update(ui::scrollbar<projects_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->projects_stockpile_spending);
		}
	}
	void administrative_pay_scrollbar::on_position(world_state& ws, ui::scrollbar<administrative_pay_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::administrative_spending, int8_t(pos));
		}
	}
	void administrative_pay_scrollbar::update(ui::scrollbar<administrative_pay_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->administrative_spending);
		}
	}
	void education_pay_scrollbar::on_position(world_state& ws, ui::scrollbar<education_pay_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::education_spending, int8_t(pos));
		}
	}
	void education_pay_scrollbar::update(ui::scrollbar<education_pay_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.update_position(player->education_spending);
		}
	}
	void military_pay_scrollbar::on_position(world_state& ws, ui::scrollbar<military_pay_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::military_spending, int8_t(pos));
		}
	}
	void military_pay_scrollbar::update(ui::scrollbar<military_pay_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_military_spending]), player->modifier_values[modifiers::national_offsets::max_military_spending] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_military_spending]) : 100);
			sb.update_position(player->military_spending);
		}
	}
	void social_spending_scrollbar::on_position(world_state& ws, ui::scrollbar<social_spending_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::social_spending, int8_t(pos));
		}
	}
	void social_spending_scrollbar::update(ui::scrollbar<social_spending_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_social_spending]), player->modifier_values[modifiers::national_offsets::max_social_spending] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_social_spending]) : 100);
			sb.update_position(player->social_spending);
		}
	}
	void tarrif_setting_scrollbar::on_position(world_state& ws, ui::scrollbar<tarrif_setting_scrollbar>& sb, int32_t pos) {
		if(auto player = ws.w.local_player_nation; player) {
			ws.w.pending_commands.template add<commands::set_budget>(player->id, commands::set_budget_type::tarrifs, int8_t(pos));
		}
	}
	void tarrif_setting_scrollbar::update(ui::scrollbar<tarrif_setting_scrollbar>& sb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			sb.set_range(ws.w.gui_m, -100, 100);
			sb.set_limits(ws.w.gui_m, int32_t(100.0f * player->modifier_values[modifiers::national_offsets::min_tariff]), player->modifier_values[modifiers::national_offsets::max_tariff] > 0 ? int32_t(100.0f * player->modifier_values[modifiers::national_offsets::max_tariff]) : 100);
			sb.update_position(player->tarrifs);
		}
	}
	void poor_needs_pie_chart::update(ui::piechart<poor_needs_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto pid = player->id; ws.w.nation_s.nations.is_valid_index(pid)) {
				auto total_strata_pop = ws.w.nation_s.nation_demographics.get(pid, population::poor_population_demo_tag(ws));
				auto total_ln = ws.w.nation_s.nation_demographics.get(pid, population::poor_life_needs_demo_tag(ws));
				auto total_en = ws.w.nation_s.nation_demographics.get(pid, population::poor_everyday_needs_demo_tag(ws));
				auto total_lx = ws.w.nation_s.nation_demographics.get(pid, population::poor_luxury_needs_demo_tag(ws));

				if(total_strata_pop == 0) {
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_no_needs]),
						1.0f,
						graphics::color_rgb{ 255ui8, 50ui8, 50ui8 });
					return;
				}

				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_lx]),
					float(total_lx) / float(total_strata_pop),
					graphics::color_rgb{50ui8, 255ui8, 50ui8});
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_en]),
					float(total_en - total_lx) / float(total_strata_pop),
					graphics::color_rgb{ 50ui8, 50ui8, 255ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_ln]),
					float(total_ln - total_en) / float(total_strata_pop),
					graphics::color_rgb{ 220ui8, 220ui8, 70ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_no_needs]),
					float(total_strata_pop - total_ln) / float(total_strata_pop),
					graphics::color_rgb{ 255ui8, 50ui8, 50ui8 });
			}
		}
	}
	void middle_needs_pie_chart::update(ui::piechart<middle_needs_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto pid = player->id; ws.w.nation_s.nations.is_valid_index(pid)) {
				auto total_strata_pop = ws.w.nation_s.nation_demographics.get(pid, population::middle_population_demo_tag(ws));
				auto total_ln = ws.w.nation_s.nation_demographics.get(pid, population::middle_life_needs_demo_tag(ws));
				auto total_en = ws.w.nation_s.nation_demographics.get(pid, population::middle_everyday_needs_demo_tag(ws));
				auto total_lx = ws.w.nation_s.nation_demographics.get(pid, population::middle_luxury_needs_demo_tag(ws));

				if(total_strata_pop == 0) {
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_no_needs]),
						1.0f,
						graphics::color_rgb{ 255ui8, 50ui8, 50ui8 });
					return;
				}

				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_lx]),
					float(total_lx) / float(total_strata_pop),
					graphics::color_rgb{ 50ui8, 255ui8, 50ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_en]),
					float(total_en - total_lx) / float(total_strata_pop),
					graphics::color_rgb{ 50ui8, 50ui8, 255ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_ln]),
					float(total_ln - total_en) / float(total_strata_pop),
					graphics::color_rgb{ 220ui8, 220ui8, 70ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_no_needs]),
					float(total_strata_pop - total_ln) / float(total_strata_pop),
					graphics::color_rgb{ 255ui8, 50ui8, 50ui8 });
			}
		}
	}
	void rich_needs_pie_chart::update(ui::piechart<rich_needs_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto pid = player->id; ws.w.nation_s.nations.is_valid_index(pid)) {
				auto total_strata_pop = ws.w.nation_s.nation_demographics.get(pid, population::rich_population_demo_tag(ws));
				auto total_ln = ws.w.nation_s.nation_demographics.get(pid, population::rich_life_needs_demo_tag(ws));
				auto total_en = ws.w.nation_s.nation_demographics.get(pid, population::rich_everyday_needs_demo_tag(ws));
				auto total_lx = ws.w.nation_s.nation_demographics.get(pid, population::rich_luxury_needs_demo_tag(ws));

				if(total_strata_pop == 0) {
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_no_needs]),
						1.0f,
						graphics::color_rgb{ 255ui8, 50ui8, 50ui8 });
					return;
				}

				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_lx]),
					float(total_lx) / float(total_strata_pop),
					graphics::color_rgb{ 50ui8, 255ui8, 50ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_en]),
					float(total_en - total_lx) / float(total_strata_pop),
					graphics::color_rgb{ 50ui8, 50ui8, 255ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_ln]),
					float(total_ln - total_en) / float(total_strata_pop),
					graphics::color_rgb{ 220ui8, 220ui8, 70ui8 });
				pie.add_entry(
					ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::pie_no_needs]),
					float(total_strata_pop - total_ln) / float(total_strata_pop),
					graphics::color_rgb{ 255ui8, 50ui8, 50ui8 });
			}
		}
	}
	ui::window_tag poor_pops_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_listitem"]);
	}
	ui::window_tag middle_pops_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_listitem"]);
	}
	ui::window_tag rich_pops_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_listitem"]);
	}
	ui::window_tag education_pops_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_listitem_small"]);
	}
	ui::window_tag administation_pops_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_listitem_small"]);
	}
	ui::window_tag military_pops_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_listitem_small"]);
	}
	void pop_button::update(ui::simple_button<pop_button>& ico, world_state & ws) {
		ico.set_frame(ws.w.gui_m, ws.s.population_m.pop_types[type].sprite - 1);
	}
	void pop_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; player) {
			float total_pop = 0.0f;
			float none_qnty = 0.0f;
			float ln_qnty = 0.0f;
			float en_qnty = 0.0f;
			float lx_qnty = 0.0f;
			
			nations::for_each_pop(ws, *player, [&ws, &total_pop, &none_qnty, &ln_qnty, &en_qnty, &lx_qnty, t = this->type](population::pop_tag p) {
				if(auto id = p; ws.w.population_s.pops.get<pop::type>(p) == t && ws.w.population_s.pops.is_valid_index(id)) {
					auto sz = float(ws.w.population_s.pop_demographics.get(id, population::total_population_tag));
					total_pop += sz;
					auto needs_sat = ws.w.population_s.pops.get<pop::needs_satisfaction>(p);
					if(needs_sat <= 1.0f) {
						ln_qnty += needs_sat * sz;
						none_qnty += (1.0f - needs_sat) * sz;
					} else {
						if(needs_sat <= 2.0f) {
							en_qnty += (needs_sat - 1.0f) * sz;
							ln_qnty += (1.0f - (needs_sat - 1.0f)) * sz;
						} else {
							if(needs_sat < 3.0f) {
								lx_qnty += (needs_sat - 2.0f) * sz;
								en_qnty += (1.0f - (needs_sat - 2.0f)) * sz;
							} else {
								lx_qnty += sz;
							}
						}
					}
				}
			});

			ui::xy_pair cursor{ 0,0 };
			cursor = ui::add_linear_text(cursor, ws.s.population_m.pop_types[type].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);

			if(total_pop == 0) {
				ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::no_pops_of_type], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
			} else {
				char16_t local_buffer[16];

				put_value_in_buffer(local_buffer, display_type::percent, none_qnty / total_pop);
				cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);
				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pie_no_needs], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);

				put_value_in_buffer(local_buffer, display_type::percent, ln_qnty / total_pop);
				cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);
				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pie_ln], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);

				put_value_in_buffer(local_buffer, display_type::percent, en_qnty / total_pop);
				cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);
				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pie_en], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);

				put_value_in_buffer(local_buffer, display_type::percent, lx_qnty / total_pop);
				cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, cursor, ui::tooltip_text_format);
				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pie_lx], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			}
		}
	}

	void warning_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::bankrupcy_warning], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void expenses_pie_chart::update(ui::piechart<expenses_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto e_amount = economy::education_spending_amount(ws, *player);
			auto m_amount = economy::military_spending_amount(ws, *player);
			auto s_amount = economy::social_spending_amount(ws, *player);
			auto a_amount = economy::administrative_spending_amount(ws, *player);
			auto tariff_costs = std::max(-1.0f * economy::project_player_tarrif_income(ws, float(player->tarrifs) / 100.0f), float(0));
			auto interest = economy::calculate_daily_debt_payment(ws, *player) / 2.0f;
			auto b_amount = economy::daily_national_building_cost(ws, *player) * float(player->projects_stockpile_spending) / 100.0f;

			auto total = e_amount + m_amount + s_amount + a_amount + tariff_costs + interest + b_amount;

			if(total <= 0)
				return;

			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::national_stockpile]),
				b_amount / total,
				graphics::color_rgb{ 30ui8, 155ui8, 250ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::edu_spending]),
				e_amount / total,
				graphics::color_rgb{ 30ui8, 155ui8, 30ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::admin_spending]),
				a_amount / total,
				graphics::color_rgb{ 155ui8, 155ui8, 30ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::social_spending]),
				s_amount / total,
				graphics::color_rgb{ 35ui8, 155ui8, 150ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::mil_spending]),
				m_amount / total,
				graphics::color_rgb{ 155ui8, 35ui8, 150ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::tariffs]),
				tariff_costs / total,
				graphics::color_rgb{ 255ui8, 155ui8, 55ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::interest]),
				interest / total,
				graphics::color_rgb{ 35ui8, 35ui8, 150ui8 });
		}
	}

	void incomes_pie_chart::update(ui::piechart<incomes_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto ptax_income = ws.w.local_player_data.collected_poor_tax * float(player->poor_tax) / 100.0f;
			auto mtax_income = ws.w.local_player_data.collected_middle_tax * float(player->middle_tax) / 100.0f;
			auto rtax_income = ws.w.local_player_data.collected_rich_tax * float(player->rich_tax) / 100.0f;
			economy::money_qnty_type tariff_income = std::max(economy::project_player_tarrif_income(ws, float(player->tarrifs) / 100.0f), float(0));
			auto total = ptax_income + mtax_income + rtax_income + tariff_income;
			
			if(total <= 0)
				return;

			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::b_poor_tax]),
				ptax_income / total,
				graphics::color_rgb{ 50ui8, 255ui8, 150ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::b_middle_tax]),
				mtax_income / total,
				graphics::color_rgb{ 150ui8, 255ui8, 50ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::b_rich_tax]),
				rtax_income / total,
				graphics::color_rgb{ 50ui8, 155ui8, 255ui8 });
			pie.add_entry(
				ws.w.gui_m,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::tariffs]),
				tariff_income / total,
				graphics::color_rgb{ 255ui8, 155ui8, 55ui8 });
		}
		
	}
}
