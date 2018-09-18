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

namespace triggers {

	namespace {
		ui::xy_pair _make_effect_description(
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt,
			uint16_t const* tval,
			void* primary_slot,
			void* this_slot,
			void* from_slot,
			population::rebel_faction* rebel_slot,
			jsf_prng& generator_copy
		);

		inline ui::xy_pair display_subeffects(
			uint16_t const* source,
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt,
			void* primary_slot,
			void* this_slot,
			void* from_slot,
			population::rebel_faction* rebel_slot,
			jsf_prng& gen) {

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
					rebel_slot,
					gen
				);

				sub_units_start += 1 + get_effect_payload_size(sub_units_start);
			}

			//lm.decrease_indent(1);

			return cursor_in;
		}

		template<typename value_type>
		ui::xy_pair display_value(value_type value, uint32_t label_index, bool positive_is_green, display_type d_type, world_state& ws,
			ui::tagged_gui_object container, ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt) {

			char16_t local_buffer[32];
			ui::text_format local_fmt{ (value < value_type(0)) == positive_is_green ? text_data::text_color::red : text_data::text_color::green, fmt.font_handle, fmt.font_size};
			if(value < value_type(0))
				local_buf[0] = u'-';
			else
				local_buf[0] = u'+';

			put_pos_value_in_buffer(local_buf + 1, d_type, std::abs(value));
			cursor_in = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				container, cursor_in, local_fmt, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[label_index], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			return cursor_in;
		}

