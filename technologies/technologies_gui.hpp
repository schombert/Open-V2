#pragma once
#include "common\\common.h"
#include "technologies_gui.h"
#include "gui\\gui.hpp"
#include "modifiers\\modifiers_gui.h"
#include "technologies_functions.h"

namespace technologies {
	template<typename W>
	void tech_window_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
		w.template get<CT_STRING("administration_type")>().associated_object->position.x += 10i16;
	}

	template<typename window_type>
	void plusminus_icon::windowed_update(ui::dynamic_icon<plusminus_icon>& self, window_type & win, world_state & ws) {
		if(win.modifier < 0.0f)
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, 1ui32);
	}
	template<typename window_type>
	void school_item_icon::windowed_update(ui::dynamic_icon<school_item_icon>& self, window_type &win, world_state & ws) {
		self.set_frame(ws.w.gui_m, win.index);
	}

	template<typename W>
	void school_item_base::on_create(W & w, world_state &) {
		associated_object->size = ui::xy_pair{ 36i16, 32i16 };
	}

	template<typename lb_type>
	void school_modifiers_lb::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto ts = player->tech_school; is_valid_index(ts)) {
				auto milmod = ws.s.modifiers_m.national_modifier_definitions[ts][modifiers::national_offsets::army_tech_research_bonus];
				auto navmod = ws.s.modifiers_m.national_modifier_definitions[ts][modifiers::national_offsets::navy_tech_research_bonus];
				auto commod = ws.s.modifiers_m.national_modifier_definitions[ts][modifiers::national_offsets::commerce_tech_research_bonus];
				auto culmod = ws.s.modifiers_m.national_modifier_definitions[ts][modifiers::national_offsets::culture_tech_research_bonus];
				auto indmod = ws.s.modifiers_m.national_modifier_definitions[ts][modifiers::national_offsets::industry_tech_research_bonus];

				if(milmod != 0.0f)
					lb.add_item(ws, std::tuple<float, uint32_t, text_data::text_tag>(
						milmod, 0ui32, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::army_tech_research_bonus]));
				if(navmod != 0.0f)
					lb.add_item(ws, std::tuple<float, uint32_t, text_data::text_tag>(
						navmod, 1ui32, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::navy_tech_research_bonus]));
				if(commod != 0.0f)
					lb.add_item(ws, std::tuple<float, uint32_t, text_data::text_tag>(
						commod, 2ui32, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::commerce_tech_research_bonus]));
				if(culmod != 0.0f)
					lb.add_item(ws, std::tuple<float, uint32_t, text_data::text_tag>(
						culmod, 3ui32, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::culture_tech_research_bonus]));
				if(indmod != 0.0f)
					lb.add_item(ws, std::tuple<float, uint32_t, text_data::text_tag>(
						indmod, 4ui32, ws.s.modifiers_m.national_offset_names[modifiers::national_offsets::industry_tech_research_bonus]));
			}
		}
	}

	template<typename window_type>
	void invention_category_icon::windowed_update(ui::dynamic_icon<invention_category_icon>& self, window_type& win, world_state & ws) {
		if(is_valid_index(win.invention)) {
			auto invention_cat = ws.s.technology_m.technologies_container[win.invention].category;
			if(invention_cat == tech_category_type::army)
				self.set_frame(ws.w.gui_m, 0ui32);
			if(invention_cat == tech_category_type::navy)
				self.set_frame(ws.w.gui_m, 1ui32);
			if(invention_cat == tech_category_type::commerce)
				self.set_frame(ws.w.gui_m, 2ui32);
			if(invention_cat == tech_category_type::culture)
				self.set_frame(ws.w.gui_m, 3ui32);
			if(invention_cat == tech_category_type::industry)
				self.set_frame(ws.w.gui_m, 4ui32);
		}
	}

	template<typename window_type>
	void invention_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.invention)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.technology_m.technologies_container[win.invention].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void invention_item_percent::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		invention = win.invention;
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(invention)) {
			auto chance = get_invention_chance(invention, ws, *player);
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::percent, chance);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}

	template<typename lb_type>
	void invention_lb::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<std::pair<tech_tag, float>, 64, concurrent_allocator<std::pair<tech_tag, float>>> data;

		if(auto player = ws.w.local_player_nation; player) {
			if(auto pid = player->id; ws.w.nation_s.nations.is_valid_index(pid)) {
				for(auto i : ws.s.technology_m.inventions) {
					if(bit_vector_test(ws.w.nation_s.active_technologies.get_row(pid), to_index(i)) == false &&
						(!is_valid_index(ws.s.technology_m.technologies_container[i].allow) ||
							triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(ws.s.technology_m.technologies_container[i].allow), ws, player, player, nullptr, nullptr))) {
						data.emplace_back(i, get_invention_chance(i, ws, *player));
					}
				}
			}
		}

		switch(ws.w.techui_invention_sort) {
			case current_state::invention_sort::none: break;
			case current_state::invention_sort::chance:
				std::sort(data.begin(), data.end(), [&ws](std::pair<tech_tag, float> a, std::pair<tech_tag, float> b) {
					return a.second > b.second;
				});
				break;
			case current_state::invention_sort::name:
			{
				vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
				std::sort(data.begin(), data.end(), [&ws, &lss](std::pair<tech_tag, float> a, std::pair<tech_tag, float> b) {
					auto a_name = ws.s.technology_m.technologies_container[a.first].name;
					auto b_name = ws.s.technology_m.technologies_container[b.first].name;
					return lss(
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
				});
				break;
			}
			case current_state::invention_sort::type:
				std::sort(data.begin(), data.end(), [&ws](std::pair<tech_tag, float> a, std::pair<tech_tag, float> b) {
					return int32_t(ws.s.technology_m.technologies_container[a.first].category) < int32_t(ws.s.technology_m.technologies_container[b.first].category);
				});
				break;
			default: break;
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}
}
