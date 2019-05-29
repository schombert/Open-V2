#pragma once
#include "common\\common.h"
#include "bottombar.h"
#include "world_state.h"
#include "gui\\gui.hpp"
#include "messages.h"

namespace current_state {
	class log_text_box {
	public:
		template<typename window_type>
		void windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};


	class log_item_base : public ui::visible_region {
	public:
		int32_t index = 0;
		void set_value(int32_t t) {
			index = t;
		}

		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class log_items_lb {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using log_item = ui::gui_window<
		CT_STRING("open_v2_log_text_box"), ui::display_text<log_text_box>,
		log_item_base
	>;

	class location_icon {
	public:
	};

	class mini_map : public ui::gui_behavior {
	public:
		mini_map() = default;
		mini_map(mini_map&&) = default;

		ui::dynamic_icon<location_icon> location;

		virtual bool mouse_consumer(ui::xy_pair) final override { return true; }
		virtual bool on_lclick(ui::gui_object_tag o, world_state& m, const ui::lbutton_down& lb) final override;
	};

	class message_settings_button {
	public:
		void button_function(ui::simple_button<message_settings_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class bottombar_base : public ui::gui_behavior {
	public:
		mini_map mmap;
		ui::simple_button<message_settings_button> settings_button;

		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class message_log_base : public ui::visible_region {
	public:
		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class chat_window_base : public ui::visible_region {
	public:
		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class close_log_button {
	public:
		void button_function(ui::simple_button<close_log_button>&, world_state& ws);
	};

	template<int32_t category>
	class message_category_button {
	public:
		void update(ui::simple_button<message_category_button<category>>& self, world_state& ws);
		void button_function(ui::simple_button<message_category_button<category>>& self, world_state& ws);
	};

	using message_log_window = ui::gui_window<
		CT_STRING("messagelog"), ui::discrete_listbox<log_items_lb, log_item, int32_t>,
		CT_STRING("closebutton"), ui::simple_button<close_log_button>,
		CT_STRING("messagecat_combat"), ui::simple_button<message_category_button<messages::message_category::combat>>,
		CT_STRING("messagecat_diplomacy"), ui::simple_button<message_category_button<messages::message_category::diplomacy>>,
		CT_STRING("messagecat_units"), ui::simple_button<message_category_button<messages::message_category::units>>,
		CT_STRING("messagecat_provinces"), ui::simple_button<message_category_button<messages::message_category::provinces>>,
		CT_STRING("messagecat_events"), ui::simple_button<message_category_button<messages::message_category::events>>,
		CT_STRING("messagecat_others"), ui::simple_button<message_category_button<messages::message_category::other>>,
		message_log_base
	>;

	using chat_window = ui::gui_window<chat_window_base>;

	template<map_mode::type mode>
	class map_mode_button {
	public:
		void update(ui::simple_button<map_mode_button<mode>>& self, world_state& ws);
		void button_function(ui::simple_button<map_mode_button<mode>>&, world_state& ws);
	};

	class open_log_button {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<open_log_button>& self, window_type& w, world_state& ws);
		void button_function(ui::simple_button<open_log_button>& self, world_state& ws);
	};

	class menu_button {
	public:
		void button_function(ui::simple_button<menu_button>& self, world_state& ws);
	};

	class ledger_button {
	public:
		void button_function(ui::simple_button<ledger_button>& self, world_state& ws);
	};

	class goto_button {
	public:
		void button_function(ui::simple_button<goto_button>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class zoom_in_button {
	public:
		void button_function(ui::simple_button<zoom_in_button>& self, world_state& ws);
	};

	class zoom_out_button {
	public:
		void button_function(ui::simple_button<zoom_out_button>& self, world_state& ws);
	};

	class bottombar_t : public ui::gui_window <
		CT_STRING("minimap_bg"), ui::background_icon,
		CT_STRING("menubar_bg"), ui::background_icon,
		CT_STRING("messagelog_window"), message_log_window,
		CT_STRING("OPENbutton"), ui::simple_button<open_log_button>,
		CT_STRING("chat_window"), chat_window,
		CT_STRING("menu_button"), ui::simple_button<menu_button>,
		CT_STRING("ledger_button"), ui::simple_button<ledger_button>,
		CT_STRING("button_goto"), ui::simple_button<goto_button>,
		CT_STRING("map_zoom_in"), ui::simple_button<zoom_in_button>,
		CT_STRING("map_zoom_out"), ui::simple_button<zoom_out_button>,
		CT_STRING("mapmode_1"), ui::simple_button<map_mode_button<map_mode::type::political>>,
		CT_STRING("mapmode_2"), ui::simple_button<map_mode_button<map_mode::type::distance>>,
		CT_STRING("mapmode_3"), ui::simple_button<map_mode_button<map_mode::type::prices>>,
		CT_STRING("mapmode_4"), ui::simple_button<map_mode_button<map_mode::type::production>>,
		CT_STRING("mapmode_5"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_6"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_7"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_8"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_9"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_10"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_11"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_12"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_13"), ui::simple_button<map_mode_button<map_mode::type::culture>>,
		CT_STRING("mapmode_14"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_15"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_16"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_17"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_18"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_19"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_20"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_21"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		CT_STRING("mapmode_22"), ui::simple_button<map_mode_button<map_mode::type::purchasing>>,
		bottombar_base
	> {};

	template<map_mode::type mode>
	void map_mode_button<mode>::update(ui::simple_button<map_mode_button<mode>>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, uint32_t(ws.w.map_view.mode == mode));
	}
	template<map_mode::type mode>
	void map_mode_button<mode>::button_function(ui::simple_button<map_mode_button<mode>>&, world_state & ws) {
		map_mode::change_mode(ws, mode);
	}
	template<typename W>
	void chat_window_base::on_create(W & w, world_state & ws) {
		ui::hide(*associated_object);
	}
	template<typename W>
	void message_log_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{447i16 + 16i16,156i16};
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			if(obj.object.align == ui::alignment::bottom_right) {
				obj.object.position.y *= -1;
				obj.object.position.x += 874 - (447 + 16);
			}
		});
		w.get<CT_STRING("closebutton")>().associated_object->position.y = -29i16;
		w.get<CT_STRING("closebutton")>().associated_object->position.x = -36i16;
		w.get<CT_STRING("messagelog")>().associated_object->position.x += 8;

		ui::hide(*w.associated_object);
	}
	template<typename W>
	void bottombar_base::on_create(W & w, world_state & ws) {
		const auto new_gobj = ws.w.gui_m.gui_objects.emplace();

		const ui::icon_def& icon_def = ws.s.gui_m.ui_definitions.icons[std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["minimap_pic"])];

		new_gobj.object.position = ui::xy_pair{-328i16,-110i16};
		new_gobj.object.flags.fetch_or(ui::icon_def::rotation_upright, std::memory_order_acq_rel);
		new_gobj.object.align = ui::alignment::bottom_right;

		ui::detail::instantiate_graphical_object(ws.s.gui_m, ws.w.gui_m, new_gobj, icon_def.graphical_object_handle, 0, true);

		new_gobj.object.size.x = int16_t(266);
		new_gobj.object.size.y = int16_t(102);

		new_gobj.object.associated_behavior = &mmap;
		mmap.associated_object = &new_gobj.object;

		ui::add_to_front(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, w.window_object }, new_gobj);


		ws.w.gui_m.flag_minimal_update();
		ui::move_to_front(ws.w.gui_m, new_gobj);

		auto location_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_location_cursor"]);
		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, location_tag,
			new_gobj,
			mmap.location));

		//settings_button
		auto button_tag = std::get<ui::button_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_message_settings_button"]);
		ui::move_to_front(ws.w.gui_m, ui::create_static_element(
			ws, button_tag,
			ui::tagged_gui_object{ *associated_object, w.window_object },
			settings_button));

		w.get<CT_STRING("button_goto")>().associated_object->position.y = -65i16;

		associated_object->size = ui::xy_pair{ 874i16,166i16 };
		associated_object->position = ui::xy_pair{ -874i16,-166i16 };
		associated_object->align = ui::alignment::bottom_right;
	}
	template<int32_t category>
	void message_category_button<category>::update(ui::simple_button<message_category_button<category>>& self, world_state & ws) {
		self.set_frame(ws.w.gui_m, uint32_t(1 - int32_t((ws.w.message_w.message_categories & category) != 0)));
	}
	template<int32_t category>
	void message_category_button<category>::button_function(ui::simple_button<message_category_button<category>>& self, world_state & ws) {
		ws.w.message_w.message_categories ^= category;
		ui::make_visible_and_update(ws.w.gui_m, *ws.w.bottombar_w.win->associated_object);
	}
	template<typename window_type>
	void open_log_button::windowed_update(ui::simple_button<open_log_button>& self, window_type & w, world_state & ws) {
		messages::log_message_instance m;
		while(ws.w.message_w.pending_log_items.try_pop(m)) {
			ws.w.message_w.first_log_item--;
			if(ws.w.message_w.first_log_item < 0)
				ws.w.message_w.first_log_item = messages::maximum_log_items - 1;

			ws.w.message_w.current_log[ws.w.message_w.first_log_item] = m;
		}
		
		if(ws.w.bottombar_w.log_is_open == false)
			ui::make_visible_immediate(*self.associated_object);
		else
			ui::hide(*self.associated_object);
	}
	template<typename W>
	void log_item_base::on_create(W & w, world_state & ws) {
	}
	
	template<typename window_type>
	void log_text_box::windowed_update(window_type& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {
		ws.w.message_w.current_log[w.index].func(ws, box, lm, fmt);
	}
	
	template<typename lb_type>
	void log_items_lb::populate_list(lb_type & lb, world_state & ws) {
		int32_t indices_to_add[messages::maximum_log_items];
		int32_t count = 0;

		for(int32_t i = 0; i < messages::maximum_log_items ; ++i) {
			int32_t actual_index = (i + ws.w.message_w.first_log_item) % messages::maximum_log_items;
			if((ws.w.message_w.current_log[actual_index].category & ws.w.message_w.message_categories) != 0) {
				indices_to_add[count] = actual_index;
				++count;
			}
		}

		lb.new_list(indices_to_add, indices_to_add + count);
	}
}