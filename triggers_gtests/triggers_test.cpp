#include "gtest\\gtest.h"
#include "triggers\\trigger_reading.h"
#include "triggers\\codes.h"
#include "scenario\\scenario.h"
#include "issues\\issues.h"
#include "simple_fs\\simple_fs.h"
#include "fake_fs\\fake_fs.h"
#include "triggers\\effects.h"
#include "triggers\\effect_reading.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace triggers;

TEST(trigger_reading, trigger_size) {
	std::vector<uint16_t> zero_t;
	std::vector<uint16_t> one_t;

	zero_t.push_back(uint16_t(trigger_codes::no_payload | trigger_codes::association_eq | trigger_codes::blockade));
	zero_t.push_back(7ui16);

	one_t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::administration_spending_pop));
	one_t.push_back(7ui16);

	EXPECT_EQ(0, get_trigger_payload_size(zero_t.data()));
	EXPECT_EQ(7, get_trigger_payload_size(one_t.data()));
} 

TEST(trigger_reading, effect_size) {
	std::vector<uint16_t> zero_t;
	std::vector<uint16_t> one_t;

	zero_t.push_back(uint16_t(effect_codes::no_payload | effect_codes::activate_technology));
	zero_t.push_back(7ui16);

	one_t.push_back(uint16_t(effect_codes::is_scope | effect_codes::add_accepted_culture));
	one_t.push_back(7ui16);

	EXPECT_EQ(0, get_effect_payload_size(zero_t.data()));
	EXPECT_EQ(7, get_effect_payload_size(one_t.data()));
}

TEST(trigger_reading, scope_recursion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::generic_scope));
		t.push_back(11ui16);
		t.push_back(uint16_t(trigger_codes::no_payload | trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::state_scope_pop));
		t.push_back(5ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(trigger_codes::no_payload | trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		t.push_back(0ui16);

		int32_t total = 0;
		recurse_over_triggers(t.data(), [&total](uint16_t*) { ++total; });
		EXPECT_EQ(6, total);

		int32_t no_payload_count = 0;
		recurse_over_triggers(t.data(), [&no_payload_count](uint16_t* v) { if (get_trigger_payload_size(v) == 0) ++no_payload_count; });
		EXPECT_EQ(2, no_payload_count);

		int32_t total_payload = 0;
		recurse_over_triggers(t.data(), [&total_payload](uint16_t* v) { total_payload += get_trigger_payload_size(v); });
		EXPECT_EQ(20, total_payload);
	}
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::generic_scope));
		t.push_back(12ui16);
		t.push_back(uint16_t(trigger_codes::no_payload | trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::integer_scope));
		t.push_back(6ui16);
		t.push_back(100ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(trigger_codes::no_payload | trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		t.push_back(0ui16);

		int32_t total = 0;
		recurse_over_triggers(t.data(), [&total](uint16_t*) { ++total; });
		EXPECT_EQ(6, total);

		int32_t no_payload_count = 0;
		recurse_over_triggers(t.data(), [&no_payload_count](uint16_t* v) { if (get_trigger_payload_size(v) == 0) ++no_payload_count; });
		EXPECT_EQ(2, no_payload_count);

		int32_t total_payload = 0;
		recurse_over_triggers(t.data(), [&total_payload](uint16_t* v) { total_payload += get_trigger_payload_size(v); });
		EXPECT_EQ(22, total_payload);
	}
}

