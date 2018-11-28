#pragma once

constexpr int32_t vector_size = 4;
using int_vector_internal = __m128i;
using fp_vector_internal = __m128;

struct fp_vector {
	fp_vector_internal value;

	__forceinline fp_vector() : value(_mm_setzero_ps()) {}
	__forceinline constexpr fp_vector(__m128 v) : value(v) {}
	__forceinline fp_vector(float v) : value(_mm_set1_ps(v)) {}
	__forceinline constexpr operator __m128() {
		return value;
	}
	__forceinline float reduce() {
		// source: Peter Cordes
		__m128 shuf = _mm_movehdup_ps(value); // broadcast elements 3,1 to 2,0
		__m128 sums = _mm_add_ps(value, shuf);
		shuf = _mm_movehl_ps(shuf, sums); // high half -> low half
		sums = _mm_add_ss(sums, shuf);
		return _mm_cvtss_f32(sums);
	}
};

struct int_vector {
	int_vector_internal value;

	__forceinline constexpr int_vector(__m128i v) : value(v) {}
	__forceinline int_vector(int32_t v) : value(_mm_set1_epi32(v)) {}
	__forceinline constexpr operator __m128i() {
		return value;
	}
	__forceinline constexpr operator fp_vector() {
		return _mm_cvtepi32_ps(value);
	}
};

__forceinline fp_vector operator+(fp_vector a, fp_vector b) {
	return _mm_add_ps(a, b);
}
__forceinline fp_vector operator-(fp_vector a, fp_vector b) {
	return _mm_sub_ps(a, b);
}
__forceinline fp_vector operator*(fp_vector a, fp_vector b) {
	return _mm_mul_ps(a, b);
}
__forceinline fp_vector operator/(fp_vector a, fp_vector b) {
	return _mm_div_ps(a, b);
}

__forceinline fp_vector operator&(fp_vector a, fp_vector b) {
	return _mm_and_ps(a, b);
}
__forceinline fp_vector operator|(fp_vector a, fp_vector b) {
	return _mm_or_ps(a, b);
}
__forceinline fp_vector operator^(fp_vector a, fp_vector b) {
	return _mm_xor_ps(a, b);
}
__forceinline fp_vector operator~(fp_vector a) {
	return _mm_xor_ps(a, _mm_cmpeq_ps(_mm_setzero_ps(), _mm_setzero_ps()));
}
__forceinline fp_vector and_not(fp_vector a, fp_vector b) {
	return _mm_andnot_ps(b, a);
}

__forceinline fp_vector inverse(fp_vector a) {
	return _mm_rcp_ps(a);
}
__forceinline fp_vector sqrt(fp_vector a) {
	return _mm_sqrt_ps(a);
}
__forceinline fp_vector inverse_sqrt(fp_vector a) {
	return _mm_rsqrt_ps(a);
}

__forceinline fp_vector multiply_and_add(fp_vector a, fp_vector b, fp_vector c) {
	return _mm_add_ps(_mm_mul_ps(a, b), c);
}
__forceinline fp_vector multiply_and_subtract(fp_vector a, fp_vector b, fp_vector c) {
	return _mm_sub_ps(_mm_mul_ps(a, b), c);
}
__forceinline fp_vector negate_multiply_and_add(fp_vector a, fp_vector b, fp_vector c) {
	return _mm_sub_ps(c, _mm_mul_ps(a, b));
}
__forceinline fp_vector negate_multiply_and_subtract(fp_vector a, fp_vector b, fp_vector c) {
	return _mm_sub_ps(_mm_sub_ps(_mm_setzero_ps(), c), _mm_mul_ps(a, b));
}

__forceinline fp_vector min(fp_vector a, fp_vector b) {
	return _mm_min_ps(a, b);
}
__forceinline fp_vector max(fp_vector a, fp_vector b) {
	return _mm_max_ps(a, b);
}
__forceinline fp_vector floor(fp_vector a) {
	return _mm_floor_ps(a);
}
__forceinline fp_vector ceil(fp_vector a) {
	return _mm_ceil_ps(a);
}

