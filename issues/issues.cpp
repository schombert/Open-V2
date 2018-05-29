#include "issues.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include <map>
#include "modifiers\\modifiers.h"
#include "scenario\\scenario.h"
#include "triggers\\trigger_reading.h"
#include "triggers\\effect_reading.h"

namespace issues {
	struct parsing_environment {
		text_handle_lookup text_lookup;
		issues_manager& manager;

		parsed_data main_file_parse_tree;
		std::map<option_tag, std::pair<const token_group*, const token_group*>> parsed_options;

		parsing_environment(const text_handle_lookup& tl, issues_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, issues_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	struct empty_type {
		void add_unknown_key(int) {
		}
	};

	struct preparse_issue_s {
		parsing_environment& env;
		issue& issue_t;

		preparse_issue_s(parsing_environment& e, issue& t) : env(e), issue_t(t) {}
		void set_administrative(bool v) {
			issue_t.administrative = v;
		}
		void set_next_step_only(bool v) {
			issue_t.next_step_only = v;
		}
		void add_option(int) { }
	};
	

	inline int preparse_option(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env, issue& i) {
		const auto opt_tag = env.manager.options.emplace_back();
		issue_option& opt = env.manager.options[opt_tag];

		for(size_t j = 0; j < std::extent<decltype(i.options)>::value; ++j) {
			if(!is_valid_index(i.options[j])) {
				i.options[j] = opt_tag;
				break;
			}
		}

		opt.name = env.text_lookup(t.start, t.end);
		opt.id = opt_tag;
		opt.parent_issue = i.id;

		env.parsed_options.emplace(opt_tag, std::make_pair(s, e));
		env.manager.named_option_index.emplace(opt.name, opt_tag);
		return 0;
	}

	template<issue_group type_constant>
	inline int preparse_issue(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env) {
		const auto issue_t = env.manager.issues_cotnainer.emplace_back();
		issue& i = env.manager.issues_cotnainer[issue_t];
		i.id = issue_t;
		i.type = type_constant;
		i.name = env.text_lookup(t.start, t.end);

		if constexpr(type_constant == issue_group::party) {
			env.manager.party_issues.push_back(issue_t);
		} else if constexpr(type_constant == issue_group::political) {
			env.manager.political_issues.push_back(issue_t);
		} else if constexpr(type_constant == issue_group::social) {
			env.manager.social_issues.push_back(issue_t);
		} else if constexpr(type_constant == issue_group::military) {
			env.manager.military_issues.push_back(issue_t);
		} else if constexpr(type_constant == issue_group::economic) {
			env.manager.economic_issues.push_back(issue_t);
		}

		env.manager.named_issue_index.emplace(i.name, issue_t);
		parse_object<preparse_issue_s, inner_issue_preparse_domain>(s, e, env, i);

		return 0;
	}

	struct party_issues_group {
		parsing_environment& env;

		party_issues_group(parsing_environment& e) : env(e) {}
		void add_issue(int) {}
	};

	struct political_issues_group {
		parsing_environment& env;

		political_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(int) {}
	};
	struct social_issues_group {
		parsing_environment& env;

		social_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(int) {}
	};
	struct economic_issues_group {
		parsing_environment& env;

		economic_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(int) {}
	};
	struct military_issues_group {
		parsing_environment& env;

		military_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(int) {}
	};

	struct preparse_issues_file {
		void insert_issue_group(const military_issues_group&) {}
		void insert_issue_group(const economic_issues_group&) {}
		void insert_issue_group(const social_issues_group&) {}
		void insert_issue_group(const political_issues_group&) {}
		void insert_issue_group(const party_issues_group&) {}
	};

	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }

	inline std::tuple<bool, uint32_t, uint32_t> rule_value(association_type, const token_and_type& r, uint32_t value, uint32_t mask) {
		return std::tuple<bool, uint32_t, uint32_t>(token_to<bool>(r), value, mask);
	}

	struct rules_reader : public rules_set {
		void set_rule(const std::tuple<bool, uint32_t, uint32_t>& v) {
			if(std::get<0>(v))
				rules_settings.rules |= std::get<1>(v);
			rules_mask.rules |= std::get<2>(v);
		}
	};

