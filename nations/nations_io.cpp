#include "nations_io.h"
#include "world_state\\world_state.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "population\\population_functions.hpp"
#include "nations_functions.h"
#include "governments\\governments_functions.h"
#include "military\\military_io.h"
#include "nations\\nations_functions.h"
#include "technologies\\technologies_functions.h"
#include "military\\military_functions.h"
#include "issues\\issues_functions.h"
#include "economy\\economy_io.h"
#include "nations_io.hpp"

#undef max
#undef min


void serialization::serializer<nations::nations_state>::serialize_object(std::byte *& output, nations::nations_state const & obj, world_state const & ws) {
	serialize(output, obj.nations, ws);
	serialize(output, obj.states, ws);

	obj.states.for_each([&ws, &output](nations::state_tag s) {
		auto prices = ws.w.nation_s.state_prices.get_row(s);
		serialize_array(output, prices.data(), ws.s.economy_m.aligned_32_goods_count);
		auto price_delta = ws.w.nation_s.state_price_delta.get_row(s);
		serialize_array(output, price_delta.data(), ws.s.economy_m.aligned_32_goods_count);
		auto demand = ws.w.nation_s.state_demand.get_row(s);
		serialize_array(output, demand.data(), ws.s.economy_m.aligned_32_goods_count);

	});

	auto s_size = obj.states.size();
	for(int32_t i = 1; i < int32_t(ws.s.economy_m.goods_count); ++i) {
		serialize_array(output, std::begin(ws.w.nation_s.state_production.get_row(economy::goods_tag(economy::goods_tag::value_base_t(i)), s_size)), s_size);
		serialize_array(output, std::begin(ws.w.nation_s.state_global_demand.get_row(economy::goods_tag(economy::goods_tag::value_base_t(i)), s_size)), s_size);
	}

	obj.nations.for_each([&ws, &output](nations::country_tag n) {
		auto active_parties = ws.w.nation_s.active_parties.get_row(n);
		serialize_array(output, active_parties.data(), ws.s.ideologies_m.ideologies_count);

		auto upper_house = ws.w.nation_s.upper_house.get_row(n);
		serialize_array(output, upper_house.data(), ws.s.ideologies_m.ideologies_count);

		//auto active_tech = ws.w.nation_s.active_technologies.get_row(n);
		//serialize_array(output, active_tech.data(), (uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32);

		//auto active_issues = ws.w.nation_s.active_issue_options.get_row(n);
		//serialize_array(output, active_issues.data(), uint32_t(ws.s.issues_m.issues_container.size()));

		auto stockpile = ws.w.nation_s.national_stockpiles.get_row(n);
		serialize_array(output, stockpile.data(), ws.s.economy_m.aligned_32_goods_count);

		auto variables = ws.w.nation_s.national_variables.get_row(n);
		serialize_array(output, variables.data(), ws.s.variables_m.count_national_variables);

		auto local_rebels = ws.w.nation_s.local_rebel_support.get_row(n);
		serialize_array(output, local_rebels.data(), ws.s.population_m.rebel_types.size());

		auto local_movement = ws.w.nation_s.local_movement_support.get_row(n);
		serialize_array(output, local_movement.data(), ws.s.issues_m.tracked_options_count);

		auto local_movement_r = ws.w.nation_s.local_movement_radicalism.get_row(n);
		serialize_array(output, local_movement_r.data(), ws.s.issues_m.tracked_options_count);

		auto local_movement_rc = ws.w.nation_s.local_movement_radicalism_cache.get_row(n);
		serialize_array(output, local_movement_rc.data(), ws.s.issues_m.tracked_options_count);
	});

	auto tech_count = int32_t(ws.s.technology_m.technologies_container.size());
	const auto n_count = obj.nations.size();
	auto byte_count = uint32_t(n_count + 8) / 8ui32;
	for(int32_t i = 0; i < tech_count; ++i) {
		serialize_array(output, obj.active_technologies.get_row(technologies::tech_tag(technologies::tech_tag::value_base_t(i)),0).data(), byte_count);
	}
	auto issues_count = int32_t(ws.s.issues_m.issues_container.size());
	for(int32_t i = 0; i < issues_count; ++i) {
		serialize_array(output, obj.active_issue_options.get_row(issues::issue_tag(issues::issue_tag::value_base_t(i)), 0).data(), n_count + 1);
	}
}

