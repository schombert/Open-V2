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
		const_parameter primary_slot,
		const_parameter this_slot,
		const_parameter from_slot,
		population::rebel_faction* rebel_slot,
		bool show_condition = true
	);
	ui::xy_pair make_effect_description(
		world_state& ws,
		ui::tagged_gui_object container,
		ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm,
		ui::text_format const& fmt,
		uint16_t const* tval,
		const_parameter primary_slot,
		const_parameter this_slot,
		const_parameter from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng generator_copy,
		bool show_condition = true
	);
}