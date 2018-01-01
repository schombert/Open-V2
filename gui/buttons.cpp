#include "buttons.hpp"
#include "gui.hpp"
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"

bool ui::button_group_member::on_lclick(tagged_gui_object o, gui_manager& m, const lbutton_down&) {
	group->select(o, m, _index);
	return true;
}

bool ui::button_group_member::on_keydown(tagged_gui_object o, gui_manager& m, const key_down& k) {
	if (k.keycode == shortcut) {
		group->select(o, m, _index);
		return true;
	} else {
		return false;
	}
}

ui::tooltip_behavior ui::button_group_member::has_tooltip(tagged_gui_object o, gui_manager& m, const mouse_move& mm) {
	return group->has_tooltip(o, m, mm, _index);
}

void ui::button_group_member::create_tooltip(tagged_gui_object o, gui_manager& m, const mouse_move& mm, tagged_gui_object tw) {
	group->create_tooltip(o, m, mm, tw, _index);
}

void ui::button_group_member::set_group(button_group_common_base* g, uint32_t i) {
	group = g;
	_index = i;
}

ui::tagged_gui_object ui::create_static_element(gui_manager& manager, button_tag handle, tagged_gui_object parent, button_group_member& b) {
	auto new_obj = ui::detail::create_element_instance(manager, handle);
	new_obj.object.flags.fetch_or(gui_object::static_behavior, std::memory_order_acq_rel);
	new_obj.object.associated_behavior = &b;

	auto& bdef = manager.ui_definitions.buttons[handle];
	b.shortcut = bdef.shortcut;

	ui::add_to_back(manager, parent, new_obj);

	if (auto gp = manager.graphics_instances.safe_at(graphics_instance_tag(new_obj.object.type_dependant_handle)); gp) {
		b.associate(gp);
	}
	return new_obj;
}