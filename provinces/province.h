#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace province {
	class container;
}

template<>
class serialization::serializer<province::container>;

namespace province {
	struct is_sea;
	struct is_coastal;
	struct is_lake;
	struct area;
	struct centroid;
	struct centroid_2d;
	struct continent;
	struct climate;
	struct state_id;

	class alignas(64) container {
		 int32_t size_used = 0;


		 struct alignas(64) dtype_0 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_0() { std::fill_n(values - 1, 1 + ((uint32_t(province::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_1() { std::fill_n(values - 1, 1 + ((uint32_t(province::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 bitfield_type padding[64]; 
			 bitfield_type values[((uint32_t(province::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32]; 
			 dtype_2() { std::fill_n(values - 1, 1 + ((uint32_t(province::container_size + 7)) / 8ui32 + 63ui32) & ~63ui32, bitfield_type{ 0ui8 }); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(Eigen::Vector3f) + 63ui32) & ~63ui32]; 
			 Eigen::Vector3f values[(sizeof(Eigen::Vector3f) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(Eigen::Vector3f))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(Eigen::Vector3f)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(Eigen::Vector3f) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(Eigen::Vector3f))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(Eigen::Vector3f)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(Eigen::Vector2f) + 63ui32) & ~63ui32]; 
			 Eigen::Vector2f values[(sizeof(Eigen::Vector2f) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(Eigen::Vector2f))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(Eigen::Vector2f)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(Eigen::Vector2f) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(Eigen::Vector2f))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(Eigen::Vector2f)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_5;
		 struct alignas(64) dtype_6 { 
			 uint8_t padding[(sizeof(modifiers::provincial_modifier_tag) + 63ui32) & ~63ui32]; 
			 modifiers::provincial_modifier_tag values[(sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_6() { std::uninitialized_value_construct_n(values - 1, (sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_6;
		 struct alignas(64) dtype_7 { 
			 uint8_t padding[(sizeof(modifiers::provincial_modifier_tag) + 63ui32) & ~63ui32]; 
			 modifiers::provincial_modifier_tag values[(sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_7() { std::uninitialized_value_construct_n(values - 1, (sizeof(modifiers::provincial_modifier_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(modifiers::provincial_modifier_tag)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_7;
		 struct alignas(64) dtype_8 { 
			 uint8_t padding[(sizeof(provinces::state_tag) + 63ui32) & ~63ui32]; 
			 provinces::state_tag values[(sizeof(provinces::state_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(provinces::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::state_tag)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_8() { std::uninitialized_value_construct_n(values - 1, (sizeof(provinces::state_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(provinces::state_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::state_tag)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_8;

		 public:
		 friend class serialization::serializer<container>;
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_sea>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_0.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province::is_sea>> {
			 bit_vector_set(m_0.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_sea>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_sea>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_0.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_coastal>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_1.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province::is_coastal>> {
			 bit_vector_set(m_1.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_coastal>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_coastal>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_1.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_lake>, bool> get(provinces::province_tag i) const {
			 return bit_vector_test(m_2.values, to_index(i));
		 }
		 template<typename INDEX>
		 auto set(provinces::province_tag i, bool v) -> std::enable_if_t<std::is_same_v<INDEX, province::is_lake>> {
			 bit_vector_set(m_2.values, to_index(i), v);
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_lake>, tagged_array_view<bitfield_type, provinces::province_tag>> get_row() {
			 return tagged_array_view<bitfield_type, provinces::province_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::is_lake>, tagged_array_view<bitfield_type const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<bitfield_type const, provinces::province_tag>(m_2.values, int32_t(uint32_t(size_used + 7) / 8ui32));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::area>, float&> get(provinces::province_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::area>, float const&> get(provinces::province_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::area>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::area>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::area>, tagged_array_view<float, provinces::province_tag>> get_row() {
			 return tagged_array_view<float, provinces::province_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::area>, tagged_array_view<float const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<float const, provinces::province_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid>, Eigen::Vector3f&> get(provinces::province_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid>, Eigen::Vector3f const&> get(provinces::province_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::centroid>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::centroid>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid>, tagged_array_view<Eigen::Vector3f, provinces::province_tag>> get_row() {
			 return tagged_array_view<Eigen::Vector3f, provinces::province_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid>, tagged_array_view<Eigen::Vector3f const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<Eigen::Vector3f const, provinces::province_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid_2d>, Eigen::Vector2f&> get(provinces::province_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid_2d>, Eigen::Vector2f const&> get(provinces::province_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::centroid_2d>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::centroid_2d>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid_2d>, tagged_array_view<Eigen::Vector2f, provinces::province_tag>> get_row() {
			 return tagged_array_view<Eigen::Vector2f, provinces::province_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::centroid_2d>, tagged_array_view<Eigen::Vector2f const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<Eigen::Vector2f const, provinces::province_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::continent>, modifiers::provincial_modifier_tag&> get(provinces::province_tag i) {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::continent>, modifiers::provincial_modifier_tag const&> get(provinces::province_tag i) const {
			 return m_6.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::continent>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::continent>> {
			 m_6.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::continent>, tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::continent>, tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>(m_6.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::climate>, modifiers::provincial_modifier_tag&> get(provinces::province_tag i) {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::climate>, modifiers::provincial_modifier_tag const&> get(provinces::province_tag i) const {
			 return m_7.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::climate>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::climate>> {
			 m_7.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::climate>, tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::climate>, tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>(m_7.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::state_id>, provinces::state_tag&> get(provinces::province_tag i) {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::state_id>, provinces::state_tag const&> get(provinces::province_tag i) const {
			 return m_8.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::state_id>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(provinces::province_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, province::state_id>> {
			 m_8.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::state_id>, tagged_array_view<provinces::state_tag, provinces::province_tag>> get_row() {
			 return tagged_array_view<provinces::state_tag, provinces::province_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, province::state_id>, tagged_array_view<provinces::state_tag const, provinces::province_tag>> get_row() const {
			 return tagged_array_view<provinces::state_tag const, provinces::province_tag>(m_8.values, ve::to_vector_size(uint32_t(size_used)));
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
struct supports_index<province::container, province::is_sea> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::is_coastal> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::is_lake> {
	static constexpr bool value = true;
	 using type = bool;
	 using const_type = bool;
	 using row = tagged_array_view<bitfield_type, provinces::province_tag>;
	 using const_row = tagged_array_view<bitfield_type const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::area> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, provinces::province_tag>;
	 using const_row = tagged_array_view<float const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::centroid> {
	static constexpr bool value = true;
	 using type = Eigen::Vector3f&;
	 using const_type = Eigen::Vector3f const&;
	 using row = tagged_array_view<Eigen::Vector3f, provinces::province_tag>;
	 using const_row = tagged_array_view<Eigen::Vector3f const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::centroid_2d> {
	static constexpr bool value = true;
	 using type = Eigen::Vector2f&;
	 using const_type = Eigen::Vector2f const&;
	 using row = tagged_array_view<Eigen::Vector2f, provinces::province_tag>;
	 using const_row = tagged_array_view<Eigen::Vector2f const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::continent> {
	static constexpr bool value = true;
	 using type = modifiers::provincial_modifier_tag&;
	 using const_type = modifiers::provincial_modifier_tag const&;
	 using row = tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::climate> {
	static constexpr bool value = true;
	 using type = modifiers::provincial_modifier_tag&;
	 using const_type = modifiers::provincial_modifier_tag const&;
	 using row = tagged_array_view<modifiers::provincial_modifier_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<modifiers::provincial_modifier_tag const, provinces::province_tag>;
};
template<>
struct supports_index<province::container, province::state_id> {
	static constexpr bool value = true;
	 using type = provinces::state_tag&;
	 using const_type = provinces::state_tag const&;
	 using row = tagged_array_view<provinces::state_tag, provinces::province_tag>;
	 using const_row = tagged_array_view<provinces::state_tag const, provinces::province_tag>;
};

template<>
class serialization::serializer<province::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, province::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 serialization::serialize_array(output, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::serialize_array(output, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<province::area, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::area, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::area, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::area, float>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::area, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::area, float>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::serialize_object(output, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::state_id, provinces::state_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::state_id, provinces::state_tag>::serialize_object(output, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, province::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_0.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_1.values, uint32_t(obj.size_used + 7) / 8ui32);
		 serialization::deserialize_array(input, obj.m_2.values, uint32_t(obj.size_used + 7) / 8ui32);
		if constexpr(serialization::tagged_serializer<province::area, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::area, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::area, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::area, float>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::area, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::area, float>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_6.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_6.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_7.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::deserialize_object(input, obj.m_7.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::state_id, provinces::state_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_8.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<province::state_id, provinces::state_tag>::deserialize_object(input, obj.m_8.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static size_t size(province::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ sizeof(bitfield_type) * (uint32_t(obj.size_used + 7) / 8ui32)+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province::area, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province::area, float>::size());
			 } else if constexpr(serialization::tagged_serializer<province::area, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province::area, float>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<province::area, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::size());
			 } else if constexpr(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](Eigen::Vector3f const& m) {
					 return serialization::tagged_serializer<province::centroid, Eigen::Vector3f>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::size());
			 } else if constexpr(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](Eigen::Vector2f const& m) {
					 return serialization::tagged_serializer<province::centroid_2d, Eigen::Vector2f>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::size(obj.m_6.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_6.values, obj.m_6.values + max, 0ui64, std::plus<>(), [&](modifiers::provincial_modifier_tag const& m) {
					 return serialization::tagged_serializer<province::continent, modifiers::provincial_modifier_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::size(obj.m_7.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_7.values, obj.m_7.values + max, 0ui64, std::plus<>(), [&](modifiers::provincial_modifier_tag const& m) {
					 return serialization::tagged_serializer<province::climate, modifiers::provincial_modifier_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<province::state_id, provinces::state_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<province::state_id, provinces::state_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<province::state_id, provinces::state_tag>::size(obj.m_8.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_8.values, obj.m_8.values + max, 0ui64, std::plus<>(), [&](provinces::state_tag const& m) {
					 return serialization::tagged_serializer<province::state_id, provinces::state_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
);
	 }
};
#pragma warning( pop )

