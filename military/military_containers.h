#pragma once
#include "common\common.h"
#include "military.h"
#include "military_io.h"
#include "army.h"
#include "army_order.h"
#include "fleet.h"
#include "military_leader.h"
#include "strategic_hq.h"
#include "war.h"
#include "border_information.h"
#include "nations\nations_containers.h"
#include "provinces\province_containers.h"

namespace military {
	class military_state {
	public:
		army::container armies;
		war::container wars;
		fleet::container fleets;
		army_order::container army_orders;
		strategic_hq::container strategic_hqs;
		military_leader::container leaders;
		border_information::container borders;

		// stable_2d_vector<economy::goods_qnty_type, army_tag, economy::goods_tag, 1024, 16> army_supplies;
		// stable_2d_vector<uint16_t, army_tag, unit_type_tag, 1024, 16> unit_type_composition;
		// stable_2d_vector<economy::goods_qnty_type, fleet_tag, economy::goods_tag, 1024, 16> fleet_supplies;

		stable_variable_vector_storage_mk_2<leader_tag, 4, 8192> leader_arrays;
		// stable_variable_vector_storage_mk_2<ship, 2, 8192> ship_arrays;
		stable_variable_vector_storage_mk_2<strategic_hq_tag, 2, 8192> hq_arrays;
		stable_variable_vector_storage_mk_2<army_tag, 4, 8192> army_arrays;
		stable_variable_vector_storage_mk_2<army_orders_tag, 4, 8192> orders_arrays;
		stable_variable_vector_storage_mk_2<fleet_tag, 4, 8192> fleet_arrays;
		stable_variable_vector_storage_mk_2<war_identifier, 1, 8192> war_arrays;
		stable_variable_vector_storage_mk_2<war_goal, 1, 8192> war_goal_arrays;
		stable_variable_vector_storage_mk_2<pending_cb, 1, 8192> cb_arrays;

		stable_variable_vector_storage_mk_2<fleet_presence, 4, 8192> fleet_presence_arrays;
		stable_variable_vector_storage_mk_2<naval_control, 32, 8192> naval_control_arrays;

		stable_variable_vector_storage_mk_2<hq_commitment_information, 1, 8192> hq_commitment_arrays;

		varying_vectorizable_2d_array<nations::country_tag, nations::country_tag, uint16_t, nation::max_count> border_sizes;
		varying_vectorizable_2d_array<nations::country_tag, nations::country_tag, provinces::province_tag, nation::max_count> border_representative_provinces;
		varying_vectorizable_2d_array<nations::country_tag, nations::country_tag, military::border_information_tag, nation::max_count> border_info_tags;
	};

	class military_manager {
	public:
		tagged_vector<cb_type, cb_type_tag> cb_types;

		tagged_vector<text_data::text_tag, leader_trait_tag> leader_traits;
		tagged_fixed_blocked_2dvector<traits::value_type, leader_trait_tag, uint32_t, aligned_allocator_32<traits::value_type>> leader_trait_definitions;

		tagged_vector<leader_trait_tag, int32_t> personality_traits;
		tagged_vector<leader_trait_tag, int32_t> background_traits;

		boost::container::flat_map<text_data::text_tag, cb_type_tag> named_cb_type_index;
		boost::container::flat_map<text_data::text_tag, leader_trait_tag> named_leader_trait_index;

		tagged_vector<float, cb_type_tag, padded_aligned_allocator_64<float>, true> cb_type_to_speed;
	};
}

template<>
class serialization::serializer<military::military_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, military::military_state const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, military::military_state& obj, world_state& ws);
	static size_t size(military::military_state const& obj, world_state const& ws);
};

template<>
class serialization::serializer<military::military_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(military::military_manager& obj);

	static void serialize_object(std::byte* &output, military::military_manager const& obj);
	static void deserialize_object(std::byte const* &input, military::military_manager& obj);
	static void deserialize_object(std::byte const* &input, military::military_manager& obj, concurrency::task_group& tg);
	static size_t size(military::military_manager const& obj);
	template<typename T>
	static size_t size(military::military_manager const& obj, T const&) {
		return size(obj);
	}
};
