#pragma warning( push )
#pragma warning( disable : 4189 )

class good_filter_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, good_filter_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, good_filter_item& b);
class gp_investment_subwindow;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, gp_investment_subwindow& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, gp_investment_subwindow& b);
class state_pop_display_window;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, state_pop_display_window& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, state_pop_display_window& b);
class factory_display;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_display& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_display& b);
class goods_category_label;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, goods_category_label& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, goods_category_label& b);
class production_info;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, production_info& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, production_info& b);
class production_info_pane;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, production_info_pane& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, production_info_pane& b);
class investment_country_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, investment_country_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, investment_country_item& b);
class factory_filter_buttons;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_filter_buttons& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_filter_buttons& b);
class investment_filter_buttons;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, investment_filter_buttons& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, investment_filter_buttons& b);
class investment_pane;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, investment_pane& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, investment_pane& b);
class state_window;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, state_window& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, state_window& b);
class project_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, project_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, project_item& b);
class production_window_t;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, production_window_t& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, production_window_t& b);
class factory_worker_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_worker_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_worker_item& b);
class factory_construction_cost_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_construction_cost_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_construction_cost_item& b);
class factory_type_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_type_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_type_item& b);
class build_factory_window_t;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, build_factory_window_t& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, build_factory_window_t& b);

class production_window_base : public ui::draggable_region {
public:

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class close_button_s {
public:

	 void button_function(ui::simple_button<close_button_s>& self, world_state& ws);
};
class production_tab_button_group_b {
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
	 void button_function(ui::simple_button<filter_all_button>& self, world_state& ws);
};
class filter_north_america_button {
public:

	 void update(ui::simple_button<filter_north_america_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_north_america_button>& self, world_state& ws);
};
class filter_south_america_button {
public:

	 void update(ui::simple_button<filter_south_america_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_south_america_button>& self, world_state& ws);
};
class filter_europe_button {
public:

	 void update(ui::simple_button<filter_europe_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_europe_button>& self, world_state& ws);
};
class filter_africa_button {
public:

	 void update(ui::simple_button<filter_africa_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_africa_button>& self, world_state& ws);
};
class filter_asia_button {
public:

	 void update(ui::simple_button<filter_asia_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_asia_button>& self, world_state& ws);
};
class filter_oceania_button {
public:

	 void update(ui::simple_button<filter_oceania_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_oceania_button>& self, world_state& ws);
};
class filter_neighbours_button {
public:

	 void update(ui::simple_button<filter_neighbours_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_neighbours_button>& self, world_state& ws);
};
class filter_sphere_button {
public:

	 void update(ui::simple_button<filter_sphere_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_sphere_button>& self, world_state& ws);
};
class filter_enemies_button {
public:

	 void update(ui::simple_button<filter_enemies_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_enemies_button>& self, world_state& ws);
};
class filter_allies_button {
public:

	 void update(ui::simple_button<filter_allies_button>& self, world_state& ws);
	 void button_function(ui::simple_button<filter_allies_button>& self, world_state& ws);
};
class clear_text_button {
public:

	 void on_create(ui::simple_button<clear_text_button>& self, world_state& ws);
};
class sort_by_country_button {
public:

	 void button_function(ui::simple_button<sort_by_country_button>& self, world_state& ws);
};
class sort_by_my_flag_button {
public:

	 void update(ui::masked_flag<sort_by_my_flag_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_my_flag_button>& self, world_state& ws);
};
class sort_by_gpflag0_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag0_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag0_button>& self, world_state& ws);
};
class sort_by_gpflag1_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag1_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag1_button>& self, world_state& ws);
};
class sort_by_gpflag2_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag2_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag2_button>& self, world_state& ws);
};
class sort_by_gpflag3_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag3_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag3_button>& self, world_state& ws);
};
class sort_by_gpflag4_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag4_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag4_button>& self, world_state& ws);
};
class sort_by_gpflag5_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag5_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag5_button>& self, world_state& ws);
};
class sort_by_gpflag6_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag6_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag6_button>& self, world_state& ws);
};
class sort_by_gpflag7_button {
public:

	 void update(ui::masked_flag<sort_by_gpflag7_button>& self, world_state& ws);
	 void button_function(ui::masked_flag<sort_by_gpflag7_button>& self, world_state& ws);
};
class sort_by_boss_button {
public:

	 void button_function(ui::simple_button<sort_by_boss_button>& self, world_state& ws);
};
class sort_by_prestige_button {
public:

	 void button_function(ui::simple_button<sort_by_prestige_button>& self, world_state& ws);
};
class sort_by_economic_button {
public:

	 void button_function(ui::simple_button<sort_by_economic_button>& self, world_state& ws);
};
class sort_by_military_button {
public:

	 void button_function(ui::simple_button<sort_by_military_button>& self, world_state& ws);
};
class sort_by_total_button {
public:

	 void button_function(ui::simple_button<sort_by_total_button>& self, world_state& ws);
};
class sort_by_opinion_button {
public:

	 void button_function(ui::simple_button<sort_by_opinion_button>& self, world_state& ws);
};
class sort_by_relation_button {
public:

	 void button_function(ui::simple_button<sort_by_relation_button>& self, world_state& ws);
};
class sort_by_invest_factories_button {
public:

	 void button_function(ui::simple_button<sort_by_invest_factories_button>& self, world_state& ws);
};
class sort_by_project_state_button {
public:

	 void button_function(ui::simple_button<sort_by_project_state_button>& self, world_state& ws);
};
class sort_by_project_type_button {
public:

	 void button_function(ui::simple_button<sort_by_project_type_button>& self, world_state& ws);
};
class sort_by_project_completion_button {
public:

	 void button_function(ui::simple_button<sort_by_project_completion_button>& self, world_state& ws);
};
class sort_by_project_investors_button {
public:

	 void button_function(ui::simple_button<sort_by_project_investors_button>& self, world_state& ws);
};
class investment_country_item_base : public ui::visible_region {
public:
	 nations::country_tag tag;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
	 void set_value(nations::country_tag t) {
		 tag = t;
	 }
};
class investment_country_item_background_button {
public:
	 nations::country_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<investment_country_item_background_button>& self, window_type& w, world_state& ws);
	 void button_function(ui::simple_button<investment_country_item_background_button>& self, world_state& ws);
};
class investment_country_item_flag {
public:

	 template<typename window_type>
	 void windowed_update(ui::masked_flag<investment_country_item_flag>& self, window_type& w, world_state& ws);
};
class investment_country_item_name {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_self_investment_text {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_sphere_leader_flag {
public:

	 template<typename window_type>
	 void windowed_update(ui::masked_flag<investment_country_item_sphere_leader_flag>& self, window_type& w, world_state& ws);
};
class investment_country_item_military_rank {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_industrial_rank {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_prestige_rank {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_overall_rank {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_opinion_type {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_relations_value {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_item_factories_count {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_country_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class subsidize_all_button {
public:

	 void update(ui::simple_button<subsidize_all_button>& self, world_state& ws);
	 void button_function(ui::simple_button<subsidize_all_button>& self, world_state& ws);
};
class unsubsidize_all_button {
public:

	 void update(ui::simple_button<unsubsidize_all_button>& self, world_state& ws);
	 void button_function(ui::simple_button<unsubsidize_all_button>& self, world_state& ws);
};
class open_all_factories_button {
public:

	 void update(ui::simple_button<open_all_factories_button>& self, world_state& ws);
	 void button_function(ui::simple_button<open_all_factories_button>& self, world_state& ws);
};
class close_all_factories_button {
public:

	 void update(ui::simple_button<close_all_factories_button>& self, world_state& ws);
	 void button_function(ui::simple_button<close_all_factories_button>& self, world_state& ws);
};
class select_all_factories_filters_button {
public:

	 void button_function(ui::simple_button<select_all_factories_filters_button>& self, world_state& ws);
};
class deselect_all_factories_filters_button {
public:

	 void button_function(ui::simple_button<deselect_all_factories_filters_button>& self, world_state& ws);
};
class show_hide_empty_states_button {
public:

	 void update(ui::button<show_hide_empty_states_button>& self, world_state& ws);
	 void button_function(ui::button<show_hide_empty_states_button>& self, world_state& ws);
};
class sort_factories_by_state_name {
public:

	 void button_function(ui::simple_button<sort_factories_by_state_name>& self, world_state& ws);
};
class sort_factories_by_worker_a {
public:

	 void button_function(ui::button<sort_factories_by_worker_a>& self, world_state& ws);
};
class sort_factories_by_worker_b {
public:

	 void button_function(ui::button<sort_factories_by_worker_b>& self, world_state& ws);
};
class sort_factories_by_owner {
public:

	 void button_function(ui::button<sort_factories_by_owner>& self, world_state& ws);
};
class sort_factories_by_count {
public:

	 void button_function(ui::simple_button<sort_factories_by_count>& self, world_state& ws);
};
class sort_factories_by_infrastructure {
public:

	 void button_function(ui::simple_button<sort_factories_by_infrastructure>& self, world_state& ws);
};
class factory_filter_buttons_base : public ui::window_pane {
public:
	 ui::button<sort_factories_by_worker_a> workers_a;
	 ui::button<sort_factories_by_worker_b> workers_b;
	 ui::button<sort_factories_by_owner> owner;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class good_filter_item_base : public ui::visible_region {
public:
	 economy::goods_tag tag;

};
class good_filter_item_button {
public:
	 economy::goods_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<good_filter_item_button>& self, window_type& w, world_state& ws);
	 void button_function(ui::simple_button<good_filter_item_button>& self, world_state& ws);
};
class good_filter_item_enabled_bg {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<good_filter_item_enabled_bg>& self, window_type& w, world_state& ws);
};
class good_filter_item_type {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<good_filter_item_type>& self, window_type& w, world_state& ws);
};
class gp_investment_subwindow_base : public ui::visible_region {
public:
	 int32_t nth_nation = 0;

};
class gp_subwindow_flag {
public:

