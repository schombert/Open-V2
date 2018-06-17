#include "gtest\\gtest.h"
#include "events\\events.h"
#include "text_data\\text_data.h"
#include "scenario\\scenario.h"
#include "fake_fs\\fake_fs.h"
#include "triggers\\codes.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace events;

class test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation gfx = directory_representation(u"gfx", test1);
	directory_representation pictures = directory_representation(u"pictures", gfx);
	directory_representation pic_events = directory_representation(u"events", pictures);
	directory_representation pic_decisions = directory_representation(u"decisions", pictures);
	directory_representation events = directory_representation(u"events", test1);
	file_representation pic = file_representation(u"pic.tga", pic_events, "");
	file_representation picb = file_representation(u"dpic.tga", pic_decisions, "");
	file_representation file_a = file_representation(u"file_a.txt", events,
		"province_event = { id = 5 }\r\n"
		"country_event = { id = 1 trigger = { is_mobilised = yes } }\r\n");
	file_representation file_b = file_representation(u"file_b.txt", events,
		"province_event = { id = 2 trigger = { has_empty_adjacent_province = yes } }\r\n");
	
	directory_representation test2 = directory_representation(u"test2", f_root);
	directory_representation gfx2 = directory_representation(u"gfx", test2);
	directory_representation common2 = directory_representation(u"common", test2);
	directory_representation pictures2 = directory_representation(u"pictures", gfx2);
	directory_representation pic_events2 = directory_representation(u"events", pictures2);
	directory_representation events2 = directory_representation(u"events", test2);
	directory_representation decisions2 = directory_representation(u"decisions", test2);
	file_representation pic2 = file_representation(u"pic.tga", pic_events2, "");
	file_representation file_c = file_representation(u"file_a.txt", events2,
		"province_event = { id = 5 is_triggered_only = yes issue_group = test_group }\r\n"
		"country_event = { id = 1 trigger = { is_mobilised = yes } is_triggered_only = yes }\r\n");
	file_representation file_d = file_representation(u"file_b.txt", events2,
		"province_event = { id = 2 trigger = { has_empty_adjacent_province = yes } is_triggered_only = yes }\r\n");
	file_representation on_actions = file_representation(u"on_actions.txt", common2, "on_yearly_pulse = {} on_new_great_nation = {10 = 1} on_election_tick = { 100 = 5 }");
	file_representation file_e = file_representation(u"d1.txt", decisions2, "political_decisions = { first = {} second = {} }");
	file_representation file_f = file_representation(u"d2.txt", decisions2, "political_decisions = { third = {} }");

	test_files() {
		set_default_root(f_root);
	}
};
TEST(event_tests, preparse_events) {
	{
		const char text[] = "id = 1\r\nis_triggered_only = yes";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		const auto result = pre_parse_event(parse_results.data(), parse_results.data() + parse_results.size());
		EXPECT_EQ(1, result.first);
		EXPECT_EQ(true, result.second);
	}
	{
		const char text[] = "id = 104\r\njunk = 8\r\njunk = { yes }\r\nis_triggered_only = no";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		const auto result = pre_parse_event(parse_results.data(), parse_results.data() + parse_results.size());
		EXPECT_EQ(104, result.first);
		EXPECT_EQ(false, result.second);
	}
}

