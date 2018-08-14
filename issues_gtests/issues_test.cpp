#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "issues\\issues.h"
#include "scenario\\scenario.h"
#include "events\\events.h"
#include "issues\\issues_io.h"
#include "events\\events_io.h"

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
	directory_representation test4 = directory_representation(u"test4", f_root);
	directory_representation common4 = directory_representation(u"common", test4);
	file_representation issues4 = file_representation(u"issues.txt", common4,
		"party_issues = {\r\n"
		"	trade_policy = {\r\n"
		"		protectionism = {\r\n"
		"			max_tariff = 1\r\n"
		"			min_tariff = -0.25\r\n"
		"			rules = {\r\n"
		"				build_factory = no\r\n"
		"				expand_factory = no\r\n"
		"				open_factory = yes\r\n"
		"				destroy_factory = no\r\n"
		"				build_railway = no\r\n"
		"				factory_priority = no\r\n"
		"				can_subsidise = no\r\n"
		"			}\r\n"
		"		}\r\n"
		"		free_trade = {\r\n"
		"			max_tariff = 0.25\r\n"
		"			min_tariff = -1\r\n"
		"		}\r\n"
		"	}\r\n"
		"	war_policy = {\r\n"
		"		jingoism = {\r\n"
		"			max_military_spending = 1.0\r\n"
		"			supply_consumption = 0.1\r\n"
		"			war_exhaustion_effect = 0.5\r\n"
		"			is_jingoism = yes\r\n"
		"		}\r\n"
		"		pro_military = {\r\n"
		"			max_military_spending = 1.0\r\n"
		"			war_exhaustion_effect = 1.0\r\n"
		"			cb_generation_speed_modifier = 0.2\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		"\r\n"
		"political_reforms = {\r\n"
		"	slavery = {\r\n"
		"		next_step_only = yes\r\n"
		"		yes_slavery = {\r\n"
		"			rules = { slavery_allowed = yes }\r\n"
		"			administrative_multiplier = 1\r\n"
		"			allow = {\r\n"
		"				NOT = { has_country_flag = communist_gov }\r\n"
		"				NOT = { has_country_flag = fascist_gov }\r\n"
		"			}\r\n"
		"		}\r\n"
		"		freedom_of_womb = {\r\n"
		"			administrative_multiplier = 2\r\n"
		"			rules = { slavery_allowed = yes }\r\n"
		"			allow = {\r\n"
		"				NOT = { has_country_flag = communist_gov }\r\n"
		"				NOT = { has_country_flag = fascist_gov }\r\n"
		"				OR = {\r\n"
		"					NOT = { has_country_flag = the_slavery_debate }\r\n"
		"					year = 1875\r\n"
		"				}\r\n"
		"			}\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		"\r\n"
		"economic_reforms = {\r\n"
		"	pre_indust = {\r\n"
		"		no_pre_indust = {\r\n"
		"			core_pop_militancy_modifier = -0.005\r\n"
		"		}\r\n"
		"		yes_pre_indust = {\r\n"
		"			technology_cost = 18000\r\n"
		"			civilization_progress_modifier = 0.2\r\n"
		"			allow = { NOT = { has_country_flag = lacking_writing_system } money = 2000 }\r\n"
		"			on_execute = {\r\n"
		"				trigger = { year = 1850 }\r\n"
		"				effect = {\r\n"
		"					treasury = -2000\r\n"
		"					any_pop = {\r\n"
		"						militancy = 1\r\n"
		"					}\r\n"
		"				}\r\n"
		"			}\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
	);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

TEST(issues_tests, test_single_issue) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	issues_manager manager;
	text_data::text_sequences tex;

	auto result = pre_parse_issues(manager, f.get_root(), tex);

	EXPECT_EQ(1ui64, manager.named_issue_index.size());
	EXPECT_EQ(2ui64, manager.named_option_index.size());

	EXPECT_EQ(1ui64, manager.party_issues.size());
	EXPECT_EQ(1ui64, manager.issues_container.size());
	EXPECT_EQ(2ui64, manager.options.size());

	auto& issues = manager.issues_container[issue_tag(0)];

	EXPECT_EQ(issue_tag(0), issues.id);
	EXPECT_FALSE(issues.next_step_only);
	EXPECT_FALSE(issues.administrative);

	EXPECT_EQ(option_tag(0), issues.options[0]);
	EXPECT_EQ(option_tag(1), issues.options[1]);
	EXPECT_EQ(option_tag(), issues.options[2]);

	auto& fresult = manager.named_option_index[manager.options[option_tag(1)].name];
	EXPECT_EQ(option_tag(1), fresult);

	auto& fresultb = manager.named_issue_index[manager.issues_container[issue_tag(0)].name];
	EXPECT_EQ(issue_tag(0), fresultb);

}

