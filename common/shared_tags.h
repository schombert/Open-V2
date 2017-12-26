#pragma once
#include "common.h"
#include <variant>

namespace graphics {
	using obj_definition_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 174634>>;
	using texture_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 74896>>;
	using font_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 860367>>;
}

namespace ui {
	using button_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15356>>;
	using icon_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15357>>;
	using text_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15358>>;
	using position_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15359>>;
	using overlapping_region_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15360>>;
	using listbox_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15361>>;
	using scrollbar_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15362>>;
	using window_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 15363>>;

	using element_tag = std::variant<std::monostate, button_tag, icon_tag, text_tag, position_tag, overlapping_region_tag, listbox_tag, scrollbar_tag, window_tag>;

	using gui_object_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 856623>>;
	using text_instance_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 856624>>;
	using graphics_instance_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 856625>>;
	using multi_texture_instance_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 856626>>;
	using data_texture_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 856627>>;
	using lines_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 856628>>;
}

namespace text_data {
	using text_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 342678>>;
}