#define EFFECT_DISPLAY_PARAMS uint16_t const* tval, world_state& ws, ui::tagged_gui_object container, \
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, void* primary_slot, \
			void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen

		ui::xy_pair es_generic_scope(EFFECT_DISPLAY_PARAMS) {
			return display_subeffects(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, rebel_slot, gen);
		}

		inline uint32_t random_or_every(uint16_t tval) {
			return (tval & effect_codes::is_random_scope) != 0 ? scenario::fixed_ui::random : scenario::fixed_ui::every;
		}

		ui::xy_pair es_x_neighbor_province_scope(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(((provinces::province_state*)primary_slot)->id);
				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner && test_trigger(limit, ws, &ps, this_slot, from_slot, rebel_slot))
							rlist.push_back(&ps);
					}
				} else {
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner)
							rlist.push_back(&ps);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
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

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_neighbor_country_scope(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				auto neighbor_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->neighboring_nations);

				boost::container::small_vector<nations::nation*, 16, concurrent_allocator<nations::nation*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n : neighbor_range) {
						auto& ns = ws.w.nation_s.nations[n];
						if(test_trigger(limit, ws, &ns, this_slot, from_slot, rebel_slot))
							rlist.push_back(&ns);
					}
				} else {
					for(auto n : neighbor_range)
						rlist.push_back(&(ws.w.nation_s.nations[n]));
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
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

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_country_scope_nation(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				boost::container::small_vector<nations::nation*, 64, concurrent_allocator<nations::nation*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					ws.w.nation_s.nations.for_each([&rlist, limit, &ws, primary_slot, this_slot, from_slot, rebel_slot](nations::nation& n) {
						if(&n != primary_slot && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0 && test_trigger(limit, ws, &n, this_slot, from_slot, rebel_slot))
							rlist.push_back(&n);
					});
				} else {
					ws.w.nation_s.nations.for_each([&rlist, &ws, primary_slot](nations::nation& n) {
						if(&n != primary_slot && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0)
							rlist.push_back(&n);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
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

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_country_scope(EFFECT_DISPLAY_PARAMS) {
			return es_x_country_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_x_empty_neighbor_province_scope(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				auto neighbor_range = ws.s.province_m.same_type_adjacency.get_row(((provinces::province_state*)primary_slot)->id);
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner == nullptr && test_trigger(limit, ws, &ps, this_slot, from_slot, rebel_slot))
							rlist.push_back(&ps);
					}
				} else {
					for(auto p : neighbor_range) {
						auto& ps = ws.w.province_s.province_state_container[p];
						if(ps.owner == nullptr)
							rlist.push_back(&ps);
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
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

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_greater_power_scope(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				auto ranked_range = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
				int32_t great_nations_count = int32_t(ws.s.modifiers_m.global_defines.great_nations_count);
				int32_t count = 0;

				boost::container::small_vector<nations::nation*, 16, concurrent_allocator<nations::nation*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							auto& nation = ws.w.nation_s.nations[*n];
							if(nations::is_great_power(ws, nation) && test_trigger(limit, ws, &nation, this_slot, from_slot, rebel_slot)) {
								++count;
								rlist.push_back(&nation);
							}
						}
					}
				} else {
					for(auto n = ranked_range.first; (n != ranked_range.second) & (count < great_nations_count); ++n) {
						if(is_valid_index(*n)) {
							auto& nation = ws.w.nation_s.nations[*n];
							if(nations::is_great_power(ws, nation)) {
								++count;
								rlist.push_back(&nation);
							}
						}
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
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

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_poor_strata_scope_nation(EFFECT_DISPLAY_PARAMS) {
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
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_poor_strata_scope_state(EFFECT_DISPLAY_PARAMS) {
			return es_poor_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_poor_strata_scope_province(EFFECT_DISPLAY_PARAMS) {
			return es_poor_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_middle_strata_scope_nation(EFFECT_DISPLAY_PARAMS) {
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
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_middle_strata_scope_state(EFFECT_DISPLAY_PARAMS) {
			return es_middle_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_middle_strata_scope_province(EFFECT_DISPLAY_PARAMS) {
			return es_middle_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_rich_strata_scope_nation(EFFECT_DISPLAY_PARAMS) {
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
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_rich_strata_scope_state(EFFECT_DISPLAY_PARAMS) {
			return es_rich_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_rich_strata_scope_province(EFFECT_DISPLAY_PARAMS) {
			return es_rich_strata_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_x_pop_scope_nation(EFFECT_DISPLAY_PARAMS) {
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
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_pop_scope_state(EFFECT_DISPLAY_PARAMS) {
			return es_x_pop_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_x_pop_scope_province(EFFECT_DISPLAY_PARAMS) {
			return es_x_pop_scope_nation(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_x_owned_scope_nation(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, *((nations::nation*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](provinces::province_state& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_province(ws, *((nations::nation*)primary_slot), [&rlist](provinces::province_state& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::great_power], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owned_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(primary_slot)
					cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}
			
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_owned_scope_state(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_province(ws, *((nations::state_instance*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](provinces::province_state& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_province(ws, *((nations::state_instance*)primary_slot), [&rlist](provinces::province_state& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::great_power], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ((nations::state_instance*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owned_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(primary_slot)
					cursor_in = ui::add_linear_text(cursor_in, ((nations::state_instance*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_core_scope(EFFECT_DISPLAY_PARAMS) {
			cultures::national_tag tag = primary_slot ? ((nations::nation*)primary_slot)->tag : cultures::national_tag();
			if(is_valid_index(tag) && (tval[0] & effect_codes::is_random_scope) != 0) {
				auto cores_range = get_range(ws.w.province_s.province_arrays, ws.w.culture_s.national_tags_state[tag].core_provinces);
				boost::container::small_vector<provinces::province_state*, 16, concurrent_allocator<provinces::province_state*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					for(auto p : cores_range) {
						if(is_valid_index(p)) {
							auto& pobj = ws.w.province_s.province_state_container[p];
							if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
								rlist.push_back(&pobj);
						}
					}
				} else {
					for(auto p : cores_range) {
						if(is_valid_index(p))
							rlist.push_back(&(ws.w.province_s.province_state_container[p]));
					}
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::core_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				}

			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::core_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(primary_slot)
					cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

				
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_x_state_scope(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::is_random_scope) != 0 && primary_slot) {
				boost::container::small_vector<nations::state_instance*, 16, concurrent_allocator<nations::state_instance*>> rlist;

				if((tval[0] & effect_codes::scope_has_limit) != 0) {
					auto limit = ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger);
					nations::for_each_state(ws, *((nations::nation*)primary_slot), [&ws, &rlist, limit, this_slot, from_slot, rebel_slot](nations::state_instance& pobj) {
						if(test_trigger(limit, ws, &pobj, this_slot, from_slot, rebel_slot))
							rlist.push_back(&pobj);
					});
				} else {
					nations::for_each_state(ws, *((nations::nation*)primary_slot), [&rlist](nations::state_instance& pobj) {
						rlist.push_back(&pobj);
					});
				}

				if(rlist.size() != 0) {
					std::uniform_int_distribution<int32_t> dist(0, int32_t(rlist.size()) - 1);
					cursor_in = ui::add_linear_text(cursor_in, rlist[uint32_t(dist(gen))]->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::state_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
					cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[random_or_every(tval[0])], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::state_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
				if(primary_slot)
					cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				else
					cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			}

			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);

			if((tval[0] & effect_codes::scope_has_limit) != 0 && !bool(primary_slot)) {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_random_list_scope(EFFECT_DISPLAY_PARAMS) {
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
					sub_units_start + 1, primary_slot, this_slot, from_slot, rebel_slot, gen);
				sub_units_start += 2 + get_effect_payload_size(sub_units_start + 1); // each member preceeded by uint16_t
			}

			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_random_scope(EFFECT_DISPLAY_PARAMS) {
			auto chance = tval[2];
			std::uniform_int_distribution<int32_t> dist(0, 99);
			dist(gen);

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
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_owner_scope_state(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((nations::state_instance*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto owner = ((nations::state_instance*)primary_slot)->owner;
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_owner_scope_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((provinces::province_state*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto owner = ((provinces::province_state*)primary_slot)->owner;
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_controller_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::controller_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((provinces::province_state*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto controller = ((provinces::province_state*)primary_slot)->controller;
				if(controller)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, controller, this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_location_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::location_of_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(primary_slot) {
				auto location = ((population::pop*)primary_slot)->location;
				if(is_valid_index(location))
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, &(ws.w.province_s.province_state_container[location]), this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_country_scope_pop(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::nation_of_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			if(primary_slot) {
				auto owner = population::get_pop_owner(ws, *((population::pop*)primary_slot));
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_country_scope_state(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::owner_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((nations::state_instance*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto owner = ((nations::state_instance*)primary_slot)->owner;
				if(owner)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, owner, this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_capital_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::capital_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto capital = ((nations::nation*)primary_slot)->current_capital;
				if(is_valid_index(capital))
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, &(ws.w.province_s.province_state_container[capital]), this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_this_scope_nation(EFFECT_DISPLAY_PARAMS) {
			if(this_slot)
				cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)this_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else 
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_this_scope_state(EFFECT_DISPLAY_PARAMS) {
			if(this_slot)
				cursor_in = ui::add_linear_text(cursor_in, ((nations::state_instance*)this_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_this_scope_province(EFFECT_DISPLAY_PARAMS) {
			if(this_slot)
				cursor_in = ui::add_linear_text(cursor_in, ((provinces::province_state*)this_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_this_scope_pop(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::this_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, this_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_nation(EFFECT_DISPLAY_PARAMS) {
			if(from_slot)
				cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)from_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_state(EFFECT_DISPLAY_PARAMS) {
			if(from_slot)
				cursor_in = ui::add_linear_text(cursor_in, ((nations::state_instance*)from_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_province(EFFECT_DISPLAY_PARAMS) {
			if(from_slot)
				cursor_in = ui::add_linear_text(cursor_in, ((provinces::province_state*)from_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			else
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_from_scope_pop(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::from_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, from_slot, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_sea_zone_scope(EFFECT_DISPLAY_PARAMS) {
			if(primary_slot) {
				auto sea_zones = ws.s.province_m.coastal_adjacency.get_row(((provinces::province_state*)primary_slot)->id);
				if(sea_zones.first != sea_zones.second) {
					cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container[*sea_zones.first].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();

					lm.increase_indent(1);
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, &(ws.w.province_s.province_state_container[*sea_zones.first]), this_slot, from_slot, rebel_slot, gen);
					lm.decrease_indent(1);
				}
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::adjacent_sea], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_cultural_union_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::cultural_union_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_overlord_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::overlord_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto overlord = ((nations::nation*)primary_slot)->overlord;
				if(overlord)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, overlord, this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_sphere_owner_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::sphere_leader_of], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((nations::nation*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				auto overlord = ((nations::nation*)primary_slot)->sphere_leader;
				if(overlord)
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, overlord, this_slot, from_slot, rebel_slot, gen);
				else
					cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_independence_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::reb_independence_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_flashpoint_tag_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::flashpoint_nation], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_crisis_state_scope(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::crisis_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_state_scope_province(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::containing_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			if(primary_slot) {
				cursor_in = ui::add_linear_text(cursor_in, ((provinces::province_state*)primary_slot)->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);

				auto si = ((provinces::province_state*)primary_slot)->state_instance;
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, si, this_slot, from_slot, rebel_slot, gen);

				lm.decrease_indent(1);
			} else {
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_province], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}

			return cursor_in;
		}
		ui::xy_pair es_state_scope_pop(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::containing_state], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::singular_pop], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();

			lm.increase_indent(1);
			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);
		
			return cursor_in;
		}
		ui::xy_pair es_tag_scope(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				auto tag = trigger_payload(tval[3]).tag;

				if(auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder; tag_holder) {
					cursor_in = ui::add_linear_text(cursor_in, tag_holder->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
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
					nullptr, this_slot, from_slot, rebel_slot);
				

				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				auto tag = trigger_payload(tval[2]).tag;
				if(auto tag_holder = ws.w.culture_s.national_tags_state[tag].holder; tag_holder) {
					cursor_in = ui::add_linear_text(cursor_in, tag_holder->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				} else {
					cursor_in = ui::add_linear_text(cursor_in, ws.s.culture_m.national_tags[tag].default_name.name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
					cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
					lm.finish_current_line();
				}
				
				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}
			return cursor_in;
		}
		ui::xy_pair es_integer_scope(EFFECT_DISPLAY_PARAMS) {
			if((tval[0] & effect_codes::scope_has_limit) != 0) {
				provinces::province_tag ptag(tval[3]);
				provinces::province_state* pstate = &(ws.w.province_s.province_state_container[ptag]);

				cursor_in = ui::add_linear_text(cursor_in, pstate->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::where], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = make_trigger_description(ws, container, cursor_in, lm, fmt,
					ws.s.trigger_m.trigger_data.data() + to_index(trigger_payload(tval[2]).trigger),
					nullptr, this_slot, from_slot, rebel_slot);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			} else {
				provinces::province_tag ptag(tval[2]);
				provinces::province_state* pstate = &(ws.w.province_s.province_state_container[ptag]);

				cursor_in = ui::add_linear_text(cursor_in, pstate->name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();

				lm.increase_indent(1);
				cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
				lm.decrease_indent(1);
			}
			return cursor_in;
		}
		ui::xy_pair es_pop_type_scope_nation(EFFECT_DISPLAY_PARAMS) {
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
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		ui::xy_pair es_pop_type_scope_state(EFFECT_DISPLAY_PARAMS) {
			return es_pop_type_scope_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_pop_type_scope_province(EFFECT_DISPLAY_PARAMS) {
			return es_pop_type_scope_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, this_slot, from_slot, rebel_slot, gen);
		}
		ui::xy_pair es_region_scope(EFFECT_DISPLAY_PARAMS) {
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
					nullptr, this_slot, from_slot, rebel_slot);
			}

			cursor_in = display_subeffects(tval, ws, container, cursor_in, lm, fmt, nullptr, this_slot, from_slot, rebel_slot, gen);
			lm.decrease_indent(1);

			return cursor_in;
		}
		
		static ui::xy_pair(*effect_scope_display_functions[])(uint16_t const*, world_state&, ui::tagged_gui_object, ui::xy_pair, ui::unlimited_line_manager&,
			ui::text_format const&, void*, void*, void*, population::rebel_faction*, jsf_prng&) = {
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
			ui::xy_pair cursor_in, ui::unlimited_line_manager& lm, ui::text_format const& fmt, void* primary_slot, \
			void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen
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
			cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container[new_capital].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_core_tag(EFFECT_DISPLAY_PARAMS) {
			auto tag = trigger_payload(tval[2]).tag;
			text_data::text_tag adj = bool(ws.w.culture_s.national_tags_state[tag].holder) ? ws.w.culture_s.national_tags_state[tag].holder->adjective : ws.s.culture_m.national_tags[tag].default_name.adjective;
			
			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, adj),
				[](ui::tagged_gui_object) {}};

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_core_int(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_core_to], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container[prov].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_core_this_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::text_tag adj = bool(this_slot) ? ((nations::nation*)this_slot)->adjective : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, adj),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_core_this_province(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				auto owner = ((provinces::province_state*)this_slot)->owner;
				if(owner)
					return ef_add_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_add_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_add_core_this_state(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				auto owner = ((nations::state_instance*)this_slot)->owner;
				if(owner)
					return ef_add_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_add_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_add_core_this_pop(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot));
				if(owner)
					return ef_add_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_add_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_add_core_from_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::text_tag adj = bool(from_slot) ? ((nations::nation*)from_slot)->adjective : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, adj),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_add_core_from_province(EFFECT_DISPLAY_PARAMS) {
			if(from_slot) {
				if(auto owner = ((provinces::province_state*)from_slot)->owner; owner)
					return ef_add_core_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, owner, nullptr, gen);
			}
			return ef_add_core_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_add_core_reb(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::rebel]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::add_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_core_tag(EFFECT_DISPLAY_PARAMS) {
			auto tag = trigger_payload(tval[2]).tag;
			text_data::text_tag adj = bool(ws.w.culture_s.national_tags_state[tag].holder) ? ws.w.culture_s.national_tags_state[tag].holder->adjective : ws.s.culture_m.national_tags[tag].default_name.adjective;

			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, adj),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_core_int(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_core_from], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_by_space(cursor_in, ws.s.gui_m, fmt);
			cursor_in = ui::add_linear_text(cursor_in, ws.w.province_s.province_state_container[prov].name, fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_core_this_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::text_tag adj = bool(this_slot) ? ((nations::nation*)this_slot)->adjective : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation];

			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, adj),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_core_this_province(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				auto owner = ((provinces::province_state*)this_slot)->owner;
				if(owner)
					return ef_remove_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_remove_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_remove_core_this_state(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				auto owner = ((nations::state_instance*)this_slot)->owner;
				if(owner)
					return ef_remove_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_remove_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_remove_core_this_pop(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot));
				if(owner)
					return ef_remove_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_remove_core_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_remove_core_from_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::text_tag adj = bool(from_slot) ? ((nations::nation*)from_slot)->adjective : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation];

			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, adj),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_remove_core_from_province(EFFECT_DISPLAY_PARAMS) {
			if(from_slot) {
				if(auto owner = ((provinces::province_state*)from_slot)->owner; owner)
					return ef_remove_core_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, owner, nullptr, gen);
			}
			return ef_remove_core_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		
		ui::xy_pair ef_remove_core_reb(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::adj,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::rebel]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::remove_x_core], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
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
			if(this_slot) {
				auto c = ((nations::nation*)this_slot)->primary_culture;
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
			if(this_slot) {
				if(auto owner = ((nations::state_instance*)this_slot)->owner; owner)
					return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_primary_culture_this_province(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				if(auto owner = ((provinces::province_state*)this_slot)->owner; owner)
					return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_primary_culture_this_pop(EFFECT_DISPLAY_PARAMS) {
			if(this_slot) {
				if(auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot)); owner)
					return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, owner, nullptr, nullptr, gen);
			}
			return ef_primary_culture_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_primary_culture_from_nation(EFFECT_DISPLAY_PARAMS) {
			if(from_slot) {
				auto c = ((nations::nation*)from_slot)->primary_culture;
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

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_national_religion], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
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

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_national_religion], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_government_reb(EFFECT_DISPLAY_PARAMS) {
			if(rebel_slot && primary_slot) {
				auto gov = ws.s.population_m.rebel_change_government_to.get(rebel_slot->type, ((nations::nation*)primary_slot)->current_government);

				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.governments_m.governments_container[gov].name),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_national_religion], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			} else {
				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::rebel]),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::make_national_religion], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
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
			if(primary_slot) {
				return display_value(read_float_from_payload(tval + 2)* (((nations::nation*)primary_slot)->tech_attributes[technologies::tech_offset::prestige] + 1.0f),
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
			if(primary_slot) {
				auto prim_culture = ((nations::nation*)primary_slot)->primary_culture;
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
			if(primary_slot) {
				auto prim_culture = ((nations::nation*)primary_slot)->primary_culture;
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
			auto holder_name = bool(ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder) ? 
				ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder->name :
				ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, holder_name),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_province_owner], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_secede_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, bool(this_slot) ? ((nations::nation*)this_slot)->name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_province_owner], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_secede_province_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_secede_province_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot, 
				bool(this_slot) ? ((nations::state_instance*)this_slot)->owner : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_secede_province_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_secede_province_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? ((provinces::province_state*)this_slot)->owner : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_secede_province_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_secede_province_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? population::get_pop_owner(ws, *((population::pop*)this_slot)) : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_secede_province_from_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, bool(from_slot) ? ((nations::nation*)from_slot)->name : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_province_owner], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_secede_province_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_secede_province_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				nullptr, bool(from_slot) ? ((provinces::province_state*)from_slot)->owner : nullptr, nullptr, gen);
		}
		ui::xy_pair ef_secede_province_reb(EFFECT_DISPLAY_PARAMS) {
			if(rebel_slot && is_valid_index(rebel_slot->independence_tag)) {
				auto holder_name = bool(ws.w.culture_s.national_tags_state[rebel_slot->independence_tag].holder) ?
					ws.w.culture_s.national_tags_state[rebel_slot->independence_tag].holder->name :
					ws.s.culture_m.national_tags[rebel_slot->independence_tag].default_name.name;

				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, holder_name),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_province_owner], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			} else {
				text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, ws.s.fixed_ui_text[scenario::fixed_ui::rebel]),
					[](ui::tagged_gui_object) {} };

				cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::change_province_owner], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
				cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
				lm.finish_current_line();
				return cursor_in;
			}
		}
		ui::xy_pair ef_inherit(EFFECT_DISPLAY_PARAMS) {
			auto holder_name = bool(ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder) ?
				ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder->name :
				ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

			text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, holder_name),
					[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::annex_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_inherit_this_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, bool(this_slot) ? ((nations::nation*)this_slot)->name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation]),
					[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::annex_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_inherit_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_inherit_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? ((nations::state_instance*)this_slot)->owner : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_inherit_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_inherit_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? ((provinces::province_state*)this_slot)->owner : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_inherit_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_inherit_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? population::get_pop_owner(ws, *((population::pop*)this_slot)) : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_inherit_from_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
				text_data::value_type::text,
				text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, bool(from_slot) ? ((nations::nation*)from_slot)->name : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation]),
				[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::annex_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_inherit_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_inherit_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				nullptr, bool(from_slot) ? ((provinces::province_state*)from_slot)->owner : nullptr, nullptr, gen);
		}
		ui::xy_pair ef_annex_to(EFFECT_DISPLAY_PARAMS) {
			auto holder_name = bool(ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder) ?
				ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder->name :
				ws.s.culture_m.national_tags[trigger_payload(tval[2]).tag].default_name.name;

			text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, holder_name),
					[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::annexed_by_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_annex_to_this_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, bool(this_slot) ? ((nations::nation*)this_slot)->name : ws.s.fixed_ui_text[scenario::fixed_ui::this_nation]),
					[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::annexed_by_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_annex_to_this_state(EFFECT_DISPLAY_PARAMS) {
			return ef_annex_to_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? ((nations::state_instance*)this_slot)->owner : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_annex_to_this_province(EFFECT_DISPLAY_PARAMS) {
			return ef_annex_to_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? ((provinces::province_state*)this_slot)->owner : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_annex_to_this_pop(EFFECT_DISPLAY_PARAMS) {
			return ef_annex_to_this_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				bool(this_slot) ? population::get_pop_owner(ws, *((population::pop*)this_slot)) : nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_annex_to_from_nation(EFFECT_DISPLAY_PARAMS) {
			text_data::replacement repl{
					text_data::value_type::text,
					text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, bool(from_slot) ? ((nations::nation*)from_slot)->name : ws.s.fixed_ui_text[scenario::fixed_ui::from_nation]),
					[](ui::tagged_gui_object) {} };

			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::annexed_by_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm, &repl, 1);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_annex_to_from_province(EFFECT_DISPLAY_PARAMS) {
			return ef_annex_to_from_nation(tval, ws, container, cursor_in, lm, fmt, primary_slot,
				nullptr, bool(from_slot) ? ((provinces::province_state*)from_slot)->owner : nullptr, nullptr, gen);
		}
		ui::xy_pair ef_release(EFFECT_DISPLAY_PARAMS) {
			nations::liberate_uncored_cores(ws, *((nations::nation*)primary_slot), trigger_payload(tval[2]).tag);
		}
		ui::xy_pair ef_release_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(is_valid_index(((nations::nation*)this_slot)->tag))
				nations::liberate_uncored_cores(ws, *((nations::nation*)primary_slot), ((nations::nation*)this_slot)->tag);
		}
		ui::xy_pair ef_release_this_state(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((nations::state_instance*)this_slot)->owner;
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_release_this_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_release_this_pop(EFFECT_DISPLAY_PARAMS) {
			auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot));
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_release_from_nation(EFFECT_DISPLAY_PARAMS) {
			if(is_valid_index(((nations::nation*)from_slot)->tag))
				nations::liberate_uncored_cores(ws, *((nations::nation*)primary_slot), ((nations::nation*)from_slot)->tag);
		}
		ui::xy_pair ef_release_from_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				ef_release_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_change_controller(EFFECT_DISPLAY_PARAMS) {
			auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
			if(holder)
				provinces::silent_set_province_controller(ws, *holder, *((provinces::province_state*)primary_slot));
		}
		ui::xy_pair ef_change_controller_this_nation(EFFECT_DISPLAY_PARAMS) {
			provinces::silent_set_province_controller(ws, *((nations::nation*)this_slot), *((provinces::province_state*)primary_slot));
		}
		ui::xy_pair ef_change_controller_this_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				provinces::silent_set_province_controller(ws, *owner, *((provinces::province_state*)primary_slot));
		}
		ui::xy_pair ef_change_controller_from_nation(EFFECT_DISPLAY_PARAMS) {
			provinces::silent_set_province_controller(ws, *((nations::nation*)from_slot), *((provinces::province_state*)primary_slot));
		}
		ui::xy_pair ef_change_controller_from_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				provinces::silent_set_province_controller(ws, *owner, *((provinces::province_state*)primary_slot));
		}
		ui::xy_pair ef_infrastructure(EFFECT_DISPLAY_PARAMS) {
			((provinces::province_state*)primary_slot)->railroad_level =
				uint8_t(std::clamp(
					int32_t(((provinces::province_state*)primary_slot)->railroad_level) + int32_t(trigger_payload(tval[2]).signed_value), 0, 255));
		}
		ui::xy_pair ef_money(EFFECT_DISPLAY_PARAMS) {
			((population::pop*)primary_slot)->money += read_float_from_payload(tval + 2);
		}
		ui::xy_pair ef_leadership(EFFECT_DISPLAY_PARAMS) {
			((nations::nation*)primary_slot)->leadership_points += trigger_payload(tval[2]).signed_value;
		}
		ui::xy_pair ef_create_vassal(EFFECT_DISPLAY_PARAMS) {
			auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
			if(holder && get_size(ws.w.province_s.province_arrays, holder->owned_provinces) != 0)
				nations::make_vassal(ws, *((nations::nation*)primary_slot), *holder);
		}
		ui::xy_pair ef_create_vassal_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(get_size(ws.w.province_s.province_arrays, ((nations::nation*)this_slot)->owned_provinces) != 0)
				nations::make_vassal(ws, *((nations::nation*)primary_slot), *((nations::nation*)this_slot));
		}
		ui::xy_pair ef_create_vassal_this_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				ef_create_vassal_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_create_vassal_from_nation(EFFECT_DISPLAY_PARAMS) {
			if(get_size(ws.w.province_s.province_arrays, ((nations::nation*)from_slot)->owned_provinces) != 0)
				nations::make_vassal(ws, *((nations::nation*)primary_slot), *((nations::nation*)from_slot));
		}
		ui::xy_pair ef_create_vassal_from_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				ef_create_vassal_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
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
			auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
			if(holder)
				nations::end_alliance(ws, *((nations::nation*)primary_slot), *holder);
		}
		ui::xy_pair ef_leave_alliance_this_nation(EFFECT_DISPLAY_PARAMS) {
			nations::end_alliance(ws, *((nations::nation*)primary_slot), *((nations::nation*)this_slot));
		}
		ui::xy_pair ef_leave_alliance_this_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				nations::end_alliance(ws, *((nations::nation*)primary_slot), *owner);
		}
		ui::xy_pair ef_leave_alliance_from_nation(EFFECT_DISPLAY_PARAMS) {
			nations::end_alliance(ws, *((nations::nation*)primary_slot), *((nations::nation*)from_slot));
		}
		ui::xy_pair ef_leave_alliance_from_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				nations::end_alliance(ws, *((nations::nation*)primary_slot), *owner);
		}
		ui::xy_pair ef_end_war(EFFECT_DISPLAY_PARAMS) {
			auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
			if(holder) {
				auto war_between = military::get_war_between(ws, *((nations::nation*)primary_slot), holder->id);
				if(war_between) {
					military::destroy_war(ws, *war_between);
					ws.w.military_s.wars.remove(war_between->id);
				}
			}
		}
		ui::xy_pair ef_end_war_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto war_between = military::get_war_between(ws, *((nations::nation*)primary_slot), ((nations::nation*)this_slot)->id);
			if(war_between) {
				military::destroy_war(ws, *war_between);
				ws.w.military_s.wars.remove(war_between->id);
			}
		}
		ui::xy_pair ef_end_war_this_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				ef_end_war_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_end_war_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto war_between = military::get_war_between(ws, *((nations::nation*)primary_slot), ((nations::nation*)from_slot)->id);
			if(war_between) {
				military::destroy_war(ws, *war_between);
				ws.w.military_s.wars.remove(war_between->id);
			}
		}
		ui::xy_pair ef_end_war_from_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				ef_end_war_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_enable_ideology(EFFECT_DISPLAY_PARAMS) {
			ws.w.ideology_s.ideology_enabled[trigger_payload(tval[2]).small.values.ideology] = 1ui8;
		}
		ui::xy_pair ef_ruling_party_ideology(EFFECT_DISPLAY_PARAMS) {
			auto new_party = ws.w.nation_s.active_parties.get(((nations::nation*)primary_slot)->id, trigger_payload(tval[2]).small.values.ideology);
			if(is_valid_index(new_party) && new_party != ((nations::nation*)primary_slot)->ruling_party)
				governments::silent_set_ruling_party(ws, *((nations::nation*)primary_slot), new_party);
		}
		ui::xy_pair ef_plurality(EFFECT_DISPLAY_PARAMS) {
			((nations::nation*)primary_slot)->plurality = std::clamp(((nations::nation*)primary_slot)->plurality + read_float_from_payload(tval + 2), 0.0f, 1.0f);
		}
		ui::xy_pair ef_remove_province_modifier(EFFECT_DISPLAY_PARAMS) {
			auto pmod = trigger_payload(tval[2]).prov_mod;
			modifiers::remove_static_modifier_from_province(ws, *((provinces::province_state*)primary_slot), pmod);
			modifiers::remove_all_timed_modifiers_from_province(ws, *((provinces::province_state*)primary_slot), pmod);
		}
		ui::xy_pair ef_remove_country_modifier(EFFECT_DISPLAY_PARAMS) {
			auto nmod = trigger_payload(tval[2]).nat_mod;
			modifiers::remove_all_static_modifiers_from_nation(ws, *((nations::nation*)primary_slot), nmod);
			modifiers::remove_all_timed_modifiers_from_nation(ws, *((nations::nation*)primary_slot), nmod);
		}
		ui::xy_pair ef_create_alliance(EFFECT_DISPLAY_PARAMS) {
			auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
			if(holder)
				nations::make_alliance(ws, *((nations::nation*)primary_slot), *holder);
		}
		ui::xy_pair ef_create_alliance_this_nation(EFFECT_DISPLAY_PARAMS) {
			nations::make_alliance(ws, *((nations::nation*)primary_slot), *((nations::nation*)this_slot));
		}
		ui::xy_pair ef_create_alliance_this_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				nations::make_alliance(ws, *((nations::nation*)primary_slot), *owner);
		}
		ui::xy_pair ef_create_alliance_from_nation(EFFECT_DISPLAY_PARAMS) {
			nations::make_alliance(ws, *((nations::nation*)primary_slot), *((nations::nation*)from_slot));
		}
		ui::xy_pair ef_create_alliance_from_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				nations::make_alliance(ws, *((nations::nation*)primary_slot), *owner);
		}
		ui::xy_pair ef_release_vassal(EFFECT_DISPLAY_PARAMS) {
			if(auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder; bool(holder) && get_size(ws.w.province_s.province_arrays, holder->owned_provinces) != 0) {
				nations::free_vassal(ws, *holder);
			} else {
				auto& new_nation = nations::liberate_uncored_cores(ws, *((nations::nation*)primary_slot), trigger_payload(tval[2]).tag);
				if(get_size(ws.w.province_s.province_arrays, new_nation.owned_provinces) != 0)
					nations::make_vassal(ws, *((nations::nation*)primary_slot), new_nation);
			}
		}
		ui::xy_pair ef_release_vassal_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(get_size(ws.w.province_s.province_arrays, ((nations::nation*)this_slot)->owned_provinces) != 0) {
				nations::free_vassal(ws, *((nations::nation*)this_slot));
			} else {
				auto& new_nation = nations::liberate_uncored_cores(ws, *((nations::nation*)primary_slot), ((nations::nation*)this_slot)->tag);
				if(get_size(ws.w.province_s.province_arrays, new_nation.owned_provinces) != 0)
					nations::make_vassal(ws, *((nations::nation*)primary_slot), new_nation);
			}
		}
		ui::xy_pair ef_release_vassal_this_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)this_slot)->owner;
			if(owner)
				ef_release_vassal_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_release_vassal_from_nation(EFFECT_DISPLAY_PARAMS) {
			ef_release_vassal_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_release_vassal_from_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)from_slot)->owner;
			if(owner)
				ef_release_vassal_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_release_vassal_reb(EFFECT_DISPLAY_PARAMS) {
			nations::liberate_all_cores(ws, *((nations::nation*)primary_slot), rebel_slot->independence_tag);
		}
		ui::xy_pair ef_release_vassal_random(EFFECT_DISPLAY_PARAMS) {
			cursor_in = ui::add_linear_text(cursor_in, ws.s.fixed_ui_text[scenario::fixed_ui::no_effect], fmt, ws.s.gui_m, ws.w.gui_m, container, lm);
			cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt);
			lm.finish_current_line();
			return cursor_in;
		}
		ui::xy_pair ef_change_province_name(EFFECT_DISPLAY_PARAMS) {
			((provinces::province_state*)primary_slot)->name = trigger_payload(tval[2]).text;
		}
		ui::xy_pair ef_enable_canal(EFFECT_DISPLAY_PARAMS) {
			provinces::enable_canal(ws, tval[2] - 1);
		}
		ui::xy_pair ef_set_global_flag(EFFECT_DISPLAY_PARAMS) {
			ws.w.variable_s.global_variables[trigger_payload(tval[2]).global_var] = 1.0f;
		}
		ui::xy_pair ef_clr_global_flag(EFFECT_DISPLAY_PARAMS) {
			ws.w.variable_s.global_variables[trigger_payload(tval[2]).global_var] = 0.0f;
		}
		ui::xy_pair ef_nationalvalue_province(EFFECT_DISPLAY_PARAMS) {
			auto owner = ((provinces::province_state*)primary_slot)->owner;
			if(owner)
				owner->national_value = trigger_payload(tval[2]).nat_mod;
		}
		ui::xy_pair ef_nationalvalue_nation(EFFECT_DISPLAY_PARAMS) {
			((nations::nation*)primary_slot)->national_value = trigger_payload(tval[2]).nat_mod;
		}
		ui::xy_pair ef_civilized_yes(EFFECT_DISPLAY_PARAMS) {
			nations::civilize_nation(ws, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_civilized_no(EFFECT_DISPLAY_PARAMS) {
			nations::uncivilize_nation(ws, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_is_slave_state_no(EFFECT_DISPLAY_PARAMS) {
			nations::unmake_slave_state(ws, *((nations::state_instance*)primary_slot));
		}
		ui::xy_pair ef_is_slave_pop_no(EFFECT_DISPLAY_PARAMS) {
			if(((population::pop*)primary_slot)->type == ws.s.population_m.slave)
				population::free_slave(ws, *((population::pop*)primary_slot));
		}
		ui::xy_pair ef_election(EFFECT_DISPLAY_PARAMS) {
			if(ws.s.governments_m.governments_container[((nations::nation*)primary_slot)->current_government].election)
				governments::start_election(ws, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_social_reform(EFFECT_DISPLAY_PARAMS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_political_reform(EFFECT_DISPLAY_PARAMS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_add_tax_relative_income(EFFECT_DISPLAY_PARAMS) {
			ws.w.nation_s.national_stockpiles.get(((nations::nation*)primary_slot)->id, economy::money_good) += economy::goods_qnty_type(((nations::nation*)primary_slot)->last_income * economy::money_qnty_type(read_float_from_payload(tval + 2)));
		}
		ui::xy_pair ef_neutrality(EFFECT_DISPLAY_PARAMS) {
			auto allies_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->allies);
			boost::container::small_vector<nations::country_tag, 16> allies_copy(allies_range.first, allies_range.second);

			for(auto a : allies_copy)
				nations::end_alliance(ws, *((nations::nation*)primary_slot), ws.w.nation_s.nations[a]);

			auto vassals_range = get_range(ws.w.nation_s.nations_arrays, ((nations::nation*)primary_slot)->vassals);
			boost::container::small_vector<nations::country_tag, 16> vassals_copy(vassals_range.first, vassals_range.second);

			for(auto a : vassals_copy)
				nations::free_vassal(ws, ws.w.nation_s.nations[a]);
		}
		ui::xy_pair ef_reduce_pop(EFFECT_DISPLAY_PARAMS) {
			population::change_pop_size(
				ws, *((population::pop*)primary_slot),
				int32_t(ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_population_tag) * read_float_from_payload(tval + 2)));
		}
		ui::xy_pair ef_move_pop(EFFECT_DISPLAY_PARAMS) {
			population::change_pop_location(ws, *((population::pop*)primary_slot), provinces::province_tag(tval[2]));
		}
		ui::xy_pair ef_pop_type(EFFECT_DISPLAY_PARAMS) {
			population::change_pop_type(ws, *((population::pop*)primary_slot), trigger_payload(tval[2]).small.values.pop_type);
		}
		ui::xy_pair ef_years_of_research(EFFECT_DISPLAY_PARAMS) {
			((nations::nation*)primary_slot)->research_points =
				int16_t(float(((nations::nation*)primary_slot)->research_points) +
					technologies::daily_research_points(ws, *((nations::nation*)primary_slot)) * 365.0f * read_float_from_payload(tval + 2));
		}
		ui::xy_pair ef_prestige_factor_positive(EFFECT_DISPLAY_PARAMS) {
			((nations::nation*)primary_slot)->base_prestige +=
				(((nations::nation*)primary_slot)->base_prestige + ((nations::nation*)primary_slot)->tech_attributes[technologies::tech_offset::permanent_prestige]) *
				(((nations::nation*)primary_slot)->tech_attributes[technologies::tech_offset::prestige] + 1.0f) *
				read_float_from_payload(tval + 2);
		}
		ui::xy_pair ef_prestige_factor_negative(EFFECT_DISPLAY_PARAMS) {
			((nations::nation*)primary_slot)->base_prestige = std::max(
				0.0f,
				((nations::nation*)primary_slot)->base_prestige +
				(((nations::nation*)primary_slot)->base_prestige + ((nations::nation*)primary_slot)->tech_attributes[technologies::tech_offset::permanent_prestige]) *
				read_float_from_payload(tval + 2));
		}
		ui::xy_pair ef_military_reform(EFFECT_DISPLAY_PARAMS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_economic_reform(EFFECT_DISPLAY_PARAMS) {
			issues::change_issue_option(ws, trigger_payload(tval[2]).small.values.option, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_remove_random_military_reforms(EFFECT_DISPLAY_PARAMS) {
			boost::container::small_vector<issues::issue_tag, 16> active_reforms;
			auto nation_id = ((nations::nation*)primary_slot)->id;
			for(auto issue : ws.s.issues_m.military_issues) {
				if(ws.w.nation_s.active_issue_options.get(nation_id, issue) != ws.s.issues_m.issues_container[issue].options[0])
					active_reforms.push_back(issue);
			}
			for(int32_t i = tval[2] - 1; active_reforms.size() != 0 && i >= 0; --i) {
				std::uniform_int_distribution<int32_t> dist(0, int32_t(active_reforms.size()) - 1);
				auto chance_taken = uint32_t(dist(gen));
				issues::change_issue_option(ws, ws.s.issues_m.issues_container[active_reforms[chance_taken]].options[0], *((nations::nation*)primary_slot));
			}
		}
		ui::xy_pair ef_remove_random_economic_reforms(EFFECT_DISPLAY_PARAMS) {
			boost::container::small_vector<issues::issue_tag, 16> active_reforms;
			auto nation_id = ((nations::nation*)primary_slot)->id;
			for(auto issue : ws.s.issues_m.economic_issues) {
				if(ws.w.nation_s.active_issue_options.get(nation_id, issue) != ws.s.issues_m.issues_container[issue].options[0])
					active_reforms.push_back(issue);
			}
			for(int32_t i = tval[2] - 1; active_reforms.size() != 0 && i >= 0; --i) {
				std::uniform_int_distribution<int32_t> dist(0, int32_t(active_reforms.size()) - 1);
				auto chance_taken = uint32_t(dist(gen));
				issues::change_issue_option(ws, ws.s.issues_m.issues_container[active_reforms[chance_taken]].options[0], *((nations::nation*)primary_slot));
			}
		}
		ui::xy_pair ef_add_crime(EFFECT_DISPLAY_PARAMS) {
			((provinces::province_state*)primary_slot)->crime = trigger_payload(tval[2]).prov_mod;
		}
		ui::xy_pair ef_add_crime_none(EFFECT_DISPLAY_PARAMS) {
			((provinces::province_state*)primary_slot)->crime = modifiers::provincial_modifier_tag();
		}
		ui::xy_pair ef_nationalize(EFFECT_DISPLAY_PARAMS) {
			nations::perform_nationalization(ws, *((nations::nation*)primary_slot));
		}
		ui::xy_pair ef_build_factory_in_capital_state(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			auto factory_type = &(ws.s.economy_m.factory_types[trigger_payload(tval[2]).small.values.factory]);
			if(is_valid_index(capital)) {
				if(auto si = ws.w.province_s.province_state_container[capital].state_instance; si) {
					int32_t free_index = -1;
					for(int32_t i = int32_t(std::extent_v<decltype(si->factories)>) - 1; i >= 0; --i) {
						if(si->factories[i].type == factory_type)
							return;
						else if(si->factories[i].type == nullptr)
							free_index = i;
					}
					if(free_index != -1) {
						si->factories[free_index].type = factory_type;
						si->factories[free_index].level = 1ui16;
					}
				}
			}
		}
		ui::xy_pair ef_activate_technology(EFFECT_DISPLAY_PARAMS) {
			bit_vector_set(ws.w.nation_s.active_technologies.get_row(((nations::nation*)primary_slot)->id), to_index(trigger_payload(tval[2]).tech), true);
			technologies::apply_single_technology(ws, *((nations::nation*)primary_slot), trigger_payload(tval[2]).tech);
		}
		ui::xy_pair ef_great_wars_enabled_yes(EFFECT_DISPLAY_PARAMS) {
			ws.w.great_wars_enabled = true;
		}
		ui::xy_pair ef_great_wars_enabled_no(EFFECT_DISPLAY_PARAMS) {
			ws.w.great_wars_enabled = false;
		}
		ui::xy_pair ef_world_wars_enabled_yes(EFFECT_DISPLAY_PARAMS) {
			ws.w.world_wars_enabled = true;
		}
		ui::xy_pair ef_world_wars_enabled_no(EFFECT_DISPLAY_PARAMS) {
			ws.w.world_wars_enabled = false;
		}
		ui::xy_pair ef_assimilate_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner) {
				provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [owner_culture = owner->primary_culture](population::pop& p){
					p.culture = owner_culture;
				});
			}
		}
		ui::xy_pair ef_assimilate_pop(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = population::get_pop_owner(ws, *((population::pop*)primary_slot)); owner) {
				((population::pop*)primary_slot)->culture = owner->primary_culture;
			}
		}
		ui::xy_pair ef_literacy(EFFECT_DISPLAY_PARAMS) {
			population::set_literacy_direct(*((population::pop*)primary_slot), population::get_literacy_direct(*((population::pop*)primary_slot)) + read_float_from_payload(tval + 2));
		}
		ui::xy_pair ef_add_crisis_interest(EFFECT_DISPLAY_PARAMS) {
			if(ws.w.current_crisis_type != current_state::crisis_type::none)
				add_item(ws.w.nation_s.nations_arrays, ws.w.crisis_interested, ((nations::nation*)primary_slot)->id);
		}
		ui::xy_pair ef_flashpoint_tension(EFFECT_DISPLAY_PARAMS) {
			((nations::state_instance*)primary_slot)->current_tension = std::clamp(((nations::state_instance*)primary_slot)->current_tension + read_float_from_payload(tval + 2), 0.0f, 100.0f);
		}
		ui::xy_pair ef_add_crisis_temperature(uint16_t const* tval, world_state& ws, void* primary_slot, void* this_slot, void* from_slot, population::rebel_faction* rebel_slot, jsf_prng& gen) {
			ws.w.crisis_temperature = std::clamp(ws.w.crisis_temperature + read_float_from_payload(tval + 2), 0.0f, 100.0f);
		}
		ui::xy_pair ef_consciousness(EFFECT_DISPLAY_PARAMS) {
			population::set_consciousness_direct(*((population::pop*)primary_slot), population::get_consciousness_direct(*((population::pop*)primary_slot)) + read_float_from_payload(tval + 2));
		}
		ui::xy_pair ef_militancy(EFFECT_DISPLAY_PARAMS) {
			population::set_militancy_direct(*((population::pop*)primary_slot), population::get_militancy_direct(*((population::pop*)primary_slot)) + read_float_from_payload(tval + 2));
		}
		ui::xy_pair ef_rgo_size(EFFECT_DISPLAY_PARAMS) {
			((provinces::province_state*)primary_slot)->rgo_size =
				uint8_t(std::clamp(int32_t(((provinces::province_state*)primary_slot)->rgo_size + trigger_payload(tval[2]).signed_value), 0, 255));
		}
		ui::xy_pair ef_fort(EFFECT_DISPLAY_PARAMS) {
			((provinces::province_state*)primary_slot)->fort_level =
				uint8_t(std::clamp(
					int32_t(((provinces::province_state*)primary_slot)->fort_level) + int32_t(trigger_payload(tval[2]).signed_value), 0, 255));
		}
		ui::xy_pair ef_naval_base(EFFECT_DISPLAY_PARAMS) {
			((provinces::province_state*)primary_slot)->naval_base_level =
				uint8_t(std::clamp(
					int32_t(((provinces::province_state*)primary_slot)->naval_base_level) + int32_t(trigger_payload(tval[2]).signed_value), 0, 255));
		}
		ui::xy_pair ef_trigger_revolt_nation(EFFECT_DISPLAY_PARAMS) {
			auto rf_range = get_range(ws.w.population_s.rebel_faction_arrays, ((nations::nation*)primary_slot)->active_rebel_factions);

			auto type = trigger_payload(tval[2]).small.values.rebel;
			auto culture = trigger_payload(tval[3]).culture;
			auto religion = trigger_payload(tval[4]).small.values.religion;
			auto ideology = trigger_payload(tval[5]).small.values.ideology;

			for(auto rf : rf_range) {
				auto& rf_obj = ws.w.population_s.rebel_factions[rf];
				if((!is_valid_index(type) || rf_obj.type == type) &&
					(!is_valid_index(culture) || rf_obj.culture == culture) &&
					(!is_valid_index(religion) || rf_obj.religion == religion) &&
					(!is_valid_index(ideology) || rf_obj.ideology == ideology)) {
					population::trigger_rising(ws, rf_obj, *((nations::nation*)primary_slot));
				}
			}
		}
		ui::xy_pair ef_trigger_revolt_state(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((nations::state_instance*)primary_slot)->owner; owner)
				ef_trigger_revolt_nation(tval, ws, owner, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_trigger_revolt_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_trigger_revolt_nation(tval, ws, owner, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_diplomatic_influence(EFFECT_DISPLAY_PARAMS) {
			if(auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder; holder)
				nations::adjust_influence(ws, *((nations::nation*)primary_slot), holder->id, trigger_payload(tval[3]).signed_value);
		}
		ui::xy_pair ef_diplomatic_influence_this_nation(EFFECT_DISPLAY_PARAMS) {
			nations::adjust_influence(ws, *((nations::nation*)primary_slot), ((nations::nation*)this_slot)->id, trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_diplomatic_influence_this_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner)
				nations::adjust_influence(ws, *((nations::nation*)primary_slot), owner->id, trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_diplomatic_influence_from_nation(EFFECT_DISPLAY_PARAMS) {
			nations::adjust_influence(ws, *((nations::nation*)primary_slot), ((nations::nation*)from_slot)->id, trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_diplomatic_influence_from_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner)
				nations::adjust_influence(ws, *((nations::nation*)primary_slot), owner->id, trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_relation(EFFECT_DISPLAY_PARAMS) {
			if(auto holder = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder; holder)
				nations::adjust_relationship(ws, *((nations::nation*)primary_slot), *holder, trigger_payload(tval[3]).signed_value);
		}
		ui::xy_pair ef_relation_this_nation(EFFECT_DISPLAY_PARAMS) {
			nations::adjust_relationship(ws, *((nations::nation*)primary_slot), *((nations::nation*)this_slot), trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_relation_this_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner)
				nations::adjust_relationship(ws, *((nations::nation*)primary_slot), *owner, trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_relation_from_nation(EFFECT_DISPLAY_PARAMS) {
			nations::adjust_relationship(ws, *((nations::nation*)primary_slot), *((nations::nation*)from_slot), trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_relation_from_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner)
				nations::adjust_relationship(ws, *((nations::nation*)primary_slot), *owner, trigger_payload(tval[2]).signed_value);
		}
		ui::xy_pair ef_add_province_modifier(EFFECT_DISPLAY_PARAMS) {
			modifiers::add_timed_modifier_to_province(
				ws,
				*((provinces::province_state*)primary_slot),
				trigger_payload(tval[2]).prov_mod,
				date_tag(to_index(ws.w.current_date) + trigger_payload(tval[3]).signed_value));
		}
		ui::xy_pair ef_add_province_modifier_no_duration(EFFECT_DISPLAY_PARAMS) {
			modifiers::add_static_modifier_to_province(ws, *((provinces::province_state*)primary_slot), trigger_payload(tval[2]).prov_mod);
		}
		ui::xy_pair ef_add_country_modifier(EFFECT_DISPLAY_PARAMS) {
			modifiers::add_timed_modifier_to_nation(
				ws,
				*((nations::nation*)primary_slot),
				trigger_payload(tval[2]).nat_mod,
				date_tag(to_index(ws.w.current_date) + trigger_payload(tval[3]).signed_value));
		}
		ui::xy_pair ef_add_country_modifier_no_duration(EFFECT_DISPLAY_PARAMS) {
			modifiers::add_unique_static_modifier_to_nation(ws, *((nations::nation*)primary_slot), trigger_payload(tval[2]).nat_mod);
		}
		ui::xy_pair ef_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;
			auto tag_target = trigger_payload(tval[4]).tag;

			if(auto holder = ws.w.culture_s.national_tags_state[tag_target].holder; holder) {
				add_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					holder->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		ui::xy_pair ef_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ws.w.province_s.province_state_container[provinces::province_tag(tval[4])].owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		ui::xy_pair ef_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays,
				((nations::nation*)primary_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)this_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
			});
		}
		ui::xy_pair ef_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ((nations::state_instance*)this_slot)->owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		ui::xy_pair ef_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		ui::xy_pair ef_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		ui::xy_pair ef_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays,
				((nations::nation*)primary_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)from_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
			});
		}
		ui::xy_pair ef_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
				});
			}
		}
		ui::xy_pair ef_add_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;
			auto tag_target = trigger_payload(tval[4]).tag;

			if(auto holder = ws.w.culture_s.national_tags_state[tag_target].holder; holder) {
				add_item(ws.w.military_s.cb_arrays,
					holder->active_cbs,
					military::pending_cb{
						((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
					});
			}
		}
		ui::xy_pair ef_add_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ws.w.province_s.province_state_container[provinces::province_tag(tval[4])].owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{
						((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
					});
			}
		}
		ui::xy_pair ef_add_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays,
				((nations::nation*)this_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
			});
		}
		ui::xy_pair ef_add_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ((nations::state_instance*)this_slot)->owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{
						((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
					});
			}
		}
		ui::xy_pair ef_add_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{
						((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
					});
			}
		}
		ui::xy_pair ef_add_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot)); owner) {
				add_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{
						((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
					});
			}
		}
		ui::xy_pair ef_add_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			add_item(ws.w.military_s.cb_arrays,
				((nations::nation*)from_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
			});
		}
		ui::xy_pair ef_add_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto months = trigger_payload(tval[3]).signed_value;

			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner) {
				add_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{
						((nations::nation*)primary_slot)->id, type, months > 0 ? date_tag(to_index(ws.w.current_date) + months * 30) : date_tag()
					});
			}
		}
		ui::xy_pair ef_remove_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto tag_target = trigger_payload(tval[3]).tag;

			if(auto holder = ws.w.culture_s.national_tags_state[tag_target].holder; holder) {
				remove_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					holder->id, type, date_tag()
				});
			}
		}
		ui::xy_pair ef_remove_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ws.w.province_s.province_state_container[provinces::province_tag(tval[3])].owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, date_tag()
				});
			}
		}
		ui::xy_pair ef_remove_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				((nations::nation*)primary_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)this_slot)->id, type, date_tag()
			});
		}
		ui::xy_pair ef_remove_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ((nations::state_instance*)this_slot)->owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, date_tag()
				});
			}
		}
		ui::xy_pair ef_remove_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, date_tag()
				});
			}
		}
		ui::xy_pair ef_remove_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, date_tag()
				});
			}
		}
		ui::xy_pair ef_remove_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				((nations::nation*)primary_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)from_slot)->id, type, date_tag()
			});
		}
		ui::xy_pair ef_remove_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					((nations::nation*)primary_slot)->active_cbs,
					military::pending_cb {
					owner->id, type, date_tag()
				});
			}
		}
		ui::xy_pair ef_this_remove_casus_belli_tag(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;
			auto tag_target = trigger_payload(tval[3]).tag;

			if(auto holder = ws.w.culture_s.national_tags_state[tag_target].holder; holder) {
				remove_item(ws.w.military_s.cb_arrays,
					holder->active_cbs,
					military::pending_cb{ ((nations::nation*)primary_slot)->id, type, date_tag() });
			}
		}
		ui::xy_pair ef_this_remove_casus_belli_int(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ws.w.province_s.province_state_container[provinces::province_tag(tval[3])].owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{ ((nations::nation*)primary_slot)->id, type, date_tag() });
			}
		}
		ui::xy_pair ef_this_remove_casus_belli_this_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				((nations::nation*)this_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)primary_slot)->id, type, date_tag()
			});
		}
		ui::xy_pair ef_this_remove_casus_belli_this_state(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ((nations::state_instance*)this_slot)->owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{ ((nations::nation*)primary_slot)->id, type, date_tag() });
			}
		}
		ui::xy_pair ef_this_remove_casus_belli_this_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{ ((nations::nation*)primary_slot)->id, type, date_tag() });
			}
		}
		ui::xy_pair ef_this_remove_casus_belli_this_pop(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot)); owner) {
				remove_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{ ((nations::nation*)primary_slot)->id, type, date_tag() });
			}
		}
		ui::xy_pair ef_this_remove_casus_belli_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			remove_item(ws.w.military_s.cb_arrays,
				((nations::nation*)from_slot)->active_cbs,
				military::pending_cb {
				((nations::nation*)primary_slot)->id, type, date_tag()
			});
		}
		ui::xy_pair ef_this_remove_casus_belli_from_province(EFFECT_DISPLAY_PARAMS) {
			auto type = trigger_payload(tval[2]).small.values.cb_type;

			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner) {
				remove_item(ws.w.military_s.cb_arrays,
					owner->active_cbs,
					military::pending_cb{ ((nations::nation*)primary_slot)->id, type, date_tag() });
			}
		}
		ui::xy_pair ef_war_tag(EFFECT_DISPLAY_PARAMS) {
			auto target = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
			if(target) {
				auto& new_war = military::create_war(ws, *((nations::nation*)primary_slot), *target, true);

				auto attacker_wg_count = tval[3];
				uint32_t payload_index = 4;
				for(int32_t i = 0; i < attacker_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = tval[payload_index + 1];
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = [&ws, province_id]() {
						if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
							return si->id;
						else
							return nations::state_tag();
					}();

					add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
						military::war_goal{ ws.w.current_date, 0.0f, ((nations::nation*)primary_slot)->id, state, target->id, liberate_country_tag, cb_type });
					payload_index += 3;
				}

				auto defender_wg_count = tval[payload_index];
				++payload_index;
				for(int32_t i = 0; i < defender_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = tval[payload_index + 1];
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = [&ws, province_id]() {
						if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
							return si->id;
						else
							return nations::state_tag();
					}();

					add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
						military::war_goal{ ws.w.current_date, 0.0f, target->id, state, ((nations::nation*)primary_slot)->id, liberate_country_tag, cb_type });
					payload_index += 3;
				}
			}
		}
		ui::xy_pair ef_war_this_nation(EFFECT_DISPLAY_PARAMS) {
			nations::nation* target = ((nations::nation*)this_slot);

			auto& new_war = military::create_war(ws, *((nations::nation*)primary_slot), *target, true);

			auto attacker_wg_count = tval[2];
			uint32_t payload_index = 3;
			for(int32_t i = 0; i < attacker_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = tval[payload_index + 1];
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = [&ws, province_id]() {
					if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
						return si->id;
					else
						return nations::state_tag();
				}();

				add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
					military::war_goal{ ws.w.current_date, 0.0f, ((nations::nation*)primary_slot)->id, state, target->id, liberate_country_tag, cb_type });
				payload_index += 3;
			}

			auto defender_wg_count = tval[payload_index];
			++payload_index;
			for(int32_t i = 0; i < defender_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = tval[payload_index + 1];
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = [&ws, province_id]() {
					if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
						return si->id;
					else
						return nations::state_tag();
				}();

				add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
					military::war_goal{ ws.w.current_date, 0.0f, target->id, state, ((nations::nation*)primary_slot)->id, liberate_country_tag, cb_type });
				payload_index += 3;
			}
		}
		ui::xy_pair ef_war_this_state(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((nations::state_instance*)this_slot)->owner; owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_this_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_this_pop(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot)); owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_from_nation(EFFECT_DISPLAY_PARAMS) {
			ef_war_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_from_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner)
				ef_war_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_no_ally_tag(EFFECT_DISPLAY_PARAMS) {
			auto target = ws.w.culture_s.national_tags_state[trigger_payload(tval[2]).tag].holder;
			if(target) {
				auto& new_war = military::create_war(ws, *((nations::nation*)primary_slot), *target, false);

				auto attacker_wg_count = tval[3];
				uint32_t payload_index = 4;
				for(int32_t i = 0; i < attacker_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = tval[payload_index + 1];
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = [&ws, province_id]() {
						if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
							return si->id;
						else
							return nations::state_tag();
					}();

					add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
						military::war_goal{ ws.w.current_date, 0.0f, ((nations::nation*)primary_slot)->id, state, target->id, liberate_country_tag, cb_type });
					payload_index += 3;
				}

				auto defender_wg_count = tval[payload_index];
				++payload_index;
				for(int32_t i = 0; i < defender_wg_count; ++i) {
					auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
					auto province_id = tval[payload_index + 1];
					auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
					auto state = [&ws, province_id]() {
						if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
							return si->id;
						else
							return nations::state_tag();
					}();

					add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
						military::war_goal{ ws.w.current_date, 0.0f, target->id, state, ((nations::nation*)primary_slot)->id, liberate_country_tag, cb_type });
					payload_index += 3;
				}
			}
		}
		ui::xy_pair ef_war_no_ally_this_nation(EFFECT_DISPLAY_PARAMS) {
			nations::nation* target = ((nations::nation*)this_slot);

			auto& new_war = military::create_war(ws, *((nations::nation*)primary_slot), *target, false);

			auto attacker_wg_count = tval[2];
			uint32_t payload_index = 3;
			for(int32_t i = 0; i < attacker_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = tval[payload_index + 1];
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = [&ws, province_id]() {
					if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
						return si->id;
					else
						return nations::state_tag();
				}();

				add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
					military::war_goal{ ws.w.current_date, 0.0f, ((nations::nation*)primary_slot)->id, state, target->id, liberate_country_tag, cb_type });
				payload_index += 3;
			}

			auto defender_wg_count = tval[payload_index];
			++payload_index;
			for(int32_t i = 0; i < defender_wg_count; ++i) {
				auto cb_type = trigger_payload(tval[payload_index + 0]).small.values.cb_type;
				auto province_id = tval[payload_index + 1];
				auto liberate_country_tag = trigger_payload(tval[payload_index + 2]).tag;
				auto state = [&ws, province_id]() {
					if(auto si = ws.w.province_s.province_state_container[provinces::province_tag(province_id)].state_instance; si)
						return si->id;
					else
						return nations::state_tag();
				}();

				add_item(ws.w.military_s.war_goal_arrays, new_war.war_goals,
					military::war_goal{ ws.w.current_date, 0.0f, target->id, state, ((nations::nation*)primary_slot)->id, liberate_country_tag, cb_type });
				payload_index += 3;
			}
		}
		ui::xy_pair ef_war_no_ally_this_state(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((nations::state_instance*)this_slot)->owner; owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_no_ally_this_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)this_slot)->owner; owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_no_ally_this_pop(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = population::get_pop_owner(ws, *((population::pop*)this_slot)); owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_no_ally_from_nation(EFFECT_DISPLAY_PARAMS) {
			ef_war_no_ally_this_nation(tval, ws, primary_slot, from_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_war_no_ally_from_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)from_slot)->owner; owner)
				ef_war_no_ally_this_nation(tval, ws, primary_slot, owner, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_this_nation(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((nations::nation*)primary_slot)->id, ((nations::nation*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_immediate_this_nation(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((nations::nation*)primary_slot)->id, ((nations::nation*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_this_nation(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((provinces::province_state*)primary_slot)->id, ((nations::nation*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_immediate_this_nation(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((provinces::province_state*)primary_slot)->id, ((nations::nation*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_this_state(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((nations::nation*)primary_slot)->id, ((nations::state_instance*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_immediate_this_state(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((nations::nation*)primary_slot)->id, ((nations::state_instance*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_this_state(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((provinces::province_state*)primary_slot)->id, ((nations::state_instance*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_immediate_this_state(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((provinces::province_state*)primary_slot)->id, ((nations::state_instance*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_this_province(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((nations::nation*)primary_slot)->id, ((provinces::province_state*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_immediate_this_province(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((nations::nation*)primary_slot)->id, ((provinces::province_state*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_this_province(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((provinces::province_state*)primary_slot)->id, ((provinces::province_state*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_immediate_this_province(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((provinces::province_state*)primary_slot)->id, ((provinces::province_state*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_this_pop(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((nations::nation*)primary_slot)->id, ((population::pop*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_immediate_this_pop(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((nations::nation*)primary_slot)->id, ((population::pop*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_this_pop(EFFECT_DISPLAY_PARAMS) {
			events::fire_delayed_event(ws, trigger_payload(tval[2]).event, tval[3], ((provinces::province_state*)primary_slot)->id, ((population::pop*)this_slot)->id);
		}
		ui::xy_pair ef_province_event_immediate_this_pop(EFFECT_DISPLAY_PARAMS) {
			events::fire_event(ws, trigger_payload(tval[2]).event, ((provinces::province_state*)primary_slot)->id, ((population::pop*)this_slot)->id);
		}
		ui::xy_pair ef_country_event_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_this_nation(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_immediate_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_immediate_this_nation(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_province_this_state(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_this_state(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_immediate_province_this_state(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_immediate_this_state(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_province_this_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_this_province(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_immediate_province_this_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_immediate_this_province(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_province_this_pop(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_this_pop(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_country_event_immediate_province_this_pop(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_country_event_immediate_this_pop(tval, ws, owner, this_slot, nullptr, nullptr, gen);
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
			ws.w.nation_s.national_variables.get(((nations::nation*)primary_slot)->id, trigger_payload(tval[2]).nat_var) = read_float_from_payload(tval + 3);
		}
		ui::xy_pair ef_change_variable(EFFECT_DISPLAY_PARAMS) {
			ws.w.nation_s.national_variables.get(((nations::nation*)primary_slot)->id, trigger_payload(tval[2]).nat_var) += read_float_from_payload(tval + 3);
		}
		ui::xy_pair ef_ideology(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto factor = read_float_from_payload(tval + 3);

			auto pop_id = ((population::pop*)primary_slot)->id;
			auto total_pop = ws.w.population_s.pop_demographics.get(pop_id, population::total_population_tag);
			auto support = ws.w.population_s.pop_demographics.get_row(pop_id) + to_index(population::to_demo_tag(ws, ideologies::ideology_tag(0)));

			support[to_index(ideology)] = std::max(0, support[to_index(ideology)] + int32_t(total_pop * factor));
			normalize_integer_vector(support, ws.s.ideologies_m.ideologies_count, total_pop);
		}
		ui::xy_pair ef_upper_house(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto amount = trigger_payload(tval[3]).signed_value;
			auto upper_house = ws.w.nation_s.upper_house.get_row(((nations::nation*)primary_slot)->id);

			upper_house[to_index(ideology)] = uint8_t(std::clamp(int32_t(upper_house[to_index(ideology)]) + amount, 0, 255));
			normalize_integer_vector(upper_house, ws.s.ideologies_m.ideologies_count, 100ui8);
		}
		ui::xy_pair ef_scaled_militancy_issue(EFFECT_DISPLAY_PARAMS) {
			auto issue_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto pop_size = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_population_tag);

			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, issue_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_militancy_direct(*((population::pop*)primary_slot), std::clamp(adjustment + population::get_militancy_direct(*((population::pop*)primary_slot)), 0.0f, 1.0f));
			}
		}
		ui::xy_pair ef_scaled_militancy_ideology(EFFECT_DISPLAY_PARAMS) {
			auto ideology_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto pop_size = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_population_tag);

			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, ideology_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_militancy_direct(*((population::pop*)primary_slot), std::clamp(adjustment + population::get_militancy_direct(*((population::pop*)primary_slot)), 0.0f, 1.0f));
			}
		}
		ui::xy_pair ef_scaled_militancy_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto pop_size = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_population_tag);

			if(pop_size != 0) {
				auto unemployed = pop_size - ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_employment_tag);
				float adjustment = read_float_from_payload(tval + 2) * float(unemployed) / float(pop_size);
				population::set_militancy_direct(*((population::pop*)primary_slot), std::clamp(adjustment + population::get_militancy_direct(*((population::pop*)primary_slot)), 0.0f, 1.0f));
			}
		}
		ui::xy_pair ef_scaled_consciousness_issue(EFFECT_DISPLAY_PARAMS) {
			auto issue_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto pop_size = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_population_tag);

			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, issue_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_consciousness_direct(*((population::pop*)primary_slot), std::clamp(adjustment + population::get_consciousness_direct(*((population::pop*)primary_slot)), 0.0f, 1.0f));
			}
		}
		ui::xy_pair ef_scaled_consciousness_ideology(EFFECT_DISPLAY_PARAMS) {
			auto ideology_demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto pop_size = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_population_tag);

			if(pop_size != 0) {
				auto support = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, ideology_demo_tag);
				float adjustment = read_float_from_payload(tval + 3) * float(support) / float(pop_size);
				population::set_consciousness_direct(*((population::pop*)primary_slot), std::clamp(adjustment + population::get_consciousness_direct(*((population::pop*)primary_slot)), 0.0f, 1.0f));
			}
		}
		ui::xy_pair ef_scaled_consciousness_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto pop_size = ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_population_tag);

			if(pop_size != 0) {
				auto unemployed = pop_size - ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, population::total_employment_tag);
				float adjustment = read_float_from_payload(tval + 2) * float(unemployed) / float(pop_size);
				population::set_consciousness_direct(*((population::pop*)primary_slot), std::clamp(adjustment + population::get_consciousness_direct(*((population::pop*)primary_slot)), 0.0f, 1.0f));
			}
		}
		ui::xy_pair ef_scaled_militancy_nation_issue(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_nation_ideology(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_nation_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_nation_issue(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_nation_ideology(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_nation_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_state_issue(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_state_ideology(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_state_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_state_issue(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_state_ideology(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_state_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto factor = read_float_from_payload(tval + 2);

			nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_province_issue(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_province_ideology(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_militancy_province_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto factor = read_float_from_payload(tval + 2);

			provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_militancy_direct(p, std::clamp(adjustment + population::get_militancy_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_province_issue(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_province_ideology(EFFECT_DISPLAY_PARAMS) {
			auto demo_tag = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.ideology);
			auto factor = read_float_from_payload(tval + 3);

			provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, demo_tag, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = ws.w.population_s.pop_demographics.get(p.id, demo_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_scaled_consciousness_province_unemployment(EFFECT_DISPLAY_PARAMS) {
			auto factor = read_float_from_payload(tval + 2);

			provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, factor](population::pop& p) {
				auto pop_size = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);

				if(pop_size != 0) {
					auto support = pop_size - ws.w.population_s.pop_demographics.get(p.id, population::total_employment_tag);
					float adjustment = factor * float(support) / float(pop_size);
					population::set_consciousness_direct(p, std::clamp(adjustment + population::get_consciousness_direct(p), 0.0f, 1.0f));
				}
			});
		}
		ui::xy_pair ef_variable_good_name(EFFECT_DISPLAY_PARAMS) {
			ws.w.nation_s.national_stockpiles.get(((nations::nation*)primary_slot)->id, trigger_payload(tval[2]).small.values.good) += economy::goods_qnty_type(read_float_from_payload(tval + 3));
		}
		ui::xy_pair ef_variable_good_name_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ws.w.nation_s.national_stockpiles.get(owner->id, trigger_payload(tval[2]).small.values.good) += economy::goods_qnty_type(read_float_from_payload(tval + 3));
		}
		ui::xy_pair ef_define_general(EFFECT_DISPLAY_PARAMS) {
			auto& new_leader = military::make_empty_leader(ws, ((nations::nation*)primary_slot)->primary_culture, true);
			new_leader.creation_date = ws.w.current_date;
			new_leader.personality = trigger_payload(tval[3]).small.values.leader_trait;
			new_leader.background = trigger_payload(tval[4]).small.values.leader_trait;
			military::calculate_leader_traits(ws, new_leader);

			add_item(ws.w.military_s.leader_arrays, ((nations::nation*)primary_slot)->generals, new_leader.id);
		}
		ui::xy_pair ef_define_admiral(EFFECT_DISPLAY_PARAMS) {
			auto& new_leader = military::make_empty_leader(ws, ((nations::nation*)primary_slot)->primary_culture, false);
			new_leader.creation_date = ws.w.current_date;
			new_leader.personality = trigger_payload(tval[3]).small.values.leader_trait;
			new_leader.background = trigger_payload(tval[4]).small.values.leader_trait;
			military::calculate_leader_traits(ws, new_leader);

			add_item(ws.w.military_s.leader_arrays, ((nations::nation*)primary_slot)->admirals, new_leader.id);
		}
		ui::xy_pair ef_dominant_issue(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto factor = read_float_from_payload(tval + 3);

			auto pop_id = ((population::pop*)primary_slot)->id;
			auto total_pop = ws.w.population_s.pop_demographics.get(pop_id, population::total_population_tag);
			auto support = ws.w.population_s.pop_demographics.get_row(pop_id) + to_index(population::to_demo_tag(ws, issues::option_tag(0)));

			support[to_index(opt)] = std::max(0, support[to_index(opt)] + int32_t(total_pop * factor));
			normalize_integer_vector(support, ws.s.issues_m.tracked_options_count, total_pop);
		}
		ui::xy_pair ef_dominant_issue_nation(EFFECT_DISPLAY_PARAMS) {
			auto opt = trigger_payload(tval[2]).small.values.option;
			auto factor = read_float_from_payload(tval + 3);
			auto first_issue_index = to_index(population::to_demo_tag(ws, issues::option_tag(0)));

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, opt, factor, first_issue_index](population::pop& p) {
				auto total_pop = ws.w.population_s.pop_demographics.get(p.id, population::total_population_tag);
				auto support = ws.w.population_s.pop_demographics.get_row(p.id) + first_issue_index;

				support[to_index(opt)] = std::max(0, support[to_index(opt)] + int32_t(total_pop * factor));
				normalize_integer_vector(support, ws.s.issues_m.tracked_options_count, total_pop);
			});
		}
		ui::xy_pair ef_add_war_goal(EFFECT_DISPLAY_PARAMS) {
			if(auto w = military::get_war_between(ws, *((nations::nation*)primary_slot), ((nations::nation*)from_slot)->id); w) {
				add_item(ws.w.military_s.war_goal_arrays, w->war_goals,
					military::war_goal{
						ws.w.current_date,
						0.0f,
						((nations::nation*)from_slot)->id,
						nations::state_tag(),
						((nations::nation*)primary_slot)->id,
						cultures::national_tag(),
						trigger_payload(tval[2]).small.values.cb_type });
			}
		}
		ui::xy_pair ef_move_issue_percentage_nation(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			nations::for_each_pop(ws, *((nations::nation*)primary_slot), [&ws, from_issue, to_issue, amount](population::pop& p) {
				auto subtract = int32_t(ws.w.population_s.pop_demographics.get(p.id, from_issue) * amount);
				ws.w.population_s.pop_demographics.get(p.id, from_issue) -= subtract;
				ws.w.population_s.pop_demographics.get(p.id, to_issue) += subtract;
			});
		}
		ui::xy_pair ef_move_issue_percentage_state(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			nations::for_each_pop(ws, *((nations::state_instance*)primary_slot), [&ws, from_issue, to_issue, amount](population::pop& p) {
				auto subtract = int32_t(ws.w.population_s.pop_demographics.get(p.id, from_issue) * amount);
				ws.w.population_s.pop_demographics.get(p.id, from_issue) -= subtract;
				ws.w.population_s.pop_demographics.get(p.id, to_issue) += subtract;
			});
		}
		ui::xy_pair ef_move_issue_percentage_province(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			provinces::for_each_pop(ws, *((provinces::province_state*)primary_slot), [&ws, from_issue, to_issue, amount](population::pop& p) {
				auto subtract = int32_t(ws.w.population_s.pop_demographics.get(p.id, from_issue) * amount);
				ws.w.population_s.pop_demographics.get(p.id, from_issue) -= subtract;
				ws.w.population_s.pop_demographics.get(p.id, to_issue) += subtract;
			});
		}
		ui::xy_pair ef_move_issue_percentage_pop(EFFECT_DISPLAY_PARAMS) {
			auto from_issue = population::to_demo_tag(ws, trigger_payload(tval[2]).small.values.option);
			auto to_issue = population::to_demo_tag(ws, trigger_payload(tval[3]).small.values.option);
			auto amount = read_float_from_payload(tval + 4);

			auto subtract = int32_t(ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, from_issue) * amount);
			ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, from_issue) -= subtract;
			ws.w.population_s.pop_demographics.get(((population::pop*)primary_slot)->id, to_issue) += subtract;
		}
		ui::xy_pair ef_party_loyalty_nation_from_province(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 3);
			ws.w.province_s.party_loyalty.get(((provinces::province*)from_slot)->id, ideology) += amount;
		}
		ui::xy_pair ef_party_loyalty_province_from_nation(EFFECT_DISPLAY_PARAMS) {
			auto ideology = trigger_payload(tval[2]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 3);
			ws.w.province_s.party_loyalty.get(((provinces::province*)primary_slot)->id, ideology) += amount;
		}
		ui::xy_pair ef_party_loyalty_province_id_nation(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);
			auto ideology = trigger_payload(tval[3]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 4);
			ws.w.province_s.party_loyalty.get(prov, ideology) += amount;
		}
		ui::xy_pair ef_party_loyalty_province_id_from_nation(EFFECT_DISPLAY_PARAMS) {
			provinces::province_tag prov(tval[2]);
			auto ideology = trigger_payload(tval[3]).small.values.ideology;
			auto amount = read_float_from_payload(tval + 4);
			ws.w.province_s.party_loyalty.get(prov, ideology) += amount;
		}
		ui::xy_pair ef_build_railway_in_capital_yes_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				if(auto si = ws.w.province_s.province_state_container[capital].state_instance; si) {
					nations::for_each_province(ws, *si, [](provinces::province_state& p) {
						if(p.modifier_values[modifiers::provincial_offsets::min_build_railroad] <= 1.0f)
							++p.railroad_level;
					});
				}
			}
		}
		ui::xy_pair ef_build_railway_in_capital_yes_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				if(auto si = ws.w.province_s.province_state_container[capital].state_instance; si) {
					nations::for_each_province(ws, *si, [](provinces::province_state& p) {
						if(p.modifier_values[modifiers::provincial_offsets::min_build_railroad] <= 1.0f)
							++p.railroad_level;
					});
				}
			}
		}
		ui::xy_pair ef_build_railway_in_capital_no_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				auto& p = ws.w.province_s.province_state_container[capital];
				if(p.modifier_values[modifiers::provincial_offsets::min_build_railroad] <= 1.0f)
					++p.railroad_level;
			}
		}
		ui::xy_pair ef_build_railway_in_capital_no_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				auto& p = ws.w.province_s.province_state_container[capital];
				if(p.modifier_values[modifiers::provincial_offsets::min_build_railroad] <= 1.0f)
					++p.railroad_level;
			}
		}
		ui::xy_pair ef_build_fort_in_capital_yes_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				if(auto si = ws.w.province_s.province_state_container[capital].state_instance; si) {
					nations::for_each_province(ws, *si, [](provinces::province_state& p) {
						++p.fort_level;
					});
				}
			}
		}
		ui::xy_pair ef_build_fort_in_capital_yes_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				if(auto si = ws.w.province_s.province_state_container[capital].state_instance; si) {
					nations::for_each_province(ws, *si, [](provinces::province_state& p) {
						++p.fort_level;
					});
				}
			}
		}
		ui::xy_pair ef_build_fort_in_capital_no_whole_state_yes_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				auto& p = ws.w.province_s.province_state_container[capital];
				++p.fort_level;
			}
		}
		ui::xy_pair ef_build_fort_in_capital_no_whole_state_no_limit(EFFECT_DISPLAY_PARAMS) {
			auto capital = ((nations::nation*)primary_slot)->current_capital;
			if(is_valid_index(capital)) {
				auto& p = ws.w.province_s.province_state_container[capital];
				++p.fort_level;
			}
		}
		ui::xy_pair ef_relation_reb(EFFECT_DISPLAY_PARAMS) {
			if(auto holder = ws.w.culture_s.national_tags_state[rebel_slot->independence_tag].holder; holder)
				nations::adjust_relationship(ws, *((nations::nation*)primary_slot), *holder, trigger_payload(tval[3]).signed_value);
		}
		ui::xy_pair ef_variable_tech_name(EFFECT_DISPLAY_PARAMS) {
			technologies::apply_single_technology(ws, *((nations::nation*)primary_slot), trigger_payload(tval[2]).tech);
		}
		ui::xy_pair ef_set_country_flag_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_set_country_flag(tval, ws, owner, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_add_country_modifier_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_add_country_modifier(tval, ws, owner, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_add_country_modifier_province_no_duration(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_add_country_modifier_no_duration(tval, ws, owner, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_relation_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_relation(tval, ws, owner, nullptr, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_relation_province_this_nation(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_relation_this_nation(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_relation_province_this_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_relation_this_province(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_relation_province_from_nation(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_relation_this_nation(tval, ws, owner, from_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_relation_province_from_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_relation_this_province(tval, ws, owner, from_slot, nullptr, nullptr, gen);
		}
		ui::xy_pair ef_relation_province_reb(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_relation_reb(tval, ws, owner, nullptr, nullptr, rebel_slot, gen);
		}
		ui::xy_pair ef_treasury_province(EFFECT_DISPLAY_PARAMS) {
			if(auto owner = ((provinces::province_state*)primary_slot)->owner; owner)
				ef_treasury(tval, ws, owner, this_slot, nullptr, nullptr, gen);
		}

		ui::xy_pair _make_effect_description(
			world_state& ws,
			ui::tagged_gui_object container,
			ui::xy_pair cursor_in,
			ui::unlimited_line_manager& lm,
			ui::text_format const& fmt,
			uint16_t const* tval,
			void* primary_slot,
			void* this_slot,
			void* from_slot,
			population::rebel_faction* rebel_slot,
			jsf_prng& generator_copy
		) {

		}
	}

	ui::xy_pair make_effect_description(
		world_state& ws,
		ui::tagged_gui_object container,
		ui::xy_pair cursor_in,
		ui::unlimited_line_manager& lm,
		ui::text_format const& fmt,
		uint16_t const* tval,
		void* primary_slot,
		void* this_slot,
		void* from_slot,
		population::rebel_faction* rebel_slot,
		jsf_prng generator_copy
	) {
		return _make_effect_description(ws, container, cursor_in, lm, fmt, tval, primary_slot, this_slot, from_slot, rebel_slot, generator_copy);
	}
}
