#include "events.h"
#include "object_parsing\\object_parsing.hpp"
#include "scenario\\scenario.h"
#include "triggers\\effect_reading.h"
#include "triggers\\trigger_reading.h"
#include "graphics\\texture.h"
#include "simple_fs\\simple_fs.h"
#include "Parsers\\parsers.hpp"

namespace events {
	event_tag event_creation_manager::register_triggered_event(event_manager& m, int32_t event_id, triggers::trigger_scope_state scope) {
		const auto created_find_result = created_triggered_events.find(id_scope_pair{ event_id, scope });
		if (created_find_result != created_triggered_events.end())
			return created_find_result->second;

		const auto pending_find_result = pending_triggered_events.find(id_scope_pair{ event_id, scope });
		if (pending_find_result != pending_triggered_events.end())
			return pending_find_result->second;

		const auto new_event_tag = m.event_container.emplace_back();
		pending_triggered_events.emplace(id_scope_pair{ event_id, scope }, new_event_tag);
		return new_event_tag;
	}

	event_tag parse_or_defer_event(scenario::scenario_manager& s, graphics::texture_manager& t, event_creation_manager& ecm, const directory& pictures_root, const triggers::trigger_scope_state& scope, const token_group* start, const token_group* end) {
		const auto[event_id, is_triggered] = pre_parse_event(start, end);
		ecm.event_sources.emplace(event_id, token_group_range{start, end});

		if(!is_triggered) {
			const auto ev = parse_single_event(s, t, ecm, pictures_root, scope, start, end);
			s.event_m.events_by_id.emplace(event_id, ev);
			return ev;
		} else {
			return event_tag();
		}
	}

	struct event_preparse {
		int32_t id = -1;
		bool is_triggered_only = false;
		void discard(int) {}
	};

#ifdef _DEBUG
	struct too_many_event_options {};
	struct unnamed_event_option {};
#endif

	struct event_parse_env {
		scenario::scenario_manager& s;
		graphics::texture_manager& tm;
		event_creation_manager& ecm;
		const directory& pictures_root;
		const triggers::trigger_scope_state scope;

		event_parse_env(scenario::scenario_manager& sc, graphics::texture_manager& t, event_creation_manager& e, const directory& root, const triggers::trigger_scope_state& so) :
			s(sc), tm(t), ecm(e), pictures_root(root), scope(so) {}
	};

	struct event_file_parse_env {
		scenario::scenario_manager& s;
		graphics::texture_manager& tm;
		event_creation_manager& ecm;
		const directory& pictures_root;

		event_file_parse_env(scenario::scenario_manager& sc, graphics::texture_manager& t, event_creation_manager& e, const directory& root) :
			s(sc), tm(t), ecm(e), pictures_root(root) {}
	};

	struct event_reader {
		event_parse_env& env;
		event_reader(event_parse_env& e) : env(e) {}

		event under_construction;
		int32_t options_count = 0;

		void set_fire_only_once(bool v) {
			if(v)
				under_construction.flags |= event::fire_only_once;
		}
		void set_major(bool v) {
			if(v)
				under_construction.flags |= event::is_major;
		}
		void set_tile(const token_and_type& t) {
			under_construction.title = text_data::get_thread_safe_text_handle(env.s.text_m, t.start, t.end);
		}
		void set_desc(const token_and_type& t) {
			under_construction.body = text_data::get_thread_safe_text_handle(env.s.text_m, t.start, t.end);
		}
		void set_picture(const token_and_type& t) {
			std::string name_with_ext = std::string("\\gfx\\pictures\\events\\") + std::string(t.start, t.end) + ".tga";
			under_construction.picture = env.tm.retrieve_by_name(env.pictures_root, name_with_ext.c_str(), name_with_ext.c_str() + name_with_ext.length());
		}
		void set_mtth(const modifiers::factor_tag mtth) {
			under_construction.mean_time_to_happen = mtth;
		}
		void set_immediate(triggers::effect_tag e) {
			under_construction.immediate_effect = e;
		}
		void set_trigger(triggers::trigger_tag t) {
			under_construction.trigger = t;
		}
		void set_option(const triggers::raw_event_option& opt) {
			if(options_count <= 5) {
				under_construction.options[options_count].ai_chance = opt.ai_chance;
				under_construction.options[options_count].effect = opt.effect;
				under_construction.options[options_count].name = opt.name;
#ifdef _DEBUG
				if(!is_valid_index(opt.name))
					throw unnamed_event_option();
#endif
				++options_count;
			} else {
#ifdef _DEBUG
				throw too_many_event_options();
#endif
			}
		}

