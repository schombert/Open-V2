#pragma once
#include "common\\common.h"
#include "military.h"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "simple_fs\\simple_fs.h"
#include "simple_serialize\\simple_serialize.hpp"
#include <ppl.h>

class world_state;

template<>
class serialization::serializer<military::war_goal> : public serialization::memcpy_serializer<military::war_goal> {};
template<>
class serialization::serializer<military::naval_control> : public serialization::memcpy_serializer<military::war_goal> {};
template<>
class serialization::serializer<military::cb_type> : public serialization::memcpy_serializer<military::cb_type> {};
template<>
class serialization::serializer<military::army_orders_type> : public serialization::memcpy_serializer<military::army_orders_type> {};



template<typename T>
class serialization::tagged_serializer<army_order::province_set, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<army_order::army_set, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<strategic_hq::army_set, T> : public serialization::discard_serializer<T> {};
template<typename T>
class serialization::tagged_serializer<strategic_hq::province_set, T> : public serialization::discard_serializer<T> {};

template<typename T>
class serialization::tagged_serializer<war::attackers, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};
template<typename T>
class serialization::tagged_serializer<war::defenders, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};
template<typename T>
class serialization::tagged_serializer<war::naval_control_set, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};
template<typename T>
class serialization::tagged_serializer<war::war_goals, T> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, T const& obj, world_state const& ws);
	static void deserialize_object(std::byte const* &input, T& obj, world_state& ws);
	static size_t size(T const& obj, world_state const& ws);
	static size_t size();
};

namespace scenario {
	class scenario_manager;
}
namespace events {
	struct event_creation_manager;
}

namespace military {
	cb_type_tag create_new_cb(scenario::scenario_manager& s);
	tagged_array_view<float, uint32_t> safe_get_trait_row(scenario::scenario_manager& s, leader_trait_tag);
	triggers::trigger_tag generic_read_trigger(token_generator& gen, scenario::scenario_manager& s, empty_error_handler&, triggers::trigger_scope_state outer_scope);
	triggers::effect_tag generic_read_effect(token_generator& gen, scenario::scenario_manager& s, events::event_creation_manager& ecm, empty_error_handler& err, triggers::trigger_scope_state outer_scope);

	template<typename SCENARIO, typename ECM, auto new_cb_fn, auto safe_row_fn, auto rt_fn, auto re_fn>
	struct parsing_environment {
		SCENARIO& s;
		ECM& ecm;
		static constexpr auto internal_new_cb_fn = new_cb_fn;
		static constexpr auto internal_safe_row_fn = safe_row_fn;
		static constexpr auto internal_read_trigger_fn = rt_fn;
		static constexpr auto internal_read_effect_fn = re_fn;

		unparsed_data cb_file;

		text_data::text_tag lib_name_a;
		text_data::text_tag lib_name_b;
		text_data::text_tag take_from_sphere;

		std::vector<std::pair<cb_type_tag, text_range>> pending_cb_parse;

		parsing_environment(SCENARIO& m, ECM& e) : s(m), ecm(e) {
			lib_name_a = m.get_thread_safe_text_handle("free_peoples");
			lib_name_b = m.get_thread_safe_text_handle("liberate_country");
			take_from_sphere = m.get_thread_safe_text_handle("take_from_sphere");
		}
	};

	class military_manager;


	using parsing_state = parsing_environment<scenario::scenario_manager, events::event_creation_manager,
		create_new_cb, safe_get_trait_row, generic_read_trigger, generic_read_effect>;

	void pre_parse_cb_types(
		parsing_state& state,
		const directory& source_directory);
	void read_leader_traits(parsing_state& state,
		const directory& source_directory);
	void read_cb_types(parsing_state const& state);
	void read_oob_file(world_state& ws, nations::country_tag for_nation, token_generator& gen);
	void read_wars(world_state& ws, date_tag target_date, const directory& root);
}