__forceinline fp_vector operator<(fp_vector a, fp_vector b) {
	return _mm_cmplt_ps(a, b);
}
__forceinline fp_vector operator>(fp_vector a, fp_vector b) {
	return _mm_cmpgt_ps(a, b);
}
__forceinline fp_vector operator<=(fp_vector a, fp_vector b) {
	return _mm_cmple_ps(a, b);
}
__forceinline fp_vector operator>=(fp_vector a, fp_vector b) {
	return _mm_cmpge_ps(a, b);
}
__forceinline fp_vector operator==(fp_vector a, fp_vector b) {
	return _mm_cmpeq_ps(a, b);
}
__forceinline fp_vector operator!=(fp_vector a, fp_vector b) {
	return _mm_cmpneq_ps(a, b);
}
__forceinline fp_vector select(fp_vector mask, fp_vector a, fp_vector b) {
	return _mm_blendv_ps(b, a, mask);
}
__forceinline int32_t compress_mask(fp_vector mask) {
	return _mm_movemask_ps(mask);
}

inline const uint32_t load_masks[8] = {
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000
};

template<int32_t blk_index = 0>
class full_vector_operation {
protected:
	uint32_t const offset;
public:
	full_vector_operation(uint32_t o) : offset(o) {}

	constexpr static int32_t block_index = blk_index;

	__forceinline fp_vector zero() {
		return _mm_setzero_ps();
	}
	__forceinline int_vector int_zero() {
		return _mm_setzero_si128();
	}
	__forceinline fp_vector constant(float v) {
		return _mm_set1_ps(v);
	}
	__forceinline int_vector constant(int32_t v) {
		return _mm_set1_epi32(v);
	}

	__forceinline fp_vector load(float const* source) {
		return _mm_load_ps(source + offset);
	}
	__forceinline int_vector load(int32_t const* source) {
		return _mm_load_si128((__m128i const*)(source + offset));
	}
	__forceinline fp_vector unaligned_load(float const* source) {
		return _mm_loadu_ps(source + offset);
	}
	__forceinline int_vector unaligned_load(int32_t const* source) {
		return _mm_loadu_si128((__m128i const*)(source + offset));
	}

	__forceinline fp_vector gather_load(float const* source, __m128i indices) {
		return _mm_setr_ps(
			source[indices.m128i_i32[0]],
			source[indices.m128i_i32[1]],
			source[indices.m128i_i32[2]],
			source[indices.m128i_i32[3]]);
	}
	__forceinline int_vector gather_load(int32_t const* source, __m128i indices) {
		return _mm_setr_epi32(
			source[indices.m128i_i32[0]],
			source[indices.m128i_i32[1]],
			source[indices.m128i_i32[2]],
			source[indices.m128i_i32[3]]);
	}
	__forceinline fp_vector gather_masked_load(float const* source, __m128i indices, fp_vector mask, fp_vector def = _mm_setzero_ps()) {
		return select(mask, gather_load(source, indices), def);
	}
	__forceinline int_vector gather_masked_load(int32_t const* source, __m128i indices, int_vector mask, int_vector def = _mm_setzero_si128()) {
		return _mm_blendv_epi8(def, gather_load(source, indices), mask);
	}

	__forceinline void store(float* dest, fp_vector value) {
		_mm_store_ps(dest + offset, value);
	}
	__forceinline void unaligned_store(float* dest, fp_vector value) {
		_mm_storeu_ps(dest + offset, value);
	}

	__forceinline fp_vector stream_load(float const* source) {
		return _mm_castsi128_ps(_mm_stream_load_si128((__m128i const*)(source + offset)));
	}
	__forceinline int_vector stream_load(int32_t const* source) {
		return _mm_stream_load_si128((__m128i const*)(source + offset));
	}
	__forceinline void stream_store(float* dest, fp_vector value) {
		_mm_stream_ps(dest + offset, value);
	}

	template<int32_t cache_lines>
	__forceinline void prefetch(int32_t const* source) {
		if constexpr(block_index % 4 == 0) {
			_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines>
	__forceinline void prefetch(float const* source) {
		if constexpr(block_index % 4 == 0) {
			_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_T0);
		}
	}
	template<int32_t cache_lines>
	__forceinline void nt_prefetch(int32_t const* source) {
		if constexpr(block_index % 4 == 0) {
			_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_NTA);
		}
	}
	template<int32_t cache_lines>
	__forceinline void nt_prefetch(float const* source) {
		if constexpr(block_index % 4 == 0) {
			_mm_prefetch((char const*)(source + 16 * cache_lines), _MM_HINT_NTA);
		}
	}

	__forceinline int_vector partial_mask() {
		return _mm_loadu_si128((__m128i const*)load_masks);
	}

	template<typename F>
	__forceinline fp_vector apply(F const& f, fp_vector arg) {
		return _mm_setr_ps(
			f(arg.value.m128_f32[0], offset),
			f(arg.value.m128_f32[1], offset + 1ui32),
			f(arg.value.m128_f32[2], offset + 2ui32),
			f(arg.value.m128_f32[3], offset + 3ui32));
	}

