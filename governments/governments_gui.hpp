#pragma once
#include "common\\common.h"
#include "governments_gui.h"
#include "triggers\\triggers.h"
#include "triggers\\effects.h"

namespace governments {
	template<typename W>
	void government_window_base::on_create(W & w, world_state &ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 671i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 38i16 };
		});

		ui::hide(*associated_object);
	}

	template<typename W>
	void decision_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 612i16, 560i16 };
		associated_object->position += ui::xy_pair{ 0i16, 34i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -34i16 };
		});
	}

	template<typename W>
	void movements_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 613i16, 560i16 };
		associated_object->position += ui::xy_pair{ 0i16, 34i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -34i16 };
		});
	}

	template<typename W>
	void release_nations_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 616i16, 560i16 };
		associated_object->position += ui::xy_pair{ 0i16, 34i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -34i16 };
		});
	}

	template<typename W>
	void civilized_reforms_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 616i16, 568i16 };
		associated_object->position += ui::xy_pair{ 381i16, 46i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -381i16, -46i16 };
		});
	}

	template<typename W>
	void uncivilized_reforms_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 616i16, 568i16 };
		associated_object->position += ui::xy_pair{ 381i16, 46i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -381i16, -46i16 };
		});
	}

	template <typename lb_type>
	void modifier_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto static_mod_range = get_range(ws.w.nation_s.static_modifier_arrays, player->static_modifiers);
			for(auto s = static_mod_range.first; s != static_mod_range.second; ++s) {
				if(auto m = *s; is_valid_index(m))
					lb.add_item(ws, ws.s.modifiers_m.national_modifiers[m].icon, *s, date_tag());
			}
			auto timed_mod_range = get_range(ws.w.nation_s.timed_modifier_arrays, player->timed_modifiers);
			for(auto s = timed_mod_range.first; s != timed_mod_range.second; ++s) {
				if(auto m = s->mod; is_valid_index(m))
					lb.add_item(ws, ws.s.modifiers_m.national_modifiers[m].icon, m, s->expiration);
			}
		}
	}

	template<typename W>
	void upperhouse_ideology_icon::windowed_update(ui::tinted_icon<upperhouse_ideology_icon>& self, W & w, world_state & ws) {
		self.set_color(ws.w.gui_m, float(w.color.r) / 255.0f, float(w.color.g) / 255.0f, float(w.color.b) / 255.0f);
	}

	template<typename window_type>
	void upperhouse_ideology_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager &lm, ui::text_format &fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, win.legend_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void upperhouse_ideology_percentage::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::percent, win.percentage);

		ui::text_chunk_to_instances( ws.s.gui_m, ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box, ui::xy_pair{ 0,0 }, fmt, lm);

		lm.finish_current_line();
	}

	template<typename lb_type>
	void upperhouse_ideology_lb::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, float>, 32> data;

		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player->id; ws.w.nation_s.nations.is_valid_index(id)) {
				auto upper_house = ws.w.nation_s.upper_house.get_row(id);
				for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
					ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
					if(upper_house[i] != 0) {
						data.emplace_back(
							ws.s.ideologies_m.ideology_container[this_tag].color,
							ws.s.ideologies_m.ideology_container[this_tag].name,
							float(upper_house[i]) / 100.0f);
					}
				}
			}
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename lb_type>
	void issues_lb::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<std::tuple<text_data::text_tag, float, float>, 32, concurrent_allocator<std::tuple<text_data::text_tag, float, float>>> data;

		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player->id; ws.w.nation_s.nations.is_valid_index(id)) {
				auto iss = ws.w.nation_s.nation_demographics.get_row(id) + to_index(population::to_demo_tag(ws, issues::option_tag(0)));
				float total_pop = float(ws.w.nation_s.nation_demographics.get(id, population::total_population_tag));

				if(total_pop != 0) {
					for(uint32_t i = 0; i < ws.s.issues_m.tracked_options_count; ++i) {
						issues::option_tag this_tag(static_cast<issues::option_tag::value_base_t>(i));
						if(iss[i] != 0) {
							data.emplace_back(
								ws.s.issues_m.options[this_tag].name,
								iss[i] / total_pop,
								0.0f);
						}
					}
				}
			}
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename window_type>
	void issues_item_popular_percentage::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::percent, win.people_percentage);

		ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box, ui::xy_pair{ 0,0 }, fmt, lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void issues_voter_percentage::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::percent, win.voters_percent);

		ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box, ui::xy_pair{ 0,0 }, fmt, lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void issues_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager &lm, ui::text_format &fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, win.legend_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void unciv_overlay::windowed_update(ui::simple_button<unciv_overlay>& self, window_type &, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if((player->flags & nations::nation::is_civilized) != 0)
				ui::hide(*self.associated_object);
			else
				ui::make_visible_immediate(*self.associated_object);
		}
	}

	template<typename window_type>
	void decision_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.event_m.decision_container[win.tag].title, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void decision_item_description::windowed_update(window_type & win, ui::tagged_gui_object box, ui::line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.event_m.decision_container[win.tag].body, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void decision_item_image::windowed_update(ui::dynamic_icon<decision_item_image>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag)) {
			graphics::texture_tag picture = ws.s.event_m.decision_container[win.tag].picture;
			auto gi = ws.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(self.associated_object->type_dependant_handle.load(std::memory_order_acquire)));

			if(is_valid_index(picture)) {
				if(gi)
					gi->t = &(ws.s.gui_m.textures.retrieve_by_key(picture));
			} else {
				if(gi)
					gi->t = default_image;
			}
		}
	}

	template<typename window_type>
	void decision_item_requirements::windowed_update(ui::dynamic_icon<decision_item_requirements>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag))
			tag = ws.s.event_m.decision_container[win.tag].allow;
	}

	template<typename window_type>
	void enact_decision_button::windowed_update(ui::simple_button<enact_decision_button>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag))
			tag = ws.s.event_m.decision_container[win.tag].effect;
	}

	template<typename window_type>
	void decision_item_checkbox::windowed_update(ui::simple_button<decision_item_checkbox>& self, window_type & w, world_state & ws) {}

	template<typename lb_type>
	void decision_listbox::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<events::decision_tag, 32, concurrent_allocator<events::decision_tag>> possible_list;

		if(auto player = ws.w.local_player_nation; player) {
			for(auto& d : ws.s.event_m.decision_container) {
				if(!is_valid_index(d.potential) || triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(d.potential), ws, player, player, nullptr, nullptr)) {
					possible_list.push_back(d.id);
				}
			}
		}

		lb.update_list(possible_list.begin().get_ptr(), possible_list.end().get_ptr());
	}

	template<typename window_type>
	void movement_ind_flag::windowed_update(ui::masked_flag<movement_ind_flag>& self, window_type & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			auto independance_nation = ws.w.population_s.pop_movements[w.tag].liberation_country;
			if(is_valid_index(independance_nation)) {
				self.set_displayed_flag(ws, independance_nation);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename window_type>
	void movements_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	template<typename window_type>
	void movements_item_nationalist_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	template<typename window_type>
	void movements_item_size::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.population_s.pop_movements[win.tag].total_population_support);

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void movements_item_radicalism::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::fp_one_place, ws.w.population_s.pop_movements[win.tag].radicalism);

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void movements_item_suppress_button::windowed_update(ui::simple_button<movement_sortby_name_button>&, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<typename window_type>
	void rebels_item_type_icon::windowed_update(ui::dynamic_icon<rebels_item_type_icon>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag)) {
			auto icon = ws.w.population_s.rebel_factions[win.tag].icon;
			if(icon != 0)
				self.set_frame(ws.w.gui_m, uint32_t(icon) - 1ui32);
			else
				self.set_frame(ws.w.gui_m, 4ui32);
		}
	}

	template<typename window_type>
	void rebels_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			if(auto type = ws.w.population_s.rebel_factions[win.tag].type; is_valid_index(type)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.population_m.rebel_types[type].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void rebels_item_size::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_brigades_ready::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_brigades_active::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_organization::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_revolt_risk::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename lb_type>
	void movements_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto movement_range = get_range(ws.w.population_s.pop_movement_arrays, player->active_movements);
			lb.update_list(movement_range.first, movement_range.second);
		}
	}

	template<typename lb_type>
	void rebels_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto rebel_range = get_range(ws.w.population_s.rebel_faction_arrays, player->active_rebel_factions);
			lb.update_list(rebel_range.first, rebel_range.second);
		}
	}

	template<typename window_type>
	void reform_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.for_issue)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.issues_m.issues_container[win.for_issue].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void unselected_option_button::windowed_update(ui::simple_button<unselected_option_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(tag)) {
			if(auto id = player->id; ws.w.nation_s.nations.is_valid_index(id)) {
				auto& iss_def = ws.s.issues_m.issues_container[ws.s.issues_m.options[tag].parent_issue];
				auto current_option = ws.w.nation_s.active_issue_options.get(id, iss_def.id);

				if(tag == current_option)
					ui::hide(*self.associated_object);
				else
					ui::make_visible_immediate(*self.associated_object);

				self.set_enabled(true);
				if(iss_def.next_step_only) {

					if(to_index(tag) + 1 != to_index(current_option) && to_index(current_option) + 1 != to_index(tag))
						self.set_enabled(false);
				}
				// todo: disable if lacking support
			}
		}
	}

	template<typename window_type>
	void option_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.issues_m.options[win.tag].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void selected_option::windowed_update(ui::dynamic_icon<selected_option>& self, window_type & win, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			if(auto id = player->id; ws.w.nation_s.nations.is_valid_index(id)) {
				auto& iss_def = ws.s.issues_m.issues_container[ws.s.issues_m.options[win.tag].parent_issue];
				auto current_option = ws.w.nation_s.active_issue_options.get(id, iss_def.id);

				if(win.tag == current_option)
					ui::make_visible_immediate(*self.associated_object);
				else
					ui::hide(*self.associated_object);
			}
		}
	}

	template<typename lb_type>
	void reforms_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(is_valid_index(for_issue)) {
			auto& iss = ws.s.issues_m.issues_container[for_issue];
			uint32_t last = uint32_t(std::extent_v<decltype(iss.options)>);
			for(uint32_t i = 0; i < std::extent_v<decltype(iss.options)>; ++i) {
				if(!is_valid_index(iss.options[i])) {
					last = i;
					break;
				}
			}
			lb.new_list(&iss.options[0], &iss.options[last]);
		}
	}

	template<typename W>
	void reform_item_base::on_create(W & w, world_state & ws) {
		auto& selected = w.template get<CT_STRING("selected")>();
		ui::xy_pair selected_xy = ui::xy_pair{ -selected.associated_object->position.x, -selected.associated_object->position.y };

		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *w.associated_object, w.window_object }, [selected_xy](ui::tagged_gui_object o) {
			o.object.position += selected_xy;
		});
	}


	template<typename W>
	void reform_window_base::on_create(W & w, world_state & ws) {
		const auto new_obj = ws.w.gui_m.gui_objects.emplace();
		auto& iss = ws.s.issues_m.issues_container[for_issue];

		uint32_t last = uint32_t(std::extent_v<decltype(iss.options)>);
		for(uint32_t i = 0; i < std::extent_v<decltype(iss.options)>; ++i) {
			if(!is_valid_index(iss.options[i])) {
				last = i;
				break;
			}
		}

		auto item_tag = reform_options.element_tag(ws.s.gui_m);
		auto& window_d = ws.s.gui_m.ui_definitions.windows[item_tag];

		new_obj.object.position = ui::xy_pair{ int16_t(lb_x_offset), int16_t(lb_y_offset) };
		new_obj.object.size = ui::xy_pair{ 175i16, int16_t(window_d.size.y * (last) + 1) };

		w.associated_object->size = ui::xy_pair{ int16_t(lb_x_offset + 175), int16_t(lb_y_offset + window_d.size.y * (last) + 1) };

		new_obj.object.associated_behavior = &reform_options;
		reform_options.associated_object = &new_obj.object;

		reform_options.for_issue = for_issue;
		reform_options.create_sub_elements(new_obj, ws);

		ui::add_to_back(ws.w.gui_m, ui::tagged_gui_object{ *w.associated_object, w.window_object }, new_obj);
		ws.w.gui_m.flag_minimal_update();
	}

	template<typename window_type>
	void release_nation_flag::windowed_update(ui::masked_flag<release_nation_flag>& self, window_type & w, world_state & ws) {
		self.set_displayed_flag(ws, w.tag);
	}
	template<typename window_type>
	void release_nation_button::windowed_update(ui::simple_button<release_nation_button>&, window_type & win, world_state & ws) {}
	template<typename window_type>
	void release_nation_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.culture_m.national_tags[win.tag].default_name.name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void release_nation_type::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename window_type>
	void release_nation_description::windowed_update(window_type & win, ui::tagged_gui_object box, ui::line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	template<typename lb_type>
	void release_nations_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			boost::container::small_vector<cultures::national_tag, 32, concurrent_allocator<cultures::national_tag>> data;

			auto owned_range = get_range(ws.w.province_s.province_arrays, player->owned_provinces);
			for(auto p : owned_range) {
				if(is_valid_index(p)) {
					if(!contains_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[p].cores, player->tag)) {
						auto core_range = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[p].cores);
						for(auto c : core_range) {
							if(is_valid_index(c) && (!ws.w.culture_s.national_tags_state[c].is_not_releasable) && std::find(data.begin(), data.end(), c) == data.end()) {
								data.push_back(c);
							}
						}
					}
				}
			}

			lb.update_list(data.begin().get_ptr(), data.end().get_ptr());
		}
	}
}
