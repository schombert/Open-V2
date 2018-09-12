#pragma once
#include "common\\common.h"
#include "population.h"
#include "gui\\gui.hpp"
#include "simple_mpl\\simple_mpl.hpp"

class world_state;

namespace population {

	class pop_country_base : public ui::visible_region {
	public:
		nations::country_tag tag;

		void set_value(size_t, nations::country_tag t) {
			tag = t;
		}
	};

	class pop_country_open_button {
	public:
		nations::country_tag tag;

		template<typename W>
		void windowed_update(ui::simple_button<pop_country_open_button>&, W& w, world_state& ws);
		void button_function(ui::simple_button<pop_country_open_button>&, world_state&);
	};

	class pop_country_name {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_country_size {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_country_growth {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_country_growth>&, W& w, world_state& ws);
	};

	using pop_country_item = ui::gui_window <
		CT_STRING("poplistbutton"), ui::simple_button<pop_country_open_button>,
		CT_STRING("poplist_name"), ui::display_text<pop_country_name, 2>,
		CT_STRING("poplist_numpops"), ui::display_text<pop_country_size>,
		CT_STRING("growth_indicator"), ui::dynamic_icon<pop_country_growth>,
		pop_country_base
	> ;

	class pop_state_base : public ui::visible_region {
	public:
		nations::state_tag tag;

		void set_value(size_t, nations::state_tag t) {
			tag = t;
		}
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class pop_state_open_button {
	public:
		nations::state_tag tag;

		template<typename W>
		void windowed_update(ui::simple_button<pop_state_open_button>&, W& w, world_state& ws);
		void button_function(ui::simple_button<pop_state_open_button>&, world_state&);
	};

	class pop_state_name {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_state_focus_button {
	public:
		nations::state_tag tag;

		template<typename W>
		void windowed_update(ui::simple_button<pop_state_focus_button>&, W& w, world_state& ws);
		void button_function(ui::simple_button<pop_state_focus_button>&, world_state&);
	};

	class pop_colonial_state_button {
	public:
		template<typename W>
		void windowed_update(ui::simple_button<pop_colonial_state_button>&, W& w, world_state& ws);
	};

	class pop_state_size {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_state_expand_button {
	public:
		nations::state_tag tag;

		template<typename W>
		void windowed_update(ui::simple_button<pop_state_expand_button>&, W& w, world_state& ws);
		void button_function(ui::simple_button<pop_state_expand_button>&, world_state&);
		void on_create(ui::simple_button<pop_state_expand_button>&, world_state&);
	};

	class pop_state_growth {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_state_growth>&, W& w, world_state& ws);
	};

	using pop_state_item = ui::gui_window <
		CT_STRING("poplistbutton"), ui::simple_button<pop_state_open_button>,
		CT_STRING("poplist_name"), ui::display_text<pop_state_name, 6>,
		CT_STRING("colonial_state_icon"), ui::simple_button<pop_colonial_state_button>,
		CT_STRING("state_focus"), ui::simple_button<pop_state_focus_button>,
		CT_STRING("poplist_numpops"), ui::display_text<pop_state_size>,
		CT_STRING("expand"), ui::simple_button<pop_state_expand_button>,
		CT_STRING("growth_indicator"), ui::dynamic_icon<pop_state_growth>,
		pop_state_base
	>;

	class pop_province_base : public ui::visible_region {
	public:
		provinces::province_tag tag;

		void set_value(size_t, provinces::province_tag t) {
			tag = t;
		}
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class pop_province_open_button {
	public:
		provinces::province_tag tag;

		template<typename W>
		void windowed_update(ui::simple_button<pop_province_open_button>&, W& w, world_state& ws);
		void button_function(ui::simple_button<pop_province_open_button>&, world_state&);
	};

	class pop_province_name {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_province_size {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_province_growth {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_province_growth>&, W& w, world_state& ws);
	};

	using pop_province_item = ui::gui_window <
		CT_STRING("poplistbutton"), ui::simple_button<pop_province_open_button>,
		CT_STRING("poplist_name"), ui::display_text<pop_province_name>,
		CT_STRING("poplist_numpops"), ui::display_text<pop_province_size>,
		CT_STRING("growth_indicator"), ui::dynamic_icon<pop_province_growth>,
		pop_province_base
	>;

	class pop_tree_view {
	public:
		static ui::window_tag element_tag(size_t level, ui::gui_static& gui_m);

		std::vector<nations::country_tag, concurrent_allocator<nations::country_tag>> base_list(world_state& ws);
		template<size_t level>
		std::vector<nations::state_tag, concurrent_allocator<nations::state_tag>> sub_list(nations::country_tag t, world_state& ws);
		template<size_t level>
		std::vector<provinces::province_tag, concurrent_allocator<provinces::province_tag>> sub_list(nations::state_tag t, world_state& ws);
	};

