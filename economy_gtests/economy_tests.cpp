#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "economy\\economy.h"
#include "scenario\\scenario.h"

using namespace economy;

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

const char gf3[] = 	"military_goods   = {\r\n"
		"money_good = {\r\n"
		"cost = 8\r\n"
		"color = { 0 0 0 }\r\n"
		"overseas_penalty  = yes\r\n"
		"tradeable = no\r\n"
		"money = yes\r\n"
		"}\r\n"
		"}\r\n"
		"other_goods = {\r\n"
		"cement = { \r\n"
		"	cost = 16.0\r\n"
		"	color = { 185 187 164 }\r\n"
		"}\r\n"
		"glass = { \r\n"
		"	cost = 2.9\r\n"
		"	color = { 201 200 199 }\r\n"
		"}\r\n"
	"}";

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation common1 = directory_representation(u"common", test1);
	file_representation goods1 = file_representation(u"goods.txt", common1,
		"military_goods   = {\r\n"
		"ammunition = {\r\n"
		"cost = 17.50\r\n"
		"color = { 208 202 127 }\r\n"
		"available_from_start = no\r\n"
		"}\r\n"
		"}\r\n");
	directory_representation test2 = directory_representation(u"test2", f_root);
	directory_representation common2 = directory_representation(u"common", test2);
	file_representation goods2 = file_representation(u"goods.txt", common2,
		"military_goods   = {\r\n"
		"money_good = {\r\n"
		"cost = 8\r\n"
		"color = { 0 0 0 }\r\n"
		"overseas_penalty  = yes\r\n"
		"tradeable = no\r\n"
		"money = yes\r\n"
		"}\r\n"
		"ammunition = {\r\n"
		"cost = 17.50\r\n"
		"color = { 208 202 127 }\r\n"
		"available_from_start = no\r\n"
		"}\r\n"
		"}\r\n");
	directory_representation test3 = directory_representation(u"test3", f_root);
	directory_representation common3 = directory_representation(u"common", test3);
	file_representation goods3 = file_representation(u"goods.txt", common3, gf3);
	file_representation buildings3 = file_representation(u"buildings.txt", common3,
		"aeroplane_factory = {\r\n"
		"type = factory\r\n"
		"on_completion = factory\r\n"
		"completion_size = 0.2\r\n"
		"max_level = 99\r\n"
		"goods_cost = \r\n"
		"{\r\n"
		"cement = 600\r\n"
		"}\r\n"
		"time = 14\r\n"
		"visibility = yes\r\n"
		"onmap = no\r\n"
		"production_type = aeroplane_factory\r\n"
		"pop_build_factory = yes\r\n"
		"advanced_factory = yes\r\n"
		"}\r\n");
	directory_representation test4 = directory_representation(u"test4", f_root);
	directory_representation common4 = directory_representation(u"common", test4);
	file_representation goods4 = file_representation(u"goods.txt", common4, gf3);
	file_representation buildings4 = file_representation(u"buildings.txt", common4,
		"aeroplane_factory = {\r\n"
		"type = factory\r\n"
		"on_completion = factory\r\n"
		"completion_size = 0.2\r\n"
		"max_level = 99\r\n"
		"goods_cost = \r\n"
		"{\r\n"
		"cement = 600\r\n"
		"}\r\n"
		"time = 14\r\n"
		"visibility = yes\r\n"
		"onmap = no\r\n"
		"production_type = aeroplane_factory\r\n"
		"pop_build_factory = yes\r\n"
		"advanced_factory = yes\r\n"
		"}\r\n"
		"fort = {\r\n"
		"type = fort\r\n"
		"goods_cost = \r\n"
		"{\r\n"
		"	cement = 100\r\n"
		"}\r\n"
		"time = 10\r\n"
		"visibility = yes\r\n"
		"onmap = yes\r\n"
		"max_level = 1\r\n"
		"province = yes\r\n"
		"fort_level = 1\r\n"
		"}\r\n"
		"naval_base = {\r\n"
		"type = naval_base\r\n"
		"cost = 15000\r\n"
		"goods_cost =\r\n"
		"{\r\n"
		"cement = 200\r\n"
		"}\r\n"
		"time = 20\r\n"
		"naval_capacity = 2\r\n"
		"capital = yes\r\n"
		"onmap = yes\r\n"
		"port = yes\r\n"
		"visibility = yes\r\n"
		"max_level = 2\r\n"
		"colonial_points = { 1 2 3 4 5 6 } #points at level\r\n"
		"province = yes\r\n"
		"one_per_state = yes\r\n"
		"colonial_range = 50\r\n"
		"local_ship_build = -1.5\r\n"
		"}\r\n"
		"railroad = {\r\n"
		"type = infrastructure\r\n"
		"goods_cost =\r\n"
		"{\r\n"
		"	cement = 300\r\n"
		"}\r\n"
		"time = 30\r\n"
		"visibility = yes\r\n"
		"onmap = yes\r\n"
		"max_level = 3\r\n"
		"province = yes\r\n"
		"infrastructure = 0.5\r\n"
		"movement_cost = -0.5\r\n"
		"pop_build_factory = yes\r\n"
		"spawn_railway_track = yes\r\n"
		"}");
	directory_representation test5 = directory_representation(u"test5", f_root);
	directory_representation common5 = directory_representation(u"common", test5);
	directory_representation poptypes = directory_representation(u"poptypes", test5);
	file_representation p1 = file_representation(u"capitalists.txt", poptypes, "");
	file_representation p2 = file_representation(u"craftsmen.txt", poptypes, "");
	file_representation p3 = file_representation(u"clerks.txt", poptypes, "");
	file_representation p4 = file_representation(u"aristocrats.txt", poptypes, "");
	file_representation p5 = file_representation(u"labourers.txt", poptypes, "");
	file_representation p6 = file_representation(u"slaves.txt", poptypes, "");
	file_representation p7 = file_representation(u"farmers.txt", poptypes, "");
	file_representation p8 = file_representation(u"artisan.txt", poptypes, "");
	file_representation goods5 = file_representation(u"goods.txt", common5,
		"military_goods   = {\r\n"
		"money_good = {\r\n"
		"cost = 8\r\n"
		"color = { 0 0 0 }\r\n"
		"overseas_penalty  = yes\r\n"
		"tradeable = no\r\n"
		"money = yes\r\n"
		"}\r\n"
		"}\r\n"
		"other_goods = {\r\n"
		"cement = { \r\n"
		"	cost = 16.0\r\n"
		"	color = { 185 187 164 }\r\n"
		"}\r\n"
		"machine_parts = { \r\n"
		"	cost = 16.0\r\n"
		"	color = { 185 187 164 }\r\n"
		"}\r\n"
		"coal = { \r\n"
		"	cost = 16.0\r\n"
		"	color = { 185 187 164 }\r\n"
		"}\r\n"
		"glass = { \r\n"
		"	cost = 2.9\r\n"
		"	color = { 201 200 199 }\r\n"
		"}\r\n"
		"}");
	file_representation buildings5 = file_representation(u"buildings.txt", common5,
		"aeroplane_factory = {\r\n"
		"type = factory\r\n"
		"production_type = aeroplane_factory\r\n"
		"}\r\n"
		"fac_2 = {\r\n"
		"type = factory\r\n"
		"production_type = electric_gear_factory\r\n"
		"}\r\n"
	);
	file_representation production_types = file_representation(u"production_types.txt", common5,
		"factory_template = {\r\n"
		"	efficiency = {\r\n"
		"		cement = 0.5 #if no cement, work at 75 % efficiency #cost = 16.0\r\n"
		"		machine_parts = 0.05\r\n"
		"	}\r\n"
		"	owner = {\r\n"
		"		poptype = capitalists\r\n"
		"		effect = input\r\n"
		"		effect_multiplier = -2.5\r\n"
		"	}\r\n"
		"	employees = {\r\n"
		"		{\r\n"
		"		poptype = craftsmen\r\n"
		"		effect = throughput\r\n"
		"		amount = 0.8\r\n"
		"		}\r\n"
		"		{\r\n"
		"		poptype = clerks\r\n"
		"		effect = output\r\n"
		"		effect_multiplier = 1.5\r\n"
		"		amount = 0.2\r\n"
		"		}\r\n"
		"	}\r\n"
		"	type = factory\r\n"
		"	workforce = 10000\r\n"
		"}\r\n"
		"aeroplane_factory = {\r\n"
		"	template = factory_template\r\n"
		"	input_goods = {\r\n"
		"		machine_parts = 1	#38.69\r\n"
		"	}\r\n"
		"	output_goods = machine_parts\r\n"
		"	value = 0.91			#100\r\n"
		"	is_coastal = yes\r\n"
		"	bonus = {\r\n"
		"		trigger = { has_building = fac_2 }\r\n"
		"		value = 0.25\r\n"
		"	}\r\n"
		"}\r\n"
		"electric_gear_factory = {\r\n"
		"	template = factory_template\r\n"
		"	input_goods = {\r\n"
		"		coal = 5		#11.5\r\n"
		"	}\r\n"
		"	output_goods = glass\r\n"
		"		value = 5			#80\r\n"
		"	bonus = {\r\n"
		"		trigger = { trade_goods_in_state = cement }\r\n"
		"		value = 0.15\r\n"
		"	}\r\n"
		"	bonus = {\r\n"
		"		trigger = { trade_goods_in_state = glass }\r\n"
		"		value = 1\r\n"
		"	}\r\n"
		"}\r\n"
		"RGO_template_farmers = {\r\n"
		"	owner = {\r\n"
		"		poptype = aristocrats\r\n"
		"		effect = output\r\n"
		"	}\r\n"
		"	employees = {\r\n"
		"		{\r\n"
		"		poptype = farmers\r\n"
		"		effect = throughput\r\n"
		"		amount = 1\r\n"
		"		}\r\n"
		"		{\r\n"
		"		poptype = slaves\r\n"
		"		effect = output\r\n"
		"		amount = 1\r\n"
		"		}\r\n"
		"	}\r\n"
		"	type = rgo\r\n"
		"	workforce = 40000\r\n"
		"}\r\n"
		"RGO_template_labourers = {\r\n"
		"	owner = {\r\n"
		"		poptype = aristocrats\r\n"
		"		effect = output\r\n"
		"	}\r\n"
		"	employees = {\r\n"
		"		{\r\n"
		"		poptype = labourers\r\n"
		"		effect = throughput\r\n"
		"		amount = 1\r\n"
		"		}\r\n"
		"		{\r\n"
		"		poptype = slaves\r\n"
		"		effect = output\r\n"
		"		amount = 1\r\n"
		"		}\r\n"
		"	}\r\n"
		"	type = rgo\r\n"
		"	workforce = 70000\r\n"
		"}\r\n"
		"cattle_ranch = {\r\n"
		"	template = RGO_template_farmers\r\n"
		"	output_goods = cement\r\n"
		"	value = 1.5\r\n"
		"	farm = yes\r\n"
		"}\r\n"
		"coal_mine = {\r\n"
		"	template = RGO_template_labourers\r\n"
		"	output_goods = coal\r\n"
		"	value = 2.0\r\n"
		"	mine = yes\r\n"
		"}\r\n"
		"artisan_steel = {\r\n"
		"	input_goods = {\r\n"
		"		glass = 20		#70\r\n"
		"		coal = 5		#11.5	total input + maint = 81.5\r\n"
		"	}\r\n"
		"	output_goods = machine_parts 		#94\r\n"
		"	value = 20\r\n"
		"	owner = {\r\n"
		"		poptype = artisan\r\n"
		"		effect = output\r\n"
		"	}\r\n"
		"	type = artisan\r\n"
		"	workforce = 50000\r\n"
		"}\r\n"
		);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

