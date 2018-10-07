#include "common\\common.h"
#include "concurrency_tools.hpp"

int32_t minimum_index(int32_t const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi32(8);

	__m256i minindices;
	__m256i minvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256i indices = _mm256_setr_epi32(i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7);

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[8];
		int32_t local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0);

		memcpy(local_copy, data + i, sizeof(int32_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		minindices = _mm256_loadu_si256((__m256i*)local_indices);
		minvalues = _mm256_loadu_si256((__m256i*)local_copy);
	} else {
		//aligned case: load initial values with first eight entries
		minindices = indices;
		minvalues = _mm256_loadu_si256((__m256i*)(data + i));
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_epi32(indices, increment);

		const __m256i values = _mm256_loadu_si256((__m256i*)(data + i));
		const __m256i lt = _mm256_cmpgt_epi32(minvalues, values);
		minindices = _mm256_blendv_epi8(minindices, indices, lt);
		minvalues = _mm256_blendv_epi8(minvalues, values, lt);
	}

	int32_t values_array[8];
	int32_t indices_array[8];

	_mm256_storeu_si256((__m256i*)values_array, minvalues);
	_mm256_storeu_si256((__m256i*)indices_array, minindices);

	int32_t minindex = indices_array[7];
	int32_t minvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return minindex;
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256 increment = _mm256_set1_ps(8.0f);

	__m256 minindices;
	__m256 minvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256 indices = _mm256_setr_ps(float(i), float(i + 1), float(i + 2), float(i + 3), float(i + 4), float(i + 5), float(i + 6), float(i + 7));

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		float local_indices[8];
		int32_t local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0.0f);

		memcpy(local_copy, data + i, sizeof(int32_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = float(n + i);
		}

		minindices = _mm256_load_ps(local_indices);
		minvalues = _mm256_cvtepi32_ps(_mm256_loadu_si256((__m256i*)local_copy));
	} else {
		//aligned case: load initial values with first eight entries
		minindices = indices;
		minvalues = _mm256_cvtepi32_ps(_mm256_loadu_si256((__m256i*)(data + i)));
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_ps(indices, increment);

		const __m256 values = _mm256_cvtepi32_ps(_mm256_loadu_si256((__m256i*)(data + i)));
		const __m256 lt = _mm256_cmp_ps(minvalues, values, _CMP_GT_OQ);
		minindices = _mm256_blendv_ps(minindices, indices, lt);
		minvalues = _mm256_blendv_ps(minvalues, values, lt);
	}

	float values_array[8];
	float indices_array[8];

	_mm256_storeu_ps(values_array, minvalues);
	_mm256_storeu_ps(indices_array, minindices);

	float minindex = indices_array[7];
	float minvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return int32_t(minindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi32(4);
	__m128i minindices;
	__m128i minvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m128i indices = _mm_setr_epi32(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[4];
		int32_t local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(int32_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = int32_t(n + i);
		}

		minindices = _mm_loadu_si128((__m128i*)local_indices);
		minvalues = _mm_loadu_si128((__m128i*)local_copy);
	} else {
		//aligned case: load initial values with first four entries
		minindices = indices;
		minvalues = _mm_loadu_si128((__m128i*)(data + i));
	}


	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm_sub_epi32(indices, increment);

		const __m128i values = _mm_loadu_si128((__m128i*)(data + i));
		const __m128i lt = _mm_cmplt_epi32(values, minvalues);
		minindices = _mm_blendv_epi8(minindices, indices, lt);
		minvalues = _mm_blendv_epi8(minvalues, values, lt);
	}

	int32_t values_array[4];
	int32_t indices_array[4];

	_mm_storeu_si128((__m128i*)values_array, minvalues);
	_mm_storeu_si128((__m128i*)indices_array, minindices);

	int32_t minindex = indices_array[3];
	int32_t minvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return minindex;
#endif
#endif
}

