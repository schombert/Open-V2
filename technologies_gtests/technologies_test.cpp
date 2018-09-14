#include "technologies\\technologies.h"
#include "gtest/gtest.h"
#include "fake_fs\\fake_fs.h"
#include "modifiers\\modifiers.h"
#include "scenario\\scenario.h"
#include "technologies\\technologies_io.h"
#include "military\\military_io.h"
#include "economy\\economy_io.h"
#include "population\\population_io.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

using namespace technologies;

const char single_tech[] = 
"tech_name = {\r\n"
" area = army_doctrine\r\n"
"other_date = {}\r\n"
"}";

const char two_techs[] =
"tech_name = {\r\n"
" area = army_doctrine\r\n"
"other_date = {}\r\n"
"}\r\n"
"tech2 = {}";

const char fake_tech_file[] =
"folders = {\r\n"
"army_tech = {\r\n"
"atech_a\r\n"
"atech_b\r\n"
"atech_c\r\n"
"atech_d\r\n"
"}\r\n"
"navy_tech = {\r\n"
"}\r\n"
"}\r\n"
"\r\n"
"schools = {\r\n"
"fake_school = {\r\n"
"army_tech_research_bonus = 0\r\n"
"}\r\n"
"}\r\n";

const char fake_tech_file_b[] =
"folders = {\r\n"
"}\r\n"
"\r\n"
"schools = {\r\n"
"fake_school = {\r\n"
"army_tech_research_bonus = 0\r\n"
"}\r\n"
"fake_school_b = { \r\n"
"commerce_tech_research_bonus = 1\r\n"
"}\r\n"
"}\r\n";