TEST(economy_tests, single_good) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	economic_scenario m;
	text_data::text_sequences tex;
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), tex);

	EXPECT_EQ(1ui64, m.goods.size());
	EXPECT_EQ(1ui64, m.good_type_names.size());
	EXPECT_EQ(1ui64, m.named_goods_index.size());

	EXPECT_EQ(goods_tag(), m.money);
	EXPECT_EQ(17.5, m.goods[goods_tag(0)].base_price);
	EXPECT_EQ(goods_tag(0), m.goods[goods_tag(0)].id);
	EXPECT_EQ(208ui8, m.goods[goods_tag(0)].color.r);
	EXPECT_EQ(202ui8, m.goods[goods_tag(0)].color.g);
	EXPECT_EQ(127ui8, m.goods[goods_tag(0)].color.b);
	EXPECT_EQ(good_definition::not_available_from_start, m.goods[goods_tag(0)].flags);
	EXPECT_EQ(goods_type_tag(0), m.goods[goods_tag(0)].type);
	EXPECT_EQ(goods_tag(0), m.named_goods_index[m.goods[goods_tag(0)].name]);
}

TEST(economy_tests, two_goods) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	economic_scenario m;
	text_data::text_sequences tex;
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), tex);

	EXPECT_EQ(2ui64, m.goods.size());
	EXPECT_EQ(1ui64, m.good_type_names.size());
	EXPECT_EQ(2ui64, m.named_goods_index.size());

	EXPECT_EQ(goods_tag(1), m.money);
	EXPECT_EQ(17.5, m.goods[goods_tag(0)].base_price);
	EXPECT_EQ(goods_tag(0), m.goods[goods_tag(0)].id);
	EXPECT_EQ(208ui8, m.goods[goods_tag(0)].color.r);
	EXPECT_EQ(202ui8, m.goods[goods_tag(0)].color.g);
	EXPECT_EQ(127ui8, m.goods[goods_tag(0)].color.b);
	EXPECT_EQ(good_definition::not_available_from_start, m.goods[goods_tag(0)].flags);
	EXPECT_EQ(goods_type_tag(0), m.goods[goods_tag(0)].type);
	EXPECT_EQ(goods_tag(0), m.named_goods_index[m.goods[goods_tag(0)].name]);

	EXPECT_EQ(8.0, m.goods[goods_tag(1)].base_price);
	EXPECT_EQ(goods_tag(1), m.goods[goods_tag(1)].id);
	EXPECT_EQ(0ui8, m.goods[goods_tag(1)].color.r);
	EXPECT_EQ(0ui8, m.goods[goods_tag(1)].color.g);
	EXPECT_EQ(0ui8, m.goods[goods_tag(1)].color.b);
	EXPECT_EQ(good_definition::money | good_definition::overseas_penalty, m.goods[goods_tag(1)].flags);
	EXPECT_EQ(goods_type_tag(0), m.goods[goods_tag(1)].type);
	EXPECT_EQ(goods_tag(1), m.named_goods_index[m.goods[goods_tag(1)].name]);
}

