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

	struct trait {
		float organisation = 0.0f;
		float morale = 0.0f;
		float attack = 0.0f;
		float defence = 0.0f;
		float reconnaissance = 0.0f;
		float speed = 0.0f;
		float experience = 0.0f;
		float reliability = 0.0f;
	};

	inline leader_trait_tag add_trait_to_manager(military_manager& m, const trait& t) {
		const auto current_size = m.leader_traits.size();
		const leader_trait_tag new_tag(static_cast<value_base_of<leader_trait_tag>>(current_size));

		auto row_ptr = m.leader_trait_definitions.safe_get_row(new_tag);
		row_ptr[traits::organisation] = t.organisation;
		row_ptr[traits::morale] = t.morale;
		row_ptr[traits::attack] = t.attack;
		row_ptr[traits::defence] = t.defence;
		row_ptr[traits::reconnaissance] = t.reconnaissance;
		row_ptr[traits::speed] = t.speed;
		row_ptr[traits::experience] = t.experience;
		row_ptr[traits::reliability] = t.reliability;

		return new_tag;
	}

	struct personalities {
		parsing_environment& env;
		personalities(parsing_environment& e) : env(e) {}

		void add_trait(const std::pair<token_and_type, trait>& t) {
			const auto new_trait = add_trait_to_manager(env.manager, t.second);
			const auto trait_name = env.text_lookup(t.first.start, t.first.end);
			env.manager.leader_traits.safe_get(new_trait) = trait_name;

			env.manager.personality_traits.push_back(new_trait);
			env.manager.named_leader_trait_index.emplace(trait_name, new_trait);
		}
		void add_no_personality(const trait& t) {
			const static char no_personality_string[] = "no_personality";

			const auto new_trait = add_trait_to_manager(env.manager, t);
			const auto trait_name = env.text_lookup(no_personality_string, no_personality_string + sizeof(no_personality_string) - 1);
			env.manager.leader_traits.safe_get(new_trait) = trait_name;

			env.manager.no_personality_trait = new_trait;
			env.manager.named_leader_trait_index.emplace(trait_name, new_trait);
		}
	};
	struct backgrounds {
		parsing_environment& env;
		backgrounds(parsing_environment& e) : env(e) {}

		void add_trait(const std::pair<token_and_type, trait>& t) {
			const auto new_trait = add_trait_to_manager(env.manager, t.second);
			const auto trait_name = env.text_lookup(t.first.start, t.first.end);
			env.manager.leader_traits.safe_get(new_trait) = trait_name;

			env.manager.background_traits.push_back(new_trait);
			env.manager.named_leader_trait_index.emplace(trait_name, new_trait);
		}
		void add_no_background(const trait& t) {
			const static char no_background_string[] = "no_background";

			const auto new_trait = add_trait_to_manager(env.manager, t);
			const auto trait_name = env.text_lookup(no_background_string, no_background_string + sizeof(no_background_string) - 1);
			env.manager.leader_traits.safe_get(new_trait) = trait_name;

			env.manager.no_background_trait = new_trait;
			env.manager.named_leader_trait_index.emplace(trait_name, new_trait);
		}
	};

	struct traits_file {
		parsing_environment& env;
		traits_file(parsing_environment& e) : env(e) {}

		void add_personalities(const personalities&) {}
		void add_backgrounds(const backgrounds&) {}
	};

	inline std::pair<token_and_type, trait> name_trait(const token_and_type& t, association_type, trait& r) {
		return std::pair<token_and_type, trait>(t, r);
	}
}

MEMBER_FDEF(military::cb_file, add_cb, "add_cb");
MEMBER_FDEF(military::cb_file, accept_peace_order, "peace_order");
MEMBER_FDEF(military::peace_order, add_cb, "add_cb");
MEMBER_FDEF(military::unit_file, add_unit, "add_unit");
MEMBER_FDEF(military::empty_type, add_unknown_key, "unknown_key");
MEMBER_DEF(military::trait, organisation, "organisation");
MEMBER_DEF(military::trait, morale, "morale");
MEMBER_DEF(military::trait, attack, "attack");
MEMBER_DEF(military::trait, defence, "defence");
MEMBER_DEF(military::trait, reconnaissance, "reconnaissance");
MEMBER_DEF(military::trait, speed, "speed");
MEMBER_DEF(military::trait, experience, "experience");
MEMBER_DEF(military::trait, reliability, "reliability");
MEMBER_FDEF(military::personalities, add_trait, "add_trait");
MEMBER_FDEF(military::personalities, add_no_personality, "no_personality");
MEMBER_FDEF(military::backgrounds, add_trait, "add_trait");
MEMBER_FDEF(military::backgrounds, add_no_background, "no_background");
MEMBER_FDEF(military::traits_file, add_personalities, "personality");
MEMBER_FDEF(military::traits_file, add_backgrounds, "background");


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

	BEGIN_DOMAIN(traits_parsing_domain)
		BEGIN_TYPE(traits_file)
		MEMBER_TYPE_ASSOCIATION("personality", "personality", personalities)
		MEMBER_TYPE_ASSOCIATION("background", "background", backgrounds)
		END_TYPE
		BEGIN_TYPE(personalities)
		MEMBER_TYPE_ASSOCIATION("no_personality", "no_personality", trait)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("add_trait", accept_all, trait, name_trait)
		END_TYPE
		BEGIN_TYPE(backgrounds)
		MEMBER_TYPE_ASSOCIATION("no_background", "no_background", trait)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("add_trait", accept_all, trait, name_trait)
		END_TYPE
		BEGIN_TYPE(trait)
		MEMBER_ASSOCIATION("organisation", "organisation", value_from_rh<traits::value_type>)
		MEMBER_ASSOCIATION("morale", "morale", value_from_rh<traits::value_type>)
		MEMBER_ASSOCIATION("attack", "attack", value_from_rh<traits::value_type>)
		MEMBER_ASSOCIATION("defence", "defence", value_from_rh<traits::value_type>)
		MEMBER_ASSOCIATION("reconnaissance", "reconnaissance", value_from_rh<traits::value_type>)
		MEMBER_ASSOCIATION("speed", "speed", value_from_rh<traits::value_type>)
		MEMBER_ASSOCIATION("experience", "experience", value_from_rh<traits::value_type>)
		MEMBER_ASSOCIATION("reliability", "reliability", value_from_rh<traits::value_type>)
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

	void parse_leader_traits(parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		const auto fi = common_dir.open_file(u"traits.txt");

		if (fi) {
			const auto sz = fi->size();
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);
			std::vector<token_group> parse_results;
			fi->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if (parse_results.size() > 0) {
				parse_object<traits_file, traits_parsing_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					*state.impl);
			}
		}
	}
}
