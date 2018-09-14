#include "text_data.h"
#include <Windows.h>
#include "Parsers\\parsers.h"

namespace text_data {
	text_color char_to_color(char in);

	text_color char_to_color(char in) {
		switch (in) {
			case 'W': return text_color::white;
			case 'G': return text_color::green;
			case 'R': return text_color::red;
			case 'Y': return text_color::yellow;
			case 'b': return text_color::black;
			default: return text_color::unspecified;
		}
	}

	void add_win1250_text_to_container(text_sequences& container, const char* s, const char *e) {
		if (s < e) {
			const uint32_t offset = static_cast<uint32_t>(container.text_data.size());
			for (auto i = s; i != e; ++i) {
				container.text_data.push_back(win1250toUTF16(*i));
			}
			container.all_components.emplace_back(text_chunk{ offset, static_cast<uint16_t>(e - s) });
		}
	}

	void add_utf8_text_to_container(text_sequences& container, const char* s, const char *e) {
		if (s < e) {
			const uint32_t offset = static_cast<uint32_t>(container.text_data.size());

			const auto size = MultiByteToWideChar(CP_UTF8, 0, s, (int)(e - s), nullptr, 0);
			char16_t* buffer = (char16_t*)_alloca((size_t)(e - s) * sizeof(char16_t));

			MultiByteToWideChar(CP_UTF8, 0, s, (int)(e - s), (wchar_t*)buffer, size);
			container.text_data.insert(container.text_data.end(), buffer, buffer + size);

			container.all_components.emplace_back(text_chunk{ offset, static_cast<uint16_t>(size) });
		}
	}

	bool is_win1250_section(const char* start, const char* end) {
		if (start < end)
			return (uint8_t)(*start) == (uint8_t)(0xA7);
		else
			return false;
	}

	bool is_utf8_section(const char* start, const char* end) {
		if (start + 1 < end)
			return ((uint8_t)(*start) == (uint8_t)(0xC2)) & ((uint8_t)(*(start + 1)) == (uint8_t)(0xA7));
		else
			return false;
	}

	template<void (*add_text)(text_sequences&, const char*, const char*), bool(*is_section)(const char*, const char*), uint32_t section_char_size>
	void add_generic_sequence(text_sequences& container, std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci>& temp_map, const char* key_start, const char* key_end, const char* seq_start, const char* seq_end) {
		const char* section_start = seq_start;

		const auto component_start_index = container.all_components.size();

		for (const char* pos = seq_start; pos < seq_end; ++pos) {
			if (is_section(pos, seq_end)) {
				add_text(container, section_start, pos);
				section_start = pos + section_char_size + 1;
				if (pos + section_char_size < seq_end)
					container.all_components.emplace_back(color_change{ char_to_color(*(pos + section_char_size)) });
				pos += section_char_size;
			} else if (*pos == '$') {
				add_text(container, section_start, pos);

				const char* vend = pos + 1;
				while (vend != seq_end) {
					if (*vend == '$')
						break;
					++vend;
				}

				container.all_components.emplace_back(value_placeholder{ value_type_from_name(pos + 1,vend) });

				pos = vend;
				section_start = vend + 1;
			} else if (*pos == '\\' && pos + 1 != seq_end && *(pos + 1) == 'n') {
				add_text(container, section_start, pos);
				section_start = pos + 2;

				container.all_components.emplace_back(line_break{});
				++pos;
			}
		}

		add_text(container, section_start, seq_end);

		const auto nh = container.all_sequences.emplace_back(
			text_sequence{
			static_cast<uint16_t>(component_start_index),
			static_cast<uint16_t>(container.all_components.size() - component_start_index) });
		temp_map.emplace(vector_backed_string<char>(key_start, key_end, container.key_data), nh);
	}

	void add_win1250_sequence(text_sequences& container, std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci>& temp_map, const char* key_start, const char* key_end, const char* seq_start, const char* seq_end) {
		add_generic_sequence<add_win1250_text_to_container, is_win1250_section, 1>(container, temp_map, key_start, key_end, seq_start, seq_end);
	}
	void add_utf8_sequence(text_sequences& container, std::map<vector_backed_string<char>, text_tag, vector_backed_string_less_ci>& temp_map, const char* key_start, const char* key_end, const char* seq_start, const char* seq_end) {
		add_generic_sequence<add_utf8_text_to_container, is_utf8_section, 2>(container, temp_map, key_start, key_end, seq_start, seq_end);
	}