	class pop_list_item_base : public ui::visible_region {
	public:
		pop_tag tag;

		void set_value(pop_tag t) {
			tag = t;
		}
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class pop_size {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_type_button {
	public:
		pop_type_tag type;
		
		template<typename W>
		void windowed_update(ui::simple_button<pop_type_button>&, W& w, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class pop_producing_icon {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_producing_icon>& self, W& w, world_state& ws);
	};

	class pop_culture {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_religion {
	public:
		cultures::religion_tag religion;

		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_religion>&, W& w, world_state& ws);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class pop_location {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_militancy {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_consciousness {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_ideology {
	public:
		template<typename window_type>
		void windowed_update(ui::piechart<pop_ideology>& pie, window_type&, world_state& ws);
	};

	class pop_issues {
	public:
		template<typename window_type>
		void windowed_update(ui::piechart<pop_issues>& pie, window_type&, world_state& ws);
	};

	class pop_unemployment_bar {
	public:
		template<typename window_type>
		void windowed_update(ui::progress_bar<pop_unemployment_bar>& bar, window_type&, world_state& ws);
	};

	class pop_cash {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class lifeneed_progress {
	public:
		template<typename window_type>
		void windowed_update(ui::progress_bar<lifeneed_progress>& bar, window_type&, world_state& ws);
	};
	class eveneed_progress {
	public:
		template<typename window_type>
		void windowed_update(ui::progress_bar<eveneed_progress>& bar, window_type&, world_state& ws);
	};
	class luxneed_progress {
	public:
		template<typename window_type>
		void windowed_update(ui::progress_bar<luxneed_progress>& bar, window_type&, world_state& ws);
	};

	class pops_unempl_overlay {
	public:
		float value = 0.0f;

		template<typename W>
		void windowed_update(ui::dynamic_icon<pops_unempl_overlay>&, W& w, world_state& ws);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class lifeneed_progress_overlay {
	public:
		float value = 0.0f;

		template<typename W>
		void windowed_update(ui::dynamic_icon<lifeneed_progress_overlay>&, W& w, world_state& ws);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class eveneed_progress_overlay {
	public:
		float value = 0.0f;

		template<typename W>
		void windowed_update(ui::dynamic_icon<eveneed_progress_overlay>&, W& w, world_state& ws);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};
	class luxneed_progress_overlay {
	public:
		float value = 0.0f;

		template<typename W>
		void windowed_update(ui::dynamic_icon<luxneed_progress_overlay>&, W& w, world_state& ws);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class pop_literacy {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class pop_revolt {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_revolt>&, W& w, world_state& ws);

		//bool has_tooltip(world_state&) { return true; }
		//void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class pop_movement_social {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_movement_social>&, W& w, world_state& ws);

		//bool has_tooltip(world_state&) { return true; }
		//void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class pop_movement_political {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_movement_political>&, W& w, world_state& ws);

		//bool has_tooltip(world_state&) { return true; }
		//void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class pop_movement_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<pop_movement_flag>& self, W& w, world_state& ws);
		//bool has_tooltip(world_state&) { return true; }
		//void create_tooltip(ui::masked_flag<pop_movement_flag>& self, world_state& ws, ui::tagged_gui_object tw);
	};

	class pop_growth {
	public:
		template<typename W>
		void windowed_update(ui::dynamic_icon<pop_growth>&, W& w, world_state& ws);

		//bool has_tooltip(world_state&) { return true; }
		//void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	using pop_list_item = ui::gui_window<
		CT_STRING("pop_size"), ui::display_text<pop_size>,
		CT_STRING("pop_type"), ui::simple_button<pop_type_button>,
		CT_STRING("pop_producing_icon"), ui::dynamic_icon<pop_producing_icon>,
		CT_STRING("pop_nation"), ui::display_text<pop_culture>,
		CT_STRING("pop_religion"), ui::dynamic_icon<pop_religion>,
		CT_STRING("pop_location"), ui::display_text<pop_location>,
		CT_STRING("pop_mil"), ui::display_text<pop_militancy>,
		CT_STRING("pop_con"), ui::display_text<pop_consciousness>,
		CT_STRING("pop_ideology"), ui::piechart<pop_ideology>,
		CT_STRING("pop_issues"), ui::piechart<pop_issues>,
		CT_STRING("pop_unemployment_bar"), ui::progress_bar<pop_unemployment_bar>,
		CT_STRING("pop_cash"), ui::display_text<pop_cash>,
		CT_STRING("lifeneed_progress"), ui::progress_bar<lifeneed_progress>,
		CT_STRING("eveneed_progress"), ui::progress_bar<eveneed_progress>,
		CT_STRING("luxneed_progress"), ui::progress_bar<luxneed_progress>,
		CT_STRING("pops_unempl_overlay"), ui::dynamic_icon<pops_unempl_overlay>,
		CT_STRING("lifeneed_progress_overlay"), ui::dynamic_icon<lifeneed_progress_overlay>,
		CT_STRING("eveneed_progress_overlay"), ui::dynamic_icon<eveneed_progress_overlay>,
		CT_STRING("luxneed_progress_overlay"), ui::dynamic_icon<luxneed_progress_overlay>,
		CT_STRING("pop_revolt"), ui::dynamic_icon<pop_revolt>,
		CT_STRING("pop_movement_social"), ui::dynamic_icon<pop_movement_social>,
		CT_STRING("pop_movement_political"), ui::dynamic_icon<pop_movement_political>,
		CT_STRING("pop_movement_flag"), ui::masked_flag<pop_movement_flag>,
		CT_STRING("growth_indicator"), ui::dynamic_icon<pop_growth>,
		CT_STRING("pop_literacy"), ui::display_text<pop_literacy>,
		pop_list_item_base
	>;

	class population_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class pop_filter_button {
	public:
		pop_type_tag type;

		pop_filter_button(pop_type_tag t) : type(t) {}
		void button_function(ui::simple_button<pop_filter_button>&, world_state&);
		void update(ui::simple_button<pop_filter_button>&, world_state&);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

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
	>;

	class religion_title {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class religion_chart {
	public:
		void update(ui::piechart<religion_chart>& pie, world_state& ws);
	};

	class religion_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using religion_details_window = ui::gui_window <
		CT_STRING("item_name"), ui::display_text<religion_title, -22>,
		CT_STRING("chart"), ui::piechart<religion_chart>,
		CT_STRING("member_names"), ui::discrete_listbox<religion_lb, legend_item, std::tuple<graphics::color_rgb, text_data::text_tag, float>, 7>,
		ui::visible_region
	>;

	class culture_title {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class culture_chart {
	public:
		void update(ui::piechart<culture_chart>& pie, world_state& ws);
	};

	class culture_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using culture_details_window = ui::gui_window <
		CT_STRING("item_name"), ui::display_text<culture_title, -22>,
		CT_STRING("chart"), ui::piechart<culture_chart>,
		CT_STRING("member_names"), ui::discrete_listbox<culture_lb, legend_item, std::tuple<graphics::color_rgb, text_data::text_tag, float>, 7>,
		ui::visible_region
	>;

	class ideology_title {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class ideology_chart {
	public:
		void update(ui::piechart<ideology_chart>& pie, world_state& ws);
	};

	class ideology_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using ideology_details_window = ui::gui_window <
		CT_STRING("item_name"), ui::display_text<ideology_title, -22>,
		CT_STRING("chart"), ui::piechart<ideology_chart>,
		CT_STRING("member_names"), ui::discrete_listbox<ideology_lb, legend_item, std::tuple<graphics::color_rgb, text_data::text_tag, float>, 7>,
		ui::visible_region
	>;

	class issues_title {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class issues_chart {
	public:
		void update(ui::piechart<issues_chart>& pie, world_state& ws);
	};

	class issues_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using issues_details_window = ui::gui_window <
		CT_STRING("item_name"), ui::display_text<issues_title, -22>,
		CT_STRING("chart"), ui::piechart<issues_chart>,
		CT_STRING("member_names"), ui::discrete_listbox<issues_lb, legend_item, std::tuple<graphics::color_rgb, text_data::text_tag, float>, 7>,
		ui::visible_region
	>;

	class electorate_title {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class electorate_chart {
	public:
		void update(ui::piechart<electorate_chart>& pie, world_state& ws);
	};

	class electorate_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using electorate_details_window = ui::gui_window <
		CT_STRING("item_name"), ui::display_text<electorate_title, -22>,
		CT_STRING("chart"), ui::piechart<electorate_chart>,
		CT_STRING("member_names"), ui::discrete_listbox<electorate_lb, legend_item, std::tuple<graphics::color_rgb, text_data::text_tag, float>, 7>,
		ui::visible_region
	>;

	class population_window_base : public ui::draggable_region {
	public:
		workforce_details_window workforce;
		culture_details_window cultures_w;
		ideology_details_window ideologies_w;
		issues_details_window issues_w;
		religion_details_window religions_w;
		electorate_details_window electorate_w;

		std::vector<ui::simple_button<pop_filter_button>> filter_buttons;

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
		CT_STRING("pop_province_list"),
			ui::tree_view<pop_tree_view,
				type_list<nations::country_tag, nations::state_tag, provinces::province_tag>,
				type_list<pop_country_item, pop_state_item, pop_province_item>>,
		CT_STRING("pop_list"), ui::discrete_listbox<population_lb, pop_list_item, pop_tag>,
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
