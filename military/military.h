#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"

namespace economy {
	class economic_scenario;
}
namespace sound {
	class sound_manager;
}
namespace events {
	struct event_creation_manager;
}
namespace scenario {
	class scenario_manager;
}
namespace military {
	struct cb_type {
		static constexpr uint32_t is_civil_war						= 0x00000001;
		static constexpr uint32_t always							= 0x00000002;
		static constexpr uint32_t is_not_triggered_only				= 0x00000004;
		static constexpr uint32_t is_not_constructing_cb			= 0x00000008;
		static constexpr uint32_t great_war_obligatory				= 0x00000010;
		static constexpr uint32_t all_allowed_states				= 0x00000020;
		static constexpr uint32_t not_in_crisis						= 0x00000040;
		static constexpr uint32_t po_clear_union_sphere				= 0x00000080;
		static constexpr uint32_t po_gunboat						= 0x00000100;
		static constexpr uint32_t po_annex							= 0x00000200;
		static constexpr uint32_t po_demand_state					= 0x00000400;
		static constexpr uint32_t po_add_to_sphere					= 0x00000800;
		static constexpr uint32_t po_disarmament					= 0x00001000;
		static constexpr uint32_t po_reparations					= 0x00002000;
		static constexpr uint32_t po_transfer_provinces				= 0x00004000;
		static constexpr uint32_t po_remove_prestige				= 0x00008000;
		static constexpr uint32_t po_make_puppet					= 0x00010000;
		static constexpr uint32_t po_release_puppet					= 0x00020000;
		static constexpr uint32_t po_status_quo						= 0x00040000;
		static constexpr uint32_t po_install_communist_gov_type		= 0x00080000;
		static constexpr uint32_t po_uninstall_communist_gov_type	= 0x00100000;
		static constexpr uint32_t po_remove_cores					= 0x00200000;
		static constexpr uint32_t po_colony							= 0x00400000;
		static constexpr uint32_t po_destroy_forts					= 0x00800000;
		static constexpr uint32_t po_destroy_naval_bases			= 0x01000000;
		static constexpr uint32_t po_liberate						= 0x02000000;
		
		uint32_t flags = 0;

		float badboy_factor = 1.0f;
		float prestige_factor = 1.0f;
		float peace_cost_factor = 1.0f;
		float penalty_factor = 1.0f;
		float break_truce_prestige_factor = 1.0f;
		float break_truce_infamy_factor = 1.0f;
		float break_truce_militancy_factor = 1.0f;
		float good_relation_prestige_factor = 0.0f;
		float good_relation_infamy_factor = 0.0f;
		float good_relation_militancy_factor = 0.0f;
		float construction_speed = 1.0f;
		float tws_battle_factor = 0.0f;

		text_data::text_tag name;
		text_data::text_tag war_name;
		
		triggers::trigger_tag allowed_states;
		triggers::trigger_tag allowed_states_in_crisis;
		triggers::trigger_tag allowed_substate_regions;
		triggers::trigger_tag allowed_countries;
		triggers::trigger_tag can_use;

		triggers::effect_tag on_add;
		triggers::effect_tag on_po_accepted;

		uint8_t sprite_index = 0ui8;
		uint8_t months = 12ui8;
		uint8_t truce_months = 60ui8;
		cb_type_tag id;
	};

	using unit_attribute_type = float;

	namespace unit_attribute {
		constexpr int32_t defense = 0;
		constexpr int32_t hull = 0;
		constexpr int32_t attack = 1;
		constexpr int32_t gun_power = 1;
		constexpr int32_t reconnaissance = 2;
		constexpr int32_t fire_range = 2;
		constexpr int32_t support = 3;
		constexpr int32_t torpedo_attack = 3;
		constexpr int32_t maneuver = 4;
		constexpr int32_t evasion = 4;
		constexpr int32_t speed = 5;
		constexpr int32_t organization = 6;
		constexpr int32_t build_time = 7;
		constexpr int32_t supply_consumption = 8;
		constexpr int32_t strength = 9;
		constexpr int32_t siege = 10;
		constexpr int32_t discipline = 11;
		constexpr int32_t enabled = 12;

		constexpr int32_t count = 13;
		constexpr static size_t aligned_32_size = ((sizeof(unit_attribute_type) * count + 31ui64) & ~31ui64) / sizeof(unit_attribute_type);
	}

	using unit_attribute_vector = Eigen::Matrix<unit_attribute_type, unit_attribute::aligned_32_size, 1>;

	struct alignas(32) unit_type {
		unit_attribute_vector base_attributes = unit_attribute_vector::Zero();

		static constexpr uint8_t primary_culture = 0x10;
		static constexpr uint8_t cant_build_overseas = 0x20;
		static constexpr uint8_t is_sail = 0x40;

		static constexpr uint8_t class_mask = 0x0F;

