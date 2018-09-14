#include "pch.h"
#include "text_data\\text_data.h"
#include "text_data\\text_data_io.h"
#include <string>
#include <variant>
#include "simple_fs\\simple_fs.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1
using namespace text_data;

std::wstring get_string(text_sequences& s, uint32_t component_num);
std::wstring get_string(text_sequences& s, uint32_t component_num) {
	return std::visit([&s](const auto& o) {
		if constexpr(std::is_same_v<std::decay_t<decltype(o)>, text_chunk>)
			return std::wstring(s.text_data.data() + o.offset, s.text_data.data() + o.offset + o.length);
		else
			return std::wstring();
	}, s.all_components[component_num]);
}

TEST(text_data_test, adding_string_data) {
	text_sequences seq_object;

	const char t1[] = "text text";

	add_win1250_text_to_container(seq_object, RANGE(t1));

	EXPECT_EQ(1ui64, seq_object.all_components.size());
	EXPECT_EQ(0ui32, std::get<text_chunk>(seq_object.all_components[0]).offset);
	EXPECT_EQ(9ui16, std::get<text_chunk>(seq_object.all_components[0]).length);
	EXPECT_EQ(std::wstring(L"text text"), get_string(seq_object,0));

	const char t2[] = "text\xC2\xA7text ";
	add_utf8_text_to_container(seq_object, RANGE(t2));

	EXPECT_EQ(2ui64, seq_object.all_components.size());
	EXPECT_EQ(10ui64, get_string(seq_object, 1).length());

	EXPECT_EQ(19ui64, seq_object.text_data.size());
}

TEST(text_data_test, single_newline) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "\\n";
	const char test_key[] = "key";

	add_utf8_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1ui64, seq_object.all_components.size());
	EXPECT_TRUE(std::holds_alternative<line_break>(seq_object.all_components[0]));
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(1ui16, seq_object.all_sequences[text_tag(0)].component_count);
}

TEST(text_data_test, win1250_color) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "\xA7W";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1ui64, seq_object.all_components.size());
	EXPECT_EQ(text_color::white, std::get<color_change>(seq_object.all_components[0]).color);
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(1ui16, seq_object.all_sequences[text_tag(0)].component_count);
}

TEST(text_data_test, utf8_color) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "\xC2\xA7W";
	const char test_key[] = "key";

	add_utf8_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1ui64, seq_object.all_components.size());
	EXPECT_EQ(text_color::white, std::get<color_change>(seq_object.all_components[0]).color);
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(1ui16, seq_object.all_sequences[text_tag(0)].component_count);
}

TEST(text_data_test, single_replacement) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "$Union$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1ui64, seq_object.all_components.size());
	EXPECT_EQ(value_type::vtype_union, std::get<value_placeholder>(seq_object.all_components[0]).value);
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(1ui16, seq_object.all_sequences[text_tag(0)].component_count);
}

TEST(text_data_test, single_bad_replacement) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "$xxxxx$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(1ui64, seq_object.all_components.size());
	EXPECT_EQ(value_type::error_no_matching_value, std::get<value_placeholder>(seq_object.all_components[0]).value);
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(1ui16, seq_object.all_sequences[text_tag(0)].component_count);
}

TEST(text_data_test, intervening_value) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "aaa$Union$bb bb";
	const char test_key[] = "key";

	add_utf8_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(3ui64, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"aaa"), get_string(seq_object, 0));
	EXPECT_EQ(value_type::vtype_union, std::get<value_placeholder>(seq_object.all_components[1]).value);
	EXPECT_EQ(std::wstring(L"bb bb"), get_string(seq_object, 2));
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(3ui16, seq_object.all_sequences[text_tag(0)].component_count);
}

TEST(text_data_test, string_color_value) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "aaa\xA7R$COUNT$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(3ui64, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"aaa"), get_string(seq_object, 0));
	EXPECT_EQ(text_color::red, std::get<color_change>(seq_object.all_components[1]).color);
	EXPECT_EQ(value_type::count, std::get<value_placeholder>(seq_object.all_components[2]).value);
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(3ui16, seq_object.all_sequences[text_tag(0)].component_count);
}

