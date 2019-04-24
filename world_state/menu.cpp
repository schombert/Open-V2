#include "menu.hpp"

void menu::menu_close_button::button_function(ui::simple_button<menu_close_button>&, world_state & ws) {
	ws.w.menu_w.hide_menu_window(ws);
}

void menu::save_and_exit_button::button_function(ui::simple_button<save_and_exit_button>&, world_state & ws) {
	
}

void menu::save_button::button_function(ui::simple_button<save_button>&, world_state & ws) {}

void menu::exit_button::button_function(ui::simple_button<exit_button>&, world_state & ws) {
	ws.w.end_game.store(true, std::memory_order_release);
	ws.w.menu_w.hide_menu_window(ws);
}

void menu::graphics_button::button_function(ui::simple_button<graphics_button>&, world_state & ws) {
	ui::make_visible_and_update(ws.w.gui_m, *(ws.w.menu_w.graphics->associated_object));
	ui::hide(*(ws.w.menu_w.win->associated_object));
}

void menu::sound_button::button_function(ui::simple_button<sound_button>&, world_state & ws) {}

void menu::controls_button::button_function(ui::simple_button<controls_button>&, world_state & ws) {}

menu::menu_window::menu_window() : win(std::make_unique<menu_window_t>()), graphics(std::make_unique<graphics_panel_t>()),
	sound(std::make_unique<sound_panel_t>()), controls(std::make_unique<controls_panel_t>()) {}

menu::menu_window::~menu_window() {}

void menu::menu_window::hide_menu_window(world_state & ws) {
	ws.w.force_paused.store(false, std::memory_order_release);
	auto gobj = win->associated_object;
	if(gobj)
		ui::hide(*gobj);
}

void menu::menu_window::update_menu_window(ui::gui_manager & gui_m) {
	ui::make_visible_and_update(gui_m, *(win->associated_object));
}

void menu::menu_window::show_menu_window(world_state & ws) {
	ws.w.force_paused.store(true, std::memory_order_release);
	ui::move_to_front(ws.w.gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
	ui::make_visible_and_update(ws.w.gui_m, *(win->associated_object));
}

void menu::menu_window::init_menu_window(world_state & ws) {
	ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_main_menu"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_graphics_menu"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *(this->graphics));
}

void menu::graphics_menu_close_button::button_function(ui::simple_button<graphics_menu_close_button>&, world_state & ws) {
	// todo: save settings if changed
	ui::make_visible_immediate( *(ws.w.menu_w.win->associated_object));
	ui::hide(*(ws.w.menu_w.graphics->associated_object));
}

void menu::window_mode_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	ui::add_linear_text(
		ui::xy_pair{0, 0},
		ws.s.fixed_ui_text[scenario::window_mode_labels[ws.s.settings.window_mode]],
		fmt,
		ws.s.gui_m,
		ws.w.gui_m,
		box,
		lm);
	lm.finish_current_line();
}

void menu::projection_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	ui::add_linear_text(
		ui::xy_pair{ 0, 0 },
		ws.s.fixed_ui_text[scenario::projection_labels[ws.s.settings.projection]],
		fmt,
		ws.s.gui_m,
		ws.w.gui_m,
		box,
		lm);
	lm.finish_current_line();
}

void menu::ui_scale_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	char16_t local_buffer[16];
	put_value_in_buffer(local_buffer, display_type::fp_two_places, scenario::ui_scales[ws.s.settings.ui_scale]);
	ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
	lm.finish_current_line();
}
