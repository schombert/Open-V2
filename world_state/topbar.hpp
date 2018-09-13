#pragma once
#include "common\\common.h"
#include "topbar.h"
#include "world_state\\world_state.h"

namespace current_state {
	template<typename W>
	void topbar_base::on_create(W& w, world_state& ws) {
		w.associated_object->size.y = 160i16;
		w.associated_object->size.x = int16_t(ws.w.gui_m.width());
	}

	template<int32_t nth>
	inline void top_produced_icon<nth>::update(ui::dynamic_icon<top_produced_icon<nth>>& self, world_state & ws) {
		if(!is_valid_index(tag))
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[tag].icon);
	}

	template<int32_t nth>
	inline void top_produced_icon<nth>::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tag))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[tag].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	template<int32_t nth>
	inline void top_imported_icon<nth>::update(ui::dynamic_icon<top_imported_icon<nth>>& self, world_state & ws) {
		if(!is_valid_index(tag))
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[tag].icon);
	}

	template<int32_t nth>
	inline void top_imported_icon<nth>::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tag))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[tag].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	template<int32_t nth>
	inline void top_exported_icon<nth>::update(ui::dynamic_icon<top_exported_icon<nth>>& self, world_state & ws) {
		if(!is_valid_index(tag))
			self.set_frame(ws.w.gui_m, 0ui32);
		else
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[tag].icon);
	}

	template<int32_t nth>
	inline void top_exported_icon<nth>::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(is_valid_index(tag))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[tag].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	template<typename lb_type>
	void war_against_lb::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto against_range = get_range(ws.w.nation_s.nations_arrays, player->opponents_in_war);
			for(auto against : against_range) {
				if(is_valid_index(against) && is_valid_index(ws.w.nation_s.nations[against].tag)) {
					lb.add_item(ws, ws.w.nation_s.nations[against].tag);
				}
			}
		}
	}
}