class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation common = directory_representation(u"common", f_root);
	directory_representation technologies = directory_representation(u"technologies", f_root);

	file_representation tech_file = file_representation(u"technology.txt", common, fake_tech_file);

	directory_representation t_alt = directory_representation(u"test_alt", f_root);
	directory_representation common_alt = directory_representation(u"common", t_alt);
	file_representation tech_file_alt = file_representation(u"technology.txt", common_alt, fake_tech_file_b);

	file_representation ta = file_representation(u"army_tech.txt", technologies, single_tech);
	file_representation tb = file_representation(u"navy_tech.txt", technologies, two_techs);

	directory_representation inventions = directory_representation(u"inventions", f_root);
	file_representation ia = file_representation(u"army_tech.txt", inventions, "invention_a = {}\r\ninvention_b = { stuff }");
	file_representation ib = file_representation(u"navy_tech.txt", inventions, "invention_c = { x = false }");

	directory_representation t2 = directory_representation(u"test2", f_root);
	directory_representation common2 = directory_representation(u"common", t2);
	directory_representation technologies2 = directory_representation(u"technologies", t2);
	directory_representation inventions2 = directory_representation(u"inventions", t2);
	file_representation tech_file2 = file_representation(u"technology.txt", common2, fake_tech_file);

	file_representation rebel_types = file_representation(u"rebel_types.txt", common2,
		"rebel_1 = {}\r\n"
		"rebel_2 = { stuff }");

	file_representation buildings3 = file_representation(u"buildings.txt", common2,
		"aeroplane_factory = {\r\n"
		"type = factory\r\n"
		"production_type = aeroplane_factory\r\n"
		"}\r\n");

	directory_representation units2 = directory_representation(u"units", t2);
	file_representation f1 = file_representation(u"dragoon.txt", units2, "dragoon = { stuff }");
	file_representation f2 = file_representation(u"airplane.txt", units2, "plane_a = { stuff }\r\nplane_b = {}");

	file_representation goods2 = file_representation(u"goods.txt", common2,
		"military_goods   = {\r\n"
		"ammunition = {\r\n"
		"cost = 2.0\r\n"
		"}\r\n"
		"cement = {\r\n"
		"cost = 3.0\r\n"
		"}\r\n"
		"canned_food = {\r\n"
		"cost = 5.0\r\n"
		"}\r\n"
		"}\r\n");

	file_representation ta2 = file_representation(u"army_tech.txt", technologies2,
		"post_napoleonic_thought = {\r\n"
		"	area = atech_a\r\n"
		"	year = 1836\r\n"
		"	cost = 3600\r\n"
		"	unciv_military = yes\r\n"
		"	activate_building = fort\r\n"
		"	army_base = {\r\n"
		"		default_organisation = 5\r\n"
		"		supply_consumption = 0.10\r\n"
		"	}\r\n"
		"	dragoon = {\r\n"
		"		attack = 2\r\n"
		"	}\r\n"
		"	dig_in_cap = 1\r\n"
		"	ai_chance = {\r\n"
		"		factor = 2\r\n"
		"		modifier = {\r\n"
		"			factor = 1.2\r\n"
		"			civilized = yes\r\n"
		"			any_neighbor_country = {\r\n"
		"				civilized = yes\r\n"
		"				NOT = { is_greater_power = yes }\r\n"
		"				military_score = THIS\r\n"
		"			}\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 1.02\r\n"
		"			tech_school = fake_school\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
	);
	file_representation tb2 = file_representation(u"navy_tech.txt", technologies2,
		"private_banks = {\r\n"
		"	area = atech_a\r\n"
		"	year = 1836\r\n"
		"	cost = 3000\r\n"
		"	activate_building = aeroplane_factory\r\n"
		"	tax_eff = 3\r\n"
		"	ai_chance = {\r\n"
		"		factor = 1.1\r\n"
		"		modifier = {\r\n"
		"			factor = 3.5\r\n"
		"			rank = 16\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 1.1\r\n"
		"			literacy = 0.3\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		"cheap_iron = {\r\n"
		"	year = 1836\r\n"
		"	cost = 10800\r\n"
		"	rgo_goods_output = {\r\n"
		"		canned_food = 0.1\r\n"
		"	}\r\n"
		"	rgo_size = {\r\n"
		"		canned_food = 0.2\r\n"
		"	}\r\n"
		"	ai_chance = {\r\n"
		"		factor = 1.1\r\n"
		"		modifier = {\r\n"
		"			factor = 1.6\r\n"
		"			big_producer = canned_food\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
	);

	file_representation ia2 = file_representation(u"army_tech.txt", inventions2,
		"gas_defense_capability = {\r\n"
		"	limit = {\r\n"
		"		post_napoleonic_thought = 1\r\n"
		"	}\r\n"
		"	chance = {\r\n"
		"		base = 0\r\n"
		"		modifier = {\r\n"
		"			factor = 5\r\n"
		"			war = yes\r\n"
		"			war_countries = { invention = secret_police }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	effect = {\r\n"
		"		gas_defence = yes\r\n"
		"	}\r\n"
		"}\r\n"
		"secret_police = {\r\n"
		"	news = no\r\n"
		"	limit = { cheap_iron = 1 }\r\n"
		"	chance = {\r\n"
		"		base = 2\r\n"
		"	}\r\n"
		"	effect = {\r\n"
		"		rebel_org_gain = {\r\n"
		"			faction = all\r\n"
		"			value = -0.25\r\n"
		"		}\r\n"
		"		rebel_org_gain = {\r\n"
		"			faction = rebel_1\r\n"
		"			value = 1\r\n"
		"		}\r\n"
		"		pop_growth = 0.25\r\n"
		"	}\r\n"
		"	farm_rgo_size = 4.0\r\n"
		"}\r\n"
		
	);
	file_representation ib2 = file_representation(u"navy_tech.txt", inventions2,
		"philosophy_of_language = {\r\n"
		"	news = no\r\n"
		"	effect = {\r\n"
		"		shared_prestige = 15\r\n"
		"		max_national_focus = 1\r\n"
		"	}\r\n"
		"}\r\n"
	);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

TEST(technologies_tests, pre_parse_single_tech) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	std::vector<token_group> results;

	technologies_manager manager;
	text_data::text_sequences text;
	modifiers::modifiers_manager mm;

	parse_pdx_file(results, single_tech, single_tech + sizeof(single_tech) - 1);

	parsing_state state(text, f.get_root(), manager, mm);

	pre_parse_single_tech_file(*state.impl, results.data(), results.data() + results.size());

	EXPECT_EQ(1ui64, manager.technologies_container.size());
	EXPECT_EQ(tech_tag(0), manager.technologies_container[tech_tag(0)].id);
	EXPECT_EQ(text_data::text_tag(0), manager.technologies_container[tech_tag(0)].name);

	const auto find_result = manager.named_technology_index.find(text_data::text_tag(0));

	EXPECT_NE(manager.named_technology_index.end(), find_result);
	EXPECT_EQ(tech_tag(0), find_result->second);
}

