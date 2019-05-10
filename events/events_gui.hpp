#pragma once
#include "common\\common.h"
#include "events_gui.h"
#include "gui\\gui.hpp"
#include "world_state\\world_state.h"

namespace events {
	class hidden_image {
	public:
		void update(ui::dynamic_icon<hidden_image>& self, world_state& ws);
	};

	class hidden_text {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws) {}
	};

	class province_event_title {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class province_event_location {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class province_event_body {
	public:
		void update(ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class hidden_button {
	public:
		void update(ui::simple_button<hidden_button>& self, world_state& ws);
	};

	class province_options_listbox {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	using event_option_pair = std::pair<pending_event, int32_t>;

	template <typename lb_type>
	void province_options_listbox::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<event_option_pair, 8, concurrent_allocator<event_option_pair>> possible_list;

		if(auto e = ws.w.province_event_w.displayed_event.e; e) {
			auto const num_options = std::count_if(
				std::begin(ws.s.event_m.event_container[e].options),
				std::end(ws.s.event_m.event_container[e].options),
				[](event_option const& opt) { return is_valid_index(opt.name); });
			for(int32_t i = 0; i < num_options; ++i) {
				possible_list.emplace_back(ws.w.province_event_w.displayed_event, i);
			}
		}

		lb.new_list(possible_list.begin().get_ptr(), possible_list.end().get_ptr());
	}

	class option_item_base : public ui::visible_region {
	public:
		event_option_pair data;
		void set_value(event_option_pair t) {
			data = t;
		}
	};

	class event_auto_button {
	public:
		int32_t value;
		pending_event e;

		template<typename window_type>
		void windowed_update(ui::simple_button<event_auto_button>& self, window_type& w, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
		void button_function(ui::simple_button<event_auto_button>& self, world_state& ws);
	};

	template <typename window_type>
	void event_auto_button::windowed_update(
		ui::simple_button<event_auto_button>& self,
		window_type& w,
		world_state& ws) {

		value = w.data.second;
		e = w.data.first;

		if((ws.s.event_m.event_container[e.e].flags & events::event::fire_only_once) != 0)
			ui::hide(*self.associated_object);
		else
			ui::make_visible_immediate(*self.associated_object);
	}

	class event_option_button {
	public:
		int32_t value;
		pending_event e;

		template<typename window_type>
		void windowed_update(ui::button<event_option_button>& self, window_type& w, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
		void button_function(ui::button<event_option_button>& self, world_state& ws);
	};

	template <typename window_type>
	void event_option_button::windowed_update(
		ui::button<event_option_button>& self,
		window_type& w,
		world_state& ws) {

		value = w.data.second;
		e = w.data.first;

		text_data::text_replacement rep_array[events::replacement_size];
		populate_replacements(ws, rep_array, e.event_for, e.event_from, text_data::text_color::dark_blue);

		self.set_text(ws, ws.s.event_m.event_container[e.e].options[value].name, rep_array, events::replacement_size);
	}

	using option_item = ui::gui_window<
		CT_STRING("auto_button"), ui::simple_button<event_auto_button>,
		CT_STRING("option_button"), ui::button<event_option_button>,
		option_item_base
	>;

	class province_event_info {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<province_event_info>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};
	class province_event_odds {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<province_event_odds>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class province_event_window_base : public ui::draggable_region {
	public:
		ui::discrete_listbox<province_options_listbox, option_item, event_option_pair> options;
		ui::dynamic_icon<province_event_info> event_info;
		ui::dynamic_icon<province_event_odds> event_odds;

		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class province_event_window_t : public ui::gui_window <
		CT_STRING("event_images"), ui::dynamic_icon<hidden_image>,
		CT_STRING("Title"), ui::display_text<province_event_title, -309>,
		CT_STRING("Date"), ui::display_text<hidden_text>,
		CT_STRING("Province"), ui::display_text<province_event_location, -95>,
		CT_STRING("Description"), ui::multiline_text<province_event_body>,
		CT_STRING("Event_Province_OptionButton"), ui::simple_button<hidden_button>,
		province_event_window_base
	> {};

	class nation_event_title {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_event_body {
	public:
		void update(ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class nation_event_image {
	public:
		graphics::texture* default_image = nullptr;

		void on_create(ui::dynamic_icon<nation_event_image>& self, world_state& ws);
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<nation_event_image>& self, window_type& w, world_state& ws);
	};

	class nation_options_listbox {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	template <typename lb_type>
	void nation_options_listbox::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<event_option_pair, 8, concurrent_allocator<event_option_pair>> possible_list;

		if(auto e = ws.w.nation_event_w.displayed_event.e; e) {
			auto const num_options = std::count_if(
				std::begin(ws.s.event_m.event_container[e].options),
				std::end(ws.s.event_m.event_container[e].options),
				[](event_option const& opt) { return is_valid_index(opt.name); });
			for(int32_t i = 0; i < num_options; ++i) {
				possible_list.emplace_back(ws.w.nation_event_w.displayed_event, i);
			}
		}

		lb.new_list(possible_list.begin().get_ptr(), possible_list.end().get_ptr());
	}

	class nation_event_info {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<nation_event_info>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};
	class nation_event_odds {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<nation_event_odds>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};

	class nation_event_window_base : public ui::draggable_region {
	public:
		ui::discrete_listbox<nation_options_listbox, option_item, event_option_pair> options;
		ui::dynamic_icon<nation_event_info> event_info;
		ui::dynamic_icon<nation_event_odds> event_odds;

		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class nation_event_window_t : public ui::gui_window <
		CT_STRING("event_images"), ui::dynamic_icon<nation_event_image>,
		CT_STRING("Title"), ui::display_text<nation_event_title>,
		CT_STRING("Date"), ui::display_text<hidden_text>,
		CT_STRING("Description"), ui::multiline_text<nation_event_body>,
		CT_STRING("Event_Country_OptionButton"), ui::simple_button<hidden_button>,
		nation_event_window_base
	> {};

	class major_event_title {
	public:
		void update(ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class major_event_body {
	public:
		void update(ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class major_event_flag {
	public:
		void update(ui::masked_flag<major_event_flag>& self, world_state& ws);
	};

	class major_options_listbox {
	public:
		template<typename lb_type>
		void populate_list(lb_type& lb, world_state& ws);
		ui::window_tag element_tag(ui::gui_static& m);
	};

	template <typename lb_type>
	void major_options_listbox::populate_list(lb_type& lb, world_state& ws) {
		boost::container::small_vector<event_option_pair, 8, concurrent_allocator<event_option_pair>> possible_list;

		if(auto e = ws.w.major_event_w.displayed_event.e; e) {
			auto const num_options = std::count_if(
				std::begin(ws.s.event_m.event_container[e].options),
				std::end(ws.s.event_m.event_container[e].options),
				[](event_option const& opt) { return is_valid_index(opt.name); });
			for(int32_t i = 0; i < num_options; ++i) {
				possible_list.emplace_back(ws.w.major_event_w.displayed_event, i);
			}
		}

		lb.new_list(possible_list.begin().get_ptr(), possible_list.end().get_ptr());
	}

	class major_event_info {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<major_event_info>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};
	class major_event_odds {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<major_event_odds>& self, window_type& win, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state&, ui::tagged_gui_object tw);
	};


	class major_event_window_base : public ui::draggable_region {
	public:
		ui::discrete_listbox<major_options_listbox, option_item, event_option_pair> options;
		ui::dynamic_icon<major_event_info> event_info;
		ui::dynamic_icon<major_event_odds> event_odds;

		template<typename W>
		void on_create(W& w, world_state& ws);
	};

	class major_event_window_t : public ui::gui_window <
		CT_STRING("country_flag1"), ui::masked_flag<major_event_flag>,
		CT_STRING("country_flag2"), ui::masked_flag<major_event_flag>,
		CT_STRING("Title"), ui::display_text<major_event_title>,
		CT_STRING("Date"), ui::display_text<hidden_text>,
		CT_STRING("Description"), ui::multiline_text<major_event_body>,
		CT_STRING("Event_Major_OptionButton"), ui::simple_button<hidden_button>,
		major_event_window_base
	> {};

	template<typename W>
	void province_event_window_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{370i16,540i16};
		{
			auto lb_tag = std::get<ui::listbox_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_province_event_list"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, lb_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				options));
			options.associated_object->position = ui::xy_pair{ 28i16,400i16 };
		}
		{
			auto icon_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_info_icon"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, icon_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				event_info));
		}
		{
			auto icon_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_odds_icon"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, icon_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				event_odds));
		}
		w.get<CT_STRING("Title")>().associated_object->position.x = 55i16;
		ui::hide(*associated_object);
	}

	template<typename W>
	void nation_event_window_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{ 585i16,705i16 };
		{
			auto lb_tag = std::get<ui::listbox_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_nation_event_list"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, lb_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				options));

			options.associated_object->position = ui::xy_pair{ 40i16,528i16 };
		}
		{
			auto icon_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_info_icon"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, icon_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				event_info));
		}
		{
			auto icon_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_odds_icon"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, icon_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				event_odds));
		}
		w.get<CT_STRING("Title")>().associated_object->position.x = 55i16;
		ui::hide(*associated_object);
	}

	template<typename W>
	void major_event_window_base::on_create(W & w, world_state & ws) {
		associated_object->size = ui::xy_pair{ 640i16,685i16 };
		{
			auto lb_tag = std::get<ui::listbox_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_nation_event_list"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, lb_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				options));

			options.associated_object->position = ui::xy_pair{ 68i16,510i16 };
		}
		{
			auto icon_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_info_icon"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, icon_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				event_info));
		}
		{
			auto icon_tag = std::get<ui::icon_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["open_v2_odds_icon"]);

			ui::move_to_front(ws.w.gui_m, ui::create_static_element(
				ws, icon_tag,
				ui::tagged_gui_object{ *associated_object, w.window_object },
				event_odds));
		}

		w.get<CT_STRING("country_flag1")>().associated_object->position += ui::xy_pair{ 21i16, -23i16 };
		w.get<CT_STRING("country_flag2")>().associated_object->position += ui::xy_pair{ 21i16, -23i16 };
		ui::hide(*associated_object);
	}

	template<typename window_type>
	void nation_event_image::windowed_update(ui::dynamic_icon<nation_event_image>& self, window_type & w, world_state & ws) {
		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {
			graphics::texture_tag picture = ws.s.event_m.event_container[e].picture;
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
	void province_event_odds::windowed_update(ui::dynamic_icon<province_event_odds>& self, window_type & win, world_state & ws) {
		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {
			if(auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen; chance_tag) {
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void province_event_info::windowed_update(ui::dynamic_icon<province_event_info>& self, window_type & win, world_state & ws) {
		if(auto const e = ws.w.province_event_w.displayed_event.e; e) {
			if(auto const t = ws.s.event_m.event_container[e].trigger; t) {
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void nation_event_info::windowed_update(ui::dynamic_icon<nation_event_info>& self, window_type & win, world_state & ws) {
		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {
			if(auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen; chance_tag) {
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void nation_event_odds::windowed_update(ui::dynamic_icon<nation_event_odds>& self, window_type & win, world_state & ws) {
		if(auto const e = ws.w.nation_event_w.displayed_event.e; e) {
			if(auto const t = ws.s.event_m.event_container[e].trigger; t) {
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void major_event_info::windowed_update(ui::dynamic_icon<major_event_info>& self, window_type & win, world_state & ws) {
		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {
			if(auto const chance_tag = ws.s.event_m.event_container[e].mean_time_to_happen; chance_tag) {
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		ui::hide(*self.associated_object);
	}

	template<typename window_type>
	void major_event_odds::windowed_update(ui::dynamic_icon<major_event_odds>& self, window_type & win, world_state & ws) {
		if(auto const e = ws.w.major_event_w.displayed_event.e; e) {
			if(auto const t = ws.s.event_m.event_container[e].trigger; t) {
				ui::make_visible_immediate(*self.associated_object);
				return;
			}
		}
		ui::hide(*self.associated_object);
	}

}
