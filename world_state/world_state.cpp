#include "common\\common.h"
#include "world_state.h"
#include "provinces\\province_functions.h"
#include "nations\\nations_functions.h"
#include "cultures\\cultures_functions.h"
#include "population\\population_function.h"
#include "variables\\variables_functions.h"
#include "military\\military_functions.h"
#include "economy\\economy_functions.h"
#include "provinces\\province_gui.hpp"
#include "ideologies\\ideologies_functions.h"
#include "population\\population_gui.hpp"
#include "topbar.hpp"
#include "governments\\governments_gui.hpp"
#include "technologies\\technologies_gui.hpp"
#include "nations\\nations_gui.hpp"

void ready_world_state(world_state& ws) {
	ws.w.selected_population.filtered_pop_types.resize(ws.s.population_m.count_poptypes);
	for(uint32_t i = 0; i < ws.s.population_m.count_poptypes; ++i)
		ws.w.selected_population.filtered_pop_types[population::pop_type_tag(static_cast<population::pop_type_tag::value_base_t>(i))] = 1ui8;

	variables::init_variables_state(ws);
	provinces::init_ready_provinces(ws);
	cultures::init_cultures_state(ws);
	population::init_population_state(ws);
	nations::init_nations_state(ws);
	military::init_military_state(ws);
	economy::init_economic_scenario(ws);
	ideologies::init_ideology_state(ws);
	technologies::init_technology_state(ws);
}

namespace current_state {
	class gui_state {
	public:
		provinces::province_window_t province_window;
		population::population_window_t population_window;
		governments::government_window_t government_window;
		technologies::tech_window_t tech_window;
		nations::diplomacy_window_t diplomacy_window;
		topbar_t topbar;
	};

	state::state() : gui_objects(std::make_unique<gui_state>()) {};
	state::~state() {}

	void state::hide_province_window() {
		auto gobj = gui_objects->province_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void state::show_province_window(provinces::province_tag p) {
		selected_province = p;
		ui::make_visible_and_update(gui_m, *(gui_objects->province_window.associated_object));
	}
	void state::init_gui_objects(world_state& ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["province_view"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->province_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_pop"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->population_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["topbar"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->topbar);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_politics"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->government_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_technology"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->tech_window);
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_diplomacy"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, gui_objects->diplomacy_window);
	}
	void state::hide_tech_window() {
		auto gobj = gui_objects->tech_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void state::show_tech_window() {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->tech_window.associated_object), gui_objects->tech_window.window_object });
		ui::make_visible_and_update(gui_m, *(gui_objects->tech_window.associated_object));
	}

