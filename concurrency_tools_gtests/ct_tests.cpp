#include "common\\common.h"
#include "common\\shared_tags.h"
#include "gtest\\gtest.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "concurrency_tools\\variable_layout.h"
#include "concurrency_tools\\ve.h"

TEST(concurrency_tools, string_construction) {
	concurrent_string a;
	concurrent_string b("small data");
	concurrent_string c(b);

	EXPECT_EQ(0ui32, a.length());
	EXPECT_EQ(10ui32, b.length());
	EXPECT_EQ(10ui32, c.length());
	EXPECT_STREQ("", a.c_str());
	EXPECT_STREQ("small data", b.c_str());
	EXPECT_STREQ("small data", c.c_str());
}

TEST(concurrency_tools, string_small_move_add) {
	concurrent_string b("small data");
	concurrent_string c(std::move(b));

	EXPECT_EQ(10ui32, c.length());
	EXPECT_STREQ("small data", c.c_str());

	c += "x";
	EXPECT_STREQ("small datax", c.c_str());

	EXPECT_EQ(11ui32, c.length());
}

TEST(concurrency_tools, string_large_move_add) {
	concurrent_string b("__not small data__");
	concurrent_string c(std::move(b));

	EXPECT_EQ(0ui32, b.length());
	EXPECT_EQ(18ui32, c.length());
	EXPECT_STREQ("", b.c_str());
	EXPECT_STREQ("__not small data__", c.c_str());

	c += "x";
	EXPECT_STREQ("__not small data__x", c.c_str());
	EXPECT_EQ(19ui32, c.length());
}

TEST(concurrency_tools, string_small_to_large) {
	concurrent_string b("small data");
	b += b;

	EXPECT_EQ(20ui32, b.length());
	EXPECT_STREQ("small datasmall data", b.c_str());
}

TEST(concurrency_tools, limit_case_str) {
	concurrent_string b("012345678901234");

	EXPECT_EQ(15ui32, b.length());
	EXPECT_STREQ("012345678901234", b.c_str());
}

TEST(concurrency_tools, clear_string) {
	concurrent_string b("012345678901234");
	concurrent_string c("012345678901234012345678901234");

	b.clear();
	c.clear();

	EXPECT_EQ(0ui32, b.length());
	EXPECT_EQ(0ui32, c.length());
	EXPECT_STREQ("", b.c_str());
	EXPECT_STREQ("", c.c_str());
}

TEST(concurrency_tools, string_expressions) {
	std::string t = empty_string_expression();

	EXPECT_EQ(0ui64, t.length());

	std::string u = empty_string_expression() + "uuu" + "vv";

	EXPECT_EQ(5ui64, u.length());
	EXPECT_EQ(u, "uuuvv");

	std::string u2 = empty_string_expression() + std::string("uuu") + "vv";

	EXPECT_EQ(5ui64, u2.length());
	EXPECT_EQ(u2, "uuuvv");
}

TEST(concurrency_tools, allocator) {
	std::vector<int, concurrent_allocator<int>> tv;
	for (int i = 0; i < 8; ++i)
		tv.push_back(i);

	EXPECT_EQ(8ui64, tv.size());
	EXPECT_EQ(0, tv[0]);
	EXPECT_EQ(7, tv[7]);

	for (int i = 8; i < 128; ++i)
		tv.push_back(i);

	EXPECT_EQ(128ui64, tv.size());
	EXPECT_EQ(0, tv[0]);
	EXPECT_EQ(7, tv[7]);
	EXPECT_EQ(71, tv[71]);
	EXPECT_EQ(127, tv[127]);

	tv.clear();
	tv.shrink_to_fit();

	EXPECT_EQ(0ui64, tv.size());
	EXPECT_EQ(0ui64, tv.capacity());
}

TEST(concurrency_tools, allocator_32) {
	std::vector<int, aligned_allocator_32<int>> tv;
	for (int i = 0; i < 8; ++i)
		tv.push_back(i);

	EXPECT_EQ(8ui64, tv.size());
	EXPECT_EQ(0, tv[0]);
	EXPECT_EQ(7, tv[7]);

	EXPECT_EQ(0ui64, (size_t)(tv.data()) & 31);

	for (int i = 8; i < 128; ++i)
		tv.push_back(i);

	EXPECT_EQ(128ui64, tv.size());
	EXPECT_EQ(0ui64, (size_t)(tv.data()) & 31);

	EXPECT_EQ(0, tv[0]);
	EXPECT_EQ(7, tv[7]);
	EXPECT_EQ(71, tv[71]);
	EXPECT_EQ(127, tv[127]);

	tv.clear();
	tv.shrink_to_fit();

	EXPECT_EQ(0ui64, tv.size());
	EXPECT_EQ(0ui64, tv.capacity());
	EXPECT_EQ(0ui64, (size_t)(tv.data()) & 31);
}

TEST(concurrency_tools, fxd_deque) {
	fixed_sz_deque<int, 64, 64> tv;


	for (int i = 0; i < 127; ++i)
		tv.emplace(i);

	const auto li = tv.emplace(127);

	EXPECT_EQ(127ui32, li.id);

	EXPECT_EQ(120, tv.at(120));
	EXPECT_EQ(0, tv.at(0));
	EXPECT_EQ(35, tv.at(35));
	EXPECT_NE(nullptr, tv.safe_at(35));
	EXPECT_EQ(35, *tv.safe_at(35));
}

TEST(concurrency_tools, fxd_deque_growing) {
	fixed_sz_deque<int, 64, 64> tv;

	EXPECT_EQ(64ui32, tv.past_end());

	for (int i = 0; i < 64; ++i)
		tv.emplace(i);

	EXPECT_EQ(64ui32, tv.past_end());

	for (int i = 64; i < 128; ++i)
		tv.emplace(i);

	EXPECT_EQ(128ui32, tv.past_end());

	tv.free(26);
	tv.free(0);
	tv.free(120);
	tv.free(5);

	EXPECT_EQ(128ui32, tv.past_end());
	EXPECT_EQ(nullptr, tv.safe_at(0));
	EXPECT_EQ(nullptr, tv.safe_at(120));

	tv.emplace(0);
	tv.emplace(0);
	tv.emplace(0);
	tv.emplace(0);

	EXPECT_EQ(128ui32, tv.past_end());
	EXPECT_NE(nullptr, tv.safe_at(0));
	EXPECT_NE(nullptr, tv.safe_at(120));

	tv.emplace(0);
	EXPECT_EQ(64ui32 *3ui32, tv.past_end());
}

TEST(concurrency_tools, fxd_deque_iterator) {
	fixed_sz_deque<int, 64, 64> tv;

	EXPECT_EQ(64ui32, tv.past_end());

	for (int i = 0; i < 128; ++i)
		tv.emplace(i);

	int32_t sz = 0;

	for (const auto e : tv) {
		if(e) ++sz;
	}

	EXPECT_EQ(64 * 2, sz);

	tv.free(26);
	tv.free(0);
	tv.free(120);
	tv.free(5);

	sz = 0;
	for (const auto e : tv) {
		if (e) ++sz;
	}

	EXPECT_EQ(64 * 2 - 4, sz);

	tv.emplace(0);
	tv.emplace(0);
	tv.emplace(0);
	tv.emplace(0);

	sz = 0;
	for (const auto e : tv) {
		if (e) ++sz;
	}

	EXPECT_EQ(64 * 2, sz);
}

TEST(concurrency_tools, fxd_deque_iterator_b) {
	fixed_sz_deque<int, 64, 64> tv;

	tv.emplace(1);
	tv.emplace(2);
	tv.emplace(3);

	int32_t total = 0;
	for (const auto e : tv) {
		if (e)
			total += *e;
	}

	EXPECT_EQ(6, total);

	tv.free(1);

	total = 0;
	for (const auto e : tv) {
		if (e)
			total += *e;
	}

	EXPECT_EQ(4, total);
}

TEST(concurrency_tools, fxd_list) {
	fixed_sz_list<int, 16, 64> tv;

	int32_t expected_size = 0;
	for (int32_t i = 1; i < 30; ++i) {
		tv.emplace(i);
		expected_size += i;
	}

	int32_t popped_value = 0;
	tv.try_pop([&popped_value](int i) { popped_value = i; });

	EXPECT_NE(0, popped_value);

	expected_size -= popped_value;

	tv.emplace(30);
	expected_size += 30;

	int32_t popped_size = 0;
	tv.flush([&popped_size](int i) { popped_size += i; });

	EXPECT_EQ(expected_size, popped_size);

	popped_size = 0;
	tv.flush([&popped_size](int i) { popped_size += i; });

	EXPECT_EQ(0, popped_size);

	bool ran = false;
	tv.try_pop([&ran](int) { ran = true; });
	EXPECT_FALSE(ran);
}

TEST(common_tests, unique_vector_backed_string) {
	std::vector<char> vec;
	auto a = vector_backed_string<char>(std::string("test_a"), vec);
	auto b = vector_backed_string<char>(std::string("test_b"), vec);
	auto c = vector_backed_string<char>(std::string("test_c"), vec);

	const auto pre_size = vec.size();
	auto d = vector_backed_string<char>::create_unique(std::string("test_b"), vec);

	EXPECT_EQ(b, d);
	EXPECT_EQ(pre_size, vec.size());
}

TEST(common_tests, tagged_fixed_2dvector_test) {
	tagged_fixed_2dvector<double, uint32_t, uint16_t> tv;

	tv.reset(7);
	EXPECT_EQ(7ui32, tv.inner_size());
	EXPECT_EQ(0ui64, tv.outer_size());
	EXPECT_EQ(0ui64, tv.size());

	EXPECT_EQ(0.0, tv.safe_get(0, 5));

	EXPECT_EQ(7ui32, tv.inner_size());
	EXPECT_EQ(1ui64, tv.outer_size());
	EXPECT_EQ(7ui64, tv.size());

	tv.get(0, 5) = 3.0;
	EXPECT_EQ(3.0, tv.get(0, 5));

	EXPECT_EQ(0.0, tv.safe_get(0, 0));

	EXPECT_EQ(0.0, tv.safe_get(1, 6));

	EXPECT_EQ(7ui32, tv.inner_size());
	EXPECT_EQ(2ui64, tv.outer_size());
	EXPECT_EQ(14ui64, tv.size());
}

