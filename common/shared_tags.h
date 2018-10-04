#pragma once
#include "common.h"
#include <variant>

using date_tag = tag_type<int32_t, std::true_type, struct date_tag_type>;

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
	using obj_definition_tag = tag_type<uint16_t, std::true_type, struct obj_definition_tag_type>;
	using texture_tag = tag_type<uint16_t, std::true_type, struct texture_tag_type>;
	using font_tag = tag_type<uint8_t, std::true_type, struct font_tag_type>;
}

namespace sound {
	using effect_tag = tag_type<uint8_t, std::true_type, struct effect_tag_type>;
	using music_tag = tag_type<uint8_t, std::true_type, struct music_tag_type>;
}

namespace ui {
	using button_tag = tag_type<uint16_t, std::true_type, struct button_tag_type>;
	using icon_tag = tag_type<uint16_t, std::true_type, struct icon_tag_type>;
	using text_tag = tag_type<uint16_t, std::true_type, struct text_tag_type>;
	using position_tag = tag_type<uint16_t, std::true_type, struct position_tag_type>;
	using overlapping_region_tag = tag_type<uint16_t, std::true_type, struct overlapping_region_tag_type>;
	using listbox_tag = tag_type<uint16_t, std::true_type, struct listbox_tag_type>;
	using scrollbar_tag = tag_type<uint16_t, std::true_type, struct scrollbar_tag_type>;
	using window_tag = tag_type<uint16_t, std::true_type, struct window_tag_type>;

	using element_tag = std::variant<std::monostate, button_tag, icon_tag, text_tag, position_tag, overlapping_region_tag, listbox_tag, scrollbar_tag, window_tag>;

	using gui_object_tag = tag_type<uint16_t, std::true_type, struct gui_object_tag_type>;
	using text_instance_tag = tag_type<uint16_t, std::true_type, struct text_instance_tag_type>;
	using graphics_instance_tag = tag_type<uint16_t, std::true_type, struct graphics_instance_tag_type>;
	using multi_texture_instance_tag = tag_type<uint16_t, std::true_type, struct multi_texture_instance_tag_type>;
	using data_texture_tag = tag_type<uint16_t, std::true_type, struct data_texture_tag_type>;
	using lines_tag = tag_type<uint16_t, std::true_type, struct lines_tag_type>;
	using tinted_icon_instance_tag = tag_type<uint16_t, std::true_type, struct tinted_icon_instance_tag_type>;
}

namespace technologies {
	using tech_category_tag = tag_type<uint8_t, std::true_type, struct tech_category_tag_type>;
	using tech_subcategory_tag = tag_type<uint8_t, std::true_type, struct tech_subcategory_tag_type>;
	using tech_tag = tag_type<uint16_t, std::true_type, struct tech_tag_type>;
	using rebel_adjustment_tag = tag_type<uint16_t, std::true_type, struct rebel_adjustment_tag_type>;
	using production_adjustment_tag = tag_type<uint16_t, std::true_type, struct production_adjustment_tag_type>;
	using unit_adjustment_tag = tag_type<uint16_t, std::true_type, struct unit_adjustment_tag_type>;
}

namespace ideologies {
	using ideology_tag = tag_type<uint8_t, std::true_type, struct ideology_tag_type>;
	using ideology_group_tag = tag_type<uint8_t, std::true_type, struct ideology_group_tag_type>;
}

namespace issues {
	using issue_tag = tag_type<uint8_t, std::true_type, struct issue_tag_type>;
	using option_tag = tag_type<uint8_t, std::true_type, struct option_tag_type>;
}

namespace population {
	using pop_type_tag = tag_type<uint8_t, std::true_type, struct pop_type_tag_type>;
	using pop_tag = tag_type<uint32_t, std::true_type, struct pop_tag_type>;
	using rebel_type_tag = tag_type<uint8_t, std::true_type, struct rebel_type_tag_type>;
	using rebel_faction_tag = tag_type<uint16_t, std::true_type, struct rebel_faction_tag_type>;
	using movement_tag = tag_type<uint16_t, std::true_type, struct movement_tag_type>;
	using demo_tag = tag_type<uint32_t, std::true_type, struct demo_tag_type>;
	using fp_demo_tag = tag_type<uint32_t, std::true_type, struct fp_demo_tag_type>;
}

