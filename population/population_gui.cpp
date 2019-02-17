#include "common\\common.h"
#include "population_gui.hpp"
#include "world_state\\world_state.h"

namespace population {
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
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::workforce], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void religion_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::religion], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void culture_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::nationality], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void ideology_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::ideology], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void issues_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::dominant_issues], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void electorate_title::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::electorate], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	void workforce_chart::update(ui::piechart<workforce_chart>& pie, world_state & ws) {
		int64_t* sums_out = (int64_t*)_alloca(sizeof(int64_t) * ws.s.population_m.count_poptypes);
		int64_t total_size = 0;
		std::fill_n(sums_out, ws.s.population_m.count_poptypes, 0);

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
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name),
					float(sums_out[i]) / float(total_size),
					ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color);
		}
	}

	void religion_chart::update(ui::piechart<religion_chart>& pie, world_state & ws) {
		int64_t* sums_out = (int64_t*)_alloca(sizeof(int64_t) * ws.s.culture_m.count_religions);
		int64_t total_size = 0;
		std::fill_n(sums_out, ws.s.culture_m.count_religions, 0);

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
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.religions[cultures::religion_tag(static_cast<cultures::religion_tag::value_base_t>(i))].name),
					float(sums_out[i]) / float(total_size),
					ws.s.culture_m.religions[cultures::religion_tag(static_cast<cultures::religion_tag::value_base_t>(i))].color);
		}
	}

	void ideology_chart::update(ui::piechart<ideology_chart>& pie, world_state & ws) {
		int64_t* sums_out = (int64_t*)_alloca(sizeof(int64_t) * ws.s.ideologies_m.ideologies_count);
		int64_t total_size = 0;
		std::fill_n(sums_out, ws.s.ideologies_m.ideologies_count, 0);

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
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].name),
					float(sums_out[i]) / float(total_size),
					ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].color);
		}
	}

	void electorate_chart::update(ui::piechart<electorate_chart>&, world_state&) {}

	void culture_chart::update(ui::piechart<culture_chart>& pie, world_state & ws) {
		std::vector<int64_t, concurrent_allocator<int64_t>> sums_out(ws.s.culture_m.count_cultures);
		int64_t total_size = 0;

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
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].name),
					float(sums_out[i]) / float(total_size),
					ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].color);
		}
	}

	void issues_chart::update(ui::piechart<issues_chart>& pie, world_state & ws) {
		std::vector<int64_t, concurrent_allocator<int64_t>> sums_out(ws.s.issues_m.tracked_options_count);
		int64_t total_size = 0;

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
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.options[issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))].name),
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

			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				green_text,
				lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_growth], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		// early exit
		if(ws.w.population_s.pops.get<pop::type>(p) == ws.s.population_m.slave)
			return; 

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_combat>(p); amount >= 1.0f) {
			char16_t local_buf[32];
			put_value_in_buffer(local_buf, display_type::integer, -amount);

			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				red_text,
				lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_combat], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_assimilation_away>(p); amount >= 1.0f) {
			char16_t local_buf[32];
			put_value_in_buffer(local_buf, display_type::integer, -amount);

			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				red_text,
				lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_assimilation], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_emigration>(p); amount >= 1.0f) {
			char16_t local_buf[32];
			put_value_in_buffer(local_buf, display_type::integer, -amount);

			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				red_text,
				lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_emigration], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_local_migration>(p); amount >= 1.0f) {
			char16_t local_buf[32];
			put_value_in_buffer(local_buf, display_type::integer, -amount);

			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				red_text,
				lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_migration], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
		}

		if(auto const amount = ws.w.population_s.pops.get<pop::size_change_from_type_change_away>(p); amount >= 1.0f) {
			char16_t local_buf[32];
			put_value_in_buffer(local_buf, display_type::integer, -amount);

			cursor = ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(local_buf),
				tw,
				cursor,
				red_text,
				lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::pop_size_type_change], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
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
		ui::add_linear_text(ui::xy_pair{0, 0}, ws.s.population_m.pop_types[type].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
		lm.finish_current_line();
	}

	void pop_type_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		if(is_valid_index(type))
			ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.s.population_m.pop_types[type].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
		lm.finish_current_line();
	}

	void pop_religion::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		if(is_valid_index(religion))
			ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.s.culture_m.religions[religion].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
		lm.finish_current_line();
	}

	void pops_unempl_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::unemployment], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::percent, value);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			tw,
			cursor,
			ui::tooltip_text_format,
			lm);

		lm.finish_current_line();
	}

	void lifeneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::life_needs], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::percent, value);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			tw,
			cursor,
			ui::tooltip_text_format,
			lm);

		lm.finish_current_line();
	}

	void eveneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::everyday_needs], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::percent, value);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			tw,
			cursor,
			ui::tooltip_text_format,
			lm);

		lm.finish_current_line();
	}

	void luxneed_progress_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto cursor = ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.s.fixed_ui_text[scenario::fixed_ui::luxury_needs], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
		cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::percent, value);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			tw,
			cursor,
			ui::tooltip_text_format,
			lm);

		lm.finish_current_line();
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
}