TEST(issues_tests, test_multiple_issues) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	issues_manager manager;
	text_data::text_sequences tex;

	auto result = pre_parse_issues(manager, f.get_root(), tex);

	EXPECT_EQ(3ui64, manager.named_issue_index.size());
	EXPECT_EQ(4ui64, manager.named_option_index.size());

	EXPECT_EQ(1ui64, manager.party_issues.size());
	EXPECT_EQ(2ui64, manager.political_issues.size());
	EXPECT_EQ(3ui64, manager.issues_container.size());
	EXPECT_EQ(4ui64, manager.options.size());

	EXPECT_EQ(issue_tag(1), manager.political_issues[0]);

	auto& issuea = manager.issues_container[issue_tag(0)];
	auto& issueb = manager.issues_container[issue_tag(1)];
	auto& issuec = manager.issues_container[issue_tag(2)];

	EXPECT_EQ(issue_tag(0), issuea.id);
	EXPECT_FALSE(issuea.next_step_only);
	EXPECT_FALSE(issuea.administrative);
	EXPECT_EQ(option_tag(0), issuea.options[0]);
	EXPECT_EQ(option_tag(1), issuea.options[1]);
	EXPECT_EQ(option_tag(), issuea.options[2]);
	EXPECT_EQ(issue_group::party, issuea.type);

	EXPECT_EQ(issue_tag(1), issueb.id);
	EXPECT_FALSE(issueb.next_step_only);
	EXPECT_TRUE(issueb.administrative);
	EXPECT_EQ(option_tag(2), issueb.options[0]);
	EXPECT_EQ(option_tag(3), issueb.options[1]);
	EXPECT_EQ(option_tag(), issueb.options[2]);
	EXPECT_EQ(issue_group::political, issueb.type);

	EXPECT_EQ(issue_tag(2), issuec.id);
	EXPECT_TRUE(issuec.next_step_only);
	EXPECT_FALSE(issuec.administrative);
	EXPECT_EQ(option_tag(), issuec.options[0]);
	EXPECT_EQ(issue_group::political, issuec.type);

	auto& fresult = manager.named_option_index[manager.options[issueb.options[1]].name];
	EXPECT_EQ(option_tag(3), fresult);

	auto& fresultb = manager.named_issue_index[manager.issues_container[issue_tag(2)].name];
	EXPECT_EQ(issue_tag(2), fresultb);
}

TEST(issues_tests, unciv_issues) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test3"));

	issues_manager manager;
	text_data::text_sequences tex;

	auto result = pre_parse_issues(manager, f.get_root(), tex);

	EXPECT_EQ(3ui64, manager.named_issue_index.size());
	EXPECT_EQ(4ui64, manager.named_option_index.size());

	EXPECT_EQ(1ui64, manager.party_issues.size());
	EXPECT_EQ(2ui64, manager.economic_issues.size());
	EXPECT_EQ(3ui64, manager.issues_container.size());
	EXPECT_EQ(4ui64, manager.options.size());

	EXPECT_EQ(issue_tag(1), manager.economic_issues[0]);

	auto& issueb = manager.issues_container[issue_tag(1)];
	auto& issuec = manager.issues_container[issue_tag(2)];

	EXPECT_EQ(issue_tag(1), issueb.id);
	EXPECT_FALSE(issueb.next_step_only);
	EXPECT_TRUE(issueb.administrative);
	EXPECT_EQ(option_tag(2), issueb.options[0]);
	EXPECT_EQ(option_tag(3), issueb.options[1]);
	EXPECT_EQ(option_tag(), issueb.options[2]);
	EXPECT_EQ(issue_group::economic, issueb.type);

	EXPECT_EQ(issue_tag(2), issuec.id);
	EXPECT_TRUE(issuec.next_step_only);
	EXPECT_FALSE(issuec.administrative);
	EXPECT_EQ(option_tag(), issuec.options[0]);
	EXPECT_EQ(issue_group::economic, issuec.type);

	auto& fresult = manager.named_option_index[manager.options[issueb.options[0]].name];
	EXPECT_EQ(option_tag(2), fresult);

	auto& fresultb = manager.named_issue_index[manager.issues_container[issue_tag(1)].name];
	EXPECT_EQ(issue_tag(1), fresultb);
}

