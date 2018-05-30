#include "technologies.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "modifiers\\modifiers.h"
#include "simple_fs\\simple_fs.h"

namespace technologies {
	struct parsing_environment{
		text_data::text_sequences& text_lookup;
		const directory& tech_directory;
		technologies_manager& manager;
		modifiers::modifiers_manager& mod_manager;

		parsed_data main_file_parse_tree;
		std::vector<std::pair<tech_category_tag, parsed_data>> tech_files_parse_trees;
		std::vector<parsed_data> inventions_parse_trees;

		parsing_environment(text_data::text_sequences& tl, const directory& tech_root, technologies_manager& m, modifiers::modifiers_manager& mm) :
			text_lookup(tl), tech_directory(tech_root), manager(m), mod_manager(mm) {
		}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, const directory& tech_directory, technologies_manager& m, modifiers::modifiers_manager& mm) :
		impl(std::make_unique<parsing_environment>(tl, tech_directory, m, mm)) {}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct tech_file_parsing_environment {
		tech_category_tag governing_category;
		text_data::text_sequences& text_lookup;
		technologies_manager& manager;

		tech_file_parsing_environment(tech_category_tag gc, text_data::text_sequences& tl, technologies_manager& m) :
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
			subcategories.push_back(text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end));
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

	struct folders {
		parsing_environment& env;

		folders(parsing_environment& e) : env(e) {}

		void add_folder(const std::pair<token_and_type, folder>& fp) {
			const auto category_name = text_data::get_thread_safe_text_handle(env.text_lookup, fp.first.start, fp.first.end);
			const auto new_category_tag = env.manager.technology_categories.emplace_back();
			auto& new_category = env.manager.technology_categories[new_category_tag];

			new_category.id = new_category_tag;
			new_category.name = category_name;

			env.manager.named_category_index.emplace(category_name, new_category_tag);

			fp.second.commit_sub_categories(new_category_tag);

			env.tech_files_parse_trees.emplace_back();
			auto& file_pair = env.tech_files_parse_trees.back();
			file_pair.first = new_category_tag;
			auto& result = file_pair.second;

			//read folder file

			std::u16string folder_file(fp.first.start, fp.first.end);
			folder_file += u".txt";

			const auto tech_file = env.tech_directory.open_file(folder_file);

			if(tech_file) {
				const auto sz = tech_file->size();
				result.parse_data = std::unique_ptr<char[]>(new char[sz]);

				tech_file->read_to_buffer(result.parse_data.get(), sz);
				parse_pdx_file(file_pair.second.parse_results, result.parse_data.get(), result.parse_data.get() + sz);

				if(result.parse_results.size() != 0)
					pre_parse_single_tech_file(new_category_tag, env.text_lookup, env.manager, result.parse_results.data(), result.parse_results.data() + result.parse_results.size());
			}
		}
	};

	inline std::pair<token_and_type, folder> bind_folder(const token_and_type& t, association_type, folder& f) {
		return std::pair<token_and_type, folder>(t, std::move(f));
	}

	struct parse_schools {
		parsing_environment& env;

		parse_schools(parsing_environment& e) : env(e) {}

		void add_school(const std::pair<text_data::text_tag, modifiers::national_modifier_tag>& p) {
			env.manager.named_tech_school_index.emplace(p.first, p.second);
		}
	};

	inline std::pair<text_data::text_tag, modifiers::national_modifier_tag> read_school_modifier(const token_group* start, const token_group* end, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		return std::make_pair(name, modifiers::parse_national_modifier(name, env.mod_manager, start, end));
	}

	struct technologies_file {
		parsing_environment& env;

		technologies_file(parsing_environment& e) : env(e) {}
		void handle_folders(const folders&) {}
		void handle_schools(const parse_schools&) {}

		void add_unknown_key(int) {
		}
	};
	
	struct specific_tech_file {
		const tech_file_parsing_environment& env;

		specific_tech_file(const tech_file_parsing_environment& e) : env(e) {}

		void insert_tech(const token_and_type& t) {
			const auto tech_name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
			const auto tag = env.manager.technologies_container.emplace_back();
			auto& new_tech = env.manager.technologies_container[tag];

			new_tech.id = tag;
			new_tech.name = tech_name;

			env.manager.named_technology_index.emplace(tech_name, tag);
		}