int32_t maximum_index(int32_t const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi32(8);

	__m256i maxindices;
	__m256i maxvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256i indices = _mm256_setr_epi32(i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7);

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[8];
		int32_t local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0);

		memcpy(local_copy, data + i, sizeof(int32_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		maxindices = _mm256_loadu_si256((__m256i*)local_indices);
		maxvalues = _mm256_loadu_si256((__m256i*)local_copy);
	} else {
		//aligned case: load initial values with first eight entries
		maxindices = indices;
		maxvalues = _mm256_loadu_si256((__m256i*)(data + i));
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_epi32(indices, increment);

		const __m256i values = _mm256_loadu_si256((__m256i*)(data + i));
		const __m256i lt = _mm256_cmpgt_epi32(values, maxvalues);
		maxindices = _mm256_blendv_epi8(maxindices, indices, lt);
		maxvalues = _mm256_blendv_epi8(maxvalues, values, lt);
	}

	int32_t values_array[8];
	int32_t indices_array[8];

	_mm256_storeu_si256((__m256i*)values_array, maxvalues);
	_mm256_storeu_si256((__m256i*)indices_array, maxindices);

	int32_t maxindex = indices_array[7];
	int32_t maxvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return maxindex;
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256 increment = _mm256_set1_ps(8.0f);

	__m256 maxindices;
	__m256 maxvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256 indices = _mm256_setr_ps(float(i), float(i + 1), float(i + 2), float(i + 3), float(i + 4), float(i + 5), float(i + 6), float(i + 7));

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		float local_indices[8];
		int32_t local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0.0f);

		memcpy(local_copy, data + i, sizeof(int32_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = float(n + i);
		}

		maxindices = _mm256_load_ps(local_indices);
		maxvalues = _mm256_cvtepi32_ps(_mm256_loadu_si256((__m256i*)local_copy));
	} else {
		//aligned case: load initial values with first eight entries
		maxindices = indices;
		maxvalues = _mm256_cvtepi32_ps(_mm256_loadu_si256((__m256i*)(data + i)));
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_ps(indices, increment);

		const __m256 values = _mm256_cvtepi32_ps(_mm256_loadu_si256((__m256i*)(data + i)));
		const __m256 lt = _mm256_cmp_ps(values, maxvalues, _CMP_GT_OQ);
		maxindices = _mm256_blendv_ps(maxindices, indices, lt);
		maxvalues = _mm256_blendv_ps(maxvalues, values, lt);
	}

	float values_array[8];
	float indices_array[8];

	_mm256_storeu_ps(values_array, maxvalues);
	_mm256_storeu_ps(indices_array, maxindices);

	float maxindex = indices_array[7];
	float maxvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return int32_t(maxindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi32(4);
	__m128i maxindices;
	__m128i maxvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m128i indices = _mm_setr_epi32(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[4];
		int32_t local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(int32_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = int32_t(n + i);
		}

		maxindices = _mm_loadu_si128((__m128i*)local_indices);
		maxvalues = _mm_loadu_si128((__m128i*)local_copy);
	} else {
		//aligned case: load initial values with first four entries
		maxindices = indices;
		maxvalues = _mm_loadu_si128((__m128i*)(data + i));
	}


	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm_sub_epi32(indices, increment);

		const __m128i values = _mm_loadu_si128((__m128i*)(data + i));
		const __m128i lt = _mm_cmpgt_epi32(values, maxvalues);
		maxindices = _mm_blendv_epi8(maxindices, indices, lt);
		maxvalues = _mm_blendv_epi8(maxvalues, values, lt);
	}

	int32_t values_array[4];
	int32_t indices_array[4];

	_mm_storeu_si128((__m128i*)values_array, maxvalues);
	_mm_storeu_si128((__m128i*)indices_array, maxindices);

	int32_t maxindex = indices_array[3];
	int32_t maxvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return maxindex;
#endif
#endif
}