	const char16_t* name_from_value_type(value_type v) {
		switch (v) {
			case value_type::error_no_matching_value: return u"error_no_matching_value";
			case value_type::action: return u"action";
			case value_type::active: return u"active";
			case value_type::actor: return u"actor";
			case value_type::adj: return u"adj";
			case value_type::against: return u"against";
			case value_type::agressor: return u"agressor";
			case value_type::allowed: return u"allowed";
			case value_type::amount: return u"amount";
			case value_type::anyprovince: return u"anyprovince";
			case value_type::army: return u"army";
			case value_type::army_name: return u"army_name";
			case value_type::astate: return u"astate";
			case value_type::attacker: return u"attacker";
			case value_type::attunit: return u"attunit";
			case value_type::bac: return u"bac";
			case value_type::badboy: return u"badboy";
			case value_type::badword: return u"badword";
			case value_type::base: return u"base";
			case value_type::base_percentage: return u"base_percentage";
			case value_type::bat: return u"bat";
			case value_type::bld: return u"bld";
			case value_type::bon: return u"bon";
			case value_type::bonus: return u"bonus";
			case value_type::brig: return u"brig";
			case value_type::bud: return u"bud";
			case value_type::building: return u"building";
			case value_type::buy: return u"buy";
			case value_type::cap: return u"cap";
			case value_type::capital: return u"capital";
			case value_type::cash: return u"cash";
			case value_type::casus: return u"casus";
			case value_type::cat: return u"cat";
			case value_type::category: return u"category";
			case value_type::cb_target_name: return u"cb_target_name";
			case value_type::cb_target_name_adj: return u"cb_target_name_adj";
			case value_type::chan: return u"chan";
			case value_type::chance: return u"chance";
			case value_type::change: return u"change";
			case value_type::commander: return u"commander";
			case value_type::con: return u"con";
			case value_type::construction: return u"construction";
			case value_type::continentname: return u"continentname";
			case value_type::cost: return u"cost";
			case value_type::count: return u"count";
			case value_type::countries: return u"countries";
			case value_type::country: return u"country";
			case value_type::country1: return u"country1";
			case value_type::country2: return u"country2";
			case value_type::countryadj: return u"countryadj";
			case value_type::countryculture: return u"countryculture";
			case value_type::countryname: return u"countryname";
			case value_type::country_adj: return u"country_adj";
			case value_type::creator: return u"creator";
			case value_type::credits: return u"credits";
			case value_type::crisisarea: return u"crisisarea";
			case value_type::crisisattacker: return u"crisisattacker";
			case value_type::crisisdefender: return u"crisisdefender";
			case value_type::crisistaker: return u"crisistaker";
			case value_type::crisistaker_adj: return u"crisistaker_adj";
			case value_type::crisistarget: return u"crisistarget";
			case value_type::crisistarget_adj: return u"crisistarget_adj";
			case value_type::cul: return u"cul";
			case value_type::culture: return u"culture";
			case value_type::culture_group_union: return u"culture_group_union";
			case value_type::curr: return u"curr";
			case value_type::current: return u"current";
			case value_type::d: return u"d";
			case value_type::date: return u"date";
			case value_type::date_long_0: return u"date_long_0";
			case value_type::date_long_1: return u"date_long_1";
			case value_type::date_short_0: return u"date_short_0";
			case value_type::day: return u"day";
			case value_type::days: return u"days";
			case value_type::defender: return u"defender";
			case value_type::defunit: return u"defunit";
			case value_type::desc: return u"desc";
			case value_type::dest: return u"dest";
			case value_type::details: return u"details";
			case value_type::direction: return u"direction";
			case value_type::dist: return u"dist";
			case value_type::effect: return u"effect";
			case value_type::effects: return u"effects";
			case value_type::emilist: return u"emilist";
			case value_type::employees: return u"employees";
			case value_type::employee_max: return u"employee_max";
			case value_type::enemy: return u"enemy";
			case value_type::engineermaxunits: return u"engineermaxunits";
			case value_type::engineerunits: return u"engineerunits";
			case value_type::escorts: return u"escorts";
			case value_type::event: return u"event";
			case value_type::eventdesc: return u"eventdesc";
			case value_type::exc: return u"exc";
			case value_type::exp: return u"exp";
			case value_type::faction: return u"faction";
			case value_type::factory: return u"factory";
			case value_type::first: return u"first";
			case value_type::focus: return u"focus";
			case value_type::folder: return u"folder";
			case value_type::vtype_for: return u"for";
			case value_type::fraction: return u"fraction";
			case value_type::vtype_friend: return u"friend";
			case value_type::from: return u"from";
			case value_type::fromcountry: return u"fromcountry";
			case value_type::fromcountry_adj: return u"fromcountry_adj";
			case value_type::fromprovince: return u"fromprovince";
			case value_type::fromruler: return u"fromruler";
			case value_type::funds: return u"funds";
			case value_type::goal: return u"goal";
			case value_type::good: return u"good";
			case value_type::goods: return u"goods";
			case value_type::gov: return u"gov";
			case value_type::government: return u"government";
			case value_type::govt: return u"govt";
			case value_type::gp: return u"gp";
			case value_type::gp_adj: return u"gp_adj";
			case value_type::group: return u"group";
			case value_type::high_tax: return u"high_tax";
			case value_type::hit: return u"hit";
			case value_type::home: return u"home";
			case value_type::hull: return u"hull";
			case value_type::iamount: return u"iamount";
			case value_type::icountry: return u"icountry";
			case value_type::ide: return u"ide";
			case value_type::ideology: return u"ideology";
			case value_type::ilocation: return u"ilocation";
			case value_type::immlist: return u"immlist";
			case value_type::impact: return u"impact";
			case value_type::iname: return u"iname";
			case value_type::income: return u"income";
			case value_type::indep: return u"indep";
			case value_type::inf: return u"inf";
			case value_type::infamy: return u"infamy";
			case value_type::input: return u"input";
			case value_type::inv: return u"inv";
			case value_type::invention: return u"invention";
			case value_type::invested: return u"invested";
			case value_type::invested_in_us_message: return u"invested_in_us_message";
			case value_type::issue: return u"issue";
			case value_type::law: return u"law";
			case value_type::leader: return u"leader";
			case value_type::lev: return u"lev";
			case value_type::level: return u"level";
			case value_type::levels: return u"levels";
			case value_type::lim: return u"lim";
			case value_type::limit: return u"limit";
			case value_type::list: return u"list";
			case value_type::literacy: return u"literacy";
			case value_type::loc: return u"loc";
			case value_type::local: return u"local";
			case value_type::location: return u"location";
			case value_type::lord: return u"lord";
			case value_type::lose: return u"lose";
			case value_type::low_tax: return u"low_tax";
			case value_type::lvl: return u"lvl";
			case value_type::m: return u"m";
			case value_type::max: return u"max";
			case value_type::maxloan: return u"maxloan";
			case value_type::men: return u"men";
			case value_type::messenger: return u"messenger";
			case value_type::mil: return u"mil";
			case value_type::militancy: return u"militancy";
			case value_type::min: return u"min";
			case value_type::monarchtitle: return u"monarchtitle";
			case value_type::money: return u"money";
			case value_type::month: return u"month";
			case value_type::months: return u"months";
			case value_type::movement: return u"movement";
			case value_type::much: return u"much";
			case value_type::name: return u"name";
			case value_type::nation: return u"nation";
			case value_type::nationalvalue: return u"nationalvalue";
			case value_type::natives: return u"natives";
			case value_type::navy: return u"navy";
			case value_type::navy_name: return u"navy_name";
			case value_type::need: return u"need";
			case value_type::needed: return u"needed";
			case value_type::negative: return u"negative";
			case value_type::neut: return u"neut";
			case value_type::vtype_new: return u"new";
			case value_type::newcountry: return u"newcountry";
			case value_type::nf: return u"nf";
			case value_type::now: return u"now";
			case value_type::num: return u"num";
			case value_type::number: return u"number";
			case value_type::numfactories: return u"numfactories";
			case value_type::numspecialfactories: return u"numspecialfactories";
			case value_type::odds: return u"odds";
			case value_type::old: return u"old";
			case value_type::oldcountry: return u"oldcountry";
			case value_type::vtype_operator: return u"operator";
			case value_type::opinion: return u"opinion";
			case value_type::opposing_army: return u"opposing_army";
			case value_type::opposing_navy: return u"opposing_navy";
			case value_type::opressor: return u"opressor";
			case value_type::opt: return u"opt";
			case value_type::optimal: return u"optimal";
			case value_type::option: return u"option";
			case value_type::order: return u"order";
			case value_type::org: return u"org";
			case value_type::organisation: return u"organisation";
			case value_type::other: return u"other";
			case value_type::otherresult: return u"otherresult";
			case value_type::ourcapital: return u"ourcapital";
			case value_type::ourcountry: return u"ourcountry";
			case value_type::ourcountry_adj: return u"ourcountry_adj";
			case value_type::our_lead: return u"our_lead";
			case value_type::our_num: return u"our_num";
			case value_type::our_res: return u"our_res";
			case value_type::output: return u"output";
			case value_type::overlord: return u"overlord";
			case value_type::param: return u"param";
			case value_type::party: return u"party";
			case value_type::passive: return u"passive";
			case value_type::pay: return u"pay";
			case value_type::pen: return u"pen";
			case value_type::penalty: return u"penalty";
			case value_type::per: return u"per";
			case value_type::perc: return u"perc";
			case value_type::perc2: return u"perc2";
			case value_type::percent: return u"percent";
			case value_type::percentage: return u"percentage";
			case value_type::player: return u"player";
			case value_type::policy: return u"policy";
			case value_type::pop: return u"pop";
			case value_type::poptype: return u"poptype";
			case value_type::popularity: return u"popularity";
			case value_type::position: return u"position";
			case value_type::positive: return u"positive";
			case value_type::power: return u"power";
			case value_type::pres: return u"pres";
			case value_type::prescence: return u"prescence";
			case value_type::prestige: return u"prestige";
			case value_type::produced: return u"produced";
			case value_type::producer: return u"producer";
			case value_type::prog: return u"prog";
			case value_type::progress: return u"progress";
			case value_type::proj: return u"proj";
			case value_type::prov: return u"prov";
			case value_type::provinceculture: return u"provinceculture";
			case value_type::provincename: return u"provincename";
			case value_type::provincereligion: return u"provincereligion";
			case value_type::provinces: return u"provinces";
			case value_type::provs: return u"provs";
			case value_type::province: return u"province";
			case value_type::rank: return u"rank";
			case value_type::rate: return u"rate";
			case value_type::rec: return u"rec";
			case value_type::recipient: return u"recipient";
			case value_type::reconmaxunits: return u"reconmaxunits";
			case value_type::reconunits: return u"reconunits";
			case value_type::reform: return u"reform";
			case value_type::region: return u"region";
			case value_type::rel: return u"rel";
			case value_type::relation: return u"relation";
			case value_type::req: return u"req";
			case value_type::reqlevel: return u"reqlevel";
			case value_type::required: return u"required";
			case value_type::resource: return u"resource";
			case value_type::result: return u"result";
			case value_type::rstate: return u"rstate";
			case value_type::rule: return u"rule";
			case value_type::runs: return u"runs";
			case value_type::scr: return u"scr";
			case value_type::sea: return u"sea";
			case value_type::second: return u"second";
			case value_type::second_country: return u"second_country";
			case value_type::self: return u"self";
			case value_type::sell: return u"sell";
			case value_type::setting: return u"setting";
			case value_type::ships: return u"ships";
			case value_type::size: return u"size";
			case value_type::skill: return u"skill";
			case value_type::source: return u"source";
			case value_type::speed: return u"speed";
			case value_type::spheremaster: return u"spheremaster";
			case value_type::state: return u"state";
			case value_type::statename: return u"statename";
			case value_type::str: return u"str";
			case value_type::strata: return u"strata";
			case value_type::strings_list_4: return u"strings_list_4";
			case value_type::string_0_0: return u"string_0_0";
			case value_type::string_0_1: return u"string_0_1";
			case value_type::string_0_2: return u"string_0_2";
			case value_type::string_0_3: return u"string_0_3";
			case value_type::string_0_4: return u"string_0_4";
			case value_type::string_9_0: return u"string_9_0";
			case value_type::sub: return u"sub";
			case value_type::table: return u"table";
			case value_type::tag: return u"tag";
			case value_type::tag0_0: return u"tag0_0";
			case value_type::tag_0_0: return u"tag_0_0";
			case value_type::tag_0_0_: return u"tag_0_0_";
			case value_type::tag_0_0_adj: return u"tag_0_0_adj";
			case value_type::tag_0_0_upper: return u"tag_0_0_upper";
			case value_type::tag_0_1: return u"tag_0_1";
			case value_type::tag_0_1_adj: return u"tag_0_1_adj";
			case value_type::tag_0_1_upper: return u"tag_0_1_upper";
			case value_type::tag_0_2: return u"tag_0_2";
			case value_type::tag_0_2_adj: return u"tag_0_2_adj";
			case value_type::tag_0_3: return u"tag_0_3";
			case value_type::tag_0_3_adj: return u"tag_0_3_adj";
			case value_type::tag_1_0: return u"tag_1_0";
			case value_type::tag_2_0: return u"tag_2_0";
			case value_type::tag_2_0_upper: return u"tag_2_0_upper";
			case value_type::tag_3_0: return u"tag_3_0";
			case value_type::tag_3_0_upper: return u"tag_3_0_upper";
			case value_type::target: return u"target";
			case value_type::targetlist: return u"targetlist";
			case value_type::target_country: return u"target_country";
			case value_type::tech: return u"tech";
			case value_type::temperature: return u"temperature";
			case value_type::terms: return u"terms";
			case value_type::terrain: return u"terrain";
			case value_type::terrainmod: return u"terrainmod";
			case value_type::text: return u"text";
			case value_type::theirlost: return u"theirlost";
			case value_type::theirnum: return u"theirnum";
			case value_type::theirship: return u"theirship";
			case value_type::their_lead: return u"their_lead";
			case value_type::their_num: return u"their_num";
			case value_type::their_res: return u"their_res";
			case value_type::them: return u"them";
			case value_type::third: return u"third";
			case value_type::threat: return u"threat";
			case value_type::time: return u"time";
			case value_type::title: return u"title";
			case value_type::to: return u"to";
			case value_type::tot: return u"tot";
			case value_type::totalemi: return u"totalemi";
			case value_type::totalimm: return u"totalimm";
			case value_type::tra: return u"tra";
			case value_type::truth: return u"truth";
			case value_type::type: return u"type";
			case value_type::unemployed: return u"unemployed";
			case value_type::vtype_union: return u"union";
			case value_type::union_adj: return u"union_adj";
			case value_type::unit: return u"unit";
			case value_type::units: return u"units";
			case value_type::until: return u"until";
			case value_type::usloss: return u"usloss";
			case value_type::usnum: return u"usnum";
			case value_type::val: return u"val";
			case value_type::value: return u"value";
			case value_type::value_int1: return u"value_int1";
			case value_type::value_int_0_0: return u"value_int_0_0";
			case value_type::value_int_0_1: return u"value_int_0_1";
			case value_type::value_int_0_2: return u"value_int_0_2";
			case value_type::value_int_0_3: return u"value_int_0_3";
			case value_type::value_int_0_4: return u"value_int_0_4";
			case value_type::verb: return u"verb";
			case value_type::versus: return u"versus";
			case value_type::war: return u"war";
			case value_type::wargoal: return u"wargoal";
			case value_type::we: return u"we";
			case value_type::what: return u"what";
			case value_type::where: return u"where";
			case value_type::which: return u"which";
			case value_type::who: return u"who";
			case value_type::winner: return u"winner";
			case value_type::x: return u"x";
			case value_type::y: return u"y";
			case value_type::year: return u"year";
			case value_type::years: return u"years";
			case value_type::chief_of_navy: return u"chief_of_navy";
			case value_type::chief_of_staff: return u"chief_of_staff";
			case value_type::control: return u"control";
			case value_type::head_of_government: return u"head_of_government";
			case value_type::owner: return u"owner";
			case value_type::playername: return u"playername";
			default: return u"";
		}
	}

