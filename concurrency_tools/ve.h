#pragma once
#include "common\\common.h"
#include <intrin.h>
#include <ppl.h>

#pragma warning( push )
#pragma warning( disable : 4324)

#ifdef __AVX2__
#include "ve_avx2.h"
#else
#ifdef __AVX__
#include "ve_avx.h"
#else // SSE
#include "ve_sse.h"
#endif
#endif

#pragma warning( pop ) 

namespace ve {


	RELEASE_INLINE constexpr float to_float(int32_t value) { return float(value); }
	RELEASE_INLINE constexpr bool and_not(bool a, bool b) { return (!b) && a; }
	RELEASE_INLINE constexpr float inverse(float a) { return 1.0f / a; }
	RELEASE_INLINE float sqrt(float a) { return std::sqrt(a); }
	RELEASE_INLINE float inverse_sqrt(float a) { return 1.0f / std::sqrt(a); }

	RELEASE_INLINE constexpr float multiply_and_add(float a, float b, float c) { return a * b + c; }
	RELEASE_INLINE constexpr float multiply_and_subtract(float a, float b, float c) { return a * b - c; }
	RELEASE_INLINE float negate_multiply_and_add(float a, float b, float c) { return c - (a * b); }
	RELEASE_INLINE float negate_multiply_and_subtract(float a, float b, float c) { return -(a * b) - c; }
	RELEASE_INLINE constexpr float min(float a, float b) { return std::min(a,b); }
	RELEASE_INLINE constexpr float max(float a, float b) { return std::max(a,b); }
	RELEASE_INLINE float ceil(float a) { return std::ceil(a); }
	RELEASE_INLINE float floor(float a) { return std::floor(a); }
	RELEASE_INLINE bool compress_mask(bool mask) { return mask; }

	RELEASE_INLINE constexpr float select(bool t, float a, float b) { return t ? a : b; }
	RELEASE_INLINE constexpr int32_t select(bool t, int32_t a, int32_t b) { return t ? a : b; }
	RELEASE_INLINE mask_vector widen_mask(mask_vector mask) { return mask; }
	RELEASE_INLINE bool widen_mask(bool mask) { return mask; }
	RELEASE_INLINE bool bit_test(int32_t val, int32_t bits) { return (val & bits) == bits; }

	template<typename T>
	RELEASE_INLINE auto improved_inverse(T a) {
		auto i = inverse(a);
		//return  -(a * i * i) + (i + i);
		return negate_multiply_and_add(a, i * i, i + i);
	}

	template<typename value_base, typename zero_is_null, typename individuator>
	RELEASE_INLINE bool load(tag_type<value_base, zero_is_null, individuator> e, bitfield_type const* source) {
		return bit_vector_test(source, to_index(e));
	}
	RELEASE_INLINE bool load(int32_t e, bitfield_type const* source) {
		return bit_vector_test(source, uint32_t(e));
	}

	template<typename value_base, typename zero_is_null, typename individuator, typename U>
	RELEASE_INLINE auto load(tag_type<value_base, zero_is_null, individuator> e, U const* source) -> std::enable_if_t<!std::is_same_v<std::remove_cv_t<U>, bitfield_type>, decay_tag<U>> {
		return source[to_index(e)];
	}

	template<typename U>
	RELEASE_INLINE auto load(int32_t e, U const* source) -> std::enable_if_t < !std::is_same_v<std::remove_cv_t<U>, bitfield_type>, decay_tag<U>> {
		return source[e];
	}
	
