#include "common\\common.h"
#include "military_io.hpp"
#include "Parsers\\parsers.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "economy\\economy.h"
#include "text_data\\text_data.h"
#include "sound\\sound.h"
#include "events\\events.h"
#include "scenario\\scenario.h"
#include "triggers\\trigger_reading.h"
#include "triggers\\effect_reading.h"
#include "world_state\\world_state.h"
#include "military_functions.h"
#include "nations\\nations_functions.h"
#include "population\\population_functions.hpp"

void serialization::serializer<military::military_state>::serialize_object(std::byte *& output, military::military_state const & obj, world_state const & ws) {
	serialize(output, obj.leaders, ws);
	serialize(output, obj.armies, ws);
	serialize(output, obj.army_orders, ws);
	serialize(output, obj.fleets, ws);
	serialize(output, obj.wars, ws);
	serialize(output, obj.strategic_hqs, ws);
}

void serialization::serializer<military::military_state>::deserialize_object(std::byte const *& input, military::military_state & obj, world_state & ws) {
	deserialize(input, obj.leaders, ws);
	deserialize(input, obj.armies, ws);
	deserialize(input, obj.army_orders, ws);
	deserialize(input, obj.fleets, ws);
	deserialize(input, obj.wars, ws);
	deserialize(input, obj.strategic_hqs, ws);
}

size_t serialization::serializer<military::military_state>::size(military::military_state const & obj, world_state const & ws) {
	return serialize_size(obj.leaders, ws) +
		serialize_size(obj.armies, ws) +
		serialize_size(obj.army_orders, ws) +
		serialize_size(obj.fleets, ws) +
		serialize_size(obj.wars, ws) +
		serialize_size(obj.strategic_hqs, ws);
}

namespace military {
	struct war_goal_reader {
		world_state& ws;
		war_goal under_construction;

