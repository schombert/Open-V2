#include "nations_io.h"
#include "world_state\\world_state.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "population\\population_function.h"
#include "nations_functions.h"
#include "governments\\governments_functions.h"

#undef max
#undef min

namespace nations {
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
		void set_upper_house_pair(std::pair<token_and_type, uint32_t> const& p) {
			const auto ihandle = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(p.first.start, p.first.end));
			investment.emplace_back(ihandle, p.second);
		}
	};
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
		void add_decision(token_and_type const& t) {
			std::string title = std::string(t.start, t.end) + "_title";
			const auto tt_title = text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, title.c_str(), title.c_str() + title.length());
			decisions.push_back(tag_from_text(ws.s.event_m.descisions_by_title_index, tt_title));
		}
		void set_primary_culture(token_and_type const& t) {
			primary_culture = tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_culture(token_and_type const& t) {
			accepted_cultures.push_back(tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void remove_culture(token_and_type const& t) {
			const auto chandle = tag_from_text(ws.s.culture_m.named_culture_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
			if(auto f = std::find(accepted_cultures.begin(), accepted_cultures.end(), chandle); f != accepted_cultures.end()) {
				*f = accepted_cultures.back();
				accepted_cultures.pop_back();
			} else {
				remove_accepted_cultures.push_back(chandle);
			}
		}
		void set_religion(token_and_type const& t) {
			religion = tag_from_text(ws.s.culture_m.named_religion_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void add_flag(token_and_type const& t) {
			set_flags.push_back(tag_from_text(ws.s.variables_m.named_national_flags, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end)));
		}
		void remove_flag(token_and_type const& t) {
			const auto fhandle = tag_from_text(ws.s.variables_m.named_national_flags, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
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
			nationalvalue = tag_from_text(ws.s.modifiers_m.named_national_modifiers_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_techschool(token_and_type const& t) {
			techschool = tag_from_text(ws.s.technology_m.named_tech_school_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_investment(investment_parse_obj&& o) {
			investment = std::move(o.investment);
		}
		void set_govt_flag(govt_flag const& o) {
			govt_flags.emplace_back(o.source, o.replacement);
		}
		void set_other(std::pair<token_and_type, token_and_type> const& p) {
			const auto left_text = text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			const auto tech = tag_from_text(ws.s.technology_m.named_technology_index, left_text);
			if(is_valid_index(tech)) {
				bit_vector_set(tech_bit_vector.data(), to_index(tech), is_fixed_token_ci(p.second, "yes") || is_fixed_token_ci(p.second, "1"));
			} else {
				const auto iss = tag_from_text(ws.s.issues_m.named_issue_index, left_text);
				if(is_valid_index(iss)) {
					const auto opt = tag_from_text(ws.s.issues_m.named_option_index, text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, p.second.start, p.second.end));
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

	void add_data_to_nation(world_state& ws, nation& target_nation, cultures::national_tag_state& nat_tag, cultures::national_tag nat_tag_id, nation_parse_object const& npo) {
		nat_tag.capital = npo.capital;
		target_nation.current_government = npo.gov;
		
		target_nation.primary_culture = npo.primary_culture;
		for(auto c : npo.accepted_cultures)
			add_item(ws.w.culture_s.culture_arrays, target_nation.accepted_cultures, c);
		target_nation.national_religion = npo.religion;
		for(auto f : npo.set_flags)
			add_item(ws.w.variable_s.national_flags_arrays, target_nation.national_flags, f);
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
					this_pop.literacy = npo.literacy ? static_cast<uint16_t>(*npo.literacy * float(std::numeric_limits<uint16_t>::max())) : 0ui16;
					this_pop.consciousness = npo.consciousness ? static_cast<uint16_t>(*npo.consciousness * float(std::numeric_limits<uint16_t>::max() / 10.0f)) : 0ui16;
				} else {
					this_pop.literacy = npo.non_state_culture_literacy ? static_cast<uint16_t>(*npo.non_state_culture_literacy * float(std::numeric_limits<uint16_t>::max())) : 0ui16;
					this_pop.consciousness = npo.nonstate_consciousness ? static_cast<uint16_t>(*npo.nonstate_consciousness * float(std::numeric_limits<uint16_t>::max() / 10.0f)) : 0ui16;
				}
			}
		}
		if(npo.plurality)
			target_nation.plurality = *npo.plurality;
		if(npo.prestige)
			target_nation.prestige = *npo.prestige;
		if(npo.civilized)
			target_nation.is_civilized = *npo.civilized;
		if(npo.is_releasbale_vassal)
			nat_tag.is_not_releasable = !(*npo.is_releasbale_vassal);
		target_nation.national_value = npo.nationalvalue;
		target_nation.tech_school = npo.techschool;

		for(auto& ip : npo.investment) {
			auto holding_nation = make_nation_for_tag(ws, ip.first);
			add_item(ws.w.nation_s.investment_arrays, target_nation.foreign_investment, investment_pair{ static_cast<float>(ip.second), holding_nation->id });
		}

		for(auto& ip : npo.govt_flags)
			ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.first) = ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.second);

		auto tech_row = ws.w.nation_s.active_technologies.get_row(target_nation.id);
		for(uint32_t i = 0; i < ws.s.technology_m.technologies_container.size(); ++i)
			tech_row[i] = npo.tech_bit_vector[i];

		auto issues_row = ws.w.nation_s.active_issue_options.get_row(target_nation.id);
		for(uint32_t i = 0; i < ws.s.issues_m.issues_container.size(); ++i)
			issues_row[i] = npo.set_options[i];

		if(is_valid_index(npo.ruling_party))
			governments::silent_set_ruling_party(ws, target_nation, npo.ruling_party);
			
		//not used: decisions, oob_file
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
