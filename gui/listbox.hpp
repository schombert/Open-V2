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
void ui::display_listbox<BASE, ELEMENT, left_expand>::set_element_definition(gui_manager& manager, window_tag t) {
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


	b.set_element_definition(manager, b.element_tag(manager));

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

template<typename BASE, typename ELEMENT>
void ui::overlap_box<BASE, ELEMENT>::set_subelement_definition(gui_manager& m, element_tag t) {
	element_def_tag = t;
	subelement_width = m.ui_definitions.get_size(t).x;
	associated_object->flags.fetch_or(gui_object::visible_after_update, std::memory_order_acq_rel);
	manager.flag_minimal_update();
}

template<typename BASE, typename ELEMENT>
void ui::overlap_box<BASE, ELEMENT>::set_subelement_alignment(text_data::alignment a) {
	subelement_alignment = a;
	associated_object->flags.fetch_or(gui_object::visible_after_update, std::memory_order_acq_rel);
	manager.flag_minimal_update();
}

template<typename BASE, typename ELEMENT>
void ui::overlap_box<BASE, ELEMENT>::set_self_information(gui_object_tag s, int32_t sp) {
	self = s;
	spacing = sp;
}

template<typename BASE, typename ELEMENT>
void  ui::overlap_box<BASE, ELEMENT>::clear_items(gui_manager& manager) {
	if (is_valid_index(self)) {
		ui::clear_children(manager, tagged_gui_object{ *associated_object, self });
	}
	contents.clear();
}

template<typename BASE, typename ELEMENT>
template<typename ... PARAMS>
void ui::overlap_box<BASE, ELEMENT>::add_item(gui_manager& manager, PARAMS&& ... params) {
	if (is_valid_index(self) && is_valid_index(element_def_tag)) {
		contents.emplace_back(std::forward<PARAMS>(params)...);
		auto& n = contents.back();

		std::visit([_this = this, &n, &manager](auto tag) {
			if constexpr(ui::detail::can_create<decltype(tag), ELEMENT>)
				ui::create_static_element(manager, tag, tagged_gui_object{ *(_this->associated_object), _this->self }, n);
			else {
#ifdef _DEBUG
				OutputDebugStringA("Unable to instantiate overlapping element: bad tag type\n");
#endif
			}
		}, element_def_tag);
	}
}

template<typename BASE, typename ELEMENT>
void ui::overlap_box<BASE, ELEMENT>::update_item_positions(gui_manager&) {
	const int32_t amount = static_cast<int32_t>(contents.size());
	if (amount == 0)
		return;

	const int32_t space_consumed = (subelement_width + spacing) * amount - spacing;
	const int32_t overlap = space_consumed > associated_object->size.x ? (space_consumed - associated_object->size.x + amount - 1) / amount: 0;
	const int32_t adjusted_space_consumed = space_consumed - overlap;
	const int32_t base_offset =
		subelement_alignment == text_data::alignment::left ?
		0 :
		(subelement_alignment == text_data::alignment::right ?
		    (associated_object->size.x - adjusted_space_consumed * amount) :
			(associated_object->size.x - adjusted_space_consumed * amount) / 2);

	for (int32_t n = amount - 1; n >= 0; -- n) {
		contents[n].associated_object->position.x = base_offset + adjusted_space_consumed * n;
	}
}

namespace ui {
	namespace detail {
		template<typename WIN, typename RET>
		struct has_member_w_update_s : public std::false_type {};
		template<typename WIN>
		struct has_member_w_update_s<WIN, decltype(void(std::declval<WIN>().member_update_in_window(std::declval<WIN&>(), std::declval<gui_manager&>(), std::declval<world_state&>())))> : public std::true_type {};
		template<typename WIN>
		constexpr bool has_member_w_update = has_member_w_update_s<WIN, void>::value;
	}
}

template<typename BASE, typename ELEMENT>
template<typename window_type>
void ui::overlap_box<BASE, ELEMENT>::windowed_update(window_type& w, gui_manager& m, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, overlap_box<BASE, ELEMENT>&, window_type&, gui_manager&, world_state&>) {
		if (is_valid_index(element_def_tag)) {
			clear_items(m);
			BASE::windowed_update(*this, w, m, s);
			BASE::populate_list(*this, m, w);
			update_item_positions(m);

			if constexpr(ui::detail::has_member_w_update<ELEMENT>) {
				for (auto& i : contents) 
					i.member_update_in_window(i, m, s);
			} else {
				for (auto& i : contents)
					i.associated_object->update_data(gui_object_tag(), m, s);
			}
		}
	}
}

template<typename BASE, typename ELEMENT>
void ui::overlap_box<BASE, ELEMENT>::update_data(gui_object_tag, gui_manager& manager, world_state& s) {
	if (is_valid_index(element_def_tag)) {
		clear_items(manager);
		BASE::populate_list(*this, manager, w);
		update_scroll_position(manager);
		if constexpr(ui::detail::has_member_w_update<ELEMENT>) {
			for (auto& i : contents)
				i.member_update_in_window(i, manager, s);
		} else {
			for (auto& i : contents)
				i.associated_object->update_data(gui_object_tag(), m, s);
		}
	}
}
