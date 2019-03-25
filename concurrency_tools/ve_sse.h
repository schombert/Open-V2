#pragma once

namespace ve {
	constexpr int32_t vector_size = 4;

	constexpr int32_t full_mask = 0x000F;
	constexpr int32_t empty_mask = 0;

	struct int_vector;

	template<typename tag_type>
	struct tagged_vector;

	struct fp_vector;

	struct mask_vector;

	template<typename T>
	struct ve_identity {
		using type = T;

	};
	struct vbitfield_type {
		using storage = uint8_t;

		uint8_t v;
	};

	RELEASE_INLINE vbitfield_type operator&(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v & b.v) };
	}
	RELEASE_INLINE vbitfield_type operator|(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v | b.v) };
	}
	RELEASE_INLINE vbitfield_type operator^(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v ^ b.v) };
	}
	RELEASE_INLINE vbitfield_type operator~(vbitfield_type a) {
		return vbitfield_type{ uint8_t(~a.v) };
	}
	RELEASE_INLINE vbitfield_type operator!(vbitfield_type a) {
		return vbitfield_type{ uint8_t(~a.v) };
	}
	RELEASE_INLINE vbitfield_type and_not(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v & (~b.v)) };
	}

	struct mask_vector {
		using wrapped_value = bool;

		__m128 value;

		RELEASE_INLINE mask_vector() : value(_mm_setzero_ps()) {}
		RELEASE_INLINE mask_vector(bool b) : value(_mm_castsi128_ps(_mm_set1_epi32(-int32_t(b)))) {}
		RELEASE_INLINE mask_vector(bool a, bool b, bool c, bool d) :
			value(_mm_castsi128_ps(_mm_setr_epi32(-int32_t(a), -int32_t(b), -int32_t(c), -int32_t(d)))) {}
		RELEASE_INLINE mask_vector(vbitfield_type b) {
			const auto repeated_mask = _mm_set1_epi32(b.v);
			const auto mask_filter = _mm_setr_epi32(
				0x00000001, 0x00000002, 0x00000004, 0x00000008);
			value = _mm_castsi128_ps(_mm_xor_si128(_mm_and_si128(repeated_mask, mask_filter), _mm_set1_epi32(-1)));
		}
		RELEASE_INLINE constexpr mask_vector(__m128 v) : value(v) {}
		RELEASE_INLINE constexpr operator __m128() const {
			return value;
		}
		RELEASE_INLINE bool operator[](uint32_t i) const noexcept {
			return _mm_castps_si128(value).m128i_i32[i] != 0;
		}
	};

	struct fp_vector {
		using wrapped_value = float;

		__m128 value;

		RELEASE_INLINE fp_vector() : value(_mm_setzero_ps()) {}
		RELEASE_INLINE constexpr fp_vector(__m128 v) : value(v) {}
		RELEASE_INLINE fp_vector(float v) : value(_mm_set1_ps(v)) {}
		RELEASE_INLINE fp_vector(float a, float b, float c, float d) : value(_mm_setr_ps(a, b, c, d)) {}
		RELEASE_INLINE constexpr operator __m128() const {
			return value;
		}
		RELEASE_INLINE float reduce() const {
			// source: Peter Cordes
			__m128 shuf = _mm_movehdup_ps(value); // broadcast elements 3,1 to 2,0
			__m128 sums = _mm_add_ps(value, shuf);
			shuf = _mm_movehl_ps(shuf, sums); // high half -> low half
			sums = _mm_add_ss(sums, shuf);
			return _mm_cvtss_f32(sums);
		}
		RELEASE_INLINE float operator[](uint32_t i) const noexcept {
			return value.m128_f32[i];
		}
		RELEASE_INLINE void set(uint32_t i, float v) noexcept {
			value.m128_f32[i] = v;
		}
	};

	struct int_vector {
		using wrapped_value = int32_t;

		__m128i value;

		RELEASE_INLINE int_vector() : value(_mm_setzero_si128()) {}
		RELEASE_INLINE constexpr int_vector(__m128i v) : value(v) {}
		RELEASE_INLINE int_vector(int32_t v) : value(_mm_set1_epi32(v)) {}
		RELEASE_INLINE int_vector(uint32_t v) : value(_mm_set1_epi32(int32_t(v))) {}
		RELEASE_INLINE int_vector(int32_t a, int32_t b, int32_t c, int32_t d) :
			value(_mm_setr_epi32(a, b, c, d)) {}
		RELEASE_INLINE int_vector(uint32_t a, uint32_t b, uint32_t c, uint32_t d) :
			value(_mm_setr_epi32(int32_t(a), int32_t(b), int32_t(c), int32_t(d))) {}
		RELEASE_INLINE constexpr operator __m128i() const {
			return value;
		}
		RELEASE_INLINE int32_t operator[](uint32_t i) const noexcept {
				return value.m128i_i32[i];
		}
		RELEASE_INLINE void set(uint32_t i, int32_t v) noexcept {
				value.m128i_i32[i] = v;
		}
	};

	template<typename tag_type>
	struct tagged_vector {
		using wrapped_value = tag_type;
		static_assert(sizeof(tag_type::value_base_t) <= 4);

		__m128i value;

		RELEASE_INLINE tagged_vector() : value(tag_type::zero_is_null_t::value ? _mm_setzero_si128() : _mm_set1_epi32(-1)) {}
		RELEASE_INLINE constexpr tagged_vector(__m128i v) : value(v) {}
		RELEASE_INLINE tagged_vector(tag_type v) : value(_mm_set1_epi32(tag_type::zero_is_null_t::value ? int32_t(v.value) : to_index(v))) {}
		RELEASE_INLINE tagged_vector(tag_type a, tag_type b, tag_type c, tag_type d) :
			value(tag_type::zero_is_null_t::value ?
				_mm_setr_epi32(int32_t(a.value), int32_t(b.value), int32_t(c.value), int32_t(d.value)) :
				_mm_setr_epi32(to_index(a), to_index(b), to_index(c), to_index(d))
			) {}

		RELEASE_INLINE constexpr operator __m128i() const {
			return value;
		}

		RELEASE_INLINE tag_type operator[](uint32_t i) const noexcept {
			if constexpr(tag_type::zero_is_null_t::value)
				return tag_type(typename tag_type::value_base_t(value.m128i_i32[i]), std::true_type());
			else
				return tag_type(typename tag_type::value_base_t(value.m128i_i32[i]));
		}
		RELEASE_INLINE void set(uint32_t i, tag_type v) noexcept {
			if constexpr(tag_type::zero_is_null_t::value)
				value.m128i_i32[i] = int32_t(v.value);
			else
				value.m128i_i32[i] = to_index(v);
		}
	};

	template<typename tag_type>
	RELEASE_INLINE int_vector to_int(tagged_vector<tag_type> v) {
		return v.value;
	}

	template<typename value_base, typename individuator>
	RELEASE_INLINE int32_t to_int(tag_type<value_base, std::true_type, individuator> v) {
		return int32_t(v.value);
	}

	template<>
	struct tagged_vector<union_tag> {
		using tag_type = union_tag;
		using wrapped_value = tag_type;

		__m128i value;

		RELEASE_INLINE tagged_vector() : value(_mm_setzero_si128()) {}
		RELEASE_INLINE constexpr tagged_vector(__m128i v) : value(v) {}
		template<typename T>
		RELEASE_INLINE constexpr tagged_vector(tagged_vector<T> v) : value(v.value) {}
		RELEASE_INLINE tagged_vector(tag_type v) : value(_mm_set1_epi32(v.value)) {}
		RELEASE_INLINE tagged_vector(tag_type a, tag_type b, tag_type c, tag_type d) :
			value(_mm_setr_epi32(a.value, b.value, c.value, d.value)) {}

		RELEASE_INLINE constexpr operator __m128i() const {
			return value;
		}

		RELEASE_INLINE union_tag operator[](uint32_t i) const noexcept {
			return union_tag(value.m128i_i32[i]);
		}
		RELEASE_INLINE void set(uint32_t i, union_tag v) noexcept {
			value.m128i_i32[i] = v.value;
		}
		template<typename T>
		RELEASE_INLINE constexpr operator tagged_vector<T>() {
			return tagged_vector<T>(value);
		}
	};

	using union_tag_vector = tagged_vector<union_tag>;

	template<typename tag_type>
	struct contiguous_tags_base {
		uint32_t value = 0;
		using wrapped_value = tag_type;

		constexpr contiguous_tags_base() : value(0) {}
		constexpr explicit contiguous_tags_base(uint32_t v) : value(v) {}
		constexpr contiguous_tags_base(const contiguous_tags_base& v) noexcept = default;
		constexpr contiguous_tags_base(contiguous_tags_base&& v) noexcept = default;

		template<typename T, typename = std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>> >
		constexpr contiguous_tags_base(contiguous_tags_base<T> v) : value(v.value) {}

		contiguous_tags_base& operator=(contiguous_tags_base&& v) noexcept = default;
		contiguous_tags_base& operator=(contiguous_tags_base const& v) noexcept = default;

		template<typename T>
		std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>, contiguous_tags_base&> operator=(contiguous_tags_base<T> v) noexcept {
			value = v.value;
			return *this;
		}

		RELEASE_INLINE tag_type operator[](uint32_t i) const noexcept {
			return tag_type(typename tag_type::value_base_t(value + i));
		}

		constexpr bool operator==(contiguous_tags_base<tag_type> o) const noexcept {
			return value == o.value;
		}
		constexpr bool operator!=(contiguous_tags_base<tag_type> o) const noexcept {
			return value != o.value;
		}
	};

	template<typename tag_type, int32_t b_index = 0>
	struct contiguous_tags : public contiguous_tags_base<tag_type> {
		constexpr static int32_t block_index = b_index;

		constexpr contiguous_tags() : contiguous_tags_base<tag_type>(0) {}
		constexpr explicit contiguous_tags(uint32_t v) : contiguous_tags_base<tag_type>(v) {}
		constexpr contiguous_tags(const contiguous_tags& v) noexcept = default;
		constexpr contiguous_tags(contiguous_tags&& v) noexcept = default;

		template<typename T, typename = std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>> >
		constexpr contiguous_tags(contiguous_tags<T> v) : contiguous_tags_base<tag_type>(v.value) {}

		contiguous_tags& operator=(contiguous_tags&& v) noexcept = default;
		contiguous_tags& operator=(contiguous_tags const& v) noexcept = default;

		template<typename T>
		std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>, contiguous_tags&> operator=(contiguous_tags<T> v) noexcept {
			contiguous_tags_base<tag_type>::value = v.value;
			return *this;
		}
	};

	template<typename tag_type, int32_t b_index = 0>
	struct unaligned_contiguous_tags : public contiguous_tags_base<tag_type> {
		constexpr static int32_t block_index = b_index;

		constexpr unaligned_contiguous_tags() : contiguous_tags_base<tag_type>(0) {}
		constexpr explicit unaligned_contiguous_tags(uint32_t v) : contiguous_tags_base<tag_type>(v) {}
		constexpr unaligned_contiguous_tags(const unaligned_contiguous_tags& v) noexcept = default;
		constexpr unaligned_contiguous_tags(unaligned_contiguous_tags&& v) noexcept = default;

		template<typename T, typename = std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>> >
		constexpr unaligned_contiguous_tags(unaligned_contiguous_tags<T> v) : contiguous_tags_base<tag_type>(v.value) {}

		unaligned_contiguous_tags& operator=(unaligned_contiguous_tags&& v) noexcept = default;
		unaligned_contiguous_tags& operator=(unaligned_contiguous_tags const& v) noexcept = default;

		template<typename T>
		std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>, unaligned_contiguous_tags&> operator=(unaligned_contiguous_tags<T> v) noexcept {
			contiguous_tags_base<tag_type>::value = v.value;
			return *this;
		}
	};

	template<typename tag_type>
	struct partial_contiguous_tags : public contiguous_tags_base<tag_type> {
		constexpr static int32_t block_index = 0;

		uint32_t subcount = vector_size;

		constexpr partial_contiguous_tags() : contiguous_tags_base<tag_type>(0), subcount(vector_size) {}
		constexpr explicit partial_contiguous_tags(uint32_t v, uint32_t s) : contiguous_tags_base<tag_type>(v), subcount(s) {}
		constexpr partial_contiguous_tags(const partial_contiguous_tags& v) noexcept = default;
		constexpr partial_contiguous_tags(partial_contiguous_tags&& v) noexcept = default;

		template<typename T, typename = std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>> >
		constexpr partial_contiguous_tags(partial_contiguous_tags<T> v) : contiguous_tags_base<tag_type>(v.value), subcount(v.subcount) {}

		partial_contiguous_tags& operator=(partial_contiguous_tags&& v) noexcept = default;
		partial_contiguous_tags& operator=(partial_contiguous_tags const& v) noexcept = default;

		template<typename T>
		std::enable_if_t<std::is_constructible_v<tag_type, T> && !std::is_same_v<tag_type, T>, partial_contiguous_tags&> operator=(partial_contiguous_tags<T> v) noexcept {
			contiguous_tags_base<tag_type>::value = v.value;
			subcount = v.subcount;
			return *this;
		}
	};

	template<typename T>
	struct value_to_vector_type_s;

	template<>
	struct value_to_vector_type_s<int32_t> {
		using type = int_vector;
	};
	template<>
	struct value_to_vector_type_s<int16_t> {
		using type = int_vector;
	};
	template<>
	struct value_to_vector_type_s<uint16_t> {
		using type = int_vector;
	};
	template<>
	struct value_to_vector_type_s<int8_t> {
		using type = int_vector;
	};
	template<>
	struct value_to_vector_type_s<uint8_t> {
		using type = int_vector;
	};
	template<>
	struct value_to_vector_type_s<void> {
		using type = void;
	};
	template<>
	struct value_to_vector_type_s<uint32_t> {
		using type = int_vector;
	};
	template<>
	struct value_to_vector_type_s<float> {
		using type = fp_vector;
	};
	template<>
	struct value_to_vector_type_s<bool> {
		using type = mask_vector;
	};
	template<>
	struct value_to_vector_type_s<union_tag> {
		using type = union_tag_vector;
	};

	template<typename value_base, typename individuator>
	struct value_to_vector_type_s<tag_type<value_base, std::true_type, individuator>> {
		using type = tagged_vector<tag_type<value_base, std::true_type, individuator>>;
	};

	template<>
	struct value_to_vector_type_s<int_vector> {
		using type = int_vector;
	};
	template<>
	struct value_to_vector_type_s<fp_vector> {
		using type = fp_vector;
	};
	template<>
	struct value_to_vector_type_s<mask_vector> {
		using type = mask_vector;
	};
	template<typename T>
	struct value_to_vector_type_s<tagged_vector<T>> {
		using type = tagged_vector<T>;
	};
	template<typename T, int32_t i>
	struct value_to_vector_type_s<contiguous_tags<T, i>> {
		using type = contiguous_tags<T, i>;
	};
	template<typename T, int32_t i>
	struct value_to_vector_type_s<unaligned_contiguous_tags<T, i>> {
		using type = unaligned_contiguous_tags<T, i>;
	};
	template<typename T>
	struct value_to_vector_type_s<partial_contiguous_tags<T>> {
		using type = partial_contiguous_tags<T>;
	};

	template<typename T>
	using value_to_vector_type = typename value_to_vector_type_s<T>::type;

	template<typename T>
	struct is_vector_type_s {
		constexpr static bool value = false;
	};
	template<>
	struct is_vector_type_s<int_vector> {
		constexpr static bool value = true;
	};
	template<>
	struct is_vector_type_s<fp_vector> {
		constexpr static bool value = true;
	};
	template<>
	struct is_vector_type_s<mask_vector> {
		constexpr static bool value = true;
	};
	template<typename T>
	struct is_vector_type_s<tagged_vector<T>> {
		constexpr static bool value = true;
	};
	template<typename T, int32_t i>
	struct is_vector_type_s<contiguous_tags<T, i>> {
		constexpr static bool value = true;
	};
	template<typename T, int32_t i>
	struct is_vector_type_s<unaligned_contiguous_tags<T, i>> {
		constexpr static bool value = true;
	};
	template<typename T>
	struct is_vector_type_s<partial_contiguous_tags<T>> {
		constexpr static bool value = true;
	};

	template<typename T>
	constexpr bool is_vector_type = is_vector_type_s<T>::value;

	template<typename ... T>
	struct any_is_vector_type;

	template<>
	struct any_is_vector_type<> { constexpr static bool value = false; };

	template<typename F, typename ... R>
	struct any_is_vector_type<F, R...> { constexpr static bool value = is_vector_type<F> || any_is_vector_type<R...>::value; };

	template<typename TO, typename FROM>
	RELEASE_INLINE auto widen_to(FROM v) -> std::conditional_t<is_vector_type<TO>, value_to_vector_type<std::remove_cv_t<FROM>>, FROM> { return v; }

	template<typename TO, typename ... REST, typename FROM>
	RELEASE_INLINE auto widen_to(FROM v) -> std::conditional_t<is_vector_type<TO>, value_to_vector_type<std::remove_cv_t<FROM>>, decltype(widen_to<REST ...>(v))> { return v; }

	template<uint32_t i, typename T>
	RELEASE_INLINE std::enable_if_t<is_vector_type<T>, typename T::wrapped_value> nth_item(T v) {
		return v[i];
	}
	template<uint32_t i, typename T>
	RELEASE_INLINE std::enable_if_t<!is_vector_type<T>, T> nth_item(T v) {
		return v;
	}

	template<typename ... T>
	struct any_is_partial_s;

	template<>
	struct any_is_partial_s<> {
		constexpr static bool value = false;
	};

	template<typename ttype, typename ... T>
	struct any_is_partial_s<partial_contiguous_tags<ttype>, T...> {
		constexpr static bool value = true;
	};

	template<typename first, typename ... T>
	struct any_is_partial_s<first, T...> {
		constexpr static bool value = any_is_partial_s<T...>::value;
	};

	template<typename ... T>
	constexpr bool any_is_partial = any_is_partial_s<T...>::value;

	RELEASE_INLINE constexpr uint32_t minimum_partial() {
		return uint32_t(vector_size);
	}

	template<typename ttype, typename ... T>
	RELEASE_INLINE uint32_t minimum_partial(partial_contiguous_tags<ttype> p, T... args) {
		return std::min(p.subcount, minimum_partial(args ...));
	}

	template<typename first, typename ... T>
	RELEASE_INLINE uint32_t minimum_partial(first, T... args) {
		return minimum_partial(args ...);
	}

	template<typename FUNC, typename ... PARAMS>
	RELEASE_INLINE auto ve_apply(FUNC&& f, PARAMS ... params) {
		if constexpr(any_is_partial<PARAMS ...>) {
			const uint32_t limit = minimum_partial(params ...);

			if constexpr(std::is_same_v<decltype(f(nth_item<0ui32>(params) ...)), void>) {
				if(limit > 0ui32) f(nth_item<0ui32>(params) ...);
				if(limit > 1ui32) f(nth_item<1ui32>(params) ...);
				if(limit > 2ui32) f(nth_item<2ui32>(params) ...);
				if(limit > 3ui32) f(nth_item<3ui32>(params) ...);
			} else {
				return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
					(limit > 0ui32) ? f(nth_item<0ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 1ui32) ? f(nth_item<1ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 2ui32) ? f(nth_item<2ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 3ui32) ? f(nth_item<3ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))()
					);
			}
		} else if constexpr(any_is_vector_type<PARAMS ...>::value) {
			if constexpr(std::is_same_v<decltype(f(nth_item<0ui32>(params) ...)), void>) {
				f(nth_item<0ui32>(params) ...);
				f(nth_item<1ui32>(params) ...);
				f(nth_item<2ui32>(params) ...);
				f(nth_item<3ui32>(params) ...);
			} else {
				return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
					f(nth_item<0ui32>(params) ...),
					f(nth_item<1ui32>(params) ...),
					f(nth_item<2ui32>(params) ...),
					f(nth_item<3ui32>(params) ...)
					);
			}
		} else {
			return f(params ...);
		}
	}

	template<typename A, typename FUNC>
	RELEASE_INLINE auto apply(A a, FUNC&& f) {
		return ve_apply(std::forward<FUNC>(f), a);
	}
	template<typename A, typename B, typename FUNC>
	RELEASE_INLINE auto apply(A a, B b, FUNC&& f) {
		return ve_apply(std::forward<FUNC>(f), a, b);
	}
	template<typename A, typename B, typename C, typename FUNC>
	RELEASE_INLINE auto apply(A a, B b, C c, FUNC&& f) {
		return ve_apply(std::forward<FUNC>(f), a, b, c);
	}

	template<typename ... PARAMS, typename FUNC>
	RELEASE_INLINE auto apply_with_indices(FUNC&& f, PARAMS ... params)
		->  value_to_vector_type<decltype(f(0ui32, nth_item<0ui32>(params) ...))> {
		if constexpr(any_is_partial<PARAMS ...>) {
			const uint32_t limit = minimum_partial(params ...);

			if constexpr(std::is_same_v<decltype(f(0ui32, nth_item<0ui32>(params) ...)), void>) {
				if(limit > 0ui32) f(0ui32, nth_item<0ui32>(params) ...);
				if(limit > 1ui32) f(1ui32, nth_item<1ui32>(params) ...);
				if(limit > 2ui32) f(2ui32, nth_item<2ui32>(params) ...);
				if(limit > 3ui32) f(3ui32, nth_item<3ui32>(params) ...);
			} else {
				return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
					(limit > 0ui32) ? f(0ui32, nth_item<0ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 1ui32) ? f(1ui32, nth_item<1ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 2ui32) ? f(2ui32, nth_item<2ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 3ui32) ? f(3ui32, nth_item<3ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))()
					);
			}
		} else if constexpr(std::is_same_v<decltype(f(0ui32, nth_item<0ui32>(params) ...)), void>) {
			f(0ui32, nth_item<0ui32>(params) ...);
			f(1ui32, nth_item<1ui32>(params) ...);
			f(2ui32, nth_item<2ui32>(params) ...);
			f(3ui32, nth_item<3ui32>(params) ...);
		} else {
			return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
				f(0ui32, nth_item<0ui32>(params) ...),
				f(1ui32, nth_item<1ui32>(params) ...),
				f(2ui32, nth_item<2ui32>(params) ...),
				f(3ui32, nth_item<3ui32>(params) ...)
				);
		}
	}

	RELEASE_INLINE fp_vector to_float(int_vector v) {
		return _mm_cvtepi32_ps(v);
	}

	RELEASE_INLINE fp_vector operator+(fp_vector a, fp_vector b) {
		return _mm_add_ps(a, b);
	}
	RELEASE_INLINE fp_vector operator-(fp_vector a, fp_vector b) {
		return _mm_sub_ps(a, b);
	}
	RELEASE_INLINE fp_vector operator*(fp_vector a, fp_vector b) {
		return _mm_mul_ps(a, b);
	}
	RELEASE_INLINE fp_vector operator/(fp_vector a, fp_vector b) {
		return _mm_div_ps(a, b);
	}


	RELEASE_INLINE int_vector operator+(int_vector a, int_vector b) {
		return _mm_add_epi32(a, b);
	}
	RELEASE_INLINE int_vector operator-(int_vector a, int_vector b) {
		return _mm_sub_epi32(a, b);
	}
	RELEASE_INLINE int_vector operator*(int_vector a, int_vector b) {
		return _mm_mullo_epi32(a, b);
	}

	RELEASE_INLINE mask_vector operator&(mask_vector a, mask_vector b) {
		return _mm_and_ps(a, b);
	}
	RELEASE_INLINE mask_vector operator|(mask_vector a, mask_vector b) {
		return _mm_or_ps(a, b);
	}
	RELEASE_INLINE mask_vector operator^(mask_vector a, mask_vector b) {
		return _mm_xor_ps(a, b);
	}
	RELEASE_INLINE mask_vector operator~(mask_vector a) {
		return _mm_xor_ps(a, mask_vector(true));
	}
	RELEASE_INLINE mask_vector operator!(mask_vector a) {
		return _mm_xor_ps(a, mask_vector(true));
	}
	RELEASE_INLINE mask_vector and_not(mask_vector a, mask_vector b) {
		return _mm_andnot_ps(b, a);
	}

	RELEASE_INLINE fp_vector inverse(fp_vector a) {
		return _mm_rcp_ps(a);
	}
	RELEASE_INLINE fp_vector sqrt(fp_vector a) {
		return _mm_sqrt_ps(a);
	}
	RELEASE_INLINE fp_vector inverse_sqrt(fp_vector a) {
		return _mm_rsqrt_ps(a);
	}

	RELEASE_INLINE fp_vector multiply_and_add(fp_vector a, fp_vector b, fp_vector c) {
		return (a * b) + c;
	}
	RELEASE_INLINE fp_vector multiply_and_subtract(fp_vector a, fp_vector b, fp_vector c) {
		return (a * b) - c;
	}
	RELEASE_INLINE fp_vector negate_multiply_and_add(fp_vector a, fp_vector b, fp_vector c) {
		return c - (a * b);
	}
	RELEASE_INLINE fp_vector negate_multiply_and_subtract(fp_vector a, fp_vector b, fp_vector c) {
		return (0.0f - c) - (a * b);
	}

	RELEASE_INLINE fp_vector min(fp_vector a, fp_vector b) {
		return _mm_min_ps(a, b);
	}
	RELEASE_INLINE fp_vector max(fp_vector a, fp_vector b) {
		return _mm_max_ps(a, b);
	}
	RELEASE_INLINE fp_vector floor(fp_vector a) {
		return _mm_floor_ps(a);
	}
	RELEASE_INLINE fp_vector ceil(fp_vector a) {
		return _mm_ceil_ps(a);
	}

	RELEASE_INLINE mask_vector operator<(fp_vector a, fp_vector b) {
		return _mm_cmp_ps(a, b, _CMP_LT_OQ);
	}
	RELEASE_INLINE mask_vector operator>(fp_vector a, fp_vector b) {
		return _mm_cmp_ps(a, b, _CMP_GT_OQ);
	}
	RELEASE_INLINE mask_vector operator<=(fp_vector a, fp_vector b) {
		return _mm_cmp_ps(a, b, _CMP_LE_OQ);
	}
	RELEASE_INLINE mask_vector operator>=(fp_vector a, fp_vector b) {
		return _mm_cmp_ps(a, b, _CMP_GE_OQ);
	}
	RELEASE_INLINE mask_vector operator==(fp_vector a, fp_vector b) {
		return _mm_cmp_ps(a, b, _CMP_EQ_OQ);
	}
	RELEASE_INLINE mask_vector operator!=(fp_vector a, fp_vector b) {
		return _mm_cmp_ps(a, b, _CMP_NEQ_OQ);
	}

	RELEASE_INLINE mask_vector operator<(int_vector a, int_vector b) {
		return _mm_castsi128_ps(_mm_cmpgt_epi32(b, a));
	}
	RELEASE_INLINE mask_vector operator>(int_vector a, int_vector b) {
		return _mm_castsi128_ps(_mm_cmpgt_epi32(a, b));
	}
	RELEASE_INLINE mask_vector operator==(int_vector a, int_vector b) {
		return _mm_castsi128_ps(_mm_cmpeq_epi32(a, b));
	}
	RELEASE_INLINE mask_vector operator!=(int_vector a, int_vector b) {
		return (a > b) | (b > a);
	}
	RELEASE_INLINE mask_vector operator<=(int_vector a, int_vector b) {
		return (a < b) | (a == b);
	}
	RELEASE_INLINE mask_vector operator>=(int_vector a, int_vector b) {
		return (a > b) | (a == b);
	}
	RELEASE_INLINE mask_vector operator==(mask_vector a, mask_vector b) {
		return _mm_cmp_ps(a, b, _CMP_EQ_OQ);
	}
	RELEASE_INLINE mask_vector operator!=(mask_vector a, mask_vector b) {
		return _mm_cmp_ps(a, b, _CMP_NEQ_OQ);
	}

	template<typename T>
	RELEASE_INLINE mask_vector operator==(tagged_vector<T> a, tagged_vector<T> b) {
		return int_vector(a.value) == int_vector(b.value);
	}
	template<typename T>
	RELEASE_INLINE mask_vector operator!=(tagged_vector<T> a, tagged_vector<T> b) {
		return int_vector(a.value) != int_vector(b.value);
	}

	template<typename T>
	RELEASE_INLINE mask_vector operator==(tagged_vector<T> a, typename ve_identity<T>::type b) {
		return a == tagged_vector<T>(b);
	}
	template<typename T>
	RELEASE_INLINE mask_vector operator!=(tagged_vector<T> a, typename ve_identity<T>::type b) {
		return a != tagged_vector<T>(b);
	}

	template<typename T>
	RELEASE_INLINE mask_vector operator==(typename ve_identity<T>::type a, tagged_vector<T> b) {
		return b == tagged_vector<T>(a);
	}
	template<typename T>
	RELEASE_INLINE mask_vector operator!=(typename ve_identity<T>::type a, tagged_vector<T> b) {
		return b != tagged_vector<T>(a);
	}

	template<typename tag_type>
	RELEASE_INLINE mask_vector operator==(contiguous_tags_base<tag_type> a, tagged_vector<typename ve_identity<tag_type>::type> b) {
		return tagged_vector<tag_type>(
			tag_type(typename tag_type::value_base_t(a.value)),
			tag_type(typename tag_type::value_base_t(a.value + 1)),
			tag_type(typename tag_type::value_base_t(a.value + 2)),
			tag_type(typename tag_type::value_base_t(a.value + 3))) == b;
	}
	template<typename tag_type>
	RELEASE_INLINE mask_vector operator!=(contiguous_tags_base<tag_type> a, tagged_vector<typename ve_identity<tag_type>::type> b) {
		return tagged_vector<tag_type>(
			tag_type(typename tag_type::value_base_t(a.value)),
			tag_type(typename tag_type::value_base_t(a.value + 1)),
			tag_type(typename tag_type::value_base_t(a.value + 2)),
			tag_type(typename tag_type::value_base_t(a.value + 3))) != b;
	}


	template<typename tag_type>
	RELEASE_INLINE mask_vector operator==(tagged_vector<typename ve_identity<tag_type>::type> b, contiguous_tags_base<tag_type> a) {
		return tagged_vector<tag_type>(
			tag_type(typename tag_type::value_base_t(a.value)),
			tag_type(typename tag_type::value_base_t(a.value + 1)),
			tag_type(typename tag_type::value_base_t(a.value + 2)),
			tag_type(typename tag_type::value_base_t(a.value + 3))) == b;
	}
	template<typename tag_type>
	RELEASE_INLINE mask_vector operator!=(tagged_vector<typename ve_identity<tag_type>::type> b, contiguous_tags_base<tag_type> a) {
		return tagged_vector<tag_type>(
			tag_type(typename tag_type::value_base_t(a.value)),
			tag_type(typename tag_type::value_base_t(a.value + 1)),
			tag_type(typename tag_type::value_base_t(a.value + 2)),
			tag_type(typename tag_type::value_base_t(a.value + 3))) != b;
	}

	RELEASE_INLINE mask_vector bit_test(int_vector val, int32_t bits) {
		auto const bit_vector = _mm_set1_epi32(bits);
		return _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_and_si128(val, bit_vector), bit_vector));
	}

	RELEASE_INLINE fp_vector select(vbitfield_type mask, fp_vector a, fp_vector b) {
		const auto repeated_mask = _mm_set1_epi32(mask.v);
		const auto mask_filter = _mm_setr_epi32(
			0x00000001, 0x00000002, 0x00000004, 0x00000008);
		auto fp_mask = mask_vector(_mm_castsi128_ps(_mm_and_si128(repeated_mask, mask_filter))) != mask_vector();
		return _mm_blendv_ps(b, a, fp_mask);
	}
	RELEASE_INLINE mask_vector widen_mask(vbitfield_type mask) {
		const auto repeated_mask = _mm_set1_epi32(mask.v);
		const auto mask_filter = _mm_setr_epi32(
			0x00000001, 0x00000002, 0x00000004, 0x00000008);
		return mask_vector(_mm_castsi128_ps(_mm_and_si128(repeated_mask, mask_filter))) != mask_vector();
	}
	RELEASE_INLINE fp_vector select(mask_vector mask, fp_vector a, fp_vector b) {
		return _mm_blendv_ps(b, a, mask);
	}

	RELEASE_INLINE int_vector select(mask_vector mask, int_vector a, int_vector b) {
		return _mm_blendv_epi8(b, a, _mm_castps_si128(mask));
	}

	template<typename T>
	RELEASE_INLINE tagged_vector<T> select(mask_vector mask, tagged_vector<T> a, tagged_vector<typename ve_identity<T>::type> b) {
		return _mm_blendv_epi8(b, a, _mm_castps_si128(mask));
	}

	RELEASE_INLINE mask_vector is_non_zero(int_vector i) {
		return i != int_vector();
	}
	RELEASE_INLINE mask_vector is_zero(int_vector i) {
		return i == int_vector();
	}

	template<typename T>
	RELEASE_INLINE mask_vector is_valid_index(tagged_vector<T> i) {
		return i != tagged_vector<T>();
	}
	template<typename T>
	RELEASE_INLINE mask_vector is_invalid(tagged_vector<T> i) {
		return i == tagged_vector<T>();
	}

	RELEASE_INLINE int32_t compress_mask(mask_vector mask) {
		return _mm_movemask_ps(mask);
	}


	inline constexpr uint32_t load_masks[16] = {
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
	};

	template<typename F>
	class alignas(__m128i) true_accumulator : public F {
	private:
		__m128i value;
		uint32_t index = 0;
		int32_t accumulated_mask = 0;
	public:
		bool result = false;

		true_accumulator(F&& f) : value(_mm_setzero_si128()), F(std::move(f)) {}

		void add_value(int32_t v) {
			if(!result) {
				accumulated_mask |= (int32_t(v != 0) << index);
				value.m128i_i32[index++] = v;

				if(index == 4) {
					result = (ve::compress_mask(F::operator()(value)) & accumulated_mask) != 0;
					value = _mm_setzero_si128();
					index = 0;
					accumulated_mask = 0;
				}
			}
		}
		void flush() {
			if(int32_t(index != 0) & ~int32_t(result)) {
				result = (ve::compress_mask(F::operator()(value)) & accumulated_mask) != 0;
				index = 0;
			}
		}
	};

	template<typename F>
	class alignas(__m128i) false_accumulator : public F {
	private:
		__m128i value;
		uint32_t index = 0;
		int32_t accumulated_mask = 0;
	public:
		bool result = true;

		false_accumulator(F&& f) : value(_mm_setzero_si128()), F(std::move(f)) {}

		void add_value(int32_t v) {
			if(result) {
				accumulated_mask |= (int32_t(v != 0) << index);
				value.m128i_i32[index++] = v;

				if(index == 4) {
					result = (ve::compress_mask(F::operator()(value)) & accumulated_mask) == accumulated_mask;
					value = _mm_setzero_si128();
					index = 0;
					accumulated_mask = 0;
				}
			}
		}
		void flush() {
			if((index != 0) & result) {
				result = (ve::compress_mask(F::operator()(value)) & accumulated_mask) == accumulated_mask;
				index = 0;
			}
		}
	};

	template<typename TAG, typename F>
	class alignas(__m256i) value_accumulator : public F {
	private:
		fp_vector value;
		tagged_vector<TAG> store;

		uint32_t index = 0;
		int32_t accumulated_mask = 0;
	public:

		value_accumulator(F&& f) : F(std::move(f)) {}

		void add_value(TAG v) {
			accumulated_mask |= (int32_t(is_valid_index(v)) << index);
			store.set(index++, v);

			if(index == ve::vector_size) {
				value = value + ve::select(accumulated_mask, F::operator()(store), 0.0f);
				index = 0;
				accumulated_mask = 0;
			}

		}
		float flush() {
			if(index != 0) {
				value = value + ve::select(accumulated_mask, F::operator()(store), 0.0f);
				index = 0;
			}

			return value.reduce();
		}
	};


	template<typename F>
	auto make_true_accumulator(F&& f) -> true_accumulator<F> {
		return true_accumulator<F>(std::forward<F>(f));
	}

	template<typename F>
	auto make_false_accumulator(F&& f) -> false_accumulator<F> {
		return false_accumulator<F>(std::forward<F>(f));
	}

	template<typename TAG, typename F>
	auto make_value_accumulator(F&& f) -> value_accumulator<TAG, F> {
		return value_accumulator<TAG, F>(std::forward<F>(f));
	}


	template<typename T, int32_t i, typename U>
	RELEASE_INLINE U partial_mask(contiguous_tags<T, i> e, U value) {
		return value;
	}
	template<typename T, int32_t i, typename U>
	RELEASE_INLINE U partial_mask(unaligned_contiguous_tags<T, i> e, U value) {
		return value;
	}
	template<typename T, typename U>
	RELEASE_INLINE U partial_mask(partial_contiguous_tags<T> e, U value) {
		mask_vector mask = _mm_loadu_ps((float const*)(load_masks + 4ui32 - e.subcount));
		return select(mask, value, U());
	}
	template<typename U>
	RELEASE_INLINE U partial_mask(int_vector indices, U value) {
		return value;
	}
	template<typename T, typename U>
	RELEASE_INLINE U partial_mask(tagged_vector<T> indices, U value) {
		return value;
	}


	template<int32_t i>
	RELEASE_INLINE vbitfield_type load(contiguous_tags<int32_t, i> e, bitfield_type const* source) {
		return vbitfield_type{ uint8_t(((source[e.value / 8ui32].v) >> (e.value & 0x00000004)) & 0x0000000F) };
	}
	template<int32_t i>
	RELEASE_INLINE vbitfield_type load(unaligned_contiguous_tags<int32_t, i> e, bitfield_type const* source) {
		return vbitfield_type{ uint8_t(((source[e.value / 8ui32].v) >> (e.value & 0x00000004)) & 0x0000000F) };
	}
	RELEASE_INLINE vbitfield_type load(partial_contiguous_tags<int32_t> e, bitfield_type const* source) {
		return vbitfield_type{ uint8_t(((source[e.value / 8ui32].v) >> (e.value & 0x00000004)) & 0x0000000F) };
	}

	RELEASE_INLINE mask_vector load(int_vector indices, bitfield_type const* source) {
		const auto mask = _mm_set1_epi32(0x00000007);
		const auto low_bit_mask = _mm_set1_epi32(0x00000001);

		const auto byte_indices_low = _mm_srai_epi32(indices, 3);
		auto const gathered_low = _mm_setr_epi32(
			source[byte_indices_low.m128i_i32[0]].v,
			source[byte_indices_low.m128i_i32[1]].v,
			source[byte_indices_low.m128i_i32[2]].v,
			source[byte_indices_low.m128i_i32[3]].v);
		const auto bit_indices_low = _mm_and_si128(indices, mask);
		auto const gathered_low_bits = _mm_and_si128(_mm_srlv_epi32(gathered_low, bit_indices_low), low_bit_mask);

		return _mm_castsi128_ps(_mm_sub_epi32(_mm_setzero_si128(), gathered_low_bits));
	}

	template<int32_t i, typename U>
	RELEASE_INLINE auto load(contiguous_tags<int32_t, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		assert((intptr_t(source + e.value) & 15) == 0);
		if constexpr(std::is_same_v<U, float>)
			return _mm_load_ps(source + e.value);
		else
			return _mm_load_si128((__m128i const*)(source + e.value));
	}
	template<int32_t i, typename U>
	RELEASE_INLINE auto load(unaligned_contiguous_tags<int32_t, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		if constexpr(std::is_same_v<U, float>)
			return _mm_loadu_ps(source + e.value);
		else
			return _mm_loadu_si128((__m128i const*)(source + e.value));
	}
	template<typename U>
	RELEASE_INLINE auto load(partial_contiguous_tags<int32_t> e, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		
		if constexpr(std::is_same_v<U, float>) {
			auto const mask = _mm_loadu_ps((float const*)(load_masks) + 4ui32 - e.subcount);
			auto const v = _mm_loadu_ps(source + e.value);
			return _mm_blendv_ps(_mm_setzero_ps(), v, mask);
		} else {
			auto const mask = _mm_loadu_si128((__m128i const*)(load_masks + 4ui32 - e.subcount));
			auto const v = _mm_loadu_si128((__m128i const*)(source + e.value));
			return _mm_blendv_epi8(_mm_setzero_si128(), v, mask);
		}
	}
	template<typename U>
	RELEASE_INLINE auto load(int_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		if constexpr(std::is_same_v<U, float>) {
			return _mm_setr_ps(
				source[indices[0]],
				source[indices[1]],
				source[indices[2]],
				source[indices[3]]
			);
		} else {
			return _mm_setr_epi32(
				source[indices[0]],
				source[indices[1]],
				source[indices[2]],
				source[indices[3]]
			);
		}
	}