void serialization::serializer<nations::nations_state>::deserialize_object(std::byte const *& input, nations::nations_state & obj, world_state & ws) {
	deserialize(input, obj.nations, ws);
	deserialize(input, obj.states, ws);

	ws.w.nation_s.state_demographics.ensure_capacity(obj.states.size());
	ws.w.nation_s.state_prices.ensure_capacity(obj.states.size());
	ws.w.nation_s.state_price_delta.ensure_capacity(obj.states.size());
	ws.w.nation_s.state_demand.ensure_capacity(obj.states.size());
	ws.w.nation_s.state_purchases.ensure_capacity(obj.states.size());

	ws.w.nation_s.active_parties.ensure_capacity(obj.nations.size());
	ws.w.nation_s.nation_demographics.ensure_capacity(obj.nations.size());
	ws.w.nation_s.nation_colonial_demographics.ensure_capacity(obj.nations.size());
	ws.w.nation_s.upper_house.ensure_capacity(obj.nations.size());
	//ws.w.nation_s.active_technologies.ensure_capacity(obj.nations.size());
	ws.w.nation_s.active_goods.ensure_capacity(obj.nations.size());
	ws.w.nation_s.collected_tariffs.ensure_capacity(obj.nations.size());
	//ws.w.nation_s.active_issue_options.ensure_capacity(obj.nations.size());
	ws.w.nation_s.national_stockpiles.ensure_capacity(obj.nations.size());
	ws.w.nation_s.national_variables.ensure_capacity(obj.nations.size());
	// ws.w.nation_s.unit_stats.ensure_capacity(obj.nations.size());
	ws.w.nation_s.rebel_org_gain.ensure_capacity(obj.nations.size());
	ws.w.nation_s.production_adjustments.ensure_capacity(obj.nations.size());

	ws.w.nation_s.local_rebel_support.ensure_capacity(obj.nations.size());
	ws.w.nation_s.local_movement_support.ensure_capacity(obj.nations.size());
	ws.w.nation_s.local_movement_radicalism.ensure_capacity(obj.nations.size());
	ws.w.nation_s.local_movement_radicalism_cache.ensure_capacity(obj.nations.size());

	obj.states.for_each([&ws, &input](nations::state_tag s) {
		auto prices = ws.w.nation_s.state_prices.get_row(s);
		deserialize_array(input, prices.data(), ws.s.economy_m.aligned_32_goods_count);
		auto prices_delta = ws.w.nation_s.state_price_delta.get_row(s);
		deserialize_array(input, prices_delta.data(), ws.s.economy_m.aligned_32_goods_count);
		auto demand = ws.w.nation_s.state_demand.get_row(s);
		deserialize_array(input, demand.data(), ws.s.economy_m.aligned_32_goods_count);
	});

	auto s_size = obj.states.size();
	for(int32_t i = 1; i < int32_t(ws.s.economy_m.goods_count); ++i) {
		deserialize_array(input, std::begin(ws.w.nation_s.state_production.get_row(economy::goods_tag(economy::goods_tag::value_base_t(i)), s_size)), s_size);
		deserialize_array(input, std::begin(ws.w.nation_s.state_global_demand.get_row(economy::goods_tag(economy::goods_tag::value_base_t(i)), s_size)), s_size);
	}

	obj.nations.for_each([&ws, &input](nations::country_tag n) {
		technologies::reset_technologies(ws, n);
		// military::reset_unit_stats(ws, n);

		auto active_parties = ws.w.nation_s.active_parties.get_row(n);
		deserialize_array(input, active_parties.data(), ws.s.ideologies_m.ideologies_count);

		auto upper_house = ws.w.nation_s.upper_house.get_row(n);
		deserialize_array(input, upper_house.data(), ws.s.ideologies_m.ideologies_count);

		//auto active_tech = ws.w.nation_s.active_technologies.get_row(n);
		//deserialize_array(input, active_tech.data(), (uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32);

		//auto active_issues = ws.w.nation_s.active_issue_options.get_row(n);
		//deserialize_array(input, active_issues.data(), uint32_t(ws.s.issues_m.issues_container.size()));

		auto stockpile = ws.w.nation_s.national_stockpiles.get_row(n);
		deserialize_array(input, stockpile.data(), ws.s.economy_m.aligned_32_goods_count);

		auto variables = ws.w.nation_s.national_variables.get_row(n);
		deserialize_array(input, variables.data(), ws.s.variables_m.count_national_variables);

		auto local_rebels = ws.w.nation_s.local_rebel_support.get_row(n);
		deserialize_array(input, local_rebels.data(), ws.s.population_m.rebel_types.size());

		auto local_movement = ws.w.nation_s.local_movement_support.get_row(n);
		deserialize_array(input, local_movement.data(), ws.s.issues_m.tracked_options_count);

		auto local_movement_r = ws.w.nation_s.local_movement_radicalism.get_row(n);
		deserialize_array(input, local_movement_r.data(), ws.s.issues_m.tracked_options_count);

		auto local_movement_rc = ws.w.nation_s.local_movement_radicalism_cache.get_row(n);
		deserialize_array(input, local_movement_rc.data(), ws.s.issues_m.tracked_options_count);


		if(auto o = ws.w.nation_s.nations.get<nation::overlord>(n); is_valid_index(o)) {
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(o), n);
		}
		if(auto o = ws.w.nation_s.nations.get<nation::sphere_leader>(n); is_valid_index(o)) {
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(o), n);
		}
		auto inf_range = get_range(ws.w.nation_s.influence_arrays, ws.w.nation_s.nations.get<nation::gp_influence>(n));
		for(auto c = inf_range.first; c != inf_range.second; ++c) {
			add_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::influencers>(c->target), n);
			ws.w.nation_s.nations.get<nation::total_foreign_investment>(c->target) += c->investment_amount;
		}
	});

	const auto n_count = obj.nations.size();

	auto tech_count = int32_t(ws.s.technology_m.technologies_container.size());
	obj.active_technologies.resize(tech_count);
	obj.active_technologies.reset();
	
	auto byte_count = uint32_t(n_count + 8) / 8ui32;
	for(int32_t i = 0; i < tech_count; ++i) {
		deserialize_array(input, obj.active_technologies.get_row(technologies::tech_tag(technologies::tech_tag::value_base_t(i)), 0).data(), byte_count);
	}

	auto issues_count = int32_t(ws.s.issues_m.issues_container.size());
	obj.active_issue_options.resize(issues_count);
	obj.active_issue_options.reset();

	for(int32_t i = 0; i < issues_count; ++i) {
		deserialize_array(input, obj.active_issue_options.get_row(issues::issue_tag(issues::issue_tag::value_base_t(i)), 0).data(), n_count + 1);
	}
}

