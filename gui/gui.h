#pragma once
#include <atomic>
#include "common\\common.h"
#include "concurrency_tools\\concurrency_tools.h"
#include "gui_definitions\\gui_definitions.h"

class texture;
class font;
class data_texture;
class open_gl_wrapper;
class lines;
enum class rotation;

namespace graphics {
	struct object;
}

struct rbutton_down;
struct mouse_move;
struct mouse_drag;
struct rbutton_up;
struct lbutton_down;
struct resize;
struct lbutton_up;
struct scroll;
struct key_down;
struct key_up;
struct text_event;

namespace ui {
	enum class text_color : uint8_t {
		black, white, red, green, yellow,
		outlined_white, outlined_black
	};
	struct text_instance {
		uint8_t font_handle;
		text_color color = text_color::black;
		uint8_t size; // *2 = display size
		uint8_t length;
		char16_t text[30];
	};
	struct graphics_instance {
		texture* t;
		graphics::object* graphics_object;
		int64_t frame;
	};
	struct flag_instance {
		texture* flag;
		texture* mask;
		texture* overlay;
	};

	class gui_object;
	class gui_manager;

	class gui_behavior {
	public:
		virtual bool on_lclick(gui_object& owner, gui_manager& manager, const lbutton_down&) { return false; };
		virtual bool on_rclick(gui_object& owner, gui_manager& manager, const rbutton_down&) { return false; };
		virtual bool on_drag(gui_object& owner, gui_manager& manager, const mouse_drag&) { return false; };
		virtual bool on_keydown(gui_object& owner, gui_manager& manager, const key_down&) { return false; };
		virtual bool on_text(gui_object& owner, gui_manager& manager, const text_event&) { return false; };
		virtual bool on_scroll(gui_object& owner, gui_manager& manager, const scroll&) { return false; };
		virtual bool on_get_focus(gui_object& owner, gui_manager& manager) { return false; };
		virtual void on_lose_focus(gui_object& owner, gui_manager& manager) { };
		virtual void update_data(gui_object& owner, gui_manager& manager) {};
		virtual bool has_tooltip(gui_object& owner, gui_manager& manager) { return false; };
		virtual void create_tooltip(gui_object& owner, gui_manager& manager, gui_object& tooltip_window) { };
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

		static constexpr uint16_t type_mask = 0x000F;
		static constexpr uint16_t type_none = 0x0000;
		static constexpr uint16_t type_graphics_object = 0x0001; // type_dependant_handle = graphics_instance
		static constexpr uint16_t type_text_instance = 0x0002; // type_dependant_handle = text_instance
		static constexpr uint16_t type_barchart = 0x0003; // type_dependant_handle = data_texture
		static constexpr uint16_t type_piechart = 0x0004; // type_dependant_handle = data_texture
		static constexpr uint16_t type_linegraph = 0x0005; // type_dependant_handle = lines
		static constexpr uint16_t type_masked_flag = 0x0006; // type_dependant_handle = flag_instance

		gui_behavior* associated_behavior = nullptr; //8bytes

		xy_pair size; //12bytes
		xy_pair position; //16bytes

		std::atomic<uint16_t> type_dependant_handle; // 18bytes

		std::atomic<uint16_t> parent = 0; //20bytes
		std::atomic<uint16_t> first_child = 0; //22 bytes
		std::atomic<uint16_t> left_sibling = 0; //24 bytes
		std::atomic<uint16_t> right_sibling = 0; //26 bytes

		std::atomic<uint16_t> flags = 0; // 28 bytes

		char padding[4]; //32 bytes

		const rotation get_rotation() const;
	};

	class gui_manager {
	private:
		void render_object_type(open_gl_wrapper&, const gui_object&, ui::xy_pair position, uint32_t type);
		void render_internal(open_gl_wrapper&, const gui_object&, ui::xy_pair position);
		template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
		bool dispatch_message_internal(const MESSAGE_FUNCTION &member_f, gui_object& obj, const MESSAGE_TYPE& message);
	public:
		fixed_sz_deque<gui_object, 128, 64> gui_objects;
		fixed_sz_deque<text_instance, 128, 64> text_instances;
		fixed_sz_deque<graphics_instance, 128, 64> graphics_instances;
		fixed_sz_deque<flag_instance, 64, 64> flag_instances;
		fixed_sz_deque<data_texture, 64, 16> data_textures;
		fixed_sz_deque<font, 32, 8> fonts;
		fixed_sz_deque<lines, 32, 8> lines_set;

		gui_object& root;
		gui_object& background;
		gui_object& foreground;
		gui_object& tooltip_window;

		gui_object* focus = nullptr;
		gui_object* tooltip = nullptr;

		float scale = 1.0f;

		gui_manager(int32_t width, int32_t height);

		void on_resize(const resize&);
		bool on_lbutton_down(const lbutton_down&);
		bool on_rbutton_down(const rbutton_down&);
		bool on_mouse_move(const mouse_move&);
		bool on_mouse_drag(const mouse_drag&);
		bool on_keydown(const key_down&);
		bool on_text(const text_event&);
		bool on_scroll(const scroll&);

		void render(open_gl_wrapper&);

		void set_visible(gui_object& g, bool visible);
		void set_enabled(gui_object& g, bool enabled);
		void set_focus(gui_object& g);
		void clear_focus();
		void hide_tooltip();
		void clear_children(gui_object& g);
		void remove_object(gui_object& g);
		void move_to_front(gui_object& g);
		void move_to_back(gui_object& g);
		void destroy(gui_object& g);
		~gui_manager();
	};
}