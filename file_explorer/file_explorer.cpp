#include <iostream>
#include "simple_fs\\simple_fs.h"
#include <map>
#include <set>
#include "graphics\\v2_window.hpp"
#include "object_parsing\\object_parsing.hpp"
#include "Parsers\\parsers.hpp"
#include "gui\\gui.hpp"
#include "soil\\SOIL.h"
#include "graphics\\world_map.h"
#include <Windows.h>
#include "scenario\\scenario.h"
#include "scenario\\scenario_io.h"
#include "simple_serialize\\simple_serialize.hpp"
#include "world_state\\world_state.h"
#include "provinces\\province_functions.h"
#include "nations\\nations_functions.h"
#include "nations\\nations_io.h"
#include "military\\military_io.h"
#include "world_state\\world_state_io.h"
#include "ideologies\\ideologies_functions.h"
#include "population\\population_function.h"
#include "economy\\economy_functions.hpp"
#include "modifiers\\modifier_functions.h"
#include <random>

#undef max
#undef min
// #define RANGE(x) (x), (x) + (sizeof((x))/sizeof((x)[0])) - 1

int main(int, char**);

struct empty_window_handler {
	graphics::texture test_tex;
	graphics::texture strip_tex;
	graphics::texture mask_tex;
	graphics::texture prog1;
	graphics::texture prog2;
	graphics::texture bord;
	graphics::texture bar_tex;

	graphics::lines graph;

	graphics::font test_fallback;
	graphics::font test_font;

	empty_window_handler() :
		//test_tex("F:\\VS2007Projects\\open_v2_test_data\\army_icon_2.dds"),
		test_tex("D:\\VS2007Projects\\open_v2_test_data\\test_tx.bmp"),
		strip_tex("D:\\VS2007Projects\\open_v2_test_data\\strip10.dds"),
		mask_tex("D:\\VS2007Projects\\open_v2_test_data\\mask.tga"),
		prog1("D:\\VS2007Projects\\open_v2_test_data\\progress1.tga"),
		prog2("D:\\VS2007Projects\\open_v2_test_data\\progress2.tga"),
		bord("D:\\VS2007Projects\\open_v2_test_data\\border.dds"),
		bar_tex("D:\\VS2007Projects\\open_v2_test_data\\barchar.tga"),
		graph(10),
		test_fallback("D:\\VS2007Projects\\open_v2_test_data\\unifont-9.0.02.ttf"),
		//test_font("F:\\VS2007Projects\\open_v2_test_data\\Primitive.ttf", test_fallback) {}
		test_font("D:\\VS2007Projects\\open_v2_test_data\\CreteRound-Regular.otf", test_fallback) {
		float yval[] = { 0.3f, 0.6f, 0.5f, 1.0f, 0.4f, 0.5f, 0.0f, 0.3f, 0.2f, 0.6f };
		graph.set_y(yval);
	}

	template<typename T>
	void operator()(T&&, ui::window_base& w) const {
		// do nothing;
	}
	void initialize_graphics(graphics::open_gl_wrapper& ogl) {
		test_fallback.load_font(ogl);
		test_font.load_font(ogl);
	}

	void render(graphics::open_gl_wrapper& ogl) {
		ogl.render_outlined_text(u"明Tasy", 5, true, 80.0f, 40.0f, 32.0f, graphics::color{ 1.0f,1.0f,1.0f }, test_font);

		ogl.render_outlined_text(u"明Tesy", 5, true, 80.0f, 80.0f, 32.0f, graphics::color{ 0.0f,0.0f,0.0f }, test_font);
		ogl.render_text(u"明Tasy", 5, true, 80.0f, 112.0f, 32.0f, graphics::color{ 1.0f,1.0f,1.0f }, test_font);

		ogl.render_character(u'y', true, 140.0f, 60.0f, 128.0f, test_font);
		ogl.render_character(u'A', true, 10.0f, 138.0f, 512.0f, test_font);

	}
};

template<int n>
class mb_button {
public:
	void button_function(ui::gui_object_tag, world_state&) {
		MessageBoxA(nullptr, std::to_string(n).c_str(), "MB_A", MB_OK | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION);
	}
};

class tt_holder : public ui::draggable_region {
public:
	virtual ui::tooltip_behavior has_tooltip(ui::gui_object_tag, world_state&, const ui::mouse_move&) override { return ui::tooltip_behavior::tooltip; }
	virtual void create_tooltip(ui::gui_object_tag, world_state& ws, const ui::mouse_move&, ui::tagged_gui_object tw) override {
		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(u"test tooltip"),
			tw,
			ui::xy_pair{ 0,0 },
			ui::text_format{ ui::text_color::white, graphics::font_tag(1), 16 },
			ui::single_line_manager());
	}
};

class debug_scrollbar {
public:
	void on_position(int32_t pos) {
		OutputDebugStringA("on_position; ");
		OutputDebugStringA(std::to_string(pos).c_str());
		OutputDebugStringA("\n");
	}
};

class empty_gui_obj {

};

class simple_button_group {
public:
	void on_select(world_state&, uint32_t i) {
		MessageBoxA(nullptr, std::to_string(i).c_str(), "GROUP BUTTON", MB_OK | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION);
	}
};

class debt_list_item_base : public ui::visible_region {
public:
	const int value;
	debt_list_item_base(int v) : value(v) {}
	debt_list_item_base(debt_list_item_base&&) = default;
	debt_list_item_base(debt_list_item_base& b) noexcept : debt_list_item_base(std::move(b)) {}
};

