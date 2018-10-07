#pragma once
#include "gui.h"

template<typename BASE, int32_t horizontal_resolution>
void ui::linechart<BASE, horizontal_resolution>::update_data(gui_object_tag, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, ui::linechart<BASE, horizontal_resolution>&, world_state&>) {
		BASE::update(*this, w);
	}
}

template<typename BASE, int32_t horizontal_resolution>
template<typename window_type>
std::enable_if_t<ui::detail::has_windowed_update<BASE, ui::linechart<BASE, horizontal_resolution>&, window_type&, world_state&>, void> ui::linechart<BASE, horizontal_resolution>::windowed_update(window_type& w, world_state& s) {
	BASE::windowed_update(*this, w, s);
}

template<typename BASE, int32_t horizontal_resolution>
ui::tooltip_behavior ui::linechart<BASE, horizontal_resolution>::has_tooltip(gui_object_tag, world_state& ws, const mouse_move&) {
	if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>)
		return BASE::has_tooltip(ws) ? tooltip_behavior::variable_tooltip : tooltip_behavior::no_tooltip;
	else
		return tooltip_behavior::no_tooltip;
}

template<typename BASE, int32_t horizontal_resolution>
void ui::linechart<BASE, horizontal_resolution>::create_tooltip(gui_object_tag o, world_state& ws, const mouse_move& mm, tagged_gui_object tw) {
	if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>) {
		auto slot = std::clamp((mm.x * horizontal_resolution) / associated_object->size.x, 0, horizontal_resolution - 1);
		BASE::create_tooltip(ws, tw, slot);
	}
}

template<typename BASE, int32_t horizontal_resolution>
void ui::linechart<BASE, horizontal_resolution>::set_values(ui::gui_manager& manager, float* values) {
	auto l = manager.lines_set.safe_at(lines_tag(associated_object->type_dependant_handle.load(std::memory_order_acquire)));
	if(l)
		l->set_y(values);
}

template<typename BASE, int32_t horizontal_resolution>
ui::tagged_gui_object ui::create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, linechart<BASE, horizontal_resolution>& b) {
	const ui::icon_def& icon_def = ws.s.gui_m.ui_definitions.icons[handle];
	const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

	const uint16_t rotation =
		(icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
		ui::gui_object::rotation_upright :
		((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);

	new_gobj.object.position = icon_def.position;
	new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
	new_gobj.object.align = alignment_from_definition(icon_def);

	ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, horizontal_resolution);

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

	ui::add_to_back(ws.w.gui_m, parent, new_gobj);
	ws.w.gui_m.flag_minimal_update();
	return new_gobj;
}

#include "gui.hpp"
