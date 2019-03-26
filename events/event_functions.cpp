#include "common\\common.h"
#include "event_functions.h"
#include "world_state\\world_state.h"
#include "triggers\\trigger_functions.h"
#include "triggers\\effects.h"
#include "modifiers\\modifier_functions.h"
#include <random>
#include "nations\nations_functions.hpp"

namespace events {
	void init_events_state(world_state& ws) {
		ws.w.event_s.country_event_has_fired.resize(ws.s.event_m.country_events.size());
		ws.w.event_s.province_event_has_fired.resize(ws.s.event_m.province_events.size());
	}
	void reset_state(event_state& s) {
		std::fill(s.country_event_has_fired.begin(), s.country_event_has_fired.end(), bitfield_type{});
		std::fill(s.province_event_has_fired.begin(), s.province_event_has_fired.end(), bitfield_type{});
	}

	int32_t wait_for_player_choice(world_state& ws) {
		std::unique_lock<std::mutex> lk(ws.w.local_player_data.player_choice_guard);

		ws.w.local_player_data.player_choice_condition.wait(lk, [&ws] {
			return ws.w.local_player_data.player_chosen_option.load(std::memory_order_acquire) != -1;
		});

		auto const result = ws.w.local_player_data.player_chosen_option.load(std::memory_order_acquire);
		ws.w.local_player_data.player_chosen_option.store(-1, std::memory_order_release);

		return result;
	}
	void make_player_choice(world_state& ws, int32_t option) {
		{
			std::lock_guard<std::mutex> lk(ws.w.local_player_data.player_choice_guard);
			ws.w.local_player_data.player_chosen_option.store(option, std::memory_order_release);
		}

		ws.w.local_player_data.player_choice_condition.notify_one();
	}

	int32_t make_ai_event_choice(world_state const& ws, event_tag e, triggers::const_parameter main_slot, triggers::const_parameter from_slot) {
		float weights[std::extent_v<decltype(event().options)>] = { 0.0f };
		float total_weights = 0.0f;
		for(int32_t i = 0; i < std::extent_v<decltype(weights)>; ++i) {
			if(auto const c = ws.s.event_m.event_container[e].options[i].ai_chance; c) {
				auto i_weight = std::max(modifiers::test_multiplicative_factor(c, ws, main_slot, from_slot), 0.0f);
				total_weights += i_weight;
			}
			weights[i] = total_weights;
		}

		if(total_weights == 0.0f)
			return 0;

		float const val = std::uniform_real_distribution<float>(0.0f, total_weights)(get_local_generator());
		for(int32_t i = 0; i < std::extent_v<decltype(weights)>; ++i) {
			if(val <= weights[i])
				return i;
		}

		return 0;
	}

	void fire_event(world_state& ws, event_tag e, event_slot_content primary_contents, event_slot_content from) {

	}
	void fire_delayed_event(world_state& ws, event_tag e, int32_t delay_days, event_slot_content primary_contents, event_slot_content from) {

	}
	void fire_event_from_list(world_state& ws, std::vector<std::pair<event_tag, uint16_t>> const& event_list, event_slot_content primary_contents, event_slot_content from) {

	}


	void execute_decision_set(std::vector<std::pair<nations::country_tag, events::decision_tag>>const& decision_set, world_state& ws) {
		for(auto& p : decision_set) {
			auto& gen = get_local_generator();
			triggers::execute_effect(ws.s.trigger_m.effect_data.data() + to_index(ws.s.event_m.decision_container[p.second].effect), ws, p.first, p.first, triggers::const_parameter(), gen);
			gen.advance_n<8>();
		}
	}

