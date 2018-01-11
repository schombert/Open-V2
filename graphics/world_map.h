#pragma once
#include "common\\common.h"

namespace graphics {
	std::pair<uint32_t, uint32_t> build_region_arrays( uint16_t* map_data, int32_t width, int32_t height, float top_latitude, float bottom_latitude, float left_longitude);
}