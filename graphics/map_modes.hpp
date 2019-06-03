#pragma once
#include "map_modes.h"
#include "gui\gui.hpp"
#include "world_state/world_state.h"

namespace map_mode {
	class generic_legend_window_base : public ui::window_pane {
	public:
		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class generic_legend_title {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class generic_legend_contents {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class generic_legend_min {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class generic_legend_max {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gradient_bar {
	public:
		void update(ui::tinted_icon<gradient_bar>& self, world_state& ws);
	};

	class population_amount_button {
	public:
		void update(ui::simple_button<population_amount_button>& self, world_state& ws);
		void button_function(ui::simple_button<population_amount_button>& self, world_state& ws);
	};

	class population_density_button {
	public:
		void update(ui::simple_button<population_density_button>& self, world_state& ws);
		void button_function(ui::simple_button<population_density_button>& self, world_state& ws);
	};

	class external_button {
	public:
		void update(ui::simple_button<external_button>& self, world_state& ws);
		void button_function(ui::simple_button<external_button>& self, world_state& ws);
	};

	class internal_button {
	public:
		void update(ui::simple_button<internal_button>& self, world_state& ws);
		void button_function(ui::simple_button<internal_button>& self, world_state& ws);
	};

	using generic_legend_window_t = ui::gui_window<
		CT_STRING("legend_title"), ui::display_text<generic_legend_title>,
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		generic_legend_window_base
	>;

	using generic_gradient_legend_window_t = ui::gui_window<
		CT_STRING("legend_title"), ui::display_text<generic_legend_title>,
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		CT_STRING("gradient_icon"), ui::tinted_icon<gradient_bar>,
		CT_STRING("gradient_min"), ui::display_text<generic_legend_min>,
		CT_STRING("gradient_max"), ui::display_text<generic_legend_max>,
		generic_legend_window_base
	>;

	using population_legend_window_t = ui::gui_window <
		CT_STRING("legend_title"), ui::display_text<generic_legend_title>,
		CT_STRING("amount_cb"), ui::simple_button<population_amount_button>,
		CT_STRING("density_cb"), ui::simple_button<population_density_button>,
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		CT_STRING("gradient_icon"), ui::tinted_icon<gradient_bar>,
		CT_STRING("gradient_min"), ui::display_text<generic_legend_min>,
		CT_STRING("gradient_max"), ui::display_text<generic_legend_max>,
		generic_legend_window_base
	>;

	using relations_legend_window_t = ui::gui_window<
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		CT_STRING("gradient_min"), ui::display_text<generic_legend_min>,
		CT_STRING("gradient_max"), ui::display_text<generic_legend_max>,
		generic_legend_window_base
	>;

	using migration_legend_window_t = ui::gui_window <
		CT_STRING("external_cb"), ui::simple_button<external_button>,
		CT_STRING("internal_cb"), ui::simple_button<internal_button>,
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		CT_STRING("gradient_min"), ui::display_text<generic_legend_min>,
		CT_STRING("gradient_max"), ui::display_text<generic_legend_max>,
		generic_legend_window_base
	>;

	class railroad_color {
	public:
		void update(ui::tinted_icon<railroad_color>& self, world_state& ws);
	};

	class naval_base_color {
	public:
		void update(ui::tinted_icon<naval_base_color>& self, world_state& ws);
	};

	using infrastructure_legend_window_t = ui::gui_window <
		CT_STRING("build_rr_color"), ui::tinted_icon<railroad_color>,
		CT_STRING("build_nb_color"), ui::tinted_icon<naval_base_color>,
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		CT_STRING("gradient_min"), ui::display_text<generic_legend_min>,
		CT_STRING("gradient_max"), ui::display_text<generic_legend_max>,
		generic_legend_window_base
	>;

	class rgo_item_base : public ui::visible_region {
	public:
		economy::goods_tag tag;
		void set_value(economy::goods_tag t) {
			tag = t;
		}
	};

	class rgo_item_button {
	public:
		economy::goods_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<rgo_item_button>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<rgo_item_button>& self, world_state& ws);
	};

	class rgo_item_color {
	public:
		template<typename window_type>
		void windowed_update(ui::tinted_icon<rgo_item_color>& self, window_type& win, world_state& ws);
	};

	class rgo_item_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<rgo_item_icon>& self, window_type& win, world_state& ws);
	};

	class rgo_item_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using rgo_item = ui::gui_window <
		CT_STRING("background"), ui::simple_button<rgo_item_button>,
		CT_STRING("resource_color"), ui::tinted_icon<rgo_item_color>,
		CT_STRING("resource_icon"), ui::dynamic_icon<rgo_item_icon>,
		CT_STRING("resource_name"), ui::display_text<rgo_item_name>,
		rgo_item_base
	>;

	class rgo_items_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using rgo_legend_window_t = ui::gui_window <
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		CT_STRING("rgo_items"), ui::discrete_listbox<rgo_items_lb, rgo_item, economy::goods_tag>,
		CT_STRING("gradient_icon"), ui::tinted_icon<gradient_bar>,
		CT_STRING("gradient_min"), ui::display_text<generic_legend_min>,
		CT_STRING("gradient_max"), ui::display_text<generic_legend_max>,
		generic_legend_window_base
	>;

	class map_icon_base : public ui::gui_behavior {
	public:
		provinces::province_tag tag;
	};

	class map_port_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<map_port_icon>& self, window_type& win, world_state& ws);
	};

