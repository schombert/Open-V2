#include "common\\common.h"
#include "icons.hpp"

namespace ui {
	tagged_gui_object create_static_element(world_state& ws, icon_tag handle, tagged_gui_object parent, background_icon& b) {
		auto new_obj = ui::detail::create_element_instance(ws, handle);

		new_obj.object.associated_behavior = &b;
		b.associated_object = &new_obj.object;

		ui::add_to_back(ws.w.gui_m, parent, new_obj);
		return new_obj;
	}
}
