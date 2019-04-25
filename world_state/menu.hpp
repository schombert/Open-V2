#pragma once
#include "common\\common.h"
#include "menu.h"
#include "world_state.h"
#include "gui\\gui.hpp"

namespace menu {
	class menu_window_base : public ui::fixed_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class menu_close_button {
	public:
		void button_function(ui::simple_button<menu_close_button>&, world_state& ws);
	};

	class save_and_exit_button {
	public:
		void button_function(ui::simple_button<save_and_exit_button>&, world_state& ws);
	};

	class save_button {
	public:
		void button_function(ui::simple_button<save_button>&, world_state& ws);
	};

	class exit_button {
	public:
		void button_function(ui::simple_button<exit_button>&, world_state& ws);
	};

	class graphics_button {
	public:
		void button_function(ui::simple_button<graphics_button>&, world_state& ws);
	};

	class sound_button {
	public:
		void button_function(ui::simple_button<sound_button>&, world_state& ws);
	};

	class controls_button {
	public:
		void button_function(ui::simple_button<controls_button>&, world_state& ws);
	};

	
	class graphics_menu_close_button {
	public:
		void button_function(ui::simple_button<graphics_menu_close_button>&, world_state& ws);
	};

	class window_mode_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<bool is_left>
	class window_mode_button {
	public:
		void button_function(ui::simple_button<window_mode_button<is_left>>&, world_state& ws);
		void update(ui::simple_button<window_mode_button<is_left>>& self, world_state& ws);
	};

	class projection_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<bool is_left>
	class projection_button {
	public:
		void button_function(ui::simple_button<projection_button<is_left>>&, world_state& ws);
		void update(ui::simple_button<projection_button<is_left>>& self, world_state& ws);
	};

	class ui_scale_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<bool is_left>
	class ui_scale_button {
	public:
		void button_function(ui::simple_button<ui_scale_button<is_left>>&, world_state& ws);
		void update(ui::simple_button<ui_scale_button<is_left>>& self, world_state& ws);
	};


	class controls_menu_close_button {
	public:
		void button_function(ui::simple_button<controls_menu_close_button>&, world_state& ws);
	};


	class zoom_mode_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<bool is_left>
	class zoom_mode_button {
	public:
		void button_function(ui::simple_button<zoom_mode_button<is_left>>&, world_state& ws);
		void update(ui::simple_button<zoom_mode_button<is_left>>& self, world_state& ws);
	};

	class sound_panel_t {};
	class controls_panel_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<controls_menu_close_button>,
		CT_STRING("zoom_mode_value"), ui::display_text<zoom_mode_text>,
		CT_STRING("zoom_mode_left"), ui::simple_button<zoom_mode_button<true>>,
		CT_STRING("zoom_mode_right"), ui::simple_button<zoom_mode_button<false>>,
		menu_window_base
	> {};

	class graphics_panel_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<graphics_menu_close_button>,
		CT_STRING("window_mode_value"), ui::display_text<window_mode_text>,
		CT_STRING("window_mode_left"), ui::simple_button<window_mode_button<true>>,
		CT_STRING("window_mode_right"), ui::simple_button<window_mode_button<false>>,
		CT_STRING("projection_value"), ui::display_text<projection_text>,
		CT_STRING("projection_left"), ui::simple_button<projection_button<true>>,
		CT_STRING("projection_right"), ui::simple_button<projection_button<false>>,
		CT_STRING("ui_scale_value"), ui::display_text<ui_scale_text>,
		CT_STRING("ui_scale_left"), ui::simple_button<ui_scale_button<true>>,
		CT_STRING("ui_scale_right"), ui::simple_button<ui_scale_button<false>>,
		menu_window_base
	> {};

