#pragma once
#include "gui.hpp"

template<typename BASE>
bool ui::simple_button<BASE>::on_lclick(gui_object_tag o, world_state& m, const lbutton_down &) {
	if constexpr(ui::detail::has_button_function<BASE, gui_object_tag, world_state&>)
		BASE::button_function(o, m);
	return true;
}

template<typename BASE>
bool ui::simple_button<BASE>::on_keydown(gui_object_tag o, world_state & m, const key_down & k) {
	if (k.keycode == shortcut) {
		if constexpr(ui::detail::has_button_function<BASE, gui_object_tag, world_state&>)
			BASE::button_function(o, m);
		return true;
	} else {
		return false;
	}
}

template<typename BASE>
void ui::simple_button<BASE>::update_data(gui_object_tag, world_state& w) {
	if constexpr(ui::detail::has_update<BASE, simple_button<BASE>&, world_state&>) {
		BASE::update(*this, w);
	}
}

template<typename BASE>
template<typename window_type>
void ui::simple_button<BASE>::windowed_update(window_type& w, world_state& s) {
	if constexpr(ui::detail::has_windowed_update<BASE, simple_button<BASE>&, window_type&, world_state&>) {
		BASE::windowed_update(*this, w, s);
	}
}

template<typename BASE>
ui::tooltip_behavior ui::simple_button<BASE>::has_tooltip(gui_object_tag, world_state& ws, const mouse_move&) {
	if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>)
		return BASE::has_tooltip(ws) ? tooltip_behavior::tooltip : tooltip_behavior::no_tooltip;
	else
		return tooltip_behavior::no_tooltip;
}

template<typename BASE>
void ui::simple_button<BASE>::create_tooltip(gui_object_tag o, world_state& ws, const mouse_move&, tagged_gui_object tw) {
	if constexpr(ui::detail::has_has_tooltip<BASE, world_state&>)
		BASE::create_tooltip(ws, tw);
}

template<typename BASE>
void ui::simple_button<BASE>::set_frame(gui_manager& m, uint32_t frame_num) {
	if (const auto go = m.graphics_instances.safe_at(graphics_instance_tag(associated_object->type_dependant_handle)); go) {
		go->frame = static_cast<int32_t>(frame_num);
	}
}

template<typename BASE>
void ui::simple_button<BASE>::set_visibility(gui_manager& m, bool visible) {
	if (visible)
		ui::make_visible_and_update(m, *associated_object);
	else
		ui::hide(*associated_object);
}

template<typename BASE>
void ui::simple_button<BASE>::set_enabled(bool enabled) {
	ui::set_enabled(*associated_object, enabled);
}

template<typename B>
ui::tagged_gui_object ui::create_static_element(world_state& ws, button_tag handle, tagged_gui_object parent, simple_button<B>& b) {
	auto new_obj = ui::detail::create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);

	new_obj.object.associated_behavior = &b;
	b.associated_object = &new_obj.object;

	auto& bdef = ws.s.gui_m.ui_definitions.buttons[handle];
	b.shortcut = bdef.shortcut;

	ui::add_to_back(ws.w.gui_m, parent, new_obj);
	ws.w.gui_m.flag_minimal_update();
	return new_obj;
}

namespace buttons_detail {
	template<typename A, typename B, typename ... C>
	struct _has_on_unselect : std::false_type {};
	template<typename A, typename ... C>
	struct _has_on_unselect<A, decltype(void(std::declval<A>().on_unselect(std::declval<C>() ...))), C...> : std::true_type {};
	template<typename A, typename ... C>
	constexpr bool has_on_unselect = _has_on_unselect<A, void, C ...>::value;

	template<uint32_t count, typename ... R>
	class _button_group;

	template<uint32_t count, typename BEHAVIOR>
	class _button_group<count, BEHAVIOR> : public BEHAVIOR, public ui::button_group_common_base {
	public:
		template<typename ... PARAMS>
		_button_group(PARAMS&& ... params) : BEHAVIOR(std::forward<PARAMS>(params) ...) {}

		template<typename window_type>
		void initialize_in_window(window_type& w, world_state& ws) {
			if constexpr(ui::detail::has_initialize_in_window<BEHAVIOR, window_type&, world_state&>)
				BEHAVIOR::initialize_in_window(w, ws);
		}

		virtual void select(world_state& m, uint32_t i) override {
			if constexpr(buttons_detail::has_on_unselect<BEHAVIOR, world_state&, uint32_t>)
				BEHAVIOR::on_unselect(m, current_index);
			current_index = i;
			BEHAVIOR::on_select(m, current_index);
		}
		virtual ui::tooltip_behavior has_tooltip(uint32_t i) override {
			if constexpr(ui::detail::has_has_tooltip<BEHAVIOR, world_state&, ui::tagged_gui_object, uint32_t>)
				return BEHAVIOR::has_tooltip(i);
			else
				return ui::tooltip_behavior::no_tooltip;
		}
		virtual void create_tooltip(world_state& m, ui::tagged_gui_object tw, uint32_t i) override {
			if constexpr(ui::detail::has_has_tooltip<BEHAVIOR, world_state&, ui::tagged_gui_object, uint32_t>)
				BEHAVIOR::create_tooltip(m, tw, i);
		}
	};

	template<uint32_t count, typename FIRST, typename ... REST>
	class _button_group<count, FIRST, REST ...> : public _button_group<count + 1, REST...> {
	private:
		ui::button_group_member* nth_button = nullptr;
	public:
		template<typename ... PARAMS>
		_button_group(PARAMS&& ... params) : _button_group<count + 1, REST...>(std::forward<PARAMS>(params)...) {}

		template<typename window_type>
		void initialize_in_window(window_type& w, world_state& ws) {
			nth_button = &w.template get<FIRST>();
			nth_button->set_group(this, count);

			if constexpr(count == 0) {
				if (const auto gi = ws.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(nth_button->associated_object->type_dependant_handle)); gi)
					gi->frame = 1;
			}

			_button_group<count + 1, REST...>::initialize_in_window(w, ws);
		}


		virtual void select(world_state& m, uint32_t i) override {
			if (const auto gi = m.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(nth_button->associated_object->type_dependant_handle)); gi) {
				if (i == count)
					gi->frame = 1;
				else
					gi->frame = 0;
			}
			_button_group<count + 1, REST...>::select(m, i);
		}
	};
}

template<typename ... N>
class ui::button_group : public buttons_detail::_button_group<0, N...> {
public:
	template<typename ... PARAMS>
	button_group(PARAMS&& ... params) :
		buttons_detail::_button_group<0, N...>(std::forward<PARAMS>(params) ...) {}
};
