#include "pch.h"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

#define TEST_METHOD(x, y) TEST(x, y)

TEST_METHOD(compile_time_computations, text_classifier_tests) {

	char v1[] = "STR in";
	const auto r1 = compile_time_str_compare_ci<CT_STRING("lalala")>(RANGE(v1));
	EXPECT_NE(0, r1);

	char v2[] = "lala";
	const auto r2 = compile_time_str_compare_ci<CT_STRING("lalala")>(RANGE(v2));
	EXPECT_NE(0, r2);

	char v3[] = "lalala";
	const auto r3 = compile_time_str_compare_ci<CT_STRING("lalala")>(RANGE(v3));
	EXPECT_EQ(0, r3);

	char v4[] = "LALAla";
	const auto r4 = compile_time_str_compare_ci<CT_STRING("lalala")>(RANGE(v4));
	EXPECT_EQ(0, r4);

#ifdef __llvm__
	static_assert(std::is_same_v<CT_S("lalala"), CT_STRING("lalala")>);
#endif
}

TEST_METHOD(empty_classifier_test, text_classifier_tests) {
	std::vector<text_identifier> options;
	using empty_map_type = type_list<>;
	using sorted_map_type = typename sorted<empty_map_type>::type;

	const auto empty_bc = make_bit_function_classifier_function(options);
	const auto empty_ls = make_linear_scan_classifier_function(options);
	const auto empty_bs = make_binary_search_classifier_function(options);
	const auto empty_bsc = binary_search_classifier(options);


	EXPECT_EQ((unsigned char)0, empty_bc(RANGE("")));
	EXPECT_EQ((unsigned char)0, empty_ls(RANGE("")));
	EXPECT_EQ((unsigned char)0, empty_bs(RANGE("")));
	EXPECT_EQ((unsigned char)0, empty_bsc.classify(RANGE("")));
	EXPECT_EQ(0, find_value_or<empty_map_type>(RANGE(""), 0));
	EXPECT_EQ(0, bt_find_value_or<sorted_map_type>(RANGE(""), 0));

	EXPECT_EQ((unsigned char)0, empty_bc(RANGE("text")));
	EXPECT_EQ((unsigned char)0, empty_ls(RANGE("text")));
	EXPECT_EQ((unsigned char)0, empty_bs(RANGE("text")));
	EXPECT_EQ((unsigned char)0, empty_bsc.classify(RANGE("text")));
	EXPECT_EQ(0, bt_find_value_or<sorted_map_type>(RANGE("text"), 0));
}

