#include "common\\common.h"
#include "population_io.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "scenario\\scenario.h"
#include "modifiers\\modifiers_io.h"
#include "triggers\\trigger_reading.h"
#include "triggers\\effect_reading.h"
#include "world_state\\world_state.h"
#include "population_functions.h"
#include "issues\\issues_functions.h"


/*
void serialization::serializer<population::pop_movement>::serialize_object(std::byte *& output, population::pop_movement const & obj) {
	serialize(output, obj.radicalism);
	serialize(output, obj.radicalism_cache);
	serialize(output, obj.liberation_country);
	serialize(output, obj.associated_issue);

	uint8_t type = uint8_t(obj.type);
	serialize(output, type);
}

void serialization::serializer<population::pop_movement>::deserialize_object(std::byte const *& input, population::pop_movement & obj) {
	deserialize(input, obj.radicalism);
	deserialize(input, obj.radicalism_cache);
	deserialize(input, obj.liberation_country);
	deserialize(input, obj.associated_issue);

	uint8_t type = 0ui8;
	deserialize(input, type);
	obj.type = population::movement_type(type);

	obj.total_population_support = 0;
}

size_t serialization::serializer<population::pop_movement>::size(population::pop_movement const & obj) {
	return serialize_size(obj.radicalism) +
		serialize_size(obj.radicalism_cache) +
		serialize_size(obj.liberation_country) +
		serialize_size(obj.associated_issue) +
		sizeof(uint8_t);
}

size_t serialization::serializer<population::pop_movement>::size() {
	return sizeof(float) +
		sizeof(float) +
		sizeof(nations::country_tag) +
		sizeof(issues::option_tag) +
		sizeof(uint8_t);
}*/

void serialization::serializer<population::population_manager>::rebuild_indexes(population::population_manager & obj) {
	obj.factory_workers.clear();

	for(auto const& i_pop : obj.pop_types) {
		obj.named_pop_type_index.emplace(i_pop.name, i_pop.id);
		if((i_pop.flags & population::pop_type::factory_worker) != 0)
			obj.factory_workers.push_back(i_pop.id);
	}
	obj.nationalist_rebels = population::rebel_type_tag();
	for(auto const& i_rebel : obj.rebel_types) {
		obj.named_rebel_type_index.emplace(i_rebel.name, i_rebel.id);
		if(!is_valid_index(obj.nationalist_rebels) && (i_rebel.flags & population::rebel_type::defection_culture) != 0)
			obj.nationalist_rebels = i_rebel.id;
	}
	obj.count_poptypes = static_cast<uint32_t>(obj.pop_types.size());
}

void serialization::serializer<population::population_manager>::serialize_object(std::byte *& output, population::population_manager const & obj) {
	serialize(output, obj.pop_types);
	serialize(output, obj.rebel_types);
	serialize(output, obj.life_needs);
	serialize(output, obj.everyday_needs);
	serialize(output, obj.luxury_needs);
	// serialize(output, obj.rebel_units);
	serialize(output, obj.issue_inclination);
	serialize(output, obj.ideological_inclination);
	serialize(output, obj.promote_to);
	serialize(output, obj.demote_to);
	serialize(output, obj.rebel_change_government_to);
	serialize(output, obj.promotion_chance);
	serialize(output, obj.demotion_chance);
	serialize(output, obj.migration_chance);
	serialize(output, obj.colonial_migration_chance);
	serialize(output, obj.emigration_chance);
	serialize(output, obj.assimilation_chance);
	serialize(output, obj.conversion_chance);
	serialize(output, obj.clergy_pay);
	serialize(output, obj.bureaucrat_pay);
	serialize(output, obj.soldier_pay);
	serialize(output, obj.officer_pay);
	serialize(output, obj.officer_leadership);
	serialize(output, obj.artisan);
	serialize(output, obj.capitalist);
	serialize(output, obj.clergy);
	serialize(output, obj.bureaucrat);
	serialize(output, obj.slave);
	serialize(output, obj.soldier);
	serialize(output, obj.officer);
	serialize(output, obj.farmer);
	serialize(output, obj.laborer);
}

void serialization::serializer<population::population_manager>::deserialize_object(std::byte const *& input, population::population_manager & obj) {
	deserialize(input, obj.pop_types);
	deserialize(input, obj.rebel_types);
	deserialize(input, obj.life_needs);
	deserialize(input, obj.everyday_needs);
	deserialize(input, obj.luxury_needs);
	// deserialize(input, obj.rebel_units);
	deserialize(input, obj.issue_inclination);
	deserialize(input, obj.ideological_inclination);
	deserialize(input, obj.promote_to);
	deserialize(input, obj.demote_to);
	deserialize(input, obj.rebel_change_government_to);
	deserialize(input, obj.promotion_chance);
	deserialize(input, obj.demotion_chance);
	deserialize(input, obj.migration_chance);
	deserialize(input, obj.colonial_migration_chance);
	deserialize(input, obj.emigration_chance);
	deserialize(input, obj.assimilation_chance);
	deserialize(input, obj.conversion_chance);
	deserialize(input, obj.clergy_pay);
	deserialize(input, obj.bureaucrat_pay);
	deserialize(input, obj.soldier_pay);
	deserialize(input, obj.officer_pay);
	deserialize(input, obj.officer_leadership);
	deserialize(input, obj.artisan);
	deserialize(input, obj.capitalist);
	deserialize(input, obj.clergy);
	deserialize(input, obj.bureaucrat);
	deserialize(input, obj.slave);
	deserialize(input, obj.soldier);
	deserialize(input, obj.officer);
	deserialize(input, obj.farmer);
	deserialize(input, obj.laborer);

	rebuild_indexes(obj);
}

void serialization::serializer<population::population_manager>::deserialize_object(std::byte const *& input, population::population_manager & obj, concurrency::task_group & tg) {
	deserialize(input, obj.pop_types);
	deserialize(input, obj.rebel_types);
	deserialize(input, obj.life_needs);
	deserialize(input, obj.everyday_needs);
	deserialize(input, obj.luxury_needs);
	// deserialize(input, obj.rebel_units);
	deserialize(input, obj.issue_inclination);
	deserialize(input, obj.ideological_inclination);
	deserialize(input, obj.promote_to);
	deserialize(input, obj.demote_to);
	deserialize(input, obj.rebel_change_government_to);
	deserialize(input, obj.promotion_chance);
	deserialize(input, obj.demotion_chance);
	deserialize(input, obj.migration_chance);
	deserialize(input, obj.colonial_migration_chance);
	deserialize(input, obj.emigration_chance);
	deserialize(input, obj.assimilation_chance);
	deserialize(input, obj.conversion_chance);
	deserialize(input, obj.clergy_pay);
	deserialize(input, obj.bureaucrat_pay);
	deserialize(input, obj.soldier_pay);
	deserialize(input, obj.officer_pay);
	deserialize(input, obj.officer_leadership);
	deserialize(input, obj.artisan);
	deserialize(input, obj.capitalist);
	deserialize(input, obj.clergy);
	deserialize(input, obj.bureaucrat);
	deserialize(input, obj.slave);
	deserialize(input, obj.soldier);
	deserialize(input, obj.officer);
	deserialize(input, obj.farmer);
	deserialize(input, obj.laborer);

	tg.run([&obj]() { rebuild_indexes(obj); });
}

