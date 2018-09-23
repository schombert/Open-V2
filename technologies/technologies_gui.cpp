#include "common\\common.h"
#include "technologies_gui.hpp"
#include "world_state\\world_state.h"
#include "technologies\\technologies_functions.h"

namespace technologies {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.hide_tech_window();
	}

	void tech_school_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto ts = player->tech_school;
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
			if(auto tech = player->current_research; is_valid_index(tech)) {
				if(auto total_points = effective_tech_cost(tech, ws, *player); total_points != 0.0f) {
					bar.set_fraction(std::min(1.0f, float(player->research_points) / total_points));
					return;
				}
			}
		}
		bar.set_fraction(0.0f);
	}
	void research_name_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto cr = player->current_research;
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
			if(auto cr = player->current_research; is_valid_index(cr)) {
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
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.technology_m.technology_subcategories[tsc].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					}
					lm.finish_current_line();
				}
			}
		}
	}
}