TEST(trigger_reading, effect_scope_recursion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::generic_scope));
		t.push_back(11ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_core_from_province));
		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::state_scope_pop));
		t.push_back(5ui16);
		t.push_back(uint16_t(effect_codes::add_core_from_province));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_war_goal));
		t.push_back(uint16_t(effect_codes::add_casus_belli_tag));
		t.push_back(2ui16);
		t.push_back(1ui16);

		t.push_back(0ui16);

		int32_t total = 0;
		recurse_over_effects(t.data(), [&total](uint16_t*) { ++total; });
		EXPECT_EQ(6, total);

		int32_t no_payload_count = 0;
		recurse_over_effects(t.data(), [&no_payload_count](uint16_t* v) { if (get_effect_payload_size(v) == 0) ++no_payload_count; });
		EXPECT_EQ(2, no_payload_count);

		int32_t total_payload = 0;
		recurse_over_effects(t.data(), [&total_payload](uint16_t* v) { total_payload += get_effect_payload_size(v); });
		EXPECT_EQ(20, total_payload);
	}
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::generic_scope));
		t.push_back(13ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::badboy));
		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::integer_scope));
		t.push_back(7ui16);
		t.push_back(trigger_payload(trigger_tag()).value);
		t.push_back(100ui16);
		t.push_back(uint16_t(effect_codes::define_general));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::secede_province_this_state));
		t.push_back(uint16_t(effect_codes::war_exhaustion));
		t.push_back(2ui16);
		t.push_back(1ui16);

		t.push_back(0ui16);

		int32_t total = 0;
		recurse_over_effects(t.data(), [&total](uint16_t*) { ++total; });
		EXPECT_EQ(6, total);

		int32_t no_payload_count = 0;
		recurse_over_effects(t.data(), [&no_payload_count](uint16_t* v) { if (get_effect_payload_size(v) == 0) ++no_payload_count; });
		EXPECT_EQ(2, no_payload_count);

		int32_t total_payload = 0;
		recurse_over_effects(t.data(), [&total_payload](uint16_t* v) { total_payload += get_effect_payload_size(v); });
		EXPECT_EQ(24, total_payload);
	}
}

TEST(trigger_reading, simple_inversion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		invert_trigger(t.data());

		EXPECT_EQ(t[0], uint16_t(trigger_codes::is_scope | trigger_codes::is_disjunctive_scope | trigger_codes::placeholder_not_scope));
		EXPECT_EQ(t[1], 4ui16);
		EXPECT_EQ(t[2], uint16_t(trigger_codes::association_ne | trigger_codes::blockade));
		EXPECT_EQ(t[3], 2ui16);
		EXPECT_EQ(t[4], 1ui16);
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::is_existance_scope | trigger_codes::is_disjunctive_scope | trigger_codes::x_core_scope_nation));
		t.push_back(4ui16);
		t.push_back(uint16_t(trigger_codes::association_ge | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		invert_trigger(t.data());

		EXPECT_EQ(t[0], uint16_t(trigger_codes::is_scope | trigger_codes::x_core_scope_nation));
		EXPECT_EQ(t[1], 4ui16);
		EXPECT_EQ(t[2], uint16_t(trigger_codes::association_lt | trigger_codes::blockade));
		EXPECT_EQ(t[3], 2ui16);
		EXPECT_EQ(t[4], 1ui16);
	}
}

TEST(trigger_reading, scope_sizing) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_TRUE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(5ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
		t.push_back(0ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_FALSE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::integer_scope));
		t.push_back(5ui16);
		t.push_back(0ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_TRUE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::integer_scope));
		t.push_back(6ui16);
		t.push_back(0ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
		t.push_back(0ui16);

		EXPECT_FALSE(scope_is_empty(t.data()));
		EXPECT_FALSE(scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(1ui16);

		EXPECT_TRUE(scope_is_empty(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::integer_scope));
		t.push_back(2ui16);
		t.push_back(0ui16);

		EXPECT_TRUE(scope_is_empty(t.data()));
	}
}

TEST(trigger_reading, effect_scope_sizing) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::generic_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(effect_codes::add_accepted_culture));
		t.push_back(2ui16);
		t.push_back(1ui16);

		EXPECT_TRUE(effect_scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::generic_scope));
		t.push_back(5ui16);
		t.push_back(uint16_t(effect_codes::add_accepted_culture));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_accepted_culture));
		t.push_back(0ui16);

		EXPECT_FALSE(effect_scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::integer_scope));
		t.push_back(5ui16);
		t.push_back(0ui16);
		t.push_back(uint16_t(effect_codes::add_accepted_culture));
		t.push_back(2ui16);
		t.push_back(1ui16);

		EXPECT_TRUE(effect_scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::integer_scope));
		t.push_back(6ui16);
		t.push_back(0ui16);
		t.push_back(uint16_t(effect_codes::add_accepted_culture));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_accepted_culture));
		t.push_back(0ui16);

		EXPECT_FALSE(effect_scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::integer_scope));
		t.push_back(6ui16);
		t.push_back(trigger_payload(trigger_tag(1)).value);
		t.push_back(0ui16);
		t.push_back(uint16_t(effect_codes::add_accepted_culture));
		t.push_back(2ui16);
		t.push_back(1ui16);

		EXPECT_TRUE(effect_scope_has_single_member(t.data()));
	}

	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::integer_scope));
		t.push_back(7ui16);
		t.push_back(trigger_payload(trigger_tag(1)).value);
		t.push_back(0ui16);
		t.push_back(uint16_t(effect_codes::add_accepted_culture));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_accepted_culture));
		t.push_back(0ui16);

		EXPECT_FALSE(effect_scope_has_single_member(t.data()));
	}
}

