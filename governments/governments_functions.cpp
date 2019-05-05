#include "common\\common.h"
#include "governments_functions.h"
#include "world_state\\world_state.h"
#include "nations\\nations.h"
#include "nations\nations_functions.hpp"
#include "provinces\province_functions.hpp"

namespace governments {
	issues::rules make_party_rules(party& p, scenario::scenario_manager& s) {
		issues::rules result;
		auto opts = s.governments_m.party_issues.get_row(p.id);
		for(uint32_t i = 0; i < s.governments_m.party_issues.inner_size(); ++i) {
			if(is_valid_index(opts[i]))
				result.rules_value |= s.issues_m.options[opts[i]].issue_rules.rules_settings.rules_value;
		}
		return result;
	}

	void get_best_parties_at_date(tagged_array_view<party_tag, ideologies::ideology_tag> parties_out, cultures::national_tag tag, date_tag date, scenario::scenario_manager& s) {
		uint32_t* rules_count = (uint32_t*)_alloca(sizeof(uint32_t) * s.ideologies_m.ideologies_count);
		for(int32_t i = 0; i < int32_t(s.ideologies_m.ideologies_count); ++i) {
			auto itag = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i));
			parties_out[itag] = party_tag();
			rules_count[i] = 0ui32;
		}
		auto const& tag_obj = s.culture_m.national_tags[tag];
		for(auto p_index = tag_obj.first_party; p_index != tag_obj.last_party; p_index = party_tag(party_tag::value_base_t(to_index(p_index) + 1))) {
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

		auto row = ws.s.governments_m.party_issues.get_row(p);
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
		auto& rules_value = ws.w.nation_s.nations.get<nation::current_rules>(this_nation);
		rules_value = 0ui32;

		int32_t max_issues = int32_t(ws.s.issues_m.issues_container.size());

		for(int32_t i = 0; i < max_issues; ++i) {
			auto i_tag = issues::issue_tag(issues::issue_tag::value_base_t(i));
			auto opt = ws.w.nation_s.active_issue_options.get(this_nation, i_tag);
			if(is_valid_index(opt)) {
				rules_value |= ws.s.issues_m.options[opt].issue_rules.rules_settings.rules_value;
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

	void pop_voting_preferences(world_state const& ws, population::pop_tag p,
		tagged_array_view<const party_tag, ideologies::ideology_tag> active_parties,
		tagged_array_view<float, ideologies::ideology_tag> effective_voting, float multiplier) {

		auto const icount = ws.s.ideologies_m.ideologies_count;
		auto const party_issue_count = ws.s.issues_m.party_issues.size();
		auto const ideology_vsize = ve::to_vector_size(icount);

		ve::set_zero(ideology_vsize, effective_voting, ve::serial{});

		for(int32_t i = 0; i < int32_t(icount); ++i) {
			auto const itag = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i));
			auto const ptag = active_parties[itag];

			if(ptag) {
				const auto pcon = ws.w.population_s.pops.get<pop::consciousness>(p);
				const auto pcon_complement = 1.0f - pcon;

				auto const ideology_support = pcon_complement * ws.w.population_s.pop_demographics.get(p, population::to_demo_tag(ws, itag));

				auto const pissues = ws.s.governments_m.party_issues.get_row(ptag);
				float issue_support = std::transform_reduce(pissues.begin(), pissues.begin() + party_issue_count, 0.0f, std::plus{}, [pcon, p, &ws](issues::option_tag s) {
					return pcon * ws.w.population_s.pop_demographics.get(p, population::to_demo_tag(ws, s));
				});

				effective_voting[itag] = ideology_support + issue_support;
			}
		}
		auto const total_support = ve::reduce(ideology_vsize, effective_voting, ve::serial{});
		if(total_support != 0.0f)
			ve::rescale(ideology_vsize, effective_voting, ws.w.population_s.pops.get<pop::size>(p) * multiplier / total_support);
	}

	void unmodified_province_vote(world_state const& ws, provinces::province_tag p, tagged_array_view<float, ideologies::ideology_tag> province_voting,
		tagged_array_view<const party_tag, ideologies::ideology_tag> active_parties) {

		auto const icount = ws.s.ideologies_m.ideologies_count;
		auto const ideology_vsize = ve::to_vector_size(icount);
		float* const pop_vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

		ve::set_zero(ideology_vsize, province_voting, ve::serial{});

		provinces::for_each_pop(ws, p, [&ws, province_voting, pop_vote, ideology_vsize, active_parties](population::pop_tag o) {
			pop_voting_preferences(ws, o, active_parties, tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), 1.0f);
			ve::accumulate(ideology_vsize, province_voting, tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), ve::serial{});
		});
	}

	void additive_province_vote(world_state const& ws, provinces::province_tag p, tagged_array_view<float, ideologies::ideology_tag> province_voting,
		cultures::culture_tag prime_culture, tagged_array_view<const party_tag, ideologies::ideology_tag> active_parties,
		float national_poor_vote, float national_middle_vote, float national_rich_vote, float accepted_culture_vote, float other_culture_vote) {

		const float province_vote_mod = 1.0f + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::number_of_voters>(p);

		auto const icount = ws.s.ideologies_m.ideologies_count;
		auto const ideology_vsize = ve::to_vector_size(icount);
		float* const pop_vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

		ve::set_zero(ideology_vsize, province_voting, ve::serial{});

		provinces::for_each_pop(ws, p, [&ws, province_voting, prime_culture, national_poor_vote, national_middle_vote, national_rich_vote,
			accepted_culture_vote, other_culture_vote, pop_vote, ideology_vsize, active_parties](population::pop_tag o) {

			auto const type = ws.w.population_s.pops.get<pop::type>(o);
			if(ws.s.population_m.slave != type) {
				float type_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::is_poor>(o))
					type_mod = national_poor_vote;
				else if(ws.w.population_s.pops.get<pop::is_middle>(o))
					type_mod = national_middle_vote;
				else
					type_mod = national_rich_vote;

				float culture_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::culture>(o) == prime_culture)
					culture_mod = 1.0f;
				else if(ws.w.population_s.pops.get<pop::is_accepted>(o))
					culture_mod = accepted_culture_vote;
				else
					culture_mod = other_culture_vote;

				const float vote_mod = type_mod * culture_mod;
				if(vote_mod > 0.0f) {
					pop_voting_preferences(ws, o, active_parties, tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), vote_mod);
					ve::accumulate(ideology_vsize, province_voting, tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), ve::serial{});
				}
			}
		});
		

		ve::rescale(ideology_vsize, province_voting, province_vote_mod);
	}

	void normalized_province_vote(world_state const& ws, provinces::province_tag p, tagged_array_view<float, ideologies::ideology_tag> province_voting,
		cultures::culture_tag prime_culture, tagged_array_view<const party_tag, ideologies::ideology_tag> active_parties,
		float national_poor_vote, float national_middle_vote, float national_rich_vote, float accepted_culture_vote, float other_culture_vote) {

		auto const icount = ws.s.ideologies_m.ideologies_count;
		auto const ideology_vsize = ve::to_vector_size(icount);
		float* const pop_vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

		ve::set_zero(ideology_vsize, province_voting, ve::serial{});

		provinces::for_each_pop(ws, p, [&ws, province_voting, prime_culture, national_poor_vote, national_middle_vote, national_rich_vote,
			accepted_culture_vote, other_culture_vote, pop_vote, ideology_vsize, active_parties](population::pop_tag o) {

			auto const type = ws.w.population_s.pops.get<pop::type>(o);
			if(ws.s.population_m.slave != type) {
				float type_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::is_poor>(o))
					type_mod = national_poor_vote;
				else if(ws.w.population_s.pops.get<pop::is_middle>(o))
					type_mod = national_middle_vote;
				else
					type_mod = national_rich_vote;

				float culture_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::culture>(o) == prime_culture)
					culture_mod = 1.0f;
				else if(ws.w.population_s.pops.get<pop::is_accepted>(o))
					culture_mod = accepted_culture_vote;
				else
					culture_mod = other_culture_vote;

				const float vote_mod = type_mod * culture_mod;
				if(vote_mod > 0.0f) {
					pop_voting_preferences(ws, o, active_parties, tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), vote_mod);
					ve::accumulate(ideology_vsize, province_voting, tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), ve::serial{});
				}
			}
		});

		auto const total_support = ve::reduce(ideology_vsize, province_voting, ve::serial{});
		if(total_support != 0.0f)
			ve::rescale(ideology_vsize, province_voting, 1.0f / total_support);
	}

	void majority_province_vote(world_state const& ws, provinces::province_tag p, tagged_array_view<float, ideologies::ideology_tag> province_voting,
		cultures::culture_tag prime_culture, tagged_array_view<const party_tag, ideologies::ideology_tag> active_parties,
		float national_poor_vote, float national_middle_vote, float national_rich_vote, float accepted_culture_vote, float other_culture_vote) {

		const float province_vote_mod = 1.0f + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::number_of_voters>(p);

		auto const icount = ws.s.ideologies_m.ideologies_count;
		auto const ideology_vsize = ve::to_vector_size(icount);

		float* const pop_vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));
		float* const province_voting_temp = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

		ve::set_zero(ideology_vsize, tagged_array_view<float, ideologies::ideology_tag>(province_voting_temp, ideology_vsize), ve::serial{});
		ve::set_zero(ideology_vsize, province_voting, ve::serial{});

		provinces::for_each_pop(ws, p, [&ws, province_voting_temp, prime_culture, national_poor_vote, national_middle_vote, national_rich_vote,
			accepted_culture_vote, other_culture_vote, pop_vote, ideology_vsize, active_parties](population::pop_tag o) {

			auto const type = ws.w.population_s.pops.get<pop::type>(o);
			if(ws.s.population_m.slave != type) {
				float type_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::is_poor>(o))
					type_mod = national_poor_vote;
				else if(ws.w.population_s.pops.get<pop::is_middle>(o))
					type_mod = national_middle_vote;
				else
					type_mod = national_rich_vote;

				float culture_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::culture>(o) == prime_culture)
					culture_mod = 1.0f;
				else if(ws.w.population_s.pops.get<pop::is_accepted>(o))
					culture_mod = accepted_culture_vote;
				else
					culture_mod = other_culture_vote;

				const float vote_mod = type_mod * culture_mod;
				if(vote_mod > 0.0f) {
					pop_voting_preferences(ws, o, active_parties, tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), vote_mod);
					ve::accumulate(ideology_vsize, tagged_array_view<float, ideologies::ideology_tag>(province_voting_temp, ideology_vsize), tagged_array_view<float, ideologies::ideology_tag>(pop_vote, ideology_vsize), ve::serial{});
				}
			}
		});

		auto const max_index = maximum_index(province_voting_temp, ideology_vsize);
		province_voting[ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(max_index))] = province_vote_mod * province_voting_temp[max_index];
	}

	void populate_voting_info(world_state const& ws, nations::country_tag this_nation,
		tagged_array_view<float, ideologies::ideology_tag> effective_voting,
		voting_type vtype) {

		auto const ideology_vsize = ve::to_vector_size(ws.s.ideologies_m.ideologies_count);

		float* const prov_vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

		ve::set_zero(ideology_vsize, effective_voting, ve::serial{});

		const float national_poor_vote = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_vote>(this_nation);
		const float national_middle_vote = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_vote>(this_nation);
		const float national_rich_vote = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_vote>(this_nation);

		auto const national_rules = ws.w.nation_s.nations.get<nation::current_rules>(this_nation);

		const float accepted_culture_vote = 
			(national_rules & issues::rules::citizens_rights_mask) == issues::rules::primary_culture_voting ? 0.5f : 1.0f;
		const float other_culture_vote = 
			(national_rules & issues::rules::citizens_rights_mask) == issues::rules::primary_culture_voting ? 1.0f 
			: ((national_rules & issues::rules::citizens_rights_mask) == issues::rules::culture_voting ? 0.5f : 0.0f);

		const auto pculture = ws.w.nation_s.nations.get<nation::primary_culture>(this_nation);
		const auto active_parties = ws.w.nation_s.active_parties.get_row(this_nation);

		switch(vtype) {
			case voting_type::unmodified:
				nations::for_each_province(ws, this_nation, [effective_voting, accepted_culture_vote, other_culture_vote,
					national_poor_vote, national_middle_vote, national_rich_vote, pculture, prov_vote, ideology_vsize, active_parties,
					&ws](provinces::province_tag p) {

					if(ws.w.province_s.province_state_container.get<province_state::is_non_state>(p) == false) {
						unmodified_province_vote(ws, p, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize), active_parties);
						ve::accumulate(ideology_vsize, effective_voting, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize), ve::serial{});
					}
				});
				break;
			case voting_type::additive:
				nations::for_each_province(ws, this_nation, [effective_voting, national_rules, accepted_culture_vote, other_culture_vote,
					national_poor_vote, national_middle_vote, national_rich_vote, pculture, prov_vote, ideology_vsize, active_parties,
					&ws](provinces::province_tag p) {

					if(ws.w.province_s.province_state_container.get<province_state::is_non_state>(p) == false) {
						additive_province_vote(ws, p, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize),
							pculture, active_parties, national_poor_vote, national_middle_vote, national_rich_vote, accepted_culture_vote, other_culture_vote);
						ve::accumulate(ideology_vsize, effective_voting, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize), ve::serial{});
					}
				});
				break;
			case voting_type::normalized:
				nations::for_each_province(ws, this_nation, [effective_voting, national_rules, accepted_culture_vote, other_culture_vote,
					national_poor_vote, national_middle_vote, national_rich_vote, pculture, prov_vote, ideology_vsize, active_parties,
					&ws](provinces::province_tag p) {

					if(ws.w.province_s.province_state_container.get<province_state::is_non_state>(p) == false) {
						normalized_province_vote(ws, p, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize),
							pculture, active_parties, national_poor_vote, national_middle_vote, national_rich_vote, accepted_culture_vote, other_culture_vote);
						ve::accumulate(ideology_vsize, effective_voting, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize), ve::serial{});
					}
				});
				break;
			case voting_type::majority:
				nations::for_each_province(ws, this_nation, [effective_voting, national_rules, accepted_culture_vote, other_culture_vote,
					national_poor_vote, national_middle_vote, national_rich_vote, pculture, prov_vote, ideology_vsize, active_parties,
					&ws](provinces::province_tag p) {

					if(ws.w.province_s.province_state_container.get<province_state::is_non_state>(p) == false) {
						majority_province_vote(ws, p, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize),
							pculture, active_parties, national_poor_vote, national_middle_vote, national_rich_vote, accepted_culture_vote, other_culture_vote);
						ve::accumulate(ideology_vsize, effective_voting, tagged_array_view<float, ideologies::ideology_tag>(prov_vote, ideology_vsize), ve::serial{});
					}
				});
				break;
		}
	}
	ideologies::ideology_tag elect_ideology(world_state const& ws, nations::country_tag this_nation) {
		auto const national_rules = ws.w.nation_s.nations.get<nation::current_rules>(this_nation);
		voting_type vtype =
			(national_rules & issues::rules::voting_system_mask) == issues::rules::largest_share ?
			voting_type::majority : ((national_rules & issues::rules::voting_system_mask) == issues::rules::dhont ? voting_type::normalized : voting_type::additive);

		auto const ideology_vsize = ve::to_vector_size(ws.s.ideologies_m.ideologies_count);
		float* const vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

		populate_voting_info(ws, this_nation, tagged_array_view<float, ideologies::ideology_tag>(vote, ideology_vsize), vtype);

		auto total_pop = ws.w.nation_s.nation_demographics.get(this_nation, population::total_population_tag);
		auto militancy = ws.w.nation_s.nation_demographics.get(this_nation, population::militancy_demo_tag(ws));

		const float n_militancy = total_pop != 0 ?  float(militancy) / float(total_pop) : 0.0f;

		if(n_militancy < 0.5f) {
			float* const group_vote = (float*)_alloca(ws.s.ideologies_m.ideology_groups.size() * sizeof(float));
			std::fill_n(group_vote, ws.s.ideologies_m.ideology_groups.size(), 0.0f);
			for(int32_t i = 0; i < int32_t(ws.s.ideologies_m.ideologies_count); ++i) {
				auto const itag = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i));
				auto const group_tag = ws.s.ideologies_m.ideology_container[itag].group;

				group_vote[to_index(group_tag)] += vote[i];
			}
			ideologies::ideology_group_tag ig(0);
			for(int32_t i = 1; i < int32_t(ws.s.ideologies_m.ideology_groups.size()); ++i) {
				if(group_vote[i] > group_vote[to_index(ig)])
					ig = ideologies::ideology_group_tag(ideologies::ideology_group_tag::value_base_t(i));
			}
			
			float max = 0;
			ideologies::ideology_tag mi;

			for(int32_t i = 0; i < int32_t(ws.s.ideologies_m.ideologies_count); ++i) {
				auto const itag = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i));
				if(ig == ws.s.ideologies_m.ideology_container[itag].group) {
					if(vote[i] >= max) {
						max = vote[i];
						mi = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i));
					}
				}
			}

			return mi;
		} else {
			auto const max_index = maximum_index(vote, ideology_vsize);
			return ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(max_index));
		}
	}


	void unmodified_upper_house_province_vote(world_state const& ws, provinces::province_tag p, tagged_array_view<float, ideologies::ideology_tag> province_voting,
		cultures::culture_tag prime_culture, float national_poor_vote, float national_middle_vote, float national_rich_vote, float accepted_culture_vote, float other_culture_vote) {

		const float province_vote_mod = 1.0f + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::number_of_voters>(p);

		auto const icount = ws.s.ideologies_m.ideologies_count;
		auto const ideology_vsize = ve::to_vector_size(icount);

		ve::set_zero(ideology_vsize, province_voting, ve::serial{});

		provinces::for_each_pop(ws, p, [&ws, province_voting, prime_culture, national_poor_vote, national_middle_vote, national_rich_vote,
			accepted_culture_vote, other_culture_vote, icount](population::pop_tag o) {

			auto const type = ws.w.population_s.pops.get<pop::type>(o);
			if(ws.s.population_m.slave != type) {
				float type_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::is_poor>(o))
					type_mod = national_poor_vote;
				else if(ws.w.population_s.pops.get<pop::is_middle>(o))
					type_mod = national_middle_vote;
				else
					type_mod = national_rich_vote;

				float culture_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::culture>(o) == prime_culture)
					culture_mod = 1.0f;
				else if(ws.w.population_s.pops.get<pop::is_accepted>(o))
					culture_mod = accepted_culture_vote;
				else
					culture_mod = other_culture_vote;

				const float vote_mod = type_mod * culture_mod;
				if(vote_mod > 0.0f) {
					float const* ideology_vals = (ws.w.population_s.pop_demographics.get_row(o) + to_index(population::to_demo_tag(ws, ideologies::ideology_tag(0)))).data();

					ve::accumulate_scaled(icount, province_voting,
						tagged_array_view<float const, ideologies::ideology_tag>(ideology_vals, uint32_t(icount)), vote_mod, ve::serial_unaligned{});
				}
			}
		});

		ve::rescale(ideology_vsize, province_voting, province_vote_mod);
	}

	void rich_only_upper_house_province_vote(world_state const& ws, provinces::province_tag p, tagged_array_view<float, ideologies::ideology_tag> province_voting,
		cultures::culture_tag prime_culture, float accepted_culture_vote, float other_culture_vote) {

		const float province_vote_mod = 1.0f + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::number_of_voters>(p);

		auto const icount = ws.s.ideologies_m.ideologies_count;
		auto const ideology_vsize = ve::to_vector_size(icount);
		ve::set_zero(ideology_vsize, province_voting, ve::serial{});

		provinces::for_each_pop(ws, p, [&ws, province_voting, prime_culture,
			accepted_culture_vote, other_culture_vote, icount](population::pop_tag o) {

			if(!ws.w.population_s.pops.get<pop::is_poor>(o) && !ws.w.population_s.pops.get<pop::is_middle>(o)) {
				float culture_mod = 0.0f;
				if(ws.w.population_s.pops.get<pop::culture>(o) == prime_culture)
					culture_mod = 1.0f;
				else if(ws.w.population_s.pops.get<pop::is_accepted>(o))
					culture_mod = accepted_culture_vote;
				else
					culture_mod = other_culture_vote;

				if(culture_mod > 0.0f) {
					auto const ideology_vals = ws.w.population_s.pop_demographics.get_row(o) + to_index(population::to_demo_tag(ws, ideologies::ideology_tag(0)));

					ve::accumulate_scaled(icount, province_voting,
						tagged_array_view<float const, ideologies::ideology_tag>(ideology_vals.begin(), uint32_t(icount)), culture_mod, ve::serial_unaligned{});
				}
			}
		});

		ve::rescale(ideology_vsize, province_voting, province_vote_mod);
	}

	enum class uh_type {
		normal,
		by_state,
		rich_only,
		fixed
	};

	void update_upper_house(world_state const& ws, nations::country_tag this_nation) {
		int32_t const icount = int32_t(ws.s.ideologies_m.ideologies_count);
		auto const ideology_vsize = ve::to_vector_size(icount);

		float* const vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));
		float* const intermediate_vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

		std::fill_n(vote, ideology_vsize, 0.0f);


		const float national_poor_vote = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_vote>(this_nation);
		const float national_middle_vote = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_vote>(this_nation);
		const float national_rich_vote = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_vote>(this_nation);

		auto const national_rules = ws.w.nation_s.nations.get<nation::current_rules>(this_nation);

		const float accepted_culture_vote =
			(national_rules & issues::rules::citizens_rights_mask) == issues::rules::primary_culture_voting ? 0.5f : 1.0f;
		const float other_culture_vote =
			(national_rules & issues::rules::citizens_rights_mask) == issues::rules::primary_culture_voting ? 1.0f
			: ((national_rules & issues::rules::citizens_rights_mask) == issues::rules::culture_voting ? 0.5f : 0.0f);

		uh_type vtype = uh_type::normal;
		if((national_rules & issues::rules::upper_house_composition_mask) == issues::rules::same_as_ruling_party)
			vtype = uh_type::fixed;
		else if((national_rules & issues::rules::upper_house_composition_mask) == issues::rules::rich_only)
			vtype = uh_type::rich_only;
		else if((national_rules & issues::rules::upper_house_composition_mask) == issues::rules::state_vote)
			vtype = uh_type::by_state;

		const auto pculture = ws.w.nation_s.nations.get<nation::primary_culture>(this_nation);

		for(int32_t i = 0; i < icount; ++i)
			ws.w.nation_s.upper_house.get(this_nation, ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i))) = 0ui8;

		switch(vtype) {
			case uh_type::fixed:
				// do nothing
				break;
			case uh_type::normal:
				nations::for_each_province(ws, this_nation, [accepted_culture_vote, other_culture_vote,
					national_poor_vote, national_middle_vote, national_rich_vote, pculture, vote, ideology_vsize,
					&ws](provinces::province_tag p) {

					if(ws.w.province_s.province_state_container.get<province_state::is_non_state>(p) == false) {
						unmodified_upper_house_province_vote(ws, p, tagged_array_view<float, ideologies::ideology_tag>(vote, ideology_vsize),
							pculture, national_poor_vote, national_middle_vote, national_rich_vote, accepted_culture_vote, other_culture_vote);
					}
				});
				break;
			case uh_type::rich_only:
				nations::for_each_province(ws, this_nation, [national_rules, accepted_culture_vote, other_culture_vote,
					pculture, vote, ideology_vsize,
					&ws](provinces::province_tag p) {

					if(ws.w.province_s.province_state_container.get<province_state::is_non_state>(p) == false) {
						rich_only_upper_house_province_vote(ws, p, tagged_array_view<float, ideologies::ideology_tag>(vote, ideology_vsize),
							pculture, accepted_culture_vote, other_culture_vote);
					}
				});
				break;
			case uh_type::by_state:
				nations::for_each_state(ws, this_nation, [national_rules, accepted_culture_vote, other_culture_vote,
					national_poor_vote, national_middle_vote, national_rich_vote, pculture, vote, intermediate_vote, ideology_vsize,
					&ws](nations::state_tag s) {

					if(ws.w.nation_s.states.get<state::is_colonial>(s) == false && ws.w.nation_s.states.get<state::is_protectorate>(s) == false) {
						std::fill_n(intermediate_vote, ideology_vsize, 0.0f);

						nations::for_each_province(ws, s, [intermediate_vote, ideology_vsize, national_poor_vote, national_middle_vote,
							national_rich_vote, accepted_culture_vote, other_culture_vote, &ws, pculture](provinces::province_tag p) {

							unmodified_upper_house_province_vote(ws, p, tagged_array_view<float, ideologies::ideology_tag>(intermediate_vote, ideology_vsize),
								pculture, national_poor_vote, national_middle_vote, national_rich_vote, accepted_culture_vote, other_culture_vote);
						});

						auto const total = ve::reduce(ws.s.ideologies_m.ideologies_count, tagged_array_view<float, ideologies::ideology_tag>(intermediate_vote, ideology_vsize), ve::serial_exact{});
						auto const inv_total = total > 0.0f ? 1.0f / total : 1.0f;
						ve::accumulate_scaled(ideology_vsize,
							tagged_array_view<float, ideologies::ideology_tag>(vote, ideology_vsize),
							tagged_array_view<float, ideologies::ideology_tag>(intermediate_vote, ideology_vsize),
							inv_total, ve::serial{});
					}
				});
				break;
		}

		if(vtype != uh_type::fixed) {
			auto const total = ve::reduce(icount, tagged_array_view<float, ideologies::ideology_tag>(vote, icount), ve::serial_exact{});
			auto const inv_total = total > 0.0f ? 1.0f / total : 1.0f;
			int32_t uh_added = 0;
			for(int32_t i = 0; i < icount; ++i) {
				int32_t const v = int32_t(vote[i] * inv_total);
				uh_added += v;
				ws.w.nation_s.upper_house.get(this_nation, ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i))) = uint8_t(v);
			}
			if(uh_added < 100) {
				ws.w.nation_s.upper_house.get(this_nation, ws.s.ideologies_m.conservative_ideology) += uint8_t(100 - uh_added);
			}
		} else {
			ws.w.nation_s.upper_house.get(this_nation, ws.w.nation_s.nations.get<nation::ruling_ideology>(this_nation)) = 100ui8;
		}
	}
	
}
