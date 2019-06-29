#pragma once
#include "gui.hpp"

#ifdef _DEBUG
#include <Windows.h>
#undef max
#undef min
#endif


template<typename OBJ, typename RET, typename ... PARAMS>
struct has_populate_list_s : public std::false_type {};
template<typename OBJ, typename ... PARAMS>
struct has_populate_list_s<OBJ, decltype(void(std::declval<OBJ>().populate_list(std::declval<PARAMS>() ...))), PARAMS ...> : public std::true_type {};
template<typename OBJ, typename ... PARAMS>
constexpr bool has_populate_list = has_populate_list_s<OBJ, void, PARAMS ...>::value;

template<typename BASE, typename ELEMENT, int32_t left_expand>
bool ui::display_listbox<BASE, ELEMENT, left_expand>::on_scroll(gui_object_tag o, world_state& m, const scroll& s) {
	return sb.on_scroll(o, m, s);
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
bool ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::on_scroll(gui_object_tag o, world_state& m, const scroll& s) {
	return sb.on_scroll(o, m, s);
}

template<typename BASE, typename data_type_list, typename gui_type_list>
bool ui::tree_view<BASE, data_type_list, gui_type_list>::on_scroll(gui_object_tag o, world_state& m, const scroll& s) {
	return sb.on_scroll(o, m, s);
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::update_data(gui_object_tag, world_state& ws) {
	clear_items(ws.w.gui_m);
	BASE::populate_list(*this, ws);
	update_scroll_position(ws.w.gui_m);
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::update_data(gui_object_tag, world_state& ws) {
	if constexpr(ui::detail::has_populate_list<BASE, discrete_listbox<BASE, ELEMENT, value_type, left_expand>&, world_state&>) {
		BASE::populate_list(*this, ws);
	}

	update_scroll_position(ws.w.gui_m);

	for(uint32_t i = 0; i < display_list.size(); ++i) {
		if(i + offset < values_list.size() && bool(values_list[i + offset])) {
			display_list[i].set_value(*(values_list[i + offset]));
			ui::make_visible_and_update(ws.w.gui_m, *(display_list[i].associated_object));
		} else {
			ui::hide(*(display_list[i].associated_object));
		}
	}

	ws.w.gui_m.tooltip = gui_object_tag();
	ws.w.gui_m.on_mouse_move(ws, ui::mouse_move{ ws.w.gui_m.last_mouse_move.x, ws.w.gui_m.last_mouse_move.y, key_modifiers::modifiers_none });
}

namespace ui {
	namespace detail {
		template<typename A, typename B, typename ... C>
		struct _has_set_value : std::false_type {};
		template<typename A, typename ... C>
		struct _has_set_value<A, decltype(void(std::declval<A>().set_value(std::declval<C>() ...))), C...> : std::true_type {};
		template<typename A, typename ... C>
		constexpr bool has_set_value = _has_set_value<A, void, C ...>::value;

		template<size_t index, typename ... gui_types, typename ... variant_types>
		void helper_display_nth(std::tuple<gui_types...>& tu, std::variant<variant_types...> const& v, world_state& ws) {
			if(v.index() == index + 1) {
				if constexpr(has_set_value<decltype(std::get<index>(tu)), size_t, decltype(std::get<index + 1>(v))>)
					std::get<index>(tu).set_value(index, std::get<index + 1>(v));
				ui::make_visible_and_update(ws.w.gui_m, *(std::get<index>(tu).associated_object));
			} else {
				ui::hide(*(std::get<index>(tu).associated_object));
			}
			if constexpr(index > 0)
				helper_display_nth<index - 1ui64>(tu, v, ws);
		}
	}
}

template<typename BASE, typename data_type_list, typename gui_type_list>
void ui::tree_view<BASE, data_type_list, gui_type_list>::force_open(type_list_to_variant<data_type_list> const& v) {
	force_visible.push_back(v);
}

template<typename BASE, typename data_type_list, typename gui_type_list>
bool ui::tree_view<BASE, data_type_list, gui_type_list>::is_open(type_list_to_variant<data_type_list> const& v) const {
	if(std::find(force_visible.begin(), force_visible.end(), v) != force_visible.end())
		return true;
	for(auto& vp : values_list) {
		if(vp.first == v)
			return vp.second;
	}
	return false;
}

template<typename BASE, typename data_type_list, typename gui_type_list>
void ui::tree_view<BASE, data_type_list, gui_type_list>::set_open(type_list_to_variant<data_type_list> const& v, bool o, ui::gui_manager& m) {
	for(auto& vp : values_list) {
		if(vp.first == v) {
			if(vp.second != o) {
				vp.second = o;
				ui::make_visible_and_update(m, *associated_object);
			}
			return;
		}
	}
}

namespace ui {
	namespace detail {
		template<size_t level, typename data_type_list, typename BASE, typename VTYPE, typename value_type>
		void fill_vector_helper(BASE& base_obj, VTYPE& vector_out, value_type v_obj, world_state& ws) {
			if constexpr(level < type_list_size<data_type_list>) {
				auto list_for = base_obj.template sub_list<level>(v_obj, ws);

				for(auto v : list_for) {
					bool is_open = base_obj.is_open(v);
					vector_out.emplace_back(v, is_open);

					if(is_open)
						fill_vector_helper<level + 1, data_type_list>(base_obj, vector_out, v, ws);
				}
			}
		}

		template<typename data_type_list, typename BASE, typename VTYPE>
		void fill_vector_helper_base(BASE& base_obj, VTYPE& vector_out, world_state& ws) {
			auto list_for = base_obj.base_list(ws);
			for(auto v : list_for) {
				bool is_open = base_obj.is_open(v);
				vector_out.emplace_back(v, is_open);

				if(is_open)
					fill_vector_helper<1, data_type_list>(base_obj, vector_out, v, ws);
			}
		}
	}
}

template<typename BASE, typename data_type_list, typename gui_type_list>
void ui::tree_view<BASE, data_type_list, gui_type_list>::update_data(gui_object_tag, world_state& ws) {
	{
		decltype(values_list) new_values_list;
		detail::fill_vector_helper_base<data_type_list>(*this, new_values_list, ws);
		values_list = std::move(new_values_list);
		force_visible.clear();
	}

	update_scroll_position(ws.w.gui_m);

	for(uint32_t i = 0; i < display_list.size(); ++i) {
		if(i + offset < values_list.size()) {
			detail::helper_display_nth<type_list_size<data_type_list> - 1ui64>(display_list[i], values_list[i + offset].first, ws);
		}
	}
}


template<typename BASE, typename ELEMENT, int32_t left_expand>
template<typename window_type>
void ui::display_listbox<BASE, ELEMENT, left_expand>::windowed_update(window_type& w, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, display_listbox<BASE, ELEMENT, left_expand>&, window_type&, world_state&>) {
		clear_items(s.w.gui_m);
		BASE::windowed_update(*this, w, s);
		BASE::populate_list(*this, w, s);
		update_scroll_position(s.w.gui_m);
	}
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
template<typename window_type>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::windowed_update(window_type& w, world_state& s) {
	if constexpr(ui::detail::has_populate_list<BASE, discrete_listbox<BASE, ELEMENT, value_type, left_expand>&, window_type&, world_state&>) {
		BASE::populate_list(*this, w, s);

		update_scroll_position(s.w.gui_m);

		for(uint32_t i = 0; i < display_list.size(); ++i) {
			if(i + offset < values_list.size() && bool(values_list[i + offset])) {
				display_list[i].set_value(*(values_list[i + offset]));
				ui::make_visible_and_update(s.w.gui_m, *(display_list[i].associated_object));
			} else {
				ui::hide(*(display_list[i].associated_object));
			}
		}

		s.w.gui_m.tooltip = gui_object_tag();
		s.w.gui_m.on_mouse_move(s, ui::mouse_move{ s.w.gui_m.last_mouse_move.x, s.w.gui_m.last_mouse_move.y, key_modifiers::modifiers_none });
	}
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::set_element_definition(gui_static& manager) {
	const auto t = BASE::element_tag(manager);
	element_def_tag = t;
	element_def = &manager.ui_definitions.windows[t];
	sb.factor = element_def->size.y;
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::set_element_definition(gui_static& manager) {
	const auto t = BASE::element_tag(manager);
	element_def_tag = t;
	element_def = &manager.ui_definitions.windows[t];
}

template<typename BASE, typename data_type_list, typename gui_type_list>
void ui::tree_view<BASE, data_type_list, gui_type_list>::set_element_definition(gui_static& manager) {
	for(uint32_t i = 0; i < type_list_size<gui_type_list>; ++i) {
		const auto t = BASE::element_tag(i, manager);
		element_def_tag[i] = t;
		element_def[i] = &manager.ui_definitions.windows[t];
	}
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

	sb.set_range(manager, 0, (extra + sb.factor - 1) / sb.factor);
	if (extra > 0) 
		ui::make_visible_and_update(manager, *(sb.associated_object));
	
	if(sb.position() * sb.factor > (extra + sb.factor - 1)) 
		sb.update_position((extra + sb.factor - 1) / sb.factor);
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::update_scroll_position(gui_manager& manager) {
	if(!element_def || !sb.associated_object)
		return;

	const int32_t extra = std::max(0, int32_t(values_list.size()) - int32_t(display_list.size()));
	sb.set_range(manager, 0, extra);

	if(extra > 0)
		ui::make_visible_and_update(manager, *(sb.associated_object));
	else
		ui::hide(*sb.associated_object);

	offset = std::min(offset, uint32_t(extra));

	if(sb.position() > extra)
		sb.update_position(extra);
}

template<typename BASE, typename data_type_list, typename gui_type_list>
int32_t ui::tree_view<BASE, data_type_list, gui_type_list>::get_max_element_height() {
	int32_t max = 1;
	for(uint32_t i = 0; i < type_list_size<gui_type_list>; ++i) {
		if(element_def[i])
			max = std::max(max, int32_t(element_def[i]->size.y));
	}
	return max;
}

template<typename BASE, typename data_type_list, typename gui_type_list>
void ui::tree_view<BASE, data_type_list, gui_type_list>::update_scroll_position(gui_manager& manager) {
	if(!sb.associated_object)
		return;

	const int32_t extra = std::max(0, static_cast<int32_t>(int32_t(values_list.size()) - associated_object->size.y / get_max_element_height()));
	sb.set_range(manager, 0, extra);

	offset = std::min(offset, uint32_t(extra));

	if(extra > 0)
		ui::make_visible_and_update(manager, *(sb.associated_object));
	else
		ui::hide(*sb.associated_object);

	if(sb.position() > extra)
		sb.update_position(extra);
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
template<typename ... PARAMS>
void ui::display_listbox<BASE, ELEMENT, left_expand>::add_item(world_state& ws, PARAMS&& ... params) {
	contents.emplace_back(std::forward<PARAMS>(params)...);
	auto& n = contents.back();

	create_static_element(ws, element_def_tag, tagged_gui_object{ *_content_frame, _content_frame_tag }, n);

	if constexpr(left_expand != 0)
		n.associated_object->flags.fetch_or(gui_object::dont_clip_children, std::memory_order_acq_rel);

	n.associated_object->position.x += static_cast<int16_t>(left_expand);
	n.associated_object->position.y = static_cast<int16_t>(element_def->size.y * static_cast<int32_t>((contents.size() - 1)));
	_content_frame->size.y = static_cast<int16_t>(element_def->size.y * static_cast<int32_t>(contents.size()));
}

template<typename BASE, typename ELEMENT, int32_t left_expand>
void ui::display_listbox<BASE, ELEMENT, left_expand>::create_sub_elements(tagged_gui_object self, world_state& ws) {
	set_element_definition(ws.s.gui_m);

	const auto inner_area = ws.w.gui_m.gui_objects.emplace();

	inner_area.object.position = ui::xy_pair{ 0, 0 };
	inner_area.object.size = ui::xy_pair{ static_cast<int16_t>(associated_object->size.x - 16), 1i16 };

	_content_frame = &inner_area.object;
	_content_frame_tag = inner_area.id;

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

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::create_sub_elements(tagged_gui_object self, world_state& ws) {
	set_element_definition(ws.s.gui_m);

	const uint32_t element_count = uint32_t(std::max(1, static_cast<int32_t>(associated_object->size.y / element_def->size.y)));
	display_list.resize(element_count);

	ui::create_static_fixed_sz_scrollbar(
		ws,
		ws.s.gui_m.ui_definitions.standardlistbox_slider,
		self,
		ui::xy_pair{ static_cast<int16_t>(associated_object->size.x - 16), 0i16 },
		element_def->size.y * element_count,
		sb);

	for(uint32_t i = 0; i < element_count; ++i) {
		auto new_obj = create_static_element(ws, element_def_tag, self, display_list[i]);
		new_obj.object.position.y = static_cast<int16_t>(int32_t(i) * element_def->size.y);
		if constexpr(left_expand != 0) {
			new_obj.object.flags.fetch_or(gui_object::dont_clip_children, std::memory_order_acq_rel);
			new_obj.object.position.x += left_expand;
		}
		new_obj.object.size.x = int16_t(std::max(associated_object->size.x - 16 - new_obj.object.position.x, int32_t(new_obj.object.size.x)));
		ui::hide(new_obj.object);
	}

	sb.associate(&offset);
	ui::hide(*(sb.associated_object));
}

namespace ui {
	namespace detail {
		template<size_t nth, typename tuple_type>
		void create_nth_ui_element(tuple_type& tuple, int32_t yoffset, ui::tagged_gui_object self, ui::window_tag const* defs, world_state& ws) {
			if constexpr(nth < std::tuple_size_v<tuple_type>) {
				auto new_obj = create_static_element(ws, defs[nth], self, std::get<nth>(tuple));
				ui::hide(new_obj.object);
				new_obj.object.position.y = static_cast<int16_t>(yoffset);

				create_nth_ui_element<nth + 1>(tuple, yoffset, self, defs, ws);
			}
		}
	}
}

template<typename BASE, typename data_type_list, typename gui_type_list>
void ui::tree_view<BASE, data_type_list, gui_type_list>::create_sub_elements(tagged_gui_object self, world_state& ws) {
	set_element_definition(ws.s.gui_m);

	const auto element_size = get_max_element_height();
	const uint32_t element_count = uint32_t(std::max(1, static_cast<int32_t>(associated_object->size.y / element_size)));
	display_list.resize(element_count);

	ui::create_static_fixed_sz_scrollbar(
		ws,
		ws.s.gui_m.ui_definitions.standardlistbox_slider,
		self,
		ui::xy_pair{ static_cast<int16_t>(associated_object->size.x - 16), 0i16 },
		associated_object->size.y,
		sb);

	for(uint32_t i = 0; i < element_count; ++i) {
		ui::detail::create_nth_ui_element<0>(display_list[i], int32_t(i) * element_size, self, element_def_tag, ws);
	}

	sb.associate(&offset);
	ui::hide(*(sb.associated_object));
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
template<typename iterator>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::new_list(iterator first, iterator last) {
	values_list.clear();
	if constexpr(!std::is_same_v<iterator, nullptr_t>) {
		for(auto it = first; it != last; ++it) {
			values_list.emplace_back(*it);
		}
	}
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
template<typename iterator>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::update_list(iterator first, iterator last) {
	for(auto& opt_v : values_list) {
		if(opt_v && std::find(first, last, *opt_v) == last)
			opt_v = std::optional<value_type>();
	}
	for(auto it = first; it != last; ++it) {
		if(auto fr = std::find(values_list.begin(), values_list.end(), std::optional<value_type>(*it)); fr == values_list.end())
			values_list.emplace_back(*it);
		else
			*fr = *it;
	}
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::goto_element(value_type const& v, ui::gui_manager& m) {
	int32_t index = [_this = this, &v]() {
		for(int32_t i = 0; i < int32_t(_this->values_list.size()); ++i) {
			if(bool(_this->values_list[i]) && *(_this->values_list[i]) == v)
				return i;
		}
		return -1;
	}();

	if(index == -1)
		return; // not in list

	const int32_t num_displayed = associated_object->size.y / element_def->size.y;
	const int32_t extra = std::max(0, static_cast<int32_t>(int32_t(values_list.size()) - num_displayed));

	if(int32_t(offset) <= index && index < int32_t(offset) + num_displayed)
		return; //already in view

	offset = uint32_t(std::min(index, extra));
	sb.update_position(int32_t(offset));

	for(uint32_t i = 0; i < display_list.size(); ++i) {
		if(i + offset < values_list.size() && bool(values_list[i + offset])) {
			display_list[i].set_value(*(values_list[i + offset]));
			ui::make_visible_and_update(m, *(display_list[i].associated_object));
		} else {
			ui::hide(*(display_list[i].associated_object));
		}
	}
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
uint32_t ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::get_position() const {
	return offset;
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
void ui::discrete_listbox<BASE, ELEMENT, value_type, left_expand>::set_position(uint32_t p, ui::gui_manager& m) {
	const int32_t num_displayed = associated_object->size.y / element_def->size.y;
	const int32_t last_top = std::max(0, static_cast<int32_t>(int32_t(values_list.size()) - num_displayed));

	offset = std::min(p, uint32_t(last_top));
	sb.update_position(int32_t(offset));

	for(uint32_t i = 0; i < display_list.size(); ++i) {
		if(i + offset < values_list.size() && bool(values_list[i + offset])) {
			display_list[i].set_value(*(values_list[i + offset]));
			ui::make_visible_and_update(m, *(display_list[i].associated_object));
		} else {
			ui::hide(*(display_list[i].associated_object));
		}
	}
}

template<typename B, typename ELEMENT, int32_t left_expand>
ui::tagged_gui_object ui::create_static_element(world_state& ws, listbox_tag handle, tagged_gui_object parent, ui::display_listbox<B, ELEMENT, left_expand>& b) {
	const ui::listbox_def& definition = ws.s.gui_m.ui_definitions.listboxes[handle];

	const auto new_obj = ws.w.gui_m.gui_objects.emplace();

	new_obj.object.position = definition.position;
	new_obj.object.size = definition.size;

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	if (is_valid_index(definition.background_handle)) {
		ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_obj, definition.background_handle);
	}

	new_obj.object.position.x -= static_cast<int16_t>(left_expand);
	new_obj.object.size.x += static_cast<int16_t>(left_expand + 16);

	if constexpr(ui::detail::has_on_create<B, ui::display_listbox<B, ELEMENT, left_expand>&, world_state&>)
		b.on_create(b, ws);

	b.create_sub_elements(new_obj, ws);

	ui::add_to_back(ws.w.gui_m, parent, new_obj);
	ws.w.gui_m.flag_minimal_update();
	return new_obj;
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
ui::tagged_gui_object ui::create_static_element(world_state& ws, listbox_tag handle, tagged_gui_object parent, discrete_listbox<BASE, ELEMENT, value_type, left_expand>& b) {
	const ui::listbox_def& definition = ws.s.gui_m.ui_definitions.listboxes[handle];

	const auto new_obj = ws.w.gui_m.gui_objects.emplace();

	new_obj.object.position = definition.position;
	new_obj.object.size = definition.size;

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	if(is_valid_index(definition.background_handle)) {
		ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_obj, definition.background_handle);
	}

	new_obj.object.position.x -= static_cast<int16_t>(left_expand);
	new_obj.object.size.x += static_cast<int16_t>(left_expand + 16);

	if constexpr(ui::detail::has_on_create<BASE, discrete_listbox<BASE, ELEMENT, value_type, left_expand>&, world_state&>)
		b.on_create(b, ws);

	b.create_sub_elements(new_obj, ws);

	ui::add_to_back(ws.w.gui_m, parent, new_obj);
	ws.w.gui_m.flag_minimal_update();
	return new_obj;
}

template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
ui::tagged_gui_object ui::create_static_element(world_state& ws, window_tag handle, tagged_gui_object parent, discrete_listbox<BASE, ELEMENT, value_type, left_expand>& b) {
	const ui::window_def& definition = ws.s.gui_m.ui_definitions.windows[handle];

	const auto new_obj = ws.w.gui_m.gui_objects.emplace();

	new_obj.object.position = definition.position;
	new_obj.object.size = definition.size;

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	if(is_valid_index(definition.background_handle)) {
		//ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_obj, definition.background_handle);
	}

	new_obj.object.position.x -= static_cast<int16_t>(left_expand);
	new_obj.object.size.x += static_cast<int16_t>(left_expand + 16);

	if constexpr(ui::detail::has_on_create<BASE, discrete_listbox<BASE, ELEMENT, value_type, left_expand>&, world_state&>)
		b.on_create(b, ws);

	b.create_sub_elements(new_obj, ws);

	ui::add_to_back(ws.w.gui_m, parent, new_obj);
	ws.w.gui_m.flag_minimal_update();
	return new_obj;
}


template<typename BASE, typename data_type_list, typename gui_type_list>
ui::tagged_gui_object ui::create_static_element(world_state& ws, listbox_tag handle, tagged_gui_object parent, tree_view<BASE, data_type_list, gui_type_list>& b) {
	const ui::listbox_def& definition = ws.s.gui_m.ui_definitions.listboxes[handle];

	const auto new_obj = ws.w.gui_m.gui_objects.emplace();

	new_obj.object.position = definition.position;
	new_obj.object.size = definition.size;

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	if(is_valid_index(definition.background_handle)) {
		ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_obj, definition.background_handle);
	}

	new_obj.object.size.x += static_cast<int16_t>(16);

	if constexpr(ui::detail::has_on_create<BASE, tree_view<BASE, data_type_list, gui_type_list>&, world_state&>)
		b.on_create(b, ws);

	b.create_sub_elements(new_obj, ws);

	ui::add_to_back(ws.w.gui_m, parent, new_obj);
	ws.w.gui_m.flag_minimal_update();
	return new_obj;
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::set_self_information(world_state& ws, gui_object_tag s, int32_t sp, text_data::alignment a) {
	const auto t = BASE::element_tag(ws.s.gui_m);
	element_def_tag = t;

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
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::add_item(world_state& ws, PARAMS&& ... params) {
	if (is_valid_index(temp) && is_valid_index(element_def_tag)) {
		if constexpr(!ui::detail::has_set_value<ELEMENT, PARAMS...>) {
			contents.emplace_back(std::forward<PARAMS>(params)...);
		} else {
			contents.emplace_back();
			contents.back().set_value(std::forward<PARAMS>(params)...);
		}
		auto& n = contents.back();
		ui::create_static_element(ws, element_def_tag, tagged_gui_object{ ws.w.gui_m.gui_objects.at(temp), temp }, n);

		if constexpr(vertical_extension != 0)
			n.associated_object->flags.fetch_or(gui_object::dont_clip_children, std::memory_order_acq_rel);
	}
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::update_item_positions() {
	const int32_t amount = static_cast<int32_t>(contents.size());
	if (amount == 0)
		return;

	int32_t space_consumed = spacing * (amount - 1);
	for(int32_t n = amount - 1; n >= 0; --n)
		space_consumed += contents[static_cast<uint32_t>(n)].associated_object->size.x;

	const int32_t overlap = amount == 1 ?
		0 :
		(space_consumed > associated_object->size.x ? 
			(space_consumed - associated_object->size.x + amount - 2) / (amount - 1) :
			0);
	const int32_t adjusted_space_consumed = space_consumed - overlap * (amount - 1);
	const int32_t base_offset =
		subelement_alignment == text_data::alignment::left ?
		0 :
		(subelement_alignment == text_data::alignment::right ?
		    (associated_object->size.x - adjusted_space_consumed) :
			(associated_object->size.x - adjusted_space_consumed) / 2);

	int32_t current_off = base_offset;
	for(int32_t n = 0; n < amount; ++n) {
		contents[static_cast<uint32_t>(n)].associated_object->position.x = int16_t(current_off);
		current_off += contents[static_cast<uint32_t>(n)].associated_object->size.x + spacing - overlap;
	}
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::update_data(gui_object_tag, world_state& s) {
	if constexpr(ui::detail::has_populate_list<BASE, ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>&, world_state&>) {
		if(is_valid_index(element_def_tag)) {
			auto temp_holder = s.w.gui_m.gui_objects.emplace();
			this->temp = temp_holder.id;
			temp_holder.object.size = associated_object->size;
			temp_holder.object.position = associated_object->position;

			std::vector<ELEMENT, concurrent_allocator<ELEMENT>> temp_vector(std::move(contents));

			//clear_items(s.w.gui_m);
			BASE::populate_list(*this, s);
			update_item_positions();

			ui::replace_children(s.w.gui_m, tagged_gui_object{ *associated_object, self }, temp_holder);
			temp = gui_object_tag();
			s.w.gui_m.destroy(temp_holder);
		}
	}
}

template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension>
template<typename window_type>
void ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>::windowed_update(window_type& w, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, ui::overlap_box<BASE, tag_type, ELEMENT, vertical_extension>&, window_type&, world_state&>) {
		if(is_valid_index(element_def_tag)) {
			auto temp_holder = s.w.gui_m.gui_objects.emplace();
			this->temp = temp_holder.id;
			temp_holder.object.size = associated_object->size;
			temp_holder.object.position = associated_object->position;

			std::vector<ELEMENT, concurrent_allocator<ELEMENT>> temp_vector(std::move(contents));

			//clear_items(s.w.gui_m);
			BASE::windowed_update(*this, w, s);
			update_item_positions();

			ui::replace_children(s.w.gui_m, tagged_gui_object{ *associated_object, self }, temp_holder);
			temp = gui_object_tag();
			s.w.gui_m.destroy(temp_holder);
		}
	}
}

template<typename B, typename tag_type, typename ELEMENT, int32_t vertical_extension>
ui::tagged_gui_object ui::create_static_element(world_state& ws, overlapping_region_tag handle, tagged_gui_object parent, ui::overlap_box<B, tag_type, ELEMENT, vertical_extension>& b) {
	const ui::overlapping_region_def& definition = ws.s.gui_m.ui_definitions.get(handle);

	const auto new_obj = ws.w.gui_m.gui_objects.emplace();

	new_obj.object.position = definition.position;
	new_obj.object.size = ui::xy_pair{ definition.size.x, static_cast<int16_t>(definition.size.y + vertical_extension) };

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	b.set_self_information(ws, new_obj.id, static_cast<int32_t>(definition.spacing), ui::text_aligment_from_overlapping_definition(definition));

	if constexpr(ui::detail::has_on_create<B, ui::overlap_box<B, tag_type, ELEMENT, vertical_extension>&, world_state&>)
		b.on_create(b, ws);

	ui::add_to_back(ws.w.gui_m, parent, new_obj);
	ws.w.gui_m.flag_minimal_update();
	return new_obj;
}