#pragma warning( push )
#pragma warning( disable : 4245)

	template<int32_t i, typename U>
	RELEASE_INLINE auto load(contiguous_tags<int32_t, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			auto const vl = _mm_loadl_epi64((const __m128i *)(source + e.value));
			return _mm_cvtepi16_epi32(vl);
		} else {
			auto const vl = _mm_loadl_epi64((const __m128i *)(source + e.value));
			return _mm_cvtepu16_epi32(vl);
		}
	}
	template<int32_t i, typename U>
	RELEASE_INLINE auto load(unaligned_contiguous_tags<int32_t, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			auto const vl = _mm_loadl_epi64((const __m128i *)(source + e.value));
			return _mm_cvtepi16_epi32(vl);
		} else {
			auto const vl = _mm_loadl_epi64((const __m128i *)(source + e.value));
			return _mm_cvtepu16_epi32(vl);
		}
	}
	template<typename U>
	RELEASE_INLINE auto load(partial_contiguous_tags<int32_t> e, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			auto const vl = _mm_loadl_epi64((const __m128i *)(source + e.value));
			auto const cl = _mm_cvtepi16_epi32(vl);

			auto const mask_l = _mm_loadu_si128((const __m128i *)(load_masks + 4ui32 - e.subcount));
			return _mm_blendv_epi8(_mm_setzero_si128(), cl, mask_l);
			
		} else {
			auto const vl = _mm_loadl_epi64((const __m128i *)(source + e.value));
			auto const cl = _mm_cvtepu16_epi32(vl);
			
			auto const mask_l = _mm_loadu_si128((const __m128i *)(load_masks + 4ui32 - e.subcount));
			return _mm_blendv_epi8(_mm_setzero_si128(), cl, mask_l);
		}
	}

	template<typename U>
	RELEASE_INLINE auto load(int_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			const auto casted_source = (int16_t const*)source;
			return _mm_setr_epi32(
				casted_source[indices[0]],
				casted_source[indices[1]],
				casted_source[indices[2]],
				casted_source[indices[3]]
			);
		} else {
			const auto casted_source = (uint16_t const*)source;
			return _mm_setr_epi32(
				casted_source[indices[0]],
				casted_source[indices[1]],
				casted_source[indices[2]],
				casted_source[indices[3]]
			);
		}
	}

	template<int32_t i, typename U>
	RELEASE_INLINE auto load(contiguous_tags<int32_t, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			auto const vl = _mm_loadu_si32(source + e.value);
			return _mm_cvtepi8_epi32(vl);
		} else {
			auto const vl = _mm_loadu_si32(source + e.value);
			return _mm_cvtepu8_epi32(vl);
		}
	}
	template<int32_t i, typename U>
	RELEASE_INLINE auto load(unaligned_contiguous_tags<int32_t, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			auto const vl = _mm_loadu_si32(source + e.value);
			return _mm_cvtepi8_epi32(vl);
		} else {
			auto const vl = _mm_loadu_si32(source + e.value);
			return _mm_cvtepu8_epi32(vl);
		}
	}
	template<typename U>
	RELEASE_INLINE auto load(partial_contiguous_tags<int32_t> e, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			auto const vl = _mm_loadu_si32(source + e.value);
			auto const cl = _mm_cvtepi8_epi32(vl);

			auto const mask_l = _mm_loadu_si128((const __m128i *)(load_masks + 4ui32 - e.subcount));
			return _mm_blendv_epi8(_mm_setzero_si128(), cl, mask_l);
		} else {
			auto const vl = _mm_loadu_si32(source + e.value);
			auto const cl = _mm_cvtepu8_epi32(vl);

			auto const mask_l = _mm_loadu_si128((const __m128i *)(load_masks + 4ui32 - e.subcount));
			return _mm_blendv_epi8(_mm_setzero_si128(), cl, mask_l);
		}
	}
	template<typename U>
	RELEASE_INLINE auto load(int_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<std::remove_cv_t<U>, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-2) < U(0)) {
			const auto casted_source = (int8_t const*)source;
			return _mm_setr_epi32(
				casted_source[indices[0]],
				casted_source[indices[1]],
				casted_source[indices[2]],
				casted_source[indices[3]]
			);
		} else {
			const auto casted_source = (uint8_t const*)source;
			return _mm_setr_epi32(
				casted_source[indices[0]],
				casted_source[indices[1]],
				casted_source[indices[2]],
				casted_source[indices[3]]
			);
		}
	}

