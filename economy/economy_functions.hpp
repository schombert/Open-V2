#pragma once
#include "common\\common.h"
#include "economy_functions.h"

namespace economy {
	constexpr money_qnty_type round_to_zero_margin = money_qnty_type(0.0001);

	template<typename F>
	void perform_cg_step(
		//Eigen::Map<Eigen::Matrix<money_qnty_type, 1, -1>, Eigen::Aligned32>& __restrict values,
		Eigen::Map<Eigen::Matrix<money_qnty_type, 1, -1>>& __restrict values,
		uint32_t values_count,
		F const& derivative_functions
	) {
		std::vector<money_qnty_type, concurrent_allocator<money_qnty_type>> first_derivatives;
		std::vector<money_qnty_type, concurrent_allocator<money_qnty_type>> second_derivatives;

		first_derivatives.resize(values_count);
		second_derivatives.resize(values_count);

		int32_t greatest_value = 0;

		for(uint32_t i = 0; i < values_count; ++i) {
			std::tie(first_derivatives[i], second_derivatives[i]) = derivative_functions(i);
			if(values[i] < round_to_zero_margin && first_derivatives[i] < 0) {
				first_derivatives[i] = 0;
				second_derivatives[i] = 0;
			}
			//if(values[i] > values[greatest_value])
			//	greatest_value = i;
			if( (values[greatest_value] <= round_to_zero_margin || (values[i] + 0.0f) * first_derivatives[i] < (values[greatest_value] + 0.0f) * first_derivatives[greatest_value]) && values[i] > round_to_zero_margin)
				greatest_value = i;
		}

		auto original_first_derivative = first_derivatives[greatest_value];
		auto original_second_derivative = second_derivatives[greatest_value];

		first_derivatives[greatest_value] = 0;
		second_derivatives[greatest_value] = 0;

		Eigen::Map<Eigen::Matrix<money_qnty_type, 1, -1>> fd_vector(first_derivatives.data(), values_count);
		Eigen::Map<Eigen::Matrix<money_qnty_type, 1, -1>> sd_vector(second_derivatives.data(), values_count);

		auto step_length = fd_vector.sum();

		auto step_first_derivative = fd_vector.dot(fd_vector) - step_length * original_first_derivative;
		auto step_second_derivative = fd_vector.dot(sd_vector) - step_length * original_second_derivative;

		fd_vector[greatest_value] = -step_length;

		auto max_step = money_qnty_type(-1) * (fd_vector.array() < Eigen::Array<money_qnty_type, 1, -1>::Zero(values_count)).select(values.array() / fd_vector.array(), std::numeric_limits<money_qnty_type>::lowest()).maxCoeff();
		auto min_step = money_qnty_type(-1) * (fd_vector.array() > Eigen::Array<money_qnty_type, 1, -1>::Zero(values_count)).select(values.array() / fd_vector.array(), std::numeric_limits<money_qnty_type>::max()).minCoeff();

		if(step_second_derivative < 0) {
			auto step_multiplier = std::clamp(-step_first_derivative / step_second_derivative, min_step, max_step);
			values += step_multiplier * fd_vector;
		} else {
			//auto step_multiplier = std::clamp(step_first_derivative / step_second_derivative, min_step, max_step);
			//values += step_multiplier * fd_vector;

			//auto scale = abs(step_first_derivative / step_second_derivative);
			if(step_first_derivative >= 0)
				values += max_step * fd_vector;
			else
				values += min_step * fd_vector;
		}
	}
}