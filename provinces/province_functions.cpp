#include "common\\common.h"
#include "province_functions.h"
#include "modifiers\\modifiers.h"

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

	void init_ready_provinces(world_state& ws) {
		const auto prov_count = ws.s.province_m.province_container.size();
		
		ws.w.province_s.party_loyalty.reset(uint32_t(ws.s.ideologies_m.ideology_container.size()));
		ws.w.province_s.party_loyalty.resize(prov_count);
		ws.w.province_s.province_state_container.resize(prov_count);
		ws.w.province_s.provincial_modifiers.reset(modifiers::provincial_offsets::count);
		ws.w.province_s.provincial_modifiers.resize(prov_count);

		for(uint32_t i = 0; i < prov_count; ++i) {
			auto& p = ws.w.province_s.province_state_container[province_tag(static_cast<uint16_t>(i))];
			p.id = province_tag(static_cast<uint16_t>(i));

			std::string name_temp("PROV");
			name_temp += std::to_string(i);
			p.name = text_data::get_thread_safe_text_handle(ws.s.gui_m.text_data_sequences, name_temp.data(), name_temp.data() + name_temp.length());

			if(!p.controller)
				p.controller = p.owner;
		}
	}
}