		void discard(int) {}
	};

	struct event_file {
		event_file_parse_env& env;
		event_file(event_file_parse_env& e) : env(e) {}

		void add_province_event(const event_tag e) {
			if(is_valid_index(e))
				env.s.event_m.province_events.emplace_back(e);
		}
		void add_country_event(const event_tag e) {
			if(is_valid_index(e))
				env.s.event_m.country_events.emplace_back(e);
		}
	};

	struct issue_group_reader {
		token_and_type issue_group;
		void discard(int) {}
	};

	struct empty_type {
		void discard(int) {}
	};

	inline int discard_empty_type(const token_and_type&, association_type, empty_type&) { return 0; }

	inline triggers::effect_tag read_immediate_effect(const token_group* s, const token_group* e, event_parse_env& env) {
		const auto effect_data = triggers::parse_effect(env.s, env.ecm, env.scope, s, e);
		return triggers::commit_effect(env.s.trigger_m, effect_data);
	}
	inline triggers::trigger_tag read_trigger(const token_group* s, const token_group* e, event_parse_env& env) {
		const auto trigger_data = triggers::parse_trigger(env.s, env.scope, s, e);
		return triggers::commit_trigger(env.s.trigger_m, trigger_data);
	}
	inline triggers::raw_event_option read_option(const token_group* s, const token_group* e, event_parse_env& env) {
		return triggers::parse_option_effect(env.s, env.ecm, env.scope, s, e);
	}
	inline modifiers::factor_tag read_mtth(const token_group* s, const token_group* e, event_parse_env& env) {
		return modifiers::parse_modifier_factors(env.s, env.scope, 1.0f, 0.0f, s, e);
	}
	inline event_tag read_province_event(const token_group* s, const token_group* e, event_file_parse_env& env) {
		return parse_or_defer_event(
			env.s,
			env.tm,
			env.ecm,
			env.pictures_root,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::province,
				triggers::trigger_slot_contents::empty,
				false },
			s, e);
	}
	inline event_tag read_country_event(const token_group* s, const token_group* e, event_file_parse_env& env) {
		return parse_or_defer_event(
			env.s,
			env.tm,
			env.ecm,
			env.pictures_root,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::empty,
				false },
				s, e);
	}

	struct on_actions_env {
		scenario::scenario_manager& s;
		event_creation_manager& ecm;

		on_actions_env(scenario::scenario_manager& sc, event_creation_manager& e) :
			s(sc), ecm(e) {}
	};

	struct on_crisis_declare_interest_s {
		on_actions_env& env;
		on_crisis_declare_interest_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_crisis_declare_interest.emplace_back(tag, p.first);
		}
	};
	struct on_my_factories_nationalized_s {
		on_actions_env& env;
		on_my_factories_nationalized_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					false
				});
			env.s.event_m.on_my_factories_nationalized.emplace_back(tag, p.first);
		}
	};
	struct on_civilize_s {
		on_actions_env& env;
		on_civilize_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_civilize.emplace_back(tag, p.first);
		}
	};
	struct on_debtor_default_second_s {
		on_actions_env& env;
		on_debtor_default_second_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					false
				});
			env.s.event_m.on_debtor_default_second.emplace_back(tag, p.first);
		}
	};
	struct on_debtor_default_small_s {
		on_actions_env& env;
		on_debtor_default_small_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					false
				});
			env.s.event_m.on_debtor_default_small.emplace_back(tag, p.first);
		}
	};
	struct on_debtor_default_s {
		on_actions_env& env;
		on_debtor_default_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					false
				});
			env.s.event_m.on_debtor_default.emplace_back(tag, p.first);
		}
	};
	struct on_battle_lost_s {
		on_actions_env& env;
		on_battle_lost_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::province,
					triggers::trigger_slot_contents::province,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_battle_lost.emplace_back(tag, p.first);
		}
	};
	struct on_battle_won_s {
		on_actions_env& env;
		on_battle_won_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::province,
					triggers::trigger_slot_contents::province,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_battle_won.emplace_back(tag, p.first);
		}
	};
	struct on_colony_to_state_free_slaves_s {
		on_actions_env& env;
		on_colony_to_state_free_slaves_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::state,
					triggers::trigger_slot_contents::state,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_colony_to_state_free_slaves.emplace_back(tag, p.first);
		}
	};
	struct on_state_conquest_s {
		on_actions_env& env;
		on_state_conquest_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::state,
					triggers::trigger_slot_contents::state,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_state_conquest.emplace_back(tag, p.first);
		}
	};
	struct on_colony_to_state_s {
		on_actions_env& env;
		on_colony_to_state_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::state,
					triggers::trigger_slot_contents::state,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_colony_to_state.emplace_back(tag, p.first);
		}
	};
	struct on_election_tick_s {
		on_actions_env& env;
		on_election_tick_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::empty,
					false
				});
			text_data::text_tag tt;
			if(auto f = env.ecm.event_sources.find(p.second); f != env.ecm.event_sources.end()) {
				const auto t = get_issue_group_for_event(f->second.start, f->second.end);
				tt = text_data::get_thread_safe_text_handle(env.s.text_m, t.start, t.end);
			}
			env.s.event_m.on_election_tick.emplace_back(tag, p.first, tt);
		}
	};
	struct on_new_great_nation_s {
		on_actions_env& env;
		on_new_great_nation_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_new_great_nation.emplace_back(tag, p.first);
		}
	};
	struct on_lost_great_nation_s {
		on_actions_env& env;
		on_lost_great_nation_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_lost_great_nation.emplace_back(tag, p.first);
		}
	};
	struct on_yearly_pulse_s {
		on_actions_env& env;
		on_yearly_pulse_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_yearly_pulse.emplace_back(tag, p.first);
		}
	};
	struct on_quarterly_pulse_s {
		on_actions_env& env;
		on_quarterly_pulse_s(on_actions_env& e) : env(e) {}
		void add_pair(const std::pair<uint16_t, int32_t>& p) {
			const auto tag = env.ecm.register_triggered_event(
				env.s.event_m,
				p.second,
				triggers::trigger_scope_state{
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::nation,
					triggers::trigger_slot_contents::empty,
					false
				});
			env.s.event_m.on_quarterly_pulse.emplace_back(tag, p.first);
		}
	};
	struct on_actions_file {
		on_actions_env& env;
		on_actions_file(on_actions_env& e) : env(e) {}

		template<typename T>
		void add_sub_category(const T&) {}
	};

	struct decision_reader {
		event_file_parse_env& env;
		decision under_construction;

		decision_reader(event_file_parse_env& e) : env(e) { }

		void set_allow(triggers::trigger_tag t) {
			under_construction.allow = t;
		}
		void set_potential(triggers::trigger_tag t) {
			under_construction.potential = t;
		}
		void set_effect(triggers::effect_tag t) {
			under_construction.effect = t;
		}
		void set_picture(const token_and_type& t) {
			std::string name_with_ext = std::string("\\gfx\\pictures\\decisions\\") + std::string(t.start, t.end) + ".tga";
			under_construction.picture = env.tm.retrieve_by_name(env.pictures_root, name_with_ext.c_str(), name_with_ext.c_str() + name_with_ext.length());
		}
		void set_ai_will_do(modifiers::factor_tag m) {
			under_construction.ai_will_do = m;
		}
		void discard(int) {}
	};
	struct decision_class {
		event_file_parse_env& env;
		decision_class(event_file_parse_env& e) : env(e) {}
		void discard(int) {}
	};
	struct decision_file {
		event_file_parse_env& env;
		decision_file(event_file_parse_env& e) : env(e) {}
		void add_class(const decision_class&) {}
	};

	inline triggers::effect_tag read_decision_effect(const token_group* s, const token_group* e, event_file_parse_env& env) {
		const auto effect_data = triggers::parse_effect( env.s, env.ecm, triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::empty,
				false}, s, e);
		return triggers::commit_effect(env.s.trigger_m, effect_data);
	}
	inline triggers::trigger_tag read_decision_trigger(const token_group* s, const token_group* e, event_file_parse_env& env) {
		const auto trigger_data = triggers::parse_trigger(env.s, triggers::trigger_scope_state{
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::empty,
			false }, s, e);
		return triggers::commit_trigger(env.s.trigger_m, trigger_data);
	}
	inline modifiers::factor_tag read_ai_will_do(const token_group* s, const token_group* e, event_file_parse_env& env) {
		return modifiers::parse_modifier_factors(env.s, triggers::trigger_scope_state{
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::nation,
			triggers::trigger_slot_contents::empty,
			false }, 1.0f, 0.0f, s, e);
	}
	inline int inner_read_decision(const token_group* s, const token_group* e, const token_and_type& t, event_file_parse_env& env) {
		const auto tag = parse_decision(env.s, env.ecm, env.tm, env.pictures_root, s, e);
		decision& new_decision = env.s.event_m.decision_container[tag];
		
		std::string token(t.start, t.end);
		std::string s_title = token + "_title";
		std::string s_desc = token + "_desc";
		new_decision.title = text_data::get_thread_safe_text_handle(env.s.text_m, s_title.c_str(), s_title.c_str() + s_title.length());
		new_decision.body = text_data::get_thread_safe_text_handle(env.s.text_m, s_desc.c_str(), s_desc.c_str() + s_desc.length());
		return 0;
	}
}

