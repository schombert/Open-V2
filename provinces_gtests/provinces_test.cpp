#include "provinces\\provinces_io.h"
#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "modifiers\\modifiers_io.h"
#include "world_state\\world_state.h"
#include "scenario\\scenario_io.h"
#include "provinces\\province_functions.h"
#include <ppl.h>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation map_dir = directory_representation(u"map", f_root);

	file_representation defintions = file_representation(u"definition.csv", map_dir,
		"province;red;green;blue;x;x\r\n"
		"1;204;229;152;Sitka;x\r\n"
		"2;204;179;153;Yakutat;x\r\n"
		"3;153;230;153;Kenai;x\r\n"
		"4;192;0;0;Dutch Harbor;x\r\n"
		";108;16;240;x\r\n"
		";109;16;244;x\r\n"
		);
	file_representation adjacent = file_representation(u"adjacencies.csv", map_dir,
		"#From;To;Type;Through;Data;Comment\r\n"
		"1;2;sea;3011;0;Vancouver Crossing\r\n"
		"#1;3;sea;2831;0;Sakhalin Strait #removed\r\n"
		"1;3;impassable;0;0;scary mountain\r\n"
		"2;3;canal;369;2;Kiel Canal"
		);
	file_representation a = file_representation(u"default.map", map_dir,
		"max_provinces = 5\r\n"
		"sea_starts = { 1 4 }\r\n"
		"definitions = \"definition.csv\"\r\n"
		"terrain_sheet_heights = { 500 }");
	file_representation b = file_representation(u"climate.txt", map_dir,
		"mild_climate = {\r\n"
		"farm_rgo_size = 0\r\n"
		"}\r\n"
		"temperate_climate  = {\r\n"
		"max_attrition = 1\r\n"
		"}\r\n"
		"mild_climate = { 1 2 3 }\r\n"
		"temperate_climate = { 0 4 }\r\n");
	file_representation c = file_representation(u"continent.txt", map_dir,
		"europe = {\r\n"
		"\tprovinces = {3 4}\r\n"
		"assimilation_rate = 0.5\r\n"
		"}\r\n"
		"asia = {\r\n"
		"\tprovinces = { 1 2 }\r\n"
		"mine_rgo_size = 10\r\n"
		"}\r\n");
	file_representation d = file_representation(u"region.txt", map_dir,
		"region_a = { 1 3 }\r\n"
		"region_b = { 2 4 }");
	file_representation e = file_representation(u"terrain.txt", map_dir,
		"terrain = 64\r\n"
		"categories = {\r\n"
		"ocean = { \r\n"
		"	movement_cost = 1.0\r\n"
		"	is_water = yes\r\n"
		"color = { 0 0 255 }\r\n"
		"}\r\n"
		"arctic = { \r\n"
		"	movement_cost = 1.0\r\n"
		"	color = { 235 235 235 }\r\n"
		"	farm_rgo_size = -0.5\r\n"
		"	farm_rgo_eff = -0.2\r\n"
		"	mine_rgo_size = -0.0\r\n"
		"	mine_rgo_eff = -0.2\r\n"
		"	min_build_railroad = 1\r\n"
		"}\r\n"
		"}\r\n"
		"ocean1 = { type = ocean color = { 254 } has_texture = no priority = 	0 }"
		"ocean2 = { type = arctic color = { 1 2 3 } has_texture = no priority = 	0 }");


	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace provinces;
using namespace modifiers;

TEST(provinces_test, default_map_read) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	province_manager m;
	modifiers_manager mm;
	text_data::text_sequences tex;;

	provinces::parsing_state state(tex, m, mm);

	read_default_map_file(state, f.get_root());

	EXPECT_EQ(5ui64, m.province_container.size());
	EXPECT_EQ(province_tag(0), m.province_container[province_tag(0)].id);
	EXPECT_EQ(0ui16, m.province_container[province_tag(0)].flags);
	EXPECT_EQ(province::sea, m.province_container[province_tag(1)].flags);
	EXPECT_EQ(province::sea, m.province_container[province_tag(4)].flags);
}