TEST_METHOD(small_classifier_test, text_classifier_tests) {
	const char dog[] = "dog";
	const char cat[] = "cat";
	const char monkey[] = "monkey";

	std::vector<text_identifier> options_a = { text_identifier{ RANGE(dog), 2 }, text_identifier{ RANGE(cat), 1 }, text_identifier{ RANGE(monkey), 3 }, };
	std::vector<text_identifier> options_b = { text_identifier{ RANGE(dog), 2 }, text_identifier{ RANGE(cat), 1 }, text_identifier{ RANGE(monkey), 3 }, };
	std::vector<text_identifier> options_c = { text_identifier{ RANGE(dog), 2 }, text_identifier{ RANGE(cat), 1 }, text_identifier{ RANGE(monkey), 3 }, };

	using map_type = type_list<CT_STRING_INT("dog", 2), CT_STRING_INT("cat", 1), CT_STRING_INT("monkey", 3)>;
	using sorted_map_type = typename sorted<map_type>::type;
	using first_in_map = typename head<1ui64, sorted_map_type>::type;
	using second_in_map = typename rest<1ui64, typename head<2ui64, sorted_map_type>::type>::type;
	using third_in_map = typename rest<2ui64, sorted_map_type>::type;

	static_assert(sorted_map_type::length == 3);
	EXPECT_EQ(1, find_value_or<first_in_map>(RANGE(cat), 0));
	EXPECT_EQ(2, find_value_or<second_in_map>(RANGE(dog), 0));
	EXPECT_EQ(3, find_value_or<third_in_map>(RANGE(monkey), 0));

	const auto small_bc = make_bit_function_classifier_function(options_a);
	const auto small_ls = make_linear_scan_classifier_function(options_b);
	const auto small_bs = make_binary_search_classifier_function(options_c);
	auto small_bsc = binary_search_classifier();

	small_bsc.add_option(text_identifier{ RANGE(dog), 2 });
	small_bsc.add_option(text_identifier{ RANGE(cat), 1 });
	small_bsc.add_option(text_identifier{ RANGE(monkey), 3 });

	EXPECT_EQ((unsigned char)1, small_bc(RANGE(cat)));
	EXPECT_EQ((unsigned char)1, small_ls(RANGE(cat)));
	EXPECT_EQ((unsigned char)1, small_bs(RANGE(cat)));
	EXPECT_EQ((unsigned char)1, small_bsc.classify(RANGE(cat)));
	EXPECT_EQ(1, find_value_or<map_type>(RANGE(cat), 0));
	EXPECT_EQ(1, bt_find_value_or<sorted_map_type>(RANGE(cat), 0));


	EXPECT_EQ((unsigned char)2, small_bc(RANGE(dog)));
	EXPECT_EQ((unsigned char)2, small_ls(RANGE(dog)));
	EXPECT_EQ((unsigned char)2, small_bs(RANGE(dog)));
	EXPECT_EQ((unsigned char)2, small_bsc.classify(RANGE(dog)));
	EXPECT_EQ(2, find_value_or<map_type>(RANGE(dog), 0));
	EXPECT_EQ(2, bt_find_value_or<sorted_map_type>(RANGE(dog), 0));

	EXPECT_EQ((unsigned char)3, small_bc(RANGE(monkey)));
	EXPECT_EQ((unsigned char)3, small_ls(RANGE(monkey)));
	EXPECT_EQ((unsigned char)3, small_bs(RANGE(monkey)));
	EXPECT_EQ((unsigned char)3, small_bsc.classify(RANGE(monkey)));
	EXPECT_EQ(3, find_value_or<map_type>(RANGE(monkey), 0));
	EXPECT_EQ(3, bt_find_value_or<sorted_map_type>(RANGE(monkey), 0));

	const char text[] = "text";

	EXPECT_EQ((unsigned char)0, small_bc(RANGE(text)));
	EXPECT_EQ((unsigned char)0, small_ls(RANGE(text)));
	EXPECT_EQ((unsigned char)0, small_bs(RANGE(text)));
	EXPECT_EQ((unsigned char)0, small_bsc.classify(RANGE(text)));
	EXPECT_EQ(0, find_value_or<map_type>(RANGE(text), 0));
	EXPECT_EQ(0, bt_find_value_or<sorted_map_type>(RANGE(text), 0));

	const char empty[] = "";

	EXPECT_EQ((unsigned char)0, small_bc(RANGE(empty)));
	EXPECT_EQ((unsigned char)0, small_ls(RANGE(empty)));
	EXPECT_EQ((unsigned char)0, small_bs(RANGE(empty)));
	EXPECT_EQ((unsigned char)0, small_bsc.classify(RANGE(empty)));
	EXPECT_EQ(0, find_value_or<map_type>(RANGE(empty), 0));
	EXPECT_EQ(0, bt_find_value_or<sorted_map_type>(RANGE(empty), 0));
}


