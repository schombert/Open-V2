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
#include "military_io_internals.hpp"

void serialization::serializer<military::military_manager>::rebuild_indexes(military::military_manager & obj) {

	obj.cb_type_to_speed.resize(obj.cb_types.size());
	for(auto const& i_cb : obj.cb_types) {
		obj.named_cb_type_index.emplace(i_cb.name, i_cb.id);
		obj.cb_type_to_speed[i_cb.id] = i_cb.construction_speed;
	}
	for(int32_t i = static_cast<int32_t>(obj.leader_traits.size()) - 1; i >= 0; --i)
		obj.named_leader_trait_index.emplace(obj.leader_traits[military::leader_trait_tag(static_cast<military::leader_trait_tag::value_base_t>(i))], military::leader_trait_tag(static_cast<military::leader_trait_tag::value_base_t>(i)));


}

void serialization::serializer<military::military_manager>::serialize_object(std::byte *& output, military::military_manager const & obj) {
	serialize(output, obj.cb_types);
	serialize(output, obj.leader_traits);
	serialize(output, obj.leader_trait_definitions);
	serialize(output, obj.personality_traits);
	serialize(output, obj.background_traits);
}

void serialization::serializer<military::military_manager>::deserialize_object(std::byte const *& input, military::military_manager & obj) {
	deserialize(input, obj.cb_types);
	deserialize(input, obj.leader_traits);
	deserialize(input, obj.leader_trait_definitions);
	deserialize(input, obj.personality_traits);
	deserialize(input, obj.background_traits);

	rebuild_indexes(obj);
}

void serialization::serializer<military::military_manager>::deserialize_object(std::byte const *& input, military::military_manager & obj, concurrency::task_group & tg) {
	deserialize(input, obj.cb_types);
	deserialize(input, obj.leader_traits);
	deserialize(input, obj.leader_trait_definitions);
	deserialize(input, obj.personality_traits);
	deserialize(input, obj.background_traits);

	tg.run([&obj]() { rebuild_indexes(obj); });
}

size_t serialization::serializer<military::military_manager>::size(military::military_manager const & obj) {
	return serialize_size(obj.cb_types) +
		serialize_size(obj.leader_traits) +
		serialize_size(obj.leader_trait_definitions) +
		serialize_size(obj.personality_traits) +
		serialize_size(obj.background_traits);
}

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

	parsing_state::parsing_state(scenario::scenario_manager& m, events::event_creation_manager& e) :
		impl(std::make_unique<parsing_environment<scenario::scenario_manager>>(m, e)) {}
	parsing_state::~parsing_state() {}

	parsing_state::parsing_state(parsing_state&& o) noexcept : impl(std::move(o.impl)) {}

	void read_cb_types(parsing_state const& state) {
		for(auto const& t : state.impl->pending_cb_parse) {
			auto& cb_obj = state.impl->s.military_m.cb_types[t.first];

			token_generator gen(t.second.start, t.second.end);
			empty_error_handler err;
			auto cb = military::military_parsing::parse_single_cb(gen, err, *(state.impl));

			cb.flags |= cb_obj.flags;
			cb.id = cb_obj.id;
			cb.name = cb_obj.name;
			cb.explanation = cb_obj.explanation;
			cb_obj = cb;
		}
		for(auto& c : state.impl->s.military_m.cb_types) {
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

			token_generator gen(main_results.parse_data.get(), main_results.parse_data.get() + sz);
			empty_error_handler err;

			military::military_parsing::parse_cb_file(gen, err, *(state.impl));
		}
	}

	void read_leader_traits(parsing_state& state,
		const directory& source_directory) {

		const auto common_dir = source_directory.get_directory(u"\\common");

		const auto fi = common_dir.open_file(u"traits.txt");

		state.impl->s.military_m.leader_trait_definitions.reset(traits::trait_count);
		state.impl->s.military_m.leader_trait_definitions.resize(2);
		state.impl->s.military_m.leader_traits.resize(2);

		{
			const static char no_personality_string[] = "no_personality";
			const auto trait_name = text_data::get_thread_safe_text_handle(state.impl->s.gui_m.text_data_sequences, no_personality_string, no_personality_string + sizeof(no_personality_string) - 1);

			state.impl->s.military_m.named_leader_trait_index.emplace(trait_name, state.impl->s.military_m.no_personality_trait);
			const auto no_personality = state.impl->s.military_m.no_personality_trait;
			state.impl->s.military_m.leader_traits[no_personality] = trait_name;
		}
		{
			const static char no_background_string[] = "no_background";
			const auto trait_name = text_data::get_thread_safe_text_handle(state.impl->s.gui_m.text_data_sequences, no_background_string, no_background_string + sizeof(no_background_string) - 1);

			state.impl->s.military_m.named_leader_trait_index.emplace(trait_name, state.impl->s.military_m.no_background_trait);
			const auto no_background = state.impl->s.military_m.no_background_trait;
			state.impl->s.military_m.leader_traits[no_background] = trait_name;
		}

		if(fi) {
			const auto sz = fi->size();
			const auto parse_data = std::unique_ptr<char[]>(new char[sz]);
			fi->read_to_buffer(parse_data.get(), sz);

			token_generator gen(parse_data.get(), parse_data.get() + sz);
			empty_error_handler err;

			military::military_parsing::parse_traits_file(gen, err, state.impl->s);
		}
	}

	void read_oob_file(world_state& ws, nations::country_tag for_nation, token_generator& gen) {
		empty_error_handler err;
		oob_constext<world_state> con{ ws, for_nation };
		military::military_parsing::parse_oob_file(gen, err, con);
	}
}
