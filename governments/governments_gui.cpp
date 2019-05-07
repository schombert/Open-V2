#include "common\\common.h"
#include "governments_gui.hpp"
#include "world_state\\world_state.h"
#include "modifiers\\modifiers_gui.h"
#include "governments_functions.h"
#include "triggers\\trigger_gui.h"
#include "commands\commands.h"

namespace governments {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ui::hide(*(ws.w.government_w.win->choose_window.associated_object));
		ws.w.government_w.hide_government_window(ws.w.gui_m);
	}
	void government_type_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto gov = ws.w.nation_s.nations.get<nation::current_government>(player); is_valid_index(gov)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.governments_m.governments_container[gov].name, fmt, ws.s.gui_m, ws.w.gui_m,
					box, lm);
				lm.finish_current_line();
			}
		}
	}
	void national_value_icon::update(ui::dynamic_icon<national_value_icon>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto nv = ws.w.nation_s.nations.get<nation::national_value>(player); is_valid_index(nv)) {
				self.set_frame(ws.w.gui_m, ws.s.modifiers_m.national_modifiers[nv].icon);
			}
		}
	}
	void national_value_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto nv = ws.w.nation_s.nations.get<nation::national_value>(player); is_valid_index(nv)) {
				ui::unlimited_line_manager lm;
				modifiers::make_national_modifier_text(ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format, nv);
			}
		}
	}
	void government_description_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto gov = ws.w.nation_s.nations.get<nation::current_government>(player); is_valid_index(gov)) {
				lm.set_border_y(0);
				ui::xy_pair cursor{ 0,0 };
				if(ws.s.governments_m.governments_container[gov].election) {
					auto next_election = governments::next_election_start_date(ws, player);

					cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::next_election], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);

					auto ymd = tag_to_date(next_election).year_month_day();

					char16_t local_buf[16];

					/*cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::month_1 + ymd.month - 1], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, fmt);
					
					put_value_in_buffer(local_buf, display_type::integer, int32_t(ymd.day));
					cursor = ui::text_chunk_to_instances(
						ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
						box, cursor, fmt, lm
					);

					cursor = ui::text_chunk_to_instances(
						ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u", "),
						box, cursor, fmt, lm
					);*/

					put_value_in_buffer(local_buf, display_type::integer, int32_t(ymd.year));
					cursor = ui::text_chunk_to_instances(
						ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
						box, cursor, fmt, lm
					);

					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				if(ws.s.governments_m.governments_container[gov].appoint_ruling_party) {
					cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::can_appoint_ruling_party], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					lm.finish_current_line();
				}
			}
		}
	}
	void plurality_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::percent, ws.w.nation_s.nations.get<nation::plurality>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}
	void revanchism_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::percent, ws.w.nation_s.nations.get<nation::revanchism>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}
	void plurality_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::plurality], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void revanchism_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::revanchism], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}
	void social_reform_possible_icon::update(ui::dynamic_icon<social_reform_possible_icon>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32); // yes = 0
	}
	void political_reform_possible_icon::update(ui::dynamic_icon<political_reform_possible_icon>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, 1ui32); // yes = 0
	}

	ui::window_tag modifier_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["country_modifier_item"]);
	}

	ui::window_tag movements_listbox::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["movement_entry"]);
	}

	ui::window_tag rebels_listbox::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["rebel_window"]);
	}

	ui::window_tag reforms_listbox::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["reform_option_window"]);
	}
	ui::window_tag release_nations_listbox::element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["vassal_nation"]);
	}

	void modifier_button::update(ui::simple_button<modifier_button>& ico, world_state & ws) {
		if(icon != 0ui32)
			ico.set_frame(ws.w.gui_m, icon + 1ui32);
		else
			ico.set_frame(ws.w.gui_m, 4ui32);
	}

	void modifier_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		if(is_valid_index(mod)) {
			ui::xy_pair cursor{ 0,0 };
			ui::unlimited_line_manager lm;
			cursor = modifiers::make_national_modifier_text(ws, tw, cursor, lm, ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 }, mod);
			if(is_valid_index(expiration)) {
				char16_t formatted_date[64];
				u16_format_date(formatted_date, expiration);
				text_data::replacement date_rep(text_data::value_type::date, vector_backed_string<char16_t>(formatted_date), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::expires_on], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &date_rep, 1ui32);
			}
		}
	}
	void governments_tab_button_group::on_select(world_state & ws, uint32_t i) {
		if(i == 0)
			ws.w.government_w.show_government_reforms_tab(ws.w.gui_m);
		else if(i == 1)
			ws.w.government_w.show_government_movements_tab(ws.w.gui_m);
		else if(i == 2)
			ws.w.government_w.show_government_decisions_tab(ws.w.gui_m);
		else if(i == 3)
			ws.w.government_w.show_government_release_nations_tab(ws.w.gui_m);
	}
	void sort_by_voters_button::button_function(ui::simple_button<sort_by_voters_button>&, world_state &) {}
	void sort_by_people_button::button_function(ui::simple_button<sort_by_people_button>&, world_state &) {}
	void sort_by_issue_name_button::button_function(ui::simple_button<sort_by_issue_name_button>&, world_state &) {}

	void hold_election_button::button_function(ui::simple_button<hold_election_button>&, world_state &) {}

	void hold_election_button::update(ui::simple_button<hold_election_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto gov = ws.w.nation_s.nations.get<nation::current_government>(player); is_valid_index(gov)) {
				if(ws.w.nation_s.nations.get<nation::is_holding_election>(player) || !(ws.s.governments_m.governments_container[gov].election)) {
					is_enabled = false;
					self.set_enabled(false);
				} else {
					is_enabled = true;
					self.set_enabled(true);
				}
			}
		}
	}
	void hold_election_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto gov = ws.w.nation_s.nations.get<nation::current_government>(player); is_valid_index(gov)) {
				if(ws.w.nation_s.nations.get<nation::is_holding_election>(player)) {

					char16_t formatted_date[64];
					u16_format_date(formatted_date, election_end_date(ws, player));
					text_data::replacement date_rep(text_data::value_type::date, vector_backed_string<char16_t>(formatted_date), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
					ui::unlimited_line_manager lm;
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::already_holding_election], ui::tooltip_text_format,
						ws.s.gui_m, ws.w.gui_m, tw, lm, &date_rep, 1ui32);
				} else if(!(ws.s.governments_m.governments_container[gov].election)) {

					text_data::replacement gov_name_replacement(
						text_data::value_type::government,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.governments_m.governments_container[gov].name),
						[](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
					ui::unlimited_line_manager lm;
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::already_holding_election], ui::tooltip_text_format,
						ws.s.gui_m, ws.w.gui_m, tw, lm, &gov_name_replacement, 1ui32);
				}
			}
		}
	}
	void upper_house_pie_chart::update(ui::piechart<upper_house_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto upper_house = ws.w.nation_s.upper_house.get_row(id);
				for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
					ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
					if(upper_house[this_tag] != 0) {
						pie.add_entry(ws.w.gui_m,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.ideologies_m.ideology_container[this_tag].name),
							float(upper_house[this_tag]) / 100.0f,
							ws.s.ideologies_m.ideology_container[this_tag].color);
					}
				}
			}
		}
	}
	void voters_ideologies_pie_chart::update(ui::piechart<voters_ideologies_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto const national_rules = ws.w.nation_s.nations.get<nation::current_rules>(player);
			voting_type vtype =
				(national_rules & issues::rules::voting_system_mask) == issues::rules::largest_share ?
				voting_type::majority : ((national_rules & issues::rules::voting_system_mask) == issues::rules::dhont ? voting_type::normalized : voting_type::additive);

			auto const ideology_vsize = ve::to_vector_size(ws.s.ideologies_m.ideologies_count);
			float* const vote = (float*)ve_aligned_alloca(ideology_vsize * sizeof(float));

			populate_voting_info(ws, player, tagged_array_view<float, ideologies::ideology_tag>(vote, ideology_vsize), vtype);

			auto total_voting = ve::reduce(ws.s.ideologies_m.ideologies_count, tagged_array_view<float, ideologies::ideology_tag>(vote, ws.s.ideologies_m.ideologies_count));

			for(int32_t i = 0; i < int32_t(ws.s.ideologies_m.ideologies_count); ++i) {
				auto const party = ws.w.nation_s.active_parties.get(player, ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i)));

				pie.add_entry(ws.w.gui_m,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.governments_m.parties[party].name),
					vote[i] / total_voting,
					ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(ideologies::ideology_tag::value_base_t(i))].color);
			}
		}
	}
	void people_ideologies_pie_chart::update(ui::piechart<people_ideologies_pie_chart>& pie, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto people_i = &(ws.w.nation_s.nation_demographics.get_row(id)[population::to_demo_tag(ws, ideologies::ideology_tag(0))]);
				float total_pop = float(ws.w.nation_s.nation_demographics.get(id, population::total_population_tag));
				if(total_pop != 0.0f) {
					for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
						ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
						if(people_i[i] != 0) {
							pie.add_entry(ws.w.gui_m,
								text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.ideologies_m.ideology_container[this_tag].name),
								float(people_i[i]) / total_pop,
								ws.s.ideologies_m.ideology_container[this_tag].color);
						}
					}
				}
			}
		}
	}
	ui::window_tag upperhouse_ideology_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["ideology_option_window"]);
	}

	ui::window_tag issues_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["issue_option_window"]);
	}
	void unciv_overlay::on_create(ui::simple_button<unciv_overlay>& self, world_state & ws) {
		self.associated_object->flags.fetch_or(ui::gui_object::force_transparency_check);
	}
	void decision_item_image::on_create(ui::dynamic_icon<decision_item_image>& self, world_state & ws) {
		auto gi = ws.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(self.associated_object->type_dependant_handle.load(std::memory_order_acquire)));
		if(gi)
			default_image = gi->t;
	}
	void decision_item_requirements::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(tag)) {
			triggers::make_effect_description(ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format,
				ws.s.trigger_m.effect_data.data() + to_index(tag), player, player, triggers::const_parameter(), get_local_generator());
		} else {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
		}
	}
	void decision_item_checkbox::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void decision_item_checkbox::button_function(ui::simple_button<decision_item_checkbox>& self, world_state & ws) {}
	void enact_decision_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto player = ws.w.local_player_nation;
		if(is_valid_index(requirements))
			triggers::make_trigger_description(ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format,
				ws.s.trigger_m.trigger_data.data() + to_index(requirements), player, player, triggers::const_parameter());
	}
	void enact_decision_button::button_function(ui::simple_button<enact_decision_button>& self, world_state & ws) {}
	ui::window_tag decision_listbox::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["decision_entry"]);
	}
	void supression_points_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.nation_s.nations.get<nation::suppression_points>(player));

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}
	void movement_sortby_size_button::button_function(ui::simple_button<movement_sortby_size_button>&, world_state &) {}
	void movement_sortby_radical_button::button_function(ui::simple_button<movement_sortby_radical_button>&, world_state &) {}
	void movement_sortby_name_button::button_function(ui::simple_button<movement_sortby_name_button>&, world_state &) {}
	void movements_item_suppress_button::button_function(ui::simple_button<movement_sortby_name_button>&, world_state &) {}

	bool civilized_reforms_pane_base::create_dynamic(world_state & ws, ui::tagged_gui_object win, ui::element_tag t, const char * name_start, const char * name_end) {
		if(!static_data_loaded) {
			reform_window_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["reform_option_window"]);
			auto& lb_entry_def = ws.s.gui_m.ui_definitions.windows[reform_window_tag];
			for(auto sub_element : lb_entry_def.sub_object_definitions) {
				if(std::holds_alternative<ui::icon_tag>(sub_element)) {
					auto selected_tag = std::get<ui::icon_tag>(sub_element);
					auto& selected_def = ws.s.gui_m.ui_definitions.icons[selected_tag];
					lb_x_offset = selected_def.position.x;
					lb_y_offset = selected_def.position.y;
				}
			}
			reform_windows.resize(ws.s.issues_m.political_issues.size() + ws.s.issues_m.social_issues.size());
			static_data_loaded = true;
		}
		if(std::holds_alternative<ui::window_tag>(t)) {
			auto text_tag = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, name_start, name_end);
			if(auto f = ws.s.issues_m.named_issue_index.find(text_tag); f != ws.s.issues_m.named_issue_index.end()) {
				auto& iss = ws.s.issues_m.issues_container[f->second];
				if(iss.type == issues::issue_group::political || iss.type == issues::issue_group::social) {
					auto index_in_array = to_index(f->second) - ws.s.issues_m.party_issues.size();
					reform_windows[index_in_array].init(f->second, lb_x_offset, lb_y_offset);
					ui::create_static_element(ws, std::get<ui::window_tag>(t), win, reform_windows[index_in_array]);
				} else {
					return false;
				}
			} else {
				return false;
			}
			return true;
		} else {
			return false;
		}
	}

	bool uncivilized_reforms_pane_base::create_dynamic(world_state & ws, ui::tagged_gui_object win, ui::element_tag t, const char * name_start, const char * name_end) {
		if(!static_data_loaded) {
			reform_window_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["reform_option_window"]);
			auto& lb_entry_def = ws.s.gui_m.ui_definitions.windows[reform_window_tag];
			for(auto sub_element : lb_entry_def.sub_object_definitions) {
				if(std::holds_alternative<ui::icon_tag>(sub_element)) {
					auto selected_tag = std::get<ui::icon_tag>(sub_element);
					auto& selected_def = ws.s.gui_m.ui_definitions.icons[selected_tag];
					lb_x_offset = selected_def.position.x;
					lb_y_offset = selected_def.position.y;
				}
			}
			reform_windows.resize(ws.s.issues_m.military_issues.size() + ws.s.issues_m.economic_issues.size());
			static_data_loaded = true;
		}
		if(std::holds_alternative<ui::window_tag>(t)) {
			auto text_tag = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, name_start, name_end);
			if(auto f = ws.s.issues_m.named_issue_index.find(text_tag); f != ws.s.issues_m.named_issue_index.end()) {
				auto& iss = ws.s.issues_m.issues_container[f->second];
				if(iss.type == issues::issue_group::military || iss.type == issues::issue_group::economic) {
					auto index_in_array = to_index(f->second) - (ws.s.issues_m.party_issues.size() + ws.s.issues_m.political_issues.size() + ws.s.issues_m.social_issues.size());
					reform_windows[index_in_array].init(f->second, lb_x_offset, lb_y_offset);
					ui::create_static_element(ws, std::get<ui::window_tag>(t), win, reform_windows[index_in_array]);
				} else {
					return false;
				}
			} else {
				return false;
			}
			return true;
		} else {
			return false;
		}
	}

	void unselected_option_button::button_function(ui::simple_button<unselected_option_button>&, world_state &) {}
	void unselected_option_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto mod = ws.s.issues_m.options[tag].modifier;
		if(is_valid_index(mod))
			modifiers::make_national_modifier_text(ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format, mod);
	}
	void release_nation_button::button_function(ui::simple_button<release_nation_button>&, world_state &) {}
	void social_reform_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::cannot_social_reform], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
	void political_reform_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::cannot_political_reform], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}
	government_window::government_window() : win(std::make_unique<government_window_t>()) {}
	government_window::~government_window() {}
	void government_window::init_government_window(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_politics"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}

	void government_window::hide_government_window(ui::gui_manager& gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void government_window::show_government_reforms_tab(ui::gui_manager& gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		win->template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 0ui32);

		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("reforms_window")>().associated_object));
		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("unciv_reforms_window")>().associated_object));

		ui::hide(*(win->template get<CT_STRING("movements_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("decision_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("release_nation")>().associated_object));

		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void government_window::show_government_movements_tab(ui::gui_manager& gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		win->template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 1ui32);

		auto& pane = win->template get<CT_STRING("movements_window")>();
		pane.template get<CT_STRING("movements_listbox")>().new_list(nullptr, nullptr);
		pane.template get<CT_STRING("rebel_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(pane.associated_object));

		ui::hide(*(win->template get<CT_STRING("decision_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("reforms_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("unciv_reforms_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("release_nation")>().associated_object));

		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void government_window::show_government_decisions_tab(ui::gui_manager& gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		win->template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 2ui32);

		auto& pane = win->template get<CT_STRING("decision_window")>();
		pane.template get<CT_STRING("decision_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(pane.associated_object));

		ui::hide(*(win->template get<CT_STRING("movements_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("reforms_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("unciv_reforms_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("release_nation")>().associated_object));

		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void government_window::show_government_release_nations_tab(ui::gui_manager& gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		win->template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 3ui32);

		auto& pane = win->template get<CT_STRING("release_nation")>();
		pane.template get<CT_STRING("nations")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(pane.associated_object));

		ui::hide(*(win->template get<CT_STRING("movements_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("decision_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("reforms_window")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("unciv_reforms_window")>().associated_object));

		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void selected_option::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::unlimited_line_manager lm;
		auto mod = ws.s.issues_m.options[tag].modifier;
		if(is_valid_index(mod))
			modifiers::make_national_modifier_text(ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format, mod);
	}
	void close_choose_party_window::button_function(ui::simple_button<close_choose_party_window>&, world_state & ws) {
		ui::hide(*(ws.w.government_w.win->choose_window.associated_object));
	}
	void issue_name_text::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		auto party_option = ws.s.governments_m.party_issues.get(value.first, value.second);

		ui::unlimited_line_manager lm;
		modifiers::make_national_modifier_text(ws, tw, ui::xy_pair{0,0}, lm, ui::tooltip_text_format, ws.s.issues_m.options[party_option].modifier);
	}
	ui::window_tag party_issues_listbox::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["party_issue_option_window"]);
	}
	ui::window_tag party_choice_listbox::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["party_window"]);
	}
	void main_party_choice_ideology_icon::update(ui::tinted_icon<main_party_choice_ideology_icon>& self, world_state & ws) {
		auto const ideology = ws.w.nation_s.nations.get<nation::ruling_ideology>(ws.w.local_player_nation);
		if(ideology) {
			auto const party_color = ws.s.ideologies_m.ideology_container[ideology].color;
			self.set_color(ws.w.gui_m, float(party_color.r) / 255.0f, float(party_color.g) / 255.0f, float(party_color.b) / 255.0f);
		} else {
			self.set_color(ws.w.gui_m, 255.0f, 255.0f, 255.0f);
		}
	}
	void main_party_choice_button::button_function(ui::button<main_party_choice_button>&, world_state& ws) {
		ui::make_visible_and_update(ws.w.gui_m, *(ws.w.government_w.win->choose_window.associated_object));
	}
	void main_party_choice_button::update(ui::button<main_party_choice_button>& self, world_state & ws) {
		if(auto player_gov = ws.w.nation_s.nations.get<nation::current_government>(ws.w.local_player_nation); player_gov) {
			self.set_enabled(ws.s.governments_m.governments_container[player_gov].appoint_ruling_party);
		}
		self.set_text(ws, ws.s.governments_m.parties[ws.w.nation_s.nations.get<nation::ruling_party>(ws.w.local_player_nation)].name);
	}
	bool main_party_choice_button::has_tooltip(world_state &ws) {
		if(auto const gov = ws.w.nation_s.nations.get<nation::current_government>(ws.w.local_player_nation); gov) {
			return ws.s.governments_m.governments_container[gov].appoint_ruling_party == false;
		} else {
			return false;
		}
	}
	void main_party_choice_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto player_gov = ws.w.nation_s.nations.get<nation::current_government>(ws.w.local_player_nation); player_gov) {
			if(ws.s.governments_m.governments_container[player_gov].appoint_ruling_party == false) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::cannot_appoint_ruling_party], ui::tooltip_text_format,
					ws.s.gui_m, ws.w.gui_m, tw);
			}
		}
	}
	void party_choice_button::button_function(ui::button<party_choice_button>& self, world_state & ws) {
		ws.w.pending_commands.add<commands::change_ruling_party>(ws.w.local_player_nation, value);
		ui::hide(*(ws.w.government_w.win->choose_window.associated_object));
	}
	void party_choice_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const player = ws.w.local_player_nation; player) {
			auto const gov = ws.w.nation_s.nations.get<nation::current_government>(player);
			auto const party_ideology = ws.s.governments_m.parties[value].ideology;

			ui::xy_pair cursor{ 0,0 };
			if(value == ws.w.nation_s.nations.get<nation::ruling_party>(player)) {
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::party_already_ruling], ui::tooltip_text_format,
					ws.s.gui_m, ws.w.gui_m, tw);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			} else if(gov && party_ideology && ws.s.governments_m.permitted_ideologies.get(gov, party_ideology) == 0) {
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::party_not_permitted], ui::tooltip_text_format,
					ws.s.gui_m, ws.w.gui_m, tw);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			}

			for(uint32_t i = 0; i < ws.s.issues_m.party_issues.size(); ++i) {
				auto const opt = ws.s.governments_m.party_issues.get(value, i);
				for(int32_t j = 0; j < issues::rules::boolean_rule_count; ++i) {
					if((ws.s.issues_m.options[opt].issue_rules.rules_mask.rules_value & issues::rules::boolean_rule_index_to_mask(j)) != 0) {
						if((ws.s.issues_m.options[opt].issue_rules.rules_settings.rules_value & issues::rules::boolean_rule_index_to_mask(j)) != 0) {
							ui::text_format local_fmt{ ui::text_color::green, ui::tooltip_text_format.font_handle, ui::tooltip_text_format.font_size };
							cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u2714 "), tw, cursor, local_fmt);
						} else {
							ui::text_format local_fmt{ ui::text_color::red, ui::tooltip_text_format.font_handle, ui::tooltip_text_format.font_size };
							cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u274C "), tw, cursor, local_fmt);
						}
						cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::build_factory + j], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
						cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
					}
				}
				
			}
		}
	}
}
