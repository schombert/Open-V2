#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace province_state {
	class container;
}

template<>
class serialization::serializer<province_state::container>;

namespace province_state {
	struct is_blockaded;
	struct is_overseas;
	struct has_owner_core;
	struct owner_building_railroad;
	struct is_non_state;
	struct fort_level;
	struct railroad_level;
	struct naval_base_level;
	struct rgo_size;
	struct dominant_religion;
	struct dominant_ideology;
	struct dominant_issue;
	struct artisan_production;
	struct rgo_production;
	struct dominant_culture;
	struct base_life_rating;
	struct terrain;
	struct crime;
	struct name;
	struct timed_modifiers;
	struct static_modifiers;
	struct fleets;
	struct pops;
	struct cores;
	struct rgo_worker_data;
	struct total_population;
	struct last_controller_change;
	struct last_immigration;
	struct nationalism;
	struct siege_progress;
	struct fort_upgrade_progress;
	struct railroad_upgrade_progress;
	struct naval_base_upgrade_progress;
	struct artisan_production_scale;
	struct net_migration_growth;
	struct net_immigration_growth;
	struct monthly_population;
	struct old_migration_growth;
	struct old_immigration_growth;
	struct old_monthly_population;
	struct owner;
	struct controller;
	struct rebel_controller;
	struct state_instance;
	struct orders;
	struct strat_hq;

	class alignas(64) container {
		 int32_t size_used = 0;


