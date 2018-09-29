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

	enum class country_sort : uint8_t {
		none,
		name,
		priority,
		gp_one,
		gp_two,
		gp_three,
		gp_four,
		gp_five,
		gp_six,
		gp_seven,
		gp_eight,
		sphere_leader,
		prestige_rank,
		economic_rank,
		military_rank,
		overall_rank,
		opinion,
		relation
	};

	enum class invention_sort : uint8_t {
		none, type, name, chance
	};

	struct player_net_income_history {
		float values[32] = { 0.0 };
	};

	constexpr int32_t max_speed = 5;

	struct crisis_state {
		float temperature = 0.0f; // from 0 to 100
		crisis_type type = crisis_type::none;
		nations::nation* primary_attacker = nullptr;
		nations::nation* primary_defender = nullptr;
		nations::nation* target = nullptr;
		nations::nation* on_behalf_of = nullptr;
		set_tag<nations::country_tag> attackers;
		set_tag<nations::country_tag> defenders;
		set_tag<nations::country_tag> interested;
		array_tag<military::war_goal> goals;
		nations::state_tag state;
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
		technologies::technologies_state technology_s;
		ui::gui_manager gui_m;

		//crisis state
		crisis_state current_crisis;

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
		technologies::tech_tag selected_technology;
		technologies::tech_category_tag selected_tech_category = technologies::tech_category_tag(0);
		struct {
			nations::country_tag selected_nation;
			bool goto_selected_pending = false;

			bool filter_north_america = true;
			bool filter_south_america = true;
			bool filter_europe = true;
			bool filter_africa = true;
			bool filter_asia = true;
			bool filter_oceania = true;
			bool filter_neighbors = false;
			bool filter_sphere = false;
			bool filter_enemies = false;
			bool filter_allies = false;

			country_sort sort_type = country_sort::none;
		} selected_diplomacy;
		
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
		void hide_diplomacy_window();
		void show_diplomacy_window_self();
		void update_diplomacy_window();
		void show_diplomacy_window(nations::country_tag t);
		void show_diplomacy_window_gp();
		void show_diplomacy_window_wars();
		void show_diplomacy_window_crisis();
		void show_diplomacy_window_cbs();
	};
}

class world_state {
public:
	current_state::state w;
	scenario::scenario_manager s;
};

//called after loading a scenario & before loading a specific world state
void ready_world_state(world_state& ws);
