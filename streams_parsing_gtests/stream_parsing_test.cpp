#include "pch.h"
#include "streams_parsing\\\streams_parsing.hpp"
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

bool bool_from_association(association_type, const token_and_type& t) {
	return parse_bool(t.start, t.end);
};

double double_from_association(association_type, const token_and_type& t) {
	return parse_double(t.start, t.end);
};

std::string string_from_association(association_type, const token_and_type& t) {
	return std::string(t.start, t.end);
};

int int_from_full_association(const token_and_type& t, association_type, const token_and_type&) {
	return parse_int(t.start, t.end);
};

std::pair<association_type, int> rh_int_pair_from_full_association(const token_and_type&, association_type a, const token_and_type& t) {
	return std::make_pair(a, parse_int(t.start, t.end));
};

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
MEMBER_ASSOCIATION("a", "a", bool_from_association)
MEMBER_ASSOCIATION("b", "b", bool_from_association)
MEMBER_ASSOCIATION("c", "c", bool_from_association)
MEMBER_ASSOCIATION("d", "d", bool_from_association)
END_TYPE
BEGIN_TYPE(three_bool_recursive)
MEMBER_ASSOCIATION("a", "a", bool_from_association)
MEMBER_ASSOCIATION("b", "b", bool_from_association)
MEMBER_ASSOCIATION("c", "c", bool_from_association)
MEMBER_ASSOCIATION("d", "d", bool_from_association)
MEMBER_TYPE_ASSOCIATION("k", "k", three_bool_recursive)
END_TYPE
BEGIN_TYPE(std::vector<int>)
MEMBER_VARIABLE_ASSOCIATION("this", accept_all_classifier, int_from_full_association)
END_TYPE
BEGIN_TYPE(three_bool_vector)
MEMBER_ASSOCIATION("a", "a", bool_from_association)
MEMBER_ASSOCIATION("b", "b", bool_from_association)
MEMBER_ASSOCIATION("c", "c", bool_from_association)
MEMBER_ASSOCIATION("d", "d", bool_from_association)
MEMBER_TYPE_ASSOCIATION("k-member", "k", std::vector<int>)
END_TYPE
BEGIN_TYPE(association_container)
MEMBER_ASSOCIATION("a", "a", bool_from_association)
MEMBER_ASSOCIATION("b", "b", bool_from_association)
MEMBER_VARIABLE_ASSOCIATION("contents", is_positive_int, rh_int_pair_from_full_association)
MEMBER_VARIABLE_TYPE_ASSOCIATION("contents", is_positive_int, int_vector_summation, rh_int_pair_from_sum)
END_TYPE
BEGIN_TYPE(int_vector_summation)
MEMBER_VARIABLE_ASSOCIATION("sum", accept_all_classifier, int_from_full_association)
END_TYPE
BEGIN_TYPE(base_parse)
MEMBER_ASSOCIATION("x", "x", double_from_association)
END_TYPE
BEGIN_TYPE(derived_parse)
MEMBER_ASSOCIATION("svalue", "svalue", string_from_association)
INHERIT_FROM(base_parse)
END_TYPE
END_DOMAIN

#define TEST_METHOD(x, y) TEST(x, y)


TEST(compile_time_tagged_member_test, object_parsing_tests) {
	std::vector<three_bool> results;

	make_stream(description_d, tokenize_char_stream_operation(), generic_object_parser<three_bool, test_domain>(), [&results](const three_bool& r) { results.push_back(r); }).yield();

	using this_in_context = type_map_get_t<test_domain, three_bool>;

	static_assert(this_in_context::length == 5);

	using tag_to_function = sorted_t<type_list_get_t<0ui64, this_in_context>>;
	using tag_to_generator = sorted_t<type_list_get_t<1ui64, this_in_context>>;
	using function_to_function = type_list_get_t<2ui64, this_in_context>;
	using function_to_generator = type_list_get_t<3ui64, this_in_context>;

	static_assert(tag_to_function::length == 4);
	static_assert(tag_to_generator::length == 0);
	static_assert(function_to_function::length == 0);
	static_assert(function_to_generator::length == 0);

	bool function_called = false;
	const bool found_in_a = map_call_functions<tag_to_function>::template bt_scan_ci<bool>(RANGE("a"),
		[&function_called](auto lambda_associated) {
		function_called = true;
		return true;
	});

	EXPECT_TRUE(function_called);
	EXPECT_TRUE(found_in_a);

	EXPECT_EQ(1ui64, results.size());
	EXPECT_TRUE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_TRUE(results[0].d);
	EXPECT_FALSE(results[0].c);

}


