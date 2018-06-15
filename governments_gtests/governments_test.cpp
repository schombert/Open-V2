#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "governments\\governments.h"
#include "ideologies\\ideologies.h"
#include "issues\\issues.h"
#include "scenario\\scenario.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace governments;

class test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation common1 = directory_representation(u"common", test1);
	file_representation gov1 = file_representation(u"governments.txt", common1,
		"proletarian_dictatorship  = {\r\n"
		"election = yes\r\n"
		"appoint_ruling_party  = yes\r\n"
		"flagType = monarchy\r\n"
		"duration = 10\r\n"
		"}\r\n");
	directory_representation test2 = directory_representation(u"test2", f_root);
	directory_representation common2 = directory_representation(u"common", test2);
	file_representation gov2 = file_representation(u"governments.txt", common2,
		"proletarian_dictatorship  = {\r\n"
		"ideology_2 = yes\r\n"
		"election = no\r\n"
		"appoint_ruling_party  = no\r\n"
		"flagType = fascist\r\n"
		"}\r\n");
	directory_representation test3 = directory_representation(u"test3", f_root);
	directory_representation common3 = directory_representation(u"common", test3);
	file_representation gov3 = file_representation(u"governments.txt", common3,
		"proletarian_dictatorship  = {\r\n"
		"ideology_2 = yes\r\n"
		"election = no\r\n"
		"appoint_ruling_party  = no\r\n"
		"flagType = none\r\n"
		"}\r\n"
		"type_b  = {\r\n"
		"ideology_1 = yes\r\n"
		"ideology_3 = yes\r\n"
		"election = yes\r\n"
		"appoint_ruling_party  = no\r\n"
		"flagType = communist\r\n"
		"}\r\n");
	file_representation issues1 = file_representation(u"issues.txt", common1,
		"party_issues  = {\r\n"
		"trade_policy  = { opt_a = { } opt_b = { } }\r\n"
		"issue_b  = { opt_c = { } opt_d = { } }\r\n"
		"}\r\n");
	file_representation file = file_representation(u"ideologies.txt", common1,
		"group_a = {\r\n"
		"member_1 = { }\r\n"
		"member_2 = { }\r\n"
		"}\r\n"
		"group_2 = { member_3 = { } }");

	test_files() {
		set_default_root(f_root);
	}
};

TEST(governments_tests, test_flag_types) {
	const char f1[] = "communist";
	const char f2[] = "republic";
	const char f3[] = "fascist";
	const char f4[] = "monarchy";
	const char f5[] = "uuuuu";

	EXPECT_EQ(flag_type::communist, text_to_flag_type(RANGE(f1)));
	EXPECT_EQ(flag_type::republic, text_to_flag_type(RANGE(f2)));
	EXPECT_EQ(flag_type::fascist, text_to_flag_type(RANGE(f3)));
	EXPECT_EQ(flag_type::monarchy, text_to_flag_type(RANGE(f4)));
	EXPECT_EQ(flag_type::not_specified, text_to_flag_type(RANGE(f5)));

	EXPECT_STREQ(f1, flag_type_to_text(flag_type::communist));
	EXPECT_STREQ(f2, flag_type_to_text(flag_type::republic));
	EXPECT_STREQ(f3, flag_type_to_text(flag_type::fascist));
	EXPECT_STREQ(f4, flag_type_to_text(flag_type::monarchy));
	EXPECT_EQ(nullptr, flag_type_to_text(flag_type::not_specified));
}

inline void setup_test_ideologies(text_data::text_sequences& ts, ideologies::ideologies_manager& m) {
	m.ideology_container.resize(3);
	static const char ideology_1[] = "ideology_1";
	static const char ideology_2[] = "ideology_2";
	static const char ideology_3[] = "ideology_3";
	m.named_ideology_index.emplace(text_data::get_thread_safe_text_handle(ts, RANGE(ideology_1)), ideologies::ideology_tag(0));
	m.named_ideology_index.emplace(text_data::get_thread_safe_text_handle(ts, RANGE(ideology_2)), ideologies::ideology_tag(1));
	m.named_ideology_index.emplace(text_data::get_thread_safe_text_handle(ts, RANGE(ideology_3)), ideologies::ideology_tag(2));
}

TEST(governments_tests, test_single_government) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	governments_manager m;
	text_data::text_sequences tex;
	ideologies::ideologies_manager im;
	setup_test_ideologies(tex, im);

	auto name_record = read_governments(m, f.get_root(), tex, im);

	EXPECT_EQ(1ui64, m.governments_container.size());
	EXPECT_EQ(1ui64, m.named_government_index.size());
	EXPECT_EQ(1ui64, m.permitted_ideologies.outer_size());
	EXPECT_EQ(3ui32, m.permitted_ideologies.inner_size());

	const auto& g = m.governments_container[government_tag(0)];
	EXPECT_EQ(government_tag(0), g.id);
	EXPECT_EQ(flag_type::monarchy, g.flag);
	EXPECT_EQ(10ui32, g.duration);
	EXPECT_EQ(true, g.appoint_ruling_party);
	EXPECT_EQ(true, g.election);
	EXPECT_EQ(std::string("proletarian_dictatorship"), name_record[government_tag(0)]);
	EXPECT_EQ(government_tag(0), m.named_government_index[g.name]);

	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(0)));
	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(1)));
	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(2)));
}

