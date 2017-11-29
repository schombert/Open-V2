#pragma once
#include <stdint.h>
#include "common\\common.h"
#include <vector>
#include <functional>
#include "boost\\container\\flat_map.hpp"
#include "simple_fs\\simple_fs.h"

namespace ui {
	enum class errors {
		expected_tooltip_empty_for_button,
		expected_tooltiptext_empty_for_button,
		expected_delayedtooltiptext_empty_for_button,
		expected_button_format_to_be_left,
		unknown_button_orientation,
		unknown_button_shortcut,
		unexpected_button_rotation,
		unexpected_button_attribute,
		unknown_icon_orientation,
		unexpected_icon_rotation,
		unexpected_icon_attribute
	};

	enum class element_types : uint8_t {
		button, icon
	};

	struct xy_pair {
		int16_t x = 0;
		int16_t y = 0;
	};

	struct button_def {
		static constexpr uint8_t orientation_mask          = 0x07;
		static constexpr uint8_t orientation_center        = 0x00;
		static constexpr uint8_t orientation_lower_left    = 0x01;
		static constexpr uint8_t orientation_lower_right   = 0x02;
		static constexpr uint8_t orientation_upper_left    = 0x03;
		static constexpr uint8_t orientation_upper_right   = 0x04;
		static constexpr uint8_t format_left               = 0x08;
		static constexpr uint8_t rotation_mask             = 0x30;
		static constexpr uint8_t rotation_upright          = 0x00;
		static constexpr uint8_t rotation_90_left          = 0x10;
		static constexpr uint8_t rotation_90_right         = 0x20;
		static constexpr uint8_t graphical_obj_type_mask   = 0x40;
		static constexpr uint8_t graphical_obj_qtex_type   = 0x00;
		static constexpr uint8_t graphical_obj_sprite_type = 0x40;

		xy_pair position;
		xy_pair size;

		uint16_t graphical_object_handle = 0;
		uint16_t text_handle = 0;
		uint16_t font_handle = 0;
		uint16_t clicksound_handle = 0;

		uint8_t flags = 0;
		virtual_key shortcut = virtual_key::NONE;
	};

	struct icon_def {
		static constexpr uint8_t orientation_mask = 0x07;
		static constexpr uint8_t orientation_center = 0x00;
		static constexpr uint8_t orientation_lower_left = 0x01;
		static constexpr uint8_t orientation_lower_right = 0x02;
		static constexpr uint8_t orientation_upper_left = 0x03;
		static constexpr uint8_t orientation_upper_right = 0x04;
		static constexpr uint8_t orientation_center_down = 0x05;
		static constexpr uint8_t orientation_center_up = 0x06;

		static constexpr uint8_t rotation_mask = 0x30;
		static constexpr uint8_t rotation_upright = 0x00;
		static constexpr uint8_t rotation_90_left = 0x10;
		static constexpr uint8_t rotation_90_right = 0x20;

		xy_pair position;
		float scale = 1.0f;
		uint16_t graphical_object_handle = 0;
		uint8_t frame = 0;
		uint8_t flags = 0;
	};

	struct name_maps {
		std::vector<std::string> button_names;
		std::vector<std::string> icon_names;

		const std::string& get_name(element_types t, uint16_t handle) {
			switch (t) {
				case element_types::button:
					return button_names[handle - 1];
				case element_types::icon:
					return icon_names[handle - 1];
			}
		}
	};

	struct definitions {
		std::vector<button_def> buttons;
		std::vector<icon_def> icons;
	};
};

using text_handle_lookup = std::function<uint16_t(const char*, const char*)>;
using font_handle_lookup = std::function<uint16_t(const char*, const char*)>;
using gobj_lookup = std::function<uint16_t(const char*, const char*)>;
using sound_lookup = std::function<uint16_t(const char*, const char*)>;

void load_ui_definitions_from_directory(
	const directory& source_directory,
	ui::name_maps& nmaps,
	ui::definitions& defs,
	std::vector<std::pair<std::string,ui::errors>>& errors_generated,
	const text_handle_lookup& th_f,
	const font_handle_lookup& fh_f,
	const gobj_lookup& qt_f,
	const sound_lookup& sl_f);