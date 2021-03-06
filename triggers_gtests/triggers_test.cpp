#include "gtest\\gtest.h"
#include "triggers\\trigger_reading.h"
#include "triggers\\codes.h"
#include "scenario\\scenario_io.h"
#include "issues\\issues.h"
#include "simple_fs\\simple_fs.h"
#include "fake_fs\\fake_fs.h"
#include "triggers\\effects.h"
#include "triggers\\effect_reading.h"
#include "events\\events_io.h"
#include "world_state\\world_state.h"
#include "world_state\\world_state_io.h"
#include "scenario\\scenario_io.h"

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

	scenario::scenario_manager sm;
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

	events::event_creation_manager ecm;
	scenario::scenario_manager sm;
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
}

TEST(trigger_reading, scope_dependant_trigger) {
	const char trigger[] = "militancy < 12";

	scenario::scenario_manager sm;
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
	file_representation issues_file = file_representation(u"issues.txt", common2,
		"party_issues = {\r\n"
		"	public_meetings = {\r\n"
		"		no_meeting = {\r\n"
		"			#no immediate effect\r\n"
		"		}\r\n"
		"		yes_meeting = {\r\n"
		"			issue_change_speed = 1\r\n"
		"			global_immigrant_attract = 0.01\r\n"
		"			suppression_points_modifier = -0.25\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n");
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

	scenario::scenario_manager sm;
	graphics::texture_manager tex;

	test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	cultures::read_national_tags(sm.culture_m, f.get_root());
	cultures::read_cultures(sm.culture_m, tex, f.get_root(), sm.gui_m.text_data_sequences);

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

	scenario::scenario_manager sm;
	events::event_creation_manager ecm;
	graphics::texture_manager tex;

	test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	cultures::read_national_tags(sm.culture_m, f.get_root());
	cultures::read_cultures(sm.culture_m, tex, f.get_root(), sm.gui_m.text_data_sequences);

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

	scenario::scenario_manager sm;

	test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	ideologies::pre_parse_ideologies(sm.ideologies_m, f.get_root(), sm.gui_m.text_data_sequences);

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

	scenario::scenario_manager sm;
	events::event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	ideologies::pre_parse_ideologies(sm.ideologies_m, f.get_root(), sm.gui_m.text_data_sequences);
	economy::read_goods(sm.economy_m, f.get_root(), sm.gui_m.text_data_sequences);

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
	EXPECT_EQ(economy::goods_tag(2), trigger_payload(parse_env.data[2]).small.values.good);
	EXPECT_EQ(5.5f, read_float_from_payload(parse_env.data.data() + 3));
}

