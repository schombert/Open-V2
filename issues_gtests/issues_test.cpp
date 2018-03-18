#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "issues\\issues.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace issues;

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation common1 = directory_representation(u"common", test1);
	file_representation issues1 = file_representation(u"issues.txt", common1,
		"party_issues  = {\r\n"
		"trade_policy  = { opt_a = { stuff } opt_b = { stuff } }\r\n"
		"}\r\n");
	directory_representation test2 = directory_representation(u"test2", f_root);
	directory_representation common2 = directory_representation(u"common", test2);
	file_representation issues2 = file_representation(u"issues.txt", common2,
		"party_issues  = {\r\n"
		"trade_policy  = { opt_a = { stuff } opt_b = { stuff } }\r\n"
		"}\r\n"
		"political_reforms = {\r\n"
		"slavery = { opt_d = {} opt_q = {} \r\n"
		"administrative = yes\r\n"
		"}\r\n"
		"vote_franschise = { next_step_only = yes\r\n"
		"}\r\n"
		"}\r\n");
	directory_representation test3 = directory_representation(u"test3", f_root);
	directory_representation common3 = directory_representation(u"common", test3);
	file_representation issues3 = file_representation(u"issues.txt", common3,
		"party_issues  = {\r\n"
		"trade_policy  = { opt_a = { stuff } opt_b = { stuff } }\r\n"
		"}\r\n"
		"economic_reforms = {\r\n"
		"slavery = { opt_d = {} opt_q = {} \r\n"
		"administrative = yes\r\n"
		"}\r\n"
		"vote_franschise = { next_step_only = yes\r\n"
		"}\r\n"
		"}\r\n");

	preparse_test_files() {
		set_default_root(f_root);
	}
};

inline auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return text_data::text_tag(i++); };
}

TEST(issues_tests, test_single_issue) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	issues_manager manager;

	auto result = pre_parse_issues(manager, f.get_root(), fake_text_handle_lookup());

	EXPECT_EQ(1ui64, manager.named_issue_index.size());
	EXPECT_EQ(2ui64, manager.named_option_index.size());

	EXPECT_EQ(1ui64, manager.party_issues.size());
	EXPECT_EQ(1ui64, manager.issues_cotnainer.size());

	auto& issues = manager.issues_cotnainer[issue_tag(0)];

	EXPECT_EQ(issue_tag(0), issues.id);
	EXPECT_FALSE(issues.next_step_only);
	EXPECT_FALSE(issues.administrative);
	EXPECT_EQ(2ui64, issues.options.size());

	EXPECT_EQ(option_tag(0), issues.options[option_tag(0)].id);
	EXPECT_EQ(option_tag(1), issues.options[option_tag(1)].id);

	auto& fresult = manager.named_option_index[issues.options[option_tag(1)].name];
	EXPECT_EQ(option_tag(1), fresult.option_id);
	EXPECT_EQ(issue_group::party, fresult.type);
	EXPECT_EQ(issue_tag(0), std::get<issue_tag>(fresult.id));

	auto& fresultb = manager.named_issue_index[manager.issues_cotnainer[issue_tag(0)].name];
	EXPECT_EQ(issue_group::party, fresultb.type);
	EXPECT_EQ(issue_tag(0), std::get<issue_tag>(fresultb.id));

}

