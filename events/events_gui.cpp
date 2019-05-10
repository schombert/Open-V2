#include "common\\common.h"
#include "events_gui.hpp"
#include "population\population_functions.h"
#include "provinces\provinces_io.h"
#include "triggers\trigger_gui.h"
#include "event_functions.h"
#include "modifiers\modifiers_gui.h"
#include "modifiers\modifier_functions.h"

namespace events {

	province_event_window::province_event_window() : win(std::make_unique<province_event_window_t>()) {}
	province_event_window::~province_event_window() {}

	nation_event_window::nation_event_window() : win(std::make_unique<nation_event_window_t>()) {}
	nation_event_window::~nation_event_window() {}

	major_event_window::major_event_window() : win(std::make_unique<major_event_window_t>()) {}
	major_event_window::~major_event_window() {}

	void province_event_window::hide_province_event_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
		gui_m.hide_tooltip();
	}
	void province_event_window::show_province_event_window(ui::gui_manager& gui_m, pending_event e) {
		displayed_event = e;
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void province_event_window::init_province_event_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["Event_Province_Window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void nation_event_window::hide_nation_event_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
		gui_m.hide_tooltip();
	}
	void nation_event_window::show_nation_event_window(ui::gui_manager& gui_m, pending_event e) {
		displayed_event = e;
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void nation_event_window::init_nation_event_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["Event_Country_Window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void major_event_window::hide_major_event_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
		gui_m.hide_tooltip();
	}
	void major_event_window::show_major_event_window(ui::gui_manager& gui_m, pending_event e) {
		displayed_event = e;
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void major_event_window::init_major_event_window(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["Event_Major_Window"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void hidden_image::update(ui::dynamic_icon<hidden_image>& self, world_state& ws) {
		ui::hide(*self.associated_object);
	}

	void province_event_title::update(
		ui::tagged_gui_object box,
		ui::text_box_line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {
		
		ui::line_manager new_lm { text_data::alignment::center, 260 };

		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {

			text_data::text_replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.province_event_w.displayed_event.event_for, ws.w.province_event_w.displayed_event.event_from, text_data::text_color::dark_blue);

			ui::add_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].title,
				ui::text_format{ ui::text_color::black, fmt.font_handle, fmt.font_size},
				ws,
				box,
				new_lm,
				rep_array,
				events::replacement_size);

			new_lm.finish_current_line();
		}

	}

	void province_event_body::update(
		ui::tagged_gui_object box,
		ui::line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {
		
		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {

			text_data::text_replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.province_event_w.displayed_event.event_for, ws.w.province_event_w.displayed_event.event_from, text_data::text_color::dark_blue);

			ui::add_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].body,
				ui::text_format{ fmt.color, graphics::font_tag(1), fmt.font_size },
				ws,
				box,
				lm,
				rep_array,
				events::replacement_size);

			lm.finish_current_line();
		}

	}

	void hidden_button::update(ui::simple_button<hidden_button>& self, world_state& ws) {
		ui::hide(*self.associated_object);
	}

	inline auto goto_nation_function(nations::country_tag target) {
		return [target](world_state& ws) {
			ws.w.diplomacy_w.show_diplomacy_window(ws.w.gui_m, target);
			graphics::map_goto(ws, target);
		};
	}

	inline auto goto_prov_function(provinces::province_tag target) {
		return [target](world_state& ws) {
			ws.w.province_w.show_province_window(ws.w.gui_m, target);
			graphics::map_goto(ws, target);
		};
	}

	inline auto goto_state_function(nations::state_tag target) {
		return [target](world_state& ws) {
			ws.w.province_w.show_province_window(ws.w.gui_m, ws.w.nation_s.states.get<state::state_capital>(target));
			graphics::map_goto(ws, target);
		};
	}

	void populate_replacements(world_state& ws, text_data::text_replacement* replacement_array, target_variant event_target, target_variant event_from, text_data::text_color link_color) {
		auto const target_nation = [event_target, &ws]() {
			if(std::holds_alternative<nations::country_tag>(event_target))
				return std::get<nations::country_tag>(event_target);
			else if(std::holds_alternative<nations::state_tag>(event_target))
				return ws.w.nation_s.states.get<state::owner>(std::get<nations::state_tag>(event_target));
			else if(std::holds_alternative<provinces::province_tag>(event_target))
				return ws.w.province_s.province_state_container.get<province_state::owner>(std::get<provinces::province_tag>(event_target));
			else if(std::holds_alternative<population::pop_tag>(event_target))
				return population::get_pop_owner(ws, std::get<population::pop_tag>(event_target));
			else
				return nations::country_tag();
		}();
		auto const target_cap = ws.w.nation_s.nations.get<nation::current_capital>(target_nation);
		auto const target_state = [event_target, &ws]() {
			if(std::holds_alternative<nations::country_tag>(event_target))
				return nations::state_tag();
			else if(std::holds_alternative<nations::state_tag>(event_target))
				return std::get<nations::state_tag>(event_target);
			else if(std::holds_alternative<provinces::province_tag>(event_target))
				return ws.w.province_s.province_state_container.get<province_state::state_instance>(std::get<provinces::province_tag>(event_target));
			else if(std::holds_alternative<population::pop_tag>(event_target))
				return ws.w.province_s.province_state_container.get<province_state::state_instance>(
					ws.w.population_s.pops.get<pop::location>(std::get<population::pop_tag>(event_target)));
			else
				return nations::state_tag();
		}();
		auto const target_province = [event_target, &ws]() {
			if(std::holds_alternative<nations::country_tag>(event_target))
				return provinces::province_tag();
			else if(std::holds_alternative<nations::state_tag>(event_target))
				return ws.w.nation_s.states.get<state::state_capital>(std::get<nations::state_tag>(event_target));
			else if(std::holds_alternative<provinces::province_tag>(event_target))
				return std::get<provinces::province_tag>(event_target);
			else if(std::holds_alternative<population::pop_tag>(event_target))
				return ws.w.population_s.pops.get<pop::location>(std::get<population::pop_tag>(event_target));
			else
				return provinces::province_tag();
		}();
		auto const from_nation = [event_from, &ws]() {
			if(std::holds_alternative<nations::country_tag>(event_from))
				return std::get<nations::country_tag>(event_from);
			else if(std::holds_alternative<nations::state_tag>(event_from))
				return ws.w.nation_s.states.get<state::owner>(std::get<nations::state_tag>(event_from));
			else if(std::holds_alternative<provinces::province_tag>(event_from))
				return ws.w.province_s.province_state_container.get<province_state::owner>(std::get<provinces::province_tag>(event_from));
			else if(std::holds_alternative<population::pop_tag>(event_from))
				return population::get_pop_owner(ws, std::get<population::pop_tag>(event_from));
			else
				return nations::country_tag();
		}();
		auto const target_union_tag = ws.s.culture_m.cultures_to_tags[ws.w.nation_s.nations.get<nation::primary_culture>(target_nation)];
		auto const target_union = ws.w.culture_s.tags_to_holders[target_union_tag];
		auto const target_culture = ws.w.province_s.province_state_container.get<province_state::dominant_culture>(target_province);
		auto const cb_target = ws.w.nation_s.nations.get<nation::cb_construction_target>(target_nation);
		auto const crisis_area = ws.w.current_crisis.state;
		auto const crisis_for = ws.w.current_crisis.on_behalf_of;
		auto const crisis_defender = ws.w.current_crisis.target;

		replacement_array[0] = text_data::text_replacement(
			text_data::value_type::capital, 
			ws.w.province_s.province_state_container.get<province_state::name>(target_cap),
			goto_prov_function(target_cap), link_color
		);
		replacement_array[1] = text_data::text_replacement(
			text_data::value_type::state,
			ws.w.nation_s.states.get<state::name>(target_state),
			goto_state_function(target_state), link_color
		);
		replacement_array[2] = text_data::text_replacement(
			text_data::value_type::statename,
			ws.w.nation_s.states.get<state::name>(target_state),
			goto_state_function(target_state), link_color
		);
		replacement_array[3] = text_data::text_replacement(
			text_data::value_type::fromcountry,
			ws.w.nation_s.nations.get<nation::name>(from_nation),
			goto_nation_function(from_nation), link_color
		);
		replacement_array[4] = text_data::text_replacement(
			text_data::value_type::fromcountry_adj,
			ws.w.nation_s.nations.get<nation::adjective>(from_nation),
			goto_nation_function(from_nation), link_color
		);
		replacement_array[5] = text_data::text_replacement(
			text_data::value_type::culture_group_union,
			ws.w.nation_s.nations.get<nation::name>(target_union)
		);
		replacement_array[6] = text_data::text_replacement(
			text_data::value_type::country,
			ws.w.nation_s.nations.get<nation::name>(target_nation),
			goto_nation_function(target_nation), link_color
		);
		replacement_array[7] = text_data::text_replacement(
			text_data::value_type::country_adj,
			ws.w.nation_s.nations.get<nation::adjective>(target_nation),
			goto_nation_function(target_nation), link_color
		);
		replacement_array[8] = text_data::text_replacement(
			text_data::value_type::provinceculture,
			ws.s.culture_m.culture_container[target_culture].name
		);
		replacement_array[9] = text_data::text_replacement(
			text_data::value_type::provincename,
			ws.w.province_s.province_state_container.get<province_state::name>(target_province),
			goto_prov_function(target_province), link_color
		);
		replacement_array[10] = text_data::text_replacement(
			text_data::value_type::cb_target_name,
			ws.w.nation_s.nations.get<nation::name>(cb_target),
			goto_nation_function(cb_target), link_color
		);
		replacement_array[11] = text_data::text_replacement(
			text_data::value_type::cb_target_name_adj,
			ws.w.nation_s.nations.get<nation::adjective>(cb_target),
			goto_nation_function(cb_target), link_color
		);
		replacement_array[12] = text_data::text_replacement(
			text_data::value_type::crisisarea,
			ws.w.nation_s.states.get<state::name>(crisis_area),
			goto_state_function(crisis_area), link_color
		);
		replacement_array[13] = text_data::text_replacement(
			text_data::value_type::crisistaker,
			ws.w.nation_s.nations.get<nation::name>(crisis_for),
			goto_nation_function(crisis_for), link_color
		);
		replacement_array[14] = text_data::text_replacement(
			text_data::value_type::crisistaker_adj,
			ws.w.nation_s.nations.get<nation::adjective>(crisis_for),
			goto_nation_function(crisis_for), link_color
		);
		replacement_array[15] = text_data::text_replacement(
			text_data::value_type::crisisattacker,
			ws.w.nation_s.nations.get<nation::name>(ws.w.current_crisis.primary_attacker),
			goto_nation_function(ws.w.current_crisis.primary_attacker), link_color
		);
		replacement_array[16] = text_data::text_replacement(
			text_data::value_type::crisisdefender,
			ws.w.nation_s.nations.get<nation::name>(ws.w.current_crisis.primary_defender),
			goto_nation_function(ws.w.current_crisis.primary_defender), link_color
		);
		replacement_array[17] = text_data::text_replacement(
			text_data::value_type::crisistarget,
			ws.w.nation_s.nations.get<nation::name>(crisis_defender),
			goto_nation_function(crisis_defender), link_color
		);
		replacement_array[18] = text_data::text_replacement(
			text_data::value_type::crisistarget_adj,
			ws.w.nation_s.nations.get<nation::adjective>(crisis_defender),
			goto_nation_function(crisis_defender), link_color
		);

		auto const ymd = tag_to_date(ws.w.current_date).year_month_day();

		replacement_array[19] = text_data::text_replacement(
			text_data::value_type::month,
			ws.s.fixed_ui_text[scenario::fixed_ui::month_1 + ymd.month - 1]
		);
		replacement_array[20] = text_data::text_replacement(
			text_data::value_type::year,
			text_data::exact_integer{ int32_t(ymd.year) }
		);
	}

	void province_event_location::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, 
			ws.w.province_s.province_state_container.get<province_state::name>(std::get<provinces::province_tag>(ws.w.province_event_w.displayed_event.event_for)),
			ui::text_format{ fmt.color, graphics::font_tag(1), fmt.font_size }, 
			ws, box, lm, nullptr, 0,
			ui::behavior_manager{ text_data::text_color::dark_blue, goto_prov_function(std::get<provinces::province_tag>(ws.w.province_event_w.displayed_event.event_for)) });
	}
	ui::window_tag province_options_listbox::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_province_event_option"]);
	}

	void event_auto_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_text(
			ui::xy_pair{0, 0},
			ws.s.fixed_ui_text[scenario::fixed_ui::automate_option],
			ui::tooltip_text_format,
			ws,
			tw);
	}

	inline triggers::const_parameter to_trigger_param(target_variant target) {
		return std::visit([](auto i) {
			if constexpr(std::is_same_v<decltype(i), std::monostate>)
				return triggers::const_parameter();
			else
				return triggers::const_parameter(i);
		}, target);
	}

	void event_auto_button::button_function(ui::simple_button<event_auto_button>& self, world_state & ws) {
		ws.w.local_player_data.saved_event_choices[e.e] = int8_t(value + 1);

		ws.w.pending_commands.add<commands::execute_event>(
			e.generator,
			to_trigger_param(e.event_for),
			to_trigger_param(e.event_from),
			e.e,
			int8_t(value));
		ws.w.province_event_w.hide_province_event_window(ws.w.gui_m);
		ws.w.nation_event_w.hide_nation_event_window(ws.w.gui_m);
		ws.w.major_event_w.hide_major_event_window(ws.w.gui_m);

		events::make_player_choice(ws, value);
	}

	void event_option_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		
			ui::xy_pair cursor{ 0,0 };
			ui::unlimited_line_manager lm;
			if(auto const immediate = ws.s.event_m.event_container[e.e].immediate_effect; immediate) {
				cursor = triggers::make_effect_description(
					ws,
					tw,
					cursor,
					lm,
					ui::tooltip_text_format,
					ws.s.trigger_m.effect_data.data() + to_index(immediate),
					to_trigger_param(e.event_for),
					to_trigger_param(e.event_for),
					to_trigger_param(e.event_from),
					e.generator);
			}
			if(auto const effect = ws.s.event_m.event_container[e.e].options[value].effect; effect) {
				cursor = triggers::make_effect_description(
					ws,
					tw,
					cursor,
					lm,
					ui::tooltip_text_format,
					ws.s.trigger_m.effect_data.data() + to_index(effect),
					to_trigger_param(e.event_for),
					to_trigger_param(e.event_for),
					to_trigger_param(e.event_from),
					e.generator);
			}
		
	}

	void event_option_button::button_function(ui::button<event_option_button>& self, world_state& ws) {
		ws.w.pending_commands.add<commands::execute_event>(
			e.generator,
			to_trigger_param(e.event_for),
			to_trigger_param(e.event_from),
			e.e,
			int8_t(value));
		ws.w.province_event_w.hide_province_event_window(ws.w.gui_m);
		ws.w.nation_event_w.hide_nation_event_window(ws.w.gui_m);
		ws.w.major_event_w.hide_major_event_window(ws.w.gui_m);

		events::make_player_choice(ws, value);
	}

	void nation_event_body::update(
		ui::tagged_gui_object box,
		ui::line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {
		
		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {

			text_data::text_replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.nation_event_w.displayed_event.event_for, ws.w.nation_event_w.displayed_event.event_from, text_data::text_color::dark_blue);

			ui::add_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].body,
				ui::text_format{ fmt.color, graphics::font_tag(1), fmt.font_size },
				ws,
				box,
				lm,
				rep_array,
				events::replacement_size);
			lm.finish_current_line();
		}
	}

	void nation_event_image::on_create(ui::dynamic_icon<nation_event_image>& self, world_state & ws) {
		auto gi = ws.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(self.associated_object->type_dependant_handle.load(std::memory_order_acquire)));
		if(gi)
			default_image = gi->t;
	}

	ui::window_tag nation_options_listbox::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_nation_event_option"]);
	}

	void nation_event_title::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::line_manager new_lm{ text_data::alignment::center, 475 };

		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {

			text_data::text_replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.nation_event_w.displayed_event.event_for, ws.w.nation_event_w.displayed_event.event_from, text_data::text_color::dark_blue);

			ui::add_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].title,
				ui::text_format{ ui::text_color::black, fmt.font_handle, fmt.font_size },
				ws,
				box,
				new_lm,
				rep_array,
				events::replacement_size);

			new_lm.finish_current_line();
		}
	}

	ui::window_tag major_options_listbox::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_nation_event_option"]);
	}

	void major_event_title::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {

		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {

			text_data::text_replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.major_event_w.displayed_event.event_for, ws.w.major_event_w.displayed_event.event_from, text_data::text_color::dark_blue);

			ui::add_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].title,
				ui::text_format{ ui::text_color::black, fmt.font_handle, fmt.font_size },
				ws,
				box,
				lm,
				rep_array,
				events::replacement_size);

			lm.finish_current_line();
		}
	}

	void major_event_body::update(
		ui::tagged_gui_object box,
		ui::line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {

		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {

			text_data::text_replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.major_event_w.displayed_event.event_for, ws.w.major_event_w.displayed_event.event_from, text_data::text_color::dark_blue);

			ui::add_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].body,
				ui::text_format{ fmt.color, graphics::font_tag(1), fmt.font_size },
				ws,
				box,
				lm,
				rep_array,
				events::replacement_size);

			lm.finish_current_line();
		}
	}

	void major_event_flag::update(ui::masked_flag<major_event_flag>& self, world_state& ws) {
		self.set_displayed_flag(ws, ws.w.local_player_nation);
	}
	void province_event_info::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {
			ui::unlimited_line_manager lm;
			triggers::make_trigger_description(
				ws,
				tw,
				ui::xy_pair{0, 0},
				lm,
				ui::tooltip_text_format,
				ws.s.trigger_m.trigger_data.data() + to_index(ws.s.event_m.event_container[e].trigger),
				to_trigger_param(ws.w.province_event_w.displayed_event.event_for),
				to_trigger_param(ws.w.province_event_w.displayed_event.event_for),
				to_trigger_param(ws.w.province_event_w.displayed_event.event_from));
		}
	}
	void province_event_odds::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {
			if(auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen; chance_tag) {
				ui::unlimited_line_manager lm;
				ui::xy_pair cursor{ 0,0 };
				cursor = ui::add_text(
					cursor,
					ws.s.fixed_ui_text[scenario::fixed_ui::monthly_chance],
					ui::tooltip_text_format,
					ws,
					tw,
					lm);

				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);


				auto const value = modifiers::test_multiplicative_factor(
					chance_tag,
					ws,
					to_trigger_param(ws.w.province_event_w.displayed_event.event_for),
					to_trigger_param(ws.w.province_event_w.displayed_event.event_from));

				auto base_chance = 2.0f / ve::max(value, 2.0f);
				auto neg_chance = 1.0f - base_chance;
				auto neg_chance_2 = neg_chance * neg_chance;
				auto neg_chance_4 = neg_chance_2 * neg_chance_2;
				auto neg_chance_8 = neg_chance_4 * neg_chance_4;
				auto neg_chance_16 = neg_chance_8 * neg_chance_8;

				const float chance = 1.0f - neg_chance_16;


				cursor = ui::add_text( cursor, text_data::percent_fp_one_place{ chance }, ui::tooltip_text_format, ws, tw, lm);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				modifiers::make_multiplicative_factor_explanation_in_days(
					ws.s.modifiers_m.factor_modifiers[chance_tag],
					ws,
					tw,
					cursor,
					lm,
					ui::tooltip_text_format,
					to_trigger_param(ws.w.province_event_w.displayed_event.event_for),
					to_trigger_param(ws.w.province_event_w.displayed_event.event_from)
				);
			}
		}
	}
	void nation_event_info::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {
			ui::unlimited_line_manager lm;
			triggers::make_trigger_description(
				ws,
				tw,
				ui::xy_pair{ 0, 0 },
				lm,
				ui::tooltip_text_format,
				ws.s.trigger_m.trigger_data.data() + to_index(ws.s.event_m.event_container[e].trigger),
				to_trigger_param(ws.w.nation_event_w.displayed_event.event_for),
				to_trigger_param(ws.w.nation_event_w.displayed_event.event_for),
				to_trigger_param(ws.w.nation_event_w.displayed_event.event_from));
		}
	}
	void nation_event_odds::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {
			if(auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen; chance_tag) {
				ui::unlimited_line_manager lm;
				ui::xy_pair cursor{ 0,0 };
				cursor = ui::add_text(
					cursor,
					ws.s.fixed_ui_text[scenario::fixed_ui::monthly_chance],
					ui::tooltip_text_format,
					ws,
					tw,
					lm);

				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);


				auto const value = modifiers::test_multiplicative_factor(
					chance_tag,
					ws,
					to_trigger_param(ws.w.nation_event_w.displayed_event.event_for),
					to_trigger_param(ws.w.nation_event_w.displayed_event.event_from));

				auto base_chance = 1.0f / ve::max(value, 2.0f);
				auto neg_chance = 1.0f - base_chance;
				auto neg_chance_2 = neg_chance * neg_chance;
				auto neg_chance_4 = neg_chance_2 * neg_chance_2;
				auto neg_chance_8 = neg_chance_4 * neg_chance_4;
				auto neg_chance_16 = neg_chance_8 * neg_chance_8;

				const float chance = 1.0f - neg_chance_16;

				cursor = ui::add_text(cursor, text_data::percent_fp_one_place{ chance }, ui::tooltip_text_format, ws, tw, lm);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				modifiers::make_multiplicative_factor_explanation_in_days(
					ws.s.modifiers_m.factor_modifiers[chance_tag],
					ws,
					tw,
					cursor,
					lm,
					ui::tooltip_text_format,
					to_trigger_param(ws.w.nation_event_w.displayed_event.event_for),
					to_trigger_param(ws.w.nation_event_w.displayed_event.event_from)
				);
			}
		}
	}
	void major_event_info::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {
			ui::unlimited_line_manager lm;
			triggers::make_trigger_description(
				ws,
				tw,
				ui::xy_pair{ 0, 0 },
				lm,
				ui::tooltip_text_format,
				ws.s.trigger_m.trigger_data.data() + to_index(ws.s.event_m.event_container[e].trigger),
				to_trigger_param(ws.w.major_event_w.displayed_event.event_for),
				to_trigger_param(ws.w.major_event_w.displayed_event.event_for),
				to_trigger_param(ws.w.major_event_w.displayed_event.event_from));
		}
	}
	void major_event_odds::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {
			if(auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen; chance_tag) {
				ui::unlimited_line_manager lm;
				ui::xy_pair cursor{ 0,0 };
				cursor = ui::add_text(
					cursor,
					ws.s.fixed_ui_text[scenario::fixed_ui::monthly_chance],
					ui::tooltip_text_format,
					ws,
					tw,
					lm);

				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);


				auto const value = modifiers::test_multiplicative_factor(
					chance_tag,
					ws,
					to_trigger_param(ws.w.major_event_w.displayed_event.event_for),
					to_trigger_param(ws.w.major_event_w.displayed_event.event_from));

				auto base_chance = 1.0f / ve::max(value, 2.0f);
				auto neg_chance = 1.0f - base_chance;
				auto neg_chance_2 = neg_chance * neg_chance;
				auto neg_chance_4 = neg_chance_2 * neg_chance_2;
				auto neg_chance_8 = neg_chance_4 * neg_chance_4;
				auto neg_chance_16 = neg_chance_8 * neg_chance_8;

				const float chance = 1.0f - neg_chance_16;

				cursor = ui::add_text(cursor, text_data::percent_fp_one_place{ chance }, ui::tooltip_text_format, ws, tw, lm);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				modifiers::make_multiplicative_factor_explanation_in_days(
					ws.s.modifiers_m.factor_modifiers[chance_tag],
					ws,
					tw,
					cursor,
					lm,
					ui::tooltip_text_format,
					to_trigger_param(ws.w.major_event_w.displayed_event.event_for),
					to_trigger_param(ws.w.major_event_w.displayed_event.event_from)
				);
			}
		}
	}
}
