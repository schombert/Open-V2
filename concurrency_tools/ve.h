#pragma once
#include "common\\common.h"
#include <intrin.h>
#include <ppl.h>

namespace ve {

#ifdef __AVX2__
#include "ve_avx2.h"
#else
#ifdef __AVX__
#include "ve_avx.h"
#else // SSE
#include "ve_sse.h"
#endif
#endif

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

	template<typename T>
	__forceinline auto improved_inverse(T a) {
		auto i = inverse(a);
		//return  -(a * i * i) + (i + i);
		return negate_multiply_and_add(a, i * i, i + i);
	}

	class single_value_operation {
	protected:
		uint32_t const offset;
	public:
		single_value_operation(uint32_t o) : offset(o) {}

		constexpr static int32_t block_index = 0;

		__forceinline float zero() {
			return 0.0f;
		}
		__forceinline int32_t int_zero() {
			return 0;
		}
		__forceinline float constant(float v) {
			return v;
		}
		__forceinline int32_t int_constant(int32_t v) {
			return v;
		}

		__forceinline float load(float const* source) {
			return source[offset];
		}
		__forceinline int32_t load(int32_t const* source) {
			return source[offset];
		}
		__forceinline float unaligned_load(float const* source) {
			return source[offset];
		}
		__forceinline int32_t unaligned_load(int32_t const* source) {
			return source[offset];
		}

		__forceinline float gather_load(float const* source, int32_t indices) {
			return source[indices];
		}
		__forceinline int32_t gather_load(int32_t const* source, int32_t indices) {
			return source[indices];
		}
		__forceinline float gather_masked_load(float const* source, int32_t indices, bool mask, float def = 0.0f) {
			return mask ? source[indices] : def;
		}
		__forceinline int32_t gather_masked_load(int32_t const* source, int32_t indices, bool mask, int32_t def = 0) {
			return mask ? source[indices] : def;
		}

		__forceinline void store(float* dest, float value) {
			dest[offset] = value;
		}
		__forceinline void unaligned_store(float* dest, float value) {
			dest[offset] = value;
		}

		__forceinline float stream_load(float const* source) {
			return source[offset];
		}
		__forceinline int32_t stream_load(int32_t const* source) {
			return source[offset];
		}
		__forceinline void stream_store(float* dest, float value) {
			dest[offset] = value;
		}

		template<typename F>
		__forceinline float apply(F const& f, float arg) {
			return f(arg, offset);
		}

		template<typename F>
		__forceinline int32_t apply(F const& f, int32_t arg) {
			return f(arg, offset);
		}

		__forceinline float partial_load(float const* source) {
			return source[offset];
		}
		__forceinline int32_t partial_load(int32_t const* source) {
			return source[offset];;
		}
		__forceinline void partial_store(float* dest, float value) {
			dest[offset] = value;
		}
		__forceinline bool partial_mask() {
			return true;
		}
	};

	constexpr int32_t block_repitition = 4;

	template<typename F>
	__forceinline void execute_serial_fast(uint32_t count, F&& functor) {
		const uint32_t full_units = (count + uint32_t(vector_size - 1)) & ~uint32_t(vector_size - 1);
		const uint32_t reps = full_units / vector_size;
		const uint32_t quad_reps = reps / uint32_t(block_repitition);

		int32_t i = 0;
		for(; i < int32_t(quad_reps); ++i) {
			functor(full_vector_operation<0>(i * (vector_size * 4)));
			functor(full_vector_operation<1>(i * (vector_size * 4) + vector_size));
			functor(full_vector_operation<2>(i * (vector_size * 4) + vector_size * 2));
			functor(full_vector_operation<3>(i * (vector_size * 4) + vector_size * 3));
		}
		if((reps & 3) == 1) {
			functor(full_vector_operation<1>(i * (vector_size * 4)));
		} else if((reps & 3) == 2) {
			functor(full_vector_operation<0>(i * (vector_size * 4)));
			functor(full_vector_operation<1>(i * (vector_size * 4) + vector_size));
		} else if((reps & 3) == 3) {
			functor(full_vector_operation<1>(i * (vector_size * 4)));
			functor(full_vector_operation<2>(i * (vector_size * 4) + vector_size));
			functor(full_vector_operation<3>(i * (vector_size * 4) + vector_size * 2));
		}
	}