TEST(common_tests, tagged_fixed_blocked_2dvector_test) {
	tagged_fixed_blocked_2dvector<double, uint32_t, uint16_t, aligned_allocator_32<double>> tv;

	tv.reset(5);
	EXPECT_EQ(8ui32, tv.inner_size());
	EXPECT_EQ(0ui64, tv.outer_size());
	EXPECT_EQ(0ui64, tv.size());

	EXPECT_EQ(0.0, tv.safe_get(0, 4));

	EXPECT_EQ(8ui32, tv.inner_size());
	EXPECT_EQ(1ui64, tv.outer_size());
	EXPECT_EQ(1ui64, tv.size());

	EXPECT_NE(nullptr, tv.get_row(0).data());
	EXPECT_EQ(0ui64, (size_t)(tv.get_row(0).data()) & 31ui64);

	tv.get(0, 4) = 3.0;
	EXPECT_EQ(3.0, tv.get(0, 4));

	EXPECT_EQ(0.0, tv.safe_get(0, 0));

	EXPECT_EQ(0.0, tv.safe_get(1, 2));

	EXPECT_EQ(8ui32, tv.inner_size());
	EXPECT_EQ(2ui64, tv.outer_size());
	EXPECT_EQ(2ui64, tv.size());

	tv.get(1, 2) = 1.5;
	EXPECT_EQ(1.5, tv.get(1, 2));

	EXPECT_NE(nullptr, tv.get_row(1).data());
	EXPECT_NE(tv.get_row(0).data(), tv.get_row(1).data());
	EXPECT_EQ(0ui64, (size_t)(tv.get_row(1).data()) & 31ui64);
}

TEST(concurrency_tools, rt_log2) {
	EXPECT_EQ(ct_log2(0), rt_log2(0));
	EXPECT_EQ(ct_log2(1), rt_log2(1));
	EXPECT_EQ(ct_log2(2), rt_log2(2));
	EXPECT_EQ(ct_log2(3), rt_log2(3));
	EXPECT_EQ(ct_log2(4), rt_log2(4));
	EXPECT_EQ(ct_log2(300), rt_log2(300));

	EXPECT_EQ(ct_log2(0), 0ui32);
	EXPECT_EQ(ct_log2(1), 0ui32);
	EXPECT_EQ(ct_log2(2), 1ui32);
	EXPECT_EQ(ct_log2(3), 1ui32);
	EXPECT_EQ(ct_log2(4), 2ui32);

	EXPECT_EQ(ct_log2_round_up(0), rt_log2_round_up(0));
	EXPECT_EQ(ct_log2_round_up(1), rt_log2_round_up(1));
	EXPECT_EQ(ct_log2_round_up(2), rt_log2_round_up(2));
	EXPECT_EQ(ct_log2_round_up(3), rt_log2_round_up(3));
	EXPECT_EQ(ct_log2_round_up(4), rt_log2_round_up(4));
	EXPECT_EQ(ct_log2_round_up(300), rt_log2_round_up(300));

	EXPECT_EQ(ct_log2_round_up(0), 0ui32);
	EXPECT_EQ(ct_log2_round_up(1), 0ui32);
	EXPECT_EQ(ct_log2_round_up(2), 1ui32);
	EXPECT_EQ(ct_log2_round_up(3), 2ui32);
	EXPECT_EQ(ct_log2_round_up(4), 2ui32);
}

struct test_f_struct {
	uint32_t id = null_value_of<uint32_t>;
	float base_f = 0.0f;
};

