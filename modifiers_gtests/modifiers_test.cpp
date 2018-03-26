#include "modifiers\\modifiers.h"
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
	file_representation nc = file_representation(u"nationalvalues.txt", common,
		"nv_a = { stuff = x }\r\n"
		"nv_x = { stuff = x }\r\n"
		"nv_b = { stuff = y\r\n stuff = 0}\r\n");
	file_representation crime = file_representation(u"crime.txt", common,
		"crime_a = { stuff = 0 }\r\n"
		"crime_b = { stuff = 1 \r\n sub = { 0 0 0 } }\r\n");

	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace modifiers;

TEST(modifiers_tests, crimes_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	modifiers_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);

	pre_parse_crimes(state, f.get_root());

	EXPECT_EQ(2ui64, m.provincial_modifiers.size());
	EXPECT_EQ(2ui64, m.named_provincial_modifiers_index.size());

	EXPECT_EQ(provincial_modifier_tag(0), m.provincial_modifiers[provincial_modifier_tag(0)].id);
	EXPECT_EQ(provincial_modifier_tag(1), m.provincial_modifiers[provincial_modifier_tag(1)].id);
	EXPECT_EQ(provincial_modifier_tag(0), m.named_provincial_modifiers_index[m.provincial_modifiers[provincial_modifier_tag(0)].name]);
	EXPECT_EQ(provincial_modifier_tag(1), m.named_provincial_modifiers_index[m.provincial_modifiers[provincial_modifier_tag(1)].name]);
}

TEST(modifiers_tests, nv_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	modifiers_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);

	pre_parse_national_values(state, f.get_root());

	EXPECT_EQ(3ui64, m.national_modifiers.size());
	EXPECT_EQ(3ui64, m.named_national_modifiers_index.size());

	EXPECT_EQ(national_modifier_tag(0), m.national_modifiers[national_modifier_tag(0)].id);
	EXPECT_EQ(national_modifier_tag(1), m.national_modifiers[national_modifier_tag(1)].id);
	EXPECT_EQ(national_modifier_tag(2), m.national_modifiers[national_modifier_tag(2)].id);
	EXPECT_EQ(national_modifier_tag(0), m.named_national_modifiers_index[m.national_modifiers[national_modifier_tag(0)].name]);
	EXPECT_EQ(national_modifier_tag(1), m.named_national_modifiers_index[m.national_modifiers[national_modifier_tag(1)].name]);
	EXPECT_EQ(national_modifier_tag(2), m.named_national_modifiers_index[m.national_modifiers[national_modifier_tag(2)].name]);
}
