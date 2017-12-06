#include "pch.h"
#include "Parsers\\parsers.hpp"
#include <Windows.h>
#include <vector>
#include <string>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

bool inverse_ignorable(char n) {
	return !ignorable_char(n);
}

#define TEST_METHOD(x, y) TEST(x, y)
#define AreEqual EXPECT_EQ
#define IsTrue EXPECT_TRUE
#define IsFalse EXPECT_FALSE

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST_METHOD(empty_file, parsers_test) {
	char tfile[] = "";
	std::vector<token_group> results;
	parse_pdx_file(results, tfile, tfile + sizeof(tfile) - 1);
	AreEqual(results.size(), size_t(0));
}


TEST_METHOD(position_advancement, parsers_test) {
	char str1[] = " \ttest=  ";
	AreEqual(9Ui64, sizeof(str1) - 1);

	char* r1 = advance_position_to_non_whitespace(str1, str1 + sizeof(str1) - 1);
	AreEqual(r1, str1 + 2);

	char* r2 = advance_position_to_breaking_char(r1, str1 + sizeof(str1) - 1);
	AreEqual(*r2, '=');

	char* r3 = advance_position_to_breaking_char(r2 + 1, str1 + sizeof(str1) - 1);
	AreEqual(r3, str1 + 7);

	char* r4 = advance_position_to_non_whitespace(r3, str1 + sizeof(str1) - 1);
	AreEqual(r4, str1 + sizeof(str1) - 1);

	char* r5 = advance_position_to_non_whitespace(r4, str1 + sizeof(str1) - 1);
	AreEqual(r5, str1 + sizeof(str1) - 1);

	char* r6 = advance_position_to_breaking_char(r5, str1 + sizeof(str1) - 1);
	AreEqual(r6, str1 + sizeof(str1) - 1);

	char str2[] = "ends_file";
	char* r7 = advance_position_to_non_whitespace(str2, str2 + sizeof(str2) - 1);
	AreEqual(r7, str2);

	char* r8 = advance_position_to_breaking_char(str2, str2 + sizeof(str2) - 1);
	AreEqual(r8, str2 + sizeof(str2) - 1);
}

TEST_METHOD(newline_advancement, parsers_test) {
	char line[] = "1: line\n2: line\r\n";

	char* r2 = advance_position_to_next_line(line, line + sizeof(line) - 1);
	AreEqual('2', *r2);

	char* r3 = advance_position_to_next_line(r2, line + sizeof(line) - 1);
	AreEqual(line + sizeof(line) - 1, r3);

	char* r4 = advance_position_to_next_line(r3, line + sizeof(line) - 1);
	AreEqual(line + sizeof(line) - 1, r4);
}

TEST_METHOD(char_classes, parsers_test) {
	AreEqual(true, ignorable_char(' '));
	AreEqual(true, ignorable_char('\r'));
	AreEqual(true, ignorable_char('\n'));
	AreEqual(true, ignorable_char('\t'));
	AreEqual(true, ignorable_char(','));
	AreEqual(false, ignorable_char('t'));
	AreEqual(false, ignorable_char('#'));
	AreEqual(false, ignorable_char('}'));

	AreEqual(true, breaking_char('}'));
	AreEqual(true, breaking_char('{'));
	AreEqual(true, breaking_char('#'));
	AreEqual(true, breaking_char('\"'));
	AreEqual(true, breaking_char('\''));
	AreEqual(true, breaking_char('<'));
	AreEqual(true, breaking_char('>'));
	AreEqual(true, breaking_char('='));
	AreEqual(true, breaking_char('!'));
	AreEqual(true, breaking_char(' '));
	AreEqual(true, breaking_char('\n'));
	AreEqual(true, breaking_char('\t'));
	AreEqual(true, breaking_char('\r'));
	AreEqual(false, breaking_char('_'));
	AreEqual(false, breaking_char('y'));
	AreEqual(false, breaking_char('.'));
	AreEqual(false, breaking_char('-'));

	AreEqual(true, line_termination('\r'));
	AreEqual(true, line_termination('\n'));
	AreEqual(false, line_termination('\t'));
	AreEqual(false, line_termination(' '));
	AreEqual(false, line_termination('x'));

	AreEqual(false, double_quote_termination('\''));
	AreEqual(false, double_quote_termination('k'));
	AreEqual(false, double_quote_termination(' '));
	AreEqual(false, double_quote_termination('\t'));
	AreEqual(true, double_quote_termination('\n'));
	AreEqual(true, double_quote_termination('\r'));
	AreEqual(true, double_quote_termination('\"'));

	AreEqual(true, single_quote_termination('\''));
	AreEqual(false, single_quote_termination('k'));
	AreEqual(false, single_quote_termination(' '));
	AreEqual(false, single_quote_termination('\t'));
	AreEqual(true, single_quote_termination('\n'));
	AreEqual(true, single_quote_termination('\r'));
	AreEqual(false, single_quote_termination('\"'));
}