	template<typename F>
	__forceinline void execute_serial(uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count - full_units;

		execute_serial_fast(full_units, std::forward<F>(functor));

		if(remainder != 0) {
			functor(partial_vector_operation(full_units, remainder));
		}
	}

	template<typename F>
	__forceinline void execute_serial_unaligned(uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count - full_units;

		const uint32_t reps = full_units / vector_size;
		const uint32_t quad_reps = reps / uint32_t(block_repitition);

		int32_t i = 0;
		for(; i < int32_t(quad_reps); ++i) {
			functor(full_unaligned_vector_operation<0>(i * (vector_size * 4)));
			functor(full_unaligned_vector_operation<1>(i * (vector_size * 4) + vector_size));
			functor(full_unaligned_vector_operation<2>(i * (vector_size * 4) + vector_size * 2));
			functor(full_unaligned_vector_operation<3>(i * (vector_size * 4) + vector_size * 3));
		}
		if((reps & 3) == 1) {
			functor(full_unaligned_vector_operation<1>(i * (vector_size * 4)));
		} else if((reps & 3) == 2) {
			functor(full_unaligned_vector_operation<0>(i * (vector_size * 4)));
			functor(full_unaligned_vector_operation<1>(i * (vector_size * 4) + vector_size));
		} else if((reps & 3) == 3) {
			functor(full_unaligned_vector_operation<1>(i * (vector_size * 4)));
			functor(full_unaligned_vector_operation<2>(i * (vector_size * 4) + vector_size));
			functor(full_unaligned_vector_operation<3>(i * (vector_size * 4) + vector_size * 2));
		}

		if(remainder != 0) {
			functor(partial_unaligned_vector_operation(full_units, remainder));
		}
	}

	template<typename index_type, typename F>
	__forceinline void execute_single(index_type offset, F&& functor) {
		functor(single_value_operation(uint32_t(to_index(offset))));
	}

	template<typename F>
	__forceinline void execute_subsequence(uint32_t start, uint32_t end, F&& functor) {
		const auto count = start - end;
		const uint32_t full_units = count & ~uint32_t(vector_size - 1);
		const uint32_t remainder = count - full_units;

		for(int32_t i = 0; i < int32_t(full_units); i += vector_size) {
			functor(full_unaligned_vector_operation(start + i));
		}
		if(remainder != 0) {
			functor(partial_unaligned_vector_operation(full_units + start, remainder));
		}
	}

	template<typename F>
	void execute_parallel(uint32_t count, F&& functor) {
		const uint32_t full_units = (count + 15ui32) & ~15ui32;
		concurrency::parallel_for(0ui32, full_units, 16ui32, [&functor](uint32_t offset) {
			if constexpr(vector_size == 16) {
				functor(full_vector_operation<0>(offset));
			} else if constexpr(vector_size == 8) {
				functor(full_vector_operation<0>(offset));
				functor(full_vector_operation<1>(offset + 8));
			} else if constexpr(vector_size == 4) {
				functor(full_vector_operation<0>(offset));
				functor(full_vector_operation<1>(offset + 4));
				functor(full_vector_operation<2>(offset + 8));
				functor(full_vector_operation<3>(offset + 12));
			} else {
				static_assert(vector_size == 16 || vector_size == 8 || vector_size == 4);
			}
		}, concurrency::static_partitioner());
	}

