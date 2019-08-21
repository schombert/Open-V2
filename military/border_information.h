#include "common\\common.h"
#include "common\\shared_tags.h"
#include "concurrency_tools\\ve.h"
#include "simple_serialize\\simple_serialize.hpp"

#pragma warning( push )
#pragma warning( disable : 4324 )

namespace border_information {
	class container;
}

template<>
class serialization::serializer<border_information::container>;

namespace border_information {
	struct readiness;
	struct supply;
	struct hqs;
	struct stance;
	struct owner;
	struct against;

constexpr int32_t max_count = province::container_size;

	class alignas(64) container {
		 int32_t size_used = 0;
		 military::border_information_tag first_free;
		 struct alignas(64) dtype_index { 
			 uint8_t padding[(sizeof(military::border_information_tag) + 63ui32) & ~63ui32]; 
			 military::border_information_tag values[(sizeof(military::border_information_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(military::border_information_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::border_information_tag)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_index() { std::uninitialized_value_construct_n(values - 1, (sizeof(military::border_information_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(military::border_information_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(military::border_information_tag)) - 1ui32) : uint32_t(province::container_size)) + 1); } 		 } m_index;

		 struct alignas(64) dtype_0 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_0() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_0;
		 struct alignas(64) dtype_1 { 
			 uint8_t padding[(sizeof(float) + 63ui32) & ~63ui32]; 
			 float values[(sizeof(float) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_1() { std::uninitialized_value_construct_n(values - 1, (sizeof(float) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(float))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(float)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_1;
		 struct alignas(64) dtype_2 { 
			 uint8_t padding[(sizeof(array_tag<military::hq_commitment_information, int32_t, false>) + 63ui32) & ~63ui32]; 
			 array_tag<military::hq_commitment_information, int32_t, false> values[(sizeof(array_tag<military::hq_commitment_information, int32_t, false>) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(array_tag<military::hq_commitment_information, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::hq_commitment_information, int32_t, false>)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_2() { std::uninitialized_value_construct_n(values - 1, (sizeof(array_tag<military::hq_commitment_information, int32_t, false>) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(array_tag<military::hq_commitment_information, int32_t, false>))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(array_tag<military::hq_commitment_information, int32_t, false>)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_2;
		 struct alignas(64) dtype_3 { 
			 uint8_t padding[(sizeof(int8_t) + 63ui32) & ~63ui32]; 
			 int8_t values[(sizeof(int8_t) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(int8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int8_t)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_3() { std::uninitialized_value_construct_n(values - 1, (sizeof(int8_t) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(int8_t))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(int8_t)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_3;
		 struct alignas(64) dtype_4 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_4() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_4;
		 struct alignas(64) dtype_5 { 
			 uint8_t padding[(sizeof(nations::country_tag) + 63ui32) & ~63ui32]; 
			 nations::country_tag values[(sizeof(nations::country_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province::container_size))]; 
			 dtype_5() { std::uninitialized_value_construct_n(values - 1, (sizeof(nations::country_tag) <= 64 ? (uint32_t(province::container_size) + (64ui32 / uint32_t(sizeof(nations::country_tag))) - 1ui32) & ~(64ui32 / uint32_t(sizeof(nations::country_tag)) - 1ui32) : uint32_t(province::container_size)) + 1); }
		 } m_5;

		 public:
		 friend class serialization::serializer<container>;
		 container() {
			 for(int32_t i = province::container_size - 1; i >= 0; --i) {
				 m_index.values[i] = first_free;
				 first_free = military::border_information_tag(military::border_information_tag::value_base_t(i));
			 }
		 }

		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::readiness>, float&> get(military::border_information_tag i) {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::readiness>, float const&> get(military::border_information_tag i) const {
			 return m_0.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::readiness>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::readiness>> {
			 m_0.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::readiness>, tagged_array_view<float, military::border_information_tag>> get_row() {
			 return tagged_array_view<float, military::border_information_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::readiness>, tagged_array_view<float const, military::border_information_tag>> get_row() const {
			 return tagged_array_view<float const, military::border_information_tag>(m_0.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::supply>, float&> get(military::border_information_tag i) {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::supply>, float const&> get(military::border_information_tag i) const {
			 return m_1.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::supply>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::supply>> {
			 m_1.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::supply>, tagged_array_view<float, military::border_information_tag>> get_row() {
			 return tagged_array_view<float, military::border_information_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::supply>, tagged_array_view<float const, military::border_information_tag>> get_row() const {
			 return tagged_array_view<float const, military::border_information_tag>(m_1.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::hqs>, array_tag<military::hq_commitment_information, int32_t, false>&> get(military::border_information_tag i) {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::hqs>, array_tag<military::hq_commitment_information, int32_t, false> const&> get(military::border_information_tag i) const {
			 return m_2.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::hqs>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::hqs>> {
			 m_2.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::hqs>, tagged_array_view<array_tag<military::hq_commitment_information, int32_t, false>, military::border_information_tag>> get_row() {
			 return tagged_array_view<array_tag<military::hq_commitment_information, int32_t, false>, military::border_information_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::hqs>, tagged_array_view<array_tag<military::hq_commitment_information, int32_t, false> const, military::border_information_tag>> get_row() const {
			 return tagged_array_view<array_tag<military::hq_commitment_information, int32_t, false> const, military::border_information_tag>(m_2.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::stance>, int8_t&> get(military::border_information_tag i) {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::stance>, int8_t const&> get(military::border_information_tag i) const {
			 return m_3.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::stance>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::stance>> {
			 m_3.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::stance>, tagged_array_view<int8_t, military::border_information_tag>> get_row() {
			 return tagged_array_view<int8_t, military::border_information_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::stance>, tagged_array_view<int8_t const, military::border_information_tag>> get_row() const {
			 return tagged_array_view<int8_t const, military::border_information_tag>(m_3.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::owner>, nations::country_tag&> get(military::border_information_tag i) {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::owner>, nations::country_tag const&> get(military::border_information_tag i) const {
			 return m_4.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::owner>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::owner>> {
			 m_4.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::owner>, tagged_array_view<nations::country_tag, military::border_information_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, military::border_information_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::owner>, tagged_array_view<nations::country_tag const, military::border_information_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, military::border_information_tag>(m_4.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::against>, nations::country_tag&> get(military::border_information_tag i) {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::against>, nations::country_tag const&> get(military::border_information_tag i) const {
			 return m_5.values[to_index(i)];
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type v) -> std::enable_if_t<std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::against>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX, typename value_type>
		 auto set(military::border_information_tag i, value_type const& v) -> std::enable_if_t<!std::is_trivially_copyable_v<value_type> && std::is_same_v<INDEX, border_information::against>> {
			 m_5.values[to_index(i)] = v;
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::against>, tagged_array_view<nations::country_tag, military::border_information_tag>> get_row() {
			 return tagged_array_view<nations::country_tag, military::border_information_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }
		 template<typename INDEX>
		 std::enable_if_t<std::is_same_v<INDEX, border_information::against>, tagged_array_view<nations::country_tag const, military::border_information_tag>> get_row() const {
			 return tagged_array_view<nations::country_tag const, military::border_information_tag>(m_5.values, ve::to_vector_size(uint32_t(size_used)));
		 }

		 military::border_information_tag get_new() {
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
		 void release(military::border_information_tag i) {
			 set<border_information::readiness>(i, float());
			 set<border_information::supply>(i, float());
			 set<border_information::hqs>(i, array_tag<military::hq_commitment_information, int32_t, false>());
			 set<border_information::stance>(i, int8_t());
			 set<border_information::owner>(i, nations::country_tag());
			 set<border_information::against>(i, nations::country_tag());
			 m_index.values[to_index(i)] = first_free;
			 first_free = i;
			 if(size_used - 1 == to_index(i)) {
				 for(int32_t j = size_used; j >= 0; --j) {
					 if(m_index.values[j] == military::border_information_tag(military::border_information_tag::value_base_t(j))) {
						 size_used = j + 1;
						 return;
					 }
				 }
				 size_used = 0;			 }
		 }
		 void reset() { this->~container(); new (this)container(); }
		 int32_t size() const { return size_used; }
		 uint32_t vector_size() const { return ve::to_vector_size(uint32_t(size_used)); }
		 bool is_valid_index(military::border_information_tag i) const { return ::is_valid_index(i) & (int32_t(to_index(i)) < size_used) & (m_index.values[to_index(i)] == i); }
		 template<typename FN>
		 void for_each(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::border_information_tag(military::border_information_tag::value_base_t(i))) f(military::border_information_tag(military::border_information_tag::value_base_t(i)));
			 }
		 }
		 template<typename FN, typename P = concurrency::auto_partitioner>
		 void parallel_for_each(FN const& f, P&& p = concurrency::auto_partitioner()) const {
			 concurrency::parallel_for(0, size_used, [&p, &f, _this = this](int32_t i) {
				 if(_this->m_index.values[i] == military::border_information_tag(military::border_information_tag::value_base_t(i))) f(military::border_information_tag(military::border_information_tag::value_base_t(i)));
			 }, p);
		 }
		 template<typename FN>
		 bool any_of(FN const& f) const {
			 for(int32_t i = 0; i < size_used; ++i) {
				 if(m_index.values[i] == military::border_information_tag(military::border_information_tag::value_base_t(i))) { if(f(military::border_information_tag(military::border_information_tag::value_base_t(i)))) return true; }
			 }
			 return false;
		 }
	};

}
template<>
struct supports_index<border_information::container, border_information::readiness> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::border_information_tag>;
	 using const_row = tagged_array_view<float const, military::border_information_tag>;
};
template<>
struct supports_index<border_information::container, border_information::supply> {
	static constexpr bool value = true;
	 using type = float&;
	 using const_type = float const&;
	 using row = tagged_array_view<float, military::border_information_tag>;
	 using const_row = tagged_array_view<float const, military::border_information_tag>;
};
template<>
struct supports_index<border_information::container, border_information::hqs> {
	static constexpr bool value = true;
	 using type = array_tag<military::hq_commitment_information, int32_t, false>&;
	 using const_type = array_tag<military::hq_commitment_information, int32_t, false> const&;
	 using row = tagged_array_view<array_tag<military::hq_commitment_information, int32_t, false>, military::border_information_tag>;
	 using const_row = tagged_array_view<array_tag<military::hq_commitment_information, int32_t, false> const, military::border_information_tag>;
};
template<>
struct supports_index<border_information::container, border_information::stance> {
	static constexpr bool value = true;
	 using type = int8_t&;
	 using const_type = int8_t const&;
	 using row = tagged_array_view<int8_t, military::border_information_tag>;
	 using const_row = tagged_array_view<int8_t const, military::border_information_tag>;
};
template<>
struct supports_index<border_information::container, border_information::owner> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, military::border_information_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, military::border_information_tag>;
};
template<>
struct supports_index<border_information::container, border_information::against> {
	static constexpr bool value = true;
	 using type = nations::country_tag&;
	 using const_type = nations::country_tag const&;
	 using row = tagged_array_view<nations::country_tag, military::border_information_tag>;
	 using const_row = tagged_array_view<nations::country_tag const, military::border_information_tag>;
};

template<>
class serialization::serializer<border_information::container> {
	 public:
	 static constexpr bool has_static_size = false; 
	 static constexpr bool has_simple_serialize = false; 
	 template<typename ... CONTEXT>
	 static void serialize_object(std::byte* &output, border_information::container const& obj, CONTEXT&& ... c) {
		 serialization::serialize(output, obj.size_used);
		 for(int32_t i = 0; i < obj.size_used; ++i) {
			 if(obj.m_index.values[i] == military::border_information_tag(military::border_information_tag::value_base_t(i))) {
				 serialization::serialize(output, obj.m_index.values[i]);
			 } else {
				 auto tag = military::border_information_tag();
				 serialization::serialize(output, tag);
			 }
		 }
		if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::readiness, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::readiness, float>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::readiness, float>::serialize_object(output, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::supply, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::supply, float>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::supply, float>::serialize_object(output, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::serialize_object(output, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::stance, int8_t>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::stance, int8_t>::serialize_object(output, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::owner, nations::country_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::owner, nations::country_tag>::serialize_object(output, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::against, nations::country_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_simple_serialize) {
				 serialization::serialize_array(output, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::against, nations::country_tag>::serialize_object(output, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static void deserialize_object(std::byte const* &input, border_information::container& obj, CONTEXT&& ... c) {
		 obj.reset();
		 serialization::deserialize(input, obj.size_used);
		 serialization::deserialize_array(input, obj.m_index.values, obj.size_used);
		obj.first_free = military::border_information_tag();
		for(int32_t i = province::container_size - 1; i >= obj.size_used; --i) {
			 obj.m_index.values[i] = obj.first_free;
			 obj.first_free = military::border_information_tag(military::border_information_tag::value_base_t(i));
		}
		for(int32_t i = obj.size_used - 1; i >= 0; --i) {
			 if(!::is_valid_index(obj.m_index.values[i])) {
				 obj.m_index.values[i] = obj.first_free;
				 obj.first_free = military::border_information_tag(military::border_information_tag::value_base_t(i));
			}
		}
		if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::readiness, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::readiness, float>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_0.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::readiness, float>::deserialize_object(input, obj.m_0.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::supply, float>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::supply, float>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_1.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::supply, float>::deserialize_object(input, obj.m_1.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_2.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::deserialize_object(input, obj.m_2.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::stance, int8_t>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_3.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::stance, int8_t>::deserialize_object(input, obj.m_3.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::owner, nations::country_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_4.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::owner, nations::country_tag>::deserialize_object(input, obj.m_4.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
		if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_static_size) {
			 if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::size() == 0) {
			 } else if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::against, nations::country_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		} else {
			 if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_simple_serialize) {
				 serialization::deserialize_array(input, obj.m_5.values, obj.size_used);
			 } else {
				 for(int32_t i = 0; i < obj.size_used; ++i)
					 serialization::tagged_serializer<border_information::against, nations::country_tag>::deserialize_object(input, obj.m_5.values[i], std::forward<CONTEXT>(c)...);
			 }
		}
	 }
	 template<typename ... CONTEXT>
	 static size_t size(border_information::container const& obj, CONTEXT&& ... c) {
		 return size_t(sizeof(obj.size_used)+ sizeof(military::border_information_tag) * obj.size_used+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<border_information::readiness, float>::size());
			 } else if constexpr(serialization::tagged_serializer<border_information::readiness, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<border_information::readiness, float>::size(obj.m_0.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_0.values, obj.m_0.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<border_information::readiness, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<border_information::supply, float>::size());
			 } else if constexpr(serialization::tagged_serializer<border_information::supply, float>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<border_information::supply, float>::size(obj.m_1.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_1.values, obj.m_1.values + max, 0ui64, std::plus<>(), [&](float const& m) {
					 return serialization::tagged_serializer<border_information::supply, float>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::size());
			 } else if constexpr(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::size(obj.m_2.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_2.values, obj.m_2.values + max, 0ui64, std::plus<>(), [&](array_tag<military::hq_commitment_information, int32_t, false> const& m) {
					 return serialization::tagged_serializer<border_information::hqs, array_tag<military::hq_commitment_information, int32_t, false>>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<border_information::stance, int8_t>::size());
			 } else if constexpr(serialization::tagged_serializer<border_information::stance, int8_t>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<border_information::stance, int8_t>::size(obj.m_3.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_3.values, obj.m_3.values + max, 0ui64, std::plus<>(), [&](int8_t const& m) {
					 return serialization::tagged_serializer<border_information::stance, int8_t>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<border_information::owner, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<border_information::owner, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<border_information::owner, nations::country_tag>::size(obj.m_4.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_4.values, obj.m_4.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<border_information::owner, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
+ [&, max = obj.size_used](){
			 if(max == 0)
				 return size_t(0);
			 if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_static_size) {
				 return size_t(max  * serialization::tagged_serializer<border_information::against, nations::country_tag>::size());
			 } else if constexpr(serialization::tagged_serializer<border_information::against, nations::country_tag>::has_simple_serialize) {
				 return size_t(serialization::tagged_serializer<border_information::against, nations::country_tag>::size(obj.m_5.values[0], std::forward<CONTEXT>(c)...) * max); 
			 } else {
				 return size_t(std::transform_reduce(obj.m_5.values, obj.m_5.values + max, 0ui64, std::plus<>(), [&](nations::country_tag const& m) {
					 return serialization::tagged_serializer<border_information::against, nations::country_tag>::size(m, std::forward<CONTEXT>(c)...); 
				 })); 
			 }
		 }()
);
	 }
};
#pragma warning( pop )