	template<typename value_base, typename zero_is_null, typename individuator, typename U>
	RELEASE_INLINE auto load(tag_type<value_base, zero_is_null, individuator> e,
		tagged_array_view<U, typename ve_identity<tag_type<value_base, zero_is_null, individuator>>::type> source) {
		return ve::load(e, source.data());
	}
	

	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	RELEASE_INLINE void prefetch(tag_type<value_base, zero_is_null, individuator> e, float const* source) {}
	template<int32_t cache_lines>
	RELEASE_INLINE void prefetch(int32_t e, float const* source) {}

	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	RELEASE_INLINE void nt_prefetch(tag_type<value_base, zero_is_null, individuator> e, float const* source) {}
	template<int32_t cache_lines>
	RELEASE_INLINE void nt_prefetch(int32_t e, float const* source) {}


	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	RELEASE_INLINE void prefetch(tag_type<value_base, zero_is_null, individuator> e, int32_t const* source) {}
	template<int32_t cache_lines>
	RELEASE_INLINE void prefetch(int32_t e, int32_t const* source) {}

	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	RELEASE_INLINE void nt_prefetch(tag_type<value_base, zero_is_null, individuator> e, int32_t const* source) {}
	template<int32_t cache_lines>
	RELEASE_INLINE void nt_prefetch(int32_t e, int32_t const* source) {}


	template<typename value_base, typename zero_is_null, typename individuator>
	RELEASE_INLINE void store(tag_type<value_base, zero_is_null, individuator> e, float* dest, float value) {
		dest[to_index(e)] = value;
	}
	RELEASE_INLINE void store(int32_t e, float* dest, float value) {
		dest[e] = value;
	}

	template<typename A, typename B, typename C>
	struct _has_prefetch : std::false_type {};
	template<typename A, typename C>
	struct _has_prefetch<A, decltype(void(std::declval<A>().prefetch(std::declval<C>()))), C> : std::true_type {};
	template<typename A, typename C>
	constexpr bool has_prefetch = _has_prefetch<A, void, C>::value;


	struct partition_range {
		uint32_t low;
		uint32_t high;
	};

	template<uint32_t divisions_count, uint32_t chunk_size>
	partition_range generate_partition_range(int32_t partition_index, int32_t total_count) {
		uint32_t const total_chunks = uint32_t((total_count + chunk_size - 1) / chunk_size);
		uint32_t const low_chunk_size = total_chunks / divisions_count;
		uint32_t const high_chunks_count = total_chunks - low_chunk_size * divisions_count;

		if(uint32_t(partition_index) < divisions_count - high_chunks_count) {
			return partition_range{ uint32_t(partition_index) * low_chunk_size * chunk_size, uint32_t(partition_index + 1) * low_chunk_size * chunk_size };
		} else {
			uint32_t const first_high_chunk = (divisions_count - high_chunks_count) * low_chunk_size * chunk_size;
			uint32_t const high_chunk_index = uint32_t(partition_index) - (divisions_count - high_chunks_count);

			return partition_range{
				first_high_chunk + high_chunk_index * (low_chunk_size + 1) * chunk_size,
				std::min(first_high_chunk + (high_chunk_index + 1) * (low_chunk_size + 1) * chunk_size, uint32_t(total_count))
			};
		}
	}

	constexpr int32_t block_repitition = 4;

