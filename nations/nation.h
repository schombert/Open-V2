#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace nation {
	class container;
}

template<>
class serialization::serializer<nation::container>;

namespace nation {
	struct sphere_leader;
	struct overlord;
	struct enabled_crimes;
	struct current_rules;
	struct total_core_population;
	struct last_election;
	struct last_reform_date;
	struct last_manual_ruling_party_change;
	struct last_lost_war;
	struct disarmed_until;
	struct total_foreign_investment;
	struct treasury;
	struct plurality;
	struct revanchism;
	struct base_prestige;
	struct infamy;
	struct war_exhaustion;
	struct suppression_points;
	struct diplomacy_points;
	struct research_points;
	struct national_debt;
	struct tax_base;
	struct political_interest_fraction;
	struct social_interest_fraction;
	struct national_administrative_efficiency;
	struct social_movement_support;
	struct political_movement_support;
	struct owned_provinces;
	struct controlled_provinces;
	struct naval_patrols;
	struct sphere_members;
	struct vassals;
	struct allies;
	struct neighboring_nations;
	struct accepted_cultures;
	struct member_states;
	struct gp_influence;
	struct influencers;
	struct relations;
	struct truces;
	struct national_focus_locations;
	struct national_flags;
	struct static_modifiers;
	struct timed_modifiers;
	struct statewise_tariff_mask;
	struct generals;
	struct admirals;
	struct armies;
	struct fleets;
	struct active_orders;
	struct strategic_hqs;
	struct active_cbs;
	struct wars_involved_in;
	struct opponents_in_war;
	struct allies_in_war;
	struct name;
	struct adjective;
	struct national_value;
	struct tech_school;
	struct flag;
	struct current_color;
	struct current_research;
	struct military_score;
	struct industrial_score;
	struct overall_rank;
	struct prestige_rank;
	struct military_rank;
	struct industrial_rank;
	struct province_count;
	struct central_province_count;
	struct rebel_controlled_provinces;
	struct blockaded_count;
	struct crime_count;
	struct leadership_points;
	struct base_colonial_points;
	struct num_connected_ports;
	struct num_ports;
	struct player_importance;
	struct cb_construction_progress;
	struct cb_construction_target;
	struct cb_construction_type;
	struct ruling_party;
	struct current_capital;
	struct tag;
	struct primary_culture;
	struct dominant_culture;
	struct dominant_issue;
	struct dominant_ideology;
	struct dominant_religion;
	struct national_religion;
	struct current_government;
	struct ruling_ideology;
	struct f_rich_tax;
	struct f_middle_tax;
	struct f_poor_tax;
	struct f_social_spending;
	struct f_administrative_spending;
	struct f_education_spending;
	struct f_military_spending;
	struct f_tariffs;
	struct f_army_stockpile_spending;
	struct f_navy_stockpile_spending;
	struct f_projects_stockpile_spending;
	struct is_civilized;
	struct is_substate;
	struct is_mobilized;
	struct is_at_war;
	struct is_not_ai_controlled;
	struct is_holding_election;
	struct is_colonial_nation;
	struct cb_construction_discovered;
	struct has_gas_attack;
	struct has_gas_defence;