TEST_METHOD(compile_time_vector, object_parsing_tests) {
	std::vector<int> result;
	make_stream("5 9 2", tokenize_char_stream_operation(), generic_object_parser<std::vector<int>, test_domain>(),
		[&result](std::vector<int>& r) { result = std::move(r); }).yield();
	EXPECT_EQ(3ui64, result.size());
	EXPECT_EQ(5, result[0]);
	EXPECT_EQ(9, result[1]);
	EXPECT_EQ(2, result[2]);
}


TEST_METHOD(compile_time_tagged_embedded_vector, object_parsing_tests) {
	std::vector<three_bool_vector> results;

	using this_in_context = type_map_get_t<test_domain, three_bool_vector>;

	static_assert(this_in_context::length == 5);

	using tag_to_function = sorted_t<type_list_get_t<0ui64, this_in_context>>;
	using tag_to_generator = sorted_t<type_list_get_t<1ui64, this_in_context>>;
	using function_to_function = type_list_get_t<2ui64, this_in_context>;
	using function_to_generator = type_list_get_t<3ui64, this_in_context>;

	static_assert(tag_to_function::length == 4);
	static_assert(tag_to_generator::length == 1);
	static_assert(function_to_function::length == 0);
	static_assert(function_to_generator::length == 0);

	make_stream(description_e, tokenize_char_stream_operation(), generic_object_parser<three_bool_vector, test_domain>(), [&results](three_bool_vector& r) { results.emplace_back(std::move(r)); }).yield();

	EXPECT_EQ(1ui64, results.size());
	EXPECT_TRUE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_TRUE(results[0].d);
	EXPECT_EQ(3ui64, results[0].j.size());
	EXPECT_EQ(1, results[0].j[0]);
	EXPECT_EQ(2, results[0].j[1]);
	EXPECT_EQ(3, results[0].j[2]);
	EXPECT_FALSE(results[0].c);
}


TEST_METHOD(streamed_tagged_member_test, object_parsing_tests) {
	std::vector<three_bool> results;
	object_parsing_definition<three_bool> op;
	op.add_case_handler("a", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.a = true; });
	op.add_case_handler("b", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.b = true; });
	op.add_case_handler("c", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.c = true; });
	op.add_case_handler("d", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.d = true; });
	make_stream(description_a, tokenize_char_stream_operation(), token_to_object_stream_operation<three_bool>(op), [&results](const three_bool& r) { results.push_back(r); }).yield();
	EXPECT_EQ(1ui64, results.size());
	EXPECT_TRUE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_TRUE(results[0].d);
	EXPECT_FALSE(results[0].c);
}

TEST_METHOD(streamed_tagged_member_defaults_test, object_parsing_tests) {
	std::vector<three_bool> results;
	object_parsing_definition<three_bool> op;
	op.add_case_handler("a", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.a = true; });
	op.add_case_handler("b", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.b = true; });
	op.add_case_handler("c", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.c = true; });
	op.add_case_handler("d", [](three_bool& r, const token_and_type&, association_type, const token_and_type&) { r.d = true; });
	make_stream(description_d, tokenize_char_stream_operation(), token_to_object_stream_operation<three_bool>(op), [&results](const three_bool& r) { results.push_back(r); }).yield();
	EXPECT_EQ(1ui64, results.size());
	EXPECT_TRUE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_TRUE(results[0].d);
	EXPECT_FALSE(results[0].c);
}


