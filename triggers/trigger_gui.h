#pragma once
#include "common\\common.h"
#include "gui\\gui.h"
#include "triggers.h"

class world_state;
namespace population {
	struct rebel_faction;
}

namespace triggers {
	ui::xy_pair make_trigger_description(
		world_state& ws,
		ui::tagged_gui_object container,
		ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm,
		ui::text_format const& fmt, 
		uint16_t const* tval,
		parameter primary_slot,
		parameter this_slot,
		parameter from_slot,
		population::rebel_faction* rebel_slot
	);
	ui::xy_pair make_effect_description(
		world_state& ws,
		ui::tagged_gui_object container,
		ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm,
		ui::text_format const& fmt,
		uint16_t const* tval,
		parameter primary_slot,
		parameter this_slot,
		parameter from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng generator_copy
	);
}