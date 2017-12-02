#include "pch.h"
#include "concurrency_tools\\concurrency_tools.h"

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