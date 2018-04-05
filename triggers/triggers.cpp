#include "triggers.h"
#include "scenario\\scenario.h"
#include <optional>
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "text_classifier\\text_classifiers.h"
#include "simple_mpl\\simple_mpl.hpp"

namespace triggers {

	//for reading simple association, non scope triggers
	namespace codes {
		// non scopes
		constexpr uint16_t year = 0x0001;
		constexpr uint16_t month = 0x0002;
		constexpr uint16_t port = 0x0003;
		constexpr uint16_t rank = 0x0004;
		constexpr uint16_t invention = 0x0005;
		constexpr uint16_t strata = 0x0006;
		constexpr uint16_t life_rating_province = 0x0007;
		constexpr uint16_t life_rating_state = 0x0008;
		constexpr uint16_t has_empty_adjacent_state_province = 0x0009;
		constexpr uint16_t has_empty_adjacent_state_state = 0x000A;
		constexpr uint16_t state_id_province = 0x000B;
		constexpr uint16_t state_id_state = 0x000C;
		constexpr uint16_t cash_reserves = 0x000D;
		constexpr uint16_t unemployment_nation = 0x000E;
		constexpr uint16_t unemployment_state = 0x000F;
		constexpr uint16_t unemployment_province = 0x0010;
		constexpr uint16_t unemployment_pop = 0x0011;
		constexpr uint16_t is_slave_nation = 0x0012;
		constexpr uint16_t is_slave_state = 0x0013;
		constexpr uint16_t is_slave_province = 0x0014;
		constexpr uint16_t is_slave_pop = 0x0015;
		constexpr uint16_t is_independant = 0x0016;
		constexpr uint16_t has_national_minority_province = 0x0017;
		constexpr uint16_t has_national_minority_state = 0x0018;
		constexpr uint16_t has_national_minority_nation = 0x0019;
		constexpr uint16_t government_nation = 0x001A;
		constexpr uint16_t government_pop = 0x001B;
		constexpr uint16_t capital = 0x001C;
		constexpr uint16_t tech_school = 0x001D;
		constexpr uint16_t primary_culture = 0x001E;
		constexpr uint16_t accepted_culture = 0x001F;
		constexpr uint16_t culture_pop = 0x0020;
		constexpr uint16_t culture_state = 0x0021;
		constexpr uint16_t culture_province = 0x0022;
		constexpr uint16_t culture_nation = 0x0023;
		constexpr uint16_t culture_pop_reb = 0x0024;
		constexpr uint16_t culture_state_reb = 0x0025;
		constexpr uint16_t culture_province_reb = 0x0026;
		constexpr uint16_t culture_nation_reb = 0x0027;
		constexpr uint16_t culture_from_nation = 0x0028;
		constexpr uint16_t culture_this_nation = 0x0028;
		constexpr uint16_t culture_this_state = 0x0029;
		constexpr uint16_t culture_this_pop = 0x002A;
		constexpr uint16_t culture_this_province = 0x002B;
		constexpr uint16_t culture_group_nation = 0x002A;
		constexpr uint16_t culture_group_pop = 0x002B;
		constexpr uint16_t culture_group_reb_nation = 0x002C;
		constexpr uint16_t culture_group_reb_pop = 0x002D;
		constexpr uint16_t culture_group_nation_from_nation = 0x002E;
		constexpr uint16_t culture_group_pop_from_nation = 0x002F;
		constexpr uint16_t culture_group_nation_this_nation = 0x0030;
		constexpr uint16_t culture_group_pop_this_nation = 0x0031;
		constexpr uint16_t culture_group_nation_this_province = 0x0032;
		constexpr uint16_t culture_group_pop_this_province = 0x0033;
		constexpr uint16_t culture_group_nation_this_state = 0x0034;
		constexpr uint16_t culture_group_pop_this_state = 0x0035;
		constexpr uint16_t culture_group_nation_this_pop = 0x0036;
		constexpr uint16_t culture_group_pop_this_pop = 0x0037;
		constexpr uint16_t religion = 0x0038;
		constexpr uint16_t religion_reb = 0x0039;
		constexpr uint16_t religion_from_nation = 0x003A;
		constexpr uint16_t religion_this_nation = 0x003B;
		constexpr uint16_t religion_this_state = 0x003C;
		constexpr uint16_t religion_this_province = 0x003D;
		constexpr uint16_t religion_this_pop = 0x003E;
		constexpr uint16_t terrain_province = 0x003F;
		constexpr uint16_t terrain_pop = 0x0040;
		constexpr uint16_t trade_goods = 0x0041;
		constexpr uint16_t is_secondary_power_pop = 0x0042;
		constexpr uint16_t is_secondary_power_nation = 0x0043;
		constexpr uint16_t has_faction_nation = 0x0044;
		constexpr uint16_t has_faction_pop = 0x0045;
		constexpr uint16_t has_unclaimed_cores = 0x0046;
		constexpr uint16_t is_cultural_union_bool = 0x0047;
		constexpr uint16_t is_cultural_union_this_self_pop = 0x0048;
		constexpr uint16_t is_cultural_union_this_pop = 0x0049;
		constexpr uint16_t is_cultural_union_this_state = 0x004A;
		constexpr uint16_t is_cultural_union_this_province = 0x004B;
		constexpr uint16_t is_cultural_union_this_nation = 0x004C;
		constexpr uint16_t is_cultural_union_this_rebel = 0x004D;
		constexpr uint16_t is_cultural_union_tag_nation = 0x004E;
		constexpr uint16_t is_cultural_union_tag_this_pop = 0x004F;
		constexpr uint16_t is_cultural_union_tag_this_state = 0x0050;
		constexpr uint16_t is_cultural_union_tag_this_province = 0x0051;
		constexpr uint16_t is_cultural_union_tag_this_nation = 0x0052;
		constexpr uint16_t can_build_factory = 0x0053;
		constexpr uint16_t war_pop = 0x0054;
		constexpr uint16_t war_nation = 0x0055;
		constexpr uint16_t war_exhaustion = 0x0056;
		constexpr uint16_t blockade = 0x0057;
		constexpr uint16_t owns = 0x0058;
		constexpr uint16_t controls = 0x0059;
		constexpr uint16_t is_core_integer = 0x005A;
		constexpr uint16_t is_core_this_nation = 0x005B;
		constexpr uint16_t is_core_this_state = 0x005C;
		constexpr uint16_t is_core_this_province = 0x005D;
		constexpr uint16_t is_core_this_pop = 0x005E;
		constexpr uint16_t is_core_from_nation = 0x005F;
		constexpr uint16_t is_core_reb = 0x0060;
		constexpr uint16_t is_core_tag = 0x0061;
		constexpr uint16_t num_of_revolts = 0x0062;
		constexpr uint16_t revolt_percentage = 0x0063;
		constexpr uint16_t num_of_cities_int = 0x0064;
		constexpr uint16_t num_of_cities_from_nation = 0x0065;
		constexpr uint16_t num_of_cities_this_nation = 0x0066;
		constexpr uint16_t num_of_cities_this_state = 0x0067;
		constexpr uint16_t num_of_cities_this_province = 0x0068;
		constexpr uint16_t num_of_cities_this_pop = 0x0069;
		constexpr uint16_t num_of_ports = 0x006A;
		constexpr uint16_t num_of_allies = 0x006B;
		constexpr uint16_t num_of_vassals = 0x006C;
		constexpr uint16_t owned_by_tag = 0x006D;
		constexpr uint16_t owned_by_from_nation = 0x006E;
		constexpr uint16_t owned_by_this_nation = 0x006F;
		constexpr uint16_t owned_by_this_province = 0x0070;
		constexpr uint16_t owned_by_this_state = 0x0071;
		constexpr uint16_t owned_by_this_pop = 0x0072;
		constexpr uint16_t exists_bool = 0x0073;
		constexpr uint16_t exists_tag = 0x0074;
		constexpr uint16_t has_country_flag = 0x0075;
		constexpr uint16_t continent_nation = 0x0076;
		constexpr uint16_t continent_state = 0x0077;
		constexpr uint16_t continent_province = 0x0078;
		constexpr uint16_t continent_pop = 0x0079;
		constexpr uint16_t continent_nation_this = 0x007A;
		constexpr uint16_t continent_state_this = 0x007B;
		constexpr uint16_t continent_province_this = 0x007C;
		constexpr uint16_t continent_pop_this = 0x007D;
		constexpr uint16_t continent_nation_from = 0x007E;
		constexpr uint16_t continent_state_from = 0x007F;
		constexpr uint16_t continent_province_from = 0x0080;
		constexpr uint16_t continent_pop_from = 0x0081;
		constexpr uint16_t casus_belli_tag = 0x0082;
		constexpr uint16_t casus_belli_from = 0x0083;
		constexpr uint16_t casus_belli_this_nation = 0x0084;
		constexpr uint16_t casus_belli_this_state = 0x0085;
		constexpr uint16_t casus_belli_this_province = 0x0086;
		constexpr uint16_t casus_belli_this_pop = 0x0087;
		constexpr uint16_t military_access_tag = 0x0088;
		constexpr uint16_t military_access_from = 0x0089;
		constexpr uint16_t military_access_this_nation = 0x008A;
		constexpr uint16_t military_access_this_state = 0x008B;
		constexpr uint16_t military_access_this_province = 0x008C;
		constexpr uint16_t military_access_this_pop = 0x008D;
		constexpr uint16_t prestige_value = 0x008E;
		constexpr uint16_t prestige_from = 0x008F;
		constexpr uint16_t prestige_this_nation = 0x0090;
		constexpr uint16_t prestige_this_state = 0x0091;
		constexpr uint16_t prestige_this_province = 0x0092;
		constexpr uint16_t prestige_this_pop = 0x0093;
		constexpr uint16_t badboy = 0x0094;
		constexpr uint16_t has_building_state = 0x0095;
		constexpr uint16_t has_building_fort = 0x0096;
		constexpr uint16_t has_building_railroad = 0x0097;
		constexpr uint16_t has_building_naval_base = 0x0098;
		constexpr uint16_t empty = 0x0099;
		constexpr uint16_t is_blockaded = 0x009A;
		constexpr uint16_t has_country_modifier = 0x009B;
		constexpr uint16_t has_province_modifier = 0x009C;
		constexpr uint16_t region = 0x009D;
		constexpr uint16_t tag_tag = 0x009E;
		constexpr uint16_t tag_this_nation = 0x009F;
		constexpr uint16_t tag_this_province = 0x00A0;
		constexpr uint16_t tag_from_nation = 0x00A1;
		constexpr uint16_t tag_from_province = 0x00A2;
		constexpr uint16_t neighbour_tag = 0x00A3;
		constexpr uint16_t neighbour_this = 0x00A4;
		constexpr uint16_t neighbour_from = 0x00A5;
		constexpr uint16_t units_in_province_value = 0x00A6;
		constexpr uint16_t units_in_province_from = 0x00A7;
		constexpr uint16_t units_in_province_this_nation = 0x00A8;
		constexpr uint16_t units_in_province_this_province = 0x00A9;
		constexpr uint16_t units_in_province_this_state = 0x00AA;
		constexpr uint16_t units_in_province_this_pop = 0x00AB;
		constexpr uint16_t war_with_tag = 0x00AC;
		constexpr uint16_t war_with_from = 0x00AD;
		constexpr uint16_t war_with_this_nation = 0x00AE;
		constexpr uint16_t war_with_this_province = 0x00AF;
		constexpr uint16_t war_with_this_state = 0x00B0;
		constexpr uint16_t war_with_this_pop = 0x00B1;
		constexpr uint16_t unit_in_battle = 0x00B2;
		constexpr uint16_t total_amount_of_divisions = 0x00B3;
		constexpr uint16_t money = 0x00B4;
		constexpr uint16_t lost_national = 0x00B5;
		constexpr uint16_t is_vassal = 0x00B6;
		constexpr uint16_t ruling_party_ideology_pop = 0x00B7;
		constexpr uint16_t ruling_party_ideology_nation = 0x00B8;
		constexpr uint16_t ruling_party = 0x00B9;
		constexpr uint16_t is_ideology_enabled = 0x00BA;
		constexpr uint16_t political_reform_want_nation = 0x00BB;
		constexpr uint16_t political_reform_want_pop = 0x00BC;
		constexpr uint16_t social_reform_want_nation = 0x00BD;
		constexpr uint16_t social_reform_want_pop = 0x00BE;
		constexpr uint16_t total_amount_of_ships = 0x00BF;
		constexpr uint16_t plurality = 0x00C0;
		constexpr uint16_t corruption = 0x00C1;
		constexpr uint16_t is_state_religion_pop = 0x00C2;
		constexpr uint16_t is_state_religion_province = 0x00C3;
		constexpr uint16_t is_state_religion_state = 0x00C4;
		constexpr uint16_t is_primary_culture_pop = 0x00C5;
		constexpr uint16_t is_primary_culture_province = 0x00C6;
		constexpr uint16_t is_primary_culture_state = 0x00C7;
		constexpr uint16_t is_primary_culture_nation_this_pop = 0x00C8;
		constexpr uint16_t is_primary_culture_nation_this_nation = 0x00C9;
		constexpr uint16_t is_primary_culture_nation_this_state = 0x00CA;
		constexpr uint16_t is_primary_culture_nation_this_province = 0x00CB;
		constexpr uint16_t is_primary_culture_state_this_pop = 0x00CC;
		constexpr uint16_t is_primary_culture_state_this_nation = 0x00CD;
		constexpr uint16_t is_primary_culture_state_this_state = 0x00CE;
		constexpr uint16_t is_primary_culture_state_this_province = 0x00CF;
		constexpr uint16_t is_primary_culture_province_this_pop = 0x00D0;
		constexpr uint16_t is_primary_culture_province_this_nation = 0x00D1;
		constexpr uint16_t is_primary_culture_province_this_state = 0x00D2;
		constexpr uint16_t is_primary_culture_province_this_province = 0x00D3;
		constexpr uint16_t is_primary_culture_pop_this_pop = 0x00D4;
		constexpr uint16_t is_primary_culture_pop_this_nation = 0x00D5;
		constexpr uint16_t is_primary_culture_pop_this_state = 0x00D6;
		constexpr uint16_t is_primary_culture_pop_this_province = 0x00D7;
		constexpr uint16_t is_accepted_culture_pop = 0x00D8;
		constexpr uint16_t is_accepted_culture_province = 0x00D9;
		constexpr uint16_t is_accepted_culture_state = 0x00DA;
		constexpr uint16_t is_coastal = 0x00DB;
		constexpr uint16_t in_sphere_tag = 0x00DC;
		constexpr uint16_t in_sphere_from = 0x00DD;
		constexpr uint16_t in_sphere_this_nation = 0x00DE;
		constexpr uint16_t in_sphere_this_province = 0x00DF;
		constexpr uint16_t in_sphere_this_state = 0x00E0;
		constexpr uint16_t in_sphere_this_pop = 0x00E1;
		constexpr uint16_t produces_nation = 0x00E2;
		constexpr uint16_t produces_state = 0x00E3;
		constexpr uint16_t produces_province = 0x00E4;
		constexpr uint16_t produces_pop = 0x00E5;
		constexpr uint16_t average_militancy_nation = 0x00E6;
		constexpr uint16_t average_militancy_state = 0x00E7;
		constexpr uint16_t average_militancy_province = 0x00E8;
		constexpr uint16_t average_consciousness_nation = 0x00E9;
		constexpr uint16_t average_consciousness_state = 0x00EA;
		constexpr uint16_t average_consciousness_province = 0x00EB;
		constexpr uint16_t is_next_reform_nation = 0x00EC;
		constexpr uint16_t is_next_reform_pop = 0x00ED;
		constexpr uint16_t rebel_power_fraction = 0x00EE;
		constexpr uint16_t recruited_percentage_nation = 0x00EF;
		constexpr uint16_t recruited_percentage_pop = 0x00F0;
		constexpr uint16_t has_culture_core = 0x00F1;
		constexpr uint16_t nationalism = 0x00F2;
		constexpr uint16_t is_overseas = 0x00F3;
		constexpr uint16_t controlled_by_rebels = 0x00F4;
		constexpr uint16_t controlled_by_tag = 0x00F5;
		constexpr uint16_t controlled_by_from = 0x00F6;
		constexpr uint16_t controlled_by_this_nation = 0x00F7;
		constexpr uint16_t controlled_by_this_province = 0x00F8;
		constexpr uint16_t controlled_by_this_state = 0x00F9;
		constexpr uint16_t controlled_by_this_pop = 0x00FA;
		constexpr uint16_t controlled_by_owner = 0x00FB;
		constexpr uint16_t controlled_by_reb = 0x00FC;
		constexpr uint16_t is_canal_enabled = 0x00FD;
		constexpr uint16_t is_state_capital = 0x00FE;
		constexpr uint16_t truce_with_tag = 0x00FF;
		constexpr uint16_t truce_with_from = 0x0100;
		constexpr uint16_t truce_with_this_nation = 0x0101;
		constexpr uint16_t truce_with_this_province = 0x0102;
		constexpr uint16_t truce_with_this_state = 0x0103;
		constexpr uint16_t truce_with_this_pop = 0x0104;
		constexpr uint16_t total_pops_nation = 0x0105;
		constexpr uint16_t total_pops_state = 0x0106;
		constexpr uint16_t total_pops_province = 0x0107;
		constexpr uint16_t total_pops_pop = 0x0108;
		constexpr uint16_t has_pop_type_nation = 0x0109;
		constexpr uint16_t has_pop_type_state = 0x010A;
		constexpr uint16_t has_pop_type_province = 0x010B;
		constexpr uint16_t has_pop_type_pop = 0x010C;
		constexpr uint16_t has_empty_adjacent_province = 0x010D;
		constexpr uint16_t has_leader = 0x010E;
		constexpr uint16_t ai = 0x010F;
		constexpr uint16_t can_create_vassals = 0x0110;
		constexpr uint16_t is_possible_vassal = 0x0111;
		constexpr uint16_t province_id = 0x0112;
		constexpr uint16_t vassal_of_tag = 0x0113;
		constexpr uint16_t vassal_of_from = 0x0114;
		constexpr uint16_t vassal_of_this_nation = 0x0115;
		constexpr uint16_t vassal_of_this_province = 0x0116;
		constexpr uint16_t vassal_of_this_state = 0x0117;
		constexpr uint16_t vassal_of_this_pop = 0x0118;
		constexpr uint16_t alliance_with_tag = 0x0119;
		constexpr uint16_t alliance_with_from = 0x011A;
		constexpr uint16_t alliance_with_this_nation = 0x011B;
		constexpr uint16_t alliance_with_this_province = 0x011C;
		constexpr uint16_t alliance_with_this_state = 0x011D;
		constexpr uint16_t alliance_with_this_pop = 0x011E;
		constexpr uint16_t has_recently_lost_war = 0x011F;
		constexpr uint16_t is_mobilised = 0x0120;
		constexpr uint16_t mobilisation_size = 0x0121;
		constexpr uint16_t crime_higher_than_education_nation = 0x0122;
		constexpr uint16_t crime_higher_than_education_state = 0x0123;
		constexpr uint16_t crime_higher_than_education_province = 0x0124;
		constexpr uint16_t crime_higher_than_education_pop = 0x0125;
		constexpr uint16_t agree_with_ruling_party = 0x0126;
		constexpr uint16_t is_colonial_state = 0x0127;
		constexpr uint16_t is_colonial_province = 0x0128;
		constexpr uint16_t has_factories = 0x0129;
		constexpr uint16_t in_default_tag = 0x012A;
		constexpr uint16_t in_default_from = 0x012B;
		constexpr uint16_t in_default_this_nation = 0x012C;
		constexpr uint16_t in_default_this_province = 0x012D;
		constexpr uint16_t in_default_this_state = 0x012E;
		constexpr uint16_t in_default_this_pop = 0x012F;
		constexpr uint16_t total_num_of_ports = 0x0130;
		constexpr uint16_t always = 0x0131;
		constexpr uint16_t election = 0x0132;
		constexpr uint16_t has_global_flag = 0x0133;
		constexpr uint16_t is_capital = 0x0134;
		constexpr uint16_t nationalvalue = 0x0135;
		constexpr uint16_t industrial_score_value = 0x0136;
		constexpr uint16_t industrial_score_from_nation = 0x0137;
		constexpr uint16_t industrial_score_this_nation = 0x0138;
		constexpr uint16_t industrial_score_this_pop = 0x0139;
		constexpr uint16_t industrial_score_this_state = 0x013A;
		constexpr uint16_t industrial_score_this_province = 0x013B;
		constexpr uint16_t military_score_value = 0x013C;
		constexpr uint16_t military_score_from_nation = 0x013D;
		constexpr uint16_t military_score_this_nation = 0x013E;
		constexpr uint16_t military_score_this_pop = 0x013F;
		constexpr uint16_t military_score_this_state = 0x0140;
		constexpr uint16_t military_score_this_province = 0x0141;
		constexpr uint16_t civilized_nation = 0x0142;
		constexpr uint16_t civilized_pop = 0x0143;
		constexpr uint16_t civilized_province = 0x0144;
		constexpr uint16_t national_provinces_occupied = 0x0145;
		constexpr uint16_t is_greater_power_nation = 0x0146;
		constexpr uint16_t is_greater_power_pop = 0x0147;
		constexpr uint16_t rich_tax = 0x0148;
		constexpr uint16_t middle_tax = 0x0149;
		constexpr uint16_t poor_tax = 0x014A;
		constexpr uint16_t social_spending_nation = 0x014B;
		constexpr uint16_t social_spending_pop = 0x014C;
		constexpr uint16_t social_spending_province = 0x014D;
		constexpr uint16_t colonial_nation = 0x014E;
		constexpr uint16_t pop_majority_religion_nation = 0x014F;
		constexpr uint16_t pop_majority_religion_state = 0x0150;
		constexpr uint16_t pop_majority_religion_province = 0x0151;
		constexpr uint16_t pop_majority_culture_nation = 0x0152;
		constexpr uint16_t pop_majority_culture_state = 0x0153;
		constexpr uint16_t pop_majority_culture_province = 0x0154;
		constexpr uint16_t pop_majority_issue_nation = 0x0155;
		constexpr uint16_t pop_majority_issue_state = 0x0156;
		constexpr uint16_t pop_majority_issue_province = 0x0157;
		constexpr uint16_t pop_majority_issue_pop = 0x0158;
		constexpr uint16_t pop_majority_ideology_nation = 0x0159;
		constexpr uint16_t pop_majority_ideology_state = 0x015A;
		constexpr uint16_t pop_majority_ideology_province = 0x015B;
		constexpr uint16_t pop_majority_ideology_pop = 0x015C;
		constexpr uint16_t poor_strata_militancy_nation = 0x015D;
		constexpr uint16_t poor_strata_militancy_state = 0x015E;
		constexpr uint16_t poor_strata_militancy_province = 0x015F;
		constexpr uint16_t poor_strata_militancy_pop = 0x0160;
		constexpr uint16_t middle_strata_militancy_nation = 0x0161;
		constexpr uint16_t middle_strata_militancy_state = 0x0162;
		constexpr uint16_t middle_strata_militancy_province = 0x0163;
		constexpr uint16_t middle_strata_militancy_pop = 0x0164;
		constexpr uint16_t rich_strata_militancy_nation = 0x0165;
		constexpr uint16_t rich_strata_militancy_state = 0x0166;
		constexpr uint16_t rich_strata_militancy_province = 0x0167;
		constexpr uint16_t rich_strata_militancy_pop = 0x0168;
		constexpr uint16_t rich_tax_above_poor = 0x0169;
		constexpr uint16_t culture_has_union_tag_pop = 0x016A;
		constexpr uint16_t culture_has_union_tag_nation = 0x016B;
		constexpr uint16_t this_culture_union_tag = 0x016C;
		constexpr uint16_t this_culture_union_from = 0x016D;
		constexpr uint16_t this_culture_union_this_nation = 0x016E;
		constexpr uint16_t this_culture_union_this_province = 0x016F;
		constexpr uint16_t this_culture_union_this_state = 0x0170;
		constexpr uint16_t this_culture_union_this_pop = 0x0171;
		constexpr uint16_t this_culture_union_this_union_nation = 0x0172;
		constexpr uint16_t this_culture_union_this_union_province = 0x0173;
		constexpr uint16_t this_culture_union_this_union_state = 0x0174;
		constexpr uint16_t this_culture_union_this_union_pop = 0x0175;
		constexpr uint16_t minorities_nation = 0x0176;
		constexpr uint16_t minorities_state = 0x0177;
		constexpr uint16_t minorities_province = 0x0178;
		constexpr uint16_t revanchism_nation = 0x0179;
		constexpr uint16_t revanchism_pop = 0x017A;
		constexpr uint16_t has_crime = 0x017B;
		constexpr uint16_t num_of_substates = 0x017C;
		constexpr uint16_t num_of_vassals_no_substates = 0x017D;
		constexpr uint16_t brigades_compare_this = 0x017E;
		constexpr uint16_t brigades_compare_from = 0x017F;
		constexpr uint16_t constructing_cb_tag = 0x0180;
		constexpr uint16_t constructing_cb_from = 0x0181;
		constexpr uint16_t constructing_cb_this_nation = 0x0182;
		constexpr uint16_t constructing_cb_this_province = 0x0183;
		constexpr uint16_t constructing_cb_this_state = 0x0184;
		constexpr uint16_t constructing_cb_this_pop = 0x0185;
		constexpr uint16_t constructing_cb_discovered = 0x0186;
		constexpr uint16_t constructing_cb_progress = 0x0187;
		constexpr uint16_t civilization_progress = 0x0188;
		constexpr uint16_t constructing_cb_type = 0x0189;
		constexpr uint16_t is_our_vassal_tag = 0x018A;
		constexpr uint16_t is_our_vassal_from = 0x018B;
		constexpr uint16_t is_our_vassal_this_nation = 0x018C;
		constexpr uint16_t is_our_vassal_this_province = 0x018D;
		constexpr uint16_t is_our_vassal_this_state = 0x018E;
		constexpr uint16_t is_our_vassal_this_pop = 0x018F;
		constexpr uint16_t substate_of_tag = 0x0190;
		constexpr uint16_t substate_of_from = 0x0191;
		constexpr uint16_t substate_of_this_nation = 0x0192;
		constexpr uint16_t substate_of_this_province = 0x0193;
		constexpr uint16_t substate_of_this_state = 0x0194;
		constexpr uint16_t substate_of_this_pop = 0x0195;
		constexpr uint16_t is_substate = 0x0196;
		constexpr uint16_t great_wars_enabled = 0x0197;
		constexpr uint16_t can_nationalize = 0x0198;
		constexpr uint16_t part_of_sphere = 0x0199;
		constexpr uint16_t is_sphere_leader_of_tag = 0x019A;
		constexpr uint16_t is_sphere_leader_of_from = 0x019B;
		constexpr uint16_t is_sphere_leader_of_this_nation = 0x019C;
		constexpr uint16_t is_sphere_leader_of_this_province = 0x019D;
		constexpr uint16_t is_sphere_leader_of_this_state = 0x019E;
		constexpr uint16_t is_sphere_leader_of_this_pop = 0x019F;
		constexpr uint16_t number_of_states = 0x01A0;
		constexpr uint16_t war_score = 0x01A1;
		constexpr uint16_t is_releasable_vassal_from = 0x01A2;
		constexpr uint16_t is_releasable_vassal_other = 0x01A3;
		constexpr uint16_t has_recent_imigration = 0x01A4;
		constexpr uint16_t province_control_days = 0x01A5;
		constexpr uint16_t is_disarmed = 0x01A6;
		constexpr uint16_t big_producer = 0x01A7;
		constexpr uint16_t someone_can_form_union_tag_from = 0x01A8;
		constexpr uint16_t someone_can_form_union_tag_other = 0x01A9;
		constexpr uint16_t social_movement_strength = 0x01AA;
		constexpr uint16_t political_movement_strength = 0x01AB;
		constexpr uint16_t can_build_factory_in_capital_state = 0x01AC;
		constexpr uint16_t social_movement = 0x01AD;
		constexpr uint16_t political_movement = 0x01AE;
		constexpr uint16_t has_cultural_sphere = 0x01AF;
		constexpr uint16_t world_wars_enabled = 0x01B0;
		constexpr uint16_t has_pop_culture_pop_this_pop = 0x01B1;
		constexpr uint16_t has_pop_culture_state_this_pop = 0x01B2;
		constexpr uint16_t has_pop_culture_province_this_pop = 0x01B3;
		constexpr uint16_t has_pop_culture_nation_this_pop = 0x01B4;
		constexpr uint16_t has_pop_culture_pop = 0x01B5;
		constexpr uint16_t has_pop_culture_state = 0x01B6;
		constexpr uint16_t has_pop_culture_province = 0x01B7;
		constexpr uint16_t has_pop_culture_nation = 0x01B8;
		constexpr uint16_t has_pop_religion_pop_this_pop = 0x01B9;
		constexpr uint16_t has_pop_religion_state_this_pop = 0x01BA;
		constexpr uint16_t has_pop_religion_province_this_pop = 0x01BB;
		constexpr uint16_t has_pop_religion_nation_this_pop = 0x01BC;
		constexpr uint16_t has_pop_religion_pop = 0x01BD;
		constexpr uint16_t has_pop_religion_state = 0x01BE;
		constexpr uint16_t has_pop_religion_province = 0x01BF;
		constexpr uint16_t has_pop_religion_nation = 0x01C0;
		constexpr uint16_t life_needs = 0x01C1;
		constexpr uint16_t everyday_needs = 0x01C2;
		constexpr uint16_t luxury_needs = 0x01C3;
		constexpr uint16_t consciousness_pop = 0x01C4;
		constexpr uint16_t consciousness_province = 0x01C5;
		constexpr uint16_t consciousness_state = 0x01C6;
		constexpr uint16_t consciousness_nation = 0x01C7;
		constexpr uint16_t literacy_pop = 0x01C8;
		constexpr uint16_t literacy_province = 0x01C9;
		constexpr uint16_t literacy_state = 0x01CA;
		constexpr uint16_t literacy_nation = 0x01CB;
		constexpr uint16_t militancy_pop = 0x01CC;
		constexpr uint16_t militancy_province = 0x01CD;
		constexpr uint16_t militancy_state = 0x01CE;
		constexpr uint16_t militancy_nation = 0x01CF;
		constexpr uint16_t military_spending_pop = 0x01D0;
		constexpr uint16_t military_spending_province = 0x01D1;
		constexpr uint16_t military_spending_state = 0x01D2;
		constexpr uint16_t military_spending_nation = 0x01D3;
		constexpr uint16_t administration_spending_pop = 0x01D4;
		constexpr uint16_t administration_spending_province = 0x01D5;
		constexpr uint16_t administration_spending_state = 0x01D6;
		constexpr uint16_t administration_spending_nation = 0x01D37;
		constexpr uint16_t education_spending_pop = 0x01D4;
		constexpr uint16_t education_spending_province = 0x01D5;
		constexpr uint16_t education_spending_state = 0x01D6;
		constexpr uint16_t education_spending_nation = 0x01D7;
		constexpr uint16_t trade_goods_in_state_state = 0x01D8;
		constexpr uint16_t trade_goods_in_state_province = 0x01D9;
		constexpr uint16_t has_flashpoint = 0x01DA;
		constexpr uint16_t flashpoint_tension = 0x01DB;
		constexpr uint16_t crisis_exist = 0x01DC;
		constexpr uint16_t is_liberation_crisis = 0x01DD;
		constexpr uint16_t is_claim_crisis = 0x01DE;
		constexpr uint16_t crisis_temperature = 0x01DF;
		constexpr uint16_t involved_in_crisis_pop = 0x01E0;
		constexpr uint16_t involved_in_crisis_nation = 0x01E1;
		
