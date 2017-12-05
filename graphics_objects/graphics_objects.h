#pragma once
#include <stdint.h>
#include <functional>
#include "simple_fs\\simple_fs.h"
#include "boost\\container\\flat_map.hpp"

namespace graphics {
	enum class errors {
		unknown_attribute,
		non_square_border_size,
		invalid_number_of_frames,
		invalid_click_sound,
		unknown_file_level_type,
		unknown_sprite_type
	};

	enum class object_type : uint8_t {
		 generic_sprite = 0x00,
		 bordered_rect = 0x01,
		 horizontal_progress_bar = 0x02,
		 vertical_progress_bar = 0x03,
		 flag_mask = 0x04,
		 tile_sprite = 0x05,
		 text_sprite = 0x06,
		 barchart = 0x07,
		 piechart = 0x08,
		 linegraph = 0x09
	};

	struct xy_pair {
		int16_t x = 0;
		int16_t y = 0;
	};

	struct object {
		constexpr static uint8_t always_transparent = 0x10;
		constexpr static uint8_t flip_v = 0x20;
		constexpr static uint8_t has_click_sound = 0x40;
		constexpr static uint8_t do_transparency_check = 0x80;

		constexpr static uint8_t type_mask = 0x0F;

		xy_pair size; //4bytes
		
		uint16_t primary_texture_handle = 0; //6bytes
		uint16_t type_dependant = 0; // secondary texture handle or border size -- 8bytes

		uint8_t flags = 0; //9bytes
		uint8_t number_of_frames = 1; //10bytes
	};

	struct name_maps {
		boost::container::flat_map<std::string, uint16_t> names;
	};

	struct object_definitions {
		std::vector<object> definitions;
	};
};

using texture_lookup = std::function<uint16_t(const char*, const char*)>;
using special_object_lookup = std::function<uint16_t(int32_t type)>;

void load_graphics_object_definitions_from_directory(
	const directory& source_directory,
	graphics::name_maps& nmaps,
	graphics::object_definitions& defs,
	std::vector<std::pair<std::string, graphics::errors>>& errors_generated,
	const texture_lookup& th_f,
	const special_object_lookup& sh_f);