class debt_tb_who {
public:
	template<typename ...PARAMS>
	debt_tb_who(PARAMS&&...) {}

	template<typename window_type>
	void windowed_update(window_type& w, ui::tagged_gui_object obj, text_data::alignment align, ui::text_format& fmt, world_state& ws) {
		char16_t lbuffer[8] = { 0,0,0,0,0,0,0,0 };
		u16itoa(w.value, lbuffer);

		ui::line_manager lm(align, obj.object.size.x);
		const auto cursor = ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(u"who: "),
			obj,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);
		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(lbuffer),
			obj,
			cursor,
			fmt,
			lm);
		lm.finish_current_line();
	}
};

class debt_tb_amount {
public:
	template<typename ...PARAMS>
	debt_tb_amount(PARAMS&&...) {}

	template<typename window_type>
	void windowed_update(window_type& w, ui::tagged_gui_object obj, text_data::alignment align, ui::text_format& fmt, world_state& ws) {
		char16_t lbuffer[8] = { 0,0,0,0,0,0,0,0 };
		u16itoa(w.value, lbuffer);

		ui::line_manager lm(align, obj.object.size.x);
		const auto cursor = ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(u"$"),
			obj,
			ui::xy_pair{ 0,0 },
			fmt,
			lm);
		ui::text_chunk_to_instances(
			ws.s.gui_m,
			ws.w.gui_m,
			vector_backed_string<char16_t>(lbuffer),
			obj,
			cursor,
			fmt,
			lm);
		lm.finish_current_line();
	}
};

using debt_listitem_t = ui::gui_window<
	CT_STRING("who"), ui::display_text<debt_tb_who>,
	CT_STRING("debt"), ui::display_text<debt_tb_amount>,
	debt_list_item_base
>;

class debt_lb {
public:
	template<typename lb_type>
	void populate_list(lb_type& lb, world_state& ws) {
		lb.add_item(ws, 1);
		lb.add_item(ws, 2);
		lb.add_item(ws, 3);
		lb.add_item(ws, 4);
		lb.add_item(ws, 5);
		lb.add_item(ws, 6);
		lb.add_item(ws, 7);
		lb.add_item(ws, 8);
		lb.add_item(ws, 9);
	}
	ui::window_tag element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["debt_listitem"]);
	}
};

class hidden_icon {
public:
	void update(ui::dynamic_icon<hidden_icon>& ico, world_state& ws) {
		ico.set_visibility(ws.w.gui_m, false);
	}
};

class frame_button {
public:
	const uint32_t n;

	frame_button(uint32_t num) : n(num) {}
	void update(ui::simple_button<frame_button>& ico, world_state& ws) {
		ico.set_frame(ws.w.gui_m, n);
	}
	void button_function(ui::gui_object_tag, world_state&) {
		MessageBoxA(nullptr, std::to_string(n).c_str(), "MB_FB", MB_OK | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_ICONINFORMATION);
	}
};

using pop_item_t = ui::gui_window<
	CT_STRING("pop"), ui::simple_button<frame_button>,
	ui::visible_region
>;

class pop_type_a {
public:
	template<typename lb_type>
	void populate_list(lb_type& lb, world_state& ws) {
		lb.add_item(ws, 0ui32);
		lb.add_item(ws, 1ui32);
		lb.add_item(ws, 2ui32);
		lb.add_item(ws, 3ui32);
		lb.add_item(ws, 4ui32);
	}
	ui::window_tag element_tag(ui::gui_static& m) {
		return std::get<ui::window_tag>(m.ui_definitions.name_to_element_map["pop_listitem"]);
	}
};

