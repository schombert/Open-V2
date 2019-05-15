#pragma once
#include "common\\common.h"
#include "modifiers.h"
#include "gui\\gui.h"
#include "triggers\\triggers.h"
#include "scenario\scenario.h"

class world_state;

namespace modifiers {
	ui::xy_pair make_province_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, modifiers::modifier_definition const& values);
	ui::xy_pair make_national_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, modifiers::modifier_definition const& values);
	ui::xy_pair make_province_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provincial_modifier_tag t);
	ui::xy_pair make_national_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, national_modifier_tag t);

	ui::xy_pair make_province_modifier_text_with_total(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provincial_modifier_tag t, float base_value = 1.0f);
	ui::xy_pair make_national_modifier_text_with_total(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, national_modifier_tag t, float base_value = 1.0f);


	ui::xy_pair explain_province_modifier(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provinces::province_tag this_province, uint32_t modifier_offset);
	ui::xy_pair explain_national_modifier(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, nations::country_tag this_nation, uint32_t modifier_offset);

	ui::xy_pair make_additive_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, uint32_t base_text = scenario::fixed_ui::chance);
	ui::xy_pair make_multiplicative_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, uint32_t base_text = scenario::fixed_ui::chance);
	ui::xy_pair make_abs_value_additive_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, bool final_value_is_additive = true, uint32_t base_text = scenario::fixed_ui::factor);
	ui::xy_pair make_multiplicative_factor_explanation_in_days(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot);
	ui::xy_pair make_factor_text_body(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot);
	ui::xy_pair make_multiplicative_factor_text_body(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, bool invert_colors);
	ui::xy_pair make_abs_factor_text_body(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot);
}
