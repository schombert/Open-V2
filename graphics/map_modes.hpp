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

	class legend_gui {
	public:
		generic_legend_window_t generic_legend_window;
		generic_gradient_legend_window_t generic_gradient_legend_window;
		population_legend_window_t population_legend_window;

		bool showing_density = false;

		provinces::province_tag current_province;
		cultures::culture_tag current_culture;
	};
	template<typename W>
	void generic_legend_window_base::on_create(W & w, world_state & ws) {
		ui::hide(*associated_object);
	}
}
