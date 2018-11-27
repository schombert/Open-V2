#include "common\\common.h"
#include "concurrency_tools\\concurrency_tools.hpp"
#include "concurrency_tools\\ve.h"
#include "performance_measurement\\performance.h"
#include <iostream>

constexpr uint32_t vector_length = 704;

constexpr float distance_factor = 0.001f;

class eigen_operation {
public:
	Eigen::Map<Eigen::Array<float, 1, -1>, Eigen::Aligned32> apparent_price_v;
	Eigen::Map<Eigen::Array<float, 1, -1>, Eigen::Aligned32> distance_vector_v;
	Eigen::Map<Eigen::Array<float, 1, -1>, Eigen::Aligned32> state_prices_copy_v;
	Eigen::Map<Eigen::Array<float, 1, -1>, Eigen::Aligned32> tarrif_mask_v;
	float state_owner_tarrifs = 10.0f / 100.0f;

	eigen_operation(float* apparent_price_buffer, float* distance_vector_buffer, float* state_prices_copy_buffer, float* tarrif_mask_buffer) :
		apparent_price_v(apparent_price_buffer, vector_length),
		distance_vector_v(distance_vector_buffer, vector_length),
		state_prices_copy_v(state_prices_copy_buffer, vector_length),
		tarrif_mask_v(tarrif_mask_buffer, vector_length)
	{}

	int test_function() {

		apparent_price_v = distance_vector_v * distance_factor +
			(state_prices_copy_v * (tarrif_mask_v * state_owner_tarrifs + 1.0f));

		return int(apparent_price_v[512]);
	}
};

class ve_operation {
public:
	float* apparent_price_v;
	float* distance_vector_v;
	float* state_prices_copy_v;
	float* tarrif_mask_v;
	float state_owner_tarrifs = 10.0f / 100.0f;

	ve_operation(float* apparent_price_buffer, float* distance_vector_buffer, float* state_prices_copy_buffer, float* tarrif_mask_buffer) :
		apparent_price_v(apparent_price_buffer),
		distance_vector_v(distance_vector_buffer),
		state_prices_copy_v(state_prices_copy_buffer),
		tarrif_mask_v(tarrif_mask_buffer) {}

	int test_function() {
		auto vef = [apparent_price_v = this->apparent_price_v, distance_vector_v = this->distance_vector_v, state_owner_tarrifs = this->state_owner_tarrifs,
			state_prices_copy_v = this->state_prices_copy_v, tarrif_mask_v = this->tarrif_mask_v](auto executor) {

			executor.store(apparent_price_v,
				ve::multiply_and_add(
					executor.load(distance_vector_v),
					executor.constant(distance_factor),
					executor.load(state_prices_copy_v) *
						ve::multiply_and_add(
							executor.load(tarrif_mask_v),
							executor.constant(state_owner_tarrifs),
							executor.constant(1.0f))
				)
			);
		};

		ve::execute_serial_fast(vector_length, vef);

		return int(apparent_price_v[512]);
	}
};

class ve_operation_b {
public:
	float* apparent_price_v;
	float* distance_vector_v;
	float* state_prices_copy_v;
	float* tarrif_mask_v;
	float state_owner_tarrifs = 10.0f / 100.0f;

	ve_operation_b(float* apparent_price_buffer, float* distance_vector_buffer, float* state_prices_copy_buffer, float* tarrif_mask_buffer) :
		apparent_price_v(apparent_price_buffer),
		distance_vector_v(distance_vector_buffer),
		state_prices_copy_v(state_prices_copy_buffer),
		tarrif_mask_v(tarrif_mask_buffer) {}

	int test_function() {
		auto vef = [apparent_price_v = this->apparent_price_v, distance_vector_v = this->distance_vector_v, state_owner_tarrifs = this->state_owner_tarrifs,
			state_prices_copy_v = this->state_prices_copy_v, tarrif_mask_v = this->tarrif_mask_v](auto executor) {

			executor.store(apparent_price_v,
				ve::multiply_and_add(
					executor.load(state_prices_copy_v),
					ve::multiply_and_add(
						executor.load(tarrif_mask_v),
						executor.constant(state_owner_tarrifs),
						executor.constant(1.0f)),
					executor.load(distance_vector_v) *
						executor.constant(distance_factor)
				)
			);
		};

		ve::execute_serial_fast(vector_length, vef);

		return int(apparent_price_v[512]);
	}
};

int main() {
	logging_object log;

	cache_clearer cc;
	
	concurrent_cache_aligned_buffer<float> apparent_price(vector_length);
	concurrent_cache_aligned_buffer<float> distance_vector(vector_length);
	concurrent_cache_aligned_buffer<float> state_prices_copy(vector_length);
	concurrent_cache_aligned_buffer<float> tarrif_mask(vector_length);

	auto fdist = std::uniform_real_distribution<float>(0.0f, 1.0f);
	auto idist = std::uniform_int_distribution<uint32_t>(0ui32, vector_length);

	for(int32_t i = 0; i < int32_t(vector_length); ++i) {
		apparent_price.buffer[i] = fdist(get_local_generator()) + 1.0f;
		distance_vector.buffer[i] = fdist(get_local_generator()) + 1.0f;
		state_prices_copy.buffer[i] = fdist(get_local_generator()) + 1.0f;
		tarrif_mask.buffer[i] = fdist(get_local_generator()) + 1.0f;
	}

	

	std::cout << cc.clear() << std::endl;

	{
		test_object<40, 1000, ve_operation> to(apparent_price.buffer, distance_vector.buffer, state_prices_copy.buffer, tarrif_mask.buffer);
		std::cout << to.log_function(log, "vector engine apparent price update") << std::endl;
	}

	std::cout << cc.clear() << std::endl;

	{
		test_object<40, 1000, eigen_operation> to(apparent_price.buffer, distance_vector.buffer, state_prices_copy.buffer, tarrif_mask.buffer);
		std::cout << to.log_function(log, "eigen apparent price update") << std::endl;
	}
	std::cout << apparent_price.buffer[idist(get_local_generator())] << std::endl;

	std::cout << cc.clear() << std::endl;

	{
		test_object<40, 1000, ve_operation_b> to(apparent_price.buffer, distance_vector.buffer, state_prices_copy.buffer, tarrif_mask.buffer);
		std::cout << to.log_function(log, "vector engine variant B apparent price update") << std::endl;
	}
	std::cout << apparent_price.buffer[idist(get_local_generator())] << std::endl;
}