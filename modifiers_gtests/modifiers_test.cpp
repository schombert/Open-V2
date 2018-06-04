#include "modifiers\\modifiers.h"
#include "gtest\\gtest.h"
#include "fake_fs\\fake_fs.h"
#include "scenario\\scenario.h"
#include "triggers\\codes.h"

#define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

class preparse_test_files {
public:
	directory_representation f_root = directory_representation(u"F:");
	directory_representation common = directory_representation(u"common", f_root);
	file_representation nc = file_representation(u"nationalvalues.txt", common,
		"nnv_order = {\r\n"
		"	mobilisation_size = 0.04\r\n"
		"	mobilisation_economy_impact = 1.0\r\n"
		"}\r\n"
		"\r\n"
		"nv_liberty = {\r\n"
		"	mobilisation_size = 0.02\r\n"
		"	mobilisation_economy_impact = 0.75\r\n"
		"}\r\n"
		"\r\n"
		"nv_equality = {\r\n"
		"	mobilisation_size = 0.06\r\n"
		"	mobilisation_economy_impact = 1.25\r\n"
		"}");
	file_representation crime = file_representation(u"crime.txt", common,
		"crime_a = { active = yes icon = 4 boost_strongest_party = 5.0}\r\n"
		"crime_b = { number_of_voters = 1.0 icon = 6 trigger = { life_rating > 10} }\r\n");

	preparse_test_files() {
		set_default_root(f_root);
	}
};

using namespace modifiers;

TEST(modifiers_tests, crimes_preparse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	modifiers_manager m;
	text_data::text_sequences tex;
	parsing_state state(tex, m);

	pre_parse_crimes(state, f.get_root());

	EXPECT_EQ(2ui64, m.provincial_modifiers.size());
	EXPECT_EQ(2ui64, m.named_provincial_modifiers_index.size());

	EXPECT_EQ(provincial_modifier_tag(0), m.provincial_modifiers[provincial_modifier_tag(0)].id);
	EXPECT_EQ(provincial_modifier_tag(1), m.provincial_modifiers[provincial_modifier_tag(1)].id);
	EXPECT_EQ(provincial_modifier_tag(0), m.named_provincial_modifiers_index[m.provincial_modifiers[provincial_modifier_tag(0)].name]);
	EXPECT_EQ(provincial_modifier_tag(1), m.named_provincial_modifiers_index[m.provincial_modifiers[provincial_modifier_tag(1)].name]);
}

TEST(modifiers_tests, crimes_parse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	scenario::scenario_manager s;

	parsing_state state(s.gui_m.text_data_sequences, s.modifiers_m);

	pre_parse_crimes(state, f.get_root());
	read_crimes(state, s);

	EXPECT_EQ(2ui64, s.modifiers_m.provincial_modifiers.size());
	EXPECT_EQ(2ui64, s.modifiers_m.named_provincial_modifiers_index.size());

	EXPECT_EQ(provincial_modifier_tag(0), s.modifiers_m.provincial_modifiers[provincial_modifier_tag(0)].id);
	EXPECT_EQ(provincial_modifier_tag(1), s.modifiers_m.provincial_modifiers[provincial_modifier_tag(1)].id);
	EXPECT_EQ(provincial_modifier_tag(0), s.modifiers_m.named_provincial_modifiers_index[s.modifiers_m.provincial_modifiers[provincial_modifier_tag(0)].name]);
	EXPECT_EQ(provincial_modifier_tag(1), s.modifiers_m.named_provincial_modifiers_index[s.modifiers_m.provincial_modifiers[provincial_modifier_tag(1)].name]);

	EXPECT_EQ(5.0f, s.modifiers_m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::boost_strongest_party));
	EXPECT_EQ(1.0f, s.modifiers_m.provincial_modifier_definitions.get(provincial_modifier_tag(1), provincial_offsets::number_of_voters));
	EXPECT_EQ(4ui32, s.modifiers_m.provincial_modifiers[provincial_modifier_tag(0)].icon);
	EXPECT_EQ(6ui32, s.modifiers_m.provincial_modifiers[provincial_modifier_tag(1)].icon);
	EXPECT_EQ(true, s.modifiers_m.crimes[provincial_modifier_tag(0)].default_active);
	EXPECT_EQ(false, s.modifiers_m.crimes[provincial_modifier_tag(1)].default_active);
	EXPECT_EQ(triggers::trigger_tag(), s.modifiers_m.crimes[provincial_modifier_tag(0)].crime_trigger);
	EXPECT_NE(triggers::trigger_tag(), s.modifiers_m.crimes[provincial_modifier_tag(1)].crime_trigger);
}

