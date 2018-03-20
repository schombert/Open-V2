#include "technologies\\technologies.h"
#include "gtest/gtest.h"
#include "fake_fs\\fake_fs.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace technologies;

const char single_tech[] = 
"tech_name = {\r\n"
" area = army_doctrine\r\n"
"other_date = {}\r\n"
"}";

const char two_techs[] =
"tech_name = {\r\n"
" area = army_doctrine\r\n"
"other_date = {}\r\n"
"}\r\n"
"tech2 = {}";

const char fake_tech_file[] =
"folders = {\r\n"
"army_tech = {\r\n"
"atech_a\r\n"
"atech_b\r\n"
"atech_c\r\n"
"atech_d\r\n"
"}\r\n"
"navy_tech = {\r\n"
"}\r\n"
"}\r\n"
"\r\n"
"schools = {\r\n"
"fake_school = {\r\n"
"bonus = 0\r\n"
"}\r\n"
"}\r\n";

const char fake_tech_file_b[] =
"folders = {\r\n"
"army_tech = {\r\n"
"atech_a\r\n"
"atech_b\r\n"
"atech_c\r\n"
"atech_d\r\n"
"}\r\n"
"navy_tech = {\r\n"
"}\r\n"
"}\r\n"
"\r\n"
"schools = {\r\n"
"fake_school = {\r\n"
"bonus = 0\r\n"
"}\r\n"
"fake_school_b = { \r\n"
"bonus = 0\r\n"
"}\r\n"
"}\r\n";

auto fake_text_handle_lookup();
auto make_fake_tech_file_parse(int& counter);

auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return text_data::text_tag(i++); };
}

auto make_fake_tech_file_parse(int& counter) {
	return [&counter](const token_and_type&,
		tech_category_tag,
		parsed_data&,
		const text_handle_lookup&,
		technologies_manager&) {
		++counter;
	};
}


class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation common = directory_representation(u"common", f_root);
	directory_representation technologies = directory_representation(u"technologies", f_root);

	file_representation tech_file = file_representation(u"technology.txt", common, fake_tech_file);

	file_representation ta = file_representation(u"army_tech.txt", technologies, single_tech);
	file_representation tb = file_representation(u"navy_tech.txt", technologies, two_techs);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

TEST(technologies_tests, pre_parse_single_tech) {
	std::vector<token_group> results;

	tech_category_tag fake_cat(1);
	technologies_manager manager;

	parse_pdx_file(results, single_tech, single_tech + sizeof(single_tech) - 1);
	parse_single_tech_file(fake_cat, fake_text_handle_lookup(), manager, results);

	ASSERT_EQ(1ui64, manager.technologies_container.size());
	ASSERT_EQ(tech_tag(0), manager.technologies_container[tech_tag(0)].id);
	ASSERT_EQ(text_data::text_tag(0), manager.technologies_container[tech_tag(0)].name);

	const auto find_result = manager.named_technology_index.find(text_data::text_tag(0));

	ASSERT_NE(manager.named_technology_index.end(), find_result);
	ASSERT_EQ(tech_tag(0), find_result->second);
}

TEST(technologies_tests, pre_parse_two_techs) {
	std::vector<token_group> results;

	tech_category_tag fake_cat(1);
	technologies_manager manager;

	parse_pdx_file(results, two_techs, two_techs + sizeof(two_techs) - 1);
	parse_single_tech_file(fake_cat, fake_text_handle_lookup(), manager, results);

	ASSERT_EQ(2ui64, manager.technologies_container.size());

	const auto find_result = manager.named_technology_index.find(text_data::text_tag(0));

	ASSERT_NE(manager.named_technology_index.end(), find_result);
	ASSERT_EQ(tech_tag(0), find_result->second);
}

TEST(technologies_tests, pre_parse_tech_file) {
	int count_sub_files = 0;

	std::vector<token_group> results;
	technologies_manager manager;

	parse_pdx_file(results, fake_tech_file, fake_tech_file + sizeof(fake_tech_file) - 1);
	pre_parse_main_technology_file(manager, results, fake_text_handle_lookup(), make_fake_tech_file_parse(count_sub_files));

	EXPECT_EQ(2, count_sub_files);
	EXPECT_EQ(2ui64, manager.technology_categories.size());
	EXPECT_EQ(4ui64, manager.technology_subcategories.size());
	EXPECT_EQ(1ui64, manager.tech_schools.size());

	EXPECT_EQ(tech_category_tag(0), manager.technology_categories[tech_category_tag(0)].id);
	EXPECT_EQ(tech_category_tag(1), manager.technology_categories[tech_category_tag(1)].id);

	EXPECT_EQ(tech_subcategory_tag(0), manager.technology_subcategories[tech_subcategory_tag(0)].id);
	EXPECT_EQ(tech_subcategory_tag(1), manager.technology_subcategories[tech_subcategory_tag(1)].id);
	EXPECT_EQ(tech_subcategory_tag(2), manager.technology_subcategories[tech_subcategory_tag(2)].id);

	EXPECT_EQ(tech_category_tag(0), manager.technology_subcategories[tech_subcategory_tag(0)].parent);
	EXPECT_EQ(tech_category_tag(0), manager.technology_subcategories[tech_subcategory_tag(1)].parent);
	EXPECT_EQ(tech_category_tag(0), manager.technology_subcategories[tech_subcategory_tag(2)].parent);

	EXPECT_EQ(2ui64, manager.named_category_index.size());
	EXPECT_EQ(4ui64, manager.named_subcategory_index.size());
	EXPECT_EQ(1ui64, manager.named_tech_school_index.size());

	EXPECT_EQ(tech_school_tag(0), manager.tech_schools[tech_school_tag(0)].id);
	const auto fr = manager.named_tech_school_index[manager.tech_schools[tech_school_tag(0)].name];
	EXPECT_EQ(fr, tech_school_tag(0));
}

TEST(technologies_tests, pre_parse_schools) {
	int count_sub_files = 0;

	std::vector<token_group> results;
	technologies_manager manager;

	parse_pdx_file(results, fake_tech_file_b, fake_tech_file_b + sizeof(fake_tech_file_b) - 1);
	pre_parse_main_technology_file(manager, results, fake_text_handle_lookup(), make_fake_tech_file_parse(count_sub_files));

	EXPECT_EQ(2ui64, manager.tech_schools.size());
	EXPECT_EQ(2ui64, manager.named_tech_school_index.size());

	EXPECT_EQ(tech_school_tag(0), manager.tech_schools[tech_school_tag(0)].id);
	EXPECT_EQ(tech_school_tag(1), manager.tech_schools[tech_school_tag(1)].id);
	const auto fr = manager.named_tech_school_index[manager.tech_schools[tech_school_tag(0)].name];
	EXPECT_EQ(fr, tech_school_tag(0));
	const auto frb = manager.named_tech_school_index[manager.tech_schools[tech_school_tag(1)].name];
	EXPECT_EQ(frb, tech_school_tag(1));
}

TEST(technologies_tests, pre_parse_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	technologies_manager manager;

	auto result = pre_parse_technologies(manager, f.get_root(), fake_text_handle_lookup());

	ASSERT_EQ(3ui64, manager.technologies_container.size());
	ASSERT_EQ(2ui64, manager.technology_categories.size());
	ASSERT_EQ(4ui64, manager.technology_subcategories.size());
}
