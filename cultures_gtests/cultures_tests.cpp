#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "cultures\\cultures.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace cultures;

inline auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return text_data::text_tag(i++); };
}

class test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation common1 = directory_representation(u"common", test1);
	file_representation countries1 = file_representation(u"countries.txt", common1,
		"GER = \"countries/file 1.txt\"\r\n"
		"ENG = \"countries/file 2.txt\"");
	file_representation rel1 = file_representation(u"religion.txt", common1,
		"group_a = {\r\n"
		"r1 = {\r\n"
		"icon = 1\r\n"
		"color = {0.5 0.0 0.0}\r\n"
		"}\r\n"
		"r2 = {\r\n"
		"icon = 2\r\n"
		"color = {0.0 1.0 0}\r\n"
		"}\r\n"
		"}\r\n"
		"group_b = {\r\n"
		"r3 = {\r\n"
		"icon = 6\r\n"
		"color = {0 0 0.5}\r\n"
		"pagan = yes\r\n"
		"}\r\n"
		"}");
	file_representation cul1 = file_representation(u"cultures.txt", common1,
		"group = {\r\n"
		"leader = X\r\n"
		"unit = Y\r\n"
		"sub_culture_a = {\r\n"
		"color = {10 20 30}\r\n"
		"first_names = {a b c}\r\n"
		"last_names = { d }\r\n"
		"}\r\n"
		"union = GER\r\n"
		"}");

	directory_representation test2 = directory_representation(u"test2", f_root);
	directory_representation common2 = directory_representation(u"common", test2);
	file_representation countries2 = file_representation(u"countries.txt", common2,
		"GER = \"countries/file 1.txt\"\r\n"
		"ENG = \"countries/file 2.txt\"");
	file_representation cul2 = file_representation(u"cultures.txt", common2,
		"\xEF\xBB\xBFgroup = {\r\n"
		"leader = X\r\n"
		"unit = Y\r\n"
		"sub_culture_a = {\r\n"
		"color = {10 20 30}\r\n"
		"first_names = {a b c}\r\n"
		"last_names = { d }\r\n"
		"}\r\n"
		"}\r\n"
		"group_b = { \r\n"
		"sub_culture_b = {\r\n"
		"color = {20 30 40}\r\n"
		"first_names = {a bddd}\r\n"
		"last_names = { d e }\r\n"
		"}\r\n"
		"sub_culture_c = {\r\n"
		"color = {50 60 70}\r\n"
		"first_names = {asss bddd csss d}\r\n"
		"last_names = { q x }\r\n"
		"}\r\n"
		"union = ENG\n"
		"}");

	test_files() {
		set_default_root(f_root);
	}
};

TEST(cultures_tests, tag_conversions) {
	const char t1[] = "TAG";
	const char t2[] = "GER";

	const auto ta = tag_to_encoding(RANGE(t1));
	const auto tb = tag_to_encoding(RANGE(t2));

	EXPECT_NE(ta, tb);

	const auto ata = encoded_tag_to_text_tag(ta);
	const auto atb = encoded_tag_to_text_tag(tb);

	EXPECT_STREQ(t1, ata.tag);
	EXPECT_STREQ(t2, atb.tag);
}

TEST(cultures_tests, tag_file) {
	test_files real_fs;
	file_system f;

	const char t1[] = "ENG";
	const char t2[] = "GER";

	f.set_root(RANGE(u"F:\\test1"));

	culture_manager m;
	const auto tvector = parse_national_tags(m, f.get_root());

	EXPECT_EQ(2ui64, tvector.size());
	EXPECT_EQ(2ui64, m.national_tags.size());
	EXPECT_EQ(2ui64, m.national_tags_index.size());
	EXPECT_EQ(std::string("file 1.txt"), tvector[national_tag(0)]);
	EXPECT_EQ(std::string("file 2.txt"), tvector[national_tag(1)]);
	EXPECT_EQ(national_tag(0), m.national_tags[national_tag(0)].id);
	EXPECT_EQ(tag_to_encoding(RANGE(t2)), m.national_tags[national_tag(0)].tag_code);
	EXPECT_EQ(national_tag(1), m.national_tags[national_tag(1)].id);
	EXPECT_EQ(tag_to_encoding(RANGE(t1)), m.national_tags[national_tag(1)].tag_code);
	EXPECT_EQ(national_tag(0), m.national_tags_index[tag_to_encoding(RANGE("GER"))]);
	EXPECT_EQ(national_tag(1), m.national_tags_index[tag_to_encoding(RANGE("ENG"))]);
}

