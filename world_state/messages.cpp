#include "common\\common.h"
#include "world_state.h"
#include "messages.hpp"
#include "technologies\\technologies_gui.h"
#include "governments\governments_gui.hpp"
#include "scenario\\settings.h"

namespace messages {
	constexpr char const* message_identifiers[] = {
		"GAINCB",
		"GAINCB_TARGET",
		"LOSECB",
		"LOSECB_TARGET",
		"REVOLTALLIANCE_CAUSE",
		"REVOLTALLIANCE_TARGET",
		"WAR_CAUSE",
		"WAR_TARGET",
		"WARGOAL_CAUSE",
		"WARGOAL_TARGET",
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
		"REBELS",
		"ANNEX_CAUSE",
		"ANNEX_TARGET",
		"PEACE_ACCEPT_CAUSE",
		"PEACE_ACCEPT_TARGET",
		"PEACE_DECLINE_CAUSE",
		"PEACE_DECLINE_TARGET",
		"MOBILIZE",
		"DEMOBILIZE",
		"BUILDING_DONE",
		"EVENTHAPPENOTHER",
		"EVENTHAPPENOTHEROPTION",
		"MAJOREVENTHAPPENOTHER",
		"MAJOREVENTHAPPENOTHEROPTION",
		"INVENTION",
		"TECH",
		"LEADERDIED",
		"LANDBATTLEOVER",
		"NAVALBATTLEOVER",
		"DECISIONOTHER",
		"NO_LONGER_GREAT_POWER",
		"BECAME_GREAT_POWER",
		"ALLIANCE_ACCEPT_CAUSE",
		"ALLIANCE_ACCEPT_TARGET",
		"ALLIANCE_DECLINE_CAUSE",
		"ALLIANCE_DECLINE_TARGET",
		"CANCELALLIANCE_CAUSE",
		"CANCELALLIANCE_TARGET",
		"INCREASEOPINION_CAUSE",
		"INCREASEOPINION_TARGET",
		"ADDTOSPHERE_CAUSE",
		"ADDTOSPHERE_TARGET",
		"REMOVEFROMSPHERE_CAUSE",
		"REMOVEFROMSPHERE_TARGET",
		"REMOVEFROMSPHERE_OTHER_TARGET",
		"INCREASERELATION_CAUSE",
		"INCREASERELATION_TARGET",
		"DECREASERELATION_CAUSE",
		"DECREASERELATION_TARGET",
		"INTERVENTION_SAME_SIDE",
		"INTERVENTION_OTHER_SIDE",
		"INTERVENTION",
		"BANKRUPTCY",
		"NEW_PARTY_AVAIL",
		"PARTY_UNAVAIL",
		"CB_DETECTED_CAUSE",
		"CB_DETECTED_TARGET",
		"CB_JUSTIFY_NO_LONGER_VALID",
		"JOIN_CRISIS_OFFER_CAUSE",
		"JOIN_CRISIS_OFFER_TARGET",
		"JOIN_CRISIS_OFFER_REJECT_CAUSE",
		"JOIN_CRISIS_OFFER_REJECT_TARGET",
		"JOIN_CRISIS_CAUSE",
		"JOIN_CRISIS_TARGET",
		"CRISIS_OFFER_CAUSE",
		"CRISIS_OFFER_TARGET",
		"CRISIS_OFFER_DECLINED_CAUSE",
		"CRISIS_OFFER_DECLINED_TARGET",
		"CRISIS_RESOLVED_CAUSE",
		"CRISIS_RESOLVED_TARGET",
		"CRISIS_STARTED",
		"CRISIS_BECAME_WAR_TARGET",
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
			std::string log_key = std::string(message_identifiers[i]) + "_MESSAGE";
			std::string description_key = std::string(message_identifiers[i]) + "_DESCRIPTION";

			s.message_m.log_text[i] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, log_key.c_str(), log_key.c_str() + log_key.size());
			s.message_m.description_text[i] = text_data::get_thread_safe_text_handle(s.gui_m.text_data_sequences, description_key.c_str(), description_key.c_str() + description_key.size());
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


	class nation_hyperlink {
	public:
		nations::country_tag target;

		nation_hyperlink(nations::country_tag t) : target(t) {}

		void button_function(ui::simple_button<nation_hyperlink>&, world_state& ws) {
			ws.w.diplomacy_w.show_diplomacy_window(ws.w.gui_m, target);
			graphics::map_goto(ws, target);
		}
	};

	template<typename ... types>
	struct t_replacements : public types ... {
		static_assert(sizeof...(types) != 0);

		std::tuple<types ...> members;

		t_replacements(t_replacements const& o) noexcept : members(o.members) {}
		t_replacements(t_replacements&& o) noexcept : members(std::move(o.members)) {}

