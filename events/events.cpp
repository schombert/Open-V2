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
		void one_sub_category(const T&) {}
	};
}

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

namespace events {
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
		MEMBER_VARIABLE_TYPE_ASSOCIATION("discard", accept_all, empty_type, discard_empty_type)
		MEMBER_VARIABLE_ASSOCIATION("discard", accept_all, discard_from_full)
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
}
