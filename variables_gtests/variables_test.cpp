#include "gtest\\gtest.h"
#include "variables\\variables.h"

using namespace variables;

TEST(variables_tests, test_global_var) {
	variables_manager m;

	const auto r = m.get_named_global_variable(text_data::text_tag(0));

	EXPECT_EQ(1ui32, m.count_global_variables);
	EXPECT_EQ(global_variable_tag(0), r);
	EXPECT_EQ(global_variable_tag(0), m.named_global_variables[text_data::text_tag(0)]);

	const auto r2 = m.get_named_global_variable(text_data::text_tag(1));

	EXPECT_EQ(2ui32, m.count_global_variables);
	EXPECT_EQ(global_variable_tag(1), r2);
	EXPECT_EQ(global_variable_tag(1), m.named_global_variables[text_data::text_tag(1)]);
}

TEST(variables_tests, test_national_var) {
	variables_manager m;

	const auto r = m.get_named_national_variable(text_data::text_tag(0));

	EXPECT_EQ(1ui32, m.count_national_variables);
	EXPECT_EQ(national_variable_tag(0), r);
	EXPECT_EQ(national_variable_tag(0), m.named_national_variables[text_data::text_tag(0)]);

	const auto r2 = m.get_named_national_variable(text_data::text_tag(1));

	EXPECT_EQ(2ui32, m.count_national_variables);
	EXPECT_EQ(national_variable_tag(1), r2);
	EXPECT_EQ(national_variable_tag(1), m.named_national_variables[text_data::text_tag(1)]);
}

TEST(variables_tests, test_national_flag) {
	variables_manager m;

	const auto r = m.get_named_national_flag(text_data::text_tag(0));

	EXPECT_EQ(1ui32, m.count_national_flags);
	EXPECT_EQ(national_flag_tag(0), r);
	EXPECT_EQ(national_flag_tag(0), m.named_national_flags[text_data::text_tag(0)]);

	const auto r2 = m.get_named_national_flag(text_data::text_tag(1));

	EXPECT_EQ(2ui32, m.count_national_flags);
	EXPECT_EQ(national_flag_tag(1), r2);
	EXPECT_EQ(national_flag_tag(1), m.named_national_flags[text_data::text_tag(1)]);
}
