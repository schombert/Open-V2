#pragma once
#include "common\\common.h"
#include "population_gui.h"

namespace population {

	template<typename W>
	void legend_icon::windowed_update(ui::tinted_icon<legend_icon>& self, W& w, world_state& ws) {
		self.set_color(ws.w.gui_m, w.color.r, w.color.g, w.color.b);
	}

	template<typename window_type>
	void legend_label::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, win.legend_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void legend_percentage::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::percent, win.percentage);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);

		lm.finish_current_line();
	}

	template<typename W>
	void population_window_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -2i16, 4i16 };
		});


		auto wwin = ui::create_static_element(
			ws,
			std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["distribution_window"]),
			ui::tagged_gui_object{ *associated_object, w.window_object },
			workforce);
		workforce.associated_object->position = ui::xy_pair{ 268i16, 90i16 };
		ui::move_to_front(ws.w.gui_m, wwin);

		auto& close_button = w.template get<CT_STRING("close_button")>();
		close_button.associated_object->position = ui::xy_pair{ -31i16, -1i16 };

		auto& sb = w.template get<CT_STRING("external_scroll_slider")>();
		ui::hide(*sb.associated_object);

		{
			auto& b = w.template get<CT_STRING("sortby_mil_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_con_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_issues_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_unemployment_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_cash_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_subsistence_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_eve_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_luxury_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_revoltrisk_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_change_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_literacy_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
		{
			auto& b = w.template get<CT_STRING("sortby_ideology_button")>();
			b.associated_object->position += ui::xy_pair{ 0i16, -1i16 };
			ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *b.associated_object, ui::gui_object_tag(0) });
		}
	}

	template<typename lb_type>
	void workforce_lb::populate_list(lb_type& lb, world_state & ws) {
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, int32_t>, 32> data;

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
					data.emplace_back(
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color,
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name,
						int32_t(100.0f * float(pop_of_type) / total_pop));
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
					data.emplace_back(
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color,
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name,
						int32_t(100.0f * float(pop_of_type) / total_pop));
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
					data.emplace_back(
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color,
						ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name,
						int32_t(100.0f * float(pop_of_type) / total_pop));
			}
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}
}
