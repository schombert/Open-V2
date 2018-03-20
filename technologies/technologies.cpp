#include "technologies.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace technologies {
	struct parsing_environment{
		text_handle_lookup text_lookup;
		tech_file_handler file_handler;

		technologies_manager& manager;

		parsed_data main_file_parse_tree;
		std::vector<std::pair<tech_category_tag, parsed_data>> tech_files_parse_trees;

		parsing_environment(const text_handle_lookup& tl, const tech_file_handler& fh, technologies_manager& m) :
			text_lookup(tl), file_handler(fh), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, const tech_file_handler& fh, technologies_manager& m) : 
		impl(std::make_unique<parsing_environment>(tl, fh, m)) {}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct tech_file_parsing_environment {
		tech_category_tag governing_category;
		text_handle_lookup text_lookup;
		technologies_manager& manager;

		tech_file_parsing_environment(tech_category_tag gc, const text_handle_lookup& tl, technologies_manager& m) :
			governing_category(gc), text_lookup(tl), manager(m) {
		}
	};

	struct empty_type {
		void add_unknown_key(int) {
		}
	};
	
	struct folder {
		parsing_environment& env;

		std::vector<text_data::text_tag> subcategories;

		folder(parsing_environment& e) : env(e) {}

		void add_sub_category(const token_and_type& t) {
			subcategories.push_back(env.text_lookup(t.start, t.end));
		}

		void commit_sub_categories(tech_category_tag folder_tag) const {
			for (auto tt : subcategories) {
				const auto new_tag = env.manager.technology_subcategories.emplace_back();
				auto& new_subcat = env.manager.technology_subcategories[new_tag];

				new_subcat.id = new_tag;
				new_subcat.name = tt;
				new_subcat.parent = folder_tag;

				env.manager.named_subcategory_index.emplace(tt, new_tag);
			}
		}
	};

	std::pair<token_and_type, folder> bind_folder(const token_and_type& t, association_type, folder& f);
	int discard_empty_type(const token_and_type&, association_type, empty_type&);
	token_and_type name_empty_type(const token_and_type& t, association_type, empty_type&);

	struct folders {
		parsing_environment& env;

		folders(parsing_environment& e) : env(e) {}

		void add_folder(const std::pair<token_and_type, folder>& fp) {
			const auto category_name = env.text_lookup(fp.first.start, fp.first.end);
			const auto new_category_tag = env.manager.technology_categories.emplace_back();
			auto& new_category = env.manager.technology_categories[new_category_tag];

			new_category.id = new_category_tag;
			new_category.name = category_name;

			env.manager.named_category_index.emplace(category_name, new_category_tag);

			fp.second.commit_sub_categories(new_category_tag);

			env.tech_files_parse_trees.emplace_back();
			auto& file_pair = env.tech_files_parse_trees.back();
			file_pair.first = new_category_tag;

			env.file_handler(fp.first, new_category_tag, file_pair.second, env.text_lookup, env.manager);
		}
	};

	std::pair<token_and_type, folder> bind_folder(const token_and_type& t, association_type, folder& f) {
		return std::pair<token_and_type, folder>(t, std::move(f));
	}

	struct preparse_schools {
		parsing_environment& env;

		preparse_schools(parsing_environment& e) : env(e) {}

		void add_school(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			const auto new_st = env.manager.tech_schools.emplace_back();
			auto& s = env.manager.tech_schools[new_st];
			s.id = new_st;
			s.name = name;
			env.manager.named_tech_school_index.emplace(name, new_st);
		}
	};

	struct technologies_file {
		parsing_environment& env;

		technologies_file(parsing_environment& e) : env(e) {}
		void handle_folders(const folders&) {}
		void handle_schools(const preparse_schools&) {}

		void add_unknown_key(int) {
		}
	};
	
	struct specific_tech_file {
		const tech_file_parsing_environment& env;

		specific_tech_file(const tech_file_parsing_environment& e) : env(e) {}

		void insert_tech(const token_and_type& t) {
			const auto tech_name = env.text_lookup(t.start, t.end);
			const auto tech_tag = env.manager.technologies_container.emplace_back();
			auto& new_tech = env.manager.technologies_container[tech_tag];

			new_tech.id = tech_tag;
			new_tech.name = tech_name;

			env.manager.named_technology_index.emplace(tech_name, tech_tag);
		}

		void add_unknown_key(int) {
		}
	};

	int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }
	token_and_type name_empty_type(const token_and_type& t, association_type, empty_type&) { return t; }
}