		template<typename ... params, typename = std::enable_if_t<sizeof...(params) == sizeof...(types)> >
		t_replacements(params && ... p) : members(std::forward<params>(p)...) {}

		static int32_t count() {
			return int32_t(sizeof...(types));
		}
		template<size_t n>
		void populate_internal(world_state& ws, text_data::replacement* repl) const {
			if constexpr(n != 0)
				populate_internal<n - 1>(ws, repl);
			std::get<n>(members).fill(ws, *(repl + n));
		}
		void populate(world_state& ws, text_data::replacement* repl) const {
			populate_internal<sizeof...(types) - 1>(ws, repl);
		}
	};

	template<>
	struct t_replacements<> {
		static int32_t count() {
			return 0;
		}
		static void populate(world_state const& ws, text_data::replacement* repl) {
		}
	};

	template<text_data::value_type param>
	struct nation_replacement {
		nations::country_tag n;
		nation_replacement() {  }
		nation_replacement(nations::country_tag t) : n(t) {}

		void fill(world_state& ws, text_data::replacement& r) const {
			r = text_data::replacement{
				param,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(n)),
				[&ws, n = this->n](ui::tagged_gui_object b) {
					ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, n);
				}
			};
		}
	};

	template<text_data::value_type param>
	struct text_replacement {
		text_data::text_tag tag;
		text_replacement() {  }
		text_replacement(text_data::text_tag t) : tag(t) {}

		void fill(world_state const& ws, text_data::replacement& r) const {
			r = text_data::replacement{
				param,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, tag),
				[&ws](ui::tagged_gui_object b) { }
			};
		}
	};


	template<text_data::value_type param>
	struct fp_replacement {
		char16_t local_buffer[16] = {};
		fp_replacement() {  }
		fp_replacement(float v) {
			put_value_in_buffer(local_buffer, display_type::fp_one_place, v);
		}

		void fill(world_state const& ws, text_data::replacement& r) const {
			r = text_data::replacement { 
				param,
				vector_backed_string<char16_t>(local_buffer),
				[](ui::tagged_gui_object) { }
			};
		}
	};

	template<typename G, typename F>
	void handle_generic_message(world_state& ws, nations::country_tag goto_tag, message_display display, int32_t message_id, G const& replacements_maker, F& popup_text) {
		switch(display.max_setting) {
			case message_setting::popup_and_pause:
				ws.w.paused.store(true, std::memory_order_release);
				// fallthrough
			case message_setting::popup:
				submit_message(ws, goto_tag, [message_id, replacements_maker, popup_text](world_state& ws, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt) {
					const auto repl_count = replacements_maker.count();
					text_data::replacement* repl = (text_data::replacement*)_alloca(sizeof(text_data::replacement) * repl_count);
					std::uninitialized_default_construct_n(repl, repl_count);
					replacements_maker.populate(ws, repl);

					auto new_cursor = ui::add_multiline_text(
						ui::xy_pair{ 0,0 },
						ws.s.message_m.log_text[message_id],
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, repl_count);
					new_cursor = ui::advance_cursor_to_newline(new_cursor, ws.s.gui_m, fmt);
					lm.finish_current_line();
					popup_text(ws, new_cursor, fmt, box, lm);
					lm.finish_current_line();
				});
				// fallthrough
			case message_setting::log:
				submit_log_item(ws, message_category::diplomacy, [message_id, replacements_maker](world_state& ws, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt) {
					const auto repl_count = replacements_maker.count();
					text_data::replacement* repl = (text_data::replacement*)_alloca(sizeof(text_data::replacement) * repl_count);
					std::uninitialized_default_construct_n(repl, repl_count);
					replacements_maker.populate(ws, repl);
				
					ui::add_linear_text(
						ui::xy_pair{ 0,0 },
						ws.s.message_m.log_text[message_id],
						fmt, ws.s.gui_m, ws.w.gui_m, box, lm, repl, repl_count);
					lm.finish_current_line();
				});
				// fallthrough
			case message_setting::discard:
				// do nothing
				break;
		}
	}

	void increase_opinion(world_state& ws, nations::country_tag by, nations::country_tag target, int32_t new_level) {
		auto display = determine_message_display(ws, message_type::INCREASEOPINION_CAUSE, by);
		display += determine_message_display(ws, message_type::INCREASEOPINION_TARGET, target);

		handle_generic_message(
			ws, by,
			display,
			message_type::INCREASEOPINION_CAUSE,
			t_replacements<nation_replacement<text_data::value_type::nation>, nation_replacement<text_data::value_type::target>, text_replacement<text_data::value_type::opinion>>(
				by, target, ws.s.fixed_ui_text[scenario::fixed_ui::rel_hostile + new_level]
				));
	}

	void add_to_sphere(world_state& ws, nations::country_tag sphere_leader, nations::country_tag target) {
		auto display = determine_message_display(ws, message_type::ADDTOSPHERE_CAUSE, sphere_leader);
		display += determine_message_display(ws, message_type::ADDTOSPHERE_TARGET, target);

		handle_generic_message(
			ws, sphere_leader,
			display,
			message_type::ADDTOSPHERE_CAUSE,
			t_replacements<nation_replacement<text_data::value_type::nation>, nation_replacement<text_data::value_type::target>>(
				sphere_leader, target
				));
	}

	void remove_from_sphere(world_state& ws, nations::country_tag actor, nations::country_tag sphere_leader, nations::country_tag target) {
		auto display = determine_message_display(ws, message_type::REMOVEFROMSPHERE_CAUSE, actor);
		display += determine_message_display(ws, message_type::REMOVEFROMSPHERE_OTHER_TARGET, sphere_leader);
		display += determine_message_display(ws, message_type::REMOVEFROMSPHERE_TARGET, target);

		handle_generic_message(
			ws, actor,
			display,
			message_type::REMOVEFROMSPHERE_CAUSE,
			t_replacements<nation_replacement<text_data::value_type::nation>, nation_replacement<text_data::value_type::target>, nation_replacement<text_data::value_type::recipient>>(
				actor, sphere_leader, target
				));
	}

	void cb_detected(world_state& ws, nations::country_tag by, nations::country_tag target, military::cb_type_tag type, float infamy_gained) {
		auto display = determine_message_display(ws, message_type::CB_DETECTED_CAUSE, by);
		display += determine_message_display(ws, message_type::CB_DETECTED_TARGET, target);

		handle_generic_message(
			ws, by,
			display,
			message_type::CB_DETECTED_CAUSE,
			t_replacements<nation_replacement<text_data::value_type::nation>, nation_replacement<text_data::value_type::target>, fp_replacement<text_data::value_type::badboy>>(
				by, target, infamy_gained
				));
	}

	void cb_construction_invalid(world_state& ws, nations::country_tag by, nations::country_tag target, military::cb_type_tag type) {
		auto display = determine_message_display(ws, message_type::CB_JUSTIFY_NO_LONGER_VALID, by);
		handle_generic_message(
			ws, by,
			display,
			message_type::CB_JUSTIFY_NO_LONGER_VALID,
			t_replacements<nation_replacement<text_data::value_type::nation>, nation_replacement<text_data::value_type::target>, text_replacement<text_data::value_type::casus>>(
				by, target, ws.s.military_m.cb_types[type].name
				));
	}

	void acquired_cb(world_state& ws, nations::country_tag by, nations::country_tag target, military::cb_type_tag type) {
		auto display = determine_message_display(ws, message_type::GAINCB, by);
		display += determine_message_display(ws, message_type::GAINCB_TARGET, target);

		handle_generic_message(
			ws, by,
			display,
			message_type::GAINCB,
			t_replacements<nation_replacement<text_data::value_type::nation>, nation_replacement<text_data::value_type::target>, text_replacement<text_data::value_type::casus>>(
				by, target, ws.s.military_m.cb_types[type].name
				));
	}

	void lost_cb(world_state& ws, nations::country_tag by, nations::country_tag target, military::cb_type_tag type) {
		auto display = determine_message_display(ws, message_type::LOSECB, by);
		display += determine_message_display(ws, message_type::LOSECB_TARGET, target);

		handle_generic_message(
			ws, by,
			display,
			message_type::GAINCB,
			t_replacements<nation_replacement<text_data::value_type::nation>, nation_replacement<text_data::value_type::target>, text_replacement<text_data::value_type::casus>>(
				by, target, ws.s.military_m.cb_types[type].name
				));
	}

	void hidden_button::update(ui::simple_button<hidden_button>& self, world_state & ws) {
		ui::hide(*self.associated_object);
	}

	void new_technology(world_state& ws, nations::country_tag by, technologies::tech_tag type) {
		auto display = determine_message_display(ws, message_type::TECH, by);
		handle_generic_message(
			ws, by,
			display,
			message_type::TECH,
			t_replacements<nation_replacement<text_data::value_type::nation>, text_replacement<text_data::value_type::type>>(
				by, ws.s.technology_m.technologies_container[type].name
				));
	}
	void message_settings_button_group::on_select(world_state & ws, uint32_t i) {
		ws.w.message_settings_w.showing_messages = (i == 0);

		//auto& lb = ws.w.message_settings_w.win->get<CT_STRING("message_settings_items")>();
		//auto const old_position = lb.get_position();
		//lb.set_position(ws.w.message_settings_w.other_lb_position, ws.w.gui_m);
		//ws.w.message_settings_w.other_lb_position = old_position;

		ws.w.message_settings_w.win->get<CT_STRING("search_edit")>().clear();
		ws.w.message_settings_w.update_message_settings_window(ws.w.gui_m);
	}
	void message_settings_close_button::button_function(ui::simple_button<message_settings_close_button>&, world_state& ws) {
		if(ws.w.message_settings_w.setting_changed) {
			settings::save_settings(ws.s);
			ws.w.message_settings_w.setting_changed = false;
		}
		ws.w.message_settings_w.hide_message_settings_window(ws.w.gui_m);
	}
	void importance_button::button_function(ui::button<importance_button>& self, world_state & ws) {
		if(std::holds_alternative<group_setting>(value)) {
			auto const old_setting = ws.s.message_m.group_importance[int32_t(std::get<group_setting>(value))];
			auto const new_setting = (old_setting + 1) % 4;
			ws.s.message_m.group_importance[int32_t(std::get<group_setting>(value))] = int8_t(new_setting);

			self.set_frame(ws.w.gui_m, new_setting);
			ws.w.message_settings_w.setting_changed = true;
		} else if(std::holds_alternative<nations::country_tag>(value)) {
			auto const old_setting = ws.w.nation_s.nations.get<nation::player_importance>(std::get<nations::country_tag>(value));
			auto const new_setting = (old_setting + 1) % 4;
			ws.w.nation_s.nations.set<nation::player_importance>(std::get<nations::country_tag>(value), int8_t(new_setting));

			self.set_frame(ws.w.gui_m, new_setting);
		} 
	}
	void importance_button::button_function(ui::button<importance_button>& self, ui::rbutton_down m, world_state & ws) {
		if(std::holds_alternative<group_setting>(value)) {
			auto const old_setting = ws.s.message_m.group_importance[int32_t(std::get<group_setting>(value))];
			auto const new_setting = (old_setting + 3) % 4;
			ws.s.message_m.group_importance[int32_t(std::get<group_setting>(value))] = int8_t(new_setting);

			self.set_frame(ws.w.gui_m, new_setting);
			ws.w.message_settings_w.setting_changed = true;
		} else if(std::holds_alternative<nations::country_tag>(value)) {
			auto const old_setting = ws.w.nation_s.nations.get<nation::player_importance>(std::get<nations::country_tag>(value));
			auto const new_setting = (old_setting + 3) % 4;
			ws.w.nation_s.nations.set<nation::player_importance>(std::get<nations::country_tag>(value), int8_t(new_setting));

			self.set_frame(ws.w.gui_m, new_setting);
		}
	}
	ui::window_tag message_settings_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["openv2_message_setting_item"]);
	}
	message_settings_window::message_settings_window() : win(std::make_unique<message_settings_window_t>()) {}
	message_settings_window::~message_settings_window() {}
	void message_settings_window::hide_message_settings_window(ui::gui_manager & gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);

		showing_messages = true;
		setting_changed = false;
		other_lb_position = 0;
	}
	void message_settings_window::update_message_settings_window(ui::gui_manager & gui_m) {
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void message_settings_window::show_message_settings_window(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		win->get<CT_STRING("search_edit")>().clear();
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void message_settings_window::init_message_settings_window(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["openv2_message_settings"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	int32_t nation_importance(world_state const& ws, nations::country_tag n) {
		int32_t importance = 0;
		if(n == ws.w.local_player_nation)
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::self)]));
		if(n == ws.w.nation_s.nations.get<nation::sphere_leader>(ws.w.local_player_nation))
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::sphere_leader)]));
		if(n == ws.w.nation_s.nations.get<nation::overlord>(ws.w.local_player_nation))
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::overlord)]));
		if(nations::is_great_power(ws, n))
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::great_powers)]));
		if(contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::allies>(ws.w.local_player_nation), n))
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::allies)]));
		if(contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::vassals>(ws.w.local_player_nation), n))
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::vassals)]));
		if(contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::sphere_members>(ws.w.local_player_nation), n))
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::sphere_members)]));
		if(contains_item(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(ws.w.local_player_nation), n))
			importance = std::max(importance, int32_t(ws.s.message_m.group_importance[int32_t(group_setting::neighbors)]));
		return importance;
	}

	message_display determine_message_display(world_state const& ws, int32_t message_id, nations::country_tag n) {
		int32_t importance = nation_importance(ws, n);
		return message_display{ importance, ws.s.message_m.settings[message_id * 4 + importance] };
	}
	void search_box::on_edit(ui::edit_box<search_box>& self, world_state & ws) {
		ws.w.message_settings_w.update_message_settings_window(ws.w.gui_m);
	}
}
