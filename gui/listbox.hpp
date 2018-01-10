#pragma once
#include "gui.hpp"

#ifdef _DEBUG
#include <Windows.h>
#endif

template<typename BASE, typename ELEMENT, int32_t left_expand>
bool ui::display_listbox<BASE, ELEMENT, left_expand>::on_scroll(gui_object_tag o, gui_manager& m, const scroll& s) {
	return sb.on_scroll(o, m, s);
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::update_data(gui_object_tag, gui_manager& manager, world_state& w) {
	clear_items(manager);
	BASE::populate_list(*this, manager, w);
	update_scroll_position(manager);
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
template<typename window_type>
void ui::display_listbox<BASE, ELEMENT, left_expand>::windowed_update(window_type& w, gui_manager& m, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, display_listbox<BASE, ELEMENT, left_expand>&, window_type&, gui_manager&, world_state&>) {
		clear_items(m);
		BASE::windowed_update(*this, w, m, s);
		BASE::populate_list(*this, m, w);
		update_scroll_position(m);
	}
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::set_element_definition(gui_manager& manager) {
	const auto t = BASE::element_tag(manager);
	element_def_tag = t;
	element_def = &manager.ui_definitions.windows[t];
	sb.factor = element_def->size.y;
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::clear_items(gui_manager& manager) {
	ui::clear_children(manager, tagged_gui_object{*_content_frame, _content_frame_tag});
	contents.clear();
	ui::hide(*(sb.associated_object));
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::update_scroll_position(gui_manager& manager) {
	const int32_t extra = std::max(0, static_cast<int32_t>(_content_frame ->size.y - associated_object->size.y));

	if (sb.position() * sb.factor > (extra + sb.factor - 1)) {
		sb.update_position((extra + sb.factor - 1) / sb.factor);
	}
	sb.set_range(manager, 0, (extra + sb.factor - 1) / sb.factor);
	if (extra > 0) {
		ui::make_visible_and_update(manager, *(sb.associated_object));
	}
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
template<typename ... PARAMS>
void ui::display_listbox<BASE, ELEMENT, left_expand>::add_item(gui_manager& manager, PARAMS&& ... params) {
	contents.emplace_back(std::forward<PARAMS>(params)...);
	auto& n = contents.back();

	create_static_element(manager, element_def_tag, tagged_gui_object{ *_content_frame, _content_frame_tag }, n);

	if constexpr(left_expand != 0)
		n.associated_object->flags.fetch_or(gui_object::dont_clip_children, std::memory_order_acq_rel);

	n.associated_object->position.x += static_cast<int16_t>(left_expand);
	n.associated_object->position.y = static_cast<int16_t>(element_def->size.y * static_cast<int32_t>((contents.size() - 1)));
	_content_frame->size.y = static_cast<int16_t>(element_def->size.y * static_cast<int32_t>(contents.size()));
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::create_sub_elements(tagged_gui_object self, gui_manager& manager) {
	set_element_definition(manager);

	const auto inner_area = manager.gui_objects.emplace();

	inner_area.object.position = ui::xy_pair{ 0, 0 };
	inner_area.object.size = ui::xy_pair{ static_cast<int16_t>(associated_object->size.x - 16), 1i16 };

	_content_frame = &inner_area.object;
	_content_frame_tag = inner_area.id;

	ui::add_to_back(manager, self, inner_area);

	ui::create_static_fixed_sz_scrollbar(
		manager,
		manager.ui_definitions.standardlistbox_slider,
		self,
		ui::xy_pair{ static_cast<int16_t>(associated_object->size.x - 16), 0i16 },
		associated_object->size.y,
		sb);

	sb.associate(&inner_area.object);
	ui::hide(*(sb.associated_object));
}

template<typename B, typename ELEMENT, int32_t left_expand>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, listbox_tag handle, tagged_gui_object parent, ui::display_listbox<B, ELEMENT, left_expand>& b) {
	const ui::listbox_def& definition = manager.ui_definitions.listboxes[handle];

	const auto new_obj = manager.gui_objects.emplace();

	new_obj.object.position = definition.position;
	new_obj.object.size = definition.size;

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	if (is_valid_index(definition.background_handle)) {
		ui::detail::instantiate_graphical_object(manager, new_obj, definition.background_handle);
	}

	new_obj.object.position.x -= static_cast<int16_t>(left_expand);
	new_obj.object.size.x += static_cast<int16_t>(left_expand + 16);

	b.create_sub_elements(new_obj, manager);

	ui::add_to_back(manager, parent, new_obj);
	manager.flag_minimal_update();
	return new_obj;
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::set_self_information(gui_manager& m, gui_object_tag s, int32_t sp, text_data::alignment a) {
	const auto t = BASE::element_tag(m);
	element_def_tag = t;
	subelement_width = m.ui_definitions.get(t).size.x;

	self = s;
	spacing = sp;
	subelement_alignment = a;
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
void  ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::clear_items(gui_manager& manager) {
	if (is_valid_index(self)) {
		ui::clear_children(manager, tagged_gui_object{ *associated_object, self });
	}
	contents.clear();
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
template<typename ... PARAMS>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::add_item(gui_manager& manager, PARAMS&& ... params) {
	if (is_valid_index(self) && is_valid_index(element_def_tag)) {
		contents.emplace_back(std::forward<PARAMS>(params)...);
		auto& n = contents.back();
		ui::create_static_element(manager, element_def_tag, tagged_gui_object{ *associated_object, self }, n);

		if constexpr(vertical_extension != 0)
			n.associated_object->flags.fetch_or(gui_object::dont_clip_children, std::memory_order_acq_rel);
	}
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::update_item_positions(gui_manager&) {
	const int32_t amount = static_cast<int32_t>(contents.size());
	if (amount == 0)
		return;

	const int32_t space_consumed = (subelement_width + spacing) * amount - spacing;
	const int32_t overlap = space_consumed > associated_object->size.x ? (space_consumed - associated_object->size.x + amount - 1) / amount: 0;
	const int32_t adjusted_space_consumed = space_consumed - overlap * amount;
	const int32_t base_offset =
		subelement_alignment == text_data::alignment::left ?
		0 :
		(subelement_alignment == text_data::alignment::right ?
		    (associated_object->size.x - adjusted_space_consumed * amount) :
			(associated_object->size.x - adjusted_space_consumed * amount) / 2);

	for (int32_t n = amount - 1; n >= 0; -- n) {
		contents[static_cast<uint32_t>(n)].associated_object->position.x = static_cast<int16_t>(base_offset + (subelement_width + spacing - overlap) * n);
	}
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
template<typename window_type>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::windowed_update(window_type& w, gui_manager& m, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, overlap_box<BASE, tag_type, ELEMENT, vertical_extension>&, window_type&, gui_manager&, world_state&>) {
		if (is_valid_index(element_def_tag)) {
			clear_items(m);
			BASE::windowed_update(*this, w, m, s);
			BASE::populate_list(*this, m, w);
			update_item_positions(m);
		}
	}
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::update_data(gui_object_tag, gui_manager& manager, world_state& s) {
	if (is_valid_index(element_def_tag)) {
		clear_items(manager);
		BASE::populate_list(*this, manager, s);
		update_item_positions(manager);
	}
}

template<typename B, typename tag_type, typename ELEMENT, int32_t vertical_extension>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, overlapping_region_tag handle, tagged_gui_object parent, ui::overlap_box<B, tag_type, ELEMENT, vertical_extension>& b) {
	const ui::overlapping_region_def& definition = manager.ui_definitions.get(handle);

	const auto new_obj = manager.gui_objects.emplace();

	new_obj.object.position = definition.position;
	new_obj.object.size = ui::xy_pair{ definition.size.x, static_cast<int16_t>(definition.size.y + vertical_extension) };

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	b.set_self_information(manager, new_obj.id, static_cast<int32_t>(definition.spacing), ui::text_aligment_from_overlapping_definition(definition));

	ui::add_to_back(manager, parent, new_obj);
	manager.flag_minimal_update();
	return new_obj;
}