TEST(provinces_test, climate_read) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	province_manager m;
	modifiers_manager mm;
	text_data::text_sequences tex;

	provinces::parsing_state state(tex, m, mm);

	read_default_map_file(state, f.get_root());
	read_climates(state, f.get_root());

	EXPECT_EQ(2ui64, mm.provincial_modifiers.size());
	EXPECT_EQ(provincial_modifier_tag(0), mm.provincial_modifiers[provincial_modifier_tag(0)].id);
	EXPECT_EQ(provincial_modifier_tag(1), mm.provincial_modifiers[provincial_modifier_tag(1)].id);
	EXPECT_EQ(provincial_modifier_tag(0), mm.named_provincial_modifiers_index[mm.provincial_modifiers[provincial_modifier_tag(0)].name]);
	EXPECT_EQ(provincial_modifier_tag(1), mm.named_provincial_modifiers_index[text_data::get_thread_safe_text_handle(tex, "temperate_climate")]);

	EXPECT_EQ(1.0f, mm.provincial_modifier_definitions[provincial_modifier_tag(1)][modifiers::provincial_offsets::max_attrition]);

	EXPECT_EQ(provincial_modifier_tag(1), m.province_container[province_tag(0)].climate);
	EXPECT_EQ(provincial_modifier_tag(1), m.province_container[province_tag(4)].climate);

	EXPECT_EQ(provincial_modifier_tag(0), m.province_container[province_tag(1)].climate);
	EXPECT_EQ(provincial_modifier_tag(0), m.province_container[province_tag(2)].climate);
	EXPECT_EQ(provincial_modifier_tag(0), m.province_container[province_tag(3)].climate);
}

TEST(provinces_test, continent_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	province_manager m;
	modifiers_manager mm;
	text_data::text_sequences tex;

	provinces::parsing_state state(tex, m, mm);

	read_default_map_file(state, f.get_root());
	read_continents(state, f.get_root());

	EXPECT_EQ(2ui64, mm.provincial_modifiers.size());
	EXPECT_EQ(provincial_modifier_tag(0), mm.provincial_modifiers[provincial_modifier_tag(0)].id);
	EXPECT_EQ(provincial_modifier_tag(1), mm.provincial_modifiers[provincial_modifier_tag(1)].id);
	EXPECT_EQ(provincial_modifier_tag(0), mm.named_provincial_modifiers_index[text_data::get_thread_safe_text_handle(tex, "europe")]);
	EXPECT_EQ(provincial_modifier_tag(1), mm.named_provincial_modifiers_index[mm.provincial_modifiers[provincial_modifier_tag(1)].name]);

	EXPECT_EQ(provincial_modifier_tag(1), m.province_container[province_tag(1)].continent);
	EXPECT_EQ(provincial_modifier_tag(1), m.province_container[province_tag(2)].continent);

	EXPECT_EQ(provincial_modifier_tag(0), m.province_container[province_tag(3)].continent);
	EXPECT_EQ(provincial_modifier_tag(0), m.province_container[province_tag(4)].continent);

	EXPECT_EQ(0.5f, mm.provincial_modifier_definitions[provincial_modifier_tag(0)][modifiers::provincial_offsets::assimilation_rate]);
	EXPECT_EQ(10.0f, mm.provincial_modifier_definitions[provincial_modifier_tag(1)][modifiers::provincial_offsets::mine_rgo_size]);
}

