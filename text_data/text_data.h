#pragma once
#include <stdint.h>
#include <variant>
#include <vector>
#include "boost\\container\\flat_map.hpp"
#include "simple_fs\\simple_fs.h"

namespace text_data {
	enum class text_color {
		black, white, red, green, yellow, unspecified
	};
	enum class value_type {
		error_no_matching_value, action, active, actor, adj, against, agressor, allowed, amount,
		anyprovince, army, army_name, astate, attacker, attunit, bac, badboy, badword, base,
		base_percentage, bat, bld, bon, bonus, brig, bud, building, buy, cap, capital, cash,
		casus, cat, category, cb_target_name, cb_target_name_adj, chan, chance, change, commander,
		con, construction, continentname, cost, count, countries, country, country1, country2,
		countryadj, countryculture, countryname, country_adj, creator, credits, crisisarea,
		crisisattacker, crisisdefender, crisistaker, crisistaker_adj, crisistarget, crisistarget_adj,
		cul, culture, culture_group_union, curr, current, d, date, date_long_0, date_long_1,
		date_short_0, day, days, defender, defunit, desc, dest, details, direction, dist, effect,
		effects, emilist, employees, employee_max, enemy, engineermaxunits, engineerunits, escorts,
		event, eventdesc, exc, exp, faction, factory, first, focus, folder, vtype_for, fraction,
		vtype_friend, from, fromcountry, fromcountry_adj, fromprovince, fromruler, funds, goal,
		good, goods, gov, government, govt, gp, gp_adj, group, high_tax, hit, home, hull, iamount,
		icountry, ide, ideology, ilocation, immlist, impact, iname, income, indep, inf, infamy,
		input, inv, invention, invested, invested_in_us_message, issue, law, leader, lev, level,
		levels, lim, limit, list, literacy, loc, local, location, lord, lose, low_tax, lvl, m,
		max, maxloan, men, messenger, mil, militancy, min, monarchtitle, money, month, months,
		movement, much, name, nation, nationalvalue, natives, navy, navy_name, need, needed,
		negative, neut, vtype_new, newcountry, nf, now, num, number, numfactories, numspecialfactories,
		odds, old, oldcountry, vtype_operator, opinion, opposing_army, opposing_navy, opressor,
		opt, optimal, option, order, org, organisation, other, otherresult, ourcapital, ourcountry,
		ourcountry_adj, our_lead, our_num, our_res, output, overlord, param, party, passive, pay,
		pen, penalty, per, perc, perc2, percent, percentage, player, policy, pop, poptype,
		popularity, position, positive, power, pres, prescence, prestige, produced, producer,
		prog, progress, proj, prov, provinceculture, provincename, provincereligion,
		provinces, provs, province, rank, rate, rec, recipient, reconmaxunits, reconunits, reform,
		region, rel, relation, req, reqlevel, required, resource, result, rstate, rule, runs, scr,
		sea, second, second_country, self, sell, setting, ships, size, skill, source, speed,
		spheremaster, state, statename, str, strata, strings_list_4, string_0_0, string_0_1,
		string_0_2, string_0_3, string_0_4, string_9_0, sub, table, tag, tag0_0, tag_0_0, tag_0_0_,
		tag_0_0_adj, tag_0_0_upper, tag_0_1, tag_0_1_adj, tag_0_1_upper, tag_0_2, tag_0_2_adj,
		tag_0_3, tag_0_3_adj, tag_1_0, tag_2_0, tag_2_0_upper, tag_3_0, tag_3_0_upper, target,
		targetlist, target_country, tech, temperature, terms, terrain, terrainmod, text, theirlost,
		theirnum, theirship, their_lead, their_num, their_res, them, third, threat, time, title, to,
		tot, totalemi, totalimm, tra, truth, type, unemployed, vtype_union, union_adj, unit, units,
		until, usloss, usnum, val, value, value_int1, value_int_0_0, value_int_0_1, value_int_0_2,
		value_int_0_3, value_int_0_4, verb, versus, war, wargoal, we, what, where, which, who, winner,
		x, y, year, years, chief_of_navy, chief_of_staff, control, head_of_government, owner, playername
	};

	struct line_break {};
	struct color_change {
		text_color color = text_color::unspecified;
	};
	struct value_placeholder {
		value_type value = value_type::error_no_matching_value;
	};
	struct text_chunk {
		uint32_t offset = 0;
		uint16_t length = 0;
	};
	using text_component = std::variant<line_break, color_change, value_placeholder, text_chunk>;

	struct text_sequence {
		uint16_t starting_component = 0;
		uint16_t component_count = 0;
	};

	struct text_sequences {
		std::vector<char16_t> text_data;
		std::vector<text_component> all_components;
		std::vector<text_sequence> all_sequences;
		boost::container::flat_map<std::string, uint32_t> key_to_sequence_map;
	};
	void add_win1250_text_to_container(text_sequences& container, const char* s, const char *e);
	void add_utf8_text_to_container(text_sequences& container, const char* s, const char *e);
	bool is_win1250_section(const char* start, const char* end);
	bool is_utf8_section(const char* start, const char* end);
	void add_utf8_sequence(text_sequences& container, const char* key_start, const char* key_end, const char* seq_start, const char* seq_end);
	void add_win1250_sequence(text_sequences& container, const char* key_start, const char* key_end, const char* seq_start, const char* seq_end);
	value_type value_type_from_name(const char* start, const char* end);
	const char16_t* name_from_value_type(value_type v);
}

void load_text_sequences_from_directory(const directory& source_directory, text_data::text_sequences& container);