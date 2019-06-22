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


	void read_cb_types(parsing_state const& state) {
		internal_read_cb_types(state);
	}


	void read_wars(world_state& ws, date_tag target_date, const directory& root) {
		internal_read_wars(ws, target_date, root);
	}


	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory) {

		internal_pre_parse_cb_types(state, source_directory);
	}

	void resize_trait_count(scenario::scenario_manager& s, int32_t sz) {
		s.military_m.leader_trait_definitions.reset(traits::trait_count);
		s.military_m.leader_trait_definitions.resize(sz);
	}

	void read_leader_traits(parsing_state& state, const directory& source_directory) {
		internal_read_leader_traits<resize_trait_count>(state, source_directory);
	}

	void read_oob_file(world_state& ws, nations::country_tag for_nation, token_generator& gen) {
		internal_read_oob_file<nations::make_nation_for_tag, military::make_army, military::make_fleet, military::make_empty_leader, military::calculate_leader_traits>(ws, for_nation, gen);
	}

	cb_type_tag create_new_cb(scenario::scenario_manager& s) {
		return s.military_m.cb_types.emplace_back();
	}
	tagged_array_view<float, uint32_t> safe_get_trait_row(scenario::scenario_manager& s, leader_trait_tag id) {
		return  s.military_m.leader_trait_definitions.safe_get_row(id);
	}

	triggers::trigger_tag generic_read_trigger(token_generator& gen, scenario::scenario_manager& s, empty_error_handler& err, triggers::trigger_scope_state outer_scope) {
		auto rng = find_group_range(gen, err, s);

		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, rng.start, rng.end);

		const auto td = triggers::parse_trigger(s, outer_scope, parse_results.data(), parse_results.data() + parse_results.size());
		return triggers::commit_trigger(s.trigger_m, td);
	}
	triggers::effect_tag generic_read_effect(token_generator& gen, scenario::scenario_manager& s, events::event_creation_manager& ecm, empty_error_handler& err, triggers::trigger_scope_state outer_scope) {
		auto rng = find_group_range(gen, err, s);
		std::vector<token_group> parse_results;
		parse_pdx_file(parse_results, rng.start, rng.end);

		const auto td = triggers::parse_effect(s, ecm,
			outer_scope, parse_results.data(), parse_results.data() + parse_results.size());
		return triggers::commit_effect(s.trigger_m, td);
	}
}
