class good_filter_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, good_filter_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, good_filter_item& b);
class gp_investment_subwindow;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, gp_investment_subwindow& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, gp_investment_subwindow& b);
class state_pop_display_window;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, state_pop_display_window& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, state_pop_display_window& b);

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
				 if constexpr(ui::detail::can_create_dynamic<good_filter_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!good_filter_item_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
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
		 good_filter_item_base::associated_object = &new_gobj.object;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<gp_investment_subwindow_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!gp_investment_subwindow_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
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
		 gp_investment_subwindow_base::associated_object = &new_gobj.object;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<state_pop_display_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!state_pop_display_window_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
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
		 state_pop_display_window_base::associated_object = &new_gobj.object;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