	template<typename tag_type, typename F>
	RELEASE_INLINE void execute_serial_fast(uint32_t count, F&& functor) {
		const uint32_t full_units = (count + uint32_t(vector_size - 1)) & ~uint32_t(vector_size - 1);
		const uint32_t reps = full_units / vector_size;
		const uint32_t quad_reps = reps / uint32_t(block_repitition);

		if constexpr(has_prefetch < F, prefetch_stride<0> > ) {

			int32_t i = 0;
			for(; i < int32_t(quad_reps); ++i) {
				functor.prefetch(prefetch_stride<0>{ i * 4 });
				functor.prefetch(prefetch_stride<1>{ i * 4 + 1 });
				functor.prefetch(prefetch_stride<2>{ i * 4 + 2 });
				functor.prefetch(prefetch_stride<3>{ i * 4 + 3 });
			}
			if((reps & 3) == 1) {
				functor.prefetch(prefetch_stride<0>{ i * 4 });
			} else if((reps & 3) == 2) {
				functor.prefetch(prefetch_stride<0>{ i * 4 });
				functor.prefetch(prefetch_stride<1>{ i * 4 + 1 });
			} else if((reps & 3) == 3) {
				functor.prefetch(prefetch_stride<0>{ i * 4 });
				functor.prefetch(prefetch_stride<1>{ i * 4 + 1 });
				functor.prefetch(prefetch_stride<2>{ i * 4 + 2 });
			}
		}

		int32_t i = 0;
		for(; i < int32_t(quad_reps); ++i) {
			functor(contiguous_tags<tag_type, 0>(i * (vector_size * 4)));
			functor(contiguous_tags<tag_type, 1>(i * (vector_size * 4) + vector_size));
			functor(contiguous_tags<tag_type, 2>(i * (vector_size * 4) + vector_size * 2));
			functor(contiguous_tags<tag_type, 3>(i * (vector_size * 4) + vector_size * 3));
		}
		if((reps & 3) == 1) {
			functor(contiguous_tags<tag_type, 1>(i * (vector_size * 4)));
		} else if((reps & 3) == 2) {
			functor(contiguous_tags<tag_type, 0>(i * (vector_size * 4)));
			functor(contiguous_tags<tag_type, 1>(i * (vector_size * 4) + vector_size));
		} else if((reps & 3) == 3) {
			functor(contiguous_tags<tag_type, 1>(i * (vector_size * 4)));
			functor(contiguous_tags<tag_type, 2>(i * (vector_size * 4) + vector_size));
			functor(contiguous_tags<tag_type, 3>(i * (vector_size * 4) + vector_size * 2));
		}
	}

	template<typename tag_type, typename F>
	RELEASE_INLINE void execute_serial(uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count & uint32_t(vector_size - 1);

		execute_serial_fast<tag_type>(full_units, std::forward<F>(functor));

		if(remainder != 0) {
			functor(partial_contiguous_tags<tag_type>(full_units, remainder));
		}
	}

	template<typename tag_type, typename F>
	RELEASE_INLINE void execute_serial_unaligned(uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count & uint32_t(vector_size - 1);

		const uint32_t reps = full_units / vector_size;
		const uint32_t quad_reps = reps / uint32_t(block_repitition);

		int32_t i = 0;
		for(; i < int32_t(quad_reps); ++i) {
			functor(unaligned_contiguous_tags<tag_type, 0>(i * (vector_size * 4)));
			functor(unaligned_contiguous_tags<tag_type, 1>(i * (vector_size * 4) + vector_size));
			functor(unaligned_contiguous_tags<tag_type, 2>(i * (vector_size * 4) + vector_size * 2));
			functor(unaligned_contiguous_tags<tag_type, 3>(i * (vector_size * 4) + vector_size * 3));
		}
		if((reps & 3) == 1) {
			functor(unaligned_contiguous_tags<tag_type, 1>(i * (vector_size * 4)));
		} else if((reps & 3) == 2) {
			functor(unaligned_contiguous_tags<tag_type, 0>(i * (vector_size * 4)));
			functor(unaligned_contiguous_tags<tag_type, 1>(i * (vector_size * 4) + vector_size));
		} else if((reps & 3) == 3) {
			functor(unaligned_contiguous_tags<tag_type, 1>(i * (vector_size * 4)));
			functor(unaligned_contiguous_tags<tag_type, 2>(i * (vector_size * 4) + vector_size));
			functor(unaligned_contiguous_tags<tag_type, 3>(i * (vector_size * 4) + vector_size * 2));
		}

		if(remainder != 0) {
			functor(partial_contiguous_tags<tag_type>(full_units, remainder));
		}
	}

	template<typename index_type, typename F>
	RELEASE_INLINE void execute_single(index_type offset, F&& functor) {
		functor(offset);
	}

