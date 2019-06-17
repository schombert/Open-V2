#pragma once
#include "military_io.hpp"
#include "nations\nations_functions.hpp"

namespace military {
	struct war_goal_reader {
		war_goal under_construction;

		war_goal_reader() {}

		template<typename ERR, typename WS>
		void set_actor(association_type, token_and_type const& t, ERR const&, WS& ws) {
			under_construction.from_country = nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end)));
		}
		template<typename ERR, typename WS>
		void set_receiver(association_type, token_and_type const& t, ERR const&, WS& ws) {
			under_construction.target_country = nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end)));
		}
		template<typename ERR, typename WS>
		void set_state(association_type, uint16_t v, ERR const&, WS& ws) {
			auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(provinces::province_tag(v));
			if(is_valid_index(state))
				under_construction.target_state = state;
		}
		template<typename ERR, typename WS>
		void set_casus_belli(association_type, token_and_type const& t, ERR const&, WS& ws) {
			under_construction.cb_type = tag_from_text(ws.s.military_m.named_cb_type_index, text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, t.start, t.end));
		}

	};

	struct war_date_block {
		std::vector<nations::country_tag> attacker;
		std::vector<nations::country_tag> defender;
		std::vector<nations::country_tag> rem_attacker;
		std::vector<nations::country_tag> rem_defender;
		std::vector<war_goal> war_goals;

		war_date_block() {}

		template<typename ERR, typename WS>
		void add_attacker(association_type, token_and_type const& t, ERR const&, WS& ws) {
			attacker.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		template<typename ERR, typename WS>
		void add_defender(association_type, token_and_type const& t, ERR const&, WS& ws) {
			defender.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		template<typename ERR, typename WS>
		void remove_attacker(association_type, token_and_type const& t, ERR const&, WS& ws) {
			rem_attacker.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		template<typename ERR, typename WS>
		void remove_defender(association_type, token_and_type const& t, ERR const&, WS& ws) {
			rem_defender.push_back(nations::make_nation_for_tag(ws, tag_from_text(ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(t.start, t.end))));
		}
		template<typename ERR, typename WS>
		void add_war_goal(war_goal_reader const& wg, ERR const&, WS& ws) {
			war_goals.push_back(wg.under_construction);
		}
	};

	struct war_file {
		std::vector<nations::country_tag> attacker;
		std::vector<nations::country_tag> defender;
		std::vector<war_goal> war_goals;

		war_file() {}

		template<typename ERR, typename WS>
		void add_date_block(token_and_type const& rdate, war_date_block const& block, ERR& err, WS& ws) {
			auto block_date = token_to<date_tag>(rdate, err);

			if(block_date <= ws.w.current_date) {
				for(auto i : block.attacker)
					attacker.push_back(i);
				for(auto i : block.defender)
					defender.push_back(i);
				for(auto i : block.rem_attacker) {
					for(int32_t j = int32_t(attacker.size()) - 1; j >= 0; --j) {
						if(attacker[uint32_t(j)] == i) {
							attacker[uint32_t(j)] = attacker.back();
							attacker.pop_back();
						}
					}
				}
				for(auto i : block.rem_defender) {
					for(int32_t j = int32_t(defender.size()) - 1; j >= 0; --j) {
						if(defender[uint32_t(j)] == i) {
							defender[uint32_t(j)] = defender.back();
							defender.pop_back();
						}
					}
				}
				for(auto& i : block.war_goals) {
					war_goals.push_back(i);
					war_goals.back().date_added = block_date;
				}
			}
		}
	};

	struct parsed_relation {
		int32_t value = 0;
		int32_t level = 2;
		int32_t influence_value = 0;
	};


	struct parsed_leader {
		bool is_general = true;
		date_tag creation_date;
		leader_trait_tag personality;
		leader_trait_tag background;

		parsed_leader() {}

		template<typename ERR, typename WS>
		void set_type(association_type, token_and_type const& t, ERR& err, WS& ws) {
			if(is_fixed_token_ci(t, "land"))
				is_general = true;
			else
				is_general = false;
		}
		template<typename ERR, typename WS>
		void set_personality(association_type, token_and_type const& t, ERR& err, WS& oob_s) {
			personality = tag_from_text(oob_s.ws.s.military_m.named_leader_trait_index, text_data::get_thread_safe_existing_text_handle(oob_s.ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
		template<typename ERR, typename WS>
		void set_background(association_type, token_and_type const& t, ERR& err, WS& oob_s) {
			background = tag_from_text(oob_s.ws.s.military_m.named_leader_trait_index, text_data::get_thread_safe_existing_text_handle(oob_s.ws.s.gui_m.text_data_sequences, t.start, t.end));
		}
	};

	struct parsed_regiment {
	};

	struct parsed_ship {
	};

	template<typename WS_TYPE>
	struct oob_constext {
		WS_TYPE& ws;
		nations::country_tag this_nation;
	};

	struct parsed_army_or_navy {
		leader_tag set_leader;
		provinces::province_tag location;

		// std::vector<provinces::province_tag> regiment_sources;
		// std::vector<unit_type_tag> ship_types;

		int32_t ship_count = 0;
		int32_t regiment_count = 0;

		parsed_army_or_navy() {}

		template<typename ERR, typename WS>
		void add_leader(parsed_leader const& l, ERR& err, WS& oob_s) {
			auto new_leader = make_empty_leader(oob_s.ws, oob_s.ws.w.nation_s.nations.get<nation::primary_culture>(oob_s.this_nation), l.is_general);
			oob_s.ws.set<military_leader::background>(new_leader, l.background);
			oob_s.ws.set<military_leader::personality>(new_leader, l.personality);
			oob_s.ws.set<military_leader::creation_date>(new_leader, l.creation_date);

			calculate_leader_traits(oob_s.ws, new_leader);

			if(l.is_general)
				oob_s.ws.add_item(oob_s.ws.get<nation::generals>(oob_s.this_nation), new_leader);
			else
				oob_s.ws.add_item(oob_s.ws.get<nation::admirals>(oob_s.this_nation), new_leader);

			set_leader = new_leader;
		}
		template<typename ERR, typename WS>
		void set_location(association_type, uint16_t p, ERR& err, WS& oob_s) {
			location = provinces::province_tag(p);
		}
		template<typename ERR, typename WS>
		void add_regiment(parsed_regiment const & r, ERR& err, WS& oob_s) {
			++regiment_count;
			// regiment_sources.push_back(r.home);
		}
		template<typename ERR, typename WS>
		void add_ship(parsed_ship const & s, ERR& err, WS& oob_s) {
			++ship_count;
			// ship_types.push_back(s.type);
		}
	};

	struct oob_file {

		oob_file() {}

		template<typename ERR, typename WS>
		void add_relation(token_and_type const& name, parsed_relation const& p, ERR& err, WS& oob_s) {
			auto other_tag = tag_from_text(oob_s.ws.s.culture_m.national_tags_index, cultures::tag_to_encoding(name.start, name.end));
			auto other_nation = nations::make_nation_for_tag(oob_s.ws, other_tag);
			if(p.value != 0)
				nations::set_relationship(oob_s.ws, oob_s.this_nation, other_nation, p.value);
			if(p.influence_value != 0 || p.level != 2)
				nations::set_influence(oob_s.ws, oob_s.this_nation, other_nation, float(p.influence_value), p.level);
		}
		template<typename ERR, typename WS>
		void add_leader(parsed_leader const& l, ERR& err, WS& oob_s) {
			auto new_leader = make_empty_leader(oob_s.ws, oob_s.ws.w.nation_s.nations.get<nation::primary_culture>(oob_s.this_nation), l.is_general);
			oob_s.ws.set<military_leader::background>(new_leader, l.background);
			oob_s.ws.set<military_leader::personality>(new_leader, l.personality);
			oob_s.ws.set<military_leader::creation_date>(new_leader, l.creation_date);

			calculate_leader_traits(oob_s.ws, new_leader);

			if(l.is_general)
				oob_s.ws.add_item(oob_s.ws.get<nation::generals>(oob_s.this_nation), new_leader);
			else
				oob_s.ws.add_item(oob_s.ws.get<nation::admirals>(oob_s.this_nation), new_leader);
		}
		template<typename ERR, typename WS>
		void add_army(parsed_army_or_navy&& a, ERR& err, WS& oob_s) {
			if(!is_valid_index(a.location))
				a.location = oob_s.ws.get<nation::current_capital>(oob_s.this_nation);

			auto new_army = make_army(oob_s.ws, oob_s.this_nation, a.location);
			if(is_valid_index(a.set_leader)) {
				oob_s.ws.set<army::leader>(new_army, a.set_leader);
				oob_s.ws.set<military_leader::is_attached>(a.set_leader, true);
			}

			oob_s.ws.set<army::target_soldiers>(new_army, 1000.0f * a.regiment_count);
		}
		template<typename ERR, typename WS>
		void add_navy(parsed_army_or_navy const& n, ERR& err, WS& oob_s) {
			auto new_fleet = make_fleet(oob_s.ws, oob_s.this_nation, n.location);
			if(is_valid_index(n.set_leader)) {
				oob_s.ws.set<fleet::leader>(new_fleet, n.set_leader);
				oob_s.ws.set<military_leader::is_attached>(n.set_leader, true);
			}
			oob_s.ws.set<fleet::size>(new_fleet, float(n.ship_count));
		}
	};

	template<typename SCENARIO>
	struct parsing_environment {
		SCENARIO& s;
		events::event_creation_manager& ecm;
		unparsed_data cb_file;

		text_data::text_tag lib_name_a;
		text_data::text_tag lib_name_b;
		text_data::text_tag take_from_sphere;

		std::vector<std::pair<cb_type_tag, text_range>> pending_cb_parse;

		parsing_environment(SCENARIO& m, events::event_creation_manager& e) : s(m), ecm(e) {
			lib_name_a = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "free_peoples");
			lib_name_b = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "liberate_country");
			take_from_sphere = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, "take_from_sphere");
		}
	};


	template<typename ERR, typename S>
	inline triggers::trigger_tag read_cb_state_trigger(token_generator& gen, ERR const& err, S& s) {
		auto rng = find_group_range(gen, err, s);

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, rng.start, rng.end);

		const auto td = triggers::parse_trigger(s.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::state,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation }, parse_results.data(), parse_results.data() + parse_results.size());
		return triggers::commit_trigger(s.s.trigger_m, td);
	}
	template<typename ERR, typename S>
	inline triggers::trigger_tag read_cb_nation_trigger(token_generator& gen, ERR const& err, S& s) {
		auto rng = find_group_range(gen, err, s);

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, rng.start, rng.end);

		const auto td = triggers::parse_trigger(s.s,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation }, parse_results.data(), parse_results.data() + parse_results.size());
		return triggers::commit_trigger(s.s.trigger_m, td);
	}
	template<typename ERR, typename S>
	inline triggers::effect_tag read_cb_nation_effect(token_generator& gen, ERR const& err, S& s) {
		auto rng = find_group_range(gen, err, s);

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, rng.start, rng.end);

		const auto td = triggers::parse_effect(s.s, s.ecm,
			triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation }, parse_results.data(), parse_results.data() + parse_results.size());
		return triggers::commit_effect(s.s.trigger_m, td);
	}

	struct single_cb : public military::cb_type {
		
		single_cb() {
		}
		template<typename ERR, typename WS>
		void set_is_civil_war(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::is_civil_war;
		}
		template<typename ERR, typename WS>
		void set_always(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::always;
		}
		template<typename ERR, typename WS>
		void set_is_triggered_only(association_type, bool v, ERR& err, WS& s) {
			if(!v) flags |= cb_type::is_not_triggered_only;
		}
		template<typename ERR, typename WS>
		void set_constructing_cb(association_type, bool v, ERR& err, WS& s) {
			if(!v) flags |= cb_type::is_not_constructing_cb;
		}
		template<typename ERR, typename WS>
		void set_war_name(association_type, token_and_type const& t, ERR& err, WS& s) {
			std::string prepended = std::string("NORMAL_") + std::string(t.start, t.end);
			war_name = text_data::get_thread_safe_text_handle(s.s.gui_m.text_data_sequences, prepended.c_str(), prepended.c_str() + prepended.length());
		}
		template<typename ERR, typename WS>
		void set_great_war_obligatory(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::great_war_obligatory;
		}
		template<typename ERR, typename WS>
		void set_all_allowed_states(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::all_allowed_states;
		}
		template<typename ERR, typename WS>
		void set_crisis(association_type, bool v, ERR& err, WS& s) {
			if(!v) flags |= cb_type::not_in_crisis;
		}
		template<typename ERR, typename WS>
		void set_po_clear_union_sphere(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_clear_union_sphere;
		}
		template<typename ERR, typename WS>
		void set_po_gunboat(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_gunboat;
		}
		template<typename ERR, typename WS>
		void set_po_annex(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_annex;
		}
		template<typename ERR, typename WS>
		void set_po_demand_state(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_demand_state;
		}
		template<typename ERR, typename WS>
		void set_po_add_to_sphere(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_add_to_sphere;
		}
		template<typename ERR, typename WS>
		void set_po_disarmament(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_disarmament;
		}
		template<typename ERR, typename WS>
		void set_po_reparations(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_reparations;
		}
		template<typename ERR, typename WS>
		void set_po_transfer_provinces(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_transfer_provinces;
		}
		template<typename ERR, typename WS>
		void set_po_remove_prestige(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_remove_prestige;
		}
		template<typename ERR, typename WS>
		void set_po_make_puppet(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_make_puppet;
		}
		template<typename ERR, typename WS>
		void set_po_release_puppet(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_release_puppet;
		}
		template<typename ERR, typename WS>
		void set_po_status_quo(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_status_quo;
		}
		template<typename ERR, typename WS>
		void set_po_install_communist_gov_type(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_install_communist_gov_type;
		}
		template<typename ERR, typename WS>
		void set_po_uninstall_communist_gov_type(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_uninstall_communist_gov_type;
		}
		template<typename ERR, typename WS>
		void set_po_remove_cores(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_remove_cores;
		}
		template<typename ERR, typename WS>
		void set_po_colony(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_colony;
		}
		template<typename ERR, typename WS>
		void set_po_destroy_forts(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_destroy_forts;
		}
		template<typename ERR, typename WS>
		void set_po_destroy_naval_bases(association_type, bool v, ERR& err, WS& s) {
			if(v) flags |= cb_type::po_destroy_naval_bases;
		}
	};


	struct peace_order {
		template<typename ERR, typename WS>
		void add_cb(const token_and_type& t, ERR& err, WS& s) {
			const auto name = text_data::get_thread_safe_text_handle(s.s.gui_m.text_data_sequences, t.start, t.end);
			if(0 == s.s.military_m.named_cb_type_index.count(name)) {
				const auto cbid = s.s.military_m.cb_types.emplace_back();
				s.s.military_m.cb_types[cbid].id = cbid;
				s.s.military_m.cb_types[cbid].name = name;

				s.s.military_m.named_cb_type_index.emplace(name, cbid);
			}
		}
	};

	struct cb_file {
		template<typename ERR, typename WS>
		void peace_order(const peace_order&, ERR& err, WS& s) {}
		template<typename ERR, typename WS>
		void reserve_cb(token_and_type const& t, text_range r, ERR& err, WS& s) {
			const auto name = text_data::get_thread_safe_text_handle(s.s.gui_m.text_data_sequences, t.start, t.end);
			const auto cbtag = tag_from_text(s.s.military_m.named_cb_type_index, name);
			if(!is_valid_index(cbtag)) {
				auto const cbid = s.s.military_m.cb_types.emplace_back();
				s.s.military_m.cb_types[cbid].id = cbid;
				s.s.military_m.cb_types[cbid].name = name;
				s.s.military_m.named_cb_type_index.emplace(name, cbid);

				std::string setup_name = std::string(t.start, t.end) + std::string("_setup");
				s.s.military_m.cb_types[cbid].explanation = text_data::get_thread_safe_text_handle(s.s.gui_m.text_data_sequences, setup_name.c_str(), setup_name.c_str() + setup_name.length());

				if((name == s.lib_name_a) | (name == s.lib_name_b))
					s.s.military_m.cb_types[cbid].flags |= cb_type::po_liberate;
				if(name == s.take_from_sphere)
					s.s.military_m.cb_types[cbid].flags |= cb_type::po_take_from_sphere;

				s.pending_cb_parse.emplace_back(cbid, r);
			} else {

				std::string setup_name = std::string(t.start, t.end) + std::string("_setup");
				s.s.military_m.cb_types[cbtag].explanation = text_data::get_thread_safe_text_handle(s.s.gui_m.text_data_sequences, setup_name.c_str(), setup_name.c_str() + setup_name.length());

				if((name == s.lib_name_a) | (name == s.lib_name_b))
					s.s.military_m.cb_types[cbtag].flags |= cb_type::po_liberate;
				if(name == s.take_from_sphere)
					s.s.military_m.cb_types[cbtag].flags |= cb_type::po_take_from_sphere;

				s.pending_cb_parse.emplace_back(cbtag, r);
			}
		}
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
		template<typename ERR, typename WS>
		void add_trait(token_and_type const& name, trait const& t, ERR const&, WS& s) {
			const auto new_trait = add_trait_to_manager(s.military_m, t);
			const auto trait_name = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, name.start, name.end);
			s.military_m.leader_traits.safe_get(new_trait) = trait_name;

			s.military_m.personality_traits.push_back(new_trait);
			s.military_m.named_leader_trait_index.emplace(trait_name, new_trait);
		}
		template<typename ERR, typename WS>
		void set_no_personality(const trait& t, ERR const&, WS& s) {
			add_trait_to_manager(s.military_m, t, s.military_m.no_personality_trait);
		}
	};
	struct backgrounds {
		template<typename ERR, typename WS>
		void add_trait(token_and_type const& name, trait const& t, ERR const&, WS& s) {
			const auto new_trait = add_trait_to_manager(s.military_m, t);
			const auto trait_name = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, name.start, name.end);
			s.military_m.leader_traits.safe_get(new_trait) = trait_name;

			s.military_m.background_traits.push_back(new_trait);
			s.military_m.named_leader_trait_index.emplace(trait_name, new_trait);
		}
		template<typename ERR, typename WS>
		void set_no_background(const trait& t, ERR const&, WS& s) {
			add_trait_to_manager(s.military_m, t, s.military_m.no_background_trait);
		}
	};

	struct traits_file {
		template<typename ERR, typename WS>
		void add_personalities(const personalities&, ERR const&, WS&) {}
		template<typename ERR, typename WS>
		void add_backgrounds(const backgrounds&, ERR const&, WS&) {}
	};

#include "military_parsing.h"
}
