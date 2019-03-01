#include "common\\common.h"
#include "world_state.h"
#include "messages.hpp"

namespace messages {
	constexpr char const* message_identifiers[] = {
		"WEGAINCB",
		"REVOLTALLIANCE",
		"WAR_THEY_ACCEPT",
		"WAR_WE_ACCEPT",
		"WAR_OTHER_ACCEPT",
		"WARGOAL_THEY_ACCEPT",
		"WARGOAL_WE_ACCEPT",
		"WARGOAL_OTHER_ACCEPT",
		"SIEGEOVER",
		"SIEGEUS",
		"SIEGEUSREBEL",
		"SIEGETHEM",
		"SIEGETHEMREBEL",
		"COLONY_SUCCESS",
		"COLONY_FAIL",
		"COLONY_ABORT",
		"SOCREFORM",
		"SOCREFORM_BAD",
		"POLREFORM",
		"POLREFORM_BAD",
		"ECONOMICREFORM",
		"ECONOMICREFORM_BAD",
		"MILITARYREFORM",
		"MILITARYREFORM_BAD",
		"SETPARTY",
		"UPPERHOUSE",
		"ELECTIONSTART",
		"ELECTIONDONE",
		"BREAKCOUNTRY",
		"BREAKCOUNTRYOTHER",
		"ANNEX",
		"PEACE_WE_ACCEPT",
		"PEACE_WE_DECLINE",
		"PEACE_THEY_ACCEPT",
		"PEACE_THEY_DECLINE",
		"PEACE_OTHER_ACCEPT",
		"PEACE_OTHER_DECLINE",
		"PARTISANS",
		"WE_MOBILIZE",
		"OTHER_MOBILIZE",
		"WE_DEMOBILIZE",
		"OTHER_DEMOBILIZE",
		"BUILDING_DONE",
		"WEDEFECT",
		"THEYDEFECT",
		"OTHERDEFECT",
		"EVENTHAPPENOTHER",
		"EVENTHAPPENOTHEROPTION",
		"MAJOREVENTHAPPENOTHER",
		"MAJOREVENTHAPPENOTHEROPTION",
		"EVENTHAPPENUS // log only",
		"INVENTION",
		"TECH_ONCE",
		"TRIGMOD",
		"TRIGMODLOST",
		"LEADERDIED",
		"ATTACKTHEM",
		"ATTACKUS",
		"LANDBATTLEOVER",
		"NAVALBATTLEOVER",
		"ALLYTAKEOVER",
		"ALLYTAKEOVERWE",
		"INDEPENDANCE",
		"PROVINCELOST",
		"NAVALATTACKTHEM",
		"NAVALATTACKUS",
		"UNITARRIVED",
		"DECISIONOTHER",
		"DECISIONUS",
		"WE_NO_LONGER_GREAT_POWER",
		"THEY_NO_LONGER_GREAT_POWER",
		"WE_BECAME_GREAT_POWER",
		"ALLIANCE_THEY_ACCEPT",
		"ALLIANCE_WE_ACCEPT",
		"ALLIANCE_OTHER_ACCEPT",
		"ALLIANCE_THEY_DECLINE",
		"ALLIANCE_WE_DECLINE",
		"ALLIANCE_OTHER_DECLINE",
		"CANCELALLIANCE_THEY_ACCEPT",
		"CANCELALLIANCE_WE_ACCEPT",
		"CANCELALLIANCE_OTHER_ACCEPT",
		"CALLALLY_THEY_ACCEPT",
		"CALLALLY_WE_ACCEPT",
		"CALLALLY_OTHER_ACCEPT",
		"CALLALLY_THEY_DECLINE",
		"CALLALLY_WE_DECLINE",
		"CALLALLY_OTHER_DECLINE",
		"EXPELADVISORS_THEY_ACCEPT",
		"EXPELADVISORS_WE_ACCEPT",
		"EXPELADVISORS_OTHER_ACCEPT",
		"BANEMBASSY_THEY_ACCEPT",
		"BANEMBASSY_WE_ACCEPT",
		"BANEMBASSY_OTHER_ACCEPT",
		"INCREASEOPINION_THEY_ACCEPT",
		"INCREASEOPINION_WE_ACCEPT",
		"INCREASEOPINION_OTHER_ACCEPT",
		"DECREASEOPINION_THEY_ACCEPT",
		"DECREASEOPINION_WE_ACCEPT",
		"DECREASEOPINION_OTHER_ACCEPT",
		"ADDTOSPHERE_THEY_ACCEPT",
		"ADDTOSPHERE_WE_ACCEPT",
		"ADDTOSPHERE_OTHER_ACCEPT",
		"REMOVEFROMSPHERE_THEY_ACCEPT",
		"REMOVEFROMSPHERE_WE_ACCEPT",
		"REMOVEFROMSPHERE_OTHER_ACCEPT",
		"INCREASERELATION_THEY_ACCEPT",
		"INCREASERELATION_WE_ACCEPT",
		"INCREASERELATION_OTHER_ACCEPT",
		"DECREASERELATION_THEY_ACCEPT",
		"DECREASERELATION_WE_ACCEPT",
		"DECREASERELATION_OTHER_ACCEPT",
		"WETARGET_EXPELADVISORS",
		"WETARGET_BANEMBASSY",
		"WETARGET_DECREASEOPINION",
		"WETARGET_REMOVEFROMSPHERE",
		"INTERVENTION_OUR_SIDE",
		"INTERVENTION_THEIR_SIDE",
		"INTERVENTION_OTHER_SIDE",
		"WE_INTERVENED",
		"BANKRUPTCY",
		"THEY_BANKRUPTCY",
		"NEW_PARTY_AVAIL",
		"PARTY_UNAVAIL",
		"GW_DISCOVERED",
		"WAR_GAIN_GW_STATUS",
		"OUR_CB_DETECTED",
		"CB_TOWARDS_US_DETECTED",
		"OTHERS_CB_DETECTED",
		"CB_JUSTIFY_NO_LONGER_VALID",
		"INVESTED_IN_US",
		"CRISIS_OFFER_THEY_ACCEPT",
		"CRISIS_OFFER_WE_ACCEPT",
		"CRISIS_OFFER_OTHER_ACCEPT",
		"CRISIS_OFFER_THEY_DECLINE",
		"CRISIS_OFFER_WE_DECLINE",
		"CRISIS_OFFER_OTHER_DECLINE",
		"BACK_CRISIS_THEY_ACCEPT",
		"BACK_CRISIS_WE_ACCEPT",
		"BACK_CRISIS_OTHER_ACCEPT",
		"BACK_CRISIS_THEY_DECLINE",
		"BACK_CRISIS_WE_DECLINE",
		"BACK_CRISIS_OTHER_DECLINE",
		"CRISIS_BACK_DOWN_WE_ACCEPT",
		"CRISIS_BACK_DOWN_WE_DECLINE",
		"CRISIS_BACK_DOWN_THEY_ACCEPT",
		"CRISIS_BACK_DOWN_THEY_DECLINE",
		"CRISIS_BACK_DOWN_OTHER_ACCEPT",
		"CRISIS_BACK_DOWN_OTHER_DECLINE",
		"CRISIS_NO_SIDE_PICKED",
		"CRISIS_BECAME_WAR",
		"CRISIS_STARTED",
		"CRISIS_NOBODY_BACKED",
	};