TEST(provinces_test, terrain_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	province_manager m;
	modifiers_manager mm;
	text_data::text_sequences tex;

	provinces::parsing_state state(tex, m, mm);

	read_default_map_file(state, f.get_root());

	graphics::name_maps gname_maps;

	read_terrain_modifiers(tex, m, mm, gname_maps, f.get_root());

	EXPECT_EQ(2ui64, mm.provincial_modifiers.size());
	EXPECT_EQ(provincial_modifier_tag(0), mm.provincial_modifiers[provincial_modifier_tag(0)].id);
	EXPECT_EQ(provincial_modifier_tag(1), mm.provincial_modifiers[provincial_modifier_tag(1)].id);
	EXPECT_EQ(provincial_modifier_tag(0), mm.named_provincial_modifiers_index[text_data::get_thread_safe_text_handle(tex, "ocean")]);
	EXPECT_EQ(provincial_modifier_tag(1), mm.named_provincial_modifiers_index[mm.provincial_modifiers[provincial_modifier_tag(1)].name]);

	auto t_map = read_terrain_colors(tex, m, mm, f.get_root());

	EXPECT_EQ(provincial_modifier_tag(0), t_map.data[254]);
	EXPECT_EQ(provincial_modifier_tag(1), t_map.data[1]);
	EXPECT_EQ(provincial_modifier_tag(1), t_map.data[2]);
	EXPECT_EQ(provincial_modifier_tag(1), t_map.data[3]);

	EXPECT_EQ(1.0f, mm.provincial_modifier_definitions[provincial_modifier_tag(0)][modifiers::provincial_offsets::movement_cost]);
	EXPECT_EQ(1.0f, mm.provincial_modifier_definitions[provincial_modifier_tag(1)][modifiers::provincial_offsets::min_build_railroad]);
}

TEST(provinces_test, region_read) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	province_manager m;
	modifiers_manager mm;
	text_data::text_sequences tex;

	provinces::parsing_state state(tex, m, mm);

	read_default_map_file(state, f.get_root());
	read_states(state, f.get_root());

	EXPECT_EQ(2ui64, m.state_names.size());
	EXPECT_EQ(2ui64, m.named_states_index.size());

	EXPECT_EQ(state_tag(), m.province_container[province_tag(0)].state_id);
	EXPECT_EQ(state_tag(0), m.province_container[province_tag(1)].state_id);
	EXPECT_EQ(state_tag(1), m.province_container[province_tag(2)].state_id);
	EXPECT_EQ(state_tag(0), m.province_container[province_tag(3)].state_id);
	EXPECT_EQ(state_tag(1), m.province_container[province_tag(4)].state_id);

	bool tagged_provs[5] = { false };
	for (auto s1_range = m.states_to_province_index.get_row(state_tag(0)); s1_range.first != s1_range.second; ++s1_range.first) {
		tagged_provs[to_index(*s1_range.first)] = true;
	}
	EXPECT_EQ(false, tagged_provs[0]);
	EXPECT_EQ(true, tagged_provs[1]);
	EXPECT_EQ(false, tagged_provs[2]);
	EXPECT_EQ(true, tagged_provs[3]);
	EXPECT_EQ(false, tagged_provs[4]);

	bool tagged_provs_b[5] = { false };
	for (auto s1_range = m.states_to_province_index.get_row(state_tag(1)); s1_range.first != s1_range.second; ++s1_range.first) {
		tagged_provs_b[to_index(*s1_range.first)] = true;
	}
	EXPECT_EQ(false, tagged_provs_b[0]);
	EXPECT_EQ(false, tagged_provs_b[1]);
	EXPECT_EQ(true, tagged_provs_b[2]);
	EXPECT_EQ(false, tagged_provs_b[3]);
	EXPECT_EQ(true, tagged_provs_b[4]);
	
	EXPECT_EQ(text_data::get_thread_safe_text_handle(tex, "region_a"), m.state_names[state_tag(0)]);
	EXPECT_EQ(text_data::get_thread_safe_text_handle(tex, "region_b"), m.state_names[state_tag(1)]);
}

TEST(provinces_test, colors) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	auto result = read_province_definition_file(f.get_root());
	EXPECT_EQ(4ui64, result.size());
	EXPECT_EQ(province_tag(1), result[rgb_to_prov_index(204ui8, 229ui8, 152ui8)]);
	EXPECT_EQ(province_tag(2), result[rgb_to_prov_index(204ui8, 179ui8, 153ui8)]);
	EXPECT_EQ(province_tag(3), result[rgb_to_prov_index(153ui8, 230ui8, 153ui8)]);
	EXPECT_EQ(province_tag(4), result[rgb_to_prov_index(192ui8, 0ui8, 0ui8)]);
}

