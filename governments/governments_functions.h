#pragma once
#include "common\\common.h"
#include "governments.h"
#include "issues\\issues.h"

class world_state;

namespace governments {
	issues::rules make_party_rules(party& p, scenario::scenario_manager& s);
	void get_best_parties_at_date(party_tag* parties_out, cultures::national_tag tag, date_tag date, scenario::scenario_manager& s);
}