TEST(trigger_reading, simple_negation_removal) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		const auto new_size = simplify_trigger(t.data());

		EXPECT_EQ(3, new_size);
		EXPECT_EQ(t[0], uint16_t(trigger_codes::association_ne | trigger_codes::blockade));
		EXPECT_EQ(t[1], 2ui16);
		EXPECT_EQ(t[2], 1ui16);
	}
}

TEST(trigger_reading, double_negation_removal) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(4ui16);
		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(2ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
		t.push_back(0ui16);

		const auto new_size = simplify_trigger(t.data());

		EXPECT_EQ(1, new_size);
		EXPECT_EQ(t[0], uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
	}
}

TEST(trigger_reading, complex_inversion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::placeholder_not_scope));
		t.push_back(9ui16);
		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::generic_scope));
		t.push_back(2ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::is_existance_scope | trigger_codes::is_disjunctive_scope | trigger_codes::x_core_scope_nation));
		t.push_back(4ui16);
		t.push_back(uint16_t(trigger_codes::association_ge | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);

		const auto new_size = simplify_trigger(t.data());

		EXPECT_EQ(8, new_size);
		EXPECT_EQ(t[0], uint16_t(trigger_codes::is_scope | trigger_codes::is_disjunctive_scope | trigger_codes::generic_scope));
		EXPECT_EQ(t[1], 7ui16);
		EXPECT_EQ(t[2], uint16_t(trigger_codes::association_ne | trigger_codes::no_payload | trigger_codes::blockade));
		EXPECT_EQ(t[3], uint16_t(trigger_codes::is_scope | trigger_codes::x_core_scope_nation));
		EXPECT_EQ(t[4], 4ui16);
		EXPECT_EQ(t[5], uint16_t(trigger_codes::association_lt | trigger_codes::blockade));
		EXPECT_EQ(t[6], 2ui16);
		EXPECT_EQ(t[7], 1ui16);
	}
}

TEST(trigger_reading, scope_absorbtion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::x_neighbor_province_scope | trigger_codes::is_disjunctive_scope | trigger_codes::is_existance_scope));
		t.push_back(7ui16);
		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::generic_scope));
		t.push_back(5ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
		t.push_back(0ui16);

		const auto new_size = simplify_trigger(t.data());

		EXPECT_EQ(6, new_size);
		EXPECT_EQ(t[0], uint16_t(trigger_codes::is_scope | trigger_codes::x_neighbor_province_scope | trigger_codes::is_existance_scope));
		EXPECT_EQ(t[1], 5ui16);
		EXPECT_EQ(t[2], uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		EXPECT_EQ(t[3], 2ui16);
		EXPECT_EQ(t[4], 1ui16);
		EXPECT_EQ(t[5], uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
	}
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::integer_scope));
		t.push_back(8ui16);
		t.push_back(15ui16);
		t.push_back(uint16_t(trigger_codes::is_scope | trigger_codes::generic_scope | trigger_codes::is_disjunctive_scope));
		t.push_back(5ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
		t.push_back(0ui16);

		const auto new_size = simplify_trigger(t.data());

		EXPECT_EQ(7, new_size);
		EXPECT_EQ(t[0], uint16_t(trigger_codes::is_scope | trigger_codes::integer_scope | trigger_codes::is_disjunctive_scope));
		EXPECT_EQ(t[1], 6ui16);
		EXPECT_EQ(t[2], 15ui16);
		EXPECT_EQ(t[3], uint16_t(trigger_codes::association_eq | trigger_codes::blockade));
		EXPECT_EQ(t[4], 2ui16);
		EXPECT_EQ(t[5], 1ui16);
		EXPECT_EQ(t[6], uint16_t(trigger_codes::association_eq | trigger_codes::no_payload | trigger_codes::blockade));
	}
}

