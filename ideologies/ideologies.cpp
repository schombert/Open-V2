#include "ideologies.h"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "scenario\\scenario.h"
#include "modifiers\\modifiers.h"

namespace ideologies {
	struct parsing_environment {
		text_data::text_sequences& text_lookup;
		ideologies_manager& manager;

		parsed_data main_file_parse_tree;
		boost::container::flat_map<ideology_tag, std::pair<const token_group*, const token_group*>> ideology_bodies_mapping;

		parsing_environment(text_data::text_sequences& tl, ideologies_manager& m) :
			text_lookup(tl), manager(m) {
		}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, ideologies_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {
	}
	parsing_state::~parsing_state() {}
	
	struct empty_type {
		void add_unknown_key(int) {
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

	struct single_ideology_parse_state {
		scenario::scenario_manager& s;
		ideology& under_construction;

		single_ideology_parse_state(scenario::scenario_manager& sm, ideology& i) : s(sm), under_construction(i) {}
	};

	struct ideology_builder {
		single_ideology_parse_state& env;

		ideology_builder(single_ideology_parse_state& e) : env(e) {}

		void add_color(const color_builder& c) {
			env.under_construction.color = c.color;
		}
		void set_date(date_tag d) {
			env.under_construction.enable_date = d;
		}
		void add_political_reform(modifiers::factor_tag t) {
			env.under_construction.add_political_reform = t;
		}
		void add_social_reform(modifiers::factor_tag t) {
			env.under_construction.add_social_reform = t;
		}
		void remove_political_reform(modifiers::factor_tag t) {
			env.under_construction.remove_political_reform = t;
		}
		void remove_social_reform(modifiers::factor_tag t) {
			env.under_construction.remove_social_reform = t;
		}
		void add_military_reform(modifiers::factor_tag t) {
			env.under_construction.add_military_reform = t;
		}
		void add_economic_reform(modifiers::factor_tag t) {
			env.under_construction.add_economic_reform = t;
		}
		void set_uncivilized(bool v) {
			env.under_construction.uncivilized = v;
		}
		void set_can_reduce_militancy(bool v) {
			if(v)
				env.s.ideologies_m.conservative_ideology = env.under_construction.id;
		}
	};

	struct pre_parse_ideology_group {
		parsing_environment& env;
		std::vector<ideology_tag> members;

		pre_parse_ideology_group(parsing_environment& e) : env(e) {}

		void add_ideology(ideology_tag new_ideology_tag) {
			members.push_back(new_ideology_tag);
		}
	};

	struct pre_parse_ideologies_file {
		parsing_environment& env;
		pre_parse_ideologies_file(parsing_environment& e) : env(e) {}

		void add_group(const std::pair<token_and_type, pre_parse_ideology_group>& group) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, group.first.start, group.first.end);
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

	inline std::pair<token_and_type, pre_parse_ideology_group> bind_group(const token_and_type& t, association_type, pre_parse_ideology_group&& f) {
		return std::pair<token_and_type, pre_parse_ideology_group>(t, std::move(f));
	}

	inline int discard_empty_type(const token_and_type&, association_type, const empty_type&) { return 0; }

	inline modifiers::factor_tag read_reform_factor(const token_group* s, const token_group* e, single_ideology_parse_state& env) {
		return modifiers::parse_modifier_factors(env.s, triggers::trigger_scope_state{
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::empty,
			false }, 0.0f, 1.0f, s, e);
	}

	inline ideology_tag internal_reserve_ideology(const token_group* s, const token_group* e, const token_and_type& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto new_ideology_tag = env.manager.ideology_container.emplace_back();
		auto& new_ideology = env.manager.ideology_container[new_ideology_tag];
		new_ideology.name = name;
		new_ideology.id = new_ideology_tag;
		env.manager.named_ideology_index.emplace(name, new_ideology_tag);

		env.ideology_bodies_mapping.emplace(new_ideology_tag, std::make_pair(s, e));

		return new_ideology_tag;
	}
}

MEMBER_FDEF(ideologies::pre_parse_ideologies_file, add_group, "group");
MEMBER_FDEF(ideologies::pre_parse_ideology_group, add_ideology, "ideology");
MEMBER_FDEF(ideologies::empty_type, add_unknown_key, "unknown_key");
MEMBER_FDEF(ideologies::color_builder, add_value, "color");
MEMBER_FDEF(ideologies::ideology_builder, add_color, "color");
MEMBER_FDEF(ideologies::ideology_builder, set_date, "date");
MEMBER_FDEF(ideologies::ideology_builder, add_political_reform, "add_political_reform");
MEMBER_FDEF(ideologies::ideology_builder, add_social_reform, "add_social_reform");
MEMBER_FDEF(ideologies::ideology_builder, remove_political_reform, "remove_political_reform");
MEMBER_FDEF(ideologies::ideology_builder, remove_social_reform, "remove_social_reform");
MEMBER_FDEF(ideologies::ideology_builder, add_military_reform, "add_military_reform");
MEMBER_FDEF(ideologies::ideology_builder, add_economic_reform, "add_economic_reform");
MEMBER_FDEF(ideologies::ideology_builder, set_uncivilized, "uncivilized");
MEMBER_FDEF(ideologies::ideology_builder, set_can_reduce_militancy, "can_reduce_militancy");

namespace ideologies {
	BEGIN_DOMAIN(ideologies_pre_parsing_domain)
		BEGIN_TYPE(pre_parse_ideologies_file)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("group", accept_all, pre_parse_ideology_group, bind_group)
		END_TYPE
		BEGIN_TYPE(pre_parse_ideology_group)
		MEMBER_VARIABLE_TYPE_EXTERN("ideology", accept_all, ideology_tag, internal_reserve_ideology)
		END_TYPE
	END_DOMAIN;

	
	BEGIN_DOMAIN(single_ideology_parse_domain)
		BEGIN_TYPE(color_builder)
		MEMBER_VARIABLE_ASSOCIATION("color", accept_all, value_from_lh<int>)
		END_TYPE
		BEGIN_TYPE(ideology_builder)
		MEMBER_TYPE_ASSOCIATION("color", "color", color_builder)
		MEMBER_ASSOCIATION("date", "date", value_from_rh<date_tag>)
		MEMBER_ASSOCIATION("uncivilized", "uncivilized", value_from_rh<bool>)
		MEMBER_ASSOCIATION("can_reduce_militancy", "can_reduce_militancy", value_from_rh<bool>)
		MEMBER_TYPE_EXTERN("add_political_reform", "add_political_reform", modifiers::factor_tag, read_reform_factor)
		MEMBER_TYPE_EXTERN("add_social_reform", "add_social_reform", modifiers::factor_tag, read_reform_factor)
		MEMBER_TYPE_EXTERN("remove_political_reform", "remove_political_reform", modifiers::factor_tag, read_reform_factor)
		MEMBER_TYPE_EXTERN("remove_social_reform", "remove_social_reform", modifiers::factor_tag, read_reform_factor)
		MEMBER_TYPE_EXTERN("add_military_reform", "add_military_reform", modifiers::factor_tag, read_reform_factor)
		MEMBER_TYPE_EXTERN("add_economic_reform", "add_economic_reform", modifiers::factor_tag, read_reform_factor)
		END_TYPE
	END_DOMAIN;

	parsing_state pre_parse_ideologies(
		ideologies_manager& manager,
		const directory& source_directory,
		text_data::text_sequences& text_function) {

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

	void parse_single_ideology(scenario::scenario_manager& s, ideology_tag tag, const token_group* start, const token_group* end) {
		single_ideology_parse_state env(s, s.ideologies_m.ideology_container[tag]);
		parse_object<ideology_builder, single_ideology_parse_domain>(start, end, env);
	}

	void parse_ideologies(scenario::scenario_manager& s, const parsing_state& state) {
		for(const auto& pr : state.impl->ideology_bodies_mapping) {
			parse_single_ideology(s, pr.first, pr.second.first, pr.second.second);
		}
	}
}
