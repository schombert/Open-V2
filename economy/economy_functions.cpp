#include "common\\common.h"
#include "economy_functions.h"
#include "world_state\\world_state.h"
#include "provinces\\province_functions.hpp"
#include "nations\\nations_functions.hpp"
#include "population\\population_function.h"
#include "nations\\nations_functions.hpp"
#include "provinces\\province_functions.hpp"
#include <random>

#ifdef DEBUG_ECONOMY
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#undef small
#endif

namespace economy {
	void init_economic_scenario(world_state& ws) {
		//ws.w.economy_s.current_prices.resize(ws.s.economy_m.aligned_32_goods_count);
		//for(auto& g : ws.s.economy_m.goods) {
		//	ws.w.economy_s.current_prices[g.id] = g.base_price;
		//}
		//if(ws.s.economy_m.aligned_32_goods_count != 0)
		//	ws.w.economy_s.current_prices[economy::money_good] = economy::money_qnty_type(1);
#ifdef DEBUG_ECONOMY
		ws.w.economy_s.world_rgo_production.resize(ws.s.economy_m.aligned_32_goods_count);
		ws.w.economy_s.world_other_production.resize(ws.s.economy_m.aligned_32_goods_count);
		ws.w.economy_s.world_pop_consumption.resize(ws.s.economy_m.aligned_32_goods_count);
		ws.w.economy_s.world_other_consumption.resize(ws.s.economy_m.aligned_32_goods_count);

		_clearfp();
		_controlfp_s(nullptr, 0, _EM_INVALID | _EM_OVERFLOW | _EM_ZERODIVIDE);
#endif
	}

	void reset_state(economic_state&) {}

	production_modifiers rgo_production_modifiers(
		world_state const& ws,
		worked_instance const& instance,
		workers_information const& workers_info,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		int32_t rgo_base_size,
		economy::goods_tag production,
		float mobilization_effect) {

		production_modifiers result;

		result.input_modifier = 1.0f;
		result.output_modifier = 1.0f;
		result.throughput_modifier = 0.0f;

		bool is_mined = (ws.s.economy_m.goods[production].flags & economy::good_definition::mined) != 0;

		float total_workforce = float(workers_info.workforce * rgo_base_size) * instance.production_scale *
			((is_mined ?
				in_province.modifier_values[modifiers::provincial_offsets::mine_rgo_size] + in_nation.modifier_values[modifiers::national_offsets::mine_rgo_size] :
				in_province.modifier_values[modifiers::provincial_offsets::farm_rgo_size] + in_nation.modifier_values[modifiers::national_offsets::farm_rgo_size]) +
				ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_size>(production)) +
				1.0f);

		if(total_workforce == 0.0f)
			return result;

		for(uint32_t i = 0; i < max_worker_types; ++i) {
			if(workers_info.workers[i].contribution == contribution_type::output) {
				result.output_modifier += workers_info.workers[i].effect_multiplier * float(instance.worker_populations[i]) / total_workforce;
			} else if(workers_info.workers[i].contribution == contribution_type::throughput) {
				result.throughput_modifier += workers_info.workers[i].effect_multiplier * float(instance.worker_populations[i]) / (workers_info.workforce * instance.production_scale);
			}
		}

		float total_pop = float(ws.w.nation_s.state_demographics.get(in_province.state_instance->id, population::total_population_tag));
		float owner_effect = total_pop != 0.0f ? workers_info.owner.effect_multiplier * (ws.w.nation_s.state_demographics.get(in_province.state_instance->id, population::to_demo_tag(ws, workers_info.owner.type))) / total_pop : 0.0f;

