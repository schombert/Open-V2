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
		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			auto selected = ws.w.selected_population.population_for_nation;
			if(!ws.w.nation_s.nations.is_valid_index(selected))
				return;
			auto total_pop = float(ws.w.nation_s.nation_demographics.get(selected, population::total_population_tag));
			if(total_pop == 0.0f)
				return;
			auto poptypes = ws.w.nation_s.nation_demographics.get_row(selected) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));
			for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
				auto pop_of_type = poptypes[i];

				if(pop_of_type != 0)
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name),
						float(pop_of_type) / total_pop,
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color);
			}
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			auto selected = ws.w.selected_population.population_for_state;
			if(!ws.w.nation_s.states.is_valid_index(selected))
				return;
			auto total_pop = float(ws.w.nation_s.state_demographics.get(selected, population::total_population_tag));
			if(total_pop == 0.0f)
				return;
			auto poptypes = ws.w.nation_s.state_demographics.get_row(selected) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));
			for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
				auto pop_of_type = poptypes[i];

				if(pop_of_type != 0)
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name),
						float(pop_of_type) / total_pop,
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color);
			}
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			auto selected = ws.w.selected_population.population_for_province;
			if(!is_valid_index(selected))
				return;
			auto total_pop = float(ws.w.province_s.province_demographics.get(selected, population::total_population_tag));
			if(total_pop == 0.0f)
				return;
			auto poptypes = ws.w.province_s.province_demographics.get_row(selected) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));
			for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
				auto pop_of_type = poptypes[i];

				if(pop_of_type != 0)
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name),
						float(pop_of_type) / total_pop,
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color);
			}
		}
	}
	ui::window_tag workforce_lb::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_legend_item"]);
	}
}