TEST(concurrency_tools, test_stable_vector) {
	stable_vector<test_f_struct, uint32_t, 16, 4> test_vec;

	EXPECT_EQ(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ(false, test_vec.is_valid_index(0));
	EXPECT_EQ(false, test_vec.is_valid_index(8));
	EXPECT_EQ(false, test_vec.is_valid_index(16));

	auto& fobj = test_vec.get_new();
	EXPECT_EQ(0ui32, fobj.id);
	EXPECT_EQ(0.0f, fobj.base_f);

	fobj.base_f = 1.0f;

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ((test_vec.index_array[0])[0].id, 0ui32);
	EXPECT_EQ(test_vec.get_id((test_vec.index_array[0])[1]), null_value_of<uint32_t>);

	EXPECT_EQ(1.0f, test_vec.get(0ui32).base_f);

	EXPECT_EQ(true, test_vec.is_valid_index(0));
	EXPECT_EQ(true, test_vec.is_valid_index(8));
	EXPECT_EQ(false, test_vec.is_valid_index(16));

	for(int32_t i = 0; i < 15; ++i) {
		test_vec.get_new();
	}

	EXPECT_EQ(true, test_vec.is_valid_index(0));
	EXPECT_EQ(true, test_vec.is_valid_index(8));
	EXPECT_EQ(false, test_vec.is_valid_index(16));

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ((test_vec.index_array[0])[0].id, 0ui32);
	EXPECT_EQ((test_vec.index_array[0])[1].id, 1ui32);
	EXPECT_EQ((test_vec.index_array[0])[15].id, 15ui32);

	test_vec.remove(1);
	EXPECT_EQ(test_vec.get_id((test_vec.index_array[0])[1]), null_value_of<uint32_t>);

	auto& fobj2 = test_vec.get_new();

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(fobj2.id, 1ui32);

	auto& fobj3 = test_vec.get_new();
	fobj3.base_f = 3.0f;

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ(true, test_vec.is_valid_index(0));
	EXPECT_EQ(true, test_vec.is_valid_index(8));
	EXPECT_EQ(true, test_vec.is_valid_index(16));
	EXPECT_EQ(false, test_vec.is_valid_index(32));

	EXPECT_EQ(16ui32, fobj3.id);
	EXPECT_EQ(3.0f, test_vec.get(fobj3.id).base_f);

	EXPECT_EQ(test_vec.get_id((test_vec.index_array[1])[1]), null_value_of<uint32_t>);
	EXPECT_EQ(test_vec.get_id(((test_vec.index_array[1])[0])), 16ui32);

	auto& fobj4 = test_vec.get_new();
	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(17ui32, fobj4.id);

	EXPECT_EQ((test_vec.index_array[1])[1].id, 17ui32);
	EXPECT_EQ((test_vec.index_array[1])[0].id, 16ui32);

	for(int32_t i = 0; i < 14; ++i) {
		test_vec.get_new();
	}

	auto& fobj5 = test_vec.get_new();
	EXPECT_EQ(32ui32, fobj5.id);

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_NE(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ((test_vec.index_array[2])[0].id, 32ui32);
	EXPECT_EQ(test_vec.get_id((test_vec.index_array[2])[1]), null_value_of<uint32_t>);
}

struct test_f_struct_b {
	provinces::province_tag id;
	float base_f = 0.0f;
};

TEST(concurrency_tools, test_stable_vector_with_tag) {
	stable_vector<test_f_struct_b, provinces::province_tag, 16, 4> test_vec;

	EXPECT_EQ(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ(false, test_vec.is_valid_index(provinces::province_tag(0ui16)));
	EXPECT_EQ(false, test_vec.is_valid_index(provinces::province_tag(8ui16)));
	EXPECT_EQ(false, test_vec.is_valid_index(provinces::province_tag(16ui16)));

	auto& fobj = test_vec.get_new();
	EXPECT_EQ(provinces::province_tag(0ui16), fobj.id);
	EXPECT_EQ(0.0f, fobj.base_f);

	fobj.base_f = 1.0f;

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ((test_vec.index_array[0])[0].id, provinces::province_tag(0ui16));
	EXPECT_EQ(test_vec.get_id((test_vec.index_array[0])[1]), provinces::province_tag());

	EXPECT_EQ(1.0f, test_vec.get(provinces::province_tag(0ui16)).base_f);

	EXPECT_EQ(true, test_vec.is_valid_index(provinces::province_tag(0ui16)));
	EXPECT_EQ(true, test_vec.is_valid_index(provinces::province_tag(8ui16)));
	EXPECT_EQ(false, test_vec.is_valid_index(provinces::province_tag(16ui16)));

	for(int32_t i = 0; i < 15; ++i) {
		test_vec.get_new();
	}

	EXPECT_EQ(true, test_vec.is_valid_index(provinces::province_tag(0ui16)));
	EXPECT_EQ(true, test_vec.is_valid_index(provinces::province_tag(8ui16)));
	EXPECT_EQ(false, test_vec.is_valid_index(provinces::province_tag(16ui16)));

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ((test_vec.index_array[0])[0].id, provinces::province_tag(0ui16));
	EXPECT_EQ((test_vec.index_array[0])[1].id, provinces::province_tag(1ui16));
	EXPECT_EQ((test_vec.index_array[0])[15].id, provinces::province_tag(15ui16));

	test_vec.remove(provinces::province_tag(1ui16));
	EXPECT_EQ(test_vec.get_id((test_vec.index_array[0])[1]), provinces::province_tag());

	auto& fobj2 = test_vec.get_new();

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(fobj2.id, provinces::province_tag(1ui16));

	auto& fobj3 = test_vec.get_new();
	fobj3.base_f = 3.0f;

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ(true, test_vec.is_valid_index(provinces::province_tag(0ui16)));
	EXPECT_EQ(true, test_vec.is_valid_index(provinces::province_tag(8ui16)));
	EXPECT_EQ(true, test_vec.is_valid_index(provinces::province_tag(16ui16)));
	EXPECT_EQ(false, test_vec.is_valid_index(provinces::province_tag(32ui16)));

	EXPECT_EQ(provinces::province_tag(16ui16), fobj3.id);
	EXPECT_EQ(3.0f, test_vec.get(fobj3.id).base_f);

	EXPECT_EQ(test_vec.get_id((test_vec.index_array[1])[1]), provinces::province_tag());
	EXPECT_EQ(test_vec.get_id(((test_vec.index_array[1])[0])), provinces::province_tag(16ui16));

	auto& fobj4 = test_vec.get_new();
	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(provinces::province_tag(17ui16), fobj4.id);

	EXPECT_EQ((test_vec.index_array[1])[1].id, provinces::province_tag(17ui16));
	EXPECT_EQ((test_vec.index_array[1])[0].id, provinces::province_tag(16ui16));

	for(int32_t i = 0; i < 14; ++i) {
		test_vec.get_new();
	}

	auto& fobj5 = test_vec.get_new();
	EXPECT_EQ(provinces::province_tag(32ui16), fobj5.id);

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_NE(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ((test_vec.index_array[2])[0].id, provinces::province_tag(32ui16));
	EXPECT_EQ(test_vec.get_id((test_vec.index_array[2])[1]), provinces::province_tag());
}

TEST(concurrency_tools, test_stable_2d_vector) {
	stable_2d_vector<float, uint32_t, int16_t, 8, 4> test_vec;
	test_vec.reset(5);

	EXPECT_EQ(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(0ui32, test_vec.indices_in_use);
	EXPECT_EQ(5ui32, test_vec.inner_size);

	test_vec.ensure_capacity(2);

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(1ui32, test_vec.indices_in_use);
	EXPECT_EQ(5ui32, test_vec.inner_size);

	test_vec.get(1, 0) = 1.0f;
	test_vec.get(1, 1) = 2.0f;
	test_vec.get(1, 2) = 3.0f;
	test_vec.get(1, 3) = 4.0f;
	test_vec.get(1, 4) = 5.0f;

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(1ui32, test_vec.indices_in_use);
	EXPECT_EQ(5ui32, test_vec.inner_size);

	EXPECT_EQ(1.0f, (test_vec.index_array[0])[5]);
	EXPECT_EQ(2.0f, (test_vec.index_array[0])[6]);
	EXPECT_EQ(3.0f, (test_vec.index_array[0])[7]);
	EXPECT_EQ(4.0f, (test_vec.index_array[0])[8]);
	EXPECT_EQ(5.0f, (test_vec.index_array[0])[9]);

	test_vec.ensure_capacity(8);
	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(1ui32, test_vec.indices_in_use);
	EXPECT_EQ(5ui32, test_vec.inner_size);

	test_vec.ensure_capacity(9);
	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(2ui32, test_vec.indices_in_use);
	EXPECT_EQ(5ui32, test_vec.inner_size);

	test_vec.get(8, 1) = 1.5f;
	EXPECT_EQ(1.5f, (test_vec.index_array[1])[1]);

	EXPECT_EQ(0.0f, test_vec.safe_get(7, 4));

	const auto row = test_vec.get_row(1);
	EXPECT_EQ(1.0f, row[0]);
	EXPECT_EQ(2.0f, row[1]);
	EXPECT_EQ(3.0f, row[2]);
	EXPECT_EQ(4.0f, row[3]);
	EXPECT_EQ(5.0f, row[4]);

	const auto rowb = test_vec.get_row(8);
	EXPECT_EQ(1.5f, rowb[1]);
}

TEST(concurrency_tools, stable_variable_vector_storage_basic_test) {
	stable_variable_vector_storage_mk_2<float, 4, 1024> test_vec;

	EXPECT_EQ(0ui32, test_vec.first_free);

	auto new_small = test_vec.make_new(1);
	EXPECT_EQ(0ui32, new_small);
	EXPECT_EQ(4ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(0ui32, get_size(test_vec, new_small));

	EXPECT_EQ(3ui32, test_vec.first_free);

	EXPECT_EQ(null_value_of<decltype(new_small)>, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);

	auto new_small_range = get_range(test_vec, new_small);
	EXPECT_EQ(new_small_range.first, new_small_range.second);
	EXPECT_NE(nullptr, new_small_range.first);

	push_back(test_vec, new_small, 1.0f);

	EXPECT_EQ(0ui32, new_small);
	EXPECT_EQ(4ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(1ui32, get_size(test_vec, new_small));

	push_back(test_vec, new_small, 2.0f);
	push_back(test_vec, new_small, 3.0f);
	push_back(test_vec, new_small, 4.0f);

	EXPECT_EQ(0ui32, new_small);
	EXPECT_EQ(4ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(4ui32, get_size(test_vec, new_small));

	auto new_small_range_b = get_range(test_vec, new_small);
	EXPECT_NE(new_small_range_b.first, new_small_range_b.second);
	EXPECT_EQ(ptrdiff_t(4), new_small_range_b.second - new_small_range_b.first);
	EXPECT_EQ(1.0f, *(new_small_range_b.first));
	EXPECT_EQ(2.0f, *(new_small_range_b.first + 1));
	EXPECT_EQ(3.0f, *(new_small_range_b.first + 2));
	EXPECT_EQ(4.0f, *(new_small_range_b.first + 3));

	EXPECT_EQ(null_value_of<decltype(new_small)>, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);

	push_back(test_vec, new_small, 5.0f);

	EXPECT_EQ(3ui32, new_small);
	EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(5ui32, get_size(test_vec, new_small));

	EXPECT_EQ(8ui32, test_vec.first_free);
	EXPECT_EQ(0ui32, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);

	auto new_small_range_c = get_range(test_vec, new_small);
	EXPECT_EQ(ptrdiff_t(5), new_small_range_c.second - new_small_range_c.first);

	EXPECT_EQ(1.0f, *(new_small_range_c.first));
	EXPECT_EQ(2.0f, *(new_small_range_c.first + 1));
	EXPECT_EQ(3.0f, *(new_small_range_c.first + 2));
	EXPECT_EQ(4.0f, *(new_small_range_c.first + 3));
	EXPECT_EQ(5.0f, *(new_small_range_c.first + 4));
}

TEST(concurrency_tools, stable_variable_vector_storage_free_list) {
	stable_variable_vector_storage_mk_2<float, 4, 1024> test_vec;

	auto new_small = test_vec.make_new(4);
	EXPECT_EQ(0ui32, new_small);
	EXPECT_EQ(4ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(0ui32, get_size(test_vec, new_small));
	EXPECT_EQ(3ui32, test_vec.first_free);

	auto new_small_b = test_vec.make_new(4);
	EXPECT_EQ(3ui32, new_small_b);
	EXPECT_EQ(6ui32, test_vec.first_free);

	EXPECT_EQ(null_value_of<decltype(new_small)>, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);

	test_vec.release(new_small_b);
	EXPECT_EQ(null_value_of<decltype(new_small)>, new_small_b);
	EXPECT_EQ(3ui32, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);

	test_vec.release(new_small);
	EXPECT_EQ(null_value_of<decltype(new_small)>, new_small);
	EXPECT_EQ(0ui32, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);

	auto new_small_c = test_vec.make_new(1);
	EXPECT_EQ(0ui32, new_small_c);
	EXPECT_EQ(6ui32, test_vec.first_free);
	EXPECT_EQ(3ui32, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);

	auto new_small_d = test_vec.make_new(1);
	EXPECT_EQ(3ui32, new_small_d);
	EXPECT_EQ(6ui32, test_vec.first_free);
	EXPECT_EQ(null_value_of<decltype(new_small)>, concurrent_key_pair_helper(test_vec.free_lists[2]).parts.index);
}

TEST(concurrency_tools, stable_variable_vector_storage_unsorted_interface) {
	stable_variable_vector_storage_mk_2<float, 4, 1024> test_vec;

	auto new_small = test_vec.make_new(7);
	EXPECT_EQ(0ui32, new_small);
	EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(0ui32, get_size(test_vec, new_small));
	EXPECT_EQ(5ui32, test_vec.first_free);

	push_back(test_vec, new_small, 1.0f);

	EXPECT_EQ(1ui32, get_size(test_vec, new_small));
	EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(1.0f, get(test_vec, new_small, 0));

	push_back(test_vec, new_small, 2.0f);
	EXPECT_EQ(2ui32, get_size(test_vec, new_small));
	EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
	EXPECT_EQ(2.0f, get(test_vec, new_small, 1));

	pop_back(test_vec, new_small);
	EXPECT_EQ(1ui32, get_size(test_vec, new_small));
	EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(1.0f, get(test_vec, new_small, 0));

	float test_vals[] = { 4.0f, 8.0f, 16.0f, 32.0f, 1.0f, 2.0f, 3.0f, 4.0f };

	add_unordered_range(test_vec, new_small, test_vals, test_vals + 8);
	EXPECT_EQ(5ui32, new_small);
	EXPECT_EQ(14ui32, test_vec.first_free);
	EXPECT_EQ(0ui32, concurrent_key_pair_helper(test_vec.free_lists[3]).parts.index);

	EXPECT_EQ(9ui32, get_size(test_vec, new_small));
	EXPECT_EQ(16ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
	EXPECT_EQ(4.0f, get(test_vec, new_small, 1));
	EXPECT_EQ(8.0f, get(test_vec, new_small, 2));
	EXPECT_EQ(4.0f, get(test_vec, new_small, 8));

	remove_unsorted_item(test_vec, new_small, 8.0f);

	EXPECT_EQ(8ui32, get_size(test_vec, new_small));
	EXPECT_EQ(16ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
	EXPECT_EQ(4.0f, get(test_vec, new_small, 1));
	EXPECT_EQ(4.0f, get(test_vec, new_small, 2));
	EXPECT_EQ(3.0f, get(test_vec, new_small, 7));

	remove_unsorted_item(test_vec, new_small, 7.0f);

	EXPECT_EQ(8ui32, get_size(test_vec, new_small));
	EXPECT_EQ(16ui32, get_capacity(test_vec, new_small));
	EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
	EXPECT_EQ(4.0f, get(test_vec, new_small, 1));
	EXPECT_EQ(4.0f, get(test_vec, new_small, 2));
	EXPECT_EQ(3.0f, get(test_vec, new_small, 7));
}

TEST(concurrency_tools, stable_variable_vector_storage_sorted_interface) {
	stable_variable_vector_storage_mk_2<float, 4, 1024> test_vec;

	{
		//multimap
		auto new_small = test_vec.make_new(7);
		EXPECT_EQ(0ui32, new_small);
		EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
		EXPECT_EQ(0ui32, get_size(test_vec, new_small));
		EXPECT_EQ(5ui32, test_vec.first_free);

		add_item(test_vec, new_small, 3.0f);
		add_item(test_vec, new_small, 1.0f);
		add_item(test_vec, new_small, 4.5f);
		add_item(test_vec, new_small, 3.0f);

		EXPECT_EQ(0ui32, new_small);
		EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
		EXPECT_EQ(4ui32, get_size(test_vec, new_small));
		EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 1));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 2));
		EXPECT_EQ(4.5f, get(test_vec, new_small, 3));

		float test_vals[] = { 1.0f, 3.0f, 5.0f, 7.0f, 10.5f };

		add_ordered_range(test_vec, new_small, test_vals, test_vals + 5);

		EXPECT_EQ(5ui32, new_small);
		EXPECT_EQ(16ui32, get_capacity(test_vec, new_small));
		EXPECT_EQ(9ui32, get_size(test_vec, new_small));

		EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
		EXPECT_EQ(1.0f, get(test_vec, new_small, 1));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 2));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 3));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 4));
		EXPECT_EQ(4.5f, get(test_vec, new_small, 5));
		EXPECT_EQ(5.0f, get(test_vec, new_small, 6));
		EXPECT_EQ(7.0f, get(test_vec, new_small, 7));
		EXPECT_EQ(10.5f, get(test_vec, new_small, 8));

		EXPECT_EQ(true, contains_item(test_vec, new_small, 3.0f));
		EXPECT_EQ(true, contains_item(test_vec, new_small, 1.0f));
		EXPECT_EQ(true, contains_item(test_vec, new_small, 10.5f));
		EXPECT_EQ(false, contains_item(test_vec, new_small, 0.0f));
		EXPECT_EQ(false, contains_item(test_vec, new_small, 20.0f));
		EXPECT_EQ(false, contains_item(test_vec, new_small, 6.0f));

		remove_sorted_item(test_vec, new_small, 3.0f);

		EXPECT_EQ(8ui32, get_size(test_vec, new_small));

		EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
		EXPECT_EQ(1.0f, get(test_vec, new_small, 1));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 2));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 3));
		EXPECT_EQ(4.5f, get(test_vec, new_small, 4));
		EXPECT_EQ(5.0f, get(test_vec, new_small, 5));
		EXPECT_EQ(7.0f, get(test_vec, new_small, 6));
		EXPECT_EQ(10.5f, get(test_vec, new_small, 7));

		remove_sorted_item(test_vec, new_small, 10.5f);

		EXPECT_EQ(7ui32, get_size(test_vec, new_small));

		EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
		EXPECT_EQ(1.0f, get(test_vec, new_small, 1));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 2));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 3));
		EXPECT_EQ(4.5f, get(test_vec, new_small, 4));
		EXPECT_EQ(5.0f, get(test_vec, new_small, 5));
		EXPECT_EQ(7.0f, get(test_vec, new_small, 6));

		remove_sorted_item(test_vec, new_small, 1.0f);
		remove_sorted_item(test_vec, new_small, 1.0f);

		EXPECT_EQ(5ui32, get_size(test_vec, new_small));

		EXPECT_EQ(3.0f, get(test_vec, new_small, 0));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 1));
		EXPECT_EQ(4.5f, get(test_vec, new_small, 2));
		EXPECT_EQ(5.0f, get(test_vec, new_small, 3));
		EXPECT_EQ(7.0f, get(test_vec, new_small, 4));

		EXPECT_EQ(true, contains_item(test_vec, new_small, 3.0f));
		EXPECT_EQ(false, contains_item(test_vec, new_small, 1.0f));
		EXPECT_EQ(false, contains_item(test_vec, new_small, 10.5f));

		test_vec.release(new_small);
	}

	{
		auto new_small = test_vec.make_new(7);
		EXPECT_EQ(0ui32, new_small);
		EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
		EXPECT_EQ(0ui32, get_size(test_vec, new_small));

		add_unique_item(test_vec, new_small, 3.0f);
		add_unique_item(test_vec, new_small, 1.0f);
		add_unique_item(test_vec, new_small, 4.5f);
		add_unique_item(test_vec, new_small, 3.0f);

		EXPECT_EQ(0ui32, new_small);
		EXPECT_EQ(8ui32, get_capacity(test_vec, new_small));
		EXPECT_EQ(3ui32, get_size(test_vec, new_small));
		EXPECT_EQ(1.0f, get(test_vec, new_small, 0));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 1));
		EXPECT_EQ(4.5f, get(test_vec, new_small, 2));

		float test_vals[] = { 0.0f, 1.0f, 3.0f, 5.0f, 7.0f, 10.5f, 20.0f, 100.0f };

		add_unique_ordered_range(test_vec, new_small, test_vals, test_vals + 8);

		EXPECT_EQ(16ui32, get_capacity(test_vec, new_small));
		EXPECT_EQ(9ui32, get_size(test_vec, new_small));

		EXPECT_EQ(0.0f, get(test_vec, new_small, 0));
		EXPECT_EQ(1.0f, get(test_vec, new_small, 1));
		EXPECT_EQ(3.0f, get(test_vec, new_small, 2));
		EXPECT_EQ(4.5f, get(test_vec, new_small, 3));
		EXPECT_EQ(5.0f, get(test_vec, new_small, 4));
		EXPECT_EQ(7.0f, get(test_vec, new_small, 5));
		EXPECT_EQ(10.5f, get(test_vec, new_small, 6));
		EXPECT_EQ(20.0f, get(test_vec, new_small, 7));
		EXPECT_EQ(100.0f, get(test_vec, new_small, 8));
	}
}

