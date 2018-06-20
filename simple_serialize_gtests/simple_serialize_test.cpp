#include "gtest\\gtest.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "common\\shared_tags.h"
#include "concurrency_tools\\concurrency_tools.hpp"

using namespace serialization;

TEST(serialize, serialize_int) {
	int i = 15;
	
	const auto sz = serialize_size(i);

	EXPECT_EQ(sizeof(int), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, i);

	int output = 0;

	std::byte const* optr = data_in.data();
	deserialize(optr, output);

	EXPECT_EQ(15, output);
}

TEST(serialize, serialize_string) {
	std::string i("text text string text string text other text padding padding padding");
	std::string i_cpy(i);

	const auto sz = serialize_size(i);

	EXPECT_EQ(sizeof(uint32_t) + i.length() * sizeof(char), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, i);

	i[0] = 'k';

	std::string output;

	std::byte const* optr = data_in.data();
	deserialize(optr, output);


	EXPECT_EQ(i_cpy.length(), output.length());
	EXPECT_EQ(i_cpy, output);
}

TEST(serialize, serialize_empty_vector) {
	std::vector<int> ivec;

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(int) * 0 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	std::byte const* optr = data_in.data();
	std::vector<int> ovec;
	deserialize(optr, ovec);

	EXPECT_EQ(0ui64, ovec.size());
}

TEST(serialize, serialize_int_array) {
	std::vector<int> ivec = { 5, -100, 45 };

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(int) * 3 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	std::byte const* optr = data_in.data();
	std::vector<int> ovec;
	deserialize(optr, ovec);

	EXPECT_EQ(3ui64, ovec.size());
	EXPECT_EQ(5, ovec[0]);
	EXPECT_EQ(-100, ovec[1]);
	EXPECT_EQ(45, ovec[2]);
}

struct tobj_a {
	int32_t ival = 0;
	float fval = 0.0f;
};

template<>
class serializer<tobj_a> : public memcpy_serializer<tobj_a> {};

TEST(serialize, serialize_simple_object_array) {
	std::vector<tobj_a> ivec = { tobj_a{5, 0.75f}, tobj_a{-100, 1.5f}, tobj_a{ 42, 100.0f } };

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(tobj_a) * 3 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	std::byte const* optr = data_in.data();
	std::vector<tobj_a> ovec;
	deserialize(optr, ovec);

	EXPECT_EQ(3ui64, ovec.size());
	EXPECT_EQ(5, ovec[0].ival);
	EXPECT_EQ(0.75f, ovec[0].fval);
	EXPECT_EQ(-100, ovec[1].ival);
	EXPECT_EQ(1.5f, ovec[1].fval);
	EXPECT_EQ(42, ovec[2].ival);
	EXPECT_EQ(100.0f, ovec[2].fval);
}

TEST(serialize, serialize_tagged_vector) {
	tagged_vector<tobj_a, military::unit_type_tag> ivec;
	ivec.emplace_back(tobj_a{ 5, 0.75f });
	ivec.emplace_back(tobj_a{ -100, 1.5f });
	ivec.emplace_back(tobj_a{ 42, 100.0f });

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(tobj_a) * 3 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	std::byte const* optr = data_in.data();
	tagged_vector<tobj_a, military::unit_type_tag> ovec;
	deserialize(optr, ovec);

	EXPECT_EQ(3ui64, ovec.size());
	EXPECT_EQ(5, ovec[military::unit_type_tag(0)].ival);
	EXPECT_EQ(0.75f, ovec[military::unit_type_tag(0)].fval);
	EXPECT_EQ(-100, ovec[military::unit_type_tag(1)].ival);
	EXPECT_EQ(1.5f, ovec[military::unit_type_tag(1)].fval);
	EXPECT_EQ(42, ovec[military::unit_type_tag(2)].ival);
	EXPECT_EQ(100.0f, ovec[military::unit_type_tag(2)].fval);
}

TEST(serialize, serialize_tagged_2d_vector) {
	tagged_fixed_2dvector<float, military::unit_type_tag, military::cb_type_tag> ivec;
	ivec.reset(3);
	ivec.resize(2);
	ivec.get(military::unit_type_tag(0), military::cb_type_tag(0)) = 10.0f;
	ivec.get(military::unit_type_tag(0), military::cb_type_tag(1)) = 5.0f;
	ivec.get(military::unit_type_tag(0), military::cb_type_tag(2)) = 1.0f;
	ivec.get(military::unit_type_tag(1), military::cb_type_tag(0)) = 0.5f;
	ivec.get(military::unit_type_tag(1), military::cb_type_tag(1)) = 1.0f;
	ivec.get(military::unit_type_tag(1), military::cb_type_tag(2)) = 3.0f;

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(float) * 6 + sizeof(uint32_t) + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	std::byte const* optr = data_in.data();
	tagged_fixed_2dvector<float, military::unit_type_tag, military::cb_type_tag> ovec;
	deserialize(optr, ovec);

	EXPECT_EQ(6ui64, ovec.size());
	EXPECT_EQ(3ui32, ovec.inner_size());
	EXPECT_EQ(2ui64, ovec.outer_size());

	EXPECT_EQ(ovec.get(military::unit_type_tag(0), military::cb_type_tag(0)), 10.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(0), military::cb_type_tag(1)), 5.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(0), military::cb_type_tag(2)), 1.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(1), military::cb_type_tag(0)), 0.5f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(1), military::cb_type_tag(1)), 1.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(1), military::cb_type_tag(2)), 3.0f);
}