		//complex
		constexpr uint16_t diplomatic_influence_tag = 0x01E2;
		constexpr uint16_t diplomatic_influence_this_nation = 0x01E3;
		constexpr uint16_t diplomatic_influence_this_province = 0x01E4;
		constexpr uint16_t diplomatic_influence_from_nation = 0x01E5;
		constexpr uint16_t diplomatic_influence_from_province = 0x01E6;
		constexpr uint16_t pop_unemployment_nation = 0x01E7;
		constexpr uint16_t pop_unemployment_state = 0x01E8;
		constexpr uint16_t pop_unemployment_province = 0x01E9;
		constexpr uint16_t pop_unemployment_pop = 0x01EA;
		constexpr uint16_t pop_unemployment_nation_this_pop = 0x01EB;
		constexpr uint16_t pop_unemployment_state_this_pop = 0x01EC;
		constexpr uint16_t pop_unemployment_province_this_pop = 0x01ED;
		constexpr uint16_t relation_tag = 0x01EE;
		constexpr uint16_t relation_this_nation = 0x01EF;
		constexpr uint16_t relation_this_province = 0x01F0;
		constexpr uint16_t relation_from_nation = 0x01F1;
		constexpr uint16_t relation_from_province = 0x01F2;
		constexpr uint16_t check_variable = 0x01F3;
		constexpr uint16_t upper_house = 0x01F4;
		constexpr uint16_t x_strata_x_needs_nation = 0x01F5;
		constexpr uint16_t x_strata_x_needs_state = 0x01F6;
		constexpr uint16_t x_strata_x_needs_province = 0x01F7;
		constexpr uint16_t x_strata_x_needs_pop = 0x01F8;
		constexpr uint16_t unemployment_by_type_nation = 0x01F8;
		constexpr uint16_t unemployment_by_type_state = 0x01F9;
		constexpr uint16_t unemployment_by_type_province = 0x01FA;
		constexpr uint16_t unemployment_by_type_pop = 0x01FB;
		constexpr uint16_t party_loyalty_nation_province_id = 0x01FC;
		constexpr uint16_t party_loyalty_from_nation_province_id = 0x01FD;
		constexpr uint16_t party_loyalty_province_province_id = 0x01FE;
		constexpr uint16_t party_loyalty_from_province_province_id = 0x01FF;
		constexpr uint16_t party_loyalty_nation_from_province = 0x0201;
		constexpr uint16_t party_loyalty_from_nation_scope_province = 0x0202;
		constexpr uint16_t can_build_in_province_railroad_no_limit_from_nation = 0x0203;
		constexpr uint16_t can_build_in_province_railroad_yes_limit_from_nation = 0x0204;
		constexpr uint16_t can_build_in_province_railroad_no_limit_this_nation = 0x0205;
		constexpr uint16_t can_build_in_province_railroad_yes_limit_this_nation = 0x0206;
		constexpr uint16_t can_build_in_province_fort_no_limit_from_nation = 0x0207;
		constexpr uint16_t can_build_in_province_fort_yes_limit_from_nation = 0x0208;
		constexpr uint16_t can_build_in_province_fort_no_limit_this_nation = 0x0209;
		constexpr uint16_t can_build_in_province_fort_yes_limit_this_nation = 0x020A;
		constexpr uint16_t can_build_in_province_naval_base_no_limit_from_nation = 0x020B;
		constexpr uint16_t can_build_in_province_naval_base_yes_limit_from_nation = 0x020C;
		constexpr uint16_t can_build_in_province_naval_base_no_limit_this_nation = 0x020D;
		constexpr uint16_t can_build_in_province_naval_base_yes_limit_this_nation = 0x020E;
		constexpr uint16_t can_build_railway_in_capital_yes_whole_state_yes_limit = 0x020F;
		constexpr uint16_t can_build_railway_in_capital_yes_whole_state_no_limit = 0x0210;
		constexpr uint16_t can_build_railway_in_capital_no_whole_state_yes_limit = 0x0211;
		constexpr uint16_t can_build_railway_in_capital_no_whole_state_no_limit = 0x0212;
		constexpr uint16_t can_build_fort_in_capital_yes_whole_state_yes_limit = 0x0213;
		constexpr uint16_t can_build_fort_in_capital_yes_whole_state_no_limit = 0x0214;
		constexpr uint16_t can_build_fort_in_capital_no_whole_state_yes_limit = 0x0215;
		constexpr uint16_t can_build_fort_in_capital_no_whole_state_no_limit = 0x0216;
		constexpr uint16_t work_available_nation = 0x0217;
		constexpr uint16_t work_available_state = 0x0218;
		constexpr uint16_t work_available_province = 0x0219;