TEST(governments_tests, test_single_ideology) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	governments_manager m;
	ideologies::ideologies_manager im;
	text_data::text_sequences tex;
	setup_test_ideologies(tex, im);

	auto name_record = read_governments(m, f.get_root(), tex, im);

	EXPECT_EQ(1ui64, m.governments_container.size());
	EXPECT_EQ(1ui64, m.named_government_index.size());
	EXPECT_EQ(1ui64, m.permitted_ideologies.outer_size());
	EXPECT_EQ(3ui32, m.permitted_ideologies.inner_size());

	const auto& g = m.governments_container[government_tag(0)];
	EXPECT_EQ(government_tag(0), g.id);
	EXPECT_EQ(flag_type::fascist, g.flag);
	EXPECT_EQ(48ui32, g.duration);
	EXPECT_EQ(false, g.appoint_ruling_party);
	EXPECT_EQ(false, g.election);
	EXPECT_EQ(std::string("proletarian_dictatorship"), name_record[government_tag(0)]);
	EXPECT_EQ(government_tag(0), m.named_government_index[g.name]);

	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(0)));
	EXPECT_EQ(1ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(1)));
	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(2)));
}

TEST(governments_tests, test_multiple) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test3"));

	governments_manager m;
	text_data::text_sequences tex;
	ideologies::ideologies_manager im;
	setup_test_ideologies(tex, im);

	auto name_record = read_governments(m, f.get_root(), tex, im);

	EXPECT_EQ(2ui64, m.governments_container.size());
	EXPECT_EQ(2ui64, m.named_government_index.size());
	EXPECT_EQ(2ui64, m.permitted_ideologies.outer_size());
	EXPECT_EQ(3ui32, m.permitted_ideologies.inner_size());

	const auto& g = m.governments_container[government_tag(0)];
	EXPECT_EQ(government_tag(0), g.id);
	EXPECT_EQ(flag_type::not_specified, g.flag);
	EXPECT_EQ(48ui32, g.duration);
	EXPECT_EQ(false, g.appoint_ruling_party);
	EXPECT_EQ(false, g.election);
	EXPECT_EQ(std::string("proletarian_dictatorship"), name_record[government_tag(0)]);
	EXPECT_EQ(government_tag(0), m.named_government_index[g.name]);

	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(0)));
	EXPECT_EQ(1ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(1)));
	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(0), ideologies::ideology_tag(2)));

	const auto& g2 = m.governments_container[government_tag(1)];
	EXPECT_EQ(government_tag(1), g2.id);
	EXPECT_EQ(flag_type::communist, g2.flag);
	EXPECT_EQ(48ui32, g2.duration);
	EXPECT_EQ(false, g2.appoint_ruling_party);
	EXPECT_EQ(true, g2.election);
	EXPECT_EQ(std::string("type_b"), name_record[government_tag(1)]);
	EXPECT_EQ(government_tag(1), m.named_government_index[g2.name]);

	EXPECT_EQ(1ui8, m.permitted_ideologies.get(government_tag(1), ideologies::ideology_tag(0)));
	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(1), ideologies::ideology_tag(1)));
	EXPECT_EQ(1ui8, m.permitted_ideologies.get(government_tag(1), ideologies::ideology_tag(2)));
}

TEST(governments_tests, individual_party) {
	test_files real_fs;
	file_system f;

	scenario::scenario_manager s;

	f.set_root(RANGE(u"F:\\test1"));

	issues::pre_parse_issues(s.issues_m, f.get_root(), s.gui_m.text_data_sequences);
	ideologies::pre_parse_ideologies(s.ideologies_m, f.get_root(), s.gui_m.text_data_sequences);

	const char party_contents[] = "start_date = 1846.1.2\r\n"
		"end_date = 1991.1.10\r\n"
		"ideology = member_1\r\n"
		"name = \"BHO_liberal\"\r\n"
		"issue_b = opt_c\r\n"
		"trade_policy = opt_b";

	std::vector<token_group> results;
	parse_pdx_file(results, RANGE(party_contents));

	ready_party_issues(s.governments_m, s.issues_m);

	auto pt = read_party(results.data(), results.data() + results.size(), s);

	EXPECT_EQ(party_tag(0), pt);
	EXPECT_EQ(party_tag(0), s.governments_m.parties[party_tag(0)].id);
	EXPECT_EQ(date_to_tag(boost::gregorian::date(1846, boost::gregorian::Jan, 2)), s.governments_m.parties[party_tag(0)].start_date);
	EXPECT_EQ(date_to_tag(boost::gregorian::date(1991, boost::gregorian::Jan, 10)), s.governments_m.parties[party_tag(0)].end_date);
	EXPECT_EQ(
		tag_from_text(s.ideologies_m.named_ideology_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("member_1"))),
		s.governments_m.parties[party_tag(0)].ideology);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("BHO_liberal")), s.governments_m.parties[party_tag(0)].name);
	EXPECT_EQ(s.governments_m.party_issues.get(party_tag(0), 0), issues::option_tag(1));
	EXPECT_EQ(s.governments_m.party_issues.get(party_tag(0), 1), issues::option_tag(2));
}