	message_window::message_window() : win(std::make_unique<message_window_t>()) {}
	message_window::~message_window() {}

	void message_window::hide_message_window(ui::gui_manager& gui_m) {
		current_message_count = 0;
		last_replaced_index = 0;
		for(auto& m : displayed_messages)
			m = message_instance{};

		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}

	void message_window::update_message_window(ui::gui_manager& gui_m) {
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void message_window::show_message_window(ui::gui_manager& gui_m) {
		currently_displayed_index = 0;
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void message_window::init_message_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["ShieldedInformationDialog"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void init_message_text(scenario::scenario_manager& s) {
		for(int32_t i = 0; i < message_count; ++i) {
			std::string log_key = std::string(message_identifiers[i]) + "_LOG";
			s.message_m.log_text[i] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, log_key.c_str(), log_key.c_str() + log_key.size());
			for(int32_t j = 0; j < 6; ++j) {
				std::string line_key = std::string(message_identifiers[i]) + "_" + std::to_string(j + 1);
				s.message_m.message_text[i].line[j] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, line_key.c_str(), line_key.c_str() + line_key.size());
			}
		}
	}

	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.message_w.hide_message_window(ws.w.gui_m);
	}

	void next_button::button_function(ui::simple_button<next_button>&, world_state& ws) {
		++ws.w.message_w.currently_displayed_index;
		if(ws.w.message_w.currently_displayed_index > ws.w.message_w.current_message_count)
			ws.w.message_w.currently_displayed_index = 0;
		ws.w.message_w.update_message_window(ws.w.gui_m);
	}

