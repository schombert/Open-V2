#include "text_classifier/text_classifiers.h"
#include "performance_measurement/performance.h"
#include <iostream>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

struct char_pair {
	const char* start;
	const char* end;
};

const std::vector<text_identifier> test_set_a = {
		text_identifier{ RANGE("end"), 1 },
		text_identifier{ RANGE("start"), 2 },
		text_identifier{ RANGE("reform"), 3 },
		text_identifier{ RANGE("history"), 4 },
		text_identifier{ RANGE("icon"), 5 } };

using test_set_a_map = type_list<
	CT_STRING_INT("end", 1),
	CT_STRING_INT("start", 2),
	CT_STRING_INT("reform", 3),
	CT_STRING_INT("history", 4),
	CT_STRING_INT("icon", 5)
>;

using sorted_test_set_a_map = sorted_t<test_set_a_map>;

const std::vector<text_identifier> test_set_b = {
		text_identifier{ RANGE("dictionary"), 1 },
		text_identifier{ RANGE("element"), 2 },
		text_identifier{ RANGE("regiment"), 3 },
		text_identifier{ RANGE("equality"), 4 },
		text_identifier{ RANGE("trigger"), 5 },
		text_identifier{ RANGE("railroad"), 6 },
		text_identifier{ RANGE("aristocrat"), 7 },
		text_identifier{ RANGE("climate"), 8 },
		text_identifier{ RANGE("function"), 9 },
		text_identifier{ RANGE("prestige"), 10 },
		text_identifier{ RANGE("color"), 11 } };

using test_set_b_map = type_list<
	CT_STRING_INT("dictionary", 1),
	CT_STRING_INT("element", 2),
	CT_STRING_INT("regiment", 3),
	CT_STRING_INT("equality", 4),
	CT_STRING_INT("trigger", 5),
	CT_STRING_INT("railroad", 6),
	CT_STRING_INT("aristocrat", 7),
	CT_STRING_INT("climate", 8),
	CT_STRING_INT("function", 9),
	CT_STRING_INT("prestige", 10),
	CT_STRING_INT("color", 11)
>;
using sorted_test_set_b_map = sorted_t<test_set_b_map>;

const std::vector<char_pair> common_test_list = {
		char_pair{ RANGE("tag") },
		char_pair{ RANGE("list") },
		char_pair{ RANGE("ui_element") },
		char_pair{ RANGE("start") },
		char_pair{ RANGE("pool") },
		char_pair{ RANGE("vector") },
		char_pair{ RANGE("element") },
		char_pair{ RANGE("l") },
		char_pair{ RANGE("function") },
		char_pair{ RANGE("23") },
		char_pair{ RANGE("hat") },
		char_pair{ RANGE("dictionary") },
		char_pair{ RANGE("table") },
		char_pair{ RANGE("hash") },
		char_pair{ RANGE("unit") },
		char_pair{ RANGE("production") },
		char_pair{ RANGE("production_type") },
		char_pair{ RANGE("size") },
		char_pair{ RANGE("count") },
		char_pair{ RANGE("color") },
		char_pair{ RANGE("history") },
		char_pair{ RANGE("date") },
		char_pair{ RANGE("end") },
		char_pair{ RANGE("maximum") },
		char_pair{ RANGE("minimum") },
		char_pair{ RANGE("window") },
		char_pair{ RANGE("gui_button") },
		char_pair{ RANGE("scrollbar") },
		char_pair{ RANGE("icon") },
		char_pair{ RANGE("end") },
		char_pair{ RANGE("ship") },
		char_pair{ RANGE("army") },
		char_pair{ RANGE("navy") },
		char_pair{ RANGE("leader") },
		char_pair{ RANGE("province") },
		char_pair{ RANGE("terrain") },
		char_pair{ RANGE("climate") },
		char_pair{ RANGE("owner") },
		char_pair{ RANGE("controller") },
		char_pair{ RANGE("equality") },
		char_pair{ RANGE("event") },
		char_pair{ RANGE("and") },
		char_pair{ RANGE("trigger") },
		char_pair{ RANGE("not") },
		char_pair{ RANGE("iron") },
		char_pair{ RANGE("gold") },
		char_pair{ RANGE("railroad") },
		char_pair{ RANGE("naval_base") },
		char_pair{ RANGE("clergy") },
		char_pair{ RANGE("artisans") },
		char_pair{ RANGE("capitalists") },
		char_pair{ RANGE("soldier") },
		char_pair{ RANGE("laborer") },
		char_pair{ RANGE("farmer") },
		char_pair{ RANGE("miner") },
		char_pair{ RANGE("officer") },
		char_pair{ RANGE("aristocrat") },
		char_pair{ RANGE("clerk") },
		char_pair{ RANGE("slave") },
		char_pair{ RANGE("reform") },
		char_pair{ RANGE("technology") },
		char_pair{ RANGE("attrition") },
		char_pair{ RANGE("supply") },
		char_pair{ RANGE("morale") },
		char_pair{ RANGE("infmay") },
		char_pair{ RANGE("prestige") },
		char_pair{ RANGE("alliance") },
		char_pair{ RANGE("dependency") },
		char_pair{ RANGE("truce") },
		char_pair{ RANGE("great power") } };

