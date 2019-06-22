#include "common\common.h"
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
#include "world_state\\world_state.h"
#include <ppl.h>
#include "nations\\nations_internals.hpp"
#include "scenario\\scenario_io.h"
#include "common\\testing_world_state.h"
#include "military\military_internals.hpp"
#include "military\military_io_internals.hpp"

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
		"}\r\n");

		directory_representation history4 = directory_representation(u"history", test4);
	directory_representation pops4 = directory_representation(u"pops", history4);
	directory_representation hd3 = directory_representation(u"1800.1.1", pops4);

	file_representation hf1 = file_representation(u"file.txt", hd3,
		"220 = {\r\n"
		"	soldiers = {\r\n"
		"		culture = albanian\r\n"
		"		religion = sunni\r\n"
		"		size = 100\r\n"
		"	}\r\n"
		"}\r\n"
		"243 = {\r\n"
		"	soldiers = {\r\n"
		"		culture = albanian\r\n"
		"		religion = sunni\r\n"
		"		size = 200\r\n"
		"	}\r\n"
		"}\r\n"
		"232 = {\r\n"
		"	soldiers = {\r\n"
		"		culture = albanian\r\n"
		"		religion = sunni\r\n"
		"		size = 200\r\n"
		"	}\r\n"
		"}\r\n"
	);

	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace military;


inline int32_t created_cb_count = 0;
cb_type_tag fake_create_new_cb(test_ws<scenario::scenario_manager>& s) {
	return cb_type_tag(created_cb_count++);
}

inline int32_t max_row_size = -1;
tagged_array_view<float, uint32_t> fake_safe_get_trait_row(test_ws<scenario::scenario_manager>& s, leader_trait_tag id) {
	max_row_size = std::max(max_row_size, to_index(id));
	s.resize<military::leader_trait_values>(id, military::traits::trait_count);
	return s.get_row<military::leader_trait_values>(id);
}


inline int32_t created_trigger_count = 0;
triggers::trigger_tag fake_generic_read_trigger(token_generator& gen, test_ws<scenario::scenario_manager>& s, empty_error_handler&, triggers::trigger_scope_state outer_scope) {
	discard_group(gen);
	return triggers::trigger_tag(created_trigger_count++);
}
inline int32_t created_effect_count = 0;
triggers::effect_tag fake_generic_read_effect(token_generator& gen, test_ws<scenario::scenario_manager>& s, events::fake_event_creation_manager& ecm, empty_error_handler& err, triggers::trigger_scope_state outer_scope) {
	discard_group(gen);
	return triggers::effect_tag(created_effect_count++);
}


TEST(military_tests, test_cb_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");


	test_ws<scenario::scenario_manager> test_state;
	events::fake_event_creation_manager ecm;
	created_cb_count = 0;

	parsing_environment<test_ws<scenario::scenario_manager>, events::fake_event_creation_manager,
		fake_create_new_cb, fake_safe_get_trait_row, fake_generic_read_trigger, fake_generic_read_effect> state(test_state, ecm);

	internal_pre_parse_cb_types(state, f.get_root());

	EXPECT_EQ(3, created_cb_count);
	EXPECT_EQ(-1, max_row_size);

	EXPECT_EQ(3ui64, test_state.size<::cb_type::name>());
	EXPECT_EQ(3ui64, test_state.name_map<military::cb_type_tag>().size());

	EXPECT_EQ(cb_type_tag(0), test_state.get<::cb_type::id>(cb_type_tag(0)));
	EXPECT_EQ(cb_type_tag(1), test_state.get<::cb_type::id>(cb_type_tag(1)));
	EXPECT_EQ(cb_type_tag(2), test_state.get<::cb_type::id>(cb_type_tag(2)));
	auto const resa = test_state.name_map<military::cb_type_tag>()[test_state.get_thread_safe_existing_text_handle("cb_a")];
	EXPECT_EQ(cb_type_tag(0), resa);
	EXPECT_EQ(cb_type_tag(1), test_state.name_map<military::cb_type_tag>()[test_state.get<::cb_type::name>(cb_type_tag(1))]);
	EXPECT_EQ(cb_type_tag(2), test_state.name_map<military::cb_type_tag>()[test_state.get<::cb_type::name>(cb_type_tag(2))]);
}

