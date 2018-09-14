#include "pch.h"
#include "object_parsing\\object_parsing.hpp"
#include "Parsers\\parsers.hpp"
#include <numeric>

static char description_a[] = "a = exists\n"
"b = yes\n"
"d = yes\n";

static char description_b[] =
"string_a\n"
"\n"
"string_b string_c\n";

static char description_c[] = "10 string 11 string 14";

static char description_d[] =
"a = yes\n"
"x = yes\n"
"c = { ignore ignore ignore } "
"c { ignore ignore ignore } "
"{ ignore ignore { } ignore } "
"b = yes\n"
"d = yes\n";

static char description_e[] =
"a = yes\n"
"x = yes\n"
"k = { 1 2 3 } "
"b = yes\n"
"d = yes\n";

static char description_f[] =
"a = yes\n"
"x = yes\n"
"k = { c = yes\n"
" k = {}\n"
" } \n"
"k = { a = yes }\n"
"b = yes\n"
"d = yes\n";

static char description_g[] =
"set_a = {5 empty = {} }\r\n"
"set_b = {1 2 3}";


#define RANGE(x) (x), (x) + ((sizeof(x)) / sizeof((x)[0])) - 1

class three_bool {
public:
	bool a = false;
	bool b = false;
	bool c = false;
	bool d = false;

	void set_b(bool i) { b = i; }
};

MEMBER_DEF(three_bool, a, "a")
MEMBER_DEF(three_bool, b, "b")
MEMBER_DEF(three_bool, c, "c")
MEMBER_DEF(three_bool, d, "d")
MEMBER_FDEF(three_bool, set_b, "alt-b")

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

inline std::pair<association_type, int> rh_int_pair_from_sum(const token_and_type&, association_type a, const int_vector_summation& t) {
	return std::make_pair(a, t.sum);
};

inline bool accept_all_classifier(const char*, const char*) {
	return true;
}

inline bool is_positive_int(const char* s, const char* e) {
	return parse_int(s, e) > 0;
}

struct empty_sub {
	int v;
	empty_sub(int value) : v(value) {}
};

struct variable_named_set {
	token_and_type name;
	std::vector<int> v;
	int value;

	variable_named_set(const token_and_type& t, int) {
		name = t;
	}
	void add_int(int i) {
		v.push_back(i);
	}
	void add_empty(const empty_sub& e) {
		v.push_back(e.v);
	}
};

MEMBER_FDEF(variable_named_set, add_int, "int")
MEMBER_FDEF(variable_named_set, add_empty, "empty")

inline variable_named_set&& get_set(const token_and_type&, association_type, variable_named_set&& t) {
	return std::move(t);
};

struct variable_named_set_container {
	std::vector<variable_named_set> set_of_sets;

	variable_named_set_container(int) {}
	void add_set(variable_named_set&& vin) {
		set_of_sets.emplace_back(std::move(vin));
	}
};

MEMBER_FDEF(variable_named_set_container, add_set, "set")

struct extern_reader {
	three_bool m_a;
	std::vector<variable_named_set> m_b;

	extern_reader(int) {}
	void add_three_bool(const three_bool& i) {
		m_a = i;
	}
	void add_variable_named_set(variable_named_set&& i) {
		m_b.emplace_back(std::move(i));
	}
};

MEMBER_FDEF(extern_reader, add_three_bool, "three_bool");
MEMBER_FDEF(extern_reader, add_variable_named_set, "variable_named_set");

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
	EMPTY_TYPE(empty_sub)
	BEGIN_TYPE(variable_named_set)
	MEMBER_VARIABLE_ASSOCIATION("int", accept_all_classifier, value_from_lh<int>)
	MEMBER_TYPE_ASSOCIATION("empty", "empty", empty_sub)
	END_TYPE
	BEGIN_TYPE(variable_named_set_container)
	MEMBER_VARIABLE_TYPE_ASSOCIATION("set", accept_all_classifier, variable_named_set, get_set)
	END_TYPE
END_DOMAIN;

inline three_bool three_bool_read(const token_group* s, const token_group* e, int) {
	return parse_object<three_bool, test_domain>(s, e);
}
inline variable_named_set variable_named_set_read(const token_group* s, const token_group* e, const token_and_type& n, int v) {
	return parse_object<variable_named_set, test_domain>(s, e, n, v);
}

