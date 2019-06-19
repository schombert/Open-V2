#pragma once
#include "common\\common.h"
#include "gui\\gui.hpp"
#include "world_state\\world_state.h"
#include "military_gui.h"
#include "military_functions.h"

namespace military {
	/*
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
	*/
	/*
	using cb_item = ui::gui_window<
		CT_STRING("wargoal_icon"), ui::dynamic_icon<cb_type_icon>,
		CT_STRING("select_cb"), ui::button<cb_type_button>,
		cb_item_base
	>;*/

	/*
	class fabricate_cb_window_base : public ui::draggable_region {
	public:
		template<typename W>
		void on_create(W& w, world_state&);
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
		void update(ui::tagged_gui_object box, ui::line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	*/

	/*
	class fabricate_cb_window_t : public ui::gui_window <
		CT_STRING("Background"), ui::simple_button<hidden_button>,
		CT_STRING("AgreeButton"), ui::simple_button<ok_button>,
		CT_STRING("DeclineButton"), ui::simple_button<cancel_button>,
		CT_STRING("LeftShield"), ui::masked_flag<self_flag>,
		CT_STRING("RightShield"), ui::masked_flag<target_flag>,
		CT_STRING("Title"), ui::display_text<dialog_title>,
		CT_STRING("Description"), ui::multiline_text<description>,
		CT_STRING("cb_list"), ui::discrete_listbox<fabricate_cb_types_lb, cb_item, military::cb_type_tag, 24>,
		fabricate_cb_window_base
	> {};*/

#include "military_windows.h"

	template<typename lb_type>
	void fabricate_cb_types_lb::populate_list(lb_type & lb, world_state & ws) {
		boost::container::small_vector<military::cb_type_tag, 32, concurrent_allocator<military::cb_type_tag>> possible_list;

		if(auto player = ws.w.local_player_nation; player) {
			for(auto& cb : ws.s.military_m.cb_types) {
				if(is_cb_construction_valid_against(ws, cb.id, player, ws.w.fabricate_cb_w.target)) {
					possible_list.push_back(cb.id);
				}
			}
		}

		lb.update_list(possible_list.begin().get_ptr(), possible_list.end().get_ptr());
	}

	template<typename window_type>
	void cb_type_icon::windowed_update(ui::dynamic_icon<cb_type_icon>& self, window_type & w, world_state & ws) {
		auto icon = ws.s.military_m.cb_types[w.type].sprite_index;
		self.set_frame(ws.w.gui_m, icon != 0 ? uint32_t(icon - 1) : 0ui32);
	}

	template<typename window_type>
	void cb_type_button::windowed_update(ui::button<cb_type_button>& self, window_type & w, world_state & ws) {
		type = w.type;
		self.set_text(ws, ws.s.military_m.cb_types[w.type].name);
	}

	template<typename W>
	void fabricate_cb_window_base::on_create(W & w, world_state &) {
		ui::hide(*associated_object);
	}

	template<typename W>
	void cb_item_base::on_create(W& w, world_state& ws) {
	}

}
