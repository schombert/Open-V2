#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "simple_fs\\simple_fs.h"
#include "common\\shared_tags.h"
#include <vector>
#include "text_data\\text_data.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "issues\\issues.h"
#include "concurrency_tools\\ve.h"
#include "concurrency_tools/ve_avx2.h"
#include "codes.h"

namespace scenario {
	class scenario_manager;
}

class world_state;

#undef small

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
				issues::option_tag option;
				military::cb_type_tag cb_type;
				population::pop_type_tag pop_type;
				military::leader_trait_tag leader_trait;
				military::unit_type_tag unit_type;

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
				inner_u(military::leader_trait_tag i) : leader_trait(i) {}
				inner_u(military::unit_type_tag i) : unit_type(i) {}
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
			small_s(military::leader_trait_tag i) : values(i), padding(0ui8) {}
			small_s(military::unit_type_tag i) : values(i), padding(0ui8) {}
		} small;

		uint16_t value;
		int16_t signed_value;
		bool boolean_value;
		
		modifiers::national_modifier_tag nat_mod;
		modifiers::provincial_modifier_tag prov_mod;
		cultures::culture_tag culture;
		cultures::culture_group_tag culture_group;
		cultures::national_tag tag;
		variables::national_variable_tag nat_var;
		variables::national_flag_tag nat_flag;
		variables::global_variable_tag global_var;
		provinces::state_tag state;
		provinces::province_tag prov;
		text_data::text_tag text;
		technologies::tech_tag tech;
		events::event_tag event;
		trigger_tag trigger;

		trigger_payload(const trigger_payload &i) noexcept : value(i.value) {}
		trigger_payload(uint16_t i) : value(i) {}
		trigger_payload(int16_t i) : signed_value(i) {}
		trigger_payload(bool i) : boolean_value(i) {}
		trigger_payload(governments::government_tag i) : small(i) {}
		trigger_payload(modifiers::national_modifier_tag i) : nat_mod(i) {}
		trigger_payload(modifiers::provincial_modifier_tag i) : prov_mod(i) {}
		trigger_payload(cultures::culture_tag i) : culture(i) {}
		trigger_payload(cultures::culture_group_tag i) : culture_group(i) {}
		trigger_payload(cultures::religion_tag i) : small(i) {}
		trigger_payload(economy::goods_tag i) : small(i) {}
		trigger_payload(population::rebel_type_tag i) : small(i) {}
		trigger_payload(cultures::national_tag i) : tag(i) {}
		trigger_payload(variables::national_variable_tag i) : nat_var(i) {}
		trigger_payload(variables::national_flag_tag i) : nat_flag(i) {}
		trigger_payload(variables::global_variable_tag i) : global_var(i) {}
		trigger_payload(technologies::tech_tag i) : tech(i) {}
		trigger_payload(economy::factory_type_tag i) : small(i) {}
		trigger_payload(provinces::state_tag i) : state(i) {}
		trigger_payload(provinces::province_tag i) : prov(i) {}
		trigger_payload(text_data::text_tag i) : text(i) {}
		trigger_payload(ideologies::ideology_tag i) : small(i) {}
		trigger_payload(issues::issue_tag i) : small(i) {}
		trigger_payload(military::cb_type_tag i) : small(i) {}
		trigger_payload(population::pop_type_tag i) : small(i) {}
		trigger_payload(issues::option_tag i) : small(i) {}
		trigger_payload(military::leader_trait_tag i) : small(i) {}
		trigger_payload(military::unit_type_tag i) : small(i) {}
		trigger_payload(events::event_tag id) : event(id) {}
		trigger_payload(trigger_tag id) : trigger(id) {}
	};

	/*union parameter {
		nations::country_tag nation;
		nations::state_tag state;
		population::pop_tag pop;
		provinces::province_tag prov;
		population::rebel_faction_tag rebel;
		int32_t value;

		constexpr parameter() noexcept : nation() {}
		constexpr parameter(nullptr_t) noexcept : nation() {}
		constexpr parameter(nations::country_tag n) noexcept : nation(n) {}
		constexpr parameter(nations::state_tag s) noexcept : state(s) {}
		constexpr parameter(population::pop_tag po) noexcept : pop(po) {}
		constexpr parameter(provinces::province_tag pr) noexcept : prov(pr) {}
		constexpr parameter(population::rebel_faction_tag r) noexcept : rebel(r) {}
		constexpr parameter(expanded_tag<nations::country_tag> n) noexcept : nation(n) {}
		constexpr parameter(expanded_tag<nations::state_tag> s) noexcept : state(s) {}
		constexpr parameter(expanded_tag<population::pop_tag> po) noexcept : pop(po) {}
		constexpr parameter(expanded_tag<provinces::province_tag> pr) noexcept : prov(pr) {}
		constexpr parameter(expanded_tag < population::rebel_faction_tag> r) noexcept : rebel(r) {}
		constexpr parameter(int32_t v) noexcept : value(v) {}
	};*/

	


	static_assert(sizeof(trigger_payload) == 2);

	class trigger_manager {
	public:
		std::vector<uint16_t> trigger_data;
		std::vector<uint16_t> effect_data;
		trigger_manager() {
			trigger_data.push_back(0ui16);
			effect_data.push_back(0ui16);
		}
	};

	int32_t get_trigger_payload_size(const uint16_t* data);
	void add_float_to_payload(std::vector<uint16_t>& v, float f);
	float read_float_from_payload(const uint16_t* data);
	void add_int32_t_to_payload(std::vector<uint16_t>& v, int32_t i);
	int32_t read_int32_t_from_payload(const uint16_t* data);
	int32_t trigger_scope_data_payload(uint16_t code);

	template<typename T>
	void recurse_over_triggers(uint16_t* source, const T& f) {
		f(source);

		if ((source[0] & trigger_codes::is_scope) != 0) {
			const auto source_size = 1 + get_trigger_payload_size(source);

			auto sub_units_start = source + 2 + trigger_scope_data_payload(source[0]);
			while (sub_units_start < source + source_size) {
				recurse_over_triggers(sub_units_start, f);
				sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
			}
		}
	}

	template<typename T>
	void recurse_over_triggers(uint16_t const* source, const T& f) {
		f(source);

		if((source[0] & trigger_codes::is_scope) != 0) {
			const auto source_size = 1 + get_trigger_payload_size(source);

			auto sub_units_start = source + 2 + trigger_scope_data_payload(source[0]);
			while(sub_units_start < source + source_size) {
				recurse_over_triggers(sub_units_start, f);
				sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
			}
		}
	}

	bool test_trigger(uint16_t const* tval, world_state const& ws, const_parameter primary_slot, const_parameter this_slot, const_parameter from_slot);
	ve::mask_vector test_contiguous_trigger(uint16_t const* tval, world_state const& ws, ve::contiguous_tags_base<union_tag> primary_offset, ve::contiguous_tags_base<union_tag> this_offset, ve::contiguous_tags_base<union_tag> from_offset);
	ve::mask_vector test_semi_contiguous_trigger(uint16_t const* tval, world_state const& ws, ve::contiguous_tags_base<union_tag> primary_offset, const_parameter this_slot, const_parameter from_slot);
}