TEST_METHOD(numeric_parsing, parsers_test) {
	AreEqual(1.5f, parse_float(RANGE("1.5")));
	AreEqual(1.5, parse_double(RANGE("1.5")));
	AreEqual(-1.5f, parse_float(RANGE("-1.5")));
	AreEqual(-1.5, parse_double(RANGE("-1.5")));
	AreEqual(0.0f, parse_float(RANGE("0")));
	AreEqual(0.0, parse_double(RANGE("0")));
	AreEqual(0.0f, parse_float(RANGE("x")));
	AreEqual(0.0, parse_double(RANGE("x")));

	AreEqual(4, parse_int(RANGE("4")));
	AreEqual(4ui32, parse_uint(RANGE("4")));
	AreEqual(0, parse_int(RANGE("0")));
	AreEqual(0ui32, parse_uint(RANGE("0")));
	AreEqual(-4, parse_int(RANGE("-4")));
	AreEqual(0ui32, parse_uint(RANGE("-4")));
	AreEqual(1, parse_int(RANGE("1.5")));
	AreEqual(1ui32, parse_uint(RANGE("1.5")));
	AreEqual(0, parse_int(RANGE("x")));
	AreEqual(0ui32, parse_uint(RANGE("x")));
}

TEST_METHOD(bool_parsing, parsers_test) {
	IsTrue(parse_bool(RANGE("yes")));
	IsTrue(parse_bool(RANGE("Yes")));
	IsTrue(parse_bool(RANGE("1")));
	IsTrue(parse_bool(RANGE("y")));
	IsTrue(parse_bool(RANGE("Y")));

	IsFalse(parse_bool(RANGE("no")));
	IsFalse(parse_bool(RANGE("No")));
	IsFalse(parse_bool(RANGE("0")));
	IsFalse(parse_bool(RANGE("n")));
	IsFalse(parse_bool(RANGE("N")));

	IsFalse(parse_bool(RANGE("")));
	IsFalse(parse_bool(RANGE("xxx")));
}

TEST_METHOD(token_parsing, parsers_test) {
	EXPECT_EQ(1.5, token_to<double>(token_and_type{ RANGE("1.5"), token_type::unknown }));
	EXPECT_EQ(1.5f, token_to<float>(token_and_type{ RANGE("1.5"), token_type::unknown }));
	EXPECT_EQ(3, token_to<int>(token_and_type{ RANGE("3"), token_type::unknown }));
	EXPECT_EQ(3ui32, token_to<uint32_t>(token_and_type{ RANGE("3"), token_type::unknown }));
	EXPECT_EQ(std::string("string"), token_to<std::string>(token_and_type{ RANGE("string"), token_type::unknown }));
	EXPECT_EQ(true, token_to<bool>(token_and_type{ RANGE("yes"), token_type::unknown }));
}

TEST_METHOD(number_classification, parsers_test) {
	char pos_integer[] = "36";
	char neg_integer[] = "-6934";
	char deformed_neg[] = "-";
	char empty_string[] = "";
	char int_with_text[] = "1234hu";
	char text_only[] = "text24";
	char pos_fp[] = "04.587";
	char neg_fp[] = "-.5367";
	char neg_fp_b[] = "-8.5367";
	char small_fp[] = ".246";
	char double_ptp[] = "0.235.56";
	char trailing_pt[] = "45.";
	char single_pt[] = ".";
	char extra_neg[] = "45.-354";
	char dbl_neg_integer[] = "--6934";

	AreEqual(true, is_positive_integer(RANGE(pos_integer)));
	AreEqual(false, is_positive_integer(RANGE(neg_integer)));
	AreEqual(false, is_positive_integer(RANGE(deformed_neg)));
	AreEqual(false, is_positive_integer(RANGE(empty_string)));
	AreEqual(false, is_positive_integer(RANGE(int_with_text)));
	AreEqual(false, is_positive_integer(RANGE(text_only)));
	AreEqual(false, is_positive_integer(RANGE(pos_fp)));
	AreEqual(false, is_positive_integer(RANGE(neg_fp)));
	AreEqual(false, is_positive_integer(RANGE(small_fp)));
	AreEqual(false, is_positive_integer(RANGE(double_ptp)));
	AreEqual(false, is_positive_integer(RANGE(trailing_pt)));
	AreEqual(false, is_positive_integer(RANGE(single_pt)));
	AreEqual(false, is_positive_integer(RANGE(extra_neg)));
	AreEqual(false, is_positive_integer(RANGE(dbl_neg_integer)));

	AreEqual(true, is_integer(RANGE(pos_integer)));
	AreEqual(true, is_integer(RANGE(neg_integer)));
	AreEqual(false, is_integer(RANGE(deformed_neg)));
	AreEqual(false, is_integer(RANGE(empty_string)));
	AreEqual(false, is_integer(RANGE(int_with_text)));
	AreEqual(false, is_integer(RANGE(text_only)));
	AreEqual(false, is_integer(RANGE(pos_fp)));
	AreEqual(false, is_integer(RANGE(neg_fp)));
	AreEqual(false, is_integer(RANGE(small_fp)));
	AreEqual(false, is_integer(RANGE(double_ptp)));
	AreEqual(false, is_integer(RANGE(trailing_pt)));
	AreEqual(false, is_integer(RANGE(single_pt)));
	AreEqual(false, is_integer(RANGE(extra_neg)));
	AreEqual(false, is_integer(RANGE(dbl_neg_integer)));

	AreEqual(true, is_positive_fp(RANGE(pos_integer)));
	AreEqual(false, is_positive_fp(RANGE(neg_integer)));
	AreEqual(false, is_positive_fp(RANGE(deformed_neg)));
	AreEqual(false, is_positive_fp(RANGE(empty_string)));
	AreEqual(false, is_positive_fp(RANGE(int_with_text)));
	AreEqual(false, is_positive_fp(RANGE(text_only)));
	AreEqual(true, is_positive_fp(RANGE(pos_fp)));
	AreEqual(false, is_positive_fp(RANGE(neg_fp)));
	AreEqual(false, is_positive_fp(RANGE(neg_fp_b)));
	AreEqual(true, is_positive_fp(RANGE(small_fp)));
	AreEqual(false, is_positive_fp(RANGE(double_ptp)));
	AreEqual(true, is_positive_fp(RANGE(trailing_pt)));
	AreEqual(false, is_positive_fp(RANGE(single_pt)));
	AreEqual(false, is_positive_fp(RANGE(extra_neg)));
	AreEqual(false, is_positive_fp(RANGE(dbl_neg_integer)));

	AreEqual(true, is_fp(RANGE(pos_integer)));
	AreEqual(true, is_fp(RANGE(neg_integer)));
	AreEqual(false, is_fp(RANGE(deformed_neg)));
	AreEqual(false, is_fp(RANGE(empty_string)));
	AreEqual(false, is_fp(RANGE(int_with_text)));
	AreEqual(false, is_fp(RANGE(text_only)));
	AreEqual(true, is_fp(RANGE(pos_fp)));
	AreEqual(true, is_fp(RANGE(neg_fp)));
	AreEqual(true, is_fp(RANGE(neg_fp_b)));
	AreEqual(true, is_fp(RANGE(small_fp)));
	AreEqual(false, is_fp(RANGE(double_ptp)));
	AreEqual(true, is_fp(RANGE(trailing_pt)));
	AreEqual(false, is_fp(RANGE(single_pt)));
	AreEqual(false, is_fp(RANGE(extra_neg)));
	AreEqual(false, is_fp(RANGE(dbl_neg_integer)));
}