int32_t minimum_index(int64_t const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi64x(4);

	__m256i minindices;
	__m256i minvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256i indices = _mm256_setr_epi64x(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[4];
		int64_t local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(int64_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		minindices = _mm256_loadu_si256((__m256i*)local_indices);
		minvalues = _mm256_loadu_si256((__m256i*)local_copy);
	} else {
		//aligned case: load initial values with first four entries
		minindices = indices;
		minvalues = _mm256_loadu_si256((__m256i*)(data + i));
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_epi64(indices, increment);

		const __m256i values = _mm256_loadu_si256((__m256i*)(data + i));
		const __m256i lt = _mm256_cmpgt_epi64(minvalues, values);
		minindices = _mm256_blendv_epi8(minindices, indices, lt);
		minvalues = _mm256_blendv_epi8(minvalues, values, lt);
	}

	int64_t values_array[4];
	int64_t indices_array[4];

	_mm256_storeu_si256((__m256i*)values_array, minvalues);
	_mm256_storeu_si256((__m256i*)indices_array, minindices);

	int64_t minindex = indices_array[3];
	int64_t minvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return int32_t(minindex);
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256d increment = _mm256_set1_pd(4.0f);

	__m256d minindices;
	__m256d minvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256d indices = _mm256_setr_pd(double(i), float(i + 1), float(i + 2), float(i + 3));

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		double local_indices[4];
		int64_t local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0.0);

		memcpy(local_copy, data + i, sizeof(int64_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = double(n + i);
		}

		minindices = _mm256_load_pd(local_indices);
		minvalues = _mm256_setr_pd(double(local_copy[0]), double(local_copy[1]), double(local_copy[2]), double(local_copy[3]));
	} else {
		//aligned case: load initial values with first four entries
		minindices = indices;
		minvalues = _mm256_setr_pd(double(data[i]), double(data[i + 1]), double(data[i + 2]), double(data[i + 3]));
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_pd(indices, increment);

		const __m256d values = _mm256_setr_pd(double(data[i]), double(data[i + 1]), double(data[i + 2]), double(data[i + 3]));
		const __m256d lt = _mm256_cmp_pd(minvalues, values, _CMP_GT_OQ);
		minindices = _mm256_blendv_pd(minindices, indices, lt);
		minvalues = _mm256_blendv_pd(minvalues, values, lt);
	}

	double values_array[4];
	double indices_array[4];

	_mm256_storeu_pd(values_array, minvalues);
	_mm256_storeu_pd(indices_array, minindices);

	double minindex = indices_array[3];
	double minvalue = values_array[3];
	for(int32_t j = 3; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return int32_t(minindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi64x(2);
	__m128i minindices;
	__m128i minvalues;

	int32_t i = int32_t((size - 1) & ~1i64);

	__m128i indices = _mm_setr_epi64x(i, i + 1);

	if(i != int32_t(size) - 2) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[2] = { int64_t(i), 0i64 };
		int64_t local_copy[2] = { data[i], data[0] };

		minindices = _mm_loadu_si128((__m128i*)local_indices);
		minvalues = _mm_loadu_si128((__m128i*)local_copy);
	} else {
		//aligned case: load initial values with first two entries
		minindices = indices;
		minvalues = _mm_loadu_si128((__m128i*)(data + i));
	}

	i -= 2;
	for(; i >= 0; i -= 2) {
		indices = _mm_sub_epi64(indices, increment);

		const __m128i values = _mm_loadu_si128((__m128i*)(data + i));
		const __m128i lt = _mm_cmpgt_epi64(minvalues, values);
		minindices = _mm_blendv_epi8(minindices, indices, lt);
		minvalues = _mm_blendv_epi8(minvalues, values, lt);
	}

	int64_t values_array[2];
	int64_t indices_array[2];

	_mm_storeu_si128((__m128i*)values_array, minvalues);
	_mm_storeu_si128((__m128i*)indices_array, minindices);

	if(values_array[0] < values_array[1])
		return int32_t(indices_array[0]);
	else
		return int32_t(indices_array[1]);
#endif
#endif
}

int32_t maximum_index(int64_t const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi64x(4);

	__m256i maxindices;
	__m256i maxvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256i indices = _mm256_setr_epi64x(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[4];
		int64_t local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(int64_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		maxindices = _mm256_loadu_si256((__m256i*)local_indices);
		maxvalues = _mm256_loadu_si256((__m256i*)local_copy);
	} else {
		//aligned case: load initial values with first four entries
		maxindices = indices;
		maxvalues = _mm256_loadu_si256((__m256i*)(data + i));
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_epi64(indices, increment);

		const __m256i values = _mm256_loadu_si256((__m256i*)(data + i));
		const __m256i lt = _mm256_cmpgt_epi64(values, maxvalues);
		maxindices = _mm256_blendv_epi8(maxindices, indices, lt);
		maxvalues = _mm256_blendv_epi8(maxvalues, values, lt);
	}

	int64_t values_array[4];
	int64_t indices_array[4];

	_mm256_storeu_si256((__m256i*)values_array, maxvalues);
	_mm256_storeu_si256((__m256i*)indices_array, maxindices);

	int64_t maxindex = indices_array[3];
	int64_t maxvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return int32_t(maxindex);
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256d increment = _mm256_set1_pd(4.0f);

	__m256d maxindices;
	__m256d maxvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256d indices = _mm256_setr_pd(double(i), float(i + 1), float(i + 2), float(i + 3));

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		double local_indices[4];
		int64_t local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0.0);

		memcpy(local_copy, data + i, sizeof(int64_t) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = double(n + i);
		}

		maxindices = _mm256_load_pd(local_indices);
		maxvalues = _mm256_setr_pd(double(local_copy[0]), double(local_copy[1]), double(local_copy[2]), double(local_copy[3]));
	} else {
		//aligned case: load initial values with first eight entries
		maxindices = indices;
		maxvalues = _mm256_setr_pd(double(data[i]), double(data[i + 1]), double(data[i + 2]), double(data[i + 3]));
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_pd(indices, increment);

		const __m256d values = _mm256_setr_pd(double(data[i]), double(data[i + 1]), double(data[i + 2]), double(data[i + 3]));
		const __m256d lt = _mm256_cmp_pd(values, maxvalues, _CMP_GT_OQ);
		maxindices = _mm256_blendv_pd(maxindices, indices, lt);
		maxvalues = _mm256_blendv_pd(maxvalues, values, lt);
	}

	double values_array[4];
	double indices_array[4];

	_mm256_storeu_pd(values_array, maxvalues);
	_mm256_storeu_pd(indices_array, maxindices);

	double maxindex = indices_array[3];
	double maxvalue = values_array[3];
	for(int32_t j = 3; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return int32_t(maxindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi64x(2);
	__m128i maxindices;
	__m128i maxvalues;

	int32_t i = int32_t((size - 1) & ~1i64);

	__m128i indices = _mm_setr_epi64x(i, i + 1);

	if(i != int32_t(size) - 2) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[2] = { int64_t(i), 0i64 };
		int64_t local_copy[2] = { data[i], data[0] };

		maxindices = _mm_loadu_si128((__m128i*)local_indices);
		maxvalues = _mm_loadu_si128((__m128i*)local_copy);
	} else {
		//aligned case: load initial values with first four entries
		maxindices = indices;
		maxvalues = _mm_loadu_si128((__m128i*)(data + i));
	}

	i -= 2;
	for(; i >= 0; i -= 2) {
		indices = _mm_sub_epi64(indices, increment);

		const __m128i values = _mm_loadu_si128((__m128i*)(data + i));
		const __m128i lt = _mm_cmpgt_epi64(values, maxvalues);
		maxindices = _mm_blendv_epi8(maxindices, indices, lt);
		maxvalues = _mm_blendv_epi8(maxvalues, values, lt);
	}

	int64_t values_array[2];
	int64_t indices_array[2];

	_mm_storeu_si128((__m128i*)values_array, maxvalues);
	_mm_storeu_si128((__m128i*)indices_array, maxindices);

	if(values_array[0] > values_array[1])
		return int32_t(indices_array[0]);
	else
		return int32_t(indices_array[1]);
#endif
#endif
}

int32_t minimum_index(float const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi32(8);

	__m256i minindices;
	__m256 minvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256i indices = _mm256_setr_epi32(i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7);

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[8];
		float local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0);

		memcpy(local_copy, data + i, sizeof(float) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		minindices = _mm256_loadu_si256((__m256i*)local_indices);
		minvalues = _mm256_loadu_ps(local_copy);
	} else {
		//aligned case: load initial values with first eight entries
		minindices = indices;
		minvalues = _mm256_loadu_ps(data + i);
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_epi32(indices, increment);

		const __m256 values = _mm256_loadu_ps(data + i);
		const __m256 lt = _mm256_cmp_ps(minvalues, values, _CMP_GT_OQ);
		minindices = _mm256_blendv_epi8(minindices, indices, _mm256_castps_si256(lt));
		minvalues = _mm256_blendv_ps(minvalues, values, lt);
	}

	float values_array[8];
	int32_t indices_array[8];

	_mm256_storeu_ps(values_array, minvalues);
	_mm256_storeu_si256((__m256i*)indices_array, minindices);

	int32_t minindex = indices_array[7];
	float minvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return minindex;
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256 increment = _mm256_set1_ps(8.0f);

	__m256 minindices;
	__m256 minvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256 indices = _mm256_setr_ps(float(i), float(i + 1), float(i + 2), float(i + 3), float(i + 4), float(i + 5), float(i + 6), float(i + 7));

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		float local_indices[8];
		float local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0.0f);

		memcpy(local_copy, data + i, sizeof(float) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = float(n + i);
		}

		minindices = _mm256_load_ps(local_indices);
		minvalues = _mm256_load_ps(local_copy);
	} else {
		//aligned case: load initial values with first eight entries
		minindices = indices;
		minvalues = _mm256_load_ps(data + i);
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_ps(indices, increment);

		const __m256 values = _mm256_load_ps(data + i);
		const __m256 lt = _mm256_cmp_ps(minvalues, values, _CMP_GT_OQ);
		minindices = _mm256_blendv_ps(minindices, indices, lt);
		minvalues = _mm256_blendv_ps(minvalues, values, lt);
	}

	float values_array[8];
	float indices_array[8];

	_mm256_storeu_ps(values_array, minvalues);
	_mm256_storeu_ps(indices_array, minindices);

	float minindex = indices_array[7];
	float minvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return int32_t(minindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi32(4);
	__m128i minindices;
	__m128 minvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m128i indices = _mm_setr_epi32(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[4];
		float local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(float) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = int32_t(n + i);
		}

		minindices = _mm_loadu_si128((__m128i*)local_indices);
		minvalues = _mm_loadu_ps(local_copy);
	} else {
		//aligned case: load initial values with first four entries
		minindices = indices;
		minvalues = _mm_loadu_ps(data + i);
	}


	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm_sub_epi32(indices, increment);

		const __m128 values = _mm_loadu_ps(data + i);
		const __m128 lt = _mm_cmplt_ps(values, minvalues);
		minindices = _mm_blendv_epi8(minindices, indices, _mm_castps_si128(lt));
		minvalues = _mm_blendv_ps(minvalues, values, lt);
	}

	float values_array[4];
	int32_t indices_array[4];

	_mm_storeu_ps(values_array, minvalues);
	_mm_storeu_si128((__m128i*)indices_array, minindices);

	int32_t minindex = indices_array[3];
	float minvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return minindex;
