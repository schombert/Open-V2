#pragma warning( push )
#pragma warning( disable : 4189 )

class cb_item;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, cb_item& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, cb_item& b);
class fabricate_cb_window_t;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, fabricate_cb_window_t& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, fabricate_cb_window_t& b);

class fabricate_cb_types_lb {
public:

	 template<typename lb_type>
	 void populate_list(lb_type& lb, world_state& ws);
	 ui::window_tag element_tag(ui::gui_static& m);
};
class cb_item_base : public ui::visible_region {
public:
	 military::cb_type_tag type;

	 template<typename W>
	 void on_create(W& w, world_state& ws);
	 void set_value(military::cb_type_tag t) {
		 type = t;
	 }
};
class cb_type_icon {
public:

	 template<typename window_type>
	 void windowed_update(ui::dynamic_icon<cb_type_icon>& self, window_type& w, world_state& ws);
};
class cb_type_button {
public:
	 military::cb_type_tag type;

	 template<typename window_type>
	 void windowed_update(ui::button<cb_type_button>& self, window_type& w, world_state& ws);
	 void button_function(ui::button<cb_type_button>& self, world_state& ws);
};
class fabricate_cb_window_base : public ui::draggable_region {
public:

	 template<typename W>
	 void on_create(W& w, world_state& ws);
};
class hidden_button {
public:

	 void update(ui::simple_button<hidden_button>& self, world_state& ws);
};
class self_flag {
public:

	 void update(ui::masked_flag<self_flag>& self, world_state& ws);
};
class target_flag {
public:

	 void update(ui::masked_flag<target_flag>& self, world_state& ws);
};
class ok_button {
public:

	 void update(ui::simple_button<ok_button>& self, world_state& ws);
	 void button_function(ui::simple_button<ok_button>& self, world_state& ws);
};
class cancel_button {
public:

	 void button_function(ui::simple_button<cancel_button>& self, world_state& ws);
};
class dialog_title {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
class description {
public:

