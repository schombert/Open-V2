#include "buttons.hpp"
#include "gui.hpp"
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"

bool ui::button_group_member::on_lclick(gui_object_tag o, gui_manager& m, const lbutton_down&) {
	group->select(m, _index);
	return true;
}

bool ui::button_group_member::on_keydown(gui_object_tag o, gui_manager& m, const key_down& k) {
	if (k.keycode == shortcut) {
		group->select(m, _index);
		return true;
	} else {
		return false;
	}
}

ui::tooltip_behavior ui::button_group_member::has_tooltip(gui_object_tag o, gui_manager& m, const mouse_move& mm) {
	return group->has_tooltip(_index);
}

void ui::button_group_member::create_tooltip(gui_object_tag o, gui_manager& m, const mouse_move& mm, tagged_gui_object tw) {
	group->create_tooltip(m, tw, _index);
}

void ui::button_group_member::set_group(button_group_common_base* g, uint32_t i) {
	group = g;
	_index = i;
}

ui::tagged_gui_object ui::create_static_element(gui_manager& manager, button_tag handle, tagged_gui_object parent, button_group_member& b) {
	auto new_obj = ui::detail::create_element_instance(manager, handle);

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	auto& bdef = manager.ui_definitions.buttons[handle];
	b.shortcut = bdef.shortcut;

	ui::add_to_back(manager, parent, new_obj);

	return new_obj;
}