#endif
#endif
}

int32_t maximum_index(float const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi32(8);

	__m256i maxindices;
	__m256 maxvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256i indices = _mm256_setr_epi32(i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7);

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[8];
		float local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0);

		memcpy(local_copy, data + i, sizeof(float) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		maxindices = _mm256_loadu_si256((__m256i*)local_indices);
		maxvalues = _mm256_loadu_ps(local_copy);
	} else {
		//aligned case: load initial values with first eight entries
		maxindices = indices;
		maxvalues = _mm256_loadu_ps(data + i);
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_epi32(indices, increment);

		const __m256 values = _mm256_loadu_ps(data + i);
		const __m256 lt = _mm256_cmp_ps(values, maxvalues, _CMP_GT_OQ);
		maxindices = _mm256_blendv_epi8(maxindices, indices, _mm256_castps_si256(lt));
		maxvalues = _mm256_blendv_ps(maxvalues, values, lt);
	}

	float values_array[8];
	int32_t indices_array[8];

	_mm256_storeu_ps(values_array, maxvalues);
	_mm256_storeu_si256((__m256i*)indices_array, maxindices);

	int32_t maxindex = indices_array[7];
	float maxvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return maxindex;
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256 increment = _mm256_set1_ps(8.0f);

	__m256 maxindices;
	__m256 maxvalues;

	int32_t i = int32_t((size - 1) & ~7i64);

	__m256 indices = _mm256_setr_ps(float(i), float(i + 1), float(i + 2), float(i + 3), float(i + 4), float(i + 5), float(i + 6), float(i + 7));

	if(i != int32_t(size) - 8) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		float local_indices[8];
		float local_copy[8];

		std::fill_n(local_copy, 8, data[0]);
		std::fill_n(local_indices, 8, 0.0f);

		memcpy(local_copy, data + i, sizeof(float) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = float(n + i);
		}

		maxindices = _mm256_load_ps(local_indices);
		maxvalues = _mm256_load_ps(local_copy);
	} else {
		//aligned case: load initial values with first eight entries
		maxindices = indices;
		maxvalues = _mm256_load_ps(data + i);
	}

	i -= 8;
	for(; i >= 0; i -= 8) {
		indices = _mm256_sub_ps(indices, increment);

		const __m256 values = _mm256_load_ps(data + i);
		const __m256 lt = _mm256_cmp_ps(values, maxvalues, _CMP_GT_OQ);
		maxindices = _mm256_blendv_ps(maxindices, indices, lt);
		maxvalues = _mm256_blendv_ps(maxvalues, values, lt);
	}

	float values_array[8];
	float indices_array[8];

	_mm256_storeu_ps(values_array, maxvalues);
	_mm256_storeu_ps(indices_array, maxindices);

	float maxindex = indices_array[7];
	float maxvalue = values_array[7];
	for(int32_t j = 6; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return int32_t(maxindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi32(4);
	__m128i maxindices;
	__m128 maxvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m128i indices = _mm_setr_epi32(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int32_t local_indices[4];
		float local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(float) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = int32_t(n + i);
		}

		maxindices = _mm_loadu_si128((__m128i*)local_indices);
		maxvalues = _mm_loadu_ps(local_copy);
	} else {
		//aligned case: load initial values with first four entries
		maxindices = indices;
		maxvalues = _mm_loadu_ps(data + i);
	}


	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm_sub_epi32(indices, increment);

		const __m128 values = _mm_loadu_ps(data + i);
		const __m128 lt = _mm_cmpgt_ps(values, maxvalues);
		maxindices = _mm_blendv_epi8(maxindices, indices, _mm_castps_si128(lt));
		maxvalues = _mm_blendv_ps(maxvalues, values, lt);
	}

	float values_array[4];
	int32_t indices_array[4];

	_mm_storeu_ps(values_array, maxvalues);
	_mm_storeu_si128((__m128i*)indices_array, maxindices);

	int32_t maxindex = indices_array[3];
	float maxvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return maxindex;
