#include "common\\common.h"
#include "events_gui.hpp"
#include "population\population_functions.h"
#include "provinces\provinces_io.h"
#include "triggers\trigger_gui.h"
#include "event_functions.h"

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
		
		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {

			text_data::replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.province_event_w.displayed_event.event_for, ws.w.province_event_w.displayed_event.event_from);

			ui::add_linear_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].title,
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm,
				rep_array,
				events::replacement_size);
		}

	}

	void province_event_body::update(
		ui::tagged_gui_object box,
		ui::line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {
		
		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {

			text_data::replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.province_event_w.displayed_event.event_for, ws.w.province_event_w.displayed_event.event_from);

			ui::add_multiline_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].body,
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm,
				rep_array,
				events::replacement_size);
		}

	}

	void hidden_button::update(ui::simple_button<hidden_button>& self, world_state& ws) {
		ui::hide(*self.associated_object);
	}

	class nation_hyperlink {
	public:
		nations::country_tag target;

		nation_hyperlink(nations::country_tag t) : target(t) {}

		void button_function(ui::simple_button<nation_hyperlink>&, world_state& ws) {
			ws.w.diplomacy_w.show_diplomacy_window(ws.w.gui_m, target);
		}
	};

	class province_hyperlink {
	public:
		provinces::province_tag target;

		province_hyperlink(provinces::province_tag t) : target(t) {}

		void button_function(ui::simple_button<province_hyperlink>&, world_state& ws) {
			ws.w.province_w.show_province_window(ws.w.gui_m, target);
		}
	};

	class state_hyperlink {
	public:
		nations::state_tag target;

		state_hyperlink(nations::state_tag t) : target(t) {}

		void button_function(ui::simple_button<province_hyperlink>&, world_state& ws) {
			ws.w.province_w.show_province_window(ws.w.gui_m, ws.w.nation_s.states.get<state::state_capital>(target));
		}
	};

	void populate_replacements(world_state& ws, text_data::replacement* replacement_array, target_variant event_target, target_variant event_from) {
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

		replacement_array[0] = text_data::replacement(
			text_data::value_type::capital, 
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.province_s.province_state_container.get<province_state::name>(target_cap)),
			[&ws, target_cap](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<province_hyperlink>>(ws, b, target_cap); }
		);
		replacement_array[1] = text_data::replacement(
			text_data::value_type::state,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.states.get<state::name>(target_state)),
			[&ws, target_state](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<state_hyperlink>>(ws, b, target_state); }
		);
		replacement_array[2] = text_data::replacement(
			text_data::value_type::statename,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.states.get<state::name>(target_state)),
			[&ws, target_state](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<state_hyperlink>>(ws, b, target_state); }
		);
		replacement_array[3] = text_data::replacement(
			text_data::value_type::fromcountry,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(from_nation)),
			[&ws, from_nation](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, from_nation); }
		);
		replacement_array[4] = text_data::replacement(
			text_data::value_type::fromcountry_adj,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::adjective>(from_nation)),
			[&ws, from_nation](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, from_nation); }
		);
		replacement_array[5] = text_data::replacement(
			text_data::value_type::culture_group_union,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target_union)),
			[](ui::tagged_gui_object b) { }
		);
		replacement_array[6] = text_data::replacement(
			text_data::value_type::country,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(target_nation)),
			[&ws, target_nation](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target_nation); }
		);
		replacement_array[7] = text_data::replacement(
			text_data::value_type::country_adj,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::adjective>(target_nation)),
			[&ws, target_nation](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, target_nation); }
		);
		replacement_array[8] = text_data::replacement(
			text_data::value_type::provinceculture,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[target_culture].name),
			[](ui::tagged_gui_object b) { }
		);
		replacement_array[9] = text_data::replacement(
			text_data::value_type::provincename,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.province_s.province_state_container.get<province_state::name>(target_province)),
			[&ws, target_province](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<province_hyperlink>>(ws, b, target_province); }
		);
		replacement_array[10] = text_data::replacement(
			text_data::value_type::cb_target_name,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(cb_target)),
			[&ws, cb_target](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, cb_target); }
		);
		replacement_array[11] = text_data::replacement(
			text_data::value_type::cb_target_name_adj,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::adjective>(cb_target)),
			[&ws, cb_target](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, cb_target); }
		);
		replacement_array[12] = text_data::replacement(
			text_data::value_type::crisisarea,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.states.get<state::name>(crisis_area)),
			[&ws, crisis_area](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<state_hyperlink>>(ws, b, crisis_area); }
		);
		replacement_array[13] = text_data::replacement(
			text_data::value_type::crisistaker,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(crisis_for)),
			[&ws, crisis_for](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, crisis_for); }
		);
		replacement_array[14] = text_data::replacement(
			text_data::value_type::crisistaker_adj,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::adjective>(crisis_for)),
			[&ws, crisis_for](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, crisis_for); }
		);
		replacement_array[15] = text_data::replacement(
			text_data::value_type::crisisattacker,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(ws.w.current_crisis.primary_attacker)),
			[&ws, n = ws.w.current_crisis.primary_attacker](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, n); }
		);
		replacement_array[16] = text_data::replacement(
			text_data::value_type::crisisdefender,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(ws.w.current_crisis.primary_defender)),
			[&ws, n = ws.w.current_crisis.primary_defender](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, n); }
		);
		replacement_array[17] = text_data::replacement(
			text_data::value_type::crisistarget,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::name>(crisis_defender)),
			[&ws, crisis_defender](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, crisis_defender); }
		);
		replacement_array[18] = text_data::replacement(
			text_data::value_type::crisistarget_adj,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.nation_s.nations.get<nation::adjective>(crisis_defender)),
			[&ws, crisis_defender](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<nation_hyperlink>>(ws, b, crisis_defender); }
		);

		auto const ymd = tag_to_date(ws.w.current_date).year_month_day();

		replacement_array[19] = text_data::replacement(
			text_data::value_type::month,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::month_1 + ymd.month - 1]),
			[](ui::tagged_gui_object b) { }
		);

		static char16_t year_buffer[16];
		put_value_in_buffer(year_buffer, display_type::exact_integer, int32_t(ymd.year));

		replacement_array[20] = text_data::replacement(
			text_data::value_type::year,
			vector_backed_string<char16_t>(year_buffer),
			[](ui::tagged_gui_object b) {}
		);
	}

	void province_event_location::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.province_s.province_state_container.get<province_state::name>(std::get<provinces::province_tag>(ws.w.province_event_w.displayed_event.event_for))),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm,
			[&ws](ui::tagged_gui_object b) { ui::attach_dynamic_behavior<ui::simple_button<province_hyperlink>>(ws, b, std::get<provinces::province_tag>(ws.w.province_event_w.displayed_event.event_for)); }
			);
		lm.finish_current_line();
	}
	ui::window_tag province_options_listbox::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_province_event_option"]);
	}

	void event_auto_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(
			ui::xy_pair{0, 0},
			ws.s.fixed_ui_text[scenario::fixed_ui::automate_option],
			ui::tooltip_text_format,
			ws.s.gui_m,
			ws.w.gui_m,
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
			ws.w.province_event_w.displayed_event.generator,
			to_trigger_param(ws.w.province_event_w.displayed_event.event_for),
			to_trigger_param(ws.w.province_event_w.displayed_event.event_from),
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

			text_data::replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.nation_event_w.displayed_event.event_for, ws.w.nation_event_w.displayed_event.event_from);

			ui::add_multiline_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].body,
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm,
				rep_array,
				events::replacement_size);
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

		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {

			text_data::replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.nation_event_w.displayed_event.event_for, ws.w.nation_event_w.displayed_event.event_from);

			ui::add_linear_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].title,
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm,
				rep_array,
				events::replacement_size);
		}
	}

	ui::window_tag major_options_listbox::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_nation_event_option"]);
	}

	void major_event_title::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {

		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {

			text_data::replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.major_event_w.displayed_event.event_for, ws.w.major_event_w.displayed_event.event_from);

			ui::add_linear_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].title,
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm,
				rep_array,
				events::replacement_size);
		}
	}

	void major_event_body::update(
		ui::tagged_gui_object box,
		ui::line_manager& lm,
		ui::text_format& fmt,
		world_state& ws) {

		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {

			text_data::replacement rep_array[events::replacement_size];
			populate_replacements(ws, rep_array, ws.w.major_event_w.displayed_event.event_for, ws.w.major_event_w.displayed_event.event_from);

			ui::add_multiline_text(
				ui::xy_pair{ 0, 0 },
				ws.s.event_m.event_container[e].body,
				fmt,
				ws.s.gui_m,
				ws.w.gui_m,
				box,
				lm,
				rep_array,
				events::replacement_size);
		}
	}

	void major_event_flag::update(ui::masked_flag<major_event_flag>& self, world_state& ws) {
		self.set_displayed_flag(ws, ws.w.local_player_nation);
	}
}