TEST_METHOD(advancing, parsers_test) {
	char test_str[] = "-6934";
	char* test_end = test_str + sizeof(test_str) - 1;
	AreEqual(test_str + 3, safe_advance(2, test_str + 1, test_end));
	AreEqual(test_end, safe_advance(30, test_str + 1, test_end));
	AreEqual(test_end, safe_advance(1, test_end, test_end));
}

TEST_METHOD(ignore_comments, parsers_test) {
	char test_str[] = "  #abc\n#def\n\t";
	char* test_end = test_str + sizeof(test_str) - 1;
	AreEqual(test_end, advance_position_to_non_comment(test_str, test_end));

	char test_str2[] = "#abc\n   def #hij";
	char* test2_end = test_str2 + sizeof(test_str2) - 1;
	AreEqual('d', *advance_position_to_non_comment(test_str2, test2_end));

	char test_str3[] = "no comment #comment";
	char* test_str3_end = test_str3 + sizeof(test_str3) - 1;
	AreEqual('n', *advance_position_to_non_comment(test_str3, test_str3_end));
}

TEST_METHOD(ignore_lua_comments, parsers_test) {
	char test_str[] = "  --abc\n--def\n\t";
	char* test_end = test_str + sizeof(test_str) - 1;
	AreEqual(test_end, advance_position_to_non_lua_comment(test_str, test_end));

	char test_str2[] = "--abc\n   def --hij";
	char* test2_end = test_str2 + sizeof(test_str2) - 1;
	AreEqual('d', *advance_position_to_non_lua_comment(test_str2, test2_end));

	char test_str3[] = "no comment --comment";
	char* test_str3_end = test_str3 + sizeof(test_str3) - 1;
	AreEqual('n', *advance_position_to_non_lua_comment(test_str3, test_str3_end));
}

TEST_METHOD(token_comparison, parsers_test) {
	char t1[] = "VAL";
	char t2[] = "val";
	char t3[] = "vAl";

	AreEqual(false, is_fixed_token(RANGE(t1), "v"));
	AreEqual(false, is_fixed_token(RANGE(t1), "V"));
	AreEqual(false, is_fixed_token(RANGE(t1), ""));
	AreEqual(false, is_fixed_token(RANGE(t1), "val"));
	AreEqual(true, is_fixed_token(RANGE(t2), "val"));
	AreEqual(false, is_fixed_token(RANGE(t3), "val"));
	AreEqual(true, is_fixed_token(RANGE(t1), "VAL"));
	AreEqual(false, is_fixed_token(RANGE(t2), "VAL"));
	AreEqual(false, is_fixed_token(RANGE(t3), "VAL"));
	AreEqual(false, is_fixed_token(RANGE(t1), "VALUE"));

	AreEqual(true, is_fixed_token_ci(RANGE(t1), "val"));
	AreEqual(true, is_fixed_token_ci(RANGE(t2), "val"));
	AreEqual(true, is_fixed_token_ci(RANGE(t3), "val"));
	AreEqual(false, is_fixed_token_ci(RANGE(t1), "value"));
	AreEqual(false, is_fixed_token_ci(RANGE(t2), "value"));
	AreEqual(false, is_fixed_token_ci(RANGE(t3), "value"));

	AreEqual(false, is_fixed_token_ci(t1, t1 + 2, "val"));
	AreEqual(false, is_fixed_token_ci(t1, t1 + 2, "VAL"));
	AreEqual(false, is_fixed_token_ci(t1, t1 + 2, "VALUE"));
}

TEST_METHOD(prefix_comparison, parsers_test) {
	char t1[] = "value";
	AreEqual(true, has_fixed_prefix(RANGE(t1), "val"));
	AreEqual(true, has_fixed_prefix(RANGE(t1), "value"));
	AreEqual(false, has_fixed_prefix(RANGE(t1), "value_plus"));
	AreEqual(false, has_fixed_prefix(RANGE(t1), "V"));
	AreEqual(false, has_fixed_prefix(RANGE(t1) - 2, "value"));

	AreEqual(true, starts_with(RANGE(t1), 'v'));
	AreEqual(false, starts_with(RANGE(t1), 'V'));
	AreEqual(false, starts_with(t1, t1, 'v'));
}