MEMBER_FDEF(events::decision_reader, set_allow, "allow");
MEMBER_FDEF(events::decision_reader, discard, "discard");
MEMBER_FDEF(events::decision_reader, set_potential, "potential");
MEMBER_FDEF(events::decision_reader, set_effect, "effect");
MEMBER_FDEF(events::decision_reader, set_picture, "picture");
MEMBER_FDEF(events::decision_reader, set_ai_will_do, "ai_will_do");

MEMBER_FDEF(events::decision_class, discard, "discard");
MEMBER_FDEF(events::decision_file, add_class, "class");

MEMBER_FDEF(events::empty_type, discard, "unknown_key");
MEMBER_DEF(events::event_preparse, id, "id");
MEMBER_DEF(events::event_preparse, is_triggered_only, "is_triggered_only");
MEMBER_FDEF(events::event_preparse, discard, "discard");

MEMBER_FDEF(events::event_reader, set_fire_only_once, "fire_only_once");
MEMBER_FDEF(events::event_reader, set_major, "major");
MEMBER_FDEF(events::event_reader, discard, "discard");
MEMBER_FDEF(events::event_reader, set_tile, "title");
MEMBER_FDEF(events::event_reader, set_desc, "desc");
MEMBER_FDEF(events::event_reader, set_mtth, "mean_time_to_happen");
MEMBER_FDEF(events::event_reader, set_immediate, "immediate");
MEMBER_FDEF(events::event_reader, set_trigger, "trigger");
MEMBER_FDEF(events::event_reader, set_picture, "picture");
MEMBER_FDEF(events::event_reader, set_option, "option");

