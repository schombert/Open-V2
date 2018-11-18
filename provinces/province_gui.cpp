#include "common\\common.h"
#include "province_gui.hpp"
#include "world_state\\world_state.h"
#include "modifiers\\modifiers_gui.h"
#include "nations\\nations_functions.h"
#include "province_functions.h"
#include "economy\\economy_functions.h"

namespace provinces {
	void close_province_window::button_function(ui::gui_object_tag, world_state& ws) {
		ws.w.province_w.hide_province_window(ws.w.gui_m);
	}

	void modifier_button::update(ui::simple_button<modifier_button>& ico, world_state & ws) {
		if(icon != 0ui32)
			ico.set_frame(ws.w.gui_m, icon - 1ui32);
		else
			ico.set_frame(ws.w.gui_m, 0ui32);
	}

	void modifier_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		if(is_valid_index(mod)) {
			ui::xy_pair cursor{ 0,0 };
			ui::unlimited_line_manager lm;
			cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::tooltip_text_format, mod);
			if(is_valid_index(expiration)) {
				char16_t formatted_date[64];
				u16_format_date(formatted_date, expiration);
				text_data::replacement date_rep(text_data::value_type::date, vector_backed_string<char16_t>(formatted_date), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::expires_on], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &date_rep, 1ui32);
			}
		}
	}

	void slave_state_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::slave_state], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void colony_button::button_function(ui::gui_object_tag, world_state&) {

	}

	void colony_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto sid = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);
			if(is_valid_index(sid)) {
				auto& state = ws.w.nation_s.states[sid];
				if(ws.w.local_player_nation == nullptr || ws.w.local_player_nation->id != ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov)) {
					if(state.flags & nations::state_instance::is_protectorate)
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::protectorate_province], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
					else if(state.flags & nations::state_instance::is_colonial)
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::colonial_province], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
				} else {
					ui::unlimited_line_manager lm;
					char16_t formatted_value[64];
					if(state.flags & nations::state_instance::is_protectorate) {
						u16itoa(nations::colonial_points_to_make_colony(ws, state), formatted_value);
						text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::protectorate_province_upgrade], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
					} else if(state.flags & nations::state_instance::is_colonial) {
						u16itoa(nations::colonial_points_to_make_state(ws, state), formatted_value);
						text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
						ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::protectorate_province_upgrade], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
					}
				}
			}
		}
	}

	void goods_type_icon::update(ui::dynamic_icon<goods_type_icon>& ico, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto rgo_type = ws.w.province_s.province_state_container.get<province_state::rgo_production>(selected_prov);
			if(is_valid_index(rgo_type))
				ico.set_frame(ws.w.gui_m, uint32_t(ws.s.economy_m.goods[rgo_type].icon));
			else
				ico.set_frame(ws.w.gui_m, 0ui32);
		}
	}

	void goods_type_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto rgo_type = ws.w.province_s.province_state_container.get<province_state::rgo_production>(selected_prov);
			if(is_valid_index(rgo_type))
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[rgo_type].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
		}
	}

	void owner_icon::update(ui::dynamic_icon<owner_icon>& ico, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto rgo_type = ws.w.province_s.province_state_container.get<province_state::rgo_production>(selected_prov);
			if(is_valid_index(rgo_type)) {
				bool is_mine = (ws.s.economy_m.goods[rgo_type].flags & economy::good_definition::mined) != 0;
				auto owner_type = is_mine ? ws.s.economy_m.rgo_mine.owner.type : ws.s.economy_m.rgo_farm.owner.type;
				ico.set_frame(ws.w.gui_m, uint32_t(ws.s.population_m.pop_types[owner_type].sprite) - 1ui32);
			}
		}
	}

	void owner_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto rgo_type = ws.w.province_s.province_state_container.get<province_state::rgo_production>(selected_prov);
			if(is_valid_index(rgo_type)) {
				bool is_mine = (ws.s.economy_m.goods[rgo_type].flags & economy::good_definition::mined) != 0;
				auto owner_type = is_mine ? ws.s.economy_m.rgo_mine.owner.type : ws.s.economy_m.rgo_farm.owner.type;

				auto demo_row = ws.w.province_s.province_demographics.get_row(selected_prov);
				auto total_pop = demo_row[to_index(population::total_population_tag)];
				auto owner_pop = demo_row[to_index(population::to_demo_tag(ws, owner_type))];

				float percent = total_pop != 0 ? float(owner_pop) / float(total_pop) : 0.0f;

				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, int32_t(percent * 100.0f + 0.5f));

				text_data::replacement value_rep[2] = {
					text_data::replacement(
						text_data::value_type::value,
						vector_backed_string<char16_t>(formatted_value),
						[](tagged_object<ui::gui_object, ui::gui_object_tag>) {}),
					text_data::replacement(
						text_data::value_type::type,
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.population_m.pop_types[owner_type].name),
						[](tagged_object<ui::gui_object, ui::gui_object_tag>) {})
				};

				ui::unlimited_line_manager lm;
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::owner_presence], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, value_rep, 2ui32);
				lm.finish_current_line();
			}
		}
	}

	void owner_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		int32_t value = 0;

		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto rgo_type = ws.w.province_s.province_state_container.get<province_state::rgo_production>(selected_prov);
			if(is_valid_index(rgo_type)) {
				bool is_mine = (ws.s.economy_m.goods[rgo_type].flags & economy::good_definition::mined) != 0;
				auto owner_type = is_mine ? ws.s.economy_m.rgo_mine.owner.type : ws.s.economy_m.rgo_farm.owner.type;

				auto demo_row = ws.w.province_s.province_demographics.get_row(selected_prov);
				auto total_pop = demo_row[to_index(population::total_population_tag)];
				auto owner_pop = demo_row[to_index(population::to_demo_tag(ws, owner_type))];

				float percent = total_pop != 0 ? float(owner_pop) / float(total_pop) : 0.0f;
				value = int32_t(percent* 100.0f + 0.5f);
			}
		}

		char16_t local_buf[64];

		auto end = u16itoa(value, local_buf);
		*end = u'%';
		*(end + 1) = char16_t(0);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);

		lm.finish_current_line();
	}


	void admin_icon::update(ui::dynamic_icon<admin_icon>& ico, world_state& ws) {
		ico.set_frame(ws.w.gui_m, uint32_t(ws.s.population_m.pop_types[ws.s.population_m.bureaucrat].sprite) - 1ui32);
	}

	bool admin_icon::has_tooltip(world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			return is_valid_index(ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov));
		}
		return false;
	}

	void admin_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state_id = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);
			if(is_valid_index(state_id)) {
				auto& state = ws.w.nation_s.states[state_id];

				ui::unlimited_line_manager lm;
				char16_t formatted_value[64];
				ui::xy_pair cursor{ 0,0 };

				u16itoa(int32_t(state.administrative_efficiency * 100.0f + 0.5f), formatted_value);
				text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();

				lm.increase_indent(1);

				u16itoa(0, formatted_value);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin_base], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();

				u16itoa(0, formatted_value);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin_pops], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();

				u16itoa(0, formatted_value);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_admin_tech], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();

				u16itoa(0, formatted_value);
				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_view_state_non_cores], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
				cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
				lm.finish_current_line();
			}
		}
	}

	void admin_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		int32_t value = 0;

		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state_id = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);
			if(is_valid_index(state_id)) {
				value = int32_t(ws.w.nation_s.states[state_id].administrative_efficiency * 100.0f + 0.5f);
			}
		}

		char16_t local_buf[64];

		auto end = u16itoa(value, local_buf);
		*end = u'%';
		*(end + 1) = char16_t(0);

		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(local_buf),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);

		lm.finish_current_line();
	}

	void terrain_icon::update(ui::dynamic_icon<terrain_icon>& ico, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto terrain_id = ws.w.province_s.province_state_container.get<province_state::terrain>(selected_prov);
			if(auto f = ws.s.province_m.terrain_graphics.find(terrain_id); f != ws.s.province_m.terrain_graphics.end()) {

				auto associated_obj_tag = ui::graphics_instance_tag(ico.associated_object->type_dependant_handle.load(std::memory_order_acquire));
				auto associated_gobj = ws.w.gui_m.graphics_instances.safe_at(associated_obj_tag);
				if(associated_gobj) {
					associated_gobj->t = &(ws.s.gui_m.textures.retrieve_by_key(ws.s.gui_m.graphics_object_definitions.definitions[f->second].primary_texture_handle));
				}

				ico.set_visibility(ws.w.gui_m, true);
				return;
			}
		}
		ico.set_visibility(ws.w.gui_m, false);
	}

	void terrain_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			ui::xy_pair cursor{ 0,0 };
			ui::unlimited_line_manager lm;

			auto terrain_mod = ws.w.province_s.province_state_container.get<province_state::terrain>(selected_prov);
			if(is_valid_index(terrain_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::tooltip_text_format, terrain_mod);

			auto climate_mod = ws.s.province_m.province_container.get<province::climate>(selected_prov);
			if(is_valid_index(climate_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::tooltip_text_format, climate_mod);

			auto continent_mod = ws.s.province_m.province_container.get<province::continent>(selected_prov);
			if(is_valid_index(continent_mod))
				cursor = modifiers::make_province_modifier_text(ws, tw, cursor, lm, ui::tooltip_text_format, continent_mod);

		}
	}

	void province_window_base::on_create(world_state& ws) {
		ui::hide(*associated_object);
		associated_object->position += ui::xy_pair{ 0i16, -19i16 };
		associated_object->size = ui::xy_pair{ 406i16, 548i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, 19i16 };
		});
	}

	void province_window_header_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 406i16, 200i16 };
		associated_object->position += ui::xy_pair{ 0i16, 1i16 };
	}

	void province_statistics_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 406i16, 352i16 };
	}

	void province_colony_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 380i16, 350i16 };
	}

	void province_other_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 380i16, 343i16 };
	}

	void colonist_list_item_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 380i16, 50i16 };
	}

	void stage_list_item_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 60i16, 48i16 };
	}

	ui::window_tag modifier_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["prov_state_modifier"]);
	}

	ui::window_tag cores_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["province_core"]);
	}

	void infrastructure_progress_base::on_create(world_state&) {
		associated_object->size = ui::xy_pair{ 70i16, 10i16 };
	}

	void province_controller_flag::create_tooltip(ui::masked_flag<province_controller_flag>&, world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto controller = ws.w.province_s.province_state_container.get<province_state::controller>(selected_prov);
			if(is_valid_index(controller)) {
				ui::unlimited_line_manager lm;
				ui::xy_pair cursor{ 0,0 };

				cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::controller], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
				cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);
				cursor = ui::add_linear_text(cursor, ws.w.nation_s.nations[controller].name, ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			}
		}
	}

	void liferating_bar::update(ui::progress_bar<liferating_bar>& self, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			self.set_fraction(provinces::get_life_rating(ws, selected_prov) / 50.0f);
		}
	}

	void flashpoint_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state_id = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);
			if(is_valid_index(state_id)) {
				auto& state = ws.w.nation_s.states[state_id];
				if(is_valid_index(state.crisis_tag)) {
					ui::unlimited_line_manager lm;
					char16_t formatted_value[64];
					ui::xy_pair cursor{ 0,0 };

					put_value_in_buffer(formatted_value, display_type::fp_two_places, state.current_tension);

					text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
					ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::flashpoint_tension], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
					return;
				}
			}
		}

		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::has_no_flashpoint], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void liferating_overlay::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			ui::unlimited_line_manager lm;
			char16_t formatted_value[64];
			ui::xy_pair cursor{ 0,0 };

			put_value_in_buffer(formatted_value, display_type::integer, provinces::get_life_rating(ws, selected_prov));

			text_data::replacement value_rep(text_data::value_type::value, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
			ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::province_liferating], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
			cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
			lm.finish_current_line();
			lm.increase_indent(1);
			modifiers::explain_province_modifier(ws, tw, cursor, lm, ui::tooltip_text_format, selected_prov, modifiers::provincial_offsets::life_rating);
		}
	}

	void occupation_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto progress = ws.w.province_s.province_state_container.get<province_state::siege_progress>(selected_prov);

			ui::unlimited_line_manager lm;
			ui::xy_pair cursor{ 0,0 };

			cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::siege_progress], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm);
			cursor = ui::advance_cursor_by_space(cursor, ws.s.gui_m, ui::tooltip_text_format);

			char16_t formatted_value[64];
			put_value_in_buffer(formatted_value, display_type::percent, progress);

			ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(formatted_value),
				tw,
				cursor,
				ui::tooltip_text_format,
				lm);

			lm.finish_current_line();
		}
	}


	void produced_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			char16_t formatted_value[64];
			put_value_in_buffer(formatted_value, display_type::fp_two_places, economy::province_rgo_production_qnty(ws, selected_prov));

			ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(formatted_value),
				box,
				ui::xy_pair{ 0,0 },
				fmt,
				lm);

			lm.finish_current_line();
		}
	}

	void crimefight_percent_text_box::update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&) {

	}

	void rebel_percent_text_box::update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&) {

	}

	void migration_text_box::update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&) {

	}

	void growth_text_box::update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&) {

	}

	void total_population_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		int32_t value = 0;
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto demo_row = ws.w.province_s.province_demographics.get_row(selected_prov);
			value = demo_row[to_index(population::total_population_tag)];
		}
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::integer, value);
		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);
		lm.finish_current_line();
	}

	void poptype_pie_chart::update(ui::piechart<poptype_pie_chart>& pie, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto demo_row = ws.w.province_s.province_demographics.get_row(selected_prov);
			const float total_pop = float(demo_row[to_index(population::total_population_tag)]);

			if(total_pop != 0.0f) {
				const auto ptype_offset = population::to_demo_tag(ws, population::pop_type_tag(0));

				for(uint32_t i = 0ui32; i < ws.s.population_m.pop_types.size(); ++i) {
					if(demo_row[to_index(ptype_offset) + i] != 0) {
						float fraction = float(demo_row[to_index(ptype_offset) + i]) / total_pop;
						pie.add_entry(
							ws.w.gui_m,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].name),
							fraction,
							ws.s.population_m.pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))].color);
					}
				}
			}
			pie.fill_remainder(ws.w.gui_m, text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::other]), graphics::color_rgb{ 255, 255, 255 });
		}
	}

	void ideology_pie_chart::update(ui::piechart<ideology_pie_chart>& pie, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto demo_row = ws.w.province_s.province_demographics.get_row(selected_prov);
			const float total_pop = float(demo_row[to_index(population::total_population_tag)]);

			if(total_pop != 0.0f) {
				const auto itype_offset = population::to_demo_tag(ws, ideologies::ideology_tag(0));

				for(uint32_t i = 0ui32; i < ws.s.ideologies_m.ideologies_count; ++i) {
					if(demo_row[to_index(itype_offset) + i] != 0) {
						float fraction = float(demo_row[to_index(itype_offset) + i]) / total_pop;
						pie.add_entry(
							ws.w.gui_m,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].name),
							fraction,
							ws.s.ideologies_m.ideology_container[ideologies::ideology_tag(static_cast<ideologies::ideology_tag::value_base_t>(i))].color);
					}
				}
			}
			pie.fill_remainder(ws.w.gui_m, text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::other]), graphics::color_rgb{ 255, 255, 255 });
		}
	}

	void culture_pie_chart::update(ui::piechart<culture_pie_chart>& pie, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto demo_row = ws.w.province_s.province_demographics.get_row(selected_prov);
			const float total_pop = float(demo_row[to_index(population::total_population_tag)]);

			if(total_pop != 0.0f) {
				const auto itype_offset = population::to_demo_tag(ws, cultures::culture_tag(0));

				for(uint32_t i = 0ui32; i < ws.s.culture_m.count_cultures; ++i) {
					if(demo_row[to_index(itype_offset) + i] != 0) {
						float fraction = float(demo_row[to_index(itype_offset) + i]) / total_pop;
						pie.add_entry(
							ws.w.gui_m,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].name),
							fraction,
							ws.s.culture_m.culture_container[cultures::culture_tag(static_cast<cultures::culture_tag::value_base_t>(i))].color);
					}
				}
			}
			pie.fill_remainder(ws.w.gui_m, text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::other]), graphics::color_rgb{ 255, 255, 255 });
		}
	}

	void income_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			char16_t formatted_value[64];
			auto rgo_type = ws.w.province_s.province_state_container.get<province_state::rgo_production>(selected_prov);
			if(auto sid = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);
				ws.w.nation_s.states.is_valid_index(sid) && is_valid_index(rgo_type)) {
				
					auto last_rgo_production = economy::province_rgo_production_qnty(ws, selected_prov);
					auto value = last_rgo_production * economy::state_current_prices(ws, sid)[to_index(rgo_type)];
					put_value_in_buffer(formatted_value, display_type::currency, value);
					
					ui::text_chunk_to_instances(
						ws.s.gui_m,
						ws.w.gui_m,
						vector_backed_string<char16_t>(formatted_value),
						box,
						ui::xy_pair{ 0,0 },
						fmt,
						lm);

					lm.finish_current_line();
				
			}
		}
	}


	void rgo_population_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			char16_t formatted_value[64];

			auto& worker_data = ws.w.province_s.province_state_container.get<province_state::rgo_worker_data>(selected_prov);
			int32_t worker_sum = std::reduce(std::begin(worker_data.worker_populations), std::end(worker_data.worker_populations), 0, std::plus<>());
			put_value_in_buffer(formatted_value, display_type::integer, worker_sum);

			ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(formatted_value),
				box,
				ui::xy_pair{ 0,0 },
				fmt,
				lm);

			lm.finish_current_line();
		}
	}


	void rgo_percent_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			char16_t formatted_value[64];

			auto& worker_data = ws.w.province_s.province_state_container.get<province_state::rgo_worker_data>(selected_prov);
			int32_t worker_sum = std::reduce(std::begin(worker_data.worker_populations), std::end(worker_data.worker_populations), 0, std::plus<>());

			put_value_in_buffer(formatted_value, display_type::percent, float(worker_sum) / std::max(1.0f, float(economy::rgo_max_employment(ws, selected_prov))));

			ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(formatted_value),
				box,
				ui::xy_pair{ 0,0 },
				fmt,
				lm);

			lm.finish_current_line();
		}
	}


	void employment_ratio::update(ui::dynamic_icon<employment_ratio>& ico, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto& worker_data = ws.w.province_s.province_state_container.get<province_state::rgo_worker_data>(selected_prov);
			int32_t worker_sum = std::reduce(std::begin(worker_data.worker_populations), std::end(worker_data.worker_populations), 0, std::plus<>());

			float ratio = float(worker_sum) / std::max(1.0f, float(economy::rgo_max_employment(ws, selected_prov)));

			ico.set_frame(ws.w.gui_m, uint32_t(ratio * 10.0f + 0.5f));
		}
	}

	void open_popscreen_button::button_function(ui::gui_object_tag, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			auto si = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);

			ws.w.population_w.show_population_window(ws.w.gui_m, owner, si, selected_prov);
		}
		ws.w.province_w.hide_province_window(ws.w.gui_m);
	}

	void open_popscreen_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			text_data::replacement value_rep(
				text_data::value_type::loc,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.w.province_s.province_state_container.get<province_state::name>(selected_prov)),
				[](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
			ui::unlimited_line_manager lm;
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::open_pop_screen], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
			lm.finish_current_line();
		}
	}

	void build_factory_button::button_function(ui::gui_object_tag, world_state&) {}
	void build_factory_button::create_tooltip(world_state&, ui::tagged_gui_object) {}
	void party_loyalty_icon::create_tooltip(world_state&, ui::tagged_gui_object) {

	}
	void supply_limit_text_box::update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&) {

	}

	void core_flag_button::update(ui::masked_flag<core_flag_button>& self, world_state& ws) {
		self.set_displayed_flag(ws, core);
	}

	void uncolonized_phase_text_box::update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&) {

	}

	void crisis_temperature_bar::update(ui::progress_bar<crisis_temperature_bar>&, world_state&) {

	}

	void stage_progress_button::button_function(ui::gui_object_tag, world_state&) {}

	void stage_progress_button::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		if(req_pts != -1) {
			ui::unlimited_line_manager lm;
			char16_t formatted_value[64];

			put_value_in_buffer(formatted_value, display_type::integer, req_pts);
			text_data::replacement value_rep(text_data::value_type::cost, vector_backed_string<char16_t>(formatted_value), [](tagged_object<ui::gui_object, ui::gui_object_tag>) {});
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::colonial_investment_cost], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw, lm, &value_rep, 1ui32);
		}
	}



	void colonist_controller_flag_button::update(ui::masked_flag<colonist_controller_flag_button>& self, world_state& ws) {
		if(is_valid_index(colonizer))
			self.set_displayed_flag(ws, ws.w.nation_s.nations.get(colonizer).tag);
	}


	void colonize_button::button_function(ui::gui_object_tag, world_state&) {}
	void colonize_button::update(ui::simple_button<colonize_button>&, world_state&) {}
	bool colonize_button::has_tooltip(world_state&) { return false; }
	void colonize_button::create_tooltip(world_state&, ui::tagged_gui_object) {}

	void withdraw_button::button_function(ui::gui_object_tag, world_state&) {}
	void withdraw_button::update(ui::simple_button<withdraw_button>&, world_state&) {}
	bool withdraw_button::has_tooltip(world_state&) { return false; }
	void withdraw_button::create_tooltip(world_state&, ui::tagged_gui_object) {}


	void province_owner_flag::update(ui::masked_flag<province_owner_flag>& self, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			if(is_valid_index(owner)) {
				self.set_displayed_flag(ws, owner);
			}
		}
	}

	void relation_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto player = ws.w.local_player_nation;
		auto selected_prov = ws.w.province_w.selected_province;
		if(player && is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			if(is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, nations::get_relationship(ws, *player, owner));

				ui::text_chunk_to_instances(
					ws.s.gui_m,
					ws.w.gui_m,
					vector_backed_string<char16_t>(formatted_value),
					box,
					ui::xy_pair{ 0,0 },
					fmt,
					lm);

				lm.finish_current_line();
			}
		}
	}

	void country_flag_frame::update(ui::dynamic_transparent_icon<country_flag_frame>& ico, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			if(is_valid_index(owner)) {
				if((ws.w.nation_s.nations[owner].flags & nations::nation::is_civilized) == 0)
					ico.set_frame(ws.w.gui_m, 3ui32);
				else if(nations::is_great_power(ws, ws.w.nation_s.nations[owner]))
					ico.set_frame(ws.w.gui_m, 0ui32);
				else if(ws.w.nation_s.nations[owner].overall_rank <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
					ico.set_frame(ws.w.gui_m, 1ui32);
				else
					ico.set_frame(ws.w.gui_m, 2ui32);
			}
		}
	}

	void province_owner_name::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			if(is_valid_index(owner)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations[owner].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}

	void country_status::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			if(is_valid_index(owner)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, nations::get_nation_status_text(ws, ws.w.nation_s.nations[owner]), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}


	void country_gov::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			if(is_valid_index(owner)) {
				if(auto gov = ws.w.nation_s.nations[owner].current_government; is_valid_index(gov)) {
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.governments_m.governments_container[gov].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					lm.finish_current_line();
				}
			}
		}
	}

	void country_party::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov);
			if(is_valid_index(owner)) {
				if(auto party = ws.w.nation_s.nations[owner].ruling_party; is_valid_index(party)) {
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.governments_m.parties[party].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
					lm.finish_current_line();
				}
			}
		}
	}

	void score_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, ws.w.nation_s.nations[owner].military_score + ws.w.nation_s.nations[owner].industrial_score + int32_t(nations::get_prestige(ws.w.nation_s.nations[owner])));

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	void rank_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, ws.w.nation_s.nations[owner].overall_rank);

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	void prestige_score_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, int32_t(nations::get_prestige(ws.w.nation_s.nations[owner])));

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	void prestige_rank_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, int32_t(ws.w.nation_s.nations[owner].prestige_rank));

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	void industrial_score_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, int32_t(ws.w.nation_s.nations[owner].industrial_score));

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	void industrial_rank_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, int32_t(ws.w.nation_s.nations[owner].industrial_rank));

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	void military_score_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, int32_t(ws.w.nation_s.nations[owner].military_score));

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	void military_rank_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				char16_t formatted_value[64];
				put_value_in_buffer(formatted_value, display_type::integer, int32_t(ws.w.nation_s.nations[owner].military_rank));

				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(formatted_value), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}

	void prestige_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::prestige], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void industrial_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::industrial_power], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void military_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::xy_pair cursor{ 0,0 };
		cursor = ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::military_power], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				if((ws.w.nation_s.nations[owner].flags & nations::nation::is_mobilized) != 0) {
					cursor = ui::advance_cursor_to_newline(cursor, ws.s.gui_m, ui::tooltip_text_format);
					ui::add_linear_text(cursor, ws.s.fixed_ui_text[scenario::fixed_ui::is_mobilized], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
				}
			}
		}
	}

	void military_icon::update(ui::dynamic_icon<military_icon>& ico, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				if((ws.w.nation_s.nations[owner].flags & nations::nation::is_mobilized) != 0)
					ico.set_frame(ws.w.gui_m, 1ui32);
				else
					ico.set_frame(ws.w.gui_m, 0ui32);
			}
		}
	}


	void sphere_label::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				if(ws.w.nation_s.nations[owner].sphere_leader == nullptr)
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::sphere], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				else
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			}
			lm.finish_current_line();
		}
	}


	void puppet_label::update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				if(ws.w.nation_s.nations[owner].overlord == nullptr)
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::puppets], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				else
					ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::liege], fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			}
			lm.finish_current_line();
		}
	}

	void send_diplomat::button_function(ui::simple_button<send_diplomat>&, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container.get<province_state::owner>(selected_prov); is_valid_index(owner)) {
				ws.w.diplomacy_w.show_diplomacy_window(ws.w.gui_m, owner);
			}
		}
	}

	void fort_level_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::fort], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void fort_level_icon::update(ui::dynamic_icon<fort_level_icon>& ico, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			ico.set_frame(ws.w.gui_m, ws.w.province_s.province_state_container.get<province_state::fort_level>(selected_prov));
		}
	}

	void naval_base_level_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::naval_base], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void naval_base_level_icon::update(ui::dynamic_icon<naval_base_level_icon>& ico, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			ico.set_frame(ws.w.gui_m, ws.w.province_s.province_state_container.get<province_state::naval_base_level>(selected_prov));
		}
	}

	void railroad_level_icon::create_tooltip(world_state& ws, ui::tagged_gui_object tw) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.fixed_ui_text[scenario::fixed_ui::railroad], ui::tooltip_text_format, ws.s.gui_m, ws.w.gui_m, tw);
	}

	void railroad_level_icon::update(ui::dynamic_icon<railroad_level_icon>& ico, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			ico.set_frame(ws.w.gui_m, ws.w.province_s.province_state_container.get<province_state::railroad_level>(selected_prov));
		}
	}

	void foreign_invest_railroad::update(ui::simple_button<foreign_invest_railroad>&, world_state&) {}
	void foreign_invest_railroad::button_function(ui::simple_button<foreign_invest_railroad>&, world_state&) {}
	void foreign_invest_railroad::create_tooltip(world_state&, ui::tagged_gui_object) {}

	void foreign_invest_factory::update(ui::simple_button<foreign_invest_factory>&, world_state&) {}
	void foreign_invest_factory::button_function(ui::simple_button<foreign_invest_factory>&, world_state&) {}
	void foreign_invest_factory::create_tooltip(world_state&, ui::tagged_gui_object) {}

	
	void infrastructure_progress_bar::update(ui::progress_bar<infrastructure_progress_bar>&, world_state&) {}

	void build_army_button::update(ui::simple_button<build_army_button>& ico, world_state&) {
		ico.set_enabled(false);
	}

	void build_navy_button::update(ui::simple_button<build_navy_button>& ico, world_state&) {
		ico.set_enabled(false);
	}

	void fort_expand_button::button_function(ui::button<fort_expand_button>&, key_modifiers m, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				if((m & modifiers_shift) != 0) {
					if(auto sid = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);
						ws.w.nation_s.states.is_valid_index(sid)) {
						
						ws.w.pending_commands.add<commands::province_building>(player->id, commands::province_building_type::state_fort, sid);
						
					}
				} else {
					ws.w.pending_commands.add<commands::province_building>(player->id, commands::province_building_type::province_fort, selected_prov);
				}
			}
		}
	}

	void fort_expand_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				ui::unlimited_line_manager lm;
				commands::explain_command_conditions(commands::province_building(player->id, commands::province_building_type::province_fort, selected_prov),
					ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format);
			}
		}
	}

	void naval_base_expand_button::button_function(ui::simple_button<naval_base_expand_button>&, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				ws.w.pending_commands.add<commands::province_building>(player->id, commands::province_building_type::province_naval_base, selected_prov);
			}
		}
	}

	void naval_base_expand_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				ui::unlimited_line_manager lm;
				commands::explain_command_conditions(commands::province_building(player->id, commands::province_building_type::province_naval_base, selected_prov),
					ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format);
			}
		}
	}

	void railroad_expand_button::button_function(ui::button<railroad_expand_button>&, key_modifiers m, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				if((m & modifiers_shift) != 0) {
					if(auto sid = ws.w.province_s.province_state_container.get<province_state::state_instance>(selected_prov);
						ws.w.nation_s.states.is_valid_index(sid)) {
						
						ws.w.pending_commands.add<commands::province_building>(player->id, commands::province_building_type::state_railroad, sid);
					}
				} else {
					ws.w.pending_commands.add<commands::province_building>(player->id, commands::province_building_type::province_railroad, selected_prov);
				}
			}
		}
	}

	void railroad_expand_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				ui::unlimited_line_manager lm;
				commands::explain_command_conditions(commands::province_building(player->id, commands::province_building_type::province_railroad, selected_prov),
					ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format);
			}
		}
	}

	void fort_expand_button::update(ui::button<fort_expand_button>& self, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				self.set_enabled(
					commands::is_command_valid(commands::province_building(player->id, commands::province_building_type::province_fort, selected_prov), ws));
			}
		}
	}
	void naval_base_expand_button::update(ui::simple_button<naval_base_expand_button>& self, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				self.set_enabled(
					commands::is_command_valid(commands::province_building(player->id, commands::province_building_type::province_naval_base, selected_prov), ws));
			}
		}
	}
	void railroad_expand_button::update(ui::button<railroad_expand_button>& self, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto player = ws.w.local_player_nation; player) {
				self.set_enabled(
					commands::is_command_valid(commands::province_building(player->id, commands::province_building_type::province_railroad, selected_prov), ws));
			}
		}
	}

	province_window::province_window() : win(std::make_unique<province_window_t>()) {}

	province_window::~province_window() {}

	void province_window::hide_province_window(ui::gui_manager & gui_m) {
		auto gobj = win->associated_object;
		if(gobj)
			ui::hide(*gobj);
	}

	void province_window::show_province_window(ui::gui_manager & gui_m, provinces::province_tag p) {
		selected_province = p;
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}

	void province_window::init_province_window(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["province_view"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
}
