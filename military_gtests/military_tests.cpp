#include "military\\military.h"
#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "economy\\economy.h"
#include "sound\\sound.h"
#include "events\\events.h"
#include "events\\events_io.h"
#include "scenario\\scenario.h"
#include "military\\military_io.h"
#include "economy\\economy_io.h"
#include "sound\\sound_io.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation test1 = directory_representation(u"test1", f_root);
	directory_representation common = directory_representation(u"common", test1);
	file_representation cb = file_representation(u"cb_types.txt", common,
		"peace_order = {\r\n"
		"cb_a\r\n"
		"liberate_country\r\n"
		"}\r\n"
		"liberate_country = {\r\n"
		"	sprite_index = 15\r\n"
		"	is_triggered_only = yes\r\n"
		"	months = 12\r\n"
		"	badboy_factor = 2\r\n"
		"	prestige_factor = 3\r\n"
		"	peace_cost_factor = 4\r\n"
		"	penalty_factor = 5\r\n"
		"	break_truce_prestige_factor = 6\r\n"
		"	break_truce_infamy_factor = 7\r\n"
		"	break_truce_militancy_factor = 8\r\n"
		"	truce_months = 48\r\n"
		"	good_relation_prestige_factor = 9\r\n"
		"	good_relation_infamy_factor = 10\r\n"
		"	good_relation_militancy_factor = 11\r\n"
		"	can_use = {\r\n"
		"		NOT = {\r\n"
		"			is_our_vassal = THIS\r\n"
		"			has_country_modifier = neutrality\r\n"
		"		}\r\n"
		"		is_vassal = no\r\n"
		"		OR = {\r\n"
		"			war_with = THIS\r\n"
		"			FROM = { involved_in_crisis = yes }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	all_allowed_states = yes\r\n"
		"	po_transfer_provinces = yes\r\n"
		"	war_name = WAR_LIBERATE_NAME\r\n"
		"	on_add = {\r\n"
		"		country_event =	2651\r\n"
		"	}\r\n"
		"}\r\n"
		"cb_a = {\r\n"
		"	always = yes\r\n"
		"	is_valid = {\r\n"
		"		this = { is_greater_power = yes }\r\n"
		"		is_greater_power = no\r\n"
		"	}\r\n"
		"	allowed_countries = {\r\n"
		"		is_sphere_leader_of = FROM\r\n"
		"		OR = {\r\n"
		"			FROM = { is_vassal = no }\r\n"
		"			THIS = { is_our_vassal = FROM }\r\n"
		"		}\r\n"
		"	}\r\n"
		"	allowed_substate_regions = {\r\n"
		"		is_colonial = no\r\n"
		"	}\r\n"
		"	po_disarmament = yes\r\n"
		"	po_reparations = yes\r\n"
		"}\r\n"
		"cb_c = {\r\n"
		"	crisis = no\r\n"
		"	construction_speed = 0.5\r\n"
		"	tws_battle_factor = 2.0\r\n"
		"	allowed_states = {\r\n"
		"		any_owned_province = {\r\n"
		"			is_core = THIS\r\n"
		"		}\r\n"
		"		not = { any_owned_province = { is_capital = yes } }\r\n"
		"	}\r\n"
		"}\r\n");
	directory_representation units = directory_representation(u"units", f_root);
	file_representation f1 = file_representation(u"dragoon.txt", units, "dragoon = { stuff }");
	file_representation f2 = file_representation(u"airplane.txt", units, "plane_a = { stuff }\r\nplane_b = {}");

	directory_representation test2 = directory_representation(u"test2", f_root);
	directory_representation common2 = directory_representation(u"common", test2);
	file_representation traits2 = file_representation(u"traits.txt", common2, "personality = { no_personality = { morale = 0.5 } test_personality = { defence = 1.0 }}");

	directory_representation test3 = directory_representation(u"test3", f_root);
	directory_representation common3 = directory_representation(u"common", test3);
	file_representation traits3 = file_representation(u"traits.txt", common3,
		"personality = { no_personality = { morale = 0.5 } test_personality = { defence = 1.0 }} \r\n"
		"background = { no_background = {} b1 = { reliability = 2.0 speed = 4.0 } b2 = {}}"
	);

	directory_representation test4 = directory_representation(u"test4", f_root);
	directory_representation common4 = directory_representation(u"common", test4);
	directory_representation interface4 = directory_representation(u"interface", test4);
	file_representation goods4 = file_representation(u"goods.txt", common4,
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
	file_representation sfx = file_representation(u"sound.sfx", interface4,
		"army_move = { file = \"GI_InfantryMove.wav\" }\r\n"
		"click = {\r\n"
		"	file = \"GI_ValidClick.wav\"\r\n"
		"}\r\n"
		"new_fort = {\r\n"
		"	file = Misc_Fortification.wav\r\n"
		"	volume = 0.5\r\n"
		"}");
	directory_representation sound4 = directory_representation(u"sound", test4);
	file_representation s1 = file_representation(u"GI_ValidClick.wav", sound4, "");
	file_representation s2 = file_representation(u"Misc_Fortification.wav", sound4, "");
	file_representation s3 = file_representation(u"GI_InfantryMove.wav", sound4, "");

	directory_representation units4 = directory_representation(u"units", test4);
	file_representation f41 = file_representation(u"dragoon.txt", units4,
		"guard = {\r\n"
		"	icon = 18\r\n"
		"	type = land\r\n"
		"	sprite = Guard\r\n"
		"	active = no\r\n"
		"	unit_type = infantry\r\n"
		"	primary_culture = yes\r\n"
		"	floating_flag = no\r\n"
		"	priority = 18\r\n"
		"	max_strength = 3\r\n"
		"	default_organisation = 30\r\n"
		"	maximum_speed = 4.00\r\n"
		"	weighted_value = 9.0\r\n"
		"	build_time = 90\r\n"
		"	build_cost = {\r\n"
		"		cement = 5\r\n"
		"		canned_food = 10\r\n"
		"	}\r\n"
		"	supply_consumption = 1.0\r\n"
		"	supply_cost = {\r\n"
		"		canned_food = 0.25\r\n"
		"	}\r\n"
		"	reconnaissance = 0\r\n"
		"	attack = 8\r\n"
		"	defence = 2\r\n"
		"	discipline = 1.0\r\n"
		"	support = 0\r\n"
		"	maneuver = 2\r\n"
		"}\r\n"
	);
	file_representation f42 = file_representation(u"airplane.txt", units4,
		"cruiser = {\r\n"
		"	icon = 11\r\n"
		"	naval_icon = 8\r\n"
		"	type = naval\r\n"
		"	sail = yes\r\n"
		"	sprite = Cruiser\r\n"
		"	unit_type = light_ship\r\n"
		"	move_sound = army_move\r\n"
		"	select_sound = click\r\n"
		"	colonial_points = 16\r\n"
		"	priority = 12\r\n"
		"	max_strength = 100\r\n"
		"	default_organisation = 30\r\n"
		"	maximum_speed = 12\r\n"
		"	weighted_value = 8.0\r\n"
		"	floating_flag = yes\r\n"
		"	can_build_overseas = no\r\n"
		"	build_time = 240\r\n"
		"	min_port_level = 4\r\n"
		"	limit_per_port = 1\r\n"
		"	supply_consumption_score = 20\r\n"
		"	supply_consumption = 1.0\r\n"
		"	hull = 50\r\n"
		"	gun_power = 30\r\n"
		"	fire_range = 7\r\n"
		"	evasion = 6\r\n"
		"	torpedo_attack = 2\r\n"
		"}\r\n"
		"plane = {\r\n"
		"	icon = 19\r\n"
		"	type = land\r\n"
		"	sprite = Plane\r\n"
		"	active = no\r\n"
		"	unit_type = cavalry\r\n"
		"	floating_flag = no\r\n"
		"	priority = 45\r\n"
		"	max_strength = 3\r\n"
		"	default_organisation = 30\r\n"
		"	maximum_speed = 5.00\r\n"
		"	weighted_value = 10.0\r\n"
		"	build_time = 120\r\n"
		"	supply_consumption = 1.0\r\n"
		"	supply_cost = {\r\n"
		"		canned_food = 9\r\n"
		"	}\r\n"
		"	reconnaissance = 4\r\n"
		"	attack = 1\r\n"
		"	defence = 10\r\n"
		"	discipline = 3\r\n"
		"	support = 2\r\n"
		"	maneuver = 2\r\n"
		"}\r\n"
	);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace military;

TEST(military_tests, test_units_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	military_manager m;
	text_data::text_sequences tex;

	parsing_state state(tex, m);
	
	pre_parse_unit_types(state, f.get_root());

	EXPECT_EQ(5ui64, m.unit_types.size());
	EXPECT_EQ(5ui64, m.named_unit_type_index.size());

	EXPECT_EQ(unit_type_tag(2), m.unit_types[unit_type_tag(2)].id);
	EXPECT_EQ(unit_type_tag(3), m.unit_types[unit_type_tag(3)].id);
	EXPECT_EQ(unit_type_tag(4), m.unit_types[unit_type_tag(4)].id);
	EXPECT_EQ(unit_type_tag(2), m.named_unit_type_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("dragoon"))]);
	EXPECT_EQ(unit_type_tag(3), m.named_unit_type_index[m.unit_types[unit_type_tag(3)].name]);
	EXPECT_EQ(unit_type_tag(4), m.named_unit_type_index[m.unit_types[unit_type_tag(4)].name]);
}