MEMBER_FDEF(events::event_file, add_province_event, "province_event");
MEMBER_FDEF(events::event_file, add_country_event, "country_event");

MEMBER_DEF(events::issue_group_reader, issue_group, "issue_group");
MEMBER_FDEF(events::issue_group_reader, discard, "unknown_key");

MEMBER_FDEF(events::on_actions_file, add_sub_category, "sub_category");
MEMBER_FDEF(events::on_yearly_pulse_s, add_pair, "item");
MEMBER_FDEF(events::on_quarterly_pulse_s, add_pair, "item");
MEMBER_FDEF(events::on_new_great_nation_s, add_pair, "item");
MEMBER_FDEF(events::on_lost_great_nation_s, add_pair, "item");
MEMBER_FDEF(events::on_election_tick_s, add_pair, "item");
MEMBER_FDEF(events::on_colony_to_state_s, add_pair, "item");
MEMBER_FDEF(events::on_state_conquest_s, add_pair, "item");
MEMBER_FDEF(events::on_colony_to_state_free_slaves_s, add_pair, "item");
MEMBER_FDEF(events::on_battle_won_s, add_pair, "item");
MEMBER_FDEF(events::on_battle_lost_s, add_pair, "item");
MEMBER_FDEF(events::on_debtor_default_s, add_pair, "item");
MEMBER_FDEF(events::on_debtor_default_small_s, add_pair, "item");
MEMBER_FDEF(events::on_debtor_default_second_s, add_pair, "item");
MEMBER_FDEF(events::on_civilize_s, add_pair, "item");
MEMBER_FDEF(events::on_my_factories_nationalized_s, add_pair, "item");
MEMBER_FDEF(events::on_crisis_declare_interest_s, add_pair, "item");


