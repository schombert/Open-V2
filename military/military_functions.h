#pragma once
#include "common\\common.h"
#include "military.h"

class world_state;

namespace nations {
	struct nation;
}
namespace population {
	struct pop;
}

namespace military {
	void init_military_state(world_state& ws);
	military_leader& make_empty_leader(world_state& ws, cultures::culture_tag culture, bool is_general);
	leader_tag make_auto_leader(world_state& ws, cultures::culture_tag culture, bool is_general, date_tag creation_date);
	void calculate_leader_traits(world_state& ws, military_leader& l);
	army& make_army(world_state& ws, nations::nation& n, provinces::province_tag location);
	fleet& make_fleet(world_state& ws, nations::nation& n, provinces::province_tag location);
	void immediate_add_pop_to_army(world_state& ws, army& target_army, population::pop& p);
	void list_opposing_countries(world_state& ws, nations::nation& this_nation, boost::container::small_vector<nations::country_tag, 32, concurrent_allocator<nations::country_tag>>& result);
}
