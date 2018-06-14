#pragma once
#include "simple_serialize.h"
#include <numeric>

namespace serialization {
	template<typename T, typename ... CONTEXT>
	void serialize(std::byte* output, T const& obj, CONTEXT const& ... c) {
		serializer<T>::serialize_object(output, obj, c...);
	}

	template<typename T, typename ... CONTEXT>
	void deserialize(std::byte const* input, T& obj, CONTEXT const& ... c) {
		serializer<T>::deserialize_object(input, obj, c...);
	}

	template<typename T>
	size_t serialize_size(T const& obj) {
		if constexpr(serializer<T>::has_static_size)
			return serializer<T>::size();
		else
			return serializer<T>::size(obj);
	}

	template<typename T>
	class serializer {
	public:
		static constexpr bool has_static_size = true;
		static constexpr bool has_simple_serialize = true;

		template<typename ... CONTEXT>
		static std::byte* serialize_object(std::byte* output, T const& obj, CONTEXT const& ... c) {
			memcpy(output, &obj, sizeof(T));
			return output + sizeof(T);
		}
		template<typename ... CONTEXT>
		static std::byte const* deserialize_object(std::byte const* input, T& obj, CONTEXT const& ... c) {
			memcpy(&obj, input, sizeof(T));
			return input + sizeof(T);
		}
		static constexpr size_t size() { return sizeof(T); }
		static constexpr size_t size(T const&) { return sizeof(T); }
	};

	template<typename IT, typename ... CONTEXT>
	std::byte const* deserialize_into_range(std::byte const* input, IT const& start, IT const& end, CONTEXT const& ... c) {
		using underlying_type = std::decay_t<decltype(*start)>;
		for(auto inner_it = start; inner_it != end; ++inner_it)
			input = serializer<underlying_type>::deserialize_object(input, *inner_it, c...);
		return input;
	}

	template<typename IT, typename ... CONTEXT>
	std::byte* serialize_range(std::byte* output, IT const& start, IT const& end, CONTEXT const& ... c) {
		using underlying_type = std::decay_t<decltype(*start)>;
		for(auto inner_it = start; inner_it != end; ++inner_it)
			output = serializer<underlying_type>::serialize_object(output, *inner_it, c...);
		return output;
	}

	template<typename T>
	std::byte* serialize_array(std::byte* output, T const* array_data, size_t array_size) {
		memcpy(output, array_data, array_size * sizeof(T));
		return output + array_size * sizeof(T);
	}

	template<typename T>
	std::byte const* deserialize_array(std::byte const* input, T* array_data, size_t array_size) {
		memcpy(array_data, input, array_size * sizeof(T));
		return input + array_size * sizeof(T);
	}


	template<typename T, typename ALLOC>
	class serializer<std::vector<T, ALLOC>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static std::byte* serialize_object(std::byte* output, std::vector<T, ALLOC> const& obj, CONTEXT const& ... c) {
			const uint32_t sz = uint32_t(obj.size());
			memcpy(output, &sz, sizeof(uint32_t));

			if constexpr(serializer<T>::has_simple_serialize)
				return serialize_array(output + sizeof(uint32_t), obj.data(), obj.size());
			else
				return serialize_range(output + sizeof(uint32_t), obj.begin(), obj.end(), c...);
		}
		template<typename ... CONTEXT>
		static std::byte const* deserialize_object(std::byte const* input, std::vector<T, ALLOC>& obj, CONTEXT const& ... c) {
			uint32_t size = 0;
			memcpy(&size, input, sizeof(uint32_t));
			obj.resize(size);

			if constexpr(serializer<T>::has_simple_serialize)
				return deserialize_array(input + sizeof(uint32_t), obj.data(), size);
			else
				return deserialize_into_range(input + sizeof(uint32_t), obj.begin(), obj.end(), c...);
		}
		static size_t size(std::vector<T, ALLOC> const& obj) {
			if constexpr(serializer<T>::has_static_size)
				return sizeof(uint32_t) + serializer<T>::size() * obj.size();
			else
				return sizeof(uint32_t) + std::accumulate(obj.begin(), obj.end(), size_t(0),
					[](size_t val, T const& item) { return val + serializer<T>::size(item); });
		}
	};
}