TEST(provinces_test, adjacent) {
	preparse_test_files real_fs;
	file_system f;

	province_manager m;

	f.set_root(u"F:");

	m.province_container.resize(8);

	std::map<province_tag, boost::container::flat_set<province_tag>> adj_map;
	adj_map[province_tag(1)].insert(province_tag(3));
	adj_map[province_tag(3)].insert(province_tag(1));

	adj_map[province_tag(1)].insert(province_tag(7));
	adj_map[province_tag(7)].insert(province_tag(1));

	adj_map[province_tag(1)].insert(province_tag(4));
	adj_map[province_tag(4)].insert(province_tag(1));

	adj_map[province_tag(2)].insert(province_tag(4));
	adj_map[province_tag(4)].insert(province_tag(2));

	adj_map[province_tag(4)].insert(province_tag(6));
	adj_map[province_tag(6)].insert(province_tag(4));

	adj_map[province_tag(2)].insert(province_tag(6));
	adj_map[province_tag(6)].insert(province_tag(2));

	adj_map[province_tag(1)].insert(province_tag(5));
	adj_map[province_tag(5)].insert(province_tag(1));

	m.province_container[province_tag(2)].flags = province::sea;
	m.province_container[province_tag(3)].flags = province::sea;
	m.province_container[province_tag(4)].flags = province::sea;
	m.province_container[province_tag(5)].flags = province::sea;
	m.province_container[province_tag(6)].flags = province::sea;

	read_adjacnencies_file(adj_map, m.canals, f.get_root());

	EXPECT_EQ(2ui64, m.canals.size());
	EXPECT_EQ(province_tag(2), m.canals[1].first);
	EXPECT_EQ(province_tag(3), m.canals[1].second);

	EXPECT_EQ(4ui64, adj_map[province_tag(1)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(1)].count(province_tag(5)));
	EXPECT_EQ(1ui64, adj_map[province_tag(1)].count(province_tag(4)));
	EXPECT_EQ(1ui64, adj_map[province_tag(1)].count(province_tag(2)));
	EXPECT_EQ(1ui64, adj_map[province_tag(1)].count(province_tag(7)));

	EXPECT_EQ(3ui64, adj_map[province_tag(2)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(2)].count(province_tag(1)));
	EXPECT_EQ(1ui64, adj_map[province_tag(2)].count(province_tag(4)));
	EXPECT_EQ(1ui64, adj_map[province_tag(2)].count(province_tag(6)));

	EXPECT_EQ(0ui64, adj_map[province_tag(3)].size());

	EXPECT_EQ(3ui64, adj_map[province_tag(4)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(4)].count(province_tag(1)));
	EXPECT_EQ(1ui64, adj_map[province_tag(4)].count(province_tag(2)));
	EXPECT_EQ(1ui64, adj_map[province_tag(4)].count(province_tag(6)));

	EXPECT_EQ(1ui64, adj_map[province_tag(5)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(5)].count(province_tag(1)));

	EXPECT_EQ(2ui64, adj_map[province_tag(6)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(6)].count(province_tag(4)));
	EXPECT_EQ(1ui64, adj_map[province_tag(6)].count(province_tag(2)));

	EXPECT_EQ(1ui64, adj_map[province_tag(7)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(7)].count(province_tag(1)));
	
	make_lakes(adj_map, m);

	EXPECT_EQ(uint16_t(province::sea), m.province_container[province_tag(2)].flags);
	EXPECT_EQ(uint16_t(province::sea), m.province_container[province_tag(4)].flags);
	EXPECT_EQ(uint16_t(province::sea), m.province_container[province_tag(6)].flags);

	EXPECT_EQ(uint16_t(0), m.province_container[province_tag(1)].flags);
	EXPECT_EQ(uint16_t(0), m.province_container[province_tag(7)].flags);

	EXPECT_EQ(uint16_t(province::sea | province::lake), m.province_container[province_tag(3)].flags);
	EXPECT_EQ(uint16_t(province::sea | province::lake), m.province_container[province_tag(5)].flags);

	EXPECT_EQ(3ui64, adj_map[province_tag(1)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(1)].count(province_tag(4)));
	EXPECT_EQ(1ui64, adj_map[province_tag(1)].count(province_tag(2)));
	EXPECT_EQ(1ui64, adj_map[province_tag(1)].count(province_tag(7)));

	EXPECT_EQ(3ui64, adj_map[province_tag(2)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(2)].count(province_tag(1)));
	EXPECT_EQ(1ui64, adj_map[province_tag(2)].count(province_tag(4)));
	EXPECT_EQ(1ui64, adj_map[province_tag(2)].count(province_tag(6)));

	EXPECT_EQ(0ui64, adj_map[province_tag(3)].size());

	EXPECT_EQ(3ui64, adj_map[province_tag(4)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(4)].count(province_tag(1)));
	EXPECT_EQ(1ui64, adj_map[province_tag(4)].count(province_tag(2)));
	EXPECT_EQ(1ui64, adj_map[province_tag(4)].count(province_tag(6)));

	EXPECT_EQ(0ui64, adj_map[province_tag(5)].size());

	EXPECT_EQ(2ui64, adj_map[province_tag(6)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(6)].count(province_tag(4)));
	EXPECT_EQ(1ui64, adj_map[province_tag(6)].count(province_tag(2)));

	EXPECT_EQ(1ui64, adj_map[province_tag(7)].size());
	EXPECT_EQ(1ui64, adj_map[province_tag(7)].count(province_tag(1)));

	make_adjacency(adj_map, m);

	EXPECT_EQ(uint16_t(province::sea | province::coastal), m.province_container[province_tag(2)].flags);
	EXPECT_EQ(uint16_t(province::sea | province::coastal), m.province_container[province_tag(4)].flags);
	EXPECT_EQ(uint16_t(province::sea), m.province_container[province_tag(6)].flags);

	EXPECT_EQ(uint16_t(province::coastal), m.province_container[province_tag(1)].flags);
	EXPECT_EQ(uint16_t(0), m.province_container[province_tag(7)].flags);

	EXPECT_EQ(uint16_t(province::sea | province::lake), m.province_container[province_tag(3)].flags);
	EXPECT_EQ(uint16_t(province::sea | province::lake), m.province_container[province_tag(5)].flags);

	{
		const auto same_row = m.same_type_adjacency.get_row(province_tag(1));
		const auto diff_row = m.coastal_adjacency.get_row(province_tag(1ui16));

		EXPECT_EQ(1i64, same_row.second - same_row.first);
		EXPECT_EQ(2i64, diff_row.second - diff_row.first);

		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(7ui16)));
		EXPECT_NE(diff_row.second, std::find(diff_row.first, diff_row.second, province_tag(2ui16)));
		EXPECT_NE(diff_row.second, std::find(diff_row.first, diff_row.second, province_tag(4ui16)));
	}
	{
		const auto same_row = m.same_type_adjacency.get_row(province_tag(2ui16));
		const auto diff_row = m.coastal_adjacency.get_row(province_tag(2ui16));

		EXPECT_EQ(2i64, same_row.second - same_row.first);
		EXPECT_EQ(1i64, diff_row.second - diff_row.first);

		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(6ui16)));
		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(4ui16)));
		EXPECT_NE(diff_row.second, std::find(diff_row.first, diff_row.second, province_tag(1ui16)));
	}
	{
		const auto same_row = m.same_type_adjacency.get_row(province_tag(3ui16));
		const auto diff_row = m.coastal_adjacency.get_row(province_tag(3ui16));

		EXPECT_EQ(0i64, same_row.second - same_row.first);
		EXPECT_EQ(0i64, diff_row.second - diff_row.first);
	}
	{
		const auto same_row = m.same_type_adjacency.get_row(province_tag(4ui16));
		const auto diff_row = m.coastal_adjacency.get_row(province_tag(4ui16));

		EXPECT_EQ(2i64, same_row.second - same_row.first);
		EXPECT_EQ(1i64, diff_row.second - diff_row.first);

		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(6ui16)));
		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(2ui16)));
		EXPECT_NE(diff_row.second, std::find(diff_row.first, diff_row.second, province_tag(1ui16)));
	}
	{
		const auto same_row = m.same_type_adjacency.get_row(province_tag(5ui16));
		const auto diff_row = m.coastal_adjacency.get_row(province_tag(5ui16));

		EXPECT_EQ(0i64, same_row.second - same_row.first);
		EXPECT_EQ(0i64, diff_row.second - diff_row.first);
	}
	{
		const auto same_row = m.same_type_adjacency.get_row(province_tag(6ui16));
		const auto diff_row = m.coastal_adjacency.get_row(province_tag(6ui16));

		EXPECT_EQ(2i64, same_row.second - same_row.first);
		EXPECT_EQ(0i64, diff_row.second - diff_row.first);

		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(4ui16)));
		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(2ui16)));
	}
	{
		const auto same_row = m.same_type_adjacency.get_row(province_tag(7ui16));
		const auto diff_row = m.coastal_adjacency.get_row(province_tag(7ui16));

		EXPECT_EQ(1i64, same_row.second - same_row.first);
		EXPECT_EQ(0i64, diff_row.second - diff_row.first);

		EXPECT_NE(same_row.second, std::find(same_row.first, same_row.second, province_tag(1ui16)));
	}
}