#pragma warning( pop ) 

	//-----

	template<typename T, int32_t i, typename U>
	RELEASE_INLINE auto load(contiguous_tags<typename ve_identity<T>::type, i> e, tagged_array_view<U, T> source) {
		return ve::load(contiguous_tags<int32_t, i>(e.value), source.data());
	}
	template<typename T, int32_t i, typename U>
	RELEASE_INLINE auto load(unaligned_contiguous_tags<typename ve_identity<T>::type, i> e, tagged_array_view<U, T> source) {
		return ve::load(unaligned_contiguous_tags<int32_t, i>(e.value), source.data());
	}
	template<typename T, typename U>
	RELEASE_INLINE auto load(partial_contiguous_tags<typename ve_identity<T>::type> e, tagged_array_view<U, T> source) {
		return ve::load(partial_contiguous_tags<int32_t>(e.value, e.subcount), source.data());
	}
	template<typename T, typename U>
	RELEASE_INLINE auto load(tagged_vector<typename ve_identity<T>::type> indices, tagged_array_view<U, T> source) {
		if constexpr(!std::is_same_v<std::remove_cv_t<U>, bitfield_type>) {
			return ve::load(indices.value, source.data() - int32_t(T::zero_is_null_t::value));
		} else if constexpr(T::zero_is_null_t::value) {
			return ve::load(int_vector(indices.value) - 1, source.data());
		} else {
			return ve::load(indices.value, source.data());
		}
	}
	//-----

	template<int32_t cache_lines, typename T, int32_t i, typename U>
	RELEASE_INLINE auto prefetch(contiguous_tags<T, i> e, U const* source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source + e.value) + 64 * cache_lines, _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	RELEASE_INLINE auto prefetch(contiguous_tags<T, i> e, tagged_array_view<U, typename ve_identity<T>::type> source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source.data() + e.value) + 64 * cache_lines, _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	RELEASE_INLINE auto prefetch(unaligned_contiguous_tags<T, i> e, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	RELEASE_INLINE auto prefetch(partial_contiguous_tags<T> e, U source) -> void {}
	template<int32_t cache_lines, typename U>
	RELEASE_INLINE auto prefetch(int_vector indices, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	RELEASE_INLINE auto prefetch(tagged_vector<T> indices, U source) -> void {}


	template<int32_t cache_lines, typename T, int32_t i, typename U>
	RELEASE_INLINE auto nt_prefetch(contiguous_tags<T, i> e, U const* source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source + e.value) + 64 * cache_lines, _MM_HINT_NTA);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	RELEASE_INLINE auto nt_prefetch(contiguous_tags<T, i> e, tagged_array_view<U, typename ve_identity<T>::type> source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source.data() + e.value) + 64 * cache_lines, _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	RELEASE_INLINE auto nt_prefetch(unaligned_contiguous_tags<T, i> e, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	RELEASE_INLINE auto nt_prefetch(partial_contiguous_tags<T> e, U source) -> void {}
	template<int32_t cache_lines, typename U>
	RELEASE_INLINE auto nt_prefetch(int_vector indices, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	RELEASE_INLINE auto nt_prefetch(tagged_vector<T> indices, U source) -> void {}

	template<int32_t stride>
	struct prefetch_stride {
		int32_t offset;
	};

	template<int32_t stride, typename T>
	RELEASE_INLINE auto prefetch(prefetch_stride<stride> e, T* source) -> void {
		static_assert(sizeof(T) <= 4);
		if constexpr((stride % (4 / sizeof(T))) == 0) {
			_mm_prefetch((char const*)(source + 16 * e.offset), _MM_HINT_T0);
		}
	}

	template<int32_t stride, typename T, typename U>
	RELEASE_INLINE auto prefetch(prefetch_stride<stride> e, tagged_array_view<T, U> source) -> void {
		prefetch(e, source.data());
	}

	template<int32_t stride, typename T>
	RELEASE_INLINE auto nt_prefetch(prefetch_stride<stride> e, T* source) -> void {
		static_assert(sizeof(T) <= 4);
		if constexpr((stride % (4 / sizeof(T))) == 0) {
			_mm_prefetch((char const*)(source + 16 * e.offset), _MM_HINT_NTA);
		}
	}

	template<int32_t stride, typename T, typename U>
	RELEASE_INLINE auto nt_prefetch(prefetch_stride<stride> e, tagged_array_view<T, U> source) -> void {
		prefetch(e, source.data());
	}

	template<int32_t i>
	RELEASE_INLINE void store(contiguous_tags<int32_t, i> e, float* dest, fp_vector values) {
		assert((intptr_t(dest + e.value) & 15) == 0);
		return _mm_store_ps(dest + e.value, values);
	}
	template<int32_t i>
	RELEASE_INLINE void store(unaligned_contiguous_tags<int32_t, i> e, float* dest, fp_vector values) {
		return _mm_storeu_ps(dest + e.value, values);
	}
	RELEASE_INLINE void store(partial_contiguous_tags<int32_t> e, float* dest, fp_vector values) {
		switch(e.subcount) {
			default:
				// fallthrough
			case 4:
				dest[e.value + 3] = values[3];
				// fallthrough
			case 3:
				dest[e.value + 2] = values[2];
				// fallthrough
			case 2:
				dest[e.value + 1] = values[1];
				// fallthrough
			case 1:
				dest[e.value + 0] = values[0];
				// fallthrough
			case 0:
				break;
		}
	}

	template<typename T, int32_t i>
	RELEASE_INLINE void store(contiguous_tags<typename ve_identity<T>::type, i> e, tagged_array_view<float, T> dest, fp_vector values) {
		ve::store(contiguous_tags<int32_t, i>(e.value), dest.data(), values);
	}
	template<typename T, int32_t i>
	RELEASE_INLINE void store(unaligned_contiguous_tags<typename ve_identity<T>::type, i> e, tagged_array_view<float, T> dest, fp_vector values) {
		ve::store(unaligned_contiguous_tags<int32_t, i>(e.value), dest.data(), values);
	}
	template<typename T>
	RELEASE_INLINE void store(partial_contiguous_tags<typename ve_identity<T>::type> e, tagged_array_view<float, T> dest, fp_vector values) {
		ve::store(partial_contiguous_tags<int32_t>(e.value, e.subcount), dest.data(), values);
	}

	RELEASE_INLINE void store(int_vector indices, float* dest, fp_vector values) {
		dest[indices[0]] = values[0];
		dest[indices[1]] = values[1];
		dest[indices[2]] = values[2];
		dest[indices[3]] = values[3];
	}

	template<typename T>
	RELEASE_INLINE void store(tagged_vector<typename ve_identity<T>::type> indices, tagged_array_view<float, T> dest, fp_vector values) {
		ve::store(indices.value, dest.data() - int32_t(T::zero_is_null_t::value), values);
	}
}