size_t serialization::serializer<population::population_manager>::size(population::population_manager const & obj) {
	return
		serialize_size(obj.pop_types) +
		serialize_size(obj.rebel_types) +
		serialize_size(obj.life_needs) +
		serialize_size(obj.everyday_needs) +
		serialize_size(obj.luxury_needs) +
		// serialize_size(obj.rebel_units) +
		serialize_size(obj.issue_inclination) +
		serialize_size(obj.ideological_inclination) +
		serialize_size(obj.promote_to) +
		serialize_size(obj.demote_to) +
		serialize_size(obj.rebel_change_government_to) +
		serialize_size(obj.promotion_chance) +
		serialize_size(obj.demotion_chance) +
		serialize_size(obj.migration_chance) +
		serialize_size(obj.colonial_migration_chance) +
		serialize_size(obj.emigration_chance) +
		serialize_size(obj.assimilation_chance) +
		serialize_size(obj.conversion_chance) +
		serialize_size(obj.clergy_pay) +
		serialize_size(obj.bureaucrat_pay) +
		serialize_size(obj.soldier_pay) +
		serialize_size(obj.officer_pay) +
		serialize_size(obj.officer_leadership) +
		serialize_size(obj.artisan) +
		serialize_size(obj.capitalist) +
		serialize_size(obj.clergy) +
		serialize_size(obj.bureaucrat) +
		serialize_size(obj.slave) +
		serialize_size(obj.soldier) +
		serialize_size(obj.officer) +
		serialize_size(obj.farmer) +
		serialize_size(obj.laborer);
}

void serialization::serializer<population::population_state>::serialize_object(std::byte *& output, population::population_state const & obj, world_state const & ws) {
	serialize(output, obj.pops);

	obj.pops.for_each([sz = population::aligned_32_issues_ideology_demo_size(ws), &obj, &output](population::pop_tag p) {
		auto demographics = obj.pop_demographics.get_row(p);
		serialize_array(output, demographics.data(), sz);
	});

	serialize_array(output, obj.independence_rebel_support.data(), ws.s.culture_m.national_tags.size());
	serialize_array(output, obj.independence_movement_support.data(), ws.s.culture_m.national_tags.size());
}

void serialization::serializer<population::population_state>::deserialize_object(std::byte const *& input, population::population_state & obj, world_state & ws) {
	deserialize(input, obj.pops);

	obj.pop_demographics.ensure_capacity(obj.pops.size());

	obj.pops.for_each([sz = population::aligned_32_issues_ideology_demo_size(ws), &obj, &input, &ws](population::pop_tag p) {
		auto demographics = obj.pop_demographics.get_row(p);
		deserialize_array(input, demographics.data(), sz);

		obj.pops.set<pop::social_interest>(p, issues::calculate_social_interest(ws, demographics));
		obj.pops.set<pop::political_interest>(p, issues::calculate_political_interest(ws, demographics));
		
		auto type = obj.pops.get<pop::type>(p);
		auto strata = ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask;
		if(strata == population::pop_type::strata_poor) {
			obj.pops.set<pop::is_poor>(p, true);
			obj.pops.set<pop::is_middle>(p, false);
		} else if(strata == population::pop_type::strata_middle) {
			obj.pops.set<pop::is_poor>(p, false);
			obj.pops.set<pop::is_middle>(p, true);
		} else {
			obj.pops.set<pop::is_poor>(p, false);
			obj.pops.set<pop::is_middle>(p, false);
		}

		obj.pops.set<pop::size>(p, ws.w.population_s.pop_demographics.get(p, population::total_population_tag));
	});

	deserialize_array(input, obj.independence_rebel_support.data(), ws.s.culture_m.national_tags.size());
	deserialize_array(input, obj.independence_movement_support.data(), ws.s.culture_m.national_tags.size());
}

size_t serialization::serializer<population::population_state>::size(population::population_state const & obj, world_state const & ws) {
	size_t pop_demo_size = 0;
	obj.pops.for_each([sz = population::aligned_32_issues_ideology_demo_size(ws), &pop_demo_size](population::pop_tag p) {
		pop_demo_size += sz * sizeof(float);
	});

	return 
		serialize_size(obj.pops) +
		pop_demo_size + 
		sizeof(float) * ws.s.culture_m.national_tags.size() * 2; // independance rebels and movements

}

/*
void serialization::serializer<population::rebel_faction>::serialize_object(std::byte *& output, population::rebel_faction const & obj) {
	serialize(output, obj.independence_tag);
	serialize(output, obj.culture);
	serialize(output, obj.religion);
	serialize(output, obj.type);
}

void serialization::serializer<population::rebel_faction>::deserialize_object(std::byte const *& input, population::rebel_faction & obj, world_state & ws) {
	deserialize(input, obj.independence_tag);
	deserialize(input, obj.culture);
	deserialize(input, obj.religion);
	deserialize(input, obj.type);

	obj.flags = ws.s.population_m.rebel_types[obj.type].flags;
	obj.icon = ws.s.population_m.rebel_types[obj.type].icon;
	obj.ideology = ws.s.population_m.rebel_types[obj.type].ideology;
}

size_t serialization::serializer<population::rebel_faction>::size(population::rebel_faction const & obj) {
	return serialize_size(obj.independence_tag) +
		serialize_size(obj.culture) +
		serialize_size(obj.religion) +
		serialize_size(obj.type);
}

size_t serialization::serializer<population::rebel_faction>::size() {
	return sizeof(nations::country_tag) +
		sizeof(cultures::culture_tag) +
		sizeof(cultures::religion_tag) +
		sizeof(population::rebel_type_tag);
}*/

namespace population {
	struct pops_in_province_environment {
		world_state& ws;
		provinces::province_tag prov;

		pops_in_province_environment(world_state& s, provinces::province_tag p) : ws(s), prov(p) {}
	};

	struct pop_reader {
		pops_in_province_environment& env;
		cultures::culture_tag culture;
		cultures::religion_tag religion;
		uint32_t size = 0ui32;
		float militancy = 0.0f;

		pop_reader(pops_in_province_environment& e) : env(e) {}

