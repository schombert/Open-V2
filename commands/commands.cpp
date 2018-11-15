#include "common\\common.h"
#include "commands.hpp"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.hpp"

namespace commands {
	set_budget::set_budget(nations::country_tag n, set_budget_type t, int8_t v) : nation_for(n), type(t) {
		switch(t) {
			default:
			case set_budget_type::all:
				rich_tax = v;
				middle_tax = v;
				poor_tax = v;
				social_spending = v;
				administrative_spending = v;
				education_spending = v;
				military_spending = v;
				tarrifs = v;
				debt_setting = v;
				army_stockpile_spending = v;
				navy_stockpile_spending = v;
				projects_stockpile_spending = v;
				return;
			case set_budget_type::rich_tax:
				rich_tax = v;
				return;
			case set_budget_type::middle_tax:
				middle_tax = v;
				return;
			case set_budget_type::poor_tax:
				poor_tax = v;
				return;
			case set_budget_type::social_spending:
				social_spending = v;
				return;
			case set_budget_type::administrative_spending:
				administrative_spending = v;
				return;
			case set_budget_type::education_spending:
				education_spending = v;
				return;
			case set_budget_type::military_spending:
				military_spending = v;
				return;
			case set_budget_type::tarrifs:
				tarrifs = v;
				return;
			case set_budget_type::debt_setting:
				debt_setting = v;
				return;
			case set_budget_type::army_stockpile_spending:
				army_stockpile_spending = v;
				return;
			case set_budget_type::navy_stockpile_spending:
				navy_stockpile_spending = v;
				return;
			case set_budget_type::projects_stockpile_spending:
				projects_stockpile_spending = v;
				return;
		}
	}

	set_budget::set_budget(nations::country_tag n, int8_t rich_tax_v, int8_t middle_tax_v, int8_t poor_tax_v,
		int8_t social_spending_v, int8_t administrative_spending_v, int8_t education_spending_v,
		int8_t military_spending_v, int8_t tarrifs_v, int8_t debt_setting_v, int8_t army_stockpile_spending_v,
		int8_t navy_stockpile_spending_v, int8_t projects_stockpile_spending_v)  : nation_for(n),
		rich_tax(rich_tax_v), middle_tax(middle_tax_v), poor_tax(poor_tax_v), social_spending(social_spending_v),
		administrative_spending(administrative_spending_v), education_spending(education_spending_v),
		military_spending(military_spending_v), tarrifs(tarrifs_v), debt_setting(debt_setting_v),
		army_stockpile_spending(army_stockpile_spending_v), navy_stockpile_spending(navy_stockpile_spending_v),
		projects_stockpile_spending(projects_stockpile_spending_v)
	{}

