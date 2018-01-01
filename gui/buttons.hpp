#pragma once
#include "gui.h"

template<typename BASE>
bool ui::simple_button<BASE>::on_lclick(tagged_gui_object o, gui_manager & m, const lbutton_down &) {
	BASE::button_function(o, m);
	return true;
}

template<typename BASE>
bool ui::simple_button<BASE>::on_keydown(tagged_gui_object o, gui_manager & m, const key_down & k) {
	if (k.keycode == shortcut) {
		BASE::button_function(o, m);
		return true;
	} else {
		return false;
	}
}

template<typename BASE>
void ui::simple_button<BASE>::update_data(tagged_gui_object o, gui_manager& m) {
	if constexpr(ui::detail::has_update<BASE, tagged_gui_object, gui_manager&>) {
		BASE::update(o, m);
	}
}

template<typename B>
ui::tagged_gui_object ui::create_static_element(gui_manager& manager, button_tag handle, tagged_gui_object parent, simple_button<B>& b) {
	auto new_obj = ui::detail::create_element_instance(manager, handle);
	new_obj.object.flags.fetch_or(gui_object::static_behavior, std::memory_order_acq_rel);
	new_obj.object.associated_behavior = &b;

	auto& bdef = manager.ui_definitions.buttons[handle];
	b.shortcut = bdef.shortcut;

	ui::add_to_back(manager, parent, new_obj);
	return new_obj;
}

namespace buttons_detail {
	template<typename A, typename B, typename ... C>
	struct _has_on_unselect : std::false_type {};
	template<typename A, typename ... C>
	struct _has_on_unselect<A, decltype(void(std::declval<A>().on_unselect(std::declval<C>() ...))), C...> : std::true_type {};
	template<typename A, typename ... C>
	constexpr bool has_on_unselect = _has_on_unselect<A, void, C ...>::value;

	template<typename A, typename B, typename ... C>
	struct _has_has_tooltip : std::false_type {};
	template<typename A, typename ... C>
	struct _has_has_tooltip<A, decltype(void(std::declval<A>().has_tooltip(std::declval<C>() ...))), C...> : std::true_type {};
	template<typename A, typename ... C>
	constexpr bool has_has_tooltip = _has_has_tooltip<A, void, C ...>::value;


	template<uint32_t count, typename ... R>
	class _button_group;

	template<uint32_t count, typename BEHAVIOR>
	class _button_group<count, BEHAVIOR> : public BEHAVIOR, public ui::button_group_common_base {
	public:
		template<typename ... PARAMS>
		_button_group(PARAMS&& ... params) : BEHAVIOR(std::forward<PARAMS>(params) ...) {}

		template<typename window_type>
		void initialize_in_window(window_type& w) {};

		virtual void select(ui::tagged_gui_object o, ui::gui_manager&, uint32_t i) override {
			if constexpr(buttons_detail::has_on_unselect<BEHAVIOR, uint32_t>)
				BEHAVIOR::on_unselect(current_index);
			current_index = i;
			BEHAVIOR::on_select(current_index);
		}
		virtual ui::tooltip_behavior has_tooltip(ui::tagged_gui_object, ui::gui_manager&, const ui::mouse_move&, uint32_t i) override {
			if constexpr(buttons_detail::has_has_tooltip<BEHAVIOR, uint32_t>)
				return BEHAVIOR::has_tooltip(i);
			else
				return ui::tooltip_behavior::no_tooltip;
		}
		virtual void create_tooltip(ui::tagged_gui_object, ui::gui_manager& m, const ui::mouse_move&, ui::tagged_gui_object tw, uint32_t i) override {
			if constexpr(buttons_detail::has_has_tooltip<BEHAVIOR, uint32_t>)
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
		void initialize_in_window(window_type& w) {
			nth_button = &w.template get<FIRST>();
			nth_button->set_group(this, count);

			if constexpr(count == 0)
				nth_button->associated()->frame = 1;

			_button_group<count + 1, REST...>::initialize_in_window(w);
		}


		virtual void select(ui::tagged_gui_object o, ui::gui_manager& m, uint32_t i) override {
			if (i == count)
				nth_button->associated()->frame = 1;
			else 
				nth_button->associated()->frame = 0;
			_button_group<count + 1, REST...>::select(o, m, i);
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

template<typename BASE>
ui::tooltip_behavior ui::simple_button<BASE>::has_tooltip(tagged_gui_object, gui_manager&, const mouse_move&) {
	if constexpr(buttons_detail::has_has_tooltip<BASE>)
		return BASE::has_tooltip();
	else
		return tooltip_behavior::no_tooltip;
}

template<typename BASE>
void ui::simple_button<BASE>::create_tooltip(tagged_gui_object o, gui_manager& m, const mouse_move&, tagged_gui_object tw) {
	if constexpr(buttons_detail::has_has_tooltip<BASE>)
		BASE::create_tooltip(m, tw);
}

#include "gui.hpp"