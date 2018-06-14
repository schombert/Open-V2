#pragma once
#include "common\\common.h"

namespace serialization {
	template<typename T, typename ... CONTEXT>
	void serialize(std::byte* output, T const& obj, CONTEXT const& ... c);
	template<typename T, typename ... CONTEXT>
	void deserialize(std::byte const* input, T& obj, CONTEXT const& ... c);
	template<typename T>
	size_t serialize_size(T const& obj);

	template<typename T>
	class serializer;


	template<typename T>
	constexpr size_t static_size_object() { return sizeof(T); }
	template<typename T>
	size_t dynamic_size_object(T const& obj);

	template<typename T>
	constexpr bool has_static_size();
	template<typename T>
	constexpr bool has_simple_serialize();

	template<typename T>
	size_t generic_size(T const& obj) {
		if constexpr(has_static_size<T>())
			return static_size_object<T>();
		else
			return dynamic_size_object(obj);
	}
}