int32_t resize_traits_called_count = 0;
void fake_resize_trait_count(test_ws<scenario::scenario_manager>&, int32_t sz) {
	++resize_traits_called_count;
}

TEST(military_tests, traits_personality) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test2");

	test_ws<scenario::scenario_manager> test_state;
	events::fake_event_creation_manager ecm;
	created_cb_count = 0;
	resize_traits_called_count = 0;
	max_row_size = -1;

	parsing_environment<test_ws<scenario::scenario_manager>, events::fake_event_creation_manager,
		fake_create_new_cb, fake_safe_get_trait_row, fake_generic_read_trigger, fake_generic_read_effect> state(test_state, ecm);

	internal_read_leader_traits<fake_resize_trait_count>(state, f.get_root());

	EXPECT_EQ(2, max_row_size);
	EXPECT_EQ(1, resize_traits_called_count);
	EXPECT_EQ(3ui64, test_state.size<leader_trait_name>());
	EXPECT_EQ(1ui64, test_state.size<personality_traits>());
	EXPECT_EQ(0ui64, test_state.size<background_traits>());

	EXPECT_EQ(military::no_personality_trait, test_state.name_map<leader_trait_tag>()[test_state.get_thread_safe_existing_text_handle("no_personality")]);
	EXPECT_EQ(leader_trait_tag(2), test_state.name_map<leader_trait_tag>()[test_state.get_thread_safe_existing_text_handle("test_personality")]);
	EXPECT_EQ(leader_trait_tag(2), test_state.get<personality_traits>(0));
	EXPECT_EQ(test_state.get_thread_safe_existing_text_handle("no_personality"), test_state.get<leader_trait_name>(military::no_personality_trait));
	EXPECT_EQ(test_state.get_thread_safe_existing_text_handle("test_personality"), test_state.get<leader_trait_name>(leader_trait_tag(2)));

	EXPECT_EQ(0.5f, test_state.get<leader_trait_values>(military::no_personality_trait, traits::morale));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(military::no_personality_trait, traits::organisation));
	EXPECT_EQ(1.0f, test_state.get<leader_trait_values>(leader_trait_tag(2), traits::defence));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(leader_trait_tag(2), traits::reliability));
}

TEST(military_tests, traits_mixed) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test3");

	test_ws<scenario::scenario_manager> test_state;
	events::fake_event_creation_manager ecm;
	created_cb_count = 0;
	resize_traits_called_count = 0;
	max_row_size = -1;

	parsing_environment<test_ws<scenario::scenario_manager>, events::fake_event_creation_manager,
		fake_create_new_cb, fake_safe_get_trait_row, fake_generic_read_trigger, fake_generic_read_effect> state(test_state, ecm);

	internal_read_leader_traits<fake_resize_trait_count>(state, f.get_root());

	EXPECT_EQ(4, max_row_size);
	EXPECT_EQ(1, resize_traits_called_count);

	EXPECT_EQ(5ui64, test_state.size<leader_trait_name>());
	EXPECT_EQ(1ui64, test_state.size<personality_traits>());
	EXPECT_EQ(2ui64, test_state.size<background_traits>());
	EXPECT_EQ(military::no_personality_trait, test_state.name_map<leader_trait_tag>()[test_state.get_thread_safe_existing_text_handle( "no_personality")]);
	EXPECT_EQ(leader_trait_tag(2), test_state.name_map<leader_trait_tag>()[test_state.get_thread_safe_existing_text_handle( "test_personality")]);
	EXPECT_EQ(military::no_background_trait, test_state.name_map<leader_trait_tag>()[test_state.get_thread_safe_existing_text_handle( "no_background")]);
	EXPECT_EQ(leader_trait_tag(3), test_state.name_map<leader_trait_tag>()[test_state.get_thread_safe_existing_text_handle( "b1")]);
	EXPECT_EQ(leader_trait_tag(4), test_state.name_map<leader_trait_tag>()[test_state.get_thread_safe_existing_text_handle("b2")]);

	EXPECT_EQ(leader_trait_tag(2), test_state.get<personality_traits>(0));
	EXPECT_EQ(leader_trait_tag(3), test_state.get<background_traits>(0));
	EXPECT_EQ(leader_trait_tag(4), test_state.get<background_traits>(1));

	EXPECT_EQ(test_state.get_thread_safe_existing_text_handle("no_personality"), test_state.get<leader_trait_name>(military::no_personality_trait));
	EXPECT_EQ(test_state.get_thread_safe_existing_text_handle("test_personality"), test_state.get<leader_trait_name>(leader_trait_tag(2)));
	EXPECT_EQ(test_state.get_thread_safe_existing_text_handle("no_background"), test_state.get<leader_trait_name>(military::no_background_trait));
	EXPECT_EQ(test_state.get_thread_safe_existing_text_handle("b1"), test_state.get<leader_trait_name>(leader_trait_tag(3)));
	EXPECT_EQ(test_state.get_thread_safe_existing_text_handle("b2"), test_state.get<leader_trait_name>(leader_trait_tag(4)));

	EXPECT_EQ(0.5f, test_state.get<leader_trait_values>(military::no_personality_trait, traits::morale));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(military::no_personality_trait, traits::organisation));
	EXPECT_EQ(1.0f, test_state.get<leader_trait_values>(leader_trait_tag(2), traits::defence));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(leader_trait_tag(2), traits::reliability));

	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(military::no_background_trait, traits::morale));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(military::no_background_trait, traits::organisation));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(leader_trait_tag(3), traits::defence));
	EXPECT_EQ(2.0f, test_state.get<leader_trait_values>(leader_trait_tag(3), traits::reliability));
	EXPECT_EQ(4.0f, test_state.get<leader_trait_values>(leader_trait_tag(3), traits::speed));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(leader_trait_tag(4), traits::defence));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(leader_trait_tag(4), traits::reliability));
	EXPECT_EQ(0.0f, test_state.get<leader_trait_values>(leader_trait_tag(4), traits::speed));
}

