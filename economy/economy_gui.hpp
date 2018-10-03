#pragma once
#include "common\\common.h"
#include "world_state\\world_state.h"
#include "gui\\gui.hpp"
#include "economy_functions.h"
#include "economy_gui.h"
#include "nations\\nations_functions.h"

namespace economy {
	class production_window_base : public ui::draggable_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class close_button {
	public:
		void button_function(ui::simple_button<close_button>&, world_state&);
	};

	class production_tab_button_group {
	public:
		void on_select(world_state& ws, uint32_t i);
	};

	class empty_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_pane_base : public ui::window_pane {
	public:
	};

	class filter_all_button {
	public:
		void update(ui::simple_button<filter_all_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_all_button>&, world_state&);
	};

	class filter_north_america_button {
	public:
		void update(ui::simple_button<filter_north_america_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_north_america_button>&, world_state&);
	};

	class filter_south_america_button {
	public:
		void update(ui::simple_button<filter_south_america_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_south_america_button>&, world_state&);
	};

	class filter_europe_button {
	public:
		void update(ui::simple_button<filter_europe_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_europe_button>&, world_state& ws);
	};

	class filter_africa_button {
	public:
		void update(ui::simple_button<filter_africa_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_africa_button>&, world_state& ws);
	};

	class filter_asia_button {
	public:
		void update(ui::simple_button<filter_asia_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_asia_button>&, world_state& ws);
	};

	class filter_oceania_button {
	public:
		void update(ui::simple_button<filter_oceania_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_oceania_button>&, world_state& ws);
	};

	class filter_neighbours_button {
	public:
		void update(ui::simple_button<filter_neighbours_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_neighbours_button>&, world_state& ws);
	};

	class filter_sphere_button {
	public:
		void update(ui::simple_button<filter_sphere_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_sphere_button>&, world_state& ws);
	};
	class filter_enemies_button {
	public:
		void update(ui::simple_button<filter_enemies_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_enemies_button>&, world_state& ws);
	};
	class filter_allies_button {
	public:
		void update(ui::simple_button<filter_allies_button>& self, world_state& ws);
		void button_function(ui::simple_button<filter_allies_button>&, world_state& ws);
	};

	class clear_text_button {
	public:
		void on_create(ui::simple_button<clear_text_button>&, world_state&);
	};

