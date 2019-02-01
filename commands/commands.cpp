#include "common\\common.h"
#include "commands.hpp"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.hpp"
#include "provinces\\province_functions.h"

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
		int8_t military_spending_v, int8_t tarrifs_v, int8_t army_stockpile_spending_v,
		int8_t navy_stockpile_spending_v, int8_t projects_stockpile_spending_v)  : nation_for(n),
		rich_tax(rich_tax_v), middle_tax(middle_tax_v), poor_tax(poor_tax_v), social_spending(social_spending_v),
		administrative_spending(administrative_spending_v), education_spending(education_spending_v),
		military_spending(military_spending_v), tarrifs(tarrifs_v),
		army_stockpile_spending(army_stockpile_spending_v), navy_stockpile_spending(navy_stockpile_spending_v),
		projects_stockpile_spending(projects_stockpile_spending_v)
	{}

	void execute_command(set_budget const& c, world_state& ws) {
		if(ws.w.nation_s.nations.is_valid_index(c.nation_for)) {
			auto& mod_values = ws.w.nation_s.modifier_values;

			switch(c.type) {
				default:
				case set_budget_type::all:
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_tax>(c.nation_for), 0.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for) > 0
						?  mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_rich_tax>(c.nation_for, std::clamp(float(c.rich_tax)/100.0f, min_v, max_v));
					ws.w.nation_s.nations.set<nation::f_middle_tax>(c.nation_for, std::clamp(float(c.middle_tax)/100.0f, min_v, max_v));
					ws.w.nation_s.nations.set<nation::f_poor_tax>(c.nation_for, std::clamp(float(c.poor_tax)/100.0f, min_v, max_v));
				}
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_social_spending>(c.nation_for), 0.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_social_spending>(c.nation_for) > 0
						? mod_values.get<modifiers::national_offsets::max_social_spending>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_social_spending>(c.nation_for, std::clamp(float(c.social_spending) / 100.0f, min_v, max_v));
				}
				ws.w.nation_s.nations.set<nation::f_administrative_spending>(c.nation_for, std::clamp(float(c.administrative_spending) / 100.0f, float(0), float(1)));
				ws.w.nation_s.nations.set<nation::f_education_spending>(c.nation_for, std::clamp(float(c.education_spending) / 100.0f, float(0), float(1)));
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_military_spending>(c.nation_for), -1.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_military_spending>(c.nation_for) > 0
						?  mod_values.get<modifiers::national_offsets::max_military_spending>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_military_spending>(c.nation_for, std::clamp(float(c.military_spending) / 100.0f, min_v, max_v));
				}
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_tariff>(c.nation_for), -1.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_tariff>(c.nation_for) > 0
						?  mod_values.get<modifiers::national_offsets::max_tariff>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_tariffs>(c.nation_for, std::clamp(float(c.tarrifs)/100.0f, min_v, max_v));
				}
				ws.w.nation_s.nations.set<nation::f_army_stockpile_spending>(c.nation_for, std::clamp(float(c.army_stockpile_spending)/100.0f, 0.0f, 1.0f));
				ws.w.nation_s.nations.set<nation::f_navy_stockpile_spending>(c.nation_for, std::clamp(float(c.navy_stockpile_spending)/100.0f, 0.0f, 1.0f));
				ws.w.nation_s.nations.set<nation::f_projects_stockpile_spending>(c.nation_for, std::clamp(float(c.projects_stockpile_spending)/100.0f, 0.0f, 1.0f));
					return;

				case set_budget_type::rich_tax:
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_tax>(c.nation_for), 0.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for) > 0
						?  mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_rich_tax>(c.nation_for, std::clamp(float(c.rich_tax)/100.0f, min_v, max_v));
				}
					return;
				case set_budget_type::middle_tax:
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_tax>(c.nation_for), 0.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for) > 0
						? mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_middle_tax>(c.nation_for, std::clamp(float(c.middle_tax)/100.0f, min_v, max_v));
				}
					return;
				case set_budget_type::poor_tax:
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_tax>(c.nation_for), 0.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for) > 0
						?  mod_values.get<modifiers::national_offsets::max_tax>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_poor_tax>(c.nation_for, std::clamp(float(c.poor_tax)/100.0f, min_v, max_v));
				}
					return;
				case set_budget_type::social_spending:
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_social_spending>(c.nation_for), 0.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_social_spending>(c.nation_for) > 0
						? mod_values.get<modifiers::national_offsets::max_social_spending>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_social_spending>(c.nation_for, std::clamp(float(c.social_spending) / 100.0f, min_v, max_v));
				}
					return;
				case set_budget_type::administrative_spending:
					ws.w.nation_s.nations.set<nation::f_administrative_spending>(c.nation_for, std::clamp(float(c.administrative_spending) / 100.0f, 0.0f, 1.0f));
					return;
				case set_budget_type::education_spending:
					ws.w.nation_s.nations.set<nation::f_education_spending>(c.nation_for, std::clamp(float(c.education_spending) / 100.0f, float(0), float(1)));
					return;
				case set_budget_type::military_spending:
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_military_spending>(c.nation_for), 0.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_military_spending>(c.nation_for) > 0
						?  mod_values.get<modifiers::national_offsets::max_military_spending>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_military_spending>(c.nation_for, std::clamp(float(c.military_spending) / 100.0f, min_v, max_v));
				}
					return;
				case set_budget_type::tarrifs:
				{
					float min_v = std::clamp(mod_values.get<modifiers::national_offsets::min_tariff>(c.nation_for), -1.0f, 1.0f);
					float max_v = std::clamp(
						mod_values.get<modifiers::national_offsets::max_tariff>(c.nation_for) > 0
						? mod_values.get<modifiers::national_offsets::max_tariff>(c.nation_for)
						: 1.0f, min_v, 1.0f);
					ws.w.nation_s.nations.set<nation::f_tariffs>(c.nation_for, std::clamp(float(c.tarrifs)/100.0f, min_v, max_v));
				}
					return;
				case set_budget_type::army_stockpile_spending:
					ws.w.nation_s.nations.set<nation::f_army_stockpile_spending>(c.nation_for, std::clamp(float(c.army_stockpile_spending)/100.0f, 0.0f, 1.0f));
					return;
				case set_budget_type::navy_stockpile_spending:
					ws.w.nation_s.nations.set<nation::f_navy_stockpile_spending>(c.nation_for, std::clamp(float(c.navy_stockpile_spending)/100.0f, 0.0f, 1.0f));
					return;
				case set_budget_type::projects_stockpile_spending:
					ws.w.nation_s.nations.set<nation::f_projects_stockpile_spending>(c.nation_for, std::clamp(float(c.projects_stockpile_spending)/100.0f, 0.0f, 1.0f));
					return;
			}
		}
	}

	void commands::execute_command(province_building const & c, world_state & ws) {
		if(is_command_valid(c, ws)) {
			switch(c.type) {
				case province_building_type::province_fort:
				{
					ws.w.province_s.province_state_container.set<province_state::fort_upgrade_progress>(c.p, 1.0f / float(ws.s.economy_m.fort.time));
					break;
				}
				case province_building_type::province_railroad:
				{
					ws.w.province_s.province_state_container.set<province_state::railroad_upgrade_progress>(c.p, 1.0f / float(ws.s.economy_m.railroad.time));
					break;
				}
				case province_building_type::province_naval_base:
				{
					ws.w.province_s.province_state_container.set<province_state::naval_base_upgrade_progress>(c.p, 1.0f / float(ws.s.economy_m.naval_base.time));
					break;
				}
				case province_building_type::state_railroad:
				{
					nations::for_each_province(ws, c.s, [n = c.nation_for, &ws](provinces::province_tag p) {
						province_building cmd(n, province_building_type::province_railroad, p);
						if(is_command_valid(cmd, ws))
							execute_command(cmd, ws);
					});
					break;
				}
				case province_building_type::state_fort:
				{
					nations::for_each_province(ws, c.s, [n = c.nation_for, &ws](provinces::province_tag p) {
						province_building cmd(n, province_building_type::province_fort, p);
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
		auto& container = ws.w.province_s.province_state_container;
		switch(c.type) {
			case province_building_type::province_fort:
			{
				if(auto owner = container.get<province_state::owner>(c.p); owner == c.nation_for) {
					return container.get<province_state::fort_upgrade_progress>(c.p) == 0
						&& container.get<province_state::fort_level>(c.p) < ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(c.nation_for);
				}
				return false;
			}
			case province_building_type::province_railroad:
			{
				if(auto owner = container.get<province_state::owner>(c.p); owner == c.nation_for) {
					return container.get<province_state::railroad_upgrade_progress>(c.p) == 0
						&& container.get<province_state::railroad_level>(c.p) < 
							(ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(c.nation_for) - ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(c.p))
						&& (ws.w.nation_s.nations.get<nation::current_rules>(owner) & issues::rules::build_railway) != 0;
				}
				return false;
			}
			case province_building_type::province_naval_base:
			{
				auto si = provinces::province_state(ws, c.p);
				if(auto owner = container.get<province_state::owner>(c.p); owner == c.nation_for && is_valid_index(si)) {
					auto port = nations::state_port_province(ws, si);
					return container.get<province_state::naval_base_upgrade_progress>(c.p) == 0
						&& container.get<province_state::naval_base_level>(c.p) < ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(c.nation_for)
						&& ws.s.province_m.province_container.get<province::is_coastal>(c.p)
						&& (port == c.p || !is_valid_index(port));
				}
				return false;
			}
			case province_building_type::state_railroad:
			{
				return ws.w.nation_s.states.get<state::owner>(c.s) == c.nation_for;
			}
			case province_building_type::state_fort:
			{
				return ws.w.nation_s.states.get<state::owner>(c.s) == c.nation_for;
			}
			default:
				return true;
		}
		return true;
	}

	ui::xy_pair explain_command_conditions(province_building const & c, world_state & ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager & lm, ui::text_format const & fmt) {
		auto& p_container = ws.w.province_s.province_state_container;

		switch(c.type) {
			case province_building_type::province_fort:
			{
				if(auto owner = p_container.get<province_state::owner>(c.p); is_valid_index(owner)) {
					if(p_container.get<province_state::fort_level>(c.p) < ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(owner)) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::level_less_than], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					char16_t local_buffer[16];
					put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(owner));
					cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				}
				return cursor_in;
			}
			case province_building_type::province_railroad:
			{
				if(auto owner = p_container.get<province_state::owner>(c.p); is_valid_index(owner)) {
					if(p_container.get<province_state::railroad_level>(c.p) <
						(ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_railroad>(owner) - ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(c.p))) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::rail_tech_prerequisite], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					char16_t local_buffer[16];
					put_value_in_buffer(local_buffer, display_type::integer, 1.0f + p_container.get<province_state::railroad_level>(c.p) + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(c.p));
					cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					
					if((ws.w.nation_s.nations.get<nation::current_rules>(owner) & issues::rules::build_railway) != 0) {
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
				auto si = p_container.get<province_state::state_instance>(c.p);
				if(auto owner = p_container.get<province_state::owner>(c.p); is_valid_index(owner) && is_valid_index(si)) {
					auto port = nations::state_port_province(ws, si);

					if(p_container.get<province_state::naval_base_level>(c.p) < ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(owner)) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), container, cursor_in, local_fmt, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), container, cursor_in, local_fmt, lm);
					}
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::level_less_than], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					char16_t local_buffer[16];
					put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(owner));
					cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

					if(ws.s.province_m.province_container.get<province::is_coastal>(c.p)) {
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
