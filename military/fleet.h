#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace fleet {
	class container;
}

template<>
class serialization::serializer<fleet::container>;

namespace fleet {
	struct leader;
	struct location;
	struct supply;
	struct readiness;
	struct size;
	struct arrival_time;

constexpr int32_t max_count = fleet::container_size;

	class alignas(64) container {
		 int32_t size_used = 0;
		 military::fleet_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(military::fleet_tag) + 63ui32) & ~63ui32]; 
			 military::fleet_tag values[(sizeof(military::fleet_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(military::fleet_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::fleet_tag)) - 1ui32) : uint32_t(fleet::container_size))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::fleet_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(military::fleet_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::fleet_tag)) - 1ui32) : uint32_t(fleet::container_size)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 uint8_t padding[(sizeof(military::leader_tag) + 63ui32) & ~63ui32]; 
			 military::leader_tag values[(sizeof(military::leader_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(military::leader_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_tag)) - 1ui32) : uint32_t(fleet::container_size))]; 
			 dtype_0() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::leader_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(military::leader_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::leader_tag)) - 1ui32) : uint32_t(fleet::container_size)) + 1); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 uint8_t padding[(sizeof(provinces::province_tag) + 63ui32) & ~63ui32]; 
			 provinces::province_tag values[(sizeof(provinces::province_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(fleet::container_size))]; 
			 dtype_1() { std::uninitialized_value_construct_n(values - 1, (sizeof(provinces::province_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(provinces::province_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(provinces::province_tag)) - 1ui32) : uint32_t(fleet::container_size)) + 1); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(fleet::container_size))]; 
			 dtype_2() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(fleet::container_size)) + 1); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(fleet::container_size))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(fleet::container_size)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(fleet::container_size))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(fleet::container_size)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(date_tag) + 63ui32) & ~63ui32]; 
			 date_tag values[(sizeof(date_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(fleet::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(date_tag) <= 64 ? (uint32_t(fleet::container_size) + (64ui32 / uint32_t(sizeof(date_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(date_tag)) - 1ui32) : uint32_t(fleet::container_size)) + 1); }
		 } m_5;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = fleet::container_size - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = military::fleet_tag(military::fleet_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::leader>, military::leader_tag&> get(military::fleet_tag i) {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::leader>, military::leader_tag const&> get(military::fleet_tag i) const {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::leader>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::leader>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::leader>, tagged_array_view<military::leader_tag, military::fleet_tag>> get_row() {
			 return tagged_array_view<military::leader_tag, military::fleet_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::leader>, tagged_array_view<military::leader_tag const, military::fleet_tag>> get_row() const {
			 return tagged_array_view<military::leader_tag const, military::fleet_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::location>, provinces::province_tag&> get(military::fleet_tag i) {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::location>, provinces::province_tag const&> get(military::fleet_tag i) const {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::location>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::location>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::location>, tagged_array_view<provinces::province_tag, military::fleet_tag>> get_row() {
			 return tagged_array_view<provinces::province_tag, military::fleet_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::location>, tagged_array_view<provinces::province_tag const, military::fleet_tag>> get_row() const {
			 return tagged_array_view<provinces::province_tag const, military::fleet_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::supply>, float&> get(military::fleet_tag i) {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::supply>, float const&> get(military::fleet_tag i) const {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::supply>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::supply>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::supply>, tagged_array_view<float, military::fleet_tag>> get_row() {
			 return tagged_array_view<float, military::fleet_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::supply>, tagged_array_view<float const, military::fleet_tag>> get_row() const {
			 return tagged_array_view<float const, military::fleet_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::readiness>, float&> get(military::fleet_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::readiness>, float const&> get(military::fleet_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::readiness>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::readiness>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::readiness>, tagged_array_view<float, military::fleet_tag>> get_row() {
			 return tagged_array_view<float, military::fleet_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::readiness>, tagged_array_view<float const, military::fleet_tag>> get_row() const {
			 return tagged_array_view<float const, military::fleet_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::size>, float&> get(military::fleet_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::size>, float const&> get(military::fleet_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::size>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::size>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::size>, tagged_array_view<float, military::fleet_tag>> get_row() {
			 return tagged_array_view<float, military::fleet_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::size>, tagged_array_view<float const, military::fleet_tag>> get_row() const {
			 return tagged_array_view<float const, military::fleet_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::arrival_time>, date_tag&> get(military::fleet_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::arrival_time>, date_tag const&> get(military::fleet_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::arrival_time>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::fleet_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, fleet::arrival_time>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::arrival_time>, tagged_array_view<date_tag, military::fleet_tag>> get_row() {
			 return tagged_array_view<date_tag, military::fleet_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, fleet::arrival_time>, tagged_array_view<date_tag const, military::fleet_tag>> get_row() const {
			 return tagged_array_view<date_tag const, military::fleet_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }

		 military::fleet_tag get_new() {
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
		 void release(military::fleet_tag i) {
			 set<fleet::leader>(i, military::leader_tag());
			 set<fleet::location>(i, provinces::province_tag());
			 set<fleet::supply>(i, float());
			 set<fleet::readiness>(i, float());
			 set<fleet::size>(i, float());
			 set<fleet::arrival_time>(i, date_tag());
			 m_index.values[to_index(i)] = first_free;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == military::fleet_tag(military::fleet_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(military::fleet_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::fleet_tag(military::fleet_tag::value_base_t(i))) f(military::fleet_tag(military::fleet_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == military::fleet_tag(military::fleet_tag::value_base_t(i))) f(military::fleet_tag(military::fleet_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::fleet_tag(military::fleet_tag::value_base_t(i))) { if(f(military::fleet_tag(military::fleet_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<fleet::container, fleet::leader> {
	static constexpr bool value = true;
	 using type = military::leader_tag&;
	 using const_type = military::leader_tag const&;
	 using row = tagged_array_view<military::leader_tag, military::fleet_tag>;
	 using const_row = tagged_array_view<military::leader_tag const, military::fleet_tag>;
};
template<>
struct supports_index<fleet::container, fleet::location> {
	static constexpr bool value = true;
	 using type = provinces::province_tag&;
	 using const_type = provinces::province_tag const&;
	 using row = tagged_array_view<provinces::province_tag, military::fleet_tag>;
	 using const_row = tagged_array_view<provinces::province_tag const, military::fleet_tag>;
};
template<>
struct supports_index<fleet::container, fleet::supply> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::fleet_tag>;
	 using const_row = tagged_array_view<float const, military::fleet_tag>;
};
template<>
struct supports_index<fleet::container, fleet::readiness> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::fleet_tag>;
	 using const_row = tagged_array_view<float const, military::fleet_tag>;
};
template<>
struct supports_index<fleet::container, fleet::size> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::fleet_tag>;
	 using const_row = tagged_array_view<float const, military::fleet_tag>;
};
template<>
struct supports_index<fleet::container, fleet::arrival_time> {
	static constexpr bool value = true;
	 using type = date_tag&;
	 using const_type = date_tag const&;
	 using row = tagged_array_view<date_tag, military::fleet_tag>;
	 using const_row = tagged_array_view<date_tag const, military::fleet_tag>;
};

template<>
class serialization::serializer<fleet::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, fleet::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == military::fleet_tag(military::fleet_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = military::fleet_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::leader, military::leader_tag>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::leader, military::leader_tag>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::location, provinces::province_tag>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::location, provinces::province_tag>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::supply, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::supply, float>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::supply, float>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::readiness, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::readiness, float>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::readiness, float>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::size, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::size, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::size, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::size, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::size, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::size, float>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::arrival_time, date_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::arrival_time, date_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, fleet::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = military::fleet_tag();
		for(int32_t i = fleet::container_size - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = military::fleet_tag(military::fleet_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = military::fleet_tag(military::fleet_tag::value_base_t(i));
			}
		}
		if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::leader, military::leader_tag>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::leader, military::leader_tag>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::location, provinces::province_tag>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::location, provinces::province_tag>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::supply, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::supply, float>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::supply, float>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::readiness, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::readiness, float>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::readiness, float>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::size, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::size, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::size, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::size, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::size, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::size, float>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::arrival_time, date_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<fleet::arrival_time, date_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static size_t size(fleet::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(military::fleet_tag) * obj.size_used+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<fleet::leader, military::leader_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<fleet::leader, military::leader_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<fleet::leader, military::leader_tag>::size(obj.m_0.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_0.values, obj.m_0.values + max, 0ui64, std::plus<>(), [&](military::leader_tag const& m) {
					 return serialization::tagged_serializer<fleet::leader, military::leader_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<fleet::location, provinces::province_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<fleet::location, provinces::province_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<fleet::location, provinces::province_tag>::size(obj.m_1.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_1.values, obj.m_1.values + max, 0ui64, std::plus<>(), [&](provinces::province_tag const& m) {
					 return serialization::tagged_serializer<fleet::location, provinces::province_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<fleet::supply, float>::size());
			 } else if constexpr(serialization::tagged_serializer<fleet::supply, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<fleet::supply, float>::size(obj.m_2.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_2.values, obj.m_2.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<fleet::supply, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<fleet::readiness, float>::size());
			 } else if constexpr(serialization::tagged_serializer<fleet::readiness, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<fleet::readiness, float>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<fleet::readiness, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<fleet::size, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<fleet::size, float>::size());
			 } else if constexpr(serialization::tagged_serializer<fleet::size, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<fleet::size, float>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<fleet::size, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<fleet::arrival_time, date_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<fleet::arrival_time, date_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<fleet::arrival_time, date_tag>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](date_tag const& m) {
					 return serialization::tagged_serializer<fleet::arrival_time, date_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
);
	 }
};
#pragma warning( pop )