MEMBER_FDEF(technologies::preparse_schools, add_school, "school");
MEMBER_FDEF(technologies::folders, add_folder, "category");
MEMBER_FDEF(technologies::folder, add_sub_category, "sub_category");
MEMBER_FDEF(technologies::technologies_file, add_unknown_key, "unknown_key");
MEMBER_FDEF(technologies::technologies_file, handle_folders, "folders");
MEMBER_FDEF(technologies::technologies_file, handle_schools, "schools");
MEMBER_FDEF(technologies::specific_tech_file, insert_tech, "technology");
MEMBER_FDEF(technologies::specific_tech_file, add_unknown_key, "unknown_key");
MEMBER_FDEF(technologies::empty_type, add_unknown_key, "unknown_key");


namespace technologies {
	BEGIN_DOMAIN(tech_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(technologies_file)
		MEMBER_TYPE_ASSOCIATION("folders", "folders", folders)
		MEMBER_TYPE_ASSOCIATION("schools", "schools", preparse_schools)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(folders)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("category", accept_all, folder, bind_folder)
		END_TYPE
		BEGIN_TYPE(folder)
		MEMBER_VARIABLE_ASSOCIATION("sub_category", accept_all, token_from_lh)
		END_TYPE
		BEGIN_TYPE(preparse_schools)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("school", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(tech_subfile_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(specific_tech_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("technology", accept_all, empty_type, name_empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		END_TYPE
	END_DOMAIN;

	void parse_single_tech_file(tech_category_tag cat, const text_handle_lookup& tl, technologies_manager& m, std::vector<token_group>& results) {
		tech_file_parsing_environment e(cat, tl, m);
		if (results.size() > 0)
			parse_object<specific_tech_file, tech_subfile_pre_parsing_domain>(&results[0], &results[0] + results.size(), e);
	}

	void pre_parse_main_technology_file(
		technologies_manager& tech_manager,
		std::vector<token_group>& parse_results,
		const text_handle_lookup& text_function,
		const tech_file_handler& file_function) {

		parsing_environment e(text_function, file_function, tech_manager);
		if (parse_results.size() > 0)
			parse_object<technologies_file, tech_pre_parsing_domain>(&parse_results[0], &parse_results[0] + parse_results.size(), e);
	}

	tech_file_handler make_subfile_perparse_handler(directory tech_directory) {
		return [dir = tech_directory](
			const token_and_type& name,
			tech_category_tag cat,
			parsed_data& results,
			const text_handle_lookup& tl,
			technologies_manager& m) {

			std::u16string folder_file(name.start, name.end);
			folder_file += u".txt";

			const auto tech_file = dir.open_file(folder_file);

			if (tech_file) {
				const auto sz = tech_file->size();
				results.parse_data = std::unique_ptr<char[]>(new char[sz]);

				tech_file->read_to_buffer(results.parse_data.get(), sz);

				parse_pdx_file(results.parse_results, results.parse_data.get(), results.parse_data.get() + sz);

				parse_single_tech_file(cat, tl, m, results.parse_results);
			}
		};
	}

	parsing_state pre_parse_technologies(
		technologies_manager& tech_manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto tech_dir = source_directory.get_directory(u"\\technologies");

		parsing_state return_state(text_function, make_subfile_perparse_handler(tech_dir), tech_manager);

		auto& main_results = return_state.impl->main_file_parse_tree;

		const auto tech_file = common_dir.open_file(u"technology.txt");

		if (tech_file) {
			const auto sz = tech_file->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			tech_file->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<technologies_file, tech_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*return_state.impl);
			}
		}

		return return_state;
	}
};