TEST(technologies_tests, pre_parse_two_techs) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	std::vector<token_group> results;

	technologies_manager manager;
	text_data::text_sequences text;
	modifiers::modifiers_manager mm;

	parse_pdx_file(results, two_techs, two_techs + sizeof(two_techs) - 1);

	parsing_state state(text, f.get_root(), manager, mm);
	pre_parse_single_tech_file(*state.impl, results.data(), results.data() + results.size());

	ASSERT_EQ(2ui64, manager.technologies_container.size());

	const auto find_result = manager.named_technology_index.find(text_data::text_tag(0));

	ASSERT_NE(manager.named_technology_index.end(), find_result);
	ASSERT_EQ(tech_tag(0), find_result->second);
}

TEST(technologies_tests, pre_parse_tech_file) {

	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	parsing_state state(text, f.get_root(), manager, mm);
	pre_parse_technologies(state, f.get_root());

	EXPECT_EQ(2ui64, manager.technology_categories.size());
	EXPECT_EQ(4ui64, manager.technology_subcategories.size());
	EXPECT_EQ(1ui64, mm.named_national_modifiers_index.size());

	EXPECT_EQ(tech_category_tag(0), manager.technology_categories[tech_category_tag(0)].id);
	EXPECT_EQ(tech_category_tag(1), manager.technology_categories[tech_category_tag(1)].id);

	EXPECT_EQ(tech_subcategory_tag(0), manager.technology_categories[tech_category_tag(0)].subcategories[0]);
	EXPECT_EQ(tech_subcategory_tag(1), manager.technology_categories[tech_category_tag(0)].subcategories[1]);
	EXPECT_EQ(tech_subcategory_tag(2), manager.technology_categories[tech_category_tag(0)].subcategories[2]);
	EXPECT_EQ(tech_subcategory_tag(3), manager.technology_categories[tech_category_tag(0)].subcategories[3]);
	EXPECT_EQ(tech_subcategory_tag(), manager.technology_categories[tech_category_tag(0)].subcategories[4]);

	EXPECT_EQ(tech_subcategory_tag(0), manager.technology_subcategories[tech_subcategory_tag(0)].id);
	EXPECT_EQ(tech_subcategory_tag(1), manager.technology_subcategories[tech_subcategory_tag(1)].id);
	EXPECT_EQ(tech_subcategory_tag(2), manager.technology_subcategories[tech_subcategory_tag(2)].id);

	EXPECT_EQ(tech_category_tag(0), manager.technology_subcategories[tech_subcategory_tag(0)].parent);
	EXPECT_EQ(tech_category_tag(0), manager.technology_subcategories[tech_subcategory_tag(1)].parent);
	EXPECT_EQ(tech_category_tag(0), manager.technology_subcategories[tech_subcategory_tag(2)].parent);

	EXPECT_EQ(2ui64, manager.named_category_index.size());
	EXPECT_EQ(4ui64, manager.named_subcategory_index.size());
	EXPECT_EQ(1ui64, mm.named_national_modifiers_index.size());

	EXPECT_EQ(modifiers::national_modifier_tag(0), mm.national_modifiers[modifiers::national_modifier_tag(0)].id);
	const auto fr = mm.named_national_modifiers_index[mm.national_modifiers[modifiers::national_modifier_tag(0)].name];
	EXPECT_EQ(modifiers::national_modifier_tag(0), fr);
}

TEST(technologies_tests, pre_parse_schools) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test_alt");

	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	parsing_state state(text, f.get_root(), manager, mm);
	pre_parse_technologies(state, f.get_root());

	EXPECT_EQ(2ui64, mm.national_modifiers.size());
	EXPECT_EQ(2ui64, mm.named_national_modifiers_index.size());
	EXPECT_EQ(2ui64, mm.named_national_modifiers_index.size());

	EXPECT_EQ(modifiers::national_modifier_tag(0), mm.national_modifiers[modifiers::national_modifier_tag(0)].id);
	EXPECT_EQ(modifiers::national_modifier_tag(1), mm.national_modifiers[modifiers::national_modifier_tag(1)].id);
	const auto fr = mm.named_national_modifiers_index[mm.national_modifiers[modifiers::national_modifier_tag(0)].name];
	EXPECT_EQ(fr, modifiers::national_modifier_tag(0));
	const auto frb = mm.named_national_modifiers_index[mm.national_modifiers[modifiers::national_modifier_tag(1)].name];
	EXPECT_EQ(frb, modifiers::national_modifier_tag(1));

	EXPECT_EQ(1.0f, mm.national_modifier_definitions[frb][modifiers::national_offsets::commerce_tech_research_bonus]);
}

