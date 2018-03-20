#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "economy\\economy.h"

using namespace economy;

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

inline auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return text_data::text_tag(i++); };
}

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
	file_representation goods3 = file_representation(u"goods.txt", common3,
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
		"glass = { \r\n"
		"	cost = 2.9\r\n"
		"	color = { 201 200 199 }\r\n"
		"}\r\n"
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

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup());

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

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup());

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

	economy::read_goods(m, f.get_root(), fake_text_handle_lookup());

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
