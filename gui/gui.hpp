#pragma once
#include "gui.h"
#include "graphics\\v2_window.h"
#include "world_state\\world_state.h"

#undef max
#undef min

namespace ui {
	class unmanaged_scrollable_region : public visible_region {
	public:
		scrollbar<unmanaged_region_scrollbar> sb;

		unmanaged_scrollable_region(gui_object& g);
		virtual bool on_scroll(gui_object_tag o, world_state& m, const scroll& s) override;
	};
}

template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
bool ui::detail::dispatch_message(const gui_manager& manager, const MESSAGE_FUNCTION &member_f, ui::tagged_gui_object obj, ui::xy_pair container_size, const MESSAGE_TYPE& message) {
	const auto actual_position = ui::detail::position_with_alignment(container_size, obj.object.position, obj.object.align);

	auto relocated_message = adjust_message_location(message, -actual_position.x, -actual_position.y);
	const auto object_flags = obj.object.flags.load(std::memory_order_acquire);

	if ((object_flags & ui::gui_object::visible) == 0)
		return false;
	
	if (message_within_bounds(relocated_message, obj.object.size.x, obj.object.size.y)) {
	
		if (std::is_same_v<MESSAGE_TYPE, ui::mouse_move> || (object_flags & ui::gui_object::enabled) != 0) {
			const gui_object_tag child = obj.object.first_child;
			if (is_valid_index(child)) {
				gui_object_tag current_child = detail::last_sibling_of(manager, tagged_gui_object{ manager.gui_objects.at(child), child }).id;
				do {
					auto& current_obj = manager.gui_objects.at(current_child);
					const gui_object_tag next_index = current_obj.left_sibling;

					if (dispatch_message(manager, member_f, tagged_gui_object{ current_obj, current_child }, obj.object.size, relocated_message))
						return true;

					current_child = next_index;
				} while (is_valid_index(current_child));
			}
		}

		if (std::is_same_v<MESSAGE_TYPE, ui::mouse_move> || (object_flags & ui::gui_object::enabled) != 0) {
			if constexpr(std::is_base_of_v<message_with_location, MESSAGE_TYPE>) {
				if ((object_flags & ui::gui_object::type_mask) == ui::gui_object::type_graphics_object) {
					const auto& gobj = manager.graphics_instances.at(graphics_instance_tag(obj.object.type_dependant_handle.load(std::memory_order_acquire)));
					if (((object_flags & ui::gui_object::force_transparency_check) != 0) | ((gobj.graphics_object->flags & graphics::object::do_transparency_check) != 0)) {
						graphics::color_rgba pixel;

						if (gobj.graphics_object->number_of_frames > 1) {
							pixel = gobj.t->get_pixel(
								(static_cast<float>(relocated_message.x + obj.object.size.x * gobj.frame)) / (static_cast<float>(obj.object.size.x * gobj.graphics_object->number_of_frames)),
								static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y)));
						} else {
							pixel = gobj.t->get_pixel(
								static_cast<float>(relocated_message.x) / (static_cast<float>(obj.object.size.x)),
								static_cast<float>(relocated_message.y) / (static_cast<float>(obj.object.size.y)));
						}

						if (pixel.a < 127)
							return false;
					}
				}
			}

			if (member_f(obj, relocated_message))
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
		template<typename RES, typename HANDLE>
		struct can_create_instance_s : public std::false_type {};
		template<typename HANDLE>
		struct can_create_instance_s<decltype(create_element_instance(std::declval<gui_static&>(), std::declval<gui_manager&>(), std::declval<HANDLE>())), HANDLE> : public std::true_type {};
		template<typename HANDLE>
		constexpr bool can_create_instance = can_create_instance_s<tagged_gui_object, HANDLE>::value;

		template<typename T>
		ui::tagged_gui_object safe_create_element_instance(gui_static& static_manager, gui_manager& manager, T handle) {
			if constexpr(can_create_instance<T>)
				return ui::detail::create_element_instance(static_manager, manager, handle);
			else
				return manager.gui_objects.emplace();
		}

		template<typename RES, typename HANDLE, typename TYPE>
		struct can_create_static_s : public std::false_type {};
		template<typename HANDLE, typename TYPE>
		struct can_create_static_s<decltype(create_static_element(std::declval<gui_static&>(), std::declval<gui_manager&>(), std::declval<HANDLE>(), std::declval<tagged_gui_object>(), std::declval<TYPE&>())), HANDLE, TYPE> : public std::true_type {};
		template<typename HANDLE, typename TYPE>
		constexpr bool can_create_static = can_create_static_s<tagged_gui_object, HANDLE, TYPE>::value;
	}
}

