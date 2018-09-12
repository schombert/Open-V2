#include "common\\common.h"
#include "topbar.hpp"
#include "nations\\nations_functions.hpp"

namespace current_state {
	void topbar_bg::on_create(ui::simple_button<topbar_bg>& bttn, world_state &) {
		bttn.associated_object->flags.fetch_or(ui::gui_object::force_transparency_check);
	}

	void player_flag::button_function(ui::masked_flag<player_flag>&, world_state&) {}
	void player_flag::update(ui::masked_flag<player_flag>& self, world_state& ws) {
		auto player = ws.w.local_player_nation;
		if(player)
			self.set_displayed_flag(ws, player->tag);
	}

	void player_flag_frame::update(ui::dynamic_transparent_icon<player_flag_frame>& ico, world_state& ws) {
		auto player = ws.w.local_player_nation;
		if(player) {
			if((player->flags & nations::nation::is_civilized) == 0)
				ico.set_frame(ws.w.gui_m, 3ui32);
			else if(nations::is_great_power(ws, *player))
				ico.set_frame(ws.w.gui_m, 0ui32);
			else if(player->overall_rank <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
				ico.set_frame(ws.w.gui_m, 1ui32);
			else
				ico.set_frame(ws.w.gui_m, 2ui32);
		}
	}
	void production_button::button_function(ui::simple_button<production_button>&, world_state &) {
	}
	void production_button::update(ui::simple_button<production_button>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void budget_button::button_function(ui::simple_button<budget_button>&, world_state&) {}
	void budget_button::update(ui::simple_button<budget_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, 1ui32);
	}
	void tech_button::button_function(ui::simple_button<tech_button>&, world_state&) {};
	void tech_button::update(ui::simple_button<tech_button>& self, world_state& ws) {
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
			auto id = player->id;
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
			auto id = player->id;
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
				nations::for_each_state(ws, *player, [&ws, &cursor, tw, &lm](nations::state_instance& si) {
					if(auto si_id = si.id; ws.w.nation_s.states.is_valid_index(si_id)) {
						for(auto w : ws.s.population_m.factory_workers) {
							if(ws.w.nation_s.state_demographics.get(si_id, population::to_employment_demo_tag(ws, w)) <
								ws.w.nation_s.state_demographics.get(si_id, population::to_demo_tag(ws, w))) {
								cursor = ui::add_linear_text(cursor, si.name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
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
			most_extreme = std::max(most_extreme, std::abs(ws.w.local_player_income_history.values[i]));
		}
		for(int32_t i = 0; i < 32; ++i) {
			adjusted_values[i] = (ws.w.local_player_income_history.values[(int32_t(to_index(ws.w.current_date)) - (31 - i)) % 32] * 0.5f)  / most_extreme + 0.5f;
		}

		self.set_values(ws.w.gui_m, adjusted_values);
	}
	void budget_linechart::create_tooltip(world_state & ws, ui::tagged_gui_object tw, int32_t x) {
		ui::xy_pair cursor{ 0,0 };
		char16_t local_buf[64];

		auto income_value = ws.w.local_player_income_history.values[(int32_t(to_index(ws.w.current_date)) - (31 - x)) % 32];

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
			auto pid = player->id;
			if(ws.w.nation_s.nations.is_valid_index(pid)) {
				char16_t local_buf[64];

				auto end_pos = put_pos_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.national_stockpiles.get(pid, economy::money_good));
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
			auto cr = player->current_research;
			if(is_valid_index(cr)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.technology_m.technologies_container[cr].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			} else {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_research], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			}
			lm.finish_current_line();
		}
	}
	void literacy_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto pid = player->id;
			if(ws.w.nation_s.nations.is_valid_index(pid)) {
				auto total_pop = ws.w.nation_s.nation_demographics.get(pid, population::total_population_tag);
				auto literacy = ws.w.nation_s.nation_demographics.get(pid, population::literacy_demo_tag(ws));
				if(total_pop != 0) {
					char16_t local_buf[16];

					put_pos_value_in_buffer(local_buf, display_type::percent, float(literacy) / float(total_pop));

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
}