TEST_METHOD(streamed_tagged_embedded_vector, object_parsing_tests) {
	std::vector<three_bool_vector> results;
	object_parsing_definition<three_bool_vector> op;
	op.add_case_handler("a", [](three_bool_vector& r, const token_and_type&, association_type, const token_and_type&) { r.a = true; });
	op.add_case_handler("b", [](three_bool_vector& r, const token_and_type&, association_type, const token_and_type&) { r.b = true; });
	op.add_case_handler("c", [](three_bool_vector& r, const token_and_type&, association_type, const token_and_type&) { r.c = true; });
	op.add_case_handler("d", [](three_bool_vector& r, const token_and_type&, association_type, const token_and_type&) { r.d = true; });
	op.add_case_handler<std::vector<std::string>>("k",
		vector_of_strings_operation(),
		[](three_bool_vector& r, const token_and_type&, association_type, std::vector<std::string>& s) { r.k = std::move(s); });

	make_stream(description_e, tokenize_char_stream_operation(), token_to_object_stream_operation<three_bool_vector>(op), [&results](three_bool_vector& r) { results.emplace_back(std::move(r)); }).yield();
	EXPECT_EQ(1ui64, results.size());
	EXPECT_TRUE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_TRUE(results[0].d);
	EXPECT_EQ(3ui64, results[0].k.size());
	EXPECT_EQ(std::string("1"), results[0].k[0]);
	EXPECT_EQ(std::string("2"), results[0].k[1]);
	EXPECT_EQ(std::string("3"), results[0].k[2]);
	EXPECT_FALSE(results[0].c);
}


TEST_METHOD(compile_time_default_associations, object_parsing_tests) {
	std::vector<association_container> results;

	using this_in_context = type_map_get_t<test_domain, association_container>;

	static_assert(this_in_context::length == 5);

	using tag_to_function = sorted_t<type_list_get_t<0ui64, this_in_context>>;
	using tag_to_generator = sorted_t<type_list_get_t<1ui64, this_in_context>>;
	using function_to_function = type_list_get_t<2ui64, this_in_context>;
	using function_to_generator = type_list_get_t<3ui64, this_in_context>;

	static_assert(tag_to_function::length == 2);
	static_assert(tag_to_generator::length == 0);
	static_assert(function_to_function::length == 1);
	static_assert(function_to_generator::length == 1);

	make_stream("0 <= 5 1 != 2 7 > -1 b = yes", tokenize_char_stream_operation(), generic_object_parser<association_container, test_domain>(), [&results](association_container& r) { results.emplace_back(std::move(r)); }).yield();

	EXPECT_EQ(1ui64, results.size());
	EXPECT_FALSE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_EQ(2ui64, results[0].contents.size());
	EXPECT_EQ((int)association_type::ne, (int)results[0].contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)results[0].contents[1].first);
	EXPECT_EQ(2, results[0].contents[0].second);
	EXPECT_EQ(-1, results[0].contents[1].second);
}

TEST_METHOD(default_associations, object_parsing_tests) {
	std::vector<association_container> results;
	object_parsing_definition<association_container> op;
	op.add_special_case_handler([](const char* start, const char* end) { return parse_uint(start, end) >= 1; },
		[](association_container& r, const token_and_type&, association_type t, const token_and_type& i) {
		r.contents.emplace_back(t, parse_int(i.start, i.end)); });
	op.add_case_handler("a", [](association_container& r, const token_and_type&, association_type, const token_and_type&) { r.a = true; });
	op.add_case_handler("b", [](association_container& r, const token_and_type&, association_type, const token_and_type&) { r.b = true; });


	make_stream("0 <= 5 1 != 2 7 > -1 b = yes", tokenize_char_stream_operation(), token_to_object_stream_operation<association_container>(op), [&results](association_container& r) { results.emplace_back(std::move(r)); }).yield();

	EXPECT_EQ(1ui64, results.size());
	EXPECT_FALSE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_EQ(2ui64, results[0].contents.size());
	EXPECT_EQ((int)association_type::ne, (int)results[0].contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)results[0].contents[1].first);
	EXPECT_EQ(2, results[0].contents[0].second);
	EXPECT_EQ(-1, results[0].contents[1].second);
}