TEST(serialize, serialize_tagged_blocked_2d_vector) {
	tagged_fixed_blocked_2dvector<float, military::unit_type_tag, military::cb_type_tag, aligned_allocator_32<float>> ivec;
	ivec.reset(3);
	ivec.resize(2);
	ivec.get(military::unit_type_tag(0), military::cb_type_tag(0)) = 10.0f;
	ivec.get(military::unit_type_tag(0), military::cb_type_tag(1)) = 5.0f;
	ivec.get(military::unit_type_tag(0), military::cb_type_tag(2)) = 1.0f;
	ivec.get(military::unit_type_tag(1), military::cb_type_tag(0)) = 0.5f;
	ivec.get(military::unit_type_tag(1), military::cb_type_tag(1)) = 1.0f;
	ivec.get(military::unit_type_tag(1), military::cb_type_tag(2)) = 3.0f;

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(64 + sizeof(uint32_t) + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	std::byte const* optr = data_in.data();
	tagged_fixed_blocked_2dvector<float, military::unit_type_tag, military::cb_type_tag, aligned_allocator_32<float>> ovec;
	deserialize(optr, ovec);

	EXPECT_EQ(ivec.size(), ovec.size());
	EXPECT_EQ(ivec.inner_size(), ovec.inner_size());
	EXPECT_EQ(ivec.outer_size(), ovec.outer_size());

	EXPECT_EQ(ovec.get(military::unit_type_tag(0), military::cb_type_tag(0)), 10.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(0), military::cb_type_tag(1)), 5.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(0), military::cb_type_tag(2)), 1.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(1), military::cb_type_tag(0)), 0.5f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(1), military::cb_type_tag(1)), 1.0f);
	EXPECT_EQ(ovec.get(military::unit_type_tag(1), military::cb_type_tag(2)), 3.0f);
}

struct t_vec_container {
	float val = 0.0f;
	std::vector<int> ivals;
};

struct int_counter {
	size_t total_ints = 0;
};

template<>
class serialization::serializer<t_vec_container> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static void serialize_object(std::byte* &output, t_vec_container const& obj, CONTEXT&& ... c) {
		serialize(output, obj.val, std::forward<CONTEXT>(c)...);
		serialize(output, obj.ivals, std::forward<CONTEXT>(c)...);
	}
	static void deserialize_object(std::byte const* &input, t_vec_container& obj) {
		deserialize(input, obj.val);
		deserialize(input, obj.ivals);
	}
	static void deserialize_object(std::byte const* &input, t_vec_container& obj, int_counter& cntr) {
		deserialize(input, obj.val, cntr);
		deserialize(input, obj.ivals, cntr);
		cntr.total_ints += obj.ivals.size();
	}
	static size_t size(t_vec_container const& obj) {
		return serialize_size(obj.val) + serialize_size(obj.ivals);
	}
};

TEST(serialize, serialize_complex_object_array) {
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

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	ivec[0].ivals[0] = 0;

	std::byte const* optr = data_in.data();
	std::vector<t_vec_container> ovec;
	deserialize(optr, ovec);

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

TEST(serialize, serialize_complex_object_array_with_context) {
	std::vector<t_vec_container> ivec;

	ivec.emplace_back();
	ivec.back().val = 5.5f;
	ivec.back().ivals = std::vector<int>{ 1, 2, 3 };
	ivec.emplace_back();
	ivec.back().val = -15.0f;
	ivec.back().ivals = std::vector<int>{ 5, 7, 9, 11 };

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(int) * 7 + sizeof(float) * 2 + sizeof(uint32_t) * 3, sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	int_counter counter;

	std::byte const* optr = data_in.data();
	std::vector<t_vec_container> ovec;
	deserialize(optr, ovec, counter);

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
	EXPECT_EQ(7ui64, counter.total_ints);
}

TEST(serialize, serialize_flat_map) {
	boost::container::flat_map<int32_t, double> ivec;
	ivec.emplace(10, 1.0);
	ivec.emplace(-10, 2.5);
	ivec.emplace(0, 3.0);

	const auto sz = serialize_size(ivec);

	EXPECT_EQ(sizeof(std::pair<int32_t, double>) * 3 + sizeof(uint32_t), sz);

	std::vector<std::byte> data_in;
	data_in.resize(sz);

	auto iptr = data_in.data();
	serialize(iptr, ivec);

	std::byte const* optr = data_in.data();
	boost::container::flat_map<int32_t, double> ovec;
	deserialize(optr, ovec);

	EXPECT_EQ(3ui64, ovec.size());
	EXPECT_EQ(1.0, ovec[10]);
	EXPECT_EQ(2.5, ovec[-10]);
	EXPECT_EQ(3.0, ovec[0]);
}
