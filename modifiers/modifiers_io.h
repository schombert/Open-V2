#pragma once
#include "common\\common.h"
#include "modifiers.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "Parsers\\parsers.hpp"
#include "text_data\\text_data.h"
#include "simple_fs\\simple_fs.h"
#include <ppl.h>

template<>
class serialization::serializer<modifiers::provincial_modifier> : public serialization::memcpy_serializer<modifiers::provincial_modifier> {};
template<>
class serialization::serializer<modifiers::national_modifier> : public serialization::memcpy_serializer<modifiers::national_modifier> {};
template<>
class serialization::serializer<modifiers::factor_modifier> : public serialization::memcpy_serializer<modifiers::factor_modifier> {};
template<>
class serialization::serializer<modifiers::national_focus> : public serialization::memcpy_serializer<modifiers::national_focus> {};
template<>
class serialization::serializer<modifiers::crime> : public serialization::memcpy_serializer<modifiers::crime> {};
template<>
class serialization::serializer<modifiers::factor_segment> : public serialization::memcpy_serializer<modifiers::factor_segment> {};
template<>
class serialization::serializer<modifiers::defines> : public serialization::memcpy_serializer<modifiers::defines> {};
template<>
class serialization::serializer<modifiers::static_modifiers_s> : public serialization::memcpy_serializer<modifiers::static_modifiers_s> {};

template<>
class serialization::serializer<modifiers::modifiers_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void rebuild_indexes(modifiers::modifiers_manager& obj) {
		for(auto const& i_mod : obj.national_modifiers)
			obj.named_national_modifiers_index.emplace(i_mod.name, i_mod.id);
		for(auto const& i_mod : obj.provincial_modifiers)
			obj.named_provincial_modifiers_index.emplace(i_mod.name, i_mod.id);
	}

	static void serialize_object(std::byte* &output, modifiers::modifiers_manager const& obj) {
		serialize(output, obj.national_modifiers);
		serialize(output, obj.provincial_modifiers);
		serialize(output, obj.factor_modifiers);
		serialize(output, obj.national_focuses);
		serialize(output, obj.factor_data);
		serialize(output, obj.triggered_modifiers);
		serialize(output, obj.crimes);
		serialize(output, obj.national_modifier_definitions);
		serialize(output, obj.provincial_modifier_definitions);
		serialize(output, obj.national_focus_goods_weights);
		serialize(output, obj.national_focus_group_count);
		serialize(output, obj.static_modifiers);
		serialize(output, obj.global_defines);
	}
	static void deserialize_object(std::byte const* &input, modifiers::modifiers_manager& obj) {
		deserialize(input, obj.national_modifiers);
		deserialize(input, obj.provincial_modifiers);
		deserialize(input, obj.factor_modifiers);
		deserialize(input, obj.national_focuses);
		deserialize(input, obj.factor_data);
		deserialize(input, obj.triggered_modifiers);
		deserialize(input, obj.crimes);
		deserialize(input, obj.national_modifier_definitions);
		deserialize(input, obj.provincial_modifier_definitions);
		deserialize(input, obj.national_focus_goods_weights);
		deserialize(input, obj.national_focus_group_count);
		deserialize(input, obj.static_modifiers);
		deserialize(input, obj.global_defines);

		rebuild_indexes(obj);
	}
	static void deserialize_object(std::byte const* &input, modifiers::modifiers_manager& obj, concurrency::task_group& tg) {
		deserialize(input, obj.national_modifiers);
		deserialize(input, obj.provincial_modifiers);
		deserialize(input, obj.factor_modifiers);
		deserialize(input, obj.national_focuses);
		deserialize(input, obj.factor_data);
		deserialize(input, obj.triggered_modifiers);
		deserialize(input, obj.crimes);
		deserialize(input, obj.national_modifier_definitions);
		deserialize(input, obj.provincial_modifier_definitions);
		deserialize(input, obj.national_focus_goods_weights);
		deserialize(input, obj.national_focus_group_count);
		deserialize(input, obj.static_modifiers);
		deserialize(input, obj.global_defines);

		tg.run([&obj]() { rebuild_indexes(obj); });
	}
	static size_t size(modifiers::modifiers_manager const& obj) {
		return serialize_size(obj.national_modifiers) +
			serialize_size(obj.provincial_modifiers) +
			serialize_size(obj.factor_modifiers) +
			serialize_size(obj.national_focuses) +
			serialize_size(obj.factor_data) +
			serialize_size(obj.triggered_modifiers) +
			serialize_size(obj.crimes) +
			serialize_size(obj.national_modifier_definitions) +
			serialize_size(obj.provincial_modifier_definitions) +
			serialize_size(obj.national_focus_goods_weights) +
			serialize_size(obj.national_focus_group_count) +
			serialize_size(obj.static_modifiers) +
			serialize_size(obj.global_defines);
	}
};

