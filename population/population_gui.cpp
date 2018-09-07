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

	void popfilter_all_button::button_function(ui::simple_button<popfilter_all_button>&, world_state &) {}

	void popfilter_none_button::button_function(ui::simple_button<popfilter_none_button>&, world_state &) {}

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
	ui::window_tag workforce_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}
}