TEST(technologies_tests, pre_parse_techs_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	const auto tech_dir = f.get_root().get_directory(u"\\technologies");

	parsing_state state(text, tech_dir, manager, mm);

	pre_parse_technologies(state, f.get_root());

	EXPECT_EQ(3ui64, manager.technologies_container.size());
	EXPECT_EQ(2ui64, manager.technology_categories.size());
	EXPECT_EQ(4ui64, manager.technology_subcategories.size());
}

TEST(technologies_tests, pre_parse_inventions_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:");

	technologies_manager manager;
	modifiers::modifiers_manager mm;
	text_data::text_sequences text;

	const auto tech_dir = f.get_root().get_directory(u"\\technologies");

	parsing_state state(text, tech_dir, manager, mm);

	pre_parse_inventions(state, f.get_root());

	EXPECT_EQ(3ui64, manager.inventions.size());
	EXPECT_EQ(3ui64, manager.named_technology_index.size());

	EXPECT_EQ(tech_tag(0), manager.technologies_container[tech_tag(0)].id);
	EXPECT_EQ(tech_tag(1), manager.technologies_container[tech_tag(1)].id);
	EXPECT_EQ(tech_tag(2), manager.technologies_container[tech_tag(2)].id);

	EXPECT_EQ(tech_tag(0), manager.named_technology_index[manager.technologies_container[tech_tag(0)].name]);
	EXPECT_EQ(tech_tag(1), manager.named_technology_index[manager.technologies_container[tech_tag(1)].name]);
	EXPECT_EQ(tech_tag(2), manager.named_technology_index[manager.technologies_container[tech_tag(2)].name]);
}

