#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "population\\population.h"
#include "scenario\\scenario.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation poptypes = directory_representation(u"poptypes", f_root);
	file_representation f1 = file_representation(u"bureaucrats.txt", poptypes, "");
	file_representation f2 = file_representation(u"clergy.txt", poptypes, "");
	file_representation f3 = file_representation(u"aristocrats.txt", poptypes, "");
	directory_representation common1 = directory_representation(u"common", f_root);
	file_representation rebel_types = file_representation(u"rebel_types.txt", common1, 
		"rebel_ 1 = {}\r\n"
		"rebel_2 = { stuff }");
	file_representation pop_types = file_representation(u"pop_types.txt", common1, 
		"demotion_chance =\r\n"
		"{\r\n"
		"	factor = 1\r\n"
		"	modifier = {\r\n"
		"		factor = -0.1\r\n"
		"		not = { religion = this }\r\n"
		"		strata = poor\r\n"
		"	}"
		"}\r\n"
		"emigration_chance = \r\n"
		"{\r\n"
		"	factor = 0.5\r\n"
		"	group = {\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 5\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 6\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 7\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 8\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 9\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace population;

TEST(population_tests, test_pre_parse_file_scan) {
	population_manager manager;

	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	int bad_names = 0;
	text_data::text_sequences tex;
	pre_parse_pop_types(manager, f.get_root(), tex);

	EXPECT_EQ(0, bad_names);
	EXPECT_EQ(3ui32, manager.count_poptypes);
	EXPECT_EQ(3ui64, manager.pop_types.size());
	EXPECT_EQ(3ui64, manager.named_pop_type_index.size());

	EXPECT_EQ(pop_type_tag(1), manager.pop_types[pop_type_tag(1)].id);
	EXPECT_EQ(pop_type_tag(1), manager.named_pop_type_index[manager.pop_types[pop_type_tag(1)].name]);
}

TEST(population_tests, test_pre_parse_rebel_types) {
	population_manager manager;

	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	text_data::text_sequences tex;
	parsing_state env(tex, manager);

	pre_parse_rebel_types(env, f.get_root());

	EXPECT_EQ(2ui64, manager.rebel_types.size());
	EXPECT_EQ(2ui64, manager.named_rebel_type_index.size());

	EXPECT_EQ(rebel_type_tag(0), manager.rebel_types[rebel_type_tag(0)].id);
	EXPECT_EQ(rebel_type_tag(1), manager.rebel_types[rebel_type_tag(1)].id);

	EXPECT_EQ(rebel_type_tag(0), manager.named_rebel_type_index[manager.rebel_types[rebel_type_tag(0)].name]);
	EXPECT_EQ(rebel_type_tag(1), manager.named_rebel_type_index[manager.rebel_types[rebel_type_tag(1)].name]);
}

TEST(population_tests, test_poptype_file_read) {
	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	scenario::scenario_manager s;

	read_main_poptype_file(s, f.get_root());

	EXPECT_NE(modifiers::factor_tag(), s.population_m.emigration_chance);
	EXPECT_NE(modifiers::factor_tag(), s.population_m.demotion_chance);
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.colonialmigration_chance);
}
