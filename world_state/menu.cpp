#include "menu.hpp"
#include "scenario\\settings.h"

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

void menu::sound_button::button_function(ui::simple_button<sound_button>&, world_state & ws) {
	ui::make_visible_and_update(ws.w.gui_m, *(ws.w.menu_w.sound->associated_object));
	ui::hide(*(ws.w.menu_w.win->associated_object));
}

void menu::controls_button::button_function(ui::simple_button<controls_button>&, world_state & ws) {
	ui::make_visible_and_update(ws.w.gui_m, *(ws.w.menu_w.controls->associated_object));
	ui::hide(*(ws.w.menu_w.win->associated_object));
}

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
	ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_controls_menu"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *(this->controls));
	ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_audio_menu"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *(this->sound));
}

void menu::graphics_menu_close_button::button_function(ui::simple_button<graphics_menu_close_button>&, world_state & ws) {
	if(ws.w.menu_w.graphics->graphics_setting_changed) {
		settings::save_settings(ws.s);
		ws.w.menu_w.graphics->graphics_setting_changed = false;
	}
	ui::make_visible_immediate( *(ws.w.menu_w.win->associated_object));
	ui::hide(*(ws.w.menu_w.graphics->associated_object));
}

void menu::window_mode_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	ui::add_text(
		ui::xy_pair{0, 0},
		ws.s.fixed_ui_text[scenario::window_mode_labels[ws.s.settings.window_mode]],
		fmt,
		ws,
		box,
		lm);
}

void menu::projection_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	ui::add_text(
		ui::xy_pair{ 0, 0 },
		ws.s.fixed_ui_text[scenario::projection_labels[ws.s.settings.projection]],
		fmt,
		ws,
		box,
		lm);
}

void menu::ui_scale_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	ui::add_text(ui::xy_pair{ 0,0 }, text_data::fp_two_places{ scenario::ui_scales[ws.s.settings.ui_scale] }, fmt, ws, box, lm);
}

void menu::controls_menu_close_button::button_function(ui::simple_button<controls_menu_close_button>&, world_state & ws) {
	if(ws.w.menu_w.controls->control_setting_changed) {
		settings::save_settings(ws.s);
		ws.w.menu_w.controls->control_setting_changed = false;
	}
	ui::make_visible_immediate(*(ws.w.menu_w.win->associated_object));
	ui::hide(*(ws.w.menu_w.controls->associated_object));
}

void menu::zoom_mode_text::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
	ui::add_text(
		ui::xy_pair{ 0, 0 },
		ws.s.fixed_ui_text[scenario::zoom_labels[int32_t(ws.s.settings.zoom_setting)]],
		fmt,
		ws,
		box,
		lm);
}

void menu::sound_menu_close_button::button_function(ui::simple_button<sound_menu_close_button>&, world_state & ws) {
	if(ws.w.menu_w.sound->sound_setting_changed) {
		settings::save_settings(ws.s);
		ws.w.menu_w.sound->sound_setting_changed = false;
	}
	ui::make_visible_immediate(*(ws.w.menu_w.win->associated_object));
	ui::hide(*(ws.w.menu_w.sound->associated_object));
}

void menu::master_volume_scroll_bar::on_position(world_state & ws, ui::scrollbar<master_volume_scroll_bar>& sb, int32_t pos) {
	auto const old_volume = ws.s.settings.master_volume;
	ws.s.settings.master_volume = float(pos) / 128.0f;
	if(old_volume == 0.0f && ws.s.settings.master_volume != 0.0f) {
		sound::play_new_track(ws);
	} else {
		ws.s.sound_m.change_music_volume(ws.s.settings.master_volume * ws.s.settings.music_volume);
	}
	ws.w.menu_w.sound->sound_setting_changed = false;
}

void menu::master_volume_scroll_bar::update(ui::scrollbar<master_volume_scroll_bar>& sb, world_state & ws) {
	sb.update_position(int32_t(ws.s.settings.master_volume * 128.0f));
}

void menu::music_volume_scroll_bar::on_position(world_state & ws, ui::scrollbar<music_volume_scroll_bar>& sb, int32_t pos) {
	auto const old_volume = ws.s.settings.music_volume;
	ws.s.settings.music_volume = float(pos) / 128.0f;
	if(old_volume == 0.0f && ws.s.settings.music_volume != 0.0f) {
		sound::play_new_track(ws);
	} else {
		ws.s.sound_m.change_music_volume(ws.s.settings.master_volume * ws.s.settings.music_volume);
	}
	ws.w.menu_w.sound->sound_setting_changed = true;
}

void menu::music_volume_scroll_bar::update(ui::scrollbar<music_volume_scroll_bar>& sb, world_state & ws) {
	sb.update_position(int32_t(ws.s.settings.music_volume * 128.0f));
}

void menu::interface_volume_scroll_bar::on_position(world_state & ws, ui::scrollbar<interface_volume_scroll_bar>& sb, int32_t pos) {
	ws.s.settings.interface_volume = float(pos) / 128.0f;
	ws.w.menu_w.sound->sound_setting_changed = true;
}

void menu::interface_volume_scroll_bar::update(ui::scrollbar<interface_volume_scroll_bar>& sb, world_state & ws) {
	sb.update_position(int32_t(ws.s.settings.interface_volume * 128.0f));
}

void menu::effect_volume_scroll_bar::on_position(world_state & ws, ui::scrollbar<effect_volume_scroll_bar>& sb, int32_t pos) {
	ws.s.settings.effects_volume = float(pos) / 128.0f;
	ws.w.menu_w.sound->sound_setting_changed = true;
}

void menu::effect_volume_scroll_bar::update(ui::scrollbar<effect_volume_scroll_bar>& sb, world_state & ws) {
	sb.update_position(int32_t(ws.s.settings.effects_volume * 128.0f));
}