TEST(trigger_reading, basic_full_read) {
	const char trigger[] = "always = no";

	scenario::scenario_manager sm;

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

	scenario::scenario_manager sm;

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

	scenario::scenario_manager sm;

	test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	cultures::read_national_tags(sm.culture_m, f.get_root());

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

	scenario::scenario_manager sm;
	events::event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	cultures::read_national_tags(sm.culture_m, f.get_root());

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

	scenario::scenario_manager sm;

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

	scenario::scenario_manager sm;
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

	scenario::scenario_manager sm;
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

TEST(trigger_reading, with_named_issue) {
	const char trigger[] =
		"\t\t\tAI = no\r\n"
		"\t\t\tpublic_meetings = no_meeting\r\n"
		"\t\t\tconsciousness = 5\r\n"
		"\t\t\tliteracy = 0.40\r\n"
		"\t\t\tyes_meeting = 10\r\n"
		"\t\t\tany_neighbor_country = {\r\n"
		"\t\t\t\tpublic_meetings = yes_meeting\r\n"
		"\t\t\t}\r\n"
		"\t\t\tNOT = {\r\n"
		"\t\t\t\tOR = {\r\n"
		"\t\t\t\t\thas_country_flag = yes_meeting_noway\r\n"
		"\t\t\t\t\thas_country_flag = yes_meeting_promised\r\n"
		"\t\t\t\t}\r\n"
		"\t\t\t}\r\n";


	test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	scenario::scenario_manager sm;
	events::event_creation_manager ecm;

	const auto issues_state = issues::pre_parse_issues(sm.issues_m, f.get_root(), sm.gui_m.text_data_sequences);
	issues::read_issue_options(issues_state, sm, ecm);

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(trigger));

	auto data = parse_trigger(
		sm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::nation,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_NE(1ui64, data.size());
}

TEST(trigger_reading, effect_option_reading) {
	const char option[] =
		"name = mmmm\r\n"
		"ai_chance = { factor = 2 modifier = { factor = 3 year = 1 } }\r\n"
		"capital = 6";

	scenario::scenario_manager sm;
	events::event_creation_manager ecm;

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(option));

	auto opt = parse_option_effect(
		sm,
		ecm,
		trigger_scope_state{
			trigger_slot_contents::nation,
			trigger_slot_contents::nation,
			trigger_slot_contents::empty,
			false },
			parse_results.data(),
			parse_results.data() + parse_results.size());

	EXPECT_EQ(text_data::text_tag(0), opt.name);
	EXPECT_EQ(effect_tag(0), opt.effect);
	EXPECT_EQ(modifiers::factor_tag(0), opt.ai_chance);

	EXPECT_EQ(2.0f, sm.modifiers_m.factor_modifiers[modifiers::factor_tag(0)].factor);
	EXPECT_EQ(0ui16, sm.modifiers_m.factor_modifiers[modifiers::factor_tag(0)].data_offset);
	EXPECT_EQ(1ui16, sm.modifiers_m.factor_modifiers[modifiers::factor_tag(0)].data_length);
	EXPECT_EQ(3.0f, sm.modifiers_m.factor_data[0].factor);
	EXPECT_EQ(trigger_tag(0), sm.modifiers_m.factor_data[0].condition);
	EXPECT_EQ(uint16_t(trigger_codes::year | trigger_codes::association_ge), sm.trigger_m.trigger_data[0]);
	EXPECT_EQ(2ui16, sm.trigger_m.trigger_data[1]);
	EXPECT_EQ(1ui16, sm.trigger_m.trigger_data[2]);

	EXPECT_EQ(uint16_t(effect_codes::capital), sm.trigger_m.effect_data[0]);
	EXPECT_EQ(2ui16, sm.trigger_m.effect_data[1]);
	EXPECT_EQ(6ui16, sm.trigger_m.effect_data[2]);
}

