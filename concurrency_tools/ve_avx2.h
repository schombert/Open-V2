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

	struct mask_vector {
		using wrapped_value = bool;

		__m256 value;

		__forceinline mask_vector() : value(_mm256_setzero_ps()) {}
		__forceinline mask_vector(bool b) : value(_mm256_castsi256_ps(_mm256_set1_epi32(-int32_t(b)))) {}
		__forceinline mask_vector(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h) :
			value(_mm256_castsi256_ps(_mm256_setr_epi32(-int32_t(a), -int32_t(b), -int32_t(c), -int32_t(d), -int32_t(e), -int32_t(f), -int32_t(g), -int32_t(h)))) {}
		__forceinline constexpr mask_vector(__m256 v) : value(v) {}
		__forceinline constexpr operator __m256() {
			return value;
		}
		__forceinline int32_t operator[](uint32_t i) const noexcept {
			return _mm256_castps_si256(value).m256i_i32[i];
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
		__forceinline float operator[](uint32_t i) const noexcept {
			return value.m256_f32[i];
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

		__forceinline bool operator[](uint32_t i) const noexcept {
			return value.m256i_i32[i] != 0;
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

		__forceinline union_tag_vector operator[](uint32_t i) const noexcept {
			return union_tag(value.m256i_i32[i]);
		}
	};


	template<typename tag_type>
	struct contiguous_tags {
		using wrapped_value = tag_type;

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
	template<typename T>
	struct value_to_vector_type_s<contiguous_tags<T>> {
		using type = contiguous_tags<T>;
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
	template<typename T>
	struct is_vector_type_s<contiguous_tags<T>> {
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

	template<typename FUNC, typename ... PARAMS>
	__forceinline auto apply(PARAMS ... params, FUNC&& f) 
		-> std::conditional_t<any_is_vector_type<PARAMS...>::value, value_to_vector_type<decltype(f(nth_item<0ui32>(params) ...))>, decltype(f(params ...))> {
		if constexpr(any_is_vector_type<PARAMS...>::value) {
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
		if constexpr(std::is_same_v<decltype(f(0ui32, nth_item<0ui32>(params) ...)), void>) {
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

	__forceinline fp_vector select(int32_t mask, fp_vector a, fp_vector b) {
		auto repeated_mask = _mm256_set1_epi32(mask);
		const auto mask_filter = _mm256_setr_epi32(
			0x00000001, 0x00000002, 0x00000004, 0x00000008,
			0x00000010, 0x00000020, 0x00000040, 0x00000080);
		auto fp_mask = mask_vector(_mm256_castsi256_ps(_mm256_and_si256(repeated_mask, mask_filter))) != mask_vector();
		return _mm256_blendv_ps(b, a, fp_mask);
	}
	__forceinline mask_vector widen_mask(int32_t mask) {
		auto repeated_mask = _mm256_set1_epi32(mask);
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

	template<int32_t blk_index = 0>
	class full_vector_operation {
	protected:
		uint32_t const offset;
	public:
		full_vector_operation(uint32_t o) : offset(o) {}

		constexpr static int32_t block_index = blk_index;
		constexpr static bool full_operation = true;

		__forceinline static fp_vector zero() {
			return _mm256_setzero_ps();
		}
		__forceinline static int_vector int_zero() {
			return _mm256_setzero_si256();
		}
		__forceinline static fp_vector constant(float v) {
			return _mm256_set1_ps(v);
		}
		__forceinline static int_vector constant(int32_t v) {
			return _mm256_set1_epi32(v);
		}

		__forceinline fp_vector load(float const* source) {
			return _mm256_load_ps(source + offset);
		}
		__forceinline int32_t load(int8_t const* source) {
			return source[offset / 8ui32];
		}
		__forceinline int_vector load(int32_t const* source) {
			return _mm256_load_si256((__m256i const*)(source + offset));
		}
		__forceinline union_tag_vector load(union_tag const* source) {
			return _mm256_load_si256((__m256i const*)(source + offset));
		}
		template<typename tag_type>
		__forceinline tagged_vector<tag_type> load(expanded_tag<tag_type> const* source) {
			return _mm256_load_si256((__m256i const*)(source + offset));
		}
		template<typename tag_type>
		__forceinline tagged_vector<tag_type> load(tag_type const* source) {
			static_assert(sizeof(tag_type) == 4);
			return _mm256_load_si256((__m256i const*)(source + offset));
		}
		__forceinline fp_vector unaligned_load(float const* source) {
			return _mm256_loadu_ps(source + offset);
		}
		__forceinline int_vector unaligned_load(int32_t const* source) {
			return _mm256_loadu_si256((__m256i const*)(source + offset));
		}

		__forceinline static fp_vector gather_load(float const* source, __m256i indices) {
			return _mm256_i32gather_ps(source, indices, 4);
		}
		__forceinline static int_vector gather_load(int32_t const* source, __m256i indices) {
			return _mm256_i32gather_epi32(source, indices, 4);
		}
		__forceinline static union_tag_vector gather_load(union_tag const* source, __m256i indices) {
			return _mm256_i32gather_epi32((int32_t const*)source, indices, 4);
		}
		template<typename tag_type>
		__forceinline static tagged_vector<tag_type> gather_load(expanded_tag<tag_type> const* source, __m256i indices) {
			return _mm256_i32gather_epi32((int32_t const*)source, indices, 4);
		}
		template<typename tag_type>
		__forceinline static tagged_vector<tag_type> gather_load(tag_type const* source, __m256i indices) {
			static_assert(sizeof(tag_type) == 4);
			return _mm256_i32gather_epi32((int32_t const*)source, indices, 4);
		}
		
		__forceinline static mask_vector gather_load(int8_t const* source, __m256i indices) {
			const auto byte_indices = _mm256_srl_epi32(indices, 3);
			const auto bit_indices = _mm256_and_si256(indices, _mm256_set1_epi32(0x00000007));
			auto gathered = _mm256_i32gather_epi32(source, byte_indices, 1);
			auto shifted = _mm256_and_si256(_mm256_srlv_epi32(gathered, bit_indices), _mm256_set1_epi32(0x00000001));
			return _mm256_castsi256_ps(_mm256_sub_epi32(_mm256_setzero_si256(), shifted));
		}

		__forceinline static fp_vector gather_masked_load(float const* source, __m256i indices, mask_vector mask, fp_vector def = _mm256_setzero_ps()) {
			return _mm256_mask_i32gather_ps(def, source, indices, mask, 4);
		}
		__forceinline static int_vector gather_masked_load(int32_t const* source, __m256i indices, mask_vector mask, int_vector def = _mm256_setzero_si256()) {
			return _mm256_mask_i32gather_epi32(def, source, indices, _mm256_castps_si256(mask), 4);
		}

		__forceinline void store(float* dest, fp_vector value) {
			_mm256_store_ps(dest + offset, value);
		}
		__forceinline void unaligned_store(float* dest, fp_vector value) {
			_mm256_storeu_ps(dest + offset, value);
		}

		__forceinline void scatter_store(float* dest, fp_vector to_store, __m256i indices) {
			dest[indices.m256i_i32[0]] = to_store.value.m256_f32[0];
			dest[indices.m256i_i32[1]] = to_store.value.m256_f32[1];
			dest[indices.m256i_i32[2]] = to_store.value.m256_f32[2];
			dest[indices.m256i_i32[3]] = to_store.value.m256_f32[3];
			dest[indices.m256i_i32[4]] = to_store.value.m256_f32[4];
			dest[indices.m256i_i32[5]] = to_store.value.m256_f32[5];
			dest[indices.m256i_i32[6]] = to_store.value.m256_f32[6];
			dest[indices.m256i_i32[7]] = to_store.value.m256_f32[7];
		}


		template<int32_t cache_lines>
		__forceinline void prefetch(int32_t const* source) {
			if constexpr(block_index % 2 == 0) {
				_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_T0);
			}
		}
		template<int32_t cache_lines>
		__forceinline void prefetch(float const* source) {
			if constexpr(block_index % 2 == 0) {
				_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_T0);
			}
		}
		template<int32_t cache_lines>
		__forceinline void nt_prefetch(int32_t const* source) {
			if constexpr(block_index % 2 == 0) {
				_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_NTA);
			}
		}
		template<int32_t cache_lines>
		__forceinline void nt_prefetch(float const* source) {
			if constexpr(block_index % 2 == 0) {
				_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_NTA);
			}
		}

		__forceinline fp_vector stream_load(float const* source) {
			return _mm256_castsi256_ps(_mm256_stream_load_si256((__m256i const*)(source + offset)));
		}
		__forceinline int_vector stream_load(int32_t const* source) {
			return _mm256_stream_load_si256((__m256i const*)(source + offset));
		}
		__forceinline void stream_store(float* dest, fp_vector value) {
			_mm256_stream_ps(dest + offset, value);
		}

		__forceinline int_vector partial_mask() {
			return _mm256_loadu_si256((__m256i const*)load_masks);
		}

		template<typename F>
		__forceinline fp_vector apply(F const& f, fp_vector arg) {
			return _mm256_setr_ps(
				f(arg.value.m256_f32[0], offset),
				f(arg.value.m256_f32[1], offset + 1ui32),
				f(arg.value.m256_f32[2], offset + 2ui32),
				f(arg.value.m256_f32[3], offset + 3ui32),
				f(arg.value.m256_f32[4], offset + 4ui32),
				f(arg.value.m256_f32[5], offset + 5ui32),
				f(arg.value.m256_f32[6], offset + 6ui32),
				f(arg.value.m256_f32[7], offset + 7ui32));
		}

		template<typename F>
		__forceinline fp_vector apply_for_mask(F const& f, int_vector arg) {
			return _mm256_castsi256_ps(_mm256_setr_epi32(
				f(arg.value.m256i_i32[0], offset),
				f(arg.value.m256i_i32[1], offset + 1ui32),
				f(arg.value.m256i_i32[2], offset + 2ui32),
				f(arg.value.m256i_i32[3], offset + 3ui32),
				f(arg.value.m256i_i32[4], offset + 4ui32),
				f(arg.value.m256i_i32[5], offset + 5ui32),
				f(arg.value.m256i_i32[6], offset + 6ui32),
				f(arg.value.m256i_i32[7], offset + 7ui32))
			);
		}

		template<typename F>
		__forceinline auto apply(F const& f, int_vector arg) -> std::conditional_t<std::is_same_v<decltype(f(0, 0ui32)), float>, fp_vector, int_vector> {
			if constexpr(std::is_same_v<decltype(f(0, 0ui32)), float>) {
				return _mm256_setr_ps(
					f(arg.value.m256i_i32[0], offset),
					f(arg.value.m256i_i32[1], offset + 1ui32),
					f(arg.value.m256i_i32[2], offset + 2ui32),
					f(arg.value.m256i_i32[3], offset + 3ui32),
					f(arg.value.m256i_i32[4], offset + 4ui32),
					f(arg.value.m256i_i32[5], offset + 5ui32),
					f(arg.value.m256i_i32[6], offset + 6ui32),
					f(arg.value.m256i_i32[7], offset + 7ui32));
			} else {
				return _mm256_setr_epi32(
					f(arg.value.m256i_i32[0], offset),
					f(arg.value.m256i_i32[1], offset + 1ui32),
					f(arg.value.m256i_i32[2], offset + 2ui32),
					f(arg.value.m256i_i32[3], offset + 3ui32),
					f(arg.value.m256i_i32[4], offset + 4ui32),
					f(arg.value.m256i_i32[5], offset + 5ui32),
					f(arg.value.m256i_i32[6], offset + 6ui32),
					f(arg.value.m256i_i32[7], offset + 7ui32));
			}
		}

		template<typename F>
		__forceinline fp_vector generate(F const& f) {
			return _mm256_setr_ps(
				f(offset),
				f(offset + 1ui32),
				f(offset + 2ui32),
				f(offset + 3ui32),
				f(offset + 4ui32),
				f(offset + 5ui32),
				f(offset + 6ui32),
				f(offset + 7ui32));
		}
	};

	template<int32_t blk_index = 0>
	class full_unaligned_vector_operation : public full_vector_operation<blk_index> {
	public:
		full_unaligned_vector_operation(uint32_t o) : full_vector_operation<blk_index>(o) {}

		__forceinline fp_vector load(float const* source) {
			return _mm256_loadu_ps(source + full_vector_operation<blk_index>::offset);
		}
		__forceinline int_vector load(int32_t const* source) {
			return _mm256_loadu_si256((__m256i const*)(source + full_vector_operation<blk_index>::offset));
		}
		__forceinline void store(float* dest, fp_vector value) {
			_mm256_storeu_ps(dest + full_vector_operation<blk_index>::offset, value);
		}

		__forceinline fp_vector stream_load(float const* source) {
			return load(source);
		}
		__forceinline int_vector stream_load(int32_t const* source) {
			return load(source);
		}
		__forceinline void stream_store(float* dest, fp_vector value) {
			store(dest, value);
		}
	};

	class partial_vector_operation : public full_vector_operation<0> {
	protected:
		uint32_t const count;
	public:
		partial_vector_operation(uint32_t o, uint32_t c) : full_vector_operation<0>(o), count(c) {}

		constexpr static bool full_operation = false;

		__forceinline fp_vector load(float const* source) {
			int_vector_internal mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - count));
			return _mm256_maskload_ps(source + offset, mask);
		}
		__forceinline int_vector load(int32_t const* source) {
			int_vector_internal mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - count));
			return _mm256_maskload_epi32(source + offset, mask); //AVX2
		}
		__forceinline int8_t load(int8_t const* source) {
			return source[offset / 8ui32];
		}
		__forceinline void store(float* dest, fp_vector value) {
			int_vector_internal mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - count));
			_mm256_maskstore_ps(dest + offset, mask, value);
		}
		__forceinline int_vector partial_mask() {
			return _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - count));
		}

		__forceinline fp_vector unaligned_load(float const* source) {
			return load(source);
		}
		__forceinline int_vector unaligned_load(int32_t const* source) {
			return load(source);
		}

		__forceinline fp_vector gather_load(float const* source, __m256i indices) {
			fp_vector_internal mask = _mm256_loadu_ps((float*)(load_masks + 8ui32 - count));
			return _mm256_mask_i32gather_ps(_mm256_setzero_ps(), source, indices, mask, 4);
		}
		__forceinline int_vector gather_load(int32_t const* source, __m256i indices) {
			int_vector_internal mask = _mm256_loadu_si256((__m256i const*)(load_masks + 8ui32 - count));
			return _mm256_mask_i32gather_epi32(_mm256_setzero_si256(), source, indices, mask, 4);
		}
		__forceinline fp_vector gather_masked_load(float const* source, __m256i indices, mask_vector mask, fp_vector def = _mm256_setzero_ps()) {
			mask_vector imask = _mm256_loadu_ps((float const*)(load_masks + 8ui32 - count));
			return _mm256_mask_i32gather_ps(def, source, indices, mask & imask, 4);
		}
		__forceinline int_vector gather_masked_load(int32_t const* source, __m256i indices, mask_vector mask, int_vector def = _mm256_setzero_si256()) {
			mask_vector imask = _mm256_loadu_ps((float const*)(load_masks + 8ui32 - count));
			return _mm256_mask_i32gather_epi32(def, source, indices, _mm256_castps_si256(mask & imask), 4);
		}

		__forceinline void unaligned_store(float* dest, fp_vector value) {
			store(dest, value);
		}

		__forceinline fp_vector stream_load(float const* source) {
			return load(source);
		}
		__forceinline int_vector stream_load(int32_t const* source) {
			return load(source);
		}
		__forceinline void stream_store(float* dest, fp_vector value) {
			store(dest, value);
		}

		template<int32_t cache_lines>
		__forceinline void prefetch(int32_t const* source) {}
		template<int32_t cache_lines>
		__forceinline void prefetch(float const* source) {}
		template<int32_t cache_lines>
		__forceinline void nt_prefetch(int32_t const* source) {}
		template<int32_t cache_lines>
		__forceinline void nt_prefetch(float const* source) {}

		__forceinline void scatter_store(float* dest, fp_vector to_store, __m256i indices) {
			switch(count) {
				case 8:
					dest[indices.m256i_i32[7]] = to_store.value.m256_f32[7];
				case 7:
					dest[indices.m256i_i32[6]] = to_store.value.m256_f32[6];
				case 6:
					dest[indices.m256i_i32[5]] = to_store.value.m256_f32[5];
				case 5:
					dest[indices.m256i_i32[4]] = to_store.value.m256_f32[4];
				case 4:
					dest[indices.m256i_i32[3]] = to_store.value.m256_f32[3];
				case 3:
					dest[indices.m256i_i32[2]] = to_store.value.m256_f32[2];
				case 2:
					dest[indices.m256i_i32[1]] = to_store.value.m256_f32[1];
				case 1:
					dest[indices.m256i_i32[0]] = to_store.value.m256_f32[0];
				default:
					break;
			}
		}

		template<typename F>
		__forceinline fp_vector apply(F const& f, fp_vector arg) {
			return _mm256_setr_ps(
				count > 0 ? f(arg.value.m256_f32[0], offset) : 0.0f,
				count > 1 ? f(arg.value.m256_f32[1], offset + 1ui32) : 0.0f,
				count > 2 ? f(arg.value.m256_f32[2], offset + 2ui32) : 0.0f,
				count > 3 ? f(arg.value.m256_f32[3], offset + 3ui32) : 0.0f,
				count > 4 ? f(arg.value.m256_f32[4], offset + 4ui32) : 0.0f,
				count > 5 ? f(arg.value.m256_f32[5], offset + 5ui32) : 0.0f,
				count > 6 ? f(arg.value.m256_f32[6], offset + 6ui32) : 0.0f,
				count > 7 ? f(arg.value.m256_f32[7], offset + 7ui32) : 0.0f);
		}

		template<typename F>
		__forceinline auto apply(F const& f, int_vector arg) -> std::conditional_t<std::is_same_v<decltype(f(0, 0ui32)), float>, fp_vector, int_vector> {
			if constexpr(std::is_same_v<decltype(f(0, 0ui32)), float>) {
				return _mm256_setr_ps(
					count > 0 ? f(arg.value.m256i_i32[0], offset) : 0,
					count > 1 ? f(arg.value.m256i_i32[1], offset + 1ui32) : 0,
					count > 2 ? f(arg.value.m256i_i32[2], offset + 2ui32) : 0,
					count > 3 ? f(arg.value.m256i_i32[3], offset + 3ui32) : 0,
					count > 4 ? f(arg.value.m256i_i32[4], offset + 4ui32) : 0,
					count > 5 ? f(arg.value.m256i_i32[5], offset + 5ui32) : 0,
					count > 6 ? f(arg.value.m256i_i32[6], offset + 6ui32) : 0,
					count > 7 ? f(arg.value.m256i_i32[7], offset + 7ui32) : 0);
			} else {
				return _mm256_setr_epi32(
					count > 0 ? f(arg.value.m256i_i32[0], offset) : 0,
					count > 1 ? f(arg.value.m256i_i32[1], offset + 1ui32) : 0,
					count > 2 ? f(arg.value.m256i_i32[2], offset + 2ui32) : 0,
					count > 3 ? f(arg.value.m256i_i32[3], offset + 3ui32) : 0,
					count > 4 ? f(arg.value.m256i_i32[4], offset + 4ui32) : 0,
					count > 5 ? f(arg.value.m256i_i32[5], offset + 5ui32) : 0,
					count > 6 ? f(arg.value.m256i_i32[6], offset + 6ui32) : 0,
					count > 7 ? f(arg.value.m256i_i32[7], offset + 7ui32) : 0);
			}
		}

		template<typename F>
		__forceinline fp_vector apply_for_mask(F const& f, int_vector arg) {
			return _mm256_castsi256_ps(_mm256_setr_epi32(
				count > 0 ? f(arg.value.m256i_i32[0], offset) : 0,
				count > 1 ? f(arg.value.m256i_i32[1], offset + 1ui32) : 0,
				count > 2 ? f(arg.value.m256i_i32[2], offset + 2ui32) : 0,
				count > 3 ? f(arg.value.m256i_i32[3], offset + 3ui32) : 0,
				count > 4 ? f(arg.value.m256i_i32[4], offset + 4ui32) : 0,
				count > 5 ? f(arg.value.m256i_i32[5], offset + 5ui32) : 0,
				count > 6 ? f(arg.value.m256i_i32[6], offset + 6ui32) : 0,
				count > 7 ? f(arg.value.m256i_i32[7], offset + 7ui32) : 0)
			);
		}

		template<typename F>
		__forceinline fp_vector generate(F const& f) {
			return _mm256_setr_ps(
				count > 0 ? f(offset) : 0.0f,
				count > 1 ? f(offset + 1ui32) : 0.0f,
				count > 2 ? f(offset + 2ui32) : 0.0f,
				count > 3 ? f(offset + 3ui32) : 0.0f,
				count > 4 ? f(offset + 4ui32) : 0.0f,
				count > 5 ? f(offset + 5ui32) : 0.0f,
				count > 6 ? f(offset + 6ui32) : 0.0f,
				count > 7 ? f(offset + 7ui32) : 0.0f);
		}
	};

	using partial_unaligned_vector_operation = partial_vector_operation;
}
