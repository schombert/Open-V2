#include "common\\common.h"
#include "population_gui.hpp"
#include "world_state\\world_state.h"

namespace population {
	bool population_window_base::on_keydown(ui::gui_object_tag, world_state& ws, const ui::key_down & k) {
		if(k.keycode == virtual_key::ESCAPE) {
			ws.w.hide_population_window();
			return true;
		} else {
			return false;
		}
	}

	
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.hide_population_window();
	}

	void popfilter_all_button::button_function(ui::simple_button<popfilter_all_button>&, world_state & ws) {
		for(auto& v : ws.w.selected_population.filtered_pop_types)
			v = 1ui8;
		ws.w.update_population_window();
	}

	void popfilter_none_button::button_function(ui::simple_button<popfilter_none_button>&, world_state & ws) {
		for(auto& v : ws.w.selected_population.filtered_pop_types)
			v = 0ui8;
		ws.w.update_population_window();
	}

	void sortby_size_button::button_function(ui::simple_button<sortby_size_button>&, world_state &) {}

	void sortby_type_button::button_function(ui::simple_button<sortby_type_button>&, world_state &) {}

	void sortby_nationality_button::button_function(ui::simple_button<sortby_nationality_button>&, world_state &) {}

	void sortby_religion_button::button_function(ui::simple_button<sortby_religion_button>&, world_state &) {}

	void sortby_location_button::button_function(ui::simple_button<sortby_location_button>&, world_state &) {}

	void sortby_mil_button::button_function(ui::simple_button<sortby_mil_button>&, world_state &) {}

	void sortby_con_button::button_function(ui::simple_button<sortby_con_button>&, world_state &) {}

	void sortby_ideology_button::button_function(ui::simple_button<sortby_ideology_button>&, world_state &) {}

	void sortby_issues_button::button_function(ui::simple_button<sortby_issues_button>&, world_state &) {}

	void sortby_unemployment_button::button_function(ui::simple_button<sortby_unemployment_button>&, world_state &) {}

	void sortby_cash_button::button_function(ui::simple_button<sortby_cash_button>&, world_state &) {}

	void sortby_subsistence_button::button_function(ui::simple_button<sortby_subsistence_button>&, world_state &) {}

	void sortby_eve_button::button_function(ui::simple_button<sortby_eve_button>&, world_state &) {}

	void sortby_luxury_button::button_function(ui::simple_button<sortby_luxury_button>&, world_state &) {}

	void sortby_revoltrisk_button::button_function(ui::simple_button<sortby_revoltrisk_button>&, world_state &) {}

	void sortby_change_button::button_function(ui::simple_button<sortby_change_button>&, world_state &) {}

	void sortby_literacy_button::button_function(ui::simple_button<sortby_literacy_button>&, world_state &) {}

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

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			total_size = sum_filtered_demo_data<population::pop_type_tag>(ws, sums_out, ws.w.selected_population.population_for_nation);
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			total_size = sum_filtered_demo_data<population::pop_type_tag>(ws, sums_out, ws.w.selected_population.population_for_state);
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			total_size = sum_filtered_demo_data<population::pop_type_tag>(ws, sums_out, ws.w.selected_population.population_for_province);
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

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			total_size = sum_filtered_demo_data<cultures::religion_tag>(ws, sums_out, ws.w.selected_population.population_for_nation);
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			total_size = sum_filtered_demo_data<cultures::religion_tag>(ws, sums_out, ws.w.selected_population.population_for_state);
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			total_size = sum_filtered_demo_data<cultures::religion_tag>(ws, sums_out, ws.w.selected_population.population_for_province);
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

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			total_size = sum_filtered_demo_data<ideologies::ideology_tag>(ws, sums_out, ws.w.selected_population.population_for_nation);
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			total_size = sum_filtered_demo_data<ideologies::ideology_tag>(ws, sums_out, ws.w.selected_population.population_for_state);
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			total_size = sum_filtered_demo_data<ideologies::ideology_tag>(ws, sums_out, ws.w.selected_population.population_for_province);
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

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_nation);
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_state);
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_province);
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

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_nation);
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_state);
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_province);
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

	void pop_filter_button::button_function(ui::simple_button<pop_filter_button>& self, world_state& ws) {
		if(ws.w.selected_population.filtered_pop_types[type] == 0) {
			ws.w.selected_population.filtered_pop_types[type] = 1ui8;
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			ws.w.selected_population.filtered_pop_types[type] = 0ui8;
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
		ws.w.update_population_window();
	}
	void pop_filter_button::update(ui::simple_button<pop_filter_button>& self, world_state& ws) {
		self.set_frame(ws.w.gui_m, uint32_t(ws.s.population_m.pop_types[type].sprite) - 1ui32);
		if(ws.w.selected_population.filtered_pop_types[type] == 0) {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		}
	}
	void pop_filter_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		ui::add_linear_text(ui::xy_pair{0, 0}, ws.s.population_m.pop_types[type].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
	}
}
