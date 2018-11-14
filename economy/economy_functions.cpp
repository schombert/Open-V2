#include "common\\common.h"
#include "economy_functions.hpp"
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

		Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices(state_current_prices(ws, si.id), ws.s.economy_m.aligned_32_goods_count);
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
		return bool(fi.type) && fi.level != 0 && fi.factory_progress > 0.0f && fi.worker_data.production_scale > 0.0f;
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

	template<bool adjust_money>
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

			auto initial_money = po.money;
			auto pop_strata = ws.s.population_m.pop_types[po.type].flags & population::pop_type::strata_mask;

			
			float pop_size_multiplier = float(ws.w.population_s.pop_demographics.get(po.id, population::total_population_tag)) / pop_needs_divisor;

			const auto ln_money = pop_size_multiplier * 11.0f;
			const auto en_money = pop_size_multiplier *
				[pop_strata, is_slave = (ws.s.population_m.slave == po.type), is_capitalist = (ws.s.population_m.capitalist == po.type)](){
				if(is_slave)
					return 1.0f; //2.3f;
				if(pop_strata == population::pop_type::strata_poor)
					return 25.0f; //55.0f;
				if(pop_strata == population::pop_type::strata_middle)
					return 55.0f; // 110.0f;
				if(is_capitalist)
					return 350.0f; //700.0f;
				return 300.0f; // 600.0f;
			}();
			const auto lx_money = pop_size_multiplier *
				[pop_strata, is_slave = (ws.s.population_m.slave == po.type), is_capitalist = (ws.s.population_m.capitalist == po.type)](){
				if(is_slave)
					return 1.5f; // 3.2f;
				if(pop_strata == population::pop_type::strata_poor)
					return 40.0f; // 80.0f;
				if(pop_strata == population::pop_type::strata_middle)
					return 125.0f; // 250.0f;
				if(is_capitalist)
					return 1000.0f; // 2000.0f;
				return 800.0f; // 1500.0f;
			}();

			Eigen::Map<Eigen::Array<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> ln(
				ws.s.population_m.life_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Array<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> en(
				ws.s.population_m.everyday_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Array<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> xn(
				ws.s.population_m.luxury_needs.get_row(po.type), ws.s.economy_m.aligned_32_goods_count);

			
			auto enabled_goods = ws.w.nation_s.active_goods.get_row(p.owner->id);
			const auto total_ln = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<>(),
				[enabled_goods, &ln](int32_t i) { return bit_vector_test(enabled_goods, i) ? ln[i] : goods_qnty_type(0); });
			const auto total_en = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<>(),
				[enabled_goods, &en](int32_t i) { return bit_vector_test(enabled_goods, i) ? en[i] : goods_qnty_type(0); });
			const auto total_xn = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<>(),
				[enabled_goods, &xn](int32_t i) { return bit_vector_test(enabled_goods, i) ? xn[i] : goods_qnty_type(0); });
			
			if(total_ln != 0)
				state_demand += (ln * ln_money / total_ln).matrix();
			state_demand += (en * en_money / total_en).matrix();
			state_demand += (xn * lx_money / total_xn).matrix();

			if(initial_money < (pop_size_multiplier * life_needs_cost_by_type[to_index(po.type)])) {
				// case: can only partially satisfy life needs
				auto fraction = initial_money / (pop_size_multiplier * life_needs_cost_by_type[to_index(po.type)]);
				po.needs_satisfaction = fraction;
				
				if constexpr(adjust_money) {
					po.money = 0.0f;
				}
			} else {
				initial_money -= pop_size_multiplier * life_needs_cost_by_type[to_index(po.type)];

				if(initial_money < pop_size_multiplier * everyday_needs_cost_by_type[to_index(po.type)]) {
					// case: can partially satisfy everyday needs

					auto fraction = initial_money / (pop_size_multiplier * everyday_needs_cost_by_type[to_index(po.type)]);
					po.needs_satisfaction = 1.0f + fraction;

					if constexpr(adjust_money) {
						po.money = 0.0f;
					}
				} else {
					// case: can fully satisfy everyday needs
					state_demand += (en * en_money / total_en).matrix();

					initial_money -= pop_size_multiplier * everyday_needs_cost_by_type[to_index(po.type)];

					// remainder of money spent on luxury needs
					auto last_fraction = initial_money * (money_qnty_type(1) - savings_rate) / (pop_size_multiplier * luxury_needs_cost_by_type[to_index(po.type)]);
					last_fraction = last_fraction > 1.0f ? (1.0f + (last_fraction - 1.0f) * 0.1f) : last_fraction;
					po.needs_satisfaction = 2.0f + last_fraction;

					if constexpr(adjust_money) {
						po.money = initial_money * savings_rate;
					}
				}
			}

			
		});
	}

	void create_masked_prices(Eigen::Map <Eigen::Matrix<economy::money_qnty_type, -1, 1>>& __restrict masked_prices,
		world_state const& ws, nations::state_instance const& si) {

		auto state_prices = state_current_prices(ws, si.id);

		masked_prices.setZero();
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(si.owner->id);
		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			masked_prices[i] = bit_vector_test(enabled_goods, i) ? state_prices[i] : economy::money_qnty_type(0);
		}
	}

	

	struct needs_values {
		money_qnty_type life_needs = 0;
		money_qnty_type everyday_needs = 0;
		money_qnty_type luxury_needs = 0;
	};

	needs_values needs_for_poptype(world_state const& ws, population::pop_type_tag type, nations::state_instance const& si) {
		auto state_cap = nations::get_state_capital(ws, si);
		auto state_owner = si.owner;

		if(!state_cap || !state_owner)
			return needs_values{ 0,0,0 };

		auto owner_id = state_owner->id;
		auto state_id = si.id;
		if(!ws.w.nation_s.nations.is_valid_index(owner_id) || !ws.w.nation_s.states.is_valid_index(state_id))
			return needs_values{ 0,0,0 };

		needs_values result{ 0,0,0 };
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(owner_id);
		auto this_strata = ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask;

		economy::money_qnty_type ln_factor;
		economy::money_qnty_type ev_factor;
		economy::money_qnty_type lx_factor;

		if(this_strata == population::pop_type::strata_poor) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::poor_life_needs] + state_cap->modifier_values[modifiers::provincial_offsets::poor_life_needs];
			ev_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::poor_everyday_needs] + state_cap->modifier_values[modifiers::provincial_offsets::poor_everyday_needs];
			lx_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::poor_luxury_needs] + state_cap->modifier_values[modifiers::provincial_offsets::poor_luxury_needs];
		} else if(this_strata == population::pop_type::strata_middle) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::middle_life_needs] + state_cap->modifier_values[modifiers::provincial_offsets::middle_life_needs];
			ev_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::middle_everyday_needs] + state_cap->modifier_values[modifiers::provincial_offsets::middle_everyday_needs];
			lx_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::middle_luxury_needs] + state_cap->modifier_values[modifiers::provincial_offsets::middle_luxury_needs];
		} else { //if(this_strata == population::pop_type::strata_rich) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::rich_life_needs] + state_cap->modifier_values[modifiers::provincial_offsets::rich_life_needs];
			ev_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::rich_everyday_needs] + state_cap->modifier_values[modifiers::provincial_offsets::rich_everyday_needs];
			lx_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::rich_luxury_needs] + state_cap->modifier_values[modifiers::provincial_offsets::rich_luxury_needs];
		}

		auto ln_goods = ws.s.population_m.life_needs.get_row(type);
		auto en_goods = ws.s.population_m.everyday_needs.get_row(type);
		auto lx_goods = ws.s.population_m.luxury_needs.get_row(type);

		auto state_prices = state_current_prices(ws, state_id);

		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			if(bit_vector_test(enabled_goods, i)) {
				result.life_needs += ln_factor * state_prices[i] * ln_goods[i];
				result.everyday_needs += ev_factor * state_prices[i] * en_goods[i];
				result.luxury_needs += lx_factor * state_prices[i] * lx_goods[i];
			}
		}

		return result;
	}

	needs_values needs_for_poptype(world_state const& ws, population::pop_type_tag type, nations::nation const& n) {
		auto ncap = n.current_capital;
		if(!is_valid_index(ncap))
			return needs_values{ 0,0,0 };
		auto ncap_state = ws.w.province_s.province_state_container[ncap].state_instance;
		if(!ncap_state)
			return needs_values{ 0,0,0 };
		return needs_for_poptype(ws, type, *ncap_state);
	}

	money_qnty_type get_life_needs_cost(world_state const& ws, nations::state_instance const& si, population::pop_type_tag ptype) {
		auto state_owner = si.owner;
		auto state_capital = nations::get_state_capital(ws, si);
		if(!state_owner || !state_capital)
			return money_qnty_type(0);
		auto owner_id = state_owner->id;
		auto state_id = si.id;
		if(!ws.w.nation_s.nations.is_valid_index(owner_id) || !ws.w.nation_s.states.is_valid_index(state_id))
			return money_qnty_type(0);


		auto this_strata = ws.s.population_m.pop_types[ptype].flags & population::pop_type::strata_mask;
		economy::money_qnty_type ln_factor;

		if(this_strata == population::pop_type::strata_poor) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::poor_life_needs] + state_capital->modifier_values[modifiers::provincial_offsets::poor_life_needs];
		} else if(this_strata == population::pop_type::strata_middle) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::middle_life_needs] + state_capital->modifier_values[modifiers::provincial_offsets::middle_life_needs];
		} else { //if(this_strata == population::pop_type::strata_rich) {
			ln_factor = economy::money_qnty_type(1) + state_owner->modifier_values[modifiers::national_offsets::rich_life_needs] + state_capital->modifier_values[modifiers::provincial_offsets::rich_life_needs];
		}

		money_qnty_type sum = 0;
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(owner_id);
		auto ln_goods = ws.s.population_m.life_needs.get_row(ptype);
		auto state_prices = state_current_prices(ws, state_id);

		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			if(bit_vector_test(enabled_goods, i)) {
				sum += ln_factor * state_prices[i] * ln_goods[i];
			}
		}

		return sum;
	}

	money_qnty_type get_life_needs_cost(world_state const& ws, population::pop_type_tag type, nations::nation const& n) {
		auto ncap = n.current_capital;
		if(!is_valid_index(ncap))
			return money_qnty_type(0);
		auto ncap_state = ws.w.province_s.province_state_container[ncap].state_instance;
		if(!ncap_state)
			return money_qnty_type(0);
		return get_life_needs_cost(ws, *ncap_state, type);
	}

	void fill_needs_costs_arrays(world_state const& ws, nations::state_instance const& si, provinces::province_state const& state_capital,
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

			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> ln(ws.s.population_m.life_needs.get_row(this_type), ws.s.economy_m.aligned_32_goods_count);
			life_needs_cost_by_type[i] = ln_factor * masked_prices.dot(ln);
			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> en(ws.s.population_m.everyday_needs.get_row(this_type), ws.s.economy_m.aligned_32_goods_count);
			everyday_needs_cost_by_type[i] = ev_factor * masked_prices.dot(en);
			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> xn(ws.s.population_m.luxury_needs.get_row(this_type), ws.s.economy_m.aligned_32_goods_count);
			luxury_needs_cost_by_type[i] = lx_factor * masked_prices.dot(xn);
		}
	}

	money_qnty_type* state_current_prices(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_prices.get_row(s);
	}

	money_qnty_type* state_price_delta(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_prices.get_row(s) + ws.s.economy_m.aligned_32_goods_count;
	}

	goods_qnty_type* state_current_production(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_production.get_row(s) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1)));
	}

	goods_qnty_type* state_old_production(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_production.get_row(s) + (ws.s.economy_m.aligned_32_goods_count * ((to_index(ws.w.current_date) & 1)));
	}

	money_qnty_type* state_current_demand(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_demand.get_row(s) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1)));
	}

	money_qnty_type calculate_daily_debt_payment(world_state const & ws, nations::nation const & n) {
		return n.national_debt * economy::daily_debt_payment;
	}

	money_qnty_type* state_old_demand(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_demand.get_row(s) + (ws.s.economy_m.aligned_32_goods_count * ((to_index(ws.w.current_date) & 1)));
	}

	money_qnty_type* state_current_global_demand(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_global_demand.get_row(s) + (ws.s.economy_m.aligned_32_goods_count * (1ui32 - (to_index(ws.w.current_date) & 1)));
	}

	money_qnty_type* state_old_global_demand(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_global_demand.get_row(s) + (ws.s.economy_m.aligned_32_goods_count * ((to_index(ws.w.current_date) & 1)));
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

	range_information global_price_range(world_state const& ws, economy::goods_tag t) {
		range_information result{ std::numeric_limits<money_qnty_type>::max(), std::numeric_limits<money_qnty_type>::lowest(), 0.0f };
		int32_t count = 0;
		ws.w.nation_s.states.for_each([&ws, t, &result, &count](nations::state_instance const& si) {
			if(auto sid = si.id; si.owner && ws.w.nation_s.states.is_valid_index(sid)) {
				auto prices = state_current_prices(ws, sid);
				auto v = prices[to_index(t)];
				result.minimum = std::min(result.minimum, v);
				result.maximum = std::max(result.maximum, v);
				result.average += v;
				++count;
			}
		});
		result.average /= float(count);
		return result;
	}



	money_qnty_type daily_state_owner_building_cost(world_state const& ws, nations::state_instance const& si) {
		auto id = si.id;
		if(!ws.w.nation_s.states.is_valid_index(id))
			return money_qnty_type(0);

		Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices_v(
			state_current_prices(ws, id), ws.s.economy_m.aligned_32_goods_count);

		Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> rr_costs(
			ws.s.economy_m.building_costs.get_row(ws.s.economy_m.railroad.cost_tag), ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> fort_costs(
			ws.s.economy_m.building_costs.get_row(ws.s.economy_m.fort.cost_tag), ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> nb_costs(
			ws.s.economy_m.building_costs.get_row(ws.s.economy_m.naval_base.cost_tag), ws.s.economy_m.aligned_32_goods_count);

		auto rr_daily_cost = prices_v.dot(rr_costs) / money_qnty_type(ws.s.economy_m.railroad.time);
		auto fort_daily_cost = prices_v.dot(fort_costs) / money_qnty_type(ws.s.economy_m.fort.time);
		auto nb_daily_cost = prices_v.dot(nb_costs) + money_qnty_type(ws.s.economy_m.naval_base.extra_cost) / money_qnty_type(ws.s.economy_m.naval_base.time);

		money_qnty_type sum = 0;
		nations::for_each_province(ws, si, [&sum, rr_daily_cost, fort_daily_cost, nb_daily_cost](provinces::province_state const& ps) {
			sum += (ps.fort_upgrade_progress > 0.0f ? fort_daily_cost : money_qnty_type(0)) +
				(ps.naval_base_upgrade_progress > 0.0f ? nb_daily_cost : money_qnty_type(0)) +
				((ps.railroad_upgrade_progress > 0.0f && (ps.flags & provinces::province_state::owner_building_railroad) != 0) ? rr_daily_cost : money_qnty_type(0));
		});

		return std::transform_reduce(std::begin(si.factories), std::end(si.factories), sum, std::plus<>(), [&prices_v, &ws](factory_instance const& f) {
			if(auto type = f.type; type && f.factory_progress > 0.0f && (f.flags & factory_instance::owner_is_upgrading) != 0) {
				Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> costs(
					ws.s.economy_m.building_costs.get_row(type->id), ws.s.economy_m.aligned_32_goods_count);
				return prices_v.dot(costs) / money_qnty_type(type->building_time);
			} else {
				return money_qnty_type(0);
			}
		});
	}
	money_qnty_type daily_national_building_cost(world_state const& ws, nations::nation const& n) {
		money_qnty_type sum = 0;
		nations::for_each_state(ws, n, [&ws, &sum](nations::state_instance const& si) {
			sum += daily_state_owner_building_cost(ws, si);
		});
		return sum;
	}

	namespace {
		template<typename T>
		class cbacked_eigen_vector {
		protected:
			std::vector<T, concurrent_allocator<T>> data_backing;
		public:
			Eigen::Map<Eigen::Matrix<T, 1, -1>, Eigen::Aligned32> vector;

			cbacked_eigen_vector(int32_t aligned_32_size) : data_backing(aligned_32_size + 32 / sizeof(T), T(0)), vector(nullptr, 1) {
				{
					size_t space = aligned_32_size * sizeof(T) + 32;
					void* ptr = data_backing.data();
					new (&vector) Eigen::Map<Eigen::Matrix<T, 1, -1>, Eigen::Aligned32>(
						(T*)std::align(32, aligned_32_size * sizeof(T), ptr, space),
						aligned_32_size);
				}
			};
			cbacked_eigen_vector(cbacked_eigen_vector const& other) = delete;
			cbacked_eigen_vector(cbacked_eigen_vector&& other) : data_backing(std::move(other.data_backing)), vector(nullptr, 1) {
				new (&vector) Eigen::Map<Eigen::Matrix<T, 1, -1>, Eigen::Aligned32>(
					other.vector.data(),
					other.vector.size());
			}
		};

		template<typename T>
		class cbacked_eigen_vector_generator {
		public:
			const int32_t size;

			cbacked_eigen_vector_generator(int32_t s) : size(int32_t(((s * sizeof(T) + 31) / 32) / sizeof(T))) {}

			auto operator()() const noexcept {
				return cbacked_eigen_vector<T>(size);
			}
		};
	}

	void update_construction_and_projects(world_state& ws) {
		int32_t nations_count = int32_t(ws.w.nation_s.nations.minimum_continuous_size());
		concurrency::combinable<cbacked_eigen_vector<money_qnty_type>> costs_by_nation = concurrency::combinable<cbacked_eigen_vector<money_qnty_type>>(cbacked_eigen_vector_generator<money_qnty_type>(nations_count));

		auto rr_time = 1.0f / money_qnty_type(ws.s.economy_m.railroad.time);
		auto fort_time = 1.0f / money_qnty_type(ws.s.economy_m.fort.time);
		auto naval_time = 1.0f / money_qnty_type(ws.s.economy_m.naval_base.time);

		ws.w.nation_s.states.parallel_for_each([&ws, &costs_by_nation, rr_time, fort_time, naval_time](nations::state_instance& si) {
			if(!si.owner)
				return;

			auto& local_nation_costs = costs_by_nation.local();

			Eigen::Map<const Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::Aligned32> prices_v(
				state_current_prices(ws, si.id), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> current_state_demand(
				state_old_demand(ws, si.id),
				ws.s.economy_m.aligned_32_goods_count);

			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> rr_costs(
				ws.s.economy_m.building_costs.get_row(ws.s.economy_m.railroad.cost_tag), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> fort_costs(
				ws.s.economy_m.building_costs.get_row(ws.s.economy_m.fort.cost_tag), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> nb_costs(
				ws.s.economy_m.building_costs.get_row(ws.s.economy_m.naval_base.cost_tag), ws.s.economy_m.aligned_32_goods_count);

			
			auto rr_daily_cost = prices_v.dot(rr_costs) * rr_time;
			auto fort_daily_cost = prices_v.dot(fort_costs) * fort_time;
			auto nb_daily_cost = prices_v.dot(nb_costs) + money_qnty_type(ws.s.economy_m.naval_base.extra_cost) * naval_time;

			auto owner_sp_setting = float(si.owner->projects_stockpile_spending) / 100.0f;
			nations::for_each_province(ws, si, [rr_daily_cost, fort_daily_cost, nb_daily_cost, rr_time, fort_time, naval_time, &current_state_demand,
				&local_nation_costs, owner_sp_setting, &rr_costs, &fort_costs, &nb_costs, &prices_v](provinces::province_state& ps) {
				
				if(ps.fort_upgrade_progress > 0.0f) {
					local_nation_costs.vector[to_index(ps.owner->id)] += fort_daily_cost * owner_sp_setting;
					ps.fort_upgrade_progress += fort_time * owner_sp_setting;
					current_state_demand += ((fort_costs.array() * prices_v.array()) * fort_time * owner_sp_setting).matrix();
					if(ps.fort_upgrade_progress >= 1.0f) {
						ps.fort_upgrade_progress = 0;
						ps.fort_level++;
					}
				}
				if(ps.naval_base_upgrade_progress > 0.0f) {
					local_nation_costs.vector[to_index(ps.owner->id)] += nb_daily_cost * owner_sp_setting;
					ps.naval_base_upgrade_progress += naval_time * owner_sp_setting;
					current_state_demand += ((nb_costs.array() * prices_v.array()) * naval_time * owner_sp_setting).matrix();
					if(ps.naval_base_upgrade_progress >= 1.0f) {
						ps.naval_base_upgrade_progress = 0;
						ps.naval_base_level++;
					}
				}
				if(ps.railroad_upgrade_progress > 0.0f && (ps.flags & provinces::province_state::owner_building_railroad) != 0) {
					local_nation_costs.vector[to_index(ps.owner->id)] += rr_daily_cost * owner_sp_setting;
					ps.railroad_upgrade_progress += rr_time * owner_sp_setting;
					current_state_demand += ((rr_costs.array() * prices_v.array()) * rr_time * owner_sp_setting).matrix();
					if(ps.railroad_upgrade_progress >= 1.0f) {
						ps.railroad_upgrade_progress = 0;
						ps.railroad_level++;
					}
				}
			});

			for(auto& f : si.factories) {
				if(auto type = f.type; type && f.factory_progress > 0.0f && (f.flags & factory_instance::owner_is_upgrading) != 0) {
					Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> costs(
						ws.s.economy_m.building_costs.get_row(type->id), ws.s.economy_m.aligned_32_goods_count);

					auto time = 1.0f / money_qnty_type(type->building_time);
					local_nation_costs.vector[to_index(si.owner->id)] += prices_v.dot(costs) * owner_sp_setting * time;
					current_state_demand += ((costs.array() * prices_v.array()) * time * owner_sp_setting).matrix();

					f.factory_progress += time;
					if(f.factory_progress >= 1.0f) {
						f.factory_progress = 0;
						f.level++;
					}
				}
			}


			// update pop projects
			switch(si.project.type) {
				default:
				case pop_project_type::none:
					break;
				case pop_project_type::railroad:
				{
					auto& ps = ws.w.province_s.province_state_container[si.project.location];
					if(si.project.funds > rr_daily_cost) {
						ps.railroad_upgrade_progress += rr_time;
						current_state_demand += ((rr_costs.array() * prices_v.array()) * rr_time).matrix();
						si.project.funds -= rr_daily_cost;
					} else {
						ps.railroad_upgrade_progress += rr_time * si.project.funds / rr_daily_cost;
						current_state_demand += ((rr_costs.array() * prices_v.array()) * rr_time * si.project.funds / rr_daily_cost).matrix();
						si.project.funds = 0;
					}
					if(ps.railroad_upgrade_progress >= 1.0f) {
						si.project.type = pop_project_type::none;
						ps.railroad_upgrade_progress = 0;
						ps.railroad_level++;
					}
				}
					break;
				case pop_project_type::factory:
				{
					auto f = std::find_if(std::begin(si.factories), std::end(si.factories),
						[t = si.project.factory_type](factory_instance const& fi) { return bool(fi.type) && (fi.type->id == t); });
					if(f == std::end(si.factories))
						break;

					if(f->factory_progress > 0.0f) {
						Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> costs(
							ws.s.economy_m.building_costs.get_row(si.project.factory_type), ws.s.economy_m.aligned_32_goods_count);

						auto time = 1.0f / money_qnty_type(f->type->building_time);
						auto daily_cost = prices_v.dot(costs) * time;

						if(si.project.funds > daily_cost) {
							f->factory_progress += time;
							current_state_demand += ((rr_costs.array() * prices_v.array()) * time).matrix();
							si.project.funds -= daily_cost;
						} else {
							f->factory_progress += time * si.project.funds / daily_cost;
							current_state_demand += ((rr_costs.array() * prices_v.array()) * time * si.project.funds / daily_cost).matrix();
							si.project.funds = 0;
						}

						if(f->factory_progress >= 1.0f) {
							si.project.type = pop_project_type::none;
							f->factory_progress = 0;
							f->level++;
						}
					} else if(f->worker_data.production_scale <= 0) {
						Eigen::Map<const Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::Aligned32> costs(
							ws.s.economy_m.factory_input_goods.get_row(si.project.factory_type), ws.s.economy_m.aligned_32_goods_count);

						auto reopen_cost = prices_v.dot(costs) * money_qnty_type(10);
						if(si.project.funds >= reopen_cost) {
							f->worker_data.production_scale = 1.0f;
							si.project.type = pop_project_type::none;
						}
					}
				}
					break;
			}
		});

		costs_by_nation.combine_each([&ws, nations_count](cbacked_eigen_vector<money_qnty_type> const& v) {
			for(int32_t i = 0; i < nations_count; ++i) {
				auto t = ws.w.nation_s.national_stockpiles.get(nations::country_tag(nations::country_tag::value_base_t(i)), economy::money_good);
				if(auto val = v.vector[i]; val > 0) {
					if(t >= val) {
						ws.w.nation_s.national_stockpiles.get(nations::country_tag(nations::country_tag::value_base_t(i)), economy::money_good) = t - val;
					} else {
						ws.w.nation_s.national_stockpiles.get(nations::country_tag(nations::country_tag::value_base_t(i)), economy::money_good) = 0;
						ws.w.nation_s.nations[nations::country_tag(nations::country_tag::value_base_t(i))].national_debt += val - t;
					}
				}
			}
		});
	}

	float distance_multiplier(float v) {
		return std::max(0.0f, 1.0f - 0.000045f * v);
	}

	constexpr int32_t price_update_delay = 2;
	constexpr money_qnty_type price_change_rate = money_qnty_type(0.025);
	//constexpr money_qnty_type price_gravity_factor = money_qnty_type(0.01);
	constexpr money_qnty_type purchasing_change_rate = money_qnty_type(1);
#define FULL_PURCHASING_CHANGE
	constexpr money_qnty_type distance_factor = money_qnty_type(0.001f);
	constexpr money_qnty_type minimum_global_purchase = money_qnty_type(0.000001f);
	constexpr money_qnty_type minimum_demand = money_qnty_type(0);
	constexpr money_qnty_type value_cutoff = money_qnty_type(0.0000001);

	namespace {
		class single_good_update_work_data_factory;

		class single_good_update_work_data {
		public:
			std::vector<economy::money_qnty_type, concurrent_allocator<economy::money_qnty_type>> sscratch_a_v;
			std::vector<economy::money_qnty_type, concurrent_allocator<economy::money_qnty_type>> sscratch_b_v;
			std::vector<economy::money_qnty_type, concurrent_allocator<economy::money_qnty_type>> tarrifs_v;
			std::vector<economy::money_qnty_type, concurrent_allocator<economy::money_qnty_type>> global_demand_by_state_v;

			std::vector<economy::money_qnty_type, concurrent_allocator<economy::money_qnty_type>> player_imports_v;
			std::vector<economy::money_qnty_type, concurrent_allocator<economy::money_qnty_type>> nation_tarrif_income_v;

			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> weightings;
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> apparent_price;
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> tarrifs;
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> global_demand_by_state;
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> player_imports;
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> nation_tarrif_income;
		protected:
			single_good_update_work_data(int32_t state_count, int32_t nations_count) :
				sscratch_a_v(state_count + 32 / sizeof(economy::money_qnty_type), economy::money_qnty_type(0)),
				sscratch_b_v(state_count + 32 / sizeof(economy::money_qnty_type), economy::money_qnty_type(0)),
				tarrifs_v(state_count + 32 / sizeof(economy::money_qnty_type), economy::money_qnty_type(0)),
				global_demand_by_state_v(state_count + 32 / sizeof(economy::money_qnty_type), economy::money_qnty_type(0)),
				player_imports_v(nations_count + 32 / sizeof(economy::money_qnty_type), economy::money_qnty_type(0)),
				nation_tarrif_income_v(nations_count + 32 / sizeof(economy::money_qnty_type), economy::money_qnty_type(0)),
				weightings(nullptr, 1), apparent_price(nullptr, 1), tarrifs(nullptr, 1), global_demand_by_state(nullptr, 1), player_imports(nullptr, 1), nation_tarrif_income(nullptr, 1) {
					{
						size_t space = state_count * sizeof(economy::money_qnty_type) + 32;
						void* ptr = sscratch_a_v.data();
						new (&weightings) Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32>(
							(economy::money_qnty_type*)std::align(32, state_count * sizeof(economy::money_qnty_type), ptr, space),
							state_count);
					}
					{
						size_t space = state_count * sizeof(economy::money_qnty_type) + 32;
						void* ptr = sscratch_b_v.data();
						new (&apparent_price) Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32>(
							(economy::money_qnty_type*)std::align(32, state_count * sizeof(economy::money_qnty_type), ptr, space),
							state_count);
					}
					{
						size_t space = state_count * sizeof(economy::money_qnty_type) + 32;
						void* ptr = tarrifs_v.data();
						new (&tarrifs) Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32>(
							(economy::money_qnty_type*)std::align(32, state_count * sizeof(economy::money_qnty_type), ptr, space),
							state_count);
					}
					{
						size_t space = state_count * sizeof(economy::money_qnty_type) + 32;
						void* ptr = global_demand_by_state_v.data();
						new (&global_demand_by_state) Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32>(
							(economy::money_qnty_type*)std::align(32, state_count * sizeof(economy::money_qnty_type), ptr, space),
							state_count);
					}
					{
						size_t space = nations_count * sizeof(economy::money_qnty_type) + 32;
						void* ptr = player_imports_v.data();
						new (&player_imports) Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32>(
							(economy::money_qnty_type*)std::align(32, nations_count * sizeof(economy::money_qnty_type), ptr, space),
							nations_count);
					}
					{
						size_t space = nations_count * sizeof(economy::money_qnty_type) + 32;
						void* ptr = nation_tarrif_income_v.data();
						new (&nation_tarrif_income) Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32>(
							(economy::money_qnty_type*)std::align(32, nations_count * sizeof(economy::money_qnty_type), ptr, space),
							nations_count);
					}
			}
		public:
			single_good_update_work_data() = delete;
			single_good_update_work_data(const single_good_update_work_data& other) = delete;
			single_good_update_work_data(single_good_update_work_data&& other) noexcept :
				sscratch_a_v(std::move(other.sscratch_a_v)),
				sscratch_b_v(std::move(other.sscratch_b_v)),
				tarrifs_v(std::move(other.tarrifs_v)),
				global_demand_by_state_v(std::move(other.global_demand_by_state_v)),
				player_imports_v(std::move(other.player_imports_v)),
				nation_tarrif_income_v(std::move(other.nation_tarrif_income_v)),
				weightings(other.weightings.data(), other.weightings.size()),
				apparent_price(other.apparent_price.data(), other.apparent_price.size()),
				tarrifs(other.tarrifs.data(), other.tarrifs.size()),
				global_demand_by_state(other.global_demand_by_state.data(), other.global_demand_by_state.size()),
				player_imports(other.player_imports.data(), other.player_imports.size()), 
				nation_tarrif_income(other.nation_tarrif_income.data(), other.nation_tarrif_income.size()) {

			}

			friend single_good_update_work_data_factory;
		};

		class single_good_update_work_data_factory {
		public:
			const int32_t state_count;
			const int32_t nations_count;

			single_good_update_work_data_factory(int32_t s, int32_t n) : state_count(s), nations_count(n) {}

			single_good_update_work_data operator()() const noexcept {
				return single_good_update_work_data(state_count, nations_count);
			}
		};

	}

	void economy_single_good_tick(world_state& ws, goods_tag tag, int32_t state_max, int32_t nations_max) {
		auto aligned_state_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(state_max) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));
		auto aligned_nations_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(nations_max) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));

		const auto base_price = ws.s.economy_m.goods[tag].base_price;

		concurrency::combinable<single_good_update_work_data> workspace(single_good_update_work_data_factory(
			aligned_state_max, // state aligned size
			aligned_nations_max) // nations aligned size
		);

		cbacked_eigen_vector<economy::money_qnty_type> global_demand_by_state(aligned_state_max);
		cbacked_eigen_vector<economy::money_qnty_type> nation_tarrif_income(aligned_nations_max);
		cbacked_eigen_vector<economy::money_qnty_type> player_imports(aligned_nations_max);

		cbacked_eigen_vector<economy::money_qnty_type> state_production_copy(aligned_state_max);
		concurrency::parallel_for(0, state_max, [&ws, &state_production_copy, tag](int32_t i) {
			state_production_copy.vector[i] = state_old_production(ws, nations::state_tag(nations::state_tag::value_base_t(i)))[to_index(tag)];
		}, concurrency::static_partitioner());
		cbacked_eigen_vector<economy::money_qnty_type> state_prices_copy(aligned_state_max);
		concurrency::parallel_for(0, state_max, [&ws, &state_prices_copy, tag](int32_t i) {
			state_prices_copy.vector[i] = state_current_prices(ws, nations::state_tag(nations::state_tag::value_base_t(i)))[to_index(tag)];
		}, concurrency::static_partitioner());
		for(int32_t j = state_max; j < int32_t(aligned_state_max); ++j)
			state_prices_copy.vector[j] = base_price;

		concurrency::parallel_for(0, state_max, [&ws, &workspace, &state_production_copy, &state_prices_copy, tag, base_price, aligned_state_max, state_max](int32_t state_index) {
			nations::state_instance const& si = ws.w.nation_s.states[nations::state_tag(nations::state_tag::value_base_t(state_index))];

			auto demand_in_state = state_old_demand(ws, si.id)[to_index(tag)];
			auto& workspace_local = workspace.local();

			if(si.owner == nullptr) { // skip remainder for this state
				state_current_prices(ws, si.id)[to_index(tag)] = base_price;
				resize(ws.w.economy_s.purchasing_arrays, ws.w.nation_s.state_purchases.get(si.id, tag), 0);
				return;
			}

			auto& purchases_for_state = ws.w.nation_s.state_purchases.get(si.id, tag);
			auto sz = get_size(ws.w.economy_s.purchasing_arrays, purchases_for_state);

			if(sz < aligned_state_max) {
				resize(ws.w.economy_s.purchasing_arrays, purchases_for_state, aligned_state_max);
			}

			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> values(
				get_range(ws.w.economy_s.purchasing_arrays, purchases_for_state).first, aligned_state_max);


			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> distance_vector(
				ws.w.province_s.state_distances.get_row(si.id),
				aligned_state_max);

			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> tarrif_mask(
				get_range(ws.w.economy_s.purchasing_arrays, si.owner->statewise_tarrif_mask).first,
				aligned_state_max);

#ifndef FULL_PURCHASING_CHANGE
			auto old_state_purchases = values.sum();

			if(old_state_purchases < money_qnty_type(0)) {
				values.setZero();
				values[to_index(si.id)] = money_qnty_type(1);
			} else if(old_state_purchases < money_qnty_type(1))
				values[to_index(si.id)] += money_qnty_type(1) - old_state_purchases;
			else if(old_state_purchases > money_qnty_type(1))
				values /= old_state_purchases;
#endif

			workspace_local.apparent_price.setConstant(1.0f);
			workspace_local.weightings.setZero();


			workspace_local.apparent_price = distance_vector * distance_factor +
				(state_prices_copy.vector.array() * (tarrif_mask.array() * (float(si.owner->tarrifs) / 100.0f) + 1.0f)).matrix();
			workspace_local.weightings = (state_production_copy.vector.array() / workspace_local.apparent_price.array().square()).matrix();
			

			auto sum_weightings = workspace_local.weightings.sum();
			if(sum_weightings > 0) {
#ifndef FULL_PURCHASING_CHANGE
				workspace_local.weightings /= sum_weightings;
				values = values * (1.0f - purchasing_change_rate) + workspace_local.weightings * purchasing_change_rate;
#else
				values = workspace_local.weightings * 1.0f / sum_weightings;
#endif
			
				

				// pay tarrifs & increase global demand
				if(si.owner != ws.w.local_player_nation) {
					// workspace_local.weightings REUSED as money used for purchasing in destination (dest price * qnty = (total money * fraction to target / apparant_price))

					workspace_local.weightings = ((demand_in_state * state_prices_copy.vector.array() * values.array()) / workspace_local.apparent_price.array()).matrix();
					workspace_local.global_demand_by_state += 0.85f * workspace_local.weightings;
					workspace_local.nation_tarrif_income[to_index(si.owner->id)] += workspace_local.weightings.dot(tarrif_mask * (float(si.owner->tarrifs) / 100.0f));
				} else {
					workspace_local.weightings = ((demand_in_state * state_prices_copy.vector.array() * values.array()) / workspace_local.apparent_price.array()).matrix();
					workspace_local.global_demand_by_state += 0.85f * workspace_local.weightings;
					workspace_local.nation_tarrif_income[to_index(si.owner->id)] += workspace_local.weightings.dot(tarrif_mask * (float(si.owner->tarrifs) / 100.0f));

					for(int32_t i = 0; i < state_max; ++i) {
						auto& other_state = ws.w.nation_s.states[nations::state_tag(nations::state_tag::value_base_t(i))];

						if(other_state.owner && other_state.owner != si.owner) {
							workspace_local.player_imports[to_index(other_state.owner->id)] += workspace_local.weightings[i];
						}
					}
				}
			}
		}, concurrency::static_partitioner());

		workspace.combine_each([&global_demand_by_state, &nation_tarrif_income, &player_imports](single_good_update_work_data const & o) {
			global_demand_by_state.vector += o.global_demand_by_state;
			nation_tarrif_income.vector += o.nation_tarrif_income;
			player_imports.vector += o.player_imports;
		});


		concurrency::parallel_for(0, int32_t(state_max),[&ws, &global_demand_by_state, tag](int32_t i){
			state_old_global_demand(ws, nations::state_tag(nations::state_tag::value_base_t(i)))[to_index(tag)] = global_demand_by_state.vector[i];
		}, concurrency::static_partitioner());
		concurrency::parallel_for(0, int32_t(nations_max), [&ws, &nation_tarrif_income, tag](int32_t i) {
			ws.w.nation_s.collected_tariffs.get(nations::country_tag(nations::country_tag::value_base_t(i)), tag) = nation_tarrif_income.vector[i];
		}, concurrency::static_partitioner());

		auto nations_aligned_sz = int32_t(((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(nations_max) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type)));
		resize(ws.w.economy_s.purchasing_arrays, ws.w.local_player_data.imports_by_country[tag], nations_max);
		Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32>(
			get_range(ws.w.economy_s.purchasing_arrays, ws.w.local_player_data.imports_by_country[tag]).first,
			nations_aligned_sz) = player_imports.vector;

		// determine new prices
		concurrency::parallel_for(0, state_max, [&ws, &workspace, &global_demand_by_state, &state_production_copy, state_max, aligned_state_max, tag, base_price](int32_t state_index) {
			nations::state_instance const& si = ws.w.nation_s.states[nations::state_tag(nations::state_tag::value_base_t(state_index))];
			auto& workspace_local = workspace.local();

			if(si.owner == nullptr) // skip remainder for this state
				return;

			auto purchases_for_state = ws.w.nation_s.state_purchases.get(si.id, tag);

			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> values(
				get_range(ws.w.economy_s.purchasing_arrays, purchases_for_state).first, aligned_state_max);

			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> distance_vector(
				ws.w.province_s.state_distances.get_row(si.id),
				aligned_state_max);

			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> tarrif_mask(
				get_range(ws.w.economy_s.purchasing_arrays, si.owner->statewise_tarrif_mask).first,
				aligned_state_max);

			//workspace_local.apparent_price.setConstant(999.9f);

			//workspace_local.apparent_price = (distance_vector.array() * distance_factor +
			//	(tarrif_mask.array() * (float(si.owner->tarrifs) / 100.0f) + 1.0f)
			//	* (state_production_copy.vector.array() > 0.0f).select(global_demand_by_state.vector.array() / state_production_copy.vector.array(), 999.9f)).matrix();

			workspace_local.apparent_price = (distance_vector.array() * distance_factor +
				(tarrif_mask.array() * (float(si.owner->tarrifs) / 100.0f) + 1.0f)
				* global_demand_by_state.vector.array() / (state_production_copy.vector.array() + 0.0001f)).matrix();


			auto current_price = state_current_prices(ws, si.id)[to_index(tag)];

			state_price_delta(ws, si.id)[to_index(tag)] = (
				(current_price * (1.0f - price_change_rate) +
					std::clamp(values.dot(workspace_local.apparent_price), 0.01f, base_price * 10.0f) * price_change_rate)
				- current_price
				) / float(price_update_delay);
		}, concurrency::static_partitioner());

		
	}

	constexpr goods_qnty_type global_rgo_production_multiplier = goods_qnty_type(7.0);
	constexpr goods_qnty_type global_throughput_multiplier = goods_qnty_type(0.5);
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
		//money_qnty_type min_wage = artisan_life_needs * atype.workforce / pop_needs_divisor;


		return output_amount * state_prices[to_index(atype.output_good)] - inputs_cost;
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

		auto profit = output_amount * state_prices[to_index(p.artisan_production)] - inputs_cost;

		if(profit > 0) {
			pay_by_type[to_index(ws.s.population_m.artisan)] += profit;

			state_production[to_index(p.artisan_production)] += output_amount;
			current_state_demand += inputs * inputs_cost / inputs.sum();
		}

		//p.artisan_production_scale = std::clamp(
		//	p.artisan_production_scale * scale_speed(output_amount * state_prices[to_index(p.artisan_production)] / (min_wage + inputs_cost)),
		//	0.05f, 1.0f);

		
		if(profit < min_wage) {
			std::uniform_real_distribution<money_qnty_type> dist(money_qnty_type(0), 4.0f);
			if(dist(get_local_generator()) > sqrt(std::clamp(profit, 0.0f, min_wage) / min_wage)) {
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

	
	template<bool adjust_money>
	void update_demand_and_production(world_state& ws, nations::state_instance& si) {
		if(!(si.owner))
			return;

		const auto state_capital = nations::get_state_capital(ws, si);
		if(!state_capital)
			return;

		
		Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> current_state_demand(
			state_old_demand(ws, si.id),
			ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> current_state_production(
			state_old_production(ws, si.id),
			ws.s.economy_m.aligned_32_goods_count);
		Eigen::Map<Eigen::Matrix<economy::goods_qnty_type, -1, 1>, Eigen::AlignmentType::Aligned32> state_prices(
			state_current_prices(ws, si.id), 
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

		nations::for_each_province(ws, si, [&ws, &current_state_demand, &current_state_production, &masked_prices, &si, &state_prices,
			life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, mobilization_effect](provinces::province_state& ps) {

			apply_pop_consumption<adjust_money>(ws, ps, current_state_demand, masked_prices, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type);

			economy::money_qnty_type* province_pay_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
			std::fill_n(province_pay_by_type, ws.s.population_m.count_poptypes, economy::money_qnty_type(0));
			auto province_population_by_type = ws.w.province_s.province_demographics.get_row(ps.id) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));

			auto& rgo_type = ((ws.s.economy_m.goods[ps.rgo_production].flags & good_definition::mined) != 0) ? ws.s.economy_m.rgo_mine : ws.s.economy_m.rgo_farm;

			update_rgo_production(ws,
				si,
				province_pay_by_type,
				current_state_production,
				state_prices[to_index(ps.rgo_production)],
				life_needs_cost_by_type,
				ps, rgo_type, mobilization_effect);
			update_artisan_production(ws,
				si,
				province_pay_by_type,
				current_state_production,
				current_state_demand,
				state_prices,
				life_needs_cost_by_type,
				ps, mobilization_effect);

			update_rgo_employment(ws, ps);

			if constexpr(adjust_money) {
				provinces::for_each_pop(ws, ps, [province_population_by_type, province_pay_by_type, &ws](population::pop& po) {
					po.money += province_pay_by_type[to_index(po.type)] * float(ws.w.population_s.pop_demographics.get(po.id, population::total_population_tag)) / float(province_population_by_type[to_index(po.type)]);
				});
			}
		});

		
		economy::money_qnty_type* state_pay_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		std::fill_n(state_pay_by_type, ws.s.population_m.count_poptypes, economy::money_qnty_type(0));
		[[maybe_unused]] auto state_population_by_type = ws.w.nation_s.state_demographics.get_row(si.id) + to_index(population::to_demo_tag(ws, population::pop_type_tag(0)));

		
		for(auto& f : si.factories) {
			if(factory_is_open(f)) {
				update_factory_production(ws,
					si,
					state_pay_by_type,
					current_state_production,
					current_state_demand,
					state_prices,
					life_needs_cost_by_type,
					f, *state_capital, mobilization_effect);
			}
		}

		update_factories_employment(ws, si);
		

		if constexpr(adjust_money) {
			nations::for_each_pop(ws, si, [state_population_by_type, state_pay_by_type, &ws](population::pop& po) {
				po.money += state_pay_by_type[to_index(po.type)] * float(ws.w.population_s.pop_demographics.get(po.id, population::total_population_tag)) / float(state_population_by_type[to_index(po.type)]);
			});
		}
		

		current_state_demand = current_state_demand.array().max(minimum_demand).matrix();
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

		auto prices = state_current_prices(ws, in_state->id);
		auto artisan_life_needs = get_life_needs_cost(ws, *in_state, ws.s.population_m.artisan);
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(owner_id);

		money_qnty_type profit_sum = money_qnty_type(0);
		for(auto& atype : ws.s.economy_m.artisan_types) {
			if(bit_vector_test(enabled_goods, to_index(atype.output_good))) {
				auto profit = artisan_profit_unit(ws, ps, *owner, atype, prices, artisan_life_needs);
				if(profit > money_qnty_type(0)) {
					profit_sum += sqrt(profit);
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


	void economy_demand_adjustment_tick(world_state& ws) {
		//ws.w.nation_s.collected_tariffs.clear_all();
		ws.w.nation_s.states.parallel_for_each([&ws](nations::state_instance& si) {
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> prices(state_current_prices(ws, si.id), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> delta(state_price_delta(ws, si.id), ws.s.economy_m.aligned_32_goods_count);
			prices += delta;
			update_demand_and_production<false>(ws, si);
		});
		concurrency::parallel_for(1ui32, ws.s.economy_m.goods_count, [&ws, state_count = ws.w.nation_s.states.minimum_continuous_size(), nations_count = ws.w.nation_s.nations.minimum_continuous_size()](uint32_t i) {
			economy_single_good_tick(ws, goods_tag(goods_tag::value_base_t(i)), state_count, nations_count);
		});
	}

	void economy_update_tick(world_state& ws) {
#ifdef DEBUG_ECONOMY
		std::fill(ws.w.economy_s.world_other_consumption.begin(), ws.w.economy_s.world_other_consumption.end(), goods_qnty_type(0));
		std::fill(ws.w.economy_s.world_other_production.begin(), ws.w.economy_s.world_other_production.end(), goods_qnty_type(0));
		std::fill(ws.w.economy_s.world_pop_consumption.begin(), ws.w.economy_s.world_pop_consumption.end(), goods_qnty_type(0));
		std::fill(ws.w.economy_s.world_rgo_production.begin(), ws.w.economy_s.world_rgo_production.end(), goods_qnty_type(0));
#endif

		ws.w.nation_s.states.parallel_for_each([&ws](nations::state_instance& si) {
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> prices(state_current_prices(ws, si.id), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> delta(state_price_delta(ws, si.id), ws.s.economy_m.aligned_32_goods_count);
			prices += delta;
			update_demand_and_production<true>(ws, si);
		});
		update_construction_and_projects(ws); // after demand update to added demand isnt clobbered

		auto offset = (to_index(ws.w.current_date)) & (price_update_delay - 1);
		concurrency::parallel_for(1 + offset, int32_t(ws.s.economy_m.goods_count), price_update_delay, [&ws, state_count = ws.w.nation_s.states.minimum_continuous_size(), nations_count = ws.w.nation_s.nations.minimum_continuous_size()](uint32_t i) {
			economy_single_good_tick(ws, goods_tag(goods_tag::value_base_t(i)), state_count, nations_count);
		});
		
		collect_taxes(ws);

		//collect tarrif income, pay pops, manage debt
		ws.w.nation_s.nations.parallel_for_each([&ws](nations::nation& n) {
			auto tincome = ws.w.nation_s.collected_tariffs.get_row(n.id);
			ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) += std::reduce(tincome, tincome + ws.s.economy_m.goods_count, economy::money_qnty_type(0), std::plus<>());
			pay_unemployement_pensions_salaries(ws, n);

			auto amount = calculate_daily_debt_payment(ws, n);
			auto current_money = ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good);
			if(current_money >= amount) {
				n.national_debt -= amount / 2.0f;
				ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) = current_money - amount;
			} else {
				n.national_debt += (amount - current_money) - amount / 2.0f;
				ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) = money_qnty_type(0);
			}
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

	void collect_taxes(world_state& ws) {
		ws.w.local_player_data.collected_poor_tax = 0;
		ws.w.local_player_data.collected_middle_tax = 0;
		ws.w.local_player_data.collected_rich_tax = 0;

		ws.w.nation_s.nations.parallel_for_each([&ws](nations::nation& n) {
			n.tax_base = 0.0f;

			if(&n != ws.w.local_player_nation) {
				money_qnty_type total = 0;
				nations::for_each_pop(ws, n, [&total, &ws, &n,
					taxeff = std::max(ws.s.modifiers_m.global_defines.base_country_tax_efficiency + n.modifier_values[modifiers::national_offsets::tax_efficiency], 0.05f),
					pt = float(n.poor_tax) / 100.0f, mt = float(n.middle_tax) / 100.0f, rt = float(n.rich_tax) / 100.0f
				](population::pop& p) {
					auto strata = ws.s.population_m.pop_types[p.type].flags & population::pop_type::strata_mask;
					if(strata == population::pop_type::strata_poor) {
						auto collected = std::min(p.money * pt * taxeff, p.money);
						total += collected;
						n.tax_base += p.money * taxeff;
						p.money -= collected;
					} else if(strata == population::pop_type::strata_middle) {
						auto collected = std::min(p.money * mt * taxeff, p.money);
						total += collected;
						n.tax_base += p.money * taxeff;
						p.money -= collected;
					} else { // strata rich
						auto collected = std::min(p.money * rt * taxeff, p.money);
						total += collected;
						n.tax_base += p.money * taxeff;
						p.money -= collected;
					}
				});
				ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) += total;
			} else {
				money_qnty_type total = 0;
				nations::for_each_pop(ws, n, [&total, &ws, &n,
					taxeff = std::max(ws.s.modifiers_m.global_defines.base_country_tax_efficiency + n.modifier_values[modifiers::national_offsets::tax_efficiency], 0.05f),
					pt = float(n.poor_tax) / 100.0f, mt = float(n.middle_tax) / 100.0f, rt = float(n.rich_tax) / 100.0f
				](population::pop& p) {
					auto strata = ws.s.population_m.pop_types[p.type].flags & population::pop_type::strata_mask;
					if(strata == population::pop_type::strata_poor) {
						auto collected = std::min(p.money * pt * taxeff, p.money);
						total += collected;
						ws.w.local_player_data.collected_poor_tax += p.money * taxeff;
						n.tax_base += p.money * taxeff;
						p.money -= collected;
					} else if(strata == population::pop_type::strata_middle) {
						auto collected = std::min(p.money * mt * taxeff, p.money);
						total += collected;
						ws.w.local_player_data.collected_middle_tax += p.money * taxeff;
						n.tax_base += p.money * taxeff;
						p.money -= collected;
					} else { // strata rich
						auto collected = std::min(p.money * rt * taxeff, p.money);
						total += collected;
						ws.w.local_player_data.collected_rich_tax += p.money * taxeff;
						n.tax_base += p.money * taxeff;
						p.money -= collected;
					}
				});
				ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) += total;
			}
		});
	}

	economy::money_qnty_type project_player_tarrif_income(world_state const& ws, float tarrif_amount) {
		if(auto player = ws.w.local_player_nation; player) {
			auto overlord = player->overlord;
			auto sphere_leader = player->sphere_leader;
			auto overlord_id = overlord ? overlord->id : nations::country_tag();
			auto sphere_leader_id = sphere_leader ? sphere_leader->id : nations::country_tag();

			return std::transform_reduce(integer_iterator(1), integer_iterator(ws.s.economy_m.goods_count), money_qnty_type(0), std::plus<>(), [&ws, tarrif_amount, overlord_id, sphere_leader_id](int32_t i) {
				auto atag = ws.w.local_player_data.imports_by_country[goods_tag(goods_tag::value_base_t(i))];
				auto sz = get_size(ws.w.economy_s.purchasing_arrays, atag);
				auto ptr = get_range(ws.w.economy_s.purchasing_arrays, atag).first;

				
				return std::transform_reduce(integer_iterator(0), integer_iterator(sz), money_qnty_type(0), std::plus<>(), [ptr, tarrif_amount, overlord_id, sphere_leader_id](int32_t i) {
					if(to_index(overlord_id) != i && to_index(sphere_leader_id) != i)
						return tarrif_amount * ptr[i];
					else
						return money_qnty_type(0);
				});
			});
		} else {
			return economy::money_qnty_type(0);
		}
	}

	constexpr float luxury_pay_fraction = 0.5f;

	money_qnty_type military_spending_amount(world_state const& ws, nations::nation const& n) {
		auto nid = n.id;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);
		
		auto military_spending = float(n.military_spending) / 100.0f;
		money_qnty_type sum = money_qnty_type(0);

		{
			auto type = ws.s.population_m.soldier;
			auto soldiers_needs = needs_for_poptype(ws, type, n);
			auto pop_size = ws.w.nation_s.nation_demographics.get(nid, population::to_demo_tag(ws, type));
			auto colonial_pop_size = ws.w.nation_s.nation_colonial_demographics.get(nid, population::to_demo_tag(ws, type));
			auto pop_size_multiplier = pop_size / pop_needs_divisor;
			auto colonial_pop_size_multiplier = colonial_pop_size / pop_needs_divisor;

			sum +=
				(pop_size_multiplier + 0.5f * colonial_pop_size_multiplier) *
				military_spending *
				(soldiers_needs.life_needs + soldiers_needs.everyday_needs + luxury_pay_fraction * soldiers_needs.luxury_needs);
			
		}
		{
			auto type = ws.s.population_m.officer;
			auto officers_needs = needs_for_poptype(ws, type, n);
			auto pop_size = ws.w.nation_s.nation_demographics.get(nid, population::to_demo_tag(ws, type));
			auto colonial_pop_size = ws.w.nation_s.nation_colonial_demographics.get(nid, population::to_demo_tag(ws, type));
			auto pop_size_multiplier = pop_size / pop_needs_divisor;
			auto colonial_pop_size_multiplier = colonial_pop_size / pop_needs_divisor;

			sum +=
				(pop_size_multiplier + 0.5f * colonial_pop_size_multiplier) *
				military_spending *
				(officers_needs.life_needs + officers_needs.everyday_needs + luxury_pay_fraction * officers_needs.luxury_needs);
			
		}

		return sum * (2.0f - n.national_administrative_efficiency);
	}

	money_qnty_type education_spending_amount(world_state const& ws, nations::nation const& n) {
		auto nid = n.id;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);

		auto education_spending = float(n.education_spending) / 100.0f;
		money_qnty_type sum = money_qnty_type(0);

		{
			auto type = ws.s.population_m.clergy;
			auto clergy_needs = needs_for_poptype(ws, type, n);
			auto pop_size = ws.w.nation_s.nation_demographics.get(nid, population::to_demo_tag(ws, type));
			auto colonial_pop_size = ws.w.nation_s.nation_colonial_demographics.get(nid, population::to_demo_tag(ws, type));
			auto pop_size_multiplier = pop_size / pop_needs_divisor;
			auto colonial_pop_size_multiplier = colonial_pop_size / pop_needs_divisor;

			sum +=
				(pop_size_multiplier + 0.5f * colonial_pop_size_multiplier) *
				education_spending *
				(clergy_needs.life_needs + clergy_needs.everyday_needs + luxury_pay_fraction * clergy_needs.luxury_needs);
			
		}
		
		return sum * (2.0f - n.national_administrative_efficiency);
	}

	money_qnty_type administrative_spending_amount(world_state const& ws, nations::nation const& n) {
		auto nid = n.id;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);

		auto education_spending = float(n.administrative_spending) / 100.0f;
		money_qnty_type sum = money_qnty_type(0);

		{
			auto type = ws.s.population_m.bureaucrat;
			auto b_needs = needs_for_poptype(ws, type, n);
			auto pop_size = ws.w.nation_s.nation_demographics.get(nid, population::to_demo_tag(ws, type));
			if(pop_size != 0) {
				auto colonial_pop_size = ws.w.nation_s.nation_colonial_demographics.get(nid, population::to_demo_tag(ws, type));
				auto pop_size_multiplier = pop_size / pop_needs_divisor;
				auto colonial_pop_size_multiplier = colonial_pop_size / pop_needs_divisor;

				sum +=
					(pop_size_multiplier + 0.5f * colonial_pop_size_multiplier) *
					education_spending *
					(b_needs.life_needs + b_needs.everyday_needs + luxury_pay_fraction * b_needs.luxury_needs);
			}
		}

		return sum * (2.0f - n.national_administrative_efficiency);
	}

	money_qnty_type social_spending_amount(world_state const& ws, nations::nation const& n) {

		auto nid = n.id;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);

		auto social_spending = float(n.social_spending) / 100.0f;
		auto pension_fraction = n.modifier_values[modifiers::national_offsets::pension_level] * social_spending;
		auto unemployment_fraction = n.modifier_values[modifiers::national_offsets::unemployment_benefit] * social_spending;

		auto est_costs = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.population_m.count_poptypes), money_qnty_type(0), std::plus<>(),
			[&ws, nid, &n, pension_fraction, unemployment_fraction](int32_t i) {

			population::pop_type_tag this_type = population::pop_type_tag(population::pop_type_tag::value_base_t(i));

			auto pop_size = ws.w.nation_s.nation_demographics.get(nid, population::to_demo_tag(ws, this_type));
			if(pop_size == 0)
				return money_qnty_type(0);

			auto pop_size_multiplier = pop_size / pop_needs_divisor;
			auto pop_employed = ws.w.nation_s.nation_demographics.get(nid, population::to_employment_demo_tag(ws, this_type));

			return get_life_needs_cost(ws, this_type, n) * (pension_fraction + unemployment_fraction * (1.0f - pop_employed / pop_size)) * pop_size_multiplier;
		});

		auto cost_with_waste = est_costs * (2.0f - n.national_administrative_efficiency);
		return cost_with_waste;
	}

	void pay_unemployement_pensions_salaries(world_state& ws, nations::nation& n) {
		if(!is_valid_index(n.current_capital))
			return;

		auto capital_state = ws.w.province_s.province_state_container[n.current_capital].state_instance;
		auto capital_of_capital = nations::get_state_capital(ws, *capital_state);

		Eigen::Map<Eigen::Matrix<economy::money_qnty_type, -1, 1>> masked_prices(
			(economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count),
			ws.s.economy_m.aligned_32_goods_count);
		create_masked_prices(masked_prices, ws, *capital_state);

		economy::money_qnty_type* life_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		economy::money_qnty_type* everyday_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		economy::money_qnty_type* luxury_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);

		fill_needs_costs_arrays(ws, *capital_state, *capital_of_capital, masked_prices, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type);

		auto admin_spending = float(n.administrative_spending) / 100.0f;
		auto education_spending = float(n.education_spending) / 100.0f;
		auto military_spending = float(n.military_spending) / 100.0f;
		auto social_spending = float(n.social_spending) / 100.0f;
		auto pension_fraction = n.modifier_values[modifiers::national_offsets::pension_level] * social_spending;
		auto unemployment_fraction = n.modifier_values[modifiers::national_offsets::unemployment_benefit] * social_spending;

		economy::money_qnty_type est_costs = 0;

		nations::for_each_state(ws, n, [&ws, &est_costs, pension_fraction, unemployment_fraction, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, admin_spending, education_spending, military_spending](nations::state_instance& si) {
			if(!nations::is_colonial_or_protectorate(si)) {
				nations::for_each_pop(ws, si, [&ws, &est_costs, pension_fraction, unemployment_fraction, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, admin_spending, education_spending, military_spending](population::pop& p) {
					auto pop_size = float(ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag));
					if(pop_size == 0)
						return;

					auto pop_size_multiplier = pop_size / pop_needs_divisor;
					auto unemployment = 1.0f - float(ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag)) / pop_size;
					
					auto pamount = life_needs_cost_by_type[to_index(p.type)] * (pension_fraction + unemployment_fraction * unemployment) * pop_size_multiplier;
					p.money += pamount;
					est_costs += pamount;

					if(p.type == ws.s.population_m.bureaucrat) {
						auto amount = pop_size_multiplier *
							admin_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					} else if(p.type == ws.s.population_m.clergy) {
						auto amount =
							pop_size_multiplier *
							education_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					} else if(p.type == ws.s.population_m.soldier) {
						auto amount =
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					} else if(p.type == ws.s.population_m.officer) {
						auto amount =
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					}
				});
			} else {
				nations::for_each_pop(ws, si, [&ws, &est_costs, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, admin_spending, education_spending, military_spending](population::pop& p) {
					auto pop_size = float(ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag));
					auto pop_size_multiplier = pop_size / pop_needs_divisor;

					if(p.type == ws.s.population_m.bureaucrat) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							admin_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					} else if(p.type == ws.s.population_m.clergy) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							education_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					} else if(p.type == ws.s.population_m.soldier) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					} else if(p.type == ws.s.population_m.officer) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(p.type)] + everyday_needs_cost_by_type[to_index(p.type)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(p.type)]);
						p.money += amount;
						est_costs += amount;
					}
				});
			}
		});

		auto cost_with_waste = est_costs * (2.0f - n.national_administrative_efficiency);
		auto diff = ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) - cost_with_waste;
		if(diff < money_qnty_type(0)) {
			ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) = 0;
			n.national_debt += -diff;
		} else {
			ws.w.nation_s.national_stockpiles.get(n.id, economy::money_good) = diff;
		}
	}

}
