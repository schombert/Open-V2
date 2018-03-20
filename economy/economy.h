#pragma once
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"

namespace economy {
	struct good_definition {
		static constexpr uint8_t not_available_from_start = 0x01;
		static constexpr uint8_t overseas_penalty         = 0x02;
		static constexpr uint8_t money                    = 0x04;

		double base_price = 1.0;
		graphics::color_rgb color = {0, 0, 0};
		text_data::text_tag name;

		goods_tag id;
		goods_type_tag type;
		uint8_t flags = 0;
	};

	class economic_scenario {
	public:
		tagged_vector<text_data::text_tag, goods_type_tag> good_type_names;
		tagged_vector<good_definition, goods_tag> goods; // note: last = money

		goods_tag money;

		boost::container::flat_map<text_data::text_tag, goods_tag> named_goods_index;
	};

	using text_handle_lookup = std::function<text_data::text_tag(const char*, const char*)>;

	void read_goods(
		economic_scenario& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function);
}
