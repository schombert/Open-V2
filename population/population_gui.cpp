#include "common\\common.h"
#include "population_gui.hpp"
#include "world_state\\world_state.h"

namespace population {
	ui::xy_pair display_assimilation(ui::xy_pair cursor, ui::text_format const& fmt, world_state& ws, ui::tagged_gui_object tw, ui::line_manager &lm, pop_tag pop_id) {
		if(ws.w.population_s.pops.get<pop::is_accepted>(pop_id)) {
			return ui::add_text(cursor, scenario::fixed_ui::no_assimilation, fmt, ws, tw);
		} else {
			float const projected_amount = project_assimilation_amount(ws, pop_id);

			cursor = ui::add_text(cursor, scenario::fixed_ui::projected_assimilation, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
			cursor = ui:add_text(cursor, text_data::exact_integer{ int32_t(projected_amount) }, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
			cursor = ui::add_text(cursor, scenario::fixed_ui::assimilation_factors, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);

			cursor = ui::display_colored_factor(cursor, ws.s.modifiers_m.global_defines.assimilation_scale, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
			cursor = ui::add_text(cursor, scenario::fixed_ui::assimilation_scale, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);

			cursor = ui::display_colored_factor(cursor,
				ws.w.province_s.modifier_values.get(modifiers::provincial_offsets::assimilation_rate, ws.w.population_s.pops.get<pop::location>(pop_id)) + 1.0f,
				fmt, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
			cursor = ui::add_text(cursor, ws.s.modifiers_m.province_offset_names[modifiers::provincial_offsets::assimilation_rate], fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
			lm.increase_indent(1);
			cursor = modifiers::explain_province_modifier(ws, tw, cursor, lm, fmt, ws.w.population_s.pops.get<pop::location>(pop_id), modifiers::provincial_offsets::assimilation_rate);
			lm.decrease_indent(1);

			cursor = ui::display_colored_factor(cursor,
				ws.w.province_s.modifier_values.get(modifiers::national_offsets::global_assimilation_rate, get_pop_owner(ws, pop_id)) + 1.0f,
				fmt, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
			cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::global_assimilation_rate], fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
			lm.increase_indent(1);
			cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, fmt, get_pop_owner(ws, pop_id), modifiers::national_offsets::global_assimilation_rate);
			lm.decrease_indent(1);

			cursor = modifiers::make_additive_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.population_m.assimilation_chance],
				ws, tw, cursor, lm, fmt, pop_id, pop_id, scenario::fixed_ui::factor);
			return cursor;
		}
	}


	ui::xy_pair display_conversion(ui::xy_pair cursor, ui::text_format const& fmt, world_state& ws, ui::tagged_gui_object tw, ui::line_manager &lm, pop_tag pop_id) {
		if(!ws.w.population_s.pops.get<pop::is_accepted>(pop_id)) {
			return ui::add_text(cursor, scenario::fixed_ui::no_conversion, fmt, ws, tw);
		} else if(ws.w.population_s.pops.get<pop::religion>(pop_id) == ws.w.nation_s.nations.get<nation::national_religion>(get_pop_owner(ws, pop_id))) {
			return ui::add_text(cursor, scenario::fixed_ui::no_conversion, fmt, ws, tw);
		} else {
			float const projected_amount = project_assimilation_amount(ws, pop_id);

			cursor = ui::add_text(cursor, scenario::fixed_ui::projected_conversion, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
			cursor = ui:add_text(cursor, text_data::exact_integer{ int32_t(projected_amount) }, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);
			cursor = ui::add_text(cursor, scenario::fixed_ui::conversion_factors, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);

			cursor = ui::display_colored_factor(cursor, ws.s.modifiers_m.global_defines.conversion_scale, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
			cursor = ui::add_text(cursor, scenario::fixed_ui::conversion_scale, fmt, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt, lm);

			cursor = modifiers::make_multiplicative_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.population_m.conversion_chance],
				ws, tw, cursor, lm, fmt, pop_id, pop_id, scenario::fixed_ui::factor);
			return cursor;
		}
	}

	bool population_window_base::on_keydown(ui::gui_object_tag, world_state& ws, const ui::key_down & k) {
		if(k.keycode == virtual_key::ESCAPE) {
			ws.w.population_w.hide_population_window(ws.w.gui_m);
			return true;
		} else {
			return false;
		}
	}

	
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.population_w.hide_population_window(ws.w.gui_m);
	}

	void popfilter_all_button::button_function(ui::simple_button<popfilter_all_button>&, world_state & ws) {
		for(auto& v : ws.w.population_w.filtered_pop_types)
			v = 1ui8;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void popfilter_none_button::button_function(ui::simple_button<popfilter_none_button>&, world_state & ws) {
		for(auto& v : ws.w.population_w.filtered_pop_types)
			v = 0ui8;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_size_button::button_function(ui::simple_button<sortby_size_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::size;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_type_button::button_function(ui::simple_button<sortby_type_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::type;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_nationality_button::button_function(ui::simple_button<sortby_nationality_button>&, world_state &ws) {
		ws.w.population_w.sort_type = population_sort::culture;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_religion_button::button_function(ui::simple_button<sortby_religion_button>&, world_state &ws) {
		ws.w.population_w.sort_type = population_sort::religion;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_location_button::button_function(ui::simple_button<sortby_location_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::location;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_mil_button::button_function(ui::simple_button<sortby_mil_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::militancy;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_con_button::button_function(ui::simple_button<sortby_con_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::consciousness;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_ideology_button::button_function(ui::simple_button<sortby_ideology_button>&, world_state &ws) {
		ws.w.population_w.sort_type = population_sort::ideoology;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_issues_button::button_function(ui::simple_button<sortby_issues_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::issues;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_unemployment_button::button_function(ui::simple_button<sortby_unemployment_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::unemployment;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_cash_button::button_function(ui::simple_button<sortby_cash_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::cash;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_subsistence_button::button_function(ui::simple_button<sortby_subsistence_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::life_needs;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_eve_button::button_function(ui::simple_button<sortby_eve_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::everyday_needs;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_luxury_button::button_function(ui::simple_button<sortby_luxury_button>&, world_state & ws) {
		ws.w.population_w.sort_type = population_sort::luxury_needs;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_revoltrisk_button::button_function(ui::simple_button<sortby_revoltrisk_button>&, world_state &ws) {
		ws.w.population_w.sort_type = population_sort::revolt_risk;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_change_button::button_function(ui::simple_button<sortby_change_button>&, world_state &ws) {
		ws.w.population_w.sort_type = population_sort::size_change;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void sortby_literacy_button::button_function(ui::simple_button<sortby_literacy_button>&, world_state &ws) {
		ws.w.population_w.sort_type = population_sort::literacy;
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}

	void workforce_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::workforce], fmt, ws, box, lm);
		lm.finish_current_line();
	}

	void religion_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::religion], fmt, ws, box, lm);
		lm.finish_current_line();
	}

	void culture_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::nationality], fmt, ws, box, lm);
		lm.finish_current_line();
	}

	void ideology_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::ideology], fmt, ws, box, lm);
		lm.finish_current_line();
	}

	void issues_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::dominant_issues], fmt, ws, box, lm);
		lm.finish_current_line();
	}

	void electorate_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::electorate], fmt, ws, box, lm);
		lm.finish_current_line();
	}

	void workforce_chart::update(ui::piechart<workforce_chart>& pie, world_state & ws) {
		float* sums_out = (float*)_alloca(sizeof(float) * ws.s.population_m.count_poptypes);
		float total_size = 0;
		std::fill_n(sums_out, ws.s.population_m.count_poptypes, 0.0f);

		if(ws.w.population_w.display_type == population_display::nation) {
			total_size = sum_filtered_demo_data<population::pop_type_tag>(ws, sums_out, ws.w.population_w.population_for_nation);
		} else if(ws.w.population_w.display_type == population_display::state) {
			total_size = sum_filtered_demo_data<population::pop_type_tag>(ws, sums_out, ws.w.population_w.population_for_state);
		} else if(ws.w.population_w.display_type == population_display::province) {
			total_size = sum_filtered_demo_data<population::pop_type_tag>(ws, sums_out, ws.w.population_w.population_for_province);
		}

		if(total_size == 0)
			return;

		for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
			if(sums_out[i] != 0)
				pie.add_entry(
					ws.w.gui_m,
					ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size),
					ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color);
		}
	}

	void religion_chart::update(ui::piechart<religion_chart>& pie, world_state & ws) {
		float* sums_out = (float*)_alloca(sizeof(float) * ws.s.culture_m.count_religions);
		float total_size = 0;
		std::fill_n(sums_out, ws.s.culture_m.count_religions, 0.0f);

		if(ws.w.population_w.display_type == population_display::nation) {
			total_size = sum_filtered_demo_data<cultures::religion_tag>(ws, sums_out, ws.w.population_w.population_for_nation);
		} else if(ws.w.population_w.display_type == population_display::state) {
			total_size = sum_filtered_demo_data<cultures::religion_tag>(ws, sums_out, ws.w.population_w.population_for_state);
		} else if(ws.w.population_w.display_type == population_display::province) {
			total_size = sum_filtered_demo_data<cultures::religion_tag>(ws, sums_out, ws.w.population_w.population_for_province);
		}

		if(total_size == 0)
			return;

		for(uint32_t i = 0; i < ws.s.culture_m.count_religions; ++i) {
			if(sums_out[i] != 0)
				pie.add_entry(
					ws.w.gui_m,
					 ws.s.culture_m.religions[cultures::religion_tag(static_cast<cultures::religion_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size),
					ws.s.culture_m.religions[cultures::religion_tag(static_cast<cultures::religion_tag::value_base_t>(i))].color);
		}
	}

	void ideology_chart::update(ui::piechart<ideology_chart>& pie, world_state & ws) {
		float* sums_out = (float*)_alloca(sizeof(float) * ws.s.ideologies_m.ideologies_count);
		float total_size = 0;
		std::fill_n(sums_out, ws.s.ideologies_m.ideologies_count, 0.0f);

		if(ws.w.population_w.display_type == population_display::nation) {
			total_size = sum_filtered_demo_data<ideologies::ideology_tag>(ws, sums_out, ws.w.population_w.population_for_nation);
		} else if(ws.w.population_w.display_type == population_display::state) {
			total_size = sum_filtered_demo_data<ideologies::ideology_tag>(ws, sums_out, ws.w.population_w.population_for_state);
		} else if(ws.w.population_w.display_type == population_display::province) {
			total_size = sum_filtered_demo_data<ideologies::ideology_tag>(ws, sums_out, ws.w.population_w.population_for_province);
		}

		if(total_size == 0)
			return;

		for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
			if(sums_out[i] != 0)
				pie.add_entry(
					ws.w.gui_m,
					ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size),
					ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].color);
		}
	}

	void electorate_chart::update(ui::piechart<electorate_chart>& pie, world_state& ws) {
		auto const ideology_vsize = ve::to_vector_size(ws.s.ideologies_m.ideologies_count);
		float* const vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));
		ve::set_zero(ideology_vsize, tagged_array_view<float, ideologies::ideology_tag>(vote, ideology_vsize));

		if(ws.w.population_w.display_type == population_display::nation) {
			sum_pop_vote(ws, vote, ws.w.population_w.population_for_nation);
		} else if(ws.w.population_w.display_type == population_display::state) {
			sum_pop_vote(ws, vote, ws.w.population_w.population_for_state);
		} else if(ws.w.population_w.display_type == population_display::province) {
			sum_pop_vote(ws, vote, ws.w.population_w.population_for_province);
		}

		auto const total = ve::reduce(ws.s.ideologies_m.ideologies_count, tagged_array_view<float, ideologies::ideology_tag>(vote, ideology_vsize));
		if(auto const player = ws.w.local_player_nation; total > 0.0f && player) {
			for(uint32_t i = 0; i < int32_t(ws.s.ideologies_m.ideologies_count); ++i) {
				const auto party_id = ws.w.nation_s.active_parties.get(player, ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i)));
				if(vote[i] > 0.0f && party_id)
					pie.add_entry(
						ws.w.gui_m,
						ws.s.governments_m.parties[party_id].name,
						vote[i] / total,
						ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].color);
			}
		}
	}

	void culture_chart::update(ui::piechart<culture_chart>& pie, world_state & ws) {
		std::vector<float, concurrent_allocator<float>> sums_out(ws.s.culture_m.count_cultures);
		float total_size = 0;

		if(ws.w.population_w.display_type == population_display::nation) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.population_w.population_for_nation);
		} else if(ws.w.population_w.display_type == population_display::state) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.population_w.population_for_state);
		} else if(ws.w.population_w.display_type == population_display::province) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.population_w.population_for_province);
		}

		if(total_size == 0)
			return;

		for(uint32_t i = 0; i < ws.s.culture_m.count_cultures; ++i) {
			if(sums_out[i] != 0)
				pie.add_entry(
					ws.w.gui_m,
					ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size),
					ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].color);
		}
	}

	void issues_chart::update(ui::piechart<issues_chart>& pie, world_state & ws) {
		std::vector<float, concurrent_allocator<float>> sums_out(ws.s.issues_m.tracked_options_count);
		float total_size = 0;

		if(ws.w.population_w.display_type == population_display::nation) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.population_w.population_for_nation);
		} else if(ws.w.population_w.display_type == population_display::state) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.population_w.population_for_state);
		} else if(ws.w.population_w.display_type == population_display::province) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.population_w.population_for_province);
		}

		if(total_size == 0)
			return;

		for(uint32_t i = 0; i < ws.s.issues_m.tracked_options_count; ++i) {
			if(sums_out[i] != 0)
				pie.add_entry(
					ws.w.gui_m,
					ws.s.issues_m.options[issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size),
					ws.s.issues_m.options[issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))].color);
		}
	}

	ui::window_tag workforce_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}
	ui::window_tag religion_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}
	ui::window_tag culture_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}
	ui::window_tag issues_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}
	ui::window_tag ideology_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}
	ui::window_tag electorate_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}

	void create_pop_size_change_description(world_state& ws, pop_tag p, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::xy_pair{ 0, 0 };
		static const ui::text_format green_text{
			ui::text_color::green,
			ui::tooltip_text_format.font_handle,
			ui::tooltip_text_format.font_size
		};
		static const ui::text_format red_text{
			ui::text_color::red,
			ui::tooltip_text_format.font_handle,
			ui::tooltip_text_format.font_size
		};
		

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_growth>(p); amount >= 1.0f) {
			char16_t local_buf[32] = u"+";
			put_value_in_buffer(local_buf + 1, display_type::integer, amount);

			cursor = ui::add_text(cursor, local_buf, green_text, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_growth], ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		// early exit
		if(ws.w.population_s.pops.get<pop::type>(p) == ws.s.population_m.slave)
			return; 

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_combat>(p); amount >= 1.0f) {
			cursor = ui::add_text(cursor, text_data::integer{ -amount }, red_text, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_combat], ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_assimilation_away>(p); amount >= 1.0f) {
			cursor = ui::add_text(cursor, text_data::integer{ -amount }, red_text, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_assimilation], ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_emigration>(p); amount >= 1.0f) {
			cursor = ui::add_text(cursor, text_data::integer{ -amount }, red_text, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_emigration], ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_local_migration>(p); amount >= 1.0f) {
			cursor = ui::add_text(cursor, text_data::integer{ -amount }, red_text, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_migration], ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_type_change_away>(p); amount >= 1.0f) {
			cursor = ui::add_text(cursor, text_data::integer{ -amount }, red_text, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_type_change], ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}
	}

	void pop_growth::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		create_pop_size_change_description(ws, tag, tw);
	}

	void pop_size::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		create_pop_size_change_description(ws, tag, tw);
	}

	ui::window_tag population_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["popinfomember_popview"]);
	}

	void pop_filter_button::button_function(ui::simple_button<pop_filter_button>& self, world_state& ws) {
		if(ws.w.population_w.filtered_pop_types[type] == 0) {
			ws.w.population_w.filtered_pop_types[type] = 1ui8;
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			ws.w.population_w.filtered_pop_types[type] = 0ui8;
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
		ws.w.population_w.update_population_window(ws.w.gui_m);
	}
	void pop_filter_button::update(ui::simple_button<pop_filter_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, uint32_t(ws.s.population_m.pop_types[type].sprite) - 1ui32);
		if(ws.w.population_w.filtered_pop_types[type] == 0) {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		}
	}
	void pop_filter_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		ui::add_text(ui::xy_pair{0, 0}, ws.s.population_m.pop_types[type].name, ui::tooltip_text_format, ws, tw, lm);
		lm.finish_current_line();
	}

	void pop_type_button::button_function(ui::simple_button<pop_type_button>&, world_state & ws) {
		ws.w.population_w.win->details_w.pop_id = pop_id;
		ui::move_to_front(ws.w.gui_m, ui::tagged_gui_object{ *(ws.w.population_w.win->details_w.associated_object), ws.w.population_w.win->details_w.window_object });
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.population_w.win->details_w.associated_object));
	}

	void pop_type_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		if(is_valid_index(type))
			ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.population_m.pop_types[type].name, ui::tooltip_text_format, ws, tw, lm);
		lm.finish_current_line();
	}

	void pop_religion::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		auto cursor = ui::xy_pair{ 0,0 };
		if(is_valid_index(religion)) {
			cursor = ui::add_text(cursor, ws.s.culture_m.religions[religion].name, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
			display_conversion(cursor, ui::tooltip_text_format, ws, tw, lm, pop_id);
		}

		lm.finish_current_line();
	}

	void pops_unempl_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::unemployment], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}

	void lifeneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::life_needs], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}

	void eveneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::everyday_needs], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}

	void luxneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::luxury_needs], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}


	void pop_country_open_button::button_function(ui::simple_button<pop_country_open_button>&, world_state& ws) {
		ws.w.population_w.show_population_window(ws.w.gui_m, tag);
	}

	void pop_state_open_button::button_function(ui::simple_button<pop_state_open_button>&, world_state& ws) {
		if(is_valid_index(tag)) {
			auto owner = ws.w.nation_s.states.get<state::owner>(tag);
			if(is_valid_index(owner))
				ws.w.population_w.show_population_window(ws.w.gui_m, owner, tag);
		}
	}

	void pop_province_open_button::button_function(ui::simple_button<pop_province_open_button>&, world_state& ws) {
		if(is_valid_index(tag)) {
			auto si = ws.w.province_s.province_state_container.get<province_state::state_instance>(tag);
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(tag);
			if(is_valid_index(owner) && is_valid_index(si))
				ws.w.population_w.show_population_window(ws.w.gui_m, owner, si, tag);
		}
	}

	void pop_state_focus_button::button_function(ui::simple_button<pop_state_focus_button>&, world_state&) {

	}

	void pop_state_expand_button::button_function(ui::simple_button<pop_state_expand_button>&, world_state& ws) {
		if(ws.w.population_w.population_window_has_state_expanded(tag))
			ws.w.population_w.population_window_set_state_expanded(ws.w.gui_m, tag, false);
		else
			ws.w.population_w.population_window_set_state_expanded(ws.w.gui_m, tag, true);
	}

	void pop_state_expand_button::on_create(ui::simple_button<pop_state_expand_button>& b, world_state& ws) {
		ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag() });
	}

	ui::window_tag pop_tree_view::element_tag(size_t level, ui::gui_static& gui_m) {
		if(level == 0)
			return std::get<ui::window_tag>(gui_m.ui_definitions.name_to_element_map["poplistitem_country"]);
		else if(level == 1)
			return std::get<ui::window_tag>(gui_m.ui_definitions.name_to_element_map["poplistitem_state"]);
		else if(level == 2)
			return std::get<ui::window_tag>(gui_m.ui_definitions.name_to_element_map["poplistitem_province"]);
		return ui::window_tag();
	}

	std::vector<nations::country_tag, concurrent_allocator<nations::country_tag>> pop_tree_view::base_list(world_state& ws) {
		std::vector<nations::country_tag, concurrent_allocator<nations::country_tag>> result;
		if(ws.w.local_player_nation)
			result.push_back(ws.w.local_player_nation);
		return result;
	}
	population_window::population_window() : win(std::make_unique<population_window_t>()) {}
	population_window::~population_window() {}
	void population_window::init_population_window(world_state & ws) {
		filtered_pop_types.resize(ws.s.population_m.count_poptypes);
		for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i)
			filtered_pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))] = 1ui8;

		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_pop"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void population_window::hide_population_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}

	void population_window::show_population_window(ui::gui_manager& gui_m, nations::country_tag top, nations::state_tag s, provinces::province_tag p) {
		win->template get<CT_STRING("pop_province_list")>().force_open(top);
		win->template get<CT_STRING("pop_province_list")>().force_open(s);
		
		population_for_province = p;
		display_type = population_display::province;
		sort_type = population_sort::none;

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update_population_window(gui_m);
	}
	void population_window::show_population_window(ui::gui_manager& gui_m, nations::country_tag top, nations::state_tag t) {
		win->template get<CT_STRING("pop_province_list")>().force_open(top);
		population_for_state = t;
		display_type = population_display::state;
		sort_type = population_sort::none;

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update_population_window(gui_m);
	}
	void population_window::show_population_window(ui::gui_manager& gui_m, nations::country_tag t) {
		win->template get<CT_STRING("pop_province_list")>().force_open(t);
		population_for_nation = t;
		display_type = population_display::nation;
		sort_type = population_sort::none;

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update_population_window(gui_m);
	}
	void population_window::update_population_window(ui::gui_manager& gui_m) {
		win->template get<CT_STRING("pop_list")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	bool population_window::population_window_has_state_expanded(nations::state_tag t) {
		auto& poptree = win->template get<CT_STRING("pop_province_list")>();
		return poptree.is_open(t);
	}
	void population_window::population_window_set_state_expanded(ui::gui_manager& gui_m, nations::state_tag t, bool expand) {
		auto& poptree = win->template get<CT_STRING("pop_province_list")>();
		poptree.set_open(t, expand, gui_m);
	}
	void close_pop_details_button::button_function(ui::simple_button<close_pop_details_button>&, world_state & ws) {
		ui::hide(*(ws.w.population_w.win->details_w.associated_object));
	}
	void pop_bg_button::button_function(ui::simple_button<pop_bg_button>&, world_state & ws) {
		ws.w.population_w.win->details_w.pop_id = pop_id;
		ui::move_to_front(ws.w.gui_m, ui::tagged_gui_object{ *(ws.w.population_w.win->details_w.associated_object), ws.w.population_w.win->details_w.window_object });
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.population_w.win->details_w.associated_object));
	}
	void pop_details_growth_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		create_pop_size_change_description(ws, pop_id, tw);
	}
	void pop_details_culture_name::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		display_assimilation(ui::xy_pair{0,0}, ui::tooltip_text_format, ws, tw, lm, pop_id);
	}
	void pop_culture::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		display_assimilation(ui::xy_pair{ 0,0 }, ui::tooltip_text_format, ws, tw, lm, pop_id);
	}
	void pop_details_religion_name::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		display_conversion(ui::xy_pair{ 0,0 }, ui::tooltip_text_format, ws, tw, lm, pop_id);
	}
	void pop_details_demotion_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		float const projected_amount = project_promotion_amount(ws, pop_id);

		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;

		if(projected_amount > -1.0f) {
			cursor = ui::add_text(cursor, scenario::fixed_ui::no_demotion, ui::tooltip_text_format, ws, tw, lm);
		} else {
			cursor = ui::add_text(cursor, scenario::fixed_ui::projected_demotion, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui:add_text(cursor, text_data::exact_integer{ int32_t(-projected_amount) }, ui::tooltip_text_format, ws, tw, lm);
		}
		
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::add_text(cursor, scenario::fixed_ui::demotion_factors, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor, ws.s.modifiers_m.global_defines.promotion_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::promotion_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor, 
			w.w.nation_s.nations.get_row<nation::national_administrative_efficiency>(get_pop_owner(ws, pop_id)),
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::admin_efficiency, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		auto state_focus = ws.w.nation_s.states.get<state::owner_national_focus>(
			w.w.province_s.province_state_container.get<province_state::state_instance>(
				ws.w.population_s.pops.get<pop::location>(pop_id)));

		auto focus_target = w.s.modifiers_m.focus_to_pop_types[state_focus] == w.w.population_s.pops.get<pop::type>(pop_id);
		if(focus_target) {
			cursor = ui::display_colored_factor(cursor,
				0.0f,
				ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, scenario::fixed_ui::national_focus, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		}

		cursor = modifiers::make_additive_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.population_m.demotion_chance],
			ws, tw, cursor, lm, ui::tooltip_text_format, pop_id, pop_id, scenario::fixed_ui::factor);

	}
	void pop_details_promotion_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		float const projected_amount = project_promotion_amount(ws, pop_id);

		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;
		if(projected_amount < 1.0f) {
			cursor = ui::add_text(cursor, scenario::fixed_ui::no_promotion, ui::tooltip_text_format, ws, tw, lm);
		} else {
			cursor = ui::add_text(cursor, scenario::fixed_ui::projected_demotion, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui:add_text(cursor, text_data::exact_integer{ int32_t(projected_amount) }, ui::tooltip_text_format, ws, tw, lm);
		}
		
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::add_text(cursor, scenario::fixed_ui::promotion_factors, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor, ws.s.modifiers_m.global_defines.promotion_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::promotion_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor,
			w.w.nation_s.nations.get_row<nation::national_administrative_efficiency>(get_pop_owner(ws, pop_id)),
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::admin_efficiency, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		auto state_focus = ws.w.nation_s.states.get<state::owner_national_focus>(
			w.w.province_s.province_state_container.get<province_state::state_instance>(
				ws.w.population_s.pops.get<pop::location>(pop_id)));

		auto focus_target = w.s.modifiers_m.focus_to_pop_types[state_focus] == w.w.population_s.pops.get<pop::type>(pop_id);
		if(focus_target) {
			cursor = ui::display_colored_factor(cursor,
				0.0f,
				ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, scenario::fixed_ui::national_focus, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		}

		cursor = modifiers::make_additive_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.population_m.promotion_chance],
			ws, tw, cursor, lm, ui::tooltip_text_format, pop_id, pop_id, scenario::fixed_ui::factor);
	}
	void pop_details_colonial_migration_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::no_colonial_migration, fmt, ws, box, lm);
	}
	void pop_details_internal_migration_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		auto const projected_amount = project_migration_amount(ws, pop_id);

		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;
		if(projected_amount < 1.0f) {
			cursor = ui::add_text(cursor, scenario::fixed_ui::no_migration, ui::tooltip_text_format, ws, tw, lm);
		} else {
			cursor = ui::add_text(cursor, scenario::fixed_ui::projected_migration, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui:add_text(cursor, text_data::exact_integer{ int32_t(projected_amount) }, ui::tooltip_text_format, ws, tw, lm);
		}

		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::add_text(cursor, scenario::fixed_ui::migration_factors, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor, ws.s.modifiers_m.global_defines.immigration_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::emigration_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor,
			ws.w.province_s.modifier_values.get(modifiers::provincial_offsets::immigrant_push, ws.w.population_s.pops.get<pop::location>(pop_id)) + 1.0f,
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.province_offset_names[modifiers::provincial_offsets::immigrant_push], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_province_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, ws.w.population_s.pops.get<pop::location>(pop_id), modifiers::provincial_offsets::immigrant_push);
		lm.decrease_indent(1);

		cursor = modifiers::make_additive_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.population_m.migration_chance],
			ws, tw, cursor, lm, ui::tooltip_text_format, pop_id, pop_id, scenario::fixed_ui::factor);
	}
	void pop_details_external_migration_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		auto const projected_amount = project_emigration_amount(ws, pop_id);

		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;
		if(projected_amount < 1.0f) {
			cursor = ui::add_text(cursor, scenario::fixed_ui::no_emigration, ui::tooltip_text_format, ws, tw, lm);
		} else {
			cursor = ui::add_text(cursor, scenario::fixed_ui::projected_emigration, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui:add_text(cursor, text_data::exact_integer{ int32_t(projected_amount) }, ui::tooltip_text_format, ws, tw, lm);
		}

		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::add_text(cursor, scenario::fixed_ui::emigration_factors, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor, ws.s.modifiers_m.global_defines.immigration_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::emigration_scale, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		auto const mod = ws.w.province_s.modifier_values.get(modifiers::provincial_offsets::immigrant_push, ws.w.population_s.pops.get<pop::location>(pop_id)) + 1.0f;
		cursor = ui::display_colored_factor(cursor,
			mod * mod,
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.province_offset_names[modifiers::provincial_offsets::immigrant_push], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::parenthetical_squared, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_province_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, ws.w.population_s.pops.get<pop::location>(pop_id), modifiers::provincial_offsets::immigrant_push);
		lm.decrease_indent(1);

		cursor = modifiers::make_additive_factor_explanation(ws.s.modifiers_m.factor_modifiers[ws.s.population_m.emigration_chance],
			ws, tw, cursor, lm, ui::tooltip_text_format, pop_id, pop_id, scenario::fixed_ui::factor);
	}
	void pop_details_literacy_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		auto pop_loc = ws.w.population_s.pops.get<pop::location>(pop_id);
		auto pop_state = ws.w.province_s.province_state_container.get<province_state::state_instance>(pop_loc);

		auto change_amount = project_literacy_change(ws, pop_state);
		ui::text_format cfmt = ui::text_format{ 
			change_amount >= 0 ? ui::text_color::green :ui::text_color::red,
			ui::tooltip_text_format.font_handle,
			ui::tooltip_text_format.font_size};

		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;
		cursor = ui::add_text(cursor, scenario::fixed_ui::projected_literacy, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui:add_text(cursor, text_data::percent_fp_one_place{ change_amount }, cfmt, ws, tw, lm);

		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::add_text(cursor, scenario::fixed_ui::lit_factors, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor, ws.s.modifiers_m.global_defines.literacy_change_speed * 0.01f, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::literacy_change_speed, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		auto clergy_amount = ws.w.nation_s.state_demographics.get(pop_state, population::to_demo_tag(ws, ws.s.population_m.clergy));
		auto cfactor = std::min(1.0f, ((clergy_amount / total_pop) - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)
			* (1.0f / (ws.s.modifiers_m.global_defines.max_clergy_for_literacy - ws.s.modifiers_m.global_defines.base_clergy_for_literacy)));

		cursor = ui::display_colored_factor(cursor, cfactor, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::clergy_amount, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		auto pop_owner = get_pop_owner(ws, pop_id);
		cursor = ui::display_colored_factor(cursor, w.w.nation_s.nations.get<nation::f_education_spending>(pop_owner), ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::edu_spending, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor,
			1.0f + w.w.nation_s.tech_attributes.get<technologies::tech_offset::education_efficiency>(pop_owner),
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::tech_edu_efficiency, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor,
			ws.w.nation_s.modifier_values.get(modifiers::national_offsets::education_efficiency_modifier, pop_owner) + 1.0f,
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::education_efficiency_modifier], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner, modifiers::national_offsets::education_efficiency_modifier);
		lm.decrease_indent(1);
	}

	void pop_details_militancy_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		auto province_v = ws.w.population_s.pops.get<pop::location>(pop_id);
		auto state_indices = ve::load(province_v, w.w.province_s.province_state_container.get_row<province_state::state_instance>());

		auto prov_mil_mod = ve::load(province_v, w.w.province_s.modifier_values.get_row<modifiers::provincial_offsets::pop_militancy_modifier>(0));

		auto owner_indices = ve::load(province_v, w.w.province_s.province_state_container.get_row<province_state::owner>());
		auto pop_owner = owner_indices;
		auto owner_mil_mod = ve::load(owner_indices, w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::global_pop_militancy_modifier>(0));
		auto owner_war_ex = ve::load(owner_indices, w.w.nation_s.nations.get_row<nation::war_exhaustion>());
		auto owner_non_accepted = ve::load(owner_indices, w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::non_accepted_pop_militancy_modifier>(0));
		auto owner_separatism = ve::load(owner_indices, w.w.nation_s.tech_attributes.get_row<technologies::tech_offset::seperatism>(0));
		auto owner_core_pop_mil = ve::load(owner_indices, w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::core_pop_militancy_modifier>(0));

		auto change_amount = project_militancy_change(ws, pop_id) * 10.0f;
		ui::text_format cfmt = ui::text_format{
			change_amount >= 0 ? ui::text_color::red : ui::text_color::green,
			ui::tooltip_text_format.font_handle,
			ui::tooltip_text_format.font_size };

		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;
		cursor = ui::add_text(cursor, scenario::fixed_ui::projected_militancy, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui:add_text(cursor, text_data::fp_two_places{ change_amount }, cfmt, ws, tw, lm);

		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::add_text(cursor, scenario::fixed_ui::militancy_factors, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		bool core_mask = nations::is_colonial_or_protectorate(ws, state_indices);

		if(!ws.w.population_s.pops.get<pop::is_accepted>(pop_id)) {
			auto prov_non_accepted_modifier =
				(owner_separatism + 1.0f) * ws.s.modifiers_m.global_defines.mil_non_accepted,
				+owner_non_accepted;

			cursor = ui::display_colored_additive_factor(cursor, prov_non_accepted_modifier * 10.0f, ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, scenario::fixed_ui::literacy_change_speed, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
			lm.increase_indent(1);

			cursor = ui::display_colored_additive_factor(cursor,
				(owner_separatism + 1.0f) * ws.s.modifiers_m.global_defines.mil_non_accepted * 10.0f,
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, scenario::fixed_ui::owner_seperatism, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

			cursor = ui::display_colored_additive_factor(cursor,
				ws.w.nation_s.modifier_values.get(modifiers::national_offsets::non_accepted_pop_militancy_modifier, pop_owner),
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::non_accepted_pop_militancy_modifier],
				ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
			lm.increase_indent(1);
			cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner,
				modifiers::national_offsets::non_accepted_pop_militancy_modifier);
			lm.decrease_indent(1);

			lm.decrease_indent(1);
		}

		auto satisfaction_factor =
			ve::select(satisfaction < 0.5f,
				ve::negate_multiply_and_add(
					ws.s.modifiers_m.global_defines.mil_no_life_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_no_life_need * 0.5f
				),
				0.0f)
			+ ve::select(satisfaction < 1.5f,
				ve::negate_multiply_and_add(
					ws.s.modifiers_m.global_defines.mil_lack_everyday_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_lack_everyday_need * 1.5f
				),
				0.0f)
			+ ve::select(satisfaction > 1.5f,
				ve::multiply_and_subtract(
					ws.s.modifiers_m.global_defines.mil_has_everyday_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_has_everyday_need * 1.5f
				),
				0.0f)
			+ ve::select(satisfaction > 2.5f,
				ve::multiply_and_subtract(
					ws.s.modifiers_m.global_defines.mil_has_luxury_need,
					satisfaction,
					ws.s.modifiers_m.global_defines.mil_has_luxury_need * 2.5f
				),
				0.0f);

		cursor = ui::display_colored_additive_factor(cursor, satisfaction_factor, ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::needs_satisfaction, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		auto ruling_ideology = ve::load(owner_indices, ws.w.nation_s.nations.get_row<nation::ruling_ideology>());

		auto const psize = ws.w.population_s.pops.get<pop::size>(pop_v) != 0 ? ws.w.population_s.pops.get<pop::size>(pop_v) : 1.0f;
		auto const pop_c_support = ws.w.population_s.pop_demographics.get(pop_v, to_demo_tag(ws, ws.s.ideologies_m.conservative_ideology)) / psize;
		auto const pop_rp_support = ws.w.population_s.pop_demographics.get(pop_v, to_demo_tag(ws, ruling_ideology)) / psize;

		cursor = ui::display_colored_additive_factor(cursor, pop_c_support * ws.s.modifiers_m.global_defines.mil_ideology,
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::conservative_support, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_additive_factor(cursor, pop_rp_support * ws.s.modifiers_m.global_defines.mil_ruling_party,
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::ruling_party_support, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_additive_factor(cursor,
			ve::load(pop_v, w.w.population_s.pops.get_row<pop::political_interest>()) * ws.s.modifiers_m.global_defines.mil_require_reform,
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::political_reform_desire, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_additive_factor(cursor,
			ve::load(pop_v, w.w.population_s.pops.get_row<pop::social_interest>()) * ws.s.modifiers_m.global_defines.mil_require_reform,
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::social_reform_desire, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_additive_factor(cursor,
			owner_war_ex * ws.s.modifiers_m.global_defines.mil_war_exhaustion,
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::war_exhaustion, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_additive_factor(cursor,
			ws.w.nation_s.modifier_values.get(modifiers::national_offsets::global_pop_militancy_modifier, pop_owner),
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::global_pop_militancy_modifier],
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner,
			modifiers::national_offsets::global_pop_militancy_modifier);
		lm.decrease_indent(1);

		if(core_mask) {
			cursor = ui::display_colored_additive_factor(cursor,
				ws.w.nation_s.modifier_values.get(modifiers::national_offsets::core_pop_militancy_modifier, pop_owner),
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::core_pop_militancy_modifier],
				ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
			lm.increase_indent(1);
			cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner,
				modifiers::national_offsets::core_pop_militancy_modifier);
			lm.decrease_indent(1);
		}

		cursor = ui::display_colored_additive_factor(cursor,
			ws.w.province_s.modifier_values.get(modifiers::provincial_offsets::pop_militancy_modifier, ws.w.population_s.pops.get<pop::location>(pop_id)),
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.province_offset_names[modifiers::provincial_offsets::pop_militancy_modifier],
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_province_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, ws.w.population_s.pops.get<pop::location>(pop_id),
			modifiers::provincial_offsets::pop_militancy_modifier);
		lm.decrease_indent(1);
	}
	void pop_details_consciousness_value::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		auto pop_v = pop_id;
		auto owners = get_pop_owner(ws, pop_v);
		auto pop_owner = owners;
		auto province_v = ws.w.population_s.pops.get<pop::location>(pop_v);
		auto states = ve::load(province_v, w.w.province_s.province_state_container.get_row<province_state::state_instance>());

		auto change_amount = project_consciousness_change(ws, pop_id) * 10.0f;
		ui::text_format cfmt = ui::text_format{
			change_amount >= 0 ? ui::text_color::red : ui::text_color::green,
			ui::tooltip_text_format.font_handle,
			ui::tooltip_text_format.font_size };

		ui::xy_pair cursor{ 0,0 };
		ui::line_manager lm;
		cursor = ui::add_text(cursor, scenario::fixed_ui::projected_consciouness, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui:add_text(cursor, text_data::fp_two_places{ change_amount }, cfmt, ws, tw, lm);

		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		cursor = ui::add_text(cursor, scenario::fixed_ui::consciousness_factors, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		auto colonial_multiplier = ve::select(core_mask, 1.0f, ws.s.modifiers_m.global_defines.con_colonial_factor);

		auto const literacy_factor =
			colonial_multiplier
			* (1.0f + ve::load(owners, w.w.nation_s.modifier_values.get_row<modifiers::national_offsets::literacy_con_impact>(0)))
			* ve::load(owners, w.w.nation_s.nations.get_row<nation::plurality>())
			* ws.s.modifiers_m.global_defines.con_literacy
			* ws.w.population_s.pops.get<pop::literacy>(pop_v);

		cursor = ui::display_colored_additive_factor(cursor,
			literacy_factor,
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::literacy_factor, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		lm.increase_indent(1);

		cursor = ui::display_colored_factor(cursor,
			ve::load(owners, w.w.nation_s.nations.get_row<nation::plurality>()),
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::plurality, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_factor(cursor,
			ws.w.population_s.pops.get<pop::literacy>() * ws.s.modifiers_m.global_defines.con_literacy,
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, scenario::fixed_ui::literacy, ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);

		cursor = ui::display_colored_additive_factor(cursor,
			ws.w.nation_s.modifier_values.get(modifiers::national_offsets::literacy_con_impact, pop_owner) + 1.0f,
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::literacy_con_impact],
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner,
			modifiers::national_offsets::literacy_con_impact);
		lm.decrease_indent(1);

		lm.decrease_indent(1);

		auto province_pop = ws.w.province_s.province_state_container.get<province_state::total_population>();
		auto clergy_populations = ws.w.province_s.province_demographics.get(province_v, to_demo_tag(ws, ws.s.population_m.clergy));
		auto const clergy_percentage = clergy_populations / (province_pop ? 0.0f ? province_pop : 1.0f);

		if(ws.w.population_s.pops.get<pop::is_poor>(pop_v)) {
			cursor = ui::display_colored_additive_factor(cursor,
				clergy_percentage * colonial_multiplier *  ws.s.modifiers_m.global_defines.con_poor_clergy,
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, scenario::fixed_ui::poor_clergy, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		} else {
			cursor = ui::display_colored_additive_factor(cursor,
				clergy_percentage * colonial_multiplier *  ws.s.modifiers_m.global_defines.con_midrich_clergy,
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, scenario::fixed_ui::mid_rich_clergy, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		}

		auto lux_satisfaction = ve::load(pop_v, pop_satisfaction) - 2.0f;
		if(lux_satisfaction > 0.0f) {
			cursor = ui::display_colored_additive_factor(cursor,
				lux_satisfaction * ws.s.modifiers_m.global_defines.con_luxury_goods,
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, scenario::fixed_ui::luxury_needs_satisfaction, ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		}

		cursor = ui::display_colored_additive_factor(cursor,
			ws.w.province_s.modifier_values.get(modifiers::provincial_offsets::pop_consciousness_modifier, ws.w.population_s.pops.get<pop::location>(pop_id)),
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.province_offset_names[modifiers::provincial_offsets::pop_consciousness_modifier],
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_province_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, ws.w.population_s.pops.get<pop::location>(pop_id),
			modifiers::provincial_offsets::pop_consciousness_modifier);
		lm.decrease_indent(1);

		cursor = ui::display_colored_additive_factor(cursor,
			ws.w.nation_s.modifier_values.get(modifiers::national_offsets::global_pop_consciousness_modifier, pop_owner),
			ui::tooltip_text_format, ws, tw, lm, true);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::global_pop_consciousness_modifier],
			ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
		lm.increase_indent(1);
		cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner,
			modifiers::national_offsets::global_pop_consciousness_modifier);
		lm.decrease_indent(1);

		auto core_mask = nations::is_colonial_or_protectorate(ws, states);
		if(core_mask) {
			cursor = ui::display_colored_additive_factor(cursor,
				ws.w.nation_s.modifier_values.get(modifiers::national_offsets::core_pop_consciousness_modifier, pop_owner),
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::core_pop_consciousness_modifier],
				ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
			lm.increase_indent(1);
			cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner,
				modifiers::national_offsets::core_pop_consciousness_modifier);
			lm.decrease_indent(1);
		}

		if(!ws.w.population_s.pops.get<pop::is_accepted>(pop_id)) {
			cursor = ui::display_colored_additive_factor(cursor,
				ws.w.nation_s.modifier_values.get(modifiers::national_offsets::non_accepted_pop_consciousness_modifier, pop_owner),
				ui::tooltip_text_format, ws, tw, lm, true);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_text(cursor, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::non_accepted_pop_consciousness_modifier],
				ui::tooltip_text_format, ws, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format, lm);
			lm.increase_indent(1);
			cursor = modifiers::explain_national_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, pop_owner,
				modifiers::national_offsets::non_accepted_pop_consciousness_modifier);
			lm.decrease_indent(1);
		}
	}
	void pop_details_unemployment_overlay::create_tooltip(world_state &, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::unemployment], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}
	void details_lifeneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::life_needs], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}
	void details_eveneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::everyday_needs], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}
	void details_luxneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::luxury_needs], ui::tooltip_text_format, ws, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
		cursor = ui::add_text(cursor, text_data::percent{ value }, ui::tooltip_text_format, ws, tw, lm);
	}
}