TEST(economy_tests, two_good_categories) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test3"));

	economic_scenario m;
	text_data::text_sequences tex;
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), tex);

	EXPECT_EQ(3ui64, m.goods.size());
	EXPECT_EQ(2ui64, m.good_type_names.size());
	EXPECT_EQ(3ui64, m.named_goods_index.size());

	EXPECT_EQ(goods_tag(2), m.money);

	EXPECT_EQ(16.0, m.goods[goods_tag(0)].base_price);
	EXPECT_EQ(goods_tag(0), m.goods[goods_tag(0)].id);
	EXPECT_EQ(185ui8, m.goods[goods_tag(0)].color.r);
	EXPECT_EQ(187ui8, m.goods[goods_tag(0)].color.g);
	EXPECT_EQ(164ui8, m.goods[goods_tag(0)].color.b);
	EXPECT_EQ(0ui8, m.goods[goods_tag(0)].flags);
	EXPECT_EQ(goods_type_tag(1), m.goods[goods_tag(0)].type);
	EXPECT_EQ(goods_tag(0), m.named_goods_index[m.goods[goods_tag(0)].name]);

	EXPECT_DOUBLE_EQ(2.9, m.goods[goods_tag(1)].base_price);
	EXPECT_EQ(goods_tag(1), m.goods[goods_tag(1)].id);
	EXPECT_EQ(201ui8, m.goods[goods_tag(1)].color.r);
	EXPECT_EQ(200ui8, m.goods[goods_tag(1)].color.g);
	EXPECT_EQ(199ui8, m.goods[goods_tag(1)].color.b);
	EXPECT_EQ(0ui8, m.goods[goods_tag(1)].flags);
	EXPECT_EQ(goods_type_tag(1), m.goods[goods_tag(1)].type);
	EXPECT_EQ(goods_tag(1), m.named_goods_index[m.goods[goods_tag(1)].name]);

	EXPECT_EQ(8.0, m.goods[goods_tag(2)].base_price);
	EXPECT_EQ(goods_tag(2), m.goods[goods_tag(2)].id);
	EXPECT_EQ(0ui8, m.goods[goods_tag(2)].color.r);
	EXPECT_EQ(0ui8, m.goods[goods_tag(2)].color.g);
	EXPECT_EQ(0ui8, m.goods[goods_tag(2)].color.b);
	EXPECT_EQ(good_definition::money | good_definition::overseas_penalty, m.goods[goods_tag(2)].flags);
	EXPECT_EQ(goods_type_tag(0), m.goods[goods_tag(2)].type);
	EXPECT_EQ(goods_tag(2), m.named_goods_index[m.goods[goods_tag(2)].name]);
}

