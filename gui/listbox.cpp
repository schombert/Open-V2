#include "listbox.hpp"

void ui::listbox_scrollbar::on_position(int32_t pos) const {
	if (_content_frame)
		_content_frame->position.y = -pos;
}