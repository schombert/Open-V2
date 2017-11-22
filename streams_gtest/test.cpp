#include "pch.h"

#include "streams\\streams.hpp"
#include <string>
#include <vector>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class pair_reduction {
public:
	bool read_first = true;
	int first_value = 0;

	template<typename NEXT>
	void operator()(const int i, NEXT& n) {
		if (read_first)
			first_value = i;
		else {
			auto v = std::make_pair(first_value, i);
			n(v);
		}
		read_first = !read_first;
	}
	template<typename NEXT>
	void yield(NEXT& n) {
		if (!read_first) {
			auto v = std::make_pair(first_value, 0);
			n(v);
		}
	}
};

class int_deref {
public:
	template<typename NEXT>
	void operator()(const int* i, NEXT& n) {
		n(*i);
	}
};

class fold_summation {
public:
	int value;
	fold_summation(int v) : value(v) {};

	void operator()(const int i) {
		value += i;
	}
	int yield() {
		return value;
	}
	void reset() {
		value = 0;
	}
};

#define TEST_METHOD(x, y) TEST(x, y)

TEST_METHOD(filter_stream, streams_tests) {
	std::string filter_result;
	auto s = make_stream(
		"t e X t aaa   ",
		stream_filter([](const char* v) {return *v != ' '; }),
		[&filter_result](const char* v) { filter_result += *v; });
	s.yield();

	EXPECT_EQ(std::string("teXtaaa"), filter_result);
}

TEST_METHOD(transforming_stream, streams_tests) {
	std::vector<std::pair<char, int>> transform_result;

	make_stream(
		"TEXT blah blah",
		[](const char* v) {return std::make_pair(*v, int(*v)); },
		[&transform_result](const std::pair<char, int>& v) { transform_result.push_back(v); }).yield();

	EXPECT_EQ(14ui64, transform_result.size());
	EXPECT_EQ('b', transform_result[5].first);
	EXPECT_EQ(int('b'), transform_result[5].second);
}

TEST_METHOD(filter_and_transform_stream, streams_tests) {
	std::vector<std::pair<char, int>> transform_result;

	make_stream(
		"TEXT blah blah",
		stream_filter([](const char* v) {return *v != ' '; }),
		[](const char* v) {return std::make_pair(*v, int(*v)); },
		[&transform_result](const std::pair<char, int>& v) { transform_result.push_back(v); }).yield();

	EXPECT_EQ(12ui64, transform_result.size());
	EXPECT_EQ('l', transform_result[5].first);
	EXPECT_EQ(int('l'), transform_result[5].second);
}

TEST_METHOD(stream_reduction, streams_tests) {
	int sequence[] = { 1, 2, 3, 4, 5 };
	std::vector<std::pair<int, int>> result;

	make_stream(sequence, int_deref(), pair_reduction(), [&result](const std::pair<int, int>& v) { result.push_back(v); }).yield();
	EXPECT_EQ(3ui64, result.size());
	EXPECT_EQ(1, result[0].first);
	EXPECT_EQ(2, result[0].second);
	EXPECT_EQ(3, result[1].first);
	EXPECT_EQ(4, result[1].second);
	EXPECT_EQ(5, result[2].first);
	EXPECT_EQ(0, result[2].second);
}

TEST_METHOD(stream_composition, streams_tests) {
	{
		std::vector<std::pair<char, int>> transform_result;

		auto tail = make_stream([](const char* v) {return std::make_pair(*v, int(*v)); },
								[&transform_result](const std::pair<char, int>& v) { transform_result.push_back(v); });
		auto full = make_stream("TEXT blah blah",
								stream_filter([](const char* v) {return *v != ' '; }), tail);

		
		full.yield();

		EXPECT_EQ(12ui64, transform_result.size());
		EXPECT_EQ('l', transform_result[5].first);
		EXPECT_EQ(int('l'), transform_result[5].second);
	}
	{
		std::vector<std::pair<char, int>> transform_result;

		auto cf = make_curried_stream("TEXT blah blah", stream_filter([](const char* v) {return *v != ' '; }));
		auto ct = cf([](const char* v) {return std::make_pair(*v, int(*v)); },
					 [&transform_result](const std::pair<char, int>& v) { transform_result.push_back(v); });
		ct.yield();
		EXPECT_EQ(12ui64, transform_result.size());
		EXPECT_EQ('l', transform_result[5].first);
		EXPECT_EQ(int('l'), transform_result[5].second);
	}
}

TEST_METHOD(stream_fold, streams_tests) {
	int sequence_a[] = { 1, 2, 3, 4, 5 };

	auto fresult = make_stream(sequence_a, int_deref(), fold_summation(0)).yield();
	EXPECT_EQ(15, fresult);
}

TEST_METHOD(stream_reset, streams_tests) {
	auto stream = make_stream([](int x) {return x * 2; }, fold_summation(0));
	stream(1);
	EXPECT_EQ(2, stream.yield());
	stream(2);
	EXPECT_EQ(6, stream.yield());
	stream(2);
	EXPECT_EQ(10, stream.yield());
	stream.reset();
	stream(3);
	EXPECT_EQ(6, stream.yield());
}

TEST_METHOD(type_erased_stream_test, streams_tests) {
	int total_a = 0;
	int total_b = 0;

	auto base_a = generic_stream<int>(make_stream([](int i) { return i + 1; }, [&total_a](int i) { total_a += i; }));
	auto base_b = generic_stream<int>(make_stream([](int i) { return i * 2; }, [&total_b](int i) { total_b += i; }));

	type_erased_stream<int>* interface = &base_a;
	(*interface)(3);
	(*interface)(0);

	EXPECT_EQ(5, total_a);

	interface = &base_b;
	(*interface)(3);
	(*interface)(0);

	EXPECT_EQ(6, total_b);
}