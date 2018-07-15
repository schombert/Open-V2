#include "common\\common.h"
#include "province_gui.hpp"
#include "world_state\\world_state.h"
#include "modifiers\\modifiers_gui.h"
#include "nations\\nations_functions.h"

namespace provinces {
	void close_province_window::button_function(ui::gui_object_tag, world_state& ws) {
		auto gobj = ws.w.province_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}

	void modifier_button::update(ui::simple_button<modifier_button>& ico, world_state & ws) {
		if(icon != 0ui32)
			ico.set_frame(ws.w.gui_m, icon - 1ui32);
		else
			ico.set_frame(ws.w.gui_m, 0ui32);
	}

	void modifier_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		if(is_valid_index(mod)) {
			ui::xy_pair cursor{ 0,0 };
			ui::unlimited_line_manager lm;
			cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 }, mod);
			if(is_valid_index(expiration)) {
				char16_t formatted_date[64];
				u16_format_date(formatted_date, expiration);
				text_data::replacement date_rep(text_data::value_type::date, vector_backed_string<char16_t>(formatted_date), [](tagged_object<ui::gui_object, ui::gui_object_tag>){});
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::expires_on], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &date_rep, 1ui32);
			}
		}
	}

	void slave_state_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::slave_state], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void colony_button::button_function(ui::gui_object_tag, world_state&) {

	}

	void colony_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;
			if(state) {
				if(ws.w.province_s.province_state_container[selected_prov].owner != ws.w.local_player_nation || ws.w.local_player_nation == nullptr) {
					if(state->flags & nations::state_instance::is_protectorate)
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::protectorate_province], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
					else if(state->flags & nations::state_instance::is_colonial)
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::colonial_province], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
				} else {
					ui::unlimited_line_manager lm;
					char16_t formatted_value[64];
					if(state->flags & nations::state_instance::is_protectorate) {
						u16itoa(nations::colonial_points_to_make_colony(ws, *state), formatted_value);
						text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::protectorate_province_upgrade], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
					} else if(state->flags & nations::state_instance::is_colonial) {
						u16itoa(nations::colonial_points_to_make_state(ws, *state), formatted_value);
						text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::protectorate_province_upgrade], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
					}
				}
			}
		}
	}

	void admin_icon::update(ui::dynamic_icon<admin_icon>& ico, world_state& ws) {
		ico.set_frame(ws.w.gui_m, uint32_t(ws.s.population_m.pop_types[ws.s.population_m.bureaucrat].sprite) - 1ui32);
	}

	bool admin_icon::has_tooltip(world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;
			if(state)
				return true;
		}
		return false;
	}

	void admin_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;
			if(state) {
				ui::unlimited_line_manager lm;
				char16_t formatted_value[64];
				ui::xy_pair cursor{ 0,0 };

				u16itoa(int32_t(state->administrative_efficiency * 100.0f), formatted_value);
				text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();

				lm.increase_indent(1);

				u16itoa(0, formatted_value);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin_base], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();

				u16itoa(0, formatted_value);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin_pops], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();

				u16itoa(0, formatted_value);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin_tech], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();
			}
		}
	}

	void admin_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		int32_t value = 0;

		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;
			if(state) {
				value = int32_t(state->administrative_efficiency * 100.0f);
			}
		}

		char16_t local_buf[64];

		auto end = u16itoa(value, local_buf);
		*end = u'%';
		*(end + 1) = char16_t(0);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{0,0},
			fmt,
			lm);

		lm.finish_current_line();
	}

	void terrain_icon::update(ui::dynamic_icon<terrain_icon>& ico, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto terrain_id = ws.w.province_s.province_state_container[selected_prov].terrain;
			if(auto f = ws.s.province_m.terrain_graphics.find(terrain_id); f != ws.s.province_m.terrain_graphics.end()) {

				auto associated_obj_tag = ui::graphics_instance_tag(ico.associated_object->type_dependant_handle.load(std::memory_order_acquire));
				auto associated_gobj = ws.w.gui_m.graphics_instances.safe_at(associated_obj_tag);
				if(associated_gobj) {
					associated_gobj->t = &(ws.s.gui_m.textures.retrieve_by_key(ws.s.gui_m.graphics_object_definitions.definitions[f->second].primary_texture_handle));
				}

				ico.set_visibility(ws.w.gui_m, true);
				return;
			}
		}
		ico.set_visibility(ws.w.gui_m, false);
	}

	void terrain_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			ui::xy_pair cursor{ 0,0 };
			ui::unlimited_line_manager lm;

			auto terrain_mod = ws.w.province_s.province_state_container[selected_prov].terrain;
			if(is_valid_index(terrain_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::tooltip_text_format, terrain_mod);
			
			auto climate_mod = ws.s.province_m.province_container[selected_prov].climate;
			if(is_valid_index(climate_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::tooltip_text_format, climate_mod);

			auto continent_mod = ws.s.province_m.province_container[selected_prov].continent;
			if(is_valid_index(continent_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::tooltip_text_format, continent_mod);
			
		}
	}

	void province_window_base::on_create(world_state& ws) {
		associated_object->position += ui::xy_pair{ 0i16, -19i16 };
		associated_object->size = ui::xy_pair{ 406i16, 548i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, 19i16 };
		});
	}

	void province_window_header_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 406i16, 548i16 };
		associated_object->position += ui::xy_pair{ 0i16, 1i16 };
	}

	ui::window_tag modifier_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["prov_state_modifier"]);
	}

	void province_controller_flag::button_function(ui::masked_flag<province_controller_flag>&, world_state&) {}
	void province_controller_flag::create_tooltip(ui::masked_flag<province_controller_flag>&, world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto controller = ws.w.province_s.province_state_container[selected_prov].controller;
			if(controller) {
				ui::unlimited_line_manager lm;
				ui::xy_pair cursor{ 0,0 };

				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::controller], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::add_linear_text(cursor, controller->name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			}
		}
	}

	void liferating_bar::update(ui::progress_bar<liferating_bar>& self, world_state& ws) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			self.set_fraction(float(ws.w.province_s.province_state_container[selected_prov].life_rating) / 50.0f);
		}
	}

	void flashpoint_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;
			if(state && is_valid_index(state->crisis_tag)) {
				ui::unlimited_line_manager lm;
				char16_t formatted_value[64];
				ui::xy_pair cursor{ 0,0 };

				put_value_in_buffer(formatted_value, display_type::fp_two_places, state->current_tension);

				text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
				ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::flashpoint_tension], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				return;
			}
		}

		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::has_no_flashpoint], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void liferating_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_window.selected_province;
		if(is_valid_index(selected_prov)) {
			ui::unlimited_line_manager lm;
			char16_t formatted_value[64];
			ui::xy_pair cursor{ 0,0 };

			put_value_in_buffer(formatted_value, display_type::integer, int32_t(ws.w.province_s.province_state_container[selected_prov].life_rating));

			text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
			ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_liferating], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
		}
	}
}