BEGIN_DOMAIN(test_domain_b)
BEGIN_TYPE(extern_reader)
MEMBER_TYPE_EXTERN("three_bool", "three_bool", three_bool, three_bool_read)
MEMBER_VARIABLE_TYPE_EXTERN("variable_named_set", accept_all, three_bool, variable_named_set_read)
END_TYPE
END_DOMAIN

static char extern_description[] =
"set_a = {5 empty = {} }\r\n"
"three_bool = {\r\n"
"a = yes\r\n"
"c = yes\r\n"
"}\r\n"
"set_b = {1 2 3}";

#define TEST_METHOD(x, y) TEST(x, y)

TEST(object_parsing_tests, value_helpers) {
	const char five[] = "5";
	const char xxxx[] = "xxxx";

	EXPECT_EQ(5, value_from_rh<int>(association_type::none, token_and_type{ RANGE(five), token_type::unknown }));
	EXPECT_EQ(std::string("5"), value_from_rh<std::string>(association_type::none, token_and_type{ RANGE(five), token_type::unknown }));
	EXPECT_EQ(5, value_from_full_rh<int>(token_and_type{ RANGE(xxxx), token_type::unknown }, association_type::none, token_and_type{ RANGE(five), token_type::unknown }));
	EXPECT_EQ(std::make_pair(association_type::ge, 5), association_and_value_from_rh<int>(association_type::ge, token_and_type{ RANGE(five), token_type::unknown }));
	EXPECT_EQ(std::make_pair(association_type::ge, 5), association_and_value_from_full_rh<int>(token_and_type{ RANGE(xxxx), token_type::unknown }, association_type::ge, token_and_type{ RANGE(five), token_type::unknown }));
	EXPECT_EQ(std::string("xxxx"), value_from_lh<std::string>(token_and_type{ RANGE(xxxx), token_type::unknown }, association_type::none, token_and_type{ RANGE(five), token_type::unknown }));
	const auto lhpair = std::make_pair(std::string("xxxx"), 5);
	const auto rhpair = pair_from_lh_rh<std::string, int>(token_and_type{ RANGE(xxxx), token_type::unknown }, association_type::ge, token_and_type{ RANGE(five), token_type::unknown });
	EXPECT_EQ(lhpair, rhpair);
}

TEST(object_parsing_tests, member_assignment_test) {
	three_bool test_obj;
	test_obj.a = true;

	_set_member<CT_STRING("a"), three_bool>::set(test_obj, false);
	EXPECT_FALSE(test_obj.a);

	_set_member<CT_STRING("alt-b"), three_bool>::set(test_obj, true);
	EXPECT_TRUE(test_obj.b);
}



TEST(object_parsing_tests, compile_time_tagged_member_test_b) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(description_d));

	three_bool results = parse_object<three_bool, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size());

	EXPECT_TRUE(results.a);
	EXPECT_TRUE(results.b);
	EXPECT_TRUE(results.d);
	EXPECT_FALSE(results.c);
}

TEST(object_parsing_tests, compile_time_string_comparison) {
	const char s[] = "rangeLimitMax";
	const auto comparison = compile_time_str_compare_ci<CT_STRING("rangelimitmin")>(RANGE(s));
	EXPECT_NE(0, comparison);
}


TEST_METHOD(object_parsing_tests, compile_time_vector_b) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, "5 9 2");

	std::vector<int> result(parse_object<std::vector<int>, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_EQ(3ui64, result.size());
	EXPECT_EQ(5, result[0]);
	EXPECT_EQ(9, result[1]);
	EXPECT_EQ(2, result[2]);
}


TEST_METHOD(object_parsing_tests, compile_time_tagged_embedded_vector) {
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

TEST_METHOD(object_parsing_tests, compile_time_default_associations) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, "0 <= 5 1 != 2 7 > -1 b = yes");

	association_container result(parse_object<association_container, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_FALSE(result.a);
	EXPECT_TRUE(result.b);
	EXPECT_EQ(2ui64, result.contents.size());
	EXPECT_EQ((int)association_type::ne, (int)result.contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)result.contents[1].first);
	EXPECT_EQ(2, result.contents[0].second);
	EXPECT_EQ(-1, result.contents[1].second);
}


