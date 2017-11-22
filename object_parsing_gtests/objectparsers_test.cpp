#include "pch.h"
#include "object_parsing\\object_parsing.hpp"
#include "Parsers\\parsers.hpp"
#include <numeric>

char description_a[] = "a = exists\n"
"b = yes\n"
"d = yes\n";

char description_b[] =
"string_a\n"
"\n"
"string_b string_c\n";

char description_c[] = "10 string 11 string 14";

char description_d[] =
"a = yes\n"
"x = yes\n"
"c = { ignore ignore ignore } "
"c { ignore ignore ignore } "
"{ ignore ignore { } ignore } "
"b = yes\n"
"d = yes\n";

char description_e[] =
"a = yes\n"
"x = yes\n"
"k = { 1 2 3 } "
"b = yes\n"
"d = yes\n";

char description_f[] =
"a = yes\n"
"x = yes\n"
"k = { c = yes\n"
" k = {}\n"
" } \n"
"k = { a = yes }\n"
"b = yes\n"
"d = yes\n";

#define RANGE(x) (x), (x) + ((sizeof(x)) / sizeof((x)[0])) - 1

class three_bool {
public:
	bool a = false;
	bool b = false;
	bool c = false;
	bool d = false;
};

MEMBER_DEF(three_bool, a, "a")
MEMBER_DEF(three_bool, b, "b")
MEMBER_DEF(three_bool, c, "c")
MEMBER_DEF(three_bool, d, "d")

class three_bool_vector {
public:
	bool a = false;
	bool b = false;
	bool c = false;
	bool d = false;
	std::vector<int> j;
	std::vector<std::string> k;
};

MEMBER_DEF(three_bool_vector, a, "a")
MEMBER_DEF(three_bool_vector, b, "b")
MEMBER_DEF(three_bool_vector, c, "c")
MEMBER_DEF(three_bool_vector, d, "d")
MEMBER_DEF(three_bool_vector, j, "k-member")

class three_bool_recursive {
public:
	bool a = false;
	bool b = false;
	bool c = false;
	bool d = false;
	std::vector<three_bool_recursive> k;
};

MEMBER_DEF(three_bool_recursive, a, "a")
MEMBER_DEF(three_bool_recursive, b, "b")
MEMBER_DEF(three_bool_recursive, c, "c")
MEMBER_DEF(three_bool_recursive, d, "d")
MEMBER_DEF(three_bool_recursive, k, "k")

class association_container {
public:
	bool a = false;
	bool b = false;
	std::vector<std::pair<association_type, int>> contents;
};

MEMBER_DEF(association_container, a, "a")
MEMBER_DEF(association_container, b, "b")
MEMBER_DEF(association_container, contents, "contents")

class int_vector_summation {
public:
	class sum_assignment {
	public:
		int& sum;
		sum_assignment(int& s) : sum(s) {}
		sum_assignment& operator=(int x) { sum += x; return *this; }
	};
	int sum = 0;
	auto get_sum_destination() {
		return sum_assignment(sum);
	}
};

MEMBER_DEF(int_vector_summation, get_sum_destination(), "sum")

struct base_parse {
	double x = 0.0;
};

MEMBER_DEF(base_parse, x, "x")

struct derived_parse : public base_parse {
	std::string svalue;
};

MEMBER_DEF(derived_parse, x, "x")
MEMBER_DEF(derived_parse, svalue, "svalue")


std::pair<association_type, int> rh_int_pair_from_sum(const token_and_type&, association_type a, int_vector_summation& t) {
	return std::make_pair(a, t.sum);
};

bool accept_all_classifier(const char*, const char*) {
	return true;
}

bool is_positive_int(const char* s, const char* e) {
	return parse_int(s, e) > 0;
}

