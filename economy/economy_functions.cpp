#include "common\\common.h"
#include "economy_functions.hpp"
#include "world_state\\world_state.h"
#include "provinces\\province_functions.hpp"
#include "nations\\nations_functions.hpp"
#include "population\\population_function.h"
#include "nations\\nations_functions.hpp"
#include "provinces\\province_functions.hpp"
#include "modifiers\\modifier_functions.h"
#include <random>
#include "concurrency_tools\\ve.h"

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
		nations::country_tag in_nation,
		provinces::province_tag in_province,
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
				ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::mine_rgo_size>(in_province) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mine_rgo_size>(in_nation) :
				ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::farm_rgo_size>(in_province) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::farm_rgo_size>(in_nation)) +
				ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_size>(production)) +
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

		auto state_id = ws.w.province_s.province_state_container.get<province_state::state_instance>(in_province);

		float total_pop = float(ws.w.nation_s.state_demographics.get(state_id, population::total_population_tag));
		float owner_effect = total_pop != 0.0f ? workers_info.owner.effect_multiplier * (ws.w.nation_s.state_demographics.get(state_id, population::to_demo_tag(ws, workers_info.owner.type))) / total_pop : 0.0f;

		result.output_modifier *= std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_rgo_output>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rgo_output>(in_nation) +
			(is_mined ? ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::farm_rgo_eff>(in_province) : ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::mine_rgo_eff>(in_province)) +
			(workers_info.owner.contribution == contribution_type::output ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_goods_output>(production)) +
			1.0f));
		result.throughput_modifier *= mobilization_effect * std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_rgo_throughput>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rgo_throughput>(in_nation) +
			(workers_info.owner.contribution == contribution_type::throughput ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_goods_throughput>(production)) +
			1.0f));

		return result;
	}

	production_modifiers factory_production_modifiers(
		world_state const& ws,
		worked_instance const& instance,
		bonus* bonuses,
		workers_information const& workers_info,
		nations::country_tag in_nation,
		provinces::province_tag in_province,
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
		auto state_id = ws.w.province_s.province_state_container.get<province_state::state_instance>(in_province);

		float total_pop = float(ws.w.nation_s.state_demographics.get(state_id, population::total_population_tag));
		float owner_effect = total_pop != 0.0f ? workers_info.owner.effect_multiplier * (ws.w.nation_s.state_demographics.get(state_id, population::to_demo_tag(ws, workers_info.owner.type))) / total_pop : 0.0f;

		result.input_modifier *= std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_factory_input>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::factory_input>(in_nation) +
			(workers_info.owner.contribution == contribution_type::input ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::factory_goods_input>(production)) +
			1.0f));
		result.output_modifier *= std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_factory_output>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::factory_output>(in_nation) +
			(workers_info.owner.contribution == contribution_type::output ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::factory_goods_output>(production)) +
			1.0f));
		result.throughput_modifier *= mobilization_effect * std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_factory_throughput>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::factory_throughput>(in_nation) +
			(workers_info.owner.contribution == contribution_type::throughput ? owner_effect : 0.0f) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::factory_goods_throughput>(production)) +
			1.0f));

		float bonus_sum = 1.0f;
		for(uint32_t i = 0; i < std::extent_v<decltype(std::declval<factory_type>().bonuses)>; ++i) {
			if(is_valid_index(bonuses[i].condition) && triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(bonuses[i].condition), ws, state_id, state_id, nullptr, nullptr)) {
				bonus_sum += bonuses[i].value;
			}
		}

		result.throughput_modifier *= bonus_sum;

		return result;
	}


	production_modifiers artisan_production_modifiers(
		world_state const& ws,
		nations::country_tag in_nation,
		provinces::province_tag in_province,
		economy::goods_tag production,
		float mobilization_effect) {

		production_modifiers result;

		result.input_modifier = 1.0f;
		result.output_modifier = 1.0f;
		result.throughput_modifier = 1.0f;


		result.input_modifier *= std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_artisan_input>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::artisan_input>(in_nation) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::artisan_goods_input>(production)) +
			1.0f));
		result.output_modifier *= std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_artisan_output>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::artisan_output>(in_nation) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::artisan_goods_output>(production)) +
			1.0f));
		result.throughput_modifier *= mobilization_effect * std::max(0.0f, (
			ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::local_artisan_throughput>(in_province) +
			ws.w.nation_s.modifier_values.get<modifiers::national_offsets::artisan_throughput>(in_nation) +
			ws.w.nation_s.production_adjustments.get(in_nation, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::artisan_goods_throughput>(production)) +
			1.0f));

		return result;
	}

	int32_t rgo_max_employment(world_state const& ws, provinces::province_tag ps) {
		auto production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(ps);
		if(!is_valid_index(production)) return 1;

		auto owner_id = ws.w.province_s.province_state_container.get<province_state::owner>(ps);
		if(!is_valid_index(owner_id)) return 0;

		int32_t rgo_size = int32_t(ws.w.province_s.province_state_container.get<province_state::rgo_size>(ps));
		auto production_scale = ws.w.province_s.province_state_container.get<province_state::rgo_worker_data>(ps).production_scale;

		bool is_mined = (ws.s.economy_m.goods[production].flags & economy::good_definition::mined) != 0;
		float total_workforce = float((is_mined ? ws.s.economy_m.rgo_mine.workforce : ws.s.economy_m.rgo_farm.workforce) * int32_t(rgo_size)) * production_scale *
			((is_mined ?
				ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::mine_rgo_size>(ps) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mine_rgo_size>(owner_id) :
				ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::farm_rgo_size>(ps) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::farm_rgo_size>(owner_id)) +
				ws.w.nation_s.production_adjustments.get(owner_id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_size>(production)) +
				1.0f);
		return std::max(0, int32_t(total_workforce));
	}

	void match_rgo_worker_type(world_state& ws, provinces::province_tag ps) {
		auto production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(ps);
		bool is_mined = (ws.s.economy_m.goods[production].flags & economy::good_definition::mined) != 0;

		if(is_mined) {
			provinces::for_each_pop(ws, ps, [&ws](population::pop_tag p) {
				auto ptype = ws.w.population_s.pops.get<pop::type>(p);
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
					if(ptype == ws.s.economy_m.rgo_farm.workers[i].type && is_valid_index(ws.s.economy_m.rgo_mine.workers[i].type) && ptype != ws.s.economy_m.rgo_mine.workers[i].type)
						population::change_pop_type(ws, p, ws.s.economy_m.rgo_mine.workers[i].type);
				}
			});
		} else {
			provinces::for_each_pop(ws, ps, [&ws](population::pop_tag p) {
				auto ptype = ws.w.population_s.pops.get<pop::type>(p);
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
					if(ptype == ws.s.economy_m.rgo_mine.workers[i].type && is_valid_index(ws.s.economy_m.rgo_farm.workers[i].type) && ptype != ws.s.economy_m.rgo_farm.workers[i].type)
						population::change_pop_type(ws, p, ws.s.economy_m.rgo_farm.workers[i].type);
				}
			});
		}
	}

	void update_rgo_employment(world_state& ws, provinces::province_tag ps) {
		auto production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(ps);
		auto owner_id = ws.w.province_s.province_state_container.get<province_state::owner>(ps);
		auto& worker_data = ws.w.province_s.province_state_container.get<province_state::rgo_worker_data>(ps);

		bool is_mined = (ws.s.economy_m.goods[production].flags & economy::good_definition::mined) != 0;

		if(!is_valid_index(owner_id)) {
			
			for(uint32_t i = 0; i < std::extent_v<decltype(worker_data.worker_populations)>; ++i)
				worker_data.worker_populations[i] = 0;

			if(is_mined) {
				provinces::for_each_pop(ws, ps, [&ws](population::pop_tag p) {
					auto ptype = ws.w.population_s.pops.get<pop::type>(p);
					for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_mine.workers)>; ++i) {
						if(ptype == ws.s.economy_m.rgo_mine.workers[i].type)
							ws.w.population_s.pop_demographics.get(p, population::total_employment_tag) = 0;
					}
				});
			} else {
				provinces::for_each_pop(ws, ps, [&ws](population::pop_tag p) {
					auto ptype = ws.w.population_s.pops.get<pop::type>(p);
					for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
						if(ptype == ws.s.economy_m.rgo_farm.workers[i].type)
							ws.w.population_s.pop_demographics.get(p, population::total_employment_tag) = 0;
					}
				});
			}
			return;
		}

		int32_t rgo_size = int32_t(ws.w.province_s.province_state_container.get<province_state::rgo_size>(ps));

		float total_workforce = float((is_mined ? ws.s.economy_m.rgo_mine.workforce : ws.s.economy_m.rgo_farm.workforce) * rgo_size) * worker_data.production_scale *
			((is_mined ?
				ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::mine_rgo_size>(ps) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mine_rgo_size>(owner_id) :
				ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::farm_rgo_size>(ps) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::farm_rgo_size>(owner_id)) +
				ws.w.nation_s.production_adjustments.get(owner_id, technologies::economy_tag_to_production_adjustment<technologies::production_adjustment::rgo_size>(production)) +
				1.0f);

		if(is_mined) {
			boost::container::small_vector<float, 8> percentage_by_type;
			for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_mine.workers)>; ++i) {
				auto pop_of_type = ws.w.province_s.province_demographics.get(ps, population::to_demo_tag(ws, ws.s.economy_m.rgo_mine.workers[i].type));
				if(pop_of_type != 0)
					percentage_by_type.push_back(std::min(1.0f, (total_workforce * ws.s.economy_m.rgo_mine.workers[i].amount) / float(pop_of_type)));
				else
					percentage_by_type.push_back(0.0f);
				worker_data.worker_populations[i] = 
					std::min(pop_of_type, total_workforce * ws.s.economy_m.rgo_mine.workers[i].amount);
			}

			provinces::for_each_pop(ws, ps, [&ws, &percentage_by_type](population::pop_tag p) {
				auto ptype = ws.w.population_s.pops.get<pop::type>(p);
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_mine.workers)>; ++i) {
					if(ptype == ws.s.economy_m.rgo_mine.workers[i].type)
						ws.w.population_s.pop_demographics.get(p, population::total_employment_tag) = percentage_by_type[i] * ws.w.population_s.pop_demographics.get(p, population::total_population_tag);
				}
			});
		} else {
			boost::container::small_vector<float, 8> percentage_by_type;
			for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
				auto pop_of_type = ws.w.province_s.province_demographics.get(ps, population::to_demo_tag(ws, ws.s.economy_m.rgo_farm.workers[i].type));
				if(pop_of_type != 0)
					percentage_by_type.push_back(std::min(1.0f, (total_workforce * ws.s.economy_m.rgo_farm.workers[i].amount) / float(pop_of_type)));
				else
					percentage_by_type.push_back(0.0f);
				worker_data.worker_populations[i] =
					std::min(pop_of_type, total_workforce * ws.s.economy_m.rgo_farm.workers[i].amount);
			}

			provinces::for_each_pop(ws, ps, [&ws, &percentage_by_type](population::pop_tag p) {
				auto ptype = ws.w.population_s.pops.get<pop::type>(p);
				for(uint32_t i = 0; i < std::extent_v<decltype(ws.s.economy_m.rgo_farm.workers)>; ++i) {
					if(ptype == ws.s.economy_m.rgo_farm.workers[i].type)
						ws.w.population_s.pop_demographics.get(p, population::total_employment_tag) = percentage_by_type[i] * ws.w.population_s.pop_demographics.get(p, population::total_population_tag);
				}
			});
		}
	}

	void update_factories_employment(world_state& ws, nations::state_tag si) {
		float* allocation_by_type = (float*)_alloca(sizeof(float) * state::factories_count * ws.s.population_m.count_poptypes);
		std::fill_n(allocation_by_type, state::factories_count * ws.s.population_m.count_poptypes, 0.0f);

		float* state_population_by_type = &(ws.w.nation_s.state_demographics.get_row(si)[population::to_demo_tag(ws, population::pop_type_tag(0))]);
		float* unallocated_workers = (float*)_alloca(sizeof(float) * ws.s.population_m.count_poptypes);

		std::copy(state_population_by_type, state_population_by_type + ws.s.population_m.count_poptypes, unallocated_workers);
		std::pair<uint32_t, float> factories_by_profitability[state::factories_count];

		auto& factories = ws.w.nation_s.states.get<state::factories>(si);

		for(uint32_t i = 0; i < state::factories_count; ++i)
			factories_by_profitability[i] = std::pair<uint32_t, float>(i, factories[i].type ? factories[i].worker_data.production_scale : 0.0f);

		std::sort(std::begin(factories_by_profitability), std::end(factories_by_profitability), [](std::pair<uint32_t, float> const& a, std::pair<uint32_t, float> const& b) { return a.second > b.second; });

		for(uint32_t i = 0; i < state::factories_count; ++i) {
			auto factory_index = factories_by_profitability[i].first;
			auto type = factories[factory_index].type;
			if(type) {
				auto factory_workers = factories[factory_index].level * type->factory_workers.workforce * factories[factory_index].worker_data.production_scale;
				for(uint32_t j = 0; j < std::extent_v<decltype(type->factory_workers.workers)>; ++j) {
					if(is_valid_index(type->factory_workers.workers[j].type)) {
						float amount_from_type = factory_workers * type->factory_workers.workers[j].amount;
						uint32_t allocation_index = to_index(type->factory_workers.workers[j].type) *  state::factories_count + factory_index;
						allocation_by_type[allocation_index] = std::min(amount_from_type, unallocated_workers[to_index(type->factory_workers.workers[j].type)]);
						unallocated_workers[to_index(type->factory_workers.workers[j].type)] -= allocation_by_type[allocation_index];

						//average with current
						allocation_by_type[allocation_index] =
							(allocation_by_type[allocation_index]) / 16.0f +
							(factories[factory_index].worker_data.worker_populations[j] * 15.0f) / 16.0f;
					}
				}
			}
		}

		for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
			Eigen::Map<Eigen::Matrix<float, 1, -1>> ivec(allocation_by_type + i * state::factories_count, state::factories_count);
			auto sum = ivec.sum();
			if(sum > 0)
				ivec *= (state_population_by_type[i] - unallocated_workers[i]) / sum;
			//normalize_integer_vector(allocation_by_type + i * state::factories_count, state::factories_count, state_population_by_type[i] - unallocated_workers[i]);
		}

		for(uint32_t i = 0; i < state::factories_count; ++i) {
			auto factory_index = factories_by_profitability[i].first;
			auto type = factories[factory_index].type;
			if(type) {
				for(uint32_t j = 0; j < std::extent_v<decltype(type->factory_workers.workers)>; ++j) {
					if(is_valid_index(type->factory_workers.workers[j].type)) {
						uint32_t allocation_index = to_index(type->factory_workers.workers[j].type) *  state::factories_count + factory_index;
						factories[factory_index].worker_data.worker_populations[j] = allocation_by_type[allocation_index];
					}
				}
			}
		}

		nations::for_each_pop(ws, si, [&ws, unallocated_workers, state_population_by_type](population::pop_tag p) {
			auto ptype = ws.w.population_s.pops.get<pop::type>(p);
			if((ws.s.population_m.pop_types[ptype].flags & population::pop_type::factory_worker) != 0) {
				ws.w.population_s.pop_demographics.get(p, population::total_employment_tag) =
					ws.w.population_s.pop_demographics.get(p, population::total_population_tag) * (state_population_by_type[to_index(ptype)] - unallocated_workers[to_index(ptype)]) / state_population_by_type[to_index(ptype)];
			}
		});
	}

	money_qnty_type get_factory_project_cost(world_state const& ws, factory_type_tag ftype, factory_project_type ptype, tagged_array_view<const float, goods_tag, false> prices) {
		if(ptype == factory_project_type::open || ptype == factory_project_type::expand) {
			return ve::dot_product<goods_tag, false>(prices, ws.s.economy_m.building_costs.get_row(ftype));
		} else {
			// TODO

			return ve::dot_product<goods_tag, false>(prices, ws.s.economy_m.factory_input_goods.get_row(ftype)) * money_qnty_type(10);
		}
	}
	money_qnty_type get_railroad_cost(world_state const& ws, tagged_array_view<const float, goods_tag, false> prices) {
		return ve::dot_product<goods_tag, false>(prices, ws.s.economy_m.building_costs.get_row(ws.s.economy_m.railroad.cost_tag));
	}


	float project_completion(world_state const& ws, nations::state_tag si, tagged_array_view<const float, goods_tag, false> prices) {
		auto& project = ws.w.nation_s.states.get<state::project>(si);
		if(project.type == pop_project_type::railroad) {
			auto cost = get_railroad_cost(ws, prices);
			return cost != 0 ? float(project.funds / cost) : 0.0f;
		} else if(project.type == pop_project_type::factory) {
			if(auto ftype = project.factory_type; is_valid_index(ftype)) {
				auto cost = get_factory_project_cost(ws, ftype, get_factory_project_type(ws, si, ftype), prices);
				return cost != 0 ? float(project.funds / cost) : 0.0f;
			}
		}
		return 0.0f;
	}

	float get_per_worker_profit(world_state const& ws, nations::state_tag si, factory_instance const& f) {
		if(!f.type)
			return std::numeric_limits<float>::min();

		auto prices = state_current_prices(ws, si);
		auto inputs_cost = ve::dot_product<goods_tag, false>(prices, ws.s.economy_m.factory_input_goods.get_row(f.type->id));

		return float(prices[f.type->output_good] * f.type->output_amount - inputs_cost);
	}

	money_qnty_type get_factory_profit(world_state const& ws, provinces::province_tag in_province, factory_instance const& f, tagged_array_view<const float, goods_tag, false> prices) {
		auto owner_id = ws.w.province_s.province_state_container.get<province_state::owner>(in_province);
		auto f_type = f.type;
		if(is_valid_index(owner_id) && bool(f_type)) {

			auto modifiers = factory_production_modifiers(ws, f.worker_data, f_type->bonuses,
				f_type->factory_workers, owner_id, in_province, f.level,
				f_type->output_good,
				(ws.w.nation_s.nations.get<nation::is_mobilized>(owner_id) == false)
					? 1.0f
					: std::max(0.0f,
						1.0f - ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mobilisation_size>(owner_id) * ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mobilisation_economy_impact>(owner_id)));

			//TODO: use modifiers properly

			
			auto inputs = ws.s.economy_m.factory_input_goods.get_row(f_type->id);
			auto inputs_cost = ve::dot_product(prices, inputs) * modifiers.input_modifier;

			return (prices[f_type->output_good] * f_type->output_amount * modifiers.output_modifier - inputs_cost) * modifiers.throughput_modifier;
		}
		return money_qnty_type(0);
	}

	float factory_employment_fraction(world_state const& ws, factory_instance const& fi) {
		if(auto f_type = fi.type; f_type) {
			float total_workforce = float(f_type->factory_workers.workforce * fi.level);
			float total_employed = std::reduce(std::begin(fi.worker_data.worker_populations), std::end(fi.worker_data.worker_populations), 0.0f, std::plus<>());
			return total_workforce != 0.0f ? total_employed / total_workforce : 0.0f;
		} else {
			return 0.0f;
		}
	}

	void init_factory_employment(world_state& ws) {
		ws.w.nation_s.states.for_each([&ws](nations::state_tag si) {
			if(bool(ws.w.nation_s.states.get<state::owner>(si))) {
				auto& factories = ws.w.nation_s.states.get<state::factories>(si);
				for(uint32_t i = 0; i < state::factories_count; ++i) {
					if(factories[i].type && factories[i].level != 0) {
						for(uint32_t j = 0; j < std::extent_v<decltype(factories[i].type->factory_workers.workers)>; ++j) {
							if(is_valid_index(factories[i].type->factory_workers.workers[j].type))
								factories[i].worker_data.worker_populations[j] = 100;
						}
					}
				}

				update_factories_employment(ws, si);
			}
		});
	}

	bool possible_to_invest_in(world_state const& ws, nations::country_tag investor, nations::country_tag target) {
		auto investor_rules_val = ws.w.nation_s.nations.get<nation::current_rules>(investor).rules_value;
		auto target_rules_val = ws.w.nation_s.nations.get<nation::current_rules>(target).rules_value;
		return !nations::is_great_power(ws, target) && nations::is_great_power(ws, investor) &&
			((investor_rules_val & (issues::rules::open_factory_invest | issues::rules::expand_factory_invest | issues::rules::build_factory_invest)) != 0) &&
			((target_rules_val & issues::rules::allow_foreign_investment) != 0) &&
			(investor != target);
	}

	int32_t count_factories_in_state(world_state const& ws, nations::state_tag si) {
		auto& factories = ws.w.nation_s.states.get<state::factories>(si);
		return
			std::transform_reduce(
				std::begin(factories),
				std::end(factories),
				0i32,
				std::plus<>(),
				[](economy::factory_instance const& f) { return int32_t(f.level != 0); });
	}

	int32_t count_factories_in_nation(world_state const& ws, nations::country_tag n) {
		auto states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations.get<nation::member_states>(n));
		int32_t total = 0;
		for(auto s = states.first; s != states.second; ++s) {
			if(auto si = s->state; bool(si)) {
				total += count_factories_in_state(ws, si);
			}
		}
		return total;
	}

	float average_railroad_level(world_state const& ws, nations::state_tag si) {
		float total_provinces = 0.0f;
		float total_levels = 0.0f;
		nations::for_each_province(ws, si, [&ws, &total_provinces, &total_levels](provinces::province_tag ps) {
			total_levels += float(ws.w.province_s.province_state_container.get<province_state::railroad_level>(ps));
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
	factory_project_type get_factory_project_type(world_state const& ws, nations::state_tag location, factory_type_tag ftype) {
		auto& factories = ws.w.nation_s.states.get<state::factories>(location);
		for(auto& fi : factories) {
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

	class state_pops_summary {
	public:
		std::vector<population::pop_tag, concurrent_aligned_allocator<population::pop_tag>> pop_ids;
		std::vector<expanded_tag<population::pop_type_tag>, concurrent_aligned_allocator<expanded_tag<population::pop_type_tag>>> pop_types;
		std::vector<float, concurrent_aligned_allocator<float>> money;
		std::vector<float, concurrent_aligned_allocator<float>> size;
		std::vector<float, concurrent_aligned_allocator<float>> satisfaction;
		std::vector<provinces::province_tag, concurrent_aligned_allocator<provinces::province_tag>> location;

		std::vector<float, concurrent_aligned_allocator<float>> ln_spending;
		std::vector<float, concurrent_aligned_allocator<float>> en_spending;
		std::vector<float, concurrent_aligned_allocator<float>> lx_spending;

		std::vector<float, concurrent_aligned_allocator<float>> ln_money_div_qnty_by_type;
		std::vector<float, concurrent_aligned_allocator<float>> en_money_div_qnty_by_type;
		std::vector<float, concurrent_aligned_allocator<float>> lx_money_div_qnty_by_type;

		std::vector<float, concurrent_aligned_allocator<float>> ln_costs_by_type;
		std::vector<float, concurrent_aligned_allocator<float>> en_costs_by_type;
		std::vector<float, concurrent_aligned_allocator<float>> lx_costs_by_type;


		state_pops_summary(world_state& ws, nations::state_tag si) {
			pop_ids.reserve(64);
			pop_types.reserve(64);
			money.reserve(64);
			size.reserve(64);
			location.reserve(64);

			nations::for_each_pop(ws, si, [&ws, _this = this](population::pop_tag id) {
				_this->pop_ids.push_back(id);
				_this->pop_types.emplace_back(ws.w.population_s.pops.get<pop::type>(id));
				_this->money.push_back(ws.w.population_s.pops.get<pop::money>(id));
				_this->location.emplace_back(ws.w.population_s.pops.get<pop::location>(id));
				_this->size.push_back(ws.w.population_s.pop_demographics.get(id, population::total_population_tag));
			});

			satisfaction.resize(pop_ids.size());
			ln_spending.resize(pop_ids.size());
			en_spending.resize(pop_ids.size());
			lx_spending.resize(pop_ids.size());

			auto ptype_size = int32_t(ws.s.population_m.pop_types.size()) + 1;

			ln_money_div_qnty_by_type.resize(ptype_size);
			en_money_div_qnty_by_type.resize(ptype_size);
			lx_money_div_qnty_by_type.resize(ptype_size);

			ln_costs_by_type.resize(ptype_size);
			en_costs_by_type.resize(ptype_size);
			lx_costs_by_type.resize(ptype_size);
		}
	};

	void create_masked_prices(tagged_array_view<float, goods_tag, false> masked_prices,
		world_state const& ws, nations::state_tag si) {

		auto state_prices = state_current_prices(ws, si);
		std::fill(masked_prices.data(), std::end(masked_prices), 0.0f);


		auto enabled_goods = ws.w.nation_s.active_goods.get_row(ws.w.nation_s.states.get<state::owner>(si));
		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			auto gt = goods_tag(goods_tag::value_base_t(i));
			masked_prices[gt] = bit_vector_test(enabled_goods, gt) ? state_prices[gt] : economy::money_qnty_type(0);
		}
	}

	

	struct needs_values {
		money_qnty_type life_needs = 0;
		money_qnty_type everyday_needs = 0;
		money_qnty_type luxury_needs = 0;
	};

	needs_values needs_for_poptype(world_state const& ws, population::pop_type_tag type, nations::state_tag si) {
		auto state_cap = nations::get_state_capital(ws, si);
		auto state_owner = ws.w.nation_s.states.get<state::owner>(si);

		if(!is_valid_index(state_cap) || !bool(state_owner))
			return needs_values{ 0,0,0 };

		auto owner_id = state_owner;
		if(!ws.w.nation_s.nations.is_valid_index(owner_id) || !ws.w.nation_s.states.is_valid_index(si))
			return needs_values{ 0,0,0 };

		needs_values result{ 0,0,0 };
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(owner_id);
		auto this_strata = ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask;

		economy::money_qnty_type ln_factor;
		economy::money_qnty_type ev_factor;
		economy::money_qnty_type lx_factor;

		if(this_strata == population::pop_type::strata_poor) {
			ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_life_needs>(state_owner) + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::poor_life_needs>(state_cap);
			ev_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_everyday_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::poor_everyday_needs>(state_cap);
			lx_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_luxury_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::poor_luxury_needs>(state_cap);
		} else if(this_strata == population::pop_type::strata_middle) {
			ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_life_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::middle_life_needs>(state_cap);
			ev_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_everyday_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::middle_everyday_needs>(state_cap);
			lx_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_luxury_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::middle_luxury_needs> (state_cap);
		} else { //if(this_strata == population::pop_type::strata_rich) {
			ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_life_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::rich_life_needs>(state_cap);
			ev_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_everyday_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::rich_everyday_needs>(state_cap);
			lx_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_luxury_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::rich_luxury_needs>(state_cap);
		}

		auto ln_goods = ws.s.population_m.life_needs.get_row(type);
		auto en_goods = ws.s.population_m.everyday_needs.get_row(type);
		auto lx_goods = ws.s.population_m.luxury_needs.get_row(type);

		auto state_prices = state_current_prices(ws, si);

		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			auto gt = goods_tag(goods_tag::value_base_t(i));
			if(bit_vector_test(enabled_goods, gt)) {
				result.life_needs += ln_factor * state_prices[gt] * ln_goods[gt];
				result.everyday_needs += ev_factor * state_prices[gt] * en_goods[gt];
				result.luxury_needs += lx_factor * state_prices[gt] * lx_goods[gt];
			}
		}

		return result;
	}

	needs_values needs_for_poptype(world_state const& ws, population::pop_type_tag type, nations::country_tag n) {
		auto ncap = ws.w.nation_s.nations.get<nation::current_capital>(n);
		if(!is_valid_index(ncap))
			return needs_values{ 0,0,0 };
		auto ncap_state = ws.w.province_s.province_state_container.get<province_state::state_instance>(ncap);
		if(!is_valid_index(ncap_state))
			return needs_values{ 0,0,0 };
		return needs_for_poptype(ws, type, ncap_state);
	}

	money_qnty_type get_life_needs_cost(world_state const& ws, nations::state_tag si, population::pop_type_tag ptype) {
		auto state_capital = nations::get_state_capital(ws, si);
		auto state_owner = ws.w.nation_s.states.get<state::owner>(si);

		if(!state_owner || !is_valid_index(state_capital))
			return money_qnty_type(0);

		if(!ws.w.nation_s.nations.is_valid_index(state_owner) || !ws.w.nation_s.states.is_valid_index(si))
			return money_qnty_type(0);


		auto this_strata = ws.s.population_m.pop_types[ptype].flags & population::pop_type::strata_mask;
		economy::money_qnty_type ln_factor;

		if(this_strata == population::pop_type::strata_poor) {
			ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_life_needs>(state_owner) + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::poor_life_needs>(state_capital);
		} else if(this_strata == population::pop_type::strata_middle) {
			ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_life_needs>(state_owner) + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::middle_life_needs>(state_capital);
		} else { //if(this_strata == population::pop_type::strata_rich) {
			ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_life_needs>(state_owner) + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::rich_life_needs>(state_capital);
		}

		money_qnty_type sum = 0;
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(state_owner);
		auto ln_goods = ws.s.population_m.life_needs.get_row(ptype);
		auto state_prices = state_current_prices(ws, si);

		for(uint32_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			auto gt = goods_tag(goods_tag::value_base_t(i));
			if(bit_vector_test(enabled_goods, gt)) {
				sum += ln_factor * state_prices[gt] * ln_goods[gt];
			}
		}

		return sum;
	}

	money_qnty_type get_life_needs_cost(world_state const& ws, population::pop_type_tag type, nations::country_tag n) {
		auto ncap = ws.w.nation_s.nations.get<nation::current_capital>(n);
		if(!is_valid_index(ncap))
			return money_qnty_type(0);
		auto ncap_state = ws.w.province_s.province_state_container.get<province_state::state_instance>(ncap);
		if(!is_valid_index(ncap_state))
			return money_qnty_type(0);
		return get_life_needs_cost(ws, ncap_state, type);
	}

	void fill_needs_costs_arrays(world_state const& ws, nations::state_tag si, provinces::province_tag state_capital,
		tagged_array_view<const float, goods_tag, false> masked_prices,
		economy::money_qnty_type * __restrict life_needs_cost_by_type,
		economy::money_qnty_type * __restrict everyday_needs_cost_by_type,
		economy::money_qnty_type * __restrict luxury_needs_cost_by_type) {

		auto state_owner = ws.w.nation_s.states.get<state::owner>(si);

		for(population::pop_type_tag::value_base_t i = 0; i < ws.s.population_m.count_poptypes; ++i) {
			population::pop_type_tag this_type(i);
			auto this_strata = ws.s.population_m.pop_types[this_type].flags & population::pop_type::strata_mask;

			economy::money_qnty_type ln_factor;
			economy::money_qnty_type ev_factor;
			economy::money_qnty_type lx_factor;

			if(this_strata == population::pop_type::strata_poor) {
				ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_life_needs>(state_owner) + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::poor_life_needs>(state_capital);
				ev_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_everyday_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::poor_everyday_needs>(state_capital);
				lx_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::poor_luxury_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::poor_luxury_needs>(state_capital);
			} else if(this_strata == population::pop_type::strata_middle) {
				ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_life_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::middle_life_needs>(state_capital);
				ev_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_everyday_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::middle_everyday_needs>(state_capital);
				lx_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::middle_luxury_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::middle_luxury_needs>(state_capital);
			} else { //if(this_strata == population::pop_type::strata_rich) {
				ln_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_life_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::rich_life_needs>(state_capital);
				ev_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_everyday_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::rich_everyday_needs>(state_capital);
				lx_factor = economy::money_qnty_type(1) + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::rich_luxury_needs>(state_owner) + ws.w.province_s.modifier_values.get <modifiers::provincial_offsets::rich_luxury_needs>(state_capital);
			}

			auto ln = ws.s.population_m.life_needs.get_row(this_type);
			life_needs_cost_by_type[i] = ln_factor * ve::dot_product(masked_prices, ln);
			
			auto en = ws.s.population_m.everyday_needs.get_row(this_type);
			everyday_needs_cost_by_type[i] = ev_factor * ve::dot_product(masked_prices, en);

			auto xn = ws.s.population_m.luxury_needs.get_row(this_type);
			luxury_needs_cost_by_type[i] = lx_factor * ve::dot_product(masked_prices, xn);
		}
	}

	tagged_array_view<const float, goods_tag, false> state_current_prices(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_prices.get_row(s);
	}

	tagged_array_view<const float, goods_tag, false> state_price_delta(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_price_delta.get_row(s);
	}

	float state_current_production(world_state const& ws, nations::state_tag s, goods_tag g) {
		return ws.w.nation_s.state_production.get(s, g);
	}

	tagged_array_view<const float, goods_tag, false> state_current_demand(world_state const& ws, nations::state_tag s) {
		return ws.w.nation_s.state_demand.get_row(s);
	}

	tagged_array_view<float, goods_tag, false> state_current_prices(world_state& ws, nations::state_tag s) {
		return ws.w.nation_s.state_prices.get_row(s);
	}
	tagged_array_view<float, goods_tag, false> state_price_delta(world_state& ws, nations::state_tag s) {
		return ws.w.nation_s.state_price_delta.get_row(s);
	}
	tagged_array_view<float, goods_tag, false> state_current_demand(world_state& ws, nations::state_tag s) {
		return ws.w.nation_s.state_demand.get_row(s);
	}

	money_qnty_type calculate_daily_debt_payment(world_state const & ws, nations::country_tag n) {
		return ws.w.nation_s.nations.get<nation::national_debt>(n) * economy::daily_debt_payment;
	}


	void init_artisan_producation(world_state& ws) {
		ws.w.province_s.province_state_container.for_each([&ws](provinces::province_tag p) {
			ws.w.province_s.province_state_container.set<province_state::artisan_production>(p, ws.s.economy_m.artisan_types[get_profitable_artisan(ws, p)].output_good);
			ws.w.province_s.province_state_container.set<province_state::artisan_production_scale>(p, 1.0f);
		});
	}

	void set_initial_money(world_state& ws) {
		ws.w.nation_s.states.for_each([&ws](nations::state_tag si) {
			auto state_owner = ws.w.nation_s.states.get<state::owner>(si);
			if(!bool(state_owner))
				return;

			const auto state_capital = nations::get_state_capital(ws, si);
			if(!is_valid_index(state_capital))
				return;

			economy::money_qnty_type* life_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
			economy::money_qnty_type* everyday_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
			economy::money_qnty_type* luxury_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);

			tagged_array_view<float, goods_tag, false> masked_prices(
				(economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count),
				ws.s.economy_m.aligned_32_goods_count);

			create_masked_prices(masked_prices, ws, si);

			fill_needs_costs_arrays(ws, si, state_capital, masked_prices, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type);

			nations::for_each_pop(ws, si, [&ws, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type](population::pop_tag p) {
				auto ptype = ws.w.population_s.pops.get<pop::type>(p);
				ws.w.population_s.pops.set<pop::money>(p,
					(life_needs_cost_by_type[to_index(ptype)] +
					everyday_needs_cost_by_type[to_index(ptype)] +
					economy::money_qnty_type(0.5) * luxury_needs_cost_by_type[to_index(ptype)])
					* float(ws.w.population_s.pop_demographics.get(p, population::total_population_tag)) / pop_needs_divisor);
			});

		});
	}

	void update_bankrupcy(world_state & ws) {
		ws.w.nation_s.nations.parallel_for_each([&ws](nations::country_tag n) {
			if(calculate_daily_debt_payment(ws, n) / 2.0f > ws.w.nation_s.nations.get<nation::tax_base>(n)) {
				ws.w.nation_s.nations.set<nation::national_debt>(n, 0.0f);
				modifiers::add_unique_timed_modifier_to_nation(ws, n,
					ws.s.modifiers_m.static_modifiers.in_bankrupcy,
					date_tag(to_index(ws.w.current_date) + int32_t(365.0f * ws.s.modifiers_m.global_defines.bankrupcy_duration)));
				ws.w.nation_s.nations.set<nation::base_prestige>(n, 0.0f);
			}
		});
	}

	bool is_bankrupt(world_state const& ws, nations::country_tag n) {
		return find(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(n), nations::timed_national_modifier{ date_tag(), ws.s.modifiers_m.static_modifiers.in_bankrupcy }) != nullptr;
	}

	range_information global_price_range(world_state const& ws, economy::goods_tag t) {
		range_information result{ std::numeric_limits<money_qnty_type>::max(), std::numeric_limits<money_qnty_type>::lowest(), 0.0f };
		int32_t count = 0;
		ws.w.nation_s.states.for_each([&ws, t, &result, &count](nations::state_tag sid) {
			if(is_valid_index(ws.w.nation_s.states.get<state::owner>(sid)) && ws.w.nation_s.states.is_valid_index(sid)) {
				auto prices = state_current_prices(ws, sid);
				auto v = prices[t];
				result.minimum = std::min(result.minimum, v);
				result.maximum = std::max(result.maximum, v);
				result.average += v;
				++count;
			}
		});
		result.average /= float(count);
		return result;
	}



	money_qnty_type daily_state_owner_building_cost(world_state const& ws, nations::state_tag id) {
		if(!ws.w.nation_s.states.is_valid_index(id))
			return money_qnty_type(0);

		auto prices_v = state_current_prices(ws, id);

		auto rr_daily_cost = ve::dot_product<goods_tag, false>(prices_v, ws.s.economy_m.building_costs.get_row(ws.s.economy_m.railroad.cost_tag))
			/ money_qnty_type(ws.s.economy_m.railroad.time);
		auto fort_daily_cost = ve::dot_product<goods_tag, false>(prices_v, ws.s.economy_m.building_costs.get_row(ws.s.economy_m.fort.cost_tag))
			/ money_qnty_type(ws.s.economy_m.fort.time);
		auto nb_daily_cost = 
			(ve::dot_product<goods_tag, false>(prices_v, ws.s.economy_m.building_costs.get_row(ws.s.economy_m.naval_base.cost_tag))
				+ money_qnty_type(ws.s.economy_m.naval_base.extra_cost))
			/ money_qnty_type(ws.s.economy_m.naval_base.time);

		money_qnty_type sum = 0;
		nations::for_each_province(ws, id, [&sum, rr_daily_cost, fort_daily_cost, nb_daily_cost, &ws](provinces::province_tag ps) {
			auto& container = ws.w.province_s.province_state_container;
			sum += (container.get<province_state::fort_upgrade_progress>(ps) > 0.0f ? fort_daily_cost : money_qnty_type(0)) +
				(container.get<province_state::naval_base_upgrade_progress>(ps) > 0.0f ? nb_daily_cost : money_qnty_type(0)) +
				((container.get<province_state::railroad_upgrade_progress>(ps) > 0.0f && container.get<province_state::owner_building_railroad>(ps)) ? rr_daily_cost : money_qnty_type(0));
		});

		auto& factories = ws.w.nation_s.states.get<state::factories>(id);
		return std::transform_reduce(std::begin(factories), std::end(factories), sum, std::plus<>(), [&prices_v, &ws](factory_instance const& f) {
			if(auto type = f.type; type && f.factory_progress > 0.0f && (f.flags & factory_instance::owner_is_upgrading) != 0) {
				return ve::dot_product<goods_tag, false>(prices_v, ws.s.economy_m.building_costs.get_row(type->id))
					/ money_qnty_type(type->building_time);
			} else {
				return money_qnty_type(0);
			}
		});
	}
	money_qnty_type daily_national_building_cost(world_state const& ws, nations::country_tag n) {
		money_qnty_type sum = 0;
		nations::for_each_state(ws, n, [&ws, &sum](nations::state_tag si) {
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

			cbacked_eigen_vector_generator(int32_t s) : size(
				int32_t(((static_cast<uint32_t>(sizeof(T)) * uint32_t(s) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(T)))
			) {}

			auto operator()() const noexcept {
				return cbacked_eigen_vector<T>(size);
			}
		};
	}

	void update_construction_and_projects(world_state& ws) {
		int32_t nations_count = ws.w.nation_s.nations.size();
		concurrency::combinable<cbacked_eigen_vector<money_qnty_type>> costs_by_nation = concurrency::combinable<cbacked_eigen_vector<money_qnty_type>>(cbacked_eigen_vector_generator<money_qnty_type>(nations_count));

		auto rr_time = 1.0f / money_qnty_type(ws.s.economy_m.railroad.time);
		auto fort_time = 1.0f / money_qnty_type(ws.s.economy_m.fort.time);
		auto naval_time = 1.0f / money_qnty_type(ws.s.economy_m.naval_base.time);

		ws.w.nation_s.states.parallel_for_each([&ws, &costs_by_nation, rr_time, fort_time, naval_time](nations::state_tag si) {
			auto owner_id = ws.w.nation_s.states.get<state::owner>(si);

			if(!is_valid_index(owner_id))
				return;

			auto& local_nation_costs = costs_by_nation.local();

			auto prices_v = state_current_prices(ws, si);
			auto current_state_demand = state_current_demand(ws, si);

			auto fort_costs = ws.s.economy_m.building_costs.get_row(ws.s.economy_m.fort.cost_tag);
			auto rr_costs = ws.s.economy_m.building_costs.get_row(ws.s.economy_m.railroad.cost_tag);
			auto nb_costs = ws.s.economy_m.building_costs.get_row(ws.s.economy_m.naval_base.cost_tag);

			auto rr_daily_cost = ve::dot_product<goods_tag, false>(prices_v, rr_costs) / money_qnty_type(ws.s.economy_m.railroad.time);
			auto fort_daily_cost = ve::dot_product<goods_tag, false>(prices_v, fort_costs) / money_qnty_type(ws.s.economy_m.fort.time);
			auto nb_daily_cost =
				(ve::dot_product<goods_tag, false>(prices_v, nb_costs) + money_qnty_type(ws.s.economy_m.naval_base.extra_cost)) / money_qnty_type(ws.s.economy_m.naval_base.time);

			auto owner_sp_setting = float(ws.w.nation_s.nations.get<nation::projects_stockpile_spending>(owner_id)) / 100.0f;
			nations::for_each_province(ws, si, [owner_id, rr_daily_cost, fort_daily_cost, nb_daily_cost, rr_time, fort_time, naval_time,
				current_state_demand, &local_nation_costs, owner_sp_setting, rr_costs, fort_costs, nb_costs, prices_v, &ws](provinces::province_tag ps) {
				
				auto& fort_upgrade_progress = ws.w.province_s.province_state_container.get<province_state::fort_upgrade_progress>(ps);
				if(fort_upgrade_progress > 0.0f) {
					local_nation_costs.vector[to_index(owner_id)] += fort_daily_cost * owner_sp_setting;
					fort_upgrade_progress += fort_time * owner_sp_setting;
					ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, fort_costs, prices_v, fort_time * owner_sp_setting);

					if(fort_upgrade_progress >= 1.0f) {
						fort_upgrade_progress = 0;
						ws.w.province_s.province_state_container.get<province_state::fort_level>(ps)++;
					}
				}

				auto& naval_base_upgrade_progress = ws.w.province_s.province_state_container.get<province_state::naval_base_upgrade_progress>(ps);
				if(naval_base_upgrade_progress > 0.0f) {
					local_nation_costs.vector[to_index(owner_id)] += nb_daily_cost * owner_sp_setting;
					naval_base_upgrade_progress += naval_time * owner_sp_setting;
					ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, nb_costs, prices_v, naval_time * owner_sp_setting);

					if(naval_base_upgrade_progress >= 1.0f) {
						naval_base_upgrade_progress = 0;
						ws.w.province_s.province_state_container.get<province_state::naval_base_level>(ps)++;
					}
				}

				auto& railroad_upgrade_progress = ws.w.province_s.province_state_container.get<province_state::railroad_upgrade_progress>(ps);
				if(railroad_upgrade_progress > 0.0f && ws.w.province_s.province_state_container.get<province_state::owner_building_railroad>(ps)) {
					local_nation_costs.vector[to_index(owner_id)] += rr_daily_cost * owner_sp_setting;
					railroad_upgrade_progress += rr_time * owner_sp_setting;
					ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, rr_costs, prices_v, rr_time * owner_sp_setting);

					if(railroad_upgrade_progress >= 1.0f) {
						railroad_upgrade_progress = 0;
						ws.w.province_s.province_state_container.get<province_state::railroad_level>(ps)++;
					}
				}
			});

			auto& factories = ws.w.nation_s.states.get<state::factories>(si);
			for(auto& f : factories) {
				if(auto type = f.type; type && f.factory_progress > 0.0f && (f.flags & factory_instance::owner_is_upgrading) != 0) {
					auto costs = ws.s.economy_m.building_costs.get_row(type->id);

					auto time = 1.0f / money_qnty_type(type->building_time);
					auto p_d_c = ve::dot_product<goods_tag, false>(prices_v, ws.s.economy_m.building_costs.get_row(type->id));
					local_nation_costs.vector[to_index(owner_id)] += p_d_c * owner_sp_setting * time;
					ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, costs, prices_v, time * owner_sp_setting);

					f.factory_progress += time;
					if(f.factory_progress >= 1.0f) {
						f.factory_progress = 0;
						f.level++;
					}
				}
			}


			// update pop projects
			auto& project = ws.w.nation_s.states.get<state::project>(si);
			switch(project.type) {
				default:
				case pop_project_type::none:
					break;
				case pop_project_type::railroad:
				{
					auto& railroad_upgrade_progress = ws.w.province_s.province_state_container.get<province_state::railroad_upgrade_progress>(project.location);
					if(project.funds > rr_daily_cost) {
						railroad_upgrade_progress += rr_time;
						ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, rr_costs, prices_v, rr_time);
						project.funds -= rr_daily_cost;
					} else {
						railroad_upgrade_progress += rr_time * project.funds / rr_daily_cost;
						ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, rr_costs, prices_v, rr_time * project.funds / rr_daily_cost);
						project.funds = 0;
					}
					if(railroad_upgrade_progress >= 1.0f) {
						project.type = pop_project_type::none;
						railroad_upgrade_progress = 0;
						ws.w.province_s.province_state_container.get<province_state::railroad_level>(project.location)++;
					}
				}
					break;
				case pop_project_type::factory:
				{
					auto f = std::find_if(std::begin(factories), std::end(factories),
						[t = project.factory_type](factory_instance const& fi) { return bool(fi.type) && (fi.type->id == t); });
					if(f == std::end(factories))
						break;

					if(f->factory_progress > 0.0f) {
						auto costs = ws.s.economy_m.building_costs.get_row(project.factory_type);

						auto time = 1.0f / money_qnty_type(f->type->building_time);
						auto daily_cost = ve::dot_product<goods_tag, false>(prices_v, costs) * time;

						if(project.funds > daily_cost) {
							f->factory_progress += time;
							ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, costs, prices_v, time);
							project.funds -= daily_cost;
						} else {
							f->factory_progress += time * project.funds / daily_cost;
							ve::accumulate_scaled_product<goods_tag, false>(current_state_demand, costs, prices_v, time * project.funds / daily_cost);
							project.funds = 0;
						}

						if(f->factory_progress >= 1.0f) {
							project.type = pop_project_type::none;
							f->factory_progress = 0;
							f->level++;
						}
					} else if(f->worker_data.production_scale <= 0) {
						auto costs = ws.s.economy_m.factory_input_goods.get_row(project.factory_type);

						auto reopen_cost = ve::dot_product<goods_tag, false>(prices_v, costs) * money_qnty_type(10);
						if(project.funds >= reopen_cost) {
							f->worker_data.production_scale = 1.0f;
							project.type = pop_project_type::none;
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
						ws.w.nation_s.nations.get<nation::national_debt>(nations::country_tag(nations::country_tag::value_base_t(i))) += val - t;
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
			moveable_concurrent_cache_aligned_buffer<float, nations::state_tag, true> weightings;
			moveable_concurrent_cache_aligned_buffer<float, nations::state_tag, true> apparent_price;
			moveable_concurrent_cache_aligned_buffer<float, nations::state_tag, true> tarrifs;
			moveable_concurrent_cache_aligned_buffer<float, nations::state_tag, true> global_demand_by_state;
			moveable_concurrent_cache_aligned_buffer<float, nations::country_tag, true> player_imports;
			moveable_concurrent_cache_aligned_buffer<float, nations::country_tag, true> nation_tarrif_income;
		protected:
			single_good_update_work_data(int32_t state_count, int32_t nations_count) :
				weightings(state_count),
				apparent_price(state_count),
				tarrifs(state_count),
				global_demand_by_state(state_count),
				player_imports(nations_count),
				nation_tarrif_income(nations_count) {

			}
		public:

			single_good_update_work_data() = delete;
			single_good_update_work_data(const single_good_update_work_data& other) = delete;
			single_good_update_work_data(single_good_update_work_data&& other) = default;

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

	constexpr int32_t prefetch_constant_a = 4;

	struct calculate_apparant_prices {
		float* const apparent_price_v;
		float* const distance_vector_v;
		float const state_owner_tarrifs;
		float* const state_prices_copy_v;
		float* const tarrif_mask_v;

		calculate_apparant_prices(float* a, float* c, float d, float* e, float* g) :
			apparent_price_v(a), distance_vector_v(c), state_owner_tarrifs(d), state_prices_copy_v(e), tarrif_mask_v(g) {}

		template<typename T>
		__forceinline void operator()(T executor) {
			executor.nt_prefetch<prefetch_constant_a>(state_prices_copy_v);
			executor.nt_prefetch<prefetch_constant_a>(tarrif_mask_v);
			executor.nt_prefetch<prefetch_constant_a>(distance_vector_v);
			executor.prefetch<prefetch_constant_a>(apparent_price_v);

			// = state_price * (tarrif_mask * state owner tarrif + 1.0) + distance_vector * distance factor 
			auto new_apparent_prices = ve::multiply_and_add(
				executor.load(state_prices_copy_v),
				ve::multiply_and_add(
					executor.load(tarrif_mask_v),
					executor.constant(state_owner_tarrifs),
					executor.constant(1.0f)),
				executor.load(distance_vector_v) *
				executor.constant(distance_factor)
			);
			executor.store(apparent_price_v, new_apparent_prices);
		}
	};

	struct calculate_weightings {
		float* const apparent_price_v;
		float* const weightings_v;
		float* const state_production_copy_v;

		calculate_weightings(float* a, float* b, float* f) :
			apparent_price_v(a), weightings_v(b), state_production_copy_v(f) {}

		template<typename T>
		__forceinline void operator()(T executor) {
			executor.prefetch<prefetch_constant_a>(apparent_price_v);
			executor.nt_prefetch<prefetch_constant_a>(weightings_v);
			executor.nt_prefetch<prefetch_constant_a>(state_production_copy_v);

			auto new_apparent_prices = executor.load(apparent_price_v);
			executor.store(weightings_v, executor.load(state_production_copy_v) / (new_apparent_prices * new_apparent_prices));
		}
	};

	struct tariff_updator {
		float* const global_demand_by_state_v;
		float* const apparent_price_v;
		float* const state_prices_copy_v;
		float* const values_v;
		float* const tarrif_mask_v;
		float const state_owner_tarrifs;
		ve::fp_vector tariff_income_accumulator[ve::block_repitition] = { ve::fp_vector{}, ve::fp_vector{}, ve::fp_vector{}, ve::fp_vector{} };

		tariff_updator(float* a, float* b, float* c, float* d, float* e, float f) :
			global_demand_by_state_v(a), apparent_price_v(b), state_prices_copy_v(c), values_v(d), tarrif_mask_v(e), state_owner_tarrifs(f) {}

		template<typename T>
		__forceinline void operator()(T executor) {
			executor.prefetch<prefetch_constant_a>(apparent_price_v);
			executor.nt_prefetch<prefetch_constant_a>(state_prices_copy_v);
			executor.nt_prefetch<prefetch_constant_a>(values_v);
			executor.nt_prefetch<prefetch_constant_a>(tarrif_mask_v);
			executor.nt_prefetch<prefetch_constant_a>(global_demand_by_state_v);

			ve::fp_vector money_spent_at_destination;
			if constexpr(executor.full_operation) {
				money_spent_at_destination =
					executor.load(state_prices_copy_v) * executor.load(values_v) / executor.load(apparent_price_v);
			} else {
				money_spent_at_destination = select(executor.partial_mask(),
					executor.load(state_prices_copy_v) * executor.load(values_v) / executor.load(apparent_price_v), executor.zero());
			}
			// global demand by state += 0.85 * money_spent_at_destination
			executor.store(global_demand_by_state_v,
				ve::multiply_and_add(money_spent_at_destination, executor.constant(0.85f), executor.load(global_demand_by_state_v)));
			tariff_income_accumulator[executor.block_index] =
				ve::multiply_and_add(executor.load(tarrif_mask_v), executor.constant(state_owner_tarrifs) * money_spent_at_destination,
					tariff_income_accumulator[executor.block_index]);
		}
	};

	struct player_tariff_updator {
		float* const global_demand_by_state_v;
		float* const apparent_price_v;
		float* const state_prices_copy_v;
		float* const values_v;
		float* const tarrif_mask_v;
		float const state_owner_tarrifs;
		ve::fp_vector tariff_income_accumulator[ve::block_repitition] = { ve::fp_vector{}, ve::fp_vector{}, ve::fp_vector{}, ve::fp_vector{} };
		float* const money_spent_values;

		player_tariff_updator(float* a, float* b, float* c, float* d, float* e, float f, float* h) :
			global_demand_by_state_v(a), apparent_price_v(b), state_prices_copy_v(c), values_v(d), tarrif_mask_v(e), state_owner_tarrifs(f), money_spent_values(h) {}

		template<typename T>
		__forceinline void operator()(T executor) {
			executor.prefetch<prefetch_constant_a>(apparent_price_v);
			executor.nt_prefetch<prefetch_constant_a>(state_prices_copy_v);
			executor.nt_prefetch<prefetch_constant_a>(values_v);
			executor.nt_prefetch<prefetch_constant_a>(tarrif_mask_v);
			executor.nt_prefetch<prefetch_constant_a>(global_demand_by_state_v);

			ve::fp_vector money_spent_at_destination;
			if constexpr(executor.full_operation) {
				money_spent_at_destination =
					executor.load(state_prices_copy_v) * executor.load(values_v) / executor.load(apparent_price_v);
			} else {
				money_spent_at_destination = select( executor.partial_mask(),
					executor.load(state_prices_copy_v) * executor.load(values_v) / executor.load(apparent_price_v), executor.zero());
			}
			// global demand by state += 0.85 * money_spent_at_destination
			executor.store(global_demand_by_state_v,
				ve::multiply_and_add(money_spent_at_destination, executor.constant(0.85f), executor.load(global_demand_by_state_v)));
			tariff_income_accumulator[executor.block_index] =
				ve::multiply_and_add(executor.load(tarrif_mask_v), executor.constant(state_owner_tarrifs) * money_spent_at_destination,
					tariff_income_accumulator[executor.block_index]);

			executor.store(money_spent_values, money_spent_at_destination);
		}
	};

	struct new_price_accumulator {
		ve::fp_vector price_times_purchases_accumulator[ve::block_repitition] = { ve::fp_vector{}, ve::fp_vector{}, ve::fp_vector{}, ve::fp_vector{} };

		float* const distance_vector;
		float* const tariff_mask;
		float* const global_demand_by_state;
		float* const state_production_copy;
		float* const values;

		float const state_owner_tariffs;
		
		new_price_accumulator(float* a, float* b, float* c, float* d, float* e, float f) :
			distance_vector(a), tariff_mask(b), global_demand_by_state(c), state_production_copy(d), values(e), state_owner_tariffs(f) {}

		template<typename T>
		__forceinline void operator()(T executor) {
			executor.prefetch<prefetch_constant_a>(distance_vector);
			executor.prefetch<prefetch_constant_a>(tariff_mask);
			executor.prefetch<prefetch_constant_a>(global_demand_by_state);
			executor.prefetch<prefetch_constant_a>(state_production_copy);
			executor.prefetch<prefetch_constant_a>(values);

			price_times_purchases_accumulator[executor.block_index] = 
				price_times_purchases_accumulator[executor.block_index] + 
				
				(executor.load(values) * ve::multiply_and_add(
					executor.load(global_demand_by_state) / (executor.load(state_production_copy) + executor.constant(0.0001f)),
					ve::multiply_and_add(
						executor.load(tariff_mask),
						executor.constant(state_owner_tariffs),
						executor.constant(1.0f)),
					executor.load(distance_vector) *
					executor.constant(distance_factor))
			);
		}
		/*
		values * (distance_vector * distance_factor +
			global_demand_by_state * (tarrif_mask * state_owner_tarrifs + 1.0f) / (state_production_copy.vector.array() + 0.0001f)).matrix();
		*/
	};

	void economy_single_good_tick(world_state& ws, goods_tag tag, int32_t state_max, int32_t nations_max) {
		auto aligned_state_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(state_max + 1) + 63ui32) & ~63ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));
		//auto aligned_nations_max = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(nations_max + 1) + 63ui32) & ~63ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));

		const auto base_price = ws.s.economy_m.goods[tag].base_price;

		concurrency::combinable<single_good_update_work_data> workspace(single_good_update_work_data_factory(
			state_max, // state aligned size
			nations_max) // nations aligned size
		);

		ws.w.nation_s.state_global_demand.reset_row(tag);

		auto global_demand_by_state = ws.w.nation_s.state_global_demand.get_row(tag, aligned_state_max);
		auto state_production = ws.w.nation_s.state_production.get_row(tag, aligned_state_max);

		concurrent_cache_aligned_buffer<float, nations::country_tag, true> nation_tarrif_income(nations_max);
		concurrent_cache_aligned_buffer<float, nations::country_tag, true> player_imports(nations_max);
		concurrent_cache_aligned_buffer<float, nations::state_tag, true> state_prices_copy(state_max, base_price);

		ws.w.nation_s.states.parallel_for_each([&ws, &state_prices_copy, tag](nations::state_tag st) {
			state_prices_copy[st] = state_current_prices(ws, st)[tag];
		}, concurrency::static_partitioner());

		ws.w.nation_s.states.parallel_for_each([&ws, &workspace, state_production, &state_prices_copy, tag, base_price, aligned_state_max, state_max](nations::state_tag si) {
			auto demand_in_state = std::max(state_current_demand(ws, si)[tag], 0.001f);
			auto& workspace_local = workspace.local();

			auto state_owner = ws.w.nation_s.states.get<state::owner>(si);

			if(!is_valid_index(state_owner) || demand_in_state <= 0) { // skip remainder for this state
				state_current_prices(ws, si)[tag] = base_price;
				resize(ws.w.economy_s.purchasing_arrays, ws.w.nation_s.state_purchases.get(si, tag), 0);
				return;
			}

			auto state_owner_tarrifs = float(ws.w.nation_s.nations.get<nation::tarrifs>(state_owner)) / 100.0f;
			auto state_owner_tarrif_mask = ws.w.nation_s.nations.get<nation::statewise_tarrif_mask>(state_owner);

			auto& purchases_for_state = ws.w.nation_s.state_purchases.get(si, tag);
			auto sz = get_size(ws.w.economy_s.purchasing_arrays, purchases_for_state);

			if(sz < aligned_state_max)
				resize(ws.w.economy_s.purchasing_arrays, purchases_for_state, state_max);
			

			auto values = get_range(ws.w.economy_s.purchasing_arrays, purchases_for_state);
			auto distance_vector = ws.w.province_s.state_distances.get_row(si);
			auto tarrif_mask = get_range(ws.w.economy_s.purchasing_arrays, state_owner_tarrif_mask);

			ve::execute_serial_fast(uint32_t(state_max + 1), calculate_apparant_prices(
				workspace_local.apparent_price.data(),
				distance_vector.data(),
				state_owner_tarrifs,
				state_prices_copy.data(),
				tarrif_mask.data()
			));

			ve::execute_serial_fast(uint32_t(state_max + 1), calculate_weightings(
				workspace_local.apparent_price.data(),
				values.data(),
				state_production.data()
			));
			

			auto sum_weightings = ve::reduce<nations::state_tag, true>(values);

			if(sum_weightings > 0) {
				ve::rescale(values, demand_in_state / sum_weightings);
				

				// pay tarrifs & increase global demand
				if(ws.w.local_player_nation && ws.w.local_player_nation != state_owner) {

					tariff_updator to_obj(workspace_local.global_demand_by_state.data(), workspace_local.apparent_price.data(),
						state_prices_copy.data(), values.data(), tarrif_mask.data(), state_owner_tarrifs);

					ve::execute_serial(uint32_t(state_max + 1), to_obj);
					workspace_local.nation_tarrif_income[state_owner] +=
						((to_obj.tariff_income_accumulator[0] + to_obj.tariff_income_accumulator[1]) + (to_obj.tariff_income_accumulator[2] + to_obj.tariff_income_accumulator[3])).reduce();

					
				} else {
					player_tariff_updator to_obj(workspace_local.global_demand_by_state.data(), workspace_local.apparent_price.data(),
						state_prices_copy.data(), values.data(), tarrif_mask.data(), state_owner_tarrifs, workspace_local.weightings.data());

					ve::execute_serial(uint32_t(state_max + 1), to_obj);
					workspace_local.nation_tarrif_income[state_owner] +=
						((to_obj.tariff_income_accumulator[0] + to_obj.tariff_income_accumulator[1]) + (to_obj.tariff_income_accumulator[2] + to_obj.tariff_income_accumulator[3])).reduce();

					ws.w.nation_s.states.for_each([&workspace_local, &ws, state_owner](nations::state_tag st) {
						auto other_state_owner = ws.w.nation_s.states.get<state::owner>(st);

						if(is_valid_index(other_state_owner) && other_state_owner != state_owner) {
							workspace_local.player_imports[other_state_owner] += workspace_local.weightings[st];
						}
					});
				}
			}
		}, concurrency::static_partitioner());

		workspace.combine_each([global_demand_by_state, &nation_tarrif_income, &player_imports, state_max, nations_max](single_good_update_work_data const & o) {
			ve::accumulate<nations::state_tag, true>(global_demand_by_state, o.global_demand_by_state.view());
			ve::accumulate<nations::country_tag, true>(nation_tarrif_income.view(), o.nation_tarrif_income.view());
			ve::accumulate<nations::country_tag, true>(player_imports.view(), o.player_imports.view());
		});

		ws.w.nation_s.nations.parallel_for_each([&ws, &nation_tarrif_income, tag](nations::country_tag nt) {
			ws.w.nation_s.collected_tariffs.get(nt, tag) = nation_tarrif_income[nt];
		}, concurrency::static_partitioner());

		auto nations_aligned_sz = (uint32_t(nations_max) + 15ui32) & ~16ui32;
		resize(ws.w.economy_s.purchasing_arrays, ws.w.local_player_data.imports_by_country[tag], nations_aligned_sz);

		auto dest_player_imports = get_range(ws.w.economy_s.purchasing_arrays, ws.w.local_player_data.imports_by_country[tag]);
		std::copy_n(player_imports.data(), std::end(dest_player_imports) - dest_player_imports.data(), dest_player_imports.data());

		// determine new prices
		ws.w.nation_s.states.parallel_for_each([&ws, global_demand_by_state, state_production, state_max, aligned_state_max, tag, base_price](nations::state_tag si) {
			auto state_owner = ws.w.nation_s.states.get<state::owner>(si);

			auto demand_in_state = std::max(state_current_demand(ws, si)[tag], 0.001f);
			if(!is_valid_index(state_owner)) // skip remainder for this state
				return;

			auto state_owner_tarrifs = float(ws.w.nation_s.nations.get<nation::tarrifs>(state_owner)) / 100.0f;
			auto state_owner_tarrif_mask = ws.w.nation_s.nations.get<nation::statewise_tarrif_mask>(state_owner);
			auto purchases_for_state = ws.w.nation_s.state_purchases.get(si, tag);

			auto values = get_range(ws.w.economy_s.purchasing_arrays, purchases_for_state);
			auto distance_vector = ws.w.province_s.state_distances.get_row(si);
			auto tariff_mask = get_range(ws.w.economy_s.purchasing_arrays, state_owner_tarrif_mask);

			/*workspace_local.apparent_price = (distance_vector.array() * distance_factor +
				(tarrif_mask.array() * (state_owner_tarrifs) + 1.0f)
				* global_demand_by_state.vector.array() / (state_production_copy.vector.array() + 0.0001f)).matrix();*/

			new_price_accumulator acc_obj(distance_vector.data(), tariff_mask.data(), global_demand_by_state.data(), state_production.data(), values.data(), state_owner_tarrifs);

			ve::execute_serial(uint32_t(state_max + 1), acc_obj);
			const auto final_dot_product =
				((acc_obj.price_times_purchases_accumulator[0] + acc_obj.price_times_purchases_accumulator[1])
					+ (acc_obj.price_times_purchases_accumulator[2] + acc_obj.price_times_purchases_accumulator[3])).reduce();


			auto current_price = state_current_prices(ws, si)[tag];
			
			state_price_delta(ws, si)[tag] = (
				(current_price * (1.0f - price_change_rate) +
				std::clamp(final_dot_product / demand_in_state, 0.01f, base_price * 10.0f) * price_change_rate)
				- current_price
				) / float(price_update_delay);

			/*
			state_price_delta(ws, si)[to_index(tag)] = (
				(current_price * (1.0f - price_change_rate) +
					std::clamp(values.dot(workspace_local.apparent_price) / (demand_in_state > 0 ? demand_in_state : 0.0001f), 0.01f, base_price * 10.0f) * price_change_rate)
				- current_price
				) / float(price_update_delay);*/

		}, concurrency::static_partitioner());

	}

	constexpr goods_qnty_type global_rgo_production_multiplier = goods_qnty_type(7.0);
	constexpr goods_qnty_type global_throughput_multiplier = goods_qnty_type(0.5);
	constexpr float production_scaling_speed_factor = 0.5f;
	constexpr float scale_speed(float v) {
		return 1.0f - production_scaling_speed_factor + production_scaling_speed_factor * v;
	}

	goods_qnty_type province_rgo_production_qnty(world_state const& ws, provinces::province_tag p) {
		auto& worker_data = ws.w.province_s.province_state_container.get<province_state::rgo_worker_data>(p);
		auto rgo_production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(p);
		auto rgo_size = ws.w.province_s.province_state_container.get<province_state::rgo_size>(p);
		auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(p);

		if(is_valid_index(owner) && is_valid_index(rgo_production)) {
			
			const float mobilization_effect = (ws.w.nation_s.nations.get<nation::is_mobilized>(owner) == false) ?
				1.0f : std::max(0.0f, 1.0f - ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mobilisation_size>(owner) * ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mobilisation_economy_impact>(owner));

			auto& rgo_type = ((ws.s.economy_m.goods[rgo_production].flags & good_definition::mined) != 0) ? ws.s.economy_m.rgo_mine : ws.s.economy_m.rgo_farm;

			auto rgo_modifiers = rgo_production_modifiers(ws, worker_data, rgo_type,
				owner, p, rgo_size, rgo_production, mobilization_effect);

			auto output_amount = global_rgo_production_multiplier *
				worker_data.production_scale *
				ws.s.economy_m.goods[rgo_production].base_rgo_value *
				rgo_modifiers.output_modifier *
				rgo_modifiers.throughput_modifier;

			return output_amount;
		} else {
			return goods_qnty_type(0);
		}
	}

	void update_rgo_production(world_state& ws,
		nations::country_tag nid,
		nations::state_tag in_state,
		economy::money_qnty_type* __restrict pay_by_type,
		money_qnty_type output_price,
		economy::money_qnty_type const* __restrict life_needs_cost_by_type,
		provinces::province_tag p, workers_information const& rgo_type, float mobilization_effect) {

		auto& worker_data = ws.w.province_s.province_state_container.get<province_state::rgo_worker_data>(p);
		auto rgo_production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(p);
		auto rgo_size = ws.w.province_s.province_state_container.get<province_state::rgo_size>(p);

		auto rgo_modifiers = rgo_production_modifiers(ws, worker_data, rgo_type,
			nid, p, rgo_size, rgo_production, mobilization_effect);

		auto output_amount = global_rgo_production_multiplier *
			worker_data.production_scale *
			ws.s.economy_m.goods[rgo_production].base_rgo_value *
			rgo_modifiers.output_modifier *
			rgo_modifiers.throughput_modifier;

		money_qnty_type min_wage = money_qnty_type(0);
		money_qnty_type total_worker_pop = money_qnty_type(0);
		for(uint32_t i = 0; i < std::extent_v<decltype(rgo_type.workers)>; ++i) {
			if(is_valid_index(rgo_type.workers[i].type)) {
				min_wage += life_needs_cost_by_type[to_index(rgo_type.workers[i].type)] * money_qnty_type(worker_data.worker_populations[i]) / pop_needs_divisor;
				total_worker_pop += money_qnty_type(worker_data.worker_populations[i]);
			}
		}

		min_wage = std::max(min_wage, 0.0001f);

		ws.w.nation_s.state_production.get(in_state , rgo_production) += output_amount;

		auto rgo_profit = output_amount * output_price - min_wage;

		if(rgo_profit <= 0) {
			for(uint32_t i = 0; i < std::extent_v<decltype(rgo_type.workers)>; ++i) {
				if(is_valid_index(rgo_type.workers[i].type)) {
					pay_by_type[to_index(rgo_type.workers[i].type)] += 
						(life_needs_cost_by_type[to_index(rgo_type.workers[i].type)] * money_qnty_type(worker_data.worker_populations[i]) / pop_needs_divisor) *
						output_amount * output_price / min_wage;
				}
			}
		} else {
			for(uint32_t i = 0; i < std::extent_v<decltype(rgo_type.workers)>; ++i) {
				if(is_valid_index(rgo_type.workers[i].type)) {
					pay_by_type[to_index(rgo_type.workers[i].type)] +=
						(life_needs_cost_by_type[to_index(rgo_type.workers[i].type)] * money_qnty_type(worker_data.worker_populations[i]) / pop_needs_divisor)
						+ rgo_profit * money_qnty_type(0.2) * money_qnty_type(worker_data.worker_populations[i]) / total_worker_pop;
				}
			}
			pay_by_type[to_index(rgo_type.owner.type)] += rgo_profit * money_qnty_type(0.8);
		}
		
		// rescale production
		// worker_data.production_scale = std::clamp(worker_data.production_scale * scale_speed(output_amount * output_price / min_wage), 0.05f, 1.0f);
	}

	money_qnty_type artisan_profit_unit(
		world_state const& ws,
		provinces::province_tag p,
		nations::country_tag owner,
		artisan_type const& atype,
		tagged_array_view<const float, goods_tag, false> prices,
		money_qnty_type artisan_life_needs
	) {

		auto artisan_modifiers = artisan_production_modifiers(ws, owner, p, atype.output_good, 1.0f);

		auto output_amount = 
			atype.output_amount * global_throughput_multiplier *
			artisan_modifiers.output_modifier *
			artisan_modifiers.throughput_modifier;

		auto inputs = ws.s.economy_m.artisan_input_goods.get_row(atype.id);

		auto inputs_cost = ve::dot_product(prices, inputs) * artisan_modifiers.input_modifier * artisan_modifiers.throughput_modifier * global_throughput_multiplier;
		//money_qnty_type min_wage = artisan_life_needs * atype.workforce / pop_needs_divisor;


		return output_amount * prices[atype.output_good] - inputs_cost;
	}


	void update_artisan_production(world_state& ws,
		nations::country_tag nid,
		nations::state_tag in_state,
		economy::money_qnty_type* __restrict pay_by_type,
		tagged_array_view<float, goods_tag, false> current_state_demand,
		tagged_array_view<const float, goods_tag, false> state_prices,
		economy::money_qnty_type const* __restrict life_needs_cost_by_type,
		provinces::province_tag p, float mobilization_effect) {

		auto& artisan_production = ws.w.province_s.province_state_container.get<province_state::artisan_production>(p);
		auto& artisan_production_scale = ws.w.province_s.province_state_container.get<province_state::artisan_production_scale>(p);

		auto artisan_modifiers = artisan_production_modifiers(ws, nid, p, artisan_production, mobilization_effect);
		const auto artisan_tag = ws.s.economy_m.goods[artisan_production].artisan_id;
		const auto& artisan_type = ws.s.economy_m.artisan_types[artisan_tag];

		auto artisan_pop = float(ws.w.province_s.province_demographics.get(p, population::to_demo_tag(ws, ws.s.population_m.artisan)));
		if(artisan_pop == 0)
			return;

		auto common_scale_amount = global_throughput_multiplier * artisan_production_scale * artisan_pop / artisan_type.workforce;

		auto output_amount =  common_scale_amount *
			artisan_type.output_amount *
			artisan_modifiers.output_modifier *
			artisan_modifiers.throughput_modifier;
		
		auto inputs = ws.s.economy_m.artisan_input_goods.get_row(artisan_tag);

		auto inputs_cost = ve::dot_product<goods_tag, false>(state_prices, inputs) * common_scale_amount * artisan_modifiers.input_modifier * artisan_modifiers.throughput_modifier;

		money_qnty_type min_wage = life_needs_cost_by_type[to_index(ws.s.population_m.artisan)] * artisan_pop / pop_needs_divisor;

		auto profit = output_amount * state_prices[artisan_production] - inputs_cost;

		if(profit > 0) {
			pay_by_type[to_index(ws.s.population_m.artisan)] += profit;

			ws.w.nation_s.state_production.get(in_state, artisan_production) += output_amount;
			ve::accumulate_scaled<goods_tag, false>(current_state_demand, inputs, inputs_cost / ve::reduce<goods_tag, false>(inputs));
		}

		//artisan_production_scale = std::clamp(
		//	artisan_production_scale * scale_speed(output_amount * state_prices[to_index(artisan_production)] / (min_wage + inputs_cost)),
		//	0.05f, 1.0f);

		
		if(profit < min_wage) {
			std::uniform_real_distribution<money_qnty_type> dist(money_qnty_type(0), 4.0f);
			if(dist(get_local_generator()) > sqrt(std::clamp(profit, 0.0f, min_wage) / min_wage)) {
				artisan_production = ws.s.economy_m.artisan_types[get_profitable_artisan(ws, p)].output_good;
				//artisan_production_scale = 1.0f;
			}
		}
		
	}

	void update_factory_production(world_state& ws,
		nations::country_tag nid,
		nations::state_tag in_state,
		economy::money_qnty_type* __restrict pay_by_type,
		tagged_array_view<float, goods_tag, false> current_state_demand,
		tagged_array_view<const float, goods_tag, false> state_prices,
		economy::money_qnty_type const* __restrict life_needs_cost_by_type,
		factory_instance& instance, provinces::province_tag capital, float mobilization_effect) {

		auto& f_type = *instance.type;

		auto inputs = ws.s.economy_m.factory_input_goods.get_row(f_type.id);

		const auto factory_modifiers = factory_production_modifiers(ws, instance.worker_data, f_type.bonuses, f_type.factory_workers,
			nid, capital, instance.level, f_type.output_good, mobilization_effect);

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

		auto inputs_cost = ve::dot_product<goods_tag, false>(state_prices, inputs) *
			 instance.worker_data.production_scale * factory_modifiers.input_modifier * factory_modifiers.throughput_modifier * global_throughput_multiplier;
		auto output_value = output_amount * state_prices[f_type.output_good];
		auto profit = output_value - min_wage - inputs_cost;

		ws.w.nation_s.state_production.get(in_state, f_type.output_good) += output_amount;

		ve::accumulate_scaled<goods_tag, false>(current_state_demand, inputs, inputs_cost / ve::reduce<goods_tag, false>(inputs));

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


	void fill_base_spending_values(world_state const& ws, state_pops_summary& state_pops, nations::country_tag owner) {
		int32_t total_types = int32_t(state_pops.ln_costs_by_type.size());

		for(int32_t i = 1; i < total_types; ++i) {
			auto ptype = population::pop_type_tag(population::pop_type_tag::value_base_t(i - 1));
			auto pop_strata = ws.s.population_m.pop_types[ptype].flags & population::pop_type::strata_mask;

			state_pops.ln_money_div_qnty_by_type[i] = 11.0f;
			state_pops.en_money_div_qnty_by_type[i] =
				[pop_strata, is_slave = (ws.s.population_m.slave == ptype), is_capitalist = (ws.s.population_m.capitalist == ptype)](){
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
			state_pops.lx_money_div_qnty_by_type[i] =
				[pop_strata, is_slave = (ws.s.population_m.slave == ptype), is_capitalist = (ws.s.population_m.capitalist == ptype)](){
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

			auto ln =
				ws.s.population_m.life_needs.get_row(ptype);
			auto  en =
				ws.s.population_m.everyday_needs.get_row(ptype);
			auto xn =
				ws.s.population_m.luxury_needs.get_row(ptype);

			auto enabled_goods = ws.w.nation_s.active_goods.get_row(owner);

			const auto total_ln = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<>(), [enabled_goods, &ln](int32_t i) { 
					auto gt =  goods_tag(goods_tag::value_base_t(i));
					return bit_vector_test(enabled_goods, gt) ? ln[gt] : goods_qnty_type(0);
			});
			const auto total_en = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<>(), [enabled_goods, &en](int32_t i) {
					auto gt = goods_tag(goods_tag::value_base_t(i));
					return bit_vector_test(enabled_goods, gt) ? en[gt] : goods_qnty_type(0);
			});
			const auto total_xn = std::transform_reduce(integer_iterator(0), integer_iterator(ws.s.economy_m.goods_count),
				goods_qnty_type(0), std::plus<>(), [enabled_goods, &xn](int32_t i) {
					auto gt = goods_tag(goods_tag::value_base_t(i));
					return bit_vector_test(enabled_goods, gt) ? xn[gt] : goods_qnty_type(0);
			});

			state_pops.ln_money_div_qnty_by_type[i] /= (total_ln > 0 ? total_ln : 1.0f);
			state_pops.en_money_div_qnty_by_type[i] /= total_en;
			state_pops.lx_money_div_qnty_by_type[i] /= total_xn;
		}
	}

	struct consumption_operation {
		float* const pop_sizes;
		int32_t* const pop_types;
		float* const pop_money;

		float* const satisfaction;

		float* const ln_spending;
		float* const en_spending;
		float* const lx_spending;

		float* const ln_money_div_qnty_by_type;
		float* const en_money_div_qnty_by_type;
		float* const lx_money_div_qnty_by_type;

		float* const ln_costs_by_type;
		float* const en_costs_by_type;
		float* const lx_costs_by_type;

		consumption_operation(state_pops_summary& state_pops) :
			pop_sizes(state_pops.size.data()), pop_types((int32_t*)(state_pops.pop_types.data())), pop_money(state_pops.money.data()),
			satisfaction(state_pops.satisfaction.data()), ln_spending(state_pops.ln_spending.data()), en_spending(state_pops.en_spending.data()),
			lx_spending(state_pops.lx_spending.data()), ln_money_div_qnty_by_type(state_pops.ln_money_div_qnty_by_type.data()),
			en_money_div_qnty_by_type(state_pops.en_money_div_qnty_by_type.data()), lx_money_div_qnty_by_type(state_pops.lx_money_div_qnty_by_type.data()),
			ln_costs_by_type(state_pops.ln_costs_by_type.data()), en_costs_by_type(state_pops.en_costs_by_type.data()),
			lx_costs_by_type(state_pops.lx_costs_by_type.data())
		{}

		template<typename T>
		void operator()(T executor) {
			auto pop_size_multiplier = executor.load(pop_sizes) * executor.constant(1.0f / pop_needs_divisor);
			auto type_indices = executor.load(pop_types);

			auto gathered_ln_mdt = executor.gather_load(ln_money_div_qnty_by_type, type_indices);
			auto gathered_en_mdt = executor.gather_load(en_money_div_qnty_by_type, type_indices);
			auto gathered_lx_mdt = executor.gather_load(lx_money_div_qnty_by_type, type_indices);

			/*
			state_demand += (ln * ln_money / total_ln).matrix();
			state_demand += (en * en_money / total_en).matrix();
			state_demand += (xn * lx_money / total_xn).matrix();
			*/
			executor.store(ln_spending, gathered_ln_mdt * pop_size_multiplier);
			executor.store(en_spending, gathered_en_mdt * pop_size_multiplier);
			executor.store(lx_spending, gathered_lx_mdt * pop_size_multiplier);

			auto gathered_ln_cost = executor.gather_load(ln_costs_by_type, type_indices);
			auto gathered_en_cost = executor.gather_load(en_costs_by_type, type_indices);
			auto gathered_lx_cost = executor.gather_load(lx_costs_by_type, type_indices);

			auto money = executor.load(pop_money);

			auto tier_one = pop_size_multiplier * gathered_ln_cost;
			auto in_ln_group = money < tier_one;

			auto multiplied_en_cost = pop_size_multiplier * gathered_en_cost;
			auto tier_two = tier_one + multiplied_en_cost;
			auto in_en_group = money < tier_two;

			auto remainder_after_tier_two = money - tier_two;

			auto new_pop_money = ve::select(in_en_group, executor.zero(), remainder_after_tier_two * executor.constant(savings_rate));
			executor.store(pop_money, new_pop_money);

			auto tier_three_fraction = remainder_after_tier_two * executor.constant(1.0f - savings_rate) / gathered_lx_cost;
			auto new_satisfaction =
				ve::select(in_en_group,
					ve::select(in_ln_group, money / tier_one, executor.constant(1.0f) + ((money - tier_one) / multiplied_en_cost)), // ln or en branch
					ve::select(tier_three_fraction < executor.constant(1.0f), executor.constant(2.0f) + tier_three_fraction, executor.constant(3.0f)) // lx branch
				); 
			
			executor.store(satisfaction, new_satisfaction);
								
			/*
			if(initial_money < (pop_size_multiplier * life_needs_cost_by_type[to_index(ptype)])) {
				// case: can only partially satisfy life needs
				auto fraction = initial_money / (pop_size_multiplier * life_needs_cost_by_type[to_index(ptype)]);
				ws.w.population_s.pops.set<pop::needs_satisfaction>(po, fraction);

				if constexpr(adjust_money) {
					p_money = 0.0f;
				}
			} else {
				initial_money -= pop_size_multiplier * life_needs_cost_by_type[to_index(ptype)];

				if(initial_money < pop_size_multiplier * everyday_needs_cost_by_type[to_index(ptype)]) {
					// case: can partially satisfy everyday needs

					auto fraction = initial_money / (pop_size_multiplier * everyday_needs_cost_by_type[to_index(ptype)]);
					ws.w.population_s.pops.set<pop::needs_satisfaction>(po, 1.0f + fraction);

					if constexpr(adjust_money) {
						p_money = 0.0f;
					}
				} else {
					// case: can fully satisfy everyday needs
					state_demand += (en * en_money / total_en).matrix();

					initial_money -= pop_size_multiplier * everyday_needs_cost_by_type[to_index(ptype)];

					// remainder of money spent on luxury needs
					auto last_fraction = initial_money * (money_qnty_type(1) - savings_rate) / (pop_size_multiplier * luxury_needs_cost_by_type[to_index(ptype)]);
					last_fraction = last_fraction > 1.0f ? (1.0f + (last_fraction - 1.0f) * 0.1f) : last_fraction;
					ws.w.population_s.pops.set<pop::needs_satisfaction>(po, 2.0f + last_fraction);

					if constexpr(adjust_money) {
						p_money = initial_money * savings_rate;
					}
				}
			}
			*/
		}
	};

	struct store_money_and_satisfaction_operation {
		int32_t* const pop_ids;
		float const* const money;
		float const* const satisfaction;

		float* const money_dest;
		float* const satisfaction_dest;

		store_money_and_satisfaction_operation(world_state& ws, state_pops_summary const& state_pops) :
			pop_ids((int32_t*)(state_pops.pop_ids.data())),
			money(state_pops.money.data()), satisfaction(state_pops.satisfaction.data()),
			money_dest(ws.w.population_s.pops.get_row<pop::money>().data()),
			satisfaction_dest(ws.w.population_s.pops.get_row<pop::needs_satisfaction>().data())
		{ }

		template<typename T>
		__forceinline void operator()(T executor) {
			auto indices = executor.load(pop_ids);
			auto money_source = executor.load(money);
			auto satisfaction_source = executor.load(satisfaction);

			executor.scatter_store(money_dest, money_source, indices);
			executor.scatter_store(satisfaction_dest, satisfaction_source, indices);
		}
	};

	void apply_pop_demand(world_state& ws, nations::state_tag in_state, state_pops_summary const& state_pops) {
		auto state_demand = state_current_demand(ws, in_state);

		const int32_t total = int32_t(state_pops.pop_ids.size());
		for(int32_t i = 0; i < total; ++i) {
			population::pop_type_tag ptype = state_pops.pop_types[i];
			if(state_pops.ln_spending[i] > 0)
				ve::accumulate_scaled<economy::goods_tag,false>(
					state_demand,
					ws.s.population_m.life_needs.get_row(ptype),
					state_pops.ln_spending[i]);
			if(state_pops.en_spending[i] > 0)
				ve::accumulate_scaled<economy::goods_tag, false>(
					state_demand,
					ws.s.population_m.everyday_needs.get_row(ptype),
					state_pops.en_spending[i]);
			if(state_pops.lx_spending[i] > 0)
				ve::accumulate_scaled<economy::goods_tag, false>(
					state_demand,
					ws.s.population_m.luxury_needs.get_row(ptype),
					state_pops.lx_spending[i]);
		}
	}
	
	template<bool adjust_money>
	void update_demand_and_production(world_state& ws, nations::state_tag si) {
		auto state_owner = ws.w.nation_s.states.get<state::owner>(si);
		if(!is_valid_index(state_owner))
			return;

		const auto state_capital_id = nations::get_state_capital(ws, si);
		if(!is_valid_index(state_capital_id))
			return;
		
		auto current_state_demand = state_current_demand(ws, si);
		auto state_prices = state_current_prices(ws, si);

		const float mobilization_effect = (ws.w.nation_s.nations.get<nation::is_mobilized>(state_owner) == false) ?
			1.0f : std::max(0.0f, 1.0f - ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mobilisation_size>(state_owner) * ws.w.nation_s.modifier_values.get<modifiers::national_offsets::mobilisation_economy_impact>(state_owner));

		std::fill(current_state_demand.data(), std::end(current_state_demand), 0.0f);

		tagged_array_view<float, goods_tag, false> masked_prices(
			(economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count),
			ws.s.economy_m.aligned_32_goods_count);
		create_masked_prices(masked_prices, ws, si);

		state_pops_summary state_pops(ws, si);
		fill_base_spending_values(ws, state_pops, state_owner);

		fill_needs_costs_arrays(ws, si, state_capital_id, masked_prices,
			state_pops.ln_costs_by_type.data() + 1,
			state_pops.en_costs_by_type.data() + 1,
			state_pops.lx_costs_by_type.data() + 1);

		ve::execute_serial(uint32_t(state_pops.pop_ids.size()), consumption_operation(state_pops));

		apply_pop_demand(ws, si, state_pops);

		nations::for_each_province(ws, si, [&ws, current_state_demand, &masked_prices, state_owner, state_prices, si,
			&state_pops, mobilization_effect](provinces::province_tag ps) {

			economy::money_qnty_type* province_pay_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
			std::fill_n(province_pay_by_type, ws.s.population_m.count_poptypes, economy::money_qnty_type(0));
			auto province_population_by_type = &(ws.w.province_s.province_demographics.get_row(ps)[population::to_demo_tag(ws, population::pop_type_tag(0))]);

			auto rgo_production = ws.w.province_s.province_state_container.get<province_state::rgo_production>(ps);
			auto& rgo_type = ((ws.s.economy_m.goods[rgo_production].flags & good_definition::mined) != 0) ? ws.s.economy_m.rgo_mine : ws.s.economy_m.rgo_farm;

			update_rgo_production(ws,
				state_owner,
				si,
				province_pay_by_type,
				state_prices[rgo_production],
				state_pops.ln_costs_by_type.data() + 1,
				ps, rgo_type, mobilization_effect);
			update_artisan_production(ws,
				state_owner,
				si,
				province_pay_by_type,
				current_state_demand,
				state_prices,
				state_pops.ln_costs_by_type.data() + 1,
				ps, mobilization_effect);

			update_rgo_employment(ws, ps);

			const int32_t total = int32_t(state_pops.pop_ids.size());
			for(int32_t i = 0; i < total; ++i) {
				if(state_pops.location[i] == ps) {
					auto ptype = state_pops.pop_types[i];
					state_pops.money[i] += province_pay_by_type[to_index(ptype)] * state_pops.size[i] / float(province_population_by_type[to_index(ptype)]);
				}
			}
		});

		
		economy::money_qnty_type* state_pay_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		std::fill_n(state_pay_by_type, ws.s.population_m.count_poptypes, economy::money_qnty_type(0));
		[[maybe_unused]] auto state_population_by_type = &(ws.w.nation_s.state_demographics.get_row(si)[population::to_demo_tag(ws, population::pop_type_tag(0))]);

		auto& factories = ws.w.nation_s.states.get<state::factories>(si);
		for(auto& f : factories) {
			if(factory_is_open(f)) {
				update_factory_production(ws,
					state_owner,
					si,
					state_pay_by_type,
					current_state_demand,
					state_prices,
					state_pops.ln_costs_by_type.data() + 1,
					f, state_capital_id, mobilization_effect);
			}
		}

		update_factories_employment(ws, si);
		

		const int32_t total = int32_t(state_pops.pop_ids.size());
		for(int32_t i = 0; i < total; ++i) {
			auto ptype = state_pops.pop_types[i];
			state_pops.money[i] += state_pay_by_type[to_index(ptype)] * state_pops.size[i] / float(state_population_by_type[to_index(ptype)]);
		}

		ve::execute_serial(uint32_t(total), store_money_and_satisfaction_operation(ws, state_pops));
	}

	artisan_type_tag get_profitable_artisan(world_state const& ws, provinces::province_tag ps) {
		boost::container::small_vector<std::pair<artisan_type_tag, money_qnty_type>, 32, concurrent_allocator<std::pair<artisan_type_tag, money_qnty_type>>> result;

		auto owner_id = ws.w.province_s.province_state_container.get<province_state::owner>(ps);
		auto in_state = ws.w.province_s.province_state_container.get<province_state::state_instance>(ps);
		
		if(!is_valid_index(owner_id) || !is_valid_index(in_state))
			return artisan_type_tag(0);
		

		auto prices = state_current_prices(ws, in_state);
		auto artisan_life_needs = get_life_needs_cost(ws, in_state, ws.s.population_m.artisan);
		auto enabled_goods = ws.w.nation_s.active_goods.get_row(owner_id);

		money_qnty_type profit_sum = money_qnty_type(0);
		for(auto& atype : ws.s.economy_m.artisan_types) {
			if(bit_vector_test(enabled_goods, atype.output_good)) {
				auto profit = artisan_profit_unit(ws, ps, owner_id, atype, prices, artisan_life_needs);
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
			if(bit_vector_test(enabled_goods, atype.output_good)) {
				result_b.push_back(atype.id);
			}
		}
		std::uniform_int_distribution<int32_t> d(0, int32_t(result_b.size() - 1));
		return result_b[d(get_local_generator())];
	}


	void economy_demand_adjustment_tick(world_state& ws) {
		/*ws.w.nation_s.state_production.reset();

		ws.w.nation_s.states.parallel_for_each([&ws](nations::state_tag si) {
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> prices(state_current_prices(ws, si).data(), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> delta(state_price_delta(ws, si).data(), ws.s.economy_m.aligned_32_goods_count);
			prices += delta;
			update_demand_and_production<false>(ws, si);
		});
		concurrency::parallel_for(1ui32, ws.s.economy_m.goods_count, [&ws, state_count = ws.w.nation_s.states.size(), nations_count = ws.w.nation_s.nations.size()](uint32_t i) {
			economy_single_good_tick(ws, goods_tag(goods_tag::value_base_t(i)), state_count, nations_count);
		});*/

		economy_update_tick(ws);
	}

	void economy_update_tick(world_state& ws) {
		ws.w.nation_s.state_production.reset();

		ws.w.nation_s.states.parallel_for_each([&ws](nations::state_tag si) {
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> prices(state_current_prices(ws, si).data(), ws.s.economy_m.aligned_32_goods_count);
			Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> delta(state_price_delta(ws, si).data(), ws.s.economy_m.aligned_32_goods_count);
			prices += delta;
			update_demand_and_production<true>(ws, si);
		});
		update_construction_and_projects(ws); // after demand update to added demand isnt clobbered

		auto offset = (to_index(ws.w.current_date)) & (price_update_delay - 1);
		concurrency::parallel_for(1 + offset, int32_t(ws.s.economy_m.goods_count), price_update_delay, [&ws, state_count = ws.w.nation_s.states.size(), nations_count = ws.w.nation_s.nations.size()](uint32_t i) {
			economy_single_good_tick(ws, goods_tag(goods_tag::value_base_t(i)), state_count, nations_count);
		});
		
		collect_taxes(ws);

		//collect tarrif income, pay pops, manage debt
		ws.w.nation_s.nations.parallel_for_each([&ws](nations::country_tag n) {
			auto tincome = ws.w.nation_s.collected_tariffs.get_row(n);
			ws.w.nation_s.national_stockpiles.get(n, economy::money_good) += std::reduce(std::begin(tincome), std::end(tincome), economy::money_qnty_type(0), std::plus<>());
			pay_unemployement_pensions_salaries(ws, n);

			auto amount = calculate_daily_debt_payment(ws, n);
			auto current_money = ws.w.nation_s.national_stockpiles.get(n, economy::money_good);
			if(current_money >= amount) {
				ws.w.nation_s.nations.get<nation::national_debt>(n) -= amount / 2.0f;
				ws.w.nation_s.national_stockpiles.get(n, economy::money_good) = current_money - amount;
			} else {
				ws.w.nation_s.nations.get<nation::national_debt>(n) += (amount - current_money) - amount / 2.0f;
				ws.w.nation_s.national_stockpiles.get(n, economy::money_good) = money_qnty_type(0);
			}
		});
	}

	void collect_taxes(world_state& ws) {
		ws.w.local_player_data.collected_poor_tax = 0;
		ws.w.local_player_data.collected_middle_tax = 0;
		ws.w.local_player_data.collected_rich_tax = 0;

		ws.w.nation_s.nations.parallel_for_each([&ws](nations::country_tag n) {
			auto& tax_base = ws.w.nation_s.nations.get<nation::tax_base>(n);
			tax_base = 0.0f;
			auto taxeff = std::max(ws.s.modifiers_m.global_defines.base_country_tax_efficiency + ws.w.nation_s.modifier_values.get<modifiers::national_offsets::tax_efficiency>(n), 0.05f);
			auto pt = float(ws.w.nation_s.nations.get<nation::poor_tax>(n)) / 100.0f;
			auto mt = float(ws.w.nation_s.nations.get<nation::middle_tax>(n)) / 100.0f;
			auto rt = float(ws.w.nation_s.nations.get<nation::rich_tax>(n)) / 100.0f;
			if(n != ws.w.local_player_nation) {
				money_qnty_type total = 0;
				nations::for_each_pop(ws, n, [&total, &ws, &tax_base, n, taxeff, pt, mt, rt](population::pop_tag p) {
					auto p_type = ws.w.population_s.pops.get<pop::type>(p);
					auto& p_money = ws.w.population_s.pops.get<pop::money>(p);
					auto strata = ws.s.population_m.pop_types[p_type].flags & population::pop_type::strata_mask;
					if(strata == population::pop_type::strata_poor) {
						auto collected = std::min(p_money * pt * taxeff, p_money);
						total += collected;
						tax_base += p_money * taxeff;
						p_money -= collected;
					} else if(strata == population::pop_type::strata_middle) {
						auto collected = std::min(p_money * mt * taxeff, p_money);
						total += collected;
						tax_base += p_money * taxeff;
						p_money -= collected;
					} else { // strata rich
						auto collected = std::min(p_money * rt * taxeff, p_money);
						total += collected;
						tax_base += p_money * taxeff;
						p_money -= collected;
					}
				});
				ws.w.nation_s.national_stockpiles.get(n, economy::money_good) += total;
			} else {
				money_qnty_type total = 0;
				nations::for_each_pop(ws, n, [&total, &ws, &tax_base, n, taxeff, pt, mt, rt](population::pop_tag p) {
					auto p_type = ws.w.population_s.pops.get<pop::type>(p);
					auto& p_money = ws.w.population_s.pops.get<pop::money>(p);
					auto strata = ws.s.population_m.pop_types[p_type].flags & population::pop_type::strata_mask;
					if(strata == population::pop_type::strata_poor) {
						auto collected = std::min(p_money * pt * taxeff, p_money);
						total += collected;
						ws.w.local_player_data.collected_poor_tax += p_money * taxeff;
						tax_base += p_money * taxeff;
						p_money -= collected;
					} else if(strata == population::pop_type::strata_middle) {
						auto collected = std::min(p_money * mt * taxeff, p_money);
						total += collected;
						ws.w.local_player_data.collected_middle_tax += p_money * taxeff;
						tax_base += p_money * taxeff;
						p_money -= collected;
					} else { // strata rich
						auto collected = std::min(p_money * rt * taxeff, p_money);
						total += collected;
						ws.w.local_player_data.collected_rich_tax += p_money * taxeff;
						tax_base += p_money * taxeff;
						p_money -= collected;
					}
				});
				ws.w.nation_s.national_stockpiles.get(n, economy::money_good) += total;
			}
		});

		update_bankrupcy(ws);
	}

	economy::money_qnty_type project_player_tarrif_income(world_state const& ws, float tarrif_amount) {
		if(auto player = ws.w.local_player_nation; player) {
			auto overlord_id = ws.w.nation_s.nations.get<nation::overlord>(player);
			auto sphere_leader_id = ws.w.nation_s.nations.get<nation::sphere_leader>(player);

			return std::transform_reduce(integer_iterator(1), integer_iterator(ws.s.economy_m.goods_count), money_qnty_type(0), std::plus<>(), [&ws, tarrif_amount, overlord_id, sphere_leader_id](int32_t i) {
				auto atag = ws.w.local_player_data.imports_by_country[goods_tag(goods_tag::value_base_t(i))];
				auto sz = get_size(ws.w.economy_s.purchasing_arrays, atag);
				auto ptr = get_range(ws.w.economy_s.purchasing_arrays, atag);

				
				return std::transform_reduce(integer_iterator(0), integer_iterator(sz), money_qnty_type(0), std::plus<>(), [ptr, tarrif_amount, overlord_id, sphere_leader_id](int32_t i) {
					if(to_index(overlord_id) != i && to_index(sphere_leader_id) != i)
						return tarrif_amount * *(std::begin(ptr) + i);
					else
						return money_qnty_type(0);
				});
			});
		} else {
			return economy::money_qnty_type(0);
		}
	}

	constexpr float luxury_pay_fraction = 0.5f;

	money_qnty_type military_spending_amount(world_state const& ws, nations::country_tag n) {
		auto nid = n;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);
		
		auto military_spending = ws.w.nation_s.nations.get<nation::f_military_spending>(n);
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

		return sum * (2.0f - ws.w.nation_s.nations.get<nation::national_administrative_efficiency>(n));
	}

	money_qnty_type education_spending_amount(world_state const& ws, nations::country_tag n) {
		auto nid = n;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);

		auto education_spending = ws.w.nation_s.nations.get<nation::f_education_spending>(n);
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
		
		return sum * (2.0f - ws.w.nation_s.nations.get<nation::national_administrative_efficiency>(n));
	}

	money_qnty_type administrative_spending_amount(world_state const& ws, nations::country_tag n) {
		auto nid = n;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);

		auto education_spending = ws.w.nation_s.nations.get<nation::f_administrative_spending>(n);
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

		return sum * (2.0f - ws.w.nation_s.nations.get<nation::national_administrative_efficiency>(n));
	}

	money_qnty_type social_spending_amount(world_state const& ws, nations::country_tag n) {

		auto nid = n;
		if(!ws.w.nation_s.nations.is_valid_index(nid))
			return money_qnty_type(0);

		auto social_spending = ws.w.nation_s.nations.get<nation::f_social_spending>(n);
		auto pension_fraction = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::pension_level>(n) * social_spending;
		auto unemployment_fraction = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::unemployment_benefit>(n) * social_spending;

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

		auto cost_with_waste = est_costs * (2.0f - ws.w.nation_s.nations.get<nation::national_administrative_efficiency>(n));
		return cost_with_waste;
	}

	void pay_unemployement_pensions_salaries(world_state& ws, nations::country_tag n) {
		auto ncap = ws.w.nation_s.nations.get<nation::current_capital>(n);
		if(!is_valid_index(ncap))
			return;

		auto capital_state_id = ws.w.province_s.province_state_container.get<province_state::state_instance>(ncap);
		auto capital_of_capital = nations::get_state_capital(ws, capital_state_id);

		tagged_array_view<float, goods_tag, false> masked_prices(
			(economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.economy_m.aligned_32_goods_count),
			ws.s.economy_m.aligned_32_goods_count);
		create_masked_prices(masked_prices, ws, capital_state_id);

		economy::money_qnty_type* life_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		economy::money_qnty_type* everyday_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);
		economy::money_qnty_type* luxury_needs_cost_by_type = (economy::money_qnty_type*)_alloca(sizeof(economy::money_qnty_type) * ws.s.population_m.count_poptypes);

		fill_needs_costs_arrays(ws, capital_state_id, capital_of_capital, masked_prices, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type);


		auto admin_spending = ws.w.nation_s.nations.get<nation::f_administrative_spending>(n);
		auto education_spending = ws.w.nation_s.nations.get<nation::f_education_spending>(n);
		auto military_spending = ws.w.nation_s.nations.get<nation::f_military_spending>(n);
		auto social_spending = ws.w.nation_s.nations.get<nation::f_social_spending>(n);
		auto pension_fraction = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::pension_level>(n) * social_spending;
		auto unemployment_fraction = ws.w.nation_s.modifier_values.get<modifiers::national_offsets::unemployment_benefit>(n) * social_spending;

		economy::money_qnty_type est_costs = 0;

		nations::for_each_state(ws, n, [&ws, &est_costs, pension_fraction, unemployment_fraction, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, admin_spending, education_spending, military_spending](nations::state_tag si) {
			if(!nations::is_colonial_or_protectorate(ws, si)) {
				nations::for_each_pop(ws, si, [&ws, &est_costs, pension_fraction, unemployment_fraction, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, admin_spending, education_spending, military_spending](population::pop_tag p) {
					auto pop_size = float(ws.w.population_s.pop_demographics.get(p, population::total_population_tag));
					if(pop_size == 0)
						return;

					auto pop_size_multiplier = pop_size / pop_needs_divisor;
					auto unemployment = 1.0f - float(ws.w.population_s.pop_demographics.get(p, population::total_employment_tag)) / pop_size;
					auto ptype = ws.w.population_s.pops.get<pop::type>(p);
					auto& p_money = ws.w.population_s.pops.get<pop::money>(p);

					auto pamount = life_needs_cost_by_type[to_index(ptype)] * (pension_fraction + unemployment_fraction * unemployment) * pop_size_multiplier;
					p_money += pamount;
					est_costs += pamount;

					if(ptype == ws.s.population_m.bureaucrat) {
						auto amount = pop_size_multiplier *
							admin_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					} else if(ptype == ws.s.population_m.clergy) {
						auto amount =
							pop_size_multiplier *
							education_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					} else if(ptype == ws.s.population_m.soldier) {
						auto amount =
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					} else if(ptype == ws.s.population_m.officer) {
						auto amount =
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					}
				});
			} else {
				nations::for_each_pop(ws, si, [&ws, &est_costs, life_needs_cost_by_type, everyday_needs_cost_by_type, luxury_needs_cost_by_type, admin_spending, education_spending, military_spending](population::pop_tag p) {
					auto pop_size = float(ws.w.population_s.pop_demographics.get(p, population::total_population_tag));
					auto pop_size_multiplier = pop_size / pop_needs_divisor;

					auto ptype = ws.w.population_s.pops.get<pop::type>(p);
					auto& p_money = ws.w.population_s.pops.get<pop::money>(p);

					if(ptype == ws.s.population_m.bureaucrat) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							admin_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					} else if(ptype == ws.s.population_m.clergy) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							education_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					} else if(ptype == ws.s.population_m.soldier) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					} else if(ptype == ws.s.population_m.officer) {
						auto amount =
							0.5f * // colonial reduction
							pop_size_multiplier *
							military_spending *
							(life_needs_cost_by_type[to_index(ptype)] + everyday_needs_cost_by_type[to_index(ptype)] + luxury_pay_fraction * luxury_needs_cost_by_type[to_index(ptype)]);
						p_money += amount;
						est_costs += amount;
					}
				});
			}
		});

		auto cost_with_waste = est_costs * (2.0f - ws.w.nation_s.nations.get<nation::national_administrative_efficiency>(n));
		auto diff = ws.w.nation_s.national_stockpiles.get(n, economy::money_good) - cost_with_waste;
		if(diff < money_qnty_type(0)) {
			ws.w.nation_s.national_stockpiles.get(n, economy::money_good) = 0;
			ws.w.nation_s.nations.get<nation::national_debt>(n) += -diff;
		} else {
			ws.w.nation_s.national_stockpiles.get(n, economy::money_good) = diff;
		}
	}

}