TEST(technologies_tests, read_technologies_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test2");

	scenario::scenario_manager s;

	economy::read_goods(s.economy_m, f.get_root(), s.gui_m.text_data_sequences);
	economy::read_buildings(s.economy_m, f.get_root(), s.gui_m.text_data_sequences);

	{
		military::parsing_state state(s.gui_m.text_data_sequences, s.military_m);
		military::pre_parse_unit_types(state, f.get_root());
	}

	{
		population::parsing_state env(s.gui_m.text_data_sequences, s.population_m);
		population::pre_parse_rebel_types(env, f.get_root());
	}

	parsing_state state(s.gui_m.text_data_sequences, f.get_root().get_directory(u"\\technologies"), s.technology_m, s.modifiers_m);

	pre_parse_technologies(state, f.get_root());
	pre_parse_inventions(state, f.get_root());

	prepare_technologies_read(s);
	read_technologies(state, s);
	EXPECT_EQ(6ui64, s.technology_m.technologies_container.size());

	const auto dragoon_tag = tag_from_text(s.military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, "dragoon"));
	const auto atech_a_cat = tag_from_text(s.technology_m.named_subcategory_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, "atech_a"));
	const auto a_factory = tag_from_text(s.economy_m.named_factory_types_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, "aeroplane_factory"));
	const auto canned_food_tag = tag_from_text(s.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, "canned_food"));

	EXPECT_TRUE(is_valid_index(s.technology_m.technologies_container[tech_tag(0)].unit_adjustment));
	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(0)].rebel_adjustment));
	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(0)].production_adjustment));
	EXPECT_EQ(2.0f, s.technology_m.unit_type_adjustments.get(s.technology_m.technologies_container[tech_tag(0)].unit_adjustment, dragoon_tag)[military::unit_attribute::attack]);
	EXPECT_EQ(0.0f, s.technology_m.unit_type_adjustments.get(s.technology_m.technologies_container[tech_tag(0)].unit_adjustment, military::unit_type_tag(0))[military::unit_attribute::attack]);
	EXPECT_EQ(0.0f, s.technology_m.unit_type_adjustments.get(s.technology_m.technologies_container[tech_tag(0)].unit_adjustment, military::unit_type_tag(1))[military::unit_attribute::attack]);
	EXPECT_EQ(5.0f, s.technology_m.unit_type_adjustments.get(s.technology_m.technologies_container[tech_tag(0)].unit_adjustment, military::unit_type_tag(0))[military::unit_attribute::organization]);
	EXPECT_EQ(0.1f, s.technology_m.unit_type_adjustments.get(s.technology_m.technologies_container[tech_tag(0)].unit_adjustment, military::unit_type_tag(0))[military::unit_attribute::supply_consumption]);
	EXPECT_EQ(tech_tag(0), s.technology_m.technology_subcategories[atech_a_cat].member_techs[0]);
	EXPECT_EQ(tech_tag(1), s.technology_m.technology_subcategories[atech_a_cat].member_techs[1]);
	EXPECT_EQ(tech_tag(), s.technology_m.technology_subcategories[atech_a_cat].member_techs[2]);
	EXPECT_EQ(1836ui16, s.technology_m.technologies_container[tech_tag(0)].year);
	EXPECT_EQ(3600ui16, s.technology_m.technologies_container[tech_tag(0)].cost);
	EXPECT_EQ(uint8_t(technology::unciv_military | technology::activate_fort), s.technology_m.technologies_container[tech_tag(0)].flags);
	EXPECT_EQ(1.0f, s.technology_m.technologies_container[tech_tag(0)].attributes[tech_offset::dig_in_cap]);
	EXPECT_NE(modifiers::factor_tag(), s.technology_m.technologies_container[tech_tag(0)].ai_chance);
	EXPECT_EQ(triggers::trigger_tag(), s.technology_m.technologies_container[tech_tag(0)].allow);
	EXPECT_EQ(modifiers::national_modifier_tag(), s.technology_m.technologies_container[tech_tag(0)].modifier);
	EXPECT_EQ(tech_tag(0), s.technology_m.technologies_container[tech_tag(0)].id);

	EXPECT_EQ(1836ui16, s.technology_m.technologies_container[tech_tag(1)].year);
	EXPECT_EQ(3000ui16, s.technology_m.technologies_container[tech_tag(1)].cost);
	EXPECT_EQ(a_factory, s.technology_m.technologies_container[tech_tag(1)].activate_factory);
	EXPECT_NE(modifiers::national_modifier_tag(), s.technology_m.technologies_container[tech_tag(1)].modifier);
	EXPECT_EQ(3.0f, s.modifiers_m.national_modifier_definitions[s.technology_m.technologies_container[tech_tag(1)].modifier][modifiers::national_offsets::tax_efficiency]);
	EXPECT_EQ(0ui16, s.technology_m.technologies_container[tech_tag(1)].flags);
	EXPECT_NE(modifiers::factor_tag(), s.technology_m.technologies_container[tech_tag(1)].ai_chance);

	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(2)].unit_adjustment));
	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(2)].rebel_adjustment));
	EXPECT_TRUE(is_valid_index(s.technology_m.technologies_container[tech_tag(2)].production_adjustment));
	EXPECT_EQ(1836ui16, s.technology_m.technologies_container[tech_tag(2)].year);
	EXPECT_EQ(10800ui16, s.technology_m.technologies_container[tech_tag(2)].cost);
	EXPECT_EQ(0ui8, s.technology_m.technologies_container[tech_tag(2)].flags);
	EXPECT_NE(modifiers::factor_tag(), s.technology_m.technologies_container[tech_tag(2)].ai_chance);
	EXPECT_EQ(0.1f, s.technology_m.production_adjustments.get(s.technology_m.technologies_container[tech_tag(2)].production_adjustment, economy_tag_to_production_adjustment<production_adjustment::rgo_goods_output>(canned_food_tag)));
	EXPECT_EQ(0.2f, s.technology_m.production_adjustments.get(s.technology_m.technologies_container[tech_tag(2)].production_adjustment, economy_tag_to_production_adjustment<production_adjustment::rgo_size>(canned_food_tag)));
	EXPECT_EQ(0.0f, s.technology_m.production_adjustments.get(s.technology_m.technologies_container[tech_tag(2)].production_adjustment, economy_tag_to_production_adjustment<production_adjustment::rgo_goods_throughput>(canned_food_tag)));
}

