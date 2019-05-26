#include "common\\common.h"
#include "modifiers_gui.hpp"
#include "world_state\\world_state.h"
#include "nations\\nations_functions.h"
#include "modifier_functions.h"
#include "triggers\\trigger_gui.h"

namespace modifiers {
	
	struct modifier_display_details {
		bool positive_is_green = true;
		display_type display_as = display_type::percent;
	};

	
	static modifier_display_details province_offset_display[] = {
		modifier_display_details{ true, display_type::integer }, // supply_limit 0
		modifier_display_details{ false, display_type::percent }, // attrition 1
		modifier_display_details{ false, display_type::integer }, // max_attrition 2
		modifier_display_details{ true, display_type::percent }, // local_ruling_party_support 3
		modifier_display_details{ false, display_type::percent }, // poor_life_needs 4
		modifier_display_details{ false, display_type::percent }, // rich_life_needs 5
		modifier_display_details{ false, display_type::percent }, // middle_life_needs 6
		modifier_display_details{ false, display_type::percent }, // poor_everyday_needs 7
		modifier_display_details{ false, display_type::percent }, // rich_everyday_needs 8
		modifier_display_details{ false, display_type::percent }, // middle_everyday_needs 9
		modifier_display_details{ false, display_type::percent }, // poor_luxury_needs 10
		modifier_display_details{ false, display_type::percent }, // middle_luxury_needs 11
		modifier_display_details{ false, display_type::percent }, // rich_luxury_needs 12
		modifier_display_details{ true, display_type::fp_three_places }, // population_growth 13
		modifier_display_details{ false, display_type::percent }, // local_factory_input 14
		modifier_display_details{ true, display_type::percent }, // local_factory_output 15
		modifier_display_details{ true, display_type::percent }, // local_factory_throughput 16
		modifier_display_details{ true, display_type::percent }, // local_rgo_input 17
		modifier_display_details{ true, display_type::percent }, // local_rgo_output 18
		modifier_display_details{ true, display_type::percent }, // local_rgo_throughput 19
		modifier_display_details{ false, display_type::percent }, // local_artisan_input 20
		modifier_display_details{ true, display_type::percent }, // local_artisan_output 21
		modifier_display_details{ true, display_type::percent }, // local_artisan_throughput 22
		modifier_display_details{ true, display_type::percent }, // number_of_voters 23
		modifier_display_details{ true, display_type::percent }, // goods_demand 24
		modifier_display_details{ true, display_type::percent }, // assimilation_rate 25
		modifier_display_details{ true, display_type::percent }, // life_rating 26
		modifier_display_details{ true, display_type::percent }, // farm_rgo_eff 27
		modifier_display_details{ true, display_type::percent }, // mine_rgo_eff 28
		modifier_display_details{ true, display_type::percent }, // farm_rgo_size 29
		modifier_display_details{ true, display_type::percent }, // mine_rgo_size 30
		modifier_display_details{ false, display_type::fp_three_places }, // pop_militancy_modifier 31
		modifier_display_details{ false, display_type::fp_three_places }, // pop_consciousness_modifier 32
		modifier_display_details{ true, display_type::percent }, // rich_income_modifier 33
		modifier_display_details{ true, display_type::percent }, // middle_income_modifier 34
		modifier_display_details{ true, display_type::percent }, // poor_income_modifier 35
		modifier_display_details{ true, display_type::percent }, // boost_strongest_party 36
		modifier_display_details{ true, display_type::percent }, // immigrant_attract 37
		modifier_display_details{ true, display_type::percent }, // immigrant_push 38
		modifier_display_details{ true, display_type::percent }, // local_repair 39
		modifier_display_details{ false, display_type::percent }, // local_ship_build 40
		modifier_display_details{ false, display_type::percent }, // movement_cost 41
		modifier_display_details{ true, display_type::integer }, // defense 42
		modifier_display_details{ true, display_type::integer }, // attack 43
		modifier_display_details{ false, display_type::percent }, // combat_width 44
		modifier_display_details{ false, display_type::integer }, // min_build_naval_base 45
		modifier_display_details{ false, display_type::integer }, // min_build_railroad 46
		modifier_display_details{ false, display_type::integer }, // min_build_fort 47
	};

