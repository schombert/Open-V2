#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.h"

class world_state;

namespace population {
	struct rebel_faction;
}

namespace triggers {
	int32_t get_effect_payload_size(const uint16_t* data);
	int32_t effect_scope_data_payload(uint16_t code);
	int32_t get_random_list_effect_chances_sum(const uint16_t* data); // used to caluclate value at creation time
	const uint16_t* get_random_list_effect(const uint16_t* data, int32_t random_value); // value [0, chances sum - 1]

	template<typename T>
	void recurse_over_effects(uint16_t* source, const T& f) {
		f(source);

		if ((source[0] & effect_codes::is_scope) != 0) {
			if ((source[0] & effect_codes::code_mask) == effect_codes::random_list_scope) {
				const auto source_size = 1 + get_effect_payload_size(source);

				auto sub_units_start = source + 4; // [code] + [payload size] + [chances total] + [first sub effect chance]
				while (sub_units_start < source + source_size) {
					recurse_over_effects(sub_units_start, f);
					sub_units_start += 2 + get_effect_payload_size(sub_units_start); // each member preceeded by uint16_t
				}
			} else {
				const auto source_size = 1 + get_effect_payload_size(source);

				auto sub_units_start = source + 2 + effect_scope_data_payload(source[0]);
				while (sub_units_start < source + source_size) {
					recurse_over_effects(sub_units_start, f);
					sub_units_start += 1 + get_effect_payload_size(sub_units_start);
				}
			}
		}
	}

	void execute_effect(
		uint16_t const* tval,
		world_state& ws,
		void* primary_slot,
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng generator_copy);
}
