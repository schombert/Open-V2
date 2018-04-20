#include "gtest\\gtest.h"
#include "triggers\\trigger_reading.h"
#include "triggers\\codes.h"

using namespace triggers;

TEST(trigger_reading, trigger_size) {
	std::vector<uint16_t> zero_t;
	std::vector<uint16_t> one_t;

	zero_t.push_back(uint16_t(codes::no_payload | codes::association_eq | codes::blockade));
	zero_t.push_back(7ui16);

	one_t.push_back(uint16_t(codes::is_scope | codes::administration_spending_pop));
	one_t.push_back(7ui16);

	EXPECT_EQ(0, get_payload_size(zero_t.data()));
	EXPECT_EQ(7, get_payload_size(one_t.data()));
}

TEST(trigger_reading, scope_recursion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::generic_scope));
		t.push_back(11ui16);
		t.push_back(uint16_t(codes::no_payload | codes::association_eq | codes::blockade));
		t.push_back(uint16_t(codes::is_scope | codes::state_scope_pop));
		t.push_back(5ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(codes::no_payload | codes::association_eq | codes::blockade));
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		t.push_back(0ui16);

		int32_t total = 0;
		recurse_over_triggers(t.data(), [&total](uint16_t*) { ++total; });
		EXPECT_EQ(6, total);

		int32_t no_payload_count = 0;
		recurse_over_triggers(t.data(), [&no_payload_count](uint16_t* v) { if (get_payload_size(v) == 0) ++no_payload_count; });
		EXPECT_EQ(2, no_payload_count);

		int32_t total_payload = 0;
		recurse_over_triggers(t.data(), [&total_payload](uint16_t* v) { total_payload += get_payload_size(v); });
		EXPECT_EQ(20, total_payload);
	}
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::generic_scope));
		t.push_back(12ui16);
		t.push_back(uint16_t(codes::no_payload | codes::association_eq | codes::blockade));
		t.push_back(uint16_t(codes::is_scope | codes::integer_scope));
		t.push_back(6ui16);
		t.push_back(100ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(codes::no_payload | codes::association_eq | codes::blockade));
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		t.push_back(0ui16);

		int32_t total = 0;
		recurse_over_triggers(t.data(), [&total](uint16_t*) { ++total; });
		EXPECT_EQ(6, total);

		int32_t no_payload_count = 0;
		recurse_over_triggers(t.data(), [&no_payload_count](uint16_t* v) { if (get_payload_size(v) == 0) ++no_payload_count; });
		EXPECT_EQ(2, no_payload_count);

		int32_t total_payload = 0;
		recurse_over_triggers(t.data(), [&total_payload](uint16_t* v) { total_payload += get_payload_size(v); });
		EXPECT_EQ(22, total_payload);
	}
}

TEST(trigger_reading, simple_inversion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::placeholder_not_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		invert_trigger(t.data());

		EXPECT_EQ(t[0], uint16_t(codes::is_scope | codes::is_disjunctive_scope | codes::placeholder_not_scope));
		EXPECT_EQ(t[1], 4ui16);
		EXPECT_EQ(t[2], uint16_t(codes::association_ne | codes::blockade));
		EXPECT_EQ(t[3], 2ui16);
		EXPECT_EQ(t[4], 1ui16);
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::is_existance_scope | codes::is_disjunctive_scope | codes::x_core_scope_nation));
		t.push_back(4ui16);
		t.push_back(uint16_t(codes::association_ge | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		invert_trigger(t.data());

		EXPECT_EQ(t[0], uint16_t(codes::is_scope | codes::x_core_scope_nation));
		EXPECT_EQ(t[1], 4ui16);
		EXPECT_EQ(t[2], uint16_t(codes::association_lt | codes::blockade));
		EXPECT_EQ(t[3], 2ui16);
		EXPECT_EQ(t[4], 1ui16);
	}
}

TEST(trigger_reading, scope_sizing) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::placeholder_not_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_TRUE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::placeholder_not_scope));
		t.push_back(5ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(codes::association_eq | codes::no_payload | codes::blockade));
		t.push_back(0ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_FALSE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::integer_scope));
		t.push_back(5ui16);
		t.push_back(0ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_TRUE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::integer_scope));
		t.push_back(6ui16);
		t.push_back(0ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(codes::association_eq | codes::no_payload | codes::blockade));
		t.push_back(0ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_FALSE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::placeholder_not_scope));
		t.push_back(1ui16);

		EXPECT_TRUE(scope_is_empty(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::integer_scope));
		t.push_back(2ui16);
		t.push_back(0ui16);

		EXPECT_TRUE(scope_is_empty(t.data()));
	}
}

TEST(trigger_reading, simple_negation_removal) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(codes::is_scope | codes::placeholder_not_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(codes::association_eq | codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		const auto new_size = simplify_trigger(t.data());

		EXPECT_EQ(3, new_size);
		EXPECT_EQ(t[0], uint16_t(codes::association_ne | codes::blockade));
		EXPECT_EQ(t[1], 2ui16);
		EXPECT_EQ(t[2], 1ui16);
	}
}