TEST(issues_tests, test_multiple_issues) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	issues_manager manager;

	auto result = pre_parse_issues(manager, f.get_root(), fake_text_handle_lookup());

	EXPECT_EQ(3ui64, manager.named_issue_index.size());
	EXPECT_EQ(4ui64, manager.named_option_index.size());

	EXPECT_EQ(1ui64, manager.party_issues.size());
	EXPECT_EQ(2ui64, manager.political_issues.size());
	EXPECT_EQ(3ui64, manager.issues_cotnainer.size());

	EXPECT_EQ(issue_tag(1), manager.political_issues[0]);

	auto& issuea = manager.issues_cotnainer[issue_tag(0)];
	auto& issueb = manager.issues_cotnainer[issue_tag(1)];
	auto& issuec = manager.issues_cotnainer[issue_tag(2)];

	EXPECT_EQ(issue_tag(0), issuea.id);
	EXPECT_FALSE(issuea.next_step_only);
	EXPECT_FALSE(issuea.administrative);
	EXPECT_EQ(2ui64, issuea.options.size());
	EXPECT_EQ(option_tag(0), issuea.options[option_tag(0)].id);
	EXPECT_EQ(option_tag(1), issuea.options[option_tag(1)].id);
	EXPECT_EQ(issue_group::party, issuea.type);

	EXPECT_EQ(issue_tag(1), issueb.id);
	EXPECT_FALSE(issueb.next_step_only);
	EXPECT_TRUE(issueb.administrative);
	EXPECT_EQ(2ui64, issueb.options.size());
	EXPECT_EQ(option_tag(0), issueb.options[option_tag(0)].id);
	EXPECT_EQ(option_tag(1), issueb.options[option_tag(1)].id);
	EXPECT_EQ(issue_group::political, issueb.type);

	EXPECT_EQ(issue_tag(2), issuec.id);
	EXPECT_TRUE(issuec.next_step_only);
	EXPECT_FALSE(issuec.administrative);
	EXPECT_EQ(0ui64, issuec.options.size());
	EXPECT_EQ(issue_group::political, issuec.type);

	auto& fresult = manager.named_option_index[issueb.options[option_tag(0)].name];
	EXPECT_EQ(option_tag(0), fresult.option_id);
	EXPECT_EQ(issue_group::political, fresult.type);
	EXPECT_EQ(issue_tag(1), std::get<issue_tag>(fresult.id));

	auto& fresultb = manager.named_issue_index[manager.issues_cotnainer[issue_tag(2)].name];
	EXPECT_EQ(issue_group::political, fresultb.type);
	EXPECT_EQ(issue_tag(2), std::get<issue_tag>(fresultb.id));
}

TEST(issues_tests, unciv_issues) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test3"));

	issues_manager manager;

	auto result = pre_parse_issues(manager, f.get_root(), fake_text_handle_lookup());

	EXPECT_EQ(3ui64, manager.named_issue_index.size());
	EXPECT_EQ(4ui64, manager.named_option_index.size());

	EXPECT_EQ(1ui64, manager.party_issues.size());
	EXPECT_EQ(2ui64, manager.economic_issues.size());
	EXPECT_EQ(1ui64, manager.issues_cotnainer.size());
	EXPECT_EQ(2ui64, manager.unciv_issues_cotnainer.size());

	EXPECT_EQ(unciv_issue_tag(0), manager.economic_issues[0]);

	auto& issueb = manager.unciv_issues_cotnainer[unciv_issue_tag(0)];
	auto& issuec = manager.unciv_issues_cotnainer[unciv_issue_tag(1)];

	EXPECT_EQ(unciv_issue_tag(0), issueb.id);
	EXPECT_FALSE(issueb.next_step_only);
	EXPECT_TRUE(issueb.administrative);
	EXPECT_EQ(2ui64, issueb.options.size());
	EXPECT_EQ(option_tag(0), issueb.options[option_tag(0)].id);
	EXPECT_EQ(option_tag(1), issueb.options[option_tag(1)].id);
	EXPECT_EQ(issue_group::economic, issueb.type);

	EXPECT_EQ(unciv_issue_tag(1), issuec.id);
	EXPECT_TRUE(issuec.next_step_only);
	EXPECT_FALSE(issuec.administrative);
	EXPECT_EQ(0ui64, issuec.options.size());
	EXPECT_EQ(issue_group::economic, issuec.type);

	auto& fresult = manager.named_option_index[issueb.options[option_tag(0)].name];
	EXPECT_EQ(option_tag(0), fresult.option_id);
	EXPECT_EQ(issue_group::economic, fresult.type);
	EXPECT_EQ(unciv_issue_tag(0), std::get<unciv_issue_tag>(fresult.id));

	auto& fresultb = manager.named_issue_index[manager.unciv_issues_cotnainer[unciv_issue_tag(1)].name];
	EXPECT_EQ(issue_group::economic, fresultb.type);
	EXPECT_EQ(unciv_issue_tag(1), std::get<unciv_issue_tag>(fresultb.id));
}
