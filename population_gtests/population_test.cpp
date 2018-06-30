#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "population\\population.h"
#include "scenario\\scenario.h"
#include "events\\events_io.h"
#include "military\\military_io.h"
#include "economy\\economy_io.h"
#include "ideologies\\ideologies_io.h"
#include "population\\population_io.h"
#include "issues\\issues_io.h"
#include "governments\\governments_io.h"
#include "world_state\\world_state.h"
#include "scenario\\scenario_io.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation poptypes = directory_representation(u"poptypes", f_root);
	file_representation f1 = file_representation(u"bureaucrats.txt", poptypes,
		"sprite = 12\r\n"
		"color = { 136 136 136 }\r\n"
		"strata = middle\r\n"
		"state_capital_only = yes\r\n"
		"demote_migrant = yes\r\n"
		"rebel =\r\n"
		"{\r\n"
		"	dragoon = 0.5\r\n"
		"	plane_b = 1.0\r\n"
		"}\r\n"
		"administrative_efficiency = yes\r\n"
		"tax_eff = 1.0\r\n"
		"life_needs_income = {\r\n"
		"	type = administration\r\n"
		"	weight = 1\r\n"
		"}\r\n"
		"everyday_needs_income = {\r\n"
		"	type = administration\r\n"
		"	weight = 2\r\n"
		"}\r\n"
		"luxury_needs_income = {\r\n"
		"	type = administration\r\n"
		"	weight = 3\r\n"
		"}\r\n"
		"luxury_needs = {\r\n"
		"	cement = 1.0\r\n"
		"	coal = 2\r\n"
		"}\r\n"
		"everyday_needs = {\r\n"
		"	cement = 10\r\n"
		"}\r\n"
		"life_needs = {\r\n"
		"	canned_food = 0.75\r\n"
		"}\r\n"
		"country_migration_target = {\r\n"
		"	factor = 1\r\n"
		"	modifier = {\r\n"
		"		factor = 0.9\r\n"
		"		unemployment = 0.05\r\n"
		"	}	\r\n"
		"	modifier = {\r\n"
		"		factor = 1.2\r\n"
		"		is_greater_power = yes\r\n"
		"	}\r\n"
		"}\r\n"
		"migration_target = {\r\n"
		"	factor = 0.01\r\n"
		"	modifier = {\r\n"
		"		factor = 0.1\r\n"
		"		unemployment = 0.1\r\n"
		"		state_scope = {\r\n"
		"			has_factories = no\r\n"
		"		}\r\n"
		"	}\r\n"
		"	modifier = {\r\n"
		"		factor = 1.2\r\n"
		"		total_pops = 150000\r\n"
		"	}\r\n"
		"}\r\n"
		"promote_to = {\r\n"
		"	clergy = {\r\n"
		"		factor = 0.01\r\n"
		"		modifier = {\r\n"
		"			factor = -1\r\n"
		"			state_scope = {\r\n"
		"				has_factories = no\r\n"
		"			}\r\n"
		"			NOT = { total_pops = 200000 }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	capitalists = {\r\n"
		"		factor = 0.01\r\n"
		"		modifier = {\r\n"
		"			factor = -1\r\n"
		"			NOT = { literacy = 0.30 }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	slaves = { factor = 1 }\r\n"
		"}"
		"ideologies = {\r\n"
		"	ideology_a = {\r\n"
		"		factor = 1\r\n"
		"		modifier = {\r\n"
		"			factor = 0.5\r\n"
		"			not = { militancy = 6 }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	ideology_b = {\r\n"
		"		factor = 1\r\n"
		"		modifier = {\r\n"
		"			factor = 0.5\r\n"
		"			not = { political_reform_want = 0.05 }\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		"issues = {\r\n"
		"	opt_a = {\r\n"
		"		factor = 1.1\r\n"
		"		modifier = {\r\n"
		"			factor = 1.1\r\n"
		"			NOT = {\r\n"
		"				life_needs = 1.0\r\n"
		"			}\r\n"
		"			country = { trade_policy = free_trade }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	opt_d = {\r\n"
		"		factor = 1\r\n"
		"		modifier = {\r\n"
		"			factor = 1.1\r\n"
		"			revanchism = 0.01\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
	);
	file_representation f2 = file_representation(u"clergy.txt", poptypes,
		"sprite = 3\r\n"
		"color = { 234 227 40 }\r\n"
		"strata = middle\r\n"
		"research_points = 3\r\n"
		"research_optimum = 0.5\r\n"
		"can_reduce_consciousness = yes\r\n"
		"luxury_needs_income = {\r\n"
		"	type = reforms\r\n"
		"	weight = 1\r\n"
		"}\r\n"
		"country_migration_target = {\r\n"
		"	factor = 1\r\n"
		"	modifier = {\r\n"
		"		factor = 0.9\r\n"
		"		unemployment = 0.05\r\n"
		"	}	\r\n"
		"	modifier = {\r\n"
		"		factor = 1.2\r\n"
		"		is_greater_power = yes\r\n"
		"	}\r\n"
		"}\r\n"
	);
	file_representation f3 = file_representation(u"capitalists.txt", poptypes,
		"sprite = 4\r\n"
		"color = { 18 129 10 }\r\n"
		"strata = rich\r\n"
		"can_build = yes\r\n"
	);
	file_representation f4 = file_representation(u"slaves.txt", poptypes,
		"sprite = 10\r\n"
		"allowed_to_vote = no\r\n"
		"is_slave = yes\r\n"
		"color = { 17 17 17 }\r\n"
		"unemployment = yes\r\n"
		"strata = poor\r\n"
	);
	directory_representation common1 = directory_representation(u"common", f_root);
	file_representation rebel_types = file_representation(u"rebel_types.txt", common1,
		"rebel_1 = {\r\n"
		"	icon = 5\r\n"
		"	area = nation\r\n"
		"	break_alliance_on_win = yes\r\n"
		"	government = {\r\n"
		"		proletarian_dictatorship = democracy\r\n"
		"		absolute_monarchy = proletarian_dictatorship\r\n"
		"	}\r\n"
		"	defection = none\r\n"
		"	independence = none	\r\n"
		"	defect_delay = 18\r\n"
		"	ideology = ideology_a\r\n"
		"	allow_all_cultures = yes\r\n"
		"	allow_all_religions = yes\r\n"
		"	allow_all_ideologies = yes\r\n"
		"	resilient = yes\r\n"
		"	reinforcing = yes\r\n"
		"	general = yes\r\n"
		"	smart = yes\r\n"
		"	unit_transfer = no	\r\n"
		"	occupation_mult = 5.0\r\n"
		"	will_rise = {\r\n"
		"		factor = 0.1\r\n"
		"		modifier = {\r\n"
		"			war = yes\r\n"
		"			factor = 0.8\r\n"
		"		}\r\n"
		"	}\r\n"
		"	spawn_chance = {\r\n"
		"		factor = 20\r\n"
		"		modifier = {\r\n"
		"			factor = 0\r\n"
		"			country = { has_country_flag = sharing_power }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	movement_evaluation = {\r\n"
		"		factor = 1\r\n"
		"		modifier = {\r\n"
		"			factor = 0.1\r\n"
		"			units_in_province = 1\r\n"
		"			NOT = { is_capital = yes }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	demands_enforced_trigger = {\r\n"
		"		OR = {\r\n"
		"			capital_scope = {\r\n"
		"				controlled_by = REB\r\n"
		"				province_control_days = 250\r\n"
		"			}\r\n"
		"			any_owned_province = {\r\n"
		"				controlled_by = REB\r\n"
		"				province_control_days = 730\r\n"
		"			}\r\n"
		"			NOT = {\r\n"
		"				any_owned_province = {\r\n"
		"					is_colonial = no\r\n"
		"					controlled_by = THIS\r\n"
		"				}\r\n"
		"			}\r\n"
		"		}\r\n"
		"	}\r\n"
		"	demands_enforced_effect = {\r\n"
		"		clr_country_flag = theocratic_government\r\n"
		"		war_exhaustion = 10\r\n"
		"	}\r\n"
		"	siege_won_trigger = {\r\n"
		"		units_in_province = 1\r\n"
		"	}\r\n"
		"	siege_won_effect = {\r\n"
		"		any_pop = {\r\n"
		"			ideology = {\r\n"
		"				value = ideology_a\r\n"
		"				factor = 0.05\r\n"
		"			}\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		"rebel_2 = {\r\n"
		"	area = religion\r\n"
		"	icon = 3\r\n"
		"	defection = pan_nationalist\r\n"
		"	independence = culture_group\r\n"
		"	allow_all_religions = no\r\n"
		"	allow_all_ideologies = no\r\n"
		"}");
	file_representation pop_types = file_representation(u"pop_types.txt", common1, 
		"demotion_chance =\r\n"
		"{\r\n"
		"	factor = 1\r\n"
		"	modifier = {\r\n"
		"		factor = -0.1\r\n"
		"		not = { religion = this }\r\n"
		"		strata = poor\r\n"
		"	}"
		"}\r\n"
		"emigration_chance = \r\n"
		"{\r\n"
		"	factor = 0.5\r\n"
		"	group = {\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 5\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 6\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 7\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 8\r\n"
		"		}\r\n"
		"		modifier = {\r\n"
		"			factor = 0.2\r\n"
		"			militancy = 9\r\n"
		"		}\r\n"
		"	}\r\n"
		"}\r\n"
		);
	directory_representation units1 = directory_representation(u"units", f_root);
	file_representation uf1 = file_representation(u"dragoon.txt", units1, "dragoon = { stuff }");
	file_representation uf2 = file_representation(u"airplane.txt", units1, "plane_a = { stuff }\r\nplane_b = {}");
	file_representation goods1 = file_representation(u"goods.txt", common1,
		"military_goods   = {\r\n"
		"ammunition = {\r\n"
		"cost = 2.0\r\n"
		"}\r\n"
		"cement = {\r\n"
		"cost = 3.0\r\n"
		"}\r\n"
		"coal = {\r\n"
		"cost = 1.0\r\n"
		"}\r\n"
		"canned_food = {\r\n"
		"cost = 5.0\r\n"
		"}\r\n"
		"}\r\n");
	file_representation id1 = file_representation(u"ideologies.txt", common1,
		"group_a = {\r\n"
		"ideology_a = {}\r\n"
		"ideology_b = {}\r\n"
		"}\r\n"
		"group_2 = { ideology_c = {} }");
	file_representation issues1 = file_representation(u"issues.txt", common1,
		"party_issues  = {\r\n"
		"trade_policy  = { opt_a = { stuff } opt_b = { stuff } }\r\n"
		"}\r\n"
		"political_reforms = {\r\n"
		"slavery = { opt_d = {} opt_q = {} \r\n"
		"administrative = yes\r\n"
		"}\r\n"
		"}\r\n");
	file_representation gov1 = file_representation(u"governments.txt", common1,
		"proletarian_dictatorship  = {\r\n"
		"}\r\n"
		"absolute_monarchy  = {\r\n"
		"}\r\n"
		"democracy  = {\r\n"
		"}\r\n");

	directory_representation history1 = directory_representation(u"history", f_root);
	directory_representation pops1 = directory_representation(u"pops", history1);
	directory_representation hd1 = directory_representation(u"1900.1.1", pops1);
	directory_representation hd2 = directory_representation(u"1850.1.1", pops1);
	directory_representation hd3 = directory_representation(u"1800.1.1", pops1);

	file_representation hf1 = file_representation(u"file.txt", hd1,
		"853 = {\r\n"
		"	aristocrats = {\r\n"
		"		culture = albanian\r\n"
		"		religion = sunni\r\n"
		"		size = 750\r\n"
		"	}\r\n"
		"}\r\n"
		);
	file_representation hf2 = file_representation(u"fileb.txt", hd2,
		"853 = {\r\n"
		"	clergymen = {\r\n"
		"		culture = albanian\r\n"
		"		religion = orthodox\r\n"
		"		size = 50\r\n"
		"	}\r\n"
		"	farmers = {\r\n"
		"		culture = albanian\r\n"
		"		religion = orthodox\r\n"
		"		size = 6750\r\n"
		"	}\r\n"
		"}\r\n");
	file_representation hf3 = file_representation(u"file.txt", hd3,
		"850 = { farmers =  { culture = beifaren religion = mahayana size = 5000 } }");
	file_representation hf4 = file_representation(u"fileb.txt", hd3,
		"851 = { officers = { culture = manchu religion = mahayana size = 50 } }");


	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace population;

TEST(population_tests, test_pre_parse_file_scan) {
	population_manager manager;

	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	text_data::text_sequences tex;
	pre_parse_pop_types(manager, f.get_root(), tex);

	EXPECT_EQ(4ui32, manager.count_poptypes);
	EXPECT_EQ(4ui64, manager.pop_types.size());
	EXPECT_EQ(4ui64, manager.named_pop_type_index.size());

	EXPECT_EQ(pop_type_tag(1), manager.pop_types[pop_type_tag(1)].id);
	EXPECT_EQ(pop_type_tag(1), manager.named_pop_type_index[manager.pop_types[pop_type_tag(1)].name]);
}

TEST(population_tests, test_pre_parse_rebel_types) {
	population_manager manager;

	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	text_data::text_sequences tex;
	parsing_state env(tex, manager);

	pre_parse_rebel_types(env, f.get_root());

	EXPECT_EQ(2ui64, manager.rebel_types.size());
	EXPECT_EQ(2ui64, manager.named_rebel_type_index.size());

	EXPECT_EQ(rebel_type_tag(0), manager.rebel_types[rebel_type_tag(0)].id);
	EXPECT_EQ(rebel_type_tag(1), manager.rebel_types[rebel_type_tag(1)].id);

	EXPECT_EQ(rebel_type_tag(0), manager.named_rebel_type_index[manager.rebel_types[rebel_type_tag(0)].name]);
	EXPECT_EQ(rebel_type_tag(1), manager.named_rebel_type_index[manager.rebel_types[rebel_type_tag(1)].name]);
}

TEST(population_tests, test_poptype_file_read) {
	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	scenario::scenario_manager s;

	read_main_poptype_file(s, f.get_root());

	EXPECT_NE(modifiers::factor_tag(), s.population_m.emigration_chance);
	EXPECT_NE(modifiers::factor_tag(), s.population_m.demotion_chance);
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.colonialmigration_chance);
}

TEST(population_tests, test_individual_poptype) {
	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	scenario::scenario_manager s;

	pre_parse_pop_types(s.population_m , f.get_root(), s.gui_m.text_data_sequences);

	military::parsing_state mstate(s.gui_m.text_data_sequences, s.military_m);
	pre_parse_unit_types(mstate, f.get_root());

	economy::read_goods(s.economy_m, f.get_root(), s.gui_m.text_data_sequences);

	ideologies::pre_parse_ideologies(s.ideologies_m, f.get_root(), s.gui_m.text_data_sequences);
	issues::pre_parse_issues(s.issues_m, f.get_root(), s.gui_m.text_data_sequences);

	read_poptypes(s, f.get_root());
	
	const auto cement_tag = tag_from_text(s.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("cement")));
	const auto canned_food_tag = tag_from_text(s.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("canned_food")));
	const auto coal_tag = tag_from_text(s.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("coal")));

	const auto dragoon_tag = tag_from_text(s.military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("dragoon")));
	const auto plane_a_tag = tag_from_text(s.military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("plane_a")));
	const auto plane_b_tag = tag_from_text(s.military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("plane_b")));

	const auto opt_a_tag = tag_from_text(s.issues_m.named_option_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("opt_a")));
	const auto opt_b_tag = tag_from_text(s.issues_m.named_option_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("opt_b")));
	const auto opt_d_tag = tag_from_text(s.issues_m.named_option_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("opt_d")));
	const auto opt_q_tag = tag_from_text(s.issues_m.named_option_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("opt_q")));

	const auto ideology_a_tag = tag_from_text(s.ideologies_m.named_ideology_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("ideology_a")));
	const auto ideology_b_tag = tag_from_text(s.ideologies_m.named_ideology_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("ideology_b")));
	const auto ideology_c_tag = tag_from_text(s.ideologies_m.named_ideology_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("ideology_c")));
	
	const auto bureaucrats_tag = tag_from_text(s.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("bureaucrats")));
	const auto clergy_tag = tag_from_text(s.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("clergy")));
	const auto capitalists_tag = tag_from_text(s.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("capitalists")));
	const auto slaves_tag = tag_from_text(s.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("slaves")));

	EXPECT_EQ(s.population_m.capitalist, capitalists_tag);
	EXPECT_EQ(s.population_m.clergy, clergy_tag);
	EXPECT_EQ(s.population_m.slave, slaves_tag);
	EXPECT_EQ(s.population_m.bureaucrat, bureaucrats_tag);
	EXPECT_EQ(s.population_m.artisan, pop_type_tag());

	EXPECT_EQ(12ui8, s.population_m.pop_types[bureaucrats_tag].sprite);
	EXPECT_EQ(136ui8, s.population_m.pop_types[bureaucrats_tag].color.b);
	EXPECT_EQ(uint8_t(pop_type::strata_middle | pop_type::state_capital_only | pop_type::demote_on_migration), s.population_m.pop_types[bureaucrats_tag].flags);
	EXPECT_EQ(0ui8, s.population_m.pop_types[bureaucrats_tag].research_points);
	EXPECT_EQ(0.0f, s.population_m.pop_types[bureaucrats_tag].research_optimum);
	EXPECT_EQ(0.5f, s.population_m.rebel_units.get(bureaucrats_tag, dragoon_tag));
	EXPECT_EQ(1.0f, s.population_m.rebel_units.get(bureaucrats_tag, plane_b_tag));
	EXPECT_EQ(0.0f, s.population_m.rebel_units.get(bureaucrats_tag, plane_a_tag));

	EXPECT_EQ(income_type::administration, s.population_m.bureaucrat_pay.life_needs_income_type);
	EXPECT_EQ(income_type::administration, s.population_m.bureaucrat_pay.everyday_needs_income_type);
	EXPECT_EQ(income_type::administration, s.population_m.bureaucrat_pay.luxury_needs_income_type);
	EXPECT_EQ(1.0f, s.population_m.bureaucrat_pay.life_needs_income_weight);
	EXPECT_EQ(2.0f, s.population_m.bureaucrat_pay.everyday_needs_income_weight);
	EXPECT_EQ(3.0f, s.population_m.bureaucrat_pay.luxury_needs_income_weight);

	EXPECT_EQ(0.75, s.population_m.life_needs.get(bureaucrats_tag, canned_food_tag));
	EXPECT_EQ(0.0, s.population_m.life_needs.get(bureaucrats_tag, cement_tag));
	EXPECT_EQ(0.0, s.population_m.life_needs.get(bureaucrats_tag, coal_tag));

	EXPECT_EQ(0.0, s.population_m.everyday_needs.get(bureaucrats_tag, canned_food_tag));
	EXPECT_EQ(10.0, s.population_m.everyday_needs.get(bureaucrats_tag, cement_tag));
	EXPECT_EQ(0.0, s.population_m.everyday_needs.get(bureaucrats_tag, coal_tag));

	EXPECT_EQ(0.0, s.population_m.luxury_needs.get(bureaucrats_tag, canned_food_tag));
	EXPECT_EQ(1.0, s.population_m.luxury_needs.get(bureaucrats_tag, cement_tag));
	EXPECT_EQ(2.0, s.population_m.luxury_needs.get(bureaucrats_tag, coal_tag));

	EXPECT_NE(modifiers::factor_tag(), s.population_m.pop_types[bureaucrats_tag].country_migration_target);
	EXPECT_NE(modifiers::factor_tag(), s.population_m.pop_types[bureaucrats_tag].migration_target);

	EXPECT_EQ(modifiers::factor_tag(), s.population_m.promote_to.get(bureaucrats_tag, slaves_tag));
	EXPECT_NE(modifiers::factor_tag(), s.population_m.promote_to.get(bureaucrats_tag, clergy_tag));
	EXPECT_NE(modifiers::factor_tag(), s.population_m.promote_to.get(bureaucrats_tag, capitalists_tag));

	EXPECT_NE(modifiers::factor_tag(), s.population_m.demote_to.get(bureaucrats_tag, slaves_tag));
	EXPECT_NE(modifiers::factor_tag(), s.population_m.demote_to.get(bureaucrats_tag, clergy_tag));
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.demote_to.get(bureaucrats_tag, capitalists_tag));

	EXPECT_NE(modifiers::factor_tag(), s.population_m.ideological_inclination.get(bureaucrats_tag, ideology_a_tag));
	EXPECT_NE(modifiers::factor_tag(), s.population_m.ideological_inclination.get(bureaucrats_tag, ideology_b_tag));
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.ideological_inclination.get(bureaucrats_tag, ideology_c_tag));

	EXPECT_NE(modifiers::factor_tag(), s.population_m.issue_inclination.get(bureaucrats_tag, opt_a_tag));
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.issue_inclination.get(bureaucrats_tag, opt_b_tag));
	EXPECT_NE(modifiers::factor_tag(), s.population_m.issue_inclination.get(bureaucrats_tag, opt_d_tag));
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.issue_inclination.get(bureaucrats_tag, opt_q_tag));

	EXPECT_EQ(3ui8, s.population_m.pop_types[clergy_tag].sprite);
	EXPECT_EQ(227ui8, s.population_m.pop_types[clergy_tag].color.g);
	EXPECT_EQ(uint8_t(pop_type::strata_middle), s.population_m.pop_types[clergy_tag].flags);
	EXPECT_EQ(3ui8, s.population_m.pop_types[clergy_tag].research_points);
	EXPECT_EQ(0.5f, s.population_m.pop_types[clergy_tag].research_optimum);

	EXPECT_EQ(income_type::none, s.population_m.clergy_pay.life_needs_income_type);
	EXPECT_EQ(income_type::none, s.population_m.clergy_pay.everyday_needs_income_type);
	EXPECT_EQ(income_type::reforms, s.population_m.clergy_pay.luxury_needs_income_type);
	EXPECT_EQ(0.0f, s.population_m.clergy_pay.life_needs_income_weight);
	EXPECT_EQ(0.0f, s.population_m.clergy_pay.everyday_needs_income_weight);
	EXPECT_EQ(1.0f, s.population_m.clergy_pay.luxury_needs_income_weight);

	EXPECT_NE(modifiers::factor_tag(), s.population_m.pop_types[clergy_tag].country_migration_target);
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.pop_types[clergy_tag].migration_target);
	EXPECT_EQ(s.population_m.pop_types[bureaucrats_tag].country_migration_target, s.population_m.pop_types[clergy_tag].country_migration_target);

	EXPECT_EQ(4ui8, s.population_m.pop_types[capitalists_tag].sprite);
	EXPECT_EQ(18ui8, s.population_m.pop_types[capitalists_tag].color.r);
	EXPECT_EQ(uint8_t(pop_type::strata_rich), s.population_m.pop_types[capitalists_tag].flags);

	EXPECT_EQ(10ui8, s.population_m.pop_types[slaves_tag].sprite);
	EXPECT_EQ(17ui8, s.population_m.pop_types[slaves_tag].color.g);
	EXPECT_EQ(uint8_t(pop_type::strata_poor | pop_type::cannot_vote | pop_type::is_employable), s.population_m.pop_types[slaves_tag].flags);
}