	void execute_command(set_budget const& c, world_state& ws) {
		if(ws.w.nation_s.nations.is_valid_index(c.nation_for)) {
			auto& n = ws.w.nation_s.nations[c.nation_for];

			switch(c.type) {
				default:
				case set_budget_type::all:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.rich_tax = std::clamp(c.rich_tax, int8_t(min_v), int8_t(max_v));
					n.middle_tax = std::clamp(c.middle_tax, int8_t(min_v), int8_t(max_v));
					n.poor_tax = std::clamp(c.poor_tax, int8_t(min_v), int8_t(max_v));
				}
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_social_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_social_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_social_spending])
						: 100, min_v, 100);
					n.social_spending = std::clamp(c.social_spending, int8_t(min_v), int8_t(max_v));
				}
					n.administrative_spending = std::clamp(c.administrative_spending, int8_t(0), int8_t(100));
					n.education_spending = std::clamp(c.education_spending, int8_t(0), int8_t(100));
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_military_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_military_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_military_spending])
						: 100, min_v, 100);
					n.military_spending = std::clamp(c.military_spending, int8_t(min_v), int8_t(max_v));
				}
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tariff]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tariff] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tariff])
						: 100, min_v, 100);
					n.tarrifs = std::clamp(c.tarrifs, int8_t(min_v), int8_t(max_v));
				}
					n.debt_setting = std::clamp(c.debt_setting, int8_t(-100), int8_t(100));
					n.army_stockpile_spending = std::clamp(c.army_stockpile_spending, int8_t(0), int8_t(100));
					n.navy_stockpile_spending = std::clamp(c.navy_stockpile_spending, int8_t(0), int8_t(100));
					n.projects_stockpile_spending = std::clamp(c.projects_stockpile_spending, int8_t(0), int8_t(100));
					return;

				case set_budget_type::rich_tax:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.rich_tax = std::clamp(c.rich_tax, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::middle_tax:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.middle_tax = std::clamp(c.middle_tax, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::poor_tax:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tax]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tax] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tax])
						: 100, min_v, 100);
					n.poor_tax = std::clamp(c.poor_tax, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::social_spending:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_social_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_social_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_social_spending])
						: 100, min_v, 100);
					n.social_spending = std::clamp(c.social_spending, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::administrative_spending:
					n.administrative_spending = std::clamp(c.administrative_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::education_spending:
					n.education_spending = std::clamp(c.education_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::military_spending:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_military_spending]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_military_spending] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_military_spending])
						: 100, min_v, 100);
					n.military_spending = std::clamp(c.military_spending, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::tarrifs:
				{
					int32_t min_v = std::clamp(int32_t(100.0f * n.modifier_values[modifiers::national_offsets::min_tariff]), -100, 100);
					int32_t max_v = std::clamp(
						n.modifier_values[modifiers::national_offsets::max_tariff] > 0
						? int32_t(100.0f * n.modifier_values[modifiers::national_offsets::max_tariff])
						: 100, min_v, 100);
					n.tarrifs = std::clamp(c.tarrifs, int8_t(min_v), int8_t(max_v));
				}
					return;
				case set_budget_type::debt_setting:
					n.debt_setting = std::clamp(c.debt_setting, int8_t(-100), int8_t(100));
					return;
				case set_budget_type::army_stockpile_spending:
					n.army_stockpile_spending = std::clamp(c.army_stockpile_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::navy_stockpile_spending:
					n.navy_stockpile_spending = std::clamp(c.navy_stockpile_spending, int8_t(0), int8_t(100));
					return;
				case set_budget_type::projects_stockpile_spending:
					n.projects_stockpile_spending = std::clamp(c.projects_stockpile_spending, int8_t(0), int8_t(100));
					return;
			}
		}
	}

	void commands::execute_command(province_building const & c, world_state & ws) {
		if(is_command_valid(c, ws)) {
			switch(c.type) {
				case province_building_type::province_fort:
				{
					auto& ps = ws.w.province_s.province_state_container[c.p];
					ps.fort_upgrade_progress = 1.0f / float(ws.s.economy_m.fort.time);
					break;
				}
				case province_building_type::province_railroad:
				{
					auto& ps = ws.w.province_s.province_state_container[c.p];
					ps.railroad_upgrade_progress = 1.0f / float(ws.s.economy_m.railroad.time);
					break;
				}
				case province_building_type::province_naval_base:
				{
					auto& ps = ws.w.province_s.province_state_container[c.p];
					ps.naval_base_upgrade_progress = 1.0f / float(ws.s.economy_m.naval_base.time);
					break;
				}
				case province_building_type::state_railroad:
				{
					auto& si = ws.w.nation_s.states[c.s];
					nations::for_each_province(ws, si, [n = c.nation_for, &ws](provinces::province_state& ps) {
						province_building cmd(n, province_building_type::province_railroad, ps.id);
						if(is_command_valid(cmd, ws))
							execute_command(cmd, ws);
					});
					break;
				}
				case province_building_type::state_fort:
				{
					auto& si = ws.w.nation_s.states[c.s];
					nations::for_each_province(ws, si, [n = c.nation_for, &ws](provinces::province_state& ps) {
						province_building cmd(n, province_building_type::province_fort, ps.id);
						if(is_command_valid(cmd, ws))
							execute_command(cmd, ws);
					});
					break;
				}
				default:
					break;
			}
		}
	}

	bool is_command_valid(province_building const & c, world_state const & ws) {

		switch(c.type) {
			case province_building_type::province_fort:
			{
				auto& ps = ws.w.province_s.province_state_container[c.p];
				if(auto owner = ps.owner; owner && owner->id == c.nation_for) {
					return ps.fort_upgrade_progress == 0
						&& ps.fort_level < owner->tech_attributes[technologies::tech_offset::max_fort];
				}
				return false;
			}
			case province_building_type::province_railroad:
			{
				auto& ps = ws.w.province_s.province_state_container[c.p];
				if(auto owner = ps.owner; owner && owner->id == c.nation_for) {
					return ps.railroad_upgrade_progress == 0
						&& ps.railroad_level < owner->tech_attributes[technologies::tech_offset::max_railroad] - ps.modifier_values[modifiers::provincial_offsets::min_build_railroad]
						&& (owner->current_rules.rules_value & issues::rules::build_railway) != 0;
				}
				return false;
			}
			case province_building_type::province_naval_base:
			{
				auto& ps = ws.w.province_s.province_state_container[c.p];
				auto si = ps.state_instance;
				if(auto owner = ps.owner; owner && si && owner->id == c.nation_for) {
					auto port = nations::state_port_province(ws, *si);
					return ps.naval_base_upgrade_progress == 0
						&& ps.naval_base_level < owner->tech_attributes[technologies::tech_offset::max_naval_base]
						&& (ws.s.province_m.province_container[c.p].flags & provinces::province::coastal) != 0
						&& (port == c.p || !is_valid_index(port));
				}
				return false;
			}
			case province_building_type::state_railroad:
			{
				auto& si = ws.w.nation_s.states[c.s];
				return si.owner && si.owner->id == c.nation_for;
			}
			case province_building_type::state_fort:
			{
				auto& si = ws.w.nation_s.states[c.s];
				return si.owner && si.owner->id == c.nation_for;
			}
			default:
				return true;
		}
		return true;
	}

	ui::xy_pair explain_command_conditions(province_building const & c, world_state & ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager & lm, ui::text_format const & fmt) {
		switch(c.type) {
			case province_building_type::province_fort:
			{
				auto& ps = ws.w.province_s.province_state_container[c.p];
				if(auto owner = ps.owner; owner) {
					if(ps.fort_level < owner->tech_attributes[technologies::tech_offset::max_fort]) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::level_less_than], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					char16_t local_buffer[16];
					put_value_in_buffer(local_buffer, display_type::integer, owner->tech_attributes[technologies::tech_offset::max_fort]);
					cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				}
				return cursor_in;
			}
			case province_building_type::province_railroad:
			{
				auto& ps = ws.w.province_s.province_state_container[c.p];
				if(auto owner = ps.owner; owner) {

					if(ps.railroad_level < owner->tech_attributes[technologies::tech_offset::max_railroad] - ps.modifier_values[modifiers::provincial_offsets::min_build_railroad]) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::rail_tech_prerequisite], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					char16_t local_buffer[16];
					put_value_in_buffer(local_buffer, display_type::integer, 1.0f + ps.railroad_level + ps.modifier_values[modifiers::provincial_offsets::min_build_railroad]);
					cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					
					if((owner->current_rules.rules_value & issues::rules::build_railway) != 0) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::allowed_build_railroad], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					lm.finish_current_line();
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

				}
				return cursor_in;
			}
			case province_building_type::province_naval_base:
			{
				auto& ps = ws.w.province_s.province_state_container[c.p];
				auto si = ps.state_instance;
				if(auto owner = ps.owner; owner && si) {
					auto port = nations::state_port_province(ws, *si);

					if(ps.naval_base_level < owner->tech_attributes[technologies::tech_offset::max_naval_base]) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::level_less_than], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					char16_t local_buffer[16];
					put_value_in_buffer(local_buffer, display_type::integer, owner->tech_attributes[technologies::tech_offset::max_naval_base]);
					cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

					if((ws.s.province_m.province_container[c.p].flags & provinces::province::coastal) != 0) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::is_coastal], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					lm.finish_current_line();
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

					if(port == c.p || !is_valid_index(port)) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_other_naval_base], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					lm.finish_current_line();
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				}
				return cursor_in;
			}
			default:
				return cursor_in;
		}
		return cursor_in;
	}
}
