#pragma once
#include "gui.h"
#include "graphics\\v2_window.h"

template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
bool ui::gui_manager::dispatch_message_internal(MESSAGE_FUNCTION member_f, gui_object& obj, MESSAGE_TYPE message) {
	auto relocated_message = adjust_message_location(message, -obj.position.x * scale, -obj.position.y * scale);

	if (message_within_bounds(relocated_message, obj.size.x * scale, obj.size.y * scale)) {
		auto child = obj.first_child.load(std::memory_order_acquire);
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

			if (dispatch_message_internal(member_f, current_obj, relocated_message))
				return true;

			child = next_index;
		}

		if (obj.associated_behavior) {
			if ((associated_behavior->*member_f)(obj, message))
				return true;
		}
	}
	return false;
}

#include "concurrency_tools\\concurrency_tools.hpp"