		void set_culture(token_and_type const& t) {
			culture = tag_from_text(
				env.ws.s.culture_m.named_culture_index,
				text_data::get_thread_safe_existing_text_handle(env.ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_religion(token_and_type const& t) {
			religion = tag_from_text(
				env.ws.s.culture_m.named_religion_index,
				text_data::get_thread_safe_existing_text_handle(env.ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void discard(int) {}
	};

	inline std::pair<token_and_type, pop_reader> read_pop(token_and_type const& l, association_type, pop_reader const& r) {
		return std::pair<token_and_type, pop_reader>(l, r);
	}

	struct pops_in_province_reader {
		pops_in_province_environment& env;
		pops_in_province_reader(pops_in_province_environment& e) : env(e) {}

		void add_pop(std::pair<token_and_type, pop_reader> const& p) {
			if(p.second.size != 0) {
				const auto pop_type = tag_from_text(
					env.ws.s.population_m.named_pop_type_index,
					text_data::get_thread_safe_existing_text_handle(env.ws.s.gui_m.text_data_sequences, p.first.start, p.first.end));
				auto new_pop = allocate_new_pop(env.ws);
				
				env.ws.w.population_s.pops.set<pop::culture>(new_pop, p.second.culture);
				env.ws.w.population_s.pops.set<pop::religion>(new_pop, p.second.religion);
				env.ws.w.population_s.pops.set<pop::type>(new_pop, pop_type);
				set_militancy_direct(env.ws, new_pop, p.second.militancy);
				env.ws.w.population_s.pops.set<pop::location>(new_pop, env.prov);
				env.ws.w.population_s.pops.set<pop::needs_satisfaction>(new_pop, 2.0f);

				init_pop_demographics(env.ws, new_pop, float(p.second.size));

				add_item(env.ws.w.population_s.pop_arrays, env.ws.w.province_s.province_state_container.get<province_state::pops>(env.prov), new_pop);
			}
		}
	};

	int read_pops_in_province(token_group const* s, token_group const* e, token_and_type const& pid, world_state& ws);

	struct pop_file_reader {
		world_state& env;
		pop_file_reader(world_state& w) : env(w) {}

		void discard(int) {}
	};

	int read_pops_in_province(token_group const* s, token_group const* e, token_and_type const& pid, world_state& ws);

	struct parsing_environment {
		text_data::text_sequences& text_lookup;
		population_manager& manager;

		parsed_data rebel_types_file;
		std::vector<std::tuple<rebel_type_tag, const token_group*, const token_group*>> pending_rebels;

		parsing_environment(text_data::text_sequences& tl, population_manager& m) :
			text_lookup(tl), manager(m) {}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, population_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	inline int inner_pre_parse_rebel(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env) {
		std::string reb_label = std::string(t.start, t.end) + "_name";
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto rtag = env.manager.rebel_types.emplace_back();
		auto& reb = env.manager.rebel_types[rtag];
		reb.id = rtag;
		reb.name = name;
		reb.label = text_data::get_thread_safe_text_handle(env.text_lookup, reb_label.c_str(), reb_label.c_str() + reb_label.size());
		env.manager.named_rebel_type_index.emplace(name, rtag);
		env.pending_rebels.emplace_back(rtag, s, e);
		return 0;
	}

	struct rebel_types_pre_parse_file {
		parsing_environment& env;
		rebel_types_pre_parse_file(parsing_environment& e) : env(e) {}

		void add_rebel_type(int) {}
	};

	struct rebel_reading_env {
		scenario::scenario_manager& s;
		events::event_creation_manager& ecm;
		rebel_type& under_construction;

		rebel_reading_env(scenario::scenario_manager& sm, events::event_creation_manager& e, rebel_type& uc) : s(sm), ecm(e), under_construction(uc) {}
	};

	inline std::pair<token_and_type, token_and_type> get_tpair(token_and_type const& l, association_type, token_and_type const& r) {
		return std::pair<token_and_type, token_and_type>(l, r);
	}
	inline modifiers::factor_tag read_rebel_spawn_factor(const token_group* s, const token_group* e, rebel_reading_env& env) {
		return modifiers::parse_modifier_factors(
			env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::pop,
				},
				1.0f, s, e);
	}
	inline modifiers::factor_tag read_rebel_movement_evaluation_factor(const token_group* s, const token_group* e, rebel_reading_env& env) {
		return modifiers::parse_modifier_factors(
			env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::rebel,
				},
				1.0f, s, e);
	}
	inline modifiers::factor_tag read_rebel_will_rise_factor(const token_group* s, const token_group* e, rebel_reading_env& env) {
		return modifiers::parse_modifier_factors(
			env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::rebel,
				},
				1.0f, s, e);
	}
	inline int discard_rebel_section(const token_group*, const token_group*, rebel_reading_env&) { return 0; }
	inline triggers::trigger_tag read_rebel_siege_won_trigger(const token_group* s, const token_group* e, rebel_reading_env& env) {
		const auto td = triggers::parse_trigger(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::rebel,
				}, s, e);
		return triggers::commit_trigger(env.s.trigger_m, td);
	}
	inline triggers::effect_tag read_rebel_siege_won_effect(const token_group* s, const token_group* e, rebel_reading_env& env) {
		const auto td = triggers::parse_effect(env.s, env.ecm,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::rebel,
				}, s, e);
		return triggers::commit_effect(env.s.trigger_m, td);
	}
	inline triggers::trigger_tag read_rebel_demands_enforced_trigger(const token_group* s, const token_group* e, rebel_reading_env& env) {
		const auto td = triggers::parse_trigger(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::rebel,
				}, s, e);
		return triggers::commit_trigger(env.s.trigger_m, td);
	}
	inline triggers::effect_tag read_rebel_demands_enforced_effect(const token_group* s, const token_group* e, rebel_reading_env& env) {
		const auto td = triggers::parse_effect(env.s, env.ecm,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::rebel,
				}, s, e);
		return triggers::commit_effect(env.s.trigger_m, td);
	}
#ifdef _DEBUG
	struct bad_government {};
	struct bad_area {};
	struct bad_defection {};
	struct bad_independence {};
#endif
	struct rebel_gov_reader {
		rebel_reading_env& env;

		rebel_gov_reader(rebel_reading_env& e) : env(e) {}
		void add_gov(std::pair<token_and_type, token_and_type> const& p) {
			const auto l_name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end);
			const auto l_gtag = tag_from_text(env.s.governments_m.named_government_index, l_name);

