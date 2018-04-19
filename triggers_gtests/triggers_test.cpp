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
