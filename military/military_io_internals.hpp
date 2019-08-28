#pragma once
#include "military_io.hpp"
#include "nations\nations_functions.hpp"
#include "nations\nations_internals.hpp"

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
		leader_trait_tag personality = military::no_personality_trait;
		leader_trait_tag background = military::no_background_trait;

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
			personality = tag_from_text(oob_s.ws.name_map<leader_trait_tag>(), oob_s.ws.get_thread_safe_existing_text_handle(t.start, t.end));
			assert(is_valid_index(personality));
		}
		template<typename ERR, typename WS>
		void set_background(association_type, token_and_type const& t, ERR& err, WS& oob_s) {
			background = tag_from_text(oob_s.ws.name_map<leader_trait_tag>(), oob_s.ws.get_thread_safe_existing_text_handle(t.start, t.end));
			assert(is_valid_index(background));
		}
	};

	struct parsed_regiment {
	};

	struct parsed_ship {
	};

	template<typename WS_TYPE, auto make_tag_holder_fn, auto make_army_fn, auto make_fleet_fn, auto make_leader_fn, auto calculate_traits_fn>
	struct oob_context {
		WS_TYPE& ws;
		nations::country_tag this_nation;

		static constexpr auto make_tag_holder = make_tag_holder_fn;
		static constexpr auto make_army = make_army_fn;
		static constexpr auto make_fleet = make_fleet_fn;
		static constexpr auto make_empty_leader = make_leader_fn;
		static constexpr auto calculate_leader_traits = calculate_traits_fn;
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
			auto new_leader = oob_s.make_empty_leader(oob_s.ws, oob_s.ws.get<nation::primary_culture>(oob_s.this_nation), l.is_general);
			oob_s.ws.set<military_leader::background>(new_leader, l.background);
			oob_s.ws.set<military_leader::personality>(new_leader, l.personality);
			oob_s.ws.set<military_leader::creation_date>(new_leader, l.creation_date);

			oob_s.calculate_leader_traits(oob_s.ws, new_leader);

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
			auto other_tag = tag_from_text(oob_s.ws.name_map<cultures::national_tag>(), cultures::tag_to_encoding(name.start, name.end));
			auto other_nation = oob_s.make_tag_holder(oob_s.ws, other_tag);  //nations::make_nation_for_tag(oob_s.ws, other_tag);
			if(p.value != 0)
				nations::internal_set_relationship(oob_s.ws, oob_s.this_nation, other_nation, p.value);
			if(p.influence_value != 0 || p.level != 2)
				nations::internal_set_influence(oob_s.ws, oob_s.this_nation, other_nation, float(p.influence_value), p.level);
		}
		template<typename ERR, typename WS>
		void add_leader(parsed_leader const& l, ERR& err, WS& oob_s) {
			auto new_leader = oob_s.make_empty_leader(oob_s.ws, oob_s.ws.get<nation::primary_culture>(oob_s.this_nation), l.is_general);
			oob_s.ws.set<military_leader::background>(new_leader, l.background);
			oob_s.ws.set<military_leader::personality>(new_leader, l.personality);
			oob_s.ws.set<military_leader::creation_date>(new_leader, l.creation_date);

			oob_s.calculate_leader_traits(oob_s.ws, new_leader);

			if(l.is_general)
				oob_s.ws.add_item(oob_s.ws.get<nation::generals>(oob_s.this_nation), new_leader);
			else
				oob_s.ws.add_item(oob_s.ws.get<nation::admirals>(oob_s.this_nation), new_leader);
		}
		template<typename ERR, typename WS>
		void add_army(parsed_army_or_navy&& a, ERR& err, WS& oob_s) {
			if(!is_valid_index(a.location))
				a.location = oob_s.ws.get<nation::current_capital>(oob_s.this_nation);

			auto new_army = oob_s.make_army(oob_s.ws, oob_s.this_nation, a.location);
			if(is_valid_index(a.set_leader)) {
				oob_s.ws.set<army::leader>(new_army, a.set_leader);
				oob_s.ws.set<military_leader::is_attached>(a.set_leader, true);
			}

			oob_s.ws.set<army::target_soldiers>(new_army, 1000.0f * a.regiment_count);
		}
		template<typename ERR, typename WS>
		void add_navy(parsed_army_or_navy const& n, ERR& err, WS& oob_s) {
			auto new_fleet = oob_s.make_fleet(oob_s.ws, oob_s.this_nation, n.location);
			if(is_valid_index(n.set_leader)) {
				oob_s.ws.set<fleet::leader>(new_fleet, n.set_leader);
				oob_s.ws.set<military_leader::is_attached>(n.set_leader, true);
			}
			oob_s.ws.set<fleet::size>(new_fleet, float(n.ship_count));
		}
	};


	template<typename ERR, typename S>
	inline triggers::trigger_tag read_cb_state_trigger(token_generator& gen, ERR& err, S& s) {
		return s.internal_read_trigger_fn(gen, s.s, err, triggers::trigger_scope_state{
				triggers::trigger_slot_contents::state,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation });
	}
	template<typename ERR, typename S>
	inline triggers::trigger_tag read_cb_nation_trigger(token_generator& gen, ERR& err, S& s) {
		return s.internal_read_trigger_fn(gen, s.s, err, triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation });
	}
	template<typename ERR, typename S>
	inline triggers::effect_tag read_cb_nation_effect(token_generator& gen, ERR& err, S& s) {
		return s.internal_read_effect_fn(gen, s.s, s.ecm, err, triggers::trigger_scope_state{
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation,
				triggers::trigger_slot_contents::nation });
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
			war_name = s.s.get_thread_safe_text_handle(prepended.c_str(), prepended.c_str() + prepended.length());
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
			const auto name = s.s.get_thread_safe_text_handle(t.start, t.end);
			if(0 == s.s.name_map<cb_type_tag>().count(name)) {
				auto const cbid = s.internal_new_cb_fn(s.s);
				s.s.set<::cb_type::id>(cbid, cbid);
				s.s.set<::cb_type::name>(cbid, name);

				s.s.name_map<cb_type_tag>().emplace(name, cbid);
			}
		}
	};

	struct cb_file {
		template<typename ERR, typename WS>
		void handle_peace_order(const peace_order&, ERR& err, WS& s) {}

		template<typename ERR, typename WS>
		void reserve_cb(token_and_type const& t, text_range r, ERR& err, WS& s) {
			const auto name = s.s.get_thread_safe_text_handle(t.start, t.end);
			const auto cbtag = tag_from_text(s.s.name_map<military::cb_type_tag>(), name);
			if(!is_valid_index(cbtag)) {
				auto const cbid = s.internal_new_cb_fn(s.s);
				s.s.set<::cb_type::id>(cbid, cbid);
				s.s.set<::cb_type::name>(cbid, name);
				s.s.name_map<military::cb_type_tag>().emplace(name, cbid);

				std::string setup_name = std::string(t.start, t.end) + std::string("_setup");
				s.s.set<::cb_type::explanation>(cbid, s.s.get_thread_safe_text_handle(setup_name.c_str(), setup_name.c_str() + setup_name.length()));

				if((name == s.lib_name_a) | (name == s.lib_name_b))
					s.s.get<::cb_type::flags>(cbid) |= cb_type::po_liberate;
				if(name == s.take_from_sphere)
					s.s.get<::cb_type::flags>(cbid) |= cb_type::po_take_from_sphere;

				s.pending_cb_parse.emplace_back(cbid, r);
			} else {

				std::string setup_name = std::string(t.start, t.end) + std::string("_setup");
				s.s.set<::cb_type::explanation>(cbtag, s.s.get_thread_safe_text_handle(setup_name.c_str(), setup_name.c_str() + setup_name.length()));

				if((name == s.lib_name_a) | (name == s.lib_name_b))
					s.s.get<::cb_type::flags>(cbtag) |= cb_type::po_liberate;
				if(name == s.take_from_sphere)
					s.s.get<::cb_type::flags>(cbtag) |= cb_type::po_take_from_sphere;

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


	template<auto safe_row_fn, typename W>
	inline void add_trait_to_manager(W& m, const trait& t, leader_trait_tag id) {
		auto row_ptr = safe_row_fn(m, id);
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
			const auto current_size = s.s.size<military::leader_trait_name>();
			const leader_trait_tag new_trait(static_cast<value_base_of<leader_trait_tag>>(current_size));

			add_trait_to_manager<s.internal_safe_row_fn>(s.s, t, new_trait);

			const auto trait_name = s.s.get_thread_safe_text_handle(name.start, name.end);
			s.s.as_vector<military::leader_trait_name>().push_back(trait_name);

			s.s.as_vector<military::personality_traits>().push_back(new_trait);
			s.s.name_map<leader_trait_tag>().emplace(trait_name, new_trait);
		}
		template<typename ERR, typename WS>
		void set_no_personality(const trait& t, ERR const&, WS& s) {
			add_trait_to_manager<s.internal_safe_row_fn>(s.s, t, military::no_personality_trait);
		}
	};
	struct backgrounds {
		template<typename ERR, typename WS>
		void add_trait(token_and_type const& name, trait const& t, ERR const&, WS& s) {
			const auto current_size = s.s.size<military::leader_trait_name>();
			const leader_trait_tag new_trait(static_cast<value_base_of<leader_trait_tag>>(current_size));

			add_trait_to_manager<s.internal_safe_row_fn>(s.s, t, new_trait);

			const auto trait_name = s.s.get_thread_safe_text_handle(name.start, name.end);
			s.s.as_vector<military::leader_trait_name>().push_back(trait_name);

			s.s.as_vector<military::background_traits>().push_back(new_trait);
			s.s.name_map<leader_trait_tag>().emplace(trait_name, new_trait);
		}
		template<typename ERR, typename WS>
		void set_no_background(const trait& t, ERR const&, WS& s) {
			add_trait_to_manager<s.internal_safe_row_fn>(s.s, t, military::no_background_trait);
		}
	};

	struct traits_file {
		template<typename ERR, typename WS>
		void add_personalities(const personalities&, ERR const&, WS&) {}
		template<typename ERR, typename WS>
		void add_backgrounds(const backgrounds&, ERR const&, WS&) {}
	};

#include "military_parsing.h"

	template<typename world_state_t>
	void internal_read_wars(world_state_t& ws, date_tag target_date, const directory& root) {
		auto history_dir = root.get_directory(u"\\history");
		auto wars_dir = history_dir.get_directory(u"\\wars");

		auto dip_files = wars_dir.list_files(u".txt");
		for(auto f : dip_files) {
			if(auto open_file = f.open_file(); open_file) {
				const auto sz = open_file->size();
				std::unique_ptr<char[]> parse_data = std::unique_ptr<char[]>(new char[sz]);


				open_file->read_to_buffer(parse_data.get(), sz);

				token_generator gen(parse_data.get(), parse_data.get() + sz);
				empty_error_handler err;

				auto old_date = ws.w.current_date;
				ws.w.current_date = target_date;
				auto result = military::military_parsing::parse_war_file(gen, err, ws);
				ws.w.current_date = old_date;

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


	template<typename PSTATE>
	void internal_pre_parse_cb_types(
		PSTATE& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		auto& main_results = state.cb_file;

		const auto fi = common_dir.open_file(u"cb_types.txt");

		if(fi) {
			const auto sz = fi->size();
			main_results.parse_data = std::unique_ptr<char[]>(new char[sz]);

			fi->read_to_buffer(main_results.parse_data.get(), sz);

			token_generator gen(main_results.parse_data.get(), main_results.parse_data.get() + sz);
			empty_error_handler err;

			military::military_parsing::parse_cb_file(gen, err, state);
		}
	}

	template<auto resize_trait_count_fn, typename PSTATE>
	void internal_read_leader_traits(PSTATE& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		const auto fi = common_dir.open_file(u"traits.txt");

		//state.s.military_m.leader_trait_definitions.reset(traits::trait_count);
		resize_trait_count_fn(state.s, 2);
		state.s.resize<military::leader_trait_name>(2);

		{
			const static char no_personality_string[] = "no_personality";
			const auto trait_name = state.s.get_thread_safe_text_handle(no_personality_string, no_personality_string + sizeof(no_personality_string) - 1);

			state.s.name_map<leader_trait_tag>().emplace(trait_name, military::no_personality_trait);
			state.s.set<military::leader_trait_name>(military::no_personality_trait, trait_name);
		}
		{
			const static char no_background_string[] = "no_background";
			const auto trait_name = state.s.get_thread_safe_text_handle(no_background_string, no_background_string + sizeof(no_background_string) - 1);

			state.s.name_map<leader_trait_tag>().emplace(trait_name, military::no_background_trait);
			state.s.set<military::leader_trait_name>(military::no_background_trait, trait_name);
		}

		if(fi) {
			const auto sz = fi->size();
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);
			fi->read_to_buffer(parse_data.get(), sz);

			token_generator gen(parse_data.get(), parse_data.get() + sz);
			empty_error_handler err;

			military::military_parsing::parse_traits_file(gen, err, state);
		}
	}

	template<auto make_tag_holder_fn, auto make_army_fn, auto make_fleet_fn, auto make_empty_leader_fn, auto calculate_leader_traits_fn, typename world_state_t>
	void internal_read_oob_file(world_state_t& ws, nations::country_tag for_nation, token_generator& gen) {
		empty_error_handler err;
		oob_context<world_state_t, make_tag_holder_fn, make_army_fn, make_fleet_fn, make_empty_leader_fn, calculate_leader_traits_fn> con{ ws, for_nation };
		military::military_parsing::parse_oob_file(gen, err, con);
	}

	template<typename PSTATE>
	void internal_read_cb_types(PSTATE const& state) {
		for(auto const& t : state.pending_cb_parse) {
			token_generator gen(t.second.start, t.second.end);
			empty_error_handler err;
			auto cb = military::military_parsing::parse_single_cb(gen, err, state);

			state.s.get<::cb_type::flags>(t.first) |= cb.flags;
			state.s.set<::cb_type::badboy_factor>(t.first, cb.badboy_factor);

			state.s.set<::cb_type::prestige_factor>(t.first, cb.prestige_factor);
			state.s.set<::cb_type::peace_cost_factor>(t.first, cb.peace_cost_factor);
			state.s.set<::cb_type::penalty_factor>(t.first, cb.penalty_factor);
			state.s.set<::cb_type::break_truce_prestige_factor>(t.first, cb.break_truce_prestige_factor);
			state.s.set<::cb_type::break_truce_infamy_factor>(t.first, cb.break_truce_infamy_factor);
			state.s.set<::cb_type::break_truce_militancy_factor>(t.first, cb.break_truce_militancy_factor);
			state.s.set<::cb_type::good_relation_prestige_factor>(t.first, cb.good_relation_prestige_factor);
			state.s.set<::cb_type::good_relation_infamy_factor>(t.first, cb.good_relation_infamy_factor);
			state.s.set<::cb_type::good_relation_militancy_factor>(t.first, cb.good_relation_militancy_factor);
			state.s.set<::cb_type::construction_speed>(t.first, cb.construction_speed);
			state.s.set<::cb_type::tws_battle_factor>(t.first, cb.tws_battle_factor);
			state.s.set<::cb_type::war_name>(t.first, cb.war_name);
			state.s.set<::cb_type::allowed_states>(t.first, cb.allowed_states);
			state.s.set<::cb_type::allowed_states_in_crisis>(t.first, cb.allowed_states_in_crisis);
			state.s.set<::cb_type::allowed_substate_regions>(t.first, cb.allowed_substate_regions);
			state.s.set<::cb_type::allowed_countries>(t.first, cb.allowed_countries);
			state.s.set<::cb_type::can_use>(t.first, cb.can_use);
			state.s.set<::cb_type::on_add>(t.first, cb.on_add);
			state.s.set<::cb_type::on_po_accepted>(t.first, cb.on_po_accepted);
			state.s.set<::cb_type::sprite_index>(t.first, cb.sprite_index);
			state.s.set<::cb_type::months>(t.first, cb.months);
			state.s.set<::cb_type::truce_months>(t.first, cb.truce_months);

			cb.id = state.s.get<::cb_type::id>(t.first);
			cb.name = state.s.get<::cb_type::name>(t.first);
			cb.explanation = state.s.get<::cb_type::explanation>(t.first);
		}
		state.s.for_each<military::cb_type_tag>([&state](military::cb_type_tag id) {
			if(!is_valid_index(state.s.get<::cb_type::explanation>(id))) {
				// prevent erronious entries from bad peace_order declaration from being used
				state.s.get<::cb_type::flags>(id) |= (cb_type::is_not_triggered_only | cb_type::is_not_constructing_cb);
			}
		});
	}
}
