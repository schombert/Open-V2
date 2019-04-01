#include "common\\common.h"
#include "world_state.h"
#include "messages.hpp"
#include "technologies\\technologies_gui.h"

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
		"WELOSECB",
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
		if(ws.w.message_w.currently_displayed_index >= ws.w.message_w.current_message_count)
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
			auto revised_format = ui::text_format{ fmt.color, graphics::font_tag(1), fmt.font_size };
			ws.w.message_w.displayed_messages[ws.w.message_w.currently_displayed_index].func(ws, box, lm, revised_format);
		}
	}

	void submit_message(world_state& ws, std::variant<std::monostate, nations::country_tag, provinces::province_tag> goto_tag, display_function&& f) {
		ws.w.message_w.pending_messages.push(message_instance{ std::move(f), goto_tag });
		ws.w.message_w.show_message_window(ws.w.gui_m);
	}

	void submit_log_item(world_state& ws, int32_t category, log_display_function&& f) {
		ws.w.message_w.pending_log_items.push(log_message_instance{ std::move(f), category });
	}

	message_setting get_single_setting(world_state const& ws, message_settings s, nations::country_tag n) {
		if(n == ws.w.local_player_nation)
			return s.self;
		else
			return s.interesting_countries;
	}

	template<typename ... NATIONS>
	message_setting get_relevant_setting(world_state const& ws, message_settings s, nations::country_tag n, NATIONS ... rest) {
		return merge_message_setting(get_single_setting(ws, s, n), get_single_setting(ws, s, rest ...));
	}

	class nation_hyperlink {
	public:
		nations::country_tag target;

		nation_hyperlink(nations::country_tag t) : target(t) {}

		void button_function(ui::simple_button<nation_hyperlink>&, world_state& ws) {
			ws.w.diplomacy_w.show_diplomacy_window(ws.w.gui_m, target);
		}
	};

	void display_message_body(world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, int32_t message_id, text_data::replacement* repl, int32_t replacements_length, int32_t max_line = 6) {
		auto cursor = add_multiline_text(
			ui::xy_pair{ 0,0 },
			ws.s.message_m.message_text[message_id].line[0],
			fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, replacements_length);
		lm.finish_current_line();
		cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt);

		for(int32_t i = 1; i < max_line; ++i) {
			cursor = add_multiline_text(
				cursor,
				ws.s.message_m.message_text[message_id].line[i],
				fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, replacements_length);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
		}
		lm.finish_current_line();
	}

	void cb_detected(world_state& ws, nations::country_tag by, nations::country_tag target, military::cb_type_tag type, float infamy_gained) {
		if(by == ws.w.local_player_nation) {
			// case by player detected
			switch(ws.w.message_w.settings[message_type::OUR_CB_DETECTED].self) {
				case message_setting::popup_and_pause:
					ws.w.paused.store(true, std::memory_order_release);
					// fallthrough
				case message_setting::popup:
					submit_message(ws, target, [type, target, infamy_gained](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
						char16_t local_buffer[16];
						put_value_in_buffer(local_buffer, display_type::fp_one_place, infamy_gained);

						text_data::replacement repl[3] = {
							text_data::replacement{text_data::value_type::casus, 
								text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name), 
								[](ui::tagged_gui_object) {}},
							text_data::replacement{text_data::value_type::target,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
								[&ws, target](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
							}},
							text_data::replacement{text_data::value_type::badboy,
								vector_backed_string<char16_t>(local_buffer),
								[](ui::tagged_gui_object) {}}
						};

						display_message_body(ws, box, lm, fmt, message_type::OUR_CB_DETECTED, repl, 3);
					});
					// fallthrough
				case message_setting::log:
					submit_log_item(ws, message_category::diplomacy, [target](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
						text_data::replacement repl[1] = {
							text_data::replacement{text_data::value_type::target,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
								[&ws, target](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
							}}
						};

						ui::add_linear_text(
							ui::xy_pair{ 0,0 },
							ws.s.message_m.log_text[message_type::OUR_CB_DETECTED],
							fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 1);
						lm.finish_current_line();
					});
					break;
				case message_setting::discard:
					//do nothing
					break;
			}
		} else if(target == ws.w.local_player_nation) {
			// case against player detected
			switch(ws.w.message_w.settings[message_type::CB_TOWARDS_US_DETECTED].self) {
				case message_setting::popup_and_pause:
					ws.w.paused.store(true, std::memory_order_release);
					// fallthrough
				case message_setting::popup:
					submit_message(ws, by, [type, by](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
						text_data::replacement repl[2] = {
							text_data::replacement{text_data::value_type::casus,
								text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
								[](ui::tagged_gui_object) {}},
							text_data::replacement{text_data::value_type::country,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(by)) ,
								[&ws, by](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, by);
							}}
						};

						display_message_body(ws, box, lm, fmt, message_type::CB_TOWARDS_US_DETECTED, repl, 2);
					});
					// fallthrough
				case message_setting::log:
					submit_log_item(ws, message_category::diplomacy, [by](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
						text_data::replacement repl[1] = {
							text_data::replacement{text_data::value_type::country,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(by)) ,
								[&ws, by](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, by);
							}}
						};

						ui::add_linear_text(
							ui::xy_pair{ 0,0 },
							ws.s.message_m.log_text[message_type::CB_TOWARDS_US_DETECTED],
							fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 1);
						lm.finish_current_line();
					});
					break;
				case message_setting::discard:
					//do nothing
					break;
			}
		} else {
			// all others
			switch(get_relevant_setting(ws, ws.w.message_w.settings[message_type::OTHERS_CB_DETECTED], target, by)) {
				case message_setting::popup_and_pause:
					ws.w.paused.store(true, std::memory_order_release);
					// fallthrough
				case message_setting::popup:
					submit_message(ws, target, [type, target, by](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
						text_data::replacement repl[3] = {
							text_data::replacement{text_data::value_type::casus,
								text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
								[](ui::tagged_gui_object) {}},
							text_data::replacement{text_data::value_type::country,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(by)) ,
								[&ws, by](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, by);
							}},
							text_data::replacement{text_data::value_type::target,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
								[&ws, target](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
							}}
						};

						display_message_body(ws, box, lm, fmt, message_type::OTHERS_CB_DETECTED, repl, 3);
					});
					// fallthrough
				case message_setting::log:
					submit_log_item(ws, message_category::diplomacy, [by, target](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
						text_data::replacement repl[2] = {
							text_data::replacement{text_data::value_type::country,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(by)) ,
								[&ws, by](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, by);
							}},
							text_data::replacement{text_data::value_type::target,
								 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
								[&ws, target](ui::tagged_gui_object b) {
								ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
							}}
						};

						ui::add_linear_text(
							ui::xy_pair{ 0,0 },
							ws.s.message_m.log_text[message_type::OTHERS_CB_DETECTED],
							fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 2);
						lm.finish_current_line();
					});
					break;
				case message_setting::discard:
					//do nothing
					break;
			}
		}
	}

	void player_cb_construction_invalid(world_state& ws, nations::country_tag target, military::cb_type_tag type) {
		switch(ws.w.message_w.settings[message_type::CB_JUSTIFY_NO_LONGER_VALID].self) {
			case message_setting::popup_and_pause:
				ws.w.paused.store(true, std::memory_order_release);
				// fallthrough
			case message_setting::popup:
				submit_message(ws, target, [type, target](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
					text_data::replacement repl[2] = {
						text_data::replacement{text_data::value_type::casus,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
							[](ui::tagged_gui_object) {}},
						text_data::replacement{text_data::value_type::target,
							 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
							[&ws, target](ui::tagged_gui_object b) {
							ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
						}}
					};
					display_message_body(ws, box, lm, fmt, message_type::CB_JUSTIFY_NO_LONGER_VALID, repl, 2);
				});
				// fallthrough
			case message_setting::log:
				submit_log_item(ws, message_category::diplomacy, [target](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
					text_data::replacement repl[1] = {
						text_data::replacement{text_data::value_type::target,
							 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
							[&ws, target](ui::tagged_gui_object b) {
							ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
						}}
					};

					ui::add_linear_text(
						ui::xy_pair{ 0,0 },
						ws.s.message_m.log_text[message_type::CB_JUSTIFY_NO_LONGER_VALID],
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 1);
					lm.finish_current_line();
				});
				break;
			case message_setting::discard:
				//do nothing
				break;
		}
	}

	void player_acquired_cb(world_state& ws, nations::country_tag target, military::cb_type_tag type) {
		switch(ws.w.message_w.settings[message_type::WEGAINCB].self) {
			case message_setting::popup_and_pause:
				ws.w.paused.store(true, std::memory_order_release);
				// fallthrough
			case message_setting::popup:
				submit_message(ws, target, [type, target](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
					text_data::replacement repl[2] = {
						text_data::replacement{text_data::value_type::casus,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
							[](ui::tagged_gui_object) {}},
						text_data::replacement{text_data::value_type::enemy,
							 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
							[&ws, target](ui::tagged_gui_object b) {
							ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
						}}
					};
					display_message_body(ws, box, lm, fmt, message_type::WEGAINCB, repl, 2, 5);
				});
				// fallthrough
			case message_setting::log:
				submit_log_item(ws, message_category::diplomacy, [type, target](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
					text_data::replacement repl[2] = {
						text_data::replacement{text_data::value_type::casus,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
							[](ui::tagged_gui_object) {}},
						text_data::replacement{text_data::value_type::enemy,
							 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
							[&ws, target](ui::tagged_gui_object b) {
							ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
						}}
					};

					ui::add_linear_text(
						ui::xy_pair{ 0,0 },
						ws.s.message_m.log_text[message_type::WEGAINCB],
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 2);
					lm.finish_current_line();
				});
				break;
			case message_setting::discard:
				//do nothing
				break;
		}
	}

	void player_lost_cb(world_state& ws, nations::country_tag target, military::cb_type_tag type) {
		switch(ws.w.message_w.settings[message_type::WELOSECB].self) {
			case message_setting::popup_and_pause:
				ws.w.paused.store(true, std::memory_order_release);
				// fallthrough
			case message_setting::popup:
				submit_message(ws, target, [type, target](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
					text_data::replacement repl[2] = {
						text_data::replacement{text_data::value_type::casus,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
							[](ui::tagged_gui_object) {}},
						text_data::replacement{text_data::value_type::enemy,
							 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
							[&ws, target](ui::tagged_gui_object b) {
							ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
						}}
					};
					display_message_body(ws, box, lm, fmt, message_type::WEGAINCB, repl, 2, 5);
				});
				// fallthrough
			case message_setting::log:
				submit_log_item(ws, message_category::diplomacy, [type, target](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
					text_data::replacement repl[2] = {
						text_data::replacement{text_data::value_type::casus,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
							[](ui::tagged_gui_object) {}},
						text_data::replacement{text_data::value_type::enemy,
							 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target)) ,
							[&ws, target](ui::tagged_gui_object b) {
							ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target);
						}}
					};

					ui::add_linear_text(
						ui::xy_pair{ 0,0 },
						ws.s.message_m.log_text[message_type::WELOSECB],
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 2);
					lm.finish_current_line();
				});
				break;
			case message_setting::discard:
				//do nothing
				break;
		}
	}

	void hidden_button::update(ui::simple_button<hidden_button>& self, world_state & ws) {
		ui::hide(*self.associated_object);
	}

	void player_technology(world_state& ws, technologies::tech_tag type) {
		switch(ws.w.message_w.settings[message_type::TECH_ONCE].self) {
			case message_setting::popup_and_pause:
				ws.w.paused.store(true, std::memory_order_release);
				// fallthrough
			case message_setting::popup:
				submit_message(ws, ws.w.local_player_nation , [type](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
					
					auto cursor = add_multiline_text(
						ui::xy_pair{ 0,0 },
						ws.s.fixed_ui_text[scenario::fixed_ui::tech_researched_header],
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					lm.finish_current_line();
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt);
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt);

					cursor = add_multiline_text(
						cursor,
						ws.s.technology_m.technologies_container[type].name,
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					lm.finish_current_line();
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt);

					technologies::explain_technology(type, ws, box, cursor, lm, fmt);
				});
				// fallthrough
			case message_setting::log:
				submit_log_item(ws, message_category::other, [type](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
					text_data::replacement repl[1] = {
						text_data::replacement{text_data::value_type::type,
							 text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.technology_m.technologies_container[type].name) ,
							[](ui::tagged_gui_object b) {}}
					};

					ui::add_linear_text(
						ui::xy_pair{ 0,0 },
						ws.s.message_m.log_text[message_type::TECH_ONCE],
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, 1);
					lm.finish_current_line();
				});
				break;
			case message_setting::discard:
				//do nothing
				break;
		}
	}
}