TEST_METHOD(compile_time_default_list_associations, object_parsing_tests) {
	std::vector<association_container> results;

	make_stream("0 <= 5 1 != {2 4 -8} 7 > -1 b = yes", tokenize_char_stream_operation(), generic_object_parser<association_container, test_domain>(), [&results](association_container& r) { results.emplace_back(std::move(r)); }).yield();

	EXPECT_EQ(1ui64, results.size());
	EXPECT_FALSE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_EQ(2ui64, results[0].contents.size());
	EXPECT_EQ((int)association_type::ne, (int)results[0].contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)results[0].contents[1].first);
	EXPECT_EQ(-2, results[0].contents[0].second);
	EXPECT_EQ(-1, results[0].contents[1].second);
}

TEST_METHOD(default_list_associations, object_parsing_tests) {
	std::vector<association_container> results;
	object_parsing_definition<association_container> op;
	op.add_special_case_handler([](const char* start, const char* end) { return parse_uint(start, end) >= 1; },
		[](association_container& r, const token_and_type&, association_type t, const token_and_type& i) {
		r.contents.emplace_back(t, parse_int(i.start, i.end)); });
	op.add_case_handler<std::vector<int>>("a",
		vector_of_ints_operation(),
		[](association_container& r, const token_and_type&, association_type t, std::vector<int>& i) {
		;
	});
	op.add_special_case_handler<std::vector<int>>([](const char* start, const char* end) { return parse_uint(start, end) >= 1; },
		vector_of_ints_operation(),
		[](association_container& r, const token_and_type&, association_type t, std::vector<int>& i) {
		r.contents.emplace_back(t, std::accumulate(i.begin(), i.end(), 0));
	});
	op.add_case_handler("a", [](association_container& r, const token_and_type&, association_type, const token_and_type&) { r.a = true; });
	op.add_case_handler("b", [](association_container& r, const token_and_type&, association_type, const token_and_type&) { r.b = true; });


	make_stream("0 <= 5 1 != {2 4 -8} 7 > -1 b = yes", tokenize_char_stream_operation(), token_to_object_stream_operation<association_container>(op), [&results](association_container& r) { results.emplace_back(std::move(r)); }).yield();

	EXPECT_EQ(1ui64, results.size());
	EXPECT_FALSE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_EQ(2ui64, results[0].contents.size());
	EXPECT_EQ((int)association_type::ne, (int)results[0].contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)results[0].contents[1].first);
	EXPECT_EQ(-2, results[0].contents[0].second);
	EXPECT_EQ(-1, results[0].contents[1].second);
}

TEST_METHOD(compile_tiime_derived_object_parsing, object_parsing_tests) {
	base_parse base_result;
	derived_parse derived_result;
	make_stream("x = 2.0 svalue = blah", tokenize_char_stream_operation(), generic_object_parser<base_parse, test_domain>(), [&base_result](base_parse& b) { base_result = b; }).yield();
	make_stream("x = 2.0 svalue = blah", tokenize_char_stream_operation(), generic_object_parser<derived_parse, test_domain>(), [&derived_result](derived_parse& b) { derived_result = b; }).yield();

	EXPECT_EQ(2.0, base_result.x);
	EXPECT_EQ(2.0, derived_result.x);
	EXPECT_EQ(std::string("blah"), derived_result.svalue);
}