TEST(modifiers_tests, nv_parse) {
	preparse_test_files real_fs;
	file_system f;

	f.set_root(RANGE(u"F:"));

	modifiers_manager m; 
	text_data::text_sequences tex;
	parsing_state state(tex, m);

	parse_national_values(state, f.get_root());

	EXPECT_EQ(3ui64, m.national_modifiers.size());
	EXPECT_EQ(3ui64, m.named_national_modifiers_index.size());

	EXPECT_EQ(national_modifier_tag(0), m.national_modifiers[national_modifier_tag(0)].id);
	EXPECT_EQ(national_modifier_tag(1), m.national_modifiers[national_modifier_tag(1)].id);
	EXPECT_EQ(national_modifier_tag(2), m.national_modifiers[national_modifier_tag(2)].id);
	EXPECT_EQ(national_modifier_tag(0), m.named_national_modifiers_index[m.national_modifiers[national_modifier_tag(0)].name]);
	EXPECT_EQ(national_modifier_tag(1), m.named_national_modifiers_index[m.national_modifiers[national_modifier_tag(1)].name]);
	EXPECT_EQ(national_modifier_tag(2), m.named_national_modifiers_index[m.national_modifiers[national_modifier_tag(2)].name]);
	EXPECT_EQ(0.75f, m.national_modifier_definitions.get(national_modifier_tag(1), modifiers::national_offsets::mobilisation_economy_impact));
}

TEST(modifiers_tests, pack_unpack_offsets) {
	const auto v1 = pack_offset_pair(1, 3);
	EXPECT_EQ(1ui32, get_provincial_offset_from_packed(v1));
	EXPECT_EQ(3ui32, get_national_offset_from_packed(v1));
	const auto v2 = pack_offset_pair(bad_offset, 0);
	EXPECT_EQ(static_cast<uint32_t>(bad_offset), get_provincial_offset_from_packed(v2));
	EXPECT_EQ(0ui32, get_national_offset_from_packed(v2));
}

