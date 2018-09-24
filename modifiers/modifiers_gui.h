#pragma once
#include "common\\common.h"
#include "modifiers.h"
#include "gui\\gui.h"

class world_state;

namespace provinces {
	struct province_state;
}
namespace nations {
	struct nation;
}

namespace population {
	struct rebel_faction;
}

namespace modifiers {
	ui::xy_pair make_province_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provincial_modifier_vector const& values);
	ui::xy_pair make_national_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, national_modifier_vector const& values);
	ui::xy_pair make_province_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provincial_modifier_tag t);
	ui::xy_pair make_national_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, national_modifier_tag t);

	ui::xy_pair explain_province_modifier(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provinces::province_state const& this_province, uint32_t modifier_offset);
	ui::xy_pair explain_national_modifier(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, nations::nation const& this_nation, uint32_t modifier_offset);

	ui::xy_pair make_additive_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot);
	ui::xy_pair make_multiplicative_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot);
	ui::xy_pair make_factor_text_body(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		void* primary_slot, void* from_slot, population::rebel_faction* rebel_slot);
}