TEST_METHOD(derived_object_parsing, object_parsing_tests) {
	object_parsing_definition<base_parse> op_base;
	op_base.add_case_handler("x", [](base_parse& r, const token_and_type&, association_type, const token_and_type& i) {
		r.x = parse_double(i.start, i.end);
	});

	object_parsing_definition<derived_parse> op_derived(op_base);
	op_derived.add_case_handler("svalue", [](derived_parse& r, const token_and_type&, association_type, const token_and_type& i) {
		r.svalue = std::string(i.start, i.end);
	});

	base_parse base_result;
	derived_parse derived_result;
	make_stream("x = 2.0 svalue = blah", tokenize_char_stream_operation(), token_to_object_stream_operation<base_parse>(op_base), [&base_result](base_parse& b) { base_result = b; }).yield();
	make_stream("x = 2.0 svalue = blah", tokenize_char_stream_operation(), token_to_object_stream_operation<derived_parse>(op_derived), [&derived_result](derived_parse& b) { derived_result = b; }).yield();

	EXPECT_EQ(2.0, base_result.x);
	EXPECT_EQ(2.0, derived_result.x);
	EXPECT_EQ(std::string("blah"), derived_result.svalue);
}

TEST_METHOD(compile_time_tagged_recursive, object_parsing_tests) {
	std::vector<three_bool_recursive> results;


	make_stream(description_f, tokenize_char_stream_operation(), generic_object_parser<three_bool_recursive, test_domain>(), [&results](three_bool_recursive& r) { results.emplace_back(std::move(r)); }).yield();

	EXPECT_EQ(1ui64, results.size());
	EXPECT_TRUE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_TRUE(results[0].d);
	EXPECT_EQ(2ui64, results[0].k.size());
	EXPECT_TRUE(results[0].k[0].c);
	EXPECT_FALSE(results[0].k[0].a);
	EXPECT_FALSE(results[0].k[0].b);

	EXPECT_EQ(1ui64, results[0].k[0].k.size());

	EXPECT_FALSE(results[0].k[0].k[0].c);
	EXPECT_FALSE(results[0].k[0].k[0].a);
	EXPECT_FALSE(results[0].k[0].k[0].b);
	EXPECT_EQ(0ui64, results[0].k[0].k[0].k.size());

	EXPECT_FALSE(results[0].k[1].c);
	EXPECT_TRUE(results[0].k[1].a);
	EXPECT_FALSE(results[0].k[1].b);
	EXPECT_EQ(0ui64, results[0].k[1].k.size());

	EXPECT_FALSE(results[0].c);
}

TEST_METHOD(streamed_tagged_recursive, object_parsing_tests) {
	std::vector<three_bool_recursive> results;
	object_parsing_definition<three_bool_recursive> op;
	op.add_case_handler("a", [](three_bool_recursive& r, const token_and_type&, association_type, const token_and_type&) { r.a = true; });
	op.add_case_handler("b", [](three_bool_recursive& r, const token_and_type&, association_type, const token_and_type&) { r.b = true; });
	op.add_case_handler("c", [](three_bool_recursive& r, const token_and_type&, association_type, const token_and_type&) { r.c = true; });
	op.add_case_handler("d", [](three_bool_recursive& r, const token_and_type&, association_type, const token_and_type&) { r.d = true; });
	op.add_case_handler<three_bool_recursive>(
		"k",
		token_to_object_stream_operation<three_bool_recursive>(op),
		[](three_bool_recursive& r, const token_and_type&, association_type, three_bool_recursive& s) {
		r.k.emplace_back(std::move(s));
	});

	make_stream(description_f, tokenize_char_stream_operation(), token_to_object_stream_operation<three_bool_recursive>(op), [&results](three_bool_recursive& r) { results.emplace_back(std::move(r)); }).yield();
	EXPECT_EQ(1ui64, results.size());
	EXPECT_TRUE(results[0].a);
	EXPECT_TRUE(results[0].b);
	EXPECT_TRUE(results[0].d);
	EXPECT_EQ(2ui64, results[0].k.size());
	EXPECT_TRUE(results[0].k[0].c);
	EXPECT_FALSE(results[0].k[0].a);
	EXPECT_FALSE(results[0].k[0].b);

	EXPECT_EQ(1ui64, results[0].k[0].k.size());

	EXPECT_FALSE(results[0].k[0].k[0].c);
	EXPECT_FALSE(results[0].k[0].k[0].a);
	EXPECT_FALSE(results[0].k[0].k[0].b);
	EXPECT_EQ(0ui64, results[0].k[0].k[0].k.size());

	EXPECT_FALSE(results[0].k[1].c);
	EXPECT_TRUE(results[0].k[1].a);
	EXPECT_FALSE(results[0].k[1].b);
	EXPECT_EQ(0ui64, results[0].k[1].k.size());

	EXPECT_FALSE(results[0].c);
}