	void daily_update(world_state& ws) {
		int32_t const date_offset = to_index(ws.w.current_date) & 31;

		concurrency::concurrent_queue<int32_t> fired_once_list;
		concurrency::concurrent_queue<std::pair<provinces::province_tag, events::event_tag>> player_province_events;

		concurrency::parallel_for(date_offset, int32_t(ws.s.event_m.province_events.size()), 32, [&ws, &fired_once_list, &player_province_events](int32_t i) {
			auto const e = ws.s.event_m.province_events[i];
			auto const allow = ws.s.event_m.event_container[e].trigger;
			auto const allow_data = ws.s.trigger_m.trigger_data.data() + to_index(allow);
			auto const only_once_type = (ws.s.event_m.event_container[e].flags & event::fire_only_once) != 0;

			if(only_once_type && ws.w.event_s.province_event_has_fired[i])
				return;

			for(int32_t j = 0; j < ws.s.province_m.first_sea_province; j += ve::vector_size) {
				auto any_valid = triggers::test_contiguous_trigger(allow_data, ws, ve::contiguous_tags<union_tag>(j), ve::contiguous_tags<union_tag>(j), ve::contiguous_tags<union_tag>(0));
				if(ve::compress_mask(any_valid) != 0) {
					auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen;
					auto const value = modifiers::test_contiguous_multiplicative_factor(chance_tag, ws, ve::contiguous_tags<union_tag>(j), ve::contiguous_tags<union_tag>(0));

					auto chance = 2.0f / ve::max(value, 2.0f);
					auto neg_chance = 1.0f - chance;
					auto neg_chance_2 = neg_chance * neg_chance;
					auto neg_chance_4 = neg_chance_2 * neg_chance_2;
					auto neg_chance_8 = neg_chance_4 * neg_chance_4;
					auto neg_chance_16 = neg_chance_8 * neg_chance_8;

					auto full_chance = 1.0f - neg_chance_16;
					auto& local_gen = get_local_generator();

					bool any_fired = false;
					ve::apply(any_valid, full_chance, ve::contiguous_tags<union_tag>(j), [&local_gen, &ws, &any_fired, only_once_type, e, &player_province_events](bool inner_valid, float inner_chance, union_tag p) {
						provinces::province_tag const prov = p;
						auto const prov_owner = ws.w.province_s.province_state_container.get<province_state::owner>(prov);

						if(inner_valid && is_valid_index(prov_owner) && (!only_once_type || !any_fired)) {
							std::uniform_real_distribution<float> dist(0.0f, 1.0f);
							auto const v = dist(local_gen);
							if(v <= inner_chance) {
								//fire event
								if(prov_owner == ws.w.local_player_nation) {
									player_province_events.push(std::pair<provinces::province_tag, events::event_tag>(prov, e));
								} else {
									auto const opt = make_ai_event_choice(ws, e, prov, triggers::const_parameter());
									auto& gen = get_local_generator();
									ws.w.pending_commands.add<commands::execute_event>(
										gen,
										prov,
										triggers::parameter(),
										e,
										int8_t(opt));
									gen.advance_n<8>();
								}
								any_fired = true;
							}
						}
					});

					if(only_once_type && any_fired) {
						fired_once_list.push(i);
						return;
					}
				}
			}
		});

	
		for(int32_t v =  0; fired_once_list.try_pop(v); ) {
			ws.w.event_s.province_event_has_fired.set(v, true);
		}

		auto& gen = get_local_generator();
		for(std::pair<provinces::province_tag, events::event_tag> v; player_province_events.try_pop(v); ) {
			auto const fixed_choice = ws.w.local_player_data.saved_event_choices[v.second];
			if(fixed_choice == 0) {
				ws.w.province_event_w.show_province_event_window(ws.w.gui_m, pending_event{ gen, v.first, target_variant(), v.second });
				gen.advance_n<8>();

				wait_for_player_choice(ws);
			} else {
				ws.w.pending_commands.add<commands::execute_event>(
					gen,
					v.first,
					triggers::parameter(),
					v.second,
					int8_t(fixed_choice - 1));
				gen.advance_n<8>();
			}
		}


		concurrency::concurrent_queue<events::event_tag> player_nation_events;

		concurrency::parallel_for(date_offset, int32_t(ws.s.event_m.country_events.size()), 32, [&ws, &fired_once_list, &player_nation_events](int32_t i) {
			auto const e = ws.s.event_m.country_events[i];
			auto const allow = ws.s.event_m.event_container[e].trigger;
			auto const allow_data = ws.s.trigger_m.trigger_data.data() + to_index(allow);
			auto const only_once_type = (ws.s.event_m.event_container[e].flags & event::fire_only_once) != 0;

			if(only_once_type && ws.w.event_s.country_event_has_fired[i])
				return;

			auto const nation_count = ws.w.nation_s.nations.size();
			for(int32_t j = 0; j < nation_count; j += ve::vector_size) {
				auto any_valid = nations::nation_exists(ws, ve::contiguous_tags<nations::country_tag>(j)) & triggers::test_contiguous_trigger(allow_data, ws, ve::contiguous_tags<union_tag>(j), ve::contiguous_tags<union_tag>(j), ve::contiguous_tags<union_tag>(0));
				
				if(ve::compress_mask(any_valid) != 0) {
					auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen;
					auto const value = modifiers::test_contiguous_multiplicative_factor(chance_tag, ws, ve::contiguous_tags<union_tag>(j), ve::contiguous_tags<union_tag>(0));

					auto chance = 1.0f / ve::max(value, 1.0f);
					auto neg_chance = 1.0f - chance;
					auto neg_chance_2 = neg_chance * neg_chance;
					auto neg_chance_4 = neg_chance_2 * neg_chance_2;
					auto neg_chance_8 = neg_chance_4 * neg_chance_4;
					auto neg_chance_16 = neg_chance_8 * neg_chance_8;

					auto full_chance = 1.0f - neg_chance_16;
					auto& local_gen = get_local_generator();

					bool any_fired = false;
					ve::apply(any_valid, full_chance, ve::contiguous_tags<union_tag>(j), [&local_gen, &ws, &any_fired, only_once_type, e, &player_nation_events](bool inner_valid, float inner_chance, union_tag p) {
						nations::country_tag const n = p;

						if(inner_valid && (!only_once_type || !any_fired)) {
							std::uniform_real_distribution<float> dist(0.0f, 1.0f);
							auto const v = dist(local_gen);
							if(v <= inner_chance) {
								//fire event
								if(n == ws.w.local_player_nation) {
									player_nation_events.push( e);
								} else {
									auto const opt = make_ai_event_choice(ws, e, n, triggers::const_parameter());
									auto& gen = get_local_generator();
									ws.w.pending_commands.add<commands::execute_event>(
										gen,
										n,
										triggers::parameter(),
										e,
										int8_t(opt));
									gen.advance_n<8>();
								}
								any_fired = true;
							}
						}
					});

					if(only_once_type && any_fired) {
						fired_once_list.push(i);
						return;
					}
				}
			}
		});


		for(int32_t v = 0; fired_once_list.try_pop(v); ) {
			ws.w.event_s.country_event_has_fired.set(v, true);
		}

		for(events::event_tag v; player_nation_events.try_pop(v); ) {
			auto const fixed_choice = ws.w.local_player_data.saved_event_choices[v];
			if(fixed_choice == 0) {
				if((ws.s.event_m.event_container[v].flags & event::is_major) != 0)
					ws.w.major_event_w.show_major_event_window(ws.w.gui_m, pending_event { gen, ws.w.local_player_nation, target_variant(), v });
				else
					ws.w.nation_event_w.show_nation_event_window(ws.w.gui_m, pending_event{ gen, ws.w.local_player_nation, target_variant(), v });
				gen.advance_n<8>();

				wait_for_player_choice(ws);
			} else {
				ws.w.pending_commands.add<commands::execute_event>(
					gen,
					ws.w.local_player_nation,
					triggers::parameter(),
					v,
					int8_t(fixed_choice - 1));
				gen.advance_n<8>();
			}
		}
	}
}