TEST(military_tests, test_cb_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	military_manager m;
	text_data::text_sequences tex;

	parsing_state state(tex, m);

	pre_parse_cb_types(state, f.get_root());

	EXPECT_EQ(3ui64, m.cb_types.size());
	EXPECT_EQ(3ui64, m.named_cb_type_index.size());

	EXPECT_EQ(cb_type_tag(0), m.cb_types[cb_type_tag(0)].id);
	EXPECT_EQ(cb_type_tag(1), m.cb_types[cb_type_tag(1)].id);
	EXPECT_EQ(cb_type_tag(2), m.cb_types[cb_type_tag(2)].id);
	EXPECT_EQ(cb_type_tag(0), m.named_cb_type_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("cb_a"))]);
	EXPECT_EQ(cb_type_tag(1), m.named_cb_type_index[m.cb_types[cb_type_tag(1)].name]);
	EXPECT_EQ(cb_type_tag(2), m.named_cb_type_index[m.cb_types[cb_type_tag(2)].name]);
}

TEST(military_tests, traits_personality) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test2"));

	military_manager m;
	text_data::text_sequences tex;
	parsing_state state(tex, m);

	read_leader_traits(state, f.get_root());

	EXPECT_EQ(3ui64, m.leader_traits.size());
	EXPECT_EQ(1ui64, m.personality_traits.size());
	EXPECT_EQ(0ui64, m.background_traits.size());

	EXPECT_EQ(m.no_personality_trait, m.named_leader_trait_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("no_personality"))]);
	EXPECT_EQ(leader_trait_tag(2), m.named_leader_trait_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("test_personality"))]);
	EXPECT_EQ(leader_trait_tag(2), m.personality_traits[0]);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(tex, RANGE("no_personality")), m.leader_traits[m.no_personality_trait]);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(tex, RANGE("test_personality")), m.leader_traits[leader_trait_tag(2)]);

	EXPECT_EQ(0.5f, m.leader_trait_definitions.get(m.no_personality_trait, traits::morale));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(m.no_personality_trait, traits::organisation));
	EXPECT_EQ(1.0f, m.leader_trait_definitions.get(leader_trait_tag(2), traits::defence));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(2), traits::reliability));
}

