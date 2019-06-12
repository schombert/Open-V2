#include "effects.h"
#include "codes.h"
#include <random>
#include "world_state\\world_state.h"
#include "nations\\nations_functions.hpp"
#include "provinces\\province_functions.hpp"
#include "population\\population_functions.hpp"
#include "governments\\governments_functions.h"
#include "cultures\\cultures_functions.h"
#include "military\\military_functions.h"
#include "modifiers\\modifier_functions.h"
#include "issues\\issues_functions.h"
#include "technologies\\technologies_functions.h"
#include "economy\\economy_functions.h"
#include "events\\event_functions.h"

namespace triggers {
	int32_t get_effect_payload_size(const uint16_t* data) {
		const auto payload_mask = uint16_t(-int16_t((effect_codes::no_payload & data[0]) == 0));
		return payload_mask & data[1];
	}
	int32_t effect_scope_data_payload(uint16_t code) {
		const auto masked_code = code & effect_codes::code_mask;
		if ((masked_code == effect_codes::tag_scope) |
			(masked_code == effect_codes::integer_scope) |
			(masked_code == effect_codes::pop_type_scope_nation) |
			(masked_code == effect_codes::pop_type_scope_state) |
			(masked_code == effect_codes::pop_type_scope_province) |
			(masked_code == effect_codes::region_scope) |
			(masked_code == effect_codes::random_scope))
			return 1 + ((code & effect_codes::scope_has_limit) != 0);
		return 0 + ((code & effect_codes::scope_has_limit) != 0);
	}

	int32_t get_random_list_effect_chances_sum(const uint16_t* data) {
		const auto source_size = 1 + get_effect_payload_size(data);
		int32_t sum = 0;

		auto sub_units_start = data + 3;
		while (sub_units_start < data + source_size) {
			sum += sub_units_start[0];
			sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1);
		}

		return sum;
	}

	const uint16_t* get_random_list_effect(const uint16_t* data, int32_t random_value) {
		const auto source_size = 1 + get_effect_payload_size(data);

		auto sub_units_start = data + 3;
		while (sub_units_start < data + source_size) {
			random_value -= static_cast<int32_t>(sub_units_start[0]);
			if (random_value < 0)
				return sub_units_start + 1;
			sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1);
		}

		return data + 2; // default to first member 
	}