TEST(trigger_reading, effect_scope_absorbtion) {
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::generic_scope));
		t.push_back(9ui16);
		t.push_back(trigger_payload(trigger_tag()).value);
		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::x_neighbor_province_scope));
		t.push_back(6ui16);
		t.push_back(trigger_payload(trigger_tag()).value);
		t.push_back(uint16_t(effect_codes::add_core_this_pop));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_core_this_pop));
		t.push_back(0ui16);

		const auto new_size = simplify_effect(t.data());

		EXPECT_EQ(6, new_size);
		EXPECT_EQ(t[0], uint16_t(effect_codes::is_scope | effect_codes::x_neighbor_province_scope));
		EXPECT_EQ(t[1], 5ui16);
		EXPECT_EQ(t[2], uint16_t(effect_codes::add_core_this_pop));
		EXPECT_EQ(t[3], 2ui16);
		EXPECT_EQ(t[4], 1ui16);
		EXPECT_EQ(t[5], uint16_t(effect_codes::no_payload | effect_codes::add_core_this_pop));
	}
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::x_neighbor_province_scope));
		t.push_back(9ui16);
		t.push_back(trigger_payload(trigger_tag()).value);
		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::generic_scope));
		t.push_back(5ui16);
		t.push_back(trigger_payload(trigger_tag()).value);
		t.push_back(uint16_t(effect_codes::add_core_this_pop));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_core_this_pop));
		t.push_back(0ui16);

		const auto new_size = simplify_effect(t.data());

		EXPECT_EQ(6, new_size);
		EXPECT_EQ(t[0], uint16_t(effect_codes::is_scope | effect_codes::x_neighbor_province_scope));
		EXPECT_EQ(t[1], 5ui16);
		EXPECT_EQ(t[2], uint16_t(effect_codes::add_core_this_pop));
		EXPECT_EQ(t[3], 2ui16);
		EXPECT_EQ(t[4], 1ui16);
		EXPECT_EQ(t[5], uint16_t(effect_codes::no_payload | effect_codes::add_core_this_pop));
	}
	{
		std::vector<uint16_t> t;

		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::integer_scope));
		t.push_back(9ui16);
		t.push_back(trigger_payload(trigger_tag(2)).value);
		t.push_back(15ui16);
		t.push_back(uint16_t(effect_codes::is_scope | effect_codes::generic_scope));
		t.push_back(5ui16);
		t.push_back(uint16_t(effect_codes::activate_technology));
		t.push_back(2ui16);
		t.push_back(1ui16);
		t.push_back(uint16_t(effect_codes::no_payload | effect_codes::add_accepted_culture));
		t.push_back(0ui16);

		const auto new_size = simplify_effect(t.data());

		EXPECT_EQ(10, new_size);
		EXPECT_EQ(t[0], uint16_t(effect_codes::is_scope | effect_codes::scope_has_limit | effect_codes::integer_scope));
		EXPECT_EQ(t[1], 9ui16);
		EXPECT_EQ(trigger_payload(t[2]).trigger, trigger_tag(2));
		EXPECT_EQ(t[3], 15ui16);
		EXPECT_EQ(t[4], uint16_t(effect_codes::is_scope | effect_codes::generic_scope));
		EXPECT_EQ(t[5], 5ui16);
		EXPECT_EQ(t[6], uint16_t(effect_codes::activate_technology));
		EXPECT_EQ(t[7], 2ui16);
		EXPECT_EQ(t[8], 1ui16);
		EXPECT_EQ(t[9], uint16_t(effect_codes::no_payload | effect_codes::add_accepted_culture));
	}
}

TEST(trigger_reading, simple_trigger) {
	const char trigger[] = "always = no";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	trigger_parsing_environment parse_env(
		sm,
		trigger_scope_state{
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false});

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	add_simple_trigger(
		parse_env,
		parse_results[0].token,
		parse_results[0].association,
		parse_results[1].token);

	EXPECT_EQ(1ui64, parse_env.data.size());
	EXPECT_EQ(uint16_t(trigger_codes::always | trigger_codes::no_payload | trigger_codes::association_ne), parse_env.data[0]);
}

TEST(trigger_reading, simple_effect) {
	const char trigger[] = "is_slave = yes";

	text_data::text_sequences ts;
	events::event_creation_manager ecm;
	scenario::scenario_manager sm(ts);
	effect_parsing_environment parse_env(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::state,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false });

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	add_simple_effect(
		parse_env,
		parse_results[0].token,
		parse_results[0].association,
		parse_results[1].token);

	EXPECT_EQ(1ui64, parse_env.data.size());
	EXPECT_EQ(uint16_t(effect_codes::is_slave_state_yes |  effect_codes::no_payload), parse_env.data[0]);
}