TEST(concurrency_tools, set_tags_interface) {
	stable_variable_vector_storage_mk_2<int, 4, 1024> test_vec;
	set_tag<int> set_a;

	EXPECT_EQ(0ui32, get_capacity(test_vec, set_a));
	EXPECT_EQ(0ui32, get_size(test_vec, set_a));
	EXPECT_EQ(nullptr, get_range(test_vec, set_a).first);

	add_item(test_vec, set_a, 1);
	add_item(test_vec, set_a, 1);

	EXPECT_EQ(1ui32, get_size(test_vec, set_a));
	EXPECT_EQ(1, get(test_vec, set_a, 0));
	EXPECT_TRUE(contains_item(test_vec, set_a, 1));
	EXPECT_EQ(1, *find(test_vec, set_a, 1));

	resize(test_vec, set_a, 0ui32);

	EXPECT_NE(0ui32, get_capacity(test_vec, set_a));
	EXPECT_EQ(0ui32, get_size(test_vec, set_a));
	EXPECT_EQ(get_range(test_vec, set_a).second, get_range(test_vec, set_a).first);
	EXPECT_FALSE(contains_item(test_vec, set_a, 1));
	EXPECT_EQ(nullptr, find(test_vec, set_a, 1));

	add_item(test_vec, set_a, 1);
	add_item(test_vec, set_a, 5);

	EXPECT_EQ(2ui32, get_size(test_vec, set_a));
	EXPECT_EQ(1, get(test_vec, set_a, 0));
	EXPECT_EQ(5, get(test_vec, set_a, 1));
	EXPECT_EQ(5, *find(test_vec, set_a, 5));

	add_item(test_vec, set_a, 7);

	EXPECT_EQ(3ui32, get_size(test_vec, set_a));

	remove_item(test_vec, set_a, 5);

	EXPECT_EQ(2ui32, get_size(test_vec, set_a));
	EXPECT_EQ(1, get(test_vec, set_a, 0));
	EXPECT_EQ(7, get(test_vec, set_a, 1));

	add_item(test_vec, set_a, 10);

	remove_item_if(test_vec, set_a, [](int i) { return i < 5; });

	EXPECT_EQ(2ui32, get_size(test_vec, set_a));
	EXPECT_EQ(7, get(test_vec, set_a, 0));
	EXPECT_EQ(10, get(test_vec, set_a, 1));

	int vals[2] = { 8, 9 };
	add_range(test_vec, set_a, vals, vals + 2);

	EXPECT_EQ(4ui32, get_size(test_vec, set_a));
	EXPECT_EQ(7, get(test_vec, set_a, 0));
	EXPECT_EQ(8, get(test_vec, set_a, 1));
	EXPECT_EQ(9, get(test_vec, set_a, 2));
	EXPECT_EQ(10, get(test_vec, set_a, 3));
}

TEST(concurrency_tools, array_tags_interface) {
	stable_variable_vector_storage_mk_2<int, 4, 1024> test_vec;
	array_tag<int, int, false> array_a;

	EXPECT_EQ(0ui32, get_capacity(test_vec, array_a));
	EXPECT_EQ(0ui32, get_size(test_vec, array_a));
	EXPECT_EQ(nullptr, get_range(test_vec, array_a).data());

	add_item(test_vec, array_a, 3);
	add_item(test_vec, array_a, 5);
	add_item(test_vec, array_a, 6);

	EXPECT_EQ(3ui32, get_size(test_vec, array_a));
	EXPECT_EQ(3, get(test_vec, array_a, 0));
	EXPECT_EQ(5, get(test_vec, array_a, 1));
	EXPECT_EQ(6, get(test_vec, array_a, 2));

	remove_item(test_vec, array_a, 3);

	EXPECT_EQ(2ui32, get_size(test_vec, array_a));
	EXPECT_EQ(6, get(test_vec, array_a, 0));
	EXPECT_EQ(5, get(test_vec, array_a, 1));

	int vals[4] = { 8, 9, 1, 3 };
	add_range(test_vec, array_a, vals, vals + 4);

	EXPECT_EQ(6ui32, get_size(test_vec, array_a));

	EXPECT_EQ(6, get(test_vec, array_a, 0));
	EXPECT_EQ(5, get(test_vec, array_a, 1));
	EXPECT_EQ(1, get(test_vec, array_a, 4));
	EXPECT_EQ(3, get(test_vec, array_a, 5));

	EXPECT_TRUE(contains_item(test_vec, array_a, 1));
	EXPECT_FALSE(contains_item(test_vec, array_a, 7));

	remove_item_if(test_vec, array_a, [](int i) { return i % 2 == 1; });

	EXPECT_EQ(2ui32, get_size(test_vec, array_a));
	EXPECT_EQ(6, get(test_vec, array_a, 0));
	EXPECT_EQ(8, get(test_vec, array_a, 1));

	resize(test_vec, array_a, 4);
	EXPECT_EQ(4ui32, get_size(test_vec, array_a));
	EXPECT_EQ(6, get(test_vec, array_a, 0));
	EXPECT_EQ(8, get(test_vec, array_a, 1));
	EXPECT_EQ(0, get(test_vec, array_a, 2));
	EXPECT_EQ(0, get(test_vec, array_a, 3));

	EXPECT_EQ(8, *find(test_vec, array_a, 8));

	clear(test_vec, array_a);
	EXPECT_EQ(0ui32, get_size(test_vec, array_a));
}

