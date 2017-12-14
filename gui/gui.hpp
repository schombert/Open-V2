#pragma once
#include "gui.h"
#include "graphics\\v2_window.h"

template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
bool ui::gui_manager::dispatch_message_internal(const MESSAGE_FUNCTION &member_f, ui::tagged_gui_object obj, const MESSAGE_TYPE& message) {
	auto relocated_message = adjust_message_location(message, -obj.object.position.x * scale, -obj.object.position.y * scale);

	if (message_within_bounds(relocated_message, obj.object.size.x * scale, obj.object.size.y * scale)) {
		auto child = obj.object.first_child.load(std::memory_order_acquire);
		while (child != 0) {
			const auto& current_obj = gui_objects.at(child);
			auto next_index = current_obj.right_sibling.load(std::memory_order_acquire);
			if (next_index == 0)
				break;
			child = next_index;
		}

		while (child != 0) {
			auto& current_obj = gui_objects.at(child);
			auto next_index = current_obj.left_sibling.load(std::memory_order_acquire);

			if (dispatch_message_internal(member_f, tagged_gui_object{ current_obj, child }, relocated_message))
				return true;

			child = next_index;
		}

		if constexpr(std::is_base_of_v<message_with_location, MESSAGE_TYPE>) {
			if ((obj.object.flags.load(std::memory_order_acquire) & ui::gui_object::type_mask) == ui::gui_object::type_graphics_object) {
				const auto& gobj = graphics_instances.at(obj.object.type_dependant_handle.load(std::memory_order_acquire));
				if ((gobj.graphics_object->flags & graphics::object::do_transparency_check) != 0) {
					color_rgba pixel;

					if (gobj.graphics_object->number_of_frames != 0) {
						pixel = gobj.t->get_pixel(
							(static_cast<float>(relocated_message.x) + scale * static_cast<float>(obj.object.size.x * gobj.frame)) / (static_cast<float>(obj.object.size.x * gobj.graphics_object->number_of_frames) * scale),
							static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y) * scale));
					} else {
						pixel = gobj.t->get_pixel(
							static_cast<float>(relocated_message.x) / (static_cast<float>(obj.object.size.x) * scale),
							static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y) * scale));
					}

					if (pixel.a < 127)
						return false;
				}
			}
		}

		if (member_f(obj, message))
			return true;
	}
	return false;
}

#include "concurrency_tools\\concurrency_tools.hpp"