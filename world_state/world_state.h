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

class world_state;

namespace current_state {
	class gui_state;

	enum class crisis_type : uint8_t {
		none,
		liberation,
		claim,
		colonial,
		influence
	};

	class state {
	public:
		provinces::provinces_state province_s;
		cultures::cultures_state culture_s;
		military::military_state military_s;
		nations::nations_state nation_s;
		economy::economic_state economy_s;
		population::population_state population_s;
		variables::variables_state variable_s;
		ideologies::ideologies_state ideology_s;
		ui::gui_manager gui_m;

		//crisis state
		float crisis_temperature = 0.0f;
		crisis_type current_crisis_type = crisis_type::none;
		nations::nation* crisis_primary_attacker = nullptr;
		nations::nation* crisis_primary_defender = nullptr;
		nations::nation* crisis_target = nullptr;
		set_tag<nations::country_tag> crisis_attackers;
		set_tag<nations::country_tag> crisis_defenders;
		set_tag<nations::country_tag> crisis_interested;
		array_tag<military::war_goal> crisis_goals;
		nations::state_tag crisis_state;

		//other global state
		date_tag current_date;
		bool great_wars_enabled = false;
		bool world_wars_enabled = false;

		//gui state
		provinces::province_tag selected_province;
		std::unique_ptr<gui_state> gui_objects;

		//player data
		nations::nation* local_player_nation = nullptr;

		state();
		~state();
		void init_gui_objects(world_state& s);
		void hide_province_window();
		void show_province_window(provinces::province_tag p);
	};
}

class world_state {
public:
	current_state::state w;
	scenario::scenario_manager s;
};

//called after loading a scenario & before loading a specific world state
void ready_world_state(world_state& ws);