TEST(trigger_reading, basic_factor_trigger) {
	const char trigger[] = "factor = 1.5 always = no";

	scenario::scenario_manager sm;

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

	scenario::scenario_manager sm;

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

	scenario::scenario_manager sm;
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

TEST(trigger_execution, set_a) {
	world_state ws;

	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();

	ready_world_state(ws);

	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_save_cmp.bin", ws.w, ws);

	{
		const char trigger[] = "GER = { exists = yes }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "USA = { exists = yes }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "exists = USA";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "exists = GER";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "NOT = { GER = { exists = yes } }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "OR = { GER = { exists = yes } USA = { exists = yes } }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "AND = { GER = { exists = yes } USA = { exists = yes } }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "year == 1836";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "year <= 1836";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "year > 1836";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "month = 1";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "month = 2";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "NOT = { month = 2 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "78 = { port = yes }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "96 = { port = no }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "ENG = { rank = 1 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "USA = { rank = 20 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "USA = { rank = 1 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "USA = { post_napoleonic_thought = 1 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "USA = { authoritarianism = yes }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "USA = { army_decision_making = 1 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "USA = { army_decision_making = 0 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "NOT = { 2133 = { any_pop = { strata = rich } } }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_TRUE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}

	{
		const char trigger[] = "NOT = { 2133 = { any_pop = { strata = poor } } }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_trigger(
			ws.s,
			trigger_scope_state{
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		t_result.push_back(0ui16);

		EXPECT_FALSE(test_trigger(t_result.data(), ws, nullptr, nullptr, nullptr, nullptr));
	}
}

TEST(effect_execution, set_a) {
	world_state ws;

	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();

	ready_world_state(ws);

	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_save_cmp.bin", ws.w, ws);

	auto ptr_ger = ws.w.culture_s.national_tags_state[ws.s.culture_m.national_tags_index[cultures::tag_to_encoding("GER")]].holder;
	auto ptr_usa = ws.w.culture_s.national_tags_state[ws.s.culture_m.national_tags_index[cultures::tag_to_encoding("USA")]].holder;

	{
		const char effect[] = "prestige = 10";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, effect);

		events::event_creation_manager ecm;

		auto e_result = parse_effect(
			ws.s,
			ecm,
			trigger_scope_state{
				trigger_slot_contents::nation,
				trigger_slot_contents::nation,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		e_result.push_back(0ui16);

		auto pre_prestige = ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger);

		execute_effect(
			e_result.data(),
			ws,
			ptr_ger,
			ptr_ger,
			nullptr,
			nullptr,
			get_local_generator());

		EXPECT_EQ(pre_prestige + 10 * (ws.w.nation_s.nations.get<nation::tech_attributes>(ptr_ger)[technologies::tech_offset::prestige] + 1.0f), ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger));
	}

	{
		const char effect[] = "any_country = { prestige = 10 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, effect);

		events::event_creation_manager ecm;

		auto e_result = parse_effect(
			ws.s,
			ecm,
			trigger_scope_state{
				trigger_slot_contents::nation,
				trigger_slot_contents::nation,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		e_result.push_back(0ui16);

		auto pre_prestige_a = ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger);
		auto pre_prestige_b = ws.w.nation_s.nations.get<nation::base_prestige>(ptr_usa);

		execute_effect(
			e_result.data(),
			ws,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			get_local_generator());

		EXPECT_EQ(pre_prestige_a + 10 * (ws.w.nation_s.nations.get<nation::tech_attributes>(ptr_ger)[technologies::tech_offset::prestige] + 1.0f), ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger));
		EXPECT_EQ(pre_prestige_b + 10 * (ws.w.nation_s.nations.get<nation::tech_attributes>(ptr_usa)[technologies::tech_offset::prestige] + 1.0f), ws.w.nation_s.nations.get<nation::base_prestige>(ptr_usa));
	}

	{
		const char effect[] = "any_country = { limit = { tag = USA } prestige = 10 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, effect);

		events::event_creation_manager ecm;

		auto e_result = parse_effect(
			ws.s,
			ecm,
			trigger_scope_state{
				trigger_slot_contents::nation,
				trigger_slot_contents::nation,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		e_result.push_back(0ui16);

		auto pre_prestige_a = ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger);
		auto pre_prestige_b = ws.w.nation_s.nations.get<nation::base_prestige>(ptr_usa);

		execute_effect(
			e_result.data(),
			ws,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			get_local_generator());

		EXPECT_EQ(pre_prestige_a, ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger));
		EXPECT_EQ(pre_prestige_b + 10 * (ws.w.nation_s.nations.get<nation::tech_attributes>(ptr_usa)[technologies::tech_offset::prestige] + 1.0f), ws.w.nation_s.nations.get<nation::base_prestige>(ptr_usa));
	}

	{
		const char effect[] = "random_country = { limit = { tag = USA } prestige = 10 }";
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, effect);

		events::event_creation_manager ecm;

		auto e_result = parse_effect(
			ws.s,
			ecm,
			trigger_scope_state{
				trigger_slot_contents::nation,
				trigger_slot_contents::nation,
				trigger_slot_contents::empty,
				false },
				parse_results.data(),
				parse_results.data() + parse_results.size());
		e_result.push_back(0ui16);

		auto pre_prestige_a = ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger);
		auto pre_prestige_b = ws.w.nation_s.nations.get<nation::base_prestige>(ptr_usa);

		execute_effect(
			e_result.data(),
			ws,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			get_local_generator());

		EXPECT_EQ(pre_prestige_a, ws.w.nation_s.nations.get<nation::base_prestige>(ptr_ger));
		EXPECT_EQ(pre_prestige_b + 10 * (ws.w.nation_s.nations.get<nation::tech_attributes>(ptr_usa)[technologies::tech_offset::prestige] + 1.0f), ws.w.nation_s.nations.get<nation::base_prestige>(ptr_usa));
	}
}