struct if_pair {
	int i = 0;
	float f = 0.0f;

	constexpr bool operator<(if_pair const& other) const noexcept { return i < other.i; }
	constexpr bool operator==(if_pair const& other) const noexcept { return i == other.i && f == other.f; }
};

TEST(concurrency_tools, multiset_tags_interface) {
	stable_variable_vector_storage_mk_2<if_pair, 4, 1024> test_vec;
	multiset_tag<if_pair> set_a;

	EXPECT_EQ(0ui32, get_capacity(test_vec, set_a));
	EXPECT_EQ(0ui32, get_size(test_vec, set_a));
	EXPECT_EQ(nullptr, get_range(test_vec, set_a).first);

	add_item(test_vec, set_a, if_pair{ 1, 0.5f });
	add_item(test_vec, set_a, if_pair{ 0, 1.5f });
	add_item(test_vec, set_a, if_pair{ 1, 2.5f });
	add_item(test_vec, set_a, if_pair{ 3, 3.0f });

	EXPECT_EQ(4ui32, get_size(test_vec, set_a));

	auto sr = get_subrange(test_vec, set_a, if_pair{1, 0.0f});

	EXPECT_EQ(2, sr.second - sr.first);
	EXPECT_TRUE(sr.first->f == 0.5f || (sr.first + 1)->f == 0.5f);
	EXPECT_TRUE(sr.first->f == 2.5f || (sr.first + 1)->f == 2.5f);
	EXPECT_TRUE(sr.first->i == 1 && (sr.first + 1)->i == 1);

	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 1, 0.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 0, 0.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 3, 0.0f }));
	EXPECT_FALSE(contains_item(test_vec, set_a, if_pair{ 2, 0.0f }));

	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 1, 0.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 1, 0.5f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 1, 2.5f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 3.0f }));
	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 3, 0.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 0, 1.5f }));
	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 0, 0.0f }));

	remove_subrange(test_vec, set_a, if_pair{ 1, 0.0f });

	EXPECT_EQ(2ui32, get_size(test_vec, set_a));

	EXPECT_FALSE(contains_item(test_vec, set_a, if_pair{ 1, 0.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 0, 0.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 3, 0.0f }));
	EXPECT_FALSE(contains_item(test_vec, set_a, if_pair{ 2, 0.0f }));

	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 1, 0.5f }));
	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 1, 2.5f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 3.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 0, 1.5f }));

	add_item(test_vec, set_a, if_pair{ 3, 3.0f });
	add_item(test_vec, set_a, if_pair{ 3, 5.0f });
	add_item(test_vec, set_a, if_pair{ 1, 0.0f });
	add_item(test_vec, set_a, if_pair{ 10, 10.0f });

	EXPECT_EQ(6ui32, get_size(test_vec, set_a));

	auto sr_a = get_subrange(test_vec, set_a, if_pair{ 3, 0.0f });

	EXPECT_EQ(3, sr_a.second - sr_a.first);
	EXPECT_EQ(1.5f, find(test_vec, set_a, if_pair{ 0, 0.0f })->f);

	remove_item(test_vec, set_a, if_pair{ 3, 3.0f });

	EXPECT_EQ(4ui32, get_size(test_vec, set_a));
	EXPECT_EQ(5.0f, find(test_vec, set_a, if_pair{ 3, 0.0f })->f);

	auto sr_b = get_subrange(test_vec, set_a, if_pair{ 3, 0.0f });
	EXPECT_EQ(1, sr_b.second - sr_b.first);

	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 1, 0.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 0, 0.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 3, 0.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 10, 0.0f }));

	add_item(test_vec, set_a, if_pair{ 3, 3.0f });
	add_item(test_vec, set_a, if_pair{ 3, 3.0f });

	EXPECT_EQ(6ui32, get_size(test_vec, set_a));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 3.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 5.0f }));

	remove_single_item(test_vec, set_a, if_pair{ 3, 3.0f });

	EXPECT_EQ(5ui32, get_size(test_vec, set_a));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 3.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 5.0f }));
	EXPECT_TRUE(contains_item(test_vec, set_a, if_pair{ 10, 0.0f }));

	resize(test_vec, set_a, 0ui32);
	EXPECT_EQ(0ui32, get_size(test_vec, set_a));

	add_item(test_vec, set_a, if_pair{ 3, 1.0f });
	add_item(test_vec, set_a, if_pair{ 10, 2.0f });

	if_pair rng[] = { if_pair{1, 0.0f}, if_pair{7, 1.0f}, if_pair{7, 2.0f} };

	add_range(test_vec, set_a, rng, rng + 3);
	EXPECT_EQ(5ui32, get_size(test_vec, set_a));

	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 1.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 10, 2.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 1, 0.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 7, 1.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 7, 2.0f }));

	remove_item_if(test_vec, set_a, [](if_pair p) { return p.f == 0.0f; });

	EXPECT_EQ(4ui32, get_size(test_vec, set_a));

	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 1.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 10, 2.0f }));
	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 1, 0.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 7, 1.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 7, 2.0f }));

	remove_subitem_if(test_vec, set_a, if_pair{ 7, 0.0f }, [](if_pair p) { return p.f == 1.0f; });

	EXPECT_EQ(3ui32, get_size(test_vec, set_a));

	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 3, 1.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 10, 2.0f }));
	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 1, 0.0f }));
	EXPECT_FALSE(contains_subitem(test_vec, set_a, if_pair{ 7, 1.0f }));
	EXPECT_TRUE(contains_subitem(test_vec, set_a, if_pair{ 7, 2.0f }));
}

TEST(concurrency_tools, minimum_index_int32_t) {
	int32_t values_a[] = {1, 6, 2, 7, 9, 11, 5, 7, -1};
	EXPECT_EQ(minimum_index(values_a, 9), 8);
	EXPECT_EQ(minimum_index(values_a, 8), 0);
	EXPECT_EQ(minimum_index(values_a, 7), 0);
	EXPECT_EQ(minimum_index(values_a, 6), 0);
	EXPECT_EQ(minimum_index(values_a, 5), 0);
	EXPECT_EQ(minimum_index(values_a, 4), 0);
	EXPECT_EQ(minimum_index(values_a, 3), 0);
	EXPECT_EQ(minimum_index(values_a, 2), 0);
	EXPECT_EQ(minimum_index(values_a, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 1, 8), 7);
	EXPECT_EQ(minimum_index(values_a + 1, 7), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 6), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 5), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 4), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 3), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 2), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 2, 7), 6);
	EXPECT_EQ(minimum_index(values_a + 2, 6), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 5), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 4), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 3, 6), 5);
	EXPECT_EQ(minimum_index(values_a + 3, 5), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 4), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 1), 0);

	int32_t values_b[] = { 1, 6, 2, 7, 9, 11, 5, -1, 45, 29, 19, 12, -1, 23, 1, 0, 6 };
	EXPECT_EQ(values_b[minimum_index(values_b, 17)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 16)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 15)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 14)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 13)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 12)], -1);

	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 16)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 15)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 14)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 13)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 12)), -1);
}

TEST(concurrency_tools, maximum_index_int32_t) {
	int32_t values_a[] = { 1, 6, 2, 7, 9, 11, 5, 7, -1 };
	EXPECT_EQ(maximum_index(values_a, 9), 5);
	EXPECT_EQ(maximum_index(values_a, 8), 5);
	EXPECT_EQ(maximum_index(values_a, 7), 5);
	EXPECT_EQ(maximum_index(values_a, 6), 5);
	EXPECT_EQ(maximum_index(values_a, 5), 4);
	EXPECT_EQ(maximum_index(values_a, 4), 3);
	EXPECT_EQ(maximum_index(values_a, 3), 1);
	EXPECT_EQ(maximum_index(values_a, 2), 1);
	EXPECT_EQ(maximum_index(values_a, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 1, 8), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 7), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 6), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 5), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 1, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 1, 2), 0);
	EXPECT_EQ(maximum_index(values_a + 1, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 2, 7), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 6), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 5), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 2, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 2, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 3, 6), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 5), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 4), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 3, 1), 0);

	int32_t values_b[] = { 45, 6, 2, 7, 9, 11, 5, -1, 45, 29, 19, 12, -1, 23, 1, 0, 6 };
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(maximum_index(values_b, 8), 0);

	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 16)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 15)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 14)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 13)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 12)), 45);
	EXPECT_EQ(maximum_index(values_b + 1, 8), 7);
	EXPECT_EQ(maximum_index(values_b + 1, 7), 4);
}

TEST(concurrency_tools, minimum_index_int64_t) {
	int64_t values_a[] = { 1, 6, 2, 7, 9, 11, 5, 7, -1 };
	EXPECT_EQ(minimum_index(values_a, 9), 8);
	EXPECT_EQ(minimum_index(values_a, 8), 0);
	EXPECT_EQ(minimum_index(values_a, 7), 0);
	EXPECT_EQ(minimum_index(values_a, 6), 0);
	EXPECT_EQ(minimum_index(values_a, 5), 0);
	EXPECT_EQ(minimum_index(values_a, 4), 0);
	EXPECT_EQ(minimum_index(values_a, 3), 0);
	EXPECT_EQ(minimum_index(values_a, 2), 0);
	EXPECT_EQ(minimum_index(values_a, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 1, 8), 7);
	EXPECT_EQ(minimum_index(values_a + 1, 7), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 6), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 5), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 4), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 3), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 2), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 2, 7), 6);
	EXPECT_EQ(minimum_index(values_a + 2, 6), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 5), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 4), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 3, 6), 5);
	EXPECT_EQ(minimum_index(values_a + 3, 5), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 4), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 1), 0);

	int64_t values_b[] = { 1, 6, 2, 7, 9, 11, 5, -1, 45, 29, 19, 12, -1, 23, 1, 0, 6 };
	EXPECT_EQ(values_b[minimum_index(values_b, 17)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 16)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 15)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 14)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 13)], -1);
	EXPECT_EQ(values_b[minimum_index(values_b, 12)], -1);

	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 16)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 15)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 14)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 13)), -1);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 12)), -1);
}

