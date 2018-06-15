#pragma once
#include "simple_serialize.h"
#include <numeric>

namespace serialization {
	template<typename T, typename ... CONTEXT>
	void serialize(std::byte* &output, T const& obj, CONTEXT&& ... c) {
		serializer<T>::serialize_object(output, obj, std::forward<CONTEXT>(c)...);
	}

	template<typename T, typename ... CONTEXT>
	void deserialize(std::byte const* &input, T& obj, CONTEXT&& ... c) {
		serializer<T>::deserialize_object(input, obj, std::forward<CONTEXT>(c)...);
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
		static void serialize_object(std::byte* &output, T const& obj, CONTEXT&& ... c) {
			memcpy(output, &obj, sizeof(T));
			output += sizeof(T);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, T& obj, CONTEXT&& ... c) {
			memcpy(&obj, input, sizeof(T));
			input += sizeof(T);
		}
		static constexpr size_t size() { return sizeof(T); }
		static constexpr size_t size(T const&) { return sizeof(T); }
	};

	template<typename IT, typename ... CONTEXT>
	void deserialize_into_range(std::byte const* &input, IT const& start, IT const& end, CONTEXT&& ... c) {
		using underlying_type = std::decay_t<decltype(*start)>;
		for(auto inner_it = start; inner_it != end; ++inner_it)
			serializer<underlying_type>::deserialize_object(input, *inner_it, std::forward<CONTEXT>(c)...);
	}

	template<typename IT, typename ... CONTEXT>
	void serialize_range(std::byte* &output, IT const& start, IT const& end, CONTEXT&& ... c) {
		using underlying_type = std::decay_t<decltype(*start)>;
		for(auto inner_it = start; inner_it != end; ++inner_it)
			serializer<underlying_type>::serialize_object(output, *inner_it, std::forward<CONTEXT>(c)...);
	}

	template<typename T>
	void serialize_array(std::byte* &output, T const* array_data, size_t array_size) {
		if(array_size != 0)
			memcpy(output, array_data, array_size * sizeof(T));
		output += array_size * sizeof(T);
	}

	template<typename T>
	void deserialize_array(std::byte const* &input, T* array_data, size_t array_size) {
		if(array_size != 0)
			memcpy(array_data, input, array_size * sizeof(T));
		input += array_size * sizeof(T);
	}