TEST_METHOD(consume_token, parsers_test) {
	char test_t1[] = "";
	char test_t2[] = "single_token";
	char test_t3[] = "double=token";
	char test_t4[] = "\"quoted value\" token";
	char test_t5[] = "\'quoted\"value\' token";
	char test_t6[] = "\"malformed quote\ntoken";
	char test_t7[] = "\"malformed quoteb";

	const auto result1 = get_current_token(RANGE(test_t1));

	AreEqual(test_t1, result1.token.start);
	AreEqual(test_t1 + 0, result1.token.end);

	const auto result2 = get_current_token(RANGE(test_t2));

	AreEqual(test_t2, result2.token.start);
	AreEqual(test_t2 + 12, result2.token.end);

	const auto result3 = get_current_token(RANGE(test_t3));

	AreEqual(test_t3, result3.token.start);
	AreEqual(test_t3 + 6, result3.token.end);

	const auto result4 = get_current_token(RANGE(test_t4));

	AreEqual(test_t4 + 1, result4.token.start);
	AreEqual(test_t4 + 13, result4.token.end);
	AreEqual('\"', *(result4.token.end));

	const auto result5 = get_current_token(RANGE(test_t5));

	AreEqual(test_t5 + 1, result5.token.start);
	AreEqual(test_t5 + 13, result5.token.end);
	AreEqual('\'', *(result5.token.end));

	const auto result6 = get_current_token(RANGE(test_t6));

	AreEqual(test_t6 + 1, result6.token.start);
	AreEqual(test_t6 + 16, result6.token.end);
	AreEqual('\n', *(result6.token.end));

	const auto result7 = get_current_token(RANGE(test_t7));

	AreEqual(test_t7 + 1, result7.token.start);
	AreEqual(test_t7 + sizeof(test_t7) - 1, result7.token.end);
}

TEST_METHOD(association_types, parsers_test) {
	char t1[] = "<= junk";
	char t2[] = "<sdff";
	char t3[] = "==adfsf";
	char t4[] = "={";
	char t5[] = ">";
	char t6[] = ">=";
	char t7[] = "!=";
	char t8[] = "<>";
	char t9[] = "0123";
	char t10[] = " >=";

	const auto r1 = parse_association_type(RANGE(t1));
	AreEqual(r1.first, association_type::le);
	AreEqual(r1.second, t1 + 2);

	const auto r2 = parse_association_type(RANGE(t2));
	AreEqual(r2.first, association_type::lt);
	AreEqual(r2.second, t2 + 1);

	const auto r3 = parse_association_type(RANGE(t3));
	AreEqual(r3.first, association_type::eq);
	AreEqual(r3.second, t3 + 2);

	const auto r4 = parse_association_type(RANGE(t4));
	AreEqual(r4.first, association_type::eq_default);
	AreEqual(r4.second, t4 + 1);

	const auto r5 = parse_association_type(RANGE(t5));
	AreEqual(r5.first, association_type::gt);
	AreEqual(r5.second, t5 + 1);

	const auto r6 = parse_association_type(RANGE(t6));
	AreEqual(r6.first, association_type::ge);
	AreEqual(r6.second, t6 + 2);

	const auto r7 = parse_association_type(RANGE(t7));
	AreEqual(r7.first, association_type::ne);
	AreEqual(r7.second, t7 + 2);

	const auto r8 = parse_association_type(RANGE(t8));
	AreEqual(r8.first, association_type::ne);
	AreEqual(r8.second, t8 + 2);

	const auto r9 = parse_association_type(RANGE(t9));
	AreEqual(r9.first, association_type::none);
	AreEqual(r9.second, t9);

	const auto r10 = parse_association_type(RANGE(t10));
	AreEqual(r10.first, association_type::none);
	AreEqual(r10.second, t10);
}

TEST_METHOD(association_types_b, parsers_test) {
	char t1[] = "<= junk";
	char t2[] = "<sdff";
	char t3[] = "==adfsf";
	char t4[] = "={";
	char t5[] = ">";
	char t6[] = ">=";
	char t7[] = "!=";
	char t8[] = "<>";
	char t9[] = "0123";
	char t10[] = " >=";

	AreEqual(parse_association_type_b(RANGE(t1)), association_type::le);
	AreEqual(parse_association_type_b(RANGE(t2)), association_type::lt);
	AreEqual(parse_association_type_b(RANGE(t3)), association_type::eq);
	AreEqual(parse_association_type_b(RANGE(t4)), association_type::eq_default);
	AreEqual(parse_association_type_b(RANGE(t5)), association_type::gt);
	AreEqual(parse_association_type_b(RANGE(t6)), association_type::ge);
	AreEqual(parse_association_type_b(RANGE(t7)), association_type::ne);
	AreEqual(parse_association_type_b(RANGE(t8)), association_type::ne);
	AreEqual(parse_association_type_b(RANGE(t9)), association_type::none);
	AreEqual(parse_association_type_b(RANGE(t10)), association_type::none);
}

