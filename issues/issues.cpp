#include "issues.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace issues {
	struct parsing_environment {
		text_handle_lookup text_lookup;
		issues_manager& manager;

		parsed_data main_file_parse_tree;

		parsing_environment(const text_handle_lookup& tl, issues_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, issues_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	struct empty_type {

	};

	struct preparse_issue {
		parsing_environment& env;

		std::vector<text_data::text_tag> option_names;
		bool admin = false;
		bool next_step = false;

		preparse_issue(parsing_environment& e) : env(e) {}

		void add_option(const token_and_type& t) {
			option_names.emplace_back(env.text_lookup(t.start, t.end));
		}
	};

	struct party_issues_group {
		parsing_environment& env;

		party_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(const std::pair<token_and_type, preparse_issue>& i) {
			const auto new_issue_tag = env.manager.issues_cotnainer.emplace_back();
			auto& new_issue = env.manager.issues_cotnainer[new_issue_tag];
			new_issue.administrative = i.second.admin;
			new_issue.next_step_only = i.second.next_step;
			new_issue.id = new_issue_tag;
			new_issue.type = issue_group::party;
			new_issue.name = env.text_lookup(i.first.start, i.first.end);

			env.manager.party_issues.push_back(new_issue_tag);

			issue_idenfitier new_ident;
			new_ident.id = new_issue_tag;
			new_ident.type = issue_group::party;

			env.manager.named_issue_index.emplace(new_issue.name, new_ident);

			for (auto n : i.second.option_names) {
				const auto option_tag = new_issue.options.emplace_back();
				auto& new_option = new_issue.options[option_tag];
				new_option.id = option_tag;
				new_option.name = n;

				option_identifier new_opt_ident;
				new_opt_ident.id = new_ident.id;
				new_opt_ident.type = new_ident.type;
				new_opt_ident.option_id = option_tag;

				env.manager.named_option_index.emplace(n, new_opt_ident);
			}
		}
	};
	struct political_issues_group {
		parsing_environment& env;

		political_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(const std::pair<token_and_type, preparse_issue>& i) {
			const auto new_issue_tag = env.manager.issues_cotnainer.emplace_back();
			auto& new_issue = env.manager.issues_cotnainer[new_issue_tag];
			new_issue.administrative = i.second.admin;
			new_issue.next_step_only = i.second.next_step;
			new_issue.id = new_issue_tag;
			new_issue.type = issue_group::political;
			new_issue.name = env.text_lookup(i.first.start, i.first.end);

			env.manager.political_issues.push_back(new_issue_tag);

			issue_idenfitier new_ident;
			new_ident.id = new_issue_tag;
			new_ident.type = issue_group::political;

			env.manager.named_issue_index.emplace(new_issue.name, new_ident);

			for (auto n : i.second.option_names) {
				const auto option_tag = new_issue.options.emplace_back();
				auto& new_option = new_issue.options[option_tag];
				new_option.id = option_tag;
				new_option.name = n;

				option_identifier new_opt_ident;
				new_opt_ident.id = new_ident.id;
				new_opt_ident.type = new_ident.type;
				new_opt_ident.option_id = option_tag;

				env.manager.named_option_index.emplace(n, new_opt_ident);
			}
		}
	};
	struct social_issues_group {
		parsing_environment& env;

		social_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(const std::pair<token_and_type, preparse_issue>& i) {
			const auto new_issue_tag = env.manager.issues_cotnainer.emplace_back();
			auto& new_issue = env.manager.issues_cotnainer[new_issue_tag];
			new_issue.administrative = i.second.admin;
			new_issue.next_step_only = i.second.next_step;
			new_issue.id = new_issue_tag;
			new_issue.type = issue_group::social;
			new_issue.name = env.text_lookup(i.first.start, i.first.end);

			env.manager.social_issues.push_back(new_issue_tag);

			issue_idenfitier new_ident;
			new_ident.id = new_issue_tag;
			new_ident.type = issue_group::social;

			env.manager.named_issue_index.emplace(new_issue.name, new_ident);

			for (auto n : i.second.option_names) {
				const auto option_tag = new_issue.options.emplace_back();
				auto& new_option = new_issue.options[option_tag];
				new_option.id = option_tag;
				new_option.name = n;

				option_identifier new_opt_ident;
				new_opt_ident.id = new_ident.id;
				new_opt_ident.type = new_ident.type;
				new_opt_ident.option_id = option_tag;

				env.manager.named_option_index.emplace(n, new_opt_ident);
			}
		}
	};
	struct economic_issues_group {
		parsing_environment& env;

		economic_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(const std::pair<token_and_type, preparse_issue>& i) {
			const auto new_issue_tag = env.manager.unciv_issues_cotnainer.emplace_back();
			auto& new_issue = env.manager.unciv_issues_cotnainer[new_issue_tag];
			new_issue.administrative = i.second.admin;
			new_issue.next_step_only = i.second.next_step;
			new_issue.id = new_issue_tag;
			new_issue.type = issue_group::economic;
			new_issue.name = env.text_lookup(i.first.start, i.first.end);

			env.manager.economic_issues.push_back(new_issue_tag);

			issue_idenfitier new_ident;
			new_ident.id = new_issue_tag;
			new_ident.type = issue_group::economic;

			env.manager.named_issue_index.emplace(new_issue.name, new_ident);

			for (auto n : i.second.option_names) {
				const auto option_tag = new_issue.options.emplace_back();
				auto& new_option = new_issue.options[option_tag];
				new_option.id = option_tag;
				new_option.name = n;

				option_identifier new_opt_ident;
				new_opt_ident.id = new_ident.id;
				new_opt_ident.type = new_ident.type;
				new_opt_ident.option_id = option_tag;

				env.manager.named_option_index.emplace(n, new_opt_ident);
			}
		}
	};
	struct military_issues_group {
		parsing_environment& env;

		military_issues_group(parsing_environment& e) : env(e) {}

		void add_issue(const std::pair<token_and_type, preparse_issue>& i) {
			const auto new_issue_tag = env.manager.unciv_issues_cotnainer.emplace_back();
			auto& new_issue = env.manager.unciv_issues_cotnainer[new_issue_tag];
			new_issue.administrative = i.second.admin;
			new_issue.next_step_only = i.second.next_step;
			new_issue.id = new_issue_tag;
			new_issue.type = issue_group::military;
			new_issue.name = env.text_lookup(i.first.start, i.first.end);

			env.manager.military_issues.push_back(new_issue_tag);

			issue_idenfitier new_ident;
			new_ident.id = new_issue_tag;
			new_ident.type = issue_group::military;

			env.manager.named_issue_index.emplace(new_issue.name, new_ident);

			for (auto n : i.second.option_names) {
				const auto option_tag = new_issue.options.emplace_back();
				auto& new_option = new_issue.options[option_tag];
				new_option.id = option_tag;
				new_option.name = n;

				option_identifier new_opt_ident;
				new_opt_ident.id = new_ident.id;
				new_opt_ident.type = new_ident.type;
				new_opt_ident.option_id = option_tag;

				env.manager.named_option_index.emplace(n, new_opt_ident);
			}
		}
	};

	struct preparse_issues_file {
		void insert_issue_group(const military_issues_group&) {}
		void insert_issue_group(const economic_issues_group&) {}
		void insert_issue_group(const social_issues_group&) {}
		void insert_issue_group(const political_issues_group&) {}
		void insert_issue_group(const party_issues_group&) {}
	};

	std::pair<token_and_type, preparse_issue> bind_issue(const token_and_type& t, association_type, preparse_issue& f);
	token_and_type name_pre_parse_option(const token_and_type& t, association_type, empty_type&);

	token_and_type name_pre_parse_option(const token_and_type& t, association_type, empty_type&) { return t; }
	std::pair<token_and_type, preparse_issue> bind_issue(const token_and_type& t, association_type, preparse_issue& f) {
		return std::pair<token_and_type, preparse_issue>(t, std::move(f));
	}
}

MEMBER_DEF(issues::preparse_issue, admin, "administrative");
MEMBER_DEF(issues::preparse_issue, next_step, "next_step_only");
MEMBER_FDEF(issues::preparse_issue, add_option, "option");
MEMBER_FDEF(issues::party_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::political_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::social_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::economic_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::military_issues_group, add_issue, "issue");
MEMBER_FDEF(issues::preparse_issues_file, insert_issue_group, "group");

namespace issues {
	BEGIN_DOMAIN(issues_pre_parsing_domain)
		EMPTY_TYPE(empty_type)
		BEGIN_TYPE(preparse_issue)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("option", accept_all, empty_type, name_pre_parse_option)
		MEMBER_ASSOCIATION("administrative", "administrative", value_from_rh<bool>)
		MEMBER_ASSOCIATION("next_step_only", "next_step_only", value_from_rh<bool>)
		END_TYPE
		BEGIN_TYPE(party_issues_group)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("issue", accept_all, preparse_issue, bind_issue)
		END_TYPE
		BEGIN_TYPE(political_issues_group)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("issue", accept_all, preparse_issue, bind_issue)
		END_TYPE
		BEGIN_TYPE(social_issues_group)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("issue", accept_all, preparse_issue, bind_issue)
		END_TYPE
		BEGIN_TYPE(economic_issues_group)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("issue", accept_all, preparse_issue, bind_issue)
		END_TYPE
		BEGIN_TYPE(military_issues_group)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("issue", accept_all, preparse_issue, bind_issue)
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
}
