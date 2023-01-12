#pragma warning( push )
#pragma warning( disable : 4189 )

class tw_good_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, tw_good_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, tw_good_item& b);
class trade_details_pane;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, trade_details_pane& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, trade_details_pane& b);
class trade_window_t;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, trade_window_t& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, trade_window_t& b);

class trade_window_base : public ui::draggable_region {
public:

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class tw_close_button {
public:

	 void button_function(ui::simple_button<tw_close_button>& self, world_state& ws);
};
class tw_good_item_base : public ui::visible_region {
public:
	 goods_tag tag;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class tw_good_item_background {
public:
	 goods_tag tag;

	 template<typename window_type>
	 void windowed_update(ui::simple_button<tw_good_item_background>& self, window_type& w, world_state& ws);
	 bool has_tooltip(world_state&) { return true; }
	 void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	 void button_function(ui::simple_button<tw_good_item_background>& self, world_state& ws);
};
class tw_good_item_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::simple_button<tw_good_item_icon>& self, window_type& w, world_state& ws);
};
class tw_good_item_price {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class tw_good_item_trend_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<tw_good_item_trend_icon>& self, window_type& w, world_state& ws);
};
class tw_good_item_buying_selling_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<tw_good_item_buying_selling_icon>& self, window_type& w, world_state& ws);
};
class tw_good_item_automation_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<tw_good_item_automation_icon>& self, window_type& w, world_state& ws);
};
template<typename = void>
class tw_good_item_internal_class : public tw_good_item_base {
public:
	 ui::simple_button<tw_good_item_background> entry_button;
	 ui::simple_button<tw_good_item_icon> goods_type;
	 ui::display_text<tw_good_item_price> price;
	 ui::dynamic_icon<tw_good_item_trend_icon> trend_indicator;
	 ui::dynamic_icon<tw_good_item_buying_selling_icon> selling_indicator;
	 ui::dynamic_icon<tw_good_item_automation_icon> automation_indicator;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("entry_button"),index > ,ui::simple_button<tw_good_item_background>&> get() {
		 return entry_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("entry_button"),index > ,ui::simple_button<tw_good_item_background> const&> get() const {
		 return entry_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_type"),index > ,ui::simple_button<tw_good_item_icon>&> get() {
		 return goods_type;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_type"),index > ,ui::simple_button<tw_good_item_icon> const&> get() const {
		 return goods_type;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price"),index > ,ui::display_text<tw_good_item_price>&> get() {
		 return price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price"),index > ,ui::display_text<tw_good_item_price> const&> get() const {
		 return price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("trend_indicator"),index > ,ui::dynamic_icon<tw_good_item_trend_icon>&> get() {
		 return trend_indicator;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("trend_indicator"),index > ,ui::dynamic_icon<tw_good_item_trend_icon> const&> get() const {
		 return trend_indicator;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("selling_indicator"),index > ,ui::dynamic_icon<tw_good_item_buying_selling_icon>&> get() {
		 return selling_indicator;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("selling_indicator"),index > ,ui::dynamic_icon<tw_good_item_buying_selling_icon> const&> get() const {
		 return selling_indicator;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("automation_indicator"),index > ,ui::dynamic_icon<tw_good_item_automation_icon>&> get() {
		 return automation_indicator;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("automation_indicator"),index > ,ui::dynamic_icon<tw_good_item_automation_icon> const&> get() const {
		 return automation_indicator;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<tw_good_item_base, world_state&>) {
			 tw_good_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<tw_good_item_base, tw_good_item_internal_class&, world_state&>) {
			 tw_good_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<tw_good_item_base, world_state&>) 
			 tw_good_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<tw_good_item_background>, tw_good_item_internal_class&, world_state&>) 
			 entry_button.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<tw_good_item_icon>, tw_good_item_internal_class&, world_state&>) 
			 goods_type.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<tw_good_item_price>, tw_good_item_internal_class&, world_state&>) 
			 price.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<tw_good_item_trend_icon>, tw_good_item_internal_class&, world_state&>) 
			 trend_indicator.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<tw_good_item_buying_selling_icon>, tw_good_item_internal_class&, world_state&>) 
			 selling_indicator.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<tw_good_item_automation_icon>, tw_good_item_internal_class&, world_state&>) 
			 automation_indicator.windowed_update(*this, ws);
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
		 tw_good_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("entry_button")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<tw_good_item_background>> vhelper(entry_button, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<tw_good_item_background>> vhelper(entry_button, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("goods_type")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<tw_good_item_icon>> vhelper(goods_type, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<tw_good_item_icon>> vhelper(goods_type, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("price")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<tw_good_item_price>> vhelper(price, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<tw_good_item_price>> vhelper(price, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("trend_indicator")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_good_item_trend_icon>> vhelper(trend_indicator, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_good_item_trend_icon>> vhelper(trend_indicator, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("selling_indicator")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_good_item_buying_selling_icon>> vhelper(selling_indicator, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_good_item_buying_selling_icon>> vhelper(selling_indicator, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("automation_indicator")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_good_item_automation_icon>> vhelper(automation_indicator, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_good_item_automation_icon>> vhelper(automation_indicator, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<tw_good_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<tw_good_item_background>, tw_good_item_internal_class&, world_state&>)
			 entry_button.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<tw_good_item_icon>, tw_good_item_internal_class&, world_state&>)
			 goods_type.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<tw_good_item_price>, tw_good_item_internal_class&, world_state&>)
			 price.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<tw_good_item_trend_icon>, tw_good_item_internal_class&, world_state&>)
			 trend_indicator.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<tw_good_item_buying_selling_icon>, tw_good_item_internal_class&, world_state&>)
			 selling_indicator.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<tw_good_item_automation_icon>, tw_good_item_internal_class&, world_state&>)
			 automation_indicator.initialize_in_window(*this, ws);
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
		 tw_good_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<tw_good_item_base, world_state&>) {
			 tw_good_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<tw_good_item_base, tw_good_item_internal_class&, world_state&>) {
			 tw_good_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class tw_good_item : public tw_good_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, tw_good_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, tw_good_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
class trade_details_pane_base : public ui::window_pane {
public:

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class tw_selected_good_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<tw_selected_good_icon>& self, window_type& w, world_state& ws);
};
class tw_selected_good_name {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class tw_selected_good_price {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class tw_good_global_max_price {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class tw_good_global_min_price {
public:

	 template<typename W>
	 void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class prices_barchart {
public:

	 template<typename window_type>
	 void windowed_update(ui::display_barchart<prices_barchart>& self, window_type& w, world_state& ws);
};
template<typename = void>
class trade_details_pane_internal_class : public trade_details_pane_base {
public:
	 ui::dynamic_icon<tw_selected_good_icon> goods_icon;
	 ui::display_text<tw_selected_good_name> goods_title;
	 ui::display_text<tw_selected_good_price> goods_price;
	 ui::display_barchart<prices_barchart> price_linechart;
	 ui::display_text<tw_good_global_min_price> price_chart_low;
	 ui::display_text<tw_good_global_max_price> price_chart_high;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_icon"),index > ,ui::dynamic_icon<tw_selected_good_icon>&> get() {
		 return goods_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_icon"),index > ,ui::dynamic_icon<tw_selected_good_icon> const&> get() const {
		 return goods_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_title"),index > ,ui::display_text<tw_selected_good_name>&> get() {
		 return goods_title;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_title"),index > ,ui::display_text<tw_selected_good_name> const&> get() const {
		 return goods_title;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_price"),index > ,ui::display_text<tw_selected_good_price>&> get() {
		 return goods_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("goods_price"),index > ,ui::display_text<tw_selected_good_price> const&> get() const {
		 return goods_price;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price_linechart"),index > ,ui::display_barchart<prices_barchart>&> get() {
		 return price_linechart;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price_linechart"),index > ,ui::display_barchart<prices_barchart> const&> get() const {
		 return price_linechart;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price_chart_low"),index > ,ui::display_text<tw_good_global_min_price>&> get() {
		 return price_chart_low;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price_chart_low"),index > ,ui::display_text<tw_good_global_min_price> const&> get() const {
		 return price_chart_low;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price_chart_high"),index > ,ui::display_text<tw_good_global_max_price>&> get() {
		 return price_chart_high;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("price_chart_high"),index > ,ui::display_text<tw_good_global_max_price> const&> get() const {
		 return price_chart_high;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<trade_details_pane_base, world_state&>) {
			 trade_details_pane_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<trade_details_pane_base, trade_details_pane_internal_class&, world_state&>) {
			 trade_details_pane_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<trade_details_pane_base, world_state&>) 
			 trade_details_pane_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<tw_selected_good_icon>, trade_details_pane_internal_class&, world_state&>) 
			 goods_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<tw_selected_good_name>, trade_details_pane_internal_class&, world_state&>) 
			 goods_title.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<tw_selected_good_price>, trade_details_pane_internal_class&, world_state&>) 
			 goods_price.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_barchart<prices_barchart>, trade_details_pane_internal_class&, world_state&>) 
			 price_linechart.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<tw_good_global_min_price>, trade_details_pane_internal_class&, world_state&>) 
			 price_chart_low.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<tw_good_global_max_price>, trade_details_pane_internal_class&, world_state&>) 
			 price_chart_high.windowed_update(*this, ws);
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
		 trade_details_pane_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("goods_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_selected_good_icon>> vhelper(goods_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<tw_selected_good_icon>> vhelper(goods_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("goods_title")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<tw_selected_good_name>> vhelper(goods_title, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<tw_selected_good_name>> vhelper(goods_title, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("goods_price")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<tw_selected_good_price>> vhelper(goods_price, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<tw_selected_good_price>> vhelper(goods_price, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("price_linechart")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_barchart<prices_barchart>> vhelper(price_linechart, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_barchart<prices_barchart>> vhelper(price_linechart, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("price_chart_low")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<tw_good_global_min_price>> vhelper(price_chart_low, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<tw_good_global_min_price>> vhelper(price_chart_low, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("price_chart_high")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<tw_good_global_max_price>> vhelper(price_chart_high, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<tw_good_global_max_price>> vhelper(price_chart_high, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<trade_details_pane_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<tw_selected_good_icon>, trade_details_pane_internal_class&, world_state&>)
			 goods_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<tw_selected_good_name>, trade_details_pane_internal_class&, world_state&>)
			 goods_title.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<tw_selected_good_price>, trade_details_pane_internal_class&, world_state&>)
			 goods_price.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_barchart<prices_barchart>, trade_details_pane_internal_class&, world_state&>)
			 price_linechart.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<tw_good_global_min_price>, trade_details_pane_internal_class&, world_state&>)
			 price_chart_low.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<tw_good_global_max_price>, trade_details_pane_internal_class&, world_state&>)
			 price_chart_high.initialize_in_window(*this, ws);
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
		 trade_details_pane_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<trade_details_pane_base, world_state&>) {
			 trade_details_pane_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<trade_details_pane_base, trade_details_pane_internal_class&, world_state&>) {
			 trade_details_pane_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class trade_details_pane : public trade_details_pane_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, trade_details_pane& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, trade_details_pane& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class trade_window_t_internal_class : public trade_window_base {
public:
	 ui::simple_button<tw_close_button> close_button;
	 ui::gui_window<goods_group_window_base<group_raw_material_goods_a, sizeof(group_raw_material_goods_a) - 1>> group_raw_material_goods;
	 ui::gui_window<goods_group_window_base<group_industrial_goods_a, sizeof(group_industrial_goods_a) - 1>> group_industrial_goods;
	 ui::gui_window<goods_group_window_base<group_consumer_goods_a, sizeof(group_consumer_goods_a) - 1>> group_consumer_goods;
	 ui::gui_window<goods_group_window_base<group_military_goods_a, sizeof(group_military_goods_a) - 1>> group_military_goods;
	 trade_details_pane trade_details;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("close_button"),index > ,ui::simple_button<tw_close_button>&> get() {
		 return close_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("close_button"),index > ,ui::simple_button<tw_close_button> const&> get() const {
		 return close_button;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_raw_material_goods"),index > ,ui::gui_window<goods_group_window_base<group_raw_material_goods_a, sizeof(group_raw_material_goods_a) - 1>>&> get() {
		 return group_raw_material_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_raw_material_goods"),index > ,ui::gui_window<goods_group_window_base<group_raw_material_goods_a, sizeof(group_raw_material_goods_a) - 1>> const&> get() const {
		 return group_raw_material_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_industrial_goods"),index > ,ui::gui_window<goods_group_window_base<group_industrial_goods_a, sizeof(group_industrial_goods_a) - 1>>&> get() {
		 return group_industrial_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_industrial_goods"),index > ,ui::gui_window<goods_group_window_base<group_industrial_goods_a, sizeof(group_industrial_goods_a) - 1>> const&> get() const {
		 return group_industrial_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_consumer_goods"),index > ,ui::gui_window<goods_group_window_base<group_consumer_goods_a, sizeof(group_consumer_goods_a) - 1>>&> get() {
		 return group_consumer_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_consumer_goods"),index > ,ui::gui_window<goods_group_window_base<group_consumer_goods_a, sizeof(group_consumer_goods_a) - 1>> const&> get() const {
		 return group_consumer_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_military_goods"),index > ,ui::gui_window<goods_group_window_base<group_military_goods_a, sizeof(group_military_goods_a) - 1>>&> get() {
		 return group_military_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("group_military_goods"),index > ,ui::gui_window<goods_group_window_base<group_military_goods_a, sizeof(group_military_goods_a) - 1>> const&> get() const {
		 return group_military_goods;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("trade_details"),index > ,trade_details_pane&> get() {
		 return trade_details;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("trade_details"),index > ,trade_details_pane const&> get() const {
		 return trade_details;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<trade_window_base, world_state&>) {
			 trade_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<trade_window_base, trade_window_t_internal_class&, world_state&>) {
			 trade_window_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<trade_window_base, world_state&>) 
			 trade_window_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<tw_close_button>, trade_window_t_internal_class&, world_state&>) 
			 close_button.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::gui_window<goods_group_window_base<group_raw_material_goods_a, sizeof(group_raw_material_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>) 
			 group_raw_material_goods.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::gui_window<goods_group_window_base<group_industrial_goods_a, sizeof(group_industrial_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>) 
			 group_industrial_goods.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::gui_window<goods_group_window_base<group_consumer_goods_a, sizeof(group_consumer_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>) 
			 group_consumer_goods.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::gui_window<goods_group_window_base<group_military_goods_a, sizeof(group_military_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>) 
			 group_military_goods.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<trade_details_pane, trade_window_t_internal_class&, world_state&>) 
			 trade_details.windowed_update(*this, ws);
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
		 trade_window_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("close_button")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<tw_close_button>> vhelper(close_button, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<tw_close_button>> vhelper(close_button, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("group_raw_material_goods")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_raw_material_goods_a, sizeof(group_raw_material_goods_a) - 1>>> vhelper(group_raw_material_goods, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_raw_material_goods_a, sizeof(group_raw_material_goods_a) - 1>>> vhelper(group_raw_material_goods, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("group_industrial_goods")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_industrial_goods_a, sizeof(group_industrial_goods_a) - 1>>> vhelper(group_industrial_goods, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_industrial_goods_a, sizeof(group_industrial_goods_a) - 1>>> vhelper(group_industrial_goods, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("group_consumer_goods")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_consumer_goods_a, sizeof(group_consumer_goods_a) - 1>>> vhelper(group_consumer_goods, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_consumer_goods_a, sizeof(group_consumer_goods_a) - 1>>> vhelper(group_consumer_goods, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("group_military_goods")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_military_goods_a, sizeof(group_military_goods_a) - 1>>> vhelper(group_military_goods, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::gui_window<goods_group_window_base<group_military_goods_a, sizeof(group_military_goods_a) - 1>>> vhelper(group_military_goods, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("trade_details")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<trade_details_pane> vhelper(trade_details, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<trade_details_pane> vhelper(trade_details, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<trade_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<tw_close_button>, trade_window_t_internal_class&, world_state&>)
			 close_button.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::gui_window<goods_group_window_base<group_raw_material_goods_a, sizeof(group_raw_material_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>)
			 group_raw_material_goods.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::gui_window<goods_group_window_base<group_industrial_goods_a, sizeof(group_industrial_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>)
			 group_industrial_goods.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::gui_window<goods_group_window_base<group_consumer_goods_a, sizeof(group_consumer_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>)
			 group_consumer_goods.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::gui_window<goods_group_window_base<group_military_goods_a, sizeof(group_military_goods_a) - 1>>, trade_window_t_internal_class&, world_state&>)
			 group_military_goods.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<trade_details_pane, trade_window_t_internal_class&, world_state&>)
			 trade_details.initialize_in_window(*this, ws);
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
		 trade_window_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<trade_window_base, world_state&>) {
			 trade_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<trade_window_base, trade_window_t_internal_class&, world_state&>) {
			 trade_window_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class trade_window_t : public trade_window_t_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, trade_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, trade_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
#pragma warning( pop )