TEST(economy_tests, single_factory) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test3"));


	const char pt[] = "aeroplane_factory";

	economic_scenario m;
	text_data::text_sequences tex;

	economy::read_goods(m, f.get_root(), tex);
	const auto map = economy::read_buildings(m, f.get_root(), tex);

	EXPECT_EQ(1ui64, m.factory_types.size());
	EXPECT_EQ(1ui64, map.size());

	const auto& fa = m.factory_types[factory_type_tag(0)];
	EXPECT_EQ(14ui32, fa.building_time);
	EXPECT_EQ(false, fa.coastal);
	EXPECT_EQ(false, fa.default_enabled);
	EXPECT_EQ(factory_type_tag(0), fa.id);
	EXPECT_EQ(factory_type_tag(0), m.named_factory_types_index[fa.name]);
	EXPECT_EQ(600.0, m.building_costs.get(factory_type_tag(0), goods_tag(0)));
	const auto pt_tag = text_data::get_thread_safe_text_handle(tex, RANGE(pt));
	EXPECT_EQ(factory_type_tag(0), map.find(pt_tag)->second);
}

TEST(economy_tests, special_buildings) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test4"));

	economic_scenario m;
	text_data::text_sequences tex;

	economy::read_goods(m, f.get_root(), tex);
	const auto map = economy::read_buildings(m, f.get_root(), tex);

	EXPECT_EQ(1ui64, m.factory_types.size());
	EXPECT_EQ(1ui64, map.size());

	const auto& fa = m.factory_types[factory_type_tag(0)];
	EXPECT_EQ(14ui32, fa.building_time);
	EXPECT_EQ(false, fa.coastal);
	EXPECT_EQ(false, fa.default_enabled);
	EXPECT_EQ(factory_type_tag(0), fa.id);
	EXPECT_EQ(factory_type_tag(0), m.named_factory_types_index[fa.name]);
	EXPECT_EQ(600.0, m.building_costs.get(factory_type_tag(0), goods_tag(0)));

	EXPECT_EQ(factory_type_tag(1), m.fort.cost_tag);
	EXPECT_EQ(1ui32, m.fort.max_level);
	EXPECT_EQ(10ui32, m.fort.time);

	EXPECT_EQ(100.0, m.building_costs.get(factory_type_tag(1), goods_tag(0)));

	EXPECT_EQ(factory_type_tag(2), m.railroad.cost_tag);
	EXPECT_EQ(3ui32, m.railroad.max_level);
	EXPECT_EQ(30ui32, m.railroad.time);
	EXPECT_EQ(0.5f, m.railroad.infrastructure);
	EXPECT_EQ(-0.5f, m.railroad.movement_cost);

	EXPECT_EQ(300.0, m.building_costs.get(factory_type_tag(2), goods_tag(0)));

	EXPECT_EQ(factory_type_tag(3), m.naval_base.cost_tag);
	EXPECT_EQ(2ui32, m.naval_base.max_level);
	EXPECT_EQ(20ui32, m.naval_base.time);
	EXPECT_EQ(1ui32, m.naval_base.colonial_points[0]);
	EXPECT_EQ(6ui32, m.naval_base.colonial_points[5]);
	EXPECT_EQ(50ui32, m.naval_base.colonial_range);
	EXPECT_EQ(15000ui32, m.naval_base.extra_cost);
	EXPECT_EQ(-1.5f, m.naval_base.local_ship_build);
	EXPECT_EQ(2ui32, m.naval_base.naval_capacity);

	EXPECT_EQ(200.0, m.building_costs.get(factory_type_tag(3), goods_tag(0)));
}