	template<typename tag_type, typename F>
	RELEASE_INLINE void execute_subsequence(uint32_t start, uint32_t end, F&& functor) {
		const auto count = start - end;
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count - full_units;

		for(int32_t i = 0; i < int32_t(full_units); i += vector_size) {
			functor(unaligned_contiguous_tags<tag_type, 0>(start + i));
		}
		if(remainder != 0) {
			functor(partial_contiguous_tags<tag_type>(full_units + start, remainder));
		}
	}

	template<typename tag_type, typename F>
	void execute_parallel(uint32_t start, uint32_t count, F&& functor) {
		const uint32_t full_units = (count + 15ui32) & ~15ui32;
		concurrency::parallel_for(start, full_units, 16ui32, [&functor](uint32_t offset) {
			if constexpr(vector_size == 16) {
				functor(contiguous_tags<tag_type, 0>(offset));
			} else if constexpr(vector_size == 8) {
				functor(contiguous_tags<tag_type, 0>(offset));
				functor(contiguous_tags<tag_type, 1>(offset + 8));
			} else if constexpr(vector_size == 4) {
				functor(contiguous_tags<tag_type, 0>(offset));
				functor(contiguous_tags<tag_type, 1>(offset + 4));
				functor(contiguous_tags<tag_type, 2>(offset + 8));
				functor(contiguous_tags<tag_type, 3>(offset + 12));
			} else {
				static_assert(vector_size == 16 || vector_size == 8 || vector_size == 4);
			}
		}, concurrency::static_partitioner());
	}

	template<typename tag_type, typename F>
	void execute_parallel(uint32_t count, F&& functor) {
		execute_parallel<tag_type>(0, count, functor);
	}

	template<typename tag_type, typename F>
	void execute_parallel_exact(uint32_t start, uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~15ui32;
		const uint32_t remainder = count - full_units;
		concurrency::parallel_for(start, full_units, 16ui32, [&functor](uint32_t offset) {
			if constexpr(vector_size == 16) {
				functor(contiguous_tags<tag_type, 0>(offset));
			} else if constexpr(vector_size == 8) {
				functor(contiguous_tags<tag_type, 0>(offset));
				functor(contiguous_tags<tag_type, 1>(offset + 8));
			} else if constexpr(vector_size == 4) {
				functor(contiguous_tags<tag_type, 0>(offset));
				functor(contiguous_tags<tag_type, 1>(offset + 4));
				functor(contiguous_tags<tag_type, 2>(offset + 8));
				functor(contiguous_tags<tag_type, 3>(offset + 12));
			} else {
				static_assert(vector_size == 16 || vector_size == 8 || vector_size == 4);
			}
		}, concurrency::static_partitioner());
		if constexpr(vector_size == 16) {
			if(remainder != 0) {
				functor(partial_contiguous_tags<tag_type>(full_units, remainder));
			}
		} else if constexpr(vector_size == 8) {
			if(remainder > 8ui32) {
				functor(contiguous_tags<tag_type, 1>(full_units));
				functor(partial_contiguous_tags<tag_type>(full_units + 8ui32, remainder - 8ui32));
			} else if(remainder == 8ui32) {
				functor(contiguous_tags<tag_type, 1>(full_units));
			} else if(remainder != 0) {
				functor(partial_contiguous_tags<tag_type>(full_units, remainder));
			}
		} else if constexpr(vector_size == 4) {
			if(remainder > 12ui32) {
				functor(contiguous_tags<tag_type, 1>(full_units));
				functor(contiguous_tags<tag_type, 2>(full_units + 4ui32));
				functor(contiguous_tags<tag_type, 3>(full_units + 8ui32));
				functor(partial_contiguous_tags<tag_type>(full_units + 12ui32, remainder - 12ui32));
			} else if(remainder == 12ui32) {
				functor(contiguous_tags<tag_type, 1>(full_units));
				functor(contiguous_tags<tag_type, 2>(full_units + 4ui32));
				functor(contiguous_tags<tag_type, 3>(full_units + 8ui32));
			} else if(remainder > 8ui32) {
				functor(contiguous_tags<tag_type, 0>(full_units));
				functor(contiguous_tags<tag_type, 1>(full_units + 4ui32));
				functor(partial_contiguous_tags<tag_type>(full_units + 8ui32, remainder - 8ui32));
			} else if(remainder == 8ui32) {
				functor(contiguous_tags<tag_type, 0>(full_units));
				functor(contiguous_tags<tag_type, 1>(full_units + 4ui32));
			} else if(remainder > 4ui32) {
				functor(contiguous_tags<tag_type, 1>(full_units));
				functor(partial_contiguous_tags<tag_type>(full_units + 4ui32, remainder - 4ui32));
			} else if(remainder == 4ui32) {
				functor(contiguous_tags<tag_type, 1>(full_units));
			} else if(remainder != 0) {
				functor(partial_contiguous_tags<tag_type>(full_units, remainder));
			}
		}
	}