TEST(military_tests, full_cb_read) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(u"F:\\test1");

	test_ws<scenario::scenario_manager> test_state;
	events::fake_event_creation_manager ecm;
	created_cb_count = 0;
	resize_traits_called_count = 0;
	max_row_size = -1;

	parsing_environment<test_ws<scenario::scenario_manager>, events::fake_event_creation_manager,
		fake_create_new_cb, fake_safe_get_trait_row, fake_generic_read_trigger, fake_generic_read_effect> state(test_state, ecm);

	created_trigger_count = 0;
	created_effect_count = 0;

	internal_pre_parse_cb_types(state, f.get_root());

	internal_read_cb_types(state);

	EXPECT_EQ(4, created_trigger_count);
	EXPECT_EQ(1, created_effect_count);
	EXPECT_EQ(3, test_state.size<::cb_type::name>());

	EXPECT_EQ(cb_type_tag(0), test_state.get<::cb_type::id>(cb_type_tag(0)));
	EXPECT_EQ(military::cb_type::always | military::cb_type::po_disarmament | military::cb_type::po_reparations, test_state.get<::cb_type::flags>(cb_type_tag(0)));
	EXPECT_NE(triggers::trigger_tag(), test_state.get<::cb_type::allowed_countries>(cb_type_tag(0)));
	EXPECT_NE(triggers::trigger_tag(), test_state.get<::cb_type::allowed_substate_regions>(cb_type_tag(0)));
	EXPECT_EQ(triggers::trigger_tag(), test_state.get<::cb_type::allowed_states_in_crisis>(cb_type_tag(0)));
	EXPECT_EQ(triggers::trigger_tag(), test_state.get<::cb_type::allowed_states>(cb_type_tag(0)));

	EXPECT_EQ(cb_type_tag(1), test_state.get<::cb_type::id>(cb_type_tag(1)));
	EXPECT_EQ(military::cb_type::all_allowed_states | military::cb_type::po_transfer_provinces | military::cb_type::po_liberate, test_state.get<::cb_type::flags>(cb_type_tag(1)));
	EXPECT_EQ(triggers::trigger_tag(), test_state.get<::cb_type::allowed_countries>(cb_type_tag(1)));
	EXPECT_NE(triggers::effect_tag(), test_state.get<::cb_type::on_add>(cb_type_tag(1)));
	EXPECT_NE(triggers::trigger_tag(), test_state.get<::cb_type::can_use>(cb_type_tag(1)));
	EXPECT_NE(text_data::text_tag(), test_state.get<::cb_type::war_name>(cb_type_tag(1)));
	EXPECT_EQ(15ui8, test_state.get<::cb_type::sprite_index>(cb_type_tag(1)));
	EXPECT_EQ(12ui8, test_state.get<::cb_type::months>(cb_type_tag(1)));
	EXPECT_EQ(48ui8, test_state.get<::cb_type::truce_months>(cb_type_tag(1)));
	EXPECT_EQ(2.0f, test_state.get<::cb_type::badboy_factor>(cb_type_tag(1)));
	EXPECT_EQ(3.0f, test_state.get<::cb_type::prestige_factor>(cb_type_tag(1)));
	EXPECT_EQ(4.0f, test_state.get<::cb_type::peace_cost_factor>(cb_type_tag(1)));
	EXPECT_EQ(5.0f, test_state.get<::cb_type::penalty_factor>(cb_type_tag(1)));
	EXPECT_EQ(6.0f, test_state.get<::cb_type::break_truce_prestige_factor>(cb_type_tag(1)));
	EXPECT_EQ(7.0f, test_state.get<::cb_type::break_truce_infamy_factor>(cb_type_tag(1)));
	EXPECT_EQ(8.0f, test_state.get<::cb_type::break_truce_militancy_factor>(cb_type_tag(1)));
	EXPECT_EQ(9.0f, test_state.get<::cb_type::good_relation_prestige_factor>(cb_type_tag(1)));
	EXPECT_EQ(10.0f, test_state.get<::cb_type::good_relation_infamy_factor>(cb_type_tag(1)));
	EXPECT_EQ(11.0f, test_state.get<::cb_type::good_relation_militancy_factor>(cb_type_tag(1)));

	EXPECT_EQ(cb_type_tag(2), test_state.get<::cb_type::id>(cb_type_tag(2)));
	EXPECT_EQ(military::cb_type::not_in_crisis, test_state.get<::cb_type::flags>(cb_type_tag(2)));
	EXPECT_EQ(0.5f, test_state.get<::cb_type::construction_speed>(cb_type_tag(2)));
	EXPECT_EQ(2.0f, test_state.get<::cb_type::tws_battle_factor>(cb_type_tag(2)));
	EXPECT_NE(triggers::trigger_tag(), test_state.get<::cb_type::allowed_states>(cb_type_tag(2)));

}