TEST(event_tests, read_single_event) {
	scenario::scenario_manager sm;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	{
		const char text[] =
			"id = 1\r\n"
			"is_triggered_only = yes\r\n"
			"title = text_a\r\n"
			"desc = body_text\r\n"
			"news_desc_short = ignore_me\r\n"
			"picture = pic\r\n"
			"fire_only_once = yes\r\n"
			"election = false\r\n"
			"major = yes\r\n"
			"issue_group = issue\r\n"
			"trigger = {NOT = { has_global_flag = the_dragon_wakes }}\r\n"
			"option = { name = \"EVTOPTA90900\"\r\n"
				"set_global_flag = the_dragon_wakes\r\n"
				"ai_chance = { \r\n"
					"factor = 5\r\n"
					"modifier = { \r\n"
						"factor = 10\r\n"
						"NOT = { \r\n"
							"relation = { who = FROM value = 100 }\r\n"
						"}\r\n"
					"}\r\n"
				"}\r\n"
				"any_country = {\r\n"
					"limit = {\r\n"
						"NOT = {\r\n"
							"tag = THIS\r\n"
						"}\r\n"
						"num_of_cities = 1\r\n"
					"}\r\n"
					"capital = 1\r\n"
				"}\r\n"
			"}\r\n"
			"option = { name = opt_b }"
			"immediate = { civilized = yes }"
			;

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		const auto tag_a = read_single_event(
			sm,
			ecm,
			f.get_root(),
			triggers::trigger_scope_state{ triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, false },
			parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(event_tag(0), tag_a);
		EXPECT_EQ(event_tag(0), sm.event_m.event_container[event_tag(0)].id);
		EXPECT_NE(triggers::trigger_tag(), sm.event_m.event_container[event_tag(0)].trigger);
		EXPECT_NE(triggers::effect_tag(), sm.event_m.event_container[event_tag(0)].immediate_effect);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].body);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].title);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].options[0].name);
		EXPECT_NE(modifiers::factor_tag(), sm.event_m.event_container[event_tag(0)].options[0].ai_chance);
		EXPECT_NE(triggers::effect_tag(), sm.event_m.event_container[event_tag(0)].options[0].effect);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].options[1].name);
		EXPECT_EQ(modifiers::factor_tag(), sm.event_m.event_container[event_tag(0)].options[1].ai_chance);
		EXPECT_EQ(triggers::effect_tag(), sm.event_m.event_container[event_tag(0)].options[1].effect);
		EXPECT_EQ(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].options[2].name);
		EXPECT_EQ(uint8_t(event::fire_only_once | event::is_major), sm.event_m.event_container[event_tag(0)].flags);
		EXPECT_NE(graphics::texture_tag(), sm.event_m.event_container[event_tag(0)].picture);
		EXPECT_EQ(std::string("F:\\test1\\gfx\\pictures\\events\\pic.tga"), sm.gui_m.textures.retrieve_by_key(sm.event_m.event_container[event_tag(0)].picture).filename);
	}
}

