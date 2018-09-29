#include "triggers.h"
#include "codes.h"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.h"
#include "military\\military_functions.h"
#include "population\\population_function.h"
#include "ideologies\\ideologies_functions.h"
#include "issues\\issues_functions.hpp"
#include "provinces\\province_functions.h"
#include "modifiers\\modifier_functions.h"

namespace triggers {
	int32_t get_trigger_payload_size(const uint16_t* data) {
		const auto payload_mask = uint16_t(-int16_t((trigger_codes::no_payload & data[0]) == 0));
		return payload_mask & data[1];
	}
	int32_t trigger_scope_data_payload(uint16_t code) {
		const auto masked_code = code & trigger_codes::code_mask;
		if ((masked_code == trigger_codes::x_provinces_in_variable_region) |
			(masked_code == trigger_codes::tag_scope) |
			(masked_code == trigger_codes::integer_scope))
			return 1;
		return 0;
	}
	void add_float_to_payload(std::vector<uint16_t>& v, float f) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			float f;
		} pack_float;
		pack_float.f = f;

		v.push_back(pack_float.v.low);
		v.push_back(pack_float.v.high);
	}
	float read_float_from_payload(const uint16_t* data) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			float f;
		} pack_float;

		pack_float.v.low = data[0];
		pack_float.v.high = data[1];

		return pack_float.f;
	}

	void add_int32_t_to_payload(std::vector<uint16_t>& v, int32_t i) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			int32_t i;
		} pack_int;
		pack_int.i = i;

		v.push_back(pack_int.v.low);
		v.push_back(pack_int.v.high);
	}
	int32_t read_int32_t_from_payload(const uint16_t* data) {
		union {
			struct {
				uint16_t low;
				uint16_t high;
			} v;
			int32_t i;
		} pack_int;
		
		pack_int.v.low = data[0];
		pack_int.v.high = data[1];

		return pack_int.i;
	}

	namespace {
#define TRIGGER_PARAMTERS uint16_t const* tval, world_state const& ws, void const* primary_slot, void const* this_slot, void const* from_slot, population::rebel_faction const* rebel_slot

	inline bool apply_disjuctively(TRIGGER_PARAMTERS) {
		const auto source_size = 1 + get_trigger_payload_size(tval);
		auto sub_units_start = tval + 2 + trigger_scope_data_payload(tval[0]);
		while(sub_units_start < tval + source_size) {
			if(test_trigger(sub_units_start, ws, primary_slot, this_slot, from_slot, rebel_slot))
				return true;
			sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
		}
		return false;
	}

	inline bool apply_conjuctively(TRIGGER_PARAMTERS) {
		const auto source_size = 1 + get_trigger_payload_size(tval);
		auto sub_units_start = tval + 2 + trigger_scope_data_payload(tval[0]);
		while(sub_units_start < tval + source_size) {
			if(!test_trigger(sub_units_start, ws, primary_slot, this_slot, from_slot, rebel_slot))
				return false;
			sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
		}
		return true;
	}

	inline bool apply_subtriggers(TRIGGER_PARAMTERS) {
		if((*tval & trigger_codes::is_disjunctive_scope) != 0)
			return apply_disjuctively(tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
		else
			return apply_conjuctively(tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
	}

#ifdef __llvm__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

	bool tf_none(TRIGGER_PARAMTERS) {
		return true;
	}

	bool tf_generic_scope(TRIGGER_PARAMTERS) {
		return apply_subtriggers(tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_x_neighbor_province_scope(TRIGGER_PARAMTERS) {
		auto prov_id = ((provinces::province_state const*)primary_slot)->id;

		auto p_same_range = ws.s.province_m.same_type_adjacency.get_row(prov_id);
		auto p_diff_range = ws.s.province_m.coastal_adjacency.get_row(prov_id);

		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p = p_same_range.first; p != p_same_range.second; ++p) {
				if(apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*p]), this_slot, from_slot, rebel_slot))
					return true;
			}
			for(auto p = p_diff_range.first; p != p_diff_range.second; ++p) {
				if(apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*p]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p = p_same_range.first; p != p_same_range.second; ++p) {
				if(!apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*p]), this_slot, from_slot, rebel_slot))
					return false;
			}
			for(auto p = p_diff_range.first; p != p_diff_range.second; ++p) {
				if(!apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*p]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_neighbor_country_scope_nation(TRIGGER_PARAMTERS) { 
		auto n_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->neighboring_nations);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto n : n_range) {
				if(is_valid_index(n) && apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto n : n_range) {
				if(is_valid_index(n) && !apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_neighbor_country_scope_pop(TRIGGER_PARAMTERS) { 
		auto pop_province = ((population::pop const*)primary_slot)->location;
		auto province_owner = ws.w.province_s.province_state_container[pop_province].owner;
		if(province_owner)
			return tf_x_neighbor_country_scope_nation(tval, ws, province_owner, this_slot, from_slot, rebel_slot);
		else
			return 0 == (*tval & trigger_codes::is_existance_scope);
	}
	bool tf_x_war_countries_scope_nation(TRIGGER_PARAMTERS) {
		auto opposing_countries = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->opponents_in_war);

		if(*tval & trigger_codes::is_existance_scope) {
			for(auto n : opposing_countries) {
				if(is_valid_index(n) && apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto n : opposing_countries) {
				if(is_valid_index(n) && !apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_war_countries_scope_pop(TRIGGER_PARAMTERS) {
		auto pop_province = ((population::pop const*)primary_slot)->location;
		auto province_owner = ws.w.province_s.province_state_container[pop_province].owner;
		if(province_owner)
			return tf_x_war_countries_scope_nation(tval, ws, province_owner, this_slot, from_slot, rebel_slot);
		else
			return 0 == (*tval & trigger_codes::is_existance_scope);
	}
	bool tf_x_greater_power_scope(TRIGGER_PARAMTERS) {
		auto ranked_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		int32_t great_nations_count = int32_t(ws.s.modifiers_m.global_defines.great_nations_count);

		if(*tval & trigger_codes::is_existance_scope) {
			int32_t count = 0;
			for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
				if(is_valid_index(*n)) {
					auto& nation = ws.w.nation_s.nations[*n];
					if(nations::is_great_power(ws, nation)) {
						++count;
						if(apply_subtriggers(tval, ws, &nation, this_slot, from_slot, rebel_slot))
							return true;
					}
				}
			}
			return false;
		} else {
			int32_t count = 0;
			for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
				if(is_valid_index(*n)) {
					auto& nation = ws.w.nation_s.nations[*n];
					if(nations::is_great_power(ws, nation)) {
						++count;
						if(!apply_subtriggers(tval, ws, &nation, this_slot, from_slot, rebel_slot))
							return false;
					}
				}
			}
			return true;
		}
	}
	bool tf_x_owned_province_scope_state(TRIGGER_PARAMTERS) {
		auto region_id = ((nations::state_instance const*)primary_slot)->region_id;
		if(!is_valid_index(region_id))
			return false;

		auto region_provinces = ws.s.province_m.states_to_province_index.get_row(region_id);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : region_provinces) {
				auto& prov = ws.w.province_s.province_state_container[p];
				if(prov.state_instance == primary_slot && apply_subtriggers(tval, ws, &prov, this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p : region_provinces) {
				auto& prov = ws.w.province_s.province_state_container[p];
				if(prov.state_instance == primary_slot && !apply_subtriggers(tval, ws, &prov, this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_owned_province_scope_nation(TRIGGER_PARAMTERS) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : owned_range) {
				if(is_valid_index(p) && apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p : owned_range) {
				if(is_valid_index(p) && !apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_core_scope_province(TRIGGER_PARAMTERS) {
		auto core_range = get_range(ws.w.province_s.core_arrays, ((provinces::province_state const*)primary_slot)->cores);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : core_range) {
				if(is_valid_index(p)) {
					auto core_holder = ws.w.culture_s.national_tags_state[p].holder;
					if(core_holder && apply_subtriggers(tval, ws, core_holder, this_slot, from_slot, rebel_slot))
						return true;
				}
			}
			return false;
		} else {
			for(auto p : core_range) {
				if(is_valid_index(p)) {
					auto core_holder = ws.w.culture_s.national_tags_state[p].holder;
					if(core_holder && !apply_subtriggers(tval, ws, core_holder, this_slot, from_slot, rebel_slot))
						return false;
				}
			}
			return true;
		}
	}
	bool tf_x_core_scope_nation(TRIGGER_PARAMTERS) {
		auto tag = ((nations::nation const*)primary_slot)->tag;
		if(!is_valid_index(tag))
			return false;

		auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : core_range) {
				if(is_valid_index(p) && apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p : core_range) {
				if(is_valid_index(p) && !apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_state_scope(TRIGGER_PARAMTERS) {
		auto states = get_range(ws.w.nation_s.state_arrays, ((nations::nation const*)primary_slot)->member_states);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto s = states.first; s != states.second; ++s) {
				if(s->state && apply_subtriggers(tval, ws, s->state, this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto s = states.first; s != states.second; ++s) {
				if(s->state && !apply_subtriggers(tval, ws, s->state, this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_substate_scope(TRIGGER_PARAMTERS) {
		auto nation_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->vassals);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				if(is_valid_index(*i)) {
					auto& n = ws.w.nation_s.nations.get(*i);
					if((n.flags & nations::nation::is_substate) != 0 && apply_subtriggers(tval, ws, &n, this_slot, from_slot, rebel_slot))
						return true;
				}
			}
			return false;
		} else {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				if(is_valid_index(*i)) {
					auto& n = ws.w.nation_s.nations.get(*i);
					if((n.flags & nations::nation::is_substate) != 0 && !apply_subtriggers(tval, ws, &n, this_slot, from_slot, rebel_slot))
						return false;
				}
			}
			return true;
		}
	}
	bool tf_x_sphere_member_scope(TRIGGER_PARAMTERS) {
		auto nation_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->sphere_members);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				if(is_valid_index(*i) && apply_subtriggers(tval, ws, &(ws.w.nation_s.nations.get(*i)), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				if(is_valid_index(*i) && !apply_subtriggers(tval, ws, &(ws.w.nation_s.nations.get(*i)), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_pop_scope_province(TRIGGER_PARAMTERS) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, ((provinces::province_state const*)primary_slot)->pops);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = pop_range.first; i != pop_range.second; ++i) {
				if(is_valid_index(*i) && apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto i = pop_range.first; i != pop_range.second; ++i) {
				if(is_valid_index(*i) && !apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_pop_scope_state(TRIGGER_PARAMTERS) {
		auto region_id = ((nations::state_instance const*)primary_slot)->region_id;
		if(!is_valid_index(region_id))
			return false;
		auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				auto& pstate = ws.w.province_s.province_state_container[*j];
				if(pstate.state_instance == primary_slot) {
					auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*j].pops);
					for(auto i = pop_range.first; i != pop_range.second; ++i) {
						if(is_valid_index(*i) && apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
							return true;
					}
				}
			}
			return false;
		} else {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				auto& pstate = ws.w.province_s.province_state_container[*j];
				if(pstate.state_instance == primary_slot) {
					auto pop_range = get_range(ws.w.population_s.pop_arrays, pstate.pops);
					for(auto i = pop_range.first; i != pop_range.second; ++i) {
						if(is_valid_index(*i) && !apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
							return false;
					}
				}
			}
			return true;
		}
	}
	bool tf_x_pop_scope_nation(TRIGGER_PARAMTERS) {
		auto province_range = get_range(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				if(is_valid_index(*j)) {
					auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*j].pops);
					for(auto i = pop_range.first; i != pop_range.second; ++i) {
						if(is_valid_index(*i) && apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
							return true;
					}
				}
			}
			return false;
		} else {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				if(is_valid_index(*j)) {
					auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*j].pops);
					for(auto i = pop_range.first; i != pop_range.second; ++i) {
						if(is_valid_index(*i) && !apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
							return false;
					}
				}
			}
			return true;
		}
	}
	bool tf_x_provinces_in_variable_region(TRIGGER_PARAMTERS) {
		auto region = trigger_payload(*(tval + 2)).state;
		auto provinces = ws.s.province_m.states_to_province_index.get_row(region);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = provinces.first; i != provinces.second; ++i) {
				if(apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*i]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto i = provinces.first; i != provinces.second; ++i) {
				if(!apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*i]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_owner_scope_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_owner_scope_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_controller_scope(TRIGGER_PARAMTERS) {
		auto controller = ((provinces::province_state const*)primary_slot)->controller;
		if(controller)
			return apply_subtriggers(tval, ws, controller, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_location_scope(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location))
			return apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[location]), this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_country_scope_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_country_scope_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(!is_valid_index(location))
			return false;
		auto owner = ws.w.province_s.province_state_container[location].owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_capital_scope(TRIGGER_PARAMTERS) {
		auto capital = ((nations::nation const*)primary_slot)->current_capital;
		if(is_valid_index(capital))
			return apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[capital]), this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_this_scope(TRIGGER_PARAMTERS) {
		return apply_subtriggers(tval, ws, this_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_from_scope(TRIGGER_PARAMTERS) {
		return apply_subtriggers(tval, ws, from_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_sea_zone_scope(TRIGGER_PARAMTERS) {
		auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(((provinces::province_state const*)primary_slot)->id);
		if(sea_zones.first != sea_zones.second)
			return apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*sea_zones.first]), this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_cultural_union_scope(TRIGGER_PARAMTERS) {
		auto prim_culture = ((nations::nation const*)primary_slot)->primary_culture;
		if(!is_valid_index(prim_culture))
			return false;
		auto culture_group = ws.s.culture_m.culture_container[prim_culture].group;
		if(is_valid_index(culture_group)) {
			auto union_tag = ws.s.culture_m.culture_groups[culture_group].union_tag;
			if(is_valid_index(union_tag)) {
				auto union_holder = ws.w.culture_s.national_tags_state[union_tag].holder;
				if(union_holder)
					return apply_subtriggers(tval, ws, union_holder, this_slot, from_slot, rebel_slot);
			}
		}
		return false;
	}
	bool tf_overlord_scope(TRIGGER_PARAMTERS) {
		auto so = ((nations::nation const*)primary_slot)->overlord;
		if(so)
			return apply_subtriggers(tval, ws, so, this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_sphere_owner_scope(TRIGGER_PARAMTERS) {
		auto so = ((nations::nation const*)primary_slot)->sphere_leader;
		if(so)
			return apply_subtriggers(tval, ws, so, this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_independence_scope(TRIGGER_PARAMTERS) {
		auto rtag = rebel_slot->independence_tag;
		if(is_valid_index(rtag)) {
			auto ination = ws.w.culture_s.national_tags_state[rtag].holder;
			if(ination)
				return apply_subtriggers(tval, ws, ination, this_slot, from_slot, rebel_slot);
		}
		return false;
	}
	bool tf_flashpoint_tag_scope(TRIGGER_PARAMTERS) {
		auto ctag = ((nations::state_instance const*)primary_slot)->crisis_tag;
		if(is_valid_index(ctag)) {
			auto ctag_holder = ws.w.culture_s.national_tags_state[ctag].holder;
			if(ctag_holder)
				return apply_subtriggers(tval, ws, ctag_holder, this_slot, from_slot, rebel_slot);
		}
		return false;
	}
	bool tf_crisis_state_scope(TRIGGER_PARAMTERS) {
		auto cstate = ws.w.current_crisis.state;
		if(is_valid_index(cstate))
			return apply_subtriggers(tval, ws, &(ws.w.nation_s.states.get(cstate)), this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_state_scope_province(TRIGGER_PARAMTERS) {
		auto state_instance = ((provinces::province_state const*)primary_slot)->state_instance;
		if(state_instance)
			return apply_subtriggers(tval, ws, state_instance, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_state_scope_pop(TRIGGER_PARAMTERS) {
		auto pop_province = ((population::pop const*)primary_slot)->location;
		if(!is_valid_index(pop_province))
			return false;
		auto prov_state = ws.w.province_s.province_state_container[pop_province].state_instance;
		if(prov_state)
			return tf_state_scope_province(tval, ws, prov_state, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_tag_scope(TRIGGER_PARAMTERS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder;
		if(tag_holder)
			return apply_subtriggers(tval, ws, tag_holder, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_integer_scope(TRIGGER_PARAMTERS) {
		provinces::province_tag ptag(tval[2]);
		provinces::province_state const* pstate = &(ws.w.province_s.province_state_container[ptag]);
		return apply_subtriggers(tval, ws, pstate, this_slot, from_slot, rebel_slot);
	}
	bool tf_country_scope_nation(TRIGGER_PARAMTERS) {
		return apply_subtriggers(tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_country_scope_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_cultural_union_scope_pop(TRIGGER_PARAMTERS) {
		auto prim_culture = ((population::pop const*)primary_slot)->culture;
		if(!is_valid_index(prim_culture))
			return false;
		auto culture_group = ws.s.culture_m.culture_container[prim_culture].group;
		if(is_valid_index(culture_group)) {
			auto union_tag = ws.s.culture_m.culture_groups[culture_group].union_tag;
			if(is_valid_index(union_tag)) {
				auto union_holder = ws.w.culture_s.national_tags_state[union_tag].holder;
				if(union_holder)
					return apply_subtriggers(tval, ws, union_holder, this_slot, from_slot, rebel_slot);
			}
		}
		return false;
	}



	static bool(*scope_functions[])(TRIGGER_PARAMTERS) = {
		tf_generic_scope, //constexpr uint16_t generic_scope = 0x0000; // or & and
		tf_x_neighbor_province_scope, //constexpr uint16_t x_neighbor_province_scope = 0x0001;
		tf_x_neighbor_country_scope_nation, //constexpr uint16_t x_neighbor_country_scope_nation = 0x0002;
		tf_x_neighbor_country_scope_pop, //constexpr uint16_t x_neighbor_country_scope_pop = 0x0003;
		tf_x_war_countries_scope_nation, //constexpr uint16_t x_war_countries_scope_nation = 0x0004;
		tf_x_war_countries_scope_pop, //constexpr uint16_t x_war_countries_scope_pop = 0x0005;
		tf_x_greater_power_scope, //constexpr uint16_t x_greater_power_scope = 0x0006;
		tf_x_owned_province_scope_state, //constexpr uint16_t x_owned_province_scope_state = 0x0007;
		tf_x_owned_province_scope_nation, //constexpr uint16_t x_owned_province_scope_nation = 0x0008;
		tf_x_core_scope_province, //constexpr uint16_t x_core_scope_province = 0x0009;
		tf_x_core_scope_nation, //constexpr uint16_t x_core_scope_nation = 0x000A;
		tf_x_state_scope, //constexpr uint16_t x_state_scope = 0x000B;
		tf_x_substate_scope, //constexpr uint16_t x_substate_scope = 0x000C;
		tf_x_sphere_member_scope, //constexpr uint16_t x_sphere_member_scope = 0x000D;
		tf_x_pop_scope_province, //constexpr uint16_t x_pop_scope_province = 0x000E;
		tf_x_pop_scope_state, //constexpr uint16_t x_pop_scope_state = 0x000F;
		tf_x_pop_scope_nation, //constexpr uint16_t x_pop_scope_nation = 0x0010;
		tf_x_provinces_in_variable_region, //constexpr uint16_t x_provinces_in_variable_region = 0x0011; // variable name
		tf_owner_scope_state, //constexpr uint16_t owner_scope_state = 0x0012;
		tf_owner_scope_province, //constexpr uint16_t owner_scope_province = 0x0013;
		tf_controller_scope, //constexpr uint16_t controller_scope = 0x0014;
		tf_location_scope, //constexpr uint16_t location_scope = 0x0015;
		tf_country_scope_state, //constexpr uint16_t country_scope_state = 0x0016;
		tf_country_scope_pop, //constexpr uint16_t country_scope_pop = 0x0017;
		tf_capital_scope, //constexpr uint16_t capital_scope = 0x0018;
		tf_this_scope, //constexpr uint16_t this_scope_pop = 0x0019;
		tf_this_scope, //constexpr uint16_t this_scope_nation = 0x001A;
		tf_this_scope, //constexpr uint16_t this_scope_state = 0x001B;
		tf_this_scope, //constexpr uint16_t this_scope_province = 0x001C;
		tf_from_scope, //constexpr uint16_t from_scope_pop = 0x001D;
		tf_from_scope, //constexpr uint16_t from_scope_nation = 0x001E;
		tf_from_scope, //constexpr uint16_t from_scope_state = 0x001F;
		tf_from_scope, //constexpr uint16_t from_scope_province = 0x0020;
		tf_sea_zone_scope, //constexpr uint16_t sea_zone_scope = 0x0021;
		tf_cultural_union_scope, //constexpr uint16_t cultural_union_scope = 0x0022;
		tf_overlord_scope, //constexpr uint16_t overlord_scope = 0x0023;
		tf_sphere_owner_scope, //constexpr uint16_t sphere_owner_scope = 0x0024;
		tf_independence_scope, //constexpr uint16_t independence_scope = 0x0025;
		tf_flashpoint_tag_scope, //constexpr uint16_t flashpoint_tag_scope = 0x0026;
		tf_crisis_state_scope, //constexpr uint16_t crisis_state_scope = 0x0027;
		tf_state_scope_pop, //constexpr uint16_t state_scope_pop = 0x0028;
		tf_state_scope_province, //constexpr uint16_t state_scope_province = 0x0029;
		tf_tag_scope, //constexpr uint16_t tag_scope = 0x002A; // variable name
		tf_integer_scope, //constexpr uint16_t integer_scope = 0x002B; // variable name
		tf_country_scope_nation, //constexpr uint16_t country_scope_nation = 0x002C;
		tf_country_scope_province, //constexpr uint16_t country_scope_province = 0x002D;
		tf_cultural_union_scope_pop, //constexpr uint16_t cultural_union_scope_pop = 0x002E;
	};

	template<typename T>
	[[nodiscard]] bool compare_values(uint16_t trigger_code, T value_a, T value_b) {
		switch(trigger_code & trigger_codes::association_mask) {
			case trigger_codes::association_eq:
				return value_a == value_b;
			case trigger_codes::association_gt:
				return value_a > value_b;
			case trigger_codes::association_lt:
				return value_a < value_b;
			case trigger_codes::association_le:
				return value_a <= value_b;
			case trigger_codes::association_ne:
				return value_a != value_b;
			case trigger_codes::association_ge:
				return value_a >= value_b;
			default:
				return value_a >= value_b;
		}
	}

	bool tf_year(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], int32_t(tag_to_date(ws.w.current_date).year()), int32_t(tval[2]));
	}
	bool tf_month(TRIGGER_PARAMTERS) { 
		return compare_values(tval[0], int32_t(tag_to_date(ws.w.current_date).month()), int32_t(tval[2]));
	}
	bool tf_port(TRIGGER_PARAMTERS) {
		auto prov_id = ((provinces::province_state const*)primary_slot)->id;
		auto is_port = (ws.s.province_m.province_container[prov_id].flags & (provinces::province::sea | provinces::province::coastal)) == provinces::province::coastal;
		return compare_values(tval[0], is_port, true);
	}
	bool tf_rank(TRIGGER_PARAMTERS) {
		// note: comparison revesed since rank 1 is "greater" than rank 1 + N
		return compare_values(tval[0], int32_t(tval[2]), int32_t(((nations::nation const*)primary_slot)->overall_rank));
	}
	bool tf_technology(TRIGGER_PARAMTERS) {
		auto technology = trigger_payload(tval[2]).tech;
		auto nation_id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto tech_row = ws.w.nation_s.active_technologies.get_row(nation_id);
			auto has_tech = bit_vector_test(tech_row, to_index(technology));
			return compare_values(tval[0], has_tech, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_strata_rich(TRIGGER_PARAMTERS) {
		auto pop_type = ((population::pop const*)primary_slot)->type;
		if(is_valid_index(pop_type))
			return compare_values(tval[0], (ws.s.population_m.pop_types[pop_type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_strata_middle(TRIGGER_PARAMTERS) {
		auto pop_type = ((population::pop const*)primary_slot)->type;
		if(is_valid_index(pop_type))
			return compare_values(tval[0], (ws.s.population_m.pop_types[pop_type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_strata_poor(TRIGGER_PARAMTERS) {
		auto pop_type = ((population::pop const*)primary_slot)->type;
		if(is_valid_index(pop_type))
			return compare_values(tval[0], (ws.s.population_m.pop_types[pop_type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_life_rating_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], provinces::get_life_rating(*((provinces::province_state const*)primary_slot)), float(trigger_payload(tval[2]).signed_value));
	}
	bool tf_life_rating_state(TRIGGER_PARAMTERS) {
		auto state = (nations::state_instance const*)primary_slot;
		auto region_id = state->region_id;
		if(!is_valid_index(region_id))
			return compare_values(tval[0], 0, int32_t(trigger_payload(tval[2]).signed_value));

		auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);
		int32_t min_life_rating = std::numeric_limits<int32_t>::max();
		for(auto p : province_range) {
			auto& ps = ws.w.province_s.province_state_container[p];
			if(ps.owner == state->owner)
				min_life_rating = std::min(min_life_rating, int32_t(provinces::get_life_rating(ps)));
		}
		return compare_values(tval[0], min_life_rating, int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_has_empty_adjacent_state_province(TRIGGER_PARAMTERS) {
		auto adj_range = ws.s.province_m.same_type_adjacency.get_row(((provinces::province_state const*)primary_slot)->id);
		for(auto p : adj_range) {
			if(ws.w.province_s.province_state_container[p].owner == nullptr) 
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_empty_adjacent_state_state(TRIGGER_PARAMTERS) {
		auto state = (nations::state_instance const*)primary_slot;
		auto region_id = state->region_id;
		if(!is_valid_index(region_id))
			return compare_values(tval[0], false, true);

		auto province_range = ws.s.province_m.states_to_province_index.get_row(region_id);
		auto state_owner = state->owner;

		for(auto p : province_range) {
			auto& ps = ws.w.province_s.province_state_container[p];

			if(ps.owner == state_owner) {
				auto adj_range = ws.s.province_m.same_type_adjacency.get_row(p);
				for(auto q : adj_range) {
					auto& qs = ws.w.province_s.province_state_container[p];
					if((qs.owner == nullptr) & (qs.state_instance != state))
						return compare_values(tval[0], true, true);
					
				}
			}
		}

		return compare_values(tval[0], false, true);
	}
	bool tf_state_id_province(TRIGGER_PARAMTERS) {
		provinces::province_tag pid(tval[2]);
		auto current_prov = ((provinces::province_state const*)primary_slot)->id;
		auto same_region = ws.s.province_m.province_container[current_prov].state_id == ws.s.province_m.province_container[pid].state_id;
		return compare_values(tval[0], same_region, true);
	}
	bool tf_state_id_state(TRIGGER_PARAMTERS) {
		provinces::province_tag pid(tval[2]);
		auto current_region = ((nations::state_instance const*)primary_slot)->region_id;
		auto same_region = current_region == ws.s.province_m.province_container[pid].state_id;
		return compare_values(tval[0], same_region, true);
	}
	bool tf_cash_reserves(TRIGGER_PARAMTERS) {
		auto ratio = economy::money_qnty_type(read_float_from_payload(tval + 2));
		auto target = population::desired_needs_spending(ws, *((population::pop const*)primary_slot));
		auto money = economy::money_qnty_type(((population::pop const*)primary_slot)->money);
		return compare_values(tval[0], target != economy::money_qnty_type(0) ? money / target : economy::money_qnty_type(100), ratio);
	}
	bool tf_unemployment_nation(TRIGGER_PARAMTERS) {
		auto nation_id = ((nations::nation const*)primary_slot)->id;
		if(!ws.w.nation_s.nations.is_valid_index(nation_id))
			return compare_values(tval[0], false, true);
		float unemployed_fraction = 1.0f - float(ws.w.nation_s.nation_demographics.get(nation_id, population::total_employment_tag)) / float(ws.w.nation_s.nation_demographics.get(nation_id, population::total_population_tag));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_state(TRIGGER_PARAMTERS) {
		auto state_id = ((nations::state_instance const*)primary_slot)->id;
		if(!ws.w.nation_s.states.is_valid_index(state_id))
			return compare_values(tval[0], false, true);
		float unemployed_fraction = 1.0f - float(ws.w.nation_s.state_demographics.get(state_id, population::total_employment_tag)) / float(ws.w.nation_s.state_demographics.get(state_id, population::total_population_tag));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_province(TRIGGER_PARAMTERS) {
		auto prov_id = ((provinces::province_state const*)primary_slot)->id;
		float unemployed_fraction = 1.0f - float(ws.w.province_s.province_demographics.get(prov_id, population::total_employment_tag)) / float(ws.w.province_s.province_demographics.get(prov_id, population::total_population_tag));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_pop(TRIGGER_PARAMTERS) {
		auto pop_id = ((population::pop const*)primary_slot)->id;
		if(!ws.w.population_s.pops.is_valid_index(pop_id))
			return compare_values(tval[0], false, true);
		float unemployed_fraction = 1.0f - float(ws.w.population_s.pop_demographics.get(pop_id, population::total_employment_tag)) / float(ws.w.population_s.pop_demographics.get(pop_id, population::total_population_tag));
		return compare_values(tval[0], unemployed_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_is_slave_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->current_rules.rules_value & issues::rules::slavery_allowed), true);
	}
	bool tf_is_slave_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::state_instance const*)primary_slot)->flags & nations::state_instance::is_slave_state), true);
	}
	bool tf_is_slave_province(TRIGGER_PARAMTERS) {
		auto state = ((provinces::province_state const*)primary_slot)->state_instance;
		if(state)
			return tf_is_slave_state(tval, ws, state, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_slave_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->type == ws.s.population_m.slave, true);
	}
	bool tf_is_independant(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord == nullptr, true);
	}
	bool tf_has_national_minority_province(TRIGGER_PARAMTERS) {
		auto powner = ((provinces::province_state const*)primary_slot)->owner;
		auto pid = ((provinces::province_state const*)primary_slot)->id;

		if(powner) {
			auto prim_culture = powner->primary_culture;
			if(is_valid_index(prim_culture)) {
				return compare_values(tval[0],
					ws.w.province_s.province_demographics.get(pid, population::to_demo_tag(ws, prim_culture)) ==
						ws.w.province_s.province_demographics.get(pid, population::total_population_tag),
					true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_national_minority_state(TRIGGER_PARAMTERS) {
		auto sowner = ((nations::state_instance const*)primary_slot)->owner;
		auto sid = ((nations::state_instance const*)primary_slot)->id;

		if(bool(sowner) & ws.w.nation_s.states.is_valid_index(sid)) {
			auto prim_culture = sowner->primary_culture;
			if(is_valid_index(prim_culture)) {
				return compare_values(tval[0],
					ws.w.nation_s.state_demographics.get(sid, population::to_demo_tag(ws, prim_culture)) ==
					ws.w.nation_s.state_demographics.get(sid, population::total_population_tag),
					true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_national_minority_nation(TRIGGER_PARAMTERS) { 
		auto nid = ((nations::nation const*)primary_slot)->id;

		if(ws.w.nation_s.nations.is_valid_index(nid)) {
			auto prim_culture = ((nations::nation const*)primary_slot)->primary_culture;
			if(is_valid_index(prim_culture)) {
				return compare_values(tval[0],
					ws.w.nation_s.nation_demographics.get(nid, population::to_demo_tag(ws, prim_culture)) ==
					ws.w.nation_s.nation_demographics.get(nid, population::total_population_tag),
					true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_government_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->current_government == trigger_payload(tval[2]).small.values.government, true);
	}
	bool tf_government_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto owner = ws.w.province_s.province_state_container[location].owner;
			if(owner) {
				return tf_government_nation(tval, ws, owner, nullptr, nullptr, nullptr);
			}
		}
		return false;
	}
	bool tf_capital(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->current_capital == provinces::province_tag(tval[2]), true);
	}
	bool tf_tech_school(TRIGGER_PARAMTERS) { 
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->tech_school == trigger_payload(tval[2]).nat_mod, true);
	}
	bool tf_primary_culture(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->primary_culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_accepted_culture(TRIGGER_PARAMTERS) { 
		auto has_culture = contains_item(ws.w.culture_s.culture_arrays, ((nations::nation const*)primary_slot)->accepted_cultures, trigger_payload(tval[2]).culture);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_culture_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->dominant_culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_culture_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->dominant_culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_culture_nation(TRIGGER_PARAMTERS) {
		auto has_culture = ((nations::nation const*)primary_slot)->primary_culture == trigger_payload(tval[2]).culture || contains_item(ws.w.culture_s.culture_arrays, ((nations::nation const*)primary_slot)->accepted_cultures, trigger_payload(tval[2]).culture);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_pop_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->culture == rebel_slot->culture, true);
	}
	bool tf_culture_state_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->dominant_culture == rebel_slot->culture, true);
	}
	bool tf_culture_province_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->dominant_culture == rebel_slot->culture, true);
	}
	bool tf_culture_nation_reb(TRIGGER_PARAMTERS) {
		auto has_culture = ((nations::nation const*)primary_slot)->primary_culture == rebel_slot->culture || contains_item(ws.w.culture_s.culture_arrays, ((nations::nation const*)primary_slot)->accepted_cultures, rebel_slot->culture);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_this_nation(TRIGGER_PARAMTERS) {
		auto has_culture = ((nations::nation const*)this_slot)->primary_culture == ((population::pop const*)primary_slot)->culture || contains_item(ws.w.culture_s.culture_arrays, ((nations::nation const*)this_slot)->accepted_cultures, ((population::pop const*)primary_slot)->culture);
		return compare_values(tval[0], has_culture, true);
	}
	bool tf_culture_from_nation(TRIGGER_PARAMTERS) {
		return tf_culture_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr);
	}
	bool tf_culture_this_state(TRIGGER_PARAMTERS) {
		auto state_owner = ((nations::state_instance const*)this_slot)->owner;
		if(state_owner)
			return tf_culture_this_nation(tval, ws, primary_slot, state_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_this_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)this_slot)->location;
		if(is_valid_index(location)) {
			auto prov_owner = ws.w.province_s.province_state_container[location].owner;
			if(prov_owner)
				return tf_culture_this_nation(tval, ws, primary_slot, prov_owner, nullptr, nullptr);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_this_province(TRIGGER_PARAMTERS) {
		auto prov_owner = ((provinces::province_state const*)this_slot)->owner;
		if(prov_owner)
			return tf_culture_this_nation(tval, ws, primary_slot, prov_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation(TRIGGER_PARAMTERS) {
		auto cg = trigger_payload(tval[2]).culture_group;
		auto nation_pculture = ((nations::nation const*)primary_slot)->primary_culture;
		if(is_valid_index(nation_pculture))
			return compare_values(tval[0], ws.s.culture_m.culture_container[nation_pculture].group == cg, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop(TRIGGER_PARAMTERS) {
		auto cg = trigger_payload(tval[2]).culture_group;
		auto pculture = ((population::pop const*)primary_slot)->culture;
		if(is_valid_index(pculture))
			return compare_values(tval[0], ws.s.culture_m.culture_container[pculture].group == cg, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_reb_nation(TRIGGER_PARAMTERS) { 
		auto rc = rebel_slot->culture;
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto nation_pculture = ((nations::nation const*)primary_slot)->primary_culture;
			if(is_valid_index(nation_pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[nation_pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_reb_pop(TRIGGER_PARAMTERS) {
		auto rc = rebel_slot->culture;
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto pculture = ((population::pop const*)primary_slot)->culture;
			if(is_valid_index(pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_this_nation(TRIGGER_PARAMTERS) {
		auto rc = ((nations::nation const*)primary_slot)->primary_culture;
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto nation_pculture = ((nations::nation const*)this_slot)->primary_culture;
			if(is_valid_index(nation_pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[nation_pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_nation(TRIGGER_PARAMTERS) {
		auto rc = ((nations::nation const*)primary_slot)->primary_culture;
		if(is_valid_index(rc)) {
			auto cg = ws.s.culture_m.culture_container[rc].group;
			auto pculture = ((population::pop const*)this_slot)->culture;
			if(is_valid_index(pculture))
				return compare_values(tval[0], ws.s.culture_m.culture_container[pculture].group == cg, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_from_nation(TRIGGER_PARAMTERS) {
		return tf_culture_group_nation_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr);
	}
	bool tf_culture_group_pop_from_nation(TRIGGER_PARAMTERS) {
		return tf_culture_group_pop_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr);
	}
	bool tf_culture_group_nation_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_culture_group_nation_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_culture_group_pop_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_culture_group_nation_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_culture_group_pop_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_nation_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_culture_group_nation_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_culture_group_pop_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_culture_group_pop_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_religion_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == rebel_slot->religion, true);
	}
	bool tf_religion_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == ((nations::nation const*)from_slot)->national_religion, true);
	}
	bool tf_religion_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == ((nations::nation const*)this_slot)->national_religion, true);
	}
	bool tf_religion_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == owner->national_religion, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == owner->national_religion, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == owner->national_religion, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_terrain_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->terrain == trigger_payload(tval[2]).prov_mod, true);
	}
	bool tf_terrain_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location))
			return compare_values(tval[0], ws.w.province_s.province_state_container[location].terrain == trigger_payload(tval[2]).prov_mod, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_trade_goods(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->rgo_production == trigger_payload(tval[2]).small.values.good, true);
	}

	bool tf_is_secondary_power_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->overall_rank <= int32_t(ws.s.modifiers_m.global_defines.colonial_rank), true);
	}
	bool tf_is_secondary_power_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_is_secondary_power_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_faction_nation(TRIGGER_PARAMTERS) {
		auto nation_factions = get_range(ws.w.population_s.rebel_faction_arrays, ((nations::nation const*)primary_slot)->active_rebel_factions);
		auto rebel_type = trigger_payload(tval[2]).small.values.rebel;
		bool has_faction = false;
		for(auto f : nation_factions) {
			if(is_valid_index(f) && ws.w.population_s.rebel_factions[f].type == rebel_type) {
				has_faction = true;
				break;
			}
		}
		return compare_values(tval[0], has_faction, true);
	}
	bool tf_has_faction_pop(TRIGGER_PARAMTERS) {
		auto pop_faction = ((population::pop const*)primary_slot)->rebel_faction;
		if(is_valid_index(pop_faction))
			return compare_values(tval[0], ws.w.population_s.rebel_factions[pop_faction].type == trigger_payload(tval[2]).small.values.rebel, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_unclaimed_cores(TRIGGER_PARAMTERS) {
		auto nation_tag = ((nations::nation const*)primary_slot)->tag;
		bool has_all_cores = true;
		if(is_valid_index(nation_tag)) {
			auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[nation_tag].core_provinces);
			for(auto p : core_range) {
				if(is_valid_index(p) && ws.w.province_s.province_state_container[p].owner != ((nations::nation const*)primary_slot)) {
					has_all_cores = false;
					break;
				}
			}
		}
		return compare_values(tval[0], has_all_cores, true);
	}
	bool tf_is_cultural_union_bool(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::union_holder_of(ws, *((nations::nation const*)primary_slot)) == primary_slot, true);
	}
	bool tf_is_cultural_union_this_self_pop(TRIGGER_PARAMTERS) {
		auto pculture = ((population::pop const*)primary_slot)->culture;
		auto pop_union = is_valid_index(pculture) ? nations::union_holder_for(ws, pculture) : nullptr;

		return compare_values(tval[0], this_slot == pop_union, true);
	}
	bool tf_is_cultural_union_pop_this_pop(TRIGGER_PARAMTERS) {
		auto nation = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(nation)
			return tf_is_cultural_union_this_self_pop(tval, ws, primary_slot, nation, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_nation(TRIGGER_PARAMTERS) {
		auto main_union = nations::union_holder_of(ws, *((nations::nation const*)primary_slot));
		return compare_values(tval[0], this_slot == main_union, true);
	}
	bool tf_is_cultural_union_this_pop(TRIGGER_PARAMTERS) {
		auto pop_owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(pop_owner)
			return tf_is_cultural_union_this_nation(tval, ws, primary_slot, pop_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_state(TRIGGER_PARAMTERS) {
		auto state_owner = ((nations::state_instance const*)this_slot)->owner;
		if(state_owner)
			return tf_is_cultural_union_this_nation(tval, ws, primary_slot, state_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_province(TRIGGER_PARAMTERS) {
		auto prov_owner = ((provinces::province_state const*)this_slot)->owner;
		if(prov_owner)
			return tf_is_cultural_union_this_nation(tval, ws, primary_slot, prov_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_this_rebel(TRIGGER_PARAMTERS) {
		if(is_valid_index(rebel_slot->culture))
			return compare_values(tval[0], primary_slot == nations::union_holder_for(ws, rebel_slot->culture), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::union_tag_of(ws, *((nations::nation const*)primary_slot)) == trigger_payload(tval[2]).tag, true);
	}
	bool tf_is_cultural_union_tag_this_pop(TRIGGER_PARAMTERS) {
		auto pop_owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(pop_owner)
			return tf_is_cultural_union_tag_nation(tval, ws, pop_owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_this_state(TRIGGER_PARAMTERS) {
		auto state_owner = ((nations::state_instance const*)this_slot)->owner;
		if(state_owner)
			return tf_is_cultural_union_tag_nation(tval, ws, state_owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_this_province(TRIGGER_PARAMTERS) {
		auto prov_owner = ((provinces::province_state const*)this_slot)->owner;
		if(prov_owner)
			return tf_is_cultural_union_tag_nation(tval, ws, prov_owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_cultural_union_tag_this_nation(TRIGGER_PARAMTERS) {
		return tf_is_cultural_union_tag_nation(tval, ws, this_slot, nullptr, nullptr, nullptr);
	}
	bool tf_can_build_factory_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->current_rules.rules_value | issues::rules::pop_build_factory), true);
	}
	bool tf_can_build_factory_province(TRIGGER_PARAMTERS) {
		auto p_owner = ((provinces::province_state const*)primary_slot)->owner;
		if(p_owner)
			return tf_can_build_factory_nation(tval, ws, p_owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_can_build_factory_pop(TRIGGER_PARAMTERS) {
		auto p_owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(p_owner)
			return tf_can_build_factory_nation(tval, ws, p_owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.military_s.war_arrays, ((nations::nation const*)primary_slot)->wars_involved_in) != 0, true);
	}
	bool tf_war_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_war_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_exhaustion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->war_exhaustion, read_float_from_payload(tval + 2));
	}
	bool tf_war_exhaustion_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_war_exhaustion_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_war_exhaustion_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_war_exhaustion_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_blockade(TRIGGER_PARAMTERS) {
		auto cpc = float(((nations::nation const*)primary_slot)->central_province_count);
		if(cpc != 0.0f)
			return compare_values(tval[0], float(((nations::nation const*)primary_slot)->blockaded_count) / cpc, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_owns(TRIGGER_PARAMTERS) {
		provinces::province_tag prov(tval[2]);
		return compare_values(tval[0], ws.w.province_s.province_state_container[prov].owner == primary_slot, true);
	}
	bool tf_controls(TRIGGER_PARAMTERS) {
		provinces::province_tag prov(tval[2]);
		return compare_values(tval[0], ws.w.province_s.province_state_container[prov].controller == primary_slot, true);
	}
	bool tf_is_core_integer(TRIGGER_PARAMTERS) {
		provinces::province_tag prov(tval[2]);
		return compare_values(tval[0], contains_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[prov].cores, ((nations::nation const*)primary_slot)->tag), true);
	}
	bool tf_is_core_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.province_s.core_arrays, ((provinces::province_state const*)primary_slot)->cores, ((nations::nation const*)this_slot)->tag), true);
	}
	bool tf_is_core_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_is_core_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_core_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_is_core_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_core_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_is_core_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_core_from_nation(TRIGGER_PARAMTERS) {
		return tf_is_core_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr);
	}
	bool tf_is_core_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.province_s.core_arrays, ((provinces::province_state const*)primary_slot)->cores, rebel_slot->independence_tag), true);
	}
	bool tf_is_core_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.province_s.core_arrays, ((provinces::province_state const*)primary_slot)->cores, trigger_payload(tval[2]).tag), true);
	}
	bool tf_num_of_revolts(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->rebel_controlled_provinces, tval[2]);
	}
	bool tf_revolt_percentage(TRIGGER_PARAMTERS) {
		auto cpc = float(((nations::nation const*)primary_slot)->central_province_count);
		if(cpc != 0.0f)
			return compare_values(tval[0], float(((nations::nation const*)primary_slot)->rebel_controlled_provinces) / cpc, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_num_of_cities_int(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces), uint32_t(tval[2]));
	}
	bool tf_num_of_cities_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			get_size(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces),
			get_size(ws.w.province_s.province_arrays, ((nations::nation const*)from_slot)->owned_provinces));
	}
	bool tf_num_of_cities_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			get_size(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces),
			get_size(ws.w.province_s.province_arrays, ((nations::nation const*)this_slot)->owned_provinces));
	}
	bool tf_num_of_cities_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_num_of_cities_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces), 0ui32);
	}
	bool tf_num_of_cities_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_num_of_cities_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces), 0ui32);
	}
	bool tf_num_of_cities_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_num_of_cities_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces), 0ui32);
	}
	bool tf_num_of_ports(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->num_connected_ports, tval[2]);
	}
	bool tf_num_of_allies(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->allies), uint32_t(tval[2]));
	}
	bool tf_num_of_vassals(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->vassals), uint32_t(tval[2]));
	}
	bool tf_owned_by_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		return compare_values(tval[0], bool((holder != nullptr)  & (holder == ((provinces::province_state const*)primary_slot)->owner)), true);
	}
	bool tf_owned_by_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], bool((this_slot != nullptr)  & (this_slot == ((provinces::province_state const*)primary_slot)->owner)), true);
	}
	bool tf_owned_by_from_nation(TRIGGER_PARAMTERS) {
		return tf_owned_by_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr);
	}
	bool tf_owned_by_this_province(TRIGGER_PARAMTERS) {
		return tf_owned_by_this_nation(tval, ws, primary_slot, ((provinces::province_state const*)this_slot)->owner, nullptr, nullptr);
	}
	bool tf_owned_by_this_state(TRIGGER_PARAMTERS) {
		return tf_owned_by_this_nation(tval, ws, primary_slot, ((nations::state_instance const*)this_slot)->owner, nullptr, nullptr);
	}
	bool tf_owned_by_this_pop(TRIGGER_PARAMTERS) {
		return tf_owned_by_this_nation(tval, ws, primary_slot, population::get_pop_owner(ws,*((population::pop const*)this_slot)), nullptr, nullptr);
	}
	bool tf_exists_bool(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces) != 0ui32, true);
	}
	bool tf_exists_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], get_size(ws.w.province_s.province_arrays, holder->owned_provinces) != 0ui32, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_country_flag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.variable_s.national_flags_arrays, ((nations::nation const*)primary_slot)->national_flags, trigger_payload(tval[2]).nat_flag), true);
	}
	bool tf_has_country_flag_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_has_country_flag(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_country_flag_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_has_country_flag(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_continent_province(TRIGGER_PARAMTERS) {
		auto prov_id = ((provinces::province_state const*)primary_slot)->id;
		return compare_values(tval[0], ws.s.province_m.province_container[prov_id].continent == trigger_payload(tval[2]).prov_mod, true);
	}
	bool tf_continent_state(TRIGGER_PARAMTERS) {
		auto region_id = ((nations::state_instance const*)primary_slot)->region_id;
		if(is_valid_index(region_id)) {
			auto prov_id = ws.s.province_m.states_to_province_index.get(region_id, 0);
			return compare_values(tval[0], ws.s.province_m.province_container[prov_id].continent == trigger_payload(tval[2]).prov_mod, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_continent_nation(TRIGGER_PARAMTERS) {
		auto prov_id = ((nations::nation const*)primary_slot)->current_capital;
		if(is_valid_index(prov_id))
			return compare_values(tval[0], ws.s.province_m.province_container[prov_id].continent == trigger_payload(tval[2]).prov_mod, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_continent_pop(TRIGGER_PARAMTERS) {
		auto prov_id = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(prov_id))
			return compare_values(tval[0], ws.s.province_m.province_container[prov_id].continent == trigger_payload(tval[2]).prov_mod, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_continent_nation_this(TRIGGER_PARAMTERS) {
		return tf_continent_nation(tval, ws, this_slot, nullptr, nullptr, nullptr);
	}
	bool tf_continent_state_this(TRIGGER_PARAMTERS) {
		return tf_continent_state(tval, ws, this_slot, nullptr, nullptr, nullptr);
	}
	bool tf_continent_province_this(TRIGGER_PARAMTERS) {
		return tf_continent_province(tval, ws, this_slot, nullptr, nullptr, nullptr);
	}
	bool tf_continent_pop_this(TRIGGER_PARAMTERS) {
		return tf_continent_pop(tval, ws, this_slot, nullptr, nullptr, nullptr);
	}
	bool tf_continent_nation_from(TRIGGER_PARAMTERS) {
		return tf_continent_nation(tval, ws, from_slot, nullptr, nullptr, nullptr);
	}
	bool tf_continent_state_from(TRIGGER_PARAMTERS) {
		return tf_continent_state(tval, ws, from_slot, nullptr, nullptr, nullptr);
	}
	bool tf_continent_province_from(TRIGGER_PARAMTERS) {
		return tf_continent_province(tval, ws, from_slot, nullptr, nullptr, nullptr);
	}
	bool tf_continent_pop_from(TRIGGER_PARAMTERS) {
		return tf_continent_pop(tval, ws, from_slot, nullptr, nullptr, nullptr);
	}
	bool tf_casus_belli_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], military::can_use_cb_against(ws, *((nations::nation const*)primary_slot), *tag_holder), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_casus_belli_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::can_use_cb_against(ws, *((nations::nation const*)primary_slot), *((nations::nation const*)from_slot)), true);
	}
	bool tf_casus_belli_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::can_use_cb_against(ws, *((nations::nation const*)primary_slot), *((nations::nation const*)this_slot)), true);
	}
	bool tf_casus_belli_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::can_use_cb_against(ws, *((nations::nation const*)primary_slot), *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_casus_belli_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::can_use_cb_against(ws, *((nations::nation const*)primary_slot), *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_casus_belli_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], military::can_use_cb_against(ws, *((nations::nation const*)primary_slot), *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], military::has_military_access_with(ws, *((nations::nation const*)primary_slot), *holder), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_military_access_with(ws, *((nations::nation const*)primary_slot), *((nations::nation const*)from_slot)), true);
	}
	bool tf_military_access_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_military_access_with(ws, *((nations::nation const*)primary_slot), *((nations::nation const*)this_slot)), true);
	}
	bool tf_military_access_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::has_military_access_with(ws, *((nations::nation const*)primary_slot), *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::has_military_access_with(ws, *((nations::nation const*)primary_slot), *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_military_access_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], military::has_military_access_with(ws, *((nations::nation const*)primary_slot), *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_prestige_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_prestige_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), nations::get_prestige(*((nations::nation const*)from_slot)));
	}
	bool tf_prestige_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), nations::get_prestige(*((nations::nation const*)this_slot)));
	}
	bool tf_prestige_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), nations::get_prestige(*owner));
		else
			return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), 0.0f);
	}
	bool tf_prestige_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), nations::get_prestige(*owner));
		else
			return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), 0.0f);
	}
	bool tf_prestige_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), nations::get_prestige(*owner));
		else
			return compare_values(tval[0], nations::get_prestige(*((nations::nation const*)primary_slot)), 0.0f);
	}
	bool tf_badboy(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->infamy, read_float_from_payload(tval + 2));
	}
	bool tf_has_building_fort(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->fort_level != 0ui8, true);
	}
	bool tf_has_building_railroad(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->railroad_level != 0ui8, true);
	}
	bool tf_has_building_naval_base(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->railroad_level != 0ui8, true);
	}

	bool tf_has_building_factory(TRIGGER_PARAMTERS) {
		auto si = ((nations::state_instance const*)primary_slot);
		auto has_factories = [si]() {
			for(uint32_t i = 0; i < std::extent_v<decltype(si->factories)>; ++i) {
				if(si->factories[i].type)
					return true;
			}
			return false;
		}();
		return compare_values(tval[0], has_factories, true);
	}
	bool tf_has_building_state(TRIGGER_PARAMTERS) {
		auto si = ((nations::state_instance const*)primary_slot);
		auto type = trigger_payload(tval[2]).small.values.factory;
		auto has_factory = [si, type]() {
			for(uint32_t i = 0; i < std::extent_v<decltype(si->factories)>; ++i) {
				if(si->factories[i].type && si->factories[i].type->id == type)
					return true;
			}
			return false;
		}();
		return compare_values(tval[0], has_factory, true);
	}
	bool tf_has_building_state_from_province(TRIGGER_PARAMTERS) {
		auto state = ((provinces::province_state const*)primary_slot)->state_instance;
		if(state)
			return tf_has_building_state(tval, ws, state, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_building_factory_from_province(TRIGGER_PARAMTERS) {
		auto state = ((provinces::province_state const*)primary_slot)->state_instance;
		if(state)
			return tf_has_building_factory(tval, ws, state, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_empty(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->owner == nullptr, true);
	}
	bool tf_is_blockaded(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], (((provinces::province_state const*)primary_slot)->flags & provinces::province_state::is_blockaded) != 0, true);
	}
	bool tf_has_country_modifier(TRIGGER_PARAMTERS) {
		const auto mod = trigger_payload(tval[2]).nat_mod;
		return compare_values(tval[0],
			modifiers::has_national_modifier(ws, *((nations::nation const*)primary_slot), mod),
			true);
	}
	bool tf_has_country_modifier_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_has_country_modifier(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_province_modifier(TRIGGER_PARAMTERS) {
		const auto mod = trigger_payload(tval[2]).prov_mod;
		return compare_values(tval[0],
			modifiers::has_provincial_modifier(ws, *((provinces::province_state const*)primary_slot), mod),
			true);
	}
	bool tf_region(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.s.province_m.province_container[((provinces::province_state const*)primary_slot)->id].state_id == trigger_payload(tval[2]).state, true);
	}
	bool tf_tag_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->tag == trigger_payload(tval[2]).tag, true);
	}
	bool tf_tag_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot == this_slot, true);
	}
	bool tf_tag_this_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot == ((provinces::province_state const*)this_slot)->owner, true);
	}
	bool tf_tag_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot == from_slot, true);
	}
	bool tf_tag_from_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], primary_slot == ((provinces::province_state const*)from_slot)->owner, true);
	}
	bool tf_tag_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_tag_tag(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_neighbour_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->neighboring_nations, tag_holder->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_neighbour_this(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			contains_item(
				ws.w.nation_s.nations_arrays,
				((nations::nation const*)primary_slot)->neighboring_nations,
				((nations::nation const*)this_slot)->id),
			true);
	}
	bool tf_neighbour_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			contains_item(
				ws.w.nation_s.nations_arrays,
				((nations::nation const*)primary_slot)->neighboring_nations,
				((nations::nation const*)from_slot)->id),
			true);
	}
	bool tf_units_in_province_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::total_units_in_province(ws, *((provinces::province_state const*)primary_slot)), uint32_t(tval[2]));
	}
	bool tf_units_in_province_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_units_in_province(ws, *((nations::nation const*)from_slot), *((provinces::province_state const*)primary_slot)), true);
	}
	bool tf_units_in_province_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_units_in_province(ws, *((nations::nation const*)this_slot), *((provinces::province_state const*)primary_slot)), true);
	}
	bool tf_units_in_province_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::has_units_in_province(ws, *owner, *((provinces::province_state const*)primary_slot)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_units_in_province_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::has_units_in_province(ws, *owner, *((provinces::province_state const*)primary_slot)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_units_in_province_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], military::has_units_in_province(ws, *owner, *((provinces::province_state const*)primary_slot)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], military::in_war_against(ws, *((nations::nation const*)primary_slot), holder->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::in_war_against(ws, *((nations::nation const*)primary_slot), ((nations::nation const*)from_slot)->id), true);
	}
	bool tf_war_with_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::in_war_against(ws, *((nations::nation const*)primary_slot), ((nations::nation const*)this_slot)->id), true);
	}
	bool tf_war_with_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::in_war_against(ws, *((nations::nation const*)primary_slot), owner->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], military::in_war_against(ws, *((nations::nation const*)primary_slot), owner->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_war_with_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], military::in_war_against(ws, *((nations::nation const*)primary_slot), owner->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_unit_in_battle(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::province_is_contested(ws, *((provinces::province_state const*)primary_slot)), true);
	}
	bool tf_total_amount_of_divisions(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::total_active_divisions(ws, *((nations::nation const*)primary_slot)), uint32_t(tval[2]));
	}
	bool tf_money(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], nations::national_treasury(ws, id), economy::goods_qnty_type(read_float_from_payload(tval + 2)));
		else
			return compare_values(tval[0], economy::goods_qnty_type(0.0), economy::goods_qnty_type(read_float_from_payload(tval + 2)));
	}
	bool tf_lost_national(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 1.0f - nations::fraction_of_cores_owned(ws, *((nations::nation const*)primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_is_vassal(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord != nullptr, true);
	}
	bool tf_ruling_party_ideology_nation(TRIGGER_PARAMTERS) {
		auto ri = ((nations::nation const*)primary_slot)->ruling_ideology;
		if(is_valid_index(ri))
			return compare_values(tval[0], ri == trigger_payload(tval[2]).small.values.ideology, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_ruling_party_ideology_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_ruling_party_ideology_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_ruling_party(TRIGGER_PARAMTERS) {
		auto rp = ((nations::nation const*)primary_slot)->ruling_party;
		if(is_valid_index(rp))
			return compare_values(tval[0], ws.s.governments_m.parties[rp].name == trigger_payload(tval[2]).text, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_ideology_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ideologies::ideology_enabled(ws, trigger_payload(tval[2]).small.values.ideology), true);
	}
	bool tf_political_reform_want_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->political_interest_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_political_reform_want_pop(TRIGGER_PARAMTERS) {
		auto pop_id = ((population::pop const*)primary_slot)->id;
		if(ws.w.population_s.pops.is_valid_index(pop_id))
			return compare_values(tval[0], issues::calculate_political_interest(ws, ws.w.population_s.pop_demographics.get_row(pop_id)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_social_reform_want_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->social_interest_fraction, read_float_from_payload(tval + 2));
	}
	bool tf_social_reform_want_pop(TRIGGER_PARAMTERS) {
		auto pop_id = ((population::pop const*)primary_slot)->id;
		if(ws.w.population_s.pops.is_valid_index(pop_id))
			return compare_values(tval[0], issues::calculate_social_interest(ws, ws.w.population_s.pop_demographics.get_row(pop_id)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_total_amount_of_ships(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::total_active_ships(ws, *((nations::nation const*)primary_slot)), uint32_t(tval[2]));
	}
	bool tf_plurality(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->plurality, read_float_from_payload(tval + 2));
	}
	bool tf_corruption(TRIGGER_PARAMTERS) {
		auto cpc = float(((nations::nation const*)primary_slot)->central_province_count);
		if(cpc != 0.0f)
			return compare_values(tval[0], float(((nations::nation const*)primary_slot)->crime_count) / cpc, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_is_state_religion_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return compare_values(tval[0], owner->national_religion == ((population::pop const*)primary_slot)->religion, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_state_religion_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->national_religion == ((provinces::province_state const*)primary_slot)->dominant_religion, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_state_religion_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->national_religion == ((nations::state_instance const*)primary_slot)->dominant_religion, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return compare_values(tval[0], owner->primary_culture == ((population::pop const*)primary_slot)->culture, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->primary_culture == ((provinces::province_state const*)primary_slot)->dominant_culture, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->primary_culture == ((nations::state_instance const*)primary_slot)->dominant_culture, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_nation_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->primary_culture == ((population::pop const*)this_slot)->culture, true);
	}
	bool tf_is_primary_culture_nation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->primary_culture == ((nations::nation const*)this_slot)->primary_culture, true);
	}
	bool tf_is_primary_culture_nation_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = ((nations::state_instance const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_nation_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_nation_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = ((provinces::province_state const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_nation_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->dominant_culture == ((nations::nation const*)this_slot)->primary_culture, true);
	}
	bool tf_is_primary_culture_state_this_pop(TRIGGER_PARAMTERS) {
		auto this_owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(this_owner)
			return tf_is_primary_culture_state_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = ((nations::state_instance const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_state_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_state_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = ((provinces::province_state const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_state_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->dominant_culture == ((nations::nation const*)this_slot)->primary_culture, true);
	}
	bool tf_is_primary_culture_province_this_pop(TRIGGER_PARAMTERS) {
		auto this_owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(this_owner)
			return tf_is_primary_culture_province_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = ((nations::state_instance const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_province_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_province_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = ((provinces::province_state const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_province_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->culture == ((nations::nation const*)this_slot)->primary_culture, true);
	}
	bool tf_is_primary_culture_pop_this_pop(TRIGGER_PARAMTERS) {
		auto this_owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(this_owner)
			return tf_is_primary_culture_pop_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop_this_state(TRIGGER_PARAMTERS) {
		auto this_owner = ((nations::state_instance const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_pop_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_primary_culture_pop_this_province(TRIGGER_PARAMTERS) {
		auto this_owner = ((provinces::province_state const*)this_slot)->owner;
		if(this_owner)
			return tf_is_primary_culture_pop_this_nation(tval, ws, primary_slot, this_owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_accepted_culture_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return compare_values(tval[0],
				owner->primary_culture == ((population::pop const*)primary_slot)->culture ||
				contains_item(ws.w.culture_s.culture_arrays, owner->accepted_cultures, ((population::pop const*)primary_slot)->culture),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_accepted_culture_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0],
				owner->primary_culture == ((provinces::province_state const*)primary_slot)->dominant_culture ||
				contains_item(ws.w.culture_s.culture_arrays, owner->accepted_cultures, ((provinces::province_state const*)primary_slot)->dominant_culture),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_accepted_culture_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0],
				owner->primary_culture == ((nations::state_instance const*)primary_slot)->dominant_culture ||
				contains_item(ws.w.culture_s.culture_arrays, owner->accepted_cultures, ((nations::state_instance const*)primary_slot)->dominant_culture),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_coastal(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			0 != (ws.s.province_m.province_container[((provinces::province_state const*)primary_slot)->id].flags & provinces::province::coastal),
			true);
	}
	bool tf_in_sphere_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			((nations::nation const*)primary_slot)->sphere_leader == ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder,
			true);
	}
	bool tf_in_sphere_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			((nations::nation const*)primary_slot)->sphere_leader == from_slot,
			true);
	}
	bool tf_in_sphere_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			((nations::nation const*)primary_slot)->sphere_leader == this_slot,
			true);
	}
	bool tf_in_sphere_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_in_sphere_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_sphere_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_in_sphere_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_sphere_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_in_sphere_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_produces_nation(TRIGGER_PARAMTERS) {
		auto good = trigger_payload(tval[2]).small.values.good;

		auto owned_range = get_range(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces);
		for(auto p : owned_range) {
			provinces::province_state const& ps = ws.w.province_s.province_state_container[p];
			if((ps.rgo_production == good) | (ps.artisan_production == good))
				return compare_values(tval[0], true, true);
		}
		auto states = get_range(ws.w.nation_s.state_arrays, ((nations::nation const*)primary_slot)->member_states);
		for(auto s = states.first; s != states.second; ++s) {
			auto si = s->state;
			for(int32_t i = int32_t(std::extent_v<decltype(si->factories)>); i--; ) {
				if(si->factories[i].type && si->factories[i].type->output_good == good)
					return compare_values(tval[0], true, true);
			}
		}

		return compare_values(tval[0], false, true);
	}
	bool tf_produces_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			(((provinces::province_state const*)primary_slot)->rgo_production == trigger_payload(tval[2]).small.values.good) ||
			(((provinces::province_state const*)primary_slot)->artisan_production == trigger_payload(tval[2]).small.values.good),
			true);
	}
	bool tf_produces_state(TRIGGER_PARAMTERS) {
		auto state_provinces = ws.s.province_m.states_to_province_index.get_row(((nations::state_instance const*)primary_slot)->region_id);
		auto state_owner = ((nations::state_instance const*)primary_slot)->owner;
		auto good = trigger_payload(tval[2]).small.values.good;

		for(auto p : state_provinces) {
			provinces::province_state const& ps = ws.w.province_s.province_state_container[p];
			if((ps.owner == state_owner) & ((ps.rgo_production == good) | (ps.artisan_production == good)))
				return compare_values(tval[0], true, true);
		}
		for(int32_t i = int32_t(std::extent_v<decltype(((nations::state_instance const*)primary_slot)->factories)>); i--; ) {
			if(((nations::state_instance const*)primary_slot)->factories[i].type && ((nations::state_instance const*)primary_slot)->factories[i].type->output_good == good)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	
	bool tf_produces_pop(TRIGGER_PARAMTERS) {
		auto pop_location = ((population::pop const*)primary_slot)->location;
		if(((population::pop const*)primary_slot)->type == ws.s.population_m.artisan && is_valid_index(pop_location))
			return compare_values(tval[0],
				ws.w.province_s.province_state_container[pop_location].rgo_production == trigger_payload(tval[2]).small.values.good,
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_average_militancy_nation(TRIGGER_PARAMTERS) {
		auto nation_id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(nation_id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(nation_id, population::militancy_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_militancy_state(TRIGGER_PARAMTERS) {
		auto state_id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(state_id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(state_id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(state_id, population::militancy_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_militancy_province(TRIGGER_PARAMTERS) {
		auto prov_id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(prov_id, population::total_population_tag);
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(prov_id, population::militancy_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_consciousness_nation(TRIGGER_PARAMTERS) {
		auto nation_id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(nation_id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(nation_id, population::consciousness_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_consciousness_state(TRIGGER_PARAMTERS) {
		auto state_id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(state_id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(state_id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(state_id, population::consciousness_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_average_consciousness_province(TRIGGER_PARAMTERS) {
		auto prov_id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(prov_id, population::total_population_tag);
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(prov_id, population::consciousness_demo_tag(ws))) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_is_next_reform_nation(TRIGGER_PARAMTERS) {
		auto reform_id = trigger_payload(tval[2]).small.values.option;
		auto reform_parent = ws.s.issues_m.options[reform_id].parent_issue;
		auto nation_id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(nation_id)) {
			auto active_option = ws.w.nation_s.active_issue_options.get(nation_id, reform_parent);
			return compare_values(tval[0], (to_index(active_option) + 1) == to_index(reform_id), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_is_next_reform_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_is_next_reform_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rebel_power_fraction(TRIGGER_PARAMTERS) {
		// note: virtually unused
		return compare_values(tval[0], false, true);
	}
	bool tf_recruited_percentage_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::recruited_pop_fraction(ws, *((nations::nation const*)primary_slot)), read_float_from_payload(tval + 2));
	}
	bool tf_recruited_percentage_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_recruited_percentage_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_has_culture_core(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		auto culture = ((population::pop const*)primary_slot)->culture;
		if(is_valid_index(location)) {
			auto cores = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[location].cores);
			for(auto tag : cores) {
				if(is_valid_index(tag)) {
					if(auto holder = ws.w.culture_s.national_tags_state[tag].holder; bool(holder) && holder->primary_culture == culture)
						return compare_values(tval[0], true, true);
				}
			}
			return compare_values(tval[0], false, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_has_culture_core_province_this_pop(TRIGGER_PARAMTERS) {
		auto location = ((provinces::province_state const*)primary_slot)->id;
		auto culture = ((population::pop const*)this_slot)->culture;
		if(is_valid_index(location)) {
			auto cores = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[location].cores);
			for(auto tag : cores) {
				if(is_valid_index(tag)) {
					if(auto holder = ws.w.culture_s.national_tags_state[tag].holder; bool(holder) && holder->primary_culture == culture)
						return compare_values(tval[0], true, true);
				}
			}
			return compare_values(tval[0], false, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_nationalism(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->nationalism, float(tval[2]));
	}
	bool tf_is_overseas(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((provinces::province_state const*)primary_slot)->flags & provinces::province_state::is_overseas), true);
	}
	bool tf_controlled_by_rebels(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->rebel_controller != nullptr, true);
	}
	bool tf_controlled_by_tag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],((provinces::province_state const*)primary_slot)->controller == ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder, true);
	}
	bool tf_controlled_by_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->controller == from_slot, true);
	}
	bool tf_controlled_by_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->controller == this_slot, true);
	}
	bool tf_controlled_by_this_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->controller == ((provinces::province_state const*)this_slot)->owner, true);
	}
	bool tf_controlled_by_this_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->controller == ((nations::state_instance const*)this_slot)->owner, true);
	}
	bool tf_controlled_by_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->controller == population::get_pop_owner(ws, *((population::pop const*)this_slot)), true);
	}
	bool tf_controlled_by_owner(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->owner == ((provinces::province_state const*)primary_slot)->controller, true);
	}
	bool tf_controlled_by_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->rebel_controller == rebel_slot, true);
	}
	bool tf_is_canal_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.is_canal_enabled[tval[2]-1] != 0ui8, true);
	}
	bool tf_is_state_capital(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto si = ((provinces::province_state const*)primary_slot)->state_instance;
		if(si) {
			auto state_range = ws.s.province_m.states_to_province_index.get_row(ws.s.province_m.province_container[id].state_id);
			for(auto ip : state_range) {
				if(ws.w.province_s.province_state_container[ip].state_instance == si) {
					if(ip == id)
						return compare_values(tval[0], true, true);
					else
						return compare_values(tval[0], false, true);
				}
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ((nations::nation const*)primary_slot)->truces, nations::truce{date_tag(), holder->id}), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ((nations::nation const*)primary_slot)->truces, nations::truce { date_tag(), ((nations::nation const*)from_slot)->id }), true);
	}
	bool tf_truce_with_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ((nations::nation const*)primary_slot)->truces, nations::truce { date_tag(), ((nations::nation const*)this_slot)->id }), true);
	}
	bool tf_truce_with_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ((nations::nation const*)primary_slot)->truces, nations::truce { date_tag(), owner->id }), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ((nations::nation const*)primary_slot)->truces, nations::truce { date_tag(), owner->id }), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_truce_with_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.truce_arrays, ((nations::nation const*)primary_slot)->truces, nations::truce { date_tag(), owner->id }), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_total_pops_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::total_population_tag)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_total_pops_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::total_population_tag)), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_total_pops_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::total_population_tag)), read_float_from_payload(tval + 2));
	}
	bool tf_total_pops_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			if(auto si = ws.w.province_s.province_state_container[location].state_instance; si)
				return tf_total_pops_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_has_pop_type_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0],
				0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type)),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_type_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0],
				0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type)),
				true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_type_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type)), true);
	}
	bool tf_has_pop_type_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->type == trigger_payload(tval[2]).small.values.pop_type, true);
	}
	bool tf_has_empty_adjacent_province(TRIGGER_PARAMTERS) {
		auto adj_range = ws.s.province_m.same_type_adjacency.get_row(((provinces::province_state const*)primary_slot)->id);
		for(auto p : adj_range) {
			if(ws.w.province_s.province_state_container[p].owner == nullptr)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_has_leader(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], military::has_named_leader(ws, *((nations::nation const*)primary_slot), trigger_payload(tval[2]).text), true);
	}
	bool tf_ai(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 == (((nations::nation const*)primary_slot)->flags & nations::nation::is_not_ai_controlled), true);
	}
	bool tf_can_create_vassals(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], true, true); // stub for apparently unused 
	}
	bool tf_is_possible_vassal(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], can_release_as_vassal(ws, *((nations::nation const*)primary_slot), trigger_payload(tval[2]).tag), true);
	}
	bool tf_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->id == provinces::province_tag(tval[2]), true);
	}
	bool tf_vassal_of_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord == tag_holder, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_vassal_of_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord == from_slot, true);
	}
	bool tf_vassal_of_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord == this_slot, true);
	}
	bool tf_vassal_of_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_vassal_of_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_vassal_of_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->overlord == owner, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_substate_of_tag(TRIGGER_PARAMTERS) {
		if(0 == (((nations::nation const*)primary_slot)->flags & nations::nation::is_substate))
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_tag(tval, ws, primary_slot, nullptr, nullptr, nullptr);
	}
	bool tf_substate_of_from(TRIGGER_PARAMTERS) {
		if(0 == (((nations::nation const*)primary_slot)->flags & nations::nation::is_substate))
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr);
	}
	bool tf_substate_of_this_nation(TRIGGER_PARAMTERS) {
		if(0 == (((nations::nation const*)primary_slot)->flags & nations::nation::is_substate))
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_nation(tval, ws, primary_slot, this_slot, nullptr, nullptr);
	}
	bool tf_substate_of_this_province(TRIGGER_PARAMTERS) {
		if(0 == (((nations::nation const*)primary_slot)->flags & nations::nation::is_substate))
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_province(tval, ws, primary_slot, this_slot, nullptr, nullptr);
	}
	bool tf_substate_of_this_state(TRIGGER_PARAMTERS) {
		if(0 == (((nations::nation const*)primary_slot)->flags & nations::nation::is_substate))
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_state(tval, ws, primary_slot, this_slot, nullptr, nullptr);
	}
	bool tf_substate_of_this_pop(TRIGGER_PARAMTERS) {
		if(0 == (((nations::nation const*)primary_slot)->flags & nations::nation::is_substate))
			return compare_values(tval[0], false, true);
		else
			return tf_vassal_of_this_pop(tval, ws, primary_slot, this_slot, nullptr, nullptr);
	}
	bool tf_alliance_with_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->allies, holder->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_alliance_with_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->allies, ((nations::nation const*)from_slot)->id), true);
	}
	bool tf_alliance_with_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->allies, ((nations::nation const*)this_slot)->id), true);
	}
	bool tf_alliance_with_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->allies, owner->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_alliance_with_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->allies, owner->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_alliance_with_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], contains_item(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->allies, owner->id), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_recently_lost_war(TRIGGER_PARAMTERS) {
		auto last_lost = ((nations::nation const*)primary_slot)->last_lost_war;
		return compare_values(tval[0], is_valid_index(last_lost) && (to_index(ws.w.current_date) - to_index(last_lost)) < (365 * 5 + 1), true);
	}
	bool tf_is_mobilised(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_mobilized), true);
	}
	bool tf_mobilisation_size(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->modifier_values[modifiers::national_offsets::mobilisation_size], read_float_from_payload(tval + 2));
	}
	bool tf_crime_higher_than_education_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->administrative_spending >= ((nations::nation const*)primary_slot)->education_spending, true);
	}
	bool tf_crime_higher_than_education_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->administrative_spending >= owner->education_spending, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_crime_higher_than_education_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->administrative_spending >= owner->education_spending, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_crime_higher_than_education_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return compare_values(tval[0], owner->administrative_spending >= owner->education_spending, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_agree_with_ruling_party(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		auto pop_id = ((population::pop const*)primary_slot)->id;
		if(owner && ws.w.population_s.pops.is_valid_index(pop_id)) {
			auto ruling_ideology = owner->ruling_ideology;
			auto population_size = ws.w.population_s.pop_demographics.get(pop_id, population::total_population_tag);
			if(is_valid_index(ruling_ideology) & (population_size != 0)) {
				return compare_values(tval[0],
					float(ws.w.population_s.pop_demographics.get(pop_id, population::to_demo_tag(ws, ruling_ideology))) / float(population_size),
					read_float_from_payload(tval + 2));
			}
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_is_colonial_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			0 != (((nations::state_instance const*)primary_slot)->flags & (nations::state_instance::is_colonial | nations::state_instance::is_protectorate)),
			true);
	}
	bool tf_is_colonial_province(TRIGGER_PARAMTERS) {
		auto si = ((provinces::province_state const*)primary_slot)->state_instance;
		if(si)
			return tf_is_colonial_state(tval, ws, si, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_factories(TRIGGER_PARAMTERS) {
		for(int32_t i = int32_t(std::extent_v<decltype(((nations::state_instance const*)primary_slot)->factories)>); i--; ) {
			if(((nations::state_instance const*)primary_slot)->factories[i].type)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_in_default_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0],
			(0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_bankrupt)) &&
				contains_item(ws.w.nation_s.loan_arrays, ((nations::nation const*)primary_slot)->loans, nations::loan { 0.0f, holder->id }),
			true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_default_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			(0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_bankrupt)) &&
			contains_item(ws.w.nation_s.loan_arrays, ((nations::nation const*)primary_slot)->loans, nations::loan { 0.0f, ((nations::nation const*)from_slot)->id }),
			true);
	}
	bool tf_in_default_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			(0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_bankrupt)) &&
			contains_item(ws.w.nation_s.loan_arrays, ((nations::nation const*)primary_slot)->loans, nations::loan { 0.0f, ((nations::nation const*)this_slot)->id }),
			true);
	}
	bool tf_in_default_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_in_default_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_default_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_in_default_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_in_default_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_in_default_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_total_num_of_ports(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->num_ports, tval[2]);
	}
	bool tf_always(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], true, true);
	}
	bool tf_election(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_holding_election) , true);
	}
	bool tf_has_global_flag(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.variable_s.global_variables[trigger_payload(tval[2]).global_var] != 0.0f, true);
	}
	bool tf_is_capital(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->current_capital == ((provinces::province_state const*)primary_slot)->id, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_nationalvalue_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->national_value == trigger_payload(tval[2]).nat_mod, true);
	}
	bool tf_nationalvalue_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_nationalvalue_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_nationalvalue_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_nationalvalue_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_industrial_score_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, trigger_payload(tval[2]).signed_value);
	}
	bool tf_industrial_score_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, ((nations::nation const*)from_slot)->industrial_score);
	}
	bool tf_industrial_score_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, ((nations::nation const*)this_slot)->industrial_score);
	}
	bool tf_industrial_score_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, owner->industrial_score);
		else
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, 0i16);
	}
	bool tf_industrial_score_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, owner->industrial_score);
		else
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, 0i16);
	}
	bool tf_industrial_score_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, owner->industrial_score);
		else
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->industrial_score, 0i16);
	}
	bool tf_military_score_value(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, trigger_payload(tval[2]).signed_value);
	}
	bool tf_military_score_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, ((nations::nation const*)from_slot)->military_score);
	}
	bool tf_military_score_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, ((nations::nation const*)this_slot)->military_score);
	}
	bool tf_military_score_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, owner->military_score);
		else
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, 0i16);
	}
	bool tf_military_score_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, owner->military_score);
		else
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, 0i16);
	}
	bool tf_military_score_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, owner->military_score);
		else
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_score, 0i16);
	}
	bool tf_civilized_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_civilized), true);
	}
	bool tf_civilized_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return compare_values(tval[0], 0 != (owner->flags & nations::nation::is_civilized), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_civilized_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], 0 != (owner->flags & nations::nation::is_civilized), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_national_provinces_occupied(TRIGGER_PARAMTERS) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ((nations::nation const*)primary_slot)->owned_provinces);
		if(owned_range.first != owned_range.second) {
			int32_t count_owned_controlled = 0;
			for(auto p : owned_range) {
				if(ws.w.province_s.province_state_container[p].controller == primary_slot)
					++count_owned_controlled;
			}
			return compare_values(tval[0], float(count_owned_controlled) / float(owned_range.second - owned_range.first), read_float_from_payload(tval + 2));
		} else {
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
		}
	}
	bool tf_is_greater_power_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::is_great_power(ws, *((nations::nation const*)primary_slot)), true);
	}
	bool tf_is_greater_power_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return compare_values(tval[0], nations::is_great_power(ws, *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_greater_power_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return compare_values(tval[0], nations::is_great_power(ws, *owner), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rich_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->rich_tax, int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_middle_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->middle_tax, int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_poor_tax(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->poor_tax, int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_social_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->social_spending, int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_social_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_social_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_social_spending_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_social_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->military_spending, int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_military_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return tf_military_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->administrative_spending, int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_administration_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return tf_administration_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->education_spending, int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_education_spending_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return tf_education_spending_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], int8_t(0), int8_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_colonial_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_colonial_nation), true);
	}
	bool tf_pop_majority_religion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->dominant_religion == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_pop_majority_religion_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->dominant_religion == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_pop_majority_religion_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->dominant_religion == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_pop_majority_culture_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->dominant_culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_pop_majority_culture_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->dominant_culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_pop_majority_culture_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->dominant_culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_pop_majority_issue_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->dominant_issue == trigger_payload(tval[2]).small.values.option, true);
	}
	bool tf_pop_majority_issue_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->dominant_issue == trigger_payload(tval[2]).small.values.option, true);
	}
	bool tf_pop_majority_issue_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->dominant_issue == trigger_payload(tval[2]).small.values.option, true);
	}
	bool tf_pop_majority_issue_pop(TRIGGER_PARAMTERS) {
		auto id = ((population::pop const*)primary_slot)->id;
		if(ws.w.population_s.pops.is_valid_index(id))
			return compare_values(tval[0], population::is_dominant_issue(ws, id, trigger_payload(tval[2]).small.values.option), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_pop_majority_ideology_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->dominant_ideology == trigger_payload(tval[2]).small.values.ideology, true);
	}
	bool tf_pop_majority_ideology_state(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->dominant_ideology == trigger_payload(tval[2]).small.values.ideology, true);
	}
	bool tf_pop_majority_ideology_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((provinces::province_state const*)primary_slot)->dominant_ideology == trigger_payload(tval[2]).small.values.ideology, true);
	}
	bool tf_pop_majority_ideology_pop(TRIGGER_PARAMTERS) {
		auto id = ((population::pop const*)primary_slot)->id;
		if(ws.w.population_s.pops.is_valid_index(id))
			return compare_values(tval[0], population::is_dominant_ideology(ws, id, trigger_payload(tval[2]).small.values.ideology), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_poor_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::total_population_tag));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::poor_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::total_population_tag));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::poor_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::total_population_tag));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::poor_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ((population::pop const*)primary_slot)->type;
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_poor)
			return compare_values(tval[0], float(((population::pop const*)primary_slot)->militancy) * 10.0f / float(std::numeric_limits<uint16_t>::max()), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::total_population_tag));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::middle_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::total_population_tag));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::middle_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::total_population_tag));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::middle_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ((population::pop const*)primary_slot)->type;
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_middle)
			return compare_values(tval[0], float(((population::pop const*)primary_slot)->militancy) * 10.0f / float(std::numeric_limits<uint16_t>::max()), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.nation_demographics.get(id, population::total_population_tag));
			auto mil = 10.0f * float(ws.w.nation_s.nation_demographics.get(id, population::rich_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto pop_size = float(ws.w.nation_s.state_demographics.get(id, population::total_population_tag));
			auto mil = 10.0f * float(ws.w.nation_s.state_demographics.get(id, population::rich_militancy_demo_tag(ws)));
			return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto pop_size = float(ws.w.province_s.province_demographics.get(id, population::total_population_tag));
		auto mil = 10.0f * float(ws.w.province_s.province_demographics.get(id, population::rich_militancy_demo_tag(ws)));
		return compare_values(tval[0], (pop_size != 0.0f) ? mil / pop_size : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_militancy_pop(TRIGGER_PARAMTERS) {
		auto type = ((population::pop const*)primary_slot)->type;
		if(is_valid_index(type) && (ws.s.population_m.pop_types[type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich)
			return compare_values(tval[0], float(((population::pop const*)primary_slot)->militancy) * 10.0f / float(std::numeric_limits<uint16_t>::max()), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_tax_above_poor(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->rich_tax > ((nations::nation const*)primary_slot)->poor_tax, true);
	}
	bool tf_culture_has_union_tag_pop(TRIGGER_PARAMTERS) {
		auto pop_culture = ((population::pop const*)primary_slot)->culture;
		if(is_valid_index(pop_culture)) {
			auto group = ws.s.culture_m.culture_container[pop_culture].group;
			return compare_values(tval[0], is_valid_index(ws.s.culture_m.culture_groups[group].union_tag), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_culture_has_union_tag_nation(TRIGGER_PARAMTERS) {
		auto primary_culture = ((nations::nation const*)primary_slot)->primary_culture;
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], is_valid_index(ws.s.culture_m.culture_groups[group].union_tag), true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_tag(TRIGGER_PARAMTERS) {
		auto primary_culture = ((nations::nation const*)primary_slot)->primary_culture;
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == trigger_payload(tval[2]).tag, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_from(TRIGGER_PARAMTERS) {
		auto primary_culture = ((nations::nation const*)primary_slot)->primary_culture;
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == ((nations::nation const*)from_slot)->tag, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_this_nation(TRIGGER_PARAMTERS) {
		auto primary_culture = ((nations::nation const*)primary_slot)->primary_culture;
		if(is_valid_index(primary_culture)) {
			auto group = ws.s.culture_m.culture_container[primary_culture].group;
			return compare_values(tval[0], ws.s.culture_m.culture_groups[group].union_tag == ((nations::nation const*)this_slot)->tag, true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_this_culture_union_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_this_culture_union_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_nation(TRIGGER_PARAMTERS) {
		auto prim_culture = ((nations::nation const*)primary_slot)->primary_culture;
		auto this_culture = ((nations::nation const*)this_slot)->primary_culture;
		return compare_values(tval[0],
			is_valid_index(prim_culture) &&
			is_valid_index(this_culture) &&
			ws.s.culture_m.culture_container[prim_culture].group == ws.s.culture_m.culture_container[this_culture].group, true);
	}
	bool tf_this_culture_union_this_union_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_this_culture_union_this_union_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_this_culture_union_this_union_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_minorities_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			auto accepted_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, ((nations::nation const*)primary_slot)->primary_culture));
			auto accepted_range = get_range(ws.w.culture_s.culture_arrays, ((nations::nation const*)primary_slot)->accepted_cultures);
			for(auto c : accepted_range)
				accepted_pop += ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, c));
			return compare_values(tval[0], total_pop != accepted_pop, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_minorities_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(ws.w.nation_s.states.is_valid_index(id) && owner) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::total_population_tag);
			auto accepted_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, owner->primary_culture));
			auto accepted_range = get_range(ws.w.culture_s.culture_arrays, owner->accepted_cultures);
			for(auto c : accepted_range)
				accepted_pop += ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, c));
			return compare_values(tval[0], total_pop != accepted_pop, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_minorities_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner) {
			auto total_pop = ws.w.province_s.province_demographics.get(id, population::total_population_tag);
			auto accepted_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, owner->primary_culture));
			auto accepted_range = get_range(ws.w.culture_s.culture_arrays, owner->accepted_cultures);
			for(auto c : accepted_range)
				accepted_pop += ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, c));
			return compare_values(tval[0], total_pop != accepted_pop, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_revanchism_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->revanchism, read_float_from_payload(tval + 2));
	}
	bool tf_revanchism_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return compare_values(tval[0], owner->revanchism, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_has_crime(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(((provinces::province_state const*)primary_slot)->crime), true);
	}
	bool tf_num_of_substates(TRIGGER_PARAMTERS) {
		auto vassal_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->vassals);
		uint32_t count_substates = 0ui32;
		for(auto v : vassal_range) {
			if(is_valid_index(v) && 0 != (ws.w.nation_s.nations[v].flags & nations::nation::is_substate))
				++count_substates;
		}
		return compare_values(tval[0], count_substates, uint32_t(tval[2]));
	}
	bool tf_num_of_vassals_no_substates(TRIGGER_PARAMTERS) {
		auto vassal_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->vassals);
		uint32_t count_non_substates = 0ui32;
		for(auto v : vassal_range) {
			if(is_valid_index(v) && 0 == (ws.w.nation_s.nations[v].flags & nations::nation::is_substate))
				++count_non_substates;
		}
		return compare_values(tval[0], count_non_substates, uint32_t(tval[2]));
	}
	bool tf_brigades_compare_this(TRIGGER_PARAMTERS) {
		auto main_brigades = military::total_active_divisions(ws, *((nations::nation const*)primary_slot));
		auto this_brigades = military::total_active_divisions(ws, *((nations::nation const*)this_slot));
		return compare_values(tval[0],
			this_brigades != 0 ? float(main_brigades) / float(this_brigades) : 1'000'000.0f,
			read_float_from_payload(tval + 2));
	}
	bool tf_brigades_compare_from(TRIGGER_PARAMTERS) {
		auto main_brigades = military::total_active_divisions(ws, *((nations::nation const*)primary_slot));
		auto from_brigades = military::total_active_divisions(ws, *((nations::nation const*)from_slot));
		return compare_values(tval[0],
			from_brigades != 0 ? float(main_brigades) / float(from_brigades) : 1'000'000.0f,
			read_float_from_payload(tval + 2));
	}
	bool tf_constructing_cb_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_target == tag_holder->id, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_target == ((nations::nation const*)from_slot)->id, true);
	}
	bool tf_constructing_cb_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_target == ((nations::nation const*)this_slot)->id, true);
	}
	bool tf_constructing_cb_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_target == owner->id, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_target == owner->id, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_target == owner->id, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_constructing_cb_discovered(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_cb_construction_discovered), true);
	}
	bool tf_constructing_cb_progress(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_progress, read_float_from_payload(tval + 2));
	}
	bool tf_civilization_progress(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->modifier_values[modifiers::national_offsets::civilization_progress_modifier], read_float_from_payload(tval + 2));
	}
	bool tf_constructing_cb_type(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->cb_construction_type == trigger_payload(tval[2]).small.values.cb_type, true);
	}
	bool tf_is_our_vassal_tag(TRIGGER_PARAMTERS) {
		auto tag_holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(tag_holder)
			return compare_values(tval[0], tag_holder->overlord == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)from_slot)->overlord == primary_slot, true);
	}
	bool tf_is_our_vassal_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)this_slot)->overlord == primary_slot, true);
	}
	bool tf_is_our_vassal_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->overlord == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->overlord == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_our_vassal_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], owner->overlord == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_substate(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], 0 != (((nations::nation const*)primary_slot)->flags & nations::nation::is_substate), true);
	}
	bool tf_great_wars_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.great_wars_enabled, true);
	}
	bool tf_can_nationalize(TRIGGER_PARAMTERS) {
		auto influencer_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->influencers);
		auto this_id = ((nations::nation const*)primary_slot)->id;
		for(auto c : influencer_range) {
			if(ws.w.nation_s.nations.is_valid_index(c) && nations::get_foreign_investment(ws, ws.w.nation_s.nations[c], this_id) != 0.0f)
				return compare_values(tval[0], true, true);
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_part_of_sphere(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->sphere_leader != nullptr, true);
	}
	bool tf_is_sphere_leader_of_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
		if(holder)
			return compare_values(tval[0], holder->sphere_leader == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_from(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)from_slot)->sphere_leader == primary_slot, true);
	}
	bool tf_is_sphere_leader_of_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)this_slot)->sphere_leader == primary_slot, true);
	}
	bool tf_is_sphere_leader_of_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->sphere_leader == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return compare_values(tval[0], owner->sphere_leader == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_sphere_leader_of_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return compare_values(tval[0], owner->sphere_leader == primary_slot, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_number_of_states(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], get_size(ws.w.nation_s.state_arrays, ((nations::nation const*)primary_slot)->member_states), uint32_t(tval[2]));
	}
	bool tf_war_score(TRIGGER_PARAMTERS) {
		//stub for apparently unused trigger
		return compare_values(tval[0], true, true);
	}
	bool tf_is_releasable_vassal_from(TRIGGER_PARAMTERS) {
		auto tag = ((nations::nation const*)from_slot)->tag;
		if(is_valid_index(tag))
			return compare_values(tval[0], !(ws.w.culture_s.national_tags_state[tag].is_not_releasable), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_is_releasable_vassal_other(TRIGGER_PARAMTERS) {
		auto tag = ((nations::nation const*)primary_slot)->tag;
		if(is_valid_index(tag))
			return compare_values(tval[0], !(ws.w.culture_s.national_tags_state[tag].is_not_releasable), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_recent_imigration(TRIGGER_PARAMTERS) {
		auto last_immigration = ((provinces::province_state const*)primary_slot)->last_immigration;
		if(!is_valid_index(last_immigration))
			return compare_values(tval[0], std::numeric_limits<uint16_t>::max(), tval[2]);
		else
			return compare_values(tval[0], to_index(ws.w.current_date) - to_index(last_immigration), int32_t(tval[2]));
	}
	bool tf_province_control_days(TRIGGER_PARAMTERS) {
		auto last_control_change = ((provinces::province_state const*)primary_slot)->last_controller_change;
		if(!is_valid_index(last_control_change))
			return compare_values(tval[0], std::numeric_limits<uint16_t>::max(), tval[2]);
		else
			return compare_values(tval[0], to_index(ws.w.current_date) - to_index(last_control_change), int32_t(tval[2]));
	}
	bool tf_is_disarmed(TRIGGER_PARAMTERS) {
		auto disarmed_date = ((nations::nation const*)primary_slot)->disarmed_until;
		return compare_values(tval[0], is_valid_index(disarmed_date) && ws.w.current_date < disarmed_date, true);
	}
	bool tf_big_producer(TRIGGER_PARAMTERS) {
		// stub: used only rarely in ai chances and would be expensive to test
		return compare_values(tval[0], false, true);
	}
	bool tf_someone_can_form_union_tag_from(TRIGGER_PARAMTERS) {
		// stub: apparently unused
		return compare_values(tval[0], false, true);
	}
	bool tf_someone_can_form_union_tag_other(TRIGGER_PARAMTERS) {
		// stub: apparently unused
		return compare_values(tval[0], false, true);
	}
	bool tf_social_movement_strength(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->social_movement_support, read_float_from_payload(tval + 2));
	}
	bool tf_political_movement_strength(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->political_movement_support, read_float_from_payload(tval + 2));
	}
	bool tf_can_build_factory_in_capital_state(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_social_movement(TRIGGER_PARAMTERS) {
		auto mt = ((population::pop const*)primary_slot)->movement;
		if(is_valid_index(mt))
			return compare_values(tval[0], ws.w.population_s.pop_movements[mt].type == population::movement_type::social, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_political_movement(TRIGGER_PARAMTERS) {
		auto mt = ((population::pop const*)primary_slot)->movement;
		if(is_valid_index(mt))
			return compare_values(tval[0], ws.w.population_s.pop_movements[mt].type == population::movement_type::political, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_cultural_sphere(TRIGGER_PARAMTERS) {
		auto prim_culture = ((nations::nation const*)primary_slot)->primary_culture;
		auto culture_group = is_valid_index(prim_culture) ? ws.s.culture_m.culture_container[prim_culture].group : cultures::culture_group_tag();
		auto sphere_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation const*)primary_slot)->sphere_members);
		for(auto c : sphere_range) {
			if(is_valid_index(c)) {
				auto sc = ws.w.nation_s.nations[c].primary_culture;
				if(is_valid_index(sc) && culture_group == ws.s.culture_m.culture_container[sc].group)
					return compare_values(tval[0], true, true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_world_wars_enabled(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.world_wars_enabled, true);
	}
	bool tf_has_pop_culture_pop_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->culture == ((population::pop const*)this_slot)->culture, true);
	}
	bool tf_has_pop_culture_state_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ((population::pop const*)this_slot)->culture;
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(is_valid_index(culture) && ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_province_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ((population::pop const*)this_slot)->culture;
		auto id = ((provinces::province_state const*)primary_slot)->id;
		if(is_valid_index(culture))
			return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_nation_this_pop(TRIGGER_PARAMTERS) {
		auto culture = ((population::pop const*)this_slot)->culture;
		auto id = ((nations::nation const*)primary_slot)->id;
		if(is_valid_index(culture) && ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->culture == trigger_payload(tval[2]).culture, true);
	}
	bool tf_has_pop_culture_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_culture_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
	}
	bool tf_has_pop_culture_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).culture)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_pop_this_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == ((population::pop const*)this_slot)->religion, true);
	}
	bool tf_has_pop_religion_state_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ((population::pop const*)this_slot)->religion;
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(is_valid_index(religion) && ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_province_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ((population::pop const*)this_slot)->religion;
		auto id = ((provinces::province_state const*)primary_slot)->id;
		if(is_valid_index(religion))
			return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_nation_this_pop(TRIGGER_PARAMTERS) {
		auto religion = ((population::pop const*)this_slot)->religion;
		auto id = ((nations::nation const*)primary_slot)->id;
		if(is_valid_index(religion) && ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->religion == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_has_pop_religion_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_pop_religion_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		return compare_values(tval[0], 0 != ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
	}
	bool tf_has_pop_religion_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], 0 != ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.religion)), true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_life_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->needs_satisfaction, read_float_from_payload(tval + 2));
	}
	bool tf_everyday_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->needs_satisfaction, read_float_from_payload(tval + 2) + 1.0f);
	}
	bool tf_luxury_needs(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((population::pop const*)primary_slot)->needs_satisfaction, read_float_from_payload(tval + 2) + 2.0f);
	}
	bool tf_consciousness_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], float(((population::pop const*)primary_slot)->consciousness) * 10.0f / float(std::numeric_limits<uint16_t>::max()), read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::total_population_tag);
		auto con = ws.w.province_s.province_demographics.get(id, population::consciousness_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) * 10.0f / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::total_population_tag);
			auto con = ws.w.nation_s.state_demographics.get(id, population::consciousness_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_consciousness_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::consciousness_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], float(((population::pop const*)primary_slot)->literacy) / float(std::numeric_limits<uint16_t>::max()), read_float_from_payload(tval + 2));
	}
	bool tf_literacy_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::total_population_tag);
		auto con = ws.w.province_s.province_demographics.get(id, population::literacy_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::total_population_tag);
			auto con = ws.w.nation_s.state_demographics.get(id, population::literacy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_literacy_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::literacy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_pop(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], float(((population::pop const*)primary_slot)->militancy) * 10.0f / float(std::numeric_limits<uint16_t>::max()), read_float_from_payload(tval + 2));
	}
	bool tf_militancy_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::total_population_tag);
		auto con = ws.w.province_s.province_demographics.get(id, population::militancy_demo_tag(ws));
		return compare_values(tval[0], total_pop != 0 ? (float(con) * 10.0f / float(total_pop)) : 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::total_population_tag);
			auto con = ws.w.nation_s.state_demographics.get(id, population::militancy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_militancy_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			auto con = ws.w.nation_s.nation_demographics.get(id, population::militancy_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(con) * 10.0f / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_trade_goods_in_state_state(TRIGGER_PARAMTERS) {
		auto state_region = ((nations::state_instance const*)primary_slot)->region_id;
		if(is_valid_index(state_region)) {
			auto prov_range = ws.s.province_m.states_to_province_index.get_row(state_region);
			for(auto p : prov_range) {
				auto& ps = ws.w.province_s.province_state_container[p];
				if(ps.state_instance == primary_slot && ps.rgo_production == trigger_payload(tval[2]).small.values.good)
					return compare_values(tval[0], true, true);
			}
		}
		return compare_values(tval[0], false, true);
	}
	bool tf_trade_goods_in_state_province(TRIGGER_PARAMTERS) {
		auto si = ((provinces::province_state const*)primary_slot)->state_instance;
		if(si)
			return tf_trade_goods_in_state_state(tval, ws, si, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_has_flashpoint(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], is_valid_index(((nations::state_instance const*)primary_slot)->crisis_tag), true);
	}
	bool tf_flashpoint_tension(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::state_instance const*)primary_slot)->current_tension, read_float_from_payload(tval + 2));
	}
	bool tf_crisis_exist(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.type != current_state::crisis_type::none, true);
	}
	bool tf_is_liberation_crisis(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.type == current_state::crisis_type::liberation, true);
	}
	bool tf_is_claim_crisis(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.type == current_state::crisis_type::claim, true);
	}
	bool tf_crisis_temperature(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.current_crisis.temperature, read_float_from_payload(tval + 2));
	}
	bool tf_involved_in_crisis_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		return compare_values(tval[0],
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers, id) ||
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders, id) ||
			contains_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, id),
			true);
	}
	bool tf_involved_in_crisis_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_involved_in_crisis_nation(nullptr, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_rich_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_rich_strata_life_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_rich_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::rich_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::rich_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::rich_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_rich_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_rich_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_middle_strata_life_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_middle_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::middle_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::middle_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::middle_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_middle_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_middle_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_life_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_life_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_poor_strata_life_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_everyday_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_everyday_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_poor_strata_everyday_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.nation_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::poor_population_demo_tag(ws));
			if(total_pop != 0)
				return compare_values(tval[0], float(ws.w.nation_s.state_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::poor_population_demo_tag(ws));
		if(total_pop != 0)
			return compare_values(tval[0], float(ws.w.province_s.province_demographics.get(id, population::poor_luxury_needs_demo_tag(ws))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_poor_strata_luxury_needs_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_poor_strata_luxury_needs_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_diplomatic_influence_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[3]).tag].holder;
		if(holder) 
			return compare_values(tval[0], nations::get_influence_value(ws, *((nations::nation const*)primary_slot), holder->id), int32_t(tval[2]));
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_diplomatic_influence_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_influence_value(ws, *((nations::nation const*)primary_slot), ((nations::nation const*)this_slot)->id), int32_t(tval[2]));
	}
	bool tf_diplomatic_influence_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_diplomatic_influence_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_diplomatic_influence_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_influence_value(ws, *((nations::nation const*)primary_slot), ((nations::nation const*)from_slot)->id), int32_t(tval[2]));
	}
	bool tf_diplomatic_influence_from_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)from_slot)->owner;
		if(owner)
			return tf_diplomatic_influence_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], 0ui16, tval[2]);
	}
	bool tf_pop_unemployment_nation(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_state(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_province(TRIGGER_PARAMTERS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type));
		if(total_pop != 0)
			return compare_values(tval[0], 1.0f - float(ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_pop(TRIGGER_PARAMTERS) {
		auto id = ((population::pop const*)primary_slot)->id;
		if(ws.w.population_s.pops.is_valid_index(id)) {
			auto total_size = ws.w.population_s.pop_demographics.get(id, population::total_population_tag);
			if(total_size != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.population_s.pop_demographics.get(id, population::total_employment_tag)) / float(total_size), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_nation_this_pop(TRIGGER_PARAMTERS) {
		auto type = ((population::pop const*)this_slot)->type;
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_state_this_pop(TRIGGER_PARAMTERS) {
		auto type = ((population::pop const*)this_slot)->type;
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type));
			if(total_pop != 0)
				return compare_values(tval[0], 1.0f - float(ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_pop_unemployment_province_this_pop(TRIGGER_PARAMTERS) {
		auto type = ((population::pop const*)this_slot)->type;
		auto id = ((provinces::province_state const*)primary_slot)->id;
		auto total_pop = ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type));
		if(total_pop != 0)
			return compare_values(tval[0], 1.0f - float(ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type))) / float(total_pop), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_relation_tag(TRIGGER_PARAMTERS) {
		auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[3]).tag].holder;
		if(holder)
			return compare_values(tval[0], nations::get_relationship(ws, *((nations::nation const*)primary_slot), holder->id), int32_t(trigger_payload(tval[2]).signed_value));
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_relation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_relationship(ws, *((nations::nation const*)primary_slot), ((nations::nation const*)this_slot)->id), int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_relation_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_relation_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_relation_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], nations::get_relationship(ws, *((nations::nation const*)primary_slot), ((nations::nation const*)from_slot)->id), int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_relation_from_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)from_slot)->owner;
		if(owner)
			return tf_relation_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr);
		else
			return compare_values(tval[0], 0i16, trigger_payload(tval[2]).signed_value);
	}
	bool tf_check_variable(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], ws.w.nation_s.national_variables.get(id, triggers::trigger_payload(tval[3]).nat_var), read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_upper_house(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.upper_house.get(id, triggers::trigger_payload(tval[3]).small.values.ideology)) / 100.0f, read_float_from_payload(tval + 2));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_unemployment_by_type_nation(TRIGGER_PARAMTERS) {
		return tf_pop_unemployment_nation(tval, ws, primary_slot, nullptr, nullptr, nullptr);
	}
	bool tf_unemployment_by_type_state(TRIGGER_PARAMTERS) {
		return tf_pop_unemployment_state(tval, ws, primary_slot, nullptr, nullptr, nullptr);
	}
	bool tf_unemployment_by_type_province(TRIGGER_PARAMTERS) {
		return tf_pop_unemployment_province(tval, ws, primary_slot, nullptr, nullptr, nullptr);
	}
	bool tf_unemployment_by_type_pop(TRIGGER_PARAMTERS) {
		auto location = ((population::pop const*)primary_slot)->location;
		if(is_valid_index(location)) {
			auto si = ws.w.province_s.province_state_container[location].state_instance;
			if(si)
				return tf_unemployment_by_type_state(tval, ws, si, nullptr, nullptr, nullptr);
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_nation_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_from_nation_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_province_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_from_province_province_id(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(provinces::province_tag(tval[2]), trigger_payload(tval[5]).small.values.ideology), read_float_from_payload(tval + 3));
	}
	bool tf_party_loyalty_nation_from_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(((provinces::province_state const*)from_slot)->id, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_generic(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(((provinces::province_state const*)primary_slot)->id, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_from_nation_scope_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(((provinces::province_state const*)primary_slot)->id, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_party_loyalty_from_province_scope_province(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ws.w.province_s.party_loyalty.get(((provinces::province_state const*)primary_slot)->id, trigger_payload(tval[4]).small.values.ideology), read_float_from_payload(tval + 2));
	}
	bool tf_can_build_in_province_railroad_no_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->railroad_level) +
			((provinces::province_state const*)primary_slot)->modifier_values[modifiers::provincial_offsets::min_build_railroad] <
				((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_railroad],
			true);
	}
	bool tf_can_build_in_province_railroad_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->railroad_level) +
			((provinces::province_state const*)primary_slot)->modifier_values[modifiers::provincial_offsets::min_build_railroad] <
				((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_railroad],
			true);
	}
	bool tf_can_build_in_province_railroad_no_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->railroad_level) +
			((provinces::province_state const*)primary_slot)->modifier_values[modifiers::provincial_offsets::min_build_railroad] <
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_railroad],
			true);
	}
	bool tf_can_build_in_province_railroad_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->railroad_level) +
			((provinces::province_state const*)primary_slot)->modifier_values[modifiers::provincial_offsets::min_build_railroad] <
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_railroad],
			true);
	}
	bool tf_can_build_in_province_fort_no_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->fort_level) <
				((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_fort],
			true);
	}
	bool tf_can_build_in_province_fort_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->fort_level) <
				((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_fort],
			true);
	}
	bool tf_can_build_in_province_fort_no_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->fort_level) <
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_fort],
			true);
	}
	bool tf_can_build_in_province_fort_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0],
			technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->fort_level) <
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_fort],
			true);
	}
	bool tf_can_build_in_province_naval_base_no_limit_from_nation(TRIGGER_PARAMTERS) {
		if(((provinces::province_state const*)primary_slot)->naval_base_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->naval_base_level) <
					((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_naval_base],
				true);
		} else if(auto si = ((provinces::province_state const*)primary_slot)->state_instance; si) {
			return compare_values(tval[0],
				(si->flags & nations::state_instance::contains_naval_base) == 0 &&
				(ws.s.province_m.province_container[((provinces::province_state const*)primary_slot)->id].flags & provinces::province::coastal) != 0 &&
				((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_naval_base] >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_yes_limit_from_nation(TRIGGER_PARAMTERS) {
		if(((provinces::province_state const*)primary_slot)->naval_base_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->naval_base_level) <
				((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_naval_base],
				true);
		} else if(auto si = ((provinces::province_state const*)primary_slot)->state_instance; si) {
			return compare_values(tval[0],
				(si->flags & nations::state_instance::contains_naval_base) == 0 &&
				(ws.s.province_m.province_container[((provinces::province_state const*)primary_slot)->id].flags & provinces::province::coastal) != 0 &&
				((nations::nation const*)from_slot)->tech_attributes[technologies::tech_offset::max_naval_base] >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_no_limit_this_nation(TRIGGER_PARAMTERS) {
		if(((provinces::province_state const*)primary_slot)->naval_base_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->naval_base_level) <
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_naval_base],
				true);
		} else if(auto si = ((provinces::province_state const*)primary_slot)->state_instance; si) {
			return compare_values(tval[0],
				(si->flags & nations::state_instance::contains_naval_base) == 0 &&
				(ws.s.province_m.province_container[((provinces::province_state const*)primary_slot)->id].flags & provinces::province::coastal) != 0 &&
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_naval_base] >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_in_province_naval_base_yes_limit_this_nation(TRIGGER_PARAMTERS) {
		if(((provinces::province_state const*)primary_slot)->naval_base_level != 0ui8) {
			return compare_values(tval[0],
				technologies::tech_attribute_type(((provinces::province_state const*)primary_slot)->naval_base_level) <
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_naval_base],
				true);
		} else if(auto si = ((provinces::province_state const*)primary_slot)->state_instance; si) {
			return compare_values(tval[0],
				(si->flags & nations::state_instance::contains_naval_base) == 0 &&
				(ws.s.province_m.province_container[((provinces::province_state const*)primary_slot)->id].flags & provinces::province::coastal) != 0 &&
				((nations::nation const*)this_slot)->tech_attributes[technologies::tech_offset::max_naval_base] >= technologies::tech_attribute_type(1),
				true);
		} else {
			return compare_values(tval[0], false, true);
		}
	}
	bool tf_can_build_railway_in_capital_yes_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_railway_in_capital_yes_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_railway_in_capital_no_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_railway_in_capital_no_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_yes_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_yes_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_no_whole_state_yes_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_can_build_fort_in_capital_no_whole_state_no_limit(TRIGGER_PARAMTERS) {
		// stub: virtually unused
		return compare_values(tval[0], true, true);
	}
	bool tf_work_available_nation(TRIGGER_PARAMTERS) {
		auto count_workers = tval[1] - 1;
		auto id = ((nations::nation const*)primary_slot)->id;

		if(!ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], false, true);

		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			if(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, type)) != 0 &&
				ws.w.nation_s.nation_demographics.get(id, population::to_employment_demo_tag(ws, type)) == 0) {
				return compare_values(tval[0], false, true);
			}
		}
		return compare_values(tval[0], true, true);
	}
	bool tf_work_available_state(TRIGGER_PARAMTERS) {
		auto count_workers = tval[1] - 1;
		auto id = ((nations::state_instance const*)primary_slot)->id;

		if(!ws.w.nation_s.states.is_valid_index(id))
			return compare_values(tval[0], false, true);

		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			if(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, type)) != 0 &&
				ws.w.nation_s.state_demographics.get(id, population::to_employment_demo_tag(ws, type)) == 0) {
				return compare_values(tval[0], false, true);
			}
		}
		return compare_values(tval[0], true, true);
	}
	bool tf_work_available_province(TRIGGER_PARAMTERS) {
		auto count_workers = tval[1] - 1;
		auto id = ((provinces::province_state const*)primary_slot)->id;

		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			if(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, type)) != 0 &&
				ws.w.province_s.province_demographics.get(id, population::to_employment_demo_tag(ws, type)) == 0) {
				return compare_values(tval[0], false, true);
			}
		}
		return compare_values(tval[0], true, true);
	}
	bool tf_variable_ideology_name_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_ideology_name_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_ideology_name_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;

		auto total_pop = ws.w.province_s.province_demographics.get(id, population::total_population_tag);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_ideology_name_pop(TRIGGER_PARAMTERS) {
		auto id = ((population::pop const*)primary_slot)->id;
		if(ws.w.population_s.pops.is_valid_index(id)) {
			auto total_pop = ws.w.population_s.pop_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.population_s.pop_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;

		auto total_pop = ws.w.province_s.province_demographics.get(id, population::total_population_tag);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_name_pop(TRIGGER_PARAMTERS) {
		auto id = ((population::pop const*)primary_slot)->id;
		if(ws.w.population_s.pops.is_valid_index(id)) {
			auto total_pop = ws.w.population_s.pop_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.population_s.pop_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_issue_group_name_nation(TRIGGER_PARAMTERS) {
		auto option = trigger_payload(tval[2]).small.values.option;
		auto issue = ws.s.issues_m.options[option].parent_issue;

		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], ws.w.nation_s.active_issue_options.get(id, issue) == option, true);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)primary_slot)->owner;
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)primary_slot)->owner;
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_issue_group_name_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)primary_slot));
		if(owner)
			return tf_variable_issue_group_name_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_variable_pop_type_name_nation(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.nation_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.nation_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_pop_type_name_state(TRIGGER_PARAMTERS) {
		auto id = ((nations::state_instance const*)primary_slot)->id;
		if(ws.w.nation_s.states.is_valid_index(id)) {
			auto total_pop = ws.w.nation_s.state_demographics.get(id, population::total_population_tag);
			if(total_pop != 0)
				return compare_values(tval[0],
					float(ws.w.nation_s.state_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type))) / float(total_pop),
					read_float_from_payload(tval + 3));
		}
		return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_pop_type_name_province(TRIGGER_PARAMTERS) {
		auto id = ((provinces::province_state const*)primary_slot)->id;

		auto total_pop = ws.w.province_s.province_demographics.get(id, population::total_population_tag);
		if(total_pop != 0)
			return compare_values(tval[0],
				float(ws.w.province_s.province_demographics.get(id, population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.pop_type))) / float(total_pop),
				read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_pop_type_name_pop(TRIGGER_PARAMTERS) {
		if(((population::pop const*)primary_slot)->type == trigger_payload(tval[2]).small.values.pop_type)
			return compare_values(tval[0], 1.0f, read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_variable_good_name(TRIGGER_PARAMTERS) {
		auto id = ((nations::nation const*)primary_slot)->id;
		if(ws.w.nation_s.nations.is_valid_index(id))
			return compare_values(tval[0], float(ws.w.nation_s.national_stockpiles.get(id, trigger_payload(tval[2]).small.values.good)), read_float_from_payload(tval + 3));
		else
			return compare_values(tval[0], 0.0f, read_float_from_payload(tval + 3));
	}
	bool tf_religion_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->national_religion == trigger_payload(tval[2]).small.values.religion, true);
	}
	bool tf_religion_nation_reb(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->national_religion == rebel_slot->religion, true);
	}
	bool tf_religion_nation_from_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->national_religion == ((nations::nation const*)from_slot)->national_religion, true);
	}
	bool tf_religion_nation_this_nation(TRIGGER_PARAMTERS) {
		return compare_values(tval[0], ((nations::nation const*)primary_slot)->national_religion == ((nations::nation const*)this_slot)->national_religion, true);
	}
	bool tf_religion_nation_this_state(TRIGGER_PARAMTERS) {
		auto owner = ((nations::state_instance const*)this_slot)->owner;
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_nation_this_province(TRIGGER_PARAMTERS) {
		auto owner = ((provinces::province_state const*)this_slot)->owner;
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}
	bool tf_religion_nation_this_pop(TRIGGER_PARAMTERS) {
		auto owner = population::get_pop_owner(ws, *((population::pop const*)this_slot));
		if(owner)
			return tf_religion_nation_this_nation(tval, ws, owner, nullptr, nullptr, nullptr);
		else
			return compare_values(tval[0], false, true);
	}

	static bool(*trigger_functions[])(TRIGGER_PARAMTERS) = {
		tf_none,
		tf_year,
		tf_month,
		tf_port,
		tf_rank,
		tf_technology,
		tf_strata_rich,
		tf_life_rating_province,
		tf_life_rating_state,
		tf_has_empty_adjacent_state_province,
		tf_has_empty_adjacent_state_state,
		tf_state_id_province,
		tf_state_id_state,
		tf_cash_reserves,
		tf_unemployment_nation,
		tf_unemployment_state,
		tf_unemployment_province,
		tf_unemployment_pop,
		tf_is_slave_nation,
		tf_is_slave_state,
		tf_is_slave_province,
		tf_is_slave_pop,
		tf_is_independant,
		tf_has_national_minority_province,
		tf_has_national_minority_state,
		tf_has_national_minority_nation,
		tf_government_nation,
		tf_government_pop,
		tf_capital,
		tf_tech_school,
		tf_primary_culture,
		tf_accepted_culture,
		tf_culture_pop,
		tf_culture_state,
		tf_culture_province,
		tf_culture_nation,
		tf_culture_pop_reb,
		tf_culture_state_reb,
		tf_culture_province_reb,
		tf_culture_nation_reb,
		tf_culture_from_nation,
		tf_culture_this_nation,
		tf_culture_this_state,
		tf_culture_this_pop,
		tf_culture_this_province,
		tf_culture_group_nation,
		tf_culture_group_pop,
		tf_culture_group_reb_nation,
		tf_culture_group_reb_pop,
		tf_culture_group_nation_from_nation,
		tf_culture_group_pop_from_nation,
		tf_culture_group_nation_this_nation,
		tf_culture_group_pop_this_nation,
		tf_culture_group_nation_this_province,
		tf_culture_group_pop_this_province,
		tf_culture_group_nation_this_state,
		tf_culture_group_pop_this_state,
		tf_culture_group_nation_this_pop,
		tf_culture_group_pop_this_pop,
		tf_religion,
		tf_religion_reb,
		tf_religion_from_nation,
		tf_religion_this_nation,
		tf_religion_this_state,
		tf_religion_this_province,
		tf_religion_this_pop,
		tf_terrain_province,
		tf_terrain_pop,
		tf_trade_goods,
		tf_is_secondary_power_pop,
		tf_is_secondary_power_nation,
		tf_has_faction_nation,
		tf_has_faction_pop,
		tf_has_unclaimed_cores,
		tf_is_cultural_union_bool,
		tf_is_cultural_union_this_self_pop,
		tf_is_cultural_union_this_pop,
		tf_is_cultural_union_this_state,
		tf_is_cultural_union_this_province,
		tf_is_cultural_union_this_nation,
		tf_is_cultural_union_this_rebel,
		tf_is_cultural_union_tag_nation,
		tf_is_cultural_union_tag_this_pop,
		tf_is_cultural_union_tag_this_state,
		tf_is_cultural_union_tag_this_province,
		tf_is_cultural_union_tag_this_nation,
		tf_can_build_factory_pop,
		tf_war_pop,
		tf_war_nation,
		tf_war_exhaustion_nation,
		tf_blockade,
		tf_owns,
		tf_controls,
		tf_is_core_integer,
		tf_is_core_this_nation,
		tf_is_core_this_state,
		tf_is_core_this_province,
		tf_is_core_this_pop,
		tf_is_core_from_nation,
		tf_is_core_reb,
		tf_is_core_tag,
		tf_num_of_revolts,
		tf_revolt_percentage,
		tf_num_of_cities_int,
		tf_num_of_cities_from_nation,
		tf_num_of_cities_this_nation,
		tf_num_of_cities_this_state,
		tf_num_of_cities_this_province,
		tf_num_of_cities_this_pop,
		tf_num_of_ports,
		tf_num_of_allies,
		tf_num_of_vassals,
		tf_owned_by_tag,
		tf_owned_by_from_nation,
		tf_owned_by_this_nation,
		tf_owned_by_this_province,
		tf_owned_by_this_state,
		tf_owned_by_this_pop,
		tf_exists_bool,
		tf_exists_tag,
		tf_has_country_flag,
		tf_continent_nation,
		tf_continent_state,
		tf_continent_province,
		tf_continent_pop,
		tf_continent_nation_this,
		tf_continent_state_this,
		tf_continent_province_this,
		tf_continent_pop_this,
		tf_continent_nation_from,
		tf_continent_state_from,
		tf_continent_province_from,
		tf_continent_pop_from,
		tf_casus_belli_tag,
		tf_casus_belli_from,
		tf_casus_belli_this_nation,
		tf_casus_belli_this_state,
		tf_casus_belli_this_province,
		tf_casus_belli_this_pop,
		tf_military_access_tag,
		tf_military_access_from,
		tf_military_access_this_nation,
		tf_military_access_this_state,
		tf_military_access_this_province,
		tf_military_access_this_pop,
		tf_prestige_value,
		tf_prestige_from,
		tf_prestige_this_nation,
		tf_prestige_this_state,
		tf_prestige_this_province,
		tf_prestige_this_pop,
		tf_badboy,
		tf_has_building_state,
		tf_has_building_fort,
		tf_has_building_railroad,
		tf_has_building_naval_base,
		tf_empty,
		tf_is_blockaded,
		tf_has_country_modifier,
		tf_has_province_modifier,
		tf_region,
		tf_tag_tag,
		tf_tag_this_nation,
		tf_tag_this_province,
		tf_tag_from_nation,
		tf_tag_from_province,
		tf_neighbour_tag,
		tf_neighbour_this,
		tf_neighbour_from,
		tf_units_in_province_value,
		tf_units_in_province_from,
		tf_units_in_province_this_nation,
		tf_units_in_province_this_province,
		tf_units_in_province_this_state,
		tf_units_in_province_this_pop,
		tf_war_with_tag,
		tf_war_with_from,
		tf_war_with_this_nation,
		tf_war_with_this_province,
		tf_war_with_this_state,
		tf_war_with_this_pop,
		tf_unit_in_battle,
		tf_total_amount_of_divisions,
		tf_money,
		tf_lost_national,
		tf_is_vassal,
		tf_ruling_party_ideology_pop,
		tf_ruling_party_ideology_nation,
		tf_ruling_party,
		tf_is_ideology_enabled,
		tf_political_reform_want_nation,
		tf_political_reform_want_pop,
		tf_social_reform_want_nation,
		tf_social_reform_want_pop,
		tf_total_amount_of_ships,
		tf_plurality,
		tf_corruption,
		tf_is_state_religion_pop,
		tf_is_state_religion_province,
		tf_is_state_religion_state,
		tf_is_primary_culture_pop,
		tf_is_primary_culture_province,
		tf_is_primary_culture_state,
		tf_is_primary_culture_nation_this_pop,
		tf_is_primary_culture_nation_this_nation,
		tf_is_primary_culture_nation_this_state,
		tf_is_primary_culture_nation_this_province,
		tf_is_primary_culture_state_this_pop,
		tf_is_primary_culture_state_this_nation,
		tf_is_primary_culture_state_this_state,
		tf_is_primary_culture_state_this_province,
		tf_is_primary_culture_province_this_pop,
		tf_is_primary_culture_province_this_nation,
		tf_is_primary_culture_province_this_state,
		tf_is_primary_culture_province_this_province,
		tf_is_primary_culture_pop_this_pop,
		tf_is_primary_culture_pop_this_nation,
		tf_is_primary_culture_pop_this_state,
		tf_is_primary_culture_pop_this_province,
		tf_is_accepted_culture_pop,
		tf_is_accepted_culture_province,
		tf_is_accepted_culture_state,
		tf_is_coastal,
		tf_in_sphere_tag,
		tf_in_sphere_from,
		tf_in_sphere_this_nation,
		tf_in_sphere_this_province,
		tf_in_sphere_this_state,
		tf_in_sphere_this_pop,
		tf_produces_nation,
		tf_produces_state,
		tf_produces_province,
		tf_produces_pop,
		tf_average_militancy_nation,
		tf_average_militancy_state,
		tf_average_militancy_province,
		tf_average_consciousness_nation,
		tf_average_consciousness_state,
		tf_average_consciousness_province,
		tf_is_next_reform_nation,
		tf_is_next_reform_pop,
		tf_rebel_power_fraction,
		tf_recruited_percentage_nation,
		tf_recruited_percentage_pop,
		tf_has_culture_core,
		tf_nationalism,
		tf_is_overseas,
		tf_controlled_by_rebels,
		tf_controlled_by_tag,
		tf_controlled_by_from,
		tf_controlled_by_this_nation,
		tf_controlled_by_this_province,
		tf_controlled_by_this_state,
		tf_controlled_by_this_pop,
		tf_controlled_by_owner,
		tf_controlled_by_reb,
		tf_is_canal_enabled,
		tf_is_state_capital,
		tf_truce_with_tag,
		tf_truce_with_from,
		tf_truce_with_this_nation,
		tf_truce_with_this_province,
		tf_truce_with_this_state,
		tf_truce_with_this_pop,
		tf_total_pops_nation,
		tf_total_pops_state,
		tf_total_pops_province,
		tf_total_pops_pop,
		tf_has_pop_type_nation,
		tf_has_pop_type_state,
		tf_has_pop_type_province,
		tf_has_pop_type_pop,
		tf_has_empty_adjacent_province,
		tf_has_leader,
		tf_ai,
		tf_can_create_vassals,
		tf_is_possible_vassal,
		tf_province_id,
		tf_vassal_of_tag,
		tf_vassal_of_from,
		tf_vassal_of_this_nation,
		tf_vassal_of_this_province,
		tf_vassal_of_this_state,
		tf_vassal_of_this_pop,
		tf_alliance_with_tag,
		tf_alliance_with_from,
		tf_alliance_with_this_nation,
		tf_alliance_with_this_province,
		tf_alliance_with_this_state,
		tf_alliance_with_this_pop,
		tf_has_recently_lost_war,
		tf_is_mobilised,
		tf_mobilisation_size,
		tf_crime_higher_than_education_nation,
		tf_crime_higher_than_education_state,
		tf_crime_higher_than_education_province,
		tf_crime_higher_than_education_pop,
		tf_agree_with_ruling_party,
		tf_is_colonial_state,
		tf_is_colonial_province,
		tf_has_factories,
		tf_in_default_tag,
		tf_in_default_from,
		tf_in_default_this_nation,
		tf_in_default_this_province,
		tf_in_default_this_state,
		tf_in_default_this_pop,
		tf_total_num_of_ports,
		tf_always,
		tf_election,
		tf_has_global_flag,
		tf_is_capital,
		tf_nationalvalue_nation,
		tf_industrial_score_value,
		tf_industrial_score_from_nation,
		tf_industrial_score_this_nation,
		tf_industrial_score_this_pop,
		tf_industrial_score_this_state,
		tf_industrial_score_this_province,
		tf_military_score_value,
		tf_military_score_from_nation,
		tf_military_score_this_nation,
		tf_military_score_this_pop,
		tf_military_score_this_state,
		tf_military_score_this_province,
		tf_civilized_nation,
		tf_civilized_pop,
		tf_civilized_province,
		tf_national_provinces_occupied,
		tf_is_greater_power_nation,
		tf_is_greater_power_pop,
		tf_rich_tax,
		tf_middle_tax,
		tf_poor_tax,
		tf_social_spending_nation,
		tf_social_spending_pop,
		tf_social_spending_province,
		tf_colonial_nation,
		tf_pop_majority_religion_nation,
		tf_pop_majority_religion_state,
		tf_pop_majority_religion_province,
		tf_pop_majority_culture_nation,
		tf_pop_majority_culture_state,
		tf_pop_majority_culture_province,
		tf_pop_majority_issue_nation,
		tf_pop_majority_issue_state,
		tf_pop_majority_issue_province,
		tf_pop_majority_issue_pop,
		tf_pop_majority_ideology_nation,
		tf_pop_majority_ideology_state,
		tf_pop_majority_ideology_province,
		tf_pop_majority_ideology_pop,
		tf_poor_strata_militancy_nation,
		tf_poor_strata_militancy_state,
		tf_poor_strata_militancy_province,
		tf_poor_strata_militancy_pop,
		tf_middle_strata_militancy_nation,
		tf_middle_strata_militancy_state,
		tf_middle_strata_militancy_province,
		tf_middle_strata_militancy_pop,
		tf_rich_strata_militancy_nation,
		tf_rich_strata_militancy_state,
		tf_rich_strata_militancy_province,
		tf_rich_strata_militancy_pop,
		tf_rich_tax_above_poor,
		tf_culture_has_union_tag_pop,
		tf_culture_has_union_tag_nation,
		tf_this_culture_union_tag,
		tf_this_culture_union_from,
		tf_this_culture_union_this_nation,
		tf_this_culture_union_this_province,
		tf_this_culture_union_this_state,
		tf_this_culture_union_this_pop,
		tf_this_culture_union_this_union_nation,
		tf_this_culture_union_this_union_province,
		tf_this_culture_union_this_union_state,
		tf_this_culture_union_this_union_pop,
		tf_minorities_nation,
		tf_minorities_state,
		tf_minorities_province,
		tf_revanchism_nation,
		tf_revanchism_pop,
		tf_has_crime,
		tf_num_of_substates,
		tf_num_of_vassals_no_substates,
		tf_brigades_compare_this,
		tf_brigades_compare_from,
		tf_constructing_cb_tag,
		tf_constructing_cb_from,
		tf_constructing_cb_this_nation,
		tf_constructing_cb_this_province,
		tf_constructing_cb_this_state,
		tf_constructing_cb_this_pop,
		tf_constructing_cb_discovered,
		tf_constructing_cb_progress,
		tf_civilization_progress,
		tf_constructing_cb_type,
		tf_is_our_vassal_tag,
		tf_is_our_vassal_from,
		tf_is_our_vassal_this_nation,
		tf_is_our_vassal_this_province,
		tf_is_our_vassal_this_state,
		tf_is_our_vassal_this_pop,
		tf_substate_of_tag,
		tf_substate_of_from,
		tf_substate_of_this_nation,
		tf_substate_of_this_province,
		tf_substate_of_this_state,
		tf_substate_of_this_pop,
		tf_is_substate,
		tf_great_wars_enabled,
		tf_can_nationalize,
		tf_part_of_sphere,
		tf_is_sphere_leader_of_tag,
		tf_is_sphere_leader_of_from,
		tf_is_sphere_leader_of_this_nation,
		tf_is_sphere_leader_of_this_province,
		tf_is_sphere_leader_of_this_state,
		tf_is_sphere_leader_of_this_pop,
		tf_number_of_states,
		tf_war_score,
		tf_is_releasable_vassal_from,
		tf_is_releasable_vassal_other,
		tf_has_recent_imigration,
		tf_province_control_days,
		tf_is_disarmed,
		tf_big_producer,
		tf_someone_can_form_union_tag_from,
		tf_someone_can_form_union_tag_other,
		tf_social_movement_strength,
		tf_political_movement_strength,
		tf_can_build_factory_in_capital_state,
		tf_social_movement,
		tf_political_movement,
		tf_has_cultural_sphere,
		tf_world_wars_enabled,
		tf_has_pop_culture_pop_this_pop,
		tf_has_pop_culture_state_this_pop,
		tf_has_pop_culture_province_this_pop,
		tf_has_pop_culture_nation_this_pop,
		tf_has_pop_culture_pop,
		tf_has_pop_culture_state,
		tf_has_pop_culture_province,
		tf_has_pop_culture_nation,
		tf_has_pop_religion_pop_this_pop,
		tf_has_pop_religion_state_this_pop,
		tf_has_pop_religion_province_this_pop,
		tf_has_pop_religion_nation_this_pop,
		tf_has_pop_religion_pop,
		tf_has_pop_religion_state,
		tf_has_pop_religion_province,
		tf_has_pop_religion_nation,
		tf_life_needs,
		tf_everyday_needs,
		tf_luxury_needs,
		tf_consciousness_pop,
		tf_consciousness_province,
		tf_consciousness_state,
		tf_consciousness_nation,
		tf_literacy_pop,
		tf_literacy_province,
		tf_literacy_state,
		tf_literacy_nation,
		tf_militancy_pop,
		tf_militancy_province,
		tf_militancy_state,
		tf_militancy_nation,
		tf_military_spending_pop,
		tf_military_spending_province,
		tf_military_spending_state,
		tf_military_spending_nation,
		tf_administration_spending_pop,
		tf_administration_spending_province,
		tf_administration_spending_state,
		tf_administration_spending_nation,
		tf_education_spending_pop,
		tf_education_spending_province,
		tf_education_spending_state,
		tf_education_spending_nation,
		tf_trade_goods_in_state_state,
		tf_trade_goods_in_state_province,
		tf_has_flashpoint,
		tf_flashpoint_tension,
		tf_crisis_exist,
		tf_is_liberation_crisis,
		tf_is_claim_crisis,
		tf_crisis_temperature,
		tf_involved_in_crisis_pop,
		tf_involved_in_crisis_nation,
		tf_rich_strata_life_needs_nation,
		tf_rich_strata_life_needs_state,
		tf_rich_strata_life_needs_province,
		tf_rich_strata_life_needs_pop,
		tf_rich_strata_everyday_needs_nation,
		tf_rich_strata_everyday_needs_state,
		tf_rich_strata_everyday_needs_province,
		tf_rich_strata_everyday_needs_pop,
		tf_rich_strata_luxury_needs_nation,
		tf_rich_strata_luxury_needs_state,
		tf_rich_strata_luxury_needs_province,
		tf_rich_strata_luxury_needs_pop,
		tf_middle_strata_life_needs_nation,
		tf_middle_strata_life_needs_state,
		tf_middle_strata_life_needs_province,
		tf_middle_strata_life_needs_pop,
		tf_middle_strata_everyday_needs_nation,
		tf_middle_strata_everyday_needs_state,
		tf_middle_strata_everyday_needs_province,
		tf_middle_strata_everyday_needs_pop,
		tf_middle_strata_luxury_needs_nation,
		tf_middle_strata_luxury_needs_state,
		tf_middle_strata_luxury_needs_province,
		tf_middle_strata_luxury_needs_pop,
		tf_poor_strata_life_needs_nation,
		tf_poor_strata_life_needs_state,
		tf_poor_strata_life_needs_province,
		tf_poor_strata_life_needs_pop,
		tf_poor_strata_everyday_needs_nation,
		tf_poor_strata_everyday_needs_state,
		tf_poor_strata_everyday_needs_province,
		tf_poor_strata_everyday_needs_pop,
		tf_poor_strata_luxury_needs_nation,
		tf_poor_strata_luxury_needs_state,
		tf_poor_strata_luxury_needs_province,
		tf_poor_strata_luxury_needs_pop,
		tf_diplomatic_influence_tag,
		tf_diplomatic_influence_this_nation,
		tf_diplomatic_influence_this_province,
		tf_diplomatic_influence_from_nation,
		tf_diplomatic_influence_from_province,
		tf_pop_unemployment_nation,
		tf_pop_unemployment_state,
		tf_pop_unemployment_province,
		tf_pop_unemployment_pop,
		tf_pop_unemployment_nation_this_pop,
		tf_pop_unemployment_state_this_pop,
		tf_pop_unemployment_province_this_pop,
		tf_relation_tag,
		tf_relation_this_nation,
		tf_relation_this_province,
		tf_relation_from_nation,
		tf_relation_from_province,
		tf_check_variable,
		tf_upper_house,
		tf_unemployment_by_type_nation,
		tf_unemployment_by_type_state,
		tf_unemployment_by_type_province,
		tf_unemployment_by_type_pop,
		tf_party_loyalty_nation_province_id,
		tf_party_loyalty_from_nation_province_id,
		tf_party_loyalty_province_province_id,
		tf_party_loyalty_from_province_province_id,
		tf_party_loyalty_nation_from_province,
		tf_party_loyalty_from_nation_scope_province,
		tf_can_build_in_province_railroad_no_limit_from_nation,
		tf_can_build_in_province_railroad_yes_limit_from_nation,
		tf_can_build_in_province_railroad_no_limit_this_nation,
		tf_can_build_in_province_railroad_yes_limit_this_nation,
		tf_can_build_in_province_fort_no_limit_from_nation,
		tf_can_build_in_province_fort_yes_limit_from_nation,
		tf_can_build_in_province_fort_no_limit_this_nation,
		tf_can_build_in_province_fort_yes_limit_this_nation,
		tf_can_build_in_province_naval_base_no_limit_from_nation,
		tf_can_build_in_province_naval_base_yes_limit_from_nation,
		tf_can_build_in_province_naval_base_no_limit_this_nation,
		tf_can_build_in_province_naval_base_yes_limit_this_nation,
		tf_can_build_railway_in_capital_yes_whole_state_yes_limit,
		tf_can_build_railway_in_capital_yes_whole_state_no_limit,
		tf_can_build_railway_in_capital_no_whole_state_yes_limit,
		tf_can_build_railway_in_capital_no_whole_state_no_limit,
		tf_can_build_fort_in_capital_yes_whole_state_yes_limit,
		tf_can_build_fort_in_capital_yes_whole_state_no_limit,
		tf_can_build_fort_in_capital_no_whole_state_yes_limit,
		tf_can_build_fort_in_capital_no_whole_state_no_limit,
		tf_work_available_nation,
		tf_work_available_state,
		tf_work_available_province,
		tf_variable_ideology_name_nation,
		tf_variable_ideology_name_state,
		tf_variable_ideology_name_province,
		tf_variable_ideology_name_pop,
		tf_variable_issue_name_nation,
		tf_variable_issue_name_state,
		tf_variable_issue_name_province,
		tf_variable_issue_name_pop,
		tf_variable_issue_group_name_nation,
		tf_variable_issue_group_name_state,
		tf_variable_issue_group_name_province,
		tf_variable_issue_group_name_pop,
		tf_variable_pop_type_name_nation,
		tf_variable_pop_type_name_state,
		tf_variable_pop_type_name_province,
		tf_variable_pop_type_name_pop,
		tf_variable_good_name,
		tf_strata_middle,
		tf_strata_poor,
		tf_party_loyalty_from_province_scope_province,
		tf_can_build_factory_nation,
		tf_can_build_factory_province,
		tf_nationalvalue_pop,
		tf_nationalvalue_province,
		tf_war_exhaustion_pop,
		tf_has_culture_core_province_this_pop,
		tf_tag_pop,
		tf_has_country_flag_pop,
		tf_has_country_flag_province,
		tf_has_country_modifier_province,
		tf_religion_nation,
		tf_religion_nation_reb,
		tf_religion_nation_from_nation,
		tf_religion_nation_this_nation,
		tf_religion_nation_this_state,
		tf_religion_nation_this_province,
		tf_religion_nation_this_pop,
		tf_war_exhaustion_province,
		tf_is_greater_power_province,
		tf_is_cultural_union_pop_this_pop,
		tf_has_building_factory,
		tf_has_building_state_from_province,
		tf_has_building_factory_from_province,
		tf_party_loyalty_generic,
	};
	}

	bool test_trigger(TRIGGER_PARAMTERS) {
		if((*tval & trigger_codes::is_scope) != 0) {
			return scope_functions[*tval & trigger_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
		} else {
			return trigger_functions[*tval & trigger_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
		}
	}

#ifdef __llvm__
#pragma clang diagnostic pop
#endif

}
