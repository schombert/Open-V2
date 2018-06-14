#include "gtest\\gtest.h"
#include "simple_serialize\\simple_serialize.hpp"

using namespace serialization;

TEST(serlialize, serialize_int) {
	int i = 15;
	
	const auto sz = serialize_size(i);

	EXPECT_EQ(sizeof(int), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	serialize(data_in.data(), i);

	int output = 0;

	deserialize(data_in.data(), output);

	EXPECT_EQ(15, output);
}

TEST(serlialize, serialize_empty_vector) {
	std::vector<int> ivec;

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(int) * 0 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	serialize(data_in.data(), ivec);

	std::vector<int> ovec;
	deserialize(data_in.data(), ovec);

	EXPECT_EQ(0ui64, ovec.size());
}

TEST(serlialize, serialize_int_array) {
	std::vector<int> ivec = { 5, -100, 45 };

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(int) * 3 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	serialize(data_in.data(), ivec);

	std::vector<int> ovec;
	deserialize(data_in.data(), ovec);

	EXPECT_EQ(3ui64, ovec.size());
	EXPECT_EQ(5, ovec[0]);
	EXPECT_EQ(-100, ovec[1]);
	EXPECT_EQ(45, ovec[2]);
}

struct tobj_a {
	int32_t ival = 0;
	float fval = 0.0f;
};

TEST(serlialize, serialize_simple_object_array) {
	std::vector<tobj_a> ivec = { tobj_a{5, 0.75f}, tobj_a{-100, 1.5f}, tobj_a{ 42, 100.0f } };

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(tobj_a) * 3 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	serialize(data_in.data(), ivec);

	std::vector<tobj_a> ovec;
	deserialize(data_in.data(), ovec);

	EXPECT_EQ(3ui64, ovec.size());
	EXPECT_EQ(5, ovec[0].ival);
	EXPECT_EQ(0.75f, ovec[0].fval);
	EXPECT_EQ(-100, ovec[1].ival);
	EXPECT_EQ(1.5f, ovec[1].fval);
	EXPECT_EQ(42, ovec[2].ival);
	EXPECT_EQ(100.0f, ovec[2].fval);
}

struct t_vec_container {
	float val = 0.0f;
	std::vector<int> ivals;
};

template<>
class serialization::serializer<t_vec_container> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static std::byte* serialize_object(std::byte* output, t_vec_container const& obj, CONTEXT const& ... c) {
		const auto s1 = serializer<float>::serialize_object(output, obj.val, c...);
		return serializer<std::vector<int>>::serialize_object(s1, obj.ivals, c...);
	}
	template<typename ... CONTEXT>
	static std::byte const* deserialize_object(std::byte const* input, t_vec_container& obj, CONTEXT const& ... c) {
		const auto s1 = serializer<float>::deserialize_object(input, obj.val, c...);
		return serializer<std::vector<int>>::deserialize_object(s1, obj.ivals, c...);
	}
	static size_t size(t_vec_container const& obj) {
		return serializer<float>::size(obj.val) + serializer<std::vector<int>>::size(obj.ivals);
	}
};

TEST(serlialize, serialize_complex_object_array) {
	std::vector<t_vec_container> ivec;

	ivec.emplace_back();
	ivec.back().val = 5.5f;
	ivec.back().ivals = std::vector<int>{ 1, 2, 3 };
	ivec.emplace_back();
	ivec.back().val = -15.0f;
	ivec.back().ivals = std::vector<int>{ 5, 7, 9, 11 };

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(int) * 7 + sizeof(float) * 2 +  sizeof(uint32_t) * 3, sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	serialize(data_in.data(), ivec);

	std::vector<t_vec_container> ovec;
	deserialize(data_in.data(), ovec);

	EXPECT_EQ(2ui64, ovec.size());
	EXPECT_EQ(5.5f, ovec[0].val);
	EXPECT_EQ(3ui64, ovec[0].ivals.size());
	EXPECT_EQ(1, ovec[0].ivals[0]);
	EXPECT_EQ(2, ovec[0].ivals[1]);
	EXPECT_EQ(3, ovec[0].ivals[2]);
	EXPECT_EQ(-15.0f, ovec[1].val);
	EXPECT_EQ(4ui64, ovec[1].ivals.size());
	EXPECT_EQ(5, ovec[1].ivals[0]);
	EXPECT_EQ(7, ovec[1].ivals[1]);
	EXPECT_EQ(9, ovec[1].ivals[2]);
	EXPECT_EQ(11, ovec[1].ivals[3]);
}
