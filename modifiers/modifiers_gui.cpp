#include "common\\common.h"
#include "modifiers_gui.h"
#include "world_state\\world_state.h"
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
		modifier_display_details{ true, display_type::percent }, // local_factory_input 14
		modifier_display_details{ true, display_type::percent }, // local_factory_output 15
		modifier_display_details{ true, display_type::percent }, // local_factory_throughput 16
		modifier_display_details{ true, display_type::percent }, // local_rgo_input 17
		modifier_display_details{ true, display_type::percent }, // local_rgo_output 18
		modifier_display_details{ true, display_type::percent }, // local_rgo_throughput 19
		modifier_display_details{ true, display_type::percent }, // local_artisan_input 20
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
		modifier_display_details{ true, display_type::percent }, // factory_input 42
		modifier_display_details{ true, display_type::percent }, // factory_output 43
		modifier_display_details{ true, display_type::percent }, // factory_throughput 44
		modifier_display_details{ true, display_type::percent }, // rgo_input 45
		modifier_display_details{ true, display_type::percent }, // rgo_output 46
		modifier_display_details{ true, display_type::percent }, // rgo_throughput 47
		modifier_display_details{ true, display_type::percent }, // artisan_input 48
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
		modifier_display_details{ true, display_type::fp_two_places }, // suppression_points_modifier 81
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

	
	
	ui::xy_pair make_province_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, value_type* values) {
		char16_t local_buf[64];
		for(uint32_t i = 0; i < provincial_offsets::count; ++i) {
			if(values[i] != value_type(0)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.modifiers_m.province_offset_names[i], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

				if(values[i] < value_type(0)) {
					local_buf[0] = u'-';
					put_pos_value_in_buffer(local_buf + 1, province_offset_display[i].display_as, -values[i]);
					cursor_in = ui::text_chunk_to_instances(
						ws.s.gui_m,
						ws.w.gui_m,
						vector_backed_string<char16_t>(local_buf),
						container,
						cursor_in,
						ui::text_format{ province_offset_display[i].positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size },
						lm);
				} else {
					local_buf[0] = u'+';
					put_pos_value_in_buffer(local_buf + 1, province_offset_display[i].display_as, values[i]);
					cursor_in = ui::text_chunk_to_instances(
						ws.s.gui_m,
						ws.w.gui_m,
						vector_backed_string<char16_t>(local_buf),
						container,
						cursor_in,
						ui::text_format{ province_offset_display[i].positive_is_green ? ui::text_color::green : ui::text_color::red, fmt.font_handle, fmt.font_size },
						lm);
				}
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}
		}
		return cursor_in;
	}

	ui::xy_pair make_national_modifier_text_body(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, value_type* values) {
		char16_t local_buf[64];
		for(uint32_t i = 0; i < national_offsets::count; ++i) {
			if(values[i] != value_type(0)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.modifiers_m.province_offset_names[i], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

				if(values[i] < value_type(0)) {
					local_buf[0] = u'-';
					put_pos_value_in_buffer(local_buf + 1, national_offset_display[i].display_as, -values[i]);
					cursor_in = ui::text_chunk_to_instances(
						ws.s.gui_m,
						ws.w.gui_m,
						vector_backed_string<char16_t>(local_buf),
						container,
						cursor_in,
						ui::text_format{ national_offset_display[i].positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size },
						lm);
				} else {
					local_buf[0] = u'+';
					put_pos_value_in_buffer(local_buf + 1, national_offset_display[i].display_as, values[i]);
					cursor_in = ui::text_chunk_to_instances(
						ws.s.gui_m,
						ws.w.gui_m,
						vector_backed_string<char16_t>(local_buf),
						container,
						cursor_in,
						ui::text_format{ national_offset_display[i].positive_is_green ? ui::text_color::green : ui::text_color::red, fmt.font_handle, fmt.font_size },
						lm);
				}
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}
		}
		return cursor_in;
	}
	ui::xy_pair make_province_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, provincial_modifier_tag t) {
		auto name = ws.s.modifiers_m.provincial_modifiers[t].name;
		if(is_valid_index(name)) {
			cursor_in = ui::add_linear_text(cursor_in, name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		lm.increase_indent(1);
		cursor_in = make_province_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.provincial_modifier_definitions.get_row(t));
		lm.decrease_indent(1);
		return cursor_in;
	}
	ui::xy_pair make_national_modifier_text(world_state& ws, ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, national_modifier_tag t) {
		auto name = ws.s.modifiers_m.national_modifiers[t].name;
		if(is_valid_index(name)) {
			cursor_in = ui::add_linear_text(cursor_in, name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
		}
		lm.increase_indent(1);
		cursor_in = make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions.get_row(t));
		lm.decrease_indent(1);
		return cursor_in;
	}
	
}