		//variable name
		constexpr uint16_t variable_tech_name = 0x021A;
		constexpr uint16_t variable_ideology_name_nation = 0x021B;
		constexpr uint16_t variable_ideology_name_state = 0x021C;
		constexpr uint16_t variable_ideology_name_province = 0x021D;
		constexpr uint16_t variable_ideology_name_pop = 0x021E;
		constexpr uint16_t variable_issue_name_nation = 0x021F;
		constexpr uint16_t variable_issue_name_state = 0x0220;
		constexpr uint16_t variable_issue_name_province = 0x0221;
		constexpr uint16_t variable_issue_name_pop = 0x0222;
		constexpr uint16_t variable_issue_group_name_nation = 0x0223;
		constexpr uint16_t variable_issue_group_name_state = 0x0224;
		constexpr uint16_t variable_issue_group_name_province = 0x0225;
		constexpr uint16_t variable_issue_group_name_pop = 0x0226;
		constexpr uint16_t variable_pop_type_name_nation = 0x0227;
		constexpr uint16_t variable_pop_type_name_state = 0x0228;
		constexpr uint16_t variable_pop_type_name_province = 0x0229;
		constexpr uint16_t variable_pop_type_name_pop = 0x022A;
		constexpr uint16_t variable_good_name = 0x022B;
		//technology name -- payload 1
		//ideology name -- 4 varianets payload 2
		//issue name -- 4 varianets payload 2
		//issue group name -- 4 varianets payload 2
		//pop type -- 4 variants payload 2
		//good name -- payload 2

