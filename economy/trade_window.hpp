#pragma once
#include "common\\common.h"
#include "trade_window.h"
#include "gui\\gui.hpp"
#include "world_state\\world_state.h"
#include "economy\\economy_functions.h"

namespace economy {
	class trade_window_base : public ui::draggable_region {
	public:
		template<typename window_type>
		void on_create(window_type& win, world_state& ws);
	};

	class tw_close_button {
	public:
		void button_function(ui::simple_button<tw_close_button>&, world_state& ws);
	};

	class tw_good_item_base : public ui::visible_region {
	public:
		goods_tag tag;

		template<typename window_type>
		void on_create(window_type& win, world_state& ws);
	};

	class tw_good_item_background  {
	public:
		goods_tag tag;

		template<typename window_type>
		void windowed_update(ui::simple_button<tw_good_item_background>& self, window_type& win, world_state& ws);
		void button_function(ui::simple_button<tw_good_item_background>& self, world_state& ws);
		bool has_tooltip(world_state&) { return true; }
		void create_tooltip(world_state& ws, ui::tagged_gui_object tw);
	};

	class tw_good_item_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::simple_button<tw_good_item_icon>& self, window_type& win, world_state& ws);
	};

	class tw_good_item_price {
	public:
		template<typename W>
		void windowed_update(W& w, ui::tagged_gui_object box, ui::text_box_line_manager& lm, ui::text_format& fmt, world_state& ws);
	};

	class tw_good_item_trend_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<tw_good_item_trend_icon>& self, window_type& win, world_state& ws);
	};

	class tw_good_item_buying_selling_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<tw_good_item_buying_selling_icon>& self, window_type& win, world_state& ws);
	};

	class tw_good_item_automation_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<tw_good_item_automation_icon>& self, window_type& win, world_state& ws);
	};

	using tw_good_item = ui::gui_window<
		CT_STRING("entry_button"), ui::simple_button<tw_good_item_background>,
		CT_STRING("goods_type"), ui::simple_button<tw_good_item_icon>,
		CT_STRING("price"), ui::display_text<tw_good_item_price>,
		CT_STRING("trend_indicator"), ui::dynamic_icon<tw_good_item_trend_icon>,
		CT_STRING("selling_indicator"), ui::dynamic_icon<tw_good_item_buying_selling_icon>,
		CT_STRING("automation_indicator"), ui::dynamic_icon<tw_good_item_automation_icon>,
		tw_good_item_base
	>;

	template<const char* group_name, size_t length>
	class goods_group_window_base : public ui::window_pane {
	public:
		std::vector<tw_good_item> member_goods;

		template<typename window_type>
		void on_create(window_type& win, world_state& ws);
	};

	inline const char group_raw_material_goods[] = "raw_material_goods";
	inline const char group_industrial_goods[] = "industrial_goods";
	inline const char group_consumer_goods[] = "consumer_goods";
	inline const char group_military_goods[] = "military_goods";

	class trade_details_pane_base : public ui::window_pane {
	public:
	};

	class tw_selected_good_icon {
	public:
		template<typename window_type>
		void windowed_update(ui::dynamic_icon<tw_selected_good_icon>& self, window_type& win, world_state& ws);
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
		void windowed_update(ui::display_barchart<prices_barchart>& self, window_type& win, world_state& ws);
	};

	using trade_details_pane = ui::gui_window<
		CT_STRING("goods_icon"), ui::dynamic_icon<tw_selected_good_icon>,
		CT_STRING("goods_title"), ui::display_text<tw_selected_good_name>,
		CT_STRING("goods_price"), ui::display_text<tw_selected_good_price>,
		CT_STRING("price_linechart"), ui::display_barchart<prices_barchart>,
		CT_STRING("price_chart_low"), ui::display_text<tw_good_global_min_price>,
		CT_STRING("price_chart_high"), ui::display_text<tw_good_global_max_price>,
		trade_details_pane_base
	>;

	class trade_window_t : public ui::gui_window<
		CT_STRING("close_button"), ui::simple_button<tw_close_button>,
		CT_STRING("group_raw_material_goods"), ui::gui_window<goods_group_window_base<group_raw_material_goods, sizeof(group_raw_material_goods) - 1>>,
		CT_STRING("group_industrial_goods"), ui::gui_window<goods_group_window_base<group_industrial_goods, sizeof(group_industrial_goods) - 1>>,
		CT_STRING("group_consumer_goods"), ui::gui_window<goods_group_window_base<group_consumer_goods, sizeof(group_consumer_goods) - 1>>,
		CT_STRING("group_military_goods"), ui::gui_window<goods_group_window_base<group_military_goods, sizeof(group_military_goods) - 1>>,
		CT_STRING("trade_details"), trade_details_pane,
		trade_window_base
	> {};

	template<typename window_type>
	void trade_window_base::on_create(window_type & win, world_state & ws) {
		associated_object->size = ui::xy_pair{ 1017i16, 636i16 };
		ui::for_each_child(ws.w.gui_m, ui::tagged_gui_object{ *associated_object, ui::gui_object_tag() }, [](ui::tagged_gui_object obj) {
			obj.object.position += ui::xy_pair{ -3i16, 3i16 };
		});
	}
	template<typename window_type>
	void tw_good_item_automation_icon::windowed_update(ui::dynamic_icon<tw_good_item_automation_icon>& self, window_type & win, world_state & ws) {}
	template<typename window_type>
	void tw_good_item_buying_selling_icon::windowed_update(ui::dynamic_icon<tw_good_item_buying_selling_icon>& self, window_type & win, world_state & ws) {}
	template<typename window_type>
	void tw_good_item_trend_icon::windowed_update(ui::dynamic_icon<tw_good_item_trend_icon>& self, window_type & win, world_state & ws) {}
	template<typename W>
	void tw_good_item_price::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto s = ws.w.trade_w.selected_state;
		if(!is_valid_index(s)) {
			if(auto player = ws.w.local_player_nation; player) {
				if(auto cap = player->current_capital; is_valid_index(cap)) {
					if(auto cap_state = ws.w.province_s.province_state_container[cap].state_instance; cap_state) {
						s = cap_state->id;
					}
				}
			}
		}
		if(!ws.w.nation_s.states.is_valid_index(s))
			return;

		
		auto prices = economy::state_current_prices(ws, s);

		char16_t local_buffer[16];
		put_value_in_buffer(local_buffer, display_type::fp_two_places, prices[to_index(w.tag)]);
		auto cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
		cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u00A3"), box, cursor, fmt, lm);
		lm.finish_current_line();
	}
	template<typename window_type>
	void tw_good_item_icon::windowed_update(ui::simple_button<tw_good_item_icon>& self, window_type & win, world_state & ws) {
		if(is_valid_index(win.tag)) {
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[win.tag].icon);
		}
	}
	template<typename window_type>
	void tw_good_item_background::windowed_update(ui::simple_button<tw_good_item_background>& self, window_type & win, world_state & ws) {
		tag = win.tag;
	}
	template<typename window_type>
	void tw_good_item_base::on_create(window_type & win, world_state & ws) {
		associated_object->size = ui::xy_pair{ 98i16, 32i16 };
	}

	template<const char * group_name, size_t length>
	template<typename window_type>
	void goods_group_window_base<group_name, length>::on_create(window_type & win, world_state & ws) {
		auto common_tag = std::get<ui::window_tag>(ws.s.gui_m.ui_definitions.name_to_element_map["goods_entry"]);
		auto group_name_tag = text_data::get_thread_safe_existing_text_handle(ws.s.gui_m.text_data_sequences, group_name, group_name + length);
		auto goods_category = [group_name_tag, &ws]() {
			for(goods_type_tag::value_base_t i = 0; i < ws.s.economy_m.good_type_names.size(); ++i) {
				if(ws.s.economy_m.good_type_names[goods_type_tag(i)] == group_name_tag)
					return goods_type_tag(i);
			}
			return goods_type_tag();
		}();
		
		constexpr int16_t size_x = 99i16;
		constexpr int16_t size_y = 33i16;
		
		int16_t cursor_x = 0;
		int16_t cursor_y = 0;

		uint32_t total_goods_in_category = std::accumulate(ws.s.economy_m.goods.begin(), ws.s.economy_m.goods.end(), 0ui32, [goods_category](uint32_t v, good_definition const& d){
			return v + uint32_t(d.type == goods_category);
		});

		member_goods.resize(total_goods_in_category);

		uint32_t nth_good = 0;

		for(goods_tag::value_base_t i = 0; i < ws.s.economy_m.goods_count; ++i) {
			if(ws.s.economy_m.goods[goods_tag(i)].type == goods_category) {
				member_goods[nth_good].tag = goods_tag(i);
				ui::move_to_front(ws.w.gui_m, ui::create_static_element(
					ws, common_tag,
					ui::tagged_gui_object{ *associated_object, win.window_object },
					member_goods[nth_good]));
				member_goods[nth_good].associated_object->position = ui::xy_pair{ cursor_x, cursor_y };
				cursor_x += size_x;
				if(cursor_x + size_x > associated_object->size.x) {
					cursor_x = 0;
					cursor_y += size_y;
				}
				++nth_good;
			}
		}
		if(cursor_x != 0)
			cursor_y += size_y;
		associated_object->size.y = cursor_y;
	}
	template<typename window_type>
	void tw_selected_good_icon::windowed_update(ui::dynamic_icon<tw_selected_good_icon>& self, window_type & win, world_state & ws) {
		if(auto g = ws.w.trade_w.selected_good; is_valid_index(g)) {
			self.set_frame(ws.w.gui_m, ws.s.economy_m.goods[g].icon);
		}
	}
	template<typename W>
	void tw_selected_good_price::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		auto s = ws.w.trade_w.selected_state;
		if(!is_valid_index(s)) {
			if(auto player = ws.w.local_player_nation; player) {
				if(auto cap = player->current_capital; is_valid_index(cap)) {
					if(auto cap_state = ws.w.province_s.province_state_container[cap].state_instance; cap_state) {
						s = cap_state->id;
					}
				}
			}
		}
		if(!ws.w.nation_s.states.is_valid_index(s))
			return;

		if(auto g = ws.w.trade_w.selected_good; is_valid_index(g)) {
			auto price = economy::state_current_prices(ws, s)[to_index(g)];

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::fp_two_places, price);
			auto cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u00A3"), box, cursor, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void tw_selected_good_name::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto g = ws.w.trade_w.selected_good; is_valid_index(g)) {
			ui::add_linear_text(ui::xy_pair{ 0,0 }, ws.s.economy_m.goods[g].name, fmt, ws.s.gui_m, ws.w.gui_m, box, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void tw_good_global_max_price::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto g = ws.w.trade_w.selected_good; is_valid_index(g)) {
			auto range = global_price_range(ws, g);

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::fp_two_places, range.maximum);
			auto cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u00A3"), box, cursor, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename W>
	void tw_good_global_min_price::windowed_update(W & w, ui::tagged_gui_object box, ui::text_box_line_manager & lm, ui::text_format & fmt, world_state & ws) {
		if(auto g = ws.w.trade_w.selected_good; is_valid_index(g)) {
			auto range = global_price_range(ws, g);

			char16_t local_buffer[16];
			put_value_in_buffer(local_buffer, display_type::fp_two_places, range.minimum - 0.01f);
			auto cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(local_buffer), box, ui::xy_pair{ 0,0 }, fmt, lm);
			cursor = ui::text_chunk_to_instances(ws.s.gui_m, ws.w.gui_m, vector_backed_string<char16_t>(u"\u00A3"), box, cursor, fmt, lm);
			lm.finish_current_line();
		}
	}
	template<typename window_type>
	void prices_barchart::windowed_update(ui::display_barchart<prices_barchart>& self, window_type & win, world_state & ws) {
		auto s = ws.w.trade_w.selected_state;
		if(!is_valid_index(s)) {
			if(auto player = ws.w.local_player_nation; player) {
				if(auto cap = player->current_capital; is_valid_index(cap)) {
					if(auto cap_state = ws.w.province_s.province_state_container[cap].state_instance; cap_state) {
						s = cap_state->id;
					}
				}
			}
		}
		if(!ws.w.nation_s.states.is_valid_index(s))
			return;

		if(auto g = ws.w.trade_w.selected_good; is_valid_index(g)) {
			auto price = economy::state_current_prices(ws, s)[to_index(g)];
			std::vector<money_qnty_type, concurrent_allocator<money_qnty_type>> state_prices;

			range_information result{ std::numeric_limits<money_qnty_type>::max(), std::numeric_limits<money_qnty_type>::lowest(), 0.0f };

			ws.w.nation_s.states.for_each([&ws, &result, &state_prices, g](nations::state_instance const& si) {
				if(auto sid = si.id; si.owner && ws.w.nation_s.states.is_valid_index(sid)) {
					auto prices = state_current_prices(ws, sid);
					auto v = prices[to_index(g)];
					state_prices.push_back(v);

					result.minimum = std::min(result.minimum, v);
					result.maximum = std::max(result.maximum, v);
				}
			});

			std::sort(state_prices.begin(), state_prices.end(), std::greater<money_qnty_type>());

			auto data = self.data(ws.w.gui_m);
			auto data_size = self.data_size();
			for(int32_t i = 0; i < data_size; ++i) {
				auto index = std::clamp(int32_t(float(i) * float(state_prices.size()) / float(data_size)), 0, int32_t(state_prices.size()));
				if(state_prices[index] < price) {
					data[i * 4 + 0] = uint8_t(255);
					data[i * 4 + 1] = uint8_t(55);
					data[i * 4 + 2] = uint8_t(55);
				} else {
					data[i * 4 + 0] = uint8_t(55);
					data[i * 4 + 1] = uint8_t(255);
					data[i * 4 + 2] = uint8_t(55);
				}
				auto percent = (state_prices[index] - (result.minimum - 0.01f)) / (result.maximum - (result.minimum - 0.01f));
				data[i * 4 + 3] = uint8_t(255.0f * percent);
			}
		}
		
	}
}