	class map_port_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};


	using infrastructure_map_icon_window_t = ui::gui_window <
		CT_STRING("port_icon"), ui::dynamic_icon<map_port_icon>,
		CT_STRING("port_level"), ui::display_text<map_port_text>,
		map_icon_base
	>;

	class rgo_map_icon {
	public:
		provinces::province_tag tag;
		void update(ui::dynamic_icon<rgo_map_icon>& self, world_state& ws);
	};

	class legend_gui {
	public:
		generic_legend_window_t generic_legend_window;
		generic_gradient_legend_window_t generic_gradient_legend_window;
		population_legend_window_t population_legend_window;
		relations_legend_window_t relations_legend_window;
		migration_legend_window_t migration_legend_window;
		infrastructure_legend_window_t infrastructure_legend_window;
		rgo_legend_window_t rgo_legend_window;

		bool showing_density = false;
		bool showing_internal_migration = false;

		ui::gui_object* infrastructure_map_container;
		std::vector<ui::gui_object*> infrastructure_map_icons;
		std::vector< infrastructure_map_icon_window_t> infrastructure_icons_windows;

		ui::gui_object* rgo_map_container;
		std::vector<ui::gui_object*> rgo_map_icons;
		std::vector<ui::dynamic_icon<rgo_map_icon>> rgo_map_icons_objects;

		provinces::province_tag current_province;
		nations::country_tag current_nation;
		cultures::culture_tag current_culture;
		economy::goods_tag current_good;
	};

	template<typename W>
	void generic_legend_window_base::on_create(W & w, world_state & ws) {
		ui::hide(*associated_object);
	}
	template<typename window_type>
	void map_port_icon::windowed_update(ui::dynamic_icon<map_port_icon>& self, window_type & win, world_state & ws) {
		auto const port_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(win.tag);
		self.set_visibility(ws.w.gui_m, port_level > 0);
	}
	template<typename window_type>
	void map_port_text::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto const port_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(win.tag);
		if(port_level > 0) {
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ port_level },
				ui::text_format{ui::text_color::white, fmt.font_handle, fmt.font_size}, ws, box, lm);
		}
	}
	template<typename window_type>
	void rgo_item_button::windowed_update(ui::simple_button<rgo_item_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		if(ws.w.map_view.legends->current_good == tag)
			self.set_frame(ws.w.gui_m, 0);
		else
			self.set_frame(ws.w.gui_m, 1);
	}
	template<typename window_type>
	void rgo_item_color::windowed_update(ui::tinted_icon<rgo_item_color>& self, window_type & win, world_state & ws) {
		auto clr = win.tag ? ws.s.economy_m.goods[win.tag].color : graphics::color_rgb{ 255ui8, 255ui8, 255ui8 };
		self.set_color(ws.w.gui_m, float(clr.r) / 255.0f, float(clr.g) / 255.0f, float(clr.b) / 255.0f);
	}
	template<typename window_type>
	void rgo_item_icon::windowed_update(ui::dynamic_icon<rgo_item_icon>& self, window_type & win, world_state & ws) {
		if(win.tag)
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[win.tag].icon);
		else
			self.set_frame(ws.w.gui_m, 0);
	}
	template<typename window_type>
	void rgo_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(win.tag)
			ui::add_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[win.tag].name, fmt, ws, box, lm);
		else
			ui::add_text(ui::xy_pair{ 0,0 }, scenario::fixed_ui::all, fmt, ws, box, lm);
	}
	template<typename lb_type>
	void rgo_items_lb::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<economy::goods_tag, 32, concurrent_allocator<economy::goods_tag>> data;
		data.push_back(economy::goods_tag());
		ws.s.economy_m.for_each_good([&ws, &data](economy::goods_tag g) {
			if((ws.s.economy_m.goods[g].flags & economy::good_definition::has_rgo) != 0)
				data.push_back(g);
		});
		lb.new_list(data.begin(), data.end());
	}
}
