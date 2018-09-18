#pragma once
#include "common\\common.h"
#include "gui\\gui.h"

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
		void* primary_slot,
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot
	);
	ui::xy_pair make_effect_description(
		world_state& ws,
		ui::tagged_gui_object container,
		ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm,
		ui::text_format const& fmt,
		uint16_t const* tval,
		void* primary_slot,
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng generator_copy
	);
}