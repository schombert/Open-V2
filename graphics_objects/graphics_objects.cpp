#include "graphics_objects.h"


namespace graphics {

	obj_definition_tag reserve_graphics_object(name_maps& nmaps, const char* name_start, const char* name_end) {
		return nmaps.names.try_emplace(std::string(name_start, name_end), obj_definition_tag(static_cast<value_base_of<obj_definition_tag>>(nmaps.names.size()))).first->second;
	}

	const char* format_error(errors  e) {
		switch (e) {
			case errors::unknown_attribute:
				return "unknown attribute";
			case errors::non_square_border_size:
				return "non-square border size";
			case errors::invalid_click_sound:
				return "invalid click sound (not one of click, close_window, or start_game)";
			case errors::invalid_number_of_frames:
				return "invalid number of frames (less than 0 or greater than 255)";
			case errors::unknown_file_level_type:
				return "unknown type found at file level";
			case errors::unknown_sprite_type:
				return "unknown sprite type";
			default:
				return "";
		}
	}
}