TEST(cultures_tests, religion_file) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	culture_manager m;
	parse_religions(m, f.get_root(), fake_text_handle_lookup());

	EXPECT_EQ(3ui64, m.religions.size());
	EXPECT_EQ(3ui64, m.named_religion_index.size());
	EXPECT_EQ(religion_tag(0), m.religions[religion_tag(0)].id);
	EXPECT_EQ(religion_tag(1), m.religions[religion_tag(1)].id);
	EXPECT_EQ(religion_tag(2), m.religions[religion_tag(2)].id);
	EXPECT_EQ(1ui8, m.religions[religion_tag(0)].icon);
	EXPECT_EQ(2ui8, m.religions[religion_tag(1)].icon);
	EXPECT_EQ(6ui8, m.religions[religion_tag(2)].icon);
	EXPECT_EQ(false, m.religions[religion_tag(0)].pagan);
	EXPECT_EQ(false, m.religions[religion_tag(1)].pagan);
	EXPECT_EQ(true, m.religions[religion_tag(2)].pagan);
	EXPECT_EQ(static_cast<uint8_t>(0.5 * 255.0), m.religions[religion_tag(0)].color.r);
	EXPECT_EQ(0ui8, m.religions[religion_tag(1)].color.r);
	EXPECT_EQ(0ui8, m.religions[religion_tag(2)].color.r);
	EXPECT_EQ(0ui8, m.religions[religion_tag(0)].color.g);
	EXPECT_EQ(255ui8, m.religions[religion_tag(1)].color.g);
	EXPECT_EQ(0ui8, m.religions[religion_tag(2)].color.g);
	EXPECT_EQ(0ui8, m.religions[religion_tag(0)].color.b);
	EXPECT_EQ(0ui8, m.religions[religion_tag(1)].color.b);
	EXPECT_EQ(static_cast<uint8_t>(0.5 * 255.0), m.religions[religion_tag(2)].color.b);
	EXPECT_EQ(religion_tag(0), m.named_religion_index[m.religions[religion_tag(0)].name]);
	EXPECT_EQ(religion_tag(1), m.named_religion_index[m.religions[religion_tag(1)].name]);
	EXPECT_EQ(religion_tag(2), m.named_religion_index[m.religions[religion_tag(2)].name]);
}

TEST(cultures_tests, cultures_file) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	culture_manager m;

	parse_national_tags(m, f.get_root());
	parse_cultures(m, f.get_root(), fake_text_handle_lookup());

	EXPECT_EQ(1ui64, m.culture_container.size());
	EXPECT_EQ(1ui64, m.culture_groups.size());
	EXPECT_EQ(1ui64, m.named_culture_group_index.size());
	EXPECT_EQ(1ui64, m.named_culture_index.size());

	EXPECT_EQ(culture_tag(0), m.culture_container[culture_tag(0)].id);
	EXPECT_EQ(culture_group_tag(0), m.culture_container[culture_tag(0)].group);
	EXPECT_EQ(10ui8, m.culture_container[culture_tag(0)].color.r);
	EXPECT_EQ(20ui8, m.culture_container[culture_tag(0)].color.g);
	EXPECT_EQ(30ui8, m.culture_container[culture_tag(0)].color.b);
	EXPECT_EQ(culture_tag(0), m.named_culture_index[m.culture_container[culture_tag(0)].name]);

	const auto fn = m.first_names_by_culture.get_row(to_index(culture_tag(0)));
	const auto ln = m.last_names_by_culture.get_row(to_index(culture_tag(0)));

	EXPECT_EQ(3i64, fn.second - fn.first);
	EXPECT_EQ(1i64, ln.second - ln.first);

	EXPECT_EQ(culture_group_tag(0), m.culture_groups[culture_group_tag(0)].id);
	EXPECT_EQ(culture_group_tag(0), m.named_culture_group_index[m.culture_groups[culture_group_tag(0)].name]);
	EXPECT_EQ(national_tag(0), m.culture_groups[culture_group_tag(0)].union_tag);
}

