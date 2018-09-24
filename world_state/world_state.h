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

	enum class population_display : uint8_t {
		none,
		province,
		state,
		nation,
	};

	enum class population_sort : uint8_t {
		none,
		size,
		type,
		culture,
		religion,
		location,
		militancy,
		consciousness,
		ideoology,
		issues,
		unemployment,
		cash,
		life_needs,
		everyday_needs,
		luxury_needs,
		revolt_risk,
		size_change,
		literacy
	};

	enum class invention_sort : uint8_t {
		none, type, name, chance
	};

	struct player_net_income_history {
		float values[32] = { 0.0 };
	};

	constexpr int32_t max_speed = 5;

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
		float crisis_temperature = 0.0f; // from 0 to 100
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
		int32_t speed = 3;
		bool paused = true;
		bool great_wars_enabled = false;
		bool world_wars_enabled = false;

		//gui state
		provinces::province_tag selected_province;

		struct {
			population_display display_type = population_display::none;
			population_sort sort_type = population_sort::none;

			provinces::province_tag population_for_province;
			nations::state_tag population_for_state;
			nations::country_tag population_for_nation;

			tagged_vector<uint8_t, population::pop_type_tag> filtered_pop_types;
		} selected_population;
		invention_sort techui_invention_sort = invention_sort::none;

		std::unique_ptr<gui_state> gui_objects;

		//player data
		player_net_income_history local_player_income_history;
		nations::nation* local_player_nation = nullptr;

		state();
		~state();
		void init_gui_objects(world_state& s);
		void hide_province_window();
		void show_province_window(provinces::province_tag p);
		void hide_population_window();
		void show_population_window(provinces::province_tag p);
		void show_population_window(nations::state_tag t);
		void show_population_window(nations::country_tag t);
		void update_population_window();
		bool population_window_has_state_expanded(nations::state_tag t);
		void population_window_set_state_expanded(nations::state_tag t, bool expand);
		void resize_topbar();
		void toggle_pause();
		void increase_speed();
		void decrease_speed();
		void hide_government_window();
		void show_government_reforms_tab();
		void show_government_movements_tab();
		void show_government_decisions_tab();
		void show_government_release_nations_tab();
		void hide_tech_window();
		void show_tech_window();
	};
}

class world_state {
public:
	current_state::state w;
	scenario::scenario_manager s;
};

//called after loading a scenario & before loading a specific world state
void ready_world_state(world_state& ws);
