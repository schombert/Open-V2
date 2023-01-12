#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace war {
	class container;
}

template<>
class serialization::serializer<war::container>;

namespace war {
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
	struct is_great_war;
	struct is_world_war;

constexpr int32_t max_count = war::container_size;

	class alignas(64) container {
		 int32_t size_used = 0;
		 military::war_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(military::war_tag) + 63ui32) & ~63ui32]; 
			 military::war_tag values[(sizeof(military::war_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(military::war_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::war_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::war_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(military::war_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::war_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_0() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_1() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 uint8_t padding[(sizeof(set_tag<military::naval_control>) + 63ui32) & ~63ui32]; 
			 set_tag<military::naval_control> values[(sizeof(set_tag<military::naval_control>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(set_tag<military::naval_control>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::naval_control>)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_2() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<military::naval_control>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(set_tag<military::naval_control>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::naval_control>)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_8;
		 struct alignas(64) dtype_9 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_9() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_9;
		 struct alignas(64) dtype_10 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_10() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_10;
		 struct alignas(64) dtype_11 { 
			 uint8_t padding[(sizeof(array_tag<military::war_goal, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::war_goal, int32_t, false> values[(sizeof(array_tag<military::war_goal, int32_t, false>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(array_tag<military::war_goal, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::war_goal, int32_t, false>)) - 1ui32) : uint32_t(war::container_size))]; 
			 dtype_11() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::war_goal, int32_t, false>) <= 64 ? (uint32_t(war::container_size) + (64ui32 / uint32_t(sizeof(array_tag<military::war_goal, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::war_goal, int32_t, false>)) - 1ui32) : uint32_t(war::container_size)) + 1); }
		 } m_11;
		 struct alignas(64) dtype_12 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(war::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_12() { std::fill_n(values - 1, 1 + ((uint32_t(war::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_12;
		 struct alignas(64) dtype_13 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(war::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_13() { std::fill_n(values - 1, 1 + ((uint32_t(war::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_13;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = war::container_size - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = military::war_tag(military::war_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::attackers>, set_tag<nations::country_tag>&> get(military::war_tag i) {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::attackers>, set_tag<nations::country_tag> const&> get(military::war_tag i) const {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::attackers>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::attackers>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::attackers>, tagged_array_view<set_tag<nations::country_tag>, military::war_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, military::war_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::attackers>, tagged_array_view<set_tag<nations::country_tag> const, military::war_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, military::war_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::defenders>, set_tag<nations::country_tag>&> get(military::war_tag i) {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::defenders>, set_tag<nations::country_tag> const&> get(military::war_tag i) const {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::defenders>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::defenders>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::defenders>, tagged_array_view<set_tag<nations::country_tag>, military::war_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, military::war_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::defenders>, tagged_array_view<set_tag<nations::country_tag> const, military::war_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, military::war_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::naval_control_set>, set_tag<military::naval_control>&> get(military::war_tag i) {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::naval_control_set>, set_tag<military::naval_control> const&> get(military::war_tag i) const {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::naval_control_set>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::naval_control_set>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::naval_control_set>, tagged_array_view<set_tag<military::naval_control>, military::war_tag>> get_row() {
			 return tagged_array_view<set_tag<military::naval_control>, military::war_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::naval_control_set>, tagged_array_view<set_tag<military::naval_control> const, military::war_tag>> get_row() const {
			 return tagged_array_view<set_tag<military::naval_control> const, military::war_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::start_date>, date_tag&> get(military::war_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::start_date>, date_tag const&> get(military::war_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::start_date>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::start_date>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::start_date>, tagged_array_view<date_tag, military::war_tag>> get_row() {
			 return tagged_array_view<date_tag, military::war_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::start_date>, tagged_array_view<date_tag const, military::war_tag>> get_row() const {
			 return tagged_array_view<date_tag const, military::war_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::current_war_score>, float&> get(military::war_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::current_war_score>, float const&> get(military::war_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::current_war_score>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::current_war_score>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::current_war_score>, tagged_array_view<float, military::war_tag>> get_row() {
			 return tagged_array_view<float, military::war_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::current_war_score>, tagged_array_view<float const, military::war_tag>> get_row() const {
			 return tagged_array_view<float const, military::war_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::name>, text_data::text_tag&> get(military::war_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::name>, text_data::text_tag const&> get(military::war_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::name>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::name>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::name>, tagged_array_view<text_data::text_tag, military::war_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, military::war_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::name>, tagged_array_view<text_data::text_tag const, military::war_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, military::war_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::first_adj>, text_data::text_tag&> get(military::war_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::first_adj>, text_data::text_tag const&> get(military::war_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::first_adj>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::first_adj>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::first_adj>, tagged_array_view<text_data::text_tag, military::war_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, military::war_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::first_adj>, tagged_array_view<text_data::text_tag const, military::war_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, military::war_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::second>, text_data::text_tag&> get(military::war_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::second>, text_data::text_tag const&> get(military::war_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::second>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::second>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::second>, tagged_array_view<text_data::text_tag, military::war_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, military::war_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::second>, tagged_array_view<text_data::text_tag const, military::war_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, military::war_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::state_name>, text_data::text_tag&> get(military::war_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::state_name>, text_data::text_tag const&> get(military::war_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::state_name>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::state_name>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::state_name>, tagged_array_view<text_data::text_tag, military::war_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, military::war_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::state_name>, tagged_array_view<text_data::text_tag const, military::war_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, military::war_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_attacker>, nations::country_tag&> get(military::war_tag i) {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_attacker>, nations::country_tag const&> get(military::war_tag i) const {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::primary_attacker>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::primary_attacker>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_attacker>, tagged_array_view<nations::country_tag, military::war_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, military::war_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_attacker>, tagged_array_view<nations::country_tag const, military::war_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, military::war_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_defender>, nations::country_tag&> get(military::war_tag i) {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_defender>, nations::country_tag const&> get(military::war_tag i) const {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::primary_defender>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::primary_defender>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_defender>, tagged_array_view<nations::country_tag, military::war_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, military::war_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::primary_defender>, tagged_array_view<nations::country_tag const, military::war_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, military::war_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::war_goals>, array_tag<military::war_goal, int32_t, false>&> get(military::war_tag i) {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::war_goals>, array_tag<military::war_goal, int32_t, false> const&> get(military::war_tag i) const {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::war_goals>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::war_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, war::war_goals>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::war_goals>, tagged_array_view<array_tag<military::war_goal, int32_t, false>, military::war_tag>> get_row() {
			 return tagged_array_view<array_tag<military::war_goal, int32_t, false>, military::war_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::war_goals>, tagged_array_view<array_tag<military::war_goal, int32_t, false> const, military::war_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::war_goal, int32_t, false> const, military::war_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::is_great_war>, bool> get(military::war_tag i) const {
			 return bit_vector_test(m_12.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(military::war_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, war::is_great_war>> {
			 bit_vector_set(m_12.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::is_great_war>, tagged_array_view<bitfield_type, military::war_tag>> get_row() {
			 return tagged_array_view<bitfield_type, military::war_tag>(m_12.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::is_great_war>, tagged_array_view<bitfield_type const, military::war_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, military::war_tag>(m_12.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::is_world_war>, bool> get(military::war_tag i) const {
			 return bit_vector_test(m_13.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(military::war_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, war::is_world_war>> {
			 bit_vector_set(m_13.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::is_world_war>, tagged_array_view<bitfield_type, military::war_tag>> get_row() {
			 return tagged_array_view<bitfield_type, military::war_tag>(m_13.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, war::is_world_war>, tagged_array_view<bitfield_type const, military::war_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, military::war_tag>(m_13.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }

		 military::war_tag get_new() {
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
		 void release(military::war_tag i) {
			 set<war::attackers>(i, set_tag<nations::country_tag>());
			 set<war::defenders>(i, set_tag<nations::country_tag>());
			 set<war::naval_control_set>(i, set_tag<military::naval_control>());
			 set<war::start_date>(i, date_tag());
			 set<war::current_war_score>(i, float());
			 set<war::name>(i, text_data::text_tag());
			 set<war::first_adj>(i, text_data::text_tag());
			 set<war::second>(i, text_data::text_tag());
			 set<war::state_name>(i, text_data::text_tag());
			 set<war::primary_attacker>(i, nations::country_tag());
			 set<war::primary_defender>(i, nations::country_tag());
			 set<war::war_goals>(i, array_tag<military::war_goal, int32_t, false>());
			 set<war::is_great_war>(i, false);
			 set<war::is_world_war>(i, false);
			 m_index.values[to_index(i)] = first_free;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == military::war_tag(military::war_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(military::war_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::war_tag(military::war_tag::value_base_t(i))) f(military::war_tag(military::war_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == military::war_tag(military::war_tag::value_base_t(i))) f(military::war_tag(military::war_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::war_tag(military::war_tag::value_base_t(i))) { if(f(military::war_tag(military::war_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<war::container, war::attackers> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, military::war_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::defenders> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, military::war_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::naval_control_set> {
	static constexpr bool value = true;
	 using type = set_tag<military::naval_control>&;
	 using const_type = set_tag<military::naval_control> const&;
	 using row = tagged_array_view<set_tag<military::naval_control>, military::war_tag>;
	 using const_row = tagged_array_view<set_tag<military::naval_control> const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::start_date> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, military::war_tag>;
	 using const_row = tagged_array_view<date_tag const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::current_war_score> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::war_tag>;
	 using const_row = tagged_array_view<float const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::name> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, military::war_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::first_adj> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, military::war_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::second> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, military::war_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::state_name> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, military::war_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::primary_attacker> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, military::war_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::primary_defender> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, military::war_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::war_goals> {
	static constexpr bool value = true;
	 using type = array_tag<military::war_goal, int32_t, false>&;
	 using const_type = array_tag<military::war_goal, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::war_goal, int32_t, false>, military::war_tag>;
	 using const_row = tagged_array_view<array_tag<military::war_goal, int32_t, false> const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::is_great_war> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, military::war_tag>;
	 using const_row = tagged_array_view<bitfield_type const, military::war_tag>;
};
template<>
struct supports_index<war::container, war::is_world_war> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, military::war_tag>;
	 using const_row = tagged_array_view<bitfield_type const, military::war_tag>;
};

template<>
class serialization::serializer<war::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, war::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == military::war_tag(military::war_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = military::war_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::start_date, date_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::start_date, date_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::current_war_score, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::current_war_score, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::current_war_score, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::name, text_data::text_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::name, text_data::text_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::first_adj, text_data::text_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::first_adj, text_data::text_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::second, text_data::text_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::second, text_data::text_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::state_name, text_data::text_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::state_name, text_data::text_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_defender, nations::country_tag>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_defender, nations::country_tag>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		 serialization::serialize_array(output, obj.m_12.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_13.values, uint32_t(obj.size_used + 7) / 8ui32);
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, war::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = military::war_tag();
		for(int32_t i = war::container_size - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = military::war_tag(military::war_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = military::war_tag(military::war_tag::value_base_t(i));
			}
		}
		if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::start_date, date_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::start_date, date_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::current_war_score, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::current_war_score, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::current_war_score, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::name, text_data::text_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::name, text_data::text_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::first_adj, text_data::text_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::first_adj, text_data::text_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::second, text_data::text_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::second, text_data::text_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::state_name, text_data::text_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::state_name, text_data::text_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_defender, nations::country_tag>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::primary_defender, nations::country_tag>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		 serialization::deserialize_array(input, obj.m_12.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_13.values, uint32_t(obj.size_used + 7) / 8ui32);
	 }
	 template<typename ... CONTEXT>
	 static size_t size(war::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(military::war_tag) * obj.size_used+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::size(obj.m_0.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_0.values, obj.m_0.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<war::attackers, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::size(obj.m_1.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_1.values, obj.m_1.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<war::defenders, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::size());
			 } else if constexpr(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::size(obj.m_2.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_2.values, obj.m_2.values + max, 0ui64, std::plus<>(), [&](set_tag<military::naval_control> const& m) {
					 return serialization::tagged_serializer<war::naval_control_set, set_tag<military::naval_control>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::start_date, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<war::start_date, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::start_date, date_tag>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<war::start_date, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::current_war_score, float>::size());
			 } else if constexpr(serialization::tagged_serializer<war::current_war_score, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::current_war_score, float>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<war::current_war_score, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::name, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<war::name, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::name, text_data::text_tag>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<war::name, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::first_adj, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::first_adj, text_data::text_tag>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<war::first_adj, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::second, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<war::second, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::second, text_data::text_tag>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<war::second, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::state_name, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<war::state_name, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::state_name, text_data::text_tag>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<war::state_name, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::size(obj.m_9.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_9.values, obj.m_9.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<war::primary_attacker, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::primary_defender, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::primary_defender, nations::country_tag>::size(obj.m_10.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_10.values, obj.m_10.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<war::primary_defender, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::size(obj.m_11.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_11.values, obj.m_11.values + max, 0ui64, std::plus<>(), [&](array_tag<military::war_goal, int32_t, false> const& m) {
					 return serialization::tagged_serializer<war::war_goals, array_tag<military::war_goal, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32));
	 }
};
#pragma warning( pop )

