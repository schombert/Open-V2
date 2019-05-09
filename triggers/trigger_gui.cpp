#include "common\\common.h"
#include "trigger_gui.h"
#include "gui\\gui.hpp"
#include "effects.h"
#include "codes.h"
#include "world_state\\world_state.h"
#include <random>
#include "population\\population_functions.h"
#include "nations\\nations_functions.hpp"
#include "modifiers\\modifiers_gui.h"
#include "technologies\\technologies_functions.h"
#include "provinces\\province_functions.h"

#undef small

namespace triggers {
	namespace {
		inline uint32_t cmp_code_to_fixed_ui(uint16_t code) {
			switch(code & trigger_codes::association_mask) {
				case trigger_codes::association_eq: return scenario::fixed_ui::cmp_eq;
				case trigger_codes::association_le: return scenario::fixed_ui::cmp_le;
				case trigger_codes::association_lt: return scenario::fixed_ui::cmp_lt;
				case trigger_codes::association_ge: return scenario::fixed_ui::cmp_ge;
				case trigger_codes::association_gt: return scenario::fixed_ui::cmp_gt;
				case trigger_codes::association_ne: return scenario::fixed_ui::cmp_ne;
			}
			return scenario::fixed_ui::cmp_eq;
		}

		inline uint32_t cmp_has_code_to_fixed_ui(uint16_t code) {
			switch(code & trigger_codes::association_mask) {
				case trigger_codes::association_eq: return scenario::fixed_ui::cmp_has_eq;
				case trigger_codes::association_le: return scenario::fixed_ui::cmp_has_le;
				case trigger_codes::association_lt: return scenario::fixed_ui::cmp_has_lt;
				case trigger_codes::association_ge: return scenario::fixed_ui::cmp_has_ge;
				case trigger_codes::association_gt: return scenario::fixed_ui::cmp_has_gt;
				case trigger_codes::association_ne: return scenario::fixed_ui::cmp_has_ne;
			}
			return scenario::fixed_ui::cmp_has_eq;
		}

		inline uint32_t any_all_code_to_fixed_ui(uint16_t code) {
			if(code & trigger_codes::is_disjunctive_scope)
				return scenario::fixed_ui::any_of_the_following;
			else
				return scenario::fixed_ui::all_of_the_following;
		}

		inline uint32_t every_any_code_to_fixed_ui(uint16_t code) {
			if(code & trigger_codes::is_existance_scope)
				return scenario::fixed_ui::any;
			else
				return scenario::fixed_ui::every;
		}

		template<typename T>
		ui::xy_pair generic_display_value(T value, display_type display_as, ui::xy_pair cursor, ui::text_format const& fmt, world_state& ws, ui::tagged_gui_object parent_object,
			ui::line_manager& lm) {

			switch(display_as) {
				case display_type::neutral_percent:
				case display_type::percent:
					return ui::add_text(cursor, text_data::percent{ float(value) }, fmt, ws, parent_object, lm);
				case display_type::exact_integer:
					return ui::add_text(cursor, text_data::exact_integer{ value }, fmt, ws, parent_object, lm);
				default:
				case display_type::integer:
				case display_type::neutral_integer:
					return ui::add_text(cursor, text_data::integer{ value }, fmt, ws, parent_object, lm);
				case display_type::currency:
					return ui::add_text(cursor, text_data::currency{ float(value) }, fmt, ws, parent_object, lm);
				case display_type::percent_fp_one_place:
					return ui::add_text(cursor, text_data::percent_fp_one_place{ float(value) }, fmt, ws, parent_object, lm);
				case display_type::fp_one_place:
					return ui::add_text(cursor, text_data::fp_one_place{ float(value) }, fmt, ws, parent_object, lm);
				case display_type::fp_two_places:
					return ui::add_text(cursor, text_data::fp_two_places{ float(value) }, fmt, ws, parent_object, lm);
				case display_type::fp_three_places:
					return ui::add_text(cursor, text_data::fp_three_places{ float(value) }, fmt, ws, parent_object, lm);
			}
			return cursor;
		}

		template<typename T>
		ui::xy_pair display_with_comparison_no_newline(
			uint16_t trigger_code,
			uint32_t left_label,
			T value,
			display_type d_type,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[left_label], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(trigger_code)], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = generic_display_value(value, d_type, cursor_in, fmt, ws, container, lm);

			return cursor_in;
		}

		template<typename T>
		ui::xy_pair display_with_has_comparison_no_newline(
			uint16_t trigger_code,
			uint32_t left_label,
			T value,
			display_type d_type,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[left_label], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_has_code_to_fixed_ui(trigger_code)], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = generic_display_value(value, d_type, cursor_in, fmt, ws, container, lm);

