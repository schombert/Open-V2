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
#include "simple_fs\\simple_fs.h"
#include "boost\\container\\small_vector.hpp"

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
		uint8_t size = 8; // *2 = display size
		uint8_t length = 0;
		char16_t text[max_instance_length] = {0};
	};
	struct graphics_instance {
		graphics::texture* t = nullptr;
		graphics::object* graphics_object = nullptr;
		int64_t frame = 0;
	};
	struct multi_texture_instance {
		graphics::texture* flag = nullptr;
		graphics::texture* mask_or_primary = nullptr;
		graphics::texture* overlay_or_secondary = nullptr;
		float progress = 0.0f;
	};

	class gui_object;
	class gui_manager;

	using tagged_gui_object = tagged_object<gui_object, gui_object_tag>;

	enum class tooltip_behavior {
		tooltip,
		no_tooltip,
		transparent
	};

	class gui_behavior {
	public:
		virtual bool on_lclick(tagged_gui_object, gui_manager&, const lbutton_down&) { return false; };
		virtual bool on_rclick(tagged_gui_object, gui_manager&, const rbutton_down&) { return false; };
		virtual bool on_drag(tagged_gui_object, gui_manager&, const mouse_drag&) { return false; };
		virtual bool on_keydown(tagged_gui_object, gui_manager&, const key_down&) { return false; };
		virtual bool on_text(tagged_gui_object, gui_manager&, const text_event&) { return false; };
		virtual bool on_scroll(tagged_gui_object, gui_manager&, const scroll&) { return false; };
		virtual bool on_get_focus(tagged_gui_object, gui_manager&) { return false; };
		virtual void on_lose_focus(tagged_gui_object, gui_manager&) { };
		virtual void update_data(tagged_gui_object, gui_manager&) {};
		virtual tooltip_behavior has_tooltip(tagged_gui_object, gui_manager&) { return tooltip_behavior::transparent; };
		virtual void create_tooltip(tagged_gui_object, gui_manager&, tagged_gui_object /*tooltip_window*/) { };
		virtual ~gui_behavior() {};
	};

	class visible_region : public gui_behavior {
	public:
		virtual bool on_lclick(tagged_gui_object, gui_manager&, const lbutton_down&) override { return true; };
		virtual bool on_rclick(tagged_gui_object, gui_manager&, const rbutton_down&) override { return true; };
		virtual bool on_scroll(tagged_gui_object, gui_manager&, const scroll&) override { return true; };
		virtual tooltip_behavior has_tooltip(tagged_gui_object, gui_manager&) override { return tooltip_behavior::no_tooltip; };
	};

	extern visible_region global_visible_region;

	template<typename BASE>
	class simple_button : public visible_region, BASE {
	public:
		virtual_key shortcut = virtual_key::NONE;

		template<typename ...P>
		explicit simple_button(P&& ... params) : BASE(std::forward<P>(params)...) {};

		template<typename T>
		simple_button(const T& f) : button_function(f) {};

		virtual bool on_lclick(tagged_gui_object o, gui_manager& m, const lbutton_down&) override;
		virtual bool on_keydown(tagged_gui_object o, gui_manager& m, const key_down& k) override;
		virtual void update_data(tagged_gui_object, gui_manager&) override;
	};

	template<typename BASE>
	class scrollbar : public visible_region, BASE {
	private:
		int32_t _position = 0;
		int32_t maximum = 0;
		int32_t minimum = 0;
		int32_t limt_maximum = 0;
		int32_t limit_minimum = 0;
		int32_t _step_size = 1;
	public:
		int32_t valid_start = 0;
		int32_t valid_end = 0;

		gui_object* minimum_limit_icon = nullptr;
		gui_object_tag lmin_tag;
		gui_object* maximum_limit_icon = nullptr;
		gui_object_tag lmax_tag;
		gui_object* slider = nullptr;

		bool vertical;

		template<typename ... PARAMS>
		scrollbar(bool vert, int32_t mini, int32_t maxi, int32_t step, PARAMS&& ... params);
		template<typename ... PARAMS>
		scrollbar(PARAMS&& ... params) : BASE(std::forward<PARAMS>(params)...) {};

		int32_t position() const { return _position; }
		std::pair<int32_t, int32_t> range() const { return std::make_pair(minimum, maximum); }
		std::pair<int32_t, int32_t> track_range() const { return std::make_pair(valid_start, valid_end); }
		int32_t step_size() const { return _step_size; }

		void initialize(bool vert, int32_t mini, int32_t maxi, int32_t step);
		void update_limit_icons(gui_manager& m);
		void adjust_position(int32_t position); // will call BASE::on_position
		void update_position(int32_t position); // will not call BASE::on_position
		void set_limits(gui_manager& m, int32_t lmin, int32_t lmax);
		void set_range(gui_manager& m, int32_t rmin, int32_t rmax);
		void set_step(int32_t s) { _step_size = s; }

		virtual bool on_scroll(tagged_gui_object, gui_manager&, const scroll&) override;
		virtual void update_data(tagged_gui_object, gui_manager&) override;
	};

	template<typename BASE, typename ELEMENT>
	class managed_listbox : public visible_region, BASE {
	private:
		gui_object* contents_frame = nullptr;
		std::vector<ELEMENT, concurrent_allocator<ELEMENT>> contents;
		std::vector<tagged_gui_object, concurrent_allocator<tagged_gui_object>> gui_items;
	public:
		virtual bool on_scroll(tagged_gui_object, gui_manager&, const scroll&) override;
		virtual void update_data(tagged_gui_object, gui_manager&) override;
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
		static constexpr uint16_t type_masked_flag = 0x0006; // type_dependant_handle = multi_texture_instance
		static constexpr uint16_t type_progress_bar = 0x0007; // type_dependant_handle = multi_texture_instance

		gui_behavior* associated_behavior = nullptr; //8bytes

		xy_pair size; //12bytes
		xy_pair position; //16bytes

		std::atomic<uint16_t> type_dependant_handle; // 18bytes

		atomic_tag<gui_object_tag> parent; //20bytes
		atomic_tag<gui_object_tag> first_child; //22 bytes
		atomic_tag<gui_object_tag> left_sibling; //24 bytes
		atomic_tag<gui_object_tag> right_sibling; //26 bytes

		std::atomic<uint16_t> flags = visible | enabled; // 28 bytes

		char padding[4]; //32 bytes

		graphics::rotation get_rotation() const;
	};

	struct text_format {
		ui::text_color color;
		graphics::font_tag font_handle;
		uint32_t font_size;
	};

	class gui_manager;

	namespace detail {
		void render_object_type(const gui_manager& manager, graphics::open_gl_wrapper&, const gui_object&, ui::xy_pair position, uint32_t type, bool currently_enabled);
		void render(const gui_manager& manager, graphics::open_gl_wrapper&, const gui_object&, ui::xy_pair position, bool parent_enabled);

		void create_linear_text(gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format&, const text_data::replacement* candidates = nullptr, uint32_t count = 0);
		void create_multiline_text(gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format&, const text_data::replacement* candidates = nullptr, uint32_t count = 0);

		tagged_gui_object last_sibling_of(const gui_manager& manager, tagged_gui_object g);

		tagged_gui_object create_element_instance(gui_manager& manager, button_tag handle);
		tagged_gui_object create_element_instance(gui_manager& manager, icon_tag handle);
		tagged_gui_object create_element_instance(gui_manager& manager, ui::text_tag handle, const text_data::replacement* candidates = nullptr, uint32_t count = 0);

		void update(gui_manager& manager, tagged_gui_object obj);

		template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
		bool dispatch_message(const gui_manager& manager, const MESSAGE_FUNCTION &member_f, tagged_gui_object obj, const MESSAGE_TYPE& message);
	}

	template<typename T, typename B>
	ui::tagged_gui_object create_static_button(gui_manager& manager, T handle, tagged_gui_object parent, B& b);
	template<typename BEHAVIOR, typename T, typename ... PARAMS>
	ui::tagged_gui_object create_dynamic_button(gui_manager& manager, T handle, tagged_gui_object parent, PARAMS&& ... params);
	template<typename BEHAVIOR, typename ... PARAMS>
	ui::tagged_gui_object create_scrollbar (gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, PARAMS&& ... params);
	template<typename BEHAVIOR, typename ... PARAMS>
	ui::tagged_gui_object create_fixed_sz_scrollbar(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, PARAMS&& ... params);
	template<typename BEHAVIOR>
	ui::tagged_gui_object create_static_scrollbar(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, scrollbar<BEHAVIOR>& b);
	template<typename BEHAVIOR>
	ui::tagged_gui_object create_static_fixed_sz_scrollbar(gui_manager& manager, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, scrollbar<BEHAVIOR>& b);
	template<typename FILL_FUNCTION>
	ui::tagged_gui_object create_scrollable_region(gui_manager& manager, tagged_gui_object parent, ui::xy_pair position, int32_t height, int32_t step_size, graphics::obj_definition_tag bg, const FILL_FUNCTION& f);
	ui::tagged_gui_object create_scrollable_text_block(gui_manager& manager, ui::text_tag handle, tagged_gui_object parent, const text_data::replacement* candidates = nullptr, uint32_t count = 0);
	ui::tagged_gui_object create_scrollable_text_block(gui_manager& manager, ui::text_tag handle, text_data::text_tag contents, tagged_gui_object parent, const text_data::replacement* candidates = nullptr, uint32_t count = 0);

	class line_manager {
	private:
		boost::container::small_vector<gui_object*, 16, concurrent_allocator<gui_object*>> current_line;
		const text_data::alignment align;
		const int32_t max_line_extent;
	public:
		line_manager(text_data::alignment a, int32_t m) : align(a), max_line_extent(m) {}
		bool exceeds_extent(int32_t w) const;
		void add_object(gui_object* o);
		void finish_current_line();
	};

	class single_line_manager {
	public:
		bool exceeds_extent(int32_t) const { return false; };
		void add_object(gui_object*) const {}
		void finish_current_line() const {}
	};

	class null_behavior_creation {
	public:
		void operator()(tagged_gui_object) const {};
	};
	
	void shorten_text_instance_to_space(ui::text_instance& txt);
	float text_component_width(text_data::text_component& c, const std::vector<char16_t>& text_data, graphics::font& this_font, uint32_t font_size);

	template<typename LM, typename BH = null_behavior_creation>
	ui::xy_pair text_chunk_to_instances(ui::gui_manager& container, vector_backed_string<char16_t> text_source, tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, LM& lm, const BH& behavior_creator = null_behavior_creation());

	void clear_children(gui_manager& manager, tagged_gui_object g);
	void remove_object(gui_manager& manager, tagged_gui_object g);
	void move_to_front(const gui_manager& manager, tagged_gui_object g);
	void move_to_back(const gui_manager& manager, tagged_gui_object g);
	void add_to_front(const gui_manager& manager, tagged_gui_object parent, tagged_gui_object child);
	void add_to_back(const gui_manager& manager, tagged_gui_object parent, tagged_gui_object child);

	void make_visible(gui_manager& manager, tagged_gui_object g);
	void hide(tagged_gui_object g);
	void set_enabled(tagged_gui_object g, bool enabled);
	void shrink_to_children(gui_manager& manager, tagged_gui_object g);
	void shrink_to_children(gui_manager& manager, tagged_gui_object g, int32_t border);

	ui::xy_pair absolute_position(gui_manager& manager, tagged_gui_object g);

	void render(const gui_manager& manager, graphics::open_gl_wrapper&);
	void update(gui_manager& manager);

	template<typename T>
	void for_each_child(gui_manager& manager, tagged_gui_object parent, const T& f);

	class gui_manager {
	private:
		float _scale = 1.0f;
		int32_t _width;
		int32_t _height;
	public:
		fixed_sz_deque<gui_object, 128, 64, gui_object_tag> gui_objects;
		fixed_sz_deque<text_instance, 128, 64, text_instance_tag> text_instances;
		fixed_sz_deque<graphics_instance, 128, 64, graphics_instance_tag> graphics_instances;
		fixed_sz_deque<multi_texture_instance, 64, 64, multi_texture_instance_tag> multi_texture_instances;
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

		void rescale(float new_scale);
		float scale() const { return _scale; }
		int32_t width() const { return _width; }
		int32_t height() const { return _height; }
		bool set_focus(tagged_gui_object g);
		void clear_focus();
		void hide_tooltip();
		
		void destroy(gui_object& g);
		~gui_manager();
	};

	void load_gui_from_directory(const directory& source_directory, gui_manager& manager);
}