#endif
#endif
}

int32_t minimum_index(double const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi64x(4);

	__m256i minindices;
	__m256d minvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256i indices = _mm256_setr_epi64x(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[4];
		double local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(double) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		minindices = _mm256_loadu_si256((__m256i*)local_indices);
		minvalues = _mm256_loadu_pd(local_copy);
	} else {
		//aligned case: load initial values with first four entries
		minindices = indices;
		minvalues = _mm256_loadu_pd(data + i);
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_epi64(indices, increment);

		const __m256d values = _mm256_loadu_pd(data + i);
		const __m256d lt = _mm256_cmp_pd(minvalues, values, _CMP_GT_OQ);
		minindices = _mm256_blendv_epi8(minindices, indices, _mm256_castpd_si256(lt));
		minvalues = _mm256_blendv_pd(minvalues, values, lt);
	}

	double values_array[4];
	int64_t indices_array[4];

	_mm256_storeu_pd(values_array, minvalues);
	_mm256_storeu_si256((__m256i*)indices_array, minindices);

	int64_t minindex = indices_array[3];
	double minvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return int32_t(minindex);
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256d increment = _mm256_set1_pd(4.0);

	__m256d minindices;
	__m256d minvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256d indices = _mm256_setr_pd(double(i), float(i + 1), float(i + 2), float(i + 3));

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		double local_indices[4];
		double local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0.0);

		memcpy(local_copy, data + i, sizeof(double) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = double(n + i);
		}

		minindices = _mm256_load_pd(local_indices);
		minvalues = _mm256_load_pd(local_copy);
	} else {
		//aligned case: load initial values with first four entries
		minindices = indices;
		minvalues = _mm256_load_pd(data + i);
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_pd(indices, increment);

		const __m256d values = _mm256_load_pd(data + i);
		const __m256d lt = _mm256_cmp_pd(minvalues, values, _CMP_GT_OQ);
		minindices = _mm256_blendv_pd(minindices, indices, lt);
		minvalues = _mm256_blendv_pd(minvalues, values, lt);
	}

	double values_array[4];
	double indices_array[4];

	_mm256_storeu_pd(values_array, minvalues);
	_mm256_storeu_pd(indices_array, minindices);

	double minindex = indices_array[3];
	double minvalue = values_array[3];
	for(int32_t j = 3; j >= 0; --j) {
		if(values_array[j] < minvalue) {
			minvalue = values_array[j];
			minindex = indices_array[j];
		}
	}

	return int32_t(minindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi64x(2);
	__m128i minindices;
	__m128d minvalues;

	int32_t i = int32_t((size - 1) & ~1i64);

	__m128i indices = _mm_setr_epi64x(i, i + 1);

	if(i != int32_t(size) - 2) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[2] = { int64_t(i), 0i64 };
		double local_copy[2] = { data[i], data[0] };

		minindices = _mm_loadu_si128((__m128i*)local_indices);
		minvalues = _mm_loadu_pd(local_copy);
	} else {
		//aligned case: load initial values with first two entries
		minindices = indices;
		minvalues = _mm_loadu_pd(data + i);
	}

	i -= 2;
	for(; i >= 0; i -= 2) {
		indices = _mm_sub_epi64(indices, increment);

		const __m128d values = _mm_loadu_pd(data + i);
		const __m128d lt = _mm_cmpgt_pd(minvalues, values);
		minindices = _mm_blendv_epi8(minindices, indices, _mm_castpd_si128(lt));
		minvalues = _mm_blendv_pd(minvalues, values, lt);
	}

	double values_array[2];
	int64_t indices_array[2];

	_mm_storeu_pd(values_array, minvalues);
	_mm_storeu_si128((__m128i*)indices_array, minindices);

	if(values_array[0] < values_array[1])
		return int32_t(indices_array[0]);
	else
		return int32_t(indices_array[1]);