TEST(trigger_reading, storing_data) {
	{
		std::vector<uint16_t> t;
		add_float_to_payload(t, 2.5f);
		EXPECT_EQ(2ui64, t.size());
		EXPECT_EQ(2.5f, read_float_from_payload(t.data()));
	}
	{
		std::vector<uint16_t> t;
		add_int32_t_to_payload(t, 10002);
		EXPECT_EQ(2ui64, t.size());
		EXPECT_EQ(10002, read_int32_t_from_payload(t.data()));
	}
	{
		std::vector<uint16_t> t;
		issues::option_identifier opt;
		
		opt.id = issues::issue_tag(1);
		opt.option_id = issues::option_tag(4);
		opt.type = issues::issue_group::party;

		add_option_identifier_to_payload(t, opt);
		EXPECT_EQ(2ui64, t.size());

		const auto res = read_option_identifier_from_payload(t.data());

		EXPECT_EQ(opt.id, res.id);
		EXPECT_EQ(opt.option_id, res.option_id);
		EXPECT_EQ(opt.type, res.type);
	}
}

TEST(trigger_reading, scope_dependant_trigger) {
	const char trigger[] = "militancy < 12";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	trigger_parsing_environment parse_env(
		sm,
		trigger_scope_state{
			trigger_slot_contents::province,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false });

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	add_simple_trigger(
		parse_env,
		parse_results[0].token,
		parse_results[0].association,
		parse_results[1].token);

	EXPECT_EQ(4ui64, parse_env.data.size());
	EXPECT_EQ(uint16_t(trigger_codes::militancy_province | trigger_codes::association_lt), parse_env.data[0]);
	EXPECT_EQ(3ui16, parse_env.data[1]);
	EXPECT_EQ(12.0f, read_float_from_payload(parse_env.data.data() + 2));
}

class test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test2 = directory_representation(u"test1", f_root);
	directory_representation common2 = directory_representation(u"common", test2);
	file_representation countries2 = file_representation(u"countries.txt", common2,
		"GER = \"countries/file 1.txt\"\r\n"
		"ENG = \"countries/file 2.txt\"");
	file_representation cul2 = file_representation(u"cultures.txt", common2,
		"\xEF\xBB\xBFgroup = {\r\n"
		"leader = X\r\n"
		"unit = Y\r\n"
		"sub_culture_a = {\r\n"
		"color = {10 20 30}\r\n"
		"first_names = {a b c}\r\n"
		"last_names = { d }\r\n"
		"}\r\n"
		"}\r\n"
		"group_b = { \r\n"
		"sub_culture_b = {\r\n"
		"color = {20 30 40}\r\n"
		"first_names = {a bddd}\r\n"
		"last_names = { d e }\r\n"
		"}\r\n"
		"sub_culture_c = {\r\n"
		"color = {50 60 70}\r\n"
		"first_names = {asss bddd csss d}\r\n"
		"last_names = { q x }\r\n"
		"}\r\n"
		"union = ENG\n"
		"}");
	file_representation ideology_file = file_representation(u"ideologies.txt", common2,
		"group_a = {\r\n"
		"member_1 = { stuff }\r\n"
		"member_2 = {}\r\n"
		"}\r\n"
		"group_2 = { member_3 = { a b c } }");
	file_representation goods_file = file_representation(u"goods.txt", common2,
		"military_goods   = {\r\n"
		"money_good = {\r\n"
		"cost = 8\r\n"
		"color = { 0 0 0 }\r\n"
		"overseas_penalty  = yes\r\n"
		"tradeable = no\r\n"
		"money = yes\r\n"
		"}\r\n"
		"}\r\n"
		"other_goods = {\r\n"
		"cement = { \r\n"
		"	cost = 16.0\r\n"
		"	color = { 185 187 164 }\r\n"
		"}\r\n"
		"glass = { \r\n"
		"	cost = 2.9\r\n"
		"	color = { 201 200 199 }\r\n"
		"}\r\n"
		"}");

	test_files() {
		set_default_root(f_root);
	}
};

