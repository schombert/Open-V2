#pragma once
#include <stdint.h>
#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "concurrency_tools\variable_layout.h"

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

namespace cb_type {
	struct flags;
	struct badboy_factor;
	struct prestige_factor;
	struct peace_cost_factor;
	struct penalty_factor;
	struct break_truce_prestige_factor;
	struct break_truce_infamy_factor;
	struct break_truce_militancy_factor;
	struct good_relation_prestige_factor;
	struct good_relation_infamy_factor;
	struct good_relation_militancy_factor;
	struct construction_speed;
	struct tws_battle_factor;

	struct name;
	struct explanation;
	struct war_name;

	struct allowed_states;
	struct allowed_states_in_crisis;
	struct allowed_substate_regions;
	struct allowed_countries;
	struct can_use;

	struct on_add;
	struct on_po_accepted;

	struct sprite_index;
	struct months;
	struct truce_months;
	struct id;

	struct construction_speed_direct;
}

namespace military {
	struct cb_type {
		static constexpr uint32_t is_civil_war = 0x00000001;
		static constexpr uint32_t always = 0x00000002;
		static constexpr uint32_t is_not_triggered_only = 0x00000004;
		static constexpr uint32_t is_not_constructing_cb = 0x00000008;
		static constexpr uint32_t great_war_obligatory = 0x00000010;
		static constexpr uint32_t all_allowed_states = 0x00000020;
		static constexpr uint32_t not_in_crisis = 0x00000040;
		static constexpr uint32_t po_clear_union_sphere = 0x00000080;
		static constexpr uint32_t po_gunboat = 0x00000100;
		static constexpr uint32_t po_annex = 0x00000200;
		static constexpr uint32_t po_demand_state = 0x00000400;
		static constexpr uint32_t po_add_to_sphere = 0x00000800;
		static constexpr uint32_t po_disarmament = 0x00001000;
		static constexpr uint32_t po_reparations = 0x00002000;
		static constexpr uint32_t po_transfer_provinces = 0x00004000;
		static constexpr uint32_t po_remove_prestige = 0x00008000;
		static constexpr uint32_t po_make_puppet = 0x00010000;
		static constexpr uint32_t po_release_puppet = 0x00020000;
		static constexpr uint32_t po_status_quo = 0x00040000;
		static constexpr uint32_t po_install_communist_gov_type = 0x00080000;
		static constexpr uint32_t po_uninstall_communist_gov_type = 0x00100000;
		static constexpr uint32_t po_remove_cores = 0x00200000;
		static constexpr uint32_t po_colony = 0x00400000;
		static constexpr uint32_t po_destroy_forts = 0x00800000;
		static constexpr uint32_t po_destroy_naval_bases = 0x01000000;
		static constexpr uint32_t po_liberate = 0x02000000;
		static constexpr uint32_t po_take_from_sphere = 0x04000000;

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
		text_data::text_tag explanation;
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
}

START_STRUCT(military::cb_type)
STRUCT_MEMBER(::cb_type::flags, flags)
STRUCT_MEMBER(::cb_type::badboy_factor, badboy_factor)
STRUCT_MEMBER(::cb_type::prestige_factor, prestige_factor)
STRUCT_MEMBER(::cb_type::peace_cost_factor, peace_cost_factor)
STRUCT_MEMBER(::cb_type::penalty_factor, penalty_factor)
STRUCT_MEMBER(::cb_type::break_truce_prestige_factor, break_truce_prestige_factor)
STRUCT_MEMBER(::cb_type::break_truce_infamy_factor, break_truce_infamy_factor)
STRUCT_MEMBER(::cb_type::break_truce_militancy_factor, break_truce_militancy_factor)
STRUCT_MEMBER(::cb_type::good_relation_prestige_factor, good_relation_prestige_factor)
STRUCT_MEMBER(::cb_type::good_relation_infamy_factor, good_relation_infamy_factor)
STRUCT_MEMBER(::cb_type::good_relation_militancy_factor, good_relation_militancy_factor)
STRUCT_MEMBER(::cb_type::construction_speed, construction_speed)
STRUCT_MEMBER(::cb_type::tws_battle_factor, tws_battle_factor)
STRUCT_MEMBER(::cb_type::name, name)
STRUCT_MEMBER(::cb_type::explanation, explanation)
STRUCT_MEMBER(::cb_type::war_name, war_name)
STRUCT_MEMBER(::cb_type::allowed_states, allowed_states)
STRUCT_MEMBER(::cb_type::allowed_states_in_crisis, allowed_states_in_crisis)
STRUCT_MEMBER(::cb_type::allowed_substate_regions, allowed_substate_regions)
STRUCT_MEMBER(::cb_type::allowed_countries, allowed_countries)
STRUCT_MEMBER(::cb_type::can_use, can_use)
STRUCT_MEMBER(::cb_type::on_add, on_add)
STRUCT_MEMBER(::cb_type::on_po_accepted, on_po_accepted)
STRUCT_MEMBER(::cb_type::sprite_index, sprite_index)
STRUCT_MEMBER(::cb_type::months, months)
STRUCT_MEMBER(::cb_type::truce_months, truce_months)
STRUCT_MEMBER(::cb_type::id, id)
END_STRUCT

namespace military {

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

	enum class army_orders_type : uint8_t {
		garrison = 0,
		defend = 1,
		attack = 2,
		naval_invasion = 3
	};

	struct fleet_presence {
		float patrol_chance = 0.0f;
		fleet_tag tag;
		nations::country_tag owner;

		bool operator<(fleet_presence const& other)  const noexcept { return tag < other.tag; }
		bool operator==(fleet_presence const& other) const noexcept { return tag == other.tag; }
	};