TEST(modifiers_tests, attribute_from_string) {
	{
		const char s[] = "supply_limit";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::supply_limit, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "attrition";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::attrition, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "max_attrition";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::max_attrition, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "war_exhaustion";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::war_exhaustion, v.second);
	}
	{
		const char s[] = "max_war_exhaustion";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::max_war_exhaustion, v.second);
	}
	{
		const char s[] = "leadership";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::leadership, v.second);
	}
	{
		const char s[] = "leadership_modifier";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::leadership_modifier, v.second);
	}
	{
		const char s[] = "supply_consumption";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::supply_consumption, v.second);
	}
	{
		const char s[] = "org_regain";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::org_regain, v.second);
	}
	{
		const char s[] = "reinforce_speed";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::reinforce_speed, v.second);
	}
	{
		const char s[] = "land_organisation";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::land_organisation, v.second);
	}
	{
		const char s[] = "naval_organisation";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::naval_organisation, v.second);
	}
	{
		const char s[] = "research_points";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::research_points, v.second);
	}
	{
		const char s[] = "research_points_modifier";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::research_points_modifier, v.second);
	}
	{
		const char s[] = "research_points_on_conquer";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::research_points_on_conquer, v.second);
	}
	{
		const char s[] = "debt_default_to";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "import_cost";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::import_cost, v.second);
	}
	{
		const char s[] = "loan_interest";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::loan_interest, v.second);
	}
	{
		const char s[] = "tax_efficiency";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::tax_efficiency, v.second);
	}
	{
		const char s[] = "min_tax";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::min_tax, v.second);
	}
	{
		const char s[] = "max_tax";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::max_tax, v.second);
	}
	{
		const char s[] = "min_military_spending";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::min_military_spending, v.second);
	}
	{
		const char s[] = "max_military_spending";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::max_military_spending, v.second);
	}
	{
		const char s[] = "min_social_spending";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::min_social_spending, v.second);
	}
	{
		const char s[] = "max_social_spending";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::max_social_spending, v.second);
	}
	{
		const char s[] = "factory_owner_cost";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::factory_owner_cost, v.second);
	}
	{
		const char s[] = "min_tariff";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::min_tariff, v.second);
	}
	{
		const char s[] = "max_tariff";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::max_tariff, v.second);
	}
	{
		const char s[] = "ruling_party_support";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::ruling_party_support, v.second);
	}
	{
		const char s[] = "local_ruling_party_support";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_ruling_party_support, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "rich_vote";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::rich_vote, v.second);
	}
	{
		const char s[] = "middle_vote";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::middle_vote, v.second);
	}
	{
		const char s[] = "poor_vote";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::poor_vote, v.second);
	}
	{
		const char s[] = "minimum_wage";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::minimum_wage, v.second);
	}
	{
		const char s[] = "factory_maintenance";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::factory_maintenance, v.second);
	}
	{
		const char s[] = "poor_life_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::poor_life_needs, v.first);
		EXPECT_EQ(national_offsets::poor_life_needs, v.second);
	}
	{
		const char s[] = "rich_life_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::rich_life_needs, v.first);
		EXPECT_EQ(national_offsets::rich_life_needs, v.second);
	}
	{
		const char s[] = "middle_life_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::middle_life_needs, v.first);
		EXPECT_EQ(national_offsets::middle_life_needs, v.second);
	}
	{
		const char s[] = "poor_everyday_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::poor_everyday_needs, v.first);
		EXPECT_EQ(national_offsets::poor_everyday_needs, v.second);
	}
	{
		const char s[] = "rich_everyday_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::rich_everyday_needs, v.first);
		EXPECT_EQ(national_offsets::rich_everyday_needs, v.second);
	}
	{
		const char s[] = "middle_everyday_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::middle_everyday_needs, v.first);
		EXPECT_EQ(national_offsets::middle_everyday_needs, v.second);
	}
	{
		const char s[] = "poor_luxury_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::poor_luxury_needs, v.first);
		EXPECT_EQ(national_offsets::poor_luxury_needs, v.second);
	}
	{
		const char s[] = "middle_luxury_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::middle_luxury_needs, v.first);
		EXPECT_EQ(national_offsets::middle_luxury_needs, v.second);
	}
	{
		const char s[] = "rich_luxury_needs";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::rich_luxury_needs, v.first);
		EXPECT_EQ(national_offsets::rich_luxury_needs, v.second);
	}
	{
		const char s[] = "unemployment_benefit";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::unemployment_benefit, v.second);
	}
	{
		const char s[] = "pension_level";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::pension_level, v.second);
	}
	{
		const char s[] = "population_growth";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::population_growth, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "global_population_growth";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::global_population_growth, v.second);
	}
	{
		const char s[] = "factory_input";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::factory_input, v.second);
	}
	{
		const char s[] = "factory_output";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::factory_output, v.second);
	}
	{
		const char s[] = "factory_throughput";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::factory_throughput, v.second);
	}
	{
		const char s[] = "rgo_input";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::rgo_input, v.second);
	}
	{
		const char s[] = "rgo_output";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::rgo_output, v.second);
	}
	{
		const char s[] = "rgo_throughput";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::rgo_throughput, v.second);
	}
	{
		const char s[] = "artisan_input";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::artisan_input, v.second);
	}
	{
		const char s[] = "artisan_output";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::artisan_output, v.second);
	}
	{
		const char s[] = "artisan_throughput";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::artisan_throughput, v.second);
	}
	{
		const char s[] = "local_factory_input";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_factory_input, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_factory_output";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_factory_output, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_factory_throughput";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_factory_throughput, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_rgo_input";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_rgo_input, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_rgo_output";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_rgo_output, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_rgo_throughput";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_rgo_throughput, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_artisan_input";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_artisan_input, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_artisan_output";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_artisan_output, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "local_artisan_throughput";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::local_artisan_throughput, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "number_of_voters";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::number_of_voters, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "goods_demand";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::goods_demand, v.first);
		EXPECT_EQ(national_offsets::goods_demand, v.second);
	}
	{
		const char s[] = "badboy";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::badboy, v.second);
	}
	{
		const char s[] = "assimilation_rate";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::assimilation_rate, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "global_assimilation_rate";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::global_assimilation_rate, v.second);
	}
	{
		const char s[] = "prestige";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::prestige, v.second);
	}
	{
		const char s[] = "factory_cost";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::factory_cost, v.second);
	}
	{
		const char s[] = "life_rating";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::life_rating, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "farm_rgo_eff";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::farm_rgo_eff, v.first);
		EXPECT_EQ(national_offsets::farm_rgo_eff, v.second);
	}
	{
		const char s[] = "mine_rgo_eff";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::mine_rgo_eff, v.first);
		EXPECT_EQ(national_offsets::mine_rgo_eff, v.second);
	}
	{
		const char s[] = "farm_rgo_size";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::farm_rgo_size, v.first);
		EXPECT_EQ(national_offsets::farm_rgo_size, v.second);
	}
	{
		const char s[] = "mine_rgo_size";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::mine_rgo_size, v.first);
		EXPECT_EQ(national_offsets::mine_rgo_size, v.second);
	}
	{
		const char s[] = "issue_change_speed";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::issue_change_speed, v.second);
	}
	{
		const char s[] = "social_reform_desire";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::social_reform_desire, v.second);
	}
	{
		const char s[] = "political_reform_desire";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::political_reform_desire, v.second);
	}
	{
		const char s[] = "literacy_con_impact";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(bad_offset, v.first);
		EXPECT_EQ(national_offsets::literacy_con_impact, v.second);
	}
	{
		const char s[] = "pop_militancy_modifier";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::pop_militancy_modifier, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "pop_consciousness_modifier";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::pop_consciousness_modifier, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
	{
		const char s[] = "rich_income_modifier";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::rich_income_modifier, v.first);
		EXPECT_EQ(national_offsets::rich_income_modifier, v.second);
	}
	{
		const char s[] = "middle_income_modifier";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::middle_income_modifier, v.first);
		EXPECT_EQ(national_offsets::middle_income_modifier, v.second);
	}
	{
		const char s[] = "poor_income_modifier";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::poor_income_modifier, v.first);
		EXPECT_EQ(national_offsets::poor_income_modifier, v.second);
	}
	{
		const char s[] = "boost_strongest_party";
		const auto v = get_provincial_and_national_offsets_from_token(RANGE(s));
		EXPECT_EQ(provincial_offsets::boost_strongest_party, v.first);
		EXPECT_EQ(bad_offset, v.second);
	}
}

