#pragma once
#include "common\\common.h"
#include "governments_gui.h"
#include "triggers\\triggers.h"
#include "triggers\\effects.h"
#include "gui\\gui.hpp"

namespace governments {
	class government_window_base : public ui::draggable_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class close_button {
	public:
		void button_function(ui::simple_button<close_button>&, world_state&);
	};

	class government_type_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class government_description_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class social_reform_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class political_reform_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class plurality_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class revanchism_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class national_value_icon {
	public:
		void update(ui::dynamic_icon<national_value_icon>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class plurality_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class revanchism_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class social_reform_possible_icon {
	public:
		void update(ui::dynamic_icon<social_reform_possible_icon>& self, world_state& ws);
	};

	class political_reform_possible_icon {
	public:
		void update(ui::dynamic_icon<political_reform_possible_icon>& self, world_state& ws);
	};

	class modifier_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class modifier_button {
	public:
		const uint32_t icon;
		::modifiers::national_modifier_tag mod;
		date_tag expiration;

		modifier_button(uint32_t num, ::modifiers::national_modifier_tag m, date_tag x) : icon(num), mod(m), expiration(x) {}

		void update(ui::simple_button<modifier_button>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using modifier_item = ui::gui_window<
		CT_STRING("country_modifier_icon"), ui::simple_button<modifier_button>,
		ui::gui_behavior
	>;

	class governments_tab_button_group {
	public:
		void on_select(world_state& ws, uint32_t i);
	};

	class hold_election_button {
	public:
		bool is_enabled = false;

		void button_function(ui::simple_button<hold_election_button>&, world_state&);
		void update(ui::simple_button<hold_election_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return !is_enabled; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class sort_by_issue_name_button {
	public:
		void button_function(ui::simple_button<sort_by_issue_name_button>&, world_state&);
	};

	class sort_by_people_button {
	public:
		void button_function(ui::simple_button<sort_by_people_button>&, world_state&);
	};

	class sort_by_voters_button {
	public:
		void button_function(ui::simple_button<sort_by_voters_button>&, world_state&);
	};

	class upper_house_pie_chart {
	public:
		void update(ui::piechart<upper_house_pie_chart>& pie, world_state& ws);
	};

	class voters_ideologies_pie_chart {
	public:
		void update(ui::piechart<voters_ideologies_pie_chart>& pie, world_state& ws);
	};

	class people_ideologies_pie_chart {
	public:
		void update(ui::piechart<people_ideologies_pie_chart>& pie, world_state& ws);
	};

	class upper_house_item_base : public ui::visible_region {
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

	class upperhouse_ideology_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class upperhouse_ideology_icon {
	public:
		template<typename W>
		void windowed_update(ui::tinted_icon<upperhouse_ideology_icon>&, W& w, world_state& ws);
	};

	class upperhouse_ideology_name {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class upperhouse_ideology_percentage {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	using upper_house_item = ui::gui_window<
		CT_STRING("ideology_icon"), ui::tinted_icon<upperhouse_ideology_icon>,
		CT_STRING("ideology_name"), ui::display_text<upperhouse_ideology_name>,
		CT_STRING("ideology_perc"), ui::display_text<upperhouse_ideology_percentage>,
		upper_house_item_base
	>;

	class issues_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class issues_item_base : public ui::visible_region {
	public:
		text_data::text_tag legend_name;
		float people_percentage = 0;
		float voters_percent = 0;

		void set_value(std::tuple<text_data::text_tag, float, float> const& t) {
			legend_name = std::get<0>(t);
			people_percentage = std::get<1>(t);
			voters_percent = std::get<2>(t);
		}
	};

	class issues_item_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class issues_item_popular_percentage {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class issues_voter_percentage {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using issues_item = ui::gui_window<
		CT_STRING("issue_name"), ui::display_text<issues_item_name>,
		CT_STRING("people_perc"), ui::display_text<issues_item_popular_percentage>,
		CT_STRING("voters_perc"), ui::display_text<issues_voter_percentage>,
		issues_item_base
	>;

	class unciv_overlay {
	public:
		void button_function(ui::simple_button<unciv_overlay>& self, world_state& ws) {} //trap mouse
		void on_create(ui::simple_button<unciv_overlay>& self, world_state& ws);
		template<typename window_type>
		void windowed_update(ui::simple_button<unciv_overlay>& self, window_type& win, world_state& ws);
	};

	class decision_pane_base : public ui::window_pane {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class decision_listbox {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class decision_item_base : public ui::visible_region {
	public:
		events::decision_tag tag;
		void set_value(events::decision_tag t) {
			tag = t;
		}
	};

	class decision_item_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class decision_item_image {
	public:
		graphics::texture* default_image = nullptr;

		void on_create(ui::dynamic_icon<decision_item_image>& self, world_state& ws);
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<decision_item_image>& self, window_type& w, world_state& ws);
	};

	class decision_item_requirements {
	public:
		triggers::effect_tag tag;

		template<typename window_type>
		void windowed_update(ui::dynamic_icon<decision_item_requirements>& self, window_type& w, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class decision_item_checkbox {
	public:
		triggers::trigger_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<decision_item_checkbox>& self, window_type& w, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
		void button_function(ui::simple_button<decision_item_checkbox>& self, world_state& ws);
	};

	class enact_decision_button {
	public:
		triggers::effect_tag tag;
		triggers::trigger_tag requirements;

		template<typename window_type>
		void windowed_update(ui::simple_button<enact_decision_button>& self, window_type& w, world_state& ws);
		bool has_tooltip(world_state&) { return is_valid_index(requirements); }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
		void button_function(ui::simple_button<enact_decision_button>& self, world_state& ws);
	};

	class decision_item_description {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using decision_item = ui::gui_window<
		CT_STRING("decision_name"), ui::display_text<decision_item_name>,
		CT_STRING("decision_image"), ui::dynamic_icon<decision_item_image>,
		CT_STRING("decision_desc"), ui::multiline_text<decision_item_description>,
		CT_STRING("requirements"), ui::dynamic_icon<decision_item_requirements>,
		CT_STRING("ignore_checkbox"), ui::simple_button<decision_item_checkbox>,
		CT_STRING("make_decision"), ui::simple_button<enact_decision_button>,
		decision_item_base
	>;

	using decision_pane = ui::gui_window <
		CT_STRING("decision_listbox"), ui::discrete_listbox<decision_listbox, decision_item, events::decision_tag>,
		decision_pane_base
	>;

	class movements_pane_base : public ui::window_pane {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class supression_points_text_box {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class movement_sortby_size_button {
	public:
		void button_function(ui::simple_button<movement_sortby_size_button>&, world_state&);
	};

	class movement_sortby_radical_button {
	public:
		void button_function(ui::simple_button<movement_sortby_radical_button>&, world_state&);
	};

	class movement_sortby_name_button {
	public:
		void button_function(ui::simple_button<movement_sortby_name_button>&, world_state&);
	};

	class movements_item_base : public ui::visible_region {
	public:
		population::movement_tag tag;
		void set_value(population::movement_tag t) {
			tag = t;
		}
	};

	class movement_ind_flag {
	public:
		template<typename window_type>
		void windowed_update(ui::masked_flag<movement_ind_flag>& self, window_type& w, world_state& ws);
	};

	class movements_item_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class movements_item_nationalist_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class movements_item_size {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class movements_item_radicalism {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class movements_item_suppress_button {
	public:
		population::movement_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<movement_sortby_name_button>&, window_type& win, world_state& ws);
		void button_function(ui::simple_button<movement_sortby_name_button>&, world_state&);
	};

	using movements_item = ui::gui_window<
		CT_STRING("flag"), ui::masked_flag<movement_ind_flag>,
		CT_STRING("name"), ui::display_text<movements_item_name>,
		CT_STRING("nationalist_name"), ui::display_text<movements_item_nationalist_name>,
		CT_STRING("size"), ui::display_text<movements_item_size>,
		CT_STRING("radical_val"), ui::display_text<movements_item_radicalism>,
		CT_STRING("suppress_button"), ui::simple_button<movements_item_suppress_button>,
		movements_item_base
	>;

	class movements_listbox {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};


	class rebels_item_base : public ui::visible_region {
	public:
		population::rebel_faction_tag tag;
		void set_value(population::rebel_faction_tag t) {
			tag = t;
		}
	};

	class rebels_item_type_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<rebels_item_type_icon>&, window_type& win, world_state& ws);
	};

	class rebels_item_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class rebels_item_size {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class rebels_item_brigades_ready {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class rebels_item_brigades_active {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class rebels_item_organization {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class rebels_item_revolt_risk {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};


	using rebels_item = ui::gui_window<
		CT_STRING("type"), ui::dynamic_icon<rebels_item_type_icon>,
		CT_STRING("name"), ui::display_text<rebels_item_name>,
		CT_STRING("size"), ui::display_text<rebels_item_size>,
		CT_STRING("brigades_ready"), ui::display_text<rebels_item_brigades_ready>,
		CT_STRING("brigades_active"), ui::display_text<rebels_item_brigades_active>,
		CT_STRING("organization"), ui::display_text<rebels_item_organization>,
		CT_STRING("revolt_risk"), ui::display_text<rebels_item_revolt_risk>,
		rebels_item_base
	>;

	class rebels_listbox {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using movements_pane = ui::gui_window <
		CT_STRING("suppression_value"), ui::display_text<supression_points_text_box>,
		CT_STRING("sortby_size_button"), ui::simple_button<movement_sortby_size_button>,
		CT_STRING("sortby_radical_button"), ui::simple_button<movement_sortby_radical_button>,
		CT_STRING("sortby_name_button"), ui::simple_button<movement_sortby_name_button>,
		CT_STRING("movements_listbox"), ui::discrete_listbox<movements_listbox, movements_item, population::movement_tag>,
		CT_STRING("rebel_listbox"), ui::discrete_listbox<rebels_listbox, rebels_item, population::rebel_faction_tag>,
		movements_pane_base
	>;

	class reforms_listbox {
	public:
		issues::issue_tag for_issue;

		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class reform_item_base : public ui::visible_region {
	public:
		issues::option_tag tag;
		void set_value(issues::option_tag t) {
			tag = t;
		}
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class option_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class selected_option {
	public:
		issues::option_tag tag;

		template<typename window_type>
		void windowed_update(ui::dynamic_icon<selected_option>&, window_type& win, world_state& ws);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class unselected_option_button {
	public:
		issues::option_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<unselected_option_button>&, window_type& win, world_state& ws);
		void button_function(ui::simple_button<unselected_option_button>&, world_state&);

		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using reform_item = ui::gui_window <
		CT_STRING("reform_option"), ui::simple_button<unselected_option_button>,
		CT_STRING("selected"), ui::dynamic_icon<selected_option>,
		CT_STRING("reform_name"), ui::display_text<option_name, -4>,
		reform_item_base
	>;

	class reform_window_base : public ui::gui_behavior {
	public:
		issues::issue_tag for_issue;
		int32_t lb_x_offset = 0;
		int32_t lb_y_offset = 0;

		ui::discrete_listbox<reforms_listbox, reform_item, issues::option_tag> reform_options;

		template<typename W>
		void on_create(W& w, world_state&);
		void init(issues::issue_tag i, int32_t x, int32_t y) {
			for_issue = i; lb_x_offset = x; lb_y_offset = y;
			reform_options.for_issue = i;
		}
	};

	class reform_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	using reform_window = ui::gui_window <
		CT_STRING("reform_name"), ui::display_text<reform_name>,
		reform_window_base
	>;

	class civilized_reforms_pane_base : public ui::window_pane {
	public:
		std::vector<reform_window> reform_windows;

		int32_t lb_x_offset = 0;
		int32_t lb_y_offset = 0;
		bool static_data_loaded = false;
		ui::window_tag reform_window_tag;

		template<typename W>
		void on_create(W& w, world_state&);

		template<typename W>
		void windowed_update(W& w, world_state& ws);
		bool create_dynamic(world_state& ws, ui::tagged_gui_object win, ui::element_tag t, const char* name_start, const char* name_end);
	};

	using civilized_reforms_pane = ui::gui_window <
		civilized_reforms_pane_base
	>;


	class uncivilized_reforms_pane_base : public ui::window_pane {
	public:
		std::vector<reform_window> reform_windows;

		int32_t lb_x_offset = 0;
		int32_t lb_y_offset = 0;
		bool static_data_loaded = false;
		ui::window_tag reform_window_tag;

		template<typename W>
		void on_create(W& w, world_state&);

		template<typename W>
		void windowed_update(W& w, world_state& ws);
		bool create_dynamic(world_state& ws, ui::tagged_gui_object win, ui::element_tag t, const char* name_start, const char* name_end);
	};

	using uncivilized_reforms_pane = ui::gui_window <
		uncivilized_reforms_pane_base
	>;

	class release_nations_pane_base : public ui::window_pane {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
	};

	class release_nations_listbox {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class release_nations_item_base : public ui::visible_region {
	public:
		cultures::national_tag tag;
		void set_value(cultures::national_tag t) {
			tag = t;
		}
	};

	class release_nation_flag {
	public:
		template<typename window_type>
		void windowed_update(ui::masked_flag<release_nation_flag>& self, window_type& w, world_state& ws);
	};

	class release_nation_name {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class release_nation_type {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class release_nation_description {
	public:
		template<typename window_type>
		void windowed_update(window_type& win, ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class release_nation_button {
	public:
		cultures::national_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<release_nation_button>&, window_type& win, world_state& ws);
		void button_function(ui::simple_button<release_nation_button>&, world_state&);
	};

	using release_nations_item = ui::gui_window <
		CT_STRING("country_flag"), ui::masked_flag<release_nation_flag>,
		CT_STRING("name"), ui::display_text<release_nation_name>,
		CT_STRING("type"), ui::display_text<release_nation_type>,
		CT_STRING("desc"), ui::multiline_text<release_nation_description>,
		CT_STRING("country_release_vassal"), ui::simple_button<release_nation_button>,
		release_nations_item_base
	>;

	using release_nations_pane = ui::gui_window <
		CT_STRING("nations"), ui::discrete_listbox<release_nations_listbox, release_nations_item, cultures::national_tag>,
		release_nations_pane_base
	>;

	class government_window_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<close_button>,
		CT_STRING("government_name"), ui::display_text<government_type_text_box>,
		CT_STRING("national_value"), ui::dynamic_icon<national_value_icon>,
		CT_STRING("government_desc"), ui::display_text<government_description_text_box>,
		CT_STRING("can_do_social_reforms"), ui::display_text<social_reform_text_box, -4>,
		CT_STRING("can_do_political_reforms"), ui::display_text<political_reform_text_box, -4>,
		CT_STRING("plurality"), ui::dynamic_icon<plurality_icon>,
		CT_STRING("plurality_value"), ui::display_text<plurality_text_box>,
		CT_STRING("revanchism"), ui::dynamic_icon<revanchism_icon>,
		CT_STRING("revanchism_value"), ui::display_text<revanchism_text_box>,
		CT_STRING("social_reforms_bock"), ui::dynamic_icon<social_reform_possible_icon>,
		CT_STRING("political_reforms_bock"), ui::dynamic_icon<political_reform_possible_icon>,
		CT_STRING("country_modifier_overlappingbox"), ui::overlap_box<modifier_lb, ui::window_tag, modifier_item>,
		CT_STRING("reforms_tab"), ui::button_group_member,
		CT_STRING("movements_tab"), ui::button_group_member,
		CT_STRING("decisions_tab"), ui::button_group_member,
		CT_STRING("release_nations_tab"), ui::button_group_member,
		CT_STRING("governments_tab_button_group"), ui::button_group<
		CT_STRING("reforms_tab"),
		CT_STRING("movements_tab"),
		CT_STRING("decisions_tab"),
		CT_STRING("release_nations_tab"), governments_tab_button_group>,
		CT_STRING("hold_election"), ui::simple_button<hold_election_button>,
		CT_STRING("sort_by_issue_name"), ui::simple_button<sort_by_issue_name_button>,
		CT_STRING("sort_by_people"), ui::simple_button<sort_by_people_button>,
		CT_STRING("sort_by_voters"), ui::simple_button<sort_by_voters_button>,
		CT_STRING("chart_upper_house"), ui::piechart<upper_house_pie_chart>,
		CT_STRING("chart_voters_ideologies"), ui::piechart<voters_ideologies_pie_chart>,
		CT_STRING("chart_people_ideologies"), ui::piechart<people_ideologies_pie_chart>,
		CT_STRING("upperhouse_ideology_listbox"), ui::discrete_listbox<upperhouse_ideology_lb, upper_house_item, std::tuple<graphics::color_rgb, text_data::text_tag, float>>,
		CT_STRING("issue_listbox"), ui::discrete_listbox<issues_lb, issues_item, std::tuple<text_data::text_tag, float, float>>,
		CT_STRING("unciv_overlay"), ui::simple_button<unciv_overlay>,
		CT_STRING("decision_window"), decision_pane,
		CT_STRING("movements_window"), movements_pane,
		CT_STRING("reforms_window"), civilized_reforms_pane,
		CT_STRING("unciv_reforms_window"), uncivilized_reforms_pane,
		CT_STRING("release_nation"), release_nations_pane,
		government_window_base
	> {};

	template<typename W>
	void government_window_base::on_create(W & w, world_state &ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 671i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 38i16 };
		});

		ui::hide(*associated_object);
	}

	template<typename W>
	void decision_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 612i16, 560i16 };
		associated_object->position += ui::xy_pair{ 0i16, 34i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -34i16 };
		});
	}

	template<typename W>
	void movements_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 613i16, 560i16 };
		associated_object->position += ui::xy_pair{ 0i16, 34i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -34i16 };
		});
	}

	template<typename W>
	void release_nations_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 616i16, 560i16 };
		associated_object->position += ui::xy_pair{ 0i16, 34i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, -34i16 };
		});
	}

	template<typename W>
	void civilized_reforms_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 616i16, 568i16 };
		associated_object->position += ui::xy_pair{ 381i16, 46i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -381i16, -46i16 };
		});
	}

	template<typename W>
	void civilized_reforms_pane_base::windowed_update(W & w, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(ws.w.nation_s.nations.get<nation::is_civilized>(player) == false) {
				ui::hide(*associated_object);
			}
		}
	}

	template<typename W>
	void uncivilized_reforms_pane_base::on_create(W& w, world_state& ws) {
		associated_object->size = ui::xy_pair{ 616i16, 568i16 };
		associated_object->position += ui::xy_pair{ 381i16, 46i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -381i16, -46i16 };
		});
	}

	template<typename W>
	void uncivilized_reforms_pane_base::windowed_update(W & w, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(ws.w.nation_s.nations.get<nation::is_civilized>(player)) {
				ui::hide(*associated_object);
			}
		}
	}

	template <typename lb_type>
	void modifier_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto static_mod_range = get_range(ws.w.nation_s.static_modifier_arrays, ws.w.nation_s.nations.get<nation::static_modifiers>(player));
			for(auto s = static_mod_range.first; s != static_mod_range.second; ++s) {
				if(auto m = *s; is_valid_index(m))
					lb.add_item(ws, ws.s.modifiers_m.national_modifiers[m].icon, *s, date_tag());
			}
			auto timed_mod_range = get_range(ws.w.nation_s.timed_modifier_arrays, ws.w.nation_s.nations.get<nation::timed_modifiers>(player));
			for(auto s = timed_mod_range.first; s != timed_mod_range.second; ++s) {
				if(auto m = s->mod; is_valid_index(m))
					lb.add_item(ws, ws.s.modifiers_m.national_modifiers[m].icon, m, s->expiration);
			}
		}
	}

	template<typename W>
	void upperhouse_ideology_icon::windowed_update(ui::tinted_icon<upperhouse_ideology_icon>& self, W & w, world_state & ws) {
		self.set_color(ws.w.gui_m, float(w.color.r) / 255.0f, float(w.color.g) / 255.0f, float(w.color.b) / 255.0f);
	}

	template<typename window_type>
	void upperhouse_ideology_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager &lm, ui::text_format &fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, win.legend_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void upperhouse_ideology_percentage::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::percent, win.percentage);

		ui::text_chunk_to_instances( ws.s.gui_m, ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box, ui::xy_pair{ 0,0 }, fmt, lm);

		lm.finish_current_line();
	}

