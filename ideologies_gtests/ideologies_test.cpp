#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "ideologies\\ideologies.h"
#include "scenario\\scenario.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation common = directory_representation(u"common", f_root);
	file_representation file = file_representation(u"ideologies.txt", common, 
		"group_a = {\r\n"
		"member_1 = { stuff }\r\n"
		"member_2 = {}\r\n"
		"}\r\n"
		"group_2 = { member_3 = { a b c } }");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation common1 = directory_representation(u"common", test1);
	file_representation file1 = file_representation(u"ideologies.txt", common1,
		"group_a = {\r\n"
		"ideology_a = {\r\n"
		"can_reduce_militancy = yes\r\n"
		"color = { 10 10 250 }\r\n"
		"remove_social_reform = {\r\n"
		"	base = 1"
		"}\r\n"
		"date = 1905.1.1\r\n"
		"uncivilized = no\r\n"
		"}\r\n"
		"ideology_b = {\r\n"
		"color = { 30 40 50 }\r\n"
		"add_political_reform = {\r\n"
		"	base = 2"
		"	group = {\r\n"
		"		modifier = {\r\n"
		"			factor = 0.1\r\n"
		"			OR = {\r\n"
		"				militancy = 1\r\n"
		"				political_movement_strength = 0.1\r\n"
		"			}\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		"}\r\n"
		"}\r\n");


	preparse_test_files() {
		set_default_root(f_root);
	}
};

auto fake_text_handle_lookup();

auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return text_data::text_tag(i++); };
}


using namespace ideologies;

TEST(ideologies_tests, preparse_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	ideologies_manager manager;
	text_data::text_sequences tex;

	auto result = pre_parse_ideologies(manager, f.get_root(), tex);

	ASSERT_EQ(3ui64, manager.ideology_container.size());
	ASSERT_EQ(2ui64, manager.ideology_groups.size());

	ASSERT_EQ(3ui64, manager.named_ideology_index.size());
	ASSERT_EQ(2ui64, manager.named_group_index.size());

	ASSERT_EQ(ideology_tag(0), manager.ideology_container[ideology_tag(0)].id);
	ASSERT_EQ(ideology_group_tag(0), manager.ideology_container[ideology_tag(0)].group);
	ASSERT_EQ(ideology_tag(1), manager.ideology_container[ideology_tag(1)].id);
	ASSERT_EQ(ideology_group_tag(0), manager.ideology_container[ideology_tag(1)].group);
	ASSERT_EQ(ideology_tag(2), manager.ideology_container[ideology_tag(2)].id);
	ASSERT_EQ(ideology_group_tag(1), manager.ideology_container[ideology_tag(2)].group);

	ASSERT_EQ(ideology_group_tag(0), manager.ideology_groups[ideology_group_tag(0)].id);
	ASSERT_EQ(ideology_group_tag(1), manager.ideology_groups[ideology_group_tag(1)].id);
}