TEST(economy_tests, production_types) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test5"));

	scenario::scenario_manager m;

	economy::read_goods(m.economy_m, f.get_root(), m.gui_m.text_data_sequences);
	auto map = economy::read_buildings(m.economy_m, f.get_root(), m.gui_m.text_data_sequences);
	population::pre_parse_pop_types(m.population_m, f.get_root(), m.gui_m.text_data_sequences);

	read_production_types(m, map, f.get_root());

	EXPECT_EQ(2ui64, m.economy_m.factory_types.size());
	EXPECT_EQ(1ui64, m.economy_m.artisan_types.size());

	const auto cement_tag = tag_from_text(m.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("cement")));
	const auto glass_tag = tag_from_text(m.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("glass")));
	const auto coal_tag = tag_from_text(m.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("coal")));
	const auto machine_parts_tag = tag_from_text(m.economy_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("machine_parts")));

	const auto craftsmen_tag = tag_from_text(m.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("craftsmen")));
	const auto clerks_tag = tag_from_text(m.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("clerks")));
	const auto capitalists_tag = tag_from_text(m.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("capitalists")));
	const auto slaves_tag = tag_from_text(m.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("slaves")));

	const auto aristocrats_tag = tag_from_text(m.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("aristocrats")));
	const auto farmers_tag = tag_from_text(m.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("farmers")));
	const auto labourers_tag = tag_from_text(m.population_m.named_pop_type_index, text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("labourers")));


	EXPECT_EQ(0.5, m.economy_m.factory_efficiency_goods.get(factory_type_tag(0), cement_tag));
	EXPECT_EQ(0.5, m.economy_m.factory_efficiency_goods.get(factory_type_tag(1), cement_tag));
	EXPECT_EQ(0.0, m.economy_m.factory_efficiency_goods.get(factory_type_tag(0), coal_tag));
	EXPECT_EQ(0.0, m.economy_m.factory_efficiency_goods.get(factory_type_tag(1), coal_tag));
	EXPECT_EQ(1.0, m.economy_m.factory_input_goods.get(factory_type_tag(0), machine_parts_tag));
	EXPECT_EQ(5.0, m.economy_m.factory_input_goods.get(factory_type_tag(1), coal_tag));
	EXPECT_EQ(0.0, m.economy_m.factory_input_goods.get(factory_type_tag(0), glass_tag));
	EXPECT_EQ(0.0, m.economy_m.factory_input_goods.get(factory_type_tag(1), glass_tag));

	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("aeroplane_factory")), m.economy_m.factory_types[factory_type_tag(0)].name);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(m.gui_m.text_data_sequences, RANGE("fac_2")), m.economy_m.factory_types[factory_type_tag(1)].name);

	EXPECT_EQ(true, m.economy_m.factory_types[factory_type_tag(0)].coastal);
	EXPECT_EQ(false , m.economy_m.factory_types[factory_type_tag(1)].coastal);

	EXPECT_EQ(factory_type_tag(0), m.economy_m.factory_types[factory_type_tag(0)].id);
	EXPECT_EQ(factory_type_tag(1), m.economy_m.factory_types[factory_type_tag(1)].id);

	EXPECT_EQ(0.91, m.economy_m.factory_types[factory_type_tag(0)].output_amount);
	EXPECT_EQ(5.0, m.economy_m.factory_types[factory_type_tag(1)].output_amount);

	EXPECT_EQ(machine_parts_tag, m.economy_m.factory_types[factory_type_tag(0)].output_good);
	EXPECT_EQ(glass_tag, m.economy_m.factory_types[factory_type_tag(1)].output_good);

	EXPECT_EQ(10000ui32, m.economy_m.factory_types[factory_type_tag(0)].workforce);
	EXPECT_EQ(10000ui32, m.economy_m.factory_types[factory_type_tag(1)].workforce);

	EXPECT_EQ(contribution_type::input, m.economy_m.factory_types[factory_type_tag(0)].owner.contribution);
	EXPECT_EQ(contribution_type::input, m.economy_m.factory_types[factory_type_tag(1)].owner.contribution);

	EXPECT_EQ(-2.5f, m.economy_m.factory_types[factory_type_tag(0)].owner.effect_multiplier);
	EXPECT_EQ(-2.5f, m.economy_m.factory_types[factory_type_tag(1)].owner.effect_multiplier);

	EXPECT_EQ(capitalists_tag, m.economy_m.factory_types[factory_type_tag(0)].owner.type);
	EXPECT_EQ(capitalists_tag, m.economy_m.factory_types[factory_type_tag(1)].owner.type);

	EXPECT_EQ(contribution_type::throughput, m.economy_m.factory_types[factory_type_tag(0)].workers[0].contribution);
	EXPECT_EQ(contribution_type::throughput, m.economy_m.factory_types[factory_type_tag(1)].workers[0].contribution);
	EXPECT_EQ(contribution_type::output, m.economy_m.factory_types[factory_type_tag(0)].workers[1].contribution);
	EXPECT_EQ(contribution_type::output, m.economy_m.factory_types[factory_type_tag(1)].workers[1].contribution);

	EXPECT_EQ(1.5f, m.economy_m.factory_types[factory_type_tag(0)].workers[1].effect_multiplier);
	EXPECT_EQ(1.5f, m.economy_m.factory_types[factory_type_tag(1)].workers[1].effect_multiplier);

	EXPECT_EQ(0.8f, m.economy_m.factory_types[factory_type_tag(0)].workers[0].amount);
	EXPECT_EQ(0.8f, m.economy_m.factory_types[factory_type_tag(1)].workers[0].amount);
	EXPECT_EQ(0.2f, m.economy_m.factory_types[factory_type_tag(0)].workers[1].amount);
	EXPECT_EQ(0.2f, m.economy_m.factory_types[factory_type_tag(1)].workers[1].amount);

	EXPECT_EQ(craftsmen_tag, m.economy_m.factory_types[factory_type_tag(0)].workers[0].type);
	EXPECT_EQ(craftsmen_tag, m.economy_m.factory_types[factory_type_tag(1)].workers[0].type);
	EXPECT_EQ(clerks_tag, m.economy_m.factory_types[factory_type_tag(0)].workers[1].type);
	EXPECT_EQ(clerks_tag, m.economy_m.factory_types[factory_type_tag(1)].workers[1].type);
	EXPECT_EQ(population::pop_type_tag(), m.economy_m.factory_types[factory_type_tag(0)].workers[2].type);
	EXPECT_EQ(population::pop_type_tag(), m.economy_m.factory_types[factory_type_tag(1)].workers[2].type);

	EXPECT_EQ(0.25f, m.economy_m.factory_types[factory_type_tag(0)].bonuses[0].value);
	EXPECT_NE(triggers::trigger_tag(), m.economy_m.factory_types[factory_type_tag(0)].bonuses[0].condition);
	EXPECT_EQ(triggers::trigger_tag(), m.economy_m.factory_types[factory_type_tag(0)].bonuses[1].condition);
	EXPECT_EQ(0.15f, m.economy_m.factory_types[factory_type_tag(1)].bonuses[0].value);
	EXPECT_NE(triggers::trigger_tag(), m.economy_m.factory_types[factory_type_tag(1)].bonuses[0].condition);
	EXPECT_EQ(1.0f, m.economy_m.factory_types[factory_type_tag(1)].bonuses[1].value);
	EXPECT_NE(triggers::trigger_tag(), m.economy_m.factory_types[factory_type_tag(1)].bonuses[1].condition);

	EXPECT_EQ(farmers_tag, m.economy_m.rgo_farm.workers[0].type);
	EXPECT_EQ(slaves_tag, m.economy_m.rgo_farm.workers[1].type);
	EXPECT_EQ(contribution_type::throughput, m.economy_m.rgo_farm.workers[0].contribution);
	EXPECT_EQ(contribution_type::output, m.economy_m.rgo_farm.workers[1].contribution);
	EXPECT_EQ(1.0f, m.economy_m.rgo_farm.workers[0].amount);
	EXPECT_EQ(1.0f, m.economy_m.rgo_farm.workers[1].amount);
	EXPECT_EQ(40000ui32, m.economy_m.rgo_farm.workforce);
	EXPECT_EQ(aristocrats_tag, m.economy_m.rgo_farm.owner.type);
	EXPECT_EQ(contribution_type::output, m.economy_m.rgo_farm.owner.contribution);

	EXPECT_EQ(labourers_tag, m.economy_m.rgo_mine.workers[0].type);
	EXPECT_EQ(slaves_tag, m.economy_m.rgo_mine.workers[1].type);
	EXPECT_EQ(contribution_type::throughput, m.economy_m.rgo_mine.workers[0].contribution);
	EXPECT_EQ(contribution_type::output, m.economy_m.rgo_mine.workers[1].contribution);
	EXPECT_EQ(1.0f, m.economy_m.rgo_mine.workers[0].amount);
	EXPECT_EQ(1.0f, m.economy_m.rgo_mine.workers[1].amount);
	EXPECT_EQ(70000ui32, m.economy_m.rgo_mine.workforce);
	EXPECT_EQ(aristocrats_tag, m.economy_m.rgo_mine.owner.type);
	EXPECT_EQ(contribution_type::output, m.economy_m.rgo_mine.owner.contribution);

	EXPECT_EQ(1.5, m.economy_m.goods[cement_tag].base_rgo_value);
	EXPECT_EQ(0, m.economy_m.goods[cement_tag].flags & good_definition::mined);

	EXPECT_EQ(2.0, m.economy_m.goods[coal_tag].base_rgo_value);
	EXPECT_NE(0, m.economy_m.goods[coal_tag].flags & good_definition::mined);

	EXPECT_EQ(20.0, m.economy_m.artisan_input_goods.get(artisan_type_tag(0), glass_tag));
	EXPECT_EQ(5.0, m.economy_m.artisan_input_goods.get(artisan_type_tag(0), coal_tag));

	EXPECT_EQ(artisan_type_tag(0), m.economy_m.artisan_types[artisan_type_tag(0)].id);
	EXPECT_EQ(contribution_type::output, m.economy_m.artisan_types[artisan_type_tag(0)].artisan_contribution);
	EXPECT_EQ(false, m.economy_m.artisan_types[artisan_type_tag(0)].coastal);
	EXPECT_EQ(20.0, m.economy_m.artisan_types[artisan_type_tag(0)].output_amount);
	EXPECT_EQ(machine_parts_tag, m.economy_m.artisan_types[artisan_type_tag(0)].output_good);
	EXPECT_EQ(50000ui32, m.economy_m.artisan_types[artisan_type_tag(0)].workforce);
}
