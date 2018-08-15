#pragma once
#include "common\\common.h"
#include "governments.h"
#include "issues\\issues.h"

class world_state;
namespace nations {
	struct nation;
}

namespace governments {
	issues::rules make_party_rules(party& p, scenario::scenario_manager& s);
	void get_best_parties_at_date(party_tag* parties_out, cultures::national_tag tag, date_tag date, scenario::scenario_manager& s);
	void silent_set_ruling_party(world_state& ws, nations::nation& this_nation, party_tag p); // does not update effects of changing party issues
	void reset_upper_house(world_state& ws, nations::country_tag id);
	void update_current_rules(world_state& ws, nations::nation& this_nation);
}