TEST(modifiers_tests, modifier_reader) {
	const char ta[] = "global_immigrant_attract";
	token_and_type t1{RANGE(ta), token_type::unknown };

	const char tb[] = "immigrant_push";
	token_and_type t2{ RANGE(tb), token_type::unknown };

	const char tc[] = "rich_income_modifier";
	token_and_type t3{ RANGE(tc), token_type::unknown };

	const char td[] = "bad_value";
	token_and_type t4{ RANGE(td), token_type::unknown };

	{
		modifier_reading_base b;
		EXPECT_EQ(static_cast<size_t>(provincial_offsets::aligned_32_size + national_offsets::aligned_32_size), b.modifier_data.size());

		EXPECT_EQ(0ui32, b.count_unique_national);
		EXPECT_EQ(0ui32, b.count_unique_provincial);
		EXPECT_EQ(0ui32, b.total_attributes);

		b.add_attribute(std::make_pair(t1, 1.0f));

		EXPECT_EQ(1ui32, b.count_unique_national);
		EXPECT_EQ(0ui32, b.count_unique_provincial);
		EXPECT_EQ(1ui32, b.total_attributes);

		b.add_attribute(std::make_pair(t3, 2.0f));

		EXPECT_EQ(1ui32, b.count_unique_national);
		EXPECT_EQ(0ui32, b.count_unique_provincial);
		EXPECT_EQ(2ui32, b.total_attributes);

		EXPECT_EQ(2.0f, b.modifier_data[provincial_offsets::rich_income_modifier]);
		EXPECT_EQ(2.0f, b.modifier_data[provincial_offsets::aligned_32_size + national_offsets::rich_income_modifier]);
		EXPECT_EQ(1.0f, b.modifier_data[provincial_offsets::aligned_32_size + national_offsets::global_immigrant_attract]);
	}

	{
		modifier_reading_base b;

		b.add_attribute(std::make_pair(t2, 1.0f));

		EXPECT_EQ(0ui32, b.count_unique_national);
		EXPECT_EQ(1ui32, b.count_unique_provincial);
		EXPECT_EQ(1ui32, b.total_attributes);

		b.add_attribute(std::make_pair(t1, 2.0f));

		EXPECT_EQ(1ui32, b.count_unique_national);
		EXPECT_EQ(1ui32, b.count_unique_provincial);
		EXPECT_EQ(2ui32, b.total_attributes);

		b.add_attribute(std::make_pair(t4, 10.0f));

		EXPECT_EQ(1ui32, b.count_unique_national);
		EXPECT_EQ(1ui32, b.count_unique_provincial);
		EXPECT_EQ(2ui32, b.total_attributes);

		EXPECT_EQ(1.0f, b.modifier_data[provincial_offsets::immigrant_push]);
		EXPECT_EQ(2.0f, b.modifier_data[provincial_offsets::aligned_32_size + national_offsets::global_immigrant_attract]);
	}
}