	template<typename tag_type, typename F>
	void execute_parallel_exact(uint32_t count, F&& functor) {
		execute_parallel_exact<tag_type>(0, count, functor);
	}

	struct serial_exact {
		template<typename tag_type, typename F>
		RELEASE_INLINE static void execute(uint32_t count, F&& functor) {
			execute_serial<tag_type>(count, functor);
		}
	};
	struct serial_unaligned {
		template<typename tag_type, typename F>
		RELEASE_INLINE static void execute(uint32_t count, F&& functor) {
			execute_serial_unaligned<tag_type>(count, functor);
		}
	};
	struct serial {
		template<typename tag_type, typename F>
		RELEASE_INLINE static void execute(uint32_t count, F&& functor) {
			assert(count % ve::vector_size == 0);
			execute_serial_fast<tag_type>(count, functor);
		}
	};
	struct par {
		template<typename tag_type, typename F>
		RELEASE_INLINE static void execute(uint32_t count, F&& functor) {
			assert(count % ve::vector_size == 0);
			execute_parallel<tag_type>(count, functor);
		}
	};
	struct par_exact {
		template<typename tag_type, typename F>
		RELEASE_INLINE static void execute(uint32_t count, F&& functor) {
			execute_parallel_exact<tag_type>(count, functor);
		}
	};