	template<typename F>
	void execute_parallel_exact(uint32_t count, F&& functor) {
		const uint32_t full_units = count & ~15ui32;
		const uint32_t remainder = count - full_units;
		concurrency::parallel_for(0ui32, full_units, 16ui32, [&functor](uint32_t offset) {
			if constexpr(vector_size == 16) {
				functor(full_vector_operation<0>(offset));
			} else if constexpr(vector_size == 8) {
				functor(full_vector_operation<0>(offset));
				functor(full_vector_operation<1>(offset + 8));
			} else if constexpr(vector_size == 4) {
				functor(full_vector_operation<0>(offset));
				functor(full_vector_operation<1>(offset + 4));
				functor(full_vector_operation<2>(offset + 8));
				functor(full_vector_operation<3>(offset + 12));
			} else {
				static_assert(false);
			}
		}, concurrency::static_partitioner());
		if constexpr(vector_size == 16) {
			if(remainder != 0) {
				functor(partial_vector_operation(full_units, remainder));
			}
		} else if constexpr(vector_size == 8) {
			if(remainder > 8ui32) {
				functor(full_vector_operation<1>(full_units));
				functor(partial_vector_operation(full_units + 8ui32, remainder - 8ui32));
			} else if(remainder == 8ui32) {
				functor(full_vector_operation<1>(full_units));
			} else if(remainder != 0) {
				functor(partial_vector_operation(full_units, remainder));
			}
		} else if constexpr(vector_size == 4) {
			if(remainder > 12ui32) {
				functor(full_vector_operation<1>(full_units));
				functor(full_vector_operation<2>(full_units + 4ui32));
				functor(full_vector_operation<3>(full_units + 8ui32));
				functor(partial_vector_operation(full_units + 12ui32, remainder - 12ui32));
			} else if(remainder == 12ui32) {
				functor(full_vector_operation<1>(full_units));
				functor(full_vector_operation<2>(full_units + 4ui32));
				functor(full_vector_operation<3>(full_units + 8ui32));
			} else if(remainder > 8ui32) {
				functor(full_vector_operation<0>(full_units));
				functor(full_vector_operation<1>(full_units + 4ui32));
				functor(partial_vector_operation(full_units + 8ui32, remainder - 8ui32));
			} else if(remainder == 8ui32) {
				functor(full_vector_operation<0>(full_units));
				functor(full_vector_operation<1>(full_units + 4ui32));
			} else if(remainder > 4ui32) {
				functor(full_vector_operation<1>(full_units));
				functor(partial_vector_operation(full_units + 4ui32, remainder - 4ui32));
			} else if(remainder == 4ui32) {
				functor(full_vector_operation<1>(full_units));
			} else if(remainder != 0) {
				functor(partial_vector_operation(full_units, remainder));
			}
		}
		
	}

	namespace ve_impl {
		struct reduce_operator {
			float const* const vector;
			fp_vector accumulator[block_repitition] = { fp_vector{}, fp_vector{}, fp_vector{}, fp_vector{} };

			reduce_operator(float const* v) : vector(v) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				accumulator[executor.block_index] = accumulator[executor.block_index] +executor.load(vector);
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline float reduce(tagged_array_view<const float, itype, padded> vector) {
		ve_impl::reduce_operator ro(vector.data());
		execute_serial(uint32_t(std::end(vector) - vector.data()), ro);
		return ((ro.accumulator[0] + ro.accumulator[1]) + (ro.accumulator[2] + ro.accumulator[3])).reduce();
	}

	namespace ve_impl {
		struct rescale_operator {
			float* const vector;
			float const scale;

			rescale_operator(float* v, float s) : vector(v), scale(s) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				executor.store(vector, executor.constant(scale) * executor.load(vector));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void rescale(tagged_array_view<float, itype, padded> vector, float scale_factor) {
		execute_serial_fast(uint32_t(std::end(vector) - vector.data()), ve_impl::rescale_operator(vector.data(), scale_factor));
	}

	namespace ve_impl {
		struct vector_accumulate_operator {
			float* const dest;
			float const* const accumulated;