BEGIN_DOMAIN(test_domain)
	BEGIN_TYPE(three_bool)
		MEMBER_ASSOCIATION("a", "a", value_from_rh<bool>)
		MEMBER_ASSOCIATION("b", "b", value_from_rh<bool>)
		MEMBER_ASSOCIATION("c", "c", value_from_rh<bool>)
		MEMBER_ASSOCIATION("d", "d", value_from_rh<bool>)
	END_TYPE
	BEGIN_TYPE(three_bool_recursive)
		MEMBER_ASSOCIATION("a", "a", value_from_rh<bool>)
		MEMBER_ASSOCIATION("b", "b", value_from_rh<bool>)
		MEMBER_ASSOCIATION("c", "c", value_from_rh<bool>)
		MEMBER_ASSOCIATION("d", "d", value_from_rh<bool>)
	    MEMBER_TYPE_ASSOCIATION("k", "k", three_bool_recursive)
	END_TYPE
	BEGIN_TYPE(std::vector<int>)
	    MEMBER_VARIABLE_ASSOCIATION("this", accept_all_classifier, value_from_lh<int>)
	END_TYPE
	BEGIN_TYPE(three_bool_vector)
		MEMBER_ASSOCIATION("a", "a", value_from_rh<bool>)
		MEMBER_ASSOCIATION("b", "b", value_from_rh<bool>)
		MEMBER_ASSOCIATION("c", "c", value_from_rh<bool>)
		MEMBER_ASSOCIATION("d", "d", value_from_rh<bool>)
		MEMBER_TYPE_ASSOCIATION("k-member", "k", std::vector<int>)
	END_TYPE
	BEGIN_TYPE(association_container)
		MEMBER_ASSOCIATION("a", "a", value_from_rh<bool>)
		MEMBER_ASSOCIATION("b", "b", value_from_rh<bool>)
		MEMBER_VARIABLE_ASSOCIATION("contents", is_positive_int, association_and_value_from_full_rh<int>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("contents", is_positive_int, int_vector_summation, rh_int_pair_from_sum)
	END_TYPE
	BEGIN_TYPE(int_vector_summation)
	    MEMBER_VARIABLE_ASSOCIATION("sum", accept_all_classifier, value_from_lh<int>)
	END_TYPE
	BEGIN_TYPE(base_parse)
	    MEMBER_ASSOCIATION("x", "x", value_from_rh<double>)
	END_TYPE
	BEGIN_TYPE(derived_parse)
		MEMBER_ASSOCIATION("svalue", "svalue", value_from_rh<std::string>)
		INHERIT_FROM(base_parse)
	END_TYPE
END_DOMAIN

#define TEST_METHOD(x, y) TEST(x, y)

TEST(value_helpers, object_parsing_tests) {
	EXPECT_EQ(5, value_from_rh<int>(association_type::none, token_and_type{ RANGE("5"), token_type::unknown }));
	EXPECT_EQ(std::string("5"), value_from_rh<std::string>(association_type::none, token_and_type{ RANGE("5"), token_type::unknown }));
	EXPECT_EQ(5, value_from_full_rh<int>(token_and_type{ RANGE("xxxx"), token_type::unknown }, association_type::none, token_and_type{ RANGE("5"), token_type::unknown }));
	EXPECT_EQ(std::make_pair(association_type::ge, 5), association_and_value_from_rh<int>(association_type::ge, token_and_type{ RANGE("5"), token_type::unknown }));
	EXPECT_EQ(std::make_pair(association_type::ge, 5), association_and_value_from_full_rh<int>(token_and_type{ RANGE("xxxx"), token_type::unknown }, association_type::ge, token_and_type{ RANGE("5"), token_type::unknown }));
	EXPECT_EQ(std::string("xxxx"), value_from_lh<std::string>(token_and_type{ RANGE("xxxx"), token_type::unknown }, association_type::none, token_and_type{ RANGE("5"), token_type::unknown }));
	const auto lhpair = std::make_pair(std::string("xxxx"), 5);
	const auto rhpair = pair_from_lh_rh<std::string, int>(token_and_type{ RANGE("xxxx"), token_type::unknown }, association_type::ge, token_and_type{ RANGE("5"), token_type::unknown });
	EXPECT_EQ(lhpair, rhpair);
}

TEST(member_assignment_test, object_parsing_tests) {
	three_bool test_obj;
	test_obj.a = true;

	using get_class = _get_member<CT_STRING("a"), three_bool>;
	EXPECT_TRUE(get_class::get(test_obj));
	get_class::get(test_obj) = false;
	EXPECT_FALSE(test_obj.a);

	move_to_member(_get_member<CT_STRING("b"), three_bool>::get(test_obj), true);
	EXPECT_TRUE(test_obj.b);
}



TEST(compile_time_tagged_member_test_b, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(description_d));

	three_bool results = parse_object<three_bool, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size());

	EXPECT_TRUE(results.a);
	EXPECT_TRUE(results.b);
	EXPECT_TRUE(results.d);
	EXPECT_FALSE(results.c);
}