TEST_METHOD(object_parsing_tests, compile_time_default_list_associations) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, "0 <= 5 1 != {2 4 -8} 7 > -1 b = yes");

	association_container result(parse_object<association_container, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_FALSE(result.a);
	EXPECT_TRUE(result.b);
	EXPECT_EQ(2ui64, result.contents.size());
	EXPECT_EQ((int)association_type::list, (int)result.contents[0].first);
	EXPECT_EQ((int)association_type::gt, (int)result.contents[1].first);
	EXPECT_EQ(-2, result.contents[0].second);
	EXPECT_EQ(-1, result.contents[1].second);
}

struct strange_object {
	int stored;
	strange_object(int s) : stored(s) {}

	struct add_to_member {
		strange_object& p;
		add_to_member(strange_object& o) : p(o) {}
		add_to_member& operator=(int i) { p.stored += i; return *this; }
	};

	add_to_member addition_target() { return add_to_member(*this); }
};

MEMBER_DEF(strange_object, addition_target(), "value")

BEGIN_DOMAIN(strange_object_domain)
BEGIN_TYPE(strange_object)
MEMBER_ASSOCIATION("value", "v", value_from_rh<int>)
END_TYPE
END_DOMAIN

TEST_METHOD(object_parsing_tests, parse_with_default_constructor_argument) {

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, "v = 1 v = 3");

	strange_object result = parse_object<strange_object, strange_object_domain>(&parse_results[0], &parse_results[0] + parse_results.size(), 4);

	EXPECT_EQ(8, result.stored);
}

TEST_METHOD(object_parsing_tests, compile_tiime_derived_object_parsing) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, "x = 2.0 svalue = blah");

	base_parse base_result(parse_object<base_parse, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));
	derived_parse derived_result(parse_object<derived_parse, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size()));

	EXPECT_EQ(2.0, base_result.x);
	EXPECT_EQ(2.0, derived_result.x);
	EXPECT_EQ(std::string("blah"), derived_result.svalue);
}


TEST_METHOD(object_parsing_tests, compile_time_tagged_recursive) {
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

TEST_METHOD(object_parsing_tests, tagged_member_test) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, description_a, description_a + sizeof(description_a) - 1);

	object_parser<three_bool> tbparser;
	tbparser.add_case_handler("a", [](three_bool& r, const token_group& , const token_group*, const token_group*) {
		r.a = true;
	});
	tbparser.add_case_handler("b", [](three_bool& r, const token_group& , const token_group*, const token_group*) {
		r.b = true;
	});
	tbparser.add_case_handler("c", [](three_bool& r, const token_group& , const token_group*, const token_group*) {
		r.c = true;
	});
	tbparser.add_case_handler("d", [](three_bool& r, const token_group& , const token_group*, const token_group*) {
		r.d = true;
	});

	three_bool built = tbparser(&parse_results[0], &parse_results[0] + parse_results.size());
	EXPECT_TRUE(built.a);
	EXPECT_TRUE(built.b);
	EXPECT_TRUE(built.d);
	EXPECT_FALSE(built.c);
}



TEST_METHOD(object_parsing_tests, resetted_default_handler) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, description_b, description_b + sizeof(description_b) - 1);

	object_parser<std::vector<std::string>> parse_string_list;
	parse_string_list.reset_default_handler([](std::vector<std::string>& object, const token_group& v, auto , auto ) {
		object.emplace_back(v.token.start, v.token.end);
	});

	const auto vector_results = parse_string_list(&parse_results[0], &parse_results[0] + parse_results.size());
	EXPECT_EQ(3ui64, vector_results.size());
	EXPECT_STREQ("string_a", vector_results[0].c_str());
	EXPECT_STREQ("string_b", vector_results[1].c_str());
	EXPECT_STREQ("string_c", vector_results[2].c_str());
}

TEST_METHOD(object_parsing_tests, special_case_handler) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, description_c, description_c + sizeof(description_c) - 1);

	object_parser<std::vector<std::string>> parse_string_list;
	parse_string_list.add_special_case_handler(
		[](auto s, auto e) { return is_integer(s, e); },
		[](std::vector<std::string>& object, const token_group& v, auto , auto ) {
		object.emplace_back(v.token.start, v.token.end);
	});

	const auto vector_results = parse_string_list(&parse_results[0], &parse_results[0] + parse_results.size());
	EXPECT_EQ(3ui64, vector_results.size());
	EXPECT_STREQ("10", vector_results[0].c_str());
	EXPECT_STREQ("11", vector_results[1].c_str());
	EXPECT_STREQ("14", vector_results[2].c_str());
}

