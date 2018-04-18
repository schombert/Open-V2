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
				issues::option_tag option;

				inner_u(governments::government_tag i) : government(i) {}
				inner_u(cultures::religion_tag i) : religion(i) {}
				inner_u(economy::goods_tag i) : good(i) {}
				inner_u(population::rebel_type_tag i) : rebel(i) {}
				inner_u(economy::factory_type_tag i) : factory(i) {}
				inner_u(ideologies::ideology_tag i) : ideology(i) {}
				inner_u(issues::issue_tag i) : issue(i) {}
				inner_u(military::cb_type_tag i) : cb_type(i) {}
				inner_u(population::pop_type_tag i) : pop_type(i) {}
				inner_u(issues::option_tag i) : option(i) {}
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
			small_s(issues::option_tag i) : values(i), padding(0ui8) {}
		} small;
		struct generic_issue_s {
			union inner_u_b {
				issues::issue_tag civ_issue;
				issues::unciv_issue_tag unciv_issue;
				inner_u_b() : civ_issue() {}
			} values;
			issues::issue_group group;
			generic_issue_s(issues::issue_idenfitier i) : group(i.type) {
				if (std::holds_alternative<issues::issue_tag>(i.id))
					values.civ_issue = std::get<issues::issue_tag>(i.id);
				else if (std::holds_alternative<issues::unciv_issue_tag>(i.id))
					values.unciv_issue = std::get<issues::unciv_issue_tag>(i.id);
			}
		} generic_issue;

		uint16_t value;
		int16_t signed_value;
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
		technologies::tech_tag tech;

		trigger_payload(const trigger_payload &i) noexcept : value(i.value) {}
		trigger_payload(uint16_t i) : value(i) {}
		trigger_payload(int16_t i) : signed_value(i) {}
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
		trigger_payload(technologies::tech_tag i) : tech(i) {}
		trigger_payload(economy::factory_type_tag i) : small(i) {}
		trigger_payload(provinces::state_tag i) : state(i) {}
		trigger_payload(text_data::text_tag i) : text(i) {}
		trigger_payload(ideologies::ideology_tag i) : small(i) {}
		trigger_payload(issues::issue_tag i) : small(i) {}
		trigger_payload(military::cb_type_tag i) : small(i) {}
		trigger_payload(population::pop_type_tag i) : small(i) {}
		trigger_payload(issues::option_tag i) : small(i) {}
		trigger_payload(issues::issue_idenfitier id) : generic_issue(id) {}
	};

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
		std::vector<trigger_payload> trigger_data;
		tagged_vector<uint32_t, trigger_tag> trigger_offsets;
	};

	int32_t get_payload_size(const uint16_t* data);
	void add_float_to_payload(std::vector<uint16_t>& v, float f);
	float read_float_from_payload(const uint16_t* data);
	void add_int32_t_to_payload(std::vector<uint16_t>& v, int32_t i);
	int32_t read_int32_t_from_payload(const uint16_t* data);
	void add_option_identifier_to_payload(std::vector<uint16_t>& v, issues::option_identifier i);
	issues::option_identifier read_option_identifier_from_payload(const uint16_t* data);
}