TEST(issues_tests, rule_reading) {
	const char text[] =
		"pop_expand_factory = yes\r\n"
		"all_voting = yes"
		;

	std::vector<token_group> parse_results;
	parse_pdx_file(parse_results, RANGE(text));

	auto result = read_rules(parse_results.data(), parse_results.data() + parse_results.size());

	EXPECT_EQ(rules::pop_expand_factory | rules::all_voting, result.rules_settings.rules);
	EXPECT_EQ(rules::pop_expand_factory | rules::citizens_rights_mask, result.rules_mask.rules);
}

TEST(issues_tests, full_parse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test4"));

	scenario::scenario_manager sm;
	events::event_creation_manager ecm;

	auto state = pre_parse_issues(sm.issues_m, f.get_root(), sm.gui_m.text_data_sequences);
	read_issue_options(state, sm, ecm);

	EXPECT_EQ(4ui64, sm.issues_m.issues_container.size());
	EXPECT_EQ(8ui64, sm.issues_m.options.size());
	EXPECT_EQ(option_tag(2), sm.issues_m.jingoism);
	EXPECT_EQ(1.0f, sm.issues_m.options[option_tag(3)].war_exhaustion_effect);
	EXPECT_EQ(2i16, sm.issues_m.options[option_tag(5)].administrative_multiplier);
	
	EXPECT_EQ(rules::build_factory | rules::expand_factory | rules::open_factory | rules::destroy_factory | rules::build_railway | rules::factory_priority | rules::can_subsidise, sm.issues_m.options[option_tag(0)].issue_rules.rules_mask.rules);
	EXPECT_EQ( rules::open_factory, sm.issues_m.options[option_tag(0)].issue_rules.rules_settings.rules);
	EXPECT_EQ(-1.0f, sm.modifiers_m.national_modifier_definitions[sm.issues_m.options[option_tag(1)].modifier][modifiers::national_offsets::min_tariff]);
	EXPECT_NE(0.0f, sm.modifiers_m.national_modifier_definitions[sm.issues_m.options[option_tag(6)].modifier][modifiers::national_offsets::core_pop_militancy_modifier]);
	EXPECT_NE(triggers::trigger_tag(), sm.issues_m.options[option_tag(4)].allow);
	EXPECT_EQ(triggers::trigger_tag(), sm.issues_m.options[option_tag(1)].allow);
	EXPECT_EQ(issue_tag(2), sm.issues_m.options[option_tag(4)].parent_issue);
	EXPECT_EQ(true, sm.issues_m.issues_container[issue_tag(2)].next_step_only);
	EXPECT_EQ(issue_group::political, sm.issues_m.issues_container[issue_tag(2)].type);
	EXPECT_NE(triggers::trigger_tag(), sm.issues_m.options[option_tag(7)].on_execute_trigger);
	EXPECT_NE(triggers::effect_tag(), sm.issues_m.options[option_tag(7)].on_execute_effect);
	EXPECT_EQ(triggers::trigger_tag(), sm.issues_m.options[option_tag(5)].on_execute_trigger);
	EXPECT_EQ(triggers::effect_tag(), sm.issues_m.options[option_tag(5)].on_execute_effect);
	EXPECT_EQ(issue_tag(3), sm.issues_m.options[option_tag(7)].parent_issue);
	EXPECT_EQ(false, sm.issues_m.issues_container[issue_tag(3)].next_step_only);
	EXPECT_EQ(issue_group::economic, sm.issues_m.issues_container[issue_tag(3)].type);
	EXPECT_EQ(option_tag(7), sm.issues_m.options[option_tag(7)].id);
	EXPECT_EQ(issue_tag(3), sm.issues_m.issues_container[issue_tag(3)].id);
	EXPECT_NE(text_data::text_tag(), sm.issues_m.options[option_tag(7)].name);
}
