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
