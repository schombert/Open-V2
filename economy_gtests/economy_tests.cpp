#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "economy\\economy.h"

using namespace economy;

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

inline auto fake_text_handle_lookup(std::map<std::string, uint16_t>& values) {
	return[j = 0ui16, &values](const char* s, const char* e) mutable {
		const auto i = std::string(s, e);
		if (values.find(i) == values.end()) {
			values[i] = j++;
		} 
		return text_data::text_tag(values[i]);
	};
}


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

	preparse_test_files() {
		set_default_root(f_root);
	}
};

TEST(economy_tests, single_good) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	economic_scenario m;
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup(v));

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
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup(v));

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
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup(v));

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
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup(v));
	const auto map = economy::read_buildings(m, f.get_root(), fake_text_handle_lookup(v));

	EXPECT_EQ(1ui64, m.factory_types.size());
	EXPECT_EQ(1ui64, map.size());

	const auto& fa = m.factory_types[factory_type_tag(0)];
	EXPECT_EQ(14ui32, fa.building_time);
	EXPECT_EQ(false, fa.coastal);
	EXPECT_EQ(false, fa.default_enabled);
	EXPECT_EQ(factory_type_tag(0), fa.id);
	EXPECT_EQ(factory_type_tag(0), m.named_factory_types_index[fa.name]);
	EXPECT_EQ(600.0, m.building_costs.get(factory_type_tag(0), goods_tag(0)));
	const auto pt_tag = fake_text_handle_lookup(v)(RANGE(pt));
	EXPECT_EQ(factory_type_tag(0), map.find(pt_tag)->second);
}

TEST(economy_tests, special_buildings) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test4"));

	economic_scenario m;
	std::map<std::string, uint16_t> v;

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup(v));
	const auto map = economy::read_buildings(m, f.get_root(), fake_text_handle_lookup(v));

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
