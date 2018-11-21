#include "common\\common.h"
#include "world_state.h"
#include "provinces\\province_functions.h"
#include "nations\\nations_functions.h"
#include "cultures\\cultures_functions.h"
#include "population\\population_function.h"
#include "variables\\variables_functions.h"
#include "military\\military_functions.h"
#include "economy\\economy_functions.h"
#include "ideologies\\ideologies_functions.h"
#include "technologies\\technologies_functions.h"
#include "modifiers\\modifier_functions.h"
#include "issues\\issues_functions.hpp"
#include <chrono>

#include <Windows.h>
#undef min
#undef max

void world_state_non_ai_update(world_state & ws) {
	economy::economy_update_tick(ws);

	provinces::update_province_demographics(ws);
	nations::update_state_nation_demographics(ws);

	ws.w.nation_s.nations.parallel_for_each([&ws](nations::nation& n) {
		nations::update_movement_support(ws, n);

		n.military_score = int16_t(nations::calculate_military_score(ws, n));
		n.industrial_score = int16_t(nations::calculate_industrial_score(ws, n));

		n.national_administrative_efficiency = nations::calculate_national_administrative_efficiency(ws, n);

		auto admin_req = issues::administrative_requirement(ws, n.id);
		auto member_states = get_range(ws.w.nation_s.state_arrays, n.member_states);
		for(auto s = member_states.first; s != member_states.second; ++s)
			ws.w.nation_s.states.set<state::administrative_efficiency>(s->state, nations::calculate_state_administrative_efficiency(ws, s->state, admin_req));
	});

	nations::update_nation_ranks(ws);

	ws.w.province_s.province_state_container.parallel_for_each([&ws](provinces::province_tag ps) {
		modifiers::reset_provincial_modifier(ws, ps);
	});
	ws.w.nation_s.nations.parallel_for_each([&ws](nations::nation& n) {
		modifiers::reset_national_modifier(ws, n);
	});
}

void world_state_update_loop(world_state & ws) {
	auto last_tick = std::chrono::steady_clock::now();

	while(ws.w.end_game.load(std::memory_order_acquire) == false) {

		bool perform_update = [&ws, last_tick](){
			if(ws.w.force_paused.load(std::memory_order_acquire) == true)
				return false;
			if(ws.w.single_step_pending.load(std::memory_order_acquire) == true)
				return true;
			if(ws.w.paused.load(std::memory_order_acquire) == true)
				return false;

			const auto speed = ws.w.speed.load(std::memory_order_acquire);
			if(speed >= 5)
				return true;
			if(speed == 4) {
				const auto new_time = std::chrono::steady_clock::now();
				const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(new_time - last_tick).count();
				return elapsed >= 100;
			} else if(speed == 3) {
				const auto new_time = std::chrono::steady_clock::now();
				const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(new_time - last_tick).count();
				return elapsed >= 300;
			} else if(speed == 2) {
				const auto new_time = std::chrono::steady_clock::now();
				const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(new_time - last_tick).count();
				return elapsed >= 800;
			} else if(speed == 1) {
				const auto new_time = std::chrono::steady_clock::now();
				const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(new_time - last_tick).count();
				return elapsed >= 1500;
			}
			return false;
		}();

		if(perform_update) {
			last_tick = std::chrono::steady_clock::now();

			world_state_non_ai_update(ws);

			ws.w.pending_commands.execute(ws);
			
			ws.w.single_step_pending.store(false, std::memory_order_release);

			ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);
			ws.w.gui_m.flag_update();
			ws.w.map_view.changed.store(true, std::memory_order_release);
		} else {
			if(ws.w.pending_commands.execute(ws)) {
				ws.w.gui_m.flag_update();
				ws.w.map_view.changed.store(true, std::memory_order_release);
			}
			Sleep(1);
		}
	}
}

void ready_world_state(world_state& ws) {
	variables::init_variables_state(ws);
	provinces::init_province_state(ws);
	cultures::init_cultures_state(ws);
	population::init_population_state(ws);
	nations::init_nations_state(ws);
	military::init_military_state(ws);
	economy::init_economic_scenario(ws);
	ideologies::init_ideology_state(ws);
	technologies::init_technology_state(ws);

	ws.w.local_player_data.imports_by_country.resize(ws.s.economy_m.goods_count);
}

namespace current_state {

	state::state() {};
	state::~state() {}

	void state::init_gui_objects(world_state& ws) {
		topbar_w.init_topbar(ws);
		government_w.init_government_window(ws);
		province_w.init_province_window(ws);
		diplomacy_w.init_diplomacy_window(ws);
		technologies_w.init_technology_winodw(ws);
		population_w.init_population_window(ws);
		production_w.init(ws);
		trade_w.init(ws);
		budget_w.init(ws);
	}

	void state::toggle_pause() {
		paused = !paused;
		topbar_w.update_topbar(gui_m);
	}
	void state::increase_speed() {
		speed = std::min(speed + 1, current_state::max_speed);
		topbar_w.update_topbar(gui_m);
	}
	void state::decrease_speed() {
		speed = std::max(speed - 1, 1);
		topbar_w.update_topbar(gui_m);
	}
}
