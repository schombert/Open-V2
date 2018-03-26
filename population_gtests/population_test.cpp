#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "population\\population.h"

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

	preparse_test_files() {
		set_default_root(f_root);
	}
};

inline auto fake_text_handle_lookup(int& outer_counter) {
	return[i = 0ui16, &outer_counter](const char* s, const char* e) mutable {
		std::string temp(s, e);
		if (temp != "bureaucrats" && temp != "clergy" && temp != "aristocrats")
			++outer_counter;
		return text_data::text_tag(i++);
	};
}

inline auto fake_text_handle_lookup_b(std::map<std::string, text_data::text_tag>& values) {
	return[j = 0ui16, &values](const char* s, const char* e) mutable {
		const auto i = std::string(s, e);
		if (values.find(i) == values.end()) {
			values[i] = text_data::text_tag(j++);
		}
		return values[i];
	};
}

using namespace population;

TEST(population_tests, test_pre_parse_file_scan) {
	population_manager manager;

	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	int bad_names = 0;
	pre_parse_pop_types(manager, f.get_root(), fake_text_handle_lookup(bad_names));

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

	std::map<std::string, text_data::text_tag> text;
	parsing_state env(fake_text_handle_lookup_b(text), manager);

	pre_parse_rebel_types(env, f.get_root());

	EXPECT_EQ(2ui64, manager.rebel_types.size());
	EXPECT_EQ(2ui64, manager.named_rebel_type_index.size());

	EXPECT_EQ(rebel_type_tag(0), manager.rebel_types[rebel_type_tag(0)].id);
	EXPECT_EQ(rebel_type_tag(1), manager.rebel_types[rebel_type_tag(1)].id);

	EXPECT_EQ(rebel_type_tag(0), manager.named_rebel_type_index[manager.rebel_types[rebel_type_tag(0)].name]);
	EXPECT_EQ(rebel_type_tag(1), manager.named_rebel_type_index[manager.rebel_types[rebel_type_tag(1)].name]);
}