TEST(modifiers_tests, modifier_adding_a) {
	modifiers_manager m;

	{
		const auto nmt = m.fetch_unique_national_modifier(text_data::text_tag(0));

		EXPECT_EQ(1ui64, m.national_modifiers.size());
		EXPECT_EQ(0ui64, m.provincial_modifiers.size());
		EXPECT_EQ(1ui64, m.named_national_modifiers_index.size());
		EXPECT_EQ(0ui64, m.named_provincial_modifiers_index.size());

		EXPECT_EQ(nmt, m.national_modifiers[nmt].id);
		EXPECT_EQ(text_data::text_tag(0), m.national_modifiers[nmt].name);
		EXPECT_EQ(nmt, m.named_national_modifiers_index[text_data::text_tag(0)]);

		const auto pmt = m.fetch_unique_provincial_modifier(text_data::text_tag(1));

		EXPECT_EQ(1ui64, m.national_modifiers.size());
		EXPECT_EQ(1ui64, m.provincial_modifiers.size());
		EXPECT_EQ(1ui64, m.named_national_modifiers_index.size());
		EXPECT_EQ(1ui64, m.named_provincial_modifiers_index.size());

		EXPECT_EQ(pmt, m.provincial_modifiers[pmt].id);
		EXPECT_EQ(national_modifier_tag(), m.provincial_modifiers[pmt].complement);
		EXPECT_EQ(text_data::text_tag(1), m.provincial_modifiers[pmt].name);
		EXPECT_EQ(pmt, m.named_provincial_modifiers_index[text_data::text_tag(1)]);
	}
	{
		const auto nmt = m.fetch_unique_national_modifier(text_data::text_tag());

		EXPECT_EQ(2ui64, m.national_modifiers.size());
		EXPECT_EQ(1ui64, m.provincial_modifiers.size());
		EXPECT_EQ(1ui64, m.named_national_modifiers_index.size());
		EXPECT_EQ(1ui64, m.named_provincial_modifiers_index.size());

		EXPECT_EQ(nmt, m.national_modifiers[nmt].id);
		EXPECT_EQ(text_data::text_tag(), m.national_modifiers[nmt].name);

		const auto pmt = m.fetch_unique_provincial_modifier(text_data::text_tag());

		EXPECT_EQ(2ui64, m.national_modifiers.size());
		EXPECT_EQ(2ui64, m.provincial_modifiers.size());
		EXPECT_EQ(1ui64, m.named_national_modifiers_index.size());
		EXPECT_EQ(1ui64, m.named_provincial_modifiers_index.size());

		EXPECT_EQ(pmt, m.provincial_modifiers[pmt].id);
		EXPECT_EQ(national_modifier_tag(), m.provincial_modifiers[pmt].complement);
		EXPECT_EQ(text_data::text_tag(), m.provincial_modifiers[pmt].name);
	}
}