TEST(military_tests, traits_mixed) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test3"));

	military_manager m;
	text_data::text_sequences tex;

	parsing_state state(tex, m);

	read_leader_traits(state, f.get_root());

	EXPECT_EQ(5ui64, m.leader_traits.size());
	EXPECT_EQ(1ui64, m.personality_traits.size());
	EXPECT_EQ(2ui64, m.background_traits.size());
	EXPECT_EQ(m.no_personality_trait, m.named_leader_trait_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("no_personality"))]);
	EXPECT_EQ(leader_trait_tag(2), m.named_leader_trait_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("test_personality"))]);
	EXPECT_EQ(m.no_background_trait, m.named_leader_trait_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("no_background"))]);
	EXPECT_EQ(leader_trait_tag(3), m.named_leader_trait_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("b1"))]);
	EXPECT_EQ(leader_trait_tag(4), m.named_leader_trait_index[text_data::get_thread_safe_existing_text_handle(tex, RANGE("b2"))]);

	EXPECT_EQ(leader_trait_tag(2), m.personality_traits[0]);
	EXPECT_EQ(leader_trait_tag(3), m.background_traits[0]);
	EXPECT_EQ(leader_trait_tag(4), m.background_traits[1]);

	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(tex, RANGE("no_personality")), m.leader_traits[m.no_personality_trait]);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(tex, RANGE("test_personality")), m.leader_traits[leader_trait_tag(2)]);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(tex, RANGE("no_background")), m.leader_traits[m.no_background_trait]);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(tex, RANGE("b1")), m.leader_traits[leader_trait_tag(3)]);
	EXPECT_EQ(text_data::get_thread_safe_existing_text_handle(tex, RANGE("b2")), m.leader_traits[leader_trait_tag(4)]);

	EXPECT_EQ(0.5f, m.leader_trait_definitions.get(m.no_personality_trait, traits::morale));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(m.no_personality_trait, traits::organisation));
	EXPECT_EQ(1.0f, m.leader_trait_definitions.get(leader_trait_tag(2), traits::defence));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(2), traits::reliability));

	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(m.no_background_trait, traits::morale));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(m.no_background_trait, traits::organisation));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(3), traits::defence));
	EXPECT_EQ(2.0f, m.leader_trait_definitions.get(leader_trait_tag(3), traits::reliability));
	EXPECT_EQ(4.0f, m.leader_trait_definitions.get(leader_trait_tag(3), traits::speed));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(4), traits::defence));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(4), traits::reliability));
	EXPECT_EQ(0.0f, m.leader_trait_definitions.get(leader_trait_tag(4), traits::speed));
}

