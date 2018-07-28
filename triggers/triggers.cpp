#include "triggers.h"
#include "codes.h"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.h"
#include "military\\military_functions.h"
#include "population\\population_function.h"

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

	inline bool apply_disjuctively(uint16_t const* source, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		const auto source_size = 1 + get_trigger_payload_size(source);
		auto sub_units_start = source + 2 + trigger_scope_data_payload(source[0]);
		while(sub_units_start < source + source_size) {
			if(test_trigger(sub_units_start, ws, primary_slot, this_slot, from_slot, rebel_slot))
				return true;
			sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
		}
		return false;
	}

	inline bool apply_conjuctively(uint16_t const* source, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		const auto source_size = 1 + get_trigger_payload_size(source);
		auto sub_units_start = source + 2 + trigger_scope_data_payload(source[0]);
		while(sub_units_start < source + source_size) {
			if(!test_trigger(sub_units_start, ws, primary_slot, this_slot, from_slot, rebel_slot))
				return false;
			sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
		}
		return true;
	}

	inline bool apply_subtriggers(uint16_t const* source, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		if((*source & trigger_codes::is_disjunctive_scope) != 0)
			return apply_disjuctively(source, ws, primary_slot, this_slot, from_slot, rebel_slot);
		else
			return apply_conjuctively(source, ws, primary_slot, this_slot, from_slot, rebel_slot);
	}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wunused-parameter"

	bool tf_generic_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_x_neighbor_province_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto prov_id = ((provinces::province_state*)primary_slot)->id;
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
	bool tf_x_neighbor_country_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { 
		auto n_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->neighboring_nations);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto n : n_range) {
				if(apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto n : n_range) {
				if(!apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_neighbor_country_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { 
		auto pop_province = ((population::pop*)primary_slot)->location;
		auto province_owner = ws.w.province_s.province_state_container[pop_province].owner;
		if(province_owner)
			return tf_x_neighbor_country_scope_nation(tval, ws, province_owner, this_slot, from_slot, rebel_slot);
		else
			return 0 == (*tval & trigger_codes::is_existance_scope);
	}
	bool tf_x_war_countries_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		boost::container::small_vector<nations::country_tag, 32, concurrent_allocator<nations::country_tag>> opposing_countries;
		military::list_opposing_countries(ws, *((nations::nation*) primary_slot), opposing_countries);

		if(*tval & trigger_codes::is_existance_scope) {
			for(auto n : opposing_countries) {
				if(apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto n : opposing_countries) {
				if(!apply_subtriggers(tval, ws, &(ws.w.nation_s.nations[n]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_war_countries_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto pop_province = ((population::pop*)primary_slot)->location;
		auto province_owner = ws.w.province_s.province_state_container[pop_province].owner;
		if(province_owner)
			return tf_x_war_countries_scope_nation(tval, ws, province_owner, this_slot, from_slot, rebel_slot);
		else
			return 0 == (*tval & trigger_codes::is_existance_scope);
	}
	bool tf_x_greater_power_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto ranked_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		int32_t great_nations_count = int32_t(ws.s.modifiers_m.global_defines.great_nations_count);

		if(*tval & trigger_codes::is_existance_scope) {
			int32_t count = 0;
			for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
				auto& nation = ws.w.nation_s.nations[*n];
				if(nations::is_great_power(ws, nation)) {
					++count;
					if(apply_subtriggers(tval, ws, &nation, this_slot, from_slot, rebel_slot))
						return true;
				}
			}
			return false;
		} else {
			int32_t count = 0;
			for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
				auto& nation = ws.w.nation_s.nations[*n];
				if(nations::is_great_power(ws, nation)) {
					++count;
					if(!apply_subtriggers(tval, ws, &nation, this_slot, from_slot, rebel_slot))
						return false;
				}
			}
			return true;
		}
	}
	bool tf_x_owned_province_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto region_provinces = ws.s.province_m.states_to_province_index.get_row(((nations::state_instance*)primary_slot)->region_id);
		auto state_owner = ((nations::state_instance*)primary_slot)->owner;
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : region_provinces) {
				auto& prov = ws.w.province_s.province_state_container[p];
				if(prov.owner == state_owner && apply_subtriggers(tval, ws, &prov, this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p : region_provinces) {
				auto& prov = ws.w.province_s.province_state_container[p];
				if(prov.owner == state_owner && !apply_subtriggers(tval, ws, &prov, this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_owned_province_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto owned_range = get_range(ws.w.province_s.province_arrays, ((nations::nation*)primary_slot)->owned_provinces);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : owned_range) {
				if(apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p : owned_range) {
				if(!apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_core_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto core_range = get_range(ws.w.province_s.core_arrays, ((provinces::province_state*)primary_slot)->cores);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : core_range) {
				auto core_holder = ws.w.culture_s.national_tags_state[p].holder;
				if(core_holder && apply_subtriggers(tval, ws, core_holder, this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p : core_range) {
				auto core_holder = ws.w.culture_s.national_tags_state[p].holder;
				if(core_holder && !apply_subtriggers(tval, ws, core_holder, this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_core_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto tag = ((nations::nation*)primary_slot)->tag;
		auto core_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto p : core_range) {
				if(apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto p : core_range) {
				if(!apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[p]), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_state_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto states = get_range(ws.w.nation_s.state_arrays, ((nations::nation*)primary_slot)->member_states);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto s = states.first; s != states.second; ++s) {
				if(apply_subtriggers(tval, ws, s->state, this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto s = states.first; s != states.second; ++s) {
				if(!apply_subtriggers(tval, ws, s->state, this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_substate_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto nation_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->vassals);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				auto& n = ws.w.nation_s.nations.get(*i);
				if((n.flags & nations::nation::is_substate) != 0 && apply_subtriggers(tval, ws, &n, this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				auto& n = ws.w.nation_s.nations.get(*i);
				if((n.flags & nations::nation::is_substate) != 0 && !apply_subtriggers(tval, ws, &n, this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_sphere_member_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto nation_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->sphere_members);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				if(apply_subtriggers(tval, ws, &(ws.w.nation_s.nations.get(*i)), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				if(!apply_subtriggers(tval, ws, &(ws.w.nation_s.nations.get(*i)), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_pop_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto pop_range = get_range(ws.w.population_s.pop_arrays, ((provinces::province_state*)primary_slot)->pops);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = pop_range.first; i != pop_range.second; ++i) {
				if(apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto i = pop_range.first; i != pop_range.second; ++i) {
				if(!apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
					return false;
			}
			return true;
		}
	}
	bool tf_x_pop_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto province_range = ws.s.province_m.states_to_province_index.get_row(((nations::state_instance*)primary_slot)->region_id);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				auto& pstate = ws.w.province_s.province_state_container[*j];
				if(pstate.owner == ((nations::state_instance*)primary_slot)->owner) {
					auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*j].pops);
					for(auto i = pop_range.first; i != pop_range.second; ++i) {
						if(apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
							return true;
					}
				}
			}
			return false;
		} else {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				auto& pstate = ws.w.province_s.province_state_container[*j];
				if(pstate.owner == ((nations::state_instance*)primary_slot)->owner) {
					auto pop_range = get_range(ws.w.population_s.pop_arrays, pstate.pops);
					for(auto i = pop_range.first; i != pop_range.second; ++i) {
						if(!apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
							return false;
					}
				}
			}
			return true;
		}
	}
	bool tf_x_pop_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto province_range = get_range(ws.w.province_s.province_arrays, ((nations::nation*)primary_slot)->owned_provinces);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*j].pops);
				for(auto i = pop_range.first; i != pop_range.second; ++i) {
					if(apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
						return true;
				}
			}
			return false;
		} else {
			for(auto j = province_range.first; j != province_range.second; ++j) {
				auto pop_range = get_range(ws.w.population_s.pop_arrays, ws.w.province_s.province_state_container[*j].pops);
				for(auto i = pop_range.first; i != pop_range.second; ++i) {
					if(!apply_subtriggers(tval, ws, &(ws.w.population_s.pops.get(*i)), this_slot, from_slot, rebel_slot))
						return false;
				}
			}
			return true;
		}
	}
	bool tf_x_provinces_in_variable_region(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
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
	bool tf_owner_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, ((nations::state_instance*)primary_slot)->owner, this_slot, from_slot, rebel_slot);
	}
	bool tf_owner_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, ((provinces::province_state*)primary_slot)->owner, this_slot, from_slot, rebel_slot);
	}
	bool tf_controller_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, ((provinces::province_state*)primary_slot)->controller, this_slot, from_slot, rebel_slot);
	}
	bool tf_location_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[((population::pop*)primary_slot)->location]), this_slot, from_slot, rebel_slot);
	}
	bool tf_country_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto owner = ((nations::state_instance*)primary_slot)->owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_country_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto location = ((population::pop*)primary_slot)->location;
		auto owner = ws.w.province_s.province_state_container[location].owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_capital_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto capital = ((nations::nation*)primary_slot)->current_capital;
		if(is_valid_index(capital))
			return apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[capital]), this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_this_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, this_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_from_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, from_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_sea_zone_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(((provinces::province_state*)primary_slot)->id);
		if(sea_zones.first != sea_zones.second)
			return apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[*sea_zones.first]), this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_cultural_union_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto prim_culture = ((nations::nation*)primary_slot)->primary_culture;
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
	bool tf_overlord_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto so = ((nations::nation*)primary_slot)->overlord;
		if(so)
			return apply_subtriggers(tval, ws, so, this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_sphere_owner_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto so = ((nations::nation*)primary_slot)->sphere_leader;
		if(so)
			return apply_subtriggers(tval, ws, so, this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_independence_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto ination = ws.w.culture_s.national_tags_state[rebel_slot->independence_tag].holder;
		if(ination)
			return apply_subtriggers(tval, ws, ination, this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_flashpoint_tag_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto ctag = ((nations::state_instance*)primary_slot)->crisis_tag;
		if(is_valid_index(ctag)) {
			auto ctag_holder = ws.w.culture_s.national_tags_state[ctag].holder;
			if(ctag_holder)
				return apply_subtriggers(tval, ws, ctag_holder, this_slot, from_slot, rebel_slot);
		}
		return false;
	}
	bool tf_crisis_state_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		if(is_valid_index(ws.w.nation_s.crisis_state))
			return apply_subtriggers(tval, ws, &(ws.w.nation_s.states.get(ws.w.nation_s.crisis_state)), this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_state_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto pop_province = ((population::pop*)primary_slot)->location;
		auto prov_state = &(ws.w.province_s.province_state_container[pop_province]);
		return apply_subtriggers(tval, ws, prov_state, this_slot, from_slot, rebel_slot);
	}
	bool tf_state_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto state_instance = ((provinces::province_state*)primary_slot)->state_instance;
		if(state_instance)
			return apply_subtriggers(tval, ws, state_instance, this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_tag_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto tag = trigger_payload(tval[2]).tag;
		auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder;
		if(tag_holder)
			return apply_subtriggers(tval, ws, tag_holder, this_slot, from_slot, rebel_slot);
		return false;
	}
	bool tf_integer_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		provinces::province_tag ptag(tval[2]);
		provinces::province_state* pstate = &(ws.w.province_s.province_state_container[ptag]);
		return apply_subtriggers(tval, ws, pstate, this_slot, from_slot, rebel_slot);
	}
	bool tf_country_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return apply_subtriggers(tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
	}
	bool tf_country_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto owner = ((provinces::province_state*)primary_slot)->owner;
		if(owner)
			return apply_subtriggers(tval, ws, owner, this_slot, from_slot, rebel_slot);
		else
			return false;
	}
	bool tf_cultural_union_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto prim_culture = ((population::pop*)primary_slot)->culture;
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



	static bool(*scope_functions[])(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) = {
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
	bool compare_values(uint16_t trigger_code, T value_a, T value_b) {
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

	bool tf_year(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return compare_values(tval[0], int32_t(tag_to_date(ws.w.current_date).year()), int32_t(tval[2]));
	}
	bool tf_month(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { 
		return compare_values(tval[0], int32_t(tag_to_date(ws.w.current_date).month()), int32_t(tval[2]));
	}
	bool tf_port(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto prov_id = ((provinces::province_state*)primary_slot)->id;
		auto is_port = (ws.s.province_m.province_container[prov_id].flags & (provinces::province::sea | provinces::province::coastal)) == provinces::province::coastal;
		return compare_values(tval[0], is_port, true);
	}
	bool tf_rank(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		// note: comparison revesed since rank 1 is "greater" than rank 1 + N
		return compare_values(tval[0], int32_t(tval[2]), int32_t(((nations::nation*)primary_slot)->overall_rank));
	}
	bool tf_technology(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto technology = trigger_payload(tval[2]).tech;
		auto nation_id = ((nations::nation*)primary_slot)->id;
		auto tech_row = ws.w.nation_s.active_technologies.get_row(nation_id);
		auto has_tech = bit_vector_test(tech_row, to_index(technology));
		return compare_values(tval[0], has_tech, true);
	}
	bool tf_strata_rich(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto pop_type = ((population::pop*)primary_slot)->type;
		return compare_values(tval[0], (ws.s.population_m.pop_types[pop_type].flags & population::pop_type::strata_mask) == population::pop_type::strata_rich, true);
	}
	bool tf_life_rating_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		return compare_values(tval[0], ((provinces::province_state*)primary_slot)->life_rating, trigger_payload(tval[2]).signed_value);
	}
	bool tf_life_rating_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto state = (nations::state_instance*)primary_slot;
		auto province_range = ws.s.province_m.states_to_province_index.get_row(state->region_id);
		int32_t min_life_rating = std::numeric_limits<int32_t>::max();
		for(auto p : province_range) {
			auto& ps = ws.w.province_s.province_state_container[p];
			if(ps.owner == state->owner)
				min_life_rating = std::min(min_life_rating, int32_t(ps.life_rating));
		}
		return compare_values(tval[0], min_life_rating, int32_t(trigger_payload(tval[2]).signed_value));
	}
	bool tf_has_empty_adjacent_state_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		bool has_empty = false;
		auto adj_range = ws.s.province_m.same_type_adjacency.get_row(((provinces::province_state*)primary_slot)->id);
		for(auto p : adj_range) {
			if(ws.w.province_s.province_state_container[p].owner == nullptr) {
				has_empty = true;
				break;
			}
		}
		return compare_values(tval[0], has_empty, true);
	}
	bool tf_has_empty_adjacent_state_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto state = (nations::state_instance*)primary_slot;
		auto province_range = ws.s.province_m.states_to_province_index.get_row(state->region_id);
		auto state_owner = state->owner;

		bool has_empty = [province_range, &ws, state_owner, state]() {
			for(auto p : province_range) {
				auto& ps = ws.w.province_s.province_state_container[p];

				if(ps.owner == state_owner) {
					auto adj_range = ws.s.province_m.same_type_adjacency.get_row(p);
					for(auto q : adj_range) {
						auto& qs = ws.w.province_s.province_state_container[p];
						if((qs.owner == nullptr) & (qs.state_instance != state)) {
							return true;
						}
					}
				}
			}
			return false;
		}();
		return compare_values(tval[0], has_empty, true);
	}
	bool tf_state_id_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		provinces::province_tag pid(tval[2]);
		auto current_prov = ((provinces::province_state*)primary_slot)->id;
		auto same_region = ws.s.province_m.province_container[current_prov].state_id == ws.s.province_m.province_container[pid].state_id;
		return compare_values(tval[0], same_region, true);
	}
	bool tf_state_id_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		provinces::province_tag pid(tval[2]);
		auto current_region = ((nations::state_instance*)primary_slot)->region_id;
		auto same_region = current_region == ws.s.province_m.province_container[pid].state_id;
		return compare_values(tval[0], same_region, true);
	}
	bool tf_cash_reserves(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto ratio = economy::money_qnty_type(read_float_from_payload(tval + 2));
		auto target = population::desired_needs_spending(ws, *((population::pop*)primary_slot));
		auto money = economy::money_qnty_type(((population::pop*)primary_slot)->money);
		return compare_values(tval[0], money / target, ratio);
	}
	bool tf_unemployment_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unemployment_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unemployment_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unemployment_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_slave_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_slave_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_slave_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_slave_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_independant(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_national_minority_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_national_minority_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_national_minority_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_government_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_government_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_capital(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_tech_school(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_primary_culture(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_accepted_culture(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_pop_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_state_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_province_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_nation_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_reb_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_reb_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_nation_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_pop_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_nation_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_pop_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_nation_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_pop_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_nation_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_pop_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_nation_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_group_pop_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_terrain_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_terrain_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_trade_goods(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_secondary_power_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_secondary_power_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_faction_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_faction_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_unclaimed_cores(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_bool(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_this_self_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_this_rebel(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_tag_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_tag_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_tag_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_tag_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_cultural_union_tag_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_factory_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_exhaustion_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_blockade(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_owns(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controls(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_integer(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_core_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_revolts(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_revolt_percentage(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_cities_int(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_cities_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_cities_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_cities_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_cities_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_cities_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_ports(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_allies(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_vassals(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_owned_by_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_owned_by_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_owned_by_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_owned_by_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_owned_by_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_owned_by_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_exists_bool(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_exists_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_country_flag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_nation_this(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_state_this(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_province_this(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_pop_this(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_nation_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_state_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_province_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_continent_pop_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_casus_belli_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_casus_belli_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_casus_belli_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_casus_belli_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_casus_belli_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_casus_belli_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_access_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_access_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_access_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_access_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_access_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_access_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_prestige_value(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_prestige_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_prestige_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_prestige_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_prestige_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_prestige_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_badboy(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_building_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_building_fort(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_building_railroad(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_building_naval_base(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_empty(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_blockaded(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_country_modifier(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_province_modifier(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_region(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_tag_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_tag_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_tag_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_tag_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_tag_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_neighbour_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_neighbour_this(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_neighbour_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_units_in_province_value(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_units_in_province_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_units_in_province_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_units_in_province_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_units_in_province_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_units_in_province_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_with_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_with_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_with_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_with_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_with_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_with_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unit_in_battle(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_total_amount_of_divisions(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_money(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_lost_national(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_vassal(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_ruling_party_ideology_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_ruling_party_ideology_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_ruling_party(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_ideology_enabled(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_political_reform_want_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_political_reform_want_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_social_reform_want_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_social_reform_want_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_total_amount_of_ships(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_plurality(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_corruption(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_state_religion_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_state_religion_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_state_religion_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_nation_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_nation_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_nation_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_nation_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_state_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_state_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_state_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_state_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_province_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_province_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_province_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_province_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_pop_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_pop_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_pop_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_primary_culture_pop_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_accepted_culture_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_accepted_culture_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_accepted_culture_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_coastal(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_sphere_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_sphere_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_sphere_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_sphere_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_sphere_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_sphere_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_produces_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_produces_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_produces_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_produces_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_average_militancy_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_average_militancy_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_average_militancy_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_average_consciousness_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_average_consciousness_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_average_consciousness_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_next_reform_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_next_reform_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rebel_power_fraction(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_recruited_percentage_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_recruited_percentage_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_culture_core(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_nationalism(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_overseas(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_rebels(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_owner(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_controlled_by_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_canal_enabled(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_state_capital(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_truce_with_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_truce_with_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_truce_with_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_truce_with_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_truce_with_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_truce_with_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_total_pops_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_total_pops_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_total_pops_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_total_pops_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_type_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_type_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_type_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_type_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_empty_adjacent_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_leader(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_ai(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_create_vassals(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_possible_vassal(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_province_id(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_vassal_of_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_vassal_of_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_vassal_of_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_vassal_of_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_vassal_of_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_vassal_of_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_alliance_with_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_alliance_with_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_alliance_with_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_alliance_with_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_alliance_with_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_alliance_with_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_recently_lost_war(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_mobilised(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_mobilisation_size(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_crime_higher_than_education_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_crime_higher_than_education_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_crime_higher_than_education_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_crime_higher_than_education_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_agree_with_ruling_party(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_colonial_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_colonial_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_factories(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_default_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_default_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_default_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_default_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_default_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_in_default_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_total_num_of_ports(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_always(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_election(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_global_flag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_capital(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_nationalvalue_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_industrial_score_value(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_industrial_score_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_industrial_score_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_industrial_score_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_industrial_score_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_industrial_score_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_score_value(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_score_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_score_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_score_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_score_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_score_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_civilized_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_civilized_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_civilized_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_national_provinces_occupied(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_greater_power_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_greater_power_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_tax(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_tax(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_tax(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_social_spending_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_social_spending_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_social_spending_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_colonial_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_religion_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_religion_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_religion_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_culture_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_culture_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_culture_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_issue_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_issue_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_issue_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_issue_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_ideology_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_ideology_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_ideology_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_majority_ideology_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_militancy_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_militancy_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_militancy_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_militancy_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_militancy_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_militancy_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_militancy_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_militancy_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_militancy_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_militancy_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_militancy_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_militancy_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_tax_above_poor(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_has_union_tag_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_culture_has_union_tag_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_union_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_union_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_union_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_this_culture_union_this_union_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_minorities_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_minorities_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_minorities_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_revanchism_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_revanchism_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_crime(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_substates(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_num_of_vassals_no_substates(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_brigades_compare_this(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_brigades_compare_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_discovered(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_progress(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_civilization_progress(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_constructing_cb_type(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_our_vassal_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_our_vassal_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_our_vassal_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_our_vassal_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_our_vassal_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_our_vassal_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_substate_of_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_substate_of_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_substate_of_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_substate_of_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_substate_of_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_substate_of_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_substate(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_great_wars_enabled(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_nationalize(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_part_of_sphere(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_sphere_leader_of_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_sphere_leader_of_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_sphere_leader_of_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_sphere_leader_of_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_sphere_leader_of_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_sphere_leader_of_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_number_of_states(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_score(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_releasable_vassal_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_releasable_vassal_other(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_recent_imigration(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_province_control_days(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_disarmed(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_big_producer(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_someone_can_form_union_tag_from(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_someone_can_form_union_tag_other(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_social_movement_strength(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_political_movement_strength(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_factory_in_capital_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_social_movement(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_political_movement(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_cultural_sphere(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_world_wars_enabled(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_pop_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_state_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_province_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_nation_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_culture_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_pop_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_state_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_province_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_nation_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_pop_religion_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_life_needs(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_everyday_needs(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_luxury_needs(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_consciousness_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_consciousness_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_consciousness_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_consciousness_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_literacy_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_literacy_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_literacy_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_literacy_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_militancy_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_militancy_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_militancy_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_militancy_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_spending_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_spending_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_spending_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_military_spending_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_administration_spending_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_administration_spending_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_administration_spending_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_administration_spending_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_education_spending_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_education_spending_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_education_spending_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_education_spending_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_trade_goods_in_state_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_trade_goods_in_state_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_flashpoint(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_flashpoint_tension(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_crisis_exist(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_liberation_crisis(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_claim_crisis(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_crisis_temperature(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_involved_in_crisis_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_involved_in_crisis_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_life_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_life_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_life_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_life_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_everyday_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_everyday_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_everyday_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_everyday_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_luxury_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_luxury_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_luxury_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_rich_strata_luxury_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_life_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_life_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_life_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_life_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_everyday_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_everyday_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_everyday_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_everyday_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_luxury_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_luxury_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_luxury_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_middle_strata_luxury_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_life_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_life_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_life_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_life_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_everyday_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_everyday_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_everyday_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_everyday_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_luxury_needs_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_luxury_needs_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_luxury_needs_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_poor_strata_luxury_needs_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_diplomatic_influence_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_diplomatic_influence_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_diplomatic_influence_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_diplomatic_influence_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_diplomatic_influence_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_unemployment_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_unemployment_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_unemployment_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_unemployment_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_unemployment_nation_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_unemployment_state_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_pop_unemployment_province_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_relation_tag(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_relation_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_relation_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_relation_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_relation_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_check_variable(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_upper_house(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unemployment_by_type_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unemployment_by_type_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unemployment_by_type_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_unemployment_by_type_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_party_loyalty_nation_province_id(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_party_loyalty_from_nation_province_id(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_party_loyalty_province_province_id(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_party_loyalty_from_province_province_id(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_party_loyalty_nation_from_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_party_loyalty_from_nation_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_railroad_no_limit_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_railroad_yes_limit_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_railroad_no_limit_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_railroad_yes_limit_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_fort_no_limit_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_fort_yes_limit_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_fort_no_limit_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_fort_yes_limit_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_naval_base_no_limit_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_naval_base_yes_limit_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_naval_base_no_limit_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_in_province_naval_base_yes_limit_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_railway_in_capital_yes_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_railway_in_capital_yes_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_railway_in_capital_no_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_railway_in_capital_no_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_fort_in_capital_yes_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_fort_in_capital_yes_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_fort_in_capital_no_whole_state_yes_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_fort_in_capital_no_whole_state_no_limit(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_work_available_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_work_available_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_work_available_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_ideology_name_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_ideology_name_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_ideology_name_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_ideology_name_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_name_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_name_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_name_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_name_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_group_name_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_group_name_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_group_name_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_issue_group_name_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_pop_type_name_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_pop_type_name_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_pop_type_name_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_pop_type_name_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_variable_good_name(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_strata_middle(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_strata_poor(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_party_loyalty_from_province_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_factory_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_can_build_factory_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_nationalvalue_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_nationalvalue_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_exhaustion_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_culture_core_province_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_tag_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_country_flag_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_country_flag_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_has_country_modifier_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_nation_reb(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_nation_from_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_nation_this_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_nation_this_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_nation_this_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_religion_nation_this_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_war_exhaustion_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_is_greater_power_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }

	static bool(*trigger_functions[])(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) = {
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
	};
	}

	bool test_trigger(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		if((*tval & trigger_codes::is_scope) != 0) {
			return scope_functions[*tval & trigger_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
		} else {
			return trigger_functions[*tval & trigger_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
		}
	}

#pragma clang diagnostic pop
	
}