nations::country_tag fake_make_tag_holder(test_ws<world_state> const&, cultures::national_tag t) {
	return nations::country_tag(to_index(t));
}

inline int32_t count_new_armies = 0;
military::army_tag fake_new_amry(test_ws<world_state>& ws) {
	return military::army_tag(count_new_armies++);;
}
military::army_tag fake_new_amry_b(test_ws<world_state>& ws, nations::country_tag owner, provinces::province_tag loc) {
	auto res =  military::army_tag(count_new_armies++);
	ws.set<army::location>(res, loc);
	ws.set<army::owner>(res, owner);

	ws.add_item(ws.get<nation::armies>(owner), res);
	return res;
}

inline int32_t count_new_fleets = 0;
military::fleet_tag fake_new_fleet(test_ws<world_state>& ws) {
	return military::fleet_tag(count_new_fleets++);;
}
military::fleet_tag fake_new_fleet_b(test_ws<world_state>& ws, nations::country_tag owner, provinces::province_tag loc) {
	auto res = military::fleet_tag(count_new_fleets++);
	ws.set<fleet::location>(res, loc);

	ws.add_item(ws.get<nation::fleets>(owner), res);
	return res;
}

inline int32_t count_leaders_made = 0;
leader_tag fake_leader(test_ws<world_state>&) {
	return leader_tag(count_leaders_made++);
}
leader_tag fake_leader_b(test_ws<world_state>&, cultures::culture_tag, bool) {
	return leader_tag(count_leaders_made++);
}

inline int32_t count_calculation_type = 0;
void fake_calculate_leader_traits(test_ws<world_state>&, military::leader_tag) {
	++count_calculation_type;
}

