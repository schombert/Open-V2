#pragma once
#include <atomic>
#include "common\\common.h"
#include "concurrency_tools\\concurrency_tools.h"
#include "gui_definitions\\gui_definitions.h"
#include "graphics_objects\\graphics_objects.h"

class texture;

namespace ui {
	struct text_instance {
		uint8_t color;
		uint8_t padding;
		char16_t text[15];
	};
	struct graphics_instance {
		texture* t;
		graphics::object* graphics_object;
		uint8_t frame;
		uint8_t rotation;
	};
	struct flag_instance {
		texture* flag;
		texture* mask;
		texture* overlay;
		uint8_t rotation;
	};

	class gui_object;

	class gui_behavior {
	public:
		virtual bool on_lclick(gui_object& owner, xy_pair pos, modifiers m) { return false; };
		virtual bool on_rclick(gui_object& owner, xy_pair pos, modifiers m) { return false; };
		virtual bool on_drag(gui_object& owner, xy_pair pos, modifiers m) { return false; };
		virtual bool on_keydown(gui_object& owner, xy_pair pos, virtual_key key, modifiers m) { return false; };
		virtual bool on_text(gui_object& owner, uint16_t code) { return false; };
		virtual void on_get_focus(gui_object& owner) { };
		virtual void on_lose_focus(gui_object& owner) { };
		virtual bool has_tooltip(gui_object& owner) { return false; };
		virtual xy_pair create_tooltip(gui_object& owner, gui_object& tooltip_window) { return xy_pair(); };
		virtual ~gui_behavior() {};
	};

	class gui_object {
	public:
		static constexpr uint16_t static_behavior = 0x0100;
		static constexpr uint16_t visible = 0x0200;
		static constexpr uint16_t enabled = 0x0400;

		static constexpr uint16_t rotation_mask = 0x0030;
		static constexpr uint16_t rotation_upright = 0x0000;
		static constexpr uint16_t rotation_right = 0x0010;
		static constexpr uint16_t rotation_left = 0x0020;

		static constexpr uint16_t alignment_mask = 0xF000;
		static constexpr uint16_t alignment_none = 0x0000;
		static constexpr uint16_t alignment_center = 0x1000;
		static constexpr uint16_t alignment_center_left = 0x2000;
		static constexpr uint16_t alignment_top_center = 0x3000;
		static constexpr uint16_t alignment_top_left = 0x4000;
		static constexpr uint16_t alignment_bottom_left = 0x5000;
		static constexpr uint16_t alignment_bottom_center = 0x6000;
		static constexpr uint16_t alignment_vertical_left = 0x7000;
		static constexpr uint16_t alignment_vertical_center = 0x8000;

		static constexpr uint16_t type_mask = 0x000F;
		static constexpr uint16_t type_none = 0x0000;
		static constexpr uint16_t type_graphics_object = 0x0001; // type_dependant_handle = graphics_instance
		static constexpr uint16_t type_text_instance = 0x0002; // type_dependant_handle = text_instance
		static constexpr uint16_t type_barchart = 0x0003; // type_dependant_handle = data_texture
		static constexpr uint16_t type_piechart = 0x0004; // type_dependant_handle = data_texture
		static constexpr uint16_t type_linegraph = 0x0005; // type_dependant_handle = data_texture
		static constexpr uint16_t type_masked_flat = 0x0006; // type_dependant_handle = flag_instance

		gui_behavior* associated_behavior = nullptr; //8bytes

		xy_pair size; //12bytes
		xy_pair position; //16bytes

		std::atomic<uint16_t> type_dependant_handle; // 18bytes

		std::atomic<uint16_t> parent = 0; //20bytes
		std::atomic<uint16_t> first_child = 0; //22 bytes
		std::atomic<uint16_t> left_sibling = 0; //24 bytes
		std::atomic<uint16_t> right_sibling = 0; //26 bytes

		std::atomic<uint16_t> flags = 0; // 28 bytes

		std::atomic<bool> alignment_pending = false; // 29 bytes

		char padding[3]; //32 bytes
	};

	class gui_manager {
	public:
		void destroy(gui_object& g) {
			if (g.associated_behavior) {
				if ((g.flags.load(std::memory_order_relaxed) & g.static_behavior) == 0) {
					g.associated_behavior->~gui_behavior();
					concurrent_allocator<gui_behavior>().deallocate(g.associated_behavior, 0);
				}
				g.associated_behavior = nullptr;
			}
			g.alignment_pending.store(false, std::memory_order_release);
			g.parent.store(0, std::memory_order_release);
			g.first_child.store(0, std::memory_order_release);
			g.left_sibling.store(0, std::memory_order_release);
			g.right_sibling.store(0, std::memory_order_release);
			g.flags.store(0, std::memory_order_release);
			g.type_dependant_handle.store(0, std::memory_order_release);
		}
	};
}