		//scopes
		constexpr uint16_t generic_scope = 0x0000; // or & and
		constexpr uint16_t x_neighbor_province_scope = 0x0001;
		constexpr uint16_t x_neighbor_country_scope_nation = 0x0002;
		constexpr uint16_t x_neighbor_country_scope_pop = 0x0003;
		constexpr uint16_t x_war_countries_scope_nation = 0x0004;
		constexpr uint16_t x_war_countries_scope_pop = 0x0005;
		constexpr uint16_t x_greater_power_scope = 0x0006;
		constexpr uint16_t x_owned_province_scope_state = 0x0007;
		constexpr uint16_t x_owned_province_scope_nation = 0x0008;
		constexpr uint16_t x_core_scope_province = 0x0009;
		constexpr uint16_t x_core_scope_nation = 0x000A;
		constexpr uint16_t x_state_scope = 0x000B;
		constexpr uint16_t x_substate_scope = 0x000C;
		constexpr uint16_t x_sphere_member_scope = 0x000D;
		constexpr uint16_t x_pop_scope_province = 0x000E;
		constexpr uint16_t x_pop_scope_state = 0x000F;
		constexpr uint16_t x_pop_scope_nation = 0x0010;
		constexpr uint16_t x_provinces_in_variable_region = 0x0011; // variable name
		constexpr uint16_t owner_scope_state = 0x0012;
		constexpr uint16_t owner_scope_province = 0x0013;
		constexpr uint16_t controller_scope = 0x0014;
		constexpr uint16_t location_scope = 0x0015;
		constexpr uint16_t country_scope_state = 0x0016;
		constexpr uint16_t country_scope_pop = 0x0017;
		constexpr uint16_t capital_scope = 0x0018;
		constexpr uint16_t this_scope_pop = 0x0019;
		constexpr uint16_t this_scope_nation = 0x001A;
		constexpr uint16_t this_scope_state = 0x001B;
		constexpr uint16_t this_scope_province = 0x001C;
		constexpr uint16_t from_scope_pop = 0x001D;
		constexpr uint16_t from_scope_nation = 0x001E;
		constexpr uint16_t from_scope_state = 0x001F;
		constexpr uint16_t from_scope_province = 0x0020;
		constexpr uint16_t sea_zone_scope = 0x0021;
		constexpr uint16_t cultural_union_scope = 0x0022;
		constexpr uint16_t overlord_scope = 0x0023;
		constexpr uint16_t sphere_owner_scope = 0x0024;
		constexpr uint16_t independence_scope = 0x0025;
		constexpr uint16_t flashpoint_tag_scope = 0x0026;
		constexpr uint16_t crisis_state_scope = 0x0027;
		constexpr uint16_t state_scope_pop = 0x0028;
		constexpr uint16_t state_scope_province = 0x0029;
		constexpr uint16_t tag_scope = 0x002A; // variable name
		constexpr uint16_t integer_scope = 0x002B; // variable name