#endif
#endif
}

int32_t maximum_index(double const* data, int32_t size) {
	if(size == 0)
		return 0;

#ifdef __AVX2__
	//AVX2 implementation

	const __m256i increment = _mm256_set1_epi64x(4);

	__m256i maxindices;
	__m256d maxvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256i indices = _mm256_setr_epi64x(i, i + 1, i + 2, i + 3);

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[4];
		double local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0);

		memcpy(local_copy, data + i, sizeof(double) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = n + i;
		}

		maxindices = _mm256_loadu_si256((__m256i*)local_indices);
		maxvalues = _mm256_loadu_pd(local_copy);
	} else {
		//aligned case: load initial values with first four entries
		maxindices = indices;
		maxvalues = _mm256_loadu_pd(data + i);
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_epi64(indices, increment);

		const __m256d values = _mm256_loadu_pd(data + i);
		const __m256d lt = _mm256_cmp_pd(values, maxvalues, _CMP_GT_OQ);
		maxindices = _mm256_blendv_epi8(maxindices, indices, _mm256_castpd_si256(lt));
		maxvalues = _mm256_blendv_pd(maxvalues, values, lt);
	}

	double values_array[4];
	int64_t indices_array[4];

	_mm256_storeu_pd(values_array, maxvalues);
	_mm256_storeu_si256((__m256i*)indices_array, maxindices);

	int64_t maxindex = indices_array[3];
	double maxvalue = values_array[3];
	for(int32_t j = 2; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return int32_t(maxindex);
#else
#ifdef __AVX__
	//AVX implementation
	// for integers: must convert to floating point, possible loss of precision

	const __m256d increment = _mm256_set1_pd(4.0);

	__m256d maxindices;
	__m256d maxvalues;

	int32_t i = int32_t((size - 1) & ~3i64);

	__m256d indices = _mm256_setr_pd(double(i), float(i + 1), float(i + 2), float(i + 3));

	if(i != int32_t(size) - 4) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		double local_indices[4];
		double local_copy[4];

		std::fill_n(local_copy, 4, data[0]);
		std::fill_n(local_indices, 4, 0.0);

		memcpy(local_copy, data + i, sizeof(double) * (size - i));
		for(int32_t n = 0; n + i < size; ++n) {
			local_indices[n] = double(n + i);
		}

		maxindices = _mm256_load_pd(local_indices);
		maxvalues = _mm256_load_pd(local_copy);
	} else {
		//aligned case: load initial values with first four entries
		maxindices = indices;
		maxvalues = _mm256_load_pd(data + i);
	}

	i -= 4;
	for(; i >= 0; i -= 4) {
		indices = _mm256_sub_pd(indices, increment);

		const __m256d values = _mm256_load_pd(data + i);
		const __m256d lt = _mm256_cmp_pd(values, maxvalues, _CMP_GT_OQ);
		maxindices = _mm256_blendv_pd(maxindices, indices, lt);
		maxvalues = _mm256_blendv_pd(maxvalues, values, lt);
	}

	double values_array[4];
	double indices_array[4];

	_mm256_storeu_pd(values_array, maxvalues);
	_mm256_storeu_pd(indices_array, maxindices);

	double maxindex = indices_array[3];
	double maxvalue = values_array[3];
	for(int32_t j = 3; j >= 0; --j) {
		if(values_array[j] > maxvalue) {
			maxvalue = values_array[j];
			maxindex = indices_array[j];
		}
	}

	return int32_t(maxindex);
