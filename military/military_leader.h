#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace military_leader {
	class container;
}

template<>
class serialization::serializer<military_leader::container>;

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

constexpr int32_t max_count = military_leader::container_size;

	class alignas(64) container {
		 int32_t size_used = 0;
		 military::leader_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(military::leader_tag) + 63ui32) & ~63ui32]; 
			 military::leader_tag values[(sizeof(military::leader_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(military::leader_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_tag)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::leader_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(military::leader_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_tag)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 uint8_t padding[(sizeof(vector_backed_string<char16_t>) + 63ui32) & ~63ui32]; 
			 vector_backed_string<char16_t> values[(sizeof(vector_backed_string<char16_t>) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_0() { std::uninitialized_value_construct_n(values - 1, (sizeof(vector_backed_string<char16_t>) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 uint8_t padding[(sizeof(vector_backed_string<char16_t>) + 63ui32) & ~63ui32]; 
			 vector_backed_string<char16_t> values[(sizeof(vector_backed_string<char16_t>) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_1() { std::uninitialized_value_construct_n(values - 1, (sizeof(vector_backed_string<char16_t>) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(vector_backed_string<char16_t>)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_2() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(graphics::texture_tag) + 63ui32) & ~63ui32]; 
			 graphics::texture_tag values[(sizeof(graphics::texture_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(graphics::texture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(graphics::texture_tag)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(graphics::texture_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(graphics::texture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(graphics::texture_tag)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(military::leader_trait_tag) + 63ui32) & ~63ui32]; 
			 military::leader_trait_tag values[(sizeof(military::leader_trait_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(military::leader_trait_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_trait_tag)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::leader_trait_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(military::leader_trait_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_trait_tag)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(military::leader_trait_tag) + 63ui32) & ~63ui32]; 
			 military::leader_trait_tag values[(sizeof(military::leader_trait_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(military::leader_trait_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_trait_tag)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::leader_trait_tag) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(military::leader_trait_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_trait_tag)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_8;
		 struct alignas(64) dtype_9 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_9() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_9;
		 struct alignas(64) dtype_10 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_10() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_10;
		 struct alignas(64) dtype_11 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_11() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_11;
		 struct alignas(64) dtype_12 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_12() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_12;
		 struct alignas(64) dtype_13 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size))]; 
			 dtype_13() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(military_leader::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(military_leader::container_size)) + 1); }
		 } m_13;
		 struct alignas(64) dtype_14 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(military_leader::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_14() { std::fill_n(values - 1, 1 + ((uint32_t(military_leader::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_14;
		 struct alignas(64) dtype_15 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(military_leader::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_15() { std::fill_n(values - 1, 1 + ((uint32_t(military_leader::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_15;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = military_leader::container_size - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = military::leader_tag(military::leader_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::first_name>, vector_backed_string<char16_t>&> get(military::leader_tag i) {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::first_name>, vector_backed_string<char16_t> const&> get(military::leader_tag i) const {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::first_name>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::first_name>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::first_name>, tagged_array_view<vector_backed_string<char16_t>, military::leader_tag>> get_row() {
			 return tagged_array_view<vector_backed_string<char16_t>, military::leader_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::first_name>, tagged_array_view<vector_backed_string<char16_t> const, military::leader_tag>> get_row() const {
			 return tagged_array_view<vector_backed_string<char16_t> const, military::leader_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::last_name>, vector_backed_string<char16_t>&> get(military::leader_tag i) {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::last_name>, vector_backed_string<char16_t> const&> get(military::leader_tag i) const {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::last_name>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::last_name>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::last_name>, tagged_array_view<vector_backed_string<char16_t>, military::leader_tag>> get_row() {
			 return tagged_array_view<vector_backed_string<char16_t>, military::leader_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::last_name>, tagged_array_view<vector_backed_string<char16_t> const, military::leader_tag>> get_row() const {
			 return tagged_array_view<vector_backed_string<char16_t> const, military::leader_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::creation_date>, date_tag&> get(military::leader_tag i) {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::creation_date>, date_tag const&> get(military::leader_tag i) const {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::creation_date>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::creation_date>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::creation_date>, tagged_array_view<date_tag, military::leader_tag>> get_row() {
			 return tagged_array_view<date_tag, military::leader_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::creation_date>, tagged_array_view<date_tag const, military::leader_tag>> get_row() const {
			 return tagged_array_view<date_tag const, military::leader_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::portrait>, graphics::texture_tag&> get(military::leader_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::portrait>, graphics::texture_tag const&> get(military::leader_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::portrait>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::portrait>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::portrait>, tagged_array_view<graphics::texture_tag, military::leader_tag>> get_row() {
			 return tagged_array_view<graphics::texture_tag, military::leader_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::portrait>, tagged_array_view<graphics::texture_tag const, military::leader_tag>> get_row() const {
			 return tagged_array_view<graphics::texture_tag const, military::leader_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::personality>, military::leader_trait_tag&> get(military::leader_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::personality>, military::leader_trait_tag const&> get(military::leader_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::personality>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::personality>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::personality>, tagged_array_view<military::leader_trait_tag, military::leader_tag>> get_row() {
			 return tagged_array_view<military::leader_trait_tag, military::leader_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::personality>, tagged_array_view<military::leader_trait_tag const, military::leader_tag>> get_row() const {
			 return tagged_array_view<military::leader_trait_tag const, military::leader_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::background>, military::leader_trait_tag&> get(military::leader_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::background>, military::leader_trait_tag const&> get(military::leader_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::background>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::background>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::background>, tagged_array_view<military::leader_trait_tag, military::leader_tag>> get_row() {
			 return tagged_array_view<military::leader_trait_tag, military::leader_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::background>, tagged_array_view<military::leader_trait_tag const, military::leader_tag>> get_row() const {
			 return tagged_array_view<military::leader_trait_tag const, military::leader_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::organisation>, float&> get(military::leader_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::organisation>, float const&> get(military::leader_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::organisation>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::organisation>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::organisation>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::organisation>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::morale>, float&> get(military::leader_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::morale>, float const&> get(military::leader_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::morale>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::morale>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::morale>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::morale>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::attack>, float&> get(military::leader_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::attack>, float const&> get(military::leader_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::attack>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::attack>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::attack>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::attack>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::defence>, float&> get(military::leader_tag i) {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::defence>, float const&> get(military::leader_tag i) const {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::defence>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::defence>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::defence>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::defence>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reconnaissance>, float&> get(military::leader_tag i) {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reconnaissance>, float const&> get(military::leader_tag i) const {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::reconnaissance>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::reconnaissance>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reconnaissance>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reconnaissance>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::speed>, float&> get(military::leader_tag i) {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::speed>, float const&> get(military::leader_tag i) const {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::speed>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::speed>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::speed>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::speed>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::experience>, float&> get(military::leader_tag i) {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::experience>, float const&> get(military::leader_tag i) const {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::experience>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::experience>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::experience>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::experience>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reliability>, float&> get(military::leader_tag i) {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reliability>, float const&> get(military::leader_tag i) const {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::reliability>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::leader_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, military_leader::reliability>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reliability>, tagged_array_view<float, military::leader_tag>> get_row() {
			 return tagged_array_view<float, military::leader_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::reliability>, tagged_array_view<float const, military::leader_tag>> get_row() const {
			 return tagged_array_view<float const, military::leader_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::is_attached>, bool> get(military::leader_tag i) const {
			 return bit_vector_test(m_14.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(military::leader_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, military_leader::is_attached>> {
			 bit_vector_set(m_14.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::is_attached>, tagged_array_view<bitfield_type, military::leader_tag>> get_row() {
			 return tagged_array_view<bitfield_type, military::leader_tag>(m_14.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::is_attached>, tagged_array_view<bitfield_type const, military::leader_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, military::leader_tag>(m_14.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::is_general>, bool> get(military::leader_tag i) const {
			 return bit_vector_test(m_15.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(military::leader_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, military_leader::is_general>> {
			 bit_vector_set(m_15.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::is_general>, tagged_array_view<bitfield_type, military::leader_tag>> get_row() {
			 return tagged_array_view<bitfield_type, military::leader_tag>(m_15.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, military_leader::is_general>, tagged_array_view<bitfield_type const, military::leader_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, military::leader_tag>(m_15.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }

		 military::leader_tag get_new() {
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
		 void release(military::leader_tag i) {
			 set<military_leader::first_name>(i, vector_backed_string<char16_t>());
			 set<military_leader::last_name>(i, vector_backed_string<char16_t>());
			 set<military_leader::creation_date>(i, date_tag());
			 set<military_leader::portrait>(i, graphics::texture_tag());
			 set<military_leader::personality>(i, military::leader_trait_tag());
			 set<military_leader::background>(i, military::leader_trait_tag());
			 set<military_leader::organisation>(i, float());
			 set<military_leader::morale>(i, float());
			 set<military_leader::attack>(i, float());
			 set<military_leader::defence>(i, float());
			 set<military_leader::reconnaissance>(i, float());
			 set<military_leader::speed>(i, float());
			 set<military_leader::experience>(i, float());
			 set<military_leader::reliability>(i, float());
			 set<military_leader::is_attached>(i, false);
			 set<military_leader::is_general>(i, false);
			 m_index.values[to_index(i)] = first_free;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == military::leader_tag(military::leader_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(military::leader_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::leader_tag(military::leader_tag::value_base_t(i))) f(military::leader_tag(military::leader_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == military::leader_tag(military::leader_tag::value_base_t(i))) f(military::leader_tag(military::leader_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::leader_tag(military::leader_tag::value_base_t(i))) { if(f(military::leader_tag(military::leader_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<military_leader::container, military_leader::first_name> {
	static constexpr bool value = true;
	 using type = vector_backed_string<char16_t>&;
	 using const_type = vector_backed_string<char16_t> const&;
	 using row = tagged_array_view<vector_backed_string<char16_t>, military::leader_tag>;
	 using const_row = tagged_array_view<vector_backed_string<char16_t> const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::last_name> {
	static constexpr bool value = true;
	 using type = vector_backed_string<char16_t>&;
	 using const_type = vector_backed_string<char16_t> const&;
	 using row = tagged_array_view<vector_backed_string<char16_t>, military::leader_tag>;
	 using const_row = tagged_array_view<vector_backed_string<char16_t> const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::creation_date> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, military::leader_tag>;
	 using const_row = tagged_array_view<date_tag const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::portrait> {
	static constexpr bool value = true;
	 using type = graphics::texture_tag&;
	 using const_type = graphics::texture_tag const&;
	 using row = tagged_array_view<graphics::texture_tag, military::leader_tag>;
	 using const_row = tagged_array_view<graphics::texture_tag const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::personality> {
	static constexpr bool value = true;
	 using type = military::leader_trait_tag&;
	 using const_type = military::leader_trait_tag const&;
	 using row = tagged_array_view<military::leader_trait_tag, military::leader_tag>;
	 using const_row = tagged_array_view<military::leader_trait_tag const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::background> {
	static constexpr bool value = true;
	 using type = military::leader_trait_tag&;
	 using const_type = military::leader_trait_tag const&;
	 using row = tagged_array_view<military::leader_trait_tag, military::leader_tag>;
	 using const_row = tagged_array_view<military::leader_trait_tag const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::organisation> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::morale> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::attack> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::defence> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::reconnaissance> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::speed> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::experience> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::reliability> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::leader_tag>;
	 using const_row = tagged_array_view<float const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::is_attached> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, military::leader_tag>;
	 using const_row = tagged_array_view<bitfield_type const, military::leader_tag>;
};
template<>
struct supports_index<military_leader::container, military_leader::is_general> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, military::leader_tag>;
	 using const_row = tagged_array_view<bitfield_type const, military::leader_tag>;
};

template<>
class serialization::serializer<military_leader::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, military_leader::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == military::leader_tag(military::leader_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = military::leader_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::creation_date, date_tag>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::creation_date, date_tag>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::organisation, float>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::organisation, float>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::morale, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::morale, float>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::morale, float>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::attack, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::attack, float>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::attack, float>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::defence, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::defence, float>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::defence, float>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reconnaissance, float>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reconnaissance, float>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::speed, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::speed, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::speed, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::experience, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::experience, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::experience, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reliability, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reliability, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		 serialization::serialize_array(output, obj.m_14.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_15.values, uint32_t(obj.size_used + 7) / 8ui32);
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, military_leader::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = military::leader_tag();
		for(int32_t i = military_leader::container_size - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = military::leader_tag(military::leader_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = military::leader_tag(military::leader_tag::value_base_t(i));
			}
		}
		if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::creation_date, date_tag>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::creation_date, date_tag>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::organisation, float>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::organisation, float>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::morale, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::morale, float>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::morale, float>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::attack, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::attack, float>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::attack, float>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::defence, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::defence, float>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::defence, float>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reconnaissance, float>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reconnaissance, float>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::speed, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::speed, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::speed, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::experience, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::experience, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::experience, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reliability, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<military_leader::reliability, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		 serialization::deserialize_array(input, obj.m_14.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_15.values, uint32_t(obj.size_used + 7) / 8ui32);
	 }
	 template<typename ... CONTEXT>
	 static size_t size(military_leader::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(military::leader_tag) * obj.size_used+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::size(obj.m_0.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_0.values, obj.m_0.values + max, 0ui64, std::plus<>(), [&](vector_backed_string<char16_t> const& m) {
					 return serialization::tagged_serializer<military_leader::first_name, vector_backed_string<char16_t>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::size(obj.m_1.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_1.values, obj.m_1.values + max, 0ui64, std::plus<>(), [&](vector_backed_string<char16_t> const& m) {
					 return serialization::tagged_serializer<military_leader::last_name, vector_backed_string<char16_t>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::creation_date, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::creation_date, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::creation_date, date_tag>::size(obj.m_2.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_2.values, obj.m_2.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<military_leader::creation_date, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](graphics::texture_tag const& m) {
					 return serialization::tagged_serializer<military_leader::portrait, graphics::texture_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](military::leader_trait_tag const& m) {
					 return serialization::tagged_serializer<military_leader::personality, military::leader_trait_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](military::leader_trait_tag const& m) {
					 return serialization::tagged_serializer<military_leader::background, military::leader_trait_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::organisation, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::organisation, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::organisation, float>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::organisation, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::morale, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::morale, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::morale, float>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::morale, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::attack, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::attack, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::attack, float>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::attack, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::defence, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::defence, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::defence, float>::size(obj.m_9.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_9.values, obj.m_9.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::defence, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::reconnaissance, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::reconnaissance, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::reconnaissance, float>::size(obj.m_10.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_10.values, obj.m_10.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::reconnaissance, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::speed, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::speed, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::speed, float>::size(obj.m_11.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_11.values, obj.m_11.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::speed, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::experience, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::experience, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::experience, float>::size(obj.m_12.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_12.values, obj.m_12.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::experience, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<military_leader::reliability, float>::size());
			 } else if constexpr(serialization::tagged_serializer<military_leader::reliability, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<military_leader::reliability, float>::size(obj.m_13.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_13.values, obj.m_13.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<military_leader::reliability, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32));
	 }
};
#pragma warning( pop )