TEST(modifiers_tests, modifier_adding_b) {
	const char ta[] = "global_immigrant_attract";
	token_and_type t1{ RANGE(ta), token_type::unknown };

	const char tb[] = "immigrant_push";
	token_and_type t2{ RANGE(tb), token_type::unknown };

	const char tc[] = "rich_income_modifier";
	token_and_type t3{ RANGE(tc), token_type::unknown };

	{
		modifiers_manager m;

		modifier_reading_base b;
		b.add_attribute(std::make_pair(t2, 2.0f));
		b.add_attribute(std::make_pair(t3, 5.0f));

		const auto tag = add_provincial_modifier(text_data::text_tag(0), b, m);

		EXPECT_EQ(provincial_modifier_tag(0), tag);
		EXPECT_EQ(1ui64, m.provincial_modifiers.size());
		EXPECT_EQ(0ui64, m.national_modifiers.size());
		EXPECT_EQ(1ui64, m.named_provincial_modifiers_index.size());
		EXPECT_EQ(0ui64, m.named_national_modifiers_index.size());

		EXPECT_EQ(0ui64, m.national_modifier_definitions.outer_size());
		EXPECT_EQ(1ui64, m.provincial_modifier_definitions.outer_size());
		EXPECT_EQ(2.0f, m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::immigrant_push));
		EXPECT_EQ(5.0f, m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::rich_income_modifier));
	}

	{
		modifiers_manager m;

		modifier_reading_base b;
		b.add_attribute(std::make_pair(t2, 2.0f));
		b.add_attribute(std::make_pair(t3, 5.0f));
		b.add_attribute(std::make_pair(t1, 1.0f));

		add_provincial_modifier(text_data::text_tag(0), b, m);
		EXPECT_EQ(1ui64, m.provincial_modifiers.size());
		EXPECT_EQ(1ui64, m.national_modifiers.size());
		EXPECT_EQ(1ui64, m.named_provincial_modifiers_index.size());
		EXPECT_EQ(0ui64, m.named_national_modifiers_index.size());

		EXPECT_EQ(1ui64, m.national_modifier_definitions.outer_size());
		EXPECT_EQ(1ui64, m.provincial_modifier_definitions.outer_size());

		EXPECT_EQ(2.0f, m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::immigrant_push));
		EXPECT_EQ(5.0f, m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::rich_income_modifier));

		EXPECT_EQ(1.0f, m.national_modifier_definitions.get(national_modifier_tag(0), national_offsets::global_immigrant_attract));
		EXPECT_EQ(0.0f, m.national_modifier_definitions.get(national_modifier_tag(0), national_offsets::rich_income_modifier));

		EXPECT_EQ(national_modifier_tag(0), m.provincial_modifiers[provincial_modifier_tag(0)].complement);
		EXPECT_EQ(text_data::text_tag(0), m.national_modifiers[national_modifier_tag(0)].name);
	}

	{
		modifiers_manager m;

		modifier_reading_base b;
		b.add_attribute(std::make_pair(t2, 2.0f));
		b.add_attribute(std::make_pair(t3, 5.0f));
		b.add_attribute(std::make_pair(t1, 1.0f));

		add_national_modifier(text_data::text_tag(0), b, m);
		EXPECT_EQ(0ui64, m.provincial_modifiers.size());
		EXPECT_EQ(1ui64, m.national_modifiers.size());
		EXPECT_EQ(0ui64, m.named_provincial_modifiers_index.size());
		EXPECT_EQ(1ui64, m.named_national_modifiers_index.size());

		EXPECT_EQ(1ui64, m.national_modifier_definitions.outer_size());
		EXPECT_EQ(0ui64, m.provincial_modifier_definitions.outer_size());

		EXPECT_EQ(1.0f, m.national_modifier_definitions.get(national_modifier_tag(0), national_offsets::global_immigrant_attract));
		EXPECT_EQ(5.0f, m.national_modifier_definitions.get(national_modifier_tag(0), national_offsets::rich_income_modifier));
	}
}