			const auto r_name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.second.start, p.second.end);
			const auto r_gtag = tag_from_text(env.s.governments_m.named_government_index, r_name);
#ifdef _DEBUG
			if(!is_valid_index(l_gtag) | !is_valid_index(r_gtag))
				throw bad_government();
#endif
			env.s.population_m.rebel_change_government_to.get(env.under_construction.id, l_gtag) = r_gtag;
		}
	};

	struct rebel_reader {
		rebel_reading_env& env;

		rebel_reader(rebel_reading_env& e) : env(e) {}

		void set_icon(uint8_t v) {
			env.under_construction.icon = v;
		}
		void discard(int) {}
		void set_will_rise(modifiers::factor_tag t) {
			env.under_construction.will_rise = t;
		}
		void set_allow_all_cultures(bool v) {
			if(!v)
				env.under_construction.flags |= rebel_type::restrict_by_culture;
		}
		void set_allow_all_religions(bool v) {
			if(!v)
				env.under_construction.flags |= rebel_type::restrict_by_religion;
		}
		void set_allow_all_culture_groups(bool v) {
			if(!v)
				env.under_construction.flags |= rebel_type::restrict_by_culture_group;
		}
		void set_allow_all_ideologies(bool v) {
			if(!v)
				env.under_construction.flags |= rebel_type::restrict_by_ideology;
		}
		void set_break_alliance_on_win(bool v) {
			if(v)
				env.under_construction.flags |= rebel_type::break_alliance_on_win;
		}
		void set_ideology(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, t.start, t.end);
			const auto itag = tag_from_text(env.s.ideologies_m.named_ideology_index, name);
			env.under_construction.ideology = itag;
		}
		void set_spawn_chance(modifiers::factor_tag t) {
			env.under_construction.spawn_chance = t;
		}
		void set_movement_evaluation(modifiers::factor_tag t) {
			env.under_construction.movement_evaluation = t;
		}
		void read_government(rebel_gov_reader const&) {}
		void set_area(const token_and_type& t) {
			if(is_fixed_token_ci(t, "nation"))
				env.under_construction.flags |= rebel_type::area_nation;
			else if(is_fixed_token_ci(t, "culture"))
				env.under_construction.flags |= rebel_type::area_culture;
			else if(is_fixed_token_ci(t, "nation_culture"))
				env.under_construction.flags |= rebel_type::area_nation_culture;
			else if(is_fixed_token_ci(t, "nation_religion"))
				env.under_construction.flags |= rebel_type::area_nation_religion;
			else if(is_fixed_token_ci(t, "religion"))
				env.under_construction.flags |= rebel_type::area_religion;
			else if(is_fixed_token_ci(t, "culture_group"))
				env.under_construction.flags |= rebel_type::area_culture_group;
			else if(is_fixed_token_ci(t, "all"))
				env.under_construction.flags |= rebel_type::area_all;
#ifdef _DEBUG
			else
				throw bad_area();
#endif
		}
		void set_defection(const token_and_type& t) {
			if(is_fixed_token_ci(t, "ideology"))
				env.under_construction.flags |= rebel_type::defection_ideology;
			else if(is_fixed_token_ci(t, "culture"))
				env.under_construction.flags |= rebel_type::defection_culture;
			else if(is_fixed_token_ci(t, "any"))
				env.under_construction.flags |= rebel_type::defection_any;
			else if(is_fixed_token_ci(t, "none"))
				env.under_construction.flags |= rebel_type::defection_none;
			else if(is_fixed_token_ci(t, "religion"))
				env.under_construction.flags |= rebel_type::defection_religion;
			else if(is_fixed_token_ci(t, "culture_group"))
				env.under_construction.flags |= rebel_type::defection_culture_group;
			else if(is_fixed_token_ci(t, "pan_nationalist"))
				env.under_construction.flags |= rebel_type::defection_pan_nationalist;
#ifdef _DEBUG
			else
				throw bad_defection();
#endif
		}
		void set_independence(const token_and_type& t) {
			if(is_fixed_token_ci(t, "colonial"))
				env.under_construction.flags |= rebel_type::independence_colonial;
			else if(is_fixed_token_ci(t, "culture"))
				env.under_construction.flags |= rebel_type::independence_culture;
			else if(is_fixed_token_ci(t, "any"))
				env.under_construction.flags |= rebel_type::independence_any;
			else if(is_fixed_token_ci(t, "none"))
				env.under_construction.flags |= rebel_type::independence_none;
			else if(is_fixed_token_ci(t, "religion"))
				env.under_construction.flags |= rebel_type::independence_religion;
			else if(is_fixed_token_ci(t, "culture_group"))
				env.under_construction.flags |= rebel_type::independence_culture_group;
			else if(is_fixed_token_ci(t, "pan_nationalist"))
				env.under_construction.flags |= rebel_type::independence_pan_nationalist;
#ifdef _DEBUG
			else
				throw bad_independence();
#endif
		}
		void set_defect_delay(uint8_t v) {
			env.under_construction.defect_delay = v;
		}
		void set_siege_won_effect(triggers::effect_tag t) {
			env.under_construction.siege_won_effect = t;
		}
		void set_siege_won_trigger(triggers::trigger_tag t) {
			env.under_construction.siege_won_trigger = t;
		}
		void set_demands_enforced_effect(triggers::effect_tag t) {
			env.under_construction.demands_enforced_effect = t;
		}
		void set_demands_enforced_trigger(triggers::trigger_tag t) {
			env.under_construction.demands_enforced_trigger = t;
		}
		void set_occupation_mult(float v) {
			env.under_construction.occupation_multiplier = v;
		}
	};

	struct single_poptype_environment {
		scenario::scenario_manager& s;
		pop_type& pt;

		single_poptype_environment(scenario::scenario_manager& sm, pop_type& p) : s(sm), pt(p) {}
	};

	struct poptypes_file {
		scenario::scenario_manager& env;
		poptypes_file(scenario::scenario_manager& e) : env(e) {}

		void set_promotion_chance(modifiers::factor_tag t) {
			env.population_m.promotion_chance = t;
		}
		void set_demotion_chance(modifiers::factor_tag t) {
			env.population_m.demotion_chance = t;
		}
		void set_migration_chance(modifiers::factor_tag t) {
			env.population_m.migration_chance = t;
		}
		void set_colonialmigration_chance(modifiers::factor_tag t) {
			env.population_m.colonial_migration_chance = t;
		}
		void set_emigration_chance(modifiers::factor_tag t) {
			env.population_m.emigration_chance = t;
		}
		void set_assimilation_chance(modifiers::factor_tag t) {
			env.population_m.assimilation_chance = t;
		}
		void set_conversion_chance(modifiers::factor_tag t) {
			env.population_m.conversion_chance = t;
		}
	};

	inline modifiers::factor_tag read_poptype_file_modifier(const token_group* s, const token_group* e, scenario::scenario_manager& env) {
		return modifiers::parse_modifier_factors(
			env,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::empty,
				},
				1.0f, s, e);
	}

	struct income {
		float weight = 0.0f;
		income_type type = income_type::none;

		void set_type(const token_and_type& t) {
			if(is_fixed_token_ci(t, "administration")) {
				type = income_type::administration;
			} else if(is_fixed_token_ci(t, "military")) {
				type = income_type::military;
			} else if(is_fixed_token_ci(t, "education")) {
				type = income_type::education;
			} else if(is_fixed_token_ci(t, "reforms")) {
				type = income_type::reforms;
			}
		}
	};

	struct color_builder {
		uint32_t current_color = 0;
		graphics::color_rgb color = { 0,0,0 };

		void add_value(int v) {
			switch(current_color) {
				case 0:
					color.r = static_cast<uint8_t>(v);
					break;
				case 1:
					color.g = static_cast<uint8_t>(v);
					break;
				case 2:
					color.b = static_cast<uint8_t>(v);
					break;
				default:
					break;
			}
			++current_color;
		}
	};

	struct poptype_issues {
		single_poptype_environment& env;
		poptype_issues(single_poptype_environment& e) : env(e) {}
		void discard(int) {}
	};
	struct poptype_ideologies {
		single_poptype_environment& env;
		poptype_ideologies(single_poptype_environment& e) : env(e) {}
		void discard(int) {}
	};
	struct poptype_promote_to {
		single_poptype_environment& env;
		poptype_promote_to(single_poptype_environment& e) : env(e) {}
		void discard(int) {}
	};

	inline int read_issue_factor(
		const token_group* start,
		const token_group* end,
		const token_and_type& name,
		single_poptype_environment& env) {

		const auto issue_tag = tag_from_text(
			env.s.issues_m.named_option_index,
			text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, name.start, name.end));
		if(is_valid_index(issue_tag)) {
			const auto ftag = modifiers::parse_modifier_factors(env.s,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::pop,
					triggers::trigger_slot_contents::pop,
					triggers::trigger_slot_contents::empty,
					},
					1.0f, start, end);
			env.s.population_m.issue_inclination.get(env.pt.id, issue_tag) = ftag;
		}
		return 0;
	}
	inline int read_ideology_factor(
		const token_group* start,
		const token_group* end,
		const token_and_type& name,
		single_poptype_environment& env) {

		const auto ideology_tag = tag_from_text(
			env.s.ideologies_m.named_ideology_index,
			text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, name.start, name.end));
		if(is_valid_index(ideology_tag)) {
			const auto ftag = modifiers::parse_modifier_factors(env.s,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::pop,
					triggers::trigger_slot_contents::pop,
					triggers::trigger_slot_contents::empty,
					},
					1.0f, start, end);
			env.s.population_m.ideological_inclination.get(env.pt.id, ideology_tag) = ftag;
		}
		return 0;
	}
	inline int read_promote_to(
		const token_group* start,
		const token_group* end,
		const token_and_type& name,
		single_poptype_environment& env) {

		const auto ptype_tag = tag_from_text(
			env.s.population_m.named_pop_type_index,
			text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, name.start, name.end));
		if(is_valid_index(ptype_tag)) {
			const auto ftag = modifiers::parse_modifier_factors(env.s,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::pop,
					triggers::trigger_slot_contents::pop,
					triggers::trigger_slot_contents::empty,
					},
					1.0f, start, end);
			env.s.population_m.promote_to.get(env.pt.id, ptype_tag) = ftag;
		} else {
			std::abort();
		}
		return 0;
	}

	inline modifiers::factor_tag read_country_migration_target(const token_group* start, const token_group* end, single_poptype_environment& env) {
		return modifiers::parse_modifier_factors(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::empty,
				},
				1.0f, start, end);
	}

	inline modifiers::factor_tag read_migration_target(const token_group* start, const token_group* end, single_poptype_environment& env) {
		return modifiers::parse_modifier_factors(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::empty,
				},
				1.0f, start, end);
	}

	struct poptype_rebels {
		single_poptype_environment& env;
		poptype_rebels(single_poptype_environment& e) : env(e) {}

		void add_rebel(const std::pair<token_and_type, float>& p) {
			//const auto utype_tag = tag_from_text(
			//	env.s.military_m.named_unit_type_index,
			//	text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			//if(is_valid_index(utype_tag))
			//	env.s.population_m.rebel_units.get(env.pt.id, utype_tag) = p.second;
		}
	};

	constexpr economy::goods_qnty_type needs_mix_factor = economy::goods_qnty_type(1) / economy::goods_qnty_type(32);
	struct ed_needs {
		single_poptype_environment& env;
		ed_needs(single_poptype_environment& e) : env(e) {}

		void add_good(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto gtag = tag_from_text(
				env.s.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(gtag)) {
				env.s.population_m.everyday_needs.get(env.pt.id, gtag) += p.second;
				//env.s.population_m.life_needs.get(env.pt.id, gtag) += needs_mix_factor * p.second;
				//env.s.population_m.luxury_needs.get(env.pt.id, gtag) +=  needs_mix_factor * p.second;
			}
		}
	};
	struct l_needs {
		single_poptype_environment& env;
		l_needs(single_poptype_environment& e) : env(e) {}

		void add_good(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto gtag = tag_from_text(
				env.s.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(gtag)) {
				env.s.population_m.life_needs.get(env.pt.id, gtag) += p.second;
				//env.s.population_m.everyday_needs.get(env.pt.id, gtag) += needs_mix_factor * p.second;
				//env.s.population_m.luxury_needs.get(env.pt.id, gtag) += needs_mix_factor * needs_mix_factor * p.second;
			}
		}
	};
	struct x_needs {
		single_poptype_environment& env;
		x_needs(single_poptype_environment& e) : env(e) {}

		void add_good(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto gtag = tag_from_text(
				env.s.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(gtag)) {
				env.s.population_m.luxury_needs.get(env.pt.id, gtag) += p.second;
				//env.s.population_m.everyday_needs.get(env.pt.id, gtag) += needs_mix_factor * p.second;
				//env.s.population_m.life_needs.get(env.pt.id, gtag) += needs_mix_factor * needs_mix_factor * p.second;
			}
		}
	};

	inline std::pair<token_and_type, float> read_token_float(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, float>(l, token_to<float>(r));
	}
	inline std::pair<token_and_type, economy::goods_qnty_type> read_token_econ_type(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, economy::goods_qnty_type>(l, token_to<economy::goods_qnty_type>(r));
	}

	struct poptype_file {
		government_employment poptype_pay;

		single_poptype_environment& env;
		poptype_file(single_poptype_environment& e) : env(e) {}

		void set_sprite(uint8_t v) {
			env.pt.sprite = v;
		}
		void set_color(const color_builder& c) {
			env.pt.color = c.color;
		}
		void set_research_points(uint8_t v) {
			env.pt.research_points = v;
		}
		void set_is_slave(bool v) {
			if(v)
				env.s.population_m.slave = env.pt.id;
		}
		void set_strata(const token_and_type& t) {
			if(is_fixed_token_ci(t, "poor")) {
				env.pt.flags |= pop_type::strata_poor;
			} else if(is_fixed_token_ci(t, "middle")) {
				env.pt.flags |= pop_type::strata_middle;
			} else if(is_fixed_token_ci(t, "rich")) {
				env.pt.flags |= pop_type::strata_rich;
			}
		}
		void discard(int) {}
		void set_unemployment(bool v) {
			if(v)
				env.pt.flags |= pop_type::is_employable;
		}
		void set_leadership(int32_t v) {
			env.s.population_m.officer_leadership = v;
			env.s.population_m.officer = env.pt.id;
		}
		void set_can_reduce_consciousness(bool v) {
			if(v)
				env.s.population_m.clergy = env.pt.id;
		}
		void set_allowed_to_vote(bool v) {
			if(!v)
				env.pt.flags |= pop_type::cannot_vote;
		}
		void set_research_optimum(float v) {
			env.pt.research_optimum = v;
		}
		void set_administrative_efficiency(bool v) {
			if(v)
				env.s.population_m.bureaucrat = env.pt.id;
		}
		void set_can_be_recruited(bool v) {
			if(v)
				env.s.population_m.soldier = env.pt.id;
		}
		void set_can_build(bool v) {
			if(v)
				env.s.population_m.capitalist = env.pt.id;
		}
		void set_can_work_factory(bool v) {
			if(v) {
				env.pt.flags |= pop_type::factory_worker;
				env.s.population_m.factory_workers.push_back(env.pt.id);
			}
		}
		void set_demote_migrant(bool v) {
			if(v)
				env.pt.flags |= pop_type::demote_on_migration;
		}
		void set_is_artisan(bool v) {
			if(v)
				env.s.population_m.artisan = env.pt.id;
		}
		void set_country_migration_target(modifiers::factor_tag t) {
			env.pt.country_migration_target = t;
		}
		void set_migration_target(modifiers::factor_tag t) {
			env.pt.migration_target = t;
		}
		void set_everyday_needs_income(const income& i) {
			poptype_pay.everyday_needs_income_weight = i.weight;
			poptype_pay.everyday_needs_income_type = i.type;
		}
		void set_life_needs_income(const income& i) {
			poptype_pay.life_needs_income_weight = i.weight;
			poptype_pay.life_needs_income_type = i.type;
		}
		void set_luxury_needs_income(const income& i) {
			poptype_pay.luxury_needs_income_weight = i.weight;
			poptype_pay.luxury_needs_income_type = i.type;
		}
		void set_state_capital_only(bool v) {
			if(v)
				env.pt.flags |= pop_type::state_capital_only;
		}
		template<typename T>
		void discard_type(const T&) {}
	};
}

