#include "common\\common.h"
#include "cultures_functions.h"
#include "world_state\\world_state.h"

namespace cultures {
	void reset_state(cultures_state& s) {
		s.culture_arrays.reset();
		for(auto& ht : s.tags_to_holders)
			ht = nations::country_tag();
		for(auto& nt : s.national_tags_state)
			nt.core_provinces = set_tag<provinces::province_tag>();
	}

	void init_cultures_state(world_state& ws) {
		if(ws.w.culture_s.national_tags_state.size() != ws.s.culture_m.national_tags.size())
			ws.w.culture_s.national_tags_state.resize(ws.s.culture_m.national_tags.size());
		if(ws.w.culture_s.country_flags_by_government.inner_size() != uint32_t(ws.s.governments_m.governments_container.size()))
			ws.w.culture_s.country_flags_by_government.reset(uint32_t(ws.s.governments_m.governments_container.size()));
		if(ws.w.culture_s.country_flags_by_government.outer_size() != ws.s.culture_m.national_tags.size())
			ws.w.culture_s.country_flags_by_government.resize(ws.s.culture_m.national_tags.size());

		for(uint32_t t = 0; t < ws.s.culture_m.national_tags.size(); ++t) {
			const national_tag ntag(static_cast<value_base_of<national_tag>>(t));
			const auto& ntag_obj = ws.s.culture_m.national_tags[ntag];
			for(uint32_t g = 0; g < ws.s.governments_m.governments_container.size(); ++g) {
				const governments::government_tag gtag(static_cast<value_base_of<governments::government_tag>>(g));
				switch(ws.s.governments_m.governments_container[gtag].flag) {
					case governments::flag_type::communist:
						ws.w.culture_s.country_flags_by_government.get(ntag, gtag) = ntag_obj.communist_flag; break;
					case governments::flag_type::fascist:
						ws.w.culture_s.country_flags_by_government.get(ntag, gtag) = ntag_obj.fascist_flag; break;
					case governments::flag_type::monarchy:
						ws.w.culture_s.country_flags_by_government.get(ntag, gtag) = ntag_obj.monarchy_flag; break;
					case governments::flag_type::not_specified:
						ws.w.culture_s.country_flags_by_government.get(ntag, gtag) = ntag_obj.base_flag; break;
					case governments::flag_type::republic:
						ws.w.culture_s.country_flags_by_government.get(ntag, gtag) = ntag_obj.republic_flag; break;
				}
			}
		}
	}

	void replace_cores(world_state& ws, national_tag replace_from, national_tag replace_to) {
		auto& core_from = ws.w.culture_s.national_tags_state[replace_from];
		auto& core_to = ws.w.culture_s.national_tags_state[replace_to];

		auto old_core_range = get_range(ws.w.province_s.province_arrays, core_from.core_provinces);
		for(auto o : old_core_range) {
			auto& cores_array = ws.w.province_s.province_state_container.get<province_state::cores>(o);
			remove_item(ws.w.province_s.core_arrays, cores_array, replace_from);
			add_item(ws.w.province_s.core_arrays, cores_array, replace_to);
			add_item(ws.w.province_s.province_arrays, core_to.core_provinces, o);
		}
		clear(ws.w.province_s.province_arrays, core_from.core_provinces);
	}
}