#ifdef __llvm__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

	namespace {

#define EFFECT_PARAMTERS uint16_t const* tval, world_state& ws, parameter primary_slot, parameter this_slot, parameter from_slot, jsf_prng& gen


		void _execute_effect(EFFECT_PARAMTERS);

		inline void apply_subeffects(EFFECT_PARAMTERS) {
			const auto source_size = 1 + get_effect_payload_size(tval);
			auto sub_units_start = tval + 2 + effect_scope_data_payload(tval[0]);

			while(sub_units_start < tval + source_size) {
				_execute_effect(sub_units_start, ws, primary_slot, this_slot, from_slot, gen);
				sub_units_start += 1 + get_effect_payload_size(sub_units_start);
			}
		}

		void es_generic_scope(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, primary_slot, this_slot, from_slot, gen);
		}

		void es_x_neighbor_province_scope(EFFECT_PARAMTERS) {
			auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(to_prov(primary_slot));

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;
				
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p))
							&& test_trigger(limit, ws, p, this_slot, from_slot)) {
							rlist.push_back(p);
						}
					}
				} else {
					for(auto p : neighbor_range) {
						if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p)))
							rlist.push_back(p);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);

					for(auto p : neighbor_range) {
						if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p))
							&& test_trigger(limit, ws, p, this_slot, from_slot)) {

							apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
						}
					}
				} else {
					for(auto p : neighbor_range) {
						if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p)))
							apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
					}
				}
			}
		}
		void es_x_neighbor_country_scope(EFFECT_PARAMTERS) {
			auto neighbor_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(to_nation(primary_slot)));

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::country_tag, 16, concurrent_allocator<nations::country_tag>> rlist;
				
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n : neighbor_range) {
						if(test_trigger(limit, ws, n, this_slot, from_slot))
							rlist.push_back(n);
					}
				} else {
					for(auto n : neighbor_range)
						rlist.push_back(n);
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);

					for(auto n : neighbor_range) {
						if(test_trigger(limit, ws, n, this_slot, from_slot))
							apply_subeffects(tval, ws, n, this_slot, from_slot, gen);
					}
				} else {
					for(auto n : neighbor_range) {
						apply_subeffects(tval, ws, n, this_slot, from_slot, gen);
					}
				}
			}
		}
		void es_x_country_scope_nation(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::country_tag, 64, concurrent_allocator<nations::country_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					ws.w.nation_s.nations.for_each([&rlist, limit, &ws, primary_slot, this_slot, from_slot](nations::country_tag n) {
						if(n != to_nation(primary_slot) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) && test_trigger(limit, ws, n, this_slot, from_slot))
							rlist.push_back(n);
					});
				} else {
					ws.w.nation_s.nations.for_each([&rlist, &ws, primary_slot](nations::country_tag n) {
						if(n != to_nation(primary_slot) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)))
							rlist.push_back(n);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					ws.w.nation_s.nations.for_each([tval, &gen, limit, &ws, this_slot, from_slot](nations::country_tag n) {
						if(test_trigger(limit, ws, n, this_slot, from_slot))
							apply_subeffects(tval, ws, n, this_slot, from_slot, gen);
					});
				} else {
					ws.w.nation_s.nations.for_each([tval, &gen, &ws, this_slot, from_slot](nations::country_tag n) {
						apply_subeffects(tval, ws, n, this_slot, from_slot, gen);
					});
				}
			}
		}
		void es_x_country_scope(EFFECT_PARAMTERS) {
			es_x_country_scope_nation(tval, ws, nations::country_tag(), this_slot, from_slot, gen);
		}
		void es_x_empty_neighbor_province_scope(EFFECT_PARAMTERS) {
			auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(to_prov(primary_slot));

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p))
							&& test_trigger(limit, ws, p, this_slot, from_slot)) {
							rlist.push_back(p);
						}
					}
				} else {
					for(auto p : neighbor_range) {
						if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p)))
							rlist.push_back(p);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);

					for(auto p : neighbor_range) {
						if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p))
							&& test_trigger(limit, ws, p, this_slot, from_slot)) {
							apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
						}
					}
				} else {
					for(auto p : neighbor_range) {
						if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p)))
							apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
					}
				}
			}
		}
		void es_x_greater_power_scope(EFFECT_PARAMTERS) {
			auto ranked_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
			int32_t great_nations_count = int32_t(ws.s.modifiers_m.global_defines.great_nations_count);
			int32_t count = 0;

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::country_tag, 16, concurrent_allocator<nations::country_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							if(nations::is_great_power(ws, *n) && test_trigger(limit, ws, *n, this_slot, from_slot)) {
								++count;
								rlist.push_back(*n);
							}
						}
					}
				} else {
					for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							if(nations::is_great_power(ws, *n)) {
								++count;
								rlist.push_back(*n);
							}
						}
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							if(nations::is_great_power(ws, *n) && test_trigger(limit, ws, *n, this_slot, from_slot)) {
								++count;
								apply_subeffects(tval, ws, *n, this_slot, from_slot, gen);
							}
						}
					}
				} else {
					for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							if(nations::is_great_power(ws, *n)) {
								++count;
								apply_subeffects(tval, ws, *n, this_slot, from_slot, gen);
							}
						}
					}
				}
			}
		}
		void es_poor_strata_scope_nation(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, to_nation(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj){
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, to_nation(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_poor_strata_scope_state(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, to_state(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, to_state(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_poor_strata_scope_province(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_middle_strata_scope_nation(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, to_nation(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, to_nation(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_middle_strata_scope_state(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, to_state(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, to_state(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_middle_strata_scope_province(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_rich_strata_scope_nation(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, to_nation(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, to_nation(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_rich_strata_scope_state(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, to_state(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, to_state(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_rich_strata_scope_province(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich &&
						test_trigger(limit, ws, pobj, this_slot, from_slot)) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			} else {
				provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
					auto type = ws.w.population_s.pops.get<pop::type>(pobj);
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich) {

						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					}
				});
			}
		}
		void es_x_pop_scope_nation(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<population::pop_tag, 16, concurrent_allocator<population::pop_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, to_nation(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](population::pop_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_pop(ws, to_nation(primary_slot), [&rlist](population::pop_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, to_nation(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				} else {
					nations::for_each_pop(ws, to_nation(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				}
			}
		}
		void es_x_pop_scope_state(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<population::pop_tag, 16, concurrent_allocator<population::pop_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, to_state(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](population::pop_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_pop(ws, to_state(primary_slot), [&rlist](population::pop_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, to_state(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				} else {
					nations::for_each_pop(ws, to_state(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				}
			}
		}
		void es_x_pop_scope_province(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<population::pop_tag, 16, concurrent_allocator<population::pop_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](population::pop_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					provinces::for_each_pop(ws, to_prov(primary_slot), [&rlist](population::pop_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				} else {
					provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, tval, this_slot, from_slot, &gen](population::pop_tag pobj) {
						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				}
			}
		}
		void es_x_owned_scope_nation(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, to_nation(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](provinces::province_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_province(ws, to_nation(primary_slot), [&rlist](provinces::province_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, to_nation(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](provinces::province_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				} else {
					nations::for_each_province(ws, to_nation(primary_slot), [&ws, tval, this_slot, from_slot, &gen](provinces::province_tag pobj) {
						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				}
			}
		}
		void es_x_owned_scope_state(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, to_state(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](provinces::province_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_province(ws, to_state(primary_slot), [&rlist](provinces::province_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, to_state(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](provinces::province_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				} else {
					nations::for_each_province(ws, to_state(primary_slot), [&ws, tval, this_slot, from_slot, &gen](provinces::province_tag pobj) {
						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				}
			}
		}
		void es_x_core_scope(EFFECT_PARAMTERS) {
			auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(primary_slot));
			if(is_valid_index(tag)) {
				auto cores_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);

				if((tval[0] & effect_codes::is_random_scope) != 0) {
					boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

					if((tval[0] & effect_codes::scope_has_limit) != 0) {
						auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
						for(auto p : cores_range) {
							if(is_valid_index(p)) {
								if(test_trigger(limit, ws, p, this_slot, from_slot))
									rlist.push_back(p);
							}
						}
					} else {
						for(auto p : cores_range) {
							if(is_valid_index(p))
								rlist.push_back(p);
						}
					}

					if(rlist.size() != 0) {
						std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
						apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
					}
				} else {
					if((tval[0] & effect_codes::scope_has_limit) != 0) {
						auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
						for(auto p : cores_range) {
							if(is_valid_index(p)) {
								if(test_trigger(limit, ws, p, this_slot, from_slot))
									apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
							}
						}
					} else {
						for(auto p : cores_range) {
							if(is_valid_index(p)) 
								apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
						}
					}
				}
			}
		}
		void es_x_state_scope(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::state_tag, 16, concurrent_allocator<nations::state_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_state(ws, to_nation(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](nations::state_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_state(ws, to_nation(primary_slot), [&rlist](nations::state_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_state(ws, to_nation(primary_slot), [&ws, limit, tval, this_slot, from_slot, &gen](nations::state_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				} else {
					nations::for_each_state(ws, to_nation(primary_slot), [&ws, tval, this_slot, from_slot, &gen](nations::state_tag pobj) {
						apply_subeffects(tval, ws, pobj, this_slot, from_slot, gen);
					});
				}
			}
		}
		void es_random_list_scope(EFFECT_PARAMTERS) {
			const auto source_size = 1 + get_effect_payload_size(tval);
			auto chances_total = tval[2];

			std::uniform_int_distribution<int32_t> dist(0, int32_t(chances_total) - 1);
			auto chance_taken = dist(gen);

			auto sub_units_start = tval + 3; // [code] + [payload size] + [chances total] + [first sub effect chance]
			while(sub_units_start < tval + source_size) {
				chance_taken -= *sub_units_start;
				if(chance_taken < 0) {
					_execute_effect(sub_units_start + 1, ws, primary_slot, this_slot, from_slot, gen);
					return;
				}
				sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1); // each member preceeded by uint16_t
			}
		}
		void es_random_scope(EFFECT_PARAMTERS) {
			auto chance = tval[2];
			std::uniform_int_distribution<int32_t> dist(0, 99);
			if(dist(gen) < chance)
				apply_subeffects(tval, ws, primary_slot, this_slot, from_slot, gen);
		}
		void es_owner_scope_state(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(primary_slot));
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, gen);
		}
		void es_owner_scope_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(primary_slot));
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, gen);
		}
		void es_controller_scope(EFFECT_PARAMTERS) {
			auto controller = provinces::province_controller(ws, to_prov(primary_slot));
			if(controller)
				apply_subeffects(tval, ws, controller, this_slot, from_slot, gen);
		}
		void es_location_scope(EFFECT_PARAMTERS) {
			auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
			if(is_valid_index(location))
				apply_subeffects(tval, ws, location, this_slot, from_slot, gen);
		}
		void es_country_scope_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, gen);
		}
		void es_country_scope_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot));
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, gen);
		}
		void es_capital_scope(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital))
				apply_subeffects(tval, ws, capital, this_slot, from_slot, gen);
		}
		void es_this_scope_nation(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, gen);
		}
		void es_this_scope_state(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, gen);
		}
		void es_this_scope_province(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, gen);
		}
		void es_this_scope_pop(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, gen);
		}
		void es_from_scope_nation(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, gen);
		}
		void es_from_scope_state(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, gen);
		}
		void es_from_scope_province(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, gen);
		}
		void es_from_scope_pop(EFFECT_PARAMTERS) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, gen);
		}
		void es_sea_zone_scope(EFFECT_PARAMTERS) {
			auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(to_prov(primary_slot));
			if(sea_zones.first != sea_zones.second)
				apply_subeffects(tval, ws, *sea_zones.first, this_slot, from_slot, gen);
		}
		void es_cultural_union_scope(EFFECT_PARAMTERS) {
			auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
			if(!is_valid_index(prim_culture))
				return;

			auto culture_group = ws.s.culture_m.culture_container[prim_culture].group;
			if(is_valid_index(culture_group)) {
				auto union_tag = ws.s.culture_m.culture_groups[culture_group].union_tag;
				if(is_valid_index(union_tag)) {
					auto union_holder = ws.w.culture_s.tags_to_holders[union_tag];
					if(union_holder)
						apply_subeffects(tval, ws, union_holder, this_slot, from_slot, gen);
				}
			}
		}
		void es_overlord_scope(EFFECT_PARAMTERS) {
			auto overlord = ws.w.nation_s.nations.get<nation::overlord>(to_nation(primary_slot));
			if(overlord)
				apply_subeffects(tval, ws, overlord, this_slot, from_slot, gen);
		}
		void es_sphere_owner_scope(EFFECT_PARAMTERS) {
			auto sphere_leader = ws.w.nation_s.nations.get<nation::sphere_leader>(to_nation(primary_slot));
			if(sphere_leader)
				apply_subeffects(tval, ws, sphere_leader, this_slot, from_slot, gen);
		}
		void es_independence_scope(EFFECT_PARAMTERS) {
			auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
			if(is_valid_index(rtag)) {
				auto ination = ws.w.culture_s.tags_to_holders[rtag];
				if(ination)
					apply_subeffects(tval, ws, ination, this_slot, from_slot, gen);
			}
		}
		void es_flashpoint_tag_scope(EFFECT_PARAMTERS) {
			auto ctag = ws.w.nation_s.states.get<state::crisis_tag>(to_state(primary_slot));
			if(is_valid_index(ctag)) {
				auto ctag_holder = ws.w.culture_s.tags_to_holders[ctag];
				if(ctag_holder)
					apply_subeffects(tval, ws, ctag_holder, this_slot, from_slot, gen);
			}
		}
		void es_crisis_state_scope(EFFECT_PARAMTERS) {
			auto cstate = ws.w.current_crisis.state;
			if(is_valid_index(cstate))
				apply_subeffects(tval, ws, cstate, this_slot, from_slot, gen);
		}
		void es_state_scope_province(EFFECT_PARAMTERS) {
			auto state = provinces::province_state(ws, to_prov(primary_slot));
			if(state)
				apply_subeffects(tval, ws, state, this_slot, from_slot, gen);
		}
		void es_state_scope_pop(EFFECT_PARAMTERS) {
			auto pop_province = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
			if(is_valid_index(pop_province)) {
				auto prov_state = provinces::province_state(ws, pop_province);
				es_state_scope_province(tval, ws, prov_state, this_slot, from_slot, gen);
			}
		}
		void es_tag_scope(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto tag = trigger_payload(tval[3]).tag;
				auto tag_holder = ws.w.culture_s.tags_to_holders[tag];
				if(tag_holder && test_trigger(limit, ws, tag_holder, this_slot, from_slot))
					apply_subeffects(tval, ws, tag_holder, this_slot, from_slot, gen);
			} else {
				auto tag = trigger_payload(tval[2]).tag;
				auto tag_holder = ws.w.culture_s.tags_to_holders[tag];
				if(tag_holder)
					apply_subeffects(tval, ws, tag_holder, this_slot, from_slot, gen);
			}
		}
		void es_integer_scope(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::province_tag ptag(tval[3]);
				
				if(test_trigger(limit, ws, ptag, this_slot, from_slot))
					apply_subeffects(tval, ws, ptag, this_slot, from_slot, gen);
			} else {
				provinces::province_tag ptag(tval[2]);
				apply_subeffects(tval, ws, ptag, this_slot, from_slot, gen);
			}
		}
		void es_pop_type_scope_nation(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto type = trigger_payload(tval[3]).small.values.pop_type;

				nations::for_each_pop(ws, to_nation(primary_slot), [limit, type, tval, &ws, this_slot, from_slot, &gen](population::pop_tag p) {
					if(ws.w.population_s.pops.get<pop::type>(p) == type && test_trigger(limit, ws, p, this_slot, from_slot))
						apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
				});
			} else {
				auto type = trigger_payload(tval[2]).small.values.pop_type;

				nations::for_each_pop(ws, to_nation(primary_slot), [type, tval, &ws, this_slot, from_slot, &gen](population::pop_tag p) {
					if(ws.w.population_s.pops.get<pop::type>(p) == type)
						apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
				});
			}
		}
		void es_pop_type_scope_state(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto type = trigger_payload(tval[3]).small.values.pop_type;

				nations::for_each_pop(ws, to_state(primary_slot), [limit, type, tval, &ws, this_slot, from_slot, &gen](population::pop_tag p) {
					if(ws.w.population_s.pops.get<pop::type>(p) == type && test_trigger(limit, ws, p, this_slot, from_slot))
						apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
				});
			} else {
				auto type = trigger_payload(tval[2]).small.values.pop_type;

				nations::for_each_pop(ws, to_state(primary_slot), [type, tval, &ws, this_slot, from_slot, &gen](population::pop_tag p) {
					if(ws.w.population_s.pops.get<pop::type>(p) == type)
						apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
				});
			}
		}
		void es_pop_type_scope_province(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto type = trigger_payload(tval[3]).small.values.pop_type;

				provinces::for_each_pop(ws, to_prov(primary_slot), [limit, type, tval, &ws, this_slot, from_slot, &gen](population::pop_tag p) {
					if(ws.w.population_s.pops.get<pop::type>(p) == type && test_trigger(limit, ws, p, this_slot, from_slot))
						apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
				});
			} else {
				auto type = trigger_payload(tval[2]).small.values.pop_type;

				provinces::for_each_pop(ws, to_prov(primary_slot), [type, tval, &ws, this_slot, from_slot, &gen](population::pop_tag p) {
					if(ws.w.population_s.pops.get<pop::type>(p) == type)
						apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
				});
			}
		}
		void es_region_scope(EFFECT_PARAMTERS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto region = trigger_payload(tval[3]).state;
				auto provinces = ws.s.province_m.states_to_province_index.get_row(region);

				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				for(auto p : provinces) {
					if(test_trigger(limit, ws, p, this_slot, from_slot))
						apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
				}
			} else {
				auto region = trigger_payload(tval[2]).state;
				auto provinces = ws.s.province_m.states_to_province_index.get_row(region);

				for(auto p : provinces)
					apply_subeffects(tval, ws, p, this_slot, from_slot, gen);
			}
		}

		static void(*effect_scope_functions[])(EFFECT_PARAMTERS) = {
			es_generic_scope,
			es_x_neighbor_province_scope ,
			es_x_neighbor_country_scope ,
			es_x_country_scope ,
			es_x_country_scope_nation,
			es_x_empty_neighbor_province_scope,
			es_x_greater_power_scope ,
			es_poor_strata_scope_nation ,
			es_poor_strata_scope_state ,
			es_poor_strata_scope_province ,
			es_middle_strata_scope_nation,
			es_middle_strata_scope_state ,
			es_middle_strata_scope_province,
			es_rich_strata_scope_nation,
			es_rich_strata_scope_state ,
			es_rich_strata_scope_province ,
			es_x_pop_scope_nation ,
			es_x_pop_scope_state ,
			es_x_pop_scope_province,
			es_x_owned_scope_nation ,
			es_x_owned_scope_state,
			es_x_core_scope ,
			es_x_state_scope,
			es_random_list_scope,
			es_random_scope ,
			es_owner_scope_state ,
			es_owner_scope_province,
			es_controller_scope,
			es_location_scope,
			es_country_scope_pop ,
			es_country_scope_state,
			es_capital_scope ,
			es_this_scope_nation ,
			es_this_scope_state ,
			es_this_scope_province ,
			es_this_scope_pop ,
			es_from_scope_nation ,
			es_from_scope_state,
			es_from_scope_province ,
			es_from_scope_pop ,
			es_sea_zone_scope ,
			es_cultural_union_scope ,
			es_overlord_scope ,
			es_sphere_owner_scope ,
			es_independence_scope ,
			es_flashpoint_tag_scope,
			es_crisis_state_scope,
			es_state_scope_pop ,
			es_state_scope_province,
			es_tag_scope,
			es_integer_scope ,
			es_pop_type_scope_nation ,
			es_pop_type_scope_state,
			es_pop_type_scope_province ,
			es_region_scope,
		};

		void ef_none(EFFECT_PARAMTERS) {}
		void ef_capital(EFFECT_PARAMTERS) {
			provinces::province_tag new_capital(tval[2]);
			if(provinces::province_owner(ws, new_capital) == to_nation(primary_slot))
				ws.w.nation_s.nations.set<nation::current_capital>(to_nation(primary_slot), new_capital);
		}
		void ef_add_core_tag(EFFECT_PARAMTERS) {
			auto tag = trigger_payload(tval[2]).tag;
			add_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(to_prov(primary_slot)), tag);
			add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, to_prov(primary_slot));
		}
		void ef_add_core_int(EFFECT_PARAMTERS) {
			auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(primary_slot));
			provinces::province_tag prov(tval[2]);
			if(is_valid_index(tag)) {
				add_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(prov), tag);
				add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, prov);
			}
		}
		void ef_add_core_this_nation(EFFECT_PARAMTERS) {
			auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(this_slot));
			if(is_valid_index(tag)) {
				add_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(to_prov(primary_slot)), tag);
				add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, to_prov(primary_slot));
			}
		}
		void ef_add_core_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_add_core_this_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_add_core_this_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(this_slot));
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_add_core_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_add_core_from_nation(EFFECT_PARAMTERS) {
			ef_add_core_this_nation(tval, ws, primary_slot, from_slot, const_parameter(), gen);
		}
		void ef_add_core_reb(EFFECT_PARAMTERS) {
			auto tag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
			if(is_valid_index(tag)) {
				add_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(to_prov(primary_slot)), tag);
				add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, to_prov(primary_slot));
			}
		}
		void ef_remove_core_tag(EFFECT_PARAMTERS) {
			auto tag = trigger_payload(tval[2]).tag;
			remove_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(to_prov(primary_slot)), tag);
			remove_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, to_prov(primary_slot));
		}
		void ef_remove_core_int(EFFECT_PARAMTERS) {
			auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(primary_slot));
			provinces::province_tag prov(tval[2]);
			if(is_valid_index(tag)) {
				remove_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(prov), tag);
				remove_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, prov);
			}
		}
		void ef_remove_core_this_nation(EFFECT_PARAMTERS) {
			auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(this_slot));
			if(is_valid_index(tag)) {
				remove_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(to_prov(primary_slot)), tag);
				remove_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, to_prov(primary_slot));
			}
		}
		void ef_remove_core_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				ef_remove_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_remove_core_this_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
			if(owner)
				ef_remove_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_remove_core_this_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(this_slot));
			if(owner)
				ef_remove_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_remove_core_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				ef_remove_core_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_remove_core_from_nation(EFFECT_PARAMTERS) {
			ef_remove_core_this_nation(tval, ws, primary_slot, from_slot, const_parameter(), gen);
		}
		void ef_remove_core_reb(EFFECT_PARAMTERS) {
			auto tag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
			if(is_valid_index(tag)) {
				remove_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(to_prov(primary_slot)), tag);
				remove_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, to_prov(primary_slot));
			}
		}
		void ef_change_region_name_state(EFFECT_PARAMTERS) {
			ws.w.nation_s.states.set<state::name>(to_state(primary_slot), trigger_payload(tval[2]).text);
		}
		void ef_change_region_name_province(EFFECT_PARAMTERS) {
			auto si = provinces::province_state(ws, to_prov(primary_slot));
			if(si)
				ws.w.nation_s.states.set<state::name>(si, trigger_payload(tval[2]).text);
		}
		void ef_trade_goods(EFFECT_PARAMTERS) {
			ws.w.province_s.province_state_container.set<province_state::rgo_production>(to_prov(primary_slot), trigger_payload(tval[2]).small.values.good);
			economy::match_rgo_worker_type(ws, to_prov(primary_slot));
		}
		void ef_add_accepted_culture(EFFECT_PARAMTERS) {
			nations::add_accepted_culture(ws, to_nation(primary_slot), trigger_payload(tval[2]).culture);
		}
		void ef_add_accepted_culture_union(EFFECT_PARAMTERS) {
			auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
			if(is_valid_index(prim_culture)) {
				auto cg_t = ws.s.culture_m.culture_container[prim_culture].group;
				nations::add_accepted_culture_group(ws, to_nation(primary_slot), cg_t);
			}
		}
		void ef_primary_culture(EFFECT_PARAMTERS) {
			nations::change_primary_culture(ws, to_nation(primary_slot), trigger_payload(tval[2]).culture);
		}
		void ef_primary_culture_this_nation(EFFECT_PARAMTERS) {
			nations::change_primary_culture(ws, to_nation(primary_slot), ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot)));
		}
		void ef_primary_culture_this_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
			if(owner)
				nations::change_primary_culture(ws, to_nation(primary_slot), ws.w.nation_s.nations.get<nation::primary_culture>(owner));
		}
		void ef_primary_culture_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				nations::change_primary_culture(ws, to_nation(primary_slot), ws.w.nation_s.nations.get<nation::primary_culture>(owner));
		}
		void ef_primary_culture_this_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(this_slot));
			if(owner)
				nations::change_primary_culture(ws, to_nation(primary_slot), ws.w.nation_s.nations.get<nation::primary_culture>(owner));
		}
		void ef_primary_culture_from_nation(EFFECT_PARAMTERS) {
			nations::change_primary_culture(ws, to_nation(primary_slot), ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(from_slot)));
		}
		void ef_remove_accepted_culture(EFFECT_PARAMTERS) {
			nations::remove_accepted_culture(ws, to_nation(primary_slot), trigger_payload(tval[2]).culture);
		}
		void ef_life_rating(EFFECT_PARAMTERS) {
			ws.w.province_s.province_state_container.get<province_state::base_life_rating>(to_prov(primary_slot)) += trigger_payload(tval[2]).signed_value;
		}
		void ef_religion(EFFECT_PARAMTERS) {
			ws.w.nation_s.nations.set<nation::national_religion>(to_nation(primary_slot), trigger_payload(tval[2]).small.values.religion);
		}
		void ef_is_slave_state_yes(EFFECT_PARAMTERS) {
			nations::make_slave_state(ws, to_state(primary_slot));
		}
		void ef_is_slave_pop_yes(EFFECT_PARAMTERS) {
			population::change_pop_type(ws, to_pop(primary_slot), ws.s.population_m.slave);
		}
		void ef_research_points(EFFECT_PARAMTERS) {
			ws.w.nation_s.nations.get<nation::research_points>(to_nation(primary_slot)) += trigger_payload(tval[2]).signed_value;
		}
		void ef_tech_school(EFFECT_PARAMTERS) {
			ws.w.nation_s.nations.set<nation::tech_school>(to_nation(primary_slot), trigger_payload(tval[2]).nat_mod);
		}
		void ef_government(EFFECT_PARAMTERS) {
			governments::silent_set_government(ws, to_nation(primary_slot), trigger_payload(tval[2]).small.values.government);
		}
		void ef_government_reb(EFFECT_PARAMTERS) {
			auto rvariant = population::from_rebel_faction_tag(to_rebel(from_slot));
			if(std::holds_alternative<population::rebel_type_tag>(rvariant)) {
				auto rtype = std::get<population::rebel_type_tag>(rvariant);
				if(rtype) {
					governments::silent_set_government(ws, to_nation(primary_slot),
						ws.s.population_m.rebel_change_government_to.get(rtype, ws.w.nation_s.nations.get<nation::current_government>(to_nation(primary_slot))));
				}
			}
			
		}
		void ef_treasury(EFFECT_PARAMTERS) {
			ws.w.nation_s.nations.get<nation::treasury>(to_nation(primary_slot)) += read_float_from_payload(tval + 2);
		}
		void ef_war_exhaustion(EFFECT_PARAMTERS) {
			auto& war_x = ws.w.nation_s.nations.get<nation::war_exhaustion>(to_nation(primary_slot));
			war_x = std::clamp(war_x + read_float_from_payload(tval + 2), 0.0f, 1.0f);
		}
		void ef_prestige(EFFECT_PARAMTERS) {
			auto amount = read_float_from_payload(tval + 2);
			auto& basep = ws.w.nation_s.nations.get<nation::base_prestige>(to_nation(primary_slot));
			if(amount >= 0.0f) {
				basep = std::max(
					0.0f,
					basep + amount * (ws.w.nation_s.tech_attributes.get<technologies::tech_offset::prestige>(to_nation(primary_slot)) + 1.0f));
			} else {
				basep = std::max(
					0.0f,
					basep + amount);
			}
		}
		void ef_change_tag(EFFECT_PARAMTERS) {
			auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(primary_slot));
			if(is_valid_index(tag))
				cultures::replace_cores(ws, tag, trigger_payload(tval[2]).tag);
			nations::change_tag(ws, to_nation(primary_slot), trigger_payload(tval[2]).tag);
		}
		void ef_change_tag_culture(EFFECT_PARAMTERS) {
			auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
			if(is_valid_index(prim_culture)) {
				auto cg_t = ws.s.culture_m.culture_container[prim_culture].group;
				auto u = ws.s.culture_m.culture_groups[cg_t].union_tag;
				if(is_valid_index(u)) {
					auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(primary_slot));
					if(is_valid_index(tag))
						cultures::replace_cores(ws, tag, u);
					nations::change_tag(ws, to_nation(primary_slot), u);
				}
			}
		}
		void ef_change_tag_no_core_switch(EFFECT_PARAMTERS) {
			if(ws.w.local_player_nation == to_nation(primary_slot)) {
				if(auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag]; holder) {
					ws.w.nation_s.nations.set<nation::is_not_ai_controlled>(ws.w.local_player_nation, false);
					ws.w.nation_s.nations.set<nation::is_not_ai_controlled>(holder, true);
					ws.w.local_player_nation = holder;
				}
			}
		}
		void ef_change_tag_no_core_switch_culture(EFFECT_PARAMTERS) {
			if(ws.w.local_player_nation == to_nation(primary_slot)) {
				auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
				if(is_valid_index(prim_culture)) {
					auto cg_t = ws.s.culture_m.culture_container[prim_culture].group;
					auto u = ws.s.culture_m.culture_groups[cg_t].union_tag;
					if(is_valid_index(u)) {
						if(auto holder = ws.w.culture_s.tags_to_holders[u]; holder) {
							ws.w.nation_s.nations.set<nation::is_not_ai_controlled>(ws.w.local_player_nation, false);
							ws.w.nation_s.nations.set<nation::is_not_ai_controlled>(holder, true);
							ws.w.local_player_nation = holder;
						}
					}
				}
			}
		}
		void ef_set_country_flag(EFFECT_PARAMTERS) {
			add_item(ws.w.variable_s.national_flags_arrays, ws.w.nation_s.nations.get<nation::national_flags>(to_nation(primary_slot)), trigger_payload(tval[2]).nat_flag);
		}
		void ef_clr_country_flag(EFFECT_PARAMTERS) {
			remove_item(ws.w.variable_s.national_flags_arrays, ws.w.nation_s.nations.get<nation::national_flags>(to_nation(primary_slot)), trigger_payload(tval[2]).nat_flag);
		}
		void ef_military_access(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_military_access_this_nation(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_military_access_this_province(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_military_access_from_nation(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_military_access_from_province(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_badboy(EFFECT_PARAMTERS) {
			auto& inf = ws.w.nation_s.nations.get<nation::infamy>(to_nation(primary_slot));
			inf = std::max(0.0f, inf + read_float_from_payload(tval + 2));
		}
		void ef_secede_province(EFFECT_PARAMTERS) {
			auto holder = nations::make_nation_for_tag(ws, trigger_payload(tval[2]).tag);
			provinces::silent_set_province_owner(ws, holder, to_prov(primary_slot));
			provinces::silent_set_province_controller(ws, holder, to_prov(primary_slot));
			provinces::silent_on_conquer_province(ws, to_prov(primary_slot));
		}
		void ef_secede_province_this_nation(EFFECT_PARAMTERS) {
			provinces::silent_set_province_owner(ws, to_nation(this_slot), to_prov(primary_slot));
			provinces::silent_set_province_controller(ws, to_nation(this_slot), to_prov(primary_slot));
			provinces::silent_on_conquer_province(ws, to_prov(primary_slot));
		}
		void ef_secede_province_this_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
			if(owner) {
				provinces::silent_set_province_owner(ws, owner, to_prov(primary_slot));
				provinces::silent_set_province_controller(ws, owner, to_prov(primary_slot));
				provinces::silent_on_conquer_province(ws, to_prov(primary_slot));
			}
		}
		void ef_secede_province_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner) {
				provinces::silent_set_province_owner(ws, owner, to_prov(primary_slot));
				provinces::silent_set_province_controller(ws, owner, to_prov(primary_slot));
				provinces::silent_on_conquer_province(ws, to_prov(primary_slot));
			}
		}
		void ef_secede_province_this_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(this_slot));
			if(owner) {
				provinces::silent_set_province_owner(ws, owner, to_prov(primary_slot));
				provinces::silent_set_province_controller(ws, owner, to_prov(primary_slot));
				provinces::silent_on_conquer_province(ws, to_prov(primary_slot));
			}
		}
		void ef_secede_province_from_nation(EFFECT_PARAMTERS) {
			ef_secede_province_this_nation(tval, ws, primary_slot, from_slot, const_parameter(), gen);
		}
		void ef_secede_province_from_province(EFFECT_PARAMTERS) {
			ef_secede_province_this_province(tval, ws, primary_slot, from_slot, const_parameter(), gen);
		}
		void ef_secede_province_reb(EFFECT_PARAMTERS) {
			if(auto itag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot)); is_valid_index(itag)) {
				auto holder = nations::make_nation_for_tag(ws, itag);
				provinces::silent_set_province_owner(ws, holder, to_prov(primary_slot));
				provinces::silent_set_province_controller(ws, holder, to_prov(primary_slot));
				provinces::silent_on_conquer_province(ws, to_prov(primary_slot));
			}
		}
		void ef_inherit(EFFECT_PARAMTERS) {
			auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(holder && to_nation(primary_slot) != holder) 
				nations::annex_nation(ws, to_nation(primary_slot), holder);
		}
		void ef_inherit_this_nation(EFFECT_PARAMTERS) {
			if(to_nation(primary_slot) != to_nation(this_slot)) 
				nations::annex_nation(ws, to_nation(primary_slot), to_nation(this_slot));
		}
		void ef_inherit_this_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, to_nation(primary_slot), owner);
		}
		void ef_inherit_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, to_nation(primary_slot), owner);
		}
		void ef_inherit_this_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(this_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, to_nation(primary_slot), owner);
		}
		void ef_inherit_from_nation(EFFECT_PARAMTERS) {
			if(to_nation(primary_slot) != to_nation(from_slot))
				nations::annex_nation(ws, to_nation(primary_slot), to_nation(from_slot));
		}
		void ef_inherit_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, to_nation(primary_slot), owner);
		}
		void ef_annex_to(EFFECT_PARAMTERS) {
			auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(holder && to_nation(primary_slot) != holder)
				nations::annex_nation(ws, holder, to_nation(primary_slot));
		}
		void ef_annex_to_this_nation(EFFECT_PARAMTERS) {
			if(to_nation(primary_slot) != to_nation(this_slot))
				nations::annex_nation(ws, to_nation(this_slot), to_nation(primary_slot));
		}
		void ef_annex_to_this_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, owner, to_nation(primary_slot));
		}
		void ef_annex_to_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, owner, to_nation(primary_slot));
		}
		void ef_annex_to_this_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(this_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, owner, to_nation(primary_slot));
		}
		void ef_annex_to_from_nation(EFFECT_PARAMTERS) {
			if(to_nation(primary_slot) != to_nation(from_slot))
				nations::annex_nation(ws, to_nation(from_slot), to_nation(primary_slot));
		}
		void ef_annex_to_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner && to_nation(primary_slot) != owner)
				nations::annex_nation(ws, owner, to_nation(primary_slot));
		}
		void ef_release(EFFECT_PARAMTERS) {
			nations::liberate_uncored_cores(ws, to_nation(primary_slot), trigger_payload(tval[2]).tag);
		}
		void ef_release_this_nation(EFFECT_PARAMTERS) {
			if(auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(this_slot)); is_valid_index(tag))
				nations::liberate_uncored_cores(ws, to_nation(primary_slot), tag);
		}
		void ef_release_this_state(EFFECT_PARAMTERS) {
			auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot));
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_release_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_release_this_pop(EFFECT_PARAMTERS) {
			auto owner = population::get_pop_owner(ws, to_pop(this_slot));
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_release_from_nation(EFFECT_PARAMTERS) {
			if(auto tag = ws.w.nation_s.nations.get<nation::tag>(to_nation(from_slot)); is_valid_index(tag))
				nations::liberate_uncored_cores(ws, to_nation(primary_slot), tag);
		}
		void ef_release_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_change_controller(EFFECT_PARAMTERS) {
			auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(holder)
				provinces::silent_set_province_controller(ws, holder, to_prov(primary_slot));
		}
		void ef_change_controller_this_nation(EFFECT_PARAMTERS) {
			provinces::silent_set_province_controller(ws, to_nation(this_slot), to_prov(primary_slot));
		}
		void ef_change_controller_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				provinces::silent_set_province_controller(ws, owner, to_prov(primary_slot));
		}
		void ef_change_controller_from_nation(EFFECT_PARAMTERS) {
			provinces::silent_set_province_controller(ws, to_nation(from_slot), to_prov(primary_slot));
		}
		void ef_change_controller_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				provinces::silent_set_province_controller(ws, owner, to_prov(primary_slot));
		}
		void ef_infrastructure(EFFECT_PARAMTERS) {
			auto& rr_level = ws.w.province_s.province_state_container.get<province_state::railroad_level>(to_prov(primary_slot));
			rr_level =
				uint8_t(std::clamp(
				int32_t(rr_level) + int32_t(trigger_payload(tval[2]).signed_value), 0, 255));
		}
		void ef_money(EFFECT_PARAMTERS) {
			auto& money = ws.w.population_s.pops.get<pop::money>(to_pop(primary_slot));
			money += read_float_from_payload(tval + 2);
		}
		void ef_leadership(EFFECT_PARAMTERS) {
			ws.w.nation_s.nations.get<nation::leadership_points>(to_nation(primary_slot)) += trigger_payload(tval[2]).signed_value;
		}
		void ef_create_vassal(EFFECT_PARAMTERS) {
			auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(holder && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(holder)))
				nations::make_vassal(ws, to_nation(primary_slot), holder);
		}
		void ef_create_vassal_this_nation(EFFECT_PARAMTERS) {
			if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(to_nation(this_slot))))
				nations::make_vassal(ws, to_nation(primary_slot), to_nation(this_slot));
		}
		void ef_create_vassal_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				ef_create_vassal_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_create_vassal_from_nation(EFFECT_PARAMTERS) {
			if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(to_nation(from_slot))))
				nations::make_vassal(ws, to_nation(primary_slot), to_nation(from_slot));
		}
		void ef_create_vassal_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				ef_create_vassal_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_end_military_access(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_end_military_access_this_nation(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_end_military_access_this_province(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_end_military_access_from_nation(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_end_military_access_from_province(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_leave_alliance(EFFECT_PARAMTERS) {
			auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(holder)
				nations::end_alliance(ws, to_nation(primary_slot), holder);
		}
		void ef_leave_alliance_this_nation(EFFECT_PARAMTERS) {
			nations::end_alliance(ws, to_nation(primary_slot), to_nation(this_slot));
		}
		void ef_leave_alliance_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				nations::end_alliance(ws, to_nation(primary_slot), owner);
		}
		void ef_leave_alliance_from_nation(EFFECT_PARAMTERS) {
			nations::end_alliance(ws, to_nation(primary_slot), to_nation(from_slot));
		}
		void ef_leave_alliance_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				nations::end_alliance(ws, to_nation(primary_slot), owner);
		}
		void ef_end_war(EFFECT_PARAMTERS) {
			auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(holder) {
				auto war_between = military::get_war_between(ws, to_nation(primary_slot), holder);
				if(war_between) {
					military::destroy_war(ws, war_between);
				}
			}
		}
		void ef_end_war_this_nation(EFFECT_PARAMTERS) {
			auto war_between = military::get_war_between(ws, to_nation(primary_slot), to_nation(this_slot));
			if(war_between) {
				military::destroy_war(ws, war_between);
			}
		}
		void ef_end_war_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				ef_end_war_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_end_war_from_nation(EFFECT_PARAMTERS) {
			auto war_between = military::get_war_between(ws, to_nation(primary_slot), to_nation(from_slot));
			if(war_between) {
				military::destroy_war(ws, war_between);
			}
		}
		void ef_end_war_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				ef_end_war_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_enable_ideology(EFFECT_PARAMTERS) {
			ws.w.ideology_s.ideology_enabled[trigger_payload(tval[2]).small.values.ideology] = 1ui8;
		}
		void ef_ruling_party_ideology(EFFECT_PARAMTERS) {
			auto new_party = ws.w.nation_s.active_parties.get(to_nation(primary_slot), trigger_payload(tval[2]).small.values.ideology);
			if(is_valid_index(new_party) && new_party != ws.w.nation_s.nations.get<nation::ruling_party>(to_nation(primary_slot)))
				governments::silent_set_ruling_party(ws, to_nation(primary_slot), new_party);
		}
		void ef_plurality(EFFECT_PARAMTERS) {
			auto& plur = ws.w.nation_s.nations.get<nation::plurality>(to_nation(primary_slot));
			plur = std::clamp(plur + read_float_from_payload(tval + 2), 0.0f, 1.0f);
		}
		void ef_remove_province_modifier(EFFECT_PARAMTERS) {
			auto pmod = trigger_payload(tval[2]).prov_mod;
			modifiers::remove_static_modifier_from_province(ws, to_prov(primary_slot), pmod);
			modifiers::remove_all_timed_modifiers_from_province(ws, to_prov(primary_slot), pmod);
		}
		void ef_remove_country_modifier(EFFECT_PARAMTERS) {
			auto nmod = trigger_payload(tval[2]).nat_mod;
			modifiers::remove_all_static_modifiers_from_nation(ws, to_nation(primary_slot), nmod);
			modifiers::remove_all_timed_modifiers_from_nation(ws, to_nation(primary_slot), nmod);
		}
		void ef_create_alliance(EFFECT_PARAMTERS) {
			auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(holder)
				nations::make_alliance(ws, to_nation(primary_slot), holder);
		}
		void ef_create_alliance_this_nation(EFFECT_PARAMTERS) {
			nations::make_alliance(ws, to_nation(primary_slot), to_nation(this_slot));
		}
		void ef_create_alliance_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				nations::make_alliance(ws, to_nation(primary_slot), owner);
		}
		void ef_create_alliance_from_nation(EFFECT_PARAMTERS) {
			nations::make_alliance(ws, to_nation(primary_slot), to_nation(from_slot));
		}
		void ef_create_alliance_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				nations::make_alliance(ws, to_nation(primary_slot), owner);
		}
		void ef_release_vassal(EFFECT_PARAMTERS) {
			if(auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag]; bool(holder) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(holder))) {
				nations::free_vassal(ws, holder);
			} else {
				auto new_nation = nations::liberate_uncored_cores(ws, to_nation(primary_slot), trigger_payload(tval[2]).tag);
				if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(new_nation)))
					nations::make_vassal(ws, to_nation(primary_slot), new_nation);
			}
		}
		void ef_release_vassal_this_nation(EFFECT_PARAMTERS) {
			if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(to_nation(this_slot)))) {
				nations::free_vassal(ws, to_nation(this_slot));
			} else {
				auto new_nation = nations::liberate_uncored_cores(ws, to_nation(primary_slot), ws.w.nation_s.nations.get<nation::tag>(to_nation(this_slot)));
				if(is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(new_nation)))
					nations::make_vassal(ws, to_nation(primary_slot), new_nation);
			}
		}
		void ef_release_vassal_this_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(this_slot));
			if(owner)
				ef_release_vassal_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_release_vassal_from_nation(EFFECT_PARAMTERS) {
			ef_release_vassal_this_nation(tval, ws, primary_slot, from_slot, const_parameter(), gen);
		}
		void ef_release_vassal_from_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(from_slot));
			if(owner)
				ef_release_vassal_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_release_vassal_reb(EFFECT_PARAMTERS) {
			auto itag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
			nations::liberate_all_cores(ws, to_nation(primary_slot), itag);
		}
		void ef_release_vassal_random(EFFECT_PARAMTERS) {
			//unused
		}
		void ef_change_province_name(EFFECT_PARAMTERS) {
			ws.w.province_s.province_state_container.set<province_state::name>(to_prov(primary_slot), trigger_payload(tval[2]).text);
		}
		void ef_enable_canal(EFFECT_PARAMTERS) {
			provinces::enable_canal(ws, tval[2] - 1);
		}
		void ef_set_global_flag(EFFECT_PARAMTERS) {
			ws.w.variable_s.global_variables[trigger_payload(tval[2]).global_var] = 1.0f;
		}
		void ef_clr_global_flag(EFFECT_PARAMTERS) {
			ws.w.variable_s.global_variables[trigger_payload(tval[2]).global_var] = 0.0f;
		}
		void ef_nationalvalue_province(EFFECT_PARAMTERS) {
			auto owner = provinces::province_owner(ws, to_prov(primary_slot));
			if(owner)
				ws.w.nation_s.nations.set<nation::national_value>(owner, trigger_payload(tval[2]).nat_mod);
		}
		void ef_nationalvalue_nation(EFFECT_PARAMTERS) {
			ws.w.nation_s.nations.set<nation::national_value>(to_nation(primary_slot),trigger_payload(tval[2]).nat_mod);
		}
		void ef_civilized_yes(EFFECT_PARAMTERS) {
			nations::civilize_nation(ws, to_nation(primary_slot));
		}
		void ef_civilized_no(EFFECT_PARAMTERS) {
			nations::uncivilize_nation(ws, to_nation(primary_slot));
		}
		void ef_is_slave_state_no(EFFECT_PARAMTERS) {
			nations::unmake_slave_state(ws, to_state(primary_slot));
		}
		void ef_is_slave_pop_no(EFFECT_PARAMTERS) {
			if(ws.w.population_s.pops.get<pop::type>(to_pop(primary_slot)) == ws.s.population_m.slave)
				population::free_slave(ws, to_pop(primary_slot));
		}
		void ef_election(EFFECT_PARAMTERS) {
			if(ws.s.governments_m.governments_container[ws.w.nation_s.nations.get<nation::current_government>(to_nation(primary_slot))].election)
				governments::start_election(ws, to_nation(primary_slot));
		}
		void ef_social_reform(EFFECT_PARAMTERS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, to_nation(primary_slot));
		}
		void ef_political_reform(EFFECT_PARAMTERS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, to_nation(primary_slot));
		}
		void ef_add_tax_relative_income(EFFECT_PARAMTERS) {
			ws.w.nation_s.nations.get<nation::treasury>(to_nation(primary_slot)) += 
				ws.w.nation_s.nations.get<nation::tax_base>(to_nation(primary_slot)) * read_float_from_payload(tval + 2);
		}
		void ef_neutrality(EFFECT_PARAMTERS) {
			auto allies_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(to_nation(primary_slot)));
			boost::container::small_vector<nations::country_tag, 16> allies_copy(allies_range.first, allies_range.second);

			for(auto a : allies_copy)
				nations::end_alliance(ws, to_nation(primary_slot), a);

			auto vassals_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(to_nation(primary_slot)));
			boost::container::small_vector<nations::country_tag, 16> vassals_copy(vassals_range.first, vassals_range.second);

			for(auto a : vassals_copy)
				nations::free_vassal(ws, a);
		}
		void ef_reduce_pop(EFFECT_PARAMTERS) {
			population::change_pop_size(
				ws, to_pop(primary_slot), 
				std::max(1, int32_t(ws.w.population_s.pops.get<pop::size>(to_pop(primary_slot)) * read_float_from_payload(tval + 2))));
		}
		void ef_move_pop(EFFECT_PARAMTERS) {
			population::change_pop_location(ws, to_pop(primary_slot), provinces::province_tag(tval[2]));
		}
		void ef_pop_type(EFFECT_PARAMTERS) {
			population::change_pop_type(ws, to_pop(primary_slot), trigger_payload(tval[2]).small.values.pop_type);
		}
		void ef_years_of_research(EFFECT_PARAMTERS) {
			auto& rp = ws.w.nation_s.nations.get<nation::research_points>(to_nation(primary_slot));
			rp = 
				int16_t(float(rp) +
					technologies::daily_research_points(ws, to_nation(primary_slot)) * 365.0f * read_float_from_payload(tval + 2));
		}
		void ef_prestige_factor_positive(EFFECT_PARAMTERS) {
			auto& bp = ws.w.nation_s.nations.get<nation::base_prestige>(to_nation(primary_slot));
			auto perm_prestige = ws.w.nation_s.tech_attributes.get<technologies::tech_offset::permanent_prestige>(to_nation(primary_slot));
			auto prestige = ws.w.nation_s.tech_attributes.get<technologies::tech_offset::prestige>(to_nation(primary_slot));
			bp +=
				(bp + perm_prestige) *
				(prestige + 1.0f) *
				read_float_from_payload(tval + 2);
		}
		void ef_prestige_factor_negative(EFFECT_PARAMTERS) {
			auto& bp = ws.w.nation_s.nations.get<nation::base_prestige>(to_nation(primary_slot));
			auto perm_prestige = ws.w.nation_s.tech_attributes.get<technologies::tech_offset::permanent_prestige>(to_nation(primary_slot));
			bp = std::max(
				0.0f,
				bp +
				(bp + perm_prestige) *
				read_float_from_payload(tval + 2));
		}
		void ef_military_reform(EFFECT_PARAMTERS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, to_nation(primary_slot));
		}
		void ef_economic_reform(EFFECT_PARAMTERS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, to_nation(primary_slot));
		}
		void ef_remove_random_military_reforms(EFFECT_PARAMTERS) {
			boost::container::small_vector<issues::issue_tag, 16> active_reforms;
			auto nation_id = to_nation(primary_slot);
			for(auto issue : ws.s.issues_m.military_issues) {
				if(ws.w.nation_s.active_issue_options.get(nation_id, issue) != ws.s.issues_m.issues_container[issue].options[0])
					active_reforms.push_back(issue);
			}
			for(int32_t i = tval[2] - 1; active_reforms.size() != 0 && i >= 0; --i) {
				std::uniform_int_distribution<int32_t> dist(0, int32_t(active_reforms.size()) - 1);
				auto chance_taken = uint32_t(dist(gen));
				issues::change_issue_option(ws, ws.s.issues_m.issues_container[active_reforms[chance_taken]].options[0], to_nation(primary_slot));
			}
		}
		void ef_remove_random_economic_reforms(EFFECT_PARAMTERS) {
			boost::container::small_vector<issues::issue_tag, 16> active_reforms;
			auto nation_id = to_nation(primary_slot);
			for(auto issue : ws.s.issues_m.economic_issues) {
				if(ws.w.nation_s.active_issue_options.get(nation_id, issue) != ws.s.issues_m.issues_container[issue].options[0])
					active_reforms.push_back(issue);
			}
			for(int32_t i = tval[2] - 1; active_reforms.size() != 0 && i >= 0; --i) {
				std::uniform_int_distribution<int32_t> dist(0, int32_t(active_reforms.size()) - 1);
				auto chance_taken = uint32_t(dist(gen));
				issues::change_issue_option(ws, ws.s.issues_m.issues_container[active_reforms[chance_taken]].options[0], to_nation(primary_slot));
			}
		}
		void ef_add_crime(EFFECT_PARAMTERS) {
			ws.w.province_s.province_state_container.set<province_state::crime>(to_prov(primary_slot), trigger_payload(tval[2]).prov_mod);
		}
		void ef_add_crime_none(EFFECT_PARAMTERS) {
			ws.w.province_s.province_state_container.set<province_state::crime>(to_prov(primary_slot), modifiers::provincial_modifier_tag());
		}
		void ef_nationalize(EFFECT_PARAMTERS) {
			nations::perform_nationalization(ws, to_nation(primary_slot));
		}
		void ef_build_factory_in_capital_state(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			auto factory_type = &(ws.s.economy_m.factory_types[trigger_payload(tval[2]).small.values.factory]);
			if(is_valid_index(capital)) {
				if(auto si = provinces::province_state(ws, capital); si) {
					int32_t free_index = -1;
					auto& factories = ws.w.nation_s.states.get<state::factories>(si);
					for(int32_t i = state::factories_count - 1; i >= 0; --i) {
						if(factories[i].type == factory_type)
							return;
						else if(factories[i].type == nullptr)
							free_index = i;
					}
					if(free_index != -1) {
						factories[free_index].type = factory_type;
						factories[free_index].level = 1ui16;
					}
				}
			}
		}
		void ef_activate_technology(EFFECT_PARAMTERS) {
			technologies::apply_single_technology(ws, to_nation(primary_slot), trigger_payload(tval[2]).tech);
		}
		void ef_great_wars_enabled_yes(EFFECT_PARAMTERS) {
			ws.w.great_wars_enabled = true;
		}
		void ef_great_wars_enabled_no(EFFECT_PARAMTERS) {
			ws.w.great_wars_enabled = false;
		}
		void ef_world_wars_enabled_yes(EFFECT_PARAMTERS) {
			ws.w.world_wars_enabled = true;
		}
		void ef_world_wars_enabled_no(EFFECT_PARAMTERS) {
			ws.w.world_wars_enabled = false;
		}
		void ef_assimilate_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner) {
				provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, owner_culture = ws.w.nation_s.nations.get<nation::primary_culture>(owner)](population::pop_tag p){
					ws.w.population_s.pops.set<pop::culture>(p, owner_culture);
				});
			}
		}
		void ef_assimilate_pop(EFFECT_PARAMTERS) {
			if(auto owner = population::get_pop_owner(ws, to_pop(primary_slot)); owner) {
				ws.w.population_s.pops.set<pop::culture>(to_pop(primary_slot), ws.w.nation_s.nations.get<nation::primary_culture>(owner));
			}
		}
		void ef_literacy(EFFECT_PARAMTERS) {
			population::set_literacy_direct(ws, to_pop(primary_slot), population::get_literacy_direct(ws, to_pop(primary_slot)) + read_float_from_payload(tval + 2));
		}
		void ef_add_crisis_interest(EFFECT_PARAMTERS) {
			if(ws.w.current_crisis.type != current_state::crisis_type::none)
				add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, to_nation(primary_slot));
		}
		void ef_flashpoint_tension(EFFECT_PARAMTERS) {
			auto& current_tension = ws.w.nation_s.states.get<state::current_tension>(to_state(primary_slot));
			current_tension = std::clamp(current_tension + read_float_from_payload(tval + 2), 0.0f, 100.0f);
		}
		void ef_add_crisis_temperature(EFFECT_PARAMTERS) {
			ws.w.current_crisis.temperature = std::clamp(ws.w.current_crisis.temperature + read_float_from_payload(tval + 2), 0.0f, 100.0f);
		}
		void ef_consciousness(EFFECT_PARAMTERS) {
			population::set_consciousness_direct(ws, to_pop(primary_slot), population::get_consciousness_direct(ws, to_pop(primary_slot)) + read_float_from_payload(tval + 2));
		}
		void ef_militancy(EFFECT_PARAMTERS) {
			population::set_militancy_direct(ws, to_pop(primary_slot), population::get_militancy_direct(ws, to_pop(primary_slot)) + read_float_from_payload(tval + 2));
		}
		void ef_rgo_size(EFFECT_PARAMTERS) {
			auto& rgo_size = ws.w.province_s.province_state_container.get<province_state::rgo_size>(to_prov(primary_slot));
			rgo_size = uint8_t(std::clamp(int32_t(rgo_size + trigger_payload(tval[2]).signed_value), 0, 255));
		}
		void ef_fort(EFFECT_PARAMTERS) {
			auto& fort_level = ws.w.province_s.province_state_container.get<province_state::fort_level>(to_prov(primary_slot));
			fort_level = uint8_t(std::clamp(int32_t(fort_level) + int32_t(trigger_payload(tval[2]).signed_value), 0, 255));
		}
		void ef_naval_base(EFFECT_PARAMTERS) {
			auto& naval_base_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(to_prov(primary_slot));
			naval_base_level = uint8_t(std::clamp(int32_t(naval_base_level) + int32_t(trigger_payload(tval[2]).signed_value), 0, 255));
		}
		void ef_trigger_revolt_nation(EFFECT_PARAMTERS) {
			//auto rf_range = get_range(ws.w.population_s.rebel_faction_arrays, ws.w.nation_s.nations.get<nation::active_rebel_factions>(to_nation(primary_slot)));

			auto type = trigger_payload(tval[2]).small.values.rebel;
			
			/*auto culture = trigger_payload(tval[3]).culture;
			auto religion = trigger_payload(tval[4]).small.values.religion;
			auto ideology = trigger_payload(tval[5]).small.values.ideology;

			for(auto rf : rf_range) {
				auto rf_obj_type = ws.w.population_s.rebel_factions.get<rebel_faction::type>(rf);
				auto rf_obj_culture = ws.w.population_s.rebel_factions.get<rebel_faction::culture>(rf);
				auto rf_obj_religion = ws.w.population_s.rebel_factions.get<rebel_faction::religion>(rf);
				auto rf_obj_ideology = ws.w.population_s.rebel_factions.get<rebel_faction::ideology>(rf);
				if((!is_valid_index(type) || rf_obj_type == type) &&
					(!is_valid_index(culture) || rf_obj_culture == culture) &&
					(!is_valid_index(religion) || rf_obj_religion == religion) &&
					(!is_valid_index(ideology) || rf_obj_ideology == ideology)) {
					population::trigger_rising(ws, rf, to_nation(primary_slot));
				}
			}*/
			if(type)
				population::trigger_rising(ws, type, to_nation(primary_slot));
		}
		void ef_trigger_revolt_state(EFFECT_PARAMTERS) {
			if(auto owner = ws.w.nation_s.states.get<state::owner>(to_state(primary_slot)); owner)
				ef_trigger_revolt_nation(tval, ws, owner, const_parameter(), const_parameter(), gen);
		}
		void ef_trigger_revolt_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_trigger_revolt_nation(tval, ws, owner, const_parameter(), const_parameter(), gen);
		}
		void ef_diplomatic_influence(EFFECT_PARAMTERS) {
			if(auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag]; holder)
				nations::adjust_influence(ws, to_nation(primary_slot), holder, trigger_payload(tval[3]).signed_value);
		}
		void ef_diplomatic_influence_this_nation(EFFECT_PARAMTERS) {
			nations::adjust_influence(ws, to_nation(primary_slot), to_nation(this_slot), trigger_payload(tval[2]).signed_value);
		}
		void ef_diplomatic_influence_this_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner)
				nations::adjust_influence(ws, to_nation(primary_slot), owner, trigger_payload(tval[2]).signed_value);
		}
		void ef_diplomatic_influence_from_nation(EFFECT_PARAMTERS) {
			nations::adjust_influence(ws, to_nation(primary_slot), to_nation(from_slot), trigger_payload(tval[2]).signed_value);
		}
		void ef_diplomatic_influence_from_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner)
				nations::adjust_influence(ws, to_nation(primary_slot), owner, trigger_payload(tval[2]).signed_value);
		}
		void ef_relation(EFFECT_PARAMTERS) {
			if(auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag]; holder)
				nations::adjust_relationship(ws, to_nation(primary_slot), holder, trigger_payload(tval[3]).signed_value);
		}
		void ef_relation_this_nation(EFFECT_PARAMTERS) {
			nations::adjust_relationship(ws, to_nation(primary_slot), to_nation(this_slot), trigger_payload(tval[2]).signed_value);
		}
		void ef_relation_this_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner)
				nations::adjust_relationship(ws, to_nation(primary_slot), owner, trigger_payload(tval[2]).signed_value);
		}
		void ef_relation_from_nation(EFFECT_PARAMTERS) {
			nations::adjust_relationship(ws, to_nation(primary_slot), to_nation(from_slot), trigger_payload(tval[2]).signed_value);
		}
		void ef_relation_from_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner)
				nations::adjust_relationship(ws, to_nation(primary_slot), owner, trigger_payload(tval[2]).signed_value);
		}
		void ef_add_province_modifier(EFFECT_PARAMTERS) {
			modifiers::add_timed_modifier_to_province(
				ws,
				to_prov(primary_slot),
				trigger_payload(tval[2]).prov_mod,
				date_tag(to_index(ws.w.current_date) + trigger_payload(tval[3]).signed_value));
		}
		void ef_add_province_modifier_no_duration(EFFECT_PARAMTERS) {
			modifiers::add_static_modifier_to_province(ws, to_prov(primary_slot), trigger_payload(tval[2]).prov_mod);
		}
		void ef_add_country_modifier(EFFECT_PARAMTERS) {
			modifiers::add_timed_modifier_to_nation(
				ws,
				to_nation(primary_slot),
				trigger_payload(tval[2]).nat_mod,
				date_tag(to_index(ws.w.current_date) + trigger_payload(tval[3]).signed_value));
		}
		void ef_add_country_modifier_no_duration(EFFECT_PARAMTERS) {
			modifiers::add_unique_static_modifier_to_nation(ws, to_nation(primary_slot), trigger_payload(tval[2]).nat_mod);
		}
		void ef_casus_belli_tag(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;
			auto tag_target = trigger_payload(tval[4]).tag;

			if(auto holder = ws.w.culture_s.tags_to_holders[tag_target]; holder) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {
						holder, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()});
			}
		}
		void ef_casus_belli_int(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = provinces::province_owner(ws, provinces::province_tag(tval[4])); owner) {
				add_item(ws.w.military_s.cb_arrays, 
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {
						owner, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()});
			}
		}
		void ef_casus_belli_this_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays, 
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
				military::pending_cb {
					to_nation(this_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()});	
		}
		void ef_casus_belli_this_state(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {
						owner, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_casus_belli_this_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {
						owner, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_casus_belli_this_pop(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = population::get_pop_owner(ws, to_pop(this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {
						owner, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_casus_belli_from_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays,
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
				military::pending_cb {
					to_nation(from_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
			});
		}
		void ef_casus_belli_from_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {
						owner, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_add_casus_belli_tag(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;
			auto tag_target = trigger_payload(tval[4]).tag;

			if(auto holder = ws.w.culture_s.tags_to_holders[tag_target]; holder) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(holder),
					military::pending_cb {
						to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_add_casus_belli_int(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = provinces::province_owner(ws, provinces::province_tag(tval[4])); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb {
						to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_add_casus_belli_this_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays,
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(this_slot)),
				military::pending_cb {
					to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
			});
		}
		void ef_add_casus_belli_this_state(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb {
						to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_add_casus_belli_this_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb {
						to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_add_casus_belli_this_pop(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = population::get_pop_owner(ws, to_pop(this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb {
						to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_add_casus_belli_from_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays,
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(from_slot)),
				military::pending_cb {
					to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
			});
		}
		void ef_add_casus_belli_from_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb {
						to_nation(primary_slot), type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		void ef_remove_casus_belli_tag(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto tag_target = trigger_payload(tval[3]).tag;

			if(auto holder = ws.w.culture_s.tags_to_holders[tag_target]; holder) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {holder, type, date_tag()});
			}
		}
		void ef_remove_casus_belli_int(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = provinces::province_owner(ws, provinces::province_tag(tval[3])); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb {owner, type, date_tag()});
			}
		}
		void ef_remove_casus_belli_this_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
				military::pending_cb {to_nation(this_slot), type, date_tag()});
		}
		void ef_remove_casus_belli_this_state(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb { owner, type, date_tag() });
			}
		}
		void ef_remove_casus_belli_this_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb { owner, type, date_tag() });
			}
		}
		void ef_remove_casus_belli_this_pop(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = population::get_pop_owner(ws, to_pop(this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb { owner, type, date_tag() });
			}
		}
		void ef_remove_casus_belli_from_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
				military::pending_cb {to_nation(from_slot), type, date_tag()});
		}
		void ef_remove_casus_belli_from_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(primary_slot)),
					military::pending_cb { owner, type, date_tag() });
			}
		}
		void ef_this_remove_casus_belli_tag(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto tag_target = trigger_payload(tval[3]).tag;

			if(auto holder = ws.w.culture_s.tags_to_holders[tag_target]; holder) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(holder),
					military::pending_cb { to_nation(primary_slot), type, date_tag()});
			}
		}
		void ef_this_remove_casus_belli_int(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = provinces::province_owner(ws, provinces::province_tag(tval[3])); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb { to_nation(primary_slot), type, date_tag()});
			}
		}
		void ef_this_remove_casus_belli_this_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(this_slot)),
				military::pending_cb {to_nation(primary_slot), type, date_tag()});
		}
		void ef_this_remove_casus_belli_this_state(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb { to_nation(primary_slot), type, date_tag() });
			}
		}
		void ef_this_remove_casus_belli_this_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb { to_nation(primary_slot), type, date_tag() });
			}
		}
		void ef_this_remove_casus_belli_this_pop(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = population::get_pop_owner(ws, to_pop(this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb { to_nation(primary_slot), type, date_tag() });
			}
		}
		void ef_this_remove_casus_belli_from_nation(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				ws.w.nation_s.nations.get<nation::active_cbs>(to_nation(from_slot)),
				military::pending_cb {to_nation(primary_slot), type, date_tag()});
		}
		void ef_this_remove_casus_belli_from_province(EFFECT_PARAMTERS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					ws.w.nation_s.nations.get<nation::active_cbs>(owner),
					military::pending_cb { to_nation(primary_slot), type, date_tag() });
			}
		}
		void ef_war_tag(EFFECT_PARAMTERS) {
			auto target = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(target) {
				auto new_war = military::create_war(ws, to_nation(primary_slot), target, true);

				auto attacker_wg_count = tval[3];
				uint32_t payload_index = 4;
				for(int32_t i = 0; i < attacker_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = provinces::province_tag(tval[payload_index + 1]);
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

					auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

					ws.add_item(ws.get<war::war_goals>(new_war),
						military::war_goal{ ws.w.current_date, 0.0f, to_nation(primary_slot), state, target, bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
					payload_index += 3;
				}

				auto defender_wg_count = tval[payload_index];
				++payload_index;
				for(int32_t i = 0; i < defender_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = provinces::province_tag(tval[payload_index + 1]);
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

					auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

					ws.add_item(ws.get<war::war_goals>(new_war),
						military::war_goal{ ws.w.current_date, 0.0f, target, state, to_nation(primary_slot), bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
					payload_index += 3;
				}
			}
		}
		void ef_war_this_nation(EFFECT_PARAMTERS) {
			auto target = to_nation(this_slot);

			auto new_war = military::create_war(ws, to_nation(primary_slot), target, true);

			auto attacker_wg_count = tval[2];
			uint32_t payload_index = 3;
			for(int32_t i = 0; i < attacker_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = provinces::province_tag(tval[payload_index + 1]);
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

				auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

				add_item(ws.w.military_s.war_goal_arrays, ws.get<war::war_goals>(new_war),
					military::war_goal{ ws.w.current_date, 0.0f, to_nation(primary_slot), state, target, bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
				payload_index += 3;
			}

			auto defender_wg_count = tval[payload_index];
			++payload_index;
			for(int32_t i = 0; i < defender_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = provinces::province_tag(tval[payload_index + 1]);
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

				auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

				add_item(ws.w.military_s.war_goal_arrays, ws.get<war::war_goals>(new_war),
					military::war_goal{ ws.w.current_date, 0.0f, target, state, to_nation(primary_slot), bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
				payload_index += 3;
			}
		}
		void ef_war_this_state(EFFECT_PARAMTERS) {
			if(auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot)); owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_war_this_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_war_this_pop(EFFECT_PARAMTERS) {
			if(auto owner = population::get_pop_owner(ws, to_pop(this_slot)); owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_war_from_nation(EFFECT_PARAMTERS) {
			ef_war_this_nation(tval, ws, primary_slot, from_slot, const_parameter(), gen);
		}
		void ef_war_from_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_war_no_ally_tag(EFFECT_PARAMTERS) {
			auto target = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
			if(target) {
				auto new_war = military::create_war(ws, to_nation(primary_slot), target, false);

				auto attacker_wg_count = tval[3];
				uint32_t payload_index = 4;
				for(int32_t i = 0; i < attacker_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = provinces::province_tag(tval[payload_index + 1]);
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

					auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

					add_item(ws.w.military_s.war_goal_arrays, ws.get<war::war_goals>(new_war),
						military::war_goal{ ws.w.current_date, 0.0f, to_nation(primary_slot), state, target, bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
					payload_index += 3;
				}

				auto defender_wg_count = tval[payload_index];
				++payload_index;
				for(int32_t i = 0; i < defender_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = provinces::province_tag(tval[payload_index + 1]);
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

					auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

					add_item(ws.w.military_s.war_goal_arrays, ws.get<war::war_goals>(new_war),
						military::war_goal{ ws.w.current_date, 0.0f, target, state, to_nation(primary_slot), bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
					payload_index += 3;
				}
			}
		}
		void ef_war_no_ally_this_nation(EFFECT_PARAMTERS) {
			nations::country_tag target = to_nation(this_slot);

			auto new_war = military::create_war(ws, to_nation(primary_slot), target, false);

			auto attacker_wg_count = tval[2];
			uint32_t payload_index = 3;
			for(int32_t i = 0; i < attacker_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = provinces::province_tag(tval[payload_index + 1]);
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

				auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

				add_item(ws.w.military_s.war_goal_arrays, ws.get<war::war_goals>(new_war),
					military::war_goal{ ws.w.current_date, 0.0f, to_nation(primary_slot), state, target, bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
				payload_index += 3;
			}

			auto defender_wg_count = tval[payload_index];
			++payload_index;
			for(int32_t i = 0; i < defender_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = provinces::province_tag(tval[payload_index + 1]);
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);

				auto lib_holder = ws.w.culture_s.tags_to_holders[liberate_country_tag];

				add_item(ws.w.military_s.war_goal_arrays, ws.get<war::war_goals>(new_war),
					military::war_goal{ ws.w.current_date, 0.0f, target, state, to_nation(primary_slot), bool(lib_holder) ? lib_holder : nations::country_tag(), cb_type });
				payload_index += 3;
			}
		}
		void ef_war_no_ally_this_state(EFFECT_PARAMTERS) {
			if(auto owner = ws.w.nation_s.states.get<state::owner>(to_state(this_slot)); owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_war_no_ally_this_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(this_slot)); owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_war_no_ally_this_pop(EFFECT_PARAMTERS) {
			if(auto owner = population::get_pop_owner(ws, to_pop(this_slot)); owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_war_no_ally_from_nation(EFFECT_PARAMTERS) {
			ef_war_no_ally_this_nation(tval, ws, primary_slot, from_slot, const_parameter(), gen);
		}
		void ef_war_no_ally_from_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(from_slot)); owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, const_parameter(), gen);
		}
		void ef_country_event_this_nation(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_nation(primary_slot), to_nation(this_slot));
		}
		void ef_country_event_immediate_this_nation(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_nation(primary_slot), to_nation(this_slot));
		}
		void ef_province_event_this_nation(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_prov(primary_slot), to_nation(this_slot));
		}
		void ef_province_event_immediate_this_nation(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_prov(primary_slot), to_nation(this_slot));
		}
		void ef_country_event_this_state(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_nation(primary_slot), to_state(this_slot));
		}
		void ef_country_event_immediate_this_state(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_nation(primary_slot), to_state(this_slot));
		}
		void ef_province_event_this_state(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_prov(primary_slot), to_state(this_slot));
		}
		void ef_province_event_immediate_this_state(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_prov(primary_slot), to_state(this_slot));
		}
		void ef_country_event_this_province(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_nation(primary_slot), to_prov(this_slot));
		}
		void ef_country_event_immediate_this_province(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_nation(primary_slot), to_prov(this_slot));
		}
		void ef_province_event_this_province(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_prov(primary_slot), to_prov(this_slot));
		}
		void ef_province_event_immediate_this_province(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_prov(primary_slot), to_prov(this_slot));
		}
		void ef_country_event_this_pop(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_nation(primary_slot), to_pop(this_slot));
		}
		void ef_country_event_immediate_this_pop(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_nation(primary_slot), to_pop(this_slot));
		}
		void ef_province_event_this_pop(EFFECT_PARAMTERS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], to_prov(primary_slot), to_pop(this_slot));
		}
		void ef_province_event_immediate_this_pop(EFFECT_PARAMTERS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, to_prov(primary_slot), to_pop(this_slot));
		}
		void ef_country_event_province_this_nation(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_this_nation(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_country_event_immediate_province_this_nation(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_immediate_this_nation(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_country_event_province_this_state(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_this_state(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_country_event_immediate_province_this_state(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_immediate_this_state(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_country_event_province_this_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_this_province(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_country_event_immediate_province_this_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_immediate_this_province(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_country_event_province_this_pop(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_this_pop(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_country_event_immediate_province_this_pop(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_country_event_immediate_this_pop(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		
		void ef_sub_unit_int(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_sub_unit_this(EFFECT_PARAMTERS) {
			//do nothing
		}
		void ef_sub_unit_from(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_sub_unit_current(EFFECT_PARAMTERS) {
			// do nothing
		}
		void ef_set_variable(EFFECT_PARAMTERS) {
			ws.w.nation_s.national_variables.get(to_nation(primary_slot), trigger_payload(tval[2]).nat_var) = read_float_from_payload(tval + 3);
		}
		void ef_change_variable(EFFECT_PARAMTERS) {
			ws.w.nation_s.national_variables.get(to_nation(primary_slot), trigger_payload(tval[2]).nat_var) += read_float_from_payload(tval + 3);
		}
		void ef_ideology(EFFECT_PARAMTERS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto factor = read_float_from_payload(tval + 3);

			auto pop_id = to_pop(primary_slot);
			auto total_pop = ws.w.population_s.pops.get<pop::size>(pop_id);
			auto support = &(ws.w.population_s.pop_demographics.get_row(pop_id)[population::to_demo_tag(ws, ideologies::ideology_tag(0))]);

			Eigen::Map<Eigen::Array<float, 1, -1>> support_vec(support, ws.s.ideologies_m.ideologies_count);
			support_vec[to_index(ideology)] = std::max(0.0f, support[to_index(ideology)] + (total_pop * factor));
			if(auto sum = support_vec.sum(); sum != 0)
				support_vec *= total_pop / sum;
		}
		void ef_upper_house(EFFECT_PARAMTERS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto amount = trigger_payload(tval[3]).signed_value;
			auto upper_house = ws.w.nation_s.upper_house.get_row(to_nation(primary_slot));

			upper_house[ideology] = uint8_t(std::clamp(int32_t(upper_house[ideology]) + amount, 0, 255));
			normalize_integer_vector(upper_house.data(), ws.s.ideologies_m.ideologies_count, 100ui8);
		}
		void ef_scaled_militancy_issue(EFFECT_PARAMTERS) {
			auto issue_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto pop_size = ws.w.population_s.pops.get<pop::size>(to_pop(primary_slot));
			
			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(to_pop(primary_slot), issue_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_militancy_direct(ws, to_pop(primary_slot), std::clamp(adjustment + population::get_militancy_direct(ws, to_pop(primary_slot)), 0.0f, 1.0f));
			}
		}
		void ef_scaled_militancy_ideology(EFFECT_PARAMTERS) {
			auto ideology_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto pop_size = ws.w.population_s.pops.get<pop::size>(to_pop(primary_slot));

			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(to_pop(primary_slot), ideology_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_militancy_direct(ws, to_pop(primary_slot), std::clamp(adjustment + population::get_militancy_direct(ws, to_pop(primary_slot)), 0.0f, 1.0f));
			}
		}
		void ef_scaled_militancy_unemployment(EFFECT_PARAMTERS) {
			auto pop_size = ws.w.population_s.pops.get<pop::size>(to_pop(primary_slot));

			if(pop_size != 0) {
				auto unemployed = pop_size - ws.w.population_s.pop_demographics.get(to_pop(primary_slot), population::total_employment_tag);
				float adjustment = read_float_from_payload(tval + 2) * float(unemployed) / float(pop_size);
				population::set_militancy_direct(ws, to_pop(primary_slot), std::clamp(adjustment + population::get_militancy_direct(ws, to_pop(primary_slot)), 0.0f, 1.0f));
			}
		}
		void ef_scaled_consciousness_issue(EFFECT_PARAMTERS) {
			auto issue_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto pop_size = ws.w.population_s.pops.get<pop::size>(to_pop(primary_slot));

			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(to_pop(primary_slot), issue_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_consciousness_direct(ws, to_pop(primary_slot), std::clamp(adjustment + population::get_consciousness_direct(ws, to_pop(primary_slot)), 0.0f, 1.0f));
			}
		}
		void ef_scaled_consciousness_ideology(EFFECT_PARAMTERS) {
			auto ideology_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto pop_size = ws.w.population_s.pops.get<pop::size>(to_pop(primary_slot));

			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(to_pop(primary_slot), ideology_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_consciousness_direct(ws, to_pop(primary_slot), std::clamp(adjustment + population::get_consciousness_direct(ws, to_pop(primary_slot)), 0.0f, 1.0f));
			}
		}
		void ef_scaled_consciousness_unemployment(EFFECT_PARAMTERS) {
			auto pop_size = ws.w.population_s.pops.get<pop::size>(to_pop(primary_slot));

			if(pop_size != 0) {
				auto unemployed = pop_size - ws.w.population_s.pop_demographics.get(to_pop(primary_slot), population::total_employment_tag);
				float adjustment = read_float_from_payload(tval + 2) * float(unemployed) / float(pop_size);
				population::set_consciousness_direct(ws, to_pop(primary_slot), std::clamp(adjustment + population::get_consciousness_direct(ws, to_pop(primary_slot)), 0.0f, 1.0f));
			}
		}
		void ef_scaled_militancy_nation_issue(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_nation_ideology(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_nation_unemployment(EFFECT_PARAMTERS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_nation_issue(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_nation_ideology(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_nation_unemployment(EFFECT_PARAMTERS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_state_issue(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_state(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_state_ideology(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_state(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_state_unemployment(EFFECT_PARAMTERS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, to_state(primary_slot), [&ws, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_state_issue(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_state(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_state_ideology(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, to_state(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_state_unemployment(EFFECT_PARAMTERS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, to_state(primary_slot), [&ws, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_province_issue(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_province_ideology(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_militancy_province_unemployment(EFFECT_PARAMTERS) {
			auto factor = read_float_from_payload(tval + 2);

			provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(ws, p, std::clamp(adjustment + population::get_militancy_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_province_issue(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_province_ideology(EFFECT_PARAMTERS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, demo_tag, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_scaled_consciousness_province_unemployment(EFFECT_PARAMTERS) {
			auto factor = read_float_from_payload(tval + 2);

			provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, factor](population::pop_tag p) {
				auto pop_size = ws.w.population_s.pops.get<pop::size>(p);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(ws, p, std::clamp(adjustment + population::get_consciousness_direct(ws, p), 0.0f, 1.0f));
				}
			});
		}
		void ef_variable_good_name(EFFECT_PARAMTERS) {
			ws.w.nation_s.national_stockpiles.get(to_nation(primary_slot), trigger_payload(tval[2]).small.values.good) += economy::goods_qnty_type(read_float_from_payload(tval + 3));
		}
		void ef_variable_good_name_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ws.w.nation_s.national_stockpiles.get(owner, trigger_payload(tval[2]).small.values.good) += economy::goods_qnty_type(read_float_from_payload(tval + 3));
		}
		void ef_define_general(EFFECT_PARAMTERS) {
			auto new_leader = military::make_empty_leader(ws, ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot)), true);
			ws.set<military_leader::creation_date>(new_leader, ws.w.current_date);
			ws.set<military_leader::personality>(new_leader, trigger_payload(tval[3]).small.values.leader_trait);
			ws.set<military_leader::background>(new_leader, trigger_payload(tval[4]).small.values.leader_trait);
			military::calculate_leader_traits(ws, new_leader);

			add_item(ws.w.military_s.leader_arrays, ws.w.nation_s.nations.get<nation::generals>(to_nation(primary_slot)), new_leader);
		}
		void ef_define_admiral(EFFECT_PARAMTERS) {
			auto new_leader = military::make_empty_leader(ws, ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot)), false);
			ws.set<military_leader::creation_date>(new_leader, ws.w.current_date);
			ws.set<military_leader::personality>(new_leader, trigger_payload(tval[3]).small.values.leader_trait);
			ws.set<military_leader::background>(new_leader, trigger_payload(tval[4]).small.values.leader_trait);
			military::calculate_leader_traits(ws, new_leader);

			add_item(ws.w.military_s.leader_arrays, ws.w.nation_s.nations.get<nation::admirals>(to_nation(primary_slot)), new_leader);
		}
		void ef_dominant_issue(EFFECT_PARAMTERS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto factor = read_float_from_payload(tval + 3);

			auto pop_id = to_pop(primary_slot);
			auto total_pop = ws.w.population_s.pop_demographics.get(pop_id, population::total_population_tag);
			auto support = &(ws.w.population_s.pop_demographics.get_row(pop_id)[population::to_demo_tag(ws, issues::option_tag(0))]);

			Eigen::Map<Eigen::Array<float, 1, -1>> support_vec(support, ws.s.issues_m.tracked_options_count);
			support_vec[to_index(opt)] = std::max(0.0f, support[to_index(opt)] + (total_pop * factor));
			if(auto sum = support_vec.sum(); sum != 0)
				support_vec *= total_pop / sum;

		}
		void ef_dominant_issue_nation(EFFECT_PARAMTERS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto factor = read_float_from_payload(tval + 3);
			auto first_issue_index = population::to_demo_tag(ws, issues::option_tag(0));

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, opt, factor, first_issue_index](population::pop_tag p) {
				auto total_pop = ws.w.population_s.pop_demographics.get(p, population::total_population_tag);
				auto support = &(ws.w.population_s.pop_demographics.get_row(p)[first_issue_index]);

				Eigen::Map<Eigen::Array<float, 1, -1>> support_vec(support, ws.s.issues_m.tracked_options_count);
				support_vec[to_index(opt)] = std::max(0.0f, support[to_index(opt)] + (total_pop * factor));
				if(auto sum = support_vec.sum(); sum != 0)
					support_vec *= total_pop / sum;
			});
		}
		void ef_add_war_goal(EFFECT_PARAMTERS) {
			if(auto w = military::get_war_between(ws, to_nation(primary_slot), to_nation(from_slot)); w) {
				add_item(ws.w.military_s.war_goal_arrays, ws.get<war::war_goals>(w),
					military::war_goal{
						ws.w.current_date,
						0.0f,
						to_nation(from_slot),
						nations::state_tag(),
						to_nation(primary_slot),
						nations::country_tag(),
						trigger_payload(tval[2]).small.values.cb_type});
			}
		}
		void ef_move_issue_percentage_nation(EFFECT_PARAMTERS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			nations::for_each_pop(ws, to_nation(primary_slot), [&ws, from_issue, to_issue, amount](population::pop_tag p) {
				auto subtract = ws.w.population_s.pop_demographics.get(p, from_issue) * amount;
				ws.w.population_s.pop_demographics.get(p, from_issue) -= subtract;
				ws.w.population_s.pop_demographics.get(p, to_issue) += subtract;
			});
		}
		void ef_move_issue_percentage_state(EFFECT_PARAMTERS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			nations::for_each_pop(ws, to_state(primary_slot), [&ws, from_issue, to_issue, amount](population::pop_tag p) {
				auto subtract = ws.w.population_s.pop_demographics.get(p, from_issue) * amount;
				ws.w.population_s.pop_demographics.get(p, from_issue) -= subtract;
				ws.w.population_s.pop_demographics.get(p, to_issue) += subtract;
			});
		}
		void ef_move_issue_percentage_province(EFFECT_PARAMTERS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			provinces::for_each_pop(ws, to_prov(primary_slot), [&ws, from_issue, to_issue, amount](population::pop_tag p) {
				auto subtract = ws.w.population_s.pop_demographics.get(p, from_issue) * amount;
				ws.w.population_s.pop_demographics.get(p, from_issue) -= subtract;
				ws.w.population_s.pop_demographics.get(p, to_issue) += subtract;
			});
		}
		void ef_move_issue_percentage_pop(EFFECT_PARAMTERS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			auto subtract = int32_t(ws.w.population_s.pop_demographics.get(to_pop(primary_slot), from_issue) * amount);
			ws.w.population_s.pop_demographics.get(to_pop(primary_slot), from_issue) -= subtract;
			ws.w.population_s.pop_demographics.get(to_pop(primary_slot), to_issue) += subtract;
		}
		void ef_party_loyalty_nation_from_province(EFFECT_PARAMTERS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 3);
			ws.w.province_s.party_loyalty.get(to_prov(from_slot), ideology) += amount;
		}
		void ef_party_loyalty_province_from_nation(EFFECT_PARAMTERS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 3);
			ws.w.province_s.party_loyalty.get(to_prov(primary_slot), ideology) += amount;
		}
		void ef_party_loyalty_province_id_nation(EFFECT_PARAMTERS) {
			provinces::province_tag prov(tval[2]);
			auto ideology = trigger_payload(tval[3]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 4);
			ws.w.province_s.party_loyalty.get(prov, ideology) += amount;
		}
		void ef_party_loyalty_province_id_from_nation(EFFECT_PARAMTERS) {
			provinces::province_tag prov(tval[2]);
			auto ideology = trigger_payload(tval[3]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 4);
			ws.w.province_s.party_loyalty.get(prov, ideology) += amount;
		}
		void ef_build_railway_in_capital_yes_whole_state_yes_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				if(auto si = provinces::province_state(ws, capital); si) {
					nations::for_each_province(ws, si, [&ws](provinces::province_tag p) {
						if(ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(p) <= 1.0f)
							++ws.w.province_s.province_state_container.get<province_state::railroad_level>(p);
					});
				}
			}
		}
		void ef_build_railway_in_capital_yes_whole_state_no_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				if(auto si = provinces::province_state(ws, capital); si) {
					nations::for_each_province(ws, si, [&ws](provinces::province_tag p) {
						if(ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(p) <= 1.0f)
							++ws.w.province_s.province_state_container.get<province_state::railroad_level>(p);
					});
				}
			}
		}
		void ef_build_railway_in_capital_no_whole_state_yes_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				if(ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(capital) <= 1.0f)
					++ws.w.province_s.province_state_container.get<province_state::railroad_level>(capital);
			}
		}
		void ef_build_railway_in_capital_no_whole_state_no_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				if(ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(capital) <= 1.0f)
					++ws.w.province_s.province_state_container.get<province_state::railroad_level>(capital);
			}
		}
		void ef_build_fort_in_capital_yes_whole_state_yes_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				if(auto si = provinces::province_state(ws, capital); si) {
					nations::for_each_province(ws, si, [&ws](provinces::province_tag p) {
						++ws.w.province_s.province_state_container.get<province_state::fort_level>(p);
					});
				}
			}
		}
		void ef_build_fort_in_capital_yes_whole_state_no_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				if(auto si = provinces::province_state(ws, capital); si) {
					nations::for_each_province(ws, si, [&ws](provinces::province_tag p) {
						++ws.w.province_s.province_state_container.get<province_state::fort_level>(p);
					});
				}
			}
		}
		void ef_build_fort_in_capital_no_whole_state_yes_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				++ws.w.province_s.province_state_container.get<province_state::fort_level>(capital);
			}
		}
		void ef_build_fort_in_capital_no_whole_state_no_limit(EFFECT_PARAMTERS) {
			auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
			if(is_valid_index(capital)) {
				++ws.w.province_s.province_state_container.get<province_state::fort_level>(capital);
			}
		}
		void ef_relation_reb(EFFECT_PARAMTERS) {
			auto itag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
			if(auto holder = ws.w.culture_s.tags_to_holders[itag]; holder)
				nations::adjust_relationship(ws, to_nation(primary_slot), holder, trigger_payload(tval[2]).signed_value);
		}
		void ef_variable_tech_name(EFFECT_PARAMTERS) {
			technologies::apply_single_technology(ws, to_nation(primary_slot), trigger_payload(tval[2]).tech);
		}
		void ef_set_country_flag_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_set_country_flag(tval, ws, owner, const_parameter(), const_parameter(), gen);
		}
		void ef_add_country_modifier_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_add_country_modifier(tval, ws, owner, const_parameter(), const_parameter(), gen);
		}
		void ef_add_country_modifier_province_no_duration(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_add_country_modifier_no_duration(tval, ws, owner, const_parameter(), const_parameter(), gen);
		}
		void ef_relation_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_relation(tval, ws, owner, const_parameter(), const_parameter(), gen);
		}
		void ef_relation_province_this_nation(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_relation_this_nation(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_relation_province_this_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_relation_this_province(tval, ws, owner, this_slot, const_parameter(), gen);
		}
		void ef_relation_province_from_nation(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_relation_this_nation(tval, ws, owner, from_slot, const_parameter(), gen);
		}
		void ef_relation_province_from_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_relation_this_province(tval, ws, owner, from_slot, const_parameter(), gen);
		}
		void ef_relation_province_reb(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_relation_reb(tval, ws, owner, const_parameter(), from_slot, gen);
		}
		void ef_treasury_province(EFFECT_PARAMTERS) {
			if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner)
				ef_treasury(tval, ws, owner, this_slot, const_parameter(), gen);
		}

		static void(*effect_functions[])(uint16_t const*, world_state&, parameter, parameter, parameter, jsf_prng&) = {
			ef_none,
			ef_capital,
			ef_add_core_tag,
			ef_add_core_int,
			ef_add_core_this_nation,
			ef_add_core_this_province,
			ef_add_core_this_state,
			ef_add_core_this_pop,
			ef_add_core_from_province,
			ef_add_core_from_nation,
			ef_add_core_reb,
			ef_remove_core_tag,
			ef_remove_core_int,
			ef_remove_core_this_nation,
			ef_remove_core_this_province,
			ef_remove_core_this_state,
			ef_remove_core_this_pop,
			ef_remove_core_from_province,
			ef_remove_core_from_nation,
			ef_remove_core_reb,
			ef_change_region_name_state,
			ef_change_region_name_province,
			ef_trade_goods,
			ef_add_accepted_culture,
			ef_add_accepted_culture_union,
			ef_primary_culture,
			ef_primary_culture_this_nation,
			ef_primary_culture_this_state,
			ef_primary_culture_this_province,
			ef_primary_culture_this_pop,
			ef_primary_culture_from_nation,
			ef_remove_accepted_culture,
			ef_life_rating,
			ef_religion,
			ef_is_slave_state_yes,
			ef_is_slave_pop_yes,
			ef_research_points,
			ef_tech_school,
			ef_government,
			ef_government_reb,
			ef_treasury,
			ef_war_exhaustion,
			ef_prestige,
			ef_change_tag,
			ef_change_tag_culture,
			ef_change_tag_no_core_switch,
			ef_change_tag_no_core_switch_culture,
			ef_set_country_flag,
			ef_clr_country_flag,
			ef_military_access,
			ef_military_access_this_nation,
			ef_military_access_this_province,
			ef_military_access_from_nation,
			ef_military_access_from_province,
			ef_badboy,
			ef_secede_province,
			ef_secede_province_this_nation,
			ef_secede_province_this_state,
			ef_secede_province_this_province,
			ef_secede_province_this_pop,
			ef_secede_province_from_nation,
			ef_secede_province_from_province,
			ef_secede_province_reb,
			ef_inherit,
			ef_inherit_this_nation,
			ef_inherit_this_state,
			ef_inherit_this_province,
			ef_inherit_this_pop,
			ef_inherit_from_nation,
			ef_inherit_from_province,
			ef_annex_to,
			ef_annex_to_this_nation,
			ef_annex_to_this_state,
			ef_annex_to_this_province,
			ef_annex_to_this_pop,
			ef_annex_to_from_nation,
			ef_annex_to_from_province,
			ef_release,
			ef_release_this_nation,
			ef_release_this_state,
			ef_release_this_province,
			ef_release_this_pop,
			ef_release_from_nation,
			ef_release_from_province,
			ef_change_controller,
			ef_change_controller_this_nation,
			ef_change_controller_this_province,
			ef_change_controller_from_nation,
			ef_change_controller_from_province,
			ef_infrastructure,
			ef_money,
			ef_leadership,
			ef_create_vassal,
			ef_create_vassal_this_nation,
			ef_create_vassal_this_province,
			ef_create_vassal_from_nation,
			ef_create_vassal_from_province,
			ef_end_military_access,
			ef_end_military_access_this_nation,
			ef_end_military_access_this_province,
			ef_end_military_access_from_nation,
			ef_end_military_access_from_province,
			ef_leave_alliance,
			ef_leave_alliance_this_nation,
			ef_leave_alliance_this_province,
			ef_leave_alliance_from_nation,
			ef_leave_alliance_from_province,
			ef_end_war,
			ef_end_war_this_nation,
			ef_end_war_this_province,
			ef_end_war_from_nation,
			ef_end_war_from_province,
			ef_enable_ideology,
			ef_ruling_party_ideology,
			ef_plurality,
			ef_remove_province_modifier,
			ef_remove_country_modifier,
			ef_create_alliance,
			ef_create_alliance_this_nation,
			ef_create_alliance_this_province,
			ef_create_alliance_from_nation,
			ef_create_alliance_from_province,
			ef_release_vassal,
			ef_release_vassal_this_nation,
			ef_release_vassal_this_province,
			ef_release_vassal_from_nation,
			ef_release_vassal_from_province,
			ef_release_vassal_reb,
			ef_release_vassal_random,
			ef_change_province_name,
			ef_enable_canal,
			ef_set_global_flag,
			ef_clr_global_flag,
			ef_nationalvalue_province,
			ef_nationalvalue_nation,
			ef_civilized_yes,
			ef_civilized_no,
			ef_is_slave_state_no,
			ef_is_slave_pop_no,
			ef_election,
			ef_social_reform,
			ef_political_reform,
			ef_add_tax_relative_income,
			ef_neutrality,
			ef_reduce_pop,
			ef_move_pop,
			ef_pop_type,
			ef_years_of_research,
			ef_prestige_factor_positive,
			ef_prestige_factor_negative,
			ef_military_reform,
			ef_economic_reform,
			ef_remove_random_military_reforms,
			ef_remove_random_economic_reforms,
			ef_add_crime,
			ef_add_crime_none,
			ef_nationalize,
			ef_build_factory_in_capital_state,
			ef_activate_technology,
			ef_great_wars_enabled_yes,
			ef_great_wars_enabled_no,
			ef_world_wars_enabled_yes,
			ef_world_wars_enabled_no,
			ef_assimilate_province,
			ef_assimilate_pop,
			ef_literacy,
			ef_add_crisis_interest,
			ef_flashpoint_tension,
			ef_add_crisis_temperature,
			ef_consciousness,
			ef_militancy,
			ef_rgo_size,
			ef_fort,
			ef_naval_base,
			ef_trigger_revolt_nation,
			ef_trigger_revolt_state,
			ef_trigger_revolt_province,
			ef_diplomatic_influence,
			ef_diplomatic_influence_this_nation,
			ef_diplomatic_influence_this_province,
			ef_diplomatic_influence_from_nation,
			ef_diplomatic_influence_from_province,
			ef_relation,
			ef_relation_this_nation,
			ef_relation_this_province,
			ef_relation_from_nation,
			ef_relation_from_province,
			ef_add_province_modifier,
			ef_add_province_modifier_no_duration,
			ef_add_country_modifier,
			ef_add_country_modifier_no_duration,
			ef_casus_belli_tag,
			ef_casus_belli_int,
			ef_casus_belli_this_nation,
			ef_casus_belli_this_state,
			ef_casus_belli_this_province,
			ef_casus_belli_this_pop,
			ef_casus_belli_from_nation,
			ef_casus_belli_from_province,
			ef_add_casus_belli_tag,
			ef_add_casus_belli_int,
			ef_add_casus_belli_this_nation,
			ef_add_casus_belli_this_state,
			ef_add_casus_belli_this_province,
			ef_add_casus_belli_this_pop,
			ef_add_casus_belli_from_nation,
			ef_add_casus_belli_from_province,
			ef_remove_casus_belli_tag,
			ef_remove_casus_belli_int,
			ef_remove_casus_belli_this_nation,
			ef_remove_casus_belli_this_state,
			ef_remove_casus_belli_this_province,
			ef_remove_casus_belli_this_pop,
			ef_remove_casus_belli_from_nation,
			ef_remove_casus_belli_from_province,
			ef_this_remove_casus_belli_tag,
			ef_this_remove_casus_belli_int,
			ef_this_remove_casus_belli_this_nation,
			ef_this_remove_casus_belli_this_state,
			ef_this_remove_casus_belli_this_province,
			ef_this_remove_casus_belli_this_pop,
			ef_this_remove_casus_belli_from_nation,
			ef_this_remove_casus_belli_from_province,
			ef_war_tag,
			ef_war_this_nation,
			ef_war_this_state,
			ef_war_this_province,
			ef_war_this_pop,
			ef_war_from_nation,
			ef_war_from_province,
			ef_war_no_ally_tag,
			ef_war_no_ally_this_nation,
			ef_war_no_ally_this_state,
			ef_war_no_ally_this_province,
			ef_war_no_ally_this_pop,
			ef_war_no_ally_from_nation,
			ef_war_no_ally_from_province,
			ef_country_event_this_nation,
			ef_country_event_immediate_this_nation,
			ef_province_event_this_nation,
			ef_province_event_immediate_this_nation,
			ef_sub_unit_int,
			ef_sub_unit_this,
			ef_sub_unit_from,
			ef_sub_unit_current,
			ef_set_variable,
			ef_change_variable,
			ef_ideology,
			ef_upper_house,
			ef_scaled_militancy_issue,
			ef_scaled_militancy_ideology,
			ef_scaled_militancy_unemployment,
			ef_scaled_consciousness_issue,
			ef_scaled_consciousness_ideology,
			ef_scaled_consciousness_unemployment,
			ef_define_general,
			ef_define_admiral,
			ef_dominant_issue,
			ef_add_war_goal,
			ef_move_issue_percentage_nation,
			ef_move_issue_percentage_state,
			ef_move_issue_percentage_province,
			ef_move_issue_percentage_pop,
			ef_party_loyalty_nation_from_province,
			ef_party_loyalty_province_from_nation,
			ef_party_loyalty_province_id_nation,
			ef_party_loyalty_province_id_from_nation,
			ef_build_railway_in_capital_yes_whole_state_yes_limit,
			ef_build_railway_in_capital_yes_whole_state_no_limit,
			ef_build_railway_in_capital_no_whole_state_yes_limit,
			ef_build_railway_in_capital_no_whole_state_no_limit,
			ef_build_fort_in_capital_yes_whole_state_yes_limit,
			ef_build_fort_in_capital_yes_whole_state_no_limit,
			ef_build_fort_in_capital_no_whole_state_yes_limit,
			ef_build_fort_in_capital_no_whole_state_no_limit,
			ef_relation_reb,
			ef_variable_tech_name,
			ef_variable_good_name,
			ef_set_country_flag_province,
			ef_add_country_modifier_province,
			ef_add_country_modifier_province_no_duration,
			ef_dominant_issue_nation,
			ef_relation_province,
			ef_relation_province_this_nation,
			ef_relation_province_this_province,
			ef_relation_province_from_nation,
			ef_relation_province_from_province,
			ef_relation_province_reb,
			ef_scaled_militancy_nation_issue,
			ef_scaled_militancy_nation_ideology,
			ef_scaled_militancy_nation_unemployment,
			ef_scaled_consciousness_nation_issue,
			ef_scaled_consciousness_nation_ideology,
			ef_scaled_consciousness_nation_unemployment,
			ef_scaled_militancy_state_issue,
			ef_scaled_militancy_state_ideology,
			ef_scaled_militancy_state_unemployment,
			ef_scaled_consciousness_state_issue,
			ef_scaled_consciousness_state_ideology,
			ef_scaled_consciousness_state_unemployment,
			ef_scaled_militancy_province_issue,
			ef_scaled_militancy_province_ideology,
			ef_scaled_militancy_province_unemployment,
			ef_scaled_consciousness_province_issue,
			ef_scaled_consciousness_province_ideology,
			ef_scaled_consciousness_province_unemployment,
			ef_variable_good_name_province,
			ef_treasury_province,
			ef_country_event_this_state,
			ef_country_event_immediate_this_state,
			ef_province_event_this_state,
			ef_province_event_immediate_this_state,
			ef_country_event_this_province,
			ef_country_event_immediate_this_province,
			ef_province_event_this_province,
			ef_province_event_immediate_this_province,
			ef_country_event_this_pop,
			ef_country_event_immediate_this_pop,
			ef_province_event_this_pop,
			ef_province_event_immediate_this_pop,
			ef_country_event_province_this_nation,
			ef_country_event_immediate_province_this_nation,
			ef_country_event_province_this_state,
			ef_country_event_immediate_province_this_state,
			ef_country_event_province_this_province,
			ef_country_event_immediate_province_this_province,
			ef_country_event_province_this_pop,
			ef_country_event_immediate_province_this_pop,
		};

		void _execute_effect(
			uint16_t const* tval,
			world_state& ws,
			parameter primary_slot,
			parameter this_slot,
			parameter from_slot,
			jsf_prng& generator_copy) {

			if((*tval & effect_codes::is_scope) != 0) {
				effect_scope_functions[*tval & effect_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, generator_copy);
			} else {
				effect_functions[*tval & effect_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, generator_copy);
			}
		}
	}

#ifdef __llvm__
#pragma clang diagnostic pop
#endif

	void execute_effect(
		uint16_t const* tval,
		world_state& ws,
		parameter primary_slot,
		parameter this_slot,
		parameter from_slot,
		jsf_prng generator_copy) {

		_execute_effect(tval, ws, primary_slot, this_slot, from_slot, generator_copy);
	}
}