	const replacement * find_replacement(value_placeholder placeholder, const replacement * candidates, uint32_t count) {
		for (int32_t i = int32_t(count) - 1; i >= 0; --i) {
			if (std::get<0>(candidates[i]) == placeholder.value)
				return &(candidates[i]);
		}
		return nullptr;
	}

	std::pair<int32_t, int32_t> align_in_bounds(text_data::alignment align, int32_t width, int32_t height, int32_t bound_x, int32_t bound_y) {
		int32_t x_offset = 0;
		if (align == text_data::alignment::right) {
			x_offset = bound_x - width;
		} else if (align == text_data::alignment::center) {
			x_offset = (bound_x - width) / 2;
		}
		return std::make_pair(x_offset, (bound_y - height) / 2);
	}


	text_tag get_text_handle(text_data::text_sequences& container, const char* key_start, const char* key_end) {
		if (key_start == key_end)
			return text_tag();

		char* const cpy = (char*)_alloca((size_t)(key_end - key_start + 1));
		memcpy(cpy, key_start, (size_t)(key_end - key_start));
		cpy[key_end - key_start] = 0;
		const auto find_result = container.key_to_sequence_map.find(cpy);

		if (find_result != container.key_to_sequence_map.end()) {
			return find_result->second;
		} else {
			text_data::add_win1250_text_to_container(container, key_start, key_end);
			const auto new_key = container.all_sequences.emplace_back(text_data::text_sequence{ static_cast<uint16_t>(container.all_components.size() - 1), 1 });

			container.key_to_sequence_map.emplace(vector_backed_string<char>(key_start, key_end, container.key_data), new_key);
			return new_key;
		}
	}

