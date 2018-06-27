#include "common\\common.h"
#include "province_functions.h"

namespace provinces {
	void add_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		add_item(ws.province_s.core_arrays, ws.province_s.province_state_container[prov].cores, tag);
		add_item(ws.province_s.province_arrays, ws.culture_s.national_tags_state[tag].core_provinces, prov);
	}
	void remove_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		remove_item(ws.province_s.core_arrays, ws.province_s.province_state_container[prov].cores, tag);
		remove_item(ws.province_s.province_arrays, ws.culture_s.national_tags_state[tag].core_provinces, prov);
	}
	bool province_has_core(current_state::state& ws, province_tag prov, cultures::national_tag tag) {
		return contains_item(ws.province_s.core_arrays, ws.province_s.province_state_container[prov].cores, tag);
	}

	void init_ready_provinces(current_state::state& ws) {
		for(auto& p : ws.province_s.province_state_container) {
			if(!p.controller)
				p.controller = p.owner;
		}
	}
}
