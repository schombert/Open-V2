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
	ui::xy_pair make_abs_value_multiplicative_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
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


	class national_focus_window_t;

	class national_focus_window {
	public:
		std::unique_ptr<national_focus_window_t> win;

		nations::state_tag in_state;

		national_focus_window();
		~national_focus_window();

		void hide(ui::gui_manager& gui_m);
		void init(world_state& ws);
		void update(ui::gui_manager& gui_m, nations::state_tag s);
		void show(ui::gui_manager& gui_m, nations::state_tag s, int32_t x, int32_t y);
	};

	uint32_t nf_tag_to_frame(world_state const& ws, national_focus_tag t);
	bool nf_button_clickable(world_state const& ws, nations::state_tag s);
	ui::xy_pair nf_modifier_text(national_focus_tag nf, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);
	ui::xy_pair nf_tooltip_text(nations::state_tag s, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt);
}
