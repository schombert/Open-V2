#include "common\\common.h"
#include "province_gui.h"
#include "world_state\\world_state.h"

namespace provinces {
	void close_province_window::button_function(ui::gui_object_tag, world_state& ws) {
		auto gobj = ws.w.province_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}

	void terrain_icon::update(ui::dynamic_icon<terrain_icon>& ico, world_state& ws) {
		ico.set_visibility(ws.w.gui_m, true);
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
