#pragma once
#include "common\\common.h"
#include <atomic>
#include "concurrency_tools\\concurrency_tools.h"
#include "gui_definitions\\gui_definitions.h"
#include "graphics\\texture.h"
#include "graphics\\text.h"
#include "text_data\\text_data.h"
#include "graphics_objects\\graphics_objects.h"
#include "common\\shared_tags.h"
#include "simple_mpl\\simple_mpl.hpp"

namespace graphics {
	class font;
	class open_gl_wrapper;
	class lines;
	enum class rotation;

	struct object;
}


class world_state;

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

	constexpr int32_t piechart_resolution = 200;

	enum class text_color : uint8_t {
		black, white, red, green, yellow,
		outlined_white, outlined_black
	};
	constexpr bool is_outlined_color(text_color c) {
		return (c == text_color::outlined_white) | (c == text_color::outlined_black);
	}
	struct alignas(int32_t) text_instance {
		static constexpr uint32_t max_instance_length = 30;

		char16_t text[max_instance_length] = {0};

		graphics::font_tag font_handle;
		text_color color = text_color::black;
		uint8_t size = 8; // *2 = display size
		uint8_t length = 0;
	};
	static_assert(sizeof(graphics::font_tag) == 1);
	static_assert(sizeof(text_color) == 1);
	static_assert(sizeof(text_instance) == 64);

	struct graphics_instance {
		graphics::texture* t = nullptr;
		const graphics::object* graphics_object = nullptr;
		int32_t frame = 0;
	};
	struct multi_texture_instance {
		graphics::texture* mask_or_primary = nullptr;
		graphics::texture* flag_or_secondary = nullptr;
		float progress = 0.0f;
	};
	struct tinted_icon_instance {
		graphics::texture* t = nullptr;
		const graphics::object* graphics_object = nullptr;
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
	};
	struct text_format {
		ui::text_color color;
		graphics::font_tag font_handle;
		uint32_t font_size;
	};

	constexpr ui::text_format tooltip_text_format{ ui::text_color::white, graphics::font_tag(1), 16 };

	class gui_object;
	class gui_manager;
	class gui_static;

	using tagged_gui_object = tagged_object<gui_object, gui_object_tag>;

	enum class tooltip_behavior {
		tooltip,
		variable_tooltip,
		no_tooltip,
		transparent
	};

	class gui_behavior {
	public:
		gui_object* associated_object = nullptr;
		gui_behavior() noexcept {}
		gui_behavior(gui_behavior&& o) noexcept;
		gui_behavior(const gui_behavior& o) = delete;
		template<typename ... PARAMS>
		explicit gui_behavior(PARAMS&& ... params) {}

		virtual bool on_lclick(gui_object_tag, world_state&, const lbutton_down&) { return false; }
		virtual bool on_rclick(gui_object_tag, world_state&, const rbutton_down&) { return false; }
		virtual bool on_drag(gui_object_tag, world_state&, const mouse_drag&) { return false; }
		virtual bool on_keydown(gui_object_tag, world_state&, const key_down&) { return false; }
		virtual bool on_text(gui_object_tag, world_state&, const text_event&) { return false; }
		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) { return false; }
		virtual bool on_get_focus(gui_object_tag, world_state&) { return false; }
		virtual void on_lose_focus(gui_object_tag, world_state&) {}
		virtual void on_visible(gui_object_tag, world_state&) {}
		virtual void update_data(gui_object_tag, world_state&) {}
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) { return tooltip_behavior::transparent; }
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) { }
		virtual ~gui_behavior();
	};

	class visible_region : public gui_behavior {
	public:
		visible_region(visible_region&&) = default;
		template<typename ...P>
		explicit visible_region(P&& ...) {}
		virtual bool on_lclick(gui_object_tag, world_state&, const lbutton_down&) override { return true; }
		virtual bool on_rclick(gui_object_tag, world_state&, const rbutton_down&) override { return true; }
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) override { return tooltip_behavior::no_tooltip; }
	};

	class draggable_region : public visible_region {
	private:
		ui::xy_pair base_position;
	public:
		draggable_region(draggable_region&&) = default;
		template<typename ...P>
		explicit draggable_region(P&& ... ) {}
		virtual bool on_get_focus(gui_object_tag, world_state&) override;
		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) override { return true; }
		virtual bool on_drag(gui_object_tag, world_state&, const mouse_drag&) final override;
		virtual bool on_text(gui_object_tag, world_state&, const text_event&) override { return true; }
	};

	class window_pane : public visible_region {
	public:
		window_pane(window_pane&&) = default;
		template<typename ...P>
		explicit window_pane(P&& ...) {}
		virtual bool on_get_focus(gui_object_tag, world_state&) override;
		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) override;
		virtual bool on_drag(gui_object_tag, world_state&, const mouse_drag&) override;
		virtual bool on_text(gui_object_tag, world_state&, const text_event&) override;
	};

	class fixed_region : public visible_region {
	public:
		fixed_region(fixed_region&&) = default;
		template<typename ...P>
		explicit fixed_region(P&& ... ) {}
		virtual bool on_get_focus(gui_object_tag, world_state&) override { return true; }
		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) override { return true; }
		virtual bool on_drag(gui_object_tag, world_state&, const mouse_drag&) override { return true; }
		virtual bool on_text(gui_object_tag, world_state&, const text_event&) override { return true; }
	};

	template<typename BASE>
	class simple_button : public gui_behavior, public BASE {
	public:
		virtual_key shortcut = virtual_key::NONE;

		simple_button(simple_button&&) = default;
		template<typename ...P>
		explicit simple_button(P&& ... params) : BASE(std::forward<P>(params)...) {}

		void set_frame(gui_manager&, uint32_t frame_num);
		void set_visibility(gui_manager&, bool visible);
		void set_enabled(bool enabled);

		virtual bool on_lclick(gui_object_tag o, world_state& m, const lbutton_down&) final override;
		virtual bool on_keydown(gui_object_tag o, world_state& m, const key_down& k) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type& w, world_state& s);
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
	};

	template<typename BASE>
	class progress_bar : public gui_behavior, public BASE {
	private:
		multi_texture_instance* underlying_obj = nullptr;
	public:
		progress_bar() : BASE() {}
		progress_bar(progress_bar&&) = default;
		template<typename ...P>
		explicit progress_bar(P&& ... params) : BASE(std::forward<P>(params)...) {}

		void set_visibility(gui_manager&, bool visible);
		void set_fraction(float fraction);
		void get_fraction();
		void set_underlying_obj(multi_texture_instance* o);

		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type& w, world_state& s);
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
	};

	template<typename BASE>
	class masked_flag : public gui_behavior, public BASE {
	private:
		cultures::national_tag displayed_flag;
		multi_texture_instance* underlying_obj = nullptr;
	public:
		virtual_key shortcut = virtual_key::NONE;
		
		masked_flag(masked_flag&&) = default;
		template<typename ...P>
		explicit masked_flag(P&& ... params) : BASE(std::forward<P>(params)...) {}

		void set_visibility(gui_manager&, bool visible);
		void set_enabled(bool enabled);
		void set_displayed_flag(world_state& ws, cultures::national_tag t);
		cultures::national_tag get_displayed_flag();
		void set_underlying_object(multi_texture_instance* o);

		virtual bool on_lclick(gui_object_tag o, world_state& m, const lbutton_down&) final override;
		virtual bool on_keydown(gui_object_tag o, world_state& m, const key_down& k) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type& w, world_state& s);
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
	};

	template<typename BASE>
	class dynamic_icon : public gui_behavior, public BASE {
	public:
		dynamic_icon(dynamic_icon&&) = default;
		template<typename ...P>
		explicit dynamic_icon(P&& ... params) : BASE(std::forward<P>(params)...) {}

		void set_frame(gui_manager&, uint32_t frame_num);
		void set_visibility(gui_manager&, bool visible);

		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type& w, world_state& s);
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
	};

	template<typename BASE>
	class tinted_icon : public gui_behavior, public BASE {
	public:
		tinted_icon(tinted_icon&&) = default;
		template<typename ...P>
		explicit tinted_icon(P&& ... params) : BASE(std::forward<P>(params)...) {}

		void set_color(gui_manager&, float r, float g, float b);
		void set_visibility(gui_manager&, bool visible);

		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type& w, world_state& s);
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
	};

	template<typename BASE>
	class dynamic_transparent_icon : public gui_behavior, public BASE {
	public:
		dynamic_transparent_icon(dynamic_transparent_icon&&) = default;
		template<typename ...P>
		explicit dynamic_transparent_icon(P&& ... params) : BASE(std::forward<P>(params)...) {}

		void set_frame(gui_manager&, uint32_t frame_num);
		void set_visibility(gui_manager&, bool visible);

		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type& w, world_state& s);
	};

	template<typename BASE, int32_t y_adjust = 0>
	class display_text : public gui_behavior, public BASE {
	private:
		text_format format;
		text_data::alignment align;
		gui_object_tag self;
	public:
		int32_t border_x = 0;
		int32_t border_y = 0;

		display_text(display_text&&) = default;
		template<typename ...P>
		display_text(P&& ... params) : BASE(std::forward<P>(params)...) {}

		template<typename window_type>
		void windowed_update(window_type&, world_state&);
		void set_visibility(gui_manager&, bool visible);
		void set_format(text_data::alignment a, const text_format& fmt) {
			align = a;
			format = fmt;
		}
		void set_self(gui_object_tag s) {
			self = s;
		}

		virtual void update_data(gui_object_tag, world_state&) final override;
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
	};

	template<typename BASE>
	class piechart : public visible_region, public BASE {
	private:
		vector_backed_string<char16_t> labels[piechart_resolution];
		float fractions[piechart_resolution];
		int32_t portion_used = 0;
		float remainder = 0.0f;
	public:
		piechart(piechart&&) = default;
		template<typename ...P>
		piechart(P&& ... params) : BASE(std::forward<P>(params)...) {}

		virtual bool on_lclick(gui_object_tag, world_state&, const lbutton_down&) final override;
		virtual bool on_rclick(gui_object_tag, world_state&, const rbutton_down&) final override;
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;

		template<typename window_type>
		void windowed_update(window_type&, world_state&);
		void clear_entries(gui_manager& manager);
		void add_entry(gui_manager& manager, vector_backed_string<char16_t> label, float fraction, graphics::color_rgb color);
		void fill_remainder(gui_manager& manager, vector_backed_string<char16_t> label, graphics::color_rgb color);
		void update_display(gui_manager& manager) const;
	};

	template<typename BASE, int32_t horizontal_resolution>
	class linechart : public gui_behavior, public BASE {
	public:
		linechart(linechart&&) = default;
		template<typename ...P>
		linechart(P&& ... params) : BASE(std::forward<P>(params)...) {}

		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;

		template<typename window_type>
		void windowed_update(window_type&, world_state&);

		void set_values(ui::gui_manager& manager, float* values);
	};

	template<typename BASE>
	class scrollbar : public visible_region, public BASE {
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

		scrollbar(scrollbar&&) = default;
		template<typename ... PARAMS>
		scrollbar(bool vert, int32_t mini, int32_t maxi, int32_t step, PARAMS&& ... params);
		template<typename ... PARAMS>
		scrollbar(PARAMS&& ... params) : BASE(std::forward<PARAMS>(params)...) {}

		int32_t position() const { return _position; }
		std::pair<int32_t, int32_t> range() const { return std::make_pair(minimum, maximum); }
		std::pair<int32_t, int32_t> track_range() const { return std::make_pair(valid_start, valid_end); }
		int32_t step_size() const { return _step_size; }

		void initialize(bool vert, int32_t mini, int32_t maxi, int32_t step);
		void update_limit_icons(gui_manager& m);
		void adjust_position(world_state& ws, int32_t position); // will call BASE::on_position
		void update_position(int32_t position); // will not call BASE::on_position
		void set_limits(gui_manager& m, int32_t lmin, int32_t lmax);
		void set_range(gui_manager& m, int32_t rmin, int32_t rmax);
		void set_step(int32_t s) { _step_size = s; }

		template<typename window_type>
		void windowed_update(window_type&, world_state&);

		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;
	};

	class unmanaged_region_scollbar {
	private:
		gui_object& contents_frame;
	public:
		unmanaged_region_scollbar(gui_object& g) : contents_frame(g) {}
		void on_position(int32_t pos);
	};

	class listbox_scrollbar {
	private:
		gui_object* _content_frame = nullptr;
	public:
		int32_t factor = 1;
		void on_position(int32_t pos) const;
		void associate(gui_object* g) { _content_frame = g; }
	};

	template<typename BASE, int32_t x_size_adjust = 0, int32_t y_size_adjust = 0>
	class multiline_text : public gui_behavior, public BASE {
	private:
		text_format format;
		text_data::alignment align;
		gui_object_tag scrollable_region;
		ui::scrollbar<listbox_scrollbar> sb;
		int32_t outer_height = 1;
	public:
		multiline_text(multiline_text&&) = default;
		template<typename ...P>
		multiline_text(P&& ... params) : BASE(std::forward<P>(params)...) {}

		template<typename window_type>
		void windowed_update(window_type&, world_state&);

		void set_format(text_data::alignment a, const text_format& fmt) {
			align = a;
			format = fmt;
		}
		void set_height(int32_t y) { outer_height = y; }
		void create_sub_elements(tagged_gui_object self, world_state& ws);
		virtual void update_data(gui_object_tag, world_state&) final override;
		virtual bool on_scroll(gui_object_tag t, world_state& ws, const scroll& s) final override;
	};


	template<typename BASE, typename ELEMENT, int32_t left_expand = 0>
	class display_listbox : public visible_region, public BASE {
	private:
		std::vector<ELEMENT, concurrent_allocator<ELEMENT>> contents;
		scrollbar<listbox_scrollbar> sb;

		window_def* element_def = nullptr;
		gui_object* _content_frame = nullptr;
		gui_object_tag _content_frame_tag;
		window_tag element_def_tag;

		void set_element_definition(gui_static& manager);
	public:
		display_listbox(display_listbox&&) = default;
		template<typename ... PARAMS>
		display_listbox(PARAMS&& ... params) : BASE(std::forward<PARAMS>(params)...) {}

		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;

		void create_sub_elements(tagged_gui_object self, world_state&);
		void clear_items(gui_manager&);
		void update_scroll_position(gui_manager&);
		template<typename ... PARAMS>
		void add_item(world_state&, PARAMS&& ...);
		template<typename window_type>
		void windowed_update(window_type&, world_state&);
	};

	class discrete_listbox_scrollbar {
	private:
		uint32_t* offset = nullptr;
	public:
		void on_position(world_state& ws, scrollbar<discrete_listbox_scrollbar>& obj, int32_t pos) const;
		void associate(uint32_t* g) { offset = g; }
	};

	template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand = 0>
	class discrete_listbox : public visible_region, public BASE {
	private:
		std::vector<ELEMENT, concurrent_allocator<ELEMENT>> display_list;
		std::vector<std::optional<value_type>, concurrent_allocator<std::optional<value_type>>> values_list;
		uint32_t offset = 0;

		scrollbar<discrete_listbox_scrollbar> sb;

		window_def* element_def = nullptr;
		window_tag element_def_tag;

		void set_element_definition(gui_static& manager);
	public:
		discrete_listbox(discrete_listbox&& o) noexcept : 
			visible_region(std::move(o)),
			display_list(std::move(o.display_list)), values_list(std::move(o.values_list)), sb(std::move(o.sb)),
			element_def(o.element_def), element_def_tag(o.element_def_tag) {
			sb.associate(&offset); o.sb.associate(nullptr);
		}
		template<typename ... PARAMS>
		discrete_listbox(PARAMS&& ... params) : BASE(std::forward<PARAMS>(params)...) {}

		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type&, world_state&);

		void create_sub_elements(tagged_gui_object self, world_state&);
		void update_scroll_position(gui_manager&);
		template<typename iterator>
		void new_list(iterator first, iterator last);
		template<typename iterator>
		void update_list(iterator first, iterator last);
		void goto_element(value_type const& v, ui::gui_manager& m);
	};

	template<typename BASE, typename data_type_list, typename gui_type_list>
	class tree_view : public visible_region, public BASE {
	private:
		std::vector<type_list_to_tuple<gui_type_list>, concurrent_allocator<type_list_to_tuple<gui_type_list>>> display_list;
		std::vector<std::pair<type_list_to_variant<data_type_list>, bool>, concurrent_allocator<std::pair<type_list_to_variant<data_type_list>, bool>>> values_list;
		std::vector<type_list_to_variant<data_type_list>, concurrent_allocator<type_list_to_variant<data_type_list>>> force_visible;

		uint32_t offset = 0;

		scrollbar<discrete_listbox_scrollbar> sb;

		window_def* element_def[type_list_size<gui_type_list>] = { nullptr };
		window_tag element_def_tag[type_list_size<gui_type_list>];

		void set_element_definition(gui_static& manager);
		int32_t get_max_element_height();
	public:
		virtual bool on_scroll(gui_object_tag, world_state&, const scroll&) final override;
		virtual void update_data(gui_object_tag, world_state&) final override;

		void create_sub_elements(tagged_gui_object self, world_state&);
		void update_scroll_position(gui_manager&);
		bool is_open(type_list_to_variant<data_type_list> const& v) const;
		void set_open(type_list_to_variant<data_type_list> const& v, bool o, ui::gui_manager& ws);
		void force_open(type_list_to_variant<data_type_list> const& v);
	};

	template<typename BASE, typename tag_type, typename ELEMENT, int32_t vertical_extension = 0>
	class overlap_box : public visible_region, public BASE {
	private:
		std::vector<ELEMENT, concurrent_allocator<ELEMENT>> contents;
		gui_object_tag self;

		int32_t spacing = 0;
		text_data::alignment subelement_alignment;
		tag_type element_def_tag;
	public:
		overlap_box(overlap_box&&) = default;
		template<typename ... PARAMS>
		overlap_box(PARAMS&& ... params) : BASE(std::forward<PARAMS>(params)...) {}

		virtual void update_data(gui_object_tag, world_state&) final override;
		template<typename window_type>
		void windowed_update(window_type& w, world_state& s);

		void set_self_information(world_state& m, gui_object_tag s, int32_t sp, text_data::alignment a);
		void clear_items(gui_manager&);
		void update_item_positions();
		template<typename ... PARAMS>
		void add_item(world_state&, PARAMS&& ...);
	};

	template<typename ... REST>
	class gui_window;

	class button_group_common_base {
	public:
		uint32_t current_index = 0;
		virtual void select(world_state&, uint32_t) = 0;
		virtual tooltip_behavior has_tooltip(uint32_t) = 0;
		virtual void create_tooltip(world_state&, tagged_gui_object /*tooltip_window*/, uint32_t) = 0;
		virtual ~button_group_common_base() {}
	};

	template<typename ... REST>
	class button_group;

	class button_group_member : public visible_region {
	private:
		uint32_t _index;
		button_group_common_base* group;
	public:
		virtual_key shortcut = virtual_key::NONE;

		button_group_member(button_group_member&&) = default;
		template<typename ...P>
		explicit button_group_member(P&& ... params) {}

		virtual bool on_lclick(gui_object_tag o, world_state& m, const lbutton_down&) final override;
		virtual bool on_keydown(gui_object_tag o, world_state& m, const key_down& k) final override;
		virtual tooltip_behavior has_tooltip(gui_object_tag, world_state&, const mouse_move&) final override;
		virtual void create_tooltip(gui_object_tag, world_state&, const mouse_move&, tagged_gui_object /*tooltip_window*/) final override;

		void set_group(button_group_common_base* g, uint32_t i);
		uint32_t index() const { return _index; }
	};

	enum class alignment : uint8_t {
		left, right, center, top_left, top_right, bottom_left, bottom_right, top_center, bottom_center
	};
	
	alignment alignment_from_definition(const button_def&);
	alignment alignment_from_definition(const icon_def&);
	alignment alignment_from_definition(const text_def&);
	alignment alignment_from_definition(const overlapping_region_def&);
	alignment alignment_from_definition(const listbox_def&);
	alignment alignment_from_definition(const scrollbar_def&);
	alignment alignment_from_definition(const window_def&);

	class gui_object {
	public:
		static constexpr uint16_t dynamic_behavior = 0x0100;
		static constexpr uint16_t visible = 0x0200;
		static constexpr uint16_t enabled = 0x0400;
		static constexpr uint16_t visible_after_update = 0x0800;
		static constexpr uint16_t dont_clip_children = 0x1000;
		static constexpr uint16_t force_transparency_check = 0x2000;
		static constexpr uint16_t display_as_disabled = 0x4000;

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
		static constexpr uint16_t type_tinted_icon = 0x0008; // type_dependant_handle = tinted_icon_instance

		gui_behavior* associated_behavior = nullptr; //8bytes

		xy_pair size; //12bytes
		xy_pair position; //16bytes

		std::atomic<uint16_t> type_dependant_handle; // 18bytes

		atomic_tag<gui_object_tag> parent; //20bytes
		atomic_tag<gui_object_tag> first_child; //22 bytes
		atomic_tag<gui_object_tag> left_sibling; //24 bytes
		atomic_tag<gui_object_tag> right_sibling; //26 bytes

		std::atomic<uint16_t> flags = visible_after_update | enabled; // 28 bytes

		ui::alignment align = ui::alignment::top_left; //32 bytes
		char padding[3];

		graphics::rotation get_rotation() const;

		gui_object() noexcept {}
	};

	static_assert(sizeof(gui_object) == 32);

	class gui_manager;
	class gui_static;

	namespace detail {
		struct screen_position {
			float effective_position_x;
			float effective_position_y;
			float effective_width;
			float effective_height;
		};

		float font_size_to_render_size(const graphics::font& f, int32_t sz);

		void render_object_type(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper&, const gui_object&, const screen_position& position, uint32_t type, bool currently_enabled);
		void render(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper&, const gui_object&, ui::xy_pair position, ui::xy_pair container_size, bool parent_enabled);

		void create_linear_text(gui_static& static_manager, gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format&, const text_data::replacement* candidates = nullptr, uint32_t count = 0);
		void create_multiline_text(gui_static& static_manager, gui_manager& manager, tagged_gui_object container, text_data::text_tag text_handle, text_data::alignment align, const text_format&, const text_data::replacement* candidates = nullptr, uint32_t count = 0);

		tagged_gui_object last_sibling_of(const gui_manager& manager, tagged_gui_object g);

		void instantiate_graphical_object(gui_static& static_manager, ui::gui_manager& manager, ui::tagged_gui_object container, graphics::obj_definition_tag gtag, int32_t frame = 0, bool force_tinted = false);
		tagged_gui_object create_element_instance(gui_static& static_manager, gui_manager& manager, button_tag handle);
		tagged_gui_object create_element_instance(gui_static& static_manager, gui_manager& manager, icon_tag handle);
		tagged_gui_object create_element_instance(gui_static& static_manager, gui_manager& manager, ui::text_tag handle, const text_data::replacement* candidates = nullptr, uint32_t count = 0);

		void update(tagged_gui_object obj, world_state&);
		void minimal_update(tagged_gui_object obj, world_state&);

		template<typename MESSAGE_FUNCTION, typename MESSAGE_TYPE>
		bool dispatch_message(const gui_manager& manager, const MESSAGE_FUNCTION &member_f, tagged_gui_object obj, ui::xy_pair container_size, const MESSAGE_TYPE& message);

		ui::xy_pair position_with_alignment(ui::xy_pair container_size, ui::xy_pair raw_position, ui::alignment align);
		std::pair<ui::xy_pair, ui::xy_pair> position_bounds_with_alignment(ui::xy_pair container_size, ui::xy_pair size, ui::alignment align);
	}

	tagged_object<ui::text_instance, ui::text_instance_tag> create_text_instance(ui::gui_manager &container, tagged_gui_object new_gobj, const text_format& fmt);

	template<typename BEHAVIOR = ui::gui_behavior, typename T, typename ... PARAMS>
	ui::tagged_gui_object create_dynamic_element(world_state& ws, T handle, tagged_gui_object parent, PARAMS&& ... params);

	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, progress_bar<B>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, button_tag handle, tagged_gui_object parent, simple_button<B>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, simple_button<B>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, masked_flag<B>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, button_tag handle, tagged_gui_object parent, masked_flag<B>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, scrollbar_tag handle, tagged_gui_object parent, scrollbar<B>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, piechart<B>& b);
	template<typename BASE, int32_t horizontal_resolution>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, linechart<BASE, horizontal_resolution>& b);
	template<typename B, int32_t y_adjust>
	ui::tagged_gui_object create_static_element(world_state& ws, text_tag handle, tagged_gui_object parent, display_text<B, y_adjust>& b);
	template<typename B, int32_t x_size_adjust, int32_t y_size_adjust>
	ui::tagged_gui_object create_static_element(world_state& ws, text_tag handle, tagged_gui_object parent, multiline_text<B, x_size_adjust, y_size_adjust>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, dynamic_icon<B>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, tinted_icon<B>& b);
	template<typename ... REST>
	ui::tagged_gui_object create_static_element(world_state& ws, window_tag handle, tagged_gui_object parent, gui_window<REST...>& b);
	template<typename B, typename ELEMENT, int32_t left_expand>
	ui::tagged_gui_object create_static_element(world_state& ws, listbox_tag handle, tagged_gui_object parent, ui::display_listbox<B, ELEMENT, left_expand>& b);
	template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
	ui::tagged_gui_object create_static_element(world_state& ws, listbox_tag handle, tagged_gui_object parent, discrete_listbox<BASE, ELEMENT, value_type, left_expand>& b);
	template<typename BASE, typename ELEMENT, typename value_type, int32_t left_expand>
	ui::tagged_gui_object create_static_element(world_state& ws, window_tag handle, tagged_gui_object parent, discrete_listbox<BASE, ELEMENT, value_type, left_expand>& b);
	template<typename BASE, typename data_type_list, typename gui_type_list>
	ui::tagged_gui_object create_static_element(world_state& ws, listbox_tag handle, tagged_gui_object parent, tree_view<BASE, data_type_list, gui_type_list>& b);
	template<typename B, typename tag_type, typename ELEMENT, int32_t vertical_extension>
	ui::tagged_gui_object create_static_element(world_state& ws, overlapping_region_tag handle, tagged_gui_object parent, ui::overlap_box<B, tag_type, ELEMENT, vertical_extension>& b);
	template<typename B>
	ui::tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, dynamic_transparent_icon<B>& b);
	ui::tagged_gui_object create_static_element(world_state& ws, button_tag handle, tagged_gui_object parent, button_group_member& b);

	template<typename BEHAVIOR, typename ... PARAMS>
	ui::tagged_gui_object create_scrollbar (world_state& ws, scrollbar_tag handle, tagged_gui_object parent, PARAMS&& ... params);
	template<typename BEHAVIOR, typename ... PARAMS>
	ui::tagged_gui_object create_fixed_sz_scrollbar(world_state& ws, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, PARAMS&& ... params);
	template<typename BEHAVIOR>
	ui::tagged_gui_object create_static_fixed_sz_scrollbar(world_state& ws, scrollbar_tag handle, tagged_gui_object parent, ui::xy_pair position, int32_t extent, scrollbar<BEHAVIOR>& b);
	template<typename FILL_FUNCTION>
	ui::tagged_gui_object create_scrollable_region(world_state& ws, tagged_gui_object parent, ui::xy_pair position, int32_t height, int32_t step_size, graphics::obj_definition_tag bg, const FILL_FUNCTION& f);
	ui::tagged_gui_object create_scrollable_text_block(world_state& ws, ui::text_tag handle, tagged_gui_object parent, const text_data::replacement* candidates = nullptr, uint32_t count = 0);
	ui::tagged_gui_object create_scrollable_text_block(world_state& ws, ui::text_tag handle, text_data::text_tag contents, tagged_gui_object parent, const text_data::replacement* candidates = nullptr, uint32_t count = 0);
	ui::tagged_gui_object create_dynamic_window(world_state& ws, window_tag t, tagged_gui_object parent);

	text_data::alignment text_aligment_from_button_definition(const button_def& def);
	text_data::alignment text_aligment_from_text_definition(const text_def& def);
	text_data::alignment text_aligment_from_overlapping_definition(const overlapping_region_def& def);
	ui::text_color text_color_to_ui_text_color(text_data::text_color c);

	class line_manager {
	private:
		constexpr static int32_t indent_size = 15;

		boost::container::small_vector<gui_object*, 16, concurrent_allocator<gui_object*>> current_line;
		const text_data::alignment align;
		const int32_t max_line_extent;
		int32_t indent = 0;
	public:
		line_manager(text_data::alignment a, int32_t m);
		line_manager();
		bool exceeds_extent(int32_t w) const;
		void add_object(gui_object* o);
		void finish_current_line();
		void increase_indent(int32_t n);
		void decrease_indent(int32_t n);
	};

	using unlimited_line_manager = line_manager;

	class text_box_line_manager {
	private:
		boost::container::small_vector<gui_object*, 16, concurrent_allocator<gui_object*>> current_line;
		const text_data::alignment align;
		const int32_t max_line_extent;
		const int32_t border_x = 0;
		int32_t border_y = 0;
	public:
		text_box_line_manager(text_data::alignment a, int32_t m, int32_t bx, int32_t by);
		void set_border_y(int32_t v) { border_y = v; }
		bool exceeds_extent(int32_t) const { return false; }
		void add_object(gui_object* o);
		void finish_current_line();
	};

	class single_line_manager {
	public:
		bool exceeds_extent(int32_t) const { return false; }
		void add_object(gui_object*) const {}
		void finish_current_line() const {}
	};

	class null_behavior_creation {
	public:
		void operator()(tagged_gui_object) const {}
	};
	
	xy_pair advance_cursor_to_newline(ui::xy_pair cursor, gui_static& manager, text_format const& fmt);
	xy_pair advance_cursor_by_space(ui::xy_pair cursor, gui_static& manager, text_format const& fmt, int32_t count = 1);

	template<typename LM = single_line_manager>
	xy_pair add_linear_text(ui::xy_pair cursor, text_data::text_tag, text_format const& fmt, gui_static& static_manager, gui_manager& manager, tagged_gui_object container, LM&& lm = single_line_manager(), const text_data::replacement* candidates = nullptr, uint32_t count = 0);


	void shorten_text_instance_to_space(ui::text_instance& txt);
	float text_component_width(const text_data::text_component& c, const std::vector<char16_t>& text_data, graphics::font& this_font, uint32_t font_size);

	template<typename LM = single_line_manager, typename BH = null_behavior_creation>
	xy_pair text_chunk_to_instances(gui_static& static_manager, gui_manager& container, vector_backed_string<char16_t> text_source, tagged_gui_object parent_object, ui::xy_pair position, const text_format& fmt, LM&& lm = single_line_manager(), const BH& behavior_creator = null_behavior_creation());

	void clear_children(gui_manager& manager, tagged_gui_object g);
	void remove_object(gui_manager& manager, tagged_gui_object g);
	void move_to_front(const gui_manager& manager, tagged_gui_object g);
	void move_to_back(const gui_manager& manager, tagged_gui_object g);
	void add_to_front(const gui_manager& manager, tagged_gui_object parent, tagged_gui_object child);
	void add_to_back(const gui_manager& manager, tagged_gui_object parent, tagged_gui_object child);

	void make_visible_and_update(gui_manager& manager, gui_object& g);
	void make_visible_immediate(gui_object& g);
	void hide(gui_object& g);
	void set_enabled(gui_object& g, bool enabled);
	void shrink_to_children(gui_manager& manager, tagged_gui_object g);
	void shrink_to_children(gui_manager& manager, tagged_gui_object g, int32_t border);

	bool set_focus(world_state& ws, tagged_gui_object g);
	void clear_focus(world_state& ws);

	ui::xy_pair absolute_position(gui_manager& manager, tagged_gui_object g);

	void render(gui_static& static_manager, const gui_manager& manager, graphics::open_gl_wrapper&);
	void update(world_state&);
	void minimal_update(world_state&);

	template<typename T>
	void for_each_child(gui_manager& manager, tagged_gui_object parent, const T& f);

	class gui_static {
	public:
		graphics::texture_manager textures;
		graphics::font_manager fonts;

		ui::definitions ui_definitions;
		ui::name_maps nmaps;
		graphics::object_definitions graphics_object_definitions;
		text_data::text_sequences text_data_sequences;
	};

	class gui_manager {
	private:
		float _scale = 1.0f;
		int32_t _width;
		int32_t _height;
		std::atomic<bool> pending_update = false;
		std::atomic<bool> pending_minimal_update = false;
	public:
		fixed_sz_deque<gui_object, 128, 64, gui_object_tag> gui_objects;
		fixed_sz_deque<text_instance, 128, 64, text_instance_tag> text_instances;
		fixed_sz_deque<graphics_instance, 128, 64, graphics_instance_tag> graphics_instances;
		fixed_sz_deque<tinted_icon_instance, 128, 64, tinted_icon_instance_tag> tinted_icon_instances;
		fixed_sz_deque<multi_texture_instance, 64, 64, multi_texture_instance_tag> multi_texture_instances;
		fixed_sz_deque<graphics::data_texture, 64, 16, data_texture_tag> data_textures;
		fixed_sz_deque<graphics::lines, 32, 8, lines_tag> lines_set;

		gui_object& root;
		gui_object& background;
		gui_object& foreground;
		gui_object& tooltip_window;

		gui_object_tag focus;
		gui_object_tag tooltip;

		gui_manager();
		gui_manager(int32_t width, int32_t height);
		
		void on_resize(const resize&);
		bool on_lbutton_down(world_state& static_manager, const lbutton_down&);
		bool on_rbutton_down(world_state& static_manager, const rbutton_down&);
		bool on_mouse_move(world_state& static_manager, const mouse_move&);
		bool on_mouse_drag(world_state& static_manager, const mouse_drag&);
		bool on_keydown(world_state& static_manager, const key_down&);
		bool on_text(world_state& static_manager, const text_event&);
		bool on_scroll(world_state& static_manager, const scroll&);

		void rescale(float new_scale);
		float scale() const { return _scale; }
		int32_t width() const { return _width; }
		int32_t height() const { return _height; }
		void hide_tooltip();
		void flag_update() { pending_update.store(true, std::memory_order_release); }
		bool check_and_clear_update() { bool expected = true; return pending_update.compare_exchange_strong(expected, false, std::memory_order_release, std::memory_order_acquire); }
		void flag_minimal_update() { pending_minimal_update.store(true, std::memory_order_release); }
		bool check_and_clear_minimal_update() { bool expected = true; return pending_minimal_update.compare_exchange_strong(expected, false, std::memory_order_release, std::memory_order_acquire); }

		void destroy(gui_object& g);
		~gui_manager();
	};

	void init_tooltip_window(gui_static& static_manager, gui_manager& manager);
}
