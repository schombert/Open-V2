#pragma once
#include "variables.h"
#include "simple_serialize\\simple_serialize.hpp"

class world_state;

template<>
class serialization::serializer<variables::variables_state> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, variables::variables_state const& obj, world_state const&) {
		serialize(output, obj.global_variables);
	}
	static void deserialize_object(std::byte const* &input, variables::variables_state& obj, world_state&) {
		deserialize(input, obj.global_variables);
	}
	static size_t size(variables::variables_state const& obj, world_state const&) {
		return serialize_size(obj.global_variables);
	}
};

template<>
class serialization::serializer<variables::variables_manager> {
public:
	static constexpr bool has_static_size = false;
	static constexpr bool has_simple_serialize = false;

	static void serialize_object(std::byte* &output, variables::variables_manager const& obj) {
		serialize(output, obj.named_global_variables);
		serialize(output, obj.named_national_variables);
		serialize(output, obj.named_national_flags);
	}
	template<typename ... CONTEXT>
	static void deserialize_object(std::byte const* &input, variables::variables_manager& obj, CONTEXT&& ... c) {
		deserialize(input, obj.named_global_variables);
		deserialize(input, obj.named_national_variables);
		deserialize(input, obj.named_national_flags);

		obj.count_national_variables = static_cast<uint32_t>(obj.named_national_variables.size());
		obj.count_national_flags = static_cast<uint32_t>(obj.named_national_flags.size());
		obj.count_global_variables = static_cast<uint32_t>(obj.named_global_variables.size());

		obj.national_flag_to_name.resize(obj.count_national_flags);
		obj.global_variable_to_name.resize(obj.count_global_variables);
		obj.national_variable_to_name.resize(obj.count_national_variables);

		for(auto& pr : obj.named_global_variables)
			obj.global_variable_to_name[pr.second] = pr.first;
		for(auto& pr : obj.named_national_variables)
			obj.national_variable_to_name[pr.second] = pr.first;
		for(auto& pr : obj.named_national_flags)
			obj.national_flag_to_name[pr.second] = pr.first;
	}
	static size_t size(variables::variables_manager const& obj) {
		return serialize_size(obj.named_global_variables) +
			serialize_size(obj.named_national_variables) +
			serialize_size(obj.named_national_flags);
	}
	template<typename T>
	static size_t size(variables::variables_manager const& obj, T const&) {
		return size(obj);
	}
};