TEST_METHOD(basic_tokenizing_stream, parsers_test) {
	std::vector<token_and_type> results;
	make_stream("aaa b ccc", tokenize_char_stream_operation(), [&results](token_and_type& in) { results.push_back(in); }).yield();

	EXPECT_EQ(3ui64, results.size());
	EXPECT_EQ(std::string("aaa"), std::string(results[0].start, results[0].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[0].type);
	EXPECT_EQ(std::string("b"), std::string(results[1].start, results[1].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[1].type);
	EXPECT_EQ(std::string("ccc"), std::string(results[2].start, results[2].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[2].type);
}

TEST_METHOD(basic_tokenizing_stream_special_ident, parsers_test) {
	std::vector<token_and_type> results;
	make_stream("aaa<=b\nccc", tokenize_char_stream_operation(), [&results](token_and_type& in) { results.push_back(in); }).yield();

	EXPECT_EQ(4ui64, results.size());
	EXPECT_EQ(std::string("aaa"), std::string(results[0].start, results[0].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[0].type);
	EXPECT_EQ(std::string("<="), std::string(results[1].start, results[1].end));
	EXPECT_EQ((int)token_type::special_identifier, (int)results[1].type);
	EXPECT_EQ(std::string("b"), std::string(results[2].start, results[2].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[2].type);
	EXPECT_EQ(std::string("ccc"), std::string(results[3].start, results[3].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[3].type);
}

TEST_METHOD(basic_tokenizing_stream_quotations, parsers_test) {
	std::vector<token_and_type> results;
	make_stream("  aaa \"b#b\r\n\t\"c cc\"gg", tokenize_char_stream_operation(), [&results](token_and_type& in) { results.push_back(in); }).yield();

	EXPECT_EQ(4ui64, results.size());
	EXPECT_EQ(std::string("aaa"), std::string(results[0].start, results[0].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[0].type);
	EXPECT_EQ(std::string("b#b"), std::string(results[1].start, results[1].end));
	EXPECT_EQ((int)token_type::quoted_string, (int)results[1].type);
	EXPECT_EQ(std::string("c cc"), std::string(results[2].start, results[2].end));
	EXPECT_EQ((int)token_type::quoted_string, (int)results[2].type);
	EXPECT_EQ(std::string("gg"), std::string(results[3].start, results[3].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[3].type);
}

TEST_METHOD(basic_tokenizing_stream_brackets, parsers_test) {
	std::vector<token_and_type> results;
	make_stream("    aaa{b}{!=}", tokenize_char_stream_operation(), [&results](token_and_type& in) { results.push_back(in); }).yield();

	EXPECT_EQ(7ui64, results.size());
	EXPECT_EQ(std::string("aaa"), std::string(results[0].start, results[0].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[0].type);
	EXPECT_EQ(std::string("{"), std::string(results[1].start, results[1].end));
	EXPECT_EQ((int)token_type::brace, (int)results[1].type);
	EXPECT_EQ(std::string("b"), std::string(results[2].start, results[2].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[2].type);
	EXPECT_EQ(std::string("}"), std::string(results[3].start, results[3].end));
	EXPECT_EQ((int)token_type::brace, (int)results[3].type);
	EXPECT_EQ(std::string("{"), std::string(results[4].start, results[4].end));
	EXPECT_EQ((int)token_type::brace, (int)results[4].type);
	EXPECT_EQ(std::string("!="), std::string(results[5].start, results[5].end));
	EXPECT_EQ((int)token_type::special_identifier, (int)results[5].type);
	EXPECT_EQ(std::string("}"), std::string(results[6].start, results[6].end));
	EXPECT_EQ((int)token_type::brace, (int)results[6].type);
}

TEST_METHOD(basic_tokenizing_stream_comments, parsers_test) {
	std::vector<token_and_type> results;
	make_stream("  aaa#comment##\"\nbb   #comment", tokenize_char_stream_operation(), [&results](token_and_type& in) { results.push_back(in); }).yield();

	EXPECT_EQ(2ui64, results.size());
	EXPECT_EQ(std::string("aaa"), std::string(results[0].start, results[0].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[0].type);
	EXPECT_EQ(std::string("bb"), std::string(results[1].start, results[1].end));
	EXPECT_EQ((int)token_type::identifier, (int)results[1].type);
}

TEST_METHOD(basic_tokenizing_stream_braces_matching, parsers_test) {

	int matched_braces_count = 0;
	make_stream("  hhh {} iii} { } jj { } } kkk lll", tokenize_char_stream_operation(), end_brace_finder(),
		[&matched_braces_count](int n) { ++matched_braces_count; }).yield();
	EXPECT_EQ(2, matched_braces_count);
}

TEST_METHOD(string_vector_from_token_stream, parsers_test) {
	std::vector<std::vector<std::string>> all_results;

	make_stream("  hhh { ignore } \"eee\" iii} kkk lll } { }", tokenize_char_stream_operation(), vector_of_strings_operation(),
		[&all_results](std::vector<std::string>& result) { all_results.emplace_back(std::move(result)); }).yield();
	EXPECT_EQ(2ui64, all_results.size());
	EXPECT_EQ(3ui64, all_results[0].size());
	EXPECT_EQ(2ui64, all_results[1].size());
	EXPECT_EQ(std::string("hhh"), all_results[0][0]);
	EXPECT_EQ(std::string("eee"), all_results[0][1]);
	EXPECT_EQ(std::string("iii"), all_results[0][2]);
	EXPECT_EQ(std::string("kkk"), all_results[1][0]);
	EXPECT_EQ(std::string("lll"), all_results[1][1]);
}

TEST_METHOD(int_vector_from_token_stream, parsers_test) {
	std::vector<std::vector<int>> all_results;

	make_stream("  1 { ignore } \"2\" 3} -4 5 } { }", tokenize_char_stream_operation(), vector_of_ints_operation(),
		[&all_results](std::vector<int>& result) { all_results.emplace_back(std::move(result)); }).yield();
	EXPECT_EQ(2ui64, all_results.size());
	EXPECT_EQ(3ui64, all_results[0].size());
	EXPECT_EQ(2ui64, all_results[1].size());
	EXPECT_EQ(1, all_results[0][0]);
	EXPECT_EQ(2, all_results[0][1]);
	EXPECT_EQ(3, all_results[0][2]);
	EXPECT_EQ(-4, all_results[1][0]);
	EXPECT_EQ(5, all_results[1][1]);
}

TEST_METHOD(double_vector_from_token_stream, parsers_test) {
	std::vector<std::vector<double>> all_results;

	make_stream("  1.5 { ignore } \"2\" 3.5} -4 5.5 } { }", tokenize_char_stream_operation(), vector_of_doubles_operation(),
		[&all_results](std::vector<double>& result) { all_results.emplace_back(std::move(result)); }).yield();
	EXPECT_EQ(2ui64, all_results.size());
	EXPECT_EQ(3ui64, all_results[0].size());
	EXPECT_EQ(2ui64, all_results[1].size());
	EXPECT_EQ(1.5, all_results[0][0]);
	EXPECT_EQ(2.0, all_results[0][1]);
	EXPECT_EQ(3.5, all_results[0][2]);
	EXPECT_EQ(-4.0, all_results[1][0]);
	EXPECT_EQ(5.5, all_results[1][1]);
}