#include "common\\common.h"
#include "nations_functions.h"
#include "world_state\\world_state.h"
#include "scenario\\scenario.h"

namespace nations {
	nation* make_nation_for_tag(world_state& ws, cultures::national_tag nt) {
		cultures::national_tag_state& tag_state = ws.w.culture_s.national_tags_state[nt];
		if(tag_state.holder)
			return tag_state.holder;
		nation& new_nation = ws.w.nation_s.nations.get_new();

		auto& fixed_tag = ws.s.culutre_m.national_tags[nt];

		tag_state.holder = &new_nation;
		new_nation.tag = nt;
		new_nation.name = fixed_tag.default_name.name;
		new_nation.adjective = fixed_tag.default_name.adjective;
		new_nation.flag = fixed_tag.base_flag;

		new_nation.current_color = fixed_tag.color;

		ws.w.nation_s.active_parties.ensure_capacity(to_index(new_nation.id) + 1);

		return &new_nation;
	}

	bool is_state_empty(world_state& ws, nation* owner, provinces::state_tag region) {
		for(auto prange = ws.s.province_m.states_to_province_index.equal_range(region); prange.first != prange.second; ++prange.first) {
			if(ws.w.province_s.province_state_container[prange.first->second].owner == owner)
				return false;
		}
		return true;
	}

	void destroy_state_instance(world_state& ws, state_tag st) {
		ws.w.nation_s.states.remove(st);
	}

	void remove_province_from_state(world_state&, provinces::province_state& p) {
		p.state_instance = nullptr;
	}

	void silent_remove_province_owner(world_state& ws, nation* owner, provinces::province_tag prov) {
		remove_item(ws.w.province_s.province_arrays, owner->owned_provinces, prov);

		auto& prov_state = ws.w.province_s.province_state_container[prov];

		prov_state.owner = nullptr;
		remove_province_from_state(ws, prov_state);

		const auto region_tag = ws.s.province_m.province_container[prov].state_id;
		if(is_state_empty(ws, owner, region_tag)) {
			region_state_pair* f = find(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ region_tag, state_tag() });
			if(f)
				destroy_state_instance(ws, f->state_id);
			remove_item(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ region_tag, state_tag() });
		}
	}

	void silent_remove_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov) {
		remove_item(ws.province_s.province_arrays, controller->controlled_provinces, prov);
		ws.province_s.province_state_container[prov].controller = nullptr;
	}

	void silent_set_province_owner(world_state& ws, nation* owner, provinces::province_tag prov) {
		auto& prov_state = ws.w.province_s.province_state_container[prov];

		add_item(ws.w.province_s.province_arrays, owner->owned_provinces, prov);
		if(prov_state.owner != nullptr)
			silent_remove_province_owner(ws, prov_state.owner, prov);

		
		prov_state.owner = owner;
		const auto region_id = ws.s.province_m.province_container[prov].state_id;

		region_state_pair* found = find(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ region_id, state_tag() });
		if(found) {
			prov_state.state_instance = &(ws.w.nation_s.states.get(found->state_id));
		} else {
			state_instance* si = &(ws.w.nation_s.states.get_new());
			prov_state.state_instance = si;
			add_item(ws.w.nation_s.state_arrays, owner->member_states, region_state_pair{ region_id, si->id });
		}
	}

	void silent_set_province_controller(current_state::state& ws, nation* controller, provinces::province_tag prov) {
		add_item(ws.province_s.province_arrays, controller->controlled_provinces, prov);
		if(ws.province_s.province_state_container[prov].controller != nullptr)
			silent_remove_province_controller(ws, ws.province_s.province_state_container[prov].controller, prov);
		ws.province_s.province_state_container[prov].controller = controller;
	}
}