		static constexpr uint8_t class_infantry = 0x00;
		static constexpr uint8_t class_cavalry = 0x01;
		static constexpr uint8_t class_special = 0x02;
		static constexpr uint8_t class_support = 0x03;
		static constexpr uint8_t class_big_ship = 0x04;
		static constexpr uint8_t class_light_ship = 0x05;
		static constexpr uint8_t class_transport = 0x06;

		text_data::text_tag name; // 2 bytes
		
		sound::effect_tag select_sound; // 3 bytes
		sound::effect_tag move_sound; // 4 bytes

		int8_t limit_per_port = -1i8; // 5 bytes
		uint8_t supply_consumption_score = 0ui8; // 6 bytes
		uint8_t icon = 0ui8; // 7 bytes
		uint8_t naval_icon = 0ui8; // 8 bytes
		uint8_t colonial_points = 0ui8; // 9 bytes
		uint8_t min_port_level = 0ui8; // 10 bytes

		uint8_t flags = 0ui8; // 11 bytes

		unit_type_tag id; // 12 bytes

		unit_type() {
			base_attributes[unit_attribute::enabled] = unit_attribute_type(1);
		}
	};

	const size_t type_size = sizeof(unit_type);
	

	namespace traits {
		constexpr int32_t organisation = 0;
		constexpr int32_t morale = 1;
		constexpr int32_t attack = 2;
		constexpr int32_t defence = 3;
		constexpr int32_t reconnaissance = 4;
		constexpr int32_t speed = 5;
		constexpr int32_t experience = 6;
		constexpr int32_t reliability = 7;

		constexpr int32_t trait_count = 8;

		using value_type = float;
	}

	struct ship {
		float hull = 1.0f;
		float org = 1.0f;
		provinces::province_tag location;
		unit_type_tag type;
	};

	struct military_leader {
		traits::value_type leader_traits[traits::trait_count] = { traits::value_type(0) };
		vector_backed_string<char16_t> first_name;
		vector_backed_string<char16_t> last_name;
		date_tag creation_date;
		graphics::texture_tag portrait;
		leader_tag id;
		leader_trait_tag personality;
		leader_trait_tag background;
	};

	struct army {
		military_leader* leader = nullptr;
		void* current_orders = nullptr; // unknown type
		float org = 1.0f;
		uint32_t total_soldiers = 0ui32;
		date_tag locked_date; // cannot be rebased until date

		array_tag<population::pop_tag> backing_pops;
		
		//extern: unit type composition
		//extern: supplies

		army_tag id;
		provinces::province_tag base;
	};

	struct fleet {
		military_leader* leader = nullptr;
		void* current_orders = nullptr; // unknown type
		date_tag locked_date; // cannot be rebased until date

		array_tag<ship> ships;

		//extern: supplies

		fleet_tag id;
		provinces::province_tag base;
	};

	class military_state {
	public:
		stable_vector<military_leader, leader_tag, 1024, 16> leaders;
		stable_vector<army, army_tag, 1024, 16> armies;
		stable_vector<fleet, fleet_tag, 1024, 16> fleets;

		stable_2d_vector<economy::goods_qnty_type, army_tag, economy::goods_tag, 1024, 16> army_supplies;
		stable_2d_vector<uint16_t, army_tag, unit_type_tag, 1024, 16> unit_type_composition;
		stable_2d_vector<economy::goods_qnty_type, army_tag, economy::goods_tag, 1024, 16> fleet_supplies;

		stable_variable_vector_storage_mk_2<leader_tag, 4, 8192> leader_arrays;
		stable_variable_vector_storage_mk_2<ship, 2, 8192> ship_arrays;
		stable_variable_vector_storage_mk_2<army_tag, 4, 8192> army_arrays;
		stable_variable_vector_storage_mk_2<fleet_tag, 4, 8192> fleet_arrays;

	};

	class military_manager {
	public:
		tagged_vector<cb_type, cb_type_tag> cb_types;
		tagged_vector<unit_type, unit_type_tag> unit_types;

		tagged_vector<text_data::text_tag, leader_trait_tag> leader_traits;
		tagged_fixed_blocked_2dvector<traits::value_type, leader_trait_tag, uint32_t, aligned_allocator_32<traits::value_type>> leader_trait_definitions;

		std::vector<leader_trait_tag> personality_traits;
		std::vector<leader_trait_tag> background_traits;
		constexpr static leader_trait_tag no_personality_trait = leader_trait_tag(0);
		constexpr static leader_trait_tag no_background_trait = leader_trait_tag(1);

		boost::container::flat_map<text_data::text_tag, unit_type_tag> named_unit_type_index;
		boost::container::flat_map<text_data::text_tag, cb_type_tag> named_cb_type_index;
		boost::container::flat_map<text_data::text_tag, leader_trait_tag> named_leader_trait_index;

		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, unit_type_tag, economy::goods_tag, aligned_allocator_32<economy::goods_qnty_type>> unit_build_costs;
		tagged_fixed_blocked_2dvector<economy::goods_qnty_type, unit_type_tag, economy::goods_tag, aligned_allocator_32<economy::goods_qnty_type>> unit_base_supply_costs;
	};
}