TEST(provinces_test, core_functions) {
	world_state ws;

	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();

	ready_world_state(ws);

	auto ger_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding("GER"));
	auto usa_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding("USA"));

	province_tag prov_a(100ui16);
	province_tag prov_b(101ui16);

	EXPECT_EQ(false, province_has_core(ws.w, prov_a, ger_tag));

	add_core(ws.w, prov_a, ger_tag);
	EXPECT_EQ(true, province_has_core(ws.w, prov_a, ger_tag));
	EXPECT_EQ(false, province_has_core(ws.w, prov_a, usa_tag));

	add_core(ws.w, prov_a, usa_tag);
	EXPECT_EQ(true, province_has_core(ws.w, prov_a, ger_tag));
	EXPECT_EQ(true, province_has_core(ws.w, prov_a, usa_tag));

	remove_core(ws.w, prov_a, usa_tag);
	EXPECT_EQ(true, province_has_core(ws.w, prov_a, ger_tag));
	EXPECT_EQ(false, province_has_core(ws.w, prov_a, usa_tag));

	add_core(ws.w, prov_b, ger_tag);

	EXPECT_EQ(2ui32, get_size(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[ger_tag].core_provinces));
	EXPECT_EQ(0ui32, get_size(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[usa_tag].core_provinces));
}

