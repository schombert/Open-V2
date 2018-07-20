#pragma once
#include "common\\common.h"
#include "provinces.h"
#include "gui\\gui.hpp"
#include "simple_mpl\\simple_mpl.hpp"

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
		void button_function(ui::masked_flag<province_controller_flag>&, world_state&);
		template<typename W>
		void windowed_update(ui::masked_flag<province_controller_flag>& self, W& w, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(ui::masked_flag<province_controller_flag>& self, world_state& ws, ui::tagged_gui_object tw);
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
		void button_function(ui::masked_flag<occupation_flag>&, world_state&);
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
		province_tag selected_province;
		date_tag last_update;

		template<typename ...P>
		explicit province_statistics_base(P&& ... params) {}
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

		void button_function(ui::masked_flag<core_flag_button>&, world_state&);
		void update(ui::masked_flag<core_flag_button>& self, world_state& ws);
	};

	using core_flag = ui::gui_window<
		CT_STRING("country_flag"), ui::masked_flag<core_flag_button>,
		ui::gui_behavior
	>;

	using province_statistics = ui::gui_window<
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
		province_statistics_base>;

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

		void button_function(ui::masked_flag<colonist_controller_flag_button>&, world_state&);
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
		void button_function(ui::masked_flag<province_owner_flag>&, world_state&);
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

	using province_other = ui::gui_window<
		CT_STRING("supply_limit"), ui::display_text<supply_limit_text_box>,
		CT_STRING("core_icons"), ui::overlap_box<cores_lb, ui::window_tag, core_flag>,
		CT_STRING("total_population"), ui::display_text<total_population_text_box>,
		CT_STRING("workforce_chart"), ui::piechart<poptype_pie_chart>,
		CT_STRING("ideology_chart"), ui::piechart<ideology_pie_chart>,
		CT_STRING("culture_chart"), ui::piechart<culture_pie_chart>,
		CT_STRING("country_flag"), ui::masked_flag<province_owner_flag>,
		CT_STRING("our_relation"), ui::display_text<relation_text_box>,
		CT_STRING("country_flag_overlay"), ui::dynamic_transparent_icon<country_flag_frame>,
		province_other_base>;

	class province_window_base : public ui::fixed_region {
	public:
		province_tag selected_province;
		date_tag last_update;

		template<typename ...P>
		explicit province_window_base(P&& ... params) : ui::fixed_region(std::forward<P>(params)...){}
		void on_create(world_state&);
	};

	using province_window_t = ui::gui_window <
		CT_STRING("close_button"), ui::simple_button<close_province_window>,
		CT_STRING("province_view_header"), province_window_header,
		CT_STRING("province_statistics"), province_statistics,
		CT_STRING("province_colony"), province_colony,
		CT_STRING("province_other"), province_other,
		province_window_base>;
}
