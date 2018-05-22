#include "gtest\\gtest.h"
#include "events\\events.h"
#include "text_data\\text_data.h"
#include "scenario\\scenario.h"
#include "fake_fs\\fake_fs.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace events;

class test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation gfx = directory_representation(u"gfx", test1);
	directory_representation pictures = directory_representation(u"pictures", gfx);
	directory_representation pic_events = directory_representation(u"events", pictures);
	directory_representation events = directory_representation(u"events", test1);
	file_representation pic = file_representation(u"pic.tga", pic_events, "");
	file_representation file_a = file_representation(u"file_a.txt", events, "");
	

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
		const char text[] = "id = 104\r\njunk = 8\r\njunk = { yes }\r\nis_triggered_only = false";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(text));

		const auto result = pre_parse_event(parse_results.data(), parse_results.data() + parse_results.size());
		EXPECT_EQ(104, result.first);
		EXPECT_EQ(false, result.second);
	}
}

TEST(event_rests, parse_single_event) {
	text_data::text_sequences ts;
	scenario::scenario_manager sm(ts);
	graphics::texture_manager tex;
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

		const auto tag_a = parse_single_event(
			sm,
			tex,
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
		EXPECT_EQ(std::string("F:\\test1\\gfx\\pictures\\events\\pic.tga"), tex.retrieve_by_key(sm.event_m.event_container[event_tag(0)].picture).filename);
	}
}
