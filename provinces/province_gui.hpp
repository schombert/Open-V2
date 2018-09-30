#pragma once
#include "province_gui.h"
#include "nations\\nations_functions.h"
#include "military\\military.h"
#include "military\\military_functions.h"
#include "gui\\gui.hpp"

namespace provinces {

	class modifier_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class modifier_button {
	public:
		const uint32_t icon;
		modifiers::provincial_modifier_tag mod;
		date_tag expiration;

		modifier_button(uint32_t num, modifiers::provincial_modifier_tag m, date_tag x) : icon(num), mod(m), expiration(x) {}
		void update(ui::simple_button<modifier_button>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using modifier_item = ui::gui_window<
		CT_STRING("modifier"), ui::simple_button<modifier_button>,
		ui::gui_behavior
	>;

	class close_province_window {
	public:
		void button_function(ui::gui_object_tag t, world_state& ws);
	};

	class terrain_icon {
	public:
		void update(ui::dynamic_icon<terrain_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class admin_icon {
	public:
		void update(ui::dynamic_icon<admin_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&);
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class admin_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class slave_state_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<slave_state_icon>& ico, window_type const&, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class colony_button {
	public:
		void button_function(ui::gui_object_tag t, world_state& ws);
		template<typename window_type>
		void windowed_update(ui::simple_button<colony_button>& ico, window_type const&, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class province_name {
	public:
		template<typename ...PARAMS>
		province_name(PARAMS&&...) {}
		template<typename window_type>
		void windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class province_state_name {
	public:
		template<typename ...PARAMS>
		province_state_name(PARAMS&&...) {}
		template<typename window_type>
		void windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class province_controller_flag {
	public:
		void create_tooltip(ui::masked_flag<province_controller_flag>&, world_state& ws, ui::tagged_gui_object tw);
		template<typename W>
		void windowed_update(ui::masked_flag<province_controller_flag>& self, W& w, world_state& ws);
	};

	class province_window_header_base : public ui::gui_behavior {
	public:
		template<typename ...P>
		explicit province_window_header_base(P&& ... params) {}
		void on_create(world_state&);
	};

	class flashpoint_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class liferating_bar {
	public:
		void update(ui::progress_bar<liferating_bar>& self, world_state& ws);
	};

	class liferating_overlay {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class occupation_progress_bar {
	public:
		template<typename W>
		void windowed_update(ui::progress_bar<occupation_progress_bar>& self, W& w, world_state& ws);
	};

	class occupation_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<occupation_icon>&, window_type&, world_state&);
	};

	class occupation_flag {
	public:
		template<typename W>
		void windowed_update(ui::masked_flag<occupation_flag>& self, W& w, world_state& ws);
	};

	class owner_icon {
	public:
		void update(ui::dynamic_icon<owner_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class owner_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	using province_window_header = ui::gui_window<
		CT_STRING("prov_terrain"), ui::dynamic_icon<terrain_icon>,
		CT_STRING("state_name"), ui::display_text<province_state_name>,
		CT_STRING("province_name"), ui::display_text<province_name>,
		CT_STRING("province_modifiers"), ui::overlap_box<modifier_lb, ui::window_tag, modifier_item>,
		CT_STRING("slave_state_icon"), ui::dynamic_icon<slave_state_icon>,
		CT_STRING("colony_button"), ui::simple_button<colony_button>,
		CT_STRING("admin_icon"), ui::dynamic_icon<admin_icon>,
		CT_STRING("admin_efficiency"), ui::display_text<admin_text_box, -4>,
		CT_STRING("controller_flag"), ui::masked_flag<province_controller_flag>,
		CT_STRING("flashpoint_indicator"), ui::dynamic_icon<flashpoint_icon>,
		CT_STRING("liferating"), ui::progress_bar<liferating_bar>,
		CT_STRING("liferating_overlay"), ui::dynamic_icon<liferating_overlay>,
		CT_STRING("occupation_progress"), ui::progress_bar<occupation_progress_bar>,
		CT_STRING("occupation_icon"), ui::dynamic_icon<occupation_icon>,
		CT_STRING("occupation_flag"), ui::masked_flag<occupation_flag>,
		CT_STRING("owner_icon"), ui::dynamic_icon<owner_icon>,
		CT_STRING("owner_presence"), ui::display_text<owner_text_box, -4>,
		province_window_header_base>;

	class province_statistics_base : public ui::gui_behavior {
	public:
		template<typename ...P>
		explicit province_statistics_base(P&& ... params) {}
		template<typename window_type>
		void windowed_update(window_type&, world_state&);
		void on_create(world_state&);
	};

	class produced_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class income_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class rgo_population_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class rgo_percent_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class goods_type_icon {
	public:
		void update(ui::dynamic_icon<goods_type_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class employment_ratio {
	public:
		void update(ui::dynamic_icon<employment_ratio>& ico, world_state& ws);
	};

	class crime_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<crime_icon>& ico, window_type const&, world_state& ws);
	};

	class crime_name {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class crimefight_percent_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class rebel_percent_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class total_population_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class migration_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class growth_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class poptype_pie_chart {
	public:
		void update(ui::piechart<poptype_pie_chart>& pie, world_state& ws);
	};

	class ideology_pie_chart {
	public:
		void update(ui::piechart<ideology_pie_chart>& pie, world_state& ws);
	};

	class culture_pie_chart {
	public:
		void update(ui::piechart<culture_pie_chart>& pie, world_state& ws);
	};

	class open_popscreen_button {
	public:
		void button_function(ui::gui_object_tag t, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class build_factory_button {
	public:
		void button_function(ui::gui_object_tag t, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class party_loyalty_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<party_loyalty_icon>& ico, window_type&, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class supply_limit_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class cores_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	class core_flag_button {
	public:
		cultures::national_tag core;

		core_flag_button(cultures::national_tag t) : core(t) {}

		void update(ui::masked_flag<core_flag_button>& self, world_state& ws);
	};

	using core_flag = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<core_flag_button>,
		ui::gui_behavior
	>;

	using province_statistics = ui::gui_window <
		CT_STRING("goods_type"), ui::dynamic_icon<goods_type_icon>,
		CT_STRING("produced"), ui::display_text<produced_text_box>,
		CT_STRING("income"), ui::display_text<income_text_box>,
		CT_STRING("employment_ratio"), ui::dynamic_icon<employment_ratio>,
		CT_STRING("rgo_population"), ui::display_text<rgo_population_text_box>,
		CT_STRING("rgo_percent"), ui::display_text<rgo_percent_text_box>,
		CT_STRING("crime_icon"), ui::dynamic_icon<crime_icon>,
		CT_STRING("crime_name"), ui::display_text<crime_name>,
		CT_STRING("crimefight_percent"), ui::display_text<crimefight_percent_text_box>,
		CT_STRING("rebel_percent"), ui::display_text<rebel_percent_text_box>,
		CT_STRING("total_population"), ui::display_text<total_population_text_box>,
		CT_STRING("migration"), ui::display_text<migration_text_box>,
		CT_STRING("growth"), ui::display_text<growth_text_box>,
		CT_STRING("workforce_chart"), ui::piechart<poptype_pie_chart>,
		CT_STRING("ideology_chart"), ui::piechart<ideology_pie_chart>,
		CT_STRING("culture_chart"), ui::piechart<culture_pie_chart>,
		CT_STRING("open_popscreen"), ui::simple_button<open_popscreen_button>,
		CT_STRING("build_factory_button"), ui::simple_button<build_factory_button>,
		CT_STRING("party_loyalty"), ui::dynamic_icon<party_loyalty_icon>,
		CT_STRING("supply_limit"), ui::display_text<supply_limit_text_box>,
		CT_STRING("core_icons"), ui::overlap_box<cores_lb, ui::window_tag, core_flag>,
		province_statistics_base > ;

	class province_colony_base : public ui::visible_region {
	public:
		template<typename ...P>
		explicit province_colony_base(P&& ... params) {}
		void on_create(world_state&);
		template<typename window_type>
		void windowed_update(window_type&, world_state&);
	};

	class uncolonized_phase_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class crisis_temperature_bar {
	public:
		void update(ui::progress_bar<crisis_temperature_bar>& self, world_state& ws);
	};

	class colonist_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m) {
			return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["colonist_item"]);
		}
		template<typename lb_type>
		void on_create(lb_type& lb, world_state&);
	};

	class colonist_list_item_base : public ui::visible_region {
	public:
		nations::country_tag colonizer;
		int32_t stage = 0;

		colonist_list_item_base(nations::country_tag c, int32_t s) : colonizer(c), stage(s) {}
		colonist_list_item_base(colonist_list_item_base&&) = default;
		colonist_list_item_base(colonist_list_item_base& b) noexcept : colonist_list_item_base(std::move(b)) {}

		void on_create(world_state&);
	};

	class stage_list_item_base : public ui::visible_region {
	public:
		int32_t stage = 0;
		int32_t required_points = 0;

		stage_list_item_base(int32_t s, int32_t p) : stage(s), required_points(p) {}
		stage_list_item_base(stage_list_item_base&&) = default;
		stage_list_item_base(stage_list_item_base& b) noexcept : stage_list_item_base(std::move(b)) {}

		void on_create(world_state&);
	};

	class stage_progress_icon {
	public:
		stage_progress_icon(int32_t, int32_t) {}

		template<typename window_type>
		void windowed_update(ui::dynamic_icon<stage_progress_icon>& ico, window_type&, world_state& ws);
	};

	class stage_progress_button {
	public:
		int32_t req_pts;

		stage_progress_button(int32_t, int32_t r) : req_pts(r) {}

		void button_function(ui::gui_object_tag t, world_state& ws);
		template<typename window_type>
		void windowed_update(ui::simple_button<stage_progress_button>& ico, window_type&, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class stage_lb {
	public:
		nations::country_tag colonizer;
		int32_t stage = 0;

		stage_lb(nations::country_tag t, int32_t s) : colonizer(t), stage(s) {}

		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m) {
			return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["level_entry"]);
		}
	};

	using stage_listitem_t = ui::gui_window<
		CT_STRING("progress_icon"), ui::dynamic_icon<stage_progress_icon>,
		CT_STRING("progress_button"), ui::simple_button<stage_progress_button>,
		stage_list_item_base
	>;

	class progress_counter_text_box {
	public:
		progress_counter_text_box(nations::country_tag, int32_t) {}

		template<typename window_type>
		void windowed_update(window_type&, ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class colonist_controller_flag_button {
	public:
		nations::country_tag colonizer;

		colonist_controller_flag_button(nations::country_tag t, int32_t) : colonizer(t) {}

		void update(ui::masked_flag<colonist_controller_flag_button>& self, world_state& ws);
	};

	using colonist_listitem_t = ui::gui_window<
		CT_STRING("controller_flag"), ui::masked_flag<colonist_controller_flag_button>,
		CT_STRING("progress_counter"), ui::display_text<progress_counter_text_box>,
		CT_STRING("levels"), ui::overlap_box<stage_lb, ui::window_tag, stage_listitem_t>,
		colonist_list_item_base
	>;

	class colonize_button {
	public:
		void button_function(ui::gui_object_tag t, world_state& ws);
		void update(ui::simple_button<colonize_button>& ico, world_state& ws);
		bool has_tooltip(world_state&);
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class withdraw_button {
	public:
		void button_function(ui::gui_object_tag t, world_state& ws);
		void update(ui::simple_button<withdraw_button>& ico, world_state& ws);
		bool has_tooltip(world_state&);
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	using province_colony = ui::gui_window<
		CT_STRING("uncolonized_phase"), ui::display_text<uncolonized_phase_text_box>,
		CT_STRING("crisis_temperature"), ui::progress_bar<crisis_temperature_bar>,
		CT_STRING("colonist_list"), ui::display_listbox<colonist_lb, colonist_listitem_t>,
		CT_STRING("colonize_button"), ui::simple_button<colonize_button>,
		CT_STRING("withdraw_button"), ui::simple_button<withdraw_button>,
		CT_STRING("goods_type"), ui::dynamic_icon<goods_type_icon>,
		CT_STRING("total_population"), ui::display_text<total_population_text_box>,
		CT_STRING("culture_chart"), ui::piechart<culture_pie_chart>,
		province_colony_base>;

	class province_other_base : public ui::visible_region {
	public:
		template<typename ...P>
		explicit province_other_base(P&& ... params) {}
		void on_create(world_state&);
		template<typename window_type>
		void windowed_update(window_type&, world_state&);
	};

	class province_owner_flag {
	public:
		void update(ui::masked_flag<province_owner_flag>& self, world_state& ws);
	};

	class relation_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class country_flag_frame {
	public:
		void update(ui::dynamic_transparent_icon<country_flag_frame>& ico, world_state& ws);
	};

	class province_owner_name {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class country_status {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class country_gov {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class country_party {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class score_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class rank_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class prestige_score_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class prestige_rank_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class industrial_score_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class industrial_rank_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class military_score_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class military_rank_text_box {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class military_icon {
	public:
		void update(ui::dynamic_icon<military_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class prestige_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class industrial_icon {
	public:
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class sphere_label {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class puppet_label {
	public:
		void update(ui::tagged_gui_object, ui::text_box_line_manager&, ui::text_format&, world_state&);
	};

	class sphere_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m) {
			return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["province_core"]);
		}
	};

	class puppets_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m) {
			return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["province_core"]);
		}
	};

	class allies_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m) {
			return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["province_core"]);
		}
	};

	class war_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m) {
			return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["province_core"]);
		}
	};

	class send_diplomat {
	public:
		void button_function(ui::simple_button<send_diplomat>&, world_state&);
	};

	class fort_level_icon {
	public:
		void update(ui::dynamic_icon<fort_level_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class naval_base_level_icon {
	public:
		void update(ui::dynamic_icon<naval_base_level_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class railroad_level_icon {
	public:
		void update(ui::dynamic_icon<railroad_level_icon>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class foreign_invest_railroad {
	public:
		void update(ui::simple_button<foreign_invest_railroad>& ico, world_state& ws);
		void button_function(ui::simple_button<foreign_invest_railroad>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class foreign_invest_factory {
	public:
		void update(ui::simple_button<foreign_invest_factory>& ico, world_state& ws);
		void button_function(ui::simple_button<foreign_invest_factory>& ico, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class infrastructure_progress_base : public ui::visible_region {
	public:
		template<typename ...P>
		explicit infrastructure_progress_base(P&& ... params) {}
		void on_create(world_state&);
		template<typename window_type>
		void windowed_update(window_type&, world_state&);
	};

	class infrastructure_progress_bar {
	public:
		void update(ui::progress_bar<infrastructure_progress_bar>& self, world_state& ws);
	};

	using infrastructure_progress = ui::gui_window<
		CT_STRING("building_progress"), ui::progress_bar<infrastructure_progress_bar>,
		infrastructure_progress_base>;

	using province_other = ui::gui_window <
		CT_STRING("supply_limit"), ui::display_text<supply_limit_text_box>,
		CT_STRING("core_icons"), ui::overlap_box<cores_lb, ui::window_tag, core_flag>,
		CT_STRING("total_population"), ui::display_text<total_population_text_box>,
		CT_STRING("workforce_chart"), ui::piechart<poptype_pie_chart>,
		CT_STRING("ideology_chart"), ui::piechart<ideology_pie_chart>,
		CT_STRING("culture_chart"), ui::piechart<culture_pie_chart>,
		CT_STRING("country_flag"), ui::masked_flag<province_owner_flag>,
		CT_STRING("our_relation"), ui::display_text<relation_text_box>,
		CT_STRING("country_flag_overlay"), ui::dynamic_transparent_icon<country_flag_frame>,
		CT_STRING("country_name"), ui::display_text<province_owner_name, 4>,
		CT_STRING("country_status"), ui::display_text<country_status, -5>,
		CT_STRING("country_gov"), ui::display_text<country_gov, -5>,
		CT_STRING("country_party"), ui::display_text<country_party, -5>,
		CT_STRING("selected_prestige_icon"), ui::dynamic_icon<prestige_icon>,
		CT_STRING("country_prestige"), ui::display_text<prestige_score_text_box, -3>,
		CT_STRING("selected_prestige_rank"), ui::display_text<prestige_rank_text_box, -13>,
		CT_STRING("selected_industry_icon"), ui::dynamic_icon<industrial_icon>,
		CT_STRING("country_economic"), ui::display_text<industrial_score_text_box, -3>,
		CT_STRING("selected_industry_rank"), ui::display_text<industrial_rank_text_box, -13>,
		CT_STRING("selected_military_icon"), ui::dynamic_icon<military_icon>,
		CT_STRING("country_military"), ui::display_text<military_score_text_box, -3>,
		CT_STRING("selected_military_rank"), ui::display_text<military_rank_text_box, -13>,
		CT_STRING("country_total"), ui::display_text<score_text_box, -3>,
		CT_STRING("selected_total_rank"), ui::display_text<rank_text_box, -13>,
		CT_STRING("sphere_label"), ui::display_text<sphere_label, -2>,
		CT_STRING("puppet_label"), ui::display_text<puppet_label, -2>,
		CT_STRING("sphere_targets"), ui::overlap_box<sphere_lb, ui::window_tag, core_flag>,
		CT_STRING("puppet_targets"), ui::overlap_box<puppets_lb, ui::window_tag, core_flag>,
		CT_STRING("allied_targets"), ui::overlap_box<allies_lb, ui::window_tag, core_flag>,
		CT_STRING("war_targets"), ui::overlap_box<war_lb, ui::window_tag, core_flag>,
		CT_STRING("send_diplomat"), ui::simple_button<send_diplomat>,
		CT_STRING("build_icon_fort"), ui::dynamic_icon<fort_level_icon>,
		CT_STRING("build_icon_navalbase"), ui::dynamic_icon<naval_base_level_icon>,
		CT_STRING("build_icon_infra"), ui::dynamic_icon<railroad_level_icon>,
		CT_STRING("infra_progress_win"), infrastructure_progress,
		CT_STRING("invest_build_infra"), ui::simple_button<foreign_invest_railroad>,
		CT_STRING("invest_factory_button"), ui::simple_button<foreign_invest_factory>,
		province_other_base > ;

	class fort_progress_bar {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::progress_bar<fort_progress_bar>& self, world_state& ws);
	};

	class naval_base_progress_bar {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::progress_bar<naval_base_progress_bar>& self, world_state& ws);
	};

	class railroad_progress_bar {
	public:
		template<typename window_type>
		void windowed_update(window_type&, ui::progress_bar<railroad_progress_bar>& self, world_state& ws);
	};

	class expand_text {};

	class fort_expand_button {
	public:
		void button_function(ui::simple_button<fort_expand_button>&, world_state&);
		template<typename window_type>
		void windowed_update(ui::simple_button<fort_expand_button>& self, window_type& w, world_state& ws);
	};

	class naval_base_expand_button {
	public:
		void button_function(ui::simple_button<naval_base_expand_button>&, world_state&);
		template<typename window_type>
		void windowed_update(ui::simple_button<naval_base_expand_button>& self, window_type& w, world_state& ws);
	};

	class railroad_expand_button {
	public:
		void button_function(ui::simple_button<railroad_expand_button>&, world_state&);
		template<typename window_type>
		void windowed_update(ui::simple_button<railroad_expand_button>& self, window_type& w, world_state& ws);
	};

	class province_buildings_base : public ui::gui_behavior {
	public:
		ui::dynamic_icon<fort_level_icon> fort_icon;
		ui::dynamic_icon<naval_base_level_icon> naval_base_icon;
		ui::dynamic_icon<railroad_level_icon> railroad_icon;

		ui::simple_button<fort_expand_button> fort_button;
		ui::simple_button<naval_base_expand_button> naval_base_button;
		ui::simple_button<railroad_expand_button> railroad_button;

		ui::progress_bar<fort_progress_bar> fort_bar;
		ui::progress_bar<naval_base_progress_bar> naval_base_bar;
		ui::progress_bar<railroad_progress_bar> railroad_bar;

		ui::display_text<expand_text> fort_expand_text;
		ui::display_text<expand_text> naval_base_expand_text;
		ui::display_text<expand_text> railroad_expand_text;

		template<typename ...P>
		explicit province_buildings_base(P&& ... params) {}
		template<typename window_type>
		void windowed_update(window_type&, world_state&);
		template<typename window_type>
		void on_create(window_type& win, world_state& ws);
	};

	class build_army_button {
	public:
		void update(ui::simple_button<build_army_button>& ico, world_state& ws);
	};
	class build_navy_button {
	public:
		void update(ui::simple_button<build_navy_button>& ico, world_state& ws);
	};

	using province_buildings = ui::gui_window<
		CT_STRING("build_army"), ui::simple_button<build_army_button>,
		CT_STRING("build_navy"), ui::simple_button<build_navy_button>,
		province_buildings_base>;

	class province_window_base : public ui::fixed_region {
	public:
		date_tag last_update;

		template<typename ...P>
		explicit province_window_base(P&& ... params) : ui::fixed_region(std::forward<P>(params)...) {}
		void on_create(world_state&);
	};

	class province_window_t : public ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<close_province_window>,
		CT_STRING("province_view_header"), province_window_header,
		CT_STRING("province_statistics"), province_statistics,
		CT_STRING("province_colony"), province_colony,
		CT_STRING("province_other"), province_other,
		CT_STRING("province_buildings"), province_buildings,
		province_window_base> {};

	template<typename lb_type>
	void modifier_lb::populate_list(lb_type& lb, world_state& ws) {
		auto selected = ws.w.province_w.selected_province;
		if(is_valid_index(selected)) {
			auto& pstate = ws.w.province_s.province_state_container[selected];
			auto static_mod_range = get_range(ws.w.province_s.static_modifier_arrays, pstate.static_modifiers);
			for(auto s = static_mod_range.first; s != static_mod_range.second; ++s) {
				if(auto m = *s; is_valid_index(m))
					lb.add_item(ws, ws.s.modifiers_m.provincial_modifiers[m].icon, m, date_tag());
			}
			auto timed_mod_range = get_range(ws.w.province_s.timed_modifier_arrays, pstate.timed_modifiers);
			for(auto s = timed_mod_range.first; s != timed_mod_range.second; ++s) {
				if(auto m = s->mod; is_valid_index(m))
					lb.add_item(ws, ws.s.modifiers_m.provincial_modifiers[m].icon, m, s->expiration);
			}
		}
	}

	template<typename window_type>
	void province_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected = ws.w.province_w.selected_province;
		if(is_valid_index(selected))
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.w.province_s.province_state_container[selected].name, fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
		lm.finish_current_line();
	}

	template<typename window_type>
	void province_state_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected = ws.w.province_w.selected_province;
		if(is_valid_index(selected)) {
			if(auto si = ws.w.province_s.province_state_container[selected].state_instance; si)
				ui::add_linear_text(ui::xy_pair{ 0,0 }, si->name, fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
			if(is_valid_index(ws.s.province_m.province_container[selected].state_id))
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.province_m.state_names[ws.s.province_m.province_container[selected].state_id], fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
		}
		lm.finish_current_line();
	}

	template<typename window_type>
	void crime_name::windowed_update(window_type& w, ui::tagged_gui_object obj, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto crime = ws.w.province_s.province_state_container[selected_prov].crime;
			if(is_valid_index(crime)) {
				ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.modifiers_m.provincial_modifiers[crime].name, fmt, ws.s.gui_m, ws.w.gui_m, obj, lm);
				lm.finish_current_line();
				ui::make_visible_immediate(obj.object);
				return;
			}
		}
		ui::hide(obj.object);
	}

	template<typename W>
	void province_controller_flag::windowed_update(ui::masked_flag<province_controller_flag>& self, W& w, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto controller = ws.w.province_s.province_state_container[selected_prov].controller;
			if(controller) {
				self.set_displayed_flag(ws, controller->tag);
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		self.set_visibility(ws.w.gui_m, false);
	}

	template<typename W>
	void occupation_flag::windowed_update(ui::masked_flag<occupation_flag>& self, W& w, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			auto sieging_nation = get_province_seiger(ws, ws.w.province_s.province_state_container[selected_prov]);
			if(is_valid_index(sieging_nation)) {
				self.set_displayed_flag(ws, ws.w.nation_s.nations.get(sieging_nation).tag);
				ui::make_visible_immediate(*self.associated_object);
			}
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}

	template<typename window_type>
	void slave_state_icon::windowed_update(ui::dynamic_icon<slave_state_icon>& ico, window_type const&, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;
			if(state) {
				if(state->flags & nations::state_instance::is_slave_state)
					ui::make_visible_immediate(*ico.associated_object);
				else
					ico.set_visibility(ws.w.gui_m, false);
				return;
			}
		}
		ico.set_visibility(ws.w.gui_m, false);
	}


	template<typename window_type>
	void crime_icon::windowed_update(ui::dynamic_icon<crime_icon>& ico, window_type const&, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto crime = ws.w.province_s.province_state_container[selected_prov].crime;
			if(is_valid_index(crime)) {
				ico.set_frame(ws.w.gui_m, ws.s.modifiers_m.provincial_modifiers[crime].icon);
				ui::make_visible_immediate(*ico.associated_object);
				return;
			}
		}
		ico.set_visibility(ws.w.gui_m, false);
	}

	template<typename window_type>
	void colony_button::windowed_update(ui::simple_button<colony_button>& ico, window_type const&, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov)) {
			auto state = ws.w.province_s.province_state_container[selected_prov].state_instance;

			if(state) {
				if(ws.w.province_s.province_state_container[selected_prov].owner != ws.w.local_player_nation || ws.w.local_player_nation == nullptr) {
					ico.set_enabled(false);
					if(state->flags & nations::state_instance::is_protectorate) {
						ico.set_frame(ws.w.gui_m, 0ui32);
						ui::make_visible_immediate(*ico.associated_object);
					} else if(state->flags & nations::state_instance::is_colonial) {
						ico.set_frame(ws.w.gui_m, 1ui32);
						ui::make_visible_immediate(*ico.associated_object);
					} else {
						ico.set_visibility(ws.w.gui_m, false);
					}
				} else {
					int32_t free_points = nations::free_colonial_points(ws, *ws.w.local_player_nation);
					if(state->flags & nations::state_instance::is_protectorate) {
						ico.set_frame(ws.w.gui_m, 0ui32);
						ico.set_enabled(nations::colonial_points_to_make_colony(ws, *state) <= free_points);
						ui::make_visible_immediate(*ico.associated_object);
					} else if(state->flags & nations::state_instance::is_colonial) {
						ico.set_frame(ws.w.gui_m, 1ui32);
						ico.set_enabled(nations::colonial_points_to_make_state(ws, *state) <= free_points);
						ui::make_visible_immediate(*ico.associated_object);
					} else {
						ico.set_visibility(ws.w.gui_m, false);
					}
				}
				return;
			}
		}
		ico.set_visibility(ws.w.gui_m, false);
	}

	template<typename W>
	void occupation_progress_bar::windowed_update(ui::progress_bar<occupation_progress_bar>& self, W&, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			self.set_fraction(ws.w.province_s.province_state_container[selected_prov].siege_progress);
			ui::make_visible_immediate(*self.associated_object);
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}


	template<typename window_type>
	void occupation_icon::windowed_update(ui::dynamic_icon<occupation_icon>& self, window_type&, world_state& ws) {
		auto selected_prov = ws.w.province_w.selected_province;
		if(is_valid_index(selected_prov) && ws.w.province_s.province_state_container[selected_prov].siege_progress != 0.0f) {
			ui::make_visible_immediate(*self.associated_object);
			return;
		}
		self.set_visibility(ws.w.gui_m, false);
	}

	template<typename window_type>
	void party_loyalty_icon::windowed_update(ui::dynamic_icon<party_loyalty_icon>& ico, window_type&, world_state& ws) {
		ico.set_visibility(ws.w.gui_m, false);
	}

	template<typename lb_type>
	void cores_lb::populate_list(lb_type& lb, world_state& ws) {
		auto selected = ws.w.province_w.selected_province;
		if(is_valid_index(selected)) {
			auto core_range = get_range(ws.w.province_s.core_arrays, ws.w.province_s.province_state_container[selected].cores);
			for(auto i = core_range.first; i != core_range.second; ++i)
				lb.add_item(ws, *i);
		}
	}

	template<typename window_type>
	void province_colony_base::windowed_update(window_type&, world_state& ws) {
		auto selected = ws.w.province_w.selected_province;
		if(is_valid_index(selected)) {
			if(ws.w.province_s.province_state_container[selected].owner == nullptr) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void province_statistics_base::windowed_update(window_type&, world_state& ws) {
		if(auto selected = ws.w.province_w.selected_province; is_valid_index(selected)) {
			if(ws.w.province_s.province_state_container[selected].owner == ws.w.local_player_nation && ws.w.local_player_nation != nullptr) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void province_buildings_base::windowed_update(window_type&, world_state& ws) {
		if(auto selected = ws.w.province_w.selected_province; is_valid_index(selected)) {
			if(ws.w.province_s.province_state_container[selected].owner == ws.w.local_player_nation && ws.w.local_player_nation != nullptr) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void province_other_base::windowed_update(window_type&, world_state& ws) {
		auto selected = ws.w.province_w.selected_province;
		if(is_valid_index(selected)) {
			auto owner = ws.w.province_s.province_state_container[selected].owner;
			if(owner != nullptr && owner != ws.w.local_player_nation) {
				ui::make_visible_immediate(*associated_object);
				return;
			}
		}
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void infrastructure_progress_base::windowed_update(window_type&, world_state& ws) {
		ui::hide(*associated_object);
	}
	
	template<typename lb_type>
	void colonist_lb::populate_list(lb_type& lb, world_state& ws) {
		auto selected = ws.w.province_w.selected_province;
		if(is_valid_index(selected)) {
			auto state = ws.w.province_s.province_state_container[selected].state_instance;
			if(state) {
				for(uint32_t i = 0; i < std::extent_v<decltype(state->colonizers)>; ++i) {
					if(state->colonizers[i].second != 0)
						lb.add_item(ws, state->colonizers[i].first, state->colonizers[i].second);
				}
			}
		}
	}

	template<typename lb_type>
	void stage_lb::populate_list(lb_type& lb, world_state& ws) {
		if(ws.w.local_player_nation == nullptr || ws.w.local_player_nation->id != colonizer) {
			for(int32_t i = 0; i < stage && i < 5; ++i) {
				lb.add_item(ws, i, -1);
			}
		} else {
			int32_t i = 0;
			for(; i < (stage - 1) && i < 4; ++i) {
				lb.add_item(ws, i, -1);
			}
			auto selected = ws.w.province_w.selected_province;
			if(is_valid_index(selected)) {
				auto state = ws.w.province_s.province_state_container[selected].state_instance;
				if(state)
					lb.add_item(ws, i, nations::points_for_next_colonial_stage(ws, *ws.w.local_player_nation, *state));
			}
		}
	}

	template<typename window_type>
	void stage_progress_icon::windowed_update(ui::dynamic_icon<stage_progress_icon>& ico, window_type& w, world_state& ws) {
		if(w.required_points == -1) {
			ico.set_frame(ws.w.gui_m, uint32_t(w.stage));
		} else {
			ico.set_visibility(ws.w.gui_m, false);
		}
	}

	template<typename window_type>
	void stage_progress_button::windowed_update(ui::simple_button<stage_progress_button>& ico, window_type& w, world_state& ws) {
		if(req_pts == -1) {
			ico.set_visibility(ws.w.gui_m, false);
		} else {
			ico.set_frame(ws.w.gui_m, uint32_t(w.stage));
			if(req_pts < nations::free_colonial_points(ws, *ws.w.local_player_nation)) {
				ico.set_enabled(false);
			} else {
				ico.set_enabled(true);
			}
		}
	}

	template<typename window_type>
	void progress_counter_text_box::windowed_update(window_type& win, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		
		if(win.stage >= 5) {
			char16_t formatted_value[64];
			put_value_in_buffer(formatted_value, display_type::integer, win.stage);

			ui::text_chunk_to_instances(
				ws.s.gui_m,
				ws.w.gui_m,
				vector_backed_string<char16_t>(formatted_value),
				box,
				ui::xy_pair{ 0,0 },
				fmt,
				lm);

			lm.finish_current_line();
		}
	}
	
	template<typename lb_type>
	void colonist_lb::on_create(lb_type& lb, world_state&) {
		lb.associated_object->size = ui::xy_pair{ 375i16, 250i16 };
	}


	template<typename lb_type>
	void sphere_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				auto leader = owner->sphere_leader;
				if(leader == nullptr) {
					auto sphere_range = get_range(ws.w.nation_s.nations_arrays, owner->sphere_members);
					for(auto i : sphere_range) {
						if(is_valid_index(i))
							lb.add_item(ws, ws.w.nation_s.nations[i].tag);
					}
				} else
					lb.add_item(ws, leader->tag);
			}
		}
	}

	template<typename lb_type>
	void puppets_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				auto leader = owner->overlord;
				if(leader == nullptr) {
					auto vassal_range = get_range(ws.w.nation_s.nations_arrays, owner->vassals);
					for(auto i : vassal_range) {
						if(is_valid_index(i))
							lb.add_item(ws, ws.w.nation_s.nations[i].tag);
					}
				} else
					lb.add_item(ws, leader->tag);
			}
		}
	}

	template<typename lb_type>
	void allies_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				auto allies_range = get_range(ws.w.nation_s.nations_arrays, owner->allies);
				for(auto i : allies_range) {
					if(is_valid_index(i))
						lb.add_item(ws, ws.w.nation_s.nations[i].tag);
				}
			}
		}
	}

	template<typename lb_type>
	void war_lb::populate_list(lb_type& lb, world_state& ws) {
		if(auto selected_prov = ws.w.province_w.selected_province; is_valid_index(selected_prov)) {
			if(auto owner = ws.w.province_s.province_state_container[selected_prov].owner; owner) {
				auto opposing_countries = get_range(ws.w.nation_s.nations_arrays, owner->opponents_in_war);

				for(auto n : opposing_countries)
					lb.add_item(ws, ws.w.nation_s.nations[n].tag);
			}
		}
	}
	
	template<typename window_type>
	void fort_progress_bar::windowed_update(window_type& win, ui::progress_bar<fort_progress_bar>& self, world_state& ws) {
		ui::hide(*self.associated_object);
		ui::hide(*win.fort_expand_text.associated_object);
	}
	
	template<typename window_type>
	void naval_base_progress_bar::windowed_update(window_type&, ui::progress_bar<naval_base_progress_bar>& self, world_state& ws) {
		ui::hide(*self.associated_object);
		ui::hide(*win.naval_base_expand_text.associated_object);
	}
	
	template<typename window_type>
	void railroad_progress_bar::windowed_update(window_type&, ui::progress_bar<railroad_progress_bar>& self, world_state& ws) {
		ui::hide(*self.associated_object);
		ui::hide(*win.railroad_expand_text.associated_object);
	}


	template<typename window_type>
	void fort_expand_button::windowed_update(ui::simple_button<fort_expand_button>& self, window_type& w, world_state& ws) {}
	template<typename window_type>
	void naval_base_expand_button::windowed_update(ui::simple_button<naval_base_expand_button>& self, window_type& w, world_state& ws) {}
	template<typename window_type>
	void railroad_expand_button::windowed_update(ui::simple_button<railroad_expand_button>& self, window_type& w, world_state& ws) {}

	template<typename window_type>
	void province_buildings_base::on_create(window_type& win, world_state& ws) {
		associated_object->position += ui::xy_pair{ 0i16, -10i16 };
		associated_object->size += ui::xy_pair{ 0i16, 10i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ 0i16, 10i16 };
		});

		const auto& window_definition = ws.s.gui_m.ui_definitions.windows[std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building"])];
		for(auto i = window_definition.sub_object_definitions.cbegin(); i != window_definition.sub_object_definitions.cend(); ++i) {
			if(std::holds_alternative<ui::button_tag>(*i)) {
				ui::create_static_element(ws, std::get<ui::button_tag>(*i), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_button);
				ui::create_static_element(ws, std::get<ui::button_tag>(*i), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_button);
				ui::create_static_element(ws, std::get<ui::button_tag>(*i), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_button);
				break;
			}
		}

		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["build_icon0"]), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_icon);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["build_icon1"]), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_icon);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["build_icon2"]), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_icon);

		ui::create_static_element(ws, std::get<ui::text_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["expand_text"]), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_expand_text);
		ui::create_static_element(ws, std::get<ui::text_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["expand_text"]), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_expand_text);
		ui::create_static_element(ws, std::get<ui::text_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["expand_text"]), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_expand_text);

		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building_progress"]), ui::tagged_gui_object{ *associated_object, win.window_object }, fort_bar);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building_progress"]), ui::tagged_gui_object{ *associated_object, win.window_object }, railroad_bar);
		ui::create_static_element(ws, std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["building_progress"]), ui::tagged_gui_object{ *associated_object, win.window_object }, naval_base_bar);

