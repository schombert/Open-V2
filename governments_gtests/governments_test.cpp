#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "governments\\governments.h"
#include "ideologies\\ideologies.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace governments;

inline auto fake_text_handle_lookup() {
	return[i = 100ui16](const char* s, const char* e) mutable {
		if (is_fixed_token_ci(s, e, "ideology_1"))
			return text_data::text_tag(0);
		else if (is_fixed_token_ci(s, e, "ideology_2"))
			return text_data::text_tag(1);
		else if (is_fixed_token_ci(s, e, "ideology_3"))
			return text_data::text_tag(2);
		return text_data::text_tag(i++);
	};
}

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

inline void setup_test_ideologies(ideologies::ideologies_manager& m) {
	m.ideology_container.resize(3);
	m.named_ideology_index.emplace(text_data::text_tag(0), ideologies::ideology_tag(0));
	m.named_ideology_index.emplace(text_data::text_tag(1), ideologies::ideology_tag(1));
	m.named_ideology_index.emplace(text_data::text_tag(2), ideologies::ideology_tag(2));
}

TEST(governments_tests, test_single_government) {
	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	governments_manager m;
	ideologies::ideologies_manager im;
	setup_test_ideologies(im);

	read_governments(m, f.get_root(), fake_text_handle_lookup(), im);

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
	EXPECT_EQ(std::string("proletarian_dictatorship"), g.name_base);
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
	setup_test_ideologies(im);

	read_governments(m, f.get_root(), fake_text_handle_lookup(), im);

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
	EXPECT_EQ(std::string("proletarian_dictatorship"), g.name_base);
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
	ideologies::ideologies_manager im;
	setup_test_ideologies(im);

	read_governments(m, f.get_root(), fake_text_handle_lookup(), im);

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
	EXPECT_EQ(std::string("proletarian_dictatorship"), g.name_base);
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
	EXPECT_EQ(std::string("type_b"), g2.name_base);
	EXPECT_EQ(government_tag(1), m.named_government_index[g2.name]);

	EXPECT_EQ(1ui8, m.permitted_ideologies.get(government_tag(1), ideologies::ideology_tag(0)));
	EXPECT_EQ(0ui8, m.permitted_ideologies.get(government_tag(1), ideologies::ideology_tag(1)));
	EXPECT_EQ(1ui8, m.permitted_ideologies.get(government_tag(1), ideologies::ideology_tag(2)));
}
