#include "common\\common.h"
#include "governments_functions.h"
#include "world_state\\world_state.h"

namespace governments {
	issues::rules make_party_rules(party& p, scenario::scenario_manager& s) {
		issues::rules result;
		issues::option_tag* opts = s.governments_m.party_issues.get_row(p.id);
		for(uint32_t i = 0; i < s.issues_m.party_issues_options_count; ++i) {
			if(is_valid_index(opts[i]))
				result.rules |= s.issues_m.options[opts[i]].issue_rules.rules_settings.rules;
		}
		return result;
	}

	void get_best_parties_at_date(party_tag* parties_out, cultures::national_tag tag, date_tag date, scenario::scenario_manager& s) {
		uint32_t* rules_count = (uint32_t*)_alloca(sizeof(uint32_t) * s.ideologies_m.ideologies_count);
		for(uint32_t i = 0; i < s.ideologies_m.ideologies_count; ++i) {
			parties_out[i] = party_tag();
			rules_count[i] = 0ui32;
		}
		auto const& tag_obj = s.culture_m.national_tags[tag];
		for(auto p_index = tag_obj.first_party; p_index != tag_obj.last_party; p_index = party_tag(to_index(p_index) + 1)) {
			party& this_party = s.governments_m.parties[p_index];

			if(this_party.start_date <= date && date < this_party.end_date) {
				uint32_t ideology_index = to_index(this_party.ideology);
				uint32_t allowed_count = uint32_t(__builtin_popcount(this_party.party_rules.rules));
				if(allowed_count > rules_count[ideology_index]) {
					parties_out[ideology_index] = p_index;
					rules_count[ideology_index] = allowed_count;
				}
			}
		}
		
	}
}
