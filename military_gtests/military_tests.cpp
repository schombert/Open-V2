#include "military\\military.h"
#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

inline auto fake_text_handle_lookup(std::map<std::string, text_data::text_tag>& values) {
	return[j = 0ui16, &values](const char* s, const char* e) mutable {
		const auto i = std::string(s, e);
		if (values.find(i) == values.end()) {
			values[i] = text_data::text_tag(j++);
		}
		return values[i];
	};
}

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation common = directory_representation(u"common", test1);
	file_representation cb = file_representation(u"cb_types.txt", common,
		"peace_order = {\r\n"
		"cb_a\r\n"
		"cb_b\r\n"
		"}\r\n"
		"cb_b = { stuff }\r\n"
		"cb_a = { stuff }\r\n"
		"cb_c = { stuff }\r\n");
	directory_representation units = directory_representation(u"units", f_root);
	file_representation f1 = file_representation(u"dragoon.txt", units, "dragoon = { stuff }");
	file_representation f2 = file_representation(u"airplane.txt", units, "plane_a = { stuff }\r\nplane_b = {}");

	directory_representation test2 = directory_representation(u"test2", f_root);
	directory_representation common2 = directory_representation(u"common", test2);
	file_representation traits2 = file_representation(u"traits.txt", common2, "personality = { no_personality = { morale = 0.5 } test_personality = { defence = 1.0 }}");

	directory_representation test3 = directory_representation(u"test3", f_root);
	directory_representation common3 = directory_representation(u"common", test3);
	file_representation traits3 = file_representation(u"traits.txt", common3,
		"personality = { no_personality = { morale = 0.5 } test_personality = { defence = 1.0 }} \r\n"
		"background = { no_background = {} b1 = { reliability = 2.0 speed = 4.0 } b2 = {}}"
	);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace military;

TEST(military_tests, test_units_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	military_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);
	
	pre_parse_unit_types(state, f.get_root());

	EXPECT_EQ(3ui64, m.unit_types.size());
	EXPECT_EQ(3ui64, m.named_unit_type_index.size());

	EXPECT_EQ(unit_type_tag(0), m.unit_types[unit_type_tag(0)].id);
	EXPECT_EQ(unit_type_tag(1), m.unit_types[unit_type_tag(1)].id);
	EXPECT_EQ(unit_type_tag(2), m.unit_types[unit_type_tag(2)].id);
	EXPECT_EQ(unit_type_tag(0), m.named_unit_type_index[v["dragoon"]]);
	EXPECT_EQ(unit_type_tag(1), m.named_unit_type_index[m.unit_types[unit_type_tag(1)].name]);
	EXPECT_EQ(unit_type_tag(2), m.named_unit_type_index[m.unit_types[unit_type_tag(2)].name]);
}

TEST(military_tests, test_cb_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	military_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);

	pre_parse_cb_types(state, f.get_root());

	EXPECT_EQ(3ui64, m.cb_types.size());
	EXPECT_EQ(3ui64, m.named_cb_type_index.size());

	EXPECT_EQ(cb_type_tag(0), m.cb_types[cb_type_tag(0)].id);
	EXPECT_EQ(cb_type_tag(1), m.cb_types[cb_type_tag(1)].id);
	EXPECT_EQ(cb_type_tag(2), m.cb_types[cb_type_tag(2)].id);
	EXPECT_EQ(cb_type_tag(0), m.named_cb_type_index[v["cb_a"]]);
	EXPECT_EQ(cb_type_tag(1), m.named_cb_type_index[m.cb_types[cb_type_tag(1)].name]);
	EXPECT_EQ(cb_type_tag(2), m.named_cb_type_index[m.cb_types[cb_type_tag(2)].name]);
}

TEST(military_tests, traits_personality) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	military_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);

	parse_leader_traits(state, f.get_root());

	EXPECT_EQ(2ui64, m.leader_traits.size());
	EXPECT_EQ(1ui64, m.personality_traits.size());
	EXPECT_EQ(0ui64, m.background_traits.size());
	EXPECT_EQ(leader_trait_tag(), m.no_background_trait);
	EXPECT_EQ(leader_trait_tag(0), m.no_personality_trait);
	EXPECT_EQ(leader_trait_tag(0), m.named_leader_trait_index[v["no_personality"]]);
	EXPECT_EQ(leader_trait_tag(1), m.named_leader_trait_index[v["test_personality"]]);
	EXPECT_EQ(leader_trait_tag(1), m.personality_traits[0]);
	EXPECT_EQ(v["no_personality"], m.leader_traits[leader_trait_tag(0)]);
	EXPECT_EQ(v["test_personality"], m.leader_traits[leader_trait_tag(1)]);

	EXPECT_EQ(0.5f, m.leader_trait_definitions.get(leader_trait_tag(0), traits::morale));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(0), traits::organisation));
	EXPECT_EQ(1.0f, m.leader_trait_definitions.get(leader_trait_tag(1), traits::defence));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(1), traits::reliability));
}

TEST(military_tests, traits_mixed) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test3"));

	military_manager m;
	std::map<std::string, text_data::text_tag> v;
	parsing_state state(fake_text_handle_lookup(v), m);

	parse_leader_traits(state, f.get_root());

	EXPECT_EQ(5ui64, m.leader_traits.size());
	EXPECT_EQ(1ui64, m.personality_traits.size());
	EXPECT_EQ(2ui64, m.background_traits.size());
	EXPECT_EQ(leader_trait_tag(2), m.no_background_trait);
	EXPECT_EQ(leader_trait_tag(0), m.no_personality_trait);
	EXPECT_EQ(leader_trait_tag(0), m.named_leader_trait_index[v["no_personality"]]);
	EXPECT_EQ(leader_trait_tag(1), m.named_leader_trait_index[v["test_personality"]]);
	EXPECT_EQ(leader_trait_tag(2), m.named_leader_trait_index[v["no_background"]]);
	EXPECT_EQ(leader_trait_tag(3), m.named_leader_trait_index[v["b1"]]);
	EXPECT_EQ(leader_trait_tag(4), m.named_leader_trait_index[v["b2"]]);

	EXPECT_EQ(leader_trait_tag(1), m.personality_traits[0]);
	EXPECT_EQ(leader_trait_tag(3), m.background_traits[0]);
	EXPECT_EQ(leader_trait_tag(4), m.background_traits[1]);

	EXPECT_EQ(v["no_personality"], m.leader_traits[leader_trait_tag(0)]);
	EXPECT_EQ(v["test_personality"], m.leader_traits[leader_trait_tag(1)]);
	EXPECT_EQ(v["no_background"], m.leader_traits[leader_trait_tag(2)]);
	EXPECT_EQ(v["b1"], m.leader_traits[leader_trait_tag(3)]);
	EXPECT_EQ(v["b2"], m.leader_traits[leader_trait_tag(4)]);

	EXPECT_EQ(0.5f, m.leader_trait_definitions.get(leader_trait_tag(0), traits::morale));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(0), traits::organisation));
	EXPECT_EQ(1.0f, m.leader_trait_definitions.get(leader_trait_tag(1), traits::defence));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(1), traits::reliability));

	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(2), traits::morale));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(2), traits::organisation));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(3), traits::defence));
	EXPECT_EQ(2.0f, m.leader_trait_definitions.get(leader_trait_tag(3), traits::reliability));
	EXPECT_EQ(4.0f, m.leader_trait_definitions.get(leader_trait_tag(3), traits::speed));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(4), traits::defence));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(4), traits::reliability));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(4), traits::speed));
}
