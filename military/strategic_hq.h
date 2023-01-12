#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace strategic_hq {
	class container;
}

template<>
class serialization::serializer<strategic_hq::container>;

namespace strategic_hq {
	struct leader;
	struct location;
	struct province_set;
	struct army_set;
	struct injured_soldiers;
	struct pow_soldiers;
	struct reserve_soldiers;
	struct allocated_soldiers;
	struct total_soldier_pops;
	struct total_non_soldier_pops;
	struct mobilization_level;

constexpr int32_t max_count = strategic_hq::container_size;

	class alignas(64) container {
		 int32_t size_used = 0;
		 military::strategic_hq_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(military::strategic_hq_tag) + 63ui32) & ~63ui32]; 
			 military::strategic_hq_tag values[(sizeof(military::strategic_hq_tag) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(military::strategic_hq_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::strategic_hq_tag)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::strategic_hq_tag) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(military::strategic_hq_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::strategic_hq_tag)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 uint8_t padding[(sizeof(military::leader_tag) + 63ui32) & ~63ui32]; 
			 military::leader_tag values[(sizeof(military::leader_tag) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(military::leader_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_tag)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_0() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::leader_tag) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(military::leader_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_tag)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 uint8_t padding[(sizeof(provinces::province_tag) + 63ui32) & ~63ui32]; 
			 provinces::province_tag values[(sizeof(provinces::province_tag) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_1() { std::uninitialized_value_construct_n(values - 1, (sizeof(provinces::province_tag) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 uint8_t padding[(sizeof(set_tag<provinces::province_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<provinces::province_tag> values[(sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_2() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<provinces::province_tag>) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<provinces::province_tag>)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(set_tag<military::army_tag>) + 63ui32) & ~63ui32]; 
			 set_tag<military::army_tag> values[(sizeof(set_tag<military::army_tag>) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(set_tag<military::army_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::army_tag>)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(set_tag<military::army_tag>) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(set_tag<military::army_tag>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(set_tag<military::army_tag>)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_8;
		 struct alignas(64) dtype_9 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_9() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_9;
		 struct alignas(64) dtype_10 { 
			 uint8_t padding[(sizeof(int8_t) + 63ui32) & ~63ui32]; 
			 int8_t values[(sizeof(int8_t) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(int8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int8_t)) - 1ui32) : uint32_t(strategic_hq::container_size))]; 
			 dtype_10() { std::uninitialized_value_construct_n(values - 1, (sizeof(int8_t) <= 64 ? (uint32_t(strategic_hq::container_size) + (64ui32 / uint32_t(sizeof(int8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int8_t)) - 1ui32) : uint32_t(strategic_hq::container_size)) + 1); }
		 } m_10;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = strategic_hq::container_size - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::leader>, military::leader_tag&> get(military::strategic_hq_tag i) {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::leader>, military::leader_tag const&> get(military::strategic_hq_tag i) const {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::leader>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::leader>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::leader>, tagged_array_view<military::leader_tag, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<military::leader_tag, military::strategic_hq_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::leader>, tagged_array_view<military::leader_tag const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<military::leader_tag const, military::strategic_hq_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::location>, provinces::province_tag&> get(military::strategic_hq_tag i) {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::location>, provinces::province_tag const&> get(military::strategic_hq_tag i) const {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::location>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::location>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::location>, tagged_array_view<provinces::province_tag, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<provinces::province_tag, military::strategic_hq_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::location>, tagged_array_view<provinces::province_tag const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<provinces::province_tag const, military::strategic_hq_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::province_set>, set_tag<provinces::province_tag>&> get(military::strategic_hq_tag i) {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::province_set>, set_tag<provinces::province_tag> const&> get(military::strategic_hq_tag i) const {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::province_set>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::province_set>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::province_set>, tagged_array_view<set_tag<provinces::province_tag>, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<set_tag<provinces::province_tag>, military::strategic_hq_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::province_set>, tagged_array_view<set_tag<provinces::province_tag> const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<set_tag<provinces::province_tag> const, military::strategic_hq_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::army_set>, set_tag<military::army_tag>&> get(military::strategic_hq_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::army_set>, set_tag<military::army_tag> const&> get(military::strategic_hq_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::army_set>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::army_set>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::army_set>, tagged_array_view<set_tag<military::army_tag>, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<set_tag<military::army_tag>, military::strategic_hq_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::army_set>, tagged_array_view<set_tag<military::army_tag> const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<set_tag<military::army_tag> const, military::strategic_hq_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::injured_soldiers>, float&> get(military::strategic_hq_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::injured_soldiers>, float const&> get(military::strategic_hq_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::injured_soldiers>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::injured_soldiers>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::injured_soldiers>, tagged_array_view<float, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<float, military::strategic_hq_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::injured_soldiers>, tagged_array_view<float const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<float const, military::strategic_hq_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::pow_soldiers>, float&> get(military::strategic_hq_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::pow_soldiers>, float const&> get(military::strategic_hq_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::pow_soldiers>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::pow_soldiers>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::pow_soldiers>, tagged_array_view<float, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<float, military::strategic_hq_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::pow_soldiers>, tagged_array_view<float const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<float const, military::strategic_hq_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::reserve_soldiers>, float&> get(military::strategic_hq_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::reserve_soldiers>, float const&> get(military::strategic_hq_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::reserve_soldiers>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::reserve_soldiers>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::reserve_soldiers>, tagged_array_view<float, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<float, military::strategic_hq_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::reserve_soldiers>, tagged_array_view<float const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<float const, military::strategic_hq_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::allocated_soldiers>, float&> get(military::strategic_hq_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::allocated_soldiers>, float const&> get(military::strategic_hq_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::allocated_soldiers>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::allocated_soldiers>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::allocated_soldiers>, tagged_array_view<float, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<float, military::strategic_hq_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::allocated_soldiers>, tagged_array_view<float const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<float const, military::strategic_hq_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_soldier_pops>, float&> get(military::strategic_hq_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_soldier_pops>, float const&> get(military::strategic_hq_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::total_soldier_pops>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::total_soldier_pops>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_soldier_pops>, tagged_array_view<float, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<float, military::strategic_hq_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_soldier_pops>, tagged_array_view<float const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<float const, military::strategic_hq_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_non_soldier_pops>, float&> get(military::strategic_hq_tag i) {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_non_soldier_pops>, float const&> get(military::strategic_hq_tag i) const {
			 return m_9.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::total_non_soldier_pops>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::total_non_soldier_pops>> {
			 m_9.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_non_soldier_pops>, tagged_array_view<float, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<float, military::strategic_hq_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::total_non_soldier_pops>, tagged_array_view<float const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<float const, military::strategic_hq_tag>(m_9.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::mobilization_level>, int8_t&> get(military::strategic_hq_tag i) {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::mobilization_level>, int8_t const&> get(military::strategic_hq_tag i) const {
			 return m_10.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::mobilization_level>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::strategic_hq_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, strategic_hq::mobilization_level>> {
			 m_10.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::mobilization_level>, tagged_array_view<int8_t, military::strategic_hq_tag>> get_row() {
			 return tagged_array_view<int8_t, military::strategic_hq_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, strategic_hq::mobilization_level>, tagged_array_view<int8_t const, military::strategic_hq_tag>> get_row() const {
			 return tagged_array_view<int8_t const, military::strategic_hq_tag>(m_10.values, ve::to_vector_size(uint32_t(size_used)));
		 }

		 military::strategic_hq_tag get_new() {
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
		 void release(military::strategic_hq_tag i) {
			 set<strategic_hq::leader>(i, military::leader_tag());
			 set<strategic_hq::location>(i, provinces::province_tag());
			 set<strategic_hq::province_set>(i, set_tag<provinces::province_tag>());
			 set<strategic_hq::army_set>(i, set_tag<military::army_tag>());
			 set<strategic_hq::injured_soldiers>(i, float());
			 set<strategic_hq::pow_soldiers>(i, float());
			 set<strategic_hq::reserve_soldiers>(i, float());
			 set<strategic_hq::allocated_soldiers>(i, float());
			 set<strategic_hq::total_soldier_pops>(i, float());
			 set<strategic_hq::total_non_soldier_pops>(i, float());
			 set<strategic_hq::mobilization_level>(i, int8_t());
			 m_index.values[to_index(i)] = first_free;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(military::strategic_hq_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i))) f(military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i))) f(military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i))) { if(f(military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<strategic_hq::container, strategic_hq::leader> {
	static constexpr bool value = true;
	 using type = military::leader_tag&;
	 using const_type = military::leader_tag const&;
	 using row = tagged_array_view<military::leader_tag, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<military::leader_tag const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::location> {
	static constexpr bool value = true;
	 using type = provinces::province_tag&;
	 using const_type = provinces::province_tag const&;
	 using row = tagged_array_view<provinces::province_tag, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<provinces::province_tag const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::province_set> {
	static constexpr bool value = true;
	 using type = set_tag<provinces::province_tag>&;
	 using const_type = set_tag<provinces::province_tag> const&;
	 using row = tagged_array_view<set_tag<provinces::province_tag>, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<set_tag<provinces::province_tag> const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::army_set> {
	static constexpr bool value = true;
	 using type = set_tag<military::army_tag>&;
	 using const_type = set_tag<military::army_tag> const&;
	 using row = tagged_array_view<set_tag<military::army_tag>, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<set_tag<military::army_tag> const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::injured_soldiers> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<float const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::pow_soldiers> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<float const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::reserve_soldiers> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<float const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::allocated_soldiers> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<float const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::total_soldier_pops> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<float const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::total_non_soldier_pops> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<float const, military::strategic_hq_tag>;
};
template<>
struct supports_index<strategic_hq::container, strategic_hq::mobilization_level> {
	static constexpr bool value = true;
	 using type = int8_t&;
	 using const_type = int8_t const&;
	 using row = tagged_array_view<int8_t, military::strategic_hq_tag>;
	 using const_row = tagged_array_view<int8_t const, military::strategic_hq_tag>;
};

template<>
class serialization::serializer<strategic_hq::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, strategic_hq::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = military::strategic_hq_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::serialize_object(output, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::serialize_object(output, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, strategic_hq::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = military::strategic_hq_tag();
		for(int32_t i = strategic_hq::container_size - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = military::strategic_hq_tag(military::strategic_hq_tag::value_base_t(i));
			}
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_9.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::deserialize_object(input, obj.m_9.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_10.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::deserialize_object(input, obj.m_10.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static size_t size(strategic_hq::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(military::strategic_hq_tag) * obj.size_used+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::size(obj.m_0.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_0.values, obj.m_0.values + max, 0ui64, std::plus<>(), [&](military::leader_tag const& m) {
					 return serialization::tagged_serializer<strategic_hq::leader, military::leader_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::size(obj.m_1.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_1.values, obj.m_1.values + max, 0ui64, std::plus<>(), [&](provinces::province_tag const& m) {
					 return serialization::tagged_serializer<strategic_hq::location, provinces::province_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::size(obj.m_2.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_2.values, obj.m_2.values + max, 0ui64, std::plus<>(), [&](set_tag<provinces::province_tag> const& m) {
					 return serialization::tagged_serializer<strategic_hq::province_set, set_tag<provinces::province_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](set_tag<military::army_tag> const& m) {
					 return serialization::tagged_serializer<strategic_hq::army_set, set_tag<military::army_tag>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<strategic_hq::injured_soldiers, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<strategic_hq::pow_soldiers, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<strategic_hq::reserve_soldiers, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<strategic_hq::allocated_soldiers, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<strategic_hq::total_soldier_pops, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::size(obj.m_9.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_9.values, obj.m_9.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<strategic_hq::total_non_soldier_pops, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::size());
			 } else if constexpr(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::size(obj.m_10.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_10.values, obj.m_10.values + max, 0ui64, std::plus<>(), [&](int8_t const& m) {
					 return serialization::tagged_serializer<strategic_hq::mobilization_level, int8_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
);
	 }
};
#pragma warning( pop )