	template<typename A, typename B, typename C>
	class serializer<tagged_vector<A, B, C>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, tagged_vector<A, B, C> const& obj, CONTEXT&& ... c) {
			const uint32_t sz = uint32_t(obj.size());
			serialize(output, sz);

			if constexpr(serializer<A>::has_simple_serialize)
				serialize_array(output, obj.data(), sz);
			else
				serialize_range(output, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, tagged_vector<A, B, C>& obj, CONTEXT&& ... c) {
			uint32_t size = 0;
			deserialize(input, size);

			obj.resize(size);
			if constexpr(serializer<A>::has_simple_serialize)
				deserialize_array(input, obj.data(), size);
			else
				deserialize_into_range(input, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		static size_t size(tagged_vector<A, B, C> const& obj) {
			if constexpr(serializer<A>::has_static_size)
				return sizeof(uint32_t) + serializer<A>::size() * obj.size();
			else
				return sizeof(uint32_t) + std::accumulate(obj.begin(), obj.end(), size_t(0),
					[](size_t val, A const& item) { return val + serializer<A>::size(item); });
		}
	};

	template<typename A, typename B, typename C, typename D>
	class serializer<tagged_fixed_2dvector<A, B, C, D>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, tagged_fixed_2dvector<A, B, C, D> const& obj, CONTEXT&& ... c) {
			uint32_t inner_sz = uint32_t(obj.inner_size());
			uint32_t outer_sz = uint32_t(obj.outer_size());
			serialize(output, inner_sz);
			serialize(output, outer_sz);

			if constexpr(serializer<A>::has_simple_serialize)
				serialize_array(output, obj.data(), obj.size());
			else
				serialize_range(output, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, tagged_fixed_2dvector<A, B, C, D>& obj, CONTEXT&& ... c) {
			uint32_t inner_sz = 0;
			uint32_t outer_sz = 0;
			deserialize(input, inner_sz);
			deserialize(input, outer_sz);

			obj.reset(inner_sz);
			obj.resize(outer_sz);

			if constexpr(serializer<A>::has_simple_serialize)
				deserialize_array(input, obj.data(), obj.size());
			else
				deserialize_into_range(input, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		static size_t size(tagged_fixed_2dvector<A, B, C, D> const& obj) {
			if constexpr(serializer<A>::has_static_size)
				return sizeof(uint32_t) + sizeof(uint32_t) + serializer<A>::size() * obj.size();
			else
				return sizeof(uint32_t) + sizeof(uint32_t) + std::accumulate(obj.begin(), obj.end(), size_t(0),
					[](size_t val, A const& item) { return val + serializer<A>::size(item); });
		}
	};

	template<typename A, typename B, typename C, typename D>
	class serializer<tagged_fixed_blocked_2dvector<A, B, C, D>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, tagged_fixed_blocked_2dvector<A, B, C, D> const& obj, CONTEXT&& ... c) {
			uint32_t inner_sz = uint32_t(obj.inner_size());
			uint32_t outer_sz = uint32_t(obj.outer_size());
			serialize(output, inner_sz);
			serialize(output, outer_sz);

			serialize_array(output, obj.data(), obj.size());
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, tagged_fixed_blocked_2dvector<A, B, C, D>& obj, CONTEXT&& ... c) {
			uint32_t inner_sz = 0;
			uint32_t outer_sz = 0;
			deserialize(input, inner_sz);
			deserialize(input, outer_sz);

			obj.reset(inner_sz);
			obj.resize(outer_sz);

			deserialize_array(input, obj.data(), obj.size());
		}
		static size_t size(tagged_fixed_blocked_2dvector<A, B, C, D> const& obj) {
			return sizeof(uint32_t) + sizeof(uint32_t) + serializer<typename tagged_fixed_blocked_2dvector<A, B, C, D>::block_s>::size() * obj.size();
		}
	};

	template<typename T, typename ALLOC>
	class serializer<std::vector<T, ALLOC>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, std::vector<T, ALLOC> const& obj, CONTEXT&& ... c) {
			const uint32_t sz = uint32_t(obj.size());
			serialize(output, sz);

			if constexpr(serializer<T>::has_simple_serialize)
				serialize_array(output, obj.data(), sz);
			else
				serialize_range(output, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::vector<T, ALLOC>& obj, CONTEXT&& ... c) {
			uint32_t size = 0;
			deserialize(input, size);

			obj.resize(size);
			if constexpr(serializer<T>::has_simple_serialize)
				deserialize_array(input, obj.data(), size);
			else
				deserialize_into_range(input, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		static size_t size(std::vector<T, ALLOC> const& obj) {
			if constexpr(serializer<T>::has_static_size)
				return sizeof(uint32_t) + serializer<T>::size() * obj.size();
			else
				return sizeof(uint32_t) + std::accumulate(obj.begin(), obj.end(), size_t(0),
					[](size_t val, T const& item) { return val + serializer<T>::size(item); });
		}
	};

	template<typename A, typename B, typename C>
	class serializer<std::basic_string<A, B, C>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, std::basic_string<A, B, C> const& obj, CONTEXT&& ... c) {
			const uint32_t sz = uint32_t(obj.length());
			serialize(output, sz);
			serialize_array(output, obj.data(), sz);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::basic_string<A, B, C>& obj, CONTEXT&& ... c) {
			uint32_t size = 0;
			deserialize(input, size);

			obj.resize(size);
			deserialize_array(input, obj.data(), size);
		}
		static size_t size(std::basic_string<A, B, C> const& obj) {
			return sizeof(uint32_t) + obj.length() * sizeof(A);
		}
	};
}
