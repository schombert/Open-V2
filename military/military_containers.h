#pragma once
#include "common\common.h"
#include "military.h"

namespace military {
	class military_state {
	public:
		army::container armies;
		war::container wars;
		fleet::container fleets;
		army_order::container army_orders;
		strategic_hq::container strategic_hqs;
		military_leader::container leaders;

		// stable_2d_vector<economy::goods_qnty_type, army_tag, economy::goods_tag, 1024, 16> army_supplies;
		// stable_2d_vector<uint16_t, army_tag, unit_type_tag, 1024, 16> unit_type_composition;
		// stable_2d_vector<economy::goods_qnty_type, fleet_tag, economy::goods_tag, 1024, 16> fleet_supplies;

		stable_variable_vector_storage_mk_2<leader_tag, 4, 8192> leader_arrays;
		// stable_variable_vector_storage_mk_2<ship, 2, 8192> ship_arrays;
		stable_variable_vector_storage_mk_2<army_tag, 4, 8192> army_arrays;
		stable_variable_vector_storage_mk_2<army_orders_tag, 4, 8192> orders_arrays;
		stable_variable_vector_storage_mk_2<fleet_tag, 4, 8192> fleet_arrays;
		stable_variable_vector_storage_mk_2<war_identifier, 1, 8192> war_arrays;
		stable_variable_vector_storage_mk_2<war_goal, 1, 8192> war_goal_arrays;
		stable_variable_vector_storage_mk_2<pending_cb, 1, 8192> cb_arrays;

		stable_variable_vector_storage_mk_2<fleet_presence, 4, 8192> fleet_presence_arrays;
		stable_variable_vector_storage_mk_2<naval_control, 32, 8192> naval_control_arrays;

		GET_SET(armies)
		GET_SET(wars)
		GET_SET(fleets)
		GET_SET(army_orders)
		GET_SET(strategic_hqs)
		GET_SET(leaders)

		ARRAY_BACKING_BASE(fleet_presence_arrays)
		ARRAY_BACKING_BASE(naval_control_arrays)
		ARRAY_BACKING_BASE(cb_arrays)
		ARRAY_BACKING_BASE(war_goal_arrays)
		ARRAY_BACKING_BASE(war_arrays)
		ARRAY_BACKING_BASE(fleet_arrays)
		ARRAY_BACKING_BASE(orders_arrays)
		ARRAY_BACKING_BASE(army_arrays)
		ARRAY_BACKING_BASE(leader_arrays)
	};

	class military_manager {
	public:
		tagged_vector<cb_type, cb_type_tag> cb_types;

		tagged_vector<text_data::text_tag, leader_trait_tag> leader_traits;
		tagged_fixed_blocked_2dvector<traits::value_type, leader_trait_tag, uint32_t, aligned_allocator_32<traits::value_type>> leader_trait_definitions;

		tagged_vector<leader_trait_tag, int32_t> personality_traits;
		tagged_vector<leader_trait_tag, int32_t> background_traits;
		constexpr static leader_trait_tag no_personality_trait = leader_trait_tag(0);
		constexpr static leader_trait_tag no_background_trait = leader_trait_tag(1);

		boost::container::flat_map<text_data::text_tag, cb_type_tag> named_cb_type_index;
		boost::container::flat_map<text_data::text_tag, leader_trait_tag> named_leader_trait_index;

		tagged_vector<float, cb_type_tag, padded_aligned_allocator_64<float>, true> cb_type_to_speed;

		GET_SET(cb_types)
		GET_SET_TV(leader_trait_name, leader_traits)
		GET_SET_TV(::military::personality_traits, personality_traits)
		GET_SET_TV(::military::background_traits, background_traits)
		GET_SET_TFV(leader_trait_values, leader_trait_definitions)
		GET_SET_TV(::cb_type::speed, cb_type_to_speed)
	};
}