TEST(concurrency_tools, maximum_index_int64_t) {
	int64_t values_a[] = { 1, 6, 2, 7, 9, 11, 5, 7, -1 };
	EXPECT_EQ(maximum_index(values_a, 9), 5);
	EXPECT_EQ(maximum_index(values_a, 8), 5);
	EXPECT_EQ(maximum_index(values_a, 7), 5);
	EXPECT_EQ(maximum_index(values_a, 6), 5);
	EXPECT_EQ(maximum_index(values_a, 5), 4);
	EXPECT_EQ(maximum_index(values_a, 4), 3);
	EXPECT_EQ(maximum_index(values_a, 3), 1);
	EXPECT_EQ(maximum_index(values_a, 2), 1);
	EXPECT_EQ(maximum_index(values_a, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 1, 8), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 7), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 6), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 5), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 1, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 1, 2), 0);
	EXPECT_EQ(maximum_index(values_a + 1, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 2, 7), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 6), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 5), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 2, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 2, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 3, 6), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 5), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 4), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 3, 1), 0);

	int64_t values_b[] = { 45, 6, 2, 7, 9, 11, 5, -1, 45, 29, 19, 12, -1, 23, 1, 0, 6 };
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45);
	EXPECT_EQ(maximum_index(values_b, 8), 0);

	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 16)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 15)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 14)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 13)), 45);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 12)), 45);
	EXPECT_EQ(maximum_index(values_b + 1, 8), 7);
	EXPECT_EQ(maximum_index(values_b + 1, 7), 4);
}

TEST(concurrency_tools, minimum_index_float) {
	float values_a[] = { 1.1f, 6.0f, 2.5f, 7.0f, 9.0f, 11.1f, 5.5f, 7.1f, -1.0f };
	EXPECT_EQ(minimum_index(values_a, 9), 8);
	EXPECT_EQ(minimum_index(values_a, 8), 0);
	EXPECT_EQ(minimum_index(values_a, 7), 0);
	EXPECT_EQ(minimum_index(values_a, 6), 0);
	EXPECT_EQ(minimum_index(values_a, 5), 0);
	EXPECT_EQ(minimum_index(values_a, 4), 0);
	EXPECT_EQ(minimum_index(values_a, 3), 0);
	EXPECT_EQ(minimum_index(values_a, 2), 0);
	EXPECT_EQ(minimum_index(values_a, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 1, 8), 7);
	EXPECT_EQ(minimum_index(values_a + 1, 7), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 6), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 5), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 4), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 3), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 2), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 2, 7), 6);
	EXPECT_EQ(minimum_index(values_a + 2, 6), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 5), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 4), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 3, 6), 5);
	EXPECT_EQ(minimum_index(values_a + 3, 5), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 4), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 1), 0);

	float values_b[] = { 1.0f, 6.0f, 2.0f, 7.0f, 9.0f, 11.0f, 5.0f, -1.0f, 45.0f, 29.0f, 19.0f, 12.0f, -1.0f, 23.0f, 1.0f, 0.0f, 6.0f };
	EXPECT_EQ(values_b[minimum_index(values_b, 17)], -1.0f);
	EXPECT_EQ(values_b[minimum_index(values_b, 16)], -1.0f);
	EXPECT_EQ(values_b[minimum_index(values_b, 15)], -1.0f);
	EXPECT_EQ(values_b[minimum_index(values_b, 14)], -1.0f);
	EXPECT_EQ(values_b[minimum_index(values_b, 13)], -1.0f);
	EXPECT_EQ(values_b[minimum_index(values_b, 12)], -1.0f);

	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 16)), -1.0f);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 15)), -1.0f);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 14)), -1.0f);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 13)), -1.0f);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 12)), -1.0f);
}

TEST(concurrency_tools, maximum_index_float) {
	float values_a[] = { 1.1f, 6.3f, 2.2f, 7.0f, 9.3f, 11.25f, 5.2f, 7.0f, -1.1f };
	EXPECT_EQ(maximum_index(values_a, 9), 5);
	EXPECT_EQ(maximum_index(values_a, 8), 5);
	EXPECT_EQ(maximum_index(values_a, 7), 5);
	EXPECT_EQ(maximum_index(values_a, 6), 5);
	EXPECT_EQ(maximum_index(values_a, 5), 4);
	EXPECT_EQ(maximum_index(values_a, 4), 3);
	EXPECT_EQ(maximum_index(values_a, 3), 1);
	EXPECT_EQ(maximum_index(values_a, 2), 1);
	EXPECT_EQ(maximum_index(values_a, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 1, 8), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 7), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 6), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 5), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 1, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 1, 2), 0);
	EXPECT_EQ(maximum_index(values_a + 1, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 2, 7), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 6), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 5), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 2, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 2, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 3, 6), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 5), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 4), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 3, 1), 0);

	float values_b[] = { 45.0f, 6.0f, 2.0f, 7.0f, 9.0f, 11.0f, 5.0f, -1.0f, 45.0f, 29.0f, 19.0f, 12.0f, -1.0f, 23.0f, 1.0f, 0.0f, 6.0f };
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0f);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0f);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0f);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0f);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0f);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0f);
	EXPECT_EQ(maximum_index(values_b, 8), 0);

	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 16)), 45.0f);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 15)), 45.0f);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 14)), 45.0f);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 13)), 45.0f);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 12)), 45.0f);
	EXPECT_EQ(maximum_index(values_b + 1, 8), 7);
	EXPECT_EQ(maximum_index(values_b + 1, 7), 4);
}

TEST(concurrency_tools, minimum_index_double) {
	double values_a[] = { 1.1, 6.0, 2.5, 7.0, 9.0, 11.1, 5.5, 7.1, -1.0 };
	EXPECT_EQ(minimum_index(values_a, 9), 8);
	EXPECT_EQ(minimum_index(values_a, 8), 0);
	EXPECT_EQ(minimum_index(values_a, 7), 0);
	EXPECT_EQ(minimum_index(values_a, 6), 0);
	EXPECT_EQ(minimum_index(values_a, 5), 0);
	EXPECT_EQ(minimum_index(values_a, 4), 0);
	EXPECT_EQ(minimum_index(values_a, 3), 0);
	EXPECT_EQ(minimum_index(values_a, 2), 0);
	EXPECT_EQ(minimum_index(values_a, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 1, 8), 7);
	EXPECT_EQ(minimum_index(values_a + 1, 7), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 6), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 5), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 4), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 3), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 2), 1);
	EXPECT_EQ(minimum_index(values_a + 1, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 2, 7), 6);
	EXPECT_EQ(minimum_index(values_a + 2, 6), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 5), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 4), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 2, 1), 0);

	EXPECT_EQ(minimum_index(values_a + 3, 6), 5);
	EXPECT_EQ(minimum_index(values_a + 3, 5), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 4), 3);
	EXPECT_EQ(minimum_index(values_a + 3, 3), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 2), 0);
	EXPECT_EQ(minimum_index(values_a + 3, 1), 0);

	double values_b[] = { 1.0, 6.0, 2.0, 7.0, 9.0, 11.0, 5.0, -1.0, 45.0, 29.0, 19.0, 12.0, -1.0, 23.0, 1.0, 0.0, 6.0 };
	EXPECT_EQ(values_b[minimum_index(values_b, 17)], -1.0);
	EXPECT_EQ(values_b[minimum_index(values_b, 16)], -1.0);
	EXPECT_EQ(values_b[minimum_index(values_b, 15)], -1.0);
	EXPECT_EQ(values_b[minimum_index(values_b, 14)], -1.0);
	EXPECT_EQ(values_b[minimum_index(values_b, 13)], -1.0);
	EXPECT_EQ(values_b[minimum_index(values_b, 12)], -1.0);

	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 16)), -1.0);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 15)), -1.0);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 14)), -1.0);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 13)), -1.0);
	EXPECT_EQ(*(values_b + 1 + minimum_index(values_b + 1, 12)), -1.0);
}

TEST(concurrency_tools, maximum_index_double) {
	double values_a[] = { 1.1, 6.3, 2.2, 7.0, 9.3, 11.25, 5.2, 7.0, -1.1 };
	EXPECT_EQ(maximum_index(values_a, 9), 5);
	EXPECT_EQ(maximum_index(values_a, 8), 5);
	EXPECT_EQ(maximum_index(values_a, 7), 5);
	EXPECT_EQ(maximum_index(values_a, 6), 5);
	EXPECT_EQ(maximum_index(values_a, 5), 4);
	EXPECT_EQ(maximum_index(values_a, 4), 3);
	EXPECT_EQ(maximum_index(values_a, 3), 1);
	EXPECT_EQ(maximum_index(values_a, 2), 1);
	EXPECT_EQ(maximum_index(values_a, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 1, 8), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 7), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 6), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 5), 4);
	EXPECT_EQ(maximum_index(values_a + 1, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 1, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 1, 2), 0);
	EXPECT_EQ(maximum_index(values_a + 1, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 2, 7), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 6), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 5), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 4), 3);
	EXPECT_EQ(maximum_index(values_a + 2, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 2, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 2, 1), 0);

	EXPECT_EQ(maximum_index(values_a + 3, 6), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 5), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 4), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 3), 2);
	EXPECT_EQ(maximum_index(values_a + 3, 2), 1);
	EXPECT_EQ(maximum_index(values_a + 3, 1), 0);

	double values_b[] = { 45.0, 6.0, 2.0, 7.0, 9.0, 11.0, 5.0, -1.0, 45.0, 29.0, 19.0, 12.0, -1.0, 23.0, 1.0, 0.0, 6.0 };
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0);
	EXPECT_EQ(values_b[maximum_index(values_b, 17)], 45.0);
	EXPECT_EQ(maximum_index(values_b, 8), 0);

	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 16)), 45.0);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 15)), 45.0);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 14)), 45.0);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 13)), 45.0);
	EXPECT_EQ(*(values_b + 1 + maximum_index(values_b + 1, 12)), 45.0);
	EXPECT_EQ(maximum_index(values_b + 1, 8), 7);
	EXPECT_EQ(maximum_index(values_b + 1, 7), 4);
}

