#include "common\\common.h"
#include "economy_gui.hpp"
#include "commands/commands.h"

namespace economy {
	void close_button_s::button_function(ui::simple_button<close_button_s>&, world_state& ws) {
		ws.w.production_w.hide(ws.w.gui_m);
	}
	production_window::production_window() : win(std::make_unique<production_window_t>()) {}
	production_window::~production_window() {}
	void production_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void production_window::init(world_state & ws) {
		factory_goods_filters.resize(ws.s.economy_m.goods_count);
		std::fill(factory_goods_filters.begin(), factory_goods_filters.end(), 1ui8);
		create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["country_production"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void production_window::update(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("investment_browser")>().template get<CT_STRING("country_listbox")>().new_list(nullptr, nullptr);
		win->template get<CT_STRING("state_listbox")>().new_list(nullptr, nullptr);
		win->template get<CT_STRING("state_listbox_invest")>().new_list(nullptr, nullptr);
		win->template get<CT_STRING("project_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void production_window::show(ui::gui_manager & gui_m) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m);
	}
	void production_window::show_factories(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 0);

		win->template get<CT_STRING("state_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("state_listbox")>().associated_object));

		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("factory_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("project_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("good_production")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox_invest")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("invest_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		show(gui_m);
	}
	void production_window::show_projects(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 3);
		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("factory_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("invest_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox_invest")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("good_production")>().associated_object));
		win->template get<CT_STRING("project_listbox")>().new_list(nullptr, nullptr);
		ui::make_visible_immediate(*(win->template get<CT_STRING("project_listbox")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::make_visible_immediate(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		show(gui_m);
	}
	void production_window::show_production(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 2);

		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("good_production")>().associated_object));

		ui::hide(*(win->template get<CT_STRING("project_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox_invest")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("invest_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("factory_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		show(gui_m);
	}
	void production_window::show_foreign_investment(ui::gui_manager & gui_m) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 1);
		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("project_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("good_production")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox_invest")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("factory_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("invest_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		foreign_investment_nation = nations::country_tag();
		show(gui_m);
	}
	void production_window::show_particular_foreign_investment(ui::gui_manager & gui_m, nations::country_tag target) {
		win->template get<CT_STRING("production_tab_button_group")>().set_selected(gui_m, 1);

		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("invest_buttons")>().associated_object));
		win->template get<CT_STRING("state_listbox_invest")>().new_list(nullptr, nullptr);
		ui::make_visible_and_update(gui_m, *(win->template get<CT_STRING("state_listbox_invest")>().associated_object));

		ui::hide(*(win->template get<CT_STRING("project_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("good_production")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("state_listbox")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("factory_buttons")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("investment_browser")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_state")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projects")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_completion")>().associated_object));
		ui::hide(*(win->template get<CT_STRING("sort_by_projecteers")>().associated_object));
		foreign_investment_nation = target;
		show(gui_m);
	}
	void production_tab_button_group_b::on_select(world_state & ws, uint32_t i) {
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
			self.set_displayed_flag(ws, player);
		}
	}
	void sort_by_my_flag_button::button_function(ui::masked_flag<sort_by_my_flag_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_self;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag0_button::update(ui::masked_flag<sort_by_gpflag0_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 0 < end(r)) {
			if(auto id = r.first[0]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag0_button::button_function(ui::masked_flag<sort_by_gpflag0_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_one;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag1_button::update(ui::masked_flag<sort_by_gpflag1_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 1 < end(r)) {
			if(auto id = r.first[1]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag1_button::button_function(ui::masked_flag<sort_by_gpflag1_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_two;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag2_button::update(ui::masked_flag<sort_by_gpflag2_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 2 < end(r)) {
			if(auto id = r.first[2]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag2_button::button_function(ui::masked_flag<sort_by_gpflag2_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_three;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag3_button::update(ui::masked_flag<sort_by_gpflag3_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 3 < end(r)) {
			if(auto id = r.first[3]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag3_button::button_function(ui::masked_flag<sort_by_gpflag3_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_four;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag4_button::update(ui::masked_flag<sort_by_gpflag4_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 4 < end(r)) {
			if(auto id = r.first[4]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag4_button::button_function(ui::masked_flag<sort_by_gpflag4_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_five;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag5_button::update(ui::masked_flag<sort_by_gpflag5_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 5 < end(r)) {
			if(auto id = r.first[5]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag5_button::button_function(ui::masked_flag<sort_by_gpflag5_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_six;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag6_button::update(ui::masked_flag<sort_by_gpflag6_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 6 < end(r)) {
			if(auto id = r.first[6]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
		}
	}
	void sort_by_gpflag6_button::button_function(ui::masked_flag<sort_by_gpflag6_button>&, world_state & ws) {
		ws.w.production_w.sort_type = country_sort::gp_seven;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_by_gpflag7_button::update(ui::masked_flag<sort_by_gpflag7_button>& self, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(begin(r) + 7 < end(r)) {
			if(auto id = r.first[7]; ws.w.nation_s.nations.is_valid_index(id))
				self.set_displayed_flag(ws, id);
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
	void sort_factories_by_infrastructure::button_function(ui::simple_button<sort_factories_by_infrastructure>& self, world_state & ws) {
		ws.w.production_w.factory_sort_type = factories_sort::infrastructure;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_factories_by_count::button_function(ui::simple_button<sort_factories_by_count>& self, world_state & ws) {
		ws.w.production_w.factory_sort_type = factories_sort::count;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_factories_by_state_name::button_function(ui::simple_button<sort_factories_by_state_name>& self, world_state & ws) {
		ws.w.production_w.factory_sort_type = factories_sort::state_name;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void show_hide_empty_states_button::update(ui::button<show_hide_empty_states_button>& self, world_state & ws) {
		if(ws.w.production_w.show_empty_states)
			self.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::hide_empty_states]);
		else
			self.set_text(ws, ws.s.fixed_ui_text[scenario::fixed_ui::show_empty_states]);
	}
	void show_hide_empty_states_button::button_function(ui::button<show_hide_empty_states_button>& self, world_state & ws) {
		ws.w.production_w.show_empty_states = !ws.w.production_w.show_empty_states;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void deselect_all_factories_filters_button::button_function(ui::simple_button<deselect_all_factories_filters_button>& self, world_state & ws) {
		std::fill(ws.w.production_w.factory_goods_filters.begin(), ws.w.production_w.factory_goods_filters.end(), 0ui8);
		ws.w.production_w.update(ws.w.gui_m);
	}
	void select_all_factories_filters_button::button_function(ui::simple_button<select_all_factories_filters_button>& self, world_state & ws) {
		std::fill(ws.w.production_w.factory_goods_filters.begin(), ws.w.production_w.factory_goods_filters.end(), 1ui8);
		ws.w.production_w.update(ws.w.gui_m);
	}
	void subsidize_all_button::update(ui::simple_button<subsidize_all_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) 
			self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::can_subsidise) != 0);
	}
	void subsidize_all_button::button_function(ui::simple_button<subsidize_all_button>& self, world_state & ws) {}
	void unsubsidize_all_button::update(ui::simple_button<unsubsidize_all_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player)
			self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::can_subsidise) != 0);
	}
	void unsubsidize_all_button::button_function(ui::simple_button<unsubsidize_all_button>& self, world_state & ws) {}
	void open_all_factories_button::update(ui::simple_button<open_all_factories_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player)
			self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::open_factory) != 0);
	}
	void open_all_factories_button::button_function(ui::simple_button<open_all_factories_button>& self, world_state & ws) {}
	void close_all_factories_button::update(ui::simple_button<close_all_factories_button>& self, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player)
			self.set_enabled((ws.w.nation_s.nations.get<nation::current_rules>(player) & issues::rules::open_factory) != 0);
	}
	void close_all_factories_button::button_function(ui::simple_button<close_all_factories_button>& self, world_state & ws) {}
	void good_filter_item_button::button_function(ui::simple_button<good_filter_item_button>& self, world_state & ws) {
		ws.w.production_w.factory_goods_filters[to_index(tag)] =
			(ws.w.production_w.factory_goods_filters[to_index(tag)] != 0) ? 0ui8 : 1ui8;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_factories_by_worker_a::button_function(ui::button<sort_factories_by_worker_a>& self, world_state & ws) {
		ws.w.production_w.factory_sort_type = factories_sort::worker_pop_a;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_factories_by_worker_b::button_function(ui::button<sort_factories_by_worker_b>& self, world_state & ws) {
		ws.w.production_w.factory_sort_type = factories_sort::worker_pop_b;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void sort_factories_by_owner::button_function(ui::button<sort_factories_by_owner>& self, world_state & ws) {
		ws.w.production_w.factory_sort_type = factories_sort::owner_pop;
		ws.w.production_w.update(ws.w.gui_m);
	}
	void investment_target_flag::update(ui::masked_flag<investment_target_flag>& self, world_state & ws) {
		self.set_displayed_flag(ws, ws.w.production_w.foreign_investment_nation);
	}
	void investment_target_flag_overlay::update(ui::dynamic_icon<investment_target_flag_overlay>& self, world_state & ws) {
		if(is_valid_index(ws.w.production_w.foreign_investment_nation)) {
			if(ws.w.nation_s.nations.get<nation::is_civilized>(ws.w.production_w.foreign_investment_nation) == false)
				self.set_frame(ws.w.gui_m, 3ui32);
			else if(nations::is_great_power(ws, ws.w.production_w.foreign_investment_nation))
				self.set_frame(ws.w.gui_m, 0ui32);
			else if(ws.w.nation_s.nations.get<nation::overall_rank>(ws.w.production_w.foreign_investment_nation) <= int16_t(ws.s.modifiers_m.global_defines.colonial_rank))
				self.set_frame(ws.w.gui_m, 1ui32);
			else
				self.set_frame(ws.w.gui_m, 2ui32);
			
		}
	}
	void investment_target_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(ws.w.production_w.foreign_investment_nation)) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations.get<nation::name>(ws.w.production_w.foreign_investment_nation), fmt,
				ws, box, lm);
		}
	}
	void investment_target_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(ws.w.production_w.foreign_investment_nation)) {
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::currency{ nations::get_foreign_investment(ws, player, ws.w.production_w.foreign_investment_nation) }, fmt,
				ws, box, lm);
		}
	}
	void investment_target_back_button::button_function(ui::simple_button<investment_target_back_button>& self, world_state & ws) {
		ws.w.production_w.show_foreign_investment(ws.w.gui_m);
	}
	void state_focus_button::button_function(ui::simple_button<state_focus_button>& self, world_state & ws) {
		auto rc = ui::absolute_position(ws.w.gui_m, ui::find_in_parent(ws.w.gui_m, *(self.associated_object)))
			+ ui::xy_pair{ self.associated_object->size.x, int16_t(self.associated_object->size.y / 2) };
		ws.w.national_focus_w.show(ws.w.gui_m, tag, rc.x, rc.y);
	}
	void state_focus_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		modifiers::nf_tooltip_text(tag, ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format);
	}
	void state_build_factory_button::button_function(ui::simple_button<state_build_factory_button>& self, world_state & ws) {
		ws.w.build_factory_w.show(ws.w.gui_m, tag);
	}
	void state_build_factory_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::line_manager lm;
		auto const fmt = ui::tooltip_text_format;
		ui::xy_pair cursor_in{ 0,0 };

		bool state_not_full = economy::count_factories_in_state(ws, tag) < state::factories_count;
		if(state_not_full) {
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, tw, lm);
		} else {
			ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, tw, lm);
		}

		text_data::text_replacement vrep(text_data::value_type::value, text_data::integer{ state::factories_count });
		cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::factory_limit, fmt, ws, tw, lm, &vrep, 1);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		auto const state_owner = ws.w.nation_s.states.get<state::owner>(tag);
		auto const owner_rules = ws.w.nation_s.nations.get<nation::current_rules>(state_owner);

		if(state_owner == ws.w.local_player_nation) {
			bool rule_allowed = (issues::rules::build_factory & owner_rules) != 0;
			if(rule_allowed) {
				ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, tw, lm);
			} else {
				ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, tw, lm);
			}
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::factory_building_allowed, fmt, ws, tw, lm);
		} else {
			bool rule_allowed = (issues::rules::allow_foreign_investment & owner_rules) != 0;
			if(rule_allowed) {
				ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, tw, lm);
			} else {
				ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
				cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, tw, lm);
			}
			cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::foreign_investment_allowed, fmt, ws, tw, lm);
		}
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);

		if(!nations::is_colonial_or_protectorate(ws, tag)) {
			ui::text_format local_fmt{ ui::text_color::green, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u2714 ", local_fmt, ws, tw, lm);
		} else {
			ui::text_format local_fmt{ ui::text_color::red, fmt.font_handle, fmt.font_size };
			cursor_in = ui::add_text(cursor_in, u"\u274C ", local_fmt, ws, tw, lm);
		}
		cursor_in = ui::add_text(cursor_in, scenario::fixed_ui::not_colonial, fmt, ws, tw, lm);
		cursor_in = ui::advance_cursor_to_newline(cursor_in, ws.s.gui_m, fmt, lm);
	}
	ui::window_tag state_details_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["state_info"]);
	}
	ui::window_tag investment_state_details_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["state_info"]);
	}
	void factory_open_background::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void factory_closed_background::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void factory_cancel_construction_button::button_function(ui::simple_button<factory_cancel_construction_button>& self, world_state & ws) {}
	void factory_cancel_construction_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void factory_upgrade_button::button_function(ui::button<factory_upgrade_button>& self, key_modifiers mod, world_state & ws) {}
	void factory_upgrade_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void factory_subsidize_button::button_function(ui::simple_button<factory_subsidize_button>& self, world_state & ws) {}
	void factory_subsidize_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void hidden_button::update(ui::simple_button<hidden_button>& self, world_state & ws) {
		ui::hide(*self.associated_object);
	}
	void factory_delete_button::button_function(ui::simple_button<factory_delete_button>& self, world_state & ws) {}
	void factory_delete_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void factory_open_or_close_button::button_function(ui::simple_button<factory_open_or_close_button>& self, world_state & ws) {}
	void factory_open_or_close_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void state_window_base::set_value(nations::state_tag t) {
		tag = t;
		workers_a.in_state = t;
		workers_b.in_state = t;
		owner.in_state = t;
	}
	void state_window_base::update(world_state & ws) {
		if(is_valid_index(tag)) {
			uint32_t fcounter = 0;

			auto& state_factories = ws.w.nation_s.states.get<state::factories>(tag);
			for(int32_t i = 0; i < state::factories_count; ++i) {
				if(auto t = state_factories[i].type; t) {
					if(ws.w.production_w.factory_goods_filters[to_index(t->output_good)] != 0) {
						factories[fcounter].location = tag;
						factories[fcounter].index = i;
						ui::make_visible_and_update(ws.w.gui_m, *(factories[fcounter].associated_object));
						++fcounter;
					}
				}
			}

			for(; fcounter < state::factories_count; ++fcounter) {
				ui::hide(*(factories[fcounter].associated_object));
				factories[fcounter].location = tag;
				factories[fcounter].index = -1;
			}
		} else {
			for(uint32_t fcounter = 0; fcounter < state::factories_count; ++fcounter) {
				ui::hide(*(factories[fcounter].associated_object));
				factories[fcounter].location = tag;
				factories[fcounter].index = -1;
			}
		}
	}
	void hidden_icon::update(ui::dynamic_icon<hidden_icon>& self, world_state& ws) {
		ui::hide(*self.associated_object);
	}
	void production_backkground::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	void project_invest_button::button_function(ui::button<project_invest_button>& self, key_modifiers mod, world_state & ws) {}
	void project_invest_button::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {}
	ui::window_tag projects_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["project_info"]);
	}
	void bf_state_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.states.get<state::name>(ws.w.build_factory_w.in_state), fmt, ws, box, lm);
	}
	void bf_output_icon::update(ui::dynamic_icon<bf_output_icon>& self, world_state & ws) {
		if(auto const g = ws.w.build_factory_w.factory_type; g) {
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[g].icon);
		}
	}
	void bf_output_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		if(auto const g = ws.w.build_factory_w.factory_type; g) {
			ui::add_text(ui::xy_pair{0,0}, ws.s.economy_m.goods[g].name, ui::tooltip_text_format, ws, tw);
		}
	}
	void bf_factory_name::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.factory_types[f_type].name, fmt, ws, box, lm);
		}
	}
	ui::window_tag factory_types_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["open_v2_factory_option"]);
	}
	ui::window_tag factory_workers_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["factory_pop"]);
	}
	ui::window_tag factory_construction_costs_lb::element_tag(ui::gui_static & m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["goods_cost_window"]);
	}
	void factory_worker_item_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.population_m.pop_types[tag].name, ui::tooltip_text_format, ws, tw);
	}
	void factory_construction_cost_item_icon::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		ui::add_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[tag].name, ui::tooltip_text_format, ws, tw);
	}
	void bf_profit_label::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::profit, fmt, ws, box, lm);
	}
	void bf_profit_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type) {
			auto const amount = project_factory_profit(ws, ws.w.build_factory_w.in_state, f_type);
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::currency{ amount }, ui::text_format{fmt.color, fmt.font_handle, fmt.font_size - 1}, ws, box, lm);
		}
	}
	void bf_description::update(ui::tagged_gui_object box, ui::line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type) {
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.factory_types[f_type].description, fmt, ws, box, lm);
		}
	}
	void bf_total_workers_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type) {
			auto& f_type_obj = ws.s.economy_m.factory_types[f_type];
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ f_type_obj.factory_workers.workforce }, fmt, ws, box, lm);
		}
	}
	void bf_base_price_label::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	void bf_input_price_label::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	void bf_base_price_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	void bf_input_price_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	void bf_stockpile_lack_label::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	void bf_stockpile_lack_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	void bf_treasury_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto const player = ws.w.local_player_nation; player)
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::currency{ ws.w.nation_s.nations.get<nation::treasury>(player) }, fmt, ws, box, lm);
	}
	void bf_total_label::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::total_cost, fmt, ws, box, lm);
	}
	void bf_total_amount::update(ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto const s = ws.w.build_factory_w.in_state;
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type && s) {
			auto const cost = total_factory_construction_cost(ws, s, f_type);
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::currency{ cost }, fmt, ws, box, lm);
		}
	}
	void bf_cancel::button_function(ui::simple_button<bf_cancel>&, world_state & ws) {
		ws.w.build_factory_w.hide(ws.w.gui_m);
	}
	void factory_item_bg::button_function(ui::simple_button<factory_item_bg>& self, world_state & ws) {
		if(tag) {
			ws.w.build_factory_w.update(ws.w.gui_m, ws.w.build_factory_w.in_state, tag);
		}
	}
	void bf_build::update(ui::simple_button<bf_build>& self, world_state & ws) {
		auto const s = ws.w.build_factory_w.in_state;
		auto const player = ws.w.local_player_nation;
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type && s && player) {
			self.set_enabled(commands::is_command_valid(commands::build_factory(player, s, f_type), ws));
		}
	}
	void bf_build::button_function(ui::simple_button<bf_build>&, world_state & ws) {
		auto const s = ws.w.build_factory_w.in_state;
		auto const player = ws.w.local_player_nation;
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type && s && player) {
			ws.w.pending_commands.add<commands::build_factory>(player, s, f_type);
		}
		ws.w.build_factory_w.hide(ws.w.gui_m);
	}
	void bf_build::create_tooltip(world_state & ws, ui::tagged_gui_object tw) {
		auto const s = ws.w.build_factory_w.in_state;
		auto const player = ws.w.local_player_nation;
		if(auto const f_type = good_to_factory_type(ws, ws.w.build_factory_w.factory_type); f_type && s && player) {
			ui::line_manager lm;
			commands::explain_command_conditions(commands::build_factory(player, s, f_type), ws, tw, ui::xy_pair{ 0,0 }, lm, ui::tooltip_text_format);
		}
	}
	build_factory_window::build_factory_window() : win(std::make_unique<build_factory_window_t>()) {}
	build_factory_window::~build_factory_window() {}
	void build_factory_window::hide(ui::gui_manager & gui_m) {
		ui::hide(*(win->associated_object));
	}
	void build_factory_window::init(world_state & ws) {
		create_static_element(ws, std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["build_factory"]), ui::tagged_gui_object{ ws.w.gui_m.root, ui::gui_object_tag(0) }, *win);
	}
	void build_factory_window::update(ui::gui_manager & gui_m, nations::state_tag s, economy::goods_tag g) {
		in_state = s;
		factory_type = g;
		ui::make_visible_and_update(gui_m, *(win->associated_object));
	}
	void build_factory_window::show(ui::gui_manager & gui_m, nations::state_tag s) {
		ui::move_to_front(gui_m, ui::tagged_gui_object{ *(win->associated_object), win->window_object });
		update(gui_m, s, goods_tag());
	}
}