TEST(technologies_tests, read_inventions_test) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test2");

	scenario::scenario_manager s;

	economy::read_goods(s.economy_m, f.get_root(), s.gui_m.text_data_sequences);
	economy::read_buildings(s.economy_m, f.get_root(), s.gui_m.text_data_sequences);

	{
		military::parsing_state state(s.gui_m.text_data_sequences, s.military_m);
		military::pre_parse_unit_types(state, f.get_root());
	}

	{
		population::parsing_state env(s.gui_m.text_data_sequences, s.population_m);
		population::pre_parse_rebel_types(env, f.get_root());
	}

	parsing_state state(s.gui_m.text_data_sequences, f.get_root().get_directory(u"\\technologies"), s.technology_m, s.modifiers_m);

	pre_parse_technologies(state, f.get_root());
	pre_parse_inventions(state, f.get_root());

	prepare_technologies_read(s);
	read_inventions(state, s);
	EXPECT_EQ(6ui64, s.technology_m.technologies_container.size());

	const auto rebel_tag = tag_from_text(s.population_m.named_rebel_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, "rebel_1"));

	EXPECT_EQ(tech_tag(3), s.technology_m.technologies_container[tech_tag(3)].id);
	EXPECT_NE(triggers::trigger_tag(), s.technology_m.technologies_container[tech_tag(3)].allow);
	EXPECT_NE(modifiers::factor_tag(), s.technology_m.technologies_container[tech_tag(3)].ai_chance);
	EXPECT_EQ(technology::gas_defence, s.technology_m.technologies_container[tech_tag(3)].flags);
	EXPECT_EQ(modifiers::national_modifier_tag(), s.technology_m.technologies_container[tech_tag(3)].modifier);

	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(4)].unit_adjustment));
	EXPECT_TRUE(is_valid_index(s.technology_m.technologies_container[tech_tag(4)].rebel_adjustment));
	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(4)].production_adjustment));
	EXPECT_NE(triggers::trigger_tag(), s.technology_m.technologies_container[tech_tag(4)].allow);
	EXPECT_NE(modifiers::factor_tag(), s.technology_m.technologies_container[tech_tag(4)].ai_chance);
	EXPECT_EQ(0ui8, s.technology_m.technologies_container[tech_tag(4)].flags);
	EXPECT_NE(modifiers::national_modifier_tag(), s.technology_m.technologies_container[tech_tag(4)].modifier);
	EXPECT_EQ(2.5f, s.modifiers_m.national_modifier_definitions[s.technology_m.technologies_container[tech_tag(4)].modifier][modifiers::national_offsets::global_population_growth]);
	EXPECT_EQ(4.0f, s.modifiers_m.national_modifier_definitions[s.technology_m.technologies_container[tech_tag(4)].modifier][modifiers::national_offsets::farm_rgo_size]);
	EXPECT_EQ(-0.25f, s.technology_m.rebel_org_gain.get(s.technology_m.technologies_container[tech_tag(4)].rebel_adjustment, population::rebel_type_tag(1)));
	EXPECT_EQ(0.75f, s.technology_m.rebel_org_gain.get(s.technology_m.technologies_container[tech_tag(4)].rebel_adjustment, rebel_tag));

	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(5)].unit_adjustment));
	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(5)].rebel_adjustment));
	EXPECT_FALSE(is_valid_index(s.technology_m.technologies_container[tech_tag(5)].production_adjustment));
	EXPECT_EQ(tech_tag(5), s.technology_m.technologies_container[tech_tag(5)].id);
	EXPECT_EQ(triggers::trigger_tag(), s.technology_m.technologies_container[tech_tag(5)].allow);
	EXPECT_EQ(modifiers::factor_tag(), s.technology_m.technologies_container[tech_tag(5)].ai_chance);
	EXPECT_EQ(0ui8, s.technology_m.technologies_container[tech_tag(5)].flags);
	EXPECT_EQ(modifiers::national_modifier_tag(), s.technology_m.technologies_container[tech_tag(5)].modifier);
	EXPECT_EQ(15.0f, s.technology_m.technologies_container[tech_tag(5)].shared_prestige);
	EXPECT_EQ(1.0f, s.technology_m.technologies_container[tech_tag(5)].attributes[tech_offset::max_national_focus]);
}