	static modifier_display_details national_offset_display[] = {
		modifier_display_details{ false, display_type::fp_two_places }, // war_exhaustion 0
		modifier_display_details{ true, display_type::integer }, // max_war_exhaustion 1
		modifier_display_details{ true, display_type::integer }, // leadership 2
		modifier_display_details{ true, display_type::percent }, // leadership_modifier 3
		modifier_display_details{ false, display_type::percent }, // supply_consumption 4
		modifier_display_details{ true, display_type::percent }, // org_regain 5
		modifier_display_details{ true, display_type::percent }, // reinforce_speed 6
		modifier_display_details{ true, display_type::percent }, // land_organisation 7
		modifier_display_details{ true, display_type::percent }, // naval_organisation 8
		modifier_display_details{ true, display_type::fp_two_places }, // research_points 9
		modifier_display_details{ true, display_type::percent }, // research_points_modifier 10
		modifier_display_details{ true, display_type::percent }, // research_points_on_conquer 11
		modifier_display_details{ false, display_type::percent }, // import_cost 12
		modifier_display_details{ false, display_type::percent }, // loan_interest 13
		modifier_display_details{ true, display_type::percent }, // tax_efficiency 14
		modifier_display_details{ true, display_type::percent }, // min_tax 15
		modifier_display_details{ true, display_type::percent }, // max_tax 16
		modifier_display_details{ true, display_type::percent }, // min_military_spending 17
		modifier_display_details{ true, display_type::percent }, // max_military_spending 18
		modifier_display_details{ true, display_type::percent }, // min_social_spending 19
		modifier_display_details{ true, display_type::percent }, // max_social_spending 20
		modifier_display_details{ false, display_type::percent }, // factory_owner_cost 21
		modifier_display_details{ true, display_type::percent }, // min_tariff 22
		modifier_display_details{ true, display_type::percent }, // max_tariff 23
		modifier_display_details{ true, display_type::percent }, // ruling_party_support 24
		modifier_display_details{ true, display_type::percent }, // rich_vote 25
		modifier_display_details{ true, display_type::percent }, // middle_vote 26
		modifier_display_details{ true, display_type::percent }, // poor_vote 27
		modifier_display_details{ true, display_type::percent }, // minimum_wage 28
		modifier_display_details{ false, display_type::percent }, // factory_maintenance 29
		modifier_display_details{ false, display_type::percent }, // poor_life_needs 30
		modifier_display_details{ false, display_type::percent }, // rich_life_needs 31
		modifier_display_details{ false, display_type::percent }, // middle_life_needs 32
		modifier_display_details{ false, display_type::percent }, // poor_everyday_needs 33
		modifier_display_details{ false, display_type::percent }, // rich_everyday_needs 34
		modifier_display_details{ false, display_type::percent }, // middle_everyday_needs 35
		modifier_display_details{ false, display_type::percent }, // poor_luxury_needs 36
		modifier_display_details{ false, display_type::percent }, // middle_luxury_needs 37
		modifier_display_details{ false, display_type::percent }, // rich_luxury_needs 38
		modifier_display_details{ true, display_type::percent }, // unemployment_benefit 39
		modifier_display_details{ true, display_type::percent }, // pension_level 40
		modifier_display_details{ true, display_type::fp_two_places }, // global_population_growth 41
		modifier_display_details{ false, display_type::percent }, // factory_input 42
		modifier_display_details{ true, display_type::percent }, // factory_output 43
		modifier_display_details{ true, display_type::percent }, // factory_throughput 44
		modifier_display_details{ true, display_type::percent }, // rgo_input 45
		modifier_display_details{ true, display_type::percent }, // rgo_output 46
		modifier_display_details{ true, display_type::percent }, // rgo_throughput 47
		modifier_display_details{ false, display_type::percent }, // artisan_input 48
		modifier_display_details{ true, display_type::percent }, // artisan_output 49
		modifier_display_details{ true, display_type::percent }, // artisan_throughput 50
		modifier_display_details{ true, display_type::percent }, // goods_demand 51
		modifier_display_details{ false, display_type::fp_two_places }, // badboy 52
		modifier_display_details{ true, display_type::percent }, // global_assimilation_rate 53
		modifier_display_details{ true, display_type::percent }, // prestige 54
		modifier_display_details{ false, display_type::percent }, // factory_cost 55
		modifier_display_details{ true, display_type::percent }, // farm_rgo_eff 56
		modifier_display_details{ true, display_type::percent }, // mine_rgo_eff 57
		modifier_display_details{ true, display_type::percent }, // farm_rgo_size 58
		modifier_display_details{ true, display_type::percent }, // mine_rgo_size 59
		modifier_display_details{ true, display_type::percent }, // issue_change_speed 60
		modifier_display_details{ false, display_type::fp_three_places }, // social_reform_desire 61
		modifier_display_details{ false, display_type::fp_three_places }, // political_reform_desire 62
		modifier_display_details{ true, display_type::percent }, // literacy_con_impact 63
		modifier_display_details{ true, display_type::percent }, // rich_income_modifier 64
		modifier_display_details{ true, display_type::percent }, // middle_income_modifier 65
		modifier_display_details{ true, display_type::percent }, // poor_income_modifier 66
		modifier_display_details{ true, display_type::percent }, // global_immigrant_attract 67
		modifier_display_details{ true, display_type::percent }, // poor_savings_modifier 68
		modifier_display_details{ true, display_type::percent }, // influence_modifier 69
		modifier_display_details{ true, display_type::fp_two_places }, // diplomatic_points_modifier 70
		modifier_display_details{ true, display_type::percent }, // mobilisation_size 71
		modifier_display_details{ false, display_type::percent }, // mobilisation_economy_impact 72
		modifier_display_details{ false, display_type::fp_three_places }, // global_pop_militancy_modifier 73
		modifier_display_details{ false, display_type::fp_three_places }, // global_pop_consciousness_modifier 74
		modifier_display_details{ false, display_type::fp_three_places }, // core_pop_militancy_modifier 75
		modifier_display_details{ false, display_type::fp_three_places }, // core_pop_consciousness_modifier 76
		modifier_display_details{ false, display_type::fp_three_places }, // non_accepted_pop_militancy_modifier 77
		modifier_display_details{ false, display_type::fp_three_places }, // non_accepted_pop_consciousness_modifier 78
		modifier_display_details{ true, display_type::percent }, // cb_generation_speed_modifier 79
		modifier_display_details{ false, display_type::percent }, // mobilization_impact 80
		modifier_display_details{ true, display_type::percent }, // suppression_points_modifier 81
		modifier_display_details{ true, display_type::percent }, // education_efficiency_modifier 82
		modifier_display_details{ true, display_type::percent }, // civilization_progress_modifier 83
		modifier_display_details{ true, display_type::percent }, // administrative_efficiency_modifier 84
		modifier_display_details{ true, display_type::percent }, // land_unit_start_experience 85
		modifier_display_details{ true, display_type::percent }, // naval_unit_start_experience 86
		modifier_display_details{ true, display_type::percent }, // naval_attack_modifier 87
		modifier_display_details{ true, display_type::percent }, // naval_defense_modifier 88
		modifier_display_details{ true, display_type::percent }, // land_attack_modifier 89
		modifier_display_details{ true, display_type::percent }, // land_defense_modifier 90
		modifier_display_details{ true, display_type::percent }, // tariff_efficiency_modifier 91
		modifier_display_details{ true, display_type::percent }, // max_loan_modifier 92
		modifier_display_details{ true, display_type::percent }, // unciv_economic_modifier 93
		modifier_display_details{ true, display_type::percent }, // unciv_military_modifier 94
		modifier_display_details{ true, display_type::percent }, // self_unciv_economic_modifier 95
		modifier_display_details{ true, display_type::percent }, // self_unciv_military_modifier 96
		modifier_display_details{ true, display_type::percent }, // commerce_tech_research_bonus 97
		modifier_display_details{ true, display_type::percent }, // army_tech_research_bonus 98
		modifier_display_details{ true, display_type::percent }, // industry_tech_research_bonus 99
		modifier_display_details{ true, display_type::percent }, // navy_tech_research_bonus 100
		modifier_display_details{ true, display_type::percent }, // culture_tech_research_bonus 101
	};

	
	