using budget_window_t = ui::gui_window <
	CT_STRING("tab_takenloans"), ui::button_group_member,
	CT_STRING("tab_givenloans"), ui::button_group_member,
	CT_STRING("loans_tab_group"), ui::button_group<CT_STRING("tab_takenloans"), CT_STRING("tab_givenloans"), simple_button_group>,
	CT_STRING("debt_sort_country"), ui::simple_button<mb_button<3>>,
	CT_STRING("debt_sort_amount"), ui::simple_button<mb_button<4>>,
	CT_STRING("tax_0_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tax_1_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tax_2_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("land_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("naval_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("projects_stockpile_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_0_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_1_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_2_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("exp_3_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("tariff_slider"), ui::scrollbar<debug_scrollbar>,
	CT_STRING("close_button"), ui::simple_button<mb_button<5>>,
	CT_STRING("take_loan"), ui::simple_button<mb_button<6>>,
	CT_STRING("repay_loan"), ui::simple_button<mb_button<7>>,
	CT_STRING("chart_0"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_1"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_2"), ui::piechart<empty_gui_obj>,
	CT_STRING("chart_debt"), ui::piechart<empty_gui_obj>,
	CT_STRING("debt_listbox"), ui::display_listbox<debt_lb, debt_listitem_t, 56>,
	CT_STRING("gunboat_alert"), ui::dynamic_icon<hidden_icon>,
	CT_STRING("tax_0_pops"), ui::overlap_box<pop_type_a, ui::window_tag, pop_item_t, 32>,
	ui::draggable_region > ;

struct gui_window_handler {
	world_state& s;
	std::string shadows_file;

	graphics::map_display map;
	Eigen::Vector3f interest = Eigen::Vector3f::UnitX();
	bool map_dragging = false;
	std::pair<int32_t, int32_t> map_drag_start;

	//ui::simple_button<mb_button> mb_button_a;
	tt_holder mb_button_a;
	ui::scrollbar<debug_scrollbar> test_sb;

	//budget_window_t budget_window;

	gui_window_handler(world_state& snm, std::string const& shadows) : s(snm), shadows_file(shadows) {}

	template<typename T>
	void operator()(const T&, ui::window_base& w) const {
		// do nothing;
	}

	void operator()(const ui::creation&, ui::window_base&) {
		s.w.init_gui_objects(s);
	}

	void operator()(const ui::resize& r, ui::window_base&) {
		s.w.gui_m.on_resize(r);
		map.state.resize(static_cast<int32_t>(r.width), static_cast<int32_t>(r.height));
		s.w.topbar_w.resize_topbar(s.w.gui_m);
	}

	void operator()(const ui::lbutton_down& m, ui::window_base&) {
		if (!s.w.gui_m.on_lbutton_down(s, m)) {
			map_dragging = true;
			map_drag_start = std::make_pair(m.x, m.y);
			interest = map.state.get_vector_for(map.state.normalize_screen_coordinates(m.x, m.y, s.w.gui_m.width(), s.w.gui_m.height()));
		}
	}
	void operator()(const ui::lbutton_up& m, ui::window_base&) {
		if(map_dragging) {
			map_dragging = false;

			if(std::abs(map_drag_start.first - m.x) < 5 && std::abs(map_drag_start.second - m.y) < 5) {
				auto map_coord = map.map_coordinates_from_screen(map.state.normalize_screen_coordinates(m.x, m.y, s.w.gui_m.width(), s.w.gui_m.height()));
				auto id = s.s.province_m.province_map_data[size_t(map_coord.first + map_coord.second * s.s.province_m.province_map_width)];
				s.w.province_w.show_province_window(s.w.gui_m, provinces::province_tag(id));
				s.w.map_view.selected_province = provinces::province_tag(id);
				if(is_valid_index(provinces::province_tag(id))) {
					auto& ps = s.w.province_s.province_state_container[provinces::province_tag(id)];
					if(auto si = ps.state_instance; si) {
						if(auto sid = si->id; s.w.nation_s.states.is_valid_index(sid) && s.w.map_view.selected_state != sid) {
							s.w.map_view.selected_state = sid;
							s.w.trade_w.selected_state = sid;
							s.w.trade_w.update(s.w.gui_m);
						}
					}
					if(auto n = ps.owner; n) {
						if(auto nid = n->id; s.w.nation_s.nations.is_valid_index(nid))
							s.w.map_view.selected_country = nid;
					}
					s.w.map_view.changed = true;
				}
			}
		}
	}
	void operator()(const ui::key_down& m, ui::window_base&) {
		if(m.keycode == virtual_key::NUMPAD1 || m.keycode == virtual_key::NUM_1) {
			s.w.map_view.mode = current_state::map_mode::political;
			s.w.map_view.changed = true;
		} else if(m.keycode == virtual_key::NUMPAD2 || m.keycode == virtual_key::NUM_2) {
			s.w.map_view.mode = current_state::map_mode::distance;
			s.w.map_view.changed = true;
		} else if(m.keycode == virtual_key::NUMPAD3 || m.keycode == virtual_key::NUM_3) {
			s.w.map_view.mode = current_state::map_mode::prices;
			s.w.map_view.changed = true;
		} else if(m.keycode == virtual_key::NUMPAD4 || m.keycode == virtual_key::NUM_4) {
			s.w.map_view.mode = current_state::map_mode::purchasing;
			s.w.map_view.changed = true;
		} else if(m.keycode == virtual_key::NUMPAD5 || m.keycode == virtual_key::NUM_5) {
			s.w.map_view.mode = current_state::map_mode::production;
			s.w.map_view.changed = true;
		}
		s.w.gui_m.on_keydown(s, m);
	}
	void operator()(const ui::scroll& ss, ui::window_base&) {
		if (!s.w.gui_m.on_scroll(s, ss)) {
			map.state.scale *= float(pow(2, ss.amount / 2.0f));
			map.state.scale = std::clamp(map.state.scale, 1.0f, 18.0f);
		}
	}
	void operator()(const ui::mouse_drag& m, ui::window_base&) {
		if (!map_dragging) {
			s.w.gui_m.on_mouse_drag(s, m);
		} else {
			Eigen::Vector3f mouse_over = map.state.get_unrotated_vector_for(map.state.normalize_screen_coordinates(map_drag_start.first + m.x, map_drag_start.second + m.y, s.w.gui_m.width(), s.w.gui_m.height()));
			map.state.move_vector_to(interest, mouse_over);
		}
	}
	void operator()(const ui::mouse_move& m, ui::window_base&) {
		s.w.gui_m.on_mouse_move(s, m);
	}
	void operator()(const ui::text_event& t, ui::window_base&) {
		if(t.text == u'.') {
			economy::economy_update_tick(s);
			s.w.current_date = date_tag(to_index(s.w.current_date) + 1);
			s.w.gui_m.flag_update();
			s.w.map_view.changed = true;
		} else {
			s.w.gui_m.on_text(s, t);
		}
	}
	void initialize_graphics(graphics::open_gl_wrapper& ogl) {
		s.s.gui_m.fonts.load_fonts(ogl);

		map.colors.init_color_data(static_cast<uint32_t>(s.s.province_m.province_container.size()));

		const auto pcolors = map.colors.primary_color_data();
		const auto scolors = map.colors.secondary_color_data();

		for(size_t i = 0; i < s.s.province_m.province_container.size(); ++i) {
			const provinces::province_tag this_province(static_cast<provinces::province_tag::value_base_t>(i));
			provinces::province& province_object = s.s.province_m.province_container[this_province];
			if(province_object.flags == uint16_t(provinces::province::lake | provinces::province::sea)) {
				pcolors[i * 3 + 0] = 0ui8;
				pcolors[i * 3 + 1] = 0ui8;
				pcolors[i * 3 + 2] = 175ui8;
				scolors[i * 3 + 0] = 0ui8;
				scolors[i * 3 + 1] = 0ui8;
				scolors[i * 3 + 2] = 175ui8;
			} else if(province_object.flags == provinces::province::sea) {
				pcolors[i * 3 + 0] = 80ui8;
				pcolors[i * 3 + 1] = 80ui8;
				pcolors[i * 3 + 2] = 255ui8;
				scolors[i * 3 + 0] = 80ui8;
				scolors[i * 3 + 1] = 80ui8;
				scolors[i * 3 + 2] = 255ui8;
			} else if(s.w.province_s.province_state_container[this_province].owner) {
				auto owner = s.w.province_s.province_state_container[this_province].owner;
				pcolors[i * 3 + 0] = owner->current_color.r;
				pcolors[i * 3 + 1] = owner->current_color.g;
				pcolors[i * 3 + 2] = owner->current_color.b;
				scolors[i * 3 + 0] = owner->current_color.r;
				scolors[i * 3 + 1] = owner->current_color.g;
				scolors[i * 3 + 2] = owner->current_color.b;
			} else if(province_object.flags == uint16_t(provinces::province::sea | provinces::province::coastal)) {
				pcolors[i * 3 + 0] = 150ui8;
				pcolors[i * 3 + 1] = 150ui8;
				pcolors[i * 3 + 2] = 255ui8;
				scolors[i * 3 + 0] = 150ui8;
				scolors[i * 3 + 1] = 150ui8;
				scolors[i * 3 + 2] = 255ui8;
			} else if(province_object.flags == uint16_t(provinces::province::coastal)) {
				pcolors[i * 3 + 0] = 150ui8;
				pcolors[i * 3 + 1] = 255ui8;
				pcolors[i * 3 + 2] = 150ui8;
				scolors[i * 3 + 0] = 150ui8;
				scolors[i * 3 + 1] = 255ui8;
				scolors[i * 3 + 2] = 150ui8;
			} else {
				pcolors[i * 3 + 0] = 50ui8;
				pcolors[i * 3 + 1] = 205ui8;
				pcolors[i * 3 + 2] = 50ui8;
				scolors[i * 3 + 0] = 50ui8;
				scolors[i * 3 + 1] = 205ui8;
				scolors[i * 3 + 2] = 50ui8;
				 
				/*const auto terrain = province_object.terrain;
				if(!is_valid_index(terrain)) {
					pcolors[i * 3 + 0] = 0ui8;
					pcolors[i * 3 + 1] = 0ui8;
					pcolors[i * 3 + 2] = 0ui8;
					scolors[i * 3 + 0] = 0ui8;
					scolors[i * 3 + 1] = 0ui8;
					scolors[i * 3 + 2] = 0ui8;
				} else {
					pcolors[i * 3 + 0] = uint8_t(18 * (to_index(terrain) - 98));
					pcolors[i * 3 + 1] = uint8_t(18 * (to_index(terrain) - 98));
					pcolors[i * 3 + 2] = uint8_t(18 * (to_index(terrain) - 98));
					scolors[i * 3 + 0] = uint8_t(18 * (to_index(terrain) - 98));
					scolors[i * 3 + 1] = uint8_t(18 * (to_index(terrain) - 98));
					scolors[i * 3 + 2] = uint8_t(18 * (to_index(terrain) - 98));
				}*/
			}
		}
		map.colors.update_ready();

		//map.initialize(ogl, s.province_m.province_map_data.data(), s.province_m.province_map_width, s.province_m.province_map_height, 0.0f, -1.2f, 1.2f);
		map.initialize(ogl, shadows_file, s.s.province_m.province_map_data.data(), s.s.province_m.province_map_width, s.s.province_m.province_map_height, 0.0f, 1.57f, -1.57f);

		map.state.resize(s.w.gui_m.width(), s.w.gui_m.height());
	}

	bool on_idle() {
		bool map_change_expected = true;
		if(s.w.map_view.changed.compare_exchange_strong(map_change_expected, false)) {
			if(auto g = s.w.map_view.selected_good; is_valid_index(g) && s.w.map_view.mode == current_state::map_mode::prices) {
				auto price_range = economy::global_price_range(s, g);

				const auto pcolors = map.colors.primary_color_data();
				const auto scolors = map.colors.secondary_color_data();

				for(uint32_t i = 0; i < s.w.province_s.province_state_container.size(); ++i) {
					if(auto si = s.w.province_s.province_state_container[provinces::province_tag(provinces::province_tag::value_base_t(i))].state_instance; si && si->owner) {
						if(auto sid = si->id; s.w.nation_s.states.is_valid_index(sid)) {
							auto local_prices = economy::state_current_prices(s, sid);

							auto fraction = (local_prices[to_index(g)] - price_range.minimum + 0.01) / (price_range.maximum - price_range.minimum + 0.01);
							pcolors[i * 3 + 0] = uint8_t((1.0f - fraction) * 255.0f);
							pcolors[i * 3 + 1] = uint8_t(fraction * 255.0f);
							pcolors[i * 3 + 2] = uint8_t(100);
							scolors[i * 3 + 0] = uint8_t((1.0f - fraction) * 255.0f);
							scolors[i * 3 + 1] = uint8_t(fraction * 255.0f);
							scolors[i * 3 + 2] = uint8_t(100);
						}
					}
				}

				map.colors.update_ready();
			} else if(auto g = s.w.map_view.selected_good; is_valid_index(g) && g != economy::money_good && s.w.map_view.mode == current_state::map_mode::purchasing) {
				if(auto sid = s.w.map_view.selected_state; s.w.nation_s.states.is_valid_index(sid)) {
					auto purchasing_handle = s.w.nation_s.state_purchases.get(sid, g);
					auto purchases_data_range = get_range(s.w.economy_s.purchasing_arrays, purchasing_handle.money_for_purchases);
					auto count_purchases = purchases_data_range.second - purchases_data_range.first;

					if(purchases_data_range.first != purchases_data_range.second) {
						auto max_purchases = *std::max_element(purchases_data_range.first, purchases_data_range.second);

						const auto pcolors = map.colors.primary_color_data();
						const auto scolors = map.colors.secondary_color_data();

						for(uint32_t i = 0; i < s.w.province_s.province_state_container.size(); ++i) {
							if(auto si = s.w.province_s.province_state_container[provinces::province_tag(provinces::province_tag::value_base_t(i))].state_instance; si && si->owner) {
								if(auto sid = si->id; s.w.nation_s.states.is_valid_index(sid) && to_index(sid) < count_purchases) {
									auto amount = purchases_data_range.first[to_index(sid)];

									auto fraction = amount / (max_purchases + 0.000001f);
									if(fraction < 0)
										std::abort();
									pcolors[i * 3 + 0] = uint8_t(fraction * 205.0f + 50.0f);
									pcolors[i * 3 + 1] = uint8_t(fraction * 205.0f + 50.0f);
									pcolors[i * 3 + 2] = uint8_t(fraction * 205.0f + 50.0f);
									scolors[i * 3 + 0] = uint8_t(fraction * 205.0f + 50.0f);
									scolors[i * 3 + 1] = uint8_t(fraction * 205.0f + 50.0f);
									scolors[i * 3 + 2] = uint8_t(fraction * 205.0f + 50.0f);
								}
							}
						}

						map.colors.update_ready();
					}
				}
			} else if(auto g = s.w.map_view.selected_good; is_valid_index(g) && g != economy::money_good && s.w.map_view.mode == current_state::map_mode::production) {
				economy::goods_qnty_type max_production = 0.0001;
				s.w.nation_s.states.for_each([&](nations::state_instance const& si) {
					if(auto id = si.id; s.w.nation_s.states.is_valid_index(id))
						max_production = std::max(max_production, economy::state_current_production(s, id)[to_index(g)]);
				});
				
				const auto pcolors = map.colors.primary_color_data();
				const auto scolors = map.colors.secondary_color_data();

				for(uint32_t i = 0; i < s.w.province_s.province_state_container.size(); ++i) {
					if(auto si = s.w.province_s.province_state_container[provinces::province_tag(provinces::province_tag::value_base_t(i))].state_instance; si && si->owner) {
						if(auto sid = si->id; s.w.nation_s.states.is_valid_index(sid)) {
							auto fraction = economy::state_current_production(s, sid)[to_index(g)] / max_production;
							if(fraction < 0)
								std::abort();
							pcolors[i * 3 + 0] = uint8_t(fraction * 205.0f + 50.0f);
							pcolors[i * 3 + 1] = uint8_t(fraction * 205.0f + 50.0f);
							pcolors[i * 3 + 2] = uint8_t(fraction * 205.0f + 50.0f);
							scolors[i * 3 + 0] = uint8_t(fraction * 205.0f + 50.0f);
							scolors[i * 3 + 1] = uint8_t(fraction * 205.0f + 50.0f);
							scolors[i * 3 + 2] = uint8_t(fraction * 205.0f + 50.0f);
						}
					}
				}

				map.colors.update_ready();

			} else if(auto p = s.w.map_view.selected_province; is_valid_index(p) && s.w.map_view.mode == current_state::map_mode::distance) {
				const auto pcolors = map.colors.primary_color_data();
				const auto scolors = map.colors.secondary_color_data();

				auto pcount = s.w.province_s.province_state_container.size();
				for(uint32_t i = 0; i < pcount; ++i) {
					auto distance = s.w.province_s.province_distance_to[to_index(p) * pcount + i];
					pcolors[i * 3 + 0] = uint8_t(std::clamp((distance / 8'000.0f - 1.0f), 0.0f, 1.0f) * 255.0f);
					pcolors[i * 3 + 1] = uint8_t(std::clamp((1.0f - distance / 8'000.0f), 0.0f, 1.0f) * 255.0f);
					pcolors[i * 3 + 2] = uint8_t(100);
					scolors[i * 3 + 0] = uint8_t(std::clamp((distance / 8'000.0f - 1.0f), 0.0f, 1.0f) * 255.0f);
					scolors[i * 3 + 1] = uint8_t(std::clamp((1.0f - distance / 8'000.0f), 0.0f, 1.0f) * 255.0f);
					scolors[i * 3 + 2] = uint8_t(100);
				}

				map.colors.update_ready();
			} else {
				const auto pcolors = map.colors.primary_color_data();
				const auto scolors = map.colors.secondary_color_data();

				for(size_t i = 0; i < s.s.province_m.province_container.size(); ++i) {
					const provinces::province_tag this_province(static_cast<provinces::province_tag::value_base_t>(i));
					provinces::province& province_object = s.s.province_m.province_container[this_province];
					if((province_object.flags & provinces::province::sea) != 0) {
						pcolors[i * 3 + 0] = 80ui8;
						pcolors[i * 3 + 1] = 80ui8;
						pcolors[i * 3 + 2] = 255ui8;
						scolors[i * 3 + 0] = 80ui8;
						scolors[i * 3 + 1] = 80ui8;
						scolors[i * 3 + 2] = 255ui8;
					} else if(auto owner = s.w.province_s.province_state_container[this_province].owner; owner) {
						pcolors[i * 3 + 0] = owner->current_color.r;
						pcolors[i * 3 + 1] = owner->current_color.g;
						pcolors[i * 3 + 2] = owner->current_color.b;
						scolors[i * 3 + 0] = owner->current_color.r;
						scolors[i * 3 + 1] = owner->current_color.g;
						scolors[i * 3 + 2] = owner->current_color.b;
					} else {
						pcolors[i * 3 + 0] = 255ui8;
						pcolors[i * 3 + 1] = 255ui8;
						pcolors[i * 3 + 2] = 255ui8;
						scolors[i * 3 + 0] = 255ui8;
						scolors[i * 3 + 1] = 255ui8;
						scolors[i * 3 + 2] = 255ui8;
					}
				}

				map.colors.update_ready();
			}
		}

		if (s.w.gui_m.check_and_clear_update()) {
			ui::update(s);
			return true;
		} else if (s.w.gui_m.check_and_clear_minimal_update()) {
			ui::minimal_update(s);
			return true;
		} else {
			return map_change_expected;
		}
	}

	void render(graphics::open_gl_wrapper& ogl) {
		map.render(ogl);
		ui::render(s.s.gui_m, s.w.gui_m, ogl);
	}
};

auto fake_text_handle_lookup();
auto fake_font_handle_lookup();
auto fake_gobj_lookup();

auto fake_text_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_font_handle_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}
auto fake_gobj_lookup() {
	return[i = 0ui16](const char*, const char*) mutable { return ++i; };
}

int main(int , char **) {


	/*
	const int32_t dimension = 100;

	std::vector<float> production_qnty(dimension);
	std::vector<float> base_spending(dimension);

	auto aligned_dimension_size = ((static_cast<uint32_t>(sizeof(economy::money_qnty_type)) * uint32_t(dimension) + 31ui32) & ~31ui32) / static_cast<uint32_t>(sizeof(economy::money_qnty_type));
	auto padded_aligned_size = aligned_dimension_size + 32ui32 / sizeof(economy::money_qnty_type);

	std::vector<float, concurrent_allocator<float>> values_v(padded_aligned_size);
	std::vector<float, concurrent_allocator<float>> first_derivatives_v(padded_aligned_size);
	std::vector<float, concurrent_allocator<float>> second_derivatives_v(padded_aligned_size);

	size_t space = padded_aligned_size * sizeof(economy::money_qnty_type);
	void* ptr = values_v.data();
	Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> values(
		(economy::money_qnty_type*)std::align(32, aligned_dimension_size * sizeof(economy::money_qnty_type), ptr, space), aligned_dimension_size);
	
	space = padded_aligned_size * sizeof(economy::money_qnty_type);
	ptr = first_derivatives_v.data();
	Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> first_derivatives(
		(economy::money_qnty_type*)std::align(32, aligned_dimension_size * sizeof(economy::money_qnty_type), ptr, space), aligned_dimension_size);

	space = padded_aligned_size * sizeof(economy::money_qnty_type);
	ptr = second_derivatives_v.data();
	Eigen::Map<Eigen::Matrix<economy::money_qnty_type, 1, -1>, Eigen::Aligned32> second_derivatives(
		(economy::money_qnty_type*)std::align(32, aligned_dimension_size * sizeof(economy::money_qnty_type), ptr, space), aligned_dimension_size);

	values[0] = 200.0f;

	std::uniform_real_distribution<float> dist(10.0f, 400.0f);
	std::uniform_real_distribution<float> pdist(0.0f, 50.0f);
	auto& gen = get_local_generator();

	for(int32_t i = 0; i < dimension; ++i) {
		base_spending[i] = dist(gen);
		auto pq = pdist(gen);
		if(pq >= 25.0f)
			production_qnty[i] = pq;
	}

	std::cout << "base value: " << production_qnty[0] * values[0] / (base_spending[0] + values[0]) << std::endl << std::flush;

	for(uint32_t i = 0; i < 400; ++i) {

		first_derivatives[0] = base_spending[0] * production_qnty[0] / ((base_spending[0] + values[0])*(base_spending[0] + values[0]));
		second_derivatives[0] = -2.0f * base_spending[0] * production_qnty[0] / ((base_spending[0] + values[0])*(base_spending[0] + values[0])* (base_spending[0] + values[0]));

		for(uint32_t i = 1; i < dimension; ++i) {
			auto ufactor = i * 0.01f;
			auto iterm = base_spending[i] + values[i] + production_qnty[i] * ufactor;
			auto cterm = -4.0f * values[i] * production_qnty[i] * ufactor + iterm * iterm;

			first_derivatives[i] = 1.0f / (2.0f * ufactor) + (-base_spending[i] - values[i] + production_qnty[i] * ufactor) / (2.0f * ufactor * sqrt(cterm));
			second_derivatives[i] = -2.0f * base_spending[i] * production_qnty[i] / std::pow(cterm, 1.5f);

			if(values[i] < 0.0001f) {
				values[i] = 0;
				if(first_derivatives[i] < 0) {
					first_derivatives[i] = 0;
					second_derivatives[i] = 0;
				}
			}
		}

		economy::perform_cg_step(values, first_derivatives, second_derivatives, uint32_t(aligned_dimension_size), 1.0f);
		auto new_value = std::transform_reduce(integer_iterator(1), integer_iterator(dimension),
			production_qnty[0] * values[0] / (base_spending[0] + values[0]), std::plus<float>(),
			[&production_qnty, &base_spending, &values](int32_t i) {
			auto ufactor = i * 0.01f;
			auto iterm = base_spending[i] + values[i] + production_qnty[i] * ufactor;
			return (base_spending[i] + values[i] + production_qnty[i] * ufactor - sqrt(-4.0f * values[i] * production_qnty[i] * ufactor + iterm * iterm)) / (2.0f * ufactor);
		});

		bool all_valid = std::transform_reduce(integer_iterator(0), integer_iterator(dimension), true,
			[](bool a, bool b) { return a && b; }, [&values](int32_t i) { return values[i] >= 0; });
	
		std::cout << "value: " << new_value << " total: " << values.sum() << " all valid: " << all_valid << std::endl << std::flush;
		for(int32_t j = 0; j < dimension; ++j) {
			if(values[j] < 0) {
				std::cout << "invalid: " << j << " value: " << values[j] << std::endl << std::flush;
			}
		}
	}
	*/

	file_system fs;
	fs.set_root(u"D:\\programs\\V2");
	fs.add_root(u"D:\\programs\\V2\\mod\\OpenV2");
	//ui::gui_manager gui_m(850, 650);

	if(DWORD dwAttrib = GetFileAttributes((const wchar_t*)(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin")); dwAttrib == INVALID_FILE_ATTRIBUTES) {
		scenario::scenario_manager s1;

		std::cout << "begin scenario read" << std::endl << std::flush;
		scenario::read_scenario(s1, fs.get_root());
		std::cout << "end scenario read" << std::endl << std::flush;

		std::cout << "begin map read" << std::endl << std::flush;
		provinces::load_province_map_data(s1.province_m, fs.get_root());

		std::cout << "end map read" << std::endl << std::flush;

		std::cout << "begin map adjacency" << std::endl << std::flush;
		auto adj_map = provinces::generate_map_adjacencies(s1.province_m.province_map_data.data(), s1.province_m.province_map_height, s1.province_m.province_map_width);
		provinces::read_adjacnencies_file(adj_map, s1.province_m.canals, fs.get_root(), s1.gui_m.text_data_sequences);
		provinces::make_lakes(adj_map, s1.province_m);
		provinces::make_adjacency(adj_map, s1.province_m);
		std::cout << "end map adjacency" << std::endl << std::flush;

		provinces::calculate_province_areas(s1.province_m, 1.57f, -1.57f);

		const auto s_size = serialization::serialize_size(s1);
		std::cout << s_size << " bytes " << s_size / 1024 << " KB " << s_size / (1024 * 1024) << " MB" << std::endl;


		std::cout << "begin serialize" << std::endl << std::flush;
		serialization::serialize_file_header dummy;
		serialization::serialize_to_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", true, dummy, s1);
		std::cout << "end serialize" << std::endl << std::flush;
	}

	world_state ws;

	std::cout << "begin deserialize" << std::endl << std::flush;
	concurrency::task_group tg;
	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_scenario.bin", ws.s, tg);
	tg.wait();
	std::cout << "end deserialize" << std::endl << std::flush;

	scenario::ready_scenario(ws.s, fs.get_root()); // ready gui fonts and sound
	ready_world_state(ws);

	if(DWORD dwAttrib = GetFileAttributes((const wchar_t*)(u"D:\\VS2007Projects\\open_v2_test_data\\test_save_cmp.bin")); dwAttrib == INVALID_FILE_ATTRIBUTES) {
		


		ws.w.current_date = date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1));
		auto const p_to_t_vector = provinces::load_province_terrain_data(ws.s.province_m, fs.get_root());
		auto color_terrain_map = provinces::read_terrain_colors(ws.s.gui_m.text_data_sequences, ws.s.province_m, ws.s.modifiers_m, fs.get_root());
		provinces::assign_terrain_color(ws.w.province_s, p_to_t_vector, color_terrain_map);
		provinces::read_province_histories(ws, fs.get_root(), date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1)));
		population::read_all_pops(fs.get_root(), ws, date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1)));
		nations::init_empty_states(ws);
		std::vector<std::pair<nations::country_tag, events::decision_tag>> decisions;
		nations::read_nations_files(ws, date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1)), fs.get_root(), decisions);
		nations::read_diplomacy_files(ws, date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1)), fs.get_root());
		military::read_wars(ws, date_to_tag(boost::gregorian::date(1836, boost::gregorian::Jan, 1)), fs.get_root());
		
		ideologies::set_default_enabled_ideologies(ws);

		for(auto& ps : ws.w.province_s.province_state_container) {
			if(is_valid_index(ps.rgo_production))
				economy::match_rgo_worker_type(ws, ps);
		}

		provinces::update_province_demographics(ws);
		nations::update_state_nation_demographics(ws);

		provinces::set_base_rgo_size(ws);

		for(auto& ps : ws.w.province_s.province_state_container) {
			if(ps.owner && ps.state_instance) 
				economy::update_rgo_employment(ws, ps);
		}
		economy::init_factory_employment(ws);
		economy::init_artisan_producation(ws);

		population::default_initialize_world_issues_and_ideology(ws);

		provinces::update_province_demographics(ws);
		nations::update_state_nation_demographics(ws);

		nations::fix_capitals(ws);

		events::execute_decision_set(decisions, ws);

		economy::set_initial_money(ws);

		ws.w.local_player_nation = &ws.w.nation_s.nations[nations::country_tag(1)];

		provinces::add_province_modifier(
			ws,
			ws.w.province_s.province_state_container[provinces::province_tag(78)],
			modifiers::provincial_modifier_tag(1));
		provinces::add_timed_province_modifier(
			ws,
			ws.w.province_s.province_state_container[provinces::province_tag(78)],
			modifiers::provincial_modifier_tag(2),
			date_to_tag(boost::gregorian::date(1900, boost::gregorian::Jan, 10)));


		{
			serialization::serialize_file_header dummy;
			serialization::serialize_to_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_save_cmp.bin", true, dummy, ws.w, ws);
		}
	}

	serialization::deserialize_from_file(u"D:\\VS2007Projects\\open_v2_test_data\\test_save_cmp.bin", ws.w, ws);

	{
		auto mod_thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "global_liberal_agitation");
		auto nmod = ws.s.modifiers_m.named_national_modifiers_index[mod_thandle];
		modifiers::add_static_modifier_to_nation(ws, *ws.w.local_player_nation, nmod);
	}
	{
		auto mod_thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "army_tech_school");
		auto nmod = ws.s.modifiers_m.named_national_modifiers_index[mod_thandle];
		ws.w.local_player_nation->tech_school = nmod;

		
	}
	{
		auto thandle = text_data::get_existing_text_handle(ws.s.gui_m.text_data_sequences, "strategic_mobility");
		auto tech = ws.s.technology_m.named_technology_index[thandle];
		ws.w.local_player_nation->current_research = tech;
		ws.w.local_player_nation->research_points = 1500.0f;
	}
	{
		ws.w.current_crisis.type = current_state::crisis_type::liberation;
		ws.w.current_crisis.primary_attacker = ws.w.nation_s.nations.get_location(nations::country_tag(0));
		ws.w.current_crisis.primary_defender = ws.w.nation_s.nations.get_location(nations::country_tag(1));
		ws.w.current_crisis.on_behalf_of = ws.w.nation_s.nations.get_location(nations::country_tag(2));
		ws.w.current_crisis.target = ws.w.nation_s.nations.get_location(nations::country_tag(1));

		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders, nations::country_tag(1));
		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers, nations::country_tag(0));
		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.attackers, nations::country_tag(2));
		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.defenders, nations::country_tag(3));

		add_item(ws.w.military_s.war_goal_arrays, ws.w.current_crisis.goals, military::war_goal{date_tag(), 0.0f,
			nations::country_tag(2), nations::state_tag(0), nations::country_tag(1), nations::country_tag(4), military::cb_type_tag(1) });

		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, nations::country_tag(5));
		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, nations::country_tag(6));
		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, nations::country_tag(7));
		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, nations::country_tag(8));
		add_item(ws.w.nation_s.nations_arrays, ws.w.current_crisis.interested, nations::country_tag(9));

		ws.w.current_crisis.temperature = 85.0f;
	}

	{
		ws.w.local_player_nation->cb_construction_progress = 0.7f;
		ws.w.local_player_nation->cb_construction_target = nations::country_tag(6);
		ws.w.local_player_nation->cb_construction_type = military::cb_type_tag(4);

		ws.w.nation_s.nations[nations::country_tag(3)].cb_construction_progress = 0.6f;
		ws.w.nation_s.nations[nations::country_tag(3)].cb_construction_target = nations::country_tag(3);
		ws.w.nation_s.nations[nations::country_tag(3)].cb_construction_type = military::cb_type_tag(2);

		ws.w.nation_s.nations[nations::country_tag(7)].cb_construction_progress = 0.3f;
		ws.w.nation_s.nations[nations::country_tag(7)].cb_construction_target = nations::country_tag(1);
		ws.w.nation_s.nations[nations::country_tag(7)].cb_construction_type = military::cb_type_tag(1);
		ws.w.nation_s.nations[nations::country_tag(7)].flags |= nations::nation::cb_construction_discovered;
	}
	{
		auto srange = get_range(ws.w.nation_s.state_arrays, ws.w.local_player_nation->member_states);
		(srange.first)->state->project.type = economy::pop_project_type::factory;
		(srange.first)->state->project.factory_type = economy::factory_type_tag(1);

		(srange.first + 3)->state->project.type = economy::pop_project_type::railroad;
		(srange.first + 3)->state->project.location = provinces::province_tag(6);
	}

	ws.w.current_date = date_tag(to_index(ws.w.current_date) - 2);
	economy::economy_demand_adjustment_tick(ws);
	ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);
	economy::economy_demand_adjustment_tick(ws);
	ws.w.current_date = date_tag(to_index(ws.w.current_date) + 1);

	init_tooltip_window(ws.s.gui_m, ws.w.gui_m);
	ws.w.gui_m.on_resize(ui::resize{ 850ui32, 650ui32 });

	{
		const auto map_dir = fs.get_root().get_directory(u"\\map");
		
		auto map_peek = map_dir.peek_file(u"shadows.png");
		std::string shadows = map_peek ? UTF16toUTF8(map_peek->file_path() + u'\\' + map_peek->file_name()) : std::string();

		ui::window<gui_window_handler> test_window(850, 650, ws, shadows);

		std::cout << "test window created" << std::endl;
		getchar();

		test_window.close_window();
	}

	getchar();

	return 0;
}