TEST_METHOD(medium_classifier_test, text_classifier_tests) {
	std::vector<text_identifier> options_a = { text_identifier{ RANGE("longer_identifier"), 11 }, text_identifier{ RANGE("train_track"), 10 }, text_identifier{ RANGE("big_pool"), 9 }, text_identifier{ RANGE("pool"), 8 }, text_identifier{ RANGE("train"), 7 }, text_identifier{ RANGE("bool"), 6 }, text_identifier{ RANGE("car"), 5 }, text_identifier{ RANGE("egg"), 4 }, text_identifier{ RANGE("dog"), 2 }, text_identifier{ RANGE("cat"), 1 }, text_identifier{ RANGE("monkey"), 3 }, };
	std::vector<text_identifier> options_b = { text_identifier{ RANGE("longer_identifier"), 11 }, text_identifier{ RANGE("train_track"), 10 }, text_identifier{ RANGE("big_pool"), 9 }, text_identifier{ RANGE("pool"), 8 }, text_identifier{ RANGE("train"), 7 },text_identifier{ RANGE("bool"), 6 }, text_identifier{ RANGE("car"), 5 }, text_identifier{ RANGE("egg"), 4 }, text_identifier{ RANGE("dog"), 2 }, text_identifier{ RANGE("cat"), 1 }, text_identifier{ RANGE("monkey"), 3 }, };
	std::vector<text_identifier> options_c = { text_identifier{ RANGE("longer_identifier"), 11 }, text_identifier{ RANGE("train_track"), 10 }, text_identifier{ RANGE("big_pool"), 9 }, text_identifier{ RANGE("pool"), 8 }, text_identifier{ RANGE("train"), 7 },text_identifier{ RANGE("bool"), 6 }, text_identifier{ RANGE("car"), 5 }, text_identifier{ RANGE("egg"), 4 }, text_identifier{ RANGE("dog"), 2 }, text_identifier{ RANGE("cat"), 1 }, text_identifier{ RANGE("monkey"), 3 }, };
	std::vector<text_identifier> options_d = { text_identifier{ RANGE("longer_identifier"), 11 }, text_identifier{ RANGE("train_track"), 10 }, text_identifier{ RANGE("big_pool"), 9 }, text_identifier{ RANGE("pool"), 8 }, text_identifier{ RANGE("train"), 7 },text_identifier{ RANGE("bool"), 6 }, text_identifier{ RANGE("car"), 5 }, text_identifier{ RANGE("egg"), 4 }, text_identifier{ RANGE("dog"), 2 }, text_identifier{ RANGE("cat"), 1 }, text_identifier{ RANGE("monkey"), 3 }, };

	using map_type = type_list<CT_STRING_INT("longer_identifier", 11), CT_STRING_INT("train_track", 10), CT_STRING_INT("big_pool", 9),
		CT_STRING_INT("pool", 8), CT_STRING_INT("big_pool", 9), CT_STRING_INT("train", 7), CT_STRING_INT("bool", 6),
		CT_STRING_INT("car", 5), CT_STRING_INT("egg", 4), CT_STRING_INT("dog", 2), CT_STRING_INT("cat", 1), CT_STRING_INT("monkey", 3)
	>;
	using sorted_map_type = typename sorted<map_type>::type;

	//using first_in_map = sorted_map_type::head<1>::type;
	//using second_in_map = sorted_map_type::rest<1>::type::head<1>::type;
	//using third_in_map = sorted_map_type::rest<2>::type::head<1>::type;

	const auto small_bc = make_bit_function_classifier_function(options_a);
	const auto small_ls = make_linear_scan_classifier_function(options_b);
	const auto small_bs = make_binary_search_classifier_function(options_c);
	const auto small_bsc = binary_search_classifier(options_d);

	EXPECT_EQ((unsigned char)11, small_bc(RANGE("longer_identifier")));
	EXPECT_EQ((unsigned char)11, small_ls(RANGE("longer_identifier")));
	EXPECT_EQ((unsigned char)11, small_bs(RANGE("longer_identifier")));
	EXPECT_EQ((unsigned char)11, small_bsc.classify(RANGE("longer_identifier")));
	EXPECT_EQ(11, find_value_or<map_type>(RANGE("longer_identifier"), 0));
	EXPECT_EQ(11, bt_find_value_or<sorted_map_type>(RANGE("longer_identifier"), 0));

	EXPECT_EQ((unsigned char)2, small_bc(RANGE("dog")));
	EXPECT_EQ((unsigned char)2, small_ls(RANGE("dog")));
	EXPECT_EQ((unsigned char)2, small_bs(RANGE("dog")));
	EXPECT_EQ((unsigned char)2, small_bsc.classify(RANGE("dog")));
	EXPECT_EQ(2, find_value_or<map_type>(RANGE("dog"), 0));
	EXPECT_EQ(2, bt_find_value_or<sorted_map_type>(RANGE("dog"), 0));

	EXPECT_EQ((unsigned char)9, small_bc(RANGE("big_pool")));
	EXPECT_EQ((unsigned char)9, small_ls(RANGE("big_pool")));
	EXPECT_EQ((unsigned char)9, small_bs(RANGE("big_pool")));
	EXPECT_EQ((unsigned char)9, small_bsc.classify(RANGE("big_pool")));
	EXPECT_EQ(9, find_value_or<map_type>(RANGE("big_pool"), 0));
	EXPECT_EQ(9, bt_find_value_or<sorted_map_type>(RANGE("big_pool"), 0));

	EXPECT_EQ((unsigned char)0, small_bc(RANGE("text")));
	EXPECT_EQ((unsigned char)0, small_ls(RANGE("text")));
	EXPECT_EQ((unsigned char)0, small_bs(RANGE("text")));
	EXPECT_EQ((unsigned char)0, small_bsc.classify(RANGE("text")));
	EXPECT_EQ(0, find_value_or<map_type>(RANGE("text"), 0));
	EXPECT_EQ(0, bt_find_value_or<sorted_map_type>(RANGE("text"), 0));

	EXPECT_EQ((unsigned char)8, small_bc(RANGE("pool")));
	EXPECT_EQ((unsigned char)8, small_ls(RANGE("pool")));
	EXPECT_EQ((unsigned char)8, small_bs(RANGE("pool")));
	EXPECT_EQ((unsigned char)8, small_bsc.classify(RANGE("pool")));
	EXPECT_EQ(8, find_value_or<map_type>(RANGE("pool"), 0));
	EXPECT_EQ(8, bt_find_value_or<sorted_map_type>(RANGE("pool"), 0));

	EXPECT_EQ((unsigned char)8, small_bc(RANGE("POOL")));
	EXPECT_EQ((unsigned char)8, small_ls(RANGE("POOL")));
	EXPECT_EQ((unsigned char)8, small_bs(RANGE("POOL")));
	EXPECT_EQ((unsigned char)8, small_bsc.classify(RANGE("POOL")));
	EXPECT_EQ(8, find_value_or<map_type>(RANGE("POOL"), 0));
	EXPECT_EQ(8, bt_find_value_or<sorted_map_type>(RANGE("POOL"), 0));
}
