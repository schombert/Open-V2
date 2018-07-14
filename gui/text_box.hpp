#pragma once
#include "gui.hpp"

template<typename BASE, int32_t y_adjust>
void ui::display_text<BASE, y_adjust>::update_data(gui_object_tag, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, tagged_gui_object, text_data::alignment, ui::text_format&, world_state&>) {
		ui::clear_children(w.w.gui_m, tagged_gui_object{ *associated_object, self });
		BASE::update(tagged_gui_object{ *associated_object, self }, align, format, w);
	} else if constexpr(ui::detail::has_update<BASE, tagged_gui_object, text_box_line_manager&, ui::text_format&, world_state&>) {
		ui::clear_children(w.w.gui_m, tagged_gui_object{ *associated_object, self });
		text_box_line_manager lm(align, associated_object->size.x, border_x, border_y);
		BASE::update(tagged_gui_object{ *associated_object, self }, lm, format, w);
	}
}

template<typename BASE, int32_t y_adjust>
template<typename window_type>
void ui::display_text<BASE, y_adjust>::windowed_update(window_type& w, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, window_type&, tagged_gui_object, text_data::alignment, ui::text_format&, world_state&>) {
		ui::clear_children(s.w.gui_m, tagged_gui_object{ *associated_object, self });
		BASE::windowed_update(w, tagged_gui_object{ *associated_object, self }, align, format, s);
	} if constexpr(ui::detail::has_windowed_update<BASE, window_type&, tagged_gui_object, text_box_line_manager&, ui::text_format&, world_state&>) {
		ui::clear_children(s.w.gui_m, tagged_gui_object{ *associated_object, self });
		text_box_line_manager lm(align, associated_object->size.x, border_x, border_y);
		BASE::windowed_update(w, tagged_gui_object{ *associated_object, self }, lm, format, s);
	}
}

template<typename B, int32_t y_adjust>
ui::tagged_gui_object ui::create_static_element(world_state& ws, ui::text_tag handle, tagged_gui_object parent, display_text<B, y_adjust>& b) {
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
	new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(text_def.max_width), static_cast<int16_t>(text_def.max_height + y_adjust) };
	new_gobj.object.align = alignment_from_definition(text_def);
	
	b.border_x = text_def.border_size.x;

	graphics::font& this_font = ws.s.gui_m.fonts.at(font_h);
	b.border_y = (int32_t(text_def.max_height + y_adjust) - int32_t(this_font.line_height(ui::detail::font_size_to_render_size(this_font, static_cast<int32_t>(int_font_size))) + 0.5f)) / 2;

	ui::add_to_back(ws.w.gui_m, parent, new_gobj);
	ws.w.gui_m.flag_minimal_update();
	return new_gobj;
}
