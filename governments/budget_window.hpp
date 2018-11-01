#pragma once
#include "common\\common.h"
#include "budget_window.h"
#include "gui\\gui.hpp"
#include "governments_functions.h"
#include "economy\\economy_functions.h"
#include "world_state\\world_state.h"

namespace governments {
	class budget_window_base : public ui::draggable_region {
	public:
		template<typename window_type>
		void on_create(window_type& win, world_state& ws);
	};

	class bw_close_button {
	public:
		void button_function(ui::simple_button<bw_close_button>&, world_state& ws);
	};

	class tarrif_percent {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class poor_tax_collected {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class middle_tax_collected {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};
	class rich_tax_collected {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class gold_income {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class budget_window_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<bw_close_button>,
		CT_STRING("tariffs_percent"), ui::display_text<tarrif_percent>,
		CT_STRING("tax_0_inc"), ui::display_text<poor_tax_collected>,
		CT_STRING("tax_1_inc"), ui::display_text<middle_tax_collected>,
		CT_STRING("tax_2_inc"), ui::display_text<rich_tax_collected>,
		CT_STRING("gold_inc"), ui::display_text<gold_income>,
		budget_window_base
	> {};

	template<typename window_type>
	void budget_window_base::on_create(window_type & win, world_state & ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
	}
	template<typename W>
	void tarrif_percent::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::percent, float(player->tarrifs) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void poor_tax_collected::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, ws.w.local_player_data.collected_poor_tax * float(player->poor_tax) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void middle_tax_collected::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, ws.w.local_player_data.collected_middle_tax * float(player->middle_tax) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void rich_tax_collected::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto player = ws.w.local_player_nation; player) {
			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::currency, ws.w.local_player_data.collected_rich_tax * float(player->rich_tax) / 100.0f);
			ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void gold_income::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {}
}