	struct option_parsing_env {
		scenario::scenario_manager& s;
		events::event_creation_manager& ecm;
		issue_option& opt;

		option_parsing_env(scenario::scenario_manager& sm, events::event_creation_manager& e, issue_option& o) : s(sm), ecm(e), opt(o) {}
	};
	struct option_on_execute {
		option_parsing_env& env;
		option_on_execute(option_parsing_env& e) : env(e) {}
		void set_trigger(triggers::trigger_tag t) {
			env.opt.on_execute_trigger = t;
		}
		void set_effect(triggers::effect_tag t) {
			env.opt.on_execute_effect = t;
		}
	};
	struct option_reader : public modifiers::modifier_reading_base {
		option_parsing_env& env;
		option_reader(option_parsing_env& e) : env(e) {}
		void set_war_exhaustion_effect(float v) {
			env.opt.war_exhaustion_effect = v;
		}
		void set_technology_cost(int32_t v) {
			env.opt.technology_cost = v;
		}
		void set_administrative_multiplier(int16_t v) {
			env.opt.administrative_multiplier = v;
		}
		void set_rules(const rules_set& r) {
			env.opt.issue_rules = r;
		}
		void set_allow(triggers::trigger_tag t) {
			env.opt.allow = t;
		}
		void set_is_jingoism(bool v) {
			if(v)
				env.s.issues_m.jingoism = env.opt.id;
		}
		void set_on_execute(const option_on_execute&) {}
	};

