#pragma once
#include "gui.hpp"

template<typename BASE, typename ELEMENT, int32_t left_expand>
bool ui::display_listbox<BASE, ELEMENT, left_expand>::on_scroll(gui_object_tag o, gui_manager& m, const scroll& s) {
	return sb.on_scroll(o, m, s);
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::update_data(gui_object_tag o, gui_manager& manager, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, display_listbox<BASE, ELEMENT, left_expand>&, gui_object_tag, gui_manager&, world_state&>) {
		if (is_valid_index(element_def_tag)) {
			clear_items(manager);
			BASE::update(*this, o, manager, w);
			update_scroll_position(manager);
			for (auto& i : contents)
				i.member_update_in_window(i, manager, w);
		}
	}
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
template<typename window_type>
void ui::display_listbox<BASE, ELEMENT, left_expand>::windowed_update(window_type& w, gui_manager& m, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, display_listbox<BASE, ELEMENT, left_expand>&, window_type&, gui_manager&, world_state&>) {
		if (is_valid_index(element_def_tag)) {
			clear_items(m);
			BASE::windowed_update(*this, w, m, s);
			update_scroll_position(m);
			for (auto& i : contents)
				i.member_update_in_window(i, m, s);
		}
	}
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::set_element_definition(gui_manager& manager, window_tag t) {
	element_def_tag = t;
	element_def = &manager.ui_definitions.windows[t];
	sb.factor = element_def->size.y;
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::clear_items(gui_manager& manager) {
	if (_content_frame) {
		ui::clear_children(manager, tagged_gui_object{*_content_frame, _content_frame_tag});
	}
	contents.clear();
	sb.associated_object->flags.fetch_and(static_cast<uint16_t>(~ui::gui_object::visible), std::memory_order_acq_rel);
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::update_scroll_position(gui_manager& manager) {
	const int32_t extra = _content_frame ? std::max(0, static_cast<int32_t>(_content_frame ->size.y - associated_object->size.y)) : 0;

	if (sb.position() * sb.factor > (extra + sb.factor - 1)) {
		sb.update_position((extra + sb.factor - 1) / sb.factor);
	}
	sb.set_range(manager, 0, (extra + sb.factor - 1) / sb.factor);
	if (extra > 0) {
		sb.associated_object->flags.fetch_or(ui::gui_object::visible, std::memory_order_acq_rel);
	}
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
template<typename ... PARAMS>
void ui::display_listbox<BASE, ELEMENT, left_expand>::add_item(gui_manager& manager, PARAMS&& ... params) {
	if (_content_frame && element_def) {
		contents.emplace_back(std::forward<PARAMS>(params)...);
		auto& n = contents.back();

		create_static_element(manager, element_def_tag, tagged_gui_object{ *_content_frame, _content_frame_tag }, n);

		if constexpr(left_expand != 0)
			n.associated_object->flags.fetch_or(gui_object::dont_clip_children, std::memory_order_acq_rel);

		n.associated_object->position.x += static_cast<int16_t>(left_expand);
		n.associated_object->position.y = static_cast<int16_t>(element_def->size.y * static_cast<int32_t>((contents.size() - 1)));
		_content_frame->size.y = static_cast<int16_t>(element_def->size.y * static_cast<int32_t>(contents.size()));
	}
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::create_sub_elements(tagged_gui_object self, gui_manager& manager) {
	
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
	sb.associated_object->flags.fetch_and(static_cast<uint16_t>(~ui::gui_object::visible), std::memory_order_acq_rel);
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

	return new_obj;
}