		void add_unknown_key(int) {
		}
	};

	struct inventions_pre_parse_file {
		parsing_environment& env;

		inventions_pre_parse_file(parsing_environment& e) : env(e) {}

		void add_invention(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
			const auto tag = env.manager.inventions.emplace_back();
			auto& new_i = env.manager.inventions[tag];

			new_i.id = tag;
			new_i.name = name;

			env.manager.named_invention_index.emplace(name, tag);
		}
	};


	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }
	inline token_and_type name_empty_type(const token_and_type& t, association_type, empty_type&) { return t; }
}

MEMBER_FDEF(technologies::parse_schools, add_school, "school");
MEMBER_FDEF(technologies::folders, add_folder, "category");
MEMBER_FDEF(technologies::folder, add_sub_category, "sub_category");
MEMBER_FDEF(technologies::technologies_file, add_unknown_key, "unknown_key");
MEMBER_FDEF(technologies::technologies_file, handle_folders, "folders");
MEMBER_FDEF(technologies::technologies_file, handle_schools, "schools");
MEMBER_FDEF(technologies::specific_tech_file, insert_tech, "technology");
MEMBER_FDEF(technologies::specific_tech_file, add_unknown_key, "unknown_key");
MEMBER_FDEF(technologies::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(technologies::inventions_pre_parse_file, add_invention, "invention");


namespace technologies {
	using name_mod_pair = std::pair<text_data::text_tag, modifiers::national_modifier_tag>;

	BEGIN_DOMAIN(tech_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(technologies_file)
		MEMBER_TYPE_ASSOCIATION("folders", "folders", folders)
		MEMBER_TYPE_ASSOCIATION("schools", "schools", parse_schools)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(folders)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("category", accept_all, folder, bind_folder)
		END_TYPE
		BEGIN_TYPE(folder)
		MEMBER_VARIABLE_ASSOCIATION("sub_category", accept_all, token_from_lh)
		END_TYPE
		BEGIN_TYPE(parse_schools)
		MEMBER_VARIABLE_TYPE_EXTERN("school", accept_all, name_mod_pair, read_school_modifier)
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

	BEGIN_DOMAIN(inventions_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(inventions_pre_parse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("invention", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	void pre_parse_single_tech_file(tech_category_tag cat, text_data::text_sequences& tl, technologies_manager& m, const token_group* start, const token_group* end) {
		tech_file_parsing_environment e(cat, tl, m);
		parse_object<specific_tech_file, tech_subfile_pre_parsing_domain>(start, end, e);
	}

	void parse_main_technology_file(
		technologies_manager& tech_manager,
		std::vector<token_group>& parse_results,
		text_data::text_sequences& text_function,
		const directory& tech_root,
		modifiers::modifiers_manager& mod_manager) {

		parsing_environment e(text_function, tech_root, tech_manager, mod_manager);
		if (parse_results.size() > 0)
			parse_object<technologies_file, tech_parsing_domain>(&parse_results[0], &parse_results[0] + parse_results.size(), e);
	}

	void parse_technologies(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto tech_dir = source_directory.get_directory(u"\\technologies");

		auto& main_results = state.impl->main_file_parse_tree;

		const auto tech_file = common_dir.open_file(u"technology.txt");

		if (tech_file) {
			const auto sz = tech_file->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			tech_file->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<technologies_file, tech_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void pre_parse_inventions(
		parsing_state& state,
		const directory& source_directory) {

		const auto dir = source_directory.get_directory(u"\\inventions");
		const auto files = dir.list_files(u".txt");
		for (const auto& f : files) {
			const auto fi = f.open_file();
			if (fi) {
				state.impl->inventions_parse_trees.emplace_back();
				auto& iparse = state.impl->inventions_parse_trees.back();
				
				const auto sz = fi->size();
				iparse.parse_data = std::unique_ptr<char[]>(new char[sz]);

				fi->read_to_buffer(iparse.parse_data.get(), sz);
				parse_pdx_file(iparse.parse_results, iparse.parse_data.get(), iparse.parse_data.get() + sz);

				if (iparse.parse_results.size() > 0) {
					parse_object<inventions_pre_parse_file, inventions_pre_parsing_domain>(
						&iparse.parse_results[0],
						&iparse.parse_results[0] + iparse.parse_results.size(),
						*state.impl);
				}
			}
		}
	}
};
