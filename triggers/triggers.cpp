#include "triggers.h"
#include "codes.h"
#include "world_state\\world_state.h"

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
	bool tf_x_neighbor_province_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_neighbor_country_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_neighbor_country_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_war_countries_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_war_countries_scope_pop(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_greater_power_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_owned_province_scope_state(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_owned_province_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_core_scope_province(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_core_scope_nation(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_state_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) { return false; }
	bool tf_x_substate_scope(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		auto nation_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->vassals);
		if(*tval & trigger_codes::is_existance_scope) {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				auto& n = ws.w.nation_s.nations.get(*i);
				if(n.is_substate && apply_subtriggers(tval, ws, &n, this_slot, from_slot, rebel_slot))
					return true;
			}
			return false;
		} else {
			for(auto i = nation_range.first; i != nation_range.second; ++i) {
				auto& n = ws.w.nation_s.nations.get(*i);
				if(n.is_substate && !apply_subtriggers(tval, ws, &n, this_slot, from_slot, rebel_slot))
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
		auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(to_index(((provinces::province_state*)primary_slot)->id));
		if(sea_zones.first != sea_zones.second)
			return apply_subtriggers(tval, ws, &(ws.w.province_s.province_state_container[provinces::province_tag(*sea_zones.first)]), this_slot, from_slot, rebel_slot);
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

	bool test_trigger(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot) {
		if((*tval & trigger_codes::is_scope) != 0) {
			return scope_functions[*tval & trigger_codes::code_mask](tval, ws, primary_slot, this_slot, from_slot, rebel_slot);
		} else {
			return false;
		}
	}

#pragma clang diagnostic pop
	
}