	constexpr inline uint32_t to_vector_size(uint32_t i) {
		return (i + (ve::vector_size - 1ui32)) & ~(ve::vector_size - 1ui32);
	}

#define ve_aligned_alloca(size) reinterpret_cast<void*>(( \
	reinterpret_cast<uintptr_t>(_alloca(size + ve::vector_size * 4)) + (ve::vector_size * 4)) & ~(uintptr_t(ve::vector_size * 4 - 1)))

	namespace ve_impl {
		struct reduce_operator {
			float const* const vector;
			fp_vector accumulator[block_repitition] = { fp_vector{}, fp_vector{}, fp_vector{}, fp_vector{} };

			reduce_operator(float const* v) : vector(v) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				accumulator[executor.block_index] = accumulator[executor.block_index] + load(executor, vector);
			}
		};
	}

	template<typename T, typename itype, typename policy = serial_exact>
	RELEASE_INLINE auto reduce(uint32_t size, tagged_array_view<T, itype> vector, policy p = serial_exact())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float>, float> {
		assert(vector.size >= int32_t(size));
		
		ve_impl::reduce_operator ro(vector.data());
		policy::template execute<int32_t>(size, ro);
		return ((ro.accumulator[0] + ro.accumulator[1]) + (ro.accumulator[2] + ro.accumulator[3])).reduce();
	}

	namespace ve_impl {
		struct rescale_operator {
			float* const vector;
			float const scale;

			rescale_operator(float* v, float s) : vector(v), scale(s) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, vector, scale * load(executor, vector));
			}
		};
	}

	template<typename T, typename itype, typename policy = serial>
	RELEASE_INLINE auto rescale(uint32_t size, tagged_array_view<T, itype> vector, float scale_factor, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float>, void> {
		assert(vector.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::rescale_operator(vector.data(), scale_factor));
	}

	namespace ve_impl {
		struct vector_accumulate_operator {
			float* const dest;
			float const* const accumulated;

			vector_accumulate_operator(float* d, float const* a) : dest(d), accumulated(a) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, dest, load(executor, accumulated) + load(executor, dest));
			}
		};
	}

	template<typename itype, typename T, typename policy = serial>
	RELEASE_INLINE auto accumulate(uint32_t size, tagged_array_view<float, itype> destination, tagged_array_view<T, itype> accumulated, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float>, void> {

		assert(destination.size >= int32_t(size));
		assert(accumulated.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_accumulate_operator(destination.data(), accumulated.data()));
	}

	namespace ve_impl {
	   struct vector_copy_operator {
		   float* const dest;
		   float const* const a;

		   vector_copy_operator(float* d, float const* a) : dest(d), a(a) {};

		   template<typename T>
		   RELEASE_INLINE void operator()(T executor) {
			   store(executor, dest, load(executor, a));
		   }
	   };
	}

	template<typename itype, typename T, typename policy = serial>
	RELEASE_INLINE auto copy(uint32_t size, tagged_array_view<float, itype> destination, tagged_array_view<T, itype> source, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float>, void> {

		assert(destination.size >= int32_t(size));
		assert(source.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_copy_operator(destination.data(), source.data()));
	}

	namespace ve_impl {
		struct vector_accumulate_scaled_operator {
			float* const dest;
			float const* const accumulated;
			float const scale;

			vector_accumulate_scaled_operator(float* d, float const* a, float s) : dest(d), accumulated(a), scale(s) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, dest, ve::multiply_and_add(scale, load(executor, accumulated), load(executor, dest)));
			}
		};
	}

	template<typename itype, typename T, typename policy = serial>
	RELEASE_INLINE auto accumulate_scaled(uint32_t size, tagged_array_view<float, itype> destination, tagged_array_view<T, itype> accumulated, float scale, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float>, void> {
		
		assert(destination.size >= int32_t(size));
		assert(accumulated.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_accumulate_scaled_operator(destination.data(), accumulated.data(), scale));
	}

	namespace ve_impl {
		struct vector_accumulate_ui8_scaled_operator {
			float* const dest;
			uint8_t const* const accumulated;
			float const scale;

			vector_accumulate_ui8_scaled_operator(float* d, uint8_t const* a, float s) : dest(d), accumulated(a), scale(s) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, dest, ve::multiply_and_add(scale, to_float(load(executor, accumulated)), load(executor, dest)));
			}
		};
	}

	template<typename itype, typename T, typename policy = serial>
	RELEASE_INLINE auto accumulate_ui8_scaled(uint32_t size, tagged_array_view<float, itype> destination, tagged_array_view<T, itype> accumulated, float scale, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, uint8_t>, void> {

		assert(destination.size >= int32_t(size));
		assert(accumulated.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_accumulate_ui8_scaled_operator(destination.data(), accumulated.data(), scale));
	}

	namespace ve_impl {
		struct dot_product_operator {
			float const* const a;
			float const* const b;
			fp_vector accumulator[block_repitition] = { fp_vector{}, fp_vector{}, fp_vector{}, fp_vector{} };

			dot_product_operator(float const* va, float const* vb) : a(va), b(vb) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				accumulator[executor.block_index] = ve::multiply_and_add(load(executor, a), load(executor, b), accumulator[executor.block_index]);
			}
		};
	}

	template<typename itype, typename T, typename U, typename policy = serial_exact>
	RELEASE_INLINE auto dot_product(uint32_t size, tagged_array_view<T, itype> a, tagged_array_view<U, itype> b, policy p = serial_exact())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float> && std::is_same_v<std::remove_cv_t<U>, float>, float> {
		
		assert(b.size >= int32_t(size));
		assert(a.size >= int32_t(size));

		ve_impl::dot_product_operator op(a.data(), b.data());
		policy::template execute<int32_t>(size, op);

		return ((op.accumulator[0] + op.accumulator[1]) + (op.accumulator[2] + op.accumulator[3])).reduce();
	}

	namespace ve_impl {
		struct vector_accumulate_product_operator {
			float* const dest;
			float const* const a;
			float const* const b;

			vector_accumulate_product_operator(float* d, float const* va, float const* vb) : dest(d), a(va), b(vb) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, dest, ve::multiply_and_add(load(executor, a), load(executor, b), load(executor, dest)));
			}
		};
	}

	template<typename itype, typename T, typename U, typename policy = serial>
	RELEASE_INLINE auto accumulate_product(uint32_t size, tagged_array_view<float, itype> destination, tagged_array_view<T, itype> a, tagged_array_view<U, itype> b, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float> && std::is_same_v<std::remove_cv_t<U>, float>, void> {
		
		assert(destination.size >= int32_t(size));
		assert(b.size >= int32_t(size));
		assert(a.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_accumulate_product_operator(destination.data(), a.data(), b.data()));
	}

	namespace ve_impl {
		struct vector_accumulate_scaled_product_operator {
			float* const dest;
			float const* const a;
			float const* const b;
			float const scale;

			vector_accumulate_scaled_product_operator(float* d, float const* va, float const* vb, float s) : dest(d), a(va), b(vb), scale(s) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, dest, ve::multiply_and_add(load(executor, a), load(executor, b) * scale, load(executor, dest)));
			}
		};
	}

	template<typename itype, typename T, typename U, typename policy = serial>
	RELEASE_INLINE auto accumulate_scaled_product(uint32_t size, tagged_array_view<float, itype> destination, tagged_array_view<T, itype> a, tagged_array_view<U, itype> b, float scale, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float> && std::is_same_v<std::remove_cv_t<U>, float>, void> {

		assert(destination.size >= int32_t(size));
		assert(b.size >= int32_t(size));
		assert(a.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_accumulate_scaled_product_operator(destination.data(), a.data(), b.data(), scale));
	}

	namespace ve_impl {
		struct vector_accumulate_sum_operator {
			float* const dest;
			float const* const a;
			float const* const b;

			vector_accumulate_sum_operator(float* d, float const* va, float const* vb) : dest(d), a(va), b(vb) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, dest, load(executor, a) + load(executor, b) + load(executor, dest));
			}
		};
	}

	template<typename itype, typename T, typename U, typename policy = serial>
	RELEASE_INLINE auto accumulate_sum(uint32_t size, tagged_array_view<float, itype> destination, tagged_array_view<T, itype> a, tagged_array_view<const float, itype> b, policy p = serial())
		-> std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, float> && std::is_same_v<std::remove_cv_t<U>, float>, void> {

		assert(destination.size >= int32_t(size));
		assert(b.size >= int32_t(size));
		assert(a.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_accumulate_sum_operator(destination.data(), a.data(), b.data()));
	}

	namespace ve_impl {
		struct vector_zero_operator {
			float* const dest;
			vector_zero_operator(float* d) : dest(d) {};

			template<typename T>
			RELEASE_INLINE void operator()(T executor) {
				store(executor, dest, 0.0f);
			}
		};
	}

	template<typename itype, typename policy = serial>
	RELEASE_INLINE void set_zero(uint32_t size, tagged_array_view<float, itype> destination, policy p = serial()) {
		assert(destination.size >= int32_t(size));

		policy::template execute<int32_t>(size, ve_impl::vector_zero_operator(destination.data()));
	}
}