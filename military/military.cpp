#include "military.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"

namespace military {
	struct parsing_environment {
		text_handle_lookup text_lookup;

		military_manager& manager;

		parsed_data cb_file;
		std::vector<parsed_data> unit_type_files;

		parsing_environment(const text_handle_lookup& tl, military_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(const text_handle_lookup& tl, military_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	struct unit_file {
		parsing_environment& env;
		unit_file(parsing_environment& e) : env(e) {}
		void add_unit(const token_and_type& t) {
			const auto uname = env.text_lookup(t.start, t.end);
			const auto uid = env.manager.unit_types.emplace_back();
			env.manager.unit_types[uid].id = uid;
			env.manager.unit_types[uid].name = uname;
			env.manager.named_unit_type_index.emplace(uname, uid);
		}
	};
	struct peace_order {
		parsing_environment& env;
		peace_order(parsing_environment& e) : env(e) {}
		void add_cb(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			if (0 == env.manager.named_cb_type_index.count(name)) {
				const auto cbid = env.manager.cb_types.emplace_back();
				env.manager.cb_types[cbid].id = cbid;
				env.manager.cb_types[cbid].name = name;
				env.manager.named_cb_type_index.emplace(name, cbid);
			}
		}
	};
	struct cb_file {
		parsing_environment& env;
		cb_file(parsing_environment& e) : env(e) {}
		void accept_peace_order(const peace_order&) {}
		void add_cb(const token_and_type& t) {
			const auto name = env.text_lookup(t.start, t.end);
			if (0 == env.manager.named_cb_type_index.count(name)) {
				const auto cbid = env.manager.cb_types.emplace_back();
				env.manager.cb_types[cbid].id = cbid;
				env.manager.cb_types[cbid].name = name;
				env.manager.named_cb_type_index.emplace(name, cbid);
			}
		}
	};
	struct empty_type {
		void add_unknown_key(int) {
		}
	};
	inline token_and_type name_empty_type(const token_and_type& t, association_type, empty_type&) { return t; }
	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }
}

MEMBER_FDEF(military::cb_file, add_cb, "add_cb");
MEMBER_FDEF(military::cb_file, accept_peace_order, "peace_order");
MEMBER_FDEF(military::peace_order, add_cb, "add_cb");
MEMBER_FDEF(military::unit_file, add_unit, "add_unit");
MEMBER_FDEF(military::empty_type, add_unknown_key, "unknown_key");

namespace military {
	BEGIN_DOMAIN(unit_types_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(unit_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("add_unit", accept_all, empty_type, name_empty_type)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(cb_types_pre_parsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(cb_file)
		MEMBER_TYPE_ASSOCIATION("peace_order", "peace_order", peace_order)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("add_cb", accept_all, empty_type, name_empty_type)
		END_TYPE
		BEGIN_TYPE(peace_order)
		MEMBER_VARIABLE_ASSOCIATION("add_cb", accept_all, token_from_lh)
		END_TYPE
	END_DOMAIN;

	void pre_parse_unit_types(
		parsing_state& state,
		const directory& source_directory) {

		const auto unit_dir = source_directory.get_directory(u"\\units");
		const auto unit_files = unit_dir.list_files(u".txt");

		for (const auto& f : unit_files) {
			const auto fi = f.open_file();
			if (fi) {
				state.impl->unit_type_files.emplace_back();
				auto& main_results = state.impl->unit_type_files.back();

				const auto sz = fi->size();
				main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

				fi->read_to_buffer(main_results.parse_data.get(), sz);
				parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

				if (main_results.parse_results.size() > 0) {
					parse_object<unit_file, unit_types_pre_parsing_domain>(
						&main_results.parse_results[0],
						&main_results.parse_results[0] + main_results.parse_results.size(),
						*state.impl);
				}
			}
		}
	}

	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		auto& main_results = state.impl->cb_file;

		const auto fi = common_dir.open_file(u"cb_types.txt");

		if (fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if (main_results.parse_results.size() > 0) {
				parse_object<cb_file, cb_types_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}
}