TEST(population_tests, read_rebels) {
	preparse_test_files real_fs;
	file_system f;
	f.set_root(RANGE(u"F:"));

	scenario::scenario_manager s;

	parsing_state env(s.gui_m.text_data_sequences, s.population_m);

	pre_parse_rebel_types(env, f.get_root());

	ideologies::pre_parse_ideologies(s.ideologies_m, f.get_root(), s.gui_m.text_data_sequences);
	governments::read_governments(s.governments_m, f.get_root(), s.gui_m.text_data_sequences, s.ideologies_m);

	events::event_creation_manager ecm;

	read_rebel_types(env, s, ecm);

	EXPECT_EQ(2ui64, s.population_m.rebel_types.size());
	EXPECT_EQ(uint16_t(rebel_type::area_nation | rebel_type::defection_none | rebel_type::independence_none | rebel_type::break_alliance_on_win), s.population_m.rebel_types[rebel_type_tag(0)].flags);
	EXPECT_EQ(rebel_type_tag(0), s.population_m.rebel_types[rebel_type_tag(0)].id);
	EXPECT_EQ(tag_from_text(s.ideologies_m.named_ideology_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("ideology_a"))), s.population_m.rebel_types[rebel_type_tag(0)].ideology);
	EXPECT_EQ(5.0f, s.population_m.rebel_types[rebel_type_tag(0)].occupation_mult);
	EXPECT_EQ(5ui8, s.population_m.rebel_types[rebel_type_tag(0)].icon);
	EXPECT_NE(modifiers::factor_tag(), s.population_m.rebel_types[rebel_type_tag(0)].will_rise);
	EXPECT_NE(modifiers::factor_tag(), s.population_m.rebel_types[rebel_type_tag(0)].movement_evaluation);
	EXPECT_NE(modifiers::factor_tag(), s.population_m.rebel_types[rebel_type_tag(0)].spawn_chance);
	EXPECT_NE(triggers::trigger_tag(), s.population_m.rebel_types[rebel_type_tag(0)].siege_won_trigger);
	EXPECT_NE(triggers::trigger_tag(), s.population_m.rebel_types[rebel_type_tag(0)].demands_enforced_trigger);
	EXPECT_NE(triggers::effect_tag(), s.population_m.rebel_types[rebel_type_tag(0)].siege_won_effect);
	EXPECT_NE(triggers::effect_tag(), s.population_m.rebel_types[rebel_type_tag(0)].demands_enforced_effect);

	EXPECT_EQ(uint16_t(rebel_type::area_religion | rebel_type::defection_pan_nationalist | rebel_type::independence_culture_group | rebel_type::restrict_by_ideology | rebel_type::restrict_by_religion), s.population_m.rebel_types[rebel_type_tag(1)].flags);
	EXPECT_EQ(rebel_type_tag(1), s.population_m.rebel_types[rebel_type_tag(1)].id);
	EXPECT_EQ(ideologies::ideology_tag(), s.population_m.rebel_types[rebel_type_tag(1)].ideology);
	EXPECT_EQ(1.0f, s.population_m.rebel_types[rebel_type_tag(1)].occupation_mult);
	EXPECT_EQ(3ui8, s.population_m.rebel_types[rebel_type_tag(1)].icon);
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.rebel_types[rebel_type_tag(1)].will_rise);
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.rebel_types[rebel_type_tag(1)].movement_evaluation);
	EXPECT_EQ(modifiers::factor_tag(), s.population_m.rebel_types[rebel_type_tag(1)].spawn_chance);
	EXPECT_EQ(triggers::trigger_tag(), s.population_m.rebel_types[rebel_type_tag(1)].siege_won_trigger);
	EXPECT_EQ(triggers::trigger_tag(), s.population_m.rebel_types[rebel_type_tag(1)].demands_enforced_trigger);
	EXPECT_EQ(triggers::effect_tag(), s.population_m.rebel_types[rebel_type_tag(1)].siege_won_effect);
	EXPECT_EQ(triggers::effect_tag(), s.population_m.rebel_types[rebel_type_tag(1)].demands_enforced_effect);

	const auto proletarian_dictatorship = tag_from_text(s.governments_m.named_government_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("proletarian_dictatorship")));
	const auto absolute_monarchy = tag_from_text(s.governments_m.named_government_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("absolute_monarchy")));
	const auto democracy = tag_from_text(s.governments_m.named_government_index, text_data::get_thread_safe_existing_text_handle(s.gui_m.text_data_sequences, RANGE("democracy")));

	EXPECT_EQ(democracy, s.population_m.rebel_change_government_to.get(rebel_type_tag(0), proletarian_dictatorship));
	EXPECT_EQ(proletarian_dictatorship, s.population_m.rebel_change_government_to.get(rebel_type_tag(0), absolute_monarchy));
	EXPECT_EQ(democracy, s.population_m.rebel_change_government_to.get(rebel_type_tag(0), democracy));

	EXPECT_EQ(proletarian_dictatorship, s.population_m.rebel_change_government_to.get(rebel_type_tag(1), proletarian_dictatorship));
	EXPECT_EQ(absolute_monarchy, s.population_m.rebel_change_government_to.get(rebel_type_tag(1), absolute_monarchy));
	EXPECT_EQ(democracy, s.population_m.rebel_change_government_to.get(rebel_type_tag(1), democracy));
}

