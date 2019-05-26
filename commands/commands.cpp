#include "common\\common.h"
#include "commands.hpp"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.hpp"
#include "provinces\\province_functions.h"
#include "nations\nations_io.h"
#include "technologies\\technologies_functions.h"
#include "triggers\\trigger_gui.h"
#include "triggers\effects.h"
#include "governments\governments_functions.h"
#include "issues\issues_functions.h"
#include "economy\economy_functions.hpp"
#include "modifiers\modifier_functions.h"

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
						cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
					}
					cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::level_less_than], fmt, ws, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					cursor_in = ui::add_text(cursor_in, text_data::integer{ ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_fort>(owner) }, fmt, ws, container, lm);
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
						cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
					}
					cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::rail_tech_prerequisite], fmt, ws, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					cursor_in = ui::add_text(cursor_in, text_data::integer{
						1.0f + p_container.get<province_state::railroad_level>(c.p) + ws.w.province_s.modifier_values.get<modifiers::provincial_offsets::min_build_railroad>(c.p)
						}, fmt, ws, container, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					
					if((ws.w.nation_s.nations.get<nation::current_rules>(owner) & issues::rules::build_railway) != 0) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
					}
					cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::allowed_build_railroad], fmt, ws, container, lm);
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
						cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
					}
					cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::level_less_than], fmt, ws, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

					cursor_in = ui::add_text(cursor_in, text_data::integer{
						ws.w.nation_s.tech_attributes.get<technologies::tech_offset::max_naval_base>(owner)
						}, fmt, ws, container, lm);
					lm.finish_current_line();

					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

					if(ws.s.province_m.province_container.get<province::is_coastal>(c.p)) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
					}
					cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::is_coastal], fmt, ws, container, lm);
					lm.finish_current_line();
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

					if(port == c.p || !is_valid_index(port)) {
						ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
					} else {
						ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
						cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
					}
					cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_other_naval_base], fmt, ws, container, lm);
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

	void execute_command(fabricate_cb const& c, world_state& ws) {
		ws.w.nation_s.nations.set<nation::cb_construction_target>(c.nation_for, c.nation_target);
		ws.w.nation_s.nations.set<nation::cb_construction_progress>(c.nation_for, 0.0f);
		ws.w.nation_s.nations.set<nation::cb_construction_discovered>(c.nation_for, false);
		ws.w.nation_s.nations.set<nation::cb_construction_type>(c.nation_for, c.type);
	}

	bool is_command_valid(fabricate_cb const& c, world_state const& ws) {
		if(is_valid_index(c.nation_target))
			return !is_valid_index(ws.w.nation_s.nations.get<nation::cb_construction_target>(c.nation_for)) && c.nation_for != c.nation_target;
		else
			return true;
	}

	ui::xy_pair explain_command_conditions(fabricate_cb const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
		bool const not_fabricating = !is_valid_index(ws.w.nation_s.nations.get<nation::cb_construction_target>(c.nation_for));

		if(not_fabricating) {
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::fabrication_condition], fmt, ws, container, lm);
		lm.finish_current_line();
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

		return cursor_in;
	}

	bool is_command_valid(change_national_focus const& c, world_state const& ws) {
		if(c.new_focus) {
			auto max_focuses = modifiers::maximum_national_focuses(ws, ws.w.local_player_nation);
			auto used_focuses = modifiers::current_focus_count(ws, ws.w.local_player_nation);

			if(auto const lim = ws.s.modifiers_m.national_focuses[c.new_focus].limit; lim) {
				return
					ws.w.nation_s.states.get<state::owner>(c.in_state) == c.by_nation
					&& (used_focuses < max_focuses || is_valid_index(ws.w.nation_s.states.get<state::owner_national_focus>(c.in_state)))
					&& triggers::test_trigger(
						ws.s.trigger_m.trigger_data.data() + to_index(lim),
						ws,
						ws.w.nation_s.states.get<state::state_capital>(c.in_state),
						ws.w.nation_s.states.get<state::owner>(c.in_state),
						triggers::const_parameter());
			} else {
				return ws.w.nation_s.states.get<state::owner>(c.in_state) == c.by_nation
					&& (used_focuses < max_focuses || is_valid_index(ws.w.nation_s.states.get<state::owner_national_focus>(c.in_state)));
			}
		} else {
			return ws.w.nation_s.states.get<state::owner>(c.in_state) == c.by_nation;
		}
	}

	void execute_command(change_national_focus const& c, world_state& ws) {
		if(is_command_valid(c, ws)) {
			auto& nf = ws.w.nation_s.states.get<state::owner_national_focus>(c.in_state);

			if(nf && !(c.new_focus))
				remove_item(ws.w.nation_s.state_tag_arrays, ws.w.nation_s.nations.get<nation::national_focus_locations>(c.by_nation), c.in_state);
			else if(!nf && c.new_focus)
				add_item(ws.w.nation_s.state_tag_arrays, ws.w.nation_s.nations.get<nation::national_focus_locations>(c.by_nation), c.in_state);

			nf = c.new_focus;
		}
	}

	bool is_command_valid(build_factory const& c, world_state const& ws) {
		return
			bit_vector_test(ws.w.nation_s.active_goods.get_row(c.by_nation), ws.s.economy_m.factory_types[c.factory_type].output_good)
			&& economy::can_build_factory_in_state(ws, c.by_nation, c.in_state)
			&& economy::factory_type_valid_in_state(ws, c.in_state, c.factory_type);
	}

	void commands::execute_command(build_factory const & c, world_state & ws) {
		if(is_command_valid(c, ws)) {
			auto& factories = ws.w.nation_s.states.get<state::factories>(c.in_state);
			for(auto& f : factories) {
				if(!is_valid_index(f.type)) {
					f.type = &(ws.s.economy_m.factory_types[c.factory_type]);
					f.factory_progress = 0.00001f;
					f.flags = economy::factory_instance::owner_is_upgrading;

					if(auto const owner = nations::state_owner(ws, c.in_state); owner != c.by_nation) {
						auto const factory_cost = economy::total_factory_construction_cost(ws, c.in_state, c.factory_type);

						ws.w.nation_s.nations.get<nation::treasury>(owner) += factory_cost;
						ws.w.nation_s.nations.get<nation::treasury>(c.by_nation) -= factory_cost;
						nations::increase_foreign_investment(ws, c.by_nation, owner, factory_cost);
					}
					return;
				}
			}
		}
	}

	ui::xy_pair explain_command_conditions(build_factory const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
		bool unlocked = bit_vector_test(ws.w.nation_s.active_goods.get_row(c.by_nation), ws.s.economy_m.factory_types[c.factory_type].output_good);

		if(unlocked) {
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}
		cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::factory_unlocked, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		bool state_not_full = economy::count_factories_in_state(ws, c.in_state) < state::factories_count;
		if(state_not_full) {
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}

		text_data::text_replacement vrep(text_data::value_type::value, text_data::integer{ state::factories_count });
		cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::factory_limit, fmt, ws, container, lm, &vrep, 1);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		auto const state_owner = ws.w.nation_s.states.get<state::owner>(c.in_state);
		auto const owner_rules = ws.w.nation_s.nations.get<nation::current_rules>(state_owner);

		if(state_owner == c.by_nation) {
			bool rule_allowed = (issues::rules::build_factory & owner_rules) != 0;
			if(rule_allowed) {
				ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
			} else {
				ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
			}
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::factory_building_allowed, fmt, ws, container, lm);
		} else {
			bool rule_allowed = (issues::rules::allow_foreign_investment & owner_rules) != 0;
			if(rule_allowed) {
				ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
			} else {
				ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
			}
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::foreign_investment_allowed, fmt, ws, container, lm);
		}
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		bool not_in_state = 
			[&ws, s = c.in_state, f_type = c.factory_type]() {
			auto& factories = ws.w.nation_s.states.get<state::factories>(s);
			for(auto& f : factories) {
				if(f.type && f.type->id == f_type)
					return false;
			}
			return true;
		}();

		if(not_in_state) {
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}
		cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::factory_already_present, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		if(ws.s.economy_m.factory_types[c.factory_type].coastal) {
			if(nations::is_state_coastal(ws, c.in_state)) {
				ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
			} else {
				ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
			}
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::state_is_coastal, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);
		}


		if(!nations::is_colonial_or_protectorate(ws, c.in_state)) {
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}
		cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::not_colonial, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		return cursor_in;
	}

	void execute_command(change_research const& c, world_state& ws) {
		if(is_command_valid(c, ws)) {
			ws.w.nation_s.nations.set<nation::current_research>(c.nation_for, c.tech);
		}
	}

	void execute_command(execute_event const& c, world_state& ws) {
		if(auto const immediate = ws.s.event_m.event_container[c.e].immediate_effect; immediate) {
			triggers::execute_effect(ws.s.trigger_m.effect_data.data() + to_index(immediate), ws, c.target, c.target, c.from, c.generator);
		}
		if(auto const opt = ws.s.event_m.event_container[c.e].options[c.option].effect; opt) {
			triggers::execute_effect(ws.s.trigger_m.effect_data.data() + to_index(opt), ws, c.target, c.target, c.from, c.generator);
		}
	}

	void execute_command(change_influence_priority_level const& c, world_state& ws) {
		if(c.level >= 0 && c.level <= 3)
			nations::set_priority_level(ws, c.nation_for, c.nation_target, c.level);
	}

	bool is_command_valid(change_ruling_party const& c, world_state const& ws) {
		auto const last_change = ws.w.nation_s.nations.get<nation::last_manual_ruling_party_change>(c.nation_for);
		auto const gov = ws.w.nation_s.nations.get<nation::current_government>(c.nation_for);
		auto const party_ideology = ws.s.governments_m.parties[c.new_party].ideology;

		if(is_valid_index(last_change) && to_index(last_change) + 365 < to_index(ws.w.current_date))
			return false;
		else if(gov && ws.s.governments_m.governments_container[gov].appoint_ruling_party == false)
			return false;
		else if(c.new_party == ws.w.nation_s.nations.get<nation::ruling_party>(c.nation_for))
			return false;
		else if(gov && party_ideology && ws.s.governments_m.permitted_ideologies.get(gov, party_ideology) == 0)
			return false;
		else
			return true;
	}

	void execute_command(change_ruling_party const& c, world_state& ws) {
		if(is_command_valid(c, ws)) {
			governments::silent_set_ruling_party(ws, c.nation_for, c.new_party);
			ws.w.nation_s.nations.set<nation::last_manual_ruling_party_change>(c.nation_for, ws.w.current_date);
		}
	}

	void execute_command(change_sphere_leader const& c, world_state& ws) {
		nations::set_sphere_leader(ws, c.nation_for, c.new_leader);
	}

	void execute_command(set_reform const& c, world_state& ws) {
		if(is_command_valid(c, ws)) {
			issues::change_issue_option(ws, c.reform, c.nation_for, true);
		}
	}

	bool is_command_valid(change_research const& c, world_state const& ws) {
		return !is_valid_index(c.tech) || (ws.w.nation_s.active_technologies.get(c.nation_for, c.tech) == false &&
		                                   technologies::can_research(c.tech, ws, c.nation_for));
	}

	bool is_command_valid(set_reform const& c, world_state const& ws) {
		return issues::is_reform_possible(ws, c.nation_for, c.reform);
	}

	ui::xy_pair explain_command_conditions(set_reform const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
		if(!issues::is_reform_timer_ready(ws, c.nation_for)) {
			ui::text_format const local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_more_reform], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

			auto const last_date = ws.w.nation_s.nations.get<nation::last_reform_date>(c.nation_for);
			auto const next_date = date_tag(to_index(last_date) + int32_t(ws.s.modifiers_m.global_defines.min_delay_between_reforms) * 32);
			
			cursor_in = ui::add_text(cursor_in, next_date, fmt, ws, container, lm);
			lm.finish_current_line();
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		}
		auto const p_issue = ws.s.issues_m.options[c.reform].parent_issue;
		if(ws.s.issues_m.issues_container[p_issue].next_step_only) {
			if(is_reform_next_step(ws, c.nation_for, c.reform)) {
				ui::text_format const local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
			} else {
				ui::text_format const local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
			}
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reform_next_step], fmt, ws, container, lm);
			lm.finish_current_line();
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		}
		auto const opt_trigger = ws.s.issues_m.options[c.reform].allow;
		if(opt_trigger) {
			cursor_in = triggers::make_trigger_description(
				ws, container, cursor_in, lm, fmt,
				ws.s.trigger_m.trigger_data.data() + to_index(opt_trigger),
				c.nation_for, c.nation_for, c.nation_for, true
			);
		}

		cursor_in = ui::add_text(cursor_in, text_data::percent{
			std::clamp(issues::get_uh_reform_support(ws, c.nation_for, c.reform), 0.0f, 100.0f) * 0.01f
			}, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::upper_house_support], fmt,
			ws, container);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

		return cursor_in;
	}

	ui::xy_pair explain_command_conditions(change_research const& c, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
		if(!is_valid_index(c.tech))
			return cursor_in;

		auto& tech = ws.s.technology_m.technologies_container[c.tech];

		if(tech.year == 0 || tag_to_date(ws.w.current_date).year() >= tech.year) {
			ui::text_format const local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format const local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tech_condition_year], fmt, ws, container, lm);
		lm.finish_current_line();
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);


		if(ws.w.nation_s.active_technologies.get(c.nation_for, c.tech) == false) {
			ui::text_format const local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format const local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tech_condition_not_done], fmt, ws, container, lm);
		lm.finish_current_line();
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

		if(!is_valid_index(tech.preceeding) || ws.w.nation_s.active_technologies.get(c.nation_for, tech.preceeding) == true) {
			ui::text_format const local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
		} else {
			ui::text_format const local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
		}
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tech_condition_previous], fmt, ws, container, lm);
		lm.finish_current_line();
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);

		if(is_valid_index(tech.allow))
			cursor_in = triggers::make_trigger_description(
				ws,
				container,
				cursor_in,
				lm,
				fmt,
				ws.s.trigger_m.trigger_data.data() + to_index(tech.allow),
				c.nation_for,
				c.nation_for,
				triggers::const_parameter()
			);

		return cursor_in;
	}

}