TEST(multiple_cultures_tests, cultures_file) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	culture_manager m;

	parse_national_tags(m, f.get_root());
	parse_cultures(m, f.get_root(), fake_text_handle_lookup());

	EXPECT_EQ(3ui64, m.culture_container.size());
	EXPECT_EQ(2ui64, m.culture_groups.size());
	EXPECT_EQ(2ui64, m.named_culture_group_index.size());
	EXPECT_EQ(3ui64, m.named_culture_index.size());

	EXPECT_EQ(culture_tag(0), m.culture_container[culture_tag(0)].id);
	EXPECT_EQ(culture_group_tag(0), m.culture_container[culture_tag(0)].group);
	EXPECT_EQ(10ui8, m.culture_container[culture_tag(0)].color.r);
	EXPECT_EQ(20ui8, m.culture_container[culture_tag(0)].color.g);
	EXPECT_EQ(30ui8, m.culture_container[culture_tag(0)].color.b);
	EXPECT_EQ(culture_tag(0), m.named_culture_index[m.culture_container[culture_tag(0)].name]);

	const auto fn = m.first_names_by_culture.get_row(to_index(culture_tag(0)));
	const auto ln = m.last_names_by_culture.get_row(to_index(culture_tag(0)));

	EXPECT_EQ(3i64, fn.second - fn.first);
	EXPECT_EQ(1i64, ln.second - ln.first);

	EXPECT_EQ(culture_tag(1), m.culture_container[culture_tag(1)].id);
	EXPECT_EQ(culture_group_tag(1), m.culture_container[culture_tag(1)].group);
	EXPECT_EQ(20ui8, m.culture_container[culture_tag(1)].color.r);
	EXPECT_EQ(30ui8, m.culture_container[culture_tag(1)].color.g);
	EXPECT_EQ(40ui8, m.culture_container[culture_tag(1)].color.b);
	EXPECT_EQ(culture_tag(1), m.named_culture_index[m.culture_container[culture_tag(1)].name]);

	const auto fn2 = m.first_names_by_culture.get_row(to_index(culture_tag(1)));
	const auto ln2 = m.last_names_by_culture.get_row(to_index(culture_tag(1)));

	EXPECT_EQ(2i64, fn2.second - fn2.first);
	EXPECT_EQ(2i64, ln2.second - ln2.first);

	EXPECT_EQ(culture_tag(2), m.culture_container[culture_tag(2)].id);
	EXPECT_EQ(culture_group_tag(1), m.culture_container[culture_tag(2)].group);
	EXPECT_EQ(50ui8, m.culture_container[culture_tag(2)].color.r);
	EXPECT_EQ(60ui8, m.culture_container[culture_tag(2)].color.g);
	EXPECT_EQ(70ui8, m.culture_container[culture_tag(2)].color.b);
	EXPECT_EQ(culture_tag(2), m.named_culture_index[m.culture_container[culture_tag(2)].name]);

	const auto fn3 = m.first_names_by_culture.get_row(to_index(culture_tag(2)));
	const auto ln3 = m.last_names_by_culture.get_row(to_index(culture_tag(2)));

	EXPECT_EQ(4i64, fn3.second - fn3.first);
	EXPECT_EQ(2i64, ln3.second - ln3.first);

	EXPECT_EQ(culture_group_tag(0), m.culture_groups[culture_group_tag(0)].id);
	EXPECT_EQ(culture_group_tag(0), m.named_culture_group_index[m.culture_groups[culture_group_tag(0)].name]);
	EXPECT_EQ(national_tag(), m.culture_groups[culture_group_tag(0)].union_tag);

	EXPECT_EQ(culture_group_tag(1), m.culture_groups[culture_group_tag(1)].id);
	EXPECT_EQ(culture_group_tag(1), m.named_culture_group_index[m.culture_groups[culture_group_tag(1)].name]);
	EXPECT_EQ(national_tag(1), m.culture_groups[culture_group_tag(1)].union_tag);
}
