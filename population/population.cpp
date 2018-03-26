#include "population.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace population {
	struct parsing_environment {
		text_handle_lookup text_lookup;

		population_manager& manager;

		parsed_data rebel_types_file;

		parsing_environment(const text_handle_lookup& tl, population_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, population_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct rebel_types_pre_parse_file {
		parsing_environment& env;
		rebel_types_pre_parse_file(parsing_environment& e) : env(e) {}

		void add_rebel_type(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			const auto rtag = env.manager.rebel_types.emplace_back();
			auto& reb = env.manager.rebel_types[rtag];
			reb.id = rtag;
			reb.name = name;
			env.manager.named_rebel_type_index.emplace(name, rtag);
		}
	};

	struct empty_type {
		void add_unknown_key(int) {
		}
	};
	inline token_and_type name_empty_type(const token_and_type& t, association_type, empty_type&) { return t; }
	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }
}

MEMBER_FDEF(population::rebel_types_pre_parse_file, add_rebel_type, "add_rebel_type");
MEMBER_FDEF(population::empty_type, add_unknown_key, "unknown_key");

namespace population {
	BEGIN_DOMAIN(rebel_types_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(rebel_types_pre_parse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("add_rebel_type", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	void pre_parse_pop_types(
		population_manager& manager,
		const directory& source_directory,
		const text_handle_lookup& text_function) {

		const auto poptype_dir = source_directory.get_directory(u"\\poptypes");
		const auto poptype_files = poptype_dir.list_files(u".txt");

		for (const auto& file : poptype_files) {
			const auto fname = file.file_name();
			const auto clipped_unicode = fname.size() >= 4ui64 ? std::string(fname.begin(), fname.end() - 4ui32) : std::string("");
			const auto name_tag = text_function(clipped_unicode.c_str(), clipped_unicode.c_str() + clipped_unicode.size());

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
}
