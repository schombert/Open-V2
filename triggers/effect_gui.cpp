#include "common\\common.h"
#include "trigger_gui.h"
#include "gui\\gui.hpp"
#include "effects.h"
#include "codes.h"
#include "world_state\\world_state.h"
#include <random>
#include "population\\population_function.h"
#include "nations\\nations_functions.hpp"
#include "modifiers\\modifiers_gui.h"
#include "technologies\\technologies_functions.h"
#include "provinces\\province_functions.h"
#undef small

#pragma warning( push )
#pragma warning( disable : 4189)

namespace triggers {

	namespace {
		ui::xy_pair _make_effect_description(
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt,
			uint16_t const* tval,
			const_parameter primary_slot,
			const_parameter this_slot,
			const_parameter from_slot,
			jsf_prng& generator_copy,
			bool show_condition
		);

		inline ui::xy_pair display_subeffects(
			uint16_t const* source,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt,
			const_parameter primary_slot,
			const_parameter this_slot,
			const_parameter from_slot,
			jsf_prng& gen,
			bool show_condition) {

			//lm.increase_indent(1);

			const auto source_size = 1 + get_effect_payload_size(source);
			auto sub_units_start = source + 2 + effect_scope_data_payload(source[0]);

			while(sub_units_start < source + source_size) {
				cursor_in = _make_effect_description(
					ws,
					container,
					cursor_in,
					lm,
					fmt,
					sub_units_start,
					primary_slot,
					this_slot,
					from_slot,
					gen,
					show_condition
				);

				sub_units_start += 1 + get_effect_payload_size(sub_units_start);
			}

			//lm.decrease_indent(1);

			return cursor_in;
		}

		template<typename value_type>
		ui::xy_pair display_value(value_type value, display_type d_type, bool positive_is_green, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			char16_t local_buffer[32];
			ui::text_format local_fmt{ (value < value_type(0)) == positive_is_green ? ui::text_color::red : ui::text_color::green, fmt.font_handle, fmt.font_size };
			if(value < value_type(0))
				local_buffer[0] = u'-';
			else
				local_buffer[0] = u'+';

			put_pos_value_in_buffer(local_buffer + 1, d_type, std::abs(value));
			cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer),
				container, cursor_in, local_fmt, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return cursor_in;
		}


		template<typename value_type>
		ui::xy_pair display_value_wo_newline(value_type value, uint32_t label_index, bool positive_is_green, display_type d_type, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			cursor_in = display_value(value, d_type, positive_is_green, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[label_index], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return cursor_in;
		}

		template<typename value_type>
		ui::xy_pair display_value(value_type value, uint32_t label_index, bool positive_is_green, display_type d_type, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			cursor_in = display_value_wo_newline(value, label_index, positive_is_green, d_type, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		ui::xy_pair fill_text_effect(uint32_t ui_id, text_data::text_tag tex, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, tex),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[ui_id], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		ui::xy_pair fill_text_effect_wo_newline(uint32_t ui_id, text_data::text_tag tex, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, tex),
					[](ui::tagged_gui_object) {} }
			};
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[ui_id], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			return cursor_in;
		}

