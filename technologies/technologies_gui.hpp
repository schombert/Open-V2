#pragma once
#include "common\\common.h"
#include "technologies_gui.h"
#include "gui\\gui.hpp"

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
}