		result.output_modifier *= std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_rgo_output] +
			in_nation.modifier_values[modifiers::national_offsets::rgo_output] +
			(is_mined ? in_province.modifier_values[modifiers::provincial_offsets::farm_rgo_eff] : in_province.modifier_values[modifiers::provincial_offsets::mine_rgo_eff]) +
			(workers_info.owner.contribution == contribution_type::output ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_goods_output>(production)) +
			1.0f));
		result.throughput_modifier *= mobilization_effect * std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_rgo_throughput] +
			in_nation.modifier_values[modifiers::national_offsets::rgo_throughput] +
			(workers_info.owner.contribution == contribution_type::throughput ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_goods_throughput>(production)) +
			1.0f));

		return result;
	}

	production_modifiers factory_production_modifiers(
		world_state const& ws,
		worked_instance const& instance,
		bonus* bonuses,
		workers_information const& workers_info,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		int32_t base_size,
		economy::goods_tag production,
		float mobilization_effect) {

		production_modifiers result;

		result.input_modifier = 1.0f;
		result.output_modifier = 1.0f;
		result.throughput_modifier = 0.0f;

		float total_workforce = float(workers_info.workforce * base_size) * instance.production_scale;

		if(total_workforce != 0) {
			for(uint32_t i = 0; i < max_worker_types; ++i) {
				if(workers_info.workers[i].contribution == contribution_type::input) {
					result.input_modifier += workers_info.workers[i].effect_multiplier * float(instance.worker_populations[i]) / total_workforce;
				} else if(workers_info.workers[i].contribution == contribution_type::output) {
					result.output_modifier += workers_info.workers[i].effect_multiplier * float(instance.worker_populations[i]) / total_workforce;
				} else if(workers_info.workers[i].contribution == contribution_type::throughput) {
					result.throughput_modifier += workers_info.workers[i].effect_multiplier * float(instance.worker_populations[i]) / (workers_info.workforce * instance.production_scale);
				}
			}
		}

		float total_pop = float(ws.w.nation_s.state_demographics.get(in_province.state_instance->id, population::total_population_tag));
		float owner_effect = total_pop != 0.0f ? workers_info.owner.effect_multiplier * (ws.w.nation_s.state_demographics.get(in_province.state_instance->id, population::to_demo_tag(ws, workers_info.owner.type))) / total_pop : 0.0f;

		result.input_modifier *= std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_factory_input] +
			in_nation.modifier_values[modifiers::national_offsets::factory_input] +
			(workers_info.owner.contribution == contribution_type::input ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::factory_goods_input>(production)) +
			1.0f));
		result.output_modifier *= std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_factory_output] +
			in_nation.modifier_values[modifiers::national_offsets::factory_output] +
			(workers_info.owner.contribution == contribution_type::output ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::factory_goods_output>(production)) +
			1.0f));
		result.throughput_modifier *= mobilization_effect * std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_factory_throughput] +
			in_nation.modifier_values[modifiers::national_offsets::factory_throughput] +
			(workers_info.owner.contribution == contribution_type::throughput ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::factory_goods_throughput>(production)) +
			1.0f));

		float bonus_sum = 1.0f;
		for(uint32_t i = 0; i < std::extent_v<decltype(std::declval<factory_type>().bonuses)>; ++i) {
			if(is_valid_index(bonuses[i].condition) && triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(bonuses[i].condition), ws, in_province.state_instance, in_province.state_instance, nullptr, nullptr)) {
				bonus_sum += bonuses[i].value;
			}
		}

		result.throughput_modifier *= bonus_sum;

		return result;
	}


	production_modifiers artisan_production_modifiers(
		world_state const& ws,
		nations::nation const& in_nation,
		provinces::province_state const& in_province,
		economy::goods_tag production,
		float mobilization_effect) {

		production_modifiers result;

		result.input_modifier = 1.0f;
		result.output_modifier = 1.0f;
		result.throughput_modifier = 1.0f;


		result.input_modifier *= std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_artisan_input] +
			in_nation.modifier_values[modifiers::national_offsets::artisan_input] +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::artisan_goods_input>(production)) +
			1.0f));
		result.output_modifier *= std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_artisan_output] +
			in_nation.modifier_values[modifiers::national_offsets::artisan_output] +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::artisan_goods_output>(production)) +
			1.0f));
		result.throughput_modifier *= mobilization_effect * std::max(0.0f, (
			in_province.modifier_values[modifiers::provincial_offsets::local_artisan_throughput] +
			in_nation.modifier_values[modifiers::national_offsets::artisan_throughput] +
			ws.w.nation_s.production_adjustments.get(in_nation.id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::artisan_goods_throughput>(production)) +
			1.0f));

		return result;
	}

	int32_t rgo_max_employment(world_state const& ws, provinces::province_state& ps) {
		auto production = ps.rgo_production;
		if(!is_valid_index(production)) return 1;

		auto owner = ps.owner;
		if(!owner) return 0;

		auto owner_id = owner->id;
		if(!ws.w.nation_s.nations.is_valid_index(owner_id)) return 0;

		bool is_mined = (ws.s.economy_m.goods[production].flags & economy::good_definition::mined) != 0;
		float total_workforce = float((is_mined ? ws.s.economy_m.rgo_mine.workforce : ws.s.economy_m.rgo_farm.workforce) * int32_t(ps.rgo_size)) * ps.rgo_worker_data.production_scale *
			((is_mined ?
				ps.modifier_values[modifiers::provincial_offsets::mine_rgo_size] + owner->modifier_values[modifiers::national_offsets::mine_rgo_size] :
				ps.modifier_values[modifiers::provincial_offsets::farm_rgo_size] + owner->modifier_values[modifiers::national_offsets::farm_rgo_size]) +
				ws.w.nation_s.production_adjustments.get(owner_id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_size>(production)) +
				1.0f);
		return std::max(0, int32_t(total_workforce));
	}

	void match_rgo_worker_type(world_state& ws, provinces::province_state& ps) {
		bool is_mined = (ws.s.economy_m.goods[ps.rgo_production].flags & economy::good_definition::mined) != 0;

		if(is_mined) {
			provinces::for_each_pop(ws, ps, [&ws](population::pop& p) {
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
					if(p.type == ws.s.economy_m.rgo_farm.workers[i].type && is_valid_index(ws.s.economy_m.rgo_mine.workers[i].type) && p.type != ws.s.economy_m.rgo_mine.workers[i].type)
						population::change_pop_type(ws, p, ws.s.economy_m.rgo_mine.workers[i].type);
				}
			});
		} else {
			provinces::for_each_pop(ws, ps, [&ws](population::pop& p) {
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
					if(p.type == ws.s.economy_m.rgo_mine.workers[i].type && is_valid_index(ws.s.economy_m.rgo_farm.workers[i].type) && p.type != ws.s.economy_m.rgo_farm.workers[i].type)
						population::change_pop_type(ws, p, ws.s.economy_m.rgo_farm.workers[i].type);
				}
			});
		}
	}

	void update_rgo_employment(world_state& ws, provinces::province_state& ps) {
		bool is_mined = (ws.s.economy_m.goods[ps.rgo_production].flags & economy::good_definition::mined) != 0;

		if(!ps.owner) {
			for(uint32_t i = 0; i < std::extent_v<decltype(ps.rgo_worker_data.worker_populations)>; ++i)
				ps.rgo_worker_data.worker_populations[i] = 0;

			if(is_mined) {
				provinces::for_each_pop(ws, ps, [&ws](population::pop& p) {
					for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_mine.workers)>; ++i) {
						if(p.type == ws.s.economy_m.rgo_mine.workers[i].type)
							ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag) = 0;
					}
				});
			} else {
				provinces::for_each_pop(ws, ps, [&ws](population::pop& p) {
					for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
						if(p.type == ws.s.economy_m.rgo_farm.workers[i].type)
							ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag) = 0;
					}
				});
			}
			return;
		}

		float total_workforce = float((is_mined ? ws.s.economy_m.rgo_mine.workforce : ws.s.economy_m.rgo_farm.workforce) * int32_t(ps.rgo_size)) * ps.rgo_worker_data.production_scale *
			((is_mined ?
				ps.modifier_values[modifiers::provincial_offsets::mine_rgo_size] + ps.owner->modifier_values[modifiers::national_offsets::mine_rgo_size] :
				ps.modifier_values[modifiers::provincial_offsets::farm_rgo_size] + ps.owner->modifier_values[modifiers::national_offsets::farm_rgo_size]) +
				ws.w.nation_s.production_adjustments.get(ps.owner->id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_size>(ps.rgo_production)) +
				1.0f);

		if(is_mined) {
			boost::container::small_vector<float, 8> percentage_by_type;
			for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_mine.workers)>; ++i) {
				auto pop_of_type = ws.w.province_s.province_demographics.get(ps.id, population::to_demo_tag(ws, ws.s.economy_m.rgo_mine.workers[i].type));
				if(pop_of_type != 0)
					percentage_by_type.push_back(std::min(1.0f, (total_workforce * ws.s.economy_m.rgo_mine.workers[i].amount) / float(pop_of_type)));
				else
					percentage_by_type.push_back(0.0f);
				ps.rgo_worker_data.worker_populations[i] = std::min(pop_of_type, int32_t(total_workforce * ws.s.economy_m.rgo_mine.workers[i].amount));
			}

			provinces::for_each_pop(ws, ps, [&ws, &percentage_by_type](population::pop& p) {
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_mine.workers)>; ++i) {
					if(p.type == ws.s.economy_m.rgo_mine.workers[i].type)
						ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag) = int32_t(percentage_by_type[i] * ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag));
				}
			});
		} else {
			boost::container::small_vector<float, 8> percentage_by_type;
			for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
				auto pop_of_type = ws.w.province_s.province_demographics.get(ps.id, population::to_demo_tag(ws, ws.s.economy_m.rgo_farm.workers[i].type));
				if(pop_of_type != 0)
					percentage_by_type.push_back(std::min(1.0f, (total_workforce * ws.s.economy_m.rgo_farm.workers[i].amount) / float(pop_of_type)));
				else
					percentage_by_type.push_back(0.0f);
				ps.rgo_worker_data.worker_populations[i] = std::min(pop_of_type, int32_t(total_workforce * ws.s.economy_m.rgo_farm.workers[i].amount));
			}

			provinces::for_each_pop(ws, ps, [&ws, &percentage_by_type](population::pop& p) {
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
					if(p.type == ws.s.economy_m.rgo_farm.workers[i].type)
						ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag) = int32_t(percentage_by_type[i] * ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag));
				}
			});
		}
	}

	void update_factories_employment(world_state& ws, nations::state_instance& si) {
		int32_t* allocation_by_type = (int32_t*)_alloca(sizeof(int32_t) * std::extent_v<decltype(si.factories)> * ws.s.population_m.count_poptypes);
		std::fill_n(allocation_by_type, std::extent_v<decltype(si.factories)> * ws.s.population_m.count_poptypes, 0);

		int32_t* state_population_by_type = ws.w.nation_s.state_demographics.get_row(si.id) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));
		int32_t* unallocated_workers = (int32_t*)_alloca(sizeof(int32_t) * ws.s.population_m.count_poptypes);

		std::copy(state_population_by_type, state_population_by_type + ws.s.population_m.count_poptypes, unallocated_workers);
		std::pair<uint32_t, float> factories_by_profitability[std::extent_v<decltype(si.factories)>];

		for(uint32_t i = 0; i < std::extent_v<decltype(si.factories)>; ++i)
			factories_by_profitability[i] = std::pair<uint32_t, float>(i, si.factories[i].type ? si.factories[i].worker_data.production_scale : 0.0f);

		std::sort(std::begin(factories_by_profitability), std::end(factories_by_profitability), [](std::pair<uint32_t, float> const& a, std::pair<uint32_t, float> const& b) { return a.second > b.second; });

		for(uint32_t i = 0; i < std::extent_v<decltype(si.factories)>; ++i) {
			auto factory_index = factories_by_profitability[i].first;
			auto type = si.factories[factory_index].type;
			if(type) {
				auto factory_workers = si.factories[factory_index].level * type->factory_workers.workforce * si.factories[factory_index].worker_data.production_scale;
				for(uint32_t j = 0; j < std::extent_v<decltype(type->factory_workers.workers)>; ++j) {
					if(is_valid_index(type->factory_workers.workers[j].type)) {
						int32_t amount_from_type = int32_t(factory_workers * type->factory_workers.workers[j].amount);
						uint32_t allocation_index = to_index(type->factory_workers.workers[j].type) *  std::extent_v<decltype(si.factories)> +factory_index;
						allocation_by_type[allocation_index] = std::min(amount_from_type, unallocated_workers[to_index(type->factory_workers.workers[j].type)]);
						unallocated_workers[to_index(type->factory_workers.workers[j].type)] -= allocation_by_type[allocation_index];

						//average with current
						allocation_by_type[allocation_index] =
							(allocation_by_type[allocation_index] + 15) / 16 +
							(si.factories[factory_index].worker_data.worker_populations[j] * 15) / 16;
					}
				}
			}
		}

		for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i)
			normalize_integer_vector(allocation_by_type + i * std::extent_v<decltype(si.factories)>, std::extent_v<decltype(si.factories)>, state_population_by_type[i] - unallocated_workers[i]);

		for(uint32_t i = 0; i < std::extent_v<decltype(si.factories)>; ++i) {
			auto factory_index = factories_by_profitability[i].first;
			auto type = si.factories[factory_index].type;
			if(type) {
				for(uint32_t j = 0; j < std::extent_v<decltype(type->factory_workers.workers)>; ++j) {
					if(is_valid_index(type->factory_workers.workers[j].type)) {
						uint32_t allocation_index = to_index(type->factory_workers.workers[j].type) *  std::extent_v<decltype(si.factories)> +factory_index;
						si.factories[factory_index].worker_data.worker_populations[j] = allocation_by_type[allocation_index];
					}
				}
			}
		}

		nations::for_each_pop(ws, si, [&ws, unallocated_workers, state_population_by_type](population::pop& p) {
			if((ws.s.population_m.pop_types[p.type].flags & population::pop_type::factory_worker) != 0) {
				ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag) =
					int32_t(float(ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag)) * float(state_population_by_type[to_index(p.type)] - unallocated_workers[to_index(p.type)]) / float(state_population_by_type[to_index(p.type)]));
			}
		});
	}

	money_qnty_type get_factory_project_cost(world_state const& ws, factory_type_tag ftype, factory_project_type ptype, money_qnty_type const* prices) {
		if(ptype == factory_project_type::open || ptype == factory_project_type::expand) {
			Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices_v(prices, ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> costs(ws.s.economy_m.building_costs.get_row(ftype), ws.s.economy_m.aligned_32_goods_count);

			return prices_v.dot(costs);
		} else {
			// TODO

			Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices_v(prices, ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> costs(ws.s.economy_m.factory_input_goods.get_row(ftype), ws.s.economy_m.aligned_32_goods_count);

			return prices_v.dot(costs) * money_qnty_type(10);
		}
	}
	money_qnty_type get_railroad_cost(world_state const& ws, money_qnty_type const* prices) {
		Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices_v(prices, ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> costs(ws.s.economy_m.building_costs.get_row(ws.s.economy_m.railroad.cost_tag), ws.s.economy_m.aligned_32_goods_count);

		return prices_v.dot(costs);
	}

	float project_completion(world_state const& ws, nations::state_instance const& si, money_qnty_type const* prices) {
		if(si.project.type == pop_project_type::railroad) {
			auto cost = get_railroad_cost(ws, prices);
			return cost != 0 ? float(si.project.funds / cost) : 0.0f;
		} else if(si.project.type == pop_project_type::factory) {
			if(auto ftype = si.project.factory_type; is_valid_index(ftype)) {
				auto cost = get_factory_project_cost(ws, ftype, get_factory_project_type(si, ftype), prices);
				return cost != 0 ? float(si.project.funds / cost) : 0.0f;
			}
		}
		return 0.0f;
	}

	float get_per_worker_profit(world_state const& ws, nations::state_instance const& si, factory_instance const& f) {
		if(!f.type)
			return std::numeric_limits<float>::min();

		Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices(state_current_prices(ws, si), ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> inputs(ws.s.economy_m.factory_input_goods.get_row(f.type->id), ws.s.economy_m.aligned_32_goods_count);

		auto inputs_cost = prices.dot(inputs);

		return float(prices[to_index(f.type->output_good)] * f.type->output_amount - inputs_cost);
	}

	money_qnty_type get_factory_profit(world_state const& ws, provinces::province_state const& in_province, factory_instance const& f, money_qnty_type const* prices) {
		auto powner = in_province.owner;
		auto f_type = f.type;
		if(bool(powner) && bool(f_type)) {
			auto modifiers = factory_production_modifiers(ws, f.worker_data, f_type->bonuses,
				f_type->factory_workers, *powner, in_province, f.level,
				f_type->output_good,
				((powner->flags & nations::nation::is_mobilized) == 0) ? 1.0f : std::max(0.0f, 1.0f - powner->modifier_values[modifiers::national_offsets::mobilisation_size] * powner->modifier_values[modifiers::national_offsets::mobilisation_economy_impact]));

			//TODO: use modifiers properly

			Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices_v(prices, ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> inputs(ws.s.economy_m.factory_input_goods.get_row(f_type->id), ws.s.economy_m.aligned_32_goods_count);

			auto inputs_cost = prices_v.dot(inputs * modifiers.input_modifier);

			return (prices[to_index(f_type->output_good)] * f_type->output_amount * modifiers.output_modifier - inputs_cost) * modifiers.throughput_modifier;
		}
		return money_qnty_type(0);
	}

	float factory_employment_fraction(world_state const& ws, factory_instance const& fi) {
		if(auto f_type = fi.type; f_type) {
			float total_workforce = float(f_type->factory_workers.workforce * fi.level);
			float total_employed = float(std::accumulate(std::begin(fi.worker_data.worker_populations), std::end(fi.worker_data.worker_populations), 0));
			return total_workforce != 0.0f ? total_employed / total_workforce : 0.0f;
		} else {
			return 0.0f;
		}
	}

	void init_factory_employment(world_state& ws) {
		ws.w.nation_s.states.for_each([&ws](nations::state_instance& si) {
			if(si.owner) {
				for(uint32_t i = 0; i < std::extent_v<decltype(si.factories)>; ++i) {
					if(si.factories[i].type && si.factories[i].level != 0) {
						for(uint32_t j = 0; j < std::extent_v<decltype(si.factories[i].type->factory_workers.workers)>; ++j) {
							if(is_valid_index(si.factories[i].type->factory_workers.workers[j].type))
								si.factories[i].worker_data.worker_populations[j] = 100;
						}
					}
				}

				update_factories_employment(ws, si);
			}
		});
	}

	bool possible_to_invest_in(world_state const& ws, nations::nation const& investor, nations::nation const& target) {
		return !nations::is_great_power(ws, target) && nations::is_great_power(ws, investor) &&
			((investor.current_rules.rules_value & (issues::rules::open_factory_invest | issues::rules::expand_factory_invest | issues::rules::build_factory_invest)) != 0) &&
			((target.current_rules.rules_value & issues::rules::allow_foreign_investment) != 0) &&
			(investor.id != target.id);
	}

	int32_t count_factories_in_state(nations::state_instance const& si) {
		return
			std::accumulate(
				std::begin(si.factories),
				std::end(si.factories),
				0i32,
				[](int32_t v, economy::factory_instance const& f) { return v + int32_t(f.level != 0); });
	}

	int32_t count_factories_in_nation(world_state const& ws, nations::nation const& n) {
		auto states = get_range(ws.w.nation_s.state_arrays, n.member_states);
		int32_t total = 0;
		for(auto s = states.first; s != states.second; ++s) {
			if(auto si = s->state; si) {
				total += count_factories_in_state(*si);
			}
		}
		return total;
	}

	float average_railroad_level(world_state const& ws, nations::state_instance const& si) {
		float total_provinces = 0.0f;
		float total_levels = 0.0f;
		nations::for_each_province(ws, si, [&total_provinces, &total_levels](provinces::province_state const& ps) {
			total_levels += float(ps.railroad_level);
			++total_provinces;
		});
		return total_provinces != 0.0f ? total_levels / total_provinces : 0.0f;
	}

	bool factory_is_open(factory_instance const& fi) {
		return bool(fi.type) && fi.level != 0 && fi.worker_data.production_scale > 0.0f;
	}
	bool factory_is_closed(factory_instance const& fi) {
		return bool(fi.type) && fi.level != 0 && fi.worker_data.production_scale <= 0.0f;
	}
	bool factory_is_under_construction(factory_instance const& fi) {
		return bool(fi.type) && fi.level == 0 && fi.factory_progress > 0.0f;
	}
	bool factory_is_upgrading(factory_instance const& fi) {
		return bool(fi.type) && fi.level != 0 && fi.factory_progress > 0.0f;
	}
	factory_project_type get_factory_project_type(nations::state_instance const& location, factory_type_tag ftype) {
		for(auto& fi : location.factories) {
			if(auto ft = fi.type; bool(ft) && ft->id == ftype) {
				if(factory_is_closed(fi))
					return factory_project_type::reopen;
				else
					return factory_project_type::expand;
			}
		}
		return factory_project_type::open;
	}


	constexpr float pop_needs_divisor = 200'000.0f;
	constexpr money_qnty_type savings_rate = money_qnty_type(0.25);

	void apply_pop_consumption(world_state & ws, provinces::province_state & p,
		Eigen::Map<Eigen::VectorXf, Eigen::AlignmentType::Aligned32>& __restrict state_demand,
		Eigen::Map<Eigen::VectorXf> const& __restrict masked_prices,
		economy::money_qnty_type const* __restrict life_needs_cost_by_type,
		economy::money_qnty_type const* __restrict everyday_needs_cost_by_type,
		economy::money_qnty_type const* __restrict luxury_needs_cost_by_type
	) {

		provinces::for_each_pop(ws, p, [&ws, &state_demand, &masked_prices, &p,
			life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type
			](population::pop& po) {

			float pop_size_multiplier = float(ws.w.population_s.pop_demographics.get(po.id, population::total_population_tag)) / pop_needs_divisor;

			Eigen::Map<Eigen::Array<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> ln(
				ws.s.population_m.life_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Array<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> en(
				ws.s.population_m.everyday_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Array<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> xn(
				ws.s.population_m.luxury_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);

			
			auto enabled_goods = ws.w.nation_s.active_goods.get_row(p.owner->id);
			const auto total_ln = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<goods_qnty_type>(),
				[enabled_goods, &ln](int32_t i) { return bit_vector_test(enabled_goods, i) ? ln[i] : goods_qnty_type(0); });
			const auto total_en = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<goods_qnty_type>(),
				[enabled_goods, &en](int32_t i) { return bit_vector_test(enabled_goods, i) ? en[i] : goods_qnty_type(0); });
			const auto total_xn = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<goods_qnty_type>(),
				[enabled_goods, &xn](int32_t i) { return bit_vector_test(enabled_goods, i) ? xn[i] : goods_qnty_type(0); });
			

			if(po.money < (pop_size_multiplier * life_needs_cost_by_type[to_index(po.type)])) {
				// case: can only partially satisfy life needs
				auto fraction = po.money / (pop_size_multiplier * life_needs_cost_by_type[to_index(po.type)]);
				po.needs_satisfaction = fraction;
				state_demand += (ln * po.money / total_ln).matrix();
#ifdef DEBUG_ECONOMY
				{
					// std::lock_guard lk(ws.w.economy_s.pop_consumption_mutex);
					//Eigen::Map<Eigen::Array<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
					//	ws.w.economy_s.world_pop_consumption.data(), ws.s.economy_m.aligned_32_goods_count) += 
					//	(ln * po.money / total_ln);
				}
#endif
				po.money = 0.0f;
			} else {
				// case: can fully satisfy life needs

				state_demand += (ln * (pop_size_multiplier * life_needs_cost_by_type[to_index(po.type)]) / total_ln).matrix();

#ifdef DEBUG_ECONOMY
				{
					//std::lock_guard lk(ws.w.economy_s.pop_consumption_mutex);
					//Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
					//	ws.w.economy_s.world_pop_consumption.data(), ws.s.economy_m.aligned_32_goods_count) += pop_size_multiplier * Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
					//		ws.s.population_m.life_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
				}
#endif
				po.money -= pop_size_multiplier * life_needs_cost_by_type[to_index(po.type)];

				if(po.money < pop_size_multiplier * everyday_needs_cost_by_type[to_index(po.type)]) {
					// case: can partially satisfy everyday needs

					auto fraction = po.money / (pop_size_multiplier * everyday_needs_cost_by_type[to_index(po.type)]);
					po.needs_satisfaction = 1.0f + fraction;

					state_demand += (en * po.money / total_en).matrix();

#ifdef DEBUG_ECONOMY
					{
						//std::lock_guard lk(ws.w.economy_s.pop_consumption_mutex);
						//Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
						//	ws.w.economy_s.world_pop_consumption.data(), ws.s.economy_m.aligned_32_goods_count) +=

						//	(po.money / everyday_needs_cost_by_type[to_index(po.type)]) *
						//	Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
						//		ws.s.population_m.everyday_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
					}
#endif

					po.money = 0.0f;
				} else {
					// case: can fully satisfy everyday needs
					state_demand += (en * (pop_size_multiplier * everyday_needs_cost_by_type[to_index(po.type)]) / total_en).matrix();

#ifdef DEBUG_ECONOMY
					{
						//std::lock_guard lk(ws.w.economy_s.pop_consumption_mutex);
						//Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
						//	ws.w.economy_s.world_pop_consumption.data(), ws.s.economy_m.aligned_32_goods_count) += pop_size_multiplier * Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
						//		ws.s.population_m.everyday_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
					}
#endif

					po.money -= pop_size_multiplier * everyday_needs_cost_by_type[to_index(po.type)];

					// remainder of money spent on luxury needs
					auto last_fraction = po.money * (money_qnty_type(1) - savings_rate) / (pop_size_multiplier * luxury_needs_cost_by_type[to_index(po.type)]);
					last_fraction = last_fraction > 1.0f ? (1.0f + (last_fraction - 1.0f) * 0.1f) : last_fraction;
					po.needs_satisfaction = 2.0f + last_fraction;

					state_demand += (xn * (last_fraction * pop_size_multiplier * luxury_needs_cost_by_type[to_index(po.type)]) / total_xn).matrix();

#ifdef DEBUG_ECONOMY
					{
						//std::lock_guard lk(ws.w.economy_s.pop_consumption_mutex);
						//Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
						//	ws.w.economy_s.world_pop_consumption.data(), ws.s.economy_m.aligned_32_goods_count) += pop_size_multiplier * last_fraction * Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
						//		ws.s.population_m.luxury_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
					}
#endif

					po.money *= savings_rate;
				}
			}

			
		});
	}

	void create_masked_prices(Eigen::Map <Eigen::Matrix<economy::money_qnty_type, -1, 1>>& __restrict masked_prices,
		world_state const& ws, nations::state_instance const& si) {

		auto state_prices = ws.w.nation_s.state_prices.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1)));

		masked_prices.setZero();
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(si.owner->id);
		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			masked_prices[i] = bit_vector_test(enabled_goods, i) ? state_prices[i] : economy::money_qnty_type(0);
		}
	}

	money_qnty_type get_life_needs_cost(world_state const& ws, nations::state_instance const& si, population::pop_type_tag ptype) {
		auto state_owner = si.owner;
		auto state_capital = nations::get_state_capital(ws, si);
		if(!state_owner || !state_capital)
			return money_qnty_type(0);

		Eigen::Map<Eigen::VectorXf> masked_prices((money_qnty_type*)_alloca(sizeof(money_qnty_type) * ws.s.economy_m.aligned_32_goods_count), ws.s.economy_m.aligned_32_goods_count);
		create_masked_prices(masked_prices, ws, si);

		auto this_strata = ws.s.population_m.pop_types[ptype].flags & population::pop_type::strata_mask;

		economy::money_qnty_type ln_factor;

		if(this_strata == population::pop_type::strata_poor) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::poor_life_needs] + state_capital->modifier_values[modifiers::provincial_offsets::poor_life_needs];
		} else if(this_strata == population::pop_type::strata_middle) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::middle_life_needs] + state_capital->modifier_values[modifiers::provincial_offsets::middle_life_needs];
		} else { //if(this_strata == population::pop_type::strata_rich) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::rich_life_needs] + state_capital->modifier_values[modifiers::provincial_offsets::rich_life_needs];
		}

		const Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> ln(ws.s.population_m.life_needs.get_row(ptype), ws.s.economy_m.aligned_32_goods_count);
		return ln_factor * masked_prices.dot(ln);
	}

	void fill_needs_costs_arrays(world_state & ws, nations::state_instance const& si, provinces::province_state const& state_capital,
		Eigen::Map<Eigen::VectorXf> const& __restrict masked_prices,
		economy::money_qnty_type * __restrict life_needs_cost_by_type,
		economy::money_qnty_type * __restrict everyday_needs_cost_by_type,
		economy::money_qnty_type * __restrict luxury_needs_cost_by_type) {

		for(population::pop_type_tag::value_base_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
			population::pop_type_tag this_type(i);
			auto this_strata = ws.s.population_m.pop_types[this_type].flags & population::pop_type::strata_mask;

			economy::money_qnty_type ln_factor;
			economy::money_qnty_type ev_factor;
			economy::money_qnty_type lx_factor;

			if(this_strata == population::pop_type::strata_poor) {
				ln_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::poor_life_needs] + state_capital.modifier_values[modifiers::provincial_offsets::poor_life_needs];
				ev_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::poor_everyday_needs] + state_capital.modifier_values[modifiers::provincial_offsets::poor_everyday_needs];
				lx_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::poor_luxury_needs] + state_capital.modifier_values[modifiers::provincial_offsets::poor_luxury_needs];
			} else if(this_strata == population::pop_type::strata_middle) {
				ln_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::middle_life_needs] + state_capital.modifier_values[modifiers::provincial_offsets::middle_life_needs];
				ev_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::middle_everyday_needs] + state_capital.modifier_values[modifiers::provincial_offsets::middle_everyday_needs];
				lx_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::middle_luxury_needs] + state_capital.modifier_values[modifiers::provincial_offsets::middle_luxury_needs];
			} else { //if(this_strata == population::pop_type::strata_rich) {
				ln_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::rich_life_needs] + state_capital.modifier_values[modifiers::provincial_offsets::rich_life_needs];
				ev_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::rich_everyday_needs] + state_capital.modifier_values[modifiers::provincial_offsets::rich_everyday_needs];
				lx_factor = economy::money_qnty_type(1) + si.owner->modifier_values[modifiers::national_offsets::rich_luxury_needs] + state_capital.modifier_values[modifiers::provincial_offsets::rich_luxury_needs];
			}

			Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> ln(ws.s.population_m.life_needs.get_row(this_type), ws.s.economy_m.aligned_32_goods_count);
			life_needs_cost_by_type[i] = ln_factor * masked_prices.dot(ln);
			Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> en(ws.s.population_m.everyday_needs.get_row(this_type), ws.s.economy_m.aligned_32_goods_count);
			everyday_needs_cost_by_type[i] = ev_factor * masked_prices.dot(en);
			Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> xn(ws.s.population_m.luxury_needs.get_row(this_type), ws.s.economy_m.aligned_32_goods_count);
			luxury_needs_cost_by_type[i] = lx_factor * masked_prices.dot(xn);

			/*
			int32_t count_ln = 0;
			int32_t count_en = 0;
			int32_t count_xn = 0;
			for(uint32_t j = 0; j < ws.s.economy_m.goods_count; ++j) {
				if(masked_prices[j] != 0) {
					if(ln[j] != 0) {
						life_needs_cost_by_type[i] += ln[j];
						++count_ln;
					}
					if(en[j] != 0) {
						everyday_needs_cost_by_type[i] += en[j];
						++count_en;
					}
					if(xn[j] != 0) {
						luxury_needs_cost_by_type[i] += xn[j];
						++count_xn;
					}
				}
			}
			life_needs_cost_by_type[i] *= ln_factor * ln_factor * masked_prices.dot(ln) / goods_qnty_type(count_ln);
			everyday_needs_cost_by_type[i] *= ev_factor * ev_factor * masked_prices.dot(en) / goods_qnty_type(count_en);
			luxury_needs_cost_by_type[i] *= lx_factor * lx_factor * masked_prices.dot(xn) / goods_qnty_type(count_xn);
			*/
		}
	}

	money_qnty_type* state_current_prices(world_state const& ws, nations::state_instance const& si) {
		return ws.w.nation_s.state_prices.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1)));
	}

	money_qnty_type* state_old_prices(world_state const& ws, nations::state_instance const& si) {
		return ws.w.nation_s.state_prices.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * ((to_index(ws.w.current_date) & 1)));
	}

	goods_qnty_type* state_current_production(world_state const& ws, nations::state_instance const& si) {
		return ws.w.nation_s.state_production.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1)));
	}

	money_qnty_type* state_current_demand(world_state const& ws, nations::state_instance const& si) {
		return ws.w.nation_s.state_demand.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1)));
	}


	void init_artisan_producation(world_state& ws) {
		for(auto& p : ws.w.province_s.province_state_container) {
			if(p.owner)
				p.artisan_production = ws.s.economy_m.artisan_types[get_profitable_artisan(ws, p)].output_good;
		}
	}

	void set_initial_money(world_state& ws) {
		ws.w.nation_s.states.for_each([&ws](nations::state_instance& si) {
			if(!(si.owner))
				return;

			const auto state_capital = nations::get_state_capital(ws, si);
			if(!state_capital)
				return;

			economy::money_qnty_type* life_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
			economy::money_qnty_type* everyday_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
			economy::money_qnty_type* luxury_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);

			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>> masked_prices(
				(economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count),
				ws.s.economy_m.aligned_32_goods_count);
			create_masked_prices(masked_prices, ws, si);

			fill_needs_costs_arrays(ws, si, *state_capital, masked_prices, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type);

			nations::for_each_pop(ws, si, [&ws, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type](population::pop& p) {
				p.money = (life_needs_cost_by_type[to_index(p.type)] +
					everyday_needs_cost_by_type[to_index(p.type)] +
					economy::money_qnty_type(0.5) * luxury_needs_cost_by_type[to_index(p.type)]) * float(ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag)) / pop_needs_divisor;
			});

		});
	}

	std::pair<money_qnty_type, money_qnty_type> global_price_range(world_state const& ws, economy::goods_tag t) {
		std::pair<money_qnty_type, money_qnty_type> result(std::numeric_limits<money_qnty_type>::max(), std::numeric_limits<money_qnty_type>::lowest());
		ws.w.nation_s.states.for_each([&ws, t, &result](nations::state_instance const& si) {
			if(si.owner) {
				auto prices = state_current_prices(ws, si);
				result.first = std::min(result.first, prices[to_index(t)]);
				result.second = std::max(result.second, prices[to_index(t)]);
			}
		});
		return result;
	}

	float distance_multiplier(float v) {
		return std::max(0.0f, 1.0f - 0.000045f * v);
	}

	constexpr money_qnty_type price_change_rate = money_qnty_type(0.25);
	constexpr money_qnty_type price_gravity_factor = money_qnty_type(0.05);

	void economy_single_good_tick(world_state& ws, goods_tag tag) {
		std::vector<goods_qnty_type, concurrent_allocator<goods_qnty_type>> effective_world_supply;
		std::vector<goods_qnty_type, concurrent_allocator<goods_qnty_type>> effective_world_demand;

		const auto total_state_count = ws.w.nation_s.states.indices_in_use * ws.w.nation_s.states.block_size_v;
		effective_world_supply.resize(total_state_count);
		effective_world_demand.resize(total_state_count);

		auto base_good_price = ws.s.economy_m.goods[tag].base_price;

		// determine effective world supply for each state

		ws.w.nation_s.states.for_each([&ws, &effective_world_supply, total_state_count, tag](nations::state_instance const& si) {
			auto state_cap = nations::get_state_capital(ws, si);

			if(!state_cap)
				return;
			auto distance_row_offset = to_index(state_cap->id) * ws.s.province_m.province_container.size();

			economy::goods_qnty_type supply_amount = std::transform_reduce(integer_iterator(0), integer_iterator(int32_t(total_state_count)),
				economy::goods_qnty_type(0), std::plus<economy::goods_qnty_type>(),
				[&ws, tag, distance_row_offset](int32_t i) {

				auto& other_state = ws.w.nation_s.states[nations::state_tag(nations::state_tag::value_base_t(i))];
				auto other_state_cap = nations::get_state_capital(ws, other_state);
				if(other_state_cap) {
					return distance_multiplier(ws.w.province_s.province_distance_to[distance_row_offset + to_index(other_state_cap->id)]) *
						ws.w.nation_s.state_production.get_row(nations::state_tag(nations::state_tag::value_base_t(i)))[to_index(tag) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1))];
				} else {
					return economy::goods_qnty_type(0);
				}
			});

			effective_world_supply[to_index(si.id)] = supply_amount;
		});

		// distribute money to each state

		ws.w.nation_s.states.for_each([&ws, &effective_world_supply, &effective_world_demand, total_state_count, tag](nations::state_instance const& si) {
			auto state_cap = nations::get_state_capital(ws, si);
			if(!state_cap)
				return;

			auto local_production = ws.w.nation_s.state_production.get_row(si.id)[to_index(tag) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1))];

			if(local_production != 0) {
				economy::money_qnty_type total_other_demand = std::transform_reduce(integer_iterator(0), integer_iterator(int32_t(total_state_count)),
					economy::money_qnty_type(0), std::plus<economy::money_qnty_type>(),
					[&ws, tag, local_capital_id = to_index(state_cap->id), local_production, &effective_world_supply](int32_t i) {

					auto& other_state = ws.w.nation_s.states[nations::state_tag(nations::state_tag::value_base_t(i))];
					auto other_state_cap = nations::get_state_capital(ws, other_state);
					if(other_state_cap) {
						auto other_total_demand = ws.w.nation_s.state_demand.get_row(nations::state_tag(nations::state_tag::value_base_t(i)))[to_index(tag) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1))];
						auto other_distance_to_self = ws.w.province_s.province_distance_to[to_index(other_state_cap->id) * ws.s.province_m.province_container.size() + local_capital_id];
						return other_total_demand * distance_multiplier(other_distance_to_self) * local_production / effective_world_supply[i];
					} else {
						return economy::money_qnty_type(0);
					}
				});
				effective_world_demand[to_index(si.id)] = total_other_demand;
			} else {
				effective_world_demand[to_index(si.id)] = goods_qnty_type(0);
			}

			
		});

		// determine new local price

		ws.w.nation_s.states.for_each([&ws, &effective_world_supply, &effective_world_demand, total_state_count, tag, base_good_price](nations::state_instance const& si) {
			auto state_cap = nations::get_state_capital(ws, si);
			if(!state_cap)
				return;

			auto distance_row_offset = to_index(state_cap->id) * ws.s.province_m.province_container.size();
			auto local_demand = ws.w.nation_s.state_demand.get_row(si.id)[to_index(tag) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1))];
			auto effective_w_supply = effective_world_supply[to_index(si.id)];
			
			economy::goods_qnty_type purchase_amount = std::transform_reduce(integer_iterator(0), integer_iterator(int32_t(total_state_count)),
				economy::goods_qnty_type(0), std::plus<economy::goods_qnty_type>(),
				[&ws, tag, effective_w_supply, local_demand, distance_row_offset, &effective_world_demand](int32_t i){

				auto& other_state = ws.w.nation_s.states[nations::state_tag(nations::state_tag::value_base_t(i))];
				auto other_state_cap = nations::get_state_capital(ws, other_state);
				if(other_state_cap) {
					auto produced_in_state = ws.w.nation_s.state_production.get_row(nations::state_tag(nations::state_tag::value_base_t(i)))[to_index(tag) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1))];
					if(produced_in_state != 0) {
						return produced_in_state * (local_demand *
							distance_multiplier(ws.w.province_s.province_distance_to[distance_row_offset + to_index(other_state_cap->id)]) *
							produced_in_state /
							effective_w_supply) / effective_world_demand[i];
					}
				} 
				return economy::goods_qnty_type(0);
			});
			
			auto old_price = ws.w.nation_s.state_prices.get_row(si.id)[to_index(tag) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - to_index(ws.w.current_date) & 1))];
			ws.w.nation_s.state_prices.get_row(si.id)[to_index(tag) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1))] =
				(purchase_amount != 0 ?
					std::min(old_price * (money_qnty_type(1) - price_change_rate) + price_change_rate * local_demand / purchase_amount, money_qnty_type(999.9)) :
					old_price * (money_qnty_type(1) - price_change_rate) + price_change_rate * money_qnty_type(999.8)) *
				(money_qnty_type(1) - price_gravity_factor) + base_good_price * price_gravity_factor;
		});
	}

	constexpr goods_qnty_type global_rgo_production_multiplier = goods_qnty_type(5.0);
	constexpr goods_qnty_type global_throughput_multiplier = goods_qnty_type(1.0);
	constexpr float production_scaling_speed_factor = 0.5f;
	constexpr float scale_speed(float v) {
		return 1.0f - production_scaling_speed_factor + production_scaling_speed_factor * v;
	}

	void update_rgo_production(world_state& ws,
		nations::state_instance& si,
		economy::money_qnty_type* __restrict pay_by_type,
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>& __restrict state_production,
		money_qnty_type output_price,
		economy::money_qnty_type const* __restrict life_needs_cost_by_type,
		provinces::province_state& p, workers_information const& rgo_type, float mobilization_effect) {

		auto rgo_modifiers = rgo_production_modifiers(ws, p.rgo_worker_data, rgo_type,
			*si.owner, p, p.rgo_size, p.rgo_production, mobilization_effect);

		auto output_amount = global_rgo_production_multiplier *
			p.rgo_worker_data.production_scale *
			ws.s.economy_m.goods[p.rgo_production].base_rgo_value *
			rgo_modifiers.output_modifier *
			rgo_modifiers.throughput_modifier;

		money_qnty_type min_wage = money_qnty_type(0);
		money_qnty_type total_worker_pop = money_qnty_type(0);
		for(uint32_t i = 0; i < std::extent_v<decltype(rgo_type.workers)>; ++i) {
			if(is_valid_index(rgo_type.workers[i].type)) {
				min_wage += life_needs_cost_by_type[to_index(rgo_type.workers[i].type)] * money_qnty_type(p.rgo_worker_data.worker_populations[i]) / pop_needs_divisor;
				total_worker_pop += money_qnty_type(p.rgo_worker_data.worker_populations[i]);
			}
		}

		min_wage = std::max(min_wage, 0.0001f);

		state_production[to_index(p.rgo_production)] += output_amount;

#ifdef DEBUG_ECONOMY
		{
			std::lock_guard lk(ws.w.economy_s.rgo_production_mutex);
			ws.w.economy_s.world_rgo_production[p.rgo_production] += output_amount;
		}
#endif

		auto rgo_profit = output_amount * output_price - min_wage;

		if(rgo_profit <= 0) {
			for(uint32_t i = 0; i < std::extent_v<decltype(rgo_type.workers)>; ++i) {
				if(is_valid_index(rgo_type.workers[i].type)) {
					pay_by_type[to_index(rgo_type.workers[i].type)] += 
						(life_needs_cost_by_type[to_index(rgo_type.workers[i].type)] * money_qnty_type(p.rgo_worker_data.worker_populations[i]) / pop_needs_divisor) *
						output_amount * output_price / min_wage;
				}
			}
		} else {
			for(uint32_t i = 0; i < std::extent_v<decltype(rgo_type.workers)>; ++i) {
				if(is_valid_index(rgo_type.workers[i].type)) {
					pay_by_type[to_index(rgo_type.workers[i].type)] +=
						(life_needs_cost_by_type[to_index(rgo_type.workers[i].type)] * money_qnty_type(p.rgo_worker_data.worker_populations[i]) / pop_needs_divisor)
						+ rgo_profit * money_qnty_type(0.2) * money_qnty_type(p.rgo_worker_data.worker_populations[i]) / total_worker_pop;
				}
			}
			pay_by_type[to_index(rgo_type.owner.type)] += rgo_profit * money_qnty_type(0.8);
		}
		
		// rescale production
		// p.rgo_worker_data.production_scale = std::clamp(p.rgo_worker_data.production_scale * scale_speed(output_amount * output_price / min_wage), 0.05f, 1.0f);
	}

	money_qnty_type artisan_profit_unit(
		world_state const& ws,
		provinces::province_state const& p,
		nations::nation const& owner,
		artisan_type const& atype,
		money_qnty_type const* __restrict prices,
		money_qnty_type artisan_life_needs
	) {
		Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>> state_prices(prices, ws.s.economy_m.goods_count);
		auto artisan_modifiers = artisan_production_modifiers(ws, owner, p, atype.output_good, 1.0f);

		auto output_amount = 
			atype.output_amount * global_throughput_multiplier *
			artisan_modifiers.output_modifier *
			artisan_modifiers.throughput_modifier;

		Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>> inputs(ws.s.economy_m.artisan_input_goods.get_row(atype.id), ws.s.economy_m.goods_count);

		auto inputs_cost = state_prices.dot(inputs) * artisan_modifiers.input_modifier * artisan_modifiers.throughput_modifier * global_throughput_multiplier;
		money_qnty_type min_wage = artisan_life_needs * atype.workforce / pop_needs_divisor;


		return output_amount * state_prices[to_index(atype.output_good)] - min_wage - inputs_cost;
	}


	void update_artisan_production(world_state& ws,
		nations::state_instance& si,
		economy::money_qnty_type* __restrict pay_by_type,
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>& __restrict state_production,
		Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>& __restrict current_state_demand,
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> const& __restrict state_prices,
		economy::money_qnty_type const* __restrict life_needs_cost_by_type,
		provinces::province_state& p, float mobilization_effect) {


		auto artisan_modifiers = artisan_production_modifiers(ws, *si.owner, p, p.artisan_production, mobilization_effect);
		const auto artisan_tag = ws.s.economy_m.goods[p.artisan_production].artisan_id;
		const auto& artisan_type = ws.s.economy_m.artisan_types[artisan_tag];

		auto artisan_pop = float(ws.w.province_s.province_demographics.get(p.id, population::to_demo_tag(ws, ws.s.population_m.artisan)));
		if(artisan_pop == 0)
			return;

		auto common_scale_amount = global_throughput_multiplier * p.artisan_production_scale * artisan_pop / artisan_type.workforce;

		auto output_amount =  common_scale_amount *
			artisan_type.output_amount *
			artisan_modifiers.output_modifier *
			artisan_modifiers.throughput_modifier;
		
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> inputs(
			ws.s.economy_m.artisan_input_goods.get_row(artisan_tag), ws.s.economy_m.aligned_32_goods_count);

		auto inputs_cost = state_prices.dot(inputs) * common_scale_amount * artisan_modifiers.input_modifier * artisan_modifiers.throughput_modifier;

		money_qnty_type min_wage = life_needs_cost_by_type[to_index(ws.s.population_m.artisan)] * artisan_pop / pop_needs_divisor;
		
		state_production[to_index(p.artisan_production)] += output_amount;
		//current_state_demand += (inputs.array() * state_prices.array() * common_scale_amount * artisan_modifiers.input_modifier * artisan_modifiers.throughput_modifier).matrix();
		current_state_demand += inputs * inputs_cost / inputs.sum();

#ifdef DEBUG_ECONOMY
		{
			std::lock_guard lk(ws.w.economy_s.other_production_mutex);
			ws.w.economy_s.world_other_production[p.artisan_production] += output_amount;
		}
		{
			//std::lock_guard lk(ws.w.economy_s.other_consumption_mutex);
			//Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
			//	ws.w.economy_s.world_other_consumption.data(), ws.s.economy_m.aligned_32_goods_count) += inputs * common_scale_amount * artisan_modifiers.input_modifier * artisan_modifiers.throughput_modifier;
		}
#endif

		auto profit = output_amount * state_prices[to_index(p.artisan_production)] - min_wage - inputs_cost;

		if(profit > 0) {
			pay_by_type[to_index(ws.s.population_m.artisan)] += profit;
		}

		//p.artisan_production_scale = std::clamp(
		//	p.artisan_production_scale * scale_speed(output_amount * state_prices[to_index(p.artisan_production)] / (min_wage + inputs_cost)),
		//	0.05f, 1.0f);

		/*if(profit < 0 && p.artisan_production_scale < 0.95f) {
			std::uniform_real_distribution<money_qnty_type> dist(money_qnty_type(0), 1.0f);
			if(dist(get_local_generator()) > sqrt(p.artisan_production_scale)) {
				p.artisan_production = ws.s.economy_m.artisan_types[get_profitable_artisan(ws, p)].output_good;
				//p.artisan_production_scale = 1.0f;
			}
		}*/

		if(profit < 0) {
			std::uniform_real_distribution<money_qnty_type> dist(money_qnty_type(0), 1.0f);
			if(dist(get_local_generator()) > sqrt(output_amount * state_prices[to_index(p.artisan_production)] / (min_wage + inputs_cost))) {
				p.artisan_production = ws.s.economy_m.artisan_types[get_profitable_artisan(ws, p)].output_good;
				//p.artisan_production_scale = 1.0f;
			}
		}
	}

	void update_factory_production(world_state& ws,
		nations::state_instance& si,
		economy::money_qnty_type* __restrict pay_by_type,
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>& __restrict state_production,
		Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>& __restrict current_state_demand,
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> const& __restrict state_prices,
		economy::money_qnty_type const* __restrict life_needs_cost_by_type,
		factory_instance& instance, provinces::province_state& capital, float mobilization_effect) {

		auto& f_type = *instance.type;

		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> inputs(
			ws.s.economy_m.factory_input_goods.get_row(f_type.id), ws.s.economy_m.aligned_32_goods_count);

		const auto factory_modifiers = factory_production_modifiers(ws, instance.worker_data, f_type.bonuses, f_type.factory_workers,
			*si.owner, capital, instance.level, f_type.output_good, mobilization_effect);

		auto output_amount = global_throughput_multiplier *
			instance.worker_data.production_scale *
			instance.type->output_amount *
			factory_modifiers.output_modifier *
			factory_modifiers.throughput_modifier;

		money_qnty_type min_wage = money_qnty_type(0);
		money_qnty_type total_worker_pop = money_qnty_type(0);
		for(uint32_t i = 0; i < std::extent_v<decltype(instance.worker_data.worker_populations)>; ++i) {
			if(is_valid_index(f_type.factory_workers.workers[i].type)) {
				min_wage += life_needs_cost_by_type[to_index(f_type.factory_workers.workers[i].type)] * money_qnty_type(instance.worker_data.worker_populations[i]) / pop_needs_divisor;
				total_worker_pop += money_qnty_type(instance.worker_data.worker_populations[i]);
			}
		}

		min_wage = std::max(min_wage, 0.0001f);

		auto inputs_cost = state_prices.dot(inputs) *
			 instance.worker_data.production_scale * factory_modifiers.input_modifier * factory_modifiers.throughput_modifier * global_throughput_multiplier;
		auto output_value = output_amount * state_prices[to_index(f_type.output_good)];
		auto profit = output_value - min_wage - inputs_cost;

		state_production[to_index(f_type.output_good)] += output_amount;
		current_state_demand += inputs * inputs_cost / inputs.sum();
		//current_state_demand +=
		//	(inputs.array() * state_prices.array() * instance.worker_data.production_scale * factory_modifiers.input_modifier * factory_modifiers.throughput_modifier * global_throughput_multiplier).matrix();

#ifdef DEBUG_ECONOMY
		{
			std::lock_guard lk(ws.w.economy_s.other_production_mutex);
			ws.w.economy_s.world_other_production[f_type.output_good] += output_amount;
		}
		{
			//std::lock_guard lk(ws.w.economy_s.other_consumption_mutex);
			//Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32>(
			//	ws.w.economy_s.world_other_consumption.data(), ws.s.economy_m.aligned_32_goods_count) += inputs * instance.worker_data.production_scale * factory_modifiers.input_modifier * factory_modifiers.throughput_modifier * global_throughput_multiplier;
		}
#endif

		if(profit <= 0) {
			if(output_value - inputs_cost > 0) {
				for(uint32_t i = 0; i < std::extent_v<decltype(instance.worker_data.worker_populations)>; ++i) {
					if(is_valid_index(f_type.factory_workers.workers[i].type)) {
						pay_by_type[to_index(f_type.factory_workers.workers[i].type)] +=
							(life_needs_cost_by_type[to_index(f_type.factory_workers.workers[i].type)] * money_qnty_type(instance.worker_data.worker_populations[i]) / pop_needs_divisor) *
							(output_value - inputs_cost) / min_wage;
					}
				}
			}
		} else {
			for(uint32_t i = 0; i < std::extent_v<decltype(instance.worker_data.worker_populations)>; ++i) {
				if(is_valid_index(f_type.factory_workers.workers[i].type)) {
					pay_by_type[to_index(f_type.factory_workers.workers[i].type)] +=
						(life_needs_cost_by_type[to_index(f_type.factory_workers.workers[i].type)] * money_qnty_type(instance.worker_data.worker_populations[i]) / pop_needs_divisor)
						+ profit * money_qnty_type(0.2) * money_qnty_type(instance.worker_data.worker_populations[i]) / total_worker_pop;
				}
			}
			pay_by_type[to_index(f_type.factory_workers.owner.type)] += profit * money_qnty_type(0.8);
		}

		// rescale production
		// instance.worker_data.production_scale = std::clamp(instance.worker_data.production_scale * scale_speed(output_value / (min_wage + inputs_cost)), 0.05f, 1.0f);
	}

	constexpr money_qnty_type minimum_demand = money_qnty_type(0.000001);

	void update_demand_and_production(world_state& ws, nations::state_instance& si) {
		if(!(si.owner))
			return;

		const auto state_capital = nations::get_state_capital(ws, si);
		if(!state_capital)
			return;

		
		Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> current_state_demand(
			ws.w.nation_s.state_demand.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1)),
			ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> current_state_production(
			ws.w.nation_s.state_production.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * (to_index(ws.w.current_date) & 1)),
			ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> old_state_prices(
			ws.w.nation_s.state_prices.get_row(si.id) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1))), 
			ws.s.economy_m.aligned_32_goods_count);
		const float mobilization_effect = ((si.owner->flags & nations::nation::is_mobilized) == 0) ?
			1.0f : std::max(0.0f, 1.0f - si.owner->modifier_values[modifiers::national_offsets::mobilisation_size] * si.owner->modifier_values[modifiers::national_offsets::mobilisation_economy_impact]);


		current_state_demand.setZero();
		current_state_production.setZero();

		Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>> masked_prices(
			(economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count),
			ws.s.economy_m.aligned_32_goods_count);
		create_masked_prices(masked_prices, ws, si);



		economy::money_qnty_type* life_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		economy::money_qnty_type* everyday_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		economy::money_qnty_type* luxury_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);

		fill_needs_costs_arrays(ws, si, *state_capital, masked_prices, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type);

		nations::for_each_province(ws, si, [&ws, &current_state_demand, &current_state_production, &masked_prices, &si, &old_state_prices,
			life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, mobilization_effect](provinces::province_state& ps) {

			apply_pop_consumption(ws, ps, current_state_demand, masked_prices, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type);

			economy::money_qnty_type* province_pay_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
			std::fill_n(province_pay_by_type, ws.s.population_m.count_poptypes, economy::money_qnty_type(0));
			auto province_population_by_type = ws.w.province_s.province_demographics.get_row(ps.id) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));

			auto& rgo_type = ((ws.s.economy_m.goods[ps.rgo_production].flags & good_definition::mined) != 0) ? ws.s.economy_m.rgo_mine : ws.s.economy_m.rgo_farm;

			update_rgo_production(ws,
				si,
				province_pay_by_type,
				current_state_production,
				old_state_prices[to_index(ps.rgo_production)],
				life_needs_cost_by_type,
				ps, rgo_type, mobilization_effect);
			update_artisan_production(ws,
				si,
				province_pay_by_type,
				current_state_production,
				current_state_demand,
				old_state_prices,
				life_needs_cost_by_type,
				ps, mobilization_effect);

			update_rgo_employment(ws, ps);

			provinces::for_each_pop(ws, ps, [province_population_by_type, province_pay_by_type, &ws](population::pop& po) {
				po.money += province_pay_by_type[to_index(po.type)] * float(ws.w.population_s.pop_demographics.get(po.id, population::total_population_tag)) / float(province_population_by_type[to_index(po.type)]);
			});
		});

		economy::money_qnty_type* state_pay_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		std::fill_n(state_pay_by_type, ws.s.population_m.count_poptypes, economy::money_qnty_type(0));
		auto state_population_by_type = ws.w.nation_s.state_demographics.get_row(si.id) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));

		for(auto& f : si.factories) {
			if(factory_is_open(f)) {
				update_factory_production(ws,
					si,
					state_pay_by_type,
					current_state_production,
					current_state_demand,
					old_state_prices,
					life_needs_cost_by_type,
					f, *state_capital, mobilization_effect);
			}
		}

		update_factories_employment(ws, si);

		nations::for_each_pop(ws, si, [state_population_by_type, state_pay_by_type, &ws](population::pop& po) {
			po.money += state_pay_by_type[to_index(po.type)] * float(ws.w.population_s.pop_demographics.get(po.id, population::total_population_tag)) / float(state_population_by_type[to_index(po.type)]);
		});

		current_state_demand = current_state_demand.array().max(minimum_demand).matrix();
	}

	void economy_update_tick(world_state& ws) {
#ifdef DEBUG_ECONOMY
		std::fill(ws.w.economy_s.world_other_consumption.begin(), ws.w.economy_s.world_other_consumption.end(), goods_qnty_type(0));
		std::fill(ws.w.economy_s.world_other_production.begin(), ws.w.economy_s.world_other_production.end(), goods_qnty_type(0));
		std::fill(ws.w.economy_s.world_pop_consumption.begin(), ws.w.economy_s.world_pop_consumption.end(), goods_qnty_type(0));
		std::fill(ws.w.economy_s.world_rgo_production.begin(), ws.w.economy_s.world_rgo_production.end(), goods_qnty_type(0));
#endif
		ws.w.nation_s.states.parallel_for_each([&ws](nations::state_instance& si) {
			update_demand_and_production(ws, si);
		});
		concurrency::parallel_for(1ui32, ws.s.economy_m.goods_count, [&ws](uint32_t i) {
			economy_single_good_tick(ws, goods_tag(goods_tag::value_base_t(i)));
		});
#ifdef DEBUG_ECONOMY
		for(uint32_t i = 1; i < ws.s.economy_m.goods_count; ++i) {
			goods_tag this_tag = goods_tag(goods_tag::value_base_t(i));
			auto name = text_data::to_string(ws.s.gui_m.text_data_sequences, ws.s.economy_m.goods[this_tag].name);
			OutputDebugStringW((LPCWSTR)name.c_str());
			OutputDebugStringA("\r\n");
			OutputDebugStringA("rgo production: ");
			OutputDebugStringA(std::to_string(ws.w.economy_s.world_rgo_production[this_tag]).c_str());
			OutputDebugStringA("\r\n");
			OutputDebugStringA("other production: ");
			OutputDebugStringA(std::to_string(ws.w.economy_s.world_other_production[this_tag]).c_str());
			OutputDebugStringA("\r\n");
			OutputDebugStringA("pop consumption: ");
			OutputDebugStringA(std::to_string(ws.w.economy_s.world_pop_consumption[this_tag]).c_str());
			OutputDebugStringA("\r\n");
			OutputDebugStringA("other consumption: ");
			OutputDebugStringA(std::to_string(ws.w.economy_s.world_other_consumption[this_tag]).c_str());
			OutputDebugStringA("\r\n");
			OutputDebugStringA("\r\n");
		}
#endif
	}

	artisan_type_tag get_profitable_artisan(world_state const& ws, provinces::province_state const& ps) {
		boost::container::small_vector<std::pair<artisan_type_tag, money_qnty_type>, 32, concurrent_allocator<std::pair<artisan_type_tag, money_qnty_type>>> result;

		auto owner = ps.owner;
		auto in_state = ps.state_instance;
		
		if(!owner || !in_state)
			return artisan_type_tag(0);
		auto owner_id = owner->id;
		if(!ws.w.nation_s.nations.is_valid_index(owner_id))
			return artisan_type_tag(0);

		auto prices = state_current_prices(ws, *in_state);
		auto artisan_life_needs = get_life_needs_cost(ws, *in_state, ws.s.population_m.artisan);
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(owner_id);

		money_qnty_type profit_sum = money_qnty_type(0);
		for(auto& atype : ws.s.economy_m.artisan_types) {
			if(bit_vector_test(enabled_goods, to_index(atype.output_good))) {
				auto profit = artisan_profit_unit(ws, ps, *owner, atype, prices, artisan_life_needs);
				if(profit > money_qnty_type(0)) {
					profit_sum += profit;
					result.emplace_back(atype.id, profit_sum);
				}
			}
		}

		std::uniform_real_distribution<money_qnty_type> dist(money_qnty_type(0), profit_sum);
		auto chosen = dist(get_local_generator());

		auto lb = std::lower_bound(result.begin(), result.end(), chosen, [](std::pair<artisan_type_tag, money_qnty_type> p, money_qnty_type v) { return p.second < v; });
		if(lb != result.end())
			return lb->first;

		// list of profitable artisan types was empty
		boost::container::small_vector<artisan_type_tag, 32, concurrent_allocator<artisan_type_tag>> result_b;
		for(auto& atype : ws.s.economy_m.artisan_types) {
			if(bit_vector_test(enabled_goods, to_index(atype.output_good))) {
				result_b.push_back(atype.id);
			}
		}
		std::uniform_int_distribution<int32_t> d(0, int32_t(result_b.size() - 1));
		return result_b[d(get_local_generator())];
	}
}
