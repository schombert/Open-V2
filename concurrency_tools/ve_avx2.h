#pragma once
#include "common\\common.h"
#include <intrin.h>

namespace ve {
	constexpr int32_t vector_size = 8;
	using int_vector_internal = __m256i;
	using fp_vector_internal = __m256;

	constexpr int32_t full_mask = 0x00FF;
	constexpr int32_t empty_mask = 0;

	struct int_vector;

	template<typename tag_type>
	struct tagged_vector;

	struct fp_vector;

	struct mask_vector;

	struct vbitfield_type {
		using storage = uint8_t;

		uint8_t v;
	};

	__forceinline vbitfield_type operator&(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v & b.v) };
	}
	__forceinline vbitfield_type operator|(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v | b.v) };
	}
	__forceinline vbitfield_type operator^(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v ^ b.v) };
	}
	__forceinline vbitfield_type operator~(vbitfield_type a) {
		return vbitfield_type{ uint8_t(~a.v) };
	}
	__forceinline vbitfield_type operator!(vbitfield_type a) {
		return vbitfield_type{ uint8_t(~a.v) };
	}
	__forceinline vbitfield_type and_not(vbitfield_type a, vbitfield_type b) {
		return vbitfield_type{ uint8_t(a.v & (~b.v)) };
	}

	struct mask_vector {
		using wrapped_value = bool;

		__m256 value;

		__forceinline mask_vector() : value(_mm256_setzero_ps()) {}
		__forceinline mask_vector(bool b) : value(_mm256_castsi256_ps(_mm256_set1_epi32(-int32_t(b)))) {}
		__forceinline mask_vector(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) :
			value(_mm256_castsi256_ps(_mm256_setr_epi32(-int32_t(a), -int32_t(b), -int32_t(c), -int32_t(d), -int32_t(e), -int32_t(f), -int32_t(g), -int32_t(h)))) {}
		__forceinline mask_vector(vbitfield_type b) {
			auto repeated_mask = _mm256_set1_epi32(b.v);
			const auto mask_filter = _mm256_setr_epi32(
				0x00000001, 0x00000002, 0x00000004, 0x00000008,
				0x00000010, 0x00000020, 0x00000040, 0x00000080);
			value = _mm256_castsi256_ps(_mm256_xor_si256(_mm256_and_si256(repeated_mask, mask_filter), _mm256_set1_epi32(-1)));
		}
		__forceinline constexpr mask_vector(__m256 v) : value(v) {}
		__forceinline constexpr operator __m256() {
			return value;
		}
		__forceinline bool operator[](uint32_t i) const noexcept {
			return _mm256_castps_si256(value).m256i_i32[i] != 0;
		}
	};

	struct fp_vector {
		using wrapped_value = float;

		__m256 value;

		__forceinline fp_vector() : value(_mm256_setzero_ps()) {}
		__forceinline constexpr fp_vector(__m256 v) : value(v) {}
		__forceinline fp_vector(float v) : value(_mm256_set1_ps(v)) {}
		__forceinline fp_vector(float a, float b, float c, float d, float e, float f, float g, float h) : value(_mm256_setr_ps(a, b, c, d, e, f, g, h)) {}
		__forceinline constexpr operator __m256() {
			return value;
		}
		__forceinline float reduce() {
			// source: Peter Cordes
			__m128 vlow = _mm256_castps256_ps128(value);
			__m128 vhigh = _mm256_extractf128_ps(value, 1); // high 128
			__m128 v = _mm_add_ps(vlow, vhigh);  // add the low 128

			__m128 shuf = _mm_movehdup_ps(v); // broadcast elements 3,1 to 2,0
			__m128 sums = _mm_add_ps(v, shuf);
			shuf = _mm_movehl_ps(shuf, sums); // high half -> low half
			sums = _mm_add_ss(sums, shuf);
			return _mm_cvtss_f32(sums);
		}
		__forceinline const float operator[](uint32_t i) const noexcept {
			return value.m256_f32[i];
		}
		__forceinline void set(uint32_t i, float v) noexcept {
			value.m256_f32[i] = v;
		}
	};

	struct int_vector {
		using wrapped_value = int32_t;

		__m256i value;

		__forceinline int_vector() : value(_mm256_setzero_si256()) {}
		__forceinline constexpr int_vector(__m256i v) : value(v) {}
		__forceinline int_vector(int32_t v) : value(_mm256_set1_epi32(v)) {}
		__forceinline int_vector(uint32_t v) : value(_mm256_set1_epi32(int32_t(v))) {}
		__forceinline int_vector(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h) : value(_mm256_setr_epi32(a, b, c, d, e, f, g, h)) {}
		__forceinline int_vector(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g, uint32_t h) :
			value(_mm256_setr_epi32(int32_t(a), int32_t(b), int32_t(c), int32_t(d), int32_t(e), int32_t(f), int32_t(g), int32_t(h))) {}
		__forceinline constexpr operator __m256i() {
			return value;
		}

		__forceinline int32_t operator[](uint32_t i) const noexcept {
			return value.m256i_i32[i];
		}
		__forceinline void set(uint32_t i, int32_t v) noexcept {
			value.m256i_i32[i] = v;
		}
	};

	template<typename tag_type>
	struct tagged_vector {
		using wrapped_value = tag_type;

		__m256i value;

		__forceinline tagged_vector() : value(_mm256_setzero_si256()) {}
		__forceinline constexpr tagged_vector(__m256i v) : value(v) {}
		__forceinline tagged_vector(tag_type v) : value(_mm256_set1_epi32(int32_t(v.value))) {}
		__forceinline tagged_vector(expanded_tag<tag_type> v) : value(_mm256_set1_epi32(v.value)) {}
		__forceinline tagged_vector(expanded_tag<tag_type> a, expanded_tag<tag_type> b, expanded_tag<tag_type> c, expanded_tag<tag_type> d, expanded_tag<tag_type> e, expanded_tag<tag_type> f, expanded_tag<tag_type> g, expanded_tag<tag_type> h)
			: value(_mm256_setr_epi32(a.value, b.value, c.value, d.value, e.value, f.value, g.value, h.value)) {}
		__forceinline constexpr operator __m256i() {
			return value;
		}

		__forceinline expanded_tag<tag_type> operator[](uint32_t i) const noexcept {
			return expanded_tag<tag_type>(value.m256i_i32[i], std::true_type());
		}
		__forceinline void set(uint32_t i, expanded_tag<tag_type> v) noexcept {
			value.m256i_i32[i] = v.value;
		}
	};

	struct union_tag_vector {
		using wrapped_value = union_tag;

		__m256i value;

		__forceinline union_tag_vector() : value(_mm256_setzero_si256()) {}
		__forceinline constexpr union_tag_vector(__m256i v) : value(v) {}
		template<typename tag_type>
		__forceinline constexpr union_tag_vector(tagged_vector<tag_type> v) : value(v.value) {}
		__forceinline union_tag_vector(union_tag v) : value(_mm256_set1_epi32(v.value)) {}
		__forceinline union_tag_vector(union_tag a, union_tag b, union_tag c, union_tag d, union_tag e, union_tag f, union_tag g, union_tag h)
			: value(_mm256_setr_epi32(a.value, b.value, c.value, d.value, e.value, f.value, g.value, h.value)) {}
		__forceinline constexpr operator __m256i() {
			return value;
		}
		template<typename tag_type>
		__forceinline constexpr operator tagged_vector<tag_type>() {
			return tagged_vector<tag_type>(value);
		}

		__forceinline union_tag operator[](uint32_t i) const noexcept {
			return union_tag(value.m256i_i32[i]);
		}
		__forceinline void set(uint32_t i, union_tag v) noexcept {
			value.m256i_i32[i] = v.value;
		}
	};


	template<typename tag_type, int32_t b_index = 0>
	struct contiguous_tags {
		using wrapped_value = tag_type;
		constexpr static int32_t block_index = b_index;

		uint32_t value = 0;

		constexpr contiguous_tags() : value(0) {}
		constexpr explicit contiguous_tags(uint32_t v) : value(v) {}
		constexpr contiguous_tags(const contiguous_tags& v) noexcept = default;
		constexpr contiguous_tags(contiguous_tags&& v) noexcept = default;

		contiguous_tags& operator=(contiguous_tags&& v) noexcept = default;
		contiguous_tags& operator=(contiguous_tags const& v) noexcept = default;

		constexpr bool operator==(contiguous_tags v) const noexcept { return value == v.value; }
		constexpr bool operator!=(contiguous_tags v) const noexcept { return value != v.value; }

		__forceinline tag_type operator[](uint32_t i) const noexcept {
			return tag_type(typename tag_type::value_base_t(value + i), std::true_type());
		}
	};

	template<typename tag_type, int32_t b_index = 0>
	struct unaligned_contiguous_tags {
		using wrapped_value = tag_type;
		constexpr static int32_t block_index = b_index;

		uint32_t value = 0;

		constexpr unaligned_contiguous_tags() : value(0) {}
		constexpr explicit unaligned_contiguous_tags(uint32_t v) : value(v) {}
		constexpr unaligned_contiguous_tags(const unaligned_contiguous_tags& v) noexcept = default;
		constexpr unaligned_contiguous_tags(unaligned_contiguous_tags&& v) noexcept = default;

		unaligned_contiguous_tags& operator=(unaligned_contiguous_tags&& v) noexcept = default;
		unaligned_contiguous_tags& operator=(unaligned_contiguous_tags const& v) noexcept = default;

		constexpr bool operator==(unaligned_contiguous_tags v) const noexcept { return value == v.value; }
		constexpr bool operator!=(unaligned_contiguous_tags v) const noexcept { return value != v.value; }

		__forceinline tag_type operator[](uint32_t i) const noexcept {
			return tag_type(typename tag_type::value_base_t(value + i), std::true_type());
		}
	};

	template<typename tag_type>
	struct partial_contiguous_tags {
		using wrapped_value = tag_type;
		constexpr static int32_t block_index = 0;

		uint32_t value = 0;
		uint32_t subcount = vector_size;

		constexpr partial_contiguous_tags() : value(0), subcount(vector_size) {}
		constexpr explicit partial_contiguous_tags(uint32_t v, uint32_t s) : value(v), subcount(s) {}
		constexpr partial_contiguous_tags(const partial_contiguous_tags& v) noexcept = default;
		constexpr partial_contiguous_tags(partial_contiguous_tags&& v) noexcept = default;

		partial_contiguous_tags& operator=(partial_contiguous_tags&& v) noexcept = default;
		partial_contiguous_tags& operator=(partial_contiguous_tags const& v) noexcept = default;

		constexpr bool operator==(partial_contiguous_tags v) const noexcept { return value == v.value; }
		constexpr bool operator!=(partial_contiguous_tags v) const noexcept { return value != v.value; }

		__forceinline tag_type operator[](uint32_t i) const noexcept {
			return tag_type(typename tag_type::value_base_t(value + i), std::true_type());
		}
	};

	template<typename T>
	struct value_to_vector_type_s;

	template<>
	struct value_to_vector_type_s<int32_t> {
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
	template<typename value_base, typename individuator>
	struct value_to_vector_type_s<expanded_tag<tag_type<value_base, std::true_type, individuator>>> {
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
	struct value_to_vector_type_s<union_tag_vector> {
		using type = union_tag_vector;
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
	template<>
	struct is_vector_type_s<union_tag_vector> {
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
	__forceinline auto widen_to(FROM v) -> std::conditional_t<is_vector_type<TO>, value_to_vector_type_s<FROM>, FROM> { return v; }

	template<uint32_t i, typename T>
	__forceinline std::enable_if_t<is_vector_type<T>, typename T::wrapped_value> nth_item(T v) {
		return v[i];
	}
	template<uint32_t i, typename T>
	__forceinline std::enable_if_t<!is_vector_type<T>, typename T::wrapped_value> nth_item(T v) {
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

	__forceinline constexpr uint32_t minimum_partial() {
		return uint32_t(vector_size);
	}

	template<typename ttype, typename ... T>
	__forceinline uint32_t minimum_partial(partial_contiguous_tags<ttype> p, T... args) {
		return std::min(p.subcount, minimum_partial(args ...));
	}

	template<typename first, typename ... T>
	__forceinline uint32_t minimum_partial(first, T... args) {
		return minimum_partial(args ...);
	}

	template<typename FUNC, typename ... PARAMS>
	__forceinline auto apply(PARAMS ... params, FUNC&& f) 
		-> std::conditional_t<any_is_vector_type<PARAMS...>::value, value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>, decltype(f(params ...))> {
		if constexpr(any_is_partial<PARAMS ...>) {
			const uint32_t limit = minimum_partial(params ...);

			if constexpr(std::is_same_v<decltype(f(nth_item<0ui32>(params) ...)), void>) {
				if(limit > 0ui32) f(nth_item<0ui32>(params) ...);
				if(limit > 1ui32) f(nth_item<1ui32>(params) ...);
				if(limit > 2ui32) f(nth_item<2ui32>(params) ...);
				if(limit > 3ui32) f(nth_item<3ui32>(params) ...);
				if(limit > 4ui32) f(nth_item<4ui32>(params) ...);
				if(limit > 5ui32) f(nth_item<5ui32>(params) ...);
				if(limit > 6ui32) f(nth_item<6ui32>(params) ...);
				if(limit > 7ui32) f(nth_item<7ui32>(params) ...);
			} else {
				return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
					(limit > 0ui32) ? f(nth_item<0ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 1ui32) ? f(nth_item<1ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 2ui32) ? f(nth_item<2ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 3ui32) ? f(nth_item<3ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 4ui32) ? f(nth_item<4ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 5ui32) ? f(nth_item<5ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 6ui32) ? f(nth_item<6ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))(),
					(limit > 7ui32) ? f(nth_item<7ui32>(params) ...) : decltype(f(nth_item<0ui32>(params) ...))()
					);
			}
		} else if constexpr(any_is_vector_type<PARAMS...>::value) {
			if constexpr(std::is_same_v<decltype(f(nth_item<0ui32>(params) ...)), void>) {
				f(nth_item<0ui32>(params) ...);
				f(nth_item<1ui32>(params) ...);
				f(nth_item<2ui32>(params) ...);
				f(nth_item<3ui32>(params) ...);
				f(nth_item<4ui32>(params) ...);
				f(nth_item<5ui32>(params) ...);
				f(nth_item<6ui32>(params) ...);
				f(nth_item<7ui32>(params) ...);
			} else {
				return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
					f(nth_item<0ui32>(params) ...),
					f(nth_item<1ui32>(params) ...),
					f(nth_item<2ui32>(params) ...),
					f(nth_item<3ui32>(params) ...),
					f(nth_item<4ui32>(params) ...),
					f(nth_item<5ui32>(params) ...),
					f(nth_item<6ui32>(params) ...),
					f(nth_item<7ui32>(params) ...)
					);
			}
		} else {
			return f(params ...);
		}
	}

	template<typename FUNC, typename ... PARAMS>
	__forceinline auto apply_with_indices(PARAMS ... params, FUNC&& f)
		->  value_to_vector_type<decltype(f(0ui32, nth_item<0ui32>(params) ...))> {
		if constexpr(any_is_partial<PARAMS ...>) {
			const uint32_t limit = minimum_partial(params ...);

			if constexpr(std::is_same_v<decltype(f(0ui32, nth_item<0ui32>(params) ...)), void>) {
				if(limit > 0ui32) f(0ui32, nth_item<0ui32>(params) ...);
				if(limit > 1ui32) f(1ui32, nth_item<1ui32>(params) ...);
				if(limit > 2ui32) f(2ui32, nth_item<2ui32>(params) ...);
				if(limit > 3ui32) f(3ui32, nth_item<3ui32>(params) ...);
				if(limit > 4ui32) f(4ui32, nth_item<4ui32>(params) ...);
				if(limit > 5ui32) f(5ui32, nth_item<5ui32>(params) ...);
				if(limit > 6ui32) f(6ui32, nth_item<6ui32>(params) ...);
				if(limit > 7ui32) f(7ui32, nth_item<7ui32>(params) ...);
			} else {
				return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
					(limit > 0ui32) ? f(0ui32, nth_item<0ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 1ui32) ? f(1ui32, nth_item<1ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 2ui32) ? f(2ui32, nth_item<2ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 3ui32) ? f(3ui32, nth_item<3ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 4ui32) ? f(4ui32, nth_item<4ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 5ui32) ? f(5ui32, nth_item<5ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 6ui32) ? f(6ui32, nth_item<6ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))(),
					(limit > 7ui32) ? f(7ui32, nth_item<7ui32>(params) ...) : decltype(f(0ui32, nth_item<0ui32>(params) ...))()
					);
			}
		} else if constexpr(std::is_same_v<decltype(f(0ui32, nth_item<0ui32>(params) ...)), void>) {
			f(0ui32, nth_item<0ui32>(params) ...);
			f(1ui32, nth_item<1ui32>(params) ...);
			f(2ui32, nth_item<2ui32>(params) ...);
			f(3ui32, nth_item<3ui32>(params) ...);
			f(4ui32, nth_item<4ui32>(params) ...);
			f(5ui32, nth_item<5ui32>(params) ...);
			f(6ui32, nth_item<6ui32>(params) ...);
			f(7ui32, nth_item<7ui32>(params) ...);
		} else {
			return value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>(
				f(0ui32, nth_item<0ui32>(params) ...),
				f(1ui32, nth_item<1ui32>(params) ...),
				f(2ui32, nth_item<2ui32>(params) ...),
				f(3ui32, nth_item<3ui32>(params) ...),
				f(4ui32, nth_item<4ui32>(params) ...),
				f(5ui32, nth_item<5ui32>(params) ...),
				f(6ui32, nth_item<6ui32>(params) ...),
				f(7ui32, nth_item<7ui32>(params) ...)
				);
		}
	}

	__forceinline fp_vector operator+(fp_vector a, fp_vector b) {
		return _mm256_add_ps(a, b);
	}
	__forceinline fp_vector operator-(fp_vector a, fp_vector b) {
		return _mm256_sub_ps(a, b);
	}
	__forceinline fp_vector operator*(fp_vector a, fp_vector b) {
		return _mm256_mul_ps(a, b);
	}
	__forceinline fp_vector operator/(fp_vector a, fp_vector b) {
		return _mm256_div_ps(a, b);
	}

	__forceinline mask_vector operator&(mask_vector a, mask_vector b) {
		return _mm256_and_ps(a, b);
	}
	__forceinline mask_vector operator|(mask_vector a, mask_vector b) {
		return _mm256_or_ps(a, b);
	}
	__forceinline mask_vector operator^(mask_vector a, mask_vector b) {
		return _mm256_xor_ps(a, b);
	}
	__forceinline mask_vector operator~(mask_vector a) {
		return _mm256_xor_ps(a, mask_vector(true));
	}
	__forceinline mask_vector operator!(mask_vector a) {
		return _mm256_xor_ps(a, mask_vector(true));
	}
	__forceinline mask_vector and_not(mask_vector a, mask_vector b) {
		return _mm256_andnot_ps(b, a);
	}

	__forceinline fp_vector inverse(fp_vector a) {
		return _mm256_rcp_ps(a);
	}
	__forceinline fp_vector sqrt(fp_vector a) {
		return _mm256_sqrt_ps(a);
	}
	__forceinline fp_vector inverse_sqrt(fp_vector a) {
		return _mm256_rsqrt_ps(a);
	}

	__forceinline fp_vector multiply_and_add(fp_vector a, fp_vector b, fp_vector c) {
		return _mm256_fmadd_ps(a, b, c);
	}
	__forceinline fp_vector multiply_and_subtract(fp_vector a, fp_vector b, fp_vector c) {
		return _mm256_fmsub_ps(a, b, c);
	}
	__forceinline fp_vector negate_multiply_and_add(fp_vector a, fp_vector b, fp_vector c) {
		return _mm256_fnmadd_ps(a, b, c);
	}
	__forceinline fp_vector negate_multiply_and_subtract(fp_vector a, fp_vector b, fp_vector c) {
		return _mm256_fnmsub_ps(a, b, c);
	}

	__forceinline fp_vector min(fp_vector a, fp_vector b) {
		return _mm256_min_ps(a, b);
	}
	__forceinline fp_vector max(fp_vector a, fp_vector b) {
		return _mm256_max_ps(a, b);
	}
	__forceinline fp_vector floor(fp_vector a) {
		return _mm256_floor_ps(a);
	}
	__forceinline fp_vector ceil(fp_vector a) {
		return _mm256_ceil_ps(a);
	}

	__forceinline mask_vector operator<(fp_vector a, fp_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_LT_OQ);
	}
	__forceinline mask_vector operator>(fp_vector a, fp_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_GT_OQ);
	}
	__forceinline mask_vector operator<=(fp_vector a, fp_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_LE_OQ);
	}
	__forceinline mask_vector operator>=(fp_vector a, fp_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_GE_OQ);
	}
	__forceinline mask_vector operator==(fp_vector a, fp_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_EQ_OQ);
	}
	__forceinline mask_vector operator!=(fp_vector a, fp_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_NEQ_OQ);
	}

	__forceinline mask_vector operator<(int_vector a, int_vector b) {
		return _mm256_castsi256_ps(_mm256_cmpgt_epi32(b, a));
	}
	__forceinline mask_vector operator>(int_vector a, int_vector b) {
		return _mm256_castsi256_ps(_mm256_cmpgt_epi32(a, b));
	}
	__forceinline mask_vector operator<=(int_vector a, int_vector b) {
		return _mm256_castsi256_ps(_mm256_or_si256(_mm256_cmpgt_epi32(b, a), _mm256_cmpeq_epi32(a, b)));
	}
	__forceinline mask_vector operator>=(int_vector a, int_vector b) {
		return _mm256_castsi256_ps(_mm256_or_si256(_mm256_cmpgt_epi32(a, b), _mm256_cmpeq_epi32(a, b)));
	}
	__forceinline mask_vector operator==(int_vector a, int_vector b) {
		return _mm256_castsi256_ps(_mm256_cmpeq_epi32(a, b));
	}
	__forceinline mask_vector operator!=(int_vector a, int_vector b) {
		return _mm256_castsi256_ps(_mm256_or_si256(_mm256_cmpgt_epi32(a, b), _mm256_cmpgt_epi32(b, a)));
	}
	__forceinline mask_vector operator==(mask_vector a, mask_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_EQ_OQ);
	}
	__forceinline mask_vector operator!=(mask_vector a, mask_vector b) {
		return _mm256_cmp_ps(a, b, _CMP_NEQ_OQ);
	}

	__forceinline mask_vector operator==(union_tag_vector a, union_tag_vector b) {
		return _mm256_castsi256_ps(_mm256_cmpeq_epi32(a, b));
	}
	__forceinline mask_vector operator!=(union_tag_vector a, union_tag_vector b) {
		return _mm256_castsi256_ps(_mm256_or_si256(_mm256_cmpgt_epi32(a, b), _mm256_cmpgt_epi32(b, a)));
	}
	template<typename T>
	__forceinline mask_vector operator==(tagged_vector<T> a, tagged_vector<T> b) {
		return _mm256_castsi256_ps(_mm256_cmpeq_epi32(a, b));
	}
	template<typename T>
	__forceinline mask_vector operator!=(tagged_vector<T> a, tagged_vector<T> b) {
		return _mm256_castsi256_ps(_mm256_or_si256(_mm256_cmpgt_epi32(a, b), _mm256_cmpgt_epi32(b, a)));
	}

	__forceinline mask_vector bit_test(int_vector val, int32_t bits) {
		return _mm256_castsi256_ps(_mm256_cmpeq_epi32(_mm256_and_si256(val, _mm256_set1_epi32(bits)), _mm256_set1_epi32(bits)));
	}

	__forceinline fp_vector select(vbitfield_type mask, fp_vector a, fp_vector b) {
		auto repeated_mask = _mm256_set1_epi32(mask.v);
		const auto mask_filter = _mm256_setr_epi32(
			0x00000001, 0x00000002, 0x00000004, 0x00000008,
			0x00000010, 0x00000020, 0x00000040, 0x00000080);
		auto fp_mask = mask_vector(_mm256_castsi256_ps(_mm256_and_si256(repeated_mask, mask_filter))) != mask_vector();
		return _mm256_blendv_ps(b, a, fp_mask);
	}
	__forceinline mask_vector widen_mask(vbitfield_type mask) {
		auto repeated_mask = _mm256_set1_epi32(mask.v);
		const auto mask_filter = _mm256_setr_epi32(
			0x00000001, 0x00000002, 0x00000004, 0x00000008,
			0x00000010, 0x00000020, 0x00000040, 0x00000080);
		return mask_vector(_mm256_castsi256_ps(_mm256_and_si256(repeated_mask, mask_filter))) != mask_vector();
	}
	__forceinline fp_vector select(mask_vector mask, fp_vector a, fp_vector b) {
		return _mm256_blendv_ps(b, a, mask);
	}
	__forceinline mask_vector is_non_zero(int_vector i) {
		return i != int_vector();
	}
	__forceinline mask_vector is_zero(int_vector i) {
		return i == int_vector();
	}

	__forceinline mask_vector is_valid_index(union_tag_vector i) {
		return i != union_tag_vector();
	}
	__forceinline mask_vector is_invalid(union_tag_vector i) {
		return i == union_tag_vector();
	}
	template<typename T>
	__forceinline mask_vector is_valid_index(tagged_vector<T> i) {
		return i != tagged_vector<T>();
	}
	template<typename T>
	__forceinline mask_vector is_invalid(tagged_vector<T> i) {
		return i == tagged_vector<T>();
	}

	__forceinline int32_t compress_mask(mask_vector mask) {
		return _mm256_movemask_ps(mask);
	}


	inline constexpr uint32_t load_masks[16] = {
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000
	};

	template<typename F>
	class alignas(__m256i) true_accumulator : public F {
	private:
		__m256i value;
		uint32_t index = 0;
		int32_t accumulated_mask = 0;
	public:
		bool result = false;

		true_accumulator(F&& f) : value(_mm256_setzero_si256()), F(std::move(f)) {}

		void add_value(int32_t v) {
			if(!result) {
				accumulated_mask |= (int32_t(v != 0) << index);
				value.m256i_i32[index++] = v;

				if(index == 8) {
					result = (ve::compress_mask(F::operator()(value)) & accumulated_mask) != 0;
					value = _mm256_setzero_si256();
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
	class alignas(__m256i) false_accumulator : public F {
	private:
		__m256i value;
		uint32_t index = 0;
		int32_t accumulated_mask = 0;
	public:
		bool result = true;

		false_accumulator(F&& f) : value(_mm256_setzero_si256()), F(std::move(f)) {}

		void add_value(int32_t v) {
			if(result) {
				accumulated_mask |= (int32_t(v != 0) << index);
				value.m256i_i32[index++] = v;

				if(index == 8) {
					result = (ve::compress_mask(F::operator()(value)) & accumulated_mask) == accumulated_mask;
					value = _mm256_setzero_si256();
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

	template<typename F>
	auto make_true_accumulator(F const& f) -> true_accumulator<F> {
		return true_accumulator<F>(f);
	}

	template<typename F>
	auto make_false_accumulator(F const& f) -> false_accumulator<F> {
		return false_accumulator<F>(f);
	}

	template<typename T, int32_t i, typename U>
	__forceinline U partial_mask(contiguous_tags<T, i> e, U value) {
		return value;
	}
	template<typename T, int32_t i, typename U>
	__forceinline U partial_mask(unaligned_contiguous_tags<T, i> e, U value) {
		return value;
	}
	template<typename T, typename U>
	__forceinline U partial_mask(partial_contiguous_tags<T> e, U value) {
		mask_vector mask = _mm256_loadu_ps((float const*)(load_masks + 8ui32 - e.subcount));
		return select(mask, value, U());
	}
	template<typename U>
	__forceinline U partial_mask(int_vector indices, U value) {
		return value;
	}
	template<typename T, typename U>
	__forceinline U partial_mask(tagged_vector<T> indices, U value) {
		return value;
	}
	template<typename U>
	__forceinline U partial_mask(union_tag_vector indices, U value) {
		return value;
	}


	template<typename T, int32_t i>
	__forceinline vbitfield_type load(contiguous_tags<T, i> e, bitfield_type const* source) {
		return vbitfield_type{ source[e.value / 8ui32].v };
	}
	template<typename T, int32_t i>
	__forceinline vbitfield_type load(unaligned_contiguous_tags<T, i> e, bitfield_type const* source) {
		return vbitfield_type{ source[e.value / 8ui32].v };
	}
	template<typename T>
	__forceinline vbitfield_type load(partial_contiguous_tags<T> e, bitfield_type const* source) {
		return vbitfield_type{ source[e.value / 8ui32].v };
	}
	__forceinline mask_vector load(int_vector indices, bitfield_type const* source) {
		const auto byte_indices = _mm256_srli_epi32(indices, 3);
		const auto bit_indices = _mm256_and_si256(indices, _mm256_set1_epi32(0x00000007));
		auto gathered = _mm256_i32gather_epi32((int32_t const*)(source), byte_indices, 1);
		auto shifted = _mm256_and_si256(_mm256_srlv_epi32(gathered, bit_indices), _mm256_set1_epi32(0x00000001));
		return _mm256_castsi256_ps(_mm256_sub_epi32(_mm256_setzero_si256(), shifted));
	}
	template<typename T>
	__forceinline mask_vector load(tagged_vector<T> indices, bitfield_type const* source) {
		const auto byte_indices = _mm256_srli_epi32(indices, 3);
		const auto bit_indices = _mm256_and_si256(indices, _mm256_set1_epi32(0x00000007));
		auto gathered = _mm256_i32gather_epi32((int32_t const*)(source), byte_indices, 1);
		auto shifted = _mm256_and_si256(_mm256_srlv_epi32(gathered, bit_indices), _mm256_set1_epi32(0x00000001));
		return _mm256_castsi256_ps(_mm256_sub_epi32(_mm256_setzero_si256(), shifted));
	}
	__forceinline mask_vector load(union_tag_vector indices, bitfield_type const* source) {
		const auto byte_indices = _mm256_srli_epi32(indices, 3);
		const auto bit_indices = _mm256_and_si256(indices, _mm256_set1_epi32(0x00000007));
		auto gathered = _mm256_i32gather_epi32((int32_t const*)(source), byte_indices, 1);
		auto shifted = _mm256_and_si256(_mm256_srlv_epi32(gathered, bit_indices), _mm256_set1_epi32(0x00000001));
		return _mm256_castsi256_ps(_mm256_sub_epi32(_mm256_setzero_si256(), shifted));
	}


	template<typename T, int32_t i, typename U>
	__forceinline auto load(contiguous_tags<T, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		if constexpr(std::is_same_v<U, float>)
			return _mm256_load_ps(source + e.value);
		else
			return _mm256_load_si256((const __m256i *)(source + e.value));
	}
	template<typename T, int32_t i, typename U>
	__forceinline auto load(unaligned_contiguous_tags<T, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		if constexpr(std::is_same_v<U, float>)
			return _mm256_loadu_ps(source + e.value);
		else
			return _mm256_loadu_si256((const __m256i *)(source + e.value));
	}
	template<typename T, typename U>
	__forceinline auto load(partial_contiguous_tags<T> e, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		auto mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - e.subcount));
		if constexpr(std::is_same_v<U, float>) {
			return _mm256_maskload_ps(source + e.value, mask);
		} else {
			return _mm256_maskload_epi32((int32_t const*)(source + e.value), mask);
		}
	}
	template<typename U>
	__forceinline auto load(int_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		if constexpr(std::is_same_v<U, float>)
			return _mm256_i32gather_ps(source, indices, 4);
		else
			return _mm256_i32gather_epi32(source, indices, 4);
	}
	template<typename T, typename U>
	__forceinline auto load(tagged_vector<T> indices, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		if constexpr(std::is_same_v<U, float>)
			return _mm256_i32gather_ps(source, indices, 4);
		else
			return _mm256_i32gather_epi32((int32_t const*)source, indices, 4);
	}
	template<typename U>
	__forceinline auto load(union_tag_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 4, value_to_vector_type<U>> {
		if constexpr(std::is_same_v<U, float>)
			return _mm256_i32gather_ps(source, indices, 4);
		else
			return _mm256_i32gather_epi32((int32_t const*)source, indices, 4);
	}


	template<typename T, int32_t i, typename U>
	__forceinline auto load(contiguous_tags<T, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_load_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepi16_epi32(_mm256_castsi256_si128(v));
		} else {
			auto v = _mm256_load_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepu16_epi32(_mm256_castsi256_si128(v));
		}
	}
	template<typename T, int32_t i, typename U>
	__forceinline auto load(unaligned_contiguous_tags<T, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepi16_epi32(_mm256_castsi256_si128(v));
		} else {
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepu16_epi32(_mm256_castsi256_si128(v));
		}
	}
	template<typename T, typename U>
	__forceinline auto load(partial_contiguous_tags<T> e, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - e.subcount));
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_blendv_ps(_mm256_setzero_si256(), _mm256_cvtepi16_epi32(_mm256_castsi256_si128(v)), mask);
		} else {
			auto mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - e.subcount));
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_blendv_ps(_mm256_setzero_si256(), _mm256_cvtepu16_epi32(_mm256_castsi256_si128(v)), mask);
		}
	}
	template<typename U>
	__forceinline auto load(int_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, _mm256_sub_epi32(indices, _mm256_set1_epi32(1)), 2);
			return _mm256_srai_epi32(v, 16);
		} else {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, indices, 2);
			return _mm256_and_si256(v, _mm256_set1_epi32(0xFFFF));
		}
	}
	template<typename T, typename U>
	__forceinline auto load(tagged_vector<T> indices, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, _mm256_sub_epi32(indices, _mm256_set1_epi32(1)), 2);
			return _mm256_srai_epi32(v, 16);
		} else {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, indices, 2);
			return _mm256_and_si256(v, _mm256_set1_epi32(0xFFFF));
		}
	}
	template<typename U>
	__forceinline auto load(union_tag_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 2, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, _mm256_sub_epi32(indices, _mm256_set1_epi32(1)), 2);
			return _mm256_srai_epi32(v, 16);
		} else {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, indices, 2);
			return _mm256_and_si256(v, _mm256_set1_epi32(0xFFFF));
		}
	}



	template<typename T, int32_t i, typename U>
	__forceinline auto load(contiguous_tags<T, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_load_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepi8_epi32(_mm256_castsi256_si128(v));
		} else {
			auto v = _mm256_load_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepu8_epi32(_mm256_castsi256_si128(v));
		}
	}
	template<typename T, int32_t i, typename U>
	__forceinline auto load(unaligned_contiguous_tags<T, i> e, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepi8_epi32(_mm256_castsi256_si128(v));
		} else {
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_cvtepu8_epi32(_mm256_castsi256_si128(v));
		}
	}
	template<typename T, typename U>
	__forceinline auto load(partial_contiguous_tags<T> e, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - e.subcount));
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_blendv_ps(_mm256_setzero_si256(), _mm256_cvtepi8_epi32(_mm256_castsi256_si128(v)), mask);
		} else {
			auto mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - e.subcount));
			auto v = _mm256_loadu_si256((const __m256i *)(source + e.value));
			return _mm256_blendv_ps(_mm256_setzero_si256(), _mm256_cvtepu8_epi32(_mm256_castsi256_si128(v)), mask);
		}
	}
	template<typename U>
	__forceinline auto load(int_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, _mm256_sub_epi32(indices, _mm256_set1_epi32(3)), 1);
			return _mm256_srai_epi32(v, 24);
		} else {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, indices, 1);
			return _mm256_and_si256(v, _mm256_set1_epi32(0xFF));
		}
	}
	template<typename T, typename U>
	__forceinline auto load(tagged_vector<T> indices, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, _mm256_sub_epi32(indices, _mm256_set1_epi32(3)), 1);
			return _mm256_srai_epi32(v, 24);
		} else {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, indices, 1);
			return _mm256_and_si256(v, _mm256_set1_epi32(0xFF));
		}
	}
	template<typename U>
	__forceinline auto load(union_tag_vector indices, U const* source) -> std::enable_if_t<sizeof(U) == 1 && !std::is_same_v<U, bitfield_type>, value_to_vector_type<U>> {
		if constexpr(U(-1) < U(0)) {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, _mm256_sub_epi32(indices, _mm256_set1_epi32(3)), 1);
			return _mm256_srai_epi32(v, 24);
		} else {
			auto v = _mm256_i32gather_epi32((int32_t const*)source, indices, 1);
			return _mm256_and_si256(v, _mm256_set1_epi32(0xFF));
		}
	}


	//-----
	template<typename T>
	struct ve_identity {
		using type = T;
	};
	template<typename T, int32_t i, typename U>
	__forceinline auto load(contiguous_tags<typename ve_identity<decay_tag<T>>::type, i> e, tagged_array_view<U, T, true> source)
		-> std::enable_if_t<!std::is_same_v<std::remove_cv_t<U>, bitfield_type>, value_to_vector_type<std::remove_cv_t<U>>> {
		return ve::load(e, source.data());
	}
	template<typename T, int32_t i, typename U>
	__forceinline auto load(unaligned_contiguous_tags<typename ve_identity<decay_tag<T>>::type, i> e, tagged_array_view<U, T, true> source)
		-> std::enable_if_t<!std::is_same_v<std::remove_cv_t<U>, bitfield_type>, value_to_vector_type<std::remove_cv_t<U>>> {
		return ve::load(e, source.data());
	}
	template<typename T, typename U>
	__forceinline auto load(partial_contiguous_tags<typename ve_identity<decay_tag<T>>::type> e, tagged_array_view<U, T, true> source)
		-> std::enable_if_t<!std::is_same_v<std::remove_cv_t<U>, bitfield_type>, value_to_vector_type<std::remove_cv_t<U>>> {
		return ve::load(e, source.data());
	}
	template<typename T, typename U>
	__forceinline auto load(tagged_vector<typename ve_identity<decay_tag<T>>::type> indices, tagged_array_view<U, T, true> source)
		-> std::enable_if_t<!std::is_same_v<std::remove_cv_t<U>, bitfield_type>, value_to_vector_type<std::remove_cv_t<U>>> {
		return ve::load(indices, source.data());
	}
	//-----

	template<int32_t cache_lines, typename T, int32_t i, typename U>
	__forceinline auto prefetch(contiguous_tags<T, i> e, U const* source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source + e.value) + 64 * cache_lines, _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	__forceinline auto prefetch(contiguous_tags<T, i> e, tagged_array_view<U, typename ve_identity<T>::type, true> source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source + e.value) + 64 * cache_lines, _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	__forceinline auto prefetch(unaligned_contiguous_tags<T, i> e, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	__forceinline auto prefetch(partial_contiguous_tags<T> e, U source) -> void {}
	template<int32_t cache_lines, typename U>
	__forceinline auto prefetch(int_vector indices, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	__forceinline auto prefetch(tagged_vector<T> indices, U source) -> void {}
	template<int32_t cache_lines, typename U>
	__forceinline auto prefetch(union_tag_vector indices, U source) -> void {}

	template<int32_t cache_lines, typename T, int32_t i, typename U>
	__forceinline auto nt_prefetch(contiguous_tags<T, i> e, U const* source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source + e.value) + 64 * cache_lines, _MM_HINT_NTA);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	__forceinline auto nt_prefetch(contiguous_tags<T, i> e, tagged_array_view<U, typename ve_identity<T>::type, true> source) -> void {
		if constexpr(i % (8 / sizeof(U)) == 0) {
			_mm_prefetch((char const*)(source + e.value) + 64 * cache_lines, _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines, typename T, int32_t i, typename U>
	__forceinline auto nt_prefetch(unaligned_contiguous_tags<T, i> e, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	__forceinline auto nt_prefetch(partial_contiguous_tags<T> e, U source) -> void {}
	template<int32_t cache_lines, typename U>
	__forceinline auto nt_prefetch(int_vector indices, U source) -> void {}
	template<int32_t cache_lines, typename T, typename U>
	__forceinline auto nt_prefetch(tagged_vector<T> indices, U source) -> void {}
	template<int32_t cache_lines, typename U>
	__forceinline auto nt_prefetch(union_tag_vector indices, U source) -> void {}


	template<typename T, int32_t i>
	__forceinline void store(contiguous_tags<T, i> e, float* dest, fp_vector values) {
		return _mm256_store_ps(dest + e.value, values);
	}
	template<typename T, int32_t i>
	__forceinline void store(unaligned_contiguous_tags<T, i> e, float* dest, fp_vector values) {
		return _mm256_storeu_ps(dest + e.value, values);
	}
	template<typename T>
	__forceinline void store(partial_contiguous_tags<T> e, float* dest, fp_vector values) {
		int_vector_internal mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - e.subcount));
		_mm256_maskstore_ps(dest + e.value, mask, values);
	}
	__forceinline void store(int_vector indices, float* dest, fp_vector values) {
		dest[indices[0]] = values[0];
		dest[indices[1]] = values[1];
		dest[indices[2]] = values[2];
		dest[indices[3]] = values[3];
		dest[indices[4]] = values[4];
		dest[indices[5]] = values[5];
		dest[indices[6]] = values[6];
		dest[indices[7]] = values[7];
	}
	template<typename T>
	__forceinline void store(tagged_vector<T> indices, float* dest, fp_vector values) {
		dest[indices[0].value] = values[0];
		dest[indices[1].value] = values[1];
		dest[indices[2].value] = values[2];
		dest[indices[3].value] = values[3];
		dest[indices[4].value] = values[4];
		dest[indices[5].value] = values[5];
		dest[indices[6].value] = values[6];
		dest[indices[7].value] = values[7];
	}
	__forceinline void store(union_tag_vector indices, float* dest, fp_vector values) {
		dest[indices[0].value] = values[0];
		dest[indices[1].value] = values[1];
		dest[indices[2].value] = values[2];
		dest[indices[3].value] = values[3];
		dest[indices[4].value] = values[4];
		dest[indices[5].value] = values[5];
		dest[indices[6].value] = values[6];
		dest[indices[7].value] = values[7];
	}
}
