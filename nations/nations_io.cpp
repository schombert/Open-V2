#include "nations_io.h"
#include "world_state\\world_state.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "population\\population_function.h"
#include "nations_functions.h"
#include "governments\\governments_functions.h"
#include "military\\military_io.h"
#include "nations\\nations_functions.h"

#undef max
#undef min

namespace nations {
	struct relation_parse_obj {
		world_state& ws;
		date_tag start_date;
		date_tag end_date;
		cultures::national_tag first;
		cultures::national_tag second;

		relation_parse_obj(world_state& w, date_tag) : ws(w) {}

		void set_first(token_and_type const& t) {
			first = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end));
		}
		void set_second(token_and_type const& t) {
			second = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end));
		}
	};
	struct relation_file_reader {
		world_state& ws;
		date_tag filter_date;

		relation_file_reader(world_state& w, date_tag d) : ws(w), filter_date(d) {}

		void set_vassal(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				silent_make_vassal(ws, *make_nation_for_tag(ws, r.first), *make_nation_for_tag(ws, r.second));
		}
		void set_substate(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				silent_make_substate(ws, *make_nation_for_tag(ws, r.first), *make_nation_for_tag(ws, r.second));
		}
		void set_alliance(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				silent_make_alliance(ws, *make_nation_for_tag(ws, r.first), *make_nation_for_tag(ws, r.second));
		}
	};

	struct upper_house_parse_obj {
		world_state& ws;
		std::vector<uint32_t> upper_house;

		upper_house_parse_obj(world_state& w, date_tag) : ws(w) {
			upper_house.resize(ws.s.ideologies_m.ideologies_count);
		}
		void set_upper_house_pair(std::pair<token_and_type, uint32_t> const& p) {
			const auto ihandle = tag_from_text(ws.s.ideologies_m.named_ideology_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			upper_house[to_index(ihandle)] = p.second;
		}
	};
	struct investment_parse_obj {
		world_state& ws;
		std::vector<std::pair<cultures::national_tag, uint32_t>> investment;

		investment_parse_obj(world_state& w, date_tag) : ws(w) {}
		void set_investment_pair(std::pair<token_and_type, uint32_t> const& p) {
			const auto ihandle = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(p.first.start, p.first.end));
			investment.emplace_back(ihandle, p.second);
		}
	};

	inline std::pair<token_and_type, uint32_t> make_token_unit32_pair(token_and_type const& t, association_type, token_and_type const& r) {
		return std::pair<token_and_type, uint32_t>(t, token_to<uint32_t>(r));
	}
	inline std::pair<token_and_type, token_and_type> make_token_token_pair(token_and_type const& t, association_type, token_and_type const& r) {
		return std::pair<token_and_type, token_and_type>(t, r);
	}

	struct govt_flag {
		world_state& ws;
		governments::government_tag source;
		governments::government_tag replacement;

		govt_flag(world_state& w, date_tag) : ws(w) {}
		void set_government(token_and_type const& t) {
			source = tag_from_text(ws.s.governments_m.named_government_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_flag(token_and_type const& t) {
			replacement = tag_from_text(ws.s.governments_m.named_government_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
	};

	struct nation_parse_object;
	void combine_nation_parse_object(nation_parse_object& base, nation_parse_object& other);

	struct nation_parse_object {
		world_state& ws;
		provinces::province_tag capital;
		governments::government_tag gov;
		governments::party_tag ruling_party;
		std::vector<events::decision_tag> decisions;
		std::string oob_filename;
		cultures::culture_tag primary_culture;
		std::vector<cultures::culture_tag> accepted_cultures;
		std::vector<cultures::culture_tag> remove_accepted_cultures;
		cultures::religion_tag religion;
		std::vector<variables::national_flag_tag> set_flags;
		std::vector<variables::global_variable_tag> global_flags;
		std::vector<variables::national_flag_tag> clear_flags;
		std::vector<uint32_t> upper_house;
		date_tag last_election;
		std::optional<float> literacy;
		std::optional<float> non_state_culture_literacy;
		std::optional<float> plurality;
		std::optional<float> consciousness;
		std::optional<float> nonstate_consciousness;
		std::optional<float> prestige;
		std::optional<bool> civilized;
		std::optional<bool> is_releasbale_vassal;
		modifiers::national_modifier_tag nationalvalue;
		modifiers::national_modifier_tag techschool;
		std::vector<std::pair<cultures::national_tag, uint32_t>> investment;
		std::vector<std::pair<governments::government_tag, governments::government_tag>> govt_flags; //first = to be replaced, second = with flag of this type
		std::vector<uint64_t> tech_bit_vector;
		std::vector<issues::option_tag> set_options;

		date_tag target_date;

		nation_parse_object(world_state& w, date_tag t) : ws(w), target_date(t) {
			tech_bit_vector.resize(ws.s.technology_m.technologies_container.size() >> 6ui64);
			set_options.resize(ws.s.issues_m.issues_container.size());
		}

		void set_capital(uint16_t v) {
			capital = provinces::province_tag(v);
		}
		void set_oob(token_and_type const& t) {
			oob_filename = std::string(t.start, t.end);
		}
		void set_ruling_party(token_and_type const& t) {
			ruling_party = tag_from_text(ws.s.governments_m.named_party_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_decision(token_and_type const& t) {
			std::string title = std::string(t.start, t.end) + "_title";
			const auto tt_title = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, title.c_str(), title.c_str() + title.length());
			decisions.push_back(tag_from_text(ws.s.event_m.descisions_by_title_index, tt_title));
		}
		void set_primary_culture(token_and_type const& t) {
			primary_culture = tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_culture(token_and_type const& t) {
			accepted_cultures.push_back(tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void remove_culture(token_and_type const& t) {
			const auto chandle = tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
			if(auto f = std::find(accepted_cultures.begin(), accepted_cultures.end(), chandle); f != accepted_cultures.end()) {
				*f = accepted_cultures.back();
				accepted_cultures.pop_back();
			} else {
				remove_accepted_cultures.push_back(chandle);
			}
		}
		void set_religion(token_and_type const& t) {
			religion = tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_flag(token_and_type const& t) {
			set_flags.push_back(tag_from_text(ws.s.variables_m.named_national_flags, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void remove_flag(token_and_type const& t) {
			const auto fhandle = tag_from_text(ws.s.variables_m.named_national_flags, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
			if(auto f = std::find(set_flags.begin(), set_flags.end(), fhandle); f != set_flags.end()) {
				*f = set_flags.back();
				set_flags.pop_back();
			} else {
				clear_flags.push_back(fhandle);
			}
		}
		void set_upper_house(upper_house_parse_obj&& o) {
			upper_house = std::move(o.upper_house);
		}
		void set_last_election(token_and_type const& t) {
			last_election = parse_date(t.start, t.end);
		}
		void set_nationalvalue(token_and_type const& t) {
			nationalvalue = tag_from_text(ws.s.modifiers_m.named_national_modifiers_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_techschool(token_and_type const& t) {
			techschool = tag_from_text(ws.s.modifiers_m.named_national_modifiers_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_government(token_and_type const& t) {
			gov = tag_from_text(ws.s.governments_m.named_government_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_investment(investment_parse_obj&& o) {
			investment = std::move(o.investment);
		}
		void set_govt_flag(govt_flag const& o) {
			govt_flags.emplace_back(o.source, o.replacement);
		}
		void add_global_flag(token_and_type const& t) {
			global_flags.push_back(tag_from_text(ws.s.variables_m.named_global_variables, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void set_other(std::pair<token_and_type, token_and_type> const& p) {
			const auto left_text = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			const auto tech = tag_from_text(ws.s.technology_m.named_technology_index, left_text);
			if(is_valid_index(tech)) {
				bit_vector_set(tech_bit_vector.data(), to_index(tech), is_fixed_token_ci(p.second, "yes") || is_fixed_token_ci(p.second, "1"));
			} else {
				const auto iss = tag_from_text(ws.s.issues_m.named_issue_index, left_text);
				if(is_valid_index(iss)) {
					const auto opt = tag_from_text(ws.s.issues_m.named_option_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, p.second.start, p.second.end));
					if(ws.s.issues_m.options[opt].parent_issue == iss) {
						set_options[to_index(iss)] = opt;
					}
				}
			}
			
		}
		void add_dated_block(std::pair<token_and_type, nation_parse_object>&& p) {
			auto date = parse_date(p.first.start, p.first.end);
			if(date <= target_date)
				combine_nation_parse_object(*this, p.second);
		}
	};

	inline std::pair<token_and_type, nation_parse_object> name_dated_block(token_and_type const& t, association_type, nation_parse_object&& p) {
		return std::pair<token_and_type, nation_parse_object>(t, std::move(p));
	}

	inline void add_data_to_nation(world_state& ws, nation& target_nation, cultures::national_tag_state& nat_tag, cultures::national_tag nat_tag_id, directory const& units_dir, nation_parse_object& npo) {
		nat_tag.capital = npo.capital;
		target_nation.current_government = npo.gov;
		
		target_nation.primary_culture = npo.primary_culture;
		for(auto c : npo.accepted_cultures)
			add_item(ws.w.culture_s.culture_arrays, target_nation.accepted_cultures, c);
		target_nation.national_religion = npo.religion;
		for(auto f : npo.set_flags)
			add_item(ws.w.variable_s.national_flags_arrays, target_nation.national_flags, f);
		for(auto f : npo.global_flags)
			ws.w.variable_s.global_variables[f] = 1.0f;
		if(npo.upper_house.size() != 0) {
			auto uh_row = ws.w.nation_s.upper_house.get_row(target_nation.id);
			for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i)
				uh_row[i] = uint8_t(npo.upper_house[i]);
		}
		target_nation.last_election = npo.last_election;

		auto owned_provs_range = get_range(ws.w.province_s.province_arrays, target_nation.owned_provinces);
		for(auto i = owned_provs_range.first; i != owned_provs_range.second; ++i) {
			auto pops_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*i].pops);
			for(auto j = pops_range.first; j < pops_range.second; ++j) {
				auto& this_pop = ws.w.population_s.pops.get(*j);
				if(population::is_pop_accepted(ws, this_pop, target_nation)) {
					if(npo.literacy)
						population::set_literacy_direct(this_pop, *npo.literacy);
					if(npo.consciousness)
						population::set_consciousness_direct(this_pop, *npo.consciousness);
				} else {
					if(npo.non_state_culture_literacy)
						population::set_literacy_direct(this_pop, *npo.non_state_culture_literacy);
					if(npo.nonstate_consciousness)
						population::set_consciousness_direct(this_pop, *npo.nonstate_consciousness);
				}
			}
		}
		if(npo.plurality)
			target_nation.plurality = *npo.plurality;
		if(npo.prestige)
			target_nation.prestige = *npo.prestige;
		if(npo.civilized && *npo.civilized)
			target_nation.flags |= nation::is_civilized;
		if(npo.is_releasbale_vassal)
			nat_tag.is_not_releasable = !(*npo.is_releasbale_vassal);
		target_nation.national_value = npo.nationalvalue;
		target_nation.tech_school = npo.techschool;
		target_nation.current_government = npo.gov;

		target_nation.current_capital = npo.capital;
		for(auto& ip : npo.investment) {
			auto holding_nation = make_nation_for_tag(ws, ip.first);
			set_foreign_investment(ws, target_nation, holding_nation->id, static_cast<float>(ip.second));
		}

		for(auto& ip : npo.govt_flags)
			ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.first) = ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.second);

		auto tech_row = ws.w.nation_s.active_technologies.get_row(target_nation.id);
		for(uint32_t i = 0; i < npo.tech_bit_vector.size(); ++i)
			tech_row[i] = npo.tech_bit_vector[i];

		auto issues_row = ws.w.nation_s.active_issue_options.get_row(target_nation.id);
		for(uint32_t i = 0; i < ws.s.issues_m.issues_container.size(); ++i)
			issues_row[i] = npo.set_options[i];

		if(is_valid_index(npo.ruling_party))
			governments::silent_set_ruling_party(ws, target_nation, npo.ruling_party);
			
		for(uint32_t i = 0; i < npo.oob_filename.length(); ++i) {
			if(npo.oob_filename[i] == '/')
				npo.oob_filename[i] = '\\';
		}
		auto fi = units_dir.open_file(npo.oob_filename.c_str(), npo.oob_filename.c_str() + npo.oob_filename.length());
		if(fi) {
			const auto sz = fi->size();
			std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
			std::vector<token_group> parse_results;

			fi->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(parse_results.size() > 0)
				military::read_oob_file(ws, target_nation, parse_results.data(), parse_results.data() + parse_results.size());
		}
		//not used: decisions
	}

	void combine_nation_parse_object(nation_parse_object& base, nation_parse_object& other) {
		if(is_valid_index(other.capital))
			base.capital = other.capital;
		if(is_valid_index(other.gov))
			base.gov = other.gov;
		if(is_valid_index(other.ruling_party))
			base.ruling_party = other.ruling_party;
		for(auto d : other.decisions) {
			if(std::find(base.decisions.begin(), base.decisions.end(), d) == base.decisions.end())
				base.decisions.push_back(d);
		}
		for(auto f : other.global_flags)
			base.global_flags.push_back(f);
		if(other.oob_filename.length() != 0)
			base.oob_filename = other.oob_filename;
		if(is_valid_index(other.primary_culture))
			base.primary_culture = other.primary_culture;
		for(auto d : other.remove_accepted_cultures) {
			if(auto f = std::find(base.accepted_cultures.begin(), base.accepted_cultures.end(), d); f != base.accepted_cultures.end()) {
				*f = base.accepted_cultures.back();
				base.accepted_cultures.pop_back();
			}
		}
		for(auto d : other.accepted_cultures) {
			if(std::find(base.accepted_cultures.begin(), base.accepted_cultures.end(), d) == base.accepted_cultures.end())
				base.accepted_cultures.push_back(d);
		}
		if(is_valid_index(other.religion))
			base.religion = other.religion;
		for(auto d : other.clear_flags) {
			if(auto f = std::find(base.set_flags.begin(), base.set_flags.end(), d); f != base.set_flags.end()) {
				*f = base.set_flags.back();
				base.set_flags.pop_back();
			}
		}
		for(auto d : other.set_flags) {
			if(std::find(base.set_flags.begin(), base.set_flags.end(), d) == base.set_flags.end())
				base.set_flags.push_back(d);
		}
		if(other.upper_house.size() != 0) {
			for(uint32_t i = 0; i < other.upper_house.size(); ++i)
				base.upper_house[i] = other.upper_house[i];
		}
		if(is_valid_index(other.last_election))
			base.last_election = other.last_election;
		if(other.literacy)
			base.literacy = other.literacy;
		if(other.non_state_culture_literacy)
			base.non_state_culture_literacy = other.non_state_culture_literacy;
		if(other.plurality)
			base.plurality = other.plurality;
		if(other.consciousness)
			base.consciousness = other.consciousness;
		if(other.nonstate_consciousness)
			base.nonstate_consciousness = other.nonstate_consciousness;
		if(other.prestige)
			base.prestige = other.prestige;
		if(other.civilized)
			base.civilized = other.civilized;
		if(other.is_releasbale_vassal)
			base.is_releasbale_vassal = other.is_releasbale_vassal;
		if(is_valid_index(other.nationalvalue))
			base.nationalvalue = other.nationalvalue;
		if(is_valid_index(other.techschool))
			base.techschool = other.techschool;
		if(other.investment.size() != 0) {
			base.investment = std::move(other.investment);
		}
		for(auto o : other.govt_flags) {
			base.govt_flags.push_back(o);
		}
		for(uint32_t i = 0; i < other.tech_bit_vector.size(); ++i) {
			base.tech_bit_vector[i] |= other.tech_bit_vector[i];
		}
		for(uint32_t i = 0; i < other.set_options.size(); ++i) {
			if(is_valid_index(other.set_options[i]))
				base.set_options[i] = other.set_options[i];
		}
	}
}

MEMBER_FDEF(nations::relation_file_reader, set_substate, "substate");
MEMBER_FDEF(nations::relation_file_reader, set_vassal, "vassal");
MEMBER_FDEF(nations::relation_file_reader, set_alliance, "alliance");
MEMBER_DEF(nations::relation_parse_obj, start_date, "start_date");
MEMBER_DEF(nations::relation_parse_obj, end_date, "end_date");
MEMBER_FDEF(nations::relation_parse_obj, set_first, "first");
MEMBER_FDEF(nations::relation_parse_obj, set_second, "second");

MEMBER_FDEF(nations::upper_house_parse_obj, set_upper_house_pair, "upper_house_pair");
MEMBER_FDEF(nations::investment_parse_obj, set_investment_pair, "investment_pair");
MEMBER_FDEF(nations::govt_flag, set_government, "government");
MEMBER_FDEF(nations::govt_flag, set_flag, "flag");
MEMBER_FDEF(nations::nation_parse_object, set_capital, "capital");
MEMBER_FDEF(nations::nation_parse_object, add_decision, "decision");
MEMBER_FDEF(nations::nation_parse_object, set_primary_culture, "primary_culture");
MEMBER_FDEF(nations::nation_parse_object, add_culture, "culture");
MEMBER_FDEF(nations::nation_parse_object, remove_culture, "remove_culture");
MEMBER_FDEF(nations::nation_parse_object, set_religion, "religion");
MEMBER_FDEF(nations::nation_parse_object, add_flag, "add_flag");
MEMBER_FDEF(nations::nation_parse_object, remove_flag, "remove_flag");
MEMBER_FDEF(nations::nation_parse_object, set_upper_house, "upper_house");
MEMBER_FDEF(nations::nation_parse_object, set_last_election, "last_election");
MEMBER_FDEF(nations::nation_parse_object, set_nationalvalue, "nationalvalue");
MEMBER_FDEF(nations::nation_parse_object, set_techschool, "school");
MEMBER_FDEF(nations::nation_parse_object, set_investment, "investment");
MEMBER_FDEF(nations::nation_parse_object, set_govt_flag, "govt_flag");
MEMBER_FDEF(nations::nation_parse_object, add_global_flag, "global_flag");
MEMBER_FDEF(nations::nation_parse_object, set_other, "other");
MEMBER_FDEF(nations::nation_parse_object, set_government, "government");
MEMBER_FDEF(nations::nation_parse_object, add_dated_block, "dated_block");
MEMBER_FDEF(nations::nation_parse_object, set_oob, "oob");
MEMBER_DEF(nations::nation_parse_object, literacy, "literacy");
MEMBER_DEF(nations::nation_parse_object, non_state_culture_literacy, "non_state_culture_literacy");
MEMBER_DEF(nations::nation_parse_object, plurality, "plurality");
MEMBER_DEF(nations::nation_parse_object, consciousness, "consciousness");
MEMBER_DEF(nations::nation_parse_object, nonstate_consciousness, "nonstate_consciousness");
MEMBER_DEF(nations::nation_parse_object, prestige, "prestige");
MEMBER_DEF(nations::nation_parse_object, civilized, "civilized");
MEMBER_FDEF(nations::nation_parse_object, set_ruling_party, "ruling_party");
MEMBER_DEF(nations::nation_parse_object, is_releasbale_vassal, "is_releasbale_vassal");

namespace nations {
	BEGIN_DOMAIN(relation_file)
		BEGIN_TYPE(relation_parse_obj)
			MEMBER_ASSOCIATION("start_date", "start_date", value_from_rh<date_tag>)
			MEMBER_ASSOCIATION("end_date", "end_date", value_from_rh<date_tag>)
			MEMBER_ASSOCIATION("first", "first", token_from_rh)
			MEMBER_ASSOCIATION("second", "second", token_from_rh)
		END_TYPE
		BEGIN_TYPE(relation_file_reader)
			MEMBER_TYPE_ASSOCIATION("alliance", "alliance", relation_parse_obj)
			MEMBER_TYPE_ASSOCIATION("substate", "substate", relation_parse_obj)
			MEMBER_TYPE_ASSOCIATION("vassal", "vassal", relation_parse_obj)
			MEMBER_TYPE_ASSOCIATION("vassal", "union", relation_parse_obj)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(nation_file)
		BEGIN_TYPE(upper_house_parse_obj)
		MEMBER_VARIABLE_ASSOCIATION("upper_house_pair", accept_all, make_token_unit32_pair)
		END_TYPE
		BEGIN_TYPE(investment_parse_obj)
		MEMBER_VARIABLE_ASSOCIATION("investment_pair", accept_all, make_token_unit32_pair)
		END_TYPE
		BEGIN_TYPE(govt_flag)
		MEMBER_ASSOCIATION("government", "government", token_from_rh)
		MEMBER_ASSOCIATION("flag", "flag", token_from_rh)
		END_TYPE
		BEGIN_TYPE(nation_parse_object)
		MEMBER_ASSOCIATION("capital", "capital", value_from_rh<uint16_t>)
		MEMBER_ASSOCIATION("decision", "decision", token_from_rh)
		MEMBER_ASSOCIATION("primary_culture", "primary_culture", token_from_rh)
		MEMBER_ASSOCIATION("government", "government", token_from_rh)
		MEMBER_ASSOCIATION("ruling_party", "ruling_party", token_from_rh)
		MEMBER_ASSOCIATION("culture", "culture", token_from_rh)
		MEMBER_ASSOCIATION("remove_culture", "remove_culture", token_from_rh)
		MEMBER_ASSOCIATION("religion", "religion", token_from_rh)
		MEMBER_ASSOCIATION("add_flag", "set_country_flag", token_from_rh)
		MEMBER_ASSOCIATION("remove_flag", "clr_country_flag", token_from_rh)
		MEMBER_ASSOCIATION("last_election", "last_election", token_from_rh)
		MEMBER_ASSOCIATION("nationalvalue", "nationalvalue", token_from_rh)
		MEMBER_ASSOCIATION("school", "schools", token_from_rh)
		MEMBER_ASSOCIATION("global_flag", "set_global_flag", token_from_rh)
		MEMBER_ASSOCIATION("oob", "oob", token_from_rh)
		MEMBER_TYPE_ASSOCIATION("upper_house", "upper_house", upper_house_parse_obj)
		MEMBER_TYPE_ASSOCIATION("investment", "foreign_investment", investment_parse_obj)
		MEMBER_TYPE_ASSOCIATION("govt_flag", "govt_flag", govt_flag)
		MEMBER_ASSOCIATION("literacy", "literacy", value_from_rh<float>)
		MEMBER_ASSOCIATION("non_state_culture_literacy", "non_state_culture_literacy", value_from_rh<float>)
		MEMBER_ASSOCIATION("plurality", "plurality", value_from_rh<float>)
		MEMBER_ASSOCIATION("consciousness", "consciousness", value_from_rh<float>)
		MEMBER_ASSOCIATION("nonstate_consciousness", "nonstate_consciousness", value_from_rh<float>)
		MEMBER_ASSOCIATION("prestige", "prestige", value_from_rh<float>)
		MEMBER_ASSOCIATION("civilized", "civilized", value_from_rh<bool>)
		MEMBER_ASSOCIATION("is_releasbale_vassal", "is_releasbale_vassal", value_from_rh<bool>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("dated_block", accept_all, nation_parse_object, name_dated_block)
		MEMBER_VARIABLE_ASSOCIATION("other", accept_all, make_token_token_pair)
		END_TYPE
	END_DOMAIN;

	void read_nation_file(world_state& ws, cultures::national_tag nat_tag_id, date_tag target_date, file const& fi, directory const& units_dir, std::vector<std::pair<country_tag, events::decision_tag>>& decisions) {
		const auto sz = fi.size();
		std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
		std::vector<token_group> parse_results;

		fi.read_to_buffer(parse_data.get(), sz);
		parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

		if(parse_results.size() != 0) {
			nation* this_nation = make_nation_for_tag(ws, nat_tag_id);
			auto npo = parse_object<nation_parse_object, nation_file>(parse_results.data(), parse_results.data() + parse_results.size(), ws, target_date);
			add_data_to_nation(ws, *this_nation, ws.w.culture_s.national_tags_state[nat_tag_id], nat_tag_id, units_dir, npo);
			for(auto d : npo.decisions)
				decisions.emplace_back(this_nation->id, d);
		}
	}

	void read_nations_files(world_state& ws, date_tag target_date, directory const& root, std::vector<std::pair<country_tag, events::decision_tag>>& decisions) {
		auto history_dir = root.get_directory(u"\\history");
		auto countries_dir = history_dir.get_directory(u"\\countries");
		auto units_dir = history_dir.get_directory(u"\\units");

		auto countries_files = countries_dir.list_files(u".txt");
		for(auto f : countries_files) {
			auto fname = f.file_name();
			if(fname.length() >= 3) {
				char tag_buffer[3] = { '\0' };

				tag_buffer[0] = char(fname[0]);
				tag_buffer[1] = char(fname[1]);
				tag_buffer[2] = char(fname[2]);

				if(tag_buffer[0] != 'R' || tag_buffer[1] != 'E' || tag_buffer[2] != 'B') {
					auto nation_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(tag_buffer, tag_buffer + 3));
					auto open_file = f.open_file();
					if(is_valid_index(nation_tag) && open_file) {
						read_nation_file(ws, nation_tag, target_date, *open_file, units_dir, decisions);
					}
				}

			}
		}
	}

	void read_diplomacy_files(world_state& ws, date_tag target_date, directory const& root) {
		auto history_dir = root.get_directory(u"\\history");
		auto diplomacy_dir = history_dir.get_directory(u"\\diplomacy");

		auto dip_files = diplomacy_dir.list_files(u".txt");
		for(auto f : dip_files) {
			if(auto open_file = f.open_file(); open_file) {
				const auto sz = open_file->size();
				std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
				std::vector<token_group> parse_results;

				open_file->read_to_buffer(parse_data.get(), sz);
				parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

				parse_object<relation_file_reader, relation_file>(parse_results.data(), parse_results.data() + parse_results.size(), ws, target_date);
			}
		}
	}
}
