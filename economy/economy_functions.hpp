#pragma once
#include "common\\common.h"
#include "economy_functions.h"

namespace economy {
	constexpr money_qnty_type round_to_zero_margin = money_qnty_type(0.0001);
	constexpr money_qnty_type stationary_margin = money_qnty_type(0.0001);

	
	inline money_qnty_type perform_cg_step(
		Eigen::Map<Eigen::Matrix<money_qnty_type, 1, -1>, Eigen::Aligned32>& __restrict values,
		Eigen::Map<Eigen::Matrix<money_qnty_type, 1, -1>, Eigen::Aligned32>& __restrict fd_vector,
		Eigen::Map<Eigen::Matrix<money_qnty_type, 1, -1>, Eigen::Aligned32>& __restrict sd_vector,
		uint32_t values_count,
		money_qnty_type scale_factor
	) {

		const auto unadjusted_step_length = fd_vector.sum();
		const auto unadjusted_step_first_derivative = fd_vector.dot(fd_vector);

		//if(unadjusted_step_first_derivative < stationary_margin)
		//	return; // current point is stationary

		const auto derivative_step_ratio = unadjusted_step_first_derivative / unadjusted_step_length;

		money_qnty_type max_selected_eval = std::numeric_limits<money_qnty_type>::lowest();
		uint32_t selected_index = 0;

		for(uint32_t i = 0; i < values_count; ++i) {
			const auto ith_value = fd_vector[i];
			if(ith_value < derivative_step_ratio) {
				// candidate index
				auto candidate_step_length = unadjusted_step_length - ith_value;
				auto replaced_step_limit = values[i]  / candidate_step_length;
				auto new_derivative = unadjusted_step_first_derivative - ith_value * ith_value - candidate_step_length * ith_value;
				if(replaced_step_limit * new_derivative > max_selected_eval) {
					max_selected_eval = replaced_step_limit * new_derivative;
					selected_index = i;
				}
			}
		}

		auto original_first_derivative = fd_vector[selected_index];
		auto original_second_derivative = sd_vector[selected_index];

		fd_vector[selected_index] = 0;
		sd_vector[selected_index] = 0;

		auto step_length = unadjusted_step_length - original_first_derivative;
		auto step_first_derivative = unadjusted_step_first_derivative - original_first_derivative * original_first_derivative - step_length * original_first_derivative;
		auto step_second_derivative = fd_vector.dot(sd_vector) - step_length * original_second_derivative;

		if(step_first_derivative <= 0)
			return 0;
		
		//assert(step_first_derivative > 0);

		fd_vector[selected_index] = -step_length;
		
		auto max_step = money_qnty_type(-1) * (fd_vector.array() < Eigen::Array<money_qnty_type, 1, -1>::Zero(values_count)).select(values.array() / fd_vector.array(), std::numeric_limits<money_qnty_type>::lowest()).maxCoeff();
		//auto min_step = money_qnty_type(-1) * (fd_vector.array() > Eigen::Array<money_qnty_type, 1, -1>::Zero(values_count)).select(values.array() / fd_vector.array(), std::numeric_limits<money_qnty_type>::max()).minCoeff();

		if(step_second_derivative < 0) {
			auto step_multiplier = std::clamp(-step_first_derivative / step_second_derivative, money_qnty_type(0), max_step);
			values += step_multiplier * scale_factor * fd_vector;

			return step_multiplier * step_first_derivative + (step_multiplier) * (step_multiplier) * step_second_derivative / 2.0f;
		} else {
			//auto step_multiplier = std::clamp(step_first_derivative / step_second_derivative, min_step, max_step);
			//values += step_multiplier * fd_vector;

			//auto scale = abs(step_first_derivative / step_second_derivative);
			//if(step_first_derivative >= 0)
				values += max_step * scale_factor * fd_vector;
			//else
			//	values += min_step * fd_vector;

			return max_step * step_first_derivative + (max_step ) * (max_step ) * step_second_derivative / 2.0f;
		}
	}
}