TEST(text_data_test, two_keys) {
	text_sequences seq_object;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	const char t1[] = "aaa\xA7R$COUNT$";
	const char test_key[] = "key";

	add_win1250_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));

	EXPECT_EQ(3ui64, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"aaa"), get_string(seq_object, 0));
	EXPECT_EQ(text_color::red, std::get<color_change>(seq_object.all_components[1]).color);
	EXPECT_EQ(value_type::count, std::get<value_placeholder>(seq_object.all_components[2]).value);
	EXPECT_EQ(1ui64, seq_object.all_sequences.size());
	EXPECT_EQ(0ui16, seq_object.all_sequences[text_tag(0)].starting_component);
	EXPECT_EQ(3ui16, seq_object.all_sequences[text_tag(0)].component_count);

	const char t2[] = "$COUNT$\xA7""btext";
	const char test_key2[] = "key2";

	add_win1250_sequence(seq_object, temp_map, RANGE(test_key2), RANGE(t2));

	EXPECT_EQ(6ui64, seq_object.all_components.size());
	EXPECT_EQ(std::wstring(L"text"), get_string(seq_object, 5));
	EXPECT_EQ(text_color::black, std::get<color_change>(seq_object.all_components[4]).color);
	EXPECT_EQ(value_type::count, std::get<value_placeholder>(seq_object.all_components[3]).value);
	EXPECT_EQ(2ui64, seq_object.all_sequences.size());
	EXPECT_EQ(3ui16, seq_object.all_sequences[text_tag(1)].starting_component);
	EXPECT_EQ(3ui16, seq_object.all_sequences[text_tag(1)].component_count);

	EXPECT_EQ(text_tag(1), temp_map["key2"]);
}

TEST(text_data_test, text_to_string) {
	text_sequences seq_object;
	const char t1[] = "aaa\xA7R$COUNT$";
	const char test_key[] = "key";
	const char t2[] = "$COUNT$\xA7""btext";
	const char test_key2[] = "key2";

	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(seq_object.key_data);

	add_win1250_sequence(seq_object, temp_map, RANGE(test_key), RANGE(t1));
	add_win1250_sequence(seq_object, temp_map, RANGE(test_key2), RANGE(t2));
	EXPECT_EQ(std::u16string(u"aaa$count$"), to_string(seq_object, text_tag(0)));
	EXPECT_EQ(std::u16string(u"$count$text"), to_string(seq_object, text_tag(1)));
}

TEST(text_data_test, text_alignment) {

	const auto a3 = align_in_bounds(text_data::alignment::center, 20, 10, 40, 20);
	EXPECT_EQ(10, a3.first);
	EXPECT_EQ(5, a3.second);

	const auto a8 = align_in_bounds(text_data::alignment::left, 20, 10, 40, 20);
	EXPECT_EQ(0, a8.first);
	EXPECT_EQ(5, a8.second);

	const auto a9 = align_in_bounds(text_data::alignment::right, 20, 10, 40, 20);
	EXPECT_EQ(20, a9.first);
	EXPECT_EQ(5, a9.second);
}

TEST(text_data_test, newtext_file) {
	text_data::text_sequences container;
	std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci> temp_map(vector_backed_string_less_ci(container.key_data));

	file_system fs;
	fs.set_root(u"D:\\programs\\V2\\localisation");
	auto op = fs.get_root().open_file(u"newtext.csv");

	EXPECT_TRUE(bool(op));

	text_data::load_single_text_file(*op, container, temp_map);

	container.key_to_sequence_map.~flat_map();
	new (&container.key_to_sequence_map) boost::container::flat_map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci>(boost::container::ordered_unique_range_t(), temp_map.begin(), temp_map.end(), vector_backed_string_less_ci(container.key_data));

	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "EVTNAME5002"));
	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "ADD_WARGOAL_DESC"));
	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "adopt_die_stem_desc"));

	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "found_the_bbc_title"));
	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "found_the_bbc_desc"));
	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "establish_the_sovnarkom_title"));
	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "establish_the_sovnarkom_desc"));

	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "EVTDESC2060"));
	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "niederwald_denkmal_desc"));
	EXPECT_NE(text_data::text_tag(), text_data::get_existing_text_handle(container, "EVTNAME16250"));
}