		ui::add_linear_text(ui::xy_pair{ 69ui16, 15ui16 }, ws.s.fixed_ui_text[scenario::fixed_ui::fort],
			ui::text_format{ ui::text_color::black, graphics::font_tag(2), 20 }, ws.s.gui_m, ws.w.gui_m,
			ui::tagged_gui_object{ *associated_object, win.window_object });

		ui::add_linear_text(ui::xy_pair{ 69ui16, 50ui16 }, ws.s.fixed_ui_text[scenario::fixed_ui::naval_base],
			ui::text_format{ ui::text_color::black, graphics::font_tag(2), 20 }, ws.s.gui_m, ws.w.gui_m,
			ui::tagged_gui_object{ *associated_object, win.window_object });

		ui::add_linear_text(ui::xy_pair{ 69ui16, 85ui16 }, ws.s.fixed_ui_text[scenario::fixed_ui::railroad],
			ui::text_format{ ui::text_color::black, graphics::font_tag(2), 20 }, ws.s.gui_m, ws.w.gui_m,
			ui::tagged_gui_object{ *associated_object, win.window_object });

		fort_icon.associated_object->position.y = 7i16;
		naval_base_icon.associated_object->position.y = 42i16;
		railroad_icon.associated_object->position.y = 77i16;

		fort_bar.associated_object->position.y = 15i16;
		naval_base_bar.associated_object->position.y = 50i16;
		railroad_bar.associated_object->position.y = 85i16;

		fort_expand_text.associated_object->position.y = 15i16;
		naval_base_expand_text.associated_object->position.y = 50i16;
		railroad_expand_text.associated_object->position.y = 85i16;

		fort_button.associated_object->position.y = 13i16;
		naval_base_button.associated_object->position.y = 48i16;
		railroad_button.associated_object->position.y = 83i16;
	}
}
