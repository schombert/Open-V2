#include "common\\common.h"
#include "technologies_gui.hpp"
#include "world_state\\world_state.h"
#include "technologies\\technologies_functions.h"

namespace technologies {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.hide_tech_window();
	}

	void tech_school_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto ts = player->tech_school;
			if(is_valid_index(ts)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.modifiers_m.national_modifiers[ts].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			} else {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::traditional_academic], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}
	void school_item_base::create_tooltip(ui::gui_object_tag self, world_state & ws, const ui::mouse_move &, ui::tagged_gui_object tw) {
		ui::text_format rg_format{ modifier < 0.0f ? ui::text_color::red : ui::text_color::green, ui::tooltip_text_format.font_handle, ui::tooltip_text_format.font_size};
		char16_t local_buffer[32];
		if(modifier < 0.0f)
			local_buffer[0] = u'-';
		else
			local_buffer[0] = u'+';
		put_pos_value_in_buffer(local_buffer + 1, display_type::percent, std::abs(modifier));
		auto cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), tw, ui::xy_pair{ 0,0 }, rg_format);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		ui::add_linear_text(cursor, modifier_name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	ui::window_tag school_modifiers_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["school_icon_window"]);
	}
	void research_progress_bar::update(ui::progress_bar<research_progress_bar>& bar, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto tech = player->current_research; is_valid_index(tech)) {
				if(auto total_points = effective_tech_cost(tech, ws, *player); total_points != 0.0f) {
					bar.set_fraction(std::min(1.0f, float(player->research_points) / total_points));
					return;
				}
			}
		}
		bar.set_fraction(0.0f);
	}
	void research_name_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
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
	void research_category_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto cr = player->current_research; is_valid_index(cr)) {
				auto tc = [cr, &ws](){
					for(auto& cat : ws.s.technology_m.technology_categories) {
						if(cat.type == ws.s.technology_m.technologies_container[cr].category)
							return cat.id;
					}
					return technologies::tech_category_tag();
				}();
				if(is_valid_index(tc)) {
					auto tsc = [tc, cr, &ws]() {
						for(auto& cat : ws.s.technology_m.technology_subcategories) {
							if(cat.parent == tc && std::find(std::begin(cat.member_techs), std::end(cat.member_techs), cr) != std::end(cat.member_techs))
								return cat.id;
						}
						return technologies::tech_subcategory_tag();
					}();
					auto cursor = ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.technology_m.technology_categories[tc].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					if(is_valid_index(tsc)) {
						cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u", "), box, cursor, fmt, lm);
						ui::add_linear_text(cursor, ws.s.technology_m.technology_subcategories[tsc].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					}
					lm.finish_current_line();
				}
			}
		}
	}
	void sort_possible_inventions_by_type_button::button_function(ui::simple_button<sort_possible_inventions_by_type_button>&, world_state & ws) {
		ws.w.techui_invention_sort = current_state::invention_sort::type;
		ws.w.show_tech_window();
	}
	void sort_possible_inventions_by_name_button::button_function(ui::simple_button<sort_possible_inventions_by_name_button>&, world_state & ws) {
		ws.w.techui_invention_sort = current_state::invention_sort::name;
		ws.w.show_tech_window();
	}
	void sort_possible_inventions_by_chance_button::button_function(ui::simple_button<sort_possible_inventions_by_chance_button>&, world_state & ws) {
		ws.w.techui_invention_sort = current_state::invention_sort::chance;
		ws.w.show_tech_window();
	}
	void invention_item_percent::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(invention)) {
			ui::unlimited_line_manager lm;
			modifiers::make_additive_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.technology_m.technologies_container[invention].ai_chance],
				ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format, player, nullptr, nullptr);
		}
	}
	ui::window_tag invention_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["invention_window"]);
	}

	namespace {
		static display_type tech_offset_display[] = {
			display_type::integer,//constexpr int32_t max_national_focus = 0;
			display_type::percent,//constexpr int32_t war_exhaustion = 1;
			display_type::percent,//constexpr int32_t supply_limit = 2;
			display_type::percent,//constexpr int32_t prestige = 3;
			display_type::integer,//constexpr int32_t combat_width = 4;
			display_type::integer,//constexpr int32_t dig_in_cap = 5;
			display_type::percent,//constexpr int32_t influence = 6;
			display_type::percent,//constexpr int32_t repair_rate = 7;
			display_type::percent,//constexpr int32_t reinforce_rate = 8;
			display_type::percent,//constexpr int32_t soldier_to_pop_loss = 9;
			display_type::integer,//constexpr int32_t regular_experience_level = 10;
			display_type::integer,//constexpr int32_t colonial_life_rating = 11;
			display_type::percent,//constexpr int32_t education_efficiency = 12;
			display_type::percent,//constexpr int32_t military_tactics = 13;
			display_type::percent,//constexpr int32_t seperatism = 14;
			display_type::percent,//constexpr int32_t land_attrition = 15;
			display_type::percent,//constexpr int32_t naval_attrition = 16;
			display_type::percent,//constexpr int32_t supply_range = 17;
			display_type::percent,//constexpr int32_t plurality = 18;
			display_type::percent,//constexpr int32_t factory_cost = 19;
			display_type::integer,//constexpr int32_t permanent_prestige = 20;
			display_type::percent,//constexpr int32_t colonial_prestige = 21;
			display_type::integer,//constexpr int32_t max_fort = 22;
			display_type::integer,//constexpr int32_t max_naval_base = 23;
			display_type::integer,//constexpr int32_t max_railroad = 24;
			display_type::percent,//constexpr int32_t morale = 25;
			display_type::percent,//constexpr int32_t colonial_migration = 26;
			display_type::integer,//constexpr int32_t colonial_points = 27;
			display_type::percent,//constexpr int32_t cb_creation_speed = 28;
		};

		ui::xy_pair display_green_value(float value, display_type d_type, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			char16_t local_buffer[32];
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			if(value < float(0))
				local_buffer[0] = u'-';
			else
				local_buffer[0] = u'+';

			put_pos_value_in_buffer(local_buffer + 1, d_type, std::abs(value));
			cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer),
				container, cursor_in, local_fmt, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return cursor_in;
		}

	}

	
	ui::xy_pair eplain_technology(tech_tag t, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

		auto& tech = ws.s.technology_m.technologies_container[t];

		if(tech.year != 0) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tech_activation_year], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

			char16_t local_buffer[32];
			put_value_in_buffer(local_buffer, display_type::exact_integer, tech.year);
			cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer),
				container, cursor_in, fmt, lm);

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		{
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tech_research_points], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

			char16_t local_buffer[32];
			put_value_in_buffer(local_buffer, display_type::exact_integer, tech.cost);
			cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer),
				container, cursor_in, fmt, lm);

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}

		bool added_item = false;
		for(uint32_t i = 0; i < tech_offset::count; ++i) {
			if(tech.attributes[i] != 0.0f) {
				cursor_in = display_green_value(tech.attributes[i], tech_offset_display[i], ws, container, cursor_in, lm, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.technology_m.tech_modifier_names[i], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				added_item = true;
			}
		}
		if(is_valid_index(tech.modifier)) {
			modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[tech.modifier]);
			added_item = true;
		}

		if(added_item) {
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = false;
		}

		if(is_valid_index(tech.enable_crime)) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::enable_crime_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.modifiers_m.provincial_modifiers[tech.enable_crime].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = true;
		}
		if(is_valid_index(tech.activate_factory)) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::activate_goods], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[ws.s.economy_m.factory_types[tech.activate_factory].output_good].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = true;
		}
		if((tech.flags & technology::activate_fort) != 0) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::activate_goods], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::fort], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = true;
		}
		if((tech.flags & technology::activate_naval_base) != 0) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::activate_goods], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::naval_base], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = true;
		}
		if((tech.flags & technology::activate_railroad) != 0) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::activate_goods], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::railroad], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = true;
		}
		if((tech.flags & technology::gas_attack) != 0) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::activate_goods], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::gas_attack_cap], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = true;
		}
		if((tech.flags & technology::gas_defence) != 0) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::activate_goods], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::gas_defence_cap], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = true;
		}


		if(added_item) {
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			added_item = false;
		}

		/*

		float shared_prestige = 0.0f;

		rebel_adjustment_tag rebel_adjustment;
		production_adjustment_tag production_adjustment;
		unit_adjustment_tag unit_adjustment;

		triggers::trigger_tag allow;

		uint8_t flags = 0ui8;
		*/

	}
}