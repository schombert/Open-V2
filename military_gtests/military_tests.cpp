#include "military\\military.h"
#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

inline auto fake_text_handle_lookup(std::map<std::string, text_data::text_tag>& values) {
	return[j = 0ui16, &values](const char* s, const char* e) mutable {
		const auto i = std::string(s, e);
		if (values.find(i) == values.end()) {
			values[i] = text_data::text_tag(j++);
		}
		return values[i];
	};
}


class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation common = directory_representation(u"common", f_root);
	file_representation cb = file_representation(u"cb_types.txt", common,
		"peace_order = {\r\n"
		"cb_a\r\n"
		"cb_b\r\n"
		"}\r\n"
		"cb_b = { stuff }\r\n"
		"cb_a = { stuff }\r\n"
		"cb_c = { stuff }\r\n");
	directory_representation units = directory_representation(u"units", f_root);
	file_representation f1 = file_representation(u"dragoon.txt", units, "dragoon = { stuff }");
	file_representation f2 = file_representation(u"airplane.txt", units, "plane_a = { stuff }\r\nplane_b = {}");

	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace military;

TEST(military_tests, test_units_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	military_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);
	
	pre_parse_unit_types(state, f.get_root());

	EXPECT_EQ(3ui64, m.unit_types.size());
	EXPECT_EQ(3ui64, m.named_unit_type_index.size());

	EXPECT_EQ(unit_type_tag(0), m.unit_types[unit_type_tag(0)].id);
	EXPECT_EQ(unit_type_tag(1), m.unit_types[unit_type_tag(1)].id);
	EXPECT_EQ(unit_type_tag(2), m.unit_types[unit_type_tag(2)].id);
	EXPECT_EQ(unit_type_tag(0), m.named_unit_type_index[v["dragoon"]]);
	EXPECT_EQ(unit_type_tag(1), m.named_unit_type_index[m.unit_types[unit_type_tag(1)].name]);
	EXPECT_EQ(unit_type_tag(2), m.named_unit_type_index[m.unit_types[unit_type_tag(2)].name]);
}

TEST(military_tests, test_cb_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	military_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);

	pre_parse_cb_types(state, f.get_root());

	EXPECT_EQ(3ui64, m.cb_types.size());
	EXPECT_EQ(3ui64, m.named_cb_type_index.size());

	EXPECT_EQ(cb_type_tag(0), m.cb_types[cb_type_tag(0)].id);
	EXPECT_EQ(cb_type_tag(1), m.cb_types[cb_type_tag(1)].id);
	EXPECT_EQ(cb_type_tag(2), m.cb_types[cb_type_tag(2)].id);
	EXPECT_EQ(cb_type_tag(0), m.named_cb_type_index[v["cb_a"]]);
	EXPECT_EQ(cb_type_tag(1), m.named_cb_type_index[m.cb_types[cb_type_tag(1)].name]);
	EXPECT_EQ(cb_type_tag(2), m.named_cb_type_index[m.cb_types[cb_type_tag(2)].name]);
}