size_t serialization::serializer<nations::nations_state>::size(nations::nations_state const & obj, world_state const & ws) {
	size_t state_data_size = 0;
	auto fixed_sz_increment =
		sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count + // state prices
		sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count + // state price delta
		sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count; // state demand

	obj.states.for_each([fixed_sz_increment, &state_data_size](nations::state_tag s) {
		state_data_size += fixed_sz_increment; 
	});

	auto s_size = obj.states.size();
	auto state_other_arrays_size = 2 * (sizeof(float) * s_size * (int32_t(ws.s.economy_m.goods_count) - 1)); // production & global demand

	size_t nation_data_size = 0;
	auto nation_fixed_sz_increment = sizeof(governments::party_tag) * ws.s.ideologies_m.ideologies_count + // active parties
		sizeof(uint8_t) * ws.s.ideologies_m.ideologies_count + // upper house
		//sizeof(uint64_t) * ((uint32_t(ws.s.technology_m.technologies_container.size()) + 63ui32) / 64ui32) + // active technologies
		//sizeof(issues::option_tag) * ws.s.issues_m.issues_container.size() + // active issue options
		sizeof(economy::goods_qnty_type) * ws.s.economy_m.aligned_32_goods_count + // national stockpiles
		sizeof(float) * ws.s.variables_m.count_national_variables + // national variables
		sizeof(float) * ws.s.population_m.rebel_types.size() + //local rebels
		sizeof(float) * ws.s.issues_m.tracked_options_count * 3 //local movement support, radicalism, radicalism cache
		;

	obj.nations.for_each([nation_fixed_sz_increment, &nation_data_size](nations::country_tag n) {
		nation_data_size += nation_fixed_sz_increment;
	});

	const auto n_count = obj.nations.size();
	auto tech_count = int32_t(ws.s.technology_m.technologies_container.size());
	auto byte_count = uint32_t(n_count + 8) / 8ui32;
	auto issues_count = int32_t(ws.s.issues_m.issues_container.size());

	return serialize_size(obj.nations, ws) +
		serialize_size(obj.states, ws) +
		state_data_size +
		state_other_arrays_size +
		nation_data_size +
		tech_count * byte_count +
		issues_count * sizeof(issues::option_tag) * (n_count + 1);
}

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
				simple_make_vassal(ws, make_nation_for_tag(ws, r.first), make_nation_for_tag(ws, r.second));
		}
		void set_substate(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				simple_make_substate(ws, make_nation_for_tag(ws, r.first), make_nation_for_tag(ws, r.second));
		}
		void set_alliance(relation_parse_obj const& r) {
			if(r.start_date <= filter_date && filter_date < r.end_date && is_valid_index(r.first) && is_valid_index(r.second))
				silent_make_alliance(ws, make_nation_for_tag(ws, r.first), make_nation_for_tag(ws, r.second));
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
		std::optional<bool> is_releasable_vassal;
		modifiers::national_modifier_tag nationalvalue;
		modifiers::national_modifier_tag techschool;
		std::vector<std::pair<cultures::national_tag, uint32_t>> investment;
		std::vector<std::pair<governments::government_tag, governments::government_tag>> govt_flags; //first = to be replaced, second = with flag of this type
		std::vector<bitfield_type> tech_bit_vector;
		std::vector<issues::option_tag> set_options;

		date_tag target_date;

		nation_parse_object(world_state& w, date_tag t) : ws(w), target_date(t) {
			tech_bit_vector.resize(ws.s.technology_m.technologies_container.size() >> 3ui64);
			set_options.resize(ws.s.issues_m.issues_container.size());
		}

		void set_capital(uint16_t v) {
			capital = ws.s.province_m.integer_to_province[v];
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
			decisions.push_back(tag_from_text(ws.s.event_m.decisions_by_title_index, tt_title));
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

	inline void add_data_to_nation(world_state& ws, country_tag target_nation, cultures::national_tag_state& nat_tag, cultures::national_tag nat_tag_id, directory const& units_dir, nation_parse_object& npo) {
		nat_tag.capital = npo.capital;
		ws.w.nation_s.nations.set<nation::current_government>(target_nation, npo.gov);
		
		ws.w.nation_s.nations.set<nation::primary_culture>(target_nation, npo.primary_culture);

		for(auto c : npo.accepted_cultures)
			add_item(ws.w.culture_s.culture_arrays, ws.w.nation_s.nations.get<nation::accepted_cultures>(target_nation), c);
		ws.w.nation_s.nations.set<nation::national_religion>(target_nation, npo.religion);
		for(auto f : npo.set_flags)
			add_item(ws.w.variable_s.national_flags_arrays, ws.w.nation_s.nations.get<nation::national_flags>(target_nation), f);
		for(auto f : npo.global_flags)
			ws.w.variable_s.global_variables[f] = 1.0f;
		if(npo.upper_house.size() != 0) {
			auto uh_row = ws.w.nation_s.upper_house.get_row(target_nation);
			for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
				auto itag = ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i));
				uh_row[itag] = uint8_t(npo.upper_house[i]);
			}
		}
		ws.w.nation_s.nations.set<nation::last_election>(target_nation, npo.last_election);

		auto owned_provs_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(target_nation));
		for(auto i = owned_provs_range.first; i != owned_provs_range.second; ++i) {
			auto pops_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container.get<province_state::pops>(*i));
			for(auto j : pops_range) {
				auto this_pop = j;
				if(population::is_pop_accepted(ws, this_pop, target_nation)) {
					if(npo.literacy)
						population::set_literacy_direct(ws, this_pop, *npo.literacy);
					if(npo.consciousness)
						population::set_consciousness_direct(ws, this_pop, *npo.consciousness);
				} else {
					if(npo.non_state_culture_literacy)
						population::set_literacy_direct(ws, this_pop, *npo.non_state_culture_literacy);
					if(npo.nonstate_consciousness)
						population::set_consciousness_direct(ws, this_pop, *npo.nonstate_consciousness);
				}
			}
		}
		if(npo.plurality)
			ws.w.nation_s.nations.set<nation::plurality>(target_nation, *npo.plurality / 100.0f);
		if(npo.prestige)
			ws.w.nation_s.nations.set<nation::base_prestige>(target_nation, *npo.prestige);
		if(npo.civilized && *npo.civilized)
			ws.w.nation_s.nations.set<nation::is_civilized>(target_nation, true);
		if(npo.is_releasable_vassal)
			nat_tag.is_not_releasable = !(*npo.is_releasable_vassal);
		ws.w.nation_s.nations.set<nation::national_value>(target_nation, npo.nationalvalue);
		ws.w.nation_s.nations.set<nation::tech_school>(target_nation, npo.techschool);
		ws.w.nation_s.nations.set<nation::current_government>(target_nation, npo.gov);

		ws.w.nation_s.nations.set<nation::current_capital>(target_nation, npo.capital);
		for(auto& ip : npo.investment) {
			auto holding_nation = make_nation_for_tag(ws, ip.first);
			set_foreign_investment(ws, target_nation, holding_nation, static_cast<float>(ip.second));
		}

		for(auto& ip : npo.govt_flags)
			ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.first) = ws.w.culture_s.country_flags_by_government.get(nat_tag_id, ip.second);

		//auto tech_row = ws.w.nation_s.active_technologies.get_row(target_nation);
		for(int32_t i = 0; i < ws.s.technology_m.technologies_container.size(); ++i) {
			ws.w.nation_s.active_technologies.set(target_nation, technologies::tech_tag(technologies::tech_tag::value_base_t(i)), bit_vector_test(npo.tech_bit_vector.data(), i));
			//tech_row.data()[i] = npo.tech_bit_vector[i];
		}

		for(int32_t i = 0; i < ws.s.issues_m.issues_container.size(); ++i) {
			auto itag = issues::issue_tag(issues::issue_tag::value_base_t(i));
			ws.w.nation_s.active_issue_options.get(target_nation, itag) = npo.set_options[i];
		}

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
		if(other.is_releasable_vassal)
			base.is_releasable_vassal = other.is_releasable_vassal;
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
			base.tech_bit_vector[i].v = base.tech_bit_vector[i].v | other.tech_bit_vector[i].v;
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
MEMBER_DEF(nations::nation_parse_object, is_releasable_vassal, "is_releasable_vassal");

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
		MEMBER_ASSOCIATION("is_releasable_vassal", "is_releasable_vassal", value_from_rh<bool>)
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
			nations::country_tag this_nation = make_nation_for_tag(ws, nat_tag_id);
			auto npo = parse_object<nation_parse_object, nation_file>(parse_results.data(), parse_results.data() + parse_results.size(), ws, target_date);
			add_data_to_nation(ws, this_nation, ws.w.culture_s.national_tags_state[nat_tag_id], nat_tag_id, units_dir, npo);
			for(auto d : npo.decisions)
				decisions.emplace_back(this_nation, d);
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
		set_default_tech_school(ws);
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
	void set_default_tech_school(world_state & ws) {
		auto mod_thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "traditional_academic");
		auto nmod = ws.s.modifiers_m.named_national_modifiers_index[mod_thandle];
		ws.w.nation_s.nations.for_each([&ws, nmod](nations::country_tag n) {
			if(!is_valid_index(ws.w.nation_s.nations.get<nation::tech_school>(n)))
				ws.w.nation_s.nations.set<nation::tech_school>(n, nmod);
		});
	}
}