	void state::hide_diplomacy_window() {
		auto gobj = gui_objects->diplomacy_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void state::show_diplomacy_window_self() {
		selected_diplomacy.selected_war = military::war_tag();
		if(auto player = local_player_nation; player)
			selected_diplomacy.selected_nation = player->id;
		else
			selected_diplomacy.selected_nation = nations::country_tag();

		gui_objects->diplomacy_window.template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 0);
		ui::hide(*(gui_objects->diplomacy_window.template get<CT_STRING("war_listbox")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->diplomacy_window.associated_object), gui_objects->diplomacy_window.window_object });
		ui::make_visible_and_update(gui_m, *(gui_objects->diplomacy_window.associated_object));
	}
	void state::show_diplomacy_window(nations::country_tag t) {
		selected_diplomacy.selected_war = military::war_tag();
		selected_diplomacy.selected_nation = t;

		gui_objects->diplomacy_window.template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 0);
		ui::hide(*(gui_objects->diplomacy_window.template get<CT_STRING("war_listbox")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->diplomacy_window.associated_object), gui_objects->diplomacy_window.window_object });
		ui::make_visible_and_update(gui_m, *(gui_objects->diplomacy_window.associated_object));
	}
	void state::show_diplomacy_window(military::war_tag t) {
		selected_diplomacy.selected_war = t;
		selected_diplomacy.selected_nation = nations::country_tag();

		gui_objects->diplomacy_window.template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 1);
		ui::make_visible_and_update(gui_m, *(gui_objects->diplomacy_window.template get<CT_STRING("war_listbox")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->diplomacy_window.associated_object), gui_objects->diplomacy_window.window_object });
		ui::make_visible_and_update(gui_m, *(gui_objects->diplomacy_window.associated_object));
	}
	void state::show_diplomacy_window_crisis() {
		selected_diplomacy.selected_war = military::war_tag();
		selected_diplomacy.selected_nation = nations::country_tag();

		gui_objects->diplomacy_window.template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 3);
		ui::hide(*(gui_objects->diplomacy_window.template get<CT_STRING("war_listbox")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->diplomacy_window.associated_object), gui_objects->diplomacy_window.window_object });
		ui::make_visible_and_update(gui_m, *(gui_objects->diplomacy_window.associated_object));
	}
	void state::show_diplomacy_window_cbs() {
		selected_diplomacy.selected_war = military::war_tag();
		selected_diplomacy.selected_nation = nations::country_tag();

		gui_objects->diplomacy_window.template get<CT_STRING("diplomacy_tab_button_group")>().set_selected(gui_m, 2);
		ui::hide(*(gui_objects->diplomacy_window.template get<CT_STRING("war_listbox")>().associated_object));
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->diplomacy_window.associated_object), gui_objects->diplomacy_window.window_object });
		ui::make_visible_and_update(gui_m, *(gui_objects->diplomacy_window.associated_object));
	}

	void state::hide_population_window() {
		auto gobj = gui_objects->population_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void state::hide_government_window() {
		auto gobj = gui_objects->government_window.associated_object;
		if(gobj)
			ui::hide(*gobj);
	}
	void state::show_government_reforms_tab() {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->government_window.associated_object), gui_objects->government_window.window_object });
		gui_objects->government_window.template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 0ui32);

		if(auto player = local_player_nation; player) {
			if((player->flags & nations::nation::is_civilized) != 0) {
				ui::make_visible_and_update(gui_m, *(gui_objects->government_window.template get<CT_STRING("reforms_window")>().associated_object));
				ui::hide(*(gui_objects->government_window.template get<CT_STRING("unciv_reforms_window")>().associated_object));
			} else {
				ui::hide(*(gui_objects->government_window.template get<CT_STRING("reforms_window")>().associated_object));
				ui::make_visible_and_update(gui_m, *(gui_objects->government_window.template get<CT_STRING("unciv_reforms_window")>().associated_object));
			}
		}
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("movements_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("decision_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("release_nation")>().associated_object));

		ui::make_visible_and_update(gui_m, *(gui_objects->government_window.associated_object));
	}
	void state::show_government_movements_tab() {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->government_window.associated_object), gui_objects->government_window.window_object });
		gui_objects->government_window.template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 1ui32);
		
		auto& pane = gui_objects->government_window.template get<CT_STRING("movements_window")>();
		pane.template get<CT_STRING("movements_listbox")>().new_list(nullptr, nullptr);
		pane.template get<CT_STRING("rebel_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(pane.associated_object));

		ui::hide(*(gui_objects->government_window.template get<CT_STRING("decision_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("reforms_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("unciv_reforms_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("release_nation")>().associated_object));

		ui::make_visible_and_update(gui_m, *(gui_objects->government_window.associated_object));
	}
	void state::show_government_decisions_tab() {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->government_window.associated_object), gui_objects->government_window.window_object });
		gui_objects->government_window.template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 2ui32);

		auto& pane = gui_objects->government_window.template get<CT_STRING("decision_window")>();
		pane.template get<CT_STRING("decision_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(pane.associated_object));

		ui::hide(*(gui_objects->government_window.template get<CT_STRING("movements_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("reforms_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("unciv_reforms_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("release_nation")>().associated_object));

		ui::make_visible_and_update(gui_m, *(gui_objects->government_window.associated_object));
	}
	void state::show_government_release_nations_tab() {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->government_window.associated_object), gui_objects->government_window.window_object });
		gui_objects->government_window.template get< CT_STRING("governments_tab_button_group")>().set_selected(gui_m, 3ui32);

		auto& pane = gui_objects->government_window.template get<CT_STRING("release_nation")>();
		pane.template get<CT_STRING("nations")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(pane.associated_object));

		ui::hide(*(gui_objects->government_window.template get<CT_STRING("movements_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("decision_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("reforms_window")>().associated_object));
		ui::hide(*(gui_objects->government_window.template get<CT_STRING("unciv_reforms_window")>().associated_object));

		ui::make_visible_and_update(gui_m, *(gui_objects->government_window.associated_object));
	}
	void state::show_population_window(provinces::province_tag p) {
		if(local_player_nation)
			gui_objects->population_window.template get<CT_STRING("pop_province_list")>().force_open(local_player_nation->id);
		if(is_valid_index(p)) {
			if(auto si = province_s.province_state_container[p].state_instance; si)
				gui_objects->population_window.template get<CT_STRING("pop_province_list")>().force_open(si->id);
		}
		selected_population.population_for_province = p;
		selected_population.display_type = population_display::province;
		selected_population.sort_type = population_sort::none;

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->population_window.associated_object), gui_objects->population_window.window_object });
		update_population_window();
	}
	void state::show_population_window(nations::state_tag t) {
		if(local_player_nation)
			gui_objects->population_window.template get<CT_STRING("pop_province_list")>().force_open(local_player_nation->id);
		selected_population.population_for_state = t;
		selected_population.display_type = population_display::state;
		selected_population.sort_type = population_sort::none;

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->population_window.associated_object), gui_objects->population_window.window_object });
		update_population_window();
	}
	void state::show_population_window(nations::country_tag t) {
		if(local_player_nation)
			gui_objects->population_window.template get<CT_STRING("pop_province_list")>().force_open(local_player_nation->id);
		selected_population.population_for_nation = t;
		selected_population.display_type = population_display::nation;
		selected_population.sort_type = population_sort::none;

		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(gui_objects->population_window.associated_object), gui_objects->population_window.window_object });
		update_population_window();
	}
	void state::update_population_window() {
		gui_objects->population_window.template get<CT_STRING("pop_list")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(gui_objects->population_window.associated_object));
	}

	void state::resize_topbar() {
		if(gui_objects->topbar.associated_object) {
			gui_objects->topbar.associated_object->size.x = int16_t(gui_m.width());
		}
	}

	void state::toggle_pause() {
		paused = !paused;
		ui::make_visible_and_update(gui_m, *(gui_objects->topbar.associated_object));
	}

	void state::increase_speed() {
		speed = std::min(speed + 1, current_state::max_speed);
		ui::make_visible_and_update(gui_m, *(gui_objects->topbar.associated_object));
	}

	void state::decrease_speed() {
		speed = std::max(speed - 1, 1);
		ui::make_visible_and_update(gui_m, *(gui_objects->topbar.associated_object));
	}

	bool state::population_window_has_state_expanded(nations::state_tag t) {
		auto& poptree = gui_objects->population_window.template get<CT_STRING("pop_province_list")>();
		return poptree.is_open(t);
	}
	void state::population_window_set_state_expanded(nations::state_tag t, bool expand) {
		auto& poptree = gui_objects->population_window.template get<CT_STRING("pop_province_list")>();
		poptree.set_open(t, expand, gui_m);
	}
}