#else
	//SSE implementation

	const __m128i increment = _mm_set1_epi64x(2);
	__m128i maxindices;
	__m128d maxvalues;

	int32_t i = int32_t((size - 1) & ~1i64);

	__m128i indices = _mm_setr_epi64x(i, i + 1);

	if(i != int32_t(size) - 2) {
		//misaligned case: load initial values with valid entries plus dummy values and indices
		int64_t local_indices[2] = { int64_t(i), 0i64 };
		double local_copy[2] = { data[i], data[0] };

		maxindices = _mm_loadu_si128((__m128i*)local_indices);
		maxvalues = _mm_loadu_pd(local_copy);
	} else {
		//aligned case: load initial values with first two entries
		maxindices = indices;
		maxvalues = _mm_loadu_pd(data + i);
	}

	i -= 2;
	for(; i >= 0; i -= 2) {
		indices = _mm_sub_epi64(indices, increment);

		const __m128d values = _mm_loadu_pd(data + i);
		const __m128d lt = _mm_cmpgt_pd(values, maxvalues);
		maxindices = _mm_blendv_epi8(maxindices, indices, _mm_castpd_si128(lt));
		maxvalues = _mm_blendv_pd(maxvalues, values, lt);
	}

	double values_array[2];
	int64_t indices_array[2];

	_mm_storeu_pd(values_array, maxvalues);
	_mm_storeu_si128((__m128i*)indices_array, maxindices);

	if(values_array[0] > values_array[1])
		return int32_t(indices_array[0]);
	else
		return int32_t(indices_array[1]);
#endif
#endif
}
