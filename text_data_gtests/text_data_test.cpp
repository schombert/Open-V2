#include "pch.h"
#include "text_data\\text_data.h"
#include <string>
#include <variant>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1
using namespace text_data;

std::wstring get_string(text_sequences& s, int component_num) {
	return std::visit([&s](const auto& o) {
		if constexpr(std::is_same_v<std::decay_t<decltype(o)>, text_chunk>)
			return std::wstring(s.text_data.data() + o.offset, s.text_data.data() + o.offset + o.length);
		else
			return std::wstring();
	}, s.all_components[component_num]);
}

TEST(adding_string_data, text_data_test) {
	text_sequences seq_object;

	const char t1[] = "text text";

	add_win1250_text_to_container(seq_object, RANGE(t1));

	EXPECT_EQ(1, seq_object.all_components.size());
	EXPECT_EQ(0, std::get<text_chunk>(seq_object.all_components[0]).offset);
	EXPECT_EQ(9, std::get<text_chunk>(seq_object.all_components[0]).length);
	EXPECT_EQ(std::wstring(L"text text"), get_string(seq_object,0));

	const char t2[] = "text\xC2\xA7text ";
	add_utf8_text_to_container(seq_object, RANGE(t2));

	EXPECT_EQ(2, seq_object.all_components.size());
	EXPECT_EQ(10, get_string(seq_object, 1).length());

	EXPECT_EQ(19, seq_object.text_data.size());
}

TEST(single_newline, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "\\n";
	const char test_key[] = "key";

	add_utf8_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1, seq_object.all_components.size());
	EXPECT_TRUE(std::holds_alternative<line_break>(seq_object.all_components[0]));
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(1, seq_object.all_sequences[0].component_count);
}

TEST(win1250_color, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "\xA7W";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1, seq_object.all_components.size());
	EXPECT_EQ(text_color::white, std::get<color_change>(seq_object.all_components[0]).color);
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(1, seq_object.all_sequences[0].component_count);
}

TEST(utf8_color, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "\xC2\xA7W";
	const char test_key[] = "key";

	add_utf8_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1, seq_object.all_components.size());
	EXPECT_EQ(text_color::white, std::get<color_change>(seq_object.all_components[0]).color);
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(1, seq_object.all_sequences[0].component_count);
}

TEST(single_replacement, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "$Union$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1, seq_object.all_components.size());
	EXPECT_EQ(value_type::vtype_union, std::get<value_placeholder>(seq_object.all_components[0]).value);
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(1, seq_object.all_sequences[0].component_count);
}

TEST(single_bad_replacement, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "$xxxxx$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1, seq_object.all_components.size());
	EXPECT_EQ(value_type::error_no_matching_value, std::get<value_placeholder>(seq_object.all_components[0]).value);
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(1, seq_object.all_sequences[0].component_count);
}

TEST(intervening_value, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "aaa$Union$bb bb";
	const char test_key[] = "key";

	add_utf8_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(3, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"aaa"), get_string(seq_object, 0));
	EXPECT_EQ(value_type::vtype_union, std::get<value_placeholder>(seq_object.all_components[1]).value);
	EXPECT_EQ(std::wstring(L"bb bb"), get_string(seq_object, 2));
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(3, seq_object.all_sequences[0].component_count);
}

TEST(string_color_value, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "aaa\xA7R$COUNT$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(3, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"aaa"), get_string(seq_object, 0));
	EXPECT_EQ(text_color::red, std::get<color_change>(seq_object.all_components[1]).color);
	EXPECT_EQ(value_type::count, std::get<value_placeholder>(seq_object.all_components[2]).value);
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(3, seq_object.all_sequences[0].component_count);
}

TEST(two_keys, text_data_test) {
	text_sequences seq_object;
	const char t1[] = "aaa\xA7R$COUNT$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(3, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"aaa"), get_string(seq_object, 0));
	EXPECT_EQ(text_color::red, std::get<color_change>(seq_object.all_components[1]).color);
	EXPECT_EQ(value_type::count, std::get<value_placeholder>(seq_object.all_components[2]).value);
	EXPECT_EQ(1, seq_object.all_sequences.size());
	EXPECT_EQ(0, seq_object.all_sequences[0].starting_component);
	EXPECT_EQ(3, seq_object.all_sequences[0].component_count);

	const char t2[] = "$COUNT$\xA7""btext";
	const char test_key2[] = "key2";

	add_win1250_sequence(seq_object, RANGE(test_key2), RANGE(t2));

	EXPECT_EQ(6, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"text"), get_string(seq_object, 5));
	EXPECT_EQ(text_color::black, std::get<color_change>(seq_object.all_components[4]).color);
	EXPECT_EQ(value_type::count, std::get<value_placeholder>(seq_object.all_components[3]).value);
	EXPECT_EQ(2, seq_object.all_sequences.size());
	EXPECT_EQ(3, seq_object.all_sequences[1].starting_component);
	EXPECT_EQ(3, seq_object.all_sequences[1].component_count);

	EXPECT_EQ(1, seq_object.key_to_sequence_map["key2"]);
}