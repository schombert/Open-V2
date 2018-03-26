#include "modifiers.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace modifiers {
	struct parsing_environment {
		text_handle_lookup text_lookup;

		modifiers_manager& manager;

		parsed_data crimes_file;
		parsed_data nv_file;

		parsing_environment(const text_handle_lookup& tl, modifiers_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, modifiers_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct empty_type {
		void add_unknown_key(int) {
		}
	};

	struct crimes_preparse_file {
		parsing_environment& env;
		crimes_preparse_file(parsing_environment& e) : env(e) {}

		void add_crime(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			const auto tag = env.manager.provincial_modifiers.emplace_back();
			auto& new_i = env.manager.provincial_modifiers[tag];

			new_i.id = tag;
			new_i.name = name;

			env.manager.named_provincial_modifiers_index.emplace(name, tag);
		}
	};

	struct nv_preparse_file {
		parsing_environment& env;
		nv_preparse_file(parsing_environment& e) : env(e) {}

		void add_nv(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			const auto tag = env.manager.national_modifiers.emplace_back();
			auto& new_i = env.manager.national_modifiers[tag];

			new_i.id = tag;
			new_i.name = name;

			env.manager.named_national_modifiers_index.emplace(name, tag);
		}
	};

	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }
	inline token_and_type name_empty_type(const token_and_type& t, association_type, empty_type&) { return t; }
}

MEMBER_FDEF(modifiers::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(modifiers::crimes_preparse_file, add_crime, "crime");
MEMBER_FDEF(modifiers::nv_preparse_file, add_nv, "national_value");

namespace modifiers {
	BEGIN_DOMAIN(crimes_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(crimes_preparse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("crime", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(national_value_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(nv_preparse_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("national_value", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	void pre_parse_crimes(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		auto& main_results = state.impl->crimes_file;
		const auto fi = common_dir.open_file(u"crime.txt");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<crimes_preparse_file, crimes_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void pre_parse_national_values(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		auto& main_results = state.impl->nv_file;
		const auto fi = common_dir.open_file(u"nationalvalues.txt");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<nv_preparse_file, national_value_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}
}
