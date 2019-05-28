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

	using generic_legend_window_t = ui::gui_window<
		CT_STRING("legend_title"), ui::display_text<generic_legend_title>,
		CT_STRING("legend_contents"), ui::multiline_text<generic_legend_contents>,
		generic_legend_window_base
	>;

	class legend_gui {
	public:
		generic_legend_window_t generic_legend_window;

		provinces::province_tag current_province;
		cultures::culture_tag current_culture;
	};
	template<typename W>
	void generic_legend_window_base::on_create(W & w, world_state & ws) {
		ui::hide(*associated_object);
	}
}
