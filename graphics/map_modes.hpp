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

	class map_icon_base : public ui::gui_behavior {
	public:
		provinces::province_tag tag;
	};

	class map_port_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<map_port_icon>& self, window_type& win, world_state& ws);
	};

	class map_fort_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<map_fort_icon>& self, window_type& win, world_state& ws);
	};

	class map_port_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class map_rail_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class map_fort_text {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using infrastructure_map_icon_window_t = ui::gui_window <
		CT_STRING("port_icon"), ui::dynamic_icon<map_port_icon>,
		CT_STRING("fort_icon"), ui::dynamic_icon<map_fort_icon>,
		CT_STRING("port_level"), ui::display_text<map_port_text>,
		CT_STRING("rail_level"), ui::display_text<map_rail_text>,
		CT_STRING("fort_level"), ui::display_text<map_fort_text>,
		map_icon_base
	>;

	class legend_gui {
	public:
		generic_legend_window_t generic_legend_window;
		generic_gradient_legend_window_t generic_gradient_legend_window;
		population_legend_window_t population_legend_window;
		relations_legend_window_t relations_legend_window;
		migration_legend_window_t migration_legend_window;

		bool showing_density = false;
		bool showing_internal_migration = false;

		ui::gui_object* infrastructure_map_container;
		std::vector<ui::gui_object*> infrastructure_map_icons;
		std::vector< infrastructure_map_icon_window_t> infrastructure_icons_windows;

		provinces::province_tag current_province;
		nations::country_tag current_nation;
		cultures::culture_tag current_culture;
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
	void map_fort_icon::windowed_update(ui::dynamic_icon<map_fort_icon>& self, window_type & win, world_state & ws) {
		auto const fort_level = ws.w.province_s.province_state_container.get<province_state::fort_level>(win.tag);
		self.set_visibility(ws.w.gui_m, fort_level > 0);
	}
	template<typename window_type>
	void map_port_text::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto const port_level = ws.w.province_s.province_state_container.get<province_state::naval_base_level>(win.tag);
		if(port_level > 0) {
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ port_level },
				ui::text_format{ui::text_color::outlined_black, fmt.font_handle, fmt.font_size}, ws, box, lm);
		}
	}
	template<typename window_type>
	void map_rail_text::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto const rr_level = ws.w.province_s.province_state_container.get<province_state::railroad_level>(win.tag);
		ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ rr_level },
			ui::text_format{ ui::text_color::outlined_black, fmt.font_handle, fmt.font_size }, ws, box, lm);
	}
	template<typename window_type>
	void map_fort_text::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto const fort_level = ws.w.province_s.province_state_container.get<province_state::fort_level>(win.tag);
		if(fort_level > 0) {
			ui::add_text(ui::xy_pair{ 0,0 }, text_data::integer{ fort_level },
				ui::text_format{ ui::text_color::outlined_black, fmt.font_handle, fmt.font_size }, ws, box, lm);
		}
	}
}