		 struct alignas(64) dtype_0 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_0() { std::fill_n(values - 1, 1 + ((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_1() { std::fill_n(values - 1, 1 + ((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_2() { std::fill_n(values - 1, 1 + ((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_3() { std::fill_n(values - 1, 1 + ((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_4() { std::fill_n(values - 1, 1 + ((uint32_t(province_state::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(uint8_t) + 63ui32) & ~63ui32]; 
			 uint8_t values[(sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(uint8_t) + 63ui32) & ~63ui32]; 
			 uint8_t values[(sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(uint8_t) + 63ui32) & ~63ui32]; 
			 uint8_t values[(sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(uint8_t) + 63ui32) & ~63ui32]; 
			 uint8_t values[(sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(uint8_t) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(uint8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(uint8_t)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_8;
		 struct alignas(64) dtype_9 { 
			 uint8_t padding[(sizeof(cultures::religion_tag) + 63ui32) & ~63ui32]; 
			 cultures::religion_tag values[(sizeof(cultures::religion_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_9() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::religion_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_9;
		 struct alignas(64) dtype_10 { 
			 uint8_t padding[(sizeof(ideologies::ideology_tag) + 63ui32) & ~63ui32]; 
			 ideologies::ideology_tag values[(sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_10() { std::uninitialized_value_construct_n(values - 1, (sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_10;
		 struct alignas(64) dtype_11 { 
			 uint8_t padding[(sizeof(issues::option_tag) + 63ui32) & ~63ui32]; 
			 issues::option_tag values[(sizeof(issues::option_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(issues::option_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(issues::option_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_11() { std::uninitialized_value_construct_n(values - 1, (sizeof(issues::option_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(issues::option_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(issues::option_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_11;
		 struct alignas(64) dtype_12 { 
			 uint8_t padding[(sizeof(economy::goods_tag) + 63ui32) & ~63ui32]; 
			 economy::goods_tag values[(sizeof(economy::goods_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(economy::goods_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(economy::goods_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_12() { std::uninitialized_value_construct_n(values - 1, (sizeof(economy::goods_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(economy::goods_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(economy::goods_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_12;
		 struct alignas(64) dtype_13 { 
			 uint8_t padding[(sizeof(economy::goods_tag) + 63ui32) & ~63ui32]; 
			 economy::goods_tag values[(sizeof(economy::goods_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(economy::goods_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(economy::goods_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_13() { std::uninitialized_value_construct_n(values - 1, (sizeof(economy::goods_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(economy::goods_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(economy::goods_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_13;
		 struct alignas(64) dtype_14 { 
			 uint8_t padding[(sizeof(cultures::culture_tag) + 63ui32) & ~63ui32]; 
			 cultures::culture_tag values[(sizeof(cultures::culture_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_14() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::culture_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_14;
		 struct alignas(64) dtype_15 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_15() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_15;
		 struct alignas(64) dtype_16 { 
			 uint8_t padding[(sizeof(modifiers::provincial_modifier_tag) + 63ui32) & ~63ui32]; 
			 modifiers::provincial_modifier_tag values[(sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_16() { std::uninitialized_value_construct_n(values - 1, (sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_16;
		 struct alignas(64) dtype_17 { 
			 uint8_t padding[(sizeof(modifiers::provincial_modifier_tag) + 63ui32) & ~63ui32]; 
			 modifiers::provincial_modifier_tag values[(sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_17() { std::uninitialized_value_construct_n(values - 1, (sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_17;
		 struct alignas(64) dtype_18 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_18() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_18;
		 struct alignas(64) dtype_19 { 
			 uint8_t padding[(sizeof(multiset_tag<provinces::timed_provincial_modifier>) + 63ui32) & ~63ui32]; 
			 multiset_tag<provinces::timed_provincial_modifier> values[(sizeof(multiset_tag<provinces::timed_provincial_modifier>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(multiset_tag<provinces::timed_provincial_modifier>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(multiset_tag<provinces::timed_provincial_modifier>)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_19() { std::uninitialized_value_construct_n(values - 1, (sizeof(multiset_tag<provinces::timed_provincial_modifier>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(multiset_tag<provinces::timed_provincial_modifier>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(multiset_tag<provinces::timed_provincial_modifier>)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_19;
		 struct alignas(64) dtype_20 { 
			 uint8_t padding[(sizeof(set_tag<modifiers::provincial_modifier_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<modifiers::provincial_modifier_tag> values[(sizeof(set_tag<modifiers::provincial_modifier_tag>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(set_tag<modifiers::provincial_modifier_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<modifiers::provincial_modifier_tag>)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_20() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<modifiers::provincial_modifier_tag>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(set_tag<modifiers::provincial_modifier_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<modifiers::provincial_modifier_tag>)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_20;
		 struct alignas(64) dtype_21 { 
			 uint8_t padding[(sizeof(set_tag<military::fleet_presence>) + 63ui32) & ~63ui32]; 
			 set_tag<military::fleet_presence> values[(sizeof(set_tag<military::fleet_presence>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(set_tag<military::fleet_presence>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::fleet_presence>)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_21() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<military::fleet_presence>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(set_tag<military::fleet_presence>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::fleet_presence>)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_21;
		 struct alignas(64) dtype_22 { 
			 uint8_t padding[(sizeof(array_tag<population::pop_tag, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<population::pop_tag, int32_t, false> values[(sizeof(array_tag<population::pop_tag, int32_t, false>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(array_tag<population::pop_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<population::pop_tag, int32_t, false>)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_22() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<population::pop_tag, int32_t, false>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(array_tag<population::pop_tag, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<population::pop_tag, int32_t, false>)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_22;
		 struct alignas(64) dtype_23 { 
			 uint8_t padding[(sizeof(set_tag<cultures::national_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<cultures::national_tag> values[(sizeof(set_tag<cultures::national_tag>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(set_tag<cultures::national_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<cultures::national_tag>)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_23() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<cultures::national_tag>) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(set_tag<cultures::national_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<cultures::national_tag>)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_23;
		 struct alignas(64) dtype_24 { 
			 uint8_t padding[(sizeof(economy::worked_instance) + 63ui32) & ~63ui32]; 
			 economy::worked_instance values[(sizeof(economy::worked_instance) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(economy::worked_instance))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(economy::worked_instance)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_24() { std::uninitialized_value_construct_n(values - 1, (sizeof(economy::worked_instance) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(economy::worked_instance))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(economy::worked_instance)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_24;
		 struct alignas(64) dtype_25 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_25() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_25;
		 struct alignas(64) dtype_26 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_26() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_26;
		 struct alignas(64) dtype_27 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_27() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_27;
		 struct alignas(64) dtype_28 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_28() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_28;
		 struct alignas(64) dtype_29 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_29() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_29;
		 struct alignas(64) dtype_30 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_30() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_30;
		 struct alignas(64) dtype_31 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_31() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_31;
		 struct alignas(64) dtype_32 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_32() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_32;
		 struct alignas(64) dtype_33 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_33() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_33;
		 struct alignas(64) dtype_34 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_34() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_34;
		 struct alignas(64) dtype_35 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_35() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_35;
		 struct alignas(64) dtype_36 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_36() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_36;
		 struct alignas(64) dtype_37 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_37() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_37;
		 struct alignas(64) dtype_38 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_38() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_38;
		 struct alignas(64) dtype_39 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_39() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_39;
		 struct alignas(64) dtype_40 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_40() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_40;
		 struct alignas(64) dtype_41 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_41() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_41;
		 struct alignas(64) dtype_42 { 
			 uint8_t padding[(sizeof(population::rebel_faction_tag) + 63ui32) & ~63ui32]; 
			 population::rebel_faction_tag values[(sizeof(population::rebel_faction_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(population::rebel_faction_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(population::rebel_faction_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_42() { std::uninitialized_value_construct_n(values - 1, (sizeof(population::rebel_faction_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(population::rebel_faction_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(population::rebel_faction_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_42;
		 struct alignas(64) dtype_43 { 
			 uint8_t padding[(sizeof(nations::state_tag) + 63ui32) & ~63ui32]; 
			 nations::state_tag values[(sizeof(nations::state_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(nations::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::state_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_43() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::state_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(nations::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::state_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_43;
		 struct alignas(64) dtype_44 { 
			 uint8_t padding[(sizeof(military::army_orders_tag) + 63ui32) & ~63ui32]; 
			 military::army_orders_tag values[(sizeof(military::army_orders_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(military::army_orders_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::army_orders_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_44() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::army_orders_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(military::army_orders_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::army_orders_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_44;
		 struct alignas(64) dtype_45 { 
			 uint8_t padding[(sizeof(military::strategic_hq_tag) + 63ui32) & ~63ui32]; 
			 military::strategic_hq_tag values[(sizeof(military::strategic_hq_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(military::strategic_hq_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::strategic_hq_tag)) - 1ui32) : uint32_t(province_state::container_size))]; 
			 dtype_45() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::strategic_hq_tag) <= 64 ? (uint32_t(province_state::container_size) + (64ui32 / uint32_t(sizeof(military::strategic_hq_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::strategic_hq_tag)) - 1ui32) : uint32_t(province_state::container_size)) + 1); }
		 } m_45;

		 public:
		 friend class serialization::serializer<container>;
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_blockaded>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_0.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province_state::is_blockaded>> {
			 bit_vector_set(m_0.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_blockaded>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_blockaded>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_overseas>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_1.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province_state::is_overseas>> {
			 bit_vector_set(m_1.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_overseas>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_overseas>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::has_owner_core>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_2.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province_state::has_owner_core>> {
			 bit_vector_set(m_2.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::has_owner_core>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::has_owner_core>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::owner_building_railroad>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_3.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province_state::owner_building_railroad>> {
			 bit_vector_set(m_3.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::owner_building_railroad>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_3.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::owner_building_railroad>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_3.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_non_state>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_4.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province_state::is_non_state>> {
			 bit_vector_set(m_4.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_non_state>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_4.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::is_non_state>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_4.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_level>, uint8_t&> get(provinces::province_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_level>, uint8_t const&> get(provinces::province_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::fort_level>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::fort_level>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_level>, tagged_array_view<uint8_t, provinces::province_tag>> get_row() {
			 return tagged_array_view<uint8_t, provinces::province_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_level>, tagged_array_view<uint8_t const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<uint8_t const, provinces::province_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_level>, uint8_t&> get(provinces::province_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_level>, uint8_t const&> get(provinces::province_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::railroad_level>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::railroad_level>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_level>, tagged_array_view<uint8_t, provinces::province_tag>> get_row() {
			 return tagged_array_view<uint8_t, provinces::province_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_level>, tagged_array_view<uint8_t const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<uint8_t const, provinces::province_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_level>, uint8_t&> get(provinces::province_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_level>, uint8_t const&> get(provinces::province_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::naval_base_level>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::naval_base_level>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_level>, tagged_array_view<uint8_t, provinces::province_tag>> get_row() {
			 return tagged_array_view<uint8_t, provinces::province_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_level>, tagged_array_view<uint8_t const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<uint8_t const, provinces::province_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_size>, uint8_t&> get(provinces::province_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_size>, uint8_t const&> get(provinces::province_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rgo_size>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rgo_size>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_size>, tagged_array_view<uint8_t, provinces::province_tag>> get_row() {
			 return tagged_array_view<uint8_t, provinces::province_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_size>, tagged_array_view<uint8_t const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<uint8_t const, provinces::province_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_religion>, cultures::religion_tag&> get(provinces::province_tag i) {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_religion>, cultures::religion_tag const&> get(provinces::province_tag i) const {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_religion>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_religion>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_religion>, tagged_array_view<cultures::religion_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<cultures::religion_tag, provinces::province_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_religion>, tagged_array_view<cultures::religion_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<cultures::religion_tag const, provinces::province_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_ideology>, ideologies::ideology_tag&> get(provinces::province_tag i) {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_ideology>, ideologies::ideology_tag const&> get(provinces::province_tag i) const {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_ideology>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_ideology>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_ideology>, tagged_array_view<ideologies::ideology_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<ideologies::ideology_tag, provinces::province_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_ideology>, tagged_array_view<ideologies::ideology_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<ideologies::ideology_tag const, provinces::province_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_issue>, issues::option_tag&> get(provinces::province_tag i) {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_issue>, issues::option_tag const&> get(provinces::province_tag i) const {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_issue>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_issue>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_issue>, tagged_array_view<issues::option_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<issues::option_tag, provinces::province_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_issue>, tagged_array_view<issues::option_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<issues::option_tag const, provinces::province_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production>, economy::goods_tag&> get(provinces::province_tag i) {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production>, economy::goods_tag const&> get(provinces::province_tag i) const {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::artisan_production>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::artisan_production>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production>, tagged_array_view<economy::goods_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<economy::goods_tag, provinces::province_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production>, tagged_array_view<economy::goods_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<economy::goods_tag const, provinces::province_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_production>, economy::goods_tag&> get(provinces::province_tag i) {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_production>, economy::goods_tag const&> get(provinces::province_tag i) const {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rgo_production>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rgo_production>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_production>, tagged_array_view<economy::goods_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<economy::goods_tag, provinces::province_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_production>, tagged_array_view<economy::goods_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<economy::goods_tag const, provinces::province_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_culture>, cultures::culture_tag&> get(provinces::province_tag i) {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_culture>, cultures::culture_tag const&> get(provinces::province_tag i) const {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_culture>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::dominant_culture>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_culture>, tagged_array_view<cultures::culture_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<cultures::culture_tag, provinces::province_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::dominant_culture>, tagged_array_view<cultures::culture_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<cultures::culture_tag const, provinces::province_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::base_life_rating>, float&> get(provinces::province_tag i) {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::base_life_rating>, float const&> get(provinces::province_tag i) const {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::base_life_rating>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::base_life_rating>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::base_life_rating>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::base_life_rating>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::terrain>, modifiers::provincial_modifier_tag&> get(provinces::province_tag i) {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::terrain>, modifiers::provincial_modifier_tag const&> get(provinces::province_tag i) const {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::terrain>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::terrain>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::terrain>, tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::terrain>, tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::crime>, modifiers::provincial_modifier_tag&> get(provinces::province_tag i) {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::crime>, modifiers::provincial_modifier_tag const&> get(provinces::province_tag i) const {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::crime>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::crime>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::crime>, tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::crime>, tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::name>, text_data::text_tag&> get(provinces::province_tag i) {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::name>, text_data::text_tag const&> get(provinces::province_tag i) const {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::name>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::name>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::name>, tagged_array_view<text_data::text_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, provinces::province_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::name>, tagged_array_view<text_data::text_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, provinces::province_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::timed_modifiers>, multiset_tag<provinces::timed_provincial_modifier>&> get(provinces::province_tag i) {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::timed_modifiers>, multiset_tag<provinces::timed_provincial_modifier> const&> get(provinces::province_tag i) const {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::timed_modifiers>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::timed_modifiers>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::timed_modifiers>, tagged_array_view<multiset_tag<provinces::timed_provincial_modifier>, provinces::province_tag>> get_row() {
			 return tagged_array_view<multiset_tag<provinces::timed_provincial_modifier>, provinces::province_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::timed_modifiers>, tagged_array_view<multiset_tag<provinces::timed_provincial_modifier> const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<multiset_tag<provinces::timed_provincial_modifier> const, provinces::province_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::static_modifiers>, set_tag<modifiers::provincial_modifier_tag>&> get(provinces::province_tag i) {
			 return m_20.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::static_modifiers>, set_tag<modifiers::provincial_modifier_tag> const&> get(provinces::province_tag i) const {
			 return m_20.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::static_modifiers>> {
			 m_20.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::static_modifiers>> {
			 m_20.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::static_modifiers>, tagged_array_view<set_tag<modifiers::provincial_modifier_tag>, provinces::province_tag>> get_row() {
			 return tagged_array_view<set_tag<modifiers::provincial_modifier_tag>, provinces::province_tag>(m_20.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::static_modifiers>, tagged_array_view<set_tag<modifiers::provincial_modifier_tag> const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<set_tag<modifiers::provincial_modifier_tag> const, provinces::province_tag>(m_20.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fleets>, set_tag<military::fleet_presence>&> get(provinces::province_tag i) {
			 return m_21.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fleets>, set_tag<military::fleet_presence> const&> get(provinces::province_tag i) const {
			 return m_21.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::fleets>> {
			 m_21.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::fleets>> {
			 m_21.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fleets>, tagged_array_view<set_tag<military::fleet_presence>, provinces::province_tag>> get_row() {
			 return tagged_array_view<set_tag<military::fleet_presence>, provinces::province_tag>(m_21.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fleets>, tagged_array_view<set_tag<military::fleet_presence> const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<set_tag<military::fleet_presence> const, provinces::province_tag>(m_21.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::pops>, array_tag<population::pop_tag, int32_t, false>&> get(provinces::province_tag i) {
			 return m_22.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::pops>, array_tag<population::pop_tag, int32_t, false> const&> get(provinces::province_tag i) const {
			 return m_22.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::pops>> {
			 m_22.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::pops>> {
			 m_22.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::pops>, tagged_array_view<array_tag<population::pop_tag, int32_t, false>, provinces::province_tag>> get_row() {
			 return tagged_array_view<array_tag<population::pop_tag, int32_t, false>, provinces::province_tag>(m_22.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::pops>, tagged_array_view<array_tag<population::pop_tag, int32_t, false> const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<array_tag<population::pop_tag, int32_t, false> const, provinces::province_tag>(m_22.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::cores>, set_tag<cultures::national_tag>&> get(provinces::province_tag i) {
			 return m_23.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::cores>, set_tag<cultures::national_tag> const&> get(provinces::province_tag i) const {
			 return m_23.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::cores>> {
			 m_23.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::cores>> {
			 m_23.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::cores>, tagged_array_view<set_tag<cultures::national_tag>, provinces::province_tag>> get_row() {
			 return tagged_array_view<set_tag<cultures::national_tag>, provinces::province_tag>(m_23.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::cores>, tagged_array_view<set_tag<cultures::national_tag> const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<set_tag<cultures::national_tag> const, provinces::province_tag>(m_23.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_worker_data>, economy::worked_instance&> get(provinces::province_tag i) {
			 return m_24.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_worker_data>, economy::worked_instance const&> get(provinces::province_tag i) const {
			 return m_24.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rgo_worker_data>> {
			 m_24.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rgo_worker_data>> {
			 m_24.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_worker_data>, tagged_array_view<economy::worked_instance, provinces::province_tag>> get_row() {
			 return tagged_array_view<economy::worked_instance, provinces::province_tag>(m_24.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rgo_worker_data>, tagged_array_view<economy::worked_instance const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<economy::worked_instance const, provinces::province_tag>(m_24.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::total_population>, float&> get(provinces::province_tag i) {
			 return m_25.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::total_population>, float const&> get(provinces::province_tag i) const {
			 return m_25.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::total_population>> {
			 m_25.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::total_population>> {
			 m_25.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::total_population>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_25.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::total_population>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_25.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_controller_change>, date_tag&> get(provinces::province_tag i) {
			 return m_26.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_controller_change>, date_tag const&> get(provinces::province_tag i) const {
			 return m_26.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::last_controller_change>> {
			 m_26.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::last_controller_change>> {
			 m_26.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_controller_change>, tagged_array_view<date_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<date_tag, provinces::province_tag>(m_26.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_controller_change>, tagged_array_view<date_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<date_tag const, provinces::province_tag>(m_26.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_immigration>, date_tag&> get(provinces::province_tag i) {
			 return m_27.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_immigration>, date_tag const&> get(provinces::province_tag i) const {
			 return m_27.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::last_immigration>> {
			 m_27.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::last_immigration>> {
			 m_27.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_immigration>, tagged_array_view<date_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<date_tag, provinces::province_tag>(m_27.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::last_immigration>, tagged_array_view<date_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<date_tag const, provinces::province_tag>(m_27.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::nationalism>, float&> get(provinces::province_tag i) {
			 return m_28.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::nationalism>, float const&> get(provinces::province_tag i) const {
			 return m_28.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::nationalism>> {
			 m_28.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::nationalism>> {
			 m_28.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::nationalism>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_28.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::nationalism>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_28.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::siege_progress>, float&> get(provinces::province_tag i) {
			 return m_29.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::siege_progress>, float const&> get(provinces::province_tag i) const {
			 return m_29.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::siege_progress>> {
			 m_29.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::siege_progress>> {
			 m_29.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::siege_progress>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_29.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::siege_progress>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_29.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_upgrade_progress>, float&> get(provinces::province_tag i) {
			 return m_30.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_upgrade_progress>, float const&> get(provinces::province_tag i) const {
			 return m_30.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::fort_upgrade_progress>> {
			 m_30.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::fort_upgrade_progress>> {
			 m_30.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_upgrade_progress>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_30.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::fort_upgrade_progress>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_30.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_upgrade_progress>, float&> get(provinces::province_tag i) {
			 return m_31.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_upgrade_progress>, float const&> get(provinces::province_tag i) const {
			 return m_31.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::railroad_upgrade_progress>> {
			 m_31.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::railroad_upgrade_progress>> {
			 m_31.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_upgrade_progress>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_31.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::railroad_upgrade_progress>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_31.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_upgrade_progress>, float&> get(provinces::province_tag i) {
			 return m_32.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_upgrade_progress>, float const&> get(provinces::province_tag i) const {
			 return m_32.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::naval_base_upgrade_progress>> {
			 m_32.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::naval_base_upgrade_progress>> {
			 m_32.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_upgrade_progress>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_32.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::naval_base_upgrade_progress>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_32.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production_scale>, float&> get(provinces::province_tag i) {
			 return m_33.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production_scale>, float const&> get(provinces::province_tag i) const {
			 return m_33.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::artisan_production_scale>> {
			 m_33.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::artisan_production_scale>> {
			 m_33.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production_scale>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_33.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::artisan_production_scale>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_33.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_migration_growth>, float&> get(provinces::province_tag i) {
			 return m_34.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_migration_growth>, float const&> get(provinces::province_tag i) const {
			 return m_34.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::net_migration_growth>> {
			 m_34.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::net_migration_growth>> {
			 m_34.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_migration_growth>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_34.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_migration_growth>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_34.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_immigration_growth>, float&> get(provinces::province_tag i) {
			 return m_35.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_immigration_growth>, float const&> get(provinces::province_tag i) const {
			 return m_35.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::net_immigration_growth>> {
			 m_35.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::net_immigration_growth>> {
			 m_35.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_immigration_growth>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_35.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::net_immigration_growth>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_35.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::monthly_population>, float&> get(provinces::province_tag i) {
			 return m_36.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::monthly_population>, float const&> get(provinces::province_tag i) const {
			 return m_36.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::monthly_population>> {
			 m_36.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::monthly_population>> {
			 m_36.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::monthly_population>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_36.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::monthly_population>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_36.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_migration_growth>, float&> get(provinces::province_tag i) {
			 return m_37.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_migration_growth>, float const&> get(provinces::province_tag i) const {
			 return m_37.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::old_migration_growth>> {
			 m_37.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::old_migration_growth>> {
			 m_37.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_migration_growth>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_37.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_migration_growth>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_37.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_immigration_growth>, float&> get(provinces::province_tag i) {
			 return m_38.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_immigration_growth>, float const&> get(provinces::province_tag i) const {
			 return m_38.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::old_immigration_growth>> {
			 m_38.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::old_immigration_growth>> {
			 m_38.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_immigration_growth>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_38.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_immigration_growth>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_38.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_monthly_population>, float&> get(provinces::province_tag i) {
			 return m_39.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_monthly_population>, float const&> get(provinces::province_tag i) const {
			 return m_39.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::old_monthly_population>> {
			 m_39.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::old_monthly_population>> {
			 m_39.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_monthly_population>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_39.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::old_monthly_population>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_39.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::owner>, nations::country_tag&> get(provinces::province_tag i) {
			 return m_40.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::owner>, nations::country_tag const&> get(provinces::province_tag i) const {
			 return m_40.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::owner>> {
			 m_40.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::owner>> {
			 m_40.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::owner>, tagged_array_view<nations::country_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, provinces::province_tag>(m_40.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::owner>, tagged_array_view<nations::country_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, provinces::province_tag>(m_40.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::controller>, nations::country_tag&> get(provinces::province_tag i) {
			 return m_41.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::controller>, nations::country_tag const&> get(provinces::province_tag i) const {
			 return m_41.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::controller>> {
			 m_41.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::controller>> {
			 m_41.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::controller>, tagged_array_view<nations::country_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, provinces::province_tag>(m_41.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::controller>, tagged_array_view<nations::country_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, provinces::province_tag>(m_41.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rebel_controller>, population::rebel_faction_tag&> get(provinces::province_tag i) {
			 return m_42.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rebel_controller>, population::rebel_faction_tag const&> get(provinces::province_tag i) const {
			 return m_42.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rebel_controller>> {
			 m_42.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::rebel_controller>> {
			 m_42.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rebel_controller>, tagged_array_view<population::rebel_faction_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<population::rebel_faction_tag, provinces::province_tag>(m_42.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::rebel_controller>, tagged_array_view<population::rebel_faction_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<population::rebel_faction_tag const, provinces::province_tag>(m_42.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::state_instance>, nations::state_tag&> get(provinces::province_tag i) {
			 return m_43.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::state_instance>, nations::state_tag const&> get(provinces::province_tag i) const {
			 return m_43.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::state_instance>> {
			 m_43.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::state_instance>> {
			 m_43.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::state_instance>, tagged_array_view<nations::state_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<nations::state_tag, provinces::province_tag>(m_43.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::state_instance>, tagged_array_view<nations::state_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<nations::state_tag const, provinces::province_tag>(m_43.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::orders>, military::army_orders_tag&> get(provinces::province_tag i) {
			 return m_44.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::orders>, military::army_orders_tag const&> get(provinces::province_tag i) const {
			 return m_44.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::orders>> {
			 m_44.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::orders>> {
			 m_44.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::orders>, tagged_array_view<military::army_orders_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<military::army_orders_tag, provinces::province_tag>(m_44.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::orders>, tagged_array_view<military::army_orders_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<military::army_orders_tag const, provinces::province_tag>(m_44.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::strat_hq>, military::strategic_hq_tag&> get(provinces::province_tag i) {
			 return m_45.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::strat_hq>, military::strategic_hq_tag const&> get(provinces::province_tag i) const {
			 return m_45.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::strat_hq>> {
			 m_45.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province_state::strat_hq>> {
			 m_45.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::strat_hq>, tagged_array_view<military::strategic_hq_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<military::strategic_hq_tag, provinces::province_tag>(m_45.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province_state::strat_hq>, tagged_array_view<military::strategic_hq_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<military::strategic_hq_tag const, provinces::province_tag>(m_45.values, ve::to_vector_size(uint32_t(size_used)));
		 }

		 void resize(int32_t s) { size_used = s; }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(provinces::province_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 f(provinces::province_tag(provinces::province_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 f(provinces::province_tag(provinces::province_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(f(provinces::province_tag(provinces::province_tag::value_base_t(i)))) return true;
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<province_state::container, province_state::is_blockaded> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::is_overseas> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::has_owner_core> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::owner_building_railroad> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::is_non_state> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::fort_level> {
	static constexpr bool value = true;
	 using type = uint8_t&;
	 using const_type = uint8_t const&;
	 using row = tagged_array_view<uint8_t, provinces::province_tag>;
	 using const_row = tagged_array_view<uint8_t const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::railroad_level> {
	static constexpr bool value = true;
	 using type = uint8_t&;
	 using const_type = uint8_t const&;
	 using row = tagged_array_view<uint8_t, provinces::province_tag>;
	 using const_row = tagged_array_view<uint8_t const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::naval_base_level> {
	static constexpr bool value = true;
	 using type = uint8_t&;
	 using const_type = uint8_t const&;
	 using row = tagged_array_view<uint8_t, provinces::province_tag>;
	 using const_row = tagged_array_view<uint8_t const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::rgo_size> {
	static constexpr bool value = true;
	 using type = uint8_t&;
	 using const_type = uint8_t const&;
	 using row = tagged_array_view<uint8_t, provinces::province_tag>;
	 using const_row = tagged_array_view<uint8_t const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::dominant_religion> {
	static constexpr bool value = true;
	 using type = cultures::religion_tag&;
	 using const_type = cultures::religion_tag const&;
	 using row = tagged_array_view<cultures::religion_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<cultures::religion_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::dominant_ideology> {
	static constexpr bool value = true;
	 using type = ideologies::ideology_tag&;
	 using const_type = ideologies::ideology_tag const&;
	 using row = tagged_array_view<ideologies::ideology_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<ideologies::ideology_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::dominant_issue> {
	static constexpr bool value = true;
	 using type = issues::option_tag&;
	 using const_type = issues::option_tag const&;
	 using row = tagged_array_view<issues::option_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<issues::option_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::artisan_production> {
	static constexpr bool value = true;
	 using type = economy::goods_tag&;
	 using const_type = economy::goods_tag const&;
	 using row = tagged_array_view<economy::goods_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<economy::goods_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::rgo_production> {
	static constexpr bool value = true;
	 using type = economy::goods_tag&;
	 using const_type = economy::goods_tag const&;
	 using row = tagged_array_view<economy::goods_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<economy::goods_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::dominant_culture> {
	static constexpr bool value = true;
	 using type = cultures::culture_tag&;
	 using const_type = cultures::culture_tag const&;
	 using row = tagged_array_view<cultures::culture_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<cultures::culture_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::base_life_rating> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::terrain> {
	static constexpr bool value = true;
	 using type = modifiers::provincial_modifier_tag&;
	 using const_type = modifiers::provincial_modifier_tag const&;
	 using row = tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::crime> {
	static constexpr bool value = true;
	 using type = modifiers::provincial_modifier_tag&;
	 using const_type = modifiers::provincial_modifier_tag const&;
	 using row = tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::name> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::timed_modifiers> {
	static constexpr bool value = true;
	 using type = multiset_tag<provinces::timed_provincial_modifier>&;
	 using const_type = multiset_tag<provinces::timed_provincial_modifier> const&;
	 using row = tagged_array_view<multiset_tag<provinces::timed_provincial_modifier>, provinces::province_tag>;
	 using const_row = tagged_array_view<multiset_tag<provinces::timed_provincial_modifier> const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::static_modifiers> {
	static constexpr bool value = true;
	 using type = set_tag<modifiers::provincial_modifier_tag>&;
	 using const_type = set_tag<modifiers::provincial_modifier_tag> const&;
	 using row = tagged_array_view<set_tag<modifiers::provincial_modifier_tag>, provinces::province_tag>;
	 using const_row = tagged_array_view<set_tag<modifiers::provincial_modifier_tag> const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::fleets> {
	static constexpr bool value = true;
	 using type = set_tag<military::fleet_presence>&;
	 using const_type = set_tag<military::fleet_presence> const&;
	 using row = tagged_array_view<set_tag<military::fleet_presence>, provinces::province_tag>;
	 using const_row = tagged_array_view<set_tag<military::fleet_presence> const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::pops> {
	static constexpr bool value = true;
	 using type = array_tag<population::pop_tag, int32_t, false>&;
	 using const_type = array_tag<population::pop_tag, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<population::pop_tag, int32_t, false>, provinces::province_tag>;
	 using const_row = tagged_array_view<array_tag<population::pop_tag, int32_t, false> const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::cores> {
	static constexpr bool value = true;
	 using type = set_tag<cultures::national_tag>&;
	 using const_type = set_tag<cultures::national_tag> const&;
	 using row = tagged_array_view<set_tag<cultures::national_tag>, provinces::province_tag>;
	 using const_row = tagged_array_view<set_tag<cultures::national_tag> const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::rgo_worker_data> {
	static constexpr bool value = true;
	 using type = economy::worked_instance&;
	 using const_type = economy::worked_instance const&;
	 using row = tagged_array_view<economy::worked_instance, provinces::province_tag>;
	 using const_row = tagged_array_view<economy::worked_instance const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::total_population> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::last_controller_change> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<date_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::last_immigration> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<date_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::nationalism> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::siege_progress> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::fort_upgrade_progress> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::railroad_upgrade_progress> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::naval_base_upgrade_progress> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::artisan_production_scale> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::net_migration_growth> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::net_immigration_growth> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::monthly_population> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::old_migration_growth> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::old_immigration_growth> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::old_monthly_population> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::owner> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::controller> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::rebel_controller> {
	static constexpr bool value = true;
	 using type = population::rebel_faction_tag&;
	 using const_type = population::rebel_faction_tag const&;
	 using row = tagged_array_view<population::rebel_faction_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<population::rebel_faction_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::state_instance> {
	static constexpr bool value = true;
	 using type = nations::state_tag&;
	 using const_type = nations::state_tag const&;
	 using row = tagged_array_view<nations::state_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<nations::state_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::orders> {
	static constexpr bool value = true;
	 using type = military::army_orders_tag&;
	 using const_type = military::army_orders_tag const&;
	 using row = tagged_array_view<military::army_orders_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<military::army_orders_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province_state::container, province_state::strat_hq> {
	static constexpr bool value = true;
	 using type = military::strategic_hq_tag&;
	 using const_type = military::strategic_hq_tag const&;
	 using row = tagged_array_view<military::strategic_hq_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<military::strategic_hq_tag const, provinces::province_tag>;
};

template<>
class serialization::serializer<province_state::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, province_state::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 serialization::serialize_array(output, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_3.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_4.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_level, uint8_t>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_level, uint8_t>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_level, uint8_t>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_level, uint8_t>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_size, uint8_t>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_size, uint8_t>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::base_life_rating, float>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::base_life_rating, float>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::name, text_data::text_tag>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::name, text_data::text_tag>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::serialize_object(output, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::serialize_object(output, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::serialize_object(output, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::serialize_object(output, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::serialize_object(output, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::serialize_object(output, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::serialize_object(output, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::serialize_object(output, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::serialize_object(output, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::serialize_object(output, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::total_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::total_population, float>::serialize_object(output, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::total_population, float>::serialize_object(output, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_controller_change, date_tag>::serialize_object(output, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_controller_change, date_tag>::serialize_object(output, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_immigration, date_tag>::serialize_object(output, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_immigration, date_tag>::serialize_object(output, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::nationalism, float>::serialize_object(output, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::nationalism, float>::serialize_object(output, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::siege_progress, float>::serialize_object(output, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::siege_progress, float>::serialize_object(output, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::serialize_object(output, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::serialize_object(output, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::serialize_object(output, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::serialize_object(output, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::serialize_object(output, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::serialize_object(output, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production_scale, float>::serialize_object(output, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production_scale, float>::serialize_object(output, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_migration_growth, float>::serialize_object(output, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_migration_growth, float>::serialize_object(output, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_immigration_growth, float>::serialize_object(output, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_immigration_growth, float>::serialize_object(output, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::monthly_population, float>::serialize_object(output, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::monthly_population, float>::serialize_object(output, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_migration_growth, float>::serialize_object(output, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_migration_growth, float>::serialize_object(output, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_immigration_growth, float>::serialize_object(output, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_immigration_growth, float>::serialize_object(output, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_monthly_population, float>::serialize_object(output, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_monthly_population, float>::serialize_object(output, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::owner, nations::country_tag>::serialize_object(output, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::owner, nations::country_tag>::serialize_object(output, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::controller, nations::country_tag>::serialize_object(output, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::controller, nations::country_tag>::serialize_object(output, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::serialize_object(output, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::serialize_object(output, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::serialize_object(output, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::serialize_object(output, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::serialize_object(output, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::serialize_object(output, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::serialize_object(output, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::serialize_object(output, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, province_state::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_3.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_4.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_level, uint8_t>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_level, uint8_t>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_level, uint8_t>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_level, uint8_t>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_size, uint8_t>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_size, uint8_t>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::base_life_rating, float>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::base_life_rating, float>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::name, text_data::text_tag>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::name, text_data::text_tag>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::deserialize_object(input, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::deserialize_object(input, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::deserialize_object(input, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_21.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::deserialize_object(input, obj.m_21.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::deserialize_object(input, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_22.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::deserialize_object(input, obj.m_22.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::deserialize_object(input, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_23.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::deserialize_object(input, obj.m_23.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::deserialize_object(input, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_24.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::deserialize_object(input, obj.m_24.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::total_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::total_population, float>::deserialize_object(input, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_25.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::total_population, float>::deserialize_object(input, obj.m_25.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_controller_change, date_tag>::deserialize_object(input, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_26.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_controller_change, date_tag>::deserialize_object(input, obj.m_26.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_immigration, date_tag>::deserialize_object(input, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_27.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::last_immigration, date_tag>::deserialize_object(input, obj.m_27.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::nationalism, float>::deserialize_object(input, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_28.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::nationalism, float>::deserialize_object(input, obj.m_28.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::siege_progress, float>::deserialize_object(input, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_29.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::siege_progress, float>::deserialize_object(input, obj.m_29.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::deserialize_object(input, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_30.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::deserialize_object(input, obj.m_30.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::deserialize_object(input, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_31.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::deserialize_object(input, obj.m_31.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::deserialize_object(input, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_32.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::deserialize_object(input, obj.m_32.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production_scale, float>::deserialize_object(input, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_33.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::artisan_production_scale, float>::deserialize_object(input, obj.m_33.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_migration_growth, float>::deserialize_object(input, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_34.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_migration_growth, float>::deserialize_object(input, obj.m_34.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_immigration_growth, float>::deserialize_object(input, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_35.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::net_immigration_growth, float>::deserialize_object(input, obj.m_35.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::monthly_population, float>::deserialize_object(input, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_36.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::monthly_population, float>::deserialize_object(input, obj.m_36.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_migration_growth, float>::deserialize_object(input, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_37.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_migration_growth, float>::deserialize_object(input, obj.m_37.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_immigration_growth, float>::deserialize_object(input, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_38.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_immigration_growth, float>::deserialize_object(input, obj.m_38.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_monthly_population, float>::deserialize_object(input, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_39.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::old_monthly_population, float>::deserialize_object(input, obj.m_39.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::owner, nations::country_tag>::deserialize_object(input, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_40.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::owner, nations::country_tag>::deserialize_object(input, obj.m_40.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::controller, nations::country_tag>::deserialize_object(input, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_41.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::controller, nations::country_tag>::deserialize_object(input, obj.m_41.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::deserialize_object(input, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_42.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::deserialize_object(input, obj.m_42.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::deserialize_object(input, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_43.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::deserialize_object(input, obj.m_43.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::deserialize_object(input, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_44.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::deserialize_object(input, obj.m_44.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::deserialize_object(input, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_45.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::deserialize_object(input, obj.m_45.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static size_t size(province_state::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::fort_level, uint8_t>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::fort_level, uint8_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::fort_level, uint8_t>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](uint8_t const& m) {
					 return serialization::tagged_serializer<province_state::fort_level, uint8_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::railroad_level, uint8_t>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::railroad_level, uint8_t>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](uint8_t const& m) {
					 return serialization::tagged_serializer<province_state::railroad_level, uint8_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](uint8_t const& m) {
					 return serialization::tagged_serializer<province_state::naval_base_level, uint8_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::rgo_size, uint8_t>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::rgo_size, uint8_t>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](uint8_t const& m) {
					 return serialization::tagged_serializer<province_state::rgo_size, uint8_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::size(obj.m_9.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_9.values, obj.m_9.values + max, 0ui64, std::plus<>(), [&](cultures::religion_tag const& m) {
					 return serialization::tagged_serializer<province_state::dominant_religion, cultures::religion_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::size(obj.m_10.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_10.values, obj.m_10.values + max, 0ui64, std::plus<>(), [&](ideologies::ideology_tag const& m) {
					 return serialization::tagged_serializer<province_state::dominant_ideology, ideologies::ideology_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::size(obj.m_11.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_11.values, obj.m_11.values + max, 0ui64, std::plus<>(), [&](issues::option_tag const& m) {
					 return serialization::tagged_serializer<province_state::dominant_issue, issues::option_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::size(obj.m_12.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_12.values, obj.m_12.values + max, 0ui64, std::plus<>(), [&](economy::goods_tag const& m) {
					 return serialization::tagged_serializer<province_state::artisan_production, economy::goods_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::size(obj.m_13.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_13.values, obj.m_13.values + max, 0ui64, std::plus<>(), [&](economy::goods_tag const& m) {
					 return serialization::tagged_serializer<province_state::rgo_production, economy::goods_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::size(obj.m_14.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_14.values, obj.m_14.values + max, 0ui64, std::plus<>(), [&](cultures::culture_tag const& m) {
					 return serialization::tagged_serializer<province_state::dominant_culture, cultures::culture_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::base_life_rating, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::base_life_rating, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::base_life_rating, float>::size(obj.m_15.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_15.values, obj.m_15.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::base_life_rating, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::size(obj.m_16.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_16.values, obj.m_16.values + max, 0ui64, std::plus<>(), [&](modifiers::provincial_modifier_tag const& m) {
					 return serialization::tagged_serializer<province_state::terrain, modifiers::provincial_modifier_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::size(obj.m_17.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_17.values, obj.m_17.values + max, 0ui64, std::plus<>(), [&](modifiers::provincial_modifier_tag const& m) {
					 return serialization::tagged_serializer<province_state::crime, modifiers::provincial_modifier_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::name, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::name, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::name, text_data::text_tag>::size(obj.m_18.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_18.values, obj.m_18.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<province_state::name, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::size(obj.m_19.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_19.values, obj.m_19.values + max, 0ui64, std::plus<>(), [&](multiset_tag<provinces::timed_provincial_modifier> const& m) {
					 return serialization::tagged_serializer<province_state::timed_modifiers, multiset_tag<provinces::timed_provincial_modifier>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::size(obj.m_20.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_20.values, obj.m_20.values + max, 0ui64, std::plus<>(), [&](set_tag<modifiers::provincial_modifier_tag> const& m) {
					 return serialization::tagged_serializer<province_state::static_modifiers, set_tag<modifiers::provincial_modifier_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::size(obj.m_21.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_21.values, obj.m_21.values + max, 0ui64, std::plus<>(), [&](set_tag<military::fleet_presence> const& m) {
					 return serialization::tagged_serializer<province_state::fleets, set_tag<military::fleet_presence>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::size(obj.m_22.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_22.values, obj.m_22.values + max, 0ui64, std::plus<>(), [&](array_tag<population::pop_tag, int32_t, false> const& m) {
					 return serialization::tagged_serializer<province_state::pops, array_tag<population::pop_tag, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::size(obj.m_23.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_23.values, obj.m_23.values + max, 0ui64, std::plus<>(), [&](set_tag<cultures::national_tag> const& m) {
					 return serialization::tagged_serializer<province_state::cores, set_tag<cultures::national_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::size(obj.m_24.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_24.values, obj.m_24.values + max, 0ui64, std::plus<>(), [&](economy::worked_instance const& m) {
					 return serialization::tagged_serializer<province_state::rgo_worker_data, economy::worked_instance>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::total_population, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::total_population, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::total_population, float>::size(obj.m_25.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_25.values, obj.m_25.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::total_population, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::last_controller_change, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::last_controller_change, date_tag>::size(obj.m_26.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_26.values, obj.m_26.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<province_state::last_controller_change, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::last_immigration, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::last_immigration, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::last_immigration, date_tag>::size(obj.m_27.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_27.values, obj.m_27.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<province_state::last_immigration, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::nationalism, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::nationalism, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::nationalism, float>::size(obj.m_28.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_28.values, obj.m_28.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::nationalism, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::siege_progress, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::siege_progress, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::siege_progress, float>::size(obj.m_29.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_29.values, obj.m_29.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::siege_progress, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::size(obj.m_30.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_30.values, obj.m_30.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::fort_upgrade_progress, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::size(obj.m_31.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_31.values, obj.m_31.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::railroad_upgrade_progress, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::size(obj.m_32.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_32.values, obj.m_32.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::naval_base_upgrade_progress, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::artisan_production_scale, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::artisan_production_scale, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::artisan_production_scale, float>::size(obj.m_33.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_33.values, obj.m_33.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::artisan_production_scale, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::net_migration_growth, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::net_migration_growth, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::net_migration_growth, float>::size(obj.m_34.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_34.values, obj.m_34.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::net_migration_growth, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::net_immigration_growth, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::net_immigration_growth, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::net_immigration_growth, float>::size(obj.m_35.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_35.values, obj.m_35.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::net_immigration_growth, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::monthly_population, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::monthly_population, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::monthly_population, float>::size(obj.m_36.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_36.values, obj.m_36.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::monthly_population, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::old_migration_growth, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::old_migration_growth, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::old_migration_growth, float>::size(obj.m_37.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_37.values, obj.m_37.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::old_migration_growth, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::old_immigration_growth, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::old_immigration_growth, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::old_immigration_growth, float>::size(obj.m_38.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_38.values, obj.m_38.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::old_immigration_growth, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::old_monthly_population, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::old_monthly_population, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::old_monthly_population, float>::size(obj.m_39.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_39.values, obj.m_39.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province_state::old_monthly_population, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::owner, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::owner, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::owner, nations::country_tag>::size(obj.m_40.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_40.values, obj.m_40.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<province_state::owner, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::controller, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::controller, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::controller, nations::country_tag>::size(obj.m_41.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_41.values, obj.m_41.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<province_state::controller, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::size(obj.m_42.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_42.values, obj.m_42.values + max, 0ui64, std::plus<>(), [&](population::rebel_faction_tag const& m) {
					 return serialization::tagged_serializer<province_state::rebel_controller, population::rebel_faction_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::size(obj.m_43.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_43.values, obj.m_43.values + max, 0ui64, std::plus<>(), [&](nations::state_tag const& m) {
					 return serialization::tagged_serializer<province_state::state_instance, nations::state_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::size(obj.m_44.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_44.values, obj.m_44.values + max, 0ui64, std::plus<>(), [&](military::army_orders_tag const& m) {
					 return serialization::tagged_serializer<province_state::orders, military::army_orders_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::size(obj.m_45.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_45.values, obj.m_45.values + max, 0ui64, std::plus<>(), [&](military::strategic_hq_tag const& m) {
					 return serialization::tagged_serializer<province_state::strat_hq, military::strategic_hq_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
);
	 }
};
#pragma warning( pop )

