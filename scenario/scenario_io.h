#pragma once
#include "scenario.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "simple_fs\\simple_fs.h"
#include "cultures\\cultures_io.h"
#include "economy\\economy_io.h"
#include "events\\events_io.h"
#include "graphics\\texture.h"
#include "gui_definitions\\gui_definitions_io.h"
#include "text_data\\text_data_io.h"
#include "graphics_objects\\graphics_objects_io.h"
#include "gui\\gui_io.h"
#include "governments\\governments_io.h"
#include "issues\\issues_io.h"
#include "ideologies\\ideologies_io.h"
#include "military\\military_io.h"
#include "modifiers\\modifiers_io.h"
#include "population\\population_io.h"
#include "provinces\\provinces_io.h"
#include "sound\\sound_io.h"
#include "technologies\\technologies_io.h"
#include "triggers\\triggers_io.h"
#include "variables\\variables_io.h"

template<>
class serialization::serializer<scenario::scenario_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, scenario::scenario_manager const& obj) {
		serialize(output, obj.population_m);
		serialize(output, obj.culture_m);
		serialize(output, obj.economy_m);
		serialize(output, obj.governments_m);
		serialize(output, obj.ideologies_m);
		serialize(output, obj.issues_m);
		serialize(output, obj.modifiers_m);
		serialize(output, obj.province_m);
		serialize(output, obj.technology_m);
		serialize(output, obj.variables_m);
		serialize(output, obj.military_m);
		serialize(output, obj.event_m);
		serialize(output, obj.trigger_m);
		serialize(output, obj.gui_m);
		serialize(output, obj.sound_m);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, scenario::scenario_manager& obj, CONTEXT&& ... c) {
		deserialize(input, obj.population_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.culture_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.economy_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.governments_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.ideologies_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.issues_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.modifiers_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.province_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.technology_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.variables_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.military_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.event_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.trigger_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.gui_m, std::forward<CONTEXT>(c) ...);
		deserialize(input, obj.sound_m, std::forward<CONTEXT>(c) ...);
	}
	static size_t size(scenario::scenario_manager const& obj) {
		return serialize_size(obj.population_m) +
			serialize_size(obj.culture_m) +
			serialize_size(obj.economy_m) +
			serialize_size(obj.governments_m) +
			serialize_size(obj.ideologies_m) +
			serialize_size(obj.issues_m) +
			serialize_size(obj.modifiers_m) +
			serialize_size(obj.province_m) +
			serialize_size(obj.technology_m) +
			serialize_size(obj.variables_m) +
			serialize_size(obj.military_m) +
			serialize_size(obj.event_m) +
			serialize_size(obj.trigger_m) +
			serialize_size(obj.gui_m) +
			serialize_size(obj.sound_m);
	}
};

namespace scenario {
	provinces::color_to_terrain_map read_scenario(scenario_manager& s, const directory& root);
}
