#pragma once
#include "gui.h"
#include "graphics\\v2_window.h"

template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
bool ui::gui_manager::dispatch_message_internal(const MESSAGE_FUNCTION &member_f, ui::tagged_gui_object obj, const MESSAGE_TYPE& message) {
	auto relocated_message = adjust_message_location(message, -obj.object.position.x * scale, -obj.object.position.y * scale);
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible) == 0)
		return false;
	
	if (message_within_bounds(relocated_message, obj.object.size.x * scale, obj.object.size.y * scale)) {
	
		if ((object_flags & ui::gui_object::enabled) != 0) {
			const gui_object_tag child = obj.object.first_child;
			if (is_valid_index(child)) {
				gui_object_tag current_child = last_sibling_of(tagged_gui_object{ gui_objects.at(child), child }).id;
				do {
					auto& current_obj = gui_objects.at(current_child);
					const gui_object_tag next_index = current_obj.left_sibling;

					if (dispatch_message_internal(member_f, tagged_gui_object{ current_obj, current_child }, relocated_message))
						return true;

					current_child = next_index;
				} while (is_valid_index(current_child));
			}
		}

		if (std::is_same_v<MESSAGE_TYPE, ui::mouse_move> || (object_flags & ui::gui_object::enabled) != 0) {
			if constexpr(std::is_base_of_v<message_with_location, MESSAGE_TYPE>) {
				if ((obj.object.flags.load(std::memory_order_acquire) & ui::gui_object::type_mask) == ui::gui_object::type_graphics_object) {
					const auto& gobj = graphics_instances.at(graphics_instance_tag(obj.object.type_dependant_handle.load(std::memory_order_acquire)));
					if ((gobj.graphics_object->flags & graphics::object::do_transparency_check) != 0) {
						graphics::color_rgba pixel;

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
	}
	return false;
}

#include "concurrency_tools\\concurrency_tools.hpp"