	ui::xy_pair make_province_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, modifier_definition const& def) {
		char16_t local_buf[64];
		bool has_effect = false;
		for(uint32_t i = 0; i < modifier_definition_size; ++i) {
			if(def.values[i] != value_type(0)) {
				cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.province_offset_names[def.offsets[i]], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

				if(def.values[i] < value_type(0)) {
					local_buf[0] = u'-';
					put_pos_value_in_buffer(local_buf + 1, province_offset_display[def.offsets[i]].display_as, -def.values[i]);
					cursor_in = ui::add_text(cursor_in, local_buf, 
						ui::text_format{ province_offset_display[def.offsets[i]].positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size },
						ws, container, lm);
					
				} else {
					local_buf[0] = u'+';
					put_pos_value_in_buffer(local_buf + 1, province_offset_display[def.offsets[i]].display_as, def.values[i]);
					cursor_in = ui::add_text(cursor_in, local_buf,
						ui::text_format{ province_offset_display[def.offsets[i]].positive_is_green ? ui::text_color::green : ui::text_color::red, fmt.font_handle, fmt.font_size },
						ws, container, lm);
				}
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				has_effect = true;
			}
		}
		if(has_effect == false) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		return cursor_in;
	}

	ui::xy_pair make_national_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, modifier_definition const& def) {
		char16_t local_buf[64];
		bool has_effect = false;
		for(uint32_t i = 0; i < modifier_definition_size; ++i) {
			if(def.values[i] != value_type(0)) {
				cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.national_offset_names[def.offsets[i]], fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

				if(def.values[i] < value_type(0)) {
					local_buf[0] = u'-';
					put_pos_value_in_buffer(local_buf + 1, national_offset_display[def.offsets[i]].display_as, -def.values[i]);
					cursor_in = ui::add_text(cursor_in, local_buf,
						ui::text_format{ national_offset_display[def.offsets[i]].positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size },
						ws, container, lm);
				} else {
					local_buf[0] = u'+';
					put_pos_value_in_buffer(local_buf + 1, national_offset_display[def.offsets[i]].display_as, def.values[i]);
					cursor_in = ui::add_text(cursor_in, local_buf,
						ui::text_format{ national_offset_display[def.offsets[i]].positive_is_green ? ui::text_color::green : ui::text_color::red, fmt.font_handle, fmt.font_size },
						ws, container, lm);
				}
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				has_effect = true;
			}
		}
		if(has_effect == false) {
			cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		return cursor_in;
	}
	ui::xy_pair make_province_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provincial_modifier_tag t) {
		auto name = ws.s.modifiers_m.provincial_modifiers[t].name;
		if(is_valid_index(name)) {
			cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		lm.increase_indent(1);
		cursor_in = make_province_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.provincial_modifier_definitions[t]);
		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair make_national_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, national_modifier_tag t) {
		auto name = ws.s.modifiers_m.national_modifiers[t].name;
		if(is_valid_index(name)) {
			cursor_in = ui::add_text(cursor_in, name, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		lm.increase_indent(1);
		cursor_in = make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[t]);
		lm.decrease_indent(1);
		return cursor_in;
	}

	inline ui::xy_pair display_single_provincial_modifier_value(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provincial_modifier_tag mod, uint32_t offset, value_type multiplier) {
		char16_t local_buf[64];

		if(!is_valid_index(mod) || multiplier == value_type(0))
			return cursor_in;

		auto value = [&ws, mod, offset]() {
			for(uint32_t i = 0; i < modifier_definition_size; ++i) {
				if(ws.s.modifiers_m.provincial_modifier_definitions[mod].offsets[i] == int32_t(offset))
					return ws.s.modifiers_m.provincial_modifier_definitions[mod].values[i];
			}
			return 0.0f;
		}();

		if(value == value_type(0))
			return cursor_in;

		if(value * multiplier < value_type(0)) {
			local_buf[0] = u'-';
			put_pos_value_in_buffer(local_buf + 1, province_offset_display[offset].display_as, -value * multiplier);
			cursor_in = ui::add_text(cursor_in, local_buf,
				ui::text_format{ province_offset_display[offset].positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size },
				ws, container, lm);
		} else {
			local_buf[0] = u'+';
			put_pos_value_in_buffer(local_buf + 1, province_offset_display[offset].display_as, value * multiplier);
			cursor_in = ui::add_text(cursor_in, local_buf,
				ui::text_format{ province_offset_display[offset].positive_is_green ? ui::text_color::green : ui::text_color::red, fmt.font_handle, fmt.font_size },
				ws, container, lm);
		}

		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		if(is_valid_index(ws.s.modifiers_m.provincial_modifiers[mod].name))
			cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.provincial_modifiers[mod].name, fmt, ws, container, lm);
		
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		return cursor_in;
	}

	inline ui::xy_pair display_single_national_modifier_value(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, national_modifier_tag mod, uint32_t offset, value_type multiplier) {
		char16_t local_buf[64];

		if(!is_valid_index(mod) || multiplier == value_type(0))
			return cursor_in;

		auto value = [&ws, mod, offset]() {
			for(uint32_t i = 0; i < modifier_definition_size; ++i) {
				if(ws.s.modifiers_m.national_modifier_definitions[mod].offsets[i] == int32_t(offset))
					return ws.s.modifiers_m.national_modifier_definitions[mod].values[i];
			}
			return 0.0f;
		}();

		if(value == value_type(0))
			return cursor_in;

		if(value * multiplier < value_type(0)) {
			local_buf[0] = u'-';
			put_pos_value_in_buffer(local_buf + 1, national_offset_display[offset].display_as, -value * multiplier);
			cursor_in = ui::add_text(cursor_in, local_buf,
				ui::text_format{ national_offset_display[offset].positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size },
				ws, container, lm);
		} else {
			local_buf[0] = u'+';
			put_pos_value_in_buffer(local_buf + 1, national_offset_display[offset].display_as, value * multiplier);
			cursor_in = ui::add_text(cursor_in, local_buf,
				ui::text_format{ national_offset_display[offset].positive_is_green ? ui::text_color::green : ui::text_color::red, fmt.font_handle, fmt.font_size },
				ws, container, lm);
		}

		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		if(is_valid_index(ws.s.modifiers_m.national_modifiers[mod].name))
			cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.national_modifiers[mod].name, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		return cursor_in;
	}

	ui::xy_pair explain_province_modifier(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provinces::province_tag this_province, uint32_t modifier_offset) {
		auto& pcontainer = ws.w.province_s.province_state_container;
		auto& pscontainer = ws.s.province_m.province_container;
		
		auto static_range = get_range(ws.w.province_s.static_modifier_arrays, pcontainer.get<province_state::static_modifiers>(this_province));
		for(auto m : static_range)
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, m, modifier_offset, value_type(1));

		auto timed_range = get_range(ws.w.province_s.timed_modifier_arrays, pcontainer.get<province_state::timed_modifiers>(this_province));
		for(auto t = timed_range.first; t != timed_range.second; ++t)
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, t->mod, modifier_offset, value_type(1));

		cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, pcontainer.get<province_state::terrain>(this_province), modifier_offset, value_type(1));
		cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, pscontainer.get<province::climate>(this_province), modifier_offset, value_type(1));
		cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, pscontainer.get<province::continent>(this_province), modifier_offset, value_type(1));
		cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, pcontainer.get<province_state::crime>(this_province), modifier_offset, value_type(1));

		if(auto si = pcontainer.get<province_state::state_instance>(this_province); is_valid_index(si)) {
			if(auto nf = ws.w.nation_s.states.get<state::owner_national_focus>(si); is_valid_index(nf))
				cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_focuses[nf].modifier, modifier_offset, value_type(1));
		}
		cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.infrastructure, modifier_offset, ws.s.economy_m.railroad.infrastructure * float(pcontainer.get<province_state::railroad_level>(this_province)));
		
		if(is_valid_index(ws.s.economy_m.fort_modifier))
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.economy_m.fort_modifier, modifier_offset, float(pcontainer.get<province_state::fort_level>(this_province)));
		if(is_valid_index(ws.s.economy_m.naval_base_modifier))
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.economy_m.naval_base_modifier, modifier_offset, float(pcontainer.get<province_state::naval_base_level>(this_province)));
		if(is_valid_index(ws.s.economy_m.railroad_modifier))
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.economy_m.railroad_modifier, modifier_offset, float(pcontainer.get<province_state::railroad_level>(this_province)));

		if(pcontainer.get<province_state::siege_progress>(this_province) != 0.0f)
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.has_siege, modifier_offset, value_type(1));
		if(pcontainer.get<province_state::is_overseas>(this_province))
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.overseas, modifier_offset, value_type(1));
		if(pcontainer.get<province_state::is_blockaded>(this_province))
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.blockaded, modifier_offset, value_type(1));
		if(pcontainer.get<province_state::has_owner_core>(this_province))
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.core, modifier_offset, value_type(1));
		if(pscontainer.get<province::is_sea>(this_province)) {
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.sea_zone, modifier_offset, value_type(1));
			if(pscontainer.get<province::is_coastal>(this_province))
				cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.coastal_sea, modifier_offset, value_type(1));
			else
				cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.non_coastal, modifier_offset, value_type(1));
		} else {
			cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.land_province, modifier_offset, value_type(1));
			if(pscontainer.get<province::is_coastal>(this_province))
				cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.coastal, modifier_offset, value_type(1));
			else
				cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.non_coastal, modifier_offset, value_type(1));
		}

		cursor_in = display_single_provincial_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.nationalism, modifier_offset, pcontainer.get<province_state::nationalism>(this_province));
		
		return cursor_in;
	}

	ui::xy_pair explain_national_modifier(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, nations::country_tag this_nation, uint32_t modifier_offset) {
		auto nation_id = this_nation;
		if(!ws.w.nation_s.nations.is_valid_index(nation_id))
			return cursor_in;

		auto static_range = get_range(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(this_nation));
		for(auto m : static_range)
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, m, modifier_offset, value_type(1));

		auto timed_range = get_range(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(this_nation));
		for(auto t = timed_range.first; t != timed_range.second; ++t)
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, t->mod, modifier_offset, value_type(1));
		
		cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.w.nation_s.nations.get<nation::tech_school>(this_nation), modifier_offset, value_type(1));
		cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.w.nation_s.nations.get<nation::national_value>(this_nation), modifier_offset, value_type(1));

		if(ws.w.nation_s.nations.get<nation::is_civilized>(this_nation) == false)
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.unciv_nation, modifier_offset, value_type(1));
		else if(nations::is_great_power(ws, this_nation))
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.great_power, modifier_offset, value_type(1));
		else if(ws.w.nation_s.nations.get<nation::overall_rank>(this_nation) <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.second_power, modifier_offset, value_type(1));
		else
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.civ_nation, modifier_offset, value_type(1));
		
		for(int32_t i = int32_t(ws.s.issues_m.issues_container.size()); i--; ) {
			auto active_option = ws.w.nation_s.active_issue_options.get(nation_id, issues::issue_tag(issues::issue_tag::value_base_t(i)));
			if(is_valid_index(active_option))
				cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.issues_m.options[active_option].modifier, modifier_offset, value_type(1));
		}

		for(int32_t i = int32_t(ws.s.technology_m.technologies_container.size()); i--; ) {
			technologies::tech_tag tag(static_cast<technologies::tech_tag::value_base_t>(i));
			if(ws.w.nation_s.active_technologies.get(nation_id, tag))
				cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.technology_m.technologies_container[tag].modifier, modifier_offset, value_type(1));
		}

		bool at_war = get_size(ws.w.military_s.war_arrays, ws.w.nation_s.nations.get<nation::wars_involved_in>(this_nation)) != 0;
		auto central_province_count = ws.w.nation_s.nations.get<nation::central_province_count>(this_nation);
		if(at_war) {
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.war, modifier_offset, value_type(1));
		} else {
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.peace, modifier_offset, value_type(1));
			if(ws.w.nation_s.nations.get<nation::rebel_controlled_provinces>(this_nation) != 0 && central_province_count != 0)
				cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.total_occupation, modifier_offset, float(ws.w.nation_s.nations.get<nation::rebel_controlled_provinces>(this_nation)) / float(central_province_count));
		}

		cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.badboy, modifier_offset, ws.w.nation_s.nations.get<nation::infamy>(this_nation));
		if(central_province_count != 0)
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.total_blockaded, modifier_offset, float(ws.w.nation_s.nations.get<nation::blockaded_count>(this_nation)) / float(central_province_count));
		cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.war_exhaustion, modifier_offset, 100.0f * ws.w.nation_s.nations.get<nation::war_exhaustion>(this_nation));
		cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.plurality, modifier_offset, 100.0f * ws.w.nation_s.nations.get<nation::plurality>(this_nation));
		
		if(is_valid_index(ws.w.nation_s.nations.get<nation::disarmed_until>(this_nation)) & (ws.w.current_date < ws.w.nation_s.nations.get<nation::disarmed_until>(this_nation)))
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.disarming, modifier_offset, value_type(1));
		
		auto total_pop = ws.w.nation_s.nation_demographics.get(nation_id, population::total_population_tag);
		auto literacy = ws.w.nation_s.nation_demographics.get(nation_id, population::literacy_demo_tag(ws));
		if(total_pop != 0)
			cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.average_literacy, modifier_offset, float(literacy) / float(total_pop));

		//if((this_nation.flags & nations::nation::is_bankrupt) != 0) {
		//	cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.in_bankrupcy, modifier_offset, value_type(1));
		//	cursor_in = display_single_national_modifier_value(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.static_modifiers.generalised_debt_default, modifier_offset, value_type(1));
		//}

		return cursor_in;
	}

	namespace {
		ui::xy_pair display_value(float value, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			char16_t local_buffer[32];
			ui::text_format local_fmt{ (value < 0.0f) ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size };
			if(value < 0.0f)
				local_buffer[0] = u'-';
			else
				local_buffer[0] = u'+';

			auto end = put_pos_value_in_buffer(local_buffer + 1, display_type::integer, std::abs(value));
			*(end) = u'%';
			*(end + 1) = char16_t(0);
			cursor_in = ui::add_text(cursor_in, local_buffer, local_fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return cursor_in;
		}
		ui::xy_pair display_multiplicative_value(float value, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, bool invert_color) {

			ui::text_format local_fmt{ ((value < 1.0f) != invert_color) ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size };
			
			cursor_in = ui::add_text(cursor_in, text_data::fp_two_places{ value }, local_fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return cursor_in;
		}
	}

	ui::xy_pair make_additive_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, uint32_t base_text) {

		auto chance = std::max(0.0f, test_additive_factor(f, ws, primary_slot, from_slot));

		cursor_in = display_value(chance, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[base_text], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		lm.increase_indent(1);

		cursor_in = display_value(f.factor, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::base_plus], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		cursor_in = make_factor_text_body(f, ws, container, cursor_in, lm, fmt, primary_slot, from_slot);

		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair make_abs_value_additive_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, bool final_value_is_additive, uint32_t base_text) {

		auto chance = std::max(0.0f, test_additive_factor(f, ws, primary_slot, from_slot));

		if(final_value_is_additive)
			cursor_in = ui::display_colored_additive_factor(cursor_in, chance, fmt, ws, container, lm, false);
		else
			cursor_in = ui::display_colored_factor(cursor_in, chance, fmt, ws, container, lm);

		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[base_text], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		lm.increase_indent(1);

		cursor_in = ui::display_colored_additive_factor(cursor_in, f.factor, fmt, ws, container, lm, false);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::base_plus], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		cursor_in = make_abs_factor_text_body(f, ws, container, cursor_in, lm, fmt, primary_slot, from_slot);

		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair make_multiplicative_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, uint32_t base_text) {

		auto chance = std::max(0.0f, test_multiplicative_factor(f, ws, primary_slot, from_slot));

		cursor_in = display_value(chance, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[base_text], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		lm.increase_indent(1);

		cursor_in = display_value(f.factor, ws, container, cursor_in, lm, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::base_times], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		cursor_in = make_multiplicative_factor_text_body(f, ws, container, cursor_in, lm, fmt, primary_slot, from_slot, false);

		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair make_abs_value_multiplicative_factor_explanation(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, uint32_t base_text) {

		auto chance = std::max(0.0f, test_multiplicative_factor(f, ws, primary_slot, from_slot));

		cursor_in = ui::display_colored_additive_factor(cursor_in, chance, fmt, ws, container, lm, false);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[base_text], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		lm.increase_indent(1);

		cursor_in = ui::display_colored_additive_factor(cursor_in, f.factor, fmt, ws, container, lm, false);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::base_times], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		cursor_in = make_multiplicative_factor_text_body(f, ws, container, cursor_in, lm, fmt, primary_slot, from_slot, false);

		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair make_multiplicative_factor_explanation_in_days(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot) {

		auto chance = std::max(0.0f, test_multiplicative_factor(f, ws, primary_slot, from_slot));

		cursor_in = ui::add_text(cursor_in, text_data::fp_two_places{ chance }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::tx_day], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();
		lm.increase_indent(1);

		cursor_in = ui::add_text(cursor_in, text_data::fp_two_places{ f.factor }, fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

		cursor_in = ui::add_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::base_times], fmt, ws, container, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
		lm.finish_current_line();

		cursor_in = make_multiplicative_factor_text_body(f, ws, container, cursor_in, lm, fmt, primary_slot, from_slot, true);

		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair make_factor_text_body(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot) {

		for(uint32_t i = 0; i < f.data_length; ++i) {
			auto segment = ws.s.modifiers_m.factor_data[f.data_offset + i];

			cursor_in = display_value(segment.factor, ws, container, cursor_in, lm, fmt);
			cursor_in = triggers::make_trigger_description(ws, container, cursor_in, lm, fmt, ws.s.trigger_m.trigger_data.data() + to_index(segment.condition), primary_slot, primary_slot, from_slot);
		}

		return cursor_in;
	}
	ui::xy_pair make_abs_factor_text_body(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot) {

		for(uint32_t i = 0; i < f.data_length; ++i) {
			auto segment = ws.s.modifiers_m.factor_data[f.data_offset + i];

			cursor_in = ui::display_colored_additive_factor(cursor_in, segment.factor, fmt, ws, container, lm, false);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = triggers::make_trigger_description(ws, container, cursor_in, lm, fmt, ws.s.trigger_m.trigger_data.data() + to_index(segment.condition), primary_slot, primary_slot, from_slot);
		}

		return cursor_in;
	}
	ui::xy_pair make_multiplicative_factor_text_body(factor_modifier const& f, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt,
		triggers::const_parameter primary_slot, triggers::const_parameter from_slot, bool invert_colors) {

		for(uint32_t i = 0; i < f.data_length; ++i) {
			auto segment = ws.s.modifiers_m.factor_data[f.data_offset + i];

			cursor_in = display_multiplicative_value(segment.factor, ws, container, cursor_in, lm, fmt, invert_colors);
			cursor_in = triggers::make_trigger_description(ws, container, cursor_in, lm, fmt, ws.s.trigger_m.trigger_data.data() + to_index(segment.condition), primary_slot, primary_slot, from_slot);
		}

		return cursor_in;
	}
	void focus_choice_button::update(ui::simple_button<focus_choice_button>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, modifiers::nf_tag_to_frame(ws, tag));
		if(auto const player = ws.w.local_player_nation; player && tag) {
			auto max_focuses = modifiers::maximum_national_focuses(ws, player);
			auto used_focuses = modifiers::current_focus_count(ws, player);

			if(used_focuses > max_focuses
				|| (used_focuses == max_focuses && !is_valid_index(ws.w.nation_s.states.get<state::owner_national_focus>(ws.w.national_focus_w.in_state)))) {
				self.set_enabled(false);
				return;
			}
			if(auto const lim = ws.s.modifiers_m.national_focuses[tag].limit; lim) {
				if(!triggers::test_trigger(
					ws.s.trigger_m.trigger_data.data() + to_index(lim),
					ws, ws.w.nation_s.states.get<state::state_capital>(ws.w.national_focus_w.in_state),
					ws.w.nation_s.states.get<state::owner>(ws.w.national_focus_w.in_state),
					triggers::const_parameter())) {

					self.set_enabled(false);
					return;
				}
			}
		}
		self.set_enabled(true);
	}
	void focus_choice_button::button_function(ui::simple_button<focus_choice_button>& self, world_state & ws) {
		ws.w.pending_commands.add<commands::change_national_focus>(ws.w.local_player_nation, ws.w.national_focus_w.in_state, tag);
		ws.w.national_focus_w.hide(ws.w.gui_m);
	}
	void focus_choice_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		nf_modifier_text(tag, ws.w.national_focus_w.in_state, ws, tw, ui::xy_pair{0,0}, lm, ui::tooltip_text_format);
	}
	void nf_window_header::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(ws.w.national_focus_w.win->choose_new_focus)
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::national_focus, fmt, ws, box, lm);
		else
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::replace_national_focus, fmt, ws, box, lm);
	}
	void close_nf_window::button_function(ui::simple_button<close_nf_window>& self, world_state & ws) {
		ws.w.national_focus_w.hide(ws.w.gui_m);
	}

	uint32_t nf_tag_to_frame(world_state const& ws, national_focus_tag t) {
		if(t) 
			return uint32_t(ws.s.modifiers_m.national_focuses[t].icon - 1);
		else
			return 0;
	}
	bool nf_button_clickable(world_state const& ws, nations::state_tag s) {
		return s && ws.w.nation_s.states.get<state::owner>(s) == ws.w.local_player_nation;
	}
	ui::xy_pair nf_modifier_text(national_focus_tag nf, nations::state_tag s, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
		if(!nf) {
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::no_focus, fmt, ws, container, lm);
		} else {
			cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.national_focuses[nf].name, fmt, ws, container, lm);
		}
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		if(nf) {
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

			auto max_focuses = modifiers::maximum_national_focuses(ws, ws.w.local_player_nation);
			auto used_focuses = modifiers::current_focus_count(ws, ws.w.local_player_nation);

			if(used_focuses < max_focuses || is_valid_index(ws.w.nation_s.states.get<state::owner_national_focus>(s))) {
				ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, container, lm);
			} else {
				ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, container, lm);
			}
			text_data::text_replacement repl(text_data::value_type::value, text_data::integer{ max_focuses });
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::focus_limit, fmt, ws, container, lm, &repl, 1);

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

			if(auto const lim = ws.s.modifiers_m.national_focuses[nf].limit; lim) {
				cursor_in = triggers::make_trigger_description(
					ws,
					container,
					cursor_in,
					lm,
					fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(lim),
					ws.w.nation_s.states.get<state::state_capital>(s),
					ws.w.nation_s.states.get<state::owner>(s),
					triggers::const_parameter()
				);
			}

			if(ws.s.modifiers_m.national_focuses[nf].modifier) {
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);
				cursor_in = make_province_modifier_text_body(
					ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.national_focuses[nf].modifier]);
			}
		}

		return cursor_in;
	}
	ui::xy_pair nf_tooltip_text(nations::state_tag s, world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
		if(s) {
			auto const nf = ws.w.nation_s.states.get<state::owner_national_focus>(s);

			if(!nf) {
				cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::no_focus, fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);
			} else {
				cursor_in = ui::add_text(cursor_in, ws.s.modifiers_m.national_focuses[nf].name, fmt, ws, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

				if(ws.s.modifiers_m.national_focuses[nf].modifier) {
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);
					cursor_in = make_province_modifier_text_body(
						ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.provincial_modifier_definitions[ws.s.modifiers_m.national_focuses[nf].modifier]);
				}
			}
		} else {
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::no_focus, fmt, ws, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);
		}
		
		return cursor_in;
	}
	national_focus_window::national_focus_window() : win(std::make_unique<national_focus_window_t>()) {}
	national_focus_window::~national_focus_window() {}
	void national_focus_window::hide(ui::gui_manager & gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void national_focus_window::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_nf_window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void national_focus_window::update(ui::gui_manager & gui_m, nations::state_tag s) {
		in_state = s;
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void national_focus_window::show(ui::gui_manager & gui_m, nations::state_tag s, int32_t x, int32_t y) {
		if(win->associated_object->size.x + x < gui_m.root.size.x) {
			win->associated_object->position.x = int16_t(x);
			win->associated_object->position.y = int16_t(std::clamp(y - win->associated_object->size.y / 2, 0, gui_m.root.size.y - win->associated_object->size.y));
		} else {
			win->associated_object->position.x = int16_t(x - win->associated_object->size.x);
			win->associated_object->position.y = int16_t(std::clamp(y - win->associated_object->size.y / 2, 0, gui_m.root.size.y - win->associated_object->size.y));
		}

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m, s);
	}
	void national_focus_window_base::update(world_state & ws) {
		auto max_focuses = modifiers::maximum_national_focuses(ws, ws.w.local_player_nation);
		auto used_focuses = modifiers::current_focus_count(ws, ws.w.local_player_nation);
		choose_new_focus = (used_focuses < max_focuses) || is_valid_index(ws.w.nation_s.states.get<state::owner_national_focus>(ws.w.national_focus_w.in_state));

		if(choose_new_focus) {
			for(auto& b : nf_buttons) {
				ui::make_visible_immediate(*b.associated_object);
			}
			ui::hide(*(ws.w.national_focus_w.win->get<CT_STRING("nf_listbox")>().associated_object));
		} else {
			for(auto& b : nf_buttons) {
				ui::hide(*b.associated_object);
			}
			ui::make_visible_and_update(ws.w.gui_m, *(ws.w.national_focus_w.win->get<CT_STRING("nf_listbox")>().associated_object));
		}
	}
	void replacement_choice_button::button_function(ui::simple_button<replacement_choice_button>& self, world_state & ws) {
		ws.w.pending_commands.add<commands::change_national_focus>(ws.w.local_player_nation, tag, national_focus_tag());
		//ws.w.national_focus_w.update(ws.w.gui_m, ws.w.national_focus_w.in_state);
	}
	ui::window_tag nf_replacement_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_nf_overwrite_entry"]);
	}
}
