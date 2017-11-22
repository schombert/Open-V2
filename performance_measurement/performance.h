#pragma once
#include <memory>
#include <chrono>

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
	test_object() : base_object() {};

	int log_function(logging_object& log, const char* test_name) {

		int i_sum = 0;
#ifndef _DEBUG
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
#endif
		return i_sum;

	}
};

