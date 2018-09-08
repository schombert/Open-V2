#pragma once
#include "common\\common.h"
#include "population_gui.h"
#include "provinces\\province_functions.hpp"
#include "nations\\nations_functions.hpp"
#include "population_function.h"

namespace population {

	template<typename W>
	void legend_icon::windowed_update(ui::tinted_icon<legend_icon>& self, W& w, world_state& ws) {
		self.set_color(ws.w.gui_m, float(w.color.r) / 255.0f, float(w.color.g) / 255.0f, float(w.color.b) / 255.0f);
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

	// returns: total pop

	template<typename tag_type, typename FUNC>
	void generic_for_each_pop(world_state& ws, tag_type tag, FUNC&& f) {
		if constexpr(std::is_same_v<tag_type, nations::country_tag>) {
			if(ws.w.nation_s.nations.is_valid_index(tag))
				nations::for_each_pop(ws, ws.w.nation_s.nations[tag], f);
		} else if constexpr(std::is_same_v<tag_type, nations::state_tag>) {
			if(ws.w.nation_s.states.is_valid_index(tag))
				nations::for_each_pop(ws, ws.w.nation_s.states[tag], f);
		} else if constexpr(std::is_same_v<tag_type, provinces::province_tag>) {
			if(is_valid_index(tag))
				provinces::for_each_pop(ws, ws.w.province_s.province_state_container[tag], f);
		}
	}

	template<typename category_type, typename tag_type>
	int64_t sum_filtered_demo_data(world_state& ws, int64_t* sums_out, tag_type tag) {
		int64_t total = 0;
		generic_for_each_pop(ws, tag, [sums_out, &ws, &total](population::pop& p) {
			auto pop_id = p.id;
			auto ptype = p.type;
			if(ws.w.population_s.pops.is_valid_index(pop_id) && is_valid_index(ptype) && ws.w.selected_population.filtered_pop_types[ptype] != 0) {
				auto size = ws.w.population_s.pop_demographics.get(pop_id, population::total_population_tag);
				total += size;
				if constexpr(std::is_same_v<category_type, cultures::culture_tag>) {
					auto c = p.culture;
					if(is_valid_index(c))
						sums_out[to_index(c)] += size;
				} else if constexpr(std::is_same_v<category_type, cultures::religion_tag>) {
					auto c = p.religion;
					if(is_valid_index(c))
						sums_out[to_index(c)] += size;
				} else if constexpr(std::is_same_v<category_type, population::pop_type_tag>) {
					sums_out[to_index(ptype)] += size;
				} else if constexpr(std::is_same_v<category_type, ideologies::ideology_tag>) {
					for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
						sums_out[i] += ws.w.population_s.pop_demographics.get(pop_id, population::to_demo_tag(ws, ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))));
					}
				} else if constexpr(std::is_same_v<category_type, issues::option_tag>) {
					for(uint32_t i = 0; i < ws.s.issues_m.tracked_options_count; ++i) {
						sums_out[i] += ws.w.population_s.pop_demographics.get(pop_id, population::to_demo_tag(ws, issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))));
					}
				} else {
					std::abort(); // called with wrong category type
				}
			}
		});
		return total;
	}
	

	template<typename W>
	void population_window_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -2i16, 4i16 };
		});


		{
			auto wwin = ui::create_static_element(
				ws,
				std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["distribution_window"]),
				ui::tagged_gui_object{ *associated_object, w.window_object },
				workforce);
			auto& pie = workforce.template get<CT_STRING("chart")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
			workforce.associated_object->position = ui::xy_pair{ 253i16, 92i16 };
			workforce.associated_object->size = ui::xy_pair{ 255i16, 95i16 };
			ui::move_to_front(ws.w.gui_m, wwin);
		}

		{
			auto wwin = ui::create_static_element(
				ws,
				std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["distribution_window"]),
				ui::tagged_gui_object{ *associated_object, w.window_object },
				cultures_w);
			auto& pie = cultures_w.template get<CT_STRING("chart")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
			cultures_w.associated_object->position = ui::xy_pair{ int16_t(253 + 243), int16_t(92) };
			cultures_w.associated_object->size = ui::xy_pair{ 255i16, 95i16 };
			ui::move_to_front(ws.w.gui_m, wwin);
		}

		{
			auto wwin = ui::create_static_element(
				ws,
				std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["distribution_window"]),
				ui::tagged_gui_object{ *associated_object, w.window_object },
				religions_w);
			auto& pie = religions_w.template get<CT_STRING("chart")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
			religions_w.associated_object->position = ui::xy_pair{ int16_t(253 + 2 * 243), int16_t(92) };
			religions_w.associated_object->size = ui::xy_pair{ 255i16, 95i16 };
			ui::move_to_front(ws.w.gui_m, wwin);
		}

		{
			auto wwin = ui::create_static_element(
				ws,
				std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["distribution_window"]),
				ui::tagged_gui_object{ *associated_object, w.window_object },
				ideologies_w);
			auto& pie = ideologies_w.template get<CT_STRING("chart")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
			ideologies_w.associated_object->position = ui::xy_pair{ int16_t(253 + 0 * 243), int16_t(92 + 101) };
			ideologies_w.associated_object->size = ui::xy_pair{ 255i16, 95i16 };
			ui::move_to_front(ws.w.gui_m, wwin);
		}

		{
			auto wwin = ui::create_static_element(
				ws,
				std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["distribution_window"]),
				ui::tagged_gui_object{ *associated_object, w.window_object },
				issues_w);
			auto& pie = issues_w.template get<CT_STRING("chart")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
			issues_w.associated_object->position = ui::xy_pair{ int16_t(253 + 1 * 243), int16_t(92 + 101) };
			issues_w.associated_object->size = ui::xy_pair{ 255i16, 95i16 };
			ui::move_to_front(ws.w.gui_m, wwin);
		}

		{
			auto wwin = ui::create_static_element(
				ws,
				std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["distribution_window"]),
				ui::tagged_gui_object{ *associated_object, w.window_object },
				electorate_w);
			auto& pie = electorate_w.template get<CT_STRING("chart")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
			electorate_w.associated_object->position = ui::xy_pair{ int16_t(253 + 2 * 243), int16_t(92 + 101) };
			electorate_w.associated_object->size = ui::xy_pair{ 255i16, 95i16 };
			ui::move_to_front(ws.w.gui_m, wwin);
		}

		for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
			filter_buttons.emplace_back(pop_type_tag(static_cast<pop_type_tag::value_base_t>(i)));

			auto fb = ui::create_static_element(
				ws,
				std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["pop_filter_button"]),
				ui::tagged_gui_object{ *associated_object, w.window_object },
				filter_buttons.back());
			fb.object.position = ui::xy_pair{ int16_t(285 + 32 * i), int16_t(25) };
			ui::move_to_front(ws.w.gui_m, fb);
		}


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
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, float>, 32> data;

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

		if(total_size == 0) {
			lb.new_list(nullptr, nullptr);
			return;
		}

		for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
			if(sums_out[i] != 0)
				data.emplace_back(
					ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color,
					ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size));
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename lb_type>
	void religion_lb::populate_list(lb_type& lb, world_state & ws) {
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, float>, 32> data;

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

		if(total_size == 0) {
			lb.new_list(nullptr, nullptr);
			return;
		}

		for(uint32_t i = 0; i < ws.s.culture_m.count_religions; ++i) {
			if(sums_out[i] != 0)
				data.emplace_back(
					ws.s.culture_m.religions[cultures::religion_tag(static_cast<cultures::religion_tag::value_base_t>(i))].color,
					ws.s.culture_m.religions[cultures::religion_tag(static_cast<cultures::religion_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size));
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename lb_type>
	void ideology_lb::populate_list(lb_type& lb, world_state & ws) {
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, float>, 32> data;

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

		if(total_size == 0) {
			lb.new_list(nullptr, nullptr);
			return;
		}

		for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
			if(sums_out[i] != 0)
				data.emplace_back(
					ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].color,
					ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size));
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename lb_type>
	void electorate_lb::populate_list(lb_type&, world_state&) {
	}

	template <typename lb_type>
	void culture_lb::populate_list(lb_type& lb, world_state & ws) {
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, float>, 32> data;

		std::vector<int64_t, concurrent_allocator<int64_t>> sums_out(ws.s.culture_m.count_cultures);
		int64_t total_size = 0;

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_nation);
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_state);
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			total_size = sum_filtered_demo_data<cultures::culture_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_province);
		}

		if(total_size == 0) {
			lb.new_list(nullptr, nullptr);
			return;
		}

		for(uint32_t i = 0; i < ws.s.culture_m.count_cultures; ++i) {
			if(sums_out[i] != 0)
				data.emplace_back(
					ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].color,
					ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size));
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template <typename lb_type>
	void issues_lb::populate_list(lb_type& lb, world_state & ws) {
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, float>, 32> data;

		std::vector<int64_t, concurrent_allocator<int64_t>> sums_out(ws.s.issues_m.tracked_options_count);
		int64_t total_size = 0;

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_nation);
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_state);
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			total_size = sum_filtered_demo_data<issues::option_tag>(ws, sums_out.data(), ws.w.selected_population.population_for_province);
		}

		if(total_size == 0) {
			lb.new_list(nullptr, nullptr);
			return;
		}

		for(uint32_t i = 0; i < ws.s.issues_m.tracked_options_count; ++i) {
			if(sums_out[i] != 0)
				data.emplace_back(
					ws.s.issues_m.options[issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))].color,
					ws.s.issues_m.options[issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))].name,
					float(sums_out[i]) / float(total_size));
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}
}