	text_tag get_thread_safe_text_handle(text_data::text_sequences& container, const char* key_start, const char* key_end) {
		if (key_start == key_end)
			return text_tag();

		char* const cpy = (char*)_alloca((size_t)(key_end - key_start + 1));
		memcpy(cpy, key_start, (size_t)(key_end - key_start));
		cpy[key_end - key_start] = 0;

		container.text_data_mutex.lock_shared();

		const auto find_result = container.key_to_sequence_map.find(cpy);

		if (find_result != container.key_to_sequence_map.end()) {
			const auto found_tag = find_result->second;
			container.text_data_mutex.unlock_shared();
			return found_tag;
		} else {
			container.text_data_mutex.unlock_shared();
			container.text_data_mutex.lock();

			text_data::add_win1250_text_to_container(container, key_start, key_end);
			const auto new_key = container.all_sequences.emplace_back(text_data::text_sequence{ static_cast<uint16_t>(container.all_components.size() - 1), 1 });

			container.key_to_sequence_map.emplace(vector_backed_string<char>(key_start, key_end, container.key_data), new_key);

			container.text_data_mutex.unlock();
			return new_key;
		}
	}

	text_tag get_thread_safe_existing_text_handle(const text_data::text_sequences& container, const char* key_start, const char* key_end) {
		if (key_start == key_end)
			return text_tag();

		char* const cpy = (char*)_alloca((size_t)(key_end - key_start + 1));
		memcpy(cpy, key_start, (size_t)(key_end - key_start));
		cpy[key_end - key_start] = 0;

		container.text_data_mutex.lock_shared();
		const auto find_result = container.key_to_sequence_map.find(cpy);

		if (find_result != container.key_to_sequence_map.end()) {
			const auto found_tag = find_result->second;
			container.text_data_mutex.unlock_shared();
			return found_tag;
		} else {
			container.text_data_mutex.unlock_shared();
			return text_tag();
		}
	}