TEST_METHOD(unnested_token_lists, parsers_test) {
	char empty[] = "";

	std::vector<token_group> r1_list;
	const auto r1 = parse_token_list(r1_list, RANGE(empty));

	AreEqual(0ui64, r1_list.size());
	AreEqual(empty, r1);


	char emptyb[] = " \n    ";

	std::vector<token_group> r2_list;
	const auto r2 = parse_token_list(r2_list, RANGE(emptyb));

	AreEqual(0ui64, r2_list.size());
	AreEqual(emptyb + sizeof(emptyb) - 1, r2);

	char flat_simple[] = " aaa b \"cc ccc\"\tddd    ";

	std::vector<token_group> r3_list;
	const auto r3 = parse_token_list(r3_list, RANGE(flat_simple));

	AreEqual(4ui64, r3_list.size());
	AreEqual(flat_simple + sizeof(flat_simple) - 1, r3);
	IsTrue(is_fixed_token(r3_list[0].token, "aaa"));
	IsTrue(is_fixed_token(r3_list[1].token, "b"));
	IsTrue(is_fixed_token(r3_list[2].token, "cc ccc"));
	IsTrue(is_fixed_token(r3_list[3].token, "ddd"));

	char flat_terminated[] = " aaa b \"cc ccc\"}ddd    ";

	std::vector<token_group> r4_list;
	const auto r4 = parse_token_list(r4_list, RANGE(flat_terminated));

	AreEqual(3ui64, r4_list.size());
	AreEqual(flat_terminated + 16, r4);
	IsTrue(is_fixed_token(r4_list[0].token, "aaa"));
	IsTrue(is_fixed_token(r4_list[1].token, "b"));
	IsTrue(is_fixed_token(r4_list[2].token, "cc ccc"));
}

TEST_METHOD(comments, parsers_test) {

	char flat_simple[] = " aaa b \"cc ccc\r\n"
		"#}\tddd    \r\n"
		"eee";

	std::vector<token_group> r3_list;
	const auto r3 = parse_token_list(r3_list, RANGE(flat_simple));

	AreEqual(4ui64, r3_list.size());
	AreEqual(flat_simple + sizeof(flat_simple) - 1, r3);
	IsTrue(is_fixed_token(r3_list[0].token, "aaa"));
	IsTrue(is_fixed_token(r3_list[1].token, "b"));
	IsTrue(is_fixed_token(r3_list[2].token, "cc ccc"));
	IsTrue(is_fixed_token(r3_list[3].token, "eee"));
}

TEST_METHOD(lua_comments, parsers_test) {

	char flat_simple[] = " aaa b \"cc ccc\r\n"
		"--}\tddd    \r\n"
		"eee";

	std::vector<token_group> r3_list;
	const auto r3 = parse_lua_token_list(r3_list, RANGE(flat_simple));

	AreEqual(4ui64, r3_list.size());
	AreEqual(flat_simple + sizeof(flat_simple) - 1, r3);
	IsTrue(is_fixed_token(r3_list[0].token, "aaa"));
	IsTrue(is_fixed_token(r3_list[1].token, "b"));
	IsTrue(is_fixed_token(r3_list[2].token, "cc ccc"));
	IsTrue(is_fixed_token(r3_list[3].token, "eee"));
}

TEST_METHOD(typed_associated, parsers_test) {
	char associtive_pair_eq[] = "value=0.5";

	std::vector<token_group> r1_list;
	const auto r1 = parse_token_list(r1_list, RANGE(associtive_pair_eq));

	AreEqual(2ui64, r1_list.size());
	AreEqual(associtive_pair_eq + sizeof(associtive_pair_eq) - 1, r1);
	IsTrue(is_fixed_token(r1_list[0].token, "value"));
	IsTrue(is_fixed_token(r1_list[1].token, "0.5"));
	AreEqual(association_type::eq_default, r1_list[0].association);
	AreEqual(association_type::none, r1_list[1].association);
	AreEqual(1ui16, r1_list[0].group_size);
	AreEqual(0ui16, r1_list[1].group_size);

	char associtive_pair_le[] = "value <= 0.5";

	std::vector<token_group> r2_list;
	const auto r2 = parse_token_list(r2_list, RANGE(associtive_pair_le));

	AreEqual(2ui64, r2_list.size());
	AreEqual(associtive_pair_le + sizeof(associtive_pair_le) - 1, r2);
	IsTrue(is_fixed_token(r2_list[0].token, "value"));
	IsTrue(is_fixed_token(r2_list[1].token, "0.5"));
	AreEqual(association_type::le, r2_list[0].association);
	AreEqual(association_type::none, r2_list[1].association);
	AreEqual(1ui16, r2_list[0].group_size);
	AreEqual(0ui16, r2_list[1].group_size);

	char associtive_pair_auto[] = "value {0.5}";

	std::vector<token_group> r3_list;
	const auto r3 = parse_token_list(r3_list, RANGE(associtive_pair_auto));

	AreEqual(2ui64, r3_list.size());
	AreEqual(associtive_pair_auto + sizeof(associtive_pair_auto) - 1, r3);
	IsTrue(is_fixed_token(r3_list[0].token, "value"));
	IsTrue(is_fixed_token(r3_list[1].token, "0.5"));
	AreEqual(association_type::list, r3_list[0].association);
	AreEqual(association_type::none, r3_list[1].association);
	AreEqual(1ui16, r3_list[0].group_size);
	AreEqual(0ui16, r3_list[1].group_size);
}

