#pragma once
#include "common.h"
#include <variant>

using date_tag = tag_type<int32_t, std::true_type, std::integral_constant<size_t, 42>>;

inline const boost::gregorian::date base_date(1800, boost::gregorian::Jan, 1);

inline date_tag date_to_tag(const boost::gregorian::date& d) {
	return date_tag(static_cast<date_tag::value_base_t>((d - base_date).days()));
}

inline char16_t* u16_format_date(char16_t* dest, date_tag date) {
	auto unpacked = base_date + boost::gregorian::days(to_index(date));
	auto ymd = unpacked.year_month_day();

	auto year_end = _u16itoa(uint32_t(ymd.year), dest);
	*year_end = u'.';
	auto month_end = _u16itoa(uint32_t(ymd.month), year_end + 1);
	*month_end = u'.';
	auto day_end = _u16itoa(uint32_t(ymd.day), month_end + 1);
	*day_end = char16_t(0);
	return day_end;
}

inline boost::gregorian::date tag_to_date(date_tag t) {
	return base_date + boost::gregorian::days(to_index(t));
}

namespace graphics {
	using obj_definition_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 174634>>;
	using texture_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 74896>>;
	using font_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 860367>>;
}

namespace sound {
	using effect_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 370167>>;
	using music_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 370168>>;
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
	using rebel_adjustment_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 72651>>;
	using production_adjustment_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 72652>>;
	using unit_adjustment_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 72653>>;
}

namespace ideologies {
	using ideology_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 785057>>;
	using ideology_group_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 785058>>;
}

namespace issues {
	using issue_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 8563>>;
	using option_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 8565>>;
}

namespace population {
	using pop_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 395725>>;
	using pop_tag = tag_type<uint32_t, std::true_type, std::integral_constant<size_t, 395726>>;
	using rebel_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 395727>>;
	using rebel_faction_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 395728>>;
	using movement_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 395729>>;
	using demo_tag = tag_type<uint32_t, std::true_type, std::integral_constant<size_t, 395730>>;
	using fp_demo_tag = tag_type<uint32_t, std::true_type, std::integral_constant<size_t, 395731>>;
}

namespace economy {
	using goods_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490175>>;
	using goods_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490176>>;
	using factory_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490177>>;
	using artisan_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7490178>>;

	using goods_qnty_type = double;
	using money_qnty_type = double;
}

namespace cultures {
	using culture_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 7367521>>;
	using culture_group_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 7367522>>;
	using religion_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 7367523>>;
	using national_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 7367524>>;
}

namespace nations {
	using country_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 22120424>>;
	using state_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 22120425>>;
}

namespace governments {
	using government_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 30607523>>;
	using party_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 30607524>>;
}

namespace modifiers {
	using provincial_modifier_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 9478475>>;
	using national_modifier_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 9478476>>;
	using factor_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 9478477>>;
	using national_focus_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 9478478>>;
	using value_type = float;
}

namespace military {
	using unit_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 647474>>;
	using cb_type_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 647475>>;
	using leader_trait_tag = tag_type<uint8_t, std::true_type, std::integral_constant<size_t, 647476>>;
	using army_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 647477>>;
	using leader_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 647478>>;
	using fleet_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 647479>>;
	using war_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 647480>>;
	using army_orders_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 647481>>;

	struct war_identifier {
		war_tag war_id;
		bool is_attacker = false;
	};
}

namespace provinces {
	using province_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 77325564>>;
	using state_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 77325565>>;
}

namespace triggers {
	using trigger_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 93484522>>;
	using effect_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 93484523>>;

	enum class trigger_slot_contents {
		empty = 0,
		province = 1,
		state = 2,
		pop = 3,
		nation = 4
	};

	struct trigger_scope_state {
		trigger_slot_contents main_slot = trigger_slot_contents::empty;
		trigger_slot_contents this_slot = trigger_slot_contents::empty;
		trigger_slot_contents from_slot = trigger_slot_contents::empty;
		bool contains_rebeltype = false;

		int32_t to_integer() const {
			return int32_t(main_slot) + int32_t(this_slot) * 8 + int32_t(from_slot) * 8 * 8 + int32_t(contains_rebeltype) * 8 * 8 * 8;
		}
	};
}

namespace text_data {
	using text_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 342678>>;
}

namespace variables {
	using national_variable_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 4745824>>;
	using national_flag_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 4745825>>;
	using global_variable_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 4745826>>;
}

namespace events {
	using event_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 75935234>>;
	using decision_tag = tag_type<uint16_t, std::true_type, std::integral_constant<size_t, 75935235>>;
}

template<typename tag_type, typename index_type, typename T, typename U>
inline tag_type tag_from_text(const boost::container::flat_map<index_type, tag_type, T, U>& map, index_type t) {
	const auto f = map.find(t);
	if (f != map.cend())
		return f->second;
	else
		return tag_type();
}

inline bool is_valid_index(const ui::element_tag& et) {
	return std::visit([](auto t) {
		if constexpr(std::is_same_v<std::monostate, decltype(t)>)
			return false;
		else
			return is_valid_index(t);
	}, et);
}
