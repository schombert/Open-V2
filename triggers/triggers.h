#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "Parsers\\parsers.h"

namespace scenario {
	class scenario_manager;
}

namespace triggers {
	struct trigger_code {
		constexpr static uint16_t is_scope = 0x8000;
		constexpr static uint16_t is_disjunctive_scope = 0x4000;
		constexpr static uint16_t is_existance_scope = 0x2000;

		constexpr static uint16_t association_mask = 0x7000;
		constexpr static uint16_t association_eq = 0x1000;
		constexpr static uint16_t association_gt = 0x2000;
		constexpr static uint16_t association_ge = 0x3000;
		constexpr static uint16_t association_lt = 0x4000;
		constexpr static uint16_t association_le = 0x5000;
		constexpr static uint16_t association_ne = 0x6000;

		constexpr static uint16_t code_mask = 0x0FFF;

		uint16_t type;
		uint16_t group_size;
	};

	inline uint16_t association_to_trigger_code(association_type a) {
		switch (a) {
			case association_type::eq: return trigger_code::association_eq;
			case association_type::eq_default: return trigger_code::association_ge;
			case association_type::ge: return trigger_code::association_ge;
			case association_type::gt: return trigger_code::association_gt;
			case association_type::lt: return trigger_code::association_lt;
			case association_type::le: return trigger_code::association_le;
			case association_type::ne: return trigger_code::association_ne;
			case association_type::none: return trigger_code::association_ge;
			case association_type::list: return trigger_code::association_ge;
		}
	}

	inline uint16_t invert_association(uint16_t a) {
		return static_cast<uint16_t>(0x7000) - a;
	}

	inline uint16_t association_to_bool_code(association_type a, const token_and_type& t) {
		if (token_to<bool>(t)) {
			if ((a == association_type::eq) | (a == association_type::eq_default))
				return trigger_code::association_eq;
			else
				return trigger_code::association_ne;
		} else {
			if ((a == association_type::eq) | (a == association_type::eq_default))
				return trigger_code::association_ne;
			else
				return trigger_code::association_eq;
		}
	}

	union trigger_bytecode {
		trigger_code identifier;
		float fp_value;
		int32_t integer_value;
		bool boolean_value;
		technologies::invention_tag invention;
		governments::government_tag government;
		modifiers::national_modifier_tag nat_mod;
		modifiers::provincial_modifier_tag prov_mod;
		cultures::culture_tag culture;
		cultures::religion_tag religion;
		economy::goods_tag good;
		population::rebel_type_tag rebel;
		cultures::national_tag tag;
		variables::national_variable_tag nat_var;
		variables::global_variable_tag global_var;
		economy::factory_type_tag factory;
		provinces::state_tag state;
		text_data::text_tag text;
		ideologies::ideology_tag ideology;
		issues::issue_tag issue;
		issues::option_identifier issue_option;
		military::cb_type_tag cb_type;

		trigger_bytecode(const trigger_bytecode& i) {
			memcpy(this, &i, sizeof(trigger_bytecode));
		}

		trigger_bytecode(trigger_code i) : identifier(i) {}
		trigger_bytecode(float i) : fp_value(i) {}
		trigger_bytecode(int32_t i) : integer_value(i) {}
		trigger_bytecode(bool i) : boolean_value(i) {}
		trigger_bytecode(technologies::invention_tag i) : invention(i) {}
		trigger_bytecode(governments::government_tag i) : government(i) {}
		trigger_bytecode(modifiers::national_modifier_tag i) : nat_mod(i) {}
		trigger_bytecode(modifiers::provincial_modifier_tag i) : prov_mod(i) {}
		trigger_bytecode(cultures::culture_tag i) : culture(i) {}
		trigger_bytecode(cultures::religion_tag i) : religion(i) {}
		trigger_bytecode(economy::goods_tag i) : good(i) {}
		trigger_bytecode(population::rebel_type_tag i) : rebel(i) {}
		trigger_bytecode(cultures::national_tag i) : tag(i) {}
		trigger_bytecode(variables::national_variable_tag i) : nat_var(i) {}
		trigger_bytecode(variables::global_variable_tag i) : global_var(i) {}
		trigger_bytecode(economy::factory_type_tag i) : factory(i) {}
		trigger_bytecode(provinces::state_tag i) : state(i) {}
		trigger_bytecode(text_data::text_tag i) : text(i) {}
		trigger_bytecode(ideologies::ideology_tag i) : ideology(i) {}
		trigger_bytecode(issues::issue_tag i) : issue(i) {}
		trigger_bytecode(issues::option_identifier i) : issue_option(i) {}
		trigger_bytecode(military::cb_type_tag i) : cb_type(i) {}
	};

	static_assert(sizeof(trigger_bytecode) == 4);

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