TEST(military_tests, read_oob_test) {
	char test_file[] =
		"JAP = {\r\n"
		"	value = 130\r\n"
		"	level = 5\r\n"
		"	influence_value = 25\r\n"
		"}\r\n"
		"MEX = {\r\n"
		"	value = 150\r\n"
		"	level = 3\r\n"
		"	influence_value = 5\r\n"
		"}\r\n"
		"leader = {\r\n"
		"	name = \"William T. Sherman\"\r\n"
		"	type = land\r\n"
		"	date = \"1846.1.1\"\r\n"
		"	personality = audacious\r\n"
		"	background = school_of_offense\r\n"
		"	picture = \"european_general_31\"\r\n"
		"}\r\n"
		"leader = {\r\n"
		"	name = \"Irvin McDowell\"\r\n"
		"	type = sea\r\n"
		"	date = \"1836.1.1\"\r\n"
		"	personality = bastard\r\n"
		"	background = generals_aide\r\n"
		"	picture = \"european_general_52\"\r\n"
		"}\r\n"
		"army = {\r\n"
		"	name = \"Washington Garrison\"\r\n"
		"	leader = {\r\n"
		"		name = \"Henry W. Halleck\"\r\n"
		"		type = land\r\n"
		"		date = \"1861.1.1\"\r\n"
		"		prestige = 0.05\r\n"
		"		personality = meticulous\r\n"
		"		background = warmonger\r\n"
		"		picture = \"european_general_6\"\r\n"
		"	}\r\n"
		"	location = 220\r\n"
		"	regiment = {\r\n"
		"		name = \"1st US Artillery\"\r\n"
		"		type = artillery\r\n"
		"		home = 220\r\n"
		"	}\r\n"
		"	regiment = {\r\n"
		"		name = \"1st US Infantry\"\r\n"
		"		type = infantry\r\n"
		"		home = 243\r\n"
		"	}\r\n"
		"	regiment = {\r\n"
		"		name = \"2nd US Infantry\"\r\n"
		"		type = infantry\r\n"
		"		home = 232\r\n"
		"	}\r\n"
		"}\r\n"
		"navy = {\r\n"
		"	name = \"Atlantic Blockading Squadron\"\r\n"
		"	location = 219\r\n"
		"	ship = {\r\n"
		"		name = \"USS Potomac\"\r\n"
		"		type = frigate\r\n"
		"	}\r\n"
		"	ship = {\r\n"
		"		name = \"USS Sabine\"\r\n"
		"		type = frigate\r\n"
		"	}\r\n"
		"	ship = {\r\n"
		"		name = \"USS Congress\"\r\n"
		"		type = frigate\r\n"
		"	}\r\n"
		"	ship = {\r\n"
		"		name = \"USS Roanoke\"\r\n"
		"		type = commerce_raider\r\n"
		"	}\r\n"
		"	ship = {\r\n"
		"		name = \"USS Minnesota\"\r\n"
		"		type = commerce_raider\r\n"
		"	}\r\n"
		"}\r\n"
		;


	

	preparse_test_files real_fs;
	file_system f;
	f.set_root(u"F:\\test4");

	const char usat[] = "USA";
	const char japt[] = "JAP";
	const char mext[] = "MEX";

	cultures::national_tag usa_t = cultures::national_tag(0);
	cultures::national_tag jap_t = cultures::national_tag(1);
	cultures::national_tag mex_t = cultures::national_tag(2);

	nations::country_tag usa = nations::country_tag(0);
	nations::country_tag jap = nations::country_tag(1);
	nations::country_tag mex = nations::country_tag(2);

	test_ws<world_state> test_state;

	test_state.name_map<cultures::national_tag>()[cultures::tag_to_encoding(usat, usat + 4)] = usa_t;
	test_state.name_map<cultures::national_tag>()[cultures::tag_to_encoding(japt, japt + 4)] = jap_t;
	test_state.name_map<cultures::national_tag>()[cultures::tag_to_encoding(mext, mext + 4)] = mex_t;


	test_state.name_map<leader_trait_tag>()[test_state.get_text_handle("meticulous")] = leader_trait_tag(0);
	test_state.name_map<leader_trait_tag>()[test_state.get_text_handle("audacious")] = leader_trait_tag(1);
	test_state.name_map<leader_trait_tag>()[test_state.get_text_handle("school_of_offense")] = leader_trait_tag(2);
	test_state.name_map<leader_trait_tag>()[test_state.get_text_handle("bastard")] = leader_trait_tag(3);
	test_state.name_map<leader_trait_tag>()[test_state.get_text_handle("generals_aide")] = leader_trait_tag(4);
	test_state.name_map<leader_trait_tag>()[test_state.get_text_handle("warmonger")] = leader_trait_tag(5);

	count_new_armies = 0;
	count_new_fleets = 0;
	count_leaders_made = 0;

	token_generator gen(RANGE(test_file));
	internal_read_oob_file<fake_make_tag_holder, fake_new_amry_b, fake_new_fleet_b, fake_leader_b, fake_calculate_leader_traits>(test_state, usa, gen);

	EXPECT_EQ(1, count_new_armies);
	EXPECT_EQ(1, count_new_fleets);
	EXPECT_EQ(3, count_leaders_made);

	EXPECT_EQ(1ui32, test_state.get_size(test_state.get<nation::sphere_members>(usa)));

	EXPECT_EQ(usa, test_state.get<nation::sphere_leader>(jap));
	EXPECT_EQ(25, nations::internal_get_influence_value(test_state, usa, jap));
	EXPECT_EQ(5, nations::internal_get_influence_level(test_state, usa, jap));
	EXPECT_EQ(130, nations::internal_get_relationship(test_state, usa, jap));
	EXPECT_EQ(130, nations::internal_get_relationship(test_state, jap, usa));


	EXPECT_NE(usa, test_state.get<nation::sphere_leader>(mex));
	EXPECT_EQ(5, nations::internal_get_influence_value(test_state, usa, mex));
	EXPECT_EQ(3, nations::internal_get_influence_level(test_state, usa, mex));
	EXPECT_EQ(150, nations::internal_get_relationship(test_state, usa, mex));
	EXPECT_EQ(150, nations::internal_get_relationship(test_state, mex, usa));

	EXPECT_EQ(2ui32, test_state.get_size(test_state.get<nation::generals>(usa)));
	EXPECT_EQ(1ui32, test_state.get_size(test_state.get<nation::admirals>(usa)));

	EXPECT_EQ(1ui32, test_state.get_size(test_state.get<nation::armies>(usa)));
	EXPECT_EQ(1ui32, test_state.get_size(test_state.get<nation::fleets>(usa)));

	military::army_tag a = test_state.get_range(test_state.get<nation::armies>(usa)).first[0i32];

	EXPECT_NE(military::leader_tag(), test_state.get<army::leader>(a));
	EXPECT_EQ(provinces::province_tag(220), test_state.get<army::location>(a));
	// EXPECT_EQ(1.0f, a.org);
	EXPECT_EQ(3000.0f, test_state.get<army::target_soldiers>(a));
	
	auto const a_leader = test_state.get<army::leader>(a);

	EXPECT_EQ(test_state.get<military_leader::is_attached>(a_leader), true);
	EXPECT_EQ(test_state.get<military_leader::background>(a_leader), tag_from_text(test_state.name_map<leader_trait_tag>(), test_state.get_existing_text_handle("warmonger")));
	EXPECT_EQ(test_state.get<military_leader::personality>(a_leader), tag_from_text(test_state.name_map<leader_trait_tag>(), test_state.get_existing_text_handle("meticulous")));
	EXPECT_EQ(test_state.get<military_leader::creation_date>(a_leader), date_to_tag(boost::gregorian::date(1861, boost::gregorian::Jan, 1)));

	/*EXPECT_EQ(test_state.get<military_leader::morale>(a_leader), -0.2f);
	EXPECT_EQ(test_state.get<military_leader::attack>(a_leader), 0.0f);
	EXPECT_EQ(test_state.get<military_leader::defence>(a_leader), 0.0f);
	EXPECT_EQ(test_state.get<military_leader::speed>(a_leader), 0.0f);
	EXPECT_EQ(test_state.get<military_leader::reconnaissance>(a_leader), 0.0f);
	EXPECT_EQ(test_state.get<military_leader::organisation>(a_leader), 0.0f);
	EXPECT_EQ(test_state.get<military_leader::experience>(a_leader), 0.0f);
	EXPECT_EQ(test_state.get<military_leader::reliability>(a_leader), 0.0f);*/

	military::fleet_tag b = test_state.get_range(test_state.get<nation::fleets>(usa)).first[0i32];
	EXPECT_NE(military::fleet_tag(), b);
	EXPECT_EQ(military::leader_tag(), test_state.get<fleet::leader>(b));
	EXPECT_EQ(provinces::province_tag(219), test_state.get<fleet::location>(b));
	EXPECT_EQ(5.0f, test_state.get<fleet::size>(b));
}

