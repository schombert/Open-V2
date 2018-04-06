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
#include "issues\\issues.h"
#include <variant>

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

	union trigger_payload {
		struct small_s {
			union inner_u {
				governments::government_tag government;
				cultures::religion_tag religion;
				economy::goods_tag good;
				population::rebel_type_tag rebel;
				economy::factory_type_tag factory;
				ideologies::ideology_tag ideology;
				issues::issue_tag issue;
				military::cb_type_tag cb_type;
				population::pop_type_tag pop_type;

				inner_u(governments::government_tag i) : government(i) {}
				inner_u(cultures::religion_tag i) : religion(i) {}
				inner_u(economy::goods_tag i) : good(i) {}
				inner_u(population::rebel_type_tag i) : rebel(i) {}
				inner_u(economy::factory_type_tag i) : factory(i) {}
				inner_u(ideologies::ideology_tag i) : ideology(i) {}
				inner_u(issues::issue_tag i) : issue(i) {}
				inner_u(military::cb_type_tag i) : cb_type(i) {}
				inner_u(population::pop_type_tag i) : pop_type(i) {}
			} values;
			uint8_t padding;

			small_s(governments::government_tag i) : values(i), padding(0ui8) {}
			small_s(cultures::religion_tag i) : values(i), padding(0ui8) {}
			small_s(economy::goods_tag i) : values(i), padding(0ui8) {}
			small_s(population::rebel_type_tag i) : values(i), padding(0ui8) {}
			small_s(economy::factory_type_tag i) : values(i), padding(0ui8) {}
			small_s(ideologies::ideology_tag i) : values(i), padding(0ui8) {}
			small_s(issues::issue_tag i) : values(i), padding(0ui8) {}
			small_s(military::cb_type_tag i) : values(i), padding(0ui8) {}
			small_s(population::pop_type_tag i) : values(i), padding(0ui8) {}
		} small;

		uint16_t value;
		bool boolean_value;
		technologies::invention_tag invention;
		
		modifiers::national_modifier_tag nat_mod;
		modifiers::provincial_modifier_tag prov_mod;
		cultures::culture_tag culture;
		cultures::national_tag tag;
		variables::national_variable_tag nat_var;
		variables::global_variable_tag global_var;
		provinces::state_tag state;
		text_data::text_tag text;

		trigger_payload(uint16_t i) : value(i) {}
		trigger_payload(bool i) : boolean_value(i) {}
		trigger_payload(technologies::invention_tag i) : invention(i) {}
		trigger_payload(governments::government_tag i) : small(i) {}
		trigger_payload(modifiers::national_modifier_tag i) : nat_mod(i) {}
		trigger_payload(modifiers::provincial_modifier_tag i) : prov_mod(i) {}
		trigger_payload(cultures::culture_tag i) : culture(i) {}
		trigger_payload(cultures::religion_tag i) : small(i) {}
		trigger_payload(economy::goods_tag i) : small(i) {}
		trigger_payload(population::rebel_type_tag i) : small(i) {}
		trigger_payload(cultures::national_tag i) : tag(i) {}
		trigger_payload(variables::national_variable_tag i) : nat_var(i) {}
		trigger_payload(variables::global_variable_tag i) : global_var(i) {}
		trigger_payload(economy::factory_type_tag i) : small(i) {}
		trigger_payload(provinces::state_tag i) : state(i) {}
		trigger_payload(text_data::text_tag i) : text(i) {}
		trigger_payload(ideologies::ideology_tag i) : small(i) {}
		trigger_payload(issues::issue_tag i) : small(i) {}
		trigger_payload(military::cb_type_tag i) : small(i) {}
		trigger_payload(population::pop_type_tag i) : small(i) {}
	};

	using trigger_bytecode = std::variant<std::monostate, int32_t, float, issues::option_identifier, trigger_payload>;

	static_assert(sizeof(trigger_payload) == 2);

	enum class trigger_slot_contents {
		empty, province, state, pop, nation
	};

	struct trigger_scope_state {
		trigger_slot_contents main_slot = trigger_slot_contents::empty;
		trigger_slot_contents this_slot = trigger_slot_contents::empty;
		trigger_slot_contents from_slot = trigger_slot_contents::empty;
		bool contains_rebeltype = false;
	};

	class trigger_manager {
	public:
		std::vector<trigger_bytecode> trigger_data;
		tagged_vector<uint32_t, trigger_tag> trigger_offsets;
	};
}
