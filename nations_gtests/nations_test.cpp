#include "gtest\\gtest.h"
#include "nations\\nations.h"
#include "nations\\nations_functions.h"
#include "scenario\\scenario.h"
#include "scenario\\scenario_io.h"
#include "world_state\\world_state.h"
#include <ppl.h>
#include "fake_fs\\fake_fs.h"
#include "population\\population_io.h"
#include "provinces\\province_functions.h"
#include "nations\\nations_io.h"

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

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation history = directory_representation(u"history", test1);
	directory_representation pops = directory_representation(u"pops", history);
	directory_representation pop_date = directory_representation(u"1800.1.1", pops);
	file_representation hf2 = file_representation(u"fileb.txt", pop_date,
		"853 = {\r\n"
		"	clergymen = {\r\n"
		"		culture = catalan\r\n"
		"		religion = orthodox\r\n"
		"		size = 50\r\n"
		"	}\r\n"
		"	farmers = {\r\n"
		"		culture = albanian\r\n"
		"		religion = orthodox\r\n"
		"		size = 6750\r\n"
		"	}\r\n"
		"}\r\n");
	directory_representation countries = directory_representation(u"countries", history);
	directory_representation units = directory_representation(u"units", history);
	directory_representation dir_1836 = directory_representation(u"1836", units);
	file_representation oob_a = file_representation(u"USA_oob.txt", units, "JAP = { value = 100 }");
	file_representation oob_b = file_representation(u"USA_oob.txt", dir_1836, "MEX = { value = 200 }");
	file_representation nfile = file_representation(u"GER - Germany.txt", countries,
		"capital = 487\r\n"
		"primary_culture = spanish\r\n"
		"culture = catalan\r\n"
		"culture = basque\r\n"
		"religion = catholic\r\n"
		"government = hms_government\r\n"
		"plurality = 25.0\r\n"
		"nationalvalue = nv_equality\r\n"
		"literacy = 0.75\r\n"
		"non_state_culture_literacy = 0.15\r\n"
		"civilized = yes\r\n"
		"prestige = 40\r\n"
		"slavery = yes_slavery\r\n"
		"press_rights = censored_press\r\n"
		"ruling_party = SPA_conservative\r\n"
		"last_election = 1834.1.1\r\n"
		"upper_house = {\r\n"
		"	fascist = 0\r\n"
		"	liberal = 35\r\n"
		"	conservative = 60\r\n"
		"	reactionary = 5\r\n"
		"	anarcho_liberal = 0\r\n"
		"	socialist = 0\r\n"
		"	communist = 0\r\n"
		"}\r\n"
		"foreign_investment = {\r\n"
		"	MEX = 10000\r\n"
		"	JAP = 2000\r\n"
		"}\r\n"
		"govt_flag = {\r\n"
		"	government = hms_government\r\n"
		"	flag = democracy\r\n"
		"}\r\n"
		"post_napoleonic_thought = 1\r\n"
		"authoritarianism = yes\r\n"
		"consciousness = 2\r\n"
		"nonstate_consciousness = 1\r\n"
		"schools = commerce_tech_school\r\n"
		"oob = \"USA_oob.txt\"\r\n"
		"set_country_flag = serfdom_not_abolished\r\n"
		"1838.1.1 = {\r\n"
		"	decision = trail_of_tears\r\n"
		"	set_global_flag = american_civil_war_has_happened\r\n"
		"	press_rights = free_press\r\n"
		"	flintlock_rifles = 1\r\n"
		"	upper_house = {\r\n"
		"		fascist = 0\r\n"
		"		liberal = 0\r\n"
		"		conservative = 95\r\n"
		"		reactionary = 5\r\n"
		"		anarcho_liberal = 0\r\n"
		"		socialist = 0\r\n"
		"		communist = 0\r\n"
		"	}\r\n"
		"	oob = \"/1836/USA_oob.txt\"\r\n"
		"	clr_country_flag = serfdom_not_abolished\r\n"
		"}\r\n");

	preparse_test_files() {
		set_default_root(f_root);
	}
};