			vector_accumulate_operator(float* d, float const* a) : dest(d), accumulated(a) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				executor.store(dest, executor.load(accumulated) + executor.load(dest));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> accumulated) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(accumulated) - accumulated.data());
#endif
		execute_serial_fast(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_operator(destination.data(), accumulated.data()));
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_exact(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> accumulated) {
		execute_serial_unaligned(std::min(uint32_t(std::end(destination) - destination.data()), uint32_t(std::end(accumulated) - accumulated.data())),
			ve_impl::vector_accumulate_operator(destination.data(), accumulated.data()));
	}

	namespace ve_impl {
		struct vector_accumulate_scaled_operator {
			float* const dest;
			float const* const accumulated;
			float const scale;

			vector_accumulate_scaled_operator(float* d, float const* a, float s) : dest(d), accumulated(a), scale(s) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				executor.store(dest, ve::multiply_and_add(executor.constant(scale), executor.load(accumulated), executor.load(dest)));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_scaled(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> accumulated, float scale) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(accumulated) - accumulated.data());
#endif
		execute_serial_fast(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_scaled_operator(destination.data(), accumulated.data(), scale));
	}

	namespace ve_impl {
		struct dot_product_operator {
			float const* const a;
			float const* const b;
			fp_vector accumulator[block_repitition] = { fp_vector{}, fp_vector{}, fp_vector{}, fp_vector{} };

			dot_product_operator(float const* va, float const* vb) : a(va), b(vb) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				accumulator[executor.block_index] = ve::multiply_and_add(executor.load(a), executor.load(b), accumulator[executor.block_index]);
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline float dot_product(tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b) {
#ifdef _DEBUG
		assert(std::end(a) - a.data() == std::end(b) - b.data());
#endif

		ve_impl::dot_product_operator op(a.data(), b.data());
		execute_serial(uint32_t(std::end(a) - a.data()), op);

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
				executor.store(dest, ve::multiply_and_add(executor.load(a), executor.load(b), executor.load(dest)));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_product(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(a) - a.data() && std::end(destination) - destination.data() == std::end(b) - b.data());
#endif
		execute_serial_fast(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_product_operator(destination.data(), a.data(), b.data()));
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
				executor.store(dest, ve::multiply_and_add(executor.load(a), executor.load(b) * executor.constant(scale), executor.load(dest)));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_scaled_product(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b, float scale) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(a) - a.data() && std::end(destination) - destination.data() == std::end(b) - b.data());
#endif
		execute_serial_fast(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_scaled_product_operator(destination.data(), a.data(), b.data(), scale));
	}

	namespace ve_impl {
		struct vector_accumulate_sum_operator {
			float* const dest;
			float const* const a;
			float const* const b;

			vector_accumulate_sum_operator(float* d, float const* va, float const* vb) : dest(d), a(va), b(vb) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				executor.store(dest, executor.load(a) + executor.load(b) + executor.load(dest));
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void accumulate_sum(tagged_array_view<float, itype, padded> destination, tagged_array_view<const float, itype, padded> a, tagged_array_view<const float, itype, padded> b) {
#ifdef _DEBUG
		assert(std::end(destination) - destination.data() == std::end(a) - a.data() && std::end(destination) - destination.data() == std::end(b) - b.data());
#endif
		execute_serial_fast(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_accumulate_sum_operator(destination.data(), a.data(), b.data()));
	}

	namespace ve_impl {
		struct vector_zero_operator {
			float* const dest;
			vector_zero_operator(float* d) : dest(d) {};

			template<typename T>
			__forceinline void operator()(T executor) {
				executor.store(dest, executor.zero());
			}
		};
	}

	template<typename itype, bool padded>
	__forceinline void set_zero(tagged_array_view<float, itype, padded> destination) {
		execute_serial_fast(uint32_t(std::end(destination) - destination.data()), ve_impl::vector_zero_operator(destination.data()));
	}
}