#pragma once
#include "common\\common.h"
#include "provinces\\provinces.h"
#include "cultures\\cultures.h"
#include "nations\\nations.h"
#include "scenario\\scenario.h"
#include "economy\\economy.h"
#include "population\\population.h"
#include "variables\\variables.h"
#include "military\\military.h"
#include "gui\\gui.h"
#include "ideologies\\ideologies.h"
#include "nations\\nations_gui.h"
#include "provinces\\province_gui.h"
#include "technologies\\technologies_gui.h"
#include "governments\\governments_gui.h"
#include "population\\population_gui.h"
#include "topbar.h"
#include "economy\\economy_gui.h"
#include "economy\\trade_window.h"
#include "governments\\budget_window.h"
#include "commands\\commands.hpp"
#include "messages.h"
#include "military\\military_gui.h"
#include "events\\events.h"
#include "events\\events_gui.h"
#include "bottombar.h"
#include "graphics\\world_map.h"

#undef small

class world_state;

namespace current_state {
	enum class crisis_type : uint8_t {
		none,
		liberation,
		claim,
		colonial,
		influence
	};

	struct player_net_income_history {
		float values[32] = { 0.0 };
	};

	constexpr int32_t max_speed = 5;

	enum class map_mode {
		political,
		prices,
		distance,
		purchasing,
		production
	};

	struct map_state {
		map_mode mode = map_mode::political;
		economy::goods_tag selected_good;
		nations::country_tag selected_country;
		nations::state_tag selected_state;
		provinces::province_tag selected_province;
		std::atomic<bool> changed = true;
	};

	struct crisis_state {
		float temperature = 0.0f; // from 0 to 100
		crisis_type type = crisis_type::none;
		nations::country_tag primary_attacker;
		nations::country_tag primary_defender;
		nations::country_tag target;
		nations::country_tag on_behalf_of;
		set_tag<nations::country_tag> attackers;
		set_tag<nations::country_tag> defenders;
		set_tag<nations::country_tag> interested;
		array_tag<military::war_goal, int32_t, false> goals;
		nations::state_tag state;
	};

	class state {
	public:
		graphics::map_display map;

		provinces::provinces_state province_s;
		cultures::cultures_state culture_s;
		military::military_state military_s;
		nations::nations_state nation_s;
		economy::economic_state economy_s;
		population::population_state population_s;
		variables::variables_state variable_s;
		ideologies::ideologies_state ideology_s;
		technologies::technologies_state technology_s;
		events::event_state event_s;
		ui::gui_manager gui_m;

		//crisis state
		crisis_state current_crisis;
		//other global state
		date_tag current_date;
		bool great_wars_enabled = false;
		bool world_wars_enabled = false;

		//game speed state
		std::atomic<int32_t> speed = 3;
		std::atomic<bool> paused = true;
		std::atomic<bool> force_paused = false;
		std::atomic<bool> single_step_pending = false;
		std::atomic<bool> end_game = false;

		commands::full_command_set pending_commands;

		//gui state
		map_state map_view;

		nations::diplomacy_window diplomacy_w;
		provinces::province_window province_w;
		technologies::technology_window technologies_w;
		governments::government_window government_w;
		governments::budget_window budget_w;
		population::population_window population_w;
		economy::production_window production_w;
		economy::trade_window trade_w;
		messages::message_window message_w;
		military::fabricate_cb_window fabricate_cb_w;
		events::province_event_window province_event_w;
		events::nation_event_window nation_event_w;
		events::major_event_window major_event_w;

		topbar topbar_w;
		bottombar bottombar_w;

		//player data
		struct {
			player_net_income_history income_history;
			economy::money_qnty_type collected_poor_tax = 0;
			economy::money_qnty_type collected_middle_tax = 0;
			economy::money_qnty_type collected_rich_tax = 0;
			tagged_vector<array_tag<economy::money_qnty_type, nations::country_tag, true>, economy::goods_tag> imports_by_country;
			tagged_vector<bitfield_type, events::decision_tag> suppressed_decisions;
			tagged_vector<int8_t, events::event_tag> saved_event_choices;
			tagged_vector<tagged_vector<bitfield_type, nations::country_tag>, military::cb_type_tag> triggered_cb_state;
			std::atomic<int32_t> player_chosen_option = -1;
			std::mutex player_choice_guard;
			std::condition_variable player_choice_condition;
		} local_player_data;
		nations::country_tag local_player_nation;

		state();
		~state();
		void init_gui_objects(world_state& s);

		void toggle_pause();
		void increase_speed();
		void decrease_speed();
		
	};
}

class world_state {
public:
	current_state::state w;
	scenario::scenario_manager s;
};

void world_state_non_ai_update(world_state & ws);
void world_state_update_loop(world_state& ws);

//called after loading a scenario & before loading a specific world state
void ready_world_state(world_state& ws);