TEST(nations_tests, read_nations_files_simple) {
	world_state ws;
	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();
	ready_world_state(ws);

	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:\\test1"));

	population::read_all_pops(f.get_root(), ws, date_to_tag(boost::gregorian::date(1801, boost::gregorian::Jan, 1)));
	
	auto ger_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("GER")));
	auto ger_nation = make_nation_for_tag(ws, ger_tag);

	silent_set_province_owner(ws, ger_nation, provinces::province_tag(853ui16));

	std::vector<std::pair<country_tag, events::decision_tag>> decisions;
	read_nations_files(ws, date_to_tag(boost::gregorian::date(1801, boost::gregorian::Jan, 1)), f.get_root(), decisions);

	auto jap_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("JAP")));
	auto jap_nation = make_nation_for_tag(ws, jap_tag);

	auto mex_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(RANGE("MEX")));
	auto mex_nation = make_nation_for_tag(ws, mex_tag);

	EXPECT_EQ(100, get_relationship(ws, *ger_nation, jap_nation->id));
	EXPECT_EQ(0, get_relationship(ws, *ger_nation, mex_nation->id));
	EXPECT_EQ(provinces::province_tag(487ui16), ger_nation->current_capital);

	EXPECT_EQ(tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("spanish"))), ger_nation->primary_culture);
	EXPECT_EQ(2ui32, get_size(ws.w.culture_s.culture_arrays, ger_nation->accepted_cultures));
	EXPECT_EQ(true, contains_item(ws.w.culture_s.culture_arrays, ger_nation->accepted_cultures, tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("catalan")))));
	EXPECT_EQ(true, contains_item(ws.w.culture_s.culture_arrays, ger_nation->accepted_cultures, tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("basque")))));
	EXPECT_EQ(tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("catholic"))), ger_nation->national_religion);
	EXPECT_EQ(tag_from_text(ws.s.governments_m.named_government_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("hms_government"))), ger_nation->current_government);
	EXPECT_EQ(25.0f, ger_nation->plurality);
	EXPECT_EQ(tag_from_text(ws.s.modifiers_m.named_national_modifiers_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("nv_equality"))), ger_nation->national_value);
	EXPECT_EQ(tag_from_text(ws.s.modifiers_m.named_national_modifiers_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("commerce_tech_school"))), ger_nation->tech_school);
	EXPECT_EQ(true, ger_nation->is_civilized);
	EXPECT_EQ(40.0f, ger_nation->prestige);
	EXPECT_EQ(tag_from_text(ws.s.governments_m.named_party_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("SPA_conservative"))), ger_nation->ruling_party);
	EXPECT_EQ(date_to_tag(boost::gregorian::date(1834, boost::gregorian::Jan, 1)), ger_nation->last_election);
	EXPECT_EQ(ws.w.nation_s.upper_house.get(ger_nation->id, tag_from_text(ws.s.ideologies_m.named_ideology_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("liberal")))), 35ui8);
	EXPECT_EQ(ws.w.nation_s.upper_house.get(ger_nation->id, tag_from_text(ws.s.ideologies_m.named_ideology_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("conservative")))), 60ui8);
	EXPECT_EQ(10000.0f, get_foreign_investment(ws, *ger_nation, mex_nation->id));
	EXPECT_EQ(2000.0f, get_foreign_investment(ws, *ger_nation, jap_nation->id));
	EXPECT_EQ(tag_from_text(ws.s.issues_m.named_option_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("yes_slavery"))), ws.w.nation_s.active_issue_options.get(ger_nation->id, tag_from_text(ws.s.issues_m.named_issue_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("slavery")))));
	EXPECT_EQ(tag_from_text(ws.s.issues_m.named_option_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("censored_press"))), ws.w.nation_s.active_issue_options.get(ger_nation->id, tag_from_text(ws.s.issues_m.named_issue_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("press_rights")))));
	EXPECT_EQ(ws.s.culture_m.national_tags[ger_nation->tag].monarchy_flag, ws.w.culture_s.country_flags_by_government.get(ger_nation->tag, tag_from_text(ws.s.governments_m.named_government_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("absolute_monarchy")))));
	EXPECT_EQ(ws.s.culture_m.national_tags[ger_nation->tag].republic_flag, ws.w.culture_s.country_flags_by_government.get(ger_nation->tag, tag_from_text(ws.s.governments_m.named_government_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("hms_government")))));

	EXPECT_EQ(true, bit_vector_test(ws.w.nation_s.active_technologies.get_row(ger_nation->id), to_index(tag_from_text(ws.s.technology_m.named_technology_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("post_napoleonic_thought"))))));
	EXPECT_EQ(true, bit_vector_test(ws.w.nation_s.active_technologies.get_row(ger_nation->id), to_index(tag_from_text(ws.s.technology_m.named_technology_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("authoritarianism"))))));
	EXPECT_EQ(false, bit_vector_test(ws.w.nation_s.active_technologies.get_row(ger_nation->id), to_index(tag_from_text(ws.s.technology_m.named_technology_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("flintlock_rifles"))))));

	EXPECT_EQ(0.0f, ws.w.variable_s.global_variables[tag_from_text(ws.s.variables_m.named_global_variables, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("american_civil_war_has_happened")))]);

	EXPECT_EQ(1ui32, get_size(ws.w.variable_s.national_flags_arrays, ger_nation->national_flags));
	EXPECT_EQ(true, contains_item(ws.w.variable_s.national_flags_arrays, ger_nation->national_flags, tag_from_text(ws.s.variables_m.named_national_flags, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("serfdom_not_abolished")))));

	auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(853ui16)].pops);
	EXPECT_EQ(2i64, pop_range.second - pop_range.first);

	for(auto p = pop_range.first; p != pop_range.second; ++p) {
		if(ws.w.population_s.pops.get(*p).culture == tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("catalan")))) {
			EXPECT_EQ(ws.w.population_s.pops.get(*p).literacy, static_cast<uint16_t>(0.75f * float(std::numeric_limits<uint16_t>::max())));
			EXPECT_EQ(ws.w.population_s.pops.get(*p).consciousness, static_cast<uint16_t>(2.0f * float(std::numeric_limits<uint16_t>::max()) / 10.0f));
		} else {
			EXPECT_EQ(ws.w.population_s.pops.get(*p).literacy, static_cast<uint16_t>(0.15f * float(std::numeric_limits<uint16_t>::max())));
			EXPECT_EQ(ws.w.population_s.pops.get(*p).consciousness, static_cast<uint16_t>(1.0f * float(std::numeric_limits<uint16_t>::max()) / 10.0f));
		}
	}

	EXPECT_EQ(0ui64, decisions.size());
}