MEMBER_DEF(population::pop_reader, size, "size");
MEMBER_FDEF(population::pop_reader, set_religion, "religion");
MEMBER_FDEF(population::pop_reader, set_culture, "culture");
MEMBER_DEF(population::pop_reader, militancy, "militancy");
MEMBER_FDEF(population::pop_reader, discard, "discard");
MEMBER_FDEF(population::pops_in_province_reader, add_pop, "add_pop");
MEMBER_FDEF(population::pop_file_reader, discard, "discard");

MEMBER_FDEF(population::rebel_gov_reader, add_gov, "gov");
MEMBER_FDEF(population::rebel_reader, set_icon, "icon");
MEMBER_FDEF(population::rebel_reader, discard, "discard");
MEMBER_FDEF(population::rebel_reader, set_will_rise, "will_rise");
MEMBER_FDEF(population::rebel_reader, set_movement_evaluation, "movement_evaluation");
MEMBER_FDEF(population::rebel_reader, set_allow_all_cultures, "allow_all_cultures");
MEMBER_FDEF(population::rebel_reader, set_allow_all_religions, "allow_all_religions");
MEMBER_FDEF(population::rebel_reader, set_allow_all_culture_groups, "allow_all_culture_groups");
MEMBER_FDEF(population::rebel_reader, set_allow_all_ideologies, "allow_all_ideologies");
MEMBER_FDEF(population::rebel_reader, set_break_alliance_on_win, "break_alliance_on_win");
MEMBER_FDEF(population::rebel_reader, set_ideology, "ideology");
MEMBER_FDEF(population::rebel_reader, set_spawn_chance, "spawn_chance");
MEMBER_FDEF(population::rebel_reader, read_government, "government");
MEMBER_FDEF(population::rebel_reader, set_area, "area");
MEMBER_FDEF(population::rebel_reader, set_defection, "defection");
MEMBER_FDEF(population::rebel_reader, set_independence, "independence");
MEMBER_FDEF(population::rebel_reader, set_defect_delay, "defect_delay");
MEMBER_FDEF(population::rebel_reader, set_siege_won_effect, "siege_won_effect");
MEMBER_FDEF(population::rebel_reader, set_siege_won_trigger, "siege_won_trigger");
MEMBER_FDEF(population::rebel_reader, set_demands_enforced_effect, "demands_enforced_effect");
MEMBER_FDEF(population::rebel_reader, set_demands_enforced_trigger, "demands_enforced_trigger");
MEMBER_FDEF(population::rebel_reader, set_occupation_mult, "occupation_mult");