TEST(event_tests, parse_or_defer) {
	scenario::scenario_manager sm;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	{
		const char text[] =
			"id = 1\r\n"
			"is_triggered_only = no\r\n"
			"title = text_a\r\n"
			"desc = body_text\r\n"
			"news_desc_short = ignore_me\r\n"
			"picture = pic\r\n"
			"fire_only_once = no\r\n"
			"election = false\r\n"
			"major = yes\r\n"
			"issue_group = issue\r\n"
			"trigger = {NOT = { has_global_flag = the_dragon_wakes }}\r\n"
			"option = { name = \"EVTOPTA90900\"\r\n"
			"set_global_flag = the_dragon_wakes\r\n"
			"ai_chance = { \r\n"
			"factor = 5\r\n"
			"modifier = { \r\n"
			"factor = 10\r\n"
			"NOT = { \r\n"
			"relation = { who = FROM value = 100 }\r\n"
			"}\r\n"
			"}\r\n"
			"}\r\n"
			"any_country = {\r\n"
			"limit = {\r\n"
			"NOT = {\r\n"
			"tag = THIS\r\n"
			"}\r\n"
			"num_of_cities = 1\r\n"
			"}\r\n"
			"capital = 1\r\n"
			"}\r\n"
			"}\r\n"
			"option = { name = opt_b }"
			"immediate = { civilized = yes }"
			;

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		const auto tag_a = read_or_defer_event(
			sm,
			ecm,
			f.get_root(),
			triggers::trigger_scope_state{ triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, false },
			parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(event_tag(0), tag_a);
		EXPECT_EQ(event_tag(0), sm.event_m.event_container[event_tag(0)].id);
		EXPECT_NE(triggers::trigger_tag(), sm.event_m.event_container[event_tag(0)].trigger);
		EXPECT_NE(triggers::effect_tag(), sm.event_m.event_container[event_tag(0)].immediate_effect);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].body);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].title);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].options[0].name);
		EXPECT_NE(modifiers::factor_tag(), sm.event_m.event_container[event_tag(0)].options[0].ai_chance);
		EXPECT_NE(triggers::effect_tag(), sm.event_m.event_container[event_tag(0)].options[0].effect);
		EXPECT_NE(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].options[1].name);
		EXPECT_EQ(modifiers::factor_tag(), sm.event_m.event_container[event_tag(0)].options[1].ai_chance);
		EXPECT_EQ(triggers::effect_tag(), sm.event_m.event_container[event_tag(0)].options[1].effect);
		EXPECT_EQ(text_data::text_tag(), sm.event_m.event_container[event_tag(0)].options[2].name);
		EXPECT_EQ(uint8_t(event::is_major), sm.event_m.event_container[event_tag(0)].flags);
		EXPECT_NE(graphics::texture_tag(), sm.event_m.event_container[event_tag(0)].picture);
		EXPECT_EQ(std::string("F:\\test1\\gfx\\pictures\\events\\pic.tga"), sm.gui_m.textures.retrieve_by_key(sm.event_m.event_container[event_tag(0)].picture).filename);

		EXPECT_EQ(parse_results.data(), ecm.event_sources[1].start);
		EXPECT_EQ(parse_results.data() + parse_results.size(), ecm.event_sources[1].end);
		EXPECT_EQ(event_tag(0), sm.event_m.events_by_id[1]);
	}
	{
		const char text[] =
			"id = 11\r\n"
			"is_triggered_only = yes\r\n"
			"title = text_a\r\n"
			"desc = body_text\r\n"
			"news_desc_short = ignore_me\r\n"
			"picture = pic\r\n"
			"fire_only_once = no\r\n"
			"election = false\r\n"
			"major = yes\r\n"
			"issue_group = issue\r\n"
			"trigger = {NOT = { has_global_flag = the_dragon_wakes }}\r\n"
			"option = { name = \"EVTOPTA90900\"\r\n"
			"set_global_flag = the_dragon_wakes\r\n"
			"ai_chance = { \r\n"
			"factor = 5\r\n"
			"modifier = { \r\n"
			"factor = 10\r\n"
			"NOT = { \r\n"
			"relation = { who = FROM value = 100 }\r\n"
			"}\r\n"
			"}\r\n"
			"}\r\n"
			"any_country = {\r\n"
			"limit = {\r\n"
			"NOT = {\r\n"
			"tag = THIS\r\n"
			"}\r\n"
			"num_of_cities = 1\r\n"
			"}\r\n"
			"capital = 1\r\n"
			"}\r\n"
			"}\r\n"
			"option = { name = opt_b }"
			"immediate = { civilized = yes }"
			;

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		const auto tag_a = read_or_defer_event(
			sm,
			ecm,
			f.get_root(),
			triggers::trigger_scope_state{ triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, false },
			parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(event_tag(), tag_a);

		EXPECT_EQ(parse_results.data(), ecm.event_sources[11].start);
		EXPECT_EQ(parse_results.data() + parse_results.size(), ecm.event_sources[11].end);
		EXPECT_EQ(sm.event_m.events_by_id.end(), sm.event_m.events_by_id.find(11));
	}
}

