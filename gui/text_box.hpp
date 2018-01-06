#pragma once

template<typename BASE>
void ui::display_text<BASE>::update_data(gui_object_tag o, gui_manager& m, world_state& w) {
	ui::clear_children(manager, tagged_gui_object{ *associated_object, o });
	BASE::update(tagged_gui_object{ *associated_object, o }, format, m, w);
}

template<typename B>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, ui::text_tag handle, tagged_gui_object parent, display_text<B>& b) {
	const auto new_gobj = ui::detail::create_element_instance(manager, handle);
	new_gobj.object.associated_behavior = &b;
	b.associated_object = &new_gobj.object;
	return new_gobj;
}
