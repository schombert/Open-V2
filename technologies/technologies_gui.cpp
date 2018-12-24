#include "common\\common.h"
#include "technologies_gui.hpp"
#include "world_state\\world_state.h"
#include "technologies\\technologies_functions.h"
#include "military\\military_functions.h"

namespace technologies {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.technologies_w.hide_technology_window(ws.w.gui_m);
	}

	void tech_school_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto ts = ws.w.nation_s.nations.get<nation::tech_school>(player);
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
			if(auto tech = ws.w.nation_s.nations.get<nation::current_research>(player); is_valid_index(tech)) {
				if(auto total_points = effective_tech_cost(tech, ws, player); total_points != 0.0f) {
					bar.set_fraction(std::min(1.0f, float(ws.w.nation_s.nations.get<nation::research_points>(player)) / total_points));
					return;
				}
			}
		}
		bar.set_fraction(0.0f);
	}
	void research_name_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
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
	void research_category_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto cr = ws.w.nation_s.nations.get<nation::current_research>(player); is_valid_index(cr)) {
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
		ws.w.technologies_w.techui_invention_sort = invention_sort::type;
		ws.w.technologies_w.show_technology_window(ws.w.gui_m);
	}
	void sort_possible_inventions_by_name_button::button_function(ui::simple_button<sort_possible_inventions_by_name_button>&, world_state & ws) {
		ws.w.technologies_w.techui_invention_sort = invention_sort::name;
		ws.w.technologies_w.show_technology_window(ws.w.gui_m);
	}
	void sort_possible_inventions_by_chance_button::button_function(ui::simple_button<sort_possible_inventions_by_chance_button>&, world_state & ws) {
		ws.w.technologies_w.techui_invention_sort = invention_sort::chance;
		ws.w.technologies_w.show_technology_window(ws.w.gui_m);
	}
	void invention_item_percent::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(invention)) {
			ui::unlimited_line_manager lm;
			modifiers::make_additive_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.technology_m.technologies_container[invention].ai_chance],
				ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format, player, triggers::const_parameter());
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

	template<typename value_type>
	ui::xy_pair display_value(value_type value, display_type d_type, bool positive_is_green, world_state& ws,
		ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

		char16_t local_buffer[32];
		ui::text_format local_fmt{ (value < value_type(0)) == positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size };
		if(value < value_type(0))
			local_buffer[0] = u'-';
		else
			local_buffer[0] = u'+';

		put_pos_value_in_buffer(local_buffer + 1, d_type, std::abs(value));
		cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer),
			container, cursor_in, local_fmt, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return cursor_in;
	}
	
	ui::xy_pair eplain_technology(tech_tag t, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in,
		ui::line_manager& lm, ui::text_format const& fmt) {

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
		if(tech.cost != 0) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tech_research_points], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

			char16_t local_buffer[32];
			put_value_in_buffer(local_buffer, display_type::exact_integer, tech.cost);
			cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer),
				container, cursor_in, fmt, lm);

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		if(tech.year != 0 || tech.cost != 0) {
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		if(tech.shared_prestige != 0.0f) {
			auto adj_value = tech.shared_prestige / float(ws.w.technology_s.discovery_count[t] + 1);
			if(adj_value >= 0.05f) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::shared_prestige_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = display_value(adj_value, display_type::fp_one_place, true, ws, container, cursor_in, lm, fmt);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}
		}

		bool added_item = false;
		for(uint32_t i = 0; i < tech_definition_size; ++i) {
			if(tech.attributes.values[i] != 0.0f) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.technology_m.tech_modifier_names[tech.attributes.offsets[i]], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = display_green_value(tech.attributes.values[i], tech_offset_display[tech.attributes.offsets[i]], ws, container, cursor_in, lm, fmt);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				added_item = true;
			}
		}
		if(is_valid_index(tech.modifier)) {
			cursor_in = modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[tech.modifier]);
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

		if(is_valid_index(tech.rebel_adjustment)) {
			auto org_row = ws.s.technology_m.rebel_org_gain.get_row(tech.rebel_adjustment);
			for(uint32_t i = 0; i < ws.s.population_m.rebel_types.size(); ++i) {
				population::rebel_type_tag tag(static_cast<population::rebel_type_tag::value_base_t>(i));
				if(org_row[tag] != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.population_m.rebel_types[tag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tech_rebel_org_gain], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(org_row[tag], display_type::percent, false, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}

		if(is_valid_index(tech.production_adjustment)) {
			//auto production_row = ws.s.technology_m.production_adjustments.get_row(tech.production_adjustment);
			for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
				economy::goods_tag gtag(static_cast<economy::goods_tag::value_base_t>(i));
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::rgo_size>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::rgo_size], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, true, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::rgo_goods_output>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::rgo_output_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, true, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::rgo_goods_throughput>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::rgo_throughput_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, true, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::factory_goods_input>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::factory_input_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, false, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::factory_goods_output>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::factory_output_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, true, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::factory_goods_throughput>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::factory_throughput_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, true, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::artisan_goods_input>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::artisan_input_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, false, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::artisan_goods_output>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::artisan_output_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, true, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(auto v = ws.s.technology_m.production_adjustments.get(
					tech.production_adjustment, economy_tag_to_production_adjustment<production_adjustment::artisan_goods_throughput>(gtag)); v != 0.0f) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[gtag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::artisan_throughput_tech], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = display_value(v, display_type::percent, true, ws, container, cursor_in, lm, fmt);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
			}
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}

		if(is_valid_index(tech.unit_adjustment)) {
			for(uint32_t i = 0; i < ws.s.military_m.unit_types_count; ++i) {
				military::unit_type_tag tag(static_cast<military::unit_type_tag::value_base_t>(i));
				auto& vector = ws.s.technology_m.unit_type_adjustments.get(tech.unit_adjustment, tag);

				for(uint32_t j = 0; j < military::unit_attribute::enabled; ++j) {
					if(vector[j] != 0) {
						cursor_in = ui::add_linear_text(cursor_in, ws.s.military_m.unit_types[tag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
						cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

						switch(j) {
							case 0:
								cursor_in = ui::add_linear_text(cursor_in,
									ws.s.fixed_ui_text[military::is_naval_unit(tag, ws) ? scenario::fixed_ui::unit_hull : scenario::fixed_ui::unit_defence],
									fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 1:
								cursor_in = ui::add_linear_text(cursor_in,
									ws.s.fixed_ui_text[military::is_naval_unit(tag, ws) ? scenario::fixed_ui::unit_gun_power : scenario::fixed_ui::unit_attack],
									fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 2:
								cursor_in = ui::add_linear_text(cursor_in,
									ws.s.fixed_ui_text[military::is_naval_unit(tag, ws) ? scenario::fixed_ui::unit_fire_range : scenario::fixed_ui::unit_reconnaissance],
									fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 3:
								cursor_in = ui::add_linear_text(cursor_in,
									ws.s.fixed_ui_text[military::is_naval_unit(tag, ws) ? scenario::fixed_ui::unit_torpedo_attack : scenario::fixed_ui::unit_support],
									fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 4:
								cursor_in = ui::add_linear_text(cursor_in,
									ws.s.fixed_ui_text[military::is_naval_unit(tag, ws) ? scenario::fixed_ui::unit_evasion : scenario::fixed_ui::unit_maneuver],
									fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 5:
								cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::unit_speed], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 6:
								cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::unit_organization], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 7:
								cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::unit_build_time], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 8:
								cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::unit_supply_consumption], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 9:
								cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::unit_strength], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 10:
								cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::unit_siege], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							case 11:
								cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::unit_discipline], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
								cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
								break;
							default:
								break;
						}

						
						cursor_in = display_value(vector[j],
							(j == military::unit_attribute::supply_consumption || j == military::unit_attribute::discipline) ? display_type::percent : (j <= 4 ? display_type::fp_two_places : display_type::integer),
							j != military::unit_attribute::supply_consumption, ws, container, cursor_in, lm, fmt);

						if(j == military::unit_attribute::build_time)
							cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tx_day], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
						
						cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
						lm.finish_current_line();
					}
				}
				if(vector[military::unit_attribute::enabled] > 0) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.military_m.unit_types[tag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::enabled], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				} else if(vector[military::unit_attribute::enabled] < 0) {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.military_m.unit_types[tag].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::disabled], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
			}
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		/*
		triggers::trigger_tag allow;
		*/

		return cursor_in;
	}
	void invention_item_name::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		eplain_technology(invention, ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format);
	}
	void tech_picture::on_create(ui::dynamic_icon<tech_picture>& self, world_state & ws) {
		auto gi = ws.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(self.associated_object->type_dependant_handle.load(std::memory_order_acquire)));
		if(gi)
			default_image = gi->t;
	}
	void tech_picture::update(ui::dynamic_icon<tech_picture>& self, world_state & ws) {
		if(is_valid_index(ws.w.technologies_w.selected_technology)) {
			graphics::texture_tag picture = ws.s.technology_m.technologies_container[ws.w.technologies_w.selected_technology].picture;
			auto gi = ws.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(self.associated_object->type_dependant_handle.load(std::memory_order_acquire)));

			if(is_valid_index(picture)) {
				if(gi)
					gi->t = &(ws.s.gui_m.textures.retrieve_by_key(picture));
			} else {
				if(gi)
					gi->t = default_image;
			}
		}
	}
	void selected_tech_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(ws.w.technologies_w.selected_technology)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.technology_m.technologies_container[ws.w.technologies_w.selected_technology].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	void selected_tech_description::update(ui::tagged_gui_object box, ui::line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(ws.w.technologies_w.selected_technology)) {
			eplain_technology(ws.w.technologies_w.selected_technology, ws, box, ui::xy_pair{ 0,0 }, lm, fmt);
		}
	}
	void selected_tech_cost::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(ws.w.technologies_w.selected_technology)) {
			char16_t local_buffer[32];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.s.technology_m.technologies_container[ws.w.technologies_w.selected_technology].cost);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void selected_tech_year::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(ws.w.technologies_w.selected_technology) && ws.s.technology_m.technologies_container[ws.w.technologies_w.selected_technology].year != 0) {
			char16_t local_buffer[32];
			put_value_in_buffer(local_buffer, display_type::exact_integer, ws.s.technology_m.technologies_container[ws.w.technologies_w.selected_technology].year);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	void start_research_button::update(ui::simple_button<start_research_button>&, world_state &) {}
	void start_research_button::button_function(ui::simple_button<start_research_button>&, world_state &) {}

	ui::window_tag selected_tech_invention_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["invention_icon_window"]);
	}
	void folder_tab_button::button_function(ui::simple_button<folder_tab_button>&, world_state & ws) {
		ws.w.technologies_w.selected_tech_category = category;
		ws.w.technologies_w.show_technology_window(ws.w.gui_m);
	}
	void individual_tech_button::button_function(ui::simple_button<individual_tech_button>&, world_state & ws) {
		ws.w.technologies_w.selected_technology = tech;
		ws.w.technologies_w.show_technology_window(ws.w.gui_m);
	}
	void individual_tech_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tech)) {
			ui::unlimited_line_manager lm;
			eplain_technology(tech, ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format);
		}
	}
	technology_window::technology_window() : win(std::make_unique<tech_window_t>()) {}
	technology_window::~technology_window() {}
	void technology_window::show_technology_window(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void technology_window::hide_technology_window(ui::gui_manager & gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void technology_window::init_technology_winodw(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_technology"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
}