TEST_METHOD(unassociated_list, parsers_test) {
	char associtive_pair_eq[] = "{0.5}";

	std::vector<token_group> r1_list;
	const auto r1 = parse_token_list(r1_list, RANGE(associtive_pair_eq));

	AreEqual(2ui64, r1_list.size());
	AreEqual(associtive_pair_eq + sizeof(associtive_pair_eq) - 1, r1);
	EXPECT_STREQ(std::string(r1_list[0].token.start, r1_list[0].token.end).c_str(), "");
	EXPECT_STREQ(std::string(r1_list[1].token.start, r1_list[1].token.end).c_str(), "0.5");
	AreEqual(association_type::list, r1_list[0].association);
	AreEqual(association_type::none, r1_list[1].association);
	AreEqual(1ui16, r1_list[0].group_size);
	AreEqual(0ui16, r1_list[1].group_size);
}

TEST_METHOD(empty_associated_list, parsers_test) {
	char associtive_pair_eq[] = "key ={ \r\n }";

	std::vector<token_group> r1_list;
	const auto r1 = parse_token_list(r1_list, RANGE(associtive_pair_eq));

	AreEqual(1ui64, r1_list.size());
	AreEqual(associtive_pair_eq + sizeof(associtive_pair_eq) - 1, r1);
	IsTrue(is_fixed_token(r1_list[0].token, "key"));
	AreEqual(association_type::list, r1_list[0].association);
	AreEqual(0ui16, r1_list[0].group_size);
}

TEST_METHOD(associated_list, parsers_test) {
	char associtive_pair_eq[] = "key ={value \"b b\" \r\n c}";

	std::vector<token_group> r1_list;
	const auto r1 = parse_token_list(r1_list, RANGE(associtive_pair_eq));

	AreEqual(4ui64, r1_list.size());
	AreEqual(associtive_pair_eq + sizeof(associtive_pair_eq) - 1, r1);
	IsTrue(is_fixed_token(r1_list[0].token, "key"));
	IsTrue(is_fixed_token(r1_list[1].token, "value"));
	IsTrue(is_fixed_token(r1_list[2].token, "b b"));
	IsTrue(is_fixed_token(r1_list[3].token, "c"));
	AreEqual(association_type::list, r1_list[0].association);
	AreEqual(association_type::none, r1_list[1].association);
	AreEqual(association_type::none, r1_list[2].association);
	AreEqual(association_type::none, r1_list[3].association);
	AreEqual(3ui16, r1_list[0].group_size);
	AreEqual(0ui16, r1_list[1].group_size);
	AreEqual(0ui16, r1_list[2].group_size);
	AreEqual(0ui16, r1_list[3].group_size);
}

TEST_METHOD(associated_list_of_pairs, parsers_test) {
	char associtive_pair_eq[] = "key ={value = \"b b\" \r\n c > 0}";

	std::vector<token_group> r1_list;
	const auto r1 = parse_token_list(r1_list, RANGE(associtive_pair_eq));

	AreEqual(5ui64, r1_list.size());
	AreEqual(associtive_pair_eq + sizeof(associtive_pair_eq) - 1, r1);
	IsTrue(is_fixed_token(r1_list[0].token, "key"));
	IsTrue(is_fixed_token(r1_list[1].token, "value"));
	IsTrue(is_fixed_token(r1_list[2].token, "b b"));
	IsTrue(is_fixed_token(r1_list[3].token, "c"));
	IsTrue(is_fixed_token(r1_list[4].token, "0"));
	AreEqual(association_type::list, r1_list[0].association);
	AreEqual(association_type::eq_default, r1_list[1].association);
	AreEqual(association_type::none, r1_list[2].association);
	AreEqual(association_type::gt, r1_list[3].association);
	AreEqual(association_type::none, r1_list[2].association);
	AreEqual(4ui16, r1_list[0].group_size);
	AreEqual(1ui16, r1_list[1].group_size);
	AreEqual(0ui16, r1_list[2].group_size);
	AreEqual(1ui16, r1_list[3].group_size);
	AreEqual(0ui16, r1_list[4].group_size);
}

TEST_METHOD(nested_lists, parsers_test) {

	char nlist[] = "l1 = { # level1\r\n"
		"\tv1 = a\r\n"
		"v2 = {\r\n"
		"v2_1 = s1\r\n"
		"v2_2 = s2\r\n"
		"\textra,#whoops\r\n"
		"}}\r\n"
		"l2 = 6";

	std::vector<token_group> r3_list;
	const auto r3 = parse_token_list(r3_list, RANGE(nlist));

	AreEqual(11ui64, r3_list.size());
	AreEqual(nlist + sizeof(nlist) - 1, r3);
	IsTrue(is_fixed_token(r3_list[0].token, "l1"));
	IsTrue(is_fixed_token(r3_list[1].token, "v1"));
	IsTrue(is_fixed_token(r3_list[2].token, "a"));
	IsTrue(is_fixed_token(r3_list[3].token, "v2"));
	IsTrue(is_fixed_token(r3_list[4].token, "v2_1"));
	IsTrue(is_fixed_token(r3_list[5].token, "s1"));
	IsTrue(is_fixed_token(r3_list[6].token, "v2_2"));
	IsTrue(is_fixed_token(r3_list[7].token, "s2"));
	IsTrue(is_fixed_token(r3_list[8].token, "extra"));
	IsTrue(is_fixed_token(r3_list[9].token, "l2"));
	IsTrue(is_fixed_token(r3_list[10].token, "6"));
	AreEqual(8ui16, r3_list[0].group_size);
	AreEqual(1ui16, r3_list[1].group_size);
	AreEqual(0ui16, r3_list[2].group_size);
	AreEqual(5ui16, r3_list[3].group_size);
	AreEqual(1ui16, r3_list[4].group_size);
	AreEqual(0ui16, r3_list[5].group_size);
	AreEqual(1ui16, r3_list[6].group_size);
	AreEqual(0ui16, r3_list[7].group_size);
	AreEqual(0ui16, r3_list[8].group_size);
	AreEqual(1ui16, r3_list[9].group_size);
	AreEqual(0ui16, r3_list[10].group_size);
}