inline int32_t new_leader_count = 0;

TEST(military_tests, internal_get_leader_picture_t) {
	test_ws<world_state> test_state;

	test_state.get<culture::group>(cultures::culture_tag(0)) = cultures::culture_group_tag(0);
	test_state.get<culture_group::leader_pictures>(cultures::culture_group_tag(0)).general_offset = 0;
	test_state.get<culture_group::leader_pictures>(cultures::culture_group_tag(0)).general_size = 1;
	test_state.get<cultures::leader_pictures>(0) = graphics::texture_tag(10);

	EXPECT_EQ(graphics::texture_tag(10), internal_get_leader_picture(test_state, cultures::culture_tag(0), true));
}

TEST(military_tests, internal_get_admiral_leader_picture_t) {
	test_ws<world_state> test_state;

	test_state.get<culture::group>(cultures::culture_tag(0)) = cultures::culture_group_tag(0);
	test_state.get<culture_group::leader_pictures>(cultures::culture_group_tag(0)).admiral_offset = 1;
	test_state.get<culture_group::leader_pictures>(cultures::culture_group_tag(0)).admiral_size = 2;
	test_state.get<cultures::leader_pictures>(0) = graphics::texture_tag(10);
	test_state.get<cultures::leader_pictures>(1) = graphics::texture_tag(5);
	test_state.get<cultures::leader_pictures>(2) = graphics::texture_tag(7);
	test_state.get<cultures::leader_pictures>(3) = graphics::texture_tag(10);

	auto res = internal_get_leader_picture(test_state, cultures::culture_tag(0), false);
	EXPECT_TRUE(res == graphics::texture_tag(5) || res == graphics::texture_tag(7));
}