		constexpr uint16_t placeholder_not_scope = trigger_code::code_mask;

		//variable
		// region name = 1 variant, x type, payload 1
		// tag = 1 variant, payload 1
		// integer = 1 variant, payload 1
	}

	constexpr bool scope_has_any_all(uint16_t code) {
		return (code >= codes::x_neighbor_province_scope) & (code <= codes::x_provinces_in_variable_region);
	}

	namespace pop_strata {
		constexpr int32_t poor = 0;
		constexpr int32_t middle = 1;
		constexpr int32_t rich = 2;
	}

	struct year_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::year | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{token_to<int32_t>(t)};
		}
	};
	struct is_canal_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::is_canal_enabled | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct month_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::month | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct great_wars_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::great_wars_enabled | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct world_wars_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::world_wars_enabled | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct crisis_exist_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::crisis_exist | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_liberation_crisis_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::is_liberation_crisis | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_claim_crisis_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::is_claim_crisis | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};

	struct port_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::port | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct involved_in_crisis_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::involved_in_crisis_pop | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::involved_in_crisis_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_cultural_sphere_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_cultural_sphere | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct social_movement_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::social_movement | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct political_movement_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::political_movement | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct rich_tax_above_poor_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_tax_above_poor | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_substate_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_substate | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_flashpoint_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_flashpoint | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_disarmed_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_disarmed | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct can_nationalize_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::can_nationalize | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct part_of_sphere_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::part_of_sphere | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct constructing_cb_discovered_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::constructing_cb_discovered | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct colonial_nation_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::colonial_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_capital_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_capital | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct election_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::election | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct always_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			return trigger_code{ uint16_t(codes::always | association_to_bool_code(a, t)), 0ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_releasable_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				if(scope.from_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_releasable_vassal_from | association_to_trigger_code(a)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::is_releasable_vassal_other | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct someone_can_form_union_tag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::someone_can_form_union_tag_from | association_to_trigger_code(a)), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::someone_can_form_union_tag_other | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_state_capital_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_state_capital | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_factories_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_factories | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_empty_adjacent_province_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_empty_adjacent_province | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct ai_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::ai | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct minorities_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::minorities_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::minorities_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::minorities_province | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct culture_has_union_tag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::culture_has_union_tag_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::culture_has_union_tag_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_colonial_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_colonial_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::is_colonial_state | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_greater_power_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_greater_power_pop | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_greater_power_nation | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct can_create_vassals_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::can_create_vassals | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_recently_lost_war_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_recently_lost_war | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_mobilised_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_mobilised | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct crime_higher_than_education_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::crime_higher_than_education_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct civilized_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::civilized_nation | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::civilized_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::civilized_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};

	struct rank_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rank | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct crisis_temperature_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::crisis_temperature | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct has_recent_imigration_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_recent_imigration | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct province_control_days_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::province_control_days | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_substates_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_substates | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_vassals_no_substates_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::num_of_vassals_no_substates | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct number_of_states_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::number_of_states | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct war_score_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::war_score | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct flashpoint_tension_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::flashpoint_tension | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct life_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::war_score | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct everyday_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::everyday_needs | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct luxury_needs_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::luxury_needs | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct social_movement_strength_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::social_movement_strength | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct political_movement_strength_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::political_movement_strength | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct total_num_of_ports_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_num_of_ports | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct agree_with_ruling_party_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::agree_with_ruling_party | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct constructing_cb_progress_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::constructing_cb_progress | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct civilization_progress_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::civilization_progress | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct revanchism_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::revanchism_pop | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::revanchism_nation | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct poor_strata_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::poor_strata_militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct middle_strata_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::middle_strata_militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct rich_strata_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::rich_strata_militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct consciousness_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::consciousness_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::consciousness_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::consciousness_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::consciousness_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct literacy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::literacy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::literacy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::literacy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::literacy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::militancy_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::militancy_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct military_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::military_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::military_spending_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::military_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::military_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct administration_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::administration_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::administration_spending_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::administration_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::administration_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct education_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::education_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::education_spending_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::education_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::education_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct national_provinces_occupied_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::national_provinces_occupied | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct social_spending_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::social_spending_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::social_spending_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::social_spending_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct brigades_compare_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if(scope.this_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::brigades_compare_this | association_to_trigger_code(a)), 1ui16 };
				else if (scope.from_slot == trigger_slot_contents::nation)
					return trigger_code{ uint16_t(codes::brigades_compare_from | association_to_trigger_code(a)), 1ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct rich_tax_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rich_tax | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct middle_tax_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::middle_tax | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct poor_tax_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::poor_tax | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct mobilisation_size_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::mobilisation_size | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct province_id_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::province_id | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct invention_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::invention | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.technology_m.named_invention_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)) );
		}
	};
	struct big_producer_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::big_producer | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct strata_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::strata | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			if (is_fixed_token_ci(t.start, t.end, "poor")) {
				return trigger_bytecode(pop_strata::poor);
			} else if (is_fixed_token_ci(t.start, t.end, "rich")) {
				return trigger_bytecode(pop_strata::rich);
			} else {
				return trigger_bytecode(pop_strata::middle);
			}
		}
	};
	struct life_raing_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::life_rating_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::life_rating_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct has_empty_adjacent_state_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_empty_adjacent_state_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_empty_adjacent_state_state | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct state_id_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::state_id_province | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::state_id_state | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct cash_reserves_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::cash_reserves | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct unemployment_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::unemployment_pop | association_to_trigger_code(a), 1ui16 };
			else if(scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::unemployment_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::unemployment_province | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::unemployment_nation | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct is_slave_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::is_slave_pop | association_to_bool_code(a, t), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::is_slave_state | association_to_bool_code(a, t), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::is_slave_province | association_to_bool_code(a, t), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::is_slave_nation | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_independant_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::is_independant | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_national_minority_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::has_national_minority_state | association_to_bool_code(a, t), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::has_national_minority_province | association_to_bool_code(a, t), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::has_national_minority_nation | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct government_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::government_nation | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::government_pop | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.governments_m.named_government_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct constructing_cb_type_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::constructing_cb_type | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.military_m.named_cb_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct can_build_factory_in_capital_state_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::can_build_factory_in_capital_state | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_factory_types_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct capital_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::capital | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct tech_school_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::tech_school | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.technology_m.named_tech_school_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct primary_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::primary_culture | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_crime_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::has_crime | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct accepted_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::accepted_culture | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::pop_majority_religion_nation | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::pop_majority_religion_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::pop_majority_religion_province | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::pop_majority_culture_nation | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::pop_majority_culture_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::pop_majority_culture_province | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_issue_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::pop_majority_issue_nation | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::pop_majority_issue_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::pop_majority_issue_province | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::pop_majority_issue_pop | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.issues_m.named_option_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct pop_majority_ideology_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::pop_majority_ideology_nation | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::pop_majority_ideology_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::pop_majority_ideology_province | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::pop_majority_ideology_pop | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct trade_goods_in_state_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::trade_goods_in_state_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::trade_goods_in_state_province | association_to_trigger_code(a), 1ui16 };
			
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop) {
					if(scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::culture_this_nation | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::culture_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::culture_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::culture_this_province | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else if (is_fixed_token_ci(t, "from")) {
				if (scope.main_slot == trigger_slot_contents::pop && scope.from_slot == trigger_slot_contents::nation) 
					return trigger_code{ codes::culture_from_nation | association_to_trigger_code(a), 0ui16 };
				else 
					return std::optional<trigger_code>();
			} else if (is_fixed_token_ci(t, "reb")) {
				if(scope.contains_rebeltype == false)
					return std::optional<trigger_code>();
				else if(scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ codes::culture_pop_reb | association_to_trigger_code(a), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ codes::culture_state_reb | association_to_trigger_code(a), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ codes::culture_province_reb | association_to_trigger_code(a), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::culture_nation_reb | association_to_trigger_code(a), 0ui16 };
			}
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::culture_pop | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::culture_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::culture_province | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::culture_nation | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.this_slot == trigger_slot_contents::pop) {
					if (scope.main_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::has_pop_culture_nation_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::has_pop_culture_pop_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::state)
						return trigger_code{ codes::has_pop_culture_state_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::province)
						return trigger_code{ codes::has_pop_culture_province_this_pop | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ codes::has_pop_culture_pop | association_to_trigger_code(a), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ codes::has_pop_culture_state | association_to_trigger_code(a), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ codes::has_pop_culture_province | association_to_trigger_code(a), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::has_pop_culture_nation | association_to_trigger_code(a), 1ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.this_slot == trigger_slot_contents::pop) {
					if (scope.main_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::has_pop_religion_nation_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::has_pop_religion_pop_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::state)
						return trigger_code{ codes::has_pop_religion_state_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::province)
						return trigger_code{ codes::has_pop_religion_province_this_pop | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ codes::has_pop_religion_pop | association_to_trigger_code(a), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ codes::has_pop_religion_state | association_to_trigger_code(a), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ codes::has_pop_religion_province | association_to_trigger_code(a), 1ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::has_pop_religion_nation | association_to_trigger_code(a), 1ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct culture_group_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::culture_group_pop_this_nation | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::culture_group_pop_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::culture_group_pop_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::culture_group_pop_this_province | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::nation) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::culture_group_nation_this_nation | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::culture_group_nation_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::culture_group_nation_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::culture_group_nation_this_province | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else if (is_fixed_token_ci(t, "from")) {
				if (scope.from_slot == trigger_slot_contents::nation) {
					if (scope.main_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::culture_group_pop_from_nation | association_to_trigger_code(a), 0ui16 };
					else if (scope.main_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::culture_group_nation_from_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else if (is_fixed_token_ci(t, "reb")) {
				if (scope.contains_rebeltype == false)
					return std::optional<trigger_code>();
				else if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ codes::culture_group_reb_pop | association_to_trigger_code(a), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::culture_group_reb_nation | association_to_trigger_code(a), 0ui16 };
			}
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::culture_group_pop | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::culture_group_nation | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_culture_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::religion_this_nation | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::religion_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::religion_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::religion_this_pop | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if(scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::religion_from_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "reb")) {
					return trigger_code{ codes::religion_reb | association_to_trigger_code(a), 0ui16 };
				} else {
					return trigger_code{ codes::religion | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.named_religion_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct terrain_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop) {
				return trigger_code{ codes::terrain_pop | association_to_trigger_code(a), 1ui16 };
			} else if (scope.main_slot == trigger_slot_contents::province) {
				return trigger_code{ codes::terrain_province | association_to_trigger_code(a), 1ui16 };
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct trade_goods_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::trade_goods | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct is_secondary_power_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::is_secondary_power_pop | association_to_bool_code(a, t), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::is_secondary_power_nation | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_faction_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::has_faction_pop | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::has_faction_nation | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.population_m.named_rebel_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_unclaimed_cores_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::has_unclaimed_cores | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_cultural_union_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes") || is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::is_cultural_union_bool | association_to_bool_code(a, t), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ codes::is_cultural_union_this_self_pop | association_to_trigger_code(a), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::pop)
					return trigger_code{ codes::is_cultural_union_this_pop | association_to_trigger_code(a), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::state)
					return trigger_code{ codes::is_cultural_union_this_state | association_to_trigger_code(a), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::province)
					return trigger_code{ codes::is_cultural_union_this_province | association_to_trigger_code(a), 0ui16 };
				else if (scope.this_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::is_cultural_union_this_nation | association_to_trigger_code(a), 0ui16 };
				else if (scope.contains_rebeltype)
					return trigger_code{ codes::is_cultural_union_this_rebel | association_to_trigger_code(a), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::is_cultural_union_tag_nation | association_to_trigger_code(a), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::pop)
					return trigger_code{ codes::is_cultural_union_tag_this_pop | association_to_trigger_code(a), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::state)
					return trigger_code{ codes::is_cultural_union_tag_this_state | association_to_trigger_code(a), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::province)
					return trigger_code{ codes::is_cultural_union_tag_this_province | association_to_trigger_code(a), 1ui16 };
				else if (scope.this_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::is_cultural_union_tag_this_nation | association_to_trigger_code(a), 1ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct can_build_factory_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::can_build_factory | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct war_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ codes::war_pop | association_to_bool_code(a, t), 0ui16 };
			else if(scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::war_nation | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct war_exhaustion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::war_exhaustion | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct blockade_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::blockade | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct owns_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::owns | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct controls_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::controls | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct is_core_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_integer(t.start, t.end)) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::is_core_integer | association_to_trigger_code(a), 1ui16 };
				else
					return std::optional<trigger_code>();
			} else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::is_core_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::is_core_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::is_core_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::is_core_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::is_core_from_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return trigger_code{ codes::is_core_reb | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::is_core_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			if (is_integer(t.start, t.end)) {
				return trigger_bytecode{ token_to<int32_t>(t) };
			} else {
				return trigger_bytecode(
					tag_from_text(
						s.culutre_m.national_tags_index,
						cultures::tag_to_encoding(t.start, t.end)));
			}
		}
	};
	struct num_of_revolts_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::num_of_revolts | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct revolt_percentage_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::revolt_percentage | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct num_of_cities_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::num_of_cities_this_nation | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::num_of_cities_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::num_of_cities_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::num_of_cities_this_pop | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::num_of_cities_from_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::num_of_cities_int | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_ports_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::num_of_ports | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_allies_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::num_of_allies | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct num_of_vassals_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::num_of_vassals | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct owned_by_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::owned_by_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::owned_by_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::owned_by_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::owned_by_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::owned_by_from_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::owned_by_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct exists_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "yes") | is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::exists_bool | association_to_bool_code(a, t), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				return trigger_code{ codes::exists_tag | association_to_trigger_code(a), 1ui16 };
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct has_country_flag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::has_country_flag | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_bytecode{
				s.variables_m.get_named_national_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct has_global_flag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			return trigger_code{ codes::has_global_flag | association_to_trigger_code(a), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_bytecode{
				s.variables_m.get_named_global_variable(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end)) };
		}
	};
	struct continent_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_nation_this | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_nation_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::continent_nation | association_to_trigger_code(a), 1ui16 };
				}
			} else if (scope.main_slot == trigger_slot_contents::state) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_state_this | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_state_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::continent_state | association_to_trigger_code(a), 1ui16 };
				}
			} else if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_province_this | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_province_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::continent_province | association_to_trigger_code(a), 1ui16 };
				}
			} else if (scope.main_slot == trigger_slot_contents::pop) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_pop_this | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::continent_pop_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::continent_pop | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
			if (is_fixed_token_ci(t, "yes") | is_fixed_token_ci(t, "no")) {
				if (scope.main_slot == trigger_slot_contents::nation)
					return trigger_code{ codes::exists_bool | association_to_bool_code(a, t), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else {
				return trigger_code{ codes::exists_tag | association_to_trigger_code(a), 1ui16 };
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct casus_belli_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::casus_belli_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::casus_belli_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::casus_belli_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::casus_belli_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::casus_belli_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::casus_belli_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct military_access_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::military_access_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::military_access_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::military_access_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::military_access_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::military_access_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::military_access_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct prestige_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::prestige_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::prestige_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::prestige_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::prestige_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::prestige_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::prestige_value | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct badboy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::badboy | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct has_building_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ codes::has_building_state | association_to_trigger_code(a), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province) {
				if(is_fixed_token_ci(t, "fort"))
					return trigger_code{ codes::has_building_fort | association_to_trigger_code(a), 0ui16 };
				else if (is_fixed_token_ci(t, "railroad"))
					return trigger_code{ codes::has_building_railroad | association_to_trigger_code(a), 0ui16 };
				else if (is_fixed_token_ci(t, "naval_base"))
					return trigger_code{ codes::has_building_naval_base | association_to_trigger_code(a), 0ui16 };
				else
					return std::optional<trigger_code>();
			} else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_factory_types_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct empty_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::empty | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_blockaded_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::is_blockaded | association_to_bool_code(a, t), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct has_country_modifier_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::has_country_modifier | association_to_trigger_code(a), 1ui16 };
			else 
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_province_modifier_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::has_province_modifier | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_provincial_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct nationalvalue_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ codes::nationalvalue | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.modifiers_m.named_national_modifiers_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct region_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ codes::region | association_to_trigger_code(a), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.province_m.named_states_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};

	struct tag_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::tag_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::tag_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::tag_from_nation | association_to_trigger_code(a), 0ui16 };
					else if (scope.from_slot == trigger_slot_contents::province)
						return trigger_code{ codes::tag_from_province | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::tag_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct neighbour_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::neighbour_this | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::neighbour_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::neighbour_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct units_in_province_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::units_in_province_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::units_in_province_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::units_in_province_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::units_in_province_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::units_in_province_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::units_in_province_value | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<int32_t>(t));
		}
	};
	struct war_with_trigger{
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::war_with_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::war_with_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::war_with_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::war_with_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::war_with_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::war_with_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct unit_in_battle_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::unit_in_battle | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct total_amount_of_divisions_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_amount_of_divisions | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct money_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::money | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct lost_national_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::lost_national | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct is_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_vassal | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct ruling_party_ideology_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::ruling_party_ideology_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::ruling_party_ideology_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct ruling_party_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::ruling_party | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct has_leader_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_leader | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(text_data::get_thread_safe_text_handle(s.text_m, t.start, t.end));
		}
	};
	struct is_ideology_enabled_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::is_ideology_enabled | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.ideologies_m.named_ideology_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct political_reform_want_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::political_reform_want_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::political_reform_want_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct social_reform_want_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::social_reform_want_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::social_reform_want_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct total_amount_of_ships_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_amount_of_ships | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
		}
	};
	struct plurality_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::plurality | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct corruption_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::corruption | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<float>(t) };
		}
	};
	struct is_state_religion_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::is_state_religion_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_state_religion_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_state_religion_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};

	struct is_primary_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (is_fixed_token_ci(t, "this")) {
				if (scope.main_slot == trigger_slot_contents::nation) {
					if(scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_nation_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::state) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_state_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::province) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_province_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (scope.main_slot == trigger_slot_contents::pop) {
					if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_nation | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_state | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_province | association_to_trigger_code(a)), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ uint16_t(codes::is_primary_culture_pop_this_pop | association_to_trigger_code(a)), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return std::optional<trigger_code>();
				}
			} else {
				if (scope.main_slot == trigger_slot_contents::state)
					return trigger_code{ uint16_t(codes::is_primary_culture_state | association_to_bool_code(a, t)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::province)
					return trigger_code{ uint16_t(codes::is_primary_culture_province | association_to_bool_code(a, t)), 0ui16 };
				else if (scope.main_slot == trigger_slot_contents::pop)
					return trigger_code{ uint16_t(codes::is_primary_culture_pop | association_to_bool_code(a, t)), 0ui16 };
				else
					return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_accepted_culture_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::is_accepted_culture_state | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_accepted_culture_province | association_to_bool_code(a, t)), 0ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_accepted_culture_pop | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
			
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct is_coastal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_coastal | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct in_sphere_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::in_sphere_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::in_sphere_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::in_sphere_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::in_sphere_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::in_sphere_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::in_sphere_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct produces_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::produces_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::produces_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::produces_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::produces_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.economy_m.named_goods_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct has_pop_type_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::has_pop_type_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::has_pop_type_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::has_pop_type_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::has_pop_type_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.population_m.named_pop_type_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct total_pops_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::total_pops_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::total_pops_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::total_pops_state | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::total_pops_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<int32_t>(t));
		}
	};
	struct average_militancy_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::average_militancy_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::average_militancy_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::average_militancy_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct average_consciousness_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::average_consciousness_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::average_consciousness_province | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::state)
				return trigger_code{ uint16_t(codes::average_consciousness_state | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct is_next_reform_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::is_next_reform_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::is_next_reform_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.issues_m.named_option_index,
					text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end)));
		}
	};
	struct rebel_power_fraction_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::rebel_power_fraction | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct recruited_percentage_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::nation)
				return trigger_code{ uint16_t(codes::recruited_percentage_nation | association_to_trigger_code(a)), 1ui16 };
			else if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::recruited_percentage_pop | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct has_culture_core_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::pop)
				return trigger_code{ uint16_t(codes::has_culture_core | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct nationalism_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type&) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::nationalism | association_to_trigger_code(a)), 1ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<int32_t>(t));
		}
	};
	struct is_overseas_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::is_overseas | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct controlled_by_rebels_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province)
				return trigger_code{ uint16_t(codes::controlled_by_rebels | association_to_bool_code(a, t)), 0ui16 };
			else
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<bool>(t) };
		}
	};
	struct controlled_by_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::province) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::controlled_by_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::controlled_by_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::controlled_by_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::controlled_by_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::controlled_by_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "owner")) {
					return trigger_code{ codes::controlled_by_owner | association_to_trigger_code(a), 0ui16 };
				} else if (is_fixed_token_ci(t, "reb")) {
					if (scope.contains_rebeltype)
						return trigger_code{ codes::controlled_by_reb | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::controlled_by_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct truce_with_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::truce_with_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::truce_with_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::truce_with_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::truce_with_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::truce_with_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::truce_with_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct is_sphere_leader_of_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::is_sphere_leader_of_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::is_sphere_leader_of_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::is_sphere_leader_of_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::is_sphere_leader_of_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::is_sphere_leader_of_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::is_sphere_leader_of_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct constructing_cb_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::constructing_cb_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::constructing_cb_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::constructing_cb_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::constructing_cb_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::constructing_cb_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::constructing_cb_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct vassal_of_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::vassal_of_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::vassal_of_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::vassal_of_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::vassal_of_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::vassal_of_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::vassal_of_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct substate_of_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::substate_of_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::substate_of_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::substate_of_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::substate_of_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::substate_of_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::substate_of_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct is_our_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::is_our_vassal_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::is_our_vassal_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::is_our_vassal_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::is_our_vassal_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::is_our_vassal_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::is_our_vassal_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct this_culture_union_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::this_culture_union_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::this_culture_union_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::this_culture_union_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::this_culture_union_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "this_union")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::this_culture_union_this_union_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::this_culture_union_this_union_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::this_culture_union_this_union_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::this_culture_union_this_union_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::this_culture_union_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::this_culture_union_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct alliance_with_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::alliance_with_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::alliance_with_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::alliance_with_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::alliance_with_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::alliance_with_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::alliance_with_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct in_default_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::in_default_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::in_default_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::in_default_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::in_default_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::in_default_from | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::in_default_tag | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	struct industrial_score_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::industrial_score_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::industrial_score_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::industrial_score_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::industrial_score_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::industrial_score_from_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::industrial_score_value | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct military_score_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) {
				if (is_fixed_token_ci(t, "this")) {
					if (scope.this_slot == trigger_slot_contents::pop)
						return trigger_code{ codes::military_score_this_pop | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::state)
						return trigger_code{ codes::military_score_this_state | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::province)
						return trigger_code{ codes::military_score_this_province | association_to_trigger_code(a), 0ui16 };
					else if (scope.this_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::military_score_this_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else if (is_fixed_token_ci(t, "from")) {
					if (scope.from_slot == trigger_slot_contents::nation)
						return trigger_code{ codes::military_score_from_nation | association_to_trigger_code(a), 0ui16 };
					else
						return std::optional<trigger_code>();
				} else {
					return trigger_code{ codes::military_score_value | association_to_trigger_code(a), 1ui16 };
				}
			} else {
				return std::optional<trigger_code>();
			}
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode(token_to<float>(t));
		}
	};
	struct is_possible_vassal_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope, association_type a, const token_and_type& t) {
			if (scope.main_slot == trigger_slot_contents::nation) 
				return trigger_code{ codes::is_possible_vassal | association_to_trigger_code(a), 1ui16 };
			else 
				return std::optional<trigger_code>();
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager& s) {
			return trigger_bytecode(
				tag_from_text(
					s.culutre_m.national_tags_index,
					cultures::tag_to_encoding(t.start, t.end)));
		}
	};
	

	//for scope triggers
	struct or_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope) {

		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {

		}
	};
}