class test_movable_fn {
public:
	movable_function<unsigned char, const char*, const char*> function;

	test_movable_fn(movable_function<unsigned char, const char*, const char*>&& s) : function(std::move(s)) {}

	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += function(i.start, i.end);
		}
		return sum;
	}
};

class test_std_fn {
public:
	std::function<unsigned char(const char*, const char*)> function;

	test_std_fn(const std::function<unsigned char(const char*, const char*)> &s) : function(s) {}

	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += function(i.start, i.end);
		}
		return sum;
	}
};

class test_binary_search_ct_a {
public:
	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += bt_find_value_or<sorted_test_set_a_map>(i.start, i.end, 0);
		}
		return sum;
	}
};

class test_binary_search_ct_b {
public:
	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += bt_find_value_or<sorted_test_set_b_map>(i.start, i.end, 0);
		}
		return sum;
	}
};

class test_linear_search_ct_a {
public:
	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += find_value_or<test_set_a_map>(i.start, i.end, 0);
		}
		return sum;
	}
};

class test_linear_search_ct_b {
public:
	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += find_value_or<test_set_b_map>(i.start, i.end, 0);
		}
		return sum;
	}
};


class test_binary_search_classifier_class_a {
public:
	binary_search_classifier bsc;

	test_binary_search_classifier_class_a() : bsc(test_set_a) {}

	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += bsc.classify(i.start, i.end);
		}
		return sum;
	}
};

class test_binary_search_classifier_class_b {
public:
	binary_search_classifier bsc;

	test_binary_search_classifier_class_b() : bsc(test_set_b) {}

	int test_function() {
		int sum = 0;
		for (const auto& i : common_test_list) {
			sum += bsc.classify(i.start, i.end);
		}
		return sum;
	}
};

class ls_a : public test_std_fn {
public:
	ls_a() : test_std_fn(make_linear_scan_classifier_function(std::vector<text_identifier>(test_set_a))) {
	}
};
class ls_b : public test_std_fn {
public:
	ls_b() : test_std_fn(make_linear_scan_classifier_function(std::vector<text_identifier>(test_set_b))) {
	}
};
class bs_a : public test_std_fn {
public:
	bs_a() : test_std_fn(make_binary_search_classifier_function(std::vector<text_identifier>(test_set_a))) {
	}
};
class bs_b : public test_std_fn {
public:
	bs_b() : test_std_fn(make_binary_search_classifier_function(std::vector<text_identifier>(test_set_b))) {
	}
};
class bt_a : public test_movable_fn {
public:
	bt_a() : test_movable_fn(make_bit_function_classifier_function(std::vector<text_identifier>(test_set_a))) {
	}
};
class bt_b : public test_movable_fn {
public:
	bt_b() : test_movable_fn(make_bit_function_classifier_function(std::vector<text_identifier>(test_set_b))) {
	}
};

int __cdecl main() {
	logging_object log;
	{
		test_object<50, 70, ls_a> to;
		std::cout << to.log_function(log, "classification: linear scan A") << std::endl;
	}
	{
		test_object<50, 70, ls_b> to;
		std::cout << to.log_function(log, "classification: linear scan B") << std::endl;
	}
	{
		test_object<50, 70, bs_a> to;
		std::cout << to.log_function(log, "classification: binary search A") << std::endl;
	}
	{
		test_object<50, 70, bs_b> to;
		std::cout << to.log_function(log, "classification: binary search B") << std::endl;
	}
	{
		test_object<50, 70, bt_a> to;
		std::cout << to.log_function(log, "classification: bit function A") << std::endl;
	}
	{
		test_object<50, 70, bt_b> to;
		std::cout << to.log_function(log, "classification: bit function B") << std::endl;
	}
	{
		test_object<50, 70, test_binary_search_classifier_class_a> to;
		std::cout << to.log_function(log, "classification: b-search class A") << std::endl;
	}
	{
		test_object<50, 70, test_binary_search_classifier_class_b> to;
		std::cout << to.log_function(log, "classification: b-search class B") << std::endl;
	}

	{
		test_object<50, 70, test_binary_search_ct_a> to;
		std::cout << to.log_function(log, "classification: compile-time b-search A") << std::endl;
	}
	{
		test_object<50, 70, test_binary_search_ct_b> to;
		std::cout << to.log_function(log, "classification: compile-time b-search B") << std::endl;
	}
	{
		test_object<50, 70, test_linear_search_ct_a> to;
		std::cout << to.log_function(log, "classification: compile-time linear scan A") << std::endl;
	}
	{
		test_object<50, 70, test_linear_search_ct_b> to;
		std::cout << to.log_function(log, "classification: compile-time linear scan B") << std::endl;
	}
	return 0;
}

