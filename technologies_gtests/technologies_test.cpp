#include "technologies\\technologies.h"
#include "gtest/gtest.h"
#include "fake_fs\\fake_fs.h"
#include "modifiers\\modifiers.h"

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
"army_tech_research_bonus = 0\r\n"
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
"army_tech_research_bonus = 0\r\n"
"}\r\n"
"fake_school_b = { \r\n"
"commerce_tech_research_bonus = 1\r\n"
"}\r\n"
"}\r\n";


class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation common = directory_representation(u"common", f_root);
	directory_representation technologies = directory_representation(u"technologies", f_root);

	file_representation tech_file = file_representation(u"technology.txt", common, fake_tech_file);

	file_representation ta = file_representation(u"army_tech.txt", technologies, single_tech);
	file_representation tb = file_representation(u"navy_tech.txt", technologies, two_techs);

	directory_representation inventions = directory_representation(u"inventions", f_root);
	file_representation ia = file_representation(u"army_tech.txt", inventions, "invention_a = {}\r\ninvention_b = { stuff }");
	file_representation ib = file_representation(u"navy_tech.txt", inventions, "invention_c = { x = false }");

	preparse_test_files() {
		set_default_root(f_root);
	}
};

TEST(technologies_tests, pre_parse_single_tech) {
	std::vector<token_group> results;

	tech_category_tag fake_cat(1);
	technologies_manager manager;
	text_data::text_sequences text;

	parse_pdx_file(results, single_tech, single_tech + sizeof(single_tech) - 1);
	pre_parse_single_tech_file(fake_cat, text, manager, results.data(), results.data() + results.size());

	EXPECT_EQ(1ui64, manager.technologies_container.size());
	EXPECT_EQ(tech_tag(0), manager.technologies_container[tech_tag(0)].id);
	EXPECT_EQ(text_data::text_tag(0), manager.technologies_container[tech_tag(0)].name);

	const auto find_result = manager.named_technology_index.find(text_data::text_tag(0));

	EXPECT_NE(manager.named_technology_index.end(), find_result);
	EXPECT_EQ(tech_tag(0), find_result->second);
}

TEST(technologies_tests, pre_parse_two_techs) {
	std::vector<token_group> results;

	tech_category_tag fake_cat(1);
	technologies_manager manager;
	text_data::text_sequences text;

	parse_pdx_file(results, two_techs, two_techs + sizeof(two_techs) - 1);
	pre_parse_single_tech_file(fake_cat, text, manager, results.data(), results.data() + results.size());

	ASSERT_EQ(2ui64, manager.technologies_container.size());

	const auto find_result = manager.named_technology_index.find(text_data::text_tag(0));

	ASSERT_NE(manager.named_technology_index.end(), find_result);
	ASSERT_EQ(tech_tag(0), find_result->second);
}

TEST(technologies_tests, pre_parse_tech_file) {

	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	std::vector<token_group> results;
	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	parse_pdx_file(results, fake_tech_file, fake_tech_file + sizeof(fake_tech_file) - 1);
	parse_main_technology_file(manager, results, text, f.get_root().get_directory(u"\\technologies"), mm);

	EXPECT_EQ(2ui64, manager.technology_categories.size());
	EXPECT_EQ(4ui64, manager.technology_subcategories.size());
	EXPECT_EQ(1ui64, manager.named_tech_school_index.size());

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

	EXPECT_EQ(modifiers::national_modifier_tag(0), mm.national_modifiers[modifiers::national_modifier_tag(0)].id);
	const auto fr = manager.named_tech_school_index[mm.national_modifiers[modifiers::national_modifier_tag(0)].name];
	EXPECT_EQ(modifiers::national_modifier_tag(0), fr);
}

TEST(technologies_tests, pre_parse_schools) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	std::vector<token_group> results;
	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	parse_pdx_file(results, fake_tech_file_b, fake_tech_file_b + sizeof(fake_tech_file_b) - 1);
	parse_main_technology_file(manager, results, text, f.get_root().get_directory(u"\\technologies"), mm);

	EXPECT_EQ(2ui64, mm.national_modifiers.size());
	EXPECT_EQ(2ui64, mm.named_national_modifiers_index.size());
	EXPECT_EQ(2ui64, manager.named_tech_school_index.size());

	EXPECT_EQ(modifiers::national_modifier_tag(0), mm.national_modifiers[modifiers::national_modifier_tag(0)].id);
	EXPECT_EQ(modifiers::national_modifier_tag(1), mm.national_modifiers[modifiers::national_modifier_tag(1)].id);
	const auto fr = manager.named_tech_school_index[mm.national_modifiers[modifiers::national_modifier_tag(0)].name];
	EXPECT_EQ(fr, modifiers::national_modifier_tag(0));
	const auto frb = manager.named_tech_school_index[mm.national_modifiers[modifiers::national_modifier_tag(1)].name];
	EXPECT_EQ(frb, modifiers::national_modifier_tag(1));

	EXPECT_EQ(1.0f, mm.national_modifier_definitions.get(frb, modifiers::national_offsets::commerce_tech_research_bonus));
}

TEST(technologies_tests, pre_parse_techs_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	const auto tech_dir = f.get_root().get_directory(u"\\technologies");

	parsing_state state(text, tech_dir, manager, mm);

	parse_technologies(state, f.get_root());

	EXPECT_EQ(3ui64, manager.technologies_container.size());
	EXPECT_EQ(2ui64, manager.technology_categories.size());
	EXPECT_EQ(4ui64, manager.technology_subcategories.size());
}

TEST(technologies_tests, pre_parse_inventions_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	const auto tech_dir = f.get_root().get_directory(u"\\technologies");

	parsing_state state(text, tech_dir, manager, mm);

	pre_parse_inventions(state, f.get_root());

	EXPECT_EQ(3ui64, manager.inventions.size());
	EXPECT_EQ(3ui64, manager.named_invention_index.size());

	EXPECT_EQ(invention_tag(0), manager.inventions[invention_tag(0)].id);
	EXPECT_EQ(invention_tag(1), manager.inventions[invention_tag(1)].id);
	EXPECT_EQ(invention_tag(2), manager.inventions[invention_tag(2)].id);

	EXPECT_EQ(invention_tag(0), manager.named_invention_index[manager.inventions[invention_tag(0)].name]);
	EXPECT_EQ(invention_tag(1), manager.named_invention_index[manager.inventions[invention_tag(1)].name]);
	EXPECT_EQ(invention_tag(2), manager.named_invention_index[manager.inventions[invention_tag(2)].name]);
}
