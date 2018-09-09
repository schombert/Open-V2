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
	void pop_list_item_base::on_create(W& w, world_state&) {
		{
			auto& pie = w.template get<CT_STRING("pop_ideology")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
		}
		{
			auto& pie = w.template get<CT_STRING("pop_issues")>();
			pie.associated_object->size.x *= 2;
			pie.associated_object->size.y *= 2;
		}
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
	void population_lb::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<pop_tag, 64, concurrent_allocator<pop_tag>> data;

		if(ws.w.selected_population.display_type == current_state::population_display::nation) {
			generic_for_each_pop(ws, ws.w.selected_population.population_for_nation, [&ws, &data](population::pop& p) {
				auto id = p.id;
				if(ws.w.population_s.pops.is_valid_index(id))
					data.push_back(id);
			});
		} else if(ws.w.selected_population.display_type == current_state::population_display::state) {
			generic_for_each_pop(ws, ws.w.selected_population.population_for_state, [&ws, &data](population::pop& p) {
				auto id = p.id;
				if(ws.w.population_s.pops.is_valid_index(id))
					data.push_back(id);
			});
		} else if(ws.w.selected_population.display_type == current_state::population_display::province) {
			generic_for_each_pop(ws, ws.w.selected_population.population_for_province, [&ws, &data](population::pop& p) {
				auto id = p.id;
				if(ws.w.population_s.pops.is_valid_index(id))
					data.push_back(id);
			});
		}

		//do sorting

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
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

	template<typename window_type>
	void pop_size::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::integer, int32_t(ws.w.population_s.pop_demographics.get(w.tag, total_population_tag)));

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void pop_culture::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		cultures::culture_tag c = ws.w.population_s.pops[w.tag].culture;
		if(is_valid_index(c))
			ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.s.culture_m.culture_container[c].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void pop_location::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		provinces::province_tag p = ws.w.population_s.pops[w.tag].location;
		if(is_valid_index(p))
			ui::add_linear_text(ui::xy_pair{ 0, 0 }, ws.w.province_s.province_state_container[p].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void pop_militancy::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		float mil = population::get_militancy_direct(ws.w.population_s.pops[w.tag]);

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::fp_two_places, mil);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void pop_consciousness::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		float mil = population::get_consciousness_direct(ws.w.population_s.pops[w.tag]);

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::fp_two_places, mil);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void pop_literacy::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		float mil = population::get_literacy_direct(ws.w.population_s.pops[w.tag]);

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::percent, mil);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void pop_cash::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		float money = ws.w.population_s.pops[w.tag].money;

		char16_t local_buf[32];
		put_value_in_buffer(local_buf, display_type::fp_two_places, money);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void pop_ideology::windowed_update(ui::piechart<pop_ideology>& pie, window_type& w, world_state& ws) {
		int32_t* demo = ws.w.population_s.pop_demographics.get_row(w.tag);
		int32_t* ideologies_values = demo + to_index(to_demo_tag(ws, ideologies::ideology_tag(0)));

		float size = float(demo[to_index(total_population_tag)]);
		if(size != 0.0f) {
			for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
				if(ideologies_values[i] != 0)
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].name),
						float(ideologies_values[i]) / size,
						ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].color);
			}
		}
	}

	template<typename window_type>
	void pop_issues::windowed_update(ui::piechart<pop_issues>& pie, window_type& w, world_state& ws) {
		int32_t* demo = ws.w.population_s.pop_demographics.get_row(w.tag);
		int32_t* issues_values = demo + to_index(to_demo_tag(ws, issues::option_tag(0)));

		float size = float(demo[to_index(total_population_tag)]);
		if(size != 0.0f) {
			for(uint32_t i = 0; i < ws.s.issues_m.tracked_options_count; ++i) {
				if(issues_values[i] != 0)
					pie.add_entry(
						ws.w.gui_m,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.options[issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))].name),
						float(issues_values[i]) / size,
						ws.s.issues_m.options[issues::option_tag(static_cast<issues::option_tag::value_base_t>(i))].color);
			}
		}
	}

	template<typename W>
	void pop_type_button::windowed_update(ui::simple_button<pop_type_button>& b, W& w, world_state& ws) {
		auto& pop = ws.w.population_s.pops[w.tag];
		type = pop.type;
		if(is_valid_index(type)) 
			b.set_frame(ws.w.gui_m, ws.s.population_m.pop_types[type].sprite - 1ui32);
	}

	template<typename W>
	void pop_producing_icon::windowed_update(ui::dynamic_icon<pop_producing_icon>& self, W& w, world_state& ws) {
		ui::hide(*self.associated_object);
	}

	template<typename W>
	void pop_religion::windowed_update(ui::dynamic_icon<pop_religion>& ico, W& w, world_state& ws) {
		religion = ws.w.population_s.pops[w.tag].religion;
		if(is_valid_index(religion))
			ico.set_frame(ws.w.gui_m, uint32_t(ws.s.culture_m.religions[religion].icon) - 1ui32);
	}

	template<typename window_type>
	void pop_unemployment_bar::windowed_update(ui::progress_bar<pop_unemployment_bar>& bar, window_type& w, world_state& ws) {
		float size = float(ws.w.population_s.pop_demographics.get(w.tag, total_population_tag));
		float employed = float(ws.w.population_s.pop_demographics.get(w.tag, total_employment_tag));

		if(size != 0.0f) 
			bar.set_fraction(1.0f - employed / size);
		else 
			bar.set_fraction(0.0f);
	}

	template<typename W>
	void pops_unempl_overlay::windowed_update(ui::dynamic_icon<pops_unempl_overlay>&, W& w, world_state& ws) {
		float size = float(ws.w.population_s.pop_demographics.get(w.tag, total_population_tag));
		float employed = float(ws.w.population_s.pop_demographics.get(w.tag, total_employment_tag));

		if(size != 0.0f)
			value = 1.0f - employed / size;
		else
			value = 0.0f;
	}

	template<typename window_type>
	void lifeneed_progress::windowed_update(ui::progress_bar<lifeneed_progress>& bar, window_type& w, world_state& ws) {
		float needs = ws.w.population_s.pops[w.tag].needs_satisfaction;

		if(needs >= 1.0f) 
			bar.set_fraction(1.0f);
		else 
			bar.set_fraction(needs);
	}

	template<typename W>
	void lifeneed_progress_overlay::windowed_update(ui::dynamic_icon<lifeneed_progress_overlay>&, W& w, world_state& ws) {
		value = std::min(1.0f, ws.w.population_s.pops[w.tag].needs_satisfaction);
	}

	template<typename window_type>
	void eveneed_progress::windowed_update(ui::progress_bar<eveneed_progress>& bar, window_type& w, world_state& ws) {
		float needs = ws.w.population_s.pops[w.tag].needs_satisfaction;

		bar.set_fraction(std::clamp(needs - 1.0f, 0.0f, 1.0f));
	}

	template<typename W>
	void eveneed_progress_overlay::windowed_update(ui::dynamic_icon<eveneed_progress_overlay>&, W& w, world_state& ws) {
		value = std::clamp(ws.w.population_s.pops[w.tag].needs_satisfaction - 1.0f, 0.0f, 1.0f);
	}

	template<typename window_type>
	void luxneed_progress::windowed_update(ui::progress_bar<luxneed_progress>& bar, window_type& w, world_state& ws) {
		float needs = ws.w.population_s.pops[w.tag].needs_satisfaction;

		bar.set_fraction(std::clamp(needs - 2.0f, 0.0f, 1.0f));
	}

	template<typename W>
	void luxneed_progress_overlay::windowed_update(ui::dynamic_icon<luxneed_progress_overlay>&, W& w, world_state& ws) {
		value = std::clamp(ws.w.population_s.pops[w.tag].needs_satisfaction - 2.0f, 0.0f, 1.0f);
	}

	template<typename W>
	void pop_revolt::windowed_update(ui::dynamic_icon<pop_revolt>& ico, W& w, world_state& ws) {
		ui::hide(*ico.associated_object);
	}

	template<typename W>
	void pop_movement_social::windowed_update(ui::dynamic_icon<pop_movement_social>& ico, W& w, world_state& ws) {
		ui::hide(*ico.associated_object);
	}

	template<typename W>
	void pop_movement_political::windowed_update(ui::dynamic_icon<pop_movement_political>& ico, W& w, world_state& ws) {
		ui::hide(*ico.associated_object);
	}

	template<typename W>
	void pop_movement_flag::windowed_update(ui::masked_flag<pop_movement_flag>& ico, W& w, world_state& ws) {
		ui::hide(*ico.associated_object);
	}
}
