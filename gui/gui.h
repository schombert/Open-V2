#pragma once
#include <atomic>
#include "common\\common.h"
#include "concurrency_tools\\concurrency_tools.h"
#include "gui_definitions\\gui_definitions.h"
#include "graphics\\texture.h"
#include "graphics\\text.h"
#include "text_data\\text_data.h"
#include "graphics_objects\\graphics_objects.h"
#include "common\\shared_tags.h"

namespace graphics {
	class font;
	class open_gl_wrapper;
	class lines;
	enum class rotation;

	struct object;
}

namespace ui {
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

	enum class text_color : uint8_t {
		black, white, red, green, yellow,
		outlined_white, outlined_black
	};
	struct text_instance {
		static constexpr uint32_t max_instance_length = 30;

		graphics::font_tag font_handle;
		text_color color = text_color::black;
		uint8_t size; // *2 = display size
		uint8_t length;
		char16_t text[max_instance_length];
	};
	struct graphics_instance {
		graphics::texture* t;
		graphics::object* graphics_object;
		int64_t frame;
	};
	struct flag_instance {
		graphics::texture* flag;
		graphics::texture* mask;
		graphics::texture* overlay;
	};

	class gui_object;
	class gui_manager;

	using tagged_gui_object = tagged_object<gui_object, gui_object_tag>;

	class gui_behavior {
	public:
		virtual bool on_lclick(tagged_gui_object owner, gui_manager& manager, const lbutton_down&) { return false; };
		virtual bool on_rclick(tagged_gui_object owner, gui_manager& manager, const rbutton_down&) { return false; };
		virtual bool on_drag(tagged_gui_object owner, gui_manager& manager, const mouse_drag&) { return false; };
		virtual bool on_keydown(tagged_gui_object owner, gui_manager& manager, const key_down&) { return false; };
		virtual bool on_text(tagged_gui_object owner, gui_manager& manager, const text_event&) { return false; };
		virtual bool on_scroll(tagged_gui_object owner, gui_manager& manager, const scroll&) { return false; };
		virtual bool on_get_focus(tagged_gui_object owner, gui_manager& manager) { return false; };
		virtual void on_lose_focus(tagged_gui_object owner, gui_manager& manager) { };
		virtual void update_data(tagged_gui_object owner, gui_manager& manager) {};
		virtual bool has_tooltip(tagged_gui_object owner, gui_manager& manager) { return false; };
		virtual void create_tooltip(tagged_gui_object owner, gui_manager& manager, tagged_gui_object tooltip_window) { };
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

		atomic_tag<gui_object_tag> parent; //20bytes
		atomic_tag<gui_object_tag> first_child; //22 bytes
		atomic_tag<gui_object_tag> left_sibling; //24 bytes
		atomic_tag<gui_object_tag> right_sibling; //26 bytes

		std::atomic<uint16_t> flags = 0; // 28 bytes

		char padding[4]; //32 bytes

		const graphics::rotation get_rotation() const;
	};

	class gui_manager {
	private:
		void render_object_type(graphics::open_gl_wrapper&, const gui_object&, ui::xy_pair position, uint32_t type, bool currently_enabled);
		void render_internal(graphics::open_gl_wrapper&, const gui_object&, ui::xy_pair position, bool parent_enabled);

		tagged_gui_object create_element_instance(uint16_t packed_handle);
		void create_simple_single_line_text(tagged_gui_object container, text_data::text_tag text_handle, ui::text_color default_color, text_data::alignment align, graphics::font_tag font_handle, uint32_t font_size);

		template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
		bool dispatch_message_internal(const MESSAGE_FUNCTION &member_f, tagged_gui_object obj, const MESSAGE_TYPE& message);

		tagged_gui_object last_sibling_of(tagged_gui_object g);

		float scale = 1.0f;
	public:
		fixed_sz_deque<gui_object, 128, 64, gui_object_tag> gui_objects;
		fixed_sz_deque<text_instance, 128, 64, text_instance_tag> text_instances;
		fixed_sz_deque<graphics_instance, 128, 64, graphics_instance_tag> graphics_instances;
		fixed_sz_deque<flag_instance, 64, 64, flag_instance_tag> flag_instances;
		fixed_sz_deque<graphics::data_texture, 64, 16, data_texture_tag> data_textures;
		fixed_sz_deque<graphics::lines, 32, 8, lines_tag> lines_set;

		graphics::texture_manager textures;
		graphics::font_manager fonts;

		ui::definitions ui_definitions;
		graphics::object_definitions graphics_object_definitions;
		text_data::text_sequences text_data_sequences;

		gui_object& root;
		gui_object& background;
		gui_object& foreground;
		gui_object& tooltip_window;

		gui_object_tag focus;
		gui_object_tag tooltip;

		

		gui_manager(int32_t width, int32_t height);

		
		void on_resize(const resize&);
		bool on_lbutton_down(const lbutton_down&);
		bool on_rbutton_down(const rbutton_down&);
		bool on_mouse_move(const mouse_move&);
		bool on_mouse_drag(const mouse_drag&);
		bool on_keydown(const key_down&);
		bool on_text(const text_event&);
		bool on_scroll(const scroll&);

		void render(graphics::open_gl_wrapper&);

		void rescale(float new_scale);

		void set_visible(tagged_gui_object g, bool visible);
		void set_enabled(tagged_gui_object g, bool enabled);
		void set_focus(tagged_gui_object g);
		void clear_focus();
		void hide_tooltip();
		void clear_children(tagged_gui_object g);
		void remove_object(tagged_gui_object g);
		void move_to_front(tagged_gui_object g);
		void move_to_back(tagged_gui_object g);
		void add_to_front(tagged_gui_object parent, tagged_gui_object child);
		void add_to_back(tagged_gui_object parent, tagged_gui_object child);
		void destroy(gui_object& g);
		~gui_manager();
	};
}