TEST(modifiers_tests, modifier_adding_indeterminate) {
	const char ta[] = "global_immigrant_attract";
	token_and_type t1{ RANGE(ta), token_type::unknown };

	const char tb[] = "immigrant_push";
	token_and_type t2{ RANGE(tb), token_type::unknown };

	const char tc[] = "rich_income_modifier";
	token_and_type t3{ RANGE(tc), token_type::unknown };

	{
		modifiers_manager m;

		modifier_reading_base b;
		b.add_attribute(std::make_pair(t2, 2.0f));
		b.add_attribute(std::make_pair(t3, 5.0f));

		add_indeterminate_modifier(text_data::text_tag(0), b, m);

		EXPECT_EQ(1ui64, m.provincial_modifiers.size());
		EXPECT_EQ(0ui64, m.national_modifiers.size());
		EXPECT_EQ(1ui64, m.named_provincial_modifiers_index.size());
		EXPECT_EQ(0ui64, m.named_national_modifiers_index.size());

		EXPECT_EQ(0ui64, m.national_modifier_definitions.outer_size());
		EXPECT_EQ(1ui64, m.provincial_modifier_definitions.outer_size());
		EXPECT_EQ(2.0f, m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::immigrant_push));
		EXPECT_EQ(5.0f, m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::rich_income_modifier));
	}

	{
		modifiers_manager m;

		modifier_reading_base b;
		b.add_attribute(std::make_pair(t1, 1.0f));

		add_indeterminate_modifier(text_data::text_tag(0), b, m);

		EXPECT_EQ(0ui64, m.provincial_modifiers.size());
		EXPECT_EQ(1ui64, m.national_modifiers.size());
		EXPECT_EQ(0ui64, m.named_provincial_modifiers_index.size());
		EXPECT_EQ(1ui64, m.named_national_modifiers_index.size());

		EXPECT_EQ(1ui64, m.national_modifier_definitions.outer_size());
		EXPECT_EQ(0ui64, m.provincial_modifier_definitions.outer_size());

		EXPECT_EQ(1.0f, m.national_modifier_definitions.get(national_modifier_tag(0), national_offsets::global_immigrant_attract));
	}

	{
		modifiers_manager m;

		modifier_reading_base b;
		b.add_attribute(std::make_pair(t3, 5.0f)); 
		b.add_attribute(std::make_pair(t1, 1.0f));

		add_indeterminate_modifier(text_data::text_tag(0), b, m);
		EXPECT_EQ(1ui64, m.provincial_modifiers.size());
		EXPECT_EQ(2ui64, m.national_modifiers.size());
		EXPECT_EQ(1ui64, m.named_provincial_modifiers_index.size());
		EXPECT_EQ(1ui64, m.named_national_modifiers_index.size());

		EXPECT_EQ(2ui64, m.national_modifier_definitions.outer_size());
		EXPECT_EQ(1ui64, m.provincial_modifier_definitions.outer_size());

		EXPECT_EQ(1.0f, m.national_modifier_definitions.get(national_modifier_tag(0), national_offsets::global_immigrant_attract));
		EXPECT_EQ(5.0f, m.national_modifier_definitions.get(national_modifier_tag(0), national_offsets::rich_income_modifier));

		EXPECT_EQ(5.0f, m.provincial_modifier_definitions.get(provincial_modifier_tag(0), provincial_offsets::rich_income_modifier));
		EXPECT_EQ(1.0f, m.national_modifier_definitions.get(national_modifier_tag(1), national_offsets::global_immigrant_attract));
		EXPECT_EQ(0.0f, m.national_modifier_definitions.get(national_modifier_tag(1), national_offsets::rich_income_modifier));

		EXPECT_EQ(national_modifier_tag(1), m.provincial_modifiers[provincial_modifier_tag(0)].complement);
	}
}

TEST(modifiers_tests, commit_factor) {
	modifiers_manager m;
	EXPECT_EQ(0ui64, m.factor_data.size());

	const auto etag = commit_factor(m, std::vector<factor_segment>());
	EXPECT_EQ(0ui64, m.factor_data.size());
	EXPECT_EQ(0ui16, etag.first);
	EXPECT_EQ(false, etag.second);

	std::vector<factor_segment> a{
		factor_segment{0.0f, triggers::trigger_tag()},
		factor_segment{1.0f, triggers::trigger_tag(0)},
		factor_segment{10.0f, triggers::trigger_tag(2)}
	};

	const auto atag = commit_factor(m, a);
	EXPECT_EQ(3ui64, m.factor_data.size());
	EXPECT_EQ(0ui16, atag.first);
	EXPECT_EQ(true, atag.second);

	std::vector<factor_segment> b{ factor_segment{5.0f, triggers::trigger_tag(5)}, factor_segment{50.0f, triggers::trigger_tag(15)} };

	const auto btag = commit_factor(m, b);
	EXPECT_EQ(5ui64, m.factor_data.size());
	EXPECT_EQ(3ui16, btag.first);
	EXPECT_EQ(true, btag.second);

	std::vector<factor_segment> c{
		factor_segment{1.0f, triggers::trigger_tag(0)},
		factor_segment{10.0f, triggers::trigger_tag(2)},
		factor_segment{5.0f, triggers::trigger_tag(5)} };

	const auto ctag = commit_factor(m, c);
	EXPECT_EQ(5ui64, m.factor_data.size());
	EXPECT_EQ(1ui16, ctag.first);
	EXPECT_EQ(false, ctag.second);
}