TEST_METHOD(object_parsing_tests, named_object_construction) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, description_g, description_g + sizeof(description_g) - 1);

	variable_named_set_container result = parse_object<variable_named_set_container, test_domain>(&parse_results[0], &parse_results[0] + parse_results.size(), 7);

	static_assert(std::is_constructible_v<variable_named_set, token_and_type, int>);
	static_assert(std::is_constructible_v<variable_named_set, token_and_type, int&>);
	static_assert(std::is_constructible_v<variable_named_set, token_and_type, const int&>);
	static_assert(std::is_constructible_v<variable_named_set, token_and_type, int&&>);
	static_assert(std::is_constructible_v<variable_named_set, const token_and_type&, int>);
	static_assert(std::is_constructible_v<variable_named_set, const token_and_type&, int&>);
	static_assert(std::is_constructible_v<variable_named_set, const token_and_type&, const int&>);
	static_assert(std::is_constructible_v<variable_named_set, const token_and_type&, int&&>);

	EXPECT_EQ(2ui64, result.set_of_sets.size());
	EXPECT_EQ(2ui64, result.set_of_sets[0].v.size());
	EXPECT_EQ(5, result.set_of_sets[0].v[0]);
	EXPECT_EQ(7, result.set_of_sets[0].v[1]);
	EXPECT_TRUE(is_fixed_token_ci(result.set_of_sets[0].name, "set_a"));
	EXPECT_EQ(3ui64, result.set_of_sets[1].v.size());
	EXPECT_EQ(1, result.set_of_sets[1].v[0]);
	EXPECT_EQ(2, result.set_of_sets[1].v[1]);
	EXPECT_EQ(3, result.set_of_sets[1].v[2]);
	EXPECT_TRUE(is_fixed_token_ci(result.set_of_sets[1].name, "set_b"));
}

TEST_METHOD(object_parsing_tests, extern_object_construction) {
	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, extern_description, extern_description + sizeof(extern_description) - 1);

	const auto result = parse_object<extern_reader, test_domain_b>(&parse_results[0], &parse_results[0] + parse_results.size(), 7);

	EXPECT_EQ(true, result.m_a.a);
	EXPECT_EQ(false, result.m_a.b);
	EXPECT_EQ(true, result.m_a.c);
	EXPECT_EQ(false, result.m_a.d);
	EXPECT_EQ(2ui64, result.m_b.size());
	EXPECT_EQ(2ui64, result.m_b[0].v.size());
	EXPECT_EQ(5, result.m_b[0].v[0]);
	EXPECT_EQ(7, result.m_b[0].v[1]);
	EXPECT_TRUE(is_fixed_token_ci(result.m_b[0].name, "set_a"));
	EXPECT_EQ(3ui64, result.m_b[1].v.size());
	EXPECT_EQ(1, result.m_b[1].v[0]);
	EXPECT_EQ(2, result.m_b[1].v[1]);
	EXPECT_EQ(3, result.m_b[1].v[2]);
	EXPECT_TRUE(is_fixed_token_ci(result.m_b[1].name, "set_b"));
}


TEST_METHOD(object_parsing_tests, associated_string_getter) {
	token_group proper_left{ token_and_type{nullptr, nullptr, token_type::unknown }, association_type::eq, 1 };
	token_group im_proper_left{ token_and_type{ nullptr, nullptr, token_type::unknown }, association_type::none, 1 };
	token_group im_proper_left_b{ token_and_type{ nullptr, nullptr, token_type::unknown }, association_type::eq, 0 };

	char token_value[] = "token string";
	token_group right[2] = { token_group{ token_and_type{RANGE(token_value),token_type::unknown }, association_type::none, 0 }, token_group{ token_and_type{ RANGE(token_value),token_type::unknown }, association_type::none, 0 } };

	EXPECT_STREQ(token_value, associated_string(proper_left, right, right + 1).c_str());
	EXPECT_STREQ("", associated_string(proper_left, right, right + 2).c_str());
	EXPECT_STREQ("", associated_string(proper_left, right, right).c_str());
	EXPECT_STREQ("", associated_string(im_proper_left, right, right + 1).c_str());
	EXPECT_STREQ("", associated_string(im_proper_left_b, right, right + 1).c_str());
}
