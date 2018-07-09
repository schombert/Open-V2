#pragma once
#include "gui.hpp"

template<typename BASE>
void ui::display_text<BASE>::update_data(gui_object_tag, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, tagged_gui_object, text_data::alignment, ui::text_format&, world_state&>) {
		ui::clear_children(w.w.gui_m, tagged_gui_object{ *associated_object, self });
		BASE::update(tagged_gui_object{ *associated_object, self }, align, format, w);
	}
}

template<typename BASE>
template<typename window_type>
void ui::display_text<BASE>::windowed_update(window_type& w, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, window_type&, tagged_gui_object, text_data::alignment, ui::text_format&, world_state&>) {
		ui::clear_children(s.w.gui_m, tagged_gui_object{ *associated_object, self });
		BASE::windowed_update(w, tagged_gui_object{ *associated_object, self }, align, format, s);
	}
}

template<typename B>
ui::tagged_gui_object ui::create_static_element(world_state& ws, ui::text_tag handle, tagged_gui_object parent, display_text<B>& b) {
	const ui::text_def& text_def = ws.s.gui_m.ui_definitions.text[handle];

	const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(text_def.font_handle);
	b.set_format(
		ui::text_aligment_from_text_definition(text_def),
		text_format{ is_black ? ui::text_color::black : ui::text_color::white, font_h, int_font_size });
	
	const auto new_gobj = ui::detail::create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);

	b.set_self(new_gobj.id);

	new_gobj.object.associated_behavior = &b;
	b.associated_object = &new_gobj.object;

	new_gobj.object.position = text_def.position;
	new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(text_def.max_width), static_cast<int16_t>(text_def.max_height) };
	new_gobj.object.align = alignment_from_definition(text_def);

	ui::add_to_back(ws.w.gui_m, parent, new_gobj);
	ws.w.gui_m.flag_minimal_update();
	return new_gobj;
}
