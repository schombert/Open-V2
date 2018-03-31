#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "concurrency_tools\\concurrency_tools.hpp"

namespace scenario {
	class scenario_manager;
}

namespace triggers {
	struct trigger_code {
		constexpr static uint16_t is_scope = 0x8000;
		constexpr static uint16_t is_disjunctive_scope = 0x4000;
		constexpr static uint16_t is_existance_scope = 0x4000;

		constexpr static uint16_t association_mask = 0x7000;
		constexpr static uint16_t association_le = 0x1000;
		constexpr static uint16_t association_lt = 0x2000;
		constexpr static uint16_t association_ge = 0x3000;
		constexpr static uint16_t association_gt = 0x4000;
		constexpr static uint16_t association_eq = 0x5000;
		constexpr static uint16_t association_ne = 0x6000;

		constexpr static uint16_t code_mask = 0x0FFF;

		uint16_t type;
		uint16_t group_size;
	};

	union trigger_bytecode {
		trigger_code identifier;
		float fp_value;
		int32_t integer_value;
		bool boolean_value;
		technologies::invention_tag invention;

		trigger_bytecode(const trigger_bytecode& i) {
			memcpy(this, &i, sizeof(trigger_bytecode));
		}

		trigger_bytecode(trigger_code i) : identifier(i) {}
		trigger_bytecode(float i) : fp_value(i) {}
		trigger_bytecode(int32_t i) : integer_value(i) {}
		trigger_bytecode(bool i) : boolean_value(i) {}
		trigger_bytecode(technologies::invention_tag i) : invention(i) {}
	};

	enum class trigger_slot_contents {
		empty, province, state, pop, nation
	};

	struct trigger_scope_state {
		trigger_slot_contents main_slot = trigger_slot_contents::empty;
		trigger_slot_contents this_slot = trigger_slot_contents::empty;
		trigger_slot_contents from_slot = trigger_slot_contents::empty;
		bool contains_rebeltype = false;
	};

	static_assert(sizeof(trigger_bytecode) == 4);

	class trigger_manager {
	public:
		std::vector<trigger_bytecode> trigger_data;
		tagged_vector<uint32_t, trigger_tag> trigger_offsets;
	};
}
