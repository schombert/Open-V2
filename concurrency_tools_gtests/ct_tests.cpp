#include "common\\common.h"
#include "gtest\\gtest.h"
#include "concurrency_tools\\concurrency_tools.hpp"

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
	EXPECT_EQ(2ui64, tv.size());

	EXPECT_NE(nullptr, tv.get_row(0));
	EXPECT_EQ(0ui64, (size_t)(tv.get_row(0)) & 31ui64);

	tv.get(0, 4) = 3.0;
	EXPECT_EQ(3.0, tv.get(0, 4));

	EXPECT_EQ(0.0, tv.safe_get(0, 0));

	EXPECT_EQ(0.0, tv.safe_get(1, 2));

	EXPECT_EQ(8ui32, tv.inner_size());
	EXPECT_EQ(2ui64, tv.outer_size());
	EXPECT_EQ(4ui64, tv.size());

	tv.get(1, 2) = 1.5;
	EXPECT_EQ(1.5, tv.get(1, 2));

	EXPECT_NE(nullptr, tv.get_row(1));
	EXPECT_NE(tv.get_row(0), tv.get_row(1));
	EXPECT_EQ(0ui64, (size_t)(tv.get_row(1)) & 31ui64);
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
	EXPECT_EQ((test_vec.index_array[0])[1].id, null_value_of<uint32_t>);

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
	EXPECT_EQ((test_vec.index_array[0])[1].id, null_value_of<uint32_t>);

	auto& fobj2 = test_vec.get_new();

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_EQ(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(fobj2.id, 1ui32);

	auto& fobj3 = test_vec.safe_get(17);
	fobj3.base_f = 3.0f;

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ(true, test_vec.is_valid_index(0));
	EXPECT_EQ(true, test_vec.is_valid_index(8));
	EXPECT_EQ(true, test_vec.is_valid_index(16));
	EXPECT_EQ(false, test_vec.is_valid_index(32));

	EXPECT_EQ(17ui32, fobj3.id);
	EXPECT_EQ(3.0f, test_vec.get(fobj3.id).base_f);

	EXPECT_EQ((test_vec.index_array[1])[0].id, null_value_of<uint32_t>);
	EXPECT_EQ((test_vec.index_array[1])[1].id, 17ui32);

	auto& fobj4 = test_vec.get_new();
	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);
	EXPECT_EQ(31ui32, fobj4.id);

	EXPECT_EQ((test_vec.index_array[1])[15].id, 31ui32);
	EXPECT_EQ((test_vec.index_array[1])[1].id, 17ui32);

	_aligned_free(test_vec.index_array[1]);
	test_vec.index_array[1] = nullptr;

	auto& fobj5 = test_vec.get_new();
	EXPECT_EQ(16ui32, fobj5.id);

	EXPECT_NE(nullptr, test_vec.index_array[0]);
	EXPECT_NE(nullptr, test_vec.index_array[1]);
	EXPECT_EQ(nullptr, test_vec.index_array[2]);
	EXPECT_EQ(nullptr, test_vec.index_array[3]);

	EXPECT_EQ((test_vec.index_array[1])[0].id, 16ui32);
	EXPECT_EQ((test_vec.index_array[1])[1].id, null_value_of<uint32_t>);
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

TEST(concurrency_tools, basic_level_stable_variable_vector_storage) {
	{
		stable_variable_vector_storage<float, 4, 8, 4> test_vec;
		test_vec.index_array[0] = (float*)_aligned_malloc(sizeof(float) * 4 * 8 + sizeof(detail::array_chunk) * 8, 64);

		detail::array_chunk* fblock = (detail::array_chunk*)(test_vec.index_array[0] + 4 * 8);

		*fblock = detail::array_chunk{ 0ui16, 0ui16, 0ui16, 1ui16 };

		detail::push_chunk_to_free_list(test_vec, 1ui16, fblock);

		EXPECT_EQ(uint16_t( 1ui16 | detail::array_chunk::is_free), fblock->size_and_use);
		EXPECT_EQ(1ui16, test_vec.free_lists[0]);

		*(fblock + 1) = detail::array_chunk{ 0ui16, 0ui16, 0ui16, 1ui16 };
		detail::push_chunk_to_free_list(test_vec, 2ui16, fblock + 1);

		EXPECT_EQ(uint16_t(1ui16 | detail::array_chunk::is_free), (fblock + 1)->size_and_use);
		EXPECT_EQ(2ui16, test_vec.free_lists[0]);
		EXPECT_EQ(2ui16, fblock->previous_in_free_list);
		EXPECT_EQ(1ui16, (fblock + 1)->next_in_free_list);

		*(fblock + 2) = detail::array_chunk{ 0ui16, 0ui16, 0ui16, 1ui16 };
		detail::push_chunk_to_free_list(test_vec, 3ui16, fblock + 2);

		detail::remove_chunk_from_free_list(test_vec, (fblock + 1));

		EXPECT_EQ(3ui16, test_vec.free_lists[0]);
		EXPECT_EQ(3ui16, fblock->previous_in_free_list);
		EXPECT_EQ(1ui16, (fblock + 2)->next_in_free_list);

		EXPECT_EQ(0ui16, (fblock + 1)->next_in_free_list);
		EXPECT_EQ(0ui16, (fblock + 1)->previous_in_free_list);
		EXPECT_EQ(1ui16, (fblock + 1)->size_and_use);

		detail::remove_chunk_from_free_list(test_vec, (fblock + 2));

		EXPECT_EQ(1ui16, test_vec.free_lists[0]);
		EXPECT_EQ(0ui16, fblock->previous_in_free_list);

		detail::push_chunk_to_free_list(test_vec, 2ui16, fblock + 1);

		EXPECT_EQ(uint16_t(1ui16 | detail::array_chunk::is_free), (fblock + 1)->size_and_use);
		EXPECT_EQ(2ui16, test_vec.free_lists[0]);
		EXPECT_EQ(2ui16, fblock->previous_in_free_list);
		EXPECT_EQ(1ui16, (fblock + 1)->next_in_free_list);

		detail::pop_chunk_from_free_list(test_vec, 0);

		EXPECT_EQ(1ui16, test_vec.free_lists[0]);
		EXPECT_EQ(0ui16, fblock->previous_in_free_list);
		EXPECT_EQ(1ui16, (fblock + 1)->size_and_use);

		detail::pop_chunk_from_free_list(test_vec, 0);
		EXPECT_EQ(0ui16, test_vec.free_lists[0]);
		EXPECT_EQ(1ui16, (fblock)->size_and_use);
	}

	{
		stable_variable_vector_storage<float, 4, 8, 4> test_vec;
		detail::make_new_backing_buffer(test_vec, 0, 1);

		EXPECT_NE(nullptr, test_vec.index_array[0]);
		EXPECT_EQ(0.0f, test_vec.index_array[0][0]);

		detail::array_chunk* fblock = (detail::array_chunk*)(test_vec.index_array[0] + 4 * 8);
		EXPECT_EQ(0ui16, fblock->local_left_chunk);
		EXPECT_EQ(0ui16, fblock->previous_in_free_list);
		EXPECT_EQ(0ui16, fblock->next_in_free_list);
		EXPECT_EQ(1ui16, fblock->size_and_use);
	}
}
