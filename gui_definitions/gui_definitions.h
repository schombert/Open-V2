#pragma once
#include <stdint.h>
#include "common\\common.h"
#include <vector>
#include <string>
#include <functional>
#include "boost\\container\\flat_map.hpp"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"

namespace ui {
	enum class errors {
		expected_tooltip_empty_for_button,
		expected_tooltiptext_empty_for_button,
		expected_delayedtooltiptext_empty_for_button,
		expected_button_format_to_be_left,
		unknown_button_orientation,
		unknown_button_shortcut,
		unexpected_button_rotation,
		unexpected_button_clicksound,
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
		unexpected_scrollbar_attribute,
		unexpected_window_attribute,
		unknown_window_orientation,
		window_background_not_found,
		unexpected_window_moveable_value,
		unknown_definition_type,
		missing_standardlistbox_slider
	};

	const char* format_error(errors  e);

	enum class element_type : uint8_t {
		button = 1,
		icon = 2,
		text = 3,
		position = 4,
		overlapping_region = 5,
		listbox = 6,
		scrollbar = 7,
		window = 8
	};

	struct xy_pair {
		int16_t x = 0;
		int16_t y = 0;

		constexpr ui::xy_pair operator+(ui::xy_pair other) const {
			return xy_pair{x+other.x, y+other.y};
		}
		void operator+=(ui::xy_pair other) {
			x += other.x;
			y += other.y;
		}
		constexpr bool operator==(ui::xy_pair other) const {
			return x == other.x && y == other.y;
		}
	};

	struct button_def {
		static constexpr uint8_t orientation_mask          = 0x07;
		static constexpr uint8_t orientation_center        = 0x00;
		static constexpr uint8_t orientation_lower_left    = 0x01;
		static constexpr uint8_t orientation_lower_right   = 0x02;
		static constexpr uint8_t orientation_upper_left    = 0x03;
		static constexpr uint8_t orientation_upper_right   = 0x04;
		static constexpr uint8_t format_mask               = 0x08;
		static constexpr uint8_t format_center             = 0x00;
		static constexpr uint8_t format_left               = 0x08;
		static constexpr uint8_t clicksound_mask           = 0x30;
		static constexpr uint8_t clicksound_none           = 0x00;
		static constexpr uint8_t clicksound_click          = 0x10;
		static constexpr uint8_t clicksound_close_window   = 0x20;
		static constexpr uint8_t clicksound_start_game     = 0x30;
		static constexpr uint8_t rotation_mask             = 0x40;
		static constexpr uint8_t rotation_upright          = 0x00;
		static constexpr uint8_t rotation_90_left          = 0x40;
		static constexpr uint8_t is_checkbox               = 0x80;

		xy_pair position; //4bytes
		xy_pair size; //8bytes

		graphics::obj_definition_tag graphical_object_handle; //10bytes
		text_data::text_tag text_handle; //12bytes
		uint16_t font_handle = 0; //14bytes

		uint8_t flags = 0; //15 bytes
		virtual_key shortcut = virtual_key::NONE; //16bytes
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
		graphics::obj_definition_tag graphical_object_handle;
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
		text_data::text_tag text_handle;
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

		graphics::obj_definition_tag background_handle;
		uint8_t flags = 0;
	};

	struct scrollbar_def {
		static constexpr uint8_t is_horizontal       = 0x01;
		static constexpr uint8_t has_range_limit     = 0x02;
		static constexpr uint8_t is_lockable         = 0x04;

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
		button_tag minimum_button;
		button_tag maximum_button;
		button_tag track;
		button_tag slider;
		icon_tag minimum_limit_icon;
		icon_tag maximum_limit_icon;
		uint8_t flags = 0;
	};

	struct window_def {
		static constexpr uint8_t is_dialog                = 0x01;
		static constexpr uint8_t is_fullscreen            = 0x02;
		static constexpr uint8_t is_moveable              = 0x04;

		static constexpr uint8_t orientation_mask         = 0x70;
		static constexpr uint8_t orientation_center       = 0x00;
		static constexpr uint8_t orientation_lower_left   = 0x10;
		static constexpr uint8_t orientation_lower_right  = 0x20;
		static constexpr uint8_t orientation_upper_left   = 0x30;
		static constexpr uint8_t orientation_upper_right  = 0x40;

		std::vector<element_tag> sub_object_definitions;
		ui::xy_pair position;
		ui::xy_pair size;
		button_tag background_handle;
		uint8_t flags = 0;
	};

