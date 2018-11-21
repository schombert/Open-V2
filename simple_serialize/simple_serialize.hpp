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

	template<typename T, typename ... CONTEXT>
	size_t serialize_size(T const& obj, CONTEXT&& ... c) {
		return serializer<T>::size(obj, std::forward<CONTEXT>(c)...);
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
		template<typename ... CONTEXT>
		static constexpr size_t size(T const&, CONTEXT&& ...) { return sizeof(T); }
	};

	template<typename T>
	class discard_serializer {
	public:
		static constexpr bool has_static_size = true;
		static constexpr bool has_simple_serialize = false;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &, T const& , CONTEXT&& ... ) {}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &, T& , CONTEXT&& ... ) {}
		static constexpr size_t size() { return 0; }
		template<typename ... CONTEXT>
		static constexpr size_t size(T const&, CONTEXT&& ...) { return 0; }
	};

	template<>
	class serializer<bool> : public memcpy_serializer<bool> {};
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
	template<>
	class serializer<serialize_file_header> : public memcpy_serializer<serialize_file_header> {};
	template<>
	class serializer<graphics::color_rgb> : public memcpy_serializer<graphics::color_rgb> {};
	template<>
	class serializer<graphics::color_rgba> : public memcpy_serializer<graphics::color_rgba> {};

	template<typename value_base, typename zero_is_null, typename individuator>
	class serializer<tag_type<value_base, zero_is_null, individuator>> : public memcpy_serializer<tag_type<value_base, zero_is_null, individuator>> {};

	template<typename tag, typename T>
	class tagged_serializer {
	public:
		static constexpr bool has_static_size = serializer<T>::has_static_size;
		static constexpr bool has_simple_serialize = serializer<T>::has_simple_serialize;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, T const& obj, CONTEXT&& ... c) {
			serializer<T>::serialize_object(output, obj, std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, T& obj, CONTEXT&& ... c) {
			serializer<T>::deserialize_object(input, obj, std::forward<CONTEXT>(c)...);
		}
		static constexpr size_t size() {
			if constexpr(has_static_size)
				return serializer<T>::size();
			else
				return sizeof(T);
		}
		template<typename ... CONTEXT>
		static size_t size(T const& obj, CONTEXT&& ... c) {
			return serializer<T>::size(obj, std::forward<CONTEXT>(c)...);
		}
	};

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
		template<typename ... CONTEXT>
		static constexpr size_t size(Eigen::Matrix<value_type, rows, cols> const&, CONTEXT&& ... ) { return sizeof(value_type) * size_t(rows * cols); }
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

	template<typename A, size_t N>
	class serializer<std::array<A, N>> {
	public:
		static constexpr bool has_static_size = serializer<A>::has_static_size;
		static constexpr bool has_simple_serialize = serializer<A>::has_simple_serialize;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, std::array<A, N> const& obj, CONTEXT&& ... c) {
			if constexpr(has_simple_serialize)
				serialize_array(output, obj.data(), N);
			else
				serialize_range(output, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::array<A, N>& obj, CONTEXT&& ... c) {
			if constexpr(has_simple_serialize)
				deserialize_array(input, obj.data(), N);
			else
				deserialize_into_range(input, obj.begin(), obj.end(), std::forward<CONTEXT>(c)...);
		}
		template<typename ... CONTEXT>
		static size_t size(std::array<A, N> const& obj, CONTEXT&& ... c) {
			if constexpr(serializer<A>::has_static_size)
				return serializer<A>::size() * N;
			else
				return std::transform_reduce(obj.begin(), obj.end(), size_t(0), std::plus<>(),
					[&](A const& item) { return serializer<A>::size(item, std::forward<CONTEXT>(c)...); });
		}
		constexpr static size_t size() {
			if constexpr(has_static_size)
				return serializer<A>::size() * N;
			else
				return sizeof(A) * N;
		}
	};

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
		template<typename ... CONTEXT>
		static size_t size(tagged_vector<A, B, C> const& obj, CONTEXT&& ... c) {
			if constexpr(serializer<A>::has_static_size)
				return sizeof(uint32_t) + serializer<A>::size() * obj.size();
			else
				return sizeof(uint32_t) + std::transform_reduce(obj.begin(), obj.end(), size_t(0), std::plus<>(),
					[&](A const& item) { return serializer<A>::size(item, std::forward<CONTEXT>(c)...); });
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
		template<typename ... CONTEXT>
		static size_t size(tagged_fixed_2dvector<A, B, C, D> const& obj, CONTEXT&& ... c) {
			if constexpr(serializer<A>::has_static_size)
				return sizeof(uint32_t) + sizeof(uint32_t) + serializer<A>::size() * obj.size();
			else
				return sizeof(uint32_t) + sizeof(uint32_t) + std::accumulate(obj.begin(), obj.end(), size_t(0),
					[&](size_t val, A const& item) { return val + serializer<A>::size(item, std::forward<CONTEXT>(c)...); });
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
		template<typename ... CONTEXT>
		static size_t size(tagged_fixed_blocked_2dvector<A, B, C, D> const& obj, CONTEXT&& ... c) {
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
		template<typename ... CONTEXT>
		static size_t size(std::vector<T, ALLOC> const& obj, CONTEXT&& ... c) {
			if constexpr(serializer<T>::has_static_size)
				return sizeof(uint32_t) + sizeof(T) * obj.size();
			else
				return sizeof(uint32_t) + std::transform_reduce(obj.begin(), obj.end(), size_t(0), std::plus<>(),
					[&](T const& item) { return serializer<T>::size(item, std::forward<CONTEXT>(c)...); });
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
		template<typename ... CONTEXT>
		static size_t size(std::basic_string<A, B, C> const& obj, CONTEXT&& ... c) {
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
		template<typename ... CONTEXT>
		static size_t size(v_vector<T, I, allocator> const& obj, CONTEXT&& ... c) {
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
		template<typename ... CONTEXT>
		static size_t size(boost::container::flat_map<A, B, compare, allocator> const& obj, CONTEXT&& ... c) {
			if constexpr(serializer<std::pair<A, B>>::has_static_size)
				return sizeof(uint32_t) + obj.size() * sizeof(std::pair<A, B>);
			else
				return sizeof(uint32_t) + std::accumulate(obj.begin(), obj.end(), size_t(0),
					[&](size_t val, std::pair<A, B> const& item) { return val + serializer<std::pair<A, B>>::size(item, std::forward<CONTEXT>(c)...); });
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
				serialize(output, obj.first, std::forward<CONTEXT>(c)...);
				serialize(output, obj.second, std::forward<CONTEXT>(c)...);
			}
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::pair<A, B>& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(&obj, input, sizeof(std::pair<A, B>));
				input += sizeof(std::pair<A, B>);
			} else {
				deserialize(input, obj.first, std::forward<CONTEXT>(c)...);
				deserialize(input, obj.second, std::forward<CONTEXT>(c)...);
			}
		}
		template<typename ... CONTEXT>
		static size_t size(std::pair<A, B> const& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize)
				return sizeof(std::pair<A, B>);
			else
				return serialize_size(obj.first, std::forward<CONTEXT>(c)...) + serialize_size(obj.second, std::forward<CONTEXT>(c)...);
		}
		constexpr static size_t size() {
			return sizeof(std::pair<A, B>);
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
				serialize(output, std::get<0>(obj), std::forward<CONTEXT>(c)...);
				serialize(output, std::get<1>(obj), std::forward<CONTEXT>(c)...);
				serialize(output, std::get<2>(obj), std::forward<CONTEXT>(c)...);
			}
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::tuple<A, B, C >& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(&obj, input, sizeof(std::tuple<A, B, C>));
				input += sizeof(std::tuple<A, B, C>);
			} else {
				deserialize(input, std::get<0>(obj), std::forward<CONTEXT>(c)...);
				deserialize(input, std::get<1>(obj), std::forward<CONTEXT>(c)...);
				deserialize(input, std::get<2>(obj), std::forward<CONTEXT>(c)...);
			}
		}
		template<typename ... CONTEXT>
		static size_t size(std::tuple<A, B, C> const& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize)
				return sizeof(std::tuple<A, B, C>);
			else
				return serialize_size(std::get<0>(obj), std::forward<CONTEXT>(c)...) +
					serialize_size(std::get<1>(obj), std::forward<CONTEXT>(c)...) +
					serialize_size(std::get<2>(obj), std::forward<CONTEXT>(c)...);
		}
	};

	template<typename A, typename B, typename C, typename D>
	class serializer<std::tuple<A, B, C, D>> {
	public:
		static constexpr bool has_static_size = serializer<A>::has_static_size && serializer<B>::has_static_size && serializer<C>::has_static_size && serializer<D>::has_static_size;
		static constexpr bool has_simple_serialize = serializer<A>::has_simple_serialize && serializer<B>::has_simple_serialize && serializer<C>::has_simple_serialize && serializer<D>::has_simple_serialize;

		template<typename ... CONTEXT>
		static void serialize_object(std::byte* &output, std::tuple<A, B, C, D> const& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(output, &obj, sizeof(std::tuple<A, B, C, D>));
				output += sizeof(std::tuple<A, B, C, D>);
			} else {
				serialize(output, std::get<0>(obj), std::forward<CONTEXT>(c)...);
				serialize(output, std::get<1>(obj), std::forward<CONTEXT>(c)...);
				serialize(output, std::get<2>(obj), std::forward<CONTEXT>(c)...);
				serialize(output, std::get<3>(obj), std::forward<CONTEXT>(c)...);
			}
		}
		template<typename ... CONTEXT>
		static void deserialize_object(std::byte const* &input, std::tuple<A, B, C, D>& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize) {
				memcpy(&obj, input, sizeof(std::tuple<A, B, C, D>));
				input += sizeof(std::tuple<A, B, C, D>);
			} else {
				deserialize(input, std::get<0>(obj), std::forward<CONTEXT>(c)...);
				deserialize(input, std::get<1>(obj), std::forward<CONTEXT>(c)...);
				deserialize(input, std::get<2>(obj), std::forward<CONTEXT>(c)...);
				deserialize(input, std::get<3>(obj), std::forward<CONTEXT>(c)...);
			}
		}
		template<typename ... CONTEXT>
		static size_t size(std::tuple<A, B, C, D> const& obj, CONTEXT&& ... c) {
			if constexpr(has_static_size && has_simple_serialize)
				return sizeof(std::tuple<A, B, C, D>);
			else
				return serialize_size(std::get<0>(obj), std::forward<CONTEXT>(c)...) +
				serialize_size(std::get<1>(obj), std::forward<CONTEXT>(c)...) +
				serialize_size(std::get<2>(obj), std::forward<CONTEXT>(c)...) +
				serialize_size(std::get<3>(obj), std::forward<CONTEXT>(c)...);
		}
	};

	class serialize_file_wrapper {
	private:
		void* file_handle = nullptr;
		void* mapping_handle = nullptr;
		void* mapped_bytes = nullptr;
		uint64_t final_size = 0ui64;
	public:
		serialize_file_wrapper(std::u16string const& file_name);
		serialize_file_wrapper(std::u16string const& file_name, size_t size);
		~serialize_file_wrapper();

		std::byte* get_bytes() const;
		uint64_t get_size() const;
		bool file_valid() const;
		void set_final_size(uint64_t s) { final_size = s; }
	};

	uint64_t impl_get_compressed_upper_bound(uint64_t source_size);
	uint64_t impl_compress(uint64_t source_size, std::byte* source, std::byte* dest); // returns actual size used
	void impl_decompress(uint64_t source_size, std::byte const* source, uint64_t dest_size, std::byte* dest);

	template<typename T, typename ... CONTEXT>
	void serialize_to_file(std::u16string const& file_name, bool compress, serialize_file_header& header, T const& obj, CONTEXT&& ... c) {
		const auto fsize = serialize_size(obj, std::forward<CONTEXT>(c) ...);
		const auto header_size = serialize_size(header);

		header.version = 1ui64;

		if(!compress) {
			header.decompressed_size = 0ui64;

			serialize_file_wrapper file(file_name, header_size + fsize);

			auto ptr = file.get_bytes();

			serialize(ptr, header);
			serialize(ptr, obj, std::forward<CONTEXT>(c) ...);
		} else {
			header.decompressed_size = fsize;

			auto compressed_size = impl_get_compressed_upper_bound(fsize);

			serialize_file_wrapper file(file_name, header_size + compressed_size);

			auto ptr = file.get_bytes();
			serialize(ptr, header);

			std::byte* temp = new std::byte[fsize];
			std::byte* temp_ptr = temp;

			serialize(temp_ptr, obj, std::forward<CONTEXT>(c) ...);

			auto actual_size = impl_compress(fsize, temp, ptr);
			file.set_final_size(actual_size + header_size);

			delete[] temp;
		}
	}

	template<typename T, typename ... CONTEXT>
	void deserialize_from_file(std::u16string const& file_name, T& obj, CONTEXT&& ... c) {
		serialize_file_wrapper file(file_name);

		if(file.file_valid()) {
			std::byte const* ptr = file.get_bytes();
			
			serialize_file_header header_out;
			deserialize(ptr, header_out);

			if(header_out.decompressed_size == 0) {
				deserialize(ptr, obj, header_out.version, std::forward<CONTEXT>(c) ...);
			} else {
				std::byte* temp = new std::byte[header_out.decompressed_size];
				std::byte const* temp_ptr = temp;

				impl_decompress(file.get_size() - serialize_size(header_out), ptr, header_out.decompressed_size, temp);

				deserialize(temp_ptr, obj, header_out.version, std::forward<CONTEXT>(c) ...);
				delete[] temp;
			}

		}
	}
}
