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
		unexpected_icon_attribute,
		unknown_text_orientation,
		unknown_text_format,
		unexpected_text_attribute,
		unexpected_text_background,
		unexpected_position_attribute,
		unknown_overlapping_region_format,
		unknown_overlapping_region_orientation,
		unexpected_overlapping_region_attribute,
		unsupported_listbox_scrollbar_type,
		unexpected_listbox_step_value,
		unexpected_listbox_spacing_value,
		unexpected_listbox_priority,
		unknown_listbox_orientation,
		horizontal_listboxes_not_supported,
		unexpected_listbox_attribute,
		unexpected_scrollbar_priority,
		unexpected_scrollbar_minimum_value,
		unexpected_scrollbar_horizontal_value,
		unexpected_scrollbar_step_size,
		scrollbar_component_not_found,
		missing_necessary_scrollbar_component,
		unexpected_scrollbar_attribute
	};

	enum class element_types : uint8_t {
		button, icon, text, position, overlapping_region, listbox,
		scrollbar
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
		static constexpr uint8_t is_checkbox               = 0x80;

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
		static constexpr uint8_t orientation_mask           = 0x07;
		static constexpr uint8_t orientation_center         = 0x00;
		static constexpr uint8_t orientation_lower_left     = 0x01;
		static constexpr uint8_t orientation_lower_right    = 0x02;
		static constexpr uint8_t orientation_upper_left     = 0x03;
		static constexpr uint8_t orientation_upper_right    = 0x04;
		static constexpr uint8_t orientation_center_down    = 0x05;
		static constexpr uint8_t orientation_center_up      = 0x06;

		static constexpr uint8_t rotation_mask              = 0x30;
		static constexpr uint8_t rotation_upright           = 0x00;
		static constexpr uint8_t rotation_90_left           = 0x10;
		static constexpr uint8_t rotation_90_right          = 0x20;

		static constexpr uint8_t is_shield                  = 0x40;

		xy_pair position;
		float scale = 1.0f;
		uint16_t graphical_object_handle = 0;
		uint8_t frame = 0;
		uint8_t flags = 0;
	};

	struct text_def {
		static constexpr uint16_t orientation_mask                  = 0x0007;
		static constexpr uint16_t orientation_center                = 0x0000;
		static constexpr uint16_t orientation_lower_left            = 0x0001;
		static constexpr uint16_t orientation_lower_right           = 0x0002;
		static constexpr uint16_t orientation_upper_left            = 0x0003;
		static constexpr uint16_t orientation_upper_right           = 0x0004;
		static constexpr uint16_t orientation_center_up             = 0x0005;
		static constexpr uint16_t orientation_center_down           = 0x0006;

		static constexpr uint16_t always_transparent                = 0x0008;

		static constexpr uint16_t format_mask                       = 0x0030;
		static constexpr uint16_t format_center                     = 0x0010;
		static constexpr uint16_t format_left                       = 0x0000;
		static constexpr uint16_t format_right                      = 0x0020;
		static constexpr uint16_t format_justified                  = 0x0030;

		static constexpr uint16_t fixed_size                        = 0x0040;
		static constexpr uint16_t instant                           = 0x0080;

		static constexpr uint16_t background_mask                   = 0x0300;
		static constexpr uint16_t background_none_specified         = 0x0000;
		static constexpr uint16_t background_tiles_dialog_tga       = 0x0100;
		static constexpr uint16_t background_transparency_tga       = 0x0200;
		static constexpr uint16_t background_small_tiles_dialog_tga = 0x0300;

		static constexpr uint16_t is_edit_box                       = 0x0400;

		xy_pair position;
		xy_pair border_size;
		uint16_t font_handle = 0;
		uint16_t max_height = 0;
		uint16_t max_width = 0;
		uint16_t text_handle = 0;
		uint16_t flags = 0;
	};

	struct position_def {
		xy_pair position;
	};

	struct overlapping_region_def {
		static constexpr uint8_t orientation_mask         = 0x03;
		static constexpr uint8_t orientation_center       = 0x00;
		static constexpr uint8_t orientation_upper_left   = 0x01;
		static constexpr uint8_t orientation_upper_right  = 0x02;

		static constexpr uint8_t format_mask              = 0x0C;
		static constexpr uint8_t format_center            = 0x00;
		static constexpr uint8_t format_left              = 0x04;
		static constexpr uint8_t format_right             = 0x08;

		xy_pair position;
		xy_pair size;
		float spacing = 0.0;
		uint8_t flags = 0;
	};

	struct listbox_def {
		static constexpr uint8_t orientation_mask         = 0x30;
		static constexpr uint8_t orientation_center       = 0x00;
		static constexpr uint8_t orientation_upper_left   = 0x10;
		static constexpr uint8_t orientation_upper_right  = 0x20;
		static constexpr uint8_t orientation_center_down  = 0x30;

		static constexpr uint8_t always_transparent       = 0x40;

		static constexpr uint8_t spacing_mask             = 0x0F;

		xy_pair position;
		xy_pair size;
		xy_pair offset;
		xy_pair border_size;

		uint16_t background_handle = 0;
		uint8_t flags = 0;
	};

	struct scrollbar_def {
		static constexpr uint8_t is_horizontal   = 0x01;
		static constexpr uint8_t has_range_limit = 0x02;
		static constexpr uint8_t is_lockable     = 0x04;

		static constexpr uint8_t step_mask           = 0xF0;
		static constexpr uint8_t step_one            = 0x00;
		static constexpr uint8_t step_two            = 0x10;
		static constexpr uint8_t step_one_tenth      = 0x20;
		static constexpr uint8_t step_one_hundredth  = 0x30;
		static constexpr uint8_t step_one_thousandth = 0x40;

		xy_pair border_size;
		xy_pair position;
		xy_pair size;
		uint16_t max_value = 1;
		uint16_t minimum_button = 0;
		uint16_t maximum_button = 0;
		uint16_t track = 0;
		uint16_t slider = 0;
		uint16_t minimum_limit_icon = 0;
		uint16_t maximum_limit_icon = 0;
		uint8_t flags = 0;
	};

	struct name_maps {
		std::vector<std::string> button_names;
		std::vector<std::string> icon_names;
		std::vector<std::string> text_names;
		std::vector<std::string> position_names;
		std::vector<std::string> overlapping_region_names;
		std::vector<std::string> listbox_names;
		std::vector<std::string> scrollbar_names;

		const std::string& get_name(element_types t, uint16_t handle) {
			switch (t) {
				case element_types::button:
					return button_names[handle - 1];
				case element_types::icon:
					return icon_names[handle - 1];
				case element_types::text:
					return text_names[handle - 1];
				case element_types::position:
					return position_names[handle - 1];
				case element_types::overlapping_region:
					return overlapping_region_names[handle - 1];
				case element_types::listbox:
					return listbox_names[handle - 1];
				case element_types::scrollbar:
					return scrollbar_names[handle - 1];
			}
		}
	};

	struct definitions {
		std::vector<button_def> buttons;
		std::vector<icon_def> icons;
		std::vector<text_def> text;
		std::vector<position_def> positions;
		std::vector<overlapping_region_def> overlapping_regions;
		std::vector<listbox_def> listboxes;
		std::vector<scrollbar_def> scrollbars;
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