	struct name_maps {
		std::vector<vector_backed_string<char>> button_names;
		std::vector<vector_backed_string<char>> icon_names;
		std::vector<vector_backed_string<char>> text_names;
		std::vector<vector_backed_string<char>> position_names;
		std::vector<vector_backed_string<char>> overlapping_region_names;
		std::vector<vector_backed_string<char>> listbox_names;
		std::vector<vector_backed_string<char>> scrollbar_names;
		std::vector<vector_backed_string<char>> window_names;

		std::string get_name(element_tag handle, std::vector<char>& backing_data) {
			return std::visit([_this = this, &backing_data](auto tag) {
				if constexpr(std::is_same_v<button_tag, decltype(tag)>)
					return _this->button_names[to_index(tag)].get_string(backing_data);
				else if constexpr(std::is_same_v<icon_tag, decltype(tag)>)
					return _this->icon_names[to_index(tag)].get_string(backing_data);
				else if constexpr(std::is_same_v<text_tag, decltype(tag)>)
					return _this->text_names[to_index(tag)].get_string(backing_data);
				else if constexpr(std::is_same_v<position_tag, decltype(tag)>)
					return _this->position_names[to_index(tag)].get_string(backing_data);
				else if constexpr(std::is_same_v<overlapping_region_tag, decltype(tag)>)
					return _this->overlapping_region_names[to_index(tag)].get_string(backing_data);
				else if constexpr(std::is_same_v<listbox_tag, decltype(tag)>)
					return _this->listbox_names[to_index(tag)].get_string(backing_data);
				else if constexpr(std::is_same_v<scrollbar_tag, decltype(tag)>)
					return _this->scrollbar_names[to_index(tag)].get_string(backing_data);
				else if constexpr(std::is_same_v<window_tag, decltype(tag)>)
					return _this->window_names[to_index(tag)].get_string(backing_data);
				else
					return std::string();
			}, handle);
		}

		vector_backed_string<char> get_raw_name(element_tag handle) {
			return std::visit([_this = this](auto tag) {
				if constexpr(std::is_same_v<button_tag, decltype(tag)>)
					return _this->button_names[to_index(tag)];
				else if constexpr(std::is_same_v<icon_tag, decltype(tag)>)
					return _this->icon_names[to_index(tag)];
				else if constexpr(std::is_same_v<text_tag, decltype(tag)>)
					return _this->text_names[to_index(tag)];
				else if constexpr(std::is_same_v<position_tag, decltype(tag)>)
					return _this->position_names[to_index(tag)];
				else if constexpr(std::is_same_v<overlapping_region_tag, decltype(tag)>)
					return _this->overlapping_region_names[to_index(tag)];
				else if constexpr(std::is_same_v<listbox_tag, decltype(tag)>)
					return _this->listbox_names[to_index(tag)];
				else if constexpr(std::is_same_v<scrollbar_tag, decltype(tag)>)
					return _this->scrollbar_names[to_index(tag)];
				else if constexpr(std::is_same_v<window_tag, decltype(tag)>)
					return _this->window_names[to_index(tag)];
				else
					return vector_backed_string<char>();
			}, handle);
		}
	};

	struct definitions {
		tagged_vector<button_def, button_tag> buttons;
		tagged_vector<icon_def, icon_tag> icons;
		tagged_vector<text_def, text_tag> text;
		tagged_vector<position_def, position_tag> positions;
		tagged_vector<overlapping_region_def, overlapping_region_tag> overlapping_regions;
		tagged_vector<listbox_def, listbox_tag> listboxes;
		tagged_vector<scrollbar_def, scrollbar_tag> scrollbars;
		tagged_vector<window_def, window_tag> windows;

		scrollbar_tag standardlistbox_slider;

		std::vector<char> name_data;
		boost::container::flat_map<vector_backed_string<char>, element_tag, vector_backed_string_less_ci> name_to_element_map;

		definitions() : name_to_element_map(vector_backed_string_less_ci(name_data)) {}
	};

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;
	using font_handle_lookup = std::function<uint16_t(const char*, const char*)>;
	using gobj_lookup = std::function<graphics::obj_definition_tag(const char*, const char*)>;

	void load_ui_definitions_from_directory(
		const directory& source_directory,
		ui::name_maps& nmaps,
		ui::definitions& defs,
		std::vector<std::pair<std::string, ui::errors>>& errors_generated,
		const text_handle_lookup& th_f,
		const font_handle_lookup& fh_f,
		const gobj_lookup& qt_f);
};

