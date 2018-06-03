#include "population.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "scenario\\scenario.h"
#include "modifiers\\modifiers.h"

namespace population {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;

		population_manager& manager;

		parsed_data rebel_types_file;

		parsing_environment(text_data::text_sequences& tl, population_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, population_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct rebel_types_pre_parse_file {
		parsing_environment& env;
		rebel_types_pre_parse_file(parsing_environment& e) : env(e) {}

		void add_rebel_type(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
			const auto rtag = env.manager.rebel_types.emplace_back();
			auto& reb = env.manager.rebel_types[rtag];
			reb.id = rtag;
			reb.name = name;
			env.manager.named_rebel_type_index.emplace(name, rtag);
		}
	};

	struct single_poptype_environment {
		scenario::scenario_manager& s;
		pop_type& pt;

		single_poptype_environment(scenario::scenario_manager& sm, pop_type& p) : s(sm), pt(p) {}
	};

	struct empty_type {
		void add_unknown_key(int) {
		}
	};
	inline token_and_type name_empty_type(const token_and_type& t, association_type, const empty_type&) { return t; }
	inline int discard_empty_type(const token_and_type&, association_type, const empty_type&) { return 0; }

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
			env.population_m.colonialmigration_chance = t;
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
				false},
			1.0f, 0.0f, s, e);
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
					false },
					1.0f, 0.0f, start, end);
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
					false },
					1.0f, 0.0f, start, end);
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
					false },
					1.0f, 0.0f, start, end);
			env.s.population_m.promote_to.get(env.pt.id, ptype_tag) = ftag;
		}
		return 0;
	}

	inline modifiers::factor_tag read_country_migration_target(const token_group* start, const token_group* end, single_poptype_environment& env) {
		return modifiers::parse_modifier_factors(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::empty,
				false },
				1.0f, 0.0f, start, end);
	}

	inline modifiers::factor_tag read_migration_target(const token_group* start, const token_group* end, single_poptype_environment& env) {
		return modifiers::parse_modifier_factors(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::pop,
				triggers::trigger_slot_contents::empty,
				false },
				1.0f, 0.0f, start, end);
	}

	struct poptype_rebels {
		single_poptype_environment& env;
		poptype_rebels(single_poptype_environment& e) : env(e) {}

		void add_rebel(const std::pair<token_and_type, float>& p) {
			const auto utype_tag = tag_from_text(
				env.s.military_m.named_unit_type_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(utype_tag))
				env.s.population_m.rebel_units.get(env.pt.id, utype_tag) = p.second;
		}
	};

	struct ed_needs {
		single_poptype_environment& env;
		ed_needs(single_poptype_environment& e) : env(e) {}

		void add_good(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto gtag = tag_from_text(
				env.s.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(gtag))
				env.s.population_m.everyday_needs.get(env.pt.id, gtag) = p.second;
		}
	};
	struct l_needs {
		single_poptype_environment& env;
		l_needs(single_poptype_environment& e) : env(e) {}

		void add_good(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto gtag = tag_from_text(
				env.s.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(gtag))
				env.s.population_m.life_needs.get(env.pt.id, gtag) = p.second;
		}
	};
	struct x_needs {
		single_poptype_environment& env;
		x_needs(single_poptype_environment& e) : env(e) {}

		void add_good(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto gtag = tag_from_text(
				env.s.economy_m.named_goods_index,
				text_data::get_thread_safe_existing_text_handle(env.s.gui_m.text_data_sequences, p.first.start, p.first.end));
			if(is_valid_index(gtag))
				env.s.population_m.luxury_needs.get(env.pt.id, gtag) = p.second;
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
			if(!v)
				env.pt.flags |= pop_type::not_employable;
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

MEMBER_FDEF(population::rebel_types_pre_parse_file, add_rebel_type, "add_rebel_type");
MEMBER_FDEF(population::empty_type, add_unknown_key, "unknown_key");
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
		MEMBER_ASSOCIATION("discard", "can_work_factory", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "rgo", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "can_work_rgo", discard_from_rh)
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
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(rebel_types_pre_parse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("add_rebel_type", accept_all, empty_type, name_empty_type)
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

	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function) {

		const auto poptype_dir = source_directory.get_directory(u"\\poptypes");
		const auto poptype_files = poptype_dir.list_files(u".txt");

		for (const auto& file : poptype_files) {
			const auto fname = file.file_name();
			const auto clipped_unicode = fname.size() >= 4ui64 ? std::string(fname.begin(), fname.end() - 4ui32) : std::string("");
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

		if (fi) {
			auto& main_results = state.impl->rebel_types_file;

			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<rebel_types_pre_parse_file, rebel_types_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
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
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					s);
			}
		}
	}

	void read_poptypes(scenario::scenario_manager& s, const directory& root) {
		const auto poptype_dir = root.get_directory(u"\\poptypes");

		const auto poptype_files = poptype_dir.list_files(u".txt");

		for(const auto& file : poptype_files) {
			const auto fname = file.file_name();
			const auto clipped_unicode = fname.size() >= 4ui64 ? std::string(fname.begin(), fname.end() - 4ui32) : std::string("");
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
	}
}
