#pragma once
#include "variables.h"
#include "simple_serialize\\simple_serialize.hpp"

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
	}
	static size_t size(variables::variables_manager const& obj) {
		return serialize_size(obj.named_global_variables) +
			serialize_size(obj.named_national_variables) +
			serialize_size(obj.named_national_flags);
	}
};
