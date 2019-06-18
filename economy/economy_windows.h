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
				 if constexpr(ui::detail::can_create_dynamic<investment_country_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!investment_country_item_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<factory_filter_buttons_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!factory_filter_buttons_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<investment_filter_buttons_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!investment_filter_buttons_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<investment_pane_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!investment_pane_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<state_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!state_window_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<project_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!project_item_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<production_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!production_window_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<factory_worker_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!factory_worker_item_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<factory_construction_cost_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!factory_construction_cost_item_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<factory_type_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!factory_type_item_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<build_factory_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!build_factory_window_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
						 std::visit([&ws, &window](auto tag) {
							 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
								 ui::create_dynamic_element(ws, tag, window);
						 }, *i);
					 }
				 } else {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
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
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
