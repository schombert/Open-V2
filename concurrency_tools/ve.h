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


	__forceinline constexpr float to_float(int32_t value) { return float(value); }
	__forceinline constexpr bool and_not(bool a, bool b) { return (!b) && a; }
	__forceinline constexpr float inverse(float a) { return 1.0f / a; }
	__forceinline float sqrt(float a) { return std::sqrt(a); }
	__forceinline float inverse_sqrt(float a) { return 1.0f / std::sqrt(a); }

	__forceinline constexpr float multiply_and_add(float a, float b, float c) { return a * b + c; }
	__forceinline constexpr float multiply_and_subtract(float a, float b, float c) { return a * b - c; }
	__forceinline float negate_multiply_and_add(float a, float b, float c) { return c - (a * b); }
	__forceinline float negate_multiply_and_subtract(float a, float b, float c) { return -(a * b) - c; }
	__forceinline constexpr float min(float a, float b) { return std::max(a,b); }
	__forceinline constexpr float max(float a, float b) { return std::min(a,b); }
	__forceinline float ceil(float a) { return std::ceil(a); }
	__forceinline float floor(float a) { return std::floor(a); }
	__forceinline bool compress_mask(bool mask) { return mask; }

	__forceinline constexpr float select(bool t, float a, float b) { return t ? a : b; }
	__forceinline mask_vector widen_mask(mask_vector mask) { return mask; }
	__forceinline bool widen_mask(bool mask) { return mask; }
	__forceinline bool bit_test(int32_t val, int32_t bits) { return (val & bits) == bits; }

	template<typename T>
	__forceinline auto improved_inverse(T a) {
		auto i = inverse(a);
		//return  -(a * i * i) + (i + i);
		return negate_multiply_and_add(a, i * i, i + i);
	}


	template<typename value_base, typename zero_is_null, typename individuator, typename U>
	__forceinline auto load(tag_type<value_base, zero_is_null, individuator> e, U const* source) -> decay_tag<U> {
		return source[to_index(e)];
	}
	template<typename tt, typename U>
	__forceinline auto load(expanded_tag<tt> e, U const* source) -> decay_tag<U> {
		return source[to_index(e)];
	}
	template<typename U>
	__forceinline auto load(int32_t e, U const* source) -> decay_tag<U> {
		return source[e];
	}


	template<typename value_base, typename zero_is_null, typename individuator, typename U>
	__forceinline auto load(tag_type<value_base, zero_is_null, individuator> e,
		tagged_array_view<U, typename ve_identity<tag_type<value_base, zero_is_null, individuator>>::type, true> source) 
	-> decay_tag<std::remove_cv_t<U>> {
		return source[e];
	}
	template<typename value_base, typename zero_is_null, typename individuator, typename U>
	__forceinline auto load(tag_type<value_base, zero_is_null, individuator> e,
		tagged_array_view<U, typename ve_identity<expanded_tag<tag_type<value_base, zero_is_null, individuator>>>::type, true> source)
		-> decay_tag<std::remove_cv_t<U>> {
		return source[e];
	}
	template<typename tt, typename U>
	__forceinline auto load(expanded_tag<tt> e, tagged_array_view<U, typename ve_identity<tt>::type, true> source)
		-> decay_tag<std::remove_cv_t<U>> {
		return source[e];
	}
	template<typename tt, typename U>
	__forceinline auto load(expanded_tag<tt> e, tagged_array_view<U, typename ve_identity<expanded_tag<tt>>::type, true> source)
		-> decay_tag<std::remove_cv_t<U>> {
		return source[e];
	}


	template<typename value_base, typename zero_is_null, typename individuator>
	__forceinline bool load(tag_type<value_base, zero_is_null, individuator> e, bitfield_type const* source) {
		return bit_vector_test(source, uint32_t(to_index(e)));
	}
	template<typename tt>
	__forceinline bool load(expanded_tag<tt> e, bitfield_type const* source) {
		return bit_vector_test(source, uint32_t(to_index(e)));
	}
	__forceinline bool load(int32_t e, bitfield_type const* source) {
		return bit_vector_test(source, uint32_t(e));
	}

	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	__forceinline void prefetch(tag_type<value_base, zero_is_null, individuator> e, float const* source) {}
	template<int32_t cache_lines, typename tt>
	__forceinline void prefetch(expanded_tag<tt> e, float const* source) {}
	template<int32_t cache_lines>
	__forceinline void prefetch(int32_t e, float const* source) {}

	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	__forceinline void nt_prefetch(tag_type<value_base, zero_is_null, individuator> e, float const* source) {}
	template<int32_t cache_lines, typename tt>
	__forceinline void nt_prefetch(expanded_tag<tt> e, float const* source) {}
	template<int32_t cache_lines>
	__forceinline void nt_prefetch(int32_t e, float const* source) {}


	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	__forceinline void prefetch(tag_type<value_base, zero_is_null, individuator> e, int32_t const* source) {}
	template<int32_t cache_lines, typename tt>
	__forceinline void prefetch(expanded_tag<tt> e, int32_t const* source) {}
	template<int32_t cache_lines>
	__forceinline void prefetch(int32_t e, int32_t const* source) {}

	template<int32_t cache_lines, typename value_base, typename zero_is_null, typename individuator>
	__forceinline void nt_prefetch(tag_type<value_base, zero_is_null, individuator> e, int32_t const* source) {}
	template<int32_t cache_lines, typename tt>
	__forceinline void nt_prefetch(expanded_tag<tt> e, int32_t const* source) {}
	template<int32_t cache_lines>
	__forceinline void nt_prefetch(int32_t e, int32_t const* source) {}


	template<typename value_base, typename zero_is_null, typename individuator>
	__forceinline void load(tag_type<value_base, zero_is_null, individuator> e, float* dest, float value) {
		dest[to_index(e)] = value;
	}
	template<typename tt>
	__forceinline void load(expanded_tag<tt> e, float* dest, float value) {
		dest[to_index(e)] = value;
	}
	__forceinline void load(int32_t e, float* dest, float value) {
		dest[e] = value;
	}


	constexpr int32_t block_repitition = 4;

	template<typename tag_type, typename F>
	__forceinline void execute_serial_fast(uint32_t count, F&& functor) {
		const uint32_t full_units = (count + uint32_t(vector_size - 1)) & ~uint32_t(vector_size - 1);
		const uint32_t reps = full_units / vector_size;
		const uint32_t quad_reps = reps / uint32_t(block_repitition);

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
	__forceinline void execute_serial(uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count - full_units;

		execute_serial_fast<tag_type>(full_units, std::forward<F>(functor));

		if(remainder != 0) {
			functor(partial_contiguous_tags<tag_type>(full_units, remainder));
		}
	}

	template<typename tag_type, typename F>
	__forceinline void execute_serial_unaligned(uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count - full_units;

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
	__forceinline void execute_single(index_type offset, F&& functor) {
		functor(offset);
	}

	template<typename tag_type, typename F>
	__forceinline void execute_subsequence(uint32_t start, uint32_t end, F&& functor) {
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

	namespace ve_impl {
		struct reduce_operator {
			float const* const vector;
			fp_vector accumulator[block_repitition] = { fp_vector{}, fp_vector{}, fp_vector{}, fp_vector{} };

			reduce_operator(float const* v) : vector(v) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				accumulator[executor.block_index] = accumulator[executor.block_index] + load(executor, vector);
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline float reduce(tagged_array_view<const float, itype, padded> vector) {
		ve_impl::reduce_operator ro(vector.data());
		execute_serial<itype>(uint32_t(std::end(vector) - vector.data()), ro);
		return ((ro.accumulator[0] + ro.accumulator[1]) + (ro.accumulator[2] + ro.accumulator[3])).reduce();
	}

	namespace ve_impl {
		struct rescale_operator {
			float* const vector;
			float const scale;

			rescale_operator(float* v, float s) : vector(v), scale(s) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				store(executor, vector, scale * load(executor, vector));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void rescale(tagged_array_view<float, itype, padded> vector, float scale_factor) {
		execute_serial_fast<itype>(uint32_t(std::end(vector) - vector.data()), ve_impl::rescale_operator(vector.data(), scale_factor));
	}

	namespace ve_impl {
		struct vector_accumulate_operator {
			float* const dest;
			float const* const accumulated;

			vector_accumulate_operator(float* d, float const* a) : dest(d), accumulated(a) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				store(executor, dest, load(executor, accumulated) + load(executor, dest));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> accumulated) {
		assert(std::end(destination) - destination.data() == std::end(accumulated) - accumulated.data());
		execute_serial_fast<itype>(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_operator(destination.data(), accumulated.data()));
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_exact(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> accumulated) {
		execute_serial_unaligned<itype>(std::min(uint32_t(std::end(destination) - destination.data()), uint32_t(std::end(accumulated) - accumulated.data())),
			ve_impl::vector_accumulate_operator(destination.data(), accumulated.data()));
	}

	namespace ve_impl {
	   struct vector_copy_operator {
		   float* const dest;
		   float const* const a;

		   vector_copy_operator(float* d, float const* a) : dest(d), a(a) {};

		   template<typename T>
		   __forceinline void operator()(T executor) {
			   store(executor, dest, load(executor, a));
		   }
	   };
	}

	template<typename itype, bool padded>
	__forceinline void copy(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> source) {
		assert(std::end(destination) - destination.data() >= std::end(source) - source.data());
		execute_serial_fast<itype>(uint32_t(std::end(source) - source.data()), ve_impl::vector_copy_operator(destination.data(), source.data()));
	}

	template<typename itype, bool padded>
	__forceinline void par_copy(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> source) {
		assert(std::end(destination) - destination.data() >= std::end(source) - source.data());
		execute_parallel<itype>(uint32_t(std::end(source) - source.data()), ve_impl::vector_copy_operator(destination.data(), source.data()));
	}

	namespace ve_impl {
		struct vector_accumulate_scaled_operator {
			float* const dest;
			float const* const accumulated;
			float const scale;

			vector_accumulate_scaled_operator(float* d, float const* a, float s) : dest(d), accumulated(a), scale(s) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				store(executor, dest, ve::multiply_and_add(scale, load(executor, accumulated), load(executor, dest)));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_scaled(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> accumulated, float scale) {
		assert(std::end(destination) - destination.data() == std::end(accumulated) - accumulated.data());
		execute_serial_fast<itype>(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_scaled_operator(destination.data(), accumulated.data(), scale));
	}

	template<typename itype, bool padded>
	__forceinline void par_accumulate_scaled(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> accumulated, float scale) {
		assert(std::end(destination) - destination.data() == std::end(accumulated) - accumulated.data());
		execute_parallel<itype>(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_scaled_operator(destination.data(), accumulated.data(), scale));
	}

	namespace ve_impl {
		struct dot_product_operator {
			float const* const a;
			float const* const b;
			fp_vector accumulator[block_repitition] = { fp_vector{}, fp_vector{}, fp_vector{}, fp_vector{} };

			dot_product_operator(float const* va, float const* vb) : a(va), b(vb) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				accumulator[executor.block_index] = ve::multiply_and_add(load(executor, a), load(executor, b), accumulator[executor.block_index]);
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline float dot_product(tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b) {
#ifdef _DEBUG
		assert(std::end(a) - a.data() == std::end(b) - b.data());
#endif

		ve_impl::dot_product_operator op(a.data(), b.data());
		execute_serial<itype>(uint32_t(std::end(a) - a.data()), op);

		return ((op.accumulator[0] + op.accumulator[1]) + (op.accumulator[2] + op.accumulator[3])).reduce();
	}

	namespace ve_impl {
		struct vector_accumulate_product_operator {
			float* const dest;
			float const* const a;
			float const* const b;

			vector_accumulate_product_operator(float* d, float const* va, float const* vb) : dest(d), a(va), b(vb) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				store(executor, dest, ve::multiply_and_add(load(executor, a), load(executor, b), load(executor, dest)));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_product(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(a) - a.data() && std::end(destination) - destination.data() == std::end(b) - b.data());
#endif
		execute_serial_fast<itype>(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_product_operator(destination.data(), a.data(), b.data()));
	}

	namespace ve_impl {
		struct vector_accumulate_scaled_product_operator {
			float* const dest;
			float const* const a;
			float const* const b;
			float const scale;

			vector_accumulate_scaled_product_operator(float* d, float const* va, float const* vb, float s) : dest(d), a(va), b(vb), scale(s) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				store(executor, dest, ve::multiply_and_add(load(executor, a), load(executor, b) * scale, load(executor, dest)));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_scaled_product(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b, float scale) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(a) - a.data() && std::end(destination) - destination.data() == std::end(b) - b.data());
#endif
		execute_serial_fast<itype>(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_scaled_product_operator(destination.data(), a.data(), b.data(), scale));
	}

	namespace ve_impl {
		struct vector_accumulate_sum_operator {
			float* const dest;
			float const* const a;
			float const* const b;

			vector_accumulate_sum_operator(float* d, float const* va, float const* vb) : dest(d), a(va), b(vb) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				store(executor, dest, load(executor, a) + load(executor, b) + load(executor, dest));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_sum(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(a) - a.data() && std::end(destination) - destination.data() == std::end(b) - b.data());
#endif
		execute_serial_fast<itype>(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_sum_operator(destination.data(), a.data(), b.data()));
	}

	namespace ve_impl {
		struct vector_zero_operator {
			float* const dest;
			vector_zero_operator(float* d) : dest(d) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				store(executor, dest, 0.0f);
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void set_zero(tagged_array_view<float, itype, padded> destination) {
		execute_serial_fast<itype>(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_zero_operator(destination.data()));
	}
}