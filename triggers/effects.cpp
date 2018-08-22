#include "effects.h"
#include "codes.h"
#include <random>
#include "world_state\\world_state.h"
#include "nations\\nations_functions.hpp"
#include "provinces\\province_functions.hpp"
#include "population\\population_function.h"

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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wunused-parameter"

	namespace {
		inline void apply_subeffects(uint16_t const* source, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			const auto source_size = 1 + get_effect_payload_size(source);
			auto sub_units_start = source + 2 + effect_scope_data_payload(source[0]);

			while(sub_units_start < source + source_size) {
				execute_effect(sub_units_start, ws, primary_slot, this_slot, from_slot, rebel_slot, gen);
				sub_units_start += 1 + get_effect_payload_size(sub_units_start);
			}
		}

		void es_generic_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, primary_slot, this_slot, from_slot, rebel_slot, gen);
		}

		void es_x_neighbor_province_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(((provinces::province_state*)primary_slot)->id);

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;
				
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner && test_trigger(limit, ws, &ps, this_slot, from_slot, rebel_slot))
							rlist.push_back(&ps);
					}
				} else {
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner)
							rlist.push_back(&ps);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);

					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner && test_trigger(limit, ws, &ps, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &ps, this_slot, from_slot, rebel_slot, gen);
					}
				} else {
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner)
							apply_subeffects(tval, ws, &ps, this_slot, from_slot, rebel_slot, gen);
					}
				}
			}
		}
		void es_x_neighbor_country_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto neighbor_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->neighboring_nations);

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::nation*, 16, concurrent_allocator<nations::nation*>> rlist;
				
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n : neighbor_range) {
						auto& ns = ws.w.nation_s.nations[n];
						if(test_trigger(limit, ws, &ns, this_slot, from_slot, rebel_slot))
							rlist.push_back(&ns);
					}
				} else {
					for(auto n : neighbor_range)
						rlist.push_back(&(ws.w.nation_s.nations[n]));
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);

					for(auto n : neighbor_range) {
						auto& ns = ws.w.nation_s.nations[n];
						if(test_trigger(limit, ws, &ns, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &ns, this_slot, from_slot, rebel_slot, gen);
					}
				} else {
					for(auto n : neighbor_range) {
						apply_subeffects(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot, gen);
					}
				}
			}
		}
		void es_x_country_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::nation*, 64, concurrent_allocator<nations::nation*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					ws.w.nation_s.nations.for_each([&rlist, limit, &ws, primary_slot, this_slot, from_slot, rebel_slot](nations::nation& n) {
						if(&n != primary_slot && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0 && test_trigger(limit, ws, &n, this_slot, from_slot, rebel_slot))
							rlist.push_back(&n);
					});
				} else {
					ws.w.nation_s.nations.for_each([&rlist, &ws, primary_slot](nations::nation& n) {
						if(&n != primary_slot && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0)
							rlist.push_back(&n);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					ws.w.nation_s.nations.for_each([tval, &gen, limit, &ws, this_slot, from_slot, rebel_slot](nations::nation& n) {
						if(test_trigger(limit, ws, &n, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &n, this_slot, from_slot, rebel_slot, gen);
					});
				} else {
					ws.w.nation_s.nations.for_each([tval, &gen, &ws, this_slot, from_slot, rebel_slot](nations::nation& n) {
						apply_subeffects(tval, ws, &n, this_slot, from_slot, rebel_slot, gen);
					});
				}
			}
		}
		void es_x_country_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			es_x_country_scope_nation(tval, ws, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		void es_x_empty_neighbor_province_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(((provinces::province_state*)primary_slot)->id);

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner == nullptr && test_trigger(limit, ws, &ps, this_slot, from_slot, rebel_slot))
							rlist.push_back(&ps);
					}
				} else {
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner == nullptr)
							rlist.push_back(&ps);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);

					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner == nullptr && test_trigger(limit, ws, &ps, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &ps, this_slot, from_slot, rebel_slot, gen);
					}
				} else {
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner == nullptr)
							apply_subeffects(tval, ws, &ps, this_slot, from_slot, rebel_slot, gen);
					}
				}
			}
		}
		void es_x_greater_power_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto ranked_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
			int32_t great_nations_count = int32_t(ws.s.modifiers_m.global_defines.great_nations_count);
			int32_t count = 0;

			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::nation*, 16, concurrent_allocator<nations::nation*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							auto& nation = ws.w.nation_s.nations[*n];
							if(nations::is_great_power(ws, nation) && test_trigger(limit, ws, &nation, this_slot, from_slot, rebel_slot)) {
								++count;
								rlist.push_back(&nation);
							}
						}
					}
				} else {
					for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							auto& nation = ws.w.nation_s.nations[*n];
							if(nations::is_great_power(ws, nation)) {
								++count;
								rlist.push_back(&nation);
							}
						}
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							auto& nation = ws.w.nation_s.nations[*n];
							if(nations::is_great_power(ws, nation) && test_trigger(limit, ws, &nation, this_slot, from_slot, rebel_slot)) {
								++count;
								apply_subeffects(tval, ws, &nation, this_slot, from_slot, rebel_slot, gen);
							}
						}
					}
				} else {
					for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							auto& nation = ws.w.nation_s.nations[*n];
							if(nations::is_great_power(ws, nation)) {
								++count;
								apply_subeffects(tval, ws, &nation, this_slot, from_slot, rebel_slot, gen);
							}
						}
					}
				}
			}
		}
		void es_poor_strata_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj){
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_poor_strata_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_poor_strata_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_middle_strata_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_middle_strata_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_middle_strata_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_rich_strata_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_rich_strata_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_rich_strata_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich &&
						test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot)) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			} else {
				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
					auto type = pobj.type;
					if(is_valid_index(type) &&
						(ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich) {

						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					}
				});
			}
		}
		void es_x_pop_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<population::pop*, 16, concurrent_allocator<population::pop*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](population::pop& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&rlist](population::pop& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				} else {
					nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				}
			}
		}
		void es_x_pop_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<population::pop*, 16, concurrent_allocator<population::pop*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](population::pop& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&rlist](population::pop& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				} else {
					nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				}
			}
		}
		void es_x_pop_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<population::pop*, 16, concurrent_allocator<population::pop*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](population::pop& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&rlist](population::pop& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				} else {
					provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](population::pop& pobj) {
						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				}
			}
		}
		void es_x_owned_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, *((nations::nation*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](provinces::province_state& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_province(ws, *((nations::nation*)primary_slot), [&rlist](provinces::province_state& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, *((nations::nation*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](provinces::province_state& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				} else {
					nations::for_each_province(ws, *((nations::nation*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](provinces::province_state& pobj) {
						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				}
			}
		}
		void es_x_owned_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, *((nations::state_instance*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](provinces::province_state& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_province(ws, *((nations::state_instance*)primary_slot), [&rlist](provinces::province_state& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, *((nations::state_instance*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](provinces::province_state& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				} else {
					nations::for_each_province(ws, *((nations::state_instance*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](provinces::province_state& pobj) {
						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				}
			}
		}
		void es_x_core_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto tag = ((nations::nation*)primary_slot)->tag;
			if(is_valid_index(tag)) {
				auto cores_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);

				if((tval[0] & effect_codes::is_random_scope) != 0) {
					boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

					if((tval[0] & effect_codes::scope_has_limit) != 0) {
						auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
						for(auto p : cores_range) {
							if(is_valid_index(p)) {
								auto& pobj = ws.w.province_s.province_state_container[p];
								if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
									rlist.push_back(&pobj);
							}
						}
					} else {
						for(auto p : cores_range) {
							if(is_valid_index(p))
								rlist.push_back(&(ws.w.province_s.province_state_container[p]));
						}
					}

					if(rlist.size() != 0) {
						std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
						apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
					}
				} else {
					if((tval[0] & effect_codes::scope_has_limit) != 0) {
						auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
						for(auto p : cores_range) {
							if(is_valid_index(p)) {
								auto& pobj = ws.w.province_s.province_state_container[p];
								if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
									apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
							}
						}
					} else {
						for(auto p : cores_range) {
							if(is_valid_index(p)) 
								apply_subeffects(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot, gen);
						}
					}
				}
			}
		}
		void es_x_state_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				boost::container::small_vector<nations::state_instance*, 16, concurrent_allocator<nations::state_instance*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_state(ws, *((nations::nation*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](nations::state_instance& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_state(ws, *((nations::nation*)primary_slot), [&rlist](nations::state_instance& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[uint32_t(dist(gen))], this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_state(ws, *((nations::nation*)primary_slot), [&ws, limit, tval, this_slot, from_slot, rebel_slot, &gen](nations::state_instance& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				} else {
					nations::for_each_state(ws, *((nations::nation*)primary_slot), [&ws, tval, this_slot, from_slot, rebel_slot, &gen](nations::state_instance& pobj) {
						apply_subeffects(tval, ws, &pobj, this_slot, from_slot, rebel_slot, gen);
					});
				}
			}
		}
		void es_random_list_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			const auto source_size = 1 + get_effect_payload_size(tval);
			auto chances_total = tval[2];

			std::uniform_int_distribution<int32_t> dist(0, int32_t(chances_total) - 1);
			auto chance_taken = dist(gen);

			auto sub_units_start = tval + 3; // [code] + [payload size] + [chances total] + [first sub effect chance]
			while(sub_units_start < tval + source_size) {
				chance_taken -= *sub_units_start;
				if(chance_taken < 0) {
					execute_effect(sub_units_start + 1, ws, primary_slot, this_slot, from_slot, rebel_slot, gen);
					return;
				}
				sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1); // each member preceeded by uint16_t
			}
		}
		void es_random_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto chance = tval[2];
			std::uniform_int_distribution<int32_t> dist(0, 99);
			if(dist(gen) < chance)
				apply_subeffects(tval, ws, primary_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_owner_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = ((nations::state_instance*)primary_slot)->owner;
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, rebel_slot, gen);
		}
		void es_owner_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = ((provinces::province_state*)primary_slot)->owner;
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, rebel_slot, gen);
		}
		void es_controller_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto controller = ((provinces::province_state*)primary_slot)->controller;
			if(controller)
				apply_subeffects(tval, ws, controller, this_slot, from_slot, rebel_slot, gen);
		}
		void es_location_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto location = ((population::pop*)primary_slot)->location;
			if(is_valid_index(location))
				apply_subeffects(tval, ws, &(ws.w.province_s.province_state_container[location]), this_slot, from_slot, rebel_slot, gen);
		}
		void es_country_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = population::get_pop_owner(ws, *((population::pop*)primary_slot));
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, rebel_slot, gen);
		}
		void es_country_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = ((nations::state_instance*)primary_slot)->owner;
			if(owner)
				apply_subeffects(tval, ws, owner, this_slot, from_slot, rebel_slot, gen);
		}
		void es_capital_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital))
				apply_subeffects(tval, ws, &(ws.w.province_s.province_state_container[capital]), this_slot, from_slot, rebel_slot, gen);
		}
		void es_this_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_this_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_this_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_this_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, this_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_from_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_from_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_from_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_from_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, from_slot, this_slot, from_slot, rebel_slot, gen);
		}
		void es_sea_zone_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(((provinces::province_state*)primary_slot)->id);
			if(sea_zones.first != sea_zones.second)
				apply_subeffects(tval, ws, &(ws.w.province_s.province_state_container[*sea_zones.first]), this_slot, from_slot, rebel_slot, gen);
		}
		void es_cultural_union_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto prim_culture = ((nations::nation*)primary_slot)->primary_culture;
			if(!is_valid_index(prim_culture))
				return;

			auto culture_group = ws.s.culture_m.culture_container[prim_culture].group;
			if(is_valid_index(culture_group)) {
				auto union_tag = ws.s.culture_m.culture_groups[culture_group].union_tag;
				if(is_valid_index(union_tag)) {
					auto union_holder = ws.w.culture_s.national_tags_state[union_tag].holder;
					if(union_holder)
						apply_subeffects(tval, ws, union_holder, this_slot, from_slot, rebel_slot, gen);
				}
			}
		}
		void es_overlord_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto overlord = ((nations::nation*)primary_slot)->overlord;
			if(overlord)
				apply_subeffects(tval, ws, overlord, this_slot, from_slot, rebel_slot, gen);
		}
		void es_sphere_owner_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto sphere_leader = ((nations::nation*)primary_slot)->sphere_leader;
			if(sphere_leader)
				apply_subeffects(tval, ws, sphere_leader, this_slot, from_slot, rebel_slot, gen);
		}
		void es_independence_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto rtag = rebel_slot->independence_tag;
			if(is_valid_index(rtag)) {
				auto ination = ws.w.culture_s.national_tags_state[rtag].holder;
				if(ination)
					apply_subeffects(tval, ws, ination, this_slot, from_slot, rebel_slot, gen);
			}
		}
		void es_flashpoint_tag_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto ctag = ((nations::state_instance*)primary_slot)->crisis_tag;
			if(is_valid_index(ctag)) {
				auto ctag_holder = ws.w.culture_s.national_tags_state[ctag].holder;
				if(ctag_holder)
					apply_subeffects(tval, ws, ctag_holder, this_slot, from_slot, rebel_slot, gen);
			}
		}
		void es_crisis_state_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto cstate = ws.w.crisis_state;
			if(is_valid_index(cstate))
				apply_subeffects(tval, ws, &(ws.w.nation_s.states.get(cstate)), this_slot, from_slot, rebel_slot, gen);
		}
		void es_state_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto state = ((provinces::province_state*)primary_slot)->state_instance;
			if(state)
				apply_subeffects(tval, ws, state, this_slot, from_slot, rebel_slot, gen);
		}
		void es_state_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto pop_province = ((population::pop*)primary_slot)->location;
			if(is_valid_index(pop_province)) {
				auto prov_state = &(ws.w.province_s.province_state_container[pop_province]);
				es_state_scope_province(tval, ws, prov_state, this_slot, from_slot, rebel_slot, gen);
			}
		}
		void es_tag_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto tag = trigger_payload(tval[3]).tag;
				auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder;
				if(tag_holder && test_trigger(limit, ws, tag_holder, this_slot, from_slot, rebel_slot))
					apply_subeffects(tval, ws, tag_holder, this_slot, from_slot, rebel_slot, gen);
			} else {
				auto tag = trigger_payload(tval[2]).tag;
				auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder;
				if(tag_holder)
					apply_subeffects(tval, ws, tag_holder, this_slot, from_slot, rebel_slot, gen);
			}
		}
		void es_integer_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				provinces::province_tag ptag(tval[3]);
				provinces::province_state* pstate = &(ws.w.province_s.province_state_container[ptag]);
				if(test_trigger(limit, ws, pstate, this_slot, from_slot, rebel_slot))
					apply_subeffects(tval, ws, pstate, this_slot, from_slot, rebel_slot, gen);
			} else {
				provinces::province_tag ptag(tval[2]);
				provinces::province_state* pstate = &(ws.w.province_s.province_state_container[ptag]);
				apply_subeffects(tval, ws, pstate, this_slot, from_slot, rebel_slot, gen);
			}
		}
		void es_pop_type_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto type = trigger_payload(tval[3]).small.values.pop_type;

				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [limit, type, tval, &ws, this_slot, from_slot, rebel_slot, &gen](population::pop& p) {
					if(p.type == type && test_trigger(limit, ws, &p, this_slot, from_slot, rebel_slot))
						apply_subeffects(tval, ws, &p, this_slot, from_slot, rebel_slot, gen);
				});
			} else {
				auto type = trigger_payload(tval[2]).small.values.pop_type;

				nations::for_each_pop(ws, *((nations::nation*)primary_slot), [type, tval, &ws, this_slot, from_slot, rebel_slot, &gen](population::pop& p) {
					if(p.type == type)
						apply_subeffects(tval, ws, &p, this_slot, from_slot, rebel_slot, gen);
				});
			}
		}
		void es_pop_type_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto type = trigger_payload(tval[3]).small.values.pop_type;

				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [limit, type, tval, &ws, this_slot, from_slot, rebel_slot, &gen](population::pop& p) {
					if(p.type == type && test_trigger(limit, ws, &p, this_slot, from_slot, rebel_slot))
						apply_subeffects(tval, ws, &p, this_slot, from_slot, rebel_slot, gen);
				});
			} else {
				auto type = trigger_payload(tval[2]).small.values.pop_type;

				nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [type, tval, &ws, this_slot, from_slot, rebel_slot, &gen](population::pop& p) {
					if(p.type == type)
						apply_subeffects(tval, ws, &p, this_slot, from_slot, rebel_slot, gen);
				});
			}
		}
		void es_pop_type_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				auto type = trigger_payload(tval[3]).small.values.pop_type;

				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [limit, type, tval, &ws, this_slot, from_slot, rebel_slot, &gen](population::pop& p) {
					if(p.type == type && test_trigger(limit, ws, &p, this_slot, from_slot, rebel_slot))
						apply_subeffects(tval, ws, &p, this_slot, from_slot, rebel_slot, gen);
				});
			} else {
				auto type = trigger_payload(tval[2]).small.values.pop_type;

				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [type, tval, &ws, this_slot, from_slot, rebel_slot, &gen](population::pop& p) {
					if(p.type == type)
						apply_subeffects(tval, ws, &p, this_slot, from_slot, rebel_slot, gen);
				});
			}
		}
		void es_region_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto region = trigger_payload(tval[3]).state;
				auto provinces = ws.s.province_m.states_to_province_index.get_row(region);

				auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
				for(auto p : provinces) {
					auto& ps = ws.w.province_s.province_state_container[p];
					if(test_trigger(limit, ws, &ps, this_slot, from_slot, rebel_slot))
						apply_subeffects(tval, ws, &ps, this_slot, from_slot, rebel_slot, gen);
				}
			} else {
				auto region = trigger_payload(tval[2]).state;
				auto provinces = ws.s.province_m.states_to_province_index.get_row(region);

				for(auto p : provinces)
					apply_subeffects(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot, gen);
			}
		}

		static void(*effect_scope_functions[])(uint16_t const*, world_state&, void*, void*, void*, population::rebel_faction*, jsf_prng&) = {
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

		void ef_none(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_capital(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			provinces::province_tag new_capital(tval[2]);
			if(ws.w.province_s.province_state_container[new_capital].owner == primary_slot)
				((nations::nation*)primary_slot)->current_capital = new_capital;
		}
		void ef_add_core_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto tag = trigger_payload(tval[2]).tag;
			add_item(ws.w.province_s.core_arrays, ((provinces::province_state*)primary_slot)->cores, tag);
			add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, ((provinces::province_state*)primary_slot)->id);
		}
		void ef_add_core_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto tag = ((nations::nation*)primary_slot)->tag;
			provinces::province_tag prov(tval[2]);
			if(is_valid_index(tag)) {
				add_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[prov].cores, tag);
				add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, prov);
			}
		}
		void ef_add_core_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto tag = ((nations::nation*)this_slot)->tag;
			if(is_valid_index(tag)) {
				add_item(ws.w.province_s.core_arrays, ((provinces::province_state*)primary_slot)->cores, tag);
				add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, ((provinces::province_state*)primary_slot)->id);
			}
		}
		void ef_add_core_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		void ef_add_core_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = ((nations::state_instance*)this_slot)->owner;
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		void ef_add_core_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot));
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		void ef_add_core_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				ef_add_core_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		void ef_add_core_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			ef_add_core_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr, gen);
		}
		void ef_add_core_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			auto tag = rebel_slot->independence_tag;
			if(is_valid_index(tag)) {
				add_item(ws.w.province_s.core_arrays, ((provinces::province_state*)primary_slot)->cores, tag);
				add_item(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces, ((provinces::province_state*)primary_slot)->id);
			}
		}
		void ef_remove_core_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_core_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_region_name_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_region_name_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_trade_goods(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_accepted_culture(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_accepted_culture_union(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_primary_culture(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_primary_culture_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_primary_culture_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_primary_culture_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_primary_culture_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_primary_culture_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_accepted_culture(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_life_rating(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_religion(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_is_slave_state_yes(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_is_slave_pop_yes(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_research_points(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_tech_school(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_government(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_government_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_treasury(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_exhaustion(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_prestige(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_tag_culture(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_tag_no_core_switch(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_tag_no_core_switch_culture(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_set_country_flag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_clr_country_flag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_military_access(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_military_access_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_military_access_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_military_access_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_military_access_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_badboy(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_secede_province_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_inherit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_inherit_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_inherit_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_inherit_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_inherit_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_inherit_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_inherit_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_annex_to(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_annex_to_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_annex_to_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_annex_to_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_annex_to_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_annex_to_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_annex_to_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_controller(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_controller_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_controller_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_controller_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_controller_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_infrastructure(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_money(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_leadership(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_vassal(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_vassal_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_vassal_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_vassal_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_vassal_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_military_access(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_military_access_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_military_access_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_military_access_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_military_access_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_leave_alliance(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_leave_alliance_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_leave_alliance_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_leave_alliance_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_leave_alliance_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_war(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_war_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_war_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_war_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_end_war_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_enable_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_ruling_party_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_plurality(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_province_modifier(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_country_modifier(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_alliance(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_alliance_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_alliance_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_alliance_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_create_alliance_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_vassal(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_vassal_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_vassal_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_vassal_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_vassal_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_vassal_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_release_vassal_random(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_province_name(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_enable_canal(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_set_global_flag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_clr_global_flag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_nationalvalue_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_nationalvalue_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_civilized_yes(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_civilized_no(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_is_slave_state_no(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_is_slave_pop_no(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_election(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_social_reform(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_political_reform(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_tax_relative_income(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_neutrality(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_reduce_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_move_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_pop_type(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_years_of_research(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_prestige_factor_positive(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_prestige_factor_negative(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_military_reform(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_economic_reform(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_random_military_reforms(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_random_economic_reforms(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_crime(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_crime_none(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_nationalize(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_factory_in_capital_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_activate_technology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_great_wars_enabled_yes(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_great_wars_enabled_no(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_world_wars_enabled_yes(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_world_wars_enabled_no(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_assimilate_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_assimilate_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_literacy(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_crisis_interest(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_flashpoint_tension(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_crisis_temperature(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_consciousness(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_militancy(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_rgo_size(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_fort(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_naval_base(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_trigger_revolt_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_trigger_revolt_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_trigger_revolt_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_diplomatic_influence(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_diplomatic_influence_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_diplomatic_influence_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_diplomatic_influence_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_diplomatic_influence_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_province_modifier(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_province_modifier_no_duration(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_country_modifier(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_country_modifier_no_duration(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_casus_belli_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_casus_belli_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_remove_casus_belli_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_this_remove_casus_belli_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_no_ally_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_no_ally_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_no_ally_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_no_ally_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_no_ally_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_no_ally_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_war_no_ally_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_country_event(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_country_event_immediate(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_province_event(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_province_event_immediate(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_sub_unit_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_sub_unit_this(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_sub_unit_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_sub_unit_current(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_set_variable(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_change_variable(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_upper_house(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_define_general(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_define_admiral(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_dominant_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_war_goal(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_move_issue_percentage_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_move_issue_percentage_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_move_issue_percentage_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_move_issue_percentage_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_party_loyalty_nation_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_party_loyalty_province_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_party_loyalty_province_id_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_party_loyalty_province_id_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_railway_in_capital_yes_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_railway_in_capital_yes_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_railway_in_capital_no_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_railway_in_capital_no_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_fort_in_capital_yes_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_fort_in_capital_yes_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_fort_in_capital_no_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_build_fort_in_capital_no_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_variable_tech_name(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_variable_good_name(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_set_country_flag_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_country_modifier_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_add_country_modifier_province_no_duration(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_dominant_issue_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_province_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_province_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_province_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_province_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_relation_province_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_nation_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_nation_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_nation_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_nation_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_nation_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_nation_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_state_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_state_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_state_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_state_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_state_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_state_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_province_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_province_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_militancy_province_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_province_issue(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_province_ideology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_scaled_consciousness_province_unemployment(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_variable_good_name_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void ef_treasury_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}

		static void(*effect_functions[])(uint16_t const*, world_state&, void*, void*, void*, population::rebel_faction*, jsf_prng&) = {
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
			ef_country_event,
			ef_country_event_immediate,
			ef_province_event,
			ef_province_event_immediate,
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
		};
	}

#pragma clang diagnostic pop

	void execute_effect(
		uint16_t const* tval,
		world_state& ws,
		void* primary_slot,
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng generator_copy) {
		
		if((*tval & effect_codes::is_scope) != 0) {
			effect_scope_functions[*tval & effect_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot, generator_copy);
		} else {
			effect_functions[*tval & effect_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot, generator_copy);
		}
	}
}
