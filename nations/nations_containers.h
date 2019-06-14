#pragma once
#include "nations.h"
#include "nations_io.h"
#include "state.h"
#include "nation.h"

namespace nations {
	class nations_state {
	public:
		array_tag<country_tag, int32_t, false> nations_by_rank;

		nation::container nations;
		state::container states;

		varying_vectorizable_2d_array<state_tag, economy::goods_tag, float, state::container_size> state_production;
		varying_vectorizable_2d_array<state_tag, economy::goods_tag, float, state::container_size> state_global_demand;
		fixed_vectorizable_2d_array<nations::country_tag, float, nation::container_size, modifiers::national_offsets::count> modifier_values;
		fixed_vectorizable_2d_array<nations::country_tag, float, nation::container_size, technologies::tech_offset::count> tech_attributes;

		stable_2d_vector<float, nations::country_tag, population::rebel_type_tag, 512, 16> local_rebel_support;
		stable_2d_vector<float, nations::country_tag, issues::option_tag, 512, 16> local_movement_support;
		stable_2d_vector<float, nations::country_tag, issues::option_tag, 512, 16> local_movement_radicalism;
		stable_2d_vector<float, nations::country_tag, issues::option_tag, 512, 16> local_movement_radicalism_cache;

		varying_vectorizable_2d_array<nations::country_tag, technologies::tech_tag, bitfield_type, nation::container_size> active_technologies;
		varying_vectorizable_2d_array<nations::country_tag, issues::issue_tag, issues::option_tag, nation::container_size> active_issue_options;


		stable_2d_vector<governments::party_tag, country_tag, ideologies::ideology_tag, 512, 16> active_parties;
		stable_2d_vector<uint8_t, country_tag, ideologies::ideology_tag, 512, 16> upper_house;
		//stable_2d_vector<uint64_t, country_tag, technologies::tech_tag, 512, 16> active_technologies;
		stable_2d_vector<bitfield_type, country_tag, economy::goods_tag, 512, 16> active_goods;
		stable_2d_vector<economy::money_qnty_type, country_tag, economy::goods_tag, 512, 16> collected_tariffs;
		//stable_2d_vector<issues::option_tag, country_tag, issues::issue_tag, 512, 16> active_issue_options;
		stable_2d_vector<economy::goods_qnty_type, country_tag, economy::goods_tag, 512, 16> national_stockpiles;
		stable_2d_vector<float, country_tag, variables::national_variable_tag, 512, 16> national_variables;
		stable_2d_vector<float, country_tag, technologies::adjusted_goods_tag, 512, 16> production_adjustments;
		//stable_2d_vector<military::unit_attribute_vector, country_tag, military::unit_type_tag, 512, 16> unit_stats;
		stable_2d_vector<float, country_tag, population::rebel_type_tag, 512, 16> rebel_org_gain;

		stable_2d_vector<economy::money_qnty_type, state_tag, economy::goods_tag, 512, 16> state_prices;
		stable_2d_vector<economy::money_qnty_type, state_tag, economy::goods_tag, 512, 16> state_price_delta;
		stable_2d_vector<economy::money_qnty_type, state_tag, economy::goods_tag, 512, 16> state_demand;
		stable_2d_vector<array_tag<economy::money_qnty_type, nations::state_tag, true>, state_tag, economy::goods_tag, 512, 16> state_purchases;


		stable_variable_vector_storage_mk_2<modifiers::national_modifier_tag, 4, 8192> static_modifier_arrays;
		stable_variable_vector_storage_mk_2<timed_national_modifier, 4, 8192> timed_modifier_arrays;
		stable_variable_vector_storage_mk_2<region_state_pair, 2, 8192> state_arrays;
		stable_variable_vector_storage_mk_2<influence, 2, 8192> influence_arrays;
		stable_variable_vector_storage_mk_2<country_tag, 4, 8192> nations_arrays;
		stable_variable_vector_storage_mk_2<state_tag, 4, 8192> state_tag_arrays;
		stable_variable_vector_storage_mk_2<relationship, 4, 8192> relations_arrays;
		stable_variable_vector_storage_mk_2<truce, 4, 8192> truce_arrays;

		stable_2d_vector<float, state_tag, population::demo_tag, 512, 16> state_demographics;
		stable_2d_vector<float, country_tag, population::demo_tag, 512, 16> nation_demographics;
		stable_2d_vector<float, country_tag, population::demo_tag, 512, 16> nation_colonial_demographics;
	};
}

template<>
class serialization::serializer<nations::nations_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, nations::nations_state const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, nations::nations_state& obj, world_state& ws);
	static size_t size(nations::nations_state const& obj, world_state const& ws);
};
