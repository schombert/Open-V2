#pragma once
#include "common\\common.h"
#include "population.h"
#include "gui\\gui.hpp"
#include "simple_mpl\\simple_mpl.hpp"

class world_state;

namespace population {

	class legend_item_base : public ui::visible_region {
	public:
		graphics::color_rgb color;
		text_data::text_tag legend_name;
		float percentage = 0;

		void set_value(std::tuple<graphics::color_rgb, text_data::text_tag, float> const& t) {
			color = std::get<0>(t);
			legend_name = std::get<1>(t);
			percentage = std::get<2>(t);
		}
	};

	class legend_icon {
	public:
		template<typename W>
		void windowed_update(ui::tinted_icon<legend_icon>&, W& w, world_state& ws);
	};

	class legend_label {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class legend_percentage {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	using legend_item = ui::gui_window<
		CT_STRING("legend_color"), ui::tinted_icon<legend_icon>,
		CT_STRING("legend_title"), ui::display_text<legend_label>,
		CT_STRING("legend_value"), ui::display_text<legend_percentage, -20>,
		legend_item_base
	>;

	class workforce_title {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class workforce_chart {
	public:
		void update(ui::piechart<workforce_chart>& pie, world_state& ws);
	};

	class workforce_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using workforce_details_window = ui::gui_window <
		CT_STRING("item_name"), ui::display_text<workforce_title, -22>,
		CT_STRING("chart"), ui::piechart<workforce_chart>,
		CT_STRING("member_names"), ui::discrete_listbox<workforce_lb, legend_item, std::tuple<graphics::color_rgb, text_data::text_tag, float>, 7>,
		ui::visible_region
	> ;

	class population_window_base : public ui::visible_region {
	public:
		workforce_details_window workforce;

		population_window_base() {}
		population_window_base(population_window_base&&) = default;
		population_window_base(population_window_base& b) noexcept : population_window_base(std::move(b)) {}

		bool on_keydown(ui::gui_object_tag, world_state& ws, const ui::key_down& k) final override;
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class popfilter_all_button {
	public:
		void button_function(ui::simple_button<popfilter_all_button>&, world_state&);
	};
	class popfilter_none_button {
	public:
		void button_function(ui::simple_button<popfilter_none_button>&, world_state&);
	};
	class sortby_size_button {
	public:
		void button_function(ui::simple_button<sortby_size_button>&, world_state&);
	};
	class sortby_type_button {
	public:
		void button_function(ui::simple_button<sortby_type_button>&, world_state&);
	};
	class sortby_nationality_button {
	public:
		void button_function(ui::simple_button<sortby_nationality_button>&, world_state&);
	};
	class sortby_religion_button {
	public:
		void button_function(ui::simple_button<sortby_religion_button>&, world_state&);
	};
	class sortby_location_button {
	public:
		void button_function(ui::simple_button<sortby_location_button>&, world_state&);
	};
	class sortby_mil_button {
	public:
		void button_function(ui::simple_button<sortby_mil_button>&, world_state&);
	};
	class sortby_con_button {
	public:
		void button_function(ui::simple_button<sortby_con_button>&, world_state&);
	};
	class sortby_ideology_button {
	public:
		void button_function(ui::simple_button<sortby_ideology_button>&, world_state&);
	};
	class sortby_issues_button {
	public:
		void button_function(ui::simple_button<sortby_issues_button>&, world_state&);
	};
	class sortby_unemployment_button {
	public:
		void button_function(ui::simple_button<sortby_unemployment_button>&, world_state&);
	};
	class sortby_cash_button {
	public:
		void button_function(ui::simple_button<sortby_cash_button>&, world_state&);
	};
	class sortby_subsistence_button {
	public:
		void button_function(ui::simple_button<sortby_subsistence_button>&, world_state&);
	};
	class sortby_eve_button {
	public:
		void button_function(ui::simple_button<sortby_eve_button>&, world_state&);
	};
	class sortby_luxury_button {
	public:
		void button_function(ui::simple_button<sortby_luxury_button>&, world_state&);
	};
	class sortby_revoltrisk_button {
	public:
		void button_function(ui::simple_button<sortby_revoltrisk_button>&, world_state&);
	};
	class sortby_change_button {
	public:
		void button_function(ui::simple_button<sortby_change_button>&, world_state&);
	};
	class sortby_literacy_button {
	public:
		void button_function(ui::simple_button<sortby_literacy_button>&, world_state&);
	};

	class close_button {
	public:
		void button_function(ui::simple_button<close_button>&, world_state&);
	};

	class dummy_scrollbar {
	public:
		void on_position(int32_t) const {}
	};

	using population_window_t = ui::gui_window<
		CT_STRING("popfilter_ALL"), ui::simple_button<popfilter_all_button>,
		CT_STRING("popfilter_DESELECT_ALL"), ui::simple_button<popfilter_none_button>,

		CT_STRING("external_scroll_slider"), ui::scrollbar<dummy_scrollbar>,
		CT_STRING("sortby_size_button"), ui::simple_button<sortby_size_button>,
		CT_STRING("sortby_type_button"), ui::simple_button<sortby_type_button>,
		CT_STRING("sortby_nationality_button"), ui::simple_button<sortby_nationality_button>,
		CT_STRING("sortby_religion_button"), ui::simple_button<sortby_religion_button>,
		CT_STRING("sortby_location_button"), ui::simple_button<sortby_location_button>,
		CT_STRING("sortby_mil_button"), ui::simple_button<sortby_mil_button>,
		CT_STRING("sortby_con_button"), ui::simple_button<sortby_con_button>,
		CT_STRING("sortby_ideology_button"), ui::simple_button<sortby_ideology_button>,
		CT_STRING("sortby_issues_button"), ui::simple_button<sortby_issues_button>,
		CT_STRING("sortby_unemployment_button"), ui::simple_button<sortby_unemployment_button>,
		CT_STRING("sortby_cash_button"), ui::simple_button<sortby_cash_button>,
		CT_STRING("sortby_subsistence_button"), ui::simple_button<sortby_subsistence_button>,
		CT_STRING("sortby_eve_button"), ui::simple_button<sortby_eve_button>,
		CT_STRING("sortby_luxury_button"), ui::simple_button<sortby_luxury_button>,
		CT_STRING("sortby_revoltrisk_button"), ui::simple_button<sortby_revoltrisk_button>,
		CT_STRING("sortby_change_button"), ui::simple_button<sortby_change_button>,
		CT_STRING("sortby_literacy_button"), ui::simple_button<sortby_literacy_button>,
		CT_STRING("close_button"), ui::simple_button<close_button>,
		population_window_base
	>;
}