	class menu_window_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<menu_close_button>,
		CT_STRING("save_and_exit"), ui::simple_button<save_and_exit_button>,
		CT_STRING("save"), ui::simple_button<save_button>,
		CT_STRING("exit"), ui::simple_button<exit_button>,
		CT_STRING("graphics"), ui::simple_button<graphics_button>,
		CT_STRING("sound"), ui::simple_button<sound_button>,
		CT_STRING("controls"), ui::simple_button<controls_button>,
		menu_window_base
	> {};

	template<typename W>
	void menu_window_base::on_create(W & w, world_state &) {
		ui::hide(*associated_object);
	}
	template<bool is_left>
	void window_mode_button<is_left>::button_function(ui::simple_button<window_mode_button<is_left>>&, world_state & ws) {
		ws.s.settings.window_mode = ws.s.settings.window_mode + (is_left ? -1 : 1);

		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.menu_w.graphics->associated_object));
	}
	template<bool is_left>
	void window_mode_button<is_left>::update(ui::simple_button<window_mode_button<is_left>>& self, world_state& ws) {
		ui::set_enabled(*self.associated_object, is_left ? (ws.s.settings.window_mode != 0) : (ws.s.settings.window_mode != scenario::window_mode_count - 1));
	}
	template<bool is_left>
	void projection_button<is_left>::button_function(ui::simple_button<projection_button<is_left>>&, world_state & ws) {
		ws.s.settings.projection = ws.s.settings.projection + (is_left ? -1 : 1);

		if(ws.s.settings.projection == 0)
			ws.w.map.state.set_projection(graphics::projection_type::standard_map);
		else if(ws.s.settings.projection == 1)
			ws.w.map.state.set_projection(graphics::projection_type::spherical);

		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.menu_w.graphics->associated_object));
	}
	template<bool is_left>
	void projection_button<is_left>::update(ui::simple_button<projection_button<is_left>>& self, world_state & ws) {
		ui::set_enabled(*self.associated_object, is_left ? (ws.s.settings.projection != 0) : (ws.s.settings.projection != scenario::projection_type_count - 1));
	}
	template<bool is_left>
	void ui_scale_button<is_left>::button_function(ui::simple_button<ui_scale_button<is_left>>&, world_state & ws) {
		ws.s.settings.ui_scale = ws.s.settings.ui_scale + (is_left ? -1 : 1);

		ws.w.gui_m.rescale(scenario::ui_scales[ws.s.settings.ui_scale]);
		ws.w.topbar_w.resize_topbar(ws.w.gui_m);

		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.menu_w.graphics->associated_object));
	}
	template<bool is_left>
	void ui_scale_button<is_left>::update(ui::simple_button<ui_scale_button<is_left>>& self, world_state & ws) {
		int max_ui_scale = 0;
		const float real_base_size = static_cast<float>(ws.w.gui_m.height());
		const float menu_height = static_cast<float>(ws.w.menu_w.win->associated_object->size.y);
		for(int i = 0; i < int32_t(scenario::ui_scale_count); ++i) {
			if(menu_height * scenario::ui_scales[i] > real_base_size)
				break;
			max_ui_scale = i;
		}
		ui::set_enabled(*self.associated_object, is_left ? (ws.s.settings.ui_scale != 0) : (ws.s.settings.ui_scale != max_ui_scale));
	}
	template<bool is_left>
	void zoom_mode_button<is_left>::button_function(ui::simple_button<zoom_mode_button<is_left>>&, world_state & ws) {
		ws.s.settings.zoom_setting = scenario::zoom_type(int32_t(ws.s.settings.zoom_setting) + (is_left ? -1 : 1));

		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.menu_w.controls->associated_object));
	}
	template<bool is_left>
	void zoom_mode_button<is_left>::update(ui::simple_button<zoom_mode_button<is_left>>& self, world_state & ws) {
		ui::set_enabled(*self.associated_object, is_left ? (int32_t(ws.s.settings.zoom_setting) != 0) : (int32_t(ws.s.settings.zoom_setting) != scenario::zoom_type_count - 1));
	}
}
