#pragma once
#include "gui.h"
#include "graphics\\v2_window.h"

template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
bool ui::detail::dispatch_message(const gui_manager& manager, const MESSAGE_FUNCTION &member_f, ui::tagged_gui_object obj, const MESSAGE_TYPE& message) {
	auto relocated_message = adjust_message_location(message, -obj.object.position.x * manager.scale(), -obj.object.position.y * manager.scale());
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible) == 0)
		return false;
	
	if (message_within_bounds(relocated_message, obj.object.size.x * manager.scale(), obj.object.size.y * manager.scale())) {
	
		if ((object_flags & ui::gui_object::enabled) != 0) {
			const gui_object_tag child = obj.object.first_child;
			if (is_valid_index(child)) {
				gui_object_tag current_child = detail::last_sibling_of(manager, tagged_gui_object{ manager.gui_objects.at(child), child }).id;
				do {
					auto& current_obj = manager.gui_objects.at(current_child);
					const gui_object_tag next_index = current_obj.left_sibling;

					if (dispatch_message(manager, member_f, tagged_gui_object{ current_obj, current_child }, relocated_message))
						return true;

					current_child = next_index;
				} while (is_valid_index(current_child));
			}
		}

		if (std::is_same_v<MESSAGE_TYPE, ui::mouse_move> || (object_flags & ui::gui_object::enabled) != 0) {
			if constexpr(std::is_base_of_v<message_with_location, MESSAGE_TYPE>) {
				if ((obj.object.flags.load(std::memory_order_acquire) & ui::gui_object::type_mask) == ui::gui_object::type_graphics_object) {
					const auto& gobj = manager.graphics_instances.at(graphics_instance_tag(obj.object.type_dependant_handle.load(std::memory_order_acquire)));
					if ((gobj.graphics_object->flags & graphics::object::do_transparency_check) != 0) {
						graphics::color_rgba pixel;

						if (gobj.graphics_object->number_of_frames != 0) {
							pixel = gobj.t->get_pixel(
								(static_cast<float>(relocated_message.x) + manager.scale() * static_cast<float>(obj.object.size.x * gobj.frame)) / (static_cast<float>(obj.object.size.x * gobj.graphics_object->number_of_frames) * manager.scale()),
								static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y) * manager.scale()));
						} else {
							pixel = gobj.t->get_pixel(
								static_cast<float>(relocated_message.x) / (static_cast<float>(obj.object.size.x) * manager.scale()),
								static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y) * manager.scale()));
						}

						if (pixel.a < 127)
							return false;
					}
				}
			}

			if (member_f(obj, message))
				return true;
		}
	}
	return false;
}

template<typename T>
void ui::for_each_child(gui_manager& manager, tagged_gui_object parent, const T& f) {
	gui_object_tag child = parent.object.first_child;
	while (is_valid_index(child)) {
		auto& current_obj = manager.gui_objects.at(child);
		const gui_object_tag next_index = current_obj.right_sibling;
		f(tagged_gui_object{ current_obj, child });
		child = next_index;
	}
}

namespace ui {
	namespace detail {
		template<typename A, typename B>
		struct _has_shortcut : public std::false_type {};
		template<typename A>
		struct _has_shortcut<A, decltype(void(std::declval<A>().shortcut))> : public std::true_type {};
		template<typename A>
		constexpr bool has_shortcut = _has_shortcut<A, void>::value;
	}
}

template<typename T, typename B>
ui::tagged_gui_object ui::create_static_button(gui_manager& manager, T handle, tagged_gui_object parent, B& b) {
	auto new_obj = ui::detail::create_element_instance(manager, handle);
	new_obj.object.flags.fetch_or(gui_object::static_behavior, std::memory_order_acq_rel);
	new_obj.object.associated_behavior = &b;

	if constexpr(std::is_same_v<T, button_tag> && detail::has_shortcut<B>) {
		auto& bdef = manager.ui_definitions.buttons[handle];
		b.shortcut = bdef.shortcut;
	}

	ui::add_to_back(manager, parent, new_obj);

	return new_obj;
}

template<typename BASE>
bool ui::simple_button<BASE>::on_lclick(tagged_gui_object o, gui_manager & m, const lbutton_down &) {
	static_cast<BASE*>(this)->button_function(o, m);
	return true;
}

template<typename BASE>
bool ui::simple_button<BASE>::on_keydown(tagged_gui_object o, gui_manager & m, const key_down & k) {
	if (k.keycode == shortcut) {
		static_cast<BASE*>(this)->button_function(o, m);
		return true;
	} else {
		return false;
	}
}

#include "concurrency_tools\\concurrency_tools.hpp"