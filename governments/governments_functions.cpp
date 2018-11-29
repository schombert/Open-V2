#include "common\\common.h"
#include "governments_functions.h"
#include "world_state\\world_state.h"
#include "nations\\nations.h"

namespace governments {
	issues::rules make_party_rules(party& p, scenario::scenario_manager& s) {
		issues::rules result;
		issues::option_tag* opts = s.governments_m.party_issues.get_row(p.id);
		for(uint32_t i = 0; i < s.governments_m.party_issues.inner_size(); ++i) {
			if(is_valid_index(opts[i]))
				result.rules_value |= s.issues_m.options[opts[i]].issue_rules.rules_settings.rules_value;
		}
		return result;
	}

	void get_best_parties_at_date(tagged_array_view<party_tag, ideologies::ideology_tag, false> parties_out, cultures::national_tag tag, date_tag date, scenario::scenario_manager& s) {
		uint32_t* rules_count = (uint32_t*)_alloca(sizeof(uint32_t) * s.ideologies_m.ideologies_count);
		for(int32_t i = 0; i < int32_t(s.ideologies_m.ideologies_count); ++i) {
			auto itag = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i));
			parties_out[itag] = party_tag();
			rules_count[i] = 0ui32;
		}
		auto const& tag_obj = s.culture_m.national_tags[tag];
		for(auto p_index = tag_obj.first_party; p_index != tag_obj.last_party; p_index = party_tag(to_index(p_index) + 1)) {
			party& this_party = s.governments_m.parties[p_index];

			if(this_party.start_date <= date && date < this_party.end_date) {
				uint32_t ideology_index = to_index(this_party.ideology);
#ifdef __llvm__
				uint32_t allowed_count = uint32_t(__builtin_popcount(this_party.party_rules.rules_value));
#else
				uint32_t allowed_count = uint32_t(__popcnt(this_party.party_rules.rules_value));
#endif
				if(allowed_count > rules_count[ideology_index]) {
					auto itag = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(ideology_index));
					parties_out[itag] = p_index;
					rules_count[ideology_index] = allowed_count;
				}
			}
		}
		
	}

	void silent_set_ruling_party(world_state& ws, nations::country_tag this_nation, party_tag p) {
		ws.w.nation_s.nations.set<nation::ruling_party>(this_nation, p);
		ws.w.nation_s.nations.set<nation::ruling_ideology>(this_nation, ws.s.governments_m.parties[p].ideology);

		issues::option_tag* row = ws.s.governments_m.party_issues.get_row(p);
		const auto sz_party_issues = ws.s.governments_m.party_issues.inner_size();

		for(uint32_t i = 0; i < sz_party_issues; ++i) {
			auto issue_tag = ws.s.issues_m.options[row[i]].parent_issue;
			ws.w.nation_s.active_issue_options.get(this_nation, issue_tag) = row[i];
		}

		update_current_rules(ws, this_nation);
	}

	void reset_upper_house(world_state& ws, nations::country_tag id) {
		auto upper_house = ws.w.nation_s.upper_house.get_row(id);

		Eigen::Map<Eigen::Matrix<uint8_t, -1, 1>>(upper_house.data(), ws.s.ideologies_m.ideologies_count) =
			Eigen::Matrix<uint8_t, -1, 1>::Zero(ws.s.ideologies_m.ideologies_count);

		upper_house[ws.s.ideologies_m.conservative_ideology] = 100ui8;
	}

	void update_current_rules(world_state& ws, nations::country_tag this_nation) {
		auto& rules_value = ws.w.nation_s.nations.get<nation::current_rules>(this_nation).rules_value;
		rules_value = 0ui32;

		uint32_t max_issues = uint32_t(ws.s.issues_m.issues_container.size());
		auto issues_row = ws.w.nation_s.active_issue_options.get_row(this_nation);

		for(int32_t i = 0; i < int32_t(max_issues); ++i) {
			auto i_tag = issues::issue_tag(issues::issue_tag::value_base_t(i));
			if(is_valid_index(issues_row[i_tag])) {
				rules_value |= ws.s.issues_m.options[issues_row[i_tag]].issue_rules.rules_settings.rules_value;
			}
		}
	}

	void silent_set_government(world_state& ws, nations::country_tag this_nation, government_tag g) {
		auto& gov = ws.s.governments_m.governments_container[g];

		ws.w.nation_s.nations.set<nation::current_government>(this_nation, g);

		if(gov.election == false)
			ws.w.nation_s.nations.set<nation::is_holding_election>(this_nation, false);
		
		if(ws.s.governments_m.permitted_ideologies.get(g, ws.w.nation_s.nations.get<nation::ruling_ideology>(this_nation)) == 0) {
			ideologies::ideology_tag best_fit;
			int32_t best_support = 0;
			for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
				ideologies::ideology_tag ctag(static_cast<ideologies::ideology_tag::value_base_t>(i));
				if(ws.s.governments_m.permitted_ideologies.get(g, ctag) != 0 &&
					int32_t(ws.w.nation_s.upper_house.get(this_nation, ctag)) > best_support) {
					best_support = int32_t(ws.w.nation_s.upper_house.get(this_nation, ctag));
					best_fit = ctag;
				}
			}

			silent_set_ruling_party(ws, this_nation, ws.w.nation_s.active_parties.get(this_nation, best_fit));
		}

		auto tag = ws.w.nation_s.nations.get<nation::tag>(this_nation);
		if(is_valid_index(tag)) {
			auto names = ws.s.culture_m.country_names_by_government.get(tag, g);

			if(is_valid_index(names.name))
				ws.w.nation_s.nations.set<nation::name>(this_nation, names.name);
			else
				ws.w.nation_s.nations.set<nation::name>(this_nation,ws.s.culture_m.national_tags[tag].default_name.name);

			if(is_valid_index(names.adjective))
				ws.w.nation_s.nations.set<nation::adjective>(this_nation, names.adjective);
			else
				ws.w.nation_s.nations.set<nation::adjective>(this_nation, ws.s.culture_m.national_tags[tag].default_name.adjective);

			ws.w.nation_s.nations.set<nation::flag>(this_nation, ws.w.culture_s.country_flags_by_government.get(tag, g));
		}
	}

	void start_election(world_state& ws, nations::country_tag this_nation) {
		ws.w.nation_s.nations.set<nation::is_holding_election>(this_nation, true);
		ws.w.nation_s.nations.set<nation::last_election>(this_nation, ws.w.current_date);
	}

	date_tag election_end_date(world_state const& ws, nations::country_tag this_nation) {
		return date_tag(static_cast<date_tag::value_base_t>(
			to_index(ws.w.nation_s.nations.get<nation::last_election>(this_nation)) + int32_t(ws.s.modifiers_m.global_defines.campaign_duration) * 30));
	}

	date_tag next_election_start_date(world_state const& ws, nations::country_tag this_nation) {
		if(auto gov = ws.w.nation_s.nations.get<nation::current_government>(this_nation); is_valid_index(gov)) {
			auto months_until = int32_t(ws.s.governments_m.governments_container[gov].duration);
			auto last_election = ws.w.nation_s.nations.get<nation::last_election>(this_nation);
			if(is_valid_index(last_election)) {
				auto n_last_election = tag_to_date(last_election);
				n_last_election += boost::gregorian::months(months_until);
				return date_to_tag(n_last_election);
			} else {
				return ws.w.current_date;
			}
		}
		return date_tag();
	}
}