	 template<typename window_type>
	 void windowed_update(ui::masked_flag<gp_subwindow_flag>& self, window_type& w, world_state& ws);
};
class gp_subwindow_investment_value {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class state_pop_display_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<state_pop_display_icon>& self, window_type& w, world_state& ws);
};
class state_pop_display_amount {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class state_pop_display_window_base : public ui::visible_region {
public:
	 population::pop_type_tag type;
	 nations::state_tag in_state;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
template<typename = void>
class good_filter_item_internal_class : public good_filter_item_base {
public:
	 ui::simple_button<good_filter_item_button> filter_button;
	 ui::dynamic_icon<good_filter_item_enabled_bg> filter_enabled;
	 ui::dynamic_icon<good_filter_item_type> goods_type;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_button"),index > ,ui::simple_button<good_filter_item_button>&> get() {
		 return filter_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_button"),index > ,ui::simple_button<good_filter_item_button> const&> get() const {
		 return filter_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_enabled"),index > ,ui::dynamic_icon<good_filter_item_enabled_bg>&> get() {
		 return filter_enabled;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_enabled"),index > ,ui::dynamic_icon<good_filter_item_enabled_bg> const&> get() const {
		 return filter_enabled;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_type"),index > ,ui::dynamic_icon<good_filter_item_type>&> get() {
		 return goods_type;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_type"),index > ,ui::dynamic_icon<good_filter_item_type> const&> get() const {
		 return goods_type;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<good_filter_item_base, world_state&>) {
			 good_filter_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<good_filter_item_base, good_filter_item_internal_class&, world_state&>) {
			 good_filter_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<good_filter_item_base, world_state&>) 
			 good_filter_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<good_filter_item_button>, good_filter_item_internal_class&, world_state&>) 
			 filter_button.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<good_filter_item_enabled_bg>, good_filter_item_internal_class&, world_state&>) 
			 filter_enabled.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<good_filter_item_type>, good_filter_item_internal_class&, world_state&>) 
			 goods_type.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 good_filter_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("filter_button")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<good_filter_item_button>> vhelper(filter_button, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<good_filter_item_button>> vhelper(filter_button, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_enabled")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<good_filter_item_enabled_bg>> vhelper(filter_enabled, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<good_filter_item_enabled_bg>> vhelper(filter_enabled, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("goods_type")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<good_filter_item_type>> vhelper(goods_type, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<good_filter_item_type>> vhelper(goods_type, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<good_filter_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<good_filter_item_button>, good_filter_item_internal_class&, world_state&>)
			 filter_button.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<good_filter_item_enabled_bg>, good_filter_item_internal_class&, world_state&>)
			 filter_enabled.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<good_filter_item_type>, good_filter_item_internal_class&, world_state&>)
			 goods_type.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 good_filter_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<good_filter_item_base, world_state&>) {
			 good_filter_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<good_filter_item_base, good_filter_item_internal_class&, world_state&>) {
			 good_filter_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class good_filter_item : public good_filter_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, good_filter_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, good_filter_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class gp_investment_subwindow_internal_class : public gp_investment_subwindow_base {
public:
	 ui::masked_flag<gp_subwindow_flag> country_flag;
	 ui::display_text<gp_subwindow_investment_value> nongp_country_investment;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag"),index > ,ui::masked_flag<gp_subwindow_flag>&> get() {
		 return country_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag"),index > ,ui::masked_flag<gp_subwindow_flag> const&> get() const {
		 return country_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("nongp_country_investment"),index > ,ui::display_text<gp_subwindow_investment_value>&> get() {
		 return nongp_country_investment;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("nongp_country_investment"),index > ,ui::display_text<gp_subwindow_investment_value> const&> get() const {
		 return nongp_country_investment;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<gp_investment_subwindow_base, world_state&>) {
			 gp_investment_subwindow_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<gp_investment_subwindow_base, gp_investment_subwindow_internal_class&, world_state&>) {
			 gp_investment_subwindow_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<gp_investment_subwindow_base, world_state&>) 
			 gp_investment_subwindow_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<gp_subwindow_flag>, gp_investment_subwindow_internal_class&, world_state&>) 
			 country_flag.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_subwindow_investment_value>, gp_investment_subwindow_internal_class&, world_state&>) 
			 nongp_country_investment.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 gp_investment_subwindow_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("country_flag")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<gp_subwindow_flag>> vhelper(country_flag, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<gp_subwindow_flag>> vhelper(country_flag, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("nongp_country_investment")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_subwindow_investment_value>> vhelper(nongp_country_investment, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_subwindow_investment_value>> vhelper(nongp_country_investment, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<gp_investment_subwindow_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<gp_subwindow_flag>, gp_investment_subwindow_internal_class&, world_state&>)
			 country_flag.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_subwindow_investment_value>, gp_investment_subwindow_internal_class&, world_state&>)
			 nongp_country_investment.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 gp_investment_subwindow_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<gp_investment_subwindow_base, world_state&>) {
			 gp_investment_subwindow_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<gp_investment_subwindow_base, gp_investment_subwindow_internal_class&, world_state&>) {
			 gp_investment_subwindow_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class gp_investment_subwindow : public gp_investment_subwindow_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, gp_investment_subwindow& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, gp_investment_subwindow& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class state_pop_display_window_internal_class : public state_pop_display_window_base {
public:
	 ui::dynamic_icon<state_pop_display_icon> pop_icon;
	 ui::display_text<state_pop_display_amount, 4> pop_amount;
	 ui::display_text<empty_text_box> pop_amount_2;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_icon"),index > ,ui::dynamic_icon<state_pop_display_icon>&> get() {
		 return pop_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_icon"),index > ,ui::dynamic_icon<state_pop_display_icon> const&> get() const {
		 return pop_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount"),index > ,ui::display_text<state_pop_display_amount, 4>&> get() {
		 return pop_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount"),index > ,ui::display_text<state_pop_display_amount, 4> const&> get() const {
		 return pop_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount_2"),index > ,ui::display_text<empty_text_box>&> get() {
		 return pop_amount_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount_2"),index > ,ui::display_text<empty_text_box> const&> get() const {
		 return pop_amount_2;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<state_pop_display_window_base, world_state&>) {
			 state_pop_display_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<state_pop_display_window_base, state_pop_display_window_internal_class&, world_state&>) {
			 state_pop_display_window_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<state_pop_display_window_base, world_state&>) 
			 state_pop_display_window_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<state_pop_display_icon>, state_pop_display_window_internal_class&, world_state&>) 
			 pop_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<state_pop_display_amount, 4>, state_pop_display_window_internal_class&, world_state&>) 
			 pop_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<empty_text_box>, state_pop_display_window_internal_class&, world_state&>) 
			 pop_amount_2.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 state_pop_display_window_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("pop_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<state_pop_display_icon>> vhelper(pop_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<state_pop_display_icon>> vhelper(pop_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<state_pop_display_amount, 4>> vhelper(pop_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<state_pop_display_amount, 4>> vhelper(pop_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_amount_2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(pop_amount_2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(pop_amount_2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<state_pop_display_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<state_pop_display_icon>, state_pop_display_window_internal_class&, world_state&>)
			 pop_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<state_pop_display_amount, 4>, state_pop_display_window_internal_class&, world_state&>)
			 pop_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<empty_text_box>, state_pop_display_window_internal_class&, world_state&>)
			 pop_amount_2.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 state_pop_display_window_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<state_pop_display_window_base, world_state&>) {
			 state_pop_display_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<state_pop_display_window_base, state_pop_display_window_internal_class&, world_state&>) {
			 state_pop_display_window_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class state_pop_display_window : public state_pop_display_window_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, state_pop_display_window& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, state_pop_display_window& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
class goods_filters_base : public ui::window_pane {
public:
	 std::vector<good_filter_item> filter_buttons;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class investment_filter_buttons_base : public ui::window_pane {
public:
	 ui::button<sort_factories_by_worker_a> workers_a;
	 ui::button<sort_factories_by_worker_b> workers_b;
	 ui::button<sort_factories_by_owner> owner;
	 gp_investment_subwindow gp_windows[8];

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class investment_target_flag {
public:

	 void update(ui::masked_flag<investment_target_flag>& self, world_state& ws);
};
class investment_target_flag_overlay {
public:

	 void update(ui::dynamic_icon<investment_target_flag_overlay>& self, world_state& ws);
};
class investment_target_name {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_target_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class investment_target_back_button {
public:

	 void button_function(ui::simple_button<investment_target_back_button>& self, world_state& ws);
};
class factory_display_base : public ui::visible_region {
public:
	 nations::state_tag location;
	 int32_t index = -1;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class factory_open_background {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_open_background>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
};
class factory_closed_background {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_closed_background>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
};
class factory_construction_progress_bar {
public:

	 template<typename window_type>
	 void windowed_update(ui::progress_bar<factory_construction_progress_bar>& self, window_type& w, world_state& ws);
};
class factory_cancel_construction_button {
public:
	 nations::state_tag location;
	 int32_t index = -1;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<factory_cancel_construction_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<factory_cancel_construction_button>& self, world_state& ws);
};
class factory_upgrade_progress_bar {
public:

	 template<typename window_type>
	 void windowed_update(ui::progress_bar<factory_upgrade_progress_bar>& self, window_type& w, world_state& ws);
};
class factory_upgrade_progress_overlay {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_upgrade_progress_overlay>& self, window_type& w, world_state& ws);
};
class factory_level_text {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_upgrade_button {
public:
	 nations::state_tag location;
	 int32_t index = -1;

	 template<typename window_type>
	 void windowed_update(ui::button<factory_upgrade_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::button<factory_upgrade_button>& self, key_modifiers mod, world_state& ws);
};
class hidden_icon {
public:

	 void update(ui::dynamic_icon<hidden_icon>& self, world_state& ws);
};
class factory_income_arrow {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_income_arrow>& self, window_type& w, world_state& ws);
};
class factory_income_text {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_subsidize_button {
public:
	 nations::state_tag location;
	 int32_t index = -1;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<factory_subsidize_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<factory_subsidize_button>& self, world_state& ws);
};
class hidden_button {
public:

	 void update(ui::simple_button<hidden_button>& self, world_state& ws);
};
class factory_workers_display {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_workers_display>& self, window_type& w, world_state& ws);
};
class factory_closed_overlay {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_closed_overlay>& self, window_type& w, world_state& ws);
};
class factory_output {
public:

	 template<typename window_type>
	 void windowed_update(ui::simple_button<factory_output>& self, window_type& w, world_state& ws);
};
class factory_delete_button {
public:
	 nations::state_tag location;
	 int32_t index = -1;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<factory_delete_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<factory_delete_button>& self, world_state& ws);
};
class factory_open_or_close_button {
public:
	 nations::state_tag location;
	 int32_t index = -1;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<factory_open_or_close_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<factory_open_or_close_button>& self, world_state& ws);
};
class factory_closed_text_s {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class goods_category_label_base : public ui::visible_region {
public:
	 economy::goods_type_tag category;

};
class goods_category_name {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class production_info_base : public ui::window_pane {
public:
	 economy::goods_tag tag;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class production_backkground {
public:
	 economy::goods_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<production_backkground>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
};
class production_pop_icon {
public:
	 population::pop_type_tag type;

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<production_pop_icon>& self, window_type& w, world_state& ws);
};
class production_pop_output {
public:
	 population::pop_type_tag type;

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class production_input_icon {
public:
	 economy::goods_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<production_input_icon>& self, window_type& w, world_state& ws);
};
class production_total_output {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class production_output_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::simple_button<production_output_icon>& self, window_type& w, world_state& ws);
};
class production_no_production_overlay {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<production_no_production_overlay>& self, window_type& w, world_state& ws);
};
template<typename = void>
class factory_display_internal_class : public factory_display_base {
public:
	 ui::dynamic_icon<factory_open_background> prod_factory_bg;
	 ui::dynamic_icon<factory_closed_background> prod_factory_inprogress_bg;
	 ui::progress_bar<factory_construction_progress_bar> build_factory_progress;
	 ui::simple_button<factory_cancel_construction_button> prod_cancel_progress;
	 ui::progress_bar<factory_upgrade_progress_bar> upgrade_factory_progress;
	 ui::dynamic_icon<factory_upgrade_progress_overlay> progress_overlay_16_64;
	 ui::display_text<factory_level_text> level;
	 ui::button<factory_upgrade_button> upgrade;
	 ui::dynamic_icon<hidden_icon> input_0_lack2;
	 ui::dynamic_icon<hidden_icon> input_1_lack2;
	 ui::dynamic_icon<hidden_icon> input_2_lack2;
	 ui::dynamic_icon<hidden_icon> input_3_lack2;
	 ui::dynamic_icon<factory_input<0>> input_0;
	 ui::dynamic_icon<factory_input<1>> input_1;
	 ui::dynamic_icon<factory_input<2>> input_2;
	 ui::dynamic_icon<factory_input<3>> input_3;
	 ui::dynamic_icon<factory_income_arrow> income_icon;
	 ui::display_text<factory_income_text, -14> income;
	 ui::simple_button<factory_subsidize_button> subsidise;
	 ui::simple_button<hidden_button> priority;
	 ui::dynamic_icon<factory_workers_display> employment_ratio;
	 ui::dynamic_icon<factory_closed_overlay> closed_overlay;
	 ui::simple_button<factory_output> output;
	 ui::simple_button<factory_delete_button> delete_factory;
	 ui::simple_button<factory_open_or_close_button> open_close;
	 ui::display_text<factory_closed_text_s> factory_closed_text;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_factory_bg"),index > ,ui::dynamic_icon<factory_open_background>&> get() {
		 return prod_factory_bg;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_factory_bg"),index > ,ui::dynamic_icon<factory_open_background> const&> get() const {
		 return prod_factory_bg;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_factory_inprogress_bg"),index > ,ui::dynamic_icon<factory_closed_background>&> get() {
		 return prod_factory_inprogress_bg;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_factory_inprogress_bg"),index > ,ui::dynamic_icon<factory_closed_background> const&> get() const {
		 return prod_factory_inprogress_bg;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("build_factory_progress"),index > ,ui::progress_bar<factory_construction_progress_bar>&> get() {
		 return build_factory_progress;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("build_factory_progress"),index > ,ui::progress_bar<factory_construction_progress_bar> const&> get() const {
		 return build_factory_progress;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_cancel_progress"),index > ,ui::simple_button<factory_cancel_construction_button>&> get() {
		 return prod_cancel_progress;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_cancel_progress"),index > ,ui::simple_button<factory_cancel_construction_button> const&> get() const {
		 return prod_cancel_progress;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("upgrade_factory_progress"),index > ,ui::progress_bar<factory_upgrade_progress_bar>&> get() {
		 return upgrade_factory_progress;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("upgrade_factory_progress"),index > ,ui::progress_bar<factory_upgrade_progress_bar> const&> get() const {
		 return upgrade_factory_progress;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("progress_overlay_16_64"),index > ,ui::dynamic_icon<factory_upgrade_progress_overlay>&> get() {
		 return progress_overlay_16_64;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("progress_overlay_16_64"),index > ,ui::dynamic_icon<factory_upgrade_progress_overlay> const&> get() const {
		 return progress_overlay_16_64;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("level"),index > ,ui::display_text<factory_level_text>&> get() {
		 return level;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("level"),index > ,ui::display_text<factory_level_text> const&> get() const {
		 return level;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("upgrade"),index > ,ui::button<factory_upgrade_button>&> get() {
		 return upgrade;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("upgrade"),index > ,ui::button<factory_upgrade_button> const&> get() const {
		 return upgrade;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0_lack2"),index > ,ui::dynamic_icon<hidden_icon>&> get() {
		 return input_0_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0_lack2"),index > ,ui::dynamic_icon<hidden_icon> const&> get() const {
		 return input_0_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1_lack2"),index > ,ui::dynamic_icon<hidden_icon>&> get() {
		 return input_1_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1_lack2"),index > ,ui::dynamic_icon<hidden_icon> const&> get() const {
		 return input_1_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2_lack2"),index > ,ui::dynamic_icon<hidden_icon>&> get() {
		 return input_2_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2_lack2"),index > ,ui::dynamic_icon<hidden_icon> const&> get() const {
		 return input_2_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3_lack2"),index > ,ui::dynamic_icon<hidden_icon>&> get() {
		 return input_3_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3_lack2"),index > ,ui::dynamic_icon<hidden_icon> const&> get() const {
		 return input_3_lack2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0"),index > ,ui::dynamic_icon<factory_input<0>>&> get() {
		 return input_0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0"),index > ,ui::dynamic_icon<factory_input<0>> const&> get() const {
		 return input_0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1"),index > ,ui::dynamic_icon<factory_input<1>>&> get() {
		 return input_1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1"),index > ,ui::dynamic_icon<factory_input<1>> const&> get() const {
		 return input_1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2"),index > ,ui::dynamic_icon<factory_input<2>>&> get() {
		 return input_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2"),index > ,ui::dynamic_icon<factory_input<2>> const&> get() const {
		 return input_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3"),index > ,ui::dynamic_icon<factory_input<3>>&> get() {
		 return input_3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3"),index > ,ui::dynamic_icon<factory_input<3>> const&> get() const {
		 return input_3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("income_icon"),index > ,ui::dynamic_icon<factory_income_arrow>&> get() {
		 return income_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("income_icon"),index > ,ui::dynamic_icon<factory_income_arrow> const&> get() const {
		 return income_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("income"),index > ,ui::display_text<factory_income_text, -14>&> get() {
		 return income;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("income"),index > ,ui::display_text<factory_income_text, -14> const&> get() const {
		 return income;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("subsidise"),index > ,ui::simple_button<factory_subsidize_button>&> get() {
		 return subsidise;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("subsidise"),index > ,ui::simple_button<factory_subsidize_button> const&> get() const {
		 return subsidise;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("priority"),index > ,ui::simple_button<hidden_button>&> get() {
		 return priority;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("priority"),index > ,ui::simple_button<hidden_button> const&> get() const {
		 return priority;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("employment_ratio"),index > ,ui::dynamic_icon<factory_workers_display>&> get() {
		 return employment_ratio;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("employment_ratio"),index > ,ui::dynamic_icon<factory_workers_display> const&> get() const {
		 return employment_ratio;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("closed_overlay"),index > ,ui::dynamic_icon<factory_closed_overlay>&> get() {
		 return closed_overlay;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("closed_overlay"),index > ,ui::dynamic_icon<factory_closed_overlay> const&> get() const {
		 return closed_overlay;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::simple_button<factory_output>&> get() {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::simple_button<factory_output> const&> get() const {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("delete_factory"),index > ,ui::simple_button<factory_delete_button>&> get() {
		 return delete_factory;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("delete_factory"),index > ,ui::simple_button<factory_delete_button> const&> get() const {
		 return delete_factory;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("open_close"),index > ,ui::simple_button<factory_open_or_close_button>&> get() {
		 return open_close;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("open_close"),index > ,ui::simple_button<factory_open_or_close_button> const&> get() const {
		 return open_close;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_closed_text"),index > ,ui::display_text<factory_closed_text_s>&> get() {
		 return factory_closed_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_closed_text"),index > ,ui::display_text<factory_closed_text_s> const&> get() const {
		 return factory_closed_text;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<factory_display_base, world_state&>) {
			 factory_display_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_display_base, factory_display_internal_class&, world_state&>) {
			 factory_display_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<factory_display_base, world_state&>) 
			 factory_display_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_open_background>, factory_display_internal_class&, world_state&>) 
			 prod_factory_bg.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_closed_background>, factory_display_internal_class&, world_state&>) 
			 prod_factory_inprogress_bg.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::progress_bar<factory_construction_progress_bar>, factory_display_internal_class&, world_state&>) 
			 build_factory_progress.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<factory_cancel_construction_button>, factory_display_internal_class&, world_state&>) 
			 prod_cancel_progress.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::progress_bar<factory_upgrade_progress_bar>, factory_display_internal_class&, world_state&>) 
			 upgrade_factory_progress.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_upgrade_progress_overlay>, factory_display_internal_class&, world_state&>) 
			 progress_overlay_16_64.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_level_text>, factory_display_internal_class&, world_state&>) 
			 level.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button<factory_upgrade_button>, factory_display_internal_class&, world_state&>) 
			 upgrade.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>) 
			 input_0_lack2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>) 
			 input_1_lack2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>) 
			 input_2_lack2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>) 
			 input_3_lack2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_input<0>>, factory_display_internal_class&, world_state&>) 
			 input_0.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_input<1>>, factory_display_internal_class&, world_state&>) 
			 input_1.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_input<2>>, factory_display_internal_class&, world_state&>) 
			 input_2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_input<3>>, factory_display_internal_class&, world_state&>) 
			 input_3.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_income_arrow>, factory_display_internal_class&, world_state&>) 
			 income_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_income_text, -14>, factory_display_internal_class&, world_state&>) 
			 income.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<factory_subsidize_button>, factory_display_internal_class&, world_state&>) 
			 subsidise.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<hidden_button>, factory_display_internal_class&, world_state&>) 
			 priority.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_workers_display>, factory_display_internal_class&, world_state&>) 
			 employment_ratio.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_closed_overlay>, factory_display_internal_class&, world_state&>) 
			 closed_overlay.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<factory_output>, factory_display_internal_class&, world_state&>) 
			 output.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<factory_delete_button>, factory_display_internal_class&, world_state&>) 
			 delete_factory.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<factory_open_or_close_button>, factory_display_internal_class&, world_state&>) 
			 open_close.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_closed_text_s>, factory_display_internal_class&, world_state&>) 
			 factory_closed_text.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 factory_display_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("prod_factory_bg")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_open_background>> vhelper(prod_factory_bg, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_open_background>> vhelper(prod_factory_bg, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_factory_inprogress_bg")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_closed_background>> vhelper(prod_factory_inprogress_bg, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_closed_background>> vhelper(prod_factory_inprogress_bg, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("build_factory_progress")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::progress_bar<factory_construction_progress_bar>> vhelper(build_factory_progress, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::progress_bar<factory_construction_progress_bar>> vhelper(build_factory_progress, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_cancel_progress")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<factory_cancel_construction_button>> vhelper(prod_cancel_progress, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<factory_cancel_construction_button>> vhelper(prod_cancel_progress, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("upgrade_factory_progress")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::progress_bar<factory_upgrade_progress_bar>> vhelper(upgrade_factory_progress, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::progress_bar<factory_upgrade_progress_bar>> vhelper(upgrade_factory_progress, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("progress_overlay_16_64")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_upgrade_progress_overlay>> vhelper(progress_overlay_16_64, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_upgrade_progress_overlay>> vhelper(progress_overlay_16_64, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("level")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_level_text>> vhelper(level, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_level_text>> vhelper(level, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("upgrade")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button<factory_upgrade_button>> vhelper(upgrade, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button<factory_upgrade_button>> vhelper(upgrade, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_0_lack2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_0_lack2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_0_lack2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_1_lack2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_1_lack2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_1_lack2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_2_lack2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_2_lack2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_2_lack2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_3_lack2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_3_lack2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<hidden_icon>> vhelper(input_3_lack2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_0")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<0>>> vhelper(input_0, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<0>>> vhelper(input_0, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_1")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<1>>> vhelper(input_1, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<1>>> vhelper(input_1, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<2>>> vhelper(input_2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<2>>> vhelper(input_2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_3")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<3>>> vhelper(input_3, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_input<3>>> vhelper(input_3, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("income_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_income_arrow>> vhelper(income_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_income_arrow>> vhelper(income_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("income")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_income_text, -14>> vhelper(income, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_income_text, -14>> vhelper(income, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("subsidise")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<factory_subsidize_button>> vhelper(subsidise, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<factory_subsidize_button>> vhelper(subsidise, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("priority")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<hidden_button>> vhelper(priority, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<hidden_button>> vhelper(priority, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("employment_ratio")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_workers_display>> vhelper(employment_ratio, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_workers_display>> vhelper(employment_ratio, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("closed_overlay")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_closed_overlay>> vhelper(closed_overlay, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_closed_overlay>> vhelper(closed_overlay, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<factory_output>> vhelper(output, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<factory_output>> vhelper(output, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("delete_factory")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<factory_delete_button>> vhelper(delete_factory, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<factory_delete_button>> vhelper(delete_factory, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("open_close")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<factory_open_or_close_button>> vhelper(open_close, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<factory_open_or_close_button>> vhelper(open_close, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("factory_closed_text")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_closed_text_s>> vhelper(factory_closed_text, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_closed_text_s>> vhelper(factory_closed_text, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<factory_display_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_open_background>, factory_display_internal_class&, world_state&>)
			 prod_factory_bg.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_closed_background>, factory_display_internal_class&, world_state&>)
			 prod_factory_inprogress_bg.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::progress_bar<factory_construction_progress_bar>, factory_display_internal_class&, world_state&>)
			 build_factory_progress.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<factory_cancel_construction_button>, factory_display_internal_class&, world_state&>)
			 prod_cancel_progress.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::progress_bar<factory_upgrade_progress_bar>, factory_display_internal_class&, world_state&>)
			 upgrade_factory_progress.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_upgrade_progress_overlay>, factory_display_internal_class&, world_state&>)
			 progress_overlay_16_64.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_level_text>, factory_display_internal_class&, world_state&>)
			 level.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button<factory_upgrade_button>, factory_display_internal_class&, world_state&>)
			 upgrade.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>)
			 input_0_lack2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>)
			 input_1_lack2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>)
			 input_2_lack2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<hidden_icon>, factory_display_internal_class&, world_state&>)
			 input_3_lack2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_input<0>>, factory_display_internal_class&, world_state&>)
			 input_0.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_input<1>>, factory_display_internal_class&, world_state&>)
			 input_1.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_input<2>>, factory_display_internal_class&, world_state&>)
			 input_2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_input<3>>, factory_display_internal_class&, world_state&>)
			 input_3.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_income_arrow>, factory_display_internal_class&, world_state&>)
			 income_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_income_text, -14>, factory_display_internal_class&, world_state&>)
			 income.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<factory_subsidize_button>, factory_display_internal_class&, world_state&>)
			 subsidise.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<hidden_button>, factory_display_internal_class&, world_state&>)
			 priority.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_workers_display>, factory_display_internal_class&, world_state&>)
			 employment_ratio.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_closed_overlay>, factory_display_internal_class&, world_state&>)
			 closed_overlay.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<factory_output>, factory_display_internal_class&, world_state&>)
			 output.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<factory_delete_button>, factory_display_internal_class&, world_state&>)
			 delete_factory.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<factory_open_or_close_button>, factory_display_internal_class&, world_state&>)
			 open_close.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_closed_text_s>, factory_display_internal_class&, world_state&>)
			 factory_closed_text.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 factory_display_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<factory_display_base, world_state&>) {
			 factory_display_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_display_base, factory_display_internal_class&, world_state&>) {
			 factory_display_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class factory_display : public factory_display_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_display& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_display& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class goods_category_label_internal_class : public goods_category_label_base {
public:
	 ui::display_text<goods_category_name> cat_name;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("cat_name"),index > ,ui::display_text<goods_category_name>&> get() {
		 return cat_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("cat_name"),index > ,ui::display_text<goods_category_name> const&> get() const {
		 return cat_name;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<goods_category_label_base, world_state&>) {
			 goods_category_label_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<goods_category_label_base, goods_category_label_internal_class&, world_state&>) {
			 goods_category_label_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<goods_category_label_base, world_state&>) 
			 goods_category_label_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<goods_category_name>, goods_category_label_internal_class&, world_state&>) 
			 cat_name.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 goods_category_label_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("cat_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<goods_category_name>> vhelper(cat_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<goods_category_name>> vhelper(cat_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<goods_category_label_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<goods_category_name>, goods_category_label_internal_class&, world_state&>)
			 cat_name.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 goods_category_label_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<goods_category_label_base, world_state&>) {
			 goods_category_label_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<goods_category_label_base, goods_category_label_internal_class&, world_state&>) {
			 goods_category_label_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class goods_category_label : public goods_category_label_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, goods_category_label& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, goods_category_label& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class production_info_internal_class : public production_info_base {
public:
	 ui::dynamic_icon<production_backkground> prod_producing_entry;
	 ui::dynamic_icon<production_pop_icon> pop_factory;
	 ui::dynamic_icon<production_pop_icon> pop_factory2;
	 ui::display_text<production_pop_output> output;
	 ui::display_text<production_pop_output> output2;
	 ui::dynamic_icon<production_input_icon> input_factory_0;
	 ui::dynamic_icon<production_input_icon> input_factory_1;
	 ui::dynamic_icon<production_input_icon> input_factory_2;
	 ui::dynamic_icon<production_input_icon> input_factory_3;
	 ui::display_text<production_total_output> output_total;
	 ui::dynamic_icon<production_no_production_overlay> prod_producing_not_total;
	 ui::simple_button<production_output_icon> output_factory;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_producing_entry"),index > ,ui::dynamic_icon<production_backkground>&> get() {
		 return prod_producing_entry;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_producing_entry"),index > ,ui::dynamic_icon<production_backkground> const&> get() const {
		 return prod_producing_entry;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_factory"),index > ,ui::dynamic_icon<production_pop_icon>&> get() {
		 return pop_factory;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_factory"),index > ,ui::dynamic_icon<production_pop_icon> const&> get() const {
		 return pop_factory;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_factory2"),index > ,ui::dynamic_icon<production_pop_icon>&> get() {
		 return pop_factory2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_factory2"),index > ,ui::dynamic_icon<production_pop_icon> const&> get() const {
		 return pop_factory2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::display_text<production_pop_output>&> get() {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::display_text<production_pop_output> const&> get() const {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output2"),index > ,ui::display_text<production_pop_output>&> get() {
		 return output2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output2"),index > ,ui::display_text<production_pop_output> const&> get() const {
		 return output2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_0"),index > ,ui::dynamic_icon<production_input_icon>&> get() {
		 return input_factory_0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_0"),index > ,ui::dynamic_icon<production_input_icon> const&> get() const {
		 return input_factory_0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_1"),index > ,ui::dynamic_icon<production_input_icon>&> get() {
		 return input_factory_1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_1"),index > ,ui::dynamic_icon<production_input_icon> const&> get() const {
		 return input_factory_1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_2"),index > ,ui::dynamic_icon<production_input_icon>&> get() {
		 return input_factory_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_2"),index > ,ui::dynamic_icon<production_input_icon> const&> get() const {
		 return input_factory_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_3"),index > ,ui::dynamic_icon<production_input_icon>&> get() {
		 return input_factory_3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_factory_3"),index > ,ui::dynamic_icon<production_input_icon> const&> get() const {
		 return input_factory_3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_total"),index > ,ui::display_text<production_total_output>&> get() {
		 return output_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_total"),index > ,ui::display_text<production_total_output> const&> get() const {
		 return output_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_producing_not_total"),index > ,ui::dynamic_icon<production_no_production_overlay>&> get() {
		 return prod_producing_not_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_producing_not_total"),index > ,ui::dynamic_icon<production_no_production_overlay> const&> get() const {
		 return prod_producing_not_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_factory"),index > ,ui::simple_button<production_output_icon>&> get() {
		 return output_factory;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_factory"),index > ,ui::simple_button<production_output_icon> const&> get() const {
		 return output_factory;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<production_info_base, world_state&>) {
			 production_info_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<production_info_base, production_info_internal_class&, world_state&>) {
			 production_info_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<production_info_base, world_state&>) 
			 production_info_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_backkground>, production_info_internal_class&, world_state&>) 
			 prod_producing_entry.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_pop_icon>, production_info_internal_class&, world_state&>) 
			 pop_factory.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_pop_icon>, production_info_internal_class&, world_state&>) 
			 pop_factory2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<production_pop_output>, production_info_internal_class&, world_state&>) 
			 output.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<production_pop_output>, production_info_internal_class&, world_state&>) 
			 output2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>) 
			 input_factory_0.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>) 
			 input_factory_1.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>) 
			 input_factory_2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>) 
			 input_factory_3.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<production_total_output>, production_info_internal_class&, world_state&>) 
			 output_total.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<production_no_production_overlay>, production_info_internal_class&, world_state&>) 
			 prod_producing_not_total.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<production_output_icon>, production_info_internal_class&, world_state&>) 
			 output_factory.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 production_info_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("prod_producing_entry")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_backkground>> vhelper(prod_producing_entry, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_backkground>> vhelper(prod_producing_entry, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_factory")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_pop_icon>> vhelper(pop_factory, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_pop_icon>> vhelper(pop_factory, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_factory2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_pop_icon>> vhelper(pop_factory2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_pop_icon>> vhelper(pop_factory2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<production_pop_output>> vhelper(output, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<production_pop_output>> vhelper(output, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<production_pop_output>> vhelper(output2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<production_pop_output>> vhelper(output2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_factory_0")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_0, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_0, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_factory_1")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_1, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_1, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_factory_2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_factory_3")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_3, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_input_icon>> vhelper(input_factory_3, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output_total")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<production_total_output>> vhelper(output_total, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<production_total_output>> vhelper(output_total, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_producing_not_total")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<production_no_production_overlay>> vhelper(prod_producing_not_total, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<production_no_production_overlay>> vhelper(prod_producing_not_total, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output_factory")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<production_output_icon>> vhelper(output_factory, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<production_output_icon>> vhelper(output_factory, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<production_info_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_backkground>, production_info_internal_class&, world_state&>)
			 prod_producing_entry.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_pop_icon>, production_info_internal_class&, world_state&>)
			 pop_factory.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_pop_icon>, production_info_internal_class&, world_state&>)
			 pop_factory2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<production_pop_output>, production_info_internal_class&, world_state&>)
			 output.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<production_pop_output>, production_info_internal_class&, world_state&>)
			 output2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>)
			 input_factory_0.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>)
			 input_factory_1.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>)
			 input_factory_2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_input_icon>, production_info_internal_class&, world_state&>)
			 input_factory_3.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<production_total_output>, production_info_internal_class&, world_state&>)
			 output_total.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<production_no_production_overlay>, production_info_internal_class&, world_state&>)
			 prod_producing_not_total.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<production_output_icon>, production_info_internal_class&, world_state&>)
			 output_factory.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 production_info_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<production_info_base, world_state&>) {
			 production_info_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<production_info_base, production_info_internal_class&, world_state&>) {
			 production_info_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class production_info : public production_info_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, production_info& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, production_info& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
class state_window_base : public ui::window_pane {
public:
	 nations::state_tag tag;
	 state_pop_display_window workers_a;
	 state_pop_display_window workers_b;
	 state_pop_display_window owner;
	 factory_display factories[state::factories_count];

	 template<typename W>
	 void on_create(W& w, world_state& ws);
	 void set_value(nations::state_tag t);
	 void update(world_state& ws);
};
class state_focus_button {
public:
	 nations::state_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<state_focus_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<state_focus_button>& self, world_state& ws);
};
class state_name_s {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class state_factory_count {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class state_build_factory_button {
public:
	 nations::state_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<state_build_factory_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<state_build_factory_button>& self, world_state& ws);
};
class state_average_infrastructure {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class state_details_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class investment_state_details_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class production_info_pane_base : public ui::window_pane {
public:
	 std::vector<goods_category_label> categories;
	 std::vector<production_info> production_details;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
template<typename = void>
class production_info_pane_internal_class : public production_info_pane_base {
public:
	 ui::gui_object_tag window_object;


	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<production_info_pane_base, world_state&>) {
			 production_info_pane_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<production_info_pane_base, production_info_pane_internal_class&, world_state&>) {
			 production_info_pane_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<production_info_pane_base, world_state&>) 
			 production_info_pane_base::update(ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 production_info_pane_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
				 if(!ui::detail::can_create_dynamic_s<production_info_pane_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
		 }
		 window_object = window.id;
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 production_info_pane_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<production_info_pane_base, world_state&>) {
			 production_info_pane_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<production_info_pane_base, production_info_pane_internal_class&, world_state&>) {
			 production_info_pane_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class production_info_pane : public production_info_pane_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, production_info_pane& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, production_info_pane& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
class project_item_base : public ui::visible_region {
public:
	 nations::state_tag location;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
	 void set_value(nations::state_tag t) {
		 location = t;
	 }
};
class project_state_name {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class project_resource_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<project_resource_icon>& self, window_type& w, world_state& ws);
};
class project_infrastructure_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<project_infrastructure_icon>& self, window_type& w, world_state& ws);
};
class project_name_s {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class project_cost_s {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class project_investor_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<project_investor_icon>& self, window_type& w, world_state& ws);
};
class project_investor_amount {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class project_invest_button {
public:
	 nations::state_tag location;

