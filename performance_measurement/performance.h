#pragma once
#include "common\\common.h"
#include <memory>
#include <chrono>
#include "concurrency_tools\\concurrency_tools.hpp"
#include "concurrency_tools\\ve.h"
#include <random>

class cache_clearer {
public:
	std::vector<float, aligned_allocator_64<float>> a;
	std::vector<float, aligned_allocator_64<float>> b;

	cache_clearer() : a(1024 * 1024 * 8,0.0f), b(1024 * 1024 * 8, 0.0f) {
		clear();
	}

	float clear() {
		auto idist = std::uniform_int_distribution<int32_t>(0, 1024 * 1024 * 8);
		auto fdist = std::uniform_real_distribution<float>(0.0f, 1.0f);

		for(int32_t i = 0; i < 1024; ++i) {
			a[idist(get_local_generator())] = fdist(get_local_generator());
			b[idist(get_local_generator())] = fdist(get_local_generator());
		}

		auto cache_shuffle = [a_vec = a.data(), b_vec = b.data(), dist = std::uniform_real_distribution<float>(0.0f, 1.0f)](auto executor) {
			auto rval = dist(get_local_generator());
			auto av = executor.load(a_vec);
			auto bv = executor.load(b_vec);
			executor.store(a_vec, (av + bv + executor.constant(rval)) * executor.constant(0.5f));
			executor.store(b_vec, av - bv - executor.constant(rval));
		};

		ve::execute_parallel(1024 * 1024 * 8, cache_shuffle);

		return a[idist(get_local_generator())];
	}
};

class intermediate_results {
public:
	uint32_t number_measurements = 0;
	uint64_t minimum_microseconds = 0;
	uint64_t maximum_microseconds = 0;
	uint64_t total_microseconds = 0;

	void add_measurement(uint64_t microseconds) {
		if (number_measurements == 0) {
			minimum_microseconds = microseconds;
			maximum_microseconds = microseconds;
		} else {
			minimum_microseconds = microseconds < minimum_microseconds ? microseconds : minimum_microseconds;
			maximum_microseconds = microseconds > maximum_microseconds ? microseconds : maximum_microseconds;
		}
		total_microseconds += microseconds;
		++number_measurements;
	}
};

class private_db_type;

class logging_object {
private:
	std::unique_ptr<private_db_type> private_db;
public:
	logging_object();
	~logging_object();
	void log_results(const intermediate_results& results, uint32_t outer_loops, uint32_t inner_loops, const char* test_name);
};

template<uint32_t outer_loops, uint32_t inner_loops, typename base_object>
class test_object : public base_object {
public:
	template<typename ... PARAMS>
	test_object(PARAMS&& ... p) : base_object(std::forward<PARAMS>(p)...) {}

#ifndef _DEBUG
	int log_function(logging_object& log, const char* test_name) {

		int i_sum = 0;

		for (uint32_t a = 0; a < 3; ++a) {
			for (uint32_t i = 0; i < inner_loops; ++i) {
				i_sum += base_object::test_function();
			}
		}

		intermediate_results results;

		for (uint32_t b = 0; b < outer_loops; ++b) {
			auto start_time = std::chrono::high_resolution_clock::now();
			for (uint32_t i = 0; i < inner_loops; ++i) {
				i_sum += base_object::test_function();
			}
			auto end_time = std::chrono::high_resolution_clock::now();
			results.add_measurement(std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());
		}

		log.log_results(results, outer_loops, inner_loops, test_name);

		return i_sum;
	}
#else
	int log_function(logging_object&, const char*) {
		return 0;
	}
#endif
};

