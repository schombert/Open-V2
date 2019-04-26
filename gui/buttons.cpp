#include "buttons.hpp"
#include "gui.hpp"
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"
#include "world_state\\world_state.h"

bool ui::button_group_member::on_lclick(gui_object_tag , world_state& m, const lbutton_down&) {
	sound::play_interface_sound(m, m.s.sound_m.click_sound);
	group->select(m, _index);
	return true;
}

bool ui::button_group_member::on_keydown(gui_object_tag , world_state& m, const key_down& k) {
	if (k.keycode == shortcut) {
		sound::play_interface_sound(m, m.s.sound_m.click_sound);
		group->select(m, _index);
		return true;
	} else {
		return false;
	}
}

ui::tooltip_behavior ui::button_group_member::has_tooltip(gui_object_tag , world_state& , const mouse_move& ) {
	return group->has_tooltip(_index);
}

void ui::button_group_member::create_tooltip(gui_object_tag , world_state& ws, const mouse_move& , tagged_gui_object tw) {
	group->create_tooltip(ws, tw, _index);
}

void ui::button_group_member::set_group(button_group_common_base* g, uint32_t i) {
	group = g;
	_index = i;
}

ui::tagged_gui_object ui::create_static_element(world_state& ws, button_tag handle, tagged_gui_object parent, button_group_member& b) {
	auto new_obj = ui::detail::create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	auto& bdef = ws.s.gui_m.ui_definitions.buttons[handle];
	b.shortcut = bdef.shortcut;

	ui::add_to_back(ws.w.gui_m, parent, new_obj);
	ws.w.gui_m.flag_minimal_update();
	return new_obj;
}
