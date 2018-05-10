#include "effects.h"
#include "codes.h"

namespace triggers {
	int32_t get_effect_payload_size(const uint16_t* data) {
		const auto payload_mask = uint16_t(-int16_t((effect_codes::no_payload & data[0]) == 0));
		return payload_mask & data[1];
	}
	int32_t effect_scope_data_payload(uint16_t code) {
		const auto masked_code = code & effect_codes::code_mask;
		if ((masked_code == effect_codes::tag_scope) |
			(masked_code == effect_codes::integer_scope) |
			(masked_code == effect_codes::pop_type_scope_nation) |
			(masked_code == effect_codes::pop_type_scope_state) |
			(masked_code == effect_codes::pop_type_scope_province) |
			(masked_code == effect_codes::region_scope) |
			(masked_code == effect_codes::random_scope))
			return 1 + ((code & effect_codes::scope_has_limit) != 0);
		return 0 + ((code & effect_codes::scope_has_limit) != 0);
	}

	int32_t get_random_list_effect_chances_sum(const uint16_t* data) {
		const auto source_size = 1 + get_effect_payload_size(data);
		int32_t sum = 0;

		auto sub_units_start = data + 3;
		while (sub_units_start < data + source_size) {
			sum += sub_units_start[0];
			sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1);
		}

		return sum;
	}

	const uint16_t* get_random_list_effect(const uint16_t* data, int32_t random_value) {
		const auto source_size = 1 + get_effect_payload_size(data);

		auto sub_units_start = data + 3;
		while (sub_units_start < data + source_size) {
			random_value -= static_cast<int32_t>(sub_units_start[0]);
			if (random_value < 0)
				return sub_units_start + 1;
			sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1);
		}

		return data + 2; // default to first member 
	}
}
