#include "common\\common.h"
#include "province_gui.h"
#include "world_state\\world_state.h"
#include "modifiers\\modifiers_gui.h"

namespace provinces {
	void close_province_window::button_function(ui::gui_object_tag, world_state& ws) {
		auto gobj = ws.w.province_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
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
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 }, terrain_mod);
			
			auto climate_mod = ws.s.province_m.province_container[selected_prov].climate;
			if(is_valid_index(climate_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 }, climate_mod);

			auto continent_mod = ws.s.province_m.province_container[selected_prov].continent;
			if(is_valid_index(continent_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 }, continent_mod);
			
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
		associated_object->size = ui::xy_pair{ 380i16, 548i16 };
		associated_object->position += ui::xy_pair{ 0i16, 1i16 };
	}
}