TEST(modifiers_tests, modifier_factors) {
	scenario::scenario_manager sm;

	{
		const char trigger[] = "factor = 1.5 modifier = { factor = 2 }";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_modifier_factors(
			sm,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::empty,
				triggers::trigger_slot_contents::empty,
				triggers::trigger_slot_contents::empty,
				false },
				5.0f,
				3.0f,
				parse_results.data(),
				parse_results.data() + parse_results.size());

		EXPECT_EQ(factor_tag(0), t_result);
		EXPECT_EQ(3.0f, sm.modifiers_m.factor_modifiers[t_result].base);
		EXPECT_EQ(1.5f, sm.modifiers_m.factor_modifiers[t_result].factor);
		EXPECT_EQ(0ui16, sm.modifiers_m.factor_modifiers[t_result].data_offset);
		EXPECT_EQ(1ui16, sm.modifiers_m.factor_modifiers[t_result].data_length);
		EXPECT_EQ(2.0f, sm.modifiers_m.factor_data[0].factor);
		EXPECT_EQ(triggers::trigger_tag(), sm.modifiers_m.factor_data[0].condition);
	}
	{
		const char trigger[] = "months = 2 base = 2.0 modifier = { factor = 2 year = 1 } modifier = { factor = 4 year = 1 }";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_modifier_factors(
			sm,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::empty,
				triggers::trigger_slot_contents::empty,
				triggers::trigger_slot_contents::empty,
				false },
				5.0f,
				3.0f,
				parse_results.data(),
				parse_results.data() + parse_results.size());

		EXPECT_EQ(factor_tag(1), t_result);
		EXPECT_EQ(2.0f, sm.modifiers_m.factor_modifiers[t_result].base);
		EXPECT_EQ(60.0f, sm.modifiers_m.factor_modifiers[t_result].factor);
		EXPECT_EQ(1ui16, sm.modifiers_m.factor_modifiers[t_result].data_offset);
		EXPECT_EQ(2ui16, sm.modifiers_m.factor_modifiers[t_result].data_length);
		EXPECT_EQ(2.0f, sm.modifiers_m.factor_data[1].factor);
		EXPECT_EQ(triggers::trigger_tag(0), sm.modifiers_m.factor_data[1].condition);
		EXPECT_EQ(4.0f, sm.modifiers_m.factor_data[2].factor);
		EXPECT_EQ(triggers::trigger_tag(0), sm.modifiers_m.factor_data[2].condition);
		EXPECT_EQ(uint16_t(triggers::trigger_codes::year | triggers::trigger_codes::association_ge), sm.trigger_m.trigger_data[0]);
		EXPECT_EQ(2ui16, sm.trigger_m.trigger_data[1]);
		EXPECT_EQ(1ui16, sm.trigger_m.trigger_data[2]);
	}
	{
		const char trigger[] = "years = 1 group = { modifier = { factor = 2 year = 1 } modifier = { factor = 4 state_id = 1 } }";

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, RANGE(trigger));

		auto t_result = parse_modifier_factors(
			sm,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::state,
				triggers::trigger_slot_contents::state,
				triggers::trigger_slot_contents::empty,
				false },
				5.0f,
				3.0f,
				parse_results.data(),
				parse_results.data() + parse_results.size());

		EXPECT_EQ(factor_tag(2), t_result);
		EXPECT_EQ(365.0f, sm.modifiers_m.factor_modifiers[t_result].factor);
		EXPECT_EQ(3ui16, sm.modifiers_m.factor_modifiers[t_result].data_offset);
		EXPECT_EQ(2ui16, sm.modifiers_m.factor_modifiers[t_result].data_length);
		EXPECT_EQ(2.0f, sm.modifiers_m.factor_data[3].factor);
		EXPECT_EQ(triggers::trigger_tag(0), sm.modifiers_m.factor_data[3].condition);
		EXPECT_EQ(4.0f, sm.modifiers_m.factor_data[4].factor);
		EXPECT_EQ(triggers::trigger_tag(3), sm.modifiers_m.factor_data[4].condition);
		EXPECT_EQ(uint16_t(triggers::trigger_codes::state_id_state | triggers::trigger_codes::association_eq), sm.trigger_m.trigger_data[3]);
		EXPECT_EQ(2ui16, sm.trigger_m.trigger_data[4]);
		EXPECT_EQ(1ui16, sm.trigger_m.trigger_data[5]);
	}
}
