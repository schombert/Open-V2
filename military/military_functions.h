#pragma once
#include "common\\common.h"
#include "military.h"

class world_state;

namespace military {
	military_leader& make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general);
	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date);
	void calculate_leader_traits(world_state& ws, military_leader& l);
}
