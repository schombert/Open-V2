#include "common\\common.h"
#include "economy_gui.hpp"

namespace economy {
	void close_button::button_function(ui::simple_button<close_button>&, world_state& ws) {
		ws.w.production_w.hide(ws.w.gui_m);
	}
	production_window::production_window() : win(std::make_unique<production_window_t>()) {}
	production_window::~production_window() {}
	void production_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void production_window::init(world_state & ws) {
		ui::create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_production"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void production_window::update(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("investment_browser")>().template get<CT_STRING("country_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void production_window::show(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void production_window::show_factories(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 0);

		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		show(gui_m);
	}
	void production_window::show_projects(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 3);
		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		show(gui_m);
	}
	void production_window::show_production(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 2);
		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		show(gui_m);
	}
	void production_window::show_foreign_investment(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 1);
		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		foreign_investment_nation = nations::country_tag();
		show(gui_m);
	}
	void production_window::show_particular_foreign_investment(ui::gui_manager & gui_m, nations::country_tag target) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 1);
		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		foreign_investment_nation = target;
		show(gui_m);
	}
	void production_tab_button_group::on_select(world_state & ws, uint32_t i) {
		if(i == 0)
			ws.w.production_w.show_factories(ws.w.gui_m);
		else if(i == 1)
			ws.w.production_w.show_foreign_investment(ws.w.gui_m);
		else if(i == 2)
			ws.w.production_w.show_production(ws.w.gui_m);
		else if(i == 3)
			ws.w.production_w.show_projects(ws.w.gui_m);
	}
	void empty_text_box::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	void filter_all_button::update(ui::simple_button<filter_all_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::all) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_all_button::button_function(ui::simple_button<filter_all_button>&, world_state & ws) {
		ws.w.production_w.filter = country_sub_filter::all;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_north_america_button::update(ui::simple_button<filter_north_america_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::continent_north_america) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_north_america_button::button_function(ui::simple_button<filter_north_america_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::continent_north_america) ?
			country_sub_filter::all :
			country_sub_filter::continent_north_america;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_south_america_button::update(ui::simple_button<filter_south_america_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::continent_south_america) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_south_america_button::button_function(ui::simple_button<filter_south_america_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::continent_south_america) ?
			country_sub_filter::all :
			country_sub_filter::continent_south_america;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_europe_button::update(ui::simple_button<filter_europe_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::continent_europe) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_europe_button::button_function(ui::simple_button<filter_europe_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::continent_europe) ?
			country_sub_filter::all :
			country_sub_filter::continent_europe;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_africa_button::update(ui::simple_button<filter_africa_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::continent_africa) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_africa_button::button_function(ui::simple_button<filter_africa_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::continent_africa) ?
			country_sub_filter::all :
			country_sub_filter::continent_africa;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_asia_button::update(ui::simple_button<filter_asia_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::continent_asia) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_asia_button::button_function(ui::simple_button<filter_asia_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::continent_asia) ?
			country_sub_filter::all :
			country_sub_filter::continent_asia;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_oceania_button::update(ui::simple_button<filter_oceania_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::continent_oceania) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_oceania_button::button_function(ui::simple_button<filter_oceania_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::continent_oceania) ?
			country_sub_filter::all :
			country_sub_filter::continent_oceania;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_neighbours_button::update(ui::simple_button<filter_neighbours_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::neighbor) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_neighbours_button::button_function(ui::simple_button<filter_neighbours_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::neighbor) ?
			country_sub_filter::all :
			country_sub_filter::neighbor;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_sphere_button::update(ui::simple_button<filter_sphere_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::sphere) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_sphere_button::button_function(ui::simple_button<filter_sphere_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::sphere) ?
			country_sub_filter::all :
			country_sub_filter::sphere;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_enemies_button::update(ui::simple_button<filter_enemies_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::enemy) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_enemies_button::button_function(ui::simple_button<filter_enemies_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::enemy) ?
			country_sub_filter::all :
			country_sub_filter::enemy;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void filter_allies_button::update(ui::simple_button<filter_allies_button>& self, world_state & ws) {
		if(ws.w.production_w.filter == country_sub_filter::ally) {
			self.associated_object->flags.fetch_and(uint16_t(~ui::gui_object::display_as_disabled), std::memory_order_release);
		} else {
			self.associated_object->flags.fetch_or(ui::gui_object::display_as_disabled, std::memory_order_release);
		}
	}
	void filter_allies_button::button_function(ui::simple_button<filter_allies_button>&, world_state & ws) {
		ws.w.production_w.filter = (ws.w.production_w.filter == country_sub_filter::ally) ?
			country_sub_filter::all :
			country_sub_filter::ally;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void clear_text_button::on_create(ui::simple_button<clear_text_button>& self, world_state& ws) {
		ui::clear_children(ws.w.gui_m, ui::tagged_gui_object{ *self.associated_object, ui::gui_object_tag(0) });
	}
	void sort_by_country_button::button_function(ui::simple_button<sort_by_country_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::name;
		ws.w.production_w.update(ws.w.gui_m);
	}

	void sort_by_my_flag_button::update(ui::masked_flag<sort_by_my_flag_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			self.set_displayed_flag(ws, player->tag);
		}
	}
	void sort_by_my_flag_button::button_function(ui::masked_flag<sort_by_my_flag_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_self;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag0_button::update(ui::masked_flag<sort_by_gpflag0_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 0 < r.second) {
			if(auto id = *(r.first + 0); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag0_button::button_function(ui::masked_flag<sort_by_gpflag0_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_one;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag1_button::update(ui::masked_flag<sort_by_gpflag1_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 1 < r.second) {
			if(auto id = *(r.first + 1); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag1_button::button_function(ui::masked_flag<sort_by_gpflag1_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_two;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag2_button::update(ui::masked_flag<sort_by_gpflag2_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 2 < r.second) {
			if(auto id = *(r.first + 2); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag2_button::button_function(ui::masked_flag<sort_by_gpflag2_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_three;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag3_button::update(ui::masked_flag<sort_by_gpflag3_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 3 < r.second) {
			if(auto id = *(r.first + 3); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag3_button::button_function(ui::masked_flag<sort_by_gpflag3_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_four;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag4_button::update(ui::masked_flag<sort_by_gpflag4_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 4 < r.second) {
			if(auto id = *(r.first + 4); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag4_button::button_function(ui::masked_flag<sort_by_gpflag4_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_five;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag5_button::update(ui::masked_flag<sort_by_gpflag5_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 5 < r.second) {
			if(auto id = *(r.first + 5); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag5_button::button_function(ui::masked_flag<sort_by_gpflag5_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_six;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag6_button::update(ui::masked_flag<sort_by_gpflag6_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 6 < r.second) {
			if(auto id = *(r.first + 6); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag6_button::button_function(ui::masked_flag<sort_by_gpflag6_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_seven;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag7_button::update(ui::masked_flag<sort_by_gpflag7_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + 7 < r.second) {
			if(auto id = *(r.first + 7); ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, ws.w.nation_s.nations[id].tag);
		}
	}
	void sort_by_gpflag7_button::button_function(ui::masked_flag<sort_by_gpflag7_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_eight;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_boss_button::button_function(ui::simple_button<sort_by_boss_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::sphere_leader;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_prestige_button::button_function(ui::simple_button<sort_by_prestige_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::prestige_rank;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_economic_button::button_function(ui::simple_button<sort_by_economic_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::economic_rank;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_military_button::button_function(ui::simple_button<sort_by_military_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::military_rank;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_total_button::button_function(ui::simple_button<sort_by_total_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::overall_rank;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_opinion_button::button_function(ui::simple_button<sort_by_opinion_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::opinion;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_relation_button::button_function(ui::simple_button<sort_by_relation_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::relation;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_invest_factories_button::button_function(ui::simple_button<sort_by_invest_factories_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::number_of_factories;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void investment_country_item_background_button::button_function(ui::simple_button<investment_country_item_background_button>& self, world_state & ws) {
		ws.w.production_w.show_particular_foreign_investment(ws.w.gui_m, tag);
	}
	ui::window_tag investment_country_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["investment_country_entry"]);
	}
	void sort_by_project_state_button::button_function(ui::simple_button<sort_by_project_state_button>&, world_state & ws) {
		ws.w.production_w.project_sort_type = project_sort::state;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_project_type_button::button_function(ui::simple_button<sort_by_project_type_button>&, world_state & ws) {
		ws.w.production_w.project_sort_type = project_sort::project_type;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_project_completion_button::button_function(ui::simple_button<sort_by_project_completion_button>&, world_state & ws) {
		ws.w.production_w.project_sort_type = project_sort::completion;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_project_investors_button::button_function(ui::simple_button<sort_by_project_investors_button>&, world_state & ws) {
		ws.w.production_w.project_sort_type = project_sort::investors;
		ws.w.production_w.update(ws.w.gui_m);
	}
}