MEMBER_FDEF(population::rebel_types_pre_parse_file, add_rebel_type, "add_rebel_type");
MEMBER_FDEF(population::poptypes_file, set_promotion_chance, "promotion_chance");
MEMBER_FDEF(population::poptypes_file, set_demotion_chance, "demotion_chance");
MEMBER_FDEF(population::poptypes_file, set_migration_chance, "migration_chance");
MEMBER_FDEF(population::poptypes_file, set_colonialmigration_chance, "colonialmigration_chance");
MEMBER_FDEF(population::poptypes_file, set_emigration_chance, "emigration_chance");
MEMBER_FDEF(population::poptypes_file, set_assimilation_chance, "assimilation_chance");
MEMBER_FDEF(population::poptypes_file, set_conversion_chance, "conversion_chance");

MEMBER_FDEF(population::income, set_type, "type");
MEMBER_DEF(population::income, weight, "weight");
MEMBER_FDEF(population::color_builder, add_value, "color");
MEMBER_FDEF(population::poptype_file, set_sprite, "sprite");
MEMBER_FDEF(population::poptype_file, set_color, "color");
MEMBER_FDEF(population::poptype_file, set_research_points, "research_points");
MEMBER_FDEF(population::poptype_file, set_is_slave, "is_slave");
MEMBER_FDEF(population::poptype_file, set_strata, "strata");
MEMBER_FDEF(population::poptype_file, discard, "discard");
MEMBER_FDEF(population::poptype_file, discard_type, "discard_type");
MEMBER_FDEF(population::poptype_file, set_unemployment, "unemployment");
MEMBER_FDEF(population::poptype_file, set_leadership, "leadership");
MEMBER_FDEF(population::poptype_file, set_can_reduce_consciousness, "can_reduce_consciousness");
MEMBER_FDEF(population::poptype_file, set_allowed_to_vote, "allowed_to_vote");
MEMBER_FDEF(population::poptype_file, set_research_optimum, "research_optimum");
MEMBER_FDEF(population::poptype_file, set_administrative_efficiency, "administrative_efficiency");
MEMBER_FDEF(population::poptype_file, set_can_be_recruited, "can_be_recruited");
MEMBER_FDEF(population::poptype_file, set_can_build, "can_build");
MEMBER_FDEF(population::poptype_file, set_can_work_factory, "can_work_factory");
MEMBER_FDEF(population::poptype_file, set_demote_migrant, "demote_migrant");
MEMBER_FDEF(population::poptype_file, set_is_artisan, "is_artisan");
MEMBER_FDEF(population::poptype_file, set_country_migration_target, "country_migration_target");
MEMBER_FDEF(population::poptype_file, set_migration_target, "migration_target");
MEMBER_FDEF(population::poptype_file, set_everyday_needs_income, "everyday_needs_income");
MEMBER_FDEF(population::poptype_file, set_life_needs_income, "life_needs_income");
MEMBER_FDEF(population::poptype_file, set_luxury_needs_income, "luxury_needs_income");
MEMBER_FDEF(population::poptype_file, set_state_capital_only, "state_capital_only");
MEMBER_FDEF(population::ed_needs, add_good, "good");
MEMBER_FDEF(population::l_needs, add_good, "good");
MEMBER_FDEF(population::x_needs, add_good, "good");
MEMBER_FDEF(population::poptype_rebels, add_rebel, "rebel");
MEMBER_FDEF(population::poptype_issues, discard, "discard");
MEMBER_FDEF(population::poptype_ideologies, discard, "discard");
MEMBER_FDEF(population::poptype_promote_to, discard, "discard");