TEST(event_tests, event_file) {
	scenario::scenario_manager sm;
	graphics::texture_manager tex;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	{
		const char text[] =
			"province_event = { id = 5 }\r\n"
			"country_event = { id = 1 trigger = { is_mobilised = yes } }\r\n"
			"province_event = { id = 2 trigger = { has_empty_adjacent_province = yes } }\r\n";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		read_event_file(
			sm,
			ecm,
			f.get_root(),
			parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(1ui64, sm.event_m.country_events.size());
		EXPECT_EQ(2ui64, sm.event_m.province_events.size());
		EXPECT_EQ(3ui64, sm.event_m.events_by_id.size());
	}
}

TEST(event_tests, event_files) {
	scenario::scenario_manager sm;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	{
		read_event_files(
			sm,
			ecm,
			f.get_root());

		EXPECT_EQ(1ui64, sm.event_m.country_events.size());
		EXPECT_EQ(2ui64, sm.event_m.province_events.size());
		EXPECT_EQ(3ui64, sm.event_m.events_by_id.size());
	}
}

TEST(event_tests, triggered_event_parsing) {
	scenario::scenario_manager sm;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	{
		read_event_files(
			sm,
			ecm,
			f.get_root());

		EXPECT_EQ(0ui64, sm.event_m.country_events.size());
		EXPECT_EQ(0ui64, sm.event_m.province_events.size());
		EXPECT_EQ(0ui64, sm.event_m.events_by_id.size());

		const auto ev_1 = ecm.register_triggered_event(sm.event_m, 1, triggers::trigger_scope_state{triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::empty, false});
		const auto ev_2 = ecm.register_triggered_event(sm.event_m, 2, triggers::trigger_scope_state{ triggers::trigger_slot_contents::province, triggers::trigger_slot_contents::province, triggers::trigger_slot_contents::empty, false });
		const auto ev_3 = ecm.register_triggered_event(sm.event_m, 1, triggers::trigger_scope_state{ triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::nation, triggers::trigger_slot_contents::empty, false });

		EXPECT_EQ(2ui64, ecm.pending_triggered_events.size());
		EXPECT_EQ(0ui64, ecm.created_triggered_events.size());
		EXPECT_EQ(1ui64, sm.trigger_m.trigger_data.size());

		EXPECT_EQ(ev_1, ev_3);

		commit_pending_triggered_events(
			sm,
			ecm,
			f.get_root());

		EXPECT_EQ(0ui64, ecm.pending_triggered_events.size());
		EXPECT_EQ(2ui64, ecm.created_triggered_events.size());
		EXPECT_NE(1ui64, sm.trigger_m.trigger_data.size());

		EXPECT_NE(triggers::trigger_tag(), sm.event_m.event_container[ev_1].trigger);
		EXPECT_NE(triggers::trigger_tag(), sm.event_m.event_container[ev_2].trigger);

		EXPECT_EQ(uint16_t(triggers::trigger_codes::is_mobilised | triggers::trigger_codes::association_eq | triggers::trigger_codes::no_payload), sm.trigger_m.trigger_data[to_index(sm.event_m.event_container[ev_1].trigger)]);
		EXPECT_EQ(uint16_t(triggers::trigger_codes::has_empty_adjacent_province | triggers::trigger_codes::association_eq | triggers::trigger_codes::no_payload), sm.trigger_m.trigger_data[to_index(sm.event_m.event_container[ev_2].trigger)]);
	}
}

TEST(event_tests, on_actions_parsing) {
	scenario::scenario_manager sm;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	{
		read_event_files(
			sm,
			ecm,
			f.get_root());

		EXPECT_EQ(0ui64, sm.event_m.country_events.size());
		EXPECT_EQ(0ui64, sm.event_m.province_events.size());
		EXPECT_EQ(0ui64, sm.event_m.events_by_id.size());

		read_on_actions_file(sm, ecm, f.get_root());

		commit_pending_triggered_events(
			sm,
			ecm,
			f.get_root());

		EXPECT_EQ(0ui64, ecm.pending_triggered_events.size());
		EXPECT_EQ(2ui64, ecm.created_triggered_events.size());

		EXPECT_EQ(0ui64, sm.event_m.on_yearly_pulse.size());
		EXPECT_EQ(1ui64, sm.event_m.on_new_great_nation.size());
		EXPECT_EQ(1ui64, sm.event_m.on_election_tick.size());

		EXPECT_EQ(10ui16, sm.event_m.on_new_great_nation[0].second);
		EXPECT_NE(event_tag(), sm.event_m.on_new_great_nation[0].first);

		EXPECT_EQ(100ui16, std::get<1>(sm.event_m.on_election_tick[0]));
		EXPECT_NE(event_tag(), std::get<0>(sm.event_m.on_election_tick[0]));
		EXPECT_NE(text_data::text_tag(), std::get<2>(sm.event_m.on_election_tick[0]));
	}
}

TEST(event_tests, single_decision) {
	scenario::scenario_manager sm;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	{
		const char text[] =
			"ai_will_do = { factor = 2 }\r\n"
			"effect = { capital = 2 }\r\n"
			"allow = { year = 1 }\r\n"
			"potential = { capital = 3 }\r\n"
			"picture = dpic\r\n"
			"news_title = blah";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		const auto d_tag = read_decision(
			sm,
			ecm,
			f.get_root(),
			parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(1ui64, sm.event_m.decision_container.size());
		EXPECT_EQ(d_tag, sm.event_m.decision_container[d_tag].id);
		EXPECT_NE(triggers::trigger_tag(), sm.event_m.decision_container[d_tag].allow);
		EXPECT_NE(triggers::trigger_tag(), sm.event_m.decision_container[d_tag].potential);
		EXPECT_NE(sm.event_m.decision_container[d_tag].allow, sm.event_m.decision_container[d_tag].potential);
		EXPECT_NE(triggers::effect_tag(), sm.event_m.decision_container[d_tag].effect);
		EXPECT_NE(graphics::texture_tag(), sm.event_m.decision_container[d_tag].picture);
		EXPECT_NE(modifiers::factor_tag(), sm.event_m.decision_container[d_tag].ai_will_do);
		EXPECT_EQ(text_data::text_tag(), sm.event_m.decision_container[d_tag].body);
		EXPECT_EQ(text_data::text_tag(), sm.event_m.decision_container[d_tag].title);
	}
}

TEST(event_tests, decision_file) {
	scenario::scenario_manager sm;
	graphics::texture_manager tex;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	{
		const char text[] =
			"political_decisions = { \r\n"
		 		"name_a = {\r\n"
					"ai_will_do = { factor = 2 }\r\n"
					"effect = { capital = 2 }\r\n"
					"allow = { year = 1 }\r\n"
					"potential = { capital = 3 }\r\n"
					"picture = dpic\r\n"
					"news_title = blah\r\n"
				"}\r\n"
				"name_b = {}"
			"}";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		read_decision_file(
			sm,
			ecm,
			f.get_root(),
			parse_results.data(), parse_results.data() + parse_results.size());

		EXPECT_EQ(2ui64, sm.event_m.decision_container.size());

		EXPECT_EQ(decision_tag(0), sm.event_m.decision_container[decision_tag(0)].id);
		EXPECT_EQ(decision_tag(1), sm.event_m.decision_container[decision_tag(1)].id);
		EXPECT_NE(triggers::trigger_tag(), sm.event_m.decision_container[decision_tag(0)].allow);
		EXPECT_NE(triggers::trigger_tag(), sm.event_m.decision_container[decision_tag(0)].potential);
		EXPECT_NE(sm.event_m.decision_container[decision_tag(0)].allow, sm.event_m.decision_container[decision_tag(0)].potential);
		EXPECT_NE(triggers::effect_tag(), sm.event_m.decision_container[decision_tag(0)].effect);
		EXPECT_NE(graphics::texture_tag(), sm.event_m.decision_container[decision_tag(0)].picture);
		EXPECT_NE(modifiers::factor_tag(), sm.event_m.decision_container[decision_tag(0)].ai_will_do);
		EXPECT_NE(text_data::text_tag(), sm.event_m.decision_container[decision_tag(0)].body);
		EXPECT_NE(text_data::text_tag(), sm.event_m.decision_container[decision_tag(0)].title);
		EXPECT_NE(sm.event_m.decision_container[decision_tag(0)].body, sm.event_m.decision_container[decision_tag(0)].title);

		EXPECT_EQ(sm.gui_m.text_data_sequences.key_to_sequence_map.find("name_a_desc")->second, sm.event_m.decision_container[decision_tag(0)].body);
		EXPECT_EQ(sm.gui_m.text_data_sequences.key_to_sequence_map.find("name_b_title")->second, sm.event_m.decision_container[decision_tag(1)].title);
	}
}

TEST(event_tests, decision_files) {
	scenario::scenario_manager sm;
	event_creation_manager ecm;

	test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	{
		read_decision_files(
			sm,
			ecm,
			f.get_root());

		EXPECT_EQ(3ui64, sm.event_m.decision_container.size());
	}
}