TEST(population_tests, population_directory_selection) {
	{
		world_state ws;
		serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s);
		ready_world_state(ws);

		preparse_test_files real_fs;
		file_system f;
		f.set_root(RANGE(u"F:"));

		read_all_pops(f.get_root(), ws, date_to_tag(boost::gregorian::date(1901, boost::gregorian::Jan, 1)));
		
		EXPECT_EQ(1ui32, get_size(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(853)].pops));

		auto popid = get(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(853)].pops, 0);
		pop& pop_obj = ws.w.population_s.pops.get(popid);

		EXPECT_EQ(750, pop_obj.size);
		EXPECT_EQ(provinces::province_tag(853), pop_obj.location);
		EXPECT_EQ(pop_obj.culture, tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("albanian"))));
		EXPECT_EQ(pop_obj.religion, tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("sunni"))));
		EXPECT_EQ(pop_obj.type, tag_from_text(ws.s.population_m.named_pop_type_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("aristocrats"))));
	}

	{
		world_state ws;
		serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s);
		ready_world_state(ws);

		preparse_test_files real_fs;
		file_system f;
		f.set_root(RANGE(u"F:"));

		read_all_pops(f.get_root(), ws, date_to_tag(boost::gregorian::date(1851, boost::gregorian::Jan, 1)));

		EXPECT_EQ(2ui32, get_size(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(853)].pops));

		{
			auto popid = get(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(853)].pops, 0);
			pop& pop_obj = ws.w.population_s.pops.get(popid);

			EXPECT_EQ(50, pop_obj.size);
			EXPECT_EQ(provinces::province_tag(853), pop_obj.location);
			EXPECT_EQ(pop_obj.culture, tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("albanian"))));
			EXPECT_EQ(pop_obj.religion, tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("orthodox"))));
			EXPECT_EQ(pop_obj.type, tag_from_text(ws.s.population_m.named_pop_type_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("clergymen"))));
		}
		{
			auto popid = get(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(853)].pops, 1);
			pop& pop_obj = ws.w.population_s.pops.get(popid);

			EXPECT_EQ(6750, pop_obj.size);
			EXPECT_EQ(provinces::province_tag(853), pop_obj.location);
			EXPECT_EQ(pop_obj.culture, tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("albanian"))));
			EXPECT_EQ(pop_obj.religion, tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("orthodox"))));
			EXPECT_EQ(pop_obj.type, tag_from_text(ws.s.population_m.named_pop_type_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("farmers"))));
		}
	}
	{
		world_state ws;
		serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s);
		ready_world_state(ws);

		preparse_test_files real_fs;
		file_system f;
		f.set_root(RANGE(u"F:"));

		read_all_pops(f.get_root(), ws, date_to_tag(boost::gregorian::date(1801, boost::gregorian::Jan, 1)));

		{
			EXPECT_EQ(1ui32, get_size(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(850)].pops));
			auto popid = get(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(850)].pops, 0);
			pop& pop_obj = ws.w.population_s.pops.get(popid);

			EXPECT_EQ(5000, pop_obj.size);
			EXPECT_EQ(provinces::province_tag(850), pop_obj.location);
			EXPECT_EQ(pop_obj.culture, tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("beifaren"))));
			EXPECT_EQ(pop_obj.religion, tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("mahayana"))));
			EXPECT_EQ(pop_obj.type, tag_from_text(ws.s.population_m.named_pop_type_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("farmers"))));
		}
		{
			EXPECT_EQ(1ui32, get_size(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(851)].pops));
			auto popid = get(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[provinces::province_tag(851)].pops, 0);
			pop& pop_obj = ws.w.population_s.pops.get(popid);

			EXPECT_EQ(50, pop_obj.size);
			EXPECT_EQ(provinces::province_tag(851), pop_obj.location);
			EXPECT_EQ(pop_obj.culture, tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("manchu"))));
			EXPECT_EQ(pop_obj.religion, tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("mahayana"))));
			EXPECT_EQ(pop_obj.type, tag_from_text(ws.s.population_m.named_pop_type_index, text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, RANGE("officers"))));
		}
	}
}