		war_goal_reader(world_state& w, date_tag) : ws(w) {}
		void set_actor(token_and_type const& t) {
			under_construction.from_country = nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end)));
		}
		void set_receiver(token_and_type const& t) {
			under_construction.target_country = nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end)));
		}
		void set_state(uint16_t v) {
			auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(provinces::province_tag(v));
			if(is_valid_index(state))
				under_construction.target_state = state;
		}
		void set_cb(token_and_type const& t) {
			under_construction.cb_type = tag_from_text(ws.s.military_m.named_cb_type_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}

	};

	struct war_date_block {
		world_state& ws;
		std::vector<nations::country_tag> attacker;
		std::vector<nations::country_tag> defender;
		std::vector<nations::country_tag> rem_attacker;
		std::vector<nations::country_tag> rem_defender;
		std::vector<war_goal> war_goals;

		war_date_block(world_state& w, date_tag) : ws(w) {}

		void add_attacker(token_and_type const& t) {
			attacker.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		void add_defender(token_and_type const& t) {
			defender.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		void remove_attacker(token_and_type const& t) {
			rem_attacker.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		void remove_defender(token_and_type const& t) {
			rem_defender.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		void add_war_goal(war_goal_reader const& wg) {
			war_goals.push_back(wg.under_construction);
		}
	};

	struct war_file {
		world_state& ws;
		date_tag filter_date;

		std::vector<nations::country_tag> attacker;
		std::vector<nations::country_tag> defender;
		std::vector<war_goal> war_goals;

		war_file(world_state& w, date_tag d) : ws(w), filter_date(d) {}

		void discard(int) {}
		void add_date_block(std::pair<date_tag, war_date_block> const& block) {
			if(block.first <= filter_date) {
				for(auto i : block.second.attacker)
					attacker.push_back(i);
				for(auto i : block.second.defender)
					defender.push_back(i);
				for(auto i : block.second.rem_attacker) {
					for(int32_t j = int32_t(attacker.size()) - 1; j >= 0; --j) {
						if(attacker[uint32_t(j)] == i) {
							attacker[uint32_t(j)] = attacker.back();
							attacker.pop_back();
						}
					}
				}
				for(auto i : block.second.rem_defender) {
					for(int32_t j = int32_t(defender.size()) - 1; j >= 0; --j) {
						if(defender[uint32_t(j)] == i) {
							defender[uint32_t(j)] = defender.back();
							defender.pop_back();
						}
					}
				}
				for(auto& i : block.second.war_goals) {
					war_goals.push_back(i);
					war_goals.back().date_added = block.first;
				}
			}
		}
	};

	inline std::pair<date_tag, war_date_block> date_block(token_and_type const& t, association_type, war_date_block&& b) {
		return std::pair<date_tag, war_date_block>(token_to<date_tag>(t), std::move(b));
	}

	struct parsed_relation {
		int32_t value = 0;
		int32_t level = 2;
		int32_t influence_value = 0;
		void discard(int) {}
	};

	inline std::pair<token_and_type, parsed_relation> name_relation(token_and_type const& t, association_type, parsed_relation const& r) {
		return std::pair<token_and_type, parsed_relation>(t, r);
	}

	struct parsed_leader {
		world_state& ws;
		nations::country_tag this_nation;

		bool is_general = true;
		date_tag creation_date;
		leader_trait_tag personality;
		leader_trait_tag background;

		parsed_leader(world_state& w, nations::country_tag n) : ws(w), this_nation(n) {}

		void set_date(token_and_type const& t) {
			creation_date = parse_date(t.start, t.end);
		}
		void set_type(token_and_type const& t) {
			if(is_fixed_token_ci(t, "land"))
				is_general = true;
			else
				is_general = false;
		}
		void set_personality(token_and_type const& t) {
			personality = tag_from_text(ws.s.military_m.named_leader_trait_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void set_background(token_and_type const& t) {
			background = tag_from_text(ws.s.military_m.named_leader_trait_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		void discard(int) {}
	};

	struct parsed_regiment {
		provinces::province_tag home;
		void discard(int) {}
		void set_home(uint16_t p) {
			home = provinces::province_tag(p);
		}
	};

	struct parsed_ship {
		world_state& ws;
		nations::country_tag this_nation;

		parsed_ship(world_state& w, nations::country_tag n) : ws(w), this_nation(n) {}

		//unit_type_tag type;
		void discard(int) {}
		void set_type(token_and_type const& t) {
			// type = tag_from_text(ws.s.military_m.named_unit_type_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
	};

	struct parsed_army_or_navy {
		world_state& ws;
		nations::country_tag this_nation;

		leader_tag set_leader;
		provinces::province_tag location;

		// std::vector<provinces::province_tag> regiment_sources;
		// std::vector<unit_type_tag> ship_types;
		
		int32_t ship_count = 0;
		int32_t regiment_count = 0;

		parsed_army_or_navy(world_state& w, nations::country_tag n) : ws(w), this_nation(n) {}

		void add_leader(parsed_leader const& l) {
			auto new_leader = make_empty_leader(ws, ws.w.nation_s.nations.get<nation::primary_culture>(this_nation), l.is_general);
			ws.set<military_leader::background>(new_leader, l.background);
			ws.set<military_leader::personality>(new_leader, l.personality);
			ws.set<military_leader::creation_date>(new_leader, l.creation_date);

			calculate_leader_traits(ws, new_leader);

			if(l.is_general)
				ws.add_item(ws.get<nation::generals>(this_nation), new_leader);
			else
				ws.add_item(ws.get<nation::admirals>(this_nation), new_leader);

			set_leader = new_leader;
		}
		void set_location(uint16_t p) {
			location = provinces::province_tag(p);
		}
		void add_regiment(parsed_regiment const & r) {
			++regiment_count;
			// regiment_sources.push_back(r.home);
		}
		void add_ship(parsed_ship const & s) {
			++ship_count;
			// ship_types.push_back(s.type);
		}
		void discard(int) {}
	};

	struct oob_file {
		world_state& ws;
		nations::country_tag this_nation;

		oob_file(world_state& w, nations::country_tag n) : ws(w), this_nation(n) {}

		void add_relation(std::pair<token_and_type, parsed_relation> const& p) {
			auto other_tag = tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(p.first.start, p.first.end));
			auto other_nation = nations::make_nation_for_tag(ws, other_tag);
			if(p.second.value != 0)
				nations::set_relationship(ws, this_nation, other_nation, p.second.value);
			if(p.second.influence_value != 0 || p.second.level != 2)
				nations::set_influence(ws, this_nation, other_nation, float(p.second.influence_value), p.second.level);
		}
		void add_leader(parsed_leader const& l) {
			auto new_leader = make_empty_leader(ws, ws.w.nation_s.nations.get<nation::primary_culture>(this_nation), l.is_general);
			ws.set<military_leader::background>(new_leader, l.background);
			ws.set<military_leader::personality>(new_leader, l.personality);
			ws.set<military_leader::creation_date>(new_leader, l.creation_date);

			calculate_leader_traits(ws, new_leader);

			if(l.is_general)
				ws.add_item(ws.get<nation::generals>(this_nation), new_leader);
			else
				ws.add_item(ws.get<nation::admirals>(this_nation), new_leader);
		}
		void add_army(parsed_army_or_navy&& a) {
			if(!is_valid_index(a.location))
				a.location = ws.get<nation::current_capital>(this_nation);

			auto new_army = make_army(ws, this_nation, a.location);
			if(is_valid_index(a.set_leader)) {
				ws.set<army::leader>(new_army, a.set_leader);
				ws.set<military_leader::is_attached>(a.set_leader, true);
			}

			ws.set<army::target_solders>(new_army, 1000.0f * a.regiment_count);
		}
		void add_navy(parsed_army_or_navy const& n) {
			auto new_fleet = make_fleet(ws, this_nation, n.location);
			if(is_valid_index(n.set_leader)) {
				ws.set<fleet::leader>(new_fleet, n.set_leader);
				ws.set<military_leader::is_attached>(n.set_leader, true);
			}
			ws.set<fleet::size>(new_fleet, float(n.ship_count));
		}
	};

	struct parsing_environment {
		text_data::text_sequences& text_lookup;

		military_manager& manager;

		parsed_data cb_file;
		// std::vector<parsed_data> unit_type_files;
		// std::vector<std::tuple<unit_type_tag, token_group const*, token_group const*>> pending_unit_parse;
		std::vector<std::tuple<cb_type_tag, token_group const*, token_group const*>> pending_cb_parse;

		parsing_environment(text_data::text_sequences& tl, military_manager& m) :
			text_lookup(tl), manager(m) {}
	};

	parsing_state::parsing_state(text_data::text_sequences& tl, military_manager& m) :
		impl(std::make_unique<parsing_environment>(tl, m)) {}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}


	struct cb_environment {
		scenario::scenario_manager& s;
		events::event_creation_manager& ecm;
		cb_type& under_construction;

		cb_environment(scenario::scenario_manager& sm, events::event_creation_manager& e, cb_type& uc) : s(sm), ecm(e), under_construction(uc) {}
	};

	inline triggers::trigger_tag read_cb_state_trigger(token_group const* s, token_group const* e, cb_environment& env) {
		const auto td = triggers::parse_trigger(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::state,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation }, s, e);
		return triggers::commit_trigger(env.s.trigger_m, td);
	}
	inline triggers::trigger_tag read_cb_nation_trigger(token_group const* s, token_group const* e, cb_environment& env) {
		const auto td = triggers::parse_trigger(env.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation }, s, e);
		return triggers::commit_trigger(env.s.trigger_m, td);
	}

	inline triggers::effect_tag read_cb_nation_effect(token_group const* s, token_group const* e, cb_environment& env) {
		const auto td = triggers::parse_effect(env.s, env.ecm,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation }, s, e);
		return triggers::commit_effect(env.s.trigger_m, td);
	}
	inline int discard_section(token_group const*, token_group const*, cb_environment&) {
		return 0;
	}

	struct single_cb {
		cb_environment& env;
		single_cb(cb_environment& e) : env(e) {
			static const auto lib_name_a = text_data::get_thread_safe_text_handle(e.s.gui_m.text_data_sequences, "free_peoples");
			static const auto lib_name_b = text_data::get_thread_safe_text_handle(e.s.gui_m.text_data_sequences, "liberate_country");
			static const auto take_from_sphere = text_data::get_thread_safe_text_handle(e.s.gui_m.text_data_sequences, "take_from_sphere");

			if((e.under_construction.name == lib_name_a) | (e.under_construction.name == lib_name_b))
				e.under_construction.flags |= cb_type::po_liberate;
			if(e.under_construction.name == take_from_sphere)
				e.under_construction.flags |= cb_type::po_take_from_sphere;
		}
		void set_is_civil_war(bool v) {
			if(v) env.under_construction.flags |= cb_type::is_civil_war;
		}
		void set_months(uint8_t v) {
			env.under_construction.months = v;
		}
		void set_sprite_index(uint8_t v) {
			env.under_construction.sprite_index = v;
		}
		void set_always(bool v) {
			if(v) env.under_construction.flags |= cb_type::always;
		}
		void set_is_triggered_only(bool v) {
			if(!v) env.under_construction.flags |= cb_type::is_not_triggered_only;
		}
		void set_constructing_cb(bool v) {
			if(!v) env.under_construction.flags |= cb_type::is_not_constructing_cb;
		}
		void set_allowed_states(triggers::trigger_tag t) {
			env.under_construction.allowed_states = t;
		}
		void set_allowed_states_in_crisis(triggers::trigger_tag t) {
			env.under_construction.allowed_states_in_crisis = t;
		}
		void set_allowed_substate_regions(triggers::trigger_tag t) {
			env.under_construction.allowed_substate_regions = t;
		}
		void set_allowed_countries(triggers::trigger_tag t) {
			env.under_construction.allowed_countries = t;
		}
		void set_can_use(triggers::trigger_tag t) {
			env.under_construction.can_use = t;
		}
		void set_on_add(triggers::effect_tag t) {
			env.under_construction.on_add = t;
		}
		void set_on_po_accepted(triggers::effect_tag t) {
			env.under_construction.on_po_accepted = t;
		}
		void set_badboy_factor(float v) {
			env.under_construction.badboy_factor = v;
		}
		void set_prestige_factor(float v) {
			env.under_construction.prestige_factor = v;
		}
		void set_peace_cost_factor(float v) {
			env.under_construction.peace_cost_factor = v;
		}
		void set_penalty_factor(float v) {
			env.under_construction.penalty_factor = v;
		}
		void set_break_truce_prestige_factor(float v) {
			env.under_construction.break_truce_prestige_factor = v;
		}
		void set_break_truce_infamy_factor(float v) {
			env.under_construction.break_truce_infamy_factor = v;
		}
		void set_break_truce_militancy_factor(float v) {
			env.under_construction.break_truce_militancy_factor = v;
		}
		void set_good_relation_prestige_factor(float v) {
			env.under_construction.good_relation_prestige_factor = v;
		}
		void set_good_relation_infamy_factor(float v) {
			env.under_construction.good_relation_infamy_factor = v;
		}
		void set_good_relation_militancy_factor(float v) {
			env.under_construction.good_relation_militancy_factor = v;
		}
		void set_truce_months(uint8_t v) {
			env.under_construction.truce_months = v;
		}
		void set_war_name(token_and_type const& t) {
			std::string prepended = std::string("NORMAL_") + std::string(t.start, t.end);
			env.under_construction.war_name = text_data::get_thread_safe_text_handle(env.s.gui_m.text_data_sequences, prepended.c_str(), prepended.c_str() + prepended.length());
		}
		void set_construction_speed(float v) {
			env.under_construction.construction_speed = v;
		}
		void set_tws_battle_factor(float v) {
			env.under_construction.tws_battle_factor = v;
		}
		void set_great_war_obligatory(bool v) {
			if(v) env.under_construction.flags |= cb_type::great_war_obligatory;
		}
		void set_all_allowed_states(bool v) {
			if(v) env.under_construction.flags |= cb_type::all_allowed_states;
		}
		void set_crisis(bool v) {
			if(!v) env.under_construction.flags |= cb_type::not_in_crisis;
		}
		void set_po_clear_union_sphere(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_clear_union_sphere;
		}
		void set_po_gunboat(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_gunboat;
		}
		void set_po_annex(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_annex;
		}
		void set_po_demand_state(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_demand_state;
		}
		void set_po_add_to_sphere(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_add_to_sphere;
		}
		void set_po_disarmament(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_disarmament;
		}
		void set_po_reparations(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_reparations;
		}
		void set_po_transfer_provinces(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_transfer_provinces;
		}
		void set_po_remove_prestige(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_remove_prestige;
		}
		void set_po_make_puppet(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_make_puppet;
		}
		void set_po_release_puppet(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_release_puppet;
		}
		void set_po_status_quo(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_status_quo;
		}
		void set_po_install_communist_gov_type(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_install_communist_gov_type;
		}
		void set_po_uninstall_communist_gov_type(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_uninstall_communist_gov_type;
		}
		void set_po_remove_cores(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_remove_cores;
		}
		void set_po_colony(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_colony;
		}
		void set_po_destroy_forts(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_destroy_forts;
		}
		void set_po_destroy_naval_bases(bool v) {
			if(v) env.under_construction.flags |= cb_type::po_destroy_naval_bases;
		}
		void discard(int) {}
	};

	/*
	struct unit_file {
		parsing_environment& env;
		unit_file(parsing_environment& e) : env(e) {}
		void add_unit(int) {}
	};

	inline int register_unit_type(const token_group* start, const token_group* end, const token_and_type& t, parsing_environment& env) {
		const auto uname = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto uid = env.manager.unit_types.emplace_back();

		env.manager.unit_types[uid].id = uid;
		env.manager.unit_types[uid].name = uname;
		env.manager.named_unit_type_index.emplace(uname, uid);
		env.pending_unit_parse.emplace_back(uid, start, end);

		return 0;
	}
	*/

	inline int register_cb_type(token_group const* start, token_group const* end, token_and_type const& t, parsing_environment& env) {
		const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
		const auto cbtag = tag_from_text(env.manager.named_cb_type_index, name);
		if(!is_valid_index(cbtag)) {
			auto const cbid = env.manager.cb_types.emplace_back();
			env.manager.cb_types[cbid].id = cbid;
			env.manager.cb_types[cbid].name = name;
			env.manager.named_cb_type_index.emplace(name, cbid);

			std::string setup_name = std::string(t.start, t.end) + std::string("_setup");
			env.manager.cb_types[cbid].explanation = text_data::get_thread_safe_text_handle(env.text_lookup, setup_name.c_str(), setup_name.c_str() + setup_name.length());

			env.pending_cb_parse.emplace_back(cbid, start, end);
		} else {

			std::string setup_name = std::string(t.start, t.end) + std::string("_setup");
			env.manager.cb_types[cbtag].explanation = text_data::get_thread_safe_text_handle(env.text_lookup, setup_name.c_str(), setup_name.c_str() + setup_name.length());

			env.pending_cb_parse.emplace_back(cbtag, start, end);
		}
		return 0;
	}

	struct peace_order {
		parsing_environment& env;
		peace_order(parsing_environment& e) : env(e) {}
		void add_cb(const token_and_type& t) {
			const auto name = text_data::get_thread_safe_text_handle(env.text_lookup, t.start, t.end);
			if(0 == env.manager.named_cb_type_index.count(name)) {
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
		void add_cb(int) {}
	};

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

	inline void add_trait_to_manager(military_manager& m, const trait& t, leader_trait_tag id) {
		auto row_ptr = m.leader_trait_definitions.safe_get_row(id);
		row_ptr[traits::organisation] = t.organisation;
		row_ptr[traits::morale] = t.morale;
		row_ptr[traits::attack] = t.attack;
		row_ptr[traits::defence] = t.defence;
		row_ptr[traits::reconnaissance] = t.reconnaissance;
		row_ptr[traits::speed] = t.speed;
		row_ptr[traits::experience] = t.experience;
		row_ptr[traits::reliability] = t.reliability;
	}

	struct personalities {
		parsing_environment& env;
		personalities(parsing_environment& e) : env(e) {}

		void add_trait(const std::pair<token_and_type, trait>& t) {
			const auto new_trait = add_trait_to_manager(env.manager, t.second);
			const auto trait_name = text_data::get_thread_safe_text_handle(env.text_lookup, t.first.start, t.first.end);
			env.manager.leader_traits.safe_get(new_trait) = trait_name;

			env.manager.personality_traits.push_back(new_trait);
			env.manager.named_leader_trait_index.emplace(trait_name, new_trait);
		}
		void add_no_personality(const trait& t) {
			add_trait_to_manager(env.manager, t, env.manager.no_personality_trait);
		}
	};
	struct backgrounds {
		parsing_environment& env;
		backgrounds(parsing_environment& e) : env(e) {}

		void add_trait(const std::pair<token_and_type, trait>& t) {
			const auto new_trait = add_trait_to_manager(env.manager, t.second);
			const auto trait_name = text_data::get_thread_safe_text_handle(env.text_lookup, t.first.start, t.first.end);
			env.manager.leader_traits.safe_get(new_trait) = trait_name;

			env.manager.background_traits.push_back(new_trait);
			env.manager.named_leader_trait_index.emplace(trait_name, new_trait);
		}
		void add_no_background(const trait& t) {
			add_trait_to_manager(env.manager, t, env.manager.no_background_trait);
		}
	};

	struct traits_file {
		parsing_environment& env;
		traits_file(parsing_environment& e) : env(e) {}

		void add_personalities(const personalities&) {}
		void add_backgrounds(const backgrounds&) {}
	};

	inline std::pair<token_and_type, trait> name_trait(const token_and_type& t, association_type, const trait& r) {
		return std::pair<token_and_type, trait>(t, r);
	}

	/*struct unit_type_env {
		military_manager& military_m;
		economy::economic_scenario& econ_m;
		sound::sound_manager& sound_m;
		text_data::text_sequences& text;
		unit_type& unit;

		unit_type_env(military_manager& a, economy::economic_scenario& b, sound::sound_manager& c, text_data::text_sequences& d, unit_type& e) :
			military_m(a), econ_m(b), sound_m(c), text(d), unit(e) {}
	};

	struct unit_build_cost {
		unit_type_env& env;
		unit_build_cost(unit_type_env& e) : env(e) {}

		void set_value(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto good_name = text_data::get_thread_safe_existing_text_handle(env.text, p.first.start, p.first.end);
			const auto good_tag = tag_from_text(env.econ_m.named_goods_index, good_name);

			if(is_valid_index(good_tag))
				env.military_m.unit_build_costs.get(env.unit.id, good_tag) = p.second;
		}
	};
	struct unit_supply_cost {
		unit_type_env& env;
		unit_supply_cost(unit_type_env& e) : env(e) {}

		void set_value(const std::pair<token_and_type, economy::goods_qnty_type>& p) {
			const auto good_name = text_data::get_thread_safe_existing_text_handle(env.text, p.first.start, p.first.end);
			const auto good_tag = tag_from_text(env.econ_m.named_goods_index, good_name);

			if(is_valid_index(good_tag))
				env.military_m.unit_base_supply_costs.get(env.unit.id, good_tag) = p.second;
		}
	};
	*/

	inline std::pair<token_and_type, economy::goods_qnty_type> get_econ_value(const token_and_type& l, association_type, const token_and_type& r) {
		return std::pair<token_and_type, economy::goods_qnty_type>(l, token_to<economy::goods_qnty_type>(r));
	}
	/*
	struct unit_type_reader {
		unit_type_env& env;

		unit_type_reader(unit_type_env& e) : env(e) {}

		void set_build_cost(const unit_build_cost&) {}
		void set_supply_cost(const unit_supply_cost&) {}

		void set_attack_gun_power(float v) {
			env.unit.base_attributes[unit_attribute::attack] = v;
		}
		void set_maneuver_evasion(float v) {
			env.unit.base_attributes[unit_attribute::maneuver] = v;
		}
		void set_max_strength(float v) {
			env.unit.base_attributes[unit_attribute::strength] = v;
		}
		void set_default_organisation(float v) {
			env.unit.base_attributes[unit_attribute::organization] = v;
		}
		void set_build_time(float v) {
			env.unit.base_attributes[unit_attribute::build_time] = v;
		}
		void set_maximum_speed(float v) {
			env.unit.base_attributes[unit_attribute::speed] = v;
		}
		void set_supply_consumption(float v) {
			env.unit.base_attributes[unit_attribute::supply_consumption] = v;
		}
		void set_defence_hull(float v) {
			env.unit.base_attributes[unit_attribute::defense] = v;
		}
		void set_reconnaissance_fire_range(float v) {
			env.unit.base_attributes[unit_attribute::reconnaissance] = v;
		}
		void set_discipline(float v) {
			env.unit.base_attributes[unit_attribute::discipline] = v;
		}
		void set_support_torpedo_attack(float v) {
			env.unit.base_attributes[unit_attribute::support] = v;
		}
		void set_siege(float v) {
			env.unit.base_attributes[unit_attribute::siege] = v;
		}
		void set_primary_culture(bool v) {
			if(v)
				env.unit.flags |= unit_type::primary_culture;
		}
		void set_active(bool v) {
			if(!v)
				env.unit.base_attributes[unit_attribute::enabled] = unit_attribute_type(0);
		}
		void set_can_build_overseas(bool v) {
			if(!v)
				env.unit.flags |= unit_type::cant_build_overseas;
		}
		void set_sail(bool v) {
			if(v)
				env.unit.flags |= unit_type::is_sail;
		}
		void set_unit_type(const token_and_type& t) {
			if(is_fixed_token_ci(t, "infantry")) {
				env.unit.flags |= unit_type::class_infantry;
			} else if(is_fixed_token_ci(t, "cavalry")) {
				env.unit.flags |= unit_type::class_cavalry;
			} else if(is_fixed_token_ci(t, "special")) {
				env.unit.flags |= unit_type::class_special;
			} else if(is_fixed_token_ci(t, "support")) {
				env.unit.flags |= unit_type::class_support;
			} else if(is_fixed_token_ci(t, "big_ship")) {
				env.unit.flags |= unit_type::class_big_ship;
			} else if(is_fixed_token_ci(t, "light_ship")) {
				env.unit.flags |= unit_type::class_light_ship;
			} else if(is_fixed_token_ci(t, "tansport")) {
				env.unit.flags |= unit_type::class_transport;
			}
		}
		void set_select_sound(const token_and_type& t) {
			//env.unit.select_sound = tag_from_text(env.sound_m.named_sound_effects, text_data::get_thread_safe_existing_text_handle(env.text, t.start, t.end));
		}
		void set_move_sound(const token_and_type& t) {
			//env.unit.move_sound = tag_from_text(env.sound_m.named_sound_effects, text_data::get_thread_safe_existing_text_handle(env.text, t.start, t.end));
		}
		void set_icon(uint8_t v) {
			env.unit.icon = v;
		}
		void set_colonial_points(uint8_t v) {
			env.unit.colonial_points = v;
		}
		void set_naval_icon(uint8_t v) {
			env.unit.naval_icon = v;
		}
		void set_min_port_level(uint8_t v) {
			env.unit.min_port_level = v;
		}
		void set_limit_per_port(int8_t v) {
			env.unit.limit_per_port = v;
		}
		void set_supply_consumption_score(uint8_t v) {
			env.unit.supply_consumption_score = v;
		}
		void discard(int) {}
	};
	*/
}

MEMBER_FDEF(military::war_goal_reader, set_actor, "actor");
MEMBER_FDEF(military::war_goal_reader, set_receiver, "receiver");
MEMBER_FDEF(military::war_goal_reader, set_state, "state");
MEMBER_FDEF(military::war_goal_reader, set_cb, "cb");
MEMBER_FDEF(military::war_date_block, add_attacker, "add_attacker");
MEMBER_FDEF(military::war_date_block, add_defender, "add_defender");
MEMBER_FDEF(military::war_date_block, remove_attacker, "rem_attacker");
MEMBER_FDEF(military::war_date_block, remove_defender, "rem_defender");
MEMBER_FDEF(military::war_date_block, add_war_goal, "add_war_goal");
MEMBER_FDEF(military::war_file, discard, "discard");
MEMBER_FDEF(military::war_file, add_date_block, "date_block");

MEMBER_FDEF(military::oob_file, add_leader, "leader");
MEMBER_FDEF(military::oob_file, add_relation, "relation");
MEMBER_FDEF(military::oob_file, add_army, "army");
MEMBER_FDEF(military::oob_file, add_navy, "navy");
MEMBER_FDEF(military::parsed_army_or_navy, add_leader, "leader");
MEMBER_FDEF(military::parsed_army_or_navy, set_location, "location");
MEMBER_FDEF(military::parsed_army_or_navy, add_regiment, "regiment");
MEMBER_FDEF(military::parsed_army_or_navy, add_ship, "ship");
MEMBER_FDEF(military::parsed_army_or_navy, discard, "discard");
MEMBER_FDEF(military::parsed_ship, set_type, "type");
MEMBER_FDEF(military::parsed_ship, discard, "discard");
MEMBER_FDEF(military::parsed_regiment, set_home, "home");
MEMBER_FDEF(military::parsed_regiment, discard, "discard");
MEMBER_FDEF(military::parsed_leader, set_date, "date");
MEMBER_FDEF(military::parsed_leader, set_type, "type");
MEMBER_FDEF(military::parsed_leader, set_personality, "personality");
MEMBER_FDEF(military::parsed_leader, set_background, "background");
MEMBER_FDEF(military::parsed_leader, discard, "discard");
MEMBER_DEF(military::parsed_relation, value, "value");
MEMBER_DEF(military::parsed_relation, level, "level");
MEMBER_DEF(military::parsed_relation, influence_value, "influence_value");
MEMBER_FDEF(military::parsed_relation, discard, "discard");

MEMBER_FDEF(military::single_cb, discard, "discard");
MEMBER_FDEF(military::single_cb, set_is_civil_war, "is_civil_war");
MEMBER_FDEF(military::single_cb, set_months, "months");
MEMBER_FDEF(military::single_cb, set_truce_months, "truce_months");
MEMBER_FDEF(military::single_cb, set_sprite_index, "sprite_index");
MEMBER_FDEF(military::single_cb, set_always, "always");
MEMBER_FDEF(military::single_cb, set_is_triggered_only, "is_triggered_only");
MEMBER_FDEF(military::single_cb, set_constructing_cb, "constructing_cb");
MEMBER_FDEF(military::single_cb, set_allowed_states, "allowed_states");
MEMBER_FDEF(military::single_cb, set_allowed_states_in_crisis, "allowed_states_in_crisis");
MEMBER_FDEF(military::single_cb, set_allowed_substate_regions, "allowed_substate_regions");
MEMBER_FDEF(military::single_cb, set_allowed_countries, "allowed_countries");
MEMBER_FDEF(military::single_cb, set_can_use, "can_use");
MEMBER_FDEF(military::single_cb, set_on_add, "on_add");
MEMBER_FDEF(military::single_cb, set_on_po_accepted, "on_po_accepted");
MEMBER_FDEF(military::single_cb, set_badboy_factor, "badboy_factor");
MEMBER_FDEF(military::single_cb, set_prestige_factor, "prestige_factor");
MEMBER_FDEF(military::single_cb, set_peace_cost_factor, "peace_cost_factor");
MEMBER_FDEF(military::single_cb, set_penalty_factor, "penalty_factor");
MEMBER_FDEF(military::single_cb, set_break_truce_prestige_factor, "break_truce_prestige_factor");
MEMBER_FDEF(military::single_cb, set_break_truce_infamy_factor, "break_truce_infamy_factor");
MEMBER_FDEF(military::single_cb, set_break_truce_militancy_factor, "break_truce_militancy_factor");
MEMBER_FDEF(military::single_cb, set_good_relation_prestige_factor, "good_relation_prestige_factor");
MEMBER_FDEF(military::single_cb, set_good_relation_infamy_factor, "good_relation_infamy_factor");
MEMBER_FDEF(military::single_cb, set_good_relation_militancy_factor, "good_relation_militancy_factor");
MEMBER_FDEF(military::single_cb, set_war_name, "war_name");
MEMBER_FDEF(military::single_cb, set_construction_speed, "construction_speed");
MEMBER_FDEF(military::single_cb, set_tws_battle_factor, "tws_battle_factor");
MEMBER_FDEF(military::single_cb, set_great_war_obligatory, "great_war_obligatory");
MEMBER_FDEF(military::single_cb, set_all_allowed_states, "all_allowed_states");
MEMBER_FDEF(military::single_cb, set_crisis, "crisis");
MEMBER_FDEF(military::single_cb, set_po_clear_union_sphere, "po_clear_union_sphere");
MEMBER_FDEF(military::single_cb, set_po_gunboat, "po_gunboat");
MEMBER_FDEF(military::single_cb, set_po_annex, "po_annex");
MEMBER_FDEF(military::single_cb, set_po_demand_state, "po_demand_state");
MEMBER_FDEF(military::single_cb, set_po_add_to_sphere, "po_add_to_sphere");
MEMBER_FDEF(military::single_cb, set_po_disarmament, "po_disarmament");
MEMBER_FDEF(military::single_cb, set_po_reparations, "po_reparations");
MEMBER_FDEF(military::single_cb, set_po_transfer_provinces, "po_transfer_provinces");
MEMBER_FDEF(military::single_cb, set_po_remove_prestige, "po_remove_prestige");
MEMBER_FDEF(military::single_cb, set_po_make_puppet, "po_make_puppet");
MEMBER_FDEF(military::single_cb, set_po_release_puppet, "po_release_puppet");
MEMBER_FDEF(military::single_cb, set_po_status_quo, "po_status_quo");
MEMBER_FDEF(military::single_cb, set_po_install_communist_gov_type, "po_install_communist_gov_type");
MEMBER_FDEF(military::single_cb, set_po_uninstall_communist_gov_type, "po_uninstall_communist_gov_type");
MEMBER_FDEF(military::single_cb, set_po_remove_cores, "po_remove_cores");
MEMBER_FDEF(military::single_cb, set_po_colony, "po_colony");
MEMBER_FDEF(military::single_cb, set_po_destroy_forts, "po_destroy_forts");
MEMBER_FDEF(military::single_cb, set_po_destroy_naval_bases, "po_destroy_naval_bases");

MEMBER_FDEF(military::cb_file, add_cb, "add_cb");
MEMBER_FDEF(military::cb_file, accept_peace_order, "peace_order");
MEMBER_FDEF(military::peace_order, add_cb, "add_cb");
//MEMBER_FDEF(military::unit_file, add_unit, "add_unit");
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

/*
MEMBER_FDEF(military::unit_build_cost, set_value, "value");
MEMBER_FDEF(military::unit_supply_cost, set_value, "value");
MEMBER_FDEF(military::unit_type_reader, discard, "discard");
MEMBER_FDEF(military::unit_type_reader, set_build_cost, "build_cost");
MEMBER_FDEF(military::unit_type_reader, set_supply_cost, "supply_cost");
MEMBER_FDEF(military::unit_type_reader, set_attack_gun_power, "attack_gun_power");
MEMBER_FDEF(military::unit_type_reader, set_maneuver_evasion, "maneuver_evasion");
MEMBER_FDEF(military::unit_type_reader, set_max_strength, "max_strength");
MEMBER_FDEF(military::unit_type_reader, set_default_organisation, "default_organisation");
MEMBER_FDEF(military::unit_type_reader, set_build_time, "build_time");
MEMBER_FDEF(military::unit_type_reader, set_maximum_speed, "maximum_speed");
MEMBER_FDEF(military::unit_type_reader, set_supply_consumption, "supply_consumption");
MEMBER_FDEF(military::unit_type_reader, set_defence_hull, "defence_hull");
MEMBER_FDEF(military::unit_type_reader, set_reconnaissance_fire_range, "reconnaissance_fire_range");
MEMBER_FDEF(military::unit_type_reader, set_discipline, "discipline");
MEMBER_FDEF(military::unit_type_reader, set_support_torpedo_attack, "support_torpedo_attack");
MEMBER_FDEF(military::unit_type_reader, set_siege, "siege");
MEMBER_FDEF(military::unit_type_reader, set_primary_culture, "primary_culture");
MEMBER_FDEF(military::unit_type_reader, set_active, "active");
MEMBER_FDEF(military::unit_type_reader, set_can_build_overseas, "can_build_overseas");
MEMBER_FDEF(military::unit_type_reader, set_sail, "sail");
MEMBER_FDEF(military::unit_type_reader, set_unit_type, "unit_type");
MEMBER_FDEF(military::unit_type_reader, set_select_sound, "select_sound");
MEMBER_FDEF(military::unit_type_reader, set_move_sound, "move_sound");
MEMBER_FDEF(military::unit_type_reader, set_icon, "icon");
MEMBER_FDEF(military::unit_type_reader, set_colonial_points, "colonial_points");
MEMBER_FDEF(military::unit_type_reader, set_naval_icon, "naval_icon");
MEMBER_FDEF(military::unit_type_reader, set_min_port_level, "min_port_level");
MEMBER_FDEF(military::unit_type_reader, set_limit_per_port, "limit_per_port");
MEMBER_FDEF(military::unit_type_reader, set_supply_consumption_score, "supply_consumption_score");
*/

namespace military {
	BEGIN_DOMAIN(war_file_domain)
		BEGIN_TYPE(war_goal_reader)
			MEMBER_ASSOCIATION("state", "state_province_id", value_from_rh<uint16_t>)
			MEMBER_ASSOCIATION("actor", "actor", token_from_rh)
			MEMBER_ASSOCIATION("receiver", "receiver", token_from_rh)
			MEMBER_ASSOCIATION("cb", "casus_belli", token_from_rh)
		END_TYPE
		BEGIN_TYPE(war_date_block)
			MEMBER_ASSOCIATION("add_attacker", "add_attacker", token_from_rh)
			MEMBER_ASSOCIATION("add_defender", "add_defender", token_from_rh)
			MEMBER_ASSOCIATION("rem_attacker", "rem_attacker", token_from_rh)
			MEMBER_ASSOCIATION("rem_defender", "rem_defender", token_from_rh)
			MEMBER_TYPE_ASSOCIATION("add_war_goal", "war_goal", war_goal_reader)
		END_TYPE
		BEGIN_TYPE(war_file)
			MEMBER_ASSOCIATION("discard", "name", discard_from_rh)
			MEMBER_VARIABLE_TYPE_ASSOCIATION("date_block", accept_all, war_date_block, date_block)
		END_TYPE
	END_DOMAIN

	BEGIN_DOMAIN(oob_domain)
		BEGIN_TYPE(parsed_relation)
			MEMBER_ASSOCIATION("value", "value", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("level", "level", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("influence_value", "influence_value", value_from_rh<int32_t>)
			MEMBER_ASSOCIATION("discard", "military_access", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(parsed_leader)
			MEMBER_ASSOCIATION("personality", "personality", token_from_rh)
			MEMBER_ASSOCIATION("background", "background", token_from_rh)
			MEMBER_ASSOCIATION("date", "date", token_from_rh)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("discard", "name", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "picture", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "prestige", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(parsed_regiment)
			MEMBER_ASSOCIATION("home", "home", value_from_rh<uint16_t>)
			MEMBER_ASSOCIATION("discard", "name", discard_from_rh)
			MEMBER_ASSOCIATION("discard", "type", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(parsed_ship)
			MEMBER_ASSOCIATION("type", "type", token_from_rh)
			MEMBER_ASSOCIATION("discard", "name", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(parsed_army_or_navy)
			MEMBER_TYPE_ASSOCIATION("leader", "leader", parsed_leader)
			MEMBER_TYPE_ASSOCIATION("ship", "ship", parsed_ship)
			MEMBER_TYPE_ASSOCIATION("regiment", "regiment", parsed_regiment)
			MEMBER_ASSOCIATION("location", "location", value_from_rh<uint16_t>)
			MEMBER_ASSOCIATION("discard", "name", discard_from_rh)
		END_TYPE
		BEGIN_TYPE(oob_file)
			MEMBER_TYPE_ASSOCIATION("leader", "leader", parsed_leader)
			MEMBER_TYPE_ASSOCIATION("army", "army", parsed_army_or_navy)
			MEMBER_TYPE_ASSOCIATION("navy", "navy", parsed_army_or_navy)
			MEMBER_VARIABLE_TYPE_ASSOCIATION("relation", accept_all, parsed_relation, name_relation)
		END_TYPE
	END_DOMAIN;

	BEGIN_DOMAIN(single_cb_domain)
		BEGIN_TYPE(single_cb)
		MEMBER_ASSOCIATION("is_civil_war", "is_civil_war", value_from_rh<bool>)
		MEMBER_ASSOCIATION("months", "months", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("truce_months", "truce_months", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("sprite_index", "sprite_index", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("always", "always", value_from_rh<bool>)
		MEMBER_ASSOCIATION("is_triggered_only", "is_triggered_only", value_from_rh<bool>)
		MEMBER_ASSOCIATION("constructing_cb", "constructing_cb", value_from_rh<bool>)
		MEMBER_TYPE_EXTERN("allowed_states", "allowed_states", triggers::trigger_tag, read_cb_state_trigger)
		MEMBER_TYPE_EXTERN("allowed_states_in_crisis", "allowed_states_in_crisis", triggers::trigger_tag, read_cb_state_trigger)
		MEMBER_TYPE_EXTERN("allowed_substate_regions", "allowed_substate_regions", triggers::trigger_tag, read_cb_state_trigger)
		MEMBER_TYPE_EXTERN("allowed_countries", "allowed_countries", triggers::trigger_tag, read_cb_nation_trigger)
		MEMBER_TYPE_EXTERN("can_use", "can_use", triggers::trigger_tag, read_cb_nation_trigger)
		MEMBER_TYPE_EXTERN("on_add", "on_add", triggers::effect_tag, read_cb_nation_effect)
		MEMBER_TYPE_EXTERN("on_po_accepted", "on_po_accepted", triggers::effect_tag, read_cb_nation_effect)
		MEMBER_ASSOCIATION("badboy_factor", "badboy_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("prestige_factor", "prestige_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("peace_cost_factor", "peace_cost_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("penalty_factor", "penalty_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("break_truce_prestige_factor", "break_truce_prestige_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("break_truce_infamy_factor", "break_truce_infamy_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("break_truce_militancy_factor", "break_truce_militancy_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("good_relation_prestige_factor", "good_relation_prestige_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("good_relation_infamy_factor", "good_relation_infamy_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("good_relation_militancy_factor", "good_relation_militancy_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("war_name", "war_name", token_from_rh)
		MEMBER_ASSOCIATION("construction_speed", "construction_speed", value_from_rh<float>)
		MEMBER_ASSOCIATION("tws_battle_factor", "tws_battle_factor", value_from_rh<float>)
		MEMBER_ASSOCIATION("great_war_obligatory", "great_war_obligatory", value_from_rh<bool>)
		MEMBER_ASSOCIATION("all_allowed_states", "all_allowed_states", value_from_rh<bool>)
		MEMBER_ASSOCIATION("crisis", "crisis", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_clear_union_sphere", "po_clear_union_sphere", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_gunboat", "po_gunboat", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_annex", "po_annex", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_demand_state", "po_demand_state", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_add_to_sphere", "po_add_to_sphere", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_disarmament", "po_disarmament", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_reparations", "po_reparations", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_transfer_provinces", "po_transfer_provinces", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_remove_prestige", "po_remove_prestige", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_make_puppet", "po_make_puppet", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_release_puppet", "po_release_puppet", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_status_quo", "po_status_quo", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_install_communist_gov_type", "po_install_communist_gov_type", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_uninstall_communist_gov_type", "po_uninstall_communist_gov_type", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_remove_cores", "po_remove_cores", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_colony", "po_colony", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_destroy_forts", "po_destroy_forts", value_from_rh<bool>)
		MEMBER_ASSOCIATION("po_destroy_naval_bases", "po_destroy_naval_bases", value_from_rh<bool>)
		MEMBER_ASSOCIATION("discard", "mutual", discard_from_rh)
		MEMBER_TYPE_EXTERN("discard", "is_valid", int, discard_section)
		END_TYPE
		END_DOMAIN;

	/*
	BEGIN_DOMAIN(unit_type_parsing)
		BEGIN_TYPE(unit_build_cost)
		MEMBER_VARIABLE_ASSOCIATION("value", accept_all, get_econ_value)
		END_TYPE
		BEGIN_TYPE(unit_supply_cost)
		MEMBER_VARIABLE_ASSOCIATION("value", accept_all, get_econ_value)
		END_TYPE
		BEGIN_TYPE(unit_type_reader)
		MEMBER_TYPE_ASSOCIATION("build_cost", "build_cost", unit_build_cost)
		MEMBER_TYPE_ASSOCIATION("supply_cost", "supply_cost", unit_supply_cost)
		MEMBER_ASSOCIATION("attack_gun_power", "attack", value_from_rh<float>)
		MEMBER_ASSOCIATION("attack_gun_power", "gun_power", value_from_rh<float>)
		MEMBER_ASSOCIATION("maneuver_evasion", "maneuver", value_from_rh<float>)
		MEMBER_ASSOCIATION("maneuver_evasion", "evasion", value_from_rh<float>)
		MEMBER_ASSOCIATION("max_strength", "max_strength", value_from_rh<float>)
		MEMBER_ASSOCIATION("default_organisation", "default_organisation", value_from_rh<float>)
		MEMBER_ASSOCIATION("default_organisation", "default_organization", value_from_rh<float>)
		MEMBER_ASSOCIATION("build_time", "build_time", value_from_rh<float>)
		MEMBER_ASSOCIATION("maximum_speed", "maximum_speed", value_from_rh<float>)
		MEMBER_ASSOCIATION("supply_consumption", "supply_consumption", value_from_rh<float>)
		MEMBER_ASSOCIATION("defence_hull", "defence", value_from_rh<float>)
		MEMBER_ASSOCIATION("defence_hull", "defense", value_from_rh<float>)
		MEMBER_ASSOCIATION("defence_hull", "hull", value_from_rh<float>)
		MEMBER_ASSOCIATION("reconnaissance_fire_range", "reconnaissance", value_from_rh<float>)
		MEMBER_ASSOCIATION("reconnaissance_fire_range", "recon", value_from_rh<float>)
		MEMBER_ASSOCIATION("reconnaissance_fire_range", "fire_range", value_from_rh<float>)
		MEMBER_ASSOCIATION("discipline", "discipline", value_from_rh<float>)
		MEMBER_ASSOCIATION("support_torpedo_attack", "support", value_from_rh<float>)
		MEMBER_ASSOCIATION("support_torpedo_attack", "torpedo_attack", value_from_rh<float>)
		MEMBER_ASSOCIATION("siege", "siege", value_from_rh<float>)
		MEMBER_ASSOCIATION("primary_culture", "primary_culture", value_from_rh<bool>)
		MEMBER_ASSOCIATION("active", "active", value_from_rh<bool>)
		MEMBER_ASSOCIATION("can_build_overseas", "can_build_overseas", value_from_rh<bool>)
		MEMBER_ASSOCIATION("sail", "sail", value_from_rh<bool>)
		MEMBER_ASSOCIATION("unit_type", "unit_type", token_from_rh)
		MEMBER_ASSOCIATION("select_sound", "select_sound", token_from_rh)
		MEMBER_ASSOCIATION("move_sound", "move_sound", token_from_rh)
		MEMBER_ASSOCIATION("icon", "icon", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("colonial_points", "colonial_points", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("naval_icon", "naval_icon", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("min_port_level", "min_port_level", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("supply_consumption_score", "supply_consumption_score", value_from_rh<uint8_t>)
		MEMBER_ASSOCIATION("limit_per_port", "limit_per_port", value_from_rh<int8_t>)
		MEMBER_ASSOCIATION("discard", "priority", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "sprite", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "type", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "capital", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "transport", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "sprite_index", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "sprite_mount", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "floating_flag", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "sprite_override", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "sprite_mount_attach_node", discard_from_rh)
		MEMBER_ASSOCIATION("discard", "weighted_value", discard_from_rh)
		END_TYPE
		END_DOMAIN;

	BEGIN_DOMAIN(unit_types_pre_parsing_domain)
		BEGIN_TYPE(unit_file)
		MEMBER_VARIABLE_TYPE_EXTERN("add_unit", accept_all, int, register_unit_type)
		END_TYPE
		END_DOMAIN;
	*/

	BEGIN_DOMAIN(cb_types_pre_parsing_domain)
		BEGIN_TYPE(cb_file)
		MEMBER_TYPE_ASSOCIATION("peace_order", "peace_order", peace_order)
		MEMBER_VARIABLE_TYPE_EXTERN("add_cb", accept_all, int, register_cb_type)
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


	void read_cb_types(parsing_state const& state, scenario::scenario_manager& s, events::event_creation_manager& ecm) {
		for(auto const& t : state.impl->pending_cb_parse) {
			cb_environment env(s, ecm, s.military_m.cb_types[std::get<0>(t)]);
			parse_object<single_cb, single_cb_domain>(std::get<1>(t), std::get<2>(t), env);
		}
		for(auto& c : s.military_m.cb_types) {
			if(!is_valid_index(c.explanation)) {
				// prevent erronious entries from bad peace_order declaration from being used
				c.flags |= (cb_type::is_not_triggered_only | cb_type::is_not_constructing_cb);
			}
		}
	}


	void read_wars(world_state& ws, date_tag target_date, const directory& root) {
		auto history_dir = root.get_directory(u"\\history");
		auto wars_dir = history_dir.get_directory(u"\\wars");

		auto dip_files = wars_dir.list_files(u".txt");
		for(auto f : dip_files) {
			if(auto open_file = f.open_file(); open_file) {
				const auto sz = open_file->size();
				std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);
				std::vector<token_group> parse_results;

				open_file->read_to_buffer(parse_data.get(), sz);
				parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

				auto result = parse_object<war_file, war_file_domain>(parse_results.data(), parse_results.data() + parse_results.size(), ws, target_date);

				if(result.attacker.size() != 0 && result.defender.size() != 0) {
					auto new_war = ws.w.military_s.wars.get_new();
					ws.set<war::primary_attacker>(new_war, result.attacker[0]);
					ws.set<war::primary_defender>(new_war, result.defender[0]);
					for(auto i : result.attacker) {
						ws.add_item(ws.get<war::attackers>(new_war), i);
						ws.add_item(ws.get<nation::wars_involved_in>(i), war_identifier{ new_war, true });
					}
					for(auto i : result.defender) {
						ws.add_item(ws.get<war::defenders>(new_war), i);
						ws.add_item(ws.get<nation::wars_involved_in>(i), war_identifier{ new_war, false });
					}
					for(auto& wg : result.war_goals) {
						ws.add_item(ws.get<war::war_goals>(new_war), wg);
					}
					for(auto& wg : result.war_goals) {
						if(wg.target_country == ws.get<war::primary_defender>(new_war)
							&& wg.from_country == ws.get<war::primary_attacker>(new_war)) {

							ws.set<war::name>(new_war, ws.s.military_m.cb_types[wg.cb_type].war_name);

							ws.set<war::first_adj>(new_war, ws.get<nation::adjective>(ws.get<war::primary_attacker>(new_war)));
							ws.set<war::second>(new_war, ws.get<nation::adjective>(ws.get<war::primary_defender>(new_war)));

							if(is_valid_index(wg.target_state))
								ws.set<war::state_name>(new_war, ws.get<state::name>(wg.target_state));

							auto& cbt = ws.s.military_m.cb_types[wg.cb_type];
							if((cbt.flags & (cb_type::po_annex | cb_type::po_make_puppet | cb_type::po_gunboat)) != 0)
								ws.set<war::second>(new_war, ws.get<nation::name>(ws.get<war::primary_defender>(new_war)));
							else if((cbt.flags & cb_type::po_liberate) != 0 && is_valid_index(wg.liberation_target))
								ws.set<war::second>(new_war, ws.get<nation::adjective>(wg.liberation_target));
							else if((cbt.flags & cb_type::po_take_from_sphere) != 0 && is_valid_index(wg.liberation_target))
								ws.set<war::second>(new_war, ws.get<nation::adjective>(wg.liberation_target));

							break;
						}
					}
					if(!is_valid_index(ws.get<war::name>(new_war))) {
						ws.set<war::name>(new_war, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, "NORMAL_WAR_NAME"));
						ws.set<war::first_adj>(new_war, ws.get<nation::adjective>(ws.get<war::primary_attacker>(new_war)));
						ws.set<war::second>(new_war, ws.get<nation::adjective>(ws.get<war::primary_defender>(new_war)));
					}
					
				}
			}
		}
	}

	/*
	void read_unit_types(
		parsing_state& state,
		military_manager& military_m,
		economy::economic_scenario& economy_m,
		sound::sound_manager& sound_m,
		text_data::text_sequences& text_m) {

		const uint32_t goods_count = economy_m.aligned_32_goods_count;
		military_m.unit_build_costs.reset(goods_count);
		military_m.unit_base_supply_costs.reset(goods_count);

		const auto unit_type_count = military_m.unit_types.size();
		military_m.unit_build_costs.resize(unit_type_count);
		military_m.unit_base_supply_costs.resize(unit_type_count);

		for(const auto& t : state.impl->pending_unit_parse) {
			auto unit_tag = std::get<0>(t);
			unit_type_env env(military_m, economy_m, sound_m, text_m, military_m.unit_types[unit_tag]);
			parse_object<unit_type_reader, unit_type_parsing>(std::get<1>(t), std::get<2>(t), env);
		}
	}

	void pre_parse_unit_types(
		parsing_state& state,
		const directory& source_directory) {

		parsing_environment& env = *(state.impl);

		static const char army_base[] = "army_base";
		static const char navy_base[] = "navy_base";

		env.manager.unit_types.emplace_back();
		env.manager.unit_types.emplace_back();

		const auto army_base_name = text_data::get_thread_safe_text_handle(env.text_lookup, army_base, army_base + sizeof(army_base) - 1);
		const auto navy_base_name = text_data::get_thread_safe_text_handle(env.text_lookup, navy_base, navy_base + sizeof(navy_base) - 1);

		env.manager.named_unit_type_index.emplace(army_base_name, army_unit_base);
		env.manager.named_unit_type_index.emplace(navy_base_name, naval_unit_base);

		env.manager.unit_types[army_unit_base].id = army_unit_base;
		env.manager.unit_types[naval_unit_base].id = unit_type_tag(1);
		env.manager.unit_types[army_unit_base].name = army_base_name;
		env.manager.unit_types[naval_unit_base].name = navy_base_name;
		env.manager.unit_types[army_unit_base].base_attributes[unit_attribute::enabled] = unit_attribute_type(0);
		env.manager.unit_types[naval_unit_base].base_attributes[unit_attribute::enabled] = unit_attribute_type(0);

		const auto unit_dir = source_directory.get_directory(u"\\units");
		const auto unit_files = unit_dir.list_files(u".txt");

		for(const auto& f : unit_files) {
			const auto fi = f.open_file();
			if(fi) {
				env.unit_type_files.emplace_back();
				auto& main_results = state.impl->unit_type_files.back();

				const auto sz = fi->size();
				main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

				fi->read_to_buffer(main_results.parse_data.get(), sz);
				parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

				if(main_results.parse_results.size() > 0) {
					parse_object<unit_file, unit_types_pre_parsing_domain>(
						main_results.parse_results.data(),
						main_results.parse_results.data() + main_results.parse_results.size(),
						env);
				}
			}
		}

		env.manager.unit_types_count = uint32_t(env.manager.unit_types.size());
	}
	*/

	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		auto& main_results = state.impl->cb_file;

		const auto fi = common_dir.open_file(u"cb_types.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);
			parse_pdx_file(main_results.parse_results, main_results.parse_data.get(), main_results.parse_data.get() + sz);

			if(main_results.parse_results.size() > 0) {
				parse_object<cb_file, cb_types_pre_parsing_domain>(
					&main_results.parse_results[0],
					&main_results.parse_results[0] + main_results.parse_results.size(),
					*state.impl);
			}
		}
	}

	void read_leader_traits(parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		const auto fi = common_dir.open_file(u"traits.txt");

		state.impl->manager.leader_trait_definitions.reset(traits::trait_count);
		state.impl->manager.leader_trait_definitions.resize(2);
		state.impl->manager.leader_traits.resize(2);

		{
			const static char no_personality_string[] = "no_personality";
			const auto trait_name = text_data::get_thread_safe_text_handle(state.impl->text_lookup, no_personality_string, no_personality_string + sizeof(no_personality_string) - 1);

			state.impl->manager.named_leader_trait_index.emplace(trait_name, state.impl->manager.no_personality_trait);
			const auto no_personality = state.impl->manager.no_personality_trait;
			state.impl->manager.leader_traits[no_personality] = trait_name;
		}
		{
			const static char no_background_string[] = "no_background";
			const auto trait_name = text_data::get_thread_safe_text_handle(state.impl->text_lookup, no_background_string, no_background_string + sizeof(no_background_string) - 1);

			state.impl->manager.named_leader_trait_index.emplace(trait_name, state.impl->manager.no_background_trait);
			const auto no_background = state.impl->manager.no_background_trait;
			state.impl->manager.leader_traits[no_background] = trait_name;
		}

		if(fi) {
			const auto sz = fi->size();
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);
			std::vector<token_group> parse_results;
			fi->read_to_buffer(parse_data.get(), sz);
			parse_pdx_file(parse_results, parse_data.get(), parse_data.get() + sz);

			if(parse_results.size() > 0) {
				parse_object<traits_file, traits_parsing_domain>(
					&parse_results[0],
					&parse_results[0] + parse_results.size(),
					*state.impl);
			}
		}
	}

	void read_oob_file(world_state& ws, nations::country_tag for_nation, token_group const* start, token_group const* end) {
		parse_object<oob_file, oob_domain>(start, end, ws, for_nation);
	}
}