	void prev_button::button_function(ui::simple_button<prev_button>&, world_state& ws) {
		--ws.w.message_w.currently_displayed_index;
		if(ws.w.message_w.currently_displayed_index < 0)
			ws.w.message_w.currently_displayed_index = ws.w.message_w.current_message_count - 1;
		ws.w.message_w.update_message_window(ws.w.gui_m);
	}

	void goto_button::button_function(ui::button<goto_button>&, world_state&) {}
	void goto_button::update(ui::button<goto_button>& self, world_state& ws) {
		self.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::goto_label]);
	}

	void message_window_base::update(world_state& ws) {
		message_instance m;
		while(ws.w.message_w.pending_messages.try_pop(m)) {
			if(ws.w.message_w.current_message_count < messages::maximum_displayed_messages) {
				ws.w.message_w.displayed_messages[ws.w.message_w.current_message_count] = m;
				++ws.w.message_w.current_message_count;
			} else {
				ws.w.message_w.displayed_messages[ws.w.message_w.last_replaced_index] = m;
				ws.w.message_w.last_replaced_index = (ws.w.message_w.last_replaced_index + 1) % messages::maximum_displayed_messages;
			}
		}
		
	}

	void messaage_flag::update(ui::masked_flag<messaage_flag>& self, world_state& ws) {
		auto const goto_variant = ws.w.message_w.displayed_messages[ws.w.message_w.currently_displayed_index].goto_tag;
		if(std::holds_alternative<nations::country_tag>(goto_variant)) {
			self.set_displayed_flag(ws, std::get<nations::country_tag>(goto_variant));
		} else if(std::holds_alternative<provinces::province_tag>(goto_variant)) {
			self.set_displayed_flag(ws, ws.w.province_s.province_state_container.get<province_state::owner>(std::get<provinces::province_tag>(goto_variant)));
		} else {
			self.set_displayed_flag(ws, ws.w.local_player_nation);
		}
	}

	void message_header::update(
		ui::tagged_gui_object box,
		ui::text_box_line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {

		char16_t local_buffer[16];
		put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.message_w.currently_displayed_index + 1);
		ui::xy_pair cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);

		put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.message_w.current_message_count);
		cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u" / "), box, cursor, fmt, lm);

		put_value_in_buffer(local_buffer, display_type::exact_integer, ws.w.message_w.current_message_count);
		cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, cursor, fmt, lm);

		lm.finish_current_line();
	}

	void message_body::update(ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws) {
		if(ws.w.message_w.displayed_messages[ws.w.message_w.currently_displayed_index].func != nullptr) {
			ws.w.message_w.displayed_messages[ws.w.message_w.currently_displayed_index].func(ws, box, lm, fmt);
		}
	}
}