TEST(trigger_reading, data_lookup_trigger) {
	const char trigger[] = "has_pop_culture = sub_culture_b";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	cultures::parse_national_tags(sm.culutre_m, f.get_root());
	cultures::parse_cultures(sm.culutre_m, f.get_root(),
		[&ts](const char* s, const char* e) {
		    return text_data::get_text_handle(ts, s, e); });

	trigger_parsing_environment parse_env(
		sm,
		trigger_scope_state{
			trigger_slot_contents::state,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false });

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	add_simple_trigger(
		parse_env,
		parse_results[0].token,
		parse_results[0].association,
		parse_results[1].token);

	EXPECT_EQ(3ui64, parse_env.data.size());
	EXPECT_EQ(uint16_t(trigger_codes::has_pop_culture_state | trigger_codes::association_eq), parse_env.data[0]);
	EXPECT_EQ(2ui16, parse_env.data[1]);
	EXPECT_EQ(cultures::culture_tag(1), trigger_payload(parse_env.data[2]).culture);
}

TEST(trigger_reading, data_lookup_effect) {
	const char trigger[] = "add_accepted_culture = sub_culture_b";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	events::event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	cultures::parse_national_tags(sm.culutre_m, f.get_root());
	cultures::parse_cultures(sm.culutre_m, f.get_root(),
		[&ts](const char* s, const char* e) {
		return text_data::get_text_handle(ts, s, e); });

	effect_parsing_environment parse_env(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false });

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	add_simple_effect(
		parse_env,
		parse_results[0].token,
		parse_results[0].association,
		parse_results[1].token);

	EXPECT_EQ(3ui64, parse_env.data.size());
	EXPECT_EQ(uint16_t(effect_codes::add_accepted_culture), parse_env.data[0]);
	EXPECT_EQ(2ui16, parse_env.data[1]);
	EXPECT_EQ(cultures::culture_tag(1), trigger_payload(parse_env.data[2]).culture);
}

TEST(trigger_reading, variable_trigger) {
	const char trigger[] = "member_3 = 2.5";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	ideologies::pre_parse_ideologies(sm.ideologies_m, f.get_root(),
		[&ts](const char* s, const char* e) {
		return text_data::get_text_handle(ts, s, e); });

	trigger_parsing_environment parse_env(
		sm,
		trigger_scope_state{
			trigger_slot_contents::province,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false });

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	add_simple_trigger(
		parse_env,
		parse_results[0].token,
		parse_results[0].association,
		parse_results[1].token);

	EXPECT_EQ(5ui64, parse_env.data.size());
	EXPECT_EQ(uint16_t(trigger_codes::variable_ideology_name_province | trigger_codes::association_ge), parse_env.data[0]);
	EXPECT_EQ(4ui16, parse_env.data[1]);
	EXPECT_EQ(ideologies::ideology_tag(2), trigger_payload(parse_env.data[2]).small.values.ideology);
	EXPECT_EQ(2.5f, read_float_from_payload(parse_env.data.data() + 3));
}

TEST(trigger_reading, variable_effect) {
	const char trigger[] = "glass = 5.5";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	events::event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	ideologies::pre_parse_ideologies(sm.ideologies_m, f.get_root(),
		[&ts](const char* s, const char* e) {
		return text_data::get_text_handle(ts, s, e); });
	economy::read_goods(sm.economy_m, f.get_root(), [&ts](const char* s, const char* e) {
		return text_data::get_text_handle(ts, s, e); });

	effect_parsing_environment parse_env(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false });

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	add_simple_effect(
		parse_env,
		parse_results[0].token,
		parse_results[0].association,
		parse_results[1].token);

	EXPECT_EQ(5ui64, parse_env.data.size());
	EXPECT_EQ(uint16_t(effect_codes::variable_good_name), parse_env.data[0]);
	EXPECT_EQ(4ui16, parse_env.data[1]);
	EXPECT_EQ(economy::goods_tag(1), trigger_payload(parse_env.data[2]).small.values.good);
	EXPECT_EQ(5.5f, read_float_from_payload(parse_env.data.data() + 3));
}