namespace population {
	BEGIN_DOMAIN(pop_parsing_domain)
		BEGIN_TYPE(pop_reader)
		MEMBER_ASSOCIATION("size", "size", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("militancy", "militancy", value_from_rh<float>)
		MEMBER_ASSOCIATION("religion", "religion", token_from_rh)
		MEMBER_ASSOCIATION("culture", "culture", token_from_rh)
		MEMBER_ASSOCIATION("discard", "rebel_type", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(pops_in_province_reader)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("add_pop", accept_all, pop_reader, read_pop)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(pop_file_domain)
		BEGIN_TYPE(pop_file_reader)
		MEMBER_VARIABLE_TYPE_EXTERN("discard", accept_all, int, read_pops_in_province)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(single_rebel_domain)
		BEGIN_TYPE(rebel_gov_reader)
		MEMBER_VARIABLE_ASSOCIATION("gov", accept_all, get_tpair)
		END_TYPE
		BEGIN_TYPE(rebel_reader)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("discard", "unit_transfer", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "general", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "resilient", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "reinforcing", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "smart", discard_from_rh)
		MEMBER_TYPE_EXTERN("will_rise", "will_rise", modifiers::factor_tag, read_rebel_will_rise_factor)
		MEMBER_TYPE_EXTERN("spawn_chance", "spawn_chance", modifiers::factor_tag, read_rebel_spawn_factor)
		MEMBER_TYPE_EXTERN("movement_evaluation", "movement_evaluation", modifiers::factor_tag, read_rebel_movement_evaluation_factor)
		MEMBER_ASSOCIATION("allow_all_cultures", "allow_all_cultures", value_from_rh<bool>)
		MEMBER_ASSOCIATION("allow_all_religions", "allow_all_religions", value_from_rh<bool>)
		MEMBER_ASSOCIATION("allow_all_culture_groups", "allow_all_culture_groups", value_from_rh<bool>)
		MEMBER_ASSOCIATION("allow_all_ideologies", "allow_all_ideologies", value_from_rh<bool>)
		MEMBER_ASSOCIATION("break_alliance_on_win", "break_alliance_on_win", value_from_rh<bool>)
		MEMBER_ASSOCIATION("ideology", "ideology", token_from_rh)
		MEMBER_ASSOCIATION("area", "area", token_from_rh)
		MEMBER_ASSOCIATION("defection", "defection", token_from_rh)
		MEMBER_ASSOCIATION("independence", "independence", token_from_rh)
		MEMBER_ASSOCIATION("defect_delay", "defect_delay", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("occupation_mult", "occupation_mult", value_from_rh<float>)
		MEMBER_TYPE_EXTERN("siege_won_trigger", "siege_won_trigger", triggers::trigger_tag, read_rebel_siege_won_trigger)
		MEMBER_TYPE_EXTERN("siege_won_effect", "siege_won_effect", triggers::trigger_tag, read_rebel_siege_won_effect)
		MEMBER_TYPE_EXTERN("demands_enforced_trigger", "demands_enforced_trigger", triggers::trigger_tag, read_rebel_demands_enforced_trigger)
		MEMBER_TYPE_EXTERN("demands_enforced_effect", "demands_enforced_effect", triggers::trigger_tag, read_rebel_demands_enforced_effect)
		MEMBER_TYPE_ASSOCIATION("government", "government", rebel_gov_reader)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(poptype_file_domain)
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
		BEGIN_TYPE(poptype_rebels)
		MEMBER_VARIABLE_ASSOCIATION("rebel", accept_all, read_token_float)
		END_TYPE
		BEGIN_TYPE(ed_needs)
		MEMBER_VARIABLE_ASSOCIATION("good", accept_all, read_token_econ_type)
		END_TYPE
		BEGIN_TYPE(l_needs)
		MEMBER_VARIABLE_ASSOCIATION("good", accept_all, read_token_econ_type)
		END_TYPE
		BEGIN_TYPE(x_needs)
		MEMBER_VARIABLE_ASSOCIATION("good", accept_all, read_token_econ_type)
		END_TYPE
		BEGIN_TYPE(income)
		MEMBER_ASSOCIATION("weight", "weight", value_from_rh<float>)
		MEMBER_ASSOCIATION("type", "type", token_from_rh)
		END_TYPE
		BEGIN_TYPE(poptype_issues)
		MEMBER_VARIABLE_TYPE_EXTERN("discard", accept_all, int, read_issue_factor)
		END_TYPE
		BEGIN_TYPE(poptype_ideologies)
		MEMBER_VARIABLE_TYPE_EXTERN("discard", accept_all, int, read_ideology_factor)
		END_TYPE
		BEGIN_TYPE(poptype_promote_to)
		MEMBER_VARIABLE_TYPE_EXTERN("discard", accept_all, int, read_promote_to)
		END_TYPE
		BEGIN_TYPE(poptype_file)
		MEMBER_ASSOCIATION("sprite", "sprite", value_from_rh<uint8_t>)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		MEMBER_ASSOCIATION("research_points", "research_points", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("is_slave", "is_slave", value_from_rh<bool>)
		MEMBER_ASSOCIATION("strata", "strata", token_from_rh)
		MEMBER_ASSOCIATION("discard", "factory", discard_from_rh)
		MEMBER_ASSOCIATION("can_work_factory", "can_work_factory", value_from_rh<bool>)
		MEMBER_ASSOCIATION("discard", "rgo", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "can_work_rgo", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "max_size", discard_from_rh)
		MEMBER_TYPE_ASSOCIATION("discard_type", "issues", poptype_issues)
		MEMBER_TYPE_ASSOCIATION("discard_type", "ideologies", poptype_ideologies)
		MEMBER_TYPE_ASSOCIATION("discard_type", "promote_to", poptype_promote_to)
		MEMBER_ASSOCIATION("unemployment", "unemployment", value_from_rh<bool>)
		MEMBER_ASSOCIATION("discard", "tax_eff", discard_from_rh)
		MEMBER_ASSOCIATION("leadership", "leadership", value_from_rh<int32_t>)
		MEMBER_TYPE_ASSOCIATION("discard_type", "rebel", poptype_rebels)
		MEMBER_ASSOCIATION("can_reduce_consciousness", "can_reduce_consciousness", value_from_rh<bool>)
		MEMBER_ASSOCIATION("allowed_to_vote", "allowed_to_vote", value_from_rh<bool>)
		MEMBER_ASSOCIATION("research_optimum", "research_optimum", value_from_rh<float>)
		MEMBER_ASSOCIATION("administrative_efficiency", "administrative_efficiency", value_from_rh<bool>)
		MEMBER_ASSOCIATION("can_be_recruited", "can_be_recruited", value_from_rh<bool>)
		MEMBER_ASSOCIATION("can_build", "can_build", value_from_rh<bool>)
		MEMBER_ASSOCIATION("discard", "merge_size", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "merge_max_size", discard_from_rh)
		MEMBER_ASSOCIATION("demote_migrant", "demote_migrant", value_from_rh<bool>)
		MEMBER_ASSOCIATION("is_artisan", "is_artisan", value_from_rh<bool>)
		MEMBER_TYPE_EXTERN("country_migration_target", "country_migration_target", modifiers::factor_tag, read_country_migration_target)
		MEMBER_TYPE_EXTERN("migration_target", "migration_target", modifiers::factor_tag, read_migration_target)
		MEMBER_ASSOCIATION("discard", "workplace_input", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "workplace_output", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "starter_share", discard_from_rh)
		MEMBER_TYPE_ASSOCIATION("discard_type", "everyday_needs", ed_needs)
		MEMBER_TYPE_ASSOCIATION("discard_type", "life_needs", l_needs)
		MEMBER_TYPE_ASSOCIATION("discard_type", "luxury_needs", x_needs)
		MEMBER_TYPE_ASSOCIATION("everyday_needs_income", "everyday_needs_income", income)
		MEMBER_TYPE_ASSOCIATION("life_needs_income", "life_needs_income", income)
		MEMBER_TYPE_ASSOCIATION("luxury_needs_income", "luxury_needs_income", income)
		MEMBER_ASSOCIATION("discard", "minimum_promotion", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "equivalent", discard_from_rh)
		MEMBER_ASSOCIATION("state_capital_only", "state_capital_only", value_from_rh<bool>)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(rebel_types_pre_parsing_domain)
		BEGIN_TYPE(rebel_types_pre_parse_file)
		MEMBER_VARIABLE_TYPE_EXTERN("add_rebel_type", accept_all, int, inner_pre_parse_rebel)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(poptypes_file_domain)
		BEGIN_TYPE(poptypes_file)
		MEMBER_TYPE_EXTERN("promotion_chance", "promotion_chance", modifiers::factor_tag, read_poptype_file_modifier)
		MEMBER_TYPE_EXTERN("demotion_chance", "demotion_chance", modifiers::factor_tag, read_poptype_file_modifier)
		MEMBER_TYPE_EXTERN("migration_chance", "migration_chance", modifiers::factor_tag, read_poptype_file_modifier)
		MEMBER_TYPE_EXTERN("colonialmigration_chance", "colonialmigration_chance", modifiers::factor_tag, read_poptype_file_modifier)
		MEMBER_TYPE_EXTERN("emigration_chance", "emigration_chance", modifiers::factor_tag, read_poptype_file_modifier)
		MEMBER_TYPE_EXTERN("assimilation_chance", "assimilation_chance", modifiers::factor_tag, read_poptype_file_modifier)
		MEMBER_TYPE_EXTERN("conversion_chance", "conversion_chance", modifiers::factor_tag, read_poptype_file_modifier)
		END_TYPE
	END_DOMAIN;

	int read_pops_in_province(token_group const* s, token_group const* e, token_and_type const& pid, world_state& ws) {
		provinces::province_tag prov(token_to<uint16_t>(pid));

		if(is_valid_index(prov) && to_index(prov) < ws.s.province_m.province_container.size()) {
			pops_in_province_environment env(ws, prov);
			parse_object<pops_in_province_reader, pop_parsing_domain>(s, e, env);
		}

		return 0;
	}

	void read_pop_file(token_group const* s, token_group const* e, world_state& ws) {
		parse_object<pop_file_reader, pop_file_domain>(s, e, ws);
	}

	void read_all_pops(directory const& root, world_state& ws, date_tag target_date) {
		const auto history_dir = root.get_directory(u"\\history");
		const auto pop_dir = history_dir.get_directory(u"\\pops");

		auto directories = pop_dir.list_directories();
		if(directories.size() > 0) {
			directory* best = &directories[0];
			const auto fname = directories[0].name();
			date_tag best_date = parse_date(fname.c_str() + 1, fname.c_str() + fname.length());

			for(uint32_t i = 1; i < directories.size(); ++i) {
				const auto iname = directories[i].name();
				date_tag i_date = parse_date(iname.c_str() + 1, iname.c_str() + iname.length());

				if((target_date < best_date || best_date < i_date) && i_date < target_date) {
					best = &directories[i];
					best_date = i_date;
				}
			}

			auto pop_files = best->list_files(u".txt");
			for(auto& f : pop_files) {
				if(auto fi = f.open_file(); fi) {
					const auto sz = fi->size();
					std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
					fi->read_to_buffer(parse_data.get(), sz);

					std::vector<token_group> parse_results;

					parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

					if(parse_results.size() > 0) 
						read_pop_file(parse_results.data(), parse_results.data() + parse_results.size(), ws);
				}
			}
		}
	}

	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function) {

		const auto poptype_dir = source_directory.get_directory(u"\\poptypes");
		const auto poptype_files = poptype_dir.list_files(u".txt");

		for(const auto& file : poptype_files) {
			const auto fname = file.file_name();
			const auto ufname = UTF16toUTF8(fname);
			const auto clipped_unicode = fname.size() >= 4ui64 ? std::string(ufname.begin(), ufname.end() - 4ui32) : std::string("");
			const auto name_tag = text_data::get_thread_safe_text_handle(text_function, clipped_unicode.c_str(), clipped_unicode.c_str() + clipped_unicode.size());

			++manager.count_poptypes;
			const auto new_ptype_tag = manager.pop_types.emplace_back();
			auto& new_poptype = manager.pop_types[new_ptype_tag];
			new_poptype.id = new_ptype_tag;
			new_poptype.name = name_tag;
			manager.named_pop_type_index.emplace(name_tag, new_ptype_tag);
		}
	}

	void pre_parse_rebel_types(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto fi = common_dir.open_file(u"rebel_types.txt");

		if(fi) {
			auto& main_results = state.impl->rebel_types_file;

			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<rebel_types_pre_parse_file, rebel_types_pre_parsing_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void read_main_poptype_file(scenario::scenario_manager& s, const directory& root) {
		const auto common_dir = root.get_directory(u"\\common");
		const auto fi = common_dir.open_file(u"pop_types.txt");

		if(fi) {
			parsed_data main_results;

			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<poptypes_file, poptypes_file_domain>(
					main_results.parse_results.data(),
					main_results.parse_results.data() + main_results.parse_results.size(),
					s);
			}
		}
	}

	void read_poptypes(scenario::scenario_manager& s, const directory& root) {
		const auto poptype_dir = root.get_directory(u"\\poptypes");

		const auto poptype_files = poptype_dir.list_files(u".txt");

		s.population_m.promote_to.reset(s.population_m.count_poptypes);
		s.population_m.promote_to.resize(s.population_m.count_poptypes);

		const auto goods_count = s.economy_m.aligned_32_goods_count;
		s.population_m.everyday_needs.reset(goods_count);
		s.population_m.everyday_needs.resize(s.population_m.count_poptypes);
		s.population_m.life_needs.reset(goods_count);
		s.population_m.life_needs.resize(s.population_m.count_poptypes);
		s.population_m.luxury_needs.reset(goods_count);
		s.population_m.luxury_needs.resize(s.population_m.count_poptypes);

		// s.population_m.rebel_units.reset(static_cast<uint32_t>(s.military_m.unit_types.size()));
		// s.population_m.rebel_units.resize(s.population_m.count_poptypes);
		s.population_m.issue_inclination.reset(static_cast<uint32_t>(s.issues_m.options.size()));
		s.population_m.issue_inclination.resize(s.population_m.count_poptypes);
		s.population_m.ideological_inclination.reset(static_cast<uint32_t>(s.ideologies_m.ideology_container.size()));
		s.population_m.ideological_inclination.resize(s.population_m.count_poptypes);

		for(const auto& file : poptype_files) {
			const auto fname = file.file_name();
			const auto ufname = UTF16toUTF8(fname);
			const auto clipped_unicode = fname.size() >= 4ui64 ? std::string(ufname.begin(), ufname.end() - 4ui32) : std::string("");
			const auto name_tag = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, clipped_unicode.c_str(), clipped_unicode.c_str() + clipped_unicode.size());

			const auto poptype_tag = s.population_m.named_pop_type_index.at(name_tag);
			auto& poptype_obj = s.population_m.pop_types[poptype_tag];

			auto fi = file.open_file();

			if(fi) {
				parsed_data main_results;

				const auto sz = fi->size();
				main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

				fi->read_to_buffer(main_results.parse_data.get(), sz);
				parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

				single_poptype_environment env(s, poptype_obj);

				if(main_results.parse_results.size() > 0) {
					const auto pr = parse_object<poptype_file, poptype_file_domain>(
						&main_results.parse_results[0],
						&main_results.parse_results[0] + main_results.parse_results.size(),
						env);
					if(poptype_tag == s.population_m.bureaucrat) {
						s.population_m.bureaucrat_pay = pr.poptype_pay;
					} else if(poptype_tag == s.population_m.clergy) {
						s.population_m.clergy_pay = pr.poptype_pay;
					} else if(poptype_tag == s.population_m.officer) {
						s.population_m.officer_pay = pr.poptype_pay;
					} else if(poptype_tag == s.population_m.soldier) {
						s.population_m.soldier_pay = pr.poptype_pay;
					}
				}
			}
		}

		populate_demote_to(s.population_m);
	}

	void populate_demote_to(population_manager& m) {
		const auto poptype_count = m.promote_to.inner_size();
		m.demote_to.reset(poptype_count);
		m.demote_to.resize(poptype_count);

		for(uint32_t i = 0; i < poptype_count; ++i) {
			const auto outer_strata = m.pop_types[pop_type_tag(static_cast<pop_type_tag::value_base_t>(i))].flags & pop_type::strata_mask;

			for(uint32_t j = 0; j < poptype_count; ++j) {
				const auto inner_strata = m.pop_types[pop_type_tag(static_cast<pop_type_tag::value_base_t>(j))].flags & pop_type::strata_mask;

				if(inner_strata <= outer_strata) {
					m.demote_to.get(
						pop_type_tag(static_cast<pop_type_tag::value_base_t>(i)),
						pop_type_tag(static_cast<pop_type_tag::value_base_t>(j))) =

						m.promote_to.get(
							pop_type_tag(static_cast<pop_type_tag::value_base_t>(i)),
							pop_type_tag(static_cast<pop_type_tag::value_base_t>(j)));

					if(inner_strata != outer_strata)
						m.promote_to.get(pop_type_tag(
							static_cast<pop_type_tag::value_base_t>(i)),
							pop_type_tag(static_cast<pop_type_tag::value_base_t>(j))) = modifiers::factor_tag();
				}

			}
		}
	}

	void read_rebel_types(parsing_state const& state, scenario::scenario_manager& s, events::event_creation_manager& ecm) {
		const uint32_t num_governments = static_cast<uint32_t>(s.governments_m.governments_container.size());
		const uint32_t num_rebels = static_cast<uint32_t>(s.population_m.rebel_types.size());

		s.population_m.rebel_change_government_to.reset(num_governments);
		s.population_m.rebel_change_government_to.resize(num_rebels);
		for(uint32_t i = 0; i < num_rebels; ++i) {
			for(uint32_t j = 0; j < num_governments; ++j) {
				s.population_m.rebel_change_government_to.get(
					rebel_type_tag(static_cast<rebel_type_tag::value_base_t>(i)),
					governments::government_tag(static_cast<governments::government_tag::value_base_t>(j))) =
					governments::government_tag(static_cast<governments::government_tag::value_base_t>(j));
			}
		}

		for(auto const& t : state.impl->pending_rebels) {
			rebel_type& reb = s.population_m.rebel_types[std::get<0>(t)];
			rebel_reading_env env(s, ecm, reb);
			parse_object<rebel_reader, single_rebel_domain>(std::get<1>(t), std::get<2>(t), env);
		}
	}
}
