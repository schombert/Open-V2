#include "ideologies.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace ideologies {
	struct parsing_environment {
		text_handle_lookup text_lookup;
		ideologies_manager& manager;

		parsed_data main_file_parse_tree;

		parsing_environment(const text_handle_lookup& tl, ideologies_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, ideologies_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}
	
	struct empty_type {

	};

	struct pre_parse_ideology_group {
		parsing_environment& env;
		std::vector<ideology_tag> members;

		pre_parse_ideology_group(parsing_environment& e) : env(e) {}

		void add_ideology(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			const auto new_ideology_tag = env.manager.ideology_container.emplace_back();
			auto& new_ideology = env.manager.ideology_container[new_ideology_tag];
			new_ideology.name = name;
			new_ideology.id = new_ideology_tag;
			env.manager.named_ideology_index.emplace(name, new_ideology_tag);
			members.push_back(new_ideology_tag);
		}
	};

	struct pre_parse_ideologies_file {
		parsing_environment& env;
		pre_parse_ideologies_file(parsing_environment& e) : env(e) {}

		void add_group(const std::pair<token_and_type, pre_parse_ideology_group>& group) {
			const auto name = env.text_lookup(group.first.start, group.first.end);
			const auto new_ideology_group_tag = env.manager.ideology_groups.emplace_back();
			auto& new_ideology_group = env.manager.ideology_groups[new_ideology_group_tag];
			new_ideology_group.name = name;
			new_ideology_group.id = new_ideology_group_tag;
			env.manager.named_group_index.emplace(name, new_ideology_group_tag);

			for (auto t : group.second.members) {
				env.manager.ideology_container[t].group = new_ideology_group_tag;
			}
		}
	};

	std::pair<token_and_type, pre_parse_ideology_group> bind_group(const token_and_type& t, association_type, pre_parse_ideology_group& f);
	token_and_type name_pre_parse_ideology(const token_and_type& t, association_type, empty_type&);

	token_and_type name_pre_parse_ideology(const token_and_type& t, association_type, empty_type&) { return t; }
	std::pair<token_and_type, pre_parse_ideology_group> bind_group(const token_and_type& t, association_type, pre_parse_ideology_group& f) {
		return std::pair<token_and_type, pre_parse_ideology_group>(t, std::move(f));
	}
}

MEMBER_FDEF(ideologies::pre_parse_ideologies_file, add_group, "group");
MEMBER_FDEF(ideologies::pre_parse_ideology_group, add_ideology, "ideology");

namespace ideologies {
	BEGIN_DOMAIN(ideologies_pre_parsing_domain)
		EMPTY_TYPE(empty_type)
		BEGIN_TYPE(pre_parse_ideologies_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("group", accept_all, pre_parse_ideology_group, bind_group)
		END_TYPE
		BEGIN_TYPE(pre_parse_ideology_group)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("ideology", accept_all, empty_type, name_pre_parse_ideology)
		END_TYPE
	END_DOMAIN;


	parsing_state pre_parse_ideologies(
		ideologies_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto file = common_dir.open_file(u"ideologies.txt");

		parsing_state return_state(text_function, manager);

		auto& main_results = return_state.impl->main_file_parse_tree;

		if (file) {
			const auto sz = file->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			file->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<pre_parse_ideologies_file, ideologies_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*return_state.impl);
			}
		}

		return return_state;
	}
}
