class factory_display;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, factory_display& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, factory_display& b);
class goods_category_label;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, goods_category_label& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, goods_category_label& b);
class production_info;
ui::tagged_gui_object create_static_element(world_state& ws, ui::window_tag handle, ui::tagged_gui_object parent, production_info& b);
ui::tagged_gui_object create_static_element(world_state& ws, ui::icon_tag handle, ui::tagged_gui_object parent, production_info& b);

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
				 if constexpr(ui::detail::can_create_dynamic<factory_display_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!factory_display_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
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
		 factory_display_base::associated_object = &new_gobj.object;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<goods_category_label_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!goods_category_label_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
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
		 goods_category_label_base::associated_object = &new_gobj.object;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
				 if constexpr(ui::detail::can_create_dynamic<production_info_base, world_state&, ui::tagged_gui_object, ui::element_tag, char const*, char const*>) {
					 if(!production_info_base::create_dynamic(ws, window, *i, rn_s, rn_e)) {
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
		 production_info_base::associated_object = &new_gobj.object;
		 return new_gobj;
	 }
	 template<typename def_type>
	 ui::tagged_gui_object create(world_state& ws, def_type const& definition) {
		 const auto win = create_gui_obj(ws, definition);
		 window_object = win.id;
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
