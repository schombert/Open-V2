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
		return serializer<T>::size(obj);
	}

	template<typename T>
	class memcpy_serializer {
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

	template<>
	class serializer<char> : public memcpy_serializer<char> {};
	template<>
	class serializer<char16_t> : public memcpy_serializer<char16_t> {};
	template<>
	class serializer<char32_t> : public memcpy_serializer<char32_t> {};
	template<>
	class serializer<int8_t> : public memcpy_serializer<int8_t> {};
	template<>
	class serializer<uint8_t> : public memcpy_serializer<uint8_t> {};
	template<>
	class serializer<int16_t> : public memcpy_serializer<int16_t> {};
	template<>
	class serializer<uint16_t> : public memcpy_serializer<uint16_t> {};
	template<>
	class serializer<int32_t> : public memcpy_serializer<int32_t> {};
	template<>
	class serializer<uint32_t> : public memcpy_serializer<uint32_t> {};
	template<>
	class serializer<int64_t> : public memcpy_serializer<int64_t> {};
	template<>
	class serializer<uint64_t> : public memcpy_serializer<uint64_t> {};
	template<>
	class serializer<float> : public memcpy_serializer<float> {};
	template<>
	class serializer<double> : public memcpy_serializer<double> {};

	template<typename value_base, typename zero_is_null, typename individuator>
	class serializer<tag_type<value_base, zero_is_null, individuator>> : public memcpy_serializer<tag_type<value_base, zero_is_null, individuator>> {};

	template<typename value_type, int rows, int cols>
	class serializer<Eigen::Matrix<value_type, rows, cols>> {
		static_assert(rows > 0 && cols > 0);
	public:
		static constexpr bool has_static_size = true;
		static constexpr bool has_simple_serialize = true;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, Eigen::Matrix<value_type, rows, cols> const& obj, CONTEXT&& ... c) {
			memcpy(output, obj.data(), sizeof(value_type) * size_t(rows * cols));
			output += sizeof(value_type) * size_t(rows * cols);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, Eigen::Matrix<value_type, rows, cols>& obj, CONTEXT&& ... c) {
			memcpy(obj.data(), input, sizeof(value_type) * size_t(rows * cols));
			input += sizeof(value_type) * size_t(rows * cols);
		}
		static constexpr size_t size() { return sizeof(value_type) * size_t(rows * cols); }
		static constexpr size_t size(Eigen::Matrix<value_type, rows, cols> const&) { return sizeof(value_type) * size_t(rows * cols); }
	};

	template<typename char_type>
	class serializer<vector_backed_string<char_type>> : public memcpy_serializer<vector_backed_string<char_type>> {};

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
			return sizeof(uint32_t) + sizeof(uint32_t) + sizeof(typename tagged_fixed_blocked_2dvector<A, B, C, D>::block_s) * obj.size();
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
				return sizeof(uint32_t) + sizeof(T) * obj.size();
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

	template<typename T, typename I, typename allocator>
	class serializer<v_vector<T, I, allocator>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, v_vector<T, I, allocator> const& obj, CONTEXT&& ... c) {
			serialize(output, obj.elements);
			serialize(output, obj.index);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, v_vector<T, I, allocator>& obj, CONTEXT&& ... c) {
			deserialize(input, obj.elements);
			deserialize(input, obj.index);
		}
		static size_t size(v_vector<T, I, allocator> const& obj) {
			return serialize_size(obj.elements) + serialize_size(obj.index);
		}
	};

	template<typename A, typename B, typename compare, typename allocator>
	class serializer<boost::container::flat_map<A, B, compare, allocator>> {
	public:
		static constexpr bool has_static_size = false;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, boost::container::flat_map<A, B, compare, allocator> const& obj, CONTEXT&& ... c) {
			const uint32_t sz = uint32_t(obj.size());
			serialize(output, sz);
			for(auto const& pr : obj)
				serialize(output, pr, std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, boost::container::flat_map<A, B, compare, allocator>& obj, CONTEXT&& ... c) {
			uint32_t sz = 0;
			deserialize(input, sz);
			for(; sz > 0; --sz) {
				std::pair<A, B> local;
				deserialize(input, local, std::forward<CONTEXT>(c)...);
				obj.insert(local);
			}
		}
		static size_t size(boost::container::flat_map<A, B, compare, allocator> const& obj) {
			if constexpr(serializer<std::pair<A, B>>::has_static_size)
				return sizeof(uint32_t) + obj.size() * sizeof(std::pair<A, B>);
			else
				return sizeof(uint32_t) + std::accumulate(obj.begin(), obj.end(), size_t(0),
					[](size_t val, std::pair<A, B> const& item) { return val + serializer<std::pair<A, B>>::size(item); });
		}
	};

	template<typename A, typename B>
	class serializer<std::pair<A, B>> {
	public:
		static constexpr bool has_static_size = serializer<A>::has_static_size && serializer<B>::has_static_size;
		static constexpr bool has_simple_serialize = serializer<A>::has_simple_serialize && serializer<B>::has_simple_serialize;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, std::pair<A, B> const& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(output, &obj, sizeof(std::pair<A, B>));
				output += sizeof(std::pair<A, B>);
			} else {
				serialize(output, obj.first);
				serialize(output, obj.second);
			}
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::pair<A, B>& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(&obj, input, sizeof(std::pair<A, B>));
				input += sizeof(std::pair<A, B>);
			} else {
				deserialize(input, obj.first);
				deserialize(input, obj.second);
			}
		}
		static size_t size(std::pair<A, B> const& obj) {
			if constexpr(has_static_size && has_simple_serialize)
				return sizeof(std::pair<A, B>);
			else
				return serialize_size(obj.first) + serialize_size(obj.second);
		}
	};

	template<typename A, typename B, typename C>
	class serializer<std::tuple<A, B, C>> {
	public:
		static constexpr bool has_static_size = serializer<A>::has_static_size && serializer<B>::has_static_size && serializer<C>::has_static_size;
		static constexpr bool has_simple_serialize = serializer<A>::has_simple_serialize && serializer<B>::has_simple_serialize && serializer<C>::has_simple_serialize;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, std::tuple<A, B, C> const& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(output, &obj, sizeof(std::tuple<A, B, C>));
				output += sizeof(std::tuple<A, B, C>);
			} else {
				serialize(output, std::get<0>(obj));
				serialize(output, std::get<1>(obj));
				serialize(output, std::get<2>(obj));
			}
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::tuple<A, B, C >& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(&obj, input, sizeof(std::tuple<A, B, C>));
				input += sizeof(std::tuple<A, B, C>);
			} else {
				deserialize(input, std::get<0>(obj));
				deserialize(input, std::get<1>(obj));
				deserialize(input, std::get<2>(obj));
			}
		}
		static size_t size(std::tuple<A, B, C> const& obj) {
			if constexpr(has_static_size && has_simple_serialize)
				return sizeof(std::tuple<A, B, C>);
			else
				return serialize_size(std::get<0>(obj)) + serialize_size(std::get<1>(obj)) + serialize_size(std::get<2>(obj));
		}
	};

	class serialize_file_wrapper {
	private:
		void* file_handle = nullptr;
		void* mapping_handle = nullptr;
		void* mapped_bytes = nullptr;
	public:
		serialize_file_wrapper(std::u16string const& file_name);
		serialize_file_wrapper(std::u16string const& file_name, size_t size);
		~serialize_file_wrapper();

		std::byte* get_bytes() const;
		bool file_valid() const;
	};

	template<typename T, typename ... CONTEXT>
	void serialize_to_file(std::u16string const& file_name, T const& obj, CONTEXT&& ... c) {
		const auto fsize = serialize_size(obj);
		serialize_file_wrapper file(file_name, fsize);

		auto ptr = file.get_bytes();
		serialize(ptr, obj, std::forward<CONTEXT>(c) ...);
	}

	template<typename T, typename ... CONTEXT>
	void deserialize_from_file(std::u16string const& file_name, T& obj, CONTEXT&& ... c) {
		serialize_file_wrapper file(file_name);

		if(file.file_valid()) {
			std::byte const* ptr = file.get_bytes();
			deserialize(ptr, obj, std::forward<CONTEXT>(c) ...);
		}
	}
}