TEST(military_tests, full_unit_read) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test4"));

	military_manager military_m;
	economy::economic_scenario econ_m;
	sound::sound_manager sound_m;
	text_data::text_sequences tex;

	economy::read_goods(econ_m, f.get_root(), tex);
	sound::read_effects(sound_m, tex, f.get_root());

	parsing_state state(tex, military_m);

	pre_parse_unit_types(state, f.get_root());

	read_unit_types(state, military_m, econ_m, sound_m, tex);

	const auto cement_tag = tag_from_text(econ_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(tex, RANGE("cement")));
	const auto canned_food_tag = tag_from_text(econ_m.named_goods_index, text_data::get_thread_safe_existing_text_handle(tex, RANGE("canned_food")));

	const auto army_move_tag = tag_from_text(sound_m.named_sound_effects, text_data::get_thread_safe_existing_text_handle(tex, RANGE("army_move")));
	const auto click_tag = tag_from_text(sound_m.named_sound_effects, text_data::get_thread_safe_existing_text_handle(tex, RANGE("click")));

	const auto guard_tag = tag_from_text(military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(tex, RANGE("guard")));
	const auto cruiser_tag = tag_from_text(military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(tex, RANGE("cruiser")));
	const auto plane_tag = tag_from_text(military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(tex, RANGE("plane")));

	EXPECT_EQ(18ui8, military_m.unit_types[guard_tag].icon);
	EXPECT_EQ(uint8_t(unit_type::class_infantry | unit_type::primary_culture), military_m.unit_types[guard_tag].flags);
	EXPECT_EQ(3.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::strength]);
	EXPECT_EQ(30.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::organization]);
	EXPECT_EQ(4.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::speed]);
	EXPECT_EQ(90.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::build_time]);
	EXPECT_EQ(1.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::supply_consumption]);
	EXPECT_EQ(0.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::reconnaissance]);
	EXPECT_EQ(8.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::attack]);
	EXPECT_EQ(2.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::defense]);
	EXPECT_EQ(1.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::discipline]);
	EXPECT_EQ(0.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::support]);
	EXPECT_EQ(2.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::maneuver]);
	EXPECT_EQ(0.0f, military_m.unit_types[guard_tag].base_attributes[unit_attribute::enabled]);
	EXPECT_EQ(5.0, military_m.unit_build_costs.get(guard_tag, cement_tag));
	EXPECT_EQ(10.0, military_m.unit_build_costs.get(guard_tag, canned_food_tag));
	EXPECT_EQ(0.25, military_m.unit_base_supply_costs.get(guard_tag, canned_food_tag));

	EXPECT_EQ(11ui8, military_m.unit_types[cruiser_tag].icon);
	EXPECT_EQ(8ui8, military_m.unit_types[cruiser_tag].naval_icon);
	EXPECT_EQ(4ui8, military_m.unit_types[cruiser_tag].min_port_level);
	EXPECT_EQ(1i8, military_m.unit_types[cruiser_tag].limit_per_port);
	EXPECT_EQ(20ui8, military_m.unit_types[cruiser_tag].supply_consumption_score);
	EXPECT_EQ(16ui8, military_m.unit_types[cruiser_tag].colonial_points);
	EXPECT_EQ(army_move_tag, military_m.unit_types[cruiser_tag].move_sound);
	EXPECT_EQ(click_tag, military_m.unit_types[cruiser_tag].select_sound);
	EXPECT_EQ(uint8_t(unit_type::class_light_ship | unit_type::is_sail | unit_type::cant_build_overseas), military_m.unit_types[cruiser_tag].flags);
	EXPECT_EQ(100.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::strength]);
	EXPECT_EQ(30.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::organization]);
	EXPECT_EQ(12.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::speed]);
	EXPECT_EQ(240.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::build_time]);
	EXPECT_EQ(1.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::supply_consumption]);
	EXPECT_EQ(50.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::hull]);
	EXPECT_EQ(30.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::gun_power]);
	EXPECT_EQ(7.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::fire_range]);
	EXPECT_EQ(6.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::evasion]);
	EXPECT_EQ(2.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::torpedo_attack]);
	EXPECT_EQ(1.0f, military_m.unit_types[cruiser_tag].base_attributes[unit_attribute::enabled]);
	EXPECT_EQ(0.0, military_m.unit_build_costs.get(cruiser_tag, cement_tag));
	EXPECT_EQ(0.0, military_m.unit_build_costs.get(cruiser_tag, canned_food_tag));
	EXPECT_EQ(0.0, military_m.unit_base_supply_costs.get(cruiser_tag, canned_food_tag));

	EXPECT_EQ(19ui8, military_m.unit_types[plane_tag].icon);
	EXPECT_EQ(uint8_t(unit_type::class_cavalry), military_m.unit_types[plane_tag].flags);
	EXPECT_EQ(3.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::strength]);
	EXPECT_EQ(30.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::organization]);
	EXPECT_EQ(5.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::speed]);
	EXPECT_EQ(120.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::build_time]);
	EXPECT_EQ(1.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::supply_consumption]);
	EXPECT_EQ(4.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::reconnaissance]);
	EXPECT_EQ(1.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::attack]);
	EXPECT_EQ(10.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::defense]);
	EXPECT_EQ(3.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::discipline]);
	EXPECT_EQ(2.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::support]);
	EXPECT_EQ(2.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::maneuver]);
	EXPECT_EQ(0.0f, military_m.unit_types[plane_tag].base_attributes[unit_attribute::enabled]);
	EXPECT_EQ(0.0, military_m.unit_build_costs.get(plane_tag, cement_tag));
	EXPECT_EQ(0.0, military_m.unit_build_costs.get(plane_tag, canned_food_tag));
	EXPECT_EQ(9.0, military_m.unit_base_supply_costs.get(plane_tag, canned_food_tag));
}