namespace labels {
	struct a;
	struct b;
	struct c;
	struct d;
}

template<typename T>
class serialization::tagged_serializer<labels::c, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	template<typename ... CONTEXT>
	static void serialize_object(std::byte* &output, T const& obj, CONTEXT&& ... c) {
		serialize(output, obj);
		int val = -1;
		serialize(output, val);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, T& obj, CONTEXT&& ... c) {
		deserialize(input, obj);
		int val = 0;
		deserialize(input, val);
		obj *= T(val);
	}
	template<typename ... CONTEXT>
	static size_t size(T const& obj, CONTEXT&& ...) {
		return serialize_size(obj) + sizeof(int);
	}
};
TEST(concurrency_tools, variable_layout_basic) {
	variable_layout_tagged_vector<provinces::province_tag, 64, labels::a, int, labels::b, float, labels::d, bitfield_type, labels::c, std::pair<int, int>> test_vec;
	EXPECT_EQ(0, test_vec.size());

	auto va = test_vec.get_new();
	EXPECT_EQ(provinces::province_tag(0), va);
	EXPECT_EQ(1, test_vec.size());
	EXPECT_EQ(0, test_vec.get<labels::a>(va));
	EXPECT_EQ(0.0f, test_vec.get<labels::b>(va));

	test_vec.get<labels::a>(va) = 10;
	test_vec.get<labels::b>(va) = 2.5f;
	test_vec.get<labels::c>(va) = std::pair<int, int>(1,7);

	EXPECT_EQ(10, test_vec.get<labels::a>(va));
	EXPECT_EQ(2.5f, test_vec.get<labels::b>(va));
	auto pv = std::pair<int, int>(1, 7);
	EXPECT_EQ(pv, test_vec.get<labels::c>(va));
	EXPECT_EQ(false, test_vec.get<labels::d>(va));

	auto vb = test_vec.get_new();
	EXPECT_EQ(provinces::province_tag(1), vb);
	EXPECT_EQ(2, test_vec.size());

	test_vec.release(va);
	EXPECT_EQ(2, test_vec.size());
	EXPECT_NE(provinces::province_tag(0), test_vec.get<index_type_marker>(provinces::province_tag(0)));


	auto vc = test_vec.get_new();
	EXPECT_EQ(provinces::province_tag(0), vc);
	EXPECT_EQ(2, test_vec.size());

	test_vec.release(vc);
	EXPECT_EQ(2, test_vec.size());

	test_vec.release(vb);
	EXPECT_EQ(0, test_vec.size());

	test_vec.set<labels::a>(va, 12);
	test_vec.set<labels::b>(va, 7.5f);
	test_vec.set<labels::c>(va, std::pair<int, int>(-1, -7));
	test_vec.set<labels::d>(va, true);

	EXPECT_EQ(12, test_vec.get<labels::a>(va));
	EXPECT_EQ(7.5f, test_vec.get<labels::b>(va));
	auto pvb = std::pair<int, int>(-1, -7);
	EXPECT_EQ(pvb, test_vec.get<labels::c>(va));
	EXPECT_EQ(true, test_vec.get<labels::d>(va));

	EXPECT_EQ(0, test_vec.get<labels::a>(provinces::province_tag(1)));
	EXPECT_EQ(0.0f, test_vec.get<labels::b>(provinces::province_tag(1)));
	auto pvc = std::pair<int, int>();
	EXPECT_EQ(pvc, test_vec.get<labels::c>(provinces::province_tag(1)));
	EXPECT_EQ(false, test_vec.get<labels::d>(provinces::province_tag(1)));

	test_vec.set<labels::d>(va, false);

	test_vec.set<labels::a>(provinces::province_tag(62), 122);
	test_vec.set<labels::b>(provinces::province_tag(62), 19.5f);
	test_vec.set<labels::c>(provinces::province_tag(62), std::pair<int, int>(-6, -1));
	test_vec.set<labels::d>(provinces::province_tag(62), true);

	EXPECT_EQ(12, test_vec.get<labels::a>(va));
	EXPECT_EQ(7.5f, test_vec.get<labels::b>(va));
	EXPECT_EQ(pvb, test_vec.get<labels::c>(va));
	EXPECT_EQ(false, test_vec.get<labels::d>(va));
}

TEST(concurrency_tools, variable_layout_serialize) {
	variable_layout_tagged_vector<provinces::province_tag, 64, labels::a, std::pair<int, int>, labels::b, float, labels::d, bitfield_type, labels::c, float> test_vec;
	auto a = test_vec.get_new();
	auto b = test_vec.get_new();
	auto c = test_vec.get_new();

	test_vec.release(b);

	EXPECT_EQ(3, test_vec.size());

	test_vec.get<labels::a>(a) = std::pair<int, int>(1, 7);
	test_vec.get<labels::b>(a) = 2.5f;
	test_vec.get<labels::c>(a) = 3.0f;
	test_vec.set<labels::d>(a, true);
	
	test_vec.get<labels::a>(c) = std::pair<int, int>(0, -9);
	test_vec.get<labels::b>(c) = 1.25f;
	test_vec.get<labels::c>(c) = 7.5f;
	test_vec.set<labels::d>(c, true);

	std::vector<std::byte> buffer(serialization::serialize_size(test_vec));
	std::byte* output = buffer.data();
	serialization::serialize(output, test_vec);

	std::byte const* input = buffer.data();
	variable_layout_tagged_vector<provinces::province_tag, 64, labels::a, std::pair<int, int>, labels::b, float, labels::d, bitfield_type, labels::c, float> test_vec_b;

	serialization::deserialize(input, test_vec_b);

	EXPECT_EQ(3, test_vec_b.size());

	auto pa = std::pair<int, int>(1, 7);
	EXPECT_EQ(pa, test_vec_b.get<labels::a>(a));
	EXPECT_EQ(2.5f, test_vec_b.get<labels::b>(a));
	EXPECT_EQ(-3.0f, test_vec_b.get<labels::c>(a));
	EXPECT_EQ(true, test_vec_b.get<labels::d>(a));

	auto pb = std::pair<int, int>(0, -9);
	EXPECT_EQ(pb, test_vec_b.get<labels::a>(c));
	EXPECT_EQ(1.25f, test_vec_b.get<labels::b>(c));
	EXPECT_EQ(-7.5f, test_vec_b.get<labels::c>(c));
	EXPECT_EQ(true, test_vec_b.get<labels::d>(c));

	auto pc = std::pair<int, int>();
	EXPECT_EQ(pc, test_vec_b.get<labels::a>(b));
	EXPECT_EQ(0.0f, test_vec_b.get<labels::b>(b));
	EXPECT_EQ(0.0f, test_vec_b.get<labels::c>(b));
	EXPECT_EQ(false, test_vec_b.get<labels::d>(b));

	EXPECT_EQ(b, test_vec_b.get_new());
}

TEST(concurrency_tools, ve_simple_math) {
	std::vector<float, aligned_allocator_64<float>> a(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> b(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> result(16, 0.0f);

	a[0] = 2.4f;
	a[1] = 2.7f;
	a[2] = 1.4f;
	a[3] = 0.4f;
	a[4] = 22.4f;
	a[5] = 201.4f;
	a[6] = 652.4f;
	a[7] = 28.4f;
	a[8] = 2.04f;
	a[9] = 2.84f;
	a[10] = 26.4f;
	a[11] = 52.4f;
	a[12] = 72.4f;
	a[13] = 29.4f;
	a[14] = 24.4f;
	a[15] = 25.4f;

	b[0] = 26.4f;
	b[1] = 122.7f;
	b[2] = 12.4f;
	b[3] = 5.4f;
	b[4] = 22.0f;
	b[5] = 21.4f;
	b[6] = 65.4f;
	b[7] = 8.4f;
	b[8] = 0.04f;
	b[9] = 1.84f;
	b[10] = 236.4f;
	b[11] = 5.4f;
	b[12] = 7.9f;
	b[13] = 9.8f;
	b[14] = 4.6f;
	b[15] = 5.1f;

	auto add_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) + ve::load(executor, b_vec));
	};

	auto sub_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) - ve::load(executor, b_vec));
	};

	auto mul_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) * ve::load(executor, b_vec));
	};

	auto div_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) / ve::load(executor, b_vec));
	};

	ve::execute_serial_fast<int32_t>(16, add_func);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] + b[i]);
	}

	ve::execute_serial_fast<int32_t>(16, sub_func);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] - b[i]);
	}

	ve::execute_serial_fast<int32_t>(16, mul_func);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] * b[i]);
	}

	ve::execute_serial_fast<int32_t>(16, div_func);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] / b[i]);
	}
}

TEST(concurrency_tools, ve_partial_load_and_store) {
	std::vector<float, aligned_allocator_64<float>> a(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> b(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> result(16, 0.0f);

	a[0] = 2.4f;
	a[1] = 2.7f;
	a[2] = 1.4f;
	a[3] = 0.4f;
	a[4] = 22.4f;
	a[5] = 201.4f;
	a[6] = 652.4f;
	a[7] = 28.4f;
	a[8] = 2.04f;
	a[9] = 2.84f;
	a[10] = 26.4f;
	a[11] = 52.4f;
	a[12] = 72.4f;
	a[13] = 29.4f;
	a[14] = 24.4f;
	a[15] = 25.4f;

	b[0] = 26.4f;
	b[1] = 122.7f;
	b[2] = 12.4f;
	b[3] = 5.4f;
	b[4] = 22.0f;
	b[5] = 21.4f;
	b[6] = 65.4f;
	b[7] = 8.4f;
	b[8] = 0.04f;
	b[9] = 1.84f;
	b[10] = 236.4f;
	b[11] = 5.4f;
	b[12] = 7.9f;
	b[13] = 9.8f;
	b[14] = 4.6f;
	b[15] = 5.1f;

	auto add_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) + ve::load(executor, b_vec));
	};

	auto sub_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) - ve::load(executor, b_vec));
	};

	auto mul_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) * ve::load(executor, b_vec));
	};

	auto div_func = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::load(executor, a_vec) / ve::load(executor, b_vec));
	};

	ve::execute_serial<int32_t>(15, add_func);

	for(uint32_t i = 0; i < 15; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] + b[i]);
	}
	EXPECT_EQ(result[15], 0.0f);

	ve::execute_serial<int32_t>(15, sub_func);

	for(uint32_t i = 0; i < 15; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] - b[i]);
	}
	EXPECT_EQ(result[15], 0.0f);

	ve::execute_serial<int32_t>(15, mul_func);

	for(uint32_t i = 0; i < 15; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] * b[i]);
	}
	EXPECT_EQ(result[15], 0.0f);

	ve::execute_serial<int32_t>(15, div_func);

	for(uint32_t i = 0; i < 15; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] / b[i]);
	}
	EXPECT_EQ(result[15], 0.0f);
}