namespace events {
	BEGIN_DOMAIN(single_decision_parse_domain)
		BEGIN_TYPE(decision_reader)
		MEMBER_TYPE_EXTERN("allow", "allow", triggers::trigger_tag, read_decision_trigger)
		MEMBER_TYPE_EXTERN("potential", "potential", triggers::trigger_tag, read_decision_trigger)
		MEMBER_TYPE_EXTERN("effect", "effect", triggers::effect_tag, read_decision_effect)
		MEMBER_TYPE_EXTERN("ai_will_do", "ai_will_do", modifiers::factor_tag, read_ai_will_do)
		MEMBER_ASSOCIATION("picture", "picture", token_from_rh)
		MEMBER_ASSOCIATION("discard", "news", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news_desc_long", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news_desc_medium", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news_desc_short", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news_title", discard_from_rh)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(decision_file_domain)
		BEGIN_TYPE(decision_file)
		MEMBER_TYPE_ASSOCIATION("class", "political_decisions", decision_class)
		MEMBER_TYPE_ASSOCIATION("class", "diplomatic_decisions", decision_class)
		END_TYPE
		BEGIN_TYPE(decision_class)
		MEMBER_VARIABLE_TYPE_EXTERN("discard", accept_all, int, inner_read_decision)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(single_event_preparsing_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(event_preparse)
		MEMBER_ASSOCIATION("id", "id", value_from_rh<int32_t>)
		MEMBER_ASSOCIATION("is_triggered_only", "is_triggered_only", value_from_rh<bool>)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("discard", accept_all, empty_type, discard_empty_type)
		MEMBER_VARIABLE_ASSOCIATION("discard", accept_all, discard_from_full)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(get_issue_group_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(issue_group_reader)
		MEMBER_ASSOCIATION("issue_group", "issue_group", token_from_rh)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(single_event_domain)
		BEGIN_TYPE(event_reader)
		MEMBER_ASSOCIATION("discard", "id", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "is_triggered_only", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "election", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "issue_group", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "allow_multiple_instances", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news_desc_long", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news_desc_medium", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news_desc_short", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "news", discard_from_rh)
		MEMBER_ASSOCIATION("title", "title", token_from_rh)
		MEMBER_ASSOCIATION("desc", "desc", token_from_rh)
		MEMBER_ASSOCIATION("fire_only_once", "fire_only_once", value_from_rh<bool>)
		MEMBER_ASSOCIATION("major", "major", value_from_rh<bool>)
		MEMBER_ASSOCIATION("picture", "picture", token_from_rh)
		MEMBER_TYPE_EXTERN("immediate", "immediate", triggers::effect_tag, read_immediate_effect)
		MEMBER_TYPE_EXTERN("trigger", "trigger", triggers::trigger_tag, read_trigger)
		MEMBER_TYPE_EXTERN("option", "option", triggers::raw_event_option, read_option)
		MEMBER_TYPE_EXTERN("mean_time_to_happen", "mean_time_to_happen", modifiers::factor_tag, read_mtth)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(event_file_domain)
		BEGIN_TYPE(event_file)
		MEMBER_TYPE_EXTERN("province_event", "province_event", event_tag, read_province_event)
		MEMBER_TYPE_EXTERN("country_event", "country_event", event_tag, read_country_event)
		END_TYPE
	END_DOMAIN;

	inline auto pair_function(const token_and_type& l, association_type a, const token_and_type& r) { return pair_from_lh_rh<uint16_t, int32_t>(l,a,r); }

	BEGIN_DOMAIN(on_actions_domain)
		BEGIN_TYPE(empty_type)
		MEMBER_VARIABLE_ASSOCIATION("unknown_key", accept_all, discard_from_full)
		MEMBER_VARIABLE_TYPE_ASSOCIATION("unknown_key", accept_all, empty_type, discard_empty_type)
		END_TYPE
		BEGIN_TYPE(on_actions_file)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_yearly_pulse", on_yearly_pulse_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_quarterly_pulse", on_quarterly_pulse_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_new_great_nation", on_new_great_nation_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_lost_great_nation", on_lost_great_nation_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_election_tick", on_election_tick_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_colony_to_state", on_colony_to_state_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_state_conquest", on_state_conquest_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_colony_to_state_free_slaves", on_colony_to_state_free_slaves_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_battle_won", on_battle_won_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_battle_lost", on_battle_lost_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_debtor_default", on_debtor_default_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_debtor_default_small", on_debtor_default_small_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_debtor_default_second", on_debtor_default_second_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_civilize", on_civilize_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_my_factories_nationalized", on_my_factories_nationalized_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_crisis_declare_interest", on_crisis_declare_interest_s)
		MEMBER_TYPE_ASSOCIATION("sub_category", "on_surrender", empty_type)
		END_TYPE
		BEGIN_TYPE(on_yearly_pulse_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_quarterly_pulse_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_new_great_nation_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_lost_great_nation_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_election_tick_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_colony_to_state_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_state_conquest_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_colony_to_state_free_slaves_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_battle_won_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_battle_lost_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_debtor_default_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_debtor_default_small_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_debtor_default_second_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_civilize_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_my_factories_nationalized_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
		BEGIN_TYPE(on_crisis_declare_interest_s)
		MEMBER_VARIABLE_ASSOCIATION("item", accept_all, pair_function)
		END_TYPE
	END_DOMAIN;

	std::pair<int32_t, bool> pre_parse_event(const token_group* start, const token_group* end) { // returns id &  bool is triggered 
		const auto result = parse_object<event_preparse, single_event_preparsing_domain>(start, end);
		return std::pair<int32_t, bool>(result.id, result.is_triggered_only);
	}

	event_tag parse_single_event(
		scenario::scenario_manager& s,
		graphics::texture_manager& t,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const triggers::trigger_scope_state& scope,
		const token_group* start,
		const token_group* end) {
		
		event_parse_env env(s, t, ecm, pictures_root, scope);
		const auto result = parse_object<event_reader, single_event_domain>(start, end, env);
		const auto new_tag = s.event_m.event_container.emplace_back(result.under_construction);
		s.event_m.event_container[new_tag].id = new_tag;

		return new_tag;
	}

	void parse_event_file(
		scenario::scenario_manager& s,
		graphics::texture_manager& t,
		event_creation_manager& ecm,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end) {

		event_file_parse_env env(s, t, ecm, pictures_root);
		parse_object<event_file, event_file_domain>(start, end, env);
	}

	void parse_event_files(
		scenario::scenario_manager& s,
		graphics::texture_manager& t,
		event_creation_manager& ecm,
		const directory& source_directory) {

		const auto event_dir = source_directory.get_directory(u"\\events");

		const auto event_files = event_dir.list_files(u".txt");
		for(const auto& efile : event_files) {
			if(auto f = efile.open_file(); f) {
				ecm.event_files_data.emplace_back();
				parsed_data& parse = ecm.event_files_data.back();

				const auto sz = f->size();
				parse.parse_data = std::unique_ptr<char[]>(new char[sz]);

				f->read_to_buffer(parse.parse_data.get(), sz);
				parse_pdx_file(parse.parse_results, parse.parse_data.get(), parse.parse_data.get() + sz);

				parse_event_file(
					s,
					t,
					ecm,
					source_directory,
					parse.parse_results.data(),
					parse.parse_results.data() + parse.parse_results.size());
			}
		}
	}

#ifdef _DEBUG
	struct event_source_missing {};
#endif

	void commit_pending_triggered_events(
		scenario::scenario_manager& s,
		graphics::texture_manager& t,
		event_creation_manager& ecm,
		const directory& pictures_root) {

		while(ecm.pending_triggered_events.size() != 0) {
			ecm.created_triggered_events.insert(ecm.pending_triggered_events.begin(), ecm.pending_triggered_events.end());
			boost::container::flat_map<id_scope_pair, event_tag> pending_copy(std::move(ecm.pending_triggered_events));
			ecm.pending_triggered_events.clear();

			for(const auto& pr : pending_copy) {
				ecm.created_triggered_events.emplace(pr.first, pr.second);

				if(const auto source = ecm.event_sources.find(pr.first.id); source != ecm.event_sources.end()) {
					event_parse_env env(s, t, ecm, pictures_root, pr.first.scope);
					const auto result = parse_object<event_reader, single_event_domain>(source->second.start, source->second.end, env);

					s.event_m.event_container[pr.second] = result.under_construction;
					s.event_m.event_container[pr.second].id = pr.second;
				} else {
#ifdef _DEBUG
					throw event_source_missing();
#endif
				}
				
			}

			//loop until committing events creates no new pending events
		}
	}

	token_and_type get_issue_group_for_event(const token_group* start, const token_group* end) {
		return parse_object<issue_group_reader, get_issue_group_domain>(start, end).issue_group;
	}

	void parse_on_actions_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");
		const auto f = common_dir.open_file(u"on_actions.txt");

		if(f) {
			on_actions_env env(s, ecm);
			const auto sz = f->size();
			std::unique_ptr<char[]> data(new char[sz]);
			std::vector<token_group> parsed_data;

			f->read_to_buffer(data.get(), sz);
			parse_pdx_file(parsed_data, data.get(), data.get() + sz);

			parse_object<on_actions_file, on_actions_domain>(parsed_data.data(), parsed_data.data() + parsed_data.size(), env);
		}
	}


	decision_tag parse_decision(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		graphics::texture_manager& t,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end) {

		event_file_parse_env env(s, t, ecm, pictures_root);
		const auto result = parse_object<decision_reader, single_decision_parse_domain>(start, end, env);
		const auto new_tag = s.event_m.decision_container.emplace_back(result.under_construction);
		s.event_m.decision_container[new_tag].id = new_tag;

		return new_tag;
	}

	void parse_decision_file(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		graphics::texture_manager& t,
		const directory& pictures_root,
		const token_group* start,
		const token_group* end) {

		event_file_parse_env env(s, t, ecm, pictures_root);
		parse_object<decision_file, decision_file_domain>(start, end, env);
	}

	void parse_decision_files(
		scenario::scenario_manager& s,
		event_creation_manager& ecm,
		graphics::texture_manager& t,
		const directory& root) {

		const auto event_dir = root.get_directory(u"\\decisions");

		const auto event_files = event_dir.list_files(u".txt");
		for(const auto& efile : event_files) {
			if(auto f = efile.open_file(); f) {
				parsed_data parse;

				const auto sz = f->size();
				parse.parse_data = std::unique_ptr<char[]>(new char[sz]);

				f->read_to_buffer(parse.parse_data.get(), sz);
				parse_pdx_file(parse.parse_results, parse.parse_data.get(), parse.parse_data.get() + sz);

				parse_decision_file(
					s,
					ecm,
					t,
					root,
					parse.parse_results.data(),
					parse.parse_results.data() + parse.parse_results.size());
			}
		}

	}
}