	struct naval_control {
		float attacker_control = 0.0f; //from 1.0 = attacker has total control to -1.0 = defender has total control
		provinces::province_tag location;

		bool operator<(naval_control const& other)  const noexcept { return location < other.location; }
		bool operator==(naval_control const& other) const noexcept { return location == other.location; }
	};

	struct war_goal {
		date_tag date_added;
		float ticking_war_score = 0.0f;

		nations::country_tag from_country;

		nations::state_tag target_state;
		nations::country_tag target_country;
		nations::country_tag liberation_target;

		cb_type_tag cb_type;

		bool operator==(war_goal const& other) const noexcept {
			return (from_country == other.from_country) &
				(target_state == other.target_state) &
				(liberation_target == other.liberation_target) &
				(cb_type == other.cb_type);
		}
	};
};

namespace army_order {
	struct type;
	struct province_set;
	struct army_set;
	struct target;
	struct leader;
		
	constexpr int32_t container_size = 4000;

	/*
	using container = variable_layout_tagged_vector < military::army_orders_tag, container_size,
		type, military::army_orders_type,
		province_set, set_tag<provinces::province_tag>,
		army_set, set_tag<military::army_tag>,
		target, provinces::province_tag,
		leader, military::leader_tag
		> ;
		*/
}

namespace military_leader {
	struct first_name;
	struct last_name;
	struct creation_date;
	struct portrait;
	struct personality;
	struct background;

	struct organisation;
	struct morale;
	struct attack;
	struct defence;
	struct reconnaissance;
	struct speed;
	struct experience;
	struct reliability;

	struct is_attached;
	struct is_general;

	constexpr int32_t container_size = 6000;

	/*
	using container = variable_layout_tagged_vector< military::leader_tag, container_size,
		first_name, vector_backed_string<char16_t>,
		last_name, vector_backed_string<char16_t>,
		creation_date, date_tag,
		portrait, graphics::texture_tag,
		personality, military::leader_trait_tag,
		background, military::leader_trait_tag,
		organisation, float,
		morale, float,
		attack, float,
		defence, float,
		reconnaissance, float,
		speed, float,
		experience, float,
		reliability, float,
		is_attached, bitfield_type
	>;*/
}

namespace army {
	struct leader;
	struct hq;
	struct location;
	struct order;
	struct current_soldiers;
	struct target_soldiers;
	struct readiness;
	struct supply;
	struct priority;
	struct composition;
	struct arrival_time;
	struct owner;

	constexpr int32_t container_size = 6000;

	/*using container = variable_layout_tagged_vector< military::army_tag, container_size,
		leader, military::leader_tag,
		hq, military::strategic_hq_tag,
		order, military::army_orders_tag,
		location, provinces::province_tag,
		owner, nations::country_tag,
		current_soldiers, float,
		target_soldiers, float,
		readiness, float,
		supply, float,
		priority, int8_t,
		composition, military::army_composition_tag,
		arrival_time, date_tag
	>;*/
}

namespace fleet {
	struct leader;
	struct location;
	struct order;
	struct supply;
	struct readiness;
	struct size;
	struct arrival_time;

	constexpr int32_t container_size = 6000;

	/*using container = variable_layout_tagged_vector< military::fleet_tag, container_size,
		leader, military::leader_tag,
		location, provinces::province_tag,
		supply, float,
		readiness, float,
		size, float,
		arrival_time, date_tag
	>;*/
}

namespace strategic_hq {
	struct leader;
	struct location;
	struct province_set;
	struct injured_soldiers;
	struct pow_soldiers;
	struct reserve_soldiers;
	struct allocated_soldiers;
	struct total_soldier_pops;
	struct total_non_soldier_pops;
	struct mobilization_level;
	struct army_set;
	struct doctrine;

	constexpr int32_t container_size = 4000;

	/*using container = variable_layout_tagged_vector < military::strategic_hq_tag, container_size,
		leader, military::leader_tag,
		location, provinces::province_tag,
		province_set, set_tag<provinces::province_tag>,
		army_set, set_tag<military::army_tag>,
		injured_soldiers, float,
		pow_soldiers, float,
		reserve_soldiers, float,
		allocated_soldiers, float,
		total_soldier_pops, float,
		total_non_soldier_pops, float,
		mobilization_level, int8_t
	>;*/
}

namespace war {
	struct is_great_war;
	struct is_world_war;

	struct attackers;
	struct defenders;
	struct naval_control_set;
	struct start_date;
	struct current_war_score;
	struct name;
	struct first_adj;
	struct second;
	struct state_name;

	struct primary_attacker;
	struct primary_defender;

	struct war_goals;
	struct flags;

	constexpr int32_t container_size = 500;

	/*using container = variable_layout_tagged_vector < military::war_tag, container_size,
		attackers, set_tag<nations::country_tag>,
		defenders, set_tag<nations::country_tag>,
		naval_control_set, set_tag<military::naval_control>,
		start_date, date_tag,
		current_war_score, float,
		name, text_data::text_tag,
		first_adj, text_data::text_tag,
		second, text_data::text_tag,
		state_name, text_data::text_tag,
		primary_attacker, nations::country_tag,
		primary_defender, nations::country_tag,
		war_goals, array_tag<military::war_goal, int32_t, false>,
		is_great_war, bitfield_type,
		is_world_war, bitfield_type
	>;*/
}

namespace military {
	constexpr unit_type_tag army_unit_base(0);
	constexpr unit_type_tag naval_unit_base(1);

	struct leader_trait_name;
	struct leader_trait_values;
	struct personality_traits;
	struct background_traits;

	constexpr static leader_trait_tag no_personality_trait = leader_trait_tag(0);
	constexpr static leader_trait_tag no_background_trait = leader_trait_tag(1);
}
