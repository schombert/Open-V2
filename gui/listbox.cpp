#include "listbox.hpp"
#include "world_state\\world_state.h"

void ui::listbox_scrollbar::on_position(int32_t pos) const {
	if (_content_frame)
		_content_frame->position.y = static_cast<int16_t>(-pos * factor);
}

void ui::discrete_listbox_scrollbar::on_position(world_state& ws, scrollbar<discrete_listbox_scrollbar>& obj, int32_t pos) const {
	if(offset)
		*offset = uint32_t(pos);
	if(obj.associated_object) {
		gui_object_tag ptag = obj.associated_object->parent;
		if(is_valid_index(ptag)) 
			ui::make_visible_and_update(ws.w.gui_m, ws.w.gui_m.gui_objects.at(ptag));
	}
}