	 template<typename window_type>
	 void windowed_update(ui::button<project_invest_button>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::button<project_invest_button>& self, key_modifiers mod, world_state& ws);
};
class projects_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class factory_worker_item_base : public ui::gui_behavior {
public:
	 factory_worker_value value;

	 void set_value(factory_worker_value t) {
		 value = t;
	 }
};
class factory_worker_item_icon {
public:
	 population::pop_type_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_worker_item_icon>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
};
class factory_worker_item_amount {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_construction_cost_item_base : public ui::gui_behavior {
public:
	 factory_construction_cost_value value;

	 void set_value(factory_construction_cost_value t) {
		 value = t;
	 }
};
class factory_construction_cost_item_icon {
public:
	 goods_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_construction_cost_item_icon>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
};
class factory_construction_cost_item_cost {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_construction_cost_item_stockpile_amount {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_type_item_base : public ui::gui_behavior {
public:
	 economy::goods_tag value;

	 void set_value(economy::goods_tag t) {
		 value = t;
	 }
};
class factory_item_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<factory_item_icon>& self, window_type& w, world_state& ws);
};
class factory_item_name {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_item_cost {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_item_time {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_item_bg {
public:
	 goods_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<factory_item_bg>& self, window_type& w, world_state& ws);
	 void button_function(ui::simple_button<factory_item_bg>& self, world_state& ws);
};
class build_factory_window_base : public ui::draggable_region {
public:

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class bf_state_name {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_output_icon {
public:

	 void update(ui::dynamic_icon<bf_output_icon>& self, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
};
class bf_factory_name {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_types_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class bf_profit_label {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_profit_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_description {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_total_workers_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class factory_workers_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class factory_construction_costs_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class bf_base_price_label {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_input_price_label {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_total_label {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_base_price_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_input_price_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_total_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_treasury_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_stockpile_lack_label {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_stockpile_lack_amount {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class bf_cancel {
public:

	 void button_function(ui::simple_button<bf_cancel>& self, world_state& ws);
};
class bf_build {
public:

	 void update(ui::simple_button<bf_build>& self, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<bf_build>& self, world_state& ws);
};
template<typename = void>
class investment_country_item_internal_class : public investment_country_item_base {
public:
	 ui::simple_button<investment_country_item_background_button> country_select;
	 ui::masked_flag<investment_country_item_flag> country_flag;
	 ui::display_text<investment_country_item_name> country_name;
	 ui::display_text<investment_country_item_self_investment_text> invest;
	 ui::display_text<gp_investment_text<0>> country_gp0;
	 ui::display_text<gp_investment_text<1>> country_gp1;
	 ui::display_text<gp_investment_text<2>> country_gp2;
	 ui::display_text<gp_investment_text<3>> country_gp3;
	 ui::display_text<gp_investment_text<4>> country_gp4;
	 ui::display_text<gp_investment_text<5>> country_gp5;
	 ui::display_text<gp_investment_text<6>> country_gp6;
	 ui::display_text<gp_investment_text<7>> country_gp7;
	 ui::masked_flag<investment_country_item_sphere_leader_flag> country_boss_flag;
	 ui::display_text<investment_country_item_prestige_rank> country_prestige;
	 ui::display_text<investment_country_item_industrial_rank> country_economic;
	 ui::display_text<investment_country_item_military_rank> country_military;
	 ui::display_text<investment_country_item_overall_rank> country_total;
	 ui::display_text<investment_country_item_opinion_type> country_opinion;
	 ui::display_text<investment_country_item_relations_value> country_relation;
	 ui::display_text<investment_country_item_factories_count> factories;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_select"),index > ,ui::simple_button<investment_country_item_background_button>&> get() {
		 return country_select;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_select"),index > ,ui::simple_button<investment_country_item_background_button> const&> get() const {
		 return country_select;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag"),index > ,ui::masked_flag<investment_country_item_flag>&> get() {
		 return country_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag"),index > ,ui::masked_flag<investment_country_item_flag> const&> get() const {
		 return country_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_name"),index > ,ui::display_text<investment_country_item_name>&> get() {
		 return country_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_name"),index > ,ui::display_text<investment_country_item_name> const&> get() const {
		 return country_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest"),index > ,ui::display_text<investment_country_item_self_investment_text>&> get() {
		 return invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest"),index > ,ui::display_text<investment_country_item_self_investment_text> const&> get() const {
		 return invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp0"),index > ,ui::display_text<gp_investment_text<0>>&> get() {
		 return country_gp0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp0"),index > ,ui::display_text<gp_investment_text<0>> const&> get() const {
		 return country_gp0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp1"),index > ,ui::display_text<gp_investment_text<1>>&> get() {
		 return country_gp1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp1"),index > ,ui::display_text<gp_investment_text<1>> const&> get() const {
		 return country_gp1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp2"),index > ,ui::display_text<gp_investment_text<2>>&> get() {
		 return country_gp2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp2"),index > ,ui::display_text<gp_investment_text<2>> const&> get() const {
		 return country_gp2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp3"),index > ,ui::display_text<gp_investment_text<3>>&> get() {
		 return country_gp3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp3"),index > ,ui::display_text<gp_investment_text<3>> const&> get() const {
		 return country_gp3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp4"),index > ,ui::display_text<gp_investment_text<4>>&> get() {
		 return country_gp4;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp4"),index > ,ui::display_text<gp_investment_text<4>> const&> get() const {
		 return country_gp4;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp5"),index > ,ui::display_text<gp_investment_text<5>>&> get() {
		 return country_gp5;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp5"),index > ,ui::display_text<gp_investment_text<5>> const&> get() const {
		 return country_gp5;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp6"),index > ,ui::display_text<gp_investment_text<6>>&> get() {
		 return country_gp6;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp6"),index > ,ui::display_text<gp_investment_text<6>> const&> get() const {
		 return country_gp6;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp7"),index > ,ui::display_text<gp_investment_text<7>>&> get() {
		 return country_gp7;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_gp7"),index > ,ui::display_text<gp_investment_text<7>> const&> get() const {
		 return country_gp7;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_boss_flag"),index > ,ui::masked_flag<investment_country_item_sphere_leader_flag>&> get() {
		 return country_boss_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_boss_flag"),index > ,ui::masked_flag<investment_country_item_sphere_leader_flag> const&> get() const {
		 return country_boss_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_prestige"),index > ,ui::display_text<investment_country_item_prestige_rank>&> get() {
		 return country_prestige;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_prestige"),index > ,ui::display_text<investment_country_item_prestige_rank> const&> get() const {
		 return country_prestige;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_economic"),index > ,ui::display_text<investment_country_item_industrial_rank>&> get() {
		 return country_economic;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_economic"),index > ,ui::display_text<investment_country_item_industrial_rank> const&> get() const {
		 return country_economic;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_military"),index > ,ui::display_text<investment_country_item_military_rank>&> get() {
		 return country_military;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_military"),index > ,ui::display_text<investment_country_item_military_rank> const&> get() const {
		 return country_military;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_total"),index > ,ui::display_text<investment_country_item_overall_rank>&> get() {
		 return country_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_total"),index > ,ui::display_text<investment_country_item_overall_rank> const&> get() const {
		 return country_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_opinion"),index > ,ui::display_text<investment_country_item_opinion_type>&> get() {
		 return country_opinion;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_opinion"),index > ,ui::display_text<investment_country_item_opinion_type> const&> get() const {
		 return country_opinion;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_relation"),index > ,ui::display_text<investment_country_item_relations_value>&> get() {
		 return country_relation;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_relation"),index > ,ui::display_text<investment_country_item_relations_value> const&> get() const {
		 return country_relation;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factories"),index > ,ui::display_text<investment_country_item_factories_count>&> get() {
		 return factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factories"),index > ,ui::display_text<investment_country_item_factories_count> const&> get() const {
		 return factories;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<investment_country_item_base, world_state&>) {
			 investment_country_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<investment_country_item_base, investment_country_item_internal_class&, world_state&>) {
			 investment_country_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<investment_country_item_base, world_state&>) 
			 investment_country_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<investment_country_item_background_button>, investment_country_item_internal_class&, world_state&>) 
			 country_select.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<investment_country_item_flag>, investment_country_item_internal_class&, world_state&>) 
			 country_flag.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_name>, investment_country_item_internal_class&, world_state&>) 
			 country_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_self_investment_text>, investment_country_item_internal_class&, world_state&>) 
			 invest.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<0>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp0.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<1>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp1.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<2>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<3>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp3.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<4>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp4.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<5>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp5.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<6>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp6.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<gp_investment_text<7>>, investment_country_item_internal_class&, world_state&>) 
			 country_gp7.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<investment_country_item_sphere_leader_flag>, investment_country_item_internal_class&, world_state&>) 
			 country_boss_flag.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_prestige_rank>, investment_country_item_internal_class&, world_state&>) 
			 country_prestige.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_industrial_rank>, investment_country_item_internal_class&, world_state&>) 
			 country_economic.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_military_rank>, investment_country_item_internal_class&, world_state&>) 
			 country_military.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_overall_rank>, investment_country_item_internal_class&, world_state&>) 
			 country_total.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_opinion_type>, investment_country_item_internal_class&, world_state&>) 
			 country_opinion.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_relations_value>, investment_country_item_internal_class&, world_state&>) 
			 country_relation.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_country_item_factories_count>, investment_country_item_internal_class&, world_state&>) 
			 factories.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 investment_country_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("country_select")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<investment_country_item_background_button>> vhelper(country_select, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<investment_country_item_background_button>> vhelper(country_select, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_flag")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<investment_country_item_flag>> vhelper(country_flag, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<investment_country_item_flag>> vhelper(country_flag, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_name>> vhelper(country_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_name>> vhelper(country_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("invest")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_self_investment_text>> vhelper(invest, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_self_investment_text>> vhelper(invest, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp0")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<0>>> vhelper(country_gp0, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<0>>> vhelper(country_gp0, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp1")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<1>>> vhelper(country_gp1, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<1>>> vhelper(country_gp1, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<2>>> vhelper(country_gp2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<2>>> vhelper(country_gp2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp3")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<3>>> vhelper(country_gp3, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<3>>> vhelper(country_gp3, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp4")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<4>>> vhelper(country_gp4, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<4>>> vhelper(country_gp4, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp5")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<5>>> vhelper(country_gp5, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<5>>> vhelper(country_gp5, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp6")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<6>>> vhelper(country_gp6, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<6>>> vhelper(country_gp6, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_gp7")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<7>>> vhelper(country_gp7, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<gp_investment_text<7>>> vhelper(country_gp7, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_boss_flag")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<investment_country_item_sphere_leader_flag>> vhelper(country_boss_flag, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<investment_country_item_sphere_leader_flag>> vhelper(country_boss_flag, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_prestige")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_prestige_rank>> vhelper(country_prestige, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_prestige_rank>> vhelper(country_prestige, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_economic")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_industrial_rank>> vhelper(country_economic, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_industrial_rank>> vhelper(country_economic, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_military")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_military_rank>> vhelper(country_military, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_military_rank>> vhelper(country_military, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_total")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_overall_rank>> vhelper(country_total, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_overall_rank>> vhelper(country_total, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_opinion")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_opinion_type>> vhelper(country_opinion, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_opinion_type>> vhelper(country_opinion, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_relation")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_relations_value>> vhelper(country_relation, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_relations_value>> vhelper(country_relation, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("factories")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_factories_count>> vhelper(factories, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_country_item_factories_count>> vhelper(factories, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<investment_country_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<investment_country_item_background_button>, investment_country_item_internal_class&, world_state&>)
			 country_select.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<investment_country_item_flag>, investment_country_item_internal_class&, world_state&>)
			 country_flag.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_name>, investment_country_item_internal_class&, world_state&>)
			 country_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_self_investment_text>, investment_country_item_internal_class&, world_state&>)
			 invest.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<0>>, investment_country_item_internal_class&, world_state&>)
			 country_gp0.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<1>>, investment_country_item_internal_class&, world_state&>)
			 country_gp1.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<2>>, investment_country_item_internal_class&, world_state&>)
			 country_gp2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<3>>, investment_country_item_internal_class&, world_state&>)
			 country_gp3.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<4>>, investment_country_item_internal_class&, world_state&>)
			 country_gp4.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<5>>, investment_country_item_internal_class&, world_state&>)
			 country_gp5.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<6>>, investment_country_item_internal_class&, world_state&>)
			 country_gp6.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<gp_investment_text<7>>, investment_country_item_internal_class&, world_state&>)
			 country_gp7.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<investment_country_item_sphere_leader_flag>, investment_country_item_internal_class&, world_state&>)
			 country_boss_flag.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_prestige_rank>, investment_country_item_internal_class&, world_state&>)
			 country_prestige.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_industrial_rank>, investment_country_item_internal_class&, world_state&>)
			 country_economic.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_military_rank>, investment_country_item_internal_class&, world_state&>)
			 country_military.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_overall_rank>, investment_country_item_internal_class&, world_state&>)
			 country_total.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_opinion_type>, investment_country_item_internal_class&, world_state&>)
			 country_opinion.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_relations_value>, investment_country_item_internal_class&, world_state&>)
			 country_relation.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_country_item_factories_count>, investment_country_item_internal_class&, world_state&>)
			 factories.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 investment_country_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<investment_country_item_base, world_state&>) {
			 investment_country_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<investment_country_item_base, investment_country_item_internal_class&, world_state&>) {
			 investment_country_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class investment_country_item : public investment_country_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, investment_country_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, investment_country_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class factory_filter_buttons_internal_class : public factory_filter_buttons_base {
public:
	 ui::simple_button<subsidize_all_button> prod_subsidize_all;
	 ui::simple_button<unsubsidize_all_button> prod_unsubsidize_all;
	 ui::simple_button<open_all_factories_button> prod_open_all_factories;
	 ui::simple_button<close_all_factories_button> prod_close_all_factories;
	 ui::simple_button<select_all_factories_filters_button> select_all;
	 ui::simple_button<deselect_all_factories_filters_button> deselect_all;
	 ui::button<show_hide_empty_states_button> show_empty_states;
	 ui::simple_button<sort_factories_by_state_name> sort_by_name;
	 ui::simple_button<sort_factories_by_count> sort_by_factories;
	 ui::simple_button<sort_factories_by_infrastructure> sort_by_infra;
	 ui::gui_window<goods_filters_base> filter_bounds;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_subsidize_all"),index > ,ui::simple_button<subsidize_all_button>&> get() {
		 return prod_subsidize_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_subsidize_all"),index > ,ui::simple_button<subsidize_all_button> const&> get() const {
		 return prod_subsidize_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_unsubsidize_all"),index > ,ui::simple_button<unsubsidize_all_button>&> get() {
		 return prod_unsubsidize_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_unsubsidize_all"),index > ,ui::simple_button<unsubsidize_all_button> const&> get() const {
		 return prod_unsubsidize_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_open_all_factories"),index > ,ui::simple_button<open_all_factories_button>&> get() {
		 return prod_open_all_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_open_all_factories"),index > ,ui::simple_button<open_all_factories_button> const&> get() const {
		 return prod_open_all_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_close_all_factories"),index > ,ui::simple_button<close_all_factories_button>&> get() {
		 return prod_close_all_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_close_all_factories"),index > ,ui::simple_button<close_all_factories_button> const&> get() const {
		 return prod_close_all_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("select_all"),index > ,ui::simple_button<select_all_factories_filters_button>&> get() {
		 return select_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("select_all"),index > ,ui::simple_button<select_all_factories_filters_button> const&> get() const {
		 return select_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("deselect_all"),index > ,ui::simple_button<deselect_all_factories_filters_button>&> get() {
		 return deselect_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("deselect_all"),index > ,ui::simple_button<deselect_all_factories_filters_button> const&> get() const {
		 return deselect_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("show_empty_states"),index > ,ui::button<show_hide_empty_states_button>&> get() {
		 return show_empty_states;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("show_empty_states"),index > ,ui::button<show_hide_empty_states_button> const&> get() const {
		 return show_empty_states;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_name"),index > ,ui::simple_button<sort_factories_by_state_name>&> get() {
		 return sort_by_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_name"),index > ,ui::simple_button<sort_factories_by_state_name> const&> get() const {
		 return sort_by_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_factories"),index > ,ui::simple_button<sort_factories_by_count>&> get() {
		 return sort_by_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_factories"),index > ,ui::simple_button<sort_factories_by_count> const&> get() const {
		 return sort_by_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_infra"),index > ,ui::simple_button<sort_factories_by_infrastructure>&> get() {
		 return sort_by_infra;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_infra"),index > ,ui::simple_button<sort_factories_by_infrastructure> const&> get() const {
		 return sort_by_infra;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_bounds"),index > ,ui::gui_window<goods_filters_base>&> get() {
		 return filter_bounds;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_bounds"),index > ,ui::gui_window<goods_filters_base> const&> get() const {
		 return filter_bounds;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<factory_filter_buttons_base, world_state&>) {
			 factory_filter_buttons_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_filter_buttons_base, factory_filter_buttons_internal_class&, world_state&>) {
			 factory_filter_buttons_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<factory_filter_buttons_base, world_state&>) 
			 factory_filter_buttons_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<subsidize_all_button>, factory_filter_buttons_internal_class&, world_state&>) 
			 prod_subsidize_all.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<unsubsidize_all_button>, factory_filter_buttons_internal_class&, world_state&>) 
			 prod_unsubsidize_all.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<open_all_factories_button>, factory_filter_buttons_internal_class&, world_state&>) 
			 prod_open_all_factories.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<close_all_factories_button>, factory_filter_buttons_internal_class&, world_state&>) 
			 prod_close_all_factories.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<select_all_factories_filters_button>, factory_filter_buttons_internal_class&, world_state&>) 
			 select_all.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<deselect_all_factories_filters_button>, factory_filter_buttons_internal_class&, world_state&>) 
			 deselect_all.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button<show_hide_empty_states_button>, factory_filter_buttons_internal_class&, world_state&>) 
			 show_empty_states.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_factories_by_state_name>, factory_filter_buttons_internal_class&, world_state&>) 
			 sort_by_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_factories_by_count>, factory_filter_buttons_internal_class&, world_state&>) 
			 sort_by_factories.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_factories_by_infrastructure>, factory_filter_buttons_internal_class&, world_state&>) 
			 sort_by_infra.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::gui_window<goods_filters_base>, factory_filter_buttons_internal_class&, world_state&>) 
			 filter_bounds.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 factory_filter_buttons_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("prod_subsidize_all")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<subsidize_all_button>> vhelper(prod_subsidize_all, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<subsidize_all_button>> vhelper(prod_subsidize_all, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_unsubsidize_all")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<unsubsidize_all_button>> vhelper(prod_unsubsidize_all, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<unsubsidize_all_button>> vhelper(prod_unsubsidize_all, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_open_all_factories")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<open_all_factories_button>> vhelper(prod_open_all_factories, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<open_all_factories_button>> vhelper(prod_open_all_factories, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_close_all_factories")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<close_all_factories_button>> vhelper(prod_close_all_factories, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<close_all_factories_button>> vhelper(prod_close_all_factories, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("select_all")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<select_all_factories_filters_button>> vhelper(select_all, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<select_all_factories_filters_button>> vhelper(select_all, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("deselect_all")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<deselect_all_factories_filters_button>> vhelper(deselect_all, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<deselect_all_factories_filters_button>> vhelper(deselect_all, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("show_empty_states")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button<show_hide_empty_states_button>> vhelper(show_empty_states, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button<show_hide_empty_states_button>> vhelper(show_empty_states, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_state_name>> vhelper(sort_by_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_state_name>> vhelper(sort_by_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_factories")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_count>> vhelper(sort_by_factories, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_count>> vhelper(sort_by_factories, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_infra")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_infrastructure>> vhelper(sort_by_infra, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_infrastructure>> vhelper(sort_by_infra, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_bounds")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::gui_window<goods_filters_base>> vhelper(filter_bounds, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::gui_window<goods_filters_base>> vhelper(filter_bounds, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<factory_filter_buttons_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<subsidize_all_button>, factory_filter_buttons_internal_class&, world_state&>)
			 prod_subsidize_all.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<unsubsidize_all_button>, factory_filter_buttons_internal_class&, world_state&>)
			 prod_unsubsidize_all.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<open_all_factories_button>, factory_filter_buttons_internal_class&, world_state&>)
			 prod_open_all_factories.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<close_all_factories_button>, factory_filter_buttons_internal_class&, world_state&>)
			 prod_close_all_factories.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<select_all_factories_filters_button>, factory_filter_buttons_internal_class&, world_state&>)
			 select_all.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<deselect_all_factories_filters_button>, factory_filter_buttons_internal_class&, world_state&>)
			 deselect_all.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button<show_hide_empty_states_button>, factory_filter_buttons_internal_class&, world_state&>)
			 show_empty_states.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_factories_by_state_name>, factory_filter_buttons_internal_class&, world_state&>)
			 sort_by_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_factories_by_count>, factory_filter_buttons_internal_class&, world_state&>)
			 sort_by_factories.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_factories_by_infrastructure>, factory_filter_buttons_internal_class&, world_state&>)
			 sort_by_infra.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::gui_window<goods_filters_base>, factory_filter_buttons_internal_class&, world_state&>)
			 filter_bounds.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 factory_filter_buttons_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<factory_filter_buttons_base, world_state&>) {
			 factory_filter_buttons_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_filter_buttons_base, factory_filter_buttons_internal_class&, world_state&>) {
			 factory_filter_buttons_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class factory_filter_buttons : public factory_filter_buttons_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_filter_buttons& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_filter_buttons& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class investment_filter_buttons_internal_class : public investment_filter_buttons_base {
public:
	 ui::masked_flag<investment_target_flag> country_flag;
	 ui::dynamic_icon<investment_target_flag_overlay> country_flag_overlay;
	 ui::display_text<investment_target_name> country_name;
	 ui::display_text<investment_target_amount> country_investment;
	 ui::simple_button<investment_target_back_button> invest_country_browse;
	 ui::simple_button<select_all_factories_filters_button> select_all;
	 ui::simple_button<deselect_all_factories_filters_button> deselect_all;
	 ui::button<show_hide_empty_states_button> show_empty_states;
	 ui::simple_button<sort_factories_by_state_name> sort_by_name;
	 ui::simple_button<sort_factories_by_count> sort_by_factories;
	 ui::simple_button<sort_factories_by_infrastructure> sort_by_infra;
	 ui::gui_window<goods_filters_base> filter_bounds;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag"),index > ,ui::masked_flag<investment_target_flag>&> get() {
		 return country_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag"),index > ,ui::masked_flag<investment_target_flag> const&> get() const {
		 return country_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag_overlay"),index > ,ui::dynamic_icon<investment_target_flag_overlay>&> get() {
		 return country_flag_overlay;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_flag_overlay"),index > ,ui::dynamic_icon<investment_target_flag_overlay> const&> get() const {
		 return country_flag_overlay;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_name"),index > ,ui::display_text<investment_target_name>&> get() {
		 return country_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_name"),index > ,ui::display_text<investment_target_name> const&> get() const {
		 return country_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_investment"),index > ,ui::display_text<investment_target_amount>&> get() {
		 return country_investment;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_investment"),index > ,ui::display_text<investment_target_amount> const&> get() const {
		 return country_investment;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest_country_browse"),index > ,ui::simple_button<investment_target_back_button>&> get() {
		 return invest_country_browse;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest_country_browse"),index > ,ui::simple_button<investment_target_back_button> const&> get() const {
		 return invest_country_browse;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("select_all"),index > ,ui::simple_button<select_all_factories_filters_button>&> get() {
		 return select_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("select_all"),index > ,ui::simple_button<select_all_factories_filters_button> const&> get() const {
		 return select_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("deselect_all"),index > ,ui::simple_button<deselect_all_factories_filters_button>&> get() {
		 return deselect_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("deselect_all"),index > ,ui::simple_button<deselect_all_factories_filters_button> const&> get() const {
		 return deselect_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("show_empty_states"),index > ,ui::button<show_hide_empty_states_button>&> get() {
		 return show_empty_states;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("show_empty_states"),index > ,ui::button<show_hide_empty_states_button> const&> get() const {
		 return show_empty_states;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_name"),index > ,ui::simple_button<sort_factories_by_state_name>&> get() {
		 return sort_by_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_name"),index > ,ui::simple_button<sort_factories_by_state_name> const&> get() const {
		 return sort_by_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_factories"),index > ,ui::simple_button<sort_factories_by_count>&> get() {
		 return sort_by_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_factories"),index > ,ui::simple_button<sort_factories_by_count> const&> get() const {
		 return sort_by_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_infra"),index > ,ui::simple_button<sort_factories_by_infrastructure>&> get() {
		 return sort_by_infra;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_infra"),index > ,ui::simple_button<sort_factories_by_infrastructure> const&> get() const {
		 return sort_by_infra;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_bounds"),index > ,ui::gui_window<goods_filters_base>&> get() {
		 return filter_bounds;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_bounds"),index > ,ui::gui_window<goods_filters_base> const&> get() const {
		 return filter_bounds;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<investment_filter_buttons_base, world_state&>) {
			 investment_filter_buttons_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<investment_filter_buttons_base, investment_filter_buttons_internal_class&, world_state&>) {
			 investment_filter_buttons_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<investment_filter_buttons_base, world_state&>) 
			 investment_filter_buttons_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<investment_target_flag>, investment_filter_buttons_internal_class&, world_state&>) 
			 country_flag.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<investment_target_flag_overlay>, investment_filter_buttons_internal_class&, world_state&>) 
			 country_flag_overlay.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_target_name>, investment_filter_buttons_internal_class&, world_state&>) 
			 country_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<investment_target_amount>, investment_filter_buttons_internal_class&, world_state&>) 
			 country_investment.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<investment_target_back_button>, investment_filter_buttons_internal_class&, world_state&>) 
			 invest_country_browse.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<select_all_factories_filters_button>, investment_filter_buttons_internal_class&, world_state&>) 
			 select_all.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<deselect_all_factories_filters_button>, investment_filter_buttons_internal_class&, world_state&>) 
			 deselect_all.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button<show_hide_empty_states_button>, investment_filter_buttons_internal_class&, world_state&>) 
			 show_empty_states.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_factories_by_state_name>, investment_filter_buttons_internal_class&, world_state&>) 
			 sort_by_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_factories_by_count>, investment_filter_buttons_internal_class&, world_state&>) 
			 sort_by_factories.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_factories_by_infrastructure>, investment_filter_buttons_internal_class&, world_state&>) 
			 sort_by_infra.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::gui_window<goods_filters_base>, investment_filter_buttons_internal_class&, world_state&>) 
			 filter_bounds.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 investment_filter_buttons_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("country_flag")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<investment_target_flag>> vhelper(country_flag, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<investment_target_flag>> vhelper(country_flag, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_flag_overlay")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<investment_target_flag_overlay>> vhelper(country_flag_overlay, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<investment_target_flag_overlay>> vhelper(country_flag_overlay, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_target_name>> vhelper(country_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_target_name>> vhelper(country_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_investment")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<investment_target_amount>> vhelper(country_investment, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<investment_target_amount>> vhelper(country_investment, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("invest_country_browse")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<investment_target_back_button>> vhelper(invest_country_browse, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<investment_target_back_button>> vhelper(invest_country_browse, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("select_all")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<select_all_factories_filters_button>> vhelper(select_all, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<select_all_factories_filters_button>> vhelper(select_all, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("deselect_all")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<deselect_all_factories_filters_button>> vhelper(deselect_all, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<deselect_all_factories_filters_button>> vhelper(deselect_all, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("show_empty_states")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button<show_hide_empty_states_button>> vhelper(show_empty_states, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button<show_hide_empty_states_button>> vhelper(show_empty_states, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_state_name>> vhelper(sort_by_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_state_name>> vhelper(sort_by_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_factories")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_count>> vhelper(sort_by_factories, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_count>> vhelper(sort_by_factories, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_infra")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_infrastructure>> vhelper(sort_by_infra, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_factories_by_infrastructure>> vhelper(sort_by_infra, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_bounds")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::gui_window<goods_filters_base>> vhelper(filter_bounds, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::gui_window<goods_filters_base>> vhelper(filter_bounds, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<investment_filter_buttons_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<investment_target_flag>, investment_filter_buttons_internal_class&, world_state&>)
			 country_flag.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<investment_target_flag_overlay>, investment_filter_buttons_internal_class&, world_state&>)
			 country_flag_overlay.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_target_name>, investment_filter_buttons_internal_class&, world_state&>)
			 country_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<investment_target_amount>, investment_filter_buttons_internal_class&, world_state&>)
			 country_investment.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<investment_target_back_button>, investment_filter_buttons_internal_class&, world_state&>)
			 invest_country_browse.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<select_all_factories_filters_button>, investment_filter_buttons_internal_class&, world_state&>)
			 select_all.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<deselect_all_factories_filters_button>, investment_filter_buttons_internal_class&, world_state&>)
			 deselect_all.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button<show_hide_empty_states_button>, investment_filter_buttons_internal_class&, world_state&>)
			 show_empty_states.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_factories_by_state_name>, investment_filter_buttons_internal_class&, world_state&>)
			 sort_by_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_factories_by_count>, investment_filter_buttons_internal_class&, world_state&>)
			 sort_by_factories.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_factories_by_infrastructure>, investment_filter_buttons_internal_class&, world_state&>)
			 sort_by_infra.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::gui_window<goods_filters_base>, investment_filter_buttons_internal_class&, world_state&>)
			 filter_bounds.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 investment_filter_buttons_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<investment_filter_buttons_base, world_state&>) {
			 investment_filter_buttons_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<investment_filter_buttons_base, investment_filter_buttons_internal_class&, world_state&>) {
			 investment_filter_buttons_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class investment_filter_buttons : public investment_filter_buttons_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, investment_filter_buttons& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, investment_filter_buttons& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class investment_pane_internal_class : public investment_pane_base {
public:
	 ui::simple_button<filter_all_button> filter_all;
	 ui::simple_button<filter_north_america_button> filter_north_america;
	 ui::simple_button<filter_south_america_button> filter_south_america;
	 ui::simple_button<filter_europe_button> filter_europe;
	 ui::simple_button<filter_africa_button> filter_africa;
	 ui::simple_button<filter_asia_button> filter_asia;
	 ui::simple_button<filter_oceania_button> filter_oceania;
	 ui::simple_button<filter_neighbours_button> filter_neighbours;
	 ui::simple_button<filter_sphere_button> filter_sphere;
	 ui::simple_button<filter_enemies_button> filter_enemies;
	 ui::simple_button<filter_allies_button> filter_allies;
	 ui::simple_button<sort_by_country_button> sort_by_country;
	 ui::simple_button<clear_text_button> sort_by_my_invest;
	 ui::masked_flag<sort_by_my_flag_button> sort_by_my_flag;
	 ui::simple_button<clear_text_button> sort_by_gp0;
	 ui::masked_flag<sort_by_gpflag0_button> sort_by_gpflag0;
	 ui::simple_button<clear_text_button> sort_by_gp1;
	 ui::masked_flag<sort_by_gpflag1_button> sort_by_gpflag1;
	 ui::simple_button<clear_text_button> sort_by_gp2;
	 ui::masked_flag<sort_by_gpflag2_button> sort_by_gpflag2;
	 ui::simple_button<clear_text_button> sort_by_gp3;
	 ui::masked_flag<sort_by_gpflag3_button> sort_by_gpflag3;
	 ui::simple_button<clear_text_button> sort_by_gp4;
	 ui::masked_flag<sort_by_gpflag4_button> sort_by_gpflag4;
	 ui::simple_button<clear_text_button> sort_by_gp5;
	 ui::masked_flag<sort_by_gpflag5_button> sort_by_gpflag5;
	 ui::simple_button<clear_text_button> sort_by_gp6;
	 ui::masked_flag<sort_by_gpflag6_button> sort_by_gpflag6;
	 ui::simple_button<clear_text_button> sort_by_gp7;
	 ui::masked_flag<sort_by_gpflag7_button> sort_by_gpflag7;
	 ui::simple_button<sort_by_boss_button> sort_by_boss;
	 ui::simple_button<sort_by_prestige_button> sort_by_prestige;
	 ui::simple_button<sort_by_economic_button> sort_by_economic;
	 ui::simple_button<sort_by_military_button> sort_by_military;
	 ui::simple_button<sort_by_total_button> sort_by_total;
	 ui::simple_button<sort_by_opinion_button> sort_by_opinion;
	 ui::simple_button<sort_by_relation_button> sort_by_relation;
	 ui::simple_button<sort_by_invest_factories_button> sort_by_invest_factories;
	 ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag> country_listbox;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_all"),index > ,ui::simple_button<filter_all_button>&> get() {
		 return filter_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_all"),index > ,ui::simple_button<filter_all_button> const&> get() const {
		 return filter_all;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_north_america"),index > ,ui::simple_button<filter_north_america_button>&> get() {
		 return filter_north_america;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_north_america"),index > ,ui::simple_button<filter_north_america_button> const&> get() const {
		 return filter_north_america;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_south_america"),index > ,ui::simple_button<filter_south_america_button>&> get() {
		 return filter_south_america;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_south_america"),index > ,ui::simple_button<filter_south_america_button> const&> get() const {
		 return filter_south_america;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_europe"),index > ,ui::simple_button<filter_europe_button>&> get() {
		 return filter_europe;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_europe"),index > ,ui::simple_button<filter_europe_button> const&> get() const {
		 return filter_europe;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_africa"),index > ,ui::simple_button<filter_africa_button>&> get() {
		 return filter_africa;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_africa"),index > ,ui::simple_button<filter_africa_button> const&> get() const {
		 return filter_africa;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_asia"),index > ,ui::simple_button<filter_asia_button>&> get() {
		 return filter_asia;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_asia"),index > ,ui::simple_button<filter_asia_button> const&> get() const {
		 return filter_asia;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_oceania"),index > ,ui::simple_button<filter_oceania_button>&> get() {
		 return filter_oceania;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_oceania"),index > ,ui::simple_button<filter_oceania_button> const&> get() const {
		 return filter_oceania;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_neighbours"),index > ,ui::simple_button<filter_neighbours_button>&> get() {
		 return filter_neighbours;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_neighbours"),index > ,ui::simple_button<filter_neighbours_button> const&> get() const {
		 return filter_neighbours;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_sphere"),index > ,ui::simple_button<filter_sphere_button>&> get() {
		 return filter_sphere;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_sphere"),index > ,ui::simple_button<filter_sphere_button> const&> get() const {
		 return filter_sphere;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_enemies"),index > ,ui::simple_button<filter_enemies_button>&> get() {
		 return filter_enemies;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_enemies"),index > ,ui::simple_button<filter_enemies_button> const&> get() const {
		 return filter_enemies;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_allies"),index > ,ui::simple_button<filter_allies_button>&> get() {
		 return filter_allies;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("filter_allies"),index > ,ui::simple_button<filter_allies_button> const&> get() const {
		 return filter_allies;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_country"),index > ,ui::simple_button<sort_by_country_button>&> get() {
		 return sort_by_country;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_country"),index > ,ui::simple_button<sort_by_country_button> const&> get() const {
		 return sort_by_country;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_my_invest"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_my_invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_my_invest"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_my_invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_my_flag"),index > ,ui::masked_flag<sort_by_my_flag_button>&> get() {
		 return sort_by_my_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_my_flag"),index > ,ui::masked_flag<sort_by_my_flag_button> const&> get() const {
		 return sort_by_my_flag;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp0"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp0"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag0"),index > ,ui::masked_flag<sort_by_gpflag0_button>&> get() {
		 return sort_by_gpflag0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag0"),index > ,ui::masked_flag<sort_by_gpflag0_button> const&> get() const {
		 return sort_by_gpflag0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp1"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp1"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag1"),index > ,ui::masked_flag<sort_by_gpflag1_button>&> get() {
		 return sort_by_gpflag1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag1"),index > ,ui::masked_flag<sort_by_gpflag1_button> const&> get() const {
		 return sort_by_gpflag1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp2"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp2"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag2"),index > ,ui::masked_flag<sort_by_gpflag2_button>&> get() {
		 return sort_by_gpflag2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag2"),index > ,ui::masked_flag<sort_by_gpflag2_button> const&> get() const {
		 return sort_by_gpflag2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp3"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp3"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag3"),index > ,ui::masked_flag<sort_by_gpflag3_button>&> get() {
		 return sort_by_gpflag3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag3"),index > ,ui::masked_flag<sort_by_gpflag3_button> const&> get() const {
		 return sort_by_gpflag3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp4"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp4;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp4"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp4;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag4"),index > ,ui::masked_flag<sort_by_gpflag4_button>&> get() {
		 return sort_by_gpflag4;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag4"),index > ,ui::masked_flag<sort_by_gpflag4_button> const&> get() const {
		 return sort_by_gpflag4;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp5"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp5;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp5"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp5;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag5"),index > ,ui::masked_flag<sort_by_gpflag5_button>&> get() {
		 return sort_by_gpflag5;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag5"),index > ,ui::masked_flag<sort_by_gpflag5_button> const&> get() const {
		 return sort_by_gpflag5;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp6"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp6;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp6"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp6;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag6"),index > ,ui::masked_flag<sort_by_gpflag6_button>&> get() {
		 return sort_by_gpflag6;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag6"),index > ,ui::masked_flag<sort_by_gpflag6_button> const&> get() const {
		 return sort_by_gpflag6;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp7"),index > ,ui::simple_button<clear_text_button>&> get() {
		 return sort_by_gp7;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gp7"),index > ,ui::simple_button<clear_text_button> const&> get() const {
		 return sort_by_gp7;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag7"),index > ,ui::masked_flag<sort_by_gpflag7_button>&> get() {
		 return sort_by_gpflag7;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_gpflag7"),index > ,ui::masked_flag<sort_by_gpflag7_button> const&> get() const {
		 return sort_by_gpflag7;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_boss"),index > ,ui::simple_button<sort_by_boss_button>&> get() {
		 return sort_by_boss;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_boss"),index > ,ui::simple_button<sort_by_boss_button> const&> get() const {
		 return sort_by_boss;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_prestige"),index > ,ui::simple_button<sort_by_prestige_button>&> get() {
		 return sort_by_prestige;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_prestige"),index > ,ui::simple_button<sort_by_prestige_button> const&> get() const {
		 return sort_by_prestige;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_economic"),index > ,ui::simple_button<sort_by_economic_button>&> get() {
		 return sort_by_economic;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_economic"),index > ,ui::simple_button<sort_by_economic_button> const&> get() const {
		 return sort_by_economic;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_military"),index > ,ui::simple_button<sort_by_military_button>&> get() {
		 return sort_by_military;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_military"),index > ,ui::simple_button<sort_by_military_button> const&> get() const {
		 return sort_by_military;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_total"),index > ,ui::simple_button<sort_by_total_button>&> get() {
		 return sort_by_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_total"),index > ,ui::simple_button<sort_by_total_button> const&> get() const {
		 return sort_by_total;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_opinion"),index > ,ui::simple_button<sort_by_opinion_button>&> get() {
		 return sort_by_opinion;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_opinion"),index > ,ui::simple_button<sort_by_opinion_button> const&> get() const {
		 return sort_by_opinion;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_relation"),index > ,ui::simple_button<sort_by_relation_button>&> get() {
		 return sort_by_relation;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_relation"),index > ,ui::simple_button<sort_by_relation_button> const&> get() const {
		 return sort_by_relation;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_invest_factories"),index > ,ui::simple_button<sort_by_invest_factories_button>&> get() {
		 return sort_by_invest_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_invest_factories"),index > ,ui::simple_button<sort_by_invest_factories_button> const&> get() const {
		 return sort_by_invest_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_listbox"),index > ,ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag>&> get() {
		 return country_listbox;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("country_listbox"),index > ,ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag> const&> get() const {
		 return country_listbox;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<investment_pane_base, world_state&>) {
			 investment_pane_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<investment_pane_base, investment_pane_internal_class&, world_state&>) {
			 investment_pane_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<investment_pane_base, world_state&>) 
			 investment_pane_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_all_button>, investment_pane_internal_class&, world_state&>) 
			 filter_all.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_north_america_button>, investment_pane_internal_class&, world_state&>) 
			 filter_north_america.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_south_america_button>, investment_pane_internal_class&, world_state&>) 
			 filter_south_america.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_europe_button>, investment_pane_internal_class&, world_state&>) 
			 filter_europe.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_africa_button>, investment_pane_internal_class&, world_state&>) 
			 filter_africa.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_asia_button>, investment_pane_internal_class&, world_state&>) 
			 filter_asia.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_oceania_button>, investment_pane_internal_class&, world_state&>) 
			 filter_oceania.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_neighbours_button>, investment_pane_internal_class&, world_state&>) 
			 filter_neighbours.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_sphere_button>, investment_pane_internal_class&, world_state&>) 
			 filter_sphere.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_enemies_button>, investment_pane_internal_class&, world_state&>) 
			 filter_enemies.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<filter_allies_button>, investment_pane_internal_class&, world_state&>) 
			 filter_allies.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_country_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_country.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_my_invest.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_my_flag_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_my_flag.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp0.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag0_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag0.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp1.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag1_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag1.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag2_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp3.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag3_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag3.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp4.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag4_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag4.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp5.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag5_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag5.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp6.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag6_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag6.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gp7.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<sort_by_gpflag7_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_gpflag7.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_boss_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_boss.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_prestige_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_prestige.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_economic_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_economic.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_military_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_military.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_total_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_total.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_opinion_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_opinion.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_relation_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_relation.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_invest_factories_button>, investment_pane_internal_class&, world_state&>) 
			 sort_by_invest_factories.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag>, investment_pane_internal_class&, world_state&>) 
			 country_listbox.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 investment_pane_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("filter_all")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_all_button>> vhelper(filter_all, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_all_button>> vhelper(filter_all, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_north_america")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_north_america_button>> vhelper(filter_north_america, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_north_america_button>> vhelper(filter_north_america, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_south_america")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_south_america_button>> vhelper(filter_south_america, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_south_america_button>> vhelper(filter_south_america, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_europe")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_europe_button>> vhelper(filter_europe, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_europe_button>> vhelper(filter_europe, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_africa")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_africa_button>> vhelper(filter_africa, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_africa_button>> vhelper(filter_africa, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_asia")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_asia_button>> vhelper(filter_asia, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_asia_button>> vhelper(filter_asia, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_oceania")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_oceania_button>> vhelper(filter_oceania, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_oceania_button>> vhelper(filter_oceania, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_neighbours")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_neighbours_button>> vhelper(filter_neighbours, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_neighbours_button>> vhelper(filter_neighbours, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_sphere")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_sphere_button>> vhelper(filter_sphere, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_sphere_button>> vhelper(filter_sphere, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_enemies")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_enemies_button>> vhelper(filter_enemies, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_enemies_button>> vhelper(filter_enemies, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("filter_allies")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<filter_allies_button>> vhelper(filter_allies, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<filter_allies_button>> vhelper(filter_allies, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_country")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_country_button>> vhelper(sort_by_country, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_country_button>> vhelper(sort_by_country, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_my_invest")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_my_invest, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_my_invest, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_my_flag")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_my_flag_button>> vhelper(sort_by_my_flag, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_my_flag_button>> vhelper(sort_by_my_flag, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp0")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp0, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp0, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag0")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag0_button>> vhelper(sort_by_gpflag0, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag0_button>> vhelper(sort_by_gpflag0, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp1")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp1, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp1, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag1")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag1_button>> vhelper(sort_by_gpflag1, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag1_button>> vhelper(sort_by_gpflag1, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag2_button>> vhelper(sort_by_gpflag2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag2_button>> vhelper(sort_by_gpflag2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp3")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp3, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp3, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag3")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag3_button>> vhelper(sort_by_gpflag3, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag3_button>> vhelper(sort_by_gpflag3, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp4")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp4, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp4, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag4")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag4_button>> vhelper(sort_by_gpflag4, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag4_button>> vhelper(sort_by_gpflag4, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp5")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp5, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp5, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag5")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag5_button>> vhelper(sort_by_gpflag5, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag5_button>> vhelper(sort_by_gpflag5, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp6")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp6, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp6, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag6")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag6_button>> vhelper(sort_by_gpflag6, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag6_button>> vhelper(sort_by_gpflag6, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gp7")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp7, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<clear_text_button>> vhelper(sort_by_gp7, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_gpflag7")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag7_button>> vhelper(sort_by_gpflag7, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<sort_by_gpflag7_button>> vhelper(sort_by_gpflag7, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_boss")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_boss_button>> vhelper(sort_by_boss, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_boss_button>> vhelper(sort_by_boss, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_prestige")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_prestige_button>> vhelper(sort_by_prestige, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_prestige_button>> vhelper(sort_by_prestige, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_economic")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_economic_button>> vhelper(sort_by_economic, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_economic_button>> vhelper(sort_by_economic, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_military")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_military_button>> vhelper(sort_by_military, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_military_button>> vhelper(sort_by_military, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_total")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_total_button>> vhelper(sort_by_total, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_total_button>> vhelper(sort_by_total, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_opinion")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_opinion_button>> vhelper(sort_by_opinion, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_opinion_button>> vhelper(sort_by_opinion, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_relation")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_relation_button>> vhelper(sort_by_relation, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_relation_button>> vhelper(sort_by_relation, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_invest_factories")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_invest_factories_button>> vhelper(sort_by_invest_factories, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_invest_factories_button>> vhelper(sort_by_invest_factories, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("country_listbox")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag>> vhelper(country_listbox, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag>> vhelper(country_listbox, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<investment_pane_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_all_button>, investment_pane_internal_class&, world_state&>)
			 filter_all.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_north_america_button>, investment_pane_internal_class&, world_state&>)
			 filter_north_america.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_south_america_button>, investment_pane_internal_class&, world_state&>)
			 filter_south_america.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_europe_button>, investment_pane_internal_class&, world_state&>)
			 filter_europe.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_africa_button>, investment_pane_internal_class&, world_state&>)
			 filter_africa.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_asia_button>, investment_pane_internal_class&, world_state&>)
			 filter_asia.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_oceania_button>, investment_pane_internal_class&, world_state&>)
			 filter_oceania.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_neighbours_button>, investment_pane_internal_class&, world_state&>)
			 filter_neighbours.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_sphere_button>, investment_pane_internal_class&, world_state&>)
			 filter_sphere.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_enemies_button>, investment_pane_internal_class&, world_state&>)
			 filter_enemies.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<filter_allies_button>, investment_pane_internal_class&, world_state&>)
			 filter_allies.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_country_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_country.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_my_invest.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_my_flag_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_my_flag.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp0.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag0_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag0.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp1.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag1_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag1.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag2_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp3.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag3_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag3.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp4.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag4_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag4.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp5.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag5_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag5.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp6.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag6_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag6.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<clear_text_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gp7.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<sort_by_gpflag7_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_gpflag7.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_boss_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_boss.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_prestige_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_prestige.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_economic_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_economic.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_military_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_military.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_total_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_total.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_opinion_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_opinion.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_relation_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_relation.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_invest_factories_button>, investment_pane_internal_class&, world_state&>)
			 sort_by_invest_factories.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<investment_country_lb, investment_country_item, nations::country_tag>, investment_pane_internal_class&, world_state&>)
			 country_listbox.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 investment_pane_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<investment_pane_base, world_state&>) {
			 investment_pane_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<investment_pane_base, investment_pane_internal_class&, world_state&>) {
			 investment_pane_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class investment_pane : public investment_pane_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, investment_pane& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, investment_pane& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class state_window_internal_class : public state_window_base {
public:
	 ui::simple_button<state_focus_button> state_focus;
	 ui::display_text<state_name_s, 4> state_name;
	 ui::display_text<state_factory_count, 4> factory_count;
	 ui::simple_button<state_build_factory_button> build_new_factory;
	 ui::display_text<state_average_infrastructure> avg_infra_text;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_focus"),index > ,ui::simple_button<state_focus_button>&> get() {
		 return state_focus;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_focus"),index > ,ui::simple_button<state_focus_button> const&> get() const {
		 return state_focus;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_name"),index > ,ui::display_text<state_name_s, 4>&> get() {
		 return state_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_name"),index > ,ui::display_text<state_name_s, 4> const&> get() const {
		 return state_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_count"),index > ,ui::display_text<state_factory_count, 4>&> get() {
		 return factory_count;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_count"),index > ,ui::display_text<state_factory_count, 4> const&> get() const {
		 return factory_count;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("build_new_factory"),index > ,ui::simple_button<state_build_factory_button>&> get() {
		 return build_new_factory;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("build_new_factory"),index > ,ui::simple_button<state_build_factory_button> const&> get() const {
		 return build_new_factory;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("avg_infra_text"),index > ,ui::display_text<state_average_infrastructure>&> get() {
		 return avg_infra_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("avg_infra_text"),index > ,ui::display_text<state_average_infrastructure> const&> get() const {
		 return avg_infra_text;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<state_window_base, world_state&>) {
			 state_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<state_window_base, state_window_internal_class&, world_state&>) {
			 state_window_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<state_window_base, world_state&>) 
			 state_window_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<state_focus_button>, state_window_internal_class&, world_state&>) 
			 state_focus.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<state_name_s, 4>, state_window_internal_class&, world_state&>) 
			 state_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<state_factory_count, 4>, state_window_internal_class&, world_state&>) 
			 factory_count.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<state_build_factory_button>, state_window_internal_class&, world_state&>) 
			 build_new_factory.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<state_average_infrastructure>, state_window_internal_class&, world_state&>) 
			 avg_infra_text.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 state_window_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("state_focus")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<state_focus_button>> vhelper(state_focus, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<state_focus_button>> vhelper(state_focus, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("state_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<state_name_s, 4>> vhelper(state_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<state_name_s, 4>> vhelper(state_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("factory_count")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<state_factory_count, 4>> vhelper(factory_count, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<state_factory_count, 4>> vhelper(factory_count, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("build_new_factory")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<state_build_factory_button>> vhelper(build_new_factory, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<state_build_factory_button>> vhelper(build_new_factory, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("avg_infra_text")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<state_average_infrastructure>> vhelper(avg_infra_text, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<state_average_infrastructure>> vhelper(avg_infra_text, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<state_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<state_focus_button>, state_window_internal_class&, world_state&>)
			 state_focus.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<state_name_s, 4>, state_window_internal_class&, world_state&>)
			 state_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<state_factory_count, 4>, state_window_internal_class&, world_state&>)
			 factory_count.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<state_build_factory_button>, state_window_internal_class&, world_state&>)
			 build_new_factory.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<state_average_infrastructure>, state_window_internal_class&, world_state&>)
			 avg_infra_text.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 state_window_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<state_window_base, world_state&>) {
			 state_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<state_window_base, state_window_internal_class&, world_state&>) {
			 state_window_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class state_window : public state_window_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, state_window& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, state_window& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class project_item_internal_class : public project_item_base {
public:
	 ui::display_text<project_state_name> state_name;
	 ui::dynamic_icon<project_resource_icon> project_icon;
	 ui::dynamic_icon<project_infrastructure_icon> infra;
	 ui::display_text<project_name_s> project_name;
	 ui::display_text<project_cost_s> project_cost;
	 ui::dynamic_icon<project_investor_icon> pop_icon;
	 ui::display_text<project_investor_amount> pop_amount;
	 ui::button<project_invest_button> invest_project;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_name"),index > ,ui::display_text<project_state_name>&> get() {
		 return state_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_name"),index > ,ui::display_text<project_state_name> const&> get() const {
		 return state_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_icon"),index > ,ui::dynamic_icon<project_resource_icon>&> get() {
		 return project_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_icon"),index > ,ui::dynamic_icon<project_resource_icon> const&> get() const {
		 return project_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("infra"),index > ,ui::dynamic_icon<project_infrastructure_icon>&> get() {
		 return infra;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("infra"),index > ,ui::dynamic_icon<project_infrastructure_icon> const&> get() const {
		 return infra;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_name"),index > ,ui::display_text<project_name_s>&> get() {
		 return project_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_name"),index > ,ui::display_text<project_name_s> const&> get() const {
		 return project_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_cost"),index > ,ui::display_text<project_cost_s>&> get() {
		 return project_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_cost"),index > ,ui::display_text<project_cost_s> const&> get() const {
		 return project_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_icon"),index > ,ui::dynamic_icon<project_investor_icon>&> get() {
		 return pop_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_icon"),index > ,ui::dynamic_icon<project_investor_icon> const&> get() const {
		 return pop_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount"),index > ,ui::display_text<project_investor_amount>&> get() {
		 return pop_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount"),index > ,ui::display_text<project_investor_amount> const&> get() const {
		 return pop_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest_project"),index > ,ui::button<project_invest_button>&> get() {
		 return invest_project;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest_project"),index > ,ui::button<project_invest_button> const&> get() const {
		 return invest_project;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<project_item_base, world_state&>) {
			 project_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<project_item_base, project_item_internal_class&, world_state&>) {
			 project_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<project_item_base, world_state&>) 
			 project_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<project_state_name>, project_item_internal_class&, world_state&>) 
			 state_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<project_resource_icon>, project_item_internal_class&, world_state&>) 
			 project_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<project_infrastructure_icon>, project_item_internal_class&, world_state&>) 
			 infra.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<project_name_s>, project_item_internal_class&, world_state&>) 
			 project_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<project_cost_s>, project_item_internal_class&, world_state&>) 
			 project_cost.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<project_investor_icon>, project_item_internal_class&, world_state&>) 
			 pop_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<project_investor_amount>, project_item_internal_class&, world_state&>) 
			 pop_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button<project_invest_button>, project_item_internal_class&, world_state&>) 
			 invest_project.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 project_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("state_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<project_state_name>> vhelper(state_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<project_state_name>> vhelper(state_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("project_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<project_resource_icon>> vhelper(project_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<project_resource_icon>> vhelper(project_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("infra")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<project_infrastructure_icon>> vhelper(infra, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<project_infrastructure_icon>> vhelper(infra, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("project_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<project_name_s>> vhelper(project_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<project_name_s>> vhelper(project_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("project_cost")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<project_cost_s>> vhelper(project_cost, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<project_cost_s>> vhelper(project_cost, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<project_investor_icon>> vhelper(pop_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<project_investor_icon>> vhelper(pop_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<project_investor_amount>> vhelper(pop_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<project_investor_amount>> vhelper(pop_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("invest_project")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button<project_invest_button>> vhelper(invest_project, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button<project_invest_button>> vhelper(invest_project, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<project_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<project_state_name>, project_item_internal_class&, world_state&>)
			 state_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<project_resource_icon>, project_item_internal_class&, world_state&>)
			 project_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<project_infrastructure_icon>, project_item_internal_class&, world_state&>)
			 infra.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<project_name_s>, project_item_internal_class&, world_state&>)
			 project_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<project_cost_s>, project_item_internal_class&, world_state&>)
			 project_cost.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<project_investor_icon>, project_item_internal_class&, world_state&>)
			 pop_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<project_investor_amount>, project_item_internal_class&, world_state&>)
			 pop_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button<project_invest_button>, project_item_internal_class&, world_state&>)
			 invest_project.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 project_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<project_item_base, world_state&>) {
			 project_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<project_item_base, project_item_internal_class&, world_state&>) {
			 project_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class project_item : public project_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, project_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, project_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class production_window_t_internal_class : public production_window_base {
public:
	 ui::simple_button<close_button_s> close_button;
	 ui::button_group_member tab_factories;
	 ui::button_group_member tab_invest;
	 ui::button_group_member tab_goodsproduction;
	 ui::button_group_member tab_popprojects;
	 ui::button_group<CT_STRING("tab_factories"), CT_STRING("tab_invest"), CT_STRING("tab_goodsproduction"), CT_STRING("tab_popprojects"), production_tab_button_group_b> production_tab_button_group;
	 ui::display_text<empty_text_box> tab_factories_text;
	 ui::display_text<empty_text_box> tab_invest_text;
	 ui::display_text<empty_text_box> tab_goodsproduction_text;
	 ui::display_text<empty_text_box> tab_popprojects_text;
	 investment_pane investment_browser;
	 ui::simple_button<sort_by_project_state_button> sort_by_state;
	 ui::simple_button<sort_by_project_type_button> sort_by_projects;
	 ui::simple_button<sort_by_project_completion_button> sort_by_completion;
	 ui::simple_button<sort_by_project_investors_button> sort_by_projecteers;
	 factory_filter_buttons factory_buttons;
	 investment_filter_buttons invest_buttons;
	 ui::discrete_listbox<state_details_lb, state_window, nations::state_tag> state_listbox;
	 ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag> state_listbox_invest;
	 production_info_pane good_production;
	 ui::discrete_listbox<projects_lb, project_item, nations::state_tag> project_listbox;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("close_button"),index > ,ui::simple_button<close_button_s>&> get() {
		 return close_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("close_button"),index > ,ui::simple_button<close_button_s> const&> get() const {
		 return close_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_factories"),index > ,ui::button_group_member&> get() {
		 return tab_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_factories"),index > ,ui::button_group_member const&> get() const {
		 return tab_factories;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_invest"),index > ,ui::button_group_member&> get() {
		 return tab_invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_invest"),index > ,ui::button_group_member const&> get() const {
		 return tab_invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_goodsproduction"),index > ,ui::button_group_member&> get() {
		 return tab_goodsproduction;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_goodsproduction"),index > ,ui::button_group_member const&> get() const {
		 return tab_goodsproduction;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_popprojects"),index > ,ui::button_group_member&> get() {
		 return tab_popprojects;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_popprojects"),index > ,ui::button_group_member const&> get() const {
		 return tab_popprojects;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("production_tab_button_group"),index > ,ui::button_group<CT_STRING("tab_factories"), CT_STRING("tab_invest"), CT_STRING("tab_goodsproduction"), CT_STRING("tab_popprojects"), production_tab_button_group_b>&> get() {
		 return production_tab_button_group;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("production_tab_button_group"),index > ,ui::button_group<CT_STRING("tab_factories"), CT_STRING("tab_invest"), CT_STRING("tab_goodsproduction"), CT_STRING("tab_popprojects"), production_tab_button_group_b> const&> get() const {
		 return production_tab_button_group;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_factories_text"),index > ,ui::display_text<empty_text_box>&> get() {
		 return tab_factories_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_factories_text"),index > ,ui::display_text<empty_text_box> const&> get() const {
		 return tab_factories_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_invest_text"),index > ,ui::display_text<empty_text_box>&> get() {
		 return tab_invest_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_invest_text"),index > ,ui::display_text<empty_text_box> const&> get() const {
		 return tab_invest_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_goodsproduction_text"),index > ,ui::display_text<empty_text_box>&> get() {
		 return tab_goodsproduction_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_goodsproduction_text"),index > ,ui::display_text<empty_text_box> const&> get() const {
		 return tab_goodsproduction_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_popprojects_text"),index > ,ui::display_text<empty_text_box>&> get() {
		 return tab_popprojects_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("tab_popprojects_text"),index > ,ui::display_text<empty_text_box> const&> get() const {
		 return tab_popprojects_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("investment_browser"),index > ,investment_pane&> get() {
		 return investment_browser;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("investment_browser"),index > ,investment_pane const&> get() const {
		 return investment_browser;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_state"),index > ,ui::simple_button<sort_by_project_state_button>&> get() {
		 return sort_by_state;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_state"),index > ,ui::simple_button<sort_by_project_state_button> const&> get() const {
		 return sort_by_state;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_projects"),index > ,ui::simple_button<sort_by_project_type_button>&> get() {
		 return sort_by_projects;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_projects"),index > ,ui::simple_button<sort_by_project_type_button> const&> get() const {
		 return sort_by_projects;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_completion"),index > ,ui::simple_button<sort_by_project_completion_button>&> get() {
		 return sort_by_completion;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_completion"),index > ,ui::simple_button<sort_by_project_completion_button> const&> get() const {
		 return sort_by_completion;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_projecteers"),index > ,ui::simple_button<sort_by_project_investors_button>&> get() {
		 return sort_by_projecteers;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("sort_by_projecteers"),index > ,ui::simple_button<sort_by_project_investors_button> const&> get() const {
		 return sort_by_projecteers;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_buttons"),index > ,factory_filter_buttons&> get() {
		 return factory_buttons;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_buttons"),index > ,factory_filter_buttons const&> get() const {
		 return factory_buttons;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest_buttons"),index > ,investment_filter_buttons&> get() {
		 return invest_buttons;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("invest_buttons"),index > ,investment_filter_buttons const&> get() const {
		 return invest_buttons;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_listbox"),index > ,ui::discrete_listbox<state_details_lb, state_window, nations::state_tag>&> get() {
		 return state_listbox;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_listbox"),index > ,ui::discrete_listbox<state_details_lb, state_window, nations::state_tag> const&> get() const {
		 return state_listbox;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_listbox_invest"),index > ,ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag>&> get() {
		 return state_listbox_invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_listbox_invest"),index > ,ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag> const&> get() const {
		 return state_listbox_invest;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("good_production"),index > ,production_info_pane&> get() {
		 return good_production;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("good_production"),index > ,production_info_pane const&> get() const {
		 return good_production;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_listbox"),index > ,ui::discrete_listbox<projects_lb, project_item, nations::state_tag>&> get() {
		 return project_listbox;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("project_listbox"),index > ,ui::discrete_listbox<projects_lb, project_item, nations::state_tag> const&> get() const {
		 return project_listbox;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<production_window_base, world_state&>) {
			 production_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<production_window_base, production_window_t_internal_class&, world_state&>) {
			 production_window_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<production_window_base, world_state&>) 
			 production_window_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<close_button_s>, production_window_t_internal_class&, world_state&>) 
			 close_button.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button_group_member, production_window_t_internal_class&, world_state&>) 
			 tab_factories.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button_group_member, production_window_t_internal_class&, world_state&>) 
			 tab_invest.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button_group_member, production_window_t_internal_class&, world_state&>) 
			 tab_goodsproduction.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button_group_member, production_window_t_internal_class&, world_state&>) 
			 tab_popprojects.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button_group<CT_STRING("tab_factories"), CT_STRING("tab_invest"), CT_STRING("tab_goodsproduction"), CT_STRING("tab_popprojects"), production_tab_button_group_b>, production_window_t_internal_class&, world_state&>) 
			 production_tab_button_group.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>) 
			 tab_factories_text.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>) 
			 tab_invest_text.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>) 
			 tab_goodsproduction_text.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>) 
			 tab_popprojects_text.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<investment_pane, production_window_t_internal_class&, world_state&>) 
			 investment_browser.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_project_state_button>, production_window_t_internal_class&, world_state&>) 
			 sort_by_state.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_project_type_button>, production_window_t_internal_class&, world_state&>) 
			 sort_by_projects.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_project_completion_button>, production_window_t_internal_class&, world_state&>) 
			 sort_by_completion.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<sort_by_project_investors_button>, production_window_t_internal_class&, world_state&>) 
			 sort_by_projecteers.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<factory_filter_buttons, production_window_t_internal_class&, world_state&>) 
			 factory_buttons.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<investment_filter_buttons, production_window_t_internal_class&, world_state&>) 
			 invest_buttons.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<state_details_lb, state_window, nations::state_tag>, production_window_t_internal_class&, world_state&>) 
			 state_listbox.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag>, production_window_t_internal_class&, world_state&>) 
			 state_listbox_invest.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<production_info_pane, production_window_t_internal_class&, world_state&>) 
			 good_production.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<projects_lb, project_item, nations::state_tag>, production_window_t_internal_class&, world_state&>) 
			 project_listbox.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 production_window_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("close_button")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<close_button_s>> vhelper(close_button, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<close_button_s>> vhelper(close_button, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_factories")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_factories, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_factories, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_invest")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_invest, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_invest, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_goodsproduction")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_goodsproduction, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_goodsproduction, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_popprojects")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_popprojects, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button_group_member> vhelper(tab_popprojects, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("production_tab_button_group")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button_group<CT_STRING("tab_factories"), CT_STRING("tab_invest"), CT_STRING("tab_goodsproduction"), CT_STRING("tab_popprojects"), production_tab_button_group_b>> vhelper(production_tab_button_group, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button_group<CT_STRING("tab_factories"), CT_STRING("tab_invest"), CT_STRING("tab_goodsproduction"), CT_STRING("tab_popprojects"), production_tab_button_group_b>> vhelper(production_tab_button_group, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_factories_text")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_factories_text, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_factories_text, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_invest_text")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_invest_text, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_invest_text, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_goodsproduction_text")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_goodsproduction_text, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_goodsproduction_text, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("tab_popprojects_text")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_popprojects_text, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(tab_popprojects_text, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("investment_browser")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<investment_pane> vhelper(investment_browser, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<investment_pane> vhelper(investment_browser, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_state")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_state_button>> vhelper(sort_by_state, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_state_button>> vhelper(sort_by_state, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_projects")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_type_button>> vhelper(sort_by_projects, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_type_button>> vhelper(sort_by_projects, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_completion")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_completion_button>> vhelper(sort_by_completion, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_completion_button>> vhelper(sort_by_completion, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("sort_by_projecteers")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_investors_button>> vhelper(sort_by_projecteers, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<sort_by_project_investors_button>> vhelper(sort_by_projecteers, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("factory_buttons")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<factory_filter_buttons> vhelper(factory_buttons, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<factory_filter_buttons> vhelper(factory_buttons, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("invest_buttons")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<investment_filter_buttons> vhelper(invest_buttons, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<investment_filter_buttons> vhelper(invest_buttons, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("state_listbox")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<state_details_lb, state_window, nations::state_tag>> vhelper(state_listbox, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<state_details_lb, state_window, nations::state_tag>> vhelper(state_listbox, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("state_listbox_invest")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag>> vhelper(state_listbox_invest, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag>> vhelper(state_listbox_invest, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("good_production")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<production_info_pane> vhelper(good_production, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<production_info_pane> vhelper(good_production, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("project_listbox")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<projects_lb, project_item, nations::state_tag>> vhelper(project_listbox, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<projects_lb, project_item, nations::state_tag>> vhelper(project_listbox, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<production_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<close_button_s>, production_window_t_internal_class&, world_state&>)
			 close_button.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button_group_member, production_window_t_internal_class&, world_state&>)
			 tab_factories.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button_group_member, production_window_t_internal_class&, world_state&>)
			 tab_invest.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button_group_member, production_window_t_internal_class&, world_state&>)
			 tab_goodsproduction.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button_group_member, production_window_t_internal_class&, world_state&>)
			 tab_popprojects.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button_group<CT_STRING("tab_factories"), CT_STRING("tab_invest"), CT_STRING("tab_goodsproduction"), CT_STRING("tab_popprojects"), production_tab_button_group_b>, production_window_t_internal_class&, world_state&>)
			 production_tab_button_group.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>)
			 tab_factories_text.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>)
			 tab_invest_text.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>)
			 tab_goodsproduction_text.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<empty_text_box>, production_window_t_internal_class&, world_state&>)
			 tab_popprojects_text.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<investment_pane, production_window_t_internal_class&, world_state&>)
			 investment_browser.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_project_state_button>, production_window_t_internal_class&, world_state&>)
			 sort_by_state.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_project_type_button>, production_window_t_internal_class&, world_state&>)
			 sort_by_projects.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_project_completion_button>, production_window_t_internal_class&, world_state&>)
			 sort_by_completion.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<sort_by_project_investors_button>, production_window_t_internal_class&, world_state&>)
			 sort_by_projecteers.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<factory_filter_buttons, production_window_t_internal_class&, world_state&>)
			 factory_buttons.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<investment_filter_buttons, production_window_t_internal_class&, world_state&>)
			 invest_buttons.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<state_details_lb, state_window, nations::state_tag>, production_window_t_internal_class&, world_state&>)
			 state_listbox.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<investment_state_details_lb, state_window, nations::state_tag>, production_window_t_internal_class&, world_state&>)
			 state_listbox_invest.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<production_info_pane, production_window_t_internal_class&, world_state&>)
			 good_production.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<projects_lb, project_item, nations::state_tag>, production_window_t_internal_class&, world_state&>)
			 project_listbox.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 production_window_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<production_window_base, world_state&>) {
			 production_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<production_window_base, production_window_t_internal_class&, world_state&>) {
			 production_window_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class production_window_t : public production_window_t_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, production_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, production_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class factory_worker_item_internal_class : public factory_worker_item_base {
public:
	 ui::dynamic_icon<factory_worker_item_icon> pop_icon;
	 ui::display_text<empty_text_box> pop_amount;
	 ui::display_text<factory_worker_item_amount> pop_amount_2;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_icon"),index > ,ui::dynamic_icon<factory_worker_item_icon>&> get() {
		 return pop_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_icon"),index > ,ui::dynamic_icon<factory_worker_item_icon> const&> get() const {
		 return pop_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount"),index > ,ui::display_text<empty_text_box>&> get() {
		 return pop_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount"),index > ,ui::display_text<empty_text_box> const&> get() const {
		 return pop_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount_2"),index > ,ui::display_text<factory_worker_item_amount>&> get() {
		 return pop_amount_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("pop_amount_2"),index > ,ui::display_text<factory_worker_item_amount> const&> get() const {
		 return pop_amount_2;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<factory_worker_item_base, world_state&>) {
			 factory_worker_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_worker_item_base, factory_worker_item_internal_class&, world_state&>) {
			 factory_worker_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<factory_worker_item_base, world_state&>) 
			 factory_worker_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_worker_item_icon>, factory_worker_item_internal_class&, world_state&>) 
			 pop_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<empty_text_box>, factory_worker_item_internal_class&, world_state&>) 
			 pop_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_worker_item_amount>, factory_worker_item_internal_class&, world_state&>) 
			 pop_amount_2.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 factory_worker_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("pop_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_worker_item_icon>> vhelper(pop_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_worker_item_icon>> vhelper(pop_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(pop_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<empty_text_box>> vhelper(pop_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("pop_amount_2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_worker_item_amount>> vhelper(pop_amount_2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_worker_item_amount>> vhelper(pop_amount_2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<factory_worker_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_worker_item_icon>, factory_worker_item_internal_class&, world_state&>)
			 pop_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<empty_text_box>, factory_worker_item_internal_class&, world_state&>)
			 pop_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_worker_item_amount>, factory_worker_item_internal_class&, world_state&>)
			 pop_amount_2.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 factory_worker_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<factory_worker_item_base, world_state&>) {
			 factory_worker_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_worker_item_base, factory_worker_item_internal_class&, world_state&>) {
			 factory_worker_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class factory_worker_item : public factory_worker_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_worker_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_worker_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class factory_construction_cost_item_internal_class : public factory_construction_cost_item_base {
public:
	 ui::dynamic_icon<factory_construction_cost_item_icon> goods_icon;
	 ui::display_text<factory_construction_cost_item_cost> goods_cost;
	 ui::display_text<factory_construction_cost_item_stockpile_amount> you_have;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_icon"),index > ,ui::dynamic_icon<factory_construction_cost_item_icon>&> get() {
		 return goods_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_icon"),index > ,ui::dynamic_icon<factory_construction_cost_item_icon> const&> get() const {
		 return goods_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_cost"),index > ,ui::display_text<factory_construction_cost_item_cost>&> get() {
		 return goods_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_cost"),index > ,ui::display_text<factory_construction_cost_item_cost> const&> get() const {
		 return goods_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("you_have"),index > ,ui::display_text<factory_construction_cost_item_stockpile_amount>&> get() {
		 return you_have;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("you_have"),index > ,ui::display_text<factory_construction_cost_item_stockpile_amount> const&> get() const {
		 return you_have;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<factory_construction_cost_item_base, world_state&>) {
			 factory_construction_cost_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_construction_cost_item_base, factory_construction_cost_item_internal_class&, world_state&>) {
			 factory_construction_cost_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<factory_construction_cost_item_base, world_state&>) 
			 factory_construction_cost_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_construction_cost_item_icon>, factory_construction_cost_item_internal_class&, world_state&>) 
			 goods_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_construction_cost_item_cost>, factory_construction_cost_item_internal_class&, world_state&>) 
			 goods_cost.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_construction_cost_item_stockpile_amount>, factory_construction_cost_item_internal_class&, world_state&>) 
			 you_have.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 factory_construction_cost_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("goods_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_construction_cost_item_icon>> vhelper(goods_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_construction_cost_item_icon>> vhelper(goods_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("goods_cost")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_construction_cost_item_cost>> vhelper(goods_cost, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_construction_cost_item_cost>> vhelper(goods_cost, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("you_have")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_construction_cost_item_stockpile_amount>> vhelper(you_have, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_construction_cost_item_stockpile_amount>> vhelper(you_have, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<factory_construction_cost_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_construction_cost_item_icon>, factory_construction_cost_item_internal_class&, world_state&>)
			 goods_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_construction_cost_item_cost>, factory_construction_cost_item_internal_class&, world_state&>)
			 goods_cost.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_construction_cost_item_stockpile_amount>, factory_construction_cost_item_internal_class&, world_state&>)
			 you_have.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 factory_construction_cost_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<factory_construction_cost_item_base, world_state&>) {
			 factory_construction_cost_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_construction_cost_item_base, factory_construction_cost_item_internal_class&, world_state&>) {
			 factory_construction_cost_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class factory_construction_cost_item : public factory_construction_cost_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_construction_cost_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_construction_cost_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class factory_type_item_internal_class : public factory_type_item_base {
public:
	 ui::simple_button<factory_item_bg> bg;
	 ui::dynamic_icon<factory_item_icon> output;
	 ui::display_text<factory_item_name> name;
	 ui::display_text<factory_item_cost> total_build_cost;
	 ui::display_text<factory_item_time> buildtime;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("bg"),index > ,ui::simple_button<factory_item_bg>&> get() {
		 return bg;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("bg"),index > ,ui::simple_button<factory_item_bg> const&> get() const {
		 return bg;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::dynamic_icon<factory_item_icon>&> get() {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::dynamic_icon<factory_item_icon> const&> get() const {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("name"),index > ,ui::display_text<factory_item_name>&> get() {
		 return name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("name"),index > ,ui::display_text<factory_item_name> const&> get() const {
		 return name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("total_build_cost"),index > ,ui::display_text<factory_item_cost>&> get() {
		 return total_build_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("total_build_cost"),index > ,ui::display_text<factory_item_cost> const&> get() const {
		 return total_build_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("buildtime"),index > ,ui::display_text<factory_item_time>&> get() {
		 return buildtime;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("buildtime"),index > ,ui::display_text<factory_item_time> const&> get() const {
		 return buildtime;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<factory_type_item_base, world_state&>) {
			 factory_type_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_type_item_base, factory_type_item_internal_class&, world_state&>) {
			 factory_type_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<factory_type_item_base, world_state&>) 
			 factory_type_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<factory_item_bg>, factory_type_item_internal_class&, world_state&>) 
			 bg.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<factory_item_icon>, factory_type_item_internal_class&, world_state&>) 
			 output.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_item_name>, factory_type_item_internal_class&, world_state&>) 
			 name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_item_cost>, factory_type_item_internal_class&, world_state&>) 
			 total_build_cost.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<factory_item_time>, factory_type_item_internal_class&, world_state&>) 
			 buildtime.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 factory_type_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("bg")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<factory_item_bg>> vhelper(bg, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<factory_item_bg>> vhelper(bg, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_item_icon>> vhelper(output, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<factory_item_icon>> vhelper(output, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_item_name>> vhelper(name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_item_name>> vhelper(name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("total_build_cost")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_item_cost>> vhelper(total_build_cost, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_item_cost>> vhelper(total_build_cost, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("buildtime")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<factory_item_time>> vhelper(buildtime, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<factory_item_time>> vhelper(buildtime, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<factory_type_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<factory_item_bg>, factory_type_item_internal_class&, world_state&>)
			 bg.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<factory_item_icon>, factory_type_item_internal_class&, world_state&>)
			 output.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_item_name>, factory_type_item_internal_class&, world_state&>)
			 name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_item_cost>, factory_type_item_internal_class&, world_state&>)
			 total_build_cost.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<factory_item_time>, factory_type_item_internal_class&, world_state&>)
			 buildtime.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 factory_type_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<factory_type_item_base, world_state&>) {
			 factory_type_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<factory_type_item_base, factory_type_item_internal_class&, world_state&>) {
			 factory_type_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class factory_type_item : public factory_type_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_type_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_type_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class build_factory_window_t_internal_class : public build_factory_window_base {
public:
	 ui::display_text<bf_state_name, 8> state_name;
	 ui::display_text<bf_input_amount<0>> input_0_amount;
	 ui::display_text<bf_input_amount<1>> input_1_amount;
	 ui::display_text<bf_input_amount<2>> input_2_amount;
	 ui::display_text<bf_input_amount<3>> input_3_amount;
	 ui::dynamic_icon<bf_output_icon> output;
	 ui::display_text<bf_factory_name> building_name;
	 ui::discrete_listbox<factory_types_lb, factory_type_item, economy::goods_tag> factory_type;
	 ui::display_text<bf_profit_label> output_label;
	 ui::display_text<bf_profit_amount> output_amount;
	 ui::dynamic_icon<bf_input_icon<0>> input_0;
	 ui::dynamic_icon<bf_input_icon<1>> input_1;
	 ui::dynamic_icon<bf_input_icon<2>> input_2;
	 ui::dynamic_icon<bf_input_icon<3>> input_3;
	 ui::multiline_text<bf_description> description_text;
	 ui::display_text<bf_total_workers_amount> needed_workers_count;
	 ui::discrete_listbox<factory_workers_lb, factory_worker_item, factory_worker_value> employment_info;
	 ui::discrete_listbox<factory_construction_costs_lb, factory_construction_cost_item, factory_construction_cost_value> goods_cost_info;
	 ui::display_text<bf_base_price_label> base_price_label;
	 ui::display_text<bf_input_price_label> input_price_label;
	 ui::display_text<bf_total_label> total_label;
	 ui::display_text<bf_base_price_amount> base_price;
	 ui::display_text<bf_input_price_amount> input_price;
	 ui::display_text<bf_total_amount> total_price;
	 ui::display_text<bf_treasury_amount> you_have;
	 ui::display_text<bf_stockpile_lack_label> prod_label;
	 ui::display_text<bf_stockpile_lack_amount> prod_cost;
	 ui::simple_button<bf_cancel> Cancel;
	 ui::simple_button<bf_build> Build;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_name"),index > ,ui::display_text<bf_state_name, 8>&> get() {
		 return state_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("state_name"),index > ,ui::display_text<bf_state_name, 8> const&> get() const {
		 return state_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0_amount"),index > ,ui::display_text<bf_input_amount<0>>&> get() {
		 return input_0_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0_amount"),index > ,ui::display_text<bf_input_amount<0>> const&> get() const {
		 return input_0_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1_amount"),index > ,ui::display_text<bf_input_amount<1>>&> get() {
		 return input_1_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1_amount"),index > ,ui::display_text<bf_input_amount<1>> const&> get() const {
		 return input_1_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2_amount"),index > ,ui::display_text<bf_input_amount<2>>&> get() {
		 return input_2_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2_amount"),index > ,ui::display_text<bf_input_amount<2>> const&> get() const {
		 return input_2_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3_amount"),index > ,ui::display_text<bf_input_amount<3>>&> get() {
		 return input_3_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3_amount"),index > ,ui::display_text<bf_input_amount<3>> const&> get() const {
		 return input_3_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::dynamic_icon<bf_output_icon>&> get() {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output"),index > ,ui::dynamic_icon<bf_output_icon> const&> get() const {
		 return output;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("building_name"),index > ,ui::display_text<bf_factory_name>&> get() {
		 return building_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("building_name"),index > ,ui::display_text<bf_factory_name> const&> get() const {
		 return building_name;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_type"),index > ,ui::discrete_listbox<factory_types_lb, factory_type_item, economy::goods_tag>&> get() {
		 return factory_type;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("factory_type"),index > ,ui::discrete_listbox<factory_types_lb, factory_type_item, economy::goods_tag> const&> get() const {
		 return factory_type;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_label"),index > ,ui::display_text<bf_profit_label>&> get() {
		 return output_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_label"),index > ,ui::display_text<bf_profit_label> const&> get() const {
		 return output_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_amount"),index > ,ui::display_text<bf_profit_amount>&> get() {
		 return output_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("output_amount"),index > ,ui::display_text<bf_profit_amount> const&> get() const {
		 return output_amount;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0"),index > ,ui::dynamic_icon<bf_input_icon<0>>&> get() {
		 return input_0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_0"),index > ,ui::dynamic_icon<bf_input_icon<0>> const&> get() const {
		 return input_0;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1"),index > ,ui::dynamic_icon<bf_input_icon<1>>&> get() {
		 return input_1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_1"),index > ,ui::dynamic_icon<bf_input_icon<1>> const&> get() const {
		 return input_1;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2"),index > ,ui::dynamic_icon<bf_input_icon<2>>&> get() {
		 return input_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_2"),index > ,ui::dynamic_icon<bf_input_icon<2>> const&> get() const {
		 return input_2;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3"),index > ,ui::dynamic_icon<bf_input_icon<3>>&> get() {
		 return input_3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_3"),index > ,ui::dynamic_icon<bf_input_icon<3>> const&> get() const {
		 return input_3;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("description_text"),index > ,ui::multiline_text<bf_description>&> get() {
		 return description_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("description_text"),index > ,ui::multiline_text<bf_description> const&> get() const {
		 return description_text;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("needed_workers_count"),index > ,ui::display_text<bf_total_workers_amount>&> get() {
		 return needed_workers_count;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("needed_workers_count"),index > ,ui::display_text<bf_total_workers_amount> const&> get() const {
		 return needed_workers_count;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("employment_info"),index > ,ui::discrete_listbox<factory_workers_lb, factory_worker_item, factory_worker_value>&> get() {
		 return employment_info;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("employment_info"),index > ,ui::discrete_listbox<factory_workers_lb, factory_worker_item, factory_worker_value> const&> get() const {
		 return employment_info;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_cost_info"),index > ,ui::discrete_listbox<factory_construction_costs_lb, factory_construction_cost_item, factory_construction_cost_value>&> get() {
		 return goods_cost_info;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_cost_info"),index > ,ui::discrete_listbox<factory_construction_costs_lb, factory_construction_cost_item, factory_construction_cost_value> const&> get() const {
		 return goods_cost_info;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("base_price_label"),index > ,ui::display_text<bf_base_price_label>&> get() {
		 return base_price_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("base_price_label"),index > ,ui::display_text<bf_base_price_label> const&> get() const {
		 return base_price_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_price_label"),index > ,ui::display_text<bf_input_price_label>&> get() {
		 return input_price_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_price_label"),index > ,ui::display_text<bf_input_price_label> const&> get() const {
		 return input_price_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("total_label"),index > ,ui::display_text<bf_total_label>&> get() {
		 return total_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("total_label"),index > ,ui::display_text<bf_total_label> const&> get() const {
		 return total_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("base_price"),index > ,ui::display_text<bf_base_price_amount>&> get() {
		 return base_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("base_price"),index > ,ui::display_text<bf_base_price_amount> const&> get() const {
		 return base_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_price"),index > ,ui::display_text<bf_input_price_amount>&> get() {
		 return input_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("input_price"),index > ,ui::display_text<bf_input_price_amount> const&> get() const {
		 return input_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("total_price"),index > ,ui::display_text<bf_total_amount>&> get() {
		 return total_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("total_price"),index > ,ui::display_text<bf_total_amount> const&> get() const {
		 return total_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("you_have"),index > ,ui::display_text<bf_treasury_amount>&> get() {
		 return you_have;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("you_have"),index > ,ui::display_text<bf_treasury_amount> const&> get() const {
		 return you_have;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_label"),index > ,ui::display_text<bf_stockpile_lack_label>&> get() {
		 return prod_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_label"),index > ,ui::display_text<bf_stockpile_lack_label> const&> get() const {
		 return prod_label;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_cost"),index > ,ui::display_text<bf_stockpile_lack_amount>&> get() {
		 return prod_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("prod_cost"),index > ,ui::display_text<bf_stockpile_lack_amount> const&> get() const {
		 return prod_cost;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Cancel"),index > ,ui::simple_button<bf_cancel>&> get() {
		 return Cancel;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Cancel"),index > ,ui::simple_button<bf_cancel> const&> get() const {
		 return Cancel;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Build"),index > ,ui::simple_button<bf_build>&> get() {
		 return Build;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Build"),index > ,ui::simple_button<bf_build> const&> get() const {
		 return Build;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<build_factory_window_base, world_state&>) {
			 build_factory_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<build_factory_window_base, build_factory_window_t_internal_class&, world_state&>) {
			 build_factory_window_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<build_factory_window_base, world_state&>) 
			 build_factory_window_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_state_name, 8>, build_factory_window_t_internal_class&, world_state&>) 
			 state_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_input_amount<0>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_0_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_input_amount<1>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_1_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_input_amount<2>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_2_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_input_amount<3>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_3_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<bf_output_icon>, build_factory_window_t_internal_class&, world_state&>) 
			 output.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_factory_name>, build_factory_window_t_internal_class&, world_state&>) 
			 building_name.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<factory_types_lb, factory_type_item, economy::goods_tag>, build_factory_window_t_internal_class&, world_state&>) 
			 factory_type.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_profit_label>, build_factory_window_t_internal_class&, world_state&>) 
			 output_label.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_profit_amount>, build_factory_window_t_internal_class&, world_state&>) 
			 output_amount.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<bf_input_icon<0>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_0.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<bf_input_icon<1>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_1.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<bf_input_icon<2>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_2.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<bf_input_icon<3>>, build_factory_window_t_internal_class&, world_state&>) 
			 input_3.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::multiline_text<bf_description>, build_factory_window_t_internal_class&, world_state&>) 
			 description_text.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_total_workers_amount>, build_factory_window_t_internal_class&, world_state&>) 
			 needed_workers_count.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<factory_workers_lb, factory_worker_item, factory_worker_value>, build_factory_window_t_internal_class&, world_state&>) 
			 employment_info.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<factory_construction_costs_lb, factory_construction_cost_item, factory_construction_cost_value>, build_factory_window_t_internal_class&, world_state&>) 
			 goods_cost_info.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_base_price_label>, build_factory_window_t_internal_class&, world_state&>) 
			 base_price_label.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_input_price_label>, build_factory_window_t_internal_class&, world_state&>) 
			 input_price_label.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_total_label>, build_factory_window_t_internal_class&, world_state&>) 
			 total_label.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_base_price_amount>, build_factory_window_t_internal_class&, world_state&>) 
			 base_price.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_input_price_amount>, build_factory_window_t_internal_class&, world_state&>) 
			 input_price.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_total_amount>, build_factory_window_t_internal_class&, world_state&>) 
			 total_price.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_treasury_amount>, build_factory_window_t_internal_class&, world_state&>) 
			 you_have.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_stockpile_lack_label>, build_factory_window_t_internal_class&, world_state&>) 
			 prod_label.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<bf_stockpile_lack_amount>, build_factory_window_t_internal_class&, world_state&>) 
			 prod_cost.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<bf_cancel>, build_factory_window_t_internal_class&, world_state&>) 
			 Cancel.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<bf_build>, build_factory_window_t_internal_class&, world_state&>) 
			 Build.windowed_update(*this, ws);
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::window_def const& definition) {
		 const auto window = ws.w.gui_m.gui_objects.emplace();
		 window.object.align = alignment_from_definition(definition);
		 if (is_valid_index(definition.background_handle)) {
			 const auto& bgdefinition = ws.s.gui_m.ui_definitions.buttons[definition.background_handle];
			 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, window, bgdefinition.graphical_object_handle);
		 } else {
			 window.object.type_dependant_handle.store(0, std::memory_order_release);
		 }
		 window.object.associated_behavior = this;
		 build_factory_window_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("state_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_state_name, 8>> vhelper(state_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_state_name, 8>> vhelper(state_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_0_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<0>>> vhelper(input_0_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<0>>> vhelper(input_0_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_1_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<1>>> vhelper(input_1_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<1>>> vhelper(input_1_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_2_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<2>>> vhelper(input_2_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<2>>> vhelper(input_2_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_3_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<3>>> vhelper(input_3_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_input_amount<3>>> vhelper(input_3_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_output_icon>> vhelper(output, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_output_icon>> vhelper(output, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("building_name")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_factory_name>> vhelper(building_name, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_factory_name>> vhelper(building_name, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("factory_type")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<factory_types_lb, factory_type_item, economy::goods_tag>> vhelper(factory_type, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<factory_types_lb, factory_type_item, economy::goods_tag>> vhelper(factory_type, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output_label")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_profit_label>> vhelper(output_label, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_profit_label>> vhelper(output_label, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("output_amount")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_profit_amount>> vhelper(output_amount, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_profit_amount>> vhelper(output_amount, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_0")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<0>>> vhelper(input_0, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<0>>> vhelper(input_0, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_1")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<1>>> vhelper(input_1, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<1>>> vhelper(input_1, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_2")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<2>>> vhelper(input_2, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<2>>> vhelper(input_2, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_3")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<3>>> vhelper(input_3, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<bf_input_icon<3>>> vhelper(input_3, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("description_text")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::multiline_text<bf_description>> vhelper(description_text, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::multiline_text<bf_description>> vhelper(description_text, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("needed_workers_count")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_total_workers_amount>> vhelper(needed_workers_count, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_total_workers_amount>> vhelper(needed_workers_count, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("employment_info")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<factory_workers_lb, factory_worker_item, factory_worker_value>> vhelper(employment_info, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<factory_workers_lb, factory_worker_item, factory_worker_value>> vhelper(employment_info, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("goods_cost_info")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<factory_construction_costs_lb, factory_construction_cost_item, factory_construction_cost_value>> vhelper(goods_cost_info, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<factory_construction_costs_lb, factory_construction_cost_item, factory_construction_cost_value>> vhelper(goods_cost_info, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("base_price_label")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_base_price_label>> vhelper(base_price_label, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_base_price_label>> vhelper(base_price_label, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_price_label")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_input_price_label>> vhelper(input_price_label, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_input_price_label>> vhelper(input_price_label, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("total_label")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_total_label>> vhelper(total_label, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_total_label>> vhelper(total_label, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("base_price")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_base_price_amount>> vhelper(base_price, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_base_price_amount>> vhelper(base_price, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("input_price")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_input_price_amount>> vhelper(input_price, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_input_price_amount>> vhelper(input_price, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("total_price")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_total_amount>> vhelper(total_price, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_total_amount>> vhelper(total_price, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("you_have")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_treasury_amount>> vhelper(you_have, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_treasury_amount>> vhelper(you_have, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_label")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_stockpile_lack_label>> vhelper(prod_label, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_stockpile_lack_label>> vhelper(prod_label, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("prod_cost")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<bf_stockpile_lack_amount>> vhelper(prod_cost, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<bf_stockpile_lack_amount>> vhelper(prod_cost, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("Cancel")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<bf_cancel>> vhelper(Cancel, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<bf_cancel>> vhelper(Cancel, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("Build")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<bf_build>> vhelper(Build, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<bf_build>> vhelper(Build, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<build_factory_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_state_name, 8>, build_factory_window_t_internal_class&, world_state&>)
			 state_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_input_amount<0>>, build_factory_window_t_internal_class&, world_state&>)
			 input_0_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_input_amount<1>>, build_factory_window_t_internal_class&, world_state&>)
			 input_1_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_input_amount<2>>, build_factory_window_t_internal_class&, world_state&>)
			 input_2_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_input_amount<3>>, build_factory_window_t_internal_class&, world_state&>)
			 input_3_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<bf_output_icon>, build_factory_window_t_internal_class&, world_state&>)
			 output.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_factory_name>, build_factory_window_t_internal_class&, world_state&>)
			 building_name.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<factory_types_lb, factory_type_item, economy::goods_tag>, build_factory_window_t_internal_class&, world_state&>)
			 factory_type.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_profit_label>, build_factory_window_t_internal_class&, world_state&>)
			 output_label.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_profit_amount>, build_factory_window_t_internal_class&, world_state&>)
			 output_amount.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<bf_input_icon<0>>, build_factory_window_t_internal_class&, world_state&>)
			 input_0.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<bf_input_icon<1>>, build_factory_window_t_internal_class&, world_state&>)
			 input_1.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<bf_input_icon<2>>, build_factory_window_t_internal_class&, world_state&>)
			 input_2.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<bf_input_icon<3>>, build_factory_window_t_internal_class&, world_state&>)
			 input_3.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::multiline_text<bf_description>, build_factory_window_t_internal_class&, world_state&>)
			 description_text.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_total_workers_amount>, build_factory_window_t_internal_class&, world_state&>)
			 needed_workers_count.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<factory_workers_lb, factory_worker_item, factory_worker_value>, build_factory_window_t_internal_class&, world_state&>)
			 employment_info.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<factory_construction_costs_lb, factory_construction_cost_item, factory_construction_cost_value>, build_factory_window_t_internal_class&, world_state&>)
			 goods_cost_info.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_base_price_label>, build_factory_window_t_internal_class&, world_state&>)
			 base_price_label.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_input_price_label>, build_factory_window_t_internal_class&, world_state&>)
			 input_price_label.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_total_label>, build_factory_window_t_internal_class&, world_state&>)
			 total_label.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_base_price_amount>, build_factory_window_t_internal_class&, world_state&>)
			 base_price.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_input_price_amount>, build_factory_window_t_internal_class&, world_state&>)
			 input_price.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_total_amount>, build_factory_window_t_internal_class&, world_state&>)
			 total_price.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_treasury_amount>, build_factory_window_t_internal_class&, world_state&>)
			 you_have.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_stockpile_lack_label>, build_factory_window_t_internal_class&, world_state&>)
			 prod_label.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<bf_stockpile_lack_amount>, build_factory_window_t_internal_class&, world_state&>)
			 prod_cost.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<bf_cancel>, build_factory_window_t_internal_class&, world_state&>)
			 Cancel.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<bf_build>, build_factory_window_t_internal_class&, world_state&>)
			 Build.initialize_in_window(*this, ws);
		 return window;
	 }
	 ui::tagged_gui_object create_gui_obj(world_state& ws, ui::icon_def const& icon_def) {
		 const auto new_gobj = ws.w.gui_m.gui_objects.emplace();
		 const uint16_t rotation =
			 (icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_upright ?
			 ui::gui_object::rotation_upright :
			 ((icon_def.flags & ui::icon_def::rotation_mask) == ui::icon_def::rotation_90_right ? ui::gui_object::rotation_right : ui::gui_object::rotation_left);
		 new_gobj.object.position = icon_def.position;
		 new_gobj.object.flags.fetch_or(rotation, std::memory_order_acq_rel);
		 new_gobj.object.align = alignment_from_definition(icon_def);
		 ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, icon_def.frame != 0 ? int32_t(icon_def.frame) - 1 : 0);
		 if(rotation == ui::gui_object::rotation_right) {
			 new_gobj.object.position = ui::xy_pair{
				 int16_t(new_gobj.object.position.x - new_gobj.object.size.y),
				 int16_t(new_gobj.object.position.y + new_gobj.object.size.y - new_gobj.object.size.x) };
			 new_gobj.object.size = ui::xy_pair{ new_gobj.object.size.y, new_gobj.object.size.x };
		 }
		 new_gobj.object.size.x = int16_t(float(new_gobj.object.size.x) * icon_def.scale);
		 new_gobj.object.size.y = int16_t(float(new_gobj.object.size.y) * icon_def.scale);
		 new_gobj.object.associated_behavior = this;
		 build_factory_window_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<build_factory_window_base, world_state&>) {
			 build_factory_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<build_factory_window_base, build_factory_window_t_internal_class&, world_state&>) {
			 build_factory_window_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class build_factory_window_t : public build_factory_window_t_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, build_factory_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, build_factory_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
#pragma warning( pop )