TEST(ideologies_tests, single_ideology) {
	scenario::scenario_manager sm;
	graphics::texture_manager tex;

	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	sm.ideologies_m.ideology_container.emplace_back();
	sm.ideologies_m.ideology_container.emplace_back();
	sm.ideologies_m.ideology_container[ideology_tag(0)].id = ideology_tag(0);
	sm.ideologies_m.ideology_container[ideology_tag(1)].id = ideology_tag(1);

	{
		const char text[] =
			"can_reduce_militancy = yes\r\n"
			"color = { 10 10 250 }\r\n"
			"remove_social_reform = {\r\n"
			"	base = 1"
			"}\r\n"
			"date = 1905.1.1\r\n"
			"uncivilized = no\r\n";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		read_single_ideology(sm, ideology_tag(0), parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(ideology_tag(0), sm.ideologies_m.conservative_ideology);
		EXPECT_EQ(false, sm.ideologies_m.ideology_container[ideology_tag(0)].uncivilized);
		EXPECT_EQ(date_to_tag(boost::gregorian::date(1905,1,1)), sm.ideologies_m.ideology_container[ideology_tag(0)].enable_date);
		EXPECT_EQ(10ui8, sm.ideologies_m.ideology_container[ideology_tag(0)].color.r);
		EXPECT_EQ(10ui8, sm.ideologies_m.ideology_container[ideology_tag(0)].color.g);
		EXPECT_EQ(250ui8, sm.ideologies_m.ideology_container[ideology_tag(0)].color.b);
		EXPECT_EQ(modifiers::factor_tag(0), sm.ideologies_m.ideology_container[ideology_tag(0)].remove_social_reform);
	}

	{
		const char text[] =
			"color = { 30 40 50 }\r\n"
			"add_political_reform = {\r\n"
			"	base = 2"
			"	group = {\r\n"
			"		modifier = {\r\n"
			"			factor = 0.1\r\n"
			"			OR = {\r\n"
			"				militancy = 1\r\n"
			"				political_movement_strength = 0.1\r\n"
			"			}\r\n"
			"		}\r\n"
			"	}\r\n"
			"}\r\n";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		read_single_ideology(sm, ideology_tag(1), parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(ideology_tag(0), sm.ideologies_m.conservative_ideology);
		EXPECT_EQ(true, sm.ideologies_m.ideology_container[ideology_tag(1)].uncivilized);
		EXPECT_EQ(date_tag(), sm.ideologies_m.ideology_container[ideology_tag(1)].enable_date);
		EXPECT_EQ(30ui8, sm.ideologies_m.ideology_container[ideology_tag(1)].color.r);
		EXPECT_EQ(40ui8, sm.ideologies_m.ideology_container[ideology_tag(1)].color.g);
		EXPECT_EQ(50ui8, sm.ideologies_m.ideology_container[ideology_tag(1)].color.b);
		EXPECT_EQ(modifiers::factor_tag(1), sm.ideologies_m.ideology_container[ideology_tag(1)].add_political_reform);
	}
}

TEST(ideologies_tests, full_file_parse) {
	scenario::scenario_manager sm;
	graphics::texture_manager tex;

	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));
	auto result = pre_parse_ideologies(sm.ideologies_m, f.get_root(), sm.gui_m.text_data_sequences);
	read_ideologies(sm, result);

	EXPECT_EQ(ideology_tag(0), sm.ideologies_m.conservative_ideology);
	EXPECT_EQ(false, sm.ideologies_m.ideology_container[ideology_tag(0)].uncivilized);
	EXPECT_EQ(date_to_tag(boost::gregorian::date(1905, 1, 1)), sm.ideologies_m.ideology_container[ideology_tag(0)].enable_date);
	EXPECT_EQ(10ui8, sm.ideologies_m.ideology_container[ideology_tag(0)].color.r);
	EXPECT_EQ(10ui8, sm.ideologies_m.ideology_container[ideology_tag(0)].color.g);
	EXPECT_EQ(250ui8, sm.ideologies_m.ideology_container[ideology_tag(0)].color.b);
	EXPECT_EQ(modifiers::factor_tag(0), sm.ideologies_m.ideology_container[ideology_tag(0)].remove_social_reform);
	EXPECT_EQ(ideology_tag(0), sm.ideologies_m.ideology_container[ideology_tag(0)].id);
	EXPECT_EQ(ideology_group_tag(0), sm.ideologies_m.ideology_container[ideology_tag(0)].group);

	EXPECT_EQ(true, sm.ideologies_m.ideology_container[ideology_tag(1)].uncivilized);
	EXPECT_EQ(date_tag(), sm.ideologies_m.ideology_container[ideology_tag(1)].enable_date);
	EXPECT_EQ(30ui8, sm.ideologies_m.ideology_container[ideology_tag(1)].color.r);
	EXPECT_EQ(40ui8, sm.ideologies_m.ideology_container[ideology_tag(1)].color.g);
	EXPECT_EQ(50ui8, sm.ideologies_m.ideology_container[ideology_tag(1)].color.b);
	EXPECT_EQ(modifiers::factor_tag(1), sm.ideologies_m.ideology_container[ideology_tag(1)].add_political_reform);
	EXPECT_EQ(ideology_tag(1), sm.ideologies_m.ideology_container[ideology_tag(1)].id);
	EXPECT_EQ(ideology_group_tag(0), sm.ideologies_m.ideology_container[ideology_tag(1)].group);
}