TEST(concurrency_tools, ve_complex_math) {
	std::vector<float, aligned_allocator_64<float>> a(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> b(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> c(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> result(16, 0.0f);

	a[0] = 2.4f;
	a[1] = 2.7f;
	a[2] = 1.4f;
	a[3] = 0.4f;
	a[4] = 22.4f;
	a[5] = 201.4f;
	a[6] = 652.4f;
	a[7] = 28.4f;
	a[8] = 2.04f;
	a[9] = 2.84f;
	a[10] = 26.4f;
	a[11] = 52.4f;
	a[12] = 72.4f;
	a[13] = 29.4f;
	a[14] = 24.4f;
	a[15] = 25.4f;

	b[0] = 26.4f;
	b[1] = 122.7f;
	b[2] = 12.4f;
	b[3] = 5.4f;
	b[4] = 22.0f;
	b[5] = 21.4f;
	b[6] = 65.4f;
	b[7] = 8.4f;
	b[8] = 0.04f;
	b[9] = 1.84f;
	b[10] = 236.4f;
	b[11] = 5.4f;
	b[12] = 7.9f;
	b[13] = 9.8f;
	b[14] = 4.6f;
	b[15] = 5.1f;

	c[0] = 76.4f;
	c[1] = 232.7f;
	c[2] = 17.4f;
	c[3] = 5.94f;
	c[4] = 212.0f;
	c[5] = 1.4f;
	c[6] = 35.4f;
	c[7] = 88.4f;
	c[8] = 2.04f;
	c[9] = 1.684f;
	c[10] = 36.4f;
	c[11] = 53.4f;
	c[12] = 76.9f;
	c[13] = 93.8f;
	c[14] = 24.6f;
	c[15] = 35.1f;

	auto fma = [a_vec = a.data(), b_vec = b.data(), c_vec = c.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::multiply_and_add(ve::load(executor, a_vec), ve::load(executor, b_vec), ve::load(executor, c_vec)));
	};

	auto nfma = [a_vec = a.data(), b_vec = b.data(), c_vec = c.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::negate_multiply_and_subtract(ve::load(executor, a_vec), ve::load(executor, b_vec), ve::load(executor, c_vec)));
	};

	auto inv = [a_vec = a.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::improved_inverse(ve::load(executor, a_vec)));
	};

	auto sq = [a_vec = a.data(), r_vec = result.data()](auto executor) {
		ve::store(executor, r_vec, ve::sqrt(ve::load(executor, a_vec)));
	};

	ve::execute_serial_fast<int32_t>(16, fma);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] * b[i] + c[i]);
	}

	ve::execute_serial_fast<int32_t>(16, nfma);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], -(a[i] * b[i]) - c[i]);
	}

	ve::execute_serial_fast<int32_t>(16, inv);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], 1.0f / a[i]);
	}

	ve::execute_serial_fast<int32_t>(16, sq);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], std::sqrt(a[i]));
	}
}

TEST(concurrency_tools, ve_comparison_gather) {
	std::vector<float, aligned_allocator_64<float>> a(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> b(16, 0.0f);
	std::vector<int32_t, aligned_allocator_64<int32_t>> c(16, 0);
	std::vector<float, aligned_allocator_64<float>> result(16, 0.0f);

	a[0] = 2.4f;
	a[1] = 2.7f;
	a[2] = 1.4f;
	a[3] = 0.4f;
	a[4] = 22.4f;
	a[5] = 201.4f;
	a[6] = 652.4f;
	a[7] = 28.4f;
	a[8] = 2.04f;
	a[9] = 2.84f;
	a[10] = 26.4f;
	a[11] = 52.4f;
	a[12] = 72.4f;
	a[13] = 29.4f;
	a[14] = 24.4f;
	a[15] = 25.4f;

	b[0] = 26.4f;
	b[1] = 122.7f;
	b[2] = 12.4f;
	b[3] = 5.4f;
	b[4] = 22.0f;
	b[5] = 21.4f;
	b[6] = 65.4f;
	b[7] = 8.4f;
	b[8] = 0.04f;
	b[9] = 1.84f;
	b[10] = 236.4f;
	b[11] = 5.4f;
	b[12] = 7.9f;
	b[13] = 9.8f;
	b[14] = 4.6f;
	b[15] = 5.1f;

	c[0] = 0;
	c[1] = 1;
	c[2] = 3;
	c[3] = 2;
	c[4] = 1;
	c[5] = 5;
	c[6] = 10;
	c[7] = 0;
	c[8] = 15;
	c[9] = 5;
	c[10] = 9;
	c[11] = 11;
	c[12] = 10;
	c[13] = 3;
	c[14] = 4;
	c[15] = 7;

	auto gt_cmp = [a_vec = a.data(), b_vec = b.data(), r_vec = result.data()](auto executor) {
		auto av = ve::load(executor, a_vec);
		ve::store(executor, r_vec, ve::select(av < ve::load(executor, b_vec), av, 2.0f));
	};

	auto gather_a = [a_vec = a.data(), c_vec = c.data(), r_vec = result.data()](auto executor) {
		auto indices = ve::load(executor, c_vec);
		ve::store(executor, r_vec, ve::load(indices, a_vec));
	};

	auto gather_b = [a_vec = a.data(), b_vec = b.data(), c_vec = c.data(), r_vec = result.data()](auto executor) {
		auto indices = ve::load(executor, c_vec);
		ve::store(executor, r_vec, ve::select(ve::load(executor, b_vec) > 20.0f, ve::load(indices, a_vec), 3.0f));
	};

	auto cmp_b = [a_vec = a.data(), r_vec = result.data()](auto executor) {
		auto av = ve::load(executor, a_vec);
		ve::store(executor, r_vec, ve::select((av <= 40.0f) & (av >= 10.0f), 0.0f, av));
	};

	ve::execute_serial_fast<int32_t>(16, gt_cmp);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[i] < b[i] ? a[i] : 2.0f);
	}

	ve::execute_serial_fast<int32_t>(16, gather_a);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], a[c[i]]);
	}

	ve::execute_serial_fast<int32_t>(16, gather_b);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], b[i] > 20.0f ? a[c[i]] : 3.0f);
	}

	ve::execute_serial_fast<int32_t>(16, cmp_b);

	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(result[i], (a[i] <= 40.0f && a[i] >= 10.0f) ? 0.0f : a[i]);
	}

}

TEST(concurrency_tools, vector_reduce) {
	std::vector<float, aligned_allocator_64<float>> a(16, 0.0f);

	a[0] = 2.4f;
	a[1] = 2.7f;
	a[2] = 1.4f;
	a[3] = 0.4f;
	a[4] = 22.4f;
	a[5] = 201.4f;
	a[6] = 652.4f;
	a[7] = 28.4f;
	a[8] = 2.04f;
	a[9] = 2.84f;
	a[10] = 26.4f;
	a[11] = 52.4f;
	a[12] = 72.4f;
	a[13] = 29.4f;
	a[14] = 24.4f;
	a[15] = 25.4f;

	auto reduce_result = ve::reduce<int32_t, false>(tagged_array_view<float, int32_t, false>(a.data(), 15));
	auto std_reduce = std::reduce(a.data(), a.data() + 15, 0.0f, std::plus<>());
	EXPECT_FLOAT_EQ(reduce_result, std_reduce);
}

TEST(concurrency_tools, vector_integer_mask) {
	std::vector<float, aligned_allocator_64<float>> a(16, 0.0f);
	std::vector<float, aligned_allocator_64<float>> b(16, 0.0f);

	a[0] = 2.4f;
	a[1] = 2.7f;
	a[2] = 1.4f;
	a[3] = 0.4f;
	a[4] = 22.4f;
	a[5] = 201.4f;
	a[6] = 652.4f;
	a[7] = 28.4f;
	a[8] = 2.04f;
	a[9] = 2.84f;
	a[10] = 26.4f;
	a[11] = 52.4f;
	a[12] = 72.4f;
	a[13] = 29.4f;
	a[14] = 24.4f;
	a[15] = 25.4f;

	auto all_not = [a_vec = a.data(), b_vec = b.data()](auto executor) {
		ve::store(executor, b_vec, ve::select(ve::vbitfield_type{ ve::vbitfield_type::storage(0) }, ve::load(executor, a_vec), 5.0f));
	};
	auto all_true = [a_vec = a.data(), b_vec = b.data()](auto executor) {
		ve::store(executor, b_vec, ve::select(ve::vbitfield_type{ ve::vbitfield_type::storage(0xFFFF) }, ve::load(executor, a_vec), 5.0f));
	};
	auto second = [a_vec = a.data(), b_vec = b.data()](auto executor) {
		ve::store(executor, b_vec, ve::select(ve::vbitfield_type{ ve::vbitfield_type::storage(0x0202) }, ve::load(executor, a_vec), 5.0f));
	};

	ve::execute_serial_fast<int32_t>(16, all_not);
	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(b[i], 5.0f);
	}

	ve::execute_serial_fast<int32_t>(16, all_true);
	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(b[i], a[i]);
	}

	ve::execute_serial<int32_t>(16, second);
	for(uint32_t i = 0; i < 16; ++i) {
		EXPECT_FLOAT_EQ(b[i], i == 1 || i == 9 ? a[i] : 5.0f);
	}
}
