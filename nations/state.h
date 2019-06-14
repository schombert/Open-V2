#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace state {
	class container;
}

template<>
class serialization::serializer<state::container>;

namespace state {
	struct is_slave_state;
	struct is_colonial;
	struct is_protectorate;
	struct dominant_religion;
	struct dominant_ideology;
	struct dominant_issue;
	struct region_id;
	struct crisis_tag;
	struct dominant_culture;
	struct state_capital;
	struct name;
	struct current_tension;
	struct administrative_efficiency;
	struct total_population;
	struct flashpoint_tension_focuses;
	struct project;
	struct owner_national_focus;
	struct owner;
	struct colonizers;
	struct factories;

	class alignas(64) container {
		 int32_t size_used = 0;
		 nations::state_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(nations::state_tag) + 63ui32) & ~63ui32]; 
			 nations::state_tag values[(sizeof(nations::state_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(nations::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::state_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::state_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(nations::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::state_tag)) - 1ui32) : uint32_t(900)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(900 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_0() { std::fill_n(values - 1, 1 + ((uint32_t(900 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(900 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_1() { std::fill_n(values - 1, 1 + ((uint32_t(900 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(900 + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_2() { std::fill_n(values - 1, 1 + ((uint32_t(900 + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(cultures::religion_tag) + 63ui32) & ~63ui32]; 
			 cultures::religion_tag values[(sizeof(cultures::religion_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::religion_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(ideologies::ideology_tag) + 63ui32) & ~63ui32]; 
			 ideologies::ideology_tag values[(sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(ideologies::ideology_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(ideologies::ideology_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(ideologies::ideology_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(issues::option_tag) + 63ui32) & ~63ui32]; 
			 issues::option_tag values[(sizeof(issues::option_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(issues::option_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(issues::option_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(issues::option_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(issues::option_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(issues::option_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(provinces::state_tag) + 63ui32) & ~63ui32]; 
			 provinces::state_tag values[(sizeof(provinces::state_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(provinces::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::state_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(provinces::state_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(provinces::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::state_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(cultures::national_tag) + 63ui32) & ~63ui32]; 
			 cultures::national_tag values[(sizeof(cultures::national_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(cultures::national_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::national_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::national_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(cultures::national_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::national_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(cultures::culture_tag) + 63ui32) & ~63ui32]; 
			 cultures::culture_tag values[(sizeof(cultures::culture_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::culture_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_8;
		 struct alignas(64) dtype_9 { 
			 uint8_t padding[(sizeof(provinces::province_tag) + 63ui32) & ~63ui32]; 
			 provinces::province_tag values[(sizeof(provinces::province_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_9() { std::uninitialized_value_construct_n(values - 1, (sizeof(provinces::province_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_9;
		 struct alignas(64) dtype_10 { 
			 uint8_t padding[(sizeof(text_data::text_tag) + 63ui32) & ~63ui32]; 
			 text_data::text_tag values[(sizeof(text_data::text_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_10() { std::uninitialized_value_construct_n(values - 1, (sizeof(text_data::text_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(text_data::text_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(text_data::text_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_10;
		 struct alignas(64) dtype_11 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(900))]; 
			 dtype_11() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_11;
		 struct alignas(64) dtype_12 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(900))]; 
			 dtype_12() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_12;
		 struct alignas(64) dtype_13 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(900))]; 
			 dtype_13() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_13;
		 struct alignas(64) dtype_14 { 
			 uint8_t padding[(sizeof(set_tag<nations::country_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<nations::country_tag> values[(sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(900))]; 
			 dtype_14() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<nations::country_tag>) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<nations::country_tag>)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_14;
		 struct alignas(64) dtype_15 { 
			 uint8_t padding[(sizeof(nations::pop_project) + 63ui32) & ~63ui32]; 
			 nations::pop_project values[(sizeof(nations::pop_project) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(nations::pop_project))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::pop_project)) - 1ui32) : uint32_t(900))]; 
			 dtype_15() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::pop_project) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(nations::pop_project))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::pop_project)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_15;
		 struct alignas(64) dtype_16 { 
			 uint8_t padding[(sizeof(modifiers::national_focus_tag) + 63ui32) & ~63ui32]; 
			 modifiers::national_focus_tag values[(sizeof(modifiers::national_focus_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(modifiers::national_focus_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::national_focus_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_16() { std::uninitialized_value_construct_n(values - 1, (sizeof(modifiers::national_focus_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(modifiers::national_focus_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::national_focus_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_16;
		 struct alignas(64) dtype_17 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(900))]; 
			 dtype_17() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_17;
		 struct alignas(64) dtype_18 { 
			 uint8_t padding[(sizeof(std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>) + 63ui32) & ~63ui32]; 
			 std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count> values[(sizeof(std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>)) - 1ui32) : uint32_t(900))]; 
			 dtype_18() { std::uninitialized_value_construct_n(values - 1, (sizeof(std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_18;
		 struct alignas(64) dtype_19 { 
			 uint8_t padding[(sizeof(std::array<economy::factory_instance, state::factories_count>) + 63ui32) & ~63ui32]; 
			 std::array<economy::factory_instance, state::factories_count> values[(sizeof(std::array<economy::factory_instance, state::factories_count>) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(std::array<economy::factory_instance, state::factories_count>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(std::array<economy::factory_instance, state::factories_count>)) - 1ui32) : uint32_t(900))]; 
			 dtype_19() { std::uninitialized_value_construct_n(values - 1, (sizeof(std::array<economy::factory_instance, state::factories_count>) <= 64 ? (uint32_t(900) + (64ui32 / uint32_t(sizeof(std::array<economy::factory_instance, state::factories_count>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(std::array<economy::factory_instance, state::factories_count>)) - 1ui32) : uint32_t(900)) + 1); }
		 } m_19;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = 900 - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = nations::state_tag(nations::state_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_slave_state>, bool> get(nations::state_tag i) const {
			 return bit_vector_test(m_0.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::state_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, is_slave_state>> {
			 bit_vector_set(m_0.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_slave_state>, tagged_array_view<bitfield_type, nations::state_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::state_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_slave_state>, tagged_array_view<bitfield_type const, nations::state_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::state_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_colonial>, bool> get(nations::state_tag i) const {
			 return bit_vector_test(m_1.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::state_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, is_colonial>> {
			 bit_vector_set(m_1.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_colonial>, tagged_array_view<bitfield_type, nations::state_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::state_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_colonial>, tagged_array_view<bitfield_type const, nations::state_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::state_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_protectorate>, bool> get(nations::state_tag i) const {
			 return bit_vector_test(m_2.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(nations::state_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, is_protectorate>> {
			 bit_vector_set(m_2.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_protectorate>, tagged_array_view<bitfield_type, nations::state_tag>> get_row() {
			 return tagged_array_view<bitfield_type, nations::state_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, is_protectorate>, tagged_array_view<bitfield_type const, nations::state_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, nations::state_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_religion>, cultures::religion_tag&> get(nations::state_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_religion>, cultures::religion_tag const&> get(nations::state_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_religion>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_religion>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_religion>, tagged_array_view<cultures::religion_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<cultures::religion_tag, nations::state_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_religion>, tagged_array_view<cultures::religion_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<cultures::religion_tag const, nations::state_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_ideology>, ideologies::ideology_tag&> get(nations::state_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_ideology>, ideologies::ideology_tag const&> get(nations::state_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_ideology>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_ideology>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_ideology>, tagged_array_view<ideologies::ideology_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<ideologies::ideology_tag, nations::state_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_ideology>, tagged_array_view<ideologies::ideology_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<ideologies::ideology_tag const, nations::state_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_issue>, issues::option_tag&> get(nations::state_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_issue>, issues::option_tag const&> get(nations::state_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_issue>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_issue>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_issue>, tagged_array_view<issues::option_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<issues::option_tag, nations::state_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_issue>, tagged_array_view<issues::option_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<issues::option_tag const, nations::state_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, region_id>, provinces::state_tag&> get(nations::state_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, region_id>, provinces::state_tag const&> get(nations::state_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, region_id>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, region_id>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, region_id>, tagged_array_view<provinces::state_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<provinces::state_tag, nations::state_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, region_id>, tagged_array_view<provinces::state_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<provinces::state_tag const, nations::state_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, crisis_tag>, cultures::national_tag&> get(nations::state_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, crisis_tag>, cultures::national_tag const&> get(nations::state_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, crisis_tag>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, crisis_tag>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, crisis_tag>, tagged_array_view<cultures::national_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<cultures::national_tag, nations::state_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, crisis_tag>, tagged_array_view<cultures::national_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<cultures::national_tag const, nations::state_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_culture>, cultures::culture_tag&> get(nations::state_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_culture>, cultures::culture_tag const&> get(nations::state_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_culture>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, dominant_culture>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_culture>, tagged_array_view<cultures::culture_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<cultures::culture_tag, nations::state_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, dominant_culture>, tagged_array_view<cultures::culture_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<cultures::culture_tag const, nations::state_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, state_capital>, provinces::province_tag&> get(nations::state_tag i) {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, state_capital>, provinces::province_tag const&> get(nations::state_tag i) const {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, state_capital>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, state_capital>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, state_capital>, tagged_array_view<provinces::province_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<provinces::province_tag, nations::state_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, state_capital>, tagged_array_view<provinces::province_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<provinces::province_tag const, nations::state_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, name>, text_data::text_tag&> get(nations::state_tag i) {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, name>, text_data::text_tag const&> get(nations::state_tag i) const {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, name>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, name>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, name>, tagged_array_view<text_data::text_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<text_data::text_tag, nations::state_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, name>, tagged_array_view<text_data::text_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<text_data::text_tag const, nations::state_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, current_tension>, float&> get(nations::state_tag i) {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, current_tension>, float const&> get(nations::state_tag i) const {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, current_tension>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, current_tension>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, current_tension>, tagged_array_view<float, nations::state_tag>> get_row() {
			 return tagged_array_view<float, nations::state_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, current_tension>, tagged_array_view<float const, nations::state_tag>> get_row() const {
			 return tagged_array_view<float const, nations::state_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, administrative_efficiency>, float&> get(nations::state_tag i) {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, administrative_efficiency>, float const&> get(nations::state_tag i) const {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, administrative_efficiency>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, administrative_efficiency>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, administrative_efficiency>, tagged_array_view<float, nations::state_tag>> get_row() {
			 return tagged_array_view<float, nations::state_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, administrative_efficiency>, tagged_array_view<float const, nations::state_tag>> get_row() const {
			 return tagged_array_view<float const, nations::state_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, total_population>, float&> get(nations::state_tag i) {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, total_population>, float const&> get(nations::state_tag i) const {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, total_population>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, total_population>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, total_population>, tagged_array_view<float, nations::state_tag>> get_row() {
			 return tagged_array_view<float, nations::state_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, total_population>, tagged_array_view<float const, nations::state_tag>> get_row() const {
			 return tagged_array_view<float const, nations::state_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, flashpoint_tension_focuses>, set_tag<nations::country_tag>&> get(nations::state_tag i) {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, flashpoint_tension_focuses>, set_tag<nations::country_tag> const&> get(nations::state_tag i) const {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, flashpoint_tension_focuses>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, flashpoint_tension_focuses>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, flashpoint_tension_focuses>, tagged_array_view<set_tag<nations::country_tag>, nations::state_tag>> get_row() {
			 return tagged_array_view<set_tag<nations::country_tag>, nations::state_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, flashpoint_tension_focuses>, tagged_array_view<set_tag<nations::country_tag> const, nations::state_tag>> get_row() const {
			 return tagged_array_view<set_tag<nations::country_tag> const, nations::state_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, project>, nations::pop_project&> get(nations::state_tag i) {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, project>, nations::pop_project const&> get(nations::state_tag i) const {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, project>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, project>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, project>, tagged_array_view<nations::pop_project, nations::state_tag>> get_row() {
			 return tagged_array_view<nations::pop_project, nations::state_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, project>, tagged_array_view<nations::pop_project const, nations::state_tag>> get_row() const {
			 return tagged_array_view<nations::pop_project const, nations::state_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner_national_focus>, modifiers::national_focus_tag&> get(nations::state_tag i) {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner_national_focus>, modifiers::national_focus_tag const&> get(nations::state_tag i) const {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, owner_national_focus>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, owner_national_focus>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner_national_focus>, tagged_array_view<modifiers::national_focus_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<modifiers::national_focus_tag, nations::state_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner_national_focus>, tagged_array_view<modifiers::national_focus_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<modifiers::national_focus_tag const, nations::state_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner>, nations::country_tag&> get(nations::state_tag i) {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner>, nations::country_tag const&> get(nations::state_tag i) const {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, owner>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, owner>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner>, tagged_array_view<nations::country_tag, nations::state_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, nations::state_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, owner>, tagged_array_view<nations::country_tag const, nations::state_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, nations::state_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, colonizers>, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>&> get(nations::state_tag i) {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, colonizers>, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count> const&> get(nations::state_tag i) const {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, colonizers>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, colonizers>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, colonizers>, tagged_array_view<std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>, nations::state_tag>> get_row() {
			 return tagged_array_view<std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>, nations::state_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, colonizers>, tagged_array_view<std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count> const, nations::state_tag>> get_row() const {
			 return tagged_array_view<std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count> const, nations::state_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, factories>, std::array<economy::factory_instance, state::factories_count>&> get(nations::state_tag i) {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, factories>, std::array<economy::factory_instance, state::factories_count> const&> get(nations::state_tag i) const {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, factories>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(nations::state_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, factories>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, factories>, tagged_array_view<std::array<economy::factory_instance, state::factories_count>, nations::state_tag>> get_row() {
			 return tagged_array_view<std::array<economy::factory_instance, state::factories_count>, nations::state_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, factories>, tagged_array_view<std::array<economy::factory_instance, state::factories_count> const, nations::state_tag>> get_row() const {
			 return tagged_array_view<std::array<economy::factory_instance, state::factories_count> const, nations::state_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }

		 nations::state_tag get_new() {
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
		 void release(nations::state_tag i) {
			 set<is_slave_state>(i, false);
			 set<is_colonial>(i, false);
			 set<is_protectorate>(i, false);
			 set<dominant_religion>(i, cultures::religion_tag());
			 set<dominant_ideology>(i, ideologies::ideology_tag());
			 set<dominant_issue>(i, issues::option_tag());
			 set<region_id>(i, provinces::state_tag());
			 set<crisis_tag>(i, cultures::national_tag());
			 set<dominant_culture>(i, cultures::culture_tag());
			 set<state_capital>(i, provinces::province_tag());
			 set<name>(i, text_data::text_tag());
			 set<current_tension>(i, float());
			 set<administrative_efficiency>(i, float());
			 set<total_population>(i, float());
			 set<flashpoint_tension_focuses>(i, set_tag<nations::country_tag>());
			 set<project>(i, nations::pop_project());
			 set<owner_national_focus>(i, modifiers::national_focus_tag());
			 set<owner>(i, nations::country_tag());
			 set<colonizers>(i, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>());
			 set<factories>(i, std::array<economy::factory_instance, state::factories_count>());
			 m_index.values[to_index(first_free)] = i;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == nations::state_tag(nations::state_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(nations::state_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == nations::state_tag(nations::state_tag::value_base_t(i))) f(nations::state_tag(nations::state_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == nations::state_tag(nations::state_tag::value_base_t(i))) f(nations::state_tag(nations::state_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == nations::state_tag(nations::state_tag::value_base_t(i))) { if(f(nations::state_tag(nations::state_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<state::container, state::is_slave_state> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::state_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::is_colonial> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::state_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::is_protectorate> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, nations::state_tag>;
	 using const_row = tagged_array_view<bitfield_type const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::dominant_religion> {
	static constexpr bool value = true;
	 using type = cultures::religion_tag&;
	 using const_type = cultures::religion_tag const&;
	 using row = tagged_array_view<cultures::religion_tag, nations::state_tag>;
	 using const_row = tagged_array_view<cultures::religion_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::dominant_ideology> {
	static constexpr bool value = true;
	 using type = ideologies::ideology_tag&;
	 using const_type = ideologies::ideology_tag const&;
	 using row = tagged_array_view<ideologies::ideology_tag, nations::state_tag>;
	 using const_row = tagged_array_view<ideologies::ideology_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::dominant_issue> {
	static constexpr bool value = true;
	 using type = issues::option_tag&;
	 using const_type = issues::option_tag const&;
	 using row = tagged_array_view<issues::option_tag, nations::state_tag>;
	 using const_row = tagged_array_view<issues::option_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::region_id> {
	static constexpr bool value = true;
	 using type = provinces::state_tag&;
	 using const_type = provinces::state_tag const&;
	 using row = tagged_array_view<provinces::state_tag, nations::state_tag>;
	 using const_row = tagged_array_view<provinces::state_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::crisis_tag> {
	static constexpr bool value = true;
	 using type = cultures::national_tag&;
	 using const_type = cultures::national_tag const&;
	 using row = tagged_array_view<cultures::national_tag, nations::state_tag>;
	 using const_row = tagged_array_view<cultures::national_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::dominant_culture> {
	static constexpr bool value = true;
	 using type = cultures::culture_tag&;
	 using const_type = cultures::culture_tag const&;
	 using row = tagged_array_view<cultures::culture_tag, nations::state_tag>;
	 using const_row = tagged_array_view<cultures::culture_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::state_capital> {
	static constexpr bool value = true;
	 using type = provinces::province_tag&;
	 using const_type = provinces::province_tag const&;
	 using row = tagged_array_view<provinces::province_tag, nations::state_tag>;
	 using const_row = tagged_array_view<provinces::province_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::name> {
	static constexpr bool value = true;
	 using type = text_data::text_tag&;
	 using const_type = text_data::text_tag const&;
	 using row = tagged_array_view<text_data::text_tag, nations::state_tag>;
	 using const_row = tagged_array_view<text_data::text_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::current_tension> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::state_tag>;
	 using const_row = tagged_array_view<float const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::administrative_efficiency> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::state_tag>;
	 using const_row = tagged_array_view<float const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::total_population> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, nations::state_tag>;
	 using const_row = tagged_array_view<float const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::flashpoint_tension_focuses> {
	static constexpr bool value = true;
	 using type = set_tag<nations::country_tag>&;
	 using const_type = set_tag<nations::country_tag> const&;
	 using row = tagged_array_view<set_tag<nations::country_tag>, nations::state_tag>;
	 using const_row = tagged_array_view<set_tag<nations::country_tag> const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::project> {
	static constexpr bool value = true;
	 using type = nations::pop_project&;
	 using const_type = nations::pop_project const&;
	 using row = tagged_array_view<nations::pop_project, nations::state_tag>;
	 using const_row = tagged_array_view<nations::pop_project const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::owner_national_focus> {
	static constexpr bool value = true;
	 using type = modifiers::national_focus_tag&;
	 using const_type = modifiers::national_focus_tag const&;
	 using row = tagged_array_view<modifiers::national_focus_tag, nations::state_tag>;
	 using const_row = tagged_array_view<modifiers::national_focus_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::owner> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, nations::state_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::colonizers> {
	static constexpr bool value = true;
	 using type = std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>&;
	 using const_type = std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count> const&;
	 using row = tagged_array_view<std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>, nations::state_tag>;
	 using const_row = tagged_array_view<std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count> const, nations::state_tag>;
};
template<>
struct supports_index<state::container, state::factories> {
	static constexpr bool value = true;
	 using type = std::array<economy::factory_instance, state::factories_count>&;
	 using const_type = std::array<economy::factory_instance, state::factories_count> const&;
	 using row = tagged_array_view<std::array<economy::factory_instance, state::factories_count>, nations::state_tag>;
	 using const_row = tagged_array_view<std::array<economy::factory_instance, state::factories_count> const, nations::state_tag>;
};

template<>
class serialization::serializer<state::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, state::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == nations::state_tag(nations::state_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = nations::state_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		 serialization::serialize_array(output, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::region_id, provinces::state_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::region_id, provinces::state_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::state_capital, provinces::province_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::state_capital, provinces::province_tag>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::name, text_data::text_tag>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::name, text_data::text_tag>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::current_tension, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::current_tension, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::current_tension, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::administrative_efficiency, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::administrative_efficiency, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::total_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::total_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::total_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::total_population, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::total_population, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::total_population, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::project, nations::pop_project>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::project, nations::pop_project>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner, nations::country_tag>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner, nations::country_tag>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, state::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = nations::state_tag();
		for(int32_t i = 900 - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = nations::state_tag(nations::state_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = nations::state_tag(nations::state_tag::value_base_t(i));
			}
		}
		 serialization::deserialize_array(input, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::region_id, provinces::state_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::region_id, provinces::state_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::state_capital, provinces::province_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::state_capital, provinces::province_tag>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::name, text_data::text_tag>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::name, text_data::text_tag>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::current_tension, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::current_tension, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::current_tension, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::administrative_efficiency, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::administrative_efficiency, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::total_population, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::total_population, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::total_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::total_population, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::total_population, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::total_population, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::project, nations::pop_project>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::project, nations::pop_project>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner, nations::country_tag>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::owner, nations::country_tag>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static size_t size(state::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(nations::state_tag) * obj.size_used+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](cultures::religion_tag const& m) {
					 return serialization::tagged_serializer<state::dominant_religion, cultures::religion_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](ideologies::ideology_tag const& m) {
					 return serialization::tagged_serializer<state::dominant_ideology, ideologies::ideology_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](issues::option_tag const& m) {
					 return serialization::tagged_serializer<state::dominant_issue, issues::option_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::region_id, provinces::state_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::region_id, provinces::state_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::region_id, provinces::state_tag>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](provinces::state_tag const& m) {
					 return serialization::tagged_serializer<state::region_id, provinces::state_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](cultures::national_tag const& m) {
					 return serialization::tagged_serializer<state::crisis_tag, cultures::national_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](cultures::culture_tag const& m) {
					 return serialization::tagged_serializer<state::dominant_culture, cultures::culture_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::state_capital, provinces::province_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::state_capital, provinces::province_tag>::size(obj.m_9.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_9.values, obj.m_9.values + max, 0ui64, std::plus<>(), [&](provinces::province_tag const& m) {
					 return serialization::tagged_serializer<state::state_capital, provinces::province_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::name, text_data::text_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::name, text_data::text_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::name, text_data::text_tag>::size(obj.m_10.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_10.values, obj.m_10.values + max, 0ui64, std::plus<>(), [&](text_data::text_tag const& m) {
					 return serialization::tagged_serializer<state::name, text_data::text_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::current_tension, float>::size());
			 } else if constexpr(serialization::tagged_serializer<state::current_tension, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::current_tension, float>::size(obj.m_11.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_11.values, obj.m_11.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<state::current_tension, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::administrative_efficiency, float>::size());
			 } else if constexpr(serialization::tagged_serializer<state::administrative_efficiency, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::administrative_efficiency, float>::size(obj.m_12.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_12.values, obj.m_12.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<state::administrative_efficiency, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::total_population, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::total_population, float>::size());
			 } else if constexpr(serialization::tagged_serializer<state::total_population, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::total_population, float>::size(obj.m_13.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_13.values, obj.m_13.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<state::total_population, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::size(obj.m_14.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_14.values, obj.m_14.values + max, 0ui64, std::plus<>(), [&](set_tag<nations::country_tag> const& m) {
					 return serialization::tagged_serializer<state::flashpoint_tension_focuses, set_tag<nations::country_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::project, nations::pop_project>::size());
			 } else if constexpr(serialization::tagged_serializer<state::project, nations::pop_project>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::project, nations::pop_project>::size(obj.m_15.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_15.values, obj.m_15.values + max, 0ui64, std::plus<>(), [&](nations::pop_project const& m) {
					 return serialization::tagged_serializer<state::project, nations::pop_project>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::size(obj.m_16.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_16.values, obj.m_16.values + max, 0ui64, std::plus<>(), [&](modifiers::national_focus_tag const& m) {
					 return serialization::tagged_serializer<state::owner_national_focus, modifiers::national_focus_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::owner, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<state::owner, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::owner, nations::country_tag>::size(obj.m_17.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_17.values, obj.m_17.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<state::owner, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::size());
			 } else if constexpr(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::size(obj.m_18.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_18.values, obj.m_18.values + max, 0ui64, std::plus<>(), [&](std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count> const& m) {
					 return serialization::tagged_serializer<state::colonizers, std::array<std::pair<nations::country_tag, int32_t>, state::colonizers_count>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::size());
			 } else if constexpr(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::size(obj.m_19.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_19.values, obj.m_19.values + max, 0ui64, std::plus<>(), [&](std::array<economy::factory_instance, state::factories_count> const& m) {
					 return serialization::tagged_serializer<state::factories, std::array<economy::factory_instance, state::factories_count>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
);
	 }
};
#pragma warning( pop )

