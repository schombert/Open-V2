#include "pch.h"
#include "concurrency_tools\\concurrency_tools.hpp"

TEST(string_construction, concurrency_tools) {
	concurrent_string a;
	concurrent_string b("small data");
	concurrent_string c(b);

	EXPECT_EQ(0, a.length());
	EXPECT_EQ(10, b.length());
	EXPECT_EQ(10, c.length());
	EXPECT_STREQ("", a.c_str());
	EXPECT_STREQ("small data", b.c_str());
	EXPECT_STREQ("small data", c.c_str());
}

TEST(string_small_move_add, concurrency_tools) {
	concurrent_string b("small data");
	concurrent_string c(std::move(b));

	EXPECT_EQ(10, c.length());
	EXPECT_STREQ("small data", c.c_str());

	c += "x";
	EXPECT_STREQ("small datax", c.c_str());

	EXPECT_EQ(11, c.length());
}

TEST(string_large_move_add, concurrency_tools) {
	concurrent_string b("__not small data__");
	concurrent_string c(std::move(b));

	EXPECT_EQ(0, b.length());
	EXPECT_EQ(18, c.length());
	EXPECT_STREQ("", b.c_str());
	EXPECT_STREQ("__not small data__", c.c_str());

	c += "x";
	EXPECT_STREQ("__not small data__x", c.c_str());
	EXPECT_EQ(19, c.length());
}

TEST(string_small_to_large, concurrency_tools) {
	concurrent_string b("small data");
	b += b;

	EXPECT_EQ(20, b.length());
	EXPECT_STREQ("small datasmall data", b.c_str());
}

TEST(limit_case_str, concurrency_tools) {
	concurrent_string b("012345678901234");

	EXPECT_EQ(15, b.length());
	EXPECT_STREQ("012345678901234", b.c_str());
}

TEST(clear_string, concurrency_tools) {
	concurrent_string b("012345678901234");
	concurrent_string c("012345678901234012345678901234");

	b.clear();
	c.clear();

	EXPECT_EQ(0, b.length());
	EXPECT_EQ(0, c.length());
	EXPECT_STREQ("", b.c_str());
	EXPECT_STREQ("", c.c_str());
}

TEST(allocator, concurrency_tools) {
	std::vector<int, concurrent_allocator<int>> tv;
	for (int i = 0; i < 8; ++i)
		tv.push_back(i);

	EXPECT_EQ(8, tv.size());
	EXPECT_EQ(0, tv[0]);
	EXPECT_EQ(7, tv[7]);

	for (int i = 8; i < 128; ++i)
		tv.push_back(i);

	EXPECT_EQ(128, tv.size());
	EXPECT_EQ(0, tv[0]);
	EXPECT_EQ(7, tv[7]);
	EXPECT_EQ(71, tv[71]);
	EXPECT_EQ(127, tv[127]);

	tv.clear();
	tv.shrink_to_fit();

	EXPECT_EQ(0, tv.size());
	EXPECT_EQ(0, tv.capacity());
}

TEST(fxd_deque, concurrency_tools) {
	fixed_sz_deque<int, 64, 64> tv;


	for (int i = 0; i < 127; ++i)
		tv.emplace_back(i);

	const auto li = tv.emplace_back(127);

	EXPECT_EQ(127, li);

	EXPECT_EQ(120, tv.at(120));
	EXPECT_EQ(0, tv.at(0));
	EXPECT_EQ(35, tv.at(35));
}

TEST(fxd_deque_growing, concurrency_tools) {
	fixed_sz_deque<int, 64, 64> tv;

	EXPECT_EQ(64, tv.past_end());

	for (int i = 0; i < 64; ++i)
		tv.emplace_back(i);

	EXPECT_EQ(64, tv.past_end());

	for (int i = 64; i < 128; ++i)
		tv.emplace_back(i);

	EXPECT_EQ(128, tv.past_end());

	tv.free(26);
	tv.free(0);
	tv.free(120);
	tv.free(5);

	EXPECT_EQ(128, tv.past_end());

	tv.emplace_back(0);
	tv.emplace_back(0);
	tv.emplace_back(0);
	tv.emplace_back(0);

	EXPECT_EQ(128, tv.past_end());

	tv.emplace_back(0);
	EXPECT_EQ(64*3, tv.past_end());
}

TEST(fxd_deque_iterator, concurrency_tools) {
	fixed_sz_deque<int, 64, 64> tv;

	EXPECT_EQ(64, tv.past_end());

	for (int i = 0; i < 128; ++i)
		tv.emplace_back(i);

	int32_t sz = 0;

	for (const auto& e : tv) {
		e.visit([&sz](int) { ++sz; });
	}

	EXPECT_EQ(64 * 2, sz);

	tv.free(26);
	tv.free(0);
	tv.free(120);
	tv.free(5);

	sz = 0;
	for (const auto& e : tv) {
		e.visit([&sz](int) { ++sz; });
	}

	EXPECT_EQ(64 * 2 - 4, sz);

	tv.emplace_back(0);
	tv.emplace_back(0);
	tv.emplace_back(0);
	tv.emplace_back(0);

	sz = 0;
	for (const auto& e : tv) {
		e.visit([&sz](int) { ++sz; });
	}

	EXPECT_EQ(64 * 2, sz);
}

TEST(fxd_deque_iterator_b, concurrency_tools) {
	fixed_sz_deque<int, 64, 64> tv;

	tv.emplace_back(1);
	tv.emplace_back(2);
	tv.emplace_back(3);

	int32_t total = 0;
	for (const auto& e : tv) {
		e.visit([&total](int i) { total += i; });
	}

	EXPECT_EQ(6, total);

	tv.free(1);

	total = 0;
	for (const auto& e : tv) {
		e.visit([&total](int i) { total += i; });
	}

	EXPECT_EQ(4, total);
}