	inline std::pair<token_and_type, float> full_to_tf_pair(const token_and_type& t, association_type, const token_and_type& r) {
		return std::pair<token_and_type, float>(t, token_to<float>(r));
	}
	inline triggers::trigger_tag read_option_trigger(const token_group* s, const token_group* e, option_parsing_env& env) {
		const auto result = triggers::parse_trigger(env.s, triggers::trigger_scope_state{
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::empty,
			false }, s, e);
		return triggers::commit_trigger(env.s.trigger_m, result);
	}
	inline triggers::effect_tag read_option_effect(const token_group* s, const token_group* e, option_parsing_env& env) {
		const auto result = triggers::parse_effect(env.s, env.ecm, triggers::trigger_scope_state{
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::empty,
			false }, s, e);
		return triggers::commit_effect(env.s.trigger_m, result);
	}
	inline rules_set read_option_rules(const token_group* s, const token_group* e, option_parsing_env&) {
		return read_rules(s, e);
	}
}

MEMBER_FDEF(issues::party_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::political_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::social_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::economic_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::military_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::preparse_issues_file, insert_issue_group, "group");
MEMBER_FDEF(issues::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(issues::rules_reader, set_rule, "rule");
MEMBER_FDEF(issues::preparse_issue_s, set_administrative, "administrative");
MEMBER_FDEF(issues::preparse_issue_s, set_next_step_only, "next_step_only");
MEMBER_FDEF(issues::preparse_issue_s, add_option, "option");

MEMBER_FDEF(issues::option_on_execute, set_trigger, "trigger");
MEMBER_FDEF(issues::option_on_execute, set_effect, "effect");
MEMBER_FDEF(issues::option_reader, set_war_exhaustion_effect, "war_exhaustion_effect");
MEMBER_FDEF(issues::option_reader, set_technology_cost, "technology_cost");
MEMBER_FDEF(issues::option_reader, set_administrative_multiplier, "administrative_multiplier");
MEMBER_FDEF(issues::option_reader, set_rules, "rules");
MEMBER_FDEF(issues::option_reader, set_allow, "allow");
MEMBER_FDEF(issues::option_reader, set_is_jingoism, "is_jingoism");
MEMBER_FDEF(issues::option_reader, set_on_execute, "on_execute");
MEMBER_DEF(issues::option_reader, icon, "icon");
MEMBER_FDEF(issues::option_reader, add_attribute, "attribute");

namespace issues {
	BEGIN_DOMAIN(rules_parsing)
		BEGIN_TYPE(rules_reader)
		MEMBER_ASSOCIATION_2("rule", "build_factory", rule_value, rules::build_factory, rules::build_factory)
		MEMBER_ASSOCIATION_2("rule", "expand_factory", rule_value, rules::expand_factory, rules::expand_factory)
		MEMBER_ASSOCIATION_2("rule", "open_factory", rule_value, rules::open_factory, rules::open_factory)
		MEMBER_ASSOCIATION_2("rule", "destroy_factory", rule_value, rules::destroy_factory, rules::destroy_factory)
		MEMBER_ASSOCIATION_2("rule", "factory_priority", rule_value, rules::factory_priority, rules::factory_priority)
		MEMBER_ASSOCIATION_2("rule", "can_subsidise", rule_value, rules::can_subsidise, rules::can_subsidise)
		MEMBER_ASSOCIATION_2("rule", "pop_build_factory", rule_value, rules::pop_build_factory, rules::pop_build_factory)
		MEMBER_ASSOCIATION_2("rule", "pop_expand_factory", rule_value, rules::pop_expand_factory, rules::pop_expand_factory)
		MEMBER_ASSOCIATION_2("rule", "pop_open_factory", rule_value, rules::pop_open_factory, rules::pop_open_factory)
		MEMBER_ASSOCIATION_2("rule", "delete_factory_if_no_input", rule_value, rules::delete_factory_if_no_input, rules::delete_factory_if_no_input)
		MEMBER_ASSOCIATION_2("rule", "build_factory_invest", rule_value, rules::build_factory_invest, rules::build_factory_invest)
		MEMBER_ASSOCIATION_2("rule", "expand_factory_invest", rule_value, rules::expand_factory_invest, rules::expand_factory_invest)
		MEMBER_ASSOCIATION_2("rule", "open_factory_invest", rule_value, rules::open_factory_invest, rules::open_factory_invest)
		MEMBER_ASSOCIATION_2("rule", "build_railway_invest", rule_value, rules::build_railway_invest, rules::build_railway_invest)
		MEMBER_ASSOCIATION_2("rule", "can_invest_in_pop_projects", rule_value, rules::can_invest_in_pop_projects, rules::can_invest_in_pop_projects)
		MEMBER_ASSOCIATION_2("rule", "pop_build_factory_invest", rule_value, rules::pop_build_factory_invest, rules::pop_build_factory_invest)
		MEMBER_ASSOCIATION_2("rule", "pop_expand_factory_invest", rule_value, rules::pop_expand_factory_invest, rules::pop_expand_factory_invest)
		MEMBER_ASSOCIATION_2("rule", "pop_open_factory_invest", rule_value, rules::pop_open_factory_invest, rules::pop_open_factory_invest)
		MEMBER_ASSOCIATION_2("rule", "allow_foreign_investment", rule_value, rules::allow_foreign_investment, rules::allow_foreign_investment)
		MEMBER_ASSOCIATION_2("rule", "slavery_allowed", rule_value, rules::slavery_allowed, rules::slavery_allowed)
		MEMBER_ASSOCIATION_2("rule", "primary_culture_voting", rule_value, rules::primary_culture_voting, rules::citizens_rights_mask)
		MEMBER_ASSOCIATION_2("rule", "culture_voting", rule_value, rules::culture_voting, rules::citizens_rights_mask)
		MEMBER_ASSOCIATION_2("rule", "all_voting", rule_value, rules::all_voting, rules::citizens_rights_mask)
		MEMBER_ASSOCIATION_2("rule", "largest_share", rule_value, rules::largest_share, rules::voting_system_mask)
		MEMBER_ASSOCIATION_2("rule", "dhont", rule_value, rules::dhont, rules::voting_system_mask)
		MEMBER_ASSOCIATION_2("rule", "sainte_lauqe", rule_value, rules::sainte_lauqe, rules::voting_system_mask)
		MEMBER_ASSOCIATION_2("rule", "same_as_ruling_party", rule_value, rules::same_as_ruling_party, rules::upper_house_composition_mask)
		MEMBER_ASSOCIATION_2("rule", "rich_only", rule_value, rules::rich_only, rules::upper_house_composition_mask)
		MEMBER_ASSOCIATION_2("rule", "state_vote", rule_value, rules::state_vote, rules::upper_house_composition_mask)
		MEMBER_ASSOCIATION_2("rule", "population_vote", rule_value, rules::population_vote, rules::upper_house_composition_mask)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(option_parsing_domain)
		BEGIN_TYPE(option_on_execute)
		MEMBER_TYPE_EXTERN("trigger", "trigger", triggers::trigger_tag, read_option_trigger)
		MEMBER_TYPE_EXTERN("effect", "effect", triggers::trigger_tag, read_option_effect)
		END_TYPE
		BEGIN_TYPE(option_reader)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint32_t>)
		MEMBER_ASSOCIATION("technology_cost", "technology_cost", value_from_rh<int32_t>)
		MEMBER_ASSOCIATION("war_exhaustion_effect", "war_exhaustion_effect", value_from_rh<float>)
		MEMBER_ASSOCIATION("administrative_multiplier", "administrative_multiplier", value_from_rh<int16_t>)
		MEMBER_ASSOCIATION("is_jingoism", "is_jingoism", value_from_rh<bool>)
		MEMBER_TYPE_EXTERN("allow", "allow", triggers::trigger_tag, read_option_trigger)
		MEMBER_TYPE_EXTERN("rules", "rules", rules_set, read_option_rules)
		MEMBER_TYPE_ASSOCIATION("on_execute", "on_execute", option_on_execute)
		MEMBER_VARIABLE_ASSOCIATION("attribute", accept_all, full_to_tf_pair)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(inner_issue_preparse_domain)
		BEGIN_TYPE(preparse_issue_s)
		MEMBER_ASSOCIATION("administrative", "administrative", value_from_rh<bool>)
		MEMBER_ASSOCIATION("next_step_only", "next_step_only", value_from_rh<bool>)
		MEMBER_VARIABLE_TYPE_EXTERN("option", accept_all, int, preparse_option)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(issues_pre_parsing_domain)
		BEGIN_TYPE(party_issues_group)
		MEMBER_VARIABLE_TYPE_EXTERN("issue", accept_all, int, preparse_issue<issue_group::party>)
		END_TYPE
		BEGIN_TYPE(political_issues_group)
		MEMBER_VARIABLE_TYPE_EXTERN("issue", accept_all, int, preparse_issue<issue_group::political>)
		END_TYPE
		BEGIN_TYPE(social_issues_group)
		MEMBER_VARIABLE_TYPE_EXTERN("issue", accept_all, int, preparse_issue<issue_group::social>)
		END_TYPE
		BEGIN_TYPE(economic_issues_group)
		MEMBER_VARIABLE_TYPE_EXTERN("issue", accept_all, int, preparse_issue<issue_group::economic>)
		END_TYPE
		BEGIN_TYPE(military_issues_group)
		MEMBER_VARIABLE_TYPE_EXTERN("issue", accept_all, int, preparse_issue<issue_group::military>)
		END_TYPE
		BEGIN_TYPE(preparse_issues_file)
		MEMBER_TYPE_ASSOCIATION("group", "party_issues", party_issues_group)
		MEMBER_TYPE_ASSOCIATION("group", "political_reforms", political_issues_group)
		MEMBER_TYPE_ASSOCIATION("group", "social_reforms", social_issues_group)
		MEMBER_TYPE_ASSOCIATION("group", "economic_reforms", economic_issues_group)
		MEMBER_TYPE_ASSOCIATION("group", "military_reforms", military_issues_group)
		END_TYPE
	END_DOMAIN;

	parsing_state pre_parse_issues(
		issues_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"issues.txt");

		parsing_state return_state(text_function, manager);

		auto& main_results = return_state.impl->main_file_parse_tree;

		if (file) {
			const auto sz = file->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<preparse_issues_file, issues_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*return_state.impl);
			}
		}

		return return_state;
	}

	rules_set read_rules(const token_group* start, const token_group* end) {
		return parse_object<rules_reader, rules_parsing>(start, end);
	}

	void read_issue_options(
		const parsing_state& ps,
		scenario::scenario_manager& s,
		events::event_creation_manager& ecm) {

		for(const auto& pending : ps.impl->parsed_options) {
			issue_option& opt = s.issues_m.options[pending.first];
			option_parsing_env env(s, ecm, opt);
			auto res = parse_object<option_reader, option_parsing_domain>(pending.second.first, pending.second.second, env);
			opt.modifier = modifiers::add_national_modifier(opt.name, res, s.modifiers_m);
		}
	}
}
