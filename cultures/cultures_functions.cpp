#include "common\\common.h"
#include "cultures_functions.h"
#include "world_state\\world_state.h"

namespace cultures {
	void reset_state(cultures_state& s) {
		s.culture_arrays.reset();
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
}