TEST_METHOD(compile_time_vector_b, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE("5 9 2"));

	std::vector<int> result(parse_object<std::vector<int>, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_EQ(3ui64, result.size());
	EXPECT_EQ(5, result[0]);
	EXPECT_EQ(9, result[1]);
	EXPECT_EQ(2, result[2]);
}


TEST_METHOD(compile_time_tagged_embedded_vector, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(description_e));

	three_bool_vector result(parse_object<three_bool_vector, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_TRUE(result.a);
	EXPECT_TRUE(result.b);
	EXPECT_TRUE(result.d);
	EXPECT_EQ(3ui64, result.j.size());
	EXPECT_EQ(1, result.j[0]);
	EXPECT_EQ(2, result.j[1]);
	EXPECT_EQ(3, result.j[2]);
	EXPECT_FALSE(result.c);
}

TEST_METHOD(compile_time_default_associations, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE("0 <= 5 1 != 2 7 > -1 b = yes"));

	association_container result(parse_object<association_container, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_FALSE(result.a);
	EXPECT_TRUE(result.b);
	EXPECT_EQ(2ui64, result.contents.size());
	EXPECT_EQ((int)association_type::ne, (int)result.contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)result.contents[1].first);
	EXPECT_EQ(2, result.contents[0].second);
	EXPECT_EQ(-1, result.contents[1].second);
}


TEST_METHOD(compile_time_default_list_associations, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE("0 <= 5 1 != {2 4 -8} 7 > -1 b = yes"));

	association_container result(parse_object<association_container, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_FALSE(result.a);
	EXPECT_TRUE(result.b);
	EXPECT_EQ(2ui64, result.contents.size());
	EXPECT_EQ((int)association_type::list, (int)result.contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)result.contents[1].first);
	EXPECT_EQ(-2, result.contents[0].second);
	EXPECT_EQ(-1, result.contents[1].second);
}

TEST_METHOD(compile_tiime_derived_object_parsing, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE("x = 2.0 svalue = blah"));

	base_parse base_result(parse_object<base_parse, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));
	derived_parse derived_result(parse_object<derived_parse, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_EQ(2.0, base_result.x);
	EXPECT_EQ(2.0, derived_result.x);
	EXPECT_EQ(std::string("blah"), derived_result.svalue);
}


TEST_METHOD(compile_time_tagged_recursive, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(description_f));

	three_bool_recursive result(parse_object<three_bool_recursive, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_TRUE(result.a);
	EXPECT_TRUE(result.b);
	EXPECT_TRUE(result.d);
	EXPECT_EQ(2ui64, result.k.size());
	EXPECT_TRUE(result.k[0].c);
	EXPECT_FALSE(result.k[0].a);
	EXPECT_FALSE(result.k[0].b);

	EXPECT_EQ(1ui64, result.k[0].k.size());

	EXPECT_FALSE(result.k[0].k[0].c);
	EXPECT_FALSE(result.k[0].k[0].a);
	EXPECT_FALSE(result.k[0].k[0].b);
	EXPECT_EQ(0ui64, result.k[0].k[0].k.size());

	EXPECT_FALSE(result.k[1].c);
	EXPECT_TRUE(result.k[1].a);
	EXPECT_FALSE(result.k[1].b);
	EXPECT_EQ(0ui64, result.k[1].k.size());

	EXPECT_FALSE(result.c);
}