TEST(military_tests, leader_names) {
	test_ws<world_state> test_state;

	vector_backed_string<char16_t> name_a(u"NAME_A");
	vector_backed_string<char16_t> name_b(u"NAME_B");
	vector_backed_string<char16_t> name_c(u"NAME_C");

	test_state.set<culture::first_names>(cultures::culture_tag(1), 0, name_a);
	test_state.set<culture::last_names>(cultures::culture_tag(1), 0, name_b);
	test_state.set<culture::last_names>(cultures::culture_tag(1), 1, name_c);

	auto res = internal_get_leader_name(test_state, cultures::culture_tag(1));

	EXPECT_EQ(res.first, name_a);
	EXPECT_TRUE(res.last == name_b || res.last == name_c);
}




inline int32_t count_pictures_found = 0;
graphics::texture_tag fake_picture(test_ws<world_state> const&, cultures::culture_tag, bool) {
	++count_pictures_found;
	return graphics::texture_tag(5);
}

inline int32_t count_names_found = 0;
names_result fake_names(test_ws<world_state> const&, cultures::culture_tag) {
	++count_names_found;
	return names_result{ vector_backed_string<char16_t>(u"FIRST"), vector_backed_string<char16_t>(u"LAST") };
}

TEST(military_tests, leader_generation) {
	test_ws<world_state> test_state;

	auto pre_a = count_leaders_made;
	auto pre_b = count_pictures_found;
	auto pre_c = count_names_found;

	auto res = internal_make_empty_leader<test_ws<world_state>, fake_leader, fake_picture, fake_names>(test_state, cultures::culture_tag(1), true);

	EXPECT_EQ(pre_a + 1, count_leaders_made);
	EXPECT_EQ(pre_b + 1, count_pictures_found);
	EXPECT_EQ(pre_c + 1, count_names_found);

	EXPECT_NE(res, military::leader_tag());
	EXPECT_EQ(test_state.get<military_leader::portrait>(res), graphics::texture_tag(5));
	EXPECT_EQ(test_state.get<military_leader::is_general>(res), true);
	EXPECT_EQ(test_state.get<military_leader::first_name>(res), fake_names(test_state, cultures::culture_tag(1)).first);
	EXPECT_EQ(test_state.get<military_leader::last_name>(res), fake_names(test_state, cultures::culture_tag(1)).last);
}