namespace economy {
	using goods_tag = tag_type<uint8_t, std::true_type, struct goods_tag_type>;
	using goods_type_tag = tag_type<uint8_t, std::true_type, struct goods_type_tag_type>;
	using factory_type_tag = tag_type<uint8_t, std::true_type, struct factory_type_tag_type>;
	using artisan_type_tag = tag_type<uint8_t, std::true_type, struct artisan_type_tag_type>;

	using goods_qnty_type = double;
	using money_qnty_type = double;
}

namespace cultures {
	using culture_tag = tag_type<uint16_t, std::true_type, struct culture_tag_type>;
	using culture_group_tag = tag_type<uint16_t, std::true_type, struct culture_group_tag_type>;
	using religion_tag = tag_type<uint8_t, std::true_type, struct religion_tag_type>;
	using national_tag = tag_type<uint16_t, std::true_type, struct national_tag_type>;
}

namespace nations {
	using country_tag = tag_type<uint16_t, std::true_type, struct country_tag_type>;
	using state_tag = tag_type<uint16_t, std::true_type, struct state_tag_type>;
}

namespace governments {
	using government_tag = tag_type<uint8_t, std::true_type, struct government_tag_type>;
	using party_tag = tag_type<uint16_t, std::true_type, struct party_tag_type>;
}

namespace modifiers {
	using provincial_modifier_tag = tag_type<uint16_t, std::true_type, struct provincial_modifier_tag_type>;
	using national_modifier_tag = tag_type<uint16_t, std::true_type, struct national_modifier_tag_type>;
	using factor_tag = tag_type<uint16_t, std::true_type, struct factor_tag_type>;
	using national_focus_tag = tag_type<uint8_t, std::true_type, struct national_focus_tag_type>;
	using value_type = float;
}

namespace military {
	using unit_type_tag = tag_type<uint8_t, std::true_type, struct unit_type_tag_type>;
	using cb_type_tag = tag_type<uint8_t, std::true_type, struct cb_type_tag_type>;
	using leader_trait_tag = tag_type<uint8_t, std::true_type, struct leader_trait_tag_type>;
	using army_tag = tag_type<uint16_t, std::true_type, struct army_tag_type>;
	using leader_tag = tag_type<uint16_t, std::true_type, struct leader_tag_type>;
	using fleet_tag = tag_type<uint16_t, std::true_type, struct fleet_tag_type>;
	using war_tag = tag_type<uint16_t, std::true_type, struct war_tag_type>;
	using army_orders_tag = tag_type<uint16_t, std::true_type, struct army_orders_tag_type>;

	struct war_identifier {
		war_tag war_id;
		bool is_attacker = false;

		bool operator<(war_identifier const& other)  const noexcept { return war_id < other.war_id; }
		bool operator==(war_identifier const& other) const noexcept { return war_id == other.war_id; }
	};

	struct pending_cb {
		nations::country_tag target;
		military::cb_type_tag type;
		date_tag expiration;

		bool operator==(pending_cb const& other) const noexcept { return target == other.target && type == other.type; }
	};
}

namespace provinces {
	using province_tag = tag_type<uint16_t, std::true_type, struct province_tag_type>;
	using state_tag = tag_type<uint16_t, std::true_type, struct state_tag_type>;
}

namespace triggers {
	using trigger_tag = tag_type<uint16_t, std::true_type, struct trigger_tag_type>;
	using effect_tag = tag_type<uint16_t, std::true_type, struct effect_tag_type>;

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
	using text_tag = tag_type<uint16_t, std::true_type, struct text_tag_type>;
}

namespace variables {
	using national_variable_tag = tag_type<uint16_t, std::true_type, struct national_variable_tag_type>;
	using national_flag_tag = tag_type<uint16_t, std::true_type, struct national_flag_tag_type>;
	using global_variable_tag = tag_type<uint16_t, std::true_type, struct global_variable_tag_type>;
}

namespace events {
	using event_tag = tag_type<uint16_t, std::true_type, struct event_tag_type>;
	using decision_tag = tag_type<uint16_t, std::true_type, struct decision_tag_type>;
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