		ui::xy_pair tag_type_effect(uint32_t ui_id, uint16_t tval, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			auto tag = trigger_payload(tval).tag;
			auto hname = ws.w.culture_s.national_tags_state[tag].holder ?
				ws.w.nation_s.nations.get<nation::name>(ws.w.culture_s.national_tags_state[tag].holder) :
				ws.s.culture_m.national_tags[tag].default_name.name;
			auto hadj = ws.w.culture_s.national_tags_state[tag].holder ?
				ws.w.nation_s.nations.get<nation::adjective>(ws.w.culture_s.national_tags_state[tag].holder) :
				ws.s.culture_m.national_tags[tag].default_name.adjective;
			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::adj,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hadj),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[ui_id], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair tag_type_rebel_slot_effect(uint32_t ui_id, population::rebel_faction_tag rebel_id, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			cultures::national_tag rtag = bool(rebel_id) ? ws.w.population_s.rebel_factions[rebel_id].independence_tag : cultures::national_tag();
			auto rtag_name = is_valid_index(rtag) ?
				(bool(ws.w.culture_s.national_tags_state[rtag].holder) ? ws.w.nation_s.nations.get<nation::name>(ws.w.culture_s.national_tags_state[rtag].holder) : ws.s.culture_m.national_tags[rtag].default_name.name)
				: ws.s.fixed_ui_text[scenario::fixed_ui::rebel];
			auto rtag_adj = is_valid_index(rtag) ?
				(bool(ws.w.culture_s.national_tags_state[rtag].holder) ? ws.w.nation_s.nations.get<nation::adjective>(ws.w.culture_s.national_tags_state[rtag].holder) : ws.s.culture_m.national_tags[rtag].default_name.adjective)
				: ws.s.fixed_ui_text[scenario::fixed_ui::rebel];

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, rtag_name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::adj,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, rtag_adj),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[ui_id], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair tag_type_this_nation_effect(uint32_t ui_id, const_parameter this_slot, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences,
						bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation]),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::adj,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences,
						bool(to_nation(this_slot)) ? ws.w.nation_s.nations.get<nation::adjective>(to_nation(this_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation]),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[ui_id], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair tag_type_this_state_effect(uint32_t ui_id, const_parameter this_slot, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
			return tag_type_this_nation_effect(ui_id, bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair tag_type_this_province_effect(uint32_t ui_id, const_parameter this_slot, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
			return tag_type_this_nation_effect(ui_id, is_valid_index(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair tag_type_this_pop_effect(uint32_t ui_id, const_parameter this_slot, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
			return tag_type_this_nation_effect(ui_id, bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair tag_type_from_nation_effect(uint32_t ui_id, const_parameter from_slot, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences,
						bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation]),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::adj,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences,
						bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::adjective>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation]),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[ui_id], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair tag_type_from_province_effect(uint32_t ui_id, const_parameter from_slot, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {
			return tag_type_from_nation_effect(ui_id, is_valid_index(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag(), ws, container, cursor_in, lm, fmt);
		}

#define EFFECT_DISPLAY_PARAMS uint16_t const* tval, world_state& ws, ui::tagged_gui_object container, \
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, const_parameter primary_slot, \
			const_parameter this_slot, const_parameter from_slot, jsf_prng& gen

		ui::xy_pair es_generic_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return display_subeffects(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, gen, show_condition);
		}

		inline uint32_t random_or_every(uint16_t tval) {
			return (tval & effect_codes::is_random_scope) != 0 ? scenario::fixed_ui::random : scenario::fixed_ui::every;
		}

		ui::xy_pair es_x_neighbor_province_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition && is_valid_index(to_prov(primary_slot))) {
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(to_prov(primary_slot));
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p))
							&& test_trigger(limit, ws, p, this_slot, from_slot)) {
							rlist.push_back(p);
						}
					}
				} else {
					for(auto p : neighbor_range) {
						if(is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p)))
							rlist.push_back(p);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::neighboring_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::neighboring_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_neighbor_country_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition && to_nation(primary_slot)) {
				auto neighbor_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations.get<nation::neighboring_nations>(to_nation(primary_slot)));

				boost::container::small_vector<nations::country_tag, 16, concurrent_allocator<nations::country_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n : neighbor_range) {
						if(test_trigger(limit, ws, n, this_slot, from_slot))
							rlist.push_back(n);
					}
				} else {
					for(auto n : neighbor_range)
						rlist.push_back(n);
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::neighboring_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::neighboring_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}


			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_country_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition && to_nation(primary_slot)) {
				boost::container::small_vector<nations::country_tag, 64, concurrent_allocator<nations::country_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					ws.w.nation_s.nations.for_each([&rlist, limit, &ws, primary_slot, this_slot, from_slot](nations::country_tag n) {
						if(n != to_nation(primary_slot) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)) && test_trigger(limit, ws, n, this_slot, from_slot))
							rlist.push_back(n);
					});
				} else {
					ws.w.nation_s.nations.for_each([&rlist, &ws, primary_slot](nations::country_tag n) {
						if(n != to_nation(primary_slot) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(n)))
							rlist.push_back(n);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_country_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_x_country_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_x_empty_neighbor_province_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition && bool(to_prov(primary_slot))) {
				auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(to_prov(primary_slot));
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p))
							&& test_trigger(limit, ws, p, this_slot, from_slot)) {
							rlist.push_back(p);
						}
					}
				} else {
					for(auto p : neighbor_range) {
						if(!is_valid_index(ws.w.province_s.province_state_container.get<province_state::owner>(p)))
							rlist.push_back(p);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::empty_neighboring_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::empty_neighboring_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_greater_power_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition) {
				auto ranked_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
				int32_t great_nations_count = int32_t(ws.s.modifiers_m.global_defines.great_nations_count);
				int32_t count = 0;

				boost::container::small_vector<nations::country_tag, 16, concurrent_allocator<nations::country_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							if(nations::is_great_power(ws, *n) && test_trigger(limit, ws, *n, this_slot, from_slot)) {
								++count;
								rlist.push_back(*n);
							}
						}
					}
				} else {
					for(auto n = std::begin(ranked_range); (n != std::end(ranked_range)) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							if(nations::is_great_power(ws, *n)) {
								++count;
								rlist.push_back(*n);
							}
						}
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::great_power], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::great_power], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}


			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_poor_strata_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::random], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				gen.advance_n<1>();
			} else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::every], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::poor_strata_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_poor_strata_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_poor_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_poor_strata_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_poor_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_middle_strata_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::random], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				gen.advance_n<1>();
			} else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::every], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::middle_strata_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_middle_strata_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_middle_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_middle_strata_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_middle_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_rich_strata_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::random], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				gen.advance_n<1>();
			} else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::every], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::rich_strata_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_rich_strata_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_rich_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_rich_strata_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_rich_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_x_pop_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::random], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				gen.advance_n<1>();
			} else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::every], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_pop_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_x_pop_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_x_pop_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_x_pop_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, show_condition);
		}
		ui::xy_pair es_x_owned_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition && bool(to_nation(primary_slot))) {
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, to_nation(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](provinces::province_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_province(ws, to_nation(primary_slot), [&rlist](provinces::province_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::great_power], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owned_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(to_nation(primary_slot))
					cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_owned_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition && bool(to_state(primary_slot))) {
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, to_state(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](provinces::province_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_province(ws, to_state(primary_slot), [&rlist](provinces::province_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::great_power], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.states.get<state::name>(to_state(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owned_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(to_state(primary_slot))
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.states.get<state::name>(to_state(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_core_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cultures::national_tag tag = bool(to_nation(primary_slot)) ? ws.w.nation_s.nations.get<nation::tag>(to_nation(primary_slot)) : cultures::national_tag();
			if(is_valid_index(tag) && (tval[0] & effect_codes::is_random_scope) != 0) {
				auto cores_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);
				boost::container::small_vector<provinces::province_tag, 16, concurrent_allocator<provinces::province_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : cores_range) {
						if(is_valid_index(p)) {
							if(test_trigger(limit, ws, p, this_slot, from_slot))
								rlist.push_back(p);
						}
					}
				} else {
					for(auto p : cores_range) {
						if(is_valid_index(p))
							rlist.push_back(p);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::core_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}

			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::core_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(to_nation(primary_slot))
					cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}


			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_state_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && show_condition && bool(to_nation(primary_slot))) {
				boost::container::small_vector<nations::state_tag, 16, concurrent_allocator<nations::state_tag>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_state(ws, to_nation(primary_slot), [&ws, &rlist, limit, this_slot, from_slot](nations::state_tag pobj) {
						if(test_trigger(limit, ws, pobj, this_slot, from_slot))
							rlist.push_back(pobj);
					});
				} else {
					nations::for_each_state(ws, to_nation(primary_slot), [&rlist](nations::state_tag pobj) {
						rlist.push_back(pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.states.get<state::name>(rlist[uint32_t(dist(gen))]), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::state_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::state_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(bool(to_nation(primary_slot)))
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_random_list_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::one_of_the_following], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			const auto source_size = 1 + get_effect_payload_size(tval);
			auto chances_total = tval[2];

			std::uniform_int_distribution<int32_t> dist(0, int32_t(chances_total) - 1);

			auto chance_taken = dist(gen);

			auto sub_units_start = tval + 3; // [code] + [payload size] + [chances total] + [first sub effect chance]
			while(sub_units_start < tval + source_size) {
				char16_t local_buffer[16];
				auto bend = put_value_in_buffer(local_buffer, display_type::percent, float(*sub_units_start) / float(chances_total));
				*bend = u':';
				*(bend + 1) = u' ';
				*(bend + 2) = char16_t(0);
				cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);

				cursor_in = _make_effect_description(ws, container, cursor_in, lm, fmt,
					sub_units_start + 1, primary_slot, this_slot, from_slot, gen, show_condition);
				sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1); // each member preceeded by uint16_t
			}

			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_random_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			auto chance = tval[2];
			std::uniform_int_distribution<int32_t> dist(0, 99);
			auto result = dist(gen);

			char16_t local_buffer[16];
			auto bend = put_value_in_buffer(local_buffer, display_type::percent, float(chance) / float(100.0));
			*bend = u':';
			*(bend + 1) = u' ';
			*(bend + 2) = char16_t(0);
			cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), container, cursor_in, fmt, lm);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::chance_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, gen, show_condition);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_owner_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && bool(to_state(primary_slot))) {
				cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.states.get<state::name>(to_state(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto owner = nations::state_owner(ws, to_state(primary_slot));
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_owner_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && bool(to_prov(primary_slot))) {
				cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto owner = provinces::province_owner(ws, to_prov(primary_slot));
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_controller_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::controller_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && bool(to_prov(primary_slot))) {
				cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto controller = provinces::province_controller(ws, to_prov(primary_slot));
				if(controller)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, controller, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_location_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::location_of_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(show_condition && bool(to_pop(primary_slot))) {
				auto location = ws.w.population_s.pops.get<pop::location>(to_pop(primary_slot));
				if(is_valid_index(location))
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, location, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, provinces::province_tag(), this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, provinces::province_tag(), this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_country_scope_pop(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation_of_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(show_condition && to_pop(primary_slot)) {
				auto owner = population::get_pop_owner(ws, to_pop(primary_slot));
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_country_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && to_state(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.states.get<state::name>(to_state(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto owner = nations::state_owner(ws, to_state(primary_slot));
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_capital_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::capital_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && to_nation(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto capital = ws.w.nation_s.nations.get<nation::current_capital>(to_nation(primary_slot));
				if(is_valid_index(capital))
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, capital, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_this_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if(to_nation(this_slot))
				cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_this_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if(to_state(this_slot))
				cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.states.get<state::name>(to_state(this_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_this_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if(bool(to_prov(this_slot)))
				cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(to_prov(this_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_this_scope_pop(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if(to_nation(from_slot))
				cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if(to_nation(from_slot))
				cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.states.get<state::name>(from_slot.state), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if(bool(to_prov(from_slot)))
				cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(to_prov(from_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_pop(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, gen, true);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_sea_zone_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if(show_condition && bool(to_prov(primary_slot))) {
				auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(to_prov(primary_slot));
				if(sea_zones.first != sea_zones.second) {
					cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(*sea_zones.first), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();

					lm.increase_indent(1);
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, *sea_zones.first, this_slot, from_slot, gen, true);
					lm.decrease_indent(1);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::adjacent_sea], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, provinces::province_tag(), this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_cultural_union_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::cultural_union_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && to_nation(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_overlord_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::overlord_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && to_nation(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto overlord = ws.w.nation_s.nations.get<nation::overlord>(to_nation(primary_slot));
				if(overlord)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, overlord, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_sphere_owner_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && to_nation(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in,ws.w.nation_s.nations.get<nation::name>(to_nation(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto overlord = ws.w.nation_s.nations.get<nation::sphere_leader>(to_nation(primary_slot));
				if(overlord)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, overlord, this_slot, from_slot, gen, true);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_independence_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reb_independence_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_flashpoint_tag_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::flashpoint_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_crisis_state_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_state_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::containing_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(show_condition && bool(to_prov(primary_slot))) {
				cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);

				auto si = provinces::province_state(ws, to_prov(primary_slot));
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, si, this_slot, from_slot, gen, bool(si));

				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_state_scope_pop(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::containing_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_tag_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto tag = trigger_payload(tval[3]).tag;

				if(auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder; tag_holder) {
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(tag_holder), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.culture_m.national_tags[tag].default_name.name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}

				lm.increase_indent(1);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);


				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				auto tag = trigger_payload(tval[2]).tag;
				if(auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder; tag_holder) {
					cursor_in = ui::add_linear_text(cursor_in, ws.w.nation_s.nations.get<nation::name>(tag_holder), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.culture_m.national_tags[tag].default_name.name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}
			return cursor_in;
		}
		ui::xy_pair es_integer_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				provinces::province_tag ptag(tval[3]);

				cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(ptag), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			} else {
				provinces::province_tag ptag(tval[2]);

				cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(ptag), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
				lm.decrease_indent(1);
			}
			return cursor_in;
		}
		ui::xy_pair es_pop_type_scope_nation(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			if((tval[0] & effect_codes::is_random_scope) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::random], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				gen.advance_n<1>();
			} else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::every], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);

			auto type = ((tval[0] & effect_codes::scope_has_limit) != 0) ? trigger_payload(tval[3]).small.values.pop_type : trigger_payload(tval[2]).small.values.pop_type;

			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.population_m.pop_types[type].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_pop_type_scope_state(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_pop_type_scope_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, gen, false);
		}
		ui::xy_pair es_pop_type_scope_province(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			return es_pop_type_scope_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, gen, false);
		}
		ui::xy_pair es_region_scope(EFFECT_DISPLAY_PARAMS, bool show_condition) {
			auto region = ((tval[0] & effect_codes::scope_has_limit) != 0) ? trigger_payload(tval[3]).state : trigger_payload(tval[2]).state;

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::every], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::province_in], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.province_m.state_names[region], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, false);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, gen, false);
			lm.decrease_indent(1);

			return cursor_in;
		}

		static ui::xy_pair(*effect_scope_functions[])(uint16_t const*, world_state&, ui::tagged_gui_object, ui::xy_pair, ui::unlimited_line_manager&,
			ui::text_format const&, const_parameter, const_parameter, const_parameter, jsf_prng&, bool) = {
			es_generic_scope,
			es_x_neighbor_province_scope ,
			es_x_neighbor_country_scope ,
			es_x_country_scope ,
			es_x_country_scope_nation,
			es_x_empty_neighbor_province_scope,
			es_x_greater_power_scope ,
			es_poor_strata_scope_nation ,
			es_poor_strata_scope_state ,
			es_poor_strata_scope_province ,
			es_middle_strata_scope_nation,
			es_middle_strata_scope_state ,
			es_middle_strata_scope_province,
			es_rich_strata_scope_nation,
			es_rich_strata_scope_state ,
			es_rich_strata_scope_province ,
			es_x_pop_scope_nation ,
			es_x_pop_scope_state ,
			es_x_pop_scope_province,
			es_x_owned_scope_nation ,
			es_x_owned_scope_state,
			es_x_core_scope ,
			es_x_state_scope,
			es_random_list_scope,
			es_random_scope ,
			es_owner_scope_state ,
			es_owner_scope_province,
			es_controller_scope,
			es_location_scope,
			es_country_scope_pop ,
			es_country_scope_state,
			es_capital_scope ,
			es_this_scope_nation ,
			es_this_scope_state ,
			es_this_scope_province ,
			es_this_scope_pop ,
			es_from_scope_nation ,
			es_from_scope_state,
			es_from_scope_province ,
			es_from_scope_pop ,
			es_sea_zone_scope ,
			es_cultural_union_scope ,
			es_overlord_scope ,
			es_sphere_owner_scope ,
			es_independence_scope ,
			es_flashpoint_tag_scope,
			es_crisis_state_scope,
			es_state_scope_pop ,
			es_state_scope_province,
			es_tag_scope,
			es_integer_scope ,
			es_pop_type_scope_nation ,
			es_pop_type_scope_state,
			es_pop_type_scope_province ,
			es_region_scope,
		};

		/*
		#define EFFECT_DISPLAY_PARAMS uint16_t const* tval, world_state& ws, ui::tagged_gui_object container, \
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, const_parameter primary_slot, \
			const_parameter this_slot, const_parameter from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen
		*/

		ui::xy_pair ef_none(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_capital(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag new_capital(tval[2]);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::move_capital_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(new_capital), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_core_tag(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::add_x_core, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_core_int(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_core_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(prov), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_core_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::add_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_core_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::add_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_core_this_state(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_state_effect(scenario::fixed_ui::add_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_core_this_pop(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_pop_effect(scenario::fixed_ui::add_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_core_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::add_x_core, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_core_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::add_x_core, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_core_reb(EFFECT_DISPLAY_PARAMS) {
			return tag_type_rebel_slot_effect(scenario::fixed_ui::add_x_core, to_rebel(from_slot), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_tag(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::remove_x_core, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_int(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_core_from], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container.get<province_state::name>(prov), fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_core_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::remove_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::remove_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_this_state(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_state_effect(scenario::fixed_ui::remove_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_this_pop(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_pop_effect(scenario::fixed_ui::remove_x_core, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::remove_x_core, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::remove_x_core, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_core_reb(EFFECT_DISPLAY_PARAMS) {
			return tag_type_rebel_slot_effect(scenario::fixed_ui::remove_x_core, to_rebel(from_slot), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_change_region_name_state(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, trigger_payload(tval[2]).text),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_name_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_change_region_name_province(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, trigger_payload(tval[2]).text),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_name_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_trade_goods(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.economy_m.goods[trigger_payload(tval[2]).small.values.good].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_rgo_production_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_accepted_culture(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[trigger_payload(tval[2]).culture].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_accepted_culture], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_accepted_culture_union(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::union_culture_accepted], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_primary_culture(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[trigger_payload(tval[2]).culture].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::primary_culture_changes_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_primary_culture_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(this_slot)) {
				auto c = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(this_slot));
				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[c].name),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::primary_culture_changes_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			} else {
				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation_culture]),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::primary_culture_changes_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			}
		}
		ui::xy_pair ef_primary_culture_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_primary_culture_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_primary_culture_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_primary_culture_from_nation(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(from_slot)) {
				auto c = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(from_slot));
				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[c].name),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::primary_culture_changes_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			} else {
				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::from_nation_culture]),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::primary_culture_changes_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			}
		}
		ui::xy_pair ef_remove_accepted_culture(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.culture_container[trigger_payload(tval[2]).culture].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_accepted_culture], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_life_rating(EFFECT_DISPLAY_PARAMS) {
			return display_value(trigger_payload(tval[2]).signed_value, scenario::fixed_ui::life_rating, true,
				display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_religion(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.religions[trigger_payload(tval[2]).small.values.religion].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_national_religion], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_is_slave_state_yes(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_slave_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_is_slave_pop_yes(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_slave_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_research_points(EFFECT_DISPLAY_PARAMS) {
			return display_value(trigger_payload(tval[2]).signed_value, scenario::fixed_ui::research_points, true,
				display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_tech_school(EFFECT_DISPLAY_PARAMS) {
			auto school = trigger_payload(tval[2]).nat_mod;

			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.national_modifiers[school].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_tech_school], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[school]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_government(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.governments_m.governments_container[trigger_payload(tval[2]).small.values.government].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_government_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_government_reb(EFFECT_DISPLAY_PARAMS) {
			if(to_rebel(from_slot) && to_nation(primary_slot)) {
				auto gov = ws.s.population_m.rebel_change_government_to.get(ws.w.population_s.rebel_factions[to_rebel(from_slot)].type, ws.w.nation_s.nations.get<nation::current_government>(to_nation(primary_slot)));

				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.governments_m.governments_container[gov].name),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_government_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			} else {
				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::rebel]),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_government_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			}
		}
		ui::xy_pair ef_treasury(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2), scenario::fixed_ui::add_to_treasury, true,
				display_type::currency, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_war_exhaustion(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2), scenario::fixed_ui::war_exhaustion, false,
				display_type::percent, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_prestige(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(primary_slot)) {
				auto amount = read_float_from_payload(tval + 2);
				if(amount >= 0.0f)
					return display_value(amount * (ws.w.nation_s.tech_attributes.get<technologies::tech_offset::prestige>(to_nation(primary_slot)) + 1.0f),
						scenario::fixed_ui::prestige, true,
						display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
				else
					return display_value(amount,
						scenario::fixed_ui::prestige, true,
						display_type::integer, ws, container, cursor_in, lm, fmt);
			} else {
				return display_value(read_float_from_payload(tval + 2), scenario::fixed_ui::prestige, true,
					display_type::integer, ws, container, cursor_in, lm, fmt);
			}
		}
		ui::xy_pair ef_change_tag(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name),
					[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::become_blank], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_change_tag_culture(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(primary_slot)) {
				auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
				if(is_valid_index(prim_culture)) {
					auto cg_t = ws.s.culture_m.culture_container[prim_culture].group;
					auto u = ws.s.culture_m.culture_groups[cg_t].union_tag;
					if(is_valid_index(u)) {
						text_data::replacement repl{
							text_data::value_type::text,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.national_tags[u].default_name.name),
							[](ui::tagged_gui_object) {} };

						cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::become_blank], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
						cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
						lm.finish_current_line();
						return cursor_in;
					}
				}
			}
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::cultural_union_nation]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::become_blank], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_change_tag_no_core_switch(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name),
					[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::player_control_change], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_change_tag_no_core_switch_culture(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(primary_slot)) {
				auto prim_culture = ws.w.nation_s.nations.get<nation::primary_culture>(to_nation(primary_slot));
				if(is_valid_index(prim_culture)) {
					auto cg_t = ws.s.culture_m.culture_container[prim_culture].group;
					auto u = ws.s.culture_m.culture_groups[cg_t].union_tag;
					if(is_valid_index(u)) {
						text_data::replacement repl{
							text_data::value_type::text,
							text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.culture_m.national_tags[u].default_name.name),
							[](ui::tagged_gui_object) {} };

						cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::player_control_change], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
						cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
						lm.finish_current_line();
						return cursor_in;
					}
				}
			}
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::cultural_union_nation]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::player_control_change], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_set_country_flag(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.variables_m.national_flag_to_name[trigger_payload(tval[2]).nat_flag]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::set_national_flag], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_clr_country_flag(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.variables_m.national_flag_to_name[trigger_payload(tval[2]).nat_flag]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_national_flag], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_military_access(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_military_access_this_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_military_access_this_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_military_access_from_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_military_access_from_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_badboy(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2), scenario::fixed_ui::infamy, false,
				display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::change_province_owner, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::change_province_owner, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province_this_state(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_state_effect(scenario::fixed_ui::change_province_owner, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::change_province_owner, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province_this_pop(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_pop_effect(scenario::fixed_ui::change_province_owner, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::change_province_owner, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::change_province_owner, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_secede_province_reb(EFFECT_DISPLAY_PARAMS) {
			return tag_type_rebel_slot_effect(scenario::fixed_ui::change_province_owner, to_rebel(from_slot), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_inherit(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::annex_effect, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_inherit_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::annex_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_inherit_this_state(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_state_effect(scenario::fixed_ui::annex_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_inherit_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::annex_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_inherit_this_pop(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_pop_effect(scenario::fixed_ui::annex_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_inherit_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::annex_effect, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_inherit_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::annex_effect, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_annex_to(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::annexed_by_effect, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_annex_to_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::annexed_by_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_annex_to_this_state(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_state_effect(scenario::fixed_ui::annexed_by_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_annex_to_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::annexed_by_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_annex_to_this_pop(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_pop_effect(scenario::fixed_ui::annexed_by_effect, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_annex_to_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::annexed_by_effect, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_annex_to_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::annexed_by_effect, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::core_return, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::core_return_short, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_this_state(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_state_effect(scenario::fixed_ui::core_return_short, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::core_return_short, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_this_pop(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_pop_effect(scenario::fixed_ui::core_return_short, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::core_return_short, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::core_return_short, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_change_controller(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::change_province_controller, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_change_controller_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::change_province_controller, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_change_controller_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::change_province_controller, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_change_controller_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::change_province_controller, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_change_controller_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::change_province_controller, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_infrastructure(EFFECT_DISPLAY_PARAMS) {
			return display_value(trigger_payload(tval[2]).signed_value, scenario::fixed_ui::railroad_level, true,
				display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_money(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2), scenario::fixed_ui::pop_savings, true,
				display_type::currency, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_leadership(EFFECT_DISPLAY_PARAMS) {
			return display_value(trigger_payload(tval[2]).signed_value, scenario::fixed_ui::leadership_points, true,
				display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_vassal(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::release_as_vassal, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_vassal_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::release_as_vassal, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_vassal_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::release_as_vassal, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_vassal_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::release_as_vassal, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_vassal_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::release_as_vassal, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_end_military_access(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_end_military_access_this_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_end_military_access_this_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_end_military_access_from_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_end_military_access_from_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_leave_alliance(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::end_alliance, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_leave_alliance_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::end_alliance, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_leave_alliance_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::end_alliance, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_leave_alliance_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::end_alliance, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_leave_alliance_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::end_alliance, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_end_war(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::end_war_with, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_end_war_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::end_war_with, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_end_war_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::end_war_with, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_end_war_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::end_war_with, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_end_war_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::end_war_with, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_enable_ideology(EFFECT_DISPLAY_PARAMS) {
			return fill_text_effect(scenario::fixed_ui::enable_blank, ws.s.ideologies_m.ideology_container[trigger_payload(tval[2]).small.values.ideology].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_ruling_party_ideology(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(primary_slot)) {
				if(auto id = to_nation(primary_slot); ws.w.nation_s.nations.is_valid_index(id)) {
					auto new_party = ws.w.nation_s.active_parties.get(id, trigger_payload(tval[2]).small.values.ideology);
					return fill_text_effect(scenario::fixed_ui::enable_blank, ws.s.governments_m.parties[new_party].name, ws, container, cursor_in, lm, fmt);
				}
			}
			return fill_text_effect(scenario::fixed_ui::enable_blank, ws.s.ideologies_m.ideology_container[trigger_payload(tval[2]).small.values.ideology].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_plurality(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2), scenario::fixed_ui::plurality, true,
				display_type::percent, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_remove_province_modifier(EFFECT_DISPLAY_PARAMS) {
			auto pmod = trigger_payload(tval[2]).prov_mod;

			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.provincial_modifiers[pmod].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_prov_mod], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_province_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.provincial_modifier_definitions[pmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_remove_country_modifier(EFFECT_DISPLAY_PARAMS) {
			auto nmod = trigger_payload(tval[2]).nat_mod;

			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.national_modifiers[nmod].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_nat_mod], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[nmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_create_alliance(EFFECT_DISPLAY_PARAMS) {
			return tag_type_effect(scenario::fixed_ui::make_alliance, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_alliance_this_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_nation_effect(scenario::fixed_ui::make_alliance, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_alliance_this_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_this_province_effect(scenario::fixed_ui::make_alliance, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_alliance_from_nation(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_nation_effect(scenario::fixed_ui::make_alliance, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_create_alliance_from_province(EFFECT_DISPLAY_PARAMS) {
			return tag_type_from_province_effect(scenario::fixed_ui::make_alliance, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_vassal(EFFECT_DISPLAY_PARAMS) {
			if(auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder; bool(holder) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(holder))) {
				return tag_type_effect(scenario::fixed_ui::become_independent, tval[2], ws, container, cursor_in, lm, fmt);
			} else {
				return tag_type_effect(scenario::fixed_ui::release_as_vassal, tval[2], ws, container, cursor_in, lm, fmt);
			}
		}
		ui::xy_pair ef_release_vassal_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(bool(to_nation(this_slot)) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(to_nation(this_slot))))
				return tag_type_this_nation_effect(scenario::fixed_ui::become_independent, this_slot, ws, container, cursor_in, lm, fmt);
			else
				return tag_type_this_nation_effect(scenario::fixed_ui::release_as_vassal, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_vassal_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_release_vassal_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_release_vassal_from_nation(EFFECT_DISPLAY_PARAMS) {
			if(bool(to_nation(from_slot)) && is_valid_index(ws.w.nation_s.nations.get<nation::current_capital>(to_nation(from_slot))))
				return tag_type_from_nation_effect(scenario::fixed_ui::become_independent, from_slot, ws, container, cursor_in, lm, fmt);
			else
				return tag_type_from_nation_effect(scenario::fixed_ui::release_as_vassal, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_vassal_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_release_vassal_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				nullptr, bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag(), gen);
		}
		ui::xy_pair ef_release_vassal_reb(EFFECT_DISPLAY_PARAMS) {
			return tag_type_rebel_slot_effect(scenario::fixed_ui::release_as_independent, to_rebel(from_slot), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_release_vassal_random(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_change_province_name(EFFECT_DISPLAY_PARAMS) {
			return fill_text_effect(scenario::fixed_ui::change_name_to, trigger_payload(tval[2]).text, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_enable_canal(EFFECT_DISPLAY_PARAMS) {
			return fill_text_effect(scenario::fixed_ui::enable_canal, std::get<2>(ws.s.province_m.canals[tval[2] - 1]), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_set_global_flag(EFFECT_DISPLAY_PARAMS) {
			return fill_text_effect(scenario::fixed_ui::set_global_flag, ws.s.variables_m.global_variable_to_name[trigger_payload(tval[2]).global_var],
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_clr_global_flag(EFFECT_DISPLAY_PARAMS) {
			return fill_text_effect(scenario::fixed_ui::remove_global_flag, ws.s.variables_m.global_variable_to_name[trigger_payload(tval[2]).global_var],
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_nationalvalue_province(EFFECT_DISPLAY_PARAMS) {
			auto nmod = trigger_payload(tval[2]).nat_mod;

			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.national_modifiers[nmod].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_national_value], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[nmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_nationalvalue_nation(EFFECT_DISPLAY_PARAMS) {
			auto nmod = trigger_payload(tval[2]).nat_mod;

			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.national_modifiers[nmod].name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_national_value], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[nmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_civilized_yes(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::become_civ], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_civilized_no(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::become_unciv], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_is_slave_state_no(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::free_slave_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_is_slave_pop_no(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::free_slave_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_election(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::hold_election], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_social_reform(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto iss = ws.s.issues_m.options[opt].parent_issue;

			text_data::replacement repl[2] = {
				text_data::replacement{
				text_data::value_type::issue,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.issues_container[iss].name),
				[](ui::tagged_gui_object) {} },
				text_data::replacement{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.options[opt].name),
				[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::issue_change], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_political_reform(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto iss = ws.s.issues_m.options[opt].parent_issue;

			text_data::replacement repl[2] = {
				text_data::replacement{
				text_data::value_type::issue,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.issues_container[iss].name),
				[](ui::tagged_gui_object) {} },
				text_data::replacement{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.options[opt].name),
				[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::issue_change], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_tax_relative_income(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(primary_slot)) {
				return display_value(ws.w.nation_s.nations.get<nation::tax_base>(to_nation(primary_slot)) * read_float_from_payload(tval + 2), scenario::fixed_ui::add_to_treasury, true,
					display_type::currency, ws, container, cursor_in, lm, fmt);
			} else {
				return display_value(read_float_from_payload(tval + 2), scenario::fixed_ui::add_relative_income, true,
					display_type::fp_two_places, ws, container, cursor_in, lm, fmt);
			}
		}
		ui::xy_pair ef_neutrality(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_neutral], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_reduce_pop(EFFECT_DISPLAY_PARAMS) {
			return display_value(1.0f - read_float_from_payload(tval + 2), scenario::fixed_ui::pop_size, true,
				display_type::percent, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_move_pop(EFFECT_DISPLAY_PARAMS) {
			return fill_text_effect(scenario::fixed_ui::move_pop_to, ws.w.province_s.province_state_container.get<province_state::name>(provinces::province_tag(tval[2])), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_pop_type(EFFECT_DISPLAY_PARAMS) {
			return fill_text_effect(scenario::fixed_ui::change_pop_type, ws.s.population_m.pop_types[trigger_payload(tval[2]).small.values.pop_type].name,
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_years_of_research(EFFECT_DISPLAY_PARAMS) {
			if(to_nation(primary_slot)) {
				return display_value(technologies::daily_research_points(ws, to_nation(primary_slot)) * 365.0f * read_float_from_payload(tval + 2),
					scenario::fixed_ui::research_points, true, display_type::integer, ws, container, cursor_in, lm, fmt);
			} else {
				return display_value(read_float_from_payload(tval + 2),
					scenario::fixed_ui::years_of_research, true, display_type::fp_two_places, ws, container, cursor_in, lm, fmt);
			}
		}
		ui::xy_pair ef_prestige_factor_positive(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2),
				scenario::fixed_ui::prestige, true, display_type::percent, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_prestige_factor_negative(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2),
				scenario::fixed_ui::prestige, true, display_type::percent, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_military_reform(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto iss = ws.s.issues_m.options[opt].parent_issue;

			text_data::replacement repl[2] = {
				text_data::replacement{
				text_data::value_type::issue,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.issues_container[iss].name),
				[](ui::tagged_gui_object) {} },
				text_data::replacement{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.options[opt].name),
				[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::issue_change], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_economic_reform(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto iss = ws.s.issues_m.options[opt].parent_issue;

			text_data::replacement repl[2] = {
				text_data::replacement{
				text_data::value_type::issue,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.issues_container[iss].name),
				[](ui::tagged_gui_object) {} },
				text_data::replacement{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.options[opt].name),
				[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::issue_change], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_random_military_reforms(EFFECT_DISPLAY_PARAMS) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, tval[2]);

			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buf),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_mil_reforms], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_random_economic_reforms(EFFECT_DISPLAY_PARAMS) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, tval[2]);

			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buf),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_econ_reforms], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_crime(EFFECT_DISPLAY_PARAMS) {
			auto pmod = trigger_payload(tval[2]).prov_mod;
			return fill_text_effect(scenario::fixed_ui::add_crime, ws.s.modifiers_m.provincial_modifiers[pmod].name,
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_crime_none(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_crime], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_nationalize(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::perform_nationalization], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_factory_in_capital_state(EFFECT_DISPLAY_PARAMS) {
			auto f = trigger_payload(tval[2]).small.values.factory;
			return fill_text_effect(scenario::fixed_ui::build_factory_in_capital, ws.s.economy_m.factory_types[f].name,
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_activate_technology(EFFECT_DISPLAY_PARAMS) {
			auto t = trigger_payload(tval[2]).tech;
			return fill_text_effect(scenario::fixed_ui::enable_blank, ws.s.technology_m.technologies_container[t].name,
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_great_wars_enabled_yes(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::enable_great_wars], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_great_wars_enabled_no(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::disable_great_wars], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_world_wars_enabled_yes(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::enable_world_wars], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_world_wars_enabled_no(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::disable_world_wars], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_assimilate_province(EFFECT_DISPLAY_PARAMS) {
			if(to_prov(primary_slot)) {
				if(auto owner = provinces::province_owner(ws, to_prov(primary_slot)); owner) {
					if(auto c = ws.w.nation_s.nations.get<nation::primary_culture>(owner); is_valid_index(c)) {
						return fill_text_effect(scenario::fixed_ui::assimilate_province, ws.s.culture_m.culture_container[c].name,
							ws, container, cursor_in, lm, fmt);
					}
				}
			}
			return fill_text_effect(scenario::fixed_ui::assimilate_province, ws.s.fixed_ui_text[scenario::fixed_ui::owner_primary_culture],
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_assimilate_pop(EFFECT_DISPLAY_PARAMS) {
			if(to_pop(primary_slot)) {
				if(auto owner = population::get_pop_owner(ws, to_pop(primary_slot)); owner) {
					if(auto c = ws.w.nation_s.nations.get<nation::primary_culture>(owner); is_valid_index(c)) {
						return fill_text_effect(scenario::fixed_ui::assimilate_pop, ws.s.culture_m.culture_container[c].name,
							ws, container, cursor_in, lm, fmt);
					}
				}
			}
			return fill_text_effect(scenario::fixed_ui::assimilate_pop, ws.s.fixed_ui_text[scenario::fixed_ui::owner_primary_culture],
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_literacy(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2),
				scenario::fixed_ui::literacy, true, display_type::percent, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_crisis_interest(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_crisis_interest], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_flashpoint_tension(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2),
				scenario::fixed_ui::flashpoint_tension, false, display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_crisis_temperature(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2),
				scenario::fixed_ui::crisis_temperature_plain, false, display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_consciousness(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2),
				scenario::fixed_ui::consciousness, false, display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_militancy(EFFECT_DISPLAY_PARAMS) {
			return display_value(read_float_from_payload(tval + 2),
				scenario::fixed_ui::militancy, false, display_type::fp_one_place, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_rgo_size(EFFECT_DISPLAY_PARAMS) {
			return display_value(trigger_payload(tval[2]).signed_value,
				scenario::fixed_ui::rgo_size, true, display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_fort(EFFECT_DISPLAY_PARAMS) {
			return display_value(trigger_payload(tval[2]).signed_value,
				scenario::fixed_ui::fort_level, true, display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_naval_base(EFFECT_DISPLAY_PARAMS) {
			return display_value(trigger_payload(tval[2]).signed_value,
				scenario::fixed_ui::naval_base_level, true, display_type::integer, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_trigger_revolt_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::trigger_every_revolt], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			auto type = trigger_payload(tval[2]).small.values.rebel;
			auto culture = trigger_payload(tval[3]).culture;
			auto religion = trigger_payload(tval[4]).small.values.religion;
			auto ideology = trigger_payload(tval[5]).small.values.ideology;

			if(is_valid_index(type))
				cursor_in = fill_text_effect(scenario::fixed_ui::of_type, ws.s.population_m.rebel_types[type].name,
					ws, container, cursor_in, lm, fmt);
			if(is_valid_index(culture))
				cursor_in = fill_text_effect(scenario::fixed_ui::where_culture_is, ws.s.culture_m.culture_container[culture].name,
					ws, container, cursor_in, lm, fmt);
			if(is_valid_index(religion))
				cursor_in = fill_text_effect(scenario::fixed_ui::where_religion_is, ws.s.culture_m.religions[religion].name,
					ws, container, cursor_in, lm, fmt);
			if(is_valid_index(ideology))
				cursor_in = fill_text_effect(scenario::fixed_ui::where_ideology_is, ws.s.ideologies_m.ideology_container[ideology].name,
					ws, container, cursor_in, lm, fmt);

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_trigger_revolt_state(EFFECT_DISPLAY_PARAMS) {
			return ef_trigger_revolt_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_trigger_revolt_province(EFFECT_DISPLAY_PARAMS) {
			return ef_trigger_revolt_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_diplomatic_influence(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[3]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_effect(scenario::fixed_ui::diplomatic_influence_with, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_diplomatic_influence_this_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_this_nation_effect(scenario::fixed_ui::diplomatic_influence_with, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_diplomatic_influence_this_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_this_province_effect(scenario::fixed_ui::diplomatic_influence_with, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_diplomatic_influence_from_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_from_nation_effect(scenario::fixed_ui::diplomatic_influence_with, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_diplomatic_influence_from_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_from_province_effect(scenario::fixed_ui::diplomatic_influence_with, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_relation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[3]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_effect(scenario::fixed_ui::relations_with, tval[2], ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_relation_this_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_this_nation_effect(scenario::fixed_ui::relations_with, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_relation_this_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_this_province_effect(scenario::fixed_ui::relations_with, this_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_relation_from_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_from_nation_effect(scenario::fixed_ui::relations_with, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_relation_from_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_from_province_effect(scenario::fixed_ui::relations_with, from_slot, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_province_modifier(EFFECT_DISPLAY_PARAMS) {
			char16_t formatted_date[64];
			u16_format_date(formatted_date, date_tag(to_index(ws.w.current_date) + trigger_payload(tval[3]).signed_value));

			auto pmod = trigger_payload(tval[2]).prov_mod;

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.provincial_modifiers[pmod].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::date,
					vector_backed_string<char16_t>(formatted_date),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_modifier_until], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_province_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.provincial_modifier_definitions[pmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_add_province_modifier_no_duration(EFFECT_DISPLAY_PARAMS) {
			auto pmod = trigger_payload(tval[2]).prov_mod;

			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.provincial_modifiers[pmod].name),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_modifier], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_province_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.provincial_modifier_definitions[pmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_add_country_modifier(EFFECT_DISPLAY_PARAMS) {
			char16_t formatted_date[64];
			u16_format_date(formatted_date, date_tag(to_index(ws.w.current_date) + trigger_payload(tval[3]).signed_value));

			auto nmod = trigger_payload(tval[2]).nat_mod;

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.national_modifiers[nmod].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::date,
					vector_backed_string<char16_t>(formatted_date),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_modifier_until], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[nmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_add_country_modifier_no_duration(EFFECT_DISPLAY_PARAMS) {
			auto nmod = trigger_payload(tval[2]).nat_mod;

			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.modifiers_m.national_modifiers[nmod].name),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_modifier], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = modifiers::make_national_modifier_text_body(ws, container, cursor_in, lm, fmt, ws.s.modifiers_m.national_modifier_definitions[nmod]);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;
			auto tag = trigger_payload(tval[4]).tag;

			auto hname = ws.w.culture_s.national_tags_state[tag].holder ?
				ws.w.nation_s.nations.get<nation::name>(ws.w.culture_s.national_tags_state[tag].holder) :
				ws.s.culture_m.national_tags[tag].default_name.name;
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_months : scenario::fixed_ui::add_cb_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			auto owner = provinces::province_owner(ws, provinces::province_tag(tval[4]));
			auto hname = bool(owner) ?
				ws.w.nation_s.nations.get<nation::name>(owner) :
				ws.s.fixed_ui_text[scenario::fixed_ui::no_nation];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_months : scenario::fixed_ui::add_cb_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			auto hname = bool(to_nation(this_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_months : scenario::fixed_ui::add_cb_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			auto hname = bool(to_nation(from_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_months : scenario::fixed_ui::add_cb_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_casus_belli_from_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, nullptr, bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag(), gen);
		}
		ui::xy_pair ef_add_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;
			auto tag = trigger_payload(tval[4]).tag;

			auto hname = ws.w.culture_s.national_tags_state[tag].holder ?
				ws.w.nation_s.nations.get<nation::name>(ws.w.culture_s.national_tags_state[tag].holder) :
				ws.s.culture_m.national_tags[tag].default_name.name;
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_reversed_months : scenario::fixed_ui::add_cb_reversed_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			auto owner = provinces::province_owner(ws, provinces::province_tag(tval[4]));
			auto hname = bool(owner) ?
				ws.w.nation_s.nations.get<nation::name>(owner) :
				ws.s.fixed_ui_text[scenario::fixed_ui::no_nation];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_reversed_months : scenario::fixed_ui::add_cb_reversed_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			auto hname = bool(to_nation(this_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_reversed_months : scenario::fixed_ui::add_cb_reversed_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_add_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_add_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_add_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_add_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_add_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_add_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			auto hname = bool(to_nation(from_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, months);

			text_data::replacement repl[3] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[months != 0 ? scenario::fixed_ui::add_cb_reversed_months : scenario::fixed_ui::add_cb_reversed_no_months],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 3);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_add_casus_belli_from_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, nullptr, bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag(), gen);
		}
		ui::xy_pair ef_remove_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto tag = trigger_payload(tval[3]).tag;

			auto hname = ws.w.culture_s.national_tags_state[tag].holder ?
				ws.w.nation_s.nations.get<nation::name>(ws.w.culture_s.national_tags_state[tag].holder) :
				ws.s.culture_m.national_tags[tag].default_name.name;

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			auto owner = provinces::province_owner(ws, provinces::province_tag(tval[3]));
			auto hname = bool(owner) ?
				ws.w.nation_s.nations.get<nation::name>(owner) :
				ws.s.fixed_ui_text[scenario::fixed_ui::no_nation];

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			auto hname = bool(to_nation(this_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_remove_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_remove_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_remove_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_remove_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_remove_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_remove_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			auto hname = bool(to_nation(from_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_remove_casus_belli_from_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, nullptr, bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag(), gen);
		}
		ui::xy_pair ef_this_remove_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto tag = trigger_payload(tval[3]).tag;

			auto hname = ws.w.culture_s.national_tags_state[tag].holder ?
				ws.w.nation_s.nations.get<nation::name>(ws.w.culture_s.national_tags_state[tag].holder) :
				ws.s.culture_m.national_tags[tag].default_name.name;

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb_reversed],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_this_remove_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			auto owner = provinces::province_owner(ws, provinces::province_tag(tval[3]));
			auto hname = bool(owner) ?
				ws.w.nation_s.nations.get<nation::name>(owner) :
				ws.s.fixed_ui_text[scenario::fixed_ui::no_nation];

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb_reversed],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_this_remove_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			auto hname = bool(to_nation(this_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(this_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb_reversed],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_this_remove_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_this_remove_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_state(this_slot)) ? nations::state_owner(ws, to_state(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_this_remove_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_this_remove_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_prov(this_slot)) ? provinces::province_owner(ws, to_prov(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_this_remove_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_this_remove_casus_belli_this_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, bool(to_pop(this_slot)) ? population::get_pop_owner(ws, to_pop(this_slot)) : nations::country_tag(), nullptr, gen);
		}
		ui::xy_pair ef_this_remove_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			auto hname = bool(to_nation(from_slot)) ?
				ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) :
				ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, hname),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::name,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_cb_reversed],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_this_remove_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_this_remove_casus_belli_from_nation(tval, ws, container, cursor_in, lm, fmt,
				nullptr, nullptr, bool(to_prov(from_slot)) ? provinces::province_owner(ws, to_prov(from_slot)) : nations::country_tag(), gen);
		}

		inline ui::xy_pair display_war_goal(military::cb_type_tag cb, nations::state_tag st, cultures::national_tag lib, bool attacker, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			if(attacker)
				cursor_in = fill_text_effect_wo_newline(scenario::fixed_ui::attacker_wg_label, ws.s.military_m.cb_types[cb].name, ws, container, cursor_in, lm, fmt);
			else
				cursor_in = fill_text_effect_wo_newline(scenario::fixed_ui::defender_wg_label, ws.s.military_m.cb_types[cb].name, ws, container, cursor_in, lm, fmt);

			if(is_valid_index(st)) {
				cursor_in = fill_text_effect(scenario::fixed_ui::for_text, ws.w.nation_s.states.get<state::name>(st), ws, container, cursor_in, lm, fmt);
			} else if(is_valid_index(lib)) {
				auto holder = ws.w.culture_s.national_tags_state[lib].holder;
				cursor_in = fill_text_effect(scenario::fixed_ui::for_text,
					holder ? ws.w.nation_s.nations.get<nation::name>(holder) : ws.s.culture_m.national_tags[lib].default_name.name, ws, container, cursor_in, lm, fmt);
			} else {
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
			}
			return cursor_in;
		}

		inline ui::xy_pair make_wg_body(uint16_t const* tval, world_state& ws, ui::tagged_gui_object container,
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			auto attacker_wg_count = tval[0];
			uint32_t payload_index = 1;
			for(int32_t i = 0; i < attacker_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = provinces::province_tag(tval[payload_index + 1]);
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);
				cursor_in = display_war_goal(cb_type, state, liberate_country_tag, true, ws, container, cursor_in, lm, fmt);
				payload_index += 3;
			}

			auto defender_wg_count = tval[payload_index];
			++payload_index;
			for(int32_t i = 0; i < defender_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = provinces::province_tag(tval[payload_index + 1]);
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = ws.w.province_s.province_state_container.get<province_state::state_instance>(province_id);
				cursor_in = display_war_goal(cb_type, state, liberate_country_tag, false, ws, container, cursor_in, lm, fmt);
				payload_index += 3;
			}

			return cursor_in;
		}

		ui::xy_pair ef_war_tag(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_effect(scenario::fixed_ui::declare_war_on, tval[2], ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 3, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_yes_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_this_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_nation_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_yes_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_this_state(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_state_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_yes_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_this_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_province_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_yes_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_this_pop(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_pop_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_yes_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_from_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_from_nation_effect(scenario::fixed_ui::declare_war_on, from_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_yes_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_from_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_from_province_effect(scenario::fixed_ui::declare_war_on, from_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_yes_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_no_ally_tag(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_effect(scenario::fixed_ui::declare_war_on, tval[2], ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 3, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_no_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_no_ally_this_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_nation_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_no_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_no_ally_this_state(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_state_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_no_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_no_ally_this_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_province_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_no_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_no_ally_this_pop(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_this_pop_effect(scenario::fixed_ui::declare_war_on, this_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_no_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_no_ally_from_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_from_nation_effect(scenario::fixed_ui::declare_war_on, from_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_no_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_war_no_ally_from_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = tag_type_from_province_effect(scenario::fixed_ui::declare_war_on, from_slot, ws, container, cursor_in, lm, fmt);
			lm.increase_indent(1);

			cursor_in = make_wg_body(tval + 2, ws, container, cursor_in, lm, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::attacker_no_allies], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.decrease_indent(1);
			return cursor_in;
		}
		inline ui::xy_pair delayed_event_display(uint16_t const* tval, ui::xy_pair cursor_in, world_state& ws, ui::tagged_gui_object container,
			ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, tval[3]);

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.event_m.event_container[trigger_payload(tval[2]).event].title),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::value,
					vector_backed_string<char16_t>(local_buffer),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::event_fires_in_days], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		inline ui::xy_pair immediate_event_display(uint16_t const* tval, ui::xy_pair cursor_in, world_state& ws, ui::tagged_gui_object container,
			ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.event_m.event_container[trigger_payload(tval[2]).event].title),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::event_fires], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_country_event_this_nation(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_this_nation(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_this_nation(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_immediate_this_nation(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_this_state(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_this_state(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_this_state(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_immediate_this_state(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_this_province(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_this_province(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_this_province(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_immediate_this_province(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_this_pop(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_this_pop(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_this_pop(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_province_event_immediate_this_pop(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_province_this_state(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_province_this_state(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_province_this_province(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_province_this_province(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_province_this_pop(EFFECT_DISPLAY_PARAMS) {
			return delayed_event_display(tval, cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_country_event_immediate_province_this_pop(EFFECT_DISPLAY_PARAMS) {
			return immediate_event_display(tval, cursor_in, ws, container, lm, fmt);
		}

		ui::xy_pair ef_sub_unit_int(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_sub_unit_this(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_sub_unit_from(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_sub_unit_current(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_set_variable(EFFECT_DISPLAY_PARAMS) {
			auto value = read_float_from_payload(tval + 3);
			cursor_in = fill_text_effect_wo_newline(scenario::fixed_ui::set_national_variable_to, ws.s.variables_m.national_variable_to_name[trigger_payload(tval[2]).nat_var],
				ws, container, cursor_in, lm, fmt);
			cursor_in = display_value(value, display_type::fp_one_place, true, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_change_variable(EFFECT_DISPLAY_PARAMS) {
			auto value = read_float_from_payload(tval + 3);
			cursor_in = fill_text_effect_wo_newline(value >= 0.0f ? scenario::fixed_ui::increase_national_variable_by : scenario::fixed_ui::decrease_national_variable_by,
				ws.s.variables_m.national_variable_to_name[trigger_payload(tval[2]).nat_var],
				ws, container, cursor_in, lm, fmt);
			cursor_in = display_value(value, display_type::fp_one_place, true, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_ideology(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto factor = read_float_from_payload(tval + 3);

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.ideologies_m.ideology_container[ideology].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_upper_house(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto factor = trigger_payload(tval[3]).signed_value;

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.ideologies_m.ideology_container[ideology].name, ws, container, cursor_in, lm, fmt);
		}

		inline ui::xy_pair display_scaled_issue(float value, uint32_t type, issues::option_tag tag, ui::xy_pair cursor_in, world_state& ws,
			ui::tagged_gui_object container, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::up_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = display_value_wo_newline(value, type, false, display_type::percent, ws, container, cursor_in, lm, fmt);

			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.issues_m.options[tag].name),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::scaled_support], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		inline ui::xy_pair display_scaled_ideology(float value, uint32_t type, ideologies::ideology_tag tag, ui::xy_pair cursor_in, world_state& ws,
			ui::tagged_gui_object container, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::up_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = display_value_wo_newline(value, type, false, display_type::percent, ws, container, cursor_in, lm, fmt);

			text_data::replacement repl[1] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.ideologies_m.ideology_container[tag].name),
					[](ui::tagged_gui_object) {} }
			};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::scaled_support], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		inline ui::xy_pair display_scaled_unemployment(float value, uint32_t type, ui::xy_pair cursor_in, world_state& ws,
			ui::tagged_gui_object container, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::up_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = display_value_wo_newline(value, type, false, display_type::percent, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::scaled_unemployment], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}

		ui::xy_pair ef_scaled_militancy_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::militancy,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::consciousness,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_nation_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_nation_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_nation_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::militancy,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_nation_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_nation_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_nation_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::consciousness,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_state_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_state_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_state_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::militancy,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_state_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_state_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_state_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::consciousness,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_province_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_province_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::militancy, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_militancy_province_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::militancy,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_province_issue(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_issue(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.option,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_province_ideology(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_ideology(read_float_from_payload(tval + 3), scenario::fixed_ui::consciousness, trigger_payload(tval[2]).small.values.ideology,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_scaled_consciousness_province_unemployment(EFFECT_DISPLAY_PARAMS) {
			return display_scaled_unemployment(read_float_from_payload(tval + 2), scenario::fixed_ui::consciousness,
				cursor_in, ws, container, lm, fmt);
		}
		ui::xy_pair ef_variable_good_name(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value_wo_newline(read_float_from_payload(tval + 3), scenario::fixed_ui::stockpile_of, true, display_type::integer, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[trigger_payload(tval[2]).small.values.good].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_variable_good_name_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value_wo_newline(read_float_from_payload(tval + 3), scenario::fixed_ui::stockpile_of, true, display_type::integer, ws, container, cursor_in, lm, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.economy_m.goods[trigger_payload(tval[2]).small.values.good].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_define_general(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::create_general], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_define_admiral(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::create_admiral], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_dominant_issue(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto factor = read_float_from_payload(tval + 3);

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[opt].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_dominant_issue_nation(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto factor = read_float_from_payload(tval + 3);

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[opt].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_add_war_goal(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			text_data::replacement repl[2] = {
				text_data::replacement{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.military_m.cb_types[type].name),
					[](ui::tagged_gui_object) {} },
				text_data::replacement{
					text_data::value_type::nation,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, bool(to_nation(from_slot)) ? ws.w.nation_s.nations.get<nation::name>(to_nation(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation]),
					[](ui::tagged_gui_object) {} } };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_war_goal], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, repl, 2);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_move_issue_percentage_nation(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = trigger_payload(tval[2]).small.values.option;
			auto to_issue = trigger_payload(tval[3]).small.values.option;
			auto factor = read_float_from_payload(tval + 4);

			cursor_in = display_value(-factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			cursor_in = fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[from_issue].name, ws, container, cursor_in, lm, fmt);

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[to_issue].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_move_issue_percentage_state(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = trigger_payload(tval[2]).small.values.option;
			auto to_issue = trigger_payload(tval[3]).small.values.option;
			auto factor = read_float_from_payload(tval + 4);

			cursor_in = display_value(-factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			cursor_in = fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[from_issue].name, ws, container, cursor_in, lm, fmt);

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[to_issue].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_move_issue_percentage_province(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = trigger_payload(tval[2]).small.values.option;
			auto to_issue = trigger_payload(tval[3]).small.values.option;
			auto factor = read_float_from_payload(tval + 4);

			cursor_in = display_value(-factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			cursor_in = fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[from_issue].name, ws, container, cursor_in, lm, fmt);

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[to_issue].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_move_issue_percentage_pop(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = trigger_payload(tval[2]).small.values.option;
			auto to_issue = trigger_payload(tval[3]).small.values.option;
			auto factor = read_float_from_payload(tval + 4);

			cursor_in = display_value(-factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			cursor_in = fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[from_issue].name, ws, container, cursor_in, lm, fmt);

			cursor_in = display_value(factor, display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::support_for_blank, ws.s.issues_m.options[to_issue].name, ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_party_loyalty_nation_from_province(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;

			cursor_in = display_value_wo_newline(read_float_from_payload(tval + 3), scenario::fixed_ui::blank_loyalty, true, display_type::percent,
				ws, container, cursor_in, lm, fmt);
			cursor_in = display_value(read_float_from_payload(tval + 3), display_type::percent, true, ws, container, cursor_in, lm, fmt);
			cursor_in = fill_text_effect_wo_newline(scenario::fixed_ui::blank_loyalty, ws.s.ideologies_m.ideology_container[ideology].name,
				ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return fill_text_effect(scenario::fixed_ui::in_blank, bool(to_prov(from_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(from_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::from_province],
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_party_loyalty_province_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;

			cursor_in = display_value_wo_newline(read_float_from_payload(tval + 3), scenario::fixed_ui::blank_loyalty, true, display_type::percent,
				ws, container, cursor_in, lm, fmt);
			cursor_in = display_value(read_float_from_payload(tval + 3), display_type::percent, true, ws, container, cursor_in, lm, fmt);
			return fill_text_effect(scenario::fixed_ui::blank_loyalty, ws.s.ideologies_m.ideology_container[ideology].name,
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_party_loyalty_province_id_nation(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);
			auto ideology = trigger_payload(tval[3]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 4);

			cursor_in = display_value_wo_newline(amount, scenario::fixed_ui::blank_loyalty, true, display_type::percent,
				ws, container, cursor_in, lm, fmt);
			cursor_in = display_value(read_float_from_payload(tval + 3), display_type::percent, true, ws, container, cursor_in, lm, fmt);
			cursor_in = fill_text_effect_wo_newline(scenario::fixed_ui::blank_loyalty, ws.s.ideologies_m.ideology_container[ideology].name,
				ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return fill_text_effect(scenario::fixed_ui::in_blank, ws.w.province_s.province_state_container.get<province_state::name>(prov),
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_party_loyalty_province_id_from_nation(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);
			auto ideology = trigger_payload(tval[3]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 4);

			cursor_in = display_value_wo_newline(amount, scenario::fixed_ui::blank_loyalty, true, display_type::percent,
				ws, container, cursor_in, lm, fmt);
			cursor_in = display_value(read_float_from_payload(tval + 3), display_type::percent, true, ws, container, cursor_in, lm, fmt);
			cursor_in = fill_text_effect_wo_newline(scenario::fixed_ui::blank_loyalty, ws.s.ideologies_m.ideology_container[ideology].name,
				ws, container, cursor_in, lm, fmt);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			return fill_text_effect(scenario::fixed_ui::in_blank, ws.w.province_s.province_state_container.get<province_state::name>(prov),
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_build_railway_in_capital_yes_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::railroad_in_capital_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_railway_in_capital_yes_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::railroad_in_capital_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_railway_in_capital_no_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::railroad_in_capital], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_railway_in_capital_no_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::railroad_in_capital], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_fort_in_capital_yes_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::fort_in_capital_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_fort_in_capital_yes_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::fort_in_capital_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_fort_in_capital_no_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::fort_in_capital], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_build_fort_in_capital_no_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::fort_in_capital], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_relation_reb(EFFECT_DISPLAY_PARAMS) {
			cursor_in = display_value(trigger_payload(tval[2]).signed_value, display_type::integer, true, ws, container, cursor_in, lm, fmt);
			return tag_type_rebel_slot_effect(scenario::fixed_ui::relations_with, to_rebel(from_slot), ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_variable_tech_name(EFFECT_DISPLAY_PARAMS) {
			auto t = trigger_payload(tval[2]).tech;
			return fill_text_effect(scenario::fixed_ui::enable_blank, ws.s.technology_m.technologies_container[t].name,
				ws, container, cursor_in, lm, fmt);
		}
		ui::xy_pair ef_set_country_flag_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_set_country_flag(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_add_country_modifier_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_add_country_modifier(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_add_country_modifier_province_no_duration(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_add_country_modifier_no_duration(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_relation_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_relation(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_relation_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_relation_this_nation(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_relation_province_this_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_relation_this_province(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_relation_province_from_nation(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_relation_from_nation(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_relation_province_from_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_relation_from_province(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_relation_province_reb(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_relation_reb(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}
		ui::xy_pair ef_treasury_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in,
				bool(to_prov(primary_slot)) ? ws.w.province_s.province_state_container.get<province_state::name>(to_prov(primary_slot)) : ws.s.fixed_ui_text[scenario::fixed_ui::singular_province],
				fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = ef_treasury(tval, ws, container, cursor_in, lm, fmt,
				bool(to_prov(primary_slot)) ? provinces::province_owner(ws, to_prov(primary_slot)) : nations::country_tag(), this_slot, from_slot, gen);
			lm.decrease_indent(1);
			return cursor_in;
		}

		/*
		#define EFFECT_DISPLAY_PARAMS uint16_t const* tval, world_state& ws, ui::tagged_gui_object container, \
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, const_parameter primary_slot, \
			const_parameter this_slot, const_parameter from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen
		*/

		static ui::xy_pair(*effect_functions[])(uint16_t const*, world_state&, ui::tagged_gui_object, ui::xy_pair, ui::unlimited_line_manager&, ui::text_format const&,
			const_parameter, const_parameter, const_parameter, jsf_prng&) = {
			ef_none,
			ef_capital,
			ef_add_core_tag,
			ef_add_core_int,
			ef_add_core_this_nation,
			ef_add_core_this_province,
			ef_add_core_this_state,
			ef_add_core_this_pop,
			ef_add_core_from_province,
			ef_add_core_from_nation,
			ef_add_core_reb,
			ef_remove_core_tag,
			ef_remove_core_int,
			ef_remove_core_this_nation,
			ef_remove_core_this_province,
			ef_remove_core_this_state,
			ef_remove_core_this_pop,
			ef_remove_core_from_province,
			ef_remove_core_from_nation,
			ef_remove_core_reb,
			ef_change_region_name_state,
			ef_change_region_name_province,
			ef_trade_goods,
			ef_add_accepted_culture,
			ef_add_accepted_culture_union,
			ef_primary_culture,
			ef_primary_culture_this_nation,
			ef_primary_culture_this_state,
			ef_primary_culture_this_province,
			ef_primary_culture_this_pop,
			ef_primary_culture_from_nation,
			ef_remove_accepted_culture,
			ef_life_rating,
			ef_religion,
			ef_is_slave_state_yes,
			ef_is_slave_pop_yes,
			ef_research_points,
			ef_tech_school,
			ef_government,
			ef_government_reb,
			ef_treasury,
			ef_war_exhaustion,
			ef_prestige,
			ef_change_tag,
			ef_change_tag_culture,
			ef_change_tag_no_core_switch,
			ef_change_tag_no_core_switch_culture,
			ef_set_country_flag,
			ef_clr_country_flag,
			ef_military_access,
			ef_military_access_this_nation,
			ef_military_access_this_province,
			ef_military_access_from_nation,
			ef_military_access_from_province,
			ef_badboy,
			ef_secede_province,
			ef_secede_province_this_nation,
			ef_secede_province_this_state,
			ef_secede_province_this_province,
			ef_secede_province_this_pop,
			ef_secede_province_from_nation,
			ef_secede_province_from_province,
			ef_secede_province_reb,
			ef_inherit,
			ef_inherit_this_nation,
			ef_inherit_this_state,
			ef_inherit_this_province,
			ef_inherit_this_pop,
			ef_inherit_from_nation,
			ef_inherit_from_province,
			ef_annex_to,
			ef_annex_to_this_nation,
			ef_annex_to_this_state,
			ef_annex_to_this_province,
			ef_annex_to_this_pop,
			ef_annex_to_from_nation,
			ef_annex_to_from_province,
			ef_release,
			ef_release_this_nation,
			ef_release_this_state,
			ef_release_this_province,
			ef_release_this_pop,
			ef_release_from_nation,
			ef_release_from_province,
			ef_change_controller,
			ef_change_controller_this_nation,
			ef_change_controller_this_province,
			ef_change_controller_from_nation,
			ef_change_controller_from_province,
			ef_infrastructure,
			ef_money,
			ef_leadership,
			ef_create_vassal,
			ef_create_vassal_this_nation,
			ef_create_vassal_this_province,
			ef_create_vassal_from_nation,
			ef_create_vassal_from_province,
			ef_end_military_access,
			ef_end_military_access_this_nation,
			ef_end_military_access_this_province,
			ef_end_military_access_from_nation,
			ef_end_military_access_from_province,
			ef_leave_alliance,
			ef_leave_alliance_this_nation,
			ef_leave_alliance_this_province,
			ef_leave_alliance_from_nation,
			ef_leave_alliance_from_province,
			ef_end_war,
			ef_end_war_this_nation,
			ef_end_war_this_province,
			ef_end_war_from_nation,
			ef_end_war_from_province,
			ef_enable_ideology,
			ef_ruling_party_ideology,
			ef_plurality,
			ef_remove_province_modifier,
			ef_remove_country_modifier,
			ef_create_alliance,
			ef_create_alliance_this_nation,
			ef_create_alliance_this_province,
			ef_create_alliance_from_nation,
			ef_create_alliance_from_province,
			ef_release_vassal,
			ef_release_vassal_this_nation,
			ef_release_vassal_this_province,
			ef_release_vassal_from_nation,
			ef_release_vassal_from_province,
			ef_release_vassal_reb,
			ef_release_vassal_random,
			ef_change_province_name,
			ef_enable_canal,
			ef_set_global_flag,
			ef_clr_global_flag,
			ef_nationalvalue_province,
			ef_nationalvalue_nation,
			ef_civilized_yes,
			ef_civilized_no,
			ef_is_slave_state_no,
			ef_is_slave_pop_no,
			ef_election,
			ef_social_reform,
			ef_political_reform,
			ef_add_tax_relative_income,
			ef_neutrality,
			ef_reduce_pop,
			ef_move_pop,
			ef_pop_type,
			ef_years_of_research,
			ef_prestige_factor_positive,
			ef_prestige_factor_negative,
			ef_military_reform,
			ef_economic_reform,
			ef_remove_random_military_reforms,
			ef_remove_random_economic_reforms,
			ef_add_crime,
			ef_add_crime_none,
			ef_nationalize,
			ef_build_factory_in_capital_state,
			ef_activate_technology,
			ef_great_wars_enabled_yes,
			ef_great_wars_enabled_no,
			ef_world_wars_enabled_yes,
			ef_world_wars_enabled_no,
			ef_assimilate_province,
			ef_assimilate_pop,
			ef_literacy,
			ef_add_crisis_interest,
			ef_flashpoint_tension,
			ef_add_crisis_temperature,
			ef_consciousness,
			ef_militancy,
			ef_rgo_size,
			ef_fort,
			ef_naval_base,
			ef_trigger_revolt_nation,
			ef_trigger_revolt_state,
			ef_trigger_revolt_province,
			ef_diplomatic_influence,
			ef_diplomatic_influence_this_nation,
			ef_diplomatic_influence_this_province,
			ef_diplomatic_influence_from_nation,
			ef_diplomatic_influence_from_province,
			ef_relation,
			ef_relation_this_nation,
			ef_relation_this_province,
			ef_relation_from_nation,
			ef_relation_from_province,
			ef_add_province_modifier,
			ef_add_province_modifier_no_duration,
			ef_add_country_modifier,
			ef_add_country_modifier_no_duration,
			ef_casus_belli_tag,
			ef_casus_belli_int,
			ef_casus_belli_this_nation,
			ef_casus_belli_this_state,
			ef_casus_belli_this_province,
			ef_casus_belli_this_pop,
			ef_casus_belli_from_nation,
			ef_casus_belli_from_province,
			ef_add_casus_belli_tag,
			ef_add_casus_belli_int,
			ef_add_casus_belli_this_nation,
			ef_add_casus_belli_this_state,
			ef_add_casus_belli_this_province,
			ef_add_casus_belli_this_pop,
			ef_add_casus_belli_from_nation,
			ef_add_casus_belli_from_province,
			ef_remove_casus_belli_tag,
			ef_remove_casus_belli_int,
			ef_remove_casus_belli_this_nation,
			ef_remove_casus_belli_this_state,
			ef_remove_casus_belli_this_province,
			ef_remove_casus_belli_this_pop,
			ef_remove_casus_belli_from_nation,
			ef_remove_casus_belli_from_province,
			ef_this_remove_casus_belli_tag,
			ef_this_remove_casus_belli_int,
			ef_this_remove_casus_belli_this_nation,
			ef_this_remove_casus_belli_this_state,
			ef_this_remove_casus_belli_this_province,
			ef_this_remove_casus_belli_this_pop,
			ef_this_remove_casus_belli_from_nation,
			ef_this_remove_casus_belli_from_province,
			ef_war_tag,
			ef_war_this_nation,
			ef_war_this_state,
			ef_war_this_province,
			ef_war_this_pop,
			ef_war_from_nation,
			ef_war_from_province,
			ef_war_no_ally_tag,
			ef_war_no_ally_this_nation,
			ef_war_no_ally_this_state,
			ef_war_no_ally_this_province,
			ef_war_no_ally_this_pop,
			ef_war_no_ally_from_nation,
			ef_war_no_ally_from_province,
			ef_country_event_this_nation,
			ef_country_event_immediate_this_nation,
			ef_province_event_this_nation,
			ef_province_event_immediate_this_nation,
			ef_sub_unit_int,
			ef_sub_unit_this,
			ef_sub_unit_from,
			ef_sub_unit_current,
			ef_set_variable,
			ef_change_variable,
			ef_ideology,
			ef_upper_house,
			ef_scaled_militancy_issue,
			ef_scaled_militancy_ideology,
			ef_scaled_militancy_unemployment,
			ef_scaled_consciousness_issue,
			ef_scaled_consciousness_ideology,
			ef_scaled_consciousness_unemployment,
			ef_define_general,
			ef_define_admiral,
			ef_dominant_issue,
			ef_add_war_goal,
			ef_move_issue_percentage_nation,
			ef_move_issue_percentage_state,
			ef_move_issue_percentage_province,
			ef_move_issue_percentage_pop,
			ef_party_loyalty_nation_from_province,
			ef_party_loyalty_province_from_nation,
			ef_party_loyalty_province_id_nation,
			ef_party_loyalty_province_id_from_nation,
			ef_build_railway_in_capital_yes_whole_state_yes_limit,
			ef_build_railway_in_capital_yes_whole_state_no_limit,
			ef_build_railway_in_capital_no_whole_state_yes_limit,
			ef_build_railway_in_capital_no_whole_state_no_limit,
			ef_build_fort_in_capital_yes_whole_state_yes_limit,
			ef_build_fort_in_capital_yes_whole_state_no_limit,
			ef_build_fort_in_capital_no_whole_state_yes_limit,
			ef_build_fort_in_capital_no_whole_state_no_limit,
			ef_relation_reb,
			ef_variable_tech_name,
			ef_variable_good_name,
			ef_set_country_flag_province,
			ef_add_country_modifier_province,
			ef_add_country_modifier_province_no_duration,
			ef_dominant_issue_nation,
			ef_relation_province,
			ef_relation_province_this_nation,
			ef_relation_province_this_province,
			ef_relation_province_from_nation,
			ef_relation_province_from_province,
			ef_relation_province_reb,
			ef_scaled_militancy_nation_issue,
			ef_scaled_militancy_nation_ideology,
			ef_scaled_militancy_nation_unemployment,
			ef_scaled_consciousness_nation_issue,
			ef_scaled_consciousness_nation_ideology,
			ef_scaled_consciousness_nation_unemployment,
			ef_scaled_militancy_state_issue,
			ef_scaled_militancy_state_ideology,
			ef_scaled_militancy_state_unemployment,
			ef_scaled_consciousness_state_issue,
			ef_scaled_consciousness_state_ideology,
			ef_scaled_consciousness_state_unemployment,
			ef_scaled_militancy_province_issue,
			ef_scaled_militancy_province_ideology,
			ef_scaled_militancy_province_unemployment,
			ef_scaled_consciousness_province_issue,
			ef_scaled_consciousness_province_ideology,
			ef_scaled_consciousness_province_unemployment,
			ef_variable_good_name_province,
			ef_treasury_province,
			ef_country_event_this_state,
			ef_country_event_immediate_this_state,
			ef_province_event_this_state,
			ef_province_event_immediate_this_state,
			ef_country_event_this_province,
			ef_country_event_immediate_this_province,
			ef_province_event_this_province,
			ef_province_event_immediate_this_province,
			ef_country_event_this_pop,
			ef_country_event_immediate_this_pop,
			ef_province_event_this_pop,
			ef_province_event_immediate_this_pop,
			ef_country_event_province_this_nation,
			ef_country_event_immediate_province_this_nation,
			ef_country_event_province_this_state,
			ef_country_event_immediate_province_this_state,
			ef_country_event_province_this_province,
			ef_country_event_immediate_province_this_province,
			ef_country_event_province_this_pop,
			ef_country_event_immediate_province_this_pop,
		};

		ui::xy_pair _make_effect_description(
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt,
			uint16_t const* tval,
			const_parameter primary_slot,
			const_parameter this_slot,
			const_parameter from_slot,
			jsf_prng& generator_copy,
			bool show_condition
		) {
			if((*tval & effect_codes::is_scope) != 0) {
				return effect_scope_functions[*tval & effect_codes::code_mask](tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, generator_copy, show_condition);
			} else {
				return effect_functions[*tval & effect_codes::code_mask](tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, generator_copy);
			}
		}
	}

	ui::xy_pair make_effect_description(
		world_state& ws,
		ui::tagged_gui_object container,
		ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm,
		ui::text_format const& fmt,
		uint16_t const* tval,
		const_parameter primary_slot,
		const_parameter this_slot,
		const_parameter from_slot,
		jsf_prng generator_copy,
		bool show_condition
	) {
		return _make_effect_description(ws, container, cursor_in, lm, fmt, tval, primary_slot, this_slot, from_slot, generator_copy, show_condition);
	}
}

#pragma warning( pop ) 