	template<typename lb_type>
	void upperhouse_ideology_lb::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<std::tuple<graphics::color_rgb, text_data::text_tag, float>, 32> data;

		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto upper_house = ws.w.nation_s.upper_house.get_row(id);
				for(uint32_t i = 0; i < ws.s.ideologies_m.ideologies_count; ++i) {
					ideologies::ideology_tag this_tag(static_cast<ideologies::ideology_tag::value_base_t>(i));
					if(upper_house[this_tag] != 0) {
						data.emplace_back(
							ws.s.ideologies_m.ideology_container[this_tag].color,
							ws.s.ideologies_m.ideology_container[this_tag].name,
							float(upper_house[this_tag]) / 100.0f);
					}
				}
			}
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename lb_type>
	void issues_lb::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<std::tuple<text_data::text_tag, float, float>, 32, concurrent_allocator<std::tuple<text_data::text_tag, float, float>>> data;

		if(auto player = ws.w.local_player_nation; player) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto iss = &(ws.w.nation_s.nation_demographics.get_row(id)[population::to_demo_tag(ws, issues::option_tag(0))]);
				float total_pop = float(ws.w.nation_s.nation_demographics.get(id, population::total_population_tag));

				if(total_pop != 0) {
					for(uint32_t i = 0; i < ws.s.issues_m.tracked_options_count; ++i) {
						issues::option_tag this_tag(static_cast<issues::option_tag::value_base_t>(i));
						if(iss[i] != 0) {
							data.emplace_back(
								ws.s.issues_m.options[this_tag].name,
								iss[i] / total_pop,
								0.0f);
						}
					}
				}
			}
		}

		lb.new_list(data.begin().get_ptr(), data.end().get_ptr());
	}

	template<typename window_type>
	void issues_item_popular_percentage::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::percent, win.people_percentage);

		ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box, ui::xy_pair{ 0,0 }, fmt, lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void issues_voter_percentage::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		char16_t formatted_value[64];
		put_value_in_buffer(formatted_value, display_type::percent, win.voters_percent);

		ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m,
			vector_backed_string<char16_t>(formatted_value),
			box, ui::xy_pair{ 0,0 }, fmt, lm);

		lm.finish_current_line();
	}

	template<typename window_type>
	void issues_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager &lm, ui::text_format &fmt, world_state & ws) {
		ui::add_linear_text(ui::xy_pair{ 0,0 }, win.legend_name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void unciv_overlay::windowed_update(ui::simple_button<unciv_overlay>& self, window_type &, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			if(ws.w.nation_s.nations.get<nation::is_civilized>(player))
				ui::hide(*self.associated_object);
			else
				ui::make_visible_immediate(*self.associated_object);
		}
	}

	template<typename window_type>
	void decision_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.event_m.decision_container[win.tag].title, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void decision_item_description::windowed_update(window_type & win, ui::tagged_gui_object box, ui::line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.event_m.decision_container[win.tag].body, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void decision_item_image::windowed_update(ui::dynamic_icon<decision_item_image>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag)) {
			graphics::texture_tag picture = ws.s.event_m.decision_container[win.tag].picture;
			auto gi = ws.w.gui_m.graphics_instances.safe_at(ui::graphics_instance_tag(self.associated_object->type_dependant_handle.load(std::memory_order_acquire)));

			if(is_valid_index(picture)) {
				if(gi)
					gi->t = &(ws.s.gui_m.textures.retrieve_by_key(picture));
			} else {
				if(gi)
					gi->t = default_image;
			}
		}
	}

	template<typename window_type>
	void decision_item_requirements::windowed_update(ui::dynamic_icon<decision_item_requirements>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag))
			tag = ws.s.event_m.decision_container[win.tag].effect;
	}

	template<typename window_type>
	void enact_decision_button::windowed_update(ui::simple_button<enact_decision_button>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag)) {
			tag = ws.s.event_m.decision_container[win.tag].effect;
			requirements = ws.s.event_m.decision_container[win.tag].allow;
			auto player = ws.w.local_player_nation;
			if(!is_valid_index(requirements) || triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(requirements), ws, player, player, nullptr, nullptr))
				self.set_enabled(true);
			else
				self.set_enabled(false);
		}
	}

	template<typename window_type>
	void decision_item_checkbox::windowed_update(ui::simple_button<decision_item_checkbox>& self, window_type & w, world_state & ws) {}

	template<typename lb_type>
	void decision_listbox::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<events::decision_tag, 32, concurrent_allocator<events::decision_tag>> possible_list;

		if(auto player = ws.w.local_player_nation; player) {
			for(auto& d : ws.s.event_m.decision_container) {
				if(!is_valid_index(d.potential) || triggers::test_trigger(ws.s.trigger_m.trigger_data.data() + to_index(d.potential), ws, player, player, nullptr, nullptr)) {
					possible_list.push_back(d.id);
				}
			}
		}

		lb.update_list(possible_list.begin().get_ptr(), possible_list.end().get_ptr());
	}

	template<typename window_type>
	void movement_ind_flag::windowed_update(ui::masked_flag<movement_ind_flag>& self, window_type & w, world_state & ws) {
		if(is_valid_index(w.tag)) {
			auto independance_nation = ws.w.population_s.pop_movements[w.tag].liberation_country;
			if(is_valid_index(independance_nation)) {
				self.set_displayed_flag(ws, independance_nation);
				ui::make_visible_immediate(*self.associated_object);
			} else {
				ui::hide(*self.associated_object);
			}
		}
	}

	template<typename window_type>
	void movements_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	template<typename window_type>
	void movements_item_nationalist_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	template<typename window_type>
	void movements_item_size::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::integer, ws.w.population_s.pop_movements[win.tag].total_population_support);

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void movements_item_radicalism::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			char16_t local_buf[16];
			put_value_in_buffer(local_buf, display_type::fp_one_place, ws.w.population_s.pop_movements[win.tag].radicalism);

			ui::text_chunk_to_instances(
				ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buf),
				box, ui::xy_pair{ 0,0 }, fmt, lm
			);

			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void movements_item_suppress_button::windowed_update(ui::simple_button<movement_sortby_name_button>&, window_type & win, world_state & ws) {
		tag = win.tag;
	}

	template<typename window_type>
	void rebels_item_type_icon::windowed_update(ui::dynamic_icon<rebels_item_type_icon>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag)) {
			auto icon = ws.w.population_s.rebel_factions[win.tag].icon;
			if(icon != 0)
				self.set_frame(ws.w.gui_m, uint32_t(icon) - 1ui32);
			else
				self.set_frame(ws.w.gui_m, 4ui32);
		}
	}

	template<typename window_type>
	void rebels_item_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			if(auto type = ws.w.population_s.rebel_factions[win.tag].type; is_valid_index(type)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.population_m.rebel_types[type].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
				lm.finish_current_line();
			}
		}
	}

	template<typename window_type>
	void rebels_item_size::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_brigades_ready::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_brigades_active::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_organization::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename window_type>
	void rebels_item_revolt_risk::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		ui::text_chunk_to_instances(
			ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"0"),
			box, ui::xy_pair{ 0,0 }, fmt, lm
		);
		lm.finish_current_line();
	}

	template<typename lb_type>
	void movements_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto movement_range = get_range(ws.w.population_s.pop_movement_arrays, ws.w.nation_s.nations.get<nation::active_movements>(player));
			lb.update_list(std::begin(movement_range), std::end(movement_range));
		}
	}

	template<typename lb_type>
	void rebels_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			auto rebel_range = get_range(ws.w.population_s.rebel_faction_arrays, ws.w.nation_s.nations.get<nation::active_rebel_factions>(player));
			lb.update_list(std::begin(rebel_range), std::end(rebel_range));
		}
	}

	template<typename window_type>
	void reform_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.for_issue)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.issues_m.issues_container[win.for_issue].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void unselected_option_button::windowed_update(ui::simple_button<unselected_option_button>& self, window_type & win, world_state & ws) {
		tag = win.tag;
		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(tag)) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto& iss_def = ws.s.issues_m.issues_container[ws.s.issues_m.options[tag].parent_issue];
				auto current_option = ws.w.nation_s.active_issue_options.get(id, iss_def.id);

				if(tag == current_option)
					ui::hide(*self.associated_object);
				else
					ui::make_visible_immediate(*self.associated_object);

				self.set_enabled(true);
				if(iss_def.next_step_only) {

					if(to_index(tag) + 1 != to_index(current_option) && to_index(current_option) + 1 != to_index(tag))
						self.set_enabled(false);
				}
				// todo: disable if lacking support
			}
		}
	}

	template<typename window_type>
	void option_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.issues_m.options[win.tag].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}

	template<typename window_type>
	void selected_option::windowed_update(ui::dynamic_icon<selected_option>& self, window_type & win, world_state & ws) {
		tag = win.tag;

		if(auto player = ws.w.local_player_nation; bool(player) && is_valid_index(win.tag)) {
			if(auto id = player; ws.w.nation_s.nations.is_valid_index(id)) {
				auto& iss_def = ws.s.issues_m.issues_container[ws.s.issues_m.options[win.tag].parent_issue];
				auto current_option = ws.w.nation_s.active_issue_options.get(id, iss_def.id);

				if(win.tag == current_option)
					ui::make_visible_immediate(*self.associated_object);
				else
					ui::hide(*self.associated_object);
			}
		}
	}

	template<typename lb_type>
	void reforms_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(is_valid_index(for_issue)) {
			auto& iss = ws.s.issues_m.issues_container[for_issue];
			uint32_t last = uint32_t(std::extent_v<decltype(iss.options)>);
			for(uint32_t i = 0; i < std::extent_v<decltype(iss.options)>; ++i) {
				if(!is_valid_index(iss.options[i])) {
					last = i;
					break;
				}
			}
			lb.new_list(&iss.options[0], &iss.options[last]);
		}
	}

	template<typename W>
	void reform_item_base::on_create(W & w, world_state & ws) {
		auto& selected = w.template get<CT_STRING("selected")>();
		ui::xy_pair selected_xy = ui::xy_pair{ -selected.associated_object->position.x, -selected.associated_object->position.y };

		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *w.associated_object, w.window_object }, [selected_xy](ui::tagged_gui_object o) {
			o.object.position += selected_xy;
		});
	}


	template<typename W>
	void reform_window_base::on_create(W & w, world_state & ws) {
		const auto new_obj = ws.w.gui_m.gui_objects.emplace();
		auto& iss = ws.s.issues_m.issues_container[for_issue];

		uint32_t last = uint32_t(std::extent_v<decltype(iss.options)>);
		for(uint32_t i = 0; i < std::extent_v<decltype(iss.options)>; ++i) {
			if(!is_valid_index(iss.options[i])) {
				last = i;
				break;
			}
		}

		auto item_tag = reform_options.element_tag(ws.s.gui_m);
		auto& window_d = ws.s.gui_m.ui_definitions.windows[item_tag];

		new_obj.object.position = ui::xy_pair{ int16_t(lb_x_offset), int16_t(lb_y_offset) };
		new_obj.object.size = ui::xy_pair{ 175i16, int16_t(window_d.size.y * (last) + 1) };

		w.associated_object->size = ui::xy_pair{ int16_t(lb_x_offset + 175), int16_t(lb_y_offset + window_d.size.y * (last) + 1) };

		new_obj.object.associated_behavior = &reform_options;
		reform_options.associated_object = &new_obj.object;

		reform_options.for_issue = for_issue;
		reform_options.create_sub_elements(new_obj, ws);

		ui::add_to_back(ws.w.gui_m, ui::tagged_gui_object{ *w.associated_object, w.window_object }, new_obj);
		ws.w.gui_m.flag_minimal_update();
	}

	template<typename window_type>
	void release_nation_flag::windowed_update(ui::masked_flag<release_nation_flag>& self, window_type & w, world_state & ws) {
		self.set_displayed_flag(ws, w.tag);
	}
	template<typename window_type>
	void release_nation_button::windowed_update(ui::simple_button<release_nation_button>&, window_type & win, world_state & ws) {}
	template<typename window_type>
	void release_nation_name::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(is_valid_index(win.tag)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.culture_m.national_tags[win.tag].default_name.name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void release_nation_type::windowed_update(window_type & win, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
	template<typename window_type>
	void release_nation_description::windowed_update(window_type & win, ui::tagged_gui_object box, ui::line_manager & lm, ui::text_format & fmt, world_state & ws) {}

	template<typename lb_type>
	void release_nations_listbox::populate_list(lb_type & lb, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			boost::container::small_vector<cultures::national_tag, 32, concurrent_allocator<cultures::national_tag>> data;

			auto owned_range = get_range(ws.w.province_s.province_arrays, ws.w.nation_s.nations.get<nation::owned_provinces>(player));
			auto ptag = ws.w.nation_s.nations.get<nation::tag>(player);
			for(auto p : owned_range) {
				if(is_valid_index(p)) {
					if(!contains_item(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(p), ptag)) {
						auto core_range = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container.get<province_state::cores>(p));
						for(auto c : core_range) {
							if(is_valid_index(c) && (!ws.w.culture_s.national_tags_state[c].is_not_releasable) && std::find(data.begin(), data.end(), c) == data.end()) {
								data.push_back(c);
							}
						}
					}
				}
			}

			lb.update_list(data.begin().get_ptr(), data.end().get_ptr());
		}
	}
}
