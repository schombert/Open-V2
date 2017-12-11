#include "gui.hpp"
#include "graphics_objects\\graphics_objects.h"
#include "graphics\\open_gl_wrapper.h"

ui::gui_manager::~gui_manager() {};

void ui::gui_manager::destroy(gui_object & g) {
	if (g.associated_behavior) {
		if ((g.flags.load(std::memory_order_relaxed) & g.static_behavior) == 0) {
			g.associated_behavior->~gui_behavior();
			concurrent_allocator<gui_behavior>().deallocate(g.associated_behavior, 0);
		}
		g.associated_behavior = nullptr;
	}
	const auto type_handle = g.type_dependant_handle.load(std::memory_order_relaxed);
	if (type_handle != 0) {
		const auto flags = g.flags.load(std::memory_order_relaxed);
		if ((flags & gui_object::type_mask) == gui_object::type_text_instance) {
			text_instances.free(type_handle - 1);
		} else if ((flags & gui_object::type_mask) == gui_object::type_graphics_object) {
			graphics_instances.free(type_handle - 1);
		} else if ((flags & gui_object::type_mask) == gui_object::type_masked_flag) {
			flag_instances.free(type_handle - 1);
		}
	}

	auto child = g.first_child.load(std::memory_order_relaxed);
	g.first_child.store(0, std::memory_order_release);

	while (child != 0) {
		auto next = gui_objects.at(child).right_sibling.load(std::memory_order_relaxed);
		gui_objects.free(child - 1, *this);
		child = next;
	}

	g.parent.store(0, std::memory_order_release);
	g.left_sibling.store(0, std::memory_order_release);
	g.right_sibling.store(0, std::memory_order_release);
	g.flags.store(0, std::memory_order_release);
	g.type_dependant_handle.store(0, std::memory_order_release);
}