TEST(military_tests, full_cb_read) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:\\test1"));

	scenario::scenario_manager s;
	events::event_creation_manager ecm;

	parsing_state state(s.gui_m.text_data_sequences, s.military_m);
	pre_parse_cb_types(state, f.get_root());

	read_cb_types(state, s, ecm);

	EXPECT_EQ(3ui64, s.military_m.cb_types.size());

	EXPECT_EQ(cb_type_tag(0), s.military_m.cb_types[cb_type_tag(0)].id);
	EXPECT_EQ(cb_type::always | cb_type::po_disarmament | cb_type::po_reparations, s.military_m.cb_types[cb_type_tag(0)].flags);
	EXPECT_NE(triggers::trigger_tag(), s.military_m.cb_types[cb_type_tag(0)].allowed_countries);
	EXPECT_NE(triggers::trigger_tag(), s.military_m.cb_types[cb_type_tag(0)].allowed_substate_regions);
	EXPECT_EQ(triggers::trigger_tag(), s.military_m.cb_types[cb_type_tag(0)].allowed_states_in_crisis);
	EXPECT_EQ(triggers::trigger_tag(), s.military_m.cb_types[cb_type_tag(0)].allowed_states);

	EXPECT_EQ(cb_type_tag(1), s.military_m.cb_types[cb_type_tag(1)].id);
	EXPECT_EQ(cb_type::all_allowed_states | cb_type::po_transfer_provinces | cb_type::po_liberate, s.military_m.cb_types[cb_type_tag(1)].flags);
	EXPECT_EQ(triggers::trigger_tag(), s.military_m.cb_types[cb_type_tag(1)].allowed_countries);
	EXPECT_NE(triggers::effect_tag(), s.military_m.cb_types[cb_type_tag(1)].on_add);
	EXPECT_NE(triggers::trigger_tag(), s.military_m.cb_types[cb_type_tag(1)].can_use);
	EXPECT_NE(text_data::text_tag(), s.military_m.cb_types[cb_type_tag(1)].war_name);
	EXPECT_EQ(15ui8, s.military_m.cb_types[cb_type_tag(1)].sprite_index);
	EXPECT_EQ(12ui8, s.military_m.cb_types[cb_type_tag(1)].months);
	EXPECT_EQ(48ui8, s.military_m.cb_types[cb_type_tag(1)].truce_months);
	EXPECT_EQ(2.0f, s.military_m.cb_types[cb_type_tag(1)].badboy_factor);
	EXPECT_EQ(3.0f, s.military_m.cb_types[cb_type_tag(1)].prestige_factor);
	EXPECT_EQ(4.0f, s.military_m.cb_types[cb_type_tag(1)].peace_cost_factor);
	EXPECT_EQ(5.0f, s.military_m.cb_types[cb_type_tag(1)].penalty_factor);
	EXPECT_EQ(6.0f, s.military_m.cb_types[cb_type_tag(1)].break_truce_prestige_factor);
	EXPECT_EQ(7.0f, s.military_m.cb_types[cb_type_tag(1)].break_truce_infamy_factor);
	EXPECT_EQ(8.0f, s.military_m.cb_types[cb_type_tag(1)].break_truce_militancy_factor);
	EXPECT_EQ(9.0f, s.military_m.cb_types[cb_type_tag(1)].good_relation_prestige_factor);
	EXPECT_EQ(10.0f, s.military_m.cb_types[cb_type_tag(1)].good_relation_infamy_factor);
	EXPECT_EQ(11.0f, s.military_m.cb_types[cb_type_tag(1)].good_relation_militancy_factor);

	EXPECT_EQ(cb_type_tag(2), s.military_m.cb_types[cb_type_tag(2)].id);
	EXPECT_EQ(cb_type::not_in_crisis, s.military_m.cb_types[cb_type_tag(2)].flags);
	EXPECT_EQ(0.5f, s.military_m.cb_types[cb_type_tag(2)].construction_speed);
	EXPECT_EQ(2.0f, s.military_m.cb_types[cb_type_tag(2)].tws_battle_factor);
	EXPECT_NE(triggers::trigger_tag(), s.military_m.cb_types[cb_type_tag(2)].allowed_states);

}
