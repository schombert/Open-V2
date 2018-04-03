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

		//multivariable
		//diplomatic_influence
		//pop_unemployment
		//relation
		//sub_unit
		//check_variable
		//upper_house

		//scopes
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

	struct month_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state&, association_type a, const token_and_type&) {
			return trigger_code{ uint16_t(codes::month | association_to_trigger_code(a)), 1ui16 };
		}
		trigger_bytecode read_value(const token_and_type& t, const scenario::scenario_manager&) {
			return trigger_bytecode{ token_to<int32_t>(t) };
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
	struct has_province_modifier_trigger {
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
			return trigger_bytecode(text_data::get_thread_safe_existing_text_handle(s.text_m, t.start, t.end));
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

	//for scope triggers
	struct or_trigger {
		static std::optional<trigger_code> produce_code(const trigger_scope_state& scope) {

		}
		static trigger_scope_state produce_new_scope(const trigger_scope_state& scope) {

		}
	};
}
