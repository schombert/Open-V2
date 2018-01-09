#pragma once
#include "gui.hpp"

template<typename BASE>
void ui::dynamic_icon<BASE>::update_data(gui_object_tag, gui_manager& m, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, dynamic_icon<BASE>&, gui_manager&, world_state&>) {
		BASE::update(*this, m, w);
	}
}

template<typename BASE>
ui::tooltip_behavior ui::dynamic_icon<BASE>::has_tooltip(gui_object_tag, gui_manager&, const mouse_move&) {
	if constexpr(ui::detail::has_has_tooltip<BASE>)
		return BASE::has_tooltip() ? tooltip_behavior::tooltip : tooltip_behavior::no_tooltip;
	else
		return tooltip_behavior::no_tooltip;
}

template<typename BASE>
void ui::dynamic_icon<BASE>::create_tooltip(gui_object_tag, gui_manager& m, const mouse_move&, tagged_gui_object tw) {
	if constexpr(ui::detail::has_has_tooltip<BASE>)
		BASE::create_tooltip(m, tw);
}

template<typename BASE>
void ui::dynamic_icon<BASE>::set_frame(gui_manager& m, uint32_t frame_num) {
	if (const auto go = m.graphics_instances.safe_at(graphics_instance_tag(associated_object->type_dependant_handle)); go) {
		go->frame = static_cast<int32_t>(frame_num);
	}
}

template<typename BASE>
void ui::dynamic_icon<BASE>::set_visibility(gui_manager& m, bool visible) {
	if (visible)
		ui::make_visible_and_update(m, *associated_object);
	else
		ui::hide(*associated_object);
}

template<typename B>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, icon_tag handle, tagged_gui_object parent, dynamic_icon<B>& b) {
	auto new_obj = ui::detail::create_element_instance(manager, handle);

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	ui::add_to_back(manager, parent, new_obj);
	manager.flag_minimal_update();
	return new_obj;
}
