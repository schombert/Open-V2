#include "effects.h"
#include "codes.h"
#include <random>
#include "world_state\\world_state.h"

namespace triggers {
	void inner_execute_effect(
		uint16_t const* tval,
		world_state& ws,
		void* primary_slot,
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng& generator_copy);

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
			if((source[0] & effect_codes::code_mask) == effect_codes::random_list_scope) {
				const auto source_size = 1 + get_effect_payload_size(source);
				auto chances_total = source[2];

				std::uniform_int_distribution<int32_t> dist(0, int32_t(chances_total) - 1);
				auto chance_taken = dist(gen);

				auto sub_units_start = source + 3; // [code] + [payload size] + [chances total] + [first sub effect chance]
				while(sub_units_start < source + source_size) {
					if(chance_taken < *sub_units_start) {
						inner_execute_effect(sub_units_start + 1, ws, primary_slot, this_slot, from_slot, rebel_slot, gen);
						return;
					}
					chance_taken -= *sub_units_start;
					sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1); // each member preceeded by uint16_t
				}
			} else {
				const auto source_size = 1 + get_effect_payload_size(source);

				auto sub_units_start = source + 2 + effect_scope_data_payload(source[0]);
				while(sub_units_start < source + source_size) {
					inner_execute_effect(sub_units_start, ws, primary_slot, this_slot, from_slot, rebel_slot, gen);
					sub_units_start += 1 + get_effect_payload_size(sub_units_start);
				}
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
					apply_subeffects(tval, ws, rlist[dist(gen)], this_slot, from_slot, rebel_slot, gen);
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
					apply_subeffects(tval, ws, rlist[dist(gen)], this_slot, from_slot, rebel_slot, gen);
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
					ws.w.nation_s.nations.for_each([&rlist, &ws, primary_slot, this_slot, from_slot, rebel_slot](nations::nation& n) {
						if(&n != primary_slot && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0)
							rlist.push_back(&n);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					apply_subeffects(tval, ws, rlist[dist(gen)], this_slot, from_slot, rebel_slot, gen);
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
					apply_subeffects(tval, ws, rlist[dist(gen)], this_slot, from_slot, rebel_slot, gen);
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
		void es_x_greater_power_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_poor_strata_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_poor_strata_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_poor_strata_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_middle_strata_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_middle_strata_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_middle_strata_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_rich_strata_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_rich_strata_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_rich_strata_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_x_pop_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_x_pop_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_x_pop_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_x_owned_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_x_owned_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_x_core_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_x_state_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_random_list_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_random_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_owner_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_owner_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_controller_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_location_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_country_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_country_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_capital_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_this_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_this_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_this_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_this_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_from_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_from_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_from_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_from_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_sea_zone_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_cultural_union_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_overlord_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_sphere_owner_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_independence_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_flashpoint_tag_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_crisis_state_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_state_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_state_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_tag_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_integer_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_pop_type_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_pop_type_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_pop_type_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}
		void es_region_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {}

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

		void ef_none(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			apply_subeffects(tval, ws, primary_slot, this_slot, from_slot, rebel_slot, gen);
		}

		static void(*effect_functions[])(uint16_t const*, world_state&, void*, void*, void*, population::rebel_faction*, jsf_prng&) = {
			ef_none
		};
	}

#pragma clang diagnostic pop

	void inner_execute_effect(
		uint16_t const* tval,
		world_state& ws,
		void* primary_slot, 
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng& generator_copy) {

		if((*tval & effect_codes::is_scope) != 0) {
			effect_scope_functions[*tval & effect_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot, generator_copy);
		} else {
			effect_functions[*tval & effect_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot, generator_copy);
		}
	}

	void execute_effect(
		uint16_t const* tval,
		world_state& ws,
		void* primary_slot,
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng generator_copy) {
		inner_execute_effect(tval, ws, primary_slot, this_slot, from_slot, rebel_slot, generator_copy);
	}
}
