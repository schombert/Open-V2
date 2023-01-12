#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace pop {
	class container;
}

template<>
class serialization::serializer<pop::container>;

namespace pop {
	struct is_accepted;
	struct is_poor;
	struct is_middle;
	struct type;
	struct religion;
	struct culture;
	struct location;
	struct size;
	struct size_change_from_combat;
	struct size_change_from_growth;
	struct size_change_from_type_change_away;
	struct size_change_from_assimilation_away;
	struct size_change_from_local_migration;
	struct size_change_from_emigration;
	struct political_interest;
	struct social_interest;
	struct money;
	struct needs_satisfaction;
	struct literacy;
	struct militancy;
	struct consciousness;

constexpr int32_t max_count = pop::container_size;

	class alignas(64) container {
		 int32_t size_used = 0;
		 population::pop_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(population::pop_tag) + 63ui32) & ~63ui32]; 
			 population::pop_tag values[(sizeof(population::pop_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(population::pop_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(population::pop_tag)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(population::pop_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(population::pop_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(population::pop_tag)) - 1ui32) : uint32_t(pop::container_size)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(pop::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_0() { std::fill_n(values - 1, 1 + ((uint32_t(pop::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(pop::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_1() { std::fill_n(values - 1, 1 + ((uint32_t(pop::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(pop::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_2() { std::fill_n(values - 1, 1 + ((uint32_t(pop::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(population::pop_type_tag) + 63ui32) & ~63ui32]; 
			 population::pop_type_tag values[(sizeof(population::pop_type_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(population::pop_type_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(population::pop_type_tag)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(population::pop_type_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(population::pop_type_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(population::pop_type_tag)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(cultures::religion_tag) + 63ui32) & ~63ui32]; 
			 cultures::religion_tag values[(sizeof(cultures::religion_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::religion_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(cultures::religion_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::religion_tag)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(cultures::culture_tag) + 63ui32) & ~63ui32]; 
			 cultures::culture_tag values[(sizeof(cultures::culture_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(cultures::culture_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(cultures::culture_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(cultures::culture_tag)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(provinces::province_tag) + 63ui32) & ~63ui32]; 
			 provinces::province_tag values[(sizeof(provinces::province_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(provinces::province_tag) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_8;
		 struct alignas(64) dtype_9 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_9() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_9;
		 struct alignas(64) dtype_10 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_10() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_10;
		 struct alignas(64) dtype_11 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_11() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_11;
		 struct alignas(64) dtype_12 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_12() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_12;
		 struct alignas(64) dtype_13 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_13() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_13;
		 struct alignas(64) dtype_14 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_14() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_14;
		 struct alignas(64) dtype_15 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_15() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_15;
		 struct alignas(64) dtype_16 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_16() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_16;
		 struct alignas(64) dtype_17 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_17() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_17;
		 struct alignas(64) dtype_18 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_18() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_18;
		 struct alignas(64) dtype_19 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_19() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_19;
		 struct alignas(64) dtype_20 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size))]; 
			 dtype_20() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(pop::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(pop::container_size)) + 1); }
		 } m_20;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = pop::container_size - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = population::pop_tag(population::pop_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_accepted>, bool> get(population::pop_tag i) const {
			 return bit_vector_test(m_0.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(population::pop_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, pop::is_accepted>> {
			 bit_vector_set(m_0.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_accepted>, tagged_array_view<bitfield_type, population::pop_tag>> get_row() {
			 return tagged_array_view<bitfield_type, population::pop_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_accepted>, tagged_array_view<bitfield_type const, population::pop_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, population::pop_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_poor>, bool> get(population::pop_tag i) const {
			 return bit_vector_test(m_1.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(population::pop_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, pop::is_poor>> {
			 bit_vector_set(m_1.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_poor>, tagged_array_view<bitfield_type, population::pop_tag>> get_row() {
			 return tagged_array_view<bitfield_type, population::pop_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_poor>, tagged_array_view<bitfield_type const, population::pop_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, population::pop_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_middle>, bool> get(population::pop_tag i) const {
			 return bit_vector_test(m_2.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(population::pop_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, pop::is_middle>> {
			 bit_vector_set(m_2.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_middle>, tagged_array_view<bitfield_type, population::pop_tag>> get_row() {
			 return tagged_array_view<bitfield_type, population::pop_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::is_middle>, tagged_array_view<bitfield_type const, population::pop_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, population::pop_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::type>, population::pop_type_tag&> get(population::pop_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::type>, population::pop_type_tag const&> get(population::pop_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::type>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::type>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::type>, tagged_array_view<population::pop_type_tag, population::pop_tag>> get_row() {
			 return tagged_array_view<population::pop_type_tag, population::pop_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::type>, tagged_array_view<population::pop_type_tag const, population::pop_tag>> get_row() const {
			 return tagged_array_view<population::pop_type_tag const, population::pop_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::religion>, cultures::religion_tag&> get(population::pop_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::religion>, cultures::religion_tag const&> get(population::pop_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::religion>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::religion>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::religion>, tagged_array_view<cultures::religion_tag, population::pop_tag>> get_row() {
			 return tagged_array_view<cultures::religion_tag, population::pop_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::religion>, tagged_array_view<cultures::religion_tag const, population::pop_tag>> get_row() const {
			 return tagged_array_view<cultures::religion_tag const, population::pop_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::culture>, cultures::culture_tag&> get(population::pop_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::culture>, cultures::culture_tag const&> get(population::pop_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::culture>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::culture>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::culture>, tagged_array_view<cultures::culture_tag, population::pop_tag>> get_row() {
			 return tagged_array_view<cultures::culture_tag, population::pop_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::culture>, tagged_array_view<cultures::culture_tag const, population::pop_tag>> get_row() const {
			 return tagged_array_view<cultures::culture_tag const, population::pop_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::location>, provinces::province_tag&> get(population::pop_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::location>, provinces::province_tag const&> get(population::pop_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::location>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::location>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::location>, tagged_array_view<provinces::province_tag, population::pop_tag>> get_row() {
			 return tagged_array_view<provinces::province_tag, population::pop_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::location>, tagged_array_view<provinces::province_tag const, population::pop_tag>> get_row() const {
			 return tagged_array_view<provinces::province_tag const, population::pop_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size>, float&> get(population::pop_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size>, float const&> get(population::pop_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_combat>, float&> get(population::pop_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_combat>, float const&> get(population::pop_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_combat>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_combat>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_combat>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_combat>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_growth>, float&> get(population::pop_tag i) {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_growth>, float const&> get(population::pop_tag i) const {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_growth>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_growth>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_growth>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_growth>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_type_change_away>, float&> get(population::pop_tag i) {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_type_change_away>, float const&> get(population::pop_tag i) const {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_type_change_away>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_type_change_away>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_type_change_away>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_type_change_away>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_assimilation_away>, float&> get(population::pop_tag i) {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_assimilation_away>, float const&> get(population::pop_tag i) const {
			 return m_11.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_assimilation_away>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_assimilation_away>> {
			 m_11.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_assimilation_away>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_assimilation_away>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_11.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_local_migration>, float&> get(population::pop_tag i) {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_local_migration>, float const&> get(population::pop_tag i) const {
			 return m_12.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_local_migration>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_local_migration>> {
			 m_12.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_local_migration>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_local_migration>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_12.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_emigration>, float&> get(population::pop_tag i) {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_emigration>, float const&> get(population::pop_tag i) const {
			 return m_13.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_emigration>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::size_change_from_emigration>> {
			 m_13.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_emigration>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::size_change_from_emigration>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_13.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::political_interest>, float&> get(population::pop_tag i) {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::political_interest>, float const&> get(population::pop_tag i) const {
			 return m_14.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::political_interest>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::political_interest>> {
			 m_14.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::political_interest>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::political_interest>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_14.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::social_interest>, float&> get(population::pop_tag i) {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::social_interest>, float const&> get(population::pop_tag i) const {
			 return m_15.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::social_interest>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::social_interest>> {
			 m_15.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::social_interest>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::social_interest>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_15.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::money>, float&> get(population::pop_tag i) {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::money>, float const&> get(population::pop_tag i) const {
			 return m_16.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::money>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::money>> {
			 m_16.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::money>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::money>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_16.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::needs_satisfaction>, float&> get(population::pop_tag i) {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::needs_satisfaction>, float const&> get(population::pop_tag i) const {
			 return m_17.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::needs_satisfaction>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::needs_satisfaction>> {
			 m_17.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::needs_satisfaction>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::needs_satisfaction>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_17.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::literacy>, float&> get(population::pop_tag i) {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::literacy>, float const&> get(population::pop_tag i) const {
			 return m_18.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::literacy>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::literacy>> {
			 m_18.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::literacy>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::literacy>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_18.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::militancy>, float&> get(population::pop_tag i) {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::militancy>, float const&> get(population::pop_tag i) const {
			 return m_19.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::militancy>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::militancy>> {
			 m_19.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::militancy>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::militancy>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_19.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::consciousness>, float&> get(population::pop_tag i) {
			 return m_20.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::consciousness>, float const&> get(population::pop_tag i) const {
			 return m_20.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::consciousness>> {
			 m_20.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(population::pop_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, pop::consciousness>> {
			 m_20.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::consciousness>, tagged_array_view<float, population::pop_tag>> get_row() {
			 return tagged_array_view<float, population::pop_tag>(m_20.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, pop::consciousness>, tagged_array_view<float const, population::pop_tag>> get_row() const {
			 return tagged_array_view<float const, population::pop_tag>(m_20.values, ve::to_vector_size(uint32_t(size_used)));
		 }

		 population::pop_tag get_new() {
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
		 void release(population::pop_tag i) {
			 set<pop::is_accepted>(i, false);
			 set<pop::is_poor>(i, false);
			 set<pop::is_middle>(i, false);
			 set<pop::type>(i, population::pop_type_tag());
			 set<pop::religion>(i, cultures::religion_tag());
			 set<pop::culture>(i, cultures::culture_tag());
			 set<pop::location>(i, provinces::province_tag());
			 set<pop::size>(i, float());
			 set<pop::size_change_from_combat>(i, float());
			 set<pop::size_change_from_growth>(i, float());
			 set<pop::size_change_from_type_change_away>(i, float());
			 set<pop::size_change_from_assimilation_away>(i, float());
			 set<pop::size_change_from_local_migration>(i, float());
			 set<pop::size_change_from_emigration>(i, float());
			 set<pop::political_interest>(i, float());
			 set<pop::social_interest>(i, float());
			 set<pop::money>(i, float());
			 set<pop::needs_satisfaction>(i, float());
			 set<pop::literacy>(i, float());
			 set<pop::militancy>(i, float());
			 set<pop::consciousness>(i, float());
			 m_index.values[to_index(i)] = first_free;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == population::pop_tag(population::pop_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(population::pop_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == population::pop_tag(population::pop_tag::value_base_t(i))) f(population::pop_tag(population::pop_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == population::pop_tag(population::pop_tag::value_base_t(i))) f(population::pop_tag(population::pop_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == population::pop_tag(population::pop_tag::value_base_t(i))) { if(f(population::pop_tag(population::pop_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<pop::container, pop::is_accepted> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, population::pop_tag>;
	 using const_row = tagged_array_view<bitfield_type const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::is_poor> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, population::pop_tag>;
	 using const_row = tagged_array_view<bitfield_type const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::is_middle> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, population::pop_tag>;
	 using const_row = tagged_array_view<bitfield_type const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::type> {
	static constexpr bool value = true;
	 using type = population::pop_type_tag&;
	 using const_type = population::pop_type_tag const&;
	 using row = tagged_array_view<population::pop_type_tag, population::pop_tag>;
	 using const_row = tagged_array_view<population::pop_type_tag const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::religion> {
	static constexpr bool value = true;
	 using type = cultures::religion_tag&;
	 using const_type = cultures::religion_tag const&;
	 using row = tagged_array_view<cultures::religion_tag, population::pop_tag>;
	 using const_row = tagged_array_view<cultures::religion_tag const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::culture> {
	static constexpr bool value = true;
	 using type = cultures::culture_tag&;
	 using const_type = cultures::culture_tag const&;
	 using row = tagged_array_view<cultures::culture_tag, population::pop_tag>;
	 using const_row = tagged_array_view<cultures::culture_tag const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::location> {
	static constexpr bool value = true;
	 using type = provinces::province_tag&;
	 using const_type = provinces::province_tag const&;
	 using row = tagged_array_view<provinces::province_tag, population::pop_tag>;
	 using const_row = tagged_array_view<provinces::province_tag const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::size> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::size_change_from_combat> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::size_change_from_growth> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::size_change_from_type_change_away> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::size_change_from_assimilation_away> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::size_change_from_local_migration> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::size_change_from_emigration> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::political_interest> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::social_interest> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::money> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::needs_satisfaction> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::literacy> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::militancy> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};
template<>
struct supports_index<pop::container, pop::consciousness> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, population::pop_tag>;
	 using const_row = tagged_array_view<float const, population::pop_tag>;
};

template<>
class serialization::serializer<pop::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, pop::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == population::pop_tag(population::pop_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = population::pop_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		 serialization::serialize_array(output, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::type, population::pop_type_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::type, population::pop_type_tag>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::religion, cultures::religion_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::religion, cultures::religion_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::culture, cultures::culture_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::culture, cultures::culture_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::location, provinces::province_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::location, provinces::province_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size, float>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size, float>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_combat, float>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_combat, float>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_growth, float>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_growth, float>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::serialize_object(output, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_local_migration, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_local_migration, float>::serialize_object(output, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_emigration, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_emigration, float>::serialize_object(output, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::political_interest, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::political_interest, float>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::political_interest, float>::serialize_object(output, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::social_interest, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::social_interest, float>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::social_interest, float>::serialize_object(output, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::money, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::money, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::money, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::money, float>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::money, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::money, float>::serialize_object(output, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::needs_satisfaction, float>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::needs_satisfaction, float>::serialize_object(output, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::literacy, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::literacy, float>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::literacy, float>::serialize_object(output, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::militancy, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::militancy, float>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::militancy, float>::serialize_object(output, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::consciousness, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::consciousness, float>::serialize_object(output, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::consciousness, float>::serialize_object(output, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, pop::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = population::pop_tag();
		for(int32_t i = pop::container_size - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = population::pop_tag(population::pop_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = population::pop_tag(population::pop_tag::value_base_t(i));
			}
		}
		 serialization::deserialize_array(input, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::type, population::pop_type_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::type, population::pop_type_tag>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::religion, cultures::religion_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::religion, cultures::religion_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::culture, cultures::culture_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::culture, cultures::culture_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::location, provinces::province_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::location, provinces::province_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size, float>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size, float>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_combat, float>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_combat, float>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_growth, float>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_growth, float>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_11.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::deserialize_object(input, obj.m_11.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_local_migration, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_12.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_local_migration, float>::deserialize_object(input, obj.m_12.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_emigration, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_13.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::size_change_from_emigration, float>::deserialize_object(input, obj.m_13.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::political_interest, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::political_interest, float>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_14.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::political_interest, float>::deserialize_object(input, obj.m_14.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::social_interest, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::social_interest, float>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_15.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::social_interest, float>::deserialize_object(input, obj.m_15.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::money, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::money, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::money, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::money, float>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::money, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_16.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::money, float>::deserialize_object(input, obj.m_16.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::needs_satisfaction, float>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_17.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::needs_satisfaction, float>::deserialize_object(input, obj.m_17.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::literacy, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::literacy, float>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_18.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::literacy, float>::deserialize_object(input, obj.m_18.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::militancy, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::militancy, float>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_19.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::militancy, float>::deserialize_object(input, obj.m_19.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<pop::consciousness, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::consciousness, float>::deserialize_object(input, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_20.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<pop::consciousness, float>::deserialize_object(input, obj.m_20.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static size_t size(pop::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(population::pop_tag) * obj.size_used+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::type, population::pop_type_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::type, population::pop_type_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::type, population::pop_type_tag>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](population::pop_type_tag const& m) {
					 return serialization::tagged_serializer<pop::type, population::pop_type_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::religion, cultures::religion_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::religion, cultures::religion_tag>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](cultures::religion_tag const& m) {
					 return serialization::tagged_serializer<pop::religion, cultures::religion_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::culture, cultures::culture_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::culture, cultures::culture_tag>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](cultures::culture_tag const& m) {
					 return serialization::tagged_serializer<pop::culture, cultures::culture_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::location, provinces::province_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::location, provinces::province_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::location, provinces::province_tag>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](provinces::province_tag const& m) {
					 return serialization::tagged_serializer<pop::location, provinces::province_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::size, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::size, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::size, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::size, float>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::size, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::size_change_from_combat, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_combat, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::size_change_from_combat, float>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::size_change_from_combat, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::size_change_from_growth, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_growth, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::size_change_from_growth, float>::size(obj.m_9.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_9.values, obj.m_9.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::size_change_from_growth, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::size(obj.m_10.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_10.values, obj.m_10.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::size_change_from_type_change_away, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::size(obj.m_11.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_11.values, obj.m_11.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::size_change_from_assimilation_away, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::size_change_from_local_migration, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::size_change_from_local_migration, float>::size(obj.m_12.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_12.values, obj.m_12.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::size_change_from_local_migration, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::size_change_from_emigration, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::size_change_from_emigration, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::size_change_from_emigration, float>::size(obj.m_13.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_13.values, obj.m_13.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::size_change_from_emigration, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::political_interest, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::political_interest, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::political_interest, float>::size(obj.m_14.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_14.values, obj.m_14.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::political_interest, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::social_interest, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::social_interest, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::social_interest, float>::size(obj.m_15.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_15.values, obj.m_15.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::social_interest, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::money, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::money, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::money, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::money, float>::size(obj.m_16.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_16.values, obj.m_16.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::money, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::needs_satisfaction, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::needs_satisfaction, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::needs_satisfaction, float>::size(obj.m_17.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_17.values, obj.m_17.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::needs_satisfaction, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::literacy, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::literacy, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::literacy, float>::size(obj.m_18.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_18.values, obj.m_18.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::literacy, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::militancy, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::militancy, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::militancy, float>::size(obj.m_19.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_19.values, obj.m_19.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::militancy, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<pop::consciousness, float>::size());
			 } else if constexpr(serialization::tagged_serializer<pop::consciousness, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<pop::consciousness, float>::size(obj.m_20.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_20.values, obj.m_20.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<pop::consciousness, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
);
	 }
};
#pragma warning( pop )