	 void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
};
template<typename = void>
class cb_item_internal_class : public cb_item_base {
public:
	 ui::dynamic_icon<cb_type_icon> wargoal_icon;
	 ui::button<cb_type_button> select_cb;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("wargoal_icon"),index > ,ui::dynamic_icon<cb_type_icon>&> get() {
		 return wargoal_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("wargoal_icon"),index > ,ui::dynamic_icon<cb_type_icon> const&> get() const {
		 return wargoal_icon;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("select_cb"),index > ,ui::button<cb_type_button>&> get() {
		 return select_cb;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("select_cb"),index > ,ui::button<cb_type_button> const&> get() const {
		 return select_cb;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<cb_item_base, world_state&>) {
			 cb_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<cb_item_base, cb_item_internal_class&, world_state&>) {
			 cb_item_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<cb_item_base, world_state&>) 
			 cb_item_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::dynamic_icon<cb_type_icon>, cb_item_internal_class&, world_state&>) 
			 wargoal_icon.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::button<cb_type_button>, cb_item_internal_class&, world_state&>) 
			 select_cb.windowed_update(*this, ws);
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
		 cb_item_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("wargoal_icon")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::dynamic_icon<cb_type_icon>> vhelper(wargoal_icon, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::dynamic_icon<cb_type_icon>> vhelper(wargoal_icon, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("select_cb")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::button<cb_type_button>> vhelper(select_cb, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::button<cb_type_button>> vhelper(select_cb, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<cb_item_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::dynamic_icon<cb_type_icon>, cb_item_internal_class&, world_state&>)
			 wargoal_icon.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::button<cb_type_button>, cb_item_internal_class&, world_state&>)
			 select_cb.initialize_in_window(*this, ws);
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
		 cb_item_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<cb_item_base, world_state&>) {
			 cb_item_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<cb_item_base, cb_item_internal_class&, world_state&>) {
			 cb_item_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class cb_item : public cb_item_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, cb_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, cb_item& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
template<typename = void>
class fabricate_cb_window_t_internal_class : public fabricate_cb_window_base {
public:
	 ui::simple_button<hidden_button> Background;
	 ui::simple_button<ok_button> AgreeButton;
	 ui::simple_button<cancel_button> DeclineButton;
	 ui::masked_flag<self_flag> LeftShield;
	 ui::masked_flag<target_flag> RightShield;
	 ui::display_text<dialog_title> Title;
	 ui::multiline_text<description> Description;
	 ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24> cb_list;
	 ui::gui_object_tag window_object;

	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Background"),index > ,ui::simple_button<hidden_button>&> get() {
		 return Background;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Background"),index > ,ui::simple_button<hidden_button> const&> get() const {
		 return Background;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("AgreeButton"),index > ,ui::simple_button<ok_button>&> get() {
		 return AgreeButton;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("AgreeButton"),index > ,ui::simple_button<ok_button> const&> get() const {
		 return AgreeButton;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("DeclineButton"),index > ,ui::simple_button<cancel_button>&> get() {
		 return DeclineButton;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("DeclineButton"),index > ,ui::simple_button<cancel_button> const&> get() const {
		 return DeclineButton;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("LeftShield"),index > ,ui::masked_flag<self_flag>&> get() {
		 return LeftShield;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("LeftShield"),index > ,ui::masked_flag<self_flag> const&> get() const {
		 return LeftShield;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("RightShield"),index > ,ui::masked_flag<target_flag>&> get() {
		 return RightShield;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("RightShield"),index > ,ui::masked_flag<target_flag> const&> get() const {
		 return RightShield;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Title"),index > ,ui::display_text<dialog_title>&> get() {
		 return Title;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Title"),index > ,ui::display_text<dialog_title> const&> get() const {
		 return Title;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Description"),index > ,ui::multiline_text<description>&> get() {
		 return Description;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("Description"),index > ,ui::multiline_text<description> const&> get() const {
		 return Description;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("cb_list"),index > ,ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24>&> get() {
		 return cb_list;
	 }
	 template<typename index>
	 std::enable_if_t<std::is_same_v<CT_STRING("cb_list"),index > ,ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24> const&> get() const {
		 return cb_list;
	 }

	 void on_create(world_state& ws) {
		 if constexpr(ui::detail::has_on_create<fabricate_cb_window_base, world_state&>) {
			 fabricate_cb_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<fabricate_cb_window_base, fabricate_cb_window_t_internal_class&, world_state&>) {
			 fabricate_cb_window_base::on_create(*this, ws);
		 }
	 }
	 virtual void update_data(ui::gui_object_tag, world_state& ws) override {
		 if constexpr(ui::detail::has_update<fabricate_cb_window_base, world_state&>) 
			 fabricate_cb_window_base::update(ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<hidden_button>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 Background.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<ok_button>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 AgreeButton.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::simple_button<cancel_button>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 DeclineButton.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<self_flag>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 LeftShield.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::masked_flag<target_flag>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 RightShield.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::display_text<dialog_title>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 Title.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::multiline_text<description>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 Description.windowed_update(*this, ws);
		 if constexpr(ui::detail::has_windowed_update<ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24>, fabricate_cb_window_t_internal_class&, world_state&>) 
			 cb_list.windowed_update(*this, ws);
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
		 fabricate_cb_window_base::associated_object = &window.object;
		 window.object.size = definition.size;
		 window.object.position = definition.position;
		 for(auto i = definition.sub_object_definitions.crbegin(); i != definition.sub_object_definitions.crend(); ++i) {
			 auto rn = ws.s.gui_m.nmaps.get_raw_name(*i);
			 const char* rn_s = rn.get_str(ws.s.gui_m.ui_definitions.name_data);
			 const char* rn_e = rn_s + rn.length();
			 if(compile_time_str_compare_ci<CT_STRING("Background")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<hidden_button>> vhelper(Background, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<hidden_button>> vhelper(Background, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("AgreeButton")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<ok_button>> vhelper(AgreeButton, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<ok_button>> vhelper(AgreeButton, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("DeclineButton")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::simple_button<cancel_button>> vhelper(DeclineButton, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::simple_button<cancel_button>> vhelper(DeclineButton, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("LeftShield")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<self_flag>> vhelper(LeftShield, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<self_flag>> vhelper(LeftShield, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("RightShield")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::masked_flag<target_flag>> vhelper(RightShield, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::masked_flag<target_flag>> vhelper(RightShield, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("Title")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::display_text<dialog_title>> vhelper(Title, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::display_text<dialog_title>> vhelper(Title, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("Description")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::multiline_text<description>> vhelper(Description, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::multiline_text<description>> vhelper(Description, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else if(compile_time_str_compare_ci<CT_STRING("cb_list")>(rn_s, rn_e) == 0) {
#ifdef _DEBUG
				 ui::detail::visitor_helper<ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24>> vhelper(cb_list, window, ws, rn_s, rn_e);
#else
				 ui::detail::visitor_helper<ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24>> vhelper(cb_list, window, ws);
#endif
				 std::visit(vhelper, *i);
			 } else {
				 if(!ui::detail::can_create_dynamic_s<fabricate_cb_window_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>::run(*this, ws, window, *i, rn_s, rn_e)) {
					 std::visit([&ws, &window](auto tag) {
						 if constexpr(!std::is_same_v<decltype(tag), std::monostate>)
							 ui::create_dynamic_element(ws, tag, window);
					 }, *i);
				 }
			 }
		 }
		 window_object = window.id;
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<hidden_button>, fabricate_cb_window_t_internal_class&, world_state&>)
			 Background.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<ok_button>, fabricate_cb_window_t_internal_class&, world_state&>)
			 AgreeButton.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::simple_button<cancel_button>, fabricate_cb_window_t_internal_class&, world_state&>)
			 DeclineButton.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<self_flag>, fabricate_cb_window_t_internal_class&, world_state&>)
			 LeftShield.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::masked_flag<target_flag>, fabricate_cb_window_t_internal_class&, world_state&>)
			 RightShield.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::display_text<dialog_title>, fabricate_cb_window_t_internal_class&, world_state&>)
			 Title.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::multiline_text<description>, fabricate_cb_window_t_internal_class&, world_state&>)
			 Description.initialize_in_window(*this, ws);
		 if constexpr(ui::detail::has_initialize_in_window<ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24>, fabricate_cb_window_t_internal_class&, world_state&>)
			 cb_list.initialize_in_window(*this, ws);
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
		 fabricate_cb_window_base::associated_object = &new_gobj.object;
		 window_object = new_gobj.id;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 if constexpr(ui::detail::has_on_create<fabricate_cb_window_base, world_state&>) {
			 fabricate_cb_window_base::on_create(ws);
		 } else if constexpr(ui::detail::has_on_create<fabricate_cb_window_base, fabricate_cb_window_t_internal_class&, world_state&>) {
			 fabricate_cb_window_base::on_create(*this, ws);
		 }
		 return win;
	 }
};
class fabricate_cb_window_t : public fabricate_cb_window_t_internal_class<void> {};
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, fabricate_cb_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.windows[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
inline ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, fabricate_cb_window_t& b) {
	 const auto& window_definition = ws.s.gui_m.ui_definitions.icons[handle];
	 const auto res = b.create(ws, window_definition);
	 ui::add_to_back(ws.w.gui_m, parent, res);
	 ws.w.gui_m.flag_minimal_update();
	 return res;
}
#pragma warning( pop )

