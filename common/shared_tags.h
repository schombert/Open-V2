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

namespace technologies {
	using tech_category_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 72648>>;
	using tech_subcategory_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 72649>>;
	using tech_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 72650>>;
	using invention_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 72651>>;
}

namespace ideologies {
	using ideology_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 785057>>;
	using ideology_group_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 785058>>;
}

namespace issues {
	using issue_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 8563>>;
	using unciv_issue_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 8564>>;
	using option_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 8565>>;
}

namespace population {
	using pop_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 395725>>;
	using pop_tag = tag_type<uint32_t, std::true_type, std::integral_constant<size_t, 395726>>;
	using rebel_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 395727>>;
}

namespace economy {
	using goods_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490175>>;
	using goods_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490176>>;
	using factory_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490177>>;
	using artisan_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490178>>;
}

namespace cultures {
	using culture_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 7367521>>;
	using culture_group_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 7367522>>;
	using religion_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7367523>>;
	using national_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 7367524>>;
}

namespace governments {
	using government_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 30607523>>;
}

namespace modifiers {
	using provincial_modifier_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 9478475>>;
	using national_modifier_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 9478476>>;
	using value_type = float;
}

namespace military {
	using unit_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 647474>>;
	using cb_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 647475>>;
}

namespace provinces {
	using province_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 77325564>>;
	using state_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 77325565>>;
}

namespace triggers {
	using trigger_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 93484522>>;
}

namespace text_data {
	using text_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 342678>>;
}

namespace variables {
	using national_variable_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 4745824>>;
	using global_variable_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 4745826>>;
}

template<typename tag_type, typename index_type, typename T, typename U>
inline tag_type tag_from_text(const boost::container::flat_map<index_type, tag_type, T, U>& map, index_type t) {
	const auto f = map.find(t);
	if (f != map.cend())
		return f->second;
	else
		return tag_type()
}

inline bool is_valid_index(const ui::element_tag& et) {
	return std::visit([](auto t) {
		if constexpr(std::is_same_v<std::monostate, decltype(t)>)
			return false;
		else
			return is_valid_index(t);
	}, et);
}