namespace modifiers {
	struct parsing_environment;

	class parsing_state {
	public:
		std::unique_ptr<parsing_environment> impl;

		parsing_state(text_data::text_sequences& tl, modifiers_manager& m);
		parsing_state(parsing_state&&) noexcept;
		~parsing_state();
	};

	class modifier_reading_base {
	public:
		std::vector<value_type, aligned_allocator_32<float>> modifier_data;
		uint32_t icon = 0;
		uint32_t count_unique_provincial = 0;
		uint32_t count_unique_national = 0;
		uint32_t total_attributes = 0;

		modifier_reading_base() : modifier_data(provincial_offsets::aligned_32_size + national_offsets::aligned_32_size) {}
		void add_attribute(const std::pair<token_and_type, float>& p);
		void add_attribute(uint32_t provincial_offset, uint32_t national_offset, float v);
		void remove_shared_national_attributes();
	};

	void set_provincial_modifier(provincial_modifier_tag tag, modifier_reading_base& mod, modifiers_manager& manager);
	void set_national_modifier(national_modifier_tag tag, const modifier_reading_base& mod, modifiers_manager& manager);

	provincial_modifier_tag add_provincial_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager);
	national_modifier_tag add_national_modifier(text_data::text_tag name, const modifier_reading_base& mod, modifiers_manager& manager);
	std::pair<provincial_modifier_tag, national_modifier_tag> add_indeterminate_modifier(text_data::text_tag name, modifier_reading_base& mod, modifiers_manager& manager);
	std::pair<uint16_t, bool> commit_factor(modifiers_manager& m, const std::vector<factor_segment>& factor);  // returns [offset into data, was newly added]

	provincial_modifier_tag parse_provincial_modifier(text_data::text_tag name, modifiers_manager& manager, const token_group* s, const token_group* e);
	national_modifier_tag parse_national_modifier(text_data::text_tag name, modifiers_manager& manager, const token_group* s, const token_group* e);
	std::pair<provincial_modifier_tag, national_modifier_tag> parse_indeterminate_modifier(
		text_data::text_tag name,
		modifiers_manager& manager,
		const token_group* s,
		const token_group* e);

	void read_defines(modifiers_manager& m, const directory& source_directory);
	void pre_parse_crimes(
		parsing_state& state,
		const directory& source_directory);
	void pre_parse_triggered_modifiers(parsing_state& state, const directory& source_directory);
	void read_national_values(
		parsing_state& state,
		const directory& source_directory);
	void read_static_modifiers(
		parsing_state& state,
		const directory& source_directory);
	void read_event_modifiers(
		parsing_state& state,
		const directory& source_directory);

	void read_crimes(parsing_state& state, scenario::scenario_manager& s);
	void read_triggered_modifiers(parsing_state& state, scenario::scenario_manager& s);

	void read_national_focuses(scenario::scenario_manager& s, const directory& source_directory);

	factor_tag parse_modifier_factors(
		scenario::scenario_manager& s,
		triggers::trigger_scope_state modifier_scope,
		float default_factor,
		float default_base,
		const token_group* start,
		const token_group* end);
}