TEST(provinces_test, single_province_read_state) {
	world_state ws;

	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();

	ready_world_state(ws);

	const char test_data[] =
		"owner = USA\r\n"
		"controller = USA\r\n"
		"add_core = USA\r\n"
		"trade_goods = tobacco\r\n"
		"life_rating = 35\r\n"
		"railroad = 1\r\n"
		"colony = 2\r\n"
		"1861.1.1 = {\r\n"
		"	owner = CSA\r\n"
		"	controller = CSA\r\n"
		"	add_core = CSA\r\n"
		"	remove_core = USA\r\n"
		"}\r\n"
		"1861.1.1 = {\r\n"
		"	railroad = 3\r\n"
		"	terrain = ocean\r\n"
		"	party_loyalty = {\r\n"
		"		ideology = liberal\r\n"
		"		loyalty_value = 10.5 # loyalty test\r\n"
		"	}\r\n"
		"}\r\n"
		"1861.1.1 = {\r\n"
		"	state_building = {\r\n"
		"		level = 1\r\n"
		"		building = steel_factory\r\n"
		"		upgrade = yes\r\n"
		"	}\r\n"
		"	state_building = {\r\n"
		"		level = 1\r\n"
		"		building = steamer_shipyard\r\n"
		"		upgrade = yes\r\n"
		"	}\r\n"
		"	colony = 0\r\n"
		"}";

	std::vector<token_group> presults;
	parse_pdx_file(presults, RANGE(test_data));


	auto usa_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding("USA"));
	auto csa_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding("CSA"));

	{
		auto& ps = ws.w.province_s.province_state_container[province_tag(10ui16)];

		read_province_history(
			ws, ps, date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1)), presults.data(), presults.data() + presults.size());

		EXPECT_EQ(ps.controller, ws.w.culture_s.national_tags_state[usa_tag].holder);
		EXPECT_EQ(ps.owner, ws.w.culture_s.national_tags_state[usa_tag].holder);
		EXPECT_EQ(ps.fort_level, 0ui8);
		EXPECT_EQ(ps.railroad_level, 1ui8);
		EXPECT_EQ(ps.naval_base_level, 0ui8);
		EXPECT_EQ(35i16, ps.base_life_rating);
		EXPECT_EQ(tag_from_text(ws.s.economy_m.named_goods_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "tobacco")), ps.rgo_production);

		EXPECT_NE(0, ps.state_instance->flags & nations::state_instance::is_colonial);
		EXPECT_EQ(true, province_has_core(ws.w, province_tag(10ui16), usa_tag));
		EXPECT_EQ(false, province_has_core(ws.w, province_tag(10ui16), csa_tag));

	}

	{
		auto& ps = ws.w.province_s.province_state_container[province_tag(11ui16)];

		read_province_history(
			ws, ps, date_to_tag(boost::gregorian::date(1866, boost::gregorian::Jan, 1)), presults.data(), presults.data() + presults.size());

		EXPECT_EQ(ps.controller, ws.w.culture_s.national_tags_state[csa_tag].holder);
		EXPECT_EQ(ps.owner, ws.w.culture_s.national_tags_state[csa_tag].holder);
		EXPECT_EQ(ps.fort_level, 0ui8);
		EXPECT_EQ(ps.railroad_level, 3ui8);
		EXPECT_EQ(ps.naval_base_level, 0ui8);
		EXPECT_EQ(35i16, ps.base_life_rating);
		EXPECT_EQ(tag_from_text(ws.s.economy_m.named_goods_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "tobacco")), ps.rgo_production);
		EXPECT_EQ(tag_from_text(ws.s.modifiers_m.named_provincial_modifiers_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "ocean")), ps.terrain);
		EXPECT_EQ(ps.state_instance->factories[0].level, 1ui16);
		EXPECT_EQ(ps.state_instance->factories[0].type->output_good,
			tag_from_text(ws.s.economy_m.named_goods_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "steel")));
		EXPECT_EQ(ps.state_instance->factories[1].level, 1ui16);
		EXPECT_EQ(ps.state_instance->factories[1].type->output_good,
			tag_from_text(ws.s.economy_m.named_goods_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "steamer_convoy")));
		EXPECT_EQ(ps.state_instance->factories[2].level, 0ui16);
		EXPECT_EQ(ps.state_instance->factories[2].type, nullptr);

		EXPECT_EQ(ws.w.province_s.party_loyalty.get(province_tag(11ui16), tag_from_text(ws.s.ideologies_m.named_ideology_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "liberal"))), 10.5f);

		EXPECT_EQ(0, ps.state_instance->flags & nations::state_instance::is_colonial);
		EXPECT_EQ(false, province_has_core(ws.w, province_tag(11ui16), usa_tag));
		EXPECT_EQ(true, province_has_core(ws.w, province_tag(11ui16), csa_tag));
	}
}