TEST(trigger_reading, basic_full_read) {
	const char trigger[] = "always = no";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_trigger(
		sm,
		trigger_scope_state{
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
		parse_results.data(),
		parse_results.data() + parse_results.size());

	EXPECT_EQ(1ui64, data.size());
	EXPECT_EQ(uint16_t(trigger_codes::always | trigger_codes::no_payload | trigger_codes::association_ne), data[0]);
}

TEST(trigger_reading, complex_full_reduction) {
	const char trigger[] = "NOT = { AND = { AND = { always = yes } any_core = { average_consciousness >= 1.0 } } }";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_trigger(
		sm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(9ui64, data.size());
	EXPECT_EQ(data[0], uint16_t(trigger_codes::is_scope | trigger_codes::is_disjunctive_scope | trigger_codes::generic_scope));
	EXPECT_EQ(data[1], 8ui16);
	EXPECT_EQ(data[2], uint16_t(trigger_codes::association_ne | trigger_codes::no_payload | trigger_codes::always));
	EXPECT_EQ(data[3], uint16_t(trigger_codes::is_scope | trigger_codes::is_disjunctive_scope | trigger_codes::x_core_scope_nation));
	EXPECT_EQ(data[4], 5ui16);
	EXPECT_EQ(data[5], uint16_t(trigger_codes::association_lt | trigger_codes::average_consciousness_province));
	EXPECT_EQ(data[6], 3ui16);
	EXPECT_EQ(1.0f, read_float_from_payload(data.data() + 7));
}

TEST(trigger_reading, multipart_trigger) {
	const char trigger[] = "diplomatic_influence = { value > 5 who = GER }";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	cultures::parse_national_tags(sm.culutre_m, f.get_root());

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_trigger(
		sm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(4ui64, data.size());
	EXPECT_EQ(data[0], uint16_t(trigger_codes::association_gt | trigger_codes::diplomatic_influence_tag));
	EXPECT_EQ(data[1], 3ui16);
	EXPECT_EQ(trigger_payload(data[2]).value, 5ui16);
	EXPECT_EQ(trigger_payload(data[3]).tag, cultures::national_tag(0));
}

TEST(trigger_reading, multipart_effect) {
	const char trigger[] = "relation = { value = -10 who = THIS }";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	events::event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	cultures::parse_national_tags(sm.culutre_m, f.get_root());

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_effect(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::province,
			trigger_slot_contents::empty,
			false },
		parse_results.data(),
		parse_results.data() + parse_results.size());

	EXPECT_EQ(3ui64, data.size());
	EXPECT_EQ(data[0], uint16_t(effect_codes::relation_this_province));
	EXPECT_EQ(data[1], 2ui16);
	EXPECT_EQ(trigger_payload(data[2]).signed_value, -10i16);
}

TEST(trigger_reading, variable_scope) {
	const char trigger[] = "6 = { average_consciousness >= 1.0 average_militancy != 0.5 }";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_trigger(
		sm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(11ui64, data.size());
	EXPECT_EQ(data[0], uint16_t(trigger_codes::is_scope | trigger_codes::integer_scope));
	EXPECT_EQ(data[1], 10ui16);
	EXPECT_EQ(data[2], 6ui16);
	EXPECT_EQ(data[3], uint16_t(trigger_codes::association_ge | trigger_codes::average_consciousness_province));
	EXPECT_EQ(data[4], 3ui16);
	EXPECT_EQ(1.0f, read_float_from_payload(data.data() + 5));
	EXPECT_EQ(data[7], uint16_t(trigger_codes::association_ne | trigger_codes::average_militancy_province));
	EXPECT_EQ(data[8], 3ui16);
	EXPECT_EQ(0.5f, read_float_from_payload(data.data() + 9));
}

TEST(trigger_reading, variable_effect_scope) {
	const char trigger[] = "6 = { owner = { capital = 1 treasury = 0.5 } }";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	events::event_creation_manager ecm;

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_effect(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::state,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(12ui64, data.size());
	EXPECT_EQ(data[0], uint16_t(effect_codes::is_scope | effect_codes::integer_scope));
	EXPECT_EQ(data[1], 11ui16);
	EXPECT_EQ(data[2], 6ui16);
	EXPECT_EQ(data[3], uint16_t(effect_codes::is_scope | effect_codes::owner_scope_province));
	EXPECT_EQ(data[4], 8ui16);
	EXPECT_EQ(data[5], uint16_t(effect_codes::capital));
	EXPECT_EQ(data[6], 2ui16);
	EXPECT_EQ(data[7], 1ui16);
	EXPECT_EQ(data[8], uint16_t(effect_codes::treasury));
	EXPECT_EQ(data[9], 3ui16);
	EXPECT_EQ(0.5f, read_float_from_payload(data.data() + 10));
}

TEST(trigger_reading, effect_with_limit) {
	const char trigger[] = "any_country = { limit = { tag = this } capital = 1 }";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	events::event_creation_manager ecm;

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_effect(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::province,
			trigger_slot_contents::province,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(6ui64, data.size());
	EXPECT_EQ(data[0], uint16_t(effect_codes::is_scope | effect_codes::x_country_scope | effect_codes::scope_has_limit));
	EXPECT_EQ(data[1], 5ui16);
	EXPECT_EQ(trigger_payload(data[2]).trigger, trigger_tag(0));
	EXPECT_EQ(data[3], uint16_t(effect_codes::capital));
	EXPECT_EQ(data[4], 2ui16);
	EXPECT_EQ(data[5], 1ui16);

	EXPECT_EQ(2ui64, sm.trigger_m.trigger_data.size());
	EXPECT_EQ(uint16_t(trigger_codes::tag_this_province | trigger_codes::association_eq | trigger_codes::no_payload), sm.trigger_m.trigger_data[0]);
	EXPECT_EQ(0ui16, sm.trigger_m.trigger_data[1]);
}

TEST(trigger_reading, basic_factor_trigger) {
	const char trigger[] = "factor = 1.5 always = no";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto t_result = parse_trigger_and_factor(
		sm,
		trigger_scope_state{
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(1.5f, t_result.factor);
	EXPECT_EQ(1ui64, t_result.data.size());
	EXPECT_EQ(uint16_t(trigger_codes::always | trigger_codes::no_payload | trigger_codes::association_ne), t_result.data[0]);
}

TEST(trigger_reading, empty_trigger) {
	const char trigger[] = "";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto t_result = parse_trigger(
		sm,
		trigger_scope_state{
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(0ui64, t_result.size());
}

TEST(trigger_reading, empty_effect) {
	const char trigger[] = "";

	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	events::event_creation_manager ecm;

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto t_result = parse_effect(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(0ui64, t_result.size());
}

TEST(trigger_reading, commit_trigger) {
	trigger_manager m;
	EXPECT_EQ(1ui64, m.trigger_data.size());

	const auto etag = commit_trigger(m, std::vector<uint16_t>());
	EXPECT_EQ(1ui64, m.trigger_data.size());
	EXPECT_EQ(trigger_tag(), etag);

	std::vector<uint16_t> a{ 1ui16, 2ui16, 3ui16 };

	const auto atag = commit_trigger(m, a);
	EXPECT_EQ(4ui64, m.trigger_data.size());
	EXPECT_EQ(trigger_tag(0), atag);

	std::vector<uint16_t> b{ 6ui16, 7ui16 };

	const auto btag = commit_trigger(m, b);
	EXPECT_EQ(6ui64, m.trigger_data.size());
	EXPECT_EQ(trigger_tag(3), btag);

	std::vector<uint16_t> c{ 2ui16, 3ui16, 6ui16 };

	const auto ctag = commit_trigger(m, c);
	EXPECT_EQ(6ui64, m.trigger_data.size());
	EXPECT_EQ(trigger_tag(1), ctag);
}

TEST(trigger_reading, commit_effect) {
	trigger_manager m;
	EXPECT_EQ(1ui64, m.effect_data.size());

	const auto etag = commit_effect(m, std::vector<uint16_t>());
	EXPECT_EQ(1ui64, m.effect_data.size());
	EXPECT_EQ(effect_tag(), etag);

	std::vector<uint16_t> a{ 1ui16, 2ui16, 3ui16 };

	const auto atag = commit_effect(m, a);
	EXPECT_EQ(4ui64, m.effect_data.size());
	EXPECT_EQ(effect_tag(0), atag);

	std::vector<uint16_t> b{ 6ui16, 7ui16 };

	const auto btag = commit_effect(m, b);
	EXPECT_EQ(6ui64, m.effect_data.size());
	EXPECT_EQ(effect_tag(3), btag);

	std::vector<uint16_t> c{ 2ui16, 3ui16, 6ui16 };

	const auto ctag = commit_effect(m, c);
	EXPECT_EQ(6ui64, m.effect_data.size());
	EXPECT_EQ(effect_tag(1), ctag);
}