			return cursor_in;
		}

		template<typename T>
		ui::xy_pair display_with_has_comparison(
			uint16_t trigger_code,
			uint32_t left_label,
			T value,
			display_type d_type,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = display_with_has_comparison_no_newline(trigger_code, left_label, value, d_type, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		template<typename T>
		ui::xy_pair display_with_comparison(
			uint16_t trigger_code,
			uint32_t left_label,
			T value,
			display_type d_type,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = display_with_comparison_no_newline(trigger_code, left_label, value, d_type, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		inline ui::xy_pair display_with_comparison_no_newline(
			uint16_t trigger_code,
			uint32_t left_label,
			text_data::text_tag value,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[left_label], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(trigger_code)], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, value, fmt, ws, container, lm);
			return cursor_in;
		}

		inline ui::xy_pair display_with_comparison_no_newline(
			uint16_t trigger_code,
			text_data::text_tag left,
			uint32_t right_label,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = ui::add_text(cursor_in, left, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(trigger_code)], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[right_label], fmt, ws, container, lm);
			return cursor_in;
		}

		inline ui::xy_pair display_with_comparison(
			uint16_t trigger_code,
			uint32_t left_label,
			text_data::text_tag value,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = display_with_comparison_no_newline(trigger_code, left_label, value, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		inline ui::xy_pair display_with_comparison(
			uint16_t trigger_code,
			text_data::text_tag left,
			uint32_t right_label,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = display_with_comparison_no_newline(trigger_code, left, right_label, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		inline ui::xy_pair display_with_comparison_no_newline(
			uint16_t trigger_code,
			text_data::text_tag value,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(trigger_code)], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, value, fmt, ws, container, lm);
			return cursor_in;

		}
		inline ui::xy_pair display_with_comparison(
			uint16_t trigger_code,
			text_data::text_tag value,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = display_with_comparison_no_newline(trigger_code, value, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		inline ui::xy_pair display_with_has_comparison_no_newline(
			uint16_t trigger_code,
			text_data::text_tag value,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_has_code_to_fixed_ui(trigger_code)], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, value, fmt, ws, container, lm);
			return cursor_in;
		}

		inline ui::xy_pair display_with_has_comparison(
			uint16_t trigger_code,
			text_data::text_tag value,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt) {

			cursor_in = display_with_has_comparison_no_newline(trigger_code, value, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		inline uint32_t count_subtriggers(uint16_t const* source) {
			uint32_t count = 0;
			const auto source_size = 1 + get_trigger_payload_size(source);
			auto sub_units_start = source + 2 + trigger_scope_data_payload(source[0]);
			while(sub_units_start < source + source_size) {
				++count;
				sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
			}
			return count;
		}

		inline ui::xy_pair display_subtriggers(
			uint16_t const* source,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt,
			const_parameter primary_slot,
			const_parameter this_slot,
			const_parameter from_slot,
			bool show_condition) {

			const auto source_size = 1 + get_trigger_payload_size(source);
			auto sub_units_start = source + 2 + trigger_scope_data_payload(source[0]);
			while(sub_units_start < source + source_size) {
				cursor_in = make_trigger_description( ws, container, cursor_in, lm, fmt,
					sub_units_start, primary_slot, this_slot, from_slot, show_condition);
				sub_units_start += 1 + get_trigger_payload_size(sub_units_start);
			}
			return cursor_in;
		}
#define TRIGGER_DISPLAY_PARAMS uint16_t const* tval, world_state& ws, ui::tagged_gui_object container, \
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, const_parameter primary_slot, \
			const_parameter this_slot, const_parameter from_slot


		ui::xy_pair tf_none(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			return cursor_in;
		}

		ui::xy_pair tf_generic_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			if(count_subtriggers(tval) > 1) {
				//cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}
			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, show_condition);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_neighbor_province_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::neighboring_province], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_neighbor_country_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::neighboring_nation], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_neighbor_country_scope_pop(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::neighboring_nation], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_war_countries_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation_at_war_against], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_war_countries_scope_pop(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation_at_war_against], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_greater_power_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::great_power], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_owned_province_scope_state(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owned_province], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_state(primary_slot)) ? ws.w.nation_s.states.get<state::name>(to_state(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_state],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_owned_province_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owned_province], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_core_scope_province(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::core_in], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_core_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::core_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_state_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::state_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_substate_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::substate_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_sphere_member_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation_in_sphere], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_pop_scope_province(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pop], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_pop_scope_state(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pop], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_pop_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pop], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_x_provinces_in_variable_region(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			auto region = trigger_payload(*(tval + 2)).state;

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[every_any_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::province_in], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				ws.s.province_m.state_names[region],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, const_parameter(), this_slot, from_slot, false);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_owner_scope_state(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_state(primary_slot)) ? ws.w.nation_s.states.get<state::name>(to_state(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_state],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				bool(to_state(primary_slot)) ? nations::state_owner(ws, to_state(primary_slot)) : nations::country_tag(), this_slot, from_slot, show_condition && bool(to_state(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_owner_scope_province(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				is_valid_index(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot,
				show_condition && is_valid_index(to_prov(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_controller_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::controller_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				is_valid_index(to_prov(primary_slot)) ? provinces::province_controller(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, 
				show_condition && is_valid_index(to_prov(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_location_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::location_of_pop], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			auto loc = bool(to_pop(primary_slot)) ? provinces::province_tag(ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot))) : provinces::province_tag();
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				 loc, this_slot, from_slot, show_condition && bool(to_pop(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_country_scope_state(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_state(primary_slot)) ? ws.w.nation_s.states.get<state::name>(to_state(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_state],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, 
				bool(to_state(primary_slot)) ? nations::state_owner(ws, to_state(primary_slot)) : nations::country_tag(), this_slot, from_slot, show_condition && bool(to_state(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_country_scope_pop(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation_of_pop], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, 
				bool(to_pop(primary_slot)) ? population::get_pop_owner(ws, to_pop(primary_slot)) : nations::country_tag(), this_slot, from_slot, show_condition && bool(to_pop(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_capital_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::capital_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			auto loc = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot)) : provinces::province_tag();
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, 
				loc, this_slot, from_slot, show_condition && bool(to_nation(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_this_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				this_slot, this_slot, from_slot, show_condition && bool(to_nation(this_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_this_scope_state(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				bool(to_state(this_slot)) ? ws.w.nation_s.states.get<state::name>(to_state(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_state],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				this_slot, this_slot, from_slot, show_condition && bool(to_state(this_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_this_scope_province(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(to_prov(this_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_province],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				this_slot, this_slot, from_slot, show_condition && bool(to_prov(this_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_this_scope_pop(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				ws.s.fixed_ui_text[scenario::fixed_ui::this_pop],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				this_slot, this_slot, from_slot, show_condition && bool(to_pop(this_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_from_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				from_slot, this_slot, from_slot, show_condition && bool(to_nation(from_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_from_scope_state(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				bool(to_state(from_slot)) ? ws.w.nation_s.states.get<state::name>(to_state(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_state],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				from_slot, this_slot, from_slot, show_condition && bool(to_state(from_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_from_scope_province(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				bool(to_prov(from_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_province],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				from_slot, this_slot, from_slot, show_condition && bool(to_prov(from_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_from_scope_pop(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				ws.s.fixed_ui_text[scenario::fixed_ui::from_pop],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				from_slot, this_slot, from_slot, show_condition && bool(to_pop(from_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_sea_zone_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in,
				ws.s.fixed_ui_text[scenario::fixed_ui::adjacent_sea],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(to_prov(primary_slot));
			if(sea_zones.first != sea_zones.second) {
				cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
					*sea_zones.first, this_slot, from_slot, show_condition);
			}
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_cultural_union_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			auto prim_culture = to_nation(primary_slot) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot)) : cultures::culture_tag();
			auto cg = is_valid_index(prim_culture) ? ws.s.culture_m.culture_container[prim_culture].group : cultures::culture_group_tag();

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::cultural_union_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			if(is_valid_index(cg)) {
				auto union_tag = ws.s.culture_m.culture_groups[cg].union_tag;
				if(is_valid_index(union_tag)) {
					auto union_holder = ws.w.culture_s.tags_to_holders[union_tag];
					cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
						union_holder, this_slot, from_slot, show_condition && bool(union_holder));
				}
			}
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_overlord_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::overlord_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::overlord>(to_nation(primary_slot)) : nations::country_tag(), this_slot, from_slot, show_condition && bool(to_nation(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_sphere_owner_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::sphere_leader>(to_nation(primary_slot)) : nations::country_tag(), this_slot, from_slot, show_condition && bool(to_nation(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_independence_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
			auto ination = is_valid_index(rtag) ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reb_independence_nation], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				ination, this_slot, from_slot, show_condition && bool(ination));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_flashpoint_tag_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			auto ctag = bool(to_state(primary_slot)) ? ws.w.nation_s.states.get<state::crisis_tag>(to_state(primary_slot)) : cultures::national_tag();
			auto ination = is_valid_index(ctag) ? ws.w.culture_s.tags_to_holders[ctag] : nations::country_tag();

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::flashpoint_nation], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				ination, this_slot, from_slot, show_condition && bool(ination));
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair tf_crisis_state_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_state], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			auto cstate = ws.w.current_crisis.state;
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				cstate, this_slot, from_slot, show_condition && is_valid_index(cstate));
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair tf_state_scope_province(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::containing_state], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_state(ws, to_prov(primary_slot)) : nations::state_tag(), this_slot, from_slot, show_condition && bool(to_prov(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_state_scope_pop(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::containing_state], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				ws.s.fixed_ui_text[scenario::fixed_ui::singular_pop],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			auto pop_province = bool(to_pop(primary_slot)) ? provinces::province_tag(ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot))) : provinces::province_tag();
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				is_valid_index(pop_province) ? provinces::province_state(ws, pop_province) : nations::state_tag(), this_slot, from_slot, show_condition && is_valid_index(pop_province));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_tag_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			auto tag = trigger_payload(tval[2]).tag;
			auto tag_holder = ws.w.culture_s.tags_to_holders[tag];

			cursor_in = ui::add_text(cursor_in, tag_holder ? ws.w.nation_s.nations.get<nation::name>(tag_holder) : ws.s.culture_m.national_tags[tag].default_name.name, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			auto pop_province = bool(to_pop(primary_slot)) ? provinces::province_tag(ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot))) : provinces::province_tag();
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				tag_holder, this_slot, from_slot, show_condition);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_integer_scope(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			provinces::province_tag ptag(tval[2]);

			cursor_in = ui::add_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(ptag), fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			auto pop_province = bool(to_pop(primary_slot)) ? provinces::province_tag(ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot))) : provinces::province_tag();
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				ptag, this_slot, from_slot, show_condition);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_country_scope_nation(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			if(count_subtriggers(tval) > 1) {
				//cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}
			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, show_condition);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_country_scope_province(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, show_condition && bool(to_prov(primary_slot)));
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair tf_cultural_union_scope_pop(TRIGGER_DISPLAY_PARAMS, bool show_condition) {
			auto prim_culture = to_pop(primary_slot) ? ws.w.population_s.pops.get<pop::culture>(to_pop(primary_slot)) : cultures::culture_tag();
			auto cg = is_valid_index(prim_culture) ? ws.s.culture_m.culture_container[prim_culture].group : cultures::culture_group_tag();

			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::cultural_union_of], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in,
				is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation],
				fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(count_subtriggers(tval) > 1) {
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt, 3);
				cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[any_all_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}

			lm.increase_indent(1);
			if(is_valid_index(cg)) {
				auto union_tag = ws.s.culture_m.culture_groups[cg].union_tag;
				if(is_valid_index(union_tag)) {
					auto union_holder = ws.w.culture_s.tags_to_holders[union_tag];
					cursor_in = display_subtriggers(tval, ws, container, cursor_in, lm, fmt,
						union_holder, this_slot, from_slot, show_condition && bool(union_holder));
				}
			}
			lm.decrease_indent(1);
			return cursor_in;
		}
	}

	/*
	#define TRIGGER_DISPLAY_PARAMS uint16_t const* tval, world_state& ws, ui::tagged_gui_object container, \
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, void* primary_slot, \
			void* this_slot, void* from_slot, population::rebel_faction* rebel_slot
	*/

	static ui::xy_pair(*scope_functions[])(uint16_t const* tval, world_state& ws, ui::tagged_gui_object,
		ui::xy_pair, ui::unlimited_line_manager&, ui::text_format const&,
		const_parameter, const_parameter, const_parameter, bool) = {
		tf_generic_scope, //constexpr uint16_t generic_scope = 0x0000; // or & and
		tf_x_neighbor_province_scope, //constexpr uint16_t x_neighbor_province_scope = 0x0001;
		tf_x_neighbor_country_scope_nation, //constexpr uint16_t x_neighbor_country_scope_nation = 0x0002;
		tf_x_neighbor_country_scope_pop, //constexpr uint16_t x_neighbor_country_scope_pop = 0x0003;
		tf_x_war_countries_scope_nation, //constexpr uint16_t x_war_countries_scope_nation = 0x0004;
		tf_x_war_countries_scope_pop, //constexpr uint16_t x_war_countries_scope_pop = 0x0005;
		tf_x_greater_power_scope, //constexpr uint16_t x_greater_power_scope = 0x0006;
		tf_x_owned_province_scope_state, //constexpr uint16_t x_owned_province_scope_state = 0x0007;
		tf_x_owned_province_scope_nation, //constexpr uint16_t x_owned_province_scope_nation = 0x0008;
		tf_x_core_scope_province, //constexpr uint16_t x_core_scope_province = 0x0009;
		tf_x_core_scope_nation, //constexpr uint16_t x_core_scope_nation = 0x000A;
		tf_x_state_scope, //constexpr uint16_t x_state_scope = 0x000B;
		tf_x_substate_scope, //constexpr uint16_t x_substate_scope = 0x000C;
		tf_x_sphere_member_scope, //constexpr uint16_t x_sphere_member_scope = 0x000D;
		tf_x_pop_scope_province, //constexpr uint16_t x_pop_scope_province = 0x000E;
		tf_x_pop_scope_state, //constexpr uint16_t x_pop_scope_state = 0x000F;
		tf_x_pop_scope_nation, //constexpr uint16_t x_pop_scope_nation = 0x0010;
		tf_x_provinces_in_variable_region, //constexpr uint16_t x_provinces_in_variable_region = 0x0011; // variable name
		tf_owner_scope_state, //constexpr uint16_t owner_scope_state = 0x0012;
		tf_owner_scope_province, //constexpr uint16_t owner_scope_province = 0x0013;
		tf_controller_scope, //constexpr uint16_t controller_scope = 0x0014;
		tf_location_scope, //constexpr uint16_t location_scope = 0x0015;
		tf_country_scope_state, //constexpr uint16_t country_scope_state = 0x0016;
		tf_country_scope_pop, //constexpr uint16_t country_scope_pop = 0x0017;
		tf_capital_scope, //constexpr uint16_t capital_scope = 0x0018;
		tf_this_scope_pop, //constexpr uint16_t this_scope_pop = 0x0019;
		tf_this_scope_nation, //constexpr uint16_t this_scope_nation = 0x001A;
		tf_this_scope_state, //constexpr uint16_t this_scope_state = 0x001B;
		tf_this_scope_province, //constexpr uint16_t this_scope_province = 0x001C;
		tf_from_scope_pop, //constexpr uint16_t from_scope_pop = 0x001D;
		tf_from_scope_nation, //constexpr uint16_t from_scope_nation = 0x001E;
		tf_from_scope_state, //constexpr uint16_t from_scope_state = 0x001F;
		tf_from_scope_province, //constexpr uint16_t from_scope_province = 0x0020;
		tf_sea_zone_scope, //constexpr uint16_t sea_zone_scope = 0x0021;
		tf_cultural_union_scope, //constexpr uint16_t cultural_union_scope = 0x0022;
		tf_overlord_scope, //constexpr uint16_t overlord_scope = 0x0023;
		tf_sphere_owner_scope, //constexpr uint16_t sphere_owner_scope = 0x0024;
		tf_independence_scope, //constexpr uint16_t independence_scope = 0x0025;
		tf_flashpoint_tag_scope, //constexpr uint16_t flashpoint_tag_scope = 0x0026;
		tf_crisis_state_scope, //constexpr uint16_t crisis_state_scope = 0x0027;
		tf_state_scope_pop, //constexpr uint16_t state_scope_pop = 0x0028;
		tf_state_scope_province, //constexpr uint16_t state_scope_province = 0x0029;
		tf_tag_scope, //constexpr uint16_t tag_scope = 0x002A; // variable name
		tf_integer_scope, //constexpr uint16_t integer_scope = 0x002B; // variable name
		tf_country_scope_nation, //constexpr uint16_t country_scope_nation = 0x002C;
		tf_country_scope_province, //constexpr uint16_t country_scope_province = 0x002D;
		tf_cultural_union_scope_pop, //constexpr uint16_t cultural_union_scope_pop = 0x002E;
	};

	ui::xy_pair tf_year(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::year, int32_t(tval[2]), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_month(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::month, int32_t(tval[2]), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_port(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_port], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rank(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::national_rank, int32_t(tval[2]), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_technology(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_has_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::technology], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		auto tech = trigger_payload(tval[2]).tech;
		cursor_in = ui::add_text(cursor_in, ws.s.technology_m.technologies_container[tech].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_strata_rich(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::strata, ws.s.fixed_ui_text[scenario::fixed_ui::rich], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_strata_middle(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::strata, ws.s.fixed_ui_text[scenario::fixed_ui::middle], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_strata_poor(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::strata, ws.s.fixed_ui_text[scenario::fixed_ui::poor], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_life_rating_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::life_rating, trigger_payload(tval[2]).signed_value, display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_life_rating_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::minimum_state_life_raing, trigger_payload(tval[2]).signed_value, display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_empty_adjacent_state_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::an_empy_adj_prov], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_empty_adjacent_state_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::an_empy_adj_prov], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_state_id_province(TRIGGER_DISPLAY_PARAMS) {
		provinces::province_tag pid(tval[2]);

		text_data::text_replacement repl[1] = {
				text_data::text_replacement{
					text_data::value_type::text,
					 ws.w.province_s.province_state_container.get<province_state::name>(pid)
					 }
		};

		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
				scenario::fixed_ui::belongs_to_same_state :
				scenario::fixed_ui::does_not_belong_to_same_state], 
			fmt, ws, container, lm, repl, 1);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_state_id_state(TRIGGER_DISPLAY_PARAMS) {
		provinces::province_tag pid(tval[2]);

		text_data::text_replacement repl[1] = {
				text_data::text_replacement{
					text_data::value_type::text,
					 ws.w.province_s.province_state_container.get<province_state::name>(pid)
					 }
		};

		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
			scenario::fixed_ui::contains_blank :
			scenario::fixed_ui::does_not_contain_blank],
			fmt, ws, container, lm, repl, 1);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_cash_reserves(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0], scenario::fixed_ui::pop_savings, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::times_target_needs_spending], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_unemployment_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_unemployment_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_unemployment_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_unemployment_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_slave_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::slavery, ws.s.fixed_ui_text[scenario::fixed_ui::allowed], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_slave_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_slave_state], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_slave_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::slavery, ws.s.fixed_ui_text[scenario::fixed_ui::allowed], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_slave_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_slave], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_independant(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::an_independent_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_national_minority_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::non_prim_culture_pops], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_national_minority_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::non_prim_culture_pops], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_national_minority_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::non_prim_culture_pops], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_government_nation(TRIGGER_DISPLAY_PARAMS) {
		auto gov = trigger_payload(tval[2]).small.values.government;
		return display_with_comparison(tval[0], scenario::fixed_ui::government, ws.s.governments_m.governments_container[gov].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_government_pop(TRIGGER_DISPLAY_PARAMS) {
		auto gov = trigger_payload(tval[2]).small.values.government;
		return display_with_comparison(tval[0], scenario::fixed_ui::government, ws.s.governments_m.governments_container[gov].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_capital(TRIGGER_DISPLAY_PARAMS) {
		auto prov = provinces::province_tag(tval[2]);
		return display_with_comparison(tval[0], scenario::fixed_ui::capital, ws.w.province_s.province_state_container.get<province_state::name>(prov),
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_tech_school(TRIGGER_DISPLAY_PARAMS) {
		auto t = trigger_payload(tval[2]).nat_mod;
		return display_with_comparison(tval[0], scenario::fixed_ui::technology_school, ws.s.modifiers_m.national_modifiers[t].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_primary_culture(TRIGGER_DISPLAY_PARAMS) {
		auto t = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0], scenario::fixed_ui::primary_culture, ws.s.culture_m.culture_container[t].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_accepted_culture(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		cursor_in = ui::add_text(cursor_in, ws.s.culture_m.culture_container[c].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::an_accepted_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0], scenario::fixed_ui::culture, ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_state(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0], scenario::fixed_ui::dominant_culture, ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_province(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0], scenario::fixed_ui::dominant_culture, ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_nation(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		cursor_in = ui::add_text(cursor_in, ws.s.culture_m.culture_container[c].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_primary_or_accepted], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_pop_reb(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(rholder);

		return display_with_comparison(tval[0], scenario::fixed_ui::culture,
			is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_state_reb(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(rholder);

		return display_with_comparison(tval[0], scenario::fixed_ui::dominant_culture,
			is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_province_reb(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(rholder); 
		
		return display_with_comparison(tval[0], scenario::fixed_ui::dominant_culture,
			is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_nation_reb(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(rholder);

		cursor_in = ui::add_text(cursor_in,
			is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_culture],
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_primary_or_accepted], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_this_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::culture, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_prim_culture],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_from_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::culture, ws.s.fixed_ui_text[scenario::fixed_ui::from_nation_prim_culture],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_this_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::culture, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_prim_culture],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_this_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::culture, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_prim_culture],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_this_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::culture, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_prim_culture],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_nation(TRIGGER_DISPLAY_PARAMS) {
		auto cg = trigger_payload(tval[2]).culture_group;
		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group, ws.s.culture_m.culture_groups[cg].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_pop(TRIGGER_DISPLAY_PARAMS) {
		auto cg = trigger_payload(tval[2]).culture_group;
		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group, ws.s.culture_m.culture_groups[cg].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_reb_nation(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(rholder); 
		auto cg = rc ? ws.s.culture_m.cultures_to_groups[rc] : cultures::culture_group_tag();

		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group,
			is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_culture_group],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_reb_pop(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rc = ws.w.nation_s.nations.get<nation::primary_culture>(rholder);
		auto cg = rc ? ws.s.culture_m.cultures_to_groups[rc] : cultures::culture_group_tag();

		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group,
			is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_culture_group],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_nation_this_nation(TRIGGER_DISPLAY_PARAMS) {
		cultures::culture_tag rc = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot)) : cultures::culture_tag();
		auto cg = is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].group : cultures::culture_group_tag();

		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group,
			is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_culture_group],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_pop_this_nation(TRIGGER_DISPLAY_PARAMS) {
		cultures::culture_tag rc = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot)) : cultures::culture_tag();
		auto cg = is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].group : cultures::culture_group_tag();

		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group,
			is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_culture_group],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_nation_from_nation(TRIGGER_DISPLAY_PARAMS) {
		cultures::culture_tag rc = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(from_slot)) : cultures::culture_tag();
		auto cg = is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].group : cultures::culture_group_tag();

		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group,
			is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation_culture_group],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_pop_from_nation(TRIGGER_DISPLAY_PARAMS) {
		cultures::culture_tag rc = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(from_slot)) : cultures::culture_tag();
		auto cg = is_valid_index(rc) ? ws.s.culture_m.culture_container[rc].group : cultures::culture_group_tag();

		return display_with_comparison(tval[0], scenario::fixed_ui::culture_group,
			is_valid_index(cg) ? ws.s.culture_m.culture_groups[cg].name : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation_culture_group],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_group_nation_this_province(TRIGGER_DISPLAY_PARAMS) {
		return tf_culture_group_nation_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
			bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), const_parameter());
	}
	ui::xy_pair tf_culture_group_pop_this_province(TRIGGER_DISPLAY_PARAMS) {
		return tf_culture_group_pop_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
			bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), const_parameter());
	}
	ui::xy_pair tf_culture_group_nation_this_state(TRIGGER_DISPLAY_PARAMS) {
		return tf_culture_group_nation_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
			bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), const_parameter());
	}
	ui::xy_pair tf_culture_group_pop_this_state(TRIGGER_DISPLAY_PARAMS) {
		return tf_culture_group_pop_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
			bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), const_parameter());
	}
	ui::xy_pair tf_culture_group_nation_this_pop(TRIGGER_DISPLAY_PARAMS) {
		return tf_culture_group_nation_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
			bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), const_parameter());
	}
	ui::xy_pair tf_culture_group_pop_this_pop(TRIGGER_DISPLAY_PARAMS) {
		return tf_culture_group_pop_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
			bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), const_parameter());
	}
	ui::xy_pair tf_religion(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.religion;
		return display_with_comparison(tval[0], scenario::fixed_ui::religion, ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_reb(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rc = ws.w.nation_s.nations.get<nation::national_religion>(rholder);

		return display_with_comparison(tval[0], scenario::fixed_ui::religion,
			is_valid_index(rc) ? ws.s.culture_m.religions[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_religion],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_from_nation(TRIGGER_DISPLAY_PARAMS) {
		cultures::religion_tag rc = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::national_religion>(to_nation(from_slot)) : cultures::religion_tag();
		return display_with_comparison(tval[0], scenario::fixed_ui::religion,
			is_valid_index(rc) ? ws.s.culture_m.religions[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation_religion],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_this_nation(TRIGGER_DISPLAY_PARAMS) {
		cultures::religion_tag rc = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::national_religion>(to_nation(this_slot)) : cultures::religion_tag();
		return display_with_comparison(tval[0], scenario::fixed_ui::religion,
			is_valid_index(rc) ? ws.s.culture_m.religions[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_religion],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		cultures::religion_tag rc = bool(owner) ? ws.w.nation_s.nations.get<nation::national_religion>(owner) : cultures::religion_tag();
		return display_with_comparison(tval[0], scenario::fixed_ui::religion,
			is_valid_index(rc) ? ws.s.culture_m.religions[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_religion],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		cultures::religion_tag rc = bool(owner) ? ws.w.nation_s.nations.get<nation::national_religion>(owner) : cultures::religion_tag();
		return display_with_comparison(tval[0], scenario::fixed_ui::religion,
			is_valid_index(rc) ? ws.s.culture_m.religions[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_religion],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		cultures::religion_tag rc = bool(owner) ? ws.w.nation_s.nations.get<nation::national_religion>(owner) : cultures::religion_tag();
		return display_with_comparison(tval[0], scenario::fixed_ui::religion,
			is_valid_index(rc) ? ws.s.culture_m.religions[rc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_religion],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_terrain_province(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).prov_mod;
		return display_with_comparison(tval[0], scenario::fixed_ui::province_terrain, ws.s.modifiers_m.provincial_modifiers[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_terrain_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).prov_mod;
		return display_with_comparison(tval[0], scenario::fixed_ui::province_terrain, ws.s.modifiers_m.provincial_modifiers[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_trade_goods(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.good;
		return display_with_comparison(tval[0], scenario::fixed_ui::rgo_production, ws.s.economy_m.goods[c].name,
			ws, container, cursor_in, lm, fmt);
	}

	ui::xy_pair tf_is_secondary_power_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_secondary_power], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_secondary_power_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::owner, ws.s.fixed_ui_text[scenario::fixed_ui::a_secondary_power], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_faction_nation(TRIGGER_DISPLAY_PARAMS) {
		auto rebel_type = trigger_payload(tval[2]).small.values.rebel;
		return display_with_comparison(tval[0], ws.s.population_m.rebel_types[rebel_type].name, scenario::fixed_ui::an_active_rebel,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_faction_pop(TRIGGER_DISPLAY_PARAMS) {
		auto rebel_type = trigger_payload(tval[2]).small.values.rebel;
		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_member_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.rebel_types[rebel_type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_unclaimed_cores(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::unclaimed_cores],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_bool(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_cultural_union],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_this_self_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_pop, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_pop_this_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_pop, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_this_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_this_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_this_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_this_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_this_rebel(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, ws.s.fixed_ui_text[scenario::fixed_ui::the_rebel_ind_nation], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_tag_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto hname = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, hname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_tag_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto hname = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, hname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_tag_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto hname = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, hname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_tag_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto hname = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, hname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_cultural_union_tag_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto hname = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;
		return display_with_comparison(tval[0], scenario::fixed_ui::union_for_nation, hname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_factory_nation(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
				scenario::fixed_ui::capitalists_can_build :
				scenario::fixed_ui::capitalists_cannot_build],
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_can_build_factory_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
			scenario::fixed_ui::capitalists_can_build :
			scenario::fixed_ui::capitalists_cannot_build],
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_can_build_factory_pop(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
			scenario::fixed_ui::capitalists_can_build :
			scenario::fixed_ui::capitalists_cannot_build],
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_war_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::at_war], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_war_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::at_war], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_war_exhaustion_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::war_exhaustion, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_war_exhaustion_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::war_exhaustion, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_war_exhaustion_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::war_exhaustion, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_blockade(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::total_blockade, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_owns(TRIGGER_DISPLAY_PARAMS) {
		provinces::province_tag prov(tval[2]);

		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
			scenario::fixed_ui::owns :
			scenario::fixed_ui::does_not_own],
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,ws.w.province_s.province_state_container.get<province_state::name>(prov),
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controls(TRIGGER_DISPLAY_PARAMS) {
		provinces::province_tag prov(tval[2]);

		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
			scenario::fixed_ui::controls :
			scenario::fixed_ui::does_not_control],
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(prov),
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_integer(TRIGGER_DISPLAY_PARAMS) {
		provinces::province_tag prov(tval[2]);
		cursor_in = display_with_has_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(prov),
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ?  nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_reb(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto reb_holder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		
		auto rnam = is_valid_index(rtag) ? 
			(bool(reb_holder) ? ws.w.nation_s.nations.get<nation::name>(reb_holder) : ws.s.culture_m.national_tags[rtag].default_name.name) :
			ws.s.fixed_ui_text[scenario::fixed_ui::the_rebel_ind_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, rnam,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_core_tag(TRIGGER_DISPLAY_PARAMS) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto nam = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::a_core_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, nam, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_num_of_revolts(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_of_reb_control, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_revolt_percentage(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::percentage_reb_control, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_num_of_cities_int(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_owned_provinces, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_num_of_cities_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::num_owned_provinces, ws.s.fixed_ui_text[scenario::fixed_ui::num_provinces_owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_num_of_cities_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::num_owned_provinces, ws.s.fixed_ui_text[scenario::fixed_ui::num_provinces_owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_num_of_cities_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::num_owned_provinces, ws.s.fixed_ui_text[scenario::fixed_ui::num_provinces_owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_num_of_cities_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::num_owned_provinces, ws.s.fixed_ui_text[scenario::fixed_ui::num_provinces_owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_num_of_cities_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::num_owned_provinces, ws.s.fixed_ui_text[scenario::fixed_ui::num_provinces_owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_num_of_ports(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_of_ports_connected, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_num_of_allies(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_of_allies, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_num_of_vassals(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_of_vassals, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_owned_by_tag(TRIGGER_DISPLAY_PARAMS) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto nam = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, nam, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_owned_by_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_owned_by_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_owned_by_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_owned_by_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_owned_by_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::owned_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_exists_bool(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
			scenario::fixed_ui::exists :
			scenario::fixed_ui::does_not_exist],
			fmt, ws, container, lm);
		
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_exists_tag(TRIGGER_DISPLAY_PARAMS) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto nam = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

		cursor_in = ui::add_text(cursor_in, nam, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.fixed_ui_text[(tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq ?
			scenario::fixed_ui::exists :
			scenario::fixed_ui::does_not_exist],
			fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_country_flag(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.variables_m.national_flag_to_name[trigger_payload(tval[2]).nat_flag], scenario::fixed_ui::set, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_country_flag_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.variables_m.national_flag_to_name[trigger_payload(tval[2]).nat_flag], scenario::fixed_ui::set, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_country_flag_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.variables_m.national_flag_to_name[trigger_payload(tval[2]).nat_flag], scenario::fixed_ui::set, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_continent_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_state(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_nation(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::capital, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_pop(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_nation_this(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::this_nation, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_state_this(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::this_state, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_province_this(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::this_province, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_pop_this(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::this_pop, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_nation_from(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::from_nation, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_state_from(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::from_state, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_province_from(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::from_province, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_continent_pop_from(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::from_pop, ws.s.fixed_ui_text[scenario::fixed_ui::fx_in], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in,
			ws.s.modifiers_m.provincial_modifiers[trigger_payload(tval[2]).prov_mod].name,
			fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_casus_belli_tag(TRIGGER_DISPLAY_PARAMS) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto nam = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			 ws.s.fixed_ui_text[scenario::fixed_ui::a_casus_belli_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, nam, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_casus_belli_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_casus_belli_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_casus_belli_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_casus_belli_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_casus_belli_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_nation(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_casus_belli_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_casus_belli_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_casus_belli_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_casus_belli_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_casus_belli_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_access_tag(TRIGGER_DISPLAY_PARAMS) {
		auto holder = ws.w.culture_s.tags_to_holders[trigger_payload(tval[2]).tag];
		auto nam = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::military_access_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, nam, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_access_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::military_access_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_access_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::military_access_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_access_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::military_access_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_access_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::military_access_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_access_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::military_access_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_prestige_value(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::prestige, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_prestige_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		cursor_in = display_with_comparison_no_newline(tval[0], scenario::fixed_ui::prestige, ws.s.fixed_ui_text[scenario::fixed_ui::prestige_of],
			ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_prestige_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], scenario::fixed_ui::prestige, ws.s.fixed_ui_text[scenario::fixed_ui::prestige_of],
			ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_prestige_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ?  nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], scenario::fixed_ui::prestige, ws.s.fixed_ui_text[scenario::fixed_ui::prestige_of],
			ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_prestige_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], scenario::fixed_ui::prestige, ws.s.fixed_ui_text[scenario::fixed_ui::prestige_of],
			ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_prestige_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0], scenario::fixed_ui::prestige, ws.s.fixed_ui_text[scenario::fixed_ui::prestige_of],
			ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_badboy(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::infamy, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_building_fort(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_fort], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_building_railroad(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_railroad], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_building_naval_base(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_naval_base], ws, container, cursor_in, lm, fmt);
	}

	ui::xy_pair tf_has_building_factory(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_factory], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_building_state(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.factory;
		return display_with_has_comparison(tval[0],
			ws.s.economy_m.factory_types[type].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_building_state_from_province(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.factory;
		return display_with_has_comparison(tval[0],
			ws.s.economy_m.factory_types[type].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_building_factory_from_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_factory], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_empty(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::empty], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_blockaded(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::blockaded], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_country_modifier(TRIGGER_DISPLAY_PARAMS) {
		const auto mod = trigger_payload(tval[2]).nat_mod;

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::national_modifier], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.national_modifiers[mod].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_country_modifier_province(TRIGGER_DISPLAY_PARAMS) {
		const auto mod = trigger_payload(tval[2]).nat_mod;

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::national_modifier], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.national_modifiers[mod].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_province_modifier(TRIGGER_DISPLAY_PARAMS) {
		const auto mod = trigger_payload(tval[2]).prov_mod;

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::national_modifier], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.provincial_modifiers[mod].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_region(TRIGGER_DISPLAY_PARAMS) {
		auto s = trigger_payload(tval[2]).state;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::part_of, ws.s.province_m.state_names[s], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_tag_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;
		return display_with_comparison(tval[0],
			name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_tag_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_tag_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_tag_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_tag_from_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_tag_pop(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		return display_with_comparison(tval[0],
			scenario::fixed_ui::owner, name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_neighbour_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_border_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_neighbour_this(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_border_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_neighbour_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_border_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_units_in_province_value(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::units_in_province, tval[2], display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_units_in_province_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::units_in_province], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_units_in_province_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::units_in_province], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_units_in_province_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::units_in_province], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_units_in_province_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::units_in_province], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_units_in_province_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			tname, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::units_in_province], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_war_with_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::at_war_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_war_with_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::at_war_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_war_with_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::at_war_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_war_with_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::at_war_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_war_with_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::at_war_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_war_with_this_pop(TRIGGER_DISPLAY_PARAMS) {;
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::at_war_against], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_unit_in_battle(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::an_ongoing_battle], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_total_amount_of_divisions(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			scenario::fixed_ui::num_of_divisions, tval[2], display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_money(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::treasury, read_float_from_payload(tval + 2), display_type::currency, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_lost_national(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::percentage_unowned_cores, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_vassal(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_vassal], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_ruling_party_ideology_nation(TRIGGER_DISPLAY_PARAMS) {
		auto ideology = trigger_payload(tval[2]).small.values.ideology;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::ruling_party_ideology, ws.s.ideologies_m.ideology_container[ideology].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_ruling_party_ideology_pop(TRIGGER_DISPLAY_PARAMS) {
		auto ideology = trigger_payload(tval[2]).small.values.ideology;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::ruling_party_ideology, ws.s.ideologies_m.ideology_container[ideology].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_ruling_party(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::ruling_party, trigger_payload(tval[2]).text, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_ideology_enabled(TRIGGER_DISPLAY_PARAMS) {
		auto id = trigger_payload(tval[2]).small.values.ideology;
		return display_with_comparison(tval[0],
			ws.s.ideologies_m.ideology_container[id].name, scenario::fixed_ui::enabled, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_political_reform_want_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::political_reform_desire, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_political_reform_want_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::political_reform_desire, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_social_reform_want_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::social_reform_desire, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_social_reform_want_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::social_reform_desire, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_total_amount_of_ships(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::num_of_ships, tval[2], display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_plurality(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::plurality, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_corruption(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::crime_percent, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_state_religion_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::member_of_nat_religion], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_state_religion_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, ws.s.fixed_ui_text[scenario::fixed_ui::dominant_religion], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_state_religion_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, ws.s.fixed_ui_text[scenario::fixed_ui::dominant_religion], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, ws.s.fixed_ui_text[scenario::fixed_ui::owner_primary_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, ws.s.fixed_ui_text[scenario::fixed_ui::owner_primary_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, ws.s.fixed_ui_text[scenario::fixed_ui::owner_primary_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_nation_this_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::primary_culture, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_culture], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_nation_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tc = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot)) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::primary_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_nation_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::primary_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_nation_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::primary_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_state_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tc = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot)) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_state_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_state_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_state_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_province_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tc = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot)) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_province_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_province_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_province_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_pop_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tc = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot)) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_pop_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_pop_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_primary_culture_pop_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto this_owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tc = bool(this_owner) ? ws.w.nation_s.nations.get<nation::primary_culture>(this_owner) : cultures::culture_tag();
		auto t = is_valid_index(tc) ? ws.s.culture_m.culture_container[tc].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_primary_culture];

		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, t, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_accepted_culture_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, ws.s.fixed_ui_text[scenario::fixed_ui::a_primary_or_accepted], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_accepted_culture_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, ws.s.fixed_ui_text[scenario::fixed_ui::a_primary_or_accepted], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_accepted_culture_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, ws.s.fixed_ui_text[scenario::fixed_ui::a_primary_or_accepted], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_coastal(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::coastal], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_in_sphere_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_sphere_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_sphere_from(TRIGGER_DISPLAY_PARAMS) {
		auto name = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_sphere_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_sphere_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto name = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_sphere_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_sphere_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto name = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_sphere_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_sphere_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto name = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_sphere_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_sphere_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto name = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_sphere_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_produces_nation(TRIGGER_DISPLAY_PARAMS) {
		auto good = trigger_payload(tval[2]).small.values.good;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_producer_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.economy_m.goods[good].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_produces_province(TRIGGER_DISPLAY_PARAMS) {
		auto good = trigger_payload(tval[2]).small.values.good;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_producer_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.economy_m.goods[good].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_produces_state(TRIGGER_DISPLAY_PARAMS) {
		auto good = trigger_payload(tval[2]).small.values.good;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_producer_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.economy_m.goods[good].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}

	ui::xy_pair tf_produces_pop(TRIGGER_DISPLAY_PARAMS) {
		auto good = trigger_payload(tval[2]).small.values.good;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_producer_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.economy_m.goods[good].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_average_militancy_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::militancy, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_average_militancy_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::militancy, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_average_militancy_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::militancy, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_average_consciousness_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::consciousness, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_average_consciousness_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::consciousness, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_average_consciousness_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::consciousness, read_float_from_payload(tval + 2), display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_next_reform_nation(TRIGGER_DISPLAY_PARAMS) {
		auto reform_id = trigger_payload(tval[2]).small.values.option;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::next_reform, ws.s.issues_m.options[reform_id].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_next_reform_pop(TRIGGER_DISPLAY_PARAMS) {
		auto reform_id = trigger_payload(tval[2]).small.values.option;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::next_reform, ws.s.issues_m.options[reform_id].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rebel_power_fraction(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::never], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_recruited_percentage_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::percentage_recruited, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_recruited_percentage_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::percentage_recruited, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_culture_core(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_cultural_core_prov], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_culture_core_province_this_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::this_pop, ws.s.fixed_ui_text[scenario::fixed_ui::in_cultural_core_prov], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_nationalism(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::nationalism, tval[2], display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_overseas(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::overseas], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_controlled_by_rebels(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by_rebel], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_controlled_by_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controlled_by_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controlled_by_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controlled_by_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controlled_by_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controlled_by_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controlled_by_owner(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_controlled_by_reb(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::controlled_by_rebel], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_canal_enabled(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			std::get<2>(ws.s.province_m.canals[tval[2] - 1]), scenario::fixed_ui::enabled, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_state_capital(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::state_capital], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_truce_with_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_truce_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_truce_with_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_truce_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_truce_with_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_truce_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_truce_with_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_truce_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_truce_with_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_truce_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_truce_with_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_truce_with], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_total_pops_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::population, read_float_from_payload(tval + 2), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_total_pops_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::population, read_float_from_payload(tval + 2), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_total_pops_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::population, read_float_from_payload(tval + 2), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_total_pops_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::size, read_float_from_payload(tval + 2), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_pop_type_nation(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::pops_of_type], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_type_state(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::pops_of_type], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_type_province(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::pops_of_type], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_type_pop(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::of_type_plain], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_empty_adjacent_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::an_empy_adj_prov], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_leader(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_leader_named], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, trigger_payload(tval[2]).text, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_ai(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::ai_controlled], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_create_vassals(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_possible_vassal(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		return display_with_comparison(tval[0],
			name, scenario::fixed_ui::a_possible_vassal, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_province_id(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.w.province_s.province_state_container.get<province_state::name>(provinces::province_tag(tval[2])), ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_vassal_of_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_vassal_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_vassal_of_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_vassal_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_vassal_of_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_vassal_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_vassal_of_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_vassal_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_vassal_of_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_vassal_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_vassal_of_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_vassal_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_substate_of_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_substate_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_substate_of_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_substate_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_substate_of_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_substate_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_substate_of_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_substate_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_substate_of_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_substate_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_substate_of_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_substate_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_alliance_with_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::allied_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_alliance_with_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::allied_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_alliance_with_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::allied_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_alliance_with_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::allied_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_alliance_with_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::allied_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_alliance_with_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::allied_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_recently_lost_war(TRIGGER_DISPLAY_PARAMS) {
		if((tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::has_recently_lost_war], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		} else {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::has_not_recently_lost_war], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
	}
	ui::xy_pair tf_is_mobilised(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::mobilized], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_mobilisation_size(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::mobilization_size, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_crime_higher_than_education_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::admin_spending, ws.s.fixed_ui_text[scenario::fixed_ui::greater_than_edu_spending], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_crime_higher_than_education_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::admin_spending, ws.s.fixed_ui_text[scenario::fixed_ui::greater_than_edu_spending], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_crime_higher_than_education_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::admin_spending, ws.s.fixed_ui_text[scenario::fixed_ui::greater_than_edu_spending], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_crime_higher_than_education_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::admin_spending, ws.s.fixed_ui_text[scenario::fixed_ui::greater_than_edu_spending], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_agree_with_ruling_party(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support_for_rp, read_float_from_payload(tval + 2), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_colonial_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::colonial], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_colonial_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::colonial], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_factories(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_factory], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_in_default_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_default_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_default_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_default_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_default_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_default_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_default_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_default_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_default_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_default_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_in_default_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_default_to], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_total_num_of_ports(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_of_ports, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_always(TRIGGER_DISPLAY_PARAMS) {
		if((tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq) 
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::always], fmt, ws, container, lm);
		else
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::never], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_election(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::holding_an_election],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_global_flag(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.variables_m.global_variable_to_name[trigger_payload(tval[2]).global_var], scenario::fixed_ui::set_globally, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_capital(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::capital],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_nationalvalue_nation(TRIGGER_DISPLAY_PARAMS) {
		auto mod = trigger_payload(tval[2]).nat_mod;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_value, ws.s.modifiers_m.national_modifiers[mod].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_nationalvalue_pop(TRIGGER_DISPLAY_PARAMS) {
		auto mod = trigger_payload(tval[2]).nat_mod;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_value, ws.s.modifiers_m.national_modifiers[mod].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_nationalvalue_province(TRIGGER_DISPLAY_PARAMS) {
		auto mod = trigger_payload(tval[2]).nat_mod;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_value, ws.s.modifiers_m.national_modifiers[mod].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_industrial_score_value(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::industrial_score, trigger_payload(tval[2]).signed_value, display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_industrial_score_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::industrial_score, ws.s.fixed_ui_text[scenario::fixed_ui::industrial_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_industrial_score_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::industrial_score, ws.s.fixed_ui_text[scenario::fixed_ui::industrial_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_industrial_score_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::industrial_score, ws.s.fixed_ui_text[scenario::fixed_ui::industrial_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_industrial_score_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::industrial_score, ws.s.fixed_ui_text[scenario::fixed_ui::industrial_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_industrial_score_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::industrial_score, ws.s.fixed_ui_text[scenario::fixed_ui::industrial_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_score_value(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::military_score, trigger_payload(tval[2]).signed_value, display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_military_score_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::military_score, ws.s.fixed_ui_text[scenario::fixed_ui::military_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_score_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::military_score, ws.s.fixed_ui_text[scenario::fixed_ui::military_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_score_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::military_score, ws.s.fixed_ui_text[scenario::fixed_ui::military_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_score_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::military_score, ws.s.fixed_ui_text[scenario::fixed_ui::military_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_military_score_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::military_score, ws.s.fixed_ui_text[scenario::fixed_ui::military_score_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_civilized_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::civilized],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_civilized_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::civilized],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_civilized_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::civilized],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_national_provinces_occupied(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::percent_occupied, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_greater_power_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::great_power],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_greater_power_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::great_power],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_greater_power_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::great_power],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_tax(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::rich_tax, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_tax(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::middle_tax, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_tax(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::poor_tax, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_social_spending_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::social_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_social_spending_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::social_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_social_spending_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::social_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_military_spending_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::mil_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_military_spending_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::mil_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_military_spending_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::mil_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_military_spending_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::mil_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_administration_spending_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::admin_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_administration_spending_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::admin_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_administration_spending_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::admin_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_administration_spending_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::admin_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_education_spending_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::edu_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_education_spending_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::edu_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_education_spending_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::edu_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_education_spending_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::edu_spending, float(trigger_payload(tval[2]).signed_value) / 100.0f, display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_colonial_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0], ws.s.fixed_ui_text[scenario::fixed_ui::colonies],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_religion_nation(TRIGGER_DISPLAY_PARAMS) {
		auto rel = trigger_payload(tval[2]).small.values.religion;
		return display_with_comparison(tval[0],
			ws.s.culture_m.religions[rel].name, scenario::fixed_ui::dominant_religion,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_religion_state(TRIGGER_DISPLAY_PARAMS) {
		auto rel = trigger_payload(tval[2]).small.values.religion;
		return display_with_comparison(tval[0],
			ws.s.culture_m.religions[rel].name, scenario::fixed_ui::dominant_religion,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_religion_province(TRIGGER_DISPLAY_PARAMS) {
		auto rel = trigger_payload(tval[2]).small.values.religion;
		return display_with_comparison(tval[0],
			ws.s.culture_m.religions[rel].name, scenario::fixed_ui::dominant_religion,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_culture_nation(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, ws.s.culture_m.culture_container[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_culture_state(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, ws.s.culture_m.culture_container[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_culture_province(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_culture, ws.s.culture_m.culture_container[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_issue_nation(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.option;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.issues_m.options[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_issue_state(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.option;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.issues_m.options[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_issue_province(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.option;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.issues_m.options[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_issue_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.option;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.issues_m.options[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_ideology_nation(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.ideology;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.ideologies_m.ideology_container[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_ideology_state(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.ideology;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.ideologies_m.ideology_container[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_ideology_province(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.ideology;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.ideologies_m.ideology_container[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_majority_ideology_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.ideology;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::dominant_issue, ws.s.ideologies_m.ideology_container[c].name,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_militancy_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::poor_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_militancy_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::poor_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_militancy_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::poor_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_militancy_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::poor_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_militancy_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::middle_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_militancy_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::middle_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_militancy_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::middle_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_militancy_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::middle_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_militancy_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::rich_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_militancy_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::rich_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_militancy_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::rich_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_militancy_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::rich_militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_tax_above_poor(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_tax, ws.s.fixed_ui_text[scenario::fixed_ui::gt_poor_tax],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_has_union_tag_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_cultural_union],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_culture_has_union_tag_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_cultural_union],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_this_culture_union_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		return display_with_comparison(tval[0],
			scenario::fixed_ui::cultural_union, name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_this_culture_union_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::cultural_union, tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_this_culture_union_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::cultural_union, tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_this_culture_union_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::cultural_union, tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_this_culture_union_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::cultural_union, tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_this_culture_union_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::cultural_union, tname, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_this_culture_union_this_union_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::cultural_union, ws.s.fixed_ui_text[scenario::fixed_ui::same_cultural_union], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_this_culture_union_this_union_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::cultural_union, ws.s.fixed_ui_text[scenario::fixed_ui::same_cultural_union], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_this_culture_union_this_union_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::cultural_union, ws.s.fixed_ui_text[scenario::fixed_ui::same_cultural_union], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_this_culture_union_this_union_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::cultural_union, ws.s.fixed_ui_text[scenario::fixed_ui::same_cultural_union], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_minorities_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::non_accepted_pops],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_minorities_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::non_accepted_pops],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_minorities_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::non_accepted_pops],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_revanchism_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::revanchism, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_revanchism_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::revanchism, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_crime(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::crime],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_num_of_substates(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_substates, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_num_of_vassals_no_substates(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0], scenario::fixed_ui::num_vassals_no_substates, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_brigades_compare_this(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::num_regiments, ws.s.fixed_ui_text[scenario::fixed_ui::num_regiments_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_brigades_compare_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			scenario::fixed_ui::num_regiments, ws.s.fixed_ui_text[scenario::fixed_ui::num_regiments_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fabricating_on], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fabricating_on], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fabricating_on], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fabricating_on], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fabricating_on], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::fabricating_on], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_discovered(TRIGGER_DISPLAY_PARAMS) {
		if((tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq)
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::cb_discovered], fmt, ws, container, lm);
		else
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::cb_not_discovered], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_constructing_cb_progress(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::cb_progress, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_civilization_progress(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::civ_progress, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_constructing_cb_type(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.cb_type;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::constructing_a], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.military_m.cb_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::casus_belli], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_our_vassal_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;
		auto scope_name = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::nation_in_scope];

		cursor_in = display_with_comparison_no_newline(tval[0],
			name, scenario::fixed_ui::a_vassal_of, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, scope_name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_our_vassal_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		auto scope_name = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::nation_in_scope];

		cursor_in = display_with_comparison_no_newline(tval[0],
			tname, scenario::fixed_ui::a_vassal_of, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, scope_name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_our_vassal_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		auto scope_name = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::nation_in_scope];

		cursor_in = display_with_comparison_no_newline(tval[0],
			tname, scenario::fixed_ui::a_vassal_of, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, scope_name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_our_vassal_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		auto scope_name = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::nation_in_scope];

		cursor_in = display_with_comparison_no_newline(tval[0],
			tname, scenario::fixed_ui::a_vassal_of, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, scope_name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_our_vassal_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		auto scope_name = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::nation_in_scope];

		cursor_in = display_with_comparison_no_newline(tval[0],
			tname, scenario::fixed_ui::a_vassal_of, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, scope_name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_our_vassal_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		auto scope_name = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::nation_in_scope];

		cursor_in = display_with_comparison_no_newline(tval[0],
			tname, scenario::fixed_ui::a_vassal_of, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, scope_name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_substate(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_substate],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_great_wars_enabled(TRIGGER_DISPLAY_PARAMS) {
		if((tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq)
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::gw_enabled], fmt, ws, container, lm);
		else
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::gw_not_enabled], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_can_nationalize(TRIGGER_DISPLAY_PARAMS) {
		if((tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq)
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::can_perform_nationalization], fmt, ws, container, lm);
		else
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::cannot_perform_nationalization], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_part_of_sphere(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::in_sphere],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_sphere_leader_of_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[2]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_sphere_leader_of_from(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_sphere_leader_of_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_sphere_leader_of_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_sphere_leader_of_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_is_sphere_leader_of_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = display_with_comparison_no_newline(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_number_of_states(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::num_states, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_war_score(TRIGGER_DISPLAY_PARAMS) {
		//stub for apparently unused trigger
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_releasable_vassal_from(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation], scenario::fixed_ui::a_releasable_vassal, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_releasable_vassal_other(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_releasable_vassal], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_recent_imigration(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::recent_immigration], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_province_control_days(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::num_control_days, tval[2], display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_disarmed(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::disarmed], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_big_producer(TRIGGER_DISPLAY_PARAMS) {
		// stub: used only rarely in ai chances and would be expensive to test
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::never], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_someone_can_form_union_tag_from(TRIGGER_DISPLAY_PARAMS) {
		// stub: apparently unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::never], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_someone_can_form_union_tag_other(TRIGGER_DISPLAY_PARAMS) {
		// stub: apparently unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::never], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_social_movement_strength(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::social_mov_support, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_political_movement_strength(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::political_mov_support, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_factory_in_capital_state(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_social_movement(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_member_of_social_mov], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_political_movement(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::a_member_of_political_mov], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_cultural_sphere(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::cultural_sphere_members], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_world_wars_enabled(TRIGGER_DISPLAY_PARAMS) {
		if((tval[0] & trigger_codes::association_mask) == trigger_codes::association_eq)
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::ww_enabled], fmt, ws, container, lm);
		else
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::ww_not_enabled], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_culture_pop_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot)) : cultures::culture_tag();
		auto cn = is_valid_index(c) ? ws.s.culture_m.culture_container[c].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_culture];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, cn, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_pop_culture_state_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot)) : cultures::culture_tag();
		if(is_valid_index(c)) {
			cursor_in =  display_with_has_comparison_no_newline(tval[0],
				ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		} else {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.fixed_ui_text[scenario::fixed_ui::pops_with], ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_culture], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
	}
	ui::xy_pair tf_has_pop_culture_province_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot)) : cultures::culture_tag();
		if(is_valid_index(c)) {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		} else {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.fixed_ui_text[scenario::fixed_ui::pops_with], ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_culture], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
	}
	ui::xy_pair tf_has_pop_culture_nation_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::culture>(to_pop(this_slot)) : cultures::culture_tag();
		if(is_valid_index(c)) {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		} else {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.fixed_ui_text[scenario::fixed_ui::pops_with], ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_culture], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
	}
	ui::xy_pair tf_has_pop_culture_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		return display_with_comparison(tval[0],
			scenario::fixed_ui::culture, ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_pop_culture_state(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_culture_province(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_culture_nation(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).culture;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.culture_m.culture_container[c].name, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_religion_pop_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot)) : cultures::religion_tag();
		auto cn = is_valid_index(c) ? ws.s.culture_m.religions[c].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_religion];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::religion, cn, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_pop_religion_state_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot)) : cultures::religion_tag();
		if(is_valid_index(c)) {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		} else {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.fixed_ui_text[scenario::fixed_ui::pops_with], ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_religion], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
	}
	ui::xy_pair tf_has_pop_religion_province_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot)) : cultures::religion_tag();
		if(is_valid_index(c)) {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		} else {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.fixed_ui_text[scenario::fixed_ui::pops_with], ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_religion], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
	}
	ui::xy_pair tf_has_pop_religion_nation_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::religion>(to_pop(this_slot)) : cultures::religion_tag();
		if(is_valid_index(c)) {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		} else {
			cursor_in = display_with_has_comparison_no_newline(tval[0],
				ws.s.fixed_ui_text[scenario::fixed_ui::pops_with], ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_religion], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
	}
	ui::xy_pair tf_has_pop_religion_pop(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.religion;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_religion_state(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.religion;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_religion_province(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.religion;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_has_pop_religion_nation(TRIGGER_DISPLAY_PARAMS) {
		auto c = trigger_payload(tval[2]).small.values.religion;
		cursor_in = display_with_has_comparison_no_newline(tval[0],
			ws.s.culture_m.religions[c].name, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pops], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_life_needs(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_everyday_needs(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_luxury_needs(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_consciousness_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::consciousness, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_consciousness_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::consciousness, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_consciousness_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::consciousness, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_consciousness_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::consciousness, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_literacy_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::literacy, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_literacy_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::literacy, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_literacy_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::literacy, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_literacy_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::literacy, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_militancy_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_militancy_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_militancy_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_militancy_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::militancy, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_trade_goods_in_state_state(TRIGGER_DISPLAY_PARAMS) {
		auto g = trigger_payload(tval[2]).small.values.good;
		return display_with_comparison(tval[0],
			ws.s.economy_m.goods[g].name, scenario::fixed_ui::present_in_state,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_trade_goods_in_state_province(TRIGGER_DISPLAY_PARAMS) {
		auto g = trigger_payload(tval[2]).small.values.good;
		return display_with_comparison(tval[0],
			ws.s.economy_m.goods[g].name, scenario::fixed_ui::present_in_state,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_has_flashpoint(TRIGGER_DISPLAY_PARAMS) {
		return display_with_has_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::flashpoint],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_flashpoint_tension(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::flashpoint_tension, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_crisis_exist(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::there, ws.s.fixed_ui_text[scenario::fixed_ui::an_ongoing_crisis],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_liberation_crisis(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::current_crisis, ws.s.fixed_ui_text[scenario::fixed_ui::liberation_crisis],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_is_claim_crisis(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::current_crisis, ws.s.fixed_ui_text[scenario::fixed_ui::claim_crisis],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_crisis_temperature(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::crisis_temperature_plain, read_float_from_payload(tval + 2), display_type::integer,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_involved_in_crisis_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::involved_in_crisis],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_involved_in_crisis_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::involved_in_crisis],
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_life_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_life_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_life_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_life_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_everyday_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_everyday_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_everyday_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_everyday_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_luxury_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_luxury_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_luxury_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_rich_strata_luxury_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::rich_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_life_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_life_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_life_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_life_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_everyday_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_everyday_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_everyday_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_everyday_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_luxury_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_luxury_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_luxury_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_middle_strata_luxury_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::middle_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_life_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_life_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_life_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_life_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_life_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_everyday_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_everyday_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_everyday_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_everyday_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_everyday_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_luxury_needs_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_luxury_needs_state(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_luxury_needs_province(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_poor_strata_luxury_needs_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::poor_luxury_needs_satisfaction, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_diplomatic_influence_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[3]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::dip_influence_over], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, text_data::integer{ tval[2] }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_diplomatic_influence_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::dip_influence_over], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ tval[2] }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_diplomatic_influence_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::dip_influence_over], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ tval[2] }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_diplomatic_influence_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::dip_influence_over], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ tval[2] }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_diplomatic_influence_from_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::dip_influence_over], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ tval[2] }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_pop_unemployment_nation(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_unemployment_state(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_unemployment_province(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_unemployment_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_unemployment_nation_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto type = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::type>(to_pop(this_slot)) : population::pop_type_tag();
		auto tname = is_valid_index(type) ? ws.s.population_m.pop_types[type].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_type];

		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_unemployment_state_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto type = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::type>(to_pop(this_slot)) : population::pop_type_tag();
		auto tname = is_valid_index(type) ? ws.s.population_m.pop_types[type].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_type];

		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_pop_unemployment_province_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto type = bool(to_pop(this_slot)) ? ws.w.population_s.pops.get<pop::type>(to_pop(this_slot)) : population::pop_type_tag();
		auto tname = is_valid_index(type) ? ws.s.population_m.pop_types[type].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_pop_type];

		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_relation_tag(TRIGGER_DISPLAY_PARAMS) {
		auto tag = trigger_payload(tval[3]).tag;
		auto holder = ws.w.culture_s.tags_to_holders[tag];
		auto name = bool(holder) ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[tag].default_name.name;

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::relationship_with], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ trigger_payload(tval[2]).signed_value }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_relation_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::relationship_with], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ trigger_payload(tval[2]).signed_value }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_relation_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::relationship_with], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ trigger_payload(tval[2]).signed_value }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_relation_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::relationship_with], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ trigger_payload(tval[2]).signed_value }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_relation_from_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag();
		auto tname = bool(owner) ? ws.w.nation_s.nations.get<nation::name>(owner) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::relationship_with], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, tname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::integer{ trigger_payload(tval[2]).signed_value }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_check_variable(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.variables_m.national_variable_to_name[triggers::trigger_payload(tval[3]).nat_var], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::nat_variable, read_float_from_payload(tval + 2), display_type::fp_one_place,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_upper_house(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[triggers::trigger_payload(tval[3]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support_in_uh, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_unemployment_by_type_nation(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_unemployment_by_type_state(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_unemployment_by_type_province(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[4]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_unemployment_by_type_pop(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::unemployment, read_float_from_payload(tval + 2), display_type::percent,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_party_loyalty_nation_province_id(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[5]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(provinces::province_tag(tval[2])), fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 3) }, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_party_loyalty_from_nation_province_id(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[5]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(provinces::province_tag(tval[2])), fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 3) }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_party_loyalty_province_province_id(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[5]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(provinces::province_tag(tval[2])), fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 3) }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_party_loyalty_from_province_province_id(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[5]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(provinces::province_tag(tval[2])), fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 3) }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_party_loyalty_nation_from_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[4]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		auto pname = bool(to_prov(from_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_province];
		cursor_in = ui::add_text(cursor_in, pname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 2) }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_party_loyalty_generic(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[4]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		auto pname = is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::scope_province];
		cursor_in = ui::add_text(cursor_in, pname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 2) }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_party_loyalty_from_nation_scope_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[4]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		auto pname = is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::scope_province];
		cursor_in = ui::add_text(cursor_in, pname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 2) }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_party_loyalty_from_province_scope_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[4]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::support_in], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		auto pname = is_valid_index(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::scope_province];
		cursor_in = ui::add_text(cursor_in, pname, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, text_data::percent{ read_float_from_payload(tval + 2) }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_can_build_in_province_railroad_no_limit_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_rr,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_railroad_yes_limit_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_rr,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_railroad_no_limit_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_rr,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_railroad_yes_limit_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_rr,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_fort_no_limit_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_fort,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_fort_yes_limit_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_fort,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_fort_no_limit_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_fort,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_fort_yes_limit_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_fort,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_naval_base_no_limit_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_naval_base,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_naval_base_yes_limit_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_naval_base,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_naval_base_no_limit_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_naval_base,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_in_province_naval_base_yes_limit_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto tname = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
		return display_with_comparison(tval[0],
			tname, scenario::fixed_ui::able_build_naval_base,
			ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_railway_in_capital_yes_whole_state_yes_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_railway_in_capital_yes_whole_state_no_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_railway_in_capital_no_whole_state_yes_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_railway_in_capital_no_whole_state_no_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_fort_in_capital_yes_whole_state_yes_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_fort_in_capital_yes_whole_state_no_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_fort_in_capital_no_whole_state_yes_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_can_build_fort_in_capital_no_whole_state_no_limit(TRIGGER_DISPLAY_PARAMS) {
		// stub: virtually unused
		return display_with_comparison(tval[0],
			ws.s.fixed_ui_text[scenario::fixed_ui::always], ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_work_available_nation(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison(tval[0],
			scenario::fixed_ui::work, ws.s.fixed_ui_text[scenario::fixed_ui::available_for], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		lm.increase_indent(1);
		auto count_workers = tval[1] - 1;
		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair tf_work_available_state(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison(tval[0],
			scenario::fixed_ui::work, ws.s.fixed_ui_text[scenario::fixed_ui::available_for], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		lm.increase_indent(1);
		auto count_workers = tval[1] - 1;
		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair tf_work_available_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = display_with_comparison(tval[0],
			scenario::fixed_ui::work, ws.s.fixed_ui_text[scenario::fixed_ui::available_for], ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		lm.increase_indent(1);
		auto count_workers = tval[1] - 1;
		for(int32_t i = 0; i < count_workers; ++i) {
			auto type = trigger_payload(tval[2 + i]).small.values.pop_type;
			cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair tf_variable_ideology_name_nation(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[2]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_ideology_name_state(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[2]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_ideology_name_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[2]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_ideology_name_pop(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.ideologies_m.ideology_container[trigger_payload(tval[2]).small.values.ideology].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_issue_name_nation(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[trigger_payload(tval[2]).small.values.option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_issue_name_state(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[trigger_payload(tval[2]).small.values.option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_issue_name_province(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[trigger_payload(tval[2]).small.values.option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_issue_name_pop(TRIGGER_DISPLAY_PARAMS) {
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[trigger_payload(tval[2]).small.values.option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::support, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_issue_group_name_nation(TRIGGER_DISPLAY_PARAMS) {
		auto option = trigger_payload(tval[2]).small.values.option;
		auto issue = ws.s.issues_m.options[option].parent_issue;
		
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.issues_container[issue].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		//cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reform], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
		//cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_variable_issue_group_name_state(TRIGGER_DISPLAY_PARAMS) {
		auto option = trigger_payload(tval[2]).small.values.option;
		auto issue = ws.s.issues_m.options[option].parent_issue;

		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.issues_container[issue].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		//cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reform], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
		//cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_variable_issue_group_name_province(TRIGGER_DISPLAY_PARAMS) {
		auto option = trigger_payload(tval[2]).small.values.option;
		auto issue = ws.s.issues_m.options[option].parent_issue;

		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.issues_container[issue].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		//cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reform], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
		//cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_variable_issue_group_name_pop(TRIGGER_DISPLAY_PARAMS) {
		auto option = trigger_payload(tval[2]).small.values.option;
		auto issue = ws.s.issues_m.options[option].parent_issue;

		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.issues_container[issue].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		//cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reform], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
		//cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[cmp_code_to_fixed_ui(tval[0])], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.issues_m.options[option].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		return cursor_in;
	}
	ui::xy_pair tf_variable_pop_type_name_nation(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::percentage, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_pop_type_name_state(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::percentage, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_pop_type_name_province(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::percentage, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_pop_type_name_pop(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.pop_type;
		cursor_in = ui::add_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::percentage, read_float_from_payload(tval + 3), display_type::percent, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_variable_good_name(TRIGGER_DISPLAY_PARAMS) {
		auto type = trigger_payload(tval[2]).small.values.good;
		cursor_in = ui::add_text(cursor_in, ws.s.economy_m.goods[type].name, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		return display_with_comparison(tval[0],
			scenario::fixed_ui::stockpile, read_float_from_payload(tval + 3), display_type::integer, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_nation(TRIGGER_DISPLAY_PARAMS) {
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, ws.s.culture_m.religions[trigger_payload(tval[2]).small.values.religion].name, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_nation_reb(TRIGGER_DISPLAY_PARAMS) {
		auto rtag = population::rebel_faction_tag_to_national_tag(to_rebel(from_slot));
		auto rholder = rtag ? ws.w.culture_s.tags_to_holders[rtag] : nations::country_tag();
		auto rr = ws.w.nation_s.nations.get<nation::national_religion>(rholder);

		auto rn = is_valid_index(rr) ? ws.s.culture_m.religions[rr].name : ws.s.fixed_ui_text[scenario::fixed_ui::rebel_religion];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, rn, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_nation_from_nation(TRIGGER_DISPLAY_PARAMS) {
		auto rr = bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::national_religion>(to_nation(from_slot)) : cultures::religion_tag();
		auto rn = is_valid_index(rr) ? ws.s.culture_m.religions[rr].name : ws.s.fixed_ui_text[scenario::fixed_ui::from_nat_religion];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, rn, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_nation_this_nation(TRIGGER_DISPLAY_PARAMS) {
		auto rr = bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::national_religion>(to_nation(this_slot)) : cultures::religion_tag();
		auto rn = is_valid_index(rr) ? ws.s.culture_m.religions[rr].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nat_religion];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, rn, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_nation_this_state(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag();
		auto rr = bool(owner) ? ws.w.nation_s.nations.get<nation::national_religion>(owner) : cultures::religion_tag();
		auto rn = is_valid_index(rr) ? ws.s.culture_m.religions[rr].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nat_religion];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, rn, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_nation_this_province(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag();
		auto rr = bool(owner) ? ws.w.nation_s.nations.get<nation::national_religion>(owner) : cultures::religion_tag();
		auto rn = is_valid_index(rr) ? ws.s.culture_m.religions[rr].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nat_religion];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, rn, ws, container, cursor_in, lm, fmt);
	}
	ui::xy_pair tf_religion_nation_this_pop(TRIGGER_DISPLAY_PARAMS) {
		auto owner = bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag();
		auto rr = bool(owner) ? ws.w.nation_s.nations.get<nation::national_religion>(owner) : cultures::religion_tag();
		auto rn = is_valid_index(rr) ? ws.s.culture_m.religions[rr].name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nat_religion];
		return display_with_comparison(tval[0],
			scenario::fixed_ui::national_religion, rn, ws, container, cursor_in, lm, fmt);
	}


	/*
	#define TRIGGER_DISPLAY_PARAMS uint16_t const* tval, world_state& ws, ui::tagged_gui_object container, \
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, void* primary_slot, \
			void* this_slot, void* from_slot, population::rebel_faction* rebel_slot
	*/

	ui::xy_pair tf_none_b(TRIGGER_DISPLAY_PARAMS) {
		return cursor_in;
	}

	ui::xy_pair (*trigger_functions[])(uint16_t const*, world_state&, ui::tagged_gui_object, ui::xy_pair, ui::unlimited_line_manager&, ui::text_format const&,
		const_parameter, const_parameter, const_parameter) = {
		tf_none_b,
		tf_year,
		tf_month,
		tf_port,
		tf_rank,
		tf_technology,
		tf_strata_rich,
		tf_life_rating_province,
		tf_life_rating_state,
		tf_has_empty_adjacent_state_province,
		tf_has_empty_adjacent_state_state,
		tf_state_id_province,
		tf_state_id_state,
		tf_cash_reserves,
		tf_unemployment_nation,
		tf_unemployment_state,
		tf_unemployment_province,
		tf_unemployment_pop,
		tf_is_slave_nation,
		tf_is_slave_state,
		tf_is_slave_province,
		tf_is_slave_pop,
		tf_is_independant,
		tf_has_national_minority_province,
		tf_has_national_minority_state,
		tf_has_national_minority_nation,
		tf_government_nation,
		tf_government_pop,
		tf_capital,
		tf_tech_school,
		tf_primary_culture,
		tf_accepted_culture,
		tf_culture_pop,
		tf_culture_state,
		tf_culture_province,
		tf_culture_nation,
		tf_culture_pop_reb,
		tf_culture_state_reb,
		tf_culture_province_reb,
		tf_culture_nation_reb,
		tf_culture_from_nation,
		tf_culture_this_nation,
		tf_culture_this_state,
		tf_culture_this_pop,
		tf_culture_this_province,
		tf_culture_group_nation,
		tf_culture_group_pop,
		tf_culture_group_reb_nation,
		tf_culture_group_reb_pop,
		tf_culture_group_nation_from_nation,
		tf_culture_group_pop_from_nation,
		tf_culture_group_nation_this_nation,
		tf_culture_group_pop_this_nation,
		tf_culture_group_nation_this_province,
		tf_culture_group_pop_this_province,
		tf_culture_group_nation_this_state,
		tf_culture_group_pop_this_state,
		tf_culture_group_nation_this_pop,
		tf_culture_group_pop_this_pop,
		tf_religion,
		tf_religion_reb,
		tf_religion_from_nation,
		tf_religion_this_nation,
		tf_religion_this_state,
		tf_religion_this_province,
		tf_religion_this_pop,
		tf_terrain_province,
		tf_terrain_pop,
		tf_trade_goods,
		tf_is_secondary_power_pop,
		tf_is_secondary_power_nation,
		tf_has_faction_nation,
		tf_has_faction_pop,
		tf_has_unclaimed_cores,
		tf_is_cultural_union_bool,
		tf_is_cultural_union_this_self_pop,
		tf_is_cultural_union_this_pop,
		tf_is_cultural_union_this_state,
		tf_is_cultural_union_this_province,
		tf_is_cultural_union_this_nation,
		tf_is_cultural_union_this_rebel,
		tf_is_cultural_union_tag_nation,
		tf_is_cultural_union_tag_this_pop,
		tf_is_cultural_union_tag_this_state,
		tf_is_cultural_union_tag_this_province,
		tf_is_cultural_union_tag_this_nation,
		tf_can_build_factory_pop,
		tf_war_pop,
		tf_war_nation,
		tf_war_exhaustion_nation,
		tf_blockade,
		tf_owns,
		tf_controls,
		tf_is_core_integer,
		tf_is_core_this_nation,
		tf_is_core_this_state,
		tf_is_core_this_province,
		tf_is_core_this_pop,
		tf_is_core_from_nation,
		tf_is_core_reb,
		tf_is_core_tag,
		tf_num_of_revolts,
		tf_revolt_percentage,
		tf_num_of_cities_int,
		tf_num_of_cities_from_nation,
		tf_num_of_cities_this_nation,
		tf_num_of_cities_this_state,
		tf_num_of_cities_this_province,
		tf_num_of_cities_this_pop,
		tf_num_of_ports,
		tf_num_of_allies,
		tf_num_of_vassals,
		tf_owned_by_tag,
		tf_owned_by_from_nation,
		tf_owned_by_this_nation,
		tf_owned_by_this_province,
		tf_owned_by_this_state,
		tf_owned_by_this_pop,
		tf_exists_bool,
		tf_exists_tag,
		tf_has_country_flag,
		tf_continent_nation,
		tf_continent_state,
		tf_continent_province,
		tf_continent_pop,
		tf_continent_nation_this,
		tf_continent_state_this,
		tf_continent_province_this,
		tf_continent_pop_this,
		tf_continent_nation_from,
		tf_continent_state_from,
		tf_continent_province_from,
		tf_continent_pop_from,
		tf_casus_belli_tag,
		tf_casus_belli_from,
		tf_casus_belli_this_nation,
		tf_casus_belli_this_state,
		tf_casus_belli_this_province,
		tf_casus_belli_this_pop,
		tf_military_access_tag,
		tf_military_access_from,
		tf_military_access_this_nation,
		tf_military_access_this_state,
		tf_military_access_this_province,
		tf_military_access_this_pop,
		tf_prestige_value,
		tf_prestige_from,
		tf_prestige_this_nation,
		tf_prestige_this_state,
		tf_prestige_this_province,
		tf_prestige_this_pop,
		tf_badboy,
		tf_has_building_state,
		tf_has_building_fort,
		tf_has_building_railroad,
		tf_has_building_naval_base,
		tf_empty,
		tf_is_blockaded,
		tf_has_country_modifier,
		tf_has_province_modifier,
		tf_region,
		tf_tag_tag,
		tf_tag_this_nation,
		tf_tag_this_province,
		tf_tag_from_nation,
		tf_tag_from_province,
		tf_neighbour_tag,
		tf_neighbour_this,
		tf_neighbour_from,
		tf_units_in_province_value,
		tf_units_in_province_from,
		tf_units_in_province_this_nation,
		tf_units_in_province_this_province,
		tf_units_in_province_this_state,
		tf_units_in_province_this_pop,
		tf_war_with_tag,
		tf_war_with_from,
		tf_war_with_this_nation,
		tf_war_with_this_province,
		tf_war_with_this_state,
		tf_war_with_this_pop,
		tf_unit_in_battle,
		tf_total_amount_of_divisions,
		tf_money,
		tf_lost_national,
		tf_is_vassal,
		tf_ruling_party_ideology_pop,
		tf_ruling_party_ideology_nation,
		tf_ruling_party,
		tf_is_ideology_enabled,
		tf_political_reform_want_nation,
		tf_political_reform_want_pop,
		tf_social_reform_want_nation,
		tf_social_reform_want_pop,
		tf_total_amount_of_ships,
		tf_plurality,
		tf_corruption,
		tf_is_state_religion_pop,
		tf_is_state_religion_province,
		tf_is_state_religion_state,
		tf_is_primary_culture_pop,
		tf_is_primary_culture_province,
		tf_is_primary_culture_state,
		tf_is_primary_culture_nation_this_pop,
		tf_is_primary_culture_nation_this_nation,
		tf_is_primary_culture_nation_this_state,
		tf_is_primary_culture_nation_this_province,
		tf_is_primary_culture_state_this_pop,
		tf_is_primary_culture_state_this_nation,
		tf_is_primary_culture_state_this_state,
		tf_is_primary_culture_state_this_province,
		tf_is_primary_culture_province_this_pop,
		tf_is_primary_culture_province_this_nation,
		tf_is_primary_culture_province_this_state,
		tf_is_primary_culture_province_this_province,
		tf_is_primary_culture_pop_this_pop,
		tf_is_primary_culture_pop_this_nation,
		tf_is_primary_culture_pop_this_state,
		tf_is_primary_culture_pop_this_province,
		tf_is_accepted_culture_pop,
		tf_is_accepted_culture_province,
		tf_is_accepted_culture_state,
		tf_is_coastal,
		tf_in_sphere_tag,
		tf_in_sphere_from,
		tf_in_sphere_this_nation,
		tf_in_sphere_this_province,
		tf_in_sphere_this_state,
		tf_in_sphere_this_pop,
		tf_produces_nation,
		tf_produces_state,
		tf_produces_province,
		tf_produces_pop,
		tf_average_militancy_nation,
		tf_average_militancy_state,
		tf_average_militancy_province,
		tf_average_consciousness_nation,
		tf_average_consciousness_state,
		tf_average_consciousness_province,
		tf_is_next_reform_nation,
		tf_is_next_reform_pop,
		tf_rebel_power_fraction,
		tf_recruited_percentage_nation,
		tf_recruited_percentage_pop,
		tf_has_culture_core,
		tf_nationalism,
		tf_is_overseas,
		tf_controlled_by_rebels,
		tf_controlled_by_tag,
		tf_controlled_by_from,
		tf_controlled_by_this_nation,
		tf_controlled_by_this_province,
		tf_controlled_by_this_state,
		tf_controlled_by_this_pop,
		tf_controlled_by_owner,
		tf_controlled_by_reb,
		tf_is_canal_enabled,
		tf_is_state_capital,
		tf_truce_with_tag,
		tf_truce_with_from,
		tf_truce_with_this_nation,
		tf_truce_with_this_province,
		tf_truce_with_this_state,
		tf_truce_with_this_pop,
		tf_total_pops_nation,
		tf_total_pops_state,
		tf_total_pops_province,
		tf_total_pops_pop,
		tf_has_pop_type_nation,
		tf_has_pop_type_state,
		tf_has_pop_type_province,
		tf_has_pop_type_pop,
		tf_has_empty_adjacent_province,
		tf_has_leader,
		tf_ai,
		tf_can_create_vassals,
		tf_is_possible_vassal,
		tf_province_id,
		tf_vassal_of_tag,
		tf_vassal_of_from,
		tf_vassal_of_this_nation,
		tf_vassal_of_this_province,
		tf_vassal_of_this_state,
		tf_vassal_of_this_pop,
		tf_alliance_with_tag,
		tf_alliance_with_from,
		tf_alliance_with_this_nation,
		tf_alliance_with_this_province,
		tf_alliance_with_this_state,
		tf_alliance_with_this_pop,
		tf_has_recently_lost_war,
		tf_is_mobilised,
		tf_mobilisation_size,
		tf_crime_higher_than_education_nation,
		tf_crime_higher_than_education_state,
		tf_crime_higher_than_education_province,
		tf_crime_higher_than_education_pop,
		tf_agree_with_ruling_party,
		tf_is_colonial_state,
		tf_is_colonial_province,
		tf_has_factories,
		tf_in_default_tag,
		tf_in_default_from,
		tf_in_default_this_nation,
		tf_in_default_this_province,
		tf_in_default_this_state,
		tf_in_default_this_pop,
		tf_total_num_of_ports,
		tf_always,
		tf_election,
		tf_has_global_flag,
		tf_is_capital,
		tf_nationalvalue_nation,
		tf_industrial_score_value,
		tf_industrial_score_from_nation,
		tf_industrial_score_this_nation,
		tf_industrial_score_this_pop,
		tf_industrial_score_this_state,
		tf_industrial_score_this_province,
		tf_military_score_value,
		tf_military_score_from_nation,
		tf_military_score_this_nation,
		tf_military_score_this_pop,
		tf_military_score_this_state,
		tf_military_score_this_province,
		tf_civilized_nation,
		tf_civilized_pop,
		tf_civilized_province,
		tf_national_provinces_occupied,
		tf_is_greater_power_nation,
		tf_is_greater_power_pop,
		tf_rich_tax,
		tf_middle_tax,
		tf_poor_tax,
		tf_social_spending_nation,
		tf_social_spending_pop,
		tf_social_spending_province,
		tf_colonial_nation,
		tf_pop_majority_religion_nation,
		tf_pop_majority_religion_state,
		tf_pop_majority_religion_province,
		tf_pop_majority_culture_nation,
		tf_pop_majority_culture_state,
		tf_pop_majority_culture_province,
		tf_pop_majority_issue_nation,
		tf_pop_majority_issue_state,
		tf_pop_majority_issue_province,
		tf_pop_majority_issue_pop,
		tf_pop_majority_ideology_nation,
		tf_pop_majority_ideology_state,
		tf_pop_majority_ideology_province,
		tf_pop_majority_ideology_pop,
		tf_poor_strata_militancy_nation,
		tf_poor_strata_militancy_state,
		tf_poor_strata_militancy_province,
		tf_poor_strata_militancy_pop,
		tf_middle_strata_militancy_nation,
		tf_middle_strata_militancy_state,
		tf_middle_strata_militancy_province,
		tf_middle_strata_militancy_pop,
		tf_rich_strata_militancy_nation,
		tf_rich_strata_militancy_state,
		tf_rich_strata_militancy_province,
		tf_rich_strata_militancy_pop,
		tf_rich_tax_above_poor,
		tf_culture_has_union_tag_pop,
		tf_culture_has_union_tag_nation,
		tf_this_culture_union_tag,
		tf_this_culture_union_from,
		tf_this_culture_union_this_nation,
		tf_this_culture_union_this_province,
		tf_this_culture_union_this_state,
		tf_this_culture_union_this_pop,
		tf_this_culture_union_this_union_nation,
		tf_this_culture_union_this_union_province,
		tf_this_culture_union_this_union_state,
		tf_this_culture_union_this_union_pop,
		tf_minorities_nation,
		tf_minorities_state,
		tf_minorities_province,
		tf_revanchism_nation,
		tf_revanchism_pop,
		tf_has_crime,
		tf_num_of_substates,
		tf_num_of_vassals_no_substates,
		tf_brigades_compare_this,
		tf_brigades_compare_from,
		tf_constructing_cb_tag,
		tf_constructing_cb_from,
		tf_constructing_cb_this_nation,
		tf_constructing_cb_this_province,
		tf_constructing_cb_this_state,
		tf_constructing_cb_this_pop,
		tf_constructing_cb_discovered,
		tf_constructing_cb_progress,
		tf_civilization_progress,
		tf_constructing_cb_type,
		tf_is_our_vassal_tag,
		tf_is_our_vassal_from,
		tf_is_our_vassal_this_nation,
		tf_is_our_vassal_this_province,
		tf_is_our_vassal_this_state,
		tf_is_our_vassal_this_pop,
		tf_substate_of_tag,
		tf_substate_of_from,
		tf_substate_of_this_nation,
		tf_substate_of_this_province,
		tf_substate_of_this_state,
		tf_substate_of_this_pop,
		tf_is_substate,
		tf_great_wars_enabled,
		tf_can_nationalize,
		tf_part_of_sphere,
		tf_is_sphere_leader_of_tag,
		tf_is_sphere_leader_of_from,
		tf_is_sphere_leader_of_this_nation,
		tf_is_sphere_leader_of_this_province,
		tf_is_sphere_leader_of_this_state,
		tf_is_sphere_leader_of_this_pop,
		tf_number_of_states,
		tf_war_score,
		tf_is_releasable_vassal_from,
		tf_is_releasable_vassal_other,
		tf_has_recent_imigration,
		tf_province_control_days,
		tf_is_disarmed,
		tf_big_producer,
		tf_someone_can_form_union_tag_from,
		tf_someone_can_form_union_tag_other,
		tf_social_movement_strength,
		tf_political_movement_strength,
		tf_can_build_factory_in_capital_state,
		tf_social_movement,
		tf_political_movement,
		tf_has_cultural_sphere,
		tf_world_wars_enabled,
		tf_has_pop_culture_pop_this_pop,
		tf_has_pop_culture_state_this_pop,
		tf_has_pop_culture_province_this_pop,
		tf_has_pop_culture_nation_this_pop,
		tf_has_pop_culture_pop,
		tf_has_pop_culture_state,
		tf_has_pop_culture_province,
		tf_has_pop_culture_nation,
		tf_has_pop_religion_pop_this_pop,
		tf_has_pop_religion_state_this_pop,
		tf_has_pop_religion_province_this_pop,
		tf_has_pop_religion_nation_this_pop,
		tf_has_pop_religion_pop,
		tf_has_pop_religion_state,
		tf_has_pop_religion_province,
		tf_has_pop_religion_nation,
		tf_life_needs,
		tf_everyday_needs,
		tf_luxury_needs,
		tf_consciousness_pop,
		tf_consciousness_province,
		tf_consciousness_state,
		tf_consciousness_nation,
		tf_literacy_pop,
		tf_literacy_province,
		tf_literacy_state,
		tf_literacy_nation,
		tf_militancy_pop,
		tf_militancy_province,
		tf_militancy_state,
		tf_militancy_nation,
		tf_military_spending_pop,
		tf_military_spending_province,
		tf_military_spending_state,
		tf_military_spending_nation,
		tf_administration_spending_pop,
		tf_administration_spending_province,
		tf_administration_spending_state,
		tf_administration_spending_nation,
		tf_education_spending_pop,
		tf_education_spending_province,
		tf_education_spending_state,
		tf_education_spending_nation,
		tf_trade_goods_in_state_state,
		tf_trade_goods_in_state_province,
		tf_has_flashpoint,
		tf_flashpoint_tension,
		tf_crisis_exist,
		tf_is_liberation_crisis,
		tf_is_claim_crisis,
		tf_crisis_temperature,
		tf_involved_in_crisis_pop,
		tf_involved_in_crisis_nation,
		tf_rich_strata_life_needs_nation,
		tf_rich_strata_life_needs_state,
		tf_rich_strata_life_needs_province,
		tf_rich_strata_life_needs_pop,
		tf_rich_strata_everyday_needs_nation,
		tf_rich_strata_everyday_needs_state,
		tf_rich_strata_everyday_needs_province,
		tf_rich_strata_everyday_needs_pop,
		tf_rich_strata_luxury_needs_nation,
		tf_rich_strata_luxury_needs_state,
		tf_rich_strata_luxury_needs_province,
		tf_rich_strata_luxury_needs_pop,
		tf_middle_strata_life_needs_nation,
		tf_middle_strata_life_needs_state,
		tf_middle_strata_life_needs_province,
		tf_middle_strata_life_needs_pop,
		tf_middle_strata_everyday_needs_nation,
		tf_middle_strata_everyday_needs_state,
		tf_middle_strata_everyday_needs_province,
		tf_middle_strata_everyday_needs_pop,
		tf_middle_strata_luxury_needs_nation,
		tf_middle_strata_luxury_needs_state,
		tf_middle_strata_luxury_needs_province,
		tf_middle_strata_luxury_needs_pop,
		tf_poor_strata_life_needs_nation,
		tf_poor_strata_life_needs_state,
		tf_poor_strata_life_needs_province,
		tf_poor_strata_life_needs_pop,
		tf_poor_strata_everyday_needs_nation,
		tf_poor_strata_everyday_needs_state,
		tf_poor_strata_everyday_needs_province,
		tf_poor_strata_everyday_needs_pop,
		tf_poor_strata_luxury_needs_nation,
		tf_poor_strata_luxury_needs_state,
		tf_poor_strata_luxury_needs_province,
		tf_poor_strata_luxury_needs_pop,
		tf_diplomatic_influence_tag,
		tf_diplomatic_influence_this_nation,
		tf_diplomatic_influence_this_province,
		tf_diplomatic_influence_from_nation,
		tf_diplomatic_influence_from_province,
		tf_pop_unemployment_nation,
		tf_pop_unemployment_state,
		tf_pop_unemployment_province,
		tf_pop_unemployment_pop,
		tf_pop_unemployment_nation_this_pop,
		tf_pop_unemployment_state_this_pop,
		tf_pop_unemployment_province_this_pop,
		tf_relation_tag,
		tf_relation_this_nation,
		tf_relation_this_province,
		tf_relation_from_nation,
		tf_relation_from_province,
		tf_check_variable,
		tf_upper_house,
		tf_unemployment_by_type_nation,
		tf_unemployment_by_type_state,
		tf_unemployment_by_type_province,
		tf_unemployment_by_type_pop,
		tf_party_loyalty_nation_province_id,
		tf_party_loyalty_from_nation_province_id,
		tf_party_loyalty_province_province_id,
		tf_party_loyalty_from_province_province_id,
		tf_party_loyalty_nation_from_province,
		tf_party_loyalty_from_nation_scope_province,
		tf_can_build_in_province_railroad_no_limit_from_nation,
		tf_can_build_in_province_railroad_yes_limit_from_nation,
		tf_can_build_in_province_railroad_no_limit_this_nation,
		tf_can_build_in_province_railroad_yes_limit_this_nation,
		tf_can_build_in_province_fort_no_limit_from_nation,
		tf_can_build_in_province_fort_yes_limit_from_nation,
		tf_can_build_in_province_fort_no_limit_this_nation,
		tf_can_build_in_province_fort_yes_limit_this_nation,
		tf_can_build_in_province_naval_base_no_limit_from_nation,
		tf_can_build_in_province_naval_base_yes_limit_from_nation,
		tf_can_build_in_province_naval_base_no_limit_this_nation,
		tf_can_build_in_province_naval_base_yes_limit_this_nation,
		tf_can_build_railway_in_capital_yes_whole_state_yes_limit,
		tf_can_build_railway_in_capital_yes_whole_state_no_limit,
		tf_can_build_railway_in_capital_no_whole_state_yes_limit,
		tf_can_build_railway_in_capital_no_whole_state_no_limit,
		tf_can_build_fort_in_capital_yes_whole_state_yes_limit,
		tf_can_build_fort_in_capital_yes_whole_state_no_limit,
		tf_can_build_fort_in_capital_no_whole_state_yes_limit,
		tf_can_build_fort_in_capital_no_whole_state_no_limit,
		tf_work_available_nation,
		tf_work_available_state,
		tf_work_available_province,
		tf_variable_ideology_name_nation,
		tf_variable_ideology_name_state,
		tf_variable_ideology_name_province,
		tf_variable_ideology_name_pop,
		tf_variable_issue_name_nation,
		tf_variable_issue_name_state,
		tf_variable_issue_name_province,
		tf_variable_issue_name_pop,
		tf_variable_issue_group_name_nation,
		tf_variable_issue_group_name_state,
		tf_variable_issue_group_name_province,
		tf_variable_issue_group_name_pop,
		tf_variable_pop_type_name_nation,
		tf_variable_pop_type_name_state,
		tf_variable_pop_type_name_province,
		tf_variable_pop_type_name_pop,
		tf_variable_good_name,
		tf_strata_middle,
		tf_strata_poor,
		tf_party_loyalty_from_province_scope_province,
		tf_can_build_factory_nation,
		tf_can_build_factory_province,
		tf_nationalvalue_pop,
		tf_nationalvalue_province,
		tf_war_exhaustion_pop,
		tf_has_culture_core_province_this_pop,
		tf_tag_pop,
		tf_has_country_flag_pop,
		tf_has_country_flag_province,
		tf_has_country_modifier_province,
		tf_religion_nation,
		tf_religion_nation_reb,
		tf_religion_nation_from_nation,
		tf_religion_nation_this_nation,
		tf_religion_nation_this_state,
		tf_religion_nation_this_province,
		tf_religion_nation_this_pop,
		tf_war_exhaustion_province,
		tf_is_greater_power_province,
		tf_is_cultural_union_pop_this_pop,
		tf_has_building_factory,
		tf_has_building_state_from_province,
		tf_has_building_factory_from_province,
		tf_party_loyalty_generic,
	};

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
		bool show_condition
	) {
		if(show_condition) {
			if(triggers::test_trigger(tval, ws, primary_slot, this_slot, from_slot)) {
				ui::text_format local_fmt{ui::text_color::green, fmt.font_handle, fmt.font_size};
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
			} else {
				ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
			}
		}

		if((*tval & trigger_codes::is_scope) != 0) {
			return scope_functions[*tval & trigger_codes::code_mask](tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, show_condition);
		} else {
			return trigger_functions[*tval & trigger_codes::code_mask](tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot);
		}
	}
}