	template<typename F>
	__forceinline int_vector apply(F const& f, int_vector arg) {
		return _mm_setr_epi32(
			f(arg.value.m128i_i32[0], offset),
			f(arg.value.m128i_i32[1], offset + 1ui32),
			f(arg.value.m128i_i32[2], offset + 2ui32),
			f(arg.value.m128i_i32[3], offset + 3ui32));
	}
};

template<int32_t blk_index = 0>
class full_unaligned_vector_operation : public full_vector_operation<blk_index> {
public:
	full_unaligned_vector_operation(uint32_t o) : full_vector_operation(o) {}

	__forceinline fp_vector load(float const* source) {
		return _mm_loadu_ps(source + offset);
	}
	__forceinline int_vector load(int32_t const* source) {
		return _mm_loadu_si128((__m128i const*)(source + offset));
	}
	__forceinline void store(float* dest, fp_vector value) {
		_mm_storeu_ps(dest + offset, value);
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
	partial_vector_operation(uint32_t o, uint32_t c) : full_vector_operation(o), count(c) {}

	__forceinline fp_vector load(float const* source) {
		fp_vector_internal mask = _mm_loadu_ps((float const*)(load_masks + 4ui32 - count));
		return _mm_and_ps(_mm_load_ps(source + offset), mask);
	}
	__forceinline int_vector load(int32_t const* source) {
		int_vector_internal mask = _mm_loadu_si128((__m128i const*)(load_masks + 4ui32 - count));
		return _mm_and_si128(_mm_load_si128((__m128i const*)(source + offset)), mask);
	}
	__forceinline void store(float* dest, fp_vector value) {
		int_vector_internal mask = _mm_loadu_si128((__m128i const*)(load_masks + 4ui32 - count));
		_mm_maskmoveu_si128(_mm_castps_si128(value), mask, (char*)(dest + offset));
	}
	__forceinline int_vector partial_mask() {
		return _mm_loadu_si128((__m128i const*)(load_masks + 4ui32 - count));
	}


	__forceinline fp_vector unaligned_load(float const* source) {
		return load(source);
	}
	__forceinline int_vector unaligned_load(int32_t const* source) {
		return load(source);
	}


	__forceinline fp_vector gather_load(float const* source, __m128i indices) {
		return _mm_setr_ps(
			count > 0 ? source[indices.m128i_i32[0]] : 0.0f,
			count > 1 ? source[indices.m128i_i32[1]] : 0.0f,
			count > 2 ? source[indices.m128i_i32[2]] : 0.0f,
			count > 3 ? source[indices.m128i_i32[3]] : 0.0f);
	}
	__forceinline int_vector gather_load(int32_t const* source, __m128i indices) {
		return _mm_setr_epi32(
			count > 0 ? source[indices.m128i_i32[0]] : 0,
			count > 1 ? source[indices.m128i_i32[1]] : 0,
			count > 2 ? source[indices.m128i_i32[2]] : 0,
			count > 3 ? source[indices.m128i_i32[3]] : 0);
	}
	__forceinline fp_vector gather_masked_load(float const* source, __m128i indices, fp_vector mask, fp_vector def = _mm_setzero_ps()) {
		return select(mask, gather_load(source, indices), def);
	}
	__forceinline int_vector gather_masked_load(int32_t const* source, __m128i indices, int_vector mask, int_vector def = _mm_setzero_si128()) {
		return _mm_blendv_epi8(def, gather_load(source, indices), mask);
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

	template<typename F>
	__forceinline fp_vector apply(F const& f, fp_vector arg) {
		return _mm_setr_ps(
			count > 0 ? f(arg.value.m128_f32[0], offset) : 0.0f,
			count > 1 ? f(arg.value.m128_f32[1], offset + 1ui32) : 0.0f,
			count > 2 ? f(arg.value.m128_f32[2], offset + 2ui32) : 0.0f,
			count > 3 ? f(arg.value.m128_f32[3], offset + 3ui32) : 0.0f);
	}

	template<typename F>
	__forceinline int_vector apply(F const& f, int_vector arg) {
		return _mm_setr_epi32(
			count > 0 ? f(arg.value.m128i_i32[0], offset) : 0,
			count > 1 ? f(arg.value.m128i_i32[1], offset + 1ui32) : 0,
			count > 2 ? f(arg.value.m128i_i32[2], offset + 2ui32) : 0,
			count > 3 ? f(arg.value.m128i_i32[3], offset + 3ui32) : 0);
	}
};

using partial_unaligned_vector_operation = partial_vector_operation;