TEST_METHOD(element_if_forall_and_find, parsers_test) {

	char nlist[] = "l1 = { # level1\r\n"
		"\tv1 = a\r\n"
		"v2 = {\r\n"
		"v2_1 = s1\r\n"
		"v2_2 = s2\r\n"
		"\textra,#whoops\r\n"
		"}}\r\n"
		"l2 = 6";

	std::vector<token_group> r3_list;
	parse_token_list(r3_list, RANGE(nlist));

	IsFalse(any_token(&r3_list[0], &r3_list[0] + 11,
							  [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return is_fixed_token(n.token, "s1"); }));
	IsTrue(all_tokens(&r3_list[0], &r3_list[0] + 11,
							  [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return n.association == association_type::list || n.association == association_type::eq_default; }));
	IsTrue(any_token_recursive(&r3_list[0], &r3_list[0] + 11,
									   [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return is_fixed_token(n.token, "s1"); }));
	IsFalse(all_tokens_recursive(&r3_list[0], &r3_list[0] + 11,
										 [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return n.association == association_type::eq_default; }));
	IsTrue(&r3_list[5] == find_token_recursive(&r3_list[0], &r3_list[0] + 11,
													   [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return is_fixed_token(n.token, "s1"); }));
	IsTrue(&r3_list[0] + 11 == find_token(&r3_list[0], &r3_list[0] + 11,
												  [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return is_fixed_token(n.token, "s1"); }));

	IsTrue(all_tokens(&r3_list[0], &r3_list[0] + 11,
							  [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return child_start != child_end; }));
	IsFalse(all_tokens_recursive(&r3_list[0], &r3_list[0] + 11,
										 [](const token_group &n, const token_group *child_start, const token_group *child_end) {
		return child_start != child_end; }));

	int countall = 0;
	int countlevel = 0;
	forall_tokens_recursive(&r3_list[0], &r3_list[0] + 11,
							[&countall](const token_group &n, const token_group *child_start, const token_group *child_end) {++countall; });
	AreEqual(11, countall);
	forall_tokens(&r3_list[0], &r3_list[0] + 11,
				  [&countlevel](const token_group &n, const token_group *child_start, const token_group *child_end) {++countlevel; });
	AreEqual(2, countlevel);
}

TEST_METHOD(parse_lua_float, parsers_test) {
	char nlist[] = "diplomacy = {\r\n"
		"\tPRESTIGE_REDUCTION_BASE = 25,\r\n"
		"\tREPARATIONS_YEARS = 5,\r\n"
		"\t\r\n"
		"\t--No longer used :\r\n"
		"\t--PO_CONCEDE_DEFEAT_P\r\n"
		"\tPRESTIGE_REDUCTION = 0.1, --Base value + % of recipient's prestige\r\n"
		"},\r\n"
		"\r\n"
		"pops = {}";

	bool float_found = false;
	int diplomacy_count = 0;
	std::vector<token_group> r3_list;
	parse_lua_file(r3_list, RANGE(nlist));

	forall_tokens(&r3_list[0], &r3_list[0] + r3_list.size(),
				  [&float_found, &diplomacy_count](const token_group &n, token_group *child_start, token_group *child_end) {
		if (is_fixed_token(n.token, "diplomacy")) {
			forall_tokens(child_start, child_end, [&float_found, &diplomacy_count](token_group &n, token_group *child_start, const token_group *child_end) {
				++diplomacy_count;
				if (is_fixed_token_ci(n.token, "prestige_reduction")) {
					if (child_start != child_end) {
						float_found = is_fp(child_start->token.start, child_start->token.end);
					}
				}
			});
		}});
	AreEqual(3, diplomacy_count);
	IsTrue(float_found);
}

TEST_METHOD(csv_non_transformative_advance_t, parsers_test) {
	char t1[] = "value start;value 2;;";
	char t2[] = "";
	char t3[] = "\r\nvalue;value";
	char t4[] = ";";
	char t5[] = "\"value; \nquoted\";next value";
	char t6[] = "\"value; \"\"quoted\";next value";
	char t7[] = "value\nvalue";

	AreEqual(t1 + 12, csv_advance(RANGE(t1), ';'));
	AreEqual(t2, csv_advance(RANGE(t2), ';'));
	AreEqual(t3, csv_advance(RANGE(t3), ';'));
	AreEqual(t4 + 1, csv_advance(RANGE(t4), ';'));
	AreEqual(t5 + 17, csv_advance(RANGE(t5), ';'));
	AreEqual(t6 + 18, csv_advance(RANGE(t6), ';'));
	AreEqual(t7 + 5, csv_advance(RANGE(t7), ';'));
}