TEST_METHOD(tagged_member_test, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, description_a, description_a + sizeof(description_a) - 1);

	object_parser<three_bool> tbparser;
	tbparser.add_case_handler("a", [](three_bool& r, const token_group& val, const token_group*, const token_group*) {
		r.a = true;
	});
	tbparser.add_case_handler("b", [](three_bool& r, const token_group& val, const token_group*, const token_group*) {
		r.b = true;
	});
	tbparser.add_case_handler("c", [](three_bool& r, const token_group& val, const token_group*, const token_group*) {
		r.c = true;
	});
	tbparser.add_case_handler("d", [](three_bool& r, const token_group& val, const token_group*, const token_group*) {
		r.d = true;
	});

	three_bool built = tbparser(&parse_results[0], &parse_results[0] + parse_results.size());
	EXPECT_TRUE(built.a);
	EXPECT_TRUE(built.b);
	EXPECT_TRUE(built.d);
	EXPECT_FALSE(built.c);
}



TEST_METHOD(resetted_default_handler, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, description_b, description_b + sizeof(description_b) - 1);

	object_parser<std::vector<std::string>> parse_string_list;
	parse_string_list.reset_default_handler([](std::vector<std::string>& object, const token_group& v, auto s, auto e) {
		object.emplace_back(v.token.start, v.token.end);
	});

	const auto vector_results = parse_string_list(&parse_results[0], &parse_results[0] + parse_results.size());
	EXPECT_EQ(3ui64, vector_results.size());
	EXPECT_STREQ("string_a", vector_results[0].c_str());
	EXPECT_STREQ("string_b", vector_results[1].c_str());
	EXPECT_STREQ("string_c", vector_results[2].c_str());
}

TEST_METHOD(special_case_handler, object_parsing_tests) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, description_c, description_c + sizeof(description_c) - 1);

	object_parser<std::vector<std::string>> parse_string_list;
	parse_string_list.add_special_case_handler(
		[](auto s, auto e) { return is_integer(s, e); },
		[](std::vector<std::string>& object, const token_group& v, auto s, auto e) {
		object.emplace_back(v.token.start, v.token.end);
	});

	const auto vector_results = parse_string_list(&parse_results[0], &parse_results[0] + parse_results.size());
	EXPECT_EQ(3ui64, vector_results.size());
	EXPECT_STREQ("10", vector_results[0].c_str());
	EXPECT_STREQ("11", vector_results[1].c_str());
	EXPECT_STREQ("14", vector_results[2].c_str());
}

TEST_METHOD(associated_string_getter, object_parsing_tests) {
	token_group proper_left{ token_and_type{nullptr, nullptr, token_type::unknown }, association_type::eq, 1 };
	token_group im_proper_left{ token_and_type{ nullptr, nullptr, token_type::unknown }, association_type::none, 1 };
	token_group im_proper_left_b{ token_and_type{ nullptr, nullptr, token_type::unknown }, association_type::eq, 0 };

	char token_value[] = "token string";
	token_group right[1] = { token_group{ token_and_type{RANGE(token_value),token_type::unknown }, association_type::none, 0 } };

	EXPECT_STREQ(token_value, associated_string(proper_left, right, right + 1).c_str());
	EXPECT_STREQ("", associated_string(proper_left, right, right + 2).c_str());
	EXPECT_STREQ("", associated_string(proper_left, right, right).c_str());
	EXPECT_STREQ("", associated_string(im_proper_left, right, right + 1).c_str());
	EXPECT_STREQ("", associated_string(im_proper_left_b, right, right + 1).c_str());
}