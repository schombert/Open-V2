#include "gtest\\gtest.h"
#include "nations\\nations.h"
#include "nations\\nations_functions.h"
#include "scenario\\scenario.h"
#include "scenario\\scenario_io.h"
#include "world_state\\world_state.h"
#include <ppl.h>

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace nations;

TEST(nations_tests, nation_creation) {
	world_state ws;

	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();

	ready_world_state(ws);

	auto ger_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("GER")));
	auto ger_nation = make_nation_for_tag(ws, ger_tag);
	
	EXPECT_NE(nullptr, ger_nation);
	EXPECT_EQ(ger_nation->tag, ger_tag);
	EXPECT_NE(country_tag(), ger_nation->id);

	EXPECT_EQ(ws.s.culture_m.national_tags[ger_tag].default_name.adjective, ger_nation->adjective);
	EXPECT_EQ(ws.s.culture_m.national_tags[ger_tag].default_name.name, ger_nation->name);
	EXPECT_EQ(ws.s.culture_m.national_tags[ger_tag].base_flag, ger_nation->flag);
	EXPECT_EQ(ws.s.culture_m.national_tags[ger_tag].color.g, ger_nation->current_color.g);

	auto usa_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("USA")));
	auto usa_nation = make_nation_for_tag(ws, usa_tag);

	EXPECT_NE(nullptr, usa_nation);
	EXPECT_EQ(usa_nation->tag, usa_tag);
	EXPECT_NE(country_tag(), usa_nation->id);
	EXPECT_NE(ger_nation->id, usa_nation->id);
}