	class alignas(64) container {
		 int32_t size_used = 0;
		 nations::country_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_0() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_1() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 uint8_t padding[(sizeof(uint64_t) + 63ui32) & ~63ui32]; 
			 uint64_t values[(sizeof(uint64_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint64_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint64_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_2() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint64_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint64_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint64_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(int32_t) + 63ui32) & ~63ui32]; 
			 int32_t values[(sizeof(int32_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int32_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int32_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(int32_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int32_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int32_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_8;
		 struct alignas(64) dtype_9 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_9() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_9;
		 struct alignas(64) dtype_10 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_10() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_10;
		 struct alignas(64) dtype_11 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_11() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_11;
		 struct alignas(64) dtype_12 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_12() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_12;
		 struct alignas(64) dtype_13 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_13() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_13;
		 struct alignas(64) dtype_14 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_14() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_14;
		 struct alignas(64) dtype_15 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_15() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_15;
		 struct alignas(64) dtype_16 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_16() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_16;
		 struct alignas(64) dtype_17 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_17() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_17;
		 struct alignas(64) dtype_18 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_18() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_18;
		 struct alignas(64) dtype_19 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_19() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_19;
		 struct alignas(64) dtype_20 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_20() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_20;
		 struct alignas(64) dtype_21 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_21() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_21;
		 struct alignas(64) dtype_22 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_22() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_22;
		 struct alignas(64) dtype_23 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_23() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_23;
		 struct alignas(64) dtype_24 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_24() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_24;
		 struct alignas(64) dtype_25 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_25() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_25;
		 struct alignas(64) dtype_26 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_26() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_26;
		 struct alignas(64) dtype_27 { 
			 uint8_t padding[(sizeof(set_tag<provinces::province_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<provinces::province_tag> values[(sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_27() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_27;
		 struct alignas(64) dtype_28 { 
			 uint8_t padding[(sizeof(set_tag<provinces::province_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<provinces::province_tag> values[(sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_28() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_28;
		 struct alignas(64) dtype_29 { 
			 uint8_t padding[(sizeof(set_tag<provinces::province_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<provinces::province_tag> values[(sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_29() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_29;
		 struct alignas(64) dtype_30 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_30() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_30;
		 struct alignas(64) dtype_31 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_31() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_31;
		 struct alignas(64) dtype_32 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_32() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_32;
		 struct alignas(64) dtype_33 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_33() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_33;
		 struct alignas(64) dtype_34 { 
			 uint8_t padding[(sizeof(set_tag<cultures::culture_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<cultures::culture_tag> values[(sizeof(set_tag<cultures::culture_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<cultures::culture_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<cultures::culture_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_34() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<cultures::culture_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<cultures::culture_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<cultures::culture_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_34;
		 struct alignas(64) dtype_35 { 
			 uint8_t padding[(sizeof(set_tag<nations::region_state_pair>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::region_state_pair> values[(sizeof(set_tag<nations::region_state_pair>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::region_state_pair>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::region_state_pair>)) - 1ui32) : uint32_t(400))]; 
			 dtype_35() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::region_state_pair>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::region_state_pair>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::region_state_pair>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_35;
		 struct alignas(64) dtype_36 { 
			 uint8_t padding[(sizeof(set_tag<nations::influence>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::influence> values[(sizeof(set_tag<nations::influence>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::influence>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::influence>)) - 1ui32) : uint32_t(400))]; 
			 dtype_36() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::influence>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::influence>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::influence>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_36;
		 struct alignas(64) dtype_37 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_37() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_37;
		 struct alignas(64) dtype_38 { 
			 uint8_t padding[(sizeof(set_tag<nations::relationship>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::relationship> values[(sizeof(set_tag<nations::relationship>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::relationship>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::relationship>)) - 1ui32) : uint32_t(400))]; 
			 dtype_38() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::relationship>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::relationship>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::relationship>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_38;
		 struct alignas(64) dtype_39 { 
			 uint8_t padding[(sizeof(set_tag<nations::truce>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::truce> values[(sizeof(set_tag<nations::truce>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::truce>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::truce>)) - 1ui32) : uint32_t(400))]; 
			 dtype_39() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::truce>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::truce>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::truce>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_39;
		 struct alignas(64) dtype_40 { 
			 uint8_t padding[(sizeof(set_tag<nations::state_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::state_tag> values[(sizeof(set_tag<nations::state_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::state_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::state_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_40() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::state_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::state_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::state_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_40;
		 struct alignas(64) dtype_41 { 
			 uint8_t padding[(sizeof(set_tag<variables::national_flag_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<variables::national_flag_tag> values[(sizeof(set_tag<variables::national_flag_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<variables::national_flag_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<variables::national_flag_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_41() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<variables::national_flag_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<variables::national_flag_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<variables::national_flag_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_41;
		 struct alignas(64) dtype_42 { 
			 uint8_t padding[(sizeof(multiset_tag<modifiers::national_modifier_tag>) + 63ui32) & ~63ui32]; 
			 multiset_tag<modifiers::national_modifier_tag> values[(sizeof(multiset_tag<modifiers::national_modifier_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(multiset_tag<modifiers::national_modifier_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(multiset_tag<modifiers::national_modifier_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_42() { std::uninitialized_value_construct_n(values - 1, (sizeof(multiset_tag<modifiers::national_modifier_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(multiset_tag<modifiers::national_modifier_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(multiset_tag<modifiers::national_modifier_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_42;
		 struct alignas(64) dtype_43 { 
			 uint8_t padding[(sizeof(multiset_tag<nations::timed_national_modifier>) + 63ui32) & ~63ui32]; 
			 multiset_tag<nations::timed_national_modifier> values[(sizeof(multiset_tag<nations::timed_national_modifier>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(multiset_tag<nations::timed_national_modifier>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(multiset_tag<nations::timed_national_modifier>)) - 1ui32) : uint32_t(400))]; 
			 dtype_43() { std::uninitialized_value_construct_n(values - 1, (sizeof(multiset_tag<nations::timed_national_modifier>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(multiset_tag<nations::timed_national_modifier>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(multiset_tag<nations::timed_national_modifier>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_43;
		 struct alignas(64) dtype_44 { 
			 uint8_t padding[(sizeof(array_tag<economy::money_qnty_type, nations::state_tag, true>) + 63ui32) & ~63ui32]; 
			 array_tag<economy::money_qnty_type, nations::state_tag, true> values[(sizeof(array_tag<economy::money_qnty_type, nations::state_tag, true>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<economy::money_qnty_type, nations::state_tag, true>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<economy::money_qnty_type, nations::state_tag, true>)) - 1ui32) : uint32_t(400))]; 
			 dtype_44() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<economy::money_qnty_type, nations::state_tag, true>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<economy::money_qnty_type, nations::state_tag, true>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<economy::money_qnty_type, nations::state_tag, true>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_44;
		 struct alignas(64) dtype_45 { 
			 uint8_t padding[(sizeof(array_tag<military::leader_tag, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::leader_tag, int32_t, false> values[(sizeof(array_tag<military::leader_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>)) - 1ui32) : uint32_t(400))]; 
			 dtype_45() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::leader_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_45;
		 struct alignas(64) dtype_46 { 
			 uint8_t padding[(sizeof(array_tag<military::leader_tag, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::leader_tag, int32_t, false> values[(sizeof(array_tag<military::leader_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>)) - 1ui32) : uint32_t(400))]; 
			 dtype_46() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::leader_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::leader_tag, int32_t, false>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_46;
		 struct alignas(64) dtype_47 { 
			 uint8_t padding[(sizeof(array_tag<military::army_tag, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::army_tag, int32_t, false> values[(sizeof(array_tag<military::army_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::army_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::army_tag, int32_t, false>)) - 1ui32) : uint32_t(400))]; 
			 dtype_47() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::army_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::army_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::army_tag, int32_t, false>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_47;
		 struct alignas(64) dtype_48 { 
			 uint8_t padding[(sizeof(array_tag<military::fleet_tag, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::fleet_tag, int32_t, false> values[(sizeof(array_tag<military::fleet_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::fleet_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::fleet_tag, int32_t, false>)) - 1ui32) : uint32_t(400))]; 
			 dtype_48() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::fleet_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::fleet_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::fleet_tag, int32_t, false>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_48;
		 struct alignas(64) dtype_49 { 
			 uint8_t padding[(sizeof(array_tag<military::army_orders_tag, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::army_orders_tag, int32_t, false> values[(sizeof(array_tag<military::army_orders_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::army_orders_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::army_orders_tag, int32_t, false>)) - 1ui32) : uint32_t(400))]; 
			 dtype_49() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::army_orders_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::army_orders_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::army_orders_tag, int32_t, false>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_49;
		 struct alignas(64) dtype_50 { 
			 uint8_t padding[(sizeof(array_tag<military::strategic_hq_tag, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::strategic_hq_tag, int32_t, false> values[(sizeof(array_tag<military::strategic_hq_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::strategic_hq_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::strategic_hq_tag, int32_t, false>)) - 1ui32) : uint32_t(400))]; 
			 dtype_50() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::strategic_hq_tag, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::strategic_hq_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::strategic_hq_tag, int32_t, false>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_50;
		 struct alignas(64) dtype_51 { 
			 uint8_t padding[(sizeof(array_tag<military::pending_cb, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::pending_cb, int32_t, false> values[(sizeof(array_tag<military::pending_cb, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::pending_cb, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::pending_cb, int32_t, false>)) - 1ui32) : uint32_t(400))]; 
			 dtype_51() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::pending_cb, int32_t, false>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(array_tag<military::pending_cb, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::pending_cb, int32_t, false>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_51;
		 struct alignas(64) dtype_52 { 
			 uint8_t padding[(sizeof(set_tag<military::war_identifier>) + 63ui32) & ~63ui32]; 
			 set_tag<military::war_identifier> values[(sizeof(set_tag<military::war_identifier>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<military::war_identifier>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::war_identifier>)) - 1ui32) : uint32_t(400))]; 
			 dtype_52() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<military::war_identifier>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<military::war_identifier>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::war_identifier>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_52;
		 struct alignas(64) dtype_53 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_53() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_53;
		 struct alignas(64) dtype_54 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400))]; 
			 dtype_54() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_54;
		 struct alignas(64) dtype_55 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_55() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_55;
		 struct alignas(64) dtype_56 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_56() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_56;
		 struct alignas(64) dtype_57 { 
			 uint8_t padding[(sizeof(modifiers::national_modifier_tag) + 63ui32) & ~63ui32]; 
			 modifiers::national_modifier_tag values[(sizeof(modifiers::national_modifier_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_57() { std::uninitialized_value_construct_n(values - 1, (sizeof(modifiers::national_modifier_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_57;
		 struct alignas(64) dtype_58 { 
			 uint8_t padding[(sizeof(modifiers::national_modifier_tag) + 63ui32) & ~63ui32]; 
			 modifiers::national_modifier_tag values[(sizeof(modifiers::national_modifier_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_58() { std::uninitialized_value_construct_n(values - 1, (sizeof(modifiers::national_modifier_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::national_modifier_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_58;
		 struct alignas(64) dtype_59 { 
			 uint8_t padding[(sizeof(graphics::texture_tag) + 63ui32) & ~63ui32]; 
			 graphics::texture_tag values[(sizeof(graphics::texture_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(graphics::texture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(graphics::texture_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_59() { std::uninitialized_value_construct_n(values - 1, (sizeof(graphics::texture_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(graphics::texture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(graphics::texture_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_59;
		 struct alignas(64) dtype_60 { 
			 uint8_t padding[(sizeof(graphics::color_rgb) + 63ui32) & ~63ui32]; 
			 graphics::color_rgb values[(sizeof(graphics::color_rgb) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(graphics::color_rgb))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(graphics::color_rgb)) - 1ui32) : uint32_t(400))]; 
			 dtype_60() { std::uninitialized_value_construct_n(values - 1, (sizeof(graphics::color_rgb) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(graphics::color_rgb))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(graphics::color_rgb)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_60;
		 struct alignas(64) dtype_61 { 
			 uint8_t padding[(sizeof(technologies::tech_tag) + 63ui32) & ~63ui32]; 
			 technologies::tech_tag values[(sizeof(technologies::tech_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(technologies::tech_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(technologies::tech_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_61() { std::uninitialized_value_construct_n(values - 1, (sizeof(technologies::tech_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(technologies::tech_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(technologies::tech_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_61;
		 struct alignas(64) dtype_62 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_62() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_62;
		 struct alignas(64) dtype_63 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_63() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_63;
		 struct alignas(64) dtype_64 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_64() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_64;
		 struct alignas(64) dtype_65 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_65() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_65;
		 struct alignas(64) dtype_66 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_66() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_66;
		 struct alignas(64) dtype_67 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_67() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_67;
		 struct alignas(64) dtype_68 { 
			 uint8_t padding[(sizeof(uint16_t) + 63ui32) & ~63ui32]; 
			 uint16_t values[(sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_68() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_68;
		 struct alignas(64) dtype_69 { 
			 uint8_t padding[(sizeof(uint16_t) + 63ui32) & ~63ui32]; 
			 uint16_t values[(sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_69() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_69;
		 struct alignas(64) dtype_70 { 
			 uint8_t padding[(sizeof(uint16_t) + 63ui32) & ~63ui32]; 
			 uint16_t values[(sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_70() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_70;
		 struct alignas(64) dtype_71 { 
			 uint8_t padding[(sizeof(uint16_t) + 63ui32) & ~63ui32]; 
			 uint16_t values[(sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_71() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_71;
		 struct alignas(64) dtype_72 { 
			 uint8_t padding[(sizeof(uint16_t) + 63ui32) & ~63ui32]; 
			 uint16_t values[(sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_72() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_72;
		 struct alignas(64) dtype_73 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_73() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_73;
		 struct alignas(64) dtype_74 { 
			 uint8_t padding[(sizeof(int16_t) + 63ui32) & ~63ui32]; 
			 int16_t values[(sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_74() { std::uninitialized_value_construct_n(values - 1, (sizeof(int16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_74;
		 struct alignas(64) dtype_75 { 
			 uint8_t padding[(sizeof(uint16_t) + 63ui32) & ~63ui32]; 
			 uint16_t values[(sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_75() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_75;
		 struct alignas(64) dtype_76 { 
			 uint8_t padding[(sizeof(uint16_t) + 63ui32) & ~63ui32]; 
			 uint16_t values[(sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_76() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint16_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(uint16_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint16_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_76;
		 struct alignas(64) dtype_77 { 
			 uint8_t padding[(sizeof(int8_t) + 63ui32) & ~63ui32]; 
			 int8_t values[(sizeof(int8_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int8_t)) - 1ui32) : uint32_t(400))]; 
			 dtype_77() { std::uninitialized_value_construct_n(values - 1, (sizeof(int8_t) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(int8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int8_t)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_77;
		 struct alignas(64) dtype_78 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_78() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_78;
		 struct alignas(64) dtype_79 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_79() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_79;
		 struct alignas(64) dtype_80 { 
			 uint8_t padding[(sizeof(military::cb_type_tag) + 63ui32) & ~63ui32]; 
			 military::cb_type_tag values[(sizeof(military::cb_type_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(military::cb_type_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::cb_type_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_80() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::cb_type_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(military::cb_type_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::cb_type_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_80;
		 struct alignas(64) dtype_81 { 
			 uint8_t padding[(sizeof(governments::party_tag) + 63ui32) & ~63ui32]; 
			 governments::party_tag values[(sizeof(governments::party_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(governments::party_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(governments::party_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_81() { std::uninitialized_value_construct_n(values - 1, (sizeof(governments::party_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(governments::party_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(governments::party_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_81;
		 struct alignas(64) dtype_82 { 
			 uint8_t padding[(sizeof(provinces::province_tag) + 63ui32) & ~63ui32]; 
			 provinces::province_tag values[(sizeof(provinces::province_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_82() { std::uninitialized_value_construct_n(values - 1, (sizeof(provinces::province_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_82;
		 struct alignas(64) dtype_83 { 
			 uint8_t padding[(sizeof(cultures::national_tag) + 63ui32) & ~63ui32]; 
			 cultures::national_tag values[(sizeof(cultures::national_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::national_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::national_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_83() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::national_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::national_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::national_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_83;
		 struct alignas(64) dtype_84 { 
			 uint8_t padding[(sizeof(cultures::culture_tag) + 63ui32) & ~63ui32]; 
			 cultures::culture_tag values[(sizeof(cultures::culture_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_84() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::culture_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_84;
		 struct alignas(64) dtype_85 { 
			 uint8_t padding[(sizeof(cultures::culture_tag) + 63ui32) & ~63ui32]; 
			 cultures::culture_tag values[(sizeof(cultures::culture_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_85() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::culture_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_85;
		 struct alignas(64) dtype_86 { 
			 uint8_t padding[(sizeof(issues::option_tag) + 63ui32) & ~63ui32]; 
			 issues::option_tag values[(sizeof(issues::option_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(issues::option_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(issues::option_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_86() { std::uninitialized_value_construct_n(values - 1, (sizeof(issues::option_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(issues::option_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(issues::option_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_86;
		 struct alignas(64) dtype_87 { 
			 uint8_t padding[(sizeof(ideologies::ideology_tag) + 63ui32) & ~63ui32]; 
			 ideologies::ideology_tag values[(sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_87() { std::uninitialized_value_construct_n(values - 1, (sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_87;
		 struct alignas(64) dtype_88 { 
			 uint8_t padding[(sizeof(cultures::religion_tag) + 63ui32) & ~63ui32]; 
			 cultures::religion_tag values[(sizeof(cultures::religion_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_88() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::religion_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_88;
		 struct alignas(64) dtype_89 { 
			 uint8_t padding[(sizeof(cultures::religion_tag) + 63ui32) & ~63ui32]; 
			 cultures::religion_tag values[(sizeof(cultures::religion_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_89() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::religion_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_89;
		 struct alignas(64) dtype_90 { 
			 uint8_t padding[(sizeof(governments::government_tag) + 63ui32) & ~63ui32]; 
			 governments::government_tag values[(sizeof(governments::government_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(governments::government_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(governments::government_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_90() { std::uninitialized_value_construct_n(values - 1, (sizeof(governments::government_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(governments::government_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(governments::government_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_90;
		 struct alignas(64) dtype_91 { 
			 uint8_t padding[(sizeof(ideologies::ideology_tag) + 63ui32) & ~63ui32]; 
			 ideologies::ideology_tag values[(sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(400))]; 
			 dtype_91() { std::uninitialized_value_construct_n(values - 1, (sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_91;
		 struct alignas(64) dtype_92 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_92() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_92;
		 struct alignas(64) dtype_93 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_93() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_93;
		 struct alignas(64) dtype_94 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_94() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_94;
		 struct alignas(64) dtype_95 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_95() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_95;
		 struct alignas(64) dtype_96 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_96() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_96;
		 struct alignas(64) dtype_97 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_97() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_97;
		 struct alignas(64) dtype_98 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_98() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_98;
		 struct alignas(64) dtype_99 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_99() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_99;
		 struct alignas(64) dtype_100 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_100() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_100;
		 struct alignas(64) dtype_101 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_101() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_101;
		 struct alignas(64) dtype_102 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400))]; 
			 dtype_102() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(400) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(400)) + 1); }
		 } m_102;
		 struct alignas(64) dtype_103 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_103() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_103;
		 struct alignas(64) dtype_104 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_104() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_104;
		 struct alignas(64) dtype_105 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_105() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_105;
		 struct alignas(64) dtype_106 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_106() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_106;
		 struct alignas(64) dtype_107 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_107() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_107;
		 struct alignas(64) dtype_108 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_108() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_108;
		 struct alignas(64) dtype_109 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_109() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_109;
		 struct alignas(64) dtype_110 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_110() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_110;
		 struct alignas(64) dtype_111 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_111() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_111;
		 struct alignas(64) dtype_112 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_112() { std::fill_n(values - 1, 1 + ((uint32_t(400 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_112;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = 400 - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = nations::country_tag(nations::country_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_leader>, nations::country_tag&> get(nations::country_tag i) {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_leader>, nations::country_tag const&> get(nations::country_tag i) const {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::sphere_leader>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::sphere_leader>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_leader>, tagged_array_view<nations::country_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, nations::country_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_leader>, tagged_array_view<nations::country_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, nations::country_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overlord>, nations::country_tag&> get(nations::country_tag i) {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overlord>, nations::country_tag const&> get(nations::country_tag i) const {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::overlord>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::overlord>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overlord>, tagged_array_view<nations::country_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, nations::country_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overlord>, tagged_array_view<nations::country_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, nations::country_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::enabled_crimes>, uint64_t&> get(nations::country_tag i) {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::enabled_crimes>, uint64_t const&> get(nations::country_tag i) const {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::enabled_crimes>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::enabled_crimes>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::enabled_crimes>, tagged_array_view<uint64_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint64_t, nations::country_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::enabled_crimes>, tagged_array_view<uint64_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint64_t const, nations::country_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_rules>, int32_t&> get(nations::country_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_rules>, int32_t const&> get(nations::country_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_rules>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_rules>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_rules>, tagged_array_view<int32_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int32_t, nations::country_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_rules>, tagged_array_view<int32_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int32_t const, nations::country_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_core_population>, float&> get(nations::country_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_core_population>, float const&> get(nations::country_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::total_core_population>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::total_core_population>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_core_population>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_core_population>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_election>, date_tag&> get(nations::country_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_election>, date_tag const&> get(nations::country_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_election>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_election>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_election>, tagged_array_view<date_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<date_tag, nations::country_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_election>, tagged_array_view<date_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<date_tag const, nations::country_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_reform_date>, date_tag&> get(nations::country_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_reform_date>, date_tag const&> get(nations::country_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_reform_date>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_reform_date>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_reform_date>, tagged_array_view<date_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<date_tag, nations::country_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_reform_date>, tagged_array_view<date_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<date_tag const, nations::country_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_manual_ruling_party_change>, date_tag&> get(nations::country_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_manual_ruling_party_change>, date_tag const&> get(nations::country_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_manual_ruling_party_change>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_manual_ruling_party_change>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_manual_ruling_party_change>, tagged_array_view<date_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<date_tag, nations::country_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_manual_ruling_party_change>, tagged_array_view<date_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<date_tag const, nations::country_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_lost_war>, date_tag&> get(nations::country_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_lost_war>, date_tag const&> get(nations::country_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_lost_war>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::last_lost_war>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_lost_war>, tagged_array_view<date_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<date_tag, nations::country_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::last_lost_war>, tagged_array_view<date_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<date_tag const, nations::country_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::disarmed_until>, date_tag&> get(nations::country_tag i) {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::disarmed_until>, date_tag const&> get(nations::country_tag i) const {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::disarmed_until>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::disarmed_until>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::disarmed_until>, tagged_array_view<date_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<date_tag, nations::country_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::disarmed_until>, tagged_array_view<date_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<date_tag const, nations::country_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_foreign_investment>, float&> get(nations::country_tag i) {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_foreign_investment>, float const&> get(nations::country_tag i) const {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::total_foreign_investment>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::total_foreign_investment>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_foreign_investment>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::total_foreign_investment>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::treasury>, float&> get(nations::country_tag i) {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::treasury>, float const&> get(nations::country_tag i) const {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::treasury>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::treasury>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::treasury>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::treasury>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::plurality>, float&> get(nations::country_tag i) {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::plurality>, float const&> get(nations::country_tag i) const {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::plurality>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::plurality>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::plurality>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::plurality>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::revanchism>, float&> get(nations::country_tag i) {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::revanchism>, float const&> get(nations::country_tag i) const {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::revanchism>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::revanchism>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::revanchism>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::revanchism>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_prestige>, float&> get(nations::country_tag i) {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_prestige>, float const&> get(nations::country_tag i) const {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::base_prestige>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::base_prestige>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_prestige>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_prestige>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::infamy>, float&> get(nations::country_tag i) {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::infamy>, float const&> get(nations::country_tag i) const {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::infamy>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::infamy>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::infamy>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::infamy>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::war_exhaustion>, float&> get(nations::country_tag i) {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::war_exhaustion>, float const&> get(nations::country_tag i) const {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::war_exhaustion>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::war_exhaustion>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::war_exhaustion>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::war_exhaustion>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::suppression_points>, float&> get(nations::country_tag i) {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::suppression_points>, float const&> get(nations::country_tag i) const {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::suppression_points>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::suppression_points>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::suppression_points>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::suppression_points>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::diplomacy_points>, float&> get(nations::country_tag i) {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::diplomacy_points>, float const&> get(nations::country_tag i) const {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::diplomacy_points>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::diplomacy_points>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::diplomacy_points>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::diplomacy_points>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::research_points>, float&> get(nations::country_tag i) {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::research_points>, float const&> get(nations::country_tag i) const {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::research_points>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::research_points>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::research_points>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::research_points>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_debt>, float&> get(nations::country_tag i) {
			 return m_20.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_debt>, float const&> get(nations::country_tag i) const {
			 return m_20.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_debt>> {
			 m_20.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_debt>> {
			 m_20.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_debt>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_20.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_debt>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_20.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tax_base>, float&> get(nations::country_tag i) {
			 return m_21.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tax_base>, float const&> get(nations::country_tag i) const {
			 return m_21.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::tax_base>> {
			 m_21.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::tax_base>> {
			 m_21.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tax_base>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_21.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tax_base>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_21.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_interest_fraction>, float&> get(nations::country_tag i) {
			 return m_22.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_interest_fraction>, float const&> get(nations::country_tag i) const {
			 return m_22.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::political_interest_fraction>> {
			 m_22.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::political_interest_fraction>> {
			 m_22.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_interest_fraction>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_22.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_interest_fraction>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_22.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_interest_fraction>, float&> get(nations::country_tag i) {
			 return m_23.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_interest_fraction>, float const&> get(nations::country_tag i) const {
			 return m_23.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::social_interest_fraction>> {
			 m_23.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::social_interest_fraction>> {
			 m_23.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_interest_fraction>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_23.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_interest_fraction>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_23.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_administrative_efficiency>, float&> get(nations::country_tag i) {
			 return m_24.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_administrative_efficiency>, float const&> get(nations::country_tag i) const {
			 return m_24.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_administrative_efficiency>> {
			 m_24.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_administrative_efficiency>> {
			 m_24.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_administrative_efficiency>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_24.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_administrative_efficiency>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_24.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_movement_support>, float&> get(nations::country_tag i) {
			 return m_25.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_movement_support>, float const&> get(nations::country_tag i) const {
			 return m_25.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::social_movement_support>> {
			 m_25.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::social_movement_support>> {
			 m_25.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_movement_support>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_25.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::social_movement_support>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_25.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_movement_support>, float&> get(nations::country_tag i) {
			 return m_26.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_movement_support>, float const&> get(nations::country_tag i) const {
			 return m_26.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::political_movement_support>> {
			 m_26.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::political_movement_support>> {
			 m_26.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_movement_support>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_26.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::political_movement_support>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_26.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::owned_provinces>, set_tag<provinces::province_tag>&> get(nations::country_tag i) {
			 return m_27.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::owned_provinces>, set_tag<provinces::province_tag> const&> get(nations::country_tag i) const {
			 return m_27.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::owned_provinces>> {
			 m_27.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::owned_provinces>> {
			 m_27.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::owned_provinces>, tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>(m_27.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::owned_provinces>, tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>(m_27.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::controlled_provinces>, set_tag<provinces::province_tag>&> get(nations::country_tag i) {
			 return m_28.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::controlled_provinces>, set_tag<provinces::province_tag> const&> get(nations::country_tag i) const {
			 return m_28.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::controlled_provinces>> {
			 m_28.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::controlled_provinces>> {
			 m_28.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::controlled_provinces>, tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>(m_28.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::controlled_provinces>, tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>(m_28.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::naval_patrols>, set_tag<provinces::province_tag>&> get(nations::country_tag i) {
			 return m_29.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::naval_patrols>, set_tag<provinces::province_tag> const&> get(nations::country_tag i) const {
			 return m_29.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::naval_patrols>> {
			 m_29.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::naval_patrols>> {
			 m_29.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::naval_patrols>, tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>(m_29.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::naval_patrols>, tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>(m_29.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_members>, set_tag<nations::country_tag>&> get(nations::country_tag i) {
			 return m_30.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_members>, set_tag<nations::country_tag> const&> get(nations::country_tag i) const {
			 return m_30.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::sphere_members>> {
			 m_30.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::sphere_members>> {
			 m_30.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_members>, tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>(m_30.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::sphere_members>, tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>(m_30.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::vassals>, set_tag<nations::country_tag>&> get(nations::country_tag i) {
			 return m_31.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::vassals>, set_tag<nations::country_tag> const&> get(nations::country_tag i) const {
			 return m_31.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::vassals>> {
			 m_31.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::vassals>> {
			 m_31.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::vassals>, tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>(m_31.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::vassals>, tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>(m_31.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies>, set_tag<nations::country_tag>&> get(nations::country_tag i) {
			 return m_32.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies>, set_tag<nations::country_tag> const&> get(nations::country_tag i) const {
			 return m_32.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::allies>> {
			 m_32.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::allies>> {
			 m_32.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies>, tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>(m_32.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies>, tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>(m_32.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::neighboring_nations>, set_tag<nations::country_tag>&> get(nations::country_tag i) {
			 return m_33.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::neighboring_nations>, set_tag<nations::country_tag> const&> get(nations::country_tag i) const {
			 return m_33.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::neighboring_nations>> {
			 m_33.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::neighboring_nations>> {
			 m_33.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::neighboring_nations>, tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>(m_33.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::neighboring_nations>, tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>(m_33.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::accepted_cultures>, set_tag<cultures::culture_tag>&> get(nations::country_tag i) {
			 return m_34.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::accepted_cultures>, set_tag<cultures::culture_tag> const&> get(nations::country_tag i) const {
			 return m_34.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::accepted_cultures>> {
			 m_34.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::accepted_cultures>> {
			 m_34.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::accepted_cultures>, tagged_array_view<set_tag<cultures::culture_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<cultures::culture_tag>, nations::country_tag>(m_34.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::accepted_cultures>, tagged_array_view<set_tag<cultures::culture_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<cultures::culture_tag> const, nations::country_tag>(m_34.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::member_states>, set_tag<nations::region_state_pair>&> get(nations::country_tag i) {
			 return m_35.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::member_states>, set_tag<nations::region_state_pair> const&> get(nations::country_tag i) const {
			 return m_35.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::member_states>> {
			 m_35.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::member_states>> {
			 m_35.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::member_states>, tagged_array_view<set_tag<nations::region_state_pair>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::region_state_pair>, nations::country_tag>(m_35.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::member_states>, tagged_array_view<set_tag<nations::region_state_pair> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::region_state_pair> const, nations::country_tag>(m_35.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::gp_influence>, set_tag<nations::influence>&> get(nations::country_tag i) {
			 return m_36.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::gp_influence>, set_tag<nations::influence> const&> get(nations::country_tag i) const {
			 return m_36.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::gp_influence>> {
			 m_36.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::gp_influence>> {
			 m_36.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::gp_influence>, tagged_array_view<set_tag<nations::influence>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::influence>, nations::country_tag>(m_36.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::gp_influence>, tagged_array_view<set_tag<nations::influence> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::influence> const, nations::country_tag>(m_36.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::influencers>, set_tag<nations::country_tag>&> get(nations::country_tag i) {
			 return m_37.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::influencers>, set_tag<nations::country_tag> const&> get(nations::country_tag i) const {
			 return m_37.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::influencers>> {
			 m_37.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::influencers>> {
			 m_37.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::influencers>, tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>(m_37.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::influencers>, tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>(m_37.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::relations>, set_tag<nations::relationship>&> get(nations::country_tag i) {
			 return m_38.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::relations>, set_tag<nations::relationship> const&> get(nations::country_tag i) const {
			 return m_38.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::relations>> {
			 m_38.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::relations>> {
			 m_38.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::relations>, tagged_array_view<set_tag<nations::relationship>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::relationship>, nations::country_tag>(m_38.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::relations>, tagged_array_view<set_tag<nations::relationship> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::relationship> const, nations::country_tag>(m_38.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::truces>, set_tag<nations::truce>&> get(nations::country_tag i) {
			 return m_39.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::truces>, set_tag<nations::truce> const&> get(nations::country_tag i) const {
			 return m_39.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::truces>> {
			 m_39.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::truces>> {
			 m_39.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::truces>, tagged_array_view<set_tag<nations::truce>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::truce>, nations::country_tag>(m_39.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::truces>, tagged_array_view<set_tag<nations::truce> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::truce> const, nations::country_tag>(m_39.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_focus_locations>, set_tag<nations::state_tag>&> get(nations::country_tag i) {
			 return m_40.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_focus_locations>, set_tag<nations::state_tag> const&> get(nations::country_tag i) const {
			 return m_40.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_focus_locations>> {
			 m_40.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_focus_locations>> {
			 m_40.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_focus_locations>, tagged_array_view<set_tag<nations::state_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::state_tag>, nations::country_tag>(m_40.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_focus_locations>, tagged_array_view<set_tag<nations::state_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::state_tag> const, nations::country_tag>(m_40.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_flags>, set_tag<variables::national_flag_tag>&> get(nations::country_tag i) {
			 return m_41.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_flags>, set_tag<variables::national_flag_tag> const&> get(nations::country_tag i) const {
			 return m_41.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_flags>> {
			 m_41.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_flags>> {
			 m_41.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_flags>, tagged_array_view<set_tag<variables::national_flag_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<variables::national_flag_tag>, nations::country_tag>(m_41.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_flags>, tagged_array_view<set_tag<variables::national_flag_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<variables::national_flag_tag> const, nations::country_tag>(m_41.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::static_modifiers>, multiset_tag<modifiers::national_modifier_tag>&> get(nations::country_tag i) {
			 return m_42.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::static_modifiers>, multiset_tag<modifiers::national_modifier_tag> const&> get(nations::country_tag i) const {
			 return m_42.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::static_modifiers>> {
			 m_42.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::static_modifiers>> {
			 m_42.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::static_modifiers>, tagged_array_view<multiset_tag<modifiers::national_modifier_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<multiset_tag<modifiers::national_modifier_tag>, nations::country_tag>(m_42.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::static_modifiers>, tagged_array_view<multiset_tag<modifiers::national_modifier_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<multiset_tag<modifiers::national_modifier_tag> const, nations::country_tag>(m_42.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::timed_modifiers>, multiset_tag<nations::timed_national_modifier>&> get(nations::country_tag i) {
			 return m_43.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::timed_modifiers>, multiset_tag<nations::timed_national_modifier> const&> get(nations::country_tag i) const {
			 return m_43.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::timed_modifiers>> {
			 m_43.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::timed_modifiers>> {
			 m_43.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::timed_modifiers>, tagged_array_view<multiset_tag<nations::timed_national_modifier>, nations::country_tag>> get_row() {
			 return tagged_array_view<multiset_tag<nations::timed_national_modifier>, nations::country_tag>(m_43.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::timed_modifiers>, tagged_array_view<multiset_tag<nations::timed_national_modifier> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<multiset_tag<nations::timed_national_modifier> const, nations::country_tag>(m_43.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::statewise_tariff_mask>, array_tag<economy::money_qnty_type, nations::state_tag, true>&> get(nations::country_tag i) {
			 return m_44.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::statewise_tariff_mask>, array_tag<economy::money_qnty_type, nations::state_tag, true> const&> get(nations::country_tag i) const {
			 return m_44.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::statewise_tariff_mask>> {
			 m_44.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::statewise_tariff_mask>> {
			 m_44.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::statewise_tariff_mask>, tagged_array_view<array_tag<economy::money_qnty_type, nations::state_tag, true>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<economy::money_qnty_type, nations::state_tag, true>, nations::country_tag>(m_44.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::statewise_tariff_mask>, tagged_array_view<array_tag<economy::money_qnty_type, nations::state_tag, true> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<economy::money_qnty_type, nations::state_tag, true> const, nations::country_tag>(m_44.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::generals>, array_tag<military::leader_tag, int32_t, false>&> get(nations::country_tag i) {
			 return m_45.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::generals>, array_tag<military::leader_tag, int32_t, false> const&> get(nations::country_tag i) const {
			 return m_45.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::generals>> {
			 m_45.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::generals>> {
			 m_45.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::generals>, tagged_array_view<array_tag<military::leader_tag, int32_t, false>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<military::leader_tag, int32_t, false>, nations::country_tag>(m_45.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::generals>, tagged_array_view<array_tag<military::leader_tag, int32_t, false> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::leader_tag, int32_t, false> const, nations::country_tag>(m_45.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::admirals>, array_tag<military::leader_tag, int32_t, false>&> get(nations::country_tag i) {
			 return m_46.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::admirals>, array_tag<military::leader_tag, int32_t, false> const&> get(nations::country_tag i) const {
			 return m_46.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::admirals>> {
			 m_46.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::admirals>> {
			 m_46.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::admirals>, tagged_array_view<array_tag<military::leader_tag, int32_t, false>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<military::leader_tag, int32_t, false>, nations::country_tag>(m_46.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::admirals>, tagged_array_view<array_tag<military::leader_tag, int32_t, false> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::leader_tag, int32_t, false> const, nations::country_tag>(m_46.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::armies>, array_tag<military::army_tag, int32_t, false>&> get(nations::country_tag i) {
			 return m_47.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::armies>, array_tag<military::army_tag, int32_t, false> const&> get(nations::country_tag i) const {
			 return m_47.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::armies>> {
			 m_47.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::armies>> {
			 m_47.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::armies>, tagged_array_view<array_tag<military::army_tag, int32_t, false>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<military::army_tag, int32_t, false>, nations::country_tag>(m_47.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::armies>, tagged_array_view<array_tag<military::army_tag, int32_t, false> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::army_tag, int32_t, false> const, nations::country_tag>(m_47.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::fleets>, array_tag<military::fleet_tag, int32_t, false>&> get(nations::country_tag i) {
			 return m_48.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::fleets>, array_tag<military::fleet_tag, int32_t, false> const&> get(nations::country_tag i) const {
			 return m_48.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::fleets>> {
			 m_48.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::fleets>> {
			 m_48.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::fleets>, tagged_array_view<array_tag<military::fleet_tag, int32_t, false>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<military::fleet_tag, int32_t, false>, nations::country_tag>(m_48.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::fleets>, tagged_array_view<array_tag<military::fleet_tag, int32_t, false> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::fleet_tag, int32_t, false> const, nations::country_tag>(m_48.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_orders>, array_tag<military::army_orders_tag, int32_t, false>&> get(nations::country_tag i) {
			 return m_49.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_orders>, array_tag<military::army_orders_tag, int32_t, false> const&> get(nations::country_tag i) const {
			 return m_49.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::active_orders>> {
			 m_49.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::active_orders>> {
			 m_49.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_orders>, tagged_array_view<array_tag<military::army_orders_tag, int32_t, false>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<military::army_orders_tag, int32_t, false>, nations::country_tag>(m_49.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_orders>, tagged_array_view<array_tag<military::army_orders_tag, int32_t, false> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::army_orders_tag, int32_t, false> const, nations::country_tag>(m_49.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::strategic_hqs>, array_tag<military::strategic_hq_tag, int32_t, false>&> get(nations::country_tag i) {
			 return m_50.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::strategic_hqs>, array_tag<military::strategic_hq_tag, int32_t, false> const&> get(nations::country_tag i) const {
			 return m_50.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::strategic_hqs>> {
			 m_50.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::strategic_hqs>> {
			 m_50.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::strategic_hqs>, tagged_array_view<array_tag<military::strategic_hq_tag, int32_t, false>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<military::strategic_hq_tag, int32_t, false>, nations::country_tag>(m_50.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::strategic_hqs>, tagged_array_view<array_tag<military::strategic_hq_tag, int32_t, false> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::strategic_hq_tag, int32_t, false> const, nations::country_tag>(m_50.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_cbs>, array_tag<military::pending_cb, int32_t, false>&> get(nations::country_tag i) {
			 return m_51.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_cbs>, array_tag<military::pending_cb, int32_t, false> const&> get(nations::country_tag i) const {
			 return m_51.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::active_cbs>> {
			 m_51.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::active_cbs>> {
			 m_51.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_cbs>, tagged_array_view<array_tag<military::pending_cb, int32_t, false>, nations::country_tag>> get_row() {
			 return tagged_array_view<array_tag<military::pending_cb, int32_t, false>, nations::country_tag>(m_51.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::active_cbs>, tagged_array_view<array_tag<military::pending_cb, int32_t, false> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::pending_cb, int32_t, false> const, nations::country_tag>(m_51.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::wars_involved_in>, set_tag<military::war_identifier>&> get(nations::country_tag i) {
			 return m_52.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::wars_involved_in>, set_tag<military::war_identifier> const&> get(nations::country_tag i) const {
			 return m_52.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::wars_involved_in>> {
			 m_52.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::wars_involved_in>> {
			 m_52.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::wars_involved_in>, tagged_array_view<set_tag<military::war_identifier>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<military::war_identifier>, nations::country_tag>(m_52.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::wars_involved_in>, tagged_array_view<set_tag<military::war_identifier> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<military::war_identifier> const, nations::country_tag>(m_52.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::opponents_in_war>, set_tag<nations::country_tag>&> get(nations::country_tag i) {
			 return m_53.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::opponents_in_war>, set_tag<nations::country_tag> const&> get(nations::country_tag i) const {
			 return m_53.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::opponents_in_war>> {
			 m_53.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::opponents_in_war>> {
			 m_53.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::opponents_in_war>, tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>(m_53.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::opponents_in_war>, tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>(m_53.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies_in_war>, set_tag<nations::country_tag>&> get(nations::country_tag i) {
			 return m_54.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies_in_war>, set_tag<nations::country_tag> const&> get(nations::country_tag i) const {
			 return m_54.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::allies_in_war>> {
			 m_54.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::allies_in_war>> {
			 m_54.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies_in_war>, tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>(m_54.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::allies_in_war>, tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>(m_54.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::name>, text_data::text_tag&> get(nations::country_tag i) {
			 return m_55.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::name>, text_data::text_tag const&> get(nations::country_tag i) const {
			 return m_55.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::name>> {
			 m_55.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::name>> {
			 m_55.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::name>, tagged_array_view<text_data::text_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, nations::country_tag>(m_55.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::name>, tagged_array_view<text_data::text_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, nations::country_tag>(m_55.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::adjective>, text_data::text_tag&> get(nations::country_tag i) {
			 return m_56.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::adjective>, text_data::text_tag const&> get(nations::country_tag i) const {
			 return m_56.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::adjective>> {
			 m_56.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::adjective>> {
			 m_56.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::adjective>, tagged_array_view<text_data::text_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, nations::country_tag>(m_56.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::adjective>, tagged_array_view<text_data::text_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, nations::country_tag>(m_56.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_value>, modifiers::national_modifier_tag&> get(nations::country_tag i) {
			 return m_57.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_value>, modifiers::national_modifier_tag const&> get(nations::country_tag i) const {
			 return m_57.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_value>> {
			 m_57.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_value>> {
			 m_57.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_value>, tagged_array_view<modifiers::national_modifier_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<modifiers::national_modifier_tag, nations::country_tag>(m_57.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_value>, tagged_array_view<modifiers::national_modifier_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<modifiers::national_modifier_tag const, nations::country_tag>(m_57.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tech_school>, modifiers::national_modifier_tag&> get(nations::country_tag i) {
			 return m_58.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tech_school>, modifiers::national_modifier_tag const&> get(nations::country_tag i) const {
			 return m_58.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::tech_school>> {
			 m_58.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::tech_school>> {
			 m_58.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tech_school>, tagged_array_view<modifiers::national_modifier_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<modifiers::national_modifier_tag, nations::country_tag>(m_58.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tech_school>, tagged_array_view<modifiers::national_modifier_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<modifiers::national_modifier_tag const, nations::country_tag>(m_58.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::flag>, graphics::texture_tag&> get(nations::country_tag i) {
			 return m_59.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::flag>, graphics::texture_tag const&> get(nations::country_tag i) const {
			 return m_59.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::flag>> {
			 m_59.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::flag>> {
			 m_59.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::flag>, tagged_array_view<graphics::texture_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<graphics::texture_tag, nations::country_tag>(m_59.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::flag>, tagged_array_view<graphics::texture_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<graphics::texture_tag const, nations::country_tag>(m_59.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_color>, graphics::color_rgb&> get(nations::country_tag i) {
			 return m_60.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_color>, graphics::color_rgb const&> get(nations::country_tag i) const {
			 return m_60.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_color>> {
			 m_60.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_color>> {
			 m_60.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_color>, tagged_array_view<graphics::color_rgb, nations::country_tag>> get_row() {
			 return tagged_array_view<graphics::color_rgb, nations::country_tag>(m_60.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_color>, tagged_array_view<graphics::color_rgb const, nations::country_tag>> get_row() const {
			 return tagged_array_view<graphics::color_rgb const, nations::country_tag>(m_60.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_research>, technologies::tech_tag&> get(nations::country_tag i) {
			 return m_61.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_research>, technologies::tech_tag const&> get(nations::country_tag i) const {
			 return m_61.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_research>> {
			 m_61.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_research>> {
			 m_61.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_research>, tagged_array_view<technologies::tech_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<technologies::tech_tag, nations::country_tag>(m_61.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_research>, tagged_array_view<technologies::tech_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<technologies::tech_tag const, nations::country_tag>(m_61.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_score>, int16_t&> get(nations::country_tag i) {
			 return m_62.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_score>, int16_t const&> get(nations::country_tag i) const {
			 return m_62.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::military_score>> {
			 m_62.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::military_score>> {
			 m_62.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_score>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_62.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_score>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_62.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_score>, int16_t&> get(nations::country_tag i) {
			 return m_63.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_score>, int16_t const&> get(nations::country_tag i) const {
			 return m_63.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::industrial_score>> {
			 m_63.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::industrial_score>> {
			 m_63.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_score>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_63.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_score>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_63.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overall_rank>, int16_t&> get(nations::country_tag i) {
			 return m_64.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overall_rank>, int16_t const&> get(nations::country_tag i) const {
			 return m_64.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::overall_rank>> {
			 m_64.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::overall_rank>> {
			 m_64.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overall_rank>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_64.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::overall_rank>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_64.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::prestige_rank>, int16_t&> get(nations::country_tag i) {
			 return m_65.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::prestige_rank>, int16_t const&> get(nations::country_tag i) const {
			 return m_65.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::prestige_rank>> {
			 m_65.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::prestige_rank>> {
			 m_65.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::prestige_rank>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_65.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::prestige_rank>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_65.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_rank>, int16_t&> get(nations::country_tag i) {
			 return m_66.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_rank>, int16_t const&> get(nations::country_tag i) const {
			 return m_66.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::military_rank>> {
			 m_66.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::military_rank>> {
			 m_66.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_rank>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_66.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::military_rank>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_66.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_rank>, int16_t&> get(nations::country_tag i) {
			 return m_67.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_rank>, int16_t const&> get(nations::country_tag i) const {
			 return m_67.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::industrial_rank>> {
			 m_67.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::industrial_rank>> {
			 m_67.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_rank>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_67.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::industrial_rank>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_67.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::province_count>, uint16_t&> get(nations::country_tag i) {
			 return m_68.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::province_count>, uint16_t const&> get(nations::country_tag i) const {
			 return m_68.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::province_count>> {
			 m_68.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::province_count>> {
			 m_68.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::province_count>, tagged_array_view<uint16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint16_t, nations::country_tag>(m_68.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::province_count>, tagged_array_view<uint16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint16_t const, nations::country_tag>(m_68.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::central_province_count>, uint16_t&> get(nations::country_tag i) {
			 return m_69.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::central_province_count>, uint16_t const&> get(nations::country_tag i) const {
			 return m_69.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::central_province_count>> {
			 m_69.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::central_province_count>> {
			 m_69.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::central_province_count>, tagged_array_view<uint16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint16_t, nations::country_tag>(m_69.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::central_province_count>, tagged_array_view<uint16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint16_t const, nations::country_tag>(m_69.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::rebel_controlled_provinces>, uint16_t&> get(nations::country_tag i) {
			 return m_70.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::rebel_controlled_provinces>, uint16_t const&> get(nations::country_tag i) const {
			 return m_70.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::rebel_controlled_provinces>> {
			 m_70.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::rebel_controlled_provinces>> {
			 m_70.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::rebel_controlled_provinces>, tagged_array_view<uint16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint16_t, nations::country_tag>(m_70.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::rebel_controlled_provinces>, tagged_array_view<uint16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint16_t const, nations::country_tag>(m_70.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::blockaded_count>, uint16_t&> get(nations::country_tag i) {
			 return m_71.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::blockaded_count>, uint16_t const&> get(nations::country_tag i) const {
			 return m_71.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::blockaded_count>> {
			 m_71.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::blockaded_count>> {
			 m_71.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::blockaded_count>, tagged_array_view<uint16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint16_t, nations::country_tag>(m_71.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::blockaded_count>, tagged_array_view<uint16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint16_t const, nations::country_tag>(m_71.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::crime_count>, uint16_t&> get(nations::country_tag i) {
			 return m_72.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::crime_count>, uint16_t const&> get(nations::country_tag i) const {
			 return m_72.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::crime_count>> {
			 m_72.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::crime_count>> {
			 m_72.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::crime_count>, tagged_array_view<uint16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint16_t, nations::country_tag>(m_72.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::crime_count>, tagged_array_view<uint16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint16_t const, nations::country_tag>(m_72.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::leadership_points>, int16_t&> get(nations::country_tag i) {
			 return m_73.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::leadership_points>, int16_t const&> get(nations::country_tag i) const {
			 return m_73.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::leadership_points>> {
			 m_73.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::leadership_points>> {
			 m_73.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::leadership_points>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_73.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::leadership_points>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_73.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_colonial_points>, int16_t&> get(nations::country_tag i) {
			 return m_74.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_colonial_points>, int16_t const&> get(nations::country_tag i) const {
			 return m_74.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::base_colonial_points>> {
			 m_74.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::base_colonial_points>> {
			 m_74.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_colonial_points>, tagged_array_view<int16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int16_t, nations::country_tag>(m_74.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::base_colonial_points>, tagged_array_view<int16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int16_t const, nations::country_tag>(m_74.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_connected_ports>, uint16_t&> get(nations::country_tag i) {
			 return m_75.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_connected_ports>, uint16_t const&> get(nations::country_tag i) const {
			 return m_75.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::num_connected_ports>> {
			 m_75.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::num_connected_ports>> {
			 m_75.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_connected_ports>, tagged_array_view<uint16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint16_t, nations::country_tag>(m_75.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_connected_ports>, tagged_array_view<uint16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint16_t const, nations::country_tag>(m_75.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_ports>, uint16_t&> get(nations::country_tag i) {
			 return m_76.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_ports>, uint16_t const&> get(nations::country_tag i) const {
			 return m_76.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::num_ports>> {
			 m_76.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::num_ports>> {
			 m_76.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_ports>, tagged_array_view<uint16_t, nations::country_tag>> get_row() {
			 return tagged_array_view<uint16_t, nations::country_tag>(m_76.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::num_ports>, tagged_array_view<uint16_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<uint16_t const, nations::country_tag>(m_76.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::player_importance>, int8_t&> get(nations::country_tag i) {
			 return m_77.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::player_importance>, int8_t const&> get(nations::country_tag i) const {
			 return m_77.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::player_importance>> {
			 m_77.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::player_importance>> {
			 m_77.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::player_importance>, tagged_array_view<int8_t, nations::country_tag>> get_row() {
			 return tagged_array_view<int8_t, nations::country_tag>(m_77.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::player_importance>, tagged_array_view<int8_t const, nations::country_tag>> get_row() const {
			 return tagged_array_view<int8_t const, nations::country_tag>(m_77.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_progress>, float&> get(nations::country_tag i) {
			 return m_78.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_progress>, float const&> get(nations::country_tag i) const {
			 return m_78.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::cb_construction_progress>> {
			 m_78.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::cb_construction_progress>> {
			 m_78.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_progress>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_78.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_progress>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_78.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_target>, nations::country_tag&> get(nations::country_tag i) {
			 return m_79.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_target>, nations::country_tag const&> get(nations::country_tag i) const {
			 return m_79.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::cb_construction_target>> {
			 m_79.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::cb_construction_target>> {
			 m_79.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_target>, tagged_array_view<nations::country_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, nations::country_tag>(m_79.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_target>, tagged_array_view<nations::country_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, nations::country_tag>(m_79.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_type>, military::cb_type_tag&> get(nations::country_tag i) {
			 return m_80.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_type>, military::cb_type_tag const&> get(nations::country_tag i) const {
			 return m_80.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::cb_construction_type>> {
			 m_80.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::cb_construction_type>> {
			 m_80.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_type>, tagged_array_view<military::cb_type_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<military::cb_type_tag, nations::country_tag>(m_80.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_type>, tagged_array_view<military::cb_type_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<military::cb_type_tag const, nations::country_tag>(m_80.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_party>, governments::party_tag&> get(nations::country_tag i) {
			 return m_81.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_party>, governments::party_tag const&> get(nations::country_tag i) const {
			 return m_81.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::ruling_party>> {
			 m_81.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::ruling_party>> {
			 m_81.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_party>, tagged_array_view<governments::party_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<governments::party_tag, nations::country_tag>(m_81.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_party>, tagged_array_view<governments::party_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<governments::party_tag const, nations::country_tag>(m_81.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_capital>, provinces::province_tag&> get(nations::country_tag i) {
			 return m_82.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_capital>, provinces::province_tag const&> get(nations::country_tag i) const {
			 return m_82.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_capital>> {
			 m_82.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_capital>> {
			 m_82.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_capital>, tagged_array_view<provinces::province_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<provinces::province_tag, nations::country_tag>(m_82.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_capital>, tagged_array_view<provinces::province_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<provinces::province_tag const, nations::country_tag>(m_82.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tag>, cultures::national_tag&> get(nations::country_tag i) {
			 return m_83.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tag>, cultures::national_tag const&> get(nations::country_tag i) const {
			 return m_83.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::tag>> {
			 m_83.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::tag>> {
			 m_83.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tag>, tagged_array_view<cultures::national_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<cultures::national_tag, nations::country_tag>(m_83.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::tag>, tagged_array_view<cultures::national_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<cultures::national_tag const, nations::country_tag>(m_83.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::primary_culture>, cultures::culture_tag&> get(nations::country_tag i) {
			 return m_84.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::primary_culture>, cultures::culture_tag const&> get(nations::country_tag i) const {
			 return m_84.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::primary_culture>> {
			 m_84.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::primary_culture>> {
			 m_84.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::primary_culture>, tagged_array_view<cultures::culture_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<cultures::culture_tag, nations::country_tag>(m_84.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::primary_culture>, tagged_array_view<cultures::culture_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<cultures::culture_tag const, nations::country_tag>(m_84.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_culture>, cultures::culture_tag&> get(nations::country_tag i) {
			 return m_85.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_culture>, cultures::culture_tag const&> get(nations::country_tag i) const {
			 return m_85.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_culture>> {
			 m_85.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_culture>> {
			 m_85.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_culture>, tagged_array_view<cultures::culture_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<cultures::culture_tag, nations::country_tag>(m_85.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_culture>, tagged_array_view<cultures::culture_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<cultures::culture_tag const, nations::country_tag>(m_85.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_issue>, issues::option_tag&> get(nations::country_tag i) {
			 return m_86.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_issue>, issues::option_tag const&> get(nations::country_tag i) const {
			 return m_86.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_issue>> {
			 m_86.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_issue>> {
			 m_86.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_issue>, tagged_array_view<issues::option_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<issues::option_tag, nations::country_tag>(m_86.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_issue>, tagged_array_view<issues::option_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<issues::option_tag const, nations::country_tag>(m_86.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_ideology>, ideologies::ideology_tag&> get(nations::country_tag i) {
			 return m_87.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_ideology>, ideologies::ideology_tag const&> get(nations::country_tag i) const {
			 return m_87.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_ideology>> {
			 m_87.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_ideology>> {
			 m_87.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_ideology>, tagged_array_view<ideologies::ideology_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<ideologies::ideology_tag, nations::country_tag>(m_87.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_ideology>, tagged_array_view<ideologies::ideology_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<ideologies::ideology_tag const, nations::country_tag>(m_87.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_religion>, cultures::religion_tag&> get(nations::country_tag i) {
			 return m_88.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_religion>, cultures::religion_tag const&> get(nations::country_tag i) const {
			 return m_88.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_religion>> {
			 m_88.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::dominant_religion>> {
			 m_88.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_religion>, tagged_array_view<cultures::religion_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<cultures::religion_tag, nations::country_tag>(m_88.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::dominant_religion>, tagged_array_view<cultures::religion_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<cultures::religion_tag const, nations::country_tag>(m_88.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_religion>, cultures::religion_tag&> get(nations::country_tag i) {
			 return m_89.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_religion>, cultures::religion_tag const&> get(nations::country_tag i) const {
			 return m_89.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_religion>> {
			 m_89.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::national_religion>> {
			 m_89.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_religion>, tagged_array_view<cultures::religion_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<cultures::religion_tag, nations::country_tag>(m_89.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::national_religion>, tagged_array_view<cultures::religion_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<cultures::religion_tag const, nations::country_tag>(m_89.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_government>, governments::government_tag&> get(nations::country_tag i) {
			 return m_90.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_government>, governments::government_tag const&> get(nations::country_tag i) const {
			 return m_90.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_government>> {
			 m_90.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::current_government>> {
			 m_90.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_government>, tagged_array_view<governments::government_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<governments::government_tag, nations::country_tag>(m_90.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::current_government>, tagged_array_view<governments::government_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<governments::government_tag const, nations::country_tag>(m_90.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_ideology>, ideologies::ideology_tag&> get(nations::country_tag i) {
			 return m_91.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_ideology>, ideologies::ideology_tag const&> get(nations::country_tag i) const {
			 return m_91.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::ruling_ideology>> {
			 m_91.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::ruling_ideology>> {
			 m_91.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_ideology>, tagged_array_view<ideologies::ideology_tag, nations::country_tag>> get_row() {
			 return tagged_array_view<ideologies::ideology_tag, nations::country_tag>(m_91.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::ruling_ideology>, tagged_array_view<ideologies::ideology_tag const, nations::country_tag>> get_row() const {
			 return tagged_array_view<ideologies::ideology_tag const, nations::country_tag>(m_91.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_rich_tax>, float&> get(nations::country_tag i) {
			 return m_92.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_rich_tax>, float const&> get(nations::country_tag i) const {
			 return m_92.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_rich_tax>> {
			 m_92.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_rich_tax>> {
			 m_92.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_rich_tax>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_92.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_rich_tax>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_92.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_middle_tax>, float&> get(nations::country_tag i) {
			 return m_93.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_middle_tax>, float const&> get(nations::country_tag i) const {
			 return m_93.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_middle_tax>> {
			 m_93.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_middle_tax>> {
			 m_93.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_middle_tax>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_93.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_middle_tax>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_93.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_poor_tax>, float&> get(nations::country_tag i) {
			 return m_94.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_poor_tax>, float const&> get(nations::country_tag i) const {
			 return m_94.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_poor_tax>> {
			 m_94.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_poor_tax>> {
			 m_94.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_poor_tax>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_94.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_poor_tax>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_94.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_social_spending>, float&> get(nations::country_tag i) {
			 return m_95.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_social_spending>, float const&> get(nations::country_tag i) const {
			 return m_95.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_social_spending>> {
			 m_95.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_social_spending>> {
			 m_95.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_social_spending>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_95.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_social_spending>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_95.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_administrative_spending>, float&> get(nations::country_tag i) {
			 return m_96.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_administrative_spending>, float const&> get(nations::country_tag i) const {
			 return m_96.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_administrative_spending>> {
			 m_96.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_administrative_spending>> {
			 m_96.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_administrative_spending>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_96.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_administrative_spending>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_96.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_education_spending>, float&> get(nations::country_tag i) {
			 return m_97.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_education_spending>, float const&> get(nations::country_tag i) const {
			 return m_97.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_education_spending>> {
			 m_97.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_education_spending>> {
			 m_97.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_education_spending>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_97.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_education_spending>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_97.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_military_spending>, float&> get(nations::country_tag i) {
			 return m_98.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_military_spending>, float const&> get(nations::country_tag i) const {
			 return m_98.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_military_spending>> {
			 m_98.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_military_spending>> {
			 m_98.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_military_spending>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_98.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_military_spending>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_98.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_tariffs>, float&> get(nations::country_tag i) {
			 return m_99.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_tariffs>, float const&> get(nations::country_tag i) const {
			 return m_99.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_tariffs>> {
			 m_99.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_tariffs>> {
			 m_99.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_tariffs>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_99.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_tariffs>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_99.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_army_stockpile_spending>, float&> get(nations::country_tag i) {
			 return m_100.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_army_stockpile_spending>, float const&> get(nations::country_tag i) const {
			 return m_100.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_army_stockpile_spending>> {
			 m_100.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_army_stockpile_spending>> {
			 m_100.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_army_stockpile_spending>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_100.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_army_stockpile_spending>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_100.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_navy_stockpile_spending>, float&> get(nations::country_tag i) {
			 return m_101.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_navy_stockpile_spending>, float const&> get(nations::country_tag i) const {
			 return m_101.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_navy_stockpile_spending>> {
			 m_101.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_navy_stockpile_spending>> {
			 m_101.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_navy_stockpile_spending>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_101.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_navy_stockpile_spending>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_101.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_projects_stockpile_spending>, float&> get(nations::country_tag i) {
			 return m_102.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_projects_stockpile_spending>, float const&> get(nations::country_tag i) const {
			 return m_102.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_projects_stockpile_spending>> {
			 m_102.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::country_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, nation::f_projects_stockpile_spending>> {
			 m_102.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_projects_stockpile_spending>, tagged_array_view<float, nations::country_tag>> get_row() {
			 return tagged_array_view<float, nations::country_tag>(m_102.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::f_projects_stockpile_spending>, tagged_array_view<float const, nations::country_tag>> get_row() const {
			 return tagged_array_view<float const, nations::country_tag>(m_102.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_civilized>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_103.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::is_civilized>> {
			 bit_vector_set(m_103.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_civilized>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_103.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_civilized>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_103.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_substate>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_104.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::is_substate>> {
			 bit_vector_set(m_104.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_substate>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_104.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_substate>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_104.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_mobilized>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_105.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::is_mobilized>> {
			 bit_vector_set(m_105.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_mobilized>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_105.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_mobilized>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_105.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_at_war>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_106.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::is_at_war>> {
			 bit_vector_set(m_106.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_at_war>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_106.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_at_war>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_106.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_not_ai_controlled>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_107.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::is_not_ai_controlled>> {
			 bit_vector_set(m_107.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_not_ai_controlled>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_107.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_not_ai_controlled>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_107.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_holding_election>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_108.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::is_holding_election>> {
			 bit_vector_set(m_108.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_holding_election>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_108.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_holding_election>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_108.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_colonial_nation>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_109.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::is_colonial_nation>> {
			 bit_vector_set(m_109.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_colonial_nation>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_109.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::is_colonial_nation>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_109.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_discovered>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_110.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_discovered>> {
			 bit_vector_set(m_110.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_discovered>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_110.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::cb_construction_discovered>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_110.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_attack>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_111.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_attack>> {
			 bit_vector_set(m_111.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_attack>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_111.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_attack>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_111.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_defence>, bool> get(nations::country_tag i) const {
			 return bit_vector_test(m_112.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::country_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_defence>> {
			 bit_vector_set(m_112.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_defence>, tagged_array_view<bitfield_type, nations::country_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::country_tag>(m_112.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, nation::has_gas_defence>, tagged_array_view<bitfield_type const, nations::country_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::country_tag>(m_112.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }

		 nations::country_tag get_new() {
#ifdef _DEBUG
			 if(!::is_valid_index(first_free))
				 std::abort();
#endif
			 auto allocated = first_free;
			 first_free = m_index.values[to_index(first_free)];
			 m_index.values[to_index(allocated)] = allocated;
			 if(int32_t(to_index(allocated) + 1) > size_used)
				 size_used = int32_t(to_index(allocated) + 1);
			 return allocated;
		 }
		 void resize(int32_t) {}
		 void release(nations::country_tag i) {
			 set<nation::sphere_leader>(i, nations::country_tag());
			 set<nation::overlord>(i, nations::country_tag());
			 set<nation::enabled_crimes>(i, uint64_t());
			 set<nation::current_rules>(i, int32_t());
			 set<nation::total_core_population>(i, float());
			 set<nation::last_election>(i, date_tag());
			 set<nation::last_reform_date>(i, date_tag());
			 set<nation::last_manual_ruling_party_change>(i, date_tag());
			 set<nation::last_lost_war>(i, date_tag());
			 set<nation::disarmed_until>(i, date_tag());
			 set<nation::total_foreign_investment>(i, float());
			 set<nation::treasury>(i, float());
			 set<nation::plurality>(i, float());
			 set<nation::revanchism>(i, float());
			 set<nation::base_prestige>(i, float());
			 set<nation::infamy>(i, float());
			 set<nation::war_exhaustion>(i, float());
			 set<nation::suppression_points>(i, float());
			 set<nation::diplomacy_points>(i, float());
			 set<nation::research_points>(i, float());
			 set<nation::national_debt>(i, float());
			 set<nation::tax_base>(i, float());
			 set<nation::political_interest_fraction>(i, float());
			 set<nation::social_interest_fraction>(i, float());
			 set<nation::national_administrative_efficiency>(i, float());
			 set<nation::social_movement_support>(i, float());
			 set<nation::political_movement_support>(i, float());
			 set<nation::owned_provinces>(i, set_tag<provinces::province_tag>());
			 set<nation::controlled_provinces>(i, set_tag<provinces::province_tag>());
			 set<nation::naval_patrols>(i, set_tag<provinces::province_tag>());
			 set<nation::sphere_members>(i, set_tag<nations::country_tag>());
			 set<nation::vassals>(i, set_tag<nations::country_tag>());
			 set<nation::allies>(i, set_tag<nations::country_tag>());
			 set<nation::neighboring_nations>(i, set_tag<nations::country_tag>());
			 set<nation::accepted_cultures>(i, set_tag<cultures::culture_tag>());
			 set<nation::member_states>(i, set_tag<nations::region_state_pair>());
			 set<nation::gp_influence>(i, set_tag<nations::influence>());
			 set<nation::influencers>(i, set_tag<nations::country_tag>());
			 set<nation::relations>(i, set_tag<nations::relationship>());
			 set<nation::truces>(i, set_tag<nations::truce>());
			 set<nation::national_focus_locations>(i, set_tag<nations::state_tag>());
			 set<nation::national_flags>(i, set_tag<variables::national_flag_tag>());
			 set<nation::static_modifiers>(i, multiset_tag<modifiers::national_modifier_tag>());
			 set<nation::timed_modifiers>(i, multiset_tag<nations::timed_national_modifier>());
			 set<nation::statewise_tariff_mask>(i, array_tag<economy::money_qnty_type, nations::state_tag, true>());
			 set<nation::generals>(i, array_tag<military::leader_tag, int32_t, false>());
			 set<nation::admirals>(i, array_tag<military::leader_tag, int32_t, false>());
			 set<nation::armies>(i, array_tag<military::army_tag, int32_t, false>());
			 set<nation::fleets>(i, array_tag<military::fleet_tag, int32_t, false>());
			 set<nation::active_orders>(i, array_tag<military::army_orders_tag, int32_t, false>());
			 set<nation::strategic_hqs>(i, array_tag<military::strategic_hq_tag, int32_t, false>());
			 set<nation::active_cbs>(i, array_tag<military::pending_cb, int32_t, false>());
			 set<nation::wars_involved_in>(i, set_tag<military::war_identifier>());
			 set<nation::opponents_in_war>(i, set_tag<nations::country_tag>());
			 set<nation::allies_in_war>(i, set_tag<nations::country_tag>());
			 set<nation::name>(i, text_data::text_tag());
			 set<nation::adjective>(i, text_data::text_tag());
			 set<nation::national_value>(i, modifiers::national_modifier_tag());
			 set<nation::tech_school>(i, modifiers::national_modifier_tag());
			 set<nation::flag>(i, graphics::texture_tag());
			 set<nation::current_color>(i, graphics::color_rgb());
			 set<nation::current_research>(i, technologies::tech_tag());
			 set<nation::military_score>(i, int16_t());
			 set<nation::industrial_score>(i, int16_t());
			 set<nation::overall_rank>(i, int16_t());
			 set<nation::prestige_rank>(i, int16_t());
			 set<nation::military_rank>(i, int16_t());
			 set<nation::industrial_rank>(i, int16_t());
			 set<nation::province_count>(i, uint16_t());
			 set<nation::central_province_count>(i, uint16_t());
			 set<nation::rebel_controlled_provinces>(i, uint16_t());
			 set<nation::blockaded_count>(i, uint16_t());
			 set<nation::crime_count>(i, uint16_t());
			 set<nation::leadership_points>(i, int16_t());
			 set<nation::base_colonial_points>(i, int16_t());
			 set<nation::num_connected_ports>(i, uint16_t());
			 set<nation::num_ports>(i, uint16_t());
			 set<nation::player_importance>(i, int8_t());
			 set<nation::cb_construction_progress>(i, float());
			 set<nation::cb_construction_target>(i, nations::country_tag());
			 set<nation::cb_construction_type>(i, military::cb_type_tag());
			 set<nation::ruling_party>(i, governments::party_tag());
			 set<nation::current_capital>(i, provinces::province_tag());
			 set<nation::tag>(i, cultures::national_tag());
			 set<nation::primary_culture>(i, cultures::culture_tag());
			 set<nation::dominant_culture>(i, cultures::culture_tag());
			 set<nation::dominant_issue>(i, issues::option_tag());
			 set<nation::dominant_ideology>(i, ideologies::ideology_tag());
			 set<nation::dominant_religion>(i, cultures::religion_tag());
			 set<nation::national_religion>(i, cultures::religion_tag());
			 set<nation::current_government>(i, governments::government_tag());
			 set<nation::ruling_ideology>(i, ideologies::ideology_tag());
			 set<nation::f_rich_tax>(i, float());
			 set<nation::f_middle_tax>(i, float());
			 set<nation::f_poor_tax>(i, float());
			 set<nation::f_social_spending>(i, float());
			 set<nation::f_administrative_spending>(i, float());
			 set<nation::f_education_spending>(i, float());
			 set<nation::f_military_spending>(i, float());
			 set<nation::f_tariffs>(i, float());
			 set<nation::f_army_stockpile_spending>(i, float());
			 set<nation::f_navy_stockpile_spending>(i, float());
			 set<nation::f_projects_stockpile_spending>(i, float());
			 set<nation::is_civilized>(i, false);
			 set<nation::is_substate>(i, false);
			 set<nation::is_mobilized>(i, false);
			 set<nation::is_at_war>(i, false);
			 set<nation::is_not_ai_controlled>(i, false);
			 set<nation::is_holding_election>(i, false);
			 set<nation::is_colonial_nation>(i, false);
			 set<nation::cb_construction_discovered>(i, false);
			 set<nation::has_gas_attack>(i, false);
			 set<nation::has_gas_defence>(i, false);
			 m_index.values[to_index(i)] = first_free;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == nations::country_tag(nations::country_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(nations::country_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == nations::country_tag(nations::country_tag::value_base_t(i))) f(nations::country_tag(nations::country_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == nations::country_tag(nations::country_tag::value_base_t(i))) f(nations::country_tag(nations::country_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == nations::country_tag(nations::country_tag::value_base_t(i))) { if(f(nations::country_tag(nations::country_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<nation::container, nation::sphere_leader> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, nations::country_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::overlord> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, nations::country_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::enabled_crimes> {
	static constexpr bool value = true;
	 using type = uint64_t&;
	 using const_type = uint64_t const&;
	 using row = tagged_array_view<uint64_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint64_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::current_rules> {
	static constexpr bool value = true;
	 using type = int32_t&;
	 using const_type = int32_t const&;
	 using row = tagged_array_view<int32_t, nations::country_tag>;
	 using const_row = tagged_array_view<int32_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::total_core_population> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::last_election> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, nations::country_tag>;
	 using const_row = tagged_array_view<date_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::last_reform_date> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, nations::country_tag>;
	 using const_row = tagged_array_view<date_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::last_manual_ruling_party_change> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, nations::country_tag>;
	 using const_row = tagged_array_view<date_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::last_lost_war> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, nations::country_tag>;
	 using const_row = tagged_array_view<date_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::disarmed_until> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, nations::country_tag>;
	 using const_row = tagged_array_view<date_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::total_foreign_investment> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::treasury> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::plurality> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::revanchism> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::base_prestige> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::infamy> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::war_exhaustion> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::suppression_points> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::diplomacy_points> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::research_points> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::national_debt> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::tax_base> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::political_interest_fraction> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::social_interest_fraction> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::national_administrative_efficiency> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::social_movement_support> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::political_movement_support> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::owned_provinces> {
	static constexpr bool value = true;
	 using type = set_tag<provinces::province_tag>&;
	 using const_type = set_tag<provinces::province_tag> const&;
	 using row = tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::controlled_provinces> {
	static constexpr bool value = true;
	 using type = set_tag<provinces::province_tag>&;
	 using const_type = set_tag<provinces::province_tag> const&;
	 using row = tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::naval_patrols> {
	static constexpr bool value = true;
	 using type = set_tag<provinces::province_tag>&;
	 using const_type = set_tag<provinces::province_tag> const&;
	 using row = tagged_array_view<set_tag<provinces::province_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<provinces::province_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::sphere_members> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::vassals> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::allies> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::neighboring_nations> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::accepted_cultures> {
	static constexpr bool value = true;
	 using type = set_tag<cultures::culture_tag>&;
	 using const_type = set_tag<cultures::culture_tag> const&;
	 using row = tagged_array_view<set_tag<cultures::culture_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<cultures::culture_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::member_states> {
	static constexpr bool value = true;
	 using type = set_tag<nations::region_state_pair>&;
	 using const_type = set_tag<nations::region_state_pair> const&;
	 using row = tagged_array_view<set_tag<nations::region_state_pair>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::region_state_pair> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::gp_influence> {
	static constexpr bool value = true;
	 using type = set_tag<nations::influence>&;
	 using const_type = set_tag<nations::influence> const&;
	 using row = tagged_array_view<set_tag<nations::influence>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::influence> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::influencers> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::relations> {
	static constexpr bool value = true;
	 using type = set_tag<nations::relationship>&;
	 using const_type = set_tag<nations::relationship> const&;
	 using row = tagged_array_view<set_tag<nations::relationship>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::relationship> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::truces> {
	static constexpr bool value = true;
	 using type = set_tag<nations::truce>&;
	 using const_type = set_tag<nations::truce> const&;
	 using row = tagged_array_view<set_tag<nations::truce>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::truce> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::national_focus_locations> {
	static constexpr bool value = true;
	 using type = set_tag<nations::state_tag>&;
	 using const_type = set_tag<nations::state_tag> const&;
	 using row = tagged_array_view<set_tag<nations::state_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::state_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::national_flags> {
	static constexpr bool value = true;
	 using type = set_tag<variables::national_flag_tag>&;
	 using const_type = set_tag<variables::national_flag_tag> const&;
	 using row = tagged_array_view<set_tag<variables::national_flag_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<variables::national_flag_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::static_modifiers> {
	static constexpr bool value = true;
	 using type = multiset_tag<modifiers::national_modifier_tag>&;
	 using const_type = multiset_tag<modifiers::national_modifier_tag> const&;
	 using row = tagged_array_view<multiset_tag<modifiers::national_modifier_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<multiset_tag<modifiers::national_modifier_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::timed_modifiers> {
	static constexpr bool value = true;
	 using type = multiset_tag<nations::timed_national_modifier>&;
	 using const_type = multiset_tag<nations::timed_national_modifier> const&;
	 using row = tagged_array_view<multiset_tag<nations::timed_national_modifier>, nations::country_tag>;
	 using const_row = tagged_array_view<multiset_tag<nations::timed_national_modifier> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::statewise_tariff_mask> {
	static constexpr bool value = true;
	 using type = array_tag<economy::money_qnty_type, nations::state_tag, true>&;
	 using const_type = array_tag<economy::money_qnty_type, nations::state_tag, true> const&;
	 using row = tagged_array_view<array_tag<economy::money_qnty_type, nations::state_tag, true>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<economy::money_qnty_type, nations::state_tag, true> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::generals> {
	static constexpr bool value = true;
	 using type = array_tag<military::leader_tag, int32_t, false>&;
	 using const_type = array_tag<military::leader_tag, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::leader_tag, int32_t, false>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<military::leader_tag, int32_t, false> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::admirals> {
	static constexpr bool value = true;
	 using type = array_tag<military::leader_tag, int32_t, false>&;
	 using const_type = array_tag<military::leader_tag, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::leader_tag, int32_t, false>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<military::leader_tag, int32_t, false> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::armies> {
	static constexpr bool value = true;
	 using type = array_tag<military::army_tag, int32_t, false>&;
	 using const_type = array_tag<military::army_tag, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::army_tag, int32_t, false>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<military::army_tag, int32_t, false> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::fleets> {
	static constexpr bool value = true;
	 using type = array_tag<military::fleet_tag, int32_t, false>&;
	 using const_type = array_tag<military::fleet_tag, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::fleet_tag, int32_t, false>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<military::fleet_tag, int32_t, false> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::active_orders> {
	static constexpr bool value = true;
	 using type = array_tag<military::army_orders_tag, int32_t, false>&;
	 using const_type = array_tag<military::army_orders_tag, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::army_orders_tag, int32_t, false>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<military::army_orders_tag, int32_t, false> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::strategic_hqs> {
	static constexpr bool value = true;
	 using type = array_tag<military::strategic_hq_tag, int32_t, false>&;
	 using const_type = array_tag<military::strategic_hq_tag, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::strategic_hq_tag, int32_t, false>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<military::strategic_hq_tag, int32_t, false> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::active_cbs> {
	static constexpr bool value = true;
	 using type = array_tag<military::pending_cb, int32_t, false>&;
	 using const_type = array_tag<military::pending_cb, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::pending_cb, int32_t, false>, nations::country_tag>;
	 using const_row = tagged_array_view<array_tag<military::pending_cb, int32_t, false> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::wars_involved_in> {
	static constexpr bool value = true;
	 using type = set_tag<military::war_identifier>&;
	 using const_type = set_tag<military::war_identifier> const&;
	 using row = tagged_array_view<set_tag<military::war_identifier>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<military::war_identifier> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::opponents_in_war> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::allies_in_war> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::country_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::name> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, nations::country_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::adjective> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, nations::country_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::national_value> {
	static constexpr bool value = true;
	 using type = modifiers::national_modifier_tag&;
	 using const_type = modifiers::national_modifier_tag const&;
	 using row = tagged_array_view<modifiers::national_modifier_tag, nations::country_tag>;
	 using const_row = tagged_array_view<modifiers::national_modifier_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::tech_school> {
	static constexpr bool value = true;
	 using type = modifiers::national_modifier_tag&;
	 using const_type = modifiers::national_modifier_tag const&;
	 using row = tagged_array_view<modifiers::national_modifier_tag, nations::country_tag>;
	 using const_row = tagged_array_view<modifiers::national_modifier_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::flag> {
	static constexpr bool value = true;
	 using type = graphics::texture_tag&;
	 using const_type = graphics::texture_tag const&;
	 using row = tagged_array_view<graphics::texture_tag, nations::country_tag>;
	 using const_row = tagged_array_view<graphics::texture_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::current_color> {
	static constexpr bool value = true;
	 using type = graphics::color_rgb&;
	 using const_type = graphics::color_rgb const&;
	 using row = tagged_array_view<graphics::color_rgb, nations::country_tag>;
	 using const_row = tagged_array_view<graphics::color_rgb const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::current_research> {
	static constexpr bool value = true;
	 using type = technologies::tech_tag&;
	 using const_type = technologies::tech_tag const&;
	 using row = tagged_array_view<technologies::tech_tag, nations::country_tag>;
	 using const_row = tagged_array_view<technologies::tech_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::military_score> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::industrial_score> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::overall_rank> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::prestige_rank> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::military_rank> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::industrial_rank> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::province_count> {
	static constexpr bool value = true;
	 using type = uint16_t&;
	 using const_type = uint16_t const&;
	 using row = tagged_array_view<uint16_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::central_province_count> {
	static constexpr bool value = true;
	 using type = uint16_t&;
	 using const_type = uint16_t const&;
	 using row = tagged_array_view<uint16_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::rebel_controlled_provinces> {
	static constexpr bool value = true;
	 using type = uint16_t&;
	 using const_type = uint16_t const&;
	 using row = tagged_array_view<uint16_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::blockaded_count> {
	static constexpr bool value = true;
	 using type = uint16_t&;
	 using const_type = uint16_t const&;
	 using row = tagged_array_view<uint16_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::crime_count> {
	static constexpr bool value = true;
	 using type = uint16_t&;
	 using const_type = uint16_t const&;
	 using row = tagged_array_view<uint16_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::leadership_points> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::base_colonial_points> {
	static constexpr bool value = true;
	 using type = int16_t&;
	 using const_type = int16_t const&;
	 using row = tagged_array_view<int16_t, nations::country_tag>;
	 using const_row = tagged_array_view<int16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::num_connected_ports> {
	static constexpr bool value = true;
	 using type = uint16_t&;
	 using const_type = uint16_t const&;
	 using row = tagged_array_view<uint16_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::num_ports> {
	static constexpr bool value = true;
	 using type = uint16_t&;
	 using const_type = uint16_t const&;
	 using row = tagged_array_view<uint16_t, nations::country_tag>;
	 using const_row = tagged_array_view<uint16_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::player_importance> {
	static constexpr bool value = true;
	 using type = int8_t&;
	 using const_type = int8_t const&;
	 using row = tagged_array_view<int8_t, nations::country_tag>;
	 using const_row = tagged_array_view<int8_t const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::cb_construction_progress> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::cb_construction_target> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, nations::country_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::cb_construction_type> {
	static constexpr bool value = true;
	 using type = military::cb_type_tag&;
	 using const_type = military::cb_type_tag const&;
	 using row = tagged_array_view<military::cb_type_tag, nations::country_tag>;
	 using const_row = tagged_array_view<military::cb_type_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::ruling_party> {
	static constexpr bool value = true;
	 using type = governments::party_tag&;
	 using const_type = governments::party_tag const&;
	 using row = tagged_array_view<governments::party_tag, nations::country_tag>;
	 using const_row = tagged_array_view<governments::party_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::current_capital> {
	static constexpr bool value = true;
	 using type = provinces::province_tag&;
	 using const_type = provinces::province_tag const&;
	 using row = tagged_array_view<provinces::province_tag, nations::country_tag>;
	 using const_row = tagged_array_view<provinces::province_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::tag> {
	static constexpr bool value = true;
	 using type = cultures::national_tag&;
	 using const_type = cultures::national_tag const&;
	 using row = tagged_array_view<cultures::national_tag, nations::country_tag>;
	 using const_row = tagged_array_view<cultures::national_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::primary_culture> {
	static constexpr bool value = true;
	 using type = cultures::culture_tag&;
	 using const_type = cultures::culture_tag const&;
	 using row = tagged_array_view<cultures::culture_tag, nations::country_tag>;
	 using const_row = tagged_array_view<cultures::culture_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::dominant_culture> {
	static constexpr bool value = true;
	 using type = cultures::culture_tag&;
	 using const_type = cultures::culture_tag const&;
	 using row = tagged_array_view<cultures::culture_tag, nations::country_tag>;
	 using const_row = tagged_array_view<cultures::culture_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::dominant_issue> {
	static constexpr bool value = true;
	 using type = issues::option_tag&;
	 using const_type = issues::option_tag const&;
	 using row = tagged_array_view<issues::option_tag, nations::country_tag>;
	 using const_row = tagged_array_view<issues::option_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::dominant_ideology> {
	static constexpr bool value = true;
	 using type = ideologies::ideology_tag&;
	 using const_type = ideologies::ideology_tag const&;
	 using row = tagged_array_view<ideologies::ideology_tag, nations::country_tag>;
	 using const_row = tagged_array_view<ideologies::ideology_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::dominant_religion> {
	static constexpr bool value = true;
	 using type = cultures::religion_tag&;
	 using const_type = cultures::religion_tag const&;
	 using row = tagged_array_view<cultures::religion_tag, nations::country_tag>;
	 using const_row = tagged_array_view<cultures::religion_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::national_religion> {
	static constexpr bool value = true;
	 using type = cultures::religion_tag&;
	 using const_type = cultures::religion_tag const&;
	 using row = tagged_array_view<cultures::religion_tag, nations::country_tag>;
	 using const_row = tagged_array_view<cultures::religion_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::current_government> {
	static constexpr bool value = true;
	 using type = governments::government_tag&;
	 using const_type = governments::government_tag const&;
	 using row = tagged_array_view<governments::government_tag, nations::country_tag>;
	 using const_row = tagged_array_view<governments::government_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::ruling_ideology> {
	static constexpr bool value = true;
	 using type = ideologies::ideology_tag&;
	 using const_type = ideologies::ideology_tag const&;
	 using row = tagged_array_view<ideologies::ideology_tag, nations::country_tag>;
	 using const_row = tagged_array_view<ideologies::ideology_tag const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_rich_tax> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_middle_tax> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_poor_tax> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_social_spending> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_administrative_spending> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_education_spending> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_military_spending> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_tariffs> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_army_stockpile_spending> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_navy_stockpile_spending> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::f_projects_stockpile_spending> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::country_tag>;
	 using const_row = tagged_array_view<float const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::is_civilized> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::is_substate> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::is_mobilized> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::is_at_war> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::is_not_ai_controlled> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::is_holding_election> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::is_colonial_nation> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::cb_construction_discovered> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::has_gas_attack> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};
template<>
struct supports_index<nation::container, nation::has_gas_defence> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::country_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::country_tag>;
};

template<>
class serialization::serializer<nation::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, nation::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == nations::country_tag(nations::country_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = nations::country_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overlord, nations::country_tag>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overlord, nations::country_tag>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_rules, int32_t>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_rules, int32_t>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_core_population, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_core_population, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_election, date_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_election, date_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_reform_date, date_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_reform_date, date_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_lost_war, date_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_lost_war, date_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::disarmed_until, date_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::disarmed_until, date_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_foreign_investment, float>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_foreign_investment, float>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::treasury, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::treasury, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::treasury, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::plurality, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::plurality, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::plurality, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::revanchism, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::revanchism, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::revanchism, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_prestige, float>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_prestige, float>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::infamy, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::infamy, float>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::infamy, float>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::war_exhaustion, float>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::war_exhaustion, float>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::suppression_points, float>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::suppression_points, float>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::diplomacy_points, float>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::diplomacy_points, float>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::research_points, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::research_points, float>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::research_points, float>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_debt, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_debt, float>::serialize_object(output, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_debt, float>::serialize_object(output, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::tax_base, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tax_base, float>::serialize_object(output, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tax_base, float>::serialize_object(output, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_interest_fraction, float>::serialize_object(output, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_interest_fraction, float>::serialize_object(output, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_interest_fraction, float>::serialize_object(output, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_interest_fraction, float>::serialize_object(output, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_administrative_efficiency, float>::serialize_object(output, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_administrative_efficiency, float>::serialize_object(output, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_movement_support, float>::serialize_object(output, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_movement_support, float>::serialize_object(output, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_movement_support, float>::serialize_object(output, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_movement_support, float>::serialize_object(output, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::serialize_object(output, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::serialize_object(output, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::serialize_object(output, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::serialize_object(output, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::serialize_object(output, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::serialize_object(output, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::serialize_object(output, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::serialize_object(output, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::serialize_object(output, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::serialize_object(output, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::serialize_object(output, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::serialize_object(output, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::serialize_object(output, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::serialize_object(output, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::serialize_object(output, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::serialize_object(output, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::serialize_object(output, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::serialize_object(output, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::serialize_object(output, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::serialize_object(output, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::serialize_object(output, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::serialize_object(output, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::serialize_object(output, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::serialize_object(output, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::serialize_object(output, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::serialize_object(output, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::serialize_object(output, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::serialize_object(output, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::serialize_object(output, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::serialize_object(output, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::serialize_object(output, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::serialize_object(output, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_46.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::serialize_object(output, obj.m_46.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_46.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::serialize_object(output, obj.m_46.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_47.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::serialize_object(output, obj.m_47.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_47.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::serialize_object(output, obj.m_47.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_48.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::serialize_object(output, obj.m_48.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_48.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::serialize_object(output, obj.m_48.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_49.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::serialize_object(output, obj.m_49.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_49.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::serialize_object(output, obj.m_49.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_50.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::serialize_object(output, obj.m_50.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_50.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::serialize_object(output, obj.m_50.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_51.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::serialize_object(output, obj.m_51.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_51.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::serialize_object(output, obj.m_51.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_52.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::serialize_object(output, obj.m_52.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_52.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::serialize_object(output, obj.m_52.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_53.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::serialize_object(output, obj.m_53.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_53.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::serialize_object(output, obj.m_53.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_54.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::serialize_object(output, obj.m_54.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_54.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::serialize_object(output, obj.m_54.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_55.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::name, text_data::text_tag>::serialize_object(output, obj.m_55.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_55.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::name, text_data::text_tag>::serialize_object(output, obj.m_55.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_56.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::adjective, text_data::text_tag>::serialize_object(output, obj.m_56.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_56.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::adjective, text_data::text_tag>::serialize_object(output, obj.m_56.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_57.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::serialize_object(output, obj.m_57.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_57.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::serialize_object(output, obj.m_57.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_58.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::serialize_object(output, obj.m_58.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_58.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::serialize_object(output, obj.m_58.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_59.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::flag, graphics::texture_tag>::serialize_object(output, obj.m_59.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_59.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::flag, graphics::texture_tag>::serialize_object(output, obj.m_59.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_60.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::serialize_object(output, obj.m_60.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_60.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::serialize_object(output, obj.m_60.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_61.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::serialize_object(output, obj.m_61.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_61.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::serialize_object(output, obj.m_61.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_62.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_score, int16_t>::serialize_object(output, obj.m_62.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_62.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_score, int16_t>::serialize_object(output, obj.m_62.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_63.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_score, int16_t>::serialize_object(output, obj.m_63.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_63.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_score, int16_t>::serialize_object(output, obj.m_63.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_64.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overall_rank, int16_t>::serialize_object(output, obj.m_64.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_64.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overall_rank, int16_t>::serialize_object(output, obj.m_64.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_65.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::prestige_rank, int16_t>::serialize_object(output, obj.m_65.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_65.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::prestige_rank, int16_t>::serialize_object(output, obj.m_65.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_66.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_rank, int16_t>::serialize_object(output, obj.m_66.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_66.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_rank, int16_t>::serialize_object(output, obj.m_66.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_67.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_rank, int16_t>::serialize_object(output, obj.m_67.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_67.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_rank, int16_t>::serialize_object(output, obj.m_67.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_68.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::province_count, uint16_t>::serialize_object(output, obj.m_68.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_68.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::province_count, uint16_t>::serialize_object(output, obj.m_68.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_69.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::central_province_count, uint16_t>::serialize_object(output, obj.m_69.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_69.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::central_province_count, uint16_t>::serialize_object(output, obj.m_69.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_70.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::serialize_object(output, obj.m_70.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_70.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::serialize_object(output, obj.m_70.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_71.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::blockaded_count, uint16_t>::serialize_object(output, obj.m_71.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_71.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::blockaded_count, uint16_t>::serialize_object(output, obj.m_71.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_72.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::crime_count, uint16_t>::serialize_object(output, obj.m_72.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_72.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::crime_count, uint16_t>::serialize_object(output, obj.m_72.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_73.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::leadership_points, int16_t>::serialize_object(output, obj.m_73.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_73.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::leadership_points, int16_t>::serialize_object(output, obj.m_73.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_74.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_colonial_points, int16_t>::serialize_object(output, obj.m_74.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_74.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_colonial_points, int16_t>::serialize_object(output, obj.m_74.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_75.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::serialize_object(output, obj.m_75.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_75.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::serialize_object(output, obj.m_75.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_76.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_ports, uint16_t>::serialize_object(output, obj.m_76.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_76.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_ports, uint16_t>::serialize_object(output, obj.m_76.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_77.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::player_importance, int8_t>::serialize_object(output, obj.m_77.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_77.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::player_importance, int8_t>::serialize_object(output, obj.m_77.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_78.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_progress, float>::serialize_object(output, obj.m_78.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_78.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_progress, float>::serialize_object(output, obj.m_78.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_79.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::serialize_object(output, obj.m_79.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_79.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::serialize_object(output, obj.m_79.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_80.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::serialize_object(output, obj.m_80.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_80.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::serialize_object(output, obj.m_80.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_81.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::serialize_object(output, obj.m_81.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_81.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::serialize_object(output, obj.m_81.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_82.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::serialize_object(output, obj.m_82.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_82.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::serialize_object(output, obj.m_82.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_83.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tag, cultures::national_tag>::serialize_object(output, obj.m_83.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_83.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tag, cultures::national_tag>::serialize_object(output, obj.m_83.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_84.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::serialize_object(output, obj.m_84.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_84.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::serialize_object(output, obj.m_84.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_85.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::serialize_object(output, obj.m_85.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_85.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::serialize_object(output, obj.m_85.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_86.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::serialize_object(output, obj.m_86.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_86.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::serialize_object(output, obj.m_86.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_87.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_87.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_87.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_87.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_88.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::serialize_object(output, obj.m_88.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_88.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::serialize_object(output, obj.m_88.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_89.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::serialize_object(output, obj.m_89.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_89.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::serialize_object(output, obj.m_89.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_90.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_government, governments::government_tag>::serialize_object(output, obj.m_90.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_90.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_government, governments::government_tag>::serialize_object(output, obj.m_90.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_91.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_91.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_91.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_91.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_92.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_rich_tax, float>::serialize_object(output, obj.m_92.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_92.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_rich_tax, float>::serialize_object(output, obj.m_92.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_93.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_middle_tax, float>::serialize_object(output, obj.m_93.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_93.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_middle_tax, float>::serialize_object(output, obj.m_93.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_94.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_poor_tax, float>::serialize_object(output, obj.m_94.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_94.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_poor_tax, float>::serialize_object(output, obj.m_94.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_95.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_social_spending, float>::serialize_object(output, obj.m_95.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_95.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_social_spending, float>::serialize_object(output, obj.m_95.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_96.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_administrative_spending, float>::serialize_object(output, obj.m_96.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_96.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_administrative_spending, float>::serialize_object(output, obj.m_96.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_97.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_education_spending, float>::serialize_object(output, obj.m_97.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_97.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_education_spending, float>::serialize_object(output, obj.m_97.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_98.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_military_spending, float>::serialize_object(output, obj.m_98.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_98.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_military_spending, float>::serialize_object(output, obj.m_98.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_99.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_tariffs, float>::serialize_object(output, obj.m_99.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_99.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_tariffs, float>::serialize_object(output, obj.m_99.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_100.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::serialize_object(output, obj.m_100.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_100.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::serialize_object(output, obj.m_100.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_101.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::serialize_object(output, obj.m_101.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_101.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::serialize_object(output, obj.m_101.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_102.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::serialize_object(output, obj.m_102.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_102.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::serialize_object(output, obj.m_102.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		 serialization::serialize_array(output, obj.m_103.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_104.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_105.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_106.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_107.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_108.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_109.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_110.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_111.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_112.values, uint32_t(obj.size_used + 7) / 8ui32);
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, nation::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = nations::country_tag();
		for(int32_t i = 400 - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = nations::country_tag(nations::country_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = nations::country_tag(nations::country_tag::value_base_t(i));
			}
		}
		if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overlord, nations::country_tag>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overlord, nations::country_tag>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_rules, int32_t>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_rules, int32_t>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_core_population, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_core_population, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_election, date_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_election, date_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_reform_date, date_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_reform_date, date_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_lost_war, date_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::last_lost_war, date_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::disarmed_until, date_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::disarmed_until, date_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_foreign_investment, float>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::total_foreign_investment, float>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::treasury, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::treasury, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::treasury, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::plurality, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::plurality, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::plurality, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::revanchism, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::revanchism, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::revanchism, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_prestige, float>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_prestige, float>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::infamy, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::infamy, float>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::infamy, float>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::war_exhaustion, float>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::war_exhaustion, float>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::suppression_points, float>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::suppression_points, float>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::diplomacy_points, float>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::diplomacy_points, float>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::research_points, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::research_points, float>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::research_points, float>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_debt, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_debt, float>::deserialize_object(input, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_debt, float>::deserialize_object(input, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::tax_base, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tax_base, float>::deserialize_object(input, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tax_base, float>::deserialize_object(input, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_interest_fraction, float>::deserialize_object(input, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_interest_fraction, float>::deserialize_object(input, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_interest_fraction, float>::deserialize_object(input, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_interest_fraction, float>::deserialize_object(input, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_administrative_efficiency, float>::deserialize_object(input, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_administrative_efficiency, float>::deserialize_object(input, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_movement_support, float>::deserialize_object(input, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::social_movement_support, float>::deserialize_object(input, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_movement_support, float>::deserialize_object(input, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::political_movement_support, float>::deserialize_object(input, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::deserialize_object(input, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::deserialize_object(input, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::deserialize_object(input, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::deserialize_object(input, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::deserialize_object(input, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::deserialize_object(input, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::deserialize_object(input, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::deserialize_object(input, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::deserialize_object(input, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::deserialize_object(input, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::deserialize_object(input, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::deserialize_object(input, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::deserialize_object(input, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::deserialize_object(input, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::deserialize_object(input, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::deserialize_object(input, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::deserialize_object(input, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::deserialize_object(input, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::deserialize_object(input, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::deserialize_object(input, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::deserialize_object(input, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::deserialize_object(input, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_46.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::deserialize_object(input, obj.m_46.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_46.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::deserialize_object(input, obj.m_46.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_47.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::deserialize_object(input, obj.m_47.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_47.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::deserialize_object(input, obj.m_47.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_48.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::deserialize_object(input, obj.m_48.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_48.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::deserialize_object(input, obj.m_48.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_49.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::deserialize_object(input, obj.m_49.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_49.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::deserialize_object(input, obj.m_49.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_50.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::deserialize_object(input, obj.m_50.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_50.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::deserialize_object(input, obj.m_50.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_51.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::deserialize_object(input, obj.m_51.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_51.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::deserialize_object(input, obj.m_51.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_52.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::deserialize_object(input, obj.m_52.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_52.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::deserialize_object(input, obj.m_52.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_53.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_53.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_53.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_53.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_54.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_54.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_54.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_54.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_55.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::name, text_data::text_tag>::deserialize_object(input, obj.m_55.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_55.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::name, text_data::text_tag>::deserialize_object(input, obj.m_55.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_56.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::adjective, text_data::text_tag>::deserialize_object(input, obj.m_56.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_56.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::adjective, text_data::text_tag>::deserialize_object(input, obj.m_56.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_57.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::deserialize_object(input, obj.m_57.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_57.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::deserialize_object(input, obj.m_57.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_58.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::deserialize_object(input, obj.m_58.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_58.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::deserialize_object(input, obj.m_58.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_59.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::flag, graphics::texture_tag>::deserialize_object(input, obj.m_59.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_59.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::flag, graphics::texture_tag>::deserialize_object(input, obj.m_59.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_60.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::deserialize_object(input, obj.m_60.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_60.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::deserialize_object(input, obj.m_60.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_61.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::deserialize_object(input, obj.m_61.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_61.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::deserialize_object(input, obj.m_61.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_62.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_score, int16_t>::deserialize_object(input, obj.m_62.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_62.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_score, int16_t>::deserialize_object(input, obj.m_62.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_63.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_score, int16_t>::deserialize_object(input, obj.m_63.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_63.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_score, int16_t>::deserialize_object(input, obj.m_63.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_64.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overall_rank, int16_t>::deserialize_object(input, obj.m_64.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_64.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::overall_rank, int16_t>::deserialize_object(input, obj.m_64.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_65.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::prestige_rank, int16_t>::deserialize_object(input, obj.m_65.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_65.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::prestige_rank, int16_t>::deserialize_object(input, obj.m_65.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_66.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_rank, int16_t>::deserialize_object(input, obj.m_66.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_66.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::military_rank, int16_t>::deserialize_object(input, obj.m_66.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_67.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_rank, int16_t>::deserialize_object(input, obj.m_67.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_67.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::industrial_rank, int16_t>::deserialize_object(input, obj.m_67.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_68.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::province_count, uint16_t>::deserialize_object(input, obj.m_68.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_68.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::province_count, uint16_t>::deserialize_object(input, obj.m_68.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_69.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::central_province_count, uint16_t>::deserialize_object(input, obj.m_69.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_69.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::central_province_count, uint16_t>::deserialize_object(input, obj.m_69.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_70.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::deserialize_object(input, obj.m_70.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_70.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::deserialize_object(input, obj.m_70.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_71.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::blockaded_count, uint16_t>::deserialize_object(input, obj.m_71.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_71.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::blockaded_count, uint16_t>::deserialize_object(input, obj.m_71.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_72.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::crime_count, uint16_t>::deserialize_object(input, obj.m_72.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_72.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::crime_count, uint16_t>::deserialize_object(input, obj.m_72.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_73.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::leadership_points, int16_t>::deserialize_object(input, obj.m_73.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_73.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::leadership_points, int16_t>::deserialize_object(input, obj.m_73.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_74.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_colonial_points, int16_t>::deserialize_object(input, obj.m_74.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_74.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::base_colonial_points, int16_t>::deserialize_object(input, obj.m_74.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_75.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::deserialize_object(input, obj.m_75.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_75.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::deserialize_object(input, obj.m_75.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_76.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_ports, uint16_t>::deserialize_object(input, obj.m_76.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_76.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::num_ports, uint16_t>::deserialize_object(input, obj.m_76.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_77.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::player_importance, int8_t>::deserialize_object(input, obj.m_77.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_77.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::player_importance, int8_t>::deserialize_object(input, obj.m_77.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_78.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_progress, float>::deserialize_object(input, obj.m_78.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_78.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_progress, float>::deserialize_object(input, obj.m_78.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_79.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::deserialize_object(input, obj.m_79.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_79.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::deserialize_object(input, obj.m_79.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_80.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::deserialize_object(input, obj.m_80.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_80.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::deserialize_object(input, obj.m_80.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_81.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::deserialize_object(input, obj.m_81.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_81.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::deserialize_object(input, obj.m_81.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_82.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::deserialize_object(input, obj.m_82.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_82.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::deserialize_object(input, obj.m_82.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_83.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tag, cultures::national_tag>::deserialize_object(input, obj.m_83.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_83.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::tag, cultures::national_tag>::deserialize_object(input, obj.m_83.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_84.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::deserialize_object(input, obj.m_84.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_84.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::deserialize_object(input, obj.m_84.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_85.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::deserialize_object(input, obj.m_85.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_85.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::deserialize_object(input, obj.m_85.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_86.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::deserialize_object(input, obj.m_86.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_86.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::deserialize_object(input, obj.m_86.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_87.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_87.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_87.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_87.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_88.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::deserialize_object(input, obj.m_88.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_88.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::deserialize_object(input, obj.m_88.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_89.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::deserialize_object(input, obj.m_89.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_89.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::deserialize_object(input, obj.m_89.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_90.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_government, governments::government_tag>::deserialize_object(input, obj.m_90.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_90.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::current_government, governments::government_tag>::deserialize_object(input, obj.m_90.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_91.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_91.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_91.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_91.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_92.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_rich_tax, float>::deserialize_object(input, obj.m_92.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_92.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_rich_tax, float>::deserialize_object(input, obj.m_92.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_93.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_middle_tax, float>::deserialize_object(input, obj.m_93.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_93.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_middle_tax, float>::deserialize_object(input, obj.m_93.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_94.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_poor_tax, float>::deserialize_object(input, obj.m_94.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_94.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_poor_tax, float>::deserialize_object(input, obj.m_94.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_95.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_social_spending, float>::deserialize_object(input, obj.m_95.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_95.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_social_spending, float>::deserialize_object(input, obj.m_95.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_96.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_administrative_spending, float>::deserialize_object(input, obj.m_96.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_96.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_administrative_spending, float>::deserialize_object(input, obj.m_96.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_97.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_education_spending, float>::deserialize_object(input, obj.m_97.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_97.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_education_spending, float>::deserialize_object(input, obj.m_97.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_98.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_military_spending, float>::deserialize_object(input, obj.m_98.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_98.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_military_spending, float>::deserialize_object(input, obj.m_98.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_99.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_tariffs, float>::deserialize_object(input, obj.m_99.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_99.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_tariffs, float>::deserialize_object(input, obj.m_99.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_100.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::deserialize_object(input, obj.m_100.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_100.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::deserialize_object(input, obj.m_100.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_101.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::deserialize_object(input, obj.m_101.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_101.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::deserialize_object(input, obj.m_101.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_102.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::deserialize_object(input, obj.m_102.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_102.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::deserialize_object(input, obj.m_102.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		 serialization::deserialize_array(input, obj.m_103.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_104.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_105.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_106.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_107.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_108.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_109.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_110.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_111.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_112.values, uint32_t(obj.size_used + 7) / 8ui32);
	 }
	 template<typename ... CONTEXT>
	 static size_t size(nation::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(nations::country_tag) * obj.size_used+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::size(obj.m_0.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_0.values, obj.m_0.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<nation::sphere_leader, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::overlord, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::overlord, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::overlord, nations::country_tag>::size(obj.m_1.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_1.values, obj.m_1.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<nation::overlord, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::size(obj.m_2.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_2.values, obj.m_2.values + max, 0ui64, std::plus<>(), [&](uint64_t const& m) {
					 return serialization::tagged_serializer<nation::enabled_crimes, uint64_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::current_rules, int32_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::current_rules, int32_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::current_rules, int32_t>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](int32_t const& m) {
					 return serialization::tagged_serializer<nation::current_rules, int32_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::total_core_population, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::total_core_population, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::total_core_population, float>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::total_core_population, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::last_election, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::last_election, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::last_election, date_tag>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<nation::last_election, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::last_reform_date, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::last_reform_date, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::last_reform_date, date_tag>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<nation::last_reform_date, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<nation::last_manual_ruling_party_change, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::last_lost_war, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::last_lost_war, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::last_lost_war, date_tag>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<nation::last_lost_war, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::disarmed_until, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::disarmed_until, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::disarmed_until, date_tag>::size(obj.m_9.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_9.values, obj.m_9.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<nation::disarmed_until, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::total_foreign_investment, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::total_foreign_investment, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::total_foreign_investment, float>::size(obj.m_10.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_10.values, obj.m_10.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::total_foreign_investment, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::treasury, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::treasury, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::treasury, float>::size(obj.m_11.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_11.values, obj.m_11.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::treasury, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::plurality, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::plurality, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::plurality, float>::size(obj.m_12.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_12.values, obj.m_12.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::plurality, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::revanchism, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::revanchism, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::revanchism, float>::size(obj.m_13.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_13.values, obj.m_13.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::revanchism, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::base_prestige, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::base_prestige, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::base_prestige, float>::size(obj.m_14.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_14.values, obj.m_14.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::base_prestige, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::infamy, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::infamy, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::infamy, float>::size(obj.m_15.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_15.values, obj.m_15.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::infamy, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::war_exhaustion, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::war_exhaustion, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::war_exhaustion, float>::size(obj.m_16.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_16.values, obj.m_16.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::war_exhaustion, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::suppression_points, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::suppression_points, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::suppression_points, float>::size(obj.m_17.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_17.values, obj.m_17.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::suppression_points, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::diplomacy_points, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::diplomacy_points, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::diplomacy_points, float>::size(obj.m_18.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_18.values, obj.m_18.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::diplomacy_points, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::research_points, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::research_points, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::research_points, float>::size(obj.m_19.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_19.values, obj.m_19.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::research_points, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::national_debt, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::national_debt, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::national_debt, float>::size(obj.m_20.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_20.values, obj.m_20.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::national_debt, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::tax_base, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::tax_base, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::tax_base, float>::size(obj.m_21.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_21.values, obj.m_21.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::tax_base, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::political_interest_fraction, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::political_interest_fraction, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::political_interest_fraction, float>::size(obj.m_22.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_22.values, obj.m_22.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::political_interest_fraction, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::social_interest_fraction, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::social_interest_fraction, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::social_interest_fraction, float>::size(obj.m_23.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_23.values, obj.m_23.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::social_interest_fraction, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::national_administrative_efficiency, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::national_administrative_efficiency, float>::size(obj.m_24.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_24.values, obj.m_24.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::national_administrative_efficiency, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::social_movement_support, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::social_movement_support, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::social_movement_support, float>::size(obj.m_25.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_25.values, obj.m_25.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::social_movement_support, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::political_movement_support, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::political_movement_support, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::political_movement_support, float>::size(obj.m_26.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_26.values, obj.m_26.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::political_movement_support, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::size(obj.m_27.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_27.values, obj.m_27.values + max, 0ui64, std::plus<>(), [&](set_tag<provinces::province_tag> const& m) {
					 return serialization::tagged_serializer<nation::owned_provinces, set_tag<provinces::province_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::size(obj.m_28.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_28.values, obj.m_28.values + max, 0ui64, std::plus<>(), [&](set_tag<provinces::province_tag> const& m) {
					 return serialization::tagged_serializer<nation::controlled_provinces, set_tag<provinces::province_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::size(obj.m_29.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_29.values, obj.m_29.values + max, 0ui64, std::plus<>(), [&](set_tag<provinces::province_tag> const& m) {
					 return serialization::tagged_serializer<nation::naval_patrols, set_tag<provinces::province_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::size(obj.m_30.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_30.values, obj.m_30.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<nation::sphere_members, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::size(obj.m_31.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_31.values, obj.m_31.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<nation::vassals, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::size(obj.m_32.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_32.values, obj.m_32.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<nation::allies, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::size(obj.m_33.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_33.values, obj.m_33.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<nation::neighboring_nations, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::size(obj.m_34.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_34.values, obj.m_34.values + max, 0ui64, std::plus<>(), [&](set_tag<cultures::culture_tag> const& m) {
					 return serialization::tagged_serializer<nation::accepted_cultures, set_tag<cultures::culture_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::size(obj.m_35.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_35.values, obj.m_35.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::region_state_pair> const& m) {
					 return serialization::tagged_serializer<nation::member_states, set_tag<nations::region_state_pair>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::size(obj.m_36.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_36.values, obj.m_36.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::influence> const& m) {
					 return serialization::tagged_serializer<nation::gp_influence, set_tag<nations::influence>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::size(obj.m_37.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_37.values, obj.m_37.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<nation::influencers, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::size(obj.m_38.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_38.values, obj.m_38.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::relationship> const& m) {
					 return serialization::tagged_serializer<nation::relations, set_tag<nations::relationship>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::size(obj.m_39.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_39.values, obj.m_39.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::truce> const& m) {
					 return serialization::tagged_serializer<nation::truces, set_tag<nations::truce>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::size(obj.m_40.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_40.values, obj.m_40.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::state_tag> const& m) {
					 return serialization::tagged_serializer<nation::national_focus_locations, set_tag<nations::state_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::size(obj.m_41.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_41.values, obj.m_41.values + max, 0ui64, std::plus<>(), [&](set_tag<variables::national_flag_tag> const& m) {
					 return serialization::tagged_serializer<nation::national_flags, set_tag<variables::national_flag_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::size(obj.m_42.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_42.values, obj.m_42.values + max, 0ui64, std::plus<>(), [&](multiset_tag<modifiers::national_modifier_tag> const& m) {
					 return serialization::tagged_serializer<nation::static_modifiers, multiset_tag<modifiers::national_modifier_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::size(obj.m_43.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_43.values, obj.m_43.values + max, 0ui64, std::plus<>(), [&](multiset_tag<nations::timed_national_modifier> const& m) {
					 return serialization::tagged_serializer<nation::timed_modifiers, multiset_tag<nations::timed_national_modifier>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::size(obj.m_44.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_44.values, obj.m_44.values + max, 0ui64, std::plus<>(), [&](array_tag<economy::money_qnty_type, nations::state_tag, true> const& m) {
					 return serialization::tagged_serializer<nation::statewise_tariff_mask, array_tag<economy::money_qnty_type, nations::state_tag, true>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::size(obj.m_45.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_45.values, obj.m_45.values + max, 0ui64, std::plus<>(), [&](array_tag<military::leader_tag, int32_t, false> const& m) {
					 return serialization::tagged_serializer<nation::generals, array_tag<military::leader_tag, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::size(obj.m_46.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_46.values, obj.m_46.values + max, 0ui64, std::plus<>(), [&](array_tag<military::leader_tag, int32_t, false> const& m) {
					 return serialization::tagged_serializer<nation::admirals, array_tag<military::leader_tag, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::size(obj.m_47.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_47.values, obj.m_47.values + max, 0ui64, std::plus<>(), [&](array_tag<military::army_tag, int32_t, false> const& m) {
					 return serialization::tagged_serializer<nation::armies, array_tag<military::army_tag, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::size(obj.m_48.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_48.values, obj.m_48.values + max, 0ui64, std::plus<>(), [&](array_tag<military::fleet_tag, int32_t, false> const& m) {
					 return serialization::tagged_serializer<nation::fleets, array_tag<military::fleet_tag, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::size(obj.m_49.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_49.values, obj.m_49.values + max, 0ui64, std::plus<>(), [&](array_tag<military::army_orders_tag, int32_t, false> const& m) {
					 return serialization::tagged_serializer<nation::active_orders, array_tag<military::army_orders_tag, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::size(obj.m_50.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_50.values, obj.m_50.values + max, 0ui64, std::plus<>(), [&](array_tag<military::strategic_hq_tag, int32_t, false> const& m) {
					 return serialization::tagged_serializer<nation::strategic_hqs, array_tag<military::strategic_hq_tag, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::size(obj.m_51.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_51.values, obj.m_51.values + max, 0ui64, std::plus<>(), [&](array_tag<military::pending_cb, int32_t, false> const& m) {
					 return serialization::tagged_serializer<nation::active_cbs, array_tag<military::pending_cb, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::size(obj.m_52.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_52.values, obj.m_52.values + max, 0ui64, std::plus<>(), [&](set_tag<military::war_identifier> const& m) {
					 return serialization::tagged_serializer<nation::wars_involved_in, set_tag<military::war_identifier>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::size(obj.m_53.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_53.values, obj.m_53.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<nation::opponents_in_war, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::size(obj.m_54.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_54.values, obj.m_54.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<nation::allies_in_war, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::name, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::name, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::name, text_data::text_tag>::size(obj.m_55.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_55.values, obj.m_55.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<nation::name, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::adjective, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::adjective, text_data::text_tag>::size(obj.m_56.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_56.values, obj.m_56.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<nation::adjective, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::size(obj.m_57.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_57.values, obj.m_57.values + max, 0ui64, std::plus<>(), [&](modifiers::national_modifier_tag const& m) {
					 return serialization::tagged_serializer<nation::national_value, modifiers::national_modifier_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::size(obj.m_58.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_58.values, obj.m_58.values + max, 0ui64, std::plus<>(), [&](modifiers::national_modifier_tag const& m) {
					 return serialization::tagged_serializer<nation::tech_school, modifiers::national_modifier_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::flag, graphics::texture_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::flag, graphics::texture_tag>::size(obj.m_59.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_59.values, obj.m_59.values + max, 0ui64, std::plus<>(), [&](graphics::texture_tag const& m) {
					 return serialization::tagged_serializer<nation::flag, graphics::texture_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::size(obj.m_60.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_60.values, obj.m_60.values + max, 0ui64, std::plus<>(), [&](graphics::color_rgb const& m) {
					 return serialization::tagged_serializer<nation::current_color, graphics::color_rgb>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::size(obj.m_61.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_61.values, obj.m_61.values + max, 0ui64, std::plus<>(), [&](technologies::tech_tag const& m) {
					 return serialization::tagged_serializer<nation::current_research, technologies::tech_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::military_score, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::military_score, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::military_score, int16_t>::size(obj.m_62.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_62.values, obj.m_62.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::military_score, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::industrial_score, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::industrial_score, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::industrial_score, int16_t>::size(obj.m_63.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_63.values, obj.m_63.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::industrial_score, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::overall_rank, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::overall_rank, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::overall_rank, int16_t>::size(obj.m_64.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_64.values, obj.m_64.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::overall_rank, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::prestige_rank, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::prestige_rank, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::prestige_rank, int16_t>::size(obj.m_65.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_65.values, obj.m_65.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::prestige_rank, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::military_rank, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::military_rank, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::military_rank, int16_t>::size(obj.m_66.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_66.values, obj.m_66.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::military_rank, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::industrial_rank, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::industrial_rank, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::industrial_rank, int16_t>::size(obj.m_67.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_67.values, obj.m_67.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::industrial_rank, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::province_count, uint16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::province_count, uint16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::province_count, uint16_t>::size(obj.m_68.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_68.values, obj.m_68.values + max, 0ui64, std::plus<>(), [&](uint16_t const& m) {
					 return serialization::tagged_serializer<nation::province_count, uint16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::central_province_count, uint16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::central_province_count, uint16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::central_province_count, uint16_t>::size(obj.m_69.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_69.values, obj.m_69.values + max, 0ui64, std::plus<>(), [&](uint16_t const& m) {
					 return serialization::tagged_serializer<nation::central_province_count, uint16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::size(obj.m_70.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_70.values, obj.m_70.values + max, 0ui64, std::plus<>(), [&](uint16_t const& m) {
					 return serialization::tagged_serializer<nation::rebel_controlled_provinces, uint16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::blockaded_count, uint16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::blockaded_count, uint16_t>::size(obj.m_71.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_71.values, obj.m_71.values + max, 0ui64, std::plus<>(), [&](uint16_t const& m) {
					 return serialization::tagged_serializer<nation::blockaded_count, uint16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::crime_count, uint16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::crime_count, uint16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::crime_count, uint16_t>::size(obj.m_72.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_72.values, obj.m_72.values + max, 0ui64, std::plus<>(), [&](uint16_t const& m) {
					 return serialization::tagged_serializer<nation::crime_count, uint16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::leadership_points, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::leadership_points, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::leadership_points, int16_t>::size(obj.m_73.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_73.values, obj.m_73.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::leadership_points, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::base_colonial_points, int16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::base_colonial_points, int16_t>::size(obj.m_74.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_74.values, obj.m_74.values + max, 0ui64, std::plus<>(), [&](int16_t const& m) {
					 return serialization::tagged_serializer<nation::base_colonial_points, int16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::size(obj.m_75.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_75.values, obj.m_75.values + max, 0ui64, std::plus<>(), [&](uint16_t const& m) {
					 return serialization::tagged_serializer<nation::num_connected_ports, uint16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::num_ports, uint16_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::num_ports, uint16_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::num_ports, uint16_t>::size(obj.m_76.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_76.values, obj.m_76.values + max, 0ui64, std::plus<>(), [&](uint16_t const& m) {
					 return serialization::tagged_serializer<nation::num_ports, uint16_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::player_importance, int8_t>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::player_importance, int8_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::player_importance, int8_t>::size(obj.m_77.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_77.values, obj.m_77.values + max, 0ui64, std::plus<>(), [&](int8_t const& m) {
					 return serialization::tagged_serializer<nation::player_importance, int8_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::cb_construction_progress, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_progress, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::cb_construction_progress, float>::size(obj.m_78.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_78.values, obj.m_78.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::cb_construction_progress, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::size(obj.m_79.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_79.values, obj.m_79.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<nation::cb_construction_target, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::size(obj.m_80.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_80.values, obj.m_80.values + max, 0ui64, std::plus<>(), [&](military::cb_type_tag const& m) {
					 return serialization::tagged_serializer<nation::cb_construction_type, military::cb_type_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::size(obj.m_81.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_81.values, obj.m_81.values + max, 0ui64, std::plus<>(), [&](governments::party_tag const& m) {
					 return serialization::tagged_serializer<nation::ruling_party, governments::party_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::size(obj.m_82.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_82.values, obj.m_82.values + max, 0ui64, std::plus<>(), [&](provinces::province_tag const& m) {
					 return serialization::tagged_serializer<nation::current_capital, provinces::province_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::tag, cultures::national_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::tag, cultures::national_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::tag, cultures::national_tag>::size(obj.m_83.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_83.values, obj.m_83.values + max, 0ui64, std::plus<>(), [&](cultures::national_tag const& m) {
					 return serialization::tagged_serializer<nation::tag, cultures::national_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::size(obj.m_84.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_84.values, obj.m_84.values + max, 0ui64, std::plus<>(), [&](cultures::culture_tag const& m) {
					 return serialization::tagged_serializer<nation::primary_culture, cultures::culture_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::size(obj.m_85.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_85.values, obj.m_85.values + max, 0ui64, std::plus<>(), [&](cultures::culture_tag const& m) {
					 return serialization::tagged_serializer<nation::dominant_culture, cultures::culture_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::size(obj.m_86.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_86.values, obj.m_86.values + max, 0ui64, std::plus<>(), [&](issues::option_tag const& m) {
					 return serialization::tagged_serializer<nation::dominant_issue, issues::option_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::size(obj.m_87.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_87.values, obj.m_87.values + max, 0ui64, std::plus<>(), [&](ideologies::ideology_tag const& m) {
					 return serialization::tagged_serializer<nation::dominant_ideology, ideologies::ideology_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::size(obj.m_88.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_88.values, obj.m_88.values + max, 0ui64, std::plus<>(), [&](cultures::religion_tag const& m) {
					 return serialization::tagged_serializer<nation::dominant_religion, cultures::religion_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::size(obj.m_89.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_89.values, obj.m_89.values + max, 0ui64, std::plus<>(), [&](cultures::religion_tag const& m) {
					 return serialization::tagged_serializer<nation::national_religion, cultures::religion_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::current_government, governments::government_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::current_government, governments::government_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::current_government, governments::government_tag>::size(obj.m_90.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_90.values, obj.m_90.values + max, 0ui64, std::plus<>(), [&](governments::government_tag const& m) {
					 return serialization::tagged_serializer<nation::current_government, governments::government_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::size(obj.m_91.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_91.values, obj.m_91.values + max, 0ui64, std::plus<>(), [&](ideologies::ideology_tag const& m) {
					 return serialization::tagged_serializer<nation::ruling_ideology, ideologies::ideology_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_rich_tax, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_rich_tax, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_rich_tax, float>::size(obj.m_92.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_92.values, obj.m_92.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_rich_tax, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_middle_tax, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_middle_tax, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_middle_tax, float>::size(obj.m_93.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_93.values, obj.m_93.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_middle_tax, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_poor_tax, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_poor_tax, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_poor_tax, float>::size(obj.m_94.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_94.values, obj.m_94.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_poor_tax, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_social_spending, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_social_spending, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_social_spending, float>::size(obj.m_95.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_95.values, obj.m_95.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_social_spending, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_administrative_spending, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_administrative_spending, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_administrative_spending, float>::size(obj.m_96.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_96.values, obj.m_96.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_administrative_spending, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_education_spending, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_education_spending, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_education_spending, float>::size(obj.m_97.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_97.values, obj.m_97.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_education_spending, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_military_spending, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_military_spending, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_military_spending, float>::size(obj.m_98.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_98.values, obj.m_98.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_military_spending, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_tariffs, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_tariffs, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_tariffs, float>::size(obj.m_99.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_99.values, obj.m_99.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_tariffs, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::size(obj.m_100.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_100.values, obj.m_100.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_army_stockpile_spending, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::size(obj.m_101.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_101.values, obj.m_101.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_navy_stockpile_spending, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::size());
			 } else if constexpr(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::size(obj.m_102.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_102.values, obj.m_102.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<nation::f_projects_stockpile_spending, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32));
	 }
};
#pragma warning( pop )