TEST_METHOD(csv_non_transformative_quote_advance_t, parsers_test) {
	char t1[] = "blah blah; blah\" post quote";
	char t2[] = "";
	char t3[] = "\r\n\"\"\t;;\" post quote";

	AreEqual(t1 + 16, csv_advdance_to_quote_end(RANGE(t1)));
	AreEqual(t2, csv_advdance_to_quote_end(RANGE(t2)));
	AreEqual(t3 + 8, csv_advdance_to_quote_end(RANGE(t3)));
}
TEST_METHOD(csv_transformative_get_value, parsers_test) {
	char t1[] = "value start;value 2;;";
	char t2[] = "";
	char t3[] = "\r\nvalue;value";
	char t4[] = ";";
	char t5[] = "\"value; \nquoted\";next value";
	char t6[] = "\"value; \"\"quoted\";next value";
	char t7[] = "value\nvalue";
	char t8[] = "\"value\"\nvalue";
	char t9[] = "\"\"value\nvalue";

	const auto r1 = csv_consume_token(RANGE(t1), ';');
	AreEqual(t1 + 12, r1.second);
	*(r1.first) = '\0';
	EXPECT_STREQ("value start", t1);

	const auto r2 = csv_consume_token(RANGE(t2), ';');
	AreEqual(t2, r2.second);
	AreEqual(t2, r2.first);

	const auto r3 = csv_consume_token(RANGE(t3), ';');
	AreEqual(t3, r3.second);
	AreEqual(t3, r3.first);

	const auto r4 = csv_consume_token(RANGE(t4), ';');
	AreEqual(t4 + 1, r4.second);
	AreEqual(t4, r4.first);

	const auto r5 = csv_consume_token(RANGE(t5), ';');
	AreEqual(t5 + 17, r5.second);
	*(r5.first) = '\0';
	EXPECT_STREQ("value; \nquoted", t5);

	const auto r6 = csv_consume_token(RANGE(t6), ';');
	AreEqual(t6 + 18, r6.second);
	*(r6.first) = '\0';
	EXPECT_STREQ("value; \"quoted", t6);

	const auto r7 = csv_consume_token(RANGE(t7), ';');
	AreEqual(t7 + 5, r7.second);
	*(r7.first) = '\0';
	EXPECT_STREQ("value", t7);

	const auto r8 = csv_consume_token(RANGE(t8), ';');
	AreEqual(t8 + 7, r8.second);
	*(r8.first) = '\0';
	EXPECT_STREQ("value", t8);

	const auto r9 = csv_consume_token(RANGE(t9), ';');
	AreEqual(t9 + 7, r9.second);
	*(r9.first) = '\0';
	EXPECT_STREQ("\"value", t9);
}

TEST_METHOD(csv_advance_to_nextline, parsers_test) {
	char t1[] = "single line";
	char t2[] = "two\nlines";
	char t3[] = "\r\nvalue;value";
	char t4[] = "start \"\" middle\nline2";
	char t5[] = "lead\"value; \nquoted\";\nthe next value";
	char t6[] = "lead\"value; \nquoted\";\n###junk;;;\n##\nthe next value";

	AreEqual(t1 + 11, csv_advance_to_next_line(RANGE(t1)));
	AreEqual(t2 + 4, csv_advance_to_next_line(RANGE(t2)));
	AreEqual(t3 + 2, csv_advance_to_next_line(RANGE(t3)));
	AreEqual(t4 + 16, csv_advance_to_next_line(RANGE(t4)));
	AreEqual(t5 + 22, csv_advance_to_next_line(RANGE(t5)));
	AreEqual(t6 + 36, csv_advance_to_next_line(RANGE(t6)));
}

TEST_METHOD(csv_fixed_quantity_parse, parsers_test) {
	char lines[] = "line1a;\"line1;b\";value c;extra\n"
		"line2a;line2\"\"b;value f";
	const auto line_range = std::make_pair(RANGE(lines));
	const auto next = parse_fixed_amount_csv_values<3>(lines, line_range.second, ';', [](std::pair<char*, char*> result[3]) {
		*(result[0].second) = '\0';
		*(result[1].second) = '\0';
		*(result[2].second) = '\0';
		EXPECT_STREQ("line1a", result[0].first);
		EXPECT_STREQ("line1;b", result[1].first);
		EXPECT_STREQ("value c", result[2].first);
	});
	const auto last = parse_fixed_amount_csv_values<3>(next, line_range.second, ';', [](std::pair<char*, char*> result[3]) {
		*(result[0].second) = '\0';
		*(result[1].second) = '\0';
		*(result[2].second) = '\0';
		EXPECT_STREQ("line2a", result[0].first);
		EXPECT_STREQ("line2\"b", result[1].first);
		EXPECT_STREQ("value f", result[2].first);
	});
	AreEqual(line_range.second, last);
}

TEST_METHOD(csv_variable_quantity_parse, parsers_test) {
	char lines[] = "line1a;\"line1;b\";value c;extra\n"
		"line2a;line2\"\"b;value f";
	const auto line_range = std::make_pair(RANGE(lines));
	const auto next = parse_first_and_nth_csv_values(3, lines, line_range.second, ';', [](std::pair<char*, char*> a, std::pair<char*, char*> b) {
		*(a.second) = '\0';
		*(b.second) = '\0';
		EXPECT_STREQ("line1a", a.first);
		EXPECT_STREQ("value c", b.first);
	});
	const auto last = parse_first_and_nth_csv_values(3, next, line_range.second, ';', [](std::pair<char*, char*> a, std::pair<char*, char*> b) {
		*(a.second) = '\0';
		*(b.second) = '\0';
		EXPECT_STREQ("line2a", a.first);
		EXPECT_STREQ("value f", b.first);
	});
	AreEqual(line_range.second, last);
}