template<typename BEHAVIOR, typename T, typename ... PARAMS>
ui::tagged_gui_object ui::create_dynamic_element(world_state& ws, T handle, tagged_gui_object parent, PARAMS&& ... params) {
	if constexpr(!std::is_same_v<BEHAVIOR, gui_behavior>) {
		BEHAVIOR* b = concurrent_allocator<BEHAVIOR>().allocate(1);
		new (b)BEHAVIOR(std::forward<PARAMS>(params) ...);

		if constexpr(detail::can_create_static<T, BEHAVIOR>) {
			const auto new_obj = create_static_element(ws, handle, parent, *b);
			new_obj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
			return new_obj;
		} else {
			auto new_obj = ui::detail::safe_create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);
			new_obj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);

			new_obj.object.associated_behavior = b;
			b->associated_object = &new_obj.object;

			ui::add_to_back(ws.w.gui_m, parent, new_obj);
			return new_obj;
		}
	} else {
		auto new_obj = ui::detail::safe_create_element_instance(ws.s.gui_m, ws.w.gui_m, handle);
		ui::add_to_back(ws.w.gui_m, parent, new_obj);
		return new_obj;
	}
}

template<typename BEHAVIOR, typename ... PARAMS>
void ui::attach_dynamic_behavior(gui_manager& m, ui::tagged_gui_object target_object, PARAMS&& ... params) {
	BEHAVIOR* b = concurrent_allocator<BEHAVIOR>().allocate(1);
	new (b)BEHAVIOR(std::forward<PARAMS>(params) ...);

	auto& obj = m.gui_objects.at(target_object);
	obj.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);

	assert(obj.associated_behavior == nullptr);
	obj.associated_behavior = b;
	b->associated_object = &obj;
}

template<typename FILL_FUNCTION>
ui::tagged_gui_object ui::create_scrollable_region(world_state& ws, tagged_gui_object parent, ui::xy_pair position, int32_t height, int32_t step_size, graphics::obj_definition_tag , const FILL_FUNCTION& f) {
	const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
	new_gobj.object.position = position;

	const auto inner_area = f(ws);
	inner_area.object.position = ui::xy_pair{ 0,0 };
	new_gobj.object.size = ui::xy_pair{ static_cast<int16_t>(inner_area.object.size.x + 16), static_cast<int16_t>(height) };

	ui::add_to_back(ws.w.gui_m, new_gobj, inner_area);

	if (inner_area.object.size.y > height) {
		unmanaged_scrollable_region* new_sr = concurrent_allocator<unmanaged_scrollable_region>().allocate(1);
		new (new_sr)unmanaged_scrollable_region(inner_area.object);
		

		int32_t size_difference = inner_area.object.size.y - height;
		ui::create_static_fixed_sz_scrollbar(ws, ws.s.gui_m.ui_definitions.standardlistbox_slider, new_gobj, ui::xy_pair{ inner_area.object.size.x, 0 }, height, new_sr->sb);
		new_sr->sb.set_range(ws.w.gui_m, 0, size_difference);
		new_sr->sb.set_step(step_size);

		new_gobj.object.associated_behavior = new_sr;
		new_sr->associated_object = &new_gobj.object;
		new_gobj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
	} else {
		ui::visible_region* new_vr = concurrent_allocator<ui::visible_region>().allocate(1);
		new (new_vr)ui::visible_region();

		new_gobj.object.associated_behavior = new_vr;
		new_vr->associated_object = &new_gobj.object;
		new_gobj.object.flags.fetch_or(ui::gui_object::dynamic_behavior, std::memory_order_acq_rel);
	}

	ui::add_to_back(ws.w.gui_m, parent, new_gobj);
	return new_gobj;
}

namespace ui {
	namespace detail {
		template<typename RESULT, typename tag_type, typename object_type>
		struct can_create_s : public std::false_type {};
		template<typename tag_type, typename object_type>
		struct can_create_s<decltype(void(ui::create_static_element(std::declval<world_state&>(), tag_type(), std::declval<tagged_gui_object>(), std::declval<object_type&>()))), tag_type, object_type> : public std::true_type {};
		
		template<typename tag_type, typename object_type>
		constexpr bool can_create = can_create_s<void, tag_type, object_type>::value;
	}
}

#include "buttons.hpp"
#include "scrollbar.hpp"
#include "piechart.hpp"
#include "gui_window.hpp"
#include "text_box.hpp"
#include "listbox.hpp"
#include "icons.hpp"
#include "progress_bar.hpp"
#include "linechart.hpp"
#include "barchart.hpp"
#include "edit_box.hpp"
#include "concurrency_tools\\concurrency_tools.hpp"