	class sort_by_country_button {
	public:
		void button_function(ui::simple_button<sort_by_country_button>&, world_state& ws);
	};
	class sort_by_my_flag_button {
	public:
		void update(ui::masked_flag<sort_by_my_flag_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_my_flag_button>&, world_state& ws);
	};
	class sort_by_gpflag0_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag0_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag0_button>&, world_state& ws);
	};
	class sort_by_gpflag1_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag1_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag1_button>&, world_state& ws);
	};
	class sort_by_gpflag2_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag2_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag2_button>&, world_state& ws);
	};
	class sort_by_gpflag3_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag3_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag3_button>&, world_state& ws);
	};
	class sort_by_gpflag4_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag4_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag4_button>&, world_state& ws);
	};
	class sort_by_gpflag5_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag5_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag5_button>&, world_state& ws);
	};
	class sort_by_gpflag6_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag6_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag6_button>&, world_state& ws);
	};
	class sort_by_gpflag7_button {
	public:
		void update(ui::masked_flag<sort_by_gpflag7_button>& self, world_state& ws);
		void button_function(ui::masked_flag<sort_by_gpflag7_button>&, world_state& ws);
	};
	class sort_by_boss_button {
	public:
		void button_function(ui::simple_button<sort_by_boss_button>&, world_state& ws);
	};
	class sort_by_prestige_button {
	public:
		void button_function(ui::simple_button<sort_by_prestige_button>&, world_state& ws);
	};
	class sort_by_economic_button {
	public:
		void button_function(ui::simple_button<sort_by_economic_button>&, world_state& ws);
	};
	class sort_by_military_button {
	public:
		void button_function(ui::simple_button<sort_by_military_button>&, world_state& ws);
	};
	class sort_by_total_button {
	public:
		void button_function(ui::simple_button<sort_by_total_button>&, world_state& ws);
	};
	class sort_by_opinion_button {
	public:
		void button_function(ui::simple_button<sort_by_opinion_button>&, world_state& ws);
	};
	class sort_by_relation_button {
	public:
		void button_function(ui::simple_button<sort_by_relation_button>&, world_state& ws);
	};
	class sort_by_invest_factories_button {
	public:
		void button_function(ui::simple_button<sort_by_invest_factories_button>&, world_state& ws);
	};

	class sort_by_project_state_button {
	public:
		void button_function(ui::simple_button<sort_by_project_state_button>&, world_state& ws);
	};
	class sort_by_project_type_button {
	public:
		void button_function(ui::simple_button<sort_by_project_type_button>&, world_state& ws);
	};
	class sort_by_project_completion_button {
	public:
		void button_function(ui::simple_button<sort_by_project_completion_button>&, world_state& ws);
	};
	class sort_by_project_investors_button {
	public:
		void button_function(ui::simple_button<sort_by_project_investors_button>&, world_state& ws);
	};

	class investment_country_item_base : public ui::visible_region {
	public:
		nations::country_tag tag;
		void set_value(nations::country_tag t) {
			tag = t;
		}

		template<typename W>
		void on_create(W& w, world_state&);
	};

	class investment_country_item_background_button {
	public:
		nations::country_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<investment_country_item_background_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<investment_country_item_background_button>& self, world_state& ws);
	};

	class investment_country_item_flag {
	public:
	   template<typename W>
	   void windowed_update(ui::masked_flag<investment_country_item_flag>& self, W& w, world_state& ws);
	};

	class investment_country_item_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_self_investment_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	template<int32_t index>
	class gp_investment_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_sphere_leader_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<investment_country_item_sphere_leader_flag>& self, W& w, world_state& ws);
	};

	class investment_country_item_military_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_industrial_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_prestige_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_overall_rank {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_opinion_type {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_relations_value {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class investment_country_item_factories_count {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using investment_country_item = ui::gui_window<
		CT_STRING("country_select"), ui::simple_button<investment_country_item_background_button>,
		CT_STRING("country_flag"), ui::masked_flag<investment_country_item_flag>,
		CT_STRING("country_name"), ui::display_text<investment_country_item_name>,
		CT_STRING("invest"), ui::display_text<investment_country_item_self_investment_text>,
		CT_STRING("country_gp0"), ui::display_text<gp_investment_text<0>>,
		CT_STRING("country_gp1"), ui::display_text<gp_investment_text<1>>,
		CT_STRING("country_gp2"), ui::display_text<gp_investment_text<2>>,
		CT_STRING("country_gp3"), ui::display_text<gp_investment_text<3>>,
		CT_STRING("country_gp4"), ui::display_text<gp_investment_text<4>>,
		CT_STRING("country_gp5"), ui::display_text<gp_investment_text<5>>,
		CT_STRING("country_gp6"), ui::display_text<gp_investment_text<6>>,
		CT_STRING("country_gp7"), ui::display_text<gp_investment_text<7>>,
		CT_STRING("country_boss_flag"), ui::masked_flag<investment_country_item_sphere_leader_flag>,
		CT_STRING("country_prestige"), ui::display_text<investment_country_item_prestige_rank>,
		CT_STRING("country_economic"), ui::display_text<investment_country_item_industrial_rank>,
		CT_STRING("country_military"), ui::display_text<investment_country_item_military_rank>,
		CT_STRING("country_total"), ui::display_text<investment_country_item_overall_rank>,
		CT_STRING("country_opinion"), ui::display_text<investment_country_item_opinion_type>,
		CT_STRING("country_relation"), ui::display_text<investment_country_item_relations_value>,
		CT_STRING("factories"), ui::display_text<investment_country_item_factories_count>,
		investment_country_item_base
	>;

	class investment_country_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using investment_pane = ui::gui_window<
		CT_STRING("filter_all"), ui::simple_button<filter_all_button>,
		CT_STRING("filter_north_america"), ui::simple_button<filter_north_america_button>,
		CT_STRING("filter_south_america"), ui::simple_button<filter_south_america_button>,
		CT_STRING("filter_europe"), ui::simple_button<filter_europe_button>,
		CT_STRING("filter_africa"), ui::simple_button<filter_africa_button>,
		CT_STRING("filter_asia"), ui::simple_button<filter_asia_button>,
		CT_STRING("filter_oceania"), ui::simple_button<filter_oceania_button>,
		CT_STRING("filter_neighbours"), ui::simple_button<filter_neighbours_button>,
		CT_STRING("filter_sphere"), ui::simple_button<filter_sphere_button>,
		CT_STRING("filter_enemies"), ui::simple_button<filter_enemies_button>,
		CT_STRING("filter_allies"), ui::simple_button<filter_allies_button>,
		CT_STRING("sort_by_country"), ui::simple_button<sort_by_country_button>,
		CT_STRING("sort_by_my_invest"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_my_flag"), ui::masked_flag<sort_by_my_flag_button>,
		CT_STRING("sort_by_gp0"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag0"), ui::masked_flag<sort_by_gpflag0_button>,
		CT_STRING("sort_by_gp1"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag1"), ui::masked_flag<sort_by_gpflag1_button>,
		CT_STRING("sort_by_gp2"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag2"), ui::masked_flag<sort_by_gpflag2_button>,
		CT_STRING("sort_by_gp3"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag3"), ui::masked_flag<sort_by_gpflag3_button>,
		CT_STRING("sort_by_gp4"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag4"), ui::masked_flag<sort_by_gpflag4_button>,
		CT_STRING("sort_by_gp5"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag5"), ui::masked_flag<sort_by_gpflag5_button>,
		CT_STRING("sort_by_gp6"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag6"), ui::masked_flag<sort_by_gpflag6_button>,
		CT_STRING("sort_by_gp7"), ui::simple_button<clear_text_button>,
		CT_STRING("sort_by_gpflag7"), ui::masked_flag<sort_by_gpflag7_button>,
		CT_STRING("sort_by_boss"), ui::simple_button<sort_by_boss_button>,
		CT_STRING("sort_by_prestige"), ui::simple_button<sort_by_prestige_button>,
		CT_STRING("sort_by_economic"), ui::simple_button<sort_by_economic_button>,
		CT_STRING("sort_by_military"), ui::simple_button<sort_by_military_button>,
		CT_STRING("sort_by_total"), ui::simple_button<sort_by_total_button>,
		CT_STRING("sort_by_opinion"), ui::simple_button<sort_by_opinion_button>,
		CT_STRING("sort_by_relation"), ui::simple_button<sort_by_relation_button>,
		CT_STRING("sort_by_invest_factories"), ui::simple_button<sort_by_invest_factories_button>,
		CT_STRING("country_listbox"), ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag>,
		investment_pane_base
	>;

	class production_window_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<close_button>,
		CT_STRING("tab_factories"), ui::button_group_member,
		CT_STRING("tab_invest"), ui::button_group_member,
		CT_STRING("tab_goodsproduction"), ui::button_group_member,
		CT_STRING("tab_popprojects"), ui::button_group_member,
		CT_STRING("production_tab_button_group"), ui::button_group<
			CT_STRING("tab_factories"),
			CT_STRING("tab_invest"),
			CT_STRING("tab_goodsproduction"),
			CT_STRING("tab_popprojects"), production_tab_button_group>,
		CT_STRING("tab_factories_text"), ui::display_text<empty_text_box>,
		CT_STRING("tab_invest_text"), ui::display_text<empty_text_box>,
		CT_STRING("tab_goodsproduction_text"), ui::display_text<empty_text_box>,
		CT_STRING("tab_popprojects_text"), ui::display_text<empty_text_box>,
		CT_STRING("investment_browser"), investment_pane,
		CT_STRING("sort_by_state"), ui::simple_button<sort_by_project_state_button>,
		CT_STRING("sort_by_projects"), ui::simple_button<sort_by_project_type_button>,
		CT_STRING("sort_by_completion"), ui::simple_button<sort_by_project_completion_button>,
		CT_STRING("sort_by_projecteers"), ui::simple_button<sort_by_project_investors_button>,
		production_window_base
	>{};


	template<typename W>
	void production_window_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
		w.template get<CT_STRING("production_tab_button_group")>().set_selected(ws.w.gui_m, 0);
	}
	template<typename W>
	void investment_country_item_base::on_create(W & w, world_state &) {
		associated_object->position.x = 0i16;
	}
	template<typename window_type>
	void investment_country_item_background_button::windowed_update(ui::simple_button<investment_country_item_background_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		if(auto player = ws.w.local_player_nation; player && is_valid_index(tag)) {
			self.set_enabled(possible_to_invest_in(ws, *player, ws.w.nation_s.nations[tag]));
		}
	}
	template<typename W>
	void investment_country_item_flag::windowed_update(ui::masked_flag<investment_country_item_flag>& self, W & w, world_state & ws) {
		self.set_displayed_flag(ws, w.tag);
	}
	template<typename window_type>
	void investment_country_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.nation_s.nations[win.tag].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_self_investment_text::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, nations::get_foreign_investment(ws, *player, win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<int32_t index>
	template<typename window_type>
	void gp_investment_text<index>::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank);
		if(r.first + index < r.second) {
			if(auto id = *(r.first + index); ws.w.nation_s.nations.is_valid_index(id)) {
				char16_t local_buffer[16];
				put_value_in_buffer(local_buffer, display_type::currency, nations::get_foreign_investment(ws, ws.w.nation_s.nations[id], win.tag));
				ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
				lm.finish_current_line();
			}
		}
	}
	template<typename W>
	void investment_country_item_sphere_leader_flag::windowed_update(ui::masked_flag<investment_country_item_sphere_leader_flag>& self, W & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			auto leader = ws.w.nation_s.nations[w.tag].sphere_leader;
			if(leader) {
				self.set_displayed_flag(ws, leader->tag);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}
	template<typename window_type>
	void investment_country_item_military_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].military_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_industrial_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].industrial_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_prestige_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].prestige_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_overall_rank::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, ws.w.nation_s.nations[win.tag].overall_rank);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_opinion_type::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, nations::influence_level_to_text(ws, nations::get_influence_level(ws, *player, win.tag)), fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_relations_value::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, nations::get_relationship(ws, *player, win.tag));
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void investment_country_item_factories_count::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			nations::country_tag t = win.tag;
			auto states = get_range(ws.w.nation_s.state_arrays, ws.w.nation_s.nations[t].member_states);
			int32_t total = 0;
			for(auto s = states.first; s != states.second; ++s) {
				if(auto si = s->state; si) {
					total += count_factories_in_state(*si);
				}
			}
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::integer, total);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename lb_type>
	void investment_country_lb::populate_list(lb_type & lb, world_state & ws) {
		std::vector<nations::country_tag, concurrent_allocator<nations::country_tag>> data;

		switch(ws.w.production_w.filter) {
			default:
			case country_sub_filter::all:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0)
						data.push_back(id);
				});
				break;
			case country_sub_filter::continent_africa:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.africa_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_asia:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.asia_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_europe:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.europe_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_north_america:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.north_america_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_oceania:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.oceania_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::continent_south_america:
				ws.w.nation_s.nations.for_each([&data, &ws](nations::nation const& n) {
					if(auto id = n.id; ws.w.nation_s.nations.is_valid_index(id) && get_size(ws.w.province_s.province_arrays, n.owned_provinces) != 0) {
						if(is_valid_index(n.current_capital) &&
							ws.s.province_m.province_container[n.current_capital].continent == ws.w.province_s.south_america_modifier)
							data.push_back(id);
					}
				});
				break;
			case country_sub_filter::enemy:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->opponents_in_war);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::ally:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->allies);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::neighbor:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->neighboring_nations);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
			case country_sub_filter::sphere:
				if(auto player = ws.w.local_player_nation; player) {
					auto srange = get_range(ws.w.nation_s.nations_arrays, player->sphere_members);
					data.insert(data.end(), srange.first, srange.second);
				}
				break;
		}

		switch(ws.w.production_w.sort_type) {
			default:
			case country_sort::none:
				break;
			case country_sort::name:
			{
				vector_backed_string_lex_less<char16_t> lss(ws.s.gui_m.text_data_sequences.text_data);
				std::sort(data.begin(), data.end(), [&ws, &lss](nations::country_tag a, nations::country_tag b) {
					auto a_name = ws.w.nation_s.nations[a].name;
					auto b_name = ws.w.nation_s.nations[b].name;
					return lss(
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, a_name),
						text_data::text_tag_to_backing(ws.s.gui_m.text_data_sequences, b_name));
				});
			}
			break;
			case country_sort::gp_self:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return nations::get_foreign_investment(ws, *player, a) > nations::get_foreign_investment(ws, *player, b);
					});					
				}
				break;
			case country_sort::gp_one:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 0 < r.second) {
					if(auto id = *(r.first + 0); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_two:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 1 < r.second) {
					if(auto id = *(r.first + 1); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_three:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 2 < r.second) {
					if(auto id = *(r.first + 2); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_four:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 3 < r.second) {
					if(auto id = *(r.first + 3); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_five:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 4 < r.second) {
					if(auto id = *(r.first + 4); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_six:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 5 < r.second) {
					if(auto id = *(r.first + 5); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_seven:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 6 < r.second) {
					if(auto id = *(r.first + 6); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::gp_eight:
				if(auto r = get_range(ws.w.nation_s.nations_arrays, ws.w.nation_s.nations_by_rank); r.first + 7 < r.second) {
					if(auto id = *(r.first + 7); ws.w.nation_s.nations.is_valid_index(id)) {
						std::sort(data.begin(), data.end(), [&ws, n = &ws.w.nation_s.nations[id]](nations::country_tag a, nations::country_tag b) {
							return nations::get_foreign_investment(ws, *n, a) > nations::get_foreign_investment(ws, *n, b);
						});
					}
				}
				break;
			case country_sort::sphere_leader:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations[a].sphere_leader > ws.w.nation_s.nations[b].sphere_leader;
				});
				break;
			case country_sort::prestige_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations[a].prestige_rank < ws.w.nation_s.nations[b].prestige_rank;
				});
				break;
			case country_sort::economic_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations[a].industrial_rank < ws.w.nation_s.nations[b].industrial_rank;
				});
				break;
			case country_sort::military_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations[a].military_rank < ws.w.nation_s.nations[b].military_rank;
				});
				break;
			case country_sort::overall_rank:
				std::sort(data.begin(), data.end(), [&ws](nations::country_tag a, nations::country_tag b) {
					return ws.w.nation_s.nations[a].overall_rank < ws.w.nation_s.nations[b].overall_rank;
				});
				break;
			case country_sort::opinion:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return nations::get_influence_level(ws, *player, a) > nations::get_influence_level(ws, *player, b);
					});
				}
				break;
			case country_sort::relation:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return nations::get_relationship(ws, *player, a) > nations::get_relationship(ws, *player, b);
					});
				}
				break;
			case country_sort::number_of_factories:
				if(auto player = ws.w.local_player_nation; player) {
					std::sort(data.begin(), data.end(), [&ws, player](nations::country_tag a, nations::country_tag b) {
						return count_factories_in_nation(ws, ws.w.nation_s.nations[a]) > count_factories_in_nation(ws, ws.w.nation_s.nations[b]);
					});
				}
				break;
		}

		lb.update_list(data.begin(), data.end());
	}
}