TEST(nations_tests, province_ownership) {
	world_state ws;
	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();
	ready_world_state(ws);

	auto ger_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("GER")));
	auto ger_nation = make_nation_for_tag(ws, ger_tag);

	auto prov_tag = provinces::province_tag(104ui16);
	auto province_region = ws.s.province_m.province_container[prov_tag].state_id;

	EXPECT_NE(provinces::state_tag(), province_region);

	silent_set_province_owner(ws, ger_nation, prov_tag);

	EXPECT_EQ(1ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(1ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(province_region, get(ws.w.nation_s.state_arrays, ger_nation->member_states, 0ui32).region_id);
	EXPECT_NE(nullptr, get(ws.w.nation_s.state_arrays, ger_nation->member_states, 0ui32).state);
	EXPECT_EQ(ger_nation, ws.w.province_s.province_state_container[prov_tag].owner);
	EXPECT_EQ(get(ws.w.nation_s.state_arrays, ger_nation->member_states, 0ui32).state, ws.w.province_s.province_state_container[prov_tag].state_instance);

	auto old_state_instance = ws.w.province_s.province_state_container[prov_tag].state_instance;

	silent_remove_province_owner(ws, ger_nation, prov_tag);

	EXPECT_EQ(0ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(0ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(nullptr, ws.w.province_s.province_state_container[prov_tag].owner);
	EXPECT_EQ(nullptr, ws.w.province_s.province_state_container[prov_tag].state_instance);
	EXPECT_EQ(state_tag(), ws.w.nation_s.states.get_id(*old_state_instance));
}

TEST(nations_tests, adding_states) {
	world_state ws;
	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();
	ready_world_state(ws);

	auto ger_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("GER")));
	auto ger_nation = make_nation_for_tag(ws, ger_tag);

	auto prov_tag_a = provinces::province_tag(104ui16);
	auto prov_tag_b = provinces::province_tag(103ui16);
	auto prov_tag_c = provinces::province_tag(102ui16);

	silent_set_province_owner(ws, ger_nation, prov_tag_a);
	silent_set_province_owner(ws, ger_nation, prov_tag_b);

	EXPECT_EQ(2ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(1ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_NE(nullptr, ws.w.province_s.province_state_container[prov_tag_b].state_instance);
	EXPECT_EQ(ws.w.province_s.province_state_container[prov_tag_a].state_instance, ws.w.province_s.province_state_container[prov_tag_b].state_instance);
	EXPECT_EQ(ger_nation, ws.w.province_s.province_state_container[prov_tag_a].owner);
	EXPECT_EQ(ger_nation, ws.w.province_s.province_state_container[prov_tag_b].owner);

	EXPECT_EQ(true, contains_item(ws.w.province_s.province_arrays, ger_nation->owned_provinces, prov_tag_a));
	EXPECT_EQ(false, contains_item(ws.w.province_s.province_arrays, ger_nation->owned_provinces, prov_tag_c));

	silent_set_province_owner(ws, ger_nation, prov_tag_c);

	EXPECT_EQ(3ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(2ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(ger_nation, ws.w.province_s.province_state_container[prov_tag_c].owner);

	EXPECT_NE(nullptr, ws.w.province_s.province_state_container[prov_tag_c].state_instance);
	EXPECT_NE(ws.w.province_s.province_state_container[prov_tag_a].state_instance, ws.w.province_s.province_state_container[prov_tag_c].state_instance);

	silent_remove_province_owner(ws, ger_nation, prov_tag_a);
	EXPECT_EQ(2ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(2ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(nullptr, ws.w.province_s.province_state_container[prov_tag_a].owner);

	EXPECT_EQ(false, contains_item(ws.w.province_s.province_arrays, ger_nation->owned_provinces, prov_tag_a));
	EXPECT_EQ(true, contains_item(ws.w.province_s.province_arrays, ger_nation->owned_provinces, prov_tag_c));

	EXPECT_NE(ws.w.province_s.province_state_container[prov_tag_a].state_instance, ws.w.province_s.province_state_container[prov_tag_b].state_instance);

	silent_remove_province_owner(ws, ger_nation, prov_tag_b);
	EXPECT_EQ(1ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(1ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(nullptr, ws.w.province_s.province_state_container[prov_tag_b].owner);

	EXPECT_EQ(nullptr, ws.w.province_s.province_state_container[prov_tag_b].state_instance);
	EXPECT_EQ(ws.w.province_s.province_state_container[prov_tag_a].state_instance, ws.w.province_s.province_state_container[prov_tag_b].state_instance);

	silent_remove_province_owner(ws, ger_nation, prov_tag_c);
	EXPECT_EQ(0ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(0ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(nullptr, ws.w.province_s.province_state_container[prov_tag_c].owner);

	EXPECT_EQ(ws.w.province_s.province_state_container[prov_tag_a].state_instance, ws.w.province_s.province_state_container[prov_tag_c].state_instance);
}

TEST(nations_tests, province_control) {
	world_state ws;
	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();
	ready_world_state(ws);

	auto ger_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("GER")));
	auto ger_nation = make_nation_for_tag(ws, ger_tag);

	auto prov_tag = provinces::province_tag(104ui16);
	auto province_region = ws.s.province_m.province_container[prov_tag].state_id;

	EXPECT_NE(provinces::state_tag(), province_region);

	silent_set_province_controller(ws.w, ger_nation, prov_tag);

	EXPECT_EQ(0ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(1ui32, get_size(ws.w.province_s.province_arrays, ger_nation->controlled_provinces));
	EXPECT_EQ(0ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(ger_nation, ws.w.province_s.province_state_container[prov_tag].controller);
	EXPECT_EQ(true, contains_item(ws.w.province_s.province_arrays, ger_nation->controlled_provinces, prov_tag));

	silent_remove_province_controller(ws.w, ger_nation, prov_tag);

	EXPECT_EQ(0ui32, get_size(ws.w.province_s.province_arrays, ger_nation->owned_provinces));
	EXPECT_EQ(0ui32, get_size(ws.w.province_s.province_arrays, ger_nation->controlled_provinces));
	EXPECT_EQ(0ui32, get_size(ws.w.nation_s.state_arrays, ger_nation->member_states));
	EXPECT_EQ(nullptr, ws.w.province_s.province_state_container[prov_tag].controller);
	EXPECT_EQ(false, contains_item(ws.w.province_s.province_arrays, ger_nation->controlled_provinces, prov_tag));
}
