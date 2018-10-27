#pragma once
#include "common\\common.h"
#include "gui.hpp"

namespace ui {
	template<typename BASE>
	uint8_t* display_barchart<BASE>::data(gui_manager& manager) {
		if(auto dt = manager.data_textures.safe_at(data_texture_tag(associated_object->type_dependant_handle)); dt) {
			return dt->data();
		} else {
			return nullptr;
		}
	}
	template<typename BASE>
	void display_barchart<BASE>::update_display(gui_manager& manager) const {
		if(auto dt = manager.data_textures.safe_at(data_texture_tag(associated_object->type_dependant_handle)); dt) {
			return dt->data_ready();
		}
	}

	template<typename BASE>
	void display_barchart<BASE>::update_data(gui_object_tag, world_state& w) {
		if constexpr(ui::detail::has_update<BASE, ui::display_barchart<BASE>&, world_state&>) {
			BASE::update(*this, w);
			update_display(w.w.gui_m);
		}
	}

	template<typename BASE>
	template<typename window_type>
	std::enable_if_t<ui::detail::has_windowed_update<BASE, display_barchart<BASE>&, window_type&, world_state&>, void> display_barchart<BASE>::windowed_update(window_type& w, world_state& s) {
		if constexpr(ui::detail::has_windowed_update<BASE, display_barchart<BASE>&, window_type&, world_state&>) {
			BASE::windowed_update(*this, w, s);
			update_display(s.w.gui_m);
		}
	}

	template<typename BASE>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, display_barchart<BASE>& b) {
		const ui::icon_def& icon_def = ws.s.gui_m.ui_definitions.icons[handle];
		const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

		const uint16_t rotation =
			(icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			ui::gui_object::rotation_upright :
			((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);

		new_gobj.object.position = icon_def.position;
		new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		new_gobj.object.align = alignment_from_definition(icon_def);
		

		auto& graphic_object_def = ws.s.gui_m.graphics_object_definitions.definitions[icon_def.graphical_object_handle];
		new_gobj.object.size = ui::xy_pair{ graphic_object_def.size.x, graphic_object_def.size.y };

		new_gobj.object.flags.fetch_or(ui::gui_object::type_barchart, std::memory_order_acq_rel);
		const auto new_dt = ws.w.gui_m.data_textures.emplace(graphic_object_def.size.x, 4ui16);
		new_gobj.object.type_dependant_handle.store(to_index(new_dt.id), std::memory_order_release);

		if(rotation == ui::gui_object::rotation_right) {
			new_gobj.object.position = ui::xy_pair{
				int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		}

		new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);

		new_gobj.object.associated_behavior = &b;
		b.associated_object = &new_gobj.object;
		b.set_size(graphic_object_def.size.x);

		auto data = b.data(ws.w.gui_m);
		memset(data, 0, graphic_object_def.size.x * 4);
		b.update_display(ws.w.gui_m);

		ui::add_to_back(ws.w.gui_m, parent, new_gobj);
		ws.w.gui_m.flag_minimal_update();
		return new_gobj;
	}
}
