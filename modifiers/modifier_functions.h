#pragma once
#include "common\\common.h"
#include "modifiers.h"

class world_state;

namespace nations {
	struct nation;
}
namespace provinces {
	struct province_state;
}

namespace modifiers {
	void add_static_modifier_to_nation(world_state& ws, nations::nation& this_nation, national_modifier_tag mod);
	void add_static_modifier_to_province(world_state& ws, provinces::province_state& this_province, provincial_modifier_tag mod);

	void set_default_crimes(world_state const& ws, nations::nation& this_nation);
	void reset_national_modifier(world_state const& ws, nations::nation& this_nation);
	void reset_provincial_modifier(world_state const& ws, provinces::province_state& this_province);
}