TEST(military_tests, auto_leader_generation) {
	test_ws<world_state> test_state;
	auto pre_a = count_leaders_made;
	auto pre_b = count_calculation_type;

	test_state.set<military::background_traits>(0, military::leader_trait_tag(0));
	test_state.set<military::personality_traits>(0, military::leader_trait_tag(1));

	auto res = internal_make_auto_leader<test_ws<world_state>, fake_leader_b, fake_calculate_leader_traits>(
		test_state, cultures::culture_tag(0), false, date_to_tag(boost::gregorian::date(1851, 1, 10)));

	EXPECT_EQ(pre_a + 1, count_leaders_made);
	EXPECT_EQ(pre_b + 1, count_calculation_type);
	EXPECT_NE(res, military::leader_tag());
	EXPECT_EQ(test_state.get<military_leader::creation_date>(res), date_to_tag(boost::gregorian::date(1851, 1, 10)));
	EXPECT_EQ(test_state.get<military_leader::background>(res), military::leader_trait_tag(0));
	EXPECT_EQ(test_state.get<military_leader::personality>(res), military::leader_trait_tag(1));
}

TEST(military_tests, trait_calculation) {
	test_ws<world_state> test_state;

	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::organisation, 1.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::morale, 3.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::attack, 4.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::defence, 5.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::reconnaissance, 6.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::speed, 7.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::experience, 8.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(0), military::traits::reliability, 2.0f);

	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::organisation, 10.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::morale, 30.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::attack, 40.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::defence, 50.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::reconnaissance, 60.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::speed, 70.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::experience, 80.0f);
	test_state.set<military::leader_trait_values>(military::leader_trait_tag(1), military::traits::reliability, 20.0f);

	test_state.set<military_leader::background>(military::leader_tag(0), military::leader_trait_tag(0));
	test_state.set<military_leader::personality>(military::leader_tag(0), military::leader_trait_tag(1));

	internal_calculate_leader_traits(test_state, military::leader_tag(0));

	EXPECT_EQ(11.0f, test_state.get<military_leader::organisation>(military::leader_tag(0)));
	EXPECT_EQ(33.0f, test_state.get<military_leader::morale>(military::leader_tag(0)));
	EXPECT_EQ(44.0f, test_state.get<military_leader::attack>(military::leader_tag(0)));
	EXPECT_EQ(55.0f, test_state.get<military_leader::defence>(military::leader_tag(0)));
	EXPECT_EQ(66.0f, test_state.get<military_leader::reconnaissance>(military::leader_tag(0)));
	EXPECT_EQ(77.0f, test_state.get<military_leader::speed>(military::leader_tag(0)));
	EXPECT_EQ(88.0f, test_state.get<military_leader::experience>(military::leader_tag(0)));
	EXPECT_EQ(22.0f, test_state.get<military_leader::reliability>(military::leader_tag(0)));
}



TEST(military_tests, army_creation_no_hq) {
	test_ws<world_state> test_state;

	auto pre_a = count_new_armies;

	auto res = internal_make_army<test_ws<world_state>, fake_new_amry>(test_state, nations::country_tag(4), provinces::province_tag(2));

	EXPECT_NE(military::army_tag(), res);
	EXPECT_EQ(pre_a + 1, count_new_armies);
	EXPECT_EQ(test_state.get<army::location>(res), provinces::province_tag(2));
	EXPECT_EQ(test_state.get<army::owner>(res), nations::country_tag(4));
	EXPECT_TRUE(test_state.contains_item(test_state.get<nation::armies>(nations::country_tag(4)), res));
	EXPECT_EQ(military::strategic_hq_tag(), test_state.get<army::hq>(res));
}

TEST(military_tests, army_creation_with_hq) {
	test_ws<world_state> test_state;

	test_state.set<province_state::strat_hq>(provinces::province_tag(2), military::strategic_hq_tag(3));

	auto pre_a = count_new_armies;
	auto res = internal_make_army<test_ws<world_state>, fake_new_amry>(test_state, nations::country_tag(4), provinces::province_tag(2));

	EXPECT_NE(military::army_tag(), res);
	EXPECT_EQ(pre_a + 1, count_new_armies);
	EXPECT_EQ(test_state.get<army::location>(res), provinces::province_tag(2));
	EXPECT_EQ(test_state.get<army::owner>(res), nations::country_tag(4));
	EXPECT_TRUE(test_state.contains_item(test_state.get<nation::armies>(nations::country_tag(4)), res));
	EXPECT_EQ(military::strategic_hq_tag(3), test_state.get<army::hq>(res));
	EXPECT_TRUE(test_state.contains_item(test_state.get<strategic_hq::army_set>(military::strategic_hq_tag(3)), res));
}
