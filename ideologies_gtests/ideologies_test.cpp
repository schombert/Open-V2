#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "ideologies\\ideologies.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation common = directory_representation(u"common", f_root);
	file_representation tech_file = file_representation(u"ideologies.txt", common, 
		"group_a = {\r\n"
		"member_1 = { stuff }\r\n"
		"member_2 = {}\r\n"
		"}\r\n"
		"group_2 = { member_3 = { a b c } }");

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

	auto result = pre_parse_ideologies(manager, f.get_root(), fake_text_handle_lookup());

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
