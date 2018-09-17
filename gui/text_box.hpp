#pragma once
#include "gui.hpp"
#include "gui.h"
#undef max
#undef min

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


template<typename BASE, int32_t x_size_adjust, int32_t y_size_adjust>
void ui::multiline_text<BASE, x_size_adjust, y_size_adjust>::update_data(gui_object_tag, world_state& w) {
	if constexpr(ui::detail::has_update < BASE, tagged_gui_object, line_manager &, ui::text_format&, world_state& > ) {
		tagged_gui_object content_frame{ w.w.gui_m.gui_objects.at(this->scrollable_region), this->scrollable_region};

		ui::clear_children(w.w.gui_m, content_frame);
		line_manager lm(align, outer_width);

		BASE::update(content_frame, lm, format, w);
		lm.finish_current_line();

		int32_t max_height = 1;
		ui::for_each_child(w.w.gui_m, content_frame, [&max_height](tagged_gui_object c) { max_height = std::max(max_height, c.object.size.y + c.object.position.y); });
		content_frame.object.size.y = int16_t(max_height);

		if(max_height > outer_height) {
			sb.set_range(w.w.gui_m, 0, max_height - outer_height);
			sb.update_position(std::min(max_height - outer_height, sb.position()));
			content_frame.object.position.y = int16_t(-sb.position());
			ui::make_visible_immediate(*sb.associated_object);
		} else {
			sb.set_range(w.w.gui_m, 0, 0);
			content_frame.object.position.y = 0i16;
			ui::hide(*sb.associated_object);
		}
	}
}

template<typename BASE, int32_t x_size_adjust, int32_t y_size_adjust>
bool ui::multiline_text<BASE, x_size_adjust, y_size_adjust>::on_scroll(gui_object_tag t, world_state & ws, const scroll & s) {
	if(ws.w.gui_m.gui_objects.at(scrollable_region).size.y > outer_height)
		return sb.on_scroll(t, ws, s);
	else
		return true;
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

template<typename BASE, int32_t x_size_adjust, int32_t y_size_adjust>
template<typename window_type>
void ui::multiline_text<BASE, x_size_adjust, y_size_adjust>::windowed_update(window_type& win, world_state& w) {
	if constexpr(ui::detail::has_windowed_update < BASE, window_type&, tagged_gui_object, line_manager &, ui::text_format&, world_state& >) {
		tagged_gui_object content_frame{ w.w.gui_m.gui_objects.at(scrollable_region), scrollable_region };

		ui::clear_children(w.w.gui_m, content_frame);
		line_manager lm(align, content_frame.object.size.x);

		BASE::windowed_update(win, content_frame, lm, format, w);
		lm.finish_current_line();

		int32_t max_height = 1;
		ui::for_each_child(w.w.gui_m, content_frame, [&max_height](tagged_gui_object c) { max_height = std::max(max_height, c.object.size.y + c.object.position.y); });
		content_frame.object.size.y = int16_t(max_height);

		if(max_height > outer_height) {
			sb.set_range(w.w.gui_m, 0, max_height - outer_height);
			sb.update_position(std::min(max_height - outer_height, sb.position()));
			content_frame.object.position.y = int16_t(-sb.position());
			ui::make_visible_immediate(*sb.associated_object);
		} else {
			content_frame.object.position.y = 0i16;
			ui::hide(*sb.associated_object);
		}
	}
}

template<typename BASE, int32_t x_size_adjust, int32_t y_size_adjust>
void ui::multiline_text<BASE, x_size_adjust, y_size_adjust>::create_sub_elements(tagged_gui_object self, world_state& ws) {
	const auto inner_area = ws.w.gui_m.gui_objects.emplace();
	
	inner_area.object.position = ui::xy_pair{ 0, 0 };
	inner_area.object.size = ui::xy_pair{ static_cast<int16_t>(associated_object->size.x - 16), 1i16 };

	scrollable_region = inner_area.id;

	ui::add_to_back(ws.w.gui_m, self, inner_area);

	ui::create_static_fixed_sz_scrollbar(
		ws,
		ws.s.gui_m.ui_definitions.standardlistbox_slider,
		self,
		ui::xy_pair{ static_cast<int16_t>(associated_object->size.x - 16), 0i16 },
		associated_object->size.y,
		sb);

	sb.associate(&inner_area.object);
	ui::hide(*(sb.associated_object));
}

template<typename B, int32_t x_size_adjust, int32_t y_size_adjust>
ui::tagged_gui_object ui::create_static_element(world_state& ws, ui::text_tag handle, tagged_gui_object parent, multiline_text<B, x_size_adjust, y_size_adjust>& b) {
	const ui::text_def& text_def = ws.s.gui_m.ui_definitions.text[handle];

	const auto[font_h, is_black, int_font_size] = graphics::unpack_font_handle(text_def.font_handle);
	b.set_format(
		ui::text_aligment_from_text_definition(text_def),
		text_format{ is_black ? ui::text_color::black : ui::text_color::white, font_h, int_font_size });

	const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

	new_gobj.object.associated_behavior = &b;
	b.associated_object = &new_gobj.object;

	new_gobj.object.position = text_def.position;
	new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(text_def.max_width + x_size_adjust), static_cast<int16_t>(text_def.max_height + y_size_adjust) };
	new_gobj.object.align = alignment_from_definition(text_def);

	b.set_height(new_gobj.object.size.y);

	b.create_sub_elements(new_gobj, ws);

	ui::add_to_back(ws.w.gui_m, parent, new_gobj);

	if constexpr(ui::detail::has_on_create<multiline_text<B, x_size_adjust, y_size_adjust>, multiline_text<B, x_size_adjust, y_size_adjust>&, world_state&>)
		b.on_create(b, ws);

	ws.w.gui_m.flag_minimal_update();
	return new_gobj;
}

template<typename BASE, int32_t y_adjust>
ui::tooltip_behavior ui::display_text<BASE, y_adjust>::has_tooltip(gui_object_tag, world_state& ws, const mouse_move&) {
	if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>)
		return BASE::has_tooltip(ws) ? tooltip_behavior::tooltip : tooltip_behavior::no_tooltip;
	else
		return tooltip_behavior::no_tooltip;
}

template<typename BASE, int32_t y_adjust>
void ui::display_text<BASE, y_adjust>::create_tooltip(gui_object_tag, world_state& ws, const mouse_move&, tagged_gui_object tw) {
	if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>)
		BASE::create_tooltip(ws, tw);
}

template<typename BASE, int32_t y_adjust>
void ui::display_text<BASE, y_adjust>::set_visibility(gui_manager& m, bool visible) {
	if(visible)
		ui::make_visible_and_update(m, *associated_object);
	else
		ui::hide(*associated_object);
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

	if constexpr(ui::detail::has_on_create<display_text<B, y_adjust>, display_text<B, y_adjust>&, world_state&>)
		b.on_create(b, ws);

	ws.w.gui_m.flag_minimal_update();
	return new_gobj;
}