	text_tag get_existing_text_handle(const text_data::text_sequences& container, const char* key_start, const char* key_end) {
		if (key_start == key_end)
			return text_tag();

		char* const cpy = (char*)_alloca((size_t)(key_end - key_start + 1));
		memcpy(cpy, key_start, (size_t)(key_end - key_start));
		cpy[key_end - key_start] = 0;
		const auto find_result = container.key_to_sequence_map.find(cpy);

		if (find_result != container.key_to_sequence_map.end()) {
			return find_result->second;
		} else {
			return text_tag();
		}
	}

	std::u16string to_string(const text_sequences& container, text_data::text_tag tag) {
		const auto& components = container.all_sequences[tag];

		const auto components_start = container.all_components.data() + components.starting_component;
		const auto components_end = components_start + components.component_count;

		std::u16string result;

		for(auto component_i = components_start; component_i != components_end; ++component_i) {
			if(std::holds_alternative<text_data::line_break>(*component_i)) {
				result += u"\r\n";
			} else if(std::holds_alternative<text_data::value_placeholder>(*component_i)) {
				const auto rep = std::get<text_data::value_placeholder>(*component_i);
				result += u'$';
				result += name_from_value_type(rep.value);
				result += u'$';
			} else if(std::holds_alternative<text_data::text_chunk>(*component_i)) {
				const auto chunk = std::get<text_data::text_chunk>(*component_i);
				result += std::u16string(container.text_data.data() + chunk.offset, container.text_data.data() + chunk.offset + chunk.length);
			}
		}

		return result;
	}

	vector_backed_string<char16_t> text_tag_to_backing(const text_sequences& container, text_data::text_tag tag) {
		if(!is_valid_index(tag))
			return vector_backed_string<char16_t>();

		const auto& components = container.all_sequences[tag];

		if(components.component_count == 0)
			return vector_backed_string<char16_t>();

		const auto components_start = container.all_components.data() + components.starting_component;

		if(std::holds_alternative<text_chunk>(*components_start))
			return vector_backed_string<char16_t>(std::get<text_chunk>(*components_start));
		else
			return vector_backed